/*
 * @Description: 匿名发送文件
 * @Version: new
 * @Autor: 马昕
 * @Date: 2020-01-06 23:06:09
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-06 14:07:45
 */
#include "nimingTask.h"
static uint8_t NiMing[NIMINGLength];
static void Ni_Ming_send(int16_t a_x, int16_t a_y, int16_t a_z, int16_t g_x,
                         int16_t g_y, int16_t g_z, int16_t m_x, int16_t m_y,
                         int16_t m_z, int32_t bar);

void nimingHandler(void const *argument) {
  while (true) {
    Ni_Ming_send(gimbal.gimbal_pid.yaw_positio_PID->set,
                 gimbal.gimbal_pid.yaw_positio_PID->get,
                 gimbal.gimbal_pid.yaw_speed_PID->set,
                 gimbal.gimbal_pid.yaw_speed_PID->get, 1, 1, 1, 1, 1, 1);
    osDelay(4);
  }
}
static void Ni_Ming_send(int16_t a_x, int16_t a_y, int16_t a_z, int16_t g_x,
                         int16_t g_y, int16_t g_z, int16_t m_x, int16_t m_y,
                         int16_t m_z, int32_t bar) {
  uint8_t _cnt = 0, sum = 0, add = 0, i = 0;
  int _temp;
  NiMing[_cnt++] = 0xAA;
  NiMing[_cnt++] = 0xFF;
  NiMing[_cnt++] = 0xF1;
  NiMing[_cnt++] = 0;
  _temp = a_x;
  NiMing[_cnt++] = BYTE0(_temp);
  NiMing[_cnt++] = BYTE1(_temp);
  _temp = a_y;
  NiMing[_cnt++] = BYTE0(_temp);
  NiMing[_cnt++] = BYTE1(_temp);
  _temp = a_z;
  NiMing[_cnt++] = BYTE0(_temp);
  NiMing[_cnt++] = BYTE1(_temp);
  _temp = g_x;
  NiMing[_cnt++] = BYTE0(_temp);
  NiMing[_cnt++] = BYTE1(_temp);
  _temp = g_y;
  NiMing[_cnt++] = BYTE0(_temp);
  NiMing[_cnt++] = BYTE1(_temp);
  _temp = g_z;
  NiMing[_cnt++] = BYTE0(_temp);
  NiMing[_cnt++] = BYTE1(_temp);
  _temp = m_x;
  NiMing[_cnt++] = BYTE0(_temp);
  NiMing[_cnt++] = BYTE1(_temp);
  _temp = m_y;
  NiMing[_cnt++] = BYTE0(_temp);
  NiMing[_cnt++] = BYTE1(_temp);
  _temp = m_z;
  NiMing[_cnt++] = BYTE0(_temp);
  NiMing[_cnt++] = BYTE1(_temp);
  NiMing[3] = _cnt - 4;
  for (i = 0; (i < NiMing[3] + 4); i++) {
    sum += NiMing[i];
    add += sum;
  }
  NiMing[_cnt++] = sum;
  NiMing[_cnt++] = add;
  HAL_UART_Transmit(&huart4, (uint8_t *)&NiMing, _cnt, 0xfff);
}