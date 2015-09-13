//
//  SearchUser.h
//  FI
//
//  Created by Parth Parikh on 3/3/15.
//
//

#ifndef __FI__SearchUser__
#define __FI__SearchUser__

#include <stdio.h>
#include "cocos2d.h"

class SearchUser : public cocos2d::Node {
    
public:
    std::string name;
    std::string user_id;
    std::string email;
    std::string image_url;
    
    cocos2d::LabelTTF *labelName;
    cocos2d::Sprite *playerPhoto;
    
    cocos2d::MenuItemImage *createGame;
    cocos2d::Menu *createGameMenu;
    
    static SearchUser* create(std::string name,std::string user_id,std::string email,std::string image_url);
    bool init(std::string name,std::string user_id,std::string email,std::string image_url);
    
    void createGameBetweenTwoPlayers(cocos2d::Ref* sender);
    
};
#endif /* defined(__FI__SearchUser__) */
