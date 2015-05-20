#ifndef _BUILD_STRING_CROSSPLATFORM_H_
#define _BUILD_STRING_CROSSPLATFORM_H_

#include "CPEncodings/CodePage.h"
#include <vector>
#include <sstream>

namespace NSString
{
#define NSSTRING_COMMON_CP(UnicodeMapCP, lCount, pData) \
	for (long i = 0; i < lCount; ++i)\
	{\
		unsigned char unChar = (unsigned char)pData[i];\
		if (unChar < MSCP_FIRST_CHAR || unChar > MSCP_LAST_CHAR)\
			pUnicode[i] = (wchar_t)unChar;\
		else\
			pUnicode[i] = (wchar_t)(UnicodeMapCP[unChar - MSCP_FIRST_CHAR]);\
	}
// end define

#define NSSTRING_WITHLEADBYTE_CP(LEAD_CHAR, UnicodeMapCP, UnicodeMapWithLeadByte, lCount, pData) \
	{\
		int nLeadByte = -1;\
		for (long i = 0; i < lCount; i++)\
		{\
			unsigned char  unCode = (unsigned char)pData[i];\
			unsigned short ushUnicode = UnicodeMapCP[unCode];\
			if (-1 == nLeadByte)\
			{\
				if (LEAD_CHAR != ushUnicode)\
				{\
					pUnicode[i] = ushUnicode;\
					nLeadByte = -1;\
				}\
				else\
				{\
					nLeadByte = ushUnicode;\
				}\
			}\
			else\
			{\
				unsigned short ushCode = (nLeadByte << 16) | ushUnicode;\
				pUnicode[i] = 0x0020;\
				TCodePagePair *pPair = (TCodePagePair*)UnicodeMapWithLeadByte;\
				while(0xFFFF != pPair->ushCode)\
				{\
					if (ushCode == pPair->ushCode)\
					{\
						pUnicode[i] = pPair->ushUnicode;\
						break;\
					}\
					pPair++;\
				}\
			}\
		}\
	}
// end define

	class CConverter
	{
	public:
		typedef enum
		{
			SINGLE_BYTE_ENCODING_DEFAULT = 0x01, // DEFAULT_CHARSET           1 (x01)
			SINGLE_BYTE_ENCODING_SYMBOL  = 0x02, // SYMBOL_CHARSET            2 (x02)
			SINGLE_BYTE_ENCODING_CP1252  = 0x00, // ANSI_CHARSET              0 (x00)
			SINGLE_BYTE_ENCODING_CP1251  = 0xCC, // RUSSIAN_CHARSET         204 (xCC)
			SINGLE_BYTE_ENCODING_CP1250  = 0xEE, // EASTEUROPE_CHARSET      238 (xEE)
			SINGLE_BYTE_ENCODING_CP1253  = 0xA1, // GREEK_CHARSET           161 (xA1)
			SINGLE_BYTE_ENCODING_CP1254  = 0xA2, // TURKISH_CHARSET         162 (xA2)
			SINGLE_BYTE_ENCODING_CP1257  = 0xBA, // BALTIC_CHARSET          186 (xBA)
			SINGLE_BYTE_ENCODING_CP1255  = 0xB1, // HEBREW_CHARSET          177 (xB1)
			SINGLE_BYTE_ENCODING_CP1256  = 0xB2, // ARABIC _CHARSET         178 (xB2)
			SINGLE_BYTE_ENCODING_CP932   = 0x80, // SHIFTJIS_CHARSET        128 (x80)
			SINGLE_BYTE_ENCODING_CP949   = 0x81, // HANGEUL_CHARSET         129 (x81)
			SINGLE_BYTE_ENCODING_CP936   = 0x86, // GB2313_CHARSET          134 (x86)
			SINGLE_BYTE_ENCODING_CP950   = 0x88, // CHINESEBIG5_CHARSET     136 (x88)
			SINGLE_BYTE_ENCODING_CP874   = 0xDE, // THAI_CHARSET            222 (xDE)
			SINGLE_BYTE_ENCODING_CP1361  = 0x82, // JOHAB_CHARSET	        130 (x82)
			SINGLE_BYTE_ENCODING_CP1258  = 0xA3, // VIETNAMESE_CHARSET      163 (xA3)
			SINGLE_BYTE_ENCODING_CP866   = 0xFF  // OEM_CHARSET             255 (xFF) // Проверить, что OEM соответствует CP866
		} ESingleByteEncoding;

		static std::wstring GetUnicodeFromSingleByteString(const unsigned char* pData, long lCount, ESingleByteEncoding eType = SINGLE_BYTE_ENCODING_DEFAULT)
		{
			wchar_t* pUnicode = new wchar_t[lCount + 1];
			if (!pUnicode)
				return std::wstring(L"");

			switch (eType)
			{
				default:
				case SINGLE_BYTE_ENCODING_DEFAULT:
				{
					for (long i = 0; i < lCount; ++i)
						pUnicode[i] = (wchar_t)(unsigned char)pData[i];

					break;
				}
				case SINGLE_BYTE_ENCODING_SYMBOL:
				{
					// Добавляем 0xF000 к кодам всех символов
					for (long i = 0; i < lCount; ++i)
					{
						pUnicode[i] = (wchar_t)(0xF000 | (unsigned char)pData[i]);
					}

					break;
				}
				case SINGLE_BYTE_ENCODING_CP866: NSSTRING_COMMON_CP(c_anUnicodeMapCP866, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP874: NSSTRING_COMMON_CP(c_anUnicodeMapCP874, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP1250: NSSTRING_COMMON_CP(c_anUnicodeMapCP1250, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP1251: NSSTRING_COMMON_CP(c_anUnicodeMapCP1251, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP1252: NSSTRING_COMMON_CP(c_anUnicodeMapCP1252, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP1253: NSSTRING_COMMON_CP(c_anUnicodeMapCP1253, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP1254: NSSTRING_COMMON_CP(c_anUnicodeMapCP1254, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP1255: NSSTRING_COMMON_CP(c_anUnicodeMapCP1255, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP1256: NSSTRING_COMMON_CP(c_anUnicodeMapCP1256, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP1257: NSSTRING_COMMON_CP(c_anUnicodeMapCP1257, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP1258: NSSTRING_COMMON_CP(c_anUnicodeMapCP1258, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP932: NSSTRING_WITHLEADBYTE_CP(MSCP932_LEAD_CHAR, c_anUnicodeMapCP932, c_aoUnicodeMapCP932WithLeadByte, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP936: NSSTRING_WITHLEADBYTE_CP(MSCP936_LEAD_CHAR, c_anUnicodeMapCP936, c_aoUnicodeMapCP936WithLeadByte, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP949: NSSTRING_WITHLEADBYTE_CP(MSCP949_LEAD_CHAR, c_anUnicodeMapCP949, c_aoUnicodeMapCP949WithLeadByte, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP950: NSSTRING_WITHLEADBYTE_CP(MSCP950_LEAD_CHAR, c_anUnicodeMapCP950, c_aoUnicodeMapCP950WithLeadByte, lCount, pData); break;
				case SINGLE_BYTE_ENCODING_CP1361:NSSTRING_WITHLEADBYTE_CP(MSCP1361_LEAD_CHAR, c_anUnicodeMapCP1361, c_aoUnicodeMapCP1361WithLeadByte, lCount, pData); break;
			}

			pUnicode[lCount] = 0;
			std::wstring s(pUnicode, lCount);

			if (pUnicode)
				delete[] pUnicode;

			return s;
		}
		static std::wstring GetUnicodeFromUTF16(const unsigned short* pData, long lCount)
		{
			if (0 == lCount)
				return L"";

			if (2 == sizeof(wchar_t))
				return std::wstring((wchar_t*)pData, lCount);

			wchar_t* pUnicode = new wchar_t[lCount + 1];
			if (!pUnicode)
				return L"";

			wchar_t* pCur = pUnicode;
			int nCurPos = 0;
			while (nCurPos < lCount)
			{
				int nLeading = pData[nCurPos]; nCurPos++;
				if (nLeading < 0xD800 || nLeading > 0xDFFF)
				{
					*pCur = (wchar_t)nLeading;
					pCur++;
				}
				else
				{
					if (nCurPos >= lCount)
						break;
					
					int nTrailing = pData[nCurPos]; nCurPos++;
					if (nTrailing >= 0xDC00 && nTrailing <= 0xDFFF)
					{
						*pCur = (wchar_t)(((nLeading & 0x03FF) << 10) | (nTrailing & 0x03FF));
						pCur++;
					}
				}
			}

			if (0 == pCur - pUnicode)
				return L"";

			std::wstring sRet(pUnicode, pCur - pUnicode);

			if (pUnicode)
				delete[] pUnicode;

			return sRet;
		}
	};

	static std::vector<std::wstring>& Split(const std::wstring& wsString, wchar_t nDelim, std::vector<std::wstring> &arrElements)
	{
		std::wstringstream wStringStream(wsString);
		std::wstring wsItem;
		while (std::getline(wStringStream, wsItem, nDelim))
		{
			arrElements.push_back(wsItem);
		}
		return arrElements;
	}
	static std::vector<std::wstring>  Split(const std::wstring& wsString, wchar_t nDelim)
	{
		std::vector<std::wstring> wsElements;
		Split(wsString, nDelim, wsElements);
		return wsElements;
	}
	static std::vector<std::wstring>& Split(const std::wstring& wsString, const std::wstring wsDelim, std::vector<std::wstring> &arrElements)
	{
		int nDelimLen = wsDelim.length();
		int nPrevPos = 0;

		if (nDelimLen > 0)
		{
			int nPos = wsString.find(wsDelim);
			while (std::wstring::npos != nPos)
			{
				if (nPrevPos != nPos)
					arrElements.push_back(wsString.substr(nPrevPos, nPos - nPrevPos));

				nPrevPos = nPos + nDelimLen;
				nPos = wsString.find(wsDelim, nPrevPos);
			}
		}

		if (nPrevPos < wsString.length())
			arrElements.push_back(wsString.substr(nPrevPos));

		return arrElements;
	}
	static std::vector<std::wstring>  Split(const std::wstring& wsString, const std::wstring& wsDelim)
	{
		std::vector<std::wstring> arrElements;

		int nDelimLen = wsDelim.length();
		if (0 == nDelimLen)
			arrElements.push_back(wsString);
		else if (1 == nDelimLen)
			Split(wsString, wchar_t(wsDelim[0]), arrElements);
		else
			Split(wsString, wsDelim, arrElements);

		return arrElements;
	}
};

#endif // _BUILD_STRING_CROSSPLATFORM_H_
