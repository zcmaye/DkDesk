#include"SJsonData.h"
#include"cJson/cJSON.h"
#include<iostream>

class Type_Error : public std::exception
{
public:
	using std::exception::exception;
	Type_Error(const std::string& msg)
		:exception(msg.c_str()) {}
};

SJsonData::SJsonData()
{
	if(!_root)
		_root = cJSON_CreateObject();
}

SJsonData::SJsonData(SJsonData&& other)
{
	if (this == &other)
		return;
	_root = other._root;
	other._root = nullptr;
}

SJsonData::~SJsonData()
{
	if(_root)
		cJSON_free(_root);
}

void SJsonData::addValue(const std::string& key, int32_t v)
{
	addValue(key, static_cast<double>(v));
}

void SJsonData::addValue(const std::string& key, uint32_t v)
{
	addValue(key, static_cast<double>(v));
}

void SJsonData::addValue(const std::string& key, uint64_t v)
{
	addValue(key, static_cast<double>(v));
}

void SJsonData::addValue(const std::string& key, double v)
{
	auto item = cJSON_GetObjectItem(_root, key.data());
	//key已经存在，直接修改
	if (item)
	{
		cJSON_SetNumberValue(item, v);
		return;
	}
	//key不存在，添加
	cJSON_AddNumberToObject(_root, key.data(),v);
}

void SJsonData::addValue(const std::string& key, bool v)
{
	auto item = cJSON_GetObjectItem(_root, key.data());
	//key已经存在，直接修改
	if (item)
	{
		item->type = v ? cJSON_True : cJSON_False;
		return;
	}
	//key不存在，添加
	cJSON_AddBoolToObject(_root, key.data(), static_cast<cJSON_bool>(v));
}

void SJsonData::addValue(const std::string& key, const std::string& v)
{
	addValue(key, v.data());
}

void SJsonData::addValue(const std::string& key, const char* v)
{
	auto item = cJSON_GetObjectItem(_root, key.data());
	//key已经存在，直接修改
	if (item)
	{
		cJSON_SetValuestring(item, v);
		return;
	}
	//key不存在，添加
	cJSON_AddStringToObject(_root, key.data(), v);
}

void SJsonData::clear()
{
	cJSON_free(_root);				//释放原来的
	_root = cJSON_CreateObject();	//创建新的
}

std::string SJsonData::toJson() const
{
	char* cstr = cJSON_PrintUnformatted(_root);
	std::string json(cstr);
	cJSON_free(cstr);

	return json;
}

bool SJsonData::boolValue(const std::string& key)
{
	auto item = cJSON_GetObjectItem(_root, key.data());
	if (item)
	{
		if (cJSON_IsBool(item))
			return static_cast<bool>(item->type);
		else
			throw Type_Error(std::string("Not a bool type,actual type is an ") + std::to_string(item->type));
	}
	else
	{
		std::cerr << "The key " + key + " was not found!" << std::endl;
		return false;
	}	
}

double SJsonData::numberValue(const std::string& key)
{
	auto item = cJSON_GetObjectItem(_root, key.data());
	if (item)
	{
		if (cJSON_IsNumber(item))
			return cJSON_GetNumberValue(item);
		else
			throw Type_Error(std::string("Not a number type,actual type is an ") + std::to_string(item->type));
	}
	else
	{
		std::cerr << "The key " + key + " was not found!" << std::endl;
		return false;
	}
}

int32_t SJsonData::intValue(const std::string& key)
{
	return static_cast<int32_t>( numberValue(key));
}

std::string SJsonData::stringValue(const std::string& key)
{
	auto item = cJSON_GetObjectItem(_root, key.data());
	if (item)
	{
		if (cJSON_IsString(item))
			return std::string(cJSON_GetStringValue(item));
		else
			throw Type_Error(std::string("Not a string type,actual type is an ") + std::to_string(item->type));
	}
	else
	{
		std::cerr << "The key " + key + " was not found!" << std::endl;
		return std::string();
	}
}

bool SJsonData::isNullValue(const std::string& key)
{
	auto item = cJSON_GetObjectItem(_root, key.data());
	if (item)
	{
		return static_cast<bool>(cJSON_IsNull(item));
	}
	else
	{
		std::cerr << "The key " + key + " was not found!" << std::endl;
		return false;
	}
}

SJsonData SJsonData::fromString(const std::string& json)
{
	return SJsonData(json.data());
}

SJsonData SJsonData::fromString(const char* json)
{
	return SJsonData(json);
}

SJsonData::SJsonData(const char* json)
	:_root(cJSON_Parse(json))
{
	
}
 
#include"cJson/cJSON.c"