///////////////////////////////////////////
//*  @file    SUDID.h
//*  @date    2023-2-22 19:07:17
//*  @brief   SUDID生成
//*
//*  @author  顿开教育-顽石老师
///////////////////////////////////////////
#ifndef SUDID_H
#define SUDID_H

#include<QString>

class SUDID 
{
public:
    SUDID();   
    inline  operator uint32_t()const { return _udid; }

    static SUDID createUDID();
private:
    uint32_t _udid;
};

#endif // SUDID_H
