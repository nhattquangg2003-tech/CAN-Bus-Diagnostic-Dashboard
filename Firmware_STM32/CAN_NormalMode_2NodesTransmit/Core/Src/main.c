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
#include <stdarg.h>  // ← cho va_list, va_start, va_end
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
void CAN_Tx_Uart(uint8_t message);
void CAN_TxADC(void);
// led
uint8_t led_no1 = 0x00;
uint8_t led_no2 = 0x00;
uint8_t led_no3 = 0x00;
uint8_t led_no4 = 0x00;
uint8_t led_no5 = 0x00;
uint8_t led_pa5_state = 0;
//
// bien tro
uint16_t speed = 0;
uint16_t fuel = 0;
//
// timer 2
//
void TIMER2_Init(void);
//
// timer 3
//
void TIMER3_Init(void);
TIM_HandleTypeDef htimer3;



// private variable
UART_HandleTypeDef huart2;
CAN_HandleTypeDef hcan1;
TIM_HandleTypeDef htimer2;
CAN_RxHeaderTypeDef RxHead = {0};
uint8_t req_counter = 0;
//
//
//
// uart interupt
uint8_t  data_buffer[100];
uint8_t  recvd_data;
uint32_t count=0;
uint8_t  reception_complete = FALSE;
//
//
// config for adc
ADC_HandleTypeDef hadc1;
static void MX_ADC1_Init(void);
/**
 * @brief  Print a string to console over UART.
 * @param  format: Format string as in printf.
 * @param  ...: Additional arguments providing the data to print.
 * @retval None
 */
char str[500];  // khai báo global

void printmsg(char *format,...)
{
    va_list args;
    va_start(args, format);
    vsprintf(str, format, args);
    // Dùng Transmit thường (không có _IT) để đảm bảo dữ liệu lên đủ
    HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
    va_end(args);
}
// main
int main (void)
{

	HAL_Init();

	SystemClockConfig(SYS_CLOCK_FREQ_56_MHZ);

	GPIO_Init();

	MX_USART2_UART_Init();
	HAL_UART_Receive_IT(&huart2,&recvd_data,1);
	TIMER2_Init();
	TIMER3_Init();


	CAN_Init();

	CAN_Filter_Config();
	printmsg("=============================================\r\n");
	printmsg("==========Giao tiep CAN giua 2 NODE==========\r\n");
	printmsg("=============================================\r\n");
	//
	printmsg("=============================================\r\n");
	printmsg("==========  DAY LA NODE 1 - NODE TRUYEN ==========\r\n");
	printmsg("=============================================\r\n");

	if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF) != HAL_OK)
	{
		Error_handler();
	}
	// starting the can
	if (HAL_CAN_Start(&hcan1) != HAL_OK)
	{
		Error_handler();
	}

	// adc init
	MX_ADC1_Init();
	// starrt timer 2






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
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	// LED Port B
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	// LED PA5
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	// PA0 -
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	// Nút nhấn PB1
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Nút nhấn PB3
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	// Nút nhấn PB4
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	// Nút nhấn PB5
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	// Nút nhấn PB6
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	// NVIC
	HAL_NVIC_SetPriority(EXTI0_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	HAL_NVIC_SetPriority(EXTI3_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
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
	static uint32_t last_time0 = 0;
	static uint32_t last_time1 = 0;
	static uint32_t last_time2 = 0;
	static uint32_t last_time3 = 0;
	static uint32_t last_time4 = 0;
	static uint32_t last_time5 = 0;
	uint32_t current_time = HAL_GetTick();

	//  PA0
	if (GPIO_Pin == GPIO_PIN_0)
	{
		if (current_time - last_time0 > 200) // Chống dội 200ms
		{
			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
			{

				HAL_TIM_Base_Start_IT(&htimer2);
				// bỏ timer 3 HAL_TIM_Base_Start_IT(&htimer3);

			}
			last_time0 = current_time;
		}
	}

	//  PB1
	if (GPIO_Pin == GPIO_PIN_1)
	{
		if (current_time - last_time1 > 200) // Chống dội 200ms
		{
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
			{
				CAN_TxHeaderTypeDef TxHeader;
				uint32_t TxMailbox;

				uint8_t message;

				if (req_counter == 4)
				{
					// n1 sending remote frame to n2
					TxHeader.DLC = 2; // n1 demanding 2 byte of reply
					TxHeader.StdId = 0x651;
					TxHeader.IDE = CAN_ID_STD;
					TxHeader.RTR = CAN_RTR_REMOTE;

					if ( 	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message, &TxMailbox) != HAL_OK )
					{
						Error_handler();
					}

					// reset biến counter
					req_counter = 0;

				} else
				{
					led_no1 = !led_no1;
					CAN_Tx();
					req_counter++;
				}
			}
			last_time1 = current_time;
		}
	}

	// PB3
	if (GPIO_Pin == GPIO_PIN_3)
	{
		if (current_time - last_time2 > 200) // Chống dội 200ms
		{
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_SET)
			{
				led_no3 = !led_no3;
				CAN_Tx();

			}
			last_time2 = current_time;
		}
	}
	// PB4
	if (GPIO_Pin == GPIO_PIN_4)
	{
		if (current_time - last_time3 > 200) // Chống dội 200ms
		{
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET)
			{
				led_no4 = !led_no4;
				CAN_Tx();

			}
			last_time3 = current_time;
		}
	}
	// PB5
	if (GPIO_Pin == GPIO_PIN_5)
	{
		if (current_time - last_time4 > 200) // Chống dội 200ms
		{
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == GPIO_PIN_SET)
			{
				led_no5 = !led_no5;
				CAN_Tx();

			}
			last_time4 = current_time;
		}
	}
	// PB6
	if (GPIO_Pin == GPIO_PIN_6)
	{
		if (current_time - last_time5 > 200) // Chống dội 200ms
		{
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET)
			{
				led_no2 = !led_no2;
				CAN_Tx();

			}
			last_time5 = current_time;
		}
	}
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
	CAN_TxHeaderTypeDef TxHeader  = {0};

	uint32_t TxMailbox = 0;

	uint8_t message = 0;

	TxHeader.DLC = 5;
	TxHeader.StdId = 0x300;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;


	message = led_no1 | ( led_no2 << 1 ) |  ( led_no3 << 2 ) | ( led_no4 << 3 ) | ( led_no5 << 4 );

	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);

	if ( 	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message, &TxMailbox) != HAL_OK )
	{
		Error_handler();
	}
}
///
///
///
void CAN_TxADC(void)
{
	CAN_TxHeaderTypeDef TxHeader  = {0};

	uint32_t TxMailbox = 0;

	uint8_t message = 0;

	TxHeader.DLC = 1;
	TxHeader.StdId = 0x200;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;


	message = (uint8_t)speed;

	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);

	if ( 	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message, &TxMailbox) != HAL_OK )
	{
		Error_handler();
	}

}
void CAN_TxADC1(void)
{
	CAN_TxHeaderTypeDef TxHeader  = {0};

	uint32_t TxMailbox = 0;

	uint8_t message = 0;

	TxHeader.DLC = 1;
	TxHeader.StdId = 0x100;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;


	message = (uint8_t)fuel;

	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);

	if ( 	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &message, &TxMailbox) != HAL_OK )
	{
		Error_handler();
	}

}
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
	//printmsg("Message Transmitted: M0 \r\n");
}

/**
 * @brief  Transmission Mailbox 1 complete callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	//printmsg("Message Transmitted: M1 \r\n");
}

/**
 * @brief  Transmission Mailbox 2 complete callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	//printmsg("Message Transmitted: M2 \r\n");
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

	uint8_t rcvd_msg[8];
	// wait until at least one message in to the rx fifo0

	if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHead, rcvd_msg) != HAL_OK )
	{
		Error_handler();
	}

	if (RxHead.StdId == 0x651 && RxHead.RTR == CAN_RTR_DATA)
	{
		// its a reply data frame by n2 to n1
		printmsg("Replied Received from n2 - %#X\r\n",rcvd_msg[0] << 8 | rcvd_msg[1]);
		led_pa5_state = 1;
		 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
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
// timer 2 for 100ms
void TIMER2_Init(void)
{
	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Prescaler = 5600 - 1;
	htimer2.Init.Period = 1000 - 1;
	if (HAL_TIM_Base_Init(&htimer2) != HAL_OK)
	{
		Error_handler();
	}
}
//
//
//
uint8_t currentChannel = 0; // 1 = speed, 4 = rpm

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        ADC_ChannelConfTypeDef sConfig = {0};

        // Cấu hình "nóng" sang Channel 1
        sConfig.Channel = ADC_CHANNEL_1;
        sConfig.Rank = ADC_REGULAR_RANK_1; // Luôn dùng Rank 1 vì NbrOfConversion = 1
        sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);

        currentChannel = 1;
        HAL_ADC_Start_IT(&hadc1);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    uint32_t val = HAL_ADC_GetValue(hadc);

    if (currentChannel == 1) {
    	speed = (val * 240) / 4095;


    // cấu hình lại ADC sang Channel 4
        ADC_ChannelConfTypeDef sConfig = {0};
        sConfig.Channel = ADC_CHANNEL_4;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);

        currentChannel = 4;
        HAL_ADC_Start_IT(&hadc1); // Kích hoạt đọc biến trở 2
    }
    else if (currentChannel == 4) {
    	fuel = (val * 100) / 4095;
       // printmsg("Speed=%d|Fuel=%d\r\n", speed, fuel);

        // Gửi lên BUS CAN
            CAN_TxADC();
            CAN_TxADC1();

        currentChannel = 0;
    }
}


//
//
// timer 3
void TIMER3_Init(void)
{
	htimer3.Instance = TIM3;
	htimer3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer3.Init.Prescaler = 5600 - 1;
	htimer3.Init.Period = 1000 - 1;
	if (HAL_TIM_Base_Init(&htimer3) != HAL_OK)
	{
		Error_handler();
	}
}
//
//
/**
 * @brief  Rx Transfer completed callbacks.
 * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module.
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

}
//
//
//
void CAN_Tx_Uart(uint8_t message)
{

}
//
//
//
/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */
	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

