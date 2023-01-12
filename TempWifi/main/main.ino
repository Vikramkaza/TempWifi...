#include "EEPROM.h"
#include "HTML.h"
#include <WebServer.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <OneWire.h> //http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <DallasTemperature.h> //  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <WiFi.h>
#include "Fetch.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include  <SPI.h>
#include "SPIFFS.h"
#include <JPEGDecoder.h>
#define DHTPIN 32
#define DHTTYPE  DHT22
boolean SPIFFSInited = false;
int imageIndex = 0;
const int oneWireBus = 33;// that is the Rx pin
int i=0;

#define minimum(a,b)     (((a) < (b)) ? (a) : (b))
float temperature_buiten;
DHT dht(DHTPIN, DHTTYPE, 15);
OneWire oneWire(oneWireBus); //oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass address of our oneWire instance to Dallas Temperature.
 
//DeviceAddress Probe02={0x28, 0x10, 0xA4, 0x57, 0x04, 0x00, 0x00, 0xA9};// if you want more sensors
#include <Fonts\comicbd7pt7b.h>
#include <Fonts\digital_77pt7b.h>
#include <Fonts\consola10pt7b.h>

WebServer server(80);
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

//char PHONE_1[21] = "8778601437";
#include "WiFiManager.h"


  unsigned long int timer;
  //long previousMillis = 0;
  int interval = 100; 



Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  
  pinMode(14,OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(1,INPUT); //for resetting WiFi creds
  EEPROM.begin(1000);
  Serial.begin(115200);
  sensors.begin();
  dht.begin();
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 5000)) ;
  if (!SPIFFS.begin()) {
      Serial.println("SPIFFS initialisation failed!");
      while (1) yield(); // Stay here twiddling thumbs waiting
    }
    SPIFFSInited = true;
 initTime("IST-5:30");   
 tft.begin();                   
  tft.setRotation(0);            
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(0,0,240,320,ILI9341_WHITE);

  delay(1000);
  if(!CheckWIFICreds())
  {
    Serial.println("No WIFI credentials stored in memory. Loading form...");
    digitalWrite(2,HIGH);
    while(loadWIFICredsForm());
    
  }else
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
      spinner();
      
      
      //printText("connection error", ILI9341_WHITE,15,130,2);
      Serial.println("Connecting to WiFi..");
       if(digitalRead(27) == HIGH)
       {
    Serial.println("Wiping WiFi credentials from memory...");
    wipeEEPROM();
    while(loadWIFICredsForm());
    }    
  }
   Serial.println("Connected to the WiFi network");
   
  }
      
  
  delay(1000);
  
}
void wipeEEPROM(){
  for(int i=0;i<1000;i++){
    EEPROM.writeByte(i,0);
  }
  EEPROM.commit();
}

void loop() {
 
  if(SPIFFSInited){

    String  fileIndex="/logo.jpg";
    drawFSJpeg(fileIndex.c_str(), 40, 20);
    

  }
  
  
  sensors.requestTemperatures(); 
  
  

 
  digitalWrite(2,HIGH);
  delay(1000);
  digitalWrite(2,LOW);
  delay(1000);
  Systemtemp();

    
}
void Systemtemp(){
  if(WiFi.status() != WL_CONNECTED){
    printText("connection error", ILI9341_WHITE,15,130,2);
    String s = EEPROM.readString(100);
    String p = EEPROM.readString(200);
     const char * ssid = s.c_str();
    const char * password = p.c_str();
    WiFi.begin(ssid, password);
  }
  if(WiFi.status() == WL_CONNECTED){
       
     tft.fillRect(15,112,220,20,ILI9341_BLACK);  
    initTime("IST-5:30");     
  if(digitalRead(27) == HIGH){
    Serial.println("Wiping WiFi credentials from memory...");
    wipeEEPROM();
    while(loadWIFICredsForm());
  }
    sensors.requestTemperatures(); 

    
    
        
    Serial.println("Requesting temperatures: ");
    Serial.println("Celcius Temperature="+String(sensors.getTempCByIndex(0))+"degC - Fahrenheit Temperature="+String(sensors.getTempFByIndex(0))+"F");
    

    float x = sensors.getTempCByIndex(0);
    Serial.println(x);
    float humidity = dht.readHumidity();
    Serial.println(humidity);
    String str2;
    String humid;
    str2 = String(x);
    humid = String(humidity);
    String mi = EEPROM.readString(500);
    String ma = EEPROM.readString(600); 
    String humin= EEPROM.readString(700);
    String humax= EEPROM.readString(800);

    char humidChar[10];
    char str2Char[10];
    char miChar[10];
    char maChar[10];
    char huminChar[10];
    char humaxChar[10];
    str2.toCharArray(str2Char,10);
    humid.toCharArray(humidChar,10);
    mi.toCharArray(miChar,10);
    ma.toCharArray(maChar,10);
    humin.toCharArray(huminChar,10);
    humax.toCharArray(humaxChar,10);

    
    /*printText("BetaMONKS", ILI9341_BLUE,55,55,3);
    tft.fillRect(50,175,150,40,ILI9341_BLACK);  
    printText(str2Char, ILI9341_WHITE,50,180,3);
    printText("o", ILI9341_WHITE,180,175,2);
    printText("C", ILI9341_WHITE,195,180,3);*/
    //printing minimum temperature
       
    float humaxx = humax.toFloat();    
    float huminn = humin.toFloat();
    float mii = mi.toFloat();
    float maa = ma.toFloat();
    Serial.println(mii);
    Serial.println(maa);
    if(x > maa || x < mii || humidity > humaxx || humidity < huminn  ){
      
    Serial.println(" temperature not in limits");
      
    tft.fillRect(10,137,220,180,ILI9341_BLACK);
    digitalWrite(14,HIGH);
    delay(500);
    digitalWrite(14,LOW);
    delay(500);
    digitalWrite(14,HIGH);
    delay(500);
    digitalWrite(14,LOW);
     
  if(SPIFFSInited){

   String  fileIndex="/logo.jpg";
    drawFSJpeg(fileIndex.c_str(), 40, 20);
    

  }
   
       
    printLocalTime();
    // printText("wrong temp", ILI9341_WHITE,15,130,2);
    String  fileIndex1="/caution.jpg";
    drawFSJpeg(fileIndex1.c_str(), 20, 135);
    
    // tft.fillRect(50,140,160,90,ILI9341_BLACK);  
     printText2(str2Char, ILI9341_GREEN,80,270,2);
     printText("o", ILI9341_WHITE,160,260,1);
     printText("c", ILI9341_WHITE,175,270,2);
 
     printText2(humidChar, ILI9341_GREEN,90,300,2);
     printText4("%", ILI9341_WHITE,155,300,1);


      }
    else{
       
       printLocalTime();
        tft.fillRect(10,137,220,180,ILI9341_BLACK);
        tft.drawRect(0,0,240,320,ILI9341_WHITE);
        
  //printText("TEMPERATURE", ILI9341_GREEN,20,130,2);
      printText("MAX", ILI9341_RED,173,240,1);
      printText("MIN", ILI9341_BLUE,37,240,1);
      float temperatureC = sensors.getTempCByIndex(0);
 //   float temperatureF = sensors.getTempFByIndex(0);
     Serial.print(temperatureC);
     Serial.println("ºC");
     float x =dht.readHumidity();
     Serial.println(x);
     String str2;
     String humid;
     char humidChar[10];
     char str2Char[10];
     humid = String(x);
     str2 = String(temperatureC);
     str2.toCharArray(str2Char,10);
     humid.toCharArray(humidChar,10);
    
     tft.fillRect(50,140,160,90,ILI9341_BLACK);  
     printText2(str2Char, ILI9341_GREEN,60,170,3);
     printText("o", ILI9341_WHITE,160,155,1);
     printText4("c", ILI9341_WHITE,175,170,2);
 
    printText2(humidChar, ILI9341_GREEN,65,220,3);
    printText4("%", ILI9341_WHITE,170,223,2);
    //minimum temperature
    printText(miChar, ILI9341_WHITE,35,270,1);
    printText("o", ILI9341_WHITE,60,260,1);
    printText("C", ILI9341_WHITE,72,270,1);
    //printing maximum temperature
    printText(maChar, ILI9341_WHITE,170,270,1);
    printText("o", ILI9341_WHITE,194,260,1);
    printText("C", ILI9341_WHITE,205,270,1);
    //minimum humidity
    printText(huminChar, ILI9341_WHITE,35,290,1);
   // printText("o", ILI9341_WHITE,60,280,1);
    printText4("%", ILI9341_WHITE,72,290,1);
    //maximum humidity
    printText(humaxChar, ILI9341_WHITE,170,290,1);
    //printText("o", ILI9341_WHITE,194,280,1);
    printText4("%", ILI9341_WHITE,205,290,1);
    
      //HTTPClient http;
      String d = EEPROM.readString(400); 
      String str1= "https://script.google.com/macros/s/AKfycbyiLZjvqk7S3EhYKEyMDsbFItAYRmKbfQqUYtXSYQSniNHglvhjwxTiuL5cqmtcsRJv/exec?Temperature=";
      String str3=str1+str2;
      String str8="&Humidity=";
      String str9=str3+str8;
      String str10=str9+humid;
      
      
      String str4= "&DeviceId=";
      String str5=str10+str4;
      String str6=str5+d;
     
      Serial.println(str6);
      
      RequestOptions options;
      options.method = "GET";
      const char * str7 = str6.c_str();
      
     
      if(i%(25) == 0){
      Response response = fetch(str7, options);
      Serial.println(str7);
      i=1;
      Serial.println(response);}
      else{
      
      
      i++;}
      
   
      Serial.println(i);
    delay(30000);
     }
    

}
else if(!CheckWIFICreds()){
    Serial.println("No WIFI credentials stored in memory. Loading form...");
    digitalWrite(2,HIGH);
    while(loadWIFICredsForm());
  
}
}
/*void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}*/
void initTime(String timezone){
  struct tm timeinfo;

  Serial.println("Setting up time");
  configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset
  if(!getLocalTime(&timeinfo)){
    Serial.println("  Failed to obtain time");
    return;
  }
  Serial.println("  Got the time from NTP");
  // Now we can set the real timezone
  setTimezone(timezone);
}
void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}
void spinner() {
  static int8_t counter = 0;
  const char* glyphs = "\xa1\xa5\xdb";

  if (counter == strlen(glyphs)) {
    counter = 0;
  }
}
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //LCD.setCursor(0, 1);
    //LCD.println("Connection Err");
    return;
  }
  
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  char minute[3];
  strftime(minute,3, "%M", &timeinfo);
  char Date[20];
 
  

  strftime(Date,20, "%d/%B/%Y", &timeinfo);
  tft.fillRect(50,60,150,50,ILI9341_BLACK);  
  
  printText1(timeHour, ILI9341_WHITE,90,85,2);
  printText1(":", ILI9341_WHITE,119,80,1);
  printText1(minute, ILI9341_WHITE,125,85,2);
  printText1(Date, ILI9341_WHITE,70,105,1);
  Serial.println(timeHour);
  Serial.println(minute);

}
void printText(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setFont (&comicbd7pt7b);
  
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}
void printText1(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setFont (&digital_77pt7b);
  tft.setTextColor(0x07FF);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}
void printText2(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setFont (&digital_77pt7b);
  
  tft.setTextColor(0xDB9A);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}
void printText4(char *text, uint16_t color, int x, int y,int textSize)
{
  tft.setCursor(x, y);
  tft.setFont (&consola10pt7b);  
  tft.setTextColor(0x07EF);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
}
//====================================================================================
//   This function opens the Filing System Jpeg image file and primes the decoder
//====================================================================================
void drawFSJpeg(const char *filename, int xpos, int ypos) {

  Serial.println("=====================================");
  Serial.print("Drawing file: "); Serial.println(filename);
  Serial.println("=====================================");

  // Open the file (the Jpeg decoder library will close it)
  fs::File jpgFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
  //  File jpgFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
 
  if ( !jpgFile ) {
    Serial.print("ERROR: File \""); Serial.print(filename); Serial.println ("\" not found!");
    return;
  }

  // To initialise the decoder and provide the file, we can use one of the three following methods:
  //boolean decoded = JpegDec.decodeFsFile(jpgFile); // We can pass the SPIFFS file handle to the decoder,
  //boolean decoded = JpegDec.decodeSdFile(jpgFile); // or we can pass the SD file handle to the decoder,
  boolean decoded = JpegDec.decodeFsFile(filename);  // or we can pass the filename (leading / distinguishes SPIFFS files)
                                                     // The filename can be a String or character array
  if (decoded) {
    // print information about the image to the serial port
    jpegInfo();

    // render the image onto the screen at given coordinates
    jpegRender(xpos, ypos);
  }
  else {
    Serial.println("Jpeg file format not supported!");
  }
}

//====================================================================================
//   Decode and paint onto the TFT screen
//====================================================================================
void jpegRender(int xpos, int ypos) {

  // retrieve infomration about the image
  uint16_t  *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while ( JpegDec.read()) {

    // save a pointer to the image block
    pImg = JpegDec.pImage;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      for (int h = 1; h < win_h-1; h++)
      {
        memcpy(pImg + h * win_w, pImg + (h + 1) * mcu_w, win_w << 1);
      }
    }


    // draw image MCU block only if it will fit on the screen
    if ( ( mcu_x + win_w) <= tft.width() && ( mcu_y + win_h) <= tft.height())
    {
      tft.drawRGBBitmap(mcu_x, mcu_y, pImg, win_w, win_h);
    }

    // Stop drawing blocks if the bottom of the screen has been reached,
    // the abort function will close the file
    else if ( ( mcu_y + win_h) >= tft.height()) JpegDec.abort();

  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime;

  // print the results to the serial port
  Serial.print  ("Total render time was    : "); Serial.print(drawTime); Serial.println(" ms");
  Serial.println("=====================================");

}

//====================================================================================
//   Send time taken to Serial port
//====================================================================================
void jpegInfo() {
  Serial.println(F("==============="));
  Serial.println(F("JPEG image info"));
  Serial.println(F("==============="));
  Serial.print(F(  "Width      :")); Serial.println(JpegDec.width);
  Serial.print(F(  "Height     :")); Serial.println(JpegDec.height);
  Serial.print(F(  "Components :")); Serial.println(JpegDec.comps);
  Serial.print(F(  "MCU / row  :")); Serial.println(JpegDec.MCUSPerRow);
  Serial.print(F(  "MCU / col  :")); Serial.println(JpegDec.MCUSPerCol);
  Serial.print(F(  "Scan type  :")); Serial.println(JpegDec.scanType);
  Serial.print(F(  "MCU width  :")); Serial.println(JpegDec.MCUWidth);
  Serial.print(F(  "MCU height :")); Serial.println(JpegDec.MCUHeight);
  Serial.println(F("==============="));
}

//====================================================================================
//   Open a Jpeg file and dump it to the Serial port as a C array
//====================================================================================
void createArray(const char *filename) {

  fs::File jpgFile;  // File handle reference for SPIFFS
  //  File jpgFile;  // File handle reference For SD library
  
  if ( !( jpgFile = SPIFFS.open( filename, "r"))) {
    Serial.println(F("JPEG file not found"));
    return;
  }

  uint8_t data;
  byte line_len = 0;
  Serial.println("// Generated by a JPEGDecoder library example sketch:");
  Serial.println("// https://github.com/Bodmer/JPEGDecoder");
  Serial.println("");
  Serial.println("#if defined(_AVR_)");
  Serial.println("  #include <avr/pgmspace.h>");
  Serial.println("#endif");
  Serial.println("");
  Serial.print("const uint8_t ");
  while (*filename != '.') Serial.print(*filename++);
  Serial.println("[] PROGMEM = {"); // PROGMEM added for AVR processors, it is ignored by Due

  while ( jpgFile.available()) {

    data = jpgFile.read();
    Serial.print("0x"); if (abs(data) < 16) Serial.print("0");
    Serial.print(data, HEX); Serial.print(",");// Add value and comma
    line_len++;
    if ( line_len >= 32) {
      line_len = 0;
      Serial.println();
    }

  }

  Serial.println("};\r\n");
  // jpgFile.seek( 0, SeekEnd);
  jpgFile.close();
}
//====================================================================================
