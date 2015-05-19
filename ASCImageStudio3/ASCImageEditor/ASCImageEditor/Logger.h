#pragma once
//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

void DEV_LOG( const char* pFormatMessage, ... );

//#define LOGGING

//#ifdef    LOGGING
//#include <conio.h>
//#include <stdio.h>
//#include <fcntl.h>
//#include <io.h>
//#include <crtdbg.h>
//#include <string>	
//#include <iostream>	
//#include <fstream>	
//
////-------------------------------------------------------------------------
//// Defines
////-------------------------------------------------------------------------
//
//#define DEV_LOG Logger::GetSingleton()->LogMessage
//
//class FileWrapper
//{
//public:
//	FileWrapper(std::string sMessage)
//	{
//		//Should the file be erased?
//		if(sMessage == "Console initialized.")
//		{
//			m_ofstreamLogFile.open("engine.log");
//
//			if(m_ofstreamLogFile.bad() || !m_ofstreamLogFile.is_open()) 
//				throw(_T("Error while handling the log file"));
//
//			m_ofstreamLogFile << sMessage << "\n";
//		}
//		else
//		{
//			m_ofstreamLogFile.open("engine.log", std::ios::app);
//
//			if(m_ofstreamLogFile.bad() || !m_ofstreamLogFile.is_open())
//				throw(_T("Error while handling the log file"));
//
//			m_ofstreamLogFile << sMessage << "\n";
//		}
//	};
//	~FileWrapper()
//	{
//		m_ofstreamLogFile.close();
//	};
//
//private:
////-------------------------------------------------------------------------
////Datamembers
////-------------------------------------------------------------------------
//
//	std::ofstream m_ofstreamLogFile;
//};
//
//class Logger
//{
//public:
//	// singleton implementation : public destructor + GetSingleton() method
//	~Logger();
//	static Logger*  GetSingleton();
//
//	void LogMessage(std::string sMessage);
//
//private:
//	// singleton implementation : private constructor + static pointer to Logger
//	Logger();
//	static Logger*  m_LoggerPtr;
//
//	// Disable the console "x"
//	void DisableConsoleClose();
//};
//
//#else
//
//#define DEV_LOG CString
//
//#endif