#include <Arduino.h>

#define AP_SSID "ESP32-Access-Point"

struct UserData {
    String ssid;
    String password;
    String owner_address;
    String host_url;
};

extern UserData user_data;

UserData get_wifi_creds_from_user(const char* robonomicsSs58Address);
