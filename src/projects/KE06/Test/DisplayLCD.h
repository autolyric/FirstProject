#ifndef _DISPLAYLCD_H
#define _DISPLAYLCD_H

#include "common.h"




typedef struct
{
    uint8    Sg_Non2        :4;
    uint8    Sg_AC          :1;           
    uint8    Sg_Def         :1;
    uint8    Sg_Auto        :1;
    uint8    Sg_Non1        :1;
        
    uint8    Num1           :7;           //degree sign
    uint8    SgC            :1;
    
    uint8    Num2           :7;
    uint8    SgI            :1;
    
    uint8    Num3           :7;
    uint8    SgDot          :1;
    
    uint8    Mode           :4;
    uint8    Cyc            :4;
    
    uint8    WinForce       :8;        
}Display_Seg;
   
void Lcd_Display_Init(void);
void Task_Lcd_Display(void);

#endif
