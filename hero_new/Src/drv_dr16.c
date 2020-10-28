/*
 * @Description: dr16 and DBUS source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 14:38:14
 * @LastEditors: maxin
 * @LastEditTime: 2020-07-30 11:42:08
 */
#include "drv_dr16.h"
struct rc_device rcDevice;
/**
 * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
 * │Esc│   │ F1│ F2│ F3│ F4│ │ F5│ F6│ F7│ F8│ │ F9│F10│F11│F12│ │P/S│S L│P/B│  ┌┐    ┌┐    ┌┐
 * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘  └┘    └┘    └┘
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
 * │~ `│! 1│@ 2│# 3│$ 4│% 5│^ 6│& 7│* 8│( 9│) 0│_ -│+ =│ BacSp │ │Ins│Hom│PUp│ │N L│ / │ * │ - │
 * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
 * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │{ [│} ]│ | \ │ │Del│End│PDn│ │ 7 │ 8 │ 9 │   │
 * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───┤ + │
 * │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │: ;│" '│ Enter  │               │ 4 │ 5 │ 6 │   │
 * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐     ├───┼───┼───┼───┤
 * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │< ,│> .│? /│  Shift   │     │ ↑ │     │ 1 │ 2 │ 3 │   │
 * ├─────┬──┴─┬─┴──┬┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤ E││
 * │ Ctrl│    │Alt │         Space         │ Alt│    │    │Ctrl│ │ ← │ ↓ │ → │ │   0   │ . │←─┘│
 * └─────┴────┴────┴───────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
 **/
/**
 * @description: calculate dr16 data
 * @param[in]: dr16 struct
 * @param[in]: usart data
 * @return: void
 */
static void getDr16Data(rc_device_t rc_dev, uint8_t *buff) {
  memcpy(&(rc_dev->last_rc_info), &rc_dev->rc_info, sizeof(struct rc_info));

  rc_info_t rc = &rc_dev->rc_info;

  rc->ch1 = (buff[0] | buff[1] << 8) & 0x07FF;
  rc->ch1 -= 1024;
  rc->ch2 = (buff[1] >> 3 | buff[2] << 5) & 0x07FF;
  rc->ch2 -= 1024;
  rc->ch3 = (buff[2] >> 6 | buff[3] << 2 | buff[4] << 10) & 0x07FF;
  rc->ch3 -= 1024;
  rc->ch4 = (buff[4] >> 1 | buff[5] << 7) & 0x07FF;
  rc->ch4 -= 1024;

  /* prevent remote control zero deviation */
  if (rc->ch1 <= 5 && rc->ch1 >= -5) rc->ch1 = 0;
  if (rc->ch2 <= 5 && rc->ch2 >= -5) rc->ch2 = 0;
  if (rc->ch3 <= 5 && rc->ch3 >= -5) rc->ch3 = 0;
  if (rc->ch4 <= 5 && rc->ch4 >= -5) rc->ch4 = 0;

  rc->sw1 = ((buff[5] >> 4) & 0x000C) >> 2;
  rc->sw2 = (buff[5] >> 4) & 0x0003;

  if ((ABS(rc->ch1) > 660) || (ABS(rc->ch2) > 660) || (ABS(rc->ch3) > 660) ||
      (ABS(rc->ch4) > 660)) {
    memset(rc, 0, sizeof(struct rc_info));
    return;
  }

  rc->mouse.x = buff[6] | (buff[7] << 8);  // x axis
  rc->mouse.y = buff[8] | (buff[9] << 8);
  rc->mouse.z = buff[10] | (buff[11] << 8);

  rc->mouse.l = buff[12];
  rc->mouse.r = buff[13];

  rc->kb.key_code = buff[14] | buff[15] << 8;  // key borad code
  rc->wheel = (buff[16] | buff[17] << 8) - 1024;
}

/**
 * @description: get dbus state
 * @param[in]: dbus struct
 * @return: void
 */
static void getDr16State(rc_device_t rc_dev) {
  if (rc_dev->rc_info.sw1 == 3) {
    rc_dev->state |= RC_S1_MID;
    rc_dev->state &= ~RC_S1_UP;
    rc_dev->state &= ~RC_S1_DOWN;
    if (rc_dev->last_rc_info.sw1 == 1) {
      rc_dev->state |= RC_S1_UP2MID;
    } else if (rc_dev->last_rc_info.sw1 == 2) {
      rc_dev->state |= RC_S1_DOWN2MID;
    }
  } else if (rc_dev->rc_info.sw1 == 1) {
    rc_dev->state &= ~RC_S1_MID;
    rc_dev->state |= RC_S1_UP;
    rc_dev->state &= ~RC_S1_DOWN;
    if (rc_dev->last_rc_info.sw1 == 3) {
      rc_dev->state |= RC_S1_MID2UP;
    }
  } else if (rc_dev->rc_info.sw1 == 2) {
    rc_dev->state &= ~RC_S1_MID;
    rc_dev->state &= ~RC_S1_UP;
    rc_dev->state |= RC_S1_DOWN;
    if (rc_dev->last_rc_info.sw1 == 3) {
      rc_dev->state |= RC_S1_MID2DOWN;
    }
  }

  if (rc_dev->rc_info.sw2 == 3) {
    rc_dev->state |= RC_S2_MID;
    rc_dev->state &= ~RC_S2_UP;
    rc_dev->state &= ~RC_S2_DOWN;
    if (rc_dev->last_rc_info.sw2 == 1) {
      rc_dev->state |= RC_S2_UP2MID;
    } else if (rc_dev->last_rc_info.sw2 == 2) {
      rc_dev->state |= RC_S2_DOWN2MID;
    }
  } else if (rc_dev->rc_info.sw2 == 1) {
    rc_dev->state &= ~RC_S2_MID;
    rc_dev->state |= RC_S2_UP;
    rc_dev->state &= ~RC_S2_DOWN;
    if (rc_dev->last_rc_info.sw2 == 3) {
      rc_dev->state |= RC_S2_MID2UP;
    }
  } else if (rc_dev->rc_info.sw2 == 2) {
    rc_dev->state &= ~RC_S2_MID;
    rc_dev->state &= ~RC_S2_UP;
    rc_dev->state |= RC_S2_DOWN;
    if (rc_dev->last_rc_info.sw2 == 3) {
      rc_dev->state |= RC_S2_MID2DOWN;
    }
  }
}

/**
 * @description: set DBUS data
 * @param[in]: usart data
 * @return: void
 */
void setDbusData(uint8_t *buff) {
  getDr16Data(&rcDevice, buff);
  getDr16State(&rcDevice);
}

/**
 * @description: get DBUS state
 * @param[in]: DBUS struct site
 * @param[in]: state
 * @return: Returns 1 if the state is needed
 */
int32_t getDbusState(rc_device_t rc_dev, uint16_t state) {
  taskENTER_CRITICAL();

  if (rc_dev != NULL) {
    if ((rc_dev->state & state) == state) {
      rc_dev->state &= (~(state & 0x00FF));
      taskEXIT_CRITICAL();
      return Hero_OK;
    } else {
      taskEXIT_CRITICAL();
      return Hero_NOSTATE;
    }
  }

  taskEXIT_CRITICAL();
  return Hero_UNREGISTERED;
}
