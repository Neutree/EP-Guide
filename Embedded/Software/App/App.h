# ifndef __APP_H
# define __APP_H

# include "USART.h"
# include "MFRC522.h"
# include "GPIO.h"
# include "LED.h"
# include "esp8266.h"
# include "Buzzer.h"

# include "WIFI.h"
# include "Communicate.h"

#include "MathTool.h"

#define APP_BUFFER_SIZE 50

class APP
{

	
public:


///////////////////////////
///构造函数，执行片上资源初始化、片外资源初始化（包括片上到片外的装配）
///////////////////////////
	APP();

///////////////////////////
///向服务器发送链路请求（心跳，定时进行）
///////////////////////////
void HeartBeatRequest();

///////////////////////////
///向服务器发送链路请求（心跳，定时进行）
///////////////////////////
void WaitHeartBeatRequest();
//////////////////////////
///超时等待信息到来，收到的信息将有效信息存到mBuffer开头
/////////////////////////
bool WaitReceiveAndDecode(unsigned char timeOut=2);

private:
/**************************硬件资源*******************************/
	/////////////////////////////////
	///片上资源分配（初始化在构造函数中）
	//////////////////////////////////
	//GPIO
	GPIO mLedRedGPIO;
	GPIO mLedGreenGPIO;
	GPIO mBuzzerGPIO;
	//串口1
	USART mCOM1;
	//串口2
	USART mCOM2;


	////////////////////////////////////
	///外设资源分配（在构造中装配（片上资源和外设的联系））
	///////////////////////////////////
	//LED
	LED mLedRed;
	LED mLedGreen;
	///Buzzer
	Buzzer mBuzzer;
	//RFID
	MFRC522 mRFID;
	//WIFI
	esp8266 mWIFI;
/*****************************************************************/




/**************************私有常量***********************************/
static const unsigned char mPICCDefaultKey[6];

/*********************************************************************/






/*************************私有变量*************************************/
unsigned char mTagInfo[MFRC522_MaxReceiveLen];
uint16_t mReqLinkCheckInterval; //心跳包间隔定义 单位：S
int8_t mToServerConnectionHealth; //标志与服务器的连接情况，由链路请求（心跳）控制 1:健康 -1：失去连接 0：正在检测
unsigned char mBuffer[APP_BUFFER_SIZE];
/*******************************************************************/






/***************************系统函数，固定,由系统调用********************/
public:
/////////////////////////////
///硬件初始化
/////////////////////////////
	void InitHardware();

///////////////////////////////
///软件需要的初始化
//////////////////////////////
	void InitSoft();



/////////////////////////////////
///循环执行部分
/////////////////////////////////
	void Loop();


/*******************************其它****************************/


};

extern APP app;

#endif
