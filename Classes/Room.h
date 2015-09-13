#ifndef NBG_GAME_ROOM
#define NBG_GAME_ROOM

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
#include "Deck.h"
#include "Field.h"
#include "ScorePointer.h"
#include "RoomButton.h"

USING_NS_CC;

class CRoom : public Node
{
public:
	CRoom();
	virtual ~CRoom();

	enum ROOM_TYPE
	{
		RT_ONE_COMPUTER,
		RT_WITH_OPPONENT,
	};

	enum ROOM_STATE
	{
		RS_IN_GAME,
		RS_END_GAME,
		RS_ENEMY_TURN,
		RS_SWAP_PANEL_SHOW,
		RS_BONUS_PANEL_SHOW,
		RS_CHECK_WORD,
		RS_CHOOSE_EMPTY_LETTER,
		RS_CHANGE_PLAYERS,
		RS_HIDED,
		RS_NONE,
	};

	enum ROOM_TURN
	{
		RT_FIRST_PLAYER,
		RT_SECOND_PLAYER,
		RT_OPPONENT
	};

    static CRoom* create(int boardType);

	virtual bool init(int boardType); 	
	void Show();
	void Hide();
	void Rematch();
	void Remove();

	void SetScorePointerVisible(bool visible);

	void SetId(int id);
	void ChangeId(int id, int turn);
	int GetId(){return m_Id;}
	void SetType(ROOM_TYPE type){m_Type = type;}
	ROOM_TYPE GetType(){return m_Type;}
	void SetState(ROOM_STATE state);
	void ReturnOldState();
	ROOM_STATE GetState(){return m_State;}
	ROOM_STATE GetOldState(){return m_OldState;}
	void SetTurn(ROOM_TURN turn){m_Turn = turn;}
	ROOM_TURN GetTurn(){return m_Turn;}
	// score's
	void SetFirstPlayerScore(int score){m_ScorePlayer_1 = score;}
	void SetSecondPlayerScore(int score){m_ScorePlayer_2 = score;}
	int GetFirstPlayerScore(){return m_ScorePlayer_1;}
	int GetSecondPlayerScore(){return m_ScorePlayer_2;}
	void AddCurrentPlayerScore(int score);
	//pass in a row
	void SetPassInARowCount(int count){m_PassInARowCount = count;}
	int GetPassInARowCount(){return m_PassInARowCount;}
	void IncreasePassInARowCount(){m_PassInARowCount++;}
	void CleanupPassInRow(){m_PassInARowCount = 0;}
	bool IsLoaded(){return m_IsLoaded;}
	void SetLoaded(bool loaded){m_IsLoaded = loaded;}
	void SetBlandData(int server_id, int turn, int uid1, int uid2, std::string opp_name);
	int GetServerId(){return m_ServerId;}

	CRoomButton* GetRoomButton(){return m_Button;}
	CScorePointer* GetCurrentPlayerScorePointer();
	void SwapScorePointerState();
	//deck's
	CDeck* GetFirstPlayerDeck(){return m_DeckPlayer_1;}
	CDeck* GetSecondPlayerDeck(){return m_DeckPlayer_2;}
	CDeck* GetCurrentPlayerDeck();
	//stack
	CStack* GetStack(){return m_Stack;}
	//field
	CField* GetField(){return m_Field;}
	//names
	void SetFirstPlayerName(std::string name){m_FirstPlayerName = name;}
	void SetSecondPlayerName(std::string name){m_SecondPlayerName = name;}
	std::string GetFirstPlayerName(){return m_FirstPlayerName;}
	std::string GetSecondPlayerName(){return m_SecondPlayerName;}
private:			
	bool m_IsLoaded;
	int m_Id;
	int m_ServerId;
	ROOM_TYPE m_Type;
	ROOM_STATE m_State;
	ROOM_STATE m_OldState;
	ROOM_TURN m_Turn;
	int m_ScorePlayer_1;
	int m_ScorePlayer_2;
	int m_PassInARowCount;
	CRoomButton* m_Button;
	CScorePointer* m_ScorePointer_1;
	CScorePointer* m_ScorePointer_2;
	CDeck* m_DeckPlayer_1;
	CDeck* m_DeckPlayer_2;
	CStack* m_Stack;
	CField* m_Field;
	std::string m_FirstPlayerName;
	std::string m_SecondPlayerName;
	int m_Uid1;
	int m_Uid2;
};
#endif //NBG_GAME_ROOM