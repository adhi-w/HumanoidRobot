#include "AHRS.h"
#include "math.h"
#include "Arduino.h"

/*#define ACCEL_X_MIN ((float) -16384.0)
 #define ACCEL_X_MAX ((float) 16384.0)
 #define ACCEL_Y_MIN ((float) -16384.0)
 #define ACCEL_Y_MAX ((float) 16384.0)
 #define ACCEL_Z_MIN ((float) -16384.0)
 #define ACCEL_Z_MAX ((float) 16384.0)
 
/*#define MAGN_X_MIN ((float) -272)
 #define MAGN_X_MAX ((float) 202)
 #define MAGN_Y_MIN ((float) -94)
 #define MAGN_Y_MAX ((float) 322)
 #define MAGN_Z_MIN ((float) -321)
 #define MAGN_Z_MAX ((float) 58)*/

#define ACCEL_X_MIN ((float) -250)
#define ACCEL_X_MAX ((float) 250)
#define ACCEL_Y_MIN ((float) -250)
#define ACCEL_Y_MAX ((float) 250)
#define ACCEL_Z_MIN ((float) -250)
#define ACCEL_Z_MAX ((float) 250)

/*#define MAGN_X_MIN ((float) -4000)
 #define MAGN_X_MAX ((float) 4000)
 #define MAGN_Y_MIN ((float) -4000)
 #define MAGN_Y_MAX ((float) 4000)
 #define MAGN_Z_MIN ((float) -4000)
 #define MAGN_Z_MAX ((float) 4000)
 
 #define GYRO_AVERAGE_OFFSET_X ((float) -429.7f)
 #define GYRO_AVERAGE_OFFSET_Y ((float) -267.7f)
 #define GYRO_AVERAGE_OFFSET_Z ((float) 27.5f)*/

#define MAGN_X_MIN ((float) 257)//0.0)//-456)
#define MAGN_X_MAX ((float) 953)//1116)//506)
#define MAGN_Y_MIN ((float) -131)//-159)//-488)
#define MAGN_Y_MAX ((float) 800)//579)//516)
#define MAGN_Z_MIN ((float) 571)//-35)//-375)
#define MAGN_Z_MAX ((float) 938)//729)//580)

#define GYRO_AVERAGE_OFFSET_X ((float) -0.87)
#define GYRO_AVERAGE_OFFSET_Y ((float) -28.24)
#define GYRO_AVERAGE_OFFSET_Z ((float) -17.23)

#define ACCEL_X_OFFSET ((ACCEL_X_MIN + ACCEL_X_MAX) / 2.0f)
#define ACCEL_Y_OFFSET ((ACCEL_Y_MIN + ACCEL_Y_MAX) / 2.0f)
#define ACCEL_Z_OFFSET ((ACCEL_Z_MIN + ACCEL_Z_MAX) / 2.0f)
#define ACCEL_X_SCALE (GRAVITY / (ACCEL_X_MAX - ACCEL_X_OFFSET))
#define ACCEL_Y_SCALE (GRAVITY / (ACCEL_Y_MAX - ACCEL_Y_OFFSET))
#define ACCEL_Z_SCALE (GRAVITY / (ACCEL_Z_MAX - ACCEL_Z_OFFSET))

#define MAGN_X_OFFSET ((MAGN_X_MIN + MAGN_X_MAX) / 2.0f)
#define MAGN_Y_OFFSET ((MAGN_Y_MIN + MAGN_Y_MAX) / 2.0f)
#define MAGN_Z_OFFSET ((MAGN_Z_MIN + MAGN_Z_MAX) / 2.0f)
#define MAGN_X_SCALE (100.0f / (MAGN_X_MAX - MAGN_X_OFFSET))
#define MAGN_Y_SCALE (100.0f / (MAGN_Y_MAX - MAGN_Y_OFFSET))
#define MAGN_Z_SCALE (100.0f / (MAGN_Z_MAX - MAGN_Z_OFFSET))

#define GYRO_GAIN 0.00875 // Same gain on all axes
#define GYRO_SCALED_RAD(x) (x * TO_RAD(GYRO_GAIN)) // Calculate the scaled gyro readings in radians per second

// DCM parameters
#define Kp_ROLLPITCH 0.02f
#define Ki_ROLLPITCH 0.00002f
#define Kp_YAW 1.2f
#define Ki_YAW 0.00002f

#define GRAVITY 250.0f // "1G reference" used for DCM filter and accelerometer calibration
#define TO_RAD(x) (x * 0.01745329252)  // *pi/180
#define TO_DEG(x) (x * 57.2957795131)  // *180/pi

AHRS::AHRS()
{
  gyro_num_samples = 0;

  DCM_Matrix[0][0] = 1;
  DCM_Matrix[0][1] = 0;
  DCM_Matrix[0][2] = 0;

  DCM_Matrix[1][0] = 0;
  DCM_Matrix[1][1] = 1;
  DCM_Matrix[1][2] = 0;

  DCM_Matrix[2][0] = 0;
  DCM_Matrix[2][1] = 0;
  DCM_Matrix[2][2] = 1;
  int k = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      Update_Matrix[i][j] = k;
      k++;
    }
  }

  reset_calibration_session_flag = true;
}

void AHRS::Init(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float t) {
  reset_sensor_fusion(ax, ay, az, gx, gy, gz, mx, my, mz, 0.02);
}

float AHRS::Vector_Dot_Product(const float v1[3], const float v2[3])
{
  float result = 0;

  for (int c = 0; c < 3; c++)
  {
    result += v1[c] * v2[c];
  }

  return result;
}

void AHRS::Vector_Cross_Product(float out[3], const float v1[3], const float v2[3])
{
  out[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
  out[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
  out[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}

void AHRS::Vector_Scale(float out[3], const float v[3], float scale)
{
  for (int c = 0; c < 3; c++)
  {
    out[c] = v[c] * scale;
  }
}

void AHRS::Vector_Add(float out[3], const float v1[3], const float v2[3])
{
  for (int c = 0; c < 3; c++)
  {
    out[c] = v1[c] + v2[c];
  }
}

void AHRS::Matrix_Multiply(const float a[3][3], const float b[3][3], float out[3][3])
{
  for (int x = 0; x < 3; x++) // rows
  {
    for (int y = 0; y < 3; y++) // columns
    {
      out[x][y] = a[x][0] * b[0][y] + a[x][1] * b[1][y] + a[x][2] * b[2][y];
    }
  }
}

void AHRS::Matrix_Vector_Multiply(const float a[3][3], const float b[3], float out[3])
{
  for (int x = 0; x < 3; x++)
  {
    out[x] = a[x][0] * b[0] + a[x][1] * b[1] + a[x][2] * b[2];
  }
}

// Init rotation matrix using euler angles
void AHRS::init_rotation_matrix(float m[3][3], float yaw, float pitch, float roll)
{
  float c1 = cos(roll);
  float s1 = sin(roll);
  float c2 = cos(pitch);
  float s2 = sin(pitch);
  float c3 = cos(yaw);
  float s3 = sin(yaw);

  // Euler angles, right-handed, intrinsic, XYZ convention
  // (which means: rotate around body axes Z, Y', X'')
  m[0][0] = c2 * c3;
  m[0][1] = c3 * s1 * s2 - c1 * s3;
  m[0][2] = s1 * s3 + c1 * c3 * s2;

  m[1][0] = c2 * s3;
  m[1][1] = c1 * c3 + s1 * s2 * s3;
  m[1][2] = c1 * s2 * s3 - c3 * s1;

  m[2][0] = -s2;
  m[2][1] = c2 * s1;
  m[2][2] = c1 * c2;
}

void AHRS::Normalize(void)
{
  float error = 0;
  float temporary[3][3];
  float renorm = 0;

  error = -Vector_Dot_Product(&DCM_Matrix[0][0], &DCM_Matrix[1][0]) * .5; //eq.19

  Vector_Scale(&temporary[0][0], &DCM_Matrix[1][0], error); //eq.19
  Vector_Scale(&temporary[1][0], &DCM_Matrix[0][0], error); //eq.19

  Vector_Add(&temporary[0][0], &temporary[0][0], &DCM_Matrix[0][0]);//eq.19
  Vector_Add(&temporary[1][0], &temporary[1][0], &DCM_Matrix[1][0]);//eq.19

  Vector_Cross_Product(&temporary[2][0], &temporary[0][0], &temporary[1][0]); // c= a x b //eq.20

  renorm = .5 * (3 - Vector_Dot_Product(&temporary[0][0], &temporary[0][0])); //eq.21
  Vector_Scale(&DCM_Matrix[0][0], &temporary[0][0], renorm);

  renorm = .5 * (3 - Vector_Dot_Product(&temporary[1][0], &temporary[1][0])); //eq.21
  Vector_Scale(&DCM_Matrix[1][0], &temporary[1][0], renorm);

  renorm = .5 * (3 - Vector_Dot_Product(&temporary[2][0], &temporary[2][0])); //eq.21
  Vector_Scale(&DCM_Matrix[2][0], &temporary[2][0], renorm);
}

void AHRS::Drift_correction(void)
{
  float mag_heading_x;
  float mag_heading_y;
  float errorCourse;

  static float Scaled_Omega_P[3];
  static float Scaled_Omega_I[3];
  float Accel_magnitude;
  float Accel_weight;

  Accel_magnitude = sqrt(Accel_Vector[0] * Accel_Vector[0] + Accel_Vector[1] * Accel_Vector[1] + Accel_Vector[2] * Accel_Vector[2]);
  Accel_magnitude = Accel_magnitude / GRAVITY; // Scale to gravity.

  Accel_weight = constrain(1 - 2 * abs(1 - Accel_magnitude), 0, 1); //

  Vector_Cross_Product(&errorRollPitch[0], &Accel_Vector[0], &DCM_Matrix[2][0]); //adjust the ground of reference
  Vector_Scale(&Omega_P[0], &errorRollPitch[0], Kp_ROLLPITCH * Accel_weight);

  Vector_Scale(&Scaled_Omega_I[0], &errorRollPitch[0], Ki_ROLLPITCH * Accel_weight);
  Vector_Add(Omega_I, Omega_I, Scaled_Omega_I);

  mag_heading_x = cos(MAG_Heading);
  mag_heading_y = sin(MAG_Heading);
  errorCourse = (DCM_Matrix[0][0] * mag_heading_y) - (DCM_Matrix[1][0] * mag_heading_x); //Calculating YAW error
  Vector_Scale(errorYaw, &DCM_Matrix[2][0], errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.

  Vector_Scale(&Scaled_Omega_P[0], &errorYaw[0], Kp_YAW); //.01proportional of YAW.
  Vector_Add(Omega_P, Omega_P, Scaled_Omega_P); //Adding  Proportional.

  Vector_Scale(&Scaled_Omega_I[0], &errorYaw[0], Ki_YAW); //.00001Integrator
  Vector_Add(Omega_I, Omega_I, Scaled_Omega_I); //adding integrator to the Omega_I
}

void AHRS::Matrix_update(void)
{
  Gyro_Vector[0] = GYRO_SCALED_RAD(gyro[0]); //gyro x roll
  Gyro_Vector[1] = GYRO_SCALED_RAD(gyro[1]); //gyro y pitch
  Gyro_Vector[2] = GYRO_SCALED_RAD(gyro[2]); //gyro z yaw

  Accel_Vector[0] = accel[0];
  Accel_Vector[1] = accel[1];
  Accel_Vector[2] = accel[2];

  Vector_Add(&Omega[0], &Gyro_Vector[0], &Omega_I[0]);  //adding proportional term
  Vector_Add(&Omega_Vector[0], &Omega[0], &Omega_P[0]); //adding Integrator term

  Update_Matrix[0][0] = 0;
  Update_Matrix[0][1] = -G_Dt * Omega_Vector[2]; //-z
  Update_Matrix[0][2] = G_Dt * Omega_Vector[1]; //y
  Update_Matrix[1][0] = G_Dt * Omega_Vector[2]; //z
  Update_Matrix[1][1] = 0;
  Update_Matrix[1][2] = -G_Dt * Omega_Vector[0]; //-x
  Update_Matrix[2][0] = -G_Dt * Omega_Vector[1]; //-y
  Update_Matrix[2][1] = G_Dt * Omega_Vector[0]; //x
  Update_Matrix[2][2] = 0;

  Matrix_Multiply(DCM_Matrix, Update_Matrix, Temporary_Matrix); //a*b=c

  for (int x = 0; x < 3; x++) //Matrix Addition (update)
  {
    for (int y = 0; y < 3; y++)
    {
      DCM_Matrix[x][y] += Temporary_Matrix[x][y];
    }
  }
}

void AHRS::Euler_angles(void)
{
  pitch = -asin(DCM_Matrix[2][0]);
  roll = atan2(DCM_Matrix[2][1], DCM_Matrix[2][2]);
  yaw = atan2(DCM_Matrix[1][0], DCM_Matrix[0][0]);
}
void AHRS::Compass_Heading()
{
  float mag_x;
  float mag_y;
  float cos_roll;
  float sin_roll;
  float cos_pitch;
  float sin_pitch;

  cos_roll = cos(roll);
  sin_roll = sin(roll);
  cos_pitch = cos(pitch);
  sin_pitch = sin(pitch);

  mag_x = magnetom[0] * cos_pitch + magnetom[1] * sin_roll * sin_pitch + magnetom[2] * cos_roll * sin_pitch;

  mag_y = magnetom[1] * cos_roll - magnetom[2] * sin_roll;

  MAG_Heading = atan2(-mag_y, mag_x);
}

void AHRS::reset_sensor_fusion(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float t) {
  float temp1[3];
  float temp2[3];
  float xAxis[] = {
    1.0f, 0.0f, 0.0f  };

  accel[0] = ax;
  accel[1] = ay;
  accel[2] = az;

  gyro[0] = gx;
  gyro[1] = gy;
  gyro[2] = gz;

  magnetom[0] = mx;
  magnetom[1] = my;
  magnetom[2] = mz;

  G_Dt = t;

  pitch = -atan2(accel[0], sqrt(accel[1] * accel[1] + accel[2] * accel[2]));

  Vector_Cross_Product(temp1, accel, xAxis);
  Vector_Cross_Product(temp2, xAxis, temp1);
  roll = atan2(temp2[1], temp2[2]);


  Compass_Heading();
  yaw = MAG_Heading;

  init_rotation_matrix(DCM_Matrix, yaw, pitch, roll);
}

void AHRS::compensate_sensor_errors() {
  // Compensate accelerometer error
  accel[0] = (accel[0] - ACCEL_X_OFFSET) * ACCEL_X_SCALE;
  accel[1] = (accel[1] - ACCEL_Y_OFFSET) * ACCEL_Y_SCALE;
  accel[2] = (accel[2] - ACCEL_Z_OFFSET) * ACCEL_Z_SCALE;

  // Compensate magnetometer error
  /*#if CALIBRATION__MAGN_USE_EXTENDED == true
   for (int i = 0; i < 3; i++)
   magnetom_tmp[i] = magnetom[i] - magn_ellipsoid_center[i];
   Matrix_Vector_Multiply(magn_ellipsoid_transform, magnetom_tmp, magnetom);
   #else*/
  magnetom[0] = (magnetom[0] - MAGN_X_OFFSET) * MAGN_X_SCALE;
  magnetom[1] = (magnetom[1] - MAGN_Y_OFFSET) * MAGN_Y_SCALE;
  magnetom[2] = (magnetom[2] - MAGN_Z_OFFSET) * MAGN_Z_SCALE;
  //#endif
  // Compensate gyroscope error
  gyro[0] -= GYRO_AVERAGE_OFFSET_X;
  gyro[1] -= GYRO_AVERAGE_OFFSET_Y;
  gyro[2] -= GYRO_AVERAGE_OFFSET_Z;
}

void AHRS::check_reset_calibration_session()
{

  if (!reset_calibration_session_flag) return;

  for (int i = 0; i < 3; i++) {
    accel_min[i] = accel_max[i] = accel[i];
    magnetom_min[i] = magnetom_max[i] = magnetom[i];
  }

  gyro_num_samples = 0;  // Reset gyro calibration averaging
  gyro_average[0] = gyro_average[1] = gyro_average[2] = 0.0f;

  reset_calibration_session_flag = false;
}

void AHRS::Update(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float t)
{
  accel[0] = (float) ax;
  accel[1] = (float) ay;
  accel[2] = (float) az;

  gyro[0] = (float) gx;
  gyro[1] = (float) gy;
  gyro[2] = (float) gz;

  magnetom[0] = (float) mx;
  magnetom[1] = (float) my;
  magnetom[2] = (float) mz;

  G_Dt = t;
  compensate_sensor_errors();
  Compass_Heading();
  Matrix_update();
  Normalize();
  Drift_correction();
  Euler_angles();
}



