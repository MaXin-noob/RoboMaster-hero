/*
 * @Description: initialize board source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 14:30:47
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-03 22:02:45
 */
#include "board.h"
/**
 * @description: initialize board functions
 * @param[in]: void
 * @return: void
 */
void initializeBoard(void) {
  taskENTER_CRITICAL();

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_CAN1_Init();
  setFilters();
  setUsartDMA();

  taskEXIT_CRITICAL();
}