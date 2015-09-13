//
//  Social_scene.cpp
//  Scrabble
//
//  Created by Mishal Hemant Shah on 2/15/15.
//
//

#include "Social_scene.h"
#include "FBHelper.h"
#include "UserLogin.h"
#include "GoogleHelper.h"
#import <FacebookSDK/FacebookSDK.h>
#include "LoginSuccess.h"
//#include "EmailLoginPopUp.h"

using namespace cocos2d;
Scene* Social_Scene::createSocialScene()
{
    Scene * scene = Scene::create();
    Layer * layer = Social_Scene::create();
    scene->addChild(layer);
    
    return scene;
}

Social_Scene* Social_Scene::create() {
    Social_Scene* obj = new Social_Scene();
    obj->init();
    return obj;
}

bool Social_Scene::init()
{
    
    if ( Layer::init() == false )
    {
        return false;
    }

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 screenSize = Director::getInstance()->getWinSize();
    
    
    FBButton = MenuItemImage::create("FBLoginButton.jpeg","FBLoginButton.jpeg",CC_CALLBACK_1(Social_Scene::FBCallback, this));
    FBButton->setPosition(Vec2(origin.x + screenSize.x/2 , origin.y + screenSize.y * 0.8));
    FBButton->setTag(1);
    
    GoogleButton = MenuItemImage::create("GoogleSignButton.jpeg","GoogleSignButton.jpeg",CC_CALLBACK_1(Social_Scene::GPlusCallback, this));
    GoogleButton->setPosition(Vec2(origin.x + screenSize.x/2 , origin.y + screenSize.y * 0.5 ));
    GoogleButton->setTag(2);
    
    emailButton = MenuItemImage::create("loginWithEmail.jpeg","loginWithEmail.jpeg",CC_CALLBACK_1(Social_Scene::emailLoginCallback, this));
    emailButton->setPosition(Vec2(origin.x + screenSize.x/2 , origin.y + screenSize.y * 0.2 ));
    emailButton->setTag(2);
    
    auto upmenu8 = Menu::create(FBButton,GoogleButton,emailButton,NULL);
    upmenu8->setPosition(Vec2::ZERO);
    this->addChild(upmenu8,1);
    
    log("Social scene called!");
    
    return true;
    
}
void UserDetailsCallBackFunc(bool sar)
{
    if(sar==true)
    {
        log("user details have been fetched!");
        UserDefault::getInstance()->setIntegerForKey("Details_Fetched_From", 1);
        //UserDefault::getInstance()->setIntegerForKey("Login_From", 1);
        UserDefault::getInstance()->setIntegerForKey("Login_Count", 1);
        LogSuccess::LoginSuccessFrom();
    }
    else
    {
        UserDefault::getInstance()->setIntegerForKey("Login_From", 0);
        log("user details have not been fetched!");
    }
}
void LoginCallBackFunc(bool car)
{
    if(car==true)
    {
        log("Call back for login method called here!");
        Facebook::FetchUserDetails(UserDetailsCallBackFunc);
    }
    else
    {
        UserDefault::getInstance()->setIntegerForKey("Login_From", 0);
        log("Login fails!");
    }
}

void Social_Scene::emailLoginCallback(Ref* gsender) {
    CCLOG("email login called");
    
    GoogleButton->setEnabled(false);
    FBButton->setEnabled(false);
    emailButton->setEnabled(false);
    
  /*  EmailLoginPopUp * emailPopUpLayer = EmailLoginPopUp::create();
    this->addChild(emailPopUpLayer, 3); */
}

void Social_Scene::FBCallback(Ref* sender)
{
    MenuItemImage *pSender = (MenuItemImage*)sender;
    log("Facebook button pressed!");
    Facebook::Login(LoginCallBackFunc);
    
}
void Social_Scene::GPlusCallback(Ref* gsender)
{
    MenuItemImage *pSender = (MenuItemImage*)gsender;
    log("Google plus button pressed!");
    Google *gobj = [[Google alloc]init];
    //[[GPPSignIn sharedInstance]signOut];
    [gobj signInGoogle];
}
