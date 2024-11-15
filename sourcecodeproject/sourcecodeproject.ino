#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // ไลบรารีสำหรับจอ LCD แบบ I2C

#define DOUT  2
#define CLK  3
int U = 2;
int W = 0;
int S = 0;
int G = 1;
int buzzer = 10;
float NUM = 0;
float calibration_factor = 8800;  // ปรับค่าเริ่มต้นตรงนี้
HX711 scale;

// กำหนดที่อยู่ของจอ LCD (ปกติคือ 0x27 หรือ 0x3F) และขนาดจอ 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    scale.begin(DOUT, CLK);
    Serial.begin(9600);

    lcd.init();       // เริ่มต้นการทำงานของจอ LCD
    lcd.backlight();  // เปิดไฟแบ็คไลท์ของจอ LCD

    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
    Serial.println("กดปุ่ม + หรือ - เพื่อปรับค่า Calibration Factor");
    Serial.println("กำลังปรับค่าเริ่มต้น กรุณารอสักครู่...");

    scale.set_scale();
    scale.tare();                             // รีเซ็ตน้ำหนักเป็น 0
    long zero_factor = scale.read_average();  // อ่านค่าน้ำหนักเริ่มต้น

    Serial.print("Zero factor: ");
    Serial.println(zero_factor);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Zero factor:");
    lcd.setCursor(0, 1);
    lcd.print(zero_factor);
    delay(2000);  // แสดงผลเป็นเวลา 2 วินาที

    lcd.clear();
   
}

void loop() {
    lcd.setCursor(0, 0);
    lcd.print("Next List....");
    scale.set_scale(calibration_factor);  // ปรับค่า calibration factor
    float weight = scale.get_units(5);    // อ่านค่าน้ำหนักเฉลี่ยจาก 5 การอ่าน
    if ((G == 1) && (S == 0) && (U == 1)) {
        scale.set_scale(calibration_factor);  // ปรับค่า calibration factor
        float weight = scale.get_units(5);    // อ่านค่าน้ำหนักเฉลี่ยจาก 5 การอ่าน
        Serial.print(G);
        Serial.print(" | ");
        Serial.print(S);
        Serial.print(" | ");
        Serial.println(U);
        lcd.setCursor(0, 0);
        lcd.print("weight");
        lcd.setCursor(8, 0);
        lcd.print(weight);
        delay(500);
        lcd.clear();
        while ((weight > 2) && (U == 1)) {
            scale.set_scale(calibration_factor);  // ปรับค่า calibration factor
            float weight = scale.get_units(5);
            lcd.setCursor(0, 0);
            lcd.print("weight");
            lcd.setCursor(8, 0);
            lcd.print(weight);
            lcd.setCursor(14, 0);
            lcd.print("kg");
            lcd.setCursor(0, 1);
            lcd.print(S);
            delay(1000);
            S += 1;
            G -= 1;
            G += 1;
            Serial.print(G);
            Serial.print(" | ");
            Serial.print(S);
            Serial.print(" | ");
            Serial.println(U);
            if (S > 6) {
                S -= 6;
             
      }
            if ((weight > 0) && (S == 5)) {
                NUM += weight;
                digitalWrite(buzzer, HIGH);
                delay(100);
                digitalWrite(buzzer, LOW);
                lcd.setCursor(0, 0);
                lcd.print("weight");
                lcd.setCursor(8, 0);
                lcd.print(weight);
                lcd.setCursor(0, 1);
                lcd.print("Done");
                delay(5000);
                lcd.clear();
                S -= 5;
                U += 1;
                Serial.print(G);
                Serial.print(" | ");
                Serial.print(S);
                Serial.print(" | ");
                Serial.println(U);
                
     
      }
         
    }
     
  }

    else if (weight > 5) {
        U -= 1;
        Serial.print(U);
        Serial.print(" | ");
        Serial.print(S);
        Serial.print(" | ");
        Serial.println(G);
     
  }
}
