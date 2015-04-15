#pragma once

#ifdef _WIN32
	#include <atlbase.h>
	#include <atlstr.h>
#else
	#include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

class RtfToken
{
public:
	typedef enum {None ,Keyword,Control ,Text ,Eof,GroupStart ,GroupEnd} RtfTokenType;
	
	RtfTokenType Type;
	CStringA Key;
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