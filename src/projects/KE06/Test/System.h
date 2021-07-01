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


#include "input.h"
#include "LIN_Slave.h"
#include "MotorCtrl.h"




typedef struct
{
	uint8_t sleep;
	uint8_t power;
	uint8_t onoff;
}MoudleStat_T;


#define MOUDLE_SLEEP   0x01
#define MOUDLE_AWAKE   0x01


#define  MOUDLE_POWER_OVERLOW  0x00
#define  MOUDLE_POWER_LOW      0x01
#define  MOUDLE_POWER_NORMAL   0x02
#define  MOUDLE_POWER_HIGH     0x03
#define  MOUDLE_POWER_OVERHIGH 0x04

#define  MOUDLE_ON     0x01
#define  MOUDLE_OFF    0x00


#define SPI_BIT_RATE    	1000000     /* ~1Mbps */

#define SIM_SCGC_VALUE      0x00003000L  


void System_Init(void);
void Task_Power_Manage(void);


#endif
