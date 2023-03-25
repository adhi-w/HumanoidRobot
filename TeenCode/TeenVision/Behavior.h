#pragma once
#include "Serial.h"
#include "cv.h"
#include "General.h"


class Behavior
{
public:
	Serial m_serial2;

	CString status, decision;

public:
	Behavior(void);
	~Behavior();

	void OpenPort( int nPort = B_comPort, int nBaud = B_Baudrate );
	void ClosePort();
	void Forward();
	void Backward();
	void Shift_Left();
	void Shift_Right();
	void FallDown_Left();
	void FallDown_Right();
	void Fast_Walk();
	void Walk();
	void Set_Robot();
	void Turn_Off();

	void TrackBall(bool findBall, bool moved, CvPoint dist);

	/*int ReadData( void *, int );
	int SendData( const char *, int );
	int ReadDataWaiting( void );

	BOOL IsOpened( void ){ return( m_bOpened ); }
	BOOL WriteCommByte( unsigned char );*/

};

