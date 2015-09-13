//
//  SearchPopUp.h
//  FI
//
//  Created by Parth Parikh on 3/3/15.
//
//

#ifndef __FI__SearchPopUp__
#define __FI__SearchPopUp__

#include <stdio.h>
#include "cocos2d.h"

class SearchUser;

class SearchPopUp  : public cocos2d::Layer, public cocos2d::TextFieldDelegate{
    
public:
    
    static SearchPopUp* instance;
    
    static SearchPopUp* getInstance();
    
    cocos2d::Vec2 origin;
    cocos2d::Vec2 screenSize;
    
    cocos2d::TextFieldTTF* searchText;
    
    cocos2d::EventListenerTouchOneByOne *searchTouchListener;
    
    cocos2d::Sprite *background;
    
    cocos2d::LayerColor *layer;
    
    cocos2d::MenuItemImage *searchBtn;
    cocos2d::Menu *searchMenu;
    
    cocos2d::MenuItemImage *closeBtn;
    cocos2d::Menu *closeMenu;
    
    cocos2d::Vector<SearchUser*> searchUsers;
    
    static SearchPopUp* create();
    bool init();
    
    void addKeyBoardInput();
    
    virtual bool onTextFieldAttachWithIME(cocos2d::TextFieldTTF *sender) override;
    
    virtual bool onTextFieldDetachWithIME(cocos2d::TextFieldTTF *sender) override;
    
    virtual bool onTextFieldInsertText(cocos2d::TextFieldTTF *sender, const char *text, size_t nLen) override;
    
    virtual bool onTextFieldDeleteBackward(cocos2d::TextFieldTTF *sender, const char *delText, size_t nLen)
    override;
    
    virtual bool onVisit(cocos2d::TextFieldTTF *sender, cocos2d::Renderer *renderer,
                         cocos2d::Mat4 const &transform, uint32_t flags) override;
    
    void searchCallBack(cocos2d::Ref* sender);
    
    void searchResult(std::vector<std::string> searchNames,std::vector<std::string> searchEmails,std::vector<std::string> searchImageUrl,std::vector<std::string> searchUserId);
    
    void closeSearchLoginPopup(cocos2d::Ref* sender);
    
    void cleanPopUp(float dt);
    
};

#endif /* defined(__FI__SearchPopUp__) */
