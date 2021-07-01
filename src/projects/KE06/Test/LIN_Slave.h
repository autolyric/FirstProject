#ifndef __LINSLAVE_H
#define __LINSLAVE_H

#include "common.h"
#include "uart.h"
#include "Tick.h"



typedef enum
{
IDLE =0 , 
_BREAK, 
SYNCH, 
PROTECTED_IDENTIFIER_SEND,
PROTECTED_IDENTIFIER_RECV,
DATA_SEND,
DATA_RECV,
DATA_RECV_OVER,
CHECKSUM 
}LIN_State_T ;



#define  LIN_ID_MAIN         0xEC
#define  LIN_ID_NODE         0x2B

#define  LIN_ID_DID_REQ      0x3C
#define  LIN_ID_DID_ACK      0x7D


#define LIN_SYNCH_DATA       0x55


#define LIN_MSG_TYPE_BREAK   0x00
#define LIN_MSG_TYPE_DATA    0x01



#define QUEUE_SIZE          30

typedef struct      
{          
    uint8_t  data;          //msg data
	uint8_t  data_type;     //msg type
}LinMsg_T;

typedef struct
{
	LinMsg_T msg_buff[QUEUE_SIZE];
	uint8_t front;
	uint8_t rear;
}LinMsgQue_T;





void UART0_Task(UART_Type *pUART);
void Task_LIN_Slave(void);
void Lin_Slave_Init(void);


#endif
