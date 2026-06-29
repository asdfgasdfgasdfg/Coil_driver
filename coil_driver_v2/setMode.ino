/*MODES:
0: Direct control unipolar OR bipolar w/ freewheeling
1: Direct control braking OR bipolar w/ braking
2: Upscaled unipolar
3: Upscaled braking
4: Upscaled bipolar w/ freewheeling
5: Upscaled bipolar w/ braking
*/

// Reverse the 3 LSBs of a byte (bit0↔bit2, bit1 stays)
static uint8_t reverseBits3(uint8_t v) {
    return ((v & 0x01) << 2) | (v & 0x02) | ((v & 0x04) >> 2);
}

void setMode(uint8_t coil1, uint8_t coil2, bool en1, bool en2) {
    // Serial.print(coil1);
    // Serial.print(" ");
    // Serial.println(en1);
    if (coil1 > 5 || coil2 > 5) {
        Serial.println("Mode out of bounds error."); 
        return;
    }

    uint8_t c1 = reverseBits3(coil1 & 0x07); // coil1[0]→bit2, [1]→bit1, [2]→bit0
    uint8_t c2 = reverseBits3(coil2 & 0x07); // same for coil2

    uint8_t data = (c1            << 5)
                 | (c2            << 2)
                 | ((uint8_t)en2  << 1)
                 | ((uint8_t)en1  << 0);

    // Fully disable SPI peripheral — releases hardware pin mux on D13
    SPI.endTransaction();
    SPCR &= ~(1 << SPE);         // ← clear SPI Enable bit in hardware
    pinMode(CLK, OUTPUT);
    digitalWrite(CLK, LOW);

    digitalWrite(D3, LOW);
    shiftOut(D2, CLK, MSBFIRST, data);
    digitalWrite(D3, HIGH);
    digitalWrite(D1, LOW);

    // Re-enable SPI peripheral and reopen transaction for DAC
    SPCR |= (1 << SPE);
    // SPI.begin();                 // ← re-asserts SPI peripheral ownership of D13
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE1));
}