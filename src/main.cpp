#include <Arduino.h>
#include <Bounce2.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
 
const int pinoLed = 18;
const int buttonPinUp = 19;
const int buttonPinDown = 4;

int brightnessIntensity = 0; // Nível de 0 a 100

Bounce debouncer_1 = Bounce();
Bounce debouncer_2 = Bounce();

bool change = false;

void setup() {
  Serial.begin(9600);
  pinMode(pinoLed, OUTPUT);
  
  digitalWrite(pinoLed, LOW); 

  debouncer_2.attach(buttonPinDown, INPUT_PULLUP);
  debouncer_2.interval(25);
  debouncer_1.attach(buttonPinUp, INPUT_PULLUP);
  debouncer_1.interval(25);

  Serial.println("Pronto para controlar o brilho. Use os botões.");

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Intensidade: 0");
}

void loop() {
  debouncer_1.update();
  debouncer_2.update();

  if (debouncer_1.fell()) {
    if (brightnessIntensity < 100 and brightnessIntensity >= 0) {
      brightnessIntensity += 10;
      change = true;
    }
  }

  if (debouncer_2.fell()) {
    if (brightnessIntensity > 0 and brightnessIntensity <= 100) {
      brightnessIntensity -= 10;
      change = true;
    }
  }

  int brightnessValue = map(brightnessIntensity, 0, 100, 0, 255);
  analogWrite(pinoLed, brightnessValue);

  Serial.print("Intensidade: ");
  Serial.print(brightnessIntensity);
  Serial.print("%  -> Valor PWM: ");
  Serial.println(brightnessValue);

  if (change == true) {
    lcd.setCursor(13, 0);
    lcd.print("    ");
    lcd.setCursor(13, 0);
    lcd.print(brightnessIntensity);

    change = false;
  }

  change = false;
  delay(20); 
}
