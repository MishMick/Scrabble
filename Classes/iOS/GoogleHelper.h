//
//  GoogleHelper.h
//  Scrabble
//
//  Created by Mishal Hemant Shah on 2/15/15.
//
//

#ifndef Scrabble_GoogleHelper_h
#define Scrabble_GoogleHelper_h
#import <Foundation/Foundation.h>
//#import "RootViewController.h"
#include <GoogleOpenSource/GoogleOpenSource.h>
#include <GooglePlus/GooglePlus.h>
#include <stdio.h>
using namespace cocos2d;
using namespace std;
@interface Google :NSObject <GPPSignInDelegate>
{
    UIButton *googlePlusSignInButton;
    GPPSignIn *signIn;
    NSMutableArray *googleContacts;
    NSError *mContactFetchError;
    NSString* _profileId;
    NSString* _profileName;
    bool isLogged;
    bool secondLogin;
    int GoogleLoginSuccess;
    
}

-(string)getGID;
-(string)getGName;
-(string)getGMail;
-(string)getGImageURL;
+ (NSDictionary*)parseURLParams:(NSString *)query;
- (void)signInGoogle;
- (void)finishedWithAuth: (GTMOAuth2Authentication *)auth error: (NSError *) error;
- (bool)isAuth;
@end

@interface NSArray (indexKeyedDictionaryExtension)
- (NSDictionary *)indexKeyedDictionary;
@end

@implementation NSArray (indexKeyedDictionaryExtension)

- (NSDictionary *)indexKeyedDictionary
{
    NSUInteger arrayCount = [self count];
    id arrayObjects[arrayCount], objectKeys[arrayCount];
    
    [self getObjects:arrayObjects range:NSMakeRange(0UL, arrayCount)];
    for(NSUInteger index = 0UL; index < arrayCount; index++) { objectKeys[index] = [NSNumber numberWithUnsignedInteger:index]; }
    
    return([NSDictionary dictionaryWithObjects:arrayObjects forKeys:objectKeys count:arrayCount]);
}
@end

#endif
