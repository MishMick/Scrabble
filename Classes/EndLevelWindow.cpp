#include "EndLevelWindow.h"
#include "GameScene.h"

float CEndLevelWindow::SHOW_ACTION_TIME = 0.2f;

//////////////////////////////////////////////////////////////////////////
CEndLevelWindow::CEndLevelWindow():cocos2d::Layer()
{
	
}

//////////////////////////////////////////////////////////////////////////
CEndLevelWindow::~CEndLevelWindow()
{

}

CEndLevelWindow* CEndLevelWindow::create() {
    CEndLevelWindow* obj = new CEndLevelWindow();
    obj->init();
    return obj;
}
//////////////////////////////////////////////////////////////////////////
bool CEndLevelWindow::init()
{	   
	m_Back = Sprite::create("choose_letter.png");
	Point Pos = Point(CGameScene::WORKSPACE_POS_X + CGameScene::WORKSPACE_SIZE_X/2,
						CGameScene::WORKSPACE_POS_Y + CGameScene::WORKSPACE_SIZE_Y/2);
	m_Back->setPosition(Pos); 
	addChild(m_Back);

	m_ResultLabel = LabelTTF::create("0","helvetica",24);	
	m_ResultLabel->setColor(Color3B(255.0f,255.0f,255.0f));
	m_ResultLabel->setPosition(m_Back->getContentSize().width/2,230.0f);
	m_Back->addChild(m_ResultLabel);

	m_RematchLabel = LabelTTF::create("0","helvetica",24);	
	m_RematchLabel->setColor(Color3B(255.0f,255.0f,255.0f));
	m_RematchLabel->setPosition(m_Back->getContentSize().width/2,170.0f);
	m_RematchLabel->setString("Do you want rematch this game?");
	m_Back->addChild(m_RematchLabel);

	m_RematchButton = CCMenuItemImage::create(
		"end_window_rematch_btn.png",
		"end_window_rematch_btn_over.png",
		this,
		menu_selector(CEndLevelWindow::RematchButtonClick));
	m_RematchButton->setPosition(-430.0f,-300.0f);
	m_RematchButton->setEnabled(false);

	m_CloseButton = CCMenuItemImage::create(
		"end_window_close_btn.png",
		"end_window_close_btn_over.png",
		this,
		menu_selector(CEndLevelWindow::CloseButtonsClick));
	m_CloseButton->setPosition(-265.0f,-300.0f);
	m_CloseButton->setEnabled(false);

	CCMenu* pMenu = CCMenu::create(m_RematchButton, m_CloseButton,NULL);
	m_Back->addChild(pMenu);

	m_Back->setCascadeOpacityEnabled(true);
	m_Back->setOpacity(0.0f);
	m_Back->setVisible(false);
	
	m_IsShow = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CEndLevelWindow::Show(int player_1_score, int player_2_score)
{
	m_Back->setVisible(true);
	m_Back->setScale(0.5f);
	m_Back->setOpacity(0.0f);
	std::string endLevelText;
	if(player_1_score > player_2_score)
	{
		endLevelText = "Player 1 win this game.";
	}
	else if(player_1_score < player_2_score)
	{
		endLevelText = "Player 2 win this game.";
	}
	else
	{
		endLevelText = "The game ended in a draw.";
	}
	m_ResultLabel->setString(endLevelText);
	auto fadeInAction = FadeIn::create(SHOW_ACTION_TIME);
	auto scaleAction = ScaleTo::create(SHOW_ACTION_TIME,1.0f);
	auto spawn = Spawn::create(fadeInAction,scaleAction,NULL);
	auto showCompleteFunc = CallFuncN::create(this,callfuncN_selector(CEndLevelWindow::ShowComplete));
	auto sequence = Sequence::create(spawn,showCompleteFunc,NULL);
	m_Back->runAction(sequence);
	m_IsShow = true;
}

//////////////////////////////////////////////////////////////////////////
void CEndLevelWindow::Hide()
{
	auto fadeOutAction = FadeOut::create(SHOW_ACTION_TIME);
	auto scaleAction = ScaleTo::create(SHOW_ACTION_TIME,0.5f);
	auto spawn = Spawn::create(fadeOutAction,scaleAction,NULL);
	auto hideCompleteFunc = CallFuncN::create(this,callfuncN_selector(CEndLevelWindow::HideComplete));
	auto sequence = Sequence::create(spawn,hideCompleteFunc,NULL);
	m_Back->runAction(sequence);

	m_RematchButton->setEnabled(false);
	m_CloseButton->setEnabled(false);

	m_IsShow = false;
}

//////////////////////////////////////////////////////////////////////////
void CEndLevelWindow::HideComplete(Node* sender)
{
	m_Back->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
void CEndLevelWindow::ShowComplete(Node* sender)
{
	m_RematchButton->setEnabled(true);
	m_CloseButton->setEnabled(true);
}

//////////////////////////////////////////////////////////////////////////
void CEndLevelWindow::RematchButtonClick(Ref* sender)
{
	Hide();
	CGameScene::Instance()->Rematch(m_RematchButton);
}

//////////////////////////////////////////////////////////////////////////
void CEndLevelWindow::CloseButtonsClick(Ref* sender)
{
	Hide();
	CGameScene::Instance()->RemoveRoom();
}