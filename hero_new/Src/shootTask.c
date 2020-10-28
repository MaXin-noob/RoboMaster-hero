/*
 * @Description: 射击控制文件
 * @Version: 2.0
 * @Autor: 马昕
 * @Date: 2020-01-06 23:06:09
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-06 16:19:34
 */

/***
 *               ii.                                         ;9ABH,
 *              SA391,                                    .r9GG35&G
 *              &#ii13Gh;                               i3X31i;:,rB1
 *              iMs,:,i5895,                         .5G91:,:;:s1:8A
 *               33::::,,;5G5,                     ,58Si,,:::,sHX;iH1
 *                Sr.,:;rs13BBX35hh11511h5Shhh5S3GAXS:.,,::,,1AG3i,GG
 *                .G51S511sr;;iiiishS8G89Shsrrsh59S;.,,,,,..5A85Si,h8
 *               :SB9s:,............................,,,.,,,SASh53h,1G.
 *            .r18S;..,,,,,,,,,,,,,,,,,,,,,,,,,,,,,....,,.1H315199,rX,
 *          ;S89s,..,,,,,,,,,,,,,,,,,,,,,,,....,,.......,,,;r1ShS8,;Xi
 *        i55s:.........,,,,,,,,,,,,,,,,.,,,......,.....,,....r9&5.:X1
 *       59;.....,.     .,,,,,,,,,,,...        .............,..:1;.:&s
 *      s8,..;53S5S3s.   .,,,,,,,.,..      i15S5h1:.........,,,..,,:99
 *      93.:39s:rSGB@A;  ..,,,,.....    .SG3hhh9G&BGi..,,,,,,,,,,,,.,83
 *      G5.G8  9#@@@@@X. .,,,,,,.....  iA9,.S&B###@@Mr...,,,,,,,,..,.;Xh
 *      Gs.X8 S@@@@@@@B；..,,,,,,,,,,. rA1 ,A@@@@@@@@@；........,,,,,,.iX:
 *     ;9. ,8A#@@@@@@#5,.,,,,,,,,,... 9A. 8@@@@@@@@@@M;    ....,,,,,,,,S8
 *     X3    iS8XAHH8s.,,,,,,,,,,...,..58hH@@@@@@@@@Hs       ...,,,,,,,:Gs
 *    r8,        ,,,...,,,,,,,,,,.....  ,h8XABMMHX3r.          .,,,,,,,.rX:
 *   :9, .    .:,..,:;;;::,.,,,,,..          .,,.               ..,,,,,,.59
 *  .Si      ,:.i8HBMMMMMB&5,....                    .            .,,,,,.sMr
 *  SS       :: h@@@@@@@@@@#; .                     ...  .         ..,,,,iM5
 *  91  .    ;:.,1&@@@@@@MXs.                            .          .,,:,:&S
 *  hS ....  .:;,,,i3MMS1;..,..... .  .     ...                     ..,:,.99
 *  ,8; ..... .,:,..,8Ms:;,,,...                                     .,::.83
 *   s&: ....  .sS553B@@HX3s;,.    .,;13h.                            .:::&1
 *    SXr  .  ...;s3G99XA&X88Shss11155hi.                             ,;:h&,
 *     iH8:  . ..   ,;iiii;,::,,,,,.                                 .;irHA
 *      ,8X5;   .     .......                                       ,;iihS8Gi
 *         1831,                                                 .,;irrrrrs&@
 *           ;5A8r.                                            .:;iiiiirrss1H
 *             :X@H3s.......                                .,:;iii;iiiiirsrh
 *              r#h:;,...,,.. .,,:;;;;;:::,...              .:;;;;;;iiiirrss1
 *             ,M8 ..,....,.....,,::::::,,...         .     .,;;;iiiiiirss11h
 *             8B;.,,,,,,,.,.....          .           ..   .:;;;;iirrsss111h
 *            i@5,:::,,,,,,,,.... .                   . .:::;;;;;irrrss111111
 *            9Bi,:,,,,......                        ..r91;;;;;iirrsss1ss1111
 *8888888888888888888射击任务能不能射出去？？？？？？？？88888888888888888888888
 */
#include "shootTask.h"

static void frictiongearControl(void);
static void pluckControl(void);
static void scanningShoot(void);
static int pluck_flag = 0;
static int scanning_flag = 0;
static int jam_time_1 = 0;
static int jam_time_2 = 0;
/**
 * @description: shoot control
 * @param[in]:void const *argument
 * @return: void
 */
void shootHandler(void const *argument) {
  uint32_t period = osKernelSysTick();
  while (1) {
    frictiongearControl();  //摩擦轮控制
    pluckControl();         // 拨盘控制
                            //  scanningShoot();        // 卡弹扫描
#if FORBID_FIRCTIONGEAR == 0
    sendCanData(&hcan1, RM3510_RM3508_ID_Send_0x200,
                shoot.shoot_pid_out.frictiongear_left_OUT,
                shoot.shoot_pid_out.frictiongear_right_OUT, 0,
                shoot.shoot_pid_out.pluck_OUT);
#endif /*end FORBID_FIRCTIONGEAR*/
    osDelayUntil(&period, 6);
  }
}
/**
 * @description: frictiongear control
 * @param[in]:void
 * @return: void
 */
static void frictiongearControl(void) {
  if (gimbal.gimbal_mode == remote) {
    if (getDbusState(&rcDevice, RC_S2_MID2UP) == Hero_OK) {
      if (shoot.frictiongear_flag == false) {
        shoot.frictiongear_flag = true;
      } else {
        shoot.frictiongear_flag = false;
      }
    }
  }
  if (gimbal.gimbal_mode == keyboard) {
    if (rcDevice.rc_info.mouse.l == true) {
      shoot.frictiongear_flag = true;
    }
  }
  if (shoot.frictiongear_flag == true) {  //开摩擦轮
    shoot.shoot_pid_out.frictiongear_left_OUT = pid_calculate(
        shoot.shoot_pid.frictiongear_left_PID,
        shoot.shoot_realy_speed.frictiongear_left_speed, -frictiongear_Speed);
    shoot.shoot_pid_out.frictiongear_right_OUT = pid_calculate(
        shoot.shoot_pid.frictiongear_right_PID,
        shoot.shoot_realy_speed.frictiongear_right_speed, frictiongear_Speed);
  } else {  //关摩擦轮
    shoot.shoot_pid_out.frictiongear_left_OUT =
        pid_calculate(shoot.shoot_pid.frictiongear_left_PID,
                      shoot.shoot_realy_speed.frictiongear_left_speed, 0);
    shoot.shoot_pid_out.frictiongear_right_OUT =
        pid_calculate(shoot.shoot_pid.frictiongear_right_PID,
                      shoot.shoot_realy_speed.frictiongear_right_speed, 0);
  }
}
/**
 * @description: pluck control
 * @param[in]:void
 * @return: void
 */
static void pluckControl(void) {
  float position_out;
  if (shoot.frictiongear_flag == true) {
    if (gimbal.gimbal_mode == remote) {
      if ((getDbusState(&rcDevice, RC_S2_MID2DOWN) == Hero_OK) &&
          (pluck_flag == 0) &&
          JudgementData.power_heat_data_t.shooter_heat1 <= 100) {
        shoot.pluck_angle += pluck_Rotator;
        pluck_flag = 1;
      }
      if (getDbusState(&rcDevice, RC_S2_DOWN2MID) == Hero_OK) {
        pluck_flag = 0;
      }
    } else if (gimbal.gimbal_mode == keyboard) {
      if ((rcDevice.rc_info.mouse.l == true) && (pluck_flag == 0) &&
          JudgementData.power_heat_data_t.shooter_heat1 <= 100) {
        shoot.pluck_angle += pluck_Rotator;
        pluck_flag = 1;
      }
      if (rcDevice.rc_info.mouse.l == false) {
        pluck_flag = 0;
      }
    }
  }
  position_out = pid_calculate(shoot.shoot_pid.pluck_positio_PID,
                               shoot.shoot_encoder.pluck_encoder->ecd_angle,
                               shoot.pluck_angle);
  shoot.shoot_pid_out.pluck_OUT = pid_calculate(
      shoot.shoot_pid.pluck_speed_PID,
      shoot.shoot_encoder.pluck_encoder->filter_rate, position_out);
}
/**
 * @description: jamming scanning
 * @param[in]: void
 * @return: void
 */
static void scanningShoot(void) {
  if ((shoot.pluck_angle - shoot.shoot_encoder.pluck_encoder->ecd_angle > 60) &&
      (scanning_flag = false)) {
    if (shoot.shoot_encoder.pluck_encoder->filter_rate <= 50) {
      jam_time_1 += 1;
      if (jam_time_1 == 80) {
        scanning_flag = true;
        shoot.pluck_angle = shoot.shoot_encoder.pluck_encoder->ecd_angle - 200;
      }
    } else {
      jam_time_1 = 0;
    }
  } else {
    jam_time_1 = 0;
  }
  if (scanning_flag == true) {
    if (jam_time_2 == 100 ||
        (shoot.pluck_angle - shoot.shoot_encoder.pluck_encoder->ecd_angle >
         -10)) {
      jam_time_2 = 0;
      scanning_flag = true;
      shoot.pluck_angle += pluck_Rotator;
    } else {
      jam_time_2 += 1;
    }
  }
}