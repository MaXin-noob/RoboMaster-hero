/*
 * @Description: usart configuration header file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 14:05:29
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-06 10:36:29
 */
#ifndef DRV_USART_H
#define DRV_USART_H
#include "main.h"
#define VERSION_LENGTH (16U)
#define VERSION_LENGTH_TX (14u)
struct version {
  uint8_t header;
  float pitch_angle;
  float yaw_angle;
  uint8_t is_armour;
  uint8_t distance;
  uint8_t is_shoot;
};
void UART_IRQHandler(UART_HandleTypeDef *huart);
void setUsartDMA(void);
uint8_t *getJudgeData(void);
#endif /*end DRV_USART_H*/