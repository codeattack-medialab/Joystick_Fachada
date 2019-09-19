/*
   WebSocketClientSocketIO.ino

    Created on: 06.06.2016

*/

#define LED D4            //  (D0)in V2 && D4 in Lolin V3

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

bool doConectRoom = true;
bool doItOnce = true;
bool bSendRightMessage = false;
bool bSendLeftMessage = false;
bool bSendClickMessage = false;
bool bDebugSockets = true;

#define USE_SERIAL Serial
//#define HAS_SSL true

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      USE_SERIAL.printf("[IOc] Disconnected!\n");
      break;
    case sIOtype_CONNECT:
      USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);
      break;
    case sIOtype_EVENT:
      USE_SERIAL.printf("[IOc] get event: %s\n", payload);
      //Do something with this event
      doBlikingLED(1);//seconds blinking
      break;
    case sIOtype_ACK:
      USE_SERIAL.printf("[IOc] get ack: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      USE_SERIAL.printf("[IOc] get error: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      USE_SERIAL.printf("[IOc] get binary: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
      hexdump(payload, length);
      break;
  }
}

void setup() {
  // USE_SERIAL.begin(921600);
  USE_SERIAL.begin(115200);

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

  // disable AP
  if (WiFi.getMode() & WIFI_AP) {
    WiFi.softAPdisconnect(true);
  }

  WiFiMulti.addAP("waifay", "internes");

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  String ip = WiFi.localIP().toString();
  USE_SERIAL.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

  // server address, port and URL
  socketIO.begin("192.168.43.244", 8004);//188.166.57.94 //pachucho.lowfill.com

  // event handler
  socketIO.onEvent(socketIOEvent);

  //LED response
  pinMode(LED, OUTPUT);   // Defining pin as output

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

//----------------------------------------------------
void sendMessageRoom(String _message2Send1, String _message2Send2) {
  ////////////////////////////////////////////////
  // creat JSON message for Socket.IO (event)
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();

  // add evnet name
  // Hint: socket.on('event_name', ....
  array.add(_message2Send1);
  array.add(_message2Send2);

  // JSON to String (serializion)
  String output;
  serializeJson(doc, output);

  // Send event
  socketIO.sendEVENT(output);

  // Print JSON for debugging
  if (bDebugSockets)USE_SERIAL.println(output);
}

//----------------------------------------------------
void sendMessage(String _message2Send1, String _message2Send2, String _from) {

  // creat JSON message for Socket.IO (event)
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();

  // add evnet name
  // Hint: socket.on('event_name', ....
  array.add(_message2Send1);

  // add payload (parameters) for the event
  JsonObject param1 = array.createNestedObject();
  param1["sender"] = _from;//"4";//from
  param1["message"] = _message2Send2;

  // JSON to String (serializion)
  String output;
  serializeJson(doc, output);

  // Send event
  socketIO.sendEVENT(output);

  // Print JSON for debugging
  if (bDebugSockets)USE_SERIAL.println(output);
}


//------------------------------------------------------
unsigned long messageTimestamp = 0;
void loop() {
  socketIO.loop();
  update_serialInput();

  uint64_t now = millis();
  if (now - messageTimestamp > 10000 && doConectRoom == true) {
    USE_SERIAL.println("Enter enter to this Room!");
    sendMessageRoom("addJoystick", "/8");//4:4
    doConectRoom = false;
  }
  else if (now - messageTimestamp > 100 && doItOnce == true) {
    sendMessageRoom("addJoystick", "/8");//4:4
    if (doItOnce) {
      if (bSendLeftMessage) {
        sendMessage("toServer", "Left", "4");
        bSendLeftMessage = false;
      }
      if (bSendRightMessage) {
        sendMessage("toServer", "Right", "4");
        bSendRightMessage = false;
      }
            if (bSendClickMessage) {
        sendMessage("toServer", "Click", "4");
        bSendClickMessage = false;
      }
    }
    doItOnce = false;
  }


}

//-------------------------------------------------
void update_serialInput() {

  String myTextLeft = "left\n";
  String myTextRight = "right\n";
  String myTextClick = "click\n";

  String mySerialReceivedString = "";
  if (USE_SERIAL.available() > 0) //Checks is there any data in buffer
  {
    //USE_SERIAL.print("We got:");
    //USE_SERIAL.println(char(USE_SERIAL.read()));  //Read serial data byte and send back to serial monitor
    mySerialReceivedString = Serial.readString();
    USE_SERIAL.print(mySerialReceivedString);

    if (mySerialReceivedString == myTextLeft) {
      USE_SERIAL.println("We send LEFT command");
      bSendLeftMessage = true;
    }

    if (mySerialReceivedString == myTextRight) {
      USE_SERIAL.print("We send RIGHT command");
      bSendRightMessage = true;
    }

    if (mySerialReceivedString == myTextClick) {
      USE_SERIAL.print("We send Click command");
      bSendClickMessage = true;
    }

    doItOnce = true;
    messageTimestamp = millis();
  }
  else
  {
    //    USE_SERIAL.println("Hello World..."); //Print Hello word every one second
    //    delay(1000);                      // Wait for a second
  }
}
