const char INDEX_HTML[] =
"<!DOCTYPE HTML> <html> <head> <meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\"> <meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\"> <title>ESP8266 Web Form Demo</title> <style> body { background-color: #03132d; } #grad1 { height: 550px; background-image: url(\"https://i.ibb.co/JF67p2x/1673318-3.jpg\"); background-repeat: no-repeat; background-position: center; background-color: #03132d; font-family: \"Monaco\", monospace; } </style> <div id=\"grad1\" style=\"text-align:center;margin:auto;color:#FFFFFF;font-size:40px;font-weight:bold\"> Wifi Manager </div> </head> <body> <style> form { position: absolute; left: 50%; top: 60%; transform: translate(-50%, -50%); font-family: \"Monaco\", monospace; } h3{ position: absolute; top: 30%; left:50%; transform: translate(-50%, -50%); font-family: \"Monaco\", monospace; color: white; } </style> <h3>Enter your WiFi credentials</h3><br><br> <form action=\"/\" method=\"post\"> <label style=\"color: white;\"for=\"ssid\">SSID:</label> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"text\" id=\"ssid\" name=\"ssid\"><br><br> <label style=\"color: white; \"for=\"password\">Key:&nbsp;</label> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"password\" id=\"password\" name=\"password\"><br><br> <label style=\"color: white;\"for=\"deviceid\">Device Id:&nbsp;</label> <input type=\"text\" id=\"deviceid\" name=\"deviceid\"><br><br> <!--...<label style=\"color: white;\"for=\"minitemp\">Minimum Temp:&nbsp;</label> <input type=\"text\" style=\"width:40px\" id=\"minitemp\" name=\"minitemp\"><br><br> <label style=\"color: white;\"for=\"maxtemp\">Maximum Temp:&nbsp;</label> <input type=\"text\" style=\"width:40px\" id=\"maxtemp\" name=\"maxtemp\"><br><br> --> <label style=\"color: white;\"for=\"mintemp\">Min Temp:</label> <input type=\"number\" id=\"mintemp\" name=\"mintemp\" min=\"-50\" max=\"50\"> <label style=\"color: white;\"for=\"maxtemp\">Max Temp:</label> <input type=\"number\" id=\"maxtemp\" name=\"maxtemp\" min=\"-50\" max=\"50\"><br><br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"submit\" value=\"Submit\"text-align:justify> <style> input[type=\"submit\"] { background-color: rgb(253, 254, 255); color: rgb(13, 40, 161); border-radius: 20px; height: 35px; width: 75px; font-family: \"Monaco\", monospace; font-weight: bold; } input[type=\"submit\"]:hover { background-color: #03132d; color: white; } </style> </form> </body> </html>\"";
