/*
 * @Description: 滤波算法头文件
 * @Version: 2.0
 * @Autor: 马昕
 * @Date: 2020-01-14 10:25:06
 * @LastEditors: maxin
 * @LastEditTime: 2020-07-28 12:35:53
 */
#ifndef FILTER_H
#define FILTER_H
#include "main.h"

typedef  struct __packed{
  float input;         //输入数据
  float out;           //滤波输出的数据
  float num;           //滤波参数
  float frame_period;  //滤波的时间间隔 单位 s
} first_order_filter;
extern first_order_filter chassis_cmd_vx;
extern first_order_filter chassis_cmd_vy;
void first_order_filter_cali(first_order_filter *first_order_filter_type,
                             float *input);
#if Recursive_average_filtering_method == 1
#define FILTER_N 12
#endif
#endif
