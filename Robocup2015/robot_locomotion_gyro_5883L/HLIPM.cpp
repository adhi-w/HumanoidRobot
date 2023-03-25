#include "HLIPM.h"
#include "math.h"
#include "Parameter.h"


HLIPM::HLIPM()
{
	shift_disten=SHIFT_DISTEN_STABLE; 
	acc_time=ACC_TIME_STABLE;  
	swing_shift_dir=HIP_SWING_FIRST_DIR;  //for hip_swing() to match foot

///////////////////  Change Hip Height   /////////////////////
	CM_c=sqrt( ((GAITWIDTH)/2.0)*((GAITWIDTH)/2.0) + (HIP_HEIGHT)*(HIP_HEIGHT) );	//CM_c will be the max mass height of cycle,Warring! CM_c < 79.0 max(78.9)
	CM_H_theta=atan((GAITWIDTH)*0.5/HIP_HEIGHT);
}

void HLIPM::OnInitialUpdate()
{
	shift_disten=SHIFT_DISTEN_STABLE; 
	acc_time=ACC_TIME_STABLE;  
	swing_shift_dir=HIP_SWING_FIRST_DIR;  //for hip_swing() to match foot
}

void HLIPM::foot_trajectory(float t,float Ts,float GaitLength,float *ankle, float GaitShift, float GaitH)   //ankle are return value
{
	float c1 = 2.0 * M_PI * (t/Ts);
	float c2 = 2.0 * M_PI *((t - FOOT_TRAJ_PERCENTHIGH * Ts)/((1.0 - FOOT_TRAJ_PERCENTHIGH)*Ts));
	float c3 = c1 / FOOT_TRAJ_PERCENTHIGH;
	ankle[0] = (GaitLength/(2.0*M_PI)) * (c1 - sin(c1));
	ankle[1] = (GaitShift/(2.0*M_PI)) * (c1 - sin(c1));

	if (t >= 0 && t <= FOOT_TRAJ_PERCENTHIGH*Ts)
		ankle[2] = (GaitH/(2.0*M_PI)) * (c3 - sin(c3)) + SHOE_HEIGHT;
	else if (t >= FOOT_TRAJ_PERCENTHIGH*Ts && t <= Ts)
		ankle[2] = GaitH - (GaitH/(2.0*M_PI)) * (c2 - sin(c2)) + SHOE_HEIGHT;
}

float HLIPM::hip_swing(int swing_foot,float t,float Ts)
{
	float Position=0;
        int t_offset;

        if(HIP_SWING_OFFSET==1 || HIP_SWING_OFFSET==0)
          t_offset=t;
        else if(HIP_SWING_OFFSET<1 && HIP_SWING_OFFSET>0)
          t_offset = ( int(t + (HIP_SWING_OFFSET * Ts) + 0.5) ) % int(Ts);
        else
          t_offset = ( int(t + (0.5 * Ts) +0.5) ) % int(Ts);
          
	float v=0;
	v= shift_disten / (acc_time * (Ts)) ;

	if ( (t_offset/Ts) < acc_time) 
		Position=v * t_offset;
	else if( (t_offset/Ts) > (1-acc_time) )
		Position = shift_disten - v * (t_offset-(1-acc_time)*Ts);     
	else
		Position=shift_disten;

	if(Position>shift_disten)
		Position=shift_disten;
	else if(Position<-shift_disten)
		Position=-shift_disten;


        if(t_offset==0)
        {
		if(swing_shift_dir==0)
          		swing_shift_dir=1;
         	else
          		swing_shift_dir=0;
        }       
	if (swing_shift_dir == 0)
		Position=-(Position*1.0);
	else
		Position=Position*1.0;

	return Position;
}


void HLIPM::comParamUpdate(int currentSwingFoot, float swingMovement_x, float swingMovement_y, struct comPosParam *comUpdate, float *right_footPostion, float *left_footPostion)
{
	if(currentSwingFoot == 1){	//left foot is swing foot, so right foot is support foot
		comUpdate->supportPos[0] = right_footPostion[0];
		comUpdate->supportPos[1] = right_footPostion[1];
	}else{				//right foot is swing foot, so left foot is support foot
		comUpdate->supportPos[0] = left_footPostion[0];
		comUpdate->supportPos[1] = left_footPostion[1];
	}
	//new position of LIPM support point 
	//footPostion.globalPos original point is COM 
	
	comUpdate->startPos[0] = (left_footPostion[0] + right_footPostion[0])/2.0;
	comUpdate->startPos[1] = (left_footPostion[1] + right_footPostion[1])/2.0;
	comUpdate->endPos[0] = comUpdate->startPos[0] + swingMovement_x/2.0;
	comUpdate->endPos[1] = comUpdate->startPos[1] + swingMovement_y/2.0;

}

void HLIPM::generateCoMTrajectory(float t,float Ts, struct comPosParam comPos,float supTime, float *movement, float Tcons)
{
	float initVelX, initVelY;

	initVelX = LIPMcalculateInitVelocity(comPos.startPos[0], comPos.endPos[0], comPos.supportPos[0], supTime, Tcons);
	initVelY = LIPMcalculateInitVelocity(comPos.startPos[1], comPos.endPos[1], comPos.supportPos[1], supTime, Tcons);
	//Get COM Velocity in X&Y dir 

	movement[0] = LIPMcalculateCoMPos(comPos.startPos[0], initVelX, comPos.supportPos[0], supTime*(t/Ts), Tcons);
	movement[1] = LIPMcalculateCoMPos(comPos.startPos[1], initVelY, comPos.supportPos[1], supTime*(t/Ts), Tcons);
	movement[2] = HIP_HEIGHT;
	//Get COM position in this moment
}

float HLIPM::LIPMcalculateInitVelocity(float initPos, float finalPos, float supPos, float supTime, float Tcons)
{
	float initVel;
	initVel = ((finalPos - supPos) - (initPos - supPos) * cosh(supTime/Tcons)) / (Tcons * sinh(supTime/Tcons));
	return initVel;
}

float HLIPM::LIPMcalculateCoMPos(float initPos, float initVel, float supPos, float timeStep, float Tcons)//, float Tcons)
{
	float currentPos;
	currentPos = (initPos - supPos) * cosh(timeStep/Tcons) + Tcons * initVel * sinh(timeStep/Tcons) + supPos;

	return currentPos;
}

float HLIPM::CMChange(float t,float Ts)
{
	float change_height;
	float reg_theta= CM_H_theta/(Ts*0.5);

	if(t<(Ts)*0.5)
		change_height = CM_c * cos(CM_H_theta - t*reg_theta );
	else
		change_height = CM_c * cos( (t-(Ts)*0.5)*reg_theta );
      
	return change_height;	
}

float HLIPM::CMChangeHuman(float t,float Ts,int swing_foot)
{
	float t_temp = 0;
	float change_height;
	t_temp = (!swing_foot)*Ts + t;
	//2*M_PI*(t_temp/(2*Ts))
	//change_height =  HIP_HEIGHT + HIP_HEIGHT_AMP * (0.5 - 0.5*cos( 4*M_PI*(t_temp/(2*Ts)-0.5) ) + 0.02*sin( 4*M_PI*(t_temp/(2*Ts)-0.5) ) );
	change_height =  HIP_HEIGHT + HIP_HEIGHT_AMP * (0.5 - 0.5*cos( 4*M_PI*(t_temp/(2*Ts)-0.5) ) );
	return change_height;		
}

float HLIPM::HipPlaneTheta(float t,float Ts,int swing_foot)
{
	//foot_select == 1	//means left foot is swing foot
	//foot_select == 0	//means right foot is swing foot
	float t_temp = 0;
	float hip_plane_theta = 0;
	t_temp = (!swing_foot)*Ts + t;
	hip_plane_theta =  Plane_Theta_AMP * sin( 2*M_PI*(t_temp/(2*Ts)-0.5) );
	return hip_plane_theta;
}

