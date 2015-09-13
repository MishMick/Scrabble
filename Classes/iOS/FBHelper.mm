//
//  Social_Login.m
//  Scrabble
//
//  Created by Mishal Hemant Shah on 2/15/15.
//
//

#include <stdlib.h>     /* srand, rand */
#include "FBHelper.h"
#include "UserLogin.h"
#include "LoginSuccess.h"
#import <FacebookSDK/FacebookSDK.h>
USING_NS_CC;
using namespace std;
static string fN,lN,fID,eM,iURL;
string fn,ln,fId,email,iurl;
// v1 in the range 0 to 99
static bool ms_bIsLoggedIn;
static NSString *ms_uPlayerFBID;
static NSString *ms_nsstrFirstName;
bool Facebook::IsLoggedIn()
{
    return ms_bIsLoggedIn;
}

void Facebook::SetLoggedIn(bool bLoggedIn)
{
    ms_bIsLoggedIn = bLoggedIn;
}

void Facebook::CreateNewSession()
{
    FBSession* session = [[FBSession alloc] init];
    [FBSession setActiveSession: session];
    log("CreateNewSession called! %d",1);
}

void Facebook::OpenSession(void (*callback1)(bool))
{
    log("Open Session called!");
    NSArray *permissions = [[NSArray alloc] initWithObjects: @"email", @"user_friends",nil];
    [FBSession openActiveSessionWithPermissions:permissions allowLoginUI:false completionHandler:^(FBSession *session, FBSessionState status, NSError *error)
     // Attempt to open the session. If the session is not open, show the user the Facebook login UX
     {
         // Did something go wrong during login? I.e. did the user cancel?
         log("Going into if-else part!");
         if (status == FBSessionStateClosedLoginFailed || status == FBSessionStateClosed || status == FBSessionStateCreatedOpening) {
             ms_bIsLoggedIn = false;
             log("Logged In is false!");
             callback1(false);
         }
         else {
             ms_bIsLoggedIn = true;
             log("LoggedIn is true!");
             callback1(true);
         }
     }];
}

void Facebook::Login(void (*callback2)(bool))
{
    
    NSLog(@"Returns: %hhd",[FBSession openActiveSessionWithAllowLoginUI:NO]);
    //InOrNot =[FBSession openActiveSessionWithAllowLoginUI:NO];
        // No, display the login page.
        NSArray *permissions = [[NSArray alloc] initWithObjects:@"email", @"user_friends",nil];
        
        // Attempt to open the session. If the session is not open, show the user the Facebook login UX
        [FBSession openActiveSessionWithReadPermissions:permissions allowLoginUI:true completionHandler:^(FBSession *session, FBSessionState status, NSError *error) {
            
            // Did something go wrong during login? I.e. did the user cancel?
            if (status == FBSessionStateClosedLoginFailed || status == FBSessionStateClosed || status == FBSessionStateCreatedOpening)
            {
                // If so, just send them round the loop again
                [[FBSession activeSession] closeAndClearTokenInformation];
                [FBSession setActiveSession:nil];
                CreateNewSession();
                ms_bIsLoggedIn = false;
                callback2(false);
            }
            else
            {
                ms_bIsLoggedIn = true;
                callback2(true);
            }
        }];
}

/*Sprite* Facebook::displayImage()
{
    NSString *path;
   // NSString *fBId = @"933024033404692";
    int width = 100;
    int height = 100;
    NSString *https = [NSString stringWithFormat:@"https://graph.facebook.com/%s/picture?width=%i&height=%i", fid, width, height];
    NSURL *url = [NSURL URLWithString:https];
    // NSAssert(url, @"URL is malformed: %@", https);
    NSError *error = nil;
    NSData *data = [NSData dataWithContentsOfURL:url options:0 error:&error];
    if (data && !error) {
        // Download the image to the Cache directory of our App.
        path = [[NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:[fid stringByAppendingString:@".jpg"]];
        [data writeToFile:path options:NSDataWritingAtomic error:&error];
        if (!error) {
            // It worked!
            log("Worked!");
        } else {
            // Error, Could not save downloaded file!
            log("Could not save downloaded file!");
        }
    } else {
        // Error, Could not download file!
        log(" Could not download file!");
    }
    const char *c = [path UTF8String];
    //CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(path);
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(c);
    // 2. Create the sprite frame with appropriate dimensions.
    SpriteFrame *frame =  SpriteFrame::createWithTexture(texture, CCRectMake(0, 0, width, height));
    // frame->initWithTexture(texture, CGRectMake(0, 0, width, height));
    Sprite *profilePic= Sprite::createWithSpriteFrame(frame);
    profilePic->setPosition(Vec2(200,200));
    return profilePic;
}
*/
   void Facebook::FetchUserDetails(void (*callback)(bool))
{
    log("Fetch user details called!");
    // Start the facebook request
    [[FBRequest requestForMe]
     startWithCompletionHandler:
     ^(FBRequestConnection *connection, NSDictionary<FBGraphUser> *result, NSError *error)
     {
         // Did everything come back okay with no errors?
         if (!error && result)
         {
             // If so we can extract out the player's Facebook ID and first name
             ms_nsstrFirstName = [[NSString alloc] initWithString:result.first_name];
             ms_uPlayerFBID = [[NSString alloc] initWithString:result.objectID];
             const char *a = [result.first_name UTF8String];
             string fN(a);
             const char *b = [result.last_name UTF8String];
             string lN(b);
             const char *c = [result.objectID UTF8String];
             string fID(c);
             const char *d = [[result objectForKey:@"email"] UTF8String];
             string eM(d);
             const char *e = [[[NSString alloc] initWithFormat: @"http://graph.facebook.com/%s/picture?type=large", c] UTF8String];
             string iURL(e);
             NSLog(@"Fetch user details completed!");

             fn=fN.c_str();
             ln=lN.c_str();
             fId=fID.c_str();
             email=eM.c_str();
             iurl=iURL.c_str();
             LogSuccess::LoginSuccessFrom();

            callback(true);
         }
         else
         {
            callback(false);
         }
     }];
}
void Facebook::profilePicDisplay()
{
    
}
string Facebook::getFn()
{
    return fn;
}
string  Facebook::getLn()
{
    return ln;
}
string Facebook::getiurl()
{
    return iurl;
}
string Facebook::getem()
{
    return email;
}
string Facebook::getfid()
{
    return fId;
}
void Facebook::objectiveC_Call()
{
    //Objective C code calling.....
    log("my class initialize!");
}

