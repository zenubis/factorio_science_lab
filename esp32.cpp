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

  srand(time(nullptr));

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
    else {
      isPrinting = false;
      Serial.println(F("[STATUS UPDATED]: Printer Resting -> Entering Soft Standby Pulse."));
    }
  }

  // 2. ANIMATION SWITCH ENGINE
  if (isPrinting) {
    //runActiveResearchSwirl();
    runResearchInProgress();
  } else {
    runIdleBreathingPulse();
  }
}

// -------------------------------------------------------------
// FACTORIO LIGHT ANIMATION ARRAYS
// -------------------------------------------------------------

void runActiveResearchSwirl() {
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
    pixels.setPixelColor(trail1, pixels.Color(r * 1.0,  g * 1.0,  b * 1.0));
    pixels.setPixelColor(trail2, pixels.Color(r * 0.66, g * 0.66, b * 0.66));
    pixels.setPixelColor(trail3, pixels.Color(r * 0.33, g * 0.33, b * 0.33));
    
    pixels.show();
    delay(60); // Speed of the rotation wrap
    
    // Quick exit catch: check terminal mid-loop so the interface remains snappy
    if (Serial.available() > 0) return; 
  }
}

float GetRandomStrength(float min, float max)
{
    return min + ((rand() % 1000) / 1000.0) * (max - min);
}

void runResearchInProgress()
{
  // animations that more closely minic the science labs in the game
    pixels.clear();

    // numbers of led to use for each color, total cannot be more than 12
    const uint8_t NUM_B = 4;
    const uint8_t NUM_R = 2;
    const uint8_t NUM_G = 2;
    const uint8_t NUM_LED = NUM_B + NUM_R + NUM_G; 

    // led idx, easy way to random without having duplicates;
    uint8_t led_id[12] = { 0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    for (int i = 0; NUM_LED > i; ++i) {
        uint8_t idx = rand() % (12-i);
        uint8_t temp = led_id[idx];
        led_id[idx] = led_id[i];
        led_id[i] = temp;
    }
    
    //char buf[50];
    //snprintf(buf, 50, "led[b1:%d b2:%d b3:%d r:%d g:%d]", led_id[0], led_id[1], led_id[2], led_id[3], led_id[4]);
    //Serial.println(buf);

    for (int i = 0; i < 20; ++i) {
        const uint8_t BLUE_R = 15;
        const uint8_t BLUE_G = 220;
        const uint8_t BLUE_B = 255;

        const uint8_t RED_R = 255;
        const uint8_t RED_G = 35;
        const uint8_t RED_B = 49;

        const uint8_t GREEN_R = 29;
        const uint8_t GREEN_G = 255;
        const uint8_t GREEN_B = 33;
        
        float strength = 0.f;
        for (int k = 0; NUM_B > k; ++k) {
            strength = GetRandomStrength(0.2f, 1.f);
            pixels.setPixelColor(led_id[k], BLUE_R * strength,  BLUE_G * strength,  BLUE_B * strength);
        }
        
        uint8_t base = NUM_B;
        for (int k = 0; NUM_R > k; ++k) {
            strength = GetRandomStrength(0.2f, 1.f);
            pixels.setPixelColor(led_id[base + k], RED_R * strength,  RED_G * strength,  RED_B * strength);
        }

        base += NUM_R;
        for (int k = 0; NUM_R > k; ++k) {
            strength = GetRandomStrength(0.2f, 1.f);
            pixels.setPixelColor(led_id[base + k], GREEN_R * strength,  GREEN_G * strength,  GREEN_B * strength);
        }
        
        pixels.show();
        delay(50);

        if (Serial.available() > 0) {
            return; // Immediate interruption catch
        } 
    }
}

void runIdleBreathingPulse() {
  // Slow, organic glowing pulse (Deep Factorio Purple style)
  const uint8_t STEP_START = 50;
  const uint8_t STEP_END = 150;
  
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
      return; // Immediate interruption catch
    } 
  }
}
