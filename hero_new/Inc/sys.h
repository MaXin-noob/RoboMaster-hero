/*
 * @Description: system header file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 15:03:24
 * @LastEditors: maxin
 * @LastEditTime: 2020-09-24 14:14:52
 */
#ifndef SYS_H
#define SYS_H
#include "main.h"
/*********************Special* options***************************/
/*是否禁用相关的功能，禁用之后can将不发送相关数据*/
#define FORBID_CHASSIS 1      /*是否禁用底盘，0为不禁用*/
#define FORBID_FIRCTIONGEAR 0 /*是否禁用摩擦轮，0为不禁用*/
#define FORBID_GIMBAL 0       /*是否禁用云台，0为不禁用*/
#define FORBID_NIMING 1       /*是否禁用匿名，0为不禁用*/

/*********************system * values****************************/
#define frictiongear_Speed 5000 /*摩擦轮初始速度*/
#define pluck_Rotator 1200      /*拨盘旋转量*/
#define sys_start_time 1500 /*云台启动时间value（time = value*2 单位：ms）*/
#define Maximum_permissible_output 30000; /*限制4个轮子的速度总和*/

#define frictiongearLeft_ID 0x201  /*左摩擦轮接收ID*/
#define frictiongearRight_ID 0x202 /*右摩擦轮接收ID*/
#define pitch_ID 0x206             /*pitch轴接收ID*/
#define yaw_ID 0x205               /*yaw轴接收ID*/
#define pluck_ID 0x204             /*拨盘接收ID*/
#define CAN_GYRO_yaw_ID 0x301      /*yaw轴陀螺仪接收ID*/
#define CAN_GYRO_pitch_ID 0x302    /*pitch轴陀螺仪接收ID*/
#define CAN_RM3508_0_ID 0x201      /*左前轮接收ID*/
#define CAN_RM3508_1_ID 0x202      /*左后轮接收ID*/
#define CAN_RM3508_2_ID 0x203      /*右前轮接收ID*/
#define CAN_RM3508_3_ID 0x204      /*右后接收ID*/

#define RM3510_RM3508_ID_Send_0x200 0x200 /*3510,3508发送id:0x200*/
#define RM3510_RM3508_ID_Send_0x1ff 0x1FF /*3510,3508发送id:0x1ff*/
#define RM6623_ID_Send_0x1ff 0x1FF        /*6623发送id：0x1ff*/
#define RM6020_ID_Send_0x1ff 0x1FF        /*6020发送id：0x1ff*/
/*********************Common * algorithms*************************/
#define VAL_LIMIT(val, min, max) \
  do {                           \
    if ((val) <= (min)) {        \
      (val) = (min);             \
    } else if ((val) >= (max)) { \
      (val) = (max);             \
    }                            \
  } while (0)

#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define Hero_FALSE (0u)
#define Hero_OK (1u)
#define Hero_NOSTATE (2u)
#define Hero_UNREGISTERED (3u)
#define Hero_INVAL (4u)
#ifndef NULL
#define NULL ((void *)0)
#endif
#ifndef PI
#define PI 3.1415926
#endif
typedef struct version *version_t;
typedef struct encoder *encoder_t;
typedef struct pid *pid_t;
typedef struct ramp *ramp_t;
typedef struct pluckEncoder *pluckEncoder_t;
typedef union {
  unsigned char UN[4];
  float F;
} unchar2float_u;
/*********************system struct and enum*********************/
/*chassis*/
struct mecanum_structure {
  float wheel_perimeter; /* 轮的周长（mm）*/
  float wheeltrack;      /* 轮距（mm）*/
  float wheelbase;       /*轴距（mm）*/
  float rotate_x_offset; /* 相对于底盘中心的x轴旋转偏移量(mm) */
  float rotate_y_offset; /* 相对于底盘中心的y轴旋转偏移量(mm) */
  float deceleration_ratio; /*电机减速比*/
  int max_vx_speed;         /*底盘的x轴的最大速度(mm/s)*/
  int max_vy_speed;         /*底盘的y轴的最大速度(mm/s)*/
  int max_vw_speed;         /*底盘的自转的最大速度(degree/s)*/
  int max_wheel_ramp;       /*3508最大转速*/
};
struct mecanum_speed {
  float vx;
  float vy;
  float vw;
};
struct mecanum {
  struct mecanum_structure param;
  struct mecanum_speed speed;
  float wheel_rpm[4];
};
enum chassis_mode_enum {
  followed,
  lose,
  rotate,
};
struct wheel_speed {
  int16_t RM3508_0;
  int16_t RM3508_1;
  int16_t RM3508_2;
  int16_t RM3508_3;
};
struct chassis_out {
  float RM3508_0_OUT;
  float RM3508_1_OUT;
  float RM3508_2_OUT;
  float RM3508_3_OUT;
  float follow_OUT;
};
struct wheel_pid {
  pid_t RM3508_0_PID;
  pid_t RM3508_1_PID;
  pid_t RM3508_2_PID;
  pid_t RM3508_3_PID;
  pid_t follow_PID;
};
typedef struct {
  struct wheel_speed chassis_realy_speed;
  struct wheel_pid chassis_pid;
  struct mecanum chassis_mecanum;
  struct chassis_out chassis_pid_out;
  enum chassis_mode_enum chassis_mode;
} chassis_data;
/*gimbal*/
struct gimbal_pid_s {
  pid_t pitch_speed_PID;
  pid_t pitch_positio_PID;
  pid_t yaw_speed_PID;
  pid_t yaw_positio_PID;
  float yaw_pid_out;
  float pitch_pid_out;
};
struct Encoder {
  encoder_t pitch_encoder;
  encoder_t yaw_encoder;
};
enum gimbal_Mode {
  start_up,
  remote,
  keyboard,
};
struct gyor {
  float Angle;
  float Angle_Last;
  float ecd_Angle;
  float GyroAngle_Bias;
  uint8_t Count;
  uint8_t lose_count;
  float Speed;
  uint8_t no_lose_count;
  uint8_t angle_start_flag;
};
struct ramp_s {
  ramp_t pitch_start_ramp;
  ramp_t yaw_start_ramp;
  ramp_t yaw_ramp;
};
struct gimbal_angel_s {
  float pitch_dbus_angle;
  float yaw_dbus_angle;
};
typedef struct {
  struct gimbal_pid_s gimbal_pid;
  struct Encoder gimbal_encoder;
  enum gimbal_Mode gimbal_mode;
  version_t version_data;
  struct gyor gyor_yaw_data;
  struct gyor gyor_pitch_data;
  struct ramp_s gimbal_ramp;
  struct gimbal_angel_s gimbal_angel;
} gimbal_data;
/*shoot*/
struct shoot_speed {
  int16_t frictiongear_left_speed;
  int16_t frictiongear_right_speed;
};
struct shoot_pid_s {
  pid_t frictiongear_left_PID;
  pid_t frictiongear_right_PID;
  pid_t pluck_positio_PID;
  pid_t pluck_speed_PID;
};
struct shoot_Encoder {
  pluckEncoder_t pluck_encoder;
};
struct shoot_out {
  float frictiongear_left_OUT;
  float frictiongear_right_OUT;
  float pluck_OUT;
};
typedef struct {
  struct shoot_speed shoot_realy_speed;
  struct shoot_pid_s shoot_pid;
  struct shoot_Encoder shoot_encoder;
  struct shoot_out shoot_pid_out;
  int frictiongear_flag;
  float pluck_angle;
} shoot_data;
/*judge*/
typedef enum {
  Robot_Status_ID = 0x0201,     //机器人状态等级
  power_heat_data_ID = 0x0202,  //枪口热量底盘功率
  robot_hurt_ID = 0x0206,       //伤害类型
  shoot_data_ID = 0x0207,       //射频射速
  student_interactive_header_ID = 0x0301,
  game_robot_pos_ID = 0x0203,  //机器人位置信息
} Judege_Cmd_ID;

//帧头
struct __packed tFrameHeader {
  uint8_t SOF;         // 0xA5
  uint16_t DataLenth;  //数据位长度
  uint8_t Seq;         //包序号
  uint8_t CRC8;        // crc8位校验
};

/*位置信息*/
struct __packed ext_game_robot_pos_t {
  float x;
  float y;
  float z;
  float yaw;
};

/*伤害类型*/
struct __packed ext_robot_hurt_t {
  uint8_t armor_id : 4;
  uint8_t hurt_type : 4;
};

/*比赛机器人状态*/
struct __packed ext_game_robot_state_t {
  uint8_t robot_id;                      //机器人ID
  uint8_t robot_level;                   //机器人等级
  uint16_t remain_HP;                    //机器人剩余HP
  uint16_t max_HP;                       //机器人上限血量
  uint16_t shooter_heat0_cooling_rate;   //机器人17mm枪口每秒冷却值
  uint16_t shooter_heat0_cooling_limit;  //枪口热量上限
  uint16_t shooter_heat1_cooling_rate;   // 42mm枪口每秒冷却值
  uint16_t shooter_heat1_cooling_limit;  //枪口热量上限
  uint8_t max_chassis_power;  //机器人最大底盘功率， 单位 w
  uint8_t mains_power_gimbal_output : 1;
  uint8_t mains_power_chassis_output : 1;
  uint8_t mains_power_shooter_output : 1;
};

/*底盘功率及枪口热量*/
struct __packed ext_power_heat_data_t {
  uint16_t chassis_volt;          //底盘输出电压  单位 毫伏
  uint16_t chassis_current;       //底盘输出电流  单位毫安
  float chassis_power;            //底盘输出功率  单位 W
  uint16_t chassis_power_buffer;  //底盘功率缓冲  单位J
  uint16_t shooter_heat0;         // 17mm枪口热量
  uint16_t shooter_heat1;         // 42mm枪口热量
  uint16_t mobile_shooter_heat2;
};

/*子弹信息*/
struct __packed ext_shoot_data_t {
  uint8_t bullet_type;  //子弹类型
  uint8_t bullet_freq;  //子弹射频
  float bullet_speed;   //子弹射速
};

struct __packed id_data_t {
  uint16_t rxCmdId;     //命令码
  uint16_t data_id;     //数据段id
  uint16_t send_id;     //数据段id
  uint16_t receive_id;  //数据段id
};

typedef struct {
  struct tFrameHeader frameHeader;                 //帧头
  uint16_t rxCmdId;                                //命令码
  struct ext_power_heat_data_t power_heat_data_t;  //底盘功率枪口热量数据
  struct ext_shoot_data_t shoot_data_t;            //射击信息
  struct ext_robot_hurt_t robot_hurt_t;            //伤害类型
  struct ext_game_robot_state_t game_robot_state_t;  //机器人基本信息
  struct ext_game_robot_pos_t game_robot_pos_t;      //机器人位置信息
  uint8_t judge_ok;
} JudgementDataTypedef;
/*********************system global * variable**********************/
extern chassis_data chassis;                //底盘
extern gimbal_data gimbal;                  //云台
extern shoot_data shoot;                    //射击
extern JudgementDataTypedef JudgementData;  //裁判系统解算数据
extern unchar2float_u unchar2float;
#endif /* end SYS_H*/