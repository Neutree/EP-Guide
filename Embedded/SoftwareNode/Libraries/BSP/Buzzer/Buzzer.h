#ifndef __BUZZER_H
#define __BUZZER_H

#include "GPIO.h"
class Buzzer
{
	public:
		////////////////////////
		///构造函数
		///@param gpio GPIO类对象，分配片上资源
		///@param highLevelOn  是否输出高电平时为Buzzer打开
		/////////////////////////
		Buzzer(GPIO &gpio,bool highLevelOn=true);
	
	
		////////////////////////
		///翻转（开-->关 ， 关-->开）
		///////////////////////
		void Toggle();
	
		//////////////////////
		///打开Buzzer
		/////////////////////
		void On();
	
		/////////////////////
		///关闭Buzzer
		////////////////////
		void Off();
	private:
		//////////////////////////
		///GPIO的类的实例对象引用
		//////////////////////////
		GPIO & mGPIO;
		////////////////////////
		///标志开启Buzzer时取的电平高低
		////////////////////////
		unsigned char mLevelOn;
};
#endif

