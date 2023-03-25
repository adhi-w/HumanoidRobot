#pragma once
#include "Serial.h"

class Behavior
{
public:
	Serial m_serial2;
public:
	Behavior(void);
	~Behavior();

	void OpenPort( int nPort = 2, int nBaud = 115200 );
	void ClosePort();
	void Forward();
	void Backward();
	void Shift_Left();
	void Shift_Right();
	void Fast_Walk();
	void Walk();
	void Set_Robot();
	void Turn_Off();

	bool TrackBall(int x, int y, int ref_x, int ref_y);

	/*int ReadData( void *, int );
	int SendData( const char *, int );
	int ReadDataWaiting( void );

	BOOL IsOpened( void ){ return( m_bOpened ); }
	BOOL WriteCommByte( unsigned char );*/

};

