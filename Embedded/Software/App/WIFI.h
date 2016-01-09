#ifndef __WIFI_H
#define __WIFI_H

#include "esp8266.h"


class WIFI
{
	public:
		static bool Init(esp8266 &wifi);
	
	public:
		/*************常量*****************/
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
	static char mStationMac[18];//固定长度
	
};



#endif


