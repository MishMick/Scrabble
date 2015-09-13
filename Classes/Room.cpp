#include "Room.h"
#include "xml.h"

#include "Stack.h"
#include "GameScene.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////
CRoom::CRoom():Node()
{
	
}

//////////////////////////////////////////////////////////////////////////
CRoom::~CRoom()
{

}

CRoom* CRoom::create(int boardType) {
    CRoom *obj = new CRoom();
    obj->init(boardType);
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CRoom::init(int boardType)
{	
	m_IsLoaded = false;
	CGameScene* scene = CGameScene::Instance();
	m_Field = CField::create(boardType);
	m_Turn = RT_FIRST_PLAYER;
	m_Type = RT_ONE_COMPUTER;
	m_State = RS_NONE;
	m_OldState = RS_NONE;
	scene->addChild(m_Field);
	m_ScorePlayer_1 = 0;
	m_ScorePlayer_2 = 0;
	m_Stack = new CStack();
	//m_Stack->CreateStack();
	m_DeckPlayer_1 = CDeck::create();
	//m_DeckPlayer_1->FirstInitEmptyCells(m_Stack);
	scene->addChild(m_DeckPlayer_1, CGameScene::OVER_GUI_LAYER);
	//m_DeckPlayer_2 = CDeck::create();
	//m_DeckPlayer_2->FirstInitEmptyCells(m_Stack);
	//scene->addChild(m_DeckPlayer_2, CGameScene::OVER_GUI_LAYER);
	m_ScorePointer_1 = CScorePointer::create();
	m_Field->GetFieldSprite()->addChild(m_ScorePointer_1, CScorePointer::ACTIVE_LAYER);
	m_ScorePointer_2 = CScorePointer::create();
	m_Field->GetFieldSprite()->addChild(m_ScorePointer_2, CScorePointer::ACTIVE_LAYER);
	m_Button = CRoomButton::create();
	m_Button->retain();
    //	scene->addChild(m_Button,CGameScene::GUI_LAYER);
	m_Id = -1;
	m_PassInARowCount = 0;
	m_FirstPlayerName = "Player 1";
	m_SecondPlayerName = "Player 2";
	return true;
}

void CRoom::SetBlandData(int server_id, int turn, int uid1, int uid2, std::string opp_name)
{
	m_ServerId = server_id;
	if(turn == 1)
	{
		m_Turn = RT_FIRST_PLAYER;
	}
	else
	{
		m_Turn =  RT_OPPONENT;
	}
	m_Uid1 = uid1;
	m_Uid2 = uid2;
	m_FirstPlayerName = CGameScene::Instance()->GetPlayer()->GetName();
	m_SecondPlayerName = opp_name;
	m_Button->SetOpponentName(opp_name);
	m_Button->SetServerId(server_id);
}

//////////////////////////////////////////////////////////////////////////
void CRoom::Rematch()
{
	m_Turn = RT_FIRST_PLAYER;
	m_State = RS_NONE;
	m_OldState = RS_NONE;
	m_ScorePointer_1->Hide();
	m_ScorePointer_2->Hide();
	m_Field->Clear();
	m_ScorePointer_1 = CScorePointer::create();
	m_ScorePointer_2 = CScorePointer::create();
	m_Field->GetFieldSprite()->addChild(m_ScorePointer_1, CScorePointer::ACTIVE_LAYER);
	m_Field->GetFieldSprite()->addChild(m_ScorePointer_2, CScorePointer::ACTIVE_LAYER);
	m_DeckPlayer_1->Clear();
//	m_DeckPlayer_2->Clear();
	m_ScorePlayer_1 = 0;
	m_ScorePlayer_2 = 0;
	m_Stack->Clear();
	m_Stack->CreateStack();
	m_DeckPlayer_1->FirstInitEmptyCells(m_Stack);
//	m_DeckPlayer_2->FirstInitEmptyCells(m_Stack);
	m_PassInARowCount = 0;
}

//////////////////////////////////////////////////////////////////////////
void CRoom::Remove()
{
    CCLOG("CRoom::Remove() Called");
//	m_Button->release();
	m_ScorePointer_1->Hide();
	m_ScorePointer_2->Hide();
	m_Field->Delete();
	m_DeckPlayer_1->Clear();
	m_DeckPlayer_2->Clear();
	m_Stack->Clear();
	m_Field->removeFromParent();
	m_DeckPlayer_1->removeFromParent();
	m_DeckPlayer_2->removeFromParent();
	removeAllChildren();
}

//////////////////////////////////////////////////////////////////////////
void CRoom::SetId(int id)
{
	m_Id = id;
	m_Button->SetId(id);
}

//////////////////////////////////////////////////////////////////////////
void CRoom::ChangeId(int id, int turn)
{
	m_Id = id;
	m_Button->ChangeId(id, turn);
}

//////////////////////////////////////////////////////////////////////////
void CRoom::AddCurrentPlayerScore(int score)
{
	if(m_Turn == RT_FIRST_PLAYER)
	{
		m_ScorePlayer_1 += score;
	}
	else
	{
		m_ScorePlayer_2 += score;
	}
}

//////////////////////////////////////////////////////////////////////////
CScorePointer* CRoom::GetCurrentPlayerScorePointer()
{
	return m_ScorePointer_1;
	/*
	if(m_Turn == RT_FIRST_PLAYER)
	{
		return m_ScorePointer_1;
	}
	else
	{
		return m_ScorePointer_2;
	}*/
}

//////////////////////////////////////////////////////////////////////////
void CRoom::SwapScorePointerState()
{
	if(m_Turn == RT_FIRST_PLAYER)
	{
		m_ScorePointer_1->SetActive(false);
		m_ScorePointer_2->SetActive(true);
		m_ScorePointer_2->Hide();
	}
	else
	{
		m_ScorePointer_1->SetActive(true);
		m_ScorePointer_2->SetActive(false);
		m_ScorePointer_1->Hide();
	}
}

//////////////////////////////////////////////////////////////////////////
CDeck* CRoom::GetCurrentPlayerDeck()
{
	return m_DeckPlayer_1;
	/*
	if(m_Turn == RT_FIRST_PLAYER)
	{
		return m_DeckPlayer_1;
	}
	else
	{
		return m_DeckPlayer_2;
	}*/
}

//////////////////////////////////////////////////////////////////////////
void CRoom::Show()
{
	m_Field->Show();
	CDeck * deck = GetFirstPlayerDeck();
	deck->setTouchEnabled(true);
	deck->setVisible(true);
	deck->ShowLetters();
	if(m_OldState != RS_END_GAME)
	{
		m_State = RS_IN_GAME;
	}
	else
	{
		m_State = m_OldState;
	}
	SetScorePointerVisible(CGameScene::Instance()->GetPlayer()->IsWordMetterOn());
}

//////////////////////////////////////////////////////////////////////////
void CRoom::Hide()
{
	m_Field->Hide();
	CDeck * deck = GetFirstPlayerDeck();
	deck->HideLettersInstantly();
	m_DeckPlayer_1->setTouchEnabled(false);
	//m_DeckPlayer_2->setTouchEnabled(false);
	m_DeckPlayer_1->setVisible(false);
	//m_DeckPlayer_2->setVisible(false);
	m_Button->setEnable(true);
	m_OldState = m_State;
	m_State = RS_HIDED;
}

//////////////////////////////////////////////////////////////////////////
void CRoom::SetState(ROOM_STATE state)
{
	m_OldState = m_State;
	m_State = state;
}

//////////////////////////////////////////////////////////////////////////
void CRoom::ReturnOldState()
{
	CRoom::ROOM_STATE state = m_State;
	m_State = m_OldState;
	m_OldState = state;
}

//////////////////////////////////////////////////////////////////////////
void CRoom::SetScorePointerVisible(bool visible)
{
	float opacity = 0.0f;
	if(visible)
	{
		opacity = 255.0f;
	}
	m_ScorePointer_1->setOpacity(opacity);
	m_ScorePointer_2->setOpacity(opacity);
}