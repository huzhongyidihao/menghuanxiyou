/*
技能列表显示
*/
#pragma once

#include "cocos2d.h"



class SkillList :public cocos2d ::LayerColor
{
public:
	bool init();
	void onEnter();
	void onExit();

	CREATE_FUNC(SkillList);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
public:
	bool IsReMoveSkill;//是否移除了skilllist
	bool IsSelectSkill;//是否选择了技能
private:


private:
	cocos2d::EventDispatcher*eventdispatcher;

	cocos2d::Sprite*_showSkillList;//显示图片
	cocos2d::Texture2D*_TextureCache;//列表的图片缓存
};

