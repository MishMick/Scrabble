#ifndef NBG_STRUCTURES
#define NBG_STRUCTURES


#include "cocos2d.h"
USING_NS_CC;

enum Modifier
{
	M_Empty			= -1,
	M_Plus5			= 0,
	M_Plus7			= 1,
	M_Plus10		= 2,
	M_Plus15		= 3,
	M_Plus20		= 4,
	M_Plus25		= 5,
	M_Plus30		= 6,
	M_Minus5		= 7,
	M_Minus7		= 8,
	M_Minus10		= 9,
	M_Minus15		= 10,
	M_Minus20		= 11,
	M_Minus25		= 12,
	M_Minus30		= 13,
	M_DoubleWord	= 14,
	M_DoubleLetter	= 15,
	M_TripleWord	= 16,
	M_TripleLetter	= 17,
	M_QuadWord		= 18,
	M_QuadLetter	= 19,
	M_StartPlace	= 20,
	M_Normal		= 21,
	M_Plus2			= 22,
};

struct SGridCell
{		
	char letter;
	bool isWithoutScore;
	Modifier modifier;
	cocos2d::Point pos;
	int x;
	int y;
	bool fixed;
};


#endif