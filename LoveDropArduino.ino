#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <Adafruit_NeoPixel.h>
#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"


#define WIFI_SSID "EY wavespace"
#define WIFI_PASSWORD "S0ciety@920"
#define FIREBASE_HOST "notedrop-379bf.firebaseio.com"
#define FIREBASE_AUTH "UJkgig4c1uvD431PBW2e5aBPWDnpFQXo4KkBPIBR"


#define TX_PIN 5 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 4 //Arduino receive GREEN WIRE labeled TX on printer

#define LED_COUNT 30
#define LED_PIN 6


SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&mySerial); 
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

String lastMessage = "";

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.show();
  setLights(255, 0, 0);


  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
  if(Firebase.success()){
    Serial.print("SUCCESS");
  }
  if(Firebase.failed()){
    Serial.print("FAILED");
  }
mySerial.begin(19200);
printer.begin();  
printer.setSize('L');
  
}

int n = 0;

void loop() {
  String currentMessage = Firebase.getString("message");
  String date = Firebase.getString("Messages/date");
  String sender = Firebase.getString("Messages/sender");
  if(!(currentMessage.equals(lastMessage))){
    printMessage(date, sender, currentMessage);
    setLights(0, 255, 0);
  }
  lastMessage = currentMessage;
  
  delay(1000);

}

void printMessage(String date, String sender, String message){
  Serial.println(date);
  Serial.println("From: " + sender);
  Serial.println(message);
  printer.println(date);
  printer.println("From: " + sender);
  printer.println(message);
  
}

void setLights(int red, int green, int blue){
  for(int i = 0; i < LED_COUNT; i++){
    strip.setPixelColor(i, red, green, blue);
  }
  strip.show();
}
 
