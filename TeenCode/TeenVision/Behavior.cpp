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

void Behavior::FallDown_Left()
{	m_serial2.WriteCommByte('Q');	}

void Behavior::FallDown_Right()
{	m_serial2.WriteCommByte('W');	}

void Behavior::Fast_Walk()
{	m_serial2.WriteCommByte('p');	}

void Behavior::Walk()
{	m_serial2.WriteCommByte('o');	}

void Behavior::Set_Robot()
{	m_serial2.WriteCommByte('0');	}

void Behavior::Turn_Off()
{	m_serial2.WriteCommByte('P');	}

void Behavior::TrackBall(bool findBall, bool moved, CvPoint dist)
{	
	if(findBall)
	{
		//________Ball is far away
		if(dist.y >= 165) 
		{ 
			if(dist.x > 10)
			{	Walk(); status = "Ball is far Right";	decision= "Walk on the ground";	}
		
			else if( dist.x < -10) 
			{ 	Walk(); status = "Ball is far Left"; decision= "Walk on the ground";	}

			else { 	Walk(); status = "Ball is far"; decision= "Walk on the ground";	}
		}

		//________Ball is in the middle
		else if(dist.y >80 && dist.y <165) 
		{ 
			if(dist.x > 10)
			{	Shift_Right();	status = "Ball is in the middle Right";	decision= "Shift Right"; }
		
			else if(dist.x <-10)
			{	Shift_Left();	status = "Ball is in the middle Left";	decision= "Shift Left";	}

			else {	Walk();	status = "Ball is in the middle";	decision= "Walk on the ground";	}
		}

		//--------Ball is near
		else if(dist.y <=80)
		{
			if(dist.x > 10)
			{	FallDown_Right(); status = "Ball is near Right";	decision= "Fall down to the Right"; }

			else if(dist.x < -10)
			{	FallDown_Left(); status = "Ball is near Left";	decision= "Fall down to the Left"; }

			else
			{	Set_Robot(); status = "Ball is near";	decision = "Stop"; }
				
		}
		else
		{
			Set_Robot(); status = "Set Robot"; decision = "Stop";
		}
	}
	
	else
	{
			Walk(); status = "Ball is Gone"; decision = "Walk & Scanning";
	}
	//if(moved)
	//{
		
	//}
	
}

