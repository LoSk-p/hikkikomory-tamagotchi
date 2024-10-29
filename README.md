# Hikkikomory Tamagochi

## Requirements

* ESP-IDF v5.1.4

## Setup

Set settings in menuconfig:
```bash
idf.py menuconfig
```
* In `Component config -> mbedTLS -> TLS Key Exchange Methods -> Enable pre-shared-key ciphersuites` check `Enable PSK based cyphersuite modes`.

* In `Component config -> ESP System Settings` set `Event loop task stack size` to 4096.

Save and quit.

In sdkconfig set 
```
CONFIG_FREERTOS_HZ=1000
```

## Build

```bash
idf.py -p <PORT> build flash
```