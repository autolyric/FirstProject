/******************************************************************************
*
*              Copyright (C) 2011 ROTHWELL, Inc.
*                      All Rights Reserved
*
* File Name   :     input.h
* Author      :     Yihua.Xiang
* Description :	    SW input driver header file
* Version 1.0 :     Yihua.Xiang      2016.08.28
******************************************************************************/

#ifndef __INPUT_H
#define __INPUT_H


#include "common.h"
#include "adc.h"
#include "tick.h"
#include "uart.h"
#include "gpio.h"
#include "system.h"




#define    DG_STAT_ON       0x01
#define    DG_STAT_OFF      0x00

#define   IGN_ON_LEVEL      0x01               //ign stat on enable
#define   ACOC_ON_LEVEL     0x01               //Ac oc on enable

#define  SCAN_NUM_INIT       0xFF

#define  IGN_SCAN_TIMES          5               
#define  IGN_FITER_TIME          8
#define  ACOC_SCAN_TIMES         5 
#define  ACOC_FITER_TIME         8


#define PORT_INPUT_IGN    	GPIO_PTD0
#define PORT_INPUT_ACOC		  GPIO_PTB6


#define PORT_OUTPUT_VPP     GPIO_PTB6
#define PORT_OUTPUT_AC      GPIO_PTB6
#define PORT_OUTPUT_LINEN   GPIO_PTB6
#define PORT_OUTPUT_LCDBG   GPIO_PTB6
#define PORT_OUTPUT_TLEEN   GPIO_PTB6 



#define  PORT_OUTPUT_TLEINH   GPIO_PTA6








#define CHANNEL_NUM      1                     //convert  channle num
#define CONVER_TIMES     4                     //convert  times for each channle

#define  CHANNEL_AD_SW1              ADC_CHANNEL_AD1             //  PTA1-AD0SE1
#define  CHANNEL_AD_SW2				 ADC_CHANNEL_AD0			//  PTA0-AD0SE0	
#define  CHANNEL_AD_EVAP             ADC_CHANNEL_AD10            //  PTC2-AD0SE10	
#define  CHANNEL_AD_MOTOR_WARM       ADC_CHANNEL_AD8             //  PTC0-AD0SE8	
#define  CHANNEL_AD_MOTOR_MODE       ADC_CHANNEL_AD9             //  PTC1-AD0SE9	
#define  CHANNEL_AD_BAT       	     ADC_CHANNEL_AD11            //  PTC3-AD0SE11

#define  CHANNEL_NUM_SW1              0             //  PTA1-AD0SE1
#define  CHANNEL_NUM_SW2			  1				//  PTA0-AD0SE0	
#define  CHANNEL_NUM_EVAP             2            	//  PTC2-AD0SE10	
#define  CHANNEL_NUM_MOTOR_MIX       3             //  PTC0-AD0SE8	
#define  CHANNEL_NUM_MOTOR_MODE       4             //  PTC1-AD0SE9	
#define  CHANNEL_NUM_BAT       	      5            	//  PTC3-AD0SE11


#define   AD_VALUE_NOKEY_UPLIMIT         0x02
#define   AD_VALUE_12K_DOWNLIMIT         0x05
#define   AD_VALUE_12K_UPLIMIT           0x10
#define   AD_VALUE_3P3K_DOWNLIMIT        0x15
#define   AD_VALUE_3P3K_UPLIMIT          0x20
#define   AD_VALUE_P56K_DOWNLIMIT        0x25
#define   AD_VALUE_P56K_UPLIMIT          0x30


#define   AD_KEY_CONFIRM_TIME             35


#define   KEY_SW1_STAT_NO_KEY_PRESSED      0x00
#define   KEY_SW1_STAT_MODE_PRESSED        0x01
#define   KEY_SW1_STAT_TEMPADD_PRESSED     0x02
#define   KEY_SW1_STAT_TEMPSUB_PRESSED     0x03


#define   KEY_SW2_STAT_NO_KEY_PRESSED      0x00
#define   KEY_SW2_STAT_AC_PRESSED        0x01
#define   KEY_SW2_STAT_FANADD_PRESSED     0x02
#define   KEY_SW2_STAT_FANSUB_PRESSED     0x03




#define   BATTERY_6V_ADVALUE            0x33
#define   BATTERY_6HV_ADVALUE           0x33
#define   BATTERY_9V_ADVALUE            0x33
#define   BATTERY_9HV_ADVALUE           0x33
#define   BATTERY_15HV_ADVALUE          0x33
#define   BATTERY_16V_ADVALUE           0x34
#define   BATTERY_17HV_ADVALUE          0x34
#define   BATTERY_18V_ADVALUE           0x34



#define   CHANNEL_AD_CHANNEL_MOBINE     0x08 //(1<<CHANNEL_AD_SW1)|(1<<CHANNEL_AD_SW2)|(1<<CHANNEL_AD_EVAP)|(1<<CHANNEL_AD_MOTOR_WARM)|(1<<CHANNEL_AD_MOTOR_MODE)|(1<<CHANNEL_AD_BAT)

#define ERR_BUSY  0

#define ERR_OK    1
#define ERR_NOTAVAIL 2



void ADC0_Task(void);
uint8 ADC_GetValue16(uint16 *Values);
void ADC_Start_Conver(void);


  
      
void Input_Init(void);
void Task_Dg_Scan(void);
void Task_Ad_Scan(void);

#endif
