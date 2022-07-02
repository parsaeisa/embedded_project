#include <SPI.h> 
#include <Wire.h> 
#include "WiFi.h" 
#include "AsyncUDP.h" 
#include <ESPmDNS.h> 
#include <WiFiUdp.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
//#include <Adafruit_SSD1306.h> 
#define WLAN_SSID "PAKY" 
#define WLAN_PASS "qima4896" 
 
#define AIO_SERVER      "45.149.77.235" 
#define AIO_SERVERPORT  1883            
#define AIO_USERNAME    "97411252" 
#define AIO_KEY         "s1wnUViV" 
#define AIO_CID         "97411252Aspire123" 
#define BARCODE_MAX_LEN 30
#define LED1_PIN 23
#define LED2_PIN 22
#define LED3_PIN 1
#define LED4_PIN 3
WiFiClient client; 

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_CID, AIO_USERNAME, AIO_KEY); 
 
Adafruit_MQTT_Publish pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/scan");
Adafruit_MQTT_Subscribe sub = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/result"); 
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin) 
 
void MQTT_connect() { 
  int8_t ret; 
  // Stop if already connected. 
  if (mqtt.connected()) { 
    return; 
  } 
  Serial.print("Connecting to MQTT... "); 
  uint8_t retries = 3; 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected 
    Serial.println(mqtt.connectErrorString(ret)); 
    Serial.println("Retrying MQTT connection in 5 seconds..."); 
    mqtt.disconnect(); 
    delay(5000);  // wait 5 seconds 
    retries--; 
    if (retries == 0) { 
      // basically die and wait for WDT to reset me 
      while (1); 
    } 
  } 
  Serial.println("MQTT Connected!"); 
} 
 
char code[BARCODE_MAX_LEN] = {'\0'};
void scanBarcode(){
  Serial.println("Waiting for barcode!");
  int index=0;
  char temp=0;
  code[0] = '\0';
  Serial.print("code: ");
  while(1){
    if (Serial2.available() > 0)     {
      temp = char(Serial2.read());    //read the input data
      Serial.print(temp);
      code[index++] = temp;
    }
    if (index == BARCODE_MAX_LEN - 1 || (temp == 0x0D && index > 0)){
      code[index] = '\0';
      Serial.println(); 
      return;
    }
  }
}

void turnLedOnEx(int pinNumber){
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED4_PIN, LOW);
  digitalWrite(pinNumber, HIGH);
}

void setup() { 
  Serial.begin(9600);
  Serial2.begin(9600);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  
  WiFi.begin(WLAN_SSID, WLAN_PASS); 
  while (WiFi.status() != WL_CONNECTED && !Serial) { 
    delay(500); 
    Serial.println("."); 
  } 
     mqtt.subscribe(&sub); 
} 
 
void loop() { 
    
  MQTT_connect();   
  delay(50);
  scanBarcode();

  if (!pub.publish(code)) Serial.println(F("Failed")); 
  else Serial.println(F("Code published!"));
  
  Adafruit_MQTT_Subscribe *subscription;
  bool recieved = false;
  char* result;
  Serial.println(F("Waiting for response!"));
  while(!recieved){
    while ((subscription = mqtt.readSubscription(5000))) { 
      if (subscription == &sub) { 
        Serial.print(F("Response: "));
        result = (char *)sub.lastread; 
        Serial.println(result);
        recieved = true; 
      } 
    } 
  }
  if(result=="1") turnLedOnEx(LED1_PIN);
  else if(result=="2") turnLedOnEx(LED2_PIN);
  else if(result=="3") turnLedOnEx(LED3_PIN);
  else if(result=="4") turnLedOnEx(LED4_PIN);
 
  if (! mqtt.ping()) { 
    mqtt.disconnect(); 
  } 
 
  delay(2000);
}
