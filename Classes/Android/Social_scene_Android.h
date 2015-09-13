//
//  Social_scene_Android.h
//  FI
//
//  Created by Mishal Hemant Shah on 4/2/15.
//
//

#ifndef __FI__Social_scene_Android__
#define __FI__Social_scene_Android__

#include <stdio.h>
#include "cocos2d.h"//;
using namespace cocos2d;
class Social_Scene_Android : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createSocialScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    MenuItemImage *FBButton,*GoogleButton,*emailButton;
    //static  bool FBLoginSuccess;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Object* pSender);
    void FBCallback(Ref* fbsender);
    void GPlusCallback(Ref* gsender);
    void emailLoginCallback(Ref* gsender);
    // implement the "static create()" method manually
    static Social_Scene_Android* create();
};

#endif /* defined(__FI__Social_scene_Android__) */
