#pragma once

#include <string>

class RtfToken
{
public:
	typedef enum {None ,Keyword,Control ,Text ,Eof,GroupStart ,GroupEnd} RtfTokenType;
	
	RtfTokenType Type;
    std::string Key;
	bool HasParameter;
	int Parameter;

	RtfToken()
    {
		Type = None;
        Key = "";
		HasParameter = false;
		Parameter = 0;
    }
};
