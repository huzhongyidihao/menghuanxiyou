#include "GameRuleAbout.h"
#include "Role.h"
#include "MapNodeClass.h"

USING_NS_CC;
#pragma execution_character_set("utf-8")

//std::vector<Vec2> MinShortWayFind(Vec2 start/*起点*/, Vec2 target/*终点*/)
//{
//	/*
//	定义一步:只能单一改变x 或者y ，即只能向上下左右走，不能斜行
//	步骤是：f1判断可行进的方向,f2选择一条方向  	
//	设bx-ax=n by-ay=h  即从A到B 需要位移x坐标n,位移y坐标h
//	即 每在x坐标上靠近b点 n-1 反之n+1，y同理;
//	当 n=h=0时  到达目标点		*/
//	enum 
//	{
//		北=0,
//		南,
//		西,
//		东
//	}godirection;//前进方向枚举
//	std::vector<Vec2> tempPoint;//存储最小路径所有点
//	int n = target.x - start.x;
//	log("两点x轴差距:%d", n);
//	int h = target.y - start.y;
//	log("两点y轴差距:%d", h);
//
//	Vec2 currpoint = start;//初始化当前坐标
//
//	while (currpoint!=target)
//	{
//		//f1 获取当前点可行进方向
//
//		
//
//	}
//
//	return tempPoint;
//}

//Vec2 GetCurrentPointOnMap()
//{
//	return Vec2(6,6);
//}





GameSuanFaClass*GameSuanFaClass::_instance = nullptr;

GameSuanFaClass * GameSuanFaClass::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new GameSuanFaClass();
	}
	return _instance;
}

std::deque<Vec2> GameSuanFaClass::MinShortWayFind(Vec2 start/*起点*/, Vec2 target/*终点*/, cocos2d::TMXTiledMap* _map, cocos2d::TMXLayer*_tileblockage/*, std::vector<MapNodeClass*> collection*/)
{
	log("起点瓦片坐标:%f,%f", start.x, start.y);
	log("终点瓦片坐标:%f,%f", target.x, target.y);
	_tilemap = _map;
	MapNodeClass::InitTMXMapandLayer(_map,_tileblockage);
	pointendx = target.x;
	pointendy = target.y;
	

	MapNodeClass* satrtpoint = new MapNodeClass(start.x,start.y);
	satrtpoint->InitEndPoint(pointendx, pointendy);
	_allNodeCollection.push_back(satrtpoint);
	_SelectCollection.push_back(satrtpoint);
	MapNodeClass* currpoint = _SelectCollection[0];//初始化当前点为起点
	
	isFindEndPoint = false;
	//主循环
	do 
	{
		CreateOutSkirtsNode(currpoint);//创建外围点
		auto temppoint =SelectNextNode(currpoint);//从外围中选出下一个当前点
		currpoint = temppoint;
		SetNodeToSelection(currpoint);//将选定的当前点添加入_SelectCollection,并将其从_unSelectionCollection删除
	
		
	} while (!isFindEndPoint/*如果终点出现在当前点的外围*/);
	

	//for(auto &w:_SelectCollection)
	//{
	//	log("路径点(%f,%f)",w->getXvaule(),w->getYvaule());//打印坐标
	//	auto _tilebaselayer = _map->getLayer("basemap");
	//	Sprite* obj=_tilebaselayer->getTileAt(Vec2(w->getXvaule(),w->getYvaule()));
	//	obj->setColor(Color3B(100,110,20));
	//}

	//此时_SelectCollection存储的节点就是有效路径,需要从中选出一条最短路径
	return  findBestWay();

}

void GameSuanFaClass::CreateOutSkirtsNode(MapNodeClass*point)
{
	auto x = point->getXvaule();
	auto y = point->getYvaule();
	auto size = _tilemap->getMapSize();
		for (int i=x-1;i<=x+1;i++)
		{
			for (int k=y-1;k<=y+1;k++)
			{
				if (i>=0&&i<size.width&&k>=0&&k<size.height)//先判断点是否存在地图上
				{
					//先判断这个要创建的点是不是终点
					if (pointendx == i&&pointendy == k)
					{
						//如果是终点
						isFindEndPoint = true;
						break;
					}
					else
					{
						auto ret = IsHaveNode(i, k);
						//判断是否已经创建过
						if (!ret)//如果没创建过
						{

							//那么创建
							MapNodeClass* obj = new MapNodeClass(i, k, point);
							_allNodeCollection.push_back(obj);
							if (obj->getIsPass())//如果可以通行
							{
								obj->setParent(point);
								_unSelectionCollection.push_back(obj);//放置入未选择容器
							}

						}
					}
				}
		
			}
		}



}

MapNodeClass* GameSuanFaClass::SelectNextNode(MapNodeClass*currpoint)
{
	//从当前节点外围选取-此时外围所有节点都已经被创建 并将所有可行的节点已经放入_unSelectionCollection
	//需要做的事比较_unSelectionCollection 中的F值
	auto x = currpoint->getXvaule();
	auto y = currpoint->getYvaule();
	auto vf = -1;
	MapNodeClass* tempselect = nullptr;
	//获得最小F值得 节点
	for (int i=x-1;i<=x+1;i++)
	{
		for (int k = y - 1; k <= y + 1; k++)
		{
			auto temp = IsFromUnSelect(i, k);//从待检测_unSelectionCollection 查询
			if (temp)//如果存在
			{
				temp->UpdateFvaule(currpoint);//更新g值
											  //使用新的F值来比较
				if (vf < 0)
				{
					vf = temp->getFvaule();
					tempselect = temp;
				}
				else
				{
					if (temp->getFvaule() < vf)
					{
						vf = temp->getFvaule();
						tempselect = temp;
					}
				}
			}

		}
	}
	if (!tempselect)//如果为空
	{
		//如果外围没有可行进节点
		//那么久要走前面没走的方向
		//改为遍历_unSelectionCollection 进行G值更新 再比较F 得出 下一个"当前点"
		for (auto &w : _unSelectionCollection)
		{
			
			if (vf < 0)
			{
				vf = w->getFvaule();
				tempselect = w;
			}
			else
			{
				if (w->getFvaule() < vf)
				{
					vf = w->getFvaule();
					tempselect = w;
				}
			}
		}
	}


	//这就是需要的结果
	return tempselect;
}

MapNodeClass* GameSuanFaClass::IsHaveNode(int x, int y)
{
	for (auto &w:_allNodeCollection)
	{
		if (w->getXvaule()==x)
		{
			if (w->getYvaule()==y)
			{
				//同时满足x y相等，则说明已经创建过这个节点
				return w;
			}
		}
	}
	return  nullptr;
}

MapNodeClass * GameSuanFaClass::IsFromUnSelect(int x, int y)
{
	for (auto &w : _unSelectionCollection)
	{
		if (w->getXvaule() == x)
		{
			if (w->getYvaule() == y)
			{
				//同时满足x y相等，则说明此节点属于待检测列表
				return w;
			}
		}
	}
	return  nullptr;
}

std::deque<Vec2> GameSuanFaClass::findBestWay()
{
	std::deque<Vec2> wayCollection;

	//在_SelectCollection 中操作
	//容器0元素 为起点 尾元素为 终点前的最后一个"当前点",最优路径就是从尾元素往前找,通过parent 获取上一个节点 直到找到首元素

	auto itend = _SelectCollection.end()-1;//获取最后一个元素 的迭代器
	MapNodeClass* curptr = *itend;
	do 
	{
		//存储当前元素的坐标
		auto x = curptr->getXvaule();
		auto y = curptr->getYvaule();
		wayCollection.push_front(Vec2(x, y));

		curptr=curptr->getParent();//传递父节点

	} while (curptr);//如果curptr存在
		
	
	return wayCollection;
}

void GameSuanFaClass::SetNodeToSelection(MapNodeClass * point)
{
	//添加到_SelectCollection

	_SelectCollection.push_back(point);
	//从_unSelectionCollection 删除
	auto tempbegin = _unSelectionCollection.begin();
	auto tempend = _unSelectionCollection.end();
	auto ret=find(tempbegin, tempend, point);
	if (ret!=tempend)
	{
		_unSelectionCollection.erase(ret);
	}
}

GameCombatRule*GameCombatRule::_instance = nullptr;
RoleSkillenum GameCombatRule::_gRoleSkill = pingA;
GameCombatRule * GameCombatRule::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new GameCombatRule();
	}
	return _instance;
}

void GameCombatRule::RoleUseSkill( Role*obj,Role*mtarget,std::vector<Role*>*roleptrvect)
{
	switch (_gRoleSkill)
	{
	case RoleSkillenum::pingA:
	{
		//根据攻防计算伤害,只有一个对象obj1	
		auto num = DamageCalculation( obj, mtarget);
		mtarget->RoleLoseHealth(num);
	}
		break;
	case RoleSkillenum::bengleizou:
		//同上，但是随机附带2个对象   共分别计算3次伤害
	{
		//主目标
		auto num0 = DamageCalculation( obj, mtarget);
		mtarget->RoleLoseHealth(num0);
		
		auto temp = RandomFromTeamGetN(roleptrvect,2,mtarget);
		log("随机附带伤害了%d个敌人",temp.size());
		if (temp.size()==2)
		{
			int num1 = DamageCalculation( obj, temp[0]);
			temp[0]->RoleLoseHealth(num1);
			int num2 = DamageCalculation( obj, temp[1]);
			temp[1]->RoleLoseHealth(num2);
		}
		else
		{
			if (temp.size()==1)
			{
				int num1 = DamageCalculation( obj, temp[0]);
				temp[0]->RoleLoseHealth(num1);
			}
		}
			


	}
		break;
	case RoleSkillenum::yiqihuasanqing:
		break;
	case RoleSkillenum::longjuanyuji:
		//根据攻防计算伤害,但是随机附带4个对象   共分别计算5次伤害
		break;
	case RoleSkillenum::taishanyading:
		//同上，但是随机附带2个对象   共分别计算3次伤害
		break;
	case RoleSkillenum::riyuetonghui:
		break;
	case RoleSkillenum::sihaishengping:
		break;
	default:
		break;
	}
}

int GameCombatRule::DamageCalculation(Role * obj1, Role * obj2)
{
	
	int num;
	
	switch (_gRoleSkill)
	{
	case RoleSkillenum::pingA:
		//获取obj1的攻击力 以及obj2的防御力
		num = obj1->getAttack() - obj2->getDefense();
		break;
	case RoleSkillenum::bengleizou:
		num = obj1->getAttack()*2.3 - obj2->getDefense();
		break;
	case RoleSkillenum::yiqihuasanqing:
		num = obj1->getAttack()*1.3 - obj2->getDefense();
		break;
	case RoleSkillenum::longjuanyuji:
		num = obj1->getAttack()*1.6 - obj2->getDefense();
		break;
	case RoleSkillenum::taishanyading:
		num = obj1->getAttack()*2.3 - obj2->getDefense();
		break;
	case RoleSkillenum::riyuetonghui:
		num = obj1->getAttack()*1.3 - obj2->getDefense();
		break;
	case RoleSkillenum::sihaishengping:
		num = obj1->getAttack()*1.3 - obj2->getDefense();
		break;
	default:
		break;
	}
	return num;
}

Role * GameCombatRule::RandomFromTeam(std::vector<Role*>*team,Role*obj)
{
	auto temp = *team;
	if (obj!=nullptr)
	{
		//排除obj
		auto it=find(temp.begin(), temp.end(), obj);
		if (it!= temp.end())//如果找到了
		{
			temp.erase(it);
		}
		//从剩余队列中随机
		int length = temp.size();
		if (length>1)
		{
			int x = random(0,length);
			return (*team)[x];
		}
	}
	else
	{
		int length = temp.size();
		if (length > 1)
		{
			int x = random(0, length);
			return (*team)[x];
		}
	}
}

std::vector<Role*> GameCombatRule::RandomFromTeamGetN(std::vector<Role*>*ptrvect, int n,Role*obj)
{
	auto temp = *ptrvect;
	std::vector<Role*>retvect;

	if (obj!=nullptr)//如果存在
	{
		auto tempit=find(temp.begin(), temp.end(), obj);
		temp.erase(tempit);
	}

	int length = temp.size();
	while (n>0)
	{
		if (length > 1)
		{
			int x = random(0, length);
			retvect.push_back(temp[x]);
			n--;
			if (n>0)
			{
				auto it=find(temp.begin(), temp.end(), temp[x]);
				temp.erase(it);
				length--;
			}
		}
		else
		{
			if (!temp.empty())//如果不为空
			{
				retvect.push_back(temp[0]);
			}
			
			break;
		}
	}
	return retvect;
}



GameFormation*GameFormation::_instance = nullptr;
GameFormation * GameFormation::GetInstance()
{
	if (_instance==nullptr)
	{
		_instance = new GameFormation();
	}
	return _instance;
}

void GameFormation::InitFormationInfro(int index)
{
	//阵法信息在这里初始化
	switch (index)
	{
	case 0:
		log("无阵");
		_formationName = "无阵";
		_formationAttack = 1;
		_formationDefense = 1;
		_formationSpeed = 1;
	case 1:
		log("天罡");
		_formationName = "天罡";
		_formationAttack = 2.2;
		_formationDefense = 1.3;
		_formationSpeed = 1.2;
	case 2:
		log("地覆");
		_formationName = "地覆";
		_formationAttack = 1.8;
		_formationDefense = 1.3;
		_formationSpeed = 1.2;
	case 3:
		log("北斗");
		_formationName = "北斗";
		_formationAttack = 1.6;
		_formationDefense = 1.6;
		_formationSpeed = 1.3;
	case 4:
		log("鹤翼");
		_formationName = "鹤翼";
		_formationAttack = 1.3;
		_formationDefense = 1.7;
		_formationSpeed = 1.1;
	case 5:
		log("风行");
		_formationName = "风行";
		_formationAttack = 1.2;
		_formationDefense = 1.1;
		_formationSpeed = 2;
	default:
		break;
	}


}


