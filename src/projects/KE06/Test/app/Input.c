/******************************************************************************
*
*              Copyright (C) 2011 ROTHWELL, Inc.
*                      All Rights Reserved
*
* File Name :     digio.c
* Author    :     Yihua.Xiang 
* Description:	  Switch Input Module  
* Version 1.0 :   Yihua.Xiang   2016.09.06
******************************************************************************/

#include "Input.h" 



extern MoudleStat_T  Moudle_Stat;

static void ClrSumV(void);


uint8 Current_Mode_Position;
uint8 Current_Mix_Position;

uint8 Win_Mode;
uint8 Mix_level;


static uint8  Ign_Stat=DG_STAT_OFF;
static uint8  AcOc_Stat=DG_STAT_OFF;


#define IDLE            0                       /* IDLE state  */
#define MEASURE         1                       /* MESURE state         */
#define CONTINUOUS      2                       /* CONTINUOS state      */
#define SINGLE          3                       /* SINGLE state         */

static const  uint8 Channels[8] = {ADC_CHANNEL_AD3,CHANNEL_AD_SW1,CHANNEL_AD_SW2,CHANNEL_AD_EVAP,CHANNEL_AD_MOTOR_WARM,CHANNEL_AD_MOTOR_MODE,CHANNEL_AD_BAT};  /* Contents for the device control register */
static uint8 OutFlg=FALSE;                            /* Measurement finish flag */
static uint8 SumChan;                           /* Number of measured channels */
static uint8 SumCnt;                            /* Number of measured channels */
volatile static uint8 ModeFlg=IDLE;                           /* Current state of device */

uint16 ADC_OutV[CHANNEL_NUM];                             /* Sum of measured values */


uint16 ADC_Value[7];


uint8 Key_SW1_Stat;
uint8 Key_SW2_Stat;

uint8 MixMotor_Position;
uint8 ModeMotor_Position;
uint8 Evap_Temp;

volatile uint8   Flag_Ad_Finished=FALSE;

void ADC_OnEnd(void)
{
	if(ERR_OK==ADC_GetValue16(ADC_Value))
	{
		Flag_Ad_Finished=TRUE;
	}

}

void ADC0_Task(void)
{
	uint16_t result;
	result=ADC_ReadResultReg(ADC);
    ADC_OutV[SumChan] += result;     /* Save measured value */
    SumChan++;                           /* Number of measurement */
    if (SumChan == CHANNEL_NUM) 
    {                                   /* Is number of measurement equal to the number of conversions? */
        SumChan = 0;                       /* If yes then set the number of measurement to 0 */
        SumCnt++;
        if ( SumCnt == CONVER_TIMES ) 
        {
            OutFlg = TRUE;                   /* Measured values are available */
            ADC_OnEnd();                     /* Invoke user event */
            ModeFlg = IDLE;                  /* Set the device to the stop mode */
            return;                          /* Return from interrupt */
        }
    }
    ADC_SetChannel(ADC,Channels[SumChan]); 
}



static void ClrSumV(void)
{
    uint8 i;
    for(i=0;i<CHANNEL_NUM;i++)
    {
        ADC_OutV[i] = 0;                     /* Set variable for storing measured values to 0 */
    }
}



void ADC_Start_Conver(void)
{
    if (IDLE==ModeFlg)
    {
        SumCnt = 0;                        				/* Set the number of conversions */
        OutFlg = FALSE;                    				/* Output values aren't available */
        SumChan = 0;                       				/* Set the number of measured channels to 0 */
        ClrSumV();                         				/* Clear measured values */
        ADC_SetChannel(ADC,Channels[SumChan]);       	/* If yes then start the conversion */

    }
    
}



uint8 ADC_Measure(uint8 WaitForResult)
{
    if (ModeFlg != IDLE) 
    {                                       /* Is the device in different mode than "stop"? */
        return ERR_BUSY;                   /* If yes then error */
    }
    ADC_Start_Conver();                        /* Enable the device */
	ModeFlg = MEASURE;                   /* Set state of device to the measure mode */
    if (WaitForResult) 
    {                                   /* Is WaitForResult TRUE? */
        while(MEASURE==ModeFlg)     	/* If yes then wait for end of measurement */
		{
		}      
    }
    return ERR_OK;                       /* OK */
}




uint8 ADC_GetValue16(uint16 *Values)
{
    uint8 i;
    if (!OutFlg) 
    {                                                   /* Is output flag set? */
        return ERR_NOTAVAIL;                            /* If no then error */
    }
    for(i=0;i<CHANNEL_NUM;i++)
    {
        Values[i] = (uint16)((ADC_OutV[i]) >> 2);      /* Save measured values to the output buffer */   
    }
    return ERR_OK;                                    
}











uint8 GPIO_ReadPin(uint8 port)
{
	if(port<=GPIO_PTD7)
	{
		if(GPIO_Read(GPIOA) & (1<<port))
		{
			return 1;
		}
		else
		{
			return 0;
		}
			
	}
	else if(port<=GPIO_PTH7)
	{
		port=port-32;
		if(GPIO_Read(GPIOB) & (1<<port))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}		
}

uint8 Read_IgnPinStat()
{
	if(IGN_ON_LEVEL==GPIO_ReadPin(PORT_INPUT_IGN))
	{
		return DG_STAT_ON;
	}
	else
	{
		return DG_STAT_OFF;
	}
}

uint8 Read_AcOcPinStat()
{
	if(ACOC_ON_LEVEL==GPIO_ReadPin(PORT_INPUT_ACOC))
	{
		return DG_STAT_ON;
	}
	else
	{
		return DG_STAT_OFF;
	}	
}





uint8_t Ign_StatInit(void)
{
	return Read_IgnPinStat();	
}

uint8 AcOc_StatInit(void)
{
	return Read_AcOcPinStat();
}





      
 /******************************************************************************
* 
* Description:  InputSW_Init
*
* Parameter  :  NONE
*
* Return     :  NONE
******************************************************************************/
void Input_Init(void) 
{
	uint16 ad_sample;
	ModeFlg = IDLE;                     //Init to IDLE  Adc Moudle can start at this state
	Ign_Stat=Ign_StatInit();            //Init Ign_Stat 
	AcOc_Stat=AcOc_StatInit();                    //Init Acoc_Stat
	
	if(ERR_OK==ADC_Measure(TRUE))
	{
		ad_sample=ADC_Value[CHANNEL_NUM_BAT];
		if(ad_sample<BATTERY_6V_ADVALUE)
		{
			Moudle_Stat.power=MOUDLE_POWER_OVERLOW;
		}
		else if(ad_sample<BATTERY_9V_ADVALUE)
		{
			Moudle_Stat.power=MOUDLE_POWER_LOW;
		}
		else if(ad_sample<BATTERY_16V_ADVALUE)
		{
			Moudle_Stat.power=MOUDLE_POWER_NORMAL;
		}
		else if(ad_sample<BATTERY_18V_ADVALUE)
		{
			Moudle_Stat.power=MOUDLE_POWER_HIGH;
		}
		else
		{
			Moudle_Stat.power=MOUDLE_POWER_OVERHIGH;
		}
	}
	
}



void Ign_StatScan()
{
	uint8 i=0;
	static uint8  stat_buff[IGN_SCAN_TIMES];
	static uint16 scan_time=FOREMOST_TIME;
	static uint8  scan_num=SCAN_NUM_INIT;
	if(SCAN_NUM_INIT==scan_num)
	{
		for(i=0;i<IGN_SCAN_TIMES;i++)
		{
			stat_buff[i]=Ign_Stat;
		}
		scan_num=0;
		scan_time=Get2MsTickVal();
		return;
	}
	else
	{
		if(Get2MsTickInterval(scan_time)>=IGN_FITER_TIME)
		{
			if(scan_num>=IGN_SCAN_TIMES)
			{
				scan_num=0;
			}
			stat_buff[scan_num]=Read_IgnPinStat();
			for(i=0;i<IGN_SCAN_TIMES;i++)
			{
				if(Ign_Stat==stat_buff[scan_num])
				{
					scan_time=Get2MsTickVal();
					scan_num++;
					return;
				}
			}
			Ign_Stat=stat_buff[0]; 
			scan_time=Get2MsTickVal();
			scan_num++;
			return;
		}
		else
		{
			 return;
		}	
	}
}

void AcOc_StatScan(void)
{
	uint8 i=0;
	static uint8  stat_buff[ACOC_SCAN_TIMES];
	static uint16 scan_time=FOREMOST_TIME;
	static uint8  scan_num=SCAN_NUM_INIT;
	if(SCAN_NUM_INIT==scan_num)
	{
		for(i=0;i<ACOC_SCAN_TIMES;i++)
		{
			stat_buff[i]=AcOc_Stat;
		}
		scan_num=0;
		scan_time=Get2MsTickVal();
		return;
	}
	else
	{
		if(Get2MsTickInterval(scan_time)>=ACOC_FITER_TIME)
		{
			if(scan_num>=ACOC_SCAN_TIMES)
			{
				scan_num=0;
			}
			stat_buff[scan_num]=Read_AcOcPinStat();
			for(i=0;i<ACOC_SCAN_TIMES;i++)
			{
				if(AcOc_Stat==stat_buff[scan_num])
				{
					scan_time=Get2MsTickVal();
					scan_num++;
					return;
				}
			}
			scan_time=Get2MsTickVal();
			AcOc_Stat=stat_buff[0]; 
			scan_num++;
			return;
		}
		else
		{
			return;
		}	
	}
}




/******************************************************************************
* 
* Description:  Task_Dg_Scan
*
* Parameter  :  NONE
*
* Return     :  NONE
******************************************************************************/
void Task_Dg_Scan()
{
	Ign_StatScan();
	AcOc_StatScan();
	

	if(DG_STAT_ON==Ign_Stat)
	{
		//LED1_On();	
	}
	else
	{
		//LED1_Off();	
	}
		/*static uint16 scan_time=FOREMOST_TIME;
		if(Get2MsTickInterval(scan_time)>=1000)
    {
        scan_time=Get2MsTickVal();
        //ADC_Start_Conver();         
    }*/
}


void Batter_ADValue_Conver(void)
{
	uint16 ad_sample;
	static uint16 sustain_time_D6V=LATTERMOST_TIME;
	static uint16 sustain_time_D9V=LATTERMOST_TIME;
	static uint16 sustain_time_D15HV=LATTERMOST_TIME;
	static uint16 sustain_time_D17HV=LATTERMOST_TIME;
	static uint16 sustain_time_U18V=LATTERMOST_TIME;
	ad_sample=ADC_Value[CHANNEL_NUM_BAT];
	
	if(ad_sample<BATTERY_6V_ADVALUE)                                          //lower than 14V for 50ms  then shut down the system
    {
        if(LATTERMOST_TIME==sustain_time_D6V)
        {
            sustain_time_D6V=Get2MsTickVal();    
        }
        if((Get2MsTickInterval(sustain_time_D6V)>=250))
        {
            Moudle_Stat.power=MOUDLE_POWER_OVERLOW;
        }
		sustain_time_D9V=LATTERMOST_TIME;
		sustain_time_D15HV=LATTERMOST_TIME;
		sustain_time_D17HV=LATTERMOST_TIME;
		sustain_time_U18V=LATTERMOST_TIME;
    }
	else if(ad_sample<BATTERY_6HV_ADVALUE)
	{
		sustain_time_D9V=LATTERMOST_TIME;
		sustain_time_D15HV=LATTERMOST_TIME;
		sustain_time_D17HV=LATTERMOST_TIME;
		sustain_time_U18V=LATTERMOST_TIME;		
	}
	else if(ad_sample<BATTERY_9V_ADVALUE)
	{
        if(LATTERMOST_TIME==sustain_time_D9V)
        {
            sustain_time_D9V=Get2MsTickVal();    
        }
		if(MOUDLE_POWER_OVERLOW==Moudle_Stat.power)
		{
			if((Get2MsTickInterval(sustain_time_D9V)>=250))
			{
				Moudle_Stat.power=MOUDLE_POWER_LOW;
			}	
		}
		else
		{
			if((Get2MsTickInterval(sustain_time_D9V)>=250))
			{
				Moudle_Stat.power=MOUDLE_POWER_LOW;
			}			
		}
		sustain_time_D6V=LATTERMOST_TIME;
		sustain_time_D15HV=LATTERMOST_TIME;
		sustain_time_D17HV=LATTERMOST_TIME;
		sustain_time_U18V=LATTERMOST_TIME;
	}
	else if(ad_sample<BATTERY_9HV_ADVALUE)
	{
		sustain_time_D6V=LATTERMOST_TIME;
		sustain_time_D15HV=LATTERMOST_TIME;
		sustain_time_D17HV=LATTERMOST_TIME;
		sustain_time_U18V=LATTERMOST_TIME;		
	}

	else if(ad_sample<BATTERY_15HV_ADVALUE)
	{
        if(LATTERMOST_TIME==sustain_time_D15HV)
        {
            sustain_time_D15HV=Get2MsTickVal();    
        }
		if(MOUDLE_POWER_HIGH==Moudle_Stat.power)
		{
			if((Get2MsTickInterval(sustain_time_D15HV)>=2000))
			{
				Moudle_Stat.power=MOUDLE_POWER_NORMAL;
			}	
		}
		else
		{
			if((Get2MsTickInterval(sustain_time_D15HV)>=250))
			{
				Moudle_Stat.power=MOUDLE_POWER_NORMAL;
			}			
		}
		sustain_time_D6V=LATTERMOST_TIME;
		sustain_time_D9V=LATTERMOST_TIME;
		sustain_time_D17HV=LATTERMOST_TIME;
		sustain_time_U18V=LATTERMOST_TIME;	
	}
	
	else if(ad_sample<BATTERY_16V_ADVALUE)
	{
		
	}
	else if(ad_sample<BATTERY_17HV_ADVALUE)
	{
        if(LATTERMOST_TIME==sustain_time_D17HV)
        {
            sustain_time_D17HV=Get2MsTickVal();    
        }
		if(MOUDLE_POWER_OVERHIGH==Moudle_Stat.power)
		{
			if((Get2MsTickInterval(sustain_time_D17HV)>=2000))
			{
				Moudle_Stat.power=MOUDLE_POWER_HIGH;
			}	
		}
		else
		{
			if((Get2MsTickInterval(sustain_time_D15HV)>=250))
			{
				Moudle_Stat.power=MOUDLE_POWER_HIGH;
			}			
		}
		sustain_time_D6V=LATTERMOST_TIME;
		sustain_time_D9V=LATTERMOST_TIME;
		sustain_time_D15HV=LATTERMOST_TIME;	
		sustain_time_U18V=LATTERMOST_TIME;		
	}
	
	else if(ad_sample<BATTERY_18V_ADVALUE)
	{
        	
	}	
	else
	{
        if(LATTERMOST_TIME==sustain_time_U18V)
        {
            sustain_time_U18V=Get2MsTickVal();    
        }
        if((Get2MsTickInterval(sustain_time_U18V)>=250))
        {
            Moudle_Stat.power=MOUDLE_POWER_OVERHIGH;
        }
		sustain_time_D6V=LATTERMOST_TIME;
		sustain_time_D9V=LATTERMOST_TIME;
		sustain_time_D15HV=LATTERMOST_TIME;
		sustain_time_D17HV=LATTERMOST_TIME;
	}	
}



uint16_t FiterFor_TempSensor(uint16 * buffer)
{
    uint8 i;
    uint16_t max_value=buffer[0];
    uint16_t min_value=buffer[0];
    uint32_t total_value=0;
    for(i=0;i<5;i++)
    {
        total_value=total_value+buffer[i];
        if(buffer[i]>max_value)
        {
           max_value=buffer[i]; 
        }
        if(buffer[i]<min_value)
        {
           min_value=buffer[i]; 
        }
    }
    total_value=total_value-max_value-min_value;
    return total_value>>2;
}




/*void SW1_ADValue_Conver(void)
{
	uint8 i;
	uint16 ad_sample;
	static uint8  scan_time=AD_SW_SCAN_TIMES+1;
	static uint16 temp_buffer[AD_SW_SCAN_TIMES];
    static uint16 cycle_time=FOREMOST_TIME; 
	ad_sample=ADC_Value[CHANNEL_NUM_SW1];
	if(Get2MsTickInterval(cycle_time)>10)
	{
		ad_sample=ADC_Value[CHANNEL_NUM_SW1];
		if(scan_time<AD_SW_SCAN_TIMES)
		{
			temp_buffer[scan_time]=ad_sample;
			scan_time++;    
		}
		else if(AD_SW_SCAN_TIMES==scan_time)
		{
			temp_buffer[0]=ad_sample;
			scan_time=1;     
		}
		else if(AD_SW_SCAN_TIMES+1==scan_time) 
		{
			for(i=0;i<AD_SW_SCAN_TIMES;i++)
			{
				temp_buffer[i]=ad_sample;    
			}
			scan_time=0;
		}
		else
		{
		}
		ad_sample=FiterFor_TempSensor(temp_buffer);		
	}
}*/



void SW1_ADValue_Conver(void)
{
	uint16 ad_sample;
	static uint16 sus_time_nokey=LATTERMOST_TIME;
	static uint16 sus_time_modekey=LATTERMOST_TIME;
	static uint16 sus_time_tempaddkey=LATTERMOST_TIME;
	static uint16 sus_time_tempsubkey=LATTERMOST_TIME;
	ad_sample=ADC_Value[CHANNEL_NUM_SW1];
	if(ad_sample<AD_VALUE_NOKEY_UPLIMIT)
	{
		if(LATTERMOST_TIME==sus_time_nokey)
		{
			sus_time_nokey=Get2MsTickVal();
		}
		else
		{
			if((Get2MsTickInterval(sus_time_nokey)>=AD_KEY_CONFIRM_TIME))
			{
				Key_SW1_Stat=KEY_SW1_STAT_NO_KEY_PRESSED;
			}
		}
		sus_time_modekey=LATTERMOST_TIME;
		sus_time_tempaddkey=LATTERMOST_TIME;
		sus_time_tempsubkey=LATTERMOST_TIME;
	}
	else if(ad_sample>AD_VALUE_12K_DOWNLIMIT && ad_sample<AD_VALUE_12K_UPLIMIT)
	{
		if(LATTERMOST_TIME==sus_time_modekey)
		{
			sus_time_modekey=Get2MsTickVal();
		}
		else
		{
			if((Get2MsTickInterval(sus_time_modekey)>=AD_KEY_CONFIRM_TIME))
			{
				Key_SW1_Stat=KEY_SW1_STAT_MODE_PRESSED;
			}
		}
		sus_time_nokey=LATTERMOST_TIME;
		sus_time_tempaddkey=LATTERMOST_TIME;
		sus_time_tempsubkey=LATTERMOST_TIME;
	}
	else if(ad_sample>AD_VALUE_3P3K_DOWNLIMIT && ad_sample<AD_VALUE_3P3K_UPLIMIT)
	{
		if(LATTERMOST_TIME==sus_time_tempaddkey)
		{
			sus_time_tempaddkey=Get2MsTickVal();
		}
		else
		{
			if((Get2MsTickInterval(sus_time_tempaddkey)>=AD_KEY_CONFIRM_TIME))
			{
				Key_SW1_Stat=KEY_SW2_STAT_FANADD_PRESSED;
			}
		}
		sus_time_modekey=LATTERMOST_TIME;
		sus_time_nokey=LATTERMOST_TIME;
		sus_time_tempsubkey=LATTERMOST_TIME;
	}
	else if(ad_sample>AD_VALUE_P56K_DOWNLIMIT && ad_sample>AD_VALUE_P56K_UPLIMIT)
	{
		if(LATTERMOST_TIME==sus_time_tempsubkey)
		{
			sus_time_tempsubkey=Get2MsTickVal();
		}
		else
		{
			if((Get2MsTickInterval(sus_time_tempsubkey)>=AD_KEY_CONFIRM_TIME))
			{
				Key_SW1_Stat=KEY_SW2_STAT_FANSUB_PRESSED;
			}
		}
		sus_time_tempaddkey=LATTERMOST_TIME;
		sus_time_modekey=LATTERMOST_TIME;
		sus_time_nokey=LATTERMOST_TIME;	
	}
	else
	{
		sus_time_nokey=LATTERMOST_TIME;
		sus_time_tempaddkey=LATTERMOST_TIME;
		sus_time_modekey=LATTERMOST_TIME;
		sus_time_tempsubkey=LATTERMOST_TIME;
	}	
}



void SW2_ADValue_Conver(void)
{
	uint16 ad_sample;
	static uint16 sus_time_nokey=LATTERMOST_TIME;
	static uint16 sus_time_ackey=LATTERMOST_TIME;
	static uint16 sus_time_fanaddkey=LATTERMOST_TIME;
	static uint16 sus_time_fansubkey=LATTERMOST_TIME;
	ad_sample=ADC_Value[CHANNEL_NUM_SW2];
	if(ad_sample<AD_VALUE_NOKEY_UPLIMIT)
	{
		if(LATTERMOST_TIME==sus_time_nokey)
		{
			sus_time_nokey=Get2MsTickVal();
		}
		else
		{
			if((Get2MsTickInterval(sus_time_nokey)>=AD_KEY_CONFIRM_TIME))
			{
				Key_SW1_Stat=KEY_SW1_STAT_NO_KEY_PRESSED;
			}
		}
		sus_time_fanaddkey=LATTERMOST_TIME;
		sus_time_ackey=LATTERMOST_TIME;
		sus_time_fansubkey=LATTERMOST_TIME;
	}
	else if(ad_sample>AD_VALUE_12K_DOWNLIMIT && ad_sample<AD_VALUE_12K_UPLIMIT)
	{
		if(LATTERMOST_TIME==sus_time_ackey)
		{
			sus_time_ackey=Get2MsTickVal();
		}
		else
		{
			if((Get2MsTickInterval(sus_time_ackey)>=AD_KEY_CONFIRM_TIME))
			{
				Key_SW2_Stat=KEY_SW2_STAT_AC_PRESSED;
			}
		}
		sus_time_fanaddkey=LATTERMOST_TIME;
		sus_time_nokey=LATTERMOST_TIME;
		sus_time_fansubkey=LATTERMOST_TIME;
	}
	else if(ad_sample>AD_VALUE_3P3K_DOWNLIMIT && ad_sample<AD_VALUE_3P3K_UPLIMIT)
	{
		if(LATTERMOST_TIME==sus_time_fanaddkey)
		{
			sus_time_fanaddkey=Get2MsTickVal();
		}
		else
		{
			if((Get2MsTickInterval(sus_time_fanaddkey)>=AD_KEY_CONFIRM_TIME))
			{
				Key_SW2_Stat=KEY_SW2_STAT_FANADD_PRESSED;
			}
		}
		sus_time_ackey=LATTERMOST_TIME;
		sus_time_nokey=LATTERMOST_TIME;
		sus_time_fansubkey=LATTERMOST_TIME;
	}
	else if(ad_sample>AD_VALUE_P56K_DOWNLIMIT && ad_sample>AD_VALUE_P56K_UPLIMIT)
	{
		if(LATTERMOST_TIME==sus_time_fansubkey)
		{
			sus_time_fansubkey=Get2MsTickVal();
		}
		else
		{
			if((Get2MsTickInterval(sus_time_fansubkey)>=AD_KEY_CONFIRM_TIME))
			{
				Key_SW2_Stat=KEY_SW2_STAT_FANSUB_PRESSED;
			}
		}
		sus_time_fanaddkey=LATTERMOST_TIME;
		sus_time_ackey=LATTERMOST_TIME;
		sus_time_nokey=LATTERMOST_TIME;	
	}
	else
	{
		sus_time_nokey=LATTERMOST_TIME;
		sus_time_fanaddkey=LATTERMOST_TIME;
		sus_time_ackey=LATTERMOST_TIME;
		sus_time_fansubkey=LATTERMOST_TIME;
	}	
}




void MixMotorFB_ADValue_Conver(void)
{
	uint16 ad_sample;
	ad_sample=ADC_Value[CHANNEL_NUM_MOTOR_MIX];
	MixMotor_Position=ad_sample;
}

void ModeMotorFB_ADValue_Conver(void)
{
	uint16 ad_sample;
	ad_sample=ADC_Value[CHANNEL_NUM_MOTOR_MODE];
	ModeMotor_Position=ad_sample;	
}

void EvapTemp_ADValue_Conver(void)
{
	uint16 ad_sample;
	ad_sample=ADC_Value[CHANNEL_NUM_EVAP];	
	Evap_Temp=ad_sample;
}

void AdValue_Conver(void)
{
	Batter_ADValue_Conver();
	SW1_ADValue_Conver();
	SW2_ADValue_Conver();
	MixMotorFB_ADValue_Conver();
	ModeMotorFB_ADValue_Conver();
	EvapTemp_ADValue_Conver();
}




/******************************************************************************
* 
* Description:  Task_Ad_Scan
*
* Parameter  :  NONE
*
* Return     :  NONE
******************************************************************************/
void Task_Ad_Scan(void)
{
	static uint16 scan_time=FOREMOST_TIME;
	if(Get2MsTickInterval(scan_time)>=1000)
	{
		scan_time=Get2MsTickVal();
		ADC_Start_Conver();         
	}
	if(Flag_Ad_Finished)
	{
		//LED1_Toggle();
		AdValue_Conver();
		Flag_Ad_Finished=FALSE;	 
	}
}



