/*
  UDPSendReceive.pde:
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender

  A Processing sketch is included at the end of file that can be used to send
  and received messages for testing with a computer.

  created 21 Aug 2010
  by Michael Margolis

  This code is in the public domain.

  adapted from Ethernet library examples
*/


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "waifay"
#define STAPSK  "internes"
#endif

unsigned int localPort = 33333;      // local port to listen on
IPAddress remoteIp(192, 168, 1, 109);

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged\r\n";       // a string to send back

WiFiUDP Udp;

bool sendUDPOnce = true;
bool bSendNoLeftNoRightMessage = false;
bool bSendXMessage = false;
bool bSendClickMessage = false;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
}


//------------------------------------------------------
void loop_udp() {

  if (sendUDPOnce) {
    if (bSendNoLeftNoRightMessage) {
      sendUdp( String(1) + "/X/" + String(0.0));
      Serial.println("bSendNoLeftNoRightMessage!");
      bSendNoLeftNoRightMessage = false;
    }
    else {
      if (bSendXMessage) {
        //sendMessage("toServer", "X/" + String(mapAnalogX_ky023), String(idJoystick)); //String(mapAnalogX_ky023)
        sendUdp( String(1) + "/X/" +  String(1.0)); //"mapAnalogX_ky023" -> -1 to +1
        Serial.println("bSendXMessage!");
        bSendXMessage = false;
      }
    }


  }
  sendUDPOnce = false;
}

//-----------------------------------------------------
void sendUdp(String _buffer) {

  // send a reply, to the IP address and port that sent us the packet we received
  Udp.beginPacket(remoteIp, localPort);
  _buffer.toCharArray(packetBuffer, _buffer.length());
  Serial.println("myBuffer = ");
  Serial.println(packetBuffer);

  Udp.write(packetBuffer);
  Udp.endPacket();

  Serial.println("after");
}

void loop() {

  sendUdp( String(1) + "/X/" + String(0.0));
  delay(50);
}

/*
  test (shell/netcat):
  --------------------
	  nc -u 192.168.esp.address 8888
*/
