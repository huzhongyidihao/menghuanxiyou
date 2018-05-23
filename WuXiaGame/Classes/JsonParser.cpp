#include "JsonParser.h"
USING_NS_CC;

JsonParser * JsonParser::createWithFile(const char * fileName)
{
	JsonParser*obj = new JsonParser();
	if (obj->initWithFile(fileName))
	{
		obj->autorelease();
		return obj;
	}
	else
	{
		delete obj;
		obj = nullptr;
		return nullptr;
	}
	
}

JsonParser * JsonParser::createWithArray(cocos2d::ValueVector pListData)
{
	JsonParser*obj = new JsonParser();
	if (obj->initWithArray(pListData))
	{
		obj->autorelease();
		return obj;
	}
	else
	{
		delete obj;
		obj = nullptr;
		return nullptr;
	}
}

bool JsonParser::initWithFile(const char * JSONName)
{
	auto sharedFileUtils = FileUtils::getInstance();
	std::string path = sharedFileUtils->fullPathForFilename(JSONName);

	Data data = sharedFileUtils->getDataFromFile(path);
	//data ����string
	content = std::string((const char*)data.getBytes(),0,data.getSize());


	return true;
}

bool JsonParser::initWithArray(cocos2d::ValueVector pListData)
{
	this->_list = pListData;
	return true;
}

void JsonParser::encode()
{
	rapidjson::Document doucment;
	doucment.SetObject();//��ʼ��document ����
	rapidjson::Document::AllocatorType&_allocator = doucment.GetAllocator();//��ȡ�ڴ������
	
	//����������
	rapidjson::Value _array(rapidjson::kArrayType);//����json�������͵�Value����
	for (auto&v:_list)
	{
		ValueMap row = v.asValueMap();

		rapidjson::Value object(rapidjson::kObjectType);//����json����

		rapidjson::Value v_id;//����Value���ͱ���v_id 
		v_id.SetString(row["ID"].asString().c_str()/*���ֵ�ȡ����ID����*/, _allocator);//����v_id�ַ���,
		object.AddMember("ID", v_id, _allocator);//��object�����ID������

		rapidjson::Value v_Date;
		v_Date.SetString(row["CDate"].asString().c_str(), _allocator);
		object.AddMember("CDate", v_Date, _allocator);

		rapidjson::Value v_Content;
		v_Content.SetString(row["Content"].asString().c_str(), _allocator);
		object.AddMember("Content", v_Content, _allocator);

		rapidjson::Value v_UserID;
		v_UserID.SetString(row["UserID"].asString().c_str(), _allocator);
		object.AddMember("UserID", v_UserID, _allocator);

		_array.PushBack(object, _allocator);

	}

	doucment.AddMember("ResultCode", 0, _allocator);//��document �����ResultCode������
	doucment.AddMember("Record", _array, _allocator); //��document �����Record������

	rapidjson::StringBuffer buffer;//����buffer����,�������ձ���֮���json����,����ΪStringBuffer
	rapidjson::Writer < rapidjson::StringBuffer >writer(buffer);//����writer����,ͨ���ö������ݱ��浽buffer��.

	doucment.Accept(writer);//�ĵ�����documentͨ��writer��������д��buffer��,����buffer�б����������ʵ�Ͼ���json����
	auto out = buffer.GetString();//��buffer��ȡ�ַ���,����ַ������Ǳ���֮���json�ַ���

	log("out:%s",out);
	

}

void JsonParser::decode()
{
	rapidjson::Document doucument;//�����ĵ�����

	doucument.Parse<0>(content.c_str());//��json����content���н���.Parse���������Ƕ�json��ʽ������Ч����֤

	//doucument.HasParseError()�ж��Ƿ��ڽ����������д�����
	CCASSERT(!doucument.HasParseError(), "Parsing to document failure.");
	log("Parsing to document succeeded.");

	//
	CC_ASSERT(doucument.IsObject()/*�ж�document�Ƿ�����Ч��json����*/ && doucument.HasMember("Record")/*�Ƿ���Record������*/);

	const rapidjson::Value &records = doucument["Record"];
	//�ж��ǲ�������
	CC_ASSERT(records.IsArray());

	for(unsigned int i =0;i<records.Size();i++)
	{
		row = ValueMap();
		//���һ����¼����
		const rapidjson::Value& record = records[i];
		//�ж����record ��û��id��
		CC_ASSERT(record.HasMember("ID"));
		const rapidjson::Value&val_id = record["ID"];
		//�ж�id����
		CC_ASSERT(val_id.IsString());
		row["id"] = Value(val_id.GetString());

		//�ж����record ��û��CDate��
		CC_ASSERT(record.HasMember("CDate"));
		const rapidjson::Value&val_CDate = record["CDate"];
		//�ж�CDate����
		CC_ASSERT(val_CDate.IsString());
		row["CDate"] = Value(val_CDate.GetString());

		//�ж����record ��û��content��
		CC_ASSERT(record.HasMember("Content"));
		const rapidjson::Value&val_Content = record["Content"];
		//�ж�Content����
		CC_ASSERT(val_Content.IsString());
		row["Content"] = Value(val_Content.GetString());

		//�ж����record��û��UserID��
		CC_ASSERT(record.HasMember("UserID"));
		const rapidjson::Value&val_UserID = record["UserID"];
		//�ж�UserID����
		CC_ASSERT(val_UserID.IsString());
		row["UserID"] = Value(val_UserID.GetString());


		_list.push_back(Value(row));
	}
	
}
