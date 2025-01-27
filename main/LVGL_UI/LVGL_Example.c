// #include "LVGL_Example.h"
// #include "driver/ledc.h"

// /**********************
//  *      TYPEDEFS
//  **********************/

// #define BACKLIGHT_PIN 22       // Set your backlight control pin
// #define LEDC_CHANNEL LEDC_CHANNEL_0
// #define LEDC_TIMER   LEDC_TIMER_0
// #define LEDC_MODE    LEDC_LOW_SPEED_MODE
// #define LEDC_FREQUENCY 5000   // 5 kHz frequency
// #define LEDC_RESOLUTION LEDC_TIMER_8_BIT

// typedef enum {
//     DISP_SMALL,
//     DISP_MEDIUM,
//     DISP_LARGE,
// } disp_size_t;

// /**********************
//  *  STATIC VARIABLES
//  **********************/
// static disp_size_t disp_size;

// static lv_obj_t * tv;

// static const lv_font_t * font_large;
// static const lv_font_t * font_normal;

// lv_obj_t * arc;
// lv_obj_t *label_battery;
// lv_obj_t *label_happiness;

// void set_lcd_counter(int32_t v)
// {
//     lv_arc_set_value(arc, v);
//     char buf[8];
//     snprintf(buf, sizeof(buf), "%ld%%", v); 
//     lv_label_set_text(label_happiness, buf);
// }

// void set_lcd_battery(unsigned int v) {
//     char buf[20];
//     snprintf(buf, sizeof(buf), "Battery: %u%%", v); 
//     lv_label_set_text(label_battery, buf);
// }

// void set_brightness(int brightness) {
//     ledc_timer_config_t ledc_timer = {
//         .speed_mode       = LEDC_MODE,
//         .timer_num        = LEDC_TIMER,
//         .duty_resolution  = LEDC_RESOLUTION,
//         .freq_hz          = LEDC_FREQUENCY,
//         .clk_cfg          = LEDC_AUTO_CLK
//     };
//     ledc_timer_config(&ledc_timer);
//     ledc_channel_config_t ledc_channel = {
//         .gpio_num       = BACKLIGHT_PIN,
//         .speed_mode     = LEDC_MODE,
//         .channel        = LEDC_CHANNEL,
//         .timer_sel      = LEDC_TIMER,
//         .duty           = 0,             // Start with brightness off
//         .hpoint         = 0
//     };
//     ledc_channel_config(&ledc_channel);

//     ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, brightness);
//     ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
// }

// /**
//  * Create an arc which acts as a loader.
//  */
// void lv_example_arc_2(void)
// {
//     set_brightness(30);

//     static lv_style_t style_label;
//     lv_style_init(&style_label);
//     lv_style_set_text_font(&style_label, &lv_font_montserrat_28);

//     arc = lv_arc_create(lv_scr_act());
//     lv_arc_set_rotation(arc, 270);
//     lv_arc_set_bg_angles(arc, 0, 360);
//     lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
//     lv_obj_center(arc);

//     label_happiness = lv_label_create(lv_scr_act());
//     // lv_obj_set_size(label, 100, 100);
//     lv_obj_add_style(label_happiness, &style_label, 0);
//     lv_obj_center(label_happiness);

//     label_battery = lv_label_create(lv_scr_act());
//     // lv_obj_set_size(label, 100, 100);
//     lv_obj_align(label_battery, LV_ALIGN_TOP_RIGHT, -10, 10);
// }

// void Lvgl_Example1(void) {

//   disp_size = DISP_SMALL;                            

//   font_large = LV_FONT_DEFAULT;                             
//   font_normal = LV_FONT_DEFAULT;                         
  
//   lv_coord_t tab_h;
//   tab_h = 45;
//   #if LV_FONT_MONTSERRAT_18
//     font_large     = &lv_font_montserrat_18;
//   #else
//     LV_LOG_WARN("LV_FONT_MONTSERRAT_18 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
//   #endif
//   #if LV_FONT_MONTSERRAT_12
//     font_normal    = &lv_font_montserrat_12;
//   #else
//     LV_LOG_WARN("LV_FONT_MONTSERRAT_12 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
//   #endif

//   lv_example_arc_2();
  
// }


#include "LVGL_Example.h"
#include "esp_log.h"
#include "driver/ledc.h"

static const char *TAG = "ESP_ZB_ON_OFF_LIGHT";

#define BACKLIGHT_PIN 22
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_TIMER   LEDC_TIMER_0
#define LEDC_MODE    LEDC_LOW_SPEED_MODE
#define LEDC_FREQUENCY 5000
#define LEDC_RESOLUTION LEDC_TIMER_8_BIT

static lv_obj_t *label_battery; // Battery level label
lv_obj_t *label_counter;
uint8_t current_screen_id = 0;
unsigned int last_battery_lvl = 100;

bool is_current_screen_main() {
    return current_screen_id == 1;
}

void set_lcd_battery(unsigned int* v) {
    last_battery_lvl = *v;
    char buf[20];
    snprintf(buf, sizeof(buf), "Battery: %u%%", *v); 
    lv_label_set_text(label_battery, buf);
}

void set_lcd_counter(uint16_t* v) {
    char buf[6];
    snprintf(buf, sizeof(buf), "%u", *v); 
    lv_label_set_text(label_counter, buf);
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
        .duty           = 0,
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, brightness);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

// Common function to create a battery label on the top-right corner
void create_battery_label(lv_obj_t *parent) {
    label_battery = lv_label_create(parent);
    lv_obj_align(label_battery, LV_ALIGN_TOP_RIGHT, -10, 10);
    set_lcd_battery(&last_battery_lvl);
}

// Screen 1: Big number in the center
void set_main_screen() {
    ESP_LOGI(TAG, "Main Screen");
    current_screen_id = 1;
    lv_obj_t *screen = lv_obj_create(NULL);
    label_counter = lv_label_create(screen);
    lv_label_set_text(label_counter, "0"); // Example number
    lv_obj_set_style_text_font(label_counter, &lv_font_montserrat_48, 0);
    lv_obj_center(label_counter);
    create_battery_label(screen);
    lv_scr_load(screen);
}

// Screen 2: "Setup WiFi info"
void set_wifi_setup_screen() {
    ESP_LOGI(TAG, "Setup WiFi Screen");
    current_screen_id = 2;
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_t *label_text = lv_label_create(screen);
    lv_label_set_text(label_text, "Setup WiFi info");
    // lv_obj_set_style_text_font(label_text, &lv_font_montserrat_28, 0);
    lv_obj_center(label_text);
    create_battery_label(screen);
    lv_scr_load(screen);
}

// Screen 3: "Sending Datalog..."
void set_sending_datalog_screen() {
    ESP_LOGI(TAG, "Sending Datalog Screen");
    current_screen_id = 3;
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_t *label_text = lv_label_create(screen);
    lv_label_set_text(label_text, "Sending Datalog...");
    // lv_obj_set_style_text_font(label_text, &lv_font_montserrat_28, 0);
    lv_obj_center(label_text);
    create_battery_label(screen);
    lv_scr_load(screen);
}

// Screen 4: "Datalog was sent"
void set_datalog_sent_screen(const char* tx_hash) {
    ESP_LOGI(TAG, "Datalog was sent Screen");
    current_screen_id = 4;

    lv_obj_t *screen = lv_obj_create(NULL);

    // Create a container to center both labels together
    lv_obj_t *container = lv_obj_create(screen);
    lv_obj_set_layout(container, LV_LAYOUT_FLEX); // Use flex layout for vertical alignment
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN); // Column layout
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling for the container
    lv_obj_center(container);

    // Set explicit size for the container (e.g., 100% of the screen size)
    lv_obj_set_size(container, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));

    // // Hide the container (make it invisible)
    // lv_obj_add_flag(container, LV_OBJ_FLAG_HIDDEN);

    // Create the first label and add it to the container
    lv_obj_t *label_text = lv_label_create(container);
    lv_label_set_text(label_text, "Datalog was sent");

    // Create the second label (tx_hash) and add it to the container
    lv_obj_t *label_tx_hash = lv_label_create(container);
    lv_label_set_text(label_tx_hash, tx_hash);
    lv_label_set_long_mode(label_tx_hash, LV_LABEL_LONG_WRAP); // Enable text wrapping
    lv_obj_set_width(label_tx_hash, lv_pct(90));              // Set width to 90% of the container

    // Create the battery label
    create_battery_label(screen);

    // Load the screen
    lv_scr_load(screen);
}


// // Function to switch between screens
// void switch_screen(int screen_id) {
//     // lv_obj_t *screen = lv_obj_create(NULL);

//     switch (screen_id) {
//         case 1: set_main_screen(); break;
//         case 2: create_screen2(); break;
//         case 3: create_screen3(); break;
//         case 4: create_screen4(); break;
//         default: return;
//     }

// }

// void lv_example_ui(void) {
//     set_brightness(30); // Set display brightness

//     // Start with screen 1
//     switch_screen(1);

//     // Example to switch screens after certain actions
//     // Uncomment to test:
//     vTaskDelay(pdMS_TO_TICKS(3000)); 
//     ESP_LOGI(TAG, "Screen 2");
//     switch_screen(2);
//     vTaskDelay(pdMS_TO_TICKS(3000)); 
//     ESP_LOGI(TAG, "Screen 3");
//     switch_screen(3);
//     vTaskDelay(pdMS_TO_TICKS(3000)); 
//     ESP_LOGI(TAG, "Screen 4");
//     switch_screen(4);
// }
