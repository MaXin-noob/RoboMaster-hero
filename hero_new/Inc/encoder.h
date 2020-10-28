/*
 * @Description: encoder calculate header file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-02-13 11:50:16
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-02 16:19:28
 */

#ifndef ENCODER_H
#define ENCODER_H
#include "main.h"
#include "sys.h"
#define RATE_BUF_SIZE 6

struct encoder {
  int32_t ecd_bias;        //初始编码器值
  int32_t raw_value;       //编码器不经处理的原始值
  int32_t last_raw_value;  //上一次的编码器原始值
  int32_t ecd_value;       //经过处理后连续的编码器值
  int32_t diff;            //两次编码器之间的差值
  int32_t temp_count;      //计数用
  uint8_t buf_count;       //滤波更新buf用
  uint8_t startFlag;       //初始化标志位
  int32_t ecd_raw_rate;    //通过编码器计算得到的速度原始值
  int32_t rate_buf[RATE_BUF_SIZE];  // buf，for filter
  int32_t round_cnt;                //圈数
  int16_t filter_rate;                //速度
  float ecd_angle;                  //角度
  float ecd_rad;                    //角度转弧度  rad
};
struct pluckEncoder {
  int32_t ecd_bias;        //初始编码器值
  uint8_t startFlag;       //初始化标志位
  int16_t raw_value;       //编码器不经处理的原始值
  int32_t last_raw_value;  //上一次的编码器原始值
  int32_t ecd_value;       //经过处理后连续的编码器值
  int32_t diff;            //两次编码器之间的差值
  int32_t temp_count;      //计数用
  int32_t round_cnt;       //圈数
  float ecd_angle;         //角度
  int16_t filter_rate;     //速度
};
void encoder_process(struct encoder *encoder, uint8_t *msg);
void pluck_encoder_process(struct pluckEncoder *encoder, uint8_t *msg);
#endif /*end ENCODER_H*/