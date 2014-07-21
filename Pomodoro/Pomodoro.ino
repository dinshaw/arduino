/* switch
 *
 * Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
 * press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
 * a minimum delay between toggles to debounce the circuit (i.e. to ignore
 * noise).
 *
 * David A. Mellis
 * 21 November 2006
 */

int inPin = 2;         // the number of the input pin
int outPin = 13;       // the number of the output pin
int buzzerPin = 9;

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin


// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long blinkInterval = 500;
long lastBlink = 0;

long debounceInterval = 200;   // the debounce time, increase if the output flickers
long debounceStart = 0;  // the last time the output pin was toggled
long pomodoroStart = 0;
long breakStart = 0;
long pomodoroTime = 1500000; // 25 minutes
long breakTime = 300000; // 5 minutes
// long pomodoroTime = 5000; // 5 seconds
// long breakTime = 5000; // 10 second
boolean onBreak = false;
boolean onPomodoro = false;


void setup()
{
  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop()
{
  reading = digitalRead(inPin);
  state = digitalRead(outPin);

  // if the time has run out, turn the light off
  if (onPomodoro == true && millis() - pomodoroStart > pomodoroTime) {
    start_break();
  }

  if (onBreak == true) {
    long currentMillis = millis();
    if (currentMillis - lastBlink > blinkInterval) {
      lastBlink = currentMillis;
      if (state == LOW)
        state = HIGH;
      else
        state = LOW;
    }
    if (millis() - breakStart > breakTime) {
      end_break();
    }
  }


  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - debounceStart > debounceInterval) {
    if (onPomodoro == true || onBreak == true) {
      stop_pomodoro();
    }
    else
    {
      start_pomodoro();
    }

    debounceStart = millis();
  }

  digitalWrite(outPin, state);

  previous = reading;
}

void start_pomodoro() {
  play_start_tone();
  state = HIGH;
  onPomodoro = true;
  pomodoroStart = millis();
}

void stop_pomodoro() {
  play_cancel_tone();
  state = LOW;
  onPomodoro = false;
  onBreak = false;
}

void start_break() {
  play_break_start_tone();
  onBreak = true;
  onPomodoro = false;
  breakStart = millis();
}

void end_break() {
  play_break_end_tone();
  onBreak = false;
  state = LOW;
}

void play_start_tone() {
  tone(buzzerPin, 1014, 500);
  delay(500);
  tone(buzzerPin, 1136, 500);
  delay(500);
  tone(buzzerPin, 1275, 500);
}

void play_cancel_tone() {
  tone(buzzerPin, 500, 1000);
}

void play_break_start_tone() {
  tone(buzzerPin, 1275, 500);
  delay(500);
  tone(buzzerPin, 1136, 500);
  delay(500);
  tone(buzzerPin, 1275, 500);
}

void play_break_end_tone() {
  tone(buzzerPin, 1136, 500);
  delay(500);
  tone(buzzerPin, 1275, 500);
}

