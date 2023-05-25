#include <Wire.h>

#define led1 8
#define led2 9
#define led3 10
#define pot1 A1
#define pot2 A2
#define pot3 A3
#include "Button.h"

Button botao(A0);

byte valor1 = 0;
byte valor2 = 0;
byte valor3 = 0;
byte codigo1 = 0;
byte codigo2 = 0;
byte codigo3 = 0;
byte intervalo = 8;

unsigned long tempoAntes = 0;
unsigned long tempoDepois = 0;
int correctstate = 0;
int err = 0;

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent); 
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);
  pinMode(pot3, INPUT);
  randomSeed(analogRead(0));

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  tempoAntes = millis();

  Serial.begin(9600);
  botao.begin();

  codigo1 = random(0, 73);
  codigo2 = random(0, 73);
  codigo3 = random(0, 73);
}

void loop() {
    valor1 = map(analogRead(pot1), 0, 1023, 0, 80);
    valor2 = map(analogRead(pot2), 0, 1023, 0, 80);
    valor3 = map(analogRead(pot3), 0, 1023, 0, 80);
    tempoDepois = millis();
    if(tempoDepois - tempoAntes >= 210) {
      Serial.print("Cod1: ");
      Serial.print(codigo1);
      Serial.print(" Pot1: ");
      Serial.print(valor1);
      Serial.print(" Cod2: ");
      Serial.print(codigo2);
      Serial.print(" Pot2: ");
      Serial.print(valor2);
      Serial.print(" Cod3: ");
      Serial.print(codigo3);
      Serial.print(" Pot3: ");
      Serial.println(valor3);
      if (valor1 >= codigo1 && valor1 < codigo1 + intervalo) {
        digitalWrite(led1, HIGH);
      }
      else {
        digitalWrite(led1, LOW);
      }
      if (valor2 >= codigo2 && valor2 < codigo2 + intervalo) {
        digitalWrite(led2, HIGH);
      }
      else {
        digitalWrite(led2, LOW);
      }
      if (valor3 >= codigo3 && valor3 < codigo3 + intervalo) {
        digitalWrite(led3, HIGH);
      }
      else {
        digitalWrite(led3, LOW);
      }
      tempoAntes = millis();
    }
    if (botao.pressed()) {
      if(digitalRead(led1) == HIGH && digitalRead(led2) == HIGH && digitalRead(led3) == HIGH) {
        correctstate = 1;
      } else {
        err = 1;
      }
    }
}

void requestEvent() {
  if(correctstate == 1) {
        Wire.write(100);
  } else {
        if (err == 1){
          Wire.write(1);
          err = 0;
        }
  }
}
