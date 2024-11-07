#include "LVGL_Example.h"
#include "driver/ledc.h"

/**********************
 *      TYPEDEFS
 **********************/

#define BACKLIGHT_PIN 22       // Set your backlight control pin
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_TIMER   LEDC_TIMER_0
#define LEDC_MODE    LEDC_LOW_SPEED_MODE
#define LEDC_FREQUENCY 5000   // 5 kHz frequency
#define LEDC_RESOLUTION LEDC_TIMER_8_BIT

typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE,
} disp_size_t;

/**********************
 *  STATIC VARIABLES
 **********************/
static disp_size_t disp_size;

static lv_obj_t * tv;

static const lv_font_t * font_large;
static const lv_font_t * font_normal;

lv_obj_t * arc;
lv_obj_t *label;

void set_angle(int32_t v)
{
    lv_arc_set_value(arc, v);
    char buf[8];
    snprintf(buf, sizeof(buf), "%ld%%", v); 
    lv_label_set_text(label, buf);
}

void set_brightness(int brightness) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_RESOLUTION,
        .freq_hz          = LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = BACKLIGHT_PIN,
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .duty           = 0,             // Start with brightness off
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, brightness);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

/**
 * Create an arc which acts as a loader.
 */
void lv_example_arc_2(void)
{
    set_brightness(30);

    static lv_style_t style_label;
    lv_style_init(&style_label);
    lv_style_set_text_font(&style_label, &lv_font_montserrat_28);

    arc = lv_arc_create(lv_scr_act());
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_center(arc);

    label = lv_label_create(lv_scr_act());
    // lv_obj_set_size(label, 100, 100);
    lv_obj_add_style(label, &style_label, 0);
    lv_obj_center(label);
}


void IRAM_ATTR auto_switch(lv_timer_t * t)
{
  uint16_t page = lv_tabview_get_tab_act(tv);

  if (page == 0) { 
    lv_tabview_set_act(tv, 1, LV_ANIM_ON); 
  } else if (page == 3) {
    lv_tabview_set_act(tv, 2, LV_ANIM_ON); 
  }
}

void Lvgl_Example1(void) {

  disp_size = DISP_SMALL;                            

  font_large = LV_FONT_DEFAULT;                             
  font_normal = LV_FONT_DEFAULT;                         
  
  lv_coord_t tab_h;
  tab_h = 45;
  #if LV_FONT_MONTSERRAT_18
    font_large     = &lv_font_montserrat_18;
  #else
    LV_LOG_WARN("LV_FONT_MONTSERRAT_18 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
  #endif
  #if LV_FONT_MONTSERRAT_12
    font_normal    = &lv_font_montserrat_12;
  #else
    LV_LOG_WARN("LV_FONT_MONTSERRAT_12 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
  #endif

  lv_example_arc_2();
  
}



