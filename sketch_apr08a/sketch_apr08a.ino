#include <Keypad.h>
#include <LiquidCrystal.h>
#define parola_boyutu 5 //keypad

LiquidCrystal lcd(22, 23, 24, 25, 26, 27); //lcd
boolean degisimKontrolu = 0;
float degistiMi= 0.0;

//LM35
const int analogPin = A0;
float sensorDeger = 0;
float sicaklikDeger = 0;

//Flame Sensor
int buzzer = 9;
int flame_sensor = 50;
int flame_degeri;

//Pir Sensor
int pir_sensor = 8;
boolean pir_sensor_degeri = 0;
int pir_tetik = 7;

//Keypad
int signalPin = 11;
int signalPin2 = 12;

char Data[parola_boyutu]; 
char Master[parola_boyutu] = "5454"; 
byte sayac = 0;
char key;

const byte ROWS = 4;
const byte COLS = 3;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {46, 47, 48, 49};
byte colPins[COLS] = {45, 44, 43};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void lcdSetup(){
  lcd.begin(16, 2);
}

void lcdLoop(){

  if(!degisimKontrolu){
    degisimKontrolu=1;
    degistiMi = sicaklikDeger;  
  }

 if(degistiMi != sicaklikDeger){
  lcd.clear();
  lcd.print("Sicaklik: ");
  lcd.print((int)sicaklikDeger);
  lcd.setCursor(0,1);
  if((int)sicaklikDeger<20){
   lcd.print("Sicaklik Dustu");
  }
  else if((int)sicaklikDeger>30){
   lcd.print("Sicaklik Yuksek");
  }
  else{
   lcd.print("Sicaklik Normal");
  }
     degistiMi=sicaklikDeger;
 } 
}

void lm35Setup(){
  
   analogReference(INTERNAL1V1);   
   pinMode(A0,INPUT);  
}

void lm35Loop(){
 sensorDeger = analogRead(analogPin);
 
  // Gerilim değerini sıcaklık değerine dönüştürüyoruz.
 sicaklikDeger = sensorDeger/9.31;
 
}

void clearData(){
  while(sayac !=0){
    Data[sayac--] = 0; 
  }
  return;
}

void keypadSetup(){
   pinMode(signalPin, OUTPUT);
   pinMode(signalPin2, OUTPUT);
}

void keypadLoop(){
  key = customKeypad.getKey();
  if (key){
    Data[sayac] = key; 
   
    sayac++;
    Serial.println(key);
    
    }

  if(sayac == parola_boyutu-1){

    if(!strcmp(Data, Master)){
  
      digitalWrite(signalPin, HIGH); 
      delay(1000);
      digitalWrite(signalPin, LOW);
      }
    else{
       digitalWrite(signalPin2, HIGH); 
      delay(1000);
      digitalWrite(signalPin2, LOW);
      }
    
    clearData();  
  }
}


void pirSetup(){ 
  pinMode(pir_sensor, INPUT);
  pinMode(pir_tetik, OUTPUT);
}
void pirLoop(){
  pir_sensor_degeri = digitalRead(pir_sensor);

  if (pir_sensor_degeri == 1) {
    digitalWrite(pir_tetik, HIGH);

  } else {
    digitalWrite(pir_tetik, LOW);
  }

}

void flameSetup(){
  pinMode(buzzer, OUTPUT);
  pinMode(flame_sensor, INPUT);
}
void flameLoop(){
  flame_degeri = digitalRead(flame_sensor); // ALEV SENSÖRÜNÜ KONTROL ET
  if (flame_degeri == 1) // SENSORDEN SİNYAL VARSA UYARI VER
  {
   digitalWrite(buzzer, HIGH);
  }
  else
  { 
   digitalWrite(buzzer, LOW);
  }
   
}
void setup()
{
  Serial.begin(9600);
  flameSetup();
  pirSetup();
  keypadSetup();
  lm35Setup();
  lcdSetup();

}
void loop()
{
  flameLoop();
  pirLoop();
  keypadLoop();
  lm35Loop();
  lcdLoop();  
}
