//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

#include "stdafx.h"
#include <stdio.h>
#include "Logger.h"

#ifdef LOGGING

class MyLogFile
{
private:
	FILE* m_pFile;

public:
	MyLogFile( const char* szFilePath = NULL ) : m_pFile( NULL )
	{
		if( !szFilePath )
			szFilePath = "c:\\log_avs_ie.txt";

		m_pFile = ::fopen( szFilePath, "a" );

		SendMessage( "\r\n-------- Open log file --------\r\n\r\n" );
	}

	~MyLogFile()
	{
		if( m_pFile )
		{
			SendMessage( "\r\n-------- Close log file --------\r\n" );
			fclose( m_pFile );
			m_pFile = NULL;
		}
	}

	void SendMessage( const char* szFormatMessage, ... )
	{
		va_list argList;
		va_start( argList, szFormatMessage );

		::vfprintf( m_pFile, szFormatMessage, argList );
	}

	void SendMessageV( const char* szFormatMessage, va_list vl )
	{
		::vfprintf( m_pFile, szFormatMessage, vl );
	}
};

void DEV_LOG( const char* szFormatMessage, ... )
{
	static MyLogFile mylog;

	if( szFormatMessage )
	{
		mylog.SendMessage( "(%08x) ", GetCurrentThreadId() );

		va_list argList;
		va_start( argList, szFormatMessage );

		mylog.SendMessageV( szFormatMessage, argList );

		mylog.SendMessage( "\r\n" );
	}
}

#else

void DEV_LOG( const char* szFormatMessage, ... )
{
}

#endif

//#ifdef LOGGING
////-----------------------------------------------------------------
//// Static Variable Initialization
////-----------------------------------------------------------------
//Logger* Logger::m_LoggerPtr = 0;
//
////-------------------------------------------------------------------------
//// Logger methods
////-------------------------------------------------------------------------
//
//Logger::Logger()
//{
//	//Locals needed to create the console
//	int iConsoleHandle;
//	long lStdHandle;
//	FILE* pFile;
//	CONSOLE_SCREEN_BUFFER_INFO CSBIConsoleInfo;
//
//	//Allocate the console to this application and set the size
//	AllocConsole();
//	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CSBIConsoleInfo);
//
//	CSBIConsoleInfo.dwSize.Y = 1024;
//
//	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), CSBIConsoleInfo.dwSize);
//
//	//Redirect to the console
//	lStdHandle = reinterpret_cast<long>(GetStdHandle(STD_OUTPUT_HANDLE));
//	iConsoleHandle = _open_osfhandle(lStdHandle, _O_TEXT);
//
//	pFile = _fdopen( iConsoleHandle, "w" );
//	*stdout = *pFile;
//	setvbuf( stdout, NULL, _IONBF, 0 );
//
//	lStdHandle = reinterpret_cast<long>(GetStdHandle(STD_ERROR_HANDLE));
//	iConsoleHandle = _open_osfhandle(lStdHandle, _O_TEXT);
//
//	pFile = _fdopen( iConsoleHandle, "w" );
//	*stderr = *pFile;
//	setvbuf( stderr, NULL, _IONBF, 0 );
//
//	//Make cout point to console
//	std::ios::sync_with_stdio();
//
//	// Disable the console "x"
//	DisableConsoleClose();
//
//	LogMessage("Start logging");
//	LogMessage("-------------------------------------------");
//}
//Logger::~Logger()
//{
//
//}
//Logger* Logger::GetSingleton()
//{
//	if ( m_LoggerPtr == 0) m_LoggerPtr = new Logger();
//	return m_LoggerPtr;
//}
//
//void Logger::DisableConsoleClose() 
//{
//    HWND hConsole;
//    wchar_t sConsoleWindowName[1024]; 
//
//	//Get the title of the console and get the HWND by name
//    GetConsoleTitleW(static_cast<LPWSTR>(sConsoleWindowName), 1024);
//    hConsole = FindWindowW(NULL, sConsoleWindowName);
//
//    RemoveMenu(GetSystemMenu(hConsole, FALSE), SC_CLOSE, MF_BYCOMMAND);
//
//	RECT rect;
//
//	// get the current window size and position
//	::GetWindowRect( hConsole, &rect );
//
//	long scrW = ::GetSystemMetrics(SM_CXSCREEN);
//	long scrH = ::GetSystemMetrics(SM_CYSCREEN);
//
//	long cwW = (rect.right-rect.left)/2;
//	long cwH = (rect.bottom-rect.top)/2;
//
//	// now change the size, position, and Z order
//	// of the window.
//	::SetWindowPos(hConsole ,  HWND_TOPMOST,  scrW - cwW - 20, scrH - cwH - 50, cwW, cwH,SWP_SHOWWINDOW);
//}
//
//void Logger::LogMessage(std::string sMessage)
//{
//	std::cout << sMessage << "\n";
//
//	/*try{ FileWrapper tempWrapper(sMessage); }
//	catch(char sMessage[1024])
//	{
//		MessageBoxA(NULL, sMessage, "ERROR!", MB_ICONERROR | MB_OK);
//	}*/
//}
//
//
//#endif