#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SHT31.h>
#include <RTClib.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

#define SCREEN_WIDHT 128
#define SCREEN_HEIGHT 64
#define OLED_I2C_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDHT,SCREEN_HEIGHT,&Wire,-1);

RTC_DS3231 rtc;

unsigned long lastControl = 0;
unsigned long intervalControl = 1000; 

void displej(int x, int y, String text){
  display.setCursor(x,y);
  display.print(text);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC,OLED_I2C_ADDRESS)){
    Serial.println("Displej sa nepodarilo inicializovat");
    while(1);
  }
  Serial.println("Displej inicializovany");
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
  displej(1,5,"Displej");
  displej(1,15,"inicializovany");


  if(!sht31.begin(0x44)){
    Serial.println("SHT31 sa nepodarilo inicializovat");
    displej(1,30,"SHT31 - chyba");
    display.display();
    while(1);
  }
  Serial.println("SHT31 inicializovany");
  displej(1,30,"SHT31");
  displej(1,40,"inicializovany");
  if(!rtc.begin()){
    Serial.println("RTC modul sa nepodarilo inicializovat");
    displej(1,55,"RTC - chyba");
    display.display();
    while(1);
  }
  Serial.println("RTC inicializovany");
  displej(1,55,"RTC- inicializovany");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  if(rtc.lostPower()){
    Serial.println("RTC stratilo napanie, nastavujem aktualny cas...");
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() - lastControl >= intervalControl){
    lastControl = millis();

    float t = sht31.readTemperature();
    float h = sht31.readHumidity();

    display.fillRect(0, 0, 128,40, SSD1306_BLACK);
    if(!isnan(t)&&!isnan(h)){
      displej(5,5,"Teplota: "+ String(t) + " C");
      displej(5,20,"Vlhkost: "+ String(h) + " %");
    } else {
      displej(1,5,"Chyba SHT");
    }
    display.display();
  }
  
  static unsigned long lastSerial = 0;
  if(millis() - lastSerial >= 500){
    lastSerial = millis();
    display.fillRect(0, 50, 128,10, SSD1306_BLACK);
    DateTime now = rtc.now();
    //zobrazenie času na sériovom monitore
    Serial.print(now.hour() < 10 ? "0" : "");
    Serial.print(now.hour());
    Serial.print(":");
    Serial.print(now.minute() < 10 ? "0" : "");
    Serial.print(now.minute());
    Serial.print(":");
    Serial.print(now.second() < 10 ? "0" : "");
    Serial.println(now.second());

    //zobrazenie dátumu na displej
    display.setCursor(5,40);
    display.print(now.day());
    display.print(".");
    display.print(now.month());
    display.print(".");
    display.print(now.year());

    //zobrazenie času na displej
    display.setCursor(5,50);
    display.print(now.hour() < 10 ? "0" : "");
    display.print(now.hour());
    display.print(":");
    display.print(now.minute() < 10 ? "0" : "");
    display.print(now.minute());
    display.print(":");
    display.print(now.second() < 10 ? "0" : "");
    display.print(now.second());
    display.display();
  }
}
