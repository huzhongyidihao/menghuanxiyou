#pragma once
#include "cocos2d.h"


#include "document.h"
#include "writer.h"
#include "reader.h"
#include "stringbuffer.h"


class JsonParser :public cocos2d::Ref
{

	//声明返回listData数据
	CC_SYNTHESIZE(cocos2d::ValueVector, _list, List);
private:
	cocos2d::ValueMap row;
	//json 文件内容
	std::string content;
public:
	static JsonParser*createWithFile(const char*fileName);
	static JsonParser*createWithArray(cocos2d::ValueVector pListData);
	bool initWithFile(const char *JSONName);
	bool initWithArray(cocos2d::ValueVector pListData);
	void encode();//编码函数
	void decode();//解码函数
private:
	
};

