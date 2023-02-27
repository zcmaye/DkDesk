///////////////////////////////////////////
//*  @file    SJsonData.h
//*  @date    2023-2-22 19:07:17
//*  @brief   Json数据生成
//*
//*  @author  顿开教育-顽石老师
///////////////////////////////////////////
#ifndef SJSONDATA_H
#define SJSONDATA_H

#include<string>
struct cJSON;
class SJsonData 
{
public:
    SJsonData();
    SJsonData(const SJsonData&) = delete;
    SJsonData(SJsonData&& other);
    ~SJsonData();
    void addValue(const std::string& key, int32_t v);
    void addValue(const std::string& key, uint32_t v);
    void addValue(const std::string& key, uint64_t v);
    void addValue(const std::string& key, double v);
    void addValue(const std::string& key, bool v);
    void addValue(const std::string& key, const std::string& v);
    void addValue(const std::string& key, const char* v);
    void clear();
    //待扩充添加数据
    std::string toJson()const;


    bool boolValue(const std::string& key);
    double numberValue(const std::string& key);
    int32_t intValue(const std::string& key);
    std::string stringValue(const std::string& key);
    bool isNullValue(const std::string& key);

public:
    static SJsonData fromString(const std::string& json);
    static SJsonData fromString(const char* json);
private:
    explicit SJsonData(const char* json);
    cJSON* _root = nullptr;
};

#endif // SJSONDATA_H
