# AutoCoffee

NodeMCU ESP8266 personal project to automate my own Coffee machine

## Wifi Credentials

The wifi credentials are saved in a file inside the `AutoCoffee` folder named `credentials.h` in the format of:

```c
#ifndef __credentials__
#define __credentials__

#define SSID PUT_SSID_HERE
#define PASSWORD PUT_PASSWORD_HERE

#endif
```

## Circuit

This uses a relay module at 3.3V. Here's the link for the exact module: [1 Channel 3.3V Low Level Trigger Relay Module Optocoupler Isolation Terminal BESTEP for Arduino - products that work with official Arduino boards](https://www.banggood.com/BESTEP-1-Channel-3_3V-Low-Level-Trigger-Relay-Module-Optocoupler-Isolation-Terminal-p-1355736.html?rmmds=search&cur_warehouse=CN)
