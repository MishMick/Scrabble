//
//  SearchPopUp.cpp
//  FI
//
//  Created by Parth Parikh on 3/3/15.
//
//

#include "SearchPopUp.h"
#include "GameScene.h"
#include "SearchUser.h"

using namespace cocos2d;


SearchPopUp* SearchPopUp::instance = NULL;

SearchPopUp* SearchPopUp::getInstance() {
    return instance;
}

SearchPopUp* SearchPopUp::create() {
    SearchPopUp* obj = new SearchPopUp();
    obj->init();
    return obj;
}

bool SearchPopUp::init() {
    
    if (!Layer::init()) {
        return false;
    }
    
    UserDefault::getInstance()->setIntegerForKey("SearchPopUp_On", 1);
    
    instance = this;
    
    layer = LayerColor::create(Color4B(0, 0, 0, 200));
    this->addChild(layer);

    origin = Director::getInstance()->getVisibleOrigin();
    screenSize = Director::getInstance()->getWinSize();
    
    background = Sprite::create("button.png");
    background->setScale(6);
    background->setPosition(Vec2(origin.x+(screenSize.x*0.5), origin.y + (screenSize.y * 0.5)));
    this->addChild(background);
    
    closeBtn = MenuItemImage::create("closeBtn.png","closeBtn.png",CC_CALLBACK_1(SearchPopUp::closeSearchLoginPopup, this));
    closeBtn->setPosition(Vec2((origin.x + background->getPosition().x) + (background->getContentSize().width * background->getScale())/2, (origin.y + background->getPosition().y) + (background->getContentSize().height * background->getScale())/2));
    
    searchBtn = MenuItemImage::create("search.png","search.png",CC_CALLBACK_1(SearchPopUp::searchCallBack, this));
    searchBtn->setPosition(Vec2(((origin.x + background->getPosition().x) + (background->getContentSize().width * background->getScale())/2) - 70, ((origin.y + background->getPosition().y) + (background->getContentSize().height * background->getScale())/2) - 70));
    
    closeMenu = Menu::create(closeBtn,searchBtn, NULL);
    closeMenu->setPosition(Vec2::ZERO);
    this->addChild(closeMenu);
    
    addKeyBoardInput();
    
    return true;
}

void SearchPopUp::searchCallBack(cocos2d::Ref *sender) {
    log("in search call back called");
    
    for (int i = 0; i<searchUsers.size(); i++) {
        this->removeChild(searchUsers.at(i));
    }
    
    searchUsers.clear();
    
    std::string query = searchText->getString();
    
    CGameScene::Instance()->GetPlayer()->searchPlayers(query);
    
}

void SearchPopUp::searchResult(std::vector<std::string> searchNames, std::vector<std::string> searchEmails, std::vector<std::string> searchImageUrl, std::vector<std::string> searchUserId) {
    
    int i = 0;
    for(std::vector<std::string>::iterator it = searchNames.begin() ; it != searchNames.end(); ++it){
        CCLOG("name of searched User :: %s",searchNames.at(i).c_str());
        
        SearchUser *user = SearchUser::create(searchNames.at(i), searchUserId.at(i), searchEmails.at(i), searchImageUrl.at(i));
        user->setPosition(origin.x + screenSize.x/4,origin.y + (screenSize.y * 0.65) - (i * 100) );
        addChild(user, 1);
        
        searchUsers.pushBack(user);
        
        i++;
    }
    
}

void SearchPopUp::closeSearchLoginPopup(cocos2d::Ref *sender) {
    
    UserDefault::getInstance()->setIntegerForKey("SearchPopUp_On", 0);
    
    CCLOG("Close search login popup called");
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(SearchPopUp::cleanPopUp), 0.01);
}

void SearchPopUp::cleanPopUp(float dt) {
    closeMenu->removeAllChildrenWithCleanup(true);
    this->removeAllChildrenWithCleanup(true);
    
    CGameScene::Instance()->searchPopupCallback();
}

void SearchPopUp::addKeyBoardInput() {
    searchText = cocos2d::TextFieldTTF::textFieldWithPlaceHolder("Click To Search",
                                                                 cocos2d::Size(screenSize.x,100),TextHAlignment::LEFT , "Arial", 36);
    
    searchText->setPosition(origin.x + screenSize.x * 0.75 ,origin.y + screenSize.y * 0.8);
    searchText->setColorSpaceHolder(Color3B::BLACK);
    searchText->setDelegate(this);
    
    searchText->setColor(Color3B(255, 0, 0));
    
    this->addChild(searchText,1);
    
    searchText->attachWithIME();
    
    
    searchTouchListener = EventListenerTouchOneByOne::create();
    
    searchTouchListener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event * event) -> bool {
        try {
            
            auto searchText = dynamic_cast<TextFieldTTF *>(event->getCurrentTarget());
            
            Vec2 touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
            
            float minX = 0;
            float maxX = searchText->getContentSize().width;
            float minY = (searchText->getPosition().y - (searchText->getContentSize().height/2));
            float maxY = (searchText->getPosition().y + (searchText->getContentSize().height/2));
            
            CCLOG("MinX :: %f MaxX :: %f MinY :: %f MaxY :: %f",minX,maxX,minY,maxY);
            CCLOG("Touch Point X :: %f Y :: %f",touchPoint.x,touchPoint.y);
            
            if (touchPoint.x > minX && touchPoint.x < maxX && touchPoint.y > minY && touchPoint.y < maxY) {
                searchText->attachWithIME();
                searchText->setPlaceHolder("Text Here");
                CCLOG("Touched in email touch began");
            }
            
            //            CCLOG("Touched in email touch began %f %f",emailField->getContentSize().width,emailField->getContentSize().height);
            //            CCLOG("email pos :: %f %f",emailField->getPosition().x,emailField->getPosition().y);
            return true;
        }
        catch(std::bad_cast & err){
            return true;
        }
    };
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(searchTouchListener, searchText);
    

}


bool SearchPopUp::onTextFieldAttachWithIME(TextFieldTTF *sender) {
    return TextFieldDelegate::onTextFieldAttachWithIME(sender);
}

bool SearchPopUp::onTextFieldDetachWithIME(TextFieldTTF *sender) {
    searchText->setPlaceHolder("Click To Search");
    return TextFieldDelegate::onTextFieldDetachWithIME(sender);
}

bool SearchPopUp::onTextFieldInsertText(TextFieldTTF *sender, const char *text, size_t nLen) {
    if (searchText->getString().size() % 20 == 0 && searchText->getString().size() != 0) {
        std::string str = searchText->getString();
        str = str + '\n';
        searchText->setString(str);
    }
    if (searchText->getString().size() > 40) {
        text = "";
    }
    return TextFieldDelegate::onTextFieldInsertText(sender, text, nLen);
}

bool SearchPopUp::onTextFieldDeleteBackward(TextFieldTTF *sender, const char *delText, size_t nLen) {
    return TextFieldDelegate::onTextFieldDeleteBackward(sender, delText, nLen);
}

bool SearchPopUp::onVisit(TextFieldTTF *sender, Renderer *renderer, const Mat4 &transform, uint32_t flags) {
    return TextFieldDelegate::onVisit(sender, renderer, transform, flags);
}