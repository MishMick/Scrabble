#ifndef NBG_GAME_SWAP_PANEL
#define NBG_GAME_SWAP_PANEL

/*
copyright 2014 New Bridge Games
email: vadim@newbridgegames.com
www: www.newbridgegames.com
*/

#include "cocos2d.h"
#include <vector>
#include <deque>
#include <map>

#include "Letter.h"

USING_NS_CC;

class CSwapPanel : public Layer
{
public:
	CSwapPanel();
	virtual ~CSwapPanel();

    static CSwapPanel* create();
    
	virtual bool init(); 	

	void PutLetterToSwapPanel(Touch * touch, CLetter * letter);
	void PutLetterToSwapPanel(int cell, CLetter * letter);
	void ClearCell(int cell);

	void ClearLetters();

	void Show();
	void Hide();
	void HideOnSwap();
	void CancelSwap();

	static const int DECK_Y_POS = 182;
	static const int CELL_SIZE_X = 62;
	static const int CELL_SIZE_Y = 62;
	static const int CELL_OFFSET_X = 388;
	static const int CELL_OFFSET_Y = 103;
private:			
	void StartSwap(Ref* sender);
	void CancelSwap(Ref* sender);

	std::vector<CLetter*>m_Letters;	

	Sprite* m_Background;
	CCMenuItem* m_OkBtn;
	CCMenuItem* m_CancelBtn;
};
#endif //NBG_GAME_SwAP_PANEL