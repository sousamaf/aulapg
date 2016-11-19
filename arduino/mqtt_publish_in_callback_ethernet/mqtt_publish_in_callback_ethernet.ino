/*
 Publishing in the callback

  - connects to an MQTT server
  - subscribes to the topic "inTopic"
  - when a message is received, republishes it to "outTopic"

  This example shows how to publish messages within the
  callback function. The callback function header needs to
  be declared before the PubSubClient constructor and the
  actual callback defined afterwards.
  This ensures the client reference in the callback function
  is valid.

*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xEF };
IPAddress ip(10, 30, 2, 99);
IPAddress server(10, 30, 2, 127);

const int led1 = 31;
const int led2 = 53;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient client(server, 8883, callback, ethClient);

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
 
  // Allocate the correct amount of memory for the payload copy
  byte* p = (byte*)malloc(length);
  
  // Copy the payload to the new buffer
  memcpy(p,payload,length);

  //converte o valor recebido para string
  //String v = String((char*)p);

  // converte o valor recebido para inteiro.
  int v;
  sscanf(p, "%d", &v);
  Serial.println(v);
  
  if(strcmp(topic, "casa/lampada/1") == 0)
  {
    client.publish("casa/lampada/1/status", p, length, true);
    digitalWrite(led1, v);
  }
  if(strcmp(topic, "casa/lampada/2") == 0)
  {
    client.publish("casa/lampada/2/status", p, length, true);
    digitalWrite(led2, v);
  }
  
  // Free the memory
  free(p);
}

void setup()
{
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  
  Serial.begin(115200);
  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient", "m", "m")) {
    client.publish("casa/lampada","wakeup");
    client.subscribe("casa/lampada/+", 1);
  }
}

void loop()
{
  client.loop();
}
