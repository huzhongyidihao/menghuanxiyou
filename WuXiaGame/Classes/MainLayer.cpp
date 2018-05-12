#include "MainLayer.h"
#include "Role.h"
#include "GameRuleAbout.h"
#include "SimpleAudioEngine.h"
#include "GameStartScene.h"
#include "PlayInfo.h"
USING_NS_CC;
using namespace  CocosDenshion;
#pragma execution_character_set("utf-8")
bool MainLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	log("origin=(%f,%f)",origin.x,origin.y);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	log("visibleSize: width=%f,height=%f", visibleSize.width, visibleSize.height);
	Size currwinSize = Director::getInstance()->getWinSize();
	log("CurrWinSize: width=%f,height=%f", currwinSize.width, currwinSize.height);
	Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	log("FrameSize: width=%f,height=%f", frameSize.width, frameSize.height);
	Rect visrect = Director::getInstance()->getOpenGLView()->getVisibleRect();





    _tilebaselayer = nullptr;
    _tileblockage = nullptr;


	//加载地图
///	std::string file = "map/citymap01over.tmx";
	//String* str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(file.c_str()).c_str());
//	std::string path = FileUtils::getInstance()->fullPathForFilename(file.c_str());
//	_tilemap = TMXTiledMap::create(path);
	_tilemap = TMXTiledMap::create("map/citymap01over.tmx");
	if (_tilemap!=nullptr)
	{

		Size  s = _tilemap->getContentSize();
		CCLOG("ContentSize: %f, %f", s.width, s.height);
	//	log("ContentSize: %f, %f", s.width, s.height);
		
		Size size = _tilemap->getTileSize();
	//	log("TileSize:%f,%f", size.width, size.height);
		CCLOG("TileSize:%f,%f", size.width, size.height);
		Size size2 = _tilemap->getMapSize();
	//	log("MapSize:%f,%f", size2.width, size2.height);
		CCLOG("MapSize:%f,%f", size2.width, size2.height);
		auto num = _tilemap->getLayerNum();
		log("layer数目=%d",num);

	}

	_tileobjgroup = _tilemap->getObjectGroup("object");
	_tileblockage = _tilemap->getLayer("collision");
	_tileblockage->setVisible(false);
	_tilebaselayer = _tilemap->getLayer("basemap");

	_tilefoe = _tilemap->getLayer("foelayer");
	_tilefoe->setVisible(false);

	_tilemap->setAnchorPoint(Vec2::ZERO);
	_tilemap->setPosition(Vec2(576,2368));
	this->addChild(_tilemap,0);

    Sprite* pictemp = Sprite::create("pic/yy.png");
    pictemp->setAnchorPoint(Vec2::ZERO);
    pictemp->setPosition(Vec2::ZERO);
    this->addChild(pictemp,0);


	//创建主角
	_role = new Role();//创建主角
	if (!GameStartScene::_playinfoSave)//判断条件为如果没有有存档
	{
		auto ret = _tileobjgroup->getObject("startpoint");
		float x = ret["x"].asFloat();
		float y = ret["y"].asFloat();
		_role->setPosition(Vec2(x, y));
		_tilemap->addChild(_role->_roleBasePic, 5);
		_role->_roleBasePic->setTag(9);
		ViewfollowWithRole(0, Point(x, y));//视角回归


      //  pictemp->setPosition(Vec2(x, y));
	}
	else
	{
		  //用当前存档同步新建的角色
		GameStartScene::_playinfoSave->SynInfo(_role);
		_role->setPosition(_role->GetStartPoint());
		_tilemap->addChild(_role->_roleBasePic, 5);
		_role->_roleBasePic->setTag(9);
		ViewfollowWithRole(0, _role->GetStartPoint());//视角回归

	
		//delete GameStartScene::_playinfoSave;
		
	}	

	scheduleUpdate();

	//设置自动寻路终点
	_pathend =Vec2(_tileobjgroup->getObject("endpoint")["x"].asFloat(),_tileobjgroup->getObject("endpoint")["y"].asFloat());
	log("终点设置为:(%f,%f)",_pathend.x,_pathend.y);

	/*其他参数*/
	IsAutoFindPath = false;
	applyEnterCombat = false;
	_maxfoenums = 3;
	_currfoenums = 0;

	_cruuCombatFoe = nullptr;


	return true;
}

void MainLayer::onEnter()
{
	Layer::onEnter();
	log("MainLaye onEnter!");
	//注册监听器
	//setTouchEnabled(true);
	//setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(MainLayer::onTouchBegan, this);
	
	listener->onTouchMoved = CC_CALLBACK_2(MainLayer::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(MainLayer::onTouchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(MainLayer::onTouchEnded, this);
	eventdispatcher = Director::getInstance()->getEventDispatcher();
	eventdispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//log("添加单点监听器");

	//地图BMG


	auto temp = _mapbgmstr.c_str();
	//log("%s",temp);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(temp);

	//初始化地图信息
	InitCurrMapInfo();
	InitFoeArea();
	//随机创建foe
	RandomCreateFoe();//创建一个
}

void MainLayer::onExit()
{
	Layer::onExit();
	log("MainLaye onExit!");
	if (_role!=nullptr)
	{
		log("如果_role 还存在");
		log("那么久释放");
		delete _role;
		_role = nullptr;
	}

}

void MainLayer::update(float delta)
{
	//if (_role->getRoleState()==1)//移动时
	//{
	//	ViewfollowWithRole(convertToWorldSpace(_role->getPosition()));
	//}
	/*if (IsAutoFindPath)
	{

	}*/
	if (!applyEnterCombat)
	{
		if (IscollisionRole(_role))//如果返回不为空,即碰到了敌人
		{

			//申请进入战斗场景
			applyEnterCombat = true;
			//终止当前角色所有动作
			_role->_roleBasePic->stopAllActions();
			this->stopAllActions();
		}
	}
	
	//
	RandomCreateFoe();

 
}

void MainLayer::ViewfollowWithRole(float time,cocos2d::Point position)
{
	/*视图始终跟随玩家角色,并置其为中央*/
	auto winSize = Director::getInstance()->getVisibleSize();
//	log("-------winSize%f-------\n",winSize);
	float x = MAX(position.x, winSize.width / 2);//屏幕向左移动的极限
	float y = MAX(position.y, winSize.height / 2);//屏幕向下移动的极限
	x = MIN(x,(_tilemap->getMapSize().width*_tilemap->getTileSize().width-winSize.width/2));//屏幕向右移动的极限
	y = MIN(y, (_tilemap->getMapSize().height*_tilemap->getTileSize().height - winSize.height / 2));//屏幕向上移动的极限
	auto actualPosition = Point(x, y);//屏幕中点应该转到的位置
	auto centerOfView = _tilemap->convertToNodeSpace(Vec2(winSize.width / 2, winSize.height / 2));//当前屏幕中点所在坐标(相对于地图的节点坐标)
	auto offset = centerOfView - actualPosition;
	runAction(MoveBy::create(time, offset));
}

void MainLayer::EnterCombat()
{
	IsCombat = true;
	HidAllRoleAndNPC();
	unscheduleUpdate();
	log("进入战斗");
}

void MainLayer::ExitCombat()
{
	IsCombat = false;
	ShowALLRoleAndNPC();
	scheduleUpdate();
	log("结束战斗");
}

bool MainLayer::onTouchBegan(Touch * touch, Event * unused_event)
{
	log("onTouchBegan  事件");
	return true;
}

void MainLayer::onTouchCancelled(Touch * touch, Event * unused_event)
{
	log("onTouchCancelled  事件");
}

void MainLayer::onTouchMoved(Touch * touch, Event * unused_event)
{
	log("onTouchMoved  事件");
}

void MainLayer::onTouchEnded(Touch * touch, Event * unused_event)
{
//	log("onTouchEnded  事件");
//	log("触摸点的世界坐标坐标=(%f,%f)", touch->getLocation().x, touch->getLocation().y);
	Vec2 touchLocation = touch->getLocation();
	Vec2 nodeLocation = _tilemap->convertToNodeSpace(touchLocation);//转换为节点坐标
//	log("触摸点相对地图的节点坐标=(%f,%f)", nodeLocation.x, nodeLocation.y);
//	log("角色相对地图的节点坐标=(%f,%f)", _role->getPosition().x, _role->getPosition().y);
//	log("角色的世界坐标=(%f,%f)",convertToWorldSpace(_role->getPosition()).x, convertToWorldSpace(_role->getPosition()).y);
	auto winSize = Director::getInstance()->getVisibleSize();
	Vec2 centerpoint = _tilemap->convertToNodeSpace(Vec2(winSize.width / 2, winSize.height / 2));
//	log("屏幕中点相对地图的节点坐标=(%f,%f)", centerpoint.x, centerpoint.y);

	//
	auto oldDirection = _role->getRoleDirection();//存储旧的朝向
	//设置朝向
	if (_role->getPosition().x<nodeLocation.x)//如果角色x坐标小于触摸点x坐标(角色获取的坐标也是节点坐标，因为它是这个瓦片地图节点的子对象)
	{
	
		////镜像反转
		//if (!_role->_roleBasePic->isFlippedX())
		//{
		//	_role->_roleBasePic->setFlippedX(true);
		//}
		//else
		//{
		//	if (_role->_roleBasePic->isFlippedX())
		//	{
		//		_role->_roleBasePic->setFlippedX(false);
		//	}
		//}		
		if (_role->getPosition().y>nodeLocation.y)
		{
			//即往右下角方向:东
			_role->setRoleDirection(0);
		}

		else
		{
			_role->setRoleDirection(2);//北
		}

	}
	else
	{
		if (_role->getPosition().y>nodeLocation.y)
		{
			_role->setRoleDirection(1);//南
		}
		else
		{
			_role->setRoleDirection(3);//西
		}
	}
	//获得目标点所在的瓦片位置
	Vec2 mappoint = ConvertPositionInMap(nodeLocation);
//	log("目标所在瓦片的x位置=(%f,%f)", mappoint.x,mappoint.y);

	//通过瓦片位置获得此瓦片的GID
	int tilegid = _tileblockage->getTileGIDAt(mappoint);
	//log("tilegid=%d", tilegid);
	if (tilegid)
	{	
		Value ret = _tilemap->getPropertiesForGID(tilegid);
	
		ValueMap temp = ret.asValueMap();
		std::string vaule = temp.at("donenter").asString();
		if (vaule.compare("true")==0)
		{
			log("我还无法移动到哪里!");
		}
		
	}
	else
	{
		//如果不存在 则可以移动
		if (abs(nodeLocation.x - _role->getPosition().x) > _tilemap->getTileSize().width*0.7)
		{
			if (_role->getRoleState() == 0)
			{
				auto temp = CountMoveTime(_role->getPosition(), nodeLocation, _role->getRoleSpeed());
				_role->setRoleState(1);//设置状态为 移动			
				_role->MoveAnimation(temp, nodeLocation);
				ViewfollowWithRole(temp, nodeLocation);
			}
			else  //否则就是移动状态 (忽略战斗状态)
			{
				
				auto temp = CountMoveTime(_role->getPosition(), nodeLocation, _role->getRoleSpeed());
				if (oldDirection == _role->getRoleDirection())//如果新方向和旧方向一致
				{
					this->stopAllActions();
					_role->MoveAnimation(temp, nodeLocation);//根据新的方向重新移动
					ViewfollowWithRole(temp, nodeLocation);
					log("角色动作不会因新目标而停止");
				}
				else
				{
					//停止当前动作 (包括动画动作和移动动作)
					_role->_roleBasePic->stopAllActions();
					this->stopAllActions();
					_role->MoveAnimation(temp, nodeLocation);//根据新的方向重新移动
					ViewfollowWithRole(temp, nodeLocation);
				}
				
			}

		}
		else
		{
			log("太近了，懒得动~~~~");
		}
	}

	

}





float MainLayer::CountMoveTime(Vec2 rolepoint,Vec2 targetpoint,float speed)
{

	Vec2 temp = targetpoint - rolepoint;
	float dis=rolepoint.distance(targetpoint);
	//log("距离dis=%f", dis);
	float timevalue = dis / (_tilemap->getTileSize().width*_role->getRoleSpeed());
//	log("tilesize.width=%f", _tilemap->getTileSize().width);
//	log("时间timevalue=%f", timevalue);
	return timevalue;
}

void MainLayer::HidAllRoleAndNPC()
{
	log("隐藏所有角色和npc");
	auto temp = this->_tilemap->getChildByTag(9);
	log("-----");
	temp->setVisible(false);
	for (auto &w: _foeCollection)
	{
		w->_roleBasePic->setVisible(false);
	}


}

void MainLayer::ShowALLRoleAndNPC()
{
	log("显示所有角色和npc");
	auto temp = this->_tilemap->getChildByTag(9);

	temp->setVisible(true);
	for (auto &w : _foeCollection)
	{
		w->_roleBasePic->setVisible(true);
	}
}

void MainLayer::setBGMstring(std::string str)
{
	_mapbgmstr = str;
}

void MainLayer::CreateFoe(Vec2 point,int typeindex)
{
	Role* obj = new Role();
	obj->setPosition(point);
	_tilemap->addChild(obj->_roleBasePic, 5);

	_foeCollection.push_back(obj);
	_currfoenums=_foeCollection.size();
	_currfoenums++;
	//创建一个敌人foe
}

bool MainLayer::IsCanCreateFoe()
{
	if (_currfoenums<_maxfoenums)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

cocos2d::Vec2 MainLayer::RandomCreateFoePoistion()
{
	
	srand((unsigned)time(nullptr));
	int length=_foeArea.size();
	int index=random(0,length-1);
	Vec2 temppoint = _foeArea[index];
	Vec2 point=ConvertPositionFromMapVec(temppoint);
	return point;
}

void MainLayer::InitFoeArea()
{
	int x=_tilemap->getMapSize().width;
	int y = _tilemap->getMapSize().height;
	for (int i=0;i<x;i++)
	{
		for (int k=0;k<y;k++)
		{
			int tilegid=_tilefoe->getTileGIDAt(Vec2(i, k));

			if (tilegid)
			{
			//	log("tilegid=%d", tilegid);
				Value ret=_tilemap->getPropertiesForGID(tilegid);
				ValueMap temp =ret.asValueMap();
				std::string str=temp["createfoe"].asString();
				if (str=="true")
				{
					_foeArea.push_back(Vec2(i,k));
				}
			}

		}
	}
	
}

Role * MainLayer::IscollisionRole(Role * obj)
{
	//由于角色精灵尺寸一致，故只要角色坐标距离d够短 则可以判断为碰撞,d可以设置为角色精灵尺寸的一半
	Size rolesize = obj->_roleBasePic->getContentSize();
	//log("角色尺寸为:width=%f,height=%f", rolesize.width, rolesize.height);
	//设置判断距离为尺寸中线 即约为1.4倍的宽或者高
	Role*temp = nullptr;
	for (auto &w:_foeCollection)
	{
		if ((w->getPosition().distance(obj->getPosition()))<=1.4*rolesize.width)
		{
			log("碰撞到了怪物,进入战斗!");
			temp = w;
			_cruuCombatFoe = w;
			break;
		}
	}
	return temp;
}

void MainLayer::autoMovePath(std::deque<cocos2d::Vec2>way,cocos2d::Vec2 endpoint)
{
	std::vector<cocos2d::Vec2> moveactionPoint;//移动动作端点
	//将所得路径拆分成直线 并分段执行移动动作
	Vec2 start = way[0];
	Vec2 temp=start;
	int pointtype=0;
	float dtx=way[1].x-way[0].x;
	float dty=way[1].y-way[0].y;
	for(int i=1;i<way.size();i++)
	{
		float tempx = way[i].x - way[i - 1].x;
		float tempy = way[i].y - way[i-1].y;
		if (dtx==tempx&&dty==tempy)
		{
			temp = way[i];
		}
		else
		{
			moveactionPoint.push_back(temp);
			dtx = tempx;
			dty = tempy;
			temp = way[i];
		}
	}
	//循环结束后temp就是最后一个点
	moveactionPoint.push_back(temp);
	//至此 序列中所有点加上终点就是我们移动所需设置的目标点参数

	Vec2 rolestart = _role->getPosition();

	Vector<FiniteTimeAction*>moveVector;
	Vector<FiniteTimeAction*>spawnVector;
	Vec2 fontpoint= rolestart;//初始化 "前面的" 坐标为起点
	for (int i=0;i<moveactionPoint.size();i++)
	{
			Vec2 temp = ConvertPositionFromMapVec(moveactionPoint[i]);
			float time = CountMoveTime(fontpoint, temp, _role->getRoleSpeed());
			auto moveaction=MoveTo::create(time, temp);
			auto callfun = CallFunc::create([&,time,temp]()
			{
				ViewfollowWithRole(time, temp);
				
			});
			moveVector.pushBack(moveaction);
			Spawn*spawnaction = Spawn::create(moveaction, callfun, nullptr);
			spawnVector.pushBack(spawnaction);
			fontpoint = temp;//更新fontpoint 		
	}
	//添加最后一段路

	float lasttime = CountMoveTime(fontpoint, endpoint, _role->getRoleSpeed());
	auto callfun = CallFunc::create([&, lasttime, endpoint]()
	{
		ViewfollowWithRole(lasttime, endpoint);

	});
	Spawn*spawnaction = Spawn::create(MoveTo::create(lasttime, endpoint), callfun, nullptr);
	//moveVector.pushBack(MoveTo::create(lasttime, endpoint));
	spawnVector.pushBack(spawnaction);


	//至此 移动动作全部添加完毕
	//auto seqaction=Sequence::create(moveVector);
	auto seqaction = Sequence::create(spawnVector);
	_role->_roleBasePic->runAction(seqaction);
}
void MainLayer::autoFindPath()
{
	IsAutoFindPath = true;
	//先转化为地图瓦片坐标
	auto start=ConvertPositionInMap(_role->getPosition());
	auto end = ConvertPositionInMap(_pathend);

	auto temp=GameSuanFaClass::GetInstance()->MinShortWayFind(start,end, _tilemap, _tileblockage);//起点和终点应该是瓦片坐标
	for(auto &w:temp)
	{
		log("路径点(%f,%f)",w.x,w.y);//打印坐标
		Sprite* obj=_tilebaselayer->getTileAt(w);
		obj->setColor(Color3B(220,170,150));
	}
	


	//移动
	autoMovePath(temp,_pathend);
}

void MainLayer::RandomCreateFoe(int index)
{
	//判断是否能生成 
	if (IsCanCreateFoe())
	{
		//获取生成坐标
		Vec2 point = RandomCreateFoePoistion();
		//生成
		CreateFoe(point, index);
	}
	



}

void MainLayer::ClearTargetRole(Role * obj)
{
	
	auto target = find(_foeCollection.begin(),_foeCollection.end(),obj);
	_tilemap->removeChild((*target)->_roleBasePic);
	if (target!=_foeCollection.end())
	{
		log("释放foe内存");
		delete *target;
		_foeCollection.erase(target);
		_currfoenums--;
	}


}

void MainLayer::InitCurrMapInfo()
{
	Size  s = _tilemap->getContentSize();
	log("地图内容尺寸: %f, %f", s.width, s.height);

	Size size = _tilemap->getTileSize();
	log("地图瓦片尺寸:%f,%f", size.width, size.height);

	Size size2 = _tilemap->getMapSize();
	log("地图尺寸:%f,%f", size2.width, size2.height);
	auto num = _tilemap->getLayerNum();
	log("layer数目=%d", num);




}

Vec2 MainLayer::ConvertPositionInMap(Vec2 point)
{
	//point 是相对于地图的节点坐标(原点仍在左下),转换的结果是地图坐标(原点在左上)
	//int x =point.x / (_tilemap->getTileSize().width/2);
	//if ((x+1)%2==0)//如果是奇数
	//{
	//	x = (x + 1) / 2;
	//}
	//else
	//{
	//	x = x / 2;
	//}
	//int y = point.y / (_tilemap->getTileSize().height / 2);
	//if ((y + 1) % 2 == 0)//如果是奇数
	//{
	//	y =_tilemap->getMapSize().height -(y + 1) / 2;
	//}
	//else
	//{
	//	y = _tilemap->getMapSize().height - y / 2;
	//}

	//
	int x = point.x;
	int y = point.y;
	int tilewidth = _tilemap->getTileSize().width;
	int tileheight = _tilemap->getTileSize().height;
	
	if (x>= tilewidth)
	{
		if (x%tilewidth == 0)
		{
			 x = (x/tilewidth)-1;
		}
		else
		{
			 x = (x/tilewidth) ;
		}
	}
	else
	{
		x = 0;
	}
	if (y>= tileheight)
	{
		if (y%tileheight == 0)
		{
			 y = _tilemap->getMapSize().height -  y/tileheight;
		}
		else
		{
			 y = _tilemap->getMapSize().height-(y / tileheight + 1);
		}
	}
	else
	{
		y = _tilemap->getMapSize().height;
	}




	Vec2 v = Vec2(x, y);
	log("返回的=Vec(%d,%d)", x, y);
	return v;
}

cocos2d::Vec2 MainLayer::ConvertPositionFromMapVec(cocos2d::Vec2 point)
{
	auto tilewidth = _tilemap->getTileSize().width;
	auto tileheigth = _tilemap->getTileSize().height;
	float x = point.x*tilewidth;
	float y = _tilemap->getTileSize().height*_tilemap->getMapSize().height- point.y*tileheigth;


	return cocos2d::Vec2(x,y);
}
