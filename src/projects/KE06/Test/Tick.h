//==============================================================================
// Copyright (c) 2012,������˼Τ���������޹�˾. All rights reserved.
// 
// 
// �ļ�����: tick.h
// ժ    Ҫ: 
// ��    ��: 1.0
//
// �޸ļ�¼: 
//      1.0.0 : ��ʼ��д����(2012��5��24��) -> �
//==============================================================================
#ifndef _TICK_H
#define _TICK_H

#include "common.h"



/* ��ʱ��ֵ��TICKֵ����ת����2msTickһ�Ρ�*/
#define MS_2MS_TICK             (uint16)(1/2)
#define MS_TO_TICK_2MS(val)     (uint16)((val)/2)
#define TICK_2MS_TO_MS(val)     (uint16)((val)*2)
#define SEC_2MS_TICK            (uint16)500
#define SEC_TO_TICK_2MS(val)    (uint16)((val)*SEC_2MS_TICK)
#define TICK_2MS_TO_SEC(val)    (uint16)((val)/SEC_2MS_TICK)
#define MIN_2MS_TICK            (uint16)30000
#define MIN_TO_TICK_2MS(val)    (uint16)((val)*MIN_2MS_TICK)
#define TICK_2MS_TO_MIN(val)    (uint16)((val)/MIN_2MS_TICK)

#define SEC_1SEC_TICK           (uint16)1
#define SEC_TO_TICK_1SEC(val)   (uint16)(val)
#define TICK_1SEC_TO_SEC(val)   (uint16)(val)
#define MIN_1SEC_TICK           (uint16)60
#define MIN_TO_TICK_1SEC(val)   (uint16)((val)*60)
#define TICK_1SEC_TO_MIN(val)   (uint16)((val)/60)

#define MAX_TICK_VALUE  0xfeff  // 65280

#define FOREMOST_TIME   0xfff0
#define LATTERMOST_TIME 0xfff1

#define MIN_INTERVAL    0
#define MAX_INTERVAL    0xffff


extern volatile uint16 w2MsTickCnt;

#define TICK_2MS()                          \
{                                           \
    w2MsTickCnt++;                          \
    if(w2MsTickCnt >= MAX_TICK_VALUE)       \
    {                                       \
        w2MsTickCnt = 0;                    \
    }                                       \
}

#ifndef IN_TICK
extern volatile uint16 w1SecTickCnt;
#endif
#define TICK_1SEC()                         \
{                                           \
    w1SecTickCnt++;                         \
    if(w1SecTickCnt >= MAX_TICK_VALUE)      \
    {                                       \
        w1SecTickCnt = 0;                   \
    }                                       \
}                   


/* �������� */
uint16 Get2MsTickVal(void);
uint16 Get2MsTickInterval(uint16 wLastTickVal);
uint16 Get1SecTickVal(void);
uint16 Get1SecTickInterval(uint16 wLastTickVal);
void TickInit(void);

void TPM_ISR(void);
void DelayUs(uint16 wDelayVar);

#endif /* _TICK_H */
