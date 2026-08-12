#include <Arduino.h>

#include "clicker.h"

ToggleSwitcher::ToggleSwitcher(int buttonPin) {
  this->buttonPin = buttonPin;

  buttonState = HIGH;
  lastButtonState = HIGH;
  lastDebounceTime = 0;
}

void ToggleSwitcher::setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  buttonState = digitalRead(buttonPin);
}

int ToggleSwitcher::read() {
  return digitalRead(buttonPin);
}

int ToggleSwitcher::getButtonState() {
  return buttonState;
}

bool ToggleSwitcher::updateToggleSwitcherState() {
  int reading = digitalRead(buttonPin);

  int t = millis();
  if (reading != lastButtonState) {
    lastDebounceTime = t;
    lastButtonState = reading;
  }

  if ((t - lastDebounceTime) > toggleSwitcherDebounce) {
    buttonState = reading;
  }

  return isToggleSwitcherStateOn();
};

bool ToggleSwitcher::isToggleSwitcherStateOn() {
  return buttonState == LOW;
}

Clicker::Clicker(int buttonPin)
  : switcher(buttonPin) {
}

void Clicker::setup() {
  this->switcher.setup();
}

bool updateClicker(Clicker& clicker) {
  bool switcherState = clicker.switcher.updateToggleSwitcherState();

  if (clicker.lastSwitcherState != switcherState) {
    clicker.lastSwitcherState = switcherState;
    clicker.clickCount++;

    return true;
  }

  return false;
}
