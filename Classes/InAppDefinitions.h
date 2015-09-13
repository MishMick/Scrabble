#ifndef NBG_INAPP_DEFITIONS
#define NBG_INAPP_DEFITIONS

enum EInAppResult
{
	R_SUCCESS,
	R_FAILED,
	R_DECLINED
};

struct SInAppResult
{
	EInAppResult result;
	std::string id;
};

#endif