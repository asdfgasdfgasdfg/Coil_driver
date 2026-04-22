const uint16_t PWM_TOP = 400;

const uint8_t  PIN_OUT = 2;
const uint8_t PIN_EN = 3;

const uint8_t  PIN_DIR = 8;
const uint8_t PIN_PWM = 9;
const uint8_t PIN_DIS = 10;

const uint8_t PIN_ANALOG = 0;

const uint16_t PRINT_INTERVAL = 100;
const uint16_t READ_INTERVAL = 10;

unsigned long printTimer = 0;
unsigned long readTimer = 0;
unsigned long time = 0;

bool dir = false;
float currentTarget = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  initPins();
  initHIP4080();
  initTimer();

  Serial.println("Current drive ready.");
  Serial.println("Enter current amount [-1.0, 1.0]:");
}

float modIndex;
float k = 0.0063;
int kIndex = 0;
void loop() {
  if (Serial.available()) {modIndex = getCurrentFromComputer(); setCurrent(modIndex);}
  time = millis();
//   runCurrentPID(time);
//   if (++kIndex > 45) {
//     kIndex = 0;
//     k = 0.0063;
//   } else {k *= 1.1;}
//   setCurrent(sin(k*time));

  if (time >= readTimer) {
    readCurrent();
    readTimer = time + READ_INTERVAL;
  }
  
  if (time >= printTimer) {
    // Serial.print(currentTarget);
    // Serial.print(", ");
    Serial.println(getCurrentSmooth());
    printTimer = time + PRINT_INTERVAL;
  }
}

