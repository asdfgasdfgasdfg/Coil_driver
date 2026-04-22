const int BUFFER_LEN = 100;

int buffer[BUFFER_LEN] = {0};
int total = 0;
int bufferIndex = 0;

const float SCALE_FACTOR = 10.0/6.1*5.0/1023.0;

void readCurrent(){
  total -= buffer[bufferIndex];
  buffer[bufferIndex] = analogRead(PIN_ANALOG);
  total += buffer[bufferIndex];
  
  if (++bufferIndex == BUFFER_LEN) {
    bufferIndex = 0;
  }
}

float getCurrentSmooth(){
  if (dir) return (float)total*(SCALE_FACTOR/BUFFER_LEN);
  else return -(float)total*(SCALE_FACTOR/BUFFER_LEN);
}

float getCurrent(){
    if (dir) return (float)buffer[(bufferIndex-1)%BUFFER_LEN]*SCALE_FACTOR;
    else return -(float)buffer[(bufferIndex-1)%BUFFER_LEN]*SCALE_FACTOR;
}