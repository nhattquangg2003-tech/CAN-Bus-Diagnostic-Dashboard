/*
 * main.h
 *
 *  Created on: Mar 2, 2026
 *      Author: Owner
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_
#include "stm32f1xx_hal.h"
#define SYS_CLOCK_FREQ_36_MHZ   36
#define SYS_CLOCK_FREQ_56_MHZ   56
#define SYS_CLOCK_FREQ_72_MHZ   72
void Error_handler(void);
#define TRUE  1
#define FALSE 0


#define LED1_PORT GPIOB
#define LED2_PORT GPIOB
#define LED3_PORT GPIOB
#define LED4_PORT GPIOB
#define LED5_PORT GPIOB

#define LED1_PIN_N0  GPIO_PIN_5
#define LED2_PIN_N0 GPIO_PIN_6
#define LED3_PIN_N0 GPIO_PIN_7
#define LED4_PIN_N0 GPIO_PIN_8
#define LED5_PIN_N0 GPIO_PIN_9

#endif /* INC_MAIN_H_ */
