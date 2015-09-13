#include "Field.h"
#include "xml.h"

#include "GameScene.h"

float CField::SHOW_TIME = 0.5f;
//////////////////////////////////////////////////////////////////////////
CField::CField()
{
	m_FieldOffset.x = 0.0f;
	m_FieldOffset.y = 0.0f;

	m_CellWidth		= 78;
	m_CellHeight	= 78;

	m_StartOffsetX = 35;
	m_StartOffsetY = 37;
}

//////////////////////////////////////////////////////////////////////////
CField::~CField()
{

}

CField* CField::create(int type) {
    CField *obj = new CField();
    obj->init(type);
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CField::init(int type)
{
    
    std::string fieldName;
    
//    if (type == 1) {
        fieldName = "field.png";
//    }
//    else {
//        fieldName = "field_standart.png";
//    }
    
	m_Field = Sprite::create(fieldName);
	//m_Field->setPosition(610,395);
	m_Field->setPosition(CGameScene::WORKSPACE_POS_X+CGameScene::WORKSPACE_SIZE_X/2 + m_Field->getContentSize().width,
							CGameScene::WORKSPACE_POS_Y+CGameScene::WORKSPACE_SIZE_Y/2);
	m_Field->setScale(0.41);
	setVisible(false);
	/*
	m_Over = Sprite::create("over.png");
	m_Over->setPosition(512,384);
	*/
	
	addChild(m_Field, 1);
	//addChild(m_Over, 2);

	

	//Initializing and binding 
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CField::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CField::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CField::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
//	auto mouseListener = EventListenerMouse::create();
//	mouseListener->onMouseScroll = CC_CALLBACK_1(CField::onZoom,this);
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
//#endif
	
	InitGrid(type);
	m_IsFirstWordPlased = false;

	return true;
}

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
//void CField::onZoom(Event *event)
//{
//	CRoom* room = CGameScene::Instance()->GetCurrentRoom();
//	if(isVisible() == false)
//	{
//		return;
//	}
//	else if(room->GetState() != CRoom::RS_IN_GAME)
//	{
//		return;
//	}
//	auto mouseEvent = static_cast<EventMouse*>(event);
//	float scale = m_Field->getScaleX();
//	float prevScale = scale;
//	scale += mouseEvent->getScrollY()*-0.1f;
//	if (scale < 0.41f)scale = 0.41f;
//	else if (scale > 0.9f)scale=0.9f;
//	if(scale == prevScale)
//	{
//		return;
//	}
//
//		
//	//float prevScale = m_Field->getScaleX();
//
//	m_Field->setScale(scale);
//	
//
//	Point vecTarget,vecResult,vecPosition;
//	vecTarget.x = mouseEvent->getCursorX();
//	vecTarget.y = mouseEvent->getCursorY();
//	vecPosition = m_Field->getPosition();
//	if (scale <= 0.42f && scale < prevScale)
//	{		
//		//m_Field->setPosition(610,395);
//		m_Field->setPosition(CGameScene::WORKSPACE_POS_X+CGameScene::WORKSPACE_SIZE_X/2,
//			CGameScene::WORKSPACE_POS_Y+CGameScene::WORKSPACE_SIZE_Y/2);
//		return;
//	}
//
//	float dx = (vecTarget.x - vecPosition.x) / prevScale;
//	float dy = (vecTarget.y - vecPosition.y) / prevScale;
//	vecResult.x = vecTarget.x - dx * scale;
//	vecResult.y = vecTarget.y - dy * scale;
//
//	Size size = (m_Field->getContentSize()*m_Field->getScale());
//	Point maxLimit = Point(CGameScene::WORKSPACE_POS_X+size.width/2,
//		CGameScene::WORKSPACE_POS_Y+size.height/2);
//	Point minLimit = Point((CGameScene::WORKSPACE_POS_X+CGameScene::WORKSPACE_SIZE_X)-size.width/2,
//		(CGameScene::WORKSPACE_POS_Y+CGameScene::WORKSPACE_SIZE_Y)-size.height/2);
//	if(vecResult.x > maxLimit.x) vecResult.x = maxLimit.x; 
//	else if(vecResult.x < minLimit.x) vecResult.x = minLimit.x;
//	if(vecResult.y > maxLimit.y) vecResult.y = maxLimit.y; 
//	else if(vecResult.y < minLimit.y) vecResult.y = minLimit.y;
//	m_Field->setPosition(vecResult.x,vecResult.y); // Двигаем камеру в эту точку
//
//}
//#endif

//////////////////////////////////////////////////////////////////////////
void CField::InitGrid(int type)
{
	pugi::xml_document doc;
    
    std::string path;
    
//    if (type == 1) {
        path = "field.xml";
//    }
//    else {
//        path = "field_standart.xml";
//    }
    
	loadXmlDocumentFromPath(doc,path);

	int row = 0;
	int col = 0;
	std::string rowS = "";
	for (pugi::xml_node rowNode = doc.first_child().first_child(); rowNode; rowNode = rowNode.next_sibling("row"))
	{	
		for (pugi::xml_node cell = rowNode.child("c"); cell; cell= cell.next_sibling("c"))
		{		
			int type = atoi(cell.child_value());

			rowS += cell.child_value();
			rowS += " ";
			m_Grid[col][row].modifier = (Modifier)type;
			m_Grid[col][row].letter = -1;
			m_Grid[col][row].isWithoutScore = false;
			m_Grid[col][row].fixed = false;
			m_Grid[col][row].pos = Point(col*m_CellWidth+m_StartOffsetX, ((GRID_HEIGHT-1)-row)*m_CellHeight+m_StartOffsetY);
			m_Grid[col][row].x = col;
			m_Grid[col][row].y = row;
			col++;
		}
		rowS = "";
		col = 0;
		row++;
	}	
}

//////////////////////////////////////////////////////////////////////////
void CField::Clear()
{
	m_Field->removeAllChildren();
	for (int row = 0; row < GRID_WIDTH; row++)
	{	
		for (int col = 0; col < GRID_HEIGHT; col++)
		{		
			m_Grid[col][row].letter = -1;
			m_Grid[col][row].isWithoutScore = false;
			m_Grid[col][row].fixed = false;
		}
	}	
	m_IsFirstWordPlased = false;
}

//////////////////////////////////////////////////////////////////////////
void CField::Delete()
{
	Clear();
	m_Field->removeFromParent();
}

//////////////////////////////////////////////////////////////////////////
bool CField::onTouchBegan(Touch *touch, Event *unused_event)
{
	if(isVisible() == false)
	{
		return false;
	}
	CRoom* room = CGameScene::Instance()->GetCurrentRoom();
	if(room->GetState() != CRoom::RS_IN_GAME)
	{
		return false;
	}
	if (m_Field->getBoundingBox().containsPoint(this->convertTouchToNodeSpace(touch)))
	{
		m_IsDrag = true;
		m_DragPos = touch->getLocation();
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
void CField::onTouchMoved(Touch *touch, Event *unused_event)
{
	if(isVisible() == false)
	{
		return;
	}
	if (m_IsDrag)
	{
		if(m_Field->getScale() <= 0.41)
		{
			return;
		}
		Point newPos = m_Field->getPosition()+touch->getDelta();
		Size size = (m_Field->getContentSize()*m_Field->getScale());
		Point maxLimit = Point(CGameScene::WORKSPACE_POS_X+size.width/2,
								CGameScene::WORKSPACE_POS_Y+size.height/2);
		Point minLimit = Point((CGameScene::WORKSPACE_POS_X+CGameScene::WORKSPACE_SIZE_X)-size.width/2,
								(CGameScene::WORKSPACE_POS_Y+CGameScene::WORKSPACE_SIZE_Y)-size.height/2);
		if(newPos.x>maxLimit.x)
		{
			newPos.x = maxLimit.x;
		}
		else if(newPos.x<minLimit.x)
		{
			newPos.x = minLimit.x;
		}
		if(newPos.y>maxLimit.y)
		{
			newPos.y = maxLimit.y;
		}
		else if(newPos.y<minLimit.y)
		{
			newPos.y = minLimit.y;
		}
		m_Field->setPosition(newPos);
		//m_Field->setPosition(m_Field->getPosition() + touch->getDelta());		
	}
}

//////////////////////////////////////////////////////////////////////////
void CField::onTouchEnded(Touch *touch, Event *unused_event)
{
	if(isVisible() == false)
	{
		return;
	}
	if(m_IsDrag)
	{
		m_IsDrag = false;
	}
}

SGridCell * CField::GetCell(const int x, const int y)
{
	if (x>=0&&x<GRID_WIDTH&&y>=0&&y<GRID_HEIGHT)
	{
		return &m_Grid[x][y];
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
SGridCell * CField::GetNearestEmptyCell(SGridCell * cell)	
{
	int radius = 1;
	int x = cell->x;
	int y = cell->y;
	SGridCell * result = NULL;	

	while (true)
	{
		result = GetCell(x-radius,y);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		result = GetCell(x,y-radius);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		result = GetCell(x+radius,y);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		result = GetCell(x,y+radius);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		radius++;
		if (radius > 6)break;
	}
	if (result)return result;

	while (true)
	{
		result = GetCell(x-radius,y);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		result = GetCell(x-radius,y-radius);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		result = GetCell(x,y-radius);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		result = GetCell(x+radius,y-radius);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		result = GetCell(x+radius,y);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		result = GetCell(x+radius,y+radius);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		result = GetCell(x,y+radius);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		result = GetCell(x-radius,y+radius);
		if (result && result->letter == -1 && result->modifier != M_Empty)break;
		radius++;
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
bool CField::CheckVerticalValid(int x, int y1, int y2)
{
	SGridCell * cell;
	for(int y=y1;y<y2;y++)
	{
		cell = &m_Grid[x][y];
		if(cell->letter == -1)
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CField::CheckHorizontalValid(int y, int x1, int x2)
{
	SGridCell * cell;
	for(int x=x1;x<x2;x++)
	{
		cell = &m_Grid[x][y];
		if(cell->letter == -1)
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
std::string CField::GetWordInColumn(const int col, const int startY, int &score)
{
	int sY = startY;
	if (sY > 0)
	{
		for (int y = startY; y >=0; y--)
		{
			SGridCell * cell = &m_Grid[col][y];
			if (cell->modifier == M_Empty)
			{
				sY = y+1;
				break;
			}
			if (cell->letter == -1)
			{
				sY = y+1;
				break;
			}
			if (y == 0)
			{
				sY = 0;
			}
		}
	}

	std::string res = "";
	for (int y=sY; y<GRID_HEIGHT; y++)
	{
		SGridCell * cell = &m_Grid[col][y];
		if (cell->modifier == M_Empty || cell->letter == -1)
		{
			break;
		}
		if(cell->isWithoutScore == false)
		{
			score +=  CGameScene::Instance()->GetPlayer()->GetLetterCost(cell->letter);//CGameScene::Instance()->GetCurrentRoom()->GetStack()->GetLetterCost(cell->letter);
		}
		res += cell->letter;
	}
	return res;	
}

//////////////////////////////////////////////////////////////////////////
std::string CField::GetWordInRow(const int row, const int startX, int &score)
{
	int sX = startX;
	if (sX > 0)
	{
		for (int x = startX; x >=0; x--)
		{
			SGridCell * cell = &m_Grid[x][row];
			if (cell->modifier == M_Empty)
			{
				sX = x+1;
				break;
			}
			if (cell->letter == -1)
			{
				sX = x+1;
				break;
			}
			if (x == 0)
			{
				sX = 0;
			}
		}
	}

	std::string res = "";
	for (int x=sX; x<GRID_WIDTH; x++)
	{
		SGridCell * cell = &m_Grid[x][row];
		if (cell->modifier == M_Empty || cell->letter == -1)
		{
			break;
		}
		if(cell->isWithoutScore == false)
		{
			score +=  CGameScene::Instance()->GetPlayer()->GetLetterCost(cell->letter);//CGameScene::Instance()->GetCurrentRoom()->GetStack()->GetLetterCost(cell->letter);
		}
		res += cell->letter;
	}
	return res;	
}

//////////////////////////////////////////////////////////////////////////
SGridCell * CField::GetCellUnderCursor(Touch * touch)
{
	float sX = m_CellWidth;
	float sY = m_CellHeight;
	Point pos = m_Field->convertTouchToNodeSpace(touch);
	int col = pos.x / sX;
	int row = pos.y / sY;
	row = (GRID_HEIGHT-1) - row;
	return GetCell(col,row);
}

//////////////////////////////////////////////////////////////////////////
SGridCell * CField::GetCellUnderCursor(Point pos)
{
	float sX = m_CellWidth;
	float sY = m_CellHeight;
	int col = pos.x / sX;
	int row = pos.y / sY;
	row = (GRID_HEIGHT-1) - row;
	if(col < 0) col = 0;
	else if(col > CField::GRID_WIDTH-1) col = CField::GRID_WIDTH;
	if(row < 0) row = 0;
	if(row > CField::GRID_HEIGHT-1) row = CField::GRID_WIDTH-1;
	return GetCell(col,row);
}

//////////////////////////////////////////////////////////////////////////
SGridCell* CField::GetCellForIndex(int index)
{
	int y = int(index/GRID_WIDTH);
	int x = index % GRID_WIDTH;
	return GetCell(x,y);
}

//////////////////////////////////////////////////////////////////////////
bool CField::CheckVerticalFixedTouch(int x, int y1, int y2)
{
	if(IsFirstWordPlased() == false)
	{
		return true;
	}
	SGridCell * cell;
	for(int row=y1-1;row<=y2+1;row++)
	{
		if(row < 0 || row >= GRID_HEIGHT)
		{
			continue;
		}
		for(int column = x-1;column <= x+1; column++)
		{
			if(column < 0 || column >= GRID_WIDTH)
			{
				continue;
			}
			cell = &m_Grid[column][row];
			if(cell->modifier != M_Empty && cell->fixed)
			{
				return true;
			}
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CField::CheckHorizontalFixedTouch(int y, int x1, int x2)
{
	if(IsFirstWordPlased() == false)
	{
		return true;
	}
	SGridCell * cell;
	for(int column=x1-1;column<=x2+1;column++)
	{
		if(column < 0 || column >= GRID_WIDTH)
		{
			continue;
		}
		for(int row = y-1;row <= y+1; row++)
		{
			if(row < 0 || row >= GRID_HEIGHT)
			{
				continue;
			}
			cell = &m_Grid[column][row];
			if(cell->modifier != M_Empty && cell->fixed)
			{
				return true;
			}
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
Point CField::GetUtmostVerticalCellPos(int startX, int startY)
{
	SGridCell * cell;
	int sx = startX;
	int sy = startY;
	for (int y=startY; y<GRID_HEIGHT; y++)
	{
		cell = &m_Grid[startX][y];
		if (cell->modifier == M_Empty || cell->letter == -1)
		{
			break;
		}
		else
		{
			sy = y;
		}
	}
	cell = m_Grid[sx,sy];
	Point pos = Point(m_CellWidth*(sx+1),
					m_CellHeight*(GRID_HEIGHT - (sy+1)));
	return pos;
	//return Point(m_CellWidth*sx,m_Field->getContentSize().height - (m_CellHeight*sy));
}

//////////////////////////////////////////////////////////////////////////
Point CField::GetUtmostHorizontalCellPos(int startX, int startY)
{
	SGridCell * cell;
	int sx = startX;
	int sy = startY;
	
	for (int x=startX; x<GRID_WIDTH; x++)
	{
		cell = &m_Grid[x][startY];
		if (cell->modifier == M_Empty || cell->letter == -1)
		{
			break;
		}
		else
		{
			sx = x;
		}
	}
	cell = m_Grid[sx,sy];
	Point pos = Point(m_CellWidth*(sx+1),
		m_CellHeight*(GRID_HEIGHT - (sy+1)));
	return pos;
	//return Point(m_CellWidth*sx,m_Field->getContentSize().height - (m_CellHeight*sy));
}

//////////////////////////////////////////////////////////////////////////
void CField::Show()
{
	Point startPos = Point((CGameScene::WORKSPACE_POS_X+CGameScene::WORKSPACE_SIZE_X/2)+(m_Field->getContentSize().width*0.41f),
		CGameScene::WORKSPACE_POS_Y+CGameScene::WORKSPACE_SIZE_Y/2);
	Point endPos = Point(CGameScene::WORKSPACE_POS_X+CGameScene::WORKSPACE_SIZE_X/2,
						CGameScene::WORKSPACE_POS_Y+CGameScene::WORKSPACE_SIZE_Y/2);
	m_Field->setPosition(startPos);
	m_Field->setScale(0.41f);
	setVisible(true);
	MoveTo * moveAction = MoveTo::create(0.5f,endPos);
	Spawn * spawn = Spawn::create(moveAction,NULL);
	m_Field->runAction(spawn);
}

//////////////////////////////////////////////////////////////////////////
void CField::Hide()
{
	Point endPos  = Point((CGameScene::WORKSPACE_POS_X+CGameScene::WORKSPACE_SIZE_X/2)+(m_Field->getContentSize().width*0.41f),
		CGameScene::WORKSPACE_POS_Y+CGameScene::WORKSPACE_SIZE_Y/2);
	Point startPos = Point(CGameScene::WORKSPACE_POS_X+CGameScene::WORKSPACE_SIZE_X/2,
		CGameScene::WORKSPACE_POS_Y+CGameScene::WORKSPACE_SIZE_Y/2);
	m_Field->setPosition(startPos);
	m_Field->setScale(0.41f);
	MoveTo * moveAction = MoveTo::create(0.5f,endPos);
	auto setInvisibleFunc = CallFuncN::create(this,callfuncN_selector(CField::setInvisible));
	Sequence * sequence = Sequence::create(moveAction, setInvisibleFunc, NULL);
	m_Field->runAction(sequence);
}

//////////////////////////////////////////////////////////////////////////
void CField::setInvisible(Node* sender)
{
	setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
int CField::GetIndexByPos(Point pos)
{
	return (pos.y * GRID_WIDTH) + pos.x;
}

//////////////////////////////////////////////////////////////////////////
void CField::AddLetterForIndex(char letter, int index, int cost, bool makeNew)
{
	CLetter * newLetter = CLetter::create();
	newLetter->retain();
	newLetter->SetCost(cost);
	newLetter->SetLetter(letter);	
	newLetter->SetStayOnField();
	SGridCell* cell = GetCellForIndex(index);
	newLetter->SetCell(cell);
	if(newLetter->GetCost() == 0)
	{
		cell->isWithoutScore = true;
	}
	cell->letter = newLetter->GetLetter();
	newLetter->SetFixed();
	newLetter->setPosition(newLetter->GetCell()->pos);
	GetFieldSprite()->addChild(newLetter,CLetter::STAY_LAYER);
	if(makeNew)
	{
		newLetter->SetNew();
	}
	newLetter->release();
}

//////////////////////////////////////////////////////////////////////////
void CField::ClearNewLetters()
{
	for(auto itr = m_Field->getChildren().begin();itr != m_Field->getChildren().end(); ++itr)
	{
		if((*itr)->getTag() == CLetter::LETTER_TAG)
		{
			CLetter* letter = reinterpret_cast<CLetter*>(*itr);
			if(letter->IsNew())
			{
				letter->SetOld();
			}
		}
	}
}