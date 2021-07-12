/*
 * isr.h
 *
 *  Created on: Mar 10, 2017
 *      Author: Administrator
 */

#ifndef ISR_H_
#define ISR_H_


#undef  VECTOR_038 
#define VECTOR_038 PIT_Ch0Isr   /*!< Vector 38 points to PIT channel 0 interrupt service routine */

#undef  VECTOR_039 
#define VECTOR_039 PIT_Ch1Isr   /*!< Vector 39 points to PIT channel 1 interrupt service routine */

#undef  VECTOR_046
#define VECTOR_046 MSCAN_RxIsr          /*!< Vector 36 points to RTC interrupt service routine */ 

#undef  VECTOR_047
#define VECTOR_047 MSCAN_TxIsr          /*!< Vector 36 points to RTC interrupt service routine */ 


extern void PIT_Ch1Isr(void);
extern void PIT_Ch0Isr(void);
extern void MSCAN_RxIsr(void);
extern void MSCAN_TxIsr(void);


extern void RTC_Isr(void);


#endif /* ISR_H_ */
