#ifndef NBG_GAME_LETTER
#define NBG_GAME_LETTER

/*
copyright 2014 New Bridge Games
email: vadim@newbridgegames.com
www: www.newbridgegames.com
*/

#include "cocos2d.h"
#include <vector>
#include <map>

#include "Structures.h"

USING_NS_CC;

#define MOVE_ACTION_TAG  1
#define SCALE_ACTION_TAG 2

class CLetter : public Layer
{
public:
	CLetter();
	virtual ~CLetter();

    static CLetter* create();
    
	static const int LETTER_TAG = 112;
	static const int STAY_LAYER = 1;
	static const int FLY_LAYER = 4;
	static float SHOW_ACTION_TIME;
	static float MOVE_ACTION_TIME;

	enum LAST_POS_TYPE
	{
		LPT_DECK,
		LPT_SWAP,
		LPT_FIELD,
	};

	Color3B COLOR_NEW;
	Color3B COLOR_OLD;

	void SetCost(const int cost);
	int GetCost(){return m_Cost;}
	void SetLetter(char letter);
	char GetLetter(){return m_Letter;}
	void SetCell(SGridCell * cell){m_Cell = cell;}
	SGridCell * GetCell(){return m_Cell;}
	void SetDeckCell(const int cell){m_DeckCell = cell;}
	int GetDeckCell(){return m_DeckCell;}
	void SetSwapPanelCell(int cell){m_SwapPanelCell = cell;}
	int GetSwapPanelCell(){return m_SwapPanelCell;}
	void SetLastPosType(LAST_POS_TYPE type);
	LAST_POS_TYPE GetLastPosType(){return m_LastPosType;}
	void GoToLastPos();
	void UseRecall();
	void SetFixed();
	void BreakTouch(){m_IsDrag = false;}
	void SetTouchPause(bool pause);
	bool IsTouchPause();
	virtual bool init();  	
	void MoveToAction(cocos2d::Point pos);
	void MoveToAction(float x, float y);
	void ShowAction(float delay = 0.0f);
	void HideAction(float delay = 0.0f);
	void HideInstantly();
	void SetStayOnField();
	bool IsNew(){return m_IsNew;}
	void SetNew();
	void SetOld();
private:		
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	void SetInvisible(Node* sender);

	bool m_IsDrag;
	bool m_IsNew;

	int m_Cost;
	char m_Letter;

	SGridCell * m_Cell;
	int m_DeckCell;
	int m_SwapPanelCell;

	Sprite * m_Base;
	LabelTTF * m_CostLabel;
	LabelTTF * m_LetterLabel;
	LAST_POS_TYPE m_LastPosType;
	bool m_ListenerOnPause;

	Spawn * m_Spawn;
	Sequence * m_Sequence;
};
#endif //NBG_GAME_LETTER