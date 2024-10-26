/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#define RED_PIN D7
#define GREEN_PIN D6
#define BLUE_PIN D5

const int pulsePin = A0;     
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

//Heartrate settings and variables
//hertrate code
unsigned long lastHeartRateReadTime = 0 ; // Tracks the time when the heart rate sensor was last read
const unsigned long heartRateReadInterval = 100; // Time interval for reading heart rate sensor (100 ms)
unsigned long currentTime; // Current time
// Array to store heart rate sensor readings

// Assuming the maximum array size based on 10 samples per second for 10 seconds
const int hrSensorMeasurements_maxSamples = 100;
int hrSensorMeasurements[hrSensorMeasurements_maxSamples];
int hrSensorThreshold = 2048;
int hrSensorMeasurements_sampleIndex = 0;
int hrSensor_CurrentValue = 0;
int currentBPM = 0;


void setLEDColor(int red, int green, int blue) {
    // For common cathode, you write the values directly
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
}


void setup() {
    // Begin serial communication for debugging
    Serial.begin(9600);
    
    // Initialize PIR pin as input
    pinMode(pulsePin, INPUT);

    // Connect to the Particle Cloud
    Particle.connect();
}



// void readHeartbeatSensor();
// void processHeartbeatSensor();


// Function to read heart rate sensor
void readHeartbeatSensor() {
    // Read the sensor value (assuming analogRead function and appropriate pin)
    int sensorValue = analogRead(pulsePin); //
    // Store the reading in the array
    hrSensorMeasurements[hrSensorMeasurements_sampleIndex] = sensorValue;
    // Increment and wrap the sample index
    hrSensorMeasurements_sampleIndex = (hrSensorMeasurements_sampleIndex + 1) % hrSensorMeasurements_maxSamples;
}



void getHeartbeatSensor() {
    int beatreate = 0;
    int isBeat = 0;
    for (int i = 0; i < hrSensorMeasurements_maxSamples; i++) {
        if (hrSensorMeasurements[i] >= hrSensorThreshold && isBeat==0) {
            isBeat = 1;
            beatreate++;
        } else{
            isBeat = 0;
        }
    }
    // (BPM)
    beatreate *= 6; // 60 seconds / 10 seconds window

    Serial.print("BPM: ");
    Serial.println (currentBPM); 
} 

void loop () {

currentTime = millis();
    // Check if it is time to read the heart rate sensor
if (currentTime - lastHeartRateReadTime >= heartRateReadInterval) {
        lastHeartRateReadTime = currentTime;
        //read hearbeat
        int sensorValue = analogRead(pulsePin); 
        hrSensorMeasurements[hrSensorMeasurements_sampleIndex] = sensorValue;
        hrSensorMeasurements_sampleIndex = (hrSensorMeasurements_sampleIndex + 1) % hrSensorMeasurements_maxSamples;
        getHeartbeatSensor();
    }

    if (currentBPM > 100) {
        // High pulse - turn LED red
        setLEDColor(255, 0, 0); // Full red
    } else {
        // Transition from red to blue as pulse decreases
        int blueIntensity = map(currentBPM, 0, 100, 255, 0);  // More blue as pulse drops
        int greenIntensity = map(currentBPM, 50, 100, 0, 255); // Green comes in as pulse is medium

        // Set LED color to transition between red, yellow/green, and blue
        setLEDColor(255 - blueIntensity, greenIntensity, blueIntensity);
    }

    delay(100);
}




