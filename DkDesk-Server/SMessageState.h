/*****************************************************************//**
 * \file   SMessageState.h
 * \brief
 *
 * \author Maye - 顽石老师
 * \date   February 2023
 *********************************************************************/

#ifndef SMESSAGESTATE_H_
#define SMESSAGESTATE_H_
#include<cstdint>
/*@消息状态*/
enum MsgState : int32_t
{
	Ok = 200,		//成功
	Created = 201,	//新的资源创建
	NotFound		//设备不存在或者不在线
};

/*@消息类型*/
enum  MsgType : int32_t
{
	//服务器
	UdidCreated,	//UDID创建请求
	UserLogin,		//用户登录请求
	UserRegister,	//用户注册请求
	ConnectBuddy,	//连接伙伴请求

	//服务器 ->客户端
	Request_Connect,	//有伙伴请求连接，是否接受(服务器会把应该绑定的IP地址和端口号发送过去)

	//客户端->服务器->客户端
	Ready_Connect,		//如果伙伴同意连接，发送给服务器，服务器转发给客户端
};


#endif // !SMESSAGESTATE_H_
