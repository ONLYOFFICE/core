/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "../Base/Base.h"
#include "../Base/SmartPtr.h"
#include "../Base/Types_32.h"

#include <algorithm>
#include <sstream>
#include <stdio.h>

#ifndef _USE_NULLABLE_PROPERTY_
using namespace NSCommon;
#endif

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace XmlUtils
{
    static std::wstring strInvalidValue = L"x(-Jdl%^8sFGs@gkp14jJU(90dyjhjnb*EcfFf%#2124sf98hc";

	// common
    AVSINLINE static int     GetDigit   (wchar_t c)
	{
		if (c >= '0' && c <= '9')
			return (int)(c - '0');
		if (c >= 'a' && c <= 'f')
			return 10 + (int)(c - 'a');
		if (c >= 'A' && c <= 'F')
			return 10 + (int)(c - 'A');

		return 0;
	}
	AVSINLINE static int     GetDigit   (char c)
	{
		if (c >= '0' && c <= '9')
			return (int)(c - '0');
		if (c >= 'a' && c <= 'f')
			return 10 + (int)(c - 'a');
		if (c >= 'A' && c <= 'F')
			return 10 + (int)(c - 'A');

		return 0;
	}
	AVSINLINE static bool     IsDigit   (wchar_t c)
	{
		if (c >= '0' && c <= '9')
			return true;
		return false;
	}
    AVSINLINE static _INT64 GetHex (const std::wstring& string)
	{
        _INT64 nResult = 0;
        size_t nLen = string.length();
		for (size_t nIndex = 0; nIndex < nLen; ++nIndex )
		{
			nResult += GetDigit( string[nIndex] ) << ( 4 * ( nLen - 1 - nIndex ) );
		}

		return nResult;
	}
    AVSINLINE static _INT64 GetHex (const std::string& string)
	{
        _INT64 nResult = 0;
        size_t nLen = string.length();
		for (size_t nIndex = 0; nIndex < nLen; ++nIndex )
		{
			nResult += GetDigit( string[nIndex] ) << ( 4 * ( nLen - 1 - nIndex ) );
		}

		return nResult;
	}
    AVSINLINE static int	GetColorBGR   (const std::wstring& string)
	{
		// variables
		int blue = 0;
		int green = 0;
		int red = 0;

        std::wstring color = string; //boost::algorithm::trim(color);
				
        if (color.find(L"0x") != -1)    color.erase(0, 2);
        if (color.find(L"#") != -1)     color.erase(0, 1);

        while (color.length() < 6)
			color = L"0" + color;

		red		= 16*GetDigit(color[0]) + GetDigit(color[1]);
		green	= 16*GetDigit(color[2]) + GetDigit(color[3]);
		blue	= 16*GetDigit(color[4]) + GetDigit(color[5]);

		return ((int)(((BYTE)(red)|((WORD)((BYTE)(green))<<8))|(((DWORD)(BYTE)(blue))<<16)));	//RGB(red, green, blue);
	}
    AVSINLINE static std::wstring GetLower(const std::wstring& string)
    {
        std::wstring sResult;

        for( size_t nIndex = 0; nIndex < string.length(); nIndex++)
            sResult += wchar_t( towlower(string[nIndex]) );

        return sResult;
    }
	AVSINLINE static void replace_all(std::wstring& subject, const std::wstring& search, const std::wstring& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::wstring::npos) 
		{
			 subject.replace(pos, search.length(), replace);
			 pos += replace.length();
		}
	}
	AVSINLINE static void replace_all(std::string& subject, const std::string& search, const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos) 
		{
			 subject.replace(pos, search.length(), replace);
			 pos += replace.length();
		}
	}
	AVSINLINE static std::wstring GetUpper(const std::wstring& string)
    {
        std::wstring sResult;

        for( size_t nIndex = 0; nIndex < string.length(); nIndex++)
            sResult += wchar_t( towupper(string[nIndex]) );

        return sResult;
    }

    AVSINLINE static bool    GetBoolean (const std::wstring& string)
	{
        std::wstring s = XmlUtils::GetLower(string);

		return (s == L"true");
	}
    AVSINLINE static bool    GetBoolean2(const std::wstring& string)
	{
        std::wstring sTemp = XmlUtils::GetLower(string);

		return ( L"true" == sTemp || L"1" == sTemp || L"t" == sTemp || L"on" == sTemp );
	}
    AVSINLINE static _INT64     GetInteger64 (const std::wstring& string)
	{
        if (string.empty()) return 0;

        try
        {
            return _wtoi64(string.c_str());
        }
        catch(...)
        {
             return 0;
		}
    }
	AVSINLINE static int     GetInteger (const std::wstring& string)
	{
        if (string.empty()) return 0;

        try
        {
            return _wtoi(string.c_str());
        }
        catch(...)
        {
        }

        try
        {
            return static_cast<int>(_wtoi64(string.c_str()));
        }
        catch(...)
        {
            return 0;
        }
    }
    AVSINLINE static unsigned int GetUInteger(const std::wstring& string)
    {
        if (string.empty()) return 0;
        try
        {
            return (unsigned int) _wtoi(string.c_str());
        }
        catch(...)
        {
        }

        try
        {
            return (unsigned int) _wtoi64(string.c_str());
        }
        catch(...)
        {
            return 0;
        }
    }
    AVSINLINE static double  GetDouble  (const std::wstring& string)
	{
        if (string.empty()) return 0;

        double d = 0;
#if defined (_WIN32) || defined (_WIN64)
		swscanf_s(string.c_str(), L"%lf", &d);
#elif defined(_IOS) || defined(__ANDROID__)
        swscanf(string.c_str(), L"%lf", &d);
#else
		_stscanf(string.c_str(), L"%lf", &d);
#endif
		return d;
	}
    AVSINLINE static float   GetFloat   (const std::wstring& string)
	{
        if (string.empty()) return 0;

        float f = 0;
#if defined (_WIN32) || defined (_WIN64)
		swscanf_s(string.c_str(), L"%f", &f);
#elif defined(_IOS) || defined(__ANDROID__)
        swscanf(string.c_str(), L"%f", &f);
#else
        _stscanf(string.c_str(), L"%f", &f);
#endif
		return f;
	}
    AVSINLINE static std::wstring BoolToString  (const bool  & value)
	{
		return ( value ? L"true" : L"false" );
	}
    AVSINLINE static std::wstring FloatToString (const float & value)
	{
		return boost::lexical_cast<std::wstring>( value);
	}

    AVSINLINE static std::wstring DoubleToString(const double& value)
	{
		return boost::lexical_cast<std::wstring>( value);
	}

    AVSINLINE static std::wstring IntToString( int value, const wchar_t* format )
    {
        if ( format == NULL ) return L"";
        std::wstringstream sstream;
        sstream << boost::wformat(format) % value;
        return sstream.str();
    }
    AVSINLINE static std::string IntToString( int value, const char* format )
    {
        if ( format == NULL ) return "";
        std::stringstream sstream;
        sstream << boost::format(format) % value;
        return sstream.str();
    }
	static int Rand()
	{
		//rand returns integral value range between 0 and RAND_MAX.(RAND_MAX at least 32767.)
		static unsigned calls = 0;   /* ensure different random header each time */
		if (++calls == 1)
		{
			srand((unsigned int) time(NULL));
		}
		return std::rand();
	}
	static int GenerateInt()
	{
		//todo c++11 <random>
		return ((Rand() & 0x7FFF) | ((Rand() & 0x7FFF) << 15) | ((Rand() & 0x3) << 30));
	}
	static std::wstring GenerateGuid()
	{
		std::wstring result;
//#if defined (_WIN32) || defined(_WIN64)
//		GUID guid;
//		CoCreateGuid(&guid);
//
//		OLECHAR* guidString;
//		StringFromCLSID(guid, &guidString);
//		
//		result = std::wstring(guidString);
//
//		CoTaskMemFree(guidString);
//#else
        std::wstringstream sstream;
		sstream << boost::wformat(L"%04X%04X-%04X-%04X-%04X-%04X%04X%04X") % (Rand() & 0xff) % (Rand() & 0xff) % (Rand() & 0xff) % ((Rand() & 0x0fff) | 0x4000) % ((Rand() % 0x3fff) + 0x8000) %  (Rand() & 0xff) % (Rand() & 0xff) % (Rand() & 0xff);
        result = sstream.str();
//#endif	
		return result;
	}
    AVSINLINE static std::wstring DoubleToString( double value, wchar_t* format )
    {
        if ( format == NULL ) return L"";

        std::wstringstream sstream;
        sstream << boost::wformat(format) % value;
        return sstream.str();
    }
	AVSINLINE static bool IsUnicodeSymbol( WCHAR symbol )
	{
		bool result = false;

		if ( ( 0x0009 == symbol ) || ( 0x000A == symbol ) || ( 0x000D == symbol ) ||
			( ( 0x0020 <= symbol ) && ( 0xD7FF >= symbol ) ) || ( ( 0xE000 <= symbol ) && ( symbol <= 0xFFFD ) ) ||
			( ( 0x10000 <= symbol ) && symbol ) )
		{
			result = true;
		}

		return result;		  
	}
    AVSINLINE static std::string EncodeXmlString(const std::string& data, bool bDeleteNoUnicode = true)
	{
        std::string buffer;
        buffer.reserve(data.size());

        if(bDeleteNoUnicode)
        {
            for(size_t pos = 0; pos < data.size(); ++pos)
            {
                switch(data[pos])
                {
                    case '&':  buffer.append("&amp;");      break;
                    case '\"': buffer.append("&quot;");     break;
                    case '\'': buffer.append("&apos;");     break;
                    case '<':  buffer.append("&lt;");       break;
                    case '>':  buffer.append("&gt;");       break;
                    default:
                    {
                        if ( false == IsUnicodeSymbol( data[pos] ) )
                        {
                            wchar_t symbol1 = data[pos];
                            if(0xD800 <= symbol1 && symbol1 <= 0xDFFF && pos + 1 < data.size())
                            {
                                pos++;
                                wchar_t symbol2 = data[pos];
                                if (symbol1 < 0xDC00 && symbol2 >= 0xDC00 && symbol2 <= 0xDFFF)
                                {
                                    buffer.append(&data[pos-1], 2);
                                }
                                else
                                {
                                    buffer.append(" ");
                                }
                            }
                            else
                            {
                                buffer.append(" ");
                            }
                        }
                        else
                            buffer.append(&data[pos], 1);
                    }break;
                }
            }
        }
        else
        {
            for(size_t pos = 0; pos < data.size(); ++pos)
            {
                switch(data[pos])
                {
                    case '&':  buffer.append("&amp;");      break;
                    case '\"': buffer.append("&quot;");     break;
                    case '\'': buffer.append("&apos;");     break;
                    case '<':  buffer.append("&lt;");       break;
                    case '>':  buffer.append("&gt;");       break;
                    case '\0':
                        return buffer;
                    default:   buffer.append(&data[pos], 1);	break;
                }
            }
        }

        return buffer;
	}
    AVSINLINE static std::wstring EncodeXmlString(const std::wstring& data, bool bDeleteNoUnicode = true)
	{
        std::wstring buffer;
        buffer.reserve(data.size());

        if(bDeleteNoUnicode)
        {
            for(size_t pos = 0; pos < data.size(); ++pos)
            {
                switch(data[pos])
                {
                    case '&':  buffer.append(L"&amp;");      break;
                    case '\"': buffer.append(L"&quot;");     break;
                    case '\'': buffer.append(L"&apos;");     break;
                    case '<':  buffer.append(L"&lt;");       break;
                    case '>':  buffer.append(L"&gt;");       break;
                    default:
                    {
                        if ( false == IsUnicodeSymbol( data[pos] ) )
                        {
                            wchar_t symbol1 = data[pos];
                            if(0xD800 <= symbol1 && symbol1 <= 0xDFFF && pos + 1 < data.size())
                            {
                                pos++;
                                wchar_t symbol2 = data[pos];
                                if (symbol1 < 0xDC00 && symbol2 >= 0xDC00 && symbol2 <= 0xDFFF)
                                {
                                    buffer.append(&data[pos-1], 2);
                                }
                                else
                                {
                                    buffer.append(L" ");
                                }
                            }
                            else
                            {
                                buffer.append(L" ");
                            }
                        }
                        else
                            buffer.append(&data[pos], 1);
                    }break;
                }
            }
        }
        else
        {
            for(size_t pos = 0; pos < data.size(); ++pos)
            {
                switch(data[pos])
                {
                    case '&':  buffer.append(L"&amp;");      break;
                    case '\"': buffer.append(L"&quot;");     break;
                    case '\'': buffer.append(L"&apos;");     break;
                    case '<':  buffer.append(L"&lt;");       break;
                    case '>':  buffer.append(L"&gt;");       break;
                    case '\0':
                        return buffer;
                    default:   buffer.append(&data[pos], 1);	break;
                }
            }
        }

        return buffer;
	}
	AVSINLINE static std::wstring EncodeXmlStringExtend(const std::wstring& data, bool bDeleteNoUnicode = true)
	{
		std::wstring buffer;
		buffer.reserve(data.size());

		if(bDeleteNoUnicode)
		{
			for(size_t pos = 0; pos < data.size(); ++pos)
			{
				switch(data[pos])
				{
					case '&':  buffer.append(L"&amp;");      break;
					case '\"': buffer.append(L"&quot;");     break;
					case '\'': buffer.append(L"&apos;");     break;
					case '<':  buffer.append(L"&lt;");       break;
					case '>':  buffer.append(L"&gt;");       break;
					case '\n': buffer.append(L"&#xA;");      break;
					case '\r': buffer.append(L"&#xD;");      break;
					case '\t': buffer.append(L"&#x9;");      break;
					default:
					{
						if ( false == IsUnicodeSymbol( data[pos] ) )
						{
							wchar_t symbol1 = data[pos];
							if(0xD800 <= symbol1 && symbol1 <= 0xDFFF && pos + 1 < data.size())
							{
								pos++;
								wchar_t symbol2 = data[pos];
								if (symbol1 < 0xDC00 && symbol2 >= 0xDC00 && symbol2 <= 0xDFFF)
								{
									buffer.append(&data[pos-1], 2);
								}
								else
								{
									buffer.append(L" ");
								}
							}
							else
							{
								buffer.append(L" ");
							}
						}
						else
							buffer.append(&data[pos], 1);
					}break;
				}
			}
		}
		else
		{
			for(size_t pos = 0; pos < data.size(); ++pos)
			{
				switch(data[pos])
				{
					case '&':  buffer.append(L"&amp;");      break;
					case '\"': buffer.append(L"&quot;");     break;
					case '\'': buffer.append(L"&apos;");     break;
					case '<':  buffer.append(L"&lt;");       break;
					case '>':  buffer.append(L"&gt;");       break;
					case '\n': buffer.append(L"&#xA;");      break;
					case '\r': buffer.append(L"&#xD;");      break;
					case '\t': buffer.append(L"&#x9;");      break;
					case '\0':
						return buffer;
					default:   buffer.append(&data[pos], 1);	break;
				}
			}
		}

		return buffer;
	}
//#ifndef _USE_LIBXML2_READER_
	class CStringWriter
	{
	private:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		CStringWriter()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;

            m_bInitTable = false;
		}
		~CStringWriter()
		{
			RELEASEMEM(m_pData);
		}

        AVSINLINE void AddSize(size_t nSize)
		{
			if (nSize < 1) return;

			if (NULL == m_pData)
			{
                m_lSize = (std::max)((int) nSize, 1000);
				m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

				m_lSizeCur = 0;
				m_pDataCur = m_pData;
				return;
			}

			if ((m_lSizeCur + nSize) > m_lSize)
			{
				while ((m_lSizeCur + nSize) > m_lSize)
				{
					m_lSize *= 2;
				}

				wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
				if (NULL != pRealloc)
				{
					// реаллок сработал
					m_pData		= pRealloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
				else
				{
					wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

					free(m_pData);
					m_pData		= pMalloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
			}
		}

	public:

        AVSINLINE void WriteString(const wchar_t* pString, size_t nLen)
		{
			if (nLen < 1) return;

			AddSize(nLen);

            memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));

            m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}

        AVSINLINE void WriteString(const std::wstring& sString)
		{
			if (sString.empty()) return;

            WriteString(sString.c_str(), sString.length());

		}

        AVSINLINE void AddCharSafe(const wchar_t& _c)
		{
			AddSize(1);
			*m_pDataCur++ = _c;
			++m_lSizeCur;
		}
        AVSINLINE void AddChar2Safe(const wchar_t _c1, const wchar_t& _c2)
		{
			AddSize(2);
			*m_pDataCur++ = _c1;
			*m_pDataCur++ = _c2;
			m_lSizeCur += 2;
		}

        inline void WriteEncodeXmlString(const std::wstring & pString)
		{
			if (pString.empty()) return;

            const wchar_t* pData = pString.c_str();

            bool isUtf16 = sizeof(wchar_t) == 2;
			bool skipNext = false;
			while (*pData != 0)
			{
				wchar_t code = *pData;
				BYTE _code;
                //todo replace std::wstring with std::wstring and choose one writer
				if (isUtf16)
				{
					if (skipNext)
					{
						skipNext = false;
						_code = 1;
					}
					else if (code >= 0xD800 && code <= 0xDFFF && *(pData + 1) != 0)
					{
						skipNext = true;
						_code = 1;
					}
					else
					{
						_code = CheckCode(code);
					}
				}
				else
				{
					_code = CheckCode(code);
				}

				switch (_code)
				{
				case 1:
					AddCharSafe(*pData);
					break;
				case 0:
					AddCharSafe((WCHAR)' ');
					break;
				case 2:
					AddSize(5);
					*m_pDataCur++ = (WCHAR)('&');
					*m_pDataCur++ = (WCHAR)('a');
					*m_pDataCur++ = (WCHAR)('m');
					*m_pDataCur++ = (WCHAR)('p');
					*m_pDataCur++ = (WCHAR)(';');
					m_lSizeCur += 5;
					break;
				case 3:
					AddSize(6);
					*m_pDataCur++ = (WCHAR)('&');
					*m_pDataCur++ = (WCHAR)('a');
					*m_pDataCur++ = (WCHAR)('p');
					*m_pDataCur++ = (WCHAR)('o');
					*m_pDataCur++ = (WCHAR)('s');
					*m_pDataCur++ = (WCHAR)(';');
					m_lSizeCur += 6;
					break;
				case 4:
					AddSize(4);
					*m_pDataCur++ = (WCHAR)('&');
					*m_pDataCur++ = (WCHAR)('l');
					*m_pDataCur++ = (WCHAR)('t');
					*m_pDataCur++ = (WCHAR)(';');
					m_lSizeCur += 4;
					break;
				case 5:
					AddSize(4);
					*m_pDataCur++ = (WCHAR)('&');
					*m_pDataCur++ = (WCHAR)('g');
					*m_pDataCur++ = (WCHAR)('t');
					*m_pDataCur++ = (WCHAR)(';');
					m_lSizeCur += 4;
					break;
				case 6:
					AddSize(6);
					*m_pDataCur++ = (WCHAR)('&');
					*m_pDataCur++ = (WCHAR)('q');
					*m_pDataCur++ = (WCHAR)('u');
					*m_pDataCur++ = (WCHAR)('o');
					*m_pDataCur++ = (WCHAR)('t');
					*m_pDataCur++ = (WCHAR)(';');
					m_lSizeCur += 6;
					break;
				default:
					break;						
				}
			
				++pData;
			}

		}

        AVSINLINE size_t GetCurSize()
		{
			return m_lSizeCur;
		}

        AVSINLINE void Write(CStringWriter& oWriter)
		{
			WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
		}

		inline void Clear()
		{
			RELEASEMEM(m_pData);

			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		inline void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

        std::wstring GetData()
		{
            return std::wstring(m_pData, m_lSizeCur);
		}

	protected:
		BYTE m_arTableUnicodes[65536];
        bool m_bInitTable;

		BYTE CheckCode(const WCHAR& c)
		{
			if (!m_bInitTable)
			{
				memset(m_arTableUnicodes, 0, 65536);
				m_arTableUnicodes[0x0009] = 1;
				m_arTableUnicodes[0x000A] = 1;
				m_arTableUnicodes[0x000D] = 1;

				memset(m_arTableUnicodes + 0x0020, 1, 0xD7FF - 0x0020 + 1);
				memset(m_arTableUnicodes + 0xE000, 1, 0xFFFD - 0xE000 + 1);

				m_arTableUnicodes['&'] = 2;
				m_arTableUnicodes['\''] = 3;
				m_arTableUnicodes['<'] = 4;
				m_arTableUnicodes['>'] = 5;
				m_arTableUnicodes['\"'] = 6;

                                m_bInitTable = true;
			}
#if !defined(_WIN32) && !defined (_WIN64)
            if (sizeof (wchar_t) == 4)
            {
                UTF16 c1=0;

                const UTF32 *pStr32 = (UTF32 *)&c;
                UTF16 *pStr16 = (UTF16 *)&c1;

                ConversionResult eUnicodeConversionResult =
                        ConvertUTF32toUTF16(&pStr32,&pStr32[1], &pStr16, &pStr16[1],strictConversion);

                if (conversionOK != eUnicodeConversionResult)
                {
                    return 1; //??
                }
                return m_arTableUnicodes[c1];
            }
            else 
#endif
				return m_arTableUnicodes[c];
		}
	};	
//#endif // ifndef _USE_LIBXML2_READER_
}
