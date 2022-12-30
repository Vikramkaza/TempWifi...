#include "EEPROM.h"
#include "HTML.h"
#include <WebServer.h>
#include <WiFi.h>
#include <HTTPClient.h>
//#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//#include <Adafruit_FONA.h>
#include "Fetch.h"
#define ONE_WIRE_BUS 15
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include  <SPI.h>      
//#include <Adafruit_ILI9341_Albert.h>
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WebServer server(80);
//#define FONA_RX            2
//#define FONA_TX            3
//#define FONA_RST           4
//#define FONA_RI_INTERRUPT  0
#define TFT_DC 2
#define TFT_CS 16
#define TFT_RST 4
#define TFT_MISO 23         
#define TFT_MOSI 19           
#define TFT_CLK 18

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     0
#define UTC_OFFSET_DST 0

//SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
 
//Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
//SoftwareSerial mySerial(11,12);

char PHONE_1[21] = "8778601437";
#include "WiFiManager.h"


  unsigned long int timer;
  long previousMillis = 0;
  int interval = 100; 
  
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
void setup() {
     
  pinMode(2, OUTPUT);
  pinMode(5,INPUT); //for resetting WiFi creds
  EEPROM.begin(800);
  Serial.begin(115200);
  tft.begin();                   
  tft.setRotation(0);            
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(0,0,319,240,ILI9341_WHITE);
  Wire.begin();
  printText("TEMPERATURE", ILI9341_GREEN,20,130,3);
  printText("MAX", ILI9341_RED,173,260,2);
  printText("MIN", ILI9341_BLUE,37,260,2);
  if(!CheckWIFICreds())
  {
    Serial.println("No WIFI credentials stored in memory. Loading form...");
    digitalWrite(2,HIGH);
    while(loadWIFICredsForm());
    
  }
  else
  {
    String s = EEPROM.readString(100);
    String p = EEPROM.readString(200);
    String d = EEPROM.readString(400); 
    String mi = EEPROM.readString(500);
    String ma = EEPROM.readString(600);
    const char * ssid = s.c_str();
    const char * password = p.c_str();
  
    Serial.println(s+p);
    WiFi.begin(ssid, password);
   
    while (WiFi.status() != WL_CONNECTED) 
    {
      
      delay(1000);
      printText("connection error", ILI9341_WHITE,50,180,1);
      Serial.println("Connecting to WiFi..");
       if(digitalRead(5) == HIGH)
       {
    Serial.println("Wiping WiFi credentials from memory...");
    wipeEEPROM();
    while(loadWIFICredsForm());
    }
    
   
     
    
  }
   Serial.println("Connected to the WiFi network");
   
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
  Systemtemp();
}
void Systemtemp(){
  if(WiFi.status() == WL_CONNECTED){
     
  if(digitalRead(5) == HIGH){
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
    Serial.println(x);

    String str2;
    str2 = String(x);
    String mi = EEPROM.readString(500);
    String ma = EEPROM.readString(600); 

    char str2Char[10];
    char miChar[10];
    char maChar[10];
    str2.toCharArray(str2Char,10);
    mi.toCharArray(miChar,10);
    ma.toCharArray(maChar,10);

    
    printText("BetaMONKS", ILI9341_BLUE,55,55,3);
    tft.fillRect(50,175,150,40,ILI9341_BLACK);  
    printText(str2Char, ILI9341_WHITE,50,180,3);
    printText("o", ILI9341_WHITE,180,175,2);
    printText("C", ILI9341_WHITE,195,180,3);
    //printing minimum temperature
    
    tft.fillRect(8,280,90,28,ILI9341_BLACK);
    printText(miChar, ILI9341_WHITE,35,290,2);
    printText("o", ILI9341_WHITE,60,280,2);
    printText("C", ILI9341_WHITE,75,290,2);
    //printing maximum temperature
    tft.fillRect(145,280,90,28,ILI9341_BLACK);
    printText(maChar, ILI9341_WHITE,170,290,2);
    printText("o", ILI9341_WHITE,194,280,2);
    printText("C", ILI9341_WHITE,208,290,2);
    
    
    float mii = mi.toFloat();
    float maa = ma.toFloat();
    Serial.println(mii);
    Serial.println(maa);
    if(x > maa || x < mii){
      
      Serial.println(" temperature not in limits");
      //SendMessage();
      
      }
    
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
      delay(100);
   
      
      
}
}
else if(!CheckWIFICreds()){
    Serial.println("No WIFI credentials stored in memory. Loading form...");
    digitalWrite(2,HIGH);
    while(loadWIFICredsForm());
  
}
}
void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}
