/*
This example will open a configuration portal when the reset button is pressed twice. 
This method works well on Wemos boards which have a single reset button on board. It avoids using a pin for launching the configuration portal.
How It Works
When the ESP8266 loses power all data in RAM is lost but when it is reset the contents of a small region of RAM is preserved. So when the device starts up it checks this region of ram for a flag to see if it has been recently reset. If so it launches a configuration portal, if not it sets the reset flag. After running for a while this flag is cleared so that it will only launch the configuration portal in response to closely spaced resets.
Settings
There are two values to be set in the sketch.
DRD_TIMEOUT - Number of seconds to wait for the second reset. Set to 10 in the example.
DRD_ADDRESS - The address in RTC RAM to store the flag. This memory must not be used for other purposes in the same sketch. Set to 0 in the example.
This example, contributed by DataCute needs the Double Reset Detector library from https://github.com/datacute/DoubleResetDetector .
*/
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager

#include <DoubleResetDetector.h>  //https://github.com/datacute/DoubleResetDetector
#include <PubSubClient.h>
#include <string.h>

// Number of seconds after reset during which a 
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 10

// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0
DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

// Onboard LED I/O pin on NodeMCU board
const int PIN_LED = 2; // D4 on NodeMCU and WeMos. Controls the onboard LED.

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

char org[] = "orange";
char blu[] = "blue";
char yel[] = "yellow";
char gre[] = "green";
char pin[] = "pink";
char pur[] = "purple";
// Indicates whether ESP has WiFi credentials saved from previous session, or double reset detected
bool initialConfig = false;

//Testing RSSI print every second
unsigned long prevTime = 0;
unsigned long interval = 3000;
double dbm = WiFi.RSSI();
double measuredPower = -78;
double N = 2.7;

//Alert
bool oor = false;

void setup() {
  // put your setup code here, to run once:
  // initialize the LED digital pin as an output.
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);  
  
  Serial.println("\n Starting");
  WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed
  if (WiFi.SSID()==""){
    Serial.println("We haven't got any access point credentials, so get them now");   
    initialConfig = true;
  }
  if (drd.detectDoubleReset()) {
    Serial.println("Double Reset Detected");
    initialConfig = true;
  }
  if (initialConfig) {
    Serial.println("Starting configuration portal.");
    digitalWrite(PIN_LED, LOW); // turn the LED on by making the voltage LOW to tell us we are in configuration mode.
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //sets timeout in seconds until configuration portal gets turned off.
    //If not specified device will remain in configuration mode until
    //switched off via webserver or device is restarted.
    //wifiManager.setConfigPortalTimeout(600);

    //it starts an access point 
    //and goes into a blocking loop awaiting configuration
    if (!wifiManager.startConfigPortal()) {
      Serial.println("Not connected to WiFi but continuing anyway.");
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
    }
    digitalWrite(PIN_LED, HIGH); // Turn led off as we are not in configuration mode.
    ESP.reset(); // This is a bit crude. For some unknown reason webserver can only be started once per boot up 
    // so resetting the device allows to go back into config mode again when it reboots.
    delay(5000);
  }

  digitalWrite(PIN_LED, HIGH); // Turn led off as we are not in configuration mode.
  WiFi.mode(WIFI_STA); // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.
  unsigned long startedAt = millis();
  Serial.print("After waiting ");
  int connRes = WiFi.waitForConnectResult();
  float waited = (millis()- startedAt);
  Serial.print(waited/1000);
  Serial.print(" secs in setup() connection result is ");
  Serial.println(connRes);
  if (WiFi.status()!=WL_CONNECTED){
    Serial.println("failed to connect, finishing setup anyway");
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

// Decode MQTT Message
void callback(char* topic, byte* payload, unsigned int length) {

  char color[length];
  Serial.print("Topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    //color[i] = (char)payload[i];
  }
  // Change Color
  Serial.print(color);
    if(strncmp((char*)payload, org, length) == 0){
         analogWrite(red,255);
         analogWrite(green,165);
         analogWrite(blue,0);    
    } else if(strncmp((char*)payload, yel, length) == 0){
         analogWrite(red,255);
         analogWrite(green,255);
         analogWrite(blue,0);
    } else if(strncmp((char*)payload, gre, length) == 0){
         analogWrite(red,0);
         analogWrite(green,255);
         analogWrite(blue,0);
    }else if(strncmp((char*)payload, blu, length) == 0){
         analogWrite(red,0);
         analogWrite(green,0);
         analogWrite(blue,255);
    }else if (strncmp((char*)payload, pur, length) == 0){
         analogWrite(red,255);
         analogWrite(green,0);
         analogWrite(blue,255);
    }else if (strncmp((char*)payload, pin, length) == 0){
         analogWrite(red,255);
         analogWrite(green,192);
         analogWrite(blue,203);
    } else {
      Serial.print("\nInvalid Color, Changing to RED");
      analogWrite(red,255);
      analogWrite(green,0);
      analogWrite(blue,0);
  }
  
 
  Serial.println();
  Serial.println("-----------------------");
 
}

// Calculate RSSI Distance
double RSSIDistance(double rssi, double mp, double n){
  double exponent = (mp - rssi)/(10*n);
  double distance = pow(10,exponent);
  return distance;
}

// Alers
bool alert(double dist){
  if(dist > 3){
    // Change Red
    Serial.println("OUT OF RANGE");
    analogWrite(red,255);
    analogWrite(green,0);
    analogWrite(blue,0); 
    return true; 
  } else {
   // Change back to color
   return false;
  }
  
}

void loop() {
  // Call the double reset detector loop method every so often,
  // so that it can recognise when the timeout expires.
  // You can also call drd.stop() when you wish to no longer
  // consider the next reset as a double reset.
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
