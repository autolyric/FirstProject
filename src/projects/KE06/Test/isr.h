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

#undef  VECTOR_028
#define VECTOR_028 UART0_Isr

#undef  VECTOR_031
#define VECTOR_031 ADC_Isr

#undef  VECTOR_024 
#define VECTOR_024 I2C0_Isr

#undef  VECTOR_026 
#define VECTOR_026 SPI0_Isr


extern void PIT_Ch1Isr(void);
extern void PIT_Ch0Isr(void);
extern void UART0_Isr(void);
extern void ADC_Isr(void);
extern void I2C0_Isr(void);
extern void SPI0_Isr(void);



extern void RTC_Isr(void);


#endif /* ISR_H_ */
