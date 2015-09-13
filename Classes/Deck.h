#ifndef NBG_GAME_DECK
#define NBG_GAME_DECK

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
#include "Stack.h"

USING_NS_CC;

class CDeck : public Layer
{
public:
	CDeck();
	virtual ~CDeck();

    static CDeck* create();
    
	static const int DECK_Y_POS = 91;
	static const int CELL_SIZE_X = 62;
	static const int CELL_SIZE_Y = 62;
	static const int CELL_OFFSET_X = 388;
	static const int CELL_OFFSET_Y = 12;
	static float ALIGN_LETTERS_TIME;
	static float SHUFFLE_LETTERS_TIME;

	virtual bool init(); 	
	virtual void setVisible(bool visible);
	CC_DEPRECATED_ATTRIBUTE void setTouchEnabled(bool value);

	void AddLetterFromStack();
	void FirstInitEmptyCells(CStack* stack);
	void InitEmptyCells();
	float InitEmptyCells(float delayPerLetter);
	void ClearCell(const int cell);	
	void PutLetterToDeck(Touch * touch, CLetter * letter);
	void PutLetterToDeck(int cell, CLetter * letter);
	void PutLetterToEmptyCell(CLetter * letter);
	void PutLetterToEmptyCell(char letterChar, int cost);
	void CheckCellSwap(Touch * touch,CLetter * activeLetter);
	void MakeShuffle();
	void AlignLetter(bool instantly = false);
	void ShowLetters(float delay = 0.0f);
	void HideLetters(float delay = 0.0f);
	void HideLettersInstantly();
	void Clear();
	int GetLettersCount();
	std::string GetDeckInString();
private:			
	std::vector<CLetter*>m_Letters;	
	
};
#endif //NBG_GAME_DECK