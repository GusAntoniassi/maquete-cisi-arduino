// Código da Maquete do CISI

// Escrito por Gustavo Antoniassi
// Baseado no exemplo Wire Slave Receiver 
// de Nicholas Zambetti <http://www.zambetti.com>

#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

int portaLuzes[] = {A0, A1, A2};
bool estadoLuzes[] = {LOW, LOW, LOW};

Servo tranca;
int PORTA_TRANCA = 11;
int ANGULO_PORTA_ABERTA = 90;
int ANGULO_PORTA_FECHADA = 180;

int PORTA_BUZZER = 9;

boolean statusAr = 0;
int tempAr = 0;
int tempAmbiente = 0;

// Notas para o buzzer (em hertz)
float NOTA_D7 = 2349.32;
float NOTA_E7 = 2637.02;
float NOTA_A7 = 3520.00;

boolean tocarBuzzerAr = false;

void setup() {
  toneInicializacao();

  pinMode(PORTA_TRANCA, OUTPUT);
  tranca.attach(PORTA_TRANCA);
  tranca.write(ANGULO_PORTA_FECHADA);
  delay(150);
  tranca.write(ANGULO_PORTA_ABERTA);
  
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  //Serial.begin(9600);           // start serial for output

  for (int i = 0; i < sizeof(portaLuzes); i++) {
    pinMode(portaLuzes[i], OUTPUT);
    digitalWrite(portaLuzes[i], HIGH);
    delay(100);
    digitalWrite(portaLuzes[i], LOW);
  }
}

void loop() {
  if (tocarBuzzerAr) {
    toneArCondicionado();
    tocarBuzzerAr = false;
  }
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  int i = 0;
  String comandos;
  
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive byte as a character
    comandos += c;
  }
  
  // Formato: Led1;Led2;Led3;Porta;Ar;TempAr(00 a 99);TempAmbiente(00 a 99)
  //Wire.write("1;0;1;1;1;18;20");
  /*
   * Posição led 1 = 0
   * Posição led 2 = 2
   * Posição led 3 = 4
   */
  // Setar o estado das luzes
  for (int i = 0; i < 3; i++) {
    estadoLuzes[i] = charToBoolean(comandos[i*2]); // i*2 para compensar pelo ponto e vírgula
  }

  toneArCondicionado();
  atualizaLuzes();
  //Serial.println(comandos[6]);
  toggleTranca(charToBoolean(comandos[6]));

  // Ar condicionado: 
  // comandos[8] = liga/desliga
  // comandos[10] = dezena
  // comandos[11] = unidades
  int novoStatusAr = charToBoolean(comandos[8]);
  int novaTempAr = (comandos[10] * 10) + comandos[11];
  //Serial.println("comandos 10");
  //Serial.println(comandos[10]);
  //Serial.println("comandos 11");
  //Serial.println(comandos[11]);
  //Serial.println(novaTempAr);

  if (statusAr != novoStatusAr) {
    statusAr = novoStatusAr;
    tocarBuzzerAr = true;
  }

  if (statusAr == 1 && novaTempAr != tempAr) {
    tempAr = novaTempAr;
    tocarBuzzerAr = true;
  }
  
  // @TODO: ler sensor DHT
  
  // Máximo 32 bytes de resposta
  //Wire.write("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbb");

  comandos = "";
}

boolean charToBoolean(char c) {
  return c != '0';
}

void atualizaLuzes() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(portaLuzes[i], estadoLuzes[i]); 
  }
}

void toggleTranca(bool trancar) {
  if (trancar) {
    tranca.write(ANGULO_PORTA_FECHADA);
  } else {      
    tranca.write(ANGULO_PORTA_ABERTA);
  }
}

void toneInicializacao() {
  tone(PORTA_BUZZER, NOTA_D7);
  delay(100);
  noTone(PORTA_BUZZER);
  delay(25);
  tone(PORTA_BUZZER, NOTA_E7);
  delay(100);
  noTone(PORTA_BUZZER);
  delay(50);
  tone(PORTA_BUZZER, NOTA_A7);
  delay(200);
  noTone(PORTA_BUZZER);
}

void toneArCondicionado() {
  tone(PORTA_BUZZER, NOTA_A7);
  delay(200);
  noTone(PORTA_BUZZER);
  delay(200);
  tone(PORTA_BUZZER, NOTA_A7);
  delay(200);
  noTone(PORTA_BUZZER);
}

