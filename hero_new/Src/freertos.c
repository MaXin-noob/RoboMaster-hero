/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId rgbTaskHandle;
osThreadId gimbalTaskHandle;
osThreadId chassisTaskHandle;
osThreadId shootTaskHandle;
osThreadId judgeSolutionHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
#if FORBID_NIMING == 0
void nimingHandler(void const *argument);
osThreadId nimingTaskHandle;
#endif /*end FORBID_NIMING*/
/* USER CODE END FunctionPrototypes */

void rgbHandler(void const * argument);
void gimbalHandler(void const * argument);
void chassisHandler(void const * argument);
void shootHandler(void const * argument);
void judgeHandler(void const * argument);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of rgbTask */
  osThreadDef(rgbTask, rgbHandler, osPriorityLow, 0, 128);
  rgbTaskHandle = osThreadCreate(osThread(rgbTask), NULL);

  /* definition and creation of gimbalTask */
  osThreadDef(gimbalTask, gimbalHandler, osPriorityRealtime, 0, 512);
  gimbalTaskHandle = osThreadCreate(osThread(gimbalTask), NULL);

  /* definition and creation of chassisTask */
  osThreadDef(chassisTask, chassisHandler, osPriorityRealtime, 0, 512);
  chassisTaskHandle = osThreadCreate(osThread(chassisTask), NULL);

  /* definition and creation of shootTask */
  osThreadDef(shootTask, shootHandler, osPriorityHigh, 0, 512);
  shootTaskHandle = osThreadCreate(osThread(shootTask), NULL);

  /* definition and creation of judgeSolution */
  osThreadDef(judgeSolution, judgeHandler, osPriorityNormal, 0, 512);
  judgeSolutionHandle = osThreadCreate(osThread(judgeSolution), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
#if FORBID_NIMING == 0
  osThreadDef(nimingTask, nimingHandler, osPriorityNormal, 0, 128);
  nimingTaskHandle = osThreadCreate(osThread(nimingTask), NULL);
#endif /*end FORBID_NIMING*/
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_rgbHandler */
/**
  * @brief  Function implementing the rgbTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_rgbHandler */
__weak void rgbHandler(void const * argument)
{
  /* USER CODE BEGIN rgbHandler */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
  /* USER CODE END rgbHandler */
}

/* USER CODE BEGIN Header_gimbalHandler */
/**
* @brief Function implementing the gimbalTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_gimbalHandler */
__weak void gimbalHandler(void const * argument)
{
  /* USER CODE BEGIN gimbalHandler */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
  /* USER CODE END gimbalHandler */
}

/* USER CODE BEGIN Header_chassisHandler */
/**
* @brief Function implementing the chassisTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_chassisHandler */
__weak void chassisHandler(void const * argument)
{
  /* USER CODE BEGIN chassisHandler */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
  /* USER CODE END chassisHandler */
}

/* USER CODE BEGIN Header_shootHandler */
/**
* @brief Function implementing the shootTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_shootHandler */
__weak void shootHandler(void const * argument)
{
  /* USER CODE BEGIN shootHandler */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
  /* USER CODE END shootHandler */
}

/* USER CODE BEGIN Header_judgeHandler */
/**
* @brief Function implementing the judgeSolution thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_judgeHandler */
__weak void judgeHandler(void const * argument)
{
  /* USER CODE BEGIN judgeHandler */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END judgeHandler */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
* @brief Function implementing the nimingTask thread.
* @param argument: Not used
* @retval None
*/
#if FORBID_NIMING == 0
__weak void nimingHandler(void const * argument)
{
  for(;;)
  {
    osDelay(1);
  }
}
#endif /**/
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
