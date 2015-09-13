#ifndef NBG_GAME_SCENE
#define NBG_GAME_SCENE

#include "cocos2d.h"
#include <vector>

#include "Dictionary.h"


#include "Stack.h"
#include "Deck.h"
#include "SwapPanel.h"
#include "ChooseLetterWindow.h"
#include "ChangePlayerWindow.h"
#include "EndLevelWindow.h"
#include "WaitingWindow.h"


#include "WaitingWindow.h"

#include "BonusPanel.h"
#include "ScorePointer.h"
#include "RoomButton.h"
#include "RoomsTable.h"
#include "Room.h"
#include "Player.h"


#include "MenuItemWithDescription.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "json/rapidjson.h"
#include "json/reader.h"
#include "json/document.h"

#include "xml.h"


class CLetter;
class CField;

class CStack;
class CDeck;
class CSwapPanel;
class CChooseLetterWindow;
class CChangePlayerWindow;
class CEndLevelWindow;



USING_NS_CC;

#define IN_APP_5050			1
#define IN_APP_RANDOM		2
#define IN_APP_MINUS_CUP	3
#define IN_APP_RESTART		4

#define IN_APP_PLAY			5
#define IN_APP_SHUFFLE		6
#define IN_APP_PASS			7
#define IN_APP_RECALL		8
#define IN_APP_SWAP			9
#define IN_APP_MORE			10
#define IN_APP_REMATCH		11
#define IN_APP_LESS			10

#define IN_APP_5050_STRING			"com.sky7.thimbles.in_app_5050"
#define IN_APP_RANDOM_STRING		"com.sky7.thimbles.in_app_random"
#define IN_APP_MINUS_CUP_STRING		"com.sky7.thimbles.in_app_minus_cup"
#define IN_APP_RESTART_STRING		"com.sky7.thimbles.in_app_restart"

void my_itoa(int value, std::string& buf, int base);

struct PLAYER_MOVE
{
	int room_server_id;
	//std::string used_letters;
	std::map<int,char> used_letters;
	std::string stack;
	std::string deck;
	int score;
	int pass_counter;
	std::string word;
};

class ChatLayer;

class CGameScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

    bool calledOnce;
    
	static CGameScene * Instance()
	{
		return _self;
	}

    bool firstTapDone;
    float tapDelay;
    
    bool zoomedIn;
    bool zoomedOut;
    
    bool zoomEnable;
    
    Vec2 oldPoint;
    Vec2 newPoint;
    
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// a selector callback
	void menuCloseCallback(cocos2d::Object* pSender);

	// implement the "static create()" method manually
    static CGameScene* create();
    
	Color3B ACTIVE_PLAYER_COLOR;
	Color3B WAITING_PLAYER_COLOR;
	static float CHECKING_GAMES_UPDATE_TIME;

	static const int WORKSPACE_POS_X = 214;
	static const int WORKSPACE_POS_Y = 91;
	static const int WORKSPACE_SIZE_X = 787;
	static const int WORKSPACE_SIZE_Y = 612;

	static const int UNDER_GUI_LAYER = 9998;
	static const int GUI_LAYER = 9999;
	static const int OVER_GUI_LAYER = 10000;
	static const int DESCRIPTION_LAYER = 10001;
	static const int WAITING_WINDOW_LAYER = 10002;

	static const int MAX_ROOM_COUNT = 5;
	static const int ROOM_TABLE_START_POS_X = 100;
	static const int ROOM_TABLE_START_POS_Y = 675;
	static const int ROOM_BUTTONS_START_X = -410;
	static const int ROOM_BUTTONS_START_Y = 290;
	static const int ROOM_BUTTONS_HEIGHT = 60;

	static const int UPDATE_ROOM_ACTION_TAG = 99;
    
    bool letterDragged;
    
    bool chatLayerOpen;
    
    ChatLayer *chatlayer;
    
    MenuItemImage *openChatButton;
    MenuItemImage *closeChatButton;

    void addChatMessage(std::string msg);
    
    void showChatRelatedThing();
    void openChatLayer(Ref* sender);

	void ModifyLetterScores(Modifier modifier, int letterCost, int &scores);
	void ModifyWordScores(Modifier modifier, int &scores);

	void SetLetterUsed(CLetter * letter);///пометить букву, как использованную
	void SetLetterNotUsed(CLetter * letter);///пометить букву, как неиспользованную

	void SetVisiblePlayButton(bool visible);
	CSwapPanel * GetSwapPanel();
	CChooseLetterWindow * GetChooseLetterWindow() {return m_ChooseLetterWindow;}
	CEndLevelWindow* GetEndLevelWindow(){return m_EndLevelWindow;}
	CBonusPanel* GetBonusPanel(){return m_BonusPanel;}
	Sprite* GetOverlay();
	void SetActiveLetter(CLetter* letter){
        m_ActiveLetter = letter;
    }
	CLetter* GetActiveLetter(){return m_ActiveLetter;}
	void ClearActiveLetter(){m_ActiveLetter = NULL;}
	bool IsHaveActiveLetter();
	void NextPlayerTurn(float delay = 0.0f);
	void SetStackRemainLabel(int count);

	void BlockControlButtons(Node * sender);
	void UnblockControlButtons(Node * sender);
	void UnblockDeck(Node* sender);
    void BuyItem(const std::string randomItem);

	void CalculateScorePointer();
	void ChangeRoom(int id);
	bool IsChangingRooms(){return m_IsChangingRooms;}
	void RebuildRoomLabels();
	void RebuildRoomButtonsPos();
	void HideRoomLabels();

    bool TouchBegan(Touch* touch, Event* event);
    void TouchMoved(Touch* touch, Event* event);
    void TouchEnded(Touch* touch, Event* event);
    void TouchCancelled(Touch* touch, Event* event);
    
	//CRoom* GetCurrentRoom(){return m_Rooms[m_CurrentRoom];}
	CRoom* GetCurrentRoom();
	void MakeRecall();
	void RemoveRoom();
	void EndCurrentRoom();
	void RemoveCurrentRoom(Node* sender);
	void RematchCurrentRoom(Node* sender);
	void Rematch(Ref* sender);

	void PutToTable(CRoomsTable::TABLE_TYPE table, CRoomButton* button);
	void DeleteFromTable(CRoomsTable::TABLE_TYPE table, CRoomButton* button);
	void RebuildTablesPos();

	void ShowBonusPanel();
    void HideBonusPanel();
    
	CPlayer* GetPlayer(){return m_Player;}
	int GetFirstRoomFromTable();
	void FacebookLoginComplete();
	void ShowWaitingAnim();
	void HideWaitingAnim();
    
    void enableGameScene(bool state);
    
	void CheckWordsOnServer();
	void SuccessServerCheck();
	void FailServerCheck();
	void ShowWrongWords();
	void ShowSendMoveWindow();
	void SendMoveWindowOkCallback(Node* sender);
	void SendMoveWindowCancelCallback(Node* sender);
	void SendingMoveCompleteSuccess();
	void SendingMoveCompleteFail();
	void CheckGamesUpdate(Node* sender);
	//void StartGameUpdateTime();
	void StopGameUpdateTime();
	void ShowPassConfirm();
	void PassConfirmOkCallback(Node* sender);
	void SendSwapMove(float delay);
	void SetOpponentTurn();
    
    void searchPopupCallback();
private:	
	void CreateWaitingAnim();
	void CreateFacebookLoginBtn();
	void RemoveFacebookBtn();
	virtual void update(float delta);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	virtual void onMouseMove(Event* event);
#endif
    
    void SendMove();
    void SuccessCleanup(std::vector<std::string> words, const int score);
    void ScorePointCalculateComplete(const int score, const cocos2d::Point pos);

	//void ShowEndWindow();
	void OnWin(Node* sender);
	void CheckWin(Node* sender);
	void InitGUI();

	bool IsHaveLetterOnStar();
	bool IsUsedLettersInRow();
	bool IsUsedLettersInColumn();

	void AlignCurrentDeckLetters();
	void AddCurrentPlayerScore(int score);
	
    void NextTurn(Ref* sender);
	void Recall(Ref* sender);
	void Shuffle(Ref* sender);
	void More(Ref* sender);
	void Less(Ref* sender);
	void Swap(Ref* sender);
	void Pass(Ref* sender);
	void FacebookLogin(Ref* sender);

	void SwitchPlayers(Node* sender);
	void ShowChangePlayerWindow(Node * sender);
	void HideChangePlayerWindow(Node* sender);
	void HideCurrentDeck(Node * sender);
	void ShowCurrentDeck(Node * sender);

	void ShowCurrentRoom(Node* sender);
	void HideCurrentRoom(Node* sender);
	void ChangingRoomsComplete(Node* sender);
	void RematchingRoomComplete(Node* sender);

	void CreateNewRoomOnOneComputer(Ref* sender);
	void CreateNewRoomWithEnemy(Ref* sender);

	void ShowEndLevelWindow();

	void ClearWordsContainers();

    void searchUsers(Ref* sender);
    
	CWaitingWindow* m_WaitingWindow;
	Sprite * m_Background;
	Sprite * m_Overlay;

	bool m_FirstLaunch;

	int m_Bid;
	
	int m_Level;
	int m_SwapCount;
	int m_CurrentSwap;
	float m_Speed;
	int m_ThimblesCount;
	int m_BallThimbleId;
	bool m_IsWin;
	bool m_IsNeedToShowEnd;
	bool m_OneCupBonusActive;

	//int m_CurrentRoom;

	static CGameScene * _self;

	std::vector<CRoomsTable*> m_RoomsTables;
	CDictionary m_Dictionary;
	CSwapPanel * m_SwapPanel;
	CChooseLetterWindow* m_ChooseLetterWindow;
	CChangePlayerWindow* m_ChangePlayerWindow;
	CEndLevelWindow* m_EndLevelWindow;
	CBonusPanel* m_BonusPanel;

	CMenuItemWithDesciption *m_PlayButton;
	CMenuItemWithDesciption *m_RecallButton;
	CMenuItemWithDesciption *m_RematchButton;
	CCMenuItem *m_ShuffleButton;
	CMenuItemWithDesciption *m_PassButton;
	CMenuItemWithDesciption *m_SwapButton;
	CCMenuItem *m_MoreButton;
	CCMenuItem *m_LessButton;
    CMenuItemWithDesciption *searchButton;
    
	CCMenuItem *m_FacebookLoginBtn;
	LabelTTF * m_FacebookLoginLabel;

	CMenuItemWithDesciption *m_NewRoomButton;

	LabelTTF * m_ScoresLabel;

	///Используемые на текущем шаге буквы
	std::vector<CLetter*>m_UsedLetters; 
	std::vector<CLetter*>::iterator m_UsedLettersIter; 
	CLetter* m_ActiveLetter;

	//std::vector<CRoom*> m_Rooms;

	LabelTTF * m_Player_1_Name;
	LabelTTF * m_Player_2_Name;
	LabelTTF * m_Player_1_Score;
	LabelTTF * m_Player_2_Score;
	LabelTTF * m_StackLetterRemainLabel;
	LabelTTF * m_StackLetterRemainCount;

	bool m_IsChangingRooms;
	CPlayer* m_Player;
	int m_LoadedRoomId;

	int m_ScoresForCurrentMove;
	std::vector<std::string> m_FindedWords;
	std::vector<std::string> m_WrongWords;
};

#endif // NBG_GAME_SCENE
