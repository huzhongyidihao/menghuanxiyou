/*
地图节点类:
一张地图有N个节点组成,这游戏载入瓦片地图都是正方形 故节点也是正方形
主要用于自动寻路
存储有当前MainLayer 所载入地图的信息
包括：尺寸 可通行范围
*/
#pragma once
#include "cocos2d.h"
class TMXTiledMap;
class TMXLayer;

class MapNodeClass
{


public:
	MapNodeClass(int x=0,int y=0 , MapNodeClass* obj=nullptr);
	~MapNodeClass();

	enum 
	{
		UnSelect=0,
		HaveSelect,

	}BelongArr;

	void initNodeClass();
public:
	int getFvaule() { return vf; }
	int getGvaule() { return vg; }
	int getXvaule() { return _x; }
	int getYvaule() { return _y; }
	MapNodeClass* getParent() { return parent; }

	bool getIsPass() { return _ispass; }
	void setParent(MapNodeClass*obj);
	void InitEndPoint(int x,int y);//初始化终点坐标
	static void InitTMXMapandLayer(cocos2d::TMXTiledMap*map, cocos2d::TMXLayer*layer)
	{
		_tilemap = map;
		_tilecollision = layer;
	}//初始化地图指针和碰撞层指针
	void UpdateFvaule(MapNodeClass*currpoint);//设置新的当前点之后更新g值

public:

private:
	bool IsAnglesRelation(MapNodeClass*targetpoint);//相邻节点间是否为对角关系
	void SetFvaule();
	void SetGvaule();
	void initHvaule();
	void InitPass();//设置通行性
	
private:
	const int linecost = 10;//走一步直线代价
	const int slashcost = 14;//走一步斜线代价
	MapNodeClass* parent;

	bool _ispass;//可通行性
	int vf;
	int vg;
	int vh;
	int _x;
	int _y;
	static int endx;
	static int endy;


	static cocos2d::TMXTiledMap*_tilemap;
	static cocos2d::TMXLayer* _tilecollision;
};