/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
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
#include "scheduler.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
void clear7SEG() {
    HAL_GPIO_WritePin(GPIOB, LED_A_Pin | LED_B_Pin | LED_C_Pin | LED_D_Pin | LED_E_Pin | LED_F_Pin | LED_G_Pin, GPIO_PIN_SET);
}


void display7SEG(int num) {
    clear7SEG();

    switch(num) {
        case 0:
            HAL_GPIO_WritePin(GPIOB, LED_A_Pin | LED_B_Pin | LED_C_Pin | LED_D_Pin | LED_E_Pin | LED_F_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOB, LED_B_Pin | LED_C_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOB, LED_A_Pin | LED_B_Pin | LED_D_Pin | LED_E_Pin | LED_G_Pin, GPIO_PIN_RESET);
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOB, LED_A_Pin | LED_B_Pin | LED_C_Pin | LED_D_Pin | LED_G_Pin, GPIO_PIN_RESET);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOB, LED_B_Pin | LED_C_Pin | LED_F_Pin | LED_G_Pin, GPIO_PIN_RESET);
            break;
        case 5:
            HAL_GPIO_WritePin(GPIOB, LED_A_Pin | LED_C_Pin | LED_D_Pin | LED_F_Pin | LED_G_Pin, GPIO_PIN_RESET);
            break;
        case 6:
            HAL_GPIO_WritePin(GPIOB, LED_A_Pin | LED_C_Pin | LED_D_Pin | LED_E_Pin | LED_F_Pin | LED_G_Pin, GPIO_PIN_RESET);
            break;
        case 7:
            HAL_GPIO_WritePin(GPIOB, LED_A_Pin | LED_B_Pin | LED_C_Pin, GPIO_PIN_RESET);
            break;
        case 8:
            HAL_GPIO_WritePin(GPIOB, LED_A_Pin | LED_B_Pin | LED_C_Pin | LED_D_Pin | LED_E_Pin | LED_F_Pin | LED_G_Pin, GPIO_PIN_RESET);
            break;
        case 9:
            HAL_GPIO_WritePin(GPIOB, LED_A_Pin | LED_B_Pin | LED_C_Pin | LED_D_Pin | LED_F_Pin | LED_G_Pin, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

//const int MAX_LED_MATRIX = 4;
int index_led = 0;
int led_buffer[4] = {1,2,3,4};

void update7SEG(int index) {
	switch(index) {
		case 0:
			display7SEG(led_buffer[0]);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET);
			break;
		case 1:
			display7SEG(led_buffer[1]);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET);
			break;
		case 2:
			display7SEG(led_buffer[2]);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9, GPIO_PIN_SET);
			break;
		case 3:
			display7SEG(led_buffer[3]);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8, GPIO_PIN_SET);
			break;
		default:
			break;
	}
}

const int MAX_LED_MATRIX = 8;
int index_led_matrix = 0;
int animate = 0;
// Matrix buffer to store LED patterns
//uint8_t matrix_buffer[8] = {0x1C, 0x22, 0x22, 0x22, 0x3E, 0x22, 0x22, 0x22};
uint8_t matrix_buffer[8] = {0x18,0x24,0x42,0x42,0x7E,0x42,0x42,0x42};
//uint8_t matrix_buffer[8] = {0xC3,0x81,0x3C,0x3C,0x3C,0x3C,0x00,0x00};
// Function to update LED matrix based on index
void updateLEDMatrix(int index, int animate) {
    // Disable all columns
    HAL_GPIO_WritePin(GPIOA, ENM0_Pin | ENM1_Pin | ENM2_Pin | ENM3_Pin | ENM4_Pin | ENM5_Pin | ENM6_Pin | ENM7_Pin, GPIO_PIN_SET);
    // Enable only the column specified by index
    int column = animate % 8;;
    switch(index) {
        case 0:
            HAL_GPIO_WritePin(GPIOA, ENM0_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (matrix_buffer[0] & 0x01 << column) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (matrix_buffer[1] & 0x01 << column) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (matrix_buffer[2] & 0x01 << column) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (matrix_buffer[3] & 0x01 << column) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (matrix_buffer[4] & 0x01 << column) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (matrix_buffer[5] & 0x01 << column) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (matrix_buffer[6] & 0x01 << column) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (matrix_buffer[7] & 0x01 << column) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOA, ENM1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (matrix_buffer[0] & 0x01<< column + 1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (matrix_buffer[1] & 0x01<< column + 1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (matrix_buffer[2] & 0x01<< column + 1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (matrix_buffer[3] & 0x01<< column + 1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (matrix_buffer[4] & 0x01<< column + 1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (matrix_buffer[5] & 0x01<< column + 1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (matrix_buffer[6] & 0x01<< column + 1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (matrix_buffer[7] & 0x01<< column + 1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOA, ENM2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (matrix_buffer[0] & 0x01<< column + 2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (matrix_buffer[1] & 0x01<< column + 2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (matrix_buffer[2] & 0x01<< column + 2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (matrix_buffer[3] & 0x01<< column + 2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (matrix_buffer[4] & 0x01<< column + 2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (matrix_buffer[5] & 0x01<< column + 2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (matrix_buffer[6] & 0x01<< column + 2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (matrix_buffer[7] & 0x01<< column + 2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOA, ENM3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (matrix_buffer[0] & 0x01<< column + 3) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (matrix_buffer[1] & 0x01<< column + 3) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (matrix_buffer[2] & 0x01<< column + 3) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (matrix_buffer[3] & 0x01<< column + 3) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (matrix_buffer[4] & 0x01<< column + 3) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (matrix_buffer[5] & 0x01<< column + 3) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (matrix_buffer[6] & 0x01<< column + 3) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (matrix_buffer[7] & 0x01<< column + 3) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOA, ENM4_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (matrix_buffer[0] & 0x01<< column + 4) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (matrix_buffer[1] & 0x01<< column + 4) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (matrix_buffer[2] & 0x01<< column + 4) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (matrix_buffer[3] & 0x01<< column + 4) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (matrix_buffer[4] & 0x01<< column + 4) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (matrix_buffer[5] & 0x01<< column + 4) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (matrix_buffer[6] & 0x01<< column + 4) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (matrix_buffer[7] & 0x01<< column + 4) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        case 5:
            HAL_GPIO_WritePin(GPIOA, ENM5_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (matrix_buffer[0] & 0x01<< column + 5) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (matrix_buffer[1] & 0x01<< column + 5) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (matrix_buffer[2] & 0x01<< column + 5) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (matrix_buffer[3] & 0x01<< column + 5) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (matrix_buffer[4] & 0x01<< column + 5) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (matrix_buffer[5] & 0x01<< column + 5) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (matrix_buffer[6] & 0x01<< column + 5) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (matrix_buffer[7] & 0x01<< column + 5) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        case 6:
            HAL_GPIO_WritePin(GPIOA, ENM6_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (matrix_buffer[0] & 0x01<< column + 6) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (matrix_buffer[1] & 0x01<< column + 6) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (matrix_buffer[2] & 0x01<< column + 6) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (matrix_buffer[3] & 0x01<< column + 6) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (matrix_buffer[4] & 0x01<< column + 6) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (matrix_buffer[5] & 0x01<< column + 6) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (matrix_buffer[6] & 0x01<< column + 6) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (matrix_buffer[7] & 0x01<< column + 6) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        case 7:
            HAL_GPIO_WritePin(GPIOA, ENM7_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, ROW0_Pin, (matrix_buffer[0] & 0x01<< column + 7) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW1_Pin, (matrix_buffer[1] & 0x01<< column + 7) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW2_Pin, (matrix_buffer[2] & 0x01<< column + 7) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW3_Pin, (matrix_buffer[3] & 0x01<< column + 7) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW4_Pin, (matrix_buffer[4] & 0x01<< column + 7) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW5_Pin, (matrix_buffer[5] & 0x01<< column + 7) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW6_Pin, (matrix_buffer[6] & 0x01<< column + 7) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOB, ROW7_Pin, (matrix_buffer[7] & 0x01<< column + 7) ? GPIO_PIN_RESET : GPIO_PIN_SET);
            break;
        default:
            // Handle error or do nothing
            break;
    }
}

void updateMatrixBuffer() {
	int temp = matrix_buffer[0];
	for(int i = 0; i < 7; i++) {
		matrix_buffer[i] = matrix_buffer[i+1];
	}
	matrix_buffer[7] = temp;
}

void LedBlinky() {
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}

void DoubleDot() {
	HAL_GPIO_TogglePin(LED_RED2_GPIO_Port, LED_RED2_Pin);
}

void updateTiming() {

}

void Led7Scan() {
	if(index_led > 3) {
		index_led = 0;
	}
	update7SEG(index_led++);
}

void LedMatrixAnimation() {
	updateLEDMatrix(index_led_matrix++,animate);

	if(index_led_matrix>7) {
			//			updateMatrixBuffer();
						index_led_matrix = 0;
						animate++;
			}
}

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT (& htim2 ) ;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	  SCH_Add_Task(LedBlinky,1,100);
	  SCH_Add_Task(DoubleDot,3,50);
	  SCH_Add_Task(Led7Scan,0,25);
	  SCH_Add_Task(LedMatrixAnimation,0,1);


  while (1)
  {
	  SCH_Dispatch_Tasks();
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  htim2.Init.Period = 9;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|LED_RED2_Pin|LED_RED_Pin
                          |PIN_6_Pin|PIN_7_Pin|PIN_8_Pin|PIN_9_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_A_Pin|LED_B_Pin|LED_C_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|LED_D_Pin|LED_E_Pin|LED_F_Pin
                          |LED_G_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin LED_RED2_Pin LED_RED_Pin
                           PIN_6_Pin PIN_7_Pin PIN_8_Pin PIN_9_Pin
                           ENM2_Pin ENM3_Pin ENM4_Pin ENM5_Pin
                           ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|LED_RED2_Pin|LED_RED_Pin
                          |PIN_6_Pin|PIN_7_Pin|PIN_8_Pin|PIN_9_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_A_Pin LED_B_Pin LED_C_Pin ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin LED_D_Pin LED_E_Pin LED_F_Pin
                           LED_G_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = LED_A_Pin|LED_B_Pin|LED_C_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|LED_D_Pin|LED_E_Pin|LED_F_Pin
                          |LED_G_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	SCH_Update();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
