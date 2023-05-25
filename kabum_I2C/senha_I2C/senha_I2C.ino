#include "Keypad.h" // Biblioteca do codigo
#include <Wire.h> 
#include <String.h>

const byte LINHAS = 4; // Linhas do teclado
const byte COLUNAS = 4; // Colunas do teclado
String senhaf = "523";
String senha = "";
int correctstate = 0;
int err = 0;
  
const char COMBINACAO1[LINHAS][COLUNAS] = { // Matriz de caracteres (mapeamento do teclado)
  {'8', '4', '6', 'A'},
  {'9', '2', '5', 'B'},
  {'0', '1', '3', 'C'},
  {'*', '7', '#', 'D'}
};

const char COMBINACAO2[LINHAS][COLUNAS] = { // Matriz de caracteres (mapeamento do teclado)
  {'2', '3', '1', 'A'},
  {'5', '6', '4', 'B'},
  {'8', '9', '7', 'C'},
  {'*', '0', '#', 'D'}
};

const char COMBINACAO3[LINHAS][COLUNAS] = { // Matriz de caracteres (mapeamento do teclado)
  {'9', '6', '5', 'A'},
  {'2', '7', '4', 'B'},
  {'0', '3', '1', 'C'},
  {'*', '8', '#', 'D'}
};

byte PINOS_LINHAS[LINHAS] = {1, 2, 3, 4}; // Pinos de conexao com as linhas do teclado
byte PINOS_COLUNAS[COLUNAS] = {8, 9, 10, 11}; // Pinos de conexao com as colunas do teclado

Keypad teclado_personalizado = Keypad(makeKeymap(COMBINACAO1), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS); // Inicia teclado

void setup() {
  Serial.begin(9600); 
  Wire.begin(7);
  Wire.onRequest(requestEvent); 
}
	
void loop() {

  char key = teclado_personalizado.getKey(); // Atribui a variavel a leitura do teclado

  if (key >= '0' && key <= '9') {
      senha += key;
  }

  if (key == 'C') {
      senha.remove(senha.length() - 1, 1);
  }

  if(key == 'D'){
    Serial.print(senha);
    if(senha == senhaf){
      correctstate = 1;
    }else {
      err = 1;
      senha = "";
    }
  }
  

  delay(100);
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
