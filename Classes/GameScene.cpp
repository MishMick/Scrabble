#include "GameScene.h"
#include "SimpleAudioEngine.h"  

#include "LoseWindow.h"
#include "ConfirmWindow.h"

#include "InApp.h"

#include "Stack.h"
#include "DescriptionWindow.h"
#include "MenuItemWithDescription.h"

#include "ChatLayer.h"

#include "Letter.h"
#include "Field.h"
#include "Dictionary.h"
#include "Stack.h"
#include "Deck.h"
#include "SwapPanel.h"
#include "ChooseLetterWindow.h"
#include "ChangePlayerWindow.h"
#include "EndLevelWindow.h"

#include "MenuItemWithDescription.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "json/rapidjson.h"
#include "json/reader.h"
#include "json/document.h"

#include "xml.h"

//#include "SearchPopUp.h"

USING_NS_CC;

#define TAP_DELAY 0.5f

void my_itoa(int value, std::string& buf, int base){
	
    if (value == 0)
    {
        buf = "0";
        return;
    }
	int i = 30;
	
	buf = "";
	
	for(; value && i ; --i, value /= base) buf = "0123456789abcdef"[value % base] + buf;
}

CGameScene * CGameScene::_self = NULL;
float CGameScene::CHECKING_GAMES_UPDATE_TIME = 3.0f;
//////////////////////////////////////////////////////////////////////////
Scene* CGameScene::createScene()
{   
	Scene * scene = Scene::create();        
	Layer * layer = CGameScene::create();
	scene->addChild(layer);    

	return scene;
}

CGameScene* CGameScene::create() {
    CGameScene *obj = new CGameScene();
    obj->init();
    return obj;
}
//////////////////////////////////////////////////////////////////////////
bool CGameScene::init()
{
    
	_self = this;
    
    chatLayerOpen = false;
    
    zoomEnable = true;
    
    letterDragged = false;
    
    zoomedIn = false;
    zoomedOut = true;
    
    firstTapDone = false;
    tapDelay = 0.0f;
    
	m_Dictionary.Init();	
	m_ScoresForCurrentMove = 0;
	m_ActiveLetter = NULL;

    calledOnce = false;
    
	//m_Deck = NULL;
	//m_CurrentRoom = -1;
	m_IsChangingRooms = false;
	ACTIVE_PLAYER_COLOR = Color3B(255,255,0);
	WAITING_PLAYER_COLOR = Color3B(255,255,255);
	/*
	if (CCUserDefault::getInstance()->getBoolForKey("GAME_STARTED",false) == false)
	{
		CCUserDefault::getInstance()->setBoolForKey("GAME_STARTED",true);
		CCUserDefault::getInstance()->setIntegerForKey("LEVEL",0);
		CCUserDefault::getInstance()->setIntegerForKey("COINS",250);
	}
	*/
	m_FirstLaunch = true;

	if ( Layer::init() == false )
	{
		return false;
	}		
		
	m_Player = new CPlayer();
	m_Player->init();
	if(m_Player->IsHaveFacebookLogin())
	{
		InitGUI();
		m_Player->LoadCostList();
//      m_Player->CheckGamesUpdate();
//		StartGameUpdateTime();
//		StopGameUpdateTime();
	}
	else
	{
		CreateFacebookLoginBtn();
	}
    
    
    
    return true;
}
void CGameScene::BuyItem(const std::string randomItem)
{
    CCLOG("DUMMY METHOD!");
}

void CGameScene::showChatRelatedThing() {
    
    if (!calledOnce) {
        calledOnce = true;
        
        openChatButton = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(CGameScene::openChatLayer, this));
        openChatButton->setScale(0.5);
        openChatButton->setPosition(Vec2(975,725));
        openChatButton->setTag(1);
        
        closeChatButton = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(CGameScene::openChatLayer, this));
        closeChatButton->setScale(0.5);
        closeChatButton->setPosition(Vec2(750,725));
        closeChatButton->setTag(2);
        
        openChatButton->setOpacity(255);
        openChatButton->setEnabled(true);
        closeChatButton->setOpacity(0);
        closeChatButton->setEnabled(false);
        
        Menu *chatMenu = Menu::create(openChatButton,closeChatButton,NULL);
        chatMenu->setPosition(Vec2::ZERO);
        this->addChild(chatMenu,DESCRIPTION_LAYER);
    }
    
}

void CGameScene::addChatMessage(std::string msg) {
    if (chatLayerOpen) {
        ChatLayer::getInstance()->addMessageFromOpponent(msg);
    }
}

void CGameScene::openChatLayer(cocos2d::Ref *sender) {
    
    MenuItemImage *btn = (MenuItemImage*)sender;
    
    if(btn->getTag() == 1) {
        CCLOG("Open chat layer called");
        chatLayerOpen = true;
        
        openChatButton->setOpacity(0);
        openChatButton->setEnabled(false);
        closeChatButton->setOpacity(255);
        closeChatButton->setEnabled(true);
        
        chatlayer = ChatLayer::create();
        this->addChild(chatlayer, OVER_GUI_LAYER);
    }
    else {
        CCLOG("close chat layer called");
        
        if (ChatLayer::getInstance()->layerLoaded) {
            chatLayerOpen = false;
            
            openChatButton->setOpacity(255);
            openChatButton->setEnabled(true);
            closeChatButton->setOpacity(0);
            closeChatButton->setEnabled(false);
            
            chatlayer->cleanAll();
            
            this->removeChild(chatlayer);

        }
    }
    
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::CreateFacebookLoginBtn()
{
	m_FacebookLoginBtn = CCMenuItemImage::create(
		"rectangle.png",
		"rectangle_over.png",
		"rectangle_disable.png",
		this,
		menu_selector(CGameScene::FacebookLogin));
	m_FacebookLoginBtn->setScale(1.5);

	CCMenu* pMenu = CCMenu::create(m_FacebookLoginBtn, NULL);
	addChild(pMenu, GUI_LAYER);

	m_FacebookLoginLabel = LabelTTF::create("Facebook Login","helvetica",13);	
	m_FacebookLoginLabel->setColor(WAITING_PLAYER_COLOR);
	Size size = Size(m_FacebookLoginBtn->getContentSize().width,m_FacebookLoginBtn->getContentSize().height);
	m_FacebookLoginLabel->setPosition(size.width/2, size.height/2);
	m_FacebookLoginLabel->setAnchorPoint(Point(0.5,0.5f));
	m_FacebookLoginBtn->addChild(m_FacebookLoginLabel,GUI_LAYER);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::InitGUI()
{
	m_Background = Sprite::create("background.png");
	m_Background->setPosition(512,384);
	addChild(m_Background);
	m_Overlay = Sprite::create("over.png");
	m_Overlay->setPosition(512,384); 
	addChild(m_Overlay,GUI_LAYER);

	m_NewRoomButton = CMenuItemWithDesciption::create(
		"start_game.png",
		"start_game_over.png",
		"start_game_disable.png",
		this,
		menu_selector(CGameScene::CreateNewRoomOnOneComputer));
	m_NewRoomButton->setPosition(-410.0f,350.0f);
	m_NewRoomButton->SetDescription("Start a game with Facebook friend, mobile user, or random opponent");

	m_PlayButton = CMenuItemWithDesciption::create(
		"play.png",
		"play_over.png",
		"play_disable.png",
		this,
		menu_selector(CGameScene::NextTurn));
	m_PlayButton->setTag(IN_APP_PLAY);
	m_PlayButton->setScale(0.5f);
	m_PlayButton->setPosition(365.0f,-345.0f);
	m_PlayButton->setEnabled(false);
	m_PlayButton->setVisible(false);
	m_PlayButton->SetDescription("When your word is ready use this to submit your turn");

	m_RecallButton = CMenuItemWithDesciption::create(
		"recall.png",
		"recall_over.png",
		"recall_disable.png",
		this,
		menu_selector(CGameScene::Recall));
	m_RecallButton->setTag(IN_APP_RECALL);
	m_RecallButton->setScale(0.5f);
	//m_RecallButton->setPosition(445.0f,-345.0f);
	m_RecallButton->setPosition(-260.0f,-345.0f);
	m_RecallButton->setEnabled(false);
	m_RecallButton->setVisible(false);
	m_RecallButton->SetDescription("Return your tiles back to the tile rack");

	m_RematchButton = CMenuItemWithDesciption::create(
		"rematch.png",
		"rematch_over.png",
		"rematch_disable.png",
		this,
		menu_selector(CGameScene::Rematch));
	m_RematchButton->setTag(IN_APP_REMATCH);
	m_RematchButton->setScale(0.5f);
	m_RematchButton->setPosition(365.0f,-345.0f);
	m_RematchButton->setEnabled(false);
	m_RematchButton->setVisible(false);
	m_RematchButton->SetDescription("Didn't get enough the first time around? Challenge your opponent to another round!");

	//m_MoreButton = CCMenuItemImage::create(
	m_MoreButton = CCMenuItemImage::create(
		"more.png",
		"more_over.png",
		"more_disable.png",
		this,
		menu_selector(CGameScene::More));
	m_MoreButton->setTag(IN_APP_MORE);
	m_MoreButton->setScale(0.5f);
	m_MoreButton->setPosition(445.0f,-1000.0f);
	//m_MoreButton->setPosition(445.0f,-345.0f);
	m_MoreButton->setEnabled(false);

	m_LessButton = CCMenuItemImage::create(
		"less.png",
		"less_over.png",
		this,
		menu_selector(CGameScene::Less));
	m_LessButton->setTag(IN_APP_LESS);
	m_LessButton->setScale(0.5f);
	//m_LessButton->setPosition(-260.0f,-345.0f);
	m_LessButton->setPosition(445.0f,-345.0f);
	m_LessButton->setEnabled(false);
	m_LessButton->setVisible(false);

	m_SwapButton = CMenuItemWithDesciption::create(
		"swap.png",
		"swap_over.png",
		"swap_disable.png",
		this,
		menu_selector(CGameScene::Swap));
	m_SwapButton->setTag(IN_APP_SWAP);
	m_SwapButton->setScale(0.5f);
	m_SwapButton->setPosition(-180.0f,-345.0f);
	m_SwapButton->setEnabled(false);
	m_SwapButton->SetDescription("This will allow you to exchange tiles in your rack for new ones.");

	m_PassButton = CMenuItemWithDesciption::create(
		"pass.png",
		"pass_over.png",
		"pass_disable.png",
		this,
		menu_selector(CGameScene::Pass));
	m_PassButton->setTag(IN_APP_PASS);
	m_PassButton->setScale(0.5f);
	m_PassButton->setPosition(365.0f,-345.0f);
	m_PassButton->setEnabled(false);
	m_PassButton->SetDescription("Skip you turn and let your opponent play again.");
	
	m_ShuffleButton = CCMenuItemImage::create(
		"shuffle.png",
		"shuffle_over.png",
		"shuffle_disable.png",
		this,
		menu_selector(CGameScene::Shuffle));
	m_ShuffleButton->setTag(IN_APP_SHUFFLE);
	m_ShuffleButton->setScale(0.5f);
	//m_ShuffleButton->setPosition(445.0f,-345.0f);
	m_ShuffleButton->setPosition(-260.0f,-345.0f);
	m_ShuffleButton->setEnabled(false);
    
    searchButton = CMenuItemWithDesciption::create(   "button.png",
                                                      "button.png",
                                                      "button.png",
                                                      this,
                                                      menu_selector(CGameScene::searchUsers));
    searchButton->setPosition(-250.0f,350.0f);
    searchButton->SetDescription("Start a game with Facebook friend, mobile user, or random opponent");

    
	CCMenu* pMenu = CCMenu::create(m_NewRoomButton, m_PlayButton, m_RecallButton, m_MoreButton, m_LessButton, m_SwapButton, m_PassButton,m_ShuffleButton, m_RematchButton,searchButton, NULL);
    
    
    LabelTTF* searchLabel = LabelTTF::create("Search\nUser","helvetica",20);
    searchLabel->setColor(Color3B(255, 0, 0));
    
    searchLabel->setPosition(50,40);
    searchLabel->setAnchorPoint(Point(0.5,0.5f));
    searchButton->addChild(searchLabel,WAITING_WINDOW_LAYER+1);

    
	addChild(pMenu, GUI_LAYER);
	addChild(m_NewRoomButton->GetDescriptionWindow(),DESCRIPTION_LAYER);
	addChild(m_PlayButton->GetDescriptionWindow(),DESCRIPTION_LAYER);
	addChild(m_RecallButton->GetDescriptionWindow(),DESCRIPTION_LAYER);
	addChild(m_RematchButton->GetDescriptionWindow(),DESCRIPTION_LAYER);
	addChild(m_SwapButton->GetDescriptionWindow(),DESCRIPTION_LAYER);
	addChild(m_PassButton->GetDescriptionWindow(),DESCRIPTION_LAYER);
	m_NewRoomButton->AlignDescription(CDescriptionWindow::AT_BOTTOM);
	m_PlayButton->AlignDescription(CDescriptionWindow::AT_OVER);
	m_RecallButton->AlignDescription(CDescriptionWindow::AT_OVER);
	m_RematchButton->AlignDescription(CDescriptionWindow::AT_OVER);
	m_SwapButton->AlignDescription(CDescriptionWindow::AT_OVER);
	m_PassButton->AlignDescription(CDescriptionWindow::AT_OVER);

	m_SwapPanel = CSwapPanel::create();
	addChild(m_SwapPanel,UNDER_GUI_LAYER);

	m_ChooseLetterWindow = CChooseLetterWindow::create();
	addChild(m_ChooseLetterWindow,OVER_GUI_LAYER);

	m_ChangePlayerWindow = CChangePlayerWindow::create();
	addChild(m_ChangePlayerWindow,UNDER_GUI_LAYER);

	m_EndLevelWindow = CEndLevelWindow::create();
	addChild(m_EndLevelWindow, UNDER_GUI_LAYER);

	m_BonusPanel = CBonusPanel::create();
	m_BonusPanel->SetWordMetterState(m_Player->IsWordMetterOn());
	addChild(m_BonusPanel,UNDER_GUI_LAYER);
	addChild(m_BonusPanel->GetResignBtn()->GetDescriptionWindow(), DESCRIPTION_LAYER);
	addChild(m_BonusPanel->GetWordMetterOnBtn()->GetDescriptionWindow(), DESCRIPTION_LAYER);
	addChild(m_BonusPanel->GetWordMetterOffBtn()->GetDescriptionWindow(), DESCRIPTION_LAYER);
	m_BonusPanel->GetResignBtn()->AlignDescription(CDescriptionWindow::AT_OVER);
	m_BonusPanel->GetWordMetterOnBtn()->AlignDescription(CDescriptionWindow::AT_OVER);
	m_BonusPanel->GetWordMetterOffBtn()->AlignDescription(CDescriptionWindow::AT_OVER);


	m_Player_1_Name = LabelTTF::create("0","helvetica",18);	
	m_Player_1_Name->setColor(WAITING_PLAYER_COLOR);
	m_Player_1_Name->setPosition(512.0f,745.0f);
	m_Player_1_Name->setAnchorPoint(Point(1.0f,0.5f));
	addChild(m_Player_1_Name,GUI_LAYER);
	m_Player_1_Name->setVisible(false);

	m_Player_2_Name = LabelTTF::create("0","helvetica",18);	
	m_Player_2_Name->setColor(WAITING_PLAYER_COLOR);
	m_Player_2_Name->setPosition(512.0f,720.0f);
	m_Player_2_Name->setAnchorPoint(Point(1.0f,0.5f));
	addChild(m_Player_2_Name,GUI_LAYER);
	m_Player_2_Name->setVisible(false);

	m_Player_1_Score = LabelTTF::create("0","helvetica",18);	
	m_Player_1_Score->setColor(WAITING_PLAYER_COLOR);
	m_Player_1_Score->setPosition(516.0f,745.0f);
	m_Player_1_Score->setAnchorPoint(Point(0.0f,0.5f));
	addChild(m_Player_1_Score,GUI_LAYER);
	m_Player_1_Score->setVisible(false);

	m_Player_2_Score = LabelTTF::create("0","helvetica",18);	
	m_Player_2_Score->setColor(WAITING_PLAYER_COLOR);
	m_Player_2_Score->setPosition(516.0f,720.0f);
	m_Player_2_Score->setAnchorPoint(Point(0.0f,0.5f));
	addChild(m_Player_2_Score,GUI_LAYER);
	m_Player_2_Score->setVisible(false);

	m_StackLetterRemainCount = LabelTTF::create("0","helvetica",20);	
	m_StackLetterRemainCount->setColor(WAITING_PLAYER_COLOR);
	m_StackLetterRemainCount->setPosition(600.0f,745.0f);
	m_StackLetterRemainCount->setAnchorPoint(Point(0.0f,0.5f));
	m_StackLetterRemainCount->setString("0");
	addChild(m_StackLetterRemainCount,GUI_LAYER);
	m_StackLetterRemainCount->setVisible(false);

	m_StackLetterRemainLabel = LabelTTF::create("0","helvetica",16);	
	m_StackLetterRemainLabel->setColor(WAITING_PLAYER_COLOR);
	m_StackLetterRemainLabel->setPosition(600.0f,720.0f);
	m_StackLetterRemainLabel->setAnchorPoint(Point(0.0f,0.5f));
	m_StackLetterRemainLabel->setString("Letters remaining");
	addChild(m_StackLetterRemainLabel,GUI_LAYER);
	m_StackLetterRemainLabel->setVisible(false);

	for(int i=0;i<CRoomsTable::TT_COUNT;i++)
	{
		CRoomsTable* table = CRoomsTable::create();
		m_RoomsTables.push_back(table);
		addChild(table,GUI_LAYER);
	}
	m_RoomsTables[CRoomsTable::TT_YOUR_MOVE]->SetMarkerLabel("Your Move");
	m_RoomsTables[CRoomsTable::TT_THEIR_MOVE]->SetMarkerLabel("Their Move");
	m_RoomsTables[CRoomsTable::TT_FIND_OPPONENT]->SetMarkerLabel("Find Opponent");
	m_RoomsTables[CRoomsTable::TT_GAME_OVER]->SetMarkerLabel("Game Over");
    
    EventListenerTouchOneByOne* touchDelegate = EventListenerTouchOneByOne::create();
    touchDelegate->onTouchBegan = std::bind(&CGameScene::TouchBegan, this, std::placeholders::_1, std::placeholders::_2);
    touchDelegate->onTouchMoved = std::bind(&CGameScene::TouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    touchDelegate->onTouchCancelled = std::bind(&CGameScene::TouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
    touchDelegate->onTouchEnded = std::bind(&CGameScene::TouchEnded, this, std::placeholders::_1, std::placeholders::_2);
    
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(touchDelegate, this);

    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchDelegate, -256);
    
//    debug
//    this->setScale(1.5);
    
//    this->setPosition(Vec2(100, 100));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(CGameScene::onMouseMove,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
#endif

	this->schedule(schedule_selector(CGameScene::update),0.033f);
	CreateWaitingAnim();
}

void CGameScene::searchUsers(cocos2d::Ref *sender) {
    log("Search Users called");
    
    m_WaitingWindow->setShow(true);
    
  /*  SearchPopUp *popup = SearchPopUp::create();
    this->addChild(popup, WAITING_WINDOW_LAYER); */
    
  //  GetPlayer()->CheckGamesUpdate();
    
}

void CGameScene::enableGameScene(bool state) {
    zoomEnable = state;
    m_IsChangingRooms = state;
    m_NewRoomButton->setEnabled(state);
    GetCurrentRoom()->GetCurrentPlayerDeck()->setTouchEnabled(state);
    if (state) {
        BlockControlButtons(0);
    }
    else {
        UnblockControlButtons(0);
    }
    //m_WaitingWindow->setShow(!state);
    searchButton->setEnabled(state);
}

bool CGameScene::TouchBegan(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    
    oldPoint = touch->getLocationInView();
    newPoint = touch->getLocationInView();
    
    if (firstTapDone && tapDelay < TAP_DELAY && zoomEnable) {
        CCLOG("DOUBLE TAP DONE");
        
        if (zoomedOut) {
            zoomedOut = false;
            zoomedIn = true;
            this->runAction(ScaleTo::create(0.1, 1.5));
        }
        else if (zoomedIn) {
            zoomedOut = true;
            zoomedIn = false;
            this->setPosition(Vec2(0, 0));
            this->runAction(ScaleTo::create(0.1, 1.0));
        }
        
    }
    
    if (!firstTapDone) {
        firstTapDone = true;
    }
    
    return true;
}

void CGameScene::TouchMoved(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    Vec2 screenSize = Director::getInstance()->getWinSize();
    //log("Screen ");
    newPoint = touch->getLocationInView();
    
    if ((newPoint.x != oldPoint.x || newPoint.y != oldPoint.y) && zoomedIn && !letterDragged) {
        
        //CCLOG("Position X :: %f Y :: %f",this->getPositionX() + (newPoint.x - oldPoint.x),this->getPositionY() - (newPoint.y - oldPoint.y));
//        if (this->getPositionY() < ((screenSize.y * 0.25)) && this->getPositionY() > (-(screenSize.y * 0.25))) {
        
            this->setPosition(Vec2(this->getPositionX() + (newPoint.x - oldPoint.x), this->getPositionY() - (newPoint.y - oldPoint.y)));
//        }
        
    
    }
    
    oldPoint = newPoint;
}

void CGameScene::TouchEnded(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    oldPoint = touch->getLocationInView();
    newPoint = touch->getLocationInView();
}

void CGameScene::TouchCancelled(Touch* touch, Event* event)
{
    // Let the gesture recognizer to do its work
    
}

void CGameScene::searchPopupCallback() {
    m_WaitingWindow->setShow(false);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::CreateWaitingAnim()
{
	m_WaitingWindow = new CWaitingWindow();
	addChild(m_WaitingWindow,WAITING_WINDOW_LAYER);
	ShowWaitingAnim();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ShowWaitingAnim()
{
	m_WaitingWindow->Show();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::HideWaitingAnim()
{
	m_WaitingWindow->Hide();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::CheckWin(Node* sender)
{
	CRoom* room = GetCurrentRoom();
	if(room->GetPassInARowCount() >= 3 && room->GetState() != CRoom::RS_END_GAME)
	{
		room->SetState(CRoom::RS_END_GAME);
		ShowEndLevelWindow();
		if(room->GetTurn() == CRoom::RT_FIRST_PLAYER)
		{
			DeleteFromTable(CRoomsTable::TT_YOUR_MOVE, room->GetRoomButton());
		}
		else
		{
			DeleteFromTable(CRoomsTable::TT_THEIR_MOVE, room->GetRoomButton());
		}
		PutToTable(CRoomsTable::TT_GAME_OVER, room->GetRoomButton());
		RebuildTablesPos();
	}
	else
	{
		UnblockControlButtons(this);
		room->SetState(CRoom::RS_IN_GAME);
	}
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::EndCurrentRoom()
{
	CRoom* room = GetCurrentRoom();
	Recall(m_RecallButton);
	BlockControlButtons(this);
	m_PassButton->setVisible(false);
	m_PassButton->setEnabled(false);
	m_RematchButton->setVisible(true);
	m_RematchButton->setEnabled(true);
	room->SetState(CRoom::RS_END_GAME);
	if(room->GetTurn() == CRoom::RT_FIRST_PLAYER)
	{
		DeleteFromTable(CRoomsTable::TT_YOUR_MOVE, room->GetRoomButton());
	}
	else
	{
		DeleteFromTable(CRoomsTable::TT_THEIR_MOVE, room->GetRoomButton());
	}
	PutToTable(CRoomsTable::TT_GAME_OVER, room->GetRoomButton());
	RebuildTablesPos();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SetLetterUsed(CLetter * letter)
{
	if (std::find(m_UsedLetters.begin(), m_UsedLetters.end(), letter) == m_UsedLetters.end())
	{
		m_UsedLetters.push_back(letter);
	}
	CRoom* room = GetCurrentRoom();
	if(m_UsedLetters.size() == 1 && room->GetState() == CRoom::RS_IN_GAME)
	{
		if(room->GetTurn() == CRoom::RT_FIRST_PLAYER)
		{
			SetVisiblePlayButton(true);
		}
		else if(room->GetTurn() == CRoom::RT_OPPONENT)
		{
			m_ShuffleButton->setVisible(false);
			m_ShuffleButton->setEnabled(false);
			m_RecallButton->setVisible(true);
			m_RecallButton->setEnabled(true);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SetLetterNotUsed(CLetter * letter)
{
	m_UsedLettersIter = std::find(m_UsedLetters.begin(), m_UsedLetters.end(), letter);
	if (m_UsedLettersIter != m_UsedLetters.end())
	{
		m_UsedLetters.erase(m_UsedLettersIter);
	}
	CRoom* room = GetCurrentRoom();
	if(m_UsedLetters.size() == 0 && room->GetState() == CRoom::RS_IN_GAME)
	{
		if(room->GetTurn() == CRoom::RT_FIRST_PLAYER)
		{
			SetVisiblePlayButton(false);
		}
		else if(room->GetTurn() == CRoom::RT_OPPONENT)
		{
			m_ShuffleButton->setVisible(true);
			m_ShuffleButton->setEnabled(true);
			m_RecallButton->setVisible(false);
			m_RecallButton->setEnabled(false);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
bool SortVerticalLetters(CLetter * a, CLetter * b)
{
	return a->GetCell()->y < b->GetCell()->y;
}
bool SortHorizontalLetters(CLetter * a, CLetter * b)
{
	return a->GetCell()->x < b->GetCell()->x;
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::NextTurn(Object* sender)
{
	CRoom* room = GetCurrentRoom();
	if (m_UsedLetters.size() == 0)
	{
		MessageBox("You should place some letters on field!","Warning!");
		return;
	}

	//CField* field = GetField();
	CField* field = GetCurrentRoom()->GetField();
	m_ScoresForCurrentMove = 0;
	
	if(field->IsFirstWordPlased() == false && IsHaveLetterOnStar() == false)
	{
		MessageBox("The first word played must cross the center star.","Warning!");
		return;
	}
	///Check vertical
	bool allVertical = true;
	int x = -1;
	for (size_t i=0; i<m_UsedLetters.size(); i++)
	{
		SGridCell * cell = m_UsedLetters[i]->GetCell();
		if (x == -1)
		{
			x = cell->x;
		}
		else
		{
			if (cell->x != x)
			{
				allVertical = false;
				break;
			}
		}		
	}
	if(m_UsedLetters.size() == 1)
	{
		SGridCell * cell = m_UsedLetters[0]->GetCell();
		Point cellPos = Point(cell->x,cell->y);
		bool haveLetterLeft = false;
		bool haveLetterRight = false;
		if(cellPos.x > 0)
		{
			cell = field->GetCell(cellPos.x-1,cellPos.y);
			if(cell->letter != -1)
			{
				haveLetterLeft = true;
			}
		}
		if(cellPos.x < CField::GRID_WIDTH -2)
		{
			cell = field->GetCell(cellPos.x+1,cellPos.y);
			if(cell->letter != -1)
			{
				haveLetterRight = true;
			}
		}
		if(haveLetterLeft || haveLetterRight)
		{
			allVertical = false;
		}
	}

	if (allVertical)
	{			
		if(IsUsedLettersInColumn() == false)
		{
			MessageBox("Must place all tiles in one row or column.","Warning!");
			return;
		}
		std::sort(m_UsedLetters.begin(), m_UsedLetters.end(), SortVerticalLetters);
		int x = m_UsedLetters.front()->GetCell()->x;
		int y = m_UsedLetters.front()->GetCell()->y;
		int startY = m_UsedLetters.front()->GetCell()->y;
		int endY =  m_UsedLetters.back()->GetCell()->y;
		bool isValidPlacement = field->CheckVerticalValid(m_UsedLetters.front()->GetCell()->x, startY, endY);
		if (!isValidPlacement)
		{
			MessageBox("You cannot have any gaps in placed tiles.","Warning!");
			return;
		}
		bool isTouchWithFixed = field->CheckVerticalFixedTouch(x,startY,endY);
		if(!isTouchWithFixed)
		{
			MessageBox("Al least one placed tile must touch an existing tile.", "Warning!");
			return;
		}

		std::string word = field->GetWordInColumn(x,startY, m_ScoresForCurrentMove);
		if(m_Dictionary.FindWord(word) == false)
		{
			word = '"' + word + '"';
			m_WrongWords.push_back(word);
		}
		else
		{
			m_FindedWords.push_back(word);
			for (size_t i = 0; i<m_UsedLetters.size(); i++)
			{
				ModifyLetterScores(m_UsedLetters[i]->GetCell()->modifier, m_UsedLetters[i]->GetCost(), m_ScoresForCurrentMove);
			}
			for (size_t i = 0; i<m_UsedLetters.size(); i++)
			{
				ModifyWordScores(m_UsedLetters[i]->GetCell()->modifier, m_ScoresForCurrentMove);
			}
		}
		for(int i=0;i<m_UsedLetters.size();i++)
		{
			SGridCell * cell = m_UsedLetters[i]->GetCell();
			int addScore = 0;
			word = field->GetWordInRow(cell->y,cell->x-1, addScore);				
			if (word.size() > 1)
			{
				if (m_Dictionary.FindWord(word) == false)
				{
					word = '"' + word + '"';
					m_WrongWords.push_back(word);
				}
				else
				{
					m_FindedWords.push_back(word);
					ModifyLetterScores(cell->modifier, m_UsedLetters[i]->GetCost(), addScore);
					ModifyWordScores(cell->modifier, addScore);
					m_ScoresForCurrentMove += addScore;
				}
			}
		}

		if(m_WrongWords.empty())
		{
            CheckWordsOnServer();
            return;
		}
		else
		{
			ShowWrongWords();
			return;
		}
	}	
	
	bool allHorizontal = true;
	int y = -1;
	for (size_t i=0; i<m_UsedLetters.size(); i++)
	{
		SGridCell * cell = m_UsedLetters[i]->GetCell();
		if (y == -1)
		{
			y = cell->y;
		}
		else
		{
			if (cell->y != y)
			{
				allHorizontal = false;
				break;
			}
		}		
	}
	if (allHorizontal)
	{			
		if(IsUsedLettersInRow() == false)
		{
			MessageBox("Must place all tiles in one row or column.","Warning!");
			return;
		}
		std::sort(m_UsedLetters.begin(), m_UsedLetters.end(), SortHorizontalLetters);
		int x = m_UsedLetters.front()->GetCell()->x;
		int y = m_UsedLetters.front()->GetCell()->y;
		int startX = m_UsedLetters.front()->GetCell()->x;
		int endX =  m_UsedLetters.back()->GetCell()->x;
		bool isValidPlacement = field->CheckHorizontalValid(m_UsedLetters.front()->GetCell()->y, startX, endX);
		if (!isValidPlacement)
		{
			MessageBox("You cannot have any gaps in placed tiles.","Warning!");
			return;
		}
		bool isTouchWithFixed = field->CheckHorizontalFixedTouch(y,startX,endX);
		if(!isTouchWithFixed)
		{
			MessageBox("Al least one placed tile must touch an existing tile.", "Warning!");
			return;
		}

		std::string word = field->GetWordInRow(y,startX, m_ScoresForCurrentMove);
		if(m_Dictionary.FindWord(word) == false)
		{
			word = '"' + word + '"';
			m_WrongWords.push_back(word);
		}
		else
		{
			m_FindedWords.push_back(word);
			for (size_t i = 0; i<m_UsedLetters.size(); i++)
			{
				ModifyLetterScores(m_UsedLetters[i]->GetCell()->modifier, m_UsedLetters[i]->GetCost(), m_ScoresForCurrentMove);
			}
			for (size_t i = 0; i<m_UsedLetters.size(); i++)
			{
				ModifyWordScores(m_UsedLetters[i]->GetCell()->modifier, m_ScoresForCurrentMove);
			}
		}
		for(int i=0;i<m_UsedLetters.size();i++)
		{
			SGridCell * cell = m_UsedLetters[i]->GetCell();
			int addScore = 0;
			word = field->GetWordInColumn(cell->x,cell->y-1, addScore);				
			if (word.size() > 1)
			{
				if (m_Dictionary.FindWord(word) == false)
				{
					word = '"' + word + '"';
					m_WrongWords.push_back(word);
				}
				else
				{
					m_FindedWords.push_back(word);
					ModifyLetterScores(cell->modifier, m_UsedLetters[i]->GetCost(), addScore);
					ModifyWordScores(cell->modifier, addScore);
					m_ScoresForCurrentMove += addScore;
				}
			}
		}

		if(m_WrongWords.empty())
		{
			CheckWordsOnServer();
            return;
		}
		else
		{
			ShowWrongWords();
			return;
		}
	}	

	if (!allHorizontal && !allVertical)
	{
		MessageBox("Must place all tiles in one row or column.","Warning!");
		ClearWordsContainers();
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::CheckWordsOnServer()
{
	ShowWaitingAnim();
	m_Player->CheckWordsOnServer(m_FindedWords);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ShowWrongWords()
{
	std::string mistakes;
	if(m_WrongWords.size() == 1)
	{
		mistakes += m_WrongWords[0];
	}
	else if(m_WrongWords.size() == 2)
	{
		mistakes = m_WrongWords[0] + " and " + m_WrongWords[1];
	}
	else
	{
		mistakes += m_WrongWords[0];
		for(int i=1;i<m_WrongWords.size();i++)
		{
			if(i < m_WrongWords.size()-1)
			{
				mistakes += ", " + m_WrongWords[i];
			}
			else
			{
				mistakes += " and " + m_WrongWords[i];
			}
		}
	}
	mistakes += " may be misspelled or may be proper nouns!";
	MessageBox(mistakes.c_str(), "Warning!");
	ClearWordsContainers();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SuccessServerCheck()
{
	m_WaitingWindow->Hide(true);
	ShowSendMoveWindow();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::FailServerCheck()
{
	m_WaitingWindow->Hide();
	ClearWordsContainers();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SuccessCleanup(std::vector<std::string> words, const int score)
{
	CRoom* room = GetCurrentRoom();
	CField* field = room->GetField();
	//CField* field = GetField();
    for (size_t i = 0; i<m_UsedLetters.size(); i++)
	{
		m_UsedLetters[i]->SetFixed();
	}
	if(field->IsFirstWordPlased() == false)
	{
		field->SetPlasedFirstWord(true);
	}
	m_UsedLetters.clear();
	room->AddCurrentPlayerScore(score);
	float currentDelay = 0.2f;
	room->GetCurrentPlayerDeck()->AlignLetter();
	currentDelay += room->GetCurrentPlayerDeck()->InitEmptyCells(0.15f);
	SetVisiblePlayButton(false);
	CallFuncN *blockButtonsFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::BlockControlButtons));
	Spawn * spawn = Spawn::create(blockButtonsFunc,NULL);
	runAction(spawn);
	room->CleanupPassInRow();
	NextPlayerTurn(currentDelay);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ModifyLetterScores(Modifier modifier, int letterCost, int &scores)
{
	if (modifier == M_Plus5)scores += 5;
	else if (modifier == M_Plus2)scores += 2;
	else if (modifier == M_Plus7)scores += 7;
	else if (modifier == M_Plus10)scores += 10;
	else if (modifier == M_Plus15)scores += 15;
	else if (modifier == M_Plus20)scores += 20;
	else if (modifier == M_Plus25)scores += 25;
	else if (modifier == M_Plus30)scores += 30;
	else if (modifier == M_Minus5)scores -= 5;
	else if (modifier == M_Minus7)scores -= 7;
	else if (modifier == M_Minus10)scores -= 10;
	else if (modifier == M_Minus15)scores -= 15;
	else if (modifier == M_Minus20)scores -= 20;
	else if (modifier == M_Minus25)scores -= 25;
	else if (modifier == M_Minus30)scores -= 30;
	else if (modifier == M_DoubleLetter)scores += letterCost;
	else if (modifier == M_TripleLetter)scores += letterCost*2;
	else if (modifier == M_QuadLetter)scores += letterCost*3;
}


//////////////////////////////////////////////////////////////////////////
void CGameScene::ModifyWordScores(Modifier modifier, int &scores)
{
	if (modifier == M_DoubleWord)scores *= 2;
	else if (modifier == M_TripleWord)scores *= 3;
	else if (modifier == M_QuadWord)scores *= 4;	
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::CreateNewRoomOnOneComputer(Ref* sender)
{
    
    if (CGameScene::Instance()->GetPlayer()->getRooms().size() >= 5) {
        MessageBox("You have reached max limit of playing game.", "Finish Games");
        return;
    }
    
	ShowWaitingAnim();
	StopGameUpdateTime();
	m_Player->CreateNewRoom();
	/*
	int room_id = m_Player->CreateNewRoomOnOneComputer();
	if(room_id == -1)
	{
		return;
	}
	ChangeRoom(room_id);
	CRoom* room = m_Player->GetCurrentRoom();
	PutToTable(CRoomsTable::TT_YOUR_MOVE,room->GetRoomButton());
	RebuildTablesPos();*/
	/*
	if(m_Rooms.size() >= MAX_ROOM_COUNT)
	{
		return;
	}
	
	CRoom * room = CRoom::create();
	room->retain();

	m_Rooms.push_back(room);
	
	int room_id = m_Rooms.size()-1;
	room->SetId(room_id);

	ChangeRoom(room_id);
	PutToTable(CRoomsTable::TT_YOUR_MOVE,room->GetRoomButton());
	RebuildTablesPos();
	*/
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::CreateNewRoomWithEnemy(Ref* sender)
{
	int room_id = m_Player->CreateNewRoomWithOpponent();
	if(room_id == -1)
	{
		return;
	}
	/*
	if(m_Rooms.size() >= MAX_ROOM_COUNT)
	{
		return;
	}*/
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ChangeRoom(int id)
{
	m_IsChangingRooms = true;
	float delay = 0.0f;
	//if(m_CurrentRoom != -1)
	if(m_Player->GetCurrentRoomId() != -1)
	{
		CRoom * room  = m_Player->GetCurrentRoom();
		Recall(m_RecallButton);
		if(room->GetState() == CRoom::RS_SWAP_PANEL_SHOW)
		{
			GetSwapPanel()->CancelSwap();
		}
		if(room->GetState() == CRoom::RS_BONUS_PANEL_SHOW)
		{
			HideBonusPanel();
		}
		if(GetEndLevelWindow()->IsShow())
		{
			GetEndLevelWindow()->Hide();
		}
		HideCurrentRoom(this);
		delay+= CField::SHOW_TIME;
	}
	auto delayAction = DelayTime::create(delay);
	auto blockButtonsFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::BlockControlButtons));
	auto changingCompleteFunc = CallFuncN::create(this,callfuncN_selector(CGameScene::ChangingRoomsComplete));
	//m_CurrentRoom = id;
	m_Player->SetCurrentRoomId(id);
	auto showRoomFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::ShowCurrentRoom));
	auto delayAction2 = DelayTime::create(CLetter::SHOW_ACTION_TIME*7);
	auto unblockButtonsFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::UnblockControlButtons));
	Sequence * sequence = Sequence::create(blockButtonsFunc,delayAction,showRoomFunc,delayAction2,unblockButtonsFunc,changingCompleteFunc,NULL);
	runAction(sequence);
	m_Player->GetCurrentRoom()->GetRoomButton()->setEnable(false);
	RebuildRoomLabels();
}

//////////////////////////////////////////////////////////////////////////
CSwapPanel* CGameScene::GetSwapPanel()
{
	return m_SwapPanel;
}

//////////////////////////////////////////////////////////////////////////
Sprite* CGameScene::GetOverlay()
{
	return m_Overlay;
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::FacebookLogin(Ref* sender)
{
	FacebookLoginComplete();
}

void CGameScene::RemoveFacebookBtn()
{
	m_FacebookLoginBtn->setVisible(false);
	//m_FacebookLoginBtn->removeAllChildren();
	//m_FacebookLoginBtn->getParent()->removeAllChildren();
	//m_FacebookLoginBtn->getParent()->removeFromParent();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::FacebookLoginComplete()
{
	RemoveFacebookBtn();
	InitGUI();
	m_Player->CreateNewFacebookPlayer("NEW", 13);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::Recall(Object* sender)
{
	MakeRecall();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::MakeRecall()
{
	if(m_UsedLetters.empty())
	{
		return;
	}

	CRoom* room = m_Player->GetCurrentRoom();
	room->GetCurrentPlayerScorePointer()->Hide();
	//GetCurrentPlayerScorePointer()->Hide();

	while(!m_UsedLetters.empty())
	{
		CLetter * letter = m_UsedLetters[0];
		letter->UseRecall();
		//GetDeck()->PutLetterToEmptyCell(letter);
		room->GetCurrentPlayerDeck()->PutLetterToEmptyCell(letter);
		m_UsedLetters.erase(m_UsedLetters.begin());
	}
	if(room->GetTurn() == CRoom::RT_FIRST_PLAYER)
	{
		SetVisiblePlayButton(false);
	}
	else
	{
		m_RecallButton->setVisible(false);
		m_RecallButton->setEnabled(false);
		m_ShuffleButton->setVisible(true);
		m_ShuffleButton->setEnabled(true);
	}
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::Shuffle(Object* sender)
{
	m_Player->GetCurrentRoom()->GetCurrentPlayerDeck()->MakeShuffle();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::More(Object* sender)
{
	ShowBonusPanel();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::Less(Object* sender)
{
	HideBonusPanel();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::Swap(Object* sender)
{
	if(m_Player->GetCurrentRoom()->GetState() == CRoom::RS_BONUS_PANEL_SHOW)
	{
		HideBonusPanel();
	}
	m_SwapPanel->Show();
	CallFuncN *blockButtonsFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::BlockControlButtons));
	Spawn* spawn = Spawn::create(blockButtonsFunc,NULL);
	runAction(spawn);
	m_Player->GetCurrentRoom()->SetState(CRoom::RS_SWAP_PANEL_SHOW);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::Pass(Object* sender)
{
	ShowPassConfirm();
	//m_Player->GetCurrentRoom()->IncreasePassInARowCount();
	//NextPlayerTurn();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ShowPassConfirm()
{
	CConfirmWindow* passConfirmWindow = CConfirmWindow::create();
	passConfirmWindow->SetConfirmLabel("Are you sure you want to pass your turn?");
	passConfirmWindow->SetOkLabel("Pass");
	passConfirmWindow->SetCancelLabel("Cancel");
	passConfirmWindow->SetOkCallback(this,callfuncN_selector(CGameScene::PassConfirmOkCallback));
	addChild(passConfirmWindow);
	passConfirmWindow->Show();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::PassConfirmOkCallback(Node* sender)
{
	PLAYER_MOVE move;
	CPlayer* player = CGameScene::Instance()->GetPlayer();
	move.room_server_id = player->GetCurrentRoom()->GetServerId();
	player->SendPassMoveToServer(move);
	CGameScene::Instance()->SetOpponentTurn();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SetOpponentTurn()
{
	CRoom* room = m_Player->GetCurrentRoom();
	CRoomButton* roomButton = room->GetRoomButton();
	room->SetTurn(CRoom::RT_OPPONENT);
	DeleteFromTable(CRoomsTable::TT_YOUR_MOVE,roomButton);
	PutToTable(CRoomsTable::TT_THEIR_MOVE,roomButton);
	m_PassButton->setEnabled(false);
	m_SwapButton->setEnabled(false);
	RebuildTablesPos();
	RebuildRoomLabels();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SendSwapMove(float delay)
{
	PLAYER_MOVE move;
	CPlayer* player = CGameScene::Instance()->GetPlayer();
	CRoom* room = player->GetCurrentRoom();
	room->GetCurrentPlayerDeck()->setTouchEnabled(false);
	DelayTime* delayAction = DelayTime::create(delay);
	CallFuncN *unblockDeck = CallFuncN::create(this, callfuncN_selector(CGameScene::UnblockDeck));
	Sequence* sequence = Sequence::create(delayAction, unblockDeck, NULL);
	move.room_server_id = player->GetCurrentRoom()->GetServerId();
	move.deck = room->GetFirstPlayerDeck()->GetDeckInString();
	move.stack = room->GetStack()->GetStackInString();
	player->SendPassMoveToServer(move,true);
	room->SetState(CRoom::RS_IN_GAME);
	SetOpponentTurn();
	runAction(sequence);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SetStackRemainLabel(int count)
{
	m_StackLetterRemainCount->setString(StringUtils::toString(count));
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SetVisiblePlayButton(bool visible)
{
	m_PlayButton->setEnabled(visible);
	m_RecallButton->setEnabled(visible);
	m_ShuffleButton->setEnabled(!visible);
	m_PassButton->setEnabled(!visible);
	m_PlayButton->setVisible(visible);
	m_RecallButton->setVisible(visible);
	m_ShuffleButton->setVisible(!visible);
	m_PassButton->setVisible(!visible);

	m_SwapButton->setEnabled(!visible);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::NextPlayerTurn(float delay)
{
	CRoom * room = m_Player->GetCurrentRoom();
	room->SwapScorePointerState();
	m_Player->GetCurrentRoom()->SetState(CRoom::RS_CHANGE_PLAYERS);
	if(room->GetType() == CRoom::RT_ONE_COMPUTER)
	{		
		float startDelay = delay;
		DelayTime *delayAction = DelayTime::create(delay);
		DelayTime *delayShowLettersAction = DelayTime::create(CLetter::SHOW_ACTION_TIME * (room->GetCurrentPlayerDeck()->GetLettersCount()+1));
		CallFuncN *switchPlayersFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::SwitchPlayers));
		CallFuncN *showDeckFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::ShowCurrentDeck));
		CallFuncN *hideDeckFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::HideCurrentDeck));
		CallFuncN *blockButtonsFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::BlockControlButtons));
		CallFuncN *checkWinFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::CheckWin));
		CallFuncN *showChangePlayerFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::ShowChangePlayerWindow));
		CallFuncN *hideChangePlayerFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::HideChangePlayerWindow));

		Sequence * sequence = Sequence::create(delayAction,showChangePlayerFunc,blockButtonsFunc,hideDeckFunc,delayShowLettersAction,switchPlayersFunc,
												showDeckFunc,delayShowLettersAction,hideChangePlayerFunc,checkWinFunc,NULL);
		runAction(sequence);
	}
	RebuildRoomLabels();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SwitchPlayers(Node* sender)
{
	CRoom * room = m_Player->GetCurrentRoom();
	bool showFirst = true;
	if(room->GetTurn() == CRoom::RT_FIRST_PLAYER)
	{
		DeleteFromTable(CRoomsTable::TT_YOUR_MOVE,room->GetRoomButton());
		PutToTable(CRoomsTable::TT_THEIR_MOVE,room->GetRoomButton());
		showFirst = false;
		room->SetTurn(CRoom::RT_SECOND_PLAYER);
		m_Player_1_Name->setColor(WAITING_PLAYER_COLOR);
		m_Player_1_Score->setColor(WAITING_PLAYER_COLOR);
		m_Player_2_Name->setColor(ACTIVE_PLAYER_COLOR);
		m_Player_2_Score->setColor(ACTIVE_PLAYER_COLOR);
	}
	else
	{
		DeleteFromTable(CRoomsTable::TT_THEIR_MOVE,room->GetRoomButton());
		PutToTable(CRoomsTable::TT_YOUR_MOVE,room->GetRoomButton());
		room->SetTurn(CRoom::RT_FIRST_PLAYER);
		m_Player_1_Name->setColor(ACTIVE_PLAYER_COLOR);
		m_Player_1_Score->setColor(ACTIVE_PLAYER_COLOR);
		m_Player_2_Name->setColor(WAITING_PLAYER_COLOR);
		m_Player_2_Score->setColor(WAITING_PLAYER_COLOR);
	}
	room->GetFirstPlayerDeck()->setTouchEnabled(showFirst);
	room->GetFirstPlayerDeck()->setVisible(showFirst);
	room->GetSecondPlayerDeck()->setTouchEnabled(!showFirst);
	room->GetSecondPlayerDeck()->setVisible(!showFirst);
	RebuildTablesPos();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ShowChangePlayerWindow(Node* sender)
{
	CRoom* room = m_Player->GetCurrentRoom();
	std::string playerName = "Player 1";
	if(room->GetTurn() == CRoom::RT_FIRST_PLAYER)
	{
		playerName = room->GetSecondPlayerName();
	}
	else
	{
		playerName = room->GetFirstPlayerName();
	}
	m_ChangePlayerWindow->Show(playerName);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::HideChangePlayerWindow(Node* sender)
{
	m_ChangePlayerWindow->Hide();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::BlockControlButtons(Node* sender)
{
	m_PassButton->setEnabled(false);
	m_ShuffleButton->setEnabled(false);
	m_SwapButton->setEnabled(false);
	m_MoreButton->setEnabled(false);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::UnblockControlButtons(Node * sender)
{
	CRoom* room = m_Player->GetCurrentRoom();
	if(room->GetState() == CRoom::RS_END_GAME)
	{
		m_PassButton->setVisible(false);
		m_RematchButton->setVisible(true);
		m_RematchButton->setEnabled(true);
	}
	else
	{
		if(room->GetState() == CRoom::RS_SWAP_PANEL_SHOW)
		{
			room->SetState(CRoom::RS_IN_GAME);
		}
		if(room->GetTurn() == CRoom::RT_FIRST_PLAYER)
		{
			m_PassButton->setEnabled(true);
			m_PassButton->setVisible(true);
			if(room->GetStack()->IsEmpty() == false)
			{
				m_SwapButton->setEnabled(true);
			}
		}
		m_ShuffleButton->setEnabled(true);
		m_MoreButton->setEnabled(true);
	}
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ShowCurrentDeck(Node * sender)
{
	m_Player->GetCurrentRoom()->GetCurrentPlayerDeck()->ShowLetters();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::HideCurrentDeck(Node * sender)
{
	m_Player->GetCurrentRoom()->GetCurrentPlayerDeck()->HideLetters();
}

//////////////////////////////////////////////////////////////////////////
bool CGameScene::IsHaveLetterOnStar()
{
	if(m_UsedLetters.empty()) return false;
	for(int i=0;i<m_UsedLetters.size();i++)
	{
		if(m_UsedLetters[i]->GetCell()->modifier == M_StartPlace)
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CGameScene::IsUsedLettersInRow()
{
	if(m_UsedLetters.empty())
	{
		return false;
	}

	int y = -1;
	for(int i=0;i<m_UsedLetters.size();i++)
	{
		if(y == -1)
		{
			y = m_UsedLetters[i]->GetCell()->y;
		}
		else if(m_UsedLetters[i]->GetCell()->y != y)
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CGameScene::IsUsedLettersInColumn()
{
	if(m_UsedLetters.empty())
	{
		return false;
	}

	int x = -1;
	for(int i=0;i<m_UsedLetters.size();i++)
	{
		if(x == -1)
		{
			x = m_UsedLetters[i]->GetCell()->x;
		}
		else if(m_UsedLetters[i]->GetCell()->x != x)
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::CalculateScorePointer()
{
	if(m_UsedLetters.empty())
	{
		return;
	}

	CRoom* room = m_Player->GetCurrentRoom();
	CField* field = room->GetField();
	Point PointerPos;
	int score = 0;

	if(field->IsFirstWordPlased() == false && IsHaveLetterOnStar() == false)
	{
		return;
	}
	///Check vertical
	bool allVertical = IsUsedLettersInColumn();
	if(m_UsedLetters.size() == 1)
	{
		SGridCell * cell = m_UsedLetters[0]->GetCell();
		Point cellPos = Point(cell->x,cell->y);
		bool haveLetterLeft = false;
		bool haveLetterRight = false;
		if(cellPos.x > 0)
		{
			cell = field->GetCell(cellPos.x-1,cellPos.y);
			if(cell->letter != -1)
			{
				haveLetterLeft = true;
			}
		}
		if(cellPos.x < CField::GRID_WIDTH -2)
		{
			cell = field->GetCell(cellPos.x+1,cellPos.y);
			if(cell->letter != -1)
			{
				haveLetterRight = true;
			}
		}
		if(haveLetterLeft || haveLetterRight)
		{
			allVertical = false;
		}
		else
		{
			bool haveLetterUp = false;
			bool haveLetterDown  = false;
			if(cellPos.y > 0)
			{
				cell = field->GetCell(cellPos.x,cellPos.y-1);
				if(cell->letter != -1)
				{
					haveLetterUp = true;
				}
			}
			if(cellPos.y < CField::GRID_HEIGHT -2)
			{
				cell = field->GetCell(cellPos.x,cellPos.y+1);
				if(cell->letter != -1)
				{
					haveLetterDown = true;
				}
			}
			if(haveLetterUp == false && haveLetterDown == false)
			{
				return;
			}
		}
	}

	if (allVertical)
	{		
		std::sort(m_UsedLetters.begin(), m_UsedLetters.end(), SortVerticalLetters);
		int x = m_UsedLetters.front()->GetCell()->x;
		int y = m_UsedLetters.front()->GetCell()->y;
		int startY = m_UsedLetters.front()->GetCell()->y;
		int endY =  m_UsedLetters.back()->GetCell()->y;
		bool isValidPlacement = field->CheckVerticalValid(m_UsedLetters.front()->GetCell()->x, startY, endY);
		if (!isValidPlacement)
		{
			return;
		}
		bool isTouchWithFixed = field->CheckVerticalFixedTouch(x,startY,endY);
		if(!isTouchWithFixed)
		{
			return;
		}

		score = 0;
		std::string word = field->GetWordInColumn(x,startY, score);
		for (size_t i = 0; i<m_UsedLetters.size(); i++)
		{
			ModifyLetterScores(m_UsedLetters[i]->GetCell()->modifier, m_UsedLetters[i]->GetCost(), score);
		}
		for (size_t i = 0; i<m_UsedLetters.size(); i++)
		{
			ModifyWordScores(m_UsedLetters[i]->GetCell()->modifier, score);
		}
		
		for(int i=0;i<m_UsedLetters.size();i++)
		{
			SGridCell * cell = m_UsedLetters[i]->GetCell();
			int addScore = 0;
			word = field->GetWordInRow(cell->y,cell->x-1, addScore);				
			if (word.size() > 1)
			{
				ModifyLetterScores(cell->modifier, m_UsedLetters[i]->GetCost(), addScore);
				ModifyWordScores(cell->modifier, addScore);
				score += addScore;	
			}
		}
		SGridCell * cell = m_UsedLetters[m_UsedLetters.size()-1]->GetCell();
		PointerPos = field->GetUtmostVerticalCellPos(cell->x,cell->y);
		ScorePointCalculateComplete(score,PointerPos);
		return;
	}
	bool allHorizontal = IsUsedLettersInRow();
	
	if (allHorizontal)
	{			
		std::sort(m_UsedLetters.begin(), m_UsedLetters.end(), SortHorizontalLetters);
		int x = m_UsedLetters.front()->GetCell()->x;
		int y = m_UsedLetters.front()->GetCell()->y;
		int startX = m_UsedLetters.front()->GetCell()->x;
		int endX =  m_UsedLetters.back()->GetCell()->x;
		bool isValidPlacement = field->CheckHorizontalValid(m_UsedLetters.front()->GetCell()->y, startX, endX);
		if (!isValidPlacement)
		{
			return;
		}
		bool isTouchWithFixed = field->CheckHorizontalFixedTouch(y,startX,endX);
		if(!isTouchWithFixed)
		{
			return;
		}

		score = 0;
		std::string word = field->GetWordInRow(y,startX, score);
		for (size_t i = 0; i<m_UsedLetters.size(); i++)
		{
			ModifyLetterScores(m_UsedLetters[i]->GetCell()->modifier, m_UsedLetters[i]->GetCost(), score);
		}
		for (size_t i = 0; i<m_UsedLetters.size(); i++)
		{
			ModifyWordScores(m_UsedLetters[i]->GetCell()->modifier, score);
		}
		
		for(int i=0;i<m_UsedLetters.size();i++)
		{
			SGridCell * cell = m_UsedLetters[i]->GetCell();
			int addScore = 0;
			word = field->GetWordInColumn(cell->x,cell->y-1, addScore);				
			if (word.size() > 1)
			{
				ModifyLetterScores(cell->modifier, m_UsedLetters[i]->GetCost(), addScore);
				ModifyWordScores(cell->modifier, addScore);
				score += addScore;
			}
		}
		SGridCell * cell = m_UsedLetters[m_UsedLetters.size()-1]->GetCell();
		PointerPos = field->GetUtmostHorizontalCellPos(cell->x,cell->y);
		ScorePointCalculateComplete(score,PointerPos);
		return;
	}

	if (!allHorizontal && !allVertical)
	{
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ScorePointCalculateComplete(const int score, const cocos2d::Point pos)
{
	CScorePointer * scorePointer = m_Player->GetCurrentRoom()->GetCurrentPlayerScorePointer();
	scorePointer->setPosition(pos);
	scorePointer->SetScore(score);
	scorePointer->Show();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ShowCurrentRoom(Node* sender)
{
	if(m_Player->GetCurrentRoomId() == -1)
	{
		return;
	}
	CRoom* room = m_Player->GetCurrentRoom();
	room->Show();
	bool show = false;
	if(room->GetState() == CRoom::RS_END_GAME)
	{
		show = true;
	}
	m_RematchButton->setVisible(show);
	m_RematchButton->setEnabled(show);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::HideCurrentRoom(Node* sender)
{
	if(m_Player->GetCurrentRoomId() == -1)
	{
		return;
	}
	m_Player->GetCurrentRoom()->Hide();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ChangingRoomsComplete(Node* sender)
{
	m_IsChangingRooms = false;
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::RematchingRoomComplete(Node* sender)
{
	m_IsChangingRooms = false;
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::RebuildRoomLabels()
{
	//if(m_CurrentRoom == -1)
	if(m_Player->GetCurrentRoomId() == -1)
	{
		return;
	}

	CRoom * room = m_Player->GetCurrentRoom();

	m_Player_1_Name->setString(room->GetFirstPlayerName() + ":");
	m_Player_2_Name->setString(room->GetSecondPlayerName() + ":");
	m_Player_1_Score->setString(StringUtils::toString(room->GetFirstPlayerScore()));
	m_Player_2_Score->setString(StringUtils::toString(room->GetSecondPlayerScore()));
	if(room->GetTurn() == CRoom::RT_FIRST_PLAYER)
	{
		m_Player_1_Name->setColor(ACTIVE_PLAYER_COLOR);
		m_Player_1_Score->setColor(ACTIVE_PLAYER_COLOR);
		m_Player_2_Name->setColor(WAITING_PLAYER_COLOR);
		m_Player_2_Score->setColor(WAITING_PLAYER_COLOR);
	}
	else
	{
		m_Player_1_Name->setColor(WAITING_PLAYER_COLOR);
		m_Player_1_Score->setColor(WAITING_PLAYER_COLOR);
		m_Player_2_Name->setColor(ACTIVE_PLAYER_COLOR);
		m_Player_2_Score->setColor(ACTIVE_PLAYER_COLOR);
	}
	m_Player_1_Name->setVisible(true);
	m_Player_2_Name->setVisible(true);
	m_Player_1_Score->setVisible(true);
	m_Player_2_Score->setVisible(true);

	m_StackLetterRemainCount->setString(StringUtils::toString(room->GetStack()->GetRemainLettersCount()));
	m_StackLetterRemainCount->setVisible(true);
	m_StackLetterRemainLabel->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::HideRoomLabels()
{
	m_Player_1_Name->setVisible(false);
	m_Player_2_Name->setVisible(false);
	m_Player_1_Score->setVisible(false);
	m_Player_2_Score->setVisible(false);
	m_StackLetterRemainCount->setVisible(false);
	m_StackLetterRemainLabel->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ShowEndLevelWindow()
{
	int player_1_score = m_Player->GetCurrentRoom()->GetFirstPlayerScore();
	int player_2_score = m_Player->GetCurrentRoom()->GetSecondPlayerScore();

	m_EndLevelWindow->Show(player_1_score,player_2_score);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::RematchCurrentRoom(Node* sender)
{
	CRoom* room = m_Player->GetCurrentRoom();
	room->Rematch();
	DeleteFromTable(CRoomsTable::TT_GAME_OVER, room->GetRoomButton());;
	PutToTable(CRoomsTable::TT_YOUR_MOVE,room->GetRoomButton());
	RebuildTablesPos();
	RebuildRoomLabels();
}

void CGameScene::Rematch(Ref* sender)
{
	m_IsChangingRooms = true;
	float delay = 0.0f;

	m_RematchButton->setVisible(false);
	m_RematchButton->setEnabled(false);
	m_PassButton->setVisible(true);
	CRoom * room  = m_Player->GetCurrentRoom();
	MakeRecall();
	if(room->GetState() == CRoom::RS_SWAP_PANEL_SHOW)
	{
		GetSwapPanel()->CancelSwap();
	}
	if(GetEndLevelWindow()->IsShow())
	{
		GetEndLevelWindow()->Hide();
	}
	HideCurrentRoom(this);
	room->GetRoomButton()->setEnable(false);
	delay+= CField::SHOW_TIME;

	auto delayAction = DelayTime::create(delay);
	auto blockButtonsFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::BlockControlButtons));
	auto changingCompleteFunc = CallFuncN::create(this,callfuncN_selector(CGameScene::ChangingRoomsComplete));
	auto showRoomFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::ShowCurrentRoom));
	auto rematchRoomFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::RematchCurrentRoom));
	auto delayAction2 = DelayTime::create(CLetter::SHOW_ACTION_TIME*7);
	auto unblockButtonsFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::UnblockControlButtons));
	Sequence * sequence = Sequence::create(blockButtonsFunc,delayAction,rematchRoomFunc,showRoomFunc,delayAction2,unblockButtonsFunc,changingCompleteFunc,NULL);
	runAction(sequence);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::RebuildRoomButtonsPos()
{
	if(m_Player->IsHaveCreatedRoom() == false)
	{
		return;
	}
	m_Player->RebuildRoomButtonPos();
/*
	if(m_Rooms.empty())
	{
		return;
	}
	int turn = 0;
	for(int i=0;i<m_Rooms.size();i++)
	{
		CRoom *room = m_Rooms[i];
		if(room->GetId() != i)
		{
			room->ChangeId(i,turn);
			turn++;
		}
	}*/
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::PutToTable(CRoomsTable::TABLE_TYPE table, CRoomButton* button)
{
	m_RoomsTables[table]->PutToTable(button);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::DeleteFromTable(CRoomsTable::TABLE_TYPE table, CRoomButton* button)
{
	m_RoomsTables[table]->DeleteFromTable(button);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::RebuildTablesPos()
{
	Point pos = Point(ROOM_TABLE_START_POS_X, ROOM_TABLE_START_POS_Y);
	bool isAlignLeft = true;
	for(int i=0;i<m_RoomsTables.size();i++)
	{
		CRoomsTable* table = m_RoomsTables[i];
		if(table->IsEmpty() == false)
		{
			table->setPosition(pos);
			pos.y -= table->GetTableHeight();
			if(isAlignLeft)
			{
				table->MarkerAlignLeft();

			}
			else
			{
				table->MarkerAlignRight();
			}
			isAlignLeft = !isAlignLeft;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ShowBonusPanel()
{
	GetBonusPanel()->Show();
	CRoom* room = m_Player->GetCurrentRoom();
	room->SetState(CRoom::RS_BONUS_PANEL_SHOW);
	m_MoreButton->setEnabled(false);
	m_MoreButton->setVisible(false);	
	m_LessButton->setEnabled(true);
	m_LessButton->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::HideBonusPanel()
{
	GetBonusPanel()->Hide();
	CRoom* room = m_Player->GetCurrentRoom();
	room->ReturnOldState();
	m_LessButton->setEnabled(false);
	m_LessButton->setVisible(false);
	m_MoreButton->setEnabled(true);
	m_MoreButton->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////
CRoom* CGameScene::GetCurrentRoom()
{
	return m_Player->GetCurrentRoom();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::RemoveRoom()
{
	m_IsChangingRooms = true;
	float delay = 0.0f;

	m_RematchButton->setVisible(false);
	m_RematchButton->setEnabled(false);
	m_PassButton->setVisible(true);
	CRoom * room  = m_Player->GetCurrentRoom();
	MakeRecall();
	if(room->GetState() == CRoom::RS_SWAP_PANEL_SHOW)
	{
		GetSwapPanel()->CancelSwap();
	}
	if(GetEndLevelWindow()->IsShow())
	{
		GetEndLevelWindow()->Hide();
	}
	HideCurrentRoom(this);
	delay+= CField::SHOW_TIME;

	auto delayAction = DelayTime::create(delay);
	auto blockButtonsFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::BlockControlButtons));
	auto removeRoomFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::RemoveCurrentRoom));
	Sequence * sequence = Sequence::create(blockButtonsFunc,delayAction,removeRoomFunc,NULL);
	runAction(sequence);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::RemoveCurrentRoom(Node* sender)
{
	m_Player->RemoveCurrentRoom();
	int currentRoomId = m_Player->GetCurrentRoomId();
	if(currentRoomId == -1)
	{
		m_IsChangingRooms = false;
		HideRoomLabels();
		return;
	}
	RebuildRoomButtonsPos();
	auto delayAction = DelayTime::create(CField::SHOW_TIME);
	auto changingCompleteFunc = CallFuncN::create(this,callfuncN_selector(CGameScene::ChangingRoomsComplete));
	auto showRoomFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::ShowCurrentRoom));
	auto unblockButtonsFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::UnblockControlButtons));
	Sequence * sequence = Sequence::create(showRoomFunc,delayAction,unblockButtonsFunc,changingCompleteFunc,NULL);
	runAction(sequence);
	m_Player->GetCurrentRoom()->GetRoomButton()->setEnable(false);
	RebuildRoomLabels();
}

//////////////////////////////////////////////////////////////////////////
int CGameScene::GetFirstRoomFromTable()
{
	if(m_RoomsTables[CRoomsTable::TT_YOUR_MOVE]->IsEmpty() == false)
	{
		return m_RoomsTables[CRoomsTable::TT_YOUR_MOVE]->GetFirstRoomId();
	}
	else if(m_RoomsTables[CRoomsTable::TT_THEIR_MOVE]->IsEmpty() == false)
	{
		return m_RoomsTables[CRoomsTable::TT_THEIR_MOVE]->GetFirstRoomId();
	}
	else
	{
		return -1;
	}
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::update(float fDelta)
{
    
    Vec2 screenSize = Director::getInstance()->getWinSize();
    
    //CCLOG("my position X :: %f Y :: %f screenSize X :: %f Y :: %f",this->getPositionX(),this->getPositionY(),screenSize.x,screenSize.y);
    
    if (this->getPositionY() <= (-(screenSize.y * 0.25))) {
        this->setPosition(Vec2(this->getPositionX(),(-(screenSize.y * 0.25))+1));
    }
    
    if (this->getPositionY() >= ((screenSize.y * 0.25))) {
        this->setPosition(Vec2(this->getPositionX(),(screenSize.y * 0.25)-1));
    }
    
    if (this->getPositionX() <= (-(screenSize.x * 0.25))) {
        this->setPosition(Vec2((-(screenSize.x * 0.25)+1), this->getPositionY()));
    }
    
    if (this->getPositionX() >= ((screenSize.x * 0.25))) {
        this->setPosition(Vec2((screenSize.x * 0.25)-1,this->getPositionY()));
    }
    
    if (firstTapDone) {
        tapDelay = tapDelay + fDelta;
        if(tapDelay >= TAP_DELAY) {
            firstTapDone = false;
            tapDelay = 0.0f;
        }
            
    }
    
	if(m_WaitingWindow->IsShow() == false)
	{
		m_NewRoomButton->update(fDelta);
		m_PlayButton->update(fDelta);
		m_PassButton->update(fDelta);
		m_SwapButton->update(fDelta);
		m_RecallButton->update(fDelta);
		m_RematchButton->update(fDelta);

		m_BonusPanel->GetResignBtn()->update(fDelta);
		m_BonusPanel->GetWordMetterOnBtn()->update(fDelta);
		m_BonusPanel->GetWordMetterOffBtn()->update(fDelta);
	}
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::ClearWordsContainers()
{
	m_ScoresForCurrentMove = 0;
	m_FindedWords.clear();
	m_WrongWords.clear();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SendMove()
{
	ShowWaitingAnim();
	PLAYER_MOVE move;
	CField* field = GetCurrentRoom()->GetField();
	move.room_server_id = GetCurrentRoom()->GetServerId();
	for(int i=0;i<m_UsedLetters.size();i++)
	{
		int index = field->GetIndexByPos(Point(m_UsedLetters[i]->GetCell()->x,m_UsedLetters[i]->GetCell()->y));
		char letter;
		letter = m_UsedLetters[i]->GetLetter();
		move.used_letters[index] = letter;
	}
	int usedLetterCount = m_UsedLetters.size();
	move.stack = GetCurrentRoom()->GetStack()->GetStackInString(usedLetterCount);
	move.deck = GetCurrentRoom()->GetFirstPlayerDeck()->GetDeckInString();
	if(usedLetterCount != 0)
	{
		move.deck += GetCurrentRoom()->GetStack()->GetFirstLettersInString(usedLetterCount);
	}
	move.score = m_ScoresForCurrentMove;
	move.word = m_FindedWords[0];
	move.pass_counter = 0;
	m_Player->SendMoveToServer(move);
}


void CGameScene::ShowSendMoveWindow()
{
	CConfirmWindow* sendMoveWindow = CConfirmWindow::create();
	sendMoveWindow->SetConfirmLabel("send move?");
	sendMoveWindow->SetOkLabel("Send Move");
	sendMoveWindow->SetCancelLabel("Cancel Move");
	sendMoveWindow->SetOkCallback(this,callfuncN_selector(CGameScene::SendMoveWindowOkCallback));
	sendMoveWindow->SetCancelCallback(this,callfuncN_selector(CGameScene::SendMoveWindowCancelCallback));
	addChild(sendMoveWindow);
	sendMoveWindow->Show();
}

void CGameScene::SendMoveWindowOkCallback(Node* sender)
{
	SendMove();
}

void CGameScene::SendMoveWindowCancelCallback(Node* sender)
{
	ClearWordsContainers();
}

void CGameScene::SendingMoveCompleteSuccess()
{
	HideWaitingAnim();
	std::string buff;
	my_itoa(m_ScoresForCurrentMove,buff,10);
	std::string success = "You reach ";
	success += buff;
	success += " scores!";
	MessageBox(success.c_str(),"Success!");

	CRoom* room = GetCurrentRoom();
	CField* field = room->GetField();
	field->ClearNewLetters();
	for (size_t i = 0; i<m_UsedLetters.size(); i++)
	{
		m_UsedLetters[i]->SetFixed();
		m_UsedLetters[i]->SetNew();
	}
	if(field->IsFirstWordPlased() == false)
	{
		field->SetPlasedFirstWord(true);
	}
	room->AddCurrentPlayerScore(m_ScoresForCurrentMove);
	float currentDelay = 0.2f;
	room->GetCurrentPlayerDeck()->setTouchEnabled(false);
	room->GetCurrentPlayerDeck()->AlignLetter();
	room->GetCurrentPlayerScorePointer()->Hide();
	currentDelay += room->GetCurrentPlayerDeck()->InitEmptyCells(0.15f);
	SetVisiblePlayButton(false);
	CallFuncN *blockButtonsFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::BlockControlButtons));
	auto delayAction = DelayTime::create(currentDelay);
	CallFuncN *unblockDeck = CallFuncN::create(this, callfuncN_selector(CGameScene::UnblockDeck));
	Spawn * spawn = Spawn::create(blockButtonsFunc,delayAction,unblockDeck,NULL);
	runAction(spawn);
	GetCurrentRoom()->GetRoomButton()->SetLastWord(m_FindedWords[0]);
	GetCurrentRoom()->GetRoomButton()->SetLastAction("Just now");
	room->CleanupPassInRow();
	ClearWordsContainers();
	m_UsedLetters.clear();
	SetOpponentTurn();
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::SendingMoveCompleteFail()
{
	HideWaitingAnim();
	MessageBox("No response from the server. Please try again later.","Fail!");
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::UnblockDeck(Node* sender)
{
	GetCurrentRoom()->GetCurrentPlayerDeck()->setTouchEnabled(true);
	m_ShuffleButton->setEnabled(true);
	m_MoreButton->setEnabled(true);
}

//////////////////////////////////////////////////////////////////////////
//void CGameScene::StartGameUpdateTime()
//{
//	auto delayAction = DelayTime::create(CHECKING_GAMES_UPDATE_TIME);
//	CallFuncN *checkFunc = CallFuncN::create(this, callfuncN_selector(CGameScene::CheckGamesUpdate));
//	auto sequnce = Sequence::create(delayAction, checkFunc, NULL);
//	sequnce->setTag(UPDATE_ROOM_ACTION_TAG);
//	runAction(sequnce);
//}

//////////////////////////////////////////////////////////////////////////
void CGameScene::StopGameUpdateTime()
{
	stopActionByTag(UPDATE_ROOM_ACTION_TAG);
}

//////////////////////////////////////////////////////////////////////////
void CGameScene::CheckGamesUpdate(Node* sender)
{
	CGameScene::Instance()->GetPlayer()->CheckGamesUpdate();
}

//////////////////////////////////////////////////////////////////////////
bool CGameScene::IsHaveActiveLetter()
{
	bool isHave = true;
	if(m_ActiveLetter == NULL)
	{
		isHave = false;
	}
	return isHave;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
//////////////////////////////////////////////////////////////////////////
void CGameScene::onMouseMove(Event* event)
{
	auto mouseEvent = static_cast<EventMouse*>(event);
	Point mousePos = Point(mouseEvent->getCursorX(), mouseEvent->getCursorY());
	m_NewRoomButton->MouseMove(mousePos);
	m_PlayButton->MouseMove(mousePos);
	m_PassButton->MouseMove(mousePos);
	m_SwapButton->MouseMove(mousePos);
	m_RecallButton->MouseMove(mousePos);
	m_RematchButton->MouseMove(mousePos);

	m_BonusPanel->GetResignBtn()->MouseMove(mousePos);
	m_BonusPanel->GetWordMetterOnBtn()->MouseMove(mousePos);
	m_BonusPanel->GetWordMetterOffBtn()->MouseMove(mousePos);
}
#endif