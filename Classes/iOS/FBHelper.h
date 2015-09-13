//
//  Social_Login.h
//  Scrabble
//
//  Created by Mishal Hemant Shah on 2/15/15.
//
//

#ifndef Scrabble_Social_Login_h
#define Scrabble_Social_Login_h
//#include "HelloWorldScene.h"
#include <stdio.h>
#include <FacebookSDK/FacebookSDK.h>
using namespace std;
using namespace cocos2d;
class Facebook:public cocos2d::Node
{
public:
    static void objectiveC_Call();
    static void CreateNewSession();
    static void OpenSession(void (*callback)(bool));
    static void Login(void(*callback)(bool));
    static void FetchUserDetails(void(*callback)(bool));
    static void profilePicDisplay();
    Sprite* displayImage();
    
    static void sendBrag(const int score);
    static bool IsLoggedIn();
    static void Logout(void (*callback)(bool));
    static void SetLoggedIn(bool bLoggedIn);
    
    static string getFn();
    static string getLn();
    static string getiurl();
    static string getem();
    static string getfid();

};
//bool InOrNot;

#endif
