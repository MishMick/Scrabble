#ifndef NBG_GAME_CHOOSE_LETTER_WINDOW
#define NBG_GAME_CHOOSE_LETTER_WINDOW

#include "cocos2d.h"
#include "Letter.h"

USING_NS_CC;

class CChooseLetterWindow : public Layer
{
public:
	CChooseLetterWindow();
	virtual ~CChooseLetterWindow();

    static CChooseLetterWindow* create();
    
	virtual bool init();  
	void Show(CLetter * letter);
	void Hide();
private:	
	void LetterBtnClick(Ref* sender);
	CCMenu* m_pMenu;
	Sprite* m_Back;
	CLetter * m_ChoosenLetter;
};
#endif //NBG_GAME_CHOOSE_LETTER_WINDOW