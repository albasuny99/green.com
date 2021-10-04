#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS1307.h>
#include "dht.h"

DS1307 rtc;
dht DHT;  

#define buzzer       2 
#define push_button  3 
#define led_blue     4 
#define led_red      5
#define fan          6 
#define pump         7 
const byte rs=8, en=9, d4=10, d5=11, d6=12, d7=13;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define WATER  A0
#define HEAT   A1
#define SOIL   A2
#define LDR    A3

boolean button ;
int count = 0;
int water;
int heat;
int soil;
int ldr;
char data;
void setup() 
{
  
  lcd.begin(16,2);
  //Serial.begin(9600);
  rtc.start();
  rtc.set(6,15,18, 3, 10, 2021); //08:00:00 24.12.2014 //sec, min, hour, day, month, year
  pinMode(buzzer, OUTPUT);
  pinMode(led_blue, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(pump, OUTPUT);

  pinMode(push_button, INPUT);
  pinMode(WATER, INPUT);
  pinMode(HEAT, INPUT);
  pinMode(SOIL, INPUT);
  pinMode(LDR, INPUT);


  digitalWrite(led_red,LOW);
  digitalWrite(led_blue,LOW);
  digitalWrite(fan,LOW);
  digitalWrite(pump,LOW);
  
  Serial.println("Smart Plant");
  lcd.setCursor(3,0);
  lcd.print("Smart Plant");
  
  digitalWrite(buzzer,HIGH);
  digitalWrite(led_red,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  delay(100);
  digitalWrite(buzzer,HIGH);
  digitalWrite(led_blue,HIGH);
  digitalWrite(led_red,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  digitalWrite(led_blue,LOW);
  digitalWrite(led_red,LOW);
  delay(2000);
}

void loop() 
{
  uint8_t sec1, min1, hour1, day1, month1;
  uint16_t year1;
  rtc.get(&sec1, &min1, &hour1, &day1, &month1, &year1);
  
 // data = Serial.read();

  DHT.read11(HEAT);

  button = digitalRead(push_button);

  water = analogRead(WATER);
  water = map(water,0,1023,0,100);

  soil = analogRead(SOIL);
  soil = map(soil,0,1023,100,0);

  ldr = analogRead(LDR);
  ldr = map(ldr,0,1023,0,100);
  
  lcd.setCursor(0,0);
  lcd.print("Time: ");

  lcd.setCursor(6,0);
  lcd.print(hour1);
  lcd.setCursor(8,0);
  lcd.print(":");
  lcd.setCursor(9,0);
  lcd.print(min1);
  lcd.setCursor(11,0);
  lcd.print(":");
  lcd.setCursor(12,0);
  lcd.print(sec1);
  
  if(button==HIGH)
  {
    delay(10);
    
    page1();
    delay(2000);
    page2();
    delay(2000);
    page3();
    delay(2000);
    page4();
    delay(2000);
    page5();
    delay(2000);
  }
  else
  {
    page0();
  }

  if( ldr <60) 
  {
    digitalWrite(led_blue,HIGH);
    digitalWrite(led_red,HIGH);

    lcd.setCursor(0,1);
    lcd.print("   Leds is ON   ");
    delay(1000);
  }
  
  else if( ldr >60)
  {
    digitalWrite(led_blue,LOW);
    digitalWrite(led_red,LOW);

    
  }

 
  

  if( water <= 50 )
  {
   lcd.setCursor(0,1);
   lcd.print("I Need to Water!");
   
  digitalWrite(buzzer, HIGH);
  delay (30);
  digitalWrite(buzzer, LOW);
  delay(1000);

  
  }

  else
  {
    digitalWrite(buzzer, LOW);

    
  }



  if( soil <= 30 )
  {
    lcd.setCursor(0,1);
    lcd.print("   PUMP is ON   ");
    
    digitalWrite(pump,LOW);
    delay(1000);
    digitalWrite(pump,HIGH);

   // Serial.println("Pump is ON");
    
}
else
{
  digitalWrite(pump,LOW);

  //Serial.println("Pump is OFF");
  
}


if( DHT.temperature > 30 )
  {
    lcd.setCursor(0,1);
    lcd.print("   Fan is ON    ");
    
    digitalWrite(fan,HIGH);
    delay(1000);  
}
else
{
  digitalWrite(fan,LOW);
}


}

void page0()
{
  uint8_t sec1, min1, hour1, day1, month1;
  uint16_t year1;
  rtc.get(&sec1, &min1, &hour1, &day1, &month1, &year1);
  
  lcd.setCursor(0,0);
  lcd.print("Time: ");

  lcd.setCursor(6,0);
  lcd.print(hour1);
  lcd.setCursor(8,0);
  lcd.print(":");
  lcd.setCursor(9,0);
  lcd.print(min1);
  lcd.setCursor(11,0);
  lcd.print(":");
  lcd.setCursor(12,0);
  lcd.print(sec1);



}

void page1()
{
  lcd.setCursor(0,0);
  lcd.print("Soil Sensor");

  lcd.setCursor(11,0);
  lcd.print(": ");
  lcd.setCursor(13,0);
  lcd.print(soil);
  lcd.setCursor(15,0);
  lcd.print("%");
}

void page2()
{
  lcd.setCursor(0,0);
  lcd.print("Water Level");

  lcd.setCursor(11,0);
  lcd.print(": ");
  lcd.setCursor(13,0);
  lcd.print(water);
  lcd.setCursor(15,0);
  lcd.print("%");
}

void page3()
{
  lcd.setCursor(0,0);
  lcd.print("Temperature");

  lcd.setCursor(11,0);
  lcd.print(": ");
  lcd.setCursor(13,0);
  lcd.print(DHT.temperature);
  lcd.setCursor(15,0);
  lcd.print("c");
}

void page4()
{
  lcd.setCursor(0,0);
  lcd.print("humidity : ");

  lcd.setCursor(11,0);
  lcd.print("  ");
  lcd.setCursor(13,0);
  lcd.print(DHT.humidity);
  lcd.setCursor(15,0);
  lcd.print("%");
}

void page5()
{
  lcd.setCursor(0,0);
  lcd.print("LDR Sensor ");

  lcd.setCursor(11,0);
  lcd.print(": ");
  lcd.setCursor(13,0);
  lcd.print(ldr);
  lcd.setCursor(15,0);
  lcd.print("%");
}
