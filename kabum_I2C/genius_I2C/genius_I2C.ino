#include <Wire.h>
#include "Button.h"

Button greenButton(2);
Button redButton(3);
Button blueButton(4);

int sequencia[5] = {};                                 
int rodada_atual = 0;     
int pressed = 0;                               
int passo_atual_na_sequencia = 0;                        

const int rled = 10;
const int gled = 11;
const int bled = 12;

const int greenLED = 6;
const int redLED = 7;
const int blueLED = 8;

int correctstate = 0;
int err = 0;

int errors = 0;
 
int botao_pressionado = 0;  // Inicializa botão em 0
int perdeu_o_jogo = false;  // Variável fim de jogo
 
void setup() {
  greenButton.begin();
  redButton.begin();
  blueButton.begin();

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);
  Wire.begin(6);
  Wire.onRequest(requestEvent); 
  Serial.begin(9600);
  
  randomSeed(analogRead(0));         // Gera um valor entre 0 e 1023
}
 
void loop() {
    if(greenButton.pressed() || blueButton.pressed() || redButton.pressed()){
      pressed = 1;
    }
    if (perdeu_o_jogo) {  
      digitalWrite(rled, HIGH);
      digitalWrite(gled, HIGH);
      digitalWrite(bled, HIGH);
      delay(1000);
      digitalWrite(rled, LOW);
      digitalWrite(gled, LOW);
      digitalWrite(bled, LOW);
      delay(1000);          
      sequencia[5] = {};
      rodada_atual = 0;
      passo_atual_na_sequencia = 0;
      err = 1;
      errors++;
      perdeu_o_jogo = false;
  }

  if(errors == 3){
    while(true){
      digitalWrite(rled, HIGH);
    }
  }
  
  if(passo_atual_na_sequencia == 5){
    correctstate = 1;
    while(true){
      digitalWrite(gled, HIGH);
    }
  } else if (pressed == 1){
      proximaRodada();        // Inicia a próxima rodada
      reproduzirSequencia();  // Reproduz a sequencia atual
      aguardarJogador();      // Aguarda jogadas
      delay(1000);            // 1 segundo de uma jogada a outra
  }
}
 
void proximaRodada() {
  int numero_sorteado = random(0, 3);
  sequencia[rodada_atual++] = numero_sorteado;
}

void flash(int color, int durationMs) {
  if(color == 0){ //red
    digitalWrite(rled, HIGH);
    delay(durationMs);
    digitalWrite(rled, LOW);
  }
  if(color == 1){ //green
    digitalWrite(gled, HIGH);
    delay(durationMs);
    digitalWrite(gled, LOW);
  }
  if(color == 2){ //blue
    digitalWrite(bled, HIGH);
    delay(durationMs);
    digitalWrite(bled, LOW);
  }
}


void reproduzirSequencia() {
  for (int i = 0; i < rodada_atual; i++) {
    flash(sequencia[i], 500);
    delay(300);
  }
}
 
void aguardarJogador() {  // Aguarda o inicio do jogo
  for (int i = 0; i < rodada_atual; i++) {
    aguardarJogada();
    verificarJogada();
 
    if (perdeu_o_jogo) {
      break;
    }
 
    passo_atual_na_sequencia++;
  }
 
  passo_atual_na_sequencia = 0;  // Redefine a variável
}
 
void aguardarJogada() {
  boolean jogada_efetuada = false;
  while (!jogada_efetuada) {
      if (greenButton.pressed()) {
        botao_pressionado = 1; 
        digitalWrite(greenLED, HIGH);
        delay(500);
        digitalWrite(greenLED, LOW);
 
        jogada_efetuada = true;
      } else if (redButton.pressed()) {
        botao_pressionado = 0; 
        digitalWrite(redLED, HIGH);
        delay(500);
        digitalWrite(redLED, LOW);
 
        jogada_efetuada = true;
      } else if (blueButton.pressed()) {
        botao_pressionado = 2; 
        digitalWrite(blueLED, HIGH);
        delay(500);
        digitalWrite(blueLED, LOW);
 
        jogada_efetuada = true;
      }
    delay(10);
  }
}
 
void verificarJogada() {
  if (sequencia[passo_atual_na_sequencia] != botao_pressionado) { 
    perdeu_o_jogo = true;
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
