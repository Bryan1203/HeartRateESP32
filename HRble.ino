#include <ArduinoBLE.h>

/* Device name which can be scene in BLE scanning software. */
#define BLE_DEVICE_NAME               "ESP32 BLE Sense"
/* Local name which should pop up when scanning for BLE devices. */
#define BLE_LOCAL_NAME                "ESP32 HR monitor"

BLEService HeartRateService("180D");
BLECharacteristic HeartRateMeasurement("2A37", BLENotify, 6);

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

  
}


void loop() {
  //BLE.scanForUuid("180D");
  BLEDevice peripheral = BLE.available();
  BLEDevice central = BLE.central();
  if (peripheral) {
                Serial.print("Found ");
                Serial.print(peripheral.address());
                Serial.print(" '");
                Serial.print(peripheral.localName());
                Serial.print("' ");
                Serial.print(peripheral.advertisedServiceUuid());
                Serial.println();
                if (peripheral.localName() == "HRM-Dual:084939") { //name of simulated Polar HR device
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
            
                while (peripheral.connected())
                {
                  BLEService service = peripheral.service("180d");
                  BLECharacteristic characteristic = service.characteristic("2a37");
                  
                  if (characteristic.valueUpdated())
                  {
                    Serial.println("value updated (no device)");
                    uint8_t value[6];
                    characteristic.readValue(value,6);
                    Serial.println(value[1]);
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
                      HeartRateMeasurement.writeValue(value, 6);
                      Serial.println("value sent to device");
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
                setup();
                //BLE.scanForUuid("180D");
                
              }
              //Serial.println("after if ");
            
}

  
