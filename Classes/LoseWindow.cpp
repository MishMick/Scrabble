#include "LoseWindow.h"


#include "GameScene.h"

//////////////////////////////////////////////////////////////////////////
CLoseWindow::CLoseWindow():cocos2d::Layer()
{
	m_Panel = NULL;	
}

//////////////////////////////////////////////////////////////////////////
CLoseWindow::~CLoseWindow()
{

}

CLoseWindow* CLoseWindow::create() {
    CLoseWindow *obj = new CLoseWindow();
    obj->init();
    return obj;
}

//////////////////////////////////////////////////////////////////////////
bool CLoseWindow::init()
{	   
    m_Panel = Sprite::create("lose.png");
    addChild(m_Panel);

	m_Restart = CCMenuItemImage::create(
		"restart.png",
		"restart_over.png",
		this,
		menu_selector(CLoseWindow::ProceedClick));
	m_Restart->setTag(IN_APP_RESTART);
	m_Restart->setScale(0.7f);
	m_Restart->setPosition(-180.0f,0.0f);
	
	CCMenu* pMenu = CCMenu::create(m_Restart, NULL);
	m_Panel->addChild(pMenu);


	return true;
}

//////////////////////////////////////////////////////////////////////////
void CLoseWindow::ProceedClick(Object* sender)
{	
	CCMenuItem * button = (CCMenuItem*) sender;
}
