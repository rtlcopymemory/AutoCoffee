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

## Modules

This project requires a 3.3V RTC I2C module.  
[I used this one](https://www.amazon.it/DS3231-Precisione-Module-Arduino-Raspberry/dp/B07Z4V6Y84/ref=sr_1_4?__mk_it_IT=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=2NPCMLX9XR22Q&dchild=1&keywords=rtc+arduino&qid=1594300862&sprefix=rtc+%2Caps%2C186&sr=8-4)
