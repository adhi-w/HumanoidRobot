#include <math.h>
//#include "DueTimer.h"
#include "TimerOne.h"
//#include "Timer.h"
#include "SSVR.h"
#include "HLIPM.h"
#include "Parameter.h"
#include "AHRS.h"
#include <Wire.h>
#include "L3G.h"
#include "LSM303.h"
#include "Kalman.h"
#define RAD_TO_DEG 57.32

//------------------------------------------------------------------------------------------------------
#define BUTTON_1 22
#define BUTTON_2 24
#define BUTTON_3 26
#define SELECT   47
//------------------------------------------------------------------------------------------------------
#include <stdio.h>

bool CALIBRATION     = 0;
bool OUTPUT_SENSOR   = 0;
bool TEST 	     = 0;  //0:game mode, receive command by a packet   1:test mode, receive command by one byte;
bool LIPM 	     = 1;  //0:use other method to plan the traj of hip; 1:use LIPM
int  CHANGE_MHEIGHT  = 2;  //0:height of hip plane center is fixed;      1:change the height by circle traj; 2:use HLIPM to change the height
bool CHANGE_HIPPLANE = 1;  //0:Rhip & Lhip stay in the same height(means the hip_plane_theta is always 0);   1:use HLIPM to plan traj of hip_plane_theta
bool SSVR_CONTROLLER = 0;  //0:turn off the function of SSVR controller; 1:turn on the function of SSVR controller
bool GYRO_FEEDBACK   = 1;  //0:turn off the gyro feedback;               1:turn on the gyro feedback

//SENSOR DECLARATION
float ax, ay, az, gx, gy, gz, mx, my, mz;
float magnetom[3], magnetom_min[3], magnetom_max[3];
float roll, pitch, yaw;
int timerCounter, motorEnable;
int standUpProcedure = 0;
bool fallDown = false;
bool bootUp = true;
int8_t phase;
bool squatPosition;
union Float2Byte {
  float f;
  byte b[4];
};

union Int2Byte {
  int i;
  unsigned char b[2];
};

union Float2Int {
  float f;
  int16_t i[2];
};
Int2Byte int2Byte;
Float2Byte float2Byte;
Float2Int float2Int;
AHRS ahrs;
float timestamp, timestamp_old, G_Dt;
float invTime, invTimeOld, dt;
unsigned char vector_temp[3] = {
  0
};

// parameters of LIPM & HLIPM
unsigned int G_Acc = G_ACC_STABLE;	//Gravitational acceleration of LIPM
float f_LIPMZc = LIPMZc;
float TConstant = sqrt(f_LIPMZc / G_Acc);	//Tc=sqrt(Zc/g), paramete of LIPM
float hip_height_amp = HIP_HEIGHT_AMP;	//height of VirtualZc(or hip plane center), parameter of HLIPM
float plane_theta_amp = Plane_Theta_AMP;	//the amplitude of hip_plane_theta trajectory, parameter of HLIPM

// parameters of waist
int waist_pos = 1544;			//the motor position value of waist joint
float waist_tilt = 0.0;			//the tilt angle(degree) of waist

// parameters of system1
unsigned int cycle = CYCLE_STABLE;	//one step is formed with cycle times timer calculating
float GaitHeight = GAITHEIGHT_INIT;   	//the gait height
int accw = ACCW_STABLE;  			//double support phase time type1
int wait_time = WAIT_TIME_STABLE;		//double support phase time type2
int step_num_tol = 32; 			//must be the multiple of "2"
int Step_n = 0;				//record the step number right now
bool stop_flag = 0;			//1:means has reveived the stop command during walking
bool foot_select = SWING_FOOT_FIRST;	//init parameter
int Robot_state = 0;

//parameters of control vector
float length_flag = LENGTH_INIT;		//the target of length vector
float length_odometer = 128;		//the odometer of length (unit: cm)
float length = LENGTH_INIT;		//the length vector
float turn_flag = TURN_INIT;		//the target of turn vector
float turn_odometer = 128;		//the odometer of turn (unit: degree)
float turn = TURN_INIT; 			//the turn vector
float yshift_flag = YSHIFT_INIT;		//the target of yshift vector
float yshift_odometer = 128;   		//the odometer of yshift (unit: cm)
float yshift = YSHIFT_INIT;		//the yshift vector

//other parameters
int KickDir = 0;			//kick degree(from 0~30. right foot will do the left kick, left foot will do the right kick
int KickForce = 30;			//kick
double gyro_init = 0;			//gyro init value
int goandback = 0;

//new robot
static float theta_flag = 0;
bool ShackBig = 0;
bool Accel_Faild = 0;//1:faild
float turn_Yofset_R =  0.18;//0.35;//0.65;//0.57;//0.48;//+0.24;//side shift rotation offset//0.084;//clockwise:+  counterclockwise:-
float turn_Yofset_L =  -0.50;//-0.45;//-0.28;//-0.25;//0.0;//-0.08;//-0.17;
float turn_Xofset_Forward = 0.0;//0.15;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
int Count_button = 0;
int START_BUTTON = 0;
int SET_BUTTON = 0;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int DEMO_STEP = 0;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

limb r_foot;
limb l_foot;
limb r_hip;
limb l_hip;
limb r_knee;
limb l_knee;
limb r_bottom;
limb l_bottom;
limb PositionCOM;

degree left_foot;
degree right_foot;

//==================================================arm======================================================================================
arm_degree right_arm;               //Three degrees of freedom
arm_degree left_arm;
//===========================================================================================================================================

comPosParam comParam = {
  {
    0, 0
  }
  , {
    0, 0
  }
  , {
    0, 0
  }
};

HLIPM m_HLIPM;

int collect_data = 0;
int sensor_l_1 = 0;
int sensor_l_2 = 0;
int sensor_l_3 = 0;
int sensor_l_4 = 0;
int sensor_r_1 = 0;
int sensor_r_2 = 0;
int sensor_r_3 = 0;
int sensor_r_4 = 0;

// gyro & accelerometer
//---------------------------compass------------------------------------------------------------------
LSM303 compass;
L3G gyroscope;
LSM303 accelerometer;
// kalman filter
Kalman kalmanX;
Kalman kalmanY;
// anlge
double gyroXangle, gyroYangle;
double compAngleX, compAngleY;
double kalAngleX, kalAngleY;
double prvAngleX, prvAngleY;
// angular vilocity
double Angular_Vel_X, Angular_Vel_Y;
uint32_t timer;

//FCBGSI
float FCBGSI_L = -1.0;
float FCBGSI_R = -1.0;
float FCBGSI = 0.0;
int T_Global = 0;

int min_x =   -151;
int min_y =   -252;
int min_z =    -14;
int max_x =   +449;
int max_y =   +529;
int max_z =   +13;

float Compass_Array[50] = {
  0
};
int Count_Compass_Stop = 0;
int Count_Compass_Step = 0;
float Heading = 0;
int Send_heading = 0;
//----------------------------------------------------------------------------------------------------

float r_hand = 180.0 - 15;// + 25;
float l_hand = 180.0 + 15;// - 25;
float r_hand_initial = 180.0 - 15;// + 25;
float l_hand_initial = 180.0 + 15;// - 250

LSM303::vector<int16_t> running_min = {
  32767, 32767, 32767
}
, running_max = {
  -32768, -32768, -32768
};
char report[160];
void setup()
{
  //---------------------------button------------------------------------------------------------------
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);
  //---------------------------------------------------------------------------------------------------
  pinMode(SELECT, OUTPUT);
  digitalWrite(SELECT, HIGH);
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);
  digitalWrite(13, HIGH);	//turn on the LED on RickyBoard
  pinMode(37, OUTPUT);	//cal timer,PC0
  //Serial2.begin(115200);
  Serial.begin(115200);
  Serial1.begin(1000000);
  Serial2.begin(1000000);
  Serial.println("start");

  //ArmInit();
  init_parameter();

  //////kalman/////
  Wire.begin();  
  //compass.init();
  //compass.enableDefault();
  /*compass.m_min = (LSM303::vector<int16_t>) {
   min_x, min_y, min_z
   };
   compass.m_max = (LSM303::vector<int16_t>) {
   max_x, max_y, max_z
   };*/

  if (GYRO_FEEDBACK == 1)
  {
    gyro_init = initialGyro();
  }

  Timer1.initialize(TIMER_CYCLETIME);
  Timer1.attachInterrupt(Packet);
}

void PrintIt() {
  Serial.print(yaw + 180.0);
  Serial.print(",");
  Serial.print(pitch);
  Serial.print(",");
  Serial.print(roll);
  Serial.print(",");
  Serial.print(G_Dt);
  Serial.print(",");
  Serial.println(dt);
}

void init_parameter()
{
  int i = 0;
  if (CHANGE_MHEIGHT == 1)
  {
    r_hip.gol[2] = m_HLIPM.CM_c * cos(m_HLIPM.CM_H_theta); //HIP_HEIGHT;
    l_hip.gol[2] = m_HLIPM.CM_c * cos(m_HLIPM.CM_H_theta); //HIP_HEIGHT;
    PositionCOM.gol[2] = m_HLIPM.CM_c * cos(m_HLIPM.CM_H_theta);
  }
  else if (CHANGE_MHEIGHT == 2)
  {
    r_hip.gol[2] = HIP_HEIGHT;
    l_hip.gol[2] = HIP_HEIGHT;
    PositionCOM.gol[2] = HIP_HEIGHT;
  }
  else
  {
    r_hip.gol[2] = HIP_HEIGHT;
    l_hip.gol[2] = HIP_HEIGHT;
    PositionCOM.gol[2] = HIP_HEIGHT;
  }

  r_hip.gol[0] = 0;
  r_hip.gol[1] = GAITWIDTH / 2.0;
  r_foot.gol[0] = 0 + FOOT_POS_OFFSET;
  r_foot.gol[1] = GAITWIDTH / 2.0 + FOOT_SHIFT_DIS ;
  r_foot.gol[2] = SHOE_HEIGHT;
  l_hip.gol[0] = 0;
  l_hip.gol[1] = -GAITWIDTH / 2.0;
  l_foot.gol[0] = 0;
  l_foot.gol[1] = -GAITWIDTH / 2.0 - FOOT_SHIFT_DIS ;
  l_foot.gol[2] = SHOE_HEIGHT;
  PositionCOM.gol[0] = 0;
  PositionCOM.gol[1] = 0;

  r_foot.tmp[0] = 0 + FOOT_POS_OFFSET;
  l_foot.tmp[0] = 0; //moto add it .initialize
  r_foot.tmp[1] = GAITWIDTH / 2.0 + FOOT_SHIFT_DIS ;
  l_foot.tmp[1] = -GAITWIDTH / 2.0 - FOOT_SHIFT_DIS ;
  PositionCOM.tmp[0] = 0;
  PositionCOM.tmp[1] = 0;

  G_Acc = G_ACC_INIT;
  TConstant =  sqrt(f_LIPMZc / G_Acc);	//Tc=sqrt(Zc/g)
  cycle = CYCLE_STABLE;
  GaitHeight = GAITHEIGHT_INIT ;
  accw = ACCW_STABLE;
  wait_time = WAIT_TIME_STABLE;
  Step_n = 0;
  foot_select = SWING_FOOT_FIRST;		//1=lift left foot first,0=lift right foot first

  length_flag = LENGTH_INIT;
  length = LENGTH_INIT;
  turn_flag = TURN_INIT;
  turn = TURN_INIT;
  yshift_flag = YSHIFT_INIT;
  yshift = YSHIFT_INIT;

  for (i = 0; i < 2; i++)
  {
    comParam.startPos[i] = 0;
    comParam.supportPos[i] = 0;
    comParam.endPos[i] = 0;
  }
  m_HLIPM.OnInitialUpdate();

  left_arm.motor_degree[0] = Arm_initial_11; //shoulder_same as elbow180~360 11
  left_arm.motor_degree[1] = Arm_initial_12; //shoulder 12
  left_arm.motor_degree[2] = Arm_initial_13; //elbow180~360 13
  right_arm.motor_degree[0] = Arm_initial_21;
  right_arm.motor_degree[1] = Arm_initial_22;
  right_arm.motor_degree[2] = Arm_initial_23;
}

void loop()
{
  //Serial.println(Robot_state);
  //PrintIt();
  //Serial.println(pitch);
  float2Int.f = yaw + 180.0f;
  if ( Robot_state == 1){	//micro squat

    if(wait_time_function(2000))
      fallDown = false;
  }

  unsigned char command_in = 1;	//can't init to 0(because when command_in=0, it's vector control)

  if(fallDown == false)
    command_in = packet_reception(vector_temp);
  RazorRead();

  motorEnable = AutoTurnOff(roll, pitch, SAFE_FALL_ANGLE, SAFE_FALL_ANGLE_PITCH);

  if (motorEnable == false) {    
    fallDown = true;
    if (standUpProcedure == 0) {
      MotorTorqueEnable(0);
      //Serial.print("turnOff");
      //Serial.print(",");
      //Serial.println(fallDown);
      standUpProcedure = 1;
    }

    if(pitch > SAFE_FALL_ANGLE_PITCH)
      Robot_state = 150;
    else if (pitch < -SAFE_FALL_ANGLE_PITCH)
      Robot_state = 200;
    else if (roll < -SAFE_FALL_ANGLE || roll > SAFE_FALL_ANGLE){
      if(phase == 0){
        Robot_state = 0;
        if(standUpProcedure == 1){
          standUpProcedure = 0;
        }
      }
    }

    //MotorTorqueEnable(0);
    //ProtectHead(-180, 180, 1000);
    //Robot_state = 0;

    //digitalWrite(16, LOW);
  }

  /*Serial2.print(command_in);
   Serial2.print(",");
   Serial2.println(pitch);
   Serial2.print(" ");
   Serial2.print(yaw);
   Serial2.println(" ");
   Serial.print(motorEnable);
   Serial.print(" ");
   Serial.println(Robot_state);*/

  static bool previous_button_1 = 0;
  bool temp_button_1 = 0;			//no button is pressed 0,There is a button 1
  temp_button_1 = digitalRead(BUTTON_1);
  if (temp_button_1 == 0 && previous_button_1 == 1)
  {
    command_in = '0';//broken
  }
  previous_button_1 = temp_button_1;

  static bool previous_button_2 = 0;
  bool temp_button_2 = 0;			//no button is pressed 0,There is a button 1
  temp_button_2 = digitalRead(BUTTON_2);
  if (temp_button_2 == 0 && previous_button_2 == 1)
  {
    command_in = 'r';
    Count_button = Count_button + 1;
  }
  previous_button_2 = temp_button_2;

  if (Count_button == 1) SET_BUTTON = 1;
  //if(Count_button == 2) START_BUTTON = 1;

  static bool previous_button_3 = 0;
  bool temp_button_3 = 0;			//0 if no button is pressed,There is a button 1
  temp_button_3 = digitalRead(BUTTON_3);
  if (temp_button_3 == 0 && previous_button_3 == 1)
  {
    START_BUTTON = 1;
  }
  previous_button_3 = temp_button_3;

  //        Serial.print("START_BUTTON ");
  //        Serial.print(START_BUTTON);
  //        Serial.print("\tSET_BUTTON ");
  //        Serial.println(SET_BUTTON);

  //////////////////////////define Robot_state//////////////////////////
  //Robot_state=0 && Step_n=0 soft
  //Robot_state=1 && Step_n=step_num_tol+1 micro squat
  //Robot_state=2 moving
  //Robot_state=3 restart wait Stop halfway and wait 1 second
  //Robot_state=4 do nothing
  //Robot_state=5 ask temperature
  //Robot_state=7  Small Left kick
  //Robot_state=8  Small Right kick
  //Robot_state=9  Left kick
  //Robot_state=10 Right kick
  //Robot_state=11 Big Left kick
  //Robot_state=12 Big Right kick

  //Robot_state=15 (squat with feet spread)
  //Robot_state=44 Temporary state while crouching
  //Robot_state=20 Squst_pre(feet open)
  //Robot_state=21 Squst(0)squat down (feet open)
  //Robot_state=22 Squst(1)stand up (legs spread)
  //Robot_state=23 Squst_pre(foot recovery)

  if ( command_in == '0' && Robot_state == 2 && Step_n < step_num_tol - 2) stop_flag = 1; //48
  //====================================================================================================================================================
  else if ( command_in == 'r') Gait_BigRobot_Arm(10); //Arm Set up
  //=========================================================================================================================================
  else if ( command_in == 'y' && Robot_state == 0) //straight leg
  {
    right_foot.motor_degree[1] = (180.0 + R_DW_KNEE_OFFSET);	    //pitch	Below the knee + X, one plus and one minus makes this foot stretch, making it harder to lift this foot
    right_foot.motor_degree[2] = (180.0 + R_UP_KNEE_OFFSET);	    //pitch	knee-X
    right_foot.motor_degree[0] = (180.0 + R_ANKLE_OFFSET  );	    //roll	ankle
    right_foot.motor_degree[3] = (180.0 + R_HIP_OFFSET    );		//roll	the hip
    right_foot.motor_degree[4] = (180.0 + R_YAW_OFFSET    );	//yaw	turning joint

    left_foot.motor_degree[1] = (180.0 + L_DW_KNEE_OFFSET);	    //pitch	Below the knee + X, one plus and one minus makes this foot stretch, making it harder to lift this foot
    left_foot.motor_degree[2] = (180.0 + L_UP_KNEE_OFFSET);	    //pitch	knee-X
    left_foot.motor_degree[0] = (180.0 + L_ANKLE_OFFSET  );	    //roll	ankle
    left_foot.motor_degree[3] = (180.0 + L_HIP_OFFSET    );	    //roll	the hip
    left_foot.motor_degree[4] = (180.0 + L_YAW_OFFSET    );	    //yaw	turning joint
    Gait_BigRobot(0, 100);
  }
  //==================================================================================================================================================////==/
  //        else if( command_in=='h') //Move Arm
  //	{
  //	  left_arm.motor_degree[0]=left_arm.motor_degree[0]  +L_Shoulder_1;//shoulder_same as elbow180~360 11
  //          left_arm.motor_degree[1]=left_arm.motor_degree[1]  +L_Shoulder_2;//shoulder 12
  //          left_arm.motor_degree[2]=left_arm.motor_degree[2]  +L_Elbow;//elbow180~360 13

  //          right_arm.motor_degree[0]=right_arm.motor_degree[0] + R_Shoulder_1;
  //          right_arm.motor_degree[1]=right_arm.motor_degree[1] + R_Shoulder_2;
  //          right_arm.motor_degree[2]=right_arm.motor_degree[2] + R_Elbow;
  //	}

  //=============================Squat====================================================================================================================
  //	else if(command_in=='e' && Robot_state==1 )
  //	{
  //  		Robot_state = 20;//(feet open)
  //	}
  else if ( command_in == 'b' && Robot_state == 44 ) Robot_state = 21; //squat down
  else if ( command_in == 'v' && Robot_state == 44 ) Robot_state = 22; //stand up
  //=================================================================================================================================================
  else if ( command_in == '0' && (Robot_state == 0)) {
    //Robot_state = 1; //hard up 48
    if(bootUp == true){
      r_hand += 10;
      l_hand -= 10;
      Gait_BigRobot_Arm(1023);
      bootUp = false;      
    }
    Robot_state = 1;
  }
  else if ( command_in == '1' && Robot_state == 1 ) Robot_state = 8; //49 small right_kick
  else if ( command_in == '2' && Robot_state == 1 ) Robot_state = 7; //50 small left_kick
  else if ( command_in == '6' && Robot_state == 1 ) Robot_state = 9;
  else if ( command_in == '7' && Robot_state == 1 ) Robot_state = 10;
  else if ( command_in == '3' && Robot_state == 1 ) Robot_state = 151;
  else if ( command_in == '4' && Robot_state == 1)  Robot_state = 152;
  else if ( command_in == '5' && Robot_state == 1)  Robot_state = 23;
  //	else if( command_in=='3' && Robot_state==1 ) Robot_state=10;//51 right_kick
  //	else if( command_in=='4' && Robot_state==1 ) Robot_state=9; //52 left_kick
  //	else if( command_in=='5' && Robot_state==1 ) Robot_state=12;//53 big right_kick
  //	else if( command_in=='6' && Robot_state==1 ) Robot_state=11;//54 big left_kick
  //	else if( command_in=='7' && Robot_state==1 ) Robot_state=14;//55 Right High kick
  //	else if( command_in=='8' && Robot_state==1 ) Robot_state=13;//56 Left High kick
  else if ( command_in == '9' && Robot_state == 1 ) //57 (squat with feet spread)
  {
    stop_flag = 1;
    length_flag = 0;
    yshift_flag = 7.0;
    turn_flag = 25.0;
    Robot_state = 15 ;
    Step_n = 0;
  }

  else if ( command_in == '-' && Robot_state == 0 ) Robot_state = 5; //read temperature
  else if (command_in == ']' && (Robot_state == 1 || Robot_state == 2) ) //93
  {
    length_flag = LENGTH_FORWARD_MAX; //14
    yshift_flag = YSHIFT_INIT;
    turn_flag = TURN_INIT + turn_Xofset_Forward * LENGTH_FORWARD_MAX;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == '[' && (Robot_state == 1 || Robot_state == 2) ) //91
  {
    length_flag = 8.0;
    yshift_flag = YSHIFT_INIT;
    turn_flag = TURN_INIT;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'p' && (Robot_state == 1 || Robot_state == 2) ) //112
  {
    length_flag = 6.0;
    yshift_flag = 0;
    turn_flag = 0;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'o' && (Robot_state == 1 || Robot_state == 2) ) //111
  {
    length_flag = 4.0;
    yshift_flag = 0;
    turn_flag = 0;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'u' && (Robot_state == 1 || Robot_state == 2) ) //117
  {
    length_flag = LENGTH_BACK_MAX;
    yshift_flag = YSHIFT_INIT;
    turn_flag = TURN_INIT;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'i' && (Robot_state == 1 || Robot_state == 2) ) //105
  {
    length_flag = LENGTH_INIT;
    turn_flag = TURN_INIT;
    yshift_flag = YSHIFT_INIT;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }

  else if (command_in == 'm'  && (Robot_state == 1 || Robot_state == 2) ) //109
  {
    length_flag = LENGTH_INIT;
    yshift_flag = YSHIFT_MAX; //12.0
    turn_flag = TURN_INIT + turn_Yofset_R * YSHIFT_MAX;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'n' && (Robot_state == 1 || Robot_state == 2) ) //110
  {
    length_flag = LENGTH_INIT;
    yshift_flag = -YSHIFT_MAX; //12.0
    turn_flag = TURN_INIT + turn_Yofset_L * YSHIFT_MAX;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'l'  && (Robot_state == 1 || Robot_state == 2) )
  {
    length_flag = 0;
    yshift_flag = 0;
    turn_flag = TURN_MAX;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'k' && (Robot_state == 1 || Robot_state == 2) )
  {
    length_flag = 0;
    yshift_flag = 0;
    turn_flag = -TURN_MAX;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }

  ///////////////////////
  else if (command_in == 'w' && (Robot_state == 1 || Robot_state == 2) ) //119
  {
    length_flag = length_flag + 1;
    yshift_flag = yshift_flag;
    turn_flag = turn_flag;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 's' && (Robot_state == 1 || Robot_state == 2) ) //115
  {
    length_flag = length_flag - 1;
    yshift_flag = yshift_flag;
    turn_flag = turn_flag;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'a' && (Robot_state == 1 || Robot_state == 2) ) //97
  {
    length_flag = length_flag;
    yshift_flag = yshift_flag - 1;
    turn_flag = turn_flag + turn_Yofset_L;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'd' && (Robot_state == 1 || Robot_state == 2) ) //100
  {
    length_flag = length_flag;
    yshift_flag = yshift_flag + 1;
    turn_flag = turn_flag + turn_Yofset_R;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'z' && (Robot_state == 1 || Robot_state == 2) ) //122
  {
    length_flag = length_flag;
    yshift_flag = yshift_flag;
    turn_flag = turn_flag - 1;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  else if (command_in == 'c' && (Robot_state == 1 || Robot_state == 2) ) //99
  {
    length_flag = length_flag;
    yshift_flag = yshift_flag;
    turn_flag = turn_flag + 1;
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }
  ///////////////////////

  else if (command_in == 0 && (Robot_state == 1 || Robot_state == 2)) //vector control
  {
    int x_vector = vector_temp[0];	//unsigned char to int
    int y_vector = vector_temp[1];
    int theta_vector = vector_temp[2];

    if (x_vector - 128 >= 0)
      length_flag = LENGTH_INIT + round( float(x_vector - 128) / 100.0 * LENGTH_FORWARD_MAX );	//x
    else if (x_vector - 128 < 0)
      length_flag = LENGTH_INIT + round( float(x_vector - 128) / 100.0 * (-LENGTH_BACK_MAX) );	//x
    else
      length_flag = 0;

    yshift_flag = YSHIFT_INIT + round( float(y_vector - 128) / 100.0 * YSHIFT_MAX );			//y
    turn_flag = TURN_INIT + round( float(theta_vector - 128) / 100.0 *  TURN_MAX);	        	//theta

    /*side shift compensation*/
    if (yshift_flag < YSHIFT_INIT) //Move left
      turn_flag = turn_flag + (turn_Yofset_L * fabs(yshift_flag)); //compensate
    else//Move to the right
    turn_flag = turn_flag + (turn_Yofset_R * fabs(yshift_flag)); //compensate
    /*forward compensation*/
    if (length_flag > LENGTH_INIT)
      turn_flag = turn_flag + (turn_Xofset_Forward * fabs(length_flag)); //compensate
    if (Robot_state == 1)
    {
      Robot_state = 2;
      Step_n = 0;
    }
  }

  if (Robot_state != 15)
  {
    if (length_flag > LENGTH_FORWARD_MAX)    length_flag = LENGTH_FORWARD_MAX;
    else if (length_flag < LENGTH_BACK_MAX)  length_flag = LENGTH_BACK_MAX;
    if (yshift_flag > YSHIFT_MAX)            yshift_flag = YSHIFT_MAX;
    else if (yshift_flag < -YSHIFT_MAX)      yshift_flag = -YSHIFT_MAX;
    if (turn_flag > TURN_MAX)                turn_flag = TURN_MAX;
    else if (turn_flag < -TURN_MAX)          turn_flag = -TURN_MAX;
  }
  /*if (Accel_Faild == 0)
   {
   ///kalman///
   // Calculate delta time
   double dt = (double)(micros() - timer) / 1000000;
   timer = micros();
   double roll, pitch;
   Get_Roll_Pitch_Accelerometer(&roll, &pitch);
   double gyroXrate, gyroYrate;
   Get_Gyro_Rate(&gyroXrate, &gyroYrate);
   // Calculate the angle using a Kalman filter
   kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt);
   kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
   // Calculate gyro angle without any filter
   gyroXangle += kalmanX.getRate() * dt;
   gyroYangle += kalmanY.getRate() * dt;
   // Calculate the angle using a Complimentary filter
   compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll;
   compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;
   // Gyro angular velocity
   Angular_Vel_X = gyroXrate;
   Angular_Vel_Y = gyroYrate;
   
   Angular_Vel_X = (kalAngleX - prvAngleX) / dt;
   Angular_Vel_Y = (kalAngleY - prvAngleY) / dt;
   prvAngleX = kalAngleX;
   prvAngleY = kalAngleY;
   ///kalman///
   //---------------------------------------------------------compass------------------------------------------------------------------
   compass.read();
   if (abs(FCBGSI) > 17.5) //19.0
   ShackBig = 1;
   if (ShackBig == 1 && Step_n < step_num_tol - 2)
   {
   stop_flag = 1;
   ShackBig = 0;
   }
   }
   Heading = compass.heading();
   Send_heading = Heading;
   //Serial.println(Send_heading);
   //        float heading = compass.heading();
   //        Serial.println(FCBGSI);
   //        Serial.print("\t");
   //        Serial.println(Angular_Vel_Y);
   //---------------------------------------------------------compass------------------------------------------------------------------;
   */
}

unsigned char packet_reception(unsigned char *vector)
{
  unsigned char command_type = 1;		//can't init to 0(because when command_in=0, it's vector control)
  static unsigned char data[120] = {
    0
  };
  static unsigned int data_number = 0;

  if (TEST == 1)
  {
    byte data[2];
    if (Serial.available() > 0)
    {
      command_type = Serial.read();
      if (command_type == '#') {
        char received = Serial.read();
        if (received == 's') {
          data[0] = Serial.read();
          data[1] = Serial.read();

          Serial.print("#SYNCH");
          Serial.write(data, 2);
          Serial.println();
        }
      }
      Serial.flush();
    }
  }
  else
  {
    for (int i = data_number; i < (data_number + Serial.available()); i++)
    {
      data[i] = Serial.read();
      data_number++;
      if (i >= 119)break;
    }
    if (data_number > 6 )
    {
      int max_buffer = data_number - 1;
      if (max_buffer > 119)  max_buffer = 119;
      for (int i = max_buffer; i >= 6; i--)
      {
        if (data[i] == 255 && data[i - 6] == 254)
        {
          command_type = data[i - 5];
          if (command_type == 49 || command_type == 50)
          {
            if (Robot_state == 1)
            {
              KickDir = data[i - 4]; //kick_dir = 1:Left 5 degrees to kick the ball	kick_dir = 2:Right 5 Degree Kick
              KickForce = data[i - 3];
              if ( KickDir < 0 )   KickDir = 0;
              else if ( KickDir > KICK_DIR_MAX )     KickDir = KICK_DIR_MAX;
              if ( KickForce > 40) KickForce = 40;
              else if ( KickForce < KICK_FORCE_MAX ) KickForce = KICK_FORCE_MAX;
            }
          }
          else
          {
            if (data[i - 4] == 128)
            {
              length_odometer = 128;
              yshift_odometer = 128;
              turn_odometer = 128;
            }
          }
          vector[0] = data[i - 3];		//x
          vector[1] = data[i - 2];		//y
          vector[2] = data[i - 1];	    //theta
          //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
          if (START_BUTTON == 1)
          {
            Count_button = 0;
            START_BUTTON = 0;
          }
          //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
          data_number = 0;
          break;
        }
      }
    }
    if (data_number >= 120)	data_number = 0;
  }
  return command_type;
}

float rotation1(bool  R_or_L, float t, float Ts, float oriented, bool swing_foot)
{
  static float right_hip_yaw = 0;
  static float left_hip_yaw = 0;
  float theta_0 = 0;
  float rotate_time = 0.18;		// (1-2*rotate_time)time to turn the yaw joint
  float rotate_unit = abs(turn * 0.5) / ((1 - 2 * rotate_time) * cycle);
  if ( t / Ts > rotate_time && t / Ts < 1 - rotate_time)
  {
    if (oriented > 0)		//turn right
    {
      right_hip_yaw = 0;
      if (swing_foot == 1)	left_hip_yaw = left_hip_yaw - rotate_unit; //for left foot raise
      else if (swing_foot == 0)	//for right foot raise
      {
        if (left_hip_yaw < 0)  left_hip_yaw = left_hip_yaw + rotate_unit;
        else  left_hip_yaw = 0;
      }
    }
    else if (oriented < 0)		//turn left
    {
      left_hip_yaw = 0;
      if (swing_foot == 1)	//for left foot raise
      {
        if (right_hip_yaw > 0) right_hip_yaw = right_hip_yaw - rotate_unit;
        else  left_hip_yaw = 0;
      }
      else if (swing_foot == 0)	right_hip_yaw = right_hip_yaw + rotate_unit; //for right foot raise
    }
    else if (oriented == 0)
    {
      if (swing_foot == 1)	//left foot raise
      {
        if (R_or_L == 1)	//left foot
        {
          if (left_hip_yaw > 0)  left_hip_yaw = left_hip_yaw - rotate_unit;
          else if (left_hip_yaw < 0) left_hip_yaw = left_hip_yaw + rotate_unit;
        }
        else right_hip_yaw = right_hip_yaw;
      }
      else if (swing_foot == 0)	//right foot raise
      {
        if (R_or_L == 1)
        {
          if (right_hip_yaw > 0)	right_hip_yaw = right_hip_yaw - rotate_unit;
          else if (right_hip_yaw < 0)	right_hip_yaw = right_hip_yaw + rotate_unit;
        }
        else	left_hip_yaw = left_hip_yaw;
      }
    }
  }
  if (R_or_L == 1)  theta_0 = left_hip_yaw;
  else 	theta_0 = right_hip_yaw;
  return theta_0;
}

void inv_kine_Chou(float hip_plane_theta, float delta_theta1_turn, float *hip, float *bottom, bool R_or_L, float *m_degree)
{ //R_or_L:means which foot we want to calculate
  float theta[4];
  int i = 0;
  float L1 = 0, L2 = 0, alpha = 0, beta = 0, g1 = 0, g2 = 0, h1 = 0, h2 = 0, cc = 0, theta2_Plus_theta1 = 0, temp = 0;
  float m_cur_hip_height = hip[2];
  for (i = 0; i < 4; i++)
    theta[i] = 0;

  float a1 = SHOE_HEIGHT;
  float a2 = FOOT_GAP;
  float a3 = LEG;
  float a4 = KNEE_GAP;
  float a5 = THIGH;
  float a6 = HIP_GAP;
  float a7 = HIP_UP_GAP;


  float theta2_turn = 0;

  theta2_turn = delta_theta1_turn;// *(65.0 /35.0);
  L1 = hip[1] - bottom[1];
  temp = (L1) / (m_cur_hip_height - a7 - a1);
  if (temp >= 1) temp = 1;
  if (temp <= -1) temp = -1;
  theta[0] = -atan(temp);

  L2 = sqrt( (m_cur_hip_height - a7 - a1) * (m_cur_hip_height - a7 - a1) + L1 * L1);
  alpha = L2  - a6 - a4 - a2 - bottom[2];
  beta = (bottom[0] - hip[0]);
  temp = (alpha * alpha + beta * beta - a3 * a3 - a5 * a5) / (2.0f * a5 * a3);
  if (temp >= 1) temp = 1;
  if (temp <= -1) temp = -1;
  theta2_Plus_theta1 = fabs(acos(temp));

  g1 = a3 + a5 * cos(theta2_Plus_theta1);
  g2 = -a5 * sin(theta2_Plus_theta1);
  cc = sqrt(g1 * g1 + g2 * g2);
  temp = -beta / cc;

  if (fabs(temp) <= 1)
  {
    h1 = asin(temp);
    temp = g2 / cc;
    if (fabs(temp) <= 1)
    {
      h2 = asin(temp);
      theta[1] = h1 - h2;
      theta[2] = theta2_Plus_theta1 - theta[1];
    }
  }

  if (R_or_L == 0)		//right foot
  {
    r_knee.gol[0] = r_hip.gol[0] + a3 * sin(theta[2]);
    r_knee.gol[1] = 0.5 * (r_hip.gol[1] + r_foot.gol[1]);
    r_knee.gol[2] = 0.5 * (r_hip.gol[2] + r_foot.gol[2]);
  }
  else
  {
    l_knee.gol[0] = l_hip.gol[0] + a3 * sin(theta[2]);
    l_knee.gol[1] = 0.5 * (l_hip.gol[1] + l_foot.gol[1]);
    l_knee.gol[2] = 0.5 * (l_hip.gol[2] + l_foot.gol[2]);
  }

  theta[3] = theta[0] - hip_plane_theta;
  theta[3] = theta[3] * 180.0f / PI * 48 / 22; // *(100.0/32.0) *(-1);
  theta[0] = theta[0] * 180.0f / PI * -48 / 22; // *(100.0/32.0) *(-1);
  theta[1] = theta[1] * 180.0f / PI;// *(48.0 /22.0);
  theta[2] = theta[2] * 180.0f / PI;// *(48.0 /22.0);

  //Serial.print(theta[2]);
  //Serial.println();

  if (R_or_L == 0)		//right foot
  {
    m_degree[2] = (180.0 + R_DW_KNEE_OFFSET) + theta[1];	   //pitch	
    m_degree[3] = (180.0 + R_UP_KNEE_OFFSET) - theta[2];	   //pitch	
    m_degree[0] = (180.0 + R_ANKLE_OFFSET  ) - theta[0];	   //roll	
    m_degree[4] = (180.0 + R_HIP_OFFSET    ) + theta[3];		//roll	
    m_degree[5] = (180.0 + R_YAW_OFFSET    ) + theta2_turn;	//yaw	
    m_degree[1] = 180;
  }
  else if (R_or_L == 1)	//left foot
  {
    m_degree[2] = (180.0 + L_DW_KNEE_OFFSET) - theta[1];	   //pitch	
    m_degree[3] = (180.0 + L_UP_KNEE_OFFSET) + theta[2];	   //pitch	
    m_degree[0] = (180.0 + L_ANKLE_OFFSET  ) - theta[0];	   //roll	
    m_degree[4] = (180.0 + L_HIP_OFFSET    ) + theta[3];	   //roll	
    m_degree[5] = (180.0 + L_YAW_OFFSET    ) + theta2_turn;	//yaw
    m_degree[1] = 157;
  }
}

void inv_kine_Chou_backUp(float hip_plane_theta, float delta_theta1_turn, float *hip, float *bottom, bool R_or_L, float *m_degree)
{ //R_or_L:means which foot we want to calculate

    int i = 0;
  float L1 = 0, L2 = 0, alpha = 0, beta = 0, g1 = 0, g2 = 0, h1 = 0, h2 = 0, cc = 0, theta2_Plus_theta1 = 0, temp = 0;
  float m_cur_hip_height = hip[2];
  float theta[4];
  for (i = 0; i < 4; i++)  theta[i] = 0;
  float a1 = SHOE_HEIGHT;
  float a2 = FOOT_GAP;
  float a3 = LEG;
  float a4 = KNEE_GAP;
  float a5 = THIGH;
  float a6 = HIP_GAP;
  float a7 = HIP_UP_GAP;


  float theta2_turn = 0;

  theta2_turn = delta_theta1_turn * (65.0 / 35.0);
  L1 = hip[1] - bottom[1];
  temp = (L1) / (m_cur_hip_height - a7 - a1);
  if (temp >= 1) temp = 1;
  if (temp <= -1) temp = -1;
  theta[0] = -atan(temp);

  L2 = sqrt( (m_cur_hip_height - a7 - a1) * (m_cur_hip_height - a7 - a1) + L1 * L1);
  alpha = L2  - a6 - a4 - a2 - bottom[2];
  beta = (bottom[0] - hip[0]);
  temp = (alpha * alpha + beta * beta - a3 * a3 - a5 * a5) / (2.0f * a5 * a3);
  if (temp >= 1) temp = 1;
  if (temp <= -1) temp = -1;
  theta2_Plus_theta1 = fabs(acos(temp));

  g1 = a3 + a5 * cos(theta2_Plus_theta1);
  g2 = -a5 * sin(theta2_Plus_theta1);
  cc = sqrt(g1 * g1 + g2 * g2);
  temp = -beta / cc;

  if (fabs(temp) <= 1)
  {
    h1 = asin(temp);
    temp = g2 / cc;
    if (fabs(temp) <= 1)
    {
      h2 = asin(temp);
      theta[1] = h1 - h2;
      theta[2] = theta2_Plus_theta1 - theta[1];
    }
  }


  if (R_or_L == 0)		//right foot
  {
    r_knee.gol[0] = r_hip.gol[0] + a3 * sin(theta[2]);
    r_knee.gol[1] = 0.5 * (r_hip.gol[1] + r_foot.gol[1]);
    r_knee.gol[2] = 0.5 * (r_hip.gol[2] + r_foot.gol[2]);
  }
  else
  {
    l_knee.gol[0] = l_hip.gol[0] + a3 * sin(theta[2]);
    l_knee.gol[1] = 0.5 * (l_hip.gol[1] + l_foot.gol[1]);
    l_knee.gol[2] = 0.5 * (l_hip.gol[2] + l_foot.gol[2]);
  }

  theta[3] = theta[0] - hip_plane_theta;
  theta[3] = theta[3] * 180.0f / PI * (100.0 / 32.0) * (-1);
  theta[0] = theta[0] * 180.0f / PI * (100.0 / 32.0) * (-1);
  theta[1] = theta[1] * 180.0f / PI * (48.0 / 22.0);
  theta[2] = theta[2] * 180.0f / PI * (48.0 / 22.0);

  if (R_or_L == 0)		//right foot
  {
    m_degree[1] = (180.0 + R_DW_KNEE_OFFSET) - theta[1];	   //pitch	
    m_degree[2] = (180.0 + R_UP_KNEE_OFFSET) + theta[2];	   //pitch	
    m_degree[0] = (180.0 + R_ANKLE_OFFSET  ) + theta[0];	   //roll	
    m_degree[3] = (180.0 + R_HIP_OFFSET    ) + theta[3];		//roll	
    m_degree[4] = (180.0 + R_YAW_OFFSET    ) + theta2_turn;	//yaw	
  }
  else if (R_or_L == 1)	//left foot
  {
    m_degree[1] = (180.0 + L_DW_KNEE_OFFSET) + theta[1];	   //pitch	
    m_degree[2] = (180.0 + L_UP_KNEE_OFFSET) - theta[2];	   //pitch	
    m_degree[0] = (180.0 + L_ANKLE_OFFSET  ) + theta[0];	   //roll	
    m_degree[3] = (180.0 + L_HIP_OFFSET    ) + theta[3];	   //roll	
    m_degree[4] = (180.0 + L_YAW_OFFSET    ) + theta2_turn;	//yaw	
  }
}

float inv_waist(float tilt_ang)	//tilt_ang:-90~90
{
  float L1 = 20.435;
  float L2 = 4.826;
  float L3 = 4.2;
  float L4 = 16;
  float P2[2] = {
    3.85 , 2.91
  };
  float middle_theta1 = 0.0857;	//Theta1 diameter when the chest is raised
  float middle_phi2 = 0.955;	

  float P3[2], P4[2];
  float L5 = 0;
  float theta1 = 0, theta2 = 0, theta3 = 0, phi1 = 0, phi2 = 0;
  float waist_motor = 0;
  //=============initial value=============

  theta1 = middle_theta1 + (tilt_ang / 180.0 * PI);
  P3[0] = L1 * sin(theta1);
  P3[1] = L1 * cos(theta1);
  L5 = sqrt( sq(P3[0] - P2[0]) + sq(P3[1] - P2[1]) );
  theta2 = acos( (L1 * L1 + L5 * L5 - L2 * L2) / (2 * L1 * L5) );
  theta3 = acos( (L4 * L4 + L5 * L5 - L3 * L3) / (2 * L4 * L5) );
  phi1 = theta2 + theta3 - theta1;
  P4[0] = P3[0] + L4 * sin(phi1);
  P4[1] = P3[1] - L4 * cos(phi1);
  phi2 = atan2( (P4[0] - P2[0]), (P4[1] - P2[1]) );

  waist_motor = 1544 - round( (middle_phi2 - phi2) / PI * 180.0 / 360.0 * 4096.0);
  return waist_motor;
}

void SSVRController(float max_swing , float step_length)
{
  float predict_height = 0;
  float predict_theta = 0;
  float predict_tilt = 0;
  SSVR ssvr1;

  ssvr1.row_data[0] = 0;									//# 1, start time
  ssvr1.row_data[1] = 0; 									//# 2, end time
  ssvr1.row_data[2] = 2.0 * cycle * (TIMER_CYCLETIME / 1000000.0) ;				//# 3, step_cycle_time,				unit:s
  ssvr1.row_data[3] = (GaitHeight) + 1.5;						      //# 4, gait_height,				unit:cm
  ssvr1.row_data[4] = 2.0 * (wait_time + accw) * (TIMER_CYCLETIME / 1000000.0) ;		//# 5, double_support phase in one step_cycle	unit:s
  ssvr1.row_data[5] = HIP_HEIGHT + 1 - HIP_UP_GAP; //(HIP_HEIGHT + (hip_height_amp/2.0));	//# 6, hip_moving_avr_height			unit:cm
  ssvr1.row_data[6] = hip_height_amp;							//# 7, hip_moving_change_height(2 times amp)	unit:cm
  ssvr1.row_data[7] = 2.0 * plane_theta_amp ;						//# 8, hip_moving_change_theta(2 times amp)	unit:degree
  ssvr1.row_data[8] = 2.0 * abs(max_swing);						//# 9, yshift amp(2 times amp)			unit:cm
  ssvr1.row_data[9] = FOOT_TRAJ_PERCENTHIGH;       					//#10, percent_hight				unit:%
  ssvr1.row_data[10] = step_length;							//#11, x length of one step			unit:cm
  ssvr1.row_data[11] = 20.0;								//#12, hip wide(in y axis)			unit:cm
  ssvr1.row_data[12] = (THIGH + LEG + SHOE_HEIGHT + FOOT_GAP + HIP_GAP + HIP_UP_GAP + KNEE_GAP) - HIP_UP_GAP; //#13, hip height(in z axis)		unit:cm
  ssvr1.row_data[13] = 0;									//#14, tilt forward angle			unit:degree
  ssvr1.row_data[14] = 146.0;								//#15, height					unit:cm
  ssvr1.row_data[15] = 15.0;								//#16, weight					unit:kg
  ssvr1.row_data[16] = 59;								//#17, COM height				unit:cm

  ssvr1.normalization(ssvr1.row_data , ssvr1.normal_row_data);
  predict_height = ssvr1.predictSSVR( 7, ssvr1.normal_row_data);		//hip_height_amp
  predict_theta = ssvr1.predictSSVR( 8, ssvr1.normal_row_data);		//plane_theta_amp
  predict_tilt  = ssvr1.predictSSVR(14, ssvr1.normal_row_data);		//tilt forward angle

  hip_height_amp = predict_height;
  plane_theta_amp = predict_theta / 2.0;
  waist_tilt = predict_tilt;
  //Serial.println(WAIST_TILT);
  if (hip_height_amp < 0.0)
    hip_height_amp = 0.0;
  else if (hip_height_amp > 5.0)
    hip_height_amp = 5.0;
  if (plane_theta_amp < 0.0)
    plane_theta_amp = 0.0;
  else if (plane_theta_amp > 15.0)
    plane_theta_amp = 15.0;
  if (waist_tilt > 15.0)
    waist_tilt = 15.0;
  else if (waist_tilt < -10.0)
    waist_tilt = -10.0;

  if (yshift != 0)
    waist_tilt = 5.0;
}

void walk(int Step, int t)
{
  float shift = 0;
  int i;
  float temp_theta = 0;
  static bool back_to_for = 0;	//backwalk transform to forward
  static bool for_to_back = 0;	//forward transform to backwalk
  static float travel = 0;
  static float distence = 0;
  static int turn_cnt = 0;

  static float temp_m4 = 0;
  static float temp_m3 = 0;

  if (Robot_state == 1 || Robot_state == 3 || Robot_state == 15)	//reset
  {
    back_to_for = 0;
    for_to_back = 0;
  }

  if (t == 0)
  {
    if ( length >= 0)
    {
      for_to_back = 1;
      if (back_to_for == 0)
        distence = fabs(l_foot.gol[0] - (r_foot.gol[0] - FOOT_POS_OFFSET) );
      else
      {
        distence = -fabs(l_foot.gol[0] - (r_foot.gol[0] - FOOT_POS_OFFSET) );
        back_to_for = 0;
      }
    }
    else if ( length < 0)
    {
      back_to_for = 1;
      if (for_to_back == 0)
        distence = -fabs(l_foot.gol[0] - (r_foot.gol[0] - FOOT_POS_OFFSET) );
      else
      {
        distence = fabs(l_foot.gol[0] - (r_foot.gol[0] - FOOT_POS_OFFSET) );
        for_to_back = 0;
      }
    }

    if (Step_n == step_num_tol - 1)
      travel = distence;
    else
      travel = distence + length;


    if (LIPM == 1)
    {
      ////////////////////////////////////////////////////////////////
      //LIPM
      m_HLIPM.comParamUpdate( foot_select, travel, yshift, &comParam, r_foot.gol, l_foot.gol);
    }

    //For SSVR_CONTROLLER
    if (SSVR_CONTROLLER == 1 && Robot_state == 2)
    {
      limb positionCOM_tmp;
      m_HLIPM.generateCoMTrajectory(round(cycle / 2.0), cycle, comParam,
      (TIMER_CYCLETIME / 1000.0 * cycle) / 1000.0, positionCOM_tmp.gol, TConstant);
      float temp_com = positionCOM_tmp.gol[1] - (l_hip.gol[1] + r_hip.gol[1]) / 2.0;
      SSVRController(temp_com, travel);
    }

  }

  if ( foot_select == 1)				//means left foot is swing foot
  {
    //#calculate the trajectory (relative) for left foot
    if ((t - 0.5 * accw) >= 0 && (t + 0.5 * accw) <= (cycle))
      m_HLIPM.foot_trajectory(  t - 0.5 * accw  , cycle - (accw),  travel, l_foot.rel, yshift, GaitHeight);
    else if ((t - 0.5 * accw) < 0 )
      m_HLIPM.foot_trajectory(  0           , cycle - (accw),  travel, l_foot.rel, yshift, GaitHeight);
    else if ((t + 0.5 * accw) > (cycle))
      m_HLIPM.foot_trajectory(  cycle - (accw), cycle - (accw),  travel, l_foot.rel, yshift, GaitHeight);

    r_foot.rel[0] = 0;
    r_foot.rel[1] = 0;
    r_foot.rel[2] = SHOE_HEIGHT;

    l_foot.gol[0] = l_foot.rel[0] + l_foot.tmp[0];
    l_foot.gol[1] = l_foot.rel[1] + l_foot.tmp[1];
    l_foot.gol[2] = l_foot.rel[2];
  }
  else if ( foot_select == 0)			//means right foot is swing foot
  {
    //calculate the trajectory (relative) for right foot
    if ((t - 0.5 * accw) >= 0 && (t + 0.5 * accw) <= (cycle))
      m_HLIPM.foot_trajectory( t - 0.5 * accw   , cycle - (accw), travel, r_foot.rel, yshift, GaitHeight);
    else if ((t - 0.5 * accw) < 0 )
      m_HLIPM.foot_trajectory( 0            , cycle - (accw), travel, r_foot.rel, yshift, GaitHeight);
    else if ((t + 0.5 * accw) > (cycle))
      m_HLIPM.foot_trajectory( cycle - (accw) , cycle - (accw), travel, r_foot.rel, yshift, GaitHeight);

    l_foot.rel[0] = 0;
    l_foot.rel[1] = 0;
    l_foot.rel[2] = SHOE_HEIGHT;

    r_foot.gol[0] = r_foot.rel[0] + r_foot.tmp[0];
    r_foot.gol[1] = r_foot.rel[1] + r_foot.tmp[1];
    r_foot.gol[2] = r_foot.rel[2];
  }

  if (LIPM == 0)
  {
    PositionCOM.gol[0] = PositionCOM.tmp[0] + (l_foot.rel[0] + r_foot.rel[0]) / 2.0;
    PositionCOM.gol[1] = PositionCOM.tmp[1] + (l_foot.rel[1] + r_foot.rel[1]) / 2.0;
    PositionCOM.gol[2] = l_hip.gol[2];

    shift =  m_HLIPM.hip_swing(foot_select, t, cycle);
    l_hip.gol[1] = PositionCOM.gol[1] - GAITWIDTH / 2.0 + shift ;
    r_hip.gol[1] = PositionCOM.gol[1] + GAITWIDTH / 2.0 + shift ;
    l_hip.gol[0] = PositionCOM.gol[0] + HIP_X_OFFSET;
    r_hip.gol[0] = PositionCOM.gol[0] + HIP_X_OFFSET;
  }

  if (LIPM == 1)	//LIPM
  {
    //轉彎時,晃大一點//////////////////////////////////////////////
    if (turn != 0 && length == 0 && yshift == 0)
    {
      if (turn < TURN_INIT)
        TConstant =  sqrt( ( f_LIPMZc - 2.0 * (fabs(turn) / TURN_MAX) ) / G_Acc); //6.0
      else
        TConstant =  sqrt( ( f_LIPMZc - 4.0 * (fabs(turn) / TURN_MAX) ) / G_Acc); //6.0

    }
    //調側移,讓側移時晃大一點//////////////////////////////////////
    else if (yshift > 0)		//左側移
    {
      if (foot_select == 0)	//right foot is swing foot
        TConstant =  sqrt( ( f_LIPMZc - 1.5 * (fabs(yshift) / YSHIFT_MAX) ) / G_Acc); //6.5
      else
        TConstant =  sqrt( ( f_LIPMZc - 1.5 * (fabs(yshift) / YSHIFT_MAX) ) / G_Acc); //5.5//4.5
    }
    else if (yshift < 0)		//右側移
    {
      if (foot_select == 1)	//left foot is swing foot
        TConstant =  sqrt( ( f_LIPMZc - 1.5 * (fabs(yshift) / YSHIFT_MAX) ) / G_Acc); //6.5
      else
        TConstant =  sqrt( ( f_LIPMZc - 1.5 * (fabs(yshift) / YSHIFT_MAX) ) / G_Acc); //4.5
    }
    else
      TConstant =  sqrt(f_LIPMZc / G_Acc);

    m_HLIPM.generateCoMTrajectory(t, cycle, comParam,
    (TIMER_CYCLETIME / 1000.0 * cycle) / 1000.0, PositionCOM.gol, TConstant);

    if (CHANGE_MHEIGHT == 1)
      PositionCOM.gol[2] = m_HLIPM.CMChange(t, cycle);
    else if (CHANGE_MHEIGHT == 2)
      PositionCOM.gol[2] =  m_HLIPM.CMChangeHuman(t, cycle, foot_select);

    l_hip.gol[2] = PositionCOM.gol[2] ;
    r_hip.gol[2] = PositionCOM.gol[2] ;
    l_hip.gol[1] = PositionCOM.gol[1] - GAITWIDTH / 2.0 ;
    r_hip.gol[1] = PositionCOM.gol[1] + GAITWIDTH / 2.0 ;
    l_hip.gol[0] = PositionCOM.gol[0] + HIP_X_OFFSET;
    r_hip.gol[0] = PositionCOM.gol[0] + HIP_X_OFFSET;

    if (CHANGE_HIPPLANE == 1)
    {
      temp_theta = m_HLIPM.HipPlaneTheta(t, cycle, foot_select);
      temp_theta = temp_theta * PI / 180.0f;
      float change_height =  (0.5 * GAITWIDTH) * sin(temp_theta);
      l_hip.gol[2] = PositionCOM.gol[2] - change_height;
      r_hip.gol[2] = PositionCOM.gol[2] + change_height;
      float change_y = (0.5 * GAITWIDTH) * cos(temp_theta);
      l_hip.gol[1] = PositionCOM.gol[1] - change_y ;
      r_hip.gol[1] = PositionCOM.gol[1] + change_y ;
    }
  }

  if (l_hip.gol[2] > 78.9 )
    l_hip.gol[2] = 78.9;
  if (r_hip.gol[2] > 78.9 )
    r_hip.gol[2] = 78.9;

  for (i = 0; i <= 1; i++)
  {
    l_bottom.gol[i] = l_foot.gol[i];
    r_bottom.gol[i] = r_foot.gol[i];
  }
  l_bottom.gol[2] = l_foot.gol[2] - SHOE_HEIGHT;
  r_bottom.gol[2] = r_foot.gol[2] - SHOE_HEIGHT;
  float right_delta_theta1_turn = 0;
  float left_delta_theta1_turn  = 0;
  right_delta_theta1_turn = rotation1(0 , t , cycle + 1, turn , foot_select);
  left_delta_theta1_turn  = rotation1(1 , t , cycle + 1, turn , foot_select);


  if (Robot_state == 15 )
  {
    if (t >= 9 && t <= 42)
    {
      theta_flag = theta_flag + turn / 34.0;
    }
    right_delta_theta1_turn = -theta_flag;
  }
  inv_kine_Chou(temp_theta , right_delta_theta1_turn, r_hip.gol, r_bottom.gol, 0, right_foot.motor_degree);	//calculate right foot
  inv_kine_Chou(temp_theta , left_delta_theta1_turn , l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);	//calculate left foot

  waist_pos = inv_waist(waist_tilt);
}

void Packet()
{
  invTimeOld = invTime;
  invTime = millis();
  if (invTime > invTimeOld)
    dt = (float) (invTime - invTimeOld);// / 1000.0f; // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
  else dt = 0;

  sei();

  static int t_global = 0;
  float ankle_offset = 0;
  bool wait_flag = 0;
  static int wait_count = 0;

  if (Robot_state != 0)
  {
    if (GYRO_FEEDBACK == 1) {
      ankle_offset = FeedBackControl();
    }

    //              gyro.read();
    //              Serial.print("gyro=");
    //              Serial.println(ankle_offset);//ankle_offset=FeedBackControl();

    if ( Step_n < step_num_tol && (Robot_state == 2 || Robot_state == 15 || Robot_state == 19) )
    {
      walk(Step_n, t_global);
      if (t_global < (cycle + 1 ))
        t_global++;
      if (t_global >= (cycle + 1 ) && wait_count < (wait_time) )
      {
        wait_count++;
        t_global = cycle;
      }
      else if (t_global >= (cycle + 1 ) && wait_count >= (wait_time) )
      {
        foot_select = !foot_select;
        t_global = 0;
        wait_count = 0;

        if (Step_n >= CHANGE_GAIT_STEP)	//Step_n>CHANGE_GAIT_STEP can change length,turn,yshift
        {
          G_Acc = G_ACC_STABLE;
          TConstant =  sqrt(f_LIPMZc / G_Acc);	//Tc=sqrt(Zc/g)
          GaitHeight = GAITHEIGHT_STABLE;

          change_length();
          change_yshift();
          change_turn();
          if (Robot_state == 15)
          {
            yshift = yshift_flag;
            turn = turn_flag;
          }
        }
        else if (Step_n < CHANGE_GAIT_STEP && Step_n >= 0)
        {
          if (abs(G_Acc - G_ACC_STABLE) <= 10)
            G_Acc = G_ACC_STABLE;
          else
            G_Acc = G_Acc + ( float(G_ACC_STABLE - G_ACC_INIT) / float(CHANGE_GAIT_STEP) );
          TConstant =  sqrt(f_LIPMZc / G_Acc);	//Tc=sqrt(Zc/g)

          if (fabs(GaitHeight - GAITHEIGHT_STABLE) <= 0.5)
            GaitHeight = GAITHEIGHT_STABLE;
          else
            GaitHeight = GaitHeight + ( float(GAITHEIGHT_STABLE - GAITHEIGHT_INIT) / float(CHANGE_GAIT_STEP) );
        }


        //////////////////////////////For_change_step////////////////////////////////////
        if (stop_flag == 1 && Step_n % 2 == 1)
        {
          Step_n = step_num_tol - 2;
          stop_flag = 0;
        }
        else
        {
          Step_n++;
          goandback++;
          //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
          //------------------------------------------------------------------------------------------------------------------------------------------------------------------
        }
        if (Step_n == step_num_tol - 4)
          Step_n = step_num_tol - 16;

        ////////////////////////For_odometer & return packet///////////////////////////
        if (Step_n == step_num_tol)
          record_odometer(1);
        else
          record_odometer(0);
        if (TEST == 0)
          Return_Packet();

        //////////////////////////////For_stop////////////////////////////////////
        if (Step_n == step_num_tol - 2 && Robot_state == 2)
          GaitHeight = GAITHEIGHT_STABLE * 4.0 / 5.0;
        else if (Step_n == step_num_tol - 1 && Robot_state == 2)
          GaitHeight = GAITHEIGHT_STABLE * 3.0 / 5.0;

        if (turn != 0 && Step_n >= step_num_tol - 2)
        {
          if (Robot_state == 15)
          {
            turn = turn;
            turn_flag = turn_flag;
          }
          else
          {
            if (turn > 0 && foot_select == 1)
            {
              turn = TURN_INIT;
              turn_flag = TURN_INIT;
            }
            else if (turn < 0 && foot_select == 0)
            {
              turn = TURN_INIT;
              turn_flag = TURN_INIT;
            }
          }
        }

        if (yshift != 0 && Step_n >= step_num_tol - 2)
        {
          if (yshift < 0 && foot_select == 1)
          {
            if (Robot_state == 15)
            {
              yshift = yshift;
              yshift_flag = yshift_flag;
            }
            else
            {
              //shift_left_stop = yshift;
              yshift = YSHIFT_INIT;
              yshift_flag = YSHIFT_INIT;
            }
          }
          else if (yshift > 0 && foot_select == 0)
          {
            if (Robot_state == 15)
            {
              yshift = yshift;
              yshift_flag = yshift_flag;
            }
            else
            {
              yshift = YSHIFT_INIT;
              yshift_flag = YSHIFT_INIT;
            }
          }
        }

        //////////////////////For record last step pos//////////////////////
        r_foot.tmp[0] = r_foot.gol[0];
        l_foot.tmp[0] = l_foot.gol[0];
        r_foot.tmp[1] = r_foot.gol[1];
        l_foot.tmp[1] = l_foot.gol[1];
        PositionCOM.tmp[0] = PositionCOM.gol[0];
        PositionCOM.tmp[1] = PositionCOM.gol[1];

      }
      Swing(t_global);
      Gait_BigRobot_Arm(512);
      Gait_BigRobot(ankle_offset, 1023);
    }

    else if ( Robot_state == 1 )
    { //  
      init_parameter();
      if (TEST == 0)
        Return_Packet();
      t_global = 0;
      walk(Step_n, t_global);
      Gait_BigRobot(ankle_offset, 100);
      Step_n = step_num_tol + 1;
    }
    else if ( Step_n == step_num_tol && (Robot_state == 2 || Robot_state == 15) )
    { // 走到一半停下
      if (Robot_state == 2)
      {
        init_parameter();
        if (TEST == 0)
          Return_Packet();
        t_global = 0;
        walk(Step_n, t_global);
        Gait_BigRobot(ankle_offset, 200);
        Step_n = step_num_tol + 1;
        Robot_state = 3;
      }
      else if (Robot_state == 15)
      {
        if (TEST == 0)
          Return_Packet();
        t_global = 0;
        walk(Step_n, t_global);
        Gait_BigRobot(ankle_offset, 200);
        Step_n = step_num_tol + 1;
        Robot_state = 44;	//Don't do anything, and after entering this state, you can't jump out(need to reset controller)
      }
    }
    else if (Robot_state == 3)		//restart wait
    { // Stop halfway and wait 1second
      init_parameter();
      if (TEST == 0)
        Return_Packet();
      t_global = 0;
      walk(Step_n, t_global);
      Gait_BigRobot(ankle_offset, 200);
      wait_flag = wait_time_function(1000);
      if (wait_flag == 1)
      {
        Robot_state = 1;
        length_odometer = 128;
        turn_odometer = 128;
        yshift_odometer = 128;
      }
    }
    else if (Robot_state == 7 ) kick(0, KickForce, 0, KickDir); //small left_kick
    //		else if (Robot_state == 9 ) kick(0, 27,0,KickDir);  //left_kick
    //		else if (Robot_state == 11) kick(0, 20,0,KickDir);  //big left_kick
    //		else if (Robot_state == 13) kick(0, 24,1,KickDir);  //high left_kick
    else if (Robot_state == 8 ) kick(1, KickForce, 0, KickDir); //small right_kick
    else if (Robot_state == 9 ) kick(0, KickForce, 1, KickDir);
    else if (Robot_state == 10) kick(1, KickForce, 1, KickDir); 
    //		else if (Robot_state == 10) kick(1, 27,0,KickDir);  //right_kick
    //		else if (Robot_state == 12) kick(1, 20,0,KickDir);  //big right_kick
    //		else if (Robot_state == 14) kick(1, 24,1,KickDir);  //high right_kick
    //=============================Squat====================================================================================================================
    else if (Robot_state == 20)	Squat_Pre();//feet open
    else if (Robot_state == 21)	squat(0);//0:squat down
    else if (Robot_state == 22)	squat(1);//1:stand up
    else if (Robot_state == 23) Jump();
    else if (Robot_state == 150 ) {
      //standUpProcedure = 0;
      StandUp();
      digitalWrite(SELECT, HIGH);
      //
      //Robot_state = 151;
    }
    else if (Robot_state == 200) {
      BackStandUp();
      digitalWrite(SELECT, HIGH);
    }
    else if (Robot_state == 151) {
      ArmUp(1023);
      FallLeft(1023);
      //Robot_state = 150;
      if (motorEnable == false) {
        if (wait_time_function(3000))
          Robot_state = 150;
      }
    }
    else if (Robot_state == 152) {
      ArmUp(1023);
      FallRight(1023);
      //Robot_state = 150;
      if (motorEnable == false) {
        if (wait_time_function(3000))
          Robot_state = 150;
      }
    }
    //===========================================================================================================================================================
    else if (Robot_state == 5)
    {
      static int read_tmp_status = 1;
      int temperature = 0;
      int motor_id = 0;
      switch (read_tmp_status)
      {
      case 1:
        motor_id = 1;
        break;
      case 2:
        motor_id = 2;
        break;
      case 3:
        motor_id = 3;
        break;
      case 4:
        motor_id = 4;
        break;
      case 5:
        motor_id = 7;
        break;
      case 6:
        motor_id = 8;
        break;
      case 7:
        motor_id = 11;
        break;
      case 8:
        motor_id = 12;
        break;
      case 9:
        motor_id = 21;
        break;
      case 10:
        motor_id = 22;
        break;
      case 11:
        motor_id = 23;
        break;
      case 12:
        motor_id = 24;
        break;
      case 13:
        motor_id = 27;
        break;
      case 14:
        motor_id = 28;
        break;
      case 15:
        motor_id = 31;
        break;
      case 16:
        motor_id = 32;
        break;
      default:
        motor_id = 1;
        break;
      }
      temperature = ReadTemp(motor_id);
      if (read_tmp_status == 16)
      {
        //Serial.println(temperature);
      }
      else
      {
        //Serial.print(temperature);
        //Serial.print(" ");
      }
      read_tmp_status++;
      if (read_tmp_status >= 16)
      {
        Robot_state = 0;
        read_tmp_status = 1;
      }
    }
  }
  T_Global = t_global;

  if (Robot_state == 2)
  {
    //Arm_walk(foot_select, t_global);
    Gait_BigRobot_Arm(50);

    if (Accel_Faild == 0)  Compute_FCBGSI(foot_select, t_global);
    Serial_Communication();
    Compute_Compass(Robot_state, foot_select, t_global);
  }
  else  Compute_Compass(Robot_state, foot_select, t_global);
}

double s_curve(double t, double tf, double L)
{
  double a2 = 3.0 * L / (tf*tf);
  double a3=  -2.0 * L / ( tf*tf*tf );
  double pos = a2*t*t + a3*t*t*t;  
  return pos;
}

void kick(int type, int kick_force, bool high_kick, int turn_offset)
{
  //type = 0:left kick, 		type = 1:right kick
  //high_kick = 0:normal kick,	high_kick = 1:high ball
  //kick_dir = 0:not corrected		kick_dir = 1:left 5degree	kick_dir = 2:right 5degree

  float RK_offset_ankle = 5.0;		
  float RK_offset_hip  =   5.0;		
  float LK_offset_ankle =  5.0;		
  float LK_offset_hip  =   5.0;		
  float foort_raise    =  5.0;		
  float foot_back_raise =  -2.0;		
  float foot_back      =  -8.0;		
  float foot_forward   =  28.0;		
  float shift_right    =  7.7;//7.5;//9.7     	
  float shift_left     =  -7.0;//-11.0;//-10.7;//-10.5;	
  float Lextend_offset_hip =  10.0;	
  float Lextend_offset_ankle =  10.0;	
  float Rextend_offset_hip = -4.0;	
  float Rextend_offset_ankle = -4.0;	

  bool wait_flag = 0;			
  static float temp_m0 = 0, temp_m3 = 0;	
  static float shift_to_middle = 0;		
  static float extend_temp_m0 = 0, extend_temp_m3 = 0;	
  static int t_kick = 0;
  static int kick_state = 1;
  
  float leg_support = 0.8;//0717 add
  float kick_raise = 35.0;//0717 add
  
  float kick_period[8];
  kick_period[0] = 13.0 * 4;
  kick_period[1] = 13.0 * 4;
  kick_period[2] = 15;
  kick_period[3] = 10.0 * 4;
  kick_period[4] = 14;//kick_force ;
  kick_period[5] = 30.0 * 4;

  if (high_kick)
    if (type == 0)  kick_period[6] = 30.0;  //left high kick
    else  kick_period[6] = 30.0;   //right  high kick
  else
    kick_period[6] = 80.0;           //normal kick
  kick_period[7] = 40.0;

  float Rhighkick_foort_raise = 2.0;        	
  float Lhighkick_foort_raise = 2.0;		
  float highkick_foort_raise2 = 8.0;//18.0;//16.0;//11.0
  float highkick_foot_forward = 10.0; //21.0;//14.0	
  float highkick_force = kick_force;
  float highkick_ratio = 1.7;
  static float temp_m4 = 0;
  static float temp_x = 0;
  static float temp_z = 0;  

  if (kick_state == 1 && t_kick == 0 )
  {
    shift_to_middle = (l_hip.gol[1] + r_hip.gol[1]) / 2.0;
    for (int i = 0; i <= 1; i++)
    {
      l_bottom.gol[i] = l_foot.gol[i];
      r_bottom.gol[i] = r_foot.gol[i];
    }
    l_bottom.gol[2] = l_foot.gol[2] - SHOE_HEIGHT;
    r_bottom.gol[2] = r_foot.gol[2] - SHOE_HEIGHT;
  }
  if (type == 0)	//left kick
  {
    if (kick_state == 1) //back to the middle
    {
      if (t_kick < kick_period[0])
      {
        l_hip.gol[1] = l_hip.gol[1] - shift_to_middle / kick_period[0];
        r_hip.gol[1] = r_hip.gol[1] - shift_to_middle / kick_period[0];
        t_kick++;
      }
      else if (t_kick >= kick_period[0])
      {
        t_kick = 0;
        kick_state = 2; //2
      }
      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      Gait_BigRobot(0, 1023);

    }
    else if (kick_state == 2) //center of gravity to the right
    {
      if (t_kick < kick_period[1])
      {
        Arm_kick( type, kick_state, kick_period[1], t_kick);
        l_hip.gol[1] = l_hip.gol[1] + shift_right / kick_period[1];
        r_hip.gol[1] = r_hip.gol[1] + shift_right / kick_period[1];
        l_hip.gol[2] = l_hip.gol[2] + leg_support / kick_period[1];
        t_kick++;
      }
      else if (t_kick >= kick_period[1])
      {
        wait_flag = wait_time_function(500);
        if (wait_flag == 1)
        {
          Arm_kick( type, kick_state, kick_period[1], t_kick);
          t_kick = 0;
          kick_state = 3; //3
          temp_m0 = right_foot.motor_degree[0];
          temp_m3 = right_foot.motor_degree[4];
        }
      }
      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      Gait_BigRobot(0, 1023);
      Gait_BigRobot_Arm(512);

    }
    else if (kick_state == 3) //Turn the center of gravity to the right + lift the left foot
    {
      if (t_kick < kick_period[2])
      {
        if (high_kick == 0)	
          l_bottom.gol[2] = l_bottom.gol[2] + foort_raise / kick_period[2];
        else			
        l_bottom.gol[2] = l_bottom.gol[2] + Lhighkick_foort_raise / kick_period[2];
        l_bottom.gol[0] = l_bottom.gol[0] + foot_back_raise / kick_period[2];

        inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
        inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);

        temp_m0 = temp_m0 - LK_offset_ankle / kick_period[2];
        temp_m3 = temp_m3 + LK_offset_hip / kick_period[2];
        right_foot.motor_degree[0] = temp_m0; //ankle
        right_foot.motor_degree[4] = temp_m3;

        t_kick++;
      }
      else if (t_kick >= kick_period[2])
      {
        wait_flag = wait_time_function(2000); //1000
        if (wait_flag == 1)
        {
          t_kick = 0;
          kick_state = 4; //4
          temp_m4 = 0.0;
        }
      }
      Gait_BigRobot(0, 400);
    }
    else if (kick_state == 4) //left foot back
    {
      if (t_kick < kick_period[3])
      {
        l_bottom.gol[0] = l_bottom.gol[0] + foot_back / kick_period[3];
        temp_m4 = temp_m4 - turn_offset / kick_period[3];
        t_kick++;
      }
      else if (t_kick >= kick_period[3])
      {
        t_kick = 0;
        kick_state = 5; //5
        temp_x = l_bottom.gol[0];
        temp_z = l_bottom.gol[2];
      }
      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, temp_m4, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      right_foot.motor_degree[0] = temp_m0; //ankle
      right_foot.motor_degree[4] = temp_m3;
      Gait_BigRobot(0, 1023);
    }

    else if (kick_state == 5) //Kick
    {
      if (high_kick == 0)	
      {
        if (t_kick < kick_period[4])
        {
          Arm_kick( type, kick_state, kick_period[4], t_kick);
          l_bottom.gol[0] = temp_x + s_curve(t_kick, kick_period[4], (foot_forward - (foot_back + foot_back_raise)));
//          l_bottom.gol[0] = l_bottom.gol[0] + (foot_forward - (foot_back + foot_back_raise)) / kick_period[4];
          l_bottom.gol[2] = temp_z + s_curve(t_kick, kick_period[4], kick_raise);// l_bottom.gol[0] + (foot_forward - (foot_back + foot_back_raise)) / kick_period[4];

          t_kick++;
        }
        else if (t_kick >= kick_period[4])
        {
          wait_flag = wait_time_function(1500);
          if (wait_flag == 1)
          {
            Arm_kick( type, kick_state, kick_period[4], t_kick);
            t_kick = 0;
            kick_state = 6; //6
          }
        }
      }
      else
      {
        if (t_kick < round(highkick_force / highkick_ratio) )
        {
          Arm_kick( type, kick_state, highkick_force , t_kick);
          l_bottom.gol[0] = l_bottom.gol[0] + (highkick_foot_forward - (foot_back + foot_back_raise)) / highkick_force;
          t_kick++;
        }
        else if (t_kick >= round(highkick_force / highkick_ratio) && t_kick < highkick_force)
        {
          Arm_kick( type, kick_state, highkick_force , t_kick);
          l_bottom.gol[0] = l_bottom.gol[0] + (highkick_foot_forward - (foot_back + foot_back_raise)) / highkick_force;
          l_bottom.gol[2] = l_bottom.gol[2] + highkick_foort_raise2 / (highkick_force - round(highkick_force / highkick_ratio));
          t_kick++;
        }
        else if (t_kick >= highkick_force)
        {
          wait_flag = wait_time_function(1500);
          if (wait_flag == 1)
          {
            Arm_kick( type, kick_state, highkick_force, highkick_force);
            t_kick = 0;
            kick_state = 6; //6
          }
        }
      }
      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, temp_m4, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      right_foot.motor_degree[0] = temp_m0; //ankle
      right_foot.motor_degree[4] = temp_m3;
      Gait_BigRobot(0, 1023);
      Gait_BigRobot_Arm(512);
    }
    else if (kick_state == 6) 
    {
      if (t_kick < kick_period[5])
      {
        if (high_kick == 0){	
          l_bottom.gol[0] = l_bottom.gol[0] - foot_forward / kick_period[5];
          l_bottom.gol[2] = l_bottom.gol[2] - kick_raise / kick_period[5];
        }
        else
          l_bottom.gol[0] = l_bottom.gol[0] - highkick_foot_forward / kick_period[5];

        temp_m4 = temp_m4 + turn_offset / kick_period[5];
        t_kick++;
      }
      else if (t_kick >= kick_period[5])
      {
        t_kick = 0;
        kick_state = 7; //7
        extend_temp_m0 = left_foot.motor_degree[0];
        extend_temp_m3 = left_foot.motor_degree[4];
        temp_m4 = 0.0;
      }
      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, temp_m4, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      right_foot.motor_degree[0] = temp_m0; 
      right_foot.motor_degree[4] = temp_m3;
      Gait_BigRobot(0, 1023);
    }
    else if (kick_state == 7) 
    {
      if (t_kick < kick_period[6])
      {
        if (high_kick == 0)	
          l_bottom.gol[2] = l_bottom.gol[2] - foort_raise / kick_period[6];
        else			
        l_bottom.gol[2] = l_bottom.gol[2] - (Lhighkick_foort_raise + highkick_foort_raise2) / kick_period[6];

        inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
        inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);

        temp_m0 = temp_m0 + (LK_offset_ankle - 3) / kick_period[6];
        temp_m3 = temp_m3 - LK_offset_hip / kick_period[6];
        right_foot.motor_degree[0] = temp_m0; //

        extend_temp_m0 -= Lextend_offset_ankle / kick_period[6]; 
        extend_temp_m3 -= Lextend_offset_hip / kick_period[6];
        left_foot.motor_degree[0] = extend_temp_m0;
        left_foot.motor_degree[4] = extend_temp_m3;

        t_kick++;
      }
      else if (t_kick >= kick_period[6])
      {
        wait_flag = wait_time_function(100);
        if (wait_flag == 1)
        {
          t_kick = 0;
          kick_state = 8; //8
        }
      }
      Gait_BigRobot(0, 1023);
    }
    else if (kick_state == 8) 
    {
      if (t_kick < kick_period[7])
      {
        Arm_kick( type, kick_state, kick_period[7], t_kick);
        l_hip.gol[1] = l_hip.gol[1] - (shift_right - shift_to_middle) / kick_period[7];
        r_hip.gol[1] = r_hip.gol[1] - (shift_right - shift_to_middle) / kick_period[7];
        l_hip.gol[2] = l_hip.gol[2] - leg_support / kick_period[7];
        t_kick++;
      }
      else if (t_kick >= kick_period[7])
      {
        wait_flag = wait_time_function(500);
        if (wait_flag == 1)
        {
          Arm_kick( type, kick_state, kick_period[7], t_kick);
          t_kick = 0;
          kick_state = 1;
          shift_to_middle = 0;
          temp_m0 = 0;
          temp_m3 = 0;
          extend_temp_m0 = 0;
          extend_temp_m3 = 0;
          Robot_state = 1;
        }
      }
      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      Gait_BigRobot(0, 80);
      Gait_BigRobot_Arm(80);
    }
  }
  else if (type == 1)	//right_kick
  {
    if (kick_state == 1) //back to the middle
    {
      if (t_kick < kick_period[0])
      {
        l_hip.gol[1] = l_hip.gol[1] - shift_to_middle / kick_period[0];
        r_hip.gol[1] = r_hip.gol[1] - shift_to_middle / kick_period[0];
        t_kick++;
      }
      else if (t_kick >= kick_period[0])
      {
        t_kick = 0;
        kick_state = 2; //2
      }
      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      Gait_BigRobot(0, 1023);

    }
    else if (kick_state == 2) //Center of gravity to the left
    {
      if (t_kick < kick_period[1])
      {
        Arm_kick( type, kick_state, kick_period[1], t_kick);
        l_hip.gol[1] = l_hip.gol[1] + shift_left / kick_period[1];
        r_hip.gol[1] = r_hip.gol[1] + shift_left / kick_period[1];
        t_kick++;
      }
      else if (t_kick >= kick_period[1])
      {
        wait_flag = wait_time_function(500);
        if (wait_flag == 1)
        {
          Arm_kick( type, kick_state, kick_period[1], t_kick);
          t_kick = 0;
          kick_state = 3; //3
          temp_m0 = left_foot.motor_degree[0];
          temp_m3 = left_foot.motor_degree[4];
        }
      }
      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      Gait_BigRobot(0, 1023);
      Gait_BigRobot_Arm(512);

    }
    else if (kick_state == 3) //Turn the center of gravity to the left + lift the right foot
    {
      if (t_kick < kick_period[2])
      {
        if (high_kick == 0)	
          r_bottom.gol[2] = r_bottom.gol[2] + foort_raise / kick_period[2];
        else
          r_bottom.gol[2] = r_bottom.gol[2] + Rhighkick_foort_raise / kick_period[2];
        r_bottom.gol[0] = r_bottom.gol[0] + foot_back_raise / kick_period[2];

        inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
        inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);

        temp_m0 = temp_m0 + RK_offset_ankle / kick_period[2];
        temp_m3 = temp_m3 - RK_offset_hip / kick_period[2];
        left_foot.motor_degree[0] = temp_m0; 
        left_foot.motor_degree[4] = temp_m3;

        t_kick++;
      }
      else if (t_kick >= kick_period[2])
      {
        wait_flag = wait_time_function(2000); //1000
        if (wait_flag == 1)
        {
          t_kick = 0;
          kick_state = 4; //4
          temp_m4 = 0.0;
        }
      }
      Gait_BigRobot(0, 400);
    }
    else if (kick_state == 4) 
    {
      if (t_kick < kick_period[3])
      {
        r_bottom.gol[0] = r_bottom.gol[0] + foot_back / kick_period[3];
        temp_m4 = temp_m4 + turn_offset / kick_period[3];
        t_kick++;
      }
      else if (t_kick >= kick_period[3])
      {
        t_kick = 0;
        kick_state = 5; //5
      }
      inv_kine_Chou(0, temp_m4, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      left_foot.motor_degree[0] = temp_m0; 
      left_foot.motor_degree[4] = temp_m3;
      Gait_BigRobot(0, 1023);
    }

    else if (kick_state == 5) 
    {
      if (high_kick == 0)	
      {
        if (t_kick < kick_period[4])
        {
          Arm_kick( type, kick_state, kick_period[4], t_kick);
          r_bottom.gol[0] = r_bottom.gol[0] + (foot_forward - (foot_back + foot_back_raise)) / kick_period[4];
          t_kick++;
        }
        else if (t_kick >= kick_period[4])
        {
          wait_flag = wait_time_function(1500);
          if (wait_flag == 1)
          {
            Arm_kick( type, kick_state, kick_period[4], t_kick);
            t_kick = 0;
            kick_state = 6; //6
          }
        }
      }
      else			
      {
        if (t_kick < round(highkick_force / highkick_ratio) )
        {
          Arm_kick( type, kick_state, highkick_force, t_kick);
          r_bottom.gol[0] = r_bottom.gol[0] + (highkick_foot_forward - (foot_back + foot_back_raise)) / highkick_force;
          t_kick++;
        }
        else if (t_kick >= round(highkick_force / highkick_ratio) && t_kick < highkick_force)
        {
          Arm_kick( type, kick_state, highkick_force, t_kick);
          r_bottom.gol[0] = r_bottom.gol[0] + (highkick_foot_forward - (foot_back + foot_back_raise)) / highkick_force;
          r_bottom.gol[2] = r_bottom.gol[2] + highkick_foort_raise2 / (highkick_force - round(highkick_force / highkick_ratio));
          t_kick++;
        }
        else if (t_kick >= highkick_force)
        {
          wait_flag = wait_time_function(1500);
          if (wait_flag == 1)
          {
            Arm_kick( type, kick_state, highkick_force, highkick_force);
            t_kick = 0;
            kick_state = 6; //6
          }
        }
      }
      inv_kine_Chou(0, temp_m4, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      left_foot.motor_degree[0] = temp_m0; 
      left_foot.motor_degree[4] = temp_m3;
      Gait_BigRobot(0, 1023);
      Gait_BigRobot_Arm(512);
    }
    else if (kick_state == 6) 
    {
      if (t_kick < kick_period[5])
      {
        if (high_kick == 0)	
          r_bottom.gol[0] = r_bottom.gol[0] - foot_forward / kick_period[5];
        else
          r_bottom.gol[0] = r_bottom.gol[0] - highkick_foot_forward / kick_period[5];
        temp_m4 = temp_m4 - turn_offset / kick_period[5];
        t_kick++;
      }
      else if (t_kick >= kick_period[5])
      {
        t_kick = 0;
        kick_state = 7; //7
        extend_temp_m0 = right_foot.motor_degree[0];
        extend_temp_m3 = right_foot.motor_degree[4];
        temp_m4 = 0.0;
      }
      inv_kine_Chou(0, temp_m4, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      left_foot.motor_degree[0] = temp_m0; //
      left_foot.motor_degree[4] = temp_m3;
      Gait_BigRobot(0, 1023);
    }
    else if (kick_state == 7) //
    {
      if (t_kick < kick_period[6])
      {
        if (high_kick == 0)	//
          r_bottom.gol[2] = r_bottom.gol[2] - foort_raise / kick_period[6];
        else
          r_bottom.gol[2] = r_bottom.gol[2] - (Rhighkick_foort_raise + highkick_foort_raise2) / kick_period[6];

        inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
        inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);

        temp_m0 = temp_m0 - RK_offset_ankle / kick_period[6];
        temp_m3 = temp_m3 + RK_offset_hip / kick_period[6];
        left_foot.motor_degree[0] = temp_m0; //
        left_foot.motor_degree[4] = temp_m3;

        extend_temp_m0 = extend_temp_m0 + Rextend_offset_ankle / kick_period[6]; //
        extend_temp_m3 = extend_temp_m3 + Rextend_offset_hip / kick_period[6];
        right_foot.motor_degree[0] = extend_temp_m0;
        right_foot.motor_degree[4] = extend_temp_m3;

        t_kick++;
      }
      else if (t_kick >= kick_period[6])
      {
        wait_flag = wait_time_function(100); //!!!!!!!!
        if (wait_flag == 1)
        {
          t_kick = 0;
          kick_state = 8; //8
        }
      }
      Gait_BigRobot(0, 1000);
    }
    /*else if (kick_state == 8) {
     if (t_kick < kick_period[6])
     {
     temp_m0 = temp_m0 - RK_offset_ankle / kick_period[6];
     temp_m3 = temp_m3 + RK_offset_hip / kick_period[6];
     left_foot.motor_degree[0] = temp_m0; //
     left_foot.motor_degree[3] = temp_m3;
     
     extend_temp_m0 += Rextend_offset_ankle / kick_period[6]; //
     extend_temp_m3 += Rextend_offset_hip / kick_period[6];
     right_foot.motor_degree[0] = extend_temp_m0;
     right_foot.motor_degree[3] = extend_temp_m3;
     
     inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
     inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
     
     t_kick++;
     }
     else if (t_kick >= kick_period[6])
     {
     wait_flag = wait_time_function(1000); //!!!!!!!!
     if (wait_flag == 1)
     {
     t_kick = 0;
     kick_state = 9; //8
     }
     }
     Gait_BigRobot(0, 1023);
     }*/
    else if (kick_state == 8) //
    {
      if (t_kick < kick_period[7])
      {
        Arm_kick( type, kick_state, kick_period[7], t_kick);
        l_hip.gol[1] = l_hip.gol[1] - (shift_left - shift_to_middle) / kick_period[7];
        r_hip.gol[1] = r_hip.gol[1] - (shift_left - shift_to_middle) / kick_period[7];
        t_kick++;
      }
      else if (t_kick >= kick_period[7])
      {
        wait_flag = wait_time_function(500);
        if (wait_flag == 1)
        {
          Arm_kick( type, kick_state, kick_period[7], t_kick);
          t_kick = 0;
          kick_state = 1;
          shift_to_middle = 0;
          temp_m0 = 0;
          temp_m3 = 0;
          extend_temp_m0 = 0;
          extend_temp_m3 = 0;
          Robot_state = 1;
        }
      }
      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      Gait_BigRobot(0, 80);
      Gait_BigRobot_Arm(80);
    }
  }
}

bool wait_time_function(int wait_minisec)
{
  static int wait_count2 = 0;
  float temp = 0;
  temp = float(wait_minisec) / float(TIMER_CYCLETIME) * 1000.0;

  if (wait_count2 < temp )
  {
    wait_count2++;
    return 0;
  }
  else
  {
    wait_count2 = 0;
    return 1;
  }
}

void record_odometer(bool stop_step)
{
  if (stop_step == 1)
  {
    if (length == LENGTH_INIT)
      length_odometer = length_odometer + (length) * 0;
    else if (length > LENGTH_INIT)
      length_odometer = length_odometer + (length * 0.0) * 1.18; //1.17
    else if (length < LENGTH_INIT)
      length_odometer = length_odometer + (length * 0.0) * 1.1; //0.99
  }
  else
  {
    if (length == LENGTH_INIT)
      length_odometer = length_odometer + (length) * 0;
    else if (length > LENGTH_INIT)
      length_odometer = length_odometer + ((length - LENGTH_INIT)) * 1.18; //1.17
    else if (length < LENGTH_INIT)
      length_odometer = length_odometer + ((length - LENGTH_INIT)) * 1.1; //0.99
  }

  if (yshift == YSHIFT_INIT)
    yshift_odometer = yshift_odometer + (yshift) * 0;
  else if (yshift > YSHIFT_INIT && Step_n % 2 == 1)
    yshift_odometer = yshift_odometer + ((yshift - YSHIFT_INIT)) * 1.09; //1.1
  else if (yshift < YSHIFT_INIT && Step_n % 2 == 0)
    yshift_odometer = yshift_odometer + ((yshift - YSHIFT_INIT)) * 1.02; //1.06

  if (turn == TURN_INIT)
    turn_odometer = turn_odometer + (turn) * 0;
  else if (turn > TURN_INIT && Step_n % 2 == 1)
    turn_odometer = turn_odometer + ((turn - TURN_INIT)) * 1.24; //1
  else if (turn < TURN_INIT && Step_n % 2 == 0)
    turn_odometer = turn_odometer + ((turn - TURN_INIT)) * 1.24; //1

  /*mechanism correction*/
  if (yshift < YSHIFT_INIT && Step_n % 2 == 0)                       //Move left
    turn_odometer = turn_odometer - (turn_Yofset_L * fabs(yshift)) * 1;
  else if (yshift >= YSHIFT_INIT && Step_n % 2 == 0)                 //Move to the right
    turn_odometer = turn_odometer - (turn_Yofset_R * fabs(yshift)) * 1;
  if (length > LENGTH_INIT && Step_n % 2 == 0)                       //go ahead
    turn_odometer = turn_odometer - (turn_Xofset_Forward * fabs(length)) * 1;
}

void Return_Packet()
{
  unsigned char start_byte = 0xFE;
  unsigned char stop_byte = 0xFF;
  unsigned char data_temp_l = round(length_odometer);
  unsigned char data_temp_y = round(yshift_odometer);
  unsigned char data_temp_t = round(turn_odometer);

  float2Int.f = yaw + 180.0f;    

  int tmp = (int) float2Int.f;
  int tmp1 = tmp / 256;
  int tmp2 = tmp % 256;
  unsigned char data_compass_1 = tmp1;
  unsigned char data_compass_2 = tmp2;

  //-------------------------------------------------------------------------
  unsigned char data_set = SET_BUTTON;
  unsigned char data_start = START_BUTTON;
  //-------------------------------------------------------------------------

  unsigned char data_state = 0;

  if ( Robot_state == 1){		//
    data_state = 100;
    if(wait_time_function(2000))
      fallDown = false;
  }
  else				
  data_state = 200;

  Serial.write(start_byte);
  Serial.write(data_temp_l);
  Serial.write(data_temp_y);
  Serial.write(data_temp_t);
  Serial.write(data_state);
  //-----------------------------------------------------------------
  Serial.write(data_compass_1);
  Serial.write(data_compass_2);
  //-----------------------------------------------------------------
  Serial.write(data_set);
  Serial.write(data_start);
  //-----------------------------------------------------------------
  Serial.write(goandback);
  Serial.write(stop_byte);
}

void change_turn()
{
  if (fabs(turn_flag - turn) <= TURN_UNIT)
  {
    if (turn * turn_flag < 0 )
    {
      if (turn < 0 && foot_select == 0)
        turn = 0;
      else if (turn > 0 && foot_select == 1)
        turn = 0;
    }
    else
    {
      if (turn == 0)
      {
        if (turn_flag < 0 && foot_select == 0)
          turn = turn_flag;
        else if (turn_flag > 0 && foot_select == 1)
          turn = turn_flag;
      }
      else if (turn < 0 && foot_select == 0)
        turn = turn_flag;
      else if (turn > 0 && foot_select == 1)
        turn = turn_flag;
    }
  }
  else if (turn_flag - turn > 0)
  {
    if (turn * (turn + TURN_UNIT) < 0)
    {
      if (turn < 0 && foot_select == 0)
        turn = 0;
      else if (turn > 0 && foot_select == 1)
        turn = 0;
    }
    else
    {
      if (turn == 0)
      {
        if (turn + TURN_UNIT < 0 && foot_select == 0)
          turn = turn + TURN_UNIT;
        else if (turn + TURN_UNIT > 0 && foot_select == 1)
          turn = turn + TURN_UNIT;
      }
      else if (turn < 0 && foot_select == 0)
        turn = turn + TURN_UNIT;
      else if (turn > 0 && foot_select == 1)
        turn = turn + TURN_UNIT;
    }
  }
  else if (turn_flag - turn < 0 )
  {
    if (turn * (turn - TURN_UNIT) < 0)
    {
      if (turn < 0 && foot_select == 0)
        turn = 0;
      else if (turn > 0 && foot_select == 1)
        turn = 0;
    }
    else
    {
      if (turn == 0)
      {
        if (turn - TURN_UNIT < 0 && foot_select == 0)
          turn = turn - TURN_UNIT;
        else if (turn - TURN_UNIT > 0 && foot_select == 1)
          turn = turn - TURN_UNIT;
      }
      else if (turn < 0 && foot_select == 0)
        turn = turn - TURN_UNIT;
      else if (turn > 0 && foot_select == 1)
        turn = turn - TURN_UNIT;
    }
  }
}


void change_yshift()
{
  if (yshift_flag - yshift > 0 && yshift >= 0 && foot_select == 0)
  {
    if (fabs(yshift_flag - yshift) < YSHIFT_UNIT)
      yshift = yshift_flag;
    else
      yshift = yshift + YSHIFT_UNIT;
  }
  else if (yshift_flag - yshift > 0 && yshift < 0 &&  foot_select == 1)
  {
    if ( (yshift * (yshift + YSHIFT_UNIT)) < 0)
      yshift = 0;
    else if (fabs(yshift_flag - yshift) < YSHIFT_UNIT)
      yshift = yshift_flag;
    else
      yshift = yshift + YSHIFT_UNIT;
  }
  else if (yshift_flag - yshift < 0 && yshift <= 0 &&  foot_select == 1)
  {
    if (fabs(yshift_flag - yshift) < YSHIFT_UNIT)
      yshift = yshift_flag;
    else
      yshift = yshift - YSHIFT_UNIT;
  }
  else if (yshift_flag - yshift < 0 && yshift > 0 &&  foot_select == 0)
  {
    if ( (yshift * (yshift - YSHIFT_UNIT)) < 0)
      yshift = 0;
    else if (fabs(yshift_flag - yshift) < YSHIFT_UNIT)
      yshift = yshift_flag;
    else
      yshift = yshift - YSHIFT_UNIT;
  }
}

void change_length()
{
  if (fabs(length_flag - length) < LENGTH_UNIT)
    length = length_flag;
  else if (length_flag - length > 0 )
    length = length + LENGTH_UNIT;
  else if (length_flag - length < 0 )
    length = length - LENGTH_UNIT;
}

void Gait_BigRobot(float ankle_roll_offset, int motor_velocity) {
  unsigned char array[108];//20*5+7+1
  unsigned int r[6];
  unsigned int l[6];

  if (GYRO_FEEDBACK == 1)
  {
    left_foot.motor_degree[0] -= ankle_roll_offset * 1.0;
    right_foot.motor_degree[0] -= ankle_roll_offset * 1.0;
  }

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

void Gait_BigRobot_backup(float ankle_roll_offset, int motor_velocity)
{
  unsigned char array[11 * 5 + 7 + 1];
  unsigned int r[6];
  unsigned int l[6];
  unsigned int r_speed[6];
  unsigned int l_speed[6];


  if (GYRO_FEEDBACK)
  {

    //        	if(foot_select==0)
    //        	{
    //		    left_foot.motor_degree[0] -= ankle_roll_offset;
    //        	}
    //        	else if(foot_select==1)
    //        	{
    //		    right_foot.motor_degree[0] -= ankle_roll_offset;
    //        	}

    left_foot.motor_degree[0] += ankle_roll_offset;
    right_foot.motor_degree[0] += ankle_roll_offset;
  }
  l[1] = round( (left_foot.motor_degree[1] / 360.0 * 4096.0)); //
  l[2] = round( (left_foot.motor_degree[2] / 360.0 * 4096.0)); //膝上
  l[3] = round( (left_foot.motor_degree[3] / 360.0 * 4096.0)); //hip
  l[4] = round( (left_foot.motor_degree[0] / 360.0 * 4096.0)); //
  l[5] = round( (left_foot.motor_degree[4] / 360.0 * 4096.0)); //hip_turn

  r[1] = round( (right_foot.motor_degree[1] / 360.0 * 4096.0));	//
  r[2] = round( (right_foot.motor_degree[2] / 360.0 * 4096.0));	//膝上	(144=offset from gear)
  r[3] = round( (right_foot.motor_degree[3] / 360.0 * 4096.0));	//hip
  r[4] = round( (right_foot.motor_degree[0] / 360.0 * 4096.0));	//
  r[5] = round( (right_foot.motor_degree[4] / 360.0 * 4096.0));	//hip_turn

  for (int i = 1; i <= 5; i++)
  {
    l_speed[i] = motor_velocity;
    r_speed[i] = motor_velocity;
  }
  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (4 + 1) * 11 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x1E;			//Parameter1 Start address to write Data
  array[6] = 0x04;			//Parameter2 Length of Data to write

  array[7] = 53; //new;
  array[8] = l[3] % 256;
  array[9] = l[3] / 256;
  array[10] = l_speed[3] % 256;
  array[11] = l_speed[3] / 256;

  array[12] = 57; //new
  array[13] = l[1] % 256;
  array[14] = l[1] / 256;
  array[15] = (l_speed[1]) % 256; //round(l_speed[1]*0.7)%256;
  array[16] = (l_speed[1]) / 256; //round(l_speed[1]*0.7)/256;

  array[17] = 55; //new
  array[18] = l[2] % 256;
  array[19] = l[2] / 256;
  array[20] = (l_speed[2]) % 256; //round(l_speed[2]*0.7)%256;
  array[21] = (l_speed[2]) / 256; //round(l_speed[2]*0.7)/256;

  array[22] = 59; //new
  array[23] = l[4] % 256;
  array[24] = l[4] / 256;
  array[25] = l_speed[4] % 256;
  array[26] = l_speed[4] / 256;

  array[27] = 51; //new
  array[28] = l[5] % 256;
  array[29] = l[5] / 256;
  array[30] = l_speed[5] % 256;
  array[31] = l_speed[5] / 256;

  array[32] = 73; //new
  array[33] = r[3] % 256;
  array[34] = r[3] / 256;
  array[35] = r_speed[3] % 256;
  array[36] = r_speed[3] / 256;

  array[37] = 77; //new
  array[38] = r[1] % 256;
  array[39] = r[1] / 256;
  array[40] = (r_speed[1]) % 256; //round(r_speed[1]*0.7)%256;
  array[41] = (r_speed[1]) / 256; //round(r_speed[1]*0.7)/256;

  array[42] = 75; //new
  array[43] = r[2] % 256;
  array[44] = r[2] / 256;
  array[45] = (r_speed[2]) % 256; //round(r_speed[2]*0.7)%256;
  array[46] = (r_speed[2]) / 256; //round(r_speed[2]*0.7)/256;

  array[47] = 79;
  array[48] = r[4] % 256;
  array[49] = r[4] / 256;
  array[50] = r_speed[4] % 256;
  array[51] = r_speed[4] / 256;

  array[52] = 71;
  array[53] = r[5] % 256;
  array[54] = r[5] / 256;
  array[55] = r_speed[5] % 256;
  array[56] = r_speed[5] / 256;

  array[57] = 10; //body?
  array[58] = waist_pos % 256;		
  array[59] = waist_pos / 256;
  array[60] = 300 % 256;
  array[61] = 300 / 256;      	/////////////////

  unsigned char sum = 0x00;

  for (int i = 2; i <= 61; i++)
  {
    sum += array[i];
  }

  array[62] = ~sum;

  //PORTD |= 0x80;
  Serial1.write(array, 63);
}

void Prework(int number)
{
  if (number == 1)
  {
    unsigned char array[2 * 5 + 7 + 1];
    array[0] = array[1] = 0xFF;	//start bytes
    array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
    array[3] = (4 + 1) * 2 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
    array[4] = 0x83;			//SYNC WRITE
    array[5] = 0x1E;			//Parameter1 Start address to write Data
    array[6] = 0x04;			//Parameter2 Length of Data to write

    unsigned int hand_speed = 100;
    array[7] = 43;
    array[8] = 580 % 256;
    array[9] = 580 / 256;
    array[10] = hand_speed % 256;
    array[11] = hand_speed / 256;

    array[12] = 44;
    array[13] = 444 % 256;
    array[14] = 444 / 256;
    array[15] = hand_speed % 256;
    array[16] = hand_speed / 256;
    unsigned char sum = 0x00;
    for (int i = 2; i <= 16; i++)
    {
      sum += array[i];
    }
    array[17] = ~sum;

    //PORTD |= 0x80;

    //Serial1.write(array, 18);
  }
  if (number == 2)
  {
    unsigned char array[11 * 5 + 7 + 1];
    array[0] = array[1] = 0xFF;	//start bytes
    array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
    array[3] = (4 + 1) * 11 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
    array[4] = 0x83;			//SYNC WRITE
    array[5] = 0x1A;			//Parameter1 Start address to write Data
    array[6] = 0x04;			//Parameter2 Length of Data to write

    array[7] = 1;   //hip
    array[8] = 0x03;	//CW Margin
    array[9] = 0x03;	//CCW Margin
    array[10] = 0x08;	//CW Slope
    array[11] = 0x08;	//CCW Slope

    array[12] = 3;  //lower knee
    array[13] = 0x06;	//CW Margin
    array[14] = 0x06;	//CCW Margin
    array[15] = 0x20;	//CW Slope
    array[16] = 0x20;	//CCW Slope

    array[17] = 7;  //higher knee
    array[18] = 0x06;	//CW Margin
    array[19] = 0x06;	//CCW Margin
    array[20] = 0x20;	//CW Slope
    array[21] = 0x20;	//CCW Slope

    array[22] = 11; //ankle
    array[23] = 0x03;	//CW Margin
    array[24] = 0x03;	//CCW Margin
    array[25] = 0x08;	//CW Slope
    array[26] = 0x08;	//CCW Slope

    array[27] = 14; //turn motor
    array[28] = 0x01;	//CW Margin
    array[29] = 0x01;	//CCW Margin
    array[30] = 0x05;	//CW Slope
    array[31] = 0x05;	//CCW Slope

    array[32] = 21;
    array[33] = 0x03;	//CW Margin
    array[34] = 0x03;	//CCW Margin
    array[35] = 0x08;	//CW Slope
    array[36] = 0x08;	//CCW Slope

    array[37] = 23;
    array[38] = 0x06;	//CW Margin
    array[39] = 0x06;	//CCW Margin
    array[40] = 0x20;	//CW Slope
    array[41] = 0x20;	//CCW Slope

    array[42] = 27;
    array[43] = 0x06;	//CW Margin
    array[44] = 0x06;	//CCW Margin
    array[45] = 0x20;	//CW Slope
    array[46] = 0x20;	//CCW Slope

    array[47] = 31;
    array[48] = 0x03;	//CW Margin
    array[49] = 0x03;	//CCW Margin
    array[50] = 0x08;	//CW Slope
    array[51] = 0x08;	//CCW Slope

    array[52] = 34;	//turn motor
    array[53] = 0x01;	//CW Margin
    array[54] = 0x01;	//CCW Margin
    array[55] = 0x05;	//CW Slope
    array[56] = 0x05;	//CCW Slope

    array[57] = 10;	//waist motor
    array[58] = 0x01;	//CW Margin
    array[59] = 0x01;	//CCW Margin
    array[60] = 0x05;	//CW Slope
    array[61] = 0x05;	//CCW Slope

    unsigned char sum = 0x00;
    for (int i = 2; i <= 61; i++)
    {
      sum += array[i];
    }
    array[62] = ~sum;

    //PORTD |= 0x80;
    //Serial1.write(array, 63);
  }
  if (number == 3)
  {
    unsigned char array[11 * 2 + 7 + 1];
    array[0] = array[1] = 0xFF;	//start bytes
    array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
    array[3] = (1 + 1) * 11 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
    array[4] = 0x83;			//SYNC WRITE
    array[5] = 0x1c;			//Parameter1 Start address to write Data
    array[6] = 0x02;			//Parameter2 Length of Data to write

    array[7] = 1;   //hip
    array[8] = 36;	//P

    array[9] = 3;  //lower knee
    array[10] = 32;

    array[11] = 7;  //higher knee
    array[12] = 32;

    array[13] = 11; //ankle
    array[14] = 36;

    array[15] = 14; //turn motor
    array[16] = 32;

    array[17] = 21;
    array[18] = 36;

    array[19] = 23;
    array[20] = 32;

    array[21] = 27;
    array[22] = 32;

    array[23] = 31;
    array[24] = 36;

    array[25] = 34;	//turn motor
    array[26] = 32;

    array[27] = 10;	//waist motor
    array[28] = 32;

    unsigned char sum = 0x00;
    for (int i = 2; i <= 28; i++)
    {
      sum += array[i];
    }
    array[29] = ~sum;

    //PORTD |= 0x80;
    //Serial1.write(array, 30);
  }
}

float Gyro_Filter_2(int init_value)
{
  float currentValue = 0;
  static float lastValue = 0;
  static float filterConstant = 1 - exp(-0.01 / 0.09); // KF = 1-e**(-Looptime/T)  where T = Time Constant of the filter

  currentValue = (gx - init_value) * 0.00875;
  if (fabs(currentValue) <= 1)
    currentValue = 0;

  currentValue = lastValue + filterConstant * (currentValue - lastValue);
  lastValue = currentValue;

  return currentValue;
}

int initialGyro()
{
  int temp[20];
  int temp_sum = 0;
  int i;
  int init_roll;

  AHRSInit();
  RazorInit();

  for (i = 0; i < 20; i++)
  {
    temp[i] = gx;
    temp_sum += temp[i];
  }
  init_roll = temp_sum / 20;

  return init_roll;
}

float FeedBackControl()
{
  static float lastValue = 0;
  static float currentValue = 0;
  float deltaValue = 0;
  float offset = 0;
  float kp = 0.5;
  float kd = 0.075;
  lastValue = currentValue;
  currentValue = Gyro_Filter_2(gyro_init);

  deltaValue = currentValue - lastValue;

  offset = (currentValue * kp) + (deltaValue * kd);
  return offset;
}

int ReadTemp(unsigned char id)
{
  unsigned char data[8];
  int i;
  int packet;
  unsigned char pack[20];
  int temp;

  data[0] = 0xff;
  data[1] = 0xff;
  data[2] = id;
  data[3] = 0x04;
  data[4] = 0x02;
  data[5] = 0x2B;
  data[6] = 0x01;
  data[7] = 0;
  for (i = 2; i < 7; i++)
  {
    data[7] = data[7] + data[i];
  }
  data[7] = (~data[7]) % 256;

  //PORTD |= 0x80;//PORTD = 0x80;

  Serial1.flush();
  //cli();
  for (i = 0; i < 8; i++)
  {
    //while ((UCSR1A & (1 << UDRE1)) == 0)
    {
    };

    //UCSR1A |= 0x40;
    //UDR1 = data[i];
  }
  //while ( !bit_is_set(//UCSR1A, 6) );
  //sei();

  /*There is a problem!!! Maybe it was interrupted by other things before the data was delivered
   	for(i = 0; i <= 7; i++)
   	{
   	 	Serial1.print(data[i],BYTE);
   	}
   */
  //PORTD = 0x00;//PORTD &= 0x7F;//PORTD = 0x00;


  while ( (Serial1.available() < 8))
  {
  };
  for (i = 0; i < 8; i++)
  {
    pack[i] = Serial1.read();
    //Serial.println(pack[i],HEX);
  }

  //PORTD |= 0x80;
  if (pack[3 + 1] == 0x03)
    return pack[5 + 1];
  else
    return 0;
}

void Compute_FCBGSI(bool sel_foot, int i_step)
{
  double vel = Angular_Vel_Y;
  static double ang_vel_left[51] = {
    0.0
  };
  static double ang_vel_right[51] = {
    0.0
  };
  if (sel_foot == 1)  ang_vel_left[i_step] = vel;
  else ang_vel_right[i_step] = vel;

  if (i_step == 50)
  {
    if (sel_foot == 1)
    {
      double avg_ang_vel_left = 0.0;
      for (int i = 0; i < 51; i++)
      {
        avg_ang_vel_left += ang_vel_left[i];
        ang_vel_left[i] = 0.0;
      }
      FCBGSI_L = avg_ang_vel_left / 51.0;
    }
    else
    {
      double avg_ang_vel_right = 0.0;
      for (int i = 0; i < 51; i++)
      {
        avg_ang_vel_right += ang_vel_right[i];
        ang_vel_right[i] = 0.0;
      }
      FCBGSI_R = avg_ang_vel_right / 51.0;
      FCBGSI = FCBGSI_R - FCBGSI_L;
      //Update_f_LIPMZc();
    }
  }
}

void Update_f_LIPMZc()
{
  double ideal_FCBGSI = 0.0;//change
  double error = FCBGSI - ideal_FCBGSI;
  if (fabs(error) > 1.0)
  {
    if (error > 0) //decreaese f_LIPMZc
    {
      f_LIPMZc = f_LIPMZc - 1.0 * fabs(error);
    }
    else //increase f_LIPMZc
    {
      f_LIPMZc = f_LIPMZc + 1.0 * fabs(error);
    }
  }
  if (f_LIPMZc > 90.0) f_LIPMZc = 100.0; //change
  else if (f_LIPMZc < 58.0) f_LIPMZc = 50.0; //change
}
void Serial_Communication()
{
  // Serial.println(FCBGSI);
  //Serial.println(f_LIPMZc);
}
void Get_Roll_Pitch_Accelerometer(double *roll, double *pitch)
{
  accelerometer.read();
  accelerometer.a.ax = accelerometer.a.x;
  accelerometer.a.ay = accelerometer.a.y;
  accelerometer.a.az = accelerometer.a.z;
  (*roll)  = atan2(-accelerometer.a.ay, accelerometer.a.az) * RAD_TO_DEG;
  (*pitch) = atan(accelerometer.a.ax / sqrt(accelerometer.a.ay * accelerometer.a.ay + accelerometer.a.az * accelerometer.a.az)) * RAD_TO_DEG;
}

void Get_Gyro_Rate(double *gyroXrate , double *gyroYrate)
{
  gyroscope.read();
  (*gyroXrate) = (gyroscope.g.x + 155) * 0.00381;
  (*gyroYrate) = (gyroscope.g.y - 95) * 0.00381;
}

void Gait_BigRobot_Arm(int speed) {
  byte array[108];

  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (4 + 1) * 8 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x1E;			//Parameter1 Start address to write Data
  array[6] = 0x04;			//Parameter2 Length of Data to write

  array[7] = 15;//3
  array[8] = int((r_hand) / 360.0 * 4096.0) % 256;
  array[9] = int((r_hand) / 360.0 * 4096.0) / 256;
  array[10] = speed % 256;
  array[11] = speed / 256;

  array[12] = 16;
  array[13] = int((l_hand) / 360.0 * 4096.0) % 256;
  array[14] = int((l_hand) / 360.0 * 4096.0) / 256;
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

//==================================================arm======================================================================================
void Gait_BigRobot_Arm_backup(int Arm_speed)
{
  unsigned char array[6 * 5 + 7 + 1];
  unsigned int r_arm[4];
  unsigned int l_arm[4];
  unsigned int r_arm_speed[4];
  unsigned int l_arm_speed[4];
  ///////////////////////arm first///////////////////////////////////////////////
  left_arm.motor_degree[0] = left_arm.motor_degree[0]  + L_Shoulder_1; //shoulder_same as elbow180~360 11
  left_arm.motor_degree[1] = left_arm.motor_degree[1]  + L_Shoulder_2; //shoulder 12
  left_arm.motor_degree[2] = left_arm.motor_degree[2]  + L_Elbow; //elbow180~360 13

  right_arm.motor_degree[0] = right_arm.motor_degree[0] + R_Shoulder_1;
  right_arm.motor_degree[1] = right_arm.motor_degree[1] + R_Shoulder_2;
  right_arm.motor_degree[2] = right_arm.motor_degree[2] + R_Elbow;
  //////////////////////////////////////////////////////////////////////
  l_arm[1] = round((left_arm.motor_degree[0] / 360.0 * 4096.0)); //shoulder_same as elbow180~360 11
  l_arm[2] = round((left_arm.motor_degree[1] / 360.0 * 4096.0)); //shoulder 12
  l_arm[3] = round((left_arm.motor_degree[2] / 360.0 * 4096.0)); //elbow180~360 13

  r_arm[1] = round((right_arm.motor_degree[0] / 360.0 * 4096.0)); //shoulder_same as elbow180~360
  r_arm[2] = round((right_arm.motor_degree[1] / 360.0 * 4096.0)); //shoulder
  r_arm[3] = round((right_arm.motor_degree[2] / 360.0 * 4096.0)); //elbow180~360

    for (int i = 1; i <= 3; i++)
  {
    l_arm_speed[i] = Arm_speed;
    r_arm_speed[i] = Arm_speed;
  }

  array[0] = array[1] = 0xFF;	//start bytes
  array[2] = 0xFE;  		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
  array[3] = (4 + 1) * 6 + 4;		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
  array[4] = 0x83;			//SYNC WRITE
  array[5] = 0x1E;			//Parameter1 Start address to write Data
  array[6] = 0x04;			//Parameter2 Length of Data to write

  array[7] = 11;
  array[8] = l_arm[1] % 256;
  array[9] = l_arm[1] / 256;
  array[10] = l_arm_speed[1] % 256;
  array[11] = l_arm_speed[2] / 256;

  array[12] = 12;
  array[13] = l_arm[2] % 256;
  array[14] = l_arm[2] / 256;
  array[15] = l_arm_speed[2] % 256;
  array[16] = l_arm_speed[2] / 256;

  array[17] = 13;
  array[18] = l_arm[3] % 256;
  array[19] = l_arm[3] / 256;
  array[20] = l_arm_speed[3] % 256;
  array[21] = l_arm_speed[3] / 256;

  array[22] = 21;
  array[23] = r_arm[1] % 256;
  array[24] = r_arm[1] / 256;
  array[25] = r_arm_speed[1] % 256;
  array[26] = r_arm_speed[2] / 256;

  array[27] = 22;
  array[28] = r_arm[2] % 256;
  array[29] = r_arm[2] / 256;
  array[30] = r_arm_speed[2] % 256;
  array[31] = r_arm_speed[2] / 256;

  array[32] = 23;
  array[33] = r_arm[3] % 256;
  array[34] = r_arm[3] / 256;
  array[35] = r_arm_speed[3] % 256;
  array[36] = r_arm_speed[3] / 256;

  unsigned char sum = 0x00;

  for (int i = 2; i <= 36; i++)
  {
    sum += array[i];
  }

  array[37] = ~sum;

  //PORTD |= 0x80;
  Serial1.write(array, 38);
}
void Arm_walk(bool sel_foot, int i_step)
{
  double Arm_Goal_L1 = Arm_initial_11 + 12.5;
  double Arm_Goal_R1 = Arm_initial_21 + 12.5;

  double Arm_Goal_L2 = Arm_initial_11 - 12.5;
  double Arm_Goal_R2 = Arm_initial_21 - 12.5;
  int t1 = 0;

  if (sel_foot == 1) //means left foot is swing foot
  {
    if (i_step == 0)
    {
      left_arm.motor_degree[0] = Arm_initial_11;      //return to original position
      right_arm.motor_degree[0] = Arm_initial_21;
    }
    else if (i_step <= 25)
    {
      left_arm.motor_degree[0] = Trajectory_Planning(i_step, Arm_initial_11, Arm_Goal_L1, 0, 0, 25);
      right_arm.motor_degree[0] = Trajectory_Planning(i_step, Arm_initial_21, Arm_Goal_R1, 0, 0, 25);
    }
    else
    {
      t1 = i_step - 25;
      left_arm.motor_degree[0] = Trajectory_Planning(t1, Arm_Goal_L1, Arm_initial_11,  0, 0, 25);
      right_arm.motor_degree[0] = Trajectory_Planning(t1, Arm_Goal_R1, Arm_initial_21,  0, 0, 25);
    }
  }
  else//means right foot is swing foot
  {
    if (i_step == 0)
    {
      left_arm.motor_degree[0] = Arm_initial_11;      //return to original position
      right_arm.motor_degree[0] = Arm_initial_21;
    }
    else if (i_step <= 25)
    {
      left_arm.motor_degree[0] = Trajectory_Planning(i_step, Arm_initial_11, Arm_Goal_L2, 0, 0, 25);
      right_arm.motor_degree[0] = Trajectory_Planning(i_step, Arm_initial_21, Arm_Goal_R2, 0, 0, 25);

    }
    else
    {
      t1 = i_step - 25;
      left_arm.motor_degree[0] = Trajectory_Planning(t1, Arm_Goal_L2, Arm_initial_11, 0, 0, 25);
      right_arm.motor_degree[0] = Trajectory_Planning(t1, Arm_Goal_R2, Arm_initial_21, 0, 0, 25);
    }
  }
  if (i_step == 50)
  {
    left_arm.motor_degree[0] = Arm_initial_11;      //return to original position
    right_arm.motor_degree[0] = Arm_initial_21;
  }
}
void Arm_kick(bool sel_foot, int kick_state, float kick_period, int t_kick)
{
  double Arm_Goal_L1 = Arm_initial_11 - 22.5;//
  double Arm_Goal_R1 = Arm_initial_21 - 22.5;
  double Arm_Goal_L2 = Arm_initial_11 + 22.5;//
  double Arm_Goal_R2 = Arm_initial_21 + 22.5;//+12.5


    //t1  center of gravity offset
  //t2  kick

  //left kick
  double L_t1_22 = Arm_initial_22 - 40.0;    //When the center of gravity is to the right, the right hand is up
  double L_t1_23 = Arm_initial_23 + 82;       //When the center of gravity is to the right, straighten the right hand
  double L_t2_11 = Arm_Goal_L2 + 10.0;       //When kicking left, the left hand is backward

  //right kick
  double R_t1_12 = Arm_initial_12 + 40.0;    //When the center of gravity is to the left, the left hand is up
  double R_t1_13 = Arm_initial_13 - 82;      //When the center of gravity is to the left, the left hand is straight
  double R_t2_21 = Arm_Goal_R1 - 10.0;       //When kicking right, the right hand goes backwards


  if (sel_foot == 0) //left kick
  {
    if (kick_state == 2) //center of gravity to the right
    {
      left_arm.motor_degree[0] = Trajectory_Planning(t_kick, Arm_initial_11, Arm_Goal_L1, 0, 0, kick_period);
      right_arm.motor_degree[0] = Trajectory_Planning(t_kick, Arm_initial_21, Arm_Goal_R1, 0, 0, kick_period);
      right_arm.motor_degree[1] = Trajectory_Planning(t_kick, Arm_initial_22, L_t1_22, 0, 0, kick_period);
      right_arm.motor_degree[2] = Trajectory_Planning(t_kick, Arm_initial_23, L_t1_23, 0, 0, kick_period);
    }

    else if (kick_state == 5) //left kicki
    {
      left_arm.motor_degree[0] = Trajectory_Planning(t_kick, Arm_Goal_L1,  L_t2_11, 0, 0, kick_period);
      right_arm.motor_degree[0] = Trajectory_Planning(t_kick, Arm_Goal_R1, Arm_Goal_R2, 0, 0, kick_period);
    }
    else if (kick_state == 8) //center of gravity back
    {
      left_arm.motor_degree[0] = Trajectory_Planning(t_kick, L_t2_11, Arm_initial_11, 0, 0, kick_period);
      right_arm.motor_degree[0] = Trajectory_Planning(t_kick, Arm_Goal_R2, Arm_initial_21, 0, 0, kick_period);
      right_arm.motor_degree[1] = Trajectory_Planning(t_kick, L_t1_22, Arm_initial_22,  0, 0, kick_period);
      right_arm.motor_degree[2] = Trajectory_Planning(t_kick, L_t1_23, Arm_initial_23,  0, 0, kick_period);
    }
  }

  else//right kick
  {
    if (kick_state == 2) //Center of gravity to the left
    {
      left_arm.motor_degree[0] = Trajectory_Planning(t_kick, Arm_initial_11, Arm_Goal_L2, 0, 0, kick_period);
      right_arm.motor_degree[0] = Trajectory_Planning(t_kick, Arm_initial_21, Arm_Goal_R2, 0, 0, kick_period);
      left_arm.motor_degree[1] = Trajectory_Planning(t_kick, Arm_initial_12, R_t1_12, 0, 0, kick_period);
      left_arm.motor_degree[2] = Trajectory_Planning(t_kick, Arm_initial_13, R_t1_13, 0, 0, kick_period);
    }

    else if (kick_state == 5) //right kick
    {
      left_arm.motor_degree[0] = Trajectory_Planning(t_kick, Arm_Goal_L2,  Arm_Goal_L1, 0, 0, kick_period);
      right_arm.motor_degree[0] = Trajectory_Planning(t_kick, Arm_Goal_R2, R_t2_21, 0, 0, kick_period);
    }
    else if (kick_state == 8) //center of gravity back
    {
      left_arm.motor_degree[0] = Trajectory_Planning(t_kick, Arm_Goal_L1, Arm_initial_11, 0, 0, kick_period);
      right_arm.motor_degree[0] = Trajectory_Planning(t_kick, R_t2_21, Arm_initial_21, 0, 0, kick_period);
      left_arm.motor_degree[1] = Trajectory_Planning(t_kick, R_t1_12, Arm_initial_12, 0, 0, kick_period);
      left_arm.motor_degree[2] = Trajectory_Planning(t_kick, R_t1_13, Arm_initial_13, 0, 0, kick_period);
    }
  }

}
double Trajectory_Planning(double t, double q0, double q1, double v0, double v1, double tf)
{
  //q0 initial position
  //q1 target location
  //v0 Initial velocity
  //v1 target speed
  double a_0;
  double a_1;
  double a_2;
  double a_3;
  double positionXYZ;

  a_0 = q0;
  a_1 = v0;
  a_2 = (3 * (q1 - q0) - (2 * v0 + v1) * tf) / (tf * tf);
  a_3 = (2 * (q0 - q1) + (v0 + v1) * tf) / (tf * tf * tf);
  return positionXYZ = ((a_0 + a_1 * t + a_2 * t * t + a_3 * t * t * t));
}

//=============================Squat====================================================================================================================
void squat(int Squat_state)
{
  //Squat_state      0:squat down  1:stand up
  static int t_squat = 0;                  
  static int squat_state = 1;              
  static float l_temp_turn = 0;            
  static float r_temp_turn = -theta_flag;            
  static float shift_to_middle = 0;        

  float squat_period[6];
  squat_period[0] = 5.0 * 4; 
  squat_period[1] = 10.0 * 4; 
  squat_period[2] = 10.0 * 15; 

  squat_period[3] = 1;

  float Test_Height = -18.0;               
  float Hip_turn = 10.0;                   

  if ( Squat_state == 0) //0:squat down
  {
    if (squat_state == 1 && t_squat == 0 )
    {
      shift_to_middle = (l_hip.gol[1] + r_hip.gol[1]) / 2.0;
      for (int i = 0; i <= 1; i++)
      {
        l_bottom.gol[i] = l_foot.gol[i];
        r_bottom.gol[i] = r_foot.gol[i];
      }
    }
    if (squat_state == 1) 
    {
      if (t_squat < squat_period[0])
      {
        l_hip.gol[1] = l_hip.gol[1] - shift_to_middle / squat_period[0];
        r_hip.gol[1] = r_hip.gol[1] - shift_to_middle / squat_period[0];
        t_squat++;
      }
      else if (t_squat >= squat_period[0])
      {
        t_squat = 0;
        squat_state = 2;
      }
      inv_kine_Chou(0, r_temp_turn, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      Gait_BigRobot(0, 1023);
    }

    else if (squat_state == 2)
    {
      if (t_squat < squat_period[2])
      {
        l_hip.gol[2] = l_hip.gol[2] + Test_Height / squat_period[2];
        r_hip.gol[2] = r_hip.gol[2] + Test_Height / squat_period[2];
        t_squat++;
      }
      else if (t_squat >= squat_period[2])
      {
        t_squat = 0;
        squat_state = 0;
        //Robot_state = 44;
        squatPosition = true;
      }

      inv_kine_Chou(0, r_temp_turn, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      Gait_BigRobot(0, 1023);
    }
  }//if( Squat_state == 0)
  //berdiri
  if ( Squat_state == 1) //1:stand up
  {
    if (squat_state == 0) //stand up
    {
      if (t_squat < squat_period[3])
      {
        l_hip.gol[2] = l_hip.gol[2] - (Test_Height + 5) / squat_period[3];
        r_hip.gol[2] = r_hip.gol[2] - (Test_Height + 5)/ squat_period[3];
        t_squat++;
      }
      else if (t_squat >= squat_period[3])
      {
        t_squat = 0;
        squat_state = 1;
        Robot_state = 1;
        squatPosition = false;
      }
      inv_kine_Chou(0, r_temp_turn, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      Gait_BigRobot(0, 1023);
    }
  }    
}
void  SerialPrintArM()
{
  //Serial.print(left_arm.motor_degree[0]);
  //Serial.print("\t");
  //Serial.println(right_arm.motor_degree[0]);
}
void Squat_Pre()
{

  float RK_offset_ankle = -11.0;		
  float RK_offset_hip  =   2.0;		
  float LK_offset_ankle =  -12.5;		
  float LK_offset_hip  =   2.0;		

  float foort_raise    =  12.0;//11.0	
  float shift_right    =  9.7;//9.7	/
  float shift_left     =  -10.5;//-10.5   
  float Lextend_offset_hip  =  -8.0;	
  float Lextend_offset_ankle  =  -8.0;	
  float Rextend_offset_hip  =  -8.0;	
  float Rextend_offset_ankle  =  0.0;	

  bool wait_flag = 0;			
  static float temp_m0 = 0, temp_m3 = 0;	
  static float shift_to_middle = 0;		
  static float extend_temp_m0 = 0, extend_temp_m3 = 0;	
  static int t_kick = 0;
  static int kick_state = 1;

  float kick_period[8];
  kick_period[0] = 15.0 * 4;
  kick_period[1] = 15.0 * 4;
  kick_period[2] = 10.0 * 2; //10  Hip_turn & lift left foot
  kick_period[3] = 10.0 * 4;
  kick_period[5] = 10.0 * 4;
  kick_period[6] = 30.0 * 4;
  kick_period[7] = 15.0 * 4;



  static float l_temp_turn = 0;            
  static float r_temp_turn = 0;            
  float Hip_turn = 10.0;                   



  if (kick_state == 1 && t_kick == 0 )
  {
    shift_to_middle = (l_hip.gol[1] + r_hip.gol[1]) / 2.0;
    for (int i = 0; i <= 1; i++)
    {
      l_bottom.gol[i] = l_foot.gol[i];
      r_bottom.gol[i] = r_foot.gol[i];
    }
    l_bottom.gol[2] = l_foot.gol[2] - SHOE_HEIGHT;
    r_bottom.gol[2] = r_foot.gol[2] - SHOE_HEIGHT;
  }


  if (kick_state == 1) 
  {
    if (t_kick < kick_period[0])
    {
      l_hip.gol[1] = l_hip.gol[1] - shift_to_middle / kick_period[0];
      r_hip.gol[1] = r_hip.gol[1] - shift_to_middle / kick_period[0];
      t_kick++;
    }
    else if (t_kick >= kick_period[0])
    {
      t_kick = 0;
      kick_state = 2;
    }
    inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
    inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
    Gait_BigRobot(0, 1023);
  }
  else if (kick_state == 2) 
  {
    if (t_kick < kick_period[1])
    {
      l_hip.gol[1] = l_hip.gol[1] + shift_right / kick_period[1];
      r_hip.gol[1] = r_hip.gol[1] + shift_right / kick_period[1];
      t_kick++;
    }
    else if (t_kick >= kick_period[1])
    {
      wait_flag = wait_time_function(500);
      if (wait_flag == 1)
      {
        t_kick = 0;
        kick_state = 3;
        temp_m0 = right_foot.motor_degree[0]; //
        temp_m3 = right_foot.motor_degree[3]; //hip
      }
    }
    inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
    inv_kine_Chou(0, 0, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
    Gait_BigRobot(0, 1023);

  }
  else if (kick_state == 3) 
  {
    if (t_kick < kick_period[2])
    {

      l_bottom.gol[2] = l_bottom.gol[2] + foort_raise / kick_period[2];

      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, l_temp_turn, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);

      temp_m0 = temp_m0 - LK_offset_ankle / kick_period[2];
      temp_m3 = temp_m3 + LK_offset_hip / kick_period[2];
      right_foot.motor_degree[0] = temp_m0; //
      right_foot.motor_degree[3] = temp_m3; //hip
      t_kick++;
    }
    else if (t_kick >= kick_period[2])
    {
      wait_flag = wait_time_function(1000);
      if (wait_flag == 1)
      {
        t_kick = 0;
        kick_state = 4;
        extend_temp_m0 = left_foot.motor_degree[0];
        extend_temp_m3 = left_foot.motor_degree[3];
      }
    }
    Gait_BigRobot(0, 400);
  }

  else if (kick_state == 4) //hip_turn
  {
    if (t_kick < kick_period[2])
    {

      l_temp_turn = l_temp_turn + Hip_turn / kick_period[2]; //hip_turn
      inv_kine_Chou(0, l_temp_turn, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
      t_kick++;
    }
    else if (t_kick >= kick_period[2])
    {
      wait_flag = wait_time_function(2000);
      if (wait_flag == 1)
      {
        t_kick = 0;
        kick_state = 5;
      }
    }
    Gait_BigRobot(0, 400);

  }
  else if (kick_state == 5) //
  {
    if (t_kick < kick_period[6])
    {

      l_bottom.gol[2] = l_bottom.gol[2] - foort_raise / kick_period[6];

      inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, l_temp_turn, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);

      temp_m0 = temp_m0 + LK_offset_ankle / kick_period[6];
      temp_m3 = temp_m3 - LK_offset_hip / kick_period[6];

      right_foot.motor_degree[0] = temp_m0; //
      right_foot.motor_degree[3] = temp_m3; //hip

      extend_temp_m0 -= Lextend_offset_ankle / kick_period[6]; //
      extend_temp_m3 -= Lextend_offset_hip / kick_period[6];
      left_foot.motor_degree[0] = extend_temp_m0;
      left_foot.motor_degree[3] = extend_temp_m3;

      t_kick++;
    }
    else if (t_kick >= kick_period[6])
    {
      wait_flag = wait_time_function(500);
      if (wait_flag == 1)
      {
        t_kick = 0;
        kick_state = 6;
      }
    }
    Gait_BigRobot(0, 1023);
  }
  else if (kick_state == 6) //
  {
    if (t_kick < kick_period[7])
    {
      l_hip.gol[1] = l_hip.gol[1] - (shift_right - shift_to_middle) / kick_period[7];
      r_hip.gol[1] = r_hip.gol[1] - (shift_right - shift_to_middle) / kick_period[7];
      t_kick++;
    }
    else if (t_kick >= kick_period[7])
    {
      wait_flag = wait_time_function(500);
      if (wait_flag == 1)
      {
        t_kick = 0;
        kick_state = 7;
        shift_to_middle = 0;
        temp_m0 = 0;
        temp_m3 = 0;
        extend_temp_m0 = 0;
        extend_temp_m3 = 0;

      }
    }
    inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
    inv_kine_Chou(0, l_temp_turn, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
    Gait_BigRobot(0, 80);
  }

  else if (kick_state == 7) 
  {
    if (t_kick < kick_period[1])
    {
      l_hip.gol[1] = l_hip.gol[1] + shift_left / kick_period[1];
      r_hip.gol[1] = r_hip.gol[1] + shift_left / kick_period[1];
      t_kick++;
    }
    else if (t_kick >= kick_period[1])
    {
      wait_flag = wait_time_function(500);
      if (wait_flag == 1)
      {
        t_kick = 0;
        kick_state = 8;
        temp_m0 = left_foot.motor_degree[0];
        temp_m3 = left_foot.motor_degree[3];
      }
    }
    inv_kine_Chou(0, 0, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
    inv_kine_Chou(0, l_temp_turn, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
    Gait_BigRobot(0, 1023);

  }
  else if (kick_state == 8) 
  {
    if (t_kick < kick_period[2])
    {
      r_bottom.gol[2] = r_bottom.gol[2] + foort_raise / kick_period[2];
      inv_kine_Chou(0, r_temp_turn, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, l_temp_turn, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);

      temp_m0 = temp_m0 + RK_offset_ankle / kick_period[2];
      temp_m3 = temp_m3 - RK_offset_hip / kick_period[2];
      left_foot.motor_degree[0] = temp_m0; //
      left_foot.motor_degree[3] = temp_m3;

      t_kick++;
    }
    else if (t_kick >= kick_period[2])
    {
      wait_flag = wait_time_function(1000);
      if (wait_flag == 1)
      {
        t_kick = 0;
        kick_state = 9;
        extend_temp_m0 = right_foot.motor_degree[0];
        extend_temp_m3 = right_foot.motor_degree[3];
      }
    }
    Gait_BigRobot(0, 400);
  }
  else if (kick_state == 9) 
  {
    if (t_kick < kick_period[2])
    {

      r_temp_turn = r_temp_turn - Hip_turn / kick_period[2];               
      inv_kine_Chou(0, r_temp_turn, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      t_kick++;
    }
    else if (t_kick >= kick_period[2])
    {
      wait_flag = wait_time_function(1000);
      if (wait_flag == 1)
      {
        t_kick = 0;
        kick_state = 10;
      }
    }
    Gait_BigRobot(0, 400);
  }

  else if (kick_state == 10) //
  {
    if (t_kick < kick_period[6])
    {
      r_bottom.gol[2] = r_bottom.gol[2] - foort_raise / kick_period[6];

      inv_kine_Chou(0, r_temp_turn, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
      inv_kine_Chou(0, l_temp_turn, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);

      temp_m0 = temp_m0 - RK_offset_ankle / kick_period[6];
      temp_m3 = temp_m3 + RK_offset_hip / kick_period[6];
      left_foot.motor_degree[0] = temp_m0; //
      left_foot.motor_degree[3] = temp_m3;

      extend_temp_m0 += Rextend_offset_ankle / kick_period[6]; //
      extend_temp_m3 += Rextend_offset_hip / kick_period[6];
      right_foot.motor_degree[0] = extend_temp_m0;
      right_foot.motor_degree[3] = extend_temp_m3;

      t_kick++;
    }
    else if (t_kick >= kick_period[6])
    {
      wait_flag = wait_time_function(500);
      if (wait_flag == 1)
      {
        t_kick = 0;
        kick_state = 11; //8
      }
    }
    Gait_BigRobot(0, 1023);
  }
  else if (kick_state == 11) //
  {
    if (t_kick < kick_period[7])
    {
      l_hip.gol[1] = l_hip.gol[1] - (shift_left - shift_to_middle) / kick_period[7];
      r_hip.gol[1] = r_hip.gol[1] - (shift_left - shift_to_middle) / kick_period[7];
      t_kick++;
    }
    else if (t_kick >= kick_period[7])
    {
      wait_flag = wait_time_function(500);
      if (wait_flag == 1)
      {
        t_kick = 0;
        kick_state = 1;
        shift_to_middle = 0;
        temp_m0 = 0;
        temp_m3 = 0;
        extend_temp_m0 = 0;
        extend_temp_m3 = 0;
        Robot_state = 4;//Stop
      }
    }
    inv_kine_Chou(0, r_temp_turn, r_hip.gol, r_bottom.gol, 0,  right_foot.motor_degree);
    inv_kine_Chou(0, l_temp_turn, l_hip.gol, l_bottom.gol, 1,  left_foot.motor_degree);
    Gait_BigRobot(0, 80);
  }

}

void BubbleSort(int count)
{
  for (int i = 0; i < count; i++)
  {
    for (int j = 1; j < count - i; j++)
    {
      if (Compass_Array[j] < Compass_Array[j - 1])
      {
        float temp = Compass_Array[j];
        Compass_Array[j] = Compass_Array[j - 1];
        Compass_Array[j - 1] = temp;
      }
    }
  }
}

void Compute_Compass(int state, bool sel_foot, int i_step)
{
  Send_heading = Heading;
  //  if(state != 2)
  //  {
  ////    Serial.println("Stop");
  //    Count_Compass_Step = 0;
  //    Compass_Array[Count_Compass_Stop] = Heading;
  //    Count_Compass_Stop++;
  //    if(Count_Compass_Stop > 10)
  //    {
  //      BubbleSort(Count_Compass_Stop);
  //      Send_heading = Compass_Array[Count_Compass_Stop/2];
  ////      Serial.println(Count_Compass_Stop);
  ////      Serial.println("Heading");
  ////      Serial.println(Heading);
  ////      Serial.println("Send_heading");
  ////      Serial.println(Send_heading);
  //      Count_Compass_Stop = 0;
  //    }
  //  }
  //  else
  //  {
  //    Count_Compass_Stop = 0;
  //
  //    int tmp = i_step % 10;
  ////    Serial.print("i_step ");
  ////    Serial.println(i_step);
  ////    Serial.print(" tmp ");
  ////    Serial.println(tmp);
  //    if(tmp == 0)
  //    {
  //      Compass_Array[Count_Compass_Step] = Heading;
  //      Count_Compass_Step++;
  ////      Serial.print("Count_Compass_Step ");
  ////      Serial.println(Count_Compass_Step);
  //    }
  //    if(sel_foot == 0 && i_step == 50)
  //    {
  //      BubbleSort(Count_Compass_Step);
  //      Send_heading = Compass_Array[Count_Compass_Step/2];
  //      Count_Compass_Step = 0;
  //    }
  //  }
}







