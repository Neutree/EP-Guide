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
	mToServerLogInStatus=-1;     	//未登录
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
	//关闭蜂鸣器
	mBuzzer.Off();
	
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
	LogIn();
	
}


/////////////////////////////////
///循环执行部分
/////////////////////////////////
void APP::Loop()
{	
/****************--①--器件检测	、状态监测、错误处理**********************/	


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
		mWIFI.kick();//检测是否恢复连接
		return ;
	}
	else
	{
		mBuzzer.Off();
	}
	
	//链路状态检查
	if(mToServerConnectionHealth==-1)//失去与服务器的连接
	{
		HeartBeatRequest();
		WaitHeartBeatRequestAck();
	}
	
	//登录状态
	if(mToServerLogInStatus==-1)//没有登录
	{
		//尝试重新登录，如果失败，返回，成功则继续进行下面的动作
		if(mToServerConnectionHealth==1)//链路正常,尝试登录
		{
			LogIn();
			if(mToServerLogInStatus==-1)//仍然失败
				return;
		}
		else//链路不正常，不用登录
			return;
		
	}
	
	

/***************************************************************/

	
/*********************--②--车辆检测**************************/

/***************************************************************/
	FindCar();

/****************--③--接收到的信息处理（取出完整的有效数据帧）**********/

/********************************************************************/



/*************************--⑥--链路保持（心跳包），周期为30秒********************/
static uint32_t heartBeatTimeOld=0;
static uint32_t heartBeatTimeNew=TaskManager::Time();

heartBeatTimeNew=TaskManager::Time();
//时间到发送链路请求
if(heartBeatTimeNew-heartBeatTimeOld>=mReqLinkCheckInterval)
{
	HeartBeatRequest();
	WaitHeartBeatRequestAck();
	heartBeatTimeOld=heartBeatTimeNew;
}

//检测来自服务器的链路响应

/********************************************************************/






}



void APP::HeartBeatRequest()
{
	mToServerConnectionHealth=0;//标志正在检测
	//心跳包数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mToServerPack[2]=temp>>8;//消息ID高字节
	Communicate::mToServerPack[3]=temp&0x00ff;//消息ID低字节
	char mac[6]={1,2,3,4,5,6};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mToServerPack[4],mac,6);//复制mac地址
	Communicate::mToServerPack[10]=0;//消息体长度：0
	Communicate::mToServerPack[11]=0;//消息体长度：0
	Communicate::mToServerPack[12]=TO_SERVER_cReqLinkCheck>>8;//命令字高字节
	Communicate::mToServerPack[13]=TO_SERVER_cReqLinkCheck&0x00ff;//命令字低字节
	
	Communicate::mToServerPack[14]=MathTool::CheckSum8((unsigned char*)Communicate::mToServerPack,14);
	
	Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mToServerPack,15);
	
}


///////////////////////////
///向服务器发送链路请求（心跳，定时进行）
///////////////////////////
void APP::WaitHeartBeatRequestAck()
{
	if(WaitReceiveAndDecode())
	{
		if(mBuffer[10]==0&&mBuffer[11]==2&&mBuffer[12]==(TO_SERVER_cAckLinkCheck>>8)&&(mBuffer[13]==(u8)TO_SERVER_cAckLinkCheck)&&mBuffer[14]==0)
		{
			if(mBuffer[15]==1)
				mToServerConnectionHealth=1;//链路正确
			else
				mToServerConnectionHealth=-1;//链路错误
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
		else
		{
			FindCar();//闲时，进行车辆扫描
		}
	}
	return false;
}

void APP::FindCar()
{
	if(mRFID.PcdRequest(MFRC522_PICC_REQALL,mTagInfo))//寻到卡
	{
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

}


////////////////////////////
///登录到服务器
////////////////////////////
bool APP::LogIn()
{
	mToServerLogInStatus=0;//标记正在登录
	//心跳包数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mToServerPack[2]=temp>>8;//消息ID高字节
	Communicate::mToServerPack[3]=temp&0x00ff;//消息ID低字节
	char mac[6]={0,0,0,0,0,0};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);  //MAC地址不发送，加密后放在消息体后发送（在mac地址串后面加"A-402"后加密）
	memcpy(&Communicate::mToServerPack[4],mac,6);//复制mac地址

	//加密
	/* 这里常数串可以，但是用下面注释了的方式不行，标记为BUG，待查看
	unsigned char encrypt[23] *//*= "12:08:07:72:07:01A-402"*/; /*
	unsigned char decrypt[16];
	memcpy(encrypt,WIFI::mStationMac,16);
	encrypt[17]='A';
	encrypt[18]='-';
	encrypt[19]='4';
	encrypt[20]='0';
	encrypt[21]='2';
	encrypt[21]=0;
	MD5_CTX md5;
	MD5Init(&md5);
	MD5Update(&md5, encrypt, strlen((char *)encrypt));
	MD5Final(&md5, decrypt);*/
	
	unsigned char encrypt[] = "12:08:07:72:07:01A-402";//21232f297a57a5a743894a0e4a801fc3  
	unsigned char decrypt[16];
	MD5_CTX md5;
	MD5Init(&md5);
	MD5Update(&md5, encrypt, strlen((char *)encrypt));
	MD5Final(&md5, decrypt);
	
	temp=16;//字符串长度（16字节)
//	if(decrypt[0]==0x64)
//		mLedRed.On();
	Communicate::mToServerPack[10]=temp>>8;//消息体长度：高位
	Communicate::mToServerPack[11]=(uint8_t)temp;//消息体长度：低位
	Communicate::mToServerPack[12]=TO_SERVER_cReqGateWayLogin>>8;//命令字高字节
	Communicate::mToServerPack[13]=TO_SERVER_cReqGateWayLogin&0x00ff;//命令字低字节
	memcpy(&Communicate::mToServerPack[14],decrypt,16);//复制加密后的mac地址
	Communicate::mToServerPack[14+temp]=MathTool::CheckSum8((unsigned char*)Communicate::mToServerPack,14+temp);
	
	Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mToServerPack,15+temp);
	
	//等待服务器响应（中间会执行RFID扫描）
	
	if(WaitReceiveAndDecode())
	{
		if(mBuffer[10]==0&&mBuffer[11]==2&&mBuffer[12]==(TO_SERVER_cAckGateWayLogin>>8)&&(mBuffer[13]==(u8)TO_SERVER_cAckGateWayLogin)&&mBuffer[14]==0)
		{
			if(mBuffer[15]==1)
				mToServerLogInStatus=1;//正确
			else
				mToServerLogInStatus=-1;//失败
		}
	}
	if(mToServerLogInStatus==0)//登录失败
		mToServerLogInStatus=-1;
	if(mToServerLogInStatus==1)
		return true;
	else
		return false;
}
