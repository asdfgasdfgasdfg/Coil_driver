void initPWM() {
    // ── Timer0: pins 5 (OC0B) and 6 (OC0A) ─ Fast PWM, TOP=0xFF ──
    // f = 16MHz / (1 * 256) = 62.5 kHz
    // NOTE: millis()/delay() will run 64x fast — avoid them
    pinMode(PWM1, OUTPUT); // pin 5
    pinMode(PWM4, OUTPUT); // pin 6
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS00);  // prescaler=1, Fast PWM TOP=0xFF
    OCR0A  = 0;            // pin 6 duty
    OCR0B  = 0;            // pin 5 duty

    // ── Timer1: pin 9 (OC1A) ─ Fast PWM, TOP=0xFF ─────────────────
    // f = 16MHz / (1 * 256) = 62.5 kHz
    pinMode(PWM3, OUTPUT); // pin 9
    TCCR1A = (1 << COM1A1) | (1 << WGM10); // Fast PWM 8-bit (mode 5, TOP=0xFF)
    TCCR1B = (1 << WGM12)  | (1 << CS10);  // prescaler=1
    OCR1A  = 0;

    // ── Timer2: pin 3 (OC2B) ─ Fast PWM, TOP=0xFF ─────────────────
    // f = 16MHz / (1 * 256) = 62.5 kHz
    pinMode(PWM2, OUTPUT); // pin 3
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS20);  // prescaler=1, Fast PWM TOP=0xFF
    OCR2B  = 0;
}

// duty: 0–255 for all pins
void setDuty(uint8_t pin, uint8_t duty) {
    if (duty == 0 || duty == 255) {
        // Disconnect timer from pin, then drive it statically
        switch (pin) {
            case 5:  TCCR0A &= ~((1 << COM0B1) | (1 << COM0B0)); break;
            case 3:  TCCR2A &= ~((1 << COM2B1) | (1 << COM2B0)); break;
            case 9:  TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0)); break;
            case 6:  TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0)); break;
        }
        digitalWrite(pin, duty == 255 ? HIGH : LOW);

    } else {
        // Re-connect timer to pin if it was previously disconnected, then set duty
        switch (pin) {
            case 5:
                TCCR0A |= (1 << COM0B1);
                OCR0B = duty;
                break;
            case 3:
                TCCR2A |= (1 << COM2B1);
                OCR2B = duty;
                break;
            case 9:
                TCCR1A |= (1 << COM1A1);
                OCR1A = duty;
                break;
            case 6:
                TCCR0A |= (1 << COM0A1);
                OCR0A = duty;
                break;
        }
    }
}