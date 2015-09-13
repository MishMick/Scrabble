#include "Thimble.h"

#include "GameScene.h"

//////////////////////////////////////////////////////////////////////////
CThimble::CThimble():cocos2d::Layer()
{
	m_Thimble = NULL;
	m_State = State_No;
	m_IsLast = false;
	m_Disabled = true;
	m_WithBall = false;
}

//////////////////////////////////////////////////////////////////////////
CThimble::~CThimble()
{

}

CThimble* CThimble::create() {
    CThimble* obj = new CThimble();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CThimble::init()
{	   
	m_Thimble = Sprite::create("game/elements/thimble/base_box.png");
	m_Shadow = Sprite::create("game/elements/thimble/shadow.png");
	m_Shadow->setPosition(15.0f,-55.0f);

	addChild(m_Shadow, 0);
	addChild(m_Thimble, 1);
	setScale(0.5f);		

	//Initializing and binding 
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CThimble::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(CThimble::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	ChangeState(State_Lie);	
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CThimble::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (m_Disabled == false && m_Thimble->getBoundingBox().containsPoint(this->convertTouchToNodeSpace(touch))) {
		return true;
	}
	return false;	
}

//////////////////////////////////////////////////////////////////////////
void CThimble::onTouchEnded(Touch *touch, Event *unused_event)
{
	ChangeState(State_Open);
	//CGameScene::Instance()->BlockAllThimbles();
}

//////////////////////////////////////////////////////////////////////////
void CThimble::ChangeState(const ThimbleState state, const float delay)
{
	if (m_State == state)return;
	m_State = state;

	if (state == State_Lie)
	{
		m_Thimble->setRotation(-110.0f);
		m_Thimble->setScale(0.8f);
		m_Shadow->setScale(0.8f);
	}
	else if (state == State_FlyToField)
	{
		float upTime	= 0.5f;
		float downTime	= 0.55f;
		float delayTime = delay;

		// ������ � ������� �������
		MoveBy* thimbleMove1		= MoveBy::create(upTime, Point(0, 150));
		RotateTo* thimbleRotate1	= RotateTo::create(upTime, -55.0f);		
		//���������� ���� � ���� � ������������
		FadeTo* shadowOpacity1		= FadeTo::create(upTime, 100);
		ScaleTo* shadowScale1		= ScaleTo::create(upTime, 0.6f);

		//�������� ��������� ��������
		Spawn * spawnThimble1	= Spawn::create(thimbleMove1, thimbleRotate1, NULL);
		Spawn * spawnShadow1	= Spawn::create(shadowOpacity1, shadowScale1,NULL);

		//����� � ������� �����
		DelayTime *thimbleDelay1	= DelayTime::create(delayTime);
		DelayTime *shadowDelay1		= DelayTime::create(delayTime);

		//�������, ������� ������ ���� ������
		CallFuncN *ballLayerFunc = CallFuncN::create(this, callfuncN_selector(CThimble::SwapLayerWithBall));
		//�������, ������� ���������� ��� ���������� ��������
		CallFuncN *appearEndFunc = CallFuncN::create(this, callfuncN_selector(CThimble::OnAppearEnds));

		// �������� ������
		_ccBezierConfig conf;
		conf.endPosition = Point(0, -250);
		BezierBy* thimbleMove2		= BezierBy::create(downTime, conf);		
		ScaleTo* thimbleScale2		= ScaleTo::create(downTime, 1.0f);
		RotateTo* thimbleRotate2	= RotateTo::create(downTime, 0.0f);
		//���������� ���� � ������ ����� ������ �
		FadeTo* shadowOpacity2		= FadeTo::create(downTime, 255);
		ScaleTo* shadowScale2		= ScaleTo::create(downTime, 1.0f);		
		MoveBy* shadowMove2			= MoveBy::create(downTime, Point(0, -100));

		//�������� ��������� ��������
		Spawn * spawnThimble2	= Spawn::create(thimbleMove2, thimbleScale2,thimbleRotate2, NULL);
		Spawn * spawnShadow2	= Spawn::create(shadowOpacity2, shadowScale2,shadowMove2, NULL);

		Sequence * seq1	= Sequence::create(thimbleDelay1, spawnThimble1,ballLayerFunc, spawnThimble2, appearEndFunc, NULL);
		Sequence * seq2	= Sequence::create(shadowDelay1, spawnShadow1,spawnShadow2,NULL);

		m_Thimble->runAction(seq1);
		m_Shadow->runAction(seq2);
	}
	else if (state == State_Open || state == State_OpenWithBonus)
	{
		float upTime	= 0.5f;
		float downTime	= 0.55f;
		float delayTime = delay;

		// ������ � ������� �������
		MoveBy* thimbleMove1		= MoveBy::create(upTime, Point(0, 250));
		RotateTo* thimbleRotate1	= RotateTo::create(upTime, -55.0f);		
		//���������� ���� � ���� � ������������
		FadeTo* shadowOpacity1		= FadeTo::create(upTime, 100);
		ScaleTo* shadowScale1		= ScaleTo::create(upTime, 0.6f);

		//�������� ��������� ��������
		Spawn * spawnThimble1	= Spawn::create(thimbleMove1, thimbleRotate1, NULL);
		Spawn * spawnShadow1	= Spawn::create(shadowOpacity1, shadowScale1,NULL);

		//����� � ������� �����
		DelayTime *thimbleDelay1	= DelayTime::create(delayTime);
		DelayTime *shadowDelay1		= DelayTime::create(delayTime);

		//�������, ������� ������ ���� ������
		CallFuncN *ballLayerFunc = CallFuncN::create(this, callfuncN_selector(CThimble::SwapLayerWithBallEnd));

		//�������, ������� ���������, ����� �� ��������� ���������� ��������
		CallFuncN *rightCheckFunc = CallFuncN::create(this, callfuncN_selector(CThimble::OnCheckLaunchRightThimble));

		

		// �������� ������
		_ccBezierConfig conf;
		conf.endPosition = Point(0, -150);
		BezierBy* thimbleMove2		= BezierBy::create(downTime, conf);		
		ScaleTo* thimbleScale2		= ScaleTo::create(downTime, 0.8f);
		RotateTo* thimbleRotate2	= RotateTo::create(downTime, -110.0f);
		//���������� ���� � ������ ����� ������ �
		FadeTo* shadowOpacity2		= FadeTo::create(downTime, 255);
		ScaleTo* shadowScale2		= ScaleTo::create(downTime, 0.8f);		
		MoveBy* shadowMove2			= MoveBy::create(downTime, Point(0, 100));

		//�������� ��������� ��������
		Spawn * spawnThimble2	= Spawn::create(thimbleMove2, thimbleScale2,thimbleRotate2, NULL);
		Spawn * spawnShadow2	= Spawn::create(shadowOpacity2, shadowScale2,shadowMove2, NULL);
		
		Sequence * seq1 = NULL;
		Sequence * seq2 = NULL;
		if (state == State_OpenWithBonus)
		{
			seq1	= Sequence::create(spawnThimble1,spawnThimble2,NULL);
			seq2	= Sequence::create(spawnShadow1,spawnShadow2,NULL);
		}
		else
		{
			seq1	= Sequence::create(spawnThimble1,ballLayerFunc, spawnThimble2,rightCheckFunc, NULL);
			seq2	= Sequence::create(spawnShadow1,spawnShadow2,NULL);

		}
		
		
		m_Thimble->runAction(seq1);
		m_Shadow->runAction(seq2);
	}
}

//////////////////////////////////////////////////////////////////////////
void CThimble::SwapLayerWithBall(Node * sender)
{
	CGameScene * scene = CGameScene::Instance();
//	scene->SetBallLayer(0);
}


//////////////////////////////////////////////////////////////////////////
void CThimble::SwapLayerWithBallEnd(Node * sender)
{
	CGameScene * scene = CGameScene::Instance();
	if (IsWithBall())
	{
	//	scene->SetBallLayer(10000);		
	//	scene->SetWin();
	}	
}

//////////////////////////////////////////////////////////////////////////
void CThimble::OnAppearEnds(Node * sender)
{
	ChangeState(State_StayOnField);
	if (m_IsLast)
	{
		CGameScene * scene = CGameScene::Instance();
//		scene->StartThimblesAnimation();
	}
}

//////////////////////////////////////////////////////////////////////////
void CThimble::OnCheckLaunchRightThimble(Node* sender)
{
	CGameScene * scene = CGameScene::Instance();
//	scene->CheckWin();
}

//////////////////////////////////////////////////////////////////////////
bool CThimble::IsActive()
{
	return m_State == State_StayOnField;
}