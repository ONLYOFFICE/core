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

#include "RtfDefine.h"

#if !defined (_WIN32) && !defined (_WIN64)
    //#include "iconv.h" .. to UnicodeConverter
#else
    #include <windows.h>
#endif

#include "../../Common/FileWriter.h"
#include "../../OOXML/Base/Unit.h"

#include "../../UnicodeConverter/UnicodeConverter.h"

#include "UniversalConverterUtils.h"

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
	static _UINT32 ToColor(const std::wstring& strValue)
	{
		// variables
		_UINT32 blue	= 0;
		_UINT32 green	= 0;
		_UINT32 red		= 0;

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
		return XmlUtils::GetInteger(strValue);
	}
	static double ToDouble(const std::wstring& strValue)
	{
		return XmlUtils::GetDouble(strValue);
	}
}

class Convert
{
public:	
	static std::wstring ToString(int i);
	static  std::wstring ToStringHex(_INT32 i, size_t nLen );
	static  int ToInt32(std::wstring str, int base = 10);
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

class RtfUtility
{
public: 
//--------------------------------------------------------------------------------------------
	static std::wstring convertDateTime (int dt);
	static int convertDateTime (const std::wstring & dt_);
//------------------------------------------------------------------------------------------------------

	class RtfInternalEncoder
    {
    public:
		static std::wstring Encode( std::wstring sFilename );
		static void Decode( std::wstring& sText, NFileWriter::CBufferedFileWriter& oFileWriter ); //сразу записывает в файл

    private:
		static void DecodeFromFile( std::wstring& sFilename, NFileWriter::CBufferedFileWriter& oFileWriter );
    };

	static float String2Percent( std::wstring sValue );
	static int String2Twips( std::wstring sValue );
	static int px2Twip(int px);
	static int pc2Twip(double pc);
	static int mm2Twip(double mm);
	static int cm2Twip(double cm);
	static int in2Twip(double in);
	static int pt2Twip(double pt);
	static float Twip2pt(int pt);
	static int pt2HalfPt(float pt);
	static int Twips2Emu(int pt);
	static int Emu2Twips(int pt);
	static float Emu2Pt(int emu);
	static void WriteDataToFileBinary(std::wstring& sFilename, BYTE* pbData, size_t nLength);
	static void WriteDataToFile(std::wstring& sFilename, std::wstring& sData);
	static void DecodeHexString( std::string sHexText, BYTE *&pData );
	static BYTE ToByte( wchar_t cChar );
	static bool IsAlpha( int nChar );
	static bool IsDigit( int nChar );
	static std::wstring Preserve( std::wstring sText );
	static int CharsetToCodepage( int nCharset );
//    static std::wstring convert_string(std::string::const_iterator start, std::string::const_iterator end, int nCodepage = 0) .. to UnicodeConverter
//    {
//        bool ansi = true;
//        std::wstring sResult;
//
//        size_t insize = end - start;
//		char* inptr = (char*)start.operator ->();
//	
//		if (nCodepage > 0)
//        {
//#if defined (_WIN32) || defined (_WIN64)
//			int outsize_with_0 = MultiByteToWideChar(nCodepage, 0, inptr, -1, NULL, NULL);
//			sResult.resize(outsize_with_0); 
//			if (MultiByteToWideChar(nCodepage, 0, inptr, -1, (LPWSTR)sResult.c_str(), outsize_with_0) > 0)
//            {
//				sResult.erase(outsize_with_0 - 1);
//                ansi = false;
//            }
//#elif defined(__linux__)
//            std::string sCodepage =  "CP" + std::to_string(nCodepage);
//
//            iconv_t ic= iconv_open("WCHAR_T", sCodepage.c_str());
//            if (ic != (iconv_t) -1)
//            {
//				sResult.resize(insize);
//				char* outptr = (char*)sResult.c_str();
//
//                size_t nconv = 0, avail = (insize) * sizeof(wchar_t), outsize = insize;
//                nconv = iconv (ic, &inptr, &insize, &outptr, &avail);
//                if (nconv == 0)
//                {
//                    if (avail > 0)
//                    {
//                        outsize = outsize - avail/sizeof(wchar_t);
//                        sResult.erase(sResult.begin() + outsize);
//                    }
//                    ansi = false;
//                }
//                iconv_close(ic);
//            }
//#endif
//        }
//        if (ansi)
//            sResult = std::wstring(start, end);
//
//        return sResult;
//	}
	static std::wstring convert_string_icu(std::string::const_iterator start, std::string::const_iterator end, int nCodepage = 0);
	static std::string convert_string_icu(std::wstring::const_iterator start, std::wstring::const_iterator end, int nCodepage = 0);

	static int CodepageToCharset( int nCodepage );
	static bool IsMacCharset( int nCharset );
	static bool IsMacCodepage( int nCodepage );
};
