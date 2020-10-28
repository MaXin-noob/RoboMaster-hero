/*
 * @Description: usart configuration source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 14:05:45
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-06 10:46:19
 */
#include "drv_usart.h"

static uint8_t version_data_buff[VERSION_LENGTH] = {0};
static uint8_t dr16_uart_rx_buff[DR16_DATA_LEN] = {0};
static uint8_t JudgeDataBuffer[JudgeBufferLength] = {0};
static uint8_t versionSend_tx_buffer[VERSION_LENGTH_TX] = {0};

/**
 * @description: usart DMA set function
 * @param[in]: void
 * @return: void
 */
void setUsartDMA(void) {
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart1, dr16_uart_rx_buff, DR16_DATA_LEN);

  __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart3, JudgeDataBuffer, JudgeBufferLength);
}

/**
 * @description: get judge data buffer
 * @param[in]: void
 * @return: void
 */
uint8_t *getJudgeData(void) { return JudgeDataBuffer; }
/**
 * @description: version data calculate
 * @param[in]: void
 * @return: void
 */
static void calculateVersionData(void) {
  uint8_t _cnt = 0;
  gimbal.version_data->header = version_data_buff[_cnt++];
  if (gimbal.version_data->header == 0xff) {
    unchar2float.UN[0] = version_data_buff[_cnt++];
    unchar2float.UN[1] = version_data_buff[_cnt++];
    unchar2float.UN[2] = version_data_buff[_cnt++];
    unchar2float.UN[3] = version_data_buff[_cnt++];
    if (version_data_buff[_cnt++] == 1)
      gimbal.version_data->pitch_angle = unchar2float.F;
    else
      gimbal.version_data->pitch_angle = -unchar2float.F;
    unchar2float.UN[0] = version_data_buff[_cnt++];
    unchar2float.UN[1] = version_data_buff[_cnt++];
    unchar2float.UN[2] = version_data_buff[_cnt++];
    unchar2float.UN[3] = version_data_buff[_cnt++];
    if (version_data_buff[_cnt++] == 1)
      gimbal.version_data->yaw_angle = unchar2float.F;
    else
      gimbal.version_data->yaw_angle = -unchar2float.F;
    gimbal.version_data->is_armour = version_data_buff[_cnt++];
    gimbal.version_data->distance = version_data_buff[_cnt++];
    gimbal.version_data->is_shoot = version_data_buff[_cnt++];
  }
}
/**
 * @description: send version data
 * @param[in]: void
 * @return: void
 */
/****** 视觉发数 ******
0       0xaa 检验位
1-4     pitch轴角度
5       pitch轴正负
6-9     yaw轴角度
10      yaw轴正负
11      等级
12      是否打符
13      0xbb 校验位
**********************/
void sendVisuslData(void) {
  versionSend_tx_buffer[0] = 0xaa;
  unchar2float.F =
      -(gimbal.gyor_yaw_data.ecd_Angle - gimbal.gyor_yaw_data.GyroAngle_Bias);
  versionSend_tx_buffer[1] = unchar2float.UN[0];
  versionSend_tx_buffer[2] = unchar2float.UN[1];
  versionSend_tx_buffer[3] = unchar2float.UN[2];
  versionSend_tx_buffer[4] = unchar2float.UN[3];
  if (-(gimbal.gyor_yaw_data.ecd_Angle - gimbal.gyor_yaw_data.GyroAngle_Bias) >
      0) {
    versionSend_tx_buffer[5] = 1;
  } else
    versionSend_tx_buffer[5] = 0;
  unchar2float.F = gimbal.gimbal_encoder.pitch_encoder->ecd_angle;
  versionSend_tx_buffer[6] = unchar2float.UN[0];
  versionSend_tx_buffer[7] = unchar2float.UN[1];
  versionSend_tx_buffer[8] = unchar2float.UN[2];
  versionSend_tx_buffer[9] = unchar2float.UN[3];
  if (gimbal.gimbal_encoder.pitch_encoder->ecd_angle > 0) {
    versionSend_tx_buffer[10] = 1;
  } else
    versionSend_tx_buffer[10] = 0;
  versionSend_tx_buffer[11] = JudgementData.game_robot_state_t.robot_level;
  versionSend_tx_buffer[12] = 0;
  versionSend_tx_buffer[13] = 0xbb;
}
/**
 * @description: usart IRQHandler function
 * @param[in]: whitch usart handle
 * @return: void
 */
void UART_IRQHandler(UART_HandleTypeDef *huart) {
  if (huart == &huart1) {
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) == 1) {
      HAL_UART_DMAStop(&huart1);
      HAL_UART_Receive_DMA(&huart1, dr16_uart_rx_buff, DR16_DATA_LEN);
      HAL_UART_IRQHandler(&huart1);
      setDbusData(dr16_uart_rx_buff);
    }
  }
  if (huart == &huart3) {
    if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) == 1) {
      HAL_UART_DMAStop(&huart3);
      HAL_UART_Receive_DMA(&huart3, JudgeDataBuffer, JudgeBufferLength);
      HAL_UART_IRQHandler(&huart3);
      JudgementData.judge_ok = true;
    }
  }
  return;
}
