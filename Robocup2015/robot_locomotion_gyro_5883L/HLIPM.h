class HLIPM
{
	public:
		HLIPM();
		void OnInitialUpdate();
		void foot_trajectory(float t,float Ts,float GaitLength,float *ankle, float GaitShift, float GaitH);
		float hip_swing(int swing_foot,float t,float Ts);
		void comParamUpdate(int currentSwingFoot, float swingMovement_x, float swingMovement_y, struct comPosParam *comUpdate, float *right_footPostion, float *left_footPostion);
		void generateCoMTrajectory(float t,float Ts, struct comPosParam comPos,float supTime, float *movement, float Tcons);
		float LIPMcalculateInitVelocity(float initPos, float finalPos, float supPos, float supTime, float Tcons);
		float LIPMcalculateCoMPos(float initPos, float initVel, float supPos, float timeStep, float Tcons);
		float CMChange(float t,float Ts);
		float CMChangeHuman(float t,float Ts,int swing_foot);
		float HipPlaneTheta(float t,float Ts,int swing_foot);

		///////////////////  hip_swing   /////////////////////
		float shift_disten;
		float acc_time;  
		bool swing_shift_dir;		
		///////////////////  Change Hip Height   /////////////////////
		float CM_c;
		float CM_H_theta;
};
