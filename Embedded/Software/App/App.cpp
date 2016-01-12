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
mBuzzerGPIO(GPIOB,5,GPIO_Mode_Out_PP,GPIO_Speed_50MHz),
mCOM1(1,115200,true),mCOM2(2,9600,true),
mLedRed(mLedRedGPIO,false),mLedGreen(mLedGreenGPIO,false),
mBuzzer(mBuzzerGPIO,false),
mRFID(&mCOM2),
mWIFI(mCOM1,115200)	
{
	mReqLinkCheckInterval=10;        //心跳包间隔30s
	mLogInFailRetryInterval=5;       //登录失败重试间隔间隔定义 单位：S
	mToServerConnectionHealth=-1;    //与服务器的连接初始化为失去连接
	mToServerLogInStatus=-1;     	//未登录
	mIsleadNow=false;                //未进行引导动作
	
	//车位信息初始化
	mAllNodeInfo.number=0;
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
	LogIn();
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
		mBuzzer.On();
	else
		mBuzzer.Off();
//获取时间
	static double timeNew=TaskManager::Time();

	timeNew=TaskManager::Time();
	
/*	//RFID 健康状况,器件存在问题时不会执行--②--
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
	
	
	//WIFI健康状况检查
	if(mWIFI.mHealth!=0)//WIFI 存在问题
	{
		mBuzzer.On();
		mWIFI.kick();//检测是否恢复连接
		haveWrong=true;
		return ;
	}
	
	

	
	//登录状态
	if(mToServerLogInStatus==-1)//没有登录
	{
		static double timeOld=TaskManager::Time();
//		mCOM1<<"LOG:not log in\n";
//		mCOM1<<timeNew<<"\t"<<timeOld<<"\n";
		if(timeNew-timeOld>=mLogInFailRetryInterval)//登录失败重新登录
		{
			timeOld=timeNew;
			//尝试重新登录，如果失败，返回，成功则继续进行下面的动作
			LogIn();
		}
		else if(timeNew-timeOld>=3)
			mLedRed.Blink2(mLedGreen,4,200);
		haveWrong=true;
		return;
	}
	
	//链路状态检查
	if(mToServerConnectionHealth==-1)//失去与服务器的连接
	{
//		mCOM1<<"LOG:connection fail\n";
		mToServerLogInStatus=-1;//登录标志置位未登录
		HeartBeatRequest();
		WaitHeartBeatRequestAck();
		haveWrong=true;
		return;
	}
	haveWrong=false;
*/
/*******************************************************************************/

	
/****************************--②--车辆检测**************************************/

/*******************************************************************************/
/*	FindCar();
*/
/*********************--③--接收到的信息处理（取出完整的有效数据帧）***************/
	

	

/*******************************************************************************/



/*************************--④--链路保持（心跳包），周期为30秒********************//*
static uint32_t heartBeatTimeOld=0;

//时间到发送链路请求
if(timeNew-heartBeatTimeOld>=mReqLinkCheckInterval)
{
	HeartBeatRequest();
	WaitHeartBeatRequestAck(); //应弃用，可能会导致阻塞
	heartBeatTimeOld=timeNew;
}
*/
/******************************************************************************/


/*****************************--⑤--轮询车位节点********************************/
unsigned char cardId[4];
unsigned char macAddr[6];
unsigned char ipAddr[4];
static double timeOld=TaskManager::Time();
if(timeNew-timeOld>15)//每2秒两轮询一个节点
{
	timeOld=timeNew;
/*
	static bool a=true;
	mMacBuffer[0][0]=0x12;
				mMacBuffer[0][1]=0x08;
				mMacBuffer[0][2]=0x07;
				mMacBuffer[0][3]=0x72;
				mMacBuffer[0][4]=0x07;
				mMacBuffer[0][5]=0x03;
				mLeadNumber=1;
	if(a)
		if(ReqAddNewNode(mMacBuffer[0]))
			mCOM1<<"add succcc\r\n";
		else
			mCOM1<<"add ffff\r\n";
	else
		if(ReqDelNode(mMacBuffer[0]))
			mCOM1<<"del succcc\r\n";
		else
			mCOM1<<"del ffffc\r\n";
		a=a?false:true;
*/
		
//	uint8_t status = QueryNodeStatus(cardId,macAddr,ipAddr);//包含掉线检查
//	if(status&NodeStatus_On_line)//在线
//	{
//		mCOM1<<"have node\r\n";
//		if(RoleStatus(macAddr)==0)//新的节点，发送新的节点请求
//		{
//			mCOM1<<"\r\nit is New\r\n";
//			int8_t status=/*ReqAddNewNode(macAddr)*/0;
//			if(status==0)//请求添加成功,添加到节点信息中
//			{
////				mLedRed.Blink(2,200);
//				mCOM1<<"\r\nadd success\r\n";
//				memcpy(mAllNodeInfo.nodeInfo[mAllNodeInfo.number].ipAddress,ipAddr,4);
//				memcpy(mAllNodeInfo.nodeInfo[mAllNodeInfo.number].macAdress,macAddr,6);
//				mAllNodeInfo.nodeInfo[mAllNodeInfo.number].status=NodeStatus_Free;
//				mAllNodeInfo.number+=1;
//			}
//			else if(status==-2)//请求被拒绝,向节点发送禁用请求，让节点在下次上电前都不再发送注册信息
//			{
////				mLedRed.Blink(4,200);
//				mCOM1<<"\n\nadd refuse\n\n";/*
//				if(ReqDisableNode(macAddr,ipAddr))
//				{
//				}
//					*/
//			}
//			else if(status==-1)
//			{
//				mCOM1<<"\n\nadd fail\n\n";
////				mLedRed.Blink(6,200);
//			}
//		}
//		else//不是新节点
//		{
//			mCOM1<<"\r\nnot New\r\n";
//			if(status & (NodeStatus_ToFree|NodeStatus_ToBusy))//车位状态更改,向服务器发送状态 更改请求
//			{
//				/*ReqChangeStatus(macAddr,(NodeStatus)(status&(NodeStatus_ToFree|NodeStatus_ToBusy)),cardId);*/
//				if((status&NodeStatus_ToBusy) && mIsleadNow)//是刚刚引导的车辆入库
//				{
//					//向节点发送请求，完成引导，关闭引导提示
//					ReqCompleteLead(mMacBuffer,mLeadNumber);
//					mIsleadNow=false;
//				}
//				if(status&NodeStatus_ToBusy)
//					mAllNodeInfo.nodeInfo[IndexOfMacAddress(macAddr)].status=NodeStatus_Busy;
//				else if(status&NodeStatus_ToFree)
//					mAllNodeInfo.nodeInfo[IndexOfMacAddress(macAddr)].status=NodeStatus_Free;
//			}
//			else if(status&NodeStatus_Free)//车位状态依然为空闲
//			{
//				mAllNodeInfo.nodeInfo[IndexOfMacAddress(macAddr)].status=NodeStatus_Free;
//			}
//			else if(status&NodeStatus_Busy)//车位状态依然为忙
//			{
//				mAllNodeInfo.nodeInfo[IndexOfMacAddress(macAddr)].status=NodeStatus_Busy;
//			}
//		}
//	}
//	else
//		mCOM1<<"No Node2\r\n\r\n";
}


/******************************************************************************/

}
/////////////////////////////////////////////////////////////循环执行部分结束/////////////////////////////////////////////////////////////////


void APP::HeartBeatRequest()
{
	mToServerConnectionHealth=0;//标志正在检测
	//心跳包数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	unsigned char mac[6]={1,2,3,4,5,6};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制mac地址
	Communicate::mCommunicatePack[10]=0;//消息体长度：0
	Communicate::mCommunicatePack[11]=0;//消息体长度：0
	Communicate::mCommunicatePack[12]=TO_SERVER_cReqLinkCheck>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=TO_SERVER_cReqLinkCheck&0x00ff;//命令字低字节
	
	Communicate::mCommunicatePack[14]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,14);
	
	Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mCommunicatePack,15);
	
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
						//关闭通信连接
						mWIFI.CloseMulitpleSend(4);
						
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
	//关闭通信连接
	mWIFI.CloseMulitpleSend(4);
	return false;
}

void APP::FindCar()
{
	if(mRFID.PcdRequest(MFRC522_PICC_REQIDL,mTagInfo))//寻到卡
	{
		if(((u16)mTagInfo[0]<<8|mTagInfo[1])==MFRC522_PICC_MIFARE_ONE_S50)//卡类型：S50
		{
			if(mRFID.PcdAntiColl(mTagInfo))//防冲撞成功(找到一张卡序列号)
			{
				mLedGreen.Blink(2,100);
				//mCOM1<<"ID:"<<mTagInfo[0]<<"\t"<<mTagInfo[1]<<"\t"<<mTagInfo[2]<<"\t"<<mTagInfo[3]<<"\n";
				//请求最短路径配置
			/*	if(ReqShorestLead(mMacBuffer,&mLeadNumber))
				{*/	
				
				mMacBuffer[0][0]=0x12;
				mMacBuffer[0][1]=0x08;
				mMacBuffer[0][2]=0x07;
				mMacBuffer[0][3]=0x72;
				mMacBuffer[0][4]=0x07;
				mMacBuffer[0][5]=0x02;
				mLeadNumber=1;
					//请求节点进行引导
					if(ReqLead(mMacBuffer,mLeadNumber))
					{
						mRFID.PcdHalt();//ID卡休眠（车离开门禁之前不再扫描）
						mIsleadNow=true;//标志正在进行引导
					}
			/*	}*/
			}
		}
	}
	mRFID.Kick();
//	mCOM1<<"version:"<<mRFID.ReadRawRC(0x37)<<"\n\n";
}


////////////////////////////
///登录到服务器
////////////////////////////
bool APP::LogIn()
{
	mToServerLogInStatus=0;//标记正在登录
	//帧数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	unsigned char mac[6]={0,0,0,0,0,0};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);  //MAC地址不发送，加密后放在消息体后发送（在mac地址串后面加"A-402"后加密）
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制mac地址

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
	Communicate::mCommunicatePack[10]=temp>>8;//消息体长度：高位
	Communicate::mCommunicatePack[11]=(uint8_t)temp;//消息体长度：低位
	Communicate::mCommunicatePack[12]=TO_SERVER_cReqGateWayLogin>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=TO_SERVER_cReqGateWayLogin&0x00ff;//命令字低字节
	memcpy(&Communicate::mCommunicatePack[14],decrypt,16);//复制加密后的mac地址
	Communicate::mCommunicatePack[14+temp]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,14+temp);
	
	Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mCommunicatePack,15+temp);
	
	//等待服务器响应（中间会执行RFID扫描）
	
	if(WaitReceiveAndDecode())
	{
		//链路没问题
		mToServerConnectionHealth=true;
		if(mBuffer[10]==0&&mBuffer[11]==2&&mBuffer[12]==(TO_SERVER_cAckGateWayLogin>>8)&&(mBuffer[13]==(u8)TO_SERVER_cAckGateWayLogin)&&mBuffer[14]==0)
		{
			if(mBuffer[15]==1)
				mToServerLogInStatus=1;//正确
			else
				mToServerLogInStatus=-1;//失败
		}
	}
	else//链路不通
	{
		mToServerConnectionHealth=false;
	}
	if(mToServerLogInStatus==0)//登录失败
		mToServerLogInStatus=-1;
	
	if(mToServerLogInStatus==1)
	{
		//闪两下
		mLedGreen.Blink2(mLedRed,2,200);
		return true;
	}
	else
	{
		//闪两下
		mLedGreen.Blink2(mLedRed,4,200);
		return false;
	}
}




///////////////////////////
///发送车位状态更改请求
///@param macAddress mac地址
///@param status 状态更改  取值:    NodeStatus_ToBusy   = 0x04, //状态改变,从空闲到有车
///                                 NodeStatus_ToFree   = 0x02  //从有车到空闲
///@param carID 车主ID
///@retval 请求是否成功
///////////////////////////
bool APP::ReqChangeStatus(unsigned char macAddress[6],NodeStatus status,unsigned char carID[4])
{
	//帧数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	unsigned char mac[6]={0,0,0,0,0,0};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制主控mac地址
	Communicate::mCommunicatePack[10]=0;//消息体长度高位：0
	Communicate::mCommunicatePack[11]=12;//消息体长度低位：0
	Communicate::mCommunicatePack[12]=TO_SERVER_cReqNodeStatusUpdate>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=TO_SERVER_cReqNodeStatusUpdate&0x00ff;//命令字低字节
	memcpy(&Communicate::mCommunicatePack[14],macAddress,6);//复制节点mac地址
	Communicate::mCommunicatePack[20]=0;//车位状态高位
	if(status&NodeStatus_ToBusy)
		Communicate::mCommunicatePack[21]=1;//车位状态低位，车位变成忙
	else if(status&NodeStatus_ToFree)
		Communicate::mCommunicatePack[21]=0;//车位变成空闲
	else //不是状态更改，错误
		return false;
	memcpy(&Communicate::mCommunicatePack[22],carID,4);//复制车主ID号
	Communicate::mCommunicatePack[26]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,26);
	//发送到服务器
	Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mCommunicatePack,27);
	//获取响应
	if(WaitReceiveAndDecode())
	{
		if(mBuffer[10]==0&&mBuffer[11]==2&&mBuffer[12]==(TO_SERVER_cAckNodeStatusUpdate>>8)&&(mBuffer[13]==(u8)TO_SERVER_cAckNodeStatusUpdate)&&mBuffer[14]==0)
		{
			if(mBuffer[15]==1)
				return true;
			else
				return false;
		}
	}
	return false;
}



///////////////////////////
///请求添加新的节点
///@param macAddress 新节点的mac地址
///@retval 是否成功 0：成功 -1：通信失败 -2：请求被拒绝
///////////////////////////
int8_t APP::ReqAddNewNode(unsigned char macAddress[6])
{
	//帧数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	unsigned char mac[6]={0,0,0,0,0,0};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制主控mac地址
	Communicate::mCommunicatePack[10]=0;//消息体长度高位：0
	Communicate::mCommunicatePack[11]=8;//消息体长度低位：0
	Communicate::mCommunicatePack[12]=TO_SERVER_cReqAddNode>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=TO_SERVER_cReqAddNode&0x00ff;//命令字低字节
	Communicate::mCommunicatePack[14]=0;
	Communicate::mCommunicatePack[15]=1;
	memcpy(&Communicate::mCommunicatePack[16],macAddress,6);//复制节点mac地址
	Communicate::mCommunicatePack[22]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,22);
	//发送到服务器
	Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mCommunicatePack,23);
	//获取响应
	if(WaitReceiveAndDecode())
	{
		if(mBuffer[10]==0&&mBuffer[11]==4&&mBuffer[12]==(TO_SERVER_cAckAddNode>>8)&&(mBuffer[13]==(u8)TO_SERVER_cAckAddNode)&&mBuffer[14]==0)
		{
			if(mBuffer[15]==1)
			{
				if(mBuffer[16]==0&&mBuffer[17]==0)//设置节点状态为空闲
				{
				}
				else//设置节点为忙
				{
					
				}
				return 0;
			}
			else//请求被拒绝
			{
				return -2;
			}
		}
	}
	return -1;
}


///////////////////////////
///请求删除节点
///@param macAddress 新节点的mac地址
///@retval 是否成功
///////////////////////////
bool APP::ReqDelNode(unsigned char macAddress[6])
{
		//帧数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	unsigned char mac[6]={0,0,0,0,0,0};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制主控mac地址
	Communicate::mCommunicatePack[10]=0;//消息体长度高位：0
	Communicate::mCommunicatePack[11]=8;//消息体长度低位：0
	Communicate::mCommunicatePack[12]=TO_SERVER_cReqNodeDel>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=TO_SERVER_cReqNodeDel&0x00ff;//命令字低字节
	Communicate::mCommunicatePack[14]=0;
	Communicate::mCommunicatePack[15]=1;
	memcpy(&Communicate::mCommunicatePack[16],macAddress,6);//复制节点mac地址
	Communicate::mCommunicatePack[22]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,22);
	//发送到服务器
	Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mCommunicatePack,23);
	//获取响应
	if(WaitReceiveAndDecode())
	{
		if(mBuffer[10]==0&&mBuffer[11]==2&&mBuffer[12]==(TO_SERVER_cAckNodeDel>>8)&&(mBuffer[13]==(u8)TO_SERVER_cAckNodeDel)&&mBuffer[14]==0)
		{
			if(mBuffer[15]==1)//删除成功
				return true;
		}
	}
	return false;
}


/////////////////////////////////////////
///向节点请求道路引导
///@param macAddress 引导节点顺序，最后一个为终点车位
///@param number 参与引导的车位数量
///@retval 是否成功
/////////////////////////////////////////
bool APP::ReqShorestLead(unsigned char macAddress[][6],uint16_t* number)
{
	//帧数据合成
	unsigned short statusTemp;
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	unsigned char mac[6]={0,0,0,0,0,0};
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制主控mac地址
	Communicate::mCommunicatePack[10]=(mAllNodeInfo.number*8+2)>>8;//消息体长度高位：0
	Communicate::mCommunicatePack[11]=(mAllNodeInfo.number*8+2);//消息体长度低位：0
	Communicate::mCommunicatePack[12]=TO_SERVER_cReqShortLoad>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=TO_SERVER_cReqShortLoad&0x00ff;//命令字低字节
	Communicate::mCommunicatePack[14]=mAllNodeInfo.number>>8;
	Communicate::mCommunicatePack[15]=mAllNodeInfo.number;
	for(uint16_t i=0;i<mAllNodeInfo.number;++i)
	{
		memcpy(&Communicate::mCommunicatePack[16+i*8],macAddress[i],6);//复制节点mac地址
		if(mAllNodeInfo.nodeInfo[i].status|NodeStatus_Busy)
			statusTemp=1;
		else
			statusTemp=0;
		Communicate::mCommunicatePack[16+i*8+6]=statusTemp>>8;
		Communicate::mCommunicatePack[16+i*8+7]=statusTemp;
	}
	Communicate::mCommunicatePack[mAllNodeInfo.number*8+16]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,17+mAllNodeInfo.number*8);
	//发送到服务器
	Communicate::SendBytesToServer(mWIFI,WIFI::mServerIPOrDomain,WIFI::mServerPort,Communicate::mCommunicatePack,17+mAllNodeInfo.number*8);
	//获取响应
	if(WaitReceiveAndDecode())
	{
		if(mBuffer[12]==(TO_SERVER_cAckShortLoad>>8)&&(mBuffer[13]==(u8)TO_SERVER_cAckShortLoad))
		{
			*number = ((unsigned short)mBuffer[14]<<8) | mBuffer[15];
			for(unsigned short i=0;i<*number;++i)
			{
				memcpy(macAddress,&mBuffer[16+i*6],6);
			}
			return true;
		}
	}
	return false;
}





////////////////////////////////////////
///轮询节点信息
///@param 车的编号（RFID卡的编号），车位状态改变时carID有值，进（出）车库的车的编号
///@param 车位的mac地址
///@retval 节点状态 
///////////////////////////////////////
uint8_t APP::QueryNodeStatus(unsigned char carID[4],unsigned char macAddress[6],unsigned char ipAddress[4])
{
	static uint8_t queryNodeCount=0,NodeNumber=0;
	if(queryNodeCount==0)
	{
		//将所有的节点的是否已经询问了的标志复位
		for(uint16_t i=0;i<mAllNodeInfo.number;++i)
			mAllNodeInfo.nodeInfo[i].checked=false;
		//获取连接到主控的节点ip
		char* ipStr = mWIFI.getJoinedDeviceIP();
		NodeNumber = WIFI::IPStringsToBytes(ipStr,mIPBuffer);
		if(NodeNumber==0)//没有节点
			return NodeStatus_OFF_Line;
	}	
	++queryNodeCount;//计数累加
	if(NodeNumber<queryNodeCount)//已经遍历每个节点了
	{
		//检测是否有节点没有检测到（失去连接了）
		for(uint16_t i=0;i<mAllNodeInfo.number;++i)
		{
			if(mAllNodeInfo.nodeInfo[i].checked==false)//没有检测到的节点,发送删除节点请求，如果失败，则报警，提示错误信息
			{
		/*		if(ReqDelNode(mAllNodeInfo.nodeInfo[i].macAdress))//删除节点成功
				{*/
					//删除这个节点的信息
					for(uint16_t j=i;j<mAllNodeInfo.number;++j)
					{
						mAllNodeInfo.nodeInfo[j]=mAllNodeInfo.nodeInfo[j+1];
					}
					mAllNodeInfo.number--;
		/*		}
				else                                              //删除失败
				{
					
				}*/
			}
		}
		queryNodeCount=0;
	}
	else//还没遍历完所有节点
	{
		//帧数据合成
		uint16_t temp=Communicate::ToServerGenerateMessageID();
		Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
		Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
/*		WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);  
		memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制mac地址
*/
		
		Communicate::mCommunicatePack[10]=0;//消息体长度：高位
		Communicate::mCommunicatePack[11]=1;//消息体长度：低位
		Communicate::mCommunicatePack[12]=To_NODE_cReqStatus>>8;//命令字高字节
		Communicate::mCommunicatePack[13]=To_NODE_cReqStatus&0x00ff;//命令字低字节
		Communicate::mCommunicatePack[14]=mAllNodeInfo.nodeInfo[queryNodeCount-1].reserved?1:0;
		Communicate::mCommunicatePack[15]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,15);
		char ip_[16];
		WIFI::IPBytesToString(mIPBuffer[queryNodeCount-1],ip_);
		Communicate::SendBytesToServer(mWIFI,(char*)ip_,WIFI::mNodePort,Communicate::mCommunicatePack,16);
		
		//等待服务器响应（中间会执行RFID扫描）
		
		if(WaitReceiveAndDecode())
		{
			mCOM1<<"wait ack succese 1\r\n";
			mAllNodeInfo.nodeInfo[queryNodeCount-1].checked=true;//标志应询问过了
			if(mBuffer[10]==0&&mBuffer[11]==5&&mBuffer[12]==(To_NODE_cAckStatus>>8)&&(mBuffer[13]==(u8)To_NODE_cAckStatus))
			{
				for(uint8_t i=0;i<6;++i)//mac地址
					macAddress[i]=mBuffer[i+4];
				for(uint8_t i=0;i<4;++i)//车辆ID号
					carID[i]=mBuffer[i+15];
				memcpy(ipAddress,mIPBuffer[queryNodeCount-1],4);
				return mBuffer[14]|NodeStatus_On_line;
			}
		}
		else
		{
			mCOM1<<"wait ack fail 1\r\n";
			return NodeStatus_OFF_Line;
		}
	}
	
	return NodeStatus_On_line;
}

/////////////////////////////////////////
///向节点请求道路引导
///@param macAddress 引导节点顺序，最后一个为终点车位
///@param number 参与引导的车位数量
///@retval 是否成功
/////////////////////////////////////////
bool APP::ReqLead(unsigned char macAddress[][6],uint16_t number)
{
	uint16_t count=0;
	for(;count<number;++count)//作为引导角色
	{
		//帧数据合成
		uint16_t temp=Communicate::ToServerGenerateMessageID();
		Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
		Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
/*		WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
		memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制mac地址
*/
		
		Communicate::mCommunicatePack[10]=0;//消息体长度：高位
		Communicate::mCommunicatePack[11]=0;//消息体长度：低位
		Communicate::mCommunicatePack[12]=To_NODE_cReqLead>>8;//命令字高字节
		Communicate::mCommunicatePack[13]=To_NODE_cReqLead&0x00ff;//命令字低字节
		if(count+1==number)
			Communicate::mCommunicatePack[14]=0x22;
		else
			Communicate::mCommunicatePack[14]=0x11;
		Communicate::mCommunicatePack[15]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,15);
		unsigned char ip_[4];
		if(!FindIPbyMacAddress(macAddress[count],ip_))
			return false;
		Communicate::SendBytesToServer(mWIFI,(char*)ip_,WIFI::mNodePort,Communicate::mCommunicatePack,16);
		
		//等待服务器响应（中间会执行RFID扫描）
		
		if(WaitReceiveAndDecode())
		{
			if(mBuffer[10]==0&&mBuffer[11]==1&&mBuffer[12]==(To_NODE_cAckLead>>8)&&(mBuffer[13]==(u8)To_NODE_cAckLead))
			{
				if(mBuffer[14]==1)
				{
					continue;
				}
				else if(mBuffer[14]==0)
				{
					return false;
				}
			}
		}
		return false;
	}
	return true;
}



/////////////////////////////////////////
///向节点请求道路引导
///@param macAddress 引导节点顺序，最后一个为终点车位
///@param number 参与引导的车位数量
///@retval 是否成功
/////////////////////////////////////////
bool APP::ReqCompleteLead(unsigned char macAddress[][6],uint16_t number)
{
	uint16_t count=0;
	for(;count<number;++count)//作为引导角色
	{
		//帧数据合成
		uint16_t temp=Communicate::ToServerGenerateMessageID();
		Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
		Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
/*		WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
		memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制mac地址
*/
		
		Communicate::mCommunicatePack[10]=0;//消息体长度：高位
		Communicate::mCommunicatePack[11]=0;//消息体长度：低位
		Communicate::mCommunicatePack[12]=To_NODE_cReqLead>>8;//命令字高字节
		Communicate::mCommunicatePack[13]=To_NODE_cReqLead&0x00ff;//命令字低字节
		Communicate::mCommunicatePack[14]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,14);
		unsigned char ip_[4];
		if(!FindIPbyMacAddress(macAddress[count],ip_))
			return false;
		Communicate::SendBytesToServer(mWIFI,(char*)ip_,WIFI::mNodePort,Communicate::mCommunicatePack,15);
		
		//等待服务器响应（中间会执行RFID扫描）
		
		if(WaitReceiveAndDecode())
		{
			if(mBuffer[10]==0&&mBuffer[11]==1&&mBuffer[12]==(To_NODE_cAckLead>>8)&&(mBuffer[13]==(u8)To_NODE_cAckLead))
			{
				if(mBuffer[14]==1)
				{
					continue;
				}
				else if(mBuffer[14]==0)
				{
					return false;
				}
			}
		}
		return false;
	}
	return true;
}

////////////////////////////////////////
///请求禁用节点
///@param macAddr 请求禁用的mac地址
///@param ipAddr  请求禁用的ip地址
///@retval 是否禁用成功
////////////////////////////////////////
bool APP::ReqDisableNode(uint8_t macAddr[6],uint8_t ipAddr[4])
{
	//帧数据合成
	uint16_t temp=Communicate::ToServerGenerateMessageID();
	Communicate::mCommunicatePack[2]=temp>>8;//消息ID高字节
	Communicate::mCommunicatePack[3]=temp&0x00ff;//消息ID低字节
	unsigned char mac[6];
	WIFI::MacAddressStringToBytes((char*)WIFI::mStationMac,mac);
	memcpy(&Communicate::mCommunicatePack[4],mac,6);//复制mac地址

	
	Communicate::mCommunicatePack[10]=0;//消息体长度：高位
	Communicate::mCommunicatePack[11]=0;//消息体长度：低位
	Communicate::mCommunicatePack[12]=To_NODE_cReqDisable>>8;//命令字高字节
	Communicate::mCommunicatePack[13]=To_NODE_cReqDisable&0x00ff;//命令字低字节
	Communicate::mCommunicatePack[14]=MathTool::CheckSum8((unsigned char*)Communicate::mCommunicatePack,14);

	Communicate::SendBytesToServer(mWIFI,(char*)ipAddr,WIFI::mNodePort,Communicate::mCommunicatePack,15);
	
	//等待服务器响应（中间会执行RFID扫描）
	
	if(WaitReceiveAndDecode())
	{
		if(mBuffer[10]==0&&mBuffer[11]==1&&mBuffer[12]==(To_NODE_cReqDisable>>8)&&(mBuffer[13]==(u8)To_NODE_cReqDisable))
		{
			if(mBuffer[14]==1)
			{
				return true;
			}
		}
	}
	return false;
}

/////////////////////////////////////////
///查询节点的MAC地址是否在节点信息中，以及是否被禁用
///@retval 0:没有该节点信息，是新的节点  1：有该节点的信息，是已经添加了的节点 
/////////////////////////////////////////
uint8_t APP::RoleStatus(uint8_t macAddress[6])
{
	for(uint8_t i=0;i<mAllNodeInfo.number;++i)
	{
		if(memcmp(macAddress,mAllNodeInfo.nodeInfo[i].macAdress,6)==0)//找到节点信息
		{
			return 1;
		}
	}
	return 0;
}





/////////////////////////////////////////
///根据mac地址寻找节点的IP地址
/////////////////////////////////////////
bool APP::FindIPbyMacAddress(uint8_t macAddress[6],uint8_t ip[4])
{
	for(uint8_t i=0;i<mAllNodeInfo.number;++i)
	{
		if(memcmp(macAddress,mAllNodeInfo.nodeInfo[i].macAdress,6)==0)//找到节点信息
		{
			memcpy(ip,mAllNodeInfo.nodeInfo[i].ipAddress,4);
			return true;
		}
	}
	return false;
}

////////////////////////////////////////
///寻找mac地址相对应的节点的下标
///@param macAddress mac地址
///@retval 下标，如果没找到，返回0xffff
////////////////////////////////////////
uint16_t APP::IndexOfMacAddress(uint8_t macAddress[6])
{
	for(uint8_t i=0;i<mAllNodeInfo.number;++i)
	{
		if(memcmp(macAddress,mAllNodeInfo.nodeInfo[i].macAdress,6)==0)//找到节点信息
		{
			memcpy(macAddress,mAllNodeInfo.nodeInfo[i].macAdress,6);
			return i;
		}
	}
	return 0xffff;
}



