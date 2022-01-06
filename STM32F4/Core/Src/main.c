/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "ILI9341_Touchscreen.h"

#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

#include "snow_tiger.h"

#define example 0
#define PBL1 0
#define PBL2 1
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LBtn GPIOG, GPIO_PIN_0
#define RBtn GPIOE, GPIO_PIN_7
#define PA0 GPIOA, GPIO_PIN_0

#define EN GPIOF, GPIO_PIN_13
#define LB GPIOF, GPIO_PIN_15
#define RB GPIOG, GPIO_PIN_1
#define F1 GPIOE, GPIO_PIN_8
#define F0 GPIOE, GPIO_PIN_10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

	uint8_t on = 1;
	uint8_t autoc = 0;
	uint8_t count = 1;

	uint32_t lastRB = 0;
	uint32_t longRB = 500;
	uint8_t readR;
	uint8_t RState = 1;
	uint8_t oldRState = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_RNG_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  ILI9341_Init();//initial driver setup to drive ili9341

  ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
  ILI9341_Draw_Image((const char*)pbb2, SCREEN_HORIZONTAL_1);
  ILI9341_Draw_Text("Welcome to", 10, 10, RED, 3, WHITE);
  ILI9341_Draw_Text("PBL2; A-3", 10, 40, RED, 3, WHITE);
  ILI9341_Draw_Text("PRESS!", 90, 105, RED, 4, WHITE);
  HAL_Delay(200);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  /***************************** DISPLAY LCD ***********************************/
	if(TP_Touchpad_Pressed())
	{
		uint16_t x_pos = 0;
		uint16_t y_pos = 0;

		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_SET);
		uint16_t position_array[2];
		if(TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK)
		{
			x_pos = position_array[1]; // invert y-axis to x-axis
			y_pos = 240 - position_array[0]; // invert vertical
			ILI9341_Draw_Image((const char*)pb2, SCREEN_HORIZONTAL_1);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
//			ILI9341_Draw_Text("This is PBL2 for you!", 10, 10, BLACK, 2, WHITE);

			ILI9341_Draw_Rectangle(10, 40, 150, 90, 0x4328); //for GLOVE-ON GREEN 0x4328(hover)
			ILI9341_Draw_Rectangle(160, 40, 150, 90, 0x7185); //for GLOVE-OFF RED 0x7185(hover)
			ILI9341_Draw_Rectangle(10, 145, 93, 85, 0x8427); //for auto-click YELLOW 0x8427(hover)
			ILI9341_Draw_Rectangle(113, 145, 93, 85, ORANGE); //for calibrate ORANGE 0xAC88(hover)
			ILI9341_Draw_Rectangle(216, 145, 94, 85, 0xA533); //for DPI 0xA533[light] 0x6B6D(hover)

			ILI9341_Draw_Text("ON", 65, 70, BLACK, 3, WHITE);
			ILI9341_Draw_Text("OFF", 200, 70, BLACK, 3, WHITE);
			ILI9341_Draw_Text("CALI", 135, 170, BLACK, 2, WHITE);
			ILI9341_Draw_Text("BRATE", 130, 190, BLACK, 2, WHITE);
			ILI9341_Draw_Text("AUTO-", 25, 170, BLACK, 2, WHITE);
			ILI9341_Draw_Text("CLICK", 20, 190, BLACK, 2, WHITE);

			ILI9341_Draw_Filled_Circle(x_pos, y_pos, 2, BLACK);//This is a dot!

			//++++++++++++++++++++FOR XY-COORDINATE HERE++++++++++++++++++++++
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
			char counter_buff[30];

			sprintf(counter_buff, "POS X:%.3d", x_pos);
			ILI9341_Draw_Text(counter_buff, 20, 10, BLACK, 2, WHITE);

			sprintf(counter_buff, "POS Y:%.3d", y_pos);
			ILI9341_Draw_Text(counter_buff, 150, 10, BLACK, 2, WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
			//++++++++++++++++++++FOR XY-COORDINATE END++++++++++++++++++++++

			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
			char counter[30];
			sprintf(counter, "%d", count);
			ILI9341_Draw_Text(counter, 245, 175, BLACK, 3, WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
		}

		ILI9341_Draw_Pixel(x_pos, y_pos, BLACK);

		//++++++++++++++++++++PLOTTING FOR SELECT-IN+++++++++++++++++++++

		if (x_pos > 10 && x_pos < 160 && y_pos > 40 && y_pos < 130){ on = 1; }
		else if (x_pos > 160 && x_pos < 310 && y_pos > 40 && y_pos < 130){ on = 0; }

		if (on == 1)
		{
			ILI9341_Draw_Rectangle(10, 40, 150, 90, GREEN);
			ILI9341_Draw_Text("ON!", 65, 70, BLACK, 3, WHITE);
		}
		else if (on == 0)
		{
			ILI9341_Draw_Rectangle(160, 40, 150, 90, RED);
			ILI9341_Draw_Text("OFF!", 200, 70, BLACK, 3, WHITE);
		}

		if (x_pos > 10 && x_pos < 103 && y_pos > 145 && y_pos < 230 && on == 1)
		{
//			if (autoc == 1){
//				autoc = 0;
//			}
//			else {
//				autoc = 1;
//			}
			EXTI0_IRQHandler();
		}

		if (autoc == 1)
		{
			ILI9341_Draw_Rectangle(10, 145, 93, 85, YELLOW);
			ILI9341_Draw_Text("AUTO-", 25, 170, BLACK, 2, WHITE);
			ILI9341_Draw_Text("CLICK!", 20, 190, BLACK, 2, WHITE);
		}
		else
		{
			ILI9341_Draw_Rectangle(10, 145, 93, 85, 0x8427);
			ILI9341_Draw_Text("AUTO-", 25, 170, BLACK, 2, WHITE);
			ILI9341_Draw_Text("CLICK", 20, 190, BLACK, 2, WHITE);
		}

		if (x_pos > 113 && x_pos < 206 && y_pos > 145 && y_pos < 230 && on == 1)
		{
			ILI9341_Draw_Rectangle(113, 145, 93, 85, 0xAC88);
			ILI9341_Draw_Text("CALI", 135, 170, BLACK, 2, WHITE);
			ILI9341_Draw_Text("BRATE!", 130, 190, BLACK, 2, WHITE);
			HAL_GPIO_WritePin(F1, 1);
			HAL_GPIO_WritePin(F0, 1);
			HAL_Delay(80);
			ILI9341_Draw_Rectangle(113, 145, 93, 85, ORANGE);
			ILI9341_Draw_Text("CALI", 135, 170, BLACK, 2, WHITE);
			ILI9341_Draw_Text("BRATE", 130, 190, BLACK, 2, WHITE);
			HAL_GPIO_WritePin(F1, 0);
			HAL_GPIO_WritePin(F0, 0);
		}

		if (x_pos > 216 && x_pos < 310 && y_pos > 145 && y_pos < 230 && on == 1)
		{
			ILI9341_Draw_Rectangle(216, 145, 94, 85, 0x6B6D);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);

			HAL_GPIO_WritePin(F1, 1);
			HAL_GPIO_WritePin(F0, 0);
			HAL_Delay(80);
			HAL_GPIO_WritePin(F1, 0);

			count++;
			if (count > 3){count = 1;}
			char counter[30];
			sprintf(counter, "%d", count);
			ILI9341_Draw_Text(counter, 245, 175, BLACK, 3, WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
			HAL_Delay(80);
			ILI9341_Draw_Rectangle(216, 145, 94, 85, 0xA533);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
			sprintf(counter, "%d", count);
			ILI9341_Draw_Text(counter, 245, 175, BLACK, 3, WHITE);
			ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
//			ILI9341_Draw_Text("DPI", 245, 170, BLACK, 2, WHITE);
		}

		//++++++++++++++++++++++++CHECK STATUS++++++++++++++++++++++++
//		char counter_buff[30];
//		sprintf(counter_buff, "\r\n%d %d\r\n", on, autoc);
//		HAL_UART_Transmit(&huart1, counter_buff, strlen(counter_buff), 200);
		//++++++++++++++++++++++++++END CHECK++++++++++++++++++++++++
		HAL_Delay(200);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
	}
	//------------ ENABLE GLOVES -------------//
	HAL_GPIO_WritePin(EN, on);

	//--------------------------------------------//
	//--------------- Right Button ---------------//
	// READ PIN PULL-UPs
	readR = HAL_GPIO_ReadPin(RBtn);
	// If Hold RB then Use FN Scroll
	if(readR != oldRState){
		lastRB = HAL_GetTick();
	}
	if((HAL_GetTick() - lastRB) > longRB){
		if(readR != RState){
			RState = readR;

			if(RState == 0){
				HAL_GPIO_WritePin(F0, !RState);
			}
		}
	}
	HAL_GPIO_WritePin(F0, !RState);
	oldRState = readR;

	// NORMAL RIGHT CLICK
	if(HAL_GPIO_ReadPin(RBtn) == 0){
		HAL_GPIO_WritePin(RB, 1);
	}
	else HAL_GPIO_WritePin(RB, 0);

	//---------------------------------------//

	//------------- Left Button -------------//
	if(HAL_GPIO_ReadPin(LBtn) == 0){         // INPUT PULL-UP
		HAL_GPIO_WritePin(LB, 1);            // LEFT CLICK
	}
	else HAL_GPIO_WritePin(LB, 0);
	//---------------------------------------//

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 120;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
