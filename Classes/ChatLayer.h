//
//  ChatLayer.h
//  Scrabble
//
//  Created by Parth Parikh on 2/12/15.
//
//

#ifndef __Scrabble__ChatLayer__
#define __Scrabble__ChatLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>

class ChatLayer : public cocos2d::Layer , public cocos2d::TextFieldDelegate {
    
public:
    
    ChatLayer();
    ~ChatLayer();
    
    cocos2d::EventListenerTouchOneByOne* touchDelegate;
    
    bool layerLoaded;
    
    cocos2d::Vec2 origin;
    cocos2d::Vec2 screenSize;
    
    int numberOfMessages;
    int numberOfLines;
    
    cocos2d::Vec2 oldPoints;
    cocos2d::Vec2 newPoints;
    
    cocos2d::TextFieldTTF* textField;
    
    cocos2d::Sprite *chatBackground;
    cocos2d::Sprite *m_AnimSprite;
    cocos2d::CCAnimate *m_AnimAction;
    
    cocos2d::Node *chatNode;
    
    cocos2d::MenuItemImage *sendbtn;
    cocos2d::Menu *sendMenu;

    std::vector<std::string> textVector;
    std::vector<std::string> senderNameVector;
    std::vector<int> senderId;
    static ChatLayer *instance;
    
    static ChatLayer* getInstance();
    
    cocos2d::EventListenerTouchOneByOne *touchListener;
    
    void stopListning(float dt);
    
    virtual void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info);
    
    virtual void keyboardDidShow(cocos2d::IMEKeyboardNotificationInfo& info);
    
    virtual void keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info);
    
    virtual void keyboardDidHide(cocos2d::IMEKeyboardNotificationInfo& info);
    
    virtual bool onTextFieldAttachWithIME(cocos2d::TextFieldTTF *sender) override;
    
    virtual bool onTextFieldDetachWithIME(cocos2d::TextFieldTTF *sender) override;
    
    virtual bool onTextFieldInsertText(cocos2d::TextFieldTTF *sender, const char *text, size_t nLen) override;
    
    virtual bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF *sender, const char *delText, size_t nLen)
    override;
    
    virtual bool onVisit(cocos2d::TextFieldTTF *sender, cocos2d::Renderer *renderer,
                         cocos2d::Mat4 const &transform, uint32_t flags) override;
    
    bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void TouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void startWaitingAnim();
    void stopWaitingAnim();
    
    void addMessageFromOpponent(std::string message);
    void addMessage(std::string message);
    
    void addKeyboardInput();
    
    void getChats(float dt);
    
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void sendMessage(cocos2d::Ref *Sender);
    
    void cleanAll();
    
    void setChatVector(std::vector<std::string> textVector ,
                       std::vector<std::string> senderNameVector,std::vector<int> senderId);
    
    CREATE_FUNC(ChatLayer);
    
};

#endif /* defined(__Scrabble__ChatLayer__) */
