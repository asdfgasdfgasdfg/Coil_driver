const float P = 0;
const float I = 1/1000.0;
const float D = 1000/1000.0;

const uint16_t PID_INTERVAL = 50;

unsigned long pidTimer = 0;

float intErr = 0;
float prevErr = 0;

void setCurrentPID(float current) {
  currentTarget = constrain(current, -2, 2);
}

void runCurrentPID(unsigned long time){
  if (time < pidTimer) return;
  
  float err = currentTarget-getCurrentSmooth();
  if (abs(err)<0.1*currentTarget) {
    pidTimer = time + PID_INTERVAL;
    return;
  }
  int dt = (time-pidTimer+PID_INTERVAL);

  float modIndex = P*err + D*(err-prevErr)/dt;
  if (modIndex+I*intErr > 1) {
    if (err < 0) {
        intErr += err*dt;
        modIndex += I*intErr;
    }
  } else if (modIndex+I*intErr < -1) {
    if (err > 0) {
        intErr += err*dt;
        modIndex += I*intErr;
    }
  } else {
    intErr += err*dt;
    modIndex += I*intErr;
  }
  pidTimer = time + PID_INTERVAL;

  setCurrent(modIndex);
}

