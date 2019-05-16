#include <ESP8266WiFi.h>

/*
 * tx pin of esp send data from esp to out. the out device is on pin rx
 */
String ssid="myHotspot";
String psw="dioporco";
const char* host="mobile-robot.herokuapp.com"; //"demiledge.com";
int serverPort=80;
String page="/backend.php";

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
  if (Serial.available()) {
    String inputData=Serial.readString();
    Serial.println(inputData);
    String data="functionName=addRecord"+"&clientIP="+ inputData+"&number="+"10";
    sendRequest(data);
  }
  //Serial.print("Connected to wifi, IP: ");
  //Serial.println(WiFi.localIP());
  //delay(1000);
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
