#ifndef _BUILD_FILE_CROSSPLATFORM_H_
#define _BUILD_FILE_CROSSPLATFORM_H_

#include <stdio.h>
#include <string>
#include <fstream>
#include <time.h>
#include "Array.h"
#include "errno.h"
#include "Base64.h"
#include <time.h>

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
	#include <wchar.h>
	#include <windows.h>
#endif

#define U_TO_UTF8(val) NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(val.c_str(), val.length())

#if defined(__linux__) || defined(_MAC) && !defined(_IOS)
#include <unistd.h>
#endif

#ifndef MAX_PATH
    #define MAX_PATH 1024
#endif

//#include "../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
//перенесено выше

namespace NSFile
{
	#define WriteUtf16_WCHAR(code, p)				\
	if (code < 0x10000)								\
		*p++ = code;								\
	else											\
	{												\
		code -= 0x10000;							\
		*p++ = 0xD800 | ((code >> 10) & 0x03FF);	\
		*p++ = 0xDC00 | (code & 0x03FF);			\
	}

	class CStringUtf16
	{
	public:
		BYTE* Data;
		int Length;

	public:
		CStringUtf16()
		{
			Data = NULL;
			Length = 0;
		}
		~CStringUtf16()
		{
			RELEASEARRAYOBJECTS(Data);
		}
	};   

	class CUtf8Converter
	{
	public:

		static std::wstring GetUnicodeFromCharPtr(const char* pData, LONG lCount, INT bIsUtf8 = FALSE)
		{
			if (bIsUtf8)
				return GetUnicodeStringFromUTF8((BYTE*)pData, lCount);

			wchar_t* pUnicode = new wchar_t[lCount + 1];
			for (LONG i = 0; i < lCount; ++i)
				pUnicode[i] = (wchar_t)(BYTE)pData[i];

			pUnicode[lCount] = 0;

			std::wstring s(pUnicode, lCount);
			RELEASEARRAYOBJECTS(pUnicode);

			return s;
		}
		static std::wstring GetUnicodeFromCharPtr(const std::string& sParam, INT bIsUtf8 = FALSE)
		{
			return GetUnicodeFromCharPtr(sParam.c_str(), (LONG)sParam.length(), bIsUtf8);			
		}
		static std::wstring GetUnicodeStringFromUTF8_4bytes( BYTE* pBuffer, LONG lCount )
		{
			WCHAR* pUnicodeString = new WCHAR[lCount + 1];
			LONG lIndexUnicode = 0;

			LONG lIndex = 0;
			while (lIndex < lCount)
			{
				BYTE byteMain = pBuffer[lIndex];
				if (0x00 == (byteMain & 0x80))
				{
					// 1 byte
					pUnicodeString[lIndexUnicode++] = (WCHAR)byteMain;
					++lIndex;
				}
				else if (0x00 == (byteMain & 0x20))
				{
					// 2 byte
					int val = (int)(((byteMain & 0x1F) << 6) |
						(pBuffer[lIndex + 1] & 0x3F));
					pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
					lIndex += 2;
				}
				else if (0x00 == (byteMain & 0x10))
				{
					// 3 byte
					int val = (int)(((byteMain & 0x0F) << 12) |
						((pBuffer[lIndex + 1] & 0x3F) << 6) |
						(pBuffer[lIndex + 2] & 0x3F));
					pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
					lIndex += 3;
				}
				else if (0x00 == (byteMain & 0x0F))
				{
					// 4 byte
					int val = (int)(((byteMain & 0x07) << 18) |
						((pBuffer[lIndex + 1] & 0x3F) << 12) |
						((pBuffer[lIndex + 2] & 0x3F) << 6) |
						(pBuffer[lIndex + 3] & 0x3F));
					pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
					lIndex += 4;
				}
				else if (0x00 == (byteMain & 0x08))
				{
					// 4 byte
					int val = (int)(((byteMain & 0x07) << 18) |
						((pBuffer[lIndex + 1] & 0x3F) << 12) |
						((pBuffer[lIndex + 2] & 0x3F) << 6) |
						(pBuffer[lIndex + 3] & 0x3F));
					pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
					lIndex += 4;
				}
				else if (0x00 == (byteMain & 0x04))
				{
					// 5 byte
					int val = (int)(((byteMain & 0x03) << 24) |
						((pBuffer[lIndex + 1] & 0x3F) << 18) |
						((pBuffer[lIndex + 2] & 0x3F) << 12) |
						((pBuffer[lIndex + 3] & 0x3F) << 6) |
						(pBuffer[lIndex + 4] & 0x3F));
					pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
					lIndex += 5;
				}
				else
				{
					// 6 byte
					int val = (int)(((byteMain & 0x01) << 30) |
						((pBuffer[lIndex + 1] & 0x3F) << 24) |
						((pBuffer[lIndex + 2] & 0x3F) << 18) |
						((pBuffer[lIndex + 3] & 0x3F) << 12) |
						((pBuffer[lIndex + 4] & 0x3F) << 6) |
						(pBuffer[lIndex + 5] & 0x3F));
					pUnicodeString[lIndexUnicode++] = (WCHAR)(val);
					lIndex += 5;
				}
			}

			pUnicodeString[lIndexUnicode] = 0;
			
			std::wstring strRes(pUnicodeString);
			
			delete [] pUnicodeString;
			
			return strRes;
		}
		static std::wstring GetUnicodeStringFromUTF8_2bytes( BYTE* pBuffer, LONG lCount )
		{
			WCHAR* pUnicodeString = new WCHAR[lCount + 1];
			WCHAR* pStart = pUnicodeString;
			
			LONG lIndex = 0;
			while (lIndex < lCount)
			{
				BYTE byteMain = pBuffer[lIndex];
				if (0x00 == (byteMain & 0x80))
				{
					// 1 byte
					*pUnicodeString++ = (WCHAR)byteMain;
					++lIndex;
				}
				else if (0x00 == (byteMain & 0x20))
				{
					// 2 byte
					int val = (int)(((byteMain & 0x1F) << 6) |
						(pBuffer[lIndex + 1] & 0x3F));
					*pUnicodeString++ = (WCHAR)(val);
					lIndex += 2;
				}
				else if (0x00 == (byteMain & 0x10))
				{
					// 3 byte
					int val = (int)(((byteMain & 0x0F) << 12) |
						((pBuffer[lIndex + 1] & 0x3F) << 6) |
						(pBuffer[lIndex + 2] & 0x3F));

					WriteUtf16_WCHAR(val, pUnicodeString);
					lIndex += 3;
				}
				else if (0x00 == (byteMain & 0x0F))
				{
					// 4 byte
					int val = (int)(((byteMain & 0x07) << 18) |
						((pBuffer[lIndex + 1] & 0x3F) << 12) |
						((pBuffer[lIndex + 2] & 0x3F) << 6) |
						(pBuffer[lIndex + 3] & 0x3F));
					
					WriteUtf16_WCHAR(val, pUnicodeString);
					lIndex += 4;
				}
				else if (0x00 == (byteMain & 0x08))
				{
					// 4 byte
					int val = (int)(((byteMain & 0x07) << 18) |
						((pBuffer[lIndex + 1] & 0x3F) << 12) |
						((pBuffer[lIndex + 2] & 0x3F) << 6) |
						(pBuffer[lIndex + 3] & 0x3F));
					
					WriteUtf16_WCHAR(val, pUnicodeString);
					lIndex += 4;
				}
				else if (0x00 == (byteMain & 0x04))
				{
					// 5 byte
					int val = (int)(((byteMain & 0x03) << 24) |
						((pBuffer[lIndex + 1] & 0x3F) << 18) |
						((pBuffer[lIndex + 2] & 0x3F) << 12) |
						((pBuffer[lIndex + 3] & 0x3F) << 6) |
						(pBuffer[lIndex + 4] & 0x3F));

					WriteUtf16_WCHAR(val, pUnicodeString);
					lIndex += 5;
				}
				else
				{
					// 6 byte
					int val = (int)(((byteMain & 0x01) << 30) |
						((pBuffer[lIndex + 1] & 0x3F) << 24) |
						((pBuffer[lIndex + 2] & 0x3F) << 18) |
						((pBuffer[lIndex + 3] & 0x3F) << 12) |
						((pBuffer[lIndex + 4] & 0x3F) << 6) |
						(pBuffer[lIndex + 5] & 0x3F));

					WriteUtf16_WCHAR(val, pUnicodeString);
					lIndex += 5;
				}
			}

			*pUnicodeString++ = 0;
			
			std::wstring strRes(pStart);
			
			delete [] pStart;
			
			return strRes;
		}

		static std::wstring GetUnicodeStringFromUTF8( BYTE* pBuffer, LONG lCount )
		{
			if (sizeof(WCHAR) == 2)
				return GetUnicodeStringFromUTF8_2bytes(pBuffer, lCount);
			return GetUnicodeStringFromUTF8_4bytes(pBuffer, lCount);
		}

		static void GetUtf8StringFromUnicode_4bytes(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, LONG& lOutputCount, bool bIsBOM = false)
		{
			if (NULL == pData)
			{
                pData = new BYTE[6 * lCount + 3 + 1 ];
			}

			BYTE* pCodesCur = pData;
			if (bIsBOM)
			{
				pCodesCur[0] = 0xEF;
				pCodesCur[1] = 0xBB;
				pCodesCur[2] = 0xBF;
				pCodesCur += 3;
			}

			const wchar_t* pEnd = pUnicodes + lCount;
			const wchar_t* pCur = pUnicodes;

			while (pCur < pEnd)
			{
				unsigned int code = (unsigned int)*pCur++;

				if (code < 0x80)
				{
					*pCodesCur++ = (BYTE)code;
				}
				else if (code < 0x0800)
				{
					*pCodesCur++ = 0xC0 | (code >> 6);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x10000)
				{
					*pCodesCur++ = 0xE0 | (code >> 12);
					*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x1FFFFF)
				{
					*pCodesCur++ = 0xF0 | (code >> 18);
					*pCodesCur++ = 0x80 | (code >> 12 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x3FFFFFF)
				{
					*pCodesCur++ = 0xF8 | (code >> 24);
					*pCodesCur++ = 0x80 | (code >> 18 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 12 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x7FFFFFFF)
				{
					*pCodesCur++ = 0xFC | (code >> 30);
					*pCodesCur++ = 0x80 | (code >> 24 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 18 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 12 & 0x3F);
					*pCodesCur++ = 0x80 | (code >> 6 & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
			}

			lOutputCount = (LONG)(pCodesCur - pData);
            *pCodesCur++ = 0;
		}

		static void GetUtf8StringFromUnicode_2bytes(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, LONG& lOutputCount, bool bIsBOM = false)
		{
			if (NULL == pData)
			{
				pData = new BYTE[6 * lCount + 3 + 1];
			}

			BYTE* pCodesCur = pData;
			if (bIsBOM)
			{
				pCodesCur[0] = 0xEF;
				pCodesCur[1] = 0xBB;
				pCodesCur[2] = 0xBF;
				pCodesCur += 3;
			}

			const wchar_t* pEnd = pUnicodes + lCount;
			const wchar_t* pCur = pUnicodes;

			while (pCur < pEnd)
			{
				unsigned int code = (unsigned int)*pCur++;
				if (code >= 0xD800 && code <= 0xDFFF && pCur < pEnd)
				{
					code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & *pCur++));
				}

				if (code < 0x80)
				{
					*pCodesCur++ = (BYTE)code;
				}
				else if (code < 0x0800)
				{
					*pCodesCur++ = 0xC0 | (code >> 6);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x10000)
				{
					*pCodesCur++ = 0xE0 | (code >> 12);
					*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x1FFFFF)
				{
					*pCodesCur++ = 0xF0 | (code >> 18);
					*pCodesCur++ = 0x80 | ((code >> 12) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x3FFFFFF)
				{
					*pCodesCur++ = 0xF8 | (code >> 24);
					*pCodesCur++ = 0x80 | ((code >> 18) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 12) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
				else if (code < 0x7FFFFFFF)
				{
					*pCodesCur++ = 0xFC | (code >> 30);
					*pCodesCur++ = 0x80 | ((code >> 24) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 18) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 12) & 0x3F);
					*pCodesCur++ = 0x80 | ((code >> 6) & 0x3F);
					*pCodesCur++ = 0x80 | (code & 0x3F);
				}
			}

			lOutputCount = (LONG)(pCodesCur - pData);
			*pCodesCur++ = 0;
		}

		static void GetUtf8StringFromUnicode(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, LONG& lOutputCount, bool bIsBOM = false)
		{
			if (sizeof(WCHAR) == 2)
				return GetUtf8StringFromUnicode_2bytes(pUnicodes, lCount, pData, lOutputCount, bIsBOM);
			return GetUtf8StringFromUnicode_4bytes(pUnicodes, lCount, pData, lOutputCount, bIsBOM);
		}

		static std::string GetUtf8StringFromUnicode2(const wchar_t* pUnicodes, LONG lCount, bool bIsBOM = false)
		{
			BYTE* pData = NULL;
			LONG lLen = 0;

			GetUtf8StringFromUnicode(pUnicodes, lCount, pData, lLen, bIsBOM);

			std::string s((char*)pData, lLen);

			RELEASEARRAYOBJECTS(pData);
			return s;
		}

        static std::string GetUtf8StringFromUnicode(const std::wstring& sData)
        {
                return GetUtf8StringFromUnicode2(sData.c_str(), (LONG)sData.length());
        }

		// utf16
        static void GetUtf16StringFromUnicode_4bytes(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, int& lOutputCount, bool bIsBOM = false)
		{
			if (NULL == pData)
			{
				pData = new BYTE[4 * lCount + 3 + 2];
			}

			BYTE* pCodesCur = pData;
			if (bIsBOM)
			{
				pCodesCur[0] = 0xEF;
				pCodesCur[1] = 0xBB;
				pCodesCur[2] = 0xBF;
				pCodesCur += 3;
			}

			const wchar_t* pEnd = pUnicodes + lCount;
			const wchar_t* pCur = pUnicodes;

			while (pCur < pEnd)
			{
				unsigned int code = (unsigned int)*pCur++;

				if (code <= 0xFFFF)
				{
					USHORT usCode = (USHORT)(code & 0xFFFF);
					memcpy(pCodesCur, &usCode, 2);
					pCodesCur += 2;
				}
				else
				{
					code -= 0x10000;
					code &= 0xFFFFF;
					
					USHORT us1 = 0xD800 | ((code >> 5) & 0x1F);
					USHORT us2 = 0xDC00 | (code & 0x1F);

					memcpy(pCodesCur, &us1, 2);
					pCodesCur += 2;

					memcpy(pCodesCur, &us2, 2);
					pCodesCur += 2;
				}				
			}

			lOutputCount = (LONG)(pCodesCur - pData);
			*pCodesCur++ = 0;
			*pCodesCur++ = 0;
		}

		static void GetUtf16StringFromUnicode_4bytes2(const wchar_t* pUnicodes, LONG lCount, CStringUtf16& data)
		{
			GetUtf16StringFromUnicode_4bytes(pUnicodes, lCount, data.Data, data.Length);
		}
	
		static std::wstring GetWStringFromUTF16(const CStringUtf16& data)
		{
			if (0 == data.Length)
				return L"";

			if (sizeof(wchar_t) == 2)
				return std::wstring((wchar_t*)data.Data, data.Length / 2);

			int nCount = data.Length / 2;
			USHORT* pShort = (USHORT*)data.Data;

			wchar_t* pWChar = new wchar_t[nCount + 1];
			wchar_t* pWCurrent = pWChar;
			
			int nCurrent = 0;
			while (nCurrent < nCount)
			{
				if (*pShort < 0xD800 || *pShort > 0xDFFF)
				{
					*pWCurrent = (wchar_t)(*pShort);
					++pShort;
					++nCurrent;
				}
				else
				{
					*pWCurrent = (wchar_t)((((pShort[0]) & 0x03FF) << 10) | ((pShort[1]) & 0x03FF));
					pShort += 2;
					nCurrent += 2;
				}
				++pWCurrent;
			}

			std::wstring sRet(pWChar, pWCurrent - pWChar);

			RELEASEARRAYOBJECTS(pWChar);
			return sRet;
		}
		static std::wstring GetWStringFromUTF16(const unsigned short* pUtf16, LONG lCount)
		{
			CStringUtf16 oString;
			oString.Data   = (BYTE*)pUtf16;
			oString.Length = lCount * 2;
			std::wstring wsResult = GetWStringFromUTF16(oString);
			oString.Data = NULL;
			return wsResult;
		}
	};

	class CFileBinary
	{
	protected:
		FILE* m_pFile;

		long m_lFilePosition;
		long m_lFileSize;

	public:
		CFileBinary()
		{
			m_pFile = NULL;
			m_lFilePosition = 0;
			m_lFileSize = 0;
		}
		~CFileBinary()
		{
			CloseFile();
		}

		void CloseFile()
		{
			m_lFilePosition = 0;
			m_lFileSize = 0;

			if (m_pFile != NULL)
			{
				fclose(m_pFile);
				m_pFile = NULL;
			}
		}

		inline FILE* GetFileNative()
		{
			return m_pFile;
		}
		inline long GetFileSize()
		{
			return m_lFileSize;
		}
		inline long GetFilePosition()
		{
			return m_lFilePosition;
		}
		bool OpenFile(const std::wstring& sFileName, bool bRewrite = false)
		{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
			m_pFile = _wfopen(sFileName.c_str(), bRewrite ? L"rb+" : L"rb");
#else
			BYTE* pUtf8 = NULL;
			LONG lLen = 0;
			CUtf8Converter::GetUtf8StringFromUnicode(sFileName.c_str(), sFileName.length(), pUtf8, lLen, false);
			m_pFile = fopen((char*)pUtf8, bRewrite ? "rb+" : "rb");

			delete [] pUtf8;
#endif		
			if (NULL == m_pFile)
				return false;

			fseek(m_pFile, 0, SEEK_END);
			m_lFileSize = ftell(m_pFile);
			fseek(m_pFile, 0, SEEK_SET);

			m_lFilePosition = 0;

			if (0 < sFileName.length())
			{
				if (((wchar_t)'/') == sFileName.c_str()[sFileName.length() - 1])
					m_lFileSize = 0x7FFFFFFF;
			}

			unsigned int err = 0x7FFFFFFF;
			unsigned int cur = (unsigned int)m_lFileSize;
			if (err == cur)
			{
				CloseFile();
				return false;
			}

			return true;
		}

		bool CreateFileW(const std::wstring& sFileName)
		{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
			m_pFile = _wfopen(sFileName.c_str(), L"wb");
#else
			BYTE* pUtf8 = NULL;
			LONG lLen = 0;
			CUtf8Converter::GetUtf8StringFromUnicode(sFileName.c_str(), sFileName.length(), pUtf8, lLen, false);
			m_pFile = fopen((char*)pUtf8, "wb");
			delete [] pUtf8;
#endif		
			if (NULL == m_pFile)
				return false;

			m_lFilePosition = 0;
			return true;
		}
		bool CreateTempFile()
		{
			m_pFile = tmpfile();
			if (NULL == m_pFile)
				return false;

			m_lFilePosition = 0;
			return true;
		}
		bool SeekFile(int lFilePosition, int nSeekMode = 0)
		{
			if (!m_pFile)
				return false;

			m_lFilePosition = fseek(m_pFile, lFilePosition, nSeekMode);
			return true;
		}
		bool ReadFile(BYTE* pData, DWORD nBytesToRead, DWORD& dwSizeRead)
		{
			if (!m_pFile)
				return false;

			dwSizeRead = (DWORD)fread((void*)pData, 1, nBytesToRead, m_pFile);
			return true;
		}
		bool WriteFile(const BYTE* pData, DWORD nBytesCount)
		{
			if (!m_pFile)
				return false;

			size_t nCountWrite = fwrite((const void*)pData, 1, nBytesCount, m_pFile);
			return true;
		}
		long TellFile()
		{
			if (!m_pFile)
				return 0;

			return ftell(m_pFile);
		}
		long SizeFile()
		{
			if (!m_pFile)
				return 0;

			long lPos = TellFile();
			fseek(m_pFile, 0, SEEK_END);
			m_lFileSize = ftell(m_pFile);
			fseek(m_pFile, lPos, SEEK_SET);

			return m_lFileSize;
		}
		void WriteStringUTF8(const std::wstring& strXml, bool bIsBOM = false)
		{
			BYTE* pData = NULL;
			LONG lLen = 0;

			CUtf8Converter::GetUtf8StringFromUnicode(strXml.c_str(), (LONG)strXml.length(), pData, lLen, bIsBOM);

			WriteFile(pData, lLen);

			RELEASEARRAYOBJECTS(pData);
		}
		static bool ReadAllBytes(const std::wstring&  strFileName, BYTE** ppData, DWORD& nBytesCount)
		{
			*ppData = NULL;
			nBytesCount = 0;
			bool bRes = false;
			CFileBinary oFileBinary;
			if (oFileBinary.OpenFile(strFileName))
			{
				long nFileSize = oFileBinary.GetFileSize();
				BYTE* pData = new BYTE[nFileSize];
				DWORD dwSizeRead;
				if (oFileBinary.ReadFile(pData, nFileSize, dwSizeRead))
				{
					oFileBinary.CloseFile();
					*ppData = pData;
					nBytesCount = dwSizeRead;
					bRes = true;
				}
				else
					RELEASEARRAYOBJECTS(pData);
			}
			return bRes;
		}
		static bool ReadAllTextUtf8(const std::wstring&  strFileName, std::wstring& sData)
		{
			bool bRes = false;
			BYTE* pData = NULL;
			DWORD nDataSize;
			if (CFileBinary::ReadAllBytes(strFileName, &pData, nDataSize))
			{
				//remove BOM if exist
				BYTE* pDataStart = pData;
				int nBOMSize = 3;
				if (nDataSize > nBOMSize && 0xef == pDataStart[0] && 0xbb == pDataStart[1] && 0xbf == pDataStart[2])
				{
					pDataStart += nBOMSize;
					nDataSize -= nBOMSize;
				}
				sData = CUtf8Converter::GetUnicodeStringFromUTF8(pDataStart, nDataSize);
				RELEASEARRAYOBJECTS(pData);
				bRes = true;
			}
			return bRes;
		}
		static bool ReadAllTextUtf8A(const std::wstring&  strFileName, std::string& sData)
		{
			bool bRes = false;
			BYTE* pData = NULL;
			DWORD nDataSize;
			if (CFileBinary::ReadAllBytes(strFileName, &pData, nDataSize))
			{
				//remove BOM if exist
				BYTE* pDataStart = pData;
				int nBOMSize = 3;
				if (nDataSize > nBOMSize && 0xef == pDataStart[0] && 0xbb == pDataStart[1] && 0xbf == pDataStart[2])
				{
					pDataStart += nBOMSize;
					nDataSize -= nBOMSize;
				}
				sData = std::string((char*)pDataStart, nDataSize);
				RELEASEARRAYOBJECTS(pData);
				bRes = true;
			}
			return bRes;
		}
		static bool SaveToFile(const std::wstring&  strFileName, const std::wstring& strXml, bool bIsBOM = false)
		{
			CFileBinary oFile;
			oFile.CreateFileW(strFileName);
			oFile.WriteStringUTF8(strXml, bIsBOM);
			oFile.CloseFile();
			return true;
		}
		static bool Exists(const std::wstring&  strFileName)
		{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
			FILE* pFile = _wfopen(strFileName.c_str(), L"rb");
#else
			BYTE* pUtf8 = NULL;
			LONG lLen = 0;
			CUtf8Converter::GetUtf8StringFromUnicode(strFileName.c_str(), strFileName.length(), pUtf8, lLen, false);
			FILE* pFile = fopen((char*)pUtf8, "rb");
			delete [] pUtf8;
#endif
			if (NULL != pFile)
			{
				fclose(pFile);
				return true;
			}
			else
				return false;
		}
		static bool Copy(const std::wstring&  strSrc, const std::wstring&  strDst)
		{
			if (strSrc == strDst)
				return true;
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
			std::wifstream  src(strSrc.c_str(), std::ios::binary);
			std::wofstream  dst(strDst.c_str(), std::ios::binary);
#else
			BYTE* pUtf8Src = NULL;
			LONG lLenSrc = 0;
			CUtf8Converter::GetUtf8StringFromUnicode(strSrc.c_str(), strSrc.length(), pUtf8Src, lLenSrc, false);
			BYTE* pUtf8Dst = NULL;
			LONG lLenDst = 0;
			CUtf8Converter::GetUtf8StringFromUnicode(strDst.c_str(), strDst.length(), pUtf8Dst, lLenDst, false);

			std::ifstream  src((char*)pUtf8Src, std::ios::binary);
			std::ofstream  dst((char*)pUtf8Dst, std::ios::binary);

			delete [] pUtf8Src;
			delete [] pUtf8Dst;
#endif
			if (src.is_open() && dst.is_open())
			{
				dst << src.rdbuf();
				src.close();
				dst.close();
				return true;
			}
			else
				return false;
		}
		static bool Remove(const std::wstring& strFileName)
		{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
			int nRes = _wremove(strFileName.c_str());
#else
			BYTE* pUtf8 = NULL;
			LONG lLen = 0;
			CUtf8Converter::GetUtf8StringFromUnicode(strFileName.c_str(), strFileName.length(), pUtf8, lLen, false);
			int nRes = std::remove((char*)pUtf8);
			delete [] pUtf8;
#endif
			return 0 == nRes;
		}
		static bool Move(const std::wstring&  strSrc, const std::wstring&  strDst)
		{
			if (strSrc == strDst)
				return true;
			if (Copy(strSrc, strDst))
				if (Remove(strSrc))
					return true;
			return false;
		}

		static std::wstring GetTempPath()
		{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
			wchar_t pBuffer[MAX_PATH + 1];
			memset(pBuffer, 0, sizeof(wchar_t) * (MAX_PATH + 1));
			::GetTempPathW(MAX_PATH, pBuffer);

			std::wstring sRet(pBuffer);

			size_t nSeparatorPos = sRet.find_last_of(wchar_t('/'));
			if (std::wstring::npos == nSeparatorPos)
			{
				nSeparatorPos = sRet.find_last_of(wchar_t('\\'));
			}

			if (std::wstring::npos == nSeparatorPos)
				return L"";

			return sRet.substr(0, nSeparatorPos);
#else
			char *folder = getenv("TEMP");

			if (NULL == folder)
				folder = getenv("TMP");
			if (NULL == folder)
				folder = getenv("TMPDIR");

			if (NULL == folder)
				folder = "/tmp";

			return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)folder, strlen(folder));
#endif			
		}
		static std::wstring CreateTempFileWithUniqueName(const std::wstring& strFolderPathRoot, const std::wstring& Prefix)
		{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
			wchar_t pBuffer[MAX_PATH + 1];
			::GetTempFileNameW(strFolderPathRoot.c_str(), Prefix.c_str(), 0, pBuffer);
			std::wstring sRet(pBuffer);
			return sRet;
#else
			char pcRes[MAX_PATH];
			BYTE* pData = (BYTE*)pcRes;

			std::wstring sPrefix = strFolderPathRoot + L"/" + Prefix + L"_XXXXXX";
			LONG lLen = 0;
			NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sPrefix.c_str(), (LONG)sPrefix.length(), pData, lLen);
			pcRes[lLen] = '\0';

			int res = mkstemp(pcRes);

			std::string sRes = pcRes;
			return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sRes.c_str(), sRes.length());
#endif	
		}
		static bool OpenTempFile(std::wstring *pwsName, FILE **ppFile, wchar_t *wsMode, wchar_t *wsExt, wchar_t *wsFolder, wchar_t* wsName = NULL)
		{
			// TODO: Реализовать когда wsName != NULL

			std::wstring wsTemp, wsFileName;
			FILE *pTempFile = NULL;
#if defined(_WIN32) || defined (_WIN64)
			wchar_t *wsTempDir;
			if ((wsTempDir = _wgetenv(L"TEMP")) && (wsFolder == NULL))
			{
				wsTemp = std::wstring(wsTempDir);
#else
			char *wsTempDirA;
			if ((wsTempDirA = getenv("TEMP")) && (wsFolder == NULL))
			{
				std::wstring wsTempDir = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)wsTempDirA, strlen(wsTempDirA));
				wsTemp = wsTempDir.c_str();
#endif
				wsTemp.append(L"/");
			}
			else if (wsFolder != NULL)
			{
				wsTemp = std::wstring(wsFolder);
				wsTemp.append(L"/");
			}
			else
			{
				wsTemp = L"";
			}
			wsTemp.append(L"x");
			int nTime = (int)time(NULL);
			for (int nIndex = 0; nIndex < 1000; ++nIndex)
			{
				wsFileName = wsTemp;
#if defined(_WIN32) || defined (_WIN64)
				wchar_t buff[32] ={};
				_itow(nTime + nIndex, buff, 10);
				wsFileName.append(buff, wcslen(buff));
#else
				wsFileName.append(std::to_wstring(nTime + nIndex));
#endif
				if (wsExt)
				{
					wsFileName.append(wsExt);
				}
#if defined (_WIN32) || defined (_WIN64)
				if (!(pTempFile = _wfopen(wsFileName.c_str(), L"r")))
				{
					if (!(pTempFile = _wfopen(wsFileName.c_str(), wsMode)))
#else
				std::string sFileName = U_TO_UTF8(wsFileName);
				if (!(pTempFile = fopen(sFileName.c_str(), "r")))
				{
					std::wstring strMode(wsMode);
					std::string sMode = U_TO_UTF8(strMode);
					if (!(pTempFile = fopen(sFileName.c_str(), sMode.c_str())))
#endif
					{
						return FALSE;
					}
					*pwsName = wsFileName;
					*ppFile = pTempFile;
					return TRUE;
				}

				fclose(pTempFile);
			}

			return FALSE;
		}
		static FILE* OpenFileNative(const std::wstring& sFileName, const std::wstring& sMode)
		{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
			return _wfopen(sFileName.c_str(), sMode.c_str());
#else
			BYTE* pUtf8 = NULL;
			LONG lLen = 0;
			CUtf8Converter::GetUtf8StringFromUnicode(sFileName.c_str(), sFileName.length(), pUtf8, lLen, false);

			BYTE* pMode = NULL;
			LONG lLenMode;
			CUtf8Converter::GetUtf8StringFromUnicode(sMode.c_str(), sMode.length(), pMode, lLenMode, false);

			FILE* pFile = fopen((char*)pUtf8, (char*)pMode);

			delete [] pUtf8;
			delete [] pMode;

			return pFile;
#endif
		}
	};

	class CBase64Converter
	{
	public:
		static bool Encode(BYTE* pDataSrc, int nLenSrc, char*& pDataDst, int& nLenDst, DWORD dwFlags = NSBase64::B64_BASE64_FLAG_NONE)
		{
			if (!pDataSrc || nLenSrc < 1)
				return false;

			nLenDst = NSBase64::Base64EncodeGetRequiredLength(nLenSrc, dwFlags);
			pDataDst = new char[nLenDst];

			if (FALSE == NSBase64::Base64Encode(pDataSrc, nLenSrc, (BYTE*)pDataDst, &nLenDst, dwFlags))
			{
				RELEASEARRAYOBJECTS(pDataDst);
				return false;
			}
			return true;
		}
                static bool Decode(const char* pDataSrc, int nLenSrc, BYTE*& pDataDst, int& nLenDst)
		{
			if (!pDataSrc || nLenSrc < 1)
				return false;

			nLenDst = NSBase64::Base64DecodeGetRequiredLength(nLenSrc);
			pDataDst = new BYTE[nLenDst];

			if (FALSE == NSBase64::Base64Decode(pDataSrc, nLenSrc, pDataDst, &nLenDst))
			{
				RELEASEARRAYOBJECTS(pDataDst);
				return false;
			}
			return true;
		}
	};
}

namespace NSFile
{
    #define NS_FILE_MAX_PATH 32768
    static std::wstring GetProcessPath()
    {
#if defined (_WIN64) || defined(_WIN32)
        wchar_t buf [NS_FILE_MAX_PATH];
        GetModuleFileNameW(GetModuleHandle(NULL), buf, NS_FILE_MAX_PATH);
        return std::wstring(buf);
#endif

#if defined(__linux__) || defined(_MAC) && !defined(_IOS)
        char buf[NS_FILE_MAX_PATH];
        if (readlink ("/proc/self/exe", buf, NS_FILE_MAX_PATH) <= 0)
        {
#ifdef _MAC
            getcwd(buf, sizeof(buf));
            std::string sUTF8(buf);
            std::wstring sRet = CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sUTF8.c_str(), sUTF8.length());
            return sRet;
#endif
            return L"";
        }

        std::string sUTF8(buf);
        std::wstring sRet = CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sUTF8.c_str(), sUTF8.length());
        return sRet;
#endif

        return L"";
    }

    static std::wstring GetProcessDirectory()
    {
        std::wstring sPath = GetProcessPath();

        size_t pos1 = sPath.find_last_of(wchar_t('/'));
        size_t pos2 = sPath.find_last_of(wchar_t('\\'));

        size_t pos = std::wstring::npos;
        if (pos1 != std::wstring::npos)
            pos = pos1;

        if (pos2 != std::wstring::npos)
        {
            if (pos == std::wstring::npos)
                pos = pos2;
            else if (pos2 > pos)
                pos = pos2;
        }

        if (pos != std::wstring::npos)
        {
            sPath = sPath.substr(0, pos);
        }
        return sPath;
    }
}

#endif //_BUILD_FILE_CROSSPLATFORM_H_
