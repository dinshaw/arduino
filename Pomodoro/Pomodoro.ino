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

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin


// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long pomodoroStart = 0;
long breakStart = 0;
long debounce = 200;   // the debounce time, increase if the output flickers
long pomodoroTime = 10000; // 10 seconds
long breakTime = 5000; //  1500000 25 minutes
boolean onBreak = false;
boolean onPomodoro = false;


void setup()
{
  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
}

void loop()
{
  reading = digitalRead(inPin);
  state = digitalRead(outPin);

  // if the time has run out, turn the light off
  if (onPomodoro == true && millis() - pomodoroStart > pomodoroTime) {
    onBreak = true;
    onPomodoro = false;
    breakStart = millis();
  }

  if (onBreak == true) {
    digitalWrite(outPin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);               // wait for a second
    digitalWrite(outPin, LOW);    // turn the LED off by making the voltage LOW
    delay(500);

    if (millis() - breakStart > breakTime) {
      onBreak = false;
      state = LOW;
    }
  }


  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH) {
      state = LOW;
    }
    else
    {
      state = HIGH;
      onPomodoro = true;
      pomodoroStart = millis();
    }

    time = millis();
  }

  digitalWrite(outPin, state);

  previous = reading;
}
