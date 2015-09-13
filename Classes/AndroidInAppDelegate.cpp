#include "AndroidInAppDelegate.h"

#include "GameScene.h"
#include "Android/Cached_Android.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "UserLogin.h"



extern "C"
{
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_Cocos2dxActivity_BuyItemSuccess(JNIEnv* env, jobject thiz, jstring test)	
	{
		CGameScene * gameScene = CGameScene::Instance();
		if (gameScene)
		{
			const char *nativeString = env->GetStringUTFChars(test, 0);
			std::string str = nativeString;
			gameScene->BuyItem(str);		
		}	
	}
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_ScrabbleJNIHelper_CallCocosSessionExist(JNIEnv*  env, jobject thiz)
    {
      CCLOG("JAVA TO C++ CALLED!");
        
        CacheLoginSceneAndroid::getInstance()->decideScene(0.1);
    
//          auto scene = UserLoginScene::createUserScene();
//        Director::getInstance()->getScheduler()->performFunctionInCocosThread([&](){
//        Director::getInstance()->replaceScene(scene);});
        //Director::getInstance()->runWithScene(scene);
    }
    
};
#endif


//////////////////////////////////////////////////////////////////////////
CAndroidInAppDelegate::CAndroidInAppDelegate()
{
    
}

//////////////////////////////////////////////////////////////////////////
CAndroidInAppDelegate::~CAndroidInAppDelegate()
{

}

//////////////////////////////////////////////////////////////////////////
void CAndroidInAppDelegate::BuyItem(const std::string item_id)
{	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JavaVM* jvm = JniHelper::getJavaVM();
	int status;
	JNIEnv *env;
	jmethodID mid;
	jobject jobj;

	bool isAttached = false;
	bool returnValue = false;

	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t, "com/sky7/thimbles/InApp" 
		,"buyItem" 
		,"(Ljava/lang/String;)Z"))
	{		
		jstring StringArg1 = t.env->NewStringUTF(item_id.c_str());
		t.env->CallStaticVoidMethod(t.classID,t.methodID, StringArg1);
	}
#endif
}