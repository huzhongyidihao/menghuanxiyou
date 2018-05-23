#pragma once
#include "cocos2d.h"


#include "document.h"
#include "writer.h"
#include "reader.h"
#include "stringbuffer.h"


class JsonParser :public cocos2d::Ref
{

	//��������listData����
	CC_SYNTHESIZE(cocos2d::ValueVector, _list, List);
private:
	cocos2d::ValueMap row;
	//json �ļ�����
	std::string content;
public:
	static JsonParser*createWithFile(const char*fileName);
	static JsonParser*createWithArray(cocos2d::ValueVector pListData);
	bool initWithFile(const char *JSONName);
	bool initWithArray(cocos2d::ValueVector pListData);
	void encode();//���뺯��
	void decode();//���뺯��
private:
	
};

