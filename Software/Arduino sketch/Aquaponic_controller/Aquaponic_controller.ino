/*
 * Hacker Lab Aquaponic system controller
 * to be installed on an ESP-01 module 
 * connected to the arduino serial port
 */

#include <ESP8266WiFi.h>            
//#include <ESP8266WebServer.h>
#include <ESPAsyncWebServer.h>


#include "index.h" //Our webpage HTML contents
#include "CSS.h" //Our webpage CSS contents
#include "js.h" //Our webpage Java script contents

//SSID and Password of your WiFi router
//const char* ssid = "2WIRE975";
//const char* password = "1985984603";
const char* ssid = "MMMwireless";
const char* password = "lorimart";

//ESP8266WebServer server(80);  //Server on port 80
AsyncWebServer server(80);  // Create AsyncWebServer object on port 80

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

String temperatureString, levelString, powerString, lightString;

// Replaces placeholders with values
String processor(const String& var){
  Serial.print("var: ");
  Serial.println(var);
  if(var == "TEMPERATURE"){
    return (temperatureString);
    Serial.println(temperatureString);
  }
  else if(var == "LEVEL"){
    return (levelString);
    Serial.println(levelString);
  }
  else if(var == "POWER"){
    return (powerString);
    Serial.println(powerString);
  }
    else if(var == "LIGHT"){
    return (lightString);
    Serial.println(lightString);
  }
  return String();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");

//  dht.begin();

  inputString.reserve(200); // reserve 200 bytes for the inputString:

  WiFi.begin(ssid, password);     //Connect to your WiFi router 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  // routines to handle server requests
//  server.on(
//    "/", []() {                     //Define the handling function for root path (HTML message)
//    server.send(200, "text/html", indexString);
//  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", indexString, processor);
  });
  
//  server.on("/script.js", []() { //Define the handling function for the javascript path
//    server.send(200, "text/javascript", jsString);
//  });
  server.on("/script.js", HTTP_GET, [] (AsyncWebServerRequest *request){ //Define the handling function for the javascript path
    request->send_P(200, "text/javascript", jsString, processor);
  });
  
//  server.on("/style.css", []() { //Define the handling function for the CSS path
//    server.send(200, "text/css", cssString);
//  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) { //Define the handling function for the CSS path
    request->send_P(200, "text/css", cssString, processor);
  });

//  server.on("/temp", []() { //Define the handling function for the temp value
//    server.send(200, "text/plain", temperatureString);
//  });
  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request) { //Define the handling function for the temp value
//    request->send_P(200, "text/plain", temperatureString.c_str, processor);
    int strLen = temperatureString.length() + 1;
    char tempArray[strLen];
    temperatureString.toCharArray(tempArray, strLen);
    request->send_P(200, "text/plain", tempArray, processor);
  });

//  server.on("/level", []() { //Define the handling function for the temp value
//    server.send(200, "text/plain", levelString);
//  });
  server.on("/level", HTTP_GET, [](AsyncWebServerRequest *request) { //Define the handling function for the temp value
//    request->send_P(200, "text/plain", levelString.c_str, processor);
    int strLen = levelString.length() + 1;
    char levelArray[strLen];
    levelString.toCharArray(levelArray, strLen);
    request->send_P(200, "text/plain", levelArray, processor);
  });

//  server.on("/power", []() { //Define the handling function for the temp value
//    server.send(200, "text/plain", powerString);
//  });
  server.on("/power", HTTP_GET, [](AsyncWebServerRequest *request) { //Define the handling function for the temp value
//    request->send_P(200, "text/plain", powerString.c_str, processor);
    int strLen = powerString.length() + 1;
    char powerArray[strLen];
    powerString.toCharArray(powerArray, strLen);
    request->send_P(200, "text/plain", powerArray, processor);
  });

//  server.on("/light", []() { //Define the handling function for the temp value
//    server.send(200, "text/plain", lightString);
//  });
  server.on("/light", HTTP_GET, [](AsyncWebServerRequest *request) { //Define the handling function for the temp value
//    request->send_P(200, "text/plain", lightString.c_str, processor);
    int strLen = lightString.length() + 1;
    char lightArray[strLen];
    lightString.toCharArray(lightArray, strLen);
    request->send_P(200, "text/plain", lightArray, processor);
  });

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop() {
  String junk;
  
//  server.handleClient();          //Handle client requests
  
  // read data from the serial port (from Arduino) 
  // and parse out temp, level, power, and light data.
  if (Serial.available()) {
    temperatureString = Serial.readStringUntil(':');
    levelString = Serial.readStringUntil(':');
    powerString = Serial.readStringUntil(':');
    lightString = Serial.readStringUntil(':');
    junk = Serial.readStringUntil('\n');    
    
    if (temperatureString.length() > 2){
 //     Serial.print(temperatureString.length());
      Serial.print("temp:");
      Serial.println(temperatureString);
    }
    if (levelString.length() > 2 ){
//      Serial.print(levelString.length());
      Serial.print("Level:");
      Serial.println(levelString);
    }
    if (powerString.length() > 2){
//      Serial.print(powerString.length());
      Serial.print("power:");
      Serial.println(powerString);
    }
    if (lightString.length() > 2){
//      Serial.print(lightString.length());
      Serial.print("light:");
      Serial.println(lightString);
    } 
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}
