#include "LIN_Slave.h"

LIN_State_T lin_stat;
LinMsgQue_T lin_msg_recv;

uint8_t lin_data_send[10];
uint8_t lin_data_recv[9];
volatile uint8_t send_bytes;

void InitLinMsgQue(LinMsgQue_T* msg_que)
{
	msg_que->front=0;
	msg_que->rear=0;
}


uint8_t IsLinQueFull(LinMsgQue_T * msg_que)  
{  
    return ((msg_que->rear+1)%QUEUE_SIZE == msg_que->front);  
}


uint8_t IsLinQueEmpty(LinMsgQue_T *msg_que)  
{  
    return (msg_que->front == msg_que->rear);  
}


void LinMsg_Enqueue(LinMsgQue_T *msg_que,LinMsg_T *msg)  
{
    if(IsLinQueFull(msg_que))  
    {  
        return;  
    } 
	msg_que->msg_buff[msg_que->rear].data=msg->data;
    msg_que->msg_buff[msg_que->rear].data_type=msg->data_type;
    msg_que->rear = (msg_que->rear+1)%QUEUE_SIZE;  
}


void LinMsg_Dequeue(LinMsgQue_T *msg_que,LinMsg_T *msg)  
{
    if(IsLinQueEmpty(msg_que))  
    {  
        return;  
    }  
	msg->data=msg_que->msg_buff[msg_que->front].data;
    msg->data_type=msg_que->msg_buff[msg_que->front].data_type;
    msg_que->front = (msg_que->front+1)%QUEUE_SIZE;  
	return;
}




uint8_t LIN_Checksum(uint8_t id,uint8_t *data,uint8_t length)
{
	uint8_t i;
	uint32_t check_sum = 0;
	if(id != 0x3c && id != 0x7d) 						 //使用增强型校验
	{
		check_sum  = id ;
	} 
	else 												 //使用标准校验
	{
		check_sum = 0 ;
	}
	for (i = 0; i < length; i++) 
	{	  
		check_sum += *(data++);
	
		if (check_sum > 0xFF)	  				     //进位
		{
			check_sum -= 0xFF;
		}
	}
	return (~check_sum);  							 //取反
}


void UART0_Task(UART_Type *pUART)
{
	LinMsg_T lin_msg_temp;
	if (UART0->S1 & UART_S1_RDRF_MASK)
	{
		if(UART_CheckFlag(TERM_PORT,UART_FlagRDRF))
		{
			if(UART_CheckFlag(TERM_PORT,UART_FlagFE))                 //receive break seg
			{
				lin_msg_temp.data=UART_GetChar(TERM_PORT);
				lin_msg_temp.data_type=LIN_MSG_TYPE_BREAK;
				LinMsg_Enqueue(&lin_msg_recv,&lin_msg_temp);
			}
			else
			{
				lin_msg_temp.data=UART_GetChar(TERM_PORT);
				lin_msg_temp.data_type=LIN_MSG_TYPE_DATA;
				LinMsg_Enqueue(&lin_msg_recv,&lin_msg_temp);
			}

		}
	}
	else if(UART0->S1 & UART_S1_TDRE_MASK)
	{
		send_bytes++;
		if(lin_data_send[0]<=send_bytes)
		{
			UART_DisableInterrupt(TERM_PORT,UART_TxCompleteInt);
			UART_EnableRx(TERM_PORT);
		}
		else
		{
			UART_PutChar(TERM_PORT,lin_data_send[send_bytes+1]);
		}
	}
	else
	{
		
	}
}


uint8_t LinRecvByte(uint8_t data_type,uint8_t data)
{
	uint8_t static recv_bytes;
	if(LIN_MSG_TYPE_BREAK==data_type)
	{
		lin_stat=_BREAK;
		return 0;
	}
	else
	{
		if(_BREAK==lin_stat)
		{
			if(LIN_SYNCH_DATA==data)
			{
				lin_stat++;
			}
			else
			{
				lin_stat=IDLE;
			}
			return 0;
		}
		else if(SYNCH==lin_stat)
		{
			lin_data_recv[0]=data;
			if(LIN_ID_MAIN==data || LIN_ID_DID_REQ==data)                      //the id request for receive task
			{
				lin_stat=PROTECTED_IDENTIFIER_RECV;
			}
			else if(LIN_ID_NODE==data || LIN_ID_DID_ACK==data)                 //the id request for send task
			{
				UART_DisableRx(TERM_PORT);	
				lin_stat=PROTECTED_IDENTIFIER_SEND;
			}
			else
			{
				lin_stat=IDLE;
			}
			return 0;
		}
		else if(PROTECTED_IDENTIFIER_RECV==lin_stat)
		{
			lin_data_recv[1]=data;
			recv_bytes=1;
			lin_stat=DATA_RECV;
			return 0;
		}
		else if(DATA_RECV==lin_stat)
		{
			recv_bytes++;
			lin_data_recv[recv_bytes]=data;
			if(LIN_ID_MAIN==lin_data_recv[0] && 8==recv_bytes)
			{
				lin_stat=DATA_RECV_OVER;
			}
			else if(LIN_ID_DID_REQ==lin_data_recv[0] && 5==recv_bytes)
			{
				lin_stat=DATA_RECV_OVER;	
			}
			else
			{
				
			}
			return 0;
		}
		else if(DATA_RECV_OVER==lin_stat)
		{
			recv_bytes++;
			lin_data_recv[recv_bytes]=data;
			lin_stat=CHECKSUM;
			return 1;
		}
		else
		{
			lin_stat=IDLE;
			return 0;
		}
	}
}




void Lin_Slave_Init(void)
{
	InitLinMsgQue(&lin_msg_recv);
	lin_stat=IDLE;
}

void UpDataSendNodeData(void)
{

	lin_data_send[0]=9;
	lin_data_send[1]=0;
	lin_data_send[2]=0;
	lin_data_send[3]=0;
	lin_data_send[4]=0;
	lin_data_send[5]=0;
	lin_data_send[6]=0;
	lin_data_send[7]=0;
	lin_data_send[8]=0;
	lin_data_send[9]=LIN_Checksum(LIN_ID_NODE,lin_data_send+1,8);
}

void UpDataSendDiagData(void)
{

	lin_data_send[0]=9;
	lin_data_send[1]=0;
	lin_data_send[2]=0;
	lin_data_send[3]=0;
	lin_data_send[4]=0;
	lin_data_send[5]=0;
	lin_data_send[6]=0;
	lin_data_send[7]=0;
	lin_data_send[8]=0;
	lin_data_send[9]=LIN_Checksum(LIN_ID_DID_ACK,lin_data_send+1,8);
}

void Lin_SendHandle(void)
{
	if(PROTECTED_IDENTIFIER_SEND==lin_stat)
	{
		if(LIN_ID_NODE==lin_data_recv[0])
		{
			UpDataSendNodeData();
			send_bytes=0;
			UART_PutChar(TERM_PORT,lin_data_send[1]);
			UART_EnableInterrupt(TERM_PORT,UART_TxCompleteInt);
		}
		else if(LIN_ID_DID_ACK==lin_data_recv[0])
		{
			UpDataSendDiagData();
			send_bytes=0;
			UART_PutChar(TERM_PORT,lin_data_send[1]);
			UART_EnableInterrupt(TERM_PORT,UART_TxCompleteInt);			
		}
		else
		{
			
		}
		lin_stat=DATA_SEND;
	}
}

void Lin_RecvHandle(void)
{
	LinMsg_T lin_msg_temp;
	if(!IsLinQueEmpty(&lin_msg_recv))
	{
		LinMsg_Dequeue(&lin_msg_recv,&lin_msg_temp);
		if(LinRecvByte(lin_msg_temp.data_type,lin_msg_temp.data))
		{

		}
		LED1_Toggle();		
	}
}

void Task_LIN_Slave(void)
{
//	static uint16 scan_time=FOREMOST_TIME;
	Lin_RecvHandle();
	Lin_SendHandle();

	/*if(Get2MsTickInterval(scan_time)>=500)
	{
			scan_time=Get2MsTickVal();
			UART_PutChar(TERM_PORT,0x55);
			LED0_Toggle(); 
	}*/  

	/*if(UART_CheckFlag(TERM_PORT,UART_FlagRDRF))
	{
		if(0x55==UART_GetChar(TERM_PORT))
		{
			LED0_Toggle();
		}
	}*/
}







