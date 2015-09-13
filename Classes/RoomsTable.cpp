#include "RoomsTable.h"
#include "GameScene.h"

//////////////////////////////////////////////////////////////////////////
CRoomsTable::CRoomsTable():cocos2d::Node()
{
	
}

//////////////////////////////////////////////////////////////////////////
CRoomsTable::~CRoomsTable()
{

}

CRoomsTable* CRoomsTable::create() {
    CRoomsTable* obj = new CRoomsTable();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CRoomsTable::init()
{	   
	m_Marker = Sprite::create("rooms_state_mark.png");
	m_Marker->setPosition(0,0);
	addChild(m_Marker,1);

	m_MarkerLabel = LabelTTF::create("0","helvetica",18);	
	m_MarkerLabel->setColor(Color3B(255.0f,255.0f,255.0f));
	m_MarkerLabel->setPosition(m_Marker->getContentSize().width/2,m_Marker->getContentSize().height/2+10.0f);
	m_MarkerLabel->setAnchorPoint(Point(0.5f,0.5f));
	m_Marker->addChild(m_MarkerLabel,1);

	pMenu = CCMenu::create(NULL);
	addChild(pMenu);

	setVisible(false);

	return true;
}

//////////////////////////////////////////////////////////////////////////
void CRoomsTable::SetMarkerLabel(std::string label)
{
	m_MarkerLabel->setString(label);
}

//////////////////////////////////////////////////////////////////////////
int CRoomsTable::GetTableHeight()
{
	if(m_Buttons.empty())
	{
		return 0;
	}
	int height = MARKER_HEIGHT + ((m_Buttons.size())*BUTTON_HEIGHT);
	return height;
}

//////////////////////////////////////////////////////////////////////////
void CRoomsTable::PutToTable(CRoomButton* button)
{
    CCLOG("Put To Table Called");
//    button->setEnable(true);
    
    CCLOG("Before :: %lu",m_Buttons.size());
    
	m_Buttons.push_back(button);
    
    CCLOG("After :: %lu",m_Buttons.size());
    
//    button->setVisible(true);
	addChild(button,2);
    button->retain();
    
	RebuildButtonsPos();
	if(isVisible() == false)
	{
		setVisible(true);
	}
}

//////////////////////////////////////////////////////////////////////////
void CRoomsTable::DeleteFromTable(CRoomButton* button)
{
    
    CCLOG("Delete From Table called");
	int id=-1;
    
    CCLOG("m_Buttons Array Count :: %lu",m_Buttons.size());
    
	for(int i=0;i<m_Buttons.size();i++)
	{
		if(m_Buttons[i] == button)
		{
			id = i;
			break;
		}
	}

	if(id != -1)
	{
        CCLOG("retain count before erase :: %d",button->getReferenceCount());
        
        removeChild(button,false);

        CCLOG("Before :: %lu",m_Buttons.size());
        
		m_Buttons.erase(m_Buttons.begin()+id);
       
        CCLOG("After :: %lu",m_Buttons.size());
        
        CCLOG("retain count after erase :: %d",button->getReferenceCount());

//        button->setEnable(false);
//        button->setVisible(false);
        
        CCLOG("retain count after removeChild :: %d",button->getReferenceCount());

	}
    
	RebuildButtonsPos();
	if(m_Buttons.empty())
	{
		setVisible(false);
	}
}

//////////////////////////////////////////////////////////////////////////
void CRoomsTable::RebuildButtonsPos()
{
	Point pos = m_Marker->getPosition();
	for(int i=0;i<m_Buttons.size();i++)
	{
		CRoomButton* button = m_Buttons[i];
		button->setPosition(0.0f,0.0f-(MARKER_HEIGHT+BUTTON_HEIGHT*i));
	}
}

//////////////////////////////////////////////////////////////////////////
void CRoomsTable::MarkerAlignLeft()
{
	m_Marker->setPosition(-37.0f,0.0f);
}

//////////////////////////////////////////////////////////////////////////
void CRoomsTable::MarkerAlignRight()
{
	m_Marker->setPosition(37.0f,0.0f);
}

//////////////////////////////////////////////////////////////////////////
int CRoomsTable::GetFirstRoomId()
{
	return m_Buttons[0]->GetId();
}