//#define AUTO_TURNOFF_ATT  25


float rmotor[6] = {
  180 + R_ANKLE_OFFSET, 180, 180 + 75 , 180 - 75, 180 + R_HIP_OFFSET, 180 + R_YAW_OFFSET
};
float lmotor[6] = {
  180 + L_ANKLE_OFFSET, 180, 180 - 75 , 180 + 75, 180 + L_HIP_OFFSET, 180 + L_YAW_OFFSET
};

void ProtectHead(int desiredPosition1, int desiredPosition2, int speed){
  //PUT THE COMMENTED COMMAND TO SETUP()
  //pinMode(SELECT, OUTPUT);
  digitalWrite(SELECT, LOW);
  
  unsigned char array[20];
  
  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (4 + 1) * 2 + 4;		//parameter2 Length of Data to write
  array[4] = 0x83;
  array[5] = 0x1E;
  array[6] = 0x04;
  
  array[7] = 103;
  array[8] = -255 % 256;
  array[9] = -255 / 256;
  array[10] = (speed/10) % 256;
  array[11] = (speed/10) / 256;
  
  array[12] = 104;
  array[13] = 512 % 256;
  array[14] = 512 / 256;
  array[15] = (speed/10) % 256;
  array[16] = (speed/10) / 256;
  
  unsigned char sum = 0;
  
  for(int i = 2; i <= 16; i++){
    sum += array[i];    
  }
  
  array[17] = ~sum;
  
  Serial2.write(array, 18);
  
  //dont forget to put this in the end of stand up procedure
  // digitalWrite(SELECT, HIGH);
}


void ArmInit() {
  byte array[20];

  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (4 + 1) * 2 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x06;			//Parameter1 Start address to write Data
  array[6] = 0x04;			//Parameter2 Length of Data to write

  array[7] = 15;//3
  
  array[8] = 255;
  array[9] = 15;  
  array[10] = 255;
  array[11] = 15;
  
  array[12] = 16;
  
  array[13] = 255;
  array[14] = 15;
  array[15] = 255;
  array[16] = 15;
  
  byte sum = 0x00;

  for (int i = 2; i <= 16 ; i++)
  {
    sum += array[i];
  }
  array[17] = ~sum;
  //      PORTD|=0x80;
  for (int j = 0 ; j <= 17; j++)
    Serial1.write(array[j]);
}


void WaistMotor(int waist, int motor_velocity)
{
  unsigned char array[108];//20*5+7+1
  unsigned int r[6];
  unsigned int l[6];

  for (int i = 0; i < 6; i++)
  {
    l[i] = round( (lmotor[i] / 360.0 * 4096.0));
    r[i] = round( (rmotor[i] / 360.0 * 4096.0));
  }

  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (4 + 1) * 13 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x1E;			//Parameter1 Start address to write Data
  array[6] = 0x04;			//Parameter2 Length of Data to write

  for (int i = 0; i < 6; i++)
  {
    array[(i * 5) + 7] = i + 2;
    array[((i * 5) + 1) + 7] = r[i] % 256;
    array[((i * 5) + 2) + 7] = r[i] / 256;
    array[((i * 5) + 3) + 7] = motor_velocity % 256;
    array[((i * 5) + 4) + 7] = motor_velocity / 256;
  }
  for (int i = 0; i < 6  ; i++)
  {
    array[(i * 5) + 37] = i + 8;
    array[((i * 5) + 1) + 37] = l[i] % 256;
    array[((i * 5) + 2) + 37] = l[i] / 256;
    array[((i * 5) + 3) + 37] = motor_velocity % 256;
    array[((i * 5) + 4) + 37] = motor_velocity / 256;
  }

  array[67] = 100;
  array[68] = round(((180 + waist) / 360.0 * 4096.0)) % 256;
  array[69] = round(((180 + waist) / 360.0 * 4096.0)) / 256;
  array[70] = motor_velocity % 256;
  array[71] = motor_velocity / 256;

  unsigned char sum = 0x00;
  for (int i = 2; i < 72 ; i++)
  {
    sum += array[i];
  }
  array[72] = ~sum;
  //      PORTD|=0x80;
  Serial1.write(array, 73);

}

void ArmStandUp(int speed, int r_hand, int l_hand, int l_elbow, int r_elbow) {
  byte array[108];

  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (4 + 1) * 8 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x1E;			//Parameter1 Start address to write Data
  array[6] = 0x04;			//Parameter2 Length of Data to write

  array[7] = 15;//3
  array[8] = int((180 + r_hand) / 360.0 * 4096.0) % 256;
  array[9] = int((180 + r_hand) / 360.0 * 4096.0) / 256;
  array[10] = speed % 256;
  array[11] = speed / 256;

  array[12] = 16;
  array[13] = int((180 + l_hand) / 360.0 * 4096.0) % 256;
  array[14] = int((180 + l_hand) / 360.0 * 4096.0) / 256;
  array[15] = speed % 256;
  array[16] = speed / 256;

  array[17] = 17;//3
  array[18] = int(180.0 / 360.0 * 4096.0) % 256;
  array[19] = int(180.0 / 360.0 * 4096.0) / 256;
  array[20] = speed % 256;
  array[21] = speed / 256;

  array[22] = 18;
  array[23] = int(180.0 / 360.0 * 4096.0) % 256;
  array[24] = int(180.0 / 360.0 * 4096.0) / 256;
  array[25] = speed % 256;
  array[26] = speed / 256;

  array[27] = 19;//3
  array[28] = int(180.0 / 360.0 * 4096.0) % 256;
  array[29] = int(180.0 / 360.0 * 4096.0) / 256;
  array[30] = speed % 256;
  array[31] = speed / 256;

  array[32] = 20;
  array[33] = int(180.0 / 360.0 * 4096) % 256;
  array[34] = int(180.0 / 360.0 * 4096) / 256;
  array[35] = speed % 256;
  array[36] = speed / 256;

  array[37] = 21;//3
  array[38] = int(180.0 / 360.0 * 4096) % 256;
  array[39] = int(180.0 / 360.0 * 4096) / 256;
  array[40] = speed % 256;
  array[41] = speed / 256;

  array[42] = 22;
  array[43] = int(180.0 / 360.0 * 4096) % 256;
  array[44] = int(180.0 / 360.0 * 4096) / 256;
  array[45] = speed % 256;
  array[46] = speed / 256;

  byte sum = 0x00;

  for (int i = 2; i < 47 ; i++)
  {
    sum += array[i];
  }
  array[47] = ~sum;
  //      PORTD|=0x80;
  for (int j = 0 ; j < 50; j++)
    Serial1.write(array[j]);
}

void ShoulderStandUp(int speed, int r_hand, int l_hand) {
  byte array[30];

  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (4 + 1) * 4 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x1E;			//Parameter1 Start address to write Data
  array[6] = 0x04;			//Parameter2 Length of Data to write

  array[7] = 15;//3
  array[8] = int((180 + r_hand) / 360.0 * 4096.0) % 256;
  array[9] = int((180 + r_hand) / 360.0 * 4096.0) / 256;
  array[10] = speed % 256;
  array[11] = speed / 256;

  array[12] = 16;
  array[13] = int((180 + l_hand) / 360.0 * 4096.0) % 256;
  array[14] = int((180 + l_hand) / 360.0 * 4096.0) / 256;
  array[15] = speed % 256;
  array[16] = speed / 256;

  array[17] = 17;//3
  array[18] = int(180.0 / 360.0 * 4096.0) % 256;
  array[19] = int(180.0 / 360.0 * 4096.0) / 256;
  array[20] = speed % 256;
  array[21] = speed / 256;

  array[22] = 18;
  array[23] = int(180.0 / 360.0 * 4096.0) % 256;
  array[24] = int(180.0 / 360.0 * 4096.0) / 256;
  array[25] = speed % 256;
  array[26] = speed / 256;

  byte sum = 0x00;

  for (int i = 2; i < 27 ; i++)
  {
    sum += array[i];
  }
  array[27] = ~sum;
  //      PORTD|=0x80;
  for (int j = 0 ; j < 28; j++)
    Serial1.write(array[j]);
}

void ElbowStandUp(int speed, int r_hand, int l_hand) {
  byte array[30];

  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (4 + 1) * 2 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x1E;			//Parameter1 Start address to write Data
  array[6] = 0x04;			//Parameter2 Length of Data to write

  array[7] = 19;//3
  array[8] = int((180.0 + l_hand) / 360.0 * 4096.0) % 256;
  array[9] = int((180.0 + l_hand) / 360.0 * 4096.0) / 256;
  array[10] = speed % 256;
  array[11] = speed / 256;

  array[12] = 20;
  array[13] = int((180.0 + r_hand) / 360.0 * 4096.0) % 256;
  array[14] = int((180.0 + r_hand) / 360.0 * 4096.0) / 256;
  array[15] = speed % 256;
  array[16] = speed / 256;


  byte sum = 0x00;

  for (int i = 2; i < 17 ; i++)
  {
    sum += array[i];
  }
  array[17] = ~sum;
  //      PORTD|=0x80;
  for (int j = 0 ; j < 18; j++)
    Serial1.write(array[j]);
}

void TorqueEnable(bool enable)
{
  unsigned char array[108];//20*5+7+1

  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (1 + 1) * 4 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x18;			//Parameter1 Start address to write Data
  array[6] = 0x01;			//Parameter2 Length of Data to write

  array[7] = 4;
  array[8] = enable;

  array[9] = 5;
  array[10] = enable;

  array[11] = 10;
  array[12] = enable;

  array[13] = 11;
  array[14] = enable;


  unsigned char sum = 0x00;
  for (int i = 2; i <= 14 ; i++)
  {
    sum += array[i];
  }
  array[15] = ~sum;
  //      PORTD|=0x80;
  Serial1.write(array, 16);
}

void MotorTorqueEnable(bool enable) {
  unsigned char array[35];//20*5+7+1

  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (1 + 1) * 15 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x18;			//Parameter1 Start address to write Data
  array[6] = 0x01;			//Parameter2 Length of Data to write

  for (int i = 0; i < 6; i++)
  {
    array[(i * 2) + 7] = i + 2;
    array[((i * 2) + 1) + 7] = enable;
  }
  for (int i = 0; i < 6  ; i++)
  {
    array[(i * 2) + 19] = i + 8;
    array[((i * 2) + 1) + 19] = enable;
  }

  array[31] = 15;
  array[32] = enable;

  array[33] = 16;
  array[34] = enable;

  array[35] = 100;
  array[36] = enable;

  unsigned char sum = 0x00;
  for (int i = 2; i <= 36 ; i++)
  {
    sum += array[i];
  }
  array[37] = ~sum;
  Serial1.write(array, 38);
  //digitalWrite(16, LOW);
}

void StandUp() {
  ProtectHead(-180, 180, 1000);
  if (phase == 0 && (wait_time_function(3000))) {
    WaistMotor(0, 100);
    WaistMotor(-90, 100);
    //Serial.print(phase);
    //Serial.print(",");
    //Serial.println(pitch);
    phase = 1;
  }
  else if (phase == 1 && (wait_time_function(2000))) {
    ShoulderStandUp(150, -370, 370);
    phase = 2;
    //Serial.print(phase);
    //Serial.print(",");
    //Serial.println(pitch);
    
  }
  else if (phase == 2 && (wait_time_function(4000))) {
    WaistMotor(0, 150);
    phase = 3;
    //Serial.print(phase);
    //Serial.print(",");
    //Serial.println(pitch);
    
  }

  else if (phase == 3 && (wait_time_function(500))) {
    TorqueEnable(0);
    phase = 4;
    //Serial.print(phase);
    //Serial.print(",");
    //Serial.println(pitch);
    
  }

  else if (phase == 4 && (wait_time_function(1500))) {
    ShoulderStandUp(150, 0, 0);
    WaistMotor(0, 50);
    Robot_state = 1;
    standUpProcedure = 0;
    phase = 0;
    //Serial.print(phase);
    //Serial.print(",");
    //Serial.println(pitch);
    
    digitalWrite(SELECT, HIGH);
  }  
}

void BackStandUp() {
  ProtectHead(-180, 180, 1000);
  if (phase == 0 && (wait_time_function(3000))) {
    WaistMotor(0, 100);
    WaistMotor(80, 100);
    //Serial.print(phase);
    //Serial.print(",");
    //Serial.println(pitch);
    phase = 1;
  }
  else if (phase == 1 && (wait_time_function(2000))) {
    ShoulderStandUp(150, 250, -250);
    //Serial.print(phase);
    //Serial.print(",");
    //Serial.println(pitch);
    
    phase = 2;
  }
  else if (phase == 2 && (wait_time_function(4000))) {
    WaistMotor(0, 150);
    ShoulderStandUp(150, 320, -320);
    //Serial.print(phase);
    //Serial.print(",");
    //Serial.println(pitch);
    
    phase = 3;
  }

  else if (phase == 3 && (wait_time_function(500))) {
    TorqueEnable(0);
    //Serial.print(phase);
    //Serial.print(",");
    //Serial.println(pitch);
    
    phase = 4;
  }

  else if (phase == 4 && (wait_time_function(1500))) {
    ShoulderStandUp(150, 0, 0);
    WaistMotor(0, 50);
    Robot_state = 1;    
    phase = 0;
    standUpProcedure = 0;
    //Serial.print(phase);
    //Serial.print(",");
    //Serial.println(pitch);
    
    digitalWrite(SELECT, HIGH);
  }    
}


bool AutoTurnOff(float roll, float pitch, float AUT0_TURNOFF_ATT, float AUTO_TURNOFF_ATT_PITCH) {
  if (roll > AUT0_TURNOFF_ATT || roll < -AUT0_TURNOFF_ATT || pitch > (AUTO_TURNOFF_ATT_PITCH)  || pitch < -AUTO_TURNOFF_ATT_PITCH) {
    return false;
  } else {
    return true;
  }
}

