#pragma once
#include "cocos2d.h"

enum RoleSkillenum {
	pingA = 0,
	bengleizou,
	yiqihuasanqing,
	longjuanyuji,
	taishanyading,
	riyuetonghui,
	sihaishengping

};//角色技能枚举

class MapNodeClass;
class GameSuanFaClass
{
public:
	static GameSuanFaClass*GetInstance();


public:
	std::deque<cocos2d::Vec2> MinShortWayFind(cocos2d::Vec2 start, cocos2d::Vec2 target,cocos2d::TMXTiledMap* _map, cocos2d::TMXLayer*_tileblockage/*, std::vector<MapNodeClass*>collection*/);//��С·���㷨
private:
	void CreateOutSkirtsNode(MapNodeClass* obj);
	MapNodeClass* SelectNextNode(MapNodeClass* currpoint);
	MapNodeClass* IsHaveNode(int x, int y);//�Ƿ��Ѿ�����������ڵ�
	MapNodeClass* IsFromUnSelect(int x, int y);
	std::deque<cocos2d::Vec2> findBestWay();//Ѱ�����·��

	void SetNodeToSelection(MapNodeClass*point);//���ýڵ������ѡ������
private:
	std::vector<MapNodeClass*>_SelectCollection;
	std::vector<MapNodeClass*>_unSelectionCollection;
	std::vector<MapNodeClass*>_allNodeCollection;

	bool isFindEndPoint;//�Ƿ��ҵ��յ�
	int pointendx;
	int pointendy;
/*---����--*/
private:
	GameSuanFaClass() {};
	GameSuanFaClass(GameSuanFaClass const&);
	GameSuanFaClass&operator=(const GameSuanFaClass&);
private:
	static GameSuanFaClass* _instance;

	class GC
	{
	public:

		~GC()
		{
			if (_instance != nullptr)
			{
				delete _instance;
				_instance = nullptr;
			}
		};
		static GC gc;//�����ͷŵ���

	};

};

class Role;

class GameCombatRule
{
public:
	static GameCombatRule*GetInstance();


	 static RoleSkillenum _gRoleSkill;
public:

	static void  RoleUseSkill( RoleSkillenum RoleSkill,Role*obj, Role*mtarget, std::vector<Role*>*roleptrvect =nullptr);
	static std::vector<Role*> RandomFromTeamGetN(std::vector<Role*>*, int n = 1, Role*obj = nullptr);
private:
	static int DamageCalculation(RoleSkillenum RoleSkill,Role*obj1, Role*obj2);

	static Role* RandomFromTeam(std::vector<Role*>*,Role*obj=nullptr);
	
private:
	GameCombatRule() {};
	GameCombatRule(GameCombatRule const&);
	GameCombatRule&operator=(const GameCombatRule&);
private:
	static GameCombatRule* _instance;

	class GC
	{
	public:

		~GC()
		{
			if (_instance != nullptr)
			{
				delete _instance;
				_instance = nullptr;
			}
		};
		static GC gc;//�����ͷŵ���

	};

};


class GameFormation  
{
public:
	static GameFormation*GetInstance();

	enum {wuzhen=0,tiangang,difu,beidou,heyi,fengxing
			}_formationType;
public:
	void InitFormationInfro(int);//��ʼ������Ϣ--������ʱ �ȵ������fun
	std::string &GetFormationName() { return _formationName; }
	int getFormationAttack() { return _formationAttack; }
	int getFormationDefense() { return _formationDefense; }
	int getFormationSpeed() { return _formationSpeed; }
private:
	std::string _formationName;//������
	float _formationAttack;//�󷨹�����    Ӱ�����ն����Ա������
	float _formationDefense;//�󷨷�����   Ӱ�����ն����Ա������
	float _formationSpeed;//���ٶ�      Ӱ�����ն����Ա�ٶ�

private:
	GameFormation() {};
	GameFormation(GameFormation const&);
	GameFormation&operator=(const GameFormation&);
private:
	static GameFormation* _instance;

	class GC
	{
	public:
		
		~GC() 
		{
			if (_instance!=nullptr)
			{
				delete _instance;
				_instance = nullptr;
			}
		};
		static GC gc;

	};

};

