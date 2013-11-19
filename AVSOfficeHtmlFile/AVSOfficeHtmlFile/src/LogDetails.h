#pragma once

#ifdef ENABLE_LOG

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
		void message(const std::string& message);
		void warning(const std::string& message);
		void error(const std::string& message);

	private:

        void write( const std::string & message, const std::string & header );

		std::ofstream m_log;
		std::ofstream m_err;

		std::streambuf* m_original_clog_buffer;
		std::streambuf* m_original_cerr_buffer;

	private:
		static const std::string s_dtPattern;
	};
} // namespace Details

#endif