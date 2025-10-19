/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <main.hpp>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <button.hpp>
#include <delay_micros.hpp>
#include <tm1637.hpp>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
Button button0 = Button(BUTTON0_GPIO_Port, BUTTON0_RED_Pin, BUTTON0_GREEN_Pin,
		BUTTON0_BLUE_Pin, BUTTON0_Pin);
Button button1 = Button(BUTTON1_GPIO_Port, BUTTON1_RED_Pin, BUTTON1_GREEN_Pin,
		BUTTON1_BLUE_Pin, BUTTON1_Pin);
TM1637 disp0 = TM1637(DISP0_CLK_GPIO_Port, DISP0_DIO_Pin, DISP0_CLK_Pin);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);

  disp0.Reset();
  disp0.Display(0x0, 1);
  disp0.Display(0x1, 2);
  disp0.Display(0x2, 3);
  disp0.Display(0x3, 4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if (button0.isShortPressed) {
		HAL_GPIO_TogglePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin);
		button0.isShortPressed = false;
		disp0.SetBlinkMode(NO_BLINK);
//		disp0.Display(0x2, 1);
	}

	if (button1.isShortPressed) {
		HAL_GPIO_TogglePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin);
		button1.isShortPressed = false;
		disp0.SetBlinkMode(NO_BLINK);
//		disp0.Display(0x0, 1);
	}

	if (button0.isPressed && !button1.isPressed) {
		button0.SetLed(RED, NORMAL);
		button1.SetLed(GREEN, OFF);
		disp0.SetBlinkMode(BLINK_LOW);
//		disp0.Display(0x2, 0);
	} else if (button1.isPressed && !button0.isPressed) {
		button0.SetLed(RED, OFF);
		button1.SetLed(GREEN, NORMAL);
		disp0.SetBlinkMode(BLINK_HIGH);
//		disp0.Display(0x0, 0);
	} else if (button0.isPressed && button1.isPressed) {
		button0.SetLed(BLUE, BLINKING);
		button1.SetLed(BLUE, BLINKING);
		disp0.SetBlinkMode(BLINK);
	} else {
		button0.SetLed(BLUE, OFF);
		button1.SetLed(BLUE, OFF);
		disp0.SetBlinkMode(NO_BLINK);
	}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 1;
  DateToUpdate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 499;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, BUTTON0_RED_Pin|BUTTON0_GREEN_Pin|BUTTON0_BLUE_Pin|BUTTON1_RED_Pin
                          |BUTTON1_GREEN_Pin|BUTTON1_BLUE_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BOARD_LED_GPIO_Port, BOARD_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DISP0_DIO_Pin|DISP0_CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BUTTON0_RED_Pin BUTTON0_GREEN_Pin BUTTON0_BLUE_Pin BUTTON1_RED_Pin
                           BUTTON1_GREEN_Pin BUTTON1_BLUE_Pin */
  GPIO_InitStruct.Pin = BUTTON0_RED_Pin|BUTTON0_GREEN_Pin|BUTTON0_BLUE_Pin|BUTTON1_RED_Pin
                          |BUTTON1_GREEN_Pin|BUTTON1_BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON0_Pin BUTTON1_Pin */
  GPIO_InitStruct.Pin = BUTTON0_Pin|BUTTON1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BOARD_LED_Pin */
  GPIO_InitStruct.Pin = BOARD_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BOARD_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP0_DIO_Pin DISP0_CLK_Pin */
  GPIO_InitStruct.Pin = DISP0_DIO_Pin|DISP0_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// button0
void EXTI3_Callback() {
	__disable_irq();
	// кнопка нажата и таймер не запущен - запускаем таймер
	if (HAL_GPIO_ReadPin(BUTTON0_GPIO_Port, BUTTON0_Pin) == GPIO_PIN_RESET
			&& !(TIM3->CR1 & TIM_CR1_CEN)) {
		TIM3->CNT = 0x0;
		HAL_TIM_Base_Start_IT(&htim3);
	}
	// кнопка отжата ...
	if (HAL_GPIO_ReadPin(BUTTON0_GPIO_Port, BUTTON0_Pin) == GPIO_PIN_SET) {
		/* ... и таймер не переполнился - короткое нажатие
		 * ставим флаг isShortPressed и останавливаем таймер
		 */
		if (!button0.isPressed && (TIM3->CR1 & TIM_CR1_CEN)) {
			HAL_TIM_Base_Stop_IT(&htim3);
			TIM3->CNT = 0x0;
			button0.isShortPressed = true;
		}
		/* ... и таймер переполнился и выключился - долгое нажатие
		 * флаг isPressed уже выставлен в TIM3_PeriodElapsedCallback
		 * здесь его убираем (кнопку уже отпустили)
		 */
		else {
			button0.isPressed = false;
			TIM3->CNT = 0x0;
		}
	}
	__enable_irq();
}

// button1
void EXTI9_5_Callback() {
	__disable_irq();
	// кнопка нажата и таймер не запущен - запускаем таймер
	if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == GPIO_PIN_RESET
			&& !(TIM3->CR1 & TIM_CR1_CEN)) {
		TIM3->CNT = 0x0;
		HAL_TIM_Base_Start_IT(&htim3);
	}
	// кнопка отжата ...
	if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == GPIO_PIN_SET) {
		/* ... и таймер не переполнился - короткое нажатие
		 * ставим флаг isShortPressed и останавливаем таймер
		 */
		if (!button1.isPressed && (TIM3->CR1 & TIM_CR1_CEN)) {
			HAL_TIM_Base_Stop_IT(&htim3);
			TIM3->CNT = 0x0;
			button1.isShortPressed = true;
		}
		/* ... и таймер переполнился и выключился - долгое нажатие
		 * флаг isPressed уже выставлен в TIM3_PeriodElapsedCallback
		 * здесь его убираем (кнопку уже отпустили)
		 */
		else {
			button1.isPressed = false;
			TIM3->CNT = 0x0;
		}
	}
	__enable_irq();
}

// TIM2 отвечает за синхронное мигание всех элементов
void TIM2_PeriodElapsedCallback() {
	__disable_irq();
	if (button0.isBlinking) button0.ToggleLed();
	if (button1.isBlinking) button1.ToggleLed();
	if (disp0.isBlinking) disp0.Toggle();
	__enable_irq();
}

// TIM3 отвечает за короткое/длинное нажатие кнопок
void TIM3_PeriodElapsedCallback() {
	__disable_irq();
	/* таймер успел переполниться (значит кнопка не отжата) - ставим флаг isPressed
	 * и останавливаем таймер
	 */
	HAL_TIM_Base_Stop_IT(&htim3);
	if (HAL_GPIO_ReadPin(BUTTON0_GPIO_Port, BUTTON0_Pin) == GPIO_PIN_RESET) {
		button0.isPressed = true;
	}
	if (HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin) == GPIO_PIN_RESET) {
		button1.isPressed = true;
	}
	TIM3->CNT = 0x0;
	__enable_irq();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
