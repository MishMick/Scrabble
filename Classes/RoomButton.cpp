#include "RoomButton.h"
#include "GameScene.h"

float CRoomButton::MOVE_ACTION_TIME = 0.2;
//////////////////////////////////////////////////////////////////////////
CRoomButton::CRoomButton():cocos2d::Node()
{
	
}

//////////////////////////////////////////////////////////////////////////
CRoomButton::~CRoomButton()
{

}

CRoomButton* CRoomButton::create() {
    CRoomButton *obj = new CRoomButton();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CRoomButton::init()
{
	ENABLE_LABEL_COLOR = Color3B(0,0,0);
	DISABLE_LABEL_COLOR = Color3B(255,255,255);
	BLINKING_BUTTON_COLOR = Color3B(238,153,20);

	m_Button = CCMenuItemImage::create(
		"room.png",
		"room_over.png",
		"room_disable.png",
		this,
		menu_selector(CRoomButton::ButtonClick));
	
	CCMenu* pMenu = CCMenu::create(m_Button,NULL);
	pMenu->setPosition(Point(0.0f,0.0f));

    CCLOG("reference count four :: %d",this->getReferenceCount());
    
	m_OppNameLabel = LabelTTF::create("","helvetica",18);	
	m_OppNameLabel->setColor(ENABLE_LABEL_COLOR);
	m_OppNameLabel->setPosition(10,m_Button->getContentSize().height/2+10);
	m_OppNameLabel->setAnchorPoint(Point(0.0f,0.5f));
	m_Button->addChild(m_OppNameLabel);

	m_LastWordLabel = LabelTTF::create("","helvetica",16);	
	m_LastWordLabel->setColor(ENABLE_LABEL_COLOR);
	m_LastWordLabel->setPosition(10,m_Button->getContentSize().height/2-15);
	m_LastWordLabel->setAnchorPoint(Point(0.0f,0.5f));
	m_Button->addChild(m_LastWordLabel);

	m_LastActionLabel = LabelTTF::create("","helvetica",16);	
	m_LastActionLabel->setColor(ENABLE_LABEL_COLOR);
	m_LastActionLabel->setPosition(10,m_Button->getContentSize().height/2-15);
	m_LastActionLabel->setAnchorPoint(Point(0.0f,0.5f));
	m_Button->addChild(m_LastActionLabel);

	addChild(pMenu);
    
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CRoomButton::SetId(int id)
{
	m_Id = id;
	/*
	Point pos = Point(START_X,
						START_Y - m_Id*STEP_Y);
	setPosition(pos);
	*/
}

//////////////////////////////////////////////////////////////////////////
void CRoomButton::ChangeId(int id, int turn)
{
	m_Id = id;
	Point pos = Point(START_X,
		START_Y - m_Id*STEP_Y);
	auto moveAction = MoveTo::create(MOVE_ACTION_TIME,pos);
	float delay = turn*MOVE_ACTION_TIME;
	auto delayAction = DelayTime::create(delay);
	auto sequence = Sequence::create(delayAction,moveAction,NULL);
	runAction(sequence);
}

//////////////////////////////////////////////////////////////////////////
void CRoomButton::SetOpponentName(std::string opp_name)
{
	m_OppNameLabel->setString(opp_name);
}

//////////////////////////////////////////////////////////////////////////
void CRoomButton::setEnable(bool enable)
{
	if(enable)
	{
		m_OppNameLabel->setColor(ENABLE_LABEL_COLOR);
	}
	else
	{
		m_OppNameLabel->setColor(DISABLE_LABEL_COLOR);
	}
	m_Button->setEnabled(enable);
}

//////////////////////////////////////////////////////////////////////////
bool CRoomButton::isEnable()
{
	return m_Button->isEnabled();
}

//////////////////////////////////////////////////////////////////////////
void CRoomButton::ButtonClick(Ref * sender)
{
	CGameScene * scene = CGameScene::Instance();
	m_Button->stopActionByTag(BLINKING_ACTION_TAG);
	
	if(scene->IsChangingRooms())
	{
		return;
	}
	if(scene->GetPlayer()->IsRoomLoaded(m_Id))
	{
		scene->ChangeRoom(m_Id);
	}
	else
	{
		scene->ShowWaitingAnim();
		scene->GetPlayer()->LoadRoomData(m_ServerId);
	}
    
    //#ChatLayer
    CGameScene::Instance()->showChatRelatedThing();
}


//////////////////////////////////////////////////////////////////////////
void CRoomButton::SetLastWord(std::string lastWord)
{
	if(lastWord != "")
	{
		lastWord+= " :";
	}
	m_LastWordLabel->setString(lastWord);
	Point pos = m_LastWordLabel->getPosition();
	pos.x += m_LastWordLabel->getContentSize().width;
	m_LastActionLabel->setPosition(pos.x + 5, pos.y);
}

//////////////////////////////////////////////////////////////////////////
void CRoomButton::SetLastAction(std::string lastAction)
{
	m_LastActionLabel->setString(lastAction);
}

//////////////////////////////////////////////////////////////////////////
void CRoomButton::MakeBlinkingAnim()
{
	TintTo* colorAction = TintTo::create(0.3,BLINKING_BUTTON_COLOR.r,BLINKING_BUTTON_COLOR.g,BLINKING_BUTTON_COLOR.b);
	TintTo* colorAction2 = TintTo::create(0.3,255,255,255);
	Sequence* sequence = Sequence::create(colorAction,colorAction2,colorAction,colorAction2,colorAction,colorAction2,NULL);
	sequence->setTag(BLINKING_ACTION_TAG);
	m_Button->runAction(sequence);
}