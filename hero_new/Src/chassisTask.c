/*
 * @Description: chassis control source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 15:32:56
 * @LastEditors: maxin
 * @LastEditTime: 2020-09-23 00:00:50
 */

#include "chassisTask.h"
static void MecanumCalculate(struct mecanum *mec);
static void ChassisModeControl(void);
void loop_float_constrain(float *Input, float minValue, float maxValue);

/**
 * @description: chassis handle
 * @param[in]: void const *argument
 * @return: void
 */
void chassisHandler(void const *argument) {
  uint32_t period = osKernelSysTick();
  float ax;
  float ay;
  float sin_calculating = 0.0f, cos_calculating = 0.0f;  //计算出的sin、cos值
  while (1) {
    sin_calculating = arm_sin_f32(
        -gimbal.gimbal_encoder.yaw_encoder->ecd_rad);  //计算出来的sin
    cos_calculating = arm_cos_f32(
        -gimbal.gimbal_encoder.yaw_encoder->ecd_rad);  //计算出来的cos值
    if (gimbal.gimbal_mode == remote) {
      ax = (float)rcDevice.rc_info.ch4 / 660 *
           (chassis.chassis_mecanum.param.max_vx_speed); /*原始x轴速度计算*/
      ay = -(float)rcDevice.rc_info.ch3 / 660 *
           (chassis.chassis_mecanum.param.max_vy_speed); /*原始y轴速度计算*/
    }
    if (gimbal.gimbal_mode == keyboard) {
      if (rcDevice.rc_info.kb.bit.W == true) {
        ax = chassis.chassis_mecanum.param.max_vx_speed;
      }
      if (rcDevice.rc_info.kb.bit.S == true) {
        ax = -chassis.chassis_mecanum.param.max_vx_speed;
      }
      if (rcDevice.rc_info.kb.bit.A == true) {
        ay = chassis.chassis_mecanum.param.max_vy_speed;
      }
      if (rcDevice.rc_info.kb.bit.D == true) {
        ay = -chassis.chassis_mecanum.param.max_vy_speed;
      }
      if ((rcDevice.rc_info.kb.bit.D == false) &&
          (rcDevice.rc_info.kb.bit.A == false))
        ay = 0;
      if ((rcDevice.rc_info.kb.bit.W == false) &&
          (rcDevice.rc_info.kb.bit.S == false))
        ax = 0;
    }
    first_order_filter_cali(&chassis_cmd_vx, &ax); /*x轴一阶滞后滤波*/
    first_order_filter_cali(&chassis_cmd_vy, &ay); /*y轴一阶滞后滤波*/
    chassis.chassis_mecanum.speed.vx =
        sin_calculating * ay + cos_calculating * ax;
    chassis.chassis_mecanum.speed.vy =
        -sin_calculating * ax + cos_calculating * ay; /*坐标系转换*/
    chassis.chassis_mecanum.speed.vw = chassis.chassis_pid_out.follow_OUT;
    // chassis.chassis_mecanum.speed.vw =-rcDevice.rc_info.ch1 * 0.2f;
    ChassisModeControl(); /*模式控制*/
    MecanumCalculate(&chassis.chassis_mecanum);
    /*
    电机ID位置明示
         (wheel_rpm[1])2||     || 1(wheel_rpm[0])
                         |     |
                         |     |
         (wheel_rpm[2])3||     || 4(wheel_rpm[3])   我爱樊小凯
    */
    chassis.chassis_pid_out.RM3508_0_OUT = pid_calculate(
        chassis.chassis_pid.RM3508_0_PID, chassis.chassis_realy_speed.RM3508_0,
        chassis.chassis_mecanum.wheel_rpm[0]);
    chassis.chassis_pid_out.RM3508_1_OUT = pid_calculate(
        chassis.chassis_pid.RM3508_1_PID, chassis.chassis_realy_speed.RM3508_1,
        chassis.chassis_mecanum.wheel_rpm[1]);
    chassis.chassis_pid_out.RM3508_2_OUT = pid_calculate(
        chassis.chassis_pid.RM3508_2_PID, chassis.chassis_realy_speed.RM3508_2,
        chassis.chassis_mecanum.wheel_rpm[2]);
    chassis.chassis_pid_out.RM3508_3_OUT = pid_calculate(
        chassis.chassis_pid.RM3508_3_PID, chassis.chassis_realy_speed.RM3508_3,
        chassis.chassis_mecanum.wheel_rpm[3]);
#if FORBID_CHASSIS == 0
    if (gimbal.gimbal_mode != start_up) {
      sendCanData(&hcan2, RM3510_RM3508_ID_Send_0x200,
                  chassis.chassis_pid_out.RM3508_0_OUT,
                  chassis.chassis_pid_out.RM3508_1_OUT,
                  chassis.chassis_pid_out.RM3508_2_OUT,
                  chassis.chassis_pid_out.RM3508_3_OUT);
    } else {
      sendCanData(&hcan2, RM3510_RM3508_ID_Send_0x200, 0, 0, 0, 0);
    }

#endif /*end FORBID_CHASSIS*/
    osDelayUntil(&period, 4);
  }
}
/**
 * @description: .F:forword; B:backword; L:left; R:right;
 * @param input : ccx=+vx(mm/s)  ccy=+vy(mm/s)  ccw=+vw(deg/s)
 *        output: every wheel speed(rpm)
 * @note  1=FR 2=FL 3=BL 4=BR
 */
static void MecanumCalculate(struct mecanum *mec) {
  static float rotate_ratio_fr;
  static float rotate_ratio_fl;
  static float rotate_ratio_bl;
  static float rotate_ratio_br;
  static float wheel_rpm_ratio;
  /*计算旋转比率*/
  rotate_ratio_fr = ((mec->param.wheelbase + mec->param.wheeltrack) / 2.0f -
                     mec->param.rotate_x_offset + mec->param.rotate_y_offset) /
                    RADIAN_COEF;
  rotate_ratio_fl = ((mec->param.wheelbase + mec->param.wheeltrack) / 2.0f -
                     mec->param.rotate_x_offset - mec->param.rotate_y_offset) /
                    RADIAN_COEF;
  rotate_ratio_bl = ((mec->param.wheelbase + mec->param.wheeltrack) / 2.0f +
                     mec->param.rotate_x_offset - mec->param.rotate_y_offset) /
                    RADIAN_COEF;
  rotate_ratio_br = ((mec->param.wheelbase + mec->param.wheeltrack) / 2.0f +
                     mec->param.rotate_x_offset + mec->param.rotate_y_offset) /
                    RADIAN_COEF;
  wheel_rpm_ratio =
      60.0f / (mec->param.wheel_perimeter * mec->param.deceleration_ratio);
  VAL_LIMIT(mec->speed.vx, -mec->param.max_vx_speed,
            mec->param.max_vx_speed);  // mm/s
  VAL_LIMIT(mec->speed.vy, -mec->param.max_vy_speed,
            mec->param.max_vy_speed);  // mm/s
  VAL_LIMIT(mec->speed.vw, -mec->param.max_vw_speed,
            mec->param.max_vw_speed);  // deg/s
  float wheel_rpm[4];
  float max = 0;
  wheel_rpm[0] =
      (-mec->speed.vx - mec->speed.vy - mec->speed.vw * rotate_ratio_fr) *
      wheel_rpm_ratio;
  wheel_rpm[1] =
      (mec->speed.vx - mec->speed.vy - mec->speed.vw * rotate_ratio_fl) *
      wheel_rpm_ratio;
  wheel_rpm[2] =
      (mec->speed.vx + mec->speed.vy - mec->speed.vw * rotate_ratio_bl) *
      wheel_rpm_ratio;
  wheel_rpm[3] =
      (-mec->speed.vx + mec->speed.vy - mec->speed.vw * rotate_ratio_br) *
      wheel_rpm_ratio;
  for (uint8_t i = 0; i < 4; i++) {
    if (fabs(wheel_rpm[i]) > max) max = fabs(wheel_rpm[i]);
  }
  if (max > mec->param.max_wheel_ramp) {
    float rate = mec->param.max_wheel_ramp / max;
    for (uint8_t i = 0; i < 4; i++) wheel_rpm[i] *= rate;
  }
  memcpy(mec->wheel_rpm, wheel_rpm, 4 * sizeof(float));
}
/**
 * @description: chassis mode control
 * @param[in]: void
 * @return: void
 */
static void ChassisModeControl(void) {
  if (gimbal.gimbal_mode == remote ||
      gimbal.gimbal_mode == keyboard) { /*启动模式*/
    if (getDbusState(&rcDevice, RC_S1_MID) == Hero_OK) {
      chassis.chassis_mode = followed;  //底盘跟随
    }
    //  else if (getDbusState(&rcDevice, RC_S1_DOWN) == Hero_OK) {
    //    chassis.chassis_mode = rotate;   //小陀螺
    // }
    if (chassis.chassis_mode == followed) {
      chassis.chassis_pid.follow_PID->param.p = 45;
      chassis.chassis_pid_out.follow_OUT =
          pid_calculate(chassis.chassis_pid.follow_PID,
                        -gimbal.gimbal_encoder.yaw_encoder->ecd_angle * 0.1, 0);
    }
    // if (chassis.chassis_mode == rotate) {
    //   chassis.chassis_pid.follow_PID->param.p = 10;
    //   chassis.chassis_pid_out.follow_OUT = pid_calculate(
    //       chassis.chassis_pid.follow_PID,
    //       -gimbal.gimbal_encoder.yaw_encoder->ecd_angle * 0.07,
    //       -gimbal.gimbal_encoder.yaw_encoder->ecd_angle * 0.07 + PI * 10);
    // }
  }
}
/**
 * @description: Cyclic limiting function
 * @param[in]: input
 * @param[in]: minValue
 * @param[in]: maxValue
 * @return: void
 */
void loop_float_constrain(float *Input, float minValue, float maxValue) {
  float len = maxValue - minValue;
  if (*Input > maxValue) {
    while (*Input > maxValue) {
      *Input -= len;
    }
  } else if (*Input < minValue) {
    while (*Input < minValue) {
      *Input += len;
    }
  }
}

/**
 * @description: chassis power control function
 * @param[in]: void
 * @return: void
 */
void chassis_power_control(void) {
  float CMRATO = 0.0f;                 //功率限制比例系数
  float Post_limiting_current = 0.0f;  //电流分配,平地模式下分配是均匀的
  float Sumpwoer = ABS(chassis.chassis_pid_out.RM3508_0_OUT) +
                   ABS(chassis.chassis_pid_out.RM3508_1_OUT) +
                   ABS(chassis.chassis_pid_out.RM3508_2_OUT) +
                   ABS(chassis.chassis_pid_out.RM3508_3_OUT);
  if (JudgementData.power_heat_data_t.chassis_power_buffer < 60) {
    CMRATO =
        (float)(JudgementData.power_heat_data_t.chassis_power_buffer / 60.0) *
        (float)(JudgementData.power_heat_data_t.chassis_power_buffer / 60.0);

    Post_limiting_current = CMRATO * Maximum_permissible_output;
  } else {  //焦耳能量恢复到一定数值
    Post_limiting_current = Maximum_permissible_output;
  }
  //底盘各电机电流重新分配
  if (Sumpwoer > Post_limiting_current) {
    chassis.chassis_pid_out.RM3508_0_OUT =
        chassis.chassis_pid_out.RM3508_0_OUT / Sumpwoer * Post_limiting_current;
    chassis.chassis_pid_out.RM3508_1_OUT =
        chassis.chassis_pid_out.RM3508_1_OUT / Sumpwoer * Post_limiting_current;
    chassis.chassis_pid_out.RM3508_2_OUT =
        chassis.chassis_pid_out.RM3508_2_OUT / Sumpwoer * Post_limiting_current;
    chassis.chassis_pid_out.RM3508_3_OUT =
        chassis.chassis_pid_out.RM3508_3_OUT / Sumpwoer * Post_limiting_current;
  }
}