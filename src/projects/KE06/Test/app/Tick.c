//==============================================================================
// Copyright (c) 2013,������˼Τ���������޹�˾. All rights reserved.
// 
// 
// �ļ�����: tick.c
// ժ    Ҫ: 
// ��    ��: 1.0
//
// �޸ļ�¼: 
//      1.0.0 : ��ʼ��д����(2013��12��12��) -> �
//==============================================================================
#include "Tick.h"

volatile uint16 w2MsCntForSecTick = 0;

/* ϵͳTick���� */
volatile uint16 w2MsTickCnt = 0;
volatile uint16 w1SecTickCnt = 0;

/* TICK�ж�ʹ�ܺ��� */
#define TICK_2MS_INT_EN()       //nop()//TPM1_TOIE_ENABLE()
#define TICK_2MS_INT_DIS()      //nop()//TPM1_TOIE_DISABLE()

#define TICK_1SEC_INT_EN()      //nop()//TPM1_TOIE_ENABLE()
#define TICK_1SEC_INT_DIS()     //nop()//TPM1_TOIE_DISABLE()



/*******************************************************************************
 * ��������: ���Tickֵ
 * �������: N/A
 * �������: N/A
 * ����ֵ  : Tickֵ
 ******************************************************************************/
uint16 Get2MsTickVal(void)
{
    volatile uint16 wTickValShade;

    TICK_2MS_INT_DIS();
    wTickValShade = w2MsTickCnt;
    TICK_2MS_INT_EN();
    
    return wTickValShade;
}

/*******************************************************************************
 * ��������: ��õ�ǰTick��ĳ��������Tick֮��Ĳ�ֵ��
 * �������: N/A
 * ���������N/A
 * ����ֵ  ��Tick��ֵ��
 ******************************************************************************/
uint16 Get2MsTickInterval(uint16 wLastTickVal)
{
    volatile uint16 wCurTickVal;
    volatile uint16 wTickInterval;
    volatile uint16 wLastTickValShade;

    if(FOREMOST_TIME == wLastTickVal)
    {
        return MAX_INTERVAL;
    }
    if(LATTERMOST_TIME == wLastTickVal)
    {
        return MIN_INTERVAL;
    }

    wLastTickValShade = wLastTickVal;
    
    TICK_2MS_INT_DIS();
    wCurTickVal = w2MsTickCnt;
    TICK_2MS_INT_EN();
    
    if(wCurTickVal >= wLastTickVal)
    {
        wTickInterval = wCurTickVal - wLastTickValShade;
    }
    else
    {
        wTickInterval = (MAX_TICK_VALUE - wLastTickValShade) + wCurTickVal;
    }

    return wTickInterval;        
}

/*******************************************************************************
 * ��������: ���Tickֵ
 * �������: N/A
 * �������: N/A
 * ����ֵ  : Tickֵ
 ******************************************************************************/
uint16 Get1SecTickVal(void)
{
    volatile uint16 wTickValShade;

    TICK_1SEC_INT_DIS();
    wTickValShade = w1SecTickCnt;
    TICK_1SEC_INT_EN();
    
    return wTickValShade;
}

/*******************************************************************************
 * ��������: ��õ�ǰTick��ĳ��������Tick֮��Ĳ�ֵ��
 * �������: N/A
 * ���������N/A
 * ����ֵ  ��Tick��ֵ��
 ******************************************************************************/
uint16 Get1SecTickInterval(uint16 wLastTickVal)
{
    volatile uint16 wCurTickVal;
    volatile uint16 wTickInterval;
    volatile uint16 wLastTickValShade;

    if(FOREMOST_TIME == wLastTickVal)
    {
        return MAX_INTERVAL;
    }
    if(LATTERMOST_TIME == wLastTickVal)
    {
        return MIN_INTERVAL;
    }

    wLastTickValShade = wLastTickVal;
    
    TICK_1SEC_INT_DIS();
    wCurTickVal = w1SecTickCnt;
    TICK_1SEC_INT_EN();
    
    if(wCurTickVal >= wLastTickVal)
    {
        wTickInterval = wCurTickVal - wLastTickValShade;
    }
    else
    {
        wTickInterval = (MAX_TICK_VALUE - wLastTickValShade) + wCurTickVal;
    }

    return wTickInterval;        
}


/*******************************************************************************
 * ��������: ϵͳ��ʱ�����������
 * �������: N/A
 * ���������N/A
 * ����ֵ  ��N/A
 ******************************************************************************/
void TPM_ISR(void)
{
    TICK_2MS();
    w2MsCntForSecTick++;
    if(w2MsCntForSecTick >= 500)
    {
        w2MsCntForSecTick = 0;
        TICK_1SEC();
    }
}


/*******************************************************************************
 * ��������: Tick��ʼ������
 * �������: N/A
 * ���������N/A
 * ����ֵ  ��N/A
 ******************************************************************************/
void TickInit(void)
{
    /* ϵͳTick������ʼ�� */
    w2MsTickCnt = 1;
    w1SecTickCnt = 1;
}

/*******************************************************************************
 * ��������: ��ʱ����
 * �������: N/A
 * ���������N/A
 * ����ֵ  ��N/A
 ******************************************************************************/
void DelayUs(uint16 wDelayVar) 
{                              
    while (wDelayVar != 0)      
    {                                                          
        wDelayVar--;             
    }                           
}


