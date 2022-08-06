#define BLYNK_TEMPLATE_ID "TMPLwQh1q3sX"
#define BLYNK_DEVICE_NAME "Penulisan Ilmiah"
#define BLYNK_AUTH_TOKEN "cRZylFr4dhIaA-3eHNw7hwMkBmgWV-mu" 
#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Asoi Tabang";
char pass[] = "lahgalaksurang";

#include <BlynkSimpleEsp32.h>
#include <EEPROM.h>
#include "GravityTDS.h"
#include <DallasTemperature.h>
#include <OneWire.h>
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS 27
#define TdsSensorPin 35
#define switchbawahpin 18
#define switchataspin 19
#define relaypin 23
#define ledhijau 17 
#define ledmerah 16
#define ledbiru 25

BlynkTimer timer;
GravityTDS gravityTds;  
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte DERAJAT = B11011111;

float tdsValue = 0;

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, pass);
    Blynk.config(auth);
    
    lcd.init();
    lcd.backlight();
    lcd.clear();
    
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(3.3);  
    gravityTds.setAdcRange(4095);  
    gravityTds.begin();  
    sensors.begin();

    pinMode(switchbawahpin, OUTPUT);
    pinMode(switchataspin, OUTPUT);
    pinMode(relaypin, OUTPUT);

    pinMode(ledhijau, OUTPUT);
    pinMode(ledbiru, OUTPUT);
    pinMode(ledmerah, OUTPUT);
    
    digitalWrite(switchbawahpin, HIGH);
    digitalWrite(switchataspin, HIGH);
    digitalWrite(relaypin, HIGH);

    digitalWrite(ledhijau, LOW);
    digitalWrite(ledbiru, LOW);
    digitalWrite(ledmerah, LOW);
}

void loop()
{
  Blynk.run();
  
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    Serial.print("Temperature is: "); 
    Serial.println(temp);
    gravityTds.setTemperature(temp); 
    gravityTds.update();  
    tdsValue = gravityTds.getTdsValue();  
    Serial.print(tdsValue,0);
    Serial.println("ppm");

    lcd.setCursor(0, 0);
    lcd.print("TDS:");
    lcd.print(tdsValue,0);
    lcd.setCursor(0,1);
    lcd.print("Temp:");
    lcd.print(temp);
    lcd.write(DERAJAT);
    lcd.print("C");

    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, tdsValue);

   int switchAtas = digitalRead(switchataspin);
   int switchBawah = digitalRead(switchbawahpin);
   Serial.print("Switch Atas:");
   Serial.println(switchAtas);
   Serial.print("Switch Bawah:");
   Serial.println(switchBawah);

   if(temp < 28){
    Blynk.virtualWrite(V5, LOW);
    Blynk.virtualWrite(V6, HIGH);
   }
   if(temp >28){
    Blynk.virtualWrite(V5, HIGH);
    Blynk.virtualWrite(V6, LOW);
    Blynk.logEvent("suhu_hangat", "Suhu Kolam Koi Anda diatas 28 Derajat Celcius!!");
   }

   if(switchBawah == 0 && switchAtas == 0){
    digitalWrite(relaypin, LOW);
    Blynk.virtualWrite(V7, LOW);
    Blynk.virtualWrite(V8, HIGH);
    Blynk.logEvent("mengisi_air", "Kolam Anda Sedang Mengisi Air!!");
   }
   if(switchBawah == 1 && switchAtas == 1){
    digitalWrite(relaypin, HIGH);
    Blynk.virtualWrite(V7, HIGH);
    Blynk.virtualWrite(V8, LOW);
   }   
    

    if(tdsValue < 300){
       digitalWrite(ledhijau, HIGH);
       digitalWrite(ledbiru, LOW);
       digitalWrite(ledmerah, LOW);;
       Blynk.virtualWrite(V2, HIGH);
       Blynk.virtualWrite(V3, LOW);
       Blynk.virtualWrite(V4, LOW);
    }
    if(tdsValue > 300 && tdsValue < 600){
       digitalWrite(ledhijau, LOW);
       digitalWrite(ledbiru, HIGH);
       digitalWrite(ledmerah, LOW);
       Blynk.virtualWrite(V2, LOW);
       Blynk.virtualWrite(V3, HIGH);
       Blynk.virtualWrite(V4, LOW);
    }
    if(tdsValue > 600){
       digitalWrite(ledhijau, LOW);
       digitalWrite(ledbiru, LOW);
       digitalWrite(ledmerah, HIGH);
       Blynk.virtualWrite(V2, LOW);
       Blynk.virtualWrite(V3, LOW);
       Blynk.virtualWrite(V4, HIGH);
       Blynk.logEvent("air_kotor", "Kualitas Air Kolam Koi Anda Buruk!!");
    }
    

    
    delay(1000);
}
