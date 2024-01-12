# ESP32 Bluetooth Heartrate Fan Speed Controller
Code that connects to garmin HRM via bluetooth and shows the heartrate data on serial port
This code example demonstrates how to establish a Bluetooth connection between an ESP32 microcontroller and a heart rate monitor, in this case a Garmin HRM. 

It also sends duty cycle corresponds to the HR data to pin 13 on ESP32.

The buttons adjust the offset (fan speed) and the toggle switch between manual auto and neutral modes. Manual mode set the fan speed by the offset value. Auto set the fan speed through Heart Rate plus offset. Neutral mode stops the fan. 

The intension was to mimic somthing like Wahoo KICKR HEADWIND, this device also has the capability to send the recieved Heart Rate signals out for indoor cycling software such as Zwift, Rouvy, etc.

![HRFAN_schamatic (1)](https://github.com/Bryan1203/HeartRateESP32/assets/12186042/f4938646-2c22-44fb-8327-9fb46cf81d0e)
