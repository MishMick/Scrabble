#ifndef NBG_GAME_BALL
#define NBG_GAME_BALL

#include "cocos2d.h"

USING_NS_CC;

class CBall : public Layer
{
public:
	CBall();
	virtual ~CBall();

    static CBall* create();
	virtual bool init();  
private:
	Sprite * m_Ball;
	Sprite * m_Shadow;

};
#endif //NBG_GAME_BALL