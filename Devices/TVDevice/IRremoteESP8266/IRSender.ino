/*
 * IRremoteESP8266: IRServer - demonstrates sending IR codes controlled from a webserver
 * An IR LED must be connected to ESP8266 pin 0.
 * Version 0.1 June, 2015
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
 
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
MDNSResponder mdns;

ESP8266WebServer server(80);

IRsend irsend(D4);

void handleRoot() {
 String htmlContent = "<html><head><title>ESP8266 IR Remote</title></head><body><h1>ESP8266 IR Remote</h1></br>";

   htmlContent += "\n";
   for (uint8_t i=0; i<server.args(); i++){
    htmlContent += server.argName(i);
    htmlContent += "=";
    htmlContent += server.arg(i);
    htmlContent += "\n<br\>";
   }
 
 
 htmlContent += "</br><p><a href=\"ir?tv=on\">TV ON</a></p><p><a href=\"ir?tv=off\">TV OFF</a></p>";
 htmlContent += "<p><a href=\"ir?tv=chanup\">TV CHANNEL UP</a></p><p><a href=\"ir?tv=chandown\">TV CHANNEL DOWN</a></p><p><a href=\"ir?tv=prev\">PREVIOUS</a></p>";
 htmlContent += "<p><a href=\"ir?tv=volup\">TV VOLUME UP</a></p><p><a href=\"ir?tv=voldown\">TV VOLUME DOWN</a></p><p><a href=\"ir?tv=mute\">TV MUTE</a></p><p><a href=\"ir?tv=input\">TV INPUT</a></p>";
 htmlContent += "<p><a href=\"ir?hdmi=1\">HDMI 1</a></p><p><a href=\"ir?hdmi=2\">HDMI 2</a></p>";
 htmlContent += "<p><a href=\"ir?hdmi=3\">HDMI 3</a></p><p><a href=\"ir?hdmi=4\">HDMI 4</a></p></body></html>";
 server.send(200, "text/html", htmlContent);
}

void handleIr(){
  for (uint8_t i=0; i<server.args(); i++){
    if(server.argName(i) == "single") 
    {
        unsigned int  rawData[67] = {9050,4500, 600,600, 550,600, 550,1700, 550,600, 550,600, 550,600, 550,600, 550,600, 550,1700, 600,1700, 550,600, 550,1700, 550,1700, 550,1700, 600,1700, 550,1700, 550,600, 550,600, 550,600, 550,1700, 600,600, 550,600, 550,600, 550,600, 550,1700, 600,1700, 600,1700, 600,600, 550,1700, 550,1700, 600,1700, 600,1700, 600};  // NEC 20DF10EF
        irsend.sendRaw(rawData, 67, 38);
    }
    else if(server.argName(i) == "tv") 
    {
      if (server.arg(i) == "on") {
        unsigned int  rawData[20] = {6000,1200, 1200,1200, 600,600, 1200,600, 600,600, 600,600, 600,600, 1200,1200, 1200,600, 600,30000};  // UNKNOWN A80E7E5E
        irsend.sendRaw(rawData, 20, 38);
        delay(1000);
        irsend.sendNEC(0x20DF23DC, 38);
      }
      if (server.arg(i) == "off") {
        unsigned int  rawData[20] = {6000,1200, 1200,1200, 600,600, 1200,600, 600,600, 600,600, 600,1200, 1200,1200, 1200,1200, 600,30000};  // UNKNOWN 692D04A0
        irsend.sendRaw(rawData, 20, 38);
        delay(1000);
        irsend.sendNEC(0x20DFA35C, 38);
      }
      if (server.arg(i) == "chanup") {
        unsigned int  chanup[20] = {6000,1200, 1200,1200, 600,600, 600,600, 600,600, 1200,1200, 600,1200, 1200,600, 1200,600, 600,30000};  // UNKNOWN 165412B7
        irsend.sendRaw(chanup, 20, 38);
      }
      if (server.arg(i) == "chandown") {
        unsigned int  chandown[20] = {6000,1200, 1200,1200, 600,600, 600,600, 600,600, 1200,1200, 1200,600, 1200,600, 1200,1200, 600,30000};  // UNKNOWN 5815B090
        irsend.sendRaw(chandown, 20, 38);
      }
      if (server.arg(i) == "prev") {
        unsigned int  prev[20] = {6000,1200, 1200,1200, 600,600, 600,600, 600,600, 1200,1200, 1200,1200, 1200,1200, 600,600, 600,30000};  // UNKNOWN 5BFBFDE9
        irsend.sendRaw(prev, 20, 38);
      }
      if (server.arg(i) == "volup") {
        irsend.sendNEC(0x20DF40BF, 38);
      }
      if (server.arg(i) == "voldown") {
        irsend.sendNEC(0x20DFC03F, 38);
      }
      if (server.arg(i) == "mute") {
        irsend.sendNEC(0x20DF906F, 38);
      }
      if (server.arg(i) == "input") {
        irsend.sendNEC(0x20DFD02F, 38);
        delay(500);
        irsend.sendNEC(0x20DFD02F, 38);
      }
    }
    else if(server.argName(i) == "hdmi") 
    {
      if (server.arg(i) == "1") {
        irsend.sendNEC(0x1FE40BF, 38);
      }
      if (server.arg(i) == "2") {
        irsend.sendNEC(0x1FE20DF, 38);
      }
      if (server.arg(i) == "3") {
        irsend.sendNEC(0x1FEA05F, 38);
      }
      if (server.arg(i) == "4") {
        irsend.sendNEC(0x1FE609F, 38);
      }
    }
  }
  handleRoot();
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
 
void setup(void){
  //TURN OFF BUILTIN LEDS
  int redLED = LED_BUILTIN; //GPIO16 also D0 also LED_BUILTIN
  int blueLED = D4; //GPIO2 also D4
  pinMode(redLED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(blueLED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(blueLED, HIGH); 
  digitalWrite(redLED, HIGH); 

  irsend.begin();
  
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", handleRoot);
  server.on("/ir", handleIr); 
 
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
