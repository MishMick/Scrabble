#ifndef NBG_IN_APP_DELEGATE_ANDROID
#define NBG_IN_APP_DELEGATE_ANDROID

#include "cocos2d.h"
#include <vector>

#include "xml.h"
#include "IInAppDelegate.h"
#include "InAppDefinitions.h"

USING_NS_CC;

class CAndroidInAppDelegate: public IInAppDelegate , public cocos2d::Layer
{
public:		
	CAndroidInAppDelegate();
	~CAndroidInAppDelegate();

	virtual void BuyItem(const std::string item_id);
private:	
};
#endif // NBG_IN_APP_DELEGATE_ANDROID
