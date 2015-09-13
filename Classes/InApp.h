#ifndef NBG_IN_APP
#define NBG_IN_APP

#include "cocos2d.h"
#include <vector>

#include "xml.h"

#include "IInAppDelegate.h"
#include "InAppDefinitions.h"

USING_NS_CC;

class CInApp
{
public:	
	static CInApp * Instance()
	{
		static CInApp inApp;
		return &inApp;
	}
	~CInApp();	
	void SetDelegate(IInAppDelegate* iapDelegate)
	{
		m_Delegate = iapDelegate;		
	}
	void BuyItem(const std::string item_id);	
private:	
	IInAppDelegate * m_Delegate;
	CInApp();
};

#endif // NBG_IN_APP
