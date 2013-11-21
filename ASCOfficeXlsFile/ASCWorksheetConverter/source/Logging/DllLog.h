
#pragma once

#define BUFFER_SIZE 32768

class CDllLog  
{
public:
	CDllLog::CDllLog(wchar_t* logFileName, int mode)
	{
		m_hLog = NULL;
		
		if(logFileName == NULL)
			return;

		if (!mode)		// always new
		{
			m_hLog = CreateFileW(logFileName,
								GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
			if(m_hLog  == INVALID_HANDLE_VALUE)
				m_hLog = NULL;
			else
				DefineEOL();			
		}
		else
		{
			m_hLog = CreateFileW(logFileName,
								GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								OPEN_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

			if(m_hLog  == INVALID_HANDLE_VALUE)
				m_hLog = NULL;		
			else
			{
				DefineEOL();

				LARGE_INTEGER la;
				::GetFileSizeEx(m_hLog, &la);

				SetFilePointer(m_hLog, 0, NULL, FILE_END);
				
				if (la.QuadPart > 0)
				{
					write(L"\n");
				}
			}	
		}
	}

	CDllLog::~CDllLog(void)
	{
		if(m_hLog != NULL)
		{
			FlushFileBuffers(m_hLog);
			CloseHandle(m_hLog);
			m_hLog = NULL;
		}
	}

	void CDllLog::DefineEOL()
	{
		m_eol[0] = '\r';//0x0D;
		m_eol[1] = '\n';//0x0A;
		m_eol[2] = '\0';//0x00;
	}
	//
	void CDllLog::write(char* string)
	{
		if((m_hLog == NULL) || (string == NULL)){	return;}
		
		WriteFile(m_hLog, string, (DWORD)strlen(string), &len, NULL);
		WriteFile(m_hLog, m_eol, (DWORD)strlen(m_eol), &len, NULL);		
	}

	void CDllLog::write(char* string1, char* string2)
	{
		if((m_hLog == NULL) || (string1 == NULL) || (string2 == NULL))
			return;

		WriteFile(m_hLog, string1, (DWORD)strlen(string1), &len, NULL);	
		WriteFile(m_hLog, "\t\t", (DWORD)strlen("\t\t"),	&len, NULL);

		WriteFile(m_hLog, string2, (DWORD)strlen(string2),	&len, NULL);				
		WriteFile(m_hLog, m_eol, (DWORD)strlen(m_eol),	&len,	NULL);	
	}

	void CDllLog::write(wchar_t* string)
	{
		if ((m_hLog == NULL) || (string == NULL))
			return;		

		memset(buf, 0, BUFFER_SIZE);

		if (ConvertString(string, buf))
		{
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);
			WriteFile(m_hLog, m_eol, (DWORD)strlen(m_eol), &len, NULL);
		}
	}

	void CDllLog::write(wchar_t* string1, wchar_t* string2)
	{
		if ((m_hLog == NULL) || (string1 == NULL) || (string2 == NULL))
			return;
		
		memset(buf, 0, BUFFER_SIZE);	
		
		if (ConvertString(string1, buf))
		{
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);	
			WriteFile(m_hLog, "\t\t", (DWORD)strlen("\t\t"),	&len, NULL);

			memset(buf, 0, MAX_PATH);	 
			
			if (ConvertString(string2, buf))
			{
				WriteFile(m_hLog, buf, (DWORD)strlen(buf),	&len, NULL);				
				WriteFile(m_hLog, m_eol, (DWORD)strlen(m_eol),	&len,	NULL);
			}
		}
	}
	
	//
	void CDllLog::write(wchar_t* string, int value)
	{
		write(string, (LONGLONG)value);
	}
	void CDllLog::write(wchar_t* string, long value)
	{
		write(string, (LONGLONG)value);
	}
	void CDllLog::write(wchar_t* string, LONGLONG value)
	{
		if((m_hLog == NULL) || (string == NULL))
			return;

		memset(buf, 0, BUFFER_SIZE);

		if (ConvertString(string, buf))
		{
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);

			memset(buf, 0, BUFFER_SIZE);
			_i64toa_s(value, buf, BUFFER_SIZE, 10);
			WriteFile(m_hLog, "\t\t", (DWORD)strlen("\t\t"), &len, NULL);
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);
			WriteFile(m_hLog, m_eol, (DWORD)strlen(m_eol),	&len, NULL);
		}
	}

	//
	void CDllLog::write(wchar_t* string, unsigned char value)
	{
		write(string, (ULONGLONG)value);
	}
	void CDllLog::write(wchar_t* string, size_t value)
	{
		write(string, (ULONGLONG)value);
	}
	void CDllLog::write(wchar_t* string, DWORD value)
	{
		write(string, (ULONGLONG)value);
	}
	void CDllLog::write(wchar_t* string, ULONGLONG value)
	{
		if((m_hLog == NULL) || (string == NULL))
			return;

		memset(buf, 0, BUFFER_SIZE);

		if (ConvertString(string, buf))
		{
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);

			memset(buf, 0, BUFFER_SIZE);
			_ui64toa_s(value, buf, BUFFER_SIZE, 10);
			WriteFile(m_hLog, "\t\t", (DWORD)strlen("\t\t"), &len, NULL);
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);
			WriteFile(m_hLog, m_eol, (DWORD)strlen(m_eol),	&len, NULL);
		}
	}

	void CDllLog::writeHex(BYTE* array, long size)
	{
		if((m_hLog == NULL) || (array == NULL)){	return;}

#pragma warning (disable:4996)
		strcpy(out, "");
#pragma warning (default:4996)
		for(long i = 0; i < size; i++)
		{
			#pragma warning (disable:4996)
			itoa(*(array+i), buf, 16);
			strcat(out, buf);
			strcat(out, " ");
			#pragma warning (default:4996)
		}
		
		WriteFile(m_hLog, out, (DWORD)strlen(out), &len, NULL);
		WriteFile(m_hLog, m_eol, (DWORD)strlen(m_eol), &len, NULL);

	}
	void CDllLog::writeHex(wchar_t* string, DWORD value)
	{
		if((m_hLog == NULL) || (string == NULL))
			return;
		
		memset(buf, 0, BUFFER_SIZE);

		if (ConvertString(string, buf))
		{
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);
		
			memset(buf, 0, BUFFER_SIZE);			
			_ltoa_s(value, buf, BUFFER_SIZE, 16);

			WriteFile(m_hLog, "\t\t", (DWORD)strlen("\t\t"), &len, NULL);
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);
			WriteFile(m_hLog, m_eol, (DWORD)strlen(m_eol),	&len, NULL);
		}
	}

	void CDllLog::writeTime(wchar_t* string, DWORD hour, DWORD min, DWORD sec, DWORD millisec)
	{
		if((m_hLog == NULL) || (string == NULL)){	return;}

		USES_CONVERSION;			
		
		if (ConvertString(string, buf))
		{
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);
			WriteFile(m_hLog, "\t\t", (DWORD)strlen("\t\t"), &len, NULL);

			memset(buf, 0, BUFFER_SIZE);
			_ltoa_s((long)hour, buf, BUFFER_SIZE, 10);
			
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);
			WriteFile(m_hLog, ":", (DWORD)strlen(":"), &len, NULL);

			memset(buf, 0, BUFFER_SIZE);
			_ltoa_s((long)min, buf, BUFFER_SIZE, 10);
			
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);
			WriteFile(m_hLog, ":", (DWORD)strlen(":"), &len, NULL);

			memset(buf, 0, BUFFER_SIZE);
			_ltoa_s((long)sec, buf, BUFFER_SIZE, 10);
			
			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);
			WriteFile(m_hLog, ":", (DWORD)strlen(":"), &len, NULL);

			memset(buf, 0, BUFFER_SIZE);
			_ltoa_s((long)millisec, buf, BUFFER_SIZE, 10);

			WriteFile(m_hLog, buf, (DWORD)strlen(buf), &len, NULL);

			WriteFile(m_hLog, m_eol, (DWORD)strlen(m_eol),	&len, NULL);
		}
	}

	//
	void CDllLog::writeWindowsVersion(DWORD winVer)
	{
		if(m_hLog == NULL){	return;}	

		write(L"OS:", GetWinVerDescription(winVer));

		write(L"");
	}

	BSTR CDllLog::GetWinVerDescription(long code)
	{
		// need WinVer.h
		switch (code)
		{
			case 0: return L"Win_Unknown"; break;
			case 1: return L"Win95"; break;
			case 2: return L"Win95_OSR2"; break;
			case 3: return L"Win98"; break;
			case 4: return L"Win98_SE"; break;
			case 5: return L"WinMe"; break;

			case 6: return L"WinNT"; break;
			case 7: return L"WinNT_351"; break;
			case 8: return L"WinNT_40_Workstation"; break;
			case 9: return L"WinNT_40_Server"; break;
			case 10: return L"WinNT_40_Server_Enterprise"; break;

			case 11: return L"Win2000_Unknown"; break;
			case 12: return L"Win2000_Professional"; break;
			case 13: return L"Win2000_Server"; break;
			case 14: return L"Win2000_Server_Datacenter"; break;
			case 15: return L"Win2000_Server_Advanced"; break;

			case 16: return L"WinXP"; break;
			case 17: return L"WinXP_Home"; break;
			case 18: return L"WinXP_Professional"; break;
			case 19: return L"WinXP_Professional_x64"; break;

			case 20: return L"WinServer2003"; break;
			case 21: return L"WinServer2003_Enterprise"; break;
			case 22: return L"WinServer2003_Datacenter"; break;
			case 23: return L"WinServer2003_WebEdition"; break;
			case 24: return L"WinServer2003_IA64"; break;
			case 25: return L"WinServer2003_IA64_Enterprise"; break;
			case 26: return L"WinServer2003_IA64_Datacenter"; break;
			case 27: return L"WinServer2003_AMD64"; break;
			case 28: return L"WinServer2003_AMD64_Enterprise"; break;
			case 29: return L"WinServer2003_AMD64_Datacenter"; break;
			case 30: return L"WinServer2003_R2"; break;

			case 31: return L"WinVista"; break;
			case 32: return L"WinVistaUnlincensed"; break;
			case 33: return L"WinVistaUltimate"; break;
			case 34: return L"WinVistaBusiness"; break;
			case 35: return L"WinVistaEnterprise"; break;
			case 36: return L"WinVistaHome"; break;
			case 37: return L"WinVistaHomeBasic"; break;
			case 38: return L"WinVistaHomePremium"; break;
			case 39: return L"WinVistaStarter"; break;

			case 40: return L"WinServer2008"; break;
			case 41: return L"WinServer2008Unlincensed"; break;
			case 42: return L"WinServer2008_Enterprise"; break;
			case 43: return L"WinServer2008_Enterprise_Core"; break;
			case 44: return L"WinServer2008_Enterprise_Full"; break;
			case 45: return L"WinServer2008_Enterprise_IA64"; break;
			case 46: return L"WinServer2008_Datacenter"; break;
			case 47: return L"WinServer2008_Datacenter_Core"; break;
			case 48: return L"WinServer2008_Datacenter_Full"; break;
			case 49: return L"WinServer2008_Standard"; break;
			case 50: return L"WinServer2008_Standard_Core"; break;
			case 51: return L"WinServer2008_Standard_Full"; break;
			case 52: return L"WinServer2008_WebServer"; break;
			case 53: return L"WinServer2008_R2"; break;

			case 54: return L"WinHomeServer"; break;

			case 55: return L"Win7"; break;
			case 56: return L"Win7Unlincensed"; break;
			case 57: return L"Win7Ultimate"; break;
			case 58: return L"Win7Business"; break;
			case 59: return L"Win7Enterprise"; break;
			case 60: return L"Win7Home"; break;
			case 61: return L"Win7HomeBasic"; break;
			case 62: return L"Win7HomePremium"; break;
			case 63: return L"Win7Starter"; break;

			case 64: return L"Win32s"; break;

			default: return L"Undefined"; break;
		}
	}		
	
	void CDllLog::writeLine()
	{	
		if (m_hLog == NULL)
			return;
		char line[] = "------------------------------------------------------------";
		DWORD len = 0;
		WriteFile(m_hLog, line, (DWORD)(strlen(line)), &len, NULL);
		WriteFile(m_hLog, m_eol, (DWORD)strlen(m_eol), &len, NULL);
	}
	bool CDllLog::ConvertString(wchar_t* input, char* output)
	{
		if (input == NULL)
			return false;
		else
		{
			#pragma warning (disable:4800)
			int len = WideCharToMultiByte(CP_ACP, 0, input, -1, output, 0, NULL, NULL);
			if (len <= BUFFER_SIZE)
				return WideCharToMultiByte(CP_ACP, 0, input, -1, output, len, NULL, NULL);
			else
				return	WideCharToMultiByte(CP_ACP, 0, input, BUFFER_SIZE, output, BUFFER_SIZE, NULL, NULL);
			#pragma warning (default:4800)
		}
	}


private:	
	HANDLE			m_hLog;
	char			m_eol[5];

	unsigned long	len;
	char			buf[32768];
	char			out[32768];	
};