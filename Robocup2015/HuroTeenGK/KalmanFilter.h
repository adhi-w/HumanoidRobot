// KalmanFilter.h: interface for the CKalmanFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KALMANFILTER_H__9CF8DED2_4CFF_49CC_9F82_41BECB3766DE__INCLUDED_)
#define AFX_KALMANFILTER_H__9CF8DED2_4CFF_49CC_9F82_41BECB3766DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cv.h"
#include "highgui.h"

class CKalmanFilter  
{
public:
	void KalmanFilterAlgorithm(float input_x, float input_y, float &output_x, float &output_y);
	void SetInitalValue();
	void Arr2Mat(CvMat *mat1, float Arr1[], int row, int col);
	CvMat* DivMat(CvMat *matrix1, CvMat *matrix2);
	CKalmanFilter();
	virtual ~CKalmanFilter();

private:
	CvMat* Kalman_para_a;
	CvMat* Kalman_para_h;
	CvMat* Kalman_para_q;
	CvMat* Kalman_para_r;
	CvMat* Kalman_para_Unitary;
	CvMat* Kalman_para_xest;
	CvMat* Kalman_para_p;
	
public:
	float Kalman_para_currx;
	float Kalman_para_curry;
	float Kalman_para_prevx;
	float Kalman_para_prevy;
	float Kalman_para_estx;
	float Kalman_para_esty;
	float Kalman_para_estvx;
	float Kalman_para_estvy;
	float Kalman_para_resultx;
	float Kalman_para_resulty;

};

#endif // !defined(AFX_KALMANFILTER_H__9CF8DED2_4CFF_49CC_9F82_41BECB3766DE__INCLUDED_)
