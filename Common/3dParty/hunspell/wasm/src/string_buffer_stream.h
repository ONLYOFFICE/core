#include <stdlib.h>
#include <string.h>
#include <stdio.h>

class string_buffer_stream
{
private:
	char* m_buffer;
	unsigned int m_pos;
	unsigned int m_size;
	
public:
	string_buffer_stream(char* buf = NULL, unsigned int len = 0)
	{
		m_buffer = buf;
		m_pos = 0;
		m_size = len;
	}
	void from_buffer(char* buf, unsigned int len)
	{
		m_buffer = buf;
		m_pos = 0;
		m_size = len;
	}
	
	// emulate
	// https://en.cppreference.com/w/cpp/string/basic_string/getline
	bool get_line(std::string& data)
	{
		data.erase();
		
		if (m_pos >= m_size)
			return false;
		
		while (m_pos < m_size)
		{
			char c = m_buffer[m_pos++];
			if (c == '\n')
				break;
			if (c == '\r')
			{
				if (m_pos < m_size && m_buffer[m_pos] == '\n')
					++m_pos;				
				break;
			}
			data += c;
		}
		return true;
	}
};
