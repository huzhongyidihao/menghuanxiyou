#include "GameSystem.h"
#include "GameStartScene.h"
#include "MainGameScene.h"
USING_NS_CC;

GameSystem* GameSystem::_instance = nullptr;
GameSystem* GameSystem::GetInstance()
{
	if (_instance==nullptr)
	{
		_instance = new GameSystem();
	}
	return _instance;
}

cocos2d::Scene* GameSystem::CreateScene()
{
	GameStartScene* firstScene = GameStartScene::create();
	return firstScene;
}

void GameSystem::GoToXXScene(int index)
{
	switch (index)
	{
	case 0:
	{
		GameStartScene* scene=GameStartScene::create();
		Director::getInstance()->replaceScene(scene);
	}
		break;
	case 1:
	{
		MainGameScene* scene = MainGameScene::create();
		Director::getInstance()->replaceScene(scene);
	}
		break;

	default:
		break;
	}


}
