void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

/*
 * Function for writing WiFi creds to EEPROM
 * Returns: true if save successful, false if unsuccessful
 */
bool writeToMemory(String ssid, String pass, String device, String minimum, String maximum,String humimin, String humimax){
  char buff1[30];
  char buff2[30];
  char buff3[30];
  char buff4[30];
  char buff5[30];
  char buff6[30];
  char buff7[30];
  ssid.toCharArray(buff1,30);
  pass.toCharArray(buff2,30);
  device.toCharArray(buff3,30);
  minimum.toCharArray(buff4,30);
  maximum.toCharArray(buff5,30);
  humimin.toCharArray(buff6,30);
  humimax.toCharArray(buff7,30);
  EEPROM.writeString(100,buff1);
  EEPROM.writeString(200,buff2);
  EEPROM.writeString(400,buff3);
  EEPROM.writeString(500,buff4);
  EEPROM.writeString(600,buff5);
  EEPROM.writeString(700,buff6);
  EEPROM.writeString(800,buff7);
  
  delay(100);
  String s = EEPROM.readString(100);
  String p = EEPROM.readString(200);
  String d = EEPROM.readString(400);
  String mi= EEPROM.readString(500);
  String ma= EEPROM.readString(600);
  String humin= EEPROM.readString(700);
  String humax= EEPROM.readString(800);
  
  //#if DEBUG
  Serial.print("Stored SSID, password,deviceid are: ");
  Serial.print(s);
  Serial.print(" / ");
  Serial.print(p);
  Serial.print(" / ");
  Serial.print(d);
  Serial.print(" / ");
  Serial.print(mi);
  Serial.print(" / ");
  Serial.print(ma);
  Serial.print(" / ");
  Serial.print(humin);
  Serial.print(" / ");
  Serial.print(humax);
  //#endif
  if(ssid == s && pass == p && device ==d && minimum==mi && maximum==ma && humimin==humin && humimax==humax){
    return true;  
  }else{
    return false;
  }
}


/*
 * Function for handling form
 */
void handleSubmit(){
  String response_success="<h1>Success</h1>";
  response_success +="<h2>Device will restart in 3 seconds</h2>";

  String response_error="<h1>Error</h1>";
  response_error +="<h2><a href='/'>Go back</a>to try again";
  String string = String(server.arg("minhumid"));
  Serial.println("Min humidity="+string);
  if(writeToMemory(String(server.arg("ssid")),String(server.arg("password")),String(server.arg("deviceid")),String(server.arg("mintemp")),String(server.arg("maxtemp")),String(server.arg("minhumid")),String(server.arg("maxhumid")))){
     server.send(400, "text/html", response_success);
     EEPROM.commit();
     delay(4000);
     ESP.restart();
  }else{
     server.send(400, "text/html", response_error);
  }
}

/*
 * Function for home page
 */
void handleRoot() {
  if (server.hasArg("ssid")&& server.hasArg("password")&& server.hasArg("deviceid")&& server.hasArg("mintemp")&& server.hasArg("maxtemp")&& server.hasArg("minhumid")&& server.hasArg("maxhumid")) {
    handleSubmit();
  }
  else {
    server.send(400, "text/html", INDEX_HTML);
  }
}

/*
 * Function for loading form
 * Returns: false if no WiFi creds in EEPROM
 */
bool loadWIFICredsForm(){
  String s = EEPROM.readString(100);
  String p = EEPROM.readString(200);
  String d = EEPROM.readString(400);
  String mi = EEPROM.readString(500);
  String ma = EEPROM.readString(600);
  String humin = EEPROM.readString(700);
  String humax = EEPROM.readString(800);
  
  const char* ssid     = "WIFI-MANAGER-BM-AC";
  const char* password = "solamaten";

  Serial.println("Setting Access Point...");
  
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();
  
  Serial.println("HTTP server started");
 
  while(s.length() <= 0 && p.length() <= 0 && d.length() <= 0){
    server.handleClient();
    delay(100);
  }
  
  return false;
}

/*
 * Function checking WiFi creds in memory 
 * Returns: true if not empty, false if empty
 */
bool CheckWIFICreds(){
  Serial.println("Checking WIFI credentials");
  String s = EEPROM.readString(100);
  String p = EEPROM.readString(200);
  String d = EEPROM.readString(400);
  String mi = EEPROM.readString(500); 
  String ma = EEPROM.readString(600);
  String humin = EEPROM.readString(700); 
  String humax = EEPROM.readString(800);
  //#if DEBUG
  Serial.print("Found credentials: ");
  Serial.print(s);
  Serial.print("/");
  Serial.print(p);
  Serial.print("/");
  Serial.print(d);
  Serial.print("/");
  Serial.print(mi);
  Serial.print("/");
  Serial.print(ma);
  Serial.print("/");
  Serial.print(humin);
  Serial.print("/");
  Serial.print(humax);
  delay(5000);
  //#endif
  if(s.length() > 0 && p.length() > 0 && d.length() > 0 && mi.length() > 0 && ma.length() > 0 ){
    return true;
  }else{
    return false;
  }
}
