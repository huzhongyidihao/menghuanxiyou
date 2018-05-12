#pragma once
#pragma execution_character_set("utf-8")
/*
主游戏场景 其下面挂了相应的主游戏layer
*/
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainLayer;
class CombatLayer;

class MainGameScene :public cocos2d::Scene
{
public:
	 bool init();
	 
	 void update(float delta);
	 CREATE_FUNC(MainGameScene);


	 enum BGMtype
	 {
		 congbiehou,
		 jingli,
		 jianyecheng
	 }bgmlist;
public:
	void menuEnterCombatCallback(cocos2d::Ref*pSender);//调试菜单项 进入战斗
	void menuExitCombatCallback(cocos2d::Ref*pSender);//调试菜单项 退出战斗
	void menuAutoFindPathCallback(cocos2d::Ref*pSender);//调试菜单项 自动寻路 

	void menuSaveCallback(cocos2d::Ref*pSender);//系统菜单项  存档退出 
	void menuExitCallback(cocos2d::Ref*pSender);//系统菜单项  直接退出

	void OpenMenu(cocos2d::Ref*sPender);//呼出/隐藏菜单
private:
	void EnterCombatLayer();//进入战斗layer
	void ExitCombatLayer();//退出战斗layer

	void HidMainUI();//隐藏主界面UI
	void ShowMainUI();//显示主界面UI

	void UpdateHealthAndEnergy();//更新血量和内力

	std::string SetBGM(BGMtype);
private:
	bool isCreateCombarLayer;
	

	bool isPlayBGM;//是否播放BGM
	float volumeBGM;
private:
	cocos2d::Menu*_mainMenu;//总菜单
	cocos2d::Menu*_debugmenu;//调试菜单

	cocos2d::Sprite*_rolePic;//角色肖像
	cocos2d::ui::LoadingBar*_roleHealth;
	cocos2d::ui::LoadingBar*_roleEnergy;
	MainLayer* _mainlayer;
	CombatLayer*_combatlayer;



};
