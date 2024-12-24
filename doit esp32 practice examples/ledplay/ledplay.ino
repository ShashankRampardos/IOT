#include<WiFi.h>
#include<iostream>
const std::string ssid = "Scutum";//c++ ke adat ho gai
const std::string password = "00000111";

void setup(){
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connectint to wifi");
  WiFi.begin(ssid.c_str(),password.c_str());

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println();
  Serial.print("Connected to: ");
  Serial.println(ssid.c_str());
  Serial.print("localIp:");
  Serial.println(WiFi.localIP());
}

void loop(){
  //nothing to repeat
}