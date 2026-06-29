#include <SPI.h>

// Corrected pin assignments for Arduino Uno hardware SPI:
// D11 -> DIN   (MOSI)  — hardware SPI MOSI
// D13 -> SCLK          — hardware SPI SCK
// D10 -> !SYNC (CS)    — driven manually

// DAC channel select (A0 bit)
#define DAC_A 0
#define DAC_B 1

// Operation modes (OP1:OP0)
#define OP_WRITE_UPDATE 0b01  // Write to register and update output

void writeDAC(uint8_t channel, uint8_t value) {
  // 16-bit input register layout (MSB first):
  // Bit 15   : 0 (must be 0)
  // Bit 14   : A0 (channel select: 0=DAC A, 1=DAC B)
  // Bits 13-12: OP1:OP0 (operation mode)
  // Bits 11-4 : D7:D0 (8-bit data, MSB first)
  // Bits 3-0  : don't care
  uint16_t word = 0;
  word |= (uint16_t)(channel & 0x01)   << 14;
  word |= (uint16_t)(OP_WRITE_UPDATE)  << 12;
  word |= (uint16_t)(value)            <<  4;

  digitalWrite(SYNC_PIN, LOW);
  SPI.transfer16(word);
  digitalWrite(SYNC_PIN, HIGH);
}

void initDACINtest() {
  pinMode(SYNC_PIN, OUTPUT);
  digitalWrite(SYNC_PIN, HIGH);  // Deselect DAC initially

  // DAC082S085 clocks data on falling SCLK edges -> SPI Mode 1
  // Max SCLK = 40 MHz; use 8 MHz to stay well within spec
  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
}

void loopDACINtest() {
  // --- Ramp UP: 0 to 255 in 1 ms ---
  // 256 steps over 1000 us -> ~3.9 us per step
  // SPI transfer at 8 MHz takes ~2 us for 16 bits, leaving ~1.9 us delay
  for (int i = 0; i <= 255; i++) {
    writeDAC(DAC_A, (uint8_t)i);
    delayMicroseconds(2);  // Tune this to hit exactly 1 ms total ramp time
  }

  // --- Ramp DOWN: 255 to 0 in 1 ms ---
  for (int i = 255; i >= 0; i--) {
    writeDAC(DAC_A, (uint8_t)i);
    delayMicroseconds(2);
  }
}