/*
 * @Description: set led color header file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 14:12:40
 * @LastEditors: maxin
 * @LastEditTime: 2020-07-28 17:24:11
 */
#ifndef DRV_LED_H
#define DRV_LED_H
#include "main.h"

typedef enum {
  OFF = 0,
  RED = 1,
  GREEN = 2,
  BLUE = 3,
  YELLOW = 4,
  PURPLE = 5,
  CYAN = 6,
  WHITE = 7,

} color_t;
color_t getFlashingColor(void);
void setLedColor(color_t color);
void setFlashingColor(color_t color);
#endif /*end DRV_LED_H*/