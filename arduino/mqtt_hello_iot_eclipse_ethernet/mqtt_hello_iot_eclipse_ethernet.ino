#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// lembre-se de trocar os valores hex.
byte mac[]    = {  0xDE, 0xE7, 0xBA, 0x01, 0xFE, 0xEF };

const int led1 = 6;
const int led2 = 7;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient client("iot.eclipse.org", 1883, callback, ethClient);

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
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  
 
  Serial.println("setup");
  
  Ethernet.begin(mac); // sem IP, pega via DHCP.
  printIPAddress();

  
  if (client.connect("arduinoClient09183")) { //Lempre-se de trocar o nome. 
    client.publish("casa/lampada","wakeup");
    client.subscribe("casa/lampada/+", 1);
  }
}

void loop()
{
  client.loop();
}

void printIPAddress()
{
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();
}
