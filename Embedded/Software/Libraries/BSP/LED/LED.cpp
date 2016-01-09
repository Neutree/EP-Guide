#include "LED.h"


///构造函数
///@param gpio GPIO类对象，分配片上资源
///@param highLevelOn  是否输出高电平时为LED打开
/////////////////////////
LED::LED(GPIO &gpio,bool highLevelOn)
	:mGPIO(gpio)
{
	if(highLevelOn)
		mLevelOn=1;
	else
		mLevelOn=0;
}


////////////////////////
///翻转（开-->关 ， 关-->开）
///////////////////////
void LED::Toggle()
{
	if(mGPIO.GetLevel())
		mGPIO.SetLevel(0);
	else
		mGPIO.SetLevel(1);
}

//////////////////////
///打开LED
/////////////////////
void LED::On()
{
	mGPIO.SetLevel(mLevelOn);
}

/////////////////////
///关闭LED
////////////////////
void LED::Off()
{
	mGPIO.SetLevel(mLevelOn^1);
}



