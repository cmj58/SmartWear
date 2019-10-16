#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>       
#include <DoubleResetDetector.h>
#include <PubSubClient.h>
#include <string.h>

//Double Reset
#define DRD_TIMEOUT 10
#define DRD_ADDRESS 0
DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

// Onboard LED
const int PIN_LED = 2;

//MQTT
const char* mqttServer = "192.168.42.1";
const int mqttPort = 1883;
const char* mqttUser = "bracelet1";
const char* mqttPassword = "12345";
WiFiClient espClient;
PubSubClient client(espClient);


// Color
const int red = 15;
const int green = 12;
const int blue = 13;

int current_red;
int current_green;
int current_blue;

char org[] = "orange";
char blu[] = "blue";
char yel[] = "yellow";
char gre[] = "green";
char pin[] = "pink";
char pur[] = "purple";

// Bool holds if saved SSID available
bool initialConfig = false;

//Testing RSSI print every second
unsigned long prevTime = 0;
unsigned long interval = 3000;
double dbm = WiFi.RSSI();
double measuredPower = -78;
double N = 2.7;

//Alert
bool oor = false;

/*=============================================================
  Method: setup
  Parameters:
    none
  Return: void
  Description: Runs initial setup on power up. Checks for
               any saved AP.
  =============================================================
*/  
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);  
  
  Serial.println("\n Starting");
  WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed
  if (WiFi.SSID()==""){
    Serial.println("GET AP");   
    initialConfig = true;
  }
  if (drd.detectDoubleReset()) {
    Serial.println("DR Detected");
    initialConfig = true;
  }
  if (initialConfig) {
    Serial.println("Showing Bracelet Connection Configuration.");
    digitalWrite(PIN_LED, LOW);

    // Initialize wifiManger
    WiFiManager wifiManager;

    // Configuration Page closes after 5 minutes
    wifiManager.setConfigPortalTimeout(300);

    // Boot into AP and check if connected to SSID
    if (!wifiManager.startConfigPortal()) {
      Serial.println("Not connected to WiFi but continuing anyway.");
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
    }
    digitalWrite(PIN_LED, HIGH); 
    ESP.reset();
    delay(5000);
  }

  digitalWrite(PIN_LED, HIGH);
  WiFi.mode(WIFI_STA); // Station mode
  unsigned long startedAt = millis();
  Serial.print("Connected after ");
  int connRes = WiFi.waitForConnectResult();
  float waited = (millis()- startedAt);
  Serial.print(waited/1000);
  Serial.print(" secs. Connection  ");
  Serial.println(connRes);
  if (WiFi.status()!=WL_CONNECTED){
    Serial.println("failed to connect");
  } else{
    Serial.print("local ip: ");
    Serial.println(WiFi.localIP());
  }

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  client.subscribe("test");
}

/*=============================================================
  Method: callback
  Parameters:
    topic (char)  : MQTT topic
    payload (byte): Message being sent
    length (int)  : Length of message
  Return: void
  Description: Used for client.setCallBack. When MQTT data is recieved
               interpret payload.
  =============================================================
*/  
void callback(char* topic, byte* payload, unsigned int length) {

  char color[length];
  Serial.print("Topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    //color[i] = (char)payload[i];
  }
  analogWrite(red,255);
  analogWrite(green,0);
  analogWrite(blue,0);  
  
  // Change Color (**TODO Create Function for changing color**)
  Serial.print(color);
    if(strncmp((char*)payload, org, length) == 0){
         analogWrite(red,255);
         analogWrite(green,165);
         analogWrite(blue,0);  
         current_red = 255;
         current_green = 165;
         current_blue = 0;  
    } else if(strncmp((char*)payload, yel, length) == 0){
         analogWrite(red,255);
         analogWrite(green,255);
         analogWrite(blue,0);
         current_red = 255;
         current_green = 255;
         current_blue = 0;  
    } else if(strncmp((char*)payload, gre, length) == 0){
         analogWrite(red,0);
         analogWrite(green,255);
         analogWrite(blue,0);
         current_red = 0;
         current_green = 255;
         current_blue = 0;  
    }else if(strncmp((char*)payload, blu, length) == 0){
         analogWrite(red,0);
         analogWrite(green,0);
         analogWrite(blue,255);
         current_red = 0;
         current_green = 0;
         current_blue = 255;  
    }else if (strncmp((char*)payload, pur, length) == 0){
         analogWrite(red,255);
         analogWrite(green,0);
         analogWrite(blue,255);
         current_red = 255;
         current_green = 0;
         current_blue = 255;  
    }else if (strncmp((char*)payload, pin, length) == 0){
         analogWrite(red,255);
         analogWrite(green,192);
         analogWrite(blue,203);
         current_red = 255;
         current_green = 192;
         current_blue = 203;  
    } else {
      Serial.print("\nInvalid Color, Changing to RED");
      analogWrite(red,255);
      analogWrite(green,0);
      analogWrite(blue,0);
  }
  
 
  Serial.println();
  Serial.println("-----------------------");
 
}

/*=============================================================
  Method: RSSIDistance
  Parameters:
    rssi (double) : RSSI Value from WiFi.RSSI()
    mp (double)   : Meassured Power (1m RSSI) Constant
    n (double)    : Environmental Constant (Ranges from 2-4)
  Return: distance (double)
  Description: Used for client.setCallBack. When MQTT data is
               recieved interpret payload.
  =============================================================
*/  
double RSSIDistance(double rssi, double mp, double n){
  double exponent = (mp - rssi)/(10*n);
  double distance = pow(10,exponent);
  return distance;
}

/*=============================================================
  Method: alert
  Parameters:
    dist (double) : Distance Calculated from RSSIDistance()
  Return: bool for alert
  Description: Handles bracelet alert. Changes Color/Buzzer.
  =============================================================
*/  
bool alert(double dist){
  if(dist > 3){
    // Change Red
    Serial.println("OUT OF RANGE");
    analogWrite(red,255);
    analogWrite(green,0);
    analogWrite(blue,0); 
    return true; 
  } else {
    analogWrite(red,current_red);
    analogWrite(green,current_green);
    analogWrite(blue,current_blue); 
    return false;
  }
  
}
/*=============================================================
  Method: loop
  Parameters:
    none
  Return: void
  Description: Main loop for ESP8266
  =============================================================
*/  
void loop() {
  // Call Double Reset Detector loop to check for double reset.
  drd.loop();
  
  // put your main code here, to run repeatedly:
  client.loop();
  
  //RSSI Loop
  unsigned long curTime = millis();
  if (curTime - prevTime > interval) {
    prevTime = curTime;
    dbm = WiFi.RSSI();
    double dist = RSSIDistance(dbm,measuredPower,N);
    Serial.print("\nRSSI: ");
    Serial.print(dbm);
    Serial.print("\n");
    Serial.print("\nEstimated Distance (Meters): ");
    Serial.print(dist);
    Serial.print("\n");
    oor = alert(dist);
    }
}
