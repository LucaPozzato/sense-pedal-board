// model.h
#ifndef MODEL_H
#define MODEL_H

int simple_yaw_classifier(float current_yaw) {
    // Threshold for yaw (you can adjust this value)
    float yawThreshold = 3500.0; // Yaw value that triggers the HTTP request

    if (current_yaw >= yawThreshold) {
      return 1;
    } else {
      return 0;
    }

}

int simple_pitch_classifier(float current_pitch) {
  if (current_pitch < 12.7) {
    return 0;  // out of range
  } else if (current_pitch < 25.4) {
    return 1;  // Interval 1
  } else if (current_pitch < 38.1) {
    return 2;  // Interval 2
  } else if (current_pitch < 50.8) {
    return 3;  // Interval 3
  } else if (current_pitch < 63.5) {
    return 4;  // Interval 4
  } else if (current_pitch < 76.2) {
    return 5;  // Interval 5
  } else if (current_pitch < 88.9) {
    return 6;  // Interval 6
  } else if (current_pitch < 101.6) {
    return 7;  // Interval 7
  } else if (current_pitch < 114.3) {
    return 8;  // Interval 8
  } else if (current_pitch < 127) {
    return 9;  // Interval 9
  }
}

#endif
