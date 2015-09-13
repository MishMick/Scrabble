//
//  UserLogin.cpp
//  Scrabble
//
//  Created by Mishal Hemant Shah on 1/9/15.
//
//

#include "UserLogin.h"
#include "GameScene.h"

using namespace cocos2d;
Scene* UserLoginScene::createUserScene()
{
    Scene * scene = Scene::create();
    Layer * layer = UserLoginScene::create();
    scene->addChild(layer);
    
    return scene;
}

UserLoginScene* UserLoginScene::create() {
    UserLoginScene* obj = new UserLoginScene();
    obj->init();
    return obj;
}

bool UserLoginScene::init()
{
    if ( Layer::init() == false )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // position the label on the center of the screen
    
    // add the label as a child to this layer
    log("User login scene called!");
    
    log("Login_From :: %d",UserDefault::getInstance()->getIntegerForKey("Login_From",0));
    CCLOG("Name :: %s",UserDefault::getInstance()->getStringForKey("Name").c_str());
    log("Provider ID:: %s",UserDefault::getInstance()->getStringForKey("Provider_Id").c_str());
    log("Image URL :: %s",UserDefault::getInstance()->getStringForKey("Image_Url").c_str());
    log("Email ID:: %s",UserDefault::getInstance()->getStringForKey("Email").c_str());
    log("Provider :: %s",UserDefault::getInstance()->getStringForKey("provider").c_str());
    log("Device Type:: %s",UserDefault::getInstance()->getStringForKey("Device_Type").c_str());
    log("Device Token :: %s",UserDefault::getInstance()->getStringForKey("Device_Token").c_str());
    
    
    Item_1 = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(UserLoginScene::buttonCallback, this));
    Item_1->setPosition(Vec2(origin.x + Item_1->getContentSize().width/2 + 50 , origin.y + visibleSize.height - Item_1->getContentSize().height/2-250 ));
    Item_1->setTag(1);
    auto label_1 = MenuItemFont::create("User 1");
    label_1->setColor(Color3B(0,0,255));
    label_1->setPosition(Vec2(Item_1->getContentSize().width/2,Item_1->getContentSize().height/2));
    Item_1->addChild(label_1);
    auto upmenu = Menu::create(Item_1,NULL);
    upmenu->setPosition(Vec2::ZERO);
    this->addChild(upmenu,1);
    
    Item_2 = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(UserLoginScene::buttonCallback, this));
    Item_2->setPosition(Vec2(origin.x + Item_2->getContentSize().width/2 + 250 , origin.y + visibleSize.height - Item_2->getContentSize().height/2-250 ));
    Item_2->setTag(2);
    auto label_2 = MenuItemFont::create("User 2");
    label_2->setColor(Color3B(0,0,255));
    label_2->setPosition(Vec2(Item_2->getContentSize().width/2,Item_2->getContentSize().height/2));
    Item_2->addChild(label_2);
    auto upmenu2 = Menu::create(Item_2,NULL);
    upmenu2->setPosition(Vec2::ZERO);
    this->addChild(upmenu2,1);
    
    Item_3 = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(UserLoginScene::buttonCallback, this));
    Item_3->setPosition(Vec2(origin.x + Item_3->getContentSize().width/2 + 450 , origin.y + visibleSize.height - Item_3->getContentSize().height/2-250 ));
    Item_3->setTag(3);
    auto label_3 = MenuItemFont::create("User 3");
    label_3->setColor(Color3B(0,0,255));
    label_3->setPosition(Vec2(Item_3->getContentSize().width/2,Item_3->getContentSize().height/2));
    Item_3->addChild(label_3);
    auto upmenu3 = Menu::create(Item_3,NULL);
    upmenu3->setPosition(Vec2::ZERO);
    this->addChild(upmenu3,1);
    
    Item_4 = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(UserLoginScene::buttonCallback, this));
    Item_4->setPosition(Vec2(origin.x + Item_4->getContentSize().width/2 + 650 , origin.y + visibleSize.height - Item_4->getContentSize().height/2-250 ));
    Item_4->setTag(4);
    auto label_4 = MenuItemFont::create("User 4");
    label_4->setColor(Color3B(0,0,255));
    label_4->setPosition(Vec2(Item_4->getContentSize().width/2,Item_4->getContentSize().height/2));
    Item_4->addChild(label_4);
    auto upmenu4 = Menu::create(Item_4,NULL);
    upmenu4->setPosition(Vec2::ZERO);
    this->addChild(upmenu4,1);
    
    Item_5 = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(UserLoginScene::buttonCallback, this));
    Item_5->setPosition(Vec2(origin.x + Item_5->getContentSize().width/2 + 850 , origin.y + visibleSize.height - Item_5->getContentSize().height/2-250 ));
    Item_5->setTag(5);
    auto label_5 = MenuItemFont::create("User 5");
    label_5->setColor(Color3B(0,0,255));
    label_5->setPosition(Vec2(Item_5->getContentSize().width/2,Item_5->getContentSize().height/2));
    Item_5->addChild(label_5);
    auto upmenu5 = Menu::create(Item_5,NULL);
    upmenu5->setPosition(Vec2::ZERO);
    this->addChild(upmenu5,1);
    
    Item_6 = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(UserLoginScene::buttonCallback, this));
    Item_6->setPosition(Vec2(origin.x + Item_6->getContentSize().width/2 + 50 , origin.y + visibleSize.height - Item_6->getContentSize().height/2-450 ));
    auto label_6 = MenuItemFont::create("User 6");
    label_6->setColor(Color3B(0,0,255));
    label_6->setPosition(Vec2(Item_6->getContentSize().width/2,Item_6->getContentSize().height/2));
    Item_6->addChild(label_6);
    Item_6->setTag(6);
    auto upmenu6 = Menu::create(Item_6,NULL);
    upmenu6->setPosition(Vec2::ZERO);
    this->addChild(upmenu6,1);
    
    Item_7 = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(UserLoginScene::buttonCallback, this));
    Item_7->setPosition(Vec2(origin.x + Item_7->getContentSize().width/2 + 250 , origin.y + visibleSize.height - Item_7->getContentSize().height/2-450 ));
    auto label_7 = MenuItemFont::create("User 7");
    label_7->setColor(Color3B(0,0,255));
    label_7->setPosition(Vec2(Item_7->getContentSize().width/2,Item_7->getContentSize().height/2));
    Item_7->addChild(label_7);
    Item_7->setTag(7);
    auto upmenu7 = Menu::create(Item_7,NULL);
    upmenu7->setPosition(Vec2::ZERO);
    this->addChild(upmenu7,1);
    
    Item_8 = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(UserLoginScene::buttonCallback, this));
    Item_8->setPosition(Vec2(origin.x + Item_8->getContentSize().width/2 + 450 , origin.y + visibleSize.height - Item_8->getContentSize().height/2-450 ));
    Item_8->setTag(8);
    auto label_8 = MenuItemFont::create("User 8");
    label_8->setColor(Color3B(0,0,255));
    label_8->setPosition(Vec2(Item_8->getContentSize().width/2,Item_8->getContentSize().height/2));
    Item_8->addChild(label_8);
    auto upmenu8 = Menu::create(Item_8,NULL);
    upmenu8->setPosition(Vec2::ZERO);
    this->addChild(upmenu8,1);
    
    Item_9 = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(UserLoginScene::buttonCallback, this));
    Item_9->setTag(9);
    Item_9->setPosition(Vec2(origin.x + Item_9->getContentSize().width/2 + 650 , origin.y + visibleSize.height - Item_9->getContentSize().height/2-450 ));
    auto label_9 = MenuItemFont::create("User 9");
    label_9->setColor(Color3B(0,0,255));
    label_9->setPosition(Vec2(Item_9->getContentSize().width/2,Item_9->getContentSize().height/2));
    Item_9->addChild(label_9);
    auto upmenu9 = Menu::create(Item_9,NULL);
    upmenu9->setPosition(Vec2::ZERO);
    this->addChild(upmenu9,1);
    
    Item_10 = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(UserLoginScene::buttonCallback, this));
    Item_10->setTag(10);
    Item_10->setPosition(Vec2(origin.x + Item_10->getContentSize().width/2 + 850 , origin.y + visibleSize.height - Item_10->getContentSize().height/2-450 ));
    auto label_10 = MenuItemFont::create("User 10");
    label_10->setColor(Color3B(0,0,255));
    label_10->setPosition(Vec2(Item_10->getContentSize().width/2,Item_10->getContentSize().height/2));
    Item_10->addChild(label_10);
    auto upmenu10 = Menu::create(Item_10,NULL);
    upmenu10->setPosition(Vec2::ZERO);
    this->addChild(upmenu10,1);
    
    return true;
    
}

void UserLoginScene::buttonCallback(Ref* sender)
{
    MenuItemImage *pSender = (MenuItemImage*)sender;
    
    UserDefault::getInstance()->setIntegerForKey("Player_ID", pSender->getTag());

    log("User key for login:%d",UserLoginKey);
    Scene *startScene  =  CGameScene::createScene();
    Director::getInstance()->replaceScene(startScene);
}