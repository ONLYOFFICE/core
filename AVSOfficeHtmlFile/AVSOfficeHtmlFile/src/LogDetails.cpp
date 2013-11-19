#include "stdafx.h"

#ifdef ENABLE_LOG

#include "LogDetails.h"
#include <iostream>
#include "DateTime.h"
#include "assert.h"


namespace Details
{
	const std::string Log::s_dtPattern("%hh:%mm:%ss");


	Log::Log(const std::string& log_file, const std::string& err_file)
		: /*m_log(log_file.c_str()),
			m_err(err_file.c_str()),*/
			m_original_clog_buffer(std::clog.rdbuf(m_log.rdbuf())),
			m_original_cerr_buffer(std::cerr.rdbuf(m_err.rdbuf()))
	{
	}


	Log::~Log()
	{
		std::clog.rdbuf(m_original_clog_buffer);
		std::cerr.rdbuf(m_original_cerr_buffer);

		/*m_log << std::flush;
		m_err << std::flush;
		m_log.close();
		m_err.close();*/
	}


	void Log::event(const std::string& message)
	{
        write( message, " EVENT: " );
	}


	void Log::message(const std::string& message)
	{
        write( message, " MESSAGE: " );
	}


	void Log::warning(const std::string& message)
	{
        write( message, " WARNING: " );
	}


	void Log::error(const std::string& message)
	{
		m_log << std::flush;
        write( message, " ERROR: " );
	}

    void Log::write( const std::string & message, const std::string & header )
    {
		DATA_TIME::DateTime dt;
		//m_log << dt.ToString() << header << message << "\n";
#ifdef _DEBUG
		std::cout << dt.ToString() << header << message << std::endl;
#endif
    }

} // namespace Details

#endif