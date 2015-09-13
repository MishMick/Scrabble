#include "WindowsInAppDelegate.h"

#include "GameScene.h"

//////////////////////////////////////////////////////////////////////////
CWindowsInAppDelegate::CWindowsInAppDelegate()
{

}

//////////////////////////////////////////////////////////////////////////
CWindowsInAppDelegate::~CWindowsInAppDelegate()
{

}

//////////////////////////////////////////////////////////////////////////
void CWindowsInAppDelegate::BuyItem(const std::string item_id)
{		
	SInAppResult res;
	res.result = R_SUCCESS;
	res.id = item_id;

	CGameScene * gameScene = CGameScene::Instance();
	if (gameScene)
	{	
		//gameScene->BuyItem(item_id);			
	}
}
