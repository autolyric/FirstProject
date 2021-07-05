/******************************************************************************
*
*              Copyright (C) 2011 ROTHWELL, Inc.
*                      All Rights Reserved
*
* File Name :     output.c
* Author    :     Yihua.Xiang 
* Description:	  OutPut Module  
* Version 1.0 :   Yihua.Xiang   2016.09.06
******************************************************************************/

#include "output.h"




/******************************************************************************
* 
* Description:  OutputCtrl_Init
*
* Parameter  :  NONE
*
* Return     :  NONE
******************************************************************************/
void OutputCtrl_Init(void) 
{
	
}




/******************************************************************************
* 
* Description:  Task_Dg_Ctrl
*
* Parameter  :  NONE
*
* Return     :  NONE
******************************************************************************/
void Task_Dg_Ctrl(void)
{
	static uint16 scan_time=FOREMOST_TIME;
	if(Get2MsTickInterval(scan_time)>=1000)
    {
        scan_time=Get2MsTickVal();
		//I2C_Start(I2C0);
		//I2C_WriteOneByte(I2C0,0x88);
		//send_byte=1;
		//I2C_IntEnable(I2C0);
			
			
        /*while(!SPI_IsSPTEF(SPI0));
        SPI_WriteDataReg(SPI0,0x55);
        SPI_TxIntEnable(SPI0);  */
        //SPI_TransferWait(SPI0,data_receive,data_send,1);
		//SPI_WriteDataReg(SPI0,0x55);
		//I2C_WriteOneByte(I2C0,0x55);
		//if(I2C_ERROR_START_NO_BUSY_FLAG==I2C_Stop(I2C0))
		{
			//LED1_Toggle(); 
		}
		//LED0_Toggle(); 
    }   
           
}





/******************************************************************************
* 
* Description:  Task_Dg_Ctrl
*
* Parameter  :  NONE
*
* Return     :  NONE
******************************************************************************/
void Task_Fan_Ctrl(void)
{
           
}

/******************************************************************************
* 
* Description:  OutPut_Test
*
* Parameter  :  NONE
*
* Return     :  NONE
******************************************************************************/



void OutPut_Test(void)
{
    
}



