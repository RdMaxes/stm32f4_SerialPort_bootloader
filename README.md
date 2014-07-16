#STM32F40x_41x IAP practice
* IAP_Port        : USART2
* IAP_Port Setting: 230400bps, 8, n, 1
* Protocol        : Ymodem-1k
* IAP Start ADDR  : 0x08000000, size = 64KB
* User App ADDR   : 0x08010000, size = 960KB

* Description:

This project is modified from STMicrocontroller AN395 example.
After the MCU is reset, the main menu shows up and user has three selections:

1: Download user application program into FLASH

2: Upload user application program from FLASH (not supported in v1.01)

3: Run user application


* Revision:

2014-07-17 v1.01 released

provide basice IAP download function.

