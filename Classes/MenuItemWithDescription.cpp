#include "MenuItemWithDescription.h"
#include "xml.h"

#include <algorithm>

float CMenuItemWithDesciption::TIME_FOR_SHOW_DESCRIPTION = 1.0f;

//////////////////////////////////////////////////////////////////////////
void CMenuItemWithDesciption::update(float fDelta)
{
	CCMenuItemSprite::update(fDelta);

	if(m_IsCursorUnder)
	{
		m_CursorOverTime += fDelta;
		if(m_CursorOverTime>= TIME_FOR_SHOW_DESCRIPTION && m_DescriptionWindow->IsShow() == false)
		{
			m_DescriptionWindow->Show();
		}
	}
	else if(m_DescriptionWindow->IsShow())
	{
		m_DescriptionWindow->Hide();
	}
}

//////////////////////////////////////////////////////////////////////////
void CMenuItemWithDesciption::MouseMove(Point pos)
{
	if(isVisible() && getNormalImage()->getBoundingBox().containsPoint(convertToNodeSpace(pos)))
	{
		m_IsCursorUnder = true;
	}
	else
	{
		m_IsCursorUnder = false;
		m_CursorOverTime = 0.0f;
	}
}

//////////////////////////////////////////////////////////////////////////
CMenuItemWithDesciption* CMenuItemWithDesciption::create()
{
	CMenuItemWithDesciption *ret = new CMenuItemWithDesciption();
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool CMenuItemWithDesciption::init(void)
{
	LoadData();

	return initWithNormalImage("", "", "", (const ccMenuCallback&)nullptr);
}

void CMenuItemWithDesciption::LoadData()
{
	if(m_IsLoadData)
	{
		return;
	}
	m_IsLoadData = true;
	m_DescriptionWindow = CDescriptionWindow::create();
	m_DescriptionWindow->SetTarget(this);
	
	m_CursorOverTime = 0.0f;
	m_IsCursorUnder = false;
}

CMenuItemWithDesciption * CMenuItemWithDesciption::create(const std::string& normalImage, const std::string& selectedImage)
{
	return CMenuItemWithDesciption::create(normalImage, selectedImage, "", (const ccMenuCallback&)nullptr);
}

// XXX deprecated
CMenuItemWithDesciption * CMenuItemWithDesciption::create(const std::string& normalImage, const std::string& selectedImage, Ref* target, SEL_MenuHandler selector)
{
	return CMenuItemWithDesciption::create(normalImage, selectedImage, "", target, selector);
}

CMenuItemWithDesciption * CMenuItemWithDesciption::create(const std::string& normalImage, const std::string& selectedImage, const ccMenuCallback& callback)
{
	return CMenuItemWithDesciption::create(normalImage, selectedImage, "", callback);
}

// XXX deprecated
CMenuItemWithDesciption * CMenuItemWithDesciption::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, Ref* target, SEL_MenuHandler selector)
{
	CMenuItemWithDesciption *ret = new CMenuItemWithDesciption();
	if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, target, selector))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

CMenuItemWithDesciption * CMenuItemWithDesciption::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
	CMenuItemWithDesciption *ret = new CMenuItemWithDesciption();
	if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, callback))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

CMenuItemWithDesciption * CMenuItemWithDesciption::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage)
{
	CMenuItemWithDesciption *ret = new CMenuItemWithDesciption();
	if (ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, (const ccMenuCallback&)nullptr))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

// XXX: deprecated
bool CMenuItemWithDesciption::initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, Ref* target, SEL_MenuHandler selector)
{
	_target = target;
	CC_SAFE_RETAIN(_target);
	return initWithNormalImage(normalImage, selectedImage, disabledImage, std::bind(selector,target, std::placeholders::_1) );
}
bool CMenuItemWithDesciption::initWithNormalImage(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
	Node *normalSprite = nullptr;
	Node *selectedSprite = nullptr;
	Node *disabledSprite = nullptr;

	LoadData();

	if (normalImage.size() >0)
	{
		normalSprite = Sprite::create(normalImage);
	}

	if (selectedImage.size() >0)
	{
		selectedSprite = Sprite::create(selectedImage);
	}

	if(disabledImage.size() >0)
	{
		disabledSprite = Sprite::create(disabledImage);
	}
	return initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback);
}

//
// Setter of sprite frames
//
void CMenuItemWithDesciption::setNormalSpriteFrame(SpriteFrame * frame)
{
	setNormalImage(Sprite::createWithSpriteFrame(frame));
}

void CMenuItemWithDesciption::setSelectedSpriteFrame(SpriteFrame * frame)
{
	setSelectedImage(Sprite::createWithSpriteFrame(frame));
}

void CMenuItemWithDesciption::setDisabledSpriteFrame(SpriteFrame * frame)
{
	setDisabledImage(Sprite::createWithSpriteFrame(frame));
}

//////////////////////////////////////////////////////////////////////////
void CMenuItemWithDesciption::SetDescription(std::string description)
{
	m_DescriptionWindow->SetDesciptionString(description);
}

//////////////////////////////////////////////////////////////////////////
void CMenuItemWithDesciption::InstantlyHideDescriptionWindow()
{
	SetToZeroDescriptionTimer();
	m_DescriptionWindow->HideInstantly();
}

//////////////////////////////////////////////////////////////////////////
void CMenuItemWithDesciption::InstantlyShowDescriptionWindow()
{
	FillDescriptionTimer();
	m_DescriptionWindow->ShowInstantly();
}

//////////////////////////////////////////////////////////////////////////
void CMenuItemWithDesciption::SetToZeroDescriptionTimer()
{
	m_IsCursorUnder = false;
	m_CursorOverTime = 0.0f;
}

//////////////////////////////////////////////////////////////////////////
void CMenuItemWithDesciption::FillDescriptionTimer()
{
	m_IsCursorUnder = true;
	m_CursorOverTime = TIME_FOR_SHOW_DESCRIPTION;
}