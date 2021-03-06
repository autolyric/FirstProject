#include "System.h"

FrameBufferInfoType	 sCAN_TxBuff;
MSCAN_RegisterFrameType sCAN_TxMsg;
MSCAN_FrameType  				CAN_Rx_Msg;
MSCAN_FrameType      sRxFrame[CAN_BUFFER_LENGTH];
uint8_t u8RxFrameBufferIndex;
uint8_t u8RxFrameHeader;
uint8_t u8RxFrameBufferFreeLength;
uint32_t u32RxInterruptCounter=0;


void PIT1_Task(void)
{
	//LED0_Toggle();          /*!< toggle LED1 */
}


void PIT0_Task(void)
{
	TPM_ISR();
}


void Platform_Init (void)
{
    SIM_ConfigType  sSIMConfig = {{0},0};
    ICS_ConfigType  sICSConfig = {0};
    

#if defined(TRIM_IRC)    
    /* if not trimmed, do trim first */
    ICS_Trim(ICS_TRIM_VALUE);
#endif  
    /* 
     * Enable SWD pin, RESET pin 
     */
    /* 
     * NOTE: please make sure other register bits are also write-once and
     * need add other bit mask here if needed.
     */
#if defined(SPI0_PINREMAP)        
    sSIMConfig.u32PinSel |= SIM_PINSEL_SPI0PS_MASK;
#endif

#if defined(OUTPUT_BUSCLK)
    sSIMConfig.sBits.bEnableCLKOUT = 1;      /* output bus clock if enabled */
#endif
    
#if defined(DISABLE_NMI)
    sSIMConfig.sBits.bDisableNMI = 1;
#endif
    
    #if !defined(CPU_KEA64) 
    /* make sure clocks to peripheral modules are enabled */
    sSIMConfig.u32SCGC |= SIM_SCGC_SWD_MASK | SIM_SCGC_FLASH_MASK |
                         SIM_SCGC_UART0_MASK | SIM_SCGC_UART1_MASK | 
                         SIM_SCGC_UART2_MASK
                         ;
    #else
    sSIMConfig.u32SCGC |= SIM_SCGC_SWD_MASK | SIM_SCGC_FLASH_MASK |
                         SIM_SCGC_UART0_MASK 
                         ;
    #endif
    
    #if !defined(CPU_KEA8)
    /* bus clock divided by 2 */
    sSIMConfig.u32CLKDIV |= SIM_CLKDIV_OUTDIV2_MASK;                           
    #endif
      
    SIM_Init(&sSIMConfig);                   /* initialize SIM */
        
#if defined(XOSC_STOP_ENABLE)   
    sICSConfig.oscConfig.bStopEnable = 1;    /*  enabled in stop mode */   
#endif          
    
#if defined(CRYST_HIGH_GAIN)    
    sICSConfig.oscConfig.bGain = 1;           /* high gain */      
#endif

    
#if  (EXT_CLK_FREQ_KHZ >=4000)
    sICSConfig.oscConfig.bRange = 1;           /* high range */
#endif
	
    sICSConfig.oscConfig.bEnable = 1;          /* enable OSC */
    sICSConfig.u32ClkFreq = EXT_CLK_FREQ_KHZ;
    
#if     defined(USE_FEE)    
    sICSConfig.u8ClkMode = ICS_CLK_MODE_FEE;    
#elif	defined(USE_FBE_OSC)
    sICSConfig.u8ClkMode = ICS_CLK_MODE_FBE_OSC;    
#elif	defined(USE_FEE_OSC)   
    sICSConfig.u8ClkMode = ICS_CLK_MODE_FEE_OSC;      
#endif      

    ICS_Init(&sICSConfig);   /* initialize ICS */
                    
#if	defined(PRINT_SYS_LOG)
    
#endif
    
 }



void System_PitInit(void)
{
	uint32_t        u32LoadValue0, u32LoadValue1;     
	PIT_ConfigType  sPITConfig0, sPITConfig1;    
	PIT_ConfigType  *pPIT_Config1   =&sPITConfig1;
	PIT_ConfigType  *pPIT_Config0   =&sPITConfig0; 

	u32LoadValue0   = 24000;                  /*!< PIT ch0 timer load value  */ 
	u32LoadValue1   = 1000;                     /*!< PIT ch1 timer load value  */     
	
	/* configure PIT channel 1 in chain mode, enable interrupt and timer */
	pPIT_Config1->u32LoadValue      = u32LoadValue1;
	pPIT_Config1->bFreeze           = FALSE;
	pPIT_Config1->bModuleDis        = FALSE;    /*!< enable PIT module */ 
	pPIT_Config1->bInterruptEn      = TRUE;
	pPIT_Config1->bChainMode        = TRUE;
	pPIT_Config1->bTimerEn          = TRUE; 
	
	/* configure PIT channel 0, only enable timer */    
	pPIT_Config0->u32LoadValue      = u32LoadValue0;
	pPIT_Config0->bFreeze           = FALSE;    
	pPIT_Config0->bModuleDis        = FALSE;    /*!< enable PIT module */     
	pPIT_Config0->bInterruptEn      = TRUE;
	pPIT_Config0->bChainMode        = FALSE; 
	pPIT_Config0->bTimerEn          = TRUE;
	
	PIT_Init(PIT_CHANNEL0, pPIT_Config0);               
	PIT_Init(PIT_CHANNEL1, pPIT_Config1);
	  
	PIT_SetCallback(PIT_CHANNEL1, PIT1_Task);
	PIT_SetCallback(PIT_CHANNEL0, PIT0_Task);

}

void MSCAN_RxProcessing(void)
{
		if(CAN_IsRxBuffFull(MSCAN))
		{
				//Mytask();
				CAN_ReadOneFrameFromBuff(MSCAN,&CAN_Rx_Msg);
				if(u8RxFrameBufferFreeLength!=0)
				{
						CAN_ReadOneFrameFromBuff(MSCAN,&sRxFrame[u8RxFrameBufferIndex++]);	
						if(u8RxFrameBufferIndex>=CAN_BUFFER_LENGTH)
						{
							u8RxFrameBufferIndex = 0;
						}
						
						u8RxFrameBufferFreeLength--;
				}
				else
				{
						CAN_ClearRXF_Flag(MSCAN);
				}
		}
}
void MSCAN_TxProcessing(void)
{
	if(CAN_IsOverRunFlag(MSCAN))
	{
		// overrunn error occur
		CAN_ClearOVRIF_Flag(MSCAN);
	}
	if(CAN_IsWakeUpIntFlag(MSCAN))
	{
		CAN_ClearWUPIF_Flag(MSCAN);
	// user processing
	}
	if(CAN_IsStatusChangeFlag(MSCAN))
	{
		CAN_ClearCSCIF_Flag(MSCAN);
		// Get currently status
		CAN_GetReceiverStatus(MSCAN);
		CAN_GetReceiveErrorCount(MSCAN);
		// user processing
	}
}


void MSCAN_GlobeVaribleInit( MSCAN_Type *pCANx )
{
		uint8_t Msg_Data[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    sCAN_TxBuff.u8FreeLength = CAN_BUFFER_LENGTH;
    sCAN_TxBuff.u8Index = 0;
    sCAN_TxBuff.u8Head = 0; 
    u8RxFrameBufferFreeLength = CAN_BUFFER_LENGTH;
    u8RxFrameBufferIndex = 0;
    u8RxFrameHeader = 0;
    MSCAN_SetRxCallBack(MSCAN_RxProcessing);
    MSCAN_SetTxCallBack(MSCAN_TxProcessing);
	
		sCAN_TxMsg.BPR=0x01;
		sCAN_TxMsg.DLR=0x08;
		
		sCAN_TxMsg.EDSR[0] = Msg_Data[0];
		sCAN_TxMsg.EDSR[1] = Msg_Data[1];
		sCAN_TxMsg.EDSR[2] = Msg_Data[2];
		sCAN_TxMsg.EDSR[3] = Msg_Data[3];
		sCAN_TxMsg.EDSR[4] = Msg_Data[4];
		sCAN_TxMsg.EDSR[5] = Msg_Data[5];
		sCAN_TxMsg.EDSR[6] = Msg_Data[6];
		sCAN_TxMsg.EDSR[7] = Msg_Data[7];
		
		sCAN_TxMsg.EIDR0=0x01;
		sCAN_TxMsg.EIDR1=0x02;
		sCAN_TxMsg.EIDR2=0x03;
		sCAN_TxMsg.EIDR3=0x04;
		
}

void TestCAN_SendCycle()
{
	static uint16 cycle_time=FOREMOST_TIME;
	if(Get2MsTickInterval(cycle_time)>=10)
	{
		cycle_time=Get2MsTickVal();
		sCAN_TxMsg.EDSR[0]=CAN_Rx_Msg.DSR[0];
		sCAN_TxMsg.EDSR[1]=CAN_Rx_Msg.DSR[1];
		sCAN_TxMsg.EDSR[2]=CAN_Rx_Msg.DSR[2];
		sCAN_TxMsg.EDSR[3]=CAN_Rx_Msg.DSR[3];
		sCAN_TxMsg.EDSR[4]=CAN_Rx_Msg.DSR[4];
		sCAN_TxMsg.EDSR[5]=CAN_Rx_Msg.DSR[5];
		sCAN_TxMsg.EDSR[6]=CAN_Rx_Msg.DSR[6];
		sCAN_TxMsg.EDSR[7]=CAN_Rx_Msg.DSR[7];
		CAN_LoadOneFrameToBuff(MSCAN,&sCAN_TxMsg);       
	}
}

void System_CanInit(void)
{
    MSCAN_ConfigType sMSCANConfig = {0};  
    MSCAN_GlobeVaribleInit(MSCAN);
    SIM->PINSEL1 &=~ 0;//&=~ CAN_RX on TH2; |= CAN_TX on PTE7,
    sMSCANConfig.sBaudRateSetting.SJW = 3; //SJW=0
    sMSCANConfig.sBaudRateSetting.BRP = 3; //1 500K  3 250K
    sMSCANConfig.sBaudRateSetting.SAMP = BAUD_RATE_SAMP; // 0
    sMSCANConfig.sBaudRateSetting.TSEG1= 14;  // 4
    sMSCANConfig.sBaudRateSetting.TSEG2= 3;  //3
    sMSCANConfig.u32IDAR0 = (CAN_IDAR0);  //CANIDAR0-3
    sMSCANConfig.u32IDAR1 = (CAN_IDAR1);  //CANIDAR4-7
    sMSCANConfig.u32IDMR0 = (CAN_IDMR0);  //CANIDMR0-3
    sMSCANConfig.u32IDMR1 = (CAN_IDMR1);  //CANIDMR4-7
    sMSCANConfig.u8IDARMode = 0x00;//ID_ACCEPT_MODE_TWO32;//ID_ACCEPT_MODE_EIGHT8; // 2 32BIT filters  ID_ACCEPT_MODE_EIGHT8;//= 
    sMSCANConfig.sSetting.bCanEn = 1;      // CAN module enable
    sMSCANConfig.sSetting.bCLKSRC = 1;  // clock source is bus clock:20Mhz 0:external 8Mhz
    sMSCANConfig.sSetting.bRxFullIEn=1;  //receive full interrupt request enable
    sMSCANConfig.sSetting.bTimerEn=1;   // enable internal MSCAN timer
    sMSCANConfig.sSetting.bOverRunIEn=1; //overrun interrup enable
    sMSCANConfig.sSetting.bStatusChangeIEn=1; // Can status change interrupt enable
    sMSCANConfig.sSetting.bBusOffUser=1;  //Bus off recovery mode// 1 - user, 0 - Auto
    CAN_Init(MSCAN,&sMSCANConfig); 
}

void System_Init(void)
{	
	GPIO_PinInit(GPIO_PTH0,GPIO_PinOutput);
	GPIO_PinSet(GPIO_PTH0);
	
	GPIO_PinInit(GPIO_PTB4,GPIO_PinOutput);
	GPIO_PinClear(GPIO_PTB4);
	
	Platform_Init();
	System_PitInit();
	System_CanInit();
	
	TickInit();
}




