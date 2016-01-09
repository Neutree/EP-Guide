#include "Communicate.h"


/**************************协议**********************************/

char Communicate::mToServerPack[50]={0xA4,0x02,};

		

/////////////////////////
///记录消息ID号（递增）
/////////////////////////
uint16_t Communicate::ToServerGenerateMessageID()
{
	static uint16_t mToServerMessagePackID=0;
	return ++mToServerMessagePackID;
}



/**************************通信***************************************/
////////////////////////////
///发送一个字节到服务器
///@param wifi wifi对象
///@param ipAddress 服务器IP地址
///@param port 服务器端口
///@param data 需要发送的一个字节的数据
///@param 是否发送成功（对方不一定接收到，只是发送成功）
////////////////////////////
bool Communicate::SendBytesToServer(esp8266 & wifi,char* ipAddress,uint32_t port,char* data,uint32_t len)
{
	if(!wifi.createTCPMutipleMode(4,ipAddress,port))//建立连接失败，关闭4号连接，重新建立，失败则返回失败信息
	{
		wifi.CloseMulitpleSend(4);
		if(!wifi.createTCPMutipleMode(4,ipAddress,port))
			return false;
	}
	wifi.sendMultipleMode(4,data,len);
	wifi.CloseMulitpleSend(4);
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


