//
//  GoogleHelper.m
//  Scrabble
//
//  Created by Mishal Hemant Shah on 2/15/15.
//
//

#include <stdlib.h>     
#include "GoogleHelper.h"
#include "UserLogin.h"
#include "Cached.h"
#include "LoginSuccess.h"
using namespace std;
static NSString* emailID;
static NSString* displayName;
static NSString* picURL;
static NSString* UID;
static NSString* gender;
static string g_displayName,g_ID,g_emailID,g_iURL;
string g_name,g_Id,g_email,g_iurl;
@implementation Google
-(void) signInGoogle 
{
    secondLogin = false;
    NSLog(@"Sign in with google called!");
    [GPPSignIn sharedInstance].shouldFetchGooglePlusUser = YES;
    [GPPSignIn sharedInstance].shouldFetchGoogleUserEmail = YES;
    [GPPSignIn sharedInstance].clientID = @"105131900840-qhfg9vsjrskp99i7ot8esaovilpfe113.apps.googleusercontent.com";
    [GPPSignIn sharedInstance].scopes = [NSArray arrayWithObjects:@"https://www.googleapis.com/auth/plus.login", nil];
    //signIn.scopes = [NSArray arrayWithObject:@"https://www.googleapis.com/auth/plus.profile.emails.read"];
    //signIn.actions = [NSArray arrayWithObjects:@"http://schemas.google.com/ListenActivity",nil];
    NSLog(@"Delegated!");
    [GPPSignIn sharedInstance].delegate = self;
    [[GPPSignIn sharedInstance] authenticate];
}

- (void)finishedWithAuth: (GTMOAuth2Authentication *)auth error: (NSError *) error
{
    if (error)
    {
        NSLog(@"Received error %@ and auth object %@",error, auth);
        isLogged=false;
        
    }
    else
    {
        GTLQueryPlus *query = [GTLQueryPlus queryForPeopleGetWithUserId:@"me"];
        
        //NSLog(@"email %@ ", [NSString stringWithFormat:@"Email: %@",[GPPSignIn sharedInstance].authentication.userEmail]);
        
        // 1. Create a |GTLServicePlus| instance to send a request to Google+.
        GTLServicePlus* plusService = [[GTLServicePlus alloc] init] ;
        plusService.retryEnabled = YES;
        
        // 2. Set a valid |GTMOAuth2Authentication| object as the authorizer.
        [plusService setAuthorizer:[GPPSignIn sharedInstance].authentication];
        
        // 3. Use the "v1" version of the Google+ API.*
        plusService.apiVersion = @"v1";
        [plusService executeQuery:query
                completionHandler:^(GTLServiceTicket *ticket,
                                    GTLPlusPerson *person,
                                    NSError *error) {
                    if (error)
                    {
                        CCLOG("show login error popup");
                        //Handle Error
                    }
                    
                    else
                    {
                        emailID=[GPPSignIn sharedInstance].authentication.userEmail;
                        const char *Email = [emailID UTF8String];
                        string g_EmailID(Email);
                        
                        UID=person.identifier;
                        const char *ID = [UID UTF8String];
                        string g_ID(ID);
                        
                        displayName=[person.name.givenName stringByAppendingFormat:@" %@", person.name.familyName];
                        const char *display = [displayName UTF8String];
                        string g_displayName(display);
                        
                        // gender= person.gender;
                        // const char *gen = [gender UTF8String];
                        
                        picURL=person.url;
                        const char *URL = [picURL UTF8String];
                        string g_iURL(URL);
                        
                        
                        
                        g_name = g_displayName;
                        g_email =  g_EmailID;
                        g_Id = g_ID;
                        g_iurl=g_iURL;
                        
                        NSLog(@"Google name: %s",g_name.c_str());
                        NSLog(@"Google email: %s",g_email.c_str());
                        NSLog(@"Google ID: %s",g_Id.c_str());
                        NSLog(@"Google URL: %s",g_iurl.c_str());
                        
                        GoogleLoginSuccess=1;
                        UserDefault::getInstance()->setIntegerForKey("Login_Count", 1);
                        UserDefault::getInstance()->setIntegerForKey("Details_Fetched_From", 2);
                        //UserDefault::getInstance()->setIntegerForKey("Login_From", 2);
                        LogSuccess::LoginSuccessFrom();
                        
                    }
                }];
        
        isLogged=true;
        
    }
}
-(string)getGID
{
    return g_Id;
}
-(string)getGName
{
    return g_name;
}
-(string)getGMail
{
    return g_email;
}
-(string)getGImageURL
{
    return g_iurl;
}

-(bool)isAuth
{
    secondLogin = true;
    UserDefault::getInstance()->setIntegerForKey("Login_Count", 2);
    [GPPSignIn sharedInstance].shouldFetchGooglePlusUser = YES;
    [GPPSignIn sharedInstance].shouldFetchGoogleUserEmail = YES;
    [GPPSignIn sharedInstance].clientID = @"105131900840-qhfg9vsjrskp99i7ot8esaovilpfe113.apps.googleusercontent.com";
    [GPPSignIn sharedInstance].scopes = [NSArray arrayWithObjects:@"https://www.googleapis.com/auth/plus.login", nil];
    [GPPSignIn sharedInstance].delegate = self;
    //[[GPPSignIn sharedInstance] signOut];
    NSLog(@"Is Auth here!");
    return [[GPPSignIn sharedInstance] trySilentAuthentication];
}
@end
