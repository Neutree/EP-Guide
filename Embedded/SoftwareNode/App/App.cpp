# include "App.h"


/////////////////////////////
///全局变量
////////////////////////////
APP app;
/*------------------------------------------------------------------------*/




///////////////////////////
///构造函数，执行片上资源初始化、片外资源初始化（包括片上到片外的装配），以及变量初始化
///////////////////////////
APP::APP()
:mLedRedGPIO(GPIOA,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz),mLedGreenGPIO(GPIOA,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz),
mCOM1(1,115200,true),mCOM2(2,9600,true),
mLedRed(mLedRedGPIO,false),mLedGreen(mLedGreenGPIO,false),
mRFID(&mCOM2),
mWIFI(mCOM1,115200)	
{
	mToServerConnectionHealth=-1;    //与服务器的连接初始化为失去连接
	mToServerSignUpStatus=-1;     	//未注册
	mIsleadNow=false;                //未进行引导动作
	mStatusNow=NodeStatus_Free;
	mIsExistCar=false;
	mIsExistCarBefor=false;
	mIsBusyToFree=false;
	mIsFreeToBusy=false;
	mIsreserved=false;
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
	
	
/*	mCOM1<<"LOG:Initialize complete\n\n\n";*/
	
	mLedRed.Blink3(mLedGreen,8,100);
	mLedRed.Off();
	mCOM1.ClearReceiveBuffer();
}

///////////////////////////////
///软件需要的初始化
//////////////////////////////
void APP::InitSoft()
{
	//登录服务器
	//LogIn();
}

//////////////////////////////////////////////////////////////循环执行部分////////////////////////////////////////////////////////////////////
/////////////////////////////////
///循环执行部分
/////////////////////////////////
void APP::Loop()
{	
/****************--①--器件检测	、状态监测、错误处理**********************************/	
	static bool haveWrong=false;
	//错误警报
	if(haveWrong)
	{
	}
	else
	{
	}
//获取时间
	static double timeNew=TaskManager::Time();

	timeNew=TaskManager::Time();
	
	//RFID 健康状况,器件存在问题时不会执行--②--
	if(!mRFID.mHealth)
	{
		static double timeOld=TaskManager::Time();
		//mCOM1<<"LOG:RFID fail\n";
		if(timeNew-timeOld>2)
		{
			mLedRed.Blink(2,100);
			timeOld=timeNew;
		}
		//一直侦测健康状况
		if(mRFID.Kick())//检测到，进行状态设置
			mRFID.PCDReset();
		//RFID连接失败处理
		haveWrong=true;
		return;
	}
	
	
//	//WIFI健康状况检查
//	if(mWIFI.mHealth!=0)//WIFI 存在问题
//	{
//		mWIFI.kick();//检测是否恢复连接
//		haveWrong=true;
//		return ;
//	}
/*******************************************************************************/
	

	
	
/****************************--②--车辆检测**************************************/


	FindCar();

/*******************************************************************************/
	
	
	
	

/*********************--③--接收到的信息处理（取出完整的有效数据帧并处理）***************/
//	
//if(mCOM1.ReceiveBufferSize()>=25)//接收到的数据大于一帧数据最小长度
//{
//	if(WaitReceiveAndDecode())//成功接收到一帧完整的数据，进行命令判断
//	{
//			

//		if(mBuffer[20]==0&& mBuffer[11]==0)
//		{
//			
//			//完成引导道路请求
//			if(mBuffer[12]==(To_NODE_cReqCompleteLead>>8) && mBuffer[13]==((uint8_t)To_NODE_cReqCompleteLead))
//			{
//				AckCompleteLead();
//				mIsleadNow=false;   //引导标识复位
//			}
//			//禁用节点请求
//			else if(mBuffer[12]==(To_NODE_cReqDisable>>8) && mBuffer[13]==((uint8_t)To_NODE_cReqDisable))
//			{
//				AckDisableNode();
//				mWIFI.leaveAP();//断开连接
//			}
//		}
//		else if(mBuffer[20]==0&& mBuffer[11]==1)
//		{
//			//状态获取请求
//			if(mBuffer[12]==(To_NODE_cReqStatus>>8) && mBuffer[13]==((uint8_t)To_NODE_cReqStatus))
//			{
//				//发送给主控的状态是实际状态，获取是否被预约是为了根据是否被预约来控制提示灯！！！！！！！！！！！！！
//				mLedGreen.Blink(2,100);
//				if(mBuffer[14]==1)//车位被预约了
//					mIsreserved=true;
//				else              //车位没被预约
//					mIsreserved=false;
//				
//				if(mIsBusyToFree)
//					mStatusNow=NodeStatus_ToFree;
//				else if(mIsFreeToBusy)
//					mStatusNow=NodeStatus_ToBusy;
//				else if(mIsExistCar)
//					mStatusNow=NodeStatus_Busy;
//				else
//					mStatusNow=NodeStatus_Free;
//				AckStatus();
//				
//			}
//			//引导命令
//			else if(mBuffer[12]==(To_NODE_cReqLead>>8) && mBuffer[13]==((uint8_t)To_NODE_cReqLead))
//			{
//				AckLead();
//				mIsleadNow=true;  //置位引导标识
//				if(mBuffer[14]==0x11)//终点
//					mIsleadDestination=true;
//				else
//					mIsleadDestination=false;
//			}
//		}
//	}
//}
	

/*******************************************************************************/


/***************************引导指示和车位状态提示********************************************/

//if(mIsleadNow)//正在进行引导
//{
//	static bool flag=false;
//	static double timeOld=TaskManager::Time();
//	if(timeNew-timeOld>1)
//	{
//		flag=flag?false:true;
//		if(flag)
//		{
//			if(mIsleadDestination)
//				mLedRed.On();
//			else
//			{
//				mLedGreen.Off();
//				mLedRed.On();
//			}
//		}
//		else
//		{
//			mLedGreen.On();
//			mLedRed.Off();
//		}
//	}
//}
//else//没有进行引导，正常显示车位是否已经被占用，红灯常亮表示占用或者被预约，路灯表示空闲
//{
//	if(mIsreserved || mIsExistCar)//被预约了或者有车
//	{
//		mLedRed.On();
//		mLedGreen.Off();
//	}
//	else
//	{
//		mLedRed.Off();
//		mLedGreen.On();
//	}
//}

/*******************************************************************************/




}
/////////////////////////////////////////////////////////////循环执行部分结束/////////////////////////////////////////////////////////////////


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
						//关闭通信连接
					//	mWIFI.CloseMulitpleSend(4);
						u16 dataLength=((unsigned short)mBuffer[10])<<8|mBuffer[11];
						//数据验证有效性
						if(mBuffer[0]==0xA4&&mBuffer[1]==0x02&&mBuffer[14+dataLength]==MathTool::CheckSum8(mBuffer,dataLength+14))//数据头和校验正确
						{
							mLedRed.Blink(2,80);
							return true;
							
						}
						else
							mLedGreen.Blink(2,80);
						return false;
					}
				}
			}
		}
		else
		{
			//FindCar();//闲时，进行车辆扫描
		}
	}
	//关闭通信连接
//	mWIFI.CloseMulitpleSend(4);
	return false;
}

void APP::FindCar()
{
	bool status=false;
	static double timeOld=TaskManager::Time();
	double time=TaskManager::Time();
	if(mRFID.PcdRequest(MFRC522_PICC_REQIDL,mTagInfo))//寻到卡
	{
		mCOM1<<"find card\n";
		if(((u16)mTagInfo[0]<<8|mTagInfo[1])==MFRC522_PICC_MIFARE_ONE_S50)//卡类型：S50
		{
			if(mRFID.PcdAntiColl(mTagInfo))//防冲撞成功(找到一张卡序列号)
			{
				//mLedRed.Blink(2,100);
				mIsExistCar=true;//标记有车辆停在这儿
				status=true;
				timeOld=time;//更新时间标记
				if(!mIsExistCarBefor)
				{
					mIsFreeToBusy=true;
					if(mIsBusyToFree)//之前的标志还没清除，主控状态获取周期太短，会漏掉状态更改
						mIsBusyToFree=false;
				}
				mIsExistCarBefor=true;
			}
			
		}
	}
//	else
//		mLedGreen.Blink(2,100);
	mRFID.Kick();
	if(!status && (time-timeOld)>3)//超过5秒没找到车，判定为离开车位
	{
		mIsExistCar=false;
		if(mIsExistCarBefor)
		{
			mIsBusyToFree=true;
			if(mIsFreeToBusy)//之前的标志还没清除，主控状态获取周期太短，会漏掉状态更改
				mIsFreeToBusy=false;
		}
		mIsExistCarBefor=false;
		timeOld=time;//更新时间标记
	}
}










/////////////////////////
///响应主控的状态请求
/////////////////////////
void APP::AckStatus()
{
	//帧数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	char mac[6]={0,0,0,0,0,0};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制节点的mac地址
	Communicate::mCommunicatePack[10]=0;//消息体长度高位：0
	Communicate::mCommunicatePack[11]=5;//消息体长度低位：5
	Communicate::mCommunicatePack[12]=To_NODE_cAckStatus>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=To_NODE_cAckStatus&0x00ff;//命令字低字节
	Communicate::mCommunicatePack[14]=mStatusNow;
	memcpy(&Communicate::mCommunicatePack[15],mCarIDNow,4);
	Communicate::mCommunicatePack[19]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,19);
	//发送到服务器
	//Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mCommunicatePack,23);
	Communicate::ReturnBytesToClient(mWIFI,0,Communicate::mCommunicatePack,23);
}


////////////////////////
///响应主控的引导道路的请求
////////////////////////
void APP::AckLead()
{
	//帧数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	char mac[6]={0,0,0,0,0,0};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制节点的mac地址
	Communicate::mCommunicatePack[10]=0;//消息体长度高位：0
	Communicate::mCommunicatePack[11]=1;//消息体长度低位：5
	Communicate::mCommunicatePack[12]=To_NODE_cAckLead>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=To_NODE_cAckLead&0x00ff;//命令字低字节
	Communicate::mCommunicatePack[14]=1;
	Communicate::mCommunicatePack[15]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,15);
	//发送到服务器
	//Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mCommunicatePack,23);
	Communicate::ReturnBytesToClient(mWIFI,0,Communicate::mCommunicatePack,16);
}



////////////////////////
///响应主控的完成引导道路的请求
////////////////////////
void APP::AckCompleteLead()
{
	//帧数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	char mac[6]={0,0,0,0,0,0};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制节点的mac地址
	Communicate::mCommunicatePack[10]=0;//消息体长度高位：0
	Communicate::mCommunicatePack[11]=1;//消息体长度低位：5
	Communicate::mCommunicatePack[12]=To_NODE_cAckCompletLead>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=To_NODE_cAckCompletLead&0x00ff;//命令字低字节
	Communicate::mCommunicatePack[14]=1;
	Communicate::mCommunicatePack[15]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,15);
	//发送到服务器
	//Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mCommunicatePack,23);
	Communicate::ReturnBytesToClient(mWIFI,0,Communicate::mCommunicatePack,16);
}

////////////////////////
///响应主控的禁用节点的请求
////////////////////////
void APP::AckDisableNode()
{
	//帧数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	char mac[6]={0,0,0,0,0,0};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制节点的mac地址
	Communicate::mCommunicatePack[10]=0;//消息体长度高位：0
	Communicate::mCommunicatePack[11]=1;//消息体长度低位：5
	Communicate::mCommunicatePack[12]=To_NODE_cAckCompletLead>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=To_NODE_cAckCompletLead&0x00ff;//命令字低字节
	Communicate::mCommunicatePack[14]=1;
	Communicate::mCommunicatePack[15]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,15);
	//发送到服务器
	//Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mCommunicatePack,23);
	Communicate::ReturnBytesToClient(mWIFI,0,Communicate::mCommunicatePack,16);
}





