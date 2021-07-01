/******************************************************************************
*
*              Copyright (C) 2011 ROTHWELL, Inc.
*                      All Rights Reserved
*
* File Name :     Tle94108.c
* Author    :     DongFang.Zhang 
* Description:	  HT1621 driver
*   
* Version 1.0 : DongFang.Zhang   2013.06.25
* Version 1.1 : Yihua.Xiang      2016.08.28
******************************************************************************/

#include "Tle94108.h" 





void TLE94108_Init(void)
{
}



/******************************************************************************
* 
* Description:  Delay 2us using asm code  one nop for 1cyc(1/8us when the bus clock is 8M) 16cyc equal 2us
*
* Parameter  :  NONE
*
* Return     :  NONE
*
******************************************************************************/
void Cpu_Delay2us(void)
{ 
   
}
