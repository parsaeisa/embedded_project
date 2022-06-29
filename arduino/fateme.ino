#include <SPI.h> 
#include <Wire.h> 
#include "WiFi.h" 
#include "AsyncUDP.h" 
#include <ESPmDNS.h> 
#include <WiFiUdp.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
#include <Adafruit_SSD1306.h> 
#define WLAN_SSID "AndroidAP4D70" 
#define WLAN_PASS "fkat5043" 
 
#define AIO_SERVER      "45.149.77.235" 
#define AIO_SERVERPORT  1883            
#define AIO_USERNAME    "97411252" 
#define AIO_KEY         "s1wnUViV" 
#define AIO_CID         "97411252Aspire123" 
WiFiClient client; 

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_CID, AIO_USERNAME, AIO_KEY); 
 
Adafruit_MQTT_Publish pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/test/1");
 
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin) 
 
int sensorValue; 
 
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
 
 
void setup() { 
  Serial.begin(115200); 
 
  WiFi.begin(WLAN_SSID, WLAN_PASS); 
  while (WiFi.status() != WL_CONNECTED && !Serial) { 
    delay(500); 
    Serial.println("."); 
  } 
   
} 
 
void loop() { 
     
  MQTT_connect();   
  delay(50); 
  sensorValue = 2 ;  

  Serial.println(sensorValue); 
  if (!pub.publish(sensorValue)) { 
    Serial.println(F("Failed")); 
  } else { 
    Serial.println(F("OK!"));
    Serial.println(sensorValue);  
  } 
 
  if (! mqtt.ping()) { 
    mqtt.disconnect(); 
  } 
 
  delay(2000); 
}
