//
//  Cached_Android.h
//  FI
//
//  Created by Mishal Hemant Shah on 4/2/15.
//
//

#ifndef __FI__Cached_Android__
#define __FI__Cached_Android__

#include <stdio.h>
#include "cocos2d.h"//;
using namespace cocos2d;
#include <stdio.h>
class CacheLoginSceneAndroid : public Layer
{
public:
    
    static CacheLoginSceneAndroid* instance;
    static CacheLoginSceneAndroid* getInstance();
    
    static CacheLoginSceneAndroid* create();
    bool init();
    void decideScene(float dt);
    static Scene* createCacheSceneAndroid();
    int LoggedIn;

};
#endif /* defined(__FI__Cached_Android__) */
