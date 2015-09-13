#ifndef NBG_GAME_END_LEVEL_WINDOW
#define NBG_GAME_END_LEVEL_WINDOW

#include "cocos2d.h"

USING_NS_CC;

class CEndLevelWindow : public Layer
{
public:
	CEndLevelWindow();
	virtual ~CEndLevelWindow();

	static float SHOW_ACTION_TIME;

    static CEndLevelWindow* create();
    
	virtual bool init();  
	void Show(int player_1_score, int player_2_score);
	void Hide();
	bool IsShow(){return m_IsShow;}
private:	
	void HideComplete(Node* sender);
	void ShowComplete(Node* sender);
	void RematchButtonClick(Ref* sender);
	void CloseButtonsClick(Ref* sender);

	Sprite* m_Back;
	LabelTTF * m_ResultLabel;
	LabelTTF* m_RematchLabel;
	CCMenuItem* m_RematchButton;
	CCMenuItem* m_CloseButton;

	bool m_IsShow;
};
#endif //NBG_GAME_END_LEVEL_WINDOW