#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//PIN

#define ONE_WIRE_BUS 4
#define PinLED  8
#define PinCLK 6
#define PinDT 2
#define PinSW 3

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

long TimeOfLastDebounce = 0;
int DelayofDebounce = 0.1;
long TimeOfLastTempMeasuring = 0;
int DelayofTempMeasuring = 2500;

int currentDT;
int currentCLK;
int PreviousCLK;
int PreviousDATA;

boolean LEDstate;

int displaycounter=25;

float temp_reading;

byte temperature_graph[128];
boolean record_temp = false;

boolean display_mode = HIGH;
boolean do_display_update;

void setup() {
  Serial.begin(9600);
  while(!Serial);

  attachInterrupt(digitalPinToInterrupt(PinSW), innterupt_action, RISING);

  PreviousCLK=digitalRead(PinCLK);
  PreviousDATA=digitalRead(PinDT);

  pinMode(PinLED, OUTPUT);
  pinMode(PinSW, INPUT);
  temperature_graph[0] = 15;
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  update_display();

}

void loop() {
  if ((millis() - TimeOfLastDebounce) > DelayofDebounce && display_mode == HIGH) {

      currentCLK = digitalRead(PinCLK);
      currentDT = digitalRead(PinDT);

      check_rotary();

      PreviousCLK=digitalRead(PinCLK);
      PreviousDATA=digitalRead(PinDT);

      TimeOfLastDebounce=millis();
   }

  if ((millis() - TimeOfLastTempMeasuring) > DelayofTempMeasuring) {
    sensors.requestTemperatures();
    temp_reading = sensors.getTempCByIndex(0);

    if (record_temp){
      record_temp = false;
      for (int n = 127; n >= 0; n--) {
        temperature_graph[n+1] = temperature_graph[n];
      }
      int wert = temp_reading*2;
      temperature_graph[0] = wert;
    }
    else {
      record_temp = true;
    }

    if (temp_reading < (displaycounter)) {
      LEDstate = HIGH;
    }
    else {
      LEDstate = LOW;
    }

    update_display();


    digitalWrite(PinLED, LEDstate);
    Serial.println(display_mode);
    TimeOfLastTempMeasuring=millis();
  }
  if (do_display_update == HIGH){
    do_display_update = LOW;
    update_display();
  }

}//END main loop

void check_rotary() {

 if (PreviousCLK == 0 && PreviousDATA == 1) {
    if (currentCLK == 1 && currentDT == 0) {
      displaycounter++;
      Serial.println(displaycounter);
      text_display();
    }
    if (currentCLK == 1 && currentDT == 1) {
      displaycounter--;
      Serial.println(displaycounter);
      text_display();
    }
  }

if (PreviousCLK == 1 && PreviousDATA == 0) {
    if (currentCLK == 0 && currentDT == 1) {
      displaycounter++;
      Serial.println(displaycounter);
      text_display();
    }
    if (currentCLK == 0 && currentDT == 0) {
      displaycounter--;
      Serial.println(displaycounter);
      text_display();
    }
  }

if (PreviousCLK == 1 && PreviousDATA == 1) {
    if (currentCLK == 0 && currentDT == 1) {
      displaycounter++;
      Serial.println(displaycounter);
      text_display();
    }
    if (currentCLK == 0 && currentDT == 0) {
      displaycounter--;
      Serial.println(displaycounter);
      text_display();
    }
  }

if (PreviousCLK == 0 && PreviousDATA == 0) {
    if (currentCLK == 1 && currentDT == 0) {
      displaycounter++;
      Serial.println(displaycounter);
      text_display();
    }
    if (currentCLK == 1 && currentDT == 1) {
      displaycounter--;
      Serial.println(displaycounter);
      text_display();
    }
  }
 }

void innterupt_action(){
  display_mode = !display_mode;
  do_display_update = HIGH;
}

void update_display(){
  if (display_mode == HIGH){
    text_display();
  }
  else {
    display_graph();
  }
}

 void text_display() {
   display.clearDisplay();
   display.setTextColor(WHITE);
   display.setTextSize(1);
   display.setCursor(0,0);
   display.print("Temperature: ");
   display.setTextSize(2);
   display.setCursor(0,10);
   display.print(temp_reading);
   display.print(" ");
   display.setTextSize(1);
   display.cp437(true);
   display.write(167);
   display.setTextSize(2);
   display.print("C");
   display.setTextSize(1);
   display.setCursor(0, 35);
   display.print("Set: ");
   display.setTextSize(2);
   display.setCursor(0, 45);
   display.print(displaycounter);
   display.print(" ");
   display.setTextSize(1);
   display.cp437(true);
   display.write(167);
   display.setTextSize(2);
   display.print("C");
   if (LEDstate == HIGH){
     display.print(" ");
     display.setTextColor(BLACK,WHITE);
     display.print("ON");
     display.setTextColor(WHITE);
     display.print("OFF");
   }
   else {
     display.print(" ");
     display.setTextColor(WHITE);
     display.print("ON");
     display.setTextColor(BLACK,WHITE);
     display.print("OFF");
   }
   display.display();
 }

 void display_graph() {
   display.clearDisplay();
   display.setTextColor(WHITE);
   display.setTextSize(1);
   display.setCursor(0, 0);
   display.print(int(temp_reading)-4);
   display.setCursor(0, 28);
   display.print(int(temp_reading));
   display.setCursor(0, 56);
   display.print(int(temp_reading)+4);
   display.drawLine(13,0,13,63,WHITE);
   display.drawPixel(12,0,WHITE);
   for (int n = 14; n<128;n=n+2){
     display.drawPixel(n, displaycounter*8-(int(temp_reading)-4)*8, WHITE);
   }
   display.drawPixel(12,31,WHITE);
   display.drawPixel(12,63,WHITE);
   for (int n = 14; n < 128; n++){
     display.drawLine(n,temperature_graph[(n-14)]*4-(int(temp_reading)-4)*8,n,temperature_graph[(n-13)]*4-(int(temp_reading)-4)*8, WHITE);
   }
   display.display();
 }
