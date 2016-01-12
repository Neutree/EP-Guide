#ifndef __WIFI_H
#define __WIFI_H

#include "esp8266.h"


class WIFI
{
public:
	/***************WIFI相关操作**********************/
		static bool Init(esp8266 &wifi);
	
public:
	/********************常量*************************/
	//AP
	static char mSoftApName[13];
	static char mSoftApPassword[20];
	static char mSoftApIP[16];
	static char mSoftApGateway[16];
	static char mSoftApNetmask[16];
	static uint32_t mSoftApPort;
	static uint16_t mSoftApClientTimeOut;//单位：S 最大值7200,0为永远不断开，不建议使用
	
	//station
	static char mJoinApName[13];
	static char mJoinApPassword[20];
	static unsigned char mStationMac[18];//固定长度
	
	//server
	static char mServerIPOrDomain[30];
	static uint32_t mServerPort;
	
	//Node
	static uint32_t mNodePort;
	
public:
	/****************WIFI相关工具******************************/
	////////////////////////
	///将字符串形式的MAC地址转换为字节形式的，6个字节
	///////////////////////
	static void MacAddressStringToBytes(char* macStr,unsigned char macAddress[6]);

	///////////////////////////////
	///将IP字符串转换为数组
	///@param IPStr 字符串
	///@param IP 存放字节类型的IP地址的数组
	///@retval 有效IP地址字符串的长度
	///////////////////////////////
	static unsigned char IPStringToBytes(char* IPStr, unsigned char IP[4]);



	//////////////////////////////////////////
	///提取包含多个IP地址的字符串中的IP地址到数组，每个IP4个字节
	///@param IPStr 字符串
	///@param IP 存放字节类型的IP地址的数组
	///@retval 找到正确格式IP的个数
	/////////////////////////////////////////
	static unsigned char IPStringsToBytes(char* IPStr, unsigned char IP[][4]);
	
	///////////////////////////////
	///将IP字符串转换为数组
	///@param IPStr 字符串
	///@param IP 存放字节类型的IP地址的数组
	///@retval 有效IP地址字符串的长度
	///////////////////////////////
	static void IPBytesToString( unsigned char IP[4],char* IPStr);

};



#endif


