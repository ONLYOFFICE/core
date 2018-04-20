﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "RtfDefine.h"

#include "../../../Common/FileWriter.h"
#include "../../../Common/DocxFormat/Source/XML/Utils.h"

#include "../../../UnicodeConverter/UnicodeConverter.h"

#include "UniversalConverterUtils.h"

#if !defined (_WIN32) && !defined (_WIN64)
    #include "iconv.h"
#endif

#include <boost/date_time.hpp>

#define BUF_SIZE 2048
#define ONE_INCH 2.54

#ifndef CP_ACP          //ansi code page
    #define CP_ACP 0
#endif

#ifndef CP_MACCP        //mac code page
    #define CP_MACCP 2
#endif

#ifndef CP_SYMBOL        //Symbol
    #define CP_SYMBOL 42
#endif

#define GETBIT(from, num)				((from & (1 << num)) != 0)
#define GETBITS(from, numL, numH)		((from & (((1 << (numH - numL + 1)) - 1) << numL)) >> numL)

#define SETBIT(to, num, setorclear)		{setorclear ? to |= (1 << num) : to &= ~(1 << num);}
#define SETBITS(to, numL, numH, val)	{to &= ~(((1 << (numH - numL + 1)) - 1) << numL); to |= ((val & ((1 << (numH - numL + 1)) - 1)) << numL);}

namespace Strings
{	
    static int ToDigit(wchar_t c)
	{
		if (c >= '0' && c <= '9')
			return (int)(c - '0');
		if (c >= 'a' && c <= 'f')
			return 10 + (int)(c - 'a');
		if (c >= 'A' && c <= 'F')
			return 10 + (int)(c - 'A');

		return 0;
	}
    static int ToColor(const std::wstring& strValue)
    {
        // variables
        int blue	= 0;
        int green	= 0;
        int red		= 0;

        std::wstring color = strValue; //color = color.Trim();
				
        if (color.find (L"0x") != -1)
            color.erase (0, 2);
        if (color.find (L"#") != -1)
            color.erase (0, 1);

        while (color.length() < 6)
            color = L"0" + color;

        red		= 16 * ToDigit(color[0]) + ToDigit(color[1]);
        green	= 16 * ToDigit(color[2]) + ToDigit(color[3]);
        blue	= 16 * ToDigit(color[4]) + ToDigit(color[5]);

        return ((int)(((BYTE)(red)|((WORD)((BYTE)(green))<<8))|(((DWORD)(BYTE)(blue))<<16))); //RGB(red, green, blue);
    }
    static void ToColor(const std::wstring& strValue, int& nR, int& nG, int& nB, int& nA)
	{
        std::wstring color = strValue;// color = color.Trim();
				
        if (color.find (L"0x")!=-1)
            color.erase (0, 2);
        if (color.find (L"#")!=-1)
            color.erase (0, 1);

        while (color.length() < 8)
			color = L"0" + color;

		nA = 16 * ToDigit(color[0]) + ToDigit(color[1]);
		nR = 16 * ToDigit(color[2]) + ToDigit(color[3]);
		nG = 16 * ToDigit(color[4]) + ToDigit(color[5]);
		nB = 16 * ToDigit(color[6]) + ToDigit(color[7]);
	}
    static int ToInteger(const std::wstring& strValue)
	{
        return _ttoi(strValue.c_str());
	}
    static double ToDouble(const std::wstring& strValue)
	{
		double d = 0;
#if defined (_WIN32) || defined(_WIN64)
        _stscanf_s(strValue.c_str(), L" %lf", &d);
#else
        _stscanf(strValue.c_str(), L" %lf", &d);
#endif
		return d;
	}

}

class Convert
{
public:	
    static std::wstring ToString(int i)
	{
		return std::to_wstring( i );
	}
    static  std::wstring ToStringHex( int i, int nLen )
	{
        std::wstring result = XmlUtils::IntToString(i, L"%X");

        for(int i = (int)result.length(); i < nLen; i++ )
            result.insert( result.begin() , '0' );
		return result;
	}
     static  int ToInt32(std::wstring str, int base = 10)
	 {
		 int nResult;
#if defined(_WIN32) || defined(_WIN64)
		 if(16 == base)
             _stscanf_s(str.c_str(), L"%X", &nResult);
		 else if(8 == base)
             _stscanf_s(str.c_str(), L"%o", &nResult);
		 else 
             _stscanf_s(str.c_str(), L"%d", &nResult);
#else
		 if(16 == base)
             _stscanf(str.c_str(), L"%X", &nResult);
		 else if(8 == base)
             _stscanf(str.c_str(), L"%o", &nResult);
		 else 
             _stscanf(str.c_str(), L"%d", &nResult);		 
#endif
		 return nResult;
	 }
};
static const int aCodePages[][2] = 
{//charset	codepage
    0,	1252, //ANSI
    1,	0,//Default
    2,	42,//Symbol
    77,	10000,//Mac Roman
    78,	10001,//Mac Shift Jis
    79,	10003,//Mac Hangul
    80,	10008,//Mac GB2312
    81,	10002,//Mac Big5
    83,	10005,//Mac Hebrew
    84,	10004,//Mac Arabic
    85,	10006,//Mac Greek
    86,	10081,//Mac Turkish
    87,	10021,//Mac Thai
    88,	10029,//Mac East Europe
    89,	10007,//Mac Russian
    128,	932,//Shift JIS
    129,	949,//Hangul
    130,	1361,//Johab
    134,	936,//GB2312
    136,	950,//Big5
    238,	1250,//Greek
    161,	1253,//Greek
    162,	1254,//Turkish
    163,	1258,//Vietnamese
    177,	1255,//Hebrew
    178,	1256, //Arabic
    186,	1257,//Baltic
    204,	1251,//Russian
    222,	874,//Thai
    238,	1250,//Eastern European
    254,	437,//PC 437
    255,	850//OEM
};

static const struct
{
    std::wstring    LCID_string;
    int             LCID_int;
} LCID_ms_convert[] =
{
    { L"af-ZA" , 	0x0436 	},
    { L"sq-AL" , 	0x041C 	},
    { L"ar-DZ" , 	0x1401 	},
    { L"ar-BH" , 	0x3C01 	},
    { L"ar-EG" , 	0x0C01 	},
    { L"ar-IQ" , 	0x0801 	},
    { L"ar-JO" , 	0x2C01 	},
    { L"ar-KW" , 	0x3401 	},
    { L"ar-LB" , 	0x3001 	},
    { L"ar-LY" , 	0x1001 	},
    { L"ar-MA" , 	0x1801 	},
    { L"ar-OM" , 	0x2001 	},
    { L"ar-QA" , 	0x4001 	},
    { L"ar-SA" , 	0x0401 	},
    { L"ar-SY" , 	0x2801 	},
    { L"ar-TN" , 	0x1C01 	},
    { L"ar-AE" , 	0x3801 	},
    { L"ar-YE" , 	0x2401 	},
    { L"hy-AM" , 	0x042B 	},
    { L"Cy-az-AZ" , 0x082C 	},
    { L"Lt-az-AZ" , 0x042C 	},
    { L"eu-ES" , 	0x042D 	},
    { L"be-BY" , 	0x0423 	},
    { L"bg-BG" , 	0x0402 	},
    { L"ca-ES" , 	0x0403 	},
    { L"zh-CN" , 	0x0804 	},
    { L"zh-HK" , 	0x0C04 	},
    { L"zh-MO" , 	0x1404 	},
    { L"zh-SG" , 	0x1004 	},
    { L"zh-TW" , 	0x0404 	},
    { L"zh-CHS" , 	0x0004 	},
    { L"zh-CHT" , 	0x7C04 	},
    { L"hr-HR" , 	0x041A 	},
    { L"cs-CZ" , 	0x0405 	},
    { L"da-DK" , 	0x0406 	},
    { L"div-MV" , 	0x0465 	},
    { L"nl-BE" , 	0x0813 	},
    { L"nl-NL" , 	0x0413 	},
    { L"en-AU" , 	0x0C09 	},
    { L"en-BZ" , 	0x2809 	},
    { L"en-CA" , 	0x1009 	},
    { L"en-CB" , 	0x2409 	},
    { L"en-IE" , 	0x1809 	},
    { L"en-JM" , 	0x2009 	},
    { L"en-NZ" , 	0x1409 	},
    { L"en-PH" , 	0x3409 	},
    { L"en-ZA" , 	0x1C09 	},
    { L"en-TT" , 	0x2C09 	},
    { L"en-GB" , 	0x0809 	},
    { L"en-US" , 	0x0409 	},
    { L"en-ZW" , 	0x3009 	},
    { L"et-EE" , 	0x0425 	},
    { L"fo-FO" , 	0x0438 	},
    { L"fa-IR" , 	0x0429 	},
    { L"fi-FI" , 	0x040B 	},
    { L"fr-BE" , 	0x080C 	},
    { L"fr-CA" , 	0x0C0C 	},
    { L"fr-FR" , 	0x040C 	},
    { L"fr-LU" , 	0x140C 	},
    { L"fr-MC" , 	0x180C 	},
    { L"fr-CH" , 	0x100C 	},
    { L"gl-ES" , 	0x0456 	},
    { L"ka-GE" , 	0x0437 	},
    { L"de-AT" , 	0x0C07 	},
    { L"de-DE" , 	0x0407 	},
    { L"de-LI" , 	0x1407 	},
    { L"de-LU" , 	0x1007 	},
    { L"de-CH" , 	0x0807 	},
    { L"el-GR" , 	0x0408 	},
    { L"gu-IN" , 	0x0447 	},
    { L"he-IL" , 	0x040D 	},
    { L"hi-IN" , 	0x0439 	},
    { L"hu-HU" , 	0x040E 	},
    { L"is-IS" , 	0x040F 	},
    { L"id-ID" , 	0x0421 	},
    { L"it-IT" , 	0x0410 	},
    { L"it-CH" , 	0x0810 	},
    { L"ja-JP" , 	0x0411 	},
    { L"kn-IN" , 	0x044B 	},
    { L"kk-KZ" , 	0x043F 	},
    { L"kok-IN" , 	0x0457 	},
    { L"ko-KR" , 	0x0412 	},
    { L"ky-KZ" , 	0x0440 	},
    { L"lv-LV" , 	0x0426 	},
    { L"lt-LT" , 	0x0427 	},
    { L"mk-MK" , 	0x042F 	},
    { L"ms-BN" , 	0x083E 	},
    { L"ms-MY" , 	0x043E 	},
    { L"mr-IN" , 	0x044E 	},
    { L"mn-MN" , 	0x0450 	},
    { L"nb-NO" , 	0x0414 	},
    { L"nn-NO" , 	0x0814 	},
    { L"pl-PL" , 	0x0415 	},
    { L"pt-BR" , 	0x0416 	},
    { L"pt-PT" , 	0x0816 	},
    { L"pa-IN" , 	0x0446 	},
    { L"ro-RO" , 	0x0418 	},
    { L"ru-RU" , 	0x0419 	},
    { L"sa-IN" , 	0x044F 	},
    { L"Cy-sr-SP" , 0x0C1A 	},
    { L"Lt-sr-SP" ,	0x081A 	},
    { L"sk-SK" , 	0x041B 	},
    { L"sl-SI" , 	0x0424 	},
    { L"es-AR" , 	0x2C0A 	},
    { L"es-BO" , 	0x400A 	},
    { L"es-CL" , 	0x340A 	},
    { L"es-CO" , 	0x240A 	},
    { L"es-CR" , 	0x140A 	},
    { L"es-DO" , 	0x1C0A 	},
    { L"es-EC" , 	0x300A 	},
    { L"es-SV" , 	0x440A 	},
    { L"es-GT" , 	0x100A 	},
    { L"es-HN" , 	0x480A 	},
    { L"es-MX" , 	0x080A 	},
    { L"es-NI" , 	0x4C0A 	},
    { L"es-PA" , 	0x180A 	},
    { L"es-PY" , 	0x3C0A 	},
    { L"es-PE" , 	0x280A 	},
    { L"es-PR" , 	0x500A 	},
    { L"es-ES" , 	0x0C0A 	},
    { L"es-UY" , 	0x380A 	},
    { L"es-VE" , 	0x200A 	},
    { L"sw-KE" , 	0x0441 	},
    { L"sv-FI" , 	0x081D 	},
    { L"sv-SE" , 	0x041D 	},
    { L"syr-SY" , 	0x045A 	},
    { L"ta-IN" , 	0x0449 	},
    { L"tt-RU" , 	0x0444 	},
    { L"te-IN" , 	0x044A 	},
    { L"th-TH" , 	0x041E 	},
    { L"tr-TR" , 	0x041F 	},
    { L"uk-UA" , 	0x0422 	},
    { L"ur-PK" , 	0x0420 	},
    { L"Cy-uz-UZ" , 0x0843 	},
    { L"Lt-uz-UZ" , 0x0443 	},
    { L"vi-VN" , 	0x042A 	}
};

class RtfUtility
{
public: 
    static std::map<int, std::string> create_mapEncodingsICU()
    {
		std::map<int, std::string> m;
		m[28596]	= "ISO-8859-6";
		m[720]		= "DOS-720";
		m[1256]		= "windows-1256";

		m[28594]	= "ISO-8859-4";
		m[28603]	= "ISO-8859-13";
		m[775]		= "IBM775";
		m[1257]		= "windows-1257";

		m[28604]	= "ISO-8859-14";

		m[28595]	= "ISO-8859-5";
		m[20866]	= "KOI8-R";
		m[21866]	= "KOI8-U";
		m[10007]	= "x-mac-cyrillic";
		m[855]		= "IBM855";
		m[866]		= "cp866";
		m[1251]		= "windows-1251";

		m[852]		= "IBM852";
		m[1250]		= "windows-1250";

		m[950]		= "Big5";
		m[936]		= "GB2312";

		m[28592]	= "ISO-8859-2";

		m[28597]	= "ISO-8859-7";
		m[737]		= "IBM737";
		m[869]		= "IBM869";
		m[1253]		= "windows-1253";

		m[28598]	= "ISO-8859-8";
		m[862]		= "DOS-862";
		m[1255]		= "windows-1255";

		m[932]		= "Shift_JIS";

		m[949]		= "KS_C_5601-1987";
		m[51949]	= "EUC-KR";

		m[861]		= "IBM861";
		m[865]		= "IBM865";

		m[874]		= "windows-874";

		m[28593]	= "ISO-8859-3";
		m[28599]	= "ISO-8859-9";
		m[857]		= "IBM857";
		m[1254]		= "windows-1254";

		m[28591]	= "ISO-8859-1";
		m[28605]	= "ISO-8859-15";
		m[850]		= "IBM850";
		m[858]		= "IBM858";
		m[860]		= "IBM860";
		m[863]		= "IBM863";
		m[437]		= "IBM437";
		m[1252]		= "windows-1252";

		m[1258]		= "windows-1258";

		m[65001]	= "UTF-8";
		m[65000]	= "UTF-7";

		m[1200]		= "UTF-16LE";
		m[1201]		= "UTF-16BE";

		m[12000]	= "UTF-32LE";
		m[12001]	= "UTF-32BE";
		return m;
	}
	static const std::map<int, std::string> mapEncodingsICU;
//--------------------------------------------------------------------------------------------
	static std::wstring convertDateTime (int dt)
	{
		if ( dt == PROP_DEF) return L"";

		short	Min		= GETBITS(dt, 0	, 5);
		short	Hour	= GETBITS(dt, 6	, 10);
		short	Day		= GETBITS(dt, 11, 15);
		short	Month	= GETBITS(dt, 16, 19);
		int		Year	= GETBITS(dt, 20, 28) + 1900;

		//to 1899-12-31T05:37:46.66569 - iso_extended_string
		std::wstring date_str = std::to_wstring(Year) 
								+ L"-" + (Month < 10 ? L"0": L"") + std::to_wstring(Month) 
								+ L"-" + (Day < 10 ? L"0": L"") + std::to_wstring(Day)
								+ L"T" + (Hour < 10 ? L"0": L"") + std::to_wstring(Hour)
								+ L":" + (Min < 10 ? L"0": L"") + std::to_wstring(Min)
								+ L":00Z";

		return date_str;
	}
	static int convertDateTime (std::wstring & dt_)
	{
		int result = 0;
		
		if ( dt_.empty() ) return PROP_DEF;

		std::string dt(dt_.begin(), dt_.end());

		try
		{
			boost::posix_time::ptime date_time_;

			boost::posix_time::time_input_facet *tif = new boost::posix_time::time_input_facet;
			tif->set_iso_extended_format();
			std::istringstream strm(dt);
			strm.imbue(std::locale(std::locale::classic(), tif));
			strm >> date_time_;

			short	Min		= date_time_.time_of_day().minutes();	
			short	Hour	= date_time_.time_of_day().hours();		
			short	Day		= date_time_.date().day();	
			short	Month	= date_time_.date().month().as_number();
			int		Year	= date_time_.date().year() - 1900;	

			SETBITS(result, 0 , 5,  Min);
			SETBITS(result, 6 , 10, Hour);
			SETBITS(result, 11, 15, Day);
			SETBITS(result, 16, 19, Month);
			SETBITS(result, 20, 28, Year);
		}
		catch(...)
		{
		}

		return result;
	}
//------------------------------------------------------------------------------------------------------
    class RtfInternalEncoder
    {
    public:
        static std::wstring Encode( std::wstring sFilename )
        {
            return L"{\\*filename " + sFilename + L"\\*end}";
        }
        static void Decode( std::wstring& sText, NFileWriter::CBufferedFileWriter& oFileWriter ) //сразу записывает в файл
        {
#if defined(_WIN32) || defined(_WIN64)
            std::string sAnsiText(sText.begin(), sText.end());
            size_t nLenth = sAnsiText.length();
            BYTE* BufferString = (BYTE*)sAnsiText.c_str() ;
#else
            std::string sAnsiText(sText.begin(),sText.end());
            int nLenth = sAnsiText.length();
            BYTE* BufferString = (BYTE*)sAnsiText.c_str() ;
#endif
            int nStart = 0;
            int nFindRes = -1;
            std::wstring sFindString = L"{\\*filename ";
            size_t nFindStringLen = sFindString.length();
            std::wstring sFindEnd = L"\\*end}";
            int nFindEndLen = (int)sFindEnd.length();
            while( -1 != (nFindRes = (int)sText.find( sFindString, nStart )) )
            {
                oFileWriter.Write( BufferString + nStart, nFindRes - nStart );

                int nRightBound = 0;
                nRightBound = (int)sText.find( sFindEnd, nStart + nFindStringLen );

                std::wstring sTargetFilename = sText.substr( nFindRes + nFindStringLen, nRightBound - nFindRes - nFindStringLen );

                DecodeFromFile( sTargetFilename, oFileWriter );

                nStart = nRightBound + nFindEndLen;
            }
            oFileWriter.Write( BufferString + nStart, nLenth - nStart );
        }
    private:
        static void DecodeFromFile( std::wstring& sFilename, NFileWriter::CBufferedFileWriter& oFileWriter )
         {
            CFile file;

            if (file.OpenFile(sFilename) != S_OK) return;

            DWORD dwBytesRead = 0;
            BYTE byteBuffer[ BUF_SIZE ];

            char aLookup[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

            file.ReadFile(byteBuffer ,BUF_SIZE);

            dwBytesRead = (DWORD)file.GetPosition();
            while( 0 != dwBytesRead )
            {
                for (size_t i = 0; i < (int)dwBytesRead; i++ )
                {
                    BYTE byteData = byteBuffer[ i ];
                    BYTE byteFirst = aLookup[ byteData / 0x10 ];
                    BYTE byteSecond = aLookup[ byteData % 0x10 ];
                    oFileWriter.Write( &byteFirst, 1 );
                    oFileWriter.Write( &byteSecond, 1 );
                }
                dwBytesRead = (DWORD)file.GetPosition();
                file.ReadFile(byteBuffer ,BUF_SIZE);
                dwBytesRead = (DWORD)file.GetPosition() - dwBytesRead;
            }
            file.CloseFile();
         }
    };
    static float String2Percent( std::wstring sValue )
	{
		int nPosition;
        if( (nPosition = (int)sValue.find( L"f" )) != -1 )
		{
            sValue = sValue.substr(0, nPosition );
			int dResult = Strings::ToInteger( sValue );
			return (float)(1.0 * dResult / 65536);
		}
        else if( (nPosition = (int)sValue.find( L"%" )) != -1 )
		{
            sValue = sValue.substr(0, nPosition );
			return (float)Strings::ToDouble( sValue ) / 100;
		}
		else
			return 0;
	}
    static int String2Twips( std::wstring sValue )
	{
		int nPosition;
        if( (nPosition = (int)sValue.find( L"pt" )) != -1 )
		{
            sValue = sValue.substr(0,  nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return pt2Twip( dResult );
		}
        else if( (nPosition = (int)sValue.find( L"in" )) != -1 )
		{
            sValue = sValue.substr(0,  nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return in2Twip(dResult);
		}
        else if( (nPosition = (int)sValue.find( L"cm" )) != -1 )
		{
            sValue = sValue.substr(0,  nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return cm2Twip(dResult);
		}
        else if( (nPosition = (int)sValue.find( L"mm" )) != -1 )
		{
            sValue = sValue.substr(0,  nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return mm2Twip(dResult);
		}
        else if( (nPosition = (int)sValue.find( L"pc" )) != -1 )
		{
            sValue = sValue.substr(0,  nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return pc2Twip(dResult);
		}
        else if( (nPosition = (int)sValue.find( L"pi" )) != -1 )
		{
            sValue = sValue.substr(0,  nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return pc2Twip(dResult);
		}
		else
			return Strings::ToInteger( sValue );
	}
	static int px2Twip(int px)
	{
		return 15 * px; //из наблюдений за word
	}
	static int pc2Twip(double pc)
	{
		return (int)( 12 * pc / 20 );
	}
	static int mm2Twip(double mm)
	{
		return cm2Twip( mm / 10 );
	}
	static int cm2Twip(double cm)
	{
		return in2Twip( cm / ONE_INCH );
	}
	static int in2Twip(double in)
	{
		return (int)( 20 * 72 * in  );
	}
	static int pt2Twip(double pt)
	{
		return (int)(pt * 20);
	}
	static float Twip2pt(int pt)
	{
		return (float)(pt / 20.0);
	}
	static int pt2HalfPt(float pt)
	{
		return (int)(pt * 2);
	}
	static int Twips2Emu(int pt)
	{
		return (int)(pt * 635);
	}
	static int Emu2Twips(int pt)
	{
		return (int)(pt / 635);
	}
	static float Emu2Pt(int emu)
	{
		return (float)(1.0 * emu / (635 * 20.0));
	}
    static void WriteDataToFileBinary(std::wstring& sFilename, BYTE* pbData, int nLength)
	{
		if( NULL == pbData )
			return;

		CFile file;
        if (file.CreateFile(sFilename) != S_OK) return;

		file.WriteFile(pbData ,nLength);	
		file.CloseFile();
	}
    static void WriteDataToFile(std::wstring& sFilename, std::wstring& sData)
	{
		CFile file;

        if (file.CreateFile(sFilename) != S_OK) return;

        wchar_t * buf  = (wchar_t *)sData.c_str();
        size_t nLengthText	= sData.length();
		size_t nLengthData	= nLengthText/2;
		
		BYTE * buf2 = new BYTE[ nLengthData];
		BYTE nByte=0;
		
		for (size_t i = 0; i < nLengthData ; i++ )
		{
			nByte = ToByte( buf[2 * i] ) << 4;
			nByte |= ToByte( buf[2 * i + 1] );
			buf2[i] = nByte;
		}
		file.WriteFile(buf2 ,(DWORD)nLengthData);	
		delete[] buf2;
		file.CloseFile();

	}
    static std::wstring DecodeHex( std::wstring sText )
	{
        std::wstring sHexText;
        for( size_t i = 0; i < sText.length(); i++ )
		{
            BYTE byteChar = (BYTE)sText[i];
            sHexText += XmlUtils::IntToString(byteChar, L"%x");
		}
		return sHexText;
	}
    static std::wstring EncodeHex( std::wstring sHexText )
	{
        std::wstring sText;
        for( size_t i = 0; i < sHexText.length() -1 ; i+=2 )
		{
			int byte1 = ToByte( sHexText[i] );
			int byte2 = ToByte(sHexText[i + 1] );
			int cChar = (byte1 << 4) + byte2;

            sText += XmlUtils::IntToString(cChar, L"%c" );
		}
		return sText;
	}
    static BYTE ToByte( wchar_t cChar )
	{
        return (BYTE)(cChar > 'F' ? cChar - 0x57 : cChar > '9' ? cChar - 0x37 : cChar - 0x30);
	}
	static bool IsAlpha( int nChar )
	{
		return ( nChar >= 'a' && nChar <= 'z' || nChar >= 'A' && nChar <= 'Z' );
	}
	static bool IsDigit( int nChar )
	{
		return nChar >= '0' && nChar <= '9';
	}
    static std::wstring Preserve( std::wstring sText )
	{
        std::wstring sResult = sText;
		//обрезавем лишние пробелы
		//sResult.Trim();

		//удаляем дублирующие пробелы
        XmlUtils::replace_all(sResult, L"  ", L" ");
//		while( sResult.Replace( L"  ", L" " ) > 0 )
//			;
		return sResult;
	}


    static int CharsetToCodepage( int nCharset )
    {
#if defined (_WIN32) || defined(_WIN64)
        CHARSETINFO Info;
        DWORD* dwAcp = (DWORD*)&nCharset;
        if( TRUE == TranslateCharsetInfo(dwAcp, &Info, TCI_SRCCHARSET) )
            return Info.ciACP;
#endif

        int nCodePagesLength =  sizeof( aCodePages ) / ( sizeof( int ) );

        for (int i = 0; i < nCodePagesLength; i++ )
            if( aCodePages[i][0] == nCharset )
                return aCodePages[i][1];

        return 1252;//ANSI
    }
    static std::wstring convert_string(std::string::const_iterator start, std::string::const_iterator end, int nCodepage = 0)
    {
        bool ansi = true;
        std::wstring sResult;

        size_t insize = end - start;
		char* inptr = (char*)start.operator ->();
	
		if (nCodepage > 0)
        {
#if defined (_WIN32) || defined (_WIN64)
			int outsize_with_0 = MultiByteToWideChar(nCodepage, 0, inptr, -1, NULL, NULL);
			sResult.resize(outsize_with_0); 
			if (MultiByteToWideChar(nCodepage, 0, inptr, -1, (LPWSTR)sResult.c_str(), outsize_with_0) > 0)
            {
				sResult.erase(outsize_with_0 - 1);
                ansi = false;
            }
#elif defined(__linux__)
            std::string sCodepage =  "CP" + std::to_string(nCodepage);

            iconv_t ic= iconv_open("WCHAR_T", sCodepage.c_str());
            if (ic != (iconv_t) -1)
            {
				sResult.resize(insize);
				char* outptr = (char*)sResult.c_str();

                size_t nconv = 0, avail = (insize) * sizeof(wchar_t), outsize = insize;
                nconv = iconv (ic, &inptr, &insize, &outptr, &avail);
                if (nconv == 0)
                {
                    if (avail > 0)
                    {
                        outsize = outsize - avail/sizeof(wchar_t);
                        sResult.erase(sResult.begin() + outsize);
                    }
                    ansi = false;
                }
                iconv_close(ic);
            }
#endif
        }
        if (ansi)
            sResult = std::wstring(start, end);

        return sResult;
	}
    static std::wstring convert_string_icu(std::string::const_iterator start, std::string::const_iterator end, int nCodepage = 0)
    {
        std::string sCodePage;
		std::map<int, std::string>::const_iterator pFind = mapEncodingsICU.find(nCodepage);
		if (pFind != mapEncodingsICU.end())
		{
			sCodePage = pFind->second;
		}

        if (sCodePage.empty() && nCodepage > 0)
            sCodePage = "CP" + std::to_string(nCodepage);

		if (sCodePage.empty())
		{
			return std::wstring(start, end);
		}
		else
		{
			unsigned int insize = (unsigned int)(end - start);
			const char* inptr = (const char*)start.operator ->();
			
			NSUnicodeConverter::CUnicodeConverter oConverter;
			return oConverter.toUnicodeExact(inptr, insize, sCodePage.c_str());
		}
    }
    static std::string convert_string(std::wstring::const_iterator start, std::wstring::const_iterator end, int nCodepage = 0)
    {
        std::string sCodePage;
        for (size_t i = 0; i < UNICODE_CONVERTER_ENCODINGS_COUNT; ++i)
        {
            if (nCodepage == NSUnicodeConverter::Encodings[i].WindowsCodePage)
            {
                sCodePage = NSUnicodeConverter::Encodings[i].Name;
                break;
            }
        }
        if (sCodePage.empty())
            sCodePage = "CP" + std::to_string(nCodepage);

        unsigned int insize = (unsigned int)(end - start);
        const wchar_t* inptr = (const wchar_t*)start.operator ->();

        NSUnicodeConverter::CUnicodeConverter oConverter;
        return oConverter.fromUnicode(inptr, insize, sCodePage.c_str());
    }

    static int CodepageToCharset( int nCodepage )
    {
#if defined (_WIN32) || defined(_WIN64)
        CHARSETINFO Info;
        DWORD* dwAcp = (DWORD*)&nCodepage;
        if( TRUE == TranslateCharsetInfo(dwAcp, &Info, TCI_SRCCODEPAGE) )
            return Info.ciCharset;
#endif
        int nCodePagesLength =  sizeof( aCodePages ) / ( sizeof( int ) );

        for (int i = 0; i < nCodePagesLength; i++ )
            if( aCodePages[i][1] == nCodepage )
                return aCodePages[i][0];

        return 0;//ANSI
    }
	static bool IsMacCharset( int nCharset )
	{
		return nCharset == 77 || nCharset == 78 || nCharset == 79 || nCharset == 80 ||
				nCharset == 81 || nCharset == 83 || nCharset == 84 || nCharset == 85 ||
				nCharset == 86 || nCharset == 87 || nCharset == 88 || nCharset == 89;
	}
	static bool IsMacCodepage( int nCodepage )
	{
		return nCodepage == 10000 || nCodepage == 10001 || nCodepage == 10003 || nCodepage == 10008 ||
				nCodepage == 10002 || nCodepage == 10005 || nCodepage == 10004 || nCodepage == 10006 ||
				nCodepage == 10081 || nCodepage == 10021 || nCodepage == 10029 || nCodepage == 10007;
	}

};
