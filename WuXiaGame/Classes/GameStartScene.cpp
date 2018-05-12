#include "GameStartScene.h"
#include "GameSystem.h"
#include "PlayInfo.h"
USING_NS_CC;
#pragma execution_character_set("utf-8")


PlayInfo* GameStartScene ::_playinfoSave =nullptr;

bool GameStartScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	/*-----------------------*/
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	MenuItemFont::setFontName("fonts/DFPShaoNvW5-GB.ttf");
	MenuItemFont::setFontSize(24);
	MenuItemFont* menuiteStart = MenuItemFont::create("新的探索",CC_CALLBACK_1(GameStartScene::menuStartCallback, this));
	MenuItemFont* menuiteGoon = MenuItemFont::create("旧的回忆", CC_CALLBACK_1(GameStartScene::menuGoCallback, this));
	MenuItemFont* menuiteExit = MenuItemFont::create("覆水难收", CC_CALLBACK_1(GameStartScene::menuExitCallback, this));
	Menu*menu = Menu::create(menuiteStart, menuiteGoon, menuiteExit, nullptr);
	menu->setAnchorPoint(Vec2::ZERO);
//	menu->setPosition(Vec2(50, 100));
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	menu->alignItemsVerticallyWithPadding(20);
	addChild(menu);
	return true;
}

void GameStartScene::menuStartCallback(cocos2d::Ref * pSender)
{
	GameSystem::GetInstance()->GoToXXScene(1);
	log("开始游戏!");
}

void GameStartScene::menuGoCallback(cocos2d::Ref * pSender)
{
	//读档
	ReadSave();
	GameSystem::GetInstance()->GoToXXScene(1);
	log("继续游戏!");
}

void GameStartScene::menuExitCallback(cocos2d::Ref * pSender)
{
	log("结束游戏!");
	Director::getInstance()->end();
}

void GameStartScene::ReadSave()
{
	//假设存档上限为1
	PlayInfo* saveitem = new PlayInfo();

	saveitem->_roleAttackValue= UserDefault::getInstance()->getIntegerForKey("攻击力");
	saveitem->_roleDefenseValue = UserDefault::getInstance()->getIntegerForKey("防御力");
	saveitem->_roleMaxHealth = UserDefault::getInstance()->getIntegerForKey("最大生命");
	saveitem->_roleCurrHealth = UserDefault::getInstance()->getIntegerForKey("当前生命");
	saveitem->_roleMaxEnergy = UserDefault::getInstance()->getIntegerForKey("最大内力");
	saveitem->_roleCurrEnergy = UserDefault::getInstance()->getIntegerForKey("当前内力");
	saveitem->_roledirection = UserDefault::getInstance()->getIntegerForKey("朝向");
	saveitem->_roleSpeed = UserDefault::getInstance()->getFloatForKey("速度");

	saveitem->_role_x = UserDefault::getInstance()->getFloatForKey("角色x坐标");
	saveitem->_role_y = UserDefault::getInstance()->getFloatForKey("角色y坐标");

	GameStartScene::_playinfoSave = saveitem;
	log("Over ReadSave!");
}
