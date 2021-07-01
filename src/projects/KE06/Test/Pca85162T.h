/******************************************************************************
*
*              Copyright (C) 2011 ROTHWELL, Inc.
*                      All Rights Reserved
*
* File Name :     ht1621.h
* Author    :     DongFang.Zhang 
* Description:	  HT1621 driver header file
*   
* Version 1.0 : DongFang.Zhang   2013.06.25
* Version 1.1 : Yihua.Xiang      2016.08.28
******************************************************************************/

#ifndef __PCA85162T_H
#define __PCA85162T_H

#include "common.h"
#include "tick.h"

   
/*
*******************************************************************************
*                          FUNCTION DECLARATION
*******************************************************************************
*/                                          
    
extern void HT1621_Init(void);         
extern void HT1621_LcdOn(void);
extern void HT1621_LcdOff(void);
extern void HT1621_SendData(uint8 addrr,uint8 len,uint8 *data);
extern void HT1621_SendData_4bit(uint8 addrr,uint8 len,uint8 data);
extern void HT1621_DisplayTest(void);
extern void HT1621_DisplayNum(uint8 num);   



#endif
