#include <String.h>
#include <Servo.h>
#include <time.h>
#include "LiquidCrystal_I2C.h"
#include "Button.h"
#include "Keypad.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define led1 23
#define led2 24
#define led3 25
#define ledf 22
#define buzzer A0

int s = 0;

int modcofre = 0;
int modsenha = 0;
int modgenius = 0;
int err = 0;

const int ledgreenexp = 9;
const int ledredexp = 8;
 
unsigned SecondsRemaining = 0;
unsigned long PreviousMillis = 0;
unsigned MinutesRemaining;
unsigned TimeElapsed = 0;
int t = SecondsRemaining;
const int nota_C4 = 262;
const int nota_DS7 = 2489;
const int nota_E7 = 2637;

//var kabum

Servo bandeira;
Servo bandeira2;
#define vibrador 53

int pos = 0;

// var senha

const byte LINHAS = 4; // Linhas do teclado
const byte COLUNAS = 4; // Colunas do teclado
String senhaf = "874746";
String senha = "";
  
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

byte PINOS_LINHAS[LINHAS] = {40, 41, 42, 43}; // Pinos de conexao com as linhas do teclado
byte PINOS_COLUNAS[COLUNAS] = {44, 45, 46, 47}; // Pinos de conexao com as colunas do teclado

Keypad teclado_personalizado1 = Keypad(makeKeymap(COMBINACAO1), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS);  
Keypad teclado_personalizado2 = Keypad(makeKeymap(COMBINACAO2), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS);  
Keypad teclado_personalizado3 = Keypad(makeKeymap(COMBINACAO3), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS);  

// var corta fio

const int fioY = 11;
const int fioR = 12;
const int fioG = 10;
const int fioB = 13 ;
boolean Y = 0;
boolean R = 0;
boolean G = 0;
boolean B = 0;
int D_current[]={G,R,B,Y};

// led central 

int rselected = 39;
int gselected = 35;
int bselected = 37;

int selectedcombination = 1;

int D1[]={0,0,1,1};
int D2[]={1,1,0,0};
int D3[]={1,0,0,0};

int state[]={0,0,0,0};

int modcurrent = 0;

// var genius

Button greenButton(30);
Button redButton(31);
Button blueButton(32);

#define SEQ 6
int sequencia[SEQ] = {};                                 
int rodada_atual = 0;     
int pressed = 0;                               
int passo_atual_na_sequencia = 0;                        

const int rled = 48;
const int gled = 49;
const int bled = 50;

const int greenLED = 29;
const int redLED = 6;
const int blueLED = 7;
 
int botao_pressionado = 0;  // Inicializa botão em 0
int perdeu_o_jogo = false;  // Variável fim de jogo

// var cofre

#define led1c 26
#define led2c 27
#define led3c 28
#define pot1 A1
#define pot2 A7
#define pot3 A3

int correct1 = 0;
int correct2 = 0;
int correct3 = 0;

Button botao(A4);

byte valor1 = 0;
byte valor2 = 0;
byte valor3 = 0;
byte codigo1 = 0;
byte codigo2 = 0;
byte codigo3 = 0;
byte intervalo = 8;

unsigned long tempoAntes = 0;
unsigned long tempoDepois = 0;

int seedgerada = 0;
int beeped1 = 0;
int beeped2 = 0;
int beeped3 = 0;

void setup() {
  Serial.begin(9600); 
  pinMode(ledredexp,OUTPUT);
  pinMode(ledgreenexp,OUTPUT);
  pinMode(vibrador,OUTPUT);
  analogWrite(vibrador, 255);  
  // corta fio setup
  pinMode(fioY,INPUT_PULLUP);
  pinMode(fioR,INPUT_PULLUP);
  pinMode(fioG,INPUT_PULLUP);
  pinMode(fioB,INPUT_PULLUP);
  pinMode(rselected,OUTPUT);
  pinMode(gselected,OUTPUT);
  pinMode(bselected,OUTPUT);


  // cofre setup

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);
  pinMode(pot3, INPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  tempoAntes = millis();

  Serial.begin(9600);
  botao.begin();

  // genius setup

  greenButton.begin();
  redButton.begin();
  blueButton.begin();

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);

  // master setup

  pinMode(led1, OUTPUT); 
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(ledf, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(ledf, LOW);
  lcd.init(); //inicialização do lcd
  lcd.backlight(); 
  lcd.clear();
  lcd.begin(16,2); //Iniciar a comunicação com o display

  // kabum setup

  bandeira.attach(4);
  bandeira.write(175);

  bandeira2.attach(5);
  bandeira2.write(pos);
  
  //INTRO:
  lcd.print("Desarme a Bomba!");
  lcd.setCursor(0,1);
  delay(300);
  lcd.write(byte(62));

  delay(300);
  lcd.write(byte(58));
  delay(300);
  lcd.write(byte(41));
  delay(300);
  //lcd.clear();
  delay(1000); 
  analogWrite(vibrador, 0);
  //tempo 
  
  MinutesRemaining = 02;
  SecondsRemaining = 04;
}

void loop() {
  countdown();

  if(seedgerada == 0){
    Serial.print("Random seed: ");
    Serial.println(analogRead(A6));
    Serial.println("-------------------------------");
    randomSeed(analogRead(A6));
    codigo1 = random(0, 73);
    codigo2 = random(0, 73);
    codigo3 = random(0, 73);
    
    selectedcombination = random(1, 4);

    if(selectedcombination == 1){
      digitalWrite(rselected, HIGH);
    }
    if(selectedcombination == 2){
      digitalWrite(gselected, HIGH);
    }
    if(selectedcombination == 3){
      digitalWrite(bselected, HIGH);
    }
    
    seedgerada = 1;
  }
  
  //------------
  // senha loop
    char key = "";
    if(selectedcombination == 1){
      key = teclado_personalizado1.getKey();
    } else if(selectedcombination == 2){
      key = teclado_personalizado2.getKey();
    } else if(selectedcombination == 3){
      key = teclado_personalizado3.getKey();
    }
  
    if(modsenha == 0){
      if (key >= '0' && key <= '9') {
          senha += key;
          lcd.setCursor(1,1);
          lcd.print(senha);
          lcd.setCursor(0,1);
      }
    
      if (key == 'C') {
          senha.remove(senha.length() - 1, 1);
      }
    
      if(key == 'D'){
        if(senha == senhaf){
          modsenha = 1;
        }else {
          err++;
          senha = "";
        }
      }
    }
    if(modsenha != 0){
        lcd.setCursor(1,1);
        lcd.print("SENHA CORRETA");        
        lcd.setCursor(0,1);
    }

     // genius loop

     if(modgenius != 0){
        digitalWrite(gled, HIGH);
     }

     if(greenButton.pressed() && selectedcombination == 1 || blueButton.pressed() && selectedcombination == 2 || redButton.pressed() && selectedcombination == 3){
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
        sequencia[SEQ] = {};
        rodada_atual = 0;
        passo_atual_na_sequencia = 0;
        err++;
        perdeu_o_jogo = false;
    } else if (pressed == 1 && modgenius == 0){
        proximaRodada();        // Inicia a próxima rodada
        reproduzirSequencia();  // Reproduz a sequencia atual
        aguardarJogador();      // Aguarda jogadas
        delay(1000);            // 1 segundo de uma jogada a outra
        countdown();
    }

    // cofre loop

    if(modcofre == 0){
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
          digitalWrite(led1c, HIGH);
          correct1 = 1;
        }
        else {
          correct1 = 0;
          digitalWrite(led1c, LOW);
        }
        if (valor2 >= codigo2 && valor2 < codigo2 + intervalo) {
          correct2 = 1;
          digitalWrite(led2c, HIGH);
        }
        else {
          digitalWrite(led2c, LOW);
          correct2 = 0;
        }
        if (valor3 >= codigo3 && valor3 < codigo3 + intervalo) {
          digitalWrite(led3c, HIGH);
          correct3 = 1;
        }
        else {
          digitalWrite(led3c, LOW);
          correct3 = 0;
        }
        tempoAntes = millis();
      }
      if (botao.pressed()) {
        if(correct1 + correct2 + correct3 == 3) {
          modcofre = 1;
        } else {
          err++;
        }
      }
    }

     if(err == 1){
        digitalWrite(led1, HIGH);
        if(beeped1 == 0){
          beeperr(err);
          beeped1 = 1;
          digitalWrite(ledredexp,HIGH);
          analogWrite(vibrador, 255);
          delay(50);
          digitalWrite(ledredexp,LOW);
          analogWrite(vibrador, 0);
        }
     }
     if(err == 2){
        digitalWrite(led2, HIGH);
        if(beeped2 == 0){
          beeperr(err);
          beeped2 = 1;
          digitalWrite(ledredexp,HIGH);
          analogWrite(vibrador, 255);
          delay(50);
          digitalWrite(ledredexp,LOW);
          analogWrite(vibrador, 0);
        }
     }
     if(err == 3){
        if(beeped3 == 0){
          beeperr(err);
          beeped3 = 1;
        }
        bandeira.write(85);
        bandeira2.write(85);
        
        if(s<10){
        lcd.clear();
        lcd.print("KBUUUUUUM!!");
        tone(buzzer, nota_C4,1000);}
       
        do{
        acendeleds();
        analogWrite(vibrador, 255);
        delay(250);
        apagaleds();
        analogWrite(vibrador, 0 );
        delay(250);
        s++;
        }while(s<25);
        while(s>=25){acendeleds();}
        
     }

     if(modcofre != 0 && modsenha != 0 && modgenius != 0 && modcurrent !=0){
        digitalWrite(ledf, HIGH);
        digitalWrite(ledgreenexp,HIGH);
        while(true){
          lcd.clear();
          lcd.print("BOMB DISARMED IN");
          lcd.setCursor(0,1);
          lcd.print("0");
          lcd.print(TimeElapsed / 60);
          lcd.print(":");
          if(TimeElapsed % 60 < 10)
          {
            lcd.print("0");
           }
          lcd.print(TimeElapsed % 60);
          delay(1000);
          lcd.setCursor(0,0);
          lcd.clear();
        }
     }

  if(modcurrent == 0){
    corrente();
    if(selectedcombination == 1){
      Serial.print(D1[0]);
      Serial.print(D1[1]);
      Serial.print(D1[2]);
      Serial.print(D1[3]);
      Serial.print("-----------");
      if(D_current[0] == D1[0] && D_current[1] == D1[1] && D_current[2] == D1[2] && D_current[3] == D1[3]){
         modcurrent = 1;
      }
      for(int ii = 0; ii < 4; ii++){
        if(D_current[ii] != 0 && D1[ii] != 1 && D_current[ii] != state[ii]){
          err++;
          for(int ij = 0; ij < 4; ij++){
            state[ij] = D_current[ij];
          }
        }
      }
    }

    if(selectedcombination == 2){
      Serial.print(D2[0]);
      Serial.print(D2[1]);
      Serial.print(D2[2]);
      Serial.print(D2[3]);
      Serial.print("-----------");
      if(D_current[0] == D2[0] && D_current[1] == D2[1] && D_current[2] == D2[2] && D_current[3] == D2[3]){
         modcurrent = 1;
      }

      for(int ii = 0; ii < 4; ii++){
        if(D_current[ii] != 0 && D2[ii] != 1 && D_current[ii] != state[ii]){
          err++;
          for(int ij = 0; ij < 4; ij++){
            state[ij] = D_current[ij];
          }
        }
      }
    }

    if(selectedcombination == 3){
      Serial.print(D3[0]);
      Serial.print(D3[1]);
      Serial.print(D3[2]);
      Serial.print(D3[3]);
      Serial.print("-----------");
      if(D_current[0] == D3[0] && D_current[1] == D3[1] && D_current[2] == D3[2] && D_current[3] == D3[3]){
         modcurrent = 1;
      }

      for(int ii = 0; ii < 4; ii++){
        if(D_current[ii] != 0 && D3[ii] != 1 && D_current[ii] != state[ii]){
          err++;
          for(int ij = 0; ij < 4; ij++){
            state[ij] = D_current[ij];
          }
        }
      }
    }
  }
}

void beeperr(int err) {
  if(err == 1){
    tone(buzzer, nota_C4,100);
  }else if(err == 2){
    tone(buzzer,nota_DS7,100);
  }else if(err == 3){
    tone(buzzer,nota_E7,100);
  }
}

// genius functions

void proximaRodada() {
  int numero_sorteado = random(0, 3);
  Serial.print("COR: ");
  Serial.print(numero_sorteado);
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
    if(rodada_atual == SEQ){
        if(modgenius == 0){
          modgenius = 1;
        }
        break;
     }
    flash(sequencia[i], 1000);
    countdown();
    correct();
  }
}
 
void aguardarJogador() {  // Aguarda o inicio do jogo
  for (int i = 0; i < rodada_atual; i++) {
    aguardarJogada();
    verificarJogada();
    countdown();
 
    if (perdeu_o_jogo) {
      break;
    }
    if(rodada_atual == SEQ){
        if(modgenius == 0){
          modgenius = 1;
        }
        break;
     }
 
    passo_atual_na_sequencia++;
  }
 
  passo_atual_na_sequencia = 0;  // Redefine a variável
}
 
void aguardarJogada() {
  boolean jogada_efetuada = false;
  while (!jogada_efetuada) {
      countdown();
      if(rodada_atual == SEQ){
        if(modgenius == 0){
          modgenius = 1;
        }
        break;
      }
      if (greenButton.pressed()) {
        botao_pressionado = 1; 
        digitalWrite(greenLED, HIGH);
        delay(500);
        digitalWrite(greenLED, LOW);
        countdown();
 
        jogada_efetuada = true;
      } else if (redButton.pressed()) {
        botao_pressionado = 0; 
        digitalWrite(redLED, HIGH);
        delay(500);
        digitalWrite(redLED, LOW);
        countdown();
 
        jogada_efetuada = true;
      } else if (blueButton.pressed()) {
        botao_pressionado = 2; 
        digitalWrite(blueLED, HIGH);
        delay(500);
        digitalWrite(blueLED, LOW);
        countdown();
 
        jogada_efetuada = true;
      }
  }
}
 
void verificarJogada() {
  countdown();
  if(rodada_atual != SEQ){
    if (sequencia[passo_atual_na_sequencia] != botao_pressionado) { 
      perdeu_o_jogo = true;
    }  
  } else if(modgenius == 0){
      if (sequencia[passo_atual_na_sequencia] != botao_pressionado) { 
        perdeu_o_jogo = true;
      } else{
        modgenius = 1;
      }
  }
}

void correct() {
  lcd.setCursor(11,0);
  lcd.print("(");
  lcd.print(modsenha + modcofre + modgenius + modcurrent);
  lcd.print("/");
  lcd.print("4");
  lcd.print(")");
  lcd.setCursor(0,1);
}

void countdown() {
  correct();
  unsigned long currentMillis = millis();
  
  if (currentMillis - PreviousMillis >= 1000)
  {
    PreviousMillis += 1000;
    if (SecondsRemaining > 0)
    {
      lcd.clear();
      SecondsRemaining--;
      TimeElapsed++;
      if (MinutesRemaining < 1){
        tone(buzzer, 3000, 100);
        digitalWrite(ledredexp,HIGH);
        delay(100);
        digitalWrite(ledredexp,LOW);
      }else {
        tone(buzzer, 523, 100);
      }
    }else if (SecondsRemaining == 0)
    {
      MinutesRemaining -= 1;     
      SecondsRemaining += 59;
      
    }
    lcd.clear();
    lcd.print(MinutesRemaining);
    lcd.print(":");
    if(SecondsRemaining < 10)
    {
      lcd.print("0");
     }
    lcd.print(SecondsRemaining);
    if(senha != ""){
      lcd.setCursor(1,1);
      lcd.print(senha);
      lcd.setCursor(0,1);
    }
    if (MinutesRemaining + SecondsRemaining == 0)
    {
        bandeira.write(85);
        bandeira2.write(85);
        
        if(s<10){
        lcd.clear();
        lcd.print("KBUUUUUUM!!");
        tone(buzzer, nota_C4,1000);}
       
        do{
        acendeleds();
        delay(250);
        apagaleds();
        delay(250);
        s++;
        }while(s<25);
        while(s>=25){acendeleds();}
    
  }
}}


void corrente(){
  Y = digitalRead(fioY);
  R = digitalRead(fioR);
  G = digitalRead(fioG);
  B = digitalRead(fioB);
  // Print for debug:
  Serial.println("D_current:");
  D_current[0] = G;
  D_current[1] = R;
  D_current[2] = B;
  D_current[3] = Y;
  Serial.println(D_current[0]);
  Serial.println(D_current[1]);
  Serial.println(D_current[2]);
  Serial.println(D_current[3]);
  Serial.println(" ");
}

void acendeleds(){
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        digitalWrite(rled, HIGH);
        digitalWrite(led1c, HIGH);
        digitalWrite(led2c, HIGH);
        digitalWrite(led3c, HIGH);
        digitalWrite(greenLED, HIGH);
        digitalWrite(blueLED, HIGH);
        digitalWrite(redLED, HIGH);
        digitalWrite(ledredexp,HIGH);
        }

void apagaleds(){
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        digitalWrite(rled, LOW);
        digitalWrite(led1c, LOW);
        digitalWrite(led2c, LOW);
        digitalWrite(led3c, LOW);
        digitalWrite(greenLED, LOW);
        digitalWrite(blueLED, LOW);
        digitalWrite(redLED, LOW);
        digitalWrite(ledredexp,LOW);
        }
