LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/Android/Cached_Android.cpp \
../../Classes/Android/Social_scene_Android.cpp \
../../Classes/AndroidInAppDelegate.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/Ball.cpp \
../../Classes/BidWindow.cpp \
../../Classes/BonusPanel.cpp \
../../Classes/ChangePlayerWindow.cpp \
../../Classes/ChatLayer.cpp \
../../Classes/ChooseLetterWindow.cpp \
../../Classes/ConfirmWindow.cpp \
../../Classes/Deck.cpp \
../../Classes/DescriptionWindow.cpp \
../../Classes/Dictionary.cpp \
../../Classes/EndLevelWindow.cpp \
../../Classes/Field.cpp \
../../Classes/GameScene.cpp \
../../Classes/InApp.cpp \
../../Classes/Letter.cpp \
../../Classes/LoseWindow.cpp \
../../Classes/MenuItemWithDescription.cpp \
../../Classes/Player.cpp \
../../Classes/Room.cpp \
../../Classes/RoomButton.cpp \
../../Classes/RoomsTable.cpp \
../../Classes/ScorePointer.cpp \
../../Classes/Stack.cpp \
../../Classes/SwapPanel.cpp \
../../Classes/Thimble.cpp \
../../Classes/UserLogin.cpp \
../../Classes/WaitingWindow.cpp \
../../Classes/WindowsInAppDelegate.cpp \
../../Classes/pugixml.cpp \
../../Classes/xml.cpp \
../../Classes/JNICPPHelper.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
