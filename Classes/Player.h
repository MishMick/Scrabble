#ifndef NBG_GAME_PLAYER
#define NBG_GAME_PLAYER

/*
copyright 2014 New Bridge Games
email: vadim@newbridgegames.com
www: www.newbridgegames.com
*/

#include "cocos2d.h"
#include <vector>
#include <deque>
#include <map>
#include "Room.h"
#include "json/rapidjson.h"
#include "json/reader.h"
#include "json/document.h"

#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"

USING_NS_CC;

static const int MAX_ROOM_COUNT = 5;

struct PLAYER_MOVE;
class CPlayer : public cocos2d::Layer
{
public:
	CPlayer();
	virtual ~CPlayer();

	static std::string SERVER_URL;

    std::string lastSentMessage;
    
	virtual bool init(); 	
	void SetCurrentRoomId(int id){m_CurrentRoomId = id;}
	int GetCurrentRoomId(){return m_CurrentRoomId;}

	bool IsWordMetterOn(){return m_IsWordMetterOn;}
	void SwapWordMetterState();

	CRoom* GetCurrentRoom();
	std::string GetName(){return m_Name;}
	int GetLetterCost(char letter){return m_LettersCost[letter];}
	void LoadRoomData(int room_id);
	void LoadRoomDataComplete(network::HttpClient* client, network::HttpResponse* response);
	int CreateNewRoomOnOneComputer(int boardType);
	int CreateNewRoomWithOpponent();
	void RemoveCurrentRoom();
	bool IsHaveCreatedRoom();
	void RebuildRoomButtonPos();
	bool IsHaveFacebookLogin();
	void CreateNewFacebookPlayer(std::string name, int id);
	void NewFacebookPlayerCreatedComplete(network::HttpClient* client, network::HttpResponse* response);
	void LoadPlayerRoomsComplete(network::HttpClient* client, network::HttpResponse* response);
	void LoadCostListComplete(network::HttpClient* client, network::HttpResponse* response);
	int CreateNewRoomBlank(int id, int turn, int uid1, int uid2, std::string opponent_name);
	int GetRoomIdByServerId(int room_server_id);
	void LoadPlayerRooms();
	void LoadCostList();
	bool IsRoomLoaded(int id){return m_Rooms[id]->IsLoaded();}
	void CheckWordsOnServer(const std::vector<std::string> &words);
	void CheckWordsOnServerComplete(network::HttpClient* client, network::HttpResponse* response);
	void SendMoveToServer(PLAYER_MOVE move);
	void SendMoveToServerComplete(network::HttpClient* client, network::HttpResponse* response);
	void CreateNewRoom();
	void CreateNewRoomComplete(network::HttpClient* client, network::HttpResponse* response);
	void CheckGamesUpdate();
	void CheckGamesUpdateComplete(network::HttpClient* client, network::HttpResponse* response);
	void UpdateRoomData(int room_id);
	void UpdateRoomDataComplete(network::HttpClient* client, network::HttpResponse* response);
	void SendPassMoveToServer(PLAYER_MOVE move, bool isSwap = false);
	void SendPassMoveToServerComplete(network::HttpClient* client, network::HttpResponse* response);
	bool CheckForUpdate(const rapidjson::Value &data, int turn);
    void getRoomChat();
    void getRoomChatComplete(network::HttpClient* client, network::HttpResponse* response);
    void sendChatMessage(std::string msg);
    void sendMessageComplete(network::HttpClient* client, network::HttpResponse* response);
    void leaveGame();
    void leaveGameComplete(network::HttpClient* client, network::HttpResponse* response);
    
    void createGameBetweenTwoPlayer(int my_id,int opp_id);
    void createGameBetweenTwoPlayerComplete(network::HttpClient* client, network::HttpResponse* response);
    
    void searchPlayers(std::string query);
    void searchPlayersComplete(network::HttpClient* client, network::HttpResponse* response);
    
    void getFinishedGames();
    void getFinishedGamesComplete(network::HttpClient* client, network::HttpResponse* response);
    
    std::vector<CRoom*> getRooms();
    
private:			
	int CheckHTTPError(network::HttpResponse* response);
	
	void InitDefaultParams();
	void SavePlayer();
	bool LoadPlayer();

	std::string BeQuoted(const std::string &value);
	std::string BeQuoted(int value);

	int m_CurrentRoomId;
	bool m_IsWordMetterOn;
	std::vector<CRoom*> m_Rooms;
	std::string m_Name;
	int m_FacebookId;
	int m_ServerId;
	std::map<char,int>m_LettersCost;
};
#endif //NBG_GAME_PLAYER