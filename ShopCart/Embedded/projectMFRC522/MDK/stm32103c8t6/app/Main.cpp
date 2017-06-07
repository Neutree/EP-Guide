




#define OFFLINE_RETRY_INTERVAL  10  //s
#define LOGGING_MAX_WAIT_TIME   10  //s
#define SEND_HEARTBEAT_TIME_INTERVAL    60 //s
#define LOGGED_MAX_HEARTBEAT_WAIT_TIME  130 //s
#define MAX_CARD_QUANTITY 10
#define MAX_CARD_NOT_READ_TIME 3  //s




#include "stm32f10x.h"
#include "Configuration.h"
#include "TaskManager.h"
#include "USART.h"
#include "LED.h"
#include "Socket_esp8266.h"
#include "Communicate.h"
#include "SPI.h"
#include "MFRC522.h"



USART com(1,115200);
USART com2(2,115200);
Socket_esp8266 wifi(com2);

GPIO ledRedGPIO(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
LED ledRed(ledRedGPIO);//LED red

SPI spi1(SPI1);
MFRC522 rfid(&spi1);

Communicate conn(wifi);


/*******************configuration****************/
//char* mApJoinName  ="Neucrack";
//char* mApJoinPasswd="15023490062...";
//char* mServerIP    = "192.168.168.125";
//uint16_t mServerPort  = 5080;

char* mApJoinName  ="LocalServer";
char* mApJoinPasswd="dandancool";
char* mServerIP    = "192.168.191.1";
uint16_t mServerPort  = 5080;

//char* mApJoinName  ="MI 5";
//char* mApJoinPasswd="12345678";
//char* mServerIP    = "192.168.43.229";
//uint16_t mServerPort  = 5080;
/***********************************************/


char dataToSend[10]={1,2,3,4,5,6,7,8,9,10};
uint8_t dataToRead[50];
unsigned char defaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct
{
	uint8_t exist;
	uint8_t cardID[8];
	double timestrap;
}cardInfo_t;
cardInfo_t cardInfo[MAX_CARD_QUANTITY];


bool WiFiInit(void);

typedef enum
{
	STATUS_LOGGING_IN=0,
	STATUS_LOGGED_IN=1,
	STATUS_OFFLINE=2,
}status_t;
status_t status;
float offline_time,recv_heartbeat_time,send_heartbeat_time;

/*****************************************************************************/

void StatusCheck();
void RFIDFind();
bool WriteNumber(uint8_t* number);
bool IsCardExist(uint8_t* cardNumber);
void CardAdd(uint8_t* cardNumber);
void FlushTimestrap(uint8_t* cardNumber);
void CardTimeoutCheck();
void SendCardInfo2Server(uint8_t* cardNumber);
uint8_t CardQuantity();


void init()
{
	com<<"start initialize...\n";
	ledRed.Off();
	status = STATUS_OFFLINE;
	offline_time = TaskManager::Time();
	if(!WiFiInit())
	{
		com<<"WiFi init fail\r\n";
		while(1)
			ledRed.Blink(0,100,false);
	}
	ledRed.Blink(6,100,true);
	com<<"start log in!\n";
	conn.Login();
	status = STATUS_LOGGING_IN;
	
	rfid.PCDReset();
	for(uint8_t i=0;i<MAX_CARD_QUANTITY;++i)
		cardInfo[i].exist = 0;
	com<<"card quantity now:"<<CardQuantity()<<"\n";
	com<<"initialize complete!\n";
}
void loop()
{
	if(status == STATUS_OFFLINE)
		ledRed.Blink(0,2000,false);
	else
		ledRed.Blink(0,500,false);

	if(com2.ReceiveBufferSize()>10)
	{
		bool success = false;
		uint16_t length = wifi.Read((char*)dataToRead);
		com <<"received data,length:"<<length<<"\n";
		int16_t cmd = conn.Decode(dataToRead,&length);
		switch(cmd)
		{
			case From_SERVER_cAckLogin:
				if(status == STATUS_LOGGING_IN && dataToRead[0]==0 && dataToRead[1]==1)
				{
					com<<"log in success!\n";
					status = STATUS_LOGGED_IN;
					recv_heartbeat_time = TaskManager::Time();
				}
				break;
			case From_SERVER_cAckLinkCheck:
				if(dataToRead[0]==0 && dataToRead[1]==1)
				{
					com<<"received heartbeat ack!\n";
					recv_heartbeat_time = TaskManager::Time();
				}
				break;
			case From_SERVER_cAckReadTag:
				if(dataToRead[0]==0 && dataToRead[1]==1)
					com<<"send card info success!\n";
				break;
			case From_SERVER_cReqWriteTag:
				com<<"received write ID command!\n";
				success = WriteNumber(dataToRead);
				conn.AckWrite(success);
				com<<"wirite card ID,success?:"<<success<<"\n";
				break;
			default:
				com<<"undefined cmd: "<<cmd<<"\n";
				break;
		}
	}
	RFIDFind();
	CardTimeoutCheck();
	StatusCheck();
}

void StatusCheck()
{
	switch((uint8_t)status)
	{
		case STATUS_OFFLINE:
			if(TaskManager::Time()-offline_time > OFFLINE_RETRY_INTERVAL)
			{
				com<<"start log in!\n";
				wifi.Close();
				if(!wifi.Connect(mServerIP,mServerPort,Socket_Type_Stream,Socket_Protocol_IPV4))
					com<<"fail to connect server!!!\n";
				conn.Login();
				status = STATUS_LOGGING_IN;
			}
			break;
		case STATUS_LOGGING_IN:
			if(TaskManager::Time()-conn.LoginReqTime() > LOGGING_MAX_WAIT_TIME)
			{
				com<<"log in fail! auto try again after "<<OFFLINE_RETRY_INTERVAL<<"s later!\n";
				status = STATUS_OFFLINE;
				offline_time = TaskManager::Time();
			}
			break;
		case STATUS_LOGGED_IN:
			if(TaskManager::Time()-send_heartbeat_time>SEND_HEARTBEAT_TIME_INTERVAL)
			{
					send_heartbeat_time = TaskManager::Time();
					conn.LinkCheck();
			}
			if(TaskManager::Time()-recv_heartbeat_time > LOGGED_MAX_HEARTBEAT_WAIT_TIME)
			{
				com<<"keep heartbeat fail! auto try log in after "<<OFFLINE_RETRY_INTERVAL<<"s later!\n";
				status = STATUS_OFFLINE;
				offline_time = TaskManager::Time(); 
			}
			break;
	}
}

void RFIDFind()
{
	uint8_t mTagInfo[4];
	if(rfid.PcdRequest(MFRC522_PICC_REQIDL,mTagInfo))//寻到卡
	{
		if(((u16)mTagInfo[0]<<8|mTagInfo[1])==MFRC522_PICC_MIFARE_ONE_S50)//卡类型：S50
		{
			if(rfid.PcdAntiColl(mTagInfo))//防冲撞成功(找到一张卡序列号)
			{
				if(rfid.PcdSelect(mTagInfo))//选卡，卡号为前一步找到的卡号
				{	
					if(rfid.PcdAuthState(MFRC522_PICC_AUTHENT1A, 2, defaultKey, mTagInfo))
					{
						uint8_t cardValue[16] = {0};
						if(rfid.PcdRead(2,cardValue))
							if(!IsCardExist(cardValue))
							{
								com<<"find new card! card quantity now:"<<CardQuantity()<<"\n";
								CardAdd(cardValue);
								SendCardInfo2Server(cardValue);
							}
					}
				}
			}
			
		}
	}
}


bool WriteNumber(uint8_t* number)
{
	uint8_t mTagInfo[4];
	if(rfid.PcdRequest(MFRC522_PICC_REQIDL ,mTagInfo))//寻到卡
	{
		if(((u16)mTagInfo[0]<<8|mTagInfo[1])==MFRC522_PICC_MIFARE_ONE_S50)//卡类型：S50
		{
			if(rfid.PcdAntiColl(mTagInfo))//防冲撞成功(找到一张卡序列号)
			{
				if(rfid.PcdSelect(mTagInfo))//选卡，卡号为前一步找到的卡号
				{	
					if(rfid.PcdAuthState(MFRC522_PICC_AUTHENT1A, 2, defaultKey, mTagInfo))
					{
						uint8_t cardValue[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
						memcpy(cardValue,number,8);
						rfid.PcdWrite(2,cardValue);
						rfid.PcdHalt();//休眠卡
						return true;
					}
				}
			}
		}
	}
	return false;
}


uint8_t CardQuantity()
{
	uint8_t i=0,quantity=0;
	for(;i<MAX_CARD_QUANTITY;++i)
	{
		if(cardInfo[i].exist == 1)
			++quantity;
	}
	return quantity;
}
bool IsCardExist(uint8_t* cardNumber)
{
	uint8_t i=0;
	for(;i<MAX_CARD_QUANTITY;++i)
	{
		if(cardInfo[i].exist == 1)
		{
			if(memcmp(cardNumber,cardInfo[i].cardID,8)==0)
			{
				//flush timestrap
				cardInfo[i].timestrap = TaskManager::Time();
				return true;
			}
		}
	}
	return false;
}
void CardAdd(uint8_t* cardNumber)
{
	uint8_t i=0;
	for(;i<MAX_CARD_QUANTITY;++i)
	{
		if(cardInfo[i].exist == 0)
		{
			cardInfo[i].exist=1;
			memcpy(cardInfo[i].cardID,cardNumber,8);
			cardInfo[i].timestrap = TaskManager::Time();
			break;
		}
	}
}
void CardTimeoutCheck()
{
	uint8_t i=0;
	for(;i<MAX_CARD_QUANTITY;++i)
	{
		if(cardInfo[i].exist == 1)
		{
			if(TaskManager::Time()-cardInfo[i].timestrap > MAX_CARD_NOT_READ_TIME)//指定时间段内没有读到卡，判定卡离线
			{
				cardInfo[i].exist = 0;
			}
		}
	}
}
void SendCardInfo2Server(uint8_t* cardNumber)
{
 	if(status != STATUS_LOGGED_IN)
		return;
	if(TaskManager::Time()-send_heartbeat_time>SEND_HEARTBEAT_TIME_INTERVAL-2)//即将发送心跳数据，防止心跳数据和读卡信息一起发送（主要是服务器处理有问题会粘包）
		send_heartbeat_time+=3;
	else if(TaskManager::Time() - send_heartbeat_time <2)//刚刚发了心跳
		TaskManager::DelayS(1);
	static float last_send_time=TaskManager::Time();	
	if(TaskManager::Time() - last_send_time<3)
		TaskManager::DelayS(2);
	conn.SendCardID(1,cardNumber);
	last_send_time = TaskManager::Time();
}


/****************************************************************************/




bool WiFiInit()
{
		com<<"setting wifi...\n";
    wifi.Init();//初始化时一定要清除串口缓冲区，因为WiFi上电后回显很多调试信息，可能会造成串口缓冲区溢出，上电大于2秒后调用
    if(!wifi.Kick())
        return false;
    wifi.SetEcho(false);//关闭回响
    wifi.SetMode(esp8266::esp8266_MODE_STATION,esp8266::esp8266_PATTERN_DEF);//设置为station模式
    wifi.SetMUX(false);//单连接模式
		com<<"WiFi join AP now....\n";
    if(!wifi.JoinAP(mApJoinName,mApJoinPasswd,esp8266::esp8266_PATTERN_DEF))//加入AP
		{
			com<<"join WiFi AP fail!!\n";
			return false;
		}
    TaskManager::DelayS(2);
		com<<"WiFi join suceess!\n";
    //连接服务器
    if(!wifi.Connect(mServerIP,mServerPort,Socket_Type_Stream,Socket_Protocol_IPV4))
        return false;
    return true;
}



int main()
{
	ledRed.On();
	TaskManager::DelayS(3);
	ledRed.Blink(4,100,true);
	ledRed.Off();
	init();
	while(1)
	{
		loop();
	}
}

