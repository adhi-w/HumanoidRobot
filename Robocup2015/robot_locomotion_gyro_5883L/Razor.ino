#define FACTOR 0.75
bool checkCalibration; 

void RazorInit() {  
  ahrs.Init(ax, ay, az, gx, gy, gz, mx, my, mz, 0.02);
  for(int i = 0; i < 3; i++){
    magnetom_min[i] = 0;
    magnetom_max[i] = 0;
  }

  checkCalibration = true;
}

void RazorRead() {
  if(GYRO_FEEDBACK == 1){    
    timestamp_old = timestamp;
    timestamp = millis();
    if (timestamp > timestamp_old)
      G_Dt = (float) (timestamp - timestamp_old) / 1000.0f; // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
    else G_Dt = 0;

    AHRSRead();    

    ahrs.Update(ax, ay, az, gx, gy, gz, mx, my, mz, G_Dt);
    roll = ahrs.roll * 180/PI;
    pitch = ahrs.pitch * 180/PI;
    yaw = ahrs.yaw * 180/PI;
    
    if(OUTPUT_SENSOR == 1 && TEST == 1){
      if(CALIBRATION == 0){
        float ypr[3];
        ypr[0] = yaw;
        ypr[1] = pitch;
        ypr[2] = roll;
        Serial.write((byte*)ypr,12);
      } 
      else if(CALIBRATION == 1) {
        if(checkCalibration == true){
          for (int i = 0; i < 3; i++) {          
            magnetom_min[i] = magnetom_max[i] = magnetom[i];
          }

          checkCalibration = false;
        }
        Serial.print("magn x,y,z (min/max) = ");
        for (int i = 0; i < 3; i++) {
          if (magnetom[i] < magnetom_min[i]) magnetom_min[i] = magnetom[i];
          if (magnetom[i] > magnetom_max[i]) magnetom_max[i] = magnetom[i];          
          Serial.print(magnetom_min[i]);
          Serial.print("/");
          Serial.print(magnetom_max[i]);
          if (i < 2) Serial.print("  ");
          else Serial.println();
        }
      }
      else if (CALIBRATION == 2){
        PrintIt();
      }  
    }
  }
}

float ReadFloat() {
  for (int i = 0; i < 4; i++) {
    float2Byte.b[i] = Serial3.read();
  }

  return float2Byte.f;
}


















