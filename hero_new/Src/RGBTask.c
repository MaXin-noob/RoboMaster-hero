/*
 * @Description: LED灯控制文件
 * @Version: 2.0
 * @Autor: 马昕
 * @Date: 2020-01-06 23:06:09
 * @LastEditors: maxin
 * @LastEditTime: 2020-07-30 23:07:56
 */
#include "RGBTask.h"

#include "main.h"

static color_t color_flag;
/**
 * @description: led control
 * @param[in]: void const *argument
 * @return: void
 */
void rgbHandler(void const *argument) {
  while (1) {
    color_flag = getFlashingColor();
    setLedColor(color_flag);
    osDelay(500);
    color_flag = OFF;
    setLedColor(color_flag);
    osDelay(500);
  }
}
