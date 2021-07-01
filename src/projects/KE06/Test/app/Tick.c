//==============================================================================
// Copyright (c) 2013,江苏罗思韦尔电气有限公司. All rights reserved.
// 
// 
// 文件名称: tick.c
// 摘    要: 
// 版    本: 1.0
//
// 修改记录: 
//      1.0.0 : 开始编写程序(2013年12月12日) -> 李晶
//==============================================================================
#include "Tick.h"

volatile uint16 w2MsCntForSecTick = 0;

/* 系统Tick变量 */
volatile uint16 w2MsTickCnt = 0;
volatile uint16 w1SecTickCnt = 0;

/* TICK中断使能函数 */
#define TICK_2MS_INT_EN()       //nop()//TPM1_TOIE_ENABLE()
#define TICK_2MS_INT_DIS()      //nop()//TPM1_TOIE_DISABLE()

#define TICK_1SEC_INT_EN()      //nop()//TPM1_TOIE_ENABLE()
#define TICK_1SEC_INT_DIS()     //nop()//TPM1_TOIE_DISABLE()



/*******************************************************************************
 * 函数介绍: 获得Tick值
 * 输入参数: N/A
 * 输出参数: N/A
 * 返回值  : Tick值
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
 * 函数介绍: 获得当前Tick与某个给定的Tick之间的差值。
 * 输入参数: N/A
 * 输出参数：N/A
 * 返回值  ：Tick差值。
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
 * 函数介绍: 获得Tick值
 * 输入参数: N/A
 * 输出参数: N/A
 * 返回值  : Tick值
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
 * 函数介绍: 获得当前Tick与某个给定的Tick之间的差值。
 * 输入参数: N/A
 * 输出参数：N/A
 * 返回值  ：Tick差值。
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
 * 函数介绍: 系统延时函数服务程序
 * 输入参数: N/A
 * 输出参数：N/A
 * 返回值  ：N/A
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
 * 函数介绍: Tick初始化函数
 * 输入参数: N/A
 * 输出参数：N/A
 * 返回值  ：N/A
 ******************************************************************************/
void TickInit(void)
{
    /* 系统Tick变量初始化 */
    w2MsTickCnt = 1;
    w1SecTickCnt = 1;
}

/*******************************************************************************
 * 函数介绍: 延时函数
 * 输入参数: N/A
 * 输出参数：N/A
 * 返回值  ：N/A
 ******************************************************************************/
void DelayUs(uint16 wDelayVar) 
{                              
    while (wDelayVar != 0)      
    {                                                          
        wDelayVar--;             
    }                           
}


