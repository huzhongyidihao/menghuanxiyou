/*
œ‘ æÃÿ–ß

*/
#pragma once
#include "cocos2d.h"

class effect :public cocos2d::Sprite
{
public:
	virtual bool init(int typeindex);
	void onEnter();
	void onExit();

	static effect*create();
	//CREATE_FUNC(effect);

public:
	cocos2d::Animate* _showeffect;
private:
	
private:

};