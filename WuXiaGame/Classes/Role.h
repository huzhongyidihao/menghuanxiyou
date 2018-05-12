#pragma once
/*
角色类:包括属性、动画
*/

#include "cocos2d.h"


//enum RoleSkillenum;

class CombatLayer;

class Role
{
	friend class PlayInfo;


public:
	Role();
	Role(const Role&obj);
	Role& operator=(const Role&obj);
	bool operator<(const Role&obj);//排序所用
	~Role();
public:
	void RoleAttack(Role*);//角色进行攻击
	void RoleDefense(Role*);//角色执行防御
	void RoleMagic(Role*,std::vector<Role*>*roleptrvect =nullptr);//角色使用法术

	void RoleRecoverHealth(int );//恢复生命
	void RoleLoseHealth(int );//损失生命

	void SetIsRoleConversely();//设置角色倒地/还是闲置状态 (死亡状态 角色设置为倒地)


	static void ValueEffectShow( Role*obj,int num,int typeindex=0);//数值特效

public:
	/*基本属性访问器*/
	int getHealth() { return _roleCurrHealth; }
	int getMaxHealth() { return _roleMaxHealth; }
	int getMaxEnergy() { return _roleMaxEnergy; }
	int getEnergy() { return _roleCurrEnergy; }
	int getAttack() { return _roleAttackValue; }
	int getDefense() { return _roleDefenseValue; }
	void setHealth();
	void setEnergy();
	void setAttack();
	int getRoleState() { return _roleState; }
	void setRoleState(int);//设置角色状态
	float getRoleSpeed() const{ return _roleSpeed; }

	int& getRoleDirection();//获取角色朝向
	void setRoleDirection(int);//设置角色朝向

	int GetRoleTag() { return _roleBasePic->getTag(); }//获取角色标签
	static	void setCurrCombatlayer(CombatLayer*);//设置当前战斗场景  :供maingammescene调用

	cocos2d::Vec2 GetStartPoint();
public:
	void StandingAnimation();//播放站立动画
	void MoveAnimation(float,cocos2d::Vec2);//播放移动动画,0-3分别代表东西南北朝向
	
	void InitCombatInfo(std::vector<Role*>&,std::vector<Role*>&);//初始化战场信息,暂时没用上
	void setPosition(cocos2d::Vec2 point);//设置坐标 
	cocos2d::Vec2 getPosition();

	//用于场景切换时 role 更新属性的方法
	void UpdateRolebase(Role*obj);//更新角色基本属性(战斗相关)

public:
	bool _haveMemory;//是否拥有记忆 是则表示角色是读档 并非重开游戏
	cocos2d::Sprite*_roleBasePic;

	
	int currSkillIndex;
private:
	/*游戏性属性-角色区别的主要属性*/
	/*  基本属性*/
	int _roleMaxHealth;//最大生命
	int _roleCurrHealth;//当前生命
	int _roleMaxEnergy;//最大内力
	int _roleCurrEnergy;//当前内力

	int _roleAttackValue;//攻击力
	int _roleDefenseValue;//防御力
	int _roledirection;//角色朝向

	float _roleSpeed;//角色移动速度

	/*-----------------------------*/
	float _role_x;//角色所处地图x坐标
	float _role_y;//角色所处地图y坐标
	int _roleState;//角色状态 0 闲置 1移动 2战斗 3死亡
private:
	//void setRoleMoveDirection();//设置人物朝向,在移动动画结束后调用


	

private:
	cocos2d::Animate*_roleStandAnimate;
	cocos2d::Animate* _roleMoveAnimate0;
	cocos2d::Animate* _roleMoveAnimate1;
	cocos2d::Animate* _roleMoveAnimate2;
	cocos2d::Animate* _roleMoveAnimate3;


	int _rolePostionFormation;//角色所处阵法位置

	/*战场信息*/
	static	CombatLayer*_currCombatlayer;//当前战斗场景
	std::vector<Role*>_OurTeamCollection;//我方队伍角色 列表
	std::vector<Role*>_FoeTeamCollection;//敌法队伍角色 列表
};

