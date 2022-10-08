//This is an IoT project for an automated irrigation and farm monitoring system. Farming parameters such as soil moisture, humidity and temperature are sent to a Raspberry pi server which handles
//the automation of the water pump. Data is also sent to ThingSpeak, an IoT analytics platform service that allows you to aggregate, visualize, and analyze live data streams in the cloud. 

//It makes use of the ESP8266 development board,DHT sensor and a soil moisture sensor. We will use an MQTT protocol to send data to the raspberry pi. MQTT is a lightweight, publish-subscribe, machine to machine network protocol.


// Import required libraries
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <HttpClient.h>
#define DHTPIN 0      
#define DHTTYPE  DHT11


int soil = A0; // Assign the soil sensor to analog pin 0
int soilV;
int soilA;
int percent;
int threshold=550;
float t;
float h;


const char* ssid = "WyFy";                   // wifi ssid
const char* password =  "Rockerfeller";         // wifi password
const char* mqttServer = "192.168.8.106";    // IP adress Raspberry Pi
const int mqttPort = 1883;
const char* mqttUser = "";      // if you don't have MQTT Username, no need input
const char* mqttPassword = "";  // if you don't have MQTT Password, no need input
const char* writeAPIKey = "VOBQYRBFH8PJIOH4";
const char* host = "api.thingspeak.com";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {

// Setup the sensors and WiFi board
  pinMode(soil,INPUT);
  Serial.begin(115200);
  dht.begin();
  delay(1000);
  WiFi.begin(ssid, password);


// Establish a WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");


// Establish a connection with the Raspberry  Pi server.
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

}

// This method will help us publish values to a specific topic 
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");
}

// Connect to MQTT server
void jaribu_tena(){
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");
    } 
    else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  }
  
  // Send temperature to MQTT server
void mod_temp(){
  t = dht.readTemperature();
  String temperatureString = String(t);
  int length = temperatureString.length();
  char temperature[length];
  temperatureString.toCharArray(temperature, temperatureString.length() +1); 
  client.publish("esp8266_t", temperature);
  }


// Send humidity to MQTT server
void mod_humid(){
  h = dht.readHumidity();
  String humidityString = String(h);
  int length = humidityString.length();
  char humidity[length];
  humidityString.toCharArray(humidity, humidityString.length() +1);
  client.publish("esp8266_h", humidity);
  }

  // Send moisture to MQTT server
void mod_moisture(){
  soilV=analogRead(soil);
  soilA=(threshold-soilV);
  percent=(soilA/.550)*.100;
  String moistureString = String(percent);
  int length = moistureString.length();
  char moisture[length];
  moistureString.toCharArray(moisture, moistureString.length() +1);
  client.publish("esp8266_m", moisture);  

//Send an alert to the server when the soil moisture falls below 20%
  if (percent>20){
    int famous = 1;
    String trigger = String(famous);
    int length = trigger.length();
    char dry[length];
    trigger.toCharArray(dry, trigger.length() +1);
    client.publish("esp8266_alert", dry);
  }
    else{
      int famous = 0;
      String trigger = String(famous);
      int length = trigger.length();
      char dry[length];
      trigger.toCharArray(dry, trigger.length() +1);
      client.publish("esp8266_alert", dry);
      }
    
    }
    
   // This methods sends a trigger prompt to the server when the soil moisture falls below 20%
void trigger(){
  soilV=analogRead(soil);
  soilA=(threshold-soilV);
  percent=(soilA/.550)*.100;

  if (percent>20){
    int retro = 1;
    String drip = String(retro);
    int length = drip.length();
    char wet[length];
    drip.toCharArray(wet, drip.length() +1);
    client.publish("esp8266_irrigation", wet);
  }
    else{
      int retro = 0;
      String drip = String(retro);
      int length = drip.length();
      char wet[length];
      drip.toCharArray(wet, drip.length() +1);
      client.publish("esp8266_irrigation", wet);
      } 
    }
 
// Send data to thingspeak
void thing_speak(){
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  soilV=analogRead(soil);
  soilA=(threshold-soilV);
  percent=(soilA/.550)*.100;

  if (isnan(humidity) || isnan(temperature)) {
    return;
  }
  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
     return;
  }

  String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(temperature);
  url+="&field2=";
  url+=String(humidity);
  url+="&field3=";
  url+=String(percent);
  url+="\r\n";
  
  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  //Output for debugging purposes
  Serial.print("Temperature:");
  Serial.println(temperature);
  Serial.print("\n");
  Serial.print("Humidity:");
  Serial.println(humidity);
  Serial.print("\n");
  Serial.print("Soil Moisture:");
  Serial.println(percent);
  Serial.println("Send to ThingSpeak.\n");
  client.stop();
  Serial.println("Wait for 15 sec to update next datapack in thingSpeak");
    delay(1000);
  } 



void loop() {
  mod_temp();
  mod_humid();
  mod_moisture();
  thing_speak();
  trigger();

  client.subscribe("esp8266_t");
  client.subscribe("esp8266_h");
  client.subscribe("esp8266_m");
  client.subscribe("esp8266_relay");
  client.subscribe("esp8266_alert");
  client.subscribe("esp8266_irrigation");

  delay(3000);
  jaribu_tena();
 client.loop();
}
