/*
 * MotorCtrl.c
 *
 *  Created on: Apr 15, 2017
 *      Author: Administrator
 */


#include "MotorCtrl.h"





const uint8 con_modemotor_position[5]={0xEB,0xBE,0xA3,0x4A,0x15};      //the standard postion of four win_mode
const uint8 con_mixmotor_position[5]={0xEB,0xBE,0xA3,0x4A,0x15};      //the standard postion of seven mix_level



volatile uint8  HighByte_TleData;
uint16_t MotorCtl_Word=0;                                                           //motor control word control 4 motor to go forward/go back/stop
uint16_t MotorMode_Word=0;



extern MoudleStat_T  Moudle_Stat;
extern uint8 Current_Mode_Position;
extern uint8 Current_Mix_Position;
extern uint8 Mix_level;
extern uint8 Win_Mode;

void SPI0_Task (void)
{
	 /* check Receiver */
	//uint8  data_receive;
	if(SPI_IsSPRF(SPI0)&& (SPI0->C1 & SPI_C1_SPIE_MASK))
	{
		//data_receive = SPI_ReadDataReg(SPI0);
		SPI_IntDisable(SPI0);

	}
	 /* check transmitter */
	else if(SPI_IsSPTEF(SPI0) && (SPI0->C1 & SPI_C1_SPTIE_MASK))
	{
			SPI_WriteDataReg(SPI0,0x88);
			SPI_TxIntDisable(SPI0);
	}
	else
	{
		(void)SPI0->S;
	}

}







void TLE94108_Init(void)
{
	GPIO_PinSet(PORT_OUTPUT_TLEINH);
}




void TLE94108_Send(uint16 *data)
{
    while(!SPI_IsSPTEF(SPI0));
    SPI_WriteDataReg(SPI0,((*data)&0x00FF));
	HighByte_TleData=(*data)>>8;
	SPI_TxIntEnable(SPI0);	
}


/******************************************************************************
* 
* Description:  Motor_Ctrl_Init:Init the motor control port direction and send the all zero control_word making all motor stop
*
* Parameter  :  NONE
*
* Return     :  NONE
*
******************************************************************************/


void Motor_Ctrl_Init(void)
{
	uint16_t motorctl_data;
    TLE94108_Init();
    motorctl_data=MOTOR_CTL_ADDR1;                   
    TLE94108_Send(&motorctl_data);                         //HB1_HS HB1_LS HB2_HS HB2_LS HB3_HS HB3_LS HB4_HS HB4_LS  Set to Low
    motorctl_data=MOTOR_CTL_ADDR2;
    TLE94108_Send(&motorctl_data);                         //HB5_HS HB5_LS HB6_HS HB6_LS HB7_HS HB7_LS HB8_HS HB8_LS  Set to Low
	
    motorctl_data=MOTOR_CTL_ADDR3+(0x00<<8);               //HB1,HB2,HB3,HB4  PWM disable            
    TLE94108_Send(&motorctl_data);
    motorctl_data=MOTOR_CTL_ADDR4+(0x00<<8);               //HB5,HB6 HB7,HB8  PWM disable
    TLE94108_Send(&motorctl_data);
    motorctl_data=MOTOR_CTL_ADDR5+(0xFF<<8);               //PWM1 channel  PWM2 channel  PWM3 channel Open  CLK:62.5Khz 
    TLE94108_Send(&motorctl_data);   
    motorctl_data=MOTOR_CTL_ADDR8+(0x80<<8);               //PWM3 50% On   
}


void ModeMotor_MoveSet(void)
{
	uint8  differ_value;
    static uint8  mode_req_pre=WIN_MODE_UNKNOWN;
    static uint8  motor_stat=MOTOR_STOP;    
    static uint16 stall_time=LATTERMOST_TIME;                                                    //pwm control parament reserved
    if(MOUDLE_ON==Moudle_Stat.onoff)
    {
        ModeMotor_Stop();
        motor_stat=MOTOR_STOP;
        stall_time=LATTERMOST_TIME;
        MotorMode_Word=0;
        return;
    }
    
    if(Get1SecTickInterval(stall_time)<=MODE_STALL_TIME)
    { 
        if(Current_Mode_Position>con_modemotor_position[Win_Mode])
        {
            differ_value=Current_Mode_Position-con_modemotor_position[Win_Mode];
            if(differ_value<=TOLERANCE_MODE_MOTOR) 
            {
                ModeMotor_Stop();
                motor_stat=MOTOR_STOP;        
            } 
            else
            {
                if(LATTERMOST_TIME==stall_time)
                {
                    stall_time=Get1SecTickVal();
                } 
                if((MOTOR_BACKWARD!=motor_stat))
                {
                    ModeMotor_BackWard();                   
                    motor_stat=MOTOR_BACKWARD;           
                }
                if(differ_value<=INERTIA_MODE_MOTOR)
                {
                    ModeMotor_PwmSet();                                        //pwm enable     
                }
                else
                {
                    ModeMotor_PwmClr();                                       //pwm disalbe
                }

            }
        }
        else
        {
            differ_value=con_modemotor_position[Win_Mode]-Current_Mode_Position;
            if(differ_value<=TOLERANCE_MODE_MOTOR) 
            {
                ModeMotor_Stop();
                motor_stat=MOTOR_STOP;    
            }
            else
            {
                if(LATTERMOST_TIME==stall_time)
                {
                    stall_time=Get1SecTickVal();
                }            
                if(MOTOR_FORWARD!=motor_stat)
                {
                    ModeMotor_ForWard();                   
                    motor_stat=MOTOR_FORWARD;          
                }
                if(differ_value<=INERTIA_MODE_MOTOR)
                {
                    ModeMotor_PwmSet();                                        //pwm enable     
                }
                else
                {
                    ModeMotor_PwmClr();                                       //pwm disalbe
                }   
            }   
        }
    }
    else
    {
        ModeMotor_Stop();
        motor_stat=MOTOR_STOP;       
    }
    if(mode_req_pre!=Win_Mode)
    {
        stall_time=LATTERMOST_TIME;    
    }
    mode_req_pre=Win_Mode;
}


void MixMotor_MoveSet(void)
{
	uint8 differ_value;
    static uint8  mix_req_pre=WIN_MODE_UNKNOWN;
    static uint8  motor_stat=MOTOR_STOP;    
    static uint16 stall_time=LATTERMOST_TIME;                                                    //pwm control parament reserved
    if(MOUDLE_ON==Moudle_Stat.onoff)
    {
        MixMotor_Stop();
        motor_stat=MOTOR_STOP;
        stall_time=LATTERMOST_TIME;
        MotorMode_Word=0;
        return;
    }
    
    if(Get1SecTickInterval(stall_time)<=MODE_STALL_TIME)
    { 
        if(Current_Mix_Position>con_mixmotor_position[Mix_level])
        {
            differ_value=Current_Mix_Position-con_mixmotor_position[Mix_level];
            if(differ_value<=TOLERANCE_MODE_MOTOR) 
            {
                MixMotor_Stop();
                motor_stat=MOTOR_STOP;        
            } 
            else
            {
                if(LATTERMOST_TIME==stall_time)
                {
                    stall_time=Get1SecTickVal();
                } 
                if((MOTOR_BACKWARD!=motor_stat))
                {
                    MixMotor_BackWard();                   
                    motor_stat=MOTOR_BACKWARD;           
                }
                if(differ_value<=INERTIA_MODE_MOTOR)
                {
                    MixMotor_PwmSet();                                        //pwm enable     
                }
                else
                {
                    MixMotor_PwmClr();                                       //pwm disalbe
                }

            }
        }
        else
        {
            differ_value=con_mixmotor_position[Mix_level]-Current_Mix_Position;
            if(differ_value<=TOLERANCE_MODE_MOTOR) 
            {
                MixMotor_Stop();
                motor_stat=MOTOR_STOP;    
            }
            else
            {
                if(LATTERMOST_TIME==stall_time)
                {
                    stall_time=Get1SecTickVal();
                }            
                if(MOTOR_FORWARD!=motor_stat)
                {
                    MixMotor_ForWard();                   
                    motor_stat=MOTOR_FORWARD;          
                }
                if(differ_value<=INERTIA_MODE_MOTOR)
                {
                    MixMotor_PwmSet();                                        //pwm enable     
                }
                else
                {
                    MixMotor_PwmClr();                                       //pwm disalbe
                }   
            }   
        }
    }
    else
    {
        MixMotor_Stop();
        motor_stat=MOTOR_STOP;       
    }
    if(mix_req_pre!=Mix_level)
    {
        stall_time=LATTERMOST_TIME;    
    }
    mix_req_pre=Mix_level;	
}

void MotorControl_Send(void)
{
		//static uint8 send_stat=0;
	static uint16 scan_time=FOREMOST_TIME;
	static uint16 delay_time=LATTERMOST_TIME;
	uint16_t send_data;
	if(Get2MsTickInterval(scan_time)>=1000)
	{
		scan_time=Get2MsTickVal();
		TLE94108_Send(&send_data);
		delay_time=Get2MsTickVal();

	}
	if(Get2MsTickInterval(delay_time)>=1)
	{
		TLE94108_Send(&send_data);
		delay_time=LATTERMOST_TIME;
	}
}



void MotorControl_Read(void)
{
}


/******************************************************************************
* 
* Description:  Task_Motor_Ctrl
*
* Parameter  :  NONE
*
* Return     :  NONE
*
******************************************************************************/
void Task_Motor_Ctrl(void)
{
	ModeMotor_MoveSet();
	MixMotor_MoveSet();
	MotorControl_Send();
	MotorControl_Read();
}
