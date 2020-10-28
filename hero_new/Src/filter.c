/*
 * @Description: filters source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-17 09:16:37
 * @LastEditors: maxin
 * @LastEditTime: 2020-07-28 13:44:21
 */

#include "filter.h"

first_order_filter chassis_cmd_vx = {
    .num = 0.1666666667f,
    .frame_period = 0.010f,
};
first_order_filter chassis_cmd_vy = {
    .num = 0.3333333333f,
    .frame_period = 0.010f,
};
void first_order_filter_cali(first_order_filter *first_order_filter_type,
                             float *input) {
  first_order_filter_type->input = *input;
  first_order_filter_type->out = first_order_filter_type->num /
                                     (first_order_filter_type->num +
                                      first_order_filter_type->frame_period) *
                                     first_order_filter_type->out +
                                 first_order_filter_type->frame_period /
                                     (first_order_filter_type->num +
                                      first_order_filter_type->frame_period) *
                                     first_order_filter_type->input;
  *input = first_order_filter_type->out;
}

#if Recursive_average_filtering_method == 1
float Recursive_average_filter(float newValue) {
  static float filter_buf[FILTER_N + 1];
  int i;
  float filter_sum = 0;
  filter_buf[FILTER_N] = newValue;
  for (i = 0; i < FILTER_N; i++) {
    filter_buf[i] = filter_buf[i + 1];
    filter_sum += filter_buf[i];
  }
  return (float)(filter_sum / FILTER_N);
}
#endif
