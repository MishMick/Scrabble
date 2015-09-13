//
//  ChatLayer.cpp
//  Scrabble
//
//  Created by Parth Parikh on 2/12/15.
//
//

#include "ChatLayer.h"
#include "cocos-ext.h"
#include "GameScene.h"
#include "Player.h"

#define LINE_HEIGHT 28

using namespace cocos2d;

#define LOWER_LIMIT 0

#define CHARACTER_IN_LINE 23

ChatLayer* ChatLayer::instance = NULL;

ChatLayer* ChatLayer::getInstance() {
    if (instance == NULL) {
        return NULL;
    }
    return instance;
}

Scene* ChatLayer::createScene()
{
    auto scene = Scene::create();
    
    auto layer = ChatLayer::create();
    scene->addChild(layer);
    
    return scene;
}

bool ChatLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    layerLoaded = false;
    
    instance = this;
    
    CGameScene::Instance()->enableGameScene(false);
    
    numberOfLines = 0;
    numberOfMessages = 0;
    
    origin = Director::getInstance()->getVisibleOrigin();
    screenSize = Director::getInstance()->getWinSize();
    
    chatBackground = Sprite::create("chatBck.png");
    chatBackground->setPosition(Vec2(874,384));
    addChild(chatBackground);
    
    chatNode = Node::create();
    chatNode->setPosition(Vec2(0, 0));
    chatBackground->addChild(chatNode);
    
    sendbtn = MenuItemImage::create("button.png","button.png",CC_CALLBACK_1(ChatLayer::sendMessage, this));
    sendbtn->setEnabled(false);
    sendbtn->setOpacity(0);
    sendbtn->setPosition(Vec2(999,500));
    sendbtn->setScale(0.5);
    sendbtn->setTag(1);
    
    sendMenu = Menu::create(sendbtn,NULL);
    sendMenu->setPosition(Vec2::ZERO);
    addChild(sendMenu);
    
    m_AnimSprite = Sprite::create("001.png");//path+
    m_AnimSprite->setPosition(900,484);
    m_AnimSprite->setScale(1);
    addChild(m_AnimSprite);
    
    touchDelegate = EventListenerTouchOneByOne::create();
    touchDelegate->onTouchBegan = std::bind(&ChatLayer::TouchBegan, this, std::placeholders::_1, std::placeholders::_2);
    touchDelegate->onTouchMoved = std::bind(&ChatLayer::TouchMoved, this, std::placeholders::_1, std::placeholders::_2);
    touchDelegate->onTouchCancelled = std::bind(&ChatLayer::TouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
    touchDelegate->onTouchEnded = std::bind(&ChatLayer::TouchEnded, this, std::placeholders::_1, std::placeholders::_2);
        
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchDelegate, -257);
    
    startWaitingAnim();
    
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(ChatLayer::getChats), 0.1);
    
    return true;
}

bool ChatLayer::TouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    
    oldPoints = touch->getLocationInView();
    newPoints = touch->getLocationInView();
    
    CCLOG("in chat touch began");
    
    return true;
}

void ChatLayer::TouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {
    //upper limit 0
    //lower limit need to calculate
    
    int upperLimit = 0;

    upperLimit = (((numberOfLines*LINE_HEIGHT) + (numberOfMessages*LINE_HEIGHT/2)) + 68) * (screenSize.y/768);
    
    log("Upper limit :: %d",upperLimit);
    
    newPoints = touch->getLocationInView();
    
    if (chatNode->getPositionY() + (oldPoints.y - newPoints.y) > LOWER_LIMIT && (chatNode->getPositionY() + (oldPoints.y - newPoints.y) < upperLimit - screenSize.y)) {
        chatNode->setPosition(Vec2(chatNode->getPositionX(),(chatNode->getPositionY() + (oldPoints.y - newPoints.y))));
    }
    
    CCLOG("Chat Node Position :: %f %f",chatNode->getPositionX(),chatNode->getPositionY());
    
    oldPoints = newPoints;
}

void ChatLayer::TouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    oldPoints = newPoints;
}

void ChatLayer::TouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
    
}

void ChatLayer::getChats(float dt) {
    CGameScene::Instance()->GetPlayer()->getRoomChat();
}

void ChatLayer::sendMessage(cocos2d::Ref *Sender) {
    CCLOG("send message called text :: %s",textField->getString().c_str());
    CGameScene::Instance()->GetPlayer()->sendChatMessage(textField->getString());
    textField->setString("");
    textField->detachWithIME();
}

void ChatLayer::startWaitingAnim() {
    Vector<SpriteFrame*> animFrames(12);
    std::string path = "";//animation/waiting/
    for(int i = 1;i<=12;i++)
    {
        std::string framePath;
        if(i>9)
        {
            framePath = path + "0" + StringUtils::toString(i) + ".png";
        }
        else
        {
            framePath = path + "00" + StringUtils::toString(i) + ".png";
        }
        auto frame = SpriteFrame::create(framePath,Rect(0,0,64,64));
        animFrames.pushBack(frame);
    }
    auto animation = CCAnimation::createWithSpriteFrames(animFrames,0.1);
    animation->setLoops(-1);
    m_AnimAction = CCAnimate::create(animation);
    m_AnimAction->setTag(100);
    m_AnimSprite->runAction(m_AnimAction);
}

void ChatLayer::stopWaitingAnim() {
    m_AnimSprite->setOpacity(0);
    m_AnimSprite->stopAllActionsByTag(100);
    
    addKeyboardInput();
}

void ChatLayer::addKeyboardInput() {
    
    
    // Create a text field
    textField = cocos2d::TextFieldTTF::textFieldWithPlaceHolder("Click here to type",
                                                                cocos2d::Size(200,100),TextHAlignment::LEFT , "Arial", 24);
    
    //this->getBoundingBox().getMidX(),
    //    this->getBoundingBox().getMaxY() - 20
    textField->setPosition(850,384);
    textField->setColorSpaceHolder(Color3B::WHITE);
    textField->setDelegate(this);
    
    this->addChild(textField,1000);
    
    textField->attachWithIME();
    
    // Add a touch handler to our textfield that will show a keyboard when touched
    touchListener = EventListenerTouchOneByOne::create();

    touchListener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event * event) -> bool {
        try {
            
            // Show the on screen keyboard
            auto textField = dynamic_cast<TextFieldTTF *>(event->getCurrentTarget());
            
            Vec2 touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());
            
            if (touchPoint.x > 700 && touchPoint.x < 1024 && touchPoint.y > 0 && touchPoint.y < 100) {
                textField->setPlaceHolder("Text Here");
                textField->attachWithIME();
            }
            return true;
        }
        catch(std::bad_cast & err){
            return true;
        }
    };
    
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, textField);
    
    sendbtn->setEnabled(true);
    sendbtn->setOpacity(255);
    
    layerLoaded = true;

}

ChatLayer::ChatLayer() {
    
}

ChatLayer::~ChatLayer() {
    
}

void ChatLayer::setChatVector(std::vector<std::string> textVector ,
                   std::vector<std::string> senderNameVector,std::vector<int> senderId) {
    
    std::reverse(textVector.begin(), textVector.end());
    std::reverse(senderNameVector.begin(), senderNameVector.end());
    std::reverse(senderId.begin(), senderId.end());
    
    this->textVector = textVector;
    this->senderNameVector = senderNameVector;
    this->senderId = senderId;
    int i = 0;

    CCLOG("__________ Message __________");
    
    for (std::vector<std::string>::iterator it = this->textVector.begin() ; it != this->textVector.end(); ++it) {
        
        std::string message;
        LabelTTF* searchLabel = NULL;
        int count = 0;
        
        if (this->senderId.at(i) == UserDefault::getInstance()->getIntegerForKey("Player_ID")) {
            message = "Me: " + this->textVector.at(i);
            
            count = (message.length()/CHARACTER_IN_LINE)+1;
            searchLabel = LabelTTF::create(message.c_str(), "helvetica", 24,Size(275,LINE_HEIGHT*count), TextHAlignment::LEFT,TextVAlignment::TOP);
            searchLabel->setAnchorPoint(Point(0,1));
            searchLabel->setPosition(10,700 - (numberOfLines * LINE_HEIGHT) - (numberOfMessages * LINE_HEIGHT/2));
        }
        else {
            message = this->senderNameVector.at(i) + ": "+ this->textVector.at(i);
            
            count = (message.length()/CHARACTER_IN_LINE)+1;
            searchLabel = LabelTTF::create(message.c_str(), "helvetica", 24,Size(275,LINE_HEIGHT*count), TextHAlignment::LEFT,TextVAlignment::TOP);
            searchLabel->setAnchorPoint(Point(0,1));
            searchLabel->setPosition(10,700 - (numberOfLines * LINE_HEIGHT) - (numberOfMessages * LINE_HEIGHT/2));
        }
        
        searchLabel->setColor(Color3B(255, 0, 0));
        chatNode->addChild(searchLabel);
        
        numberOfLines = numberOfLines + count;
        numberOfMessages = numberOfMessages + 1;
        
        CCLOG("Count :: %d Message :: %s",count,message.c_str());

        i++;
    }
    
    stopWaitingAnim();
}

void ChatLayer::addMessage(std::string message) {
    std::string msg = "Me: " + message;
    LabelTTF* searchLabel = NULL;
    
    int count = (msg.length()/CHARACTER_IN_LINE)+1;
    
    searchLabel = LabelTTF::create(msg.c_str(), "helvetica", 24,Size(275,LINE_HEIGHT*count), TextHAlignment::LEFT,TextVAlignment::TOP);
    searchLabel->setAnchorPoint(Point(0,1));
    searchLabel->setPosition(10,700 - (numberOfLines * LINE_HEIGHT) - (numberOfMessages * LINE_HEIGHT/2));
    
    searchLabel->setColor(Color3B(255, 0, 0));
    chatNode->addChild(searchLabel);
    
    numberOfLines = numberOfLines + count;
    numberOfMessages = numberOfMessages + 1;
}

void ChatLayer::addMessageFromOpponent(std::string message) {
    std::string msg = "Parth: " + message;
    LabelTTF* searchLabel = NULL;
    
    int count = (msg.length()/CHARACTER_IN_LINE)+1;
    
    searchLabel = LabelTTF::create(msg.c_str(), "helvetica", 24,Size(275,LINE_HEIGHT*count), TextHAlignment::LEFT,TextVAlignment::TOP);
    searchLabel->setAnchorPoint(Point(0,1));
    searchLabel->setPosition(10,700 - (numberOfLines * LINE_HEIGHT) - (numberOfMessages * LINE_HEIGHT/2));
    
    searchLabel->setColor(Color3B(255, 0, 0));
    chatNode->addChild(searchLabel);
    
    numberOfLines = numberOfLines + count;
    numberOfMessages = numberOfMessages + 1;
}

void ChatLayer::cleanAll() {
    
    CGameScene::Instance()->enableGameScene(true);
    
    if (textField) {
        textField->detachWithIME();
    }
    
    if (touchDelegate) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(touchDelegate);
    }
    
    if (touchListener) {
        this->_eventDispatcher->removeEventListener(touchListener);
    }
    
}

void ChatLayer::keyboardWillShow(IMEKeyboardNotificationInfo& info) {
    log("KeyBoard Will Show");
}

void ChatLayer::keyboardDidShow(IMEKeyboardNotificationInfo& info) {
    log("KeyBoard Did Show");
}

void ChatLayer::keyboardWillHide(IMEKeyboardNotificationInfo& info) {
    sendbtn->setPosition(Vec2(999,50));
    textField->setPosition(850,50);
    textField->setPlaceHolder("Click here to type");
    log("KeyBoard Will Hide");
}

void ChatLayer::keyboardDidHide(IMEKeyboardNotificationInfo& info) {
    sendbtn->setPosition(Vec2(999,50));
    textField->setPosition(850,50);
    textField->setPlaceHolder("Click here to type");
    log("KeyBoard Did Hide");
}

bool ChatLayer::onTextFieldAttachWithIME(TextFieldTTF *sender) {
    sendbtn->setPosition(Vec2(999,384));
    textField->setPosition(Vec2(850,384));
    CCLOG("In onTextFieldAttachWithIME");
    return TextFieldDelegate::onTextFieldAttachWithIME(sender);
}

bool ChatLayer::onTextFieldDetachWithIME(TextFieldTTF *sender) {
    sendbtn->setPosition(Vec2(999,50));
    textField->setPosition(850,50);
    textField->setPlaceHolder("Click here to type");
    CCLOG("In onTextFieldDetachWithIME");
    return TextFieldDelegate::onTextFieldDetachWithIME(sender);
}

bool ChatLayer::onTextFieldInsertText(TextFieldTTF *sender, const char *text, size_t nLen) {
    CCLOG("In onTextFieldInsertText");
    CCLOG("String :: %s",sender->getString().c_str());
    
    return TextFieldDelegate::onTextFieldInsertText(sender, text, nLen);
}

bool ChatLayer::onTextFieldDeleteBackward(TextFieldTTF *sender, const char *delText, size_t nLen) {
    CCLOG("In onTextFieldDeleteBackward");
    return TextFieldDelegate::onTextFieldDeleteBackward(sender, delText, nLen);
}

bool ChatLayer::onVisit(TextFieldTTF *sender, Renderer *renderer, const Mat4 &transform, uint32_t flags) {
    //CCLOG("In onVisit");
    return TextFieldDelegate::onVisit(sender, renderer, transform, flags);
}


