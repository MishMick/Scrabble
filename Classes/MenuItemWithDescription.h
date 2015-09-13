#ifndef NBG_GAME_MENU_ITEM_WITH_DESCRIPTION
#define NBG_GAME_MENU_ITEM_WITH_DESCRIPTION

/*
copyright 2014 New Bridge Games
email: vadim@newbridgegames.com
www: www.newbridgegames.com
*/

#include "cocos2d.h"
#include <vector>
#include <deque>
#include <map>
#include "DescriptionWindow.h"

USING_NS_CC;

class CMenuItemWithDesciption : public CCMenuItemSprite
{
public:
	CMenuItemWithDesciption(){m_IsLoadData = false;}
	virtual ~CMenuItemWithDesciption(){}
		
	static float TIME_FOR_SHOW_DESCRIPTION;

	/** Creates an CMenuItemWithDesciption. */
	static CMenuItemWithDesciption* create();
	/** creates a menu item with a normal and selected image*/
	static CMenuItemWithDesciption* create(const std::string& normalImage, const std::string& selectedImage);
	/** creates a menu item with a normal,selected  and disabled image*/
	static CMenuItemWithDesciption* create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage);
	/** creates a menu item with a normal and selected image with target/selector */
	CC_DEPRECATED_ATTRIBUTE static CMenuItemWithDesciption* create(const std::string& normalImage, const std::string& selectedImage, Ref* target, SEL_MenuHandler selector);
	/** creates a menu item with a normal and selected image with a callable object */
	static CMenuItemWithDesciption* create(const std::string&normalImage, const std::string&selectedImage, const ccMenuCallback& callback);

	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	CC_DEPRECATED_ATTRIBUTE static CMenuItemWithDesciption* create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, Ref* target, SEL_MenuHandler selector);
	/** creates a menu item with a normal,selected  and disabled image with a callable object */
	static CMenuItemWithDesciption* create(const std::string&normalImage, const std::string&selectedImage, const std::string&disabledImage, const ccMenuCallback& callback);

	/** sets the sprite frame for the normal image */
	void setNormalSpriteFrame(SpriteFrame* frame);
	/** sets the sprite frame for the selected image */
	void setSelectedSpriteFrame(SpriteFrame* frame);
	/** sets the sprite frame for the disabled image */
	void setDisabledSpriteFrame(SpriteFrame* frame);

	// MY FUNCIONS
	void SetDescription(std::string description);
    void MouseMove(cocos2d::Point pos);
	virtual void update(float delta);
	void AlignDescription(CDescriptionWindow::ALIGN_TYPE align){m_DescriptionWindow->Align(align);}
	CDescriptionWindow* GetDescriptionWindow(){return m_DescriptionWindow;}
	void InstantlyHideDescriptionWindow();
	void InstantlyShowDescriptionWindow();
protected:

	virtual bool init(); 	
    /** initializes a menu item with a normal, selected  and disabled image with target/selector */
    CC_DEPRECATED_ATTRIBUTE bool initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, Ref* target, SEL_MenuHandler selector);
    /** initializes a menu item with a normal, selected  and disabled image with a callable object */
    bool initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback);
private:			
	void LoadData();
	void SetToZeroDescriptionTimer();
	void FillDescriptionTimer();

	CDescriptionWindow* m_DescriptionWindow;
	bool m_IsCursorUnder;
	float m_CursorOverTime;
	bool m_IsLoadData;

	CC_DISALLOW_COPY_AND_ASSIGN(CMenuItemWithDesciption);
};
#endif //NBG_GAME_MENU_ITEM_WITH_DESCRIPTION