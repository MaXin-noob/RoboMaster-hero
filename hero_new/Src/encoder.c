/*
 * @Description: encoder calculate source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-02-13 11:50:55
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-03 01:25:46
 */
#include "encoder.h"

#include "sys.h"

void encoder_process(struct encoder *encoder, uint8_t *msg) {
  int i = 0;
  int32_t temp_sum = 0;
  encoder->last_raw_value = encoder->raw_value;
  encoder->raw_value = (msg[0] << 8) + msg[1];
  if ((encoder->startFlag) == 0) {
    if (encoder->ecd_bias >= 4096) {
      if (encoder->raw_value < (encoder->ecd_bias - 4096)) encoder->round_cnt++;
    } else {
      if (encoder->raw_value > (encoder->ecd_bias + 4096)) encoder->round_cnt--;
    }
    encoder->last_raw_value = encoder->raw_value;
    encoder->startFlag = 1;
  }
  encoder->diff = encoder->raw_value - encoder->last_raw_value;
  if (encoder->diff < -7500) {
    encoder->round_cnt++;
    encoder->ecd_raw_rate = encoder->diff + 8192;
  } else if (encoder->diff > 7500) {
    encoder->round_cnt--;
    encoder->ecd_raw_rate = encoder->diff - 8192;
  } else {
    encoder->ecd_raw_rate = encoder->diff;
  }
  encoder->ecd_value = encoder->raw_value + encoder->round_cnt * 8192;
  encoder->ecd_angle =
      (float)(encoder->raw_value - encoder->ecd_bias) * 360 / 8192 +
      encoder->round_cnt * 360;
  loop_float_constrain(&(encoder->ecd_angle), -180, 180);
  encoder->round_cnt = 0;
  encoder->ecd_rad = encoder->ecd_angle * (PI / 180);
  encoder->rate_buf[encoder->buf_count++] = encoder->ecd_raw_rate;
  if (encoder->buf_count == RATE_BUF_SIZE) {
    encoder->buf_count = 0;
  }
  for (i = 0; i < RATE_BUF_SIZE; i++) {
    temp_sum += encoder->rate_buf[i];
  }
  encoder->filter_rate = (int32_t)(temp_sum / RATE_BUF_SIZE);
}

void pluck_encoder_process(struct pluckEncoder *encoder, uint8_t *msg) {
  encoder->last_raw_value = encoder->raw_value;
  encoder->raw_value = (msg[0] << 8) + msg[1];
  encoder->filter_rate = (msg[2] << 8) + msg[3];
  encoder->diff = encoder->raw_value - encoder->last_raw_value;

  if (encoder->diff < -7000) {
    encoder->round_cnt++;
  } else if (encoder->diff > 7000) {
    encoder->round_cnt--;
  }
  if (encoder->startFlag == 0) {
    encoder->ecd_bias =
        (float)(encoder->raw_value) * 360 / 8192 + encoder->round_cnt * 360;
    encoder->startFlag = 1;
  }
  encoder->ecd_value = encoder->raw_value + encoder->round_cnt * 8192;
  encoder->ecd_angle = (float)(encoder->raw_value) * 360 / 8192 +
                       encoder->round_cnt * 360 - encoder->ecd_bias;
}
