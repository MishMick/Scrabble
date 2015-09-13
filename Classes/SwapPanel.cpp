#include "SwapPanel.h"
#include "xml.h"

#include "Stack.h"
#include "GameScene.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////
CSwapPanel::CSwapPanel():Layer()
{
	m_Letters.resize(7);
	for (int i=0; i<m_Letters.size(); i++)
	{
		m_Letters[i] = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
CSwapPanel::~CSwapPanel()
{

}

CSwapPanel* CSwapPanel::create() {
    CSwapPanel* obj = new CSwapPanel();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CSwapPanel::init()
{	
	m_Background = Sprite::create("swap_panel.png");
	float posY = m_Background->getContentSize().height/2;
	m_Background->setPosition(613,posY); 
	addChild(m_Background);

	m_OkBtn = CCMenuItemImage::create(
		"swap_ok.png",
		"swap_ok_over.png",
		this,
		menu_selector(CSwapPanel::StartSwap));
	m_OkBtn->setTag(IN_APP_PASS);
	m_OkBtn->setPosition(190,-320);

	m_CancelBtn = CCMenuItemImage::create(
		"swap_cancel.png",
		"swap_cancel_over.png",
		this,
		menu_selector(CSwapPanel::CancelSwap));
	m_CancelBtn->setTag(IN_APP_SHUFFLE);
	m_CancelBtn->setPosition(190,-360);

	CCMenu* pMenu = CCMenu::create(m_OkBtn, m_CancelBtn, NULL);
	m_Background->addChild(pMenu);
	m_Background->setCascadeOpacityEnabled(true);
	
	m_OkBtn->setEnabled(false);
	m_CancelBtn->setEnabled(false);
	setCascadeOpacityEnabled(true);
	setOpacity(0.0f);

	return true;
}

//////////////////////////////////////////////////////////////////////////////
void CSwapPanel::StartSwap(Ref* sender)
{
	std::vector<CLetter*> swapLetters;
	for(int i=0;i<m_Letters.size();i++)
	{
		if(m_Letters[i] != NULL)
		{
			swapLetters.push_back(m_Letters[i]);
			m_Letters[i] = NULL;
		}
	}

	if(!swapLetters.empty())
	{
		CGameScene* scene = CGameScene::Instance();
		CRoom* room = scene->GetCurrentRoom();
		room->IncreasePassInARowCount();
		CDeck* deck = room->GetCurrentPlayerDeck();
		CStack * stack = room->GetStack();
		CLetter* letter;
		CLetter* letterFromStack;
		int count = 0;
		float currentDelay = 0.2f;
		float delayPerLetter = 0.15f;
		deck->AlignLetter();
		for(int i=0;i<swapLetters.size();i++)
		{
			letter = swapLetters[i];
            log("letter for swap :: %c",letter->GetLetter());
			letterFromStack = stack->SwapLetter(letter,count);
			letter->removeFromParent();
			letter->SetDeckCell(-1);
			letter->SetSwapPanelCell(-1);
			count++;
			if(count == swapLetters.size())
			{
				deck->PutLetterToEmptyCell(letterFromStack);
				stack->ShuffleLetters();
				HideOnSwap();
				letterFromStack->ShowAction(currentDelay);
				currentDelay+=delayPerLetter;
				CGameScene::Instance()->SendSwapMove(currentDelay);
				//scene->NextPlayerTurn(currentDelay);
			}
			else
			{
				currentDelay+=delayPerLetter;
				deck->PutLetterToEmptyCell(letterFromStack);
				letterFromStack->ShowAction(currentDelay);
			}
		}
		
	}
}

//////////////////////////////////////////////////////////////////////////////
void CSwapPanel::CancelSwap()
{
	CGameScene * scene = CGameScene::Instance();
	CDeck * deck = scene->GetCurrentRoom()->GetCurrentPlayerDeck();
	for(int i=0;i<m_Letters.size();i++)
	{
		if(m_Letters[i] != NULL)
		{
			deck->PutLetterToEmptyCell(m_Letters[i]);
			m_Letters[i] = NULL;
		}
	}
	Hide();
}

//////////////////////////////////////////////////////////////////////////////
void CSwapPanel::CancelSwap(Ref* sender)
{
	CGameScene * scene = CGameScene::Instance();
	CDeck * deck = scene->GetCurrentRoom()->GetCurrentPlayerDeck();
	for(int i=0;i<m_Letters.size();i++)
	{
		if(m_Letters[i] != NULL)
		{
			deck->PutLetterToEmptyCell(m_Letters[i]);
			m_Letters[i] = NULL;
		}
	}
	Hide();
}

//////////////////////////////////////////////////////////////////////////
void CSwapPanel::PutLetterToSwapPanel(Touch * touch, CLetter * letter)
{
	Point l = touch->getLocation();
	int id = 0;
	if (l.x > CELL_OFFSET_X)
	{
		int x = l.x - CELL_OFFSET_X;
		id = x/CELL_SIZE_X;		
	}	

	Point pos = Point(CELL_OFFSET_X +(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
	for (int i=id; i<m_Letters.size(); i++)
	{
		if (m_Letters[i] == NULL)
		{			
			letter->setPosition(pos.x + i*CELL_SIZE_X,pos.y);
			letter->SetSwapPanelCell(i);			
			m_Letters[i] = letter;	
			return;
		}
	}

	for (int i=0; i<m_Letters.size(); i++)
	{
		if (m_Letters[i] == NULL)
		{			
			letter->setPosition(pos.x + i*CELL_SIZE_X,pos.y);
			letter->SetSwapPanelCell(i);			
			m_Letters[i] = letter;	
			return;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void CSwapPanel::PutLetterToSwapPanel(int cell, CLetter * letter)
{
	Point pos = Point(CELL_OFFSET_X +(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
	pos.x += cell*CELL_SIZE_X;
	m_Letters[cell] = letter;
	letter->setPosition(pos);
}

/////////////////////////////////////////////////////////////////////////////
void CSwapPanel::ClearCell(int cell)
{
	m_Letters[cell] = NULL;
}

//////////////////////////////////////////////////////////////////////////
void CSwapPanel::Show()
{
	Point moveBy = Point(0.0f,91.0f);
	auto showAction	= CCMoveBy::create(0.3f,moveBy);
	auto alphaAction = CCFadeIn::create(0.3f);
	auto spawn			= Spawn::create(showAction,alphaAction,NULL);
	m_OkBtn->setEnabled(true);
	m_CancelBtn->setEnabled(true);
	runAction(spawn);
}

//////////////////////////////////////////////////////////////////////////
void CSwapPanel::Hide()
{
	Point moveBy = Point(0.0f,-91.0f);
	CallFuncN *blockButtonsFunc = CallFuncN::create(CGameScene::Instance(), callfuncN_selector(CGameScene::UnblockControlButtons));
	auto showAction	= CCMoveBy::create(0.3f,moveBy);
	auto alphaAction = CCFadeOut::create(0.3f);
	auto spawn			= Spawn::create(showAction,alphaAction,NULL);
	auto sequence = Sequence::create(spawn,blockButtonsFunc,NULL);
	m_OkBtn->setEnabled(false);
	m_CancelBtn->setEnabled(false);
	runAction(sequence);
}

//////////////////////////////////////////////////////////////////////////
void CSwapPanel::HideOnSwap()
{
	Point moveBy = Point(0.0f,-91.0f);
	auto showAction	= CCMoveBy::create(0.3f,moveBy);
	auto alphaAction = CCFadeOut::create(0.3f);
	auto spawn			= Spawn::create(showAction,alphaAction,NULL);
	m_OkBtn->setEnabled(false);
	m_CancelBtn->setEnabled(false);
	runAction(spawn);
}