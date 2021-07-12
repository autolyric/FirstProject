/*
 * main implementation: use this 'C' sample to create your own application
 *
 */






#include "derivative.h" /* include peripheral declarations */
#include "common.h"
#include "System.h"



const int const_data __attribute__((at(0x00001000))) = 10; 


void TestLED_Toggle(void)
{
	static uint16 cycle_time=FOREMOST_TIME;
	if(Get2MsTickInterval(cycle_time)>=1000)
	{
		cycle_time=Get2MsTickVal();
		GPIO_PinToggle(GPIO_PTH0);         
	}
}

int main(void)
{
	LED0_Init();
	LED1_Init();
	LED2_Init();
	
	System_Init();
	while(1)  
	{
		TestLED_Toggle();
		TestCAN_SendCycle();
	}	
}



