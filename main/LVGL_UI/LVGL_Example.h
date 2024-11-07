#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "demos/lv_demos.h"

#include "../LVGL_Driver/LVGL_Driver.h"
// #include "SD_SPI.h"
// #include "Wireless.h"

#define EXAMPLE1_LVGL_TICK_PERIOD_MS  1000


void Lvgl_Example1(void);
void set_angle(int32_t v);
void set_brightness(int brightness);

#ifdef __cplusplus
}
#endif