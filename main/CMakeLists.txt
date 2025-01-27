set(COMPONENT_REQUIRES "Crypto" "Arduino_JSON" "WebSockets" "mbedtls" "lvgl__lvgl" "SparkFun_BQ27441" "esp-robonomics-client")

idf_component_register(SRCS "main.cpp"
                       "LCD_Driver/Vernon_ST7789T/Vernon_ST7789T.c"
                       "LCD_Driver/ST7789.c"
                       "LVGL_Driver/LVGL_Driver.c"
                       "LVGL_UI/LVGL_Example.c"
                       "RGB/RGB.c"
                       "web_server.cpp"
                       "utils/nvs_utils.c"
                       "utils/battery_utils.cpp"
                    INCLUDE_DIRS ".")