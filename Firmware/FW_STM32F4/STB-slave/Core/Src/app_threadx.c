/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"

#include "serial_to_GUI_Task.h"
#include "powerstep_Task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define THREAD_STACK_SIZE 1024
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
#ifdef GREEN_LED_CONNECTED
uint8_t led_stack[THREAD_STACK_SIZE];
TX_THREAD thread_ptr;
#endif
uint8_t serial_to_GUI_stack[THREAD_STACK_SIZE];
TX_THREAD serial_to_GUI_thread_ptr;

uint8_t powerstep_stack[THREAD_STACK_SIZE];
TX_THREAD powerstep_thread_ptr;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
#ifdef GREEN_LED_CONNECTED
VOID blink_led(ULONG delay);
#endif
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  (void)byte_pool;

  task_Serial_to_GUI_Init();
  initMotor();
  task_Powerstep01_Init();

  tx_thread_create(&serial_to_GUI_thread_ptr, "serial to gui", task_Serial_to_GUI, 20, serial_to_GUI_stack, THREAD_STACK_SIZE, 14, 14, 1, TX_AUTO_START);

  tx_thread_create(&powerstep_thread_ptr, "powerstep", task_Powerstep01, 30, powerstep_stack, THREAD_STACK_SIZE, 7, 7, 1, TX_AUTO_START);

#ifdef GREEN_LED_CONNECTED
  tx_thread_create(&thread_ptr, "blink led", blink_led, 100, led_stack, THREAD_STACK_SIZE, 10, 10, 1, TX_AUTO_START);
#endif
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
#ifdef GREEN_LED_CONNECTED
VOID blink_led(ULONG delay){
	while(1){
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		tx_thread_sleep(delay);
	}
}
#endif
/* USER CODE END 1 */
