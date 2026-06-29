#include <stdlib.h>

const uint8_t PWM1 = 5;
const uint8_t PWM2 = 3;
const uint8_t PWM3 = 9;
const uint8_t PWM4 = 6;

const uint8_t D1 = 4; //!OE for Shift reg
const uint8_t D2 = 7; //Shift in for Shift reg
const uint8_t D3 = 8; //RCLK for shift reg
const uint8_t CLK = 13; //SPI CLK for DAC & shift reg
const uint8_t SYNC_PIN = 10; //SPI !SYNC for DAC
const uint8_t DIN = 11; //SPI DIN for DAC

float funcFreq = 0.5; //function generator sin wave frequency in Hz
float funcAmp = 0.3; //function generator amplitude (scalar from 0 to 1)

float current = 0;
int   mode    = 0; //0: braking, 1: unipolar, 2: bipolar
bool upscaler = false;
bool funcGen = false;
bool printOn = false;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);

    initpins();
    setMode(1, 0, 0, 0); //uint8_t coil1, uint8_t coil2, bool en1, bool en2
    initDACINtest();
    initPWM();

    // setMode(0, 0, 0, 0); //uint8_t coil1, uint8_t coil2, bool en1, bool en2
    // setMode(1, 0, 1, 1);
    
    Serial.println("Coil ready.");
}

long printTimer = 0; uint8_t printInterval = 100;
long funcTimer = 0; uint8_t funcInterval = 10;

bool test = false;
void loop() {
    // setMode(1, 0, test, 1);
    // test = !test;
    // delay(64);

    /* --- Serial commands: ---
    M0: braking
    M1: unipolar
    M2: bipolar
    U: toggle upscaler
    F: toggle function generator
    -1 -> 1: set current directly (disabled if function generator is off) */
    serialLoop(); //check for serial commands and update stuff accordingly

    //---Timed loops---
    long time = millis()/64; //64x correction factor bc of the PWM TIMER1 clock change
    if (funcGen && time > funcTimer) { //Function generator
        setCurrent(funcAmp * sin(2*PI*funcFreq/1000*time));
        funcTimer = time + funcInterval;
        // Serial.println(funcAmp * sin(2*PI*funcFreq/1000*time));
    }
    if (printOn && time > printTimer) { //Print current reading, in amps
        // Serial.println(analogRead(0));
        Serial.println((analogRead(0)-512)*5.0/1023.0/0.9);
        printTimer = time + printInterval;
    }
}
