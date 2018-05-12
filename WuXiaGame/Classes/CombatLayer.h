/*
战斗layer  和梦幻一样 战斗时直接在大地图上添加一层layer
*/
#pragma once
#include "cocos2d.h"
class Role;
class SkillList;

class CombatLayer:public cocos2d::LayerColor
{
public:
	bool init();
	void onEnter();
	void onExit();
	void update(float delta);

	//CREATE_FUNC(CombatLayer);
	static CombatLayer*create();
public:
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void addTeamMember(Role*,int);//添加队伍成员
	void UpdateActionOrder();//更新行动顺序
	void CountDown();
	void UpdateActionRole();//更新行动角色

	void DrawOurTeam();//描绘己方队伍
	void DrawFoeTeam();//描绘敌人
	std::vector<Role*>*GetRoleTeamCollection() { return  &_showRoleCollection; }
	std::vector<Role*>*GetFoeTeamCollection() { return  &_showFoeCollection; }


	void GetAllRoleSpriteShowSize();//获取所有角色精灵的显示大小  这个函数是上面函数的辅助函数

	void setBGMstring(std::string);
public:
	bool isStartCombat;//是否开始战斗
	bool isEndCombat;//是否结束战斗
	bool isSucceed;//战斗是否胜利

	/*----------自身角色---------------*/
	Role*_myRole;//玩家自身角色指针
private:
	bool IsSelectRoleTarget(cocos2d::Vec2);//是否选择了角色目标
	

	bool JudgeTargetIsRight();//判断目标是否合格
	void UpdateStateTime(float dt);//更新回合时间

	void IsCombatEnd();//战斗是否结束判断函数
private:
	void _roleAttackFunction(cocos2d::Ref*pSender);
	void _roleDefenseFunction(cocos2d::Ref*pSender);
	void _roleMagicFunction(cocos2d::Ref*pSender);

	bool IsOverRoleTeam();//己方队伍是否团灭
	bool IsOverFoeTeam();//敌法队伍是否团灭


	void ShowRoleStateUI();//显示角色回合UI  自身角色回合开始时调用
	void HideRoleStateUI();//隐藏角色回合UI  自身角色回合结束后调用


	void FoeDoOrder(Role*obj);//敌人执行指令fun
private:
	std::string _combatbgmstr;

	bool isCountDown;//是否计时
	int _stateMaxTime;//回合总时间 x秒
	int _stateCurrTime;//回合当前时间 x秒

	bool isShowRoleUI;//是否显示了角色UI

	/*  ------指令相关-----*/
	bool IsDetectionTouch;//是否开启攻击指令触摸检测
//	bool IsMagicDetectionTouch;//是否开启施法指令触摸检测

	bool IsSatisfyTerm;//是否满足指令条件
	Role*_tatgetRole;//指令目标角色
	
	enum {	gongji = 0, fangyu, shifa	}_RoleOrderType;//当前命令类型
	/*---------------------*/
	enum {diren=0,youjun}_roleType;



	cocos2d::Label*_timeLabel;//显示时间的标签
	cocos2d::Menu*_menu;
	cocos2d::MenuItemImage*_attackItem;
	cocos2d::MenuItemImage*_magicItem;
	cocos2d::MenuItemImage*_defenseItem;
	std::vector<Role*>_showRoleCollection;//队友阵列显示容器
	std::vector<Role*>_showFoeCollection;//敌人阵列显示容器
	std::vector<Role*>_ActionOrder;//行动顺序容器
	Role*_CurrenActionRole;//当前行动角色
	int _CurrentActionIndex;//当前行动角色索引
	cocos2d::EventDispatcher*eventdispatcher;

	SkillList*_skilllayer;//技能栏layer




	//存放精灵显示的RECT的容器
	std::vector<cocos2d::Rect>_allRoleRect;
	std::vector<Role*>_allRoleRectFrom;

	/*战斗流程相关*/
	bool IsdoOrder;//是否执行了指令  判断回合是否提前结束



};