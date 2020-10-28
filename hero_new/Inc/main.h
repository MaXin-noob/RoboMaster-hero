/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "RGBTask.h"
#include "arm_math.h"
#include "board.h"
#include "can.h"
#include "chassisTask.h"
#include "cmsis_os.h"
#include "crc.h"
#include "dma.h"
#include "drv_can.h"
#include "drv_dr16.h"
#include "drv_led.h"
#include "drv_usart.h"
#include "encoder.h"
#include "filter.h"
#include "gimbalTask.h"
#include "gpio.h"
#include "judgeTask.h"
#include "math.h"
#include "nimingTask.h"
#include "pid.h"
#include "queue.h"
#include "ramp.h"
#include "shootTask.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "sys.h"
#include "task.h"
#include "usart.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
void SystemClock_Config(void);
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VERSION_TX_Pin GPIO_PIN_10
#define VERSION_TX_GPIO_Port GPIOB
#define VERSION_RX_Pin GPIO_PIN_11
#define VERSION_RX_GPIO_Port GPIOB
#define DBUS_RX_Pin GPIO_PIN_10
#define DBUS_RX_GPIO_Port GPIOA
#define GPIO_LED_BLUE_Pin GPIO_PIN_8
#define GPIO_LED_BLUE_GPIO_Port GPIOB
#define GPIO_LED_GREEN_Pin GPIO_PIN_9
#define GPIO_LED_GREEN_GPIO_Port GPIOB
#define GPIO_LED_RED_Pin GPIO_PIN_0
#define GPIO_LED_RED_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
