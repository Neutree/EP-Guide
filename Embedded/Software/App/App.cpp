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
mCOM1(1,115200,true),mCOM2(2,9600,true),
mLedRed(mLedRedGPIO,false),mLedGreen(mLedGreenGPIO,false),mRFID(&mCOM2)
	
{
	
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
	
	TaskManager::DelayS(1);
	mLedGreen.On();
	TaskManager::DelayS(1);
	mLedGreen.Toggle();
	
	//PCD复位
	mRFID.PCDReset();
	
	
	
	
}

///////////////////////////////
///软件需要的初始化
//////////////////////////////
void APP::InitSoft()
{
	
}


/////////////////////////////////
///循环执行部分
/////////////////////////////////
void APP::Loop()
{
/****************--①--器件检测	、错误处理**********************/	


	//RFID 健康状况,器件存在问题时不会执行--②--
//	if(!mRFID.mHealth)
//	{
//		//一直侦测健康状况
//		if(mRFID.Kick())//检测到，进行状态设置
//		{
//			mRFID.PCDReset();
//			return;
//		}
//		//RFID连接失败处理
//		
//			
//		return;
//	}
	//WIFI健康状况检查
	
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
	mCOM1<<"find fail\n";
	mLedGreen.Off();
}
mCOM1<<"version:"<<mRFID.ReadRawRC(0x37)<<"\n\n";
/***************************************************************/
	
	
}













