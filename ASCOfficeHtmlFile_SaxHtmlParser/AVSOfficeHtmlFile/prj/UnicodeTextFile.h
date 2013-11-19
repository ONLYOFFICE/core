#pragma once
#include <list>
#include "windows.h"
#include "atlstr.h"

namespace NUnicodeTextFile
{

	enum TBOMType {TB_ERROR, TB_UNKNOWN, TB_UTF8, TB_UTF16BE, TB_UTF16LE, TB_UTF32BE, TB_UTF32LE, TB_UTF7, TB_UTF1, TB_UTFEBCDIC, TB_SCSU, TB_BOCU1, TB_GB18030, 
		TB_WINDOWS1250, TB_WINDOWS1251, TB_WINDOWS1252, TB_WINDOWS1253, TB_WINDOWS1254, TB_WINDOWS1255, TB_WINDOWS1256, TB_WINDOWS1257, TB_WINDOWS1258, 
		TB_WINDOWS874, TB_KOI8R, TB_ISO8859_1, TB_SHIFT_JIS};

	static BOOL ConvertMultibyteToUnicode (const UINT aCodePage, const LPCSTR aInput, const int aInputSize, CStringW &aOut) throw()
	{
		ATLASSERT (aInput);

		if (0 == aInputSize)
		{
			aOut = L"";
			return TRUE;
		}

		const int nUnicodeSize = ::MultiByteToWideChar (aCodePage, 0, aInput, aInputSize, NULL, 0);
		if (0 == nUnicodeSize)
		{
			aOut = L"";
			return TRUE;
		}


		WCHAR *pwStr = aOut.GetBufferSetLength (nUnicodeSize);
		if (NULL == pwStr)
		{
			
			ATLTRACE2("(Error) NUnicodeTextFile::ConvertMultibyteToUnicode:"
				" new failed;"
				" GetLastError() returns 0x%08x.\n", ::GetLastError());
				
			return FALSE;

			
		}
		else
		{
			const int nStrSize = ::MultiByteToWideChar (aCodePage, 0, aInput, aInputSize, pwStr, nUnicodeSize);
			//ATLTRACE2 ("NUnicodeTextFile::ConvertMultibyteToUnicode(): %i bytes has been converted to unicode\n", nStrSize);
			aOut.ReleaseBuffer();
		}

		// Preprocess
		
		return TRUE;
	}
	static BOOL ConvertUnicodeToMultibyte (const UINT aCodePage, const LPCWSTR aInput, const int aInputSize, CStringA &aOut) throw()
	{
		ATLASSERT (aInput);
		//ATLASSERT (aInputSize);

		const int nAnsiSize = ::WideCharToMultiByte (aCodePage, 0, aInput, aInputSize, NULL, 0, NULL, NULL);
		if (0 != nAnsiSize)
		{
			LPSTR pBuffer = aOut.GetBufferSetLength (nAnsiSize);
			const int nWritten = ::WideCharToMultiByte (aCodePage, 0, aInput, aInputSize, pBuffer, nAnsiSize, NULL, NULL);
			ATLASSERT (nWritten == nAnsiSize);
			aOut.ReleaseBuffer();
		}
		else 
			return FALSE;
		return TRUE;
	}

	static TBOMType GetBomType (HANDLE hFile, int &nBomSize)
	{
		// read BOM
		TBOMType nBomType (TB_UNKNOWN);
		nBomSize = 0;

		DWORD dwBOM (0);
		DWORD dwRead (0);
		const BOOL bReadRes = ::ReadFile (hFile, &dwBOM, sizeof (dwBOM), &dwRead, NULL);
		if ((0 == bReadRes) || (sizeof (dwBOM) != dwRead))
		{
			ATLTRACE2("(Error) NUnicodeTextFile::GetBomType:"
					" ::ReadFile(BOM) failed;"
					" GetLastError() returns 0x%08x.\n", ::GetLastError());
			return TB_ERROR;
		}
		
		// check BOM (http://en.wikipedia.org/wiki/Byte_order_mark)
		if (0x00BFBBEF == (dwBOM & 0x00FFFFFF))
		{
			nBomType = TB_UTF8;
			nBomSize = 3;
		}
		else if (0x0000FFFE == (dwBOM & 0x0000FFFF))
		{
			nBomType = TB_UTF16BE;
			nBomSize = 2;
		}
		else if (0x0000FEFF == (dwBOM & 0x0000FFFF))
		{
			nBomType = TB_UTF16LE;
			nBomSize = 2;
		}
		else if (0xFFFE0000 == dwBOM)
		{
			nBomType = TB_UTF32BE;
			nBomSize = 4;
		}
		else if (0x0000FEFF == dwBOM)
		{
			nBomType = TB_UTF32LE;
			nBomSize = 4;
		}
		else if (0x38762F2B == dwBOM)
		{
			nBomType = TB_UTF7;
			nBomSize = 4;
		}
		else if (0x39762F2B == dwBOM)
		{
			nBomType = TB_UTF7;
			nBomSize = 4;
		}
		else if (0x2B762F2B == dwBOM)
		{
			nBomType = TB_UTF7;
			nBomSize = 4;
		}
		else if (0x2F762F2B == dwBOM)
		{
			nBomType = TB_UTF7;
			nBomSize = 4;
		}
		else if (0x004C64F7 == (dwBOM & 0x00FFFFFF))
		{
			nBomType = TB_UTF1;
			nBomSize = 3;
		}
		else if (0x736673DD == dwBOM)
		{
			nBomType = TB_UTFEBCDIC;
			nBomSize = 4;
		}
		else if (0x00FFFE0E == (dwBOM & 0x00FFFFFF))
		{
			nBomType = TB_SCSU;
			nBomSize = 3;
		}
		else if (0x0028EEFB == (dwBOM & 0x00FFFFFF))
		{
			nBomType = TB_BOCU1;
			nBomSize = 3;
		}
		else if (0x33953184 == dwBOM)
		{
			nBomType = TB_GB18030;
			nBomSize = 4;
		}

		return nBomType;
	}
	static TBOMType GetBomType (const CString &aFileName, int &aBomSize)
	{
		TBOMType aBomType (TB_ERROR);
		HANDLE hFile = ::CreateFile (aFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ATLASSERT (INVALID_HANDLE_VALUE != hFile);
		if (INVALID_HANDLE_VALUE != hFile)
		{

			aBomType = GetBomType (hFile, aBomSize);
			::CloseHandle (hFile);
			hFile = NULL;
		}
		return aBomType;
	}

	// read unicode string from multibyte text file (with conversion and BOM skipping)
	static const CStringW ReadFileMultibyte (const CString &aFileName, UINT aCodePage = CP_ACP, int aSkipFirstBytes = 0)
	{
		CStringW sResult;
		HANDLE hFile = ::CreateFile (aFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ATLASSERT (INVALID_HANDLE_VALUE != hFile);
		if (INVALID_HANDLE_VALUE != hFile)
		{
			CStringA sUtf8;
			const DWORD nFileSize = ::GetFileSize (hFile, NULL);

			LPSTR lpsrtUtf8Buffer = sUtf8.GetBufferSetLength (nFileSize);
			DWORD dwRead (0);

			const BOOL bReadRes = ::ReadFile (hFile, lpsrtUtf8Buffer, nFileSize, &dwRead, NULL);
			if ((0 == bReadRes) || (nFileSize != dwRead))
			{
				ATLTRACE2 ("(Error) NUnicodeTextFile::ReadFileMultibyte:"
						" ::ReadFile() failed;"
						" GetLastError() returns 0x%08x.\n", ::GetLastError());
				
				sUtf8.ReleaseBuffer ();
				::CloseHandle (hFile);
				return L"";
			}

			::CloseHandle (hFile);

			// convert to unicode 
			ConvertMultibyteToUnicode (CP_UTF8, lpsrtUtf8Buffer + aSkipFirstBytes, nFileSize - aSkipFirstBytes, sResult);

			sUtf8.ReleaseBuffer ();
		}
		return sResult;
	}

	// read unicode string from unicode text file (with BOM skipping)
	static const CStringW ReadFileUtf16LE (const CString &aFileName, int aSkipFirstBytes = 0)
	{
		CStringW sResult;
		HANDLE hFile = ::CreateFile (aFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ATLASSERT (INVALID_HANDLE_VALUE != hFile);
		if (INVALID_HANDLE_VALUE != hFile)
		{

			const DWORD nFileSize = ::GetFileSize (hFile, NULL);
			const int nSizeInChars = (nFileSize - aSkipFirstBytes) / sizeof (WCHAR);

			WCHAR *pwcBuffer = sResult.GetBufferSetLength (nSizeInChars);
			if (NULL != pwcBuffer)
			{
				//::memcpy (pwcBuffer, lpsz, nSizeInChars * sizeof (WCHAR));	// even quantity of chars
				if (0 != aSkipFirstBytes)
				{
					DWORD dwSeekResult = ::SetFilePointer (hFile, aSkipFirstBytes, NULL, FILE_BEGIN);
					ATLASSERT (INVALID_SET_FILE_POINTER != dwSeekResult);
				}
				DWORD dwRead (0);
				const BOOL bReadRes = ::ReadFile (hFile, pwcBuffer, nSizeInChars * sizeof (WCHAR), &dwRead, NULL);
				ATLASSERT (bReadRes);
				ATLASSERT (nFileSize != dwRead);
				if ((0 == bReadRes) || (nFileSize != dwRead))
				{
					ATLTRACE2 ("(Error) NUnicodeTextFile::ReadFileUtf16LE:"
							" ::ReadFile() failed;"
							" GetLastError() returns 0x%08x.\n", ::GetLastError());
					
					sResult.ReleaseBuffer ();
					::CloseHandle (hFile);
					return L"";
				}
				sResult.ReleaseBuffer ();
			}

		}
		return sResult;
	}
	// read unicode string from unicode text file (with conversion and BOM skipping)
	static const CStringW ReadFileUtf16BE (const CString &aFileName, int aSkipFirstBytes = 0)
	{
		CStringW sString = ReadFileUtf16LE (aFileName, aSkipFirstBytes);
		const int nLength = sString.GetLength();
		WCHAR *pwcBuffer = sString.GetBuffer ();
		if (NULL != pwcBuffer)
		{
			for (int nChar = 0; nChar < nLength; nChar++)
			{
				USHORT usChar = pwcBuffer[nChar];
				pwcBuffer[nChar] = (usChar << 8) | (usChar >> 8);
			}
			sString.ReleaseBuffer ();
		}
		return sString;
	}
	// read unicode string from text file (with BOM detection and skipping)
	static const CStringW ReadFile (const CString aFileName)
	{
		int nBomSize (0);
		const TBOMType aBomType = GetBomType (aFileName, nBomSize);
		if (TB_UTF8 == aBomType)
			return ReadFileMultibyte (aFileName, CP_UTF8, nBomSize);
		else if (TB_UTF16LE == aBomType)
			return ReadFileUtf16LE (aFileName, nBomSize);
		else if (TB_UTF16BE == aBomType)
			return ReadFileUtf16BE (aFileName, nBomSize);
		else 
			return ReadFileMultibyte (aFileName, CP_ACP);
	}
	// read list of unicode stl strings from file (with BOM detection and skipping)
	static void DeleteBadChars (CString &aLine)
	{
		static const TCHAR * BadSymbols = _T("\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19");
		int nBadPos = aLine.FindOneOf (BadSymbols);
		while (-1 != nBadPos)
		{
			aLine.Delete (nBadPos);
			nBadPos = aLine.FindOneOf (BadSymbols);
		}
	}
	static const std::list<std::wstring> ReadFileToList (const CString &aFileName)
	{
		CStringW sFileContent = ReadFile (aFileName);
		std::list<std::wstring> lines;
		
		// split a string by lines
		int nPos = 0;
		const int nLength = sFileContent.GetLength();
		while (nPos < nLength)
		{
			const int nReturnPos = sFileContent.Find ('\r', nPos);
			if (-1 == nReturnPos)
			{
				CString sLine = sFileContent.Mid (nPos);
				DeleteBadChars (sLine);

				lines.push_back (std::wstring (sLine));
				break;
			}
			else
			{
				CString sLine = sFileContent.Mid (nPos, nReturnPos - nPos);
				DeleteBadChars (sLine);
				lines.push_back (std::wstring (sLine));
			}
			nPos = nReturnPos + 1;
		}
		return lines;
	}
	static std::wstring utf82unicode (const std::string& line)
	{
		LPCSTR pBuffer = line.c_str();
		DWORD dwBufferSize = line.size();

		CStringW sResult;
		ConvertMultibyteToUnicode (CP_UTF8, pBuffer, dwBufferSize, sResult);
		return (std::wstring) sResult.GetString();
	}
	static std::wstring ansi2unicode (const std::string& line)
	{
		LPCSTR pBuffer = line.c_str();
		DWORD dwBufferSize = line.size();

		CStringW sResult;
		ConvertMultibyteToUnicode (CP_ACP, pBuffer, dwBufferSize, sResult);
		return (std::wstring) sResult.GetString();
	}
	static std::string unicode2ansi (const std::wstring& line)
	{
		LPCWSTR pBuffer = line.c_str();
		DWORD dwBufferSize = line.size();

		CStringA sResult;
		ConvertUnicodeToMultibyte (CP_ACP, pBuffer, dwBufferSize, sResult);
		return (std::string) sResult.GetString();
	}
	
};
