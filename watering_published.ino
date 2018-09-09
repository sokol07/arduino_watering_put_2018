#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

bool water=0; //check if there is water, init as "no"
int moist[]={1020,1020,1020}; //moisture levels from sensor, init as "very wet"
int moist_trig[]={1022,1022,1022};  //watering moisture trigegr level, init as "totally dry"
int water_amount[]={5000,5000,5000};  //watering time in ms
bool enable = true;
int result;

//dla odczytu z TS:
const char* ssid     = "YOUR WIFI SSID";
const char* password = "YOUR WIFI PASSWORD";
const char* host = "api.thingspeak.com";
static char responseBuffer[2*1024]; 
int value = 1; 

//dla uploadu na TS:
String apiKey = "YOUR THINGSPEAK API";     //  Enter your Write API key from ThingSpeak
const char* server = "api.thingspeak.com";
WiFiClient client;


bool decodeJSON(char *json) {
  StaticJsonBuffer <2*1024> jsonBuffer;
  char *jsonstart = strchr(json, '{'); // Skip characters until first '{' found and ignore length, if present
  if (jsonstart == NULL) {
    Serial.println("JSON data missing");
    return false;
  }
  json = jsonstart;;
  JsonObject& root = jsonBuffer.parseObject(json); // Parse JSON
  if (!root.success()) {
    Serial.println(F("jsonBuffer.parseObject() failed"));
    //return false;
  }
 JsonObject& root_data = root["channel"]; // Begins and ends within first set of { }
 
  String id   = root_data["id"];
  String name = root_data["name"];
  String field1_name = root_data["field1"]; // Extracts field name in this example field1 is called 'Pressure'
  String datetime    = root_data["updated_at"];
  Serial.println("\n\n Channel id: "+id+" Name: "+ name);
  Serial.println(" Readings last updated at: "+datetime);
  int result=0;
  JsonObject& channel = root["feeds"][result]; // Now we can read 'feeds' values and so-on
  String entry_id     = channel["entry_id"];
  String field1value  = channel["field1"];
  String field2value  = channel["field2"];
  String field3value  = channel["field3"];
  String field4value  = channel["field4"];
  String field5value  = channel["field5"];
  String field6value  = channel["field6"];
  String field7value  = channel["field7"];
  //Serial.print(" Field1 entry number ["+entry_id+"] had a value of: ");Serial.println(field1value);
  if(field1value != "null"){
   moist_trig[0]=field1value.toInt();
  }
  if(field2value != "null"){
    moist_trig[1]=field2value.toInt();
  }
  if(field3value != "null"){
    moist_trig[2]=field3value.toInt();
  }
  if(field4value != "null"){
    water_amount[0]=field4value.toInt();
  }
  if(field5value != "null"){
    water_amount[1]=field5value.toInt();
  }
  if(field6value != "null"){
    water_amount[2]=field6value.toInt();
  }
  if (field7value=="1") {
    enable = true;
  } else if (field7value=="0"){
    enable = false;
  }
}


 
void setup() {
  pinMode(15,INPUT);  //GPIO8, water level sensor
  pinMode(A0,INPUT); //A0, moisture level input
  pinMode(2,OUTPUT); //D4, moisture sensors power relay
  pinMode(0,OUTPUT); //D3, pump relay
  pinMode(4,OUTPUT); //D2, valve1
  pinMode(5,OUTPUT); //D1, valve2
  pinMode(16,OUTPUT); //D0, valve3
  pinMode(D6,OUTPUT); //D6, multiplexer s0
  pinMode(D7,OUTPUT); //D7, multiplexer s1
  
  digitalWrite(0,HIGH);
  digitalWrite(2,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(16,HIGH);
  
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Connecting to ");
  Serial.println(host);
}

  void loop() {
    //odczyt wilgotnosci

    digitalWrite(2,LOW);   //turn sensors power on
    delay(1000);    
    for(int i=0; i<3; i++){
      moist[i]=check_moist(i);   //check moist
    }
    digitalWrite(2,HIGH);    //turn sensors power off
  //FUNKCJE WEBOWE
  //READ
  WiFiClient Client;
  const int httpPort = 80;
  if (!Client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/channels/YOUR_CHANNEL_NO/feeds.json?results=1&api_key=YOUR_API_KEY"; //INSERT YOUR THINGSPEAK CHANNEL No AND API
  Serial.print("Requesting URL: ");
  Serial.println(host + url);
  //Serial.println(String("TRY: ") + value + ".");
  
  // This will send the request to the server
 Client.print(String("GET ") + url + "&headers=false" + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  delay(5000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(Client.available()){
      String line = Client.readStringUntil('\r');
      //Serial.println("Read message:");
      
      if ( line.indexOf('{') > 0) {                  // Ignore data that is not likely to be JSON formatted, so must contain a '{'
             //Serial.println(line);                            // Show the text received
             line.toCharArray(responseBuffer, line.length()+1); // Convert to char array for the JSON decoder
             decodeJSON(responseBuffer);                      // Decode the JSON text
          }
       }
       Serial.println("");
  Serial.println("");
  //Serial.println(String("Try nr. ") + value + " is finished.");
  Serial.println("Waiting...");
  Serial.println("");
  //value = value + 1;
  //UPLOAD
    if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(moist[0]);
                             postStr +="&field2=";
                             postStr += String(moist[1]);
                             postStr +="&field3=";
                             postStr += String(moist[2]);
                             postStr += "\r\n\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");   
                             client.print(postStr);
 
                             Serial.print("Moist1: ");
                             Serial.println(moist[0]);
                             Serial.print("Moist2: ");
                             Serial.println(moist[1]);
                             Serial.print("Moist3: ");
                             Serial.println(moist[2]);
                             Serial.println("Send to Thingspeak.");
                             Serial.println("");
                        }
          //client.stop();
  
  //FUNKCJE LOKALNE
  check_water();    //check if there is water
  if(water==1 && enable==true){   //watering plants one after another
    if(moist[0]<moist_trig[0]){
      watering(0);
    }
    if(moist[1]<moist_trig[1]){
      watering(1);
    }
    if(moist[2]<moist_trig[2]){
      watering(2);
    }
  }
  Serial.print("moist_trig0: ");
  Serial.println(moist_trig[0]);
  Serial.print("moist_trig1: ");
  Serial.println(moist_trig[1]);
  Serial.print("moist_trig2: ");
  Serial.println(moist_trig[2]);
  Serial.print("water_amount0: ");
  Serial.println(water_amount[0]);
  Serial.print("water_amount1: ");
  Serial.println(water_amount[1]);
  Serial.print("water_amount2: ");
  Serial.println(water_amount[2]);
  Serial.print("enable: ");
  Serial.println(enable);
  Serial.print("water sensor: ");
  Serial.println(water);
  Serial.println("waiting...");
  delay(30000);   //30sec pause
  
}

int check_moist(int num){   //check moisture
  switch (num){
    case 0:
    digitalWrite(D6,LOW);
    digitalWrite(D7,LOW);
    break;
    case 1:
    digitalWrite(D6,LOW);
    digitalWrite(D7,HIGH);
    break;
    case 2:
    digitalWrite(D6,HIGH);
    digitalWrite(D7,LOW);
    break;
  }
  delay(5000);
  result = 1024 - analogRead(A0);
  return result;
}

void watering(int num){ 
  digitalWrite(0,LOW);
   switch (num){
    case 0:
    digitalWrite(4,LOW);
    break;
    case 1:
    digitalWrite(5,LOW);
    break;
    case 2:
    digitalWrite(16,LOW);
    break;
   }
   
   delay(water_amount[num]);
   digitalWrite(0,HIGH);
   digitalWrite(4,HIGH);
   digitalWrite(5,HIGH);
   digitalWrite(16,HIGH);
      
}

void check_water(){   //check if there is water in tank
  if(digitalRead(15)==1){
    water = 1;
  }
  else{
    water = 0;
  }
}
