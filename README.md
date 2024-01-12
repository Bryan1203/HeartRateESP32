# ESP32 Bluetooth Heartrate Fan Speed Controller
# Introduction
This project recieves Bluetooth signals from heart rate monitor then displayed it on serial ports and adjust the fan speed through PWN. 

The buttons adjust the offset (fan speed) and the toggle switch between manual auto and neutral modes.

Manual mode set the fan speed by the offset value.
Auto set the fan speed through Heart Rate plus offset. 
Neutral mode stops the fan. 

The intension was to mimic somthing like Wahoo KICKR HEADWIND, this device also has the capability to send the recieved Heart Rate signals out for indoor cycling software such as Zwift, Rouvy, etc.

# Hardware and equiments
1x ESP32 dev board
1x 120mm 12V Fan with ARGB
1x 3 position toggle switch 
2x Panel Mount buttons
1x 12V DC source
1x DC panel mount female plug
some wires

![HRFAN_schamatic (1)](https://github.com/Bryan1203/HeartRateESP32/assets/12186042/f4938646-2c22-44fb-8327-9fb46cf81d0e)
