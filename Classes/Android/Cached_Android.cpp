//
//  Cached_Android.cpp
//  FI
//
//  Created by Mishal Hemant Shah on 4/2/15.
//
//

#include "Cached_Android.h"
#include "GameScene.h"
#include "Social_scene_Android.h"
using namespace cocos2d;

CacheLoginSceneAndroid* CacheLoginSceneAndroid::instance = NULL;

CacheLoginSceneAndroid* CacheLoginSceneAndroid::getInstance() {
    if (instance == NULL) {
        return NULL;
    }
    return instance;
}
bool CacheLoginSceneAndroid::init() {
    
    if (!Layer::init()) {
        return false;
    }
    
    instance = this;
    
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
    
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(CacheLoginSceneAndroid::decideScene),0.1);
    return true;
}
void CacheLoginSceneAndroid::decideScene(float dt)
{
    
    CCLOG("Decide Scene called");
//    LoggedIn=UserDefault::getInstance()->getIntegerForKey("Login_From", 0);
//    
//    if(LoggedIn!=0)
//    {
//        log("Login_From :: %d",UserDefault::getInstance()->getIntegerForKey("Login_From",0));
//        CCLOG("Name :: %s",UserDefault::getInstance()->getStringForKey("Name").c_str());
//        log("Provider ID:: %s",UserDefault::getInstance()->getStringForKey("Provider_Id").c_str());
//        log("Image URL :: %s",UserDefault::getInstance()->getStringForKey("Image_Url").c_str());
//        log("Email ID:: %s",UserDefault::getInstance()->getStringForKey("Email").c_str());
//        log("Provider :: %s",UserDefault::getInstance()->getStringForKey("provider").c_str());
//        log("Device Type:: %s",UserDefault::getInstance()->getStringForKey("Device_Type").c_str());
//        log("Device Token :: %s",UserDefault::getInstance()->getStringForKey("Device_Token").c_str());
//        UserDefault::getInstance()->setIntegerForKey("Login_Count", 2);
//        
//        auto scene = CGameScene::createScene();
//        Director::getInstance()->replaceScene(scene);
//    }
//    else
//    {
        auto scene = Social_Scene_Android::createSocialScene();
        Director::getInstance()->replaceScene(scene);
//    }
}

CacheLoginSceneAndroid* CacheLoginSceneAndroid::create() {
    CacheLoginSceneAndroid *obj = new CacheLoginSceneAndroid();
    obj->init();
    return obj;
}

Scene* CacheLoginSceneAndroid::createCacheSceneAndroid()
{
    CCLOG("Cache Scene called");
    Scene * scene = Scene::create();
    Layer * layer = CacheLoginSceneAndroid::create();
    scene->addChild(layer);
    
    return scene;
}
