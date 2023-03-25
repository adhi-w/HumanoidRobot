///////////// parameters of mechanical ///////////////////////////////////////
//#define GAITWIDTH 29.0          //left and right hip distance
//#define THIGH 25.0	 	//a5 
//#define LEG 25.0         	//a3
////-----------------------------------------------------------------------------------------------------------------
//#define SHOE_HEIGHT   4.9	//a1 Distance from the ground to the ankle roll joint
//#define FOOT_GAP 4.1     	//a2 Distance from ankle roll joint to ankle pitch joint
//#define HIP_GAP 4.1     	//a6 Distance from hip pitch joint to hip roll joint
//#define HIP_UP_GAP 6.95  	//a7 Distance from hip roll joint to hip plane (HIP_HEIGHT position)
//#define KNEE_GAP 10.06     	//A4 upper and lower knee distance
////-----------------------------------------------------------------------------------------------------------------

#define SAFE_FALL_ANGLE 25
#define SAFE_FALL_ANGLE_PITCH 25
#define GAITWIDTH 15.0

#define THIGH 14.0	 	//a5??
#define LEG 14.0         	//a3??
#define SHOE_HEIGHT  4.8	//a1?????roll????
#define FOOT_GAP 6.6     	//a2??roll?????pitch????
#define HIP_GAP 2.0     	//a6?pitch????roll????
#define HIP_UP_GAP 5.8  	//a7?roll??????(HIP_HEIGHT??)??
#define KNEE_GAP 4.1            //a4上下膝蓋間距
#define HIP_HEIGHT  49.0//48.0

///////////// parameters of LIPM & HLIPM /////////////////////////////////////
#define FOOT_TRAJ_PERCENTHIGH 0.5//When the height trajectory of the end point reaches the highest point, the time ratio in one step
#define G_ACC_INIT 900//550//600//630.0//650.0//690.0//720.0//750.0	//LIPM parameter, which affects the size of the left and right swings, used when starting (big when the vibration is large, small when the vibration is small)
#define G_ACC_STABLE 950 //1000//1150//720//840.0//880.0//860.0//930.0//950.0    //LIPM parameter, which affects the size of the left and right swings, and is used when it is stable (when it is large, the vibration is large, and when it is small, the vibration is small)

#define LIPMZc 26//33.5//50.0//67.0//60.0		//The LIPM parameter affects the size of the left and right swings. Note that it must be a floating point number.
#define HIP_HEIGHT_AMP  1.5	//HIP_HEIGHT~(HIP_HEIGHT+HIP_HEIGHT_AMP)
#define Plane_Theta_AMP  1.5//1.3//1.5	//-Plane_Theta_AMP~+Plane_Theta_AMP unit:degree
//Warring! (GAITWIDTH/2.0)*sin(Plane_Theta_AMP) + HIP_HEIGHT < 79.0 max(78.9)


///////////// parameters of system ///////////////////////////////////////////
#define TIMER_CYCLETIME 18000  //14000   us 	
#define CYCLE_STABLE 34   	//A step is divided into CYCLE_STABLE timer operations
#define SWING_FOOT_FIRST 1	//1=lift the left foot first, 0 lift the right foot first
#define CHANGE_GAIT_STEP 2	//number of strides at start,be positive, Step_n>CHANGE_GAIT_STEP can change length,turn,yshift
//#define HIP_HEIGHT   76.0  	//hip base height79.0 max(78.9)(cm)
#define GAITHEIGHT_INIT  1.5//3.0//4.2	//Foot lift height, used when starting (cm)
#define GAITHEIGHT_STABLE 5.0//7.0//5.5//5.0	/Foot lift height, used when stable (cm)
#define HIP_X_OFFSET 0.0//1.0//0.0	//Adjust the front and rear translation of the upper bodycm)
#define FOOT_POS_OFFSET 0.5//-4.7	//Adjust the offset for the front and rear feet, so that the end point of the right foot moves back and forth(cm)
#define FOOT_SHIFT_DIS -0.2//0.0//-0.8	//For internal or external adjustment offset(cm)
#define WAIT_TIME_STABLE 1	//the total wait time equal to TIMER_CYCLETIME*WAIT_TIME_STABLE	,if WAIT_TIME_START=2 two more cycle at the end delay
#define ACCW_STABLE 1		//extend the period of foot on the ground 			,if ACCW_START=4 Then 1 cycle has 4 more grids on the ground
//CYCLE_START=70(不論奇偶),WAIT_TIME_START=0,ACCW_START=0 means that there are 1 grid on the ground at the beginning and the end


///////////// parameters of nonLIPM an nonHLIPM, for hip_swing() /////////////
/////////////// Parameters in this area are rarely used /////////////////////
#define SHIFT_DISTEN_STABLE 1.5	//hip swing amplitude       
#define HIP_SWING_FIRST_DIR 0	//0 means swing left first,1 means swing right first
#define ACC_TIME_STABLE 0.5 	//0<ACC_TIME_START<=0.5 , control hip_traj, 0.5 means triangle wave
#define HIP_SWING_OFFSET 0.5 	//0<=HIP_SWING_OFFSET<1, this parameter work in function hip_swing()


///////////// parameters of control vector ///////////////////////////////////
//The value of standing still
#define LENGTH_INIT -0.5//0.1//1.5//0.3//0.0	
#define TURN_INIT 1.0//2.5//1.5//-1.0//-2.3//-3.7//-2.9//-2.7//-2.3  //in:-2.4 [:2.0  //counterclockwise - clockwise+
#define YSHIFT_INIT -0.0//-0.9//0.0 right- left+

//units for each change in the vector
#define LENGTH_UNIT 5.0//3.0
#define TURN_UNIT 5.0//4.0
#define YSHIFT_UNIT 3.5//2.5

//maximum travel vector
#define LENGTH_FORWARD_MAX 12.0//14.0//15.0//14.0  //12.0
#define LENGTH_BACK_MAX -12.0 //12.0   
#define TURN_MAX 30.0//17.0//13.0            
#define YSHIFT_MAX 8.0//11.0//12.0 //8.0          
#define KICK_DIR_MAX 30
#define KICK_FORCE_MAX 10

///////////// motor offset (unit:degree) /////////////////////////////////////
//73
///////////// motor offset (unit:degree) /////////////////////////////////////
//73
#define R_HIP_OFFSET 8.0//0.0//-10.0
//53
#define L_HIP_OFFSET -8.0//8.0//5.0//0.0//-4.0
//75
#define R_UP_KNEE_OFFSET 0.0//-68.0//-73.0//-65.0
//55
#define L_UP_KNEE_OFFSET 0.0//+50.0
//77
#define R_DW_KNEE_OFFSET 0.0//75.0//72.0l
//57
#define L_DW_KNEE_OFFSET 0.0//-58.0
//79
#define R_ANKLE_OFFSET 5.0
//59
#define L_ANKLE_OFFSET -23.0//10.0//5.0//6.0//-2.0
//71
#define R_YAW_OFFSET -11.0//-49.0//-50.0//-8.0//+3.0
//51
#define L_YAW_OFFSET 6.0//-9.0//-1.0//1.0

#define L_Shoulder_1 +0.0//11
#define L_Shoulder_2 +0.0//12
#define L_Elbow      +0.0//13

#define R_Shoulder_1 -0.0//21
#define R_Shoulder_2 -0.0//22
#define R_Elbow      +0.0//23

#define Arm_initial_11 205.0
#define Arm_initial_12 100.0
#define Arm_initial_13 262.0
#define Arm_initial_21 155.0
#define Arm_initial_22 260.0
#define Arm_initial_23 98.0


///////////// others /////////////////////////////////////////////////////////
#define GYRO_MASK 7


///////////// define structures //////////////////////////////////////////////
struct comPosParam {
  float startPos[2];
  float supportPos[2];
  float endPos[2];
};
struct limb
{
  float rel[3];
  float gol[3];
  float tmp[3];
};
struct degree
{
  float motor_degree[6];		//one foot has 5 motor
};

///////////// arm //////////////////////////////////////////////
struct arm_degree
{
  float motor_degree[3];		//one foot has 5 motor
};





