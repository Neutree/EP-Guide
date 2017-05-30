#include "Communicate.h"
#include "MD5.h"
#include "string.h"
#include "TaskManager.h"

Communicate::Communicate(Socket_esp8266& wifi)
:mWiFi(wifi),mMac("12:08:07:72:07:01")
{
	mMacBytes[0] = 0x12;
	mMacBytes[1] = 0x08;
	mMacBytes[2] = 0x07;
	mMacBytes[3] = 0x72;
	mMacBytes[4] = 0x07;
	mMacBytes[5] = 0x01;
}

bool Communicate::Login()
{
	unsigned char encrypt[23] = "00:00:00:00:00:00A-402";//21232f297a57a5a743894a0e4a801fc3  
	unsigned char body[16];
	memcpy(encrypt,mMac,17);
	MD5_CTX md5;
	MD5Init(&md5);
	MD5Update(&md5, encrypt, strlen((char *)encrypt));
	MD5Final(&md5, body);
	mLoginReqTime = TaskManager::Time();
	return SendDataToServer(TO_SERVER_cReqLogin,body,16);
}

bool Communicate::LinkCheck()
{
	mLinkCheckReqTime = TaskManager::Time();
	return SendDataToServer(TO_SERVER_cReqLinkCheck,NULL,0);
}

bool Communicate::AckWrite(bool isSuccess)
{
	uint16_t success = isSuccess?1:0;
	return SendDataToServer(TO_SERVER_cAckWriteTag,(uint8_t*)&success,2);
}
bool Communicate::SendCardID(uint8_t quantity,uint8_t* ID)
{
	return SendDataToServer(To_SERVER_cReqReadTag,ID,quantity*8);
}

bool Communicate::SendDataToServer(uint16_t msgCmd,uint8_t* data,uint16_t dataLen)
{
	frame_t frame={
		.head = 0xa402,
		.msgID = ++mMsgID,
		.bodyLen = dataLen,
		.msgCmd = msgCmd
	};
	memcpy(frame.mac,mMacBytes,6);
	memcpy(frame.body,data,dataLen);
	uint8_t i=0;
	uint32_t sum=0;
	for(;i<14+dataLen;++i)
		sum += ((uint8_t*)&frame)[i];
	frame.body[dataLen] = sum&0xff;
	LittleEndian2BigEndian(&frame.head);
	LittleEndian2BigEndian(&frame.msgID);
	LittleEndian2BigEndian(&frame.bodyLen);
	LittleEndian2BigEndian(&frame.msgCmd);	
	return mWiFi.Write((char*)&frame,15+dataLen);
}

/**
  *解码接收到的数据
  *@param data 接收到的数据传入，如果有body，将body从数组开头开始放
  *@param len  接收到的数据的长度传入，如果有数据中有body部分，则传出body的长度
  *@return 0:数据不完整 其它：数据类型，见communicate.h开头部分
  */
uint16_t Communicate::Decode(uint8_t* data,uint16_t* len)
{
	uint8_t i=0;
	uint32_t sum=0;
	for(;i<*len-1;++i)
		sum += data[i];
	if((sum&0xff) == data[*len-1])
	{
		if(data[0]==0xa4 && data[1]==0x02)
		{
			if(memcmp(data+4,mMacBytes,6)==0)
			{
				*len = ((uint16_t)data[10]<<8)|data[11];
				memmove(data,data+14,*len);
				return (uint16_t)data[12]<<8 | data[13];
			}
		}
	}
	return 0;
}

void Communicate::LittleEndian2BigEndian(uint16_t* data_)
{
	uint16_t temp = *data_<<8&0xff00;
	*data_ = (*data_ >> 8 & 0x00ff) |temp;
}

float Communicate::LoginReqTime()
{
	return mLoginReqTime;
}
float Communicate::LinkCheckReqTime()
{
	return mLinkCheckReqTime;
}
