#include <Wire.h>

// TCA9554 I2C address (default is 0x20, can be 0x20-0x27 depending on A0-A2 pins)
#define TCA9554_ADDRESS 0x20

// TCA9554 Register addresses
#define TCA9554_INPUT_REG     0x00  // Input port register
#define TCA9554_OUTPUT_REG    0x01  // Output port register  
#define TCA9554_POLARITY_REG  0x02  // Polarity inversion register
#define TCA9554_CONFIG_REG    0x03  // Configuration register

// Pin assignments on TCA9554 (P0-P7)
#define ENCODER_A_PIN    7  // P0 - Encoder A signal
#define ENCODER_B_PIN    5  // P1 - Encoder B signal
#define ENCODER_BTN_PIN  6  // P2 - Encoder button

#define ENCODER2_A_PIN    4  // P0 - Encoder A signal
#define ENCODER2_B_PIN    1  // P1 - Encoder B signal
#define ENCODER2_BTN_PIN  0  // P2 - Encoder button

#define ENCODER_BTN3_PIN  2  // P2 - Encoder button
#define ENCODER_BTN4_PIN  3  // P2 - Encoder button

#define INT_PIN          43  // Arduino interrupt pin connected to TCA9554 INT

// Encoder variables
volatile bool encoderChanged = false;

int encoderPosition = 0;
int lastEncoderA = 0;
int lastEncoderB = 0;
bool lastButtonState = HIGH;
bool buttonPressed = false;


int encoder2Position = 0;
int lastEncoder2A = 0;
int lastEncoder2B = 0;
bool lastButton2State = HIGH;
bool button2Pressed = false;


bool button3Pressed = false;
bool button4Pressed = false;
bool lastButton3State = HIGH;
bool lastButton4State = HIGH;

// Timing variables for debouncing
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long lastDebounce2Time = 0;
unsigned long lastDebounce3Time = 0;
unsigned long lastDebounce4Time = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(5,6);
  scanI2C();
  // Initialize interrupt pin
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), encoderISR, FALLING);
  
  // Initialize TCA9554
  if (initTCA9554()) {
    Serial.println("TCA9554 initialized successfully");
  } else {
    Serial.println("Failed to initialize TCA9554");
    while(1); // Halt if initialization fails
  }
  
  // Read initial encoder state
  uint8_t initialState = readTCA9554();
  lastEncoderA = (initialState >> ENCODER_A_PIN) & 1;
  lastEncoderB = (initialState >> ENCODER_B_PIN) & 1;
  lastButtonState = (initialState >> ENCODER_BTN_PIN) & 1;

  lastEncoder2A = (initialState >> ENCODER2_A_PIN) & 1;
  lastEncoder2B = (initialState >> ENCODER2_B_PIN) & 1;
  lastButton2State = (initialState >> ENCODER2_BTN_PIN) & 1;

  lastButton3State = (initialState >> ENCODER_BTN3_PIN) & 1;
  lastButton4State = (initialState >> ENCODER_BTN4_PIN) & 1;
  
  Serial.println("Rotary Encoder with TCA9554 Ready");
  Serial.println("Turn encoder or press button...");
}

void loop() {
  // Check if encoder state changed
  if (encoderChanged) {
    encoderChanged = false;
    handleEncoderChange();
  }
  
  // Handle button press detection with debouncing
  if (buttonPressed && (millis() - lastDebounceTime) > debounceDelay) {
    buttonPressed = false;
    Serial.println("Button pressed!");
  }

  // Handle button press detection with debouncing
  if (button2Pressed && (millis() - lastDebounce2Time) > debounceDelay) {
    button2Pressed = false;
    Serial.println("Button2 pressed!");
  }
  
  // Handle button press detection with debouncing
  if (button3Pressed && (millis() - lastDebounce3Time) > debounceDelay) {
    button3Pressed = false;
    Serial.println("Button3 pressed!");
  }

  // Handle button press detection with debouncing
  if (button4Pressed && (millis() - lastDebounce4Time) > debounceDelay) {
    button4Pressed = false;
    Serial.println("Button4 pressed!");
  }

  // Small delay to prevent overwhelming the serial output
  delay(10);
}

// Interrupt service routine
void encoderISR() {
  encoderChanged = true;
}

bool initTCA9554() {
  // Set pins as inputs (1 = input, 0 = output)
  // We want encoder pins and button as inputs
  uint8_t configValue = (1 << ENCODER_A_PIN) | (1 << ENCODER_B_PIN) | (1 << ENCODER_BTN_PIN) | (1 << ENCODER2_A_PIN) | (1 << ENCODER2_B_PIN) | (1 << ENCODER2_BTN_PIN) | (1 << ENCODER_BTN3_PIN) | (1 << ENCODER_BTN4_PIN);
  
  Wire.beginTransmission(TCA9554_ADDRESS);
  Wire.write(TCA9554_CONFIG_REG);
  Wire.write(configValue);
  uint8_t result = Wire.endTransmission();
  
  if (result != 0) {
    Serial.print("I2C Error: ");
    Serial.println(result);
    return false;
  }
  
  // Optional: Set polarity inversion (0 = normal, 1 = inverted)
  Wire.beginTransmission(TCA9554_ADDRESS);
  Wire.write(TCA9554_POLARITY_REG);
  Wire.write(0x00); // No polarity inversion
  Wire.endTransmission();
  
  return true;
}

uint8_t readTCA9554() {
  Wire.beginTransmission(TCA9554_ADDRESS);
  Wire.write(TCA9554_INPUT_REG);
  uint8_t result = Wire.endTransmission();
  
  if (result != 0) {
    Serial.print("I2C Read Error: ");
    Serial.println(result);
    return 0;
  }
  
  Wire.requestFrom(TCA9554_ADDRESS, 1);
  if (Wire.available()) {
    return Wire.read();
  }
  return 0;
}

void handleEncoderChange() {
  uint8_t currentState = readTCA9554();
  
  // Extract individual pin states
  int currentEncoderA = (currentState >> ENCODER_A_PIN) & 1;
  int currentEncoderB = (currentState >> ENCODER_B_PIN) & 1;
  int currentButtonState = (currentState >> ENCODER_BTN_PIN) & 1;
  
  int currentEncoder2A = (currentState >> ENCODER2_A_PIN) & 1;
  int currentEncoder2B = (currentState >> ENCODER2_B_PIN) & 1;
  int currentButton2State = (currentState >> ENCODER2_BTN_PIN) & 1;

  int currentButton3State = (currentState >> ENCODER_BTN3_PIN) & 1;
  int currentButton4State = (currentState >> ENCODER_BTN4_PIN) & 1;

  // Handle encoder rotation
  if (currentEncoderA != lastEncoderA) {
    // Encoder A changed
    if (currentEncoderA == 0) { // Falling edge of A
      if (currentEncoderB == 1) {
        encoderPosition++; // Clockwise
        Serial.print("Encoder: ");
        Serial.print(encoderPosition);
        Serial.println(" (CW)");
      } else {
        encoderPosition--; // Counter-clockwise
        Serial.print("Encoder: ");
        Serial.print(encoderPosition);
        Serial.println(" (CCW)");
      }
    }
  }


  // Handle encoder rotation
  if (currentEncoder2A != lastEncoder2A) {
    // Encoder A changed
    if (currentEncoder2A == 0) { // Falling edge of A
      if (currentEncoder2B == 1) {
        encoder2Position++; // Clockwise
        Serial.print("Encoder2: ");
        Serial.print(encoder2Position);
        Serial.println(" (CW)");
      } else {
        encoder2Position--; // Counter-clockwise
        Serial.print("Encoder2: ");
        Serial.print(encoder2Position);
        Serial.println(" (CCW)");
      }
    }
  }
  
  // Handle button press (falling edge detection with debouncing)
  if (currentButtonState != lastButtonState) {
    if (currentButtonState == LOW && lastButtonState == HIGH) {
      // Button pressed (assuming active low)
      buttonPressed = true;
      lastDebounceTime = millis();
    }
  }

  // Handle button press (falling edge detection with debouncing)
  if (currentButton2State != lastButton2State) {
    if (currentButton2State == LOW && lastButton2State == HIGH) {
      // Button pressed (assuming active low)
      button2Pressed = true;
      lastDebounce2Time = millis();
    }
  }
  
  // Handle button press (falling edge detection with debouncing)
  if (currentButton3State != lastButton3State) {
    if (currentButton3State == LOW && lastButton3State == HIGH) {
      // Button pressed (assuming active low)
      button3Pressed = true;
      lastDebounce2Time = millis();
    }
  }

  // Handle button press (falling edge detection with debouncing)
  if (currentButton4State != lastButton4State) {
    if (currentButton4State == LOW && lastButton4State == HIGH) {
      // Button pressed (assuming active low)
      button4Pressed = true;
      lastDebounce2Time = millis();
    }
  }

  // Update last states
  lastEncoderA = currentEncoderA;
  lastEncoderB = currentEncoderB;
  lastButtonState = currentButtonState;

  lastEncoder2A = currentEncoder2A;
  lastEncoder2B = currentEncoder2B;
  lastButton2State = currentButton2State;

  lastButton3State = currentButton3State;
  lastButton3State = currentButton3State;
}

// Optional: Function to scan for I2C devices (useful for debugging)
void scanI2C() {
  Serial.println("Scanning I2C devices...");
  int deviceCount = 0;
  
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      deviceCount++;
    }
  }
  
  if (deviceCount == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.print("Found ");
    Serial.print(deviceCount);
    Serial.println(" I2C device(s)");
  }
}