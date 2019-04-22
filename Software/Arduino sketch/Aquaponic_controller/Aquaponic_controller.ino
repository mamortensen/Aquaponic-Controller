/*
 * Hacker Lab Aquaponic system controller
 * to be installed on an ESP-01 module 
 * connected to the arduino serial port
 */

#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>

#include "index.h" //Our webpage HTML contents
#include "CSS.h" //Our webpage CSS contents
#include "js.h" //Our webpage Java script contents

//SSID and Password of your WiFi router
const char* ssid = "2WIRE975";
const char* password = "1985984603";

ESP8266WebServer server(80); //Server on port 80


void setup() {
  Serial.begin(9600);

//  Serial.println(indexString);
//  Serial.println(cssString);
//  Serial.println(jsString);
  
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

  //server.on("/", handleRoot);      //Which routine to handle at root location
  
  server.on(
    "/", []() {                     //Define the handling function for root path (HTML message)
    server.send(200, "text/html", indexString);
  });
  
  server.on("/script.js", []() { //Define the handling function for the javascript path
    server.send(200, "text/javascript", jsString);
  });
  
  server.on("/style.css", []() { //Define the handling function for the CSS path
    server.send(200, "text/css", cssString);
  });

  server.begin();                  //Start server
  Serial.println("HTTP server started");

}

void loop() {
  server.handleClient();          //Handle client requests


}
