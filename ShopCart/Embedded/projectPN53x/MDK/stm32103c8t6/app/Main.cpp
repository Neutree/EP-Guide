




#define OFFLINE_RETRY_INTERVAL  10  //s
#define LOGGING_MAX_WAIT_TIME   10  //s
#define LOGGED_MAX_HEARTBEAT_WAIT_TIME  62 //s
#define MAX_CARD_QUANTITY 10
#define MAX_CARD_NOT_READ_TIME 3  //s




#include "stm32f10x.h"
#include "Configuration.h"
#include "TaskManager.h"
#include "USART.h"
#include "LED.h"
#include "Socket_esp8266.h"
#include "Communicate.h"
#include "PN53x.h"



USART com(1,115200);
USART com2(2,115200);
USART com3(3,115200);
Socket_esp8266 wifi(com2);

GPIO ledRedGPIO(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
LED ledRed(ledRedGPIO);//LED red


PN53x rfid(com3);

Communicate conn(wifi);


/*******************configuration****************/
char* mApJoinName  ="Neucrack";
char* mApJoinPasswd="15023490062...";
char* mServerIP    = "192.168.168.125";
uint16_t mServerPort  = 5080;

//char* mApJoinName  ="LocalServer";
//char* mApJoinPasswd="dandancool";
//char* mServerIP    = "192.168.191.1";
//uint16_t mServerPort  = 5080;

//char* mApJoinName  ="MI 5";
//char* mApJoinPasswd="12345678";
//char* mServerIP    = "192.168.43.229";
//uint16_t mServerPort  = 5080;
/***********************************************/

#define MFRC522_PICC_MIFARE_ONE_S50 0x0004
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
float offline_time,recv_heartbeat_time;

/*****************************************************************************/

void StatusCheck();
void RFIDFind();
bool WriteNumber(uint8_t* number);
bool IsCardExist(uint8_t* cardNumber);
void CardAdd(uint8_t* cardNumber);
void FlushTimestrap(uint8_t* cardNumber);
void CardTimeoutCheck();
void SendCardInfo2Server();
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
			ledRed.Blink(0,200,false);
	}
	ledRed.Blink(6,100,true);
	com<<"start log in!\n";
	conn.Login();
	status = STATUS_LOGGING_IN;
	
	rfid.PcdWakeUp();
	for(uint8_t i=0;i<MAX_CARD_QUANTITY;++i)
		cardInfo[i].exist = 0;
	com<<"card quantity now:"<<CardQuantity()<<"\n";
	com<<"initialize complete!\n";
}
void loop()
{
	static double sliceHeartbeat=0;
	if(status == STATUS_OFFLINE)
		ledRed.Blink(0,2000,false);
	else
		ledRed.Blink(0,500,false);
	if(tskmgr.TimeSlice(sliceHeartbeat,30))
	{
		if(status == STATUS_LOGGED_IN)
		{
			conn.LinkCheck();
		}
	}
	if(com2.ReceiveBufferSize()>10)
	{
		bool success = false;
		uint16_t length = wifi.Read((char*)dataToRead);
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
				com2.ClearReceiveBuffer();
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
	uint8_t uid[4];
	uint16_t type;
	if(rfid.PcdListPassiveTarget(&type,uid))
	{
		if(MFRC522_PICC_MIFARE_ONE_S50 == type)
		{
			if(rfid.PcdVerifyKeyA(uid,defaultKey))
			{
				uint8_t cardValue[16] = {0};
				if(rfid.PcdRead(cardValue))
					if(!IsCardExist(cardValue))
					{
						com<<"find new card! uid:";
						com.PrintHex(uid,4," ")<<",value:";
						com.PrintHex(cardValue,8," ")<<",";
						CardAdd(cardValue);
						SendCardInfo2Server();
					}
			}
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
				com<<"card remove,uid:";
				com.PrintHex(cardInfo[i].cardID,4," ")<<",";
				SendCardInfo2Server();
			}
		}
	}
}

bool WriteNumber(uint8_t* number)
{
	uint8_t uid[4];
	uint16_t type;
	if(rfid.PcdListPassiveTarget(&type,uid))
	{
		if(MFRC522_PICC_MIFARE_ONE_S50 == type)
		{
			if(rfid.PcdVerifyKeyA(uid,defaultKey))
			{
				if(rfid.PcdWrite(number))
					return true;
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

void SendCardInfo2Server()
{
 	if(status != STATUS_LOGGED_IN)
		return;
	uint8_t cardQuantity = CardQuantity();
	com<<"card quantity now:"<<cardQuantity<<"\n";
	uint8_t cardId[cardQuantity*8];
	uint8_t i=0,j=0;
	for(;i<MAX_CARD_QUANTITY;++i)
	{
		if(cardInfo[i].exist == 1)
		{
			memcpy(cardId+j*8,cardInfo[i].cardID,8);
			++j;
		}
	}
	conn.SendCardID(cardQuantity,cardId);
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

