#include <SoftwareSerial.h>
#include <A6HTTP.h>
#include <TroykaMQ.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
A6HTTP myA6("MYAPN", "www.posttestserver.com", "/post.php", 80, "application/x-www-form-urlencoded");


#define PIN_MQ2  A0
MQ2 mq2(PIN_MQ2);
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
int gaz = 0; bool trevoga = false, call = false; bool net = false;
void setup()
{
  pinMode(6, OUTPUT);//lamp2
  pinMode(5, OUTPUT);//lamp1
  pinMode(12, OUTPUT); //rele
  pinMode(11, OUTPUT); //buzzer
  lcd.begin();
  lcd.backlight();
  mq2.calibrate(2);
  lcd.print("Yuklanish...");
  myA6.begin();
  lcd.clear();
  lcd.print("SGA tizimi");
  delay(1000);
}
void loop()
{

  if (!trevoga)
  {
    lcd.clear();
    gaz = mq2.readMethane();
    lcd.print("Tabiiy gaz: " + (String)gaz);
    delay(500);
  }

  if (gaz > 3)
  {
    trevoga = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gaz xavfi!");
    if (!call) {
      call = true;
      myA6.A6command("ATD+998993657710", "OK", "yy", 3000, 2);
      while (true)
      {
        if (trevoga)
        {
          digitalWrite(12, 1);
          digitalWrite(6, 1);
          digitalWrite(5, 0);
          digitalWrite(11, 0);
          delay(100);
          digitalWrite(11, 1);
          digitalWrite(6, 0);
          digitalWrite(5, 1);
          delay(100);
          digitalWrite(11, 0);

        }
      }
    }
  }
}
