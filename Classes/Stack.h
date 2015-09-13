#ifndef NBG_GAME_STACK
#define NBG_GAME_STACK

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

class CStack
{
public:
	CStack();
	~CStack();

	bool Init(); 
	void CreateStack();	
	void PushLetterToStack(char letterChar, int cost);
	CLetter* GetLetterFromStack();
	CLetter* SwapLetter(CLetter* letterToStack, int count);
	int GetLetterCost(char letter){return m_LettersCost[letter];}
	void ShuffleLetters();
	void SetStackRemainLabel();
	int GetRemainLettersCount();
	bool IsEmpty();
	void Clear();
	std::string GetStackInString(int from = 0);
	std::string GetFirstLettersInString(int count);
private:			
	std::deque<CLetter*>m_Letters;
	std::map<char,int>m_LettersCost;
};
#endif //NBG_GAME_DICTIONARY