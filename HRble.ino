#include <ArduinoBLE.h>
#include <FastLED.h>
#include <algorithm>
#include <iostream>

/* led define */
FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    12
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    16
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          100
#define FRAMES_PER_SECOND  120
CRGB colorStart = CRGB::Green;
CRGB colorEnd = CRGB::Red;
float stepR = (colorEnd.r - colorStart.r) / float(255);
float stepG = (colorEnd.g - colorStart.g) / float(255);
float stepB = (colorEnd.b - colorStart.b) / float(255);
/* led define end*/

/* Device name which can be scene in BLE scanning software. */
#define BLE_DEVICE_NAME               "ESP32 BLE Sense"
/* Local name which should pop up when scanning for BLE devices. */
#define BLE_LOCAL_NAME                "ESP32 HR monitor"


#define MIN_HR 45
#define MAX_HR 210
#define FAN_PWM_PIN 13
#define PLUS_PIN 14
#define MINUS_PIN 27
#define AUTO 26
#define MANUAL 25

/* define global variables */
volatile uint8_t duty_cycle;
volatile int offset = 170;
//determine if using HR to control the fan; default to false
//volatile bool automatic = !digitalRead(AUTO);
volatile bool neutral = true;

BLEService HeartRateService("180D");
BLECharacteristic HeartRateMeasurement("2A37", BLENotify, 6);
BLEDevice peripheral;

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

void IRAM_ATTR plusISR(){
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
  
    if (offset+10 > 255){
      offset = 255;
      } else{
        offset += 10;
        }
    Serial.println("plus offset");
    last_button_time = button_time;

  }
}
void IRAM_ATTR minusISR(){
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    if (offset-10 < 0){
      offset = 0;
      } else{
        offset -= 10;
        }
     Serial.println("minus offset");
     last_button_time = button_time;

  }
  
}
void IRAM_ATTR autoISR(){
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    Serial.println("Entered Auto Mode");
//    analogWrite(FAN_PWM_PIN, 0);
//    automatic = true; 
//    neutral = false;
    last_button_time = button_time;

  }
}

void IRAM_ATTR manualISR(){button_time = millis();
   button_time = millis();
  if (button_time - last_button_time > 250)
  {
    Serial.println("Entered Manual Mode");
//    peripheral.disconnect();
//    automatic = false;
//    neutral = false;
    last_button_time = button_time;

  }
}
void IRAM_ATTR neutralISR(){
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    Serial.println("Entered Neutral Mode");
//    peripheral.disconnect();
//    automatic = false;
//    neutral = true;
    last_button_time = button_time;

  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  Serial.println("BLE Central for Garmin HRM");
  
  
  BLE.setDeviceName(BLE_DEVICE_NAME);
  BLE.setLocalName(BLE_LOCAL_NAME);
  BLE.setAdvertisedService(HeartRateService);
  HeartRateService.addCharacteristic(HeartRateMeasurement);
  BLE.addService(HeartRateService);
  BLE.advertise();
  Serial.println("Center established");
  BLE.scanForUuid("180D");

  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(PLUS_PIN,INPUT_PULLUP);
  pinMode(MINUS_PIN,INPUT_PULLUP);
  pinMode(AUTO,INPUT_PULLUP);
  pinMode(MANUAL,INPUT_PULLUP);

  attachInterrupt(PLUS_PIN, plusISR, CHANGE);
  attachInterrupt(MINUS_PIN, minusISR, FALLING);
  attachInterrupt(AUTO, autoISR, FALLING);
  attachInterrupt(MANUAL, manualISR, FALLING);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


void loop() {
  //BLE.scanForUuid("180D");
  peripheral = BLE.available();
  BLEDevice central = BLE.central();
  
  if (!digitalRead(AUTO)&&!(digitalRead(AUTO)&&digitalRead(MANUAL))) {
    //Serial.println("Auto Heart Rate Mode: ");
    analogWrite(FAN_PWM_PIN, 0);
    if (peripheral){
                
                Serial.print("Found ");
                Serial.print(peripheral.address());
                Serial.print(" '");
                Serial.print(peripheral.localName());
                Serial.print("' ");
                Serial.print(peripheral.advertisedServiceUuid());
                Serial.println();
                if (peripheral.localName() == "HRM-Dual:084939") { //name of BLE HRM device
                  BLE.stopScan();
                  if (peripheral.connect())
                    Serial.print("Connected to Garmin HRM ");
                  Serial.println("Discovering attributes ...");
                  if (peripheral.discoverAttributes()) { //find services and characterisitics
                    Serial.println("Attributes discovered");
                    BLEService service = peripheral.service("180d");
                    BLECharacteristic characteristic = service.characteristic("2a37");
                    characteristic.subscribe();
                    Serial.println("subscribed to 2a37");
                    
                  } else {
                    Serial.println("Attribute discovery failed!");
                    peripheral.disconnect();
                    setup();
                    return;
                  }
                }
                else {
                  Serial.println("Failed to connect!");
                  return;
                }
            
                while (peripheral.connected()&!digitalRead(AUTO)&&!(digitalRead(AUTO)&&digitalRead(MANUAL)))
                {
                  BLEService service = peripheral.service("180d");
                  BLECharacteristic characteristic = service.characteristic("2a37");
                  
                  if (characteristic.valueUpdated())
                  {
                    Serial.println("value updated (no device)");
                    uint8_t value[6];
                    characteristic.readValue(value,6);
                    Serial.println(value[1]);
                    //change the fan speed to the corresponding HR value
                    //map the HR to 0 - 100 that corresponding to the duty cycle (PWN) fan speed
                    Serial.print("Fan PWM 0-255: ");
                    duty_cycle = min((int)(((double)(value[1]-MIN_HR)/(MAX_HR-MIN_HR))*255)+offset,255);
                    
                    Serial.println(duty_cycle);
                    analogWrite(FAN_PWM_PIN, duty_cycle);
                    fill_solid(leds,NUM_LEDS, CRGB(colorStart.r + int(stepR * duty_cycle), colorStart.g + int(stepG * duty_cycle), colorStart.b + int(stepB * duty_cycle)));
                    FastLED.show();
                    if(BLE.central())
                    {     
                      /* 
                      If a BLE device is connected, magnetic data will start being read, 
                      and the data will be processed
                      */
                      Serial.println("device connected");
                      characteristic.readValue(value,6);
                      Serial.println("value updated (device connected)");
                      Serial.println(value[1]);
                      
//                      HeartRateMeasurement.writeValue(value, 6);
//                      Serial.println("value sent to device");
                      //change the fan speed to the corresponding HR value
                      //map the HR to 0 - 100 that corresponding to the duty cycle (PWN) fan speed
                      Serial.print("Fan PWM 0-255: ");
                      duty_cycle = min((int)(((double)(value[1]-MIN_HR)/(MAX_HR-MIN_HR))*255)+offset,255);
                      
                      Serial.println(duty_cycle);
                      analogWrite(FAN_PWM_PIN, duty_cycle);
                      fill_solid(leds,NUM_LEDS, CRGB(colorStart.r + int(stepR * duty_cycle), colorStart.g + int(stepG * duty_cycle), colorStart.b + int(stepB * duty_cycle)));
                      FastLED.show();
                        
                    }
                  }
                  //Serial.println();
                  delay(1000);
                  /*
                  if(BLE.disconnect()&&peripheral.localName() == "HRM-Dual:084939")
                  {
                    peripheral.connect();
                    }
                    */
                  
                }
                Serial.println("disconnected to HRM");
                peripheral.disconnect();
                setup();
                //BLE.scanForUuid("180D");
                }
           /* manual mode */     
          }else if (!digitalRead(MANUAL)&&!(digitalRead(AUTO)&&digitalRead(MANUAL))){
            //Serial.println("Manual Mode:");
            delay(1000);
            Serial.print("Fan PWM 0-255: ");
            duty_cycle = offset;
            Serial.println(duty_cycle);
            analogWrite(FAN_PWM_PIN, duty_cycle);
            fill_solid(leds,NUM_LEDS, CRGB(colorStart.r + int(stepR * duty_cycle), colorStart.g + int(stepG * duty_cycle), colorStart.b + int(stepB * duty_cycle)));
            FastLED.show();
            
            } 
            /* neutral mode */
            else if (digitalRead(AUTO)&&digitalRead(MANUAL)){
              Serial.println("Neutral Mode:");
              analogWrite(FAN_PWM_PIN, 0);
              fill_solid(leds,NUM_LEDS,CRGB::Black);
              FastLED.show();
              delay(1000);
            }            
}



  