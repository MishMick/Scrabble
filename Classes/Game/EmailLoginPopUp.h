//
//  EmailLoginPopUp.h
//  FI
//
//  Created by Parth Parikh on 2/27/15.
//
//

#ifndef __FI__EmailLoginPopUp__
#define __FI__EmailLoginPopUp__

#include <stdio.h>
#include "cocos2d.h"

class EmailLoginPopUp : public cocos2d::Layer, public cocos2d::TextFieldDelegate {
  
public:
    
    cocos2d::Vec2 origin;
    cocos2d::Vec2 screenSize;
    
    cocos2d::TextFieldTTF* emailField;
    cocos2d::TextFieldTTF* password;

    cocos2d::EventListenerTouchOneByOne *emailTouchListener;
    cocos2d::EventListenerTouchOneByOne *passwordTouchListener;
    
    cocos2d::Sprite *background;
    
    cocos2d::LayerColor *layer;
    
    cocos2d::MenuItemImage *loginBtn;
    cocos2d::Menu *loginMenu;
    
    cocos2d::MenuItemImage *closeBtn;
    cocos2d::Menu *closeMenu;
    
    static std::string email;
    static std::string pwd;
    static std::string name;
    
    static EmailLoginPopUp* create();
    bool init();
    
    void addKeyBoardInput();
    
    virtual bool onTextFieldAttachWithIME(cocos2d::TextFieldTTF *sender) override;
    
    virtual bool onTextFieldDetachWithIME(cocos2d::TextFieldTTF *sender) override;
    
    virtual bool onTextFieldInsertText(cocos2d::TextFieldTTF *sender, const char *text, size_t nLen) override;
    
    virtual bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF *sender, const char *delText, size_t nLen)
    override;
    
    virtual bool onVisit(cocos2d::TextFieldTTF *sender, cocos2d::Renderer *renderer,
                         cocos2d::Mat4 const &transform, uint32_t flags) override;
    
    void loginCallBack(cocos2d::Ref* sender);
    
    void closeEmailLoginPopup(cocos2d::Ref* sender);
    
    void cleanPopUp(float dt);
};

#endif /* defined(__FI__EmailLoginPopUp__) */
