// KalmanFilter.cpp: implementation of the CKalmanFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HuroEvolutionSystem.h"
#include "KalmanFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKalmanFilter::CKalmanFilter()
{
	SetInitalValue();
}

CKalmanFilter::~CKalmanFilter()
{

}

CvMat* CKalmanFilter::DivMat(CvMat *matrix1, CvMat *matrix2)
{
	CvMat* result_matrix = cvCreateMat(4, 2, CV_32FC1);
	
	float temp1[8];
	int i, j, k = 0;
	for(i = 0; i < matrix2->rows; i++)
		for(j = 0; j < matrix2->cols; j++)
		{
			if(CV_MAT_ELEM(*matrix2, float, i, j) > 0)
				temp1[k] = CV_MAT_ELEM(*matrix1, float, i, j) / CV_MAT_ELEM(*matrix2, float, i, j);
			else
				temp1[k] = 0;
			k++;
		}
		for(i = 2; i < matrix2->rows+2; i++)
			for(j = 0; j < matrix2->cols; j++)
			{
				if(CV_MAT_ELEM(*matrix2, float, i-2, j) > 0)
					temp1[k] = CV_MAT_ELEM(*matrix1, float, i, j) / CV_MAT_ELEM(*matrix2, float, i-2, j);
				else
					temp1[k] = 0;
				k++;
			}
			k = 0;
			for(i = 0; i < 4; i++)
				for(j = 0; j < 2; j++)
				{
					CV_MAT_ELEM(*result_matrix, float, i, j) = temp1[k];
					k++;
				}
	return	result_matrix;
}

void CKalmanFilter::Arr2Mat(CvMat *mat1, float Arr1[], int row, int col)
{
	int i, j, k=0;
	for(i = 0; i < row; i++)
	{
		for(j = 0; j < col; j++)
		{
			CV_MAT_ELEM(*mat1, float, i, j) = Arr1[k];
			k++;
		}
	}
}

void CKalmanFilter::SetInitalValue()
{
	////////////////////////////////////////////////////////////////////////
	float a[] = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1};//state matrix
	float b[] = {0, 0, 1, 1};
	float h[] = {1, 0, 0, 0, 0, 1, 0, 0};//measurement matrix 
	float q[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};//prediction error
	float r[] = {1, 0, 0, 1};//measurement error
	float unitary[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	
	Kalman_para_a = cvCreateMat(4, 4, CV_32FC1);
//	Kalman_para_b = cvCreateMat(1, 4, CV_32FC1);
	Kalman_para_b = cvCreateMat(4, 1, CV_32FC1);
	Kalman_para_h = cvCreateMat(2, 4, CV_32FC1);
	Kalman_para_q = cvCreateMat(4, 4, CV_32FC1);
	Kalman_para_r = cvCreateMat(2, 2, CV_32FC1);
	Kalman_para_Unitary = cvCreateMat(4, 4, CV_32FC1);
	
	Arr2Mat(Kalman_para_a, a, 4, 4);
//	Arr2Mat(Kalman_para_b, b, 1, 4);
	Arr2Mat(Kalman_para_b, b, 4, 1);
	Arr2Mat(Kalman_para_h, h, 2, 4);
	Arr2Mat(Kalman_para_q, q, 4, 4);
	Arr2Mat(Kalman_para_r, r, 2, 2);
	Arr2Mat(Kalman_para_Unitary, unitary, 4, 4);
	
	////////////////////////////////////////////////////////////////////////
	
	Kalman_para_prevx = 0;
	Kalman_para_prevy = 0;
	Kalman_para_prevvx = 0;
	Kalman_para_prevvy = 0;
	Kalman_para_estx = 0;
	Kalman_para_esty = 0;
	Kalman_para_estvx = 40;
	Kalman_para_estvy = 20;
	float xest[] = {Kalman_para_estx, Kalman_para_esty, Kalman_para_estvx, Kalman_para_estvy};
	float p[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	Kalman_para_xest = cvCreateMat(4, 1, CV_32FC1);
	Kalman_para_p = cvCreateMat(4, 4, CV_32FC1);
	Arr2Mat(Kalman_para_xest, xest, 4, 1);
	Arr2Mat(Kalman_para_p, p, 4, 4);
}

void CKalmanFilter::KalmanFilterAlgorithm(float input_x, float input_y, float &output_x, float &output_y)
{
	float vx = input_x - Kalman_para_prevx;
	float vy = input_y - Kalman_para_prevy;
 	float ax = vx - Kalman_para_prevvx;
 	float ay = vy - Kalman_para_prevvy;
	float xactualary[] = {input_x, input_y, vx, vy};
//	float acclerate[] = {vx, vy, ax, ay};
	float acclerate = ax;

	CvMat* xactual = cvCreateMat(4, 1, CV_32FC1);
	Arr2Mat(xactual, xactualary, 4, 1);	
// 	CvMat* xactual_plun = cvCreateMat(4, 1, CV_32FC1);
// 	Arr2Mat(xactual_plun, acclerate, 4, 1);	

	CvMat* yactual = cvCreateMat(2, 1, CV_32FC1);
	cvMatMul(Kalman_para_h, xactual, yactual);
	CvMat* yest = cvCreateMat(2, 1, CV_32FC1);
	cvMatMul(Kalman_para_h, Kalman_para_xest, yest);

	output_x = CV_MAT_ELEM(*yest, float, 0, 0);
	output_y = CV_MAT_ELEM(*yest, float, 1, 0);
////////////////////////////1'st Kalman Equation/////////////////////////////
	/*
		Xk(t+1) = A * Xk(t) + B * Xk'(t)
	*/
	CvMat* xestnext = cvCreateMat(4, 1, CV_32FC1);
	cvMatMul(Kalman_para_a, Kalman_para_xest, xestnext);
	
	CvMat* Bk = cvCreateMat(4, 1, CV_32FC1);
//	cvMatMul(Kalman_para_b, xactual_plun, Bk);
	int i, j;
	for(i = 0; i < Bk->rows; i++)
	{
		for(j = 0; j < Bk->cols; j++)
		{
			CV_MAT_ELEM(*Bk, float, i, j) = CV_MAT_ELEM(*Kalman_para_b, float, i, j) * acclerate;
		}
	}

	CvMat* xactualnext = cvCreateMat(4, 1, CV_32FC1);
	cvMatMul(Kalman_para_a, xactual, xactualnext);
	for(i = 0; i < xactualnext->rows; i++)
	{
		for(j = 0; j < xactualnext->cols; j++)
		{
			CV_MAT_ELEM(*xactualnext, float, i, j) = CV_MAT_ELEM(*xactualnext, float, i, j) + CV_MAT_ELEM(*Bk, float, i, 0);
		}
	}



	CvMat* Zk = cvCreateMat(2, 1, CV_32FC1);
	cvMatMul(Kalman_para_h, xactualnext, Zk);
	CvMat* a_Transpose = cvCreateMat(4, 4, CV_32FC1);
	cvTranspose(Kalman_para_a, a_Transpose);
////////////////////////////////////////////////////////////////////////////

////////////////////////////2'sd Kalman Equation////////////////////////////
	/*
		Pk(t+1) = A(t+1)*Pk(t)*A(t+1)_transpose + Q 
	*/
	CvMat* pnext = cvCreateMat(4, 4, CV_32FC1);
	cvMatMul(Kalman_para_a, Kalman_para_p, pnext);
	cvMatMulAdd(pnext, a_Transpose, Kalman_para_q, pnext);
////////////////////////////////////////////////////////////////////////////

////////////////////////////4'th Kalman Equation////////////////////////////
	/*
					Pk(t+1)*H_transpose
		Kk = -------------------------------------
				H*Pk(t+1)*H_transpose + R
	*/
	CvMat* h_Transpose = cvCreateMat(4, 2, CV_32FC1);
	cvTranspose(Kalman_para_h, h_Transpose);
	CvMat* temp1 = cvCreateMat(4, 2, CV_32FC1);
	cvMatMul(pnext, h_Transpose, temp1);
	CvMat* temp2 = cvCreateMat(2, 4, CV_32FC1);
	CvMat* temp3 = cvCreateMat(2, 2, CV_32FC1);
	cvMatMul(Kalman_para_h, pnext, temp2);
	cvMatMulAdd(temp2, h_Transpose, Kalman_para_r, temp3);
	CvMat* kalman_gain = cvCreateMat(4, 2, CV_32FC1);
	kalman_gain = DivMat(temp1, temp3);
////////////////////////////////////////////////////////////////////////////
	

///////////////////////////3'rd Kalman Equation/////////////////////////////
	/*
		Xk = Xk(t+1)+Kk*(Zk - H*Xk(t+1))
	*/
	CvMat* xupdate = cvCreateMat(4, 1, CV_32FC1);
	CvMat* temp4 = cvCreateMat(2, 1, CV_32FC1);
	cvMatMul(Kalman_para_h, xestnext, temp4);
	cvSub(Zk, temp4, temp4);
	cvMatMul(kalman_gain, temp4, xupdate);
	cvAdd(xestnext, xupdate, xupdate);
////////////////////////////////////////////////////////////////////////////

	
///////////////////////////5'th Kalman Equation/////////////////////////////
	/*
		Pk = (I-Kk*H)*Pk(t+1)
	*/
	CvMat* pupdate = cvCreateMat(4, 4, CV_32FC1);
	cvMatMul(kalman_gain, Kalman_para_h, pupdate);
	cvSub(Kalman_para_Unitary, pupdate, pupdate);
	cvMatMul(pupdate, pnext, pupdate);
////////////////////////////////////////////////////////////////////////////

///////////////////////////Update Kalman Parameters/////////////////////////
	Kalman_para_xest = xupdate;
	Kalman_para_p = pupdate;
	Kalman_para_prevx = input_x;
	Kalman_para_prevy = input_y;
 	Kalman_para_prevvx = vx;
 	Kalman_para_prevvy = vy;
	
////////////////////////////////////////////////////////////////////////////
	cvReleaseMat(&xactual);
	cvReleaseMat(&yactual);
	cvReleaseMat(&yest);
	cvReleaseMat(&xestnext);
	cvReleaseMat(&xactualnext);
	cvReleaseMat(&Zk);
	cvReleaseMat(&a_Transpose);
	cvReleaseMat(&pnext);
	cvReleaseMat(&h_Transpose);
	cvReleaseMat(&temp1);
	cvReleaseMat(&temp2);
	cvReleaseMat(&temp3);
	cvReleaseMat(&kalman_gain);
}

void CKalmanFilter::UpdateState(double t)
{
	float delta_t = t;
	float a[] = {1, 0, delta_t, 0, 0, 1, 0, delta_t, 0, 0, 1, 0, 0, 0, 0, 1};//state matrix
	Arr2Mat(Kalman_para_a, a, 4, 4);
	float b[] = {0, 0, delta_t, delta_t}; 
	Arr2Mat(Kalman_para_b, b, 4, 1);
}
