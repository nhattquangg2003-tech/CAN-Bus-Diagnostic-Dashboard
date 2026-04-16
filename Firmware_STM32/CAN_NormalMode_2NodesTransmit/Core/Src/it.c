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
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htimer3;
void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}
void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}
void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}
void USB_HP_CAN1_TX_IRQHandler (void)
{
	HAL_CAN_IRQHandler(&hcan1);
}
void USB_LP_CAN1_RX0_IRQHandler (void)
{

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
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer3);

}
//
//
//
void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart2);

}
//
//
//
/**
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  /* USER CODE END ADC1_2_IRQn 1 */
}
