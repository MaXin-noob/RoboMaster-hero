/*
 * @Description: judgeCalculate task source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-07-30 23:00:36
 * @LastEditors: maxin
 * @LastEditTime: 2020-08-01 20:47:25
 */
#include "judgeTask.h"

static void judgeCalculate(uint8_t *JudgeDataBuffer);
void judgeHandler(void const *argument) {
  while (1) {
    if (JudgementData.judge_ok == true) {
      judgeCalculate(getJudgeData());
    }
    osDelay(10);
  }
}

static void judgeCalculate(uint8_t *JudgeDataBuffer) {
  static uint16_t start_pos = 0, next_start_pos = 0;
  while (true) {
    memcpy(&JudgementData.frameHeader, &JudgeDataBuffer[start_pos],
           FrameHeader_Len);
    /*先校验头帧0xA5 然后crc8校验帧头 再crc16位校验整包*/
    if ((JudgementData.frameHeader.SOF == (uint16_t)JudgeFrameHeader) &&
        (1 ==
         Verify_CRC8_Check_Sum(&JudgeDataBuffer[start_pos], FrameHeader_Len)) &&
        (1 == Verify_CRC16_Check_Sum(
                  &JudgeDataBuffer[start_pos],
                  JudgementData.frameHeader.DataLenth + FrameHeader_Len +
                      4)))  //数据位长度+帧头长度+命令码长度+校验码长度
    {
      memcpy(&JudgementData.rxCmdId, (&JudgeDataBuffer[start_pos] + 5),
             sizeof(JudgementData.rxCmdId));
      JudgeDataBuffer
          [start_pos]++;  //每处理完一次就在帧头加一防止再次处理这帧数据
      next_start_pos = start_pos + 9 +
                       JudgementData.frameHeader
                           .DataLenth;  // 9为 5位帧头 2位数据长度 2校验位
      switch (JudgementData.rxCmdId) {
        case Robot_Status_ID:  //读取机器人等级血量等
        {
          memcpy(&JudgementData.game_robot_state_t,
                 (&JudgeDataBuffer[start_pos] + 7),
                 JudgementData.frameHeader
                     .DataLenth);  //把数组中的数据复制到对应的结构体中去
        } break;
        case power_heat_data_ID:  //读取机器人枪口热量
        {
          memcpy(&JudgementData.power_heat_data_t,
                 (&JudgeDataBuffer[start_pos] + 7),
                 JudgementData.frameHeader
                     .DataLenth);  //把数组中的数据复制到对应的结构体中去
        } break;
        case shoot_data_ID:  //读取机器人射频射速
        {
          memcpy(&JudgementData.shoot_data_t, (&JudgeDataBuffer[start_pos] + 7),
                 JudgementData.frameHeader
                     .DataLenth);  //把数组中的数据复制到对应的结构体中去
        } break;
        case robot_hurt_ID:  //伤害类型
        {
          memcpy(&JudgementData.robot_hurt_t, (&JudgeDataBuffer[start_pos] + 7),
                 JudgementData.frameHeader
                     .DataLenth);  //把数组中的数据复制到对应的结构体中去
        } break;
        case game_robot_pos_ID:  //机器人位置
        {
          memcpy(&JudgementData.game_robot_pos_t,
                 (&JudgeDataBuffer[start_pos] + 7),
                 JudgementData.frameHeader.DataLenth);
        } break;
      }
      start_pos = next_start_pos;
    } else {
      start_pos = 0;
      break;
    }
    /**如果头指针越界了退出循环**/
    if (start_pos > JudgeBufferLength) {
      start_pos = 0;
      JudgementData.judge_ok = false;
      break;
    }
  }
}