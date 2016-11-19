
#include <WiFiEsp.h>
#include <MQTTClient.h>

char *ssid = "Toca";
char *pass = "32247030TO";

WiFiEspClient net;
MQTTClient client;

const int led1 = 13;
const int led2 = 12;

unsigned long lastMillis = 0;

int status = WL_IDLE_STATUS;     // the Wifi radio's status

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(11, 10); // RX, TX
#endif

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  
  Serial.begin(115200);
  Serial1.begin(115200);
  WiFi.init(&Serial1);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("You're connected to the network");

  Serial.println("Conectando ao mosquitto");
  //client.begin("192.168.0.102", net);
  //client.setClient(net);
  //client.begin("10.30.2.127", 8883, net);

  connect();
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "m", "m")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.publish("casa/lampada", "wakeup");
  client.subscribe("casa/lampada/+");
  // client.unsubscribe("casa/escritorio/+");
}

void loop() {
  client.loop();

  if(!client.connected()) {
    connect();
  }

  // publica uma mensagem a cada segundo.
  if(millis() - lastMillis > 1000) {
    lastMillis = millis();
    //client.publish("casa/lampada", "lampadaON");
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  int v;
  //sscanf(payload.toChar(), "%d", &v);
  v = payload.toInt();
  Serial.println(v);

  MQTTMessage *msg;
  msg->payload = payload.c_str();
  msg->length = length;
  msg->retained = true;
  
  if(topic == "casa/lampada/1")
  {
    msg->topic = "casa/lampada/1/status";
    client.publish(msg);
    
    //client.publish("casa/lampada/1/status", payload, true);
    digitalWrite(led1, v);
  }
  if(topic == "casa/lampada/2")
  {
    msg->topic = "casa/lampada/2/status";
    client.publish(msg);
    
    //client.publish("casa/lampada/2/status", payload, true);
    digitalWrite(led2, v);
  }
}
