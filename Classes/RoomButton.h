#ifndef NBG_GAME_ROOM_BUTTON
#define NBG_GAME_ROOM_BUTTON

#include "cocos2d.h"

USING_NS_CC;

class CRoomButton : public Node
{
public:
	CRoomButton();
	virtual ~CRoomButton();

	Color3B ENABLE_LABEL_COLOR;
	Color3B DISABLE_LABEL_COLOR;
	Color3B BLINKING_BUTTON_COLOR;

	static const int START_X = -410;
	static const int START_Y = 290;
	static const int STEP_Y = 60;
	static float MOVE_ACTION_TIME;
	static const int BLINKING_ACTION_TAG = 1;

    static CRoomButton* create();
	virtual bool init();  

	void SetId(int id);
	void SetServerId(int server_id){m_ServerId = server_id;}
	void ChangeId(int id,int turn);
	int GetId(){return m_Id;}
	virtual void setEnable(bool visible);
	virtual bool isEnable();
	CCMenuItemImage* GetButton(){return m_Button;}
	void SetOpponentName(std::string opp_name);
	void SetLastWord(std::string lastWord);
	void SetLastAction(std::string lastAction);
	void MakeBlinkingAnim();
    void ButtonClick(Ref * Sender);

private:
	
	CCMenuItemImage * m_Button;
	LabelTTF* m_OppNameLabel;
	LabelTTF* m_LastWordLabel;
	LabelTTF* m_LastActionLabel;
	int m_Id;
	int m_ServerId;
};
#endif //NBG_GAME_ROOM_BUTTON