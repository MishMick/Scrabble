#ifndef NBG_IN_APP_DELEGATE_INTERFACE
#define NBG_IN_APP_DELEGATE_INTERFACE

#include "cocos2d.h"
#include <vector>

#include "xml.h"
#include "InAppDefinitions.h"

USING_NS_CC;

class IInAppDelegate
{
public:			
	virtual void BuyItem(const std::string item_id) = 0;	
private:	
};

#endif // NBG_IN_APP_DELEGATE_INTERFACE
