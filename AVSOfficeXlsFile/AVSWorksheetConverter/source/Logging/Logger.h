#pragma once

class Logger
{
private:
	Logger(const std::string& log_file);
	~Logger();

public:
	static Logger& getLogger();
	void writeLine(const std::string& type, const std::string& str);

private:
	std::ofstream m_log;
};

