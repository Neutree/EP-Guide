#include "TaskManager.h"

TaskManager tskmgr;

double TaskManager::_it_time = 0;
double TaskManager::_new_time = 0;
double TaskManager::_old_time = 0;

//Constructor
TaskManager::TaskManager()
{
	SysTick->CTRL &= 0xFFFFFFFB; //Clock div 8 = 9M
	SysTick->LOAD = 16200000;     //1.8s
	SysTick->CTRL |= 0x00000003; //INT +ENABLE
}	
//get current time since power on
double TaskManager::Time(void)
{
	_new_time = _it_time + 1.8 - SysTick->VAL/9000000.0; //update current time
	
	if(_new_time - _old_time > 1.799) //check if breaked by SysTick interrupt
	{	
		_new_time -= 1.8;	              //calibrate current time
	}		          
	_old_time = _new_time;            //update old time
	return _new_time;
}

//SysTick interrupt IRQ handler
extern "C"
{
	void SysTick_Handler(void)
	{
		TaskManager::_it_time += 1.8;
	}
}


