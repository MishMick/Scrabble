//
//  UserLogin.h
//  Scrabble
//
//  Created by Mishal Hemant Shah on 1/9/15.
//
//

#ifndef __Scrabble__UserLogin__
#define __Scrabble__UserLogin__
#include "cocos2d.h"//;
using namespace cocos2d;
#include <stdio.h>
class UserLoginScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createUserScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    int UserLoginKey;
    MenuItemImage *Item_1,*Item_2,*Item_3,*Item_4,*Item_5,*Item_6,*Item_7,*Item_8,*Item_9,*Item_10;
    // a selector callback
    void menuCloseCallback(cocos2d::Object* pSender);
    void buttonCallback(Ref* sender);
    // implement the "static create()" method manually
    static UserLoginScene* create();
    
};
    
 
    

#endif /* defined(__Scrabble__UserLogin__) */
