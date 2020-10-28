/*
 * @Description: dr16 and DBUS header file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 14:37:04
 * @LastEditors: maxin
 * @LastEditTime: 2020-07-27 12:33:11
 */
#ifndef DRV_DR16_H
#define DRV_DR16_H
#include "main.h"
#define DR16_DATA_LEN (19u)
#define RC_S1_UP2MID (1 << 0u)
#define RC_S1_MID2UP (1 << 1u)
#define RC_S1_DOWN2MID (1 << 2u)
#define RC_S1_MID2DOWN (1 << 3u)

#define RC_S2_UP2MID (1 << 4u)
#define RC_S2_MID2UP (1 << 5u)
#define RC_S2_DOWN2MID (1 << 6u)
#define RC_S2_MID2DOWN (1 << 7u)

#define RC_S1_UP (1 << 8u)
#define RC_S1_MID (1 << 9u)
#define RC_S1_DOWN (1 << 10u)
#define RC_S2_UP (1 << 11u)
#define RC_S2_MID (1 << 12u)
#define RC_S2_DOWN (1 << 13u)
typedef struct rc_info *rc_info_t;
typedef struct rc_device *rc_device_t;
struct rc_info {
  /* rocker channel information */
  int16_t ch1;
  int16_t ch2;
  int16_t ch3;
  int16_t ch4;
  /* left and right lever information */
  uint8_t sw1;
  uint8_t sw2;
  /* mouse movement and button information */
  struct {
    int16_t x;
    int16_t y;
    int16_t z;

    uint8_t l;
    uint8_t r;
  } mouse;
  /* keyboard key information */
  union {
    uint16_t key_code;
    struct {
      uint16_t W : 1;
      uint16_t S : 1;
      uint16_t A : 1;
      uint16_t D : 1;
      uint16_t SHIFT : 1;
      uint16_t CTRL : 1;
      uint16_t Q : 1;
      uint16_t E : 1;
      uint16_t R : 1;
      uint16_t F : 1;
      uint16_t G : 1;
      uint16_t Z : 1;
      uint16_t X : 1;
      uint16_t C : 1;
      uint16_t V : 1;
      uint16_t B : 1;
    } bit;
  } kb;
  int16_t wheel;
};
struct rc_device {
  uint16_t state;
  struct rc_info rc_info;
  struct rc_info last_rc_info;
};
extern struct rc_device rcDevice;
void setDbusData(uint8_t *buff);
int32_t getDbusState(rc_device_t rc_dev, uint16_t state);
#endif /*end DRV_DR16_H*/
