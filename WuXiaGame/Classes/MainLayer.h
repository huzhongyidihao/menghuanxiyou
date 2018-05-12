/*主游戏layer 描绘大地图 玩家 等*/
#pragma once


#include "cocos2d.h"

class Role;
class MapNodeClass;
class GameSuanFaClass;

class MainLayer:public cocos2d::Layer
{
public:
	bool init();
	void onEnter();
	void onExit();
	void update(float delta);
	CREATE_FUNC(MainLayer);
public:
	void ViewfollowWithRole(float,cocos2d::Point position);//视图跟随玩家角色

	void EnterCombat();//进入战斗
	void ExitCombat();//离开战斗
	Role* GetMainRole() { return _role; }
	cocos2d::TMXTiledMap*GetMap() { return _tilemap; }
public:
	bool onTouchBegan(cocos2d::Touch *touch,cocos2d:: Event *unused_event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void HidAllRoleAndNPC();//隐藏所有角色和NPC
	void ShowALLRoleAndNPC();//显示所有角色和NPC

	void autoFindPath();//自动寻路-供maingamescene 调试调用


						/*随机生成怪物*/
	void RandomCreateFoe(int index = 0);//根据index 怪物类型 随机生成怪物

	void ClearTargetRole(Role*obj);//清除目标role 主要是用于消除战斗后的怪物

public:
	bool applyEnterCombat;//申请进入战斗

	Role*_cruuCombatFoe;//当前场景进入战斗的怪物

	void setBGMstring(std::string);
private:
	void CreateFoe(cocos2d::Vec2 point, int typeindex=0);//创建一个foe
	bool IsCanCreateFoe();//是否能够生成怪物
	cocos2d::Vec2 RandomCreateFoePoistion();//随机生成怪物位置
	void InitFoeArea();//初始化怪物区

	Role* IscollisionRole(Role*obj);//是否发生角色碰撞


	void autoMovePath(std::deque<cocos2d::Vec2>,cocos2d::Vec2);//自动移动 寻路所得路径  辅助函数 供autoFindPath调用
	float CountMoveTime(cocos2d::Vec2 rolepoint,cocos2d::Vec2 targetpoint,float speed);//计算移动到目标点的时间
	
	void InitCurrMapInfo();//初始化当前地图信息
	cocos2d::Vec2 ConvertPositionInMap(cocos2d::Vec2);//获取当前角色在地图上的位置 哪个瓦片
	cocos2d::Vec2 ConvertPositionFromMapVec(cocos2d::Vec2);//将瓦片坐标转换为 地图节点坐标(即地图上的位置而非第几个瓦片)
private:
	Role*_role;
	cocos2d::TMXTiledMap*_tilemap;
	cocos2d::TMXObjectGroup*_tileobjgroup;
	cocos2d::TMXLayer*_tilefoe;
	cocos2d::TMXLayer*_tileblockage;
	cocos2d::TMXLayer*_tilebaselayer;
	cocos2d::EventDispatcher*eventdispatcher;

	cocos2d::Vec2 _pathend;//终点  地图坐标

	bool IsCombat;//是否进入战斗
	bool IsAutoFindPath;//是否正在自动寻路(移动)

	/*当前地图相关*/
	std::vector<cocos2d::Vec2>_foeArea;//怪物区 生成怪物的地域
	std::vector<MapNodeClass*> _currmapinfo;//当前地图信息

	std::string _mapbgmstr;



	int _currfoenums;//当前场景怪物数量
	int _maxfoenums;//当前场景最大怪物数量
	std::vector<Role*> _foeCollection;//地图foe容器
};