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
#define MQTT_SERVER "ํYOUR_PUBLIC_IP"
#define MQTT_PORT 1883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define MQTT_NAME "ESP32_1"
#define MCP23017 0x20

#define trigPin 13
#define echoPin 27
#define SOUND_SPEED 0.034

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
  Wire.beginTransmission(MCP23017);  
  Wire.write(0x12);              
  Wire.write(0x00); 
  Wire.endTransmission();
}
void back_motorStop() {
  Wire.beginTransmission(MCP23017);  
  Wire.write(0x13);              
  Wire.write(0x00); 
  Wire.endTransmission();
}

void front_motorForward() {
  Serial.println("TOGGG");
  Wire.beginTransmission(MCP23017);  
  Wire.write(0x12);              
  Wire.write(0x05); //reverse bit
  Wire.endTransmission();
}
void back_motorForward() {
  Wire.beginTransmission(MCP23017);  
  Wire.write(0x13);              
  Wire.write(0x05); 
  Wire.endTransmission();
}

void front_motorbackForward() {
  Serial.println("TOGGG");
  Wire.beginTransmission(MCP23017);  
  Wire.write(0x12);              
  Wire.write(0x0A); //reverse bit
  Wire.endTransmission();
}
void back_motorbackForward() {
  Wire.beginTransmission(MCP23017);  
  Wire.write(0x13);              
  Wire.write(0x0A); 
  Wire.endTransmission();
}

void front_motorTurnleft() {
  Wire.beginTransmission(MCP23017);  
  Wire.write(0x12);              
  Wire.write(0x04); 
  Wire.endTransmission();
}

void back_motorTurnleft() {
  Wire.beginTransmission(MCP23017);  
  Wire.write(0x13);              
  Wire.write(0x04); 
  Wire.endTransmission();
}

float distanceCM_cal(long duration){
  return duration * SOUND_SPEED/2;
}
void ultraRead_Front(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  float distanceCm = distanceCM_cal(duration);

  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  delay(1000);
}

void ultraRead_Right(){

}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while (!Serial);
  Serial.println("\nI2C Scanner");
  //I2C SETUP
  setDirectionOutputPortA();
  setDirectionOutputPortB();

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  //MQTT Setup Section
  Serial.print("Connecting to ");
  WiFi.mode(WIFI_STA);
  WiFi.begin("Wifi", " wifipassword");
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
    ultraRead_Front();
    //front_motorForward();
    //back_motorForward();
    //front_motorTurnleft();
    //back_motorTurnleft();
  }else{
    front_motorStop();
    back_motorStop();

  }
}
