void ArmUp(int speed) {
  byte array[108];

  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (4 + 1) * 8 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x1E;			//Parameter1 Start address to write Data
  array[6] = 0x04;			//Parameter2 Length of Data to write

  array[7] = 15;//3
  array[8] = int((0) / 360.0 * 4096.0) % 256;
  array[9] = int((0) / 360.0 * 4096.0) / 256;
  array[10] = speed % 256;
  array[11] = speed / 256;

  array[12] = 16;
  array[13] = int((360) / 360.0 * 4096.0) % 256;
  array[14] = int((360) / 360.0 * 4096.0) / 256;
  array[15] = speed % 256;
  array[16] = speed / 256;

  array[17] = 17;//3
  array[18] = 580 % 256;
  array[19] = 580 / 256;
  array[20] = speed % 256;
  array[21] = speed / 256;

  array[22] = 18;
  array[23] = 410 % 256;
  array[24] = 410 / 256;
  array[25] = speed % 256;
  array[26] = speed / 256;

  array[27] = 19;//3
  array[28] = 512 % 256;
  array[29] = 512 / 256;
  array[30] = speed % 256;
  array[31] = speed / 256;

  array[32] = 20;
  array[33] = 512 % 256;
  array[34] = 512 / 256;
  array[35] = speed % 256;
  array[36] = speed / 256;

  array[37] = 21;//3
  array[38] = 239 % 256;
  array[39] = 239 / 256;
  array[40] = speed % 256;
  array[41] = speed / 256;

  array[42] = 22;
  array[43] = 733 % 256;
  array[44] = 733 / 256;
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

void FallLeft(int motor_velocity) {
  unsigned char array[108];//20*5+7+1
  unsigned int r[6];
  unsigned int l[6];

  left_foot.motor_degree[0] = (250.0 + L_ANKLE_OFFSET);
  right_foot.motor_degree[0] = (250.0 + R_ANKLE_OFFSET);

  for (int i = 0; i < 6; i++)
  {
    l[i] = round( (left_foot.motor_degree[i] / 360.0 * 4096.0));
    r[i] = round( (right_foot.motor_degree[i] / 360.0 * 4096.0));
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
  array[68] = round((180 / 360.0 * 4096.0)) % 256;
  array[69] = round((180 / 360.0 * 4096.0)) / 256;
  array[70] = 16 % 256;
  array[71] = 16 / 256;

  unsigned char sum = 0x00;
  for (int i = 2; i < 72 ; i++)
  {
    sum += array[i];
  }
  array[72] = ~sum;
  //      PORTD|=0x80;
  Serial1.write(array, 73);
}

void FallRight(int motor_velocity) {
  unsigned char array[108];//20*5+7+1
  unsigned int r[6];
  unsigned int l[6];

  left_foot.motor_degree[0] = (110.0 + L_ANKLE_OFFSET);
  right_foot.motor_degree[0] = (110.0 + R_ANKLE_OFFSET);

  for (int i = 0; i < 6; i++)
  {
    l[i] = round( (left_foot.motor_degree[i] / 360.0 * 4096.0));
    r[i] = round( (right_foot.motor_degree[i] / 360.0 * 4096.0));
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
  array[68] = round((180 / 360.0 * 4096.0)) % 256;
  array[69] = round((180 / 360.0 * 4096.0)) / 256;
  array[70] = 16 % 256;
  array[71] = 16 / 256;

  unsigned char sum = 0x00;
  for (int i = 2; i < 72 ; i++)
  {
    sum += array[i];
  }
  array[72] = ~sum;
  //      PORTD|=0x80;
  Serial1.write(array, 73);

}


