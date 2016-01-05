# include "MFRC522.h"


MFRC522::MFRC522(USART *usart,GPIO *reset,GPIO *pctrl)
:mUsart(*usart),mResetPin(*reset),mPctrlPin(*pctrl)
{
	
	
}
void MFRC522::PCDReset()
{
	if(&mPctrlPin!=0x000000)//使用了硬件电源控制
	{
		//打开电源控制
		mPctrlPin.SetLevel(0);
		
	}
	if(&mResetPin!=0)
	{
		//复位使能
		mResetPin.SetLevel(1);
		TaskManager::DelayUs(1);
		mResetPin.SetLevel(0);
		TaskManager::DelayUs(1);
		mResetPin.SetLevel(1);
		TaskManager::DelayUs(1);
	}
	//向“启动/停止”寄存器写复位命令，即打开模拟电路、打开电源
	WriteRawRC(MFRC522_CommandReg,MFRC522_PCD_RESETPHASE);
	TaskManager::DelayUs(1);
	WriteRawRC(MFRC522_ModeReg,0x3D);   //定义发送和接收的模式，和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(MFRC522_TReloadRegL,30); //定时器重装值
    WriteRawRC(MFRC522_TReloadRegH,0);  //定时器重装值
    WriteRawRC(MFRC522_TModeReg,0x8D);  //定时器模式设置
    WriteRawRC(MFRC522_TPrescalerReg,0x3E);//预分频设置
    WriteRawRC(MFRC522_TxAskReg,0x40);     //传输调制设置：强制100％ASK调制独立的ModGsPReg的寄存器设置
	
	PcdAntennaOff();//关载波
	TaskManager::DelayMs(1);
	PcdAntennaOn();//开载波
	TaskManager::DelayMs(1);
}


bool MFRC522::WriteRawRC(unsigned char address, unsigned char value)
{
	double timeOut=TaskManager::Time();
	u8 reteunval;
	mUsart.ClearReceiveBuffer();
	//最高位为0，表示写数据
	/*	address = ((address<<1)&0x7E);*///！！！！这里不使用这个，部分文档说明不一，请根据所使用硬件更改
	address = address&0x7F;
	mUsart.SendData(&address,1);
	while(mUsart.ReceiveBufferSize()<=0)
	{
		if(TaskManager::Time()-timeOut>0.010)//大于10ms判定是超时，时间可改
			return false;
	}
	mUsart.GetReceivedData(&reteunval,1);
	if(reteunval!=address)//返回的地址与发的不相同，错误
		return false;
	mUsart.SendData(&value,1);
	return true;
}

unsigned char MFRC522::ReadRawRC(unsigned char address)
{
	double timeOut=TaskManager::Time();
	mUsart.ClearReceiveBuffer();
//	address = ((address<<1)&0x7E)|0x80;
	address = (address&0x7F)|0x80;
	mUsart.SendData(&address,1);
	while(mUsart.ReceiveBufferSize()<=0)
	{
		if(TaskManager::Time()-timeOut>0.010)//大于10ms判定是超时，时间可改
			break;
	}
	unsigned char temp;
	mUsart.GetReceivedData(&temp,1);
	return temp;
}

void MFRC522::ClearBitMask(unsigned char reg, unsigned char mask)
{
	char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
}
void MFRC522::SetBitMask(unsigned char reg,unsigned char mask)
{
	char tmp = 0x00;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}
void MFRC522::PcdAntennaOn()
{
	unsigned char i;
    i = ReadRawRC(MFRC522_TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(MFRC522_TxControlReg, 0x03);
    }
}

void MFRC522::PcdAntennaOff()
{
	ClearBitMask(MFRC522_TxControlReg,0x03);//停止发送13.56MHz的载波
}

bool MFRC522::PcdRequest(unsigned char whichTag,unsigned char *pTagType)
{
	bool status=false;
	unsigned int  unLen=0;                                //存放接收到的数据的长度
	unsigned char ucComMF522Buf[MFRC522_MaxReceiveLen];  //存放接收到的数据,这里不直接使用pTagType是保险，防止用户传入长度太短的数组，导致数组越界程序崩溃

	ClearBitMask(MFRC522_Status2Reg,0x08);//清MIFAREe认证标志
	WriteRawRC(MFRC522_BitFramingReg,0x07);//面向位的帧调节，置位最后一个字节的位数，当此标志位0时，代表数据发送完毕
	SetBitMask(MFRC522_TxControlReg,0x03);//使能管脚TX1和TX2经载波后发送


	status = PcdComPicc(MFRC522_PCD_TRANSCEIVE,&whichTag,1,ucComMF522Buf,&unLen);

	if ((status == true) && (unLen == 0x10))
	{    
	   *pTagType     = ucComMF522Buf[0];
	   *(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   status = false;   }

	return status;
}



bool MFRC522::PcdComPicc(unsigned char Command,unsigned char *pDataToPicc,unsigned char toPiccLength,
	                                       unsigned char *pDataInPcd, unsigned int  *pInBitLength)
{
    bool status = false;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
       case MFRC522_PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case MFRC522_PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
   
    WriteRawRC(MFRC522_ComIEnReg,irqEn|0x80);
    ClearBitMask(MFRC522_ComIrqReg,0x80);
    WriteRawRC(MFRC522_CommandReg,MFRC522_PCD_IDLE);
    SetBitMask(MFRC522_FIFOLevelReg,0x80);
    
    for (i=0; i<toPiccLength; i++)
    	WriteRawRC(MFRC522_FIFODataReg, pDataToPicc[i]);
    WriteRawRC(MFRC522_CommandReg, Command);
   
    
    if (Command == MFRC522_PCD_TRANSCEIVE)
    {    SetBitMask(MFRC522_BitFramingReg,0x80);  }
    
    i = 0;//操作M1卡最大等待时间25ms
	double timeOut=TaskManager::Time();
    do 
    {
        
		if(TaskManager::Time()-timeOut > 1.027)
		{
			i=1;//超时
			break;
		}
		n = ReadRawRC(MFRC522_ComIrqReg);//获取中断标志寄存器
    }while (!(n&0x01) && !(n&waitFor));
    ClearBitMask(MFRC522_BitFramingReg,0x80);
	      
    if (i==0)//没有超时
    {    
		char temp=(ReadRawRC(MFRC522_ErrorReg));
         if(!(temp&0x1B))
         {
             status = true;
             if (n & irqEn & 0x01)
                status = false;
             if (Command == MFRC522_PCD_TRANSCEIVE)
             {
				n = ReadRawRC(MFRC522_FIFOLevelReg);//获取FIFO中数据的长度
				lastBits = ReadRawRC(MFRC522_ControlReg) & 0x07;//最后接收到的字节的有效位数，为零时整个字节有效

				if (lastBits)
					*pInBitLength = (n-1)*8 + lastBits;
				else
					*pInBitLength = n*8;

				if (n == 0)//队列中没有数据
					n = 1;

				if (n > MFRC522_MaxReceiveLen)//长度超过了最大长度
					n = MFRC522_MaxReceiveLen;

				for (i=0; i<n; i++)
					pDataInPcd[i] = ReadRawRC(MFRC522_FIFODataReg);
            }
         }
         else
            status = false;
        
   }
   

   SetBitMask(MFRC522_ControlReg,0x80);           // stop timer now
   WriteRawRC(MFRC522_CommandReg,MFRC522_PCD_IDLE); //取消当前命令的执行
   return status;
}

