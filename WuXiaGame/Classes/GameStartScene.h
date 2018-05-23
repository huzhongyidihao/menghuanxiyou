/*
游戏程序 开始场景
*/
#pragma once
#include "cocos2d.h"
class PlayInfo;
class GameStartScene:public cocos2d::Scene
{
public:
	bool init();
	CREATE_FUNC(GameStartScene);


	void menuStartCallback(cocos2d::Ref*pSender);
	void menuGoCallback(cocos2d::Ref*pSender);
	void menuExitCallback(cocos2d::Ref*pSender);
public:
	/*存档*/
	static PlayInfo*_playinfoSave;//玩家信息存档 



	cocos2d::TMXTiledMap*_tilemap;
private:
	void ReadSave();//读档
};

