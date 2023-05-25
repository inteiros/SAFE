#include <Wire.h> 
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define led1 8
#define led2 9
#define led3 10
#define ledf 11
#define buzzer A1

int cofre = 0;
int senha = 0;
int genius = 0;
int err = 0;
int c = 0;
int d = 0;
int g = 0;

unsigned SecondsRemaining = 0;
unsigned long PreviousMillis = 0;
unsigned MinutesRemaining;
int t = SecondsRemaining;
const int nota_C4 = 262;
const int nota_DS7 = 2489;
const int nota_E7 = 2637;


void setup() {
  Serial.begin(9600);
  Wire.begin(); 

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
  
  MinutesRemaining = 02;
  SecondsRemaining = 04;
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - PreviousMillis >= 1000)
  {
    PreviousMillis += 1000;
    if (SecondsRemaining > 0)
    {
      lcd.clear();
      SecondsRemaining--;
      if (MinutesRemaining == 00 && SecondsRemaining < 10){
        tone(buzzer, 3000, 100);
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
    if (MinutesRemaining + SecondsRemaining == 0)
    {
      lcd.clear();
      lcd.print("BOOM");
      tone(buzzer, 523, 3000);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      delay(1000);
      lcd.clear();
      while(true){
        lcd.print("GAME OVER");
        delay(1000);
        lcd.clear();
      }
    }
  }
  //------------
     //delay(100);
     if(cofre == 0){
        Wire.requestFrom(8, 1); 
        if(Wire.available()){
          c = Wire.read();
        }
     }
     if(senha == 0){
        Wire.requestFrom(7, 1);
        if(Wire.available()){
          d = Wire.read();
        }
     }
     if(genius == 0){
        Wire.requestFrom(6, 1);
        if(Wire.available()){
          g = Wire.read();
        }
     }

     if(c == 100){
        cofre++;     
     } else if(c == 1){
        err = err + 1;
        beeperr(err);
     }
     if(d == 100){
        senha++;     
     } else if(d == 1){
        err = err + 1;
        beeperr(err);
     }
     if(g == 100){
        genius++;     
     } else if(g == 1){
        err = err + 1;
        beeperr(err);
     }

     if(err == 1){
        digitalWrite(led1, HIGH);
     }
     if(err == 2){

        digitalWrite(led2, HIGH);
     }
     if(err == 3){
        digitalWrite(led3, HIGH);
     }


     if(cofre != 0 && senha != 0 && genius != 0){
        digitalWrite(ledf, HIGH);
     }
     
  //delay(500);
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
