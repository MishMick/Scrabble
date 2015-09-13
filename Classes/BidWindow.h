#ifndef NBG_GAME_BID_WINDOW
#define NBG_GAME_BID_WINDOW

#include "cocos2d.h"

USING_NS_CC;

class CBidWindow : public Layer
{
public:
	CBidWindow();
	virtual ~CBidWindow();
    
    static CBidWindow* create();
	virtual bool init();  
private:
	void ProceedBidClick(Ref* sender);
	Sprite * m_Panel;	


	CCMenuItem *m_Bid50;
	CCMenuItem *m_Bid100;
	CCMenuItem *m_Bid250;
	CCMenuItem *m_Bid500;
};
#endif //NBG_GAME_BALL