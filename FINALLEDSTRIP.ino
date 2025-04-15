#include <FastLED.h>

#define LED_PIN     2
#define NUM_LEDS    10
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  100

CRGB leds[NUM_LEDS];

char incomingChar;
bool sequenceActive = false;
int sequenceState = 0;

unsigned long sequenceStartTime = 0;
unsigned long lastUpdateTime = 0;

int currentIndex = 6; // Start from LED 7 (index 6)
CRGB baseColor = CRGB(255, 200, 0); // Your target color (orange-ish)
uint8_t fadeBrightness = 255;
unsigned long fadeLastTime = 0;

// Track which LEDs are lit
bool ledLit[NUM_LEDS] = { false };

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  Serial.begin(9600);
}

void loop() {
  // Read serial input
  if (Serial.available() > 0) {
    incomingChar = Serial.read();
    if (incomingChar == 'a') {
      sequenceActive = true;
      sequenceState = 0;
      currentIndex = 6;
      fadeBrightness = 255;
      FastLED.clear();
      FastLED.show();
      // Clear which LEDs were lit
      for (int i = 0; i < NUM_LEDS; i++) {
        ledLit[i] = false;
      }
    }
  }

  if (sequenceActive) {
    unsigned long now = millis();

    // State 0: Light up LEDs from 7 to 1 one by one
    if (sequenceState == 0) {
      if (now - lastUpdateTime > 200) { // Delay between lighting each LED
        if (currentIndex >= 0) {
          leds[currentIndex] = baseColor;
          ledLit[currentIndex] = true; // Mark as lit
          FastLED.show();
          currentIndex--;
          lastUpdateTime = now;
        } else {
          sequenceStartTime = now;
          sequenceState = 1;
        }
      }
    }

    // State 1: Hold the color for 15 seconds
    else if (sequenceState == 1) {
      if (now - sequenceStartTime >= 10000) {
        sequenceState = 2;
        fadeLastTime = now;
      }
    }

    // State 2: Fade out ONLY the lit LEDs
    else if (sequenceState == 2) {
      if (now - fadeLastTime > 50) { // control fade speed
        fadeBrightness = max(0, fadeBrightness - 5); // reduce brightness
        for (int i = 0; i < NUM_LEDS; i++) {
          if (ledLit[i]) {
            leds[i] = baseColor;
            leds[i].nscale8(fadeBrightness); // scale brightness down
          } else {
            leds[i] = CRGB::Black;
          }
        }
        FastLED.show();
        fadeLastTime = now;
      }

      if (fadeBrightness == 0) {
        sequenceActive = false;
        sequenceState = 0;
        FastLED.clear();
        FastLED.show();
      }
    }
  }
}