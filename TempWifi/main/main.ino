#include "EEPROM.h"
#include "HTML.h"
#include <WebServer.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Fetch.h"
#define ONE_WIRE_BUS 22        
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WebServer server(80);

#include "WiFiManager.h"


  unsigned long int timer;
  long previousMillis = 0;
  int interval = 10000; 
  

void setup() {
     
  pinMode(2, OUTPUT);
  pinMode(15,INPUT); //for resetting WiFi creds
  EEPROM.begin(600);
  Serial.begin(115200);
  if(!CheckWIFICreds()){
    Serial.println("No WIFI credentials stored in memory. Loading form...");
    digitalWrite(2,HIGH);
    while(loadWIFICredsForm());
  }
  else{
    String s = EEPROM.readString(100);
    String p = EEPROM.readString(200);
    String d = EEPROM.readString(400); 
    const char * ssid = s.c_str();
    const char * password = p.c_str();
  
    Serial.println(s+p);
    WiFi.begin(ssid, password);
   
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
    
    Serial.println("Connected to the WiFi network");
     Systemtemp();
   
  }
      
  
  
  
}
void wipeEEPROM(){
  for(int i=0;i<600;i++){
    EEPROM.writeByte(i,0);
  }
  EEPROM.commit();
}

void loop() {
 
  digitalWrite(2,HIGH);
  delay(1000);
  digitalWrite(2,LOW);
  delay(1000);
  
}
void Systemtemp(){
  if(WiFi.status() == WL_CONNECTED){
     
  if(digitalRead(15) == HIGH){
    Serial.println("Wiping WiFi credentials from memory...");
    wipeEEPROM();
    while(loadWIFICredsForm());
  }
  
  if (millis() - previousMillis > interval) 
    {
    sensors.requestTemperatures();
    }

    Serial.println("Requesting temperatures: ");
    Serial.println("Celcius Temperature="+String(sensors.getTempCByIndex(0))+"degC - Fahrenheit Temperature="+String(sensors.getTempFByIndex(0))+"F");
    

    float x = sensors.getTempCByIndex(0);
    String str2;
    str2 = String(x);
   
  
    
    if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
      
      //HTTPClient http;
      String d = EEPROM.readString(400); 
      String str1= "https://script.google.com/macros/s/AKfycbxDDwlLMk5pNpAJ98ALu_JxbJqt6cJusyQkcVHUjep78tPgtReJExFkP5fPOhOSB_3K/exec?Temperature=";
      String str3=str1+str2;
      String str4= "&DeviceId=";
      String str5=str4+d;
      String str6=str3+str5;
     
      Serial.println(str6);
      
      RequestOptions options;
      options.method = "GET";
      const char * str7 = str6.c_str();
      
     
      Response response = fetch(str7, options);
      Serial.println(str7);
     
      Serial.println(response);
      
      
      previousMillis = millis();
      delay(1000);
   
      Systemtemp();
      
}
}
else if(!CheckWIFICreds()){
    Serial.println("No WIFI credentials stored in memory. Loading form...");
    digitalWrite(2,HIGH);
    while(loadWIFICredsForm());
  
}
}

