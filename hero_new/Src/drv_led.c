/*
 * @Description: set led color source file
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-01-16 14:12:28
 * @LastEditors: maxin
 * @LastEditTime: 2020-07-26 23:14:50
 */
#include "drv_led.h"
static color_t ledColor = WHITE;
/**
 * @description: set flash color function
 * @param[in]: color
 * @return: void
 */
void setFlashingColor(color_t color) { ledColor = color; }

/**
 * @description: get flash color function
 * @param[in]:
 * @return: void
 */
color_t getFlashingColor(void) { return ledColor; }
/**
 * @description: set GPIO high or low
 * @param[in]: whitch GPIO Pin
 * @param[in]: set high or low level
 * @return: void
 */
static void writeLedColor(uint16_t GPIO_Pin, GPIO_PinState PinState) {
  if (GPIO_Pin == GPIO_LED_RED_Pin)
    HAL_GPIO_WritePin(GPIO_LED_RED_GPIO_Port, GPIO_Pin, PinState);
  else if (GPIO_Pin == GPIO_LED_GREEN_Pin)
    HAL_GPIO_WritePin(GPIO_LED_GREEN_GPIO_Port, GPIO_Pin, PinState);
  else if (GPIO_Pin == GPIO_LED_BLUE_Pin)
    HAL_GPIO_WritePin(GPIO_LED_BLUE_GPIO_Port, GPIO_Pin, PinState);
}

/**
 * @description: set led color
 * @param[in]: color
 * @return: void
 */
void setLedColor(color_t color) {
  switch (color) {
    case RED: {
      writeLedColor(GPIO_LED_RED_Pin, GPIO_PIN_RESET);
      writeLedColor(GPIO_LED_GREEN_Pin, GPIO_PIN_SET);
      writeLedColor(GPIO_LED_BLUE_Pin, GPIO_PIN_SET);
    } break;
    case GREEN: {
      writeLedColor(GPIO_LED_RED_Pin, GPIO_PIN_SET);
      writeLedColor(GPIO_LED_GREEN_Pin, GPIO_PIN_RESET);
      writeLedColor(GPIO_LED_BLUE_Pin, GPIO_PIN_SET);
    } break;
    case BLUE: {
      writeLedColor(GPIO_LED_RED_Pin, GPIO_PIN_SET);
      writeLedColor(GPIO_LED_GREEN_Pin, GPIO_PIN_SET);
      writeLedColor(GPIO_LED_BLUE_Pin, GPIO_PIN_RESET);
    } break;
    case YELLOW: {
      writeLedColor(GPIO_LED_RED_Pin, GPIO_PIN_RESET);
      writeLedColor(GPIO_LED_GREEN_Pin, GPIO_PIN_RESET);
      writeLedColor(GPIO_LED_BLUE_Pin, GPIO_PIN_SET);
    } break;
    case PURPLE: {
      writeLedColor(GPIO_LED_RED_Pin, GPIO_PIN_RESET);
      writeLedColor(GPIO_LED_GREEN_Pin, GPIO_PIN_SET);
      writeLedColor(GPIO_LED_BLUE_Pin, GPIO_PIN_RESET);
    } break;
    case CYAN: {
      writeLedColor(GPIO_LED_RED_Pin, GPIO_PIN_SET);
      writeLedColor(GPIO_LED_GREEN_Pin, GPIO_PIN_RESET);
      writeLedColor(GPIO_LED_BLUE_Pin, GPIO_PIN_RESET);
    } break;
    case WHITE: {
      writeLedColor(GPIO_LED_RED_Pin, GPIO_PIN_RESET);
      writeLedColor(GPIO_LED_GREEN_Pin, GPIO_PIN_RESET);
      writeLedColor(GPIO_LED_BLUE_Pin, GPIO_PIN_RESET);
    } break;
    case OFF: {
      writeLedColor(GPIO_LED_RED_Pin, GPIO_PIN_SET);
      writeLedColor(GPIO_LED_GREEN_Pin, GPIO_PIN_SET);
      writeLedColor(GPIO_LED_BLUE_Pin, GPIO_PIN_SET);
    } break;
  }
}
