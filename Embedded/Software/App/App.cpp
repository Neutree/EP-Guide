# include "App.h"


/////////////////////////////
///全局变量
////////////////////////////
APP app;
/*------------------------------------------------------------------------*/




///////////////////////////
///构造函数，执行片上资源初始化、片外资源初始化（包括片上到片外的装配）
///////////////////////////
APP::APP()
:mLedRedGPIO(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz),mLedGreenGPIO(GPIOB,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz),
mBuzzerGPIO(GPIOB,5,GPIO_Mode_Out_PP,GPIO_Speed_50MHz),
mCOM1(1,115200,true),mCOM2(2,9600,true),
mLedRed(mLedRedGPIO,false),mLedGreen(mLedGreenGPIO,false),
mBuzzer(mBuzzerGPIO,false),
mRFID(&mCOM2),
mWIFI(mCOM1,115200)	
{
	mReqLinkCheckInterval=30;        //心跳包间隔30s
	mToServerConnectionHealth=-1;    //与服务器的连接初始化为失去连接
}


/////////////////////////////////////
///静态常量初始化
/////////////////////////////////////
const unsigned char APP::mPICCDefaultKey[6]= {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};




/////////////////////////////
///硬件初始化
/////////////////////////////
void APP::InitHardware()
{
	//关闭LED
	mLedRed.Off();
	mLedGreen.Off();
	
	//PCD复位
	mRFID.PCDReset();
	
	//wifi初始化
	WIFI::Init(mWIFI);
	
	
	mCOM1<<"Initialize complete\n\n\n";
	for(u8 i=0;i<10;++i)
	{
		mLedGreen.On();
		mLedRed.Off();
		TaskManager::DelayMs(100);
		mLedGreen.Off();
		mLedRed.On();
		TaskManager::DelayMs(100);
	}
	mLedRed.Off();
	mCOM1.ClearReceiveBuffer();
}

///////////////////////////////
///软件需要的初始化
//////////////////////////////
void APP::InitSoft()
{
	//登录服务器
	
}


/////////////////////////////////
///循环执行部分
/////////////////////////////////
void APP::Loop()
{
/****************--①--器件检测	、错误处理**********************/	


	//RFID 健康状况,器件存在问题时不会执行--②--
	if(!mRFID.mHealth)
	{
		//一直侦测健康状况
		if(mRFID.Kick())//检测到，进行状态设置
		{
			mRFID.PCDReset();
			return;
		}
		//RFID连接失败处理
		
			
		mBuzzer.On();
		return;
	}
	else
	{
		mBuzzer.Off();
	}
	//WIFI健康状况检查
	if(mWIFI.mHealth!=0)//WIFI 存在问题
	{
		mBuzzer.On();
		return ;
	}
	else
	{
		mBuzzer.Off();
	}
/***************************************************************/

	
/*********************--②--车辆检测**************************/

if(mRFID.PcdRequest(MFRC522_PICC_REQALL,mTagInfo))//寻到卡
{
	mCOM1<<"find ok\n";
	if(((u16)mTagInfo[0]<<8|mTagInfo[1])==MFRC522_PICC_MIFARE_ONE_S50)//卡类型：S50
	{
		if(mRFID.PcdAntiColl(mTagInfo))//防冲撞成功(找到一张卡序列号)
		{
			mLedGreen.On();
			
			mRFID.PcdHalt();
		}
		else
		{
			mLedGreen.Off();
		}
	}
}
else
{
	mLedGreen.Off();
}

/***************************************************************/
	

/****************--③--接收到的信息处理（取出完整的有效数据帧）**********/

/********************************************************************/



/*************************--⑥--链路保持（心跳包），周期为10秒********************/
static uint32_t heartBeatTimeOld=0;
static uint32_t heartBeatTimeNew=TaskManager::Time();

heartBeatTimeNew=TaskManager::Time();
//时间到发送链路请求
if(heartBeatTimeNew-heartBeatTimeOld>=mReqLinkCheckInterval)
{
	HeartBeatRequest();
	WaitHeartBeatRequest();
	heartBeatTimeOld=heartBeatTimeNew;
}
if(mToServerConnectionHealth==-1)//失去与服务器的连接
{
	
}

//检测来自服务器的链路响应

/********************************************************************/






}



void APP::HeartBeatRequest()
{
	//心跳包数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mToServerLinkCheckPack[2]=temp>>8;//消息ID高字节
	Communicate::mToServerLinkCheckPack[3]=temp&0x00ff;//消息ID低字节
	char mac[6]={1,2,3,4,5,6};
	WIFI::MacAddressStringToBytes(WIFI::mStationMac,mac);
	memcpy(&Communicate::mToServerLinkCheckPack[4],mac,6);//复制mac地址
	Communicate::mToServerLinkCheckPack[10]=0;//消息体长度：0
	Communicate::mToServerLinkCheckPack[11]=0;//消息体长度：0
	Communicate::mToServerLinkCheckPack[12]=TO_SERVER_cReqLinkCheck>>8;//命令字高字节
	Communicate::mToServerLinkCheckPack[13]=TO_SERVER_cReqLinkCheck&0x00ff;//命令字低字节
	
	Communicate::mToServerLinkCheckPack[14]=MathTool::CheckSum8((unsigned char*)Communicate::mToServerLinkCheckPack,14);
	
	Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mToServerLinkCheckPack,15);
	mToServerConnectionHealth=0;//标志正在检测
}


///////////////////////////
///向服务器发送链路请求（心跳，定时进行）
///////////////////////////
void APP::WaitHeartBeatRequest()
{
	mToServerConnectionHealth=0;
	if(WaitReceiveAndDecode())
	{
		if(mBuffer[10]==0&&mBuffer[11]==0&&mBuffer[12]==(TO_SERVER_cAckLinkCheck>>8)&&(mBuffer[13]==(u8)TO_SERVER_cAckLinkCheck))
		{
			mToServerConnectionHealth=1;//链路正确
		}
	}
	if(mToServerConnectionHealth==0)
	{
		mToServerConnectionHealth=-1;
	}
}


//////////////////////////
///超时等待信息到来，收到的信息将有效信息存到mBuffer开头
/////////////////////////
bool APP::WaitReceiveAndDecode(unsigned char timeOut)
{
	double time=TaskManager::Time();
	unsigned char temp=0;
	unsigned char count=0,count2=0;
	uint16_t length=0;
	while(1)
	{
		if(TaskManager::Time()-time>timeOut)
		{
			
			break;
		}
		
		if(mCOM1.ReceiveBufferSize()+count>=25)
		{
			while(mCOM1.ReceiveBufferSize()>0)
			{
				mCOM1.GetReceivedData(&temp,1);
				if(temp=='+'&&count==0)
				{
					count++;
				}
				else if(temp=='I'&&count==1)
					count++;
				else if(temp=='P'&&count==2)
					count++;
				else if(temp=='D'&&count==3)
					count++;
				else if(temp==',' &&count==4)
					count++;
				else if(count==5)
					count++;
				else if(temp==','&&count==6)
					count++;
				else if(temp!=':'&&count==7)//长度
				{
					++count;
					length=(temp-'0');
				}
				else if(temp!=':'&&count==8)
				{
					++count;
					length=length*10+(temp-'0');
				}
				else if(temp==':'&& count>6)//后面的数据都是服务器发来的消息
				{
					count=24;
				}
				else if(count==24)//数据
				{
					mBuffer[count2++]=temp;//放进缓冲区
					
					if(count2>=length)
					{

						u16 dataLength=((unsigned short)mBuffer[10])<<8|mBuffer[11];
						//数据验证有效性
						if(mBuffer[0]==0xA4&&mBuffer[1]==0x02&&mBuffer[14+dataLength]==MathTool::CheckSum8(mBuffer,dataLength+14))//数据头和校验正确
						{
							return true;
							
						}
						return false;
					}
				}
			}
		}
	}
	return false;
}



