/*
 * @Description: PID control header file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-17 09:16:37
 * @LastEditors: maxin
 * @LastEditTime: 2020-07-28 12:09:19
 */

#include "pid.h"

/**
 * @description: limiting function
 * @param[in]: The value to be clipped
 * @param[in]: max value
 * @return: void
 */
void abs_limit(float *a, float ABS_MAX) {
  if (*a > ABS_MAX) *a = ABS_MAX;
  if (*a < -ABS_MAX) *a = -ABS_MAX;
}

/**
 * @description: reset PID
 * @param[in]:pid struct
 * @param[in]:kp
 * @param[in]:ki
 * @param[in]:kd
 * @return: void
 */
void pid_reset(struct pid *pid, float kp, float ki, float kd) {
  pid->param.p = kp;
  pid->param.i = ki;
  pid->param.d = kd;

  pid->pout = 0;
  pid->iout = 0;
  pid->dout = 0;
  pid->out = 0;
}
/**
 * @description: calculate PID
 * @param[in]:pid struct
 * @param[in]:true value
 * @param[in]:set value
 * @return: float
 */
float pid_calculate(struct pid *pid, float get, float set) {
  pid->last_err = pid->err;
  pid->get = get;
  pid->set = set;
  pid->err = set - get;
  if ((pid->param.input_max_err != 0) &&
      (fabs(pid->err) > pid->param.input_max_err))
    return 0;

  pid->pout = pid->param.p * pid->err;
  pid->iout += pid->param.i * pid->err;
  pid->dout = pid->param.d * (pid->err - pid->last_err);

  abs_limit(&(pid->iout), pid->param.inte_limit);
  pid->out = pid->pout + pid->iout + pid->dout;
  abs_limit(&(pid->out), pid->param.max_out);

  return pid->out;
}
