#ifndef NBG_GAME_FIELD
#define NBG_GAME_FIELD

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

class CField : public Layer
{
public:
	CField();
	virtual ~CField();

	static const int GRID_WIDTH		= 24;
	static const int GRID_HEIGHT	= 18;
	static const int SCORE_POINTER_LAYER  = 3;
	static float SHOW_TIME;

    static CField* create(int type);
    
    virtual bool init(int type);

	void Clear();
	void Delete();
    cocos2d::Point GetFieldOffset(){return m_FieldOffset;}
	Sprite * GetFieldSprite(){return m_Field;}
	SGridCell * GetCellUnderCursor(Touch * touch);
	SGridCell * GetCellUnderCursor(cocos2d::Point pos);
	SGridCell * GetCell(const int x, const int y);
	SGridCell * GetNearestEmptyCell(SGridCell * cell);
	SGridCell* GetCellForIndex(int index);

	int GetIndexByPos(cocos2d::Point pos);

	cocos2d::Point GetUtmostVerticalCellPos(int startX, int startY);
	cocos2d::Point GetUtmostHorizontalCellPos(int startX, int startY);

	std::string GetWordInColumn(const int col, const int startY, int &score);
	std::string GetWordInRow(const int row, const int startX, int &score);

	bool CheckVerticalValid(int x, int y1, int y2);
	bool CheckHorizontalValid(int y, int x1, int x2);
	bool CheckVerticalFixedTouch(int x, int y1, int y2);
	bool CheckHorizontalFixedTouch(int y, int x1, int x2);

	void SetPlasedFirstWord(bool plased){m_IsFirstWordPlased = plased;}
	bool IsFirstWordPlased(){return m_IsFirstWordPlased;}

	void Show();
	void Hide();
	
	void AddLetterForIndex(char letter, int index, int cost, bool makeNew = false);
	void ClearNewLetters();
private:
	void InitGrid(int type);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
//	virtual void onZoom(Event *event);
//#endif
    
	void setInvisible(Node * sender);
	SGridCell m_Grid[GRID_WIDTH][GRID_HEIGHT];

	int m_CellWidth;
	int m_CellHeight;

	int m_StartOffsetX;
	int m_StartOffsetY;

	Sprite * m_Field;
	Sprite * m_Over;

	cocos2d::Point m_FieldOffset;

	bool m_IsDrag;
	cocos2d::Point m_DragPos;	

	bool m_IsFirstWordPlased;
};
#endif //NBG_GAME_LETTER