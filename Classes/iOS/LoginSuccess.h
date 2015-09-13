//
//  LoginSuccess.h
//  Scrabble
//
//  Created by Mishal Hemant Shah on 2/24/15.
//
//

#ifndef __Scrabble__LoginSuccess__
#define __Scrabble__LoginSuccess__

#include <stdio.h>
#include<iostream>
#include <HttpClient.h>
#include <HttpRequest.h>
#include <HttpResponse.h>
#include "cocos2d.h"


using namespace std;
using namespace cocos2d;

class LogSuccess : public cocos2d::Layer
{
public:
    static LogSuccess* instance;
    
    static LogSuccess* getInstance();
    
    static void LoginSuccessFrom();
    void user_register();
    void sendMessageComplete(network::HttpClient *client, network::HttpResponse *response);
    bool CheckHTTPErr(network::HttpResponse* response);
    int LogFrom;
};
#endif /* defined(__Scrabble__LoginSuccess__) */
