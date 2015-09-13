#ifndef NBG_GAME_WAITING_WINDOW
#define NBG_GAME_WAITING_WINDOW

/*
copyright 2014 New Bridge Games
email: vadim@newbridgegames.com
www: www.newbridgegames.com
*/

#include "cocos2d.h"
#include <vector>
#include <deque>
#include <map>

#include "ui/UIScale9Sprite.h"

USING_NS_CC;
using namespace ui;


class CWaitingWindow : public Layer
{
public:
	CWaitingWindow();
	virtual ~CWaitingWindow();

	static const int ANIM_ACTION_TAG = 1;

    static CWaitingWindow* create();
	
    virtual bool init();

	void SetDesciptionString(std::string description);
	void Show();
	void Hide(bool instantly = false);
	void OnHideComplete(Node* sender);
	bool IsShow();
    
    void setShow(bool status);
    
private:			
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	void StartAnim();

	CCAnimate* m_AnimAction;
	Sprite* m_AnimSprite;
	bool m_IsShow;

};
#endif //NBG_GAME_WAITING_WINDOW