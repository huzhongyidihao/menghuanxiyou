#include "Role.h"
#include "CombatLayer.h"
#include "GameRuleAbout.h"
#include "effect.h"
USING_NS_CC;

#pragma execution_character_set("utf-8")
CombatLayer* Role::_currCombatlayer = nullptr;
Role::Role():_roleMaxHealth(200),_roleCurrHealth(200),_roleMaxEnergy(300),_roleCurrEnergy(300),_roleAttackValue(50), _roleDefenseValue(22),_roleState(0), _roleSpeed(1),_roledirection(0),
_role_x(0.0f),_role_y(0.0f), _haveMemory(false)
{
	//角色属性随机变更
	_roleAttackValue = _roleAttackValue*random(0.8, 1.2);
	_roleDefenseValue = _roleDefenseValue*random(0.8,1.2);
	_roleSpeed = _roleSpeed*random(0.8, 1.3);


	_currCombatlayer = nullptr;
	currSkillIndex =1;

	isChangeDirection = true;


	//初始化动画
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("renwu/shusheng.plist");
	Director::getInstance()->getTextureCache()->addImage("renwu/shusheng.png");
	//创建角色 精灵
	
	_roleBasePic = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-10.png"));

	/*移动动画*/
	Animation*animation0 = Animation::create();
	for (int i = 0; i <= 3; i++)
	{
		std::string szName = StringUtils::format("f-1%d.png", i);
		animation0->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
	}
	animation0->setDelayPerUnit(3.0f / 15.0f);
	animation0->setRestoreOriginalFrame(true);
	AnimationCache::getInstance()->addAnimation(animation0, "movedirect0");//缓存动画和动画帧
	
	Animation*animation1 = Animation::create();
	for (int i = 0; i <= 3; i++)
	{
		std::string szName = StringUtils::format("f-2%d.png", i);
		animation1->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
	}
	animation1->setDelayPerUnit(3.0f / 15.0f);
	animation1->setRestoreOriginalFrame(true);
	AnimationCache::getInstance()->addAnimation(animation1, "movedirect1");//缓存动画和动画帧

	Animation*animation2 = Animation::create();
	for (int i = 0; i <= 3; i++)
	{
		std::string szName = StringUtils::format("f-3%d.png", i);
		animation2->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
	}
	animation2->setDelayPerUnit(3.0f / 15.0f);
	animation2->setRestoreOriginalFrame(true);
	AnimationCache::getInstance()->addAnimation(animation2, "movedirect2");//缓存动画和动画帧

	Animation*animation3 = Animation::create();
	for (int i = 0; i <= 3; i++)
	{
		std::string szName = StringUtils::format("f-4%d.png", i);
		animation3->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
	}
	animation3->setDelayPerUnit(3.0f / 15.0f);
	animation3->setRestoreOriginalFrame(true);
	AnimationCache::getInstance()->addAnimation(animation3, "movedirect3");//缓存动画和动画帧

	/*移动动作*/
	_roleMoveAnimate0 = Animate::create(AnimationCache::getInstance()->getAnimation("movedirect0"));
	_roleMoveAnimate1 = Animate::create(AnimationCache::getInstance()->getAnimation("movedirect1"));
	_roleMoveAnimate2 = Animate::create(AnimationCache::getInstance()->getAnimation("movedirect2"));
	_roleMoveAnimate3 = Animate::create(AnimationCache::getInstance()->getAnimation("movedirect3"));
	_roleMoveAnimate0->retain();
	_roleMoveAnimate1->retain();
	_roleMoveAnimate2->retain();
	_roleMoveAnimate3->retain();

}

Role::Role(const Role & obj)
{
}

//
//Role & Role::operator=(const Role & obj)
//{
//	// TODO: 在此处插入 return 语句
//
//}

bool Role::operator<(const Role & obj)
{
	if (obj.getRoleSpeed()<this->getRoleSpeed())
	{
		return false;
	}
	else
	{
		return true;
	}

}

Role::~Role()
{
	_roleMoveAnimate0->release();
	_roleMoveAnimate1->release();
	_roleMoveAnimate2->release();
	_roleMoveAnimate3->release();
}

void Role::RoleAttack(Role*target)
{
	log("角色进行攻击");
	

	//执行攻击动画并移动到目标前面
	Vec2 tempAttpoint = Vec2(target->getPosition().x+20, target->getPosition().y-20);
	Vec2 temp = this->getPosition();
	this->_roleBasePic->runAction(Sequence::create(MoveTo::create(0.8f, tempAttpoint), CallFunc::create([&,target]() 
	{
		//技能效果 ,需要用到游戏规则类
		GameCombatRule::_gRoleSkill = pingA;


		GameCombatRule::RoleUseSkill(this,target);
	}), MoveTo::create(0.9f, temp), nullptr));
	log("完成攻击");
}

void Role::RoleDefense(Role*target)
{
	log("角色执行防御");
}

void Role::RoleMagic(Role*target,std::vector<Role*>*roleptrvect)
{
	
	log("角色使用法术");
	//根据法术类型执行不同的动作:角色施法动作 特效之类的
	switch (currSkillIndex)
	{
	case 0:
		//选择不到这项
		break;
	case 1:
		{
			//奔雷咒动作
			log("奔雷咒抬手");

			//法术特效
			effect*_effect = effect::create();
			_effect->init(1);//初始化为1号特效
			_effect->setPosition(target->getPosition());
			_currCombatlayer->addChild(_effect, 6);
	
			_effect->runAction(Sequence::create(_effect->_showeffect, CallFunc::create([&,target,roleptrvect]() {
				_currCombatlayer->removeChild(_effect);
				log("移除特效");
				log("结算技能效果");
				Role* obj = this;
				GameCombatRule::RoleUseSkill(obj, target, roleptrvect);
				_currCombatlayer->IsdoOrder = true;
			}), nullptr));
			//

		}
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	default:
		break;
	}


	log("完成施法");
}

void Role::RoleRecoverHealth(int num)
{
	ValueEffectShow(this, num,1);
	
	if((_roleCurrHealth+num)<=_roleMaxHealth)
	{
		_roleCurrHealth = _roleCurrHealth + num;
	}
	else
	{
		_roleCurrHealth = _roleMaxHealth;
	}
	log("恢复生命");
}

void Role::RoleLoseHealth(int num)
{
	ValueEffectShow(this, num);
	if ((_roleCurrHealth - num)<=0)
	{
		_roleCurrHealth = 0;
	}
	else
	{
		_roleCurrHealth = _roleCurrHealth - num;
	}
	log("损失生命:%d,剩余生命:%d",num,_roleCurrHealth);
}

void Role::RoleAddMaxHealth(int v)
{
	_roleMaxHealth = _roleMaxHealth + v;
}

void Role::RoleAddMaxEnergy(int v)
{
	_roleMaxEnergy = _roleMaxEnergy + v;
}

void Role::RoleAddAttackValue(int v)
{
	_roleAttackValue = _roleAttackValue + v;
}

void Role::RoleAddDefenseValue(int v)
{
	_roleDefenseValue = _roleDefenseValue + v;
}

void Role::SetIsRoleConversely()
{
	//_roleBasePic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-10.png"));
	//暂时没有倒地图片 以改变精灵颜色作为代替
	if (_roleState==3)
	{
		auto color1=_roleBasePic->getDisplayedColor();
		log("color=%d,%d,%d", color1.r, color1.g, color1.b);
		auto color2 = _roleBasePic->getColor();
		log("color=%d,%d,%d", color1.r, color1.g, color1.b);
		_roleBasePic->setColor(Color3B(250, 10, 50));
	}
	else
	{

	}

}

void Role::ValueEffectShow(Role*obj,int num,int typeindex)
{
	auto winSize=Director::getInstance()->getVisibleSize();
	//auto temp =std::to_string(num);//在Android 编译平台会有问题 因为有的ndk不支持C11 
	std::string temp = StringUtils::format("%d", num);//其他方法1
	std::string temp1 = Value(num).asString();//其他方法2
	std::string temp2 = StringUtils::toString(num);//其他方法3
	//char*temp4;
	//sprintf(temp4, "%d", num);//其他方法5
	LabelAtlas*atlasLabel;
	if (typeindex==0)
	{
		atlasLabel = LabelAtlas::create(temp, "pic/labelatlasimg_gold.png", 24, 32, '0');
	}
	else
	{
		atlasLabel = LabelAtlas::create(temp, "pic/labelatlasimg.png", 22, 27, '0');
	}
	//atlasLabel->setPosition(winSize.width / 2, winSize.height / 2);
	atlasLabel->setPosition(obj->getPosition().x-15,obj->getPosition().y+10);
	atlasLabel->setScale(0.7f);
	_currCombatlayer->addChild(atlasLabel);

	auto act1 = Spawn::create(MoveBy::create(1.2, Vec2(0, 50)),
		ScaleBy::create(1, 0.8),
		FadeTo::create(1, 0),
		nullptr);
	auto act2 = Sequence::create(act1, CallFunc::create([=]() {
		_currCombatlayer->removeChild(atlasLabel);
		//log("移除字体特效");
	}
	), nullptr);
	atlasLabel->runAction(act2);
}

void Role::setHealth(int v)
{
	if (v>=0)
	{
		if (v <= _roleMaxHealth)
		{
			_roleCurrHealth = v;
		}
		else
		{
			_roleCurrHealth = _roleMaxHealth;
		}
	}
	
}

void Role::setEnergy()
{
}

void Role::setAttack()
{
}

void Role::setRoleState(int statevaule)
{
	_roleState = statevaule;
}

int& Role::getRoleDirection()
{
	return _roledirection;
}

void Role::setRoleDirection(int value)
{
	_roledirection = value;
}

void Role::setCurrCombatlayer(CombatLayer*obj)
{
	_currCombatlayer = obj;
}


cocos2d::Vec2 Role::GetStartPoint()
{

	return cocos2d::Vec2(_role_x,_role_y);
}

void Role::StandingAnimation()
{
}

void Role::MoveAnimation(float time ,Vec2 point)
{
	//更新方向
	if (getPosition().x < point.x)//如果角色x坐标小于触摸点x坐标(角色获取的坐标也是节点坐标，因为它是这个瓦片地图节点的子对象)
	{


		if (getPosition().y > point.y)
		{
			//即往右下角方向:东
			_roledirection = 0;
		}

		else
		{
			_roledirection=2;//北
		}

	}
	else
	{
		if (getPosition().y > point.y)
		{
			_roledirection=1;//南
		}
		else
		{
			_roledirection=3;//西
		}
	}
	
	//根据朝向direction 来执行不同朝向的动画
//	log("当前角色朝向:%d", _roledirection);
	switch (_roledirection)
	{
	case 0://东
	{

		if (isChangeDirection)//如果改变了方向
		{
			auto repaction0 = RepeatForever::create(_roleMoveAnimate0);
			repaction0->setTag(1);
			_roleBasePic->runAction(repaction0);
		//	log("动画0");
		}
		moveaction = MoveTo::create(time, point);
		_roleBasePic->runAction(moveaction);
		_roleBasePic->runAction(Sequence::create(DelayTime::create(time), CallFunc::create([=]() {
			if (getPosition().distance(point)<=0.8f)
			{
				log("到达目标，停止角色移动动画");
				_roleBasePic->stopActionByTag(1);

				//log("停止动作");
				_roleBasePic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-10.png"));
				_roleBasePic->setFlippedX(false);
				setRoleState(0);
			}
			else
			{
				log("时间到了，但是没有到达目标，继续播放移动动画");
			}		
		}), nullptr));
	//	log("移动东0");
		
	}
	break;
	case 1://南
	{	
	
	//	log("动画1的引用计数为=%d", _roleMoveAnimate1->getReferenceCount());
		if (isChangeDirection)//如果改变了方向
		{
			auto repaction1 = RepeatForever::create(_roleMoveAnimate1);
			repaction1->setTag(1);
			_roleBasePic->runAction(repaction1);
		//	log("动画1");
		}
		
		moveaction = MoveTo::create(time, point);
		_roleBasePic->runAction(moveaction);
		_roleBasePic->runAction(Sequence::create(DelayTime::create(time), CallFunc::create([=]() {

			if (getPosition().distance(point) <= 1.0f)
			{
				_roleBasePic->stopActionByTag(1);
				//		log("停止动作");
				_roleBasePic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-23.png"));
				setRoleState(0);
			}
			else
			{
				log("时间到了，但是没有到达目标，继续播放移动动画");
			}

		}), nullptr));
	//log("移动南1");
	}
		break;
	case 2://北
	{
		if (isChangeDirection)//如果改变了方向
		{
			auto repaction2 = RepeatForever::create(_roleMoveAnimate2);
			repaction2->setTag(1);
			_roleBasePic->runAction(repaction2);
		//	log("动画2");
		}
		moveaction = MoveTo::create(time, point);
		_roleBasePic->runAction(moveaction);
		_roleBasePic->runAction(Sequence::create(DelayTime::create(time), CCCallFunc::create([=]() {
			if (getPosition().distance(point) <= 1.0f)
			{
				_roleBasePic->stopActionByTag(1);
				//		log("停止动作");
				_roleBasePic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-33.png"));
				setRoleState(0);
			}
			else
			{
				log("时间到了，但是没有到达目标，继续播放移动动画");
			}
	
		}), nullptr));
	//log("移动北2");
	}
		break;
	case 3://西
	{

		if (isChangeDirection)//如果改变了方向
		{
			auto repaction3 = RepeatForever::create(_roleMoveAnimate3);
			repaction3->setTag(1);
			_roleBasePic->runAction(repaction3);
		//	log("动画3");
			
		}

		moveaction = MoveTo::create(time, point);
		_roleBasePic->runAction(moveaction);
		_roleBasePic->runAction(Sequence::create(DelayTime::create(time), CCCallFunc::create([=]() {

			if (getPosition().distance(point) <= 1.0f)
			{
				_roleBasePic->stopActionByTag(1);
				//	log("停止动作");
				_roleBasePic->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-43.png"));
				setRoleState(0);
			}
			else
			{
				log("时间到了，但是没有到达目标，继续播放移动动画");
			}
			
		}), nullptr));
	//log("移动西3");
	}
		break;
	default:
		break;
	}
}

void Role::InitCombatInfo(std::vector<Role*>&rhrole, std::vector<Role*>&rhfoe)
{
	for (auto &w:rhrole)
	{
		_OurTeamCollection.push_back(w);
	}
	for (auto &w:rhfoe)
	{
		_FoeTeamCollection.push_back(w);
	}
	log("完成初始化战场信息");

}

void Role::setPosition(Vec2 point)
{
	_roleBasePic->setPosition(point);
}

Vec2 Role::getPosition()
{
	
	auto x=_roleBasePic->getPositionX();
	auto y = _roleBasePic->getPositionY();

	return Vec2(x,y);
}

void Role::UpdateRolebase(Role * obj)
{
	_roleMaxHealth = obj->_roleMaxHealth;
	_roleCurrHealth = obj->_roleCurrHealth;
	_roleMaxEnergy = obj->_roleMaxEnergy;
	_roleCurrEnergy = obj->_roleCurrEnergy;
	_roleAttackValue = obj->_roleAttackValue;
	_roleDefenseValue = obj->_roleDefenseValue;
	_roleSpeed = obj->_roleSpeed;

	_rolePostionFormation = obj->_rolePostionFormation;
	_OurTeamCollection = obj->_OurTeamCollection;
	_FoeTeamCollection = obj->_FoeTeamCollection;

}

//void Role::setRoleMoveDirection()
//{
//	
//	switch (_roledirection)
//	{
//	case 0://东
//	{
//		Sprite*obj = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-10.png"));
//		delete _roleBasePic;
//		_roleBasePic = obj;
//	}
//		break;
//	case 1://南
//	{
//		Sprite*obj = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-10.png"));
//		obj->setFlippedX(true);
//		delete _roleBasePic;
//		_roleBasePic = obj;
//		
//	}
//		break;	
//	case 2://北
//	{
//		Sprite*obj = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-43.png"));
//		obj->setFlippedX(true);
//		delete _roleBasePic;
//		_roleBasePic = obj;
//	}
//		break;
//	case 3://西
//		{
//			Sprite*obj = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("f-43.png"));
//			delete _roleBasePic;
//			_roleBasePic = obj;
//		}
//		break;
//		
//	default:
//		break;
//	}
//
//
//
//}
