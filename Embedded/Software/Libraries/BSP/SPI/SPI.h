/**
  *@file SPI.h
  *@frief SPI driver lib for stm32f10x
  *@author Neucrack(Neutree)
  *@version v0.1
  *@date create on 2016-01-06
  *      last update on 2016-01-06
  *@copyright CQUT IOT LIB all right reserved
  *@bug only for master mode now
  */

#ifndef __SPI_H_
#define __SPI_H_

#include "stm32f10x.h"
#include "TaskManager.h"


////////////////////////
///define the speed of SPI
////////////////////////
typedef enum
{
	SPI_SPEED_2  = 0,  //72/2=36MHz
	SPI_SPEED_8  = 1,  //72/8=9MHz
	SPI_SPEED_16 = 2,  //72/16=4.5MHz
	SPI_SPEED_256= 3   //72/256=281.25kHz
}SPI_Speed;

/////////////////////////
///define the first bit of transfer LSB or MSB
////////////////////////
typedef enum
{
	SPI_FirstBit_MSB_ = ((uint16_t)0x0000),
	SPI_FirstBit_LSB_ = ((uint16_t)0x0080)
}SPI_FirstBit;

class SPI{

public:	
	///////////////////////////
	///Constructor for SPI 
	///@param SPI choose which SPI to use SPI1 or SPI2
	///@param irqGpio interrupt GPIO when recieved data
	///@param irqPin  interrupt Pin when recieved data 
	///@param if remap pin
	///@param speed speed of SPI default:SPI_SPEED_256(281.25kHz) @see SPI_Speed 
	///@param firstBit the first bit of transfer LSB or MSB @see SPI_FirstBit  default:SPI_FirstBit_MSB_
	//////////////////////////
	SPI(SPI_TypeDef* spi,GPIO_TypeDef* irqGpio,uint16_t irqPin,bool remap=false,SPI_Speed speed=SPI_SPEED_256,SPI_FirstBit firstBit=SPI_FirstBit_MSB_);

	/////////////////////////////////////
	///@param speed the speed of SPI @see SPI_Speed
	//////////////////////////////////////
	void SetSpeed(SPI_Speed speed);

	/////////////////////////////////////
	///@retval speed the speed of SPI @see SPI_Speed
	//////////////////////////////////////
	SPI_Speed GetSpeed(void);


	///////////////////////////////
	///read a byte from slave or write a byte to slave 
	///@param dataToSend if write: the data will send to slave
	///                  if read : the slave register address to read
	///@retval if read(write) succeed
	///////////////////////////////
	bool ReadOrWriteByte(u8 dataTosend);

private:
	
	//
	SPI_TypeDef* mSPI;
	


	///////////////////////////
	///Constructor for SPI 
	///@param SPI choose which SPI to use SPI1(default) or SPI2
	///@param irqGpio interrupt GPIO when recieved data
	///@param irqPin  interrupt Pin when recieved data 
	///@param if remap pin
	///@param speed speed of SPI @see SPI_Speed default:SPI_SPEED_16
	///@param firstBit the first bit of transfer LSB or MSB @see SPI_FirstBit  default:SPI_FirstBit_MSB_
	//////////////////////////
	void Init(SPI_TypeDef* spi,GPIO_TypeDef* irqGpio,uint16_t irqPin,bool remap=false,SPI_Speed speed=SPI_SPEED_16,SPI_FirstBit firstBit=SPI_FirstBit_MSB_);
	
};


#endif


