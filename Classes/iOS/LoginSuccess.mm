//
//  LoginSuccess.cpp
//  Scrabble
//
//  Created by Mishal Hemant Shah on 2/24/15.
//
//

#include "LoginSuccess.h"
#include "GoogleHelper.h"
#include "FBHelper.h"
//#include "UserLogin.h"
#include "json/rapidjson.h"
#include "json/reader.h"
#include "json/document.h"

#include "GameScene.h"

//#include "Player.h"
//#include "Social_scene.h"
using namespace std;
using namespace cocos2d;
static int detailsFetchedFrom,LoggedInCount;
static string NameOfUser,ID,URL,Email_Address,pwd;
static string SERVERURL= "http://hardikdubal.com/scrabble/api/"; //"http://192.168.1.11:1234/api/";  //"http://192.168.1.5:1234/api/";
static string FBORGP;

LogSuccess* LogSuccess::instance = NULL;

LogSuccess* LogSuccess::getInstance() {
    return instance;
}

void LogSuccess::LoginSuccessFrom()
{
    LogSuccess *obj = new LogSuccess();
    instance = obj;
    
    detailsFetchedFrom = cocos2d::UserDefault::getInstance()->getIntegerForKey("Details_Fetched_From", 0);
    LoggedInCount = UserDefault::getInstance()->getIntegerForKey("Login_Count",0);
    
    if(detailsFetchedFrom==1)
    {
        FBORGP = "facebook";
        NameOfUser =          Facebook::getFn() +" "+ Facebook::getLn();
        URL  =          Facebook::getiurl();
        Email_Address = Facebook::getem();
        ID    =         Facebook::getfid();
        NSLog(@"Name : %s",      NameOfUser.c_str());
        NSLog(@"Facebook ID: %s",ID.c_str());
        NSLog(@"Email: %s",      Email_Address.c_str());
        NSLog(@"Image URL: %s",  URL.c_str());
        if(LoggedInCount==1)
        {
            obj->user_register();
            NSLog(@"Logged in from %s for first time",FBORGP.c_str());
        }
        else if(LoggedInCount==2)
        {
            NSLog(@"Logged in from %s for second time",FBORGP.c_str());}
        }
    else if(detailsFetchedFrom==2)
    {
       FBORGP = "google";
       Google *gobj = [[Google alloc]init];
       ID =            [gobj getGID];
       NameOfUser =    [gobj getGName];
       Email_Address = [gobj getGMail];
       URL =           [gobj getGImageURL];
       NSLog(@"Name : %s",      NameOfUser.c_str());
       NSLog(@"Google ID: %s",  ID.c_str());
       NSLog(@"Email: %s",      Email_Address.c_str());
       NSLog(@"Profile URL: %s",URL.c_str());
        if(LoggedInCount==1)
        {
            obj->user_register();
            NSLog(@"Logged in from %s for first time",FBORGP.c_str());
        }
       else if(LoggedInCount==2)
       NSLog(@"Logged in from %s for second time",FBORGP.c_str());
    }
  /*  else if (detailsFetchedFrom==3) {
        FBORGP = "email";
        NameOfUser = EmailLoginPopUp::name;
        Email_Address = EmailLoginPopUp::email;
        pwd = EmailLoginPopUp::pwd;
   
        obj->user_register();
    }*/
}



void LogSuccess::user_register()
{
    CCLOG("User reg called!");
    auto client = network::HttpClient::getInstance();
    auto request = new cocos2d::network::HttpRequest();
    std::string url = SERVERURL + "user_register";
    request->setUrl(url.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(this,httpresponse_selector(LogSuccess::sendMessageComplete));
    
    std::string postData;
    
    std::string deviceToken = UserDefault::getInstance()->getStringForKey("Device_Token");
    
    CCLOG("device token :: %s",deviceToken.c_str());
    if (deviceToken.empty()) {
        deviceToken = "123456";
    }
    
    CCLOG("Final Device TOken :: %s",deviceToken.c_str());
    
    if (detailsFetchedFrom == 1 || detailsFetchedFrom == 2) {
        postData = "name=" + NameOfUser + "&image_url=" + URL + "&email=" + Email_Address + "&provider=" + FBORGP + "&device_type=" + "ios" + "&device_token=" + UserDefault::getInstance()->getStringForKey("Device_Token");
    }
    else {
        postData = "name=" + NameOfUser + "&email=" + Email_Address + "&pwd=" + pwd + "&provider_id=" + ID + "&device_type=" + "ios" + "&device_token=" + deviceToken;
    }
    
    CCLOG("Name of the gamer:: %s",NameOfUser.c_str());
    CCLOG("ID: %s",                ID.c_str());
    CCLOG("Email: %s",             Email_Address.c_str());
    CCLOG("Profile URL: %s",       URL.c_str());
    CCLOG("postData : %s",         postData.c_str());
    
    request->setRequestData(postData.c_str(), strlen(postData.c_str()));
    client->send(request);
}

void LogSuccess::sendMessageComplete(network::HttpClient *client, network::HttpResponse *response)
{
    CCLOG("response code :: %ld",response->getResponseCode());
    
    auto data = response->getResponseData();
    std::string str(data->begin(),data->end());
    
    CCLOG("response data :: %s",str.c_str());
    
    rapidjson::Document dc;
    dc.Parse<0>(str.c_str());
    
    if(response->getResponseCode()==200)
    {
        const rapidjson::Value& user = dc["User"];
        
        UserDefault::getInstance()->setIntegerForKey("Login_From",detailsFetchedFrom);

        int user_id = atoi(user["id"].GetString());

        UserDefault::getInstance()->setIntegerForKey("Player_ID", user_id);
        CCLOG("USER ID :: %d",user_id);
        UserDefault::getInstance()->setStringForKey("Name", NameOfUser);
        UserDefault::getInstance()->setStringForKey("Provider_Id", ID);
        UserDefault::getInstance()->setStringForKey("Image_Url", URL);
        UserDefault::getInstance()->setStringForKey("Email", Email_Address);
        UserDefault::getInstance()->setStringForKey("password", pwd);
        UserDefault::getInstance()->setStringForKey("provider", FBORGP);
        UserDefault::getInstance()->setStringForKey("Device_Type", "ios");
    
        Scene *startScene  =  CGameScene::createScene();
        Director::getInstance()->replaceScene(startScene);
    }
    else
    {
        UserDefault::getInstance()->setIntegerForKey("Login_From",0);
        MessageBox("Login Parameter are incorrect", "Error");
        CCLOG("error buffer :: %s",response->getErrorBuffer());
        CCLOG("Message not successfully sent!");
    }
}



