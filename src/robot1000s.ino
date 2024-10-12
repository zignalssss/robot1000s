/*
  Project:robot1000s
  Date:8/7/2024 -> Present
*/

//import Library
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

//End import Library

//Define Global Variable SECTION (preprocessor)
#define MQTT_SERVER "20.2.222.63"
#define MQTT_PORT 1883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define MQTT_NAME "ESP32_1"
#define MCP23017 0x20

#define IN1 
#define IN2 
#define IN3 
#define IN4 

//End Define Vaiable SECTION

//Define MQTT BROKER
WiFiClient client;
PubSubClient mqtt(client);
//End Define MQTT BROKER

//global variable section
bool isToggle = false;



//End global variable section

//callback function for mqttbroker
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  if (String(topic) == "/ESP32_1/STATUS") {
    Serial.print("Changing output to ");
    if(messageTemp == "toggle"){
      Serial.println("Motor is Toggle!");
      isToggle = true;
    }else{
      Serial.println("Motor is Stop!");
      isToggle = false;
    }
  }
}
void setDirectionOutputPortA() {
  Wire.beginTransmission(MCP23017);  
  Wire.write(0x00);             
  Wire.write(0x00); 
  Wire.endTransmission(); 

}
void setDirectionOutputPortB() {
  Wire.beginTransmission(MCP23017);  
  Wire.write(0x01);              
  Wire.write(0x00); 
  Wire.endTransmission(); 
}

void front_motorStop() {
  Wire.beginTransmission(0x20);  
  Wire.write(0x12);              
  Wire.write(0x00); 
  Wire.endTransmission();
}
void back_motorStop() {
  Wire.beginTransmission(0x20);  
  Wire.write(0x13);              
  Wire.write(0x00); 
  Wire.endTransmission();
}

void front_motorForward() {
  Wire.beginTransmission(0x20);  
  Wire.write(0x12);              
  Wire.write(0xA0); 
  Wire.endTransmission();
}
void back_motorForward() {
  Wire.beginTransmission(0x20);  
  Wire.write(0x13);              
  Wire.write(0xA0); 
  Wire.endTransmission();
}

void front_motorTurnleft() {
  Wire.beginTransmission(0x20);  
  Wire.write(0x12);              
  Wire.write(0x10); 
  Wire.endTransmission();
}

void back_motorTurnleft() {
  Wire.beginTransmission(0x20);  
  Wire.write(0x12);              
  Wire.write(0x10); 
  Wire.endTransmission();
}


void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (!Serial);
  Serial.println("\nI2C Scanner");
  //I2C SETUP
  setDirectionOutputPortA();
  setDirectionOutputPortB();
  //SETUP OUTPUT



  //End SETUP OUTPUT

  //SETUP INPUT


  //End SETUP INPUT

  //MQTT Setup Section
  Serial.print("Connecting to ");
  WiFi.mode(WIFI_STA);
  WiFi.begin("zigzigbotbot", "123456;)");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);
  //End MQTT Setup Section
}
void loop() {
  if (mqtt.connected() == false) {
    Serial.print("MQTT connection... ");
    if (mqtt.connect(MQTT_NAME, MQTT_USERNAME,
                     MQTT_PASSWORD)) {
      Serial.println("connected");
      mqtt.subscribe("/ESP32_1/STATUS");
    } else {
      Serial.println("failed");
      delay(5000);
    }
  } else {
    mqtt.loop();
  }
  if(isToggle){
    Serial.println("TOGGG");
    Wire.beginTransmission(MCP23017);  
    Wire.write(0x12);              
    Wire.write(0x05); //reverse bit
    Wire.endTransmission();
  }else{
     Wire.beginTransmission(MCP23017);  
    Wire.write(0x12);              
    Wire.write(0x00); 
    Wire.endTransmission();
  }
}
