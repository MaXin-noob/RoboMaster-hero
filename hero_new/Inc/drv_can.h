/*
 * @Description: CAN configuration header file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 13:52:54
 * @LastEditors: maxin
 * @LastEditTime: 2020-07-26 23:17:47
 */
#ifndef DRV_CAN_H
#define DRV_CAN_H
#include "main.h"

void sendCanData(CAN_HandleTypeDef *_hcan, int stdid, int num1, int num2,
                 int num3, int num4);
void setFilters(void);
#endif /*end DRV_CAN_H*/