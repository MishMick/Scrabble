//
//  JNICPPHelper.h
//  FI
//
//  Created by Mishal Hemant Shah on 4/23/15.
//
//

#ifndef __FI__JNICPPHelper__
#define __FI__JNICPPHelper__

#include <stdio.h>
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#endif

extern "C"
{
	extern void CallJavaLogin();
	//extern void checkFBSession();
}
#endif /* defined(__FI__JNICPPHelper__) */






