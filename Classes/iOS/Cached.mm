//
//  Cached.m
//  Scrabble
//
//  Created by Mishal Hemant Shah on 2/21/15.
//
//

#include "Cached.h"
#include "Social_scene.h"
//#include "UserLogin.h"
#include "GameScene.h"
#include "GoogleHelper.h"
#include "FBHelper.h"
#include "LoginSuccess.h"

using namespace cocos2d;

CacheLoginScene* CacheLoginScene::instance = NULL;

CacheLoginScene* CacheLoginScene::getInstance() {
    if (instance == NULL) {
        return NULL;
    }
    return instance;
}

bool CacheLoginScene::checkGoogle()
{
    NSLog(@"Yes,cache login screen called!");
    Google *gobj = [[Google alloc]init];
    isCachedGoogle=[gobj isAuth];
    NSLog(@"%hhd",isCachedGoogle);
    return isCachedGoogle;
}

//void fetchUserDetailsCallbackFunc(bool var) {
//    if (var) {
//        auto scene = UserLoginScene::createUserScene();
//        Director::getInstance()->replaceScene(scene);
//    }
//    else {
//        auto scene = Social_Scene::createSocialScene();
//        Director::getInstance()->replaceScene(scene);
//    }
//}
//
//void CacheLoginScene::fetchGoogleUserDetailsCallBack(bool var) {
//    if (var)
//    {
//        //UserDefault::getInstance()->setIntegerForKey("Login_Count_GP", 2);
//        auto scene = UserLoginScene::createUserScene();
//        Director::getInstance()->replaceScene(scene);
//    }
//    else
//    {
//        auto scene = Social_Scene::createSocialScene();
//        Director::getInstance()->replaceScene(scene);
//    }
//}

bool CacheLoginScene::checkFB()
{
    return [FBSession openActiveSessionWithAllowLoginUI:NO];
}

bool CacheLoginScene::init() {
    
    if (!Layer::init()) {
        return false;
    }
    
    instance = this;
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(CacheLoginScene::decideScene),0.1);
    return true;
}

void CacheLoginScene::decideScene(float dt)
{
    //Googchecked = checkGoogle();
    //FBChecked = checkFB();
   /* if(FBChecked)
    {
        NSLog(@"FB checked true!");
        Facebook::FetchUserDetails(fetchUserDetailsCallbackFunc);
        UserDefault::getInstance()->setIntegerForKey("Login_Count_FB", 2);
    }
    else if(Googchecked)
    {
        NSLog(@"waiting for finish auth");
    } */
    
    LoggedIn=UserDefault::getInstance()->getIntegerForKey("Login_From", 0);

    if(LoggedIn!=0)
    {
        log("Login_From :: %d",UserDefault::getInstance()->getIntegerForKey("Login_From",0));
        CCLOG("Name :: %s",UserDefault::getInstance()->getStringForKey("Name").c_str());
        log("Provider ID:: %s",UserDefault::getInstance()->getStringForKey("Provider_Id").c_str());
        log("Image URL :: %s",UserDefault::getInstance()->getStringForKey("Image_Url").c_str());
        log("Email ID:: %s",UserDefault::getInstance()->getStringForKey("Email").c_str());
        log("Provider :: %s",UserDefault::getInstance()->getStringForKey("provider").c_str());
        log("Device Type:: %s",UserDefault::getInstance()->getStringForKey("Device_Type").c_str());
        log("Device Token :: %s",UserDefault::getInstance()->getStringForKey("Device_Token").c_str());
        UserDefault::getInstance()->setIntegerForKey("Login_Count", 2);
        
        auto scene = CGameScene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
    else
    {
        auto scene = Social_Scene::createSocialScene();
        Director::getInstance()->replaceScene(scene);
    }
}

CacheLoginScene* CacheLoginScene::create() {
    CacheLoginScene *obj = new CacheLoginScene();
    obj->init();
    return obj;
}

Scene* CacheLoginScene::createCacheScene()
{
    CCLOG("Cache Scene called");
    Scene * scene = Scene::create();
    Layer * layer = CacheLoginScene::create();
    scene->addChild(layer);
    
    return scene;
}