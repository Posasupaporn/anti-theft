#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>


Servo servo_3;

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27,16,2);
 
#define SS_PIN 10
#define RST_PIN 9




MFRC522 mfrc522(SS_PIN, RST_PIN);   


const int trigPin = 4;
const int echoPin = 3;
const int buzzer = 2;
const int Led_B = 5;
const int Led_G = 6;
const int Led_R = 7;

long duration;
int distance;
int safetyDistance;
 
void setup()
{
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(buzzer, OUTPUT);
  pinMode(Led_B, OUTPUT);
  pinMode(Led_G, OUTPUT);
  pinMode(Led_R, OUTPUT);
  servo_3.attach(8);

  
  Serial.begin(9600);  

  lcd.begin(16,2);
  lcd.backlight();

  SPI.begin();      
  mfrc522.PCD_Init();  
  
  
 
  noTone(buzzer);
  Serial.println("Put your card to the reader...");
  Serial.println();

}
void loop() 
{
  digitalWrite(Led_G, LOW);
  digitalWrite(Led_B, HIGH);
  
  
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;

  safetyDistance = distance;
  
   
  if (safetyDistance <= 5){
      servo_3.write(0);    
      lcd.clear();
      digitalWrite(Led_B, LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter your Card");
      lcd.setCursor(0,1);
      lcd.print("in 5 Seconds");
      digitalWrite(Led_R, HIGH);
      digitalWrite(Led_R, LOW);
      delay(5000);

      
      digitalWrite(Led_R, HIGH);
      delay(700);
      lcd.clear();
      digitalWrite(Led_R, LOW);
      delay(300);
      digitalWrite(Led_B, LOW);
      tone(buzzer, 700);
      digitalWrite(Led_R, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please get out");
      
    }

  
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
 
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "55 52 08 AD") 
  {
      digitalWrite(Led_B, LOW);
      lcd.clear();
      lcd.print("Welcome");
      noTone(buzzer);
      digitalWrite(Led_R, LOW);
      digitalWrite(Led_G, HIGH);
      delay(2000);
      digitalWrite(Led_G, LOW);
    
  
    Serial.println("Authorized access");
    Serial.println();
    
    
  }
 
 else   
 {
    servo_3.write(90); 
    Serial.println(" Access denied");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Your card is invalid");
    
  
    
  }
} 