#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>

#include <Servo.h>
#include "credentials.h"

#define ServoPin2 D5 //D4

// prototypes
boolean connectWifi();


//callback functions
void servoChanged(uint8_t brightness);

// Change this!!
const char* ssid = SSID;
const char* password = PASSWD;

boolean wifiConnected = false;
String Window_AC = "Window AC";

// Vars for maing a delay
// ========================
boolean delayFlag = false;
int period = 1000;
unsigned long time_now = 0;
// ========================

Espalexa espalexa;
Servo servo_mtr;

void setup()
{
  servo_mtr.attach(ServoPin2, 500, 2400);
  servo_mtr.write(90);
  
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    
    // Define your devices here. 
    espalexa.addDevice(Window_AC, servoChanged);
    espalexa.begin();
    
  } else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Restarting ESP");
      delay(2500);
      ESP.restart();
    }
  }
}
 
void loop()
{
   espalexa.loop();
   delay(1);
}

//our callback functions
void servoChanged(uint8_t brightness)
{
  Serial.print("brightness: ");
  Serial.println(brightness);
  delayFlag = true;

  if(brightness == 0){
    servo_mtr.write(0);
    Serial.println("AC OFF");
  }
  else if(brightness == 255){
    servo_mtr.write(175);
    Serial.println("AC ON");
  }
  else{
    Serial.println("Command Not Supported!");
  }
  time_now = millis();
}


// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
