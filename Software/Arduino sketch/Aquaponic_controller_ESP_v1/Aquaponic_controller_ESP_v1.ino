/*
 * Hacker Lab Aquaponic system controller
 * to be installed on an ESP-01 module 
 * connected to the arduino serial port
 * 
 * The web page files in the data subdirectory need to also be loaded 
 * using the "ESP8266 Sketch Data Upload" utility under tools.
 * 
 * It expects to recieve a string from the arduino via the serial port 
 * With the format"<temperature>:<level>:<power>"
 * i.e. floating point numbers separated by :
 * 
 */

#define DEBUG false

#include <Arduino.h>
#include <ESP8266WiFi.h>            
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <NTPClient.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <Timezone.h>

//SSID and Password of your WiFi router

const char* ssid = "HackerLab-Members";
const char* password = "sierracollege";

AsyncWebServer server(80);  // Create AsyncWebServer object on port 80

String inputString = "";      // a String to hold incoming data

String temperatureString, levelString, powerString, lightString;
String onTime, offTime;


//Set up time server
TimeChangeRule PDT = {"PDT", Second, Sun, Mar, 2, -420};     //Central European Summer Time
TimeChangeRule PST = {"PST", First, Sun, Nov, 2, -480};       //Central European Standard Time
Timezone PT(PDT, PST);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  inputString.reserve(200); // reserve 200 bytes for the inputString:

  // connect to WiFi
  WiFi.hostname("HL_Aquaponics");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);     //Connect to your WiFi router 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("");
  //If connection successful show IP address in serial monitor
  if (DEBUG) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  }

  //Set up for over the air programming
  // Port defaults to 8266
  //ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("Aquaponic8266");
  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  // routines to handle server requests
  // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  //Define the handling function for the javascript path
  server.on("/script.js", HTTP_GET, [] (AsyncWebServerRequest *request){
   request->send(SPIFFS, "/script.js");
  });

  //Define the handling function for the CSS path
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css");
  });
  
  //Define the handling function for the temp value
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", temperatureString.c_str());
    temperatureString = "";    
  });

  //Define the handling function for the level value
  server.on("/level", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", levelString.c_str());
    levelString = "";    
  });

  //Define the handling function for the power value
  server.on("/power", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", powerString.c_str());
    powerString = "";    
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

    String inputMessage;
    String inputParam;

    //Define the handling function for "lights on" time
    if (request->hasParam("ontime")) {
      inputMessage = request->getParam("ontime")->value();
      onTime = inputMessage;
      onTime.remove(2,1);

    }
    //Define the handling function for "lights_off" time
    if (request->hasParam("offtime")) {
      inputMessage = request->getParam("offtime")->value();
      offTime = inputMessage;
      offTime.remove(2,1);
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    if (DEBUG) {
      Serial.print("on Time: ");
      Serial.print(onTime);
      Serial.print(" off Time: ");
      Serial.println(offTime);
    }
      request->redirect("/");
  });
  
  server.onNotFound(notFound);
  
  server.begin();                  //Start server
  timeClient.begin();  // Start NTP UDP client
//  Serial.println("HTTP server started");
}




void loop() {
  ArduinoOTA.handle();

  String junk;
  String currentTime = "";
  String t = "";
  int onT, offT, curT;
  time_t local, utc;
  tmElements_t te, ute; 

  //To add: get saved values from the file format: onTime offTime
  
  timeClient.update();

  unsigned long epochTime =  timeClient.getEpochTime();
  // convert received time stamp to time_t object
  utc = epochTime;
  local = PT.toLocal(utc);
  // break the time into fields and put it into a string of the right format
  breakTime(local, te);
    if (te.Hour < 10) {
      currentTime += "0";
    }
    currentTime += te.Hour;
  if (te.Minute < 10) {
    currentTime += "0";
  }
  currentTime += te.Minute;
  curT = currentTime.toInt();
  if (onTime != "" && offTime != "") {
    onT = onTime.toInt();
    offT = offTime.toInt();
    if ((curT >= onT) && (curT < offT)){
      Serial.println("lights:on:");
    }
    else {
      Serial.println("lights:off:");
    }
  }
  
  if (DEBUG) {
      Serial.print (" onTime: ");
      Serial.print (onTime);
      Serial.print (" offTime: ");
      Serial.print (offTime);
      Serial.print (" currentTime: ");
      Serial.println (currentTime);
  }
  
  
  // read data from the serial port (from Arduino) 
  // and parse out temp, level, power, and light data.
  if (Serial.available()) {
    temperatureString = Serial.readStringUntil(':');
    levelString = Serial.readStringUntil(':');
    powerString = Serial.readStringUntil(':');
    junk = Serial.readStringUntil('\n');    
    if (DEBUG) {
      Serial.print (" temp:");
      Serial.print (temperatureString);
      Serial.print (" level:");
      Serial.print (levelString);
      Serial.print (" power:");
      Serial.print (powerString);
      Serial.print (" junk:");
      Serial.println (junk);   
    }
    // clear the string:
    inputString = "";
  }
  
//  delay(1000); // wait a sec
//  delay(3000); // wait 3 sec
  delay(30000); // wait 30 sec
//  delay(60000); // wait a minute
}
