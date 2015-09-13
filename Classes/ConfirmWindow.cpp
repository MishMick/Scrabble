#include "ConfirmWindow.h"
#include "GameScene.h"

float CConfirmWindow::SHOW_ACTION_TIME = 0.2f;

//////////////////////////////////////////////////////////////////////////
CConfirmWindow::CConfirmWindow():cocos2d::Layer()
{
	
}

//////////////////////////////////////////////////////////////////////////
CConfirmWindow::~CConfirmWindow()
{

}

CConfirmWindow* CConfirmWindow::create() {
    CConfirmWindow *obj = new CConfirmWindow();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CConfirmWindow::init()
{	   
	m_MenuClickState = false;
	m_OkTarget = NULL;
	m_CancelTarget = NULL;

	m_Back = Sprite::create("confirm_back.png");
	Point Pos = Point(CGameScene::WORKSPACE_POS_X + CGameScene::WORKSPACE_SIZE_X/2,
						CGameScene::WORKSPACE_POS_Y + CGameScene::WORKSPACE_SIZE_Y/2);
	m_Back->setPosition(Pos); 
	addChild(m_Back);

	m_OkBtnLabel = LabelTTF::create("0","helvetica",24);	
	m_OkBtnLabel->setColor(Color3B(255.0f,255.0f,255.0f));
	m_OkBtnLabel->setString("Ok");

	m_CancelBtnLabel = LabelTTF::create("0","helvetica",24);	
	m_CancelBtnLabel->setColor(Color3B(255.0f,255.0f,255.0f));
	m_CancelBtnLabel->setString("Cancel");

	m_DescriptionLabel = LabelTTF::create("0","helvetica",24);	
	m_DescriptionLabel->setColor(Color3B(255.0f,255.0f,255.0f));
	m_DescriptionLabel->setPosition(m_Back->getContentSize().width/2,m_Back->getContentSize().height/2+20);
	m_DescriptionLabel->setString("Description");
	m_Back->addChild(m_DescriptionLabel);

	m_OkBtn = CCMenuItemImage::create(
		"rectangle.png",
		"rectangle_over.png",
		this,
		menu_selector(CConfirmWindow::OkBtnClick));
	m_OkBtn->setPosition(-100,-20);
	m_OkBtn->setEnabled(false);
	//m_OkBtnLabel->setPosition(m_OkBtn->getContentSize().width/2, m_OkBtn->getContentSize().height/2);

	m_CancelBtn = CCMenuItemImage::create(
		"rectangle.png",
		"rectangle_over.png",
		this,
		menu_selector(CConfirmWindow::CancelBtnClick));
	m_CancelBtn->setPosition(100,-20);
	m_CancelBtn->setEnabled(false);
	//m_CancelBtnLabel->setPosition(m_CancelBtn->getContentSize().width/2, m_CancelBtn->getContentSize().height/2);

	m_Menu = CCMenu::create(m_OkBtn, m_CancelBtn,NULL);
	m_Menu->setPosition(m_Back->getContentSize().width/2,m_Back->getContentSize().height/2);			
	m_Back->addChild(m_Menu);
	m_Back->addChild(m_OkBtnLabel);
	m_Back->addChild(m_CancelBtnLabel);
	m_OkBtnLabel->setPosition(m_Menu->getPosition() + m_OkBtn->getPosition());
	m_CancelBtnLabel->setPosition(m_Menu->getPosition() + m_CancelBtn->getPosition());


	m_Back->setCascadeOpacityEnabled(true);
	m_Back->setOpacity(0.0f);
	m_Back->setVisible(false);
	
	m_IsShow = false;
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CConfirmWindow::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CConfirmWindow::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CConfirmWindow::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithFixedPriority(listener,-129);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::SetOkLabel(std::string label)
{
	m_OkBtnLabel->setString(label);
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::SetCancelLabel(std::string label)
{
	m_CancelBtnLabel->setString(label);
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::SetConfirmLabel(std::string label)
{
	m_DescriptionLabel->setString(label);
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::Show()
{
	m_Back->setVisible(true);
	m_Back->setScale(0.5f);
	m_Back->setOpacity(0.0f);
	
	auto fadeInAction = FadeIn::create(SHOW_ACTION_TIME);
	auto scaleAction = ScaleTo::create(SHOW_ACTION_TIME,1.0f);
	auto spawn = Spawn::create(fadeInAction,scaleAction,NULL);
	auto showCompleteFunc = CallFuncN::create(this,callfuncN_selector(CConfirmWindow::ShowComplete));
	auto sequence = Sequence::create(spawn,showCompleteFunc,NULL);
	m_Back->runAction(sequence);
	m_IsShow = true;
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::Hide()
{
	auto fadeOutAction = FadeOut::create(SHOW_ACTION_TIME);
	auto scaleAction = ScaleTo::create(SHOW_ACTION_TIME,0.5f);
	auto spawn = Spawn::create(fadeOutAction,scaleAction,NULL);
	auto hideCompleteFunc = CallFuncN::create(this,callfuncN_selector(CConfirmWindow::HideComplete));
	auto sequence = Sequence::create(spawn,hideCompleteFunc,NULL);
	m_Back->runAction(sequence);

	m_OkBtn->setEnabled(false);
	m_CancelBtn->setEnabled(false);

	m_IsShow = false;
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::HideComplete(Node* sender)
{
	this->removeFromParent();
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::ShowComplete(Node* sender)
{
	m_OkBtn->setEnabled(true);
	m_CancelBtn->setEnabled(true);
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::OkBtnClick(Ref* sender)
{
	Hide();
	if (m_OkTarget)
	{
		if (m_OkSelector) {
			(m_OkTarget->*m_OkSelector)(NULL);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::CancelBtnClick(Ref* sender)
{
	Hide();
	if (m_CancelTarget)
	{
		if (m_CancelSelector) {
			(m_CancelTarget->*m_CancelSelector)(NULL);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::SetOkCallback(Ref* target, SEL_CallFuncN selector)
{
	m_OkTarget = target;
	m_OkSelector = selector;
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::SetCancelCallback(Ref* target, SEL_CallFuncN selector)
{
	m_CancelTarget = target;
	m_CancelSelector = selector;
}

//////////////////////////////////////////////////////////////////////////
bool CConfirmWindow::onTouchBegan(Touch *touch, Event *unused_event)
{	
	m_MenuClickState = m_Menu->onTouchBegan(touch,unused_event);
	if(m_IsShow)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (m_MenuClickState)
	{
		m_Menu->onTouchMoved(touch,unused_event);
	}	
}

//////////////////////////////////////////////////////////////////////////
void CConfirmWindow::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (m_MenuClickState)
	{
		m_Menu->onTouchEnded(touch,unused_event);
	}
}