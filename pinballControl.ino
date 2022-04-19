#include <Wire.h> // Library for I2C communication 
#include <LiquidCrystal_I2C.h> // Library for LCD 
// Wiring: SDA pin is connected to A4 and SCL pin to A5. 
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered) 
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);  
// Change to (0x27,20,4) for 20x4 LCD. 

const int ledPin = 4;
const int bumperPin = 5;
const int voltIn = 10;
int points = 0;
boolean newGame = true;
boolean score = false;
int buttonState;  
int lastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 10;    // the debounce time; increase if the output flickers

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(bumperPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(voltIn, INPUT);
  lcd.init(); 
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(voltIn));
  if (newGame) {
    lcd.setCursor(0, 0); // Set the cursor on the third column and first row. 
    lcd.print("Welcome To"); // Print the string "Hello World!"
    lcd.setCursor(0, 1); // Set the cursor on the third column and first row. 
    lcd.print("Pinball!!!"); // Print the string "Hello World!"

    digitalWrite(ledPin, HIGH);

    newGame = false;
    delay(3000);

    lcd.clear();
    lcd.setCursor(0, 0); // Set the cursor on the third column and first row. 
    lcd.print("Player1"); // Print the string "Hello World!"
    lcd.setCursor(0, 1); // Set the cursor on the third column and first row. 
    lcd.print("Score: "); // Print the string "Hello World!"
    lcd.setCursor(8, 1);
    lcd.print(points);

    digitalWrite(ledPin, LOW);
  }

  int reading = digitalRead(voltIn);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        points = points + 10;
        lcd.clear();
        lcd.setCursor(0, 0); // Set the cursor on the third column and first row. 
        lcd.print("Player1"); // Print the string "Hello World!"
        lcd.setCursor(0, 1); // Set the cursor on the third column and first row. 
        lcd.print("Score: "); // Print the string "Hello World!"
        lcd.setCursor(8, 1);
        lcd.print(points);
      }
    }
  }

  if (digitalRead(voltIn) == 1) {
    digitalWrite(bumperPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(20);
  } else if (digitalRead(voltIn) == 0){
    digitalWrite(bumperPin, LOW);
    digitalWrite(ledPin, LOW);
    score = true;
  }

  lastButtonState = reading;
}
