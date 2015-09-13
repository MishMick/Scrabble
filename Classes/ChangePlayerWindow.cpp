#include "ChangePlayerWindow.h"
#include "GameScene.h"

//////////////////////////////////////////////////////////////////////////
CChangePlayerWindow::CChangePlayerWindow():cocos2d::Layer()
{
	
}

//////////////////////////////////////////////////////////////////////////
CChangePlayerWindow::~CChangePlayerWindow()
{

}

CChangePlayerWindow* CChangePlayerWindow::create() {
    CChangePlayerWindow *obj = new CChangePlayerWindow();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CChangePlayerWindow::init()
{	   
	m_Back = Sprite::create("next_player_panel.png");
	m_Back->setAnchorPoint(Point(0.5f,0.0f));
	m_Back->setPosition(CGameScene::WORKSPACE_POS_X+CGameScene::WORKSPACE_SIZE_X/2,
							CGameScene::WORKSPACE_POS_Y-m_Back->getContentSize().height); 
	Point backCenter = Point(m_Back->getContentSize().width/2,
								m_Back->getContentSize().height/2);
	addChild(m_Back);

	m_PlayerName = LabelTTF::create("0","helvetica",24);	
	m_PlayerName->setColor(Color3B(255.0f,255.0f,255.0f));
	m_PlayerName->setPosition(backCenter.x,backCenter.y+15.0f);
	m_PlayerName->setAnchorPoint(Point(0.5f,0.5f));

	m_TurnLabel = LabelTTF::create("0","helvetica",20);	
	m_TurnLabel->setColor(Color3B(255.0f,255.0f,255.0f));
	m_TurnLabel->setPosition(backCenter.x,backCenter.y-10.0f);
	m_TurnLabel->setString("turns");
	m_TurnLabel->setAnchorPoint(Point(0.5f,0.5f));

	m_Back->addChild(m_TurnLabel);
	m_Back->addChild(m_PlayerName);

	m_Back->setCascadeOpacityEnabled(true);
	m_Back->setOpacity(0.0f);

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CChangePlayerWindow::Show(std::string playerName)
{
	m_PlayerName->setString(playerName);
	Point moveBy = Point(0.0f,m_Back->getContentSize().height);
	auto showAction	= CCMoveBy::create(0.3f,moveBy);
	auto alphaAction = CCFadeIn::create(0.3f);
	auto spawn			= Spawn::create(showAction,alphaAction,NULL);
	m_Back->runAction(spawn);
}

//////////////////////////////////////////////////////////////////////////
void CChangePlayerWindow::Hide()
{
	Point moveBy = Point(0.0f,-m_Back->getContentSize().height);
	auto showAction	= CCMoveBy::create(0.3f,moveBy);
	auto alphaAction = CCFadeOut::create(0.3f);
	auto spawn			= Spawn::create(showAction,alphaAction,NULL);
	m_Back->runAction(spawn);
}
