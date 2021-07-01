/*
 * MotorCtrl.h
 *
 *  Created on: Apr 15, 2017
 *      Author: Administrator
 */

#ifndef MOTORCTRL_H_
#define MOTORCTRL_H_

#include "common.h"
#include "Input.h"




#define  PWM_ENABLE              3
#define  PWM_DISABLE             0

#define  HIGH                    1
#define  LOW                     0


typedef struct 
{
        uint8 hb5_ls            :1;                          //                         
        uint8 hb5_hs            :1;                          //                                 
        uint8 hb6_ls            :1;                          //         
        uint8 hb6_hs            :1;                          //            
        uint8 hb7_ls            :1;                          //        
        uint8 hb7_hs            :1;                          //         
        uint8 hb8_ls            :1;                          //          
        uint8 hb8_hs            :1;                          //                                   
        uint8 hb1_ls            :1;                          //           
        uint8 hb1_hs            :1;                          //            
        uint8 hb2_ls            :1;                          //            
        uint8 hb2_hs            :1;                          //          
        uint8 hb3_ls            :1;                          //          
        uint8 hb3_hs            :1;                          //          
        uint8 hb4_ls            :1;                          //         
        uint8 hb4_hs            :1;                          //                                      
}MotorCtl_T;


typedef struct
{
        uint8 hb5_mode          :2;
        uint8 hb6_mode          :2;
        uint8 hb7_mode          :2;
        uint8 hb8_mode          :2;
        uint8 hb1_mode          :2;
        uint8 hb2_mode          :2;
        uint8 hb3_mode          :2;
        uint8 hb4_mode          :2;
}MotorMode_T;



#define  MixMotor_Stop()        ((MotorCtl_T*)(&MotorCtl_Word))->hb1_ls=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb1_hs=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb2_ls=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb2_hs=DISABLE
                                      
                                 
#define  MixMotor_ForWard()     ((MotorCtl_T*)(&MotorCtl_Word))->hb1_ls=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb1_hs=ENABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb2_ls=ENABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb2_hs=DISABLE
                                 
#define  MixMotor_BackWard()    ((MotorCtl_T*)(&MotorCtl_Word))->hb1_ls=ENABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb1_hs=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb2_ls=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb2_hs=ENABLE
                                
#define  MixMotor_PwmSet()      ((MotorMode_T*)(&MotorMode_Word))->hb1_mode=PWM_ENABLE
#define  MixMotor_PwmClr()      ((MotorMode_T*)(&MotorMode_Word))->hb1_mode=PWM_DISABLE




#define  ModeMotor_Stop()       ((MotorCtl_T*)(&MotorCtl_Word))->hb5_ls=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb5_hs=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb6_ls=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb6_hs=DISABLE
                                                                      
#define  ModeMotor_ForWard()    ((MotorCtl_T*)(&MotorCtl_Word))->hb5_ls=ENABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb5_hs=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb6_ls=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb6_hs=ENABLE
              
#define  ModeMotor_BackWard()   ((MotorCtl_T*)(&MotorCtl_Word))->hb5_ls=DISABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb5_hs=ENABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb6_ls=ENABLE;\
                                ((MotorCtl_T*)(&MotorCtl_Word))->hb6_hs=DISABLE

#define  ModeMotor_PwmSet()      ((MotorMode_T*)(&MotorMode_Word))->hb6_mode=PWM_ENABLE
#define  ModeMotor_PwmClr()      ((MotorMode_T*)(&MotorMode_Word))->hb6_mode=PWM_DISABLE




#define  MODE_STALL_TIME     0x0A               //the full route time  8S
#define  MIX_STALL_TIME      0x05               //the full route time  3S


#define TOLERANCE_MODE_MOTOR     0x44             //equal 2% of full route //ad value precision need test practial according the AD sample bits and AD precision                                      
#define INERTIA_MODE_MOTOR       0xAB             //equal 5% of full route 

#define  MOTOR_STOP         	0xFF
#define  MOTOR_FORWARD      	0x00
#define  MOTOR_BACKWARD     	0x01



/******************************************************************************
Win_Mode
******************************************************************************/
#define WIN_MODE_FACE                 0
#define WIN_MODE_FACEFOOT             1
#define WIN_MODE_FOOT                 2
#define WIN_MODE_FOOTDEF              3
#define WIN_MODE_DEF                  4
#define WIN_MODE_UNKNOWN              5






#define  MOTOR_CTL_ADDR1  0x83
#define  MOTOR_CTL_ADDR2  0xC3
#define  MOTOR_CTL_ADDR3  0xE3
#define  MOTOR_CTL_ADDR4  0x93
#define  MOTOR_CTL_ADDR5  0xB3

#define  MOTOR_CTL_ADDR8  0xCB                   //pwm channale 3  duty set register





void SPI0_Task (void);
void Motor_Ctrl_Init(void);
void Task_Motor_Ctrl(void);




#endif /* MOTORCTRL_H_ */
