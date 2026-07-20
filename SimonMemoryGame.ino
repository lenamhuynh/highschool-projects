#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

int ledPin1 = 8;
int ledPin2 = 9;
int ledPin3 = 10;
int ledPin4 = 11;
int button1 = 2;
int button2 = 3;
int button3 = 4;
int button4 = 5;

int count = 1;
int delay_time = 1000;
int currentPlayer = 1;
int sequence[100];

// INPUT_PULLUP
int prevButton1State = HIGH; int button1State = HIGH;
int prevButton2State = HIGH; int button2State = HIGH;
int prevButton3State = HIGH; int button3State = HIGH;
int prevButton4State = HIGH; int button4State = HIGH;

bool player1failed = false;
bool player2failed = false;

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int melodyLost[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int melodyWin[] = {
  NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5,
  NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_C5,
  NOTE_C6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_G6, NOTE_E6,
  NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_B5,
  NOTE_FS5, NOTE_DS5, NOTE_DS5, NOTE_E5, NOTE_F5,
  NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_B5
};
int durations[] = {
  16, 16, 16, 16,
  32, 16, 8, 16,
  16, 16, 16, 32, 16, 8,
  16, 16, 16, 16, 32,
  16, 8, 32, 32, 32,
  32, 32, 32, 32, 32, 16, 8
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void defeatSound(){ // Online resources
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(12, melodyLost[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(12);
  }
}

void victorySound(){ // Online resources
  int size = sizeof(durations) / sizeof(int);
  for (int note = 0; note < size; note++) {
    int duration = 1000 / durations[note];
    tone(12, melodyWin[note], duration);
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    noTone(12);
  }
}

int waitforButtonPress(){
  // Initially HIGH
  button1State = digitalRead(button1);
  button2State = digitalRead(button2);
  button3State = digitalRead(button3);
  button4State = digitalRead(button4);

  while (true){
    prevButton1State = button1State;
    prevButton2State = button2State;
    prevButton3State = button3State;
    prevButton4State = button4State;
    delay(50);
    button1State = digitalRead(button1);
    button2State = digitalRead(button2);
    button3State = digitalRead(button3);
    button4State = digitalRead(button4);

    // If the player presses, the arduino will wait until they release. Up until that time, the LED that the button controls will keep lighting, and the tone will keep playing
    if (button1State == LOW && prevButton1State == HIGH) {
      digitalWrite(ledPin1, HIGH); 
      tone(12, NOTE_B0);
    }
    if (button1State == HIGH && prevButton1State == LOW) {
      digitalWrite(ledPin1, LOW); 
      noTone(12); 
      return 0;
    }
    if (button2State == LOW && prevButton2State == HIGH) {
      digitalWrite(ledPin2, HIGH); 
      tone(12, NOTE_G2);
    }
    if (button2State == HIGH && prevButton2State == LOW) {
      digitalWrite(ledPin2, LOW); 
      noTone(12); 
      return 1;
    }
    if (button3State == LOW && prevButton3State == HIGH) {
      digitalWrite(ledPin3, HIGH); 
      tone(12, NOTE_FS3);
    }
    if (button3State == HIGH && prevButton3State == LOW) {
      digitalWrite(ledPin3, LOW); 
      noTone(12); 
      return 2;
    }
    if (button4State == LOW && prevButton4State == HIGH) {
      digitalWrite(ledPin4, HIGH); 
      tone(12, NOTE_C4);
    }
    if (button4State == HIGH && prevButton4State == LOW) {
      digitalWrite(ledPin4, LOW); 
      noTone(12); 
      return 3;
    }
  }
}
// Reset the game, but the array is not reset. We instead change the elements of it. For example, if the sequence[0] is 0 at first and the new element is 1, it will be changed to 1 accordingly
void resetGame() {
  count = 1;
  delay_time = 1000;
  currentPlayer = 1;
  player1failed = false;
  player2failed = false;
}

void flashLed(){
  digitalWrite(ledPin1, HIGH); digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH); digitalWrite(ledPin4, HIGH);
  delay(1000);
  lcd.clear();
  digitalWrite(ledPin1, LOW); digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW); digitalWrite(ledPin4, LOW);
}

void setup(){
  pinMode(ledPin1, OUTPUT); 
  digitalWrite(ledPin1, LOW); 
  pinMode(ledPin2, OUTPUT); 
  digitalWrite(ledPin2, LOW);
  pinMode(ledPin3, OUTPUT); 
  digitalWrite(ledPin3, LOW);
  pinMode(ledPin4, OUTPUT); 
  digitalWrite(ledPin4, LOW);
  // Switching to input pullup, without a resistor connected to ground. Pushbutton is connected to ground and pin --> Simpler, but the logic will be completely reversed
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(12, OUTPUT);
  noTone(12);
  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(0));

  delay(500);
  button1State = digitalRead(button1); 
  prevButton1State = button1State;
  button2State = digitalRead(button2); 
  prevButton2State = button2State;
  button3State = digitalRead(button3); 
  prevButton3State = button3State;
  button4State = digitalRead(button4); 
  prevButton4State = button4State;
}

void loop(){
  noTone(12);
  delay(1000);
  lcd.clear();
  lcd.print("Level: ");
  lcd.print(count);
  lcd.setCursor(0, 1);
  lcd.print("Player ");
  lcd.print(currentPlayer);
  lcd.print("'s turn");
  delay(2000);
  lcd.clear();

  if (currentPlayer == 1 && !player1failed){
    sequence[count - 1] = random(0,4);
  }

  for (int i = 0; i < count; i++){
    int num = sequence[i];
    if (num == 0){
      digitalWrite(ledPin1, HIGH);
      tone(12, NOTE_B0, delay_time);
      delay(delay_time);
      digitalWrite(ledPin1, LOW);
    }
    else if (num == 1){
      digitalWrite(ledPin2, HIGH);
      tone(12, NOTE_G2, delay_time);
      delay(delay_time);
      digitalWrite(ledPin2, LOW);
    }
    else if (num == 2){
      digitalWrite(ledPin3, HIGH);
      tone(12, NOTE_FS3, delay_time);
      delay(delay_time);
      digitalWrite(ledPin3, LOW);
    }
    else{
      digitalWrite(ledPin4, HIGH);
      tone(12, NOTE_C4, delay_time);
      delay(delay_time);
      digitalWrite(ledPin4, LOW);
    }
    delay(200);
  }

  lcd.setCursor(0, 0);
  lcd.print("Your turn!");

  bool currentRoundSuccess = true;
  for (int i = 0; i < count; i++){
    int playerInput = waitforButtonPress();
    if (playerInput != sequence[i]){
      currentRoundSuccess = false;
      break;
    }
  }

  if (currentRoundSuccess == false){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Player ");
    lcd.print(currentPlayer);
    lcd.setCursor(0, 1);
    lcd.print("failed!");
    delay(1000);
    flashLed();
    if (currentPlayer == 1){
      player1failed = true;
    } else {
      player2failed = true;
    }
  }

  if (player1failed && currentPlayer == 2){
    lcd.clear();
    lcd.setCursor(0, 0);
    if (currentRoundSuccess){
      lcd.print("Player 2 Wins!");
      victorySound();
    } else {
      lcd.print("It's a draw!");
      lcd.setCursor(0, 1);
      lcd.print("Both failed!");
      defeatSound();
    }
    delay(2000);
    resetGame();
    return;
  }

  if (!player1failed && player2failed && currentPlayer == 2){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Player 1 Wins!");
    victorySound();
    delay(2000);
    resetGame();
    return;
  }

  if (currentPlayer == 1){
    currentPlayer = 2;
  } 
  else {
    currentPlayer = 1;
    count += 1;
    if (delay_time > 200){
      delay_time -= 50; // 200 ms is the fastest speed that a player can reach
    }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Level passed!");
  delay(1500);
  }
}
