/*program load pins
 * ESP:
 * TX       GND
 * CH_PD    GPIO2
 * RST      GPIO0
 * VCC      RX
 *    ESP   |   USB
-----------------
TX       |   RX
RX       |   TX
GND      |   GND
VCC      |   VCC
CH       |   VCC
GPIO0    |   GND

NOTE: Once you have wired as described above, set RST -> GND, 
      then move RST -> VCC (these two steps reset the module). 
      Repeat it a couple of times and leave RST -> VCC
 */
#include <ESP8266WiFi.h>

/*
 * tx pin of esp send data from esp to out. the out device is on pin rx
 */
String ssid="VM9338061"; //"myHotspot";
String psw="ckds6xnYwfch"; //"dioporco";
const char* host="http://demiledge.com/assignment/"; //"demiledge.com";
int serverPort=80;
String page="main.php";

//stores the connection with the server. Call it to send request
WiFiClient myclient;

void setup() 
{
  Serial.begin(9600);
  
  WiFi.begin(ssid, psw);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(5000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  establishConnection(host);
}

void loop() 
{
  String data="functionName=addRecord&clientIP=test&number=10";
  sendRequest(data);
  
  Serial.print("Connected to wifi, IP: ");
  Serial.println(WiFi.localIP());
  delay(10000);
}


void establishConnection(String myhost) {   
   Serial.print("Establishing connection to: ");
   Serial.println(myhost);
   while(!myclient.connect(myhost, serverPort)){
       Serial.print(".");
       delay(300);
   }
   Serial.println("");
   Serial.print("Connected successfully to ");
   Serial.println(myhost);
}

void sendRequest(String data) {
   Serial.println("Sending information to the server..");
   
   String postRequest=
   "POST " + page + " HTTP/1.1\r\n" + 
   "Host: " + host + "\r\n" + 
   "Content-Length: " + data.length() + "\r\n"+
   "Content-Type: application/x-www-form-urlencoded\r\n" + 
   "\r\n" + 
   data;
   
   myclient.print(postRequest);
}
