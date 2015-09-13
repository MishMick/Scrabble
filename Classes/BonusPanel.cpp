#include "BonusPanel.h"
#include "xml.h"

#include "GameScene.h"
#include "ConfirmWindow.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////
CBonusPanel::CBonusPanel():Layer()
{
	
}

//////////////////////////////////////////////////////////////////////////
CBonusPanel::~CBonusPanel()
{

}

CBonusPanel* CBonusPanel::create() {
    CBonusPanel* obj = new CBonusPanel();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CBonusPanel::init()
{	
	m_IsWordMetterOn = true;


	m_Background = Sprite::create("bonus_panel.png");
	float posY = m_Background->getContentSize().height/2;
	m_Background->setPosition(613,posY); 
	addChild(m_Background);

	m_ResignBtn = CMenuItemWithDesciption::create(
		"resign.png",
		"resign_over.png",
		"resign_disable.png",
		this,
		menu_selector(CBonusPanel::ResingBtnClick));
	m_ResignBtn->setTag(BT_RESIGN);
	m_ResignBtn->setPosition(-460,-340);
	m_ResignBtn->setEnabled(false);
	m_ResignBtn->setScale(0.5f);
	m_ResignBtn->SetDescription("Resign");
	m_ResignBtn->setVisible(false);

	m_WordMetterOffBtn = CMenuItemWithDesciption::create(
		"wordmetter_off.png",
		"wordmetter_off_over.png",
		this,
		menu_selector(CBonusPanel::WordMetterStateSwap));
	m_WordMetterOffBtn->setTag(BT_WORDMETTER_OFF);
	m_WordMetterOffBtn->setPosition(162,-340);
	m_WordMetterOffBtn->setEnabled(false);
	m_WordMetterOffBtn->setScale(0.5f);
	m_WordMetterOffBtn->SetDescription("WordMetter Off");
	m_WordMetterOffBtn->setVisible(false);

	m_WordMetterOnBtn = CMenuItemWithDesciption::create(
		"wordmetter_on.png",
		"wordmetter_on.png",
		this,
		menu_selector(CBonusPanel::WordMetterStateSwap));
	m_WordMetterOnBtn->setTag(BT_WORDMETTER_ON);
	m_WordMetterOnBtn->setPosition(162,-340);
	m_WordMetterOnBtn->setEnabled(false);
	m_WordMetterOnBtn->setScale(0.5f);
	m_WordMetterOnBtn->SetDescription("WordMetter On");
	m_WordMetterOnBtn->setVisible(false);

	CCMenu* pMenu = CCMenu::create(m_ResignBtn, m_WordMetterOffBtn, m_WordMetterOnBtn, NULL);
	m_Background->addChild(pMenu);
	m_Background->setCascadeOpacityEnabled(true);

	setCascadeOpacityEnabled(true);
	setOpacity(0.0f);
	m_IsShow = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CBonusPanel::Show()
{
	m_IsShow = true;
	Point moveBy = Point(0.0f,91.0f);
	auto showAction	= CCMoveBy::create(0.3f,moveBy);
	auto alphaAction = CCFadeIn::create(0.3f);
	auto showCompleteFunc = CallFuncN::create(this, callfuncN_selector(CBonusPanel::OnShowComplete));
	auto spawn	= Spawn::create(showAction,alphaAction,NULL);
	auto sequence = Sequence::create(spawn,showCompleteFunc,NULL);
	m_ResignBtn->setEnabled(true);
	m_ResignBtn->setVisible(true);
	if(m_IsWordMetterOn)
	{
		m_WordMetterOnBtn->setEnabled(true);
		m_WordMetterOnBtn->setVisible(true);
	}
	else
	{
		m_WordMetterOffBtn->setEnabled(true);
		m_WordMetterOffBtn->setVisible(true);
	}

	runAction(sequence);
}

//////////////////////////////////////////////////////////////////////////
void CBonusPanel::Hide()
{
	m_IsShow = false;
	Point moveBy = Point(0.0f,-91.0f);
	auto showAction	= CCMoveBy::create(0.3f,moveBy);
	auto alphaAction = CCFadeOut::create(0.3f);
	auto hideCompleteFunc = CallFuncN::create(this, callfuncN_selector(CBonusPanel::OnHideComplete));
	auto spawn	= Spawn::create(showAction,alphaAction,NULL);
	auto sequnce = Sequence::create(spawn,hideCompleteFunc,NULL);
	m_ResignBtn->setEnabled(false);
	runAction(sequnce);
}

//////////////////////////////////////////////////////////////////////////
void CBonusPanel::OnHideComplete(Node* sender)
{
	m_ResignBtn->setVisible(false);
	m_WordMetterOnBtn->setVisible(false);
	m_WordMetterOffBtn->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
void CBonusPanel::OnShowComplete(Node* sender)
{
	m_ResignBtn->GetDescriptionWindow()->RebuildPos();
	m_WordMetterOnBtn->GetDescriptionWindow()->RebuildPos();
	m_WordMetterOffBtn->GetDescriptionWindow()->RebuildPos();
}

//////////////////////////////////////////////////////////////////////////
void CBonusPanel::ResingBtnClick(Ref* sender)
{
	CConfirmWindow* confirmWindow = CConfirmWindow::create();
    confirmWindow->SetConfirmLabel("Are you Sure?");
	confirmWindow->SetOkCallback(this,callfuncN_selector(CBonusPanel::ConfirmOkCallback));
	addChild(confirmWindow);
	confirmWindow->Show();
}

//////////////////////////////////////////////////////////////////////////
void CBonusPanel::WordMetterStateSwap(Ref* sender)
{
	m_IsWordMetterOn = !m_IsWordMetterOn;
	m_WordMetterOnBtn->setVisible(m_IsWordMetterOn);
	m_WordMetterOnBtn->setEnabled(m_IsWordMetterOn);
	m_WordMetterOffBtn->setVisible(!m_IsWordMetterOn);
	m_WordMetterOffBtn->setEnabled(!m_IsWordMetterOn);
	if(m_IsWordMetterOn == false && m_WordMetterOnBtn->GetDescriptionWindow()->IsShow())
	{
		m_WordMetterOnBtn->InstantlyHideDescriptionWindow();
		m_WordMetterOffBtn->InstantlyShowDescriptionWindow();
	}
	else if(m_IsWordMetterOn && m_WordMetterOffBtn->GetDescriptionWindow()->IsShow())
	{
		m_WordMetterOffBtn->InstantlyHideDescriptionWindow();
		m_WordMetterOnBtn->InstantlyShowDescriptionWindow();
	}
	CGameScene::Instance()->GetPlayer()->SwapWordMetterState();
}

//////////////////////////////////////////////////////////////////////////
void CBonusPanel::ConfirmOkCallback(Node* sender)
{
	CGameScene::Instance()->HideBonusPanel();
    CGameScene::Instance()->GetPlayer()->leaveGame();
}

//////////////////////////////////////////////////////////////////////////
void CBonusPanel::SetWordMetterState(bool on)
{
	m_IsWordMetterOn = on;
	if(m_IsShow)
	{
		m_WordMetterOnBtn->setVisible(m_IsWordMetterOn);
		m_WordMetterOffBtn->setVisible(!m_IsWordMetterOn);
	}
}