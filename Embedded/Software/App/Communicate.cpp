#include "Communicate.h"






////////////////////////////
///发送一个字节到服务器
///@param wifi wifi对象
///@param ipAddress 服务器IP地址
///@param port 服务器端口
///@param data 需要发送的一个字节的数据
///@param 是否发送成功（对方不一定接收到，只是发送成功）
////////////////////////////
bool Communicate::SendBytesToServer(esp8266 & wifi,char* ipAddress,uint32_t port,char data)
{
	return false;
}


//////////////////////////////
///发送http请求到服务器
///@param wifi wifi对象
///@param ipAddress 服务器IP地址
///@param port 服务器端口
///@param data 请求内容
///@param 是否发送成功（对方不一定接收到，只是发送成功）
/////////////////////////////
bool Communicate::HttpRequest(esp8266 & wifi,char* ipAddress,uint32_t port,char* data)
{
	return false;
}


