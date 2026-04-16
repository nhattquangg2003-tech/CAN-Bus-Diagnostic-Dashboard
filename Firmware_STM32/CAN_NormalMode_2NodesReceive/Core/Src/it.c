/*
 * it.c
 *
 *  Created on: Mar 2, 2026
 *      Author: Owner
 */
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htimer2;
void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);

}
void USB_HP_CAN1_TX_IRQHandler (void)
{
	HAL_CAN_IRQHandler(&hcan1);
}
void USB_LP_CAN1_RX0_IRQHandler(void)  //
{
    HAL_CAN_IRQHandler(&hcan1);
}
void CAN1_RX1_IRQHandler (void)
{

}
void CAN1_SCE_IRQHandler (void)
{

}
//
//
//
void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer2);

}
