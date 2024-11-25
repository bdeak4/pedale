#include "Keyboard.h"

const int ctrlButton = 2;
const int shiftButton = 3;
const int pageUpButton = 4;
const int pageDownButton = 7;

// Debounce variables
const unsigned long debounceDelay = 50; // Debounce time in milliseconds
unsigned long lastDebounceTimePageUp = 0;
unsigned long lastDebounceTimePageDown = 0;
unsigned long lastDebounceTimeHome = 0;
unsigned long lastDebounceTimeEnd = 0;
unsigned long lastDebounceTimeLock = 0;

int lastPageUpState = LOW;
int lastPageDownState = LOW;
bool pageUpTriggered = false;
bool pageDownTriggered = false;
bool homeTriggered = false;
bool endTriggered = false;
bool lockTriggered = false;

void setup() {
  pinMode(ctrlButton, INPUT);
  pinMode(shiftButton, INPUT);
  pinMode(pageUpButton, INPUT);
  pinMode(pageDownButton, INPUT);
  Keyboard.begin();
}

void loop() {
  int currentCtrlState = digitalRead(ctrlButton);
  int currentShiftState = digitalRead(shiftButton);
  int currentPageUpState = digitalRead(pageUpButton);
  int currentPageDownState = digitalRead(pageDownButton);

  // Debounce for page up button
  if (currentPageUpState != lastPageUpState) {
    lastDebounceTimePageUp = millis();
  }
  if ((millis() - lastDebounceTimePageUp) > debounceDelay) {
    if (currentPageUpState == HIGH && !pageUpTriggered) {
      Serial.print("page up\n");
      Keyboard.press(KEY_PAGE_UP);
      delay(10);
      Keyboard.release(KEY_PAGE_UP);
      pageUpTriggered = true;
    } else if (currentPageUpState == LOW) {
      pageUpTriggered = false;
    }
  }

  // Debounce for page down button
  if (currentPageDownState != lastPageDownState) {
    lastDebounceTimePageDown = millis();
  }
  if ((millis() - lastDebounceTimePageDown) > debounceDelay) {
    if (currentPageDownState == HIGH && !pageDownTriggered) {
      Serial.print("page down\n");
      Keyboard.press(KEY_PAGE_DOWN);
      delay(10);
      Keyboard.release(KEY_PAGE_DOWN);
      pageDownTriggered = true;
    } else if (currentPageDownState == LOW) {
      pageDownTriggered = false;
    }
  }

  // Debounce for home key (Shift + Page Up)
  if ((millis() - lastDebounceTimeHome) > debounceDelay) {
    if (currentShiftState == HIGH && currentPageUpState == HIGH && !homeTriggered) {
      lastDebounceTimeHome = millis();
      Serial.print("home\n");
      Keyboard.releaseAll();
      Keyboard.press(KEY_HOME);
      delay(10);
      Keyboard.releaseAll();
      homeTriggered = true;
    } else if (currentShiftState == LOW || currentPageUpState == LOW) {
      homeTriggered = false;
    }
  }

  // Debounce for end key (Shift + Page Down)
  if ((millis() - lastDebounceTimeEnd) > debounceDelay) {
    if (currentShiftState == HIGH && currentPageDownState == HIGH && !endTriggered) {
      lastDebounceTimeEnd = millis();
      Serial.print("end\n");
      Keyboard.releaseAll();
      Keyboard.press(KEY_END);
      delay(10);
      Keyboard.releaseAll();
      endTriggered = true;
    } else if (currentShiftState == LOW || currentPageDownState == LOW) {
      endTriggered = false;
    }
  }

  // Debounce for lock action (Ctrl + Page Down)
  if ((millis() - lastDebounceTimeLock) > debounceDelay) {
    if (currentCtrlState == HIGH && currentPageDownState == HIGH && !lockTriggered) {
      lastDebounceTimeLock = millis();
      Keyboard.releaseAll();
      Serial.print("lock\n");
      Keyboard.press(KEY_LEFT_GUI); // Simulate Windows key
      Keyboard.press('l');          // Simulate 'L' key
      delay(10);
      Keyboard.releaseAll();
      lockTriggered = true;
    } else if (currentCtrlState == LOW || currentPageDownState == LOW) {
      lockTriggered = false;
    }
  }

  // Ctrl and Shift keys (no debounce)
  if (currentCtrlState == HIGH) {
    Serial.print("ctrl\n");
    Keyboard.press(KEY_LEFT_CTRL);
    return;
  }

  if (currentShiftState == HIGH) {
    Serial.print("shift\n");
    Keyboard.press(KEY_LEFT_SHIFT);
    return;
  }

  lastPageUpState = currentPageUpState;
  lastPageDownState = currentPageDownState;

  Keyboard.releaseAll();
}
