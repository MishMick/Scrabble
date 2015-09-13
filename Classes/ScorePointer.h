#ifndef NBG_GAME_SCORE_POINTER
#define NBG_GAME_SCORE_POINTER

#include "cocos2d.h"

USING_NS_CC;

class CScorePointer : public Layer
{
public:
	CScorePointer();
	virtual ~CScorePointer();

    static CScorePointer* create();
    
	static const int BIG_FONT_SIZE = 36;
	static const int SMALL_FONT_SIZE = 24;
	static const int PASSIVE_LAYER = 2;
	static const int ACTIVE_LAYER = 3;
	virtual bool init();  

	void Hide();
	void Show();
	void SetActive(bool active);
	void SetScore(int score);
private:
	Sprite * m_Back;
	LabelTTF * m_ScoreLabel;

};
#endif //NBG_GAME_SCORE_POINTER