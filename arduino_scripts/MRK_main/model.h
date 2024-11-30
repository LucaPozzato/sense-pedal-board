// model.h
#ifndef MODEL_H
#define MODEL_H

int simple_yaw_classifier(float current_yaw) {

    // Threshold for yaw (you can adjust this value)
    float yawThreshold = 4500.0; // Yaw value that triggers the HTTP request

    if (current_yaw > yawThreshold) {
      return 1;
    } else {
      return 0;
    }

}

int simple_pitch_classifier(float current_pitch) {
  if (current_pitch < 25.5) {
    return 0;  // out of range
  } else if (current_pitch < 51){
      return 1;  // OK
  } else if (current_pitch < 76.5){
      return 2;  // OK
  } else if (current_pitch < 102){
      return 3;  // OK
  } else {
      return 4;  // OK
  }
}

#endif
