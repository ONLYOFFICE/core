#include <windows.h>
#include "utils.h"
#include <string>

namespace AVS
{
	namespace Utils
	{
		std::string __WtoS(const wchar_t* wstr, int codepage)
		{
			std::string _outstring;
			if(!wstr)
				return "";
			char* str;
			int strlrn = 0;
			strlrn = WideCharToMultiByte(codepage, 0, wstr, -1, NULL, 0, 0, 0); 
			if(!strlrn)
				return "";

			str = (char*)malloc(sizeof(char)*strlrn);//new char[strlrn]; 
			if (!str)
				return "";
			WideCharToMultiByte(codepage, 0, wstr, -1, str, (int)strlrn, 0, 0);
			_outstring.assign(str);
			free(str);
			return _outstring;
		}

		std::wstring __StoW(const char * str, int codepage)
		{
			std::wstring _outstring;
			if (!str)
				return L"";
			int ccWideChar = MultiByteToWideChar(codepage, 0, str, -1, NULL, 0);
			if (!ccWideChar)
				return L"";
			wchar_t * wcstr = (wchar_t*)malloc(sizeof(wchar_t)*ccWideChar);
			if (!wcstr)
				return L"";
#ifdef _DEBUG			
			memset(wcstr, 0, sizeof(wchar_t)*(ccWideChar));
#endif
			MultiByteToWideChar(codepage, 0, str, -1, wcstr, ccWideChar);
			_outstring.assign(wcstr);
			free(wcstr);
			return _outstring;
		}

	}
}