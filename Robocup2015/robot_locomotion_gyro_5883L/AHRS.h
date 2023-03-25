class AHRS
{
public:
  //MPU6050 accelgyro;
  AHRS();
  float accel[3];  // Actually stores the NEGATED acceleration (equals gravity, if board not moving).
  float accel_min[3];
  float accel_max[3];

  float magnetom[3];
  float magnetom_min[3];
  float magnetom_max[3];
  float magnetom_tmp[3];

  float gyro[3];
  float gyro_average[3];
  int gyro_num_samples;
  // DCM variables
  float MAG_Heading;
  float Accel_Vector[3]; // Store the acceleration in a vector
  float Gyro_Vector[3]; // Store the gyros turn rate in a vector
  float Omega_Vector[3]; // Corrected Gyro_Vector data
  float Omega_P[3]; // Omega Proportional correction
  float Omega_I[3]; // Omega Integrator
  float Omega[3];
  float errorRollPitch[3];
  float errorYaw[3];
  float DCM_Matrix[3][3];
  float Update_Matrix[3][3];
  float Temporary_Matrix[3][3];

  // Euler angles
  float yaw;
  float pitch;
  float roll;

  // DCM timing in the main loop
  unsigned long timestamp;
  unsigned long timestamp_old;
  float G_Dt; // Integration time for DCM algorithm

  // More output-state variables
  bool output_stream_on;
  bool output_single_on;
  int curr_calibration_sensor;
  bool reset_calibration_session_flag;
  int num_accel_errors;
  int num_magn_errors ;
  int num_gyro_errors;

  void Compass(void);
  void Normalize(void);
  void Drift_correction(void);
  void Matrix_update(void);
  void Euler_angles(void);
  void Compass_Heading();
  float Vector_Dot_Product(const float v1[3], const float v2[3]);
  void Vector_Cross_Product(float out[3], const float v1[3], const float v2[3]);
  void Vector_Scale(float out[3], const float v[3], float scale);
  void Vector_Add(float out[3], const float v1[3], const float v2[3]);
  void Matrix_Multiply(const float a[3][3], const float b[3][3], float out[3][3]);
  void Matrix_Vector_Multiply(const float a[3][3], const float b[3], float out[3]);
  void init_rotation_matrix(float m[3][3], float yaw, float pitch, float roll);
  void check_reset_calibration_session();
  void compensate_sensor_errors();
  void reset_sensor_fusion(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float t);
  void Update(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float t);
  void Init(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float t);
  float error;
  float temporary[3][3];
  float renorm;
  float mag_heading_x;
  float mag_heading_y;
  float errorCourse;
  //Compensation the Roll, Pitch and Yaw drift.
  static float Scaled_Omega_P[3];
  static float Scaled_Omega_I[3];
  float Accel_magnitude;
  float Accel_weight;
  float mag_x;
  float mag_y;
  float cos_roll;
  float sin_roll;
  float cos_pitch;
  float sin_pitch;
};

