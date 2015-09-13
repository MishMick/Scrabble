#ifndef NBG_GAME_ROOMS_TABLE
#define NBG_GAME_ROOMS_TABLE

#include "cocos2d.h"
#include "RoomButton.h"

USING_NS_CC;

class CRoomsTable : public Node
{
public:
	CRoomsTable();
	virtual ~CRoomsTable();

	static const int TABLE_WIDTH = 200;
	static const int TABLE_HEIGHT = 600;
	static const int MARKER_HEIGHT = 30;
	static const int BUTTON_HEIGHT = 60;

	enum TABLE_TYPE
	{
		TT_YOUR_MOVE,
		TT_THEIR_MOVE,
		TT_FIND_OPPONENT,
		TT_GAME_OVER,
		TT_COUNT
	};

    static CRoomsTable* create();

	virtual bool init();  
	
	void MarkerAlignLeft();
	void MarkerAlignRight();
	void SetMarkerLabel(std::string label);
	int GetTableHeight();
	int GetTableSize(){return m_Buttons.size();}
	bool IsEmpty(){return m_Buttons.empty();}
	void PutToTable(CRoomButton* button);
	void DeleteFromTable(CRoomButton* button);
	void RebuildButtonsPos();
	int GetFirstRoomId();
private:
	Sprite* m_Marker;
	LabelTTF * m_MarkerLabel;
	CCMenu* pMenu;
	std::vector<CRoomButton*> m_Buttons;
};
#endif //NBG_GAME_ROOMS_TABLE