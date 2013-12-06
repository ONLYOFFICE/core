
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "LogDetails.h"
#include "DateTime.h"

namespace Details
{
	const std::string Log::s_dtPattern("%hh:%mm:%ss");


	Log::Log(const std::string& log_file, const std::string& err_file)
#ifdef _DEBUG
		: m_log(log_file.c_str()),
			m_err(err_file.c_str())
#endif
	{
	}


	Log::~Log()
	{
#ifdef _DEBUG
		m_log << std::flush;
		m_err << std::flush;
		m_log.close();
		m_err.close();
#endif
	}


	void Log::event(const std::string& message)
	{
		toLog(" EVENT: " + message);
	}


	void Log::event(const std::wstring& message)
	{
		toLog(L" EVENT: " + message);
	}


	void Log::message(const std::string& message)
	{
		toLog(" MESSAGE: " + message);
	}


	void Log::message(const std::wstring& message)
	{
		toLog(L" MESSAGE: " + message);
	}


	void Log::warning(const std::string& message)
	{
		toLog(" WARNING: " + message);
	}


	void Log::warning(const std::wstring& message)
	{
		toLog(L" WARNING: " + message);
	}


	void Log::error(const std::string& message)
	{
#ifdef _DEBUG
		DateTime dt;
		m_log << std::flush;
		m_err << dt.ToString(s_dtPattern).c_str() << " ERROR: " << message.c_str() << std::endl;
		std::cout << dt.ToString(s_dtPattern) << " ERROR: " << message << std::endl;
#endif
	}


	void Log::error(const std::wstring& message)
	{
#ifdef _DEBUG
		DateTime dt;
		m_log << std::flush;
		m_err << dt.ToString(s_dtPattern).c_str() << L" ERROR: " << message << std::endl;
		std::wcout << dt.ToString(s_dtPattern).c_str() << L" ERROR: " << message << std::endl;
#endif
	}


	void Log::toLog(const std::string& str)
	{
#ifdef _DEBUG
		DateTime dt;
		m_log << dt.ToString(s_dtPattern).c_str() << str.c_str() << "\n";
		std::cout << dt.ToString(s_dtPattern) << str << std::endl;
#endif
	}


	void Log::toLog(const std::wstring& str)
	{
#ifdef _DEBUG
		DateTime dt;
		m_log << dt.ToString(s_dtPattern).c_str() << str << "\n";
		std::wcout << dt.ToString(s_dtPattern).c_str() << str << std::endl;
#endif
	}

} // namespace Details