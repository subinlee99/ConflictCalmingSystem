// Define pin constants for RGB LED and pulse sensor

#include <Particle.h>

#define RED_PIN D7
#define GREEN_PIN D6
#define BLUE_PIN D5
#define PULSE_PIN A1

int pulseValue = 0;

void setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(PULSE_PIN, INPUT);
}

void loop() {
    // Read pulse sensor value
    pulseValue = analogRead(PULSE_PIN);

    // Map the pulse value to a scale for smooth color transition
    int mappedPulse = map(pulseValue, 0, 1023, 0, 255);
    printf("%d", pulseValue);
    if (pulseValue > 100) {
        // High pulse - turn LED red
        setLEDColor(255, 0, 0); // Full red
    } else {
        // Transition from red to blue as pulse decreases
        int blueIntensity = map(pulseValue, 0, 100, 255, 0);  // More blue as pulse drops
        int greenIntensity = map(pulseValue, 50, 100, 0, 255); // Green comes in as pulse is medium

        // Set LED color to transition between red, yellow/green, and blue
        setLEDColor(255 - blueIntensity, greenIntensity, blueIntensity);
    }

    delay(100);
}

void setLEDColor(int red, int green, int blue) {
    // For common cathode, you write the values directly
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
}
