/*
 * @Description: CAN configuration source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 13:51:38
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-03 22:06:33
 */
#include "drv_can.h"

#include "encoder.h"

/**
 * @description: set can filters
 * @param[in]:void
 * @return: void
 */
void setFilters(void) {
  CAN_FilterTypeDef can_filter_st;
  can_filter_st.SlaveStartFilterBank = 14;
  can_filter_st.FilterBank = 14;
  can_filter_st.FilterActivation = ENABLE;
  can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
  can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
  can_filter_st.FilterIdHigh = 0x0000;
  can_filter_st.FilterIdLow = 0x0000;
  can_filter_st.FilterMaskIdHigh = 0x0000;
  can_filter_st.FilterMaskIdLow = 0x0000;
  can_filter_st.FilterBank = 0;
  can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
  HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
  HAL_CAN_Start(&hcan1);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

  can_filter_st.SlaveStartFilterBank = 14;
  can_filter_st.FilterBank = 14;
  HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
  HAL_CAN_Start(&hcan2);
  HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);

  return;
}

/**
 * @description: send can message
 * @param[in]: whitch can handle
 * @param[in]:stdid
 * @param[in]:Motor1 data
 * @param[in]:Motor2 data
 * @param[in]:Motor3 data
 * @param[in]:Motor4 data
 * @return: void
 */
void sendCanData(CAN_HandleTypeDef *_hcan, int stdid, int num1, int num2,
                 int num3, int num4) {
  CAN_TxHeaderTypeDef sendmessage;
  uint8_t Data[8];
  uint32_t send_mail_box;
  sendmessage.DLC = 0x08;
  sendmessage.IDE = CAN_ID_STD;
  sendmessage.RTR = CAN_RTR_DATA;
  sendmessage.StdId = stdid;
  Data[0] = ((num1) >> 8);
  Data[1] = (num1);
  Data[2] = ((num2) >> 8);
  Data[3] = (num2);
  Data[4] = ((num3) >> 8);
  Data[5] = (num3);
  Data[6] = ((num4) >> 8);
  Data[7] = (num4);
  if ((_hcan) == &hcan1) {
    HAL_CAN_AddTxMessage(&hcan1, &sendmessage, Data, &send_mail_box);
  }
  if ((_hcan) == &hcan2) {
    HAL_CAN_AddTxMessage(&hcan2, &sendmessage, Data, &send_mail_box);
  }
}
/**
 * @description: can1 and can2 receive callback function
 * @param[in]:Which can channel
 * @return: void
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  CAN_RxHeaderTypeDef rx_header;
  uint8_t rx_data[8];
  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
  if (hcan == &hcan1) {
    switch (rx_header.StdId) {
      case pitch_ID: {
        encoder_process(gimbal.gimbal_encoder.pitch_encoder, rx_data);
        break;
      }
      case frictiongearLeft_ID: {
        shoot.shoot_realy_speed.frictiongear_left_speed =
            (rx_data[2] << 8) + rx_data[3];
        break;
      }
      case frictiongearRight_ID: {
        shoot.shoot_realy_speed.frictiongear_right_speed =
            (rx_data[2] << 8) + rx_data[3];
        break;
      }
      case CAN_GYRO_yaw_ID: {
        gimbal.gyor_yaw_data.Angle_Last = gimbal.gyor_yaw_data.Angle;
        unchar2float.UN[0] = rx_data[0];
        unchar2float.UN[1] = rx_data[1];
        unchar2float.UN[2] = rx_data[2];
        unchar2float.UN[3] = rx_data[3];
        gimbal.gyor_yaw_data.Angle = unchar2float.F;
        if (gimbal.gyor_yaw_data.Angle - gimbal.gyor_yaw_data.Angle_Last >= 330)
          gimbal.gyor_yaw_data.Count--;
        else if (gimbal.gyor_yaw_data.Angle - gimbal.gyor_yaw_data.Angle_Last <=
                 -330)
          gimbal.gyor_yaw_data.Count++;
        gimbal.gyor_yaw_data.ecd_Angle =
            gimbal.gyor_yaw_data.Angle + gimbal.gyor_yaw_data.Count * 360;
        break;
      }
      case pluck_ID: {
        pluck_encoder_process(shoot.shoot_encoder.pluck_encoder, rx_data);
        break;
      }
      case yaw_ID: {
        encoder_process(gimbal.gimbal_encoder.yaw_encoder, rx_data);
        gimbal.gimbal_encoder.yaw_encoder->filter_rate =
            (rx_data[2] << 8) | rx_data[3];  //速度改为6020电机返回值
        // TODO:统一化更改以后再说
        break;
      }
    }
  }
  if (hcan == &hcan2) {
    switch (rx_header.StdId) {
      case CAN_RM3508_0_ID: {
        chassis.chassis_realy_speed.RM3508_0 = (rx_data[2] << 8) + rx_data[3];
        break;
      }
      case CAN_RM3508_1_ID: {
        chassis.chassis_realy_speed.RM3508_1 = (rx_data[2] << 8) + rx_data[3];
        break;
      }
      case CAN_RM3508_2_ID: {
        chassis.chassis_realy_speed.RM3508_2 = (rx_data[2] << 8) + rx_data[3];
        break;
      }
      case CAN_RM3508_3_ID: {
        chassis.chassis_realy_speed.RM3508_3 = (rx_data[2] << 8) + rx_data[3];
        break;
      }
    }
  }
}