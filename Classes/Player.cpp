#include "Player.h"
#include "xml.h"
#include "GameScene.h"
#include <vector>
#include <algorithm>
#include "ChatLayer.h"
//#include "SearchPopUp.h"

std::string CPlayer::SERVER_URL = "http://hardikdubal.com/scrabble/api/"; //"http://192.168.1.5:1234/api/" //"http://newbridgegames.com/external/wwf/api/";

//////////////////////////////////////////////////////////////////////////
CPlayer::CPlayer()
{
	
}

//////////////////////////////////////////////////////////////////////////
CPlayer::~CPlayer()
{

}

//////////////////////////////////////////////////////////////////////////
bool CPlayer::init()
{	
	retain();
	m_IsWordMetterOn = true;
	m_CurrentRoomId = -1;
	if(LoadPlayer() == false)
	{
		InitDefaultParams();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
int CPlayer::CheckHTTPError(network::HttpResponse* response)
{
//    if (response->getResponseCode() == 200) {
//        return false;
//    }
//    else if (response->getResponseCode() == 403) {
//        return true;
//    }
//	else {
//		//auto data = response->getResponseData();
//		//std::string str(data->begin(),data->end());		
//		//MessageBox(str.c_str(), "Error!");
//		return true;
//	}

    
	return response->getResponseCode();
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::CreateNewFacebookPlayer(std::string name, int id)
{
	m_Name = name;
	m_FacebookId = id;
	auto client = network::HttpClient::getInstance();
	auto request = new cocos2d::network::HttpRequest();
	std::string url = CPlayer::SERVER_URL + "create_user?name=" + name + "&facebook_id=" + StringUtils::toString(id);
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, httpresponse_selector(CPlayer::NewFacebookPlayerCreatedComplete));
	client->send(request);
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::NewFacebookPlayerCreatedComplete(network::HttpClient* client, network::HttpResponse* response)
{
	if (CheckHTTPError(response) != 200)
	{
		MessageBox("No answer from server, please try again later", "Sorry!")	;
		return;
	}

	auto data = response->getResponseData();
	std::string str(data->begin(),data->end());

	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	const rapidjson::Value& userId= doc["user_id"];
	m_ServerId = atoi(userId.GetString());

	SavePlayer();
	LoadCostList();
}

void CPlayer::createGameBetweenTwoPlayer(int my_id, int opp_id) {
    auto client = network::HttpClient::getInstance();
    auto request = new cocos2d::network::HttpRequest();
    std::string url = CPlayer::SERVER_URL + "startGameUsers?uid1=" + StringUtils::toString(my_id) + "&uid2=" + StringUtils::toString(opp_id);
    
    request->setUrl(url.c_str());
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(CPlayer::createGameBetweenTwoPlayerComplete));
    client->send(request);
}

void CPlayer::createGameBetweenTwoPlayerComplete(network::HttpClient *client, network::HttpResponse *response) {
    if (CheckHTTPError(response) != 200) {
        return;
    }
    
    CreateNewRoomComplete(client, response);
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::CreateNewRoom()
{
	auto client = network::HttpClient::getInstance();
	auto request = new cocos2d::network::HttpRequest();
	std::string url = CPlayer::SERVER_URL + "start_game?uid=" + StringUtils::toString(m_ServerId);
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, httpresponse_selector(CPlayer::CreateNewRoomComplete));
	client->send(request);
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::CreateNewRoomComplete(network::HttpClient* client, network::HttpResponse* response)
{

    if (CheckHTTPError(response) == 200) {
        auto data = response->getResponseData();
        std::string str(data->begin(),data->end());
        
        CCLOG("Start new game :: %s",str.c_str());
        
        rapidjson::Document doc;
        doc.Parse<0>(str.c_str());
        
        const rapidjson::Value& game = doc;
        if (game.IsArray() || game.IsObject())
        {
            int room_server_id = atoi(game["id"].GetString());
            int turn;
            
            turn = game["turn"].GetInt();
            
            int uid1 = m_ServerId;
            int uid2 = atoi(game["uid2"].GetString());
            std::string opponent_name = game["opponent_name"].GetString();
            
            int board_type = game["board_type"].GetInt();
            int room_id = CreateNewRoomOnOneComputer(board_type);
            CRoom* room = m_Rooms[room_id];
            room->SetBlandData(room_server_id, turn, uid1, uid2, opponent_name);
            room->GetRoomButton()->SetLastWord("");
            room->GetRoomButton()->SetLastAction("No move yet!");
            if(turn == 1)
            {
                CGameScene::Instance()->PutToTable(CRoomsTable::TT_YOUR_MOVE,m_Rooms[room_id]->GetRoomButton());
            }
            else
            {
                CGameScene::Instance()->PutToTable(CRoomsTable::TT_THEIR_MOVE,m_Rooms[room_id]->GetRoomButton());
            }
            
            CGameScene::Instance()->RebuildTablesPos();
            
            const rapidjson::Value& stack_data = game["stack"]["characters"];
            CStack* stack = room->GetStack();
            
            for (rapidjson::SizeType i = 0; i < stack_data.Size(); i++)
            {
                std::string letter = stack_data[i].GetString();
                char letterChar = letter.c_str()[0];
                stack->PushLetterToStack(letterChar,m_LettersCost[letterChar]);
            }
            
            CDeck* deck1 = room->GetFirstPlayerDeck();
            //CDeck* deck2 = room->GetSecondPlayerDeck();
            
            std::string deckId = "raw_deck2";
            if(turn == 1)
            {
                deckId = "raw_deck1";	
            }
            
            const rapidjson::Value& deck1_data = game[deckId.c_str()];
            for (rapidjson::SizeType i = 0; i < deck1_data.Size(); i++)
            {
                std::string letter = deck1_data[i].GetString();
                char letterChar = letter.c_str()[0];
                if(letterChar == '_')
                {
                    break;
                }
                deck1->PutLetterToEmptyCell(letterChar,m_LettersCost[letterChar]);
            }
            
            room->SetLoaded(true);
            CGameScene::Instance()->HideWaitingAnim();
            CheckGamesUpdate();
//            CGameScene::Instance()->StartGameUpdateTime();
            CGameScene::Instance()->ChangeRoom(room_id);
        }
    }
    else if(CheckHTTPError(response) == 202)
    {
        MessageBox("Not Able To FInd USER!", "Sorry :(");
        CGameScene::Instance()->HideWaitingAnim();
        return;
    }
    else if (CheckHTTPError(response) == 201) {
        CheckGamesUpdate();
        CGameScene::Instance()->HideWaitingAnim();

    }
    else {
        CGameScene::Instance()->HideWaitingAnim();
        CheckGamesUpdate();
//        CGameScene::Instance()->StartGameUpdateTime();
        return;
    }
		
}

void CPlayer::leaveGame() {
    auto client = network::HttpClient::getInstance();
    auto request = new cocos2d::network::HttpRequest();
    std::string url = CPlayer::SERVER_URL + "gameLeave";
    request->setUrl(url.c_str());
    
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(CPlayer::leaveGameComplete));
    
    std::string postData = "game_id=" +StringUtils::toString(this->GetCurrentRoom()->GetServerId())+ "&user_id=" + StringUtils::toString(m_ServerId);
    
    CCLOG("send chat request :: %s",postData.c_str());
    
    request->setRequestData(postData.c_str(), strlen(postData.c_str()));
    
    client->send(request);
}

void CPlayer::leaveGameComplete(network::HttpClient *client, network::HttpResponse *response) {
    
    if (CheckHTTPError(response) != 200)
    {
        return;
    }

    CGameScene::Instance()->EndCurrentRoom();

    CheckGamesUpdate();

}

//////////////////////////////////////////////////////////////////////////
void CPlayer::LoadPlayerRooms()
{
	auto client = network::HttpClient::getInstance();
	auto request = new cocos2d::network::HttpRequest();
    
    CCLOG("ServerId :: %d",m_ServerId);
    
	std::string url = CPlayer::SERVER_URL + "get_user_games?uid=" + StringUtils::toString(m_ServerId);
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, httpresponse_selector(CPlayer::LoadPlayerRoomsComplete));
	client->send(request);
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::LoadPlayerRoomsComplete(network::HttpClient* client, network::HttpResponse* response)
{
	if (CheckHTTPError(response) != 200)
	{
        CheckGamesUpdate();
//		CGameScene::Instance()->StartGameUpdateTime();
		return;
	}

	CGameScene* scene = CGameScene::Instance();

	auto data = response->getResponseData();
	std::string str(data->begin(),data->end());

	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	const rapidjson::Value& my_turn_rooms = doc["my"];	
	if (my_turn_rooms.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < my_turn_rooms.Size(); i++)
		{
			const rapidjson::Value& room_data = my_turn_rooms[i];
			int server_id = atoi(room_data["id"].GetString());
			int turn = 1;
			int uid1 = atoi(room_data["uid1"].GetString());
			int uid2 = atoi(room_data["uid2"].GetString());
            int board_type = atoi(room_data["board_type"].GetString());
			std::string opponent_name = room_data["opp_name"].GetString();
			std::string last_action = room_data["last_action"].GetString();
			std::string last_word = room_data["last_word"].GetString();
			int room_id = CreateNewRoomOnOneComputer(board_type);
			CRoom* room = m_Rooms[room_id];
			CRoomButton* roomButton = room->GetRoomButton();

			room->SetBlandData(server_id, turn, uid1, uid2, opponent_name);
			roomButton->SetLastAction(last_action);
			roomButton->SetLastWord(last_word);
			scene->PutToTable(CRoomsTable::TT_YOUR_MOVE,roomButton);
		}
	}	
	const rapidjson::Value& they_turn_rooms = doc["opponent"];
	if (they_turn_rooms.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < they_turn_rooms.Size(); i++)
		{
			const rapidjson::Value& room_data = they_turn_rooms[i];
			int server_id = atoi(room_data["id"].GetString());
			int turn = 0;
			int uid1 = atoi(room_data["uid1"].GetString());
			int uid2 = atoi(room_data["uid2"].GetString());
            int board_type = atoi(room_data["board_type"].GetString());
			std::string opponent_name = room_data["opp_name"].GetString();
			std::string last_action = room_data["last_action"].GetString();
			std::string last_word = room_data["last_word"].GetString();
			int room_id = CreateNewRoomOnOneComputer(board_type);
			CRoom* room = m_Rooms[room_id];
			CRoomButton* roomButton = room->GetRoomButton();

			room->SetBlandData(server_id, turn, uid1, uid2, opponent_name);
			roomButton->SetLastAction(last_action);
			roomButton->SetLastWord(last_word);
			scene->PutToTable(CRoomsTable::TT_THEIR_MOVE,m_Rooms[room_id]->GetRoomButton());
		}
	}	
	scene->RebuildTablesPos();
    
    if (m_Rooms.size() > 0) {
        CRoom* room = m_Rooms[0];
        CRoomButton* roomButton = room->GetRoomButton();
        roomButton->ButtonClick(0);
    }
    
    
	scene->HideWaitingAnim();
    //CheckGamesUpdate();
//	scene->StartGameUpdateTime();
}

//////////////////////////////////////////////////////////////////////////
bool CPlayer::LoadPlayer()
{
	//m_Name = CCUserDefault::getInstance()->getStringForKey("name", "Player");
	pugi::xml_document doc;
	std::string path = "test.xml";
	loadXmlDocumentFromPath(doc,path);
	pugi::xml_node rowNode = doc.first_child();
    m_ServerId = UserDefault::getInstance()->getIntegerForKey("Player_ID", 1);
    //atoi(rowNode.attribute("id").value());
    m_Name = UserDefault::getInstance()->getStringForKey("Name");
	//m_ServerId = CCUserDefault::getInstance()->getIntegerForKey("server_id", -1);
	//m_FacebookId = CCUserDefault::getInstance()->getIntegerForKey("facebook_id", -1);
	m_FacebookId = 213;
//	if(m_FacebookId != -1)
//	{
    return true;
//	}
//	return false;
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::SavePlayer()
{
//	CCUserDefault::getInstance()->setStringForKey("name", m_Name);
//	CCUserDefault::getInstance()->setIntegerForKey("server_id", m_ServerId);
//	CCUserDefault::getInstance()->setIntegerForKey("facebook_id", m_FacebookId);
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::InitDefaultParams()
{
	m_FacebookId = -1;
	m_ServerId = -1;
	m_Name = "Player";
}

//////////////////////////////////////////////////////////////////////////
bool CPlayer::IsHaveFacebookLogin()
{
	if(m_FacebookId != -1)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
CRoom* CPlayer::GetCurrentRoom()
{
	return m_Rooms[m_CurrentRoomId];
}

//////////////////////////////////////////////////////////////////////////
int CPlayer::CreateNewRoomOnOneComputer(int boardType)
{
	if(m_Rooms.size() >= MAX_ROOM_COUNT)
	{
		return -1;
	}

	CRoom * room = CRoom::create(boardType);
	room->retain();

	m_Rooms.push_back(room);

	int room_id = m_Rooms.size()-1;
	room->SetId(room_id);
	return room_id;
}

//////////////////////////////////////////////////////////////////////////
int CPlayer::CreateNewRoomWithOpponent()
{
	if(m_Rooms.size() >= MAX_ROOM_COUNT)
	{
		return -1;
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::RemoveCurrentRoom()
{
	if(m_CurrentRoomId == -1)
	{
		return;
	}
	CRoom* room = GetCurrentRoom();
	if(room->GetOldState() == CRoom::RS_END_GAME)
	{
		CGameScene::Instance()->DeleteFromTable(CRoomsTable::TT_GAME_OVER,room->GetRoomButton());
	}
	else if(room->GetTurn() == CRoom::RT_FIRST_PLAYER)
	{
		CGameScene::Instance()->DeleteFromTable(CRoomsTable::TT_YOUR_MOVE,room->GetRoomButton());
	}
	else if(room->GetTurn() == CRoom::RT_SECOND_PLAYER)
	{
		CGameScene::Instance()->DeleteFromTable(CRoomsTable::TT_THEIR_MOVE,room->GetRoomButton());
	}
	
	m_Rooms[m_CurrentRoomId]->Remove();
	m_Rooms.erase(m_Rooms.begin()+m_CurrentRoomId);
	if(!m_Rooms.empty())
	{
		for(int i=0;i < m_Rooms.size();i++)
		{
			room = m_Rooms[i];
			room->SetId(i);
		}
		m_CurrentRoomId = CGameScene::Instance()->GetFirstRoomFromTable();
	}
	else
	{
		m_CurrentRoomId = -1;
	}
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::LoadRoomData(int room_id)
{
	auto client = network::HttpClient::getInstance();
	auto request = new cocos2d::network::HttpRequest();
	std::string url = CPlayer::SERVER_URL + "get_game?id=" + StringUtils::toString(room_id);
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, httpresponse_selector(CPlayer::LoadRoomDataComplete));
	client->send(request);
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::LoadCostList()
{
	auto client = network::HttpClient::getInstance();
	auto request = new cocos2d::network::HttpRequest();
	std::string url = CPlayer::SERVER_URL + "get_letter_costs";
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, httpresponse_selector(CPlayer::LoadCostListComplete));
	client->send(request);
}

//////////////////////////////////////////////////////////////////////////
int CPlayer::GetRoomIdByServerId(int room_server_id)
{
	for(int i=0; i < m_Rooms.size();i++)
	{
		if(m_Rooms[i]->GetServerId() == room_server_id)
		{
			return i;
		}
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPlayer::LoadCostListComplete(network::HttpClient* client, network::HttpResponse* response)
{
	if (CheckHTTPError(response) != 200)
	{
		LoadCostList();
		return;
	}

	auto data = response->getResponseData();
	std::string str(data->begin(),data->end());

	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	for (rapidjson::Value::ConstMemberIterator itr = doc.MemberonBegin(); itr != doc.MemberonEnd(); ++itr)
	{
		char letterKey = itr->name.GetString()[0];
		int letterCost = itr->value.GetInt();
		m_LettersCost[letterKey] = letterCost;
	}
	LoadPlayerRooms();
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::LoadRoomDataComplete(network::HttpClient* client, network::HttpResponse* response)
{
    if (CheckHTTPError(response) != 200) {
        return;
    }

	auto data = response->getResponseData();
	std::string str(data->begin(),data->end());

	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	const rapidjson::Value& game = doc["game"];	
	if (game.IsArray() || game.IsObject())
	{
		int room_server_id = atoi(game["id"].GetString());
		int p1_score = atoi(game["p1_score"].GetString());
		int p2_score = atoi(game["p2_score"].GetString());
		int room_id = GetRoomIdByServerId(room_server_id);
		int uid1 = atoi(game["uid1"].GetString());
		int uid2 = atoi(game["uid2"].GetString());

		const rapidjson::Value& stack_data = game["stack"];	
		CRoom* room = m_Rooms[room_id];
		CStack* stack = room->GetStack();

		for (rapidjson::SizeType i = 0; i < stack_data.Size(); i++)
		{
			std::string letter = stack_data[i].GetString();
			char letterChar = letter.c_str()[0];
			stack->PushLetterToStack(letterChar,m_LettersCost[letterChar]);
		}
		
		const rapidjson::Value& used_letters = game["used_letters"];
		std::vector<int> usedLettersIndex;
		for (rapidjson::SizeType i = 0; i < used_letters.Size(); i++)
		{
			int index;
			int iter = 1;
			for (rapidjson::Value::ConstMemberIterator itr = used_letters[i].MemberonBegin(); itr != used_letters[i].MemberonEnd(); ++itr)
			{
				if(iter != 1)
				{
					index = atoi(itr->value.GetString());
					usedLettersIndex.push_back(index);
				}
				iter++;
			}		
		}

		const rapidjson::Value& grid_data = game["grid"];	
		CField* field = room->GetField();
		if(!grid_data.Empty())
		{
			field->SetPlasedFirstWord(true);
		}
		for (rapidjson::SizeType i = 0; i < grid_data.Size(); i++)
		{
			char letter;
			int index;
			int iter = 1;
			for (rapidjson::Value::ConstMemberIterator itr = grid_data[i].MemberonBegin(); itr != grid_data[i].MemberonEnd(); ++itr)
			{
				if(iter == 1)
				{
					letter = (char)atoi(itr->value.GetString());
				}
				else
				{
					index = atoi(itr->value.GetString());
				}
				iter++;
			}
			int cost = m_LettersCost[letter];
			bool isNew = false;
			if(usedLettersIndex.empty() == false)
			{
				for(int i=0;i<usedLettersIndex.size();i++)
				{
					if(usedLettersIndex[i] == index)
					{
						isNew = true;
						break;
					}
				}
			}
			field->AddLetterForIndex(letter, index, cost, isNew);
			
		}

		CDeck* deck1 = room->GetFirstPlayerDeck();
		//CDeck* deck2 = room->GetSecondPlayerDeck();
			
		std::string deckId = "deck2";
		if(m_ServerId == uid1)
		{
			deckId = "deck1";	
			room->SetFirstPlayerScore(p1_score);
			room->SetSecondPlayerScore(p2_score);
		}
		else
		{
			room->SetFirstPlayerScore(p2_score);
			room->SetSecondPlayerScore(p1_score);
		}

		const rapidjson::Value& deck1_data = game[deckId.c_str()];
		for (rapidjson::SizeType i = 0; i < deck1_data.Size(); i++)
		{
			std::string letter = deck1_data[i].GetString();
			char letterChar = letter.c_str()[0];
			if(letterChar == '_')
			{
				break;
			}
			deck1->PutLetterToEmptyCell(letterChar,m_LettersCost[letterChar]);
		}

		room->SetLoaded(true);
		CGameScene::Instance()->HideWaitingAnim();
		CGameScene::Instance()->ChangeRoom(room_id);
	}	
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::SendPassMoveToServer(PLAYER_MOVE move, bool isSwap)
{
	auto client = network::HttpClient::getInstance();
	auto request = new cocos2d::network::HttpRequest();
	std::string url = CPlayer::SERVER_URL + "skip_move";
	url += "?uid=" + StringUtils::toString(m_ServerId);
	url += "&id=" +  StringUtils::toString(move.room_server_id);
	if(isSwap)
	{
		url += "&deck=" + move.deck;
		url += "&stack=" + move.stack;
	}
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, httpresponse_selector(CPlayer::SendPassMoveToServerComplete));
	client->send(request);
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::SendPassMoveToServerComplete(network::HttpClient* client, network::HttpResponse* response)
{
        
    if (CheckHTTPError(response) != 200) {
        
        MessageBox("Pass or swap failed", "error");
        return;
    }
}

void CPlayer::searchPlayers(std::string query) {
    auto client = network::HttpClient::getInstance();
    auto request = new cocos2d::network::HttpRequest();
    std::string url = CPlayer::SERVER_URL + "userSearch";
    url += "?query=" + query + "&user_id=" + StringUtils::toString(m_ServerId);
    
    request->setUrl(url.c_str());
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(CPlayer::searchPlayersComplete));
    client->send(request);
}

void CPlayer::searchPlayersComplete(network::HttpClient *client, network::HttpResponse *response) {
    auto data = response->getResponseData();
    std::string str(data->begin(),data->end());
    
    log("Search result :: %s",str.c_str());
    
    std::vector<std::string> searchName;
    std::vector<std::string> searchuserId;
    std::vector<std::string> searchImageUrl;
    std::vector<std::string> searchEmail;
    
    rapidjson::Document doc;
    doc.Parse<0>(str.c_str());

    const rapidjson::Value& game = doc;
    
    if (game.IsArray()) {
        CCLOG("game is an array");
        
        for (rapidjson::SizeType i = 0; i < game.Size(); i++)
        {
            const rapidjson::Value& searchInfo = game[i];
            
            std::string name = searchInfo["User"]["name"].GetString();
            std::string email = searchInfo["User"]["email"].GetString();
            std::string imageUrl = searchInfo["User"]["image_url"].GetString();
            std::string userId = searchInfo["User"]["id"].GetString();
            
            searchName.push_back(name);
            searchEmail.push_back(email);

            searchImageUrl.push_back(imageUrl);
            searchuserId.push_back(userId);

        }
        
     /*   if (UserDefault::getInstance()->getIntegerForKey("SearchPopUp_On") == 1) {
            SearchPopUp::getInstance()->searchResult(searchName, searchEmail, searchImageUrl, searchuserId);
        }*/
        
    }
    

}

//////////////////////////////////////////////////////////////////////////
void CPlayer::SendMoveToServer(PLAYER_MOVE move)
{
	auto client = network::HttpClient::getInstance();
	auto request = new cocos2d::network::HttpRequest();
	std::string url = CPlayer::SERVER_URL + "send_move?";
	url += "id=" + StringUtils::toString(move.room_server_id) +'&';			//id room
	url += "uid=" + StringUtils::toString(m_ServerId) + '&';				// id player
	url += "used_letters={" + BeQuoted("letters") + ":[";					// used letters
	for(std::map<int,char>::iterator it = move.used_letters.begin();it != move.used_letters.end();++it)
	{
		
			url += '{' + BeQuoted("id") + ':' + BeQuoted(it->first) + ',';
			url += BeQuoted("letter") + ':' + BeQuoted(it->second) + '}';
			if(!(it == --move.used_letters.end()))
			{
				url += ',';
			}
	}
	url += "]}&";
	url += "score=" + StringUtils::toString(move.score) + '&';				// score
	url += "stack=" + move.stack + '&';										// stack
	url += "deck=" + move.deck + '&';										// deck
	url += "word=" + move.word;												// first word

    CCLOG("Player Send Requesr :: %s",url.c_str());
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, httpresponse_selector(CPlayer::SendMoveToServerComplete));
	client->send(request);
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::SendMoveToServerComplete(network::HttpClient* client, network::HttpResponse* response)
{
	if(response->getResponseCode() == 200)
	{
		CGameScene::Instance()->SendingMoveCompleteSuccess();
	}
	else
	{
		CGameScene::Instance()->SendingMoveCompleteFail();
	}
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::CheckWordsOnServer(const std::vector<std::string> &words)
{
	auto client = network::HttpClient::getInstance();
	auto request = new cocos2d::network::HttpRequest();
	std::string url = CPlayer::SERVER_URL + "check_words";
	for(int i=0; i<words.size();i++)
	{
		std::string prefix;
		if(i == 0)
		{
			prefix = "?w";
		}
		else
		{
			prefix = "&w";
		}
		std::string word = prefix+StringUtils::toString(i)+"=";
		word += words[i];
		url+=word;
	}
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, httpresponse_selector(CPlayer::CheckWordsOnServerComplete));
	client->send(request);
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::CheckWordsOnServerComplete(network::HttpClient* client, network::HttpResponse* response)
{
	if(response->getResponseCode() == 200)
	{
		CGameScene::Instance()->SuccessServerCheck();
	}
	else
	{
		CGameScene::Instance()->FailServerCheck();
	}
}

//////////////////////////////////////////////////////////////////////////
bool CPlayer::IsHaveCreatedRoom()
{
	if(m_Rooms.empty())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::RebuildRoomButtonPos()
{
	int turn = 0;
	for(int i=0;i<m_Rooms.size();i++)
	{
		CRoom *room = m_Rooms[i];
		if(room->GetId() != i)
		{
			room->ChangeId(i,turn);
			turn++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::SwapWordMetterState()
{
	m_IsWordMetterOn = !m_IsWordMetterOn;
	GetCurrentRoom()->SetScorePointerVisible(m_IsWordMetterOn);
	
}

//////////////////////////////////////////////////////////////////////////
std::string CPlayer::BeQuoted(const std::string &value)
{
	return "%22" + value + "%22";
}

//////////////////////////////////////////////////////////////////////////
std::string CPlayer::BeQuoted(int value)
{
	return BeQuoted(StringUtils::toString(value));
}

void CPlayer::getFinishedGames() {
    auto client = network::HttpClient::getInstance();
    auto request = new cocos2d::network::HttpRequest();
    std::string url = CPlayer::SERVER_URL + "getFinishedGames?user_id=" + StringUtils::toString(m_ServerId);
    request->setUrl(url.c_str());
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(CPlayer::getFinishedGamesComplete));
    client->send(request);
}

void CPlayer::getFinishedGamesComplete(network::HttpClient *client, network::HttpResponse *response) {
    
    if (CheckHTTPError(response) != 200)
    {
        MessageBox("Can not Fetch Finished Games From Server", "Error");
        return;
    }
    
    CGameScene* scene = CGameScene::Instance();
    
    auto data = response->getResponseData();
    std::string str(data->begin(),data->end());
    
    rapidjson::Document doc;
    doc.Parse<0>(str.c_str());
    
    if (doc.IsArray())
    {
        for (rapidjson::SizeType i = 0; i < doc.Size(); i++)
        {
            const rapidjson::Value& finishedGames = doc["Game"];
            const rapidjson::Value& room_data = finishedGames[i];
            int server_id = atoi(room_data["id"].GetString());
            int turn = 1;
            int uid1 = atoi(room_data["uid1"].GetString());
            int uid2 = atoi(room_data["uid2"].GetString());
            int p1Score = atoi(room_data["p1_score"].GetString());
            int p2Score = atoi(room_data["p2_score"].GetString());
            
            const rapidjson::Value& finishedGamesuser = doc["User"];

            if (finishedGamesuser.IsArray()) {
                for (rapidjson::SizeType i = 0; i < finishedGamesuser.Size(); i++)
                {
                    if (i==0) { //opponent data
                        std::string opponent_name = room_data["name"].GetString();
                    }
                    else if (i==1) {//my data
                        std::string my_name = room_data["name"].GetString();
                    }
                }
            }

//            int room_id = CreateNewRoomOnOneComputer();
//            CRoom* room = m_Rooms[room_id];
//            CRoomButton* roomButton = room->GetRoomButton();
//            
//            room->SetBlandData(server_id, turn, uid1, uid2, opponent_name);
//            roomButton->SetLastAction(last_action);
//            roomButton->SetLastWord(last_word);
//            scene->PutToTable(CRoomsTable::TT_GAME_OVER,roomButton);
        }
    }	

}

//////////////////////////////////////////////////////////////////////////
void CPlayer::CheckGamesUpdate()
{
	auto client = network::HttpClient::getInstance();
	auto request = new cocos2d::network::HttpRequest();
	std::string url = CPlayer::SERVER_URL + "get_user_games?uid=" + StringUtils::toString(m_ServerId);
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, httpresponse_selector(CPlayer::CheckGamesUpdateComplete));
	client->send(request);
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::CheckGamesUpdateComplete(network::HttpClient* client, network::HttpResponse* response)
{
	if (CheckHTTPError(response) != 200)
	{
        CheckGamesUpdate();
//		CGameScene::Instance()->StartGameUpdateTime();
		return;
	}

	CGameScene* scene = CGameScene::Instance();

	auto data = response->getResponseData();
	std::string str(data->begin(),data->end());

	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	bool have_update = false;
	const rapidjson::Value& my_turn_rooms = doc["my"];	
	if(CheckForUpdate(my_turn_rooms, 1)) have_update = true;
	const rapidjson::Value& opp_turn_rooms = doc["opponent"];	
	if(CheckForUpdate(opp_turn_rooms, 0)) have_update = true;
	if(!have_update)
	{
        //#check //dont know why this is here
//        CheckGamesUpdate();
		//scene->StartGameUpdateTime();
	}
}

//////////////////////////////////////////////////////////////////////////
bool CPlayer::CheckForUpdate(const rapidjson::Value &data, int turn)
{
	if (data.IsArray())
	{
		bool have_update = false;
		for (rapidjson::SizeType i = 0; i < data.Size(); i++)
		{
			const rapidjson::Value& room_data = data[i];
			int server_id = atoi(room_data["id"].GetString());
			CRoom::ROOM_TURN roomTurn;
			if(turn == 1)
			{
				roomTurn = CRoom::RT_FIRST_PLAYER;
			}
			else
			{
				roomTurn = CRoom::RT_OPPONENT;
			}
			int room_id = GetRoomIdByServerId(server_id);
			CRoom* room;
			if(room_id == -1)
			{
                int board_type = atoi(room_data["board_type"].GetString());
				room_id = CreateNewRoomOnOneComputer(board_type);
				room = m_Rooms[room_id];
				int uid1 = atoi(room_data["uid1"].GetString());
				int uid2 = atoi(room_data["uid2"].GetString());
				std::string opponent_name = room_data["opp_name"].GetString();
				int wrongTurn;
				if(turn == 0)
				{
					wrongTurn = 1;
				}
				else
				{
					wrongTurn = 0;
				}
				room->SetBlandData(server_id, wrongTurn, uid1, uid2, opponent_name);
				//continue;
			}
			CGameScene* scene = CGameScene::Instance();
			room = m_Rooms[room_id];
			std::string last_action = room_data["last_action"].GetString();
			room->GetRoomButton()->SetLastAction(last_action);
			if(room->GetTurn() != roomTurn)
			{
				std::string last_word = room_data["last_word"].GetString();
				room->GetRoomButton()->SetLastWord(last_word);
				room->SetLoaded(false);
				if(turn == 1)
				{
					scene->DeleteFromTable(CRoomsTable::TT_THEIR_MOVE,room->GetRoomButton());
					scene->PutToTable(CRoomsTable::TT_YOUR_MOVE,room->GetRoomButton());
					room->SetTurn(CRoom::RT_FIRST_PLAYER);
				}
				else
				{
					scene->DeleteFromTable(CRoomsTable::TT_YOUR_MOVE,room->GetRoomButton());
					scene->PutToTable(CRoomsTable::TT_THEIR_MOVE,room->GetRoomButton());
					room->SetTurn(CRoom::RT_OPPONENT);
				}
				scene->RebuildTablesPos();
				scene->RebuildRoomButtonsPos();
				scene->RebuildRoomLabels();
				if(m_CurrentRoomId == room_id)
				{
					scene->ShowWaitingAnim();
					if(scene->IsHaveActiveLetter())
					{
						scene->GetActiveLetter()->BreakTouch();
						scene->GetActiveLetter()->GoToLastPos();
						scene->ClearActiveLetter();
					}
					UpdateRoomData(room->GetServerId());
                    CGameScene::Instance()->UnblockControlButtons(0);
					return true;
				}
				else
				{
					room->GetRoomButton()->MakeBlinkingAnim();
				}
			}
		}
	}
	return false;
}

void CPlayer::getRoomChatComplete(network::HttpClient* client, network::HttpResponse* response) {
    if (CheckHTTPError(response) != 200) {
        return;
    }
    
    auto data = response->getResponseData();
    std::string str(data->begin(),data->end());
    
    rapidjson::Document doc;
    doc.Parse<0>(str.c_str());
    
    if (doc.IsArray()) {
        
        std::vector<std::string> textVector;
        std::vector<std::string> senderNameVector;
        std::vector<int> senderId;
        
        for (rapidjson::SizeType i = 0; i < doc.Size(); i++)
        {
            const rapidjson::Value& messageInfo = doc[i];
            
            const rapidjson::Value& message = messageInfo["Message"];
            
            std::string text = message["message"].GetString();
            
            std::string sender = messageInfo["User"]["name"].GetString();
            
            int userid = atoi(message["user_id"].GetString());
            
            CCLOG("userid : %d message :: %s  ==> %s",userid,sender.c_str(),text.c_str());
            
            senderId.push_back(userid);
            textVector.push_back(text);
            senderNameVector.push_back(sender);
            
        }
        
        ChatLayer::getInstance()->setChatVector(textVector, senderNameVector,senderId);
    }
    
}

void CPlayer::getRoomChat() {
    auto client = network::HttpClient::getInstance();
    auto request = new cocos2d::network::HttpRequest();
    std::string url = CPlayer::SERVER_URL + "getMessages?game_id="+StringUtils::toString(this->GetCurrentRoom()->GetServerId()) + "&user_id=" + StringUtils::toString(m_ServerId);
    request->setUrl(url.c_str());
    
    request->setRequestType(network::HttpRequest::Type::GET);
    request->setResponseCallback(this, httpresponse_selector(CPlayer::getRoomChatComplete));
    
    client->send(request);
}

void CPlayer::sendChatMessage(std::string msg) {
    
    lastSentMessage = msg;
    
    auto client = network::HttpClient::getInstance();
    auto request = new cocos2d::network::HttpRequest();
    std::string url = CPlayer::SERVER_URL + "sendMessage";
    request->setUrl(url.c_str());
    
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(CPlayer::sendMessageComplete));
    
    std::string postData = "message=" + msg + "&game_id=" +StringUtils::toString(this->GetCurrentRoom()->GetServerId())+ "&user_id=" + StringUtils::toString(m_ServerId);
    
    CCLOG("send chat request :: %s",postData.c_str());
    
    request->setRequestData(postData.c_str(), strlen(postData.c_str()));
    
    client->send(request);
}

void CPlayer::sendMessageComplete(network::HttpClient *client, network::HttpResponse *response) {
    
    CCLOG("message successfully sent to server :: %ld",response->getResponseCode());

    
//    if (CheckHTTPError(response) != 200) {
//        return;
//    }
    
    ChatLayer::getInstance()->addMessage(lastSentMessage);
    
    CCLOG("message successfully sent to server");
}

//////////////////////////////////////////////////////////////////////////
void CPlayer::UpdateRoomData(int room_id)
{
	auto client = network::HttpClient::getInstance();
	auto request = new cocos2d::network::HttpRequest();
	std::string url = CPlayer::SERVER_URL + "get_game?id=" + StringUtils::toString(room_id);
	request->setUrl(url.c_str());
	request->setRequestType(network::HttpRequest::Type::GET); 
	request->setResponseCallback(this, httpresponse_selector(CPlayer::UpdateRoomDataComplete));
	client->send(request);
}



//////////////////////////////////////////////////////////////////////////
void CPlayer::UpdateRoomDataComplete(network::HttpClient* client, network::HttpResponse* response)
{
	if (CheckHTTPError(response) != 200)
	{
		CGameScene::Instance()->HideWaitingAnim();
//		CGameScene::Instance()->StartGameUpdateTime();
        CheckGamesUpdate();
		return;
	}

	auto data = response->getResponseData();
	std::string str(data->begin(),data->end());

	rapidjson::Document doc;
	doc.Parse<0>(str.c_str());

	CGameScene* scene = CGameScene::Instance();

	const rapidjson::Value& game = doc["game"];	
	if (game.IsArray() || game.IsObject())
	{
		int room_server_id = atoi(game["id"].GetString());
        int turn = game["turn"].GetBool();
		int p1_score = atoi(game["p1_score"].GetString());
		int p2_score = atoi(game["p2_score"].GetString());
		int room_id = GetRoomIdByServerId(room_server_id);
		int uid1 = atoi(game["uid1"].GetString());
		int uid2 = atoi(game["uid2"].GetString());

		const rapidjson::Value& stack_data = game["stack"];	
		CRoom* room = m_Rooms[room_id];
		CStack* stack = room->GetStack();
		stack->Clear();
		for (rapidjson::SizeType i = 0; i < stack_data.Size(); i++)
		{
			std::string letter = stack_data[i].GetString();
			char letterChar = letter.c_str()[0];
			stack->PushLetterToStack(letterChar,m_LettersCost[letterChar]);
		}

		CField* field = room->GetField();
		field->ClearNewLetters();
		scene->MakeRecall();

		const rapidjson::Value& used_letters = game["used_letters"];
		for (rapidjson::SizeType i = 0; i < used_letters.Size(); i++)
		{
			char letter;
			int index;
			int iter = 1;
			for (rapidjson::Value::ConstMemberIterator itr = used_letters[i].MemberonBegin(); itr != used_letters[i].MemberonEnd(); ++itr)
			{
				if(iter == 1)
				{
					letter = (char)atoi(itr->value.GetString());
				}
				else
				{
					index = atoi(itr->value.GetString());
				}
				iter++;
			}
			int cost = m_LettersCost[letter];
			field->AddLetterForIndex(letter, index, cost, true);
			if(field->IsFirstWordPlased() == false)
			{
				field->SetPlasedFirstWord(true);
			}
		}
			

		if(m_ServerId == uid1)
		{
			room->SetFirstPlayerScore(p1_score);
			room->SetSecondPlayerScore(p2_score);
		}
		else
		{
			room->SetFirstPlayerScore(p2_score);
			room->SetSecondPlayerScore(p1_score);
		}
		scene->RebuildTablesPos();
		scene->RebuildRoomButtonsPos();
		scene->RebuildRoomLabels();
	}
	scene->HideWaitingAnim();
//	scene->StartGameUpdateTime();
    CheckGamesUpdate();
}

std::vector<CRoom*> CPlayer::getRooms() {
    return m_Rooms;
}
