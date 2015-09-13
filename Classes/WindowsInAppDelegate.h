#ifndef NBG_IN_APP_DELEGATE_WINDOWS
#define NBG_IN_APP_DELEGATE_WINDOWS

#include "cocos2d.h"
#include <vector>

#include "xml.h"
#include "IInAppDelegate.h"
#include "InAppDefinitions.h"

USING_NS_CC;

class CWindowsInAppDelegate: public IInAppDelegate
{
public:		
	CWindowsInAppDelegate();
	~CWindowsInAppDelegate();

	virtual void BuyItem(const std::string item_id);	
private:	
};
#endif // NBG_IN_APP_DELEGATE_WINDOWS
