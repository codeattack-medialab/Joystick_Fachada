/*
   WebSocketClientSocketIO.ino

    Created on: 06.06.2016

*/

#define LED D4            //  (D0)in V2 && D4 in Lolin V3


#include <WiFiUdp.h>
WiFiUDP Udp;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
unsigned int localPort = 33333;      // local port to listen on
//String remoteIp = "192.168.1.171";
//char * remoteIp = "192.168.1.171";
IPAddress remoteIpFachada(192, 168, 1, 171);
IPAddress remoteIp(192, 168, 1, 109); //192.168.1.109 //192, 168, 200, 102

//----------------------------------------------
void wifiSetup() {
  // Connection to wireless network
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.print("Connecting to ");
  USE_SERIAL.println(ssid);

  //Configuring the WI-FI with the specified static IP.
  WiFi.config(ip, gateway, subnet);

  //Start the WI-FI connection with specified ACCESS-POINT
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address in the serial monitor.
  Serial.print("Type this address in URL to connect: ");
  Serial.print("http://");
  Serial.println(ip);
  Serial.println("/");

  Serial.print("Local IP = ");
  Serial.println(WiFi.localIP());
}

void setup_webSockets() {

  USE_SERIAL.println("Hello WbsocketsIO");

  //USE_SERIAL.setDebugOutput(true);
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  ////
  wifiSetup();

  //LED response
  pinMode(LED, OUTPUT);   // Defining pin as output

  //COUNTER TIMER
  messageTimestamp = millis();

}

//---------------------------------------------------
//seconds blinking
void doBlikingLED(int _seconds) {

  USE_SERIAL.println("doBlikingLED Init");
  long auxSince = millis() + 1000 * _seconds;

  while (auxSince > millis()) {

    //    USE_SERIAL.println("auxSince = " + String(auxSince));
    //    USE_SERIAL.println("auxNow = " + String(millis()));

    digitalWrite(LED, LOW); // turn the LED on.
    delay(100);
    digitalWrite(LED, HIGH);// turn the LED off.(Note that LOW is the voltage level but actually
    //the LED is on; this is because it is acive low on the ESP8266.
    delay(10);
  }
}

//-----------------------------------------------------
void sendUdp(String _buffer) {

  // send a reply, to the IP address and port that sent us the packet we received
  Udp.beginPacket(remoteIpFachada, localPort); //Udp.remoteIP() && Udp.remotePort() //remoteIpFachada
  _buffer.toCharArray(packetBuffer, _buffer.length());
  if(bDebugPrint)USE_SERIAL.println("myBuffer = ");
  if(bDebugPrint)USE_SERIAL.println(packetBuffer);

  Udp.write(packetBuffer);
  Udp.endPacket();

  if(bDebugPrint)USE_SERIAL.println("after");
}

//------------------------------------------------------
void loop_udp() {

  if (sendUDPOnce) {
    if (bSendNoLeftNoRightMessage) {
      sendUdp( String(idJoystick) + "/X/" + String(0.0));
      if(bDebugPrint)USE_SERIAL.println("bSendNoLeftNoRightMessage!");
      bSendNoLeftNoRightMessage = false;
    }
    else {
      if (bSendXMessage) {
        //sendMessage("toServer", "X/" + String(mapAnalogX_ky023), String(idJoystick)); //String(mapAnalogX_ky023)
        sendUdp( String(idJoystick) + "/X/" +  String(mapAnalogX_ky023));
        if(bDebugPrint)USE_SERIAL.println("bSendXMessage!");
        bSendXMessage = false;
      }
    }

    if (bSendClickMessage) {

      //TODO if pressed
      //sendMessage("toServer", "Click/" + String(1.0), String(idJoystick)); // "Click is just one Click"
      sendUdp(String(idJoystick) + "/Click/" + String(1.00));
      bSendClickMessage = false;
      if(bDebugPrint)USE_SERIAL.println("bSendClickMessage!");

      //TODO if released
    }
  }
  sendUDPOnce = false;
}


//-----------------------------------------------
void sendNoLeftNoRightWebSockets() {
  bSendNoLeftNoRightMessage = true;
  sendUDPOnce = true;
}


//-----------------------------------------------
void sendXWebSockets() {
  bSendXMessage = true;
  sendUDPOnce = true;
}
//-------------------------------------------------
void sendClickWebSockets() {
  bSendClickMessage = true;
  sendUDPOnce = true;
}
//
////-------------------------------------------------
//void update_serialInput() {
//
//  String myTextLeft = "left\n";
//  String myTextRight = "right\n";
//  String myTextClick = "click\n";
//
//  String mySerialReceivedString = "";
//  if (USE_SERIAL.available() > 0) //Checks is there any data in buffer
//  {
//    //USE_SERIAL.print("We got:");
//    //USE_SERIAL.println(char(USE_SERIAL.read()));  //Read serial data byte and send back to serial monitor
//    mySerialReceivedString = Serial.readString();
//    USE_SERIAL.print(mySerialReceivedString);
//
//    if (mySerialReceivedString == myTextLeft) {
//      USE_SERIAL.println("We send LEFT command");
//      bSendLeftMessage = true;
//    }
//
//    if (mySerialReceivedString == myTextRight) {
//      USE_SERIAL.print("We send RIGHT command");
//      bSendRightMessage = true;
//    }
//
//    if (mySerialReceivedString == myTextClick) {
//      USE_SERIAL.print("We send Click command");
//      bSendClickMessage = true;
//    }
//
//    sendUDPOnce = true;
//    messageTimestamp = millis();
//  }
//  else
//  {
//    //    USE_SERIAL.println("Hello World..."); //Print Hello word every one second
//    //    delay(1000);                      // Wait for a second
//  }
//}
