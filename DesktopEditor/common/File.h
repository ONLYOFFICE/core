#ifndef _BUILD_FILE_CROSSPLATFORM_H_
#define _BUILD_FILE_CROSSPLATFORM_H_

#include <stdio.h>
#include <string>
#include "Array.h"

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

	class CUtf8Converter
	{
	public:
		static std::wstring GetUnicodeFromCharPtr(const char* pData, LONG lCount, BOOL bIsUtf8 = FALSE)
		{
			if (bIsUtf8)
				return GetUnicodeStringFromUTF8((BYTE*)pData, lCount);

			wchar_t* pUnicode = new wchar_t[lCount + 1];
			for (LONG i = 0; i < lCount; ++i)
				pUnicode[i] = (wchar_t)pData[i];

			pUnicode[lCount] = 0;

			std::wstring s(pUnicode, lCount);
			RELEASEARRAYOBJECTS(pUnicode);

			return s;
		}
		static std::wstring GetUnicodeFromCharPtr(std::string& sParam, BOOL bIsUtf8 = FALSE)
		{
			return GetUnicodeFromCharPtr(sParam.c_str(), (LONG)sParam.length(), bIsUtf8);			
		}
		static std::wstring GetUnicodeStringFromUTF8_4bytes( BYTE* pBuffer, LONG lCount )
		{
			LONG lLenght = 0;

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
				pData = new BYTE[6 * lCount + 3];
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
		}

		static void GetUtf8StringFromUnicode_2bytes(const wchar_t* pUnicodes, LONG lCount, BYTE*& pData, LONG& lOutputCount, bool bIsBOM = false)
		{
			if (NULL == pData)
			{
				pData = new BYTE[6 * lCount + 3];
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
	};

	class CFileBinary
	{
	private:
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

		bool OpenFile(const std::wstring& sFileName)
		{
#if defined(WIN32) || defined(_WIN32_WCE)
			m_pFile = _wfopen(sFileName.c_str(), L"rb");
#else
			BYTE* pUtf8 = NULL;
			LONG lLen = 0;
			CUtf8Converter::GetUtf8StringFromUnicode(sFileName, sFileName.length(), pUtf8, lLen, true);
			m_pFile = fopen((char*)pUtf8, "rb");
			delete [] pUtf8;
#endif		
			if (NULL == m_pFile)
				return false;
			
			fseek(m_pFile, 0, SEEK_END);
			m_lFileSize = ftell(m_pFile);
			fseek(m_pFile, 0, SEEK_SET);

			m_lFilePosition = 0;
			return true;
		}

		bool CreateFile(const std::wstring& sFileName)
		{
			#if defined(WIN32) || defined(_WIN32_WCE)
			m_pFile = _wfopen(sFileName.c_str(), L"wb");
#else
			BYTE* pUtf8 = NULL;
			LONG lLen = 0;
			CUtf8Converter::GetUtf8StringFromUnicode(sFileName, sFileName.length(), pUtf8, lLen, true);
			m_pFile = fopen((char*)pUtf8, "wb");
			delete [] pUtf8;
#endif		
			if (NULL == m_pFile)
				return false;
			
			m_lFilePosition = 0;
			return true;
		}

		bool ReadFile(BYTE* pData, DWORD nBytesToRead, DWORD& dwSizeRead)
		{
			if (!m_pFile)
				return false;

			dwSizeRead = (DWORD)fread((void*)pData, 1, nBytesToRead, m_pFile);
			return true;
		}
	};
}

#endif //_BUILD_FILE_CROSSPLATFORM_H_