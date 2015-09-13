//
//  Social_scene.h
//  Scrabble
//
//  Created by Mishal Hemant Shah on 2/15/15.
//
//

#ifndef __Scrabble__Social_scene__
#define __Scrabble__Social_scene__

#include <stdio.h>
#include "cocos2d.h"//;
using namespace cocos2d;
class Social_Scene : public cocos2d::Layer
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
    static Social_Scene* create();
};

#endif /* defined(__Scrabble__Social_scene__) */
