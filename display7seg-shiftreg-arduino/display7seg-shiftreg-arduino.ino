
//**************************************************************
//  Notes: Using SN74HC595N Shift Register to turn on
//         7 segment display
//  Components Datasheet:
//    https://img.filipeflop.com/files/download/Datasheet_74HC595.pdf
//**************************************************************


#define SEGMENT_COUNT 7  //how many segments I'm controlling
#define INIT_VALUE_INDEX 0
#define INIT_DISPLAY_INDEX 0

// shift register clock
int srclkPin = 2;
// storage register clock
int rclkPin = 4;
int dataPin = 11;

// the numbers to display from 1 to 9
static const char displayValues[] = { 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
static const int displayValuesSize = 9;
// the wait value will change the bottom dot of each display
static const char displayWaitValue = 0x80;
// display control port order (from left display to right)
static const int controlPins[] = { 12, 9, 8, 6 };
static const int controlPinsSize = 4;

int displayCount = 4;
int selectedDisplay = INIT_DISPLAY_INDEX;
int currentValueIndex = INIT_VALUE_INDEX;
int maxValueIndex = 8;

void setup() {
  pinMode(srclkPin, OUTPUT);
  pinMode(rclkPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(controlPins[0], OUTPUT);
  pinMode(controlPins[1], OUTPUT);
  pinMode(controlPins[2], OUTPUT);
  pinMode(controlPins[3], OUTPUT);
  clearDisplays();
  Serial.begin(9600);
}

void loop() {
  Serial.print("\n------------------------------------------\n");
  setActiveControlPin(controlPins[selectedDisplay]);
  digitalWrite(rclkPin, LOW);
  Serial.print("\n");
  Serial.print("VALUE INDEX: ");
  Serial.print(currentValueIndex);
  Serial.print("\n");
  shiftOut(dataPin, srclkPin, MSBFIRST, displayValues[currentValueIndex]);
  digitalWrite(rclkPin, HIGH);
  if (selectedDisplay >= (displayCount-1)) {
    selectedDisplay = INIT_DISPLAY_INDEX;
  } else {
    selectedDisplay++;
  }
  if (currentValueIndex >= maxValueIndex) {
    currentValueIndex = INIT_VALUE_INDEX;
  } else {
    currentValueIndex++;
  }
  delay(5000);
  clearDisplays();
  delay(100);
}

void clearDisplays() {
  shiftOut(dataPin, srclkPin, MSBFIRST, 0x00);
}

void setActiveControlPin(int pin) {
  Serial.print("\n");
  Serial.print("CONTROL COMPARE\n");
  Serial.print("\n");
  for (int i = 0; i < controlPinsSize; i++) {
    if (controlPins[i] == pin) {
      Serial.print(controlPins[i]);
      Serial.print("\t");
      Serial.print("set to high\n");
      // TODO: send 0 to enable the display
      digitalWrite(controlPins[i], LOW);
    } else {
      Serial.print(controlPins[i]);
      Serial.print("\t");
      Serial.print("set to low\n");
      // TODO: send 1 to disable the display
      digitalWrite(controlPins[i], HIGH);
    }
  }
}
