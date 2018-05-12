#include "MainGameScene.h"
#include "MainLayer.h"
#include "CombatLayer.h"
#include "Role.h"

USING_NS_CC;
#pragma execution_character_set("utf-8")

bool MainGameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size winSize = Director::getInstance()->getVisibleSize();
	log("VisibleSize: width=%f,height=%f",winSize.width,winSize.height);
	
	
	Size currwinSize = Director::getInstance()->getWinSize();
	log("CurrWinSize: width=%f,height=%f", currwinSize.width, currwinSize.height);
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	log("FrameSize: width=%f,height=%f", frameSize.width, frameSize.height);
	Rect visrect=Director::getInstance()->getOpenGLView()->getVisibleRect();
	log("rect: width=%f,height=%f",visrect.size.width,visrect.size.height);
	log("rect poition=(%f,%f)", visrect.origin.x, visrect.origin.y);

	/*------------------------*/
	isCreateCombarLayer = false;


	isPlayBGM = true;
	/*------------------------*/
	/*初始化角色信息UI*/
	/*显示角色信息: 头像 血条 内力 之类*/
	log("初始化角色信息ui");
	_rolePic = Sprite::create("pic/rolehead.png");
	_rolePic->setScale(0.5);
	_rolePic->setAnchorPoint(Vec2::ZERO);
	_rolePic->setPosition(1, Director::getInstance()->getVisibleSize().height - 160 * 0.5);
	addChild(_rolePic, 7);
	_roleHealth = ui::LoadingBar::create("pic/healthslot.png");
	_roleHealth->setPercent(100.f);
	_roleHealth->setAnchorPoint(Vec2::ZERO);
	_roleHealth->setVisible(true);
	_roleHealth->setPosition(Vec2(1, 23));
	addChild(_roleHealth, 7);
	_roleEnergy = ui::LoadingBar::create("pic/energyslot.png");
	_roleEnergy->setPercent(100.f);
	_roleEnergy->setAnchorPoint(Vec2::ZERO);
	_roleEnergy->setVisible(true);
	_roleEnergy->setPosition(Vec2(1, 2));
	addChild(_roleEnergy, 7);
	/*-----------------*/
	
	_mainlayer = MainLayer::create();
	_mainlayer->setBGMstring(SetBGM(BGMtype::jianyecheng));

	addChild(_mainlayer,0);
	log("添加MainLayer");


	MenuItemImage*mainitem = MenuItemImage::create("pic/menu.png", "pic/menu.png", CC_CALLBACK_1(MainGameScene::OpenMenu, this));

	_mainMenu = Menu::create(mainitem, nullptr);
	_mainMenu->setAnchorPoint(Vec2::ZERO);
	_mainMenu->setPosition(winSize.width-20,winSize.height-20);
	_mainMenu->alignItemsHorizontally();
	addChild(_mainMenu,2);
	


	MenuItemFont::setFontSize(14);
//	MenuItemFont* entorCombat = MenuItemFont::create("进入战斗!", CC_CALLBACK_1(MainGameScene::menuEnterCombatCallback, this));
//	MenuItemFont*exitCombat = MenuItemFont::create("退出战斗!", CC_CALLBACK_1(MainGameScene::menuExitCombatCallback, this));
	MenuItemFont*autoFindPath = MenuItemFont::create("自动寻路!", CC_CALLBACK_1(MainGameScene::menuAutoFindPathCallback, this));
	MenuItemFont*exitSave = MenuItemFont::create("存档!", CC_CALLBACK_1(MainGameScene::menuSaveCallback, this));
	MenuItemFont*exitNoSave = MenuItemFont::create("退出!", CC_CALLBACK_1(MainGameScene::menuExitCallback, this));
	_debugmenu = Menu::create(exitSave, exitNoSave, autoFindPath,nullptr);
	addChild(_debugmenu, 2);
	_debugmenu->setPosition(50,Director::getInstance()->getVisibleSize().height-200);
	_debugmenu->alignItemsVerticallyWithPadding(20);
	scheduleUpdate();


	return true;
}

void MainGameScene::update(float delta)
{
		//if (_combatlayer->isStartCombat)//如果开始战斗
		//{
		//}
	UpdateHealthAndEnergy();
	if (_mainlayer->applyEnterCombat)//大地图是否 发送了申请战斗
	{
		if (isCreateCombarLayer != true)
		{
			_mainlayer->HidAllRoleAndNPC();
			EnterCombatLayer();
		}
	}

	if (isCreateCombarLayer)
	{
		//隐藏主界面UI
		HidMainUI();
		if (_combatlayer->isStartCombat)
		{
			//开始战斗后 开始同步战斗场景中主角 信息到大地图中的主角信息
			_mainlayer->GetMainRole()->UpdateRolebase(_combatlayer->_myRole);
			
		}
		if (_combatlayer->isEndCombat)
		{

			//消除此怪物
			_mainlayer->ClearTargetRole(_mainlayer->_cruuCombatFoe);
			
			ExitCombatLayer();
			ShowMainUI();
			log("结束战斗");
			
		}
	}
	

}

void MainGameScene::menuEnterCombatCallback(cocos2d::Ref * pSender)
{
	if (isCreateCombarLayer !=true)
	{
		_mainlayer->HidAllRoleAndNPC();
		EnterCombatLayer();
	}	
	log("进入战斗,MainGameScene 调用");
}

void MainGameScene::menuExitCombatCallback(cocos2d::Ref * pSender)
{
	
	ExitCombatLayer();
	log("退出战斗,MainGameScene 调用");
}

void MainGameScene::menuAutoFindPathCallback(cocos2d::Ref * pSender)
{

	_mainlayer->autoFindPath();


	log("调试菜单项,自动寻路MainGameScene 调用");
}

void MainGameScene::menuSaveCallback(cocos2d::Ref * pSender)
{
	log("存档");
	
		UserDefault::getInstance()->setIntegerForKey("攻击力", 0);
		UserDefault::getInstance()->setIntegerForKey("防御力", 0);
		UserDefault::getInstance()->setIntegerForKey("最大生命", 0);
		UserDefault::getInstance()->setIntegerForKey("当前生命", 0);
		UserDefault::getInstance()->setIntegerForKey("最大内力", 0);
		UserDefault::getInstance()->setIntegerForKey("当前内力", 0);
		UserDefault::getInstance()->setFloatForKey("速度", 0.0f);
		UserDefault::getInstance()->setIntegerForKey("朝向", 0);
		UserDefault::getInstance()->setFloatForKey("角色x坐标", 0);
		UserDefault::getInstance()->setFloatForKey("角色y坐标", 0);

		//
		auto obj = _mainlayer->GetMainRole();
		UserDefault::getInstance()->setIntegerForKey("攻击力",obj->getAttack());
		UserDefault::getInstance()->setIntegerForKey("防御力", obj->getDefense());
		UserDefault::getInstance()->setIntegerForKey("最大生命", obj->getMaxHealth());
		UserDefault::getInstance()->setIntegerForKey("当前生命", obj->getHealth());
		UserDefault::getInstance()->setIntegerForKey("最大内力", obj->getMaxEnergy());
		UserDefault::getInstance()->setIntegerForKey("当前内力", obj->getEnergy());
		UserDefault::getInstance()->setFloatForKey("速度", obj->getRoleSpeed());
		UserDefault::getInstance()->setIntegerForKey("朝向", obj->getRoleDirection());
		UserDefault::getInstance()->setFloatForKey("角色x坐标", obj->getPosition().x);
		UserDefault::getInstance()->setFloatForKey("角色y坐标", obj->getPosition().y);

		UserDefault::getInstance()->flush();
		std::string filepath = UserDefault::getInstance()->getXMLFilePath();
		log("xml文件保存路径:%s",filepath.c_str());

}

void MainGameScene::menuExitCallback(cocos2d::Ref * pSender)
{
	log("退出");
	Director::getInstance()->end();
}

void MainGameScene::OpenMenu(cocos2d::Ref * sPender)
{
	if (_debugmenu->isVisible())
	{
		log("隐藏菜单");
		_debugmenu->setVisible(false);
	}
	else
	{
		log("显示菜单");
		_debugmenu->setVisible(true);
	}
}

void MainGameScene::EnterCombatLayer()
{
	log("创建战斗场景");
	_combatlayer = CombatLayer::create();
	_combatlayer->setBGMstring(SetBGM(BGMtype::congbiehou));
	isCreateCombarLayer = true;
	/*为战斗场景添加战斗角色*/
	Role* foe01 = new Role();
	Role* foe02 = new Role();
	_combatlayer->addTeamMember(foe01, 0);
	_combatlayer->addTeamMember(foe02, 0);

	_combatlayer->_myRole = new Role();
	_combatlayer->_myRole->UpdateRolebase(_mainlayer->GetMainRole());
	_combatlayer->addTeamMember(_combatlayer->_myRole, 1);
	Role::setCurrCombatlayer(_combatlayer);
	addChild(_combatlayer,1);
	_combatlayer->DrawOurTeam();//描绘己方队伍
	_combatlayer->DrawFoeTeam();//描绘敌人
	_combatlayer->UpdateActionOrder();//第一次调用,更新行动顺序

	_combatlayer->UpdateActionRole();//更新当前行动角色
	_combatlayer->GetAllRoleSpriteShowSize();
	
	
	_combatlayer->isStartCombat = true;
	log("开始战斗!");
}

void MainGameScene::ExitCombatLayer()
{
	_combatlayer->setBGMstring(SetBGM(BGMtype::jianyecheng));
	this->removeChild(_combatlayer);
	_mainlayer->ShowALLRoleAndNPC();

	log("移除战斗场景");
	_mainlayer->applyEnterCombat = false;
	isCreateCombarLayer = false;
}

void MainGameScene::HidMainUI()
{
	_rolePic->setVisible(false);

	_debugmenu->setVisible(false);


	//log("隐藏主界面UI");
}

void MainGameScene::ShowMainUI()
{
	_rolePic->setVisible(true);
	_debugmenu->setVisible(true);
	log("显示主界面UI");
}

void MainGameScene::UpdateHealthAndEnergy()
{
	auto obj=_mainlayer->GetMainRole();
	float health =( obj->getHealth()*1.0f / obj->getMaxHealth()*1.0f)*100.0f;
	float energy =(obj->getEnergy()*1.0f / obj->getMaxEnergy()*1.0f)*100.0f;
	_roleHealth->setPercent(health);
	_roleEnergy->setPercent(energy);
}

std::string MainGameScene::SetBGM(BGMtype bgmlist)
{
	std::string str;

	switch (bgmlist)
	{
	case congbiehou:
		str = "music/cbh.mp3";
		break;
	case jingli:
		str = "music/jl.mp3";
		break;
	case jianyecheng:
		str = "music/jyc.mp3";
		break;
	default:
		break;
	}
	return str;
}
