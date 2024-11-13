#include <SparkFunBQ27441.h>
#include "esp_log.h"
#include "battery_utils.h"

// Set BATTERY_CAPACITY to the design capacity of your battery.
const unsigned int BATTERY_CAPACITY = 80;
const uint16_t TERMINATE_VOLTAGE = 3000;
const uint16_t TAPER_CURRENT = 105;

static const char *TAG = "BATTERY_UTILS";

void setupBatteryMeter(void)
{
  if (!lipo.begin()) { // begin() will return true if communication is successful
    ESP_LOGI(TAG, "Error: Unable to communicate with BQ27441.");
  }
  ESP_LOGI(TAG, "Connected to BQ27441!");
  lipo.enterConfig();  
  lipo.setCapacity(BATTERY_CAPACITY);
  lipo.setDesignEnergy(BATTERY_CAPACITY * 3.8f);
  lipo.setTerminateVoltage(TERMINATE_VOLTAGE);
  lipo.exitConfig(); 
}

unsigned int getBatteryState() {
  unsigned int soc = lipo.soc();                   // Read state-of-charge (%)
  unsigned int volts = lipo.voltage();             // Read battery voltage (mV)
  int current = lipo.current(AVG);                 // Read average current (mA)
  unsigned int fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
  unsigned int capacity = lipo.capacity(REMAIN);   // Read remaining capacity (mAh)
  int power = lipo.power();                        // Read average power draw (mW)
  int health = lipo.soh();                         // Read state-of-health (%)

  // Assemble a string to print
  String toPrint = "[" + String(millis() / 1000) + "] ";
  toPrint += String(soc) + "% | ";
  toPrint += String(volts) + " mV | ";
  toPrint += String(current) + " mA | ";
  toPrint += String(capacity) + " / ";
  toPrint += String(fullCapacity) + " mAh | ";
  toPrint += String(power) + " mW | ";
  toPrint += String(health) + "%";

  //fast charging allowed
  if (lipo.chgFlag())
      toPrint += " CHG";

  //full charge detected
  if (lipo.fcFlag())
      toPrint += " FC";

  //battery is discharging
  if (lipo.dsgFlag())
      toPrint += " DSG";

  // Print the string
  ESP_LOGI(TAG, "%s", toPrint.c_str());
  unsigned int percentage = ((volts - MIN_VOLTAGE) * 100) / (MAX_VOLTAGE - MIN_VOLTAGE);

    return percentage;
}
