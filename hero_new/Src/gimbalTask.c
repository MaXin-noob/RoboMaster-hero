/*
 * @Description: yaw control source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 15:32:56
 * @LastEditors: maxin
 * @LastEditTime: 2020-09-23 00:02:49
 */

#include "gimbalTask.h"

static void yawControl(void);
static void pitchControl(void);
static int gyroscopeStartLossDetection(struct gyor* data);
static int time = 0;
/**
 * @description: gimbal handler
 * @param[in]:void const *argument
 * @return: void
 */
void gimbalHandler(void const* argument) {
  uint32_t period = osKernelSysTick();
  while (true) {
    if (rcDevice.rc_info.wheel >= 600) {  //紧急重启
      __set_FAULTMASK(1);
      NVIC_SystemReset();
    }
    if (time <= sys_start_time) {  //初始化模式
      gimbal.gimbal_mode = start_up;
      setFlashingColor(YELLOW);
      time += 1;
    } else {
      // if (gyroscopeStartLossDetection(&gimbal.gyor_yaw_data) == Hero_OK) {
      //   gimbal.gimbal_mode = remote;
      //   setFlashingColor(BLUE);
      // } else {
      //   setFlashingColor(RED);
      // }
      if (getDbusState(&rcDevice, RC_S1_DOWN) == Hero_OK) {
        gimbal.gimbal_mode = keyboard;
        setFlashingColor(BLUE);
      }
      if (getDbusState(&rcDevice, RC_S1_MID) == Hero_OK) {
        gimbal.gimbal_mode = remote;
        setFlashingColor(GREEN);
      }
    }
    yawControl();    // yaw轴控制
    pitchControl();  // pitch轴控制

#if FORBID_GIMBAL == 0
    sendCanData(&hcan1, RM6020_ID_Send_0x1ff, gimbal.gimbal_pid.yaw_pid_out,
                gimbal.gimbal_pid.pitch_pid_out, 0, 0);
#endif /*end FORBID_YAW*/

    osDelayUntil(&period, 2); /*2ms*/
  }
}

/**
 * @description: yaw control
 * @param[in]:void
 * @return: voidikjj
 */
static void yawControl(void) {
  float position_out;
  if (gimbal.gimbal_mode == start_up) { /*启动模式*/
    position_out =
        pid_calculate(gimbal.gimbal_pid.yaw_positio_PID,
                      gimbal.gimbal_encoder.yaw_encoder->ecd_angle *
                          ramp_calculate(gimbal.gimbal_ramp.yaw_start_ramp),
                      gimbal.gimbal_angel.yaw_dbus_angle);
    gimbal.gimbal_pid.yaw_pid_out = pid_calculate(
        gimbal.gimbal_pid.yaw_speed_PID,
        gimbal.gimbal_encoder.yaw_encoder->filter_rate, position_out);
    gimbal.gyor_yaw_data.GyroAngle_Bias = gimbal.gyor_yaw_data.ecd_Angle;
  } else if (gimbal.gimbal_mode == remote) { /*遥控模式*/
    gimbal.gimbal_angel.yaw_dbus_angle = -rcDevice.rc_info.ch1 * 0.04f;
    // position_out = pid_calculate(
    //     gimbal.gimbal_pid.yaw_positio_PID,
    //     -(gimbal.gyor_yaw_data.ecd_Angle -
    //     gimbal.gyor_yaw_data.GyroAngle_Bias),
    //     gimbal.gimbal_angel.yaw_dbus_angle);
    // gimbal.gimbal_pid.yaw_pid_out = pid_calculate(
    //     gimbal.gimbal_pid.yaw_speed_PID,
    //     gimbal.gimbal_encoder.yaw_encoder->filter_rate, position_out);
    VAL_LIMIT(gimbal.gimbal_angel.yaw_dbus_angle, -60, 60);
    position_out =
        pid_calculate(gimbal.gimbal_pid.yaw_positio_PID,
                      gimbal.gimbal_encoder.yaw_encoder->ecd_angle *
                          ramp_calculate(gimbal.gimbal_ramp.yaw_start_ramp),
                      gimbal.gimbal_angel.yaw_dbus_angle);
    gimbal.gimbal_pid.yaw_pid_out = pid_calculate(
        gimbal.gimbal_pid.yaw_speed_PID,
        gimbal.gimbal_encoder.yaw_encoder->filter_rate, position_out);
    gimbal.gyor_yaw_data.GyroAngle_Bias = gimbal.gyor_yaw_data.ecd_Angle;
  } else if (gimbal.gimbal_mode == keyboard) { /*键盘模式*/
    gimbal.gimbal_angel.yaw_dbus_angle = -rcDevice.rc_info.mouse.x * 0.05;
    VAL_LIMIT(gimbal.gimbal_angel.yaw_dbus_angle, -60, 60);
    position_out =
        pid_calculate(gimbal.gimbal_pid.yaw_positio_PID,
                      gimbal.gimbal_encoder.yaw_encoder->ecd_angle *
                          ramp_calculate(gimbal.gimbal_ramp.yaw_start_ramp),
                      gimbal.gimbal_angel.yaw_dbus_angle);
    gimbal.gimbal_pid.yaw_pid_out = pid_calculate(
        gimbal.gimbal_pid.yaw_speed_PID,
        gimbal.gimbal_encoder.yaw_encoder->filter_rate, position_out);
    gimbal.gyor_yaw_data.GyroAngle_Bias = gimbal.gyor_yaw_data.ecd_Angle;
  }
}
/**
 * @description: pitch control
 * @param[in]:void
 * @return: void
 */
static void pitchControl(void) {
  float position_out;
  if (gimbal.gimbal_mode == start_up) {
    position_out =
        pid_calculate(gimbal.gimbal_pid.pitch_positio_PID,
                      gimbal.gimbal_encoder.pitch_encoder->ecd_angle *
                          ramp_calculate(gimbal.gimbal_ramp.pitch_start_ramp),
                      gimbal.gimbal_angel.pitch_dbus_angle);
    gimbal.gimbal_pid.pitch_pid_out = pid_calculate(
        gimbal.gimbal_pid.pitch_speed_PID,
        gimbal.gimbal_encoder.pitch_encoder->filter_rate, -position_out);
  } else if (gimbal.gimbal_mode == remote) {
    gimbal.gimbal_angel.pitch_dbus_angle += rcDevice.rc_info.ch2 * 0.0001f;
    VAL_LIMIT(gimbal.gimbal_angel.pitch_dbus_angle, -15, 15);
    position_out = pid_calculate(gimbal.gimbal_pid.pitch_positio_PID,
                                 gimbal.gimbal_encoder.pitch_encoder->ecd_angle,
                                 gimbal.gimbal_angel.pitch_dbus_angle);
    gimbal.gimbal_pid.pitch_pid_out = pid_calculate(
        gimbal.gimbal_pid.pitch_speed_PID,
        gimbal.gimbal_encoder.pitch_encoder->filter_rate, -position_out);
  } else if (gimbal.gimbal_mode == keyboard) {
    gimbal.gimbal_angel.pitch_dbus_angle += rcDevice.rc_info.mouse.y * 0.0001f;
    VAL_LIMIT(gimbal.gimbal_angel.pitch_dbus_angle, -15, 15);
    position_out = pid_calculate(gimbal.gimbal_pid.pitch_positio_PID,
                                 gimbal.gimbal_encoder.pitch_encoder->ecd_angle,
                                 gimbal.gimbal_angel.pitch_dbus_angle);
    gimbal.gimbal_pid.pitch_pid_out = pid_calculate(
        gimbal.gimbal_pid.pitch_speed_PID,
        gimbal.gimbal_encoder.pitch_encoder->filter_rate, -position_out);
  }
}
/**
 * @description: Gyroscope start loss detection
 * @param[in]: gyro data
 * @return: Marker bit
 */
static int gyroscopeStartLossDetection(struct gyor* data) {
  float diff_angle = ABS(data->Angle - data->Angle_Last);
  if (data->angle_start_flag == false) {
    if ((diff_angle <= 0.003f) && (diff_angle != 0)) {
      data->lose_count = 0;
      data->no_lose_count++;
    } else {
      data->lose_count++;
      data->no_lose_count = 0;
    }
    if (data->lose_count >= 150) {
      return Hero_FALSE;
    }
    if (data->no_lose_count >= 150) {
      data->angle_start_flag = true;
      return Hero_OK;
    }
    return Hero_INVAL;
  }
  return Hero_OK;
}
