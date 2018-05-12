/*
玩家信息类  存储玩家角色的关键信息
就是存档类
*/

#pragma once

class Role;
class PlayInfo  
{
	//friend class GameStartScene;
public:
	PlayInfo();
	~PlayInfo();
public:
	void SynInfo(Role*);//同步信息到角色




public:
	bool _haveMemory;//是否拥有记忆 是则表示角色是读档 并非重开游戏

	/*玩家角色属性*/
	int _roleMaxHealth;//最大生命
	int _roleCurrHealth;//当前生命
	int _roleMaxEnergy;//最大内力
	int _roleCurrEnergy;//当前内力

	int _roleAttackValue;//攻击力
	int _roleDefenseValue;//防御力
	int _roledirection;//角色朝向

	float _roleSpeed;//角色移动速度

	/* 位置信息*/
	float _role_x;
	float _role_y;

	

private:

};

