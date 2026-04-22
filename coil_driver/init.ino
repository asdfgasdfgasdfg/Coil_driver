void initPins() {
  pinMode(PIN_OUT, INPUT); //Comparator Out
  pinMode(PIN_EN, OUTPUT); //En
  digitalWrite(PIN_EN, 0);  //set enable to 0
  pinMode(PIN_DIS, OUTPUT);
  digitalWrite(PIN_DIS, 1);
  pinMode(PIN_PWM, OUTPUT); //PWM
  digitalWrite(PIN_PWM, 0); //set PWM to 0, means 0 current
  pinMode(PIN_DIR, OUTPUT); //DIR
}

void initHIP4080(){
  digitalWrite(PIN_DIR, 0);
  delay(50);
  while (digitalRead(PIN_OUT) != 0) {
    digitalWrite(PIN_DIR, 1);
    delay(50);
    digitalWrite(PIN_DIR, 0);
    delay(50);
  }
  digitalWrite(PIN_DIR, 1);
  delay(50);
  while (digitalRead(PIN_OUT) != 1) {
    digitalWrite(PIN_DIR, 0);
    delay(50);
    digitalWrite(PIN_DIR, 1);
    delay(50);
  }
  digitalWrite(PIN_DIR, 0);
  delay(50);
  while (digitalRead(PIN_OUT) != 0) {
    digitalWrite(PIN_DIR, 1);
    delay(50);
    digitalWrite(PIN_DIR, 0);
    delay(50);
  }
  digitalWrite(PIN_EN, 1);
}

void initTimer(){
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
  ICR1   = PWM_TOP;
  setCurrent(0);
}