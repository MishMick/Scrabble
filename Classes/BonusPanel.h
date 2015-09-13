#ifndef NBG_GAME_BONUS_PANEL
#define NBG_GAME_BONUS_PANEL

/*
copyright 2014 New Bridge Games
email: vadim@newbridgegames.com
www: www.newbridgegames.com
*/

#include "cocos2d.h"
#include <vector>
#include <deque>
#include <map>
#include "MenuItemWithDescription.h"

USING_NS_CC;

class CBonusPanel : public Layer
{
public:
	CBonusPanel();
	virtual ~CBonusPanel();

    static CBonusPanel* create();
    
	virtual bool init(); 	
	enum BUTTONS_TAGS
	{
		BT_RESIGN,
		BT_WORDMETTER_OFF,
		BT_WORDMETTER_ON
	};

	void Show();
	void Hide();

	void ConfirmOkCallback(Node* sender);
	void SetWordMetterState(bool on);
	CMenuItemWithDesciption* GetResignBtn(){return m_ResignBtn;}
	CMenuItemWithDesciption* GetWordMetterOnBtn(){return m_WordMetterOnBtn;}
	CMenuItemWithDesciption* GetWordMetterOffBtn(){return m_WordMetterOffBtn;}
	bool IsShow(){return m_IsShow;}
private:			
	void ResingBtnClick(Ref* sender);
	void WordMetterStateSwap(Ref* sender);
	void OnShowComplete(Node* sender);
	void OnHideComplete(Node* sender);

	Sprite* m_Background;
	CMenuItemWithDesciption* m_ResignBtn;
	CMenuItemWithDesciption* m_WordMetterOnBtn;
	CMenuItemWithDesciption* m_WordMetterOffBtn;

	bool m_IsWordMetterOn;
	bool m_IsShow;
};
#endif //NBG_GAME_BONUS_PANEL