// Código da Maquete do CISI

// Escrito por Gustavo Antoniassi
// Baseado no exemplo Wire Slave Receiver 
// de Nicholas Zambetti <http://www.zambetti.com>

#include <Wire.h>

int portaLuzes[] = {2, 3, 4};
bool estadoLuzes[] = {LOW, LOW, LOW};

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output

  for (int i = 0; i < sizeof(portaLuzes); i++) {
    pinMode(portaLuzes[i], OUTPUT); 
  }
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer

  gerenciaLuzes(x);
}

void gerenciaLuzes(int indice) {
  // Master vai passar luz "1", "2" ou "3"
  indice -= 1;
  estadoLuzes[indice] = !estadoLuzes[indice];
  digitalWrite(portaLuzes[indice], estadoLuzes[indice]);
}

