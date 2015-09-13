#include "Deck.h"
#include "xml.h"

#include "Stack.h"
#include "GameScene.h"

#include <algorithm>

float CDeck::ALIGN_LETTERS_TIME = 1.5f;
float CDeck::SHUFFLE_LETTERS_TIME = 1.5f;

//////////////////////////////////////////////////////////////////////////
CDeck::CDeck():Layer()
{
	m_Letters.resize(7);
	for (int i=0; i<m_Letters.size(); i++)
	{
		m_Letters[i] = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
CDeck::~CDeck()
{

}

CDeck* CDeck::create() {
    CDeck* obj = new CDeck();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CDeck::init()
{	
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CDeck::setVisible(bool visible)
{
	Layer::setVisible(visible);
	if(m_Letters.empty())
	{
		return;
	}
	for(int i=0;i<m_Letters.size();i++)
	{
		if(m_Letters[i] != NULL)
		{
			m_Letters[i]->setVisible(visible);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
CC_DEPRECATED_ATTRIBUTE void CDeck::setTouchEnabled(bool value)
{
	Layer::setTouchEnabled(value);
	if(m_Letters.empty())
	{
		return;
	}
	for(int i=0;i<m_Letters.size();i++)
	{
		if(m_Letters[i] != NULL)
		{
			m_Letters[i]->SetTouchPause(!value);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CDeck::FirstInitEmptyCells(CStack* stack)
{
	for (int i=0; i<m_Letters.size(); i++)
	{
		if (m_Letters[i] == NULL)
		{
			//CStack * stack = CGameScene::Instance()->GetStack();
			if(stack->IsEmpty())
			{
				return;
			}
			CLetter * letter = stack->GetLetterFromStack();	
			addChild(letter,CLetter::STAY_LAYER);
			float scale = 0.45;
			Point pos = Point(CELL_OFFSET_X+(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
			letter->setPosition(pos.x + i*CELL_SIZE_X,pos.y);
			letter->SetDeckCell(i);		
			letter->SetLastPosType(CLetter::LPT_DECK);
			m_Letters[i] = letter;		
		}
	}
	setVisible(false);
	setTouchEnabled(false);
}

//////////////////////////////////////////////////////////////////////////
void CDeck::InitEmptyCells()
{
	for (int i=0; i<m_Letters.size(); i++)
	{
		if (m_Letters[i] == NULL)
		{
			CStack * stack = CGameScene::Instance()->GetCurrentRoom()->GetStack();
			if(stack->IsEmpty())
			{
				return;
			}
			CLetter * letter = stack->GetLetterFromStack();	
			//CGameScene::Instance()->addChild(letter);
			addChild(letter,CLetter::STAY_LAYER);
			float scale = 0.45;

			Point pos = Point(CELL_OFFSET_X+(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
			letter->setPosition(pos.x + i*CELL_SIZE_X,pos.y);
			//letter->setOpacity(0);
			
			//letter->setScale(0.2f);
			letter->SetDeckCell(i);		
			letter->SetLastPosType(CLetter::LPT_DECK);
			m_Letters[i] = letter;		
			//letter->ShowAction();
			/*
			auto scaleAction	= CCScaleTo::create(0.5f,scale,scale);
			auto showAction		= CCFadeIn::create(0.3f);
			auto spawn			= Spawn::create(scaleAction,NULL);
			letter->runAction(spawn);*/
		}
	}
	ShowLetters();
}

//////////////////////////////////////////////////////////////////////////
float CDeck::InitEmptyCells(float delayPerLetter)
{
	float currentDelay = 0.0f;
	for (int i=0; i<m_Letters.size(); i++)
	{
		if (m_Letters[i] == NULL)
		{
			CStack * stack = CGameScene::Instance()->GetCurrentRoom()->GetStack();
			if(stack->IsEmpty())
			{
				return currentDelay;
			}
			CLetter * letter = stack->GetLetterFromStack();	
			addChild(letter,CLetter::STAY_LAYER);
			float scale = 0.45;

			Point pos = Point(CELL_OFFSET_X+(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
			letter->setPosition(pos.x + i*CELL_SIZE_X,pos.y);
			
			letter->SetDeckCell(i);		
			letter->SetLastPosType(CLetter::LPT_DECK);
			m_Letters[i] = letter;		
			currentDelay += delayPerLetter;
			letter->ShowAction(currentDelay);
		}
	}
	return currentDelay;
}

//////////////////////////////////////////////////////////////////////////
void CDeck::PutLetterToDeck(Touch * touch, CLetter * letter)
{
	Point l = touch->getLocation();
	int id = 0;
	if (l.x > CELL_OFFSET_X)
	{
		int x = l.x - CELL_OFFSET_X;
		id = x/CELL_SIZE_X;		
	}	

	Point pos = Point(CELL_OFFSET_X +(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
	if(m_Letters[id] != NULL)
	{
		int offset = 1;
		int freeCellId = id;
		while (true)
		{
			freeCellId = id - offset;
			if(freeCellId>=0)
			{
				if(m_Letters[freeCellId] == NULL)
				{
					id = freeCellId;
					break;
				}
			}
			freeCellId = id + offset;
			if(freeCellId < m_Letters.size())
			{
				if(m_Letters[freeCellId] == NULL)
				{
					id = freeCellId;
					break;
				}
			}
		}
	}

	letter->setPosition(pos.x + id*CELL_SIZE_X,pos.y);
	letter->SetDeckCell(id);			
	m_Letters[id] = letter;
}


//////////////////////////////////////////////////////////////////////////
void CDeck::PutLetterToDeck(int cell, CLetter * letter)
{
	Point pos = Point(CELL_OFFSET_X +(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
	pos.x += cell*CELL_SIZE_X;
	if(m_Letters[cell] == NULL)
	{
		m_Letters[cell] = letter;
		letter->setPosition(pos);
	}
	else
	{
		PutLetterToEmptyCell(letter);
	}
}

//////////////////////////////////////////////////////////////////////////
void CDeck::PutLetterToEmptyCell(CLetter * letter)
{
	Point pos = Point(CELL_OFFSET_X +(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
	for (int i=0; i<m_Letters.size(); i++)
	{
		if (m_Letters[i] == NULL)
		{			
			letter->setScale(0.45f);
			letter->setPosition(pos.x + i*CELL_SIZE_X,pos.y);
			letter->SetDeckCell(i);	
			letter->SetLastPosType(CLetter::LPT_DECK);
			m_Letters[i] = letter;	
			if(!letter->getParent())
			{
				addChild(letter,CLetter::STAY_LAYER);
			}
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CDeck::PutLetterToEmptyCell(char letterChar, int cost)
{
	CLetter * letter = CLetter::create();
	letter->retain();
	letter->SetCost(cost);
	letter->SetLetter(letterChar);	
	PutLetterToEmptyCell(letter);
}

//////////////////////////////////////////////////////////////////////////
void CDeck::CheckCellSwap(Touch * touch, CLetter * activeLetter)
{
	Point l = touch->getLocation();
	if(l.x > CELL_OFFSET_X && l.x < (CELL_OFFSET_X + (CELL_SIZE_X*m_Letters.size()-1)))
	{
		if(l.y > CELL_OFFSET_Y && l.y < (CELL_OFFSET_Y + CELL_SIZE_Y))
		{
			int id = (l.x - CELL_OFFSET_X)/CELL_SIZE_X;
			if(m_Letters[id] != NULL)
			{
				bool swapInLeft = true;
				int emptyId;
				if(id != 0)
				{
					for(int i=id-1;i>=0;i--)
					{
						if(m_Letters[i] == NULL)
						{
							emptyId = i;
							goto START_SWAP;
						}
					}
				}
				
				swapInLeft = false;
				for(int i=id+1;i<m_Letters.size();i++)
				{
					if(m_Letters[i] == NULL)
					{
						emptyId = i;
						break;
					}
				}
				
			START_SWAP:
				activeLetter->SetDeckCell(id);
				CLetter* letter;
				Point pos = Point(CELL_OFFSET_X +(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
				if(swapInLeft)
				{
					for(int i = emptyId;i < id;i++)
					{
						m_Letters[i] = m_Letters[i+1];
						m_Letters[i+1] = NULL;
						letter = m_Letters[i];
						letter->SetDeckCell(i);
						letter->MoveToAction(pos.x + i*CELL_SIZE_X,pos.y);
						//letter->setPosition(pos.x + i*CELL_SIZE_X,pos.y);
					}
				}
				else
				{
					for(int i = emptyId;i > id;i--)
					{
						m_Letters[i] = m_Letters[i-1];
						m_Letters[i-1] = NULL;
						letter = m_Letters[i];
						letter->SetDeckCell(i);
						//letter->setPosition(pos.x + i*CELL_SIZE_X,pos.y);
						letter->MoveToAction(pos.x + i*CELL_SIZE_X,pos.y);
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CDeck::ClearCell(const int cell)
{
	m_Letters[cell] = NULL;
}

//////////////////////////////////////////////////////////////////////////
void CDeck::AddLetterFromStack()
{
	
}

//////////////////////////////////////////////////////////////////////////
void CDeck::MakeShuffle()
{
	Point pos = Point(CELL_OFFSET_X +(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
	int fullCells = 0;
	for(int i=0;i<m_Letters.size();i++)
	{
		if(m_Letters[i] != NULL)
		{
			fullCells++;
		}
	}
	if(fullCells < m_Letters.size()-1)
	{
		AlignLetter(true);
	}
	std::random_shuffle(m_Letters.begin(),m_Letters.begin()+fullCells-1);
	for(int i=0;i<fullCells;i++)
	{
		CLetter* letter = m_Letters[i];
		if(letter->GetDeckCell() != i)
		{
			letter->SetDeckCell(i);
			//letter->setPosition(pos.x+i*CELL_SIZE_X,pos.y);
			letter->MoveToAction(pos.x+i*CELL_SIZE_X,pos.y);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CDeck::AlignLetter(bool instantly)
{
	Point pos = Point(CELL_OFFSET_X +(CELL_SIZE_X/2),CELL_OFFSET_Y+(CELL_SIZE_Y/2));
	int id = 0;
	for(int i=0;i<m_Letters.size();i++)
	{
		if(m_Letters[i] == NULL)
		{
			id = i+1;
			if(id < m_Letters.size())
			{
				while(id<m_Letters.size())
				{
					if(m_Letters[id] != NULL)
					{
						break;
					}
					else
					{
						id++;
					}
				}
				if(id>=m_Letters.size())
				{
					break;
				}
			}
			else
			{
				break;
			}
			if(m_Letters[id] != NULL)
			{
				m_Letters[i] = m_Letters[id];
				m_Letters[id] = NULL;
				m_Letters[i]->SetDeckCell(i);
				if(instantly)
				{
					m_Letters[i]->setPosition(pos.x + i*CELL_SIZE_X,pos.y);
				}
				else
				{
					m_Letters[i]->MoveToAction(pos.x + i*CELL_SIZE_X,pos.y);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CDeck::ShowLetters(float delay)
{
	float delayPerLetter = CLetter::SHOW_ACTION_TIME;
	float currentDelay = delay;
	for(int i=0;i<m_Letters.size();i++)
	{
		CLetter * letter = m_Letters[i];
		if(letter == NULL)
		{
			continue;
		}
		letter->ShowAction(currentDelay);
		currentDelay += delayPerLetter;
	}
}

//////////////////////////////////////////////////////////////////////////
void CDeck::HideLetters(float delay)
{
	float delayPerLetter = CLetter::SHOW_ACTION_TIME;
	float currentDelay = delay;
	for(int i=m_Letters.size()-1;i>=0;i--)
	{
		CLetter * letter = m_Letters[i];
		if(letter == NULL)
		{
			continue;
		}
		letter->HideAction(currentDelay);
		currentDelay += delayPerLetter;
	}
}

//////////////////////////////////////////////////////////////////////////
void CDeck::HideLettersInstantly()
{
	for(int i=m_Letters.size()-1;i>=0;i--)
	{
		CLetter * letter = m_Letters[i];
		if(letter == NULL)
		{
			continue;
		}
		letter->HideInstantly();
	}
}

//////////////////////////////////////////////////////////////////////////
void CDeck::Clear()
{
	removeAllChildren();
	for(int i=0;i<m_Letters.size();i++)
	{
		m_Letters[i] = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
int CDeck::GetLettersCount()
{
	int lettersCount = 0;
	for(int i=0;i<m_Letters.size();i++)
	{
		if(m_Letters[i] != NULL)
		{
			lettersCount ++;
		}
	}
	return lettersCount;
}

//////////////////////////////////////////////////////////////////////////
std::string CDeck::GetDeckInString()
{
	std::string deckString;
	for(int i=0;i<m_Letters.size();i++)
	{
		if(m_Letters[i] != NULL)
		{
			if(m_Letters[i]->GetLetter() == ' ')
			{
				deckString += '_';
			}
			else
			{
				deckString += m_Letters[i]->GetLetter();
			}
		}
		else
		{
			continue;
		}
	}
	return deckString;
}