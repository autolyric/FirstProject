#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "common.h"
#include "pit.h"
#include "sim.h"
#include "uart.h"
#include "ics.h"
#include "gpio.h"
#include "adc.h"
#include "i2c.h"
#include "Tick.h"
#include "spi.h"
#include "ftm.h"
#include "mscan.h"


#define SIM_SCGC_VALUE      0x00003000L  


#define CAN_IDAR0		0XFFFFFFFF   // <<1 is for RTR IDAR0-3  0X802
#define CAN_IDAR1		0XFFFFFFFF   //IDAR4-7   0X803
#define CAN_IDMR0		0XFFFFFFFF  
#define CAN_IDMR1   0XFFFFFFFF

#define CAN_BUFFER_LENGTH		8


typedef struct
{
	MSCAN_RegisterFrameType	sFrameRegisterBuffer[CAN_BUFFER_LENGTH];
	uint8_t u8Head;
	uint8_t u8Index;
	uint8_t u8FreeLength;
}FrameBufferInfoType,*FrameBufferInfoPtr;

void System_Init(void);
void TestCAN_SendCycle(void);
void Task_Power_Manage(void);


#endif
