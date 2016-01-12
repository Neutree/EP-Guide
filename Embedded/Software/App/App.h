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
#include "MD5.h"

#define APP_BUFFER_SIZE 50
#define APP_MAX_PARKING_SPACE_SIZE 50      //最多同时50个车位
#define APP_MAX_IP_ADDRESS_BUFFER_SIZE 200 //最多50个车位，每个4个字节的IP地址


typedef enum
{
	NodeStatus_OFF_Line = 0x80, //掉线
	NodeStatus_On_line  = 0x40, //在线
	NodeStatus_Busy     = 0x20, //有车
	NodeStatus_Free     = 0x10, //空闲
	NodeStatus_Disable  = 0x08, //不使用该车位
	
	NodeStatus_ToBusy   = 0x04, //状态改变,从空闲到有车
	NodeStatus_ToFree   = 0x02  //从有车到空闲
}NodeStatus;

typedef struct 
{
	unsigned char macAdress[6];
	unsigned char ipAddress[4];
	NodeStatus status;
	bool checked;
}NodeInfo;


typedef struct 
{
	uint16_t number;//车位数量
	NodeInfo nodeInfo[APP_MAX_PARKING_SPACE_SIZE];//车位状态
}AllNodeInfo;


class APP
{

	
public:


///////////////////////////
///构造函数，执行片上资源初始化、片外资源初始化（包括片上到片外的装配）
///////////////////////////
	APP();



/**********************************与服务器交互**************************************/
///////////////////////////
///向服务器发送链路请求（心跳，定时进行）
///////////////////////////
void HeartBeatRequest();

///////////////////////////
///向服务器发送链路请求（心跳，定时进行）
///////////////////////////
void WaitHeartBeatRequestAck();
//////////////////////////
///超时等待信息到来，收到的信息将有效信息存到mBuffer开头
/////////////////////////
bool WaitReceiveAndDecode(unsigned char timeOut=2);

////////////////////////////
///登录到服务器
////////////////////////////
bool LogIn();

///////////////////////////
///发送车位状态更改请求
///@param macAddress mac地址
///@param status 状态更改  取值:    NodeStatus_ToBusy   = 0x04, //状态改变,从空闲到有车
///                                 NodeStatus_ToFree   = 0x02  //从有车到空闲
///@param 车主ID号
///@retval 请求是否成功
///////////////////////////
bool ReqChangeStatus(unsigned char macAddress[6],NodeStatus status,unsigned char carID[4]);


///////////////////////////
///请求添加新的节点
///@param macAddress 新节点的mac地址
///@retval 是否成功 0：成功 -1：通信失败 -2：请求被拒绝
///////////////////////////
int8_t ReqAddNewNode(unsigned char macAddress[6]);

///////////////////////////
///请求删除节点
///@param macAddress 新节点的mac地址
///@retval 是否成功
///////////////////////////
bool ReqDelNode(unsigned char macAddress[6]);


/////////////////////////////////////////
///向节点请求道路引导
///@param macAddress 引导节点顺序，最后一个为终点车位
///@param number 参与引导的车位数量
///@retval 是否成功
/////////////////////////////////////////
bool ReqShorestLead(unsigned char macAddress[][6],uint16_t *number);
	
/***********************************************************************************/





/*************************RFID*******************************************************/
void FindCar();
/***********************************************************************************/






/**********************************与节点交互***************************************/
////////////////////////////////////////
///轮询节点信息
///@param 车的编号（RFID卡的编号），车位状态改变时carID有值，进（出）车库的车的编号
///@param 车位的mac地址
///@retval 节点状态 
///////////////////////////////////////
NodeStatus QueryNodeStatus(unsigned char carID[4],unsigned char macAddress[6],unsigned char ipAddress[4]);

/////////////////////////////////////////
///向节点请求道路引导
///@param macAddress 引导节点顺序，最后一个为终点车位
///@param number 参与引导的车位数量
///@retval 是否成功
/////////////////////////////////////////
bool ReqLead(unsigned char macAddress[][6],uint16_t number);

/////////////////////////////////////////
///向节点请求道路引导
///@param macAddress 引导节点顺序，最后一个为终点车位
///@param number 参与引导的车位数量
///@retval 是否成功
/////////////////////////////////////////
bool ReqCompleteLead(unsigned char macAddress[][6],uint16_t number);


////////////////////////////////////////
///请求禁用节点
///@param macAddr 请求禁用的mac地址
///@param ipAddr  请求禁用的ip地址
///@retval 是否禁用成功
////////////////////////////////////////
bool ReqDisableNode(uint8_t macAddr[6],uint8_t ipAddr[4]);



/////////////////////////////////////////
///查询节点的MAC地址是否在节点信息中，以及是否被禁用
///@retval 0:没有该节点信息，是新的节点  1：有该节点的信息，是已经添加了的节点 
/////////////////////////////////////////
uint8_t RoleStatus(uint8_t macAddress[6]);

/////////////////////////////////////////
///根据mac地址寻找节点的IP地址
///@param mac地址
///@param ip 地址
/////////////////////////////////////////
bool FindIPbyMacAddress(uint8_t macAddress[6],uint8_t ip[4]);
/************************************************************************************/

////////////////////////////////////////
///寻找mac地址相对应的节点的下标
///@param macAddress mac地址
///@retval 下标，如果没找到，返回0xffff
////////////////////////////////////////
uint16_t IndexOfMacAddress(uint8_t macAddress[6]);

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
uint16_t mLogInFailRetryInterval; //登录失败重试间隔间隔定义 单位：S
int8_t mToServerConnectionHealth; //标志与服务器的连接情况，由链路请求（心跳）控制 1:健康 -1：失去连接 0：正在检测
int8_t mToServerLogInStatus; //标志与服务器的连接情况，由链路请求（心跳）控制 1:已经登录 -1：未登录 0：正在登录
unsigned char mBuffer[APP_BUFFER_SIZE];
//unsigned char mBufferReceive[APP_BUFFER_SIZE];//用来接收数据的缓冲区

//wifi客户端信息（IP地址保存）
unsigned char mIPBuffer[APP_MAX_PARKING_SPACE_SIZE][4];
unsigned char mMacBuffer[APP_MAX_PARKING_SPACE_SIZE][6];
unsigned short mLeadNumber; //参与引导的车位数量
//车位信息
AllNodeInfo mAllNodeInfo;

bool mIsleadNow;//标志是否正在进行引导动作

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
