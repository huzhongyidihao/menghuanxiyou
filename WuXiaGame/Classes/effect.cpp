#include "effect.h"

USING_NS_CC;


bool effect::init(int typeindex)
{
	if (!Sprite::init())
	{
		return false;
	}
	/*-----------------*/
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pic/sl_effect.plist");
	Director::getInstance()->getTextureCache()->addImage("pic/sl_effect.png");

	this->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("sl00.png"));
	Animation* _effect = Animation::create();
	for (int i=0;i<3;i++)
	{
		for (int k=0;k<10;k++)
		{
			if (i==2&&k==5)
			{
				break;
			}
			else
			{
				std::string str = StringUtils::format("sl%d%d.png", i, k);
				_effect->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(str));
			}
		
		}
		
	}

	_effect->setDelayPerUnit(3.0f / 60.0f);
	//_effect->setRestoreOriginalFrame(true);
	AnimationCache::getInstance()->addAnimation(_effect, "slxz_lg");//»º´æ¶¯»­ºÍ¶¯»­Ö¡

	_showeffect = Animate::create(AnimationCache::getInstance()->getAnimation("slxz_lg"));

	
	return true;
}

void effect::onEnter()
{
	Sprite::onEnter();




}

void effect::onExit()
{
	Sprite::onExit();




}

effect * effect::create()
{
	effect* obj = new effect();
	if (obj)
	{
		obj->autorelease();
		return obj;
	}
	else
	{
		delete obj;
		obj = nullptr;
		return nullptr;
	}
	return nullptr;
}
