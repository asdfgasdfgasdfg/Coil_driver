// // Robust float parse — returns true if successful, false on parse failure
// bool parseFloatSafe(const String& s, float& out) {
//     char buf[s.length() + 1];
//     s.toCharArray(buf, sizeof(buf));
//     char* end;
//     out = strtof(buf, &end);
//     return end != buf;  // false if no valid digits were consumed
// }

void updateMode(){
    if (upscaler) {
        switch (mode) {
            case 0: //upscaled braking
                setMode(3, 0, 1, 0);
                break;
            case 1: //upscaled unipolar
                setMode(2, 0, 1, 0);
                break;
            case 2: //upscaled bipolar
                setMode(4, 0, 1, 0);
                break;
        }
    } else {
        switch (mode) {
            case 0: //braking
                setMode(1, 0, 1, 0);
                break;
            case 1: //unipolar
                setMode(0, 0, 1, 0);
                break;
            case 2: //bipolar
                setMode(0, 0, 1, 0);
                break;
        }
    }
    if (!funcGen) setCurrent(current);
}

void serialLoop(){
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();  // Strip any trailing \r or whitespace

        if (input.length() == 0) return;

        if (input.charAt(0) == 'M') { //M{0-2} control modes (braking, unipolar, bipolar)
            String modeStr = input.substring(1);
            int parsed = modeStr.toInt();
            if (parsed >= 0 && parsed <= 2) {
                mode = parsed;
                updateMode();

                Serial.print("Mode set to: ");
                if (upscaler) Serial.print("Upscaled ");
                switch (mode) {
                    case 0:
                        Serial.println("Brake-mode PWM");
                        break;
                    case 1:
                        Serial.println("Unipolar PWM");
                        break;
                    case 2:
                        Serial.println("Bipolar PWM");
                        break;
                }
            } else Serial.println("Error, mode must be 0-2");
        } else if (input.charAt(0) == 'U') { //toggle upscaler
            upscaler = !upscaler;
            updateMode();

            Serial.print("Mode is now: ");
            if (upscaler) Serial.print("Upscaled ");
            switch (mode) {
                case 0:
                    Serial.println("Brake-mode PWM");
                    break;
                case 1:
                    Serial.println("Unipolar PWM");
                    break;
                case 2:
                    Serial.println("Bipolar PWM");
                    break;
            }
        }
        // if (input.startsWith("FA")) {
        //     // --- "FA{-1 to 1}" --- set funcAmp
        //     float parsed = input.substring(2).toFloat();
        //     if (parseFloatSafe(input.substring(2), parsed)) {
        //         if (parsed >= -1.0f && parsed <= 1.0f) {
        //             funcAmp = parsed;
        //             Serial.print("funcAmp set to: ");
        //             Serial.println(funcAmp, 4);
        //         } else {
        //             Serial.println("Error: funcAmp must be between -1.0 and 1.0.");
        //         }
        //     } else {
        //         Serial.println("Error: could not parse float after FA.");
        //     }

        // } else if (input.startsWith("FF")) {
        //     // --- "FF{0 to 1000}" --- set funcFreq
        //     float parsed = input.substring(2).toFloat();
        //     if (true) {
        //         if (parsed >= 0.0f && parsed <= 1000.0f) {
        //             funcFreq = parsed;
        //             Serial.print("funcFreq set to: ");
        //             Serial.println(funcFreq, 4);
        //         } else {
        //             Serial.println("Error: funcFreq must be between 0 and 1000.");
        //         }
        //     }
        // }
        else if (input.charAt(0) == 'F') {
            if (!funcGen) {
                funcGen = true;
                Serial.println("Function generator on");
            } else {
                funcGen = false;
                Serial.println("Function generator off");
            }
        }
        else if (input.charAt(0) == 'P') {
            if (!printOn) {
                printOn = true;
                Serial.println("Printing on");
            } else {
                printOn = false;
                Serial.println("Printing off");
            }
        }
        else { //set current
            float parsed = input.toFloat();
            if (parsed >= -1.0f && parsed <= 1.0f) {
                setCurrent(parsed);
                current = parsed;
                Serial.print("Current set to: ");
                Serial.println(parsed, 4);  // 4 decimal places
            } else {
                Serial.println("Error: current must be between -1.0 and 1.0.");
            }
        }
    }
}