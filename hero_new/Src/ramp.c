/*
 * @Description: calculate ramp source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-02-26 12:20:46
 * @LastEditors: maxin
 * @LastEditTime: 2020-02-26 12:31:26
 */


#include "ramp.h"
/**
 * @description: calculate ramp
 * @param[in]:ramp struct
 * @return: float
 */
float ramp_calculate(struct ramp *ramp)
{
    if (ramp->scale <= 0)
        return 0;

    if (ramp->count++ >= ramp->scale)
        ramp->count = ramp->scale;

    ramp->out = ramp->count / ((float)ramp->scale);
    return ramp->out;
}

/**
 * @description: initialize ramp
 * @param[in]:ramp struct
 * @param[in]:ramp scale
 * @return: void
 */
void ramp_init(struct ramp *ramp, int32_t scale)
{
    ramp->count = 0;
    ramp->scale = scale;
}
