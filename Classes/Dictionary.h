#ifndef NBG_GAME_DICTIONARY
#define NBG_GAME_DICTIONARY

/*
copyright 2014 New Bridge Games
email: vadim@newbridgegames.com
www: www.newbridgegames.com
*/

#include "cocos2d.h"
#include <vector>
#include <map>

USING_NS_CC;

class CDictionary
{
public:
	CDictionary();
	~CDictionary();

	bool Init();  
	bool FindWord(std::string &word);
private:	
	void CreateBinaryDump(const std::string &path);

	struct SLetterOffset
	{
		int first;
		int end;
		char ch;
	};
	std::map<char,SLetterOffset>m_LetterOffsets;
	std::vector<std::string> m_Strings;
};
#endif //NBG_GAME_DICTIONARY