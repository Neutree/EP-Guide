#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_

extern "C"{
#include "stm32f10x.h"
void SysTick_Handler(void);
}

class TaskManager
{
public:
	volatile static u32 _it_cnt;      //SysTick interrupt counter
public:		
	TaskManager();
	static double Time(void);
	static void DelayUs(u16 nus);
	static void DelayMs(u16 nms);
	static void DelayS(u16 s);
};

extern TaskManager tskmgr;

#endif
