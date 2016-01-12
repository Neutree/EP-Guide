#include "Buzzer.h"



///构造函数
///@param gpio GPIO类对象，分配片上资源
///@param highLevelOn  是否输出高电平时为Buzzer打开
/////////////////////////
Buzzer::Buzzer(GPIO &gpio,bool highLevelOn)
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
void Buzzer::Toggle()
{
	if(mGPIO.GetLevel())
		mGPIO.SetLevel(0);
	else
		mGPIO.SetLevel(1);
}

//////////////////////
///打开Buzzer
/////////////////////
void Buzzer::On()
{
	mGPIO.SetLevel(mLevelOn);
}

/////////////////////
///关闭Buzzer
////////////////////
void Buzzer::Off()
{
	mGPIO.SetLevel(mLevelOn^1);
}



