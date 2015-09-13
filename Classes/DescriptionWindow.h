#ifndef NBG_GAME_DESCRIPTION_WINDOW
#define NBG_GAME_DESCRIPTION_WINDOW

/*
copyright 2014 New Bridge Games
email: vadim@newbridgegames.com
www: www.newbridgegames.com
*/

#include "cocos2d.h"
#include <vector>
#include <deque>
#include <map>
#include "ui/UIScale9Sprite.h"

USING_NS_CC;
using namespace ui;

class CDescriptionWindow : public Layer
{
public:
	CDescriptionWindow();
	virtual ~CDescriptionWindow();

    static CDescriptionWindow* create();
    
	enum ALIGN_TYPE
	{
		AT_OVER,
		AT_BOTTOM,
		AT_LEFT,
		AT_RIGHT,
		AT_NONE
	};

	static const int WIDTH = 300;
	static const int HORIZONTAL_INTENT = 15;
	static const int VERTICAL_INTENT = 17;

	virtual bool init(); 	

	void SetDesciptionString(std::string description);
	void Show();
	void Hide();
	void OnHideComplete(Node* sender);
	bool IsShow(){return m_IsShow;}
	void Align(ALIGN_TYPE type);
	void RebuildPos();
	void SetTarget(CCNode* target){m_Target = target;}
	void ShowInstantly();
	void HideInstantly();
private:			
	bool m_IsShow;
	std::string m_DescriptionString;
	LabelTTF* m_DescriptionLabel;
	Scale9Sprite * m_Sprite9;
	ALIGN_TYPE m_AlignType;
	CCNode* m_Target;
};
#endif //NBG_GAME_DESCRIPTION_WINDOW