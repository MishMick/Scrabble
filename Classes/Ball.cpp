#include "Ball.h"

//////////////////////////////////////////////////////////////////////////
CBall::CBall():cocos2d::Layer()
{
	m_Ball = NULL;
	m_Shadow = NULL;
}

//////////////////////////////////////////////////////////////////////////
CBall::~CBall()
{

}

CBall* CBall::create() {
    CBall *obj = new CBall();
    obj->init();
    return obj;
}
//////////////////////////////////////////////////////////////////////////
bool CBall::init()
{	   
    m_Ball = Sprite::create("game/elements/ball/base_ball.png");
	m_Shadow = Sprite::create("game/elements/ball/shadow.png");
	m_Shadow->setPosition(15.0f,-25.0f);

    addChild(m_Shadow, 0);
	addChild(m_Ball, 1);

	setScale(0.5f);
		
	return true;
}



