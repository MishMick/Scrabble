#include "ChooseLetterWindow.h"


#include "GameScene.h"

//////////////////////////////////////////////////////////////////////////
CChooseLetterWindow::CChooseLetterWindow():cocos2d::Layer()
{
	
}

//////////////////////////////////////////////////////////////////////////
CChooseLetterWindow::~CChooseLetterWindow()
{

}

CChooseLetterWindow* CChooseLetterWindow::create() {
    CChooseLetterWindow* obj = new CChooseLetterWindow();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CChooseLetterWindow::init()
{	   
	m_Back = Sprite::create("choose_letter.png");
	m_Back->setPosition(600,384); 
	m_Back->setVisible(false);
	addChild(m_Back);
	std::vector<char> m_Letters;
	m_Letters.resize(26);
	m_Letters[0] = 'a'; m_Letters[1] = 'b'; m_Letters[2] = 'c'; m_Letters[3] = 'd'; m_Letters[4] = 'e'; m_Letters[5] = 'f'; m_Letters[6] = 'g'; 
	m_Letters[7] = 'h'; m_Letters[8] = 'i'; m_Letters[9] = 'j'; m_Letters[10] = 'k'; m_Letters[11] = 'l'; m_Letters[12] = 'm'; m_Letters[13] = 'n'; 
	m_Letters[14] = 'o'; m_Letters[15] = 'p'; m_Letters[16] = 'q'; m_Letters[17] = 'r'; m_Letters[18] = 's'; m_Letters[19] = 't'; m_Letters[20] = 'u'; 
	m_Letters[21] = 'v'; m_Letters[22] = 'w'; m_Letters[23] = 'x'; m_Letters[24] = 'y'; m_Letters[25] = 'z'; 

	float startPosX = -460.0f;
	float startPosY = -150.0f;
	float stepX = 45.0f;
	float stepY = 45.0f;
	int columnCount = 6;
	int rowCount = 4;
	int id = 0;
	int currentColumn = 0;
	int currentRow = 0;
	m_pMenu = CCMenu::create();
	for(int i=0;i<m_Letters.size();i++)
	{
		Point pos = Point(startPosX+(currentColumn*stepX),startPosY+(-currentRow*stepY));
		CCMenuItem * button = CCMenuItemImage::create(
			"base.png",
			"fixed.png",
			this,
			menu_selector(CChooseLetterWindow::LetterBtnClick));
		button->setTag(int(m_Letters[i]));
		button->setScale(0.35f);
		button->setPosition(pos);
		LabelTTF * letterLabel = LabelTTF::create("0","helvetica",82);	
		letterLabel->setPosition(button->getContentSize().width/2,button->getContentSize().height/2);
		letterLabel->setColor(Color3B(0,0,0));
		std::string str = "A";
		str[0] = m_Letters[i];
		std::transform(str.begin(), str.end(),str.begin(), ::toupper);
		letterLabel->setString(str);
		button->addChild(letterLabel);
		//letterLabel->setPosition(pos);

		currentColumn++;
		if(currentColumn == columnCount)
		{
			currentColumn = 0;
			currentRow++;
		}
		m_pMenu->addChild(button);
		m_pMenu->setEnabled(false);
	}

	m_Back->addChild(m_pMenu);


	return true;
}

//////////////////////////////////////////////////////////////////////////
void CChooseLetterWindow::LetterBtnClick(Object* sender)
{	
	if(!m_ChoosenLetter)
	{
		return;
	}
	CCMenuItem * button = (CCMenuItem*) sender;
	m_ChoosenLetter->SetLetter((char)button->getTag());
	Hide();
}

//////////////////////////////////////////////////////////////////////////
void CChooseLetterWindow::Show(CLetter * letter)
{
	m_ChoosenLetter = letter;
	m_Back->setVisible(true);
	m_pMenu->setEnabled(true);
}

//////////////////////////////////////////////////////////////////////////
void CChooseLetterWindow::Hide()
{
	m_ChoosenLetter = NULL;
	m_Back->setVisible(false);
	m_pMenu->setEnabled(false);
}
