int led = 13;
int botao = 2;
volatile boolean vLed = false;
void setup() {
  pinMode(led, OUTPUT);
  pinMode(botao, INPUT_PULLUP);
  attachInterrupt(1, algoAconteceu, LOW);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(2, vLed);
  //Serial.println("loop");
}


void algoAconteceu() {
  vLed = !vLed;
  Serial.println("interrupcao");
}

