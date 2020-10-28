/*
 * @Description: pid control source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-17 09:16:14
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-02 22:30:15
 */
#ifndef __PID_H__
#define __PID_H__

#include "main.h"

struct pid_param {
  float p;
  float i;
  float d;
  float input_max_err;

  float max_out;
  float inte_limit;
};

struct pid {
  struct pid_param param;

  float set;
  float get;

  float err;
  float last_err;

  float pout;
  float iout;
  float dout;
  float out;
};

float pid_calculate(struct pid *pid, float get, float set);
void pid_reset(struct pid *pid, float kp, float ki, float kd);
#endif
