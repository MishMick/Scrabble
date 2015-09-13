#include "Stack.h"
#include "GameScene.h"
#include "xml.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////
CStack::CStack()
{
}

//////////////////////////////////////////////////////////////////////////
CStack::~CStack()
{

}

//////////////////////////////////////////////////////////////////////////
bool CStack::Init()
{		
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CStack::CreateStack()
{	
	pugi::xml_document doc;
	loadXmlDocumentFromPath(doc,"xml/dictionaries/en_config.xml");

	pugi::xml_node chars = doc.child("characters");

	int id = 0;

	for (pugi::xml_node character = chars.first_child(); character; character = character.next_sibling("character"))
	{		
		int count = character.attribute("count").as_int();
		for (int i=0; i<count; i++)
		{			
			CLetter * letter = CLetter::create();
			letter->retain();
			letter->SetCost(character.attribute("cost").as_int());
			letter->SetLetter(character.attribute("id").value()[0]);	
			m_Letters.push_back(letter);		
			m_LettersCost[character.attribute("id").value()[0]] = character.attribute("cost").as_int();
		}		
	}	
	std::random_shuffle(m_Letters.begin(), m_Letters.end());
};

//////////////////////////////////////////////////////////////////////////
void CStack::PushLetterToStack(char letterChar, int cost)
{
	CLetter * letter = CLetter::create();
	letter->retain();
	letter->SetCost(cost);
	letter->SetLetter(letterChar);	
	m_Letters.push_back(letter);		
	m_LettersCost[letterChar] = cost;
}

//////////////////////////////////////////////////////////////////////////
CLetter * CStack::GetLetterFromStack()
{
	CLetter * letter = m_Letters.front();	
	m_Letters.pop_front();
	SetStackRemainLabel();
	return letter;
}

//////////////////////////////////////////////////////////////////////////
void CStack::SetStackRemainLabel()
{
	CGameScene * scene = CGameScene::Instance();
	if(m_Letters.empty())
	{
		scene->SetStackRemainLabel(0);
	}
	else
	{
		scene->SetStackRemainLabel(m_Letters.size());
	}
}

//////////////////////////////////////////////////////////////////////////
int CStack::GetRemainLettersCount()
{
	if(m_Letters.empty())
	{
		return 0;
	}
	else
	{
		return m_Letters.size();
	}
}

//////////////////////////////////////////////////////////////////////////
CLetter * CStack::SwapLetter(CLetter* letterToStack, int count)
{
	if(count > m_Letters.size()-1)
	{
		return letterToStack;
	}
	CLetter * letterFromStack = m_Letters[count];
	m_Letters[count] = letterToStack;
	m_Letters[count]->HideInstantly();
	return letterFromStack;
}

///////////////////////////////////////////////////////////////////////////
void CStack::ShuffleLetters()
{
	if(!m_Letters.empty() && m_Letters.size() != 1)
	{
		std::random_shuffle(m_Letters.begin(),m_Letters.end());
	}
}

//////////////////////////////////////////////////////////////////////////
bool CStack::IsEmpty()
{
	return m_Letters.empty();
}

//////////////////////////////////////////////////////////////////////////
void CStack::Clear()
{
	m_Letters.clear();
}

//////////////////////////////////////////////////////////////////////////
std::string CStack::GetStackInString(int from)
{
	std::string stackString;
	if(m_Letters.empty() || m_Letters.size() < from)
	{
		stackString = "";
		return stackString;
	}
	for(int i=from;i<m_Letters.size();i++)
	{
		if(m_Letters[i]->GetLetter() == ' ')
		{
			stackString += '_';
		}
		else
		{
			stackString += m_Letters[i]->GetLetter();
		}
	}
	return stackString;
}

//////////////////////////////////////////////////////////////////////////
std::string CStack::GetFirstLettersInString(int count)
{
	std::string stackString;
	if(m_Letters.empty())
	{
		return stackString;
	}
	for(int i=0;i<count;i++)
	{
		if(i>=count)
		{
			break;
		}
		if(m_Letters[i]->GetLetter() == ' ')
		{
			stackString += '_';
		}
		else
		{
			stackString += m_Letters[i]->GetLetter();
		}
	}
	return stackString;
}