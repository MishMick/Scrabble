#ifndef NBG_GAME_CHANGE_PLAYER_WINDOW
#define NBG_GAME_CHANGE_PLAYER_WINDOW

#include "cocos2d.h"

USING_NS_CC;

class CChangePlayerWindow : public Layer
{
public:
	CChangePlayerWindow();
	virtual ~CChangePlayerWindow();

    static CChangePlayerWindow* create();
    
	virtual bool init();  
	void Show(std::string playerName);
	void Hide();
private:	
	Sprite* m_Back;
	LabelTTF * m_PlayerName;
	LabelTTF * m_TurnLabel;
};
#endif //NBG_GAME_CHANGE_PLAYER_WINDOW