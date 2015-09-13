//
//  SearchUser.cpp
//  FI
//
//  Created by Parth Parikh on 3/3/15.
//
//

#include "SearchUser.h"
#include "GameScene.h"
#include "SearchPopUp.h"

using namespace cocos2d;

SearchUser* SearchUser::create(std::string name, std::string user_id, std::string email, std::string image_url) {
    SearchUser *obj = new SearchUser();
    obj->init(name, user_id, email, image_url);
    return obj;
}

bool SearchUser::init(std::string name, std::string user_id, std::string email, std::string image_url) {
    
    if (!Node::init()) {
        return false;
    }
    
    this->name = name;
    this->user_id = user_id;
    this->email = email;
    this->image_url = image_url;
    
    playerPhoto = Sprite::create("DefaultImage.png");
    playerPhoto->setPosition(0,0);
    addChild(playerPhoto,1);
    
    labelName = LabelTTF::create(name,"helvetica",26);
    labelName->setColor(Color3B(255.0f,0.0f,0.0f));
    labelName->setPosition(100,0);
    labelName->setAnchorPoint(Point(0.5f,0.5f));
    addChild(labelName,1);
    
    createGame = MenuItemImage::create(  "play.png",
                                    "play_over.png",
                                    "play_disable.png",
                                    this,
                                    menu_selector(SearchUser::createGameBetweenTwoPlayers));
    
    createGame->setScale(0.5f);
    createGame->setPosition(250,0);
    
    createGameMenu = Menu::create(createGame, NULL);
    createGameMenu->setPosition(Vec2::ZERO);
    addChild(createGameMenu);
    
    return true;
}

void SearchUser::createGameBetweenTwoPlayers(cocos2d::Ref *sender) {
    
    if (CGameScene::Instance()->GetPlayer()->getRooms().size() >= 5) {
        MessageBox("You have reached max limit of playing game.", "Finish Games");
        return;
    }
    
    int opp_id = atoi(user_id.c_str());
    int my_id = UserDefault::getInstance()->getIntegerForKey("Player_ID");
    
    CCLOG("Opponent UserId :: %d",atoi(user_id.c_str()));
    CCLOG("MY UserId :: %d",UserDefault::getInstance()->getIntegerForKey("Player_ID"));
    
    CGameScene::Instance()->GetPlayer()->createGameBetweenTwoPlayer(my_id, opp_id);
    
    SearchPopUp::getInstance()->closeSearchLoginPopup(0);
}


