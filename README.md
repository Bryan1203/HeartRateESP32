# ESP32 Bluetooth Heartrate Fan Speed Controller
# Introduction
This project recieves Bluetooth signals from heart rate monitor then displayed it on serial ports and adjust the fan speed through PWN. 

The buttons adjust the offset (fan speed) and the toggle switch swtiches between manual, auto, and neutral modes.

Manual mode set the fan speed by the offset value.
Auto set the fan speed through Heart Rate plus offset. 
Neutral mode stops the fan. 

The intension was to mimic somthing like Wahoo KICKR HEADWIND, this device also has the capability to send the recieved Heart Rate signals out for indoor cycling software such as Zwift, Rouvy, etc.

# Hardware and equiments
1x ESP32 dev board  
1x 120mm 12V Fan with ARGB  
1x Bluetooth Heart Rate Band or Smart Watch that could broadcast HR data through Bluetooth  
1x 3 position toggle switch  
2x Panel Mount buttons  
1x 12V DC source  
1x DC panel mount female plug  
some wires  

# Schematic
![HRFAN_schamatic (1)](https://github.com/Bryan1203/HeartRateESP32/assets/12186042/f4938646-2c22-44fb-8327-9fb46cf81d0e)

# Building it up!!!
It's time to build it up! Follow the schematic shown above, then test it if it works. Then, for the casing I search up something related to this project and I found [this](https://www.instructables.com/Neo-Desktop-Fan/?fbclid=IwAR02mM12bzN0vI-XH6ynZ7b5XTv7ZkDJrPBm9M5zgRpZU5WP5N7Cm5cuvWM). But since I am using a larger version of esp32, I had to redesign the bottom container and add four more holes for the buttons, switch, and the DC barrel. Here are the printed parts.

<img src="https://github.com/Bryan1203/HeartRateESP32/assets/12186042/26e9dead-a86d-44f6-8ae9-a059a2fb35b0">

<img src="https://github.com/Bryan1203/HeartRateESP32/assets/12186042/bbc56e2e-46ee-4520-aa68-a9be02265d58">

<img src="https://github.com/Bryan1203/HeartRateESP32/assets/12186042/9935da17-5fe4-4181-bd22-1174d5838864">

Then install the ESP32, buttons, switch, and the barrel plug to the container. 

<img src="https://github.com/Bryan1203/HeartRateESP32/assets/12186042/d4cac89a-4931-4bf3-8f6e-e45c951d0df7">
Finally, screw the screws and close the container. 

<img src="https://github.com/Bryan1203/HeartRateESP32/assets/12186042/3757f798-38b6-43c4-a1dc-723aa2297d30">


