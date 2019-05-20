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
NOTE: to enter run prgram mode, simply unplug RST and GPIO0
NOTE: tx pin of esp send data from esp to out. the out device is on pin rx
 */
#include <ESP8266WiFi.h>
String ssid="Myhotspot";
String psw="pasadina";
const char* host="www.demiledge.com";
int serverPort=80;
String page="assignment/main.php";

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

  connectToServer();
}

void loop() 
{ 
  if (Serial.available()) {   
    String receivedInput=String(Serial.readString());
    if(receivedInput[0]=='$') {
      receivedInput.remove(0,1);
      while (!myclient.connected()) {
        Serial.println("[Client connection failed!]");
        connectToServer();
        delay(50);
      }

      Serial.println("[Client connected]");
      Serial.println("[Sending a request]");
      
      Serial.print("Input:");
      Serial.println(receivedInput);
      
      sendPostRequest(receivedInput);
      Serial.println("[Request sent]");
    } else {
      Serial.println("The input has not $, so it is not data to send"); 
    }
  }

  //Display responses if any available
  //checkResponses();
}

void sendPostRequest(String data) {
  myclient.print("POST /"+ String(page)+" HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Content-Length: " + data.length() + "\r\n"+
             "Content-Type: application/x-www-form-urlencoded\r\n" + 
             "\r\n" + 
             data
            );
}

void sendGetRequest() {
  myclient.print("GET /"+ String(page) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n" +
               "\r\n");
}

void connectToServer() {
  Serial.printf("\n[Connecting to %s]", host);
  while (!myclient.connect(host, 80)) {
    Serial.print(".");
    delay(100);
  } 
  Serial.printf("\n[Connected to %s]", host);
  Serial.println("");
}

void checkResponses() {
  if (myclient.available()) {
    Serial.println("[Client response:]");
    String line = myclient.readStringUntil('\n');
    Serial.println(line);
  }
}
