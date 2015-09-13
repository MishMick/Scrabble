#include "BidWindow.h"


#include "GameScene.h"

//////////////////////////////////////////////////////////////////////////
CBidWindow::CBidWindow():cocos2d::Layer()
{
	m_Panel = NULL;	
}

//////////////////////////////////////////////////////////////////////////
CBidWindow::~CBidWindow()
{

}

CBidWindow* CBidWindow::create() {
    CBidWindow *obj = new CBidWindow();
    obj->init();
    return obj;
}
//////////////////////////////////////////////////////////////////////////
bool CBidWindow::init()
{	   
    m_Panel = Sprite::create("bid.png");
    addChild(m_Panel);

	m_Bid50 = CCMenuItemImage::create(
		"bid50.png",
		"bid50_over.png",
		this,
		menu_selector(CBidWindow::ProceedBidClick));
	m_Bid50->setTag(50);
	m_Bid50->setScale(0.7f);
	m_Bid50->setPosition(-180.0f,0.0f);

	m_Bid100 = CCMenuItemImage::create(
		"bid100.png",
		"bid100_over.png",
		this,
		menu_selector(CBidWindow::ProceedBidClick));
	m_Bid100->setTag(100);
	m_Bid100->setScale(0.7f);
	m_Bid100->setPosition(-180.0f,-100.0f);

	m_Bid250 = CCMenuItemImage::create(
		"bid250.png",
		"bid250_over.png",
		this,
		menu_selector(CBidWindow::ProceedBidClick));
	m_Bid250->setTag(250);
	m_Bid250->setScale(0.7f);
	m_Bid250->setPosition(-180.0f,-200.0f);

	m_Bid500 = CCMenuItemImage::create(
		"bid500.png",
		"bid500_over.png",
		this,
		menu_selector(CBidWindow::ProceedBidClick));
	m_Bid500->setTag(500);
	m_Bid500->setScale(0.7f);
	m_Bid500->setPosition(-180.0f,-300.0f);
	

	CCMenu* pMenu = CCMenu::create(m_Bid50, m_Bid100,m_Bid250, m_Bid500, NULL);
	m_Panel->addChild(pMenu);


	return true;
}

//////////////////////////////////////////////////////////////////////////
void CBidWindow::ProceedBidClick(Object* sender)
{
	MenuItem * item = reinterpret_cast<MenuItem*>(sender);
	int cost = item->getTag();	
	int playerMoney = CCUserDefault::getInstance()->getIntegerForKey("COINS",0);

	if (cost > playerMoney)
	{
		MessageBox("You have no enough coins!", "Warning");
		return;
	}

	playerMoney -= cost;
	CCUserDefault::getInstance()->setIntegerForKey("COINS",playerMoney);

	setVisible(false);

//	CGameScene::Instance()->MakeBid(cost);
}

