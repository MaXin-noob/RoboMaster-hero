/*
 * @Description: calculate ramp header file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-02-26 12:21:53
 * @LastEditors: maxin
 * @LastEditTime: 2020-02-26 12:31:50
 */
#ifndef __RAMP_H__
#define __RAMP_H__

#include "main.h"

struct ramp
{
    int32_t count;
    int32_t scale;
    float out;
};

float ramp_calculate(struct ramp *ramp);
void ramp_init(struct ramp *ramp, int32_t scale);

#endif
