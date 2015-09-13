#include "ScorePointer.h"

//////////////////////////////////////////////////////////////////////////
CScorePointer::CScorePointer():cocos2d::Layer()
{
	
}

//////////////////////////////////////////////////////////////////////////
CScorePointer::~CScorePointer()
{

}

CScorePointer* CScorePointer::create() {
    CScorePointer *obj = new CScorePointer();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CScorePointer::init()
{	   
	m_Back = Sprite::create("active.png");	
	addChild(m_Back);

	m_ScoreLabel = LabelTTF::create("0","helvetica",BIG_FONT_SIZE);
	m_ScoreLabel->setColor(Color3B(255,255,255));
	addChild(m_ScoreLabel);;
	
	setCascadeOpacityEnabled(true);
	Hide();

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CScorePointer::Hide()
{
	m_Back->setVisible(false);
	m_ScoreLabel->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
void CScorePointer::Show()
{
	if(m_ScoreLabel->getString().size()>3)
	{
		m_ScoreLabel->setFontSize(SMALL_FONT_SIZE);
	}
	else
	{
		m_ScoreLabel->setFontSize(BIG_FONT_SIZE);
	}
	m_Back->setVisible(true);
	m_ScoreLabel->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////
void CScorePointer::SetActive(bool active)
{
	if(active)
	{
		m_Back->setTexture("active.png");
		setScale(1.0f);
		setZOrder(ACTIVE_LAYER);
	}
	else
	{
		m_Back->setTexture("passive.png");
		setScale(0.8f);
		setZOrder(PASSIVE_LAYER);
	}
}

//////////////////////////////////////////////////////////////////////////
void CScorePointer::SetScore(int score)
{
	m_ScoreLabel->setString(StringUtils::toString(score));
}