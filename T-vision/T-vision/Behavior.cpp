#include "StdAfx.h"
#include "Behavior.h"

Behavior::Behavior(void)
{	
}

Behavior::~Behavior()
{	ClosePort();
}

void Behavior::OpenPort(int port, int baudrate)
{	m_serial2.Open(port,baudrate); 
}

void Behavior::ClosePort()
{	m_serial2.Close(); 
}

void Behavior::Forward()
{	m_serial2.WriteCommByte('w');	}

void Behavior::Backward()
{	m_serial2.WriteCommByte('s');	}

void Behavior::Shift_Left()
{	m_serial2.WriteCommByte('d');	}

void Behavior::Shift_Right()
{	m_serial2.WriteCommByte('a');	}

void Behavior::Fast_Walk()
{	m_serial2.WriteCommByte('p');	}

void Behavior::Walk()
{	m_serial2.WriteCommByte('o');	}

void Behavior::Set_Robot()
{	m_serial2.WriteCommByte('0');	}

void Behavior::Turn_Off()
{	m_serial2.WriteCommByte('P');	}

bool Behavior::TrackBall(int x, int y, int ref_x, int ref_y)
{	
	if(x < ref_x-15)	Shift_Right();	//Turn Right
	else if(x > ref_x+15)	Shift_Left(); 	//Turn Left
	else	Forward();

	//if(y > ref_y)	Walk();
	//else Walk();//Set_Robot();

	return true;
}