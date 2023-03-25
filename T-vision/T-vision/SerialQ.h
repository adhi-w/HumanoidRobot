#include "stdafx.h"
#include <stdlib.h>
#using <System.dll>

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

BOOL     PortReady;
HANDLE   Comm;
CString  ComPort;
DCB      dcb;
COMMTIMEOUTS CommTimeout;
BOOL     dWrite;
BOOL     dRead;
DWORD iBytesWritten;
DWORD iBytesRead;

OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
BOOL m_bOpened;

char       sBuffer[128];

char buff[128];
char ad[10];


BOOL serial_open(int port,int baud)
{	
	if(port==1)ComPort="Com1";
	else if(port==2)ComPort="Com2";
	else if(port==3)ComPort="Com3";
	else if(port==4)ComPort="Com4";
	else if(port==5)ComPort="Com5";
	else if(port==6)ComPort="Com6";
	else if(port==7)ComPort="Com7";

	if( m_bOpened ) return( TRUE );

	Comm = CreateFile(ComPort, 
		GENERIC_READ | GENERIC_WRITE,
		0, // exclusive access
		NULL, // no security
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // no overlapped I/O
		NULL); // null template 

	if( Comm == NULL ) return( FALSE );
	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );

	//PortReady = SetupComm(Comm, 128, 128); // set buffer sizes
	CommTimeout.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeout.ReadTotalTimeoutConstant = 0;
	CommTimeout.ReadTotalTimeoutMultiplier = 0;
	CommTimeout.WriteTotalTimeoutConstant = 5000;
	CommTimeout.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts (Comm, &CommTimeout);

	m_OverlappedRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_OverlappedWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	dcb.DCBlength = sizeof( DCB );
	GetCommState(Comm, &dcb);
	dcb.BaudRate = baud;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
//	dcb.fAbortOnError = TRUE;

	unsigned char ucSet;
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_DTRDSR ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_RTSCTS ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_XONXOFF ) != 0 );

	if( !SetCommState(Comm, &dcb) ||
		!SetupComm( Comm, 10000, 10000 ) ||
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL ){
		DWORD dwError = GetLastError();
		if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
		if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
		CloseHandle( Comm );
		return( FALSE );
		}

	m_bOpened = TRUE;

	return( m_bOpened );	
}

BOOL serial_close( void )
{
	if( !m_bOpened || Comm == NULL ) return( TRUE );

	if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
	if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
	CloseHandle( Comm );
	m_bOpened = FALSE;
	Comm = NULL;

	return( TRUE );
}

BOOL WriteCommByte( unsigned char ucByte )
{
	BOOL bWriteStat;
	DWORD dwBytesWritten;

	bWriteStat = WriteFile( Comm, (LPSTR) &ucByte, 1, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) ){
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) dwBytesWritten = 0;
		else{
			GetOverlappedResult(Comm, &m_OverlappedWrite, &dwBytesWritten, FALSE );
			m_OverlappedWrite.Offset += dwBytesWritten;
			}
		}
	return( TRUE );
}

int SendData( const char *buffer, int size )
{
	if( !m_bOpened || Comm== NULL ) return( 0 );

	DWORD dwBytesWritten = 0;
	int i;
	for( i=0; i<size; i++ ){
		WriteCommByte( buffer[i] );
		dwBytesWritten++;
		}
	return( (int) dwBytesWritten );
}

int ReadDataWaiting( void )
{
	if( !m_bOpened || Comm == NULL ) return( 0 );

	DWORD dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( Comm, &dwErrorFlags, &ComStat );

	return( (int) ComStat.cbInQue );
}

int ReadData( void *buffer, int limit )
{
	if( !m_bOpened || Comm == NULL ) return( 0 );

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( Comm, &dwErrorFlags, &ComStat );
	if( !ComStat.cbInQue ) return( 0 );

	dwBytesRead = (DWORD) ComStat.cbInQue;
	if( limit < (int) dwBytesRead ) dwBytesRead = (DWORD) limit;

	bReadStatus = ReadFile( Comm, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead );
	if( !bReadStatus ){
		if( GetLastError() == ERROR_IO_PENDING ){
			WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
			return( (int) dwBytesRead );
			}
		return( 0 );
		}
	return( (int) dwBytesRead );
}

//----------------------------------------------
void add(char z)
{	
	sprintf(ad,"%c",z);
	dWrite = WriteFile(Comm,ad,1,&iBytesWritten,NULL);
}

void data(int x)
{	
	if(x>=100)
	{	sprintf(buff,"%d",x);
		dWrite = WriteFile(Comm,buff,3,&iBytesWritten,NULL);
	}	
	else if(x>=10)
	{	sprintf(buff,"0%d",x);
		dWrite = WriteFile(Comm,buff,3,&iBytesWritten,NULL);
	}	
	else
	{	sprintf(buff,"00%d",x);
		dWrite = WriteFile(Comm,buff,3,&iBytesWritten,NULL);
	}	
}

void kirim(char q,int x,int y)
{	
	add(q);	data(x); add(',');data(y);
}
