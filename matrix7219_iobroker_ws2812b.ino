//#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <time.h>
#include <PubSubClient.h>
#include "secret.h"
#include "rgbled.h"
WiFiClient espClient; 
PubSubClient client(espClient); 


bool starte = false;
char tmp[50];
String Text = ""; // Variable Text wird angelegt.
String client_id = "ESP8266-";


// define Topics
const char *ledTopicEsp8266 = "esp8266/ledState";
const char *btnTopicEsp8266 = "esp8266/btnState";
const char *btnTopicEsp32 = "esp32/btnState";

void connect_to_MQTT() {
 client.setServer(mqtt_server, 1883);//MQTT Server, - Port
 client.setCallback(callback); // aktiviert das zuhören 
 // Solltet Ihr kein Benutzer/Passwort haben so müssen folgende Zeilen entfernt werden: 
 // (client.connect("display" , mqttUser, mqttPassword))
 // und mit folgender getauscht werden: 
 // if (client.connect("display");

  if (client.connect("display" , mqttUser, mqttPassword)) {
    Serial.println("Verbinde zum MQTT Server");
  } else {
    Serial.println("zu MQTT Server nicht verbunden ");
    if (!client.connect("display" , mqttUser, mqttPassword)) {
            Serial.print("Fehlgeschlagen, state= ");
            Serial.print(client.state());
            Serial.println(" Versuch in 5 Sekunden nochmal");
            delay(5000);
        }
  }
  client.subscribe("display1/text");
  client.subscribe("display1/led");
  // Es könne weitere Tropics hinzugefügt werden! 
  // Alternativ steht folgende Alternative zur Verfügung: 
  //client.subscribe("tropic/#"); --> es werden dann alle tropics abonniert ( Beispiel: Wohnzimmer/# ) Es werden alle Nachrichten aus Wohnzimmer empfangen! 

  
  //client.subscribe("display1/woche");
  //client.subscribe("display1/verbraucht");
  //client.subscribe("display1/internet");
  //client.subscribe("display1/zimmertemperatur");
}
        
int pinCS = D8; // CS PIN
int numberOfHorizontalDisplays = 4; //Display Anzahl
int numberOfVerticalDisplays   = 1; // Diplay höhen Anzahl
char time_value[20];
// LED Matrix Pin -> ESP8266 Pin
// Vcc            -> 3v / 5v
// Gnd            -> Gnd 
// DIN            -> D7 
// CS             -> D8  
// CLK            -> D5

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 95; // In millis -> Scrollgeschwindigkeit! 
int spacer = 1;
int width  = 5 + spacer; // Die Panals haben 5 Pixel + Leerzeichen

void setup() {
   // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  

  
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  connect_to_MQTT(); 
  
  configTime(1 * 3600, 0, "pool.ntp.org", "time.nist.gov"); // Zeitserver
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3 ",1);// CET-1CEST,M3.5.0,M10.5.0/3  Orginal: GMT0BST,M3.5.0/02,M10.5.0/02
  matrix.setIntensity(4); // Lichtstärke einstellen! Bis 8 kann man sicherlich über den Wemos D1 mini ansteuern, alles darüber sollte das Netzteil aufgeteilt werden und direkt mit Strom versorgt werden!! 
  // Adjust to your own needs
  matrix.setPosition(3, 0, 0); // (Display-Nr ab Einspeisung, X, Y)
  matrix.setPosition(2, 1, 0);
  matrix.setPosition(1, 2, 0);
  matrix.setPosition(0, 3, 0);  
  
  matrix.setRotation(0, 3);    
  matrix.setRotation(1, 3);    
  matrix.setRotation(2, 3);    
  matrix.setRotation(3, 3);    
}



void loop() {
   
  matrix.fillScreen(LOW);
  time_t now = time(nullptr);
  String time = String(ctime(&now));
  time.trim();
  Serial.println(time);
  time.substring(11,19).toCharArray(time_value, 10); 
  matrix.drawChar(2,0, time_value[0], HIGH,LOW,1); // H
  matrix.drawChar(8,0, time_value[1], HIGH,LOW,1); // HH
  matrix.drawChar(14,0,time_value[2], HIGH,LOW,1); // HH:
  matrix.drawChar(20,0,time_value[3], HIGH,LOW,1); // HH:M
  matrix.drawChar(26,0,time_value[4], HIGH,LOW,1); // HH:MM
  matrix.write(); // Bitmap an das Display senden.

  
  
  delay(2000);
  if (!client.connected()) {
    Serial.println("Keine Verbindung zum MQTT Server");
    connect_to_MQTT();

  //ledanz();
  //delay(2000);
  
  }
  client.loop();
}

void display_message(String message){
   for ( int i = 0 ; i < width * message.length() + matrix.width() - spacer; i++ ) {
    //matrix.fillScreen(LOW);
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // Zentriert die Ausgabe.
    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < message.length() ) {
        matrix.drawChar(x, y, message[letter], HIGH, LOW, 1); // HIGH LOW 
      }
      letter--;
      x -= width;
    }
    matrix.write(); // Bitmap an das Display senden.
    delay(wait/2);
  }
}



void callback(char* topic, byte* payload, unsigned int length) {
 // Zähler
 int i = 0;
 // Hilfsvariablen für die Convertierung der Nachricht in ein String
 char message_buff[100];
 
 Serial.println("Message arrived: topic: " + String(topic));
 Serial.println("Length: " + String(length,DEC));
 
 // Kopieren der Nachricht und erstellen eines Bytes mit abschließender \0
 for(i=0; i<length; i++) {
 message_buff[i] = payload[i];
 }
 message_buff[i] = '\0';
 
 // Konvertierung der nachricht in ein String
 String msgString = String(message_buff);
 Serial.println("Payload: " + msgString);
 if (strcmp(topic,"display1/text/set")==0){
    Text = msgString;
  matrix.fillScreen(LOW);
  display_message(Text);
  display_message(Text);
  }
  if (strcmp(topic,"display1/text")==0){
    Text = msgString;
  matrix.fillScreen(LOW);
  display_message(Text);
  display_message(Text);
  }
  if (strcmp(topic,"display1/led")==0){
   if(msgString == "100"){
   Serial.print("Changing LED to Green\n");
   ledanz();
   delay(2000);
   client.publish(ledTopicEsp8266, "Green");
   ledgreen();
   delay(2000);
  }
  if (strcmp(topic,"display1/led")==0){
   if(msgString == "75"){
   Serial.print("Changing LED to Green\n");
   ledanz();
   delay(2000);
   client.publish(ledTopicEsp8266, "Green");
   ledgreenx();
   delay(2000);
  }
  if (strcmp(topic,"display1/led")==0){
   if(msgString == "50"){
   Serial.print("Changing LED to Yellow\n");
   ledanz();
   delay(2000);
   client.publish(ledTopicEsp8266, "Yellow");
   ledyellow();
   delay(2000);
  }
  if (strcmp(topic,"display1/led")==0){
   if(msgString == "25"){
   Serial.print("Changing LED to Yellow\n");
   ledanz();
   delay(2000);
   client.publish(ledTopicEsp8266, "Yellow");
   ledyellowx();
   delay(2000);
  }
  if (strcmp(topic,"display1/led")==0){
   if(msgString == "10"){
   Serial.print("Changing LED to RED\n");
   ledanz();
   delay(2000);
   client.publish(ledTopicEsp8266, "RED");
   ledred();
   delay(2000);
  }



 
  /*
  Zum freimachen, das "/*" und am ende "* /" entfernen!
    

  ################################################ 
  ## Tropic filtern und etwas bestimmtes machen!#
  ################################################
  
  if (strcmp(topic,"display1/verbraucht")==0){
    Text = "Internet verbraucht: " + msgString + "Std.";
  display_message(Text);
  display_message(Text);
  }
  if (strcmp(topic,"display1/woche")==0){
    Text = "Woche Gesamt: " + msgString + "Std.";
  display_message(Text);
  display_message(Text);
  }
  if (strcmp(topic,"display1/zimmertemperatur")==0){
    Text = msgString + " Grad";
    display_message(Text);
    display_message(Text);
    long rssi = WiFi.RSSI();
    itoa(rssi,tmp,10);
    client.publish("display1/rssi",tmp);
  //display_message(Text);
  }
   
*/}
  }
  }
  }
  }
}
