#ifndef NBG_GAME_LOSE_WINDOW
#define NBG_GAME_LOSE_WINDOW

#include "cocos2d.h"

USING_NS_CC;

class CLoseWindow : public Layer
{
public:
	CLoseWindow();
	virtual ~CLoseWindow();

    static CLoseWindow* create();
    
	virtual bool init();  
private:	
	void ProceedClick(Object* sender);
	Sprite * m_Panel;	

	CCMenuItem * m_Restart;
};
#endif //NBG_GAME_BALL