#include "AppDelegate.h"
#include "GameScene.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "Social_scene.h"
#include "Cached.h"
#endif
#include "SimpleAudioEngine.h"  
//#include "FBHelper.h"
#include "InApp.h"
#include "AndroidInAppDelegate.h"
#include "WindowsInAppDelegate.h"
#include "UserLogin.h"
//#include "GoogleHelper.h"
//ANDROID FILES
#include "Android/Cached_Android.h"
//iOS FILES
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}


bool AppDelegate::applicationDidFinishLaunching()
{
    
    CCLOG("applicationDidFinishLaunching Called");
        
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
		glview = GLViewImpl::create("My Game");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		glview->setFrameSize(1024,768);		
#endif
		director->setOpenGLView(glview);
	}
	glview->setDesignResolutionSize(1024,768,ResolutionPolicy::SHOW_ALL);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	CInApp::Instance()->SetDelegate(new CWindowsInAppDelegate());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	CInApp::Instance()->SetDelegate(new CAndroidInAppDelegate()); //make class for ios
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CInApp::Instance()->SetDelegate(new CAndroidInAppDelegate());
#endif
	srand((uintptr_t)this); 
	director->setDisplayStats(true);    
	director->setAnimationInterval(1.0 / 60);
    log("Appdelegate!");
    
 //   auto scene = Social_Scene::createSocialScene();
    // run
 //   Director::getInstance()->runWithScene(scene);
    
    //auto scene = CacheLoginScene::createCacheScene();
    
    
    FileUtils *pFileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths;
    
    searchPaths.push_back("animation/");
    searchPaths.push_back("animation/waiting/");
    
    searchPaths.push_back("fonts/");
    
    searchPaths.push_back("game/");

    searchPaths.push_back("game/elements/ball/");
    searchPaths.push_back("game/elements/letter/");
    searchPaths.push_back("game/elements/score_pointer/");
    searchPaths.push_back("game/elements/thimble/");
    
    searchPaths.push_back("game/levels/");
    searchPaths.push_back("game/levels/1/");
    searchPaths.push_back("game/levels/2/");
    searchPaths.push_back("game/levels/3/");
    
    searchPaths.push_back("gui/");
    searchPaths.push_back("gui/panels/");

    searchPaths.push_back("gui/buttons/");
    searchPaths.push_back("gui/windows/");

    searchPaths.push_back("sounds/");
    
    searchPaths.push_back("xml/");
    searchPaths.push_back("xml/dictionaries/");
    
    pFileUtils->setSearchPaths(searchPaths);

    CCLOG("File Utils path are set");
    
    //auto scene = CacheLoginScene::createCacheScene();
    //auto scene = UserLoginScene::createUserScene();
    
   auto scene = CacheLoginSceneAndroid::createCacheSceneAndroid();
    Director::getInstance()->runWithScene(scene);
    
    return true;
}

void AppDelegate::applicationDidReceivePushNotification(const char* pushType,const char* message) {
    CCLOG("Application did receive push notification in c++ appDelegate :: pushtype %s and message %s",pushType,message);
    if (strcmp(pushType, "gameChat") == 0) {
        CGameScene::Instance()->addChatMessage(message);
    }
    else {
        CGameScene::Instance()->CheckGamesUpdate(0);
    }
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() 
{
	Director::getInstance()->stopAnimation();    
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() 
{
    CGameScene::Instance()->CheckGamesUpdate(0);

	Director::getInstance()->startAnimation();    
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}