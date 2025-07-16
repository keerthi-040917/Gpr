#include <SPI.h>

#define FSYNC 10  // AD9833 Chip Select (SS)

void setup() {
    Serial.begin(9600);  // Start Serial Monitor for visualization
    pinMode(FSYNC, OUTPUT);
    SPI.begin();
    digitalWrite(FSYNC, HIGH);
    delay(10);
}

void loop() {
    // Sweep from 8.39 kHz to 16.39 kHz in 100 Hz steps
    for (long freq = 8390; freq <= 16390; freq += 100) {  
        setFrequency(freq);  // Set AD9833 to the frequency
        Serial.println(freq); // Print frequency to Serial Plotter
        delay(10); // Adjust for smooth chirp transition
    }
}

// Function to set the frequency of AD9833
void setFrequency(long frequency) {
    long freqReg = (frequency * 268435456) / 25000000;  // Convert to AD9833 register value
    int freqLSB = freqReg & 0x3FFF; // Lower 14 bits
    int freqMSB = (freqReg >> 14) & 0x3FFF; // Upper 14 bits

    digitalWrite(FSYNC, LOW);
    SPI.transfer(0x21);  // Control Register: Reset
    SPI.transfer(0x40 | (freqLSB & 0xFF)); // Lower 8 bits
    SPI.transfer(0x40 | ((freqLSB >> 8) & 0x3F)); // Upper 6 bits of LSB
    SPI.transfer(0x40 | (freqMSB & 0xFF)); // Lower 8 bits of MSB
    SPI.transfer(0x40 | ((freqMSB >> 8) & 0x3F)); // Upper 6 bits of MSB
    SPI.transfer(0xC0);  // Exit Reset
    digitalWrite(FSYNC, HIGH);
}
