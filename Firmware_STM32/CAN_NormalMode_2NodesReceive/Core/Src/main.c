/*
 * main.c
 *
 *  Created on: Mar 2, 2026
 *      Author: Owner
 */
#include "main.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
// declare function
void SystemClockConfig(uint8_t clock_freq);
void Error_handler(void);
void GPIO_Init(void);
void MX_USART2_UART_Init(void);
//can configguration function
////
////
void CAN_Init(void);
void CAN_Tx(void);
void CAN_Rx(void);
void CAN_Filter_Config(void);
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan);
void Send_response(uint32_t StdId);
void LED_Manage_Output(uint8_t led_no);
//
// timer 2
//
void TIMER2_Init(void);
// private variable
UART_HandleTypeDef huart2;
CAN_HandleTypeDef hcan1;
TIM_HandleTypeDef htimer2;
CAN_RxHeaderTypeDef RxHead = {0};
uint8_t req_counter = 0 ;
/**
 * @brief  Print a string to console over UART.
 * @param  format: Format string as in printf.
 * @param  ...: Additional arguments providing the data to print.
 * @retval None
 */
char str[80];

void printmsg(char *format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(str, format, args);
    HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY); // blocking
    va_end(args);
}
// main
int main (void)
{

	HAL_Init();

	SystemClockConfig(SYS_CLOCK_FREQ_56_MHZ);

	GPIO_Init();

	MX_USART2_UART_Init();

	TIMER2_Init();




	CAN_Init();

	CAN_Filter_Config();
	printmsg("=============================================\r\n");
	printmsg("==========Giao tiep CAN giua 2 NODE==========\r\n");
	printmsg("=============================================\r\n");
	printmsg("=============================================\r\n");
	printmsg("==========  DAY LA NODE 2 - NODE NHAN ==========\r\n");
	printmsg("=============================================\r\n");

	if (HAL_CAN_ActivateNotification(&hcan1,
	    CAN_IT_TX_MAILBOX_EMPTY |
	    CAN_IT_RX_FIFO0_MSG_PENDING |  //
	    CAN_IT_BUSOFF) != HAL_OK)
	{
	    Error_handler();
	}
	// starting the can
	if (HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		Error_handler();
	}
	while (1)
	{

	}
	return 0;
}


void SystemClockConfig(uint8_t clock_freq)
{
	RCC_OscInitTypeDef Osc_Init = {0};
	RCC_ClkInitTypeDef Clock_Init = {0};

	// F103 dùng HSI 8MHz làm nguồn PLL
	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
	Osc_Init.HSIState = RCC_HSI_ON;
	Osc_Init.HSEState = RCC_HSE_ON;
	Osc_Init.HSIState = RCC_HSI_ON;
	Osc_Init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2; // HSI/2 = 4MHz vào PLL

	switch(clock_freq)
	{
	case SYS_CLOCK_FREQ_36_MHZ:
		// 4MHz × 9 = 36MHz
		Osc_Init.PLL.PLLMUL = RCC_PLL_MUL9;
		Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK |
				RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		Clock_Init.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
		Clock_Init.AHBCLKDivider  = RCC_SYSCLK_DIV1;
		Clock_Init.APB1CLKDivider = RCC_HCLK_DIV1; // APB1 max 36MHz → OK
		Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
		break;

	case SYS_CLOCK_FREQ_56_MHZ:
		// 4MHz × 14 = 56MHz
		Osc_Init.PLL.PLLMUL = RCC_PLL_MUL14;
		Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK |
				RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		Clock_Init.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
		Clock_Init.AHBCLKDivider  = RCC_SYSCLK_DIV1;
		Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2; // APB1 max 36MHz → 56/2=28MHz OK
		Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
		break;

	case SYS_CLOCK_FREQ_72_MHZ:
		Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		Osc_Init.HSEState = RCC_HSE_ON;
		Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE; // HSE 8MHz
		Osc_Init.PLL.PLLMUL = RCC_PLL_MUL9;         // 8MHz × 9 = 72MHz ✅
		Clock_Init.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
		Clock_Init.AHBCLKDivider  = RCC_SYSCLK_DIV1;
		Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;  // 36MHz
		Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;  // 72MHz
		break;

	default:
		return;
	}


	HAL_RCC_OscConfig(&Osc_Init);
	HAL_RCC_ClockConfig(&Clock_Init, FLASH_LATENCY_2);

	/*Configure the systick timer interrupt frequency (for every 1 ms) */
	uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_freq/1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}
/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_handler(void)
{
	while(1);
}
//
//
//
//
//
//
void GPIO_Init(void)
{
	// LED PB2 and 	// led for can
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio = {0};
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pin = GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_5;
	ledgpio.Pull = GPIO_NOPULL;
	ledgpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &ledgpio);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_toggle = {0};
	gpio_toggle.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_toggle.Pin = GPIO_PIN_5;
	gpio_toggle.Pull = GPIO_NOPULL;
	gpio_toggle.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio_toggle);

	GPIO_InitTypeDef GpioBtn = {0};
	GpioBtn.Mode = GPIO_MODE_IT_FALLING;
	GpioBtn.Pin = GPIO_PIN_0;
	GpioBtn.Pull = GPIO_PULLDOWN;
	GpioBtn.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GpioBtn);

	// IRQ CONFIGURATIONS
	HAL_NVIC_SetPriority(EXTI0_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);



}
/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */
}
//
//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}
//
//
//
void CAN_Init(void)
{
	hcan1.Instance = CAN1;
	hcan1.Init.Mode =  CAN_MODE_NORMAL;
	hcan1.Init.AutoBusOff = ENABLE;
	hcan1.Init.AutoRetransmission = ENABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;

	// setting realted to can bit timmings
	hcan1.Init.Prescaler = 4;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_11TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;

	if ( HAL_CAN_Init(&hcan1) != HAL_OK )
	{
		Error_handler();
	}

}
///
///
///
void CAN_Tx(void)
{

}
///
///
///
void CAN_Rx(void)
{
	CAN_RxHeaderTypeDef RxHead = {0};
	uint8_t rcvd_msgg[8];
	// wait until at least one message in to the rx fifo0


	while(! HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0));
	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHead, rcvd_msgg) != HAL_OK )
	{
		Error_handler();
	}

	printmsg("Message Receive Successfully: %s \r\n",rcvd_msgg);
}
//
//
//
void CAN_Filter_Config(void)
{
	CAN_FilterTypeDef can1_filter_init;

	can1_filter_init.FilterActivation = ENABLE;
	can1_filter_init.FilterBank = 0;
	can1_filter_init.FilterFIFOAssignment = CAN_RX_FIFO0;
	can1_filter_init.FilterIdHigh = 0x0000;
	can1_filter_init.FilterIdLow = 0x0000;
	can1_filter_init.FilterMaskIdHigh = 0x0000;
	can1_filter_init.FilterMaskIdLow = 0x0000;
	can1_filter_init.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_filter_init.FilterScale = CAN_FILTERSCALE_32BIT;


	if (HAL_CAN_ConfigFilter(&hcan1, &can1_filter_init) != HAL_OK )
	{
		Error_handler();
	}
}
/**
 * @brief  Transmission Mailbox 0 complete callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{

}

/**
 * @brief  Transmission Mailbox 1 complete callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{

}

/**
 * @brief  Transmission Mailbox 2 complete callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{

}
static uint8_t rcvd_msgs[8] = {0};
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{


	// wait until at least one message in to the rx fifo0

	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHead, rcvd_msgs) != HAL_OK )
	{
		Error_handler();
	}
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	if (RxHead.StdId == 0x300 && RxHead.RTR == CAN_RTR_DATA)
	{
		// this is data frame send by n1 with led
		LED_Manage_Output(rcvd_msgs[0]);
		printmsg("LED=%#X\r\n",rcvd_msgs[0]);

	} 	else if (RxHead.StdId == 0x200  && RxHead.RTR == CAN_RTR_DATA)
	{
        // this ís data frame send by n1 with speed
		printmsg("SPEED=%d\r\n", rcvd_msgs[0]);
	}
	else if (RxHead.StdId == 0x100  && RxHead.RTR == CAN_RTR_DATA)
		{
	        // this ís data frame send by n1 with speed
			printmsg("FUEL=%d\r\n", rcvd_msgs[0]);
		}
	else if (RxHead.StdId == 0x651 && RxHead.RTR == CAN_RTR_REMOTE)
	{
		// this is a request data from n1
		// this is a remote frame send by n1 to n2
		Send_response(RxHead.StdId);
		return;
	}
}
/**
 * @brief  Error CAN callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	printmsg("CAN Error Detected \r\n");
}
//
//
// timer 2
void TIMER2_Init(void)
{
	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Prescaler = 5600 - 1;
	htimer2.Init.Period = 10000 - 1;
	if (HAL_TIM_Base_Init(&htimer2) != HAL_OK)
	{
		Error_handler();
	}
}
//
//
//
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

}
//
//
//
// function of can
void Send_response(uint32_t StdId)
{
	CAN_TxHeaderTypeDef TxHeader  = {0};

	uint32_t TxMailbox = 0;

	uint8_t response[2] = {0xAB,0XCD};

	TxHeader.DLC = 2;
	TxHeader.StdId = StdId;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;

	if ( 	HAL_CAN_AddTxMessage(&hcan1, &TxHeader,response, &TxMailbox) != HAL_OK )
	{
		Error_handler();
	}
}
void LED_Manage_Output(uint8_t led_no)
{

	if (0x01 & led_no)
	{
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_N0, GPIO_PIN_SET);
	} else
	{
		HAL_GPIO_WritePin(LED1_PORT, LED1_PIN_N0, GPIO_PIN_RESET);
	}

	if (0x02 & led_no)
	{
		HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_N0, GPIO_PIN_SET);

	}else
	{
		HAL_GPIO_WritePin(LED2_PORT, LED2_PIN_N0, GPIO_PIN_RESET);
	}

	if (0x04 & led_no)
	{
		HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_N0, GPIO_PIN_SET);
	}else
	{
		HAL_GPIO_WritePin(LED3_PORT, LED3_PIN_N0, GPIO_PIN_RESET);
	}

	if (0x08 & led_no)
	{
		HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_N0, GPIO_PIN_SET);
	}else
	{
		HAL_GPIO_WritePin(LED4_PORT, LED4_PIN_N0, GPIO_PIN_RESET);
	}
	if (0x10 & led_no)
	{
		HAL_GPIO_WritePin(LED5_PORT, LED5_PIN_N0, GPIO_PIN_SET);
	}else
	{
		HAL_GPIO_WritePin(LED5_PORT, LED5_PIN_N0, GPIO_PIN_RESET);
	}

}


