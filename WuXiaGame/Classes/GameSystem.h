/*
游戏系统类

*/

#pragma once
#include "cocos2d.h"

class GameSystem
{
public:
	static GameSystem* GetInstance();
	static cocos2d::Scene*CreateScene();
	void GoToXXScene(int );

private:
	GameSystem() {}
	GameSystem(const GameSystem&);
	GameSystem& operator=(const GameSystem&);
private:
	static  GameSystem*_instance;

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
		}
		static GC gc;
	};
};

