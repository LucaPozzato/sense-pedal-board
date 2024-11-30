// model.h
#ifndef MODEL_H
#define MODEL_H

int simple_classifier(float current_yaw) {

    // Threshold for yaw (you can adjust this value)
    float yawThreshold = 4500.0; // Yaw value that triggers the HTTP request

    if (current_yaw >= yawThreshold) {
        return 1;  // out of range
    } else {
        return 0;  // OK
    }
}

#endif
