//sets duty cycle and direction depending on current control mode

void setCurrent(float current) {
    if (current > 1) current = 1;
    else if (current < -1) current = -1;

    if (upscaler) {
        switch (mode) {
            case 0: //braking
                setDuty(PWM1, (current>0)*255); //set direction
                writeDAC(0, int( (1-abs(current))*255.0 )); //PWM DIS pin
                break;
            case 1: //unipolar
                setDuty(PWM1, (current>0)*255); //set direction
                writeDAC(0, int( abs(current)*255.0 )); //PWM HEN pin
                break;
            case 2: //bipolar
                writeDAC(0, int( (current/2.0+0.5)*255.0 )); //PWM DIR pin
                if (current == 0) setDuty(PWM2, 0); //Set HEN to 0 to freewheel
                else setDuty(PWM2, 255); //Set HEN to 1
                break;
        }
    } else {
        switch (mode) {
            case 0: //braking
                setDuty(PWM1, (current>0)*255); //set direction
                setDuty(PWM2, int( (1-abs(current))*255.0) ); //PWM DIS pin
                break;
            case 1: //unipolar
                setDuty(PWM1, (current>0)*255); //set direction
                setDuty(PWM2, int( abs(current)*255.0) ); //PWM HEN pin
                break;
            case 2: //bipolar
                setDuty(PWM1, int( (current/2.0+0.5)*255.0 ) ); //PWM DIR pin
                if (current == 0) setDuty(PWM2, 0); //Set HEN to 0 to freewheel
                else setDuty(PWM2, 255); //Set HEN to 1
                break;
        }
    }
}