#pragma once
#ifndef UTILITY_LOG_DETAILS_INCLUDE_H_
#define UTILITY_LOG_DETAILS_INCLUDE_H_

#include <string>
#include <fstream>


namespace Details
{
	class Log
	{
	public:
		Log(const std::string& log_file, const std::string& err_file);
		~Log();

	public:
		void event(const std::string& message);
		void event(const std::wstring& message);
		void message(const std::string& message);
		void message(const std::wstring& message);
		void warning(const std::string& message);
		void warning(const std::wstring& message);
		void error(const std::string& message);
		void error(const std::wstring& message);

	private:
		std::wofstream m_log;
		std::wofstream m_err;

	private:
		void toLog(const std::string& str);
		void toLog(const std::wstring& str);

	private:
		static const std::string s_dtPattern;
	};
} // namespace Details

#endif // UTILITY_LOG_DETAILS_INCLUDE_H_