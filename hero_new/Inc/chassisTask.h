/*
 * @Description: 底盘控制头文件
 * @Version: 2.0
 * @Autor: 马昕
 * @Date: 2020-01-06 23:06:09
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-03 01:19:13
 */
#ifndef CHASSISTASK_H
#define CHASSISTASK_H
#include "main.h"
void loop_float_constrain(float *Input, float minValue, float maxValue);
#ifndef RADIAN_COEF
#define RADIAN_COEF 57.3f
#endif

#endif
