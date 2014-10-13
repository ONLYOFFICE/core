#pragma once
#include <fstream>

class Logger
{
private:
	Logger(const std::string& log_file);
	~Logger();

public:
	static Logger& getLogger();
	void writeLine(const std::string& type, const std::string& str);
	void writeLine(const std::wstring& type, const std::wstring& str);

private:
	std::wofstream m_log;
};

