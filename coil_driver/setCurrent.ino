// vel: -1.0 (full reverse) to +1.0 (full forward), 0 = stop
void setCurrent(float modulationIndex) {
  modulationIndex = constrain(modulationIndex, -1.0f, 1.0f);

  if (modulationIndex == 0.0f) {
    TCCR1A &= ~(_BV(COM1A1) | _BV(COM1A0)); // Disconnect OC1A from Timer1
    digitalWrite(PIN_DIS, HIGH); // Drive HIGH = motor inactive, shut off all mosfets and break
    digitalWrite(PIN_PWM, LOW);                   
  } else {
    digitalWrite(PIN_DIS, LOW);
    // COM1A1 only = non-inverting mode: OC1A HIGH on match, LOW at BOTTOM
    TCCR1A = (TCCR1A & ~_BV(COM1A0)) | _BV(COM1A1);


    if (modulationIndex > 0.0f) {
      dir = true;
      digitalWrite(PIN_DIR, HIGH);
      OCR1A = (uint32_t)(PWM_TOP * modulationIndex);
    } else {
      dir = false;
      digitalWrite(PIN_DIR, LOW);
      OCR1A = (uint32_t)(PWM_TOP * -modulationIndex);
    }
  }
}

float getCurrentFromComputer(){
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() > 0) {
      float modulationIndex = input.toFloat();

      // Clamp to [-1, 1]
    //   modulationIndex = constrain(modulationIndex, -1.0f, 1.0f);

      Serial.print("Current set to: ");
      Serial.println(modulationIndex, 3);

      return modulationIndex;
    }
}