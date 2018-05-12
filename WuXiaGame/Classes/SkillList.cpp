#include "SkillList.h"

USING_NS_CC;
#pragma execution_character_set("utf-8")

bool SkillList::init()
{
	if (!LayerColor::init())
	{
		return false;
	}
	/*---------*/
	IsReMoveSkill = false;
	IsSelectSkill = false;
	/*---------*/
	Size winSize = Director::getInstance()->getVisibleSize();


	_showSkillList = Sprite::create("pic/skilllist.png");
//	_TextureCache = Director::getInstance()->getTextureCache()->addImage("pic/skilllist.png");
//	_showSkillList->setTexture(_TextureCache);
	_showSkillList->setScale(0.7);
	auto temp=_showSkillList->getContentSize()*_showSkillList->getScale();
//	log("尺寸:width=%f,height=%f", temp.width, temp.height);
//	_showSkillList->setAnchorPoint(Vec2::ZERO);


	this->setContentSize(temp);//设置层尺寸同图片精灵尺寸
	addChild(_showSkillList);

	log("技能列表的 初始化 函数");
	return true;
}

void SkillList::onEnter()
{

	LayerColor::onEnter();

	//setTouchEnabled(true);
	//setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(SkillList::onTouchBegan, this);

	listener->onTouchMoved = CC_CALLBACK_2(SkillList::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(SkillList::onTouchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(SkillList::onTouchEnded, this);
	eventdispatcher = Director::getInstance()->getEventDispatcher();
	eventdispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	log("技能列表的 onEnter 函数");
}

void SkillList::onExit()
{
	LayerColor::onExit();
}

bool SkillList::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	log("技能栏的触摸开始");
	return true;
}

void SkillList::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	
}

void SkillList::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
}

void SkillList::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unused_event)
{
	Vec2 v = touch->getLocation();
//	log("返回触摸点: %f,%f",v.x,v.y);
//	log("技能栏左下角坐标%f,%f", getPositionX() - getContentSize().width / 2, getPositionY() - getContentSize().height / 2);
//	log("精灵尺寸:%f,%f",_showSkillList->getContentSize().width,_showSkillList->getContentSize().height);
//	log("页面尺寸:%f,%f", getContentSize().width, getContentSize().height);
	Rect rect = Rect(Vec2(getPositionX()-getContentSize().width/2,getPositionY()-getContentSize().height/2),Size(_showSkillList->getContentSize().width*0.7, _showSkillList->getContentSize().height*0.7));
//	log("尺寸:width=%f,height=%f", _showSkillList->getContentSize().width*0.7, _showSkillList->getContentSize().height*0.7);
	if (rect.containsPoint(v))
	{
		
		log("点击到了技能列表");
		
		if (IsSelectSkill)//如果选择了技能
		{
			IsReMoveSkill = true;
		}


	}
	else
	{
		log("点击到了技能列表外围");
		IsReMoveSkill = true;
		//关闭技能列表
	}

}
