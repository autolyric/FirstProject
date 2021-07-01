/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "common.h"


#include "Input.h" 
#include "Output.h"
#include "DisplayLCD.h"
#include "LIN_Slave.h"
#include "MotorCtrl.h"
#include "System.h"


void delay()
{
	unsigned int delay=0x8FFFF;
	while(delay--);
}

int main(void)
{
	LED0_Init();
	LED1_Init();
	System_Init();
	Input_Init();
	OutputCtrl_Init();
	Motor_Ctrl_Init();
	Lin_Slave_Init();
	Lcd_Display_Init();
	
	while(1)  
	{
		Task_Dg_Scan();
		Task_Ad_Scan();
		Task_Dg_Ctrl();
		Task_Fan_Ctrl();
		Task_Motor_Ctrl();
		Task_LIN_Slave();
		Task_Lcd_Display();
		GPIO_PinClear(GPIO_PTA0);
		//LED0_Toggle();
		//delay();
	}	
	return 0;
}



