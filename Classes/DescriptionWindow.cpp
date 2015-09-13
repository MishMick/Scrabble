#include "DescriptionWindow.h"
#include "xml.h"

#include "GameScene.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////
CDescriptionWindow::CDescriptionWindow():Layer()
{
	
}

//////////////////////////////////////////////////////////////////////////
CDescriptionWindow::~CDescriptionWindow()
{

}

CDescriptionWindow* CDescriptionWindow::create() {
    CDescriptionWindow *obj = new CDescriptionWindow();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CDescriptionWindow::init()
{	
	m_IsShow = false;

	m_DescriptionLabel = LabelTTF::create("0","helvetica",18,Size(WIDTH-(HORIZONTAL_INTENT*2),0));
	m_DescriptionLabel->setColor(Color3B(0.0f,0.0f,0.0f));
	m_DescriptionLabel->setString("Description");
	Size label_size = m_DescriptionLabel->getContentSize();
	label_size.height += VERTICAL_INTENT*2;
	setCascadeOpacityEnabled(true);
	setOpacity(0.0f);

	m_Sprite9 = Scale9Sprite::create("description.png");
	m_Sprite9->setContentSize(Size(WIDTH,label_size.height));
	m_Sprite9->setCascadeOpacityEnabled(true);
	addChild(m_Sprite9);
	Size size = m_Sprite9->getContentSize();
	m_DescriptionLabel->setPosition(size.width/2,size.height/2);
	m_Sprite9->addChild(m_DescriptionLabel);
	setVisible(false);
	setTouchEnabled(false);
	m_AlignType = AT_NONE;
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CDescriptionWindow::Show()
{
	setVisible(true);
	m_IsShow = true;
	auto alphaAction = CCFadeIn::create(0.3f);
	auto spawn			= Spawn::create(alphaAction,NULL);
	runAction(spawn);
}

//////////////////////////////////////////////////////////////////////////
void CDescriptionWindow::Hide()
{
	m_IsShow = false;
	auto alphaAction = CCFadeOut::create(0.3f);
	auto hideCompleteFunc = CallFuncN::create(this, callfuncN_selector(CDescriptionWindow::OnHideComplete));
	auto sequence			= Sequence::create(alphaAction,hideCompleteFunc,NULL);
	runAction(sequence);
}

//////////////////////////////////////////////////////////////////////////
void CDescriptionWindow::ShowInstantly()	/* Must call from target button */
{
	setVisible(true);
	m_IsShow = true;
	setOpacity(255.0f);
}

//////////////////////////////////////////////////////////////////////////
void CDescriptionWindow::HideInstantly()	/* Must call from target button */
{
	setOpacity(0.0f);
	m_IsShow = false;
	setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
void CDescriptionWindow::OnHideComplete(Node* sender)
{
	setVisible(false);
}
//////////////////////////////////////////////////////////////////////////
void CDescriptionWindow::SetDesciptionString(std::string description)
{
	m_DescriptionString = description;
	m_DescriptionLabel->setString(description);
	Size label_size = m_DescriptionLabel->getContentSize();
	label_size.height += VERTICAL_INTENT*2;
	m_Sprite9->setContentSize(Size(WIDTH,label_size.height));
	Size size = m_Sprite9->getContentSize();
	m_DescriptionLabel->setPosition(size.width/2,size.height/2);
}


//////////////////////////////////////////////////////////////////////////
void CDescriptionWindow::Align(ALIGN_TYPE type)
{
	if(m_Target)
	{
		m_AlignType = type;
		Size targetSize = m_Target->getContentSize()*m_Target->getScale();
		Size backSize = m_Sprite9->getContentSize()*getScale();
		Point targetPos = m_Target->getParent()->convertToWorldSpace(m_Target->getPosition());// + Point(512,384);//convertToWorldSpace(m_Target->getPosition()) + Point(512,384);
		Point pos = targetPos;
		
		switch (type)
		{
		case AT_BOTTOM:
			pos.y -= targetSize.height/2 + backSize.height/2;
			break;
		case AT_OVER:
			pos.y += targetSize.height/2 + backSize.height/2;
			break;
		case AT_LEFT:
			pos.x -= targetSize.width/2 + backSize.width/2;
			break;
		case AT_RIGHT:
			pos.x += targetSize.width/2 + backSize.width/2;
			break;
		}
		
		float leftEdge = pos.x - backSize.width/2;
		float rightEdge = pos.x + backSize.width/2;
		float upEdge = pos.y + backSize.height/2;
		float downEdge = pos.y - backSize.height/2;
		if(leftEdge < 10)
		{
			pos.x += (0 - leftEdge) + 10;
		}
		else if(rightEdge > CGameScene::WORKSPACE_POS_X + CGameScene::WORKSPACE_SIZE_X)
		{
			pos.x -= rightEdge - (CGameScene::WORKSPACE_POS_X + CGameScene::WORKSPACE_SIZE_X);
		}
		if(downEdge < 10)
		{
			pos.y += (0 - downEdge) + 10;
		}
		else if(upEdge > CGameScene::WORKSPACE_POS_Y + CGameScene::WORKSPACE_SIZE_Y)
		{
			pos.y -= upEdge - (CGameScene::WORKSPACE_POS_Y + CGameScene::WORKSPACE_SIZE_Y);
		}

		setPosition(pos);
	}
}

//////////////////////////////////////////////////////////////////////////
void CDescriptionWindow::RebuildPos()
{
	Align(m_AlignType);
}