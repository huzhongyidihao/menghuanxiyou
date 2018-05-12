LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/HelloWorldScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/SkillList.cpp \
				   $(LOCAL_PATH)/../../../Classes/PlayInfo.cpp \
				   $(LOCAL_PATH)/../../../Classes/MapNodeClass.cpp \
				   $(LOCAL_PATH)/../../../Classes/MainLayer.cpp \
				   $(LOCAL_PATH)/../../../Classes/MainGameScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/GameStartScene.cpp \
				   $(LOCAL_PATH)/../../../Classes/CombatLayer.cpp \
				   $(LOCAL_PATH)/../../../Classes/GameRuleAbout.cpp \
				   $(LOCAL_PATH)/../../../Classes/GameSystem.cpp \
				   $(LOCAL_PATH)/../../../Classes/Role.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
