#include "WaitingWindow.h"
#include "xml.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////
CWaitingWindow::CWaitingWindow():Layer()
{
	init();
}

//////////////////////////////////////////////////////////////////////////
CWaitingWindow::~CWaitingWindow()
{

}

CWaitingWindow* CWaitingWindow::create() {
    CWaitingWindow *obj = new CWaitingWindow();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CWaitingWindow::init()
{	
	m_IsShow = false;

	Sprite* overlay = Sprite::create("overlay.png");
	overlay->setScale(512);
	overlay->setPosition(512,384);
	overlay->setOpacity(150);
	addChild(overlay);

	std::string path = "animation/waiting/";
	m_AnimSprite = Sprite::create("001.png");//path+
	m_AnimSprite->setPosition(512,384);
	m_AnimSprite->setScale(1);
	addChild(m_AnimSprite);

	setVisible(false);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CWaitingWindow::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CWaitingWindow::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CWaitingWindow::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void CWaitingWindow::StartAnim()
{
	Vector<SpriteFrame*> animFrames(12);
	std::string path = "";//animation/waiting/
	for(int i = 1;i<=12;i++)
	{
		std::string framePath;
		if(i>9)
		{
			framePath = path + "0" + StringUtils::toString(i) + ".png";
		}
		else
		{
			framePath = path + "00" + StringUtils::toString(i) + ".png";
		}
		auto frame = SpriteFrame::create(framePath,Rect(0,0,64,64));
		animFrames.pushBack(frame);
	}
	auto animation = CCAnimation::createWithSpriteFrames(animFrames,0.1);
	animation->setLoops(-1);
	m_AnimAction = CCAnimate::create(animation);
	m_AnimAction->setTag(ANIM_ACTION_TAG);
	m_AnimSprite->runAction(m_AnimAction);
}

//////////////////////////////////////////////////////////////////////////
void CWaitingWindow::Show()
{
	setVisible(true);
	StartAnim();
	auto alphaAction = CCFadeIn::create(0.3f);
	auto spawn = Spawn::create(alphaAction,NULL);
	m_IsShow = true;
	runAction(spawn);
}

void CWaitingWindow::setShow(bool status) {
    m_IsShow = status;
}

//////////////////////////////////////////////////////////////////////////
void CWaitingWindow::Hide(bool instantly)
{
	float actionTime = 0.3f;
	if(instantly)
	{
		actionTime = 0.0f;
	}
	auto alphaAction = CCFadeOut::create(actionTime);
	auto hideCompleteFunc = CallFuncN::create(this, callfuncN_selector(CWaitingWindow::OnHideComplete));
	auto sequence			= Sequence::create(alphaAction,hideCompleteFunc,NULL);
	runAction(sequence);
}

//////////////////////////////////////////////////////////////////////////
void CWaitingWindow::OnHideComplete(Node* sender)
{
	setVisible(false);
	m_IsShow = false;
	m_AnimSprite->stopActionByTag(ANIM_ACTION_TAG);
}

bool CWaitingWindow::IsShow()
{
	return m_IsShow;
}

//////////////////////////////////////////////////////////////////////////
bool CWaitingWindow::onTouchBegan(Touch *touch, Event *unused_event)
{	
	if(m_IsShow)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
void CWaitingWindow::onTouchMoved(Touch *touch, Event *unused_event)
{
	
}

//////////////////////////////////////////////////////////////////////////
void CWaitingWindow::onTouchEnded(Touch *touch, Event *unused_event)
{
	
}