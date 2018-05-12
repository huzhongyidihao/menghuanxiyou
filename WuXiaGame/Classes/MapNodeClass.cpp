#include "MapNodeClass.h"
#include "math.h"

#pragma execution_character_set("utf-8")
USING_NS_CC;
int MapNodeClass::endx = 0;
int MapNodeClass::endy = 0;
cocos2d::TMXTiledMap* MapNodeClass::_tilemap=nullptr;
cocos2d::TMXLayer*  MapNodeClass::_tilecollision=nullptr;
MapNodeClass::MapNodeClass(int x,int y, MapNodeClass * obj):_x(x),_y(y), parent(obj),vf(0),vg(0),vh(0),_ispass(true)
{
	initNodeClass();
}

MapNodeClass::~MapNodeClass()
{
}

void MapNodeClass::initNodeClass()
{

	SetGvaule();
	initHvaule();
	SetFvaule();
	InitPass();
}

void MapNodeClass::UpdateFvaule(MapNodeClass*currpoint)
{
	auto currvg=currpoint->getGvaule();//获取当前点的G
	bool ret = IsAnglesRelation(currpoint);
	if (ret)
	{
		if ((currvg + slashcost) <vg)//如果新路径的G小于当前G
		{
			parent = currpoint;//那么设置父节点parent 为当前节点
			vg = currvg + slashcost;
		}
	}
	else
	{
		if ((currvg + linecost) < vg)//如果新路径的G小于当前G
		{
			parent = currpoint;//那么设置父节点parent 为当前节点
			vg = currvg + linecost;
		}
	}
	vf = vg + vh;

}

void MapNodeClass::setParent(MapNodeClass*obj)
{
	parent = obj;
}

void MapNodeClass::InitEndPoint(int x,int y)
{
	endx = x;
	endy = y;
}

//void MapNodeClass::InitTMXMapandLayer(cocos2d::TMXTiledMap*map,cocos2d::TMXLayer * layer)
//{
//	_tilemap = map;
//	_tilecollision = layer;
//}


bool MapNodeClass::IsAnglesRelation(MapNodeClass * targetpoint)
{
	if (abs(getXvaule()-targetpoint->getXvaule())==1&&abs(getYvaule()-targetpoint->getYvaule())==1)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void MapNodeClass::SetFvaule()
{
	vf = vg + vh;
}

void MapNodeClass::SetGvaule()
{
	if (parent)
	{
		if (parent->getXvaule() != _x&&parent->getYvaule() != _y)//都不相等则说明本节点与parent在斜线上
		{
			vg = parent->getGvaule() + slashcost;
		}
		else
		{
			vg = parent->getGvaule() + linecost;
		}
	}
	else
	{
		vg = 0;
	}
}

void MapNodeClass::initHvaule()
{

	//vh = sqrt((double)(endx -_x)*(double)(endx - _x) + (double)(endy - _y)*(double)(endy - _y))*linecost;
	vh = (abs(endx - _x) + abs(endy - _y))*linecost;

}

void MapNodeClass::InitPass()
{
	if (_tilecollision)//如果存在
	{
		int tilegid = _tilecollision->getTileGIDAt(Vec2(_x, _y));
	//	log("GID=%d", tilegid);
		if (tilegid)
		{
			Value ret = _tilemap->getPropertiesForGID(tilegid);
			ValueMap temp = ret.asValueMap();
			std::string vaule = temp.at("donenter").asString();
			if (vaule.compare("true") == 0)
			{
				_ispass = false;
				
			}
		}
		else
		{
			_ispass = true;
			log("节点(%d,%d)可以通行",_x,_y);
		}
	}
}
