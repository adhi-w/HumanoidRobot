#include "SSVR.h"
#include "math.h"

int Input_Feature[MODEL_NUM_FEA]={3, 4, 5, 6, 9, 10, 11, 12, 13, 15, 16, 17};

float normal_feature[TOTAL_FEATURE_NUM][2] = {
	{    1,    0},		//# 1, start time
	{    1,    0},		//# 2, end time
	{  4.5,  0.5},		//# 3, step_cycle_time,				unit:s
	{   25,    1},		//# 4, gait_height,				unit:cm
	{ 2.45,    0},		//# 5, double_support phase in one step_cycle	unit:s
	{   90,   60},		//# 6, hip_moving_avr_height			unit:cm
	{  5.0,  0.0},		//# 7, hip_moving_change_height(2 times amp)	unit:cm
	{   42,    0},		//# 8, hip_moving_change_theta(2 times amp)	unit:degree
	{   17,    0},		//# 9, yshift amp(2 times amp)			unit:cm
	{  0.9, 0.05},		//#10, percent_hight				unit:%
	{   60,  -60},		//#11, x length of one step		        unit:cm
	{   35,   15},		//#12, hip wide(in y axis)             		unit:cm
	{   90,   60},		//#13, hip height(in z axis)           		unit:cm
	{   10,   -6},		//#14, tilt forward angle           		unit:degree
	{  190,  130},		//#15, height		           		unit:cm
	{   90,   15},		//#16, weight		           		unit:kg
	{  105,   55},		//#17, COM height	           		unit:cm	
};
	//# 7,hip_moving_change_height(2 times amp)
	float height_model_w[MODEL_NUM_FEA] ={
	0.1096, 0.6974, -0.1582, 1.0766, -0.0246, -0.1661, -0.0457, -0.0515,
	-1.4167, -0.1922, 0.1478, 0.4542, 
	};
	float height_model_b =0.2429;

	//# 8, hip_moving_change_theta(2 times amp)
	float theta_model_w[MODEL_NUM_FEA] ={
	0.2855, 0.7920, -0.1336, 1.1588, -0.0546, -0.1465, -0.0017, -0.0803,
	-1.3914, -0.4766, 0.2067, 0.3703, 
	};
	float theta_model_b =0.3274;

	//#14, tilt forward angle 

	float tilt_model_w[MODEL_NUM_FEA] ={
	0.0792, 0.0706, -0.1290, -2.8316, -0.2852, -0.1020, 0.1171, 0.1274,
	3.3006, -0.6686, -0.1343, 0.0988, 
	};
	float tilt_model_b =0.6742;
/*
	float tilt_model_w[MODEL_NUM_FEA] ={
	0.0600, 0.0753, -0.1454, -3.0444, -0.2697, -0.1152, -0.1044, 0.1333,
	3.5181, -0.6552, -0.1468, 0.1016, 
	};
	float tilt_model_b =0.8163;
*/

SSVR::SSVR()
{
	int i;
	for(i=0;i<TOTAL_FEATURE_NUM;i++)
		row_data[i] = 0;
	for(i=0;i<MODEL_NUM_FEA;i++)
		normal_row_data[i] = 0;
	
	predict = 0;
}

float SSVR::predictSSVR(int label_feature, float inst[MODEL_NUM_FEA])
{
        //Use exist model to predict label
        //inst[MODEL_NUM_FEA] :input data you want to predict label
        
	int i,j;
	float PredictedLabel = 0;

	//only solve in primal form
	if(label_feature == 7)
	{
       		for(i=0 ;i<MODEL_NUM_FEA ;i++)
        	{	
        		PredictedLabel = PredictedLabel + (inst[i]*height_model_w[i]);
       		}
       		PredictedLabel = PredictedLabel  +  height_model_b;
	}
	else if(label_feature == 8)
	{
       		for(i=0 ;i<MODEL_NUM_FEA ;i++)
        	{	
        		PredictedLabel = PredictedLabel + (inst[i]*theta_model_w[i]);
       		}
       		PredictedLabel = PredictedLabel  +  theta_model_b;
	}
	else if(label_feature == 14)
	{
       		for(i=0 ;i<MODEL_NUM_FEA ;i++)
        	{	
        		PredictedLabel = PredictedLabel + (inst[i]*tilt_model_w[i]);
       		}
       		PredictedLabel = PredictedLabel  +  tilt_model_b;
	}	
	else
	{
		PredictedLabel = 0.5;
	}

        PredictedLabel = PredictedLabel * (normal_feature[label_feature-1][0] - normal_feature[label_feature-1][1]) + normal_feature[label_feature-1][1];
	return PredictedLabel;
}


void SSVR::normalization(float in_row_data[TOTAL_FEATURE_NUM] ,float out_row_data[MODEL_NUM_FEA])
{
        //normalization data
	//in_row_data[TOTAL_FEATURE_NUM] : all fearture of data
	//label_feature : position of label in matlab model
	//out_row_data[MODEL_NUM_FEA]

	int i;
	int temp=0;
	int temp2=0;
	for(i=0; i<TOTAL_FEATURE_NUM; i++)
	{
		if(i == (Input_Feature[temp2]-1))
		{
			out_row_data[temp] = ( in_row_data[i] - normal_feature[i][1] ) / (normal_feature[i][0] - normal_feature[i][1]);
			temp ++;
			temp2++;
		}
	}
}

