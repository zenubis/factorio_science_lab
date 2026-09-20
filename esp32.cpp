#include <Adafruit_NeoPixel.h>

// -------------------------------------------------------------
// HARDWARE SETTINGS
// -------------------------------------------------------------
#define LED_PIN    18   // Maps directly to GPIO 8 / IO8 on the ESP32-C3 Super Mini
#define NUM_LEDS   12   // Adjust to match your virtual NeoPixel ring array

// Instantiate the NeoPixel Controller Matrix
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// -------------------------------------------------------------
// GLOBAL APPLICATION STATE
// -------------------------------------------------------------
bool isPrinting = false; // Controls which factorio animation state plays

void setup() {
  // Initialize the Serial Debug Bus for text commands at standard speed
  Serial.begin(115200);
  
  // Power up the LED hardware channels
  pixels.begin();
  pixels.setBrightness(255); // 0 (Off) to 255 (Maximum Brightness)
  pixels.show();             // Wipe all pixels to clear startup noise

  Serial.println(F("============================================="));
  Serial.println(F("Factorio Science Lab - ESP32 C++ Simulation"));
  Serial.println(F("Instructions: Type 'PRINTING' or 'IDLE' below"));
  Serial.println(F("============================================="));
}

void loop() {
  // 1. SCENARIO EMULATOR: Read incoming commands from the simulation console
  if (Serial.available() > 0) {
    String inputCommand = Serial.readStringUntil('\n');
    inputCommand.trim(); // Strip white space and newlines
    
    if (inputCommand.equalsIgnoreCase("PRINTING")) {
      isPrinting = true;
      Serial.println(F("[STATUS UPDATED]: Printer Running -> Swirling Active Science Packs!"));
    } 
    else if (inputCommand.equalsIgnoreCase("IDLE")) {
      isPrinting = false;
      Serial.println(F("[STATUS UPDATED]: Printer Resting -> Entering Soft Standby Pulse."));
    }
  }

  // 2. ANIMATION SWITCH ENGINE
  if (isPrinting) {
    runActiveResearchSwirl();
  } else {
    runIdleBreathingPulse();
  }
}

// -------------------------------------------------------------
// FACTORIO LIGHT ANIMATION ARRAYS
// -------------------------------------------------------------
void runActiveResearchSwirl() 
{
  // Vibrant high-tech science pack green
  int r = 0;   
  int g = 255; 
  int b = 100;

  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.clear();
    
    // Set the leading bright pixel
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    
    // Set two fading trail pixels behind it to create an electrical arc look
    int trail1 = (i - 1 + NUM_LEDS) % NUM_LEDS;
    int trail2 = (i - 2 + NUM_LEDS) % NUM_LEDS;
    int trail3 = (i - 3 + NUM_LEDS) % NUM_LEDS;
    pixels.setPixelColor(trail1, pixels.Color(r,        g,        b));
    pixels.setPixelColor(trail2, pixels.Color(r * 0.66, g * 0.66, b * 0.66));
    pixels.setPixelColor(trail3, pixels.Color(r * 0.33, g * 0.33, b * 0.33));
    
    pixels.show();
    delay(60); // Speed of the rotation wrap
    
    // Quick exit catch: check terminal mid-loop so the interface remains snappy
    if (Serial.available() > 0) {
      return; 
    }
  }
}

void runIdleBreathingPulse() 
{
  // Slow, organic glowing pulse (Deep Factorio Purple style)
  const uint_8 STEP_START = 50;
  const uint_8 STEP_END = 150;
  
  // Breathe Brightness UP
  for (int step = STEP_START; step < STEP_END; step++) {
    pixels.fill(pixels.Color(step * 0.4, 0, step));
    pixels.show();
    delay(20);
    if (Serial.available() > 0) {
      return; // Immediate interruption catch
    }
  }
  
  // Breathe Brightness DOWN
  for (int step = STEP_END; step >= STEP_START; step--) {
    pixels.fill(pixels.Color(step * 0.4, 0, step));
    pixels.show();
    delay(20);
    if (Serial.available() > 0) {
      return;
    }
  }
}
