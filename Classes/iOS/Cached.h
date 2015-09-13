//
//  Cached.h
//  Scrabble
//
//  Created by Mishal Hemant Shah on 2/21/15.
//
//

#ifndef Scrabble_Cached_h
#define Scrabble_Cached_h

#include "cocos2d.h"//;
using namespace cocos2d;
#include <stdio.h>
class CacheLoginScene : public Layer
{
public:
    bool checkGoogle();
    bool checkFB();
    
    static CacheLoginScene* instance;
    static CacheLoginScene* getInstance();
    
    static CacheLoginScene* create();
    bool init();
    static Scene* createCacheScene();
    
    void decideScene(float dt);
    static bool replace_var;

    void fetchGoogleUserDetailsCallBack(bool var);
    int LoggedIn;
private:
    bool Googchecked,FBChecked;
    bool isCachedGoogle;
    bool isCachedFB;
};


#endif
