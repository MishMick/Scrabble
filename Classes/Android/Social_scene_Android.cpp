//
//  Social_Scene_Android_Android.cpp
//  FI
//
//  Created by Mishal Hemant Shah on 4/2/15.
//
//

#include "Social_Scene_Android.h"
#include "JNICPPHelper.h"
using namespace cocos2d;
Scene* Social_Scene_Android::createSocialScene()
{
    Scene * scene = Scene::create();
    Layer * layer = Social_Scene_Android::create();
    scene->addChild(layer);
    
    return scene;
}

Social_Scene_Android* Social_Scene_Android::create() {
    Social_Scene_Android* obj = new Social_Scene_Android();
    obj->init();
    return obj;
}

bool Social_Scene_Android::init()
{
    
    if ( Layer::init() == false )
    {
        return false;
    }
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 screenSize = Director::getInstance()->getWinSize();
    
    
    FBButton = MenuItemImage::create("FBLoginButton.jpeg","FBLoginButton.jpeg",CC_CALLBACK_1(Social_Scene_Android::FBCallback, this));
    FBButton->setPosition(Vec2(origin.x + screenSize.x/2 , origin.y + screenSize.y * 0.8));
    FBButton->setTag(1);
    
    GoogleButton = MenuItemImage::create("GoogleSignButton.jpeg","GoogleSignButton.jpeg",CC_CALLBACK_1(Social_Scene_Android::GPlusCallback, this));
    GoogleButton->setPosition(Vec2(origin.x + screenSize.x/2 , origin.y + screenSize.y * 0.5 ));
    GoogleButton->setTag(2);
    
    emailButton = MenuItemImage::create("loginWithEmail.jpeg","loginWithEmail.jpeg",CC_CALLBACK_1(Social_Scene_Android::emailLoginCallback, this));
    emailButton->setPosition(Vec2(origin.x + screenSize.x/2 , origin.y + screenSize.y * 0.2 ));
    emailButton->setTag(2);
    
    auto upmenu8 = Menu::create(FBButton,GoogleButton,emailButton,NULL);
    upmenu8->setPosition(Vec2::ZERO);
    this->addChild(upmenu8,1);
    
    log("Social scene called!");
    
    return true;
    
}

void Social_Scene_Android::emailLoginCallback(Ref* gsender)
{
    log("email login called");
}

void Social_Scene_Android::FBCallback(Ref* sender)
{
    log("Facebook button pressed!");
    CallJavaLogin();
}
void Social_Scene_Android::GPlusCallback(Ref* gsender)
{
    log("Google plus button pressed!");
}
