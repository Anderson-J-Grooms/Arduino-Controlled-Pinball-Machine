#include <Wire.h> // Library for I2C communication 
#include <LiquidCrystal_I2C.h> // Library for LCD 
// Wiring: SDA pin is connected to A4 and SCL pin to A5. 
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered) 
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);  
// Change to (0x27,20,4) for 20x4 LCD. 

// Values for scoring
const int ledPin = 4;
int points = 0;
int pointCheck = 0;
boolean newGame = true;

// Values for new game
const int startPin = 13;
const int startLight = 6;
boolean lightDim = false;
double startCount = 0;
int lightFlash = 0;

// Values for pop bumpers
int bumperCheck = 0;
const int bumperPin = 5;
const int bumperIn = 10;

// Values for ramp
const int rampPin = A2;
boolean rampCheck = true;

// Values for ball return
const int returnPin = A3;
boolean returnCheck = true;
int ballCount = 1;
double earlyCount = 1200;

void setup() {
  Serial.begin(9600);
  pinMode(startLight, OUTPUT);
  pinMode(bumperPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(bumperIn, INPUT);
  pinMode(rampPin, INPUT);
  pinMode(returnPin, INPUT); 
  pinMode(startPin, INPUT);
  lcd.init(); 
  lcd.backlight();
}

void loop() {

// New game setup
  if (newGame) {
    // New game message 1
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome To");
    lcd.setCursor(0, 1);
    lcd.print("Pinball!!!");

    digitalWrite(ledPin, HIGH);

    newGame = false;
    delay(3000);

    // Flash message until start is pressed
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press Start");
    lcd.setCursor(0, 1);
    lcd.print("To Continue");

    lightFlash = 0;
    startCount = 0;
    
    while (lightFlash == 0) {
      lightFlash = digitalRead(startPin);
      if (lightDim) {
        digitalWrite(startLight, HIGH);
        startCount = startCount + 1;
        
        if (startCount >= 33000) {
          lightDim = false;
        }
      } else {
        digitalWrite(startLight, LOW);
        startCount = startCount - 1;

        if (startCount <= 0) {
          lightDim = true;
        }
      }
    }

    digitalWrite(startLight, LOW);

    delay(1000);

    // Display score data
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ball: ");
    lcd.setCursor(6, 0);
    lcd.print(ballCount);
    lcd.setCursor(0, 1); 
    lcd.print("Score: ");
    lcd.setCursor(8, 1);
    lcd.print(points);

    digitalWrite(ledPin, LOW);
  }

  // Check for reset
  if (digitalRead(startPin) == HIGH) {
    gameReset();
  }

  // Check for ramp score
  if (analogRead(rampPin) >= 300 && rampCheck) {
     points = points + 50;
     bonus();
     scoreUpdate();
     rampCheck = false;
  } else {
    rampCheck = true;
  }

  // Check for ball return
  if (analogRead(returnPin) >= 300 && returnCheck) {
        ballCount = ballCount + 1;
        returnCheck = false;
        if (ballCount >= 4) {
          earlyCount = 1200;
          gameReset();
        } else if (earlyCount > 0) {
          ballCount = ballCount - 1;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Ball Saved");
          lcd.setCursor(0, 1);
          lcd.print("Try Again");

          delay(2000);

          scoreUpdate();
        }else {
          earlyCount = 1200;
          sad();
          scoreUpdate();
        }
        earlyCount = 1200;
  } else {
    returnCheck = true;
  }


  // Check if bumper is hit
  int bumperReading = digitalRead(bumperIn);

  if (bumperReading == 1) {
    digitalWrite(bumperPin, HIGH);
    digitalWrite(ledPin, HIGH);

    if (bumperCheck > 10) {
      points = points + 10;
      scoreUpdate();
      bumperCheck = 0;;
    }
  } else if (bumperReading == 0){
    digitalWrite(bumperPin, LOW);
    digitalWrite(ledPin, LOW);
    bumperCheck = bumperCheck + 1;
  }

  //Serial.println(earlyCount);
  if (earlyCount > 0) {
    earlyCount = earlyCount - 1;
  }
}

// Ball return message
void sad() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ball Lost");

  delay(2000);
}

// Ramp score message
void bonus() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ramp Bonus!!!");

  delay(1000);
}

// Update LCD
void scoreUpdate() {
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("Ball: ");
  lcd.setCursor(6, 0);
  lcd.print(ballCount);
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.setCursor(8, 1);
  lcd.print(points);
}

// Start new game
void gameReset() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over!!!");

  delay(3000);

  points = 0;
  ballCount = 1;
  newGame = true;
}
