


bool writeToMemory(String ssid, String pass, String device){
  char buff1[30];
  char buff2[30];
  char buff3[30];
  ssid.toCharArray(buff1,30);
  pass.toCharArray(buff2,30);
  device.toCharArray(buff3,30);
  EEPROM.writeString(100,buff1);
  EEPROM.writeString(200,buff2);
  EEPROM.writeString(400,buff3);
  delay(100);
  String s = EEPROM.readString(100);
  String p = EEPROM.readString(200);
  String d = EEPROM.readString(400);
  
  //#if DEBUG
  Serial.print("Stored SSID, password,deviceid are: ");
  Serial.print(s);
  Serial.print(" / ");
  Serial.print(p);
  Serial.print(" / ");
  Serial.print(d);
  //#endif
  if(ssid == s && pass == p && device ==d){
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
  
  if(writeToMemory(String(server.arg("ssid")),String(server.arg("password")),String(server.arg("deviceid")))){
     server.send(400, "text/html", response_success);
     EEPROM.commit();
     delay(4000);
     ESP.restart();
  }else{
     server.send(400, "text/html", response_error);
  }
}


void handleRoot() {
  if (server.hasArg("ssid")&& server.hasArg("password")&& server.hasArg("deviceid")) {
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
  
  const char* ssid     = "Enoda wifi kuduka maten";
  const char* password = "solamaten";

  Serial.println("Setting Access Point...");
  
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.on("/", handleRoot);

  

  server.begin();
  
  Serial.println("HTTP server started");
 

  return false;
}


bool CheckWIFICreds(){
  Serial.println("Checking WIFI credentials");
  String s = EEPROM.readString(100);
  String p = EEPROM.readString(200);
  String d = EEPROM.readString(400); 
  //#if DEBUG
  Serial.print("Found credentials: ");
  Serial.print(s);
  Serial.print("/");
  Serial.print(p);
  Serial.print("/");
  Serial.print(d);
  delay(5000);
  //#endif
  if(s.length() > 0 && p.length() > 0 && d.length() > 0 ){
    return true;
  }else{
    return false;
  }
}