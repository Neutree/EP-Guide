#include "TaskManager.h"

TaskManager tskmgr;

volatile u32 TaskManager::_it_cnt = 0;
//Constructor
TaskManager::TaskManager()
{
	SysTick->CTRL &= 0xFFFFFFFB; //Clock div 8 = 9M
	SysTick->LOAD = 9000000;     //1s
	SysTick->CTRL |= 0x00000003; //INT +ENABLE
}	
//get current time since power on
double TaskManager::Time(void)
{
	static double newT = 0, oldT = 0;
	newT = _it_cnt + 1.0 - double(SysTick->VAL) / 9000000.0;
	if(newT-oldT>0.99) newT -= 1;
	oldT = newT;
	return newT;
}

//微秒延时
void TaskManager::DelayUs(u16 nus)
{
	double OldT=Time();
	while((Time()-OldT)<double(nus)/1000000.0);
}

//毫秒延时
void TaskManager::DelayMs(u16 nms)
{
	double OldT=Time();
	while((Time()-OldT)<double(nms)/1000.0);
}

void TaskManager::DelayS(u16 s)
{
	double OldT=Time();
	while((Time()-OldT)<double(s));
}

//SysTick interrupt IRQ handler
extern "C"
{
	void SysTick_Handler(void)
	{
		TaskManager::_it_cnt++;
	}
}
