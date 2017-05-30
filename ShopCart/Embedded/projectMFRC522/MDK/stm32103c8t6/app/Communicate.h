#ifndef __COMMUNICATE_H
#define __COMMUNICATE_H
#include "stdint.h"
#include "Socket_esp8266.h"

typedef struct 
{
	uint16_t head;
	uint16_t msgID;
	uint8_t  mac[6];
	uint16_t bodyLen;
	uint16_t msgCmd;
	uint8_t body[50];
}frame_t;

/***************************与服务器通信的消息命令常量*******************/
//| 基本请求 | 登录、心跳包等 |
#define TO_SERVER_cReqLogin             0x0101
#define From_SERVER_cAckLogin           0x0801
#define TO_SERVER_cReqLinkCheck         0x0102
#define From_SERVER_cAckLinkCheck       0x0802
#define From_SERVER_cReqWriteTag        0x0104
#define TO_SERVER_cAckWriteTag          0x0804
#define To_SERVER_cReqReadTag           0x0105
#define From_SERVER_cAckReadTag         0x0805

class Communicate
{
	public:
		Communicate(Socket_esp8266&);
		bool Login();
		bool LinkCheck();
		bool AckWrite(bool);
		bool SendCardID(uint8_t quantity,uint8_t* ID);//8byte*n
		uint16_t Decode(uint8_t* data,uint16_t* len);
		float LoginReqTime();
		float LinkCheckReqTime();
	private:
		Socket_esp8266& mWiFi;
		uint16_t mMsgID;
		bool SendDataToServer(uint16_t msgCmd,uint8_t* data,uint16_t dataLen);
		char mMac[18];
		uint8_t mMacBytes[6];
		void LittleEndian2BigEndian(uint16_t*);
		float mLoginReqTime,mLinkCheckReqTime;
};

#endif
