// model.h
#ifndef MODEL_H
#define MODEL_H

int simple_classifier(float current_temp) {

    // Threshold for yaw (you can adjust this value)
    float yawThreshold = 45.0; // Yaw value that triggers the HTTP request

    if (current_temp >= yawThreshold) {
        return 1;  // out of range
    } else {
        return 0;  // OK
    }
}

#endif
