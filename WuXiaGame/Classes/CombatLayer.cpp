#include "CombatLayer.h"
#include "Role.h"
#include "SkillList.h"
#include "GameRuleAbout.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace CocosDenshion;
#pragma execution_character_set("utf-8")
bool CombatLayer::init()
{
	if (!LayerColor::init())
	{
		return false;
	}
	/*-----------基础---------*/
	auto color = cocos2d::Color3B(120, 110, 230);
	this->setColor(color);
	setOpacity(10);
	scheduleUpdate();

	Size winSize= Director::getInstance()->getVisibleSize();

	/*----------------------------------*/
	/*全局UI相关*/
	//显示回合计时
	_timeLabel = Label::createWithTTF("xx", "fonts/DFPShaoNvW5-GB.ttf", 24);
	_timeLabel->setPosition(winSize.width/2, winSize.height-35);
	addChild(_timeLabel,11);

	isCountDown = false;
	_stateMaxTime = 30;
	_stateCurrTime = _stateMaxTime;
	isEndCombat = false;
	//角色UI
	isShowRoleUI = false;
	


	/*战斗UI相关*/
	_attackItem = MenuItemImage::create("pic/attack1.png","pic/attack2.png",CC_CALLBACK_1(CombatLayer::_roleAttackFunction,this));
	_defenseItem = MenuItemImage::create("pic/defense1.png", "pic/defense2.png", CC_CALLBACK_1(CombatLayer::_roleDefenseFunction,this));
	_magicItem = MenuItemImage::create("pic/magic1.png", "pic/magic2.png", CC_CALLBACK_1(CombatLayer::_roleMagicFunction, this));
	_attackItem->setScale(0.6);
	_defenseItem->setScale(0.6);
	_magicItem->setScale(0.6);
	_menu = Menu::create(_attackItem, _defenseItem, _magicItem, nullptr);
	_menu->setAnchorPoint(Vec2(1, 1));
	_menu->setPosition(Director::getInstance()->getVisibleSize().width - 30, Director::getInstance()->getVisibleSize().height - 50);
	_menu->alignItemsVerticallyWithPadding(5);
	addChild(_menu,10);

	/*战斗指令相关*/
	IsDetectionTouch = false;
//	IsMagicDetectionTouch = false;
	IsSatisfyTerm = false;
	/*AI相关*/
	IsdoOrder = false;
	/*-----------角色相关--------*/
	_myRole = nullptr;
	_tatgetRole = nullptr;
	DrawOurTeam();//显示己方队伍
	DrawFoeTeam();//显示敌方队伍
	_CurrentActionIndex = 0;
	_CurrenActionRole = nullptr;

	isSucceed = false;
	isStartCombat = false;

	/*技能栏UI*/
	_skilllayer = false;
	/*------*/

	log("战斗场景初始化完成");
	return true;
}

void CombatLayer::onEnter()
{
	LayerColor::onEnter();
	log("CombatLayer onEnter!");
	//setTouchEnabled(true);
	//setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CombatLayer::onTouchBegan, this);

	listener->onTouchMoved = CC_CALLBACK_2(CombatLayer::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(CombatLayer::onTouchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(CombatLayer::onTouchEnded, this);
	eventdispatcher = Director::getInstance()->getEventDispatcher();
	eventdispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto temp = _combatbgmstr.c_str();
	//log("%s",temp);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(temp);

}

void CombatLayer::onExit()
{
	LayerColor::onExit();

	log("CombatLayer onExit!");
	for (int i=0;i<_showRoleCollection.size();i++)
	{
		delete _showRoleCollection[i];//释放内存
		log("delete _showRoleCollection ");
	}
	_showRoleCollection.clear();
	for (int i = 0; i < _showFoeCollection.size(); i++)
	{
		delete _showFoeCollection[i];//释放内存
		log("delete _showFoeCollection ");
	}
	_showFoeCollection.clear();

	if (!_ActionOrder.empty())
	{
		_ActionOrder.clear();
		log("清空__ActionOrder");
	}
	if (!_allRoleRectFrom.empty())
	{
		_allRoleRect.clear();
		_allRoleRectFrom.clear();
		log("清空_allRoleRect和_allRoleRectFrom");
	}
	//if (_ActionOrder.empty()!=true)
	//{
	//	for (int i = 0; i < _ActionOrder.size(); i++)
	//	{
	//		delete _ActionOrder[i];//释放内存
	//		log("delete _ActionOrder ");
	//	}
	//	_ActionOrder.clear();
	//}
	//else
	//{
	//	log("_ActionOrder为空 没有内存释放");
	//}
	//


	log("内存释放");

	//
	

	auto temp = _combatbgmstr.c_str();
	//log("%s",temp);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(temp);
}

void CombatLayer::update(float delta)
{
	//log("开始调用战斗场景循环!");
	if (isStartCombat)//如果开始战斗
	{
		IsCombatEnd();//判断是否结束战斗
		if (!isCountDown)
		{
			CountDown();
			
		}

		if (_CurrenActionRole==_myRole)//如果是自己的行动回合
		{
			if (!isShowRoleUI)
			{
				ShowRoleStateUI();
				isShowRoleUI = true;
				log("轮到我了!");
			}
			if (IsdoOrder)//如果已经执行操作
			{
				IsdoOrder = false;
				_stateCurrTime = 0;
				HideRoleStateUI();
				log("角色回合结束");
			}
			if (_skilllayer)
			{
				if (_skilllayer->IsReMoveSkill)//如果要移除skill
				{
					removeChild(_skilllayer);
					_skilllayer->IsReMoveSkill = false;
					log("移除技能栏列表");
				}
			}
		}
		else
		{
			
			if (_stateCurrTime<=28)
			{
				//回合开始2秒后 执行操作
				if (!IsdoOrder)//如果还没有进行操作
				{
					FoeDoOrder(_CurrenActionRole);
					IsdoOrder = true;
					_stateCurrTime = 0;
				}
			}
			
		
		}
	}
}

CombatLayer * CombatLayer::create()
{
	CombatLayer *pRet = new(std::nothrow) CombatLayer(); 
		if (pRet && pRet->init()) 
		{ 
			pRet->autorelease(); 
			return pRet; 
		} 
		else 
		{ 
			delete pRet; 
			pRet = nullptr; 
			return nullptr; 
		} 
}

bool CombatLayer::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{

	log("战斗状态下的触摸事件响应");
	return true;
}

void CombatLayer::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}

void CombatLayer::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}

void CombatLayer::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	auto ve = touch->getLocation();
//	log("触摸点坐标(%f,%f)", ve.x, ve.y);
	if (IsDetectionTouch)
	{
	
		log("触摸点坐标(%f,%f)", ve.x, ve.y);
		//判断是否点击到了角色(无论己方敌法)
		bool ret=IsSelectRoleTarget(ve);//获得结果
		if (ret)//如果返回true
		{
			//判断目标是不是合格:即 如果攻击指令 则目标只能为敌方
			bool IsRight=JudgeTargetIsRight();
			if (IsRight)
			{
				//如果符合指令条件
				switch (_RoleOrderType)
				{
				case CombatLayer::gongji:
					_myRole->RoleAttack(_tatgetRole);//进行攻击
					IsDetectionTouch = false;
					IsdoOrder = true;
					break;
				case CombatLayer::fangyu:
					_myRole->RoleDefense(_tatgetRole);
					IsDetectionTouch = false;
					IsdoOrder = true;
					break;
				case CombatLayer::shifa:
				{
					GameCombatRule::_gRoleSkill = bengleizou;
					_myRole->currSkillIndex = GameCombatRule::_gRoleSkill;
				
					_myRole->RoleMagic(_tatgetRole, GetFoeTeamCollection());
					IsDetectionTouch = false;
					//IsdoOrder = true;
				}
					break;
				default:
					break;
				}
				
				
			}
		}
	}



}

void CombatLayer::addTeamMember(Role *obj,int index)
{
	if (index==0)
	{
		_showFoeCollection.push_back(obj);
		log("当前敌方队伍人数:%d", _showFoeCollection.size());
		log("为敌方队伍添加成员");
	}
	else
	{
		_showRoleCollection.push_back(obj);
		log("当前己方队伍人数:%d", _showRoleCollection.size());
		log("为己方队伍添加成员");
	}
}

void CombatLayer::UpdateActionOrder()
{
	if(_ActionOrder.empty()!=true)
	{
		
		std::sort(_ActionOrder.begin(), _ActionOrder.end());	
	}
	else
	{
		//填充_ActionOrder容器
		_ActionOrder.insert(_ActionOrder.end(), _showRoleCollection.begin(), _showRoleCollection.end());
		_ActionOrder.insert(_ActionOrder.end(), _showFoeCollection.begin(), _showFoeCollection.end());
		log("_ActionOrder 长度:%d", _ActionOrder.size());
	}
}

void CombatLayer::CountDown()
{
	isCountDown = true;
	schedule(schedule_selector(CombatLayer::UpdateStateTime),1.0f);
}

void CombatLayer::DrawOurTeam()
{
	Size winSize = Director::getInstance()->getVisibleSize();
	if (_showRoleCollection.empty()!=true)
	{
		for (int i=0;i<_showRoleCollection.size();i++)
		{	
			addChild(_showRoleCollection[i]->_roleBasePic);
			_showRoleCollection[i]->setPosition(Vec2(winSize.width - 120, winSize.height - 200));
			auto v = _showRoleCollection[i]->getPosition();
			_showRoleCollection[i]->_roleBasePic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-41.png"));
			log("己方角色的坐标:(%f,%f)", v.x, v.y);
			_showRoleCollection[i]->_roleBasePic->setTag(1);

		}
		
	}
	log("描绘己方队伍");
}

void CombatLayer::DrawFoeTeam()
{
	Size winSize = Director::getInstance()->getVisibleSize();
	if (_showFoeCollection.empty() != true)
	{
		for (int i=0;i<_showFoeCollection.size();i++)
		{
			
			addChild(_showFoeCollection[i]->_roleBasePic);
			_showFoeCollection[i]->setPosition(Vec2(50+i*50, winSize.height-50-i*50));
			_showFoeCollection[i]->_roleBasePic->setTag(0);
		
			auto v = _showFoeCollection[i]->getPosition();
			log("敌法角色的坐标:(%f,%f)", v.x, v.y);
			log("已经描绘一个敌人");
		}
	}
	log("描绘敌方队伍");
}
void CombatLayer::GetAllRoleSpriteShowSize()
{
	//获取己方角色的显示范围
	for (auto &w : _showRoleCollection)
	{
		auto tempSize = w->_roleBasePic->getContentSize();
		auto v = w->getPosition();
		log("己方角色的坐标:(%f,%f)",v.x,v.y);
		_allRoleRect.push_back(Rect(Vec2(v.x - tempSize.width / 2, v.y - tempSize.height / 2),tempSize));
		_allRoleRectFrom.push_back(w);
	}
	//获取敌法角色的显示范围
	for (auto&w:_showFoeCollection)
	{
		auto tempSize = w->_roleBasePic->getContentSize();
		auto v = w->getPosition();
		log("敌法角色的坐标:(%f,%f)", v.x, v.y);
		_allRoleRect.push_back(Rect(Vec2(v.x-tempSize.width/2,v.y-tempSize.height/2), tempSize));
		_allRoleRectFrom.push_back(w);
	}
}
void CombatLayer::setBGMstring(std::string str)
{
	_combatbgmstr = str;
}
bool CombatLayer::IsSelectRoleTarget(Vec2 point)
{
	for (int i=0;i<_allRoleRect.size();i++)
	{
		log("执行了一次循环");
		if (_allRoleRect[i].containsPoint(point))//如果在范围内
		{
			_tatgetRole=_allRoleRectFrom[i];//得到目标角色
			log("选择了目标,跳出for循环");
			return true;
		}
	}
	log("for循环结束了");

	
	return false;
}

bool CombatLayer::JudgeTargetIsRight()
{
	switch (_RoleOrderType)
	{
	case gongji:
		if (_tatgetRole->GetRoleTag()==diren)
		{
			log("满足条件，可以攻击");
			return true;
		}
		else
		{
			return false;
		}
		break;
	case fangyu:
		if (_tatgetRole->GetRoleTag() == youjun)
		{
			log("满足条件，可以防御");
			return true;
		}
		else
		{
			return false;
		}
		break;
	case shifa:
		if (_tatgetRole->GetRoleTag() == diren)
		{
			log("满足条件，可以施法");
			return true;
		}
		else
		{
			return false;
		}
		break;
	default:
		return false;
		break;
		
	}





}

void CombatLayer::UpdateStateTime(float dt)
{
	

	if (_stateCurrTime <=0)
	{
		isCountDown = false;
		_stateCurrTime = _stateMaxTime;
		UpdateActionRole();
	//	log("更新行动角色");
		
		
	}
	else
	{
		_stateCurrTime--;
		_timeLabel->setString(StringUtils::format("%d",_stateCurrTime));
		//	log("当前回合时间:%d", _stateCurrTime);
	}
}

void CombatLayer::UpdateActionRole()
{
		if (_CurrentActionIndex<_ActionOrder.size())
		{	
			_CurrenActionRole = _ActionOrder[_CurrentActionIndex];
			_CurrentActionIndex++;
		}
		else
		{
			_CurrentActionIndex = 0;
			_CurrenActionRole = _ActionOrder[_CurrentActionIndex];
			_CurrentActionIndex++;
		}
		isShowRoleUI = false;
		if (_CurrenActionRole== _showRoleCollection[0])
		{
			log("玩家A的回合");
		}
		else
		{
			if (_CurrenActionRole==_showFoeCollection[0])
			{
				log("电脑B的回合");
			}
			else
			{
				log("电脑C的回合");
			}
		}

		IsdoOrder =false;
		//UpdateActionOrder();//更新行动顺序
}

void CombatLayer::IsCombatEnd()
{
	//log("判断战斗是否结束");
	if (IsOverRoleTeam())
	{
		isEndCombat = true;
		isSucceed = false;
		log("胜败乃兵家常事,大侠请重新来过.");
	}
	if (IsOverFoeTeam())
	{
		isEndCombat = true;
		isSucceed = true;
		log("恭喜!战斗胜利!");
	}
}

void CombatLayer::_roleAttackFunction(cocos2d::Ref * pSender)
{
	
	_RoleOrderType = gongji;
	IsDetectionTouch = true;//开启指令触摸检测  可以选取攻击目标了
	log("攻击指令");
}

void CombatLayer::_roleDefenseFunction(cocos2d::Ref * pSender)
{
	
	_RoleOrderType = fangyu;
	IsDetectionTouch = true;
	log("防御指令");
}

void CombatLayer::_roleMagicFunction(cocos2d::Ref * pSender)
{
	auto winSize=Director::getInstance()->getVisibleSize();
	_RoleOrderType = shifa;
	IsDetectionTouch = true;

	//创建一个技能栏 来选择技能
	//_skilllayer = SkillList::create();
	//_skilllayer->setPosition(winSize.width / 2, winSize.height / 2);
	//_skilllayer->IsReMoveSkill = false;
	//addChild(_skilllayer, 11);//优先级高于战斗UI按钮
	//暂定法术指定为 奔雷咒
//	RoleSkill = bengleizou;
	log("法术指令");
}

bool CombatLayer::IsOverRoleTeam()
{
	int temp = 0;
	for (int i=0;i<_showRoleCollection.size();i++)
	{
		if (_showRoleCollection[i]->getHealth()==0)//如果角色阵亡
		{
			log("角色阵亡.");
			_showRoleCollection[i]->setRoleState(3);
			temp++;
		}
	
	}
	if (temp==_showRoleCollection.size())
	{
		log("己方队伍团灭了~~");
		return true;
	}
	return false;
}

bool CombatLayer::IsOverFoeTeam()
{
	int temp = 0;
	for (int i = 0; i < _showFoeCollection.size(); i++)
	{
		if (_showFoeCollection[i]->getHealth()==0)//如果角色阵亡
		{
			log("角色阵亡.");
			_showFoeCollection[i]->setRoleState(3);
			temp++;
		}

	}
	if (temp == _showFoeCollection.size())
	{
		log("敌法队伍团灭了~~");
		return true;
	}
	return false;
}

void CombatLayer::ShowRoleStateUI()
{
	log("显示角色回合UI");
	_menu->setVisible(true);
	isShowRoleUI = true;
}

void CombatLayer::HideRoleStateUI()
{

	_menu->setVisible(false);

	log("隐藏角色回合UI");
}

void CombatLayer::FoeDoOrder(Role*obj)
{
	//全力进攻
	auto temp = GameCombatRule::RandomFromTeamGetN(&_showRoleCollection, 1);
	obj->RoleAttack(temp[0]);
	log("foe的回合");
}
