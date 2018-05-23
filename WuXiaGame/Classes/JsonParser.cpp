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
	//data 构建string
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
	doucment.SetObject();//初始化document 对象
	rapidjson::Document::AllocatorType&_allocator = doucment.GetAllocator();//获取内存分配器
	
	//添加数组对象
	rapidjson::Value _array(rapidjson::kArrayType);//定义json数组类型的Value对象
	for (auto&v:_list)
	{
		ValueMap row = v.asValueMap();

		rapidjson::Value object(rapidjson::kObjectType);//定义json对象

		rapidjson::Value v_id;//声明Value类型变量v_id 
		v_id.SetString(row["ID"].asString().c_str()/*从字典取出的ID数据*/, _allocator);//设置v_id字符串,
		object.AddMember("ID", v_id, _allocator);//向object中添加ID数据项

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

	doucment.AddMember("ResultCode", 0, _allocator);//在document 中添加ResultCode数据项
	doucment.AddMember("Record", _array, _allocator); //在document 中添加Record数据项

	rapidjson::StringBuffer buffer;//声明buffer变量,用来接收编码之后的json数据,类型为StringBuffer
	rapidjson::Writer < rapidjson::StringBuffer >writer(buffer);//声明writer对象,通过该对象将数据保存到buffer中.

	doucment.Accept(writer);//文档对象document通过writer对象将数据写到buffer中,这样buffer中保存的数据事实上就是json数据
	auto out = buffer.GetString();//从buffer获取字符串,这个字符串就是编码之后的json字符串

	log("out:%s",out);
	

}

void JsonParser::decode()
{
	rapidjson::Document doucument;//声明文档对象

	doucument.Parse<0>(content.c_str());//将json数据content进行解码.Parse解析过程是对json格式进行有效性验证

	//doucument.HasParseError()判断是否在解析过程中有错误发生
	CCASSERT(!doucument.HasParseError(), "Parsing to document failure.");
	log("Parsing to document succeeded.");

	//
	CC_ASSERT(doucument.IsObject()/*判断document是否是有效的json对象*/ && doucument.HasMember("Record")/*是否有Record数据项*/);

	const rapidjson::Value &records = doucument["Record"];
	//判断是不是数组
	CC_ASSERT(records.IsArray());

	for(unsigned int i =0;i<records.Size();i++)
	{
		row = ValueMap();
		//获得一条记录对象
		const rapidjson::Value& record = records[i];
		//判断这个record 有没有id键
		CC_ASSERT(record.HasMember("ID"));
		const rapidjson::Value&val_id = record["ID"];
		//判断id类型
		CC_ASSERT(val_id.IsString());
		row["id"] = Value(val_id.GetString());

		//判断这个record 有没有CDate键
		CC_ASSERT(record.HasMember("CDate"));
		const rapidjson::Value&val_CDate = record["CDate"];
		//判断CDate类型
		CC_ASSERT(val_CDate.IsString());
		row["CDate"] = Value(val_CDate.GetString());

		//判断这个record 有没有content键
		CC_ASSERT(record.HasMember("Content"));
		const rapidjson::Value&val_Content = record["Content"];
		//判断Content类型
		CC_ASSERT(val_Content.IsString());
		row["Content"] = Value(val_Content.GetString());

		//判断这个record有没有UserID键
		CC_ASSERT(record.HasMember("UserID"));
		const rapidjson::Value&val_UserID = record["UserID"];
		//判断UserID类型
		CC_ASSERT(val_UserID.IsString());
		row["UserID"] = Value(val_UserID.GetString());


		_list.push_back(Value(row));
	}
	
}
