#ifndef NBG_GAME_CONFIRM_WINDOW
#define NBG_GAME_CONFIRM_WINDOW

#include "cocos2d.h"

USING_NS_CC;

class CConfirmWindow : public Layer
{
public:
	CConfirmWindow();
	virtual ~CConfirmWindow();

	static float SHOW_ACTION_TIME;

    static CConfirmWindow* create();
    
	virtual bool init();  
	void Show();
	void Hide();
	bool IsShow(){return m_IsShow;}
	void SetOkLabel(std::string label);
	void SetCancelLabel(std::string label);
	void SetConfirmLabel(std::string label);

	void SetOkCallback(Ref* target, SEL_CallFuncN selector);
	void SetCancelCallback(Ref* target, SEL_CallFuncN selector);
private:	
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	void HideComplete(Node* sender);
	void ShowComplete(Node* sender);
	void OkBtnClick(Ref* sender);
	void CancelBtnClick(Ref* sender);

	Sprite* m_Back;
	LabelTTF * m_OkBtnLabel;
	LabelTTF* m_CancelBtnLabel;
	LabelTTF* m_DescriptionLabel;
	CCMenuItem* m_OkBtn;
	CCMenuItem* m_CancelBtn;
	CallFuncN* m_OkBtnCallback;
	CallFuncN* m_CancelBtnCallback;
	CCMenu* m_Menu;
	bool m_MenuClickState;

	Ref * m_OkTarget;
	SEL_CallFuncN m_OkSelector;
	Ref * m_CancelTarget;
	SEL_CallFuncN m_CancelSelector;

	bool m_IsShow;
};
#endif //NBG_GAME_CONFIRM_WINDOW