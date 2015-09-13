#include "Letter.h"

#include "Deck.h"
#include "SwapPanel.h"

#include "GameScene.h"

#include "ChooseLetterWindow.h"


float CLetter::SHOW_ACTION_TIME = 0.1f;
float CLetter::MOVE_ACTION_TIME = 0.1f;

//////////////////////////////////////////////////////////////////////////
CLetter::CLetter():cocos2d::Layer()
{
	m_Cell = NULL;
	m_DeckCell = -1;
	m_SwapPanelCell = -1;
}

//////////////////////////////////////////////////////////////////////////
CLetter::~CLetter()
{

}

CLetter* CLetter::create() {
    CLetter* obj = new CLetter();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CLetter::init()
{
	setTag(LETTER_TAG);
	COLOR_NEW = Color3B(255,255,255);
	COLOR_OLD = Color3B(0,0,0);

	m_IsNew = false;
	m_IsDrag = false;
	m_SwapPanelCell = -1;
	m_ListenerOnPause = true;

	m_Base = Sprite::create("base_box.png");	
	addChild(m_Base);

	m_CostLabel = LabelTTF::create("0","helvetica",42);
	m_CostLabel->setPosition(35.0f,35.0f);
	m_CostLabel->setColor(COLOR_OLD);
	addChild(m_CostLabel);

	m_LetterLabel = LabelTTF::create("0","helvetica",82);	
	m_LetterLabel->setColor(COLOR_OLD);
	m_LetterLabel->setPosition(0.0f,-20.0f);
	addChild(m_LetterLabel);

	setScale(0.8f);
	setOpacity(0);
	setCascadeOpacityEnabled(true);
	//Initializing and binding 
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CLetter::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CLetter::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CLetter::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	m_Spawn = NULL;
	m_Sequence = NULL;
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CLetter::SetTouchPause(bool pause)
{
	m_ListenerOnPause = pause;
}

//////////////////////////////////////////////////////////////////////////
bool CLetter::IsTouchPause()
{
	return m_ListenerOnPause;
}


//////////////////////////////////////////////////////////////////////////
bool CLetter::onTouchBegan(Touch *touch, Event *unused_event)
{
	if(m_ListenerOnPause)
	{
		return false;
	}
	if (m_Base->getBoundingBox().containsPoint(this->convertTouchToNodeSpace(touch)))
	{
		CGameScene* scene = CGameScene::Instance();
		scene->SetActiveLetter(this);
        log("letter dragged set true");
        scene->letterDragged = true;
        
		CRoom* room = scene->GetCurrentRoom();
		if(room->GetState() == CRoom::RS_BONUS_PANEL_SHOW)
		{
			scene->HideBonusPanel();
		}
		else if(room->GetState() != CRoom::RS_IN_GAME && room->GetState() != CRoom::RS_SWAP_PANEL_SHOW)
		{
			return false;
		}
		if(m_Cell && m_Cell->fixed)
		{
			return false;
		}
		setZOrder(FLY_LAYER);
		m_IsDrag = true;
		room->GetCurrentPlayerScorePointer()->Hide();
		if (m_Cell)		
		{
			m_Cell->letter = -1;
			m_Cell->isWithoutScore = false;
		}		
		else if (getPositionY() <= CDeck::DECK_Y_POS)
		{
			CDeck * deck = room->GetCurrentPlayerDeck();
			if (deck)
			{
				deck->ClearCell(m_DeckCell);
			}			
		}
		else if(room->GetState() == CRoom::RS_SWAP_PANEL_SHOW && m_SwapPanelCell != -1)
		{
			CSwapPanel* swapPanel = scene->GetSwapPanel();
			swapPanel->ClearCell(m_SwapPanelCell);
		}

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
void CLetter::onTouchMoved(Touch *touch, Event *unused_event)
{
	if(m_ListenerOnPause)
	{
		return;
	}
	if (m_IsDrag)
	{
		CGameScene* scene = CGameScene::Instance();
		CRoom* room = scene->GetCurrentRoom();
		setPosition(touch->getLocation());
		Point pos = touch->getLocation();
		float deckHeight = CDeck::DECK_Y_POS + (m_Base->getContentSize().height/2)*0.45f;
		if (pos.y > deckHeight)
		{
			if(room->GetState() == CRoom::RS_IN_GAME)
			{
				if(this->getParent() == room->GetCurrentPlayerDeck())
				{
					retain();
					removeFromParent();
					setScale(0.55f);
					room->GetField()->GetFieldSprite()->addChild(this,FLY_LAYER);
					release();				
					//CGameScene::Instance()->SetLetterUsed(this);
				}			
				CField * field = room->GetField();
				//Point pos = scene->GetField()->GetFieldSprite()->convertTouchToNodeSpace(touch);
				Size size = m_Base->getContentSize();
				size = (m_Base->getContentSize()*getScale());
				size = size*field->GetFieldSprite()->getScale();
				//size = ((m_Base->getContentSize()*getScale())*field->getScale());
				Point maxLimit = Point(CGameScene::WORKSPACE_POS_X+CGameScene::WORKSPACE_SIZE_X-size.width/2,
					CGameScene::WORKSPACE_POS_Y+CGameScene::WORKSPACE_SIZE_Y-size.height/2);
				Point minLimit = Point(CGameScene::WORKSPACE_POS_X+size.width/2,
					CGameScene::WORKSPACE_POS_Y+size.height/2);
				maxLimit += field->GetFieldOffset();
				if(pos.x > maxLimit.x) pos.x = maxLimit.x;
				else if(pos.x < minLimit.x) pos.x = minLimit.x;
				if(pos.y > maxLimit.y) pos.y = maxLimit.y;
				pos = field->GetFieldSprite()->convertToNodeSpace(pos);
				setPosition(pos);
			}
		}
		else
		{
			CDeck* deck = room->GetCurrentPlayerDeck();
			deck->CheckCellSwap(touch,this);
			setPosition(pos);
			//if (this->getParent() != CGameScene::Instance())
			if(this->getParent() != deck)
			{
				retain();
				removeFromParent();
				setScale(0.45f);
				//CGameScene::Instance()->addChild(this);
				deck->addChild(this,FLY_LAYER);
				release();
				//CGameScene::Instance()->SetLetterNotUsed(this);				
			}			
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CLetter::onTouchEnded(Touch *touch, Event *unused_event)
{
	if(m_ListenerOnPause)
	{
		return;
	}
	
	if(m_IsDrag)
	{		
		CGameScene* scene = CGameScene::Instance();
		scene->ClearActiveLetter();
        log("letter dragged set false");
        scene->letterDragged = false;
		CRoom* room = scene->GetCurrentRoom();
		CField* field = room->GetField();
		m_IsDrag = false;
		setZOrder(STAY_LAYER);
		//Point pos = field->GetFieldSprite()->convertToWorldSpace(getPosition());
		Point pos = touch->getLocation();
		float deckHeight = CDeck::DECK_Y_POS + (m_Base->getContentSize().height/2)*0.45f;
		if (pos.y > deckHeight)
		{
			if(room->GetState() == CRoom::RS_SWAP_PANEL_SHOW)
			{
				if(getPositionY() <= CSwapPanel::DECK_Y_POS)
				{
					SetLastPosType(LPT_SWAP);
					scene->GetSwapPanel()->PutLetterToSwapPanel(touch,this);
				}
				else
				{
					GoToLastPos();
				}
			}
			else
			{
				SGridCell * cell = field->GetCellUnderCursor(getPosition());
				if (cell)
				{
					if (cell->modifier == M_Empty || cell->letter != -1)
					{
						cell = field->GetNearestEmptyCell(cell);
					}
				}	
				if (cell)
				{
					SetLastPosType(LPT_FIELD);
					setPosition(cell->pos);
					cell->letter = m_Letter;
					m_Cell = cell;
					scene->SetLetterUsed(this);
					if(GetCost() == 0)
					{
						scene->GetChooseLetterWindow()->Show(this);
						cell->isWithoutScore = true;
					}
				}		
			}
		}
		else
		{
			if (room->GetCurrentPlayerDeck())
			{
				if(pos.y > CDeck::DECK_Y_POS && GetLastPosType() == LPT_DECK)
				{
					GoToLastPos();
					return;
				}
				SetLastPosType(LPT_DECK);
				room->GetCurrentPlayerDeck()->PutLetterToDeck(touch,this);
				scene->SetLetterNotUsed(this);
				if(GetCost() == 0)
				{
					SetLetter(' ');
				}
			}			
		}
		scene->CalculateScorePointer();
	}
}

//////////////////////////////////////////////////////////////////////////
void CLetter::SetStayOnField()
{
	setScale(0.55f);
	setOpacity(255);
}

//////////////////////////////////////////////////////////////////////////
void CLetter::SetCost(const int cost)
{
	m_Cost = cost;
    std::string buff;
	my_itoa(cost,buff,10);
	std::string str = buff;
	m_CostLabel->setString(str);

	m_CostLabel->setVisible(m_Cost>0);
}

//////////////////////////////////////////////////////////////////////////
void CLetter::SetLetter(char letter)
{
	m_Letter = letter;
	std::string str = "A";
	str[0] = letter;
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);
	m_LetterLabel->setString(str);
	if(m_Cell != NULL)
	{
		m_Cell->letter = m_Letter;
		if(m_Cost == 0)
		{
			m_Cell->isWithoutScore = true;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CLetter::UseRecall()
{
	if (m_Cell)
	{
		m_Cell->letter = -1;
		m_Cell->isWithoutScore = false;
		m_Cell = NULL;
	}
	if(GetCost() == 0)
	{
		SetLetter(' ');
	}
	retain();
	removeFromParent();
	setScale(0.45f);
	CGameScene::Instance()->GetCurrentRoom()->GetCurrentPlayerDeck()->addChild(this,STAY_LAYER);
	release();
}

/////////////////////////////////////////////////////////////////////////////////
void CLetter::SetFixed()
{
	GetCell()->fixed = true;
	m_Base->setTexture("fixed.png");
}

/////////////////////////////////////////////////////////////////////////////////
void CLetter::GoToLastPos()
{
	CGameScene* scene = CGameScene::Instance();
	CSwapPanel* swapPanel = scene->GetSwapPanel();
	CDeck* deck = scene->GetCurrentRoom()->GetCurrentPlayerDeck();
	switch(m_LastPosType)
	{
	case LPT_DECK:
		if(this->getParent() != deck)
		{
			retain();
			removeFromParent();
			setScale(0.45f);
			deck->addChild(this,STAY_LAYER);
			release();
		}
		deck->PutLetterToDeck(m_DeckCell,this);
		m_Cell = NULL;
		m_SwapPanelCell = -1;
		break;
	case LPT_SWAP:
		setZOrder(STAY_LAYER);
		swapPanel->PutLetterToSwapPanel(m_SwapPanelCell,this);
		m_Cell = NULL;
		m_DeckCell = -1;
		break;
	case LPT_FIELD:
		if(this->getParent() == deck)
		{
			retain();
			removeFromParent();
			setScale(0.55f);
			scene->GetCurrentRoom()->GetField()->GetFieldSprite()->addChild(this,STAY_LAYER);
			release();				
		}			
		setPosition(m_Cell->pos);
		m_Cell->letter = m_Letter;
		if(m_Cost == 0)
		{
			m_Cell->isWithoutScore = true;
		}
		CGameScene::Instance()->SetLetterUsed(this);
		m_DeckCell = -1;
		m_SwapPanelCell = -1;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
void CLetter::SetLastPosType(LAST_POS_TYPE type)
{
	m_LastPosType = type;
	switch(m_LastPosType)
	{
	case LPT_DECK:
		m_SwapPanelCell = -1;
		m_Cell = NULL;
		break;
	case LPT_SWAP:
		m_DeckCell = -1;
		m_Cell = NULL;
		break;
	case LPT_FIELD:
		m_DeckCell = -1;
		m_SwapPanelCell = -1;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
void CLetter::MoveToAction(Point pos)
{
	stopActionByTag(MOVE_ACTION_TAG);

	MoveTo* moveToAction = MoveTo::create(MOVE_ACTION_TIME,pos);
	Spawn * spawn	= Spawn::create(moveToAction, NULL);
	m_Spawn->setTag(MOVE_ACTION_TAG);
	runAction(spawn);
}

//////////////////////////////////////////////////////////////////////////
void CLetter::MoveToAction(float x, float y)
{
	stopActionByTag(MOVE_ACTION_TAG);
	
	MoveTo* moveToAction = MoveTo::create(MOVE_ACTION_TIME,Point(x,y));
	m_Spawn	= Spawn::create(moveToAction, NULL);
	m_Spawn->setTag(MOVE_ACTION_TAG);
	runAction(m_Spawn);
}

//////////////////////////////////////////////////////////////////////////
void CLetter::ShowAction(float delay)
{
	stopActionByTag(SCALE_ACTION_TAG);

	setOpacity(0.0f);
	setScale(0.2f);
	setVisible(true);
	float scale = 0.45;
	auto preScaleAction = CCScaleTo::create(0.15f,0.5f);
	auto scaleAction	= CCScaleTo::create(0.1f,scale,scale);
	auto showAction		= CCFadeIn::create(0.20f);
	m_Spawn			= Spawn::create(preScaleAction, showAction, NULL);
	if(delay == 0.0f)
	{
		m_Sequence = Sequence::create(m_Spawn,scaleAction,NULL);
		m_Sequence->setTag(SCALE_ACTION_TAG);
		runAction(m_Sequence);
	}
	else
	{
		auto delayAction	= DelayTime::create(delay);
		m_Sequence = Sequence::create(delayAction,m_Spawn, scaleAction, NULL);
		m_Sequence->setTag(SCALE_ACTION_TAG);
		runAction(m_Sequence);
	}
	
}

//////////////////////////////////////////////////////////////////////////
void CLetter::HideAction(float delay)
{
	stopActionByTag(SCALE_ACTION_TAG);

	setScale(0.45f);
	float scale = 0.2;
	auto preScaleAction = CCScaleTo::create(0.1f,0.5f);
	auto scaleAction	= CCScaleTo::create(0.15f,scale,scale);
	auto hideAction		= CCFadeOut::create(0.20f);
	auto invisibleFunc = CallFuncN::create(this, callfuncN_selector(CLetter::SetInvisible));
	m_Spawn			= Spawn::create(scaleAction, hideAction, NULL);
	if(delay == 0.0f)
	{
		m_Sequence = Sequence::create(preScaleAction, m_Spawn, invisibleFunc, NULL);
		m_Sequence->setTag(SCALE_ACTION_TAG);
		runAction(m_Sequence);
	}
	else
	{
		auto delayAction	= DelayTime::create(delay);
		m_Sequence = Sequence::create(delayAction, preScaleAction, m_Spawn, invisibleFunc, NULL);
		m_Sequence->setTag(SCALE_ACTION_TAG);
		runAction(m_Sequence);
	}
}

//////////////////////////////////////////////////////////////////////////
void CLetter::HideInstantly()
{
	setScale(0.2);
	setOpacity(0.0f);
}

//////////////////////////////////////////////////////////////////////////
void CLetter::SetInvisible(Node* sender)
{
	setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
void CLetter::SetNew()
{
	m_LetterLabel->setColor(COLOR_NEW);
	m_IsNew = true;
}

//////////////////////////////////////////////////////////////////////////
void CLetter::SetOld()
{
	m_LetterLabel->setColor(COLOR_OLD);
	m_IsNew = false;
}