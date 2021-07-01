/******************************************************************************
*
*              Copyright (C) 2011 ROTHWELL, Inc.
*                      All Rights Reserved
*
* File Name   :     output.h
* Author      :     Yihua.Xiang
* Description :	    SW input driver header file
* Version 1.0 :     Yihua.Xiang      2016.08.28
******************************************************************************/

#ifndef __OUTPUT_H
#define __OUTPUT_H


#include "common.h"
#include "tick.h"
#include "i2c.h"
#include "spi.h"
#include "uart.h"

extern volatile uint8_t send_byte;



#define 	IDLE       0

void OutputCtrl_Init(void);
void Task_Dg_Ctrl(void);
void Task_Fan_Ctrl(void);

                  
#endif
