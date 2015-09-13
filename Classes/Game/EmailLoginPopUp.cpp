//
//  EmailLoginPopUp.cpp
//  FI
//
//  Created by Parth Parikh on 2/27/15.
//
//

#include "EmailLoginPopUp.h"
#include <regex>
#include "LoginSuccess.h"

using namespace cocos2d;

#define EMAIL    1
#define PASSWORD 2

EmailLoginPopUp* EmailLoginPopUp::create() {
    EmailLoginPopUp* obj = new EmailLoginPopUp();
    obj->init();
    return obj;
}

bool EmailLoginPopUp::init() {
    
    if (!Layer::init()) {
        return false;
    }
    
//    std::string email = "parth16.parikh@gmail.com";
//    
//    
//    bool valid = std::regex_match(email, std::regex("\^[_a-z0-9-]+(.[_a-z0-9-]+)*@[a-z0-9-]+(.[a-z0-9-]+)*(.[a-z]{2,4})\$") );
//    
//    if( valid ) {
//        CCLOG("email is valid");
//    }
//    else {
//        CCLOG("email is not valid");
//    }
    
    layer = LayerColor::create(Color4B(0, 0, 0, 200));
    this->addChild(layer);
    
    origin = Director::getInstance()->getVisibleOrigin();
    screenSize = Director::getInstance()->getWinSize();
    
    background = Sprite::create("button.png");
    background->setScale(5);
    background->setPosition(Vec2(origin.x+(screenSize.x*0.5), origin.y + (screenSize.y * 0.6)));
    this->addChild(background);
    
    loginBtn = MenuItemImage::create("login.png","login.png",CC_CALLBACK_1(EmailLoginPopUp::loginCallBack, this));
    loginBtn->setPosition(Vec2((origin.x + background->getPosition().x), origin.y + background->getPosition().y - (background->getContentSize().height)));
    
    loginMenu = Menu::create(loginBtn, NULL);
    loginMenu->setPosition(Vec2::ZERO);
    this->addChild(loginMenu);
    
    closeBtn = MenuItemImage::create("closeBtn.png","closeBtn.png",CC_CALLBACK_1(EmailLoginPopUp::closeEmailLoginPopup, this));
    closeBtn->setPosition(Vec2((origin.x + background->getPosition().x) + (background->getContentSize().width * background->getScale())/2, (origin.y + background->getPosition().y) + (background->getContentSize().height * background->getScale())/2));
    
    closeMenu = Menu::create(closeBtn, NULL);
    closeMenu->setPosition(Vec2::ZERO);
    this->addChild(closeMenu);
    
    addKeyBoardInput();
    
    return true;
}

std::string EmailLoginPopUp::email;
std::string EmailLoginPopUp::pwd;
std::string EmailLoginPopUp::name;

void EmailLoginPopUp::loginCallBack(cocos2d::Ref *sender) {
    
    email = emailField->getString();
    pwd = password->getString();
    
    bool valid = std::regex_match(email, std::regex("\^[_a-z0-9-]+(.[_a-z0-9-]+)*@[a-z0-9-]+(.[a-z0-9-]+)*(.[a-z]{2,4})\$"));
    
    if (!valid || pwd.size() == 0) {
        MessageBox("Invalid Email or Password", "Error");
        return;
    }
    
    std::size_t found = email.find("@");
    name = email.substr(0,found);
    
    CCLOG("Name of the user :: %s",name.c_str());
    
    UserDefault::getInstance()->setIntegerForKey("Details_Fetched_From", 3);
    
    LogSuccess::LoginSuccessFrom();
    
}

void EmailLoginPopUp::addKeyBoardInput() {
 
    emailField = cocos2d::TextFieldTTF::textFieldWithPlaceHolder("Click To Type Email",
                                                                cocos2d::Size(screenSize.x,50),TextHAlignment::CENTER , "Arial", 36);
    
    emailField->setTag(EMAIL);
    emailField->setPosition(origin.x + screenSize.x * 0.5 ,origin.y + screenSize.y * 0.8);
    emailField->setColorSpaceHolder(Color3B::BLACK);
    emailField->setDelegate(this);

    emailField->setColor(Color3B(255, 0, 0));
    
    this->addChild(emailField,1);
    
    emailField->attachWithIME();
    
    
    emailTouchListener = EventListenerTouchOneByOne::create();
    
    emailTouchListener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event * event) -> bool {
        try {
            
            auto emailField = dynamic_cast<TextFieldTTF *>(event->getCurrentTarget());
            
            Vec2 touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
            
            float minX = 0;
            float maxX = emailField->getContentSize().width;
            float minY = (emailField->getPosition().y - (emailField->getContentSize().height/2));
            float maxY = (emailField->getPosition().y + (emailField->getContentSize().height/2));
            
//            CCLOG("MinX :: %f MaxX :: %f MinY :: %f MaxY :: %f",minX,maxX,minY,maxY);
//            CCLOG("Touch Point X :: %f Y :: %f",touchPoint.x,touchPoint.y);
            
            if (touchPoint.x > minX && touchPoint.x < maxX && touchPoint.y > minY && touchPoint.y < maxY) {
                emailField->attachWithIME();
                emailField->setPlaceHolder("");
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
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(emailTouchListener, emailField);
    
    password = cocos2d::TextFieldTTF::textFieldWithPlaceHolder("Click To Type Password",
                                                                 cocos2d::Size(screenSize.x,50),TextHAlignment::CENTER , "Arial", 36);
    password->setTag(PASSWORD);
    password->setPosition(origin.x + screenSize.x * 0.5 ,origin.y + screenSize.y * 0.67);
    password->setColorSpaceHolder(Color3B::BLACK);
    password->setDelegate(this);
    password->setSecureTextEntry(true);
    password->setColor(Color3B(255,0,0));
    
    this->addChild(password,1);
    
    passwordTouchListener = EventListenerTouchOneByOne::create();
    
    passwordTouchListener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event * event) -> bool {
        try {
            auto password = dynamic_cast<TextFieldTTF *>(event->getCurrentTarget());
            
            Vec2 touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
            
            float minX = 0;
            float maxX = password->getContentSize().width;
            float minY = (password->getPosition().y - (password->getContentSize().height/2));
            float maxY = (password->getPosition().y + (password->getContentSize().height/2));
            
            if (touchPoint.x > minX && touchPoint.x < maxX && touchPoint.y > minY && touchPoint.y < maxY) {
                password->setPlaceHolder("");
                password->attachWithIME();
                CCLOG("Touched in password touch began");
            }
            
            return true;
        }
        catch(std::bad_cast & err){
            return true;
        }
    };
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(passwordTouchListener, password);
    
}

void EmailLoginPopUp::closeEmailLoginPopup(cocos2d::Ref *sender) {
    CCLOG("close email popup selected");
    
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(EmailLoginPopUp::cleanPopUp), 0.05);
    
}

void EmailLoginPopUp::cleanPopUp(float dt) {
    
    closeMenu->removeAllChildren();
    this->removeChild(closeMenu);
    this->removeChild(background);
    this->removeChild(layer);
    
}

bool EmailLoginPopUp::onTextFieldAttachWithIME(TextFieldTTF *sender) {
    
    if (sender->getTag() == EMAIL) {
        CCLOG("In onTextFieldAttachWithIME FROM EMAIL");
    }
    else if (sender->getTag() == PASSWORD) {
        CCLOG("In onTextFieldAttachWithIME FROM PASSWORD");
    }
    
    return TextFieldDelegate::onTextFieldAttachWithIME(sender);
}

bool EmailLoginPopUp::onTextFieldDetachWithIME(TextFieldTTF *sender) {
    CCLOG("In onTextFieldDetachWithIME");
    
    if (sender->getTag() == EMAIL) {
        emailField->setPlaceHolder("Click To Type email");
    }
    else if (sender->getTag() == PASSWORD) {
        password->setPlaceHolder("Click To Type Password");
    }
    
    return TextFieldDelegate::onTextFieldDetachWithIME(sender);
}

bool EmailLoginPopUp::onTextFieldInsertText(TextFieldTTF *sender, const char *text, size_t nLen) {
    CCLOG("In onTextFieldInsertText");
    
    if (sender->getTag() == EMAIL) {
        
    }
    else if (sender->getTag() == PASSWORD) {
        
    }
    
    return TextFieldDelegate::onTextFieldInsertText(sender, text, nLen);
}

bool EmailLoginPopUp::onTextFieldDeleteBackward(TextFieldTTF *sender, const char *delText, size_t nLen) {
    CCLOG("In onTextFieldDeleteBackward");
    
    if (sender->getTag() == EMAIL) {
        
    }
    else if (sender->getTag() == PASSWORD) {
        
    }
    
    return TextFieldDelegate::onTextFieldDeleteBackward(sender, delText, nLen);
}

bool EmailLoginPopUp::onVisit(TextFieldTTF *sender, Renderer *renderer, const Mat4 &transform, uint32_t flags) {
    //CCLOG("In onVisit");
    
    if (sender->getTag() == EMAIL) {
        
    }
    else if (sender->getTag() == PASSWORD) {
        
    }
    
    return TextFieldDelegate::onVisit(sender, renderer, transform, flags);
}








