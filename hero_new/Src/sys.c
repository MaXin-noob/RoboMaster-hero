/*
 * @Description: system source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 15:03:12
 * @LastEditors: maxin
 * @LastEditTime: 2020-09-23 10:05:46
 */
#include "sys.h"
unchar2float_u unchar2float;
JudgementDataTypedef JudgementData = {
    .judge_ok = false,
};
/*chassis init */
/*PID init */
struct pid RM3508_0_PID = {
    .param =
        {
            .p = 3,
            .i = 0,
            .d = 0,
            .inte_limit = 0,
            .max_out = 20000,
        },
};
struct pid RM3508_1_PID = {
    .param =
        {
            .p = 3,
            .i = 0,
            .d = 0,
            .inte_limit = 0,
            .max_out = 20000,
        },
};
struct pid RM3508_2_PID = {
    .param =
        {
            .p = 3,
            .i = 0,
            .d = 0,
            .inte_limit = 0,
            .max_out = 20000,
        },
};
struct pid RM3508_3_PID = {
    .param =
        {
            .p = 3,
            .i = 0,
            .d = 0,
            .inte_limit = 0,
            .max_out = 20000,
        },
};
/*底盘跟随速度*/
struct pid follow_PID = {
    .param =
        {
            .p = 0,
            .i = 0,
            .d = 0,
            .max_out = 1500,
        },
};
/*data init */
chassis_data chassis = {
    .chassis_mecanum =
        {
            .param =
                {
                    .wheel_perimeter = 478, /* 轮的周长（mm）*/
                    .wheeltrack = 460,      /* 轮距（mm）*/
                    .wheelbase = 500,       /*轴距（mm）*/
                    .rotate_x_offset =
                        22, /* 相对于底盘中心的x轴旋转偏移量(mm) */
                    .rotate_y_offset =
                        -7, /* 相对于底盘中心的y轴旋转偏移量(mm) */
                    .deceleration_ratio = 1.0f / 14.0f, /*电机减速比*/
                    .max_vx_speed = 3000, /*底盘的x轴的最大速度(mm/s)*/
                    .max_vy_speed = 3000, /*底盘的y轴的最大速度(mm/s)*/
                    .max_vw_speed = 1400, /*底盘的自转的最大速度(degree/s)*/
                    .max_wheel_ramp = 8500, /*3508最大转速*/
                },
            .speed =
                {
                    .vx = 0.0f,
                    .vy = 0.0f,
                    .vw = 0.0f,
                },
            .wheel_rpm = {0, 0, 0, 0},
        },
    /*底盘PID初始化*/
    .chassis_pid =
        {
            .RM3508_0_PID = &RM3508_0_PID,
            .RM3508_1_PID = &RM3508_1_PID,
            .RM3508_2_PID = &RM3508_2_PID,
            .RM3508_3_PID = &RM3508_3_PID,
            .follow_PID = &follow_PID,
        },
    .chassis_realy_speed =
        {
            .RM3508_0 = 0,
            .RM3508_1 = 0,
            .RM3508_2 = 0,
            .RM3508_3 = 0,
        },
    .chassis_mode = followed,
};
/*gimbal*/
/*PID init */
struct pid pitch_speed_PID = {
    .param =
        {
            .p = 27,
            .i = 0,
            .d = 0,
            .max_out = 6000,
        },
};
struct pid pitch_positio_PID = {
    .param =
        {
            .p = 26.5,
            .i = 0,
            .d = 0,
            .max_out = 8191,
        },
};
struct pid yaw_speed_PID = {
    .param =
        {
            .p = 117,
            .i = 0,
            .d = 0,
            .max_out = 29000,
        },
};
struct pid yaw_positio_PID = {
    .param =
        {
            .p = 12,
            .i = 0,
            .d = 0.3,
            .inte_limit = 300,
            .max_out = 29000,
        },
};
/*encoder init */
struct encoder pitch_encoder = {
    .ecd_bias = 3370, /*pitch轴初始位置*/
    .startFlag = 0,
};
struct encoder yaw_encoder = {
    .ecd_bias = 996, /*yaw轴初始位置*/
    .startFlag = 0,
};
struct version gimbal_version;
struct ramp pitch_start_ramp = {
    .scale = 2000, /*pitch轴启动斜坡*/
};
struct ramp yaw_start_ramp = {
    .scale = 996, /*yaw轴启动斜坡*/
};
struct ramp yaw_ramp = {
    .scale = 1000, /*yaw轴启动斜坡*/
};
/*data init */
gimbal_data gimbal = {
    .gimbal_encoder =
        {
            .pitch_encoder = &pitch_encoder,
            .yaw_encoder = &yaw_encoder,
        },
    .gimbal_pid =
        {
            .pitch_speed_PID = &pitch_speed_PID,
            .pitch_positio_PID = &pitch_positio_PID,
            .yaw_speed_PID = &yaw_speed_PID,
            .yaw_positio_PID = &yaw_positio_PID,
        },
    .version_data = &gimbal_version,
    .gimbal_ramp =
        {
            .pitch_start_ramp = &pitch_start_ramp,
            .yaw_start_ramp = &yaw_start_ramp,
            .yaw_ramp = &yaw_ramp,
        },
    .gimbal_angel =
        {
            .pitch_dbus_angle = 0.0f,
            .yaw_dbus_angle = 0.0f,
        },
    .gimbal_mode = start_up,
};
/*shoot*/
/*PID init */
struct pid frictiongear_left_PID = {
    .param =
        {
            .p = 20,
            .i = 0,
            .d = 0,
            .max_out = 6000,
        },
};
struct pid frictiongear_right_PID = {
    .param =
        {
            .p = 20,
            .i = 0,
            .d = 0,
            .max_out = 6000,
        },
};
struct pid pluck_positio_PID = {
    .param =
        {
            .p = 10,
            .i = 0,
            .d = 0,
            .max_out = 20000,
        },
};
struct pid pluck_speed_PID = {
    .param =
        {
            .p = 10,
            .i = 0,
            .d = 0,
            .max_out = 20000,
        },
};
/*encoder init */
struct pluckEncoder pluck_encoder;
/*data init */
shoot_data shoot = {
    .shoot_pid =
        {
            .frictiongear_left_PID = &frictiongear_left_PID,
            .frictiongear_right_PID = &frictiongear_right_PID,
            .pluck_positio_PID = &pluck_positio_PID,
            .pluck_speed_PID = &pluck_speed_PID,
        },
    .shoot_encoder =
        {
            .pluck_encoder = &pluck_encoder,
        },
    .frictiongear_flag = false,
    .pluck_angle = 0,
};