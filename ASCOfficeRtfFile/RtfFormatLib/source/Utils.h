#pragma once

#include "../../../Common/FileWriter.h"

#include "UniversalConverterUtils.h"

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

namespace Strings
{	
	static int ToDigit(TCHAR c)
	{
		if (c >= '0' && c <= '9')
			return (int)(c - '0');
		if (c >= 'a' && c <= 'f')
			return 10 + (int)(c - 'a');
		if (c >= 'A' && c <= 'F')
			return 10 + (int)(c - 'A');

		return 0;
	}
    static int ToColor(const CString& strValue)
    {
        // variables
        int blue = 0;
        int green = 0;
        int red = 0;

        CString color = strValue; color = color.Trim();
				
        if (color.Find(_T("0x"))!=-1)
            color.Delete(0,2);
        if (color.Find(_T("#"))!=-1)
            color.Delete(0,1);

        while (color.GetLength() < 6)
            color = _T("0") + color;

        red = 16*ToDigit(color[0]) + ToDigit(color[1]);
        green = 16*ToDigit(color[2]) + ToDigit(color[3]);
        blue = 16*ToDigit(color[4]) + ToDigit(color[5]);

        return RGB(red, green, blue);
    }
	static void ToColor(const CString& strValue, int& nR, int& nG, int& nB, int& nA)
	{
		CString color = strValue; color = color.Trim();
				
		if (color.Find(_T("0x"))!=-1)
			color.Delete(0,2);
		if (color.Find(_T("#"))!=-1)
			color.Delete(0,1);

		while (color.GetLength() < 8)
			color = _T("0") + color;

		nA = 16*ToDigit(color[0]) + ToDigit(color[1]);
		nR = 16*ToDigit(color[2]) + ToDigit(color[3]);
		nG = 16*ToDigit(color[4]) + ToDigit(color[5]);
		nB = 16*ToDigit(color[6]) + ToDigit(color[7]);
	}
	static bool ToBoolean(const CString& strValue)
	{
		CString s = strValue;
		
		s.MakeLower();

		return (s == _T("true"));
	}
	static int ToInteger(const CString& strValue)
	{
		return _ttoi(strValue);
	}
	static double ToDouble(const CString& strValue)
	{
		double d = 0;

		_stscanf(strValue, _T(" %lf"), &d);

		return d;
	}
	
	static CString FromInteger(int Value, int Base = 10)
	{
		CString str;
		
		str.Format(_T("%d"), Value);

		return str;
	}
	static CString FromDouble(double Value)
	{
		CString str;
		
		str.Format(_T("%lf"), Value);

		return str;
	}
	static CString FromBoolean(bool Value)
	{
		if (Value)
			return _T("true");

		return _T("false");
	}
	
}

class Convert
{
public:	
	static  CString ToString(int i)
	{
		CString result;
		result.Format( _T("%i"), i);
		return result;
	}
	static  CString ToStringHex( int i, int nLen )
	{
		CString result;
		result.Format( _T("%x"), i);
		for( int i = result.GetLength(); i < nLen; i++ )
			result.Insert( 0 , '0' );
		result.MakeUpper();
		return result;
	}
	 static  int ToInt32(CString str, int base = 10)
	 {
		 int nResult;
		 if(16 == base)
			 _stscanf(str, _T("%x"), &nResult);
		 else if(8 == base)
			 _stscanf(str, _T("%o"), &nResult);
		 else 
			 _stscanf(str, _T("%d"), &nResult);
		 return nResult;
	 }
};
static const int aCodePages[][2] = {
    //charset	codepage
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
    { L"Cy-az-AZ" , 	0x082C 	},
    { L"Lt-az-AZ" , 	0x042C 	},
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
    { L"Cy-sr-SP" , 	0x0C1A 	},
    { L"Lt-sr-SP" , 	0x081A 	},
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
    { L"Cy-uz-UZ" , 	0x0843 	},
    { L"Lt-uz-UZ" , 	0x0443 	},
    { L"vi-VN" , 	0x042A 	}
};

class RtfUtility
{
public: 

    class RtfInternalEncoder
    {
    public:
        static CString Encode( CString sFilename )
        {
            return _T("{\\*filename ") + sFilename + _T("\\*end}");
        }
        static void Decode( CString& sText, NFileWriter::CBufferedFileWriter& oFileWriter ) //сразу записывает в файл
        {
#if defined(_WIN32) || defined(_WIN64)
            CStringA sAnsiText; sAnsiText = sText;
            int nLenth = sAnsiText.GetLength();
            BYTE* BufferString = (BYTE*)sAnsiText.GetBuffer() ;
#else
            std::string sAnsiText(sText.begin(),sText.end());
            int nLenth = sAnsiText.length();
            BYTE* BufferString = (BYTE*)sAnsiText.c_str() ;
#endif
            int nStart = 0;
            int nFindRes = -1;
            CString sFindString = _T("{\\*filename ");
            int nFindStringLen = sFindString.GetLength();
            CString sFindEnd = _T("\\*end}");
            int nFindEndLen = sFindEnd.GetLength();
            while( -1 != (nFindRes = sText.Find( sFindString, nStart )) )
            {
                oFileWriter.Write( BufferString + nStart, nFindRes - nStart );
                sText.ReleaseBuffer();

                int nRightBound = 0;
                nRightBound = sText.Find( sFindEnd, nStart + nFindStringLen );

                CString sTargetFilename = sText.Mid( nFindRes + nFindStringLen, nRightBound - nFindRes - nFindStringLen );

                DecodeFromFile( sTargetFilename, oFileWriter );

                nStart = nRightBound + nFindEndLen;
            }
            oFileWriter.Write( BufferString + nStart, nLenth - nStart );
            sText.ReleaseBuffer();
        }
    private:
        static void DecodeFromFile( CString& sFilename, NFileWriter::CBufferedFileWriter& oFileWriter )
         {
            CFile file;

            if (file.OpenFile(sFilename) != S_OK) return;

            DWORD dwBytesRead = 0;
            BYTE byteBuffer[ BUF_SIZE ];

            char aLookup[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

            file.ReadFile(byteBuffer ,BUF_SIZE);

            dwBytesRead = file.GetPosition();
            while( 0 != dwBytesRead )
            {
                for( int i = 0; i < (int)dwBytesRead; i++ )
                {
                    BYTE byteData = byteBuffer[ i ];
                    BYTE byteFirst = aLookup[ byteData / 0x10 ];
                    BYTE byteSecond = aLookup[ byteData % 0x10 ];
                    oFileWriter.Write( &byteFirst, 1 );
                    oFileWriter.Write( &byteSecond, 1 );
                }
                dwBytesRead = file.GetPosition();
                file.ReadFile(byteBuffer ,BUF_SIZE);
                dwBytesRead = file.GetPosition() - dwBytesRead;
            }
            file.CloseFile();
         }
    };
    static float String2Percent( CString sValue )
	{
		int nPosition;
		if( (nPosition = sValue.Find( _T("f") )) != -1 )
		{
			sValue = sValue.Left( nPosition );
			int dResult = Strings::ToInteger( sValue );
			return (float)(1.0 * dResult / 65536);
		}
		else if( (nPosition = sValue.Find( _T("%") )) != -1 )
		{
			sValue = sValue.Left( nPosition );
			return (float)Strings::ToDouble( sValue ) / 100;
		}
		else
			return 0;
	}
	static int String2Twips( CString sValue )
	{
		int nPosition;
		if( (nPosition = sValue.Find( _T("pt") )) != -1 )
		{
			sValue = sValue.Left( nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return pt2Twip( dResult );
		}
		else if( (nPosition = sValue.Find( _T("in") )) != -1 )
		{
			sValue = sValue.Left( nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return in2Twip(dResult);
		}
		else if( (nPosition = sValue.Find( _T("cm") )) != -1 )
		{
			sValue = sValue.Left( nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return cm2Twip(dResult);
		}
		else if( (nPosition = sValue.Find( _T("mm") )) != -1 )
		{
			sValue = sValue.Left( nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return mm2Twip(dResult);
		}
		else if( (nPosition = sValue.Find( _T("pc") )) != -1 )
		{
			sValue = sValue.Left( nPosition );
			float dResult = (float)Strings::ToDouble( sValue );
			return pc2Twip(dResult);
		}
		else if( (nPosition = sValue.Find( _T("pi") )) != -1 )
		{
			sValue = sValue.Left( nPosition );
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
    static void WriteDataToFileBinary(CString& sFilename, BYTE* pbData, int nLength)
	{
		if( NULL == pbData )
			return;

		CFile file;
        if (file.CreateFile(sFilename) != S_OK) return;

		file.WriteFile(pbData ,nLength);	
		file.CloseFile();
	}
	static void WriteDataToFile(CString& sFilename, CString& sData)
	{
		CFile file;

        if (file.CreateFile(sFilename) != S_OK) return;

		TCHAR * buf  = sData.GetBuffer();
		int nLengthText = sData.GetLength();
		int nLengthData = nLengthText/2;
		BYTE * buf2 = new BYTE[ nLengthData];
		BYTE nByte=0;
		for( int i=0; i < nLengthData ; i++ )
		{
			nByte = ToByte( buf[2 * i] ) << 4;
			nByte |= ToByte( buf[2 * i + 1] );
			buf2[i] = nByte;
		}
		file.WriteFile(buf2 ,nLengthData);	
		sData.ReleaseBuffer();
		delete[] buf2;
		file.CloseFile();

	}
	static void WriteDataToBinary( CString sData, BYTE** ppData, long& nSize)
	{
		TCHAR * buf  = sData.GetBuffer();
		int nLengthText = sData.GetLength();
		nSize = nLengthText/2;
		BYTE * buf2 = new BYTE[ nSize];
		(*ppData) = buf2;
		BYTE nByte=0;
		for( int i=0; i < nSize ; i++ )
		{
			nByte = ToByte(buf[ 2*i])<<4;
			nByte |= ToByte(buf[ 2*i+1]);
			buf2[i] = nByte;
		}
		sData.ReleaseBuffer();
	}
	static CString DecodeHex( CString sText )
	{
		CString sHexText;
		for( int i = 0; i < sText.GetLength(); i++ )
		{
            BYTE byteChar = sText[i];
			sHexText.AppendFormat( _T("%x"), byteChar );
		}
		return sHexText;
	}
	static CString EncodeHex( CString sHexText )
	{
		CString sText;
		for( int i = 0; i < sHexText.GetLength() -1 ; i+=2 )
		{
			int byte1 = ToByte( sHexText[i] );
			int byte2 = ToByte(sHexText[i + 1] );
			int cChar = (byte1 << 4) + byte2;
			sText.AppendFormat( _T("%c"), cChar );
		}
		return sText;
	}
    static BYTE ToByte( TCHAR cChar )
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
	static CString Preserve( CString sText )
	{
		CString sResult = sText;
		//обрезавем лишние пробелы
		sResult.Trim();
		//удаляем дублирующие пробелы
		while( sResult.Replace( _T("  "), _T(" ") ) > 0 )
			;
		return sResult;
	}


    static int CharsetToCodepage( int nCharset )
    {
#if defined (_WIN32) || defined(_WIN64)
        CHARSETINFO Info;
        DWORD* dwAcp = (DWORD*)nCharset;
        if( TRUE == TranslateCharsetInfo(dwAcp, &Info, TCI_SRCCHARSET) )
            return Info.ciACP;
#endif

        int nCodePagesLength =  sizeof( aCodePages ) / ( sizeof( int ) );

        for( int i = 0; i < nCodePagesLength; i++ )
            if( aCodePages[i][0] == nCharset )
                return aCodePages[i][1];

        return 1252;//ANSI
    }
    static CString convert_string(std::string::const_iterator start, std::string::const_iterator end, int nCodepage = 0)
    {
        bool ansi = true;

        size_t insize = end- start;
        CString w_out;

        w_out.GetBuffer(insize);

		char *inptr = (char*)start.operator ->();
        char* outptr = (char*)w_out.GetBuffer();
		
		if (nCodepage > 0)
        {
#if defined (_WIN32) || defined (_WIN64)
			int insize = MultiByteToWideChar(nCodepage, 0, inptr, -1, NULL, NULL);
			if (MultiByteToWideChar(nCodepage, 0, inptr, -1, (LPWSTR)outptr, insize) > 0)
            {
                w_out.ReleaseBuffer();
                ansi = false;
            }
#else
            std::string sCodepage =  "CP" + std::to_string(nCodepage);

            iconv_t ic= iconv_open("WCHAR_T", sCodepage.c_str());
            if (ic != (iconv_t) -1)
            {
                size_t nconv = 0, avail = (insize) * sizeof(wchar_t);

                nconv = iconv (ic, &inptr, &insize, &outptr, &avail);
                if (nconv == 0)
                {
                    w_out.ReleaseBuffer();
                    ansi = false;
                }
                iconv_close(ic);
            }
#endif
        }
        if (ansi)
            w_out = std::wstring(start, end).c_str();

        return w_out;
    }
    static std::string convert_string(std::wstring::const_iterator start, std::wstring::const_iterator end, int nCodepage = 0)
    {
        std::string out;
        bool ansi = true;

        size_t insize = end - start;
        out.reserve(insize);

        char *inptr = (char*)start.operator ->();
        char* outptr = (char*)out.c_str();

		if (nCodepage > 0)
        {
#if defined (_WIN32) || defined (_WIN64)
            insize = WideCharToMultiByte(nCodepage, 0, (LPCWSTR)inptr, -1, NULL, 0, NULL, NULL);

			if (WideCharToMultiByte(nCodepage, 0, (LPCWSTR)inptr, -1, outptr, insize, NULL, NULL) > 0)
			{
				ansi = false;
			}
#else
            std::string sCodepage =  "CP" + std::to_string(nCodepage);

            iconv_t ic= iconv_open(sCodepage.c_str(), "WCHAR_T");
            if (ic != (iconv_t) -1)
            {
                size_t nconv = 0, avail = insize * sizeof(wchar_t);

				nconv = iconv (ic, &inptr, &insize, &outptr, &avail);
                if (nconv == 0) ansi = false;
                iconv_close(ic);
            }
#endif
        }

        if (ansi)
            out = std::string(start, end);

        return out;
    }
    static int CodepageToCharset( int nCodepage )
    {
#if defined (_WIN32) || defined(_WIN64)
        CHARSETINFO Info;
        DWORD* dwAcp = (DWORD*)nCodepage;
        if( TRUE == TranslateCharsetInfo(dwAcp, &Info, TCI_SRCCODEPAGE) )
            return Info.ciCharset;
#endif
        int nCodePagesLength =  sizeof( aCodePages ) / ( sizeof( int ) );

        for( int i = 0; i < nCodePagesLength; i++ )
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
