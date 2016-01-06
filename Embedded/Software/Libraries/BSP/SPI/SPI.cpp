/**
  *@file SPI.cpp
  *@frief SPI driver lib for stm32f10x
  *@author Neucrack(Neutree)
  *@version v0.1
  *@date create on 2016-01-06
  *      last update on 2016-01-06
  *@copyright CQUT IOT LIB all right reserved
  *@bug only for master mode now
  */
  
  
# include "SPI.h"

///////////////////////////
///Constructor for SPI 
///@param spiNumber choose which SPI to use 1(default) or 2
///@param if remap pin
///@param speed speed of SPI @see SPI_Speed default:SPI_SPEED_16
///@param firstBit the first bit of transfer LSB or MSB @see SPI_FirstBit  default:SPI_FirstBit_MSB_
//////////////////////////
SPI::SPI(SPI_TypeDef* spi,bool remap,SPI_Speed speed,SPI_FirstBit firstBit)
:mSPI(spi)
{
	Init(spi,remap,speed,firstBit);
}

/////////////////////////////////////
///@param speed the speed of SPI @see SPI_Speed
//////////////////////////////////////
void SPI::SetSpeed(SPI_Speed SpeedSet)
{
	mSPI->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//二分频
	{
		mSPI->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz
	}else if(SpeedSet==SPI_SPEED_8)//八分频 
	{
		mSPI->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz
	}else if(SpeedSet==SPI_SPEED_16)//十六分频
	{
		mSPI->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else //256分频
	{
		mSPI->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz 低速模式
	}
	mSPI->CR1|=1<<6; //SPI设备使能
}

/////////////////////////////////////
///@retval speed the speed of SPI @see SPI_Speed
//////////////////////////////////////
SPI_Speed SPI::GetSpeed(void)
{
	switch((mSPI->CR1&0x38)>>3)
	{
		case 0:
			return SPI_SPEED_2;
		case 2:
			return SPI_SPEED_8;
		case 3:
			return SPI_SPEED_16;
		default:
			return SPI_SPEED_256;
	}
}

///////////////////////////
///Constructor for SPI 
///@param SPI choose which SPI to use SPI1(default) or SPI2
///@param if remap pin
///@param speed speed of SPI @see SPI_Speed default:SPI_SPEED_16
///@param firstBit the first bit of transfer LSB or MSB @see SPI_FirstBit  default:SPI_FirstBit_MSB_
//////////////////////////
void SPI::Init(SPI_TypeDef* spi,bool remap,SPI_Speed speed,SPI_FirstBit firstBit)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t MOSI_Pin,MISO_Pin,SCK_Pin;
	GPIO_TypeDef* SPI_GPIO;
	//开启SPI所用引脚的时钟
	if(mSPI==SPI2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
		//开启复用时钟//开启NSS 引脚对应的时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
		MOSI_Pin = GPIO_Pin_15;
		MISO_Pin = GPIO_Pin_14;
		SCK_Pin  = GPIO_Pin_13;
		mNSS_Pin  = GPIO_Pin_12;
		SPI_GPIO= GPIOB;
		mNSS_GPIO = GPIOB;
	}
	else if(mSPI==SPI3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
		if(remap)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
			MOSI_Pin = GPIO_Pin_12;
			MISO_Pin = GPIO_Pin_11;
			SCK_Pin  = GPIO_Pin_10;
			mNSS_Pin  = GPIO_Pin_4;
			SPI_GPIO= GPIOC;
			mNSS_GPIO = GPIOA;
		}
		else
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
			MOSI_Pin = GPIO_Pin_5;
			MISO_Pin = GPIO_Pin_4;
			SCK_Pin  = GPIO_Pin_3;
			mNSS_Pin  = GPIO_Pin_15;
			SPI_GPIO= GPIOB;
			mNSS_GPIO = GPIOA;
		}
	}
	else
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
		if(remap)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
			MOSI_Pin = GPIO_Pin_5;
			MISO_Pin = GPIO_Pin_4;
			SCK_Pin  = GPIO_Pin_3;
			mNSS_Pin  = GPIO_Pin_15;
			SPI_GPIO= GPIOB;
			mNSS_GPIO = GPIOA;
		}
		else
		{
			//开启复用时钟//开启CSN 引脚对应的时钟
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
			MOSI_Pin = GPIO_Pin_7;
			MISO_Pin = GPIO_Pin_6;
			SCK_Pin  = GPIO_Pin_5;
			mNSS_Pin  = GPIO_Pin_4;
			SPI_GPIO= GPIOA;
			mNSS_GPIO = GPIOA;
		}
	}
	
	
	//配置 NSS引脚 
	GPIO_InitStructure.GPIO_Pin = mNSS_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//PA9时钟速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	GPIO_Init(mNSS_GPIO, &GPIO_InitStructure);
	
	//配置SCK MISO MOSI引脚 ，复用输出
	/*!< Configure SPI_FLASH_SPI pins: SCK MISO MOSI*/
	GPIO_InitStructure.GPIO_Pin = MISO_Pin|MOSI_Pin|SCK_Pin;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_GPIO, &GPIO_InitStructure);

	
	SPI_I2S_DeInit(mSPI);                          
	SPI_Cmd(mSPI, DISABLE); //必须先禁用,才能改变MODE
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //主
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;      //8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;        //CPOL=0 时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;       //CPHA=0 数据捕获第1个
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //软件NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8 ; //8分频 9MHz
	SPI_InitStructure.SPI_FirstBit = firstBit;      //高(低)位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;        //CRC7

	SPI_Init(mSPI, &SPI_InitStructure);
	SPI_Cmd(mSPI, ENABLE);
	
	SetSpeed(speed);//设置速度
	//this->ReadWriteByte(0xff);   //启动传输
}

///////////////////////////////
///read a byte from slave or write a byte to slave 
///@param dataToSend if write: the data will send to slave
///                  if read : the slave register address to read
///@dataReturn allow empty   if write: the same data with send data
///                          if read : the slave register's data
///@retval if read or write succeed
///////////////////////////////
bool SPI::ReadOrWriteByte(u8 dataTosend,u8 *dataReturn)
{
	u8 retry=0;
	EnableSPI();
	while((mSPI->SR&0x02)==0)//等待发送区空//while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
	{
		retry++;
		if(retry>200)
		{
			DisableSPI();
			return false;
		}
	}  
	mSPI->DR=dataTosend;  //发送一个byte //SPI_I2S_SendData(SPI2,data);
	retry=0;
	while((mSPI->SR&0x01)==0) //等待接收完一个byte  //while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
	{
		retry++;
		if(retry>200)
		{
			DisableSPI();
			return false;
		}
	}
	if(dataReturn)
		*dataReturn=mSPI->DR;          //返回收到的数据 //SPI_I2S_ReceiveData(SPI2);
	DisableSPI();
	return true;
}


//////////////////////////
///使能SPI（使能NSS）
//////////////////////////
void SPI::EnableSPI(void)
{
	mNSS_GPIO->BRR = mNSS_Pin;
}

//////////////////////////
///失能SPI（使能NSS）
//////////////////////////
void SPI::DisableSPI(void)
{
	mNSS_GPIO->BSRR = mNSS_Pin;
}

