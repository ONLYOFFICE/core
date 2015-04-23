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
    int id;
    std::string name;
} aCodePagesNames[] =
{
    {	37	,	"IBM037"	},
    {	437	,	"IBM437"	},
    {	500	,	"IBM500"	},
    {	708	,	"ASMO-708"	},
    {	720	,	"DOS-720"	},
    {	737	,	"ibm737"	},
    {	775	,	"ibm775"	},
    {	850	,	"ibm850"	},
    {	852	,	"ibm852"	},
    {	855	,	"IBM855"	},
    {	857	,	"ibm857"	},
    {	858	,	"IBM00858"	},
    {	860	,	"IBM860"	},
    {	861	,	"ibm861"	},
    {	862	,	"DOS-862"	},
    {	863	,	"IBM863"	},
    {	864	,	"IBM864"	},
    {	865	,	"IBM865"	},
    {	866	,	"cp866"	},
    {	869	,	"ibm869"	},
    {	870	,	"IBM870"	},
    {	874	,	"windows-874"	},
    {	875	,	"cp875"	},
    {	932	,	"shift_jis"	},
    {	936	,	"gb2312"	},
    {	949	,	"KS_C_5601-1987"	},
    {	950	,	"big5"	},
    {	1026	,	"IBM1026"	},
    {	1047	,	"IBM01047"	},
    {	1140	,	"IBM01140"	},
    {	1141	,	"IBM01141"	},
    {	1142	,	"IBM01142"	},
    {	1143	,	"IBM01143"	},
    {	1144	,	"IBM01144"	},
    {	1145	,	"IBM01145"	},
    {	1146	,	"IBM01146"	},
    {	1147	,	"IBM01147"	},
    {	1148	,	"IBM01148"	},
    {	1149	,	"IBM01149"	},
    {	1200	,	"utf-16"	},
    {	1201	,	"unicodeFFFE"	},
    {	1250	,	"windows-1250"	},
    {	1251	,	"windows-1251"	},
    {	1252	,	"windows-1252"	},
    {	1253	,	"windows-1253"	},
    {	1254	,	"windows-1254"	},
    {	1255	,	"windows-1255"	},
    {	1256	,	"windows-1256"	},
    {	1257	,	"windows-1257"	},
    {	1258	,	"windows-1258"	},
    {	1361	,	"Johab"	},
    {	10000	,	"macintosh"	},
    {	10001	,	"x-mac-japanese"	},
    {	10002	,	"x-mac-chinesetrad"	},
    {	10003	,	"x-mac-korean"	},
    {	10004	,	"x-mac-arabic"	},
    {	10005	,	"x-mac-hebrew"	},
    {	10006	,	"x-mac-greek"	},
    {	10007	,	"x-mac-cyrillic"	},
    {	10008	,	"x-mac-chinesesimp"	},
    {	10010	,	"x-mac-romanian"	},
    {	10017	,	"x-mac-ukrainian"	},
    {	10021	,	"x-mac-thai"	},
    {	10029	,	"x-mac-ce"	},
    {	10079	,	"x-mac-icelandic"	},
    {	10081	,	"x-mac-turkish"	},
    {	10082	,	"x-mac-croatian"	},
    {	12000	,	"utf-32"	},
    {	12001	,	"utf-32BE"	},
    {	20000	,	"x-Chinese_CNS"	},
    {	20001	,	"x-cp20001"	},
    {	20002	,	"x_Chinese-Eten"	},
    {	20003	,	"x-cp20003"	},
    {	20004	,	"x-cp20004"	},
    {	20005	,	"x-cp20005"	},
    {	20105	,	"x-IA5"	},
    {	20106	,	"x-IA5-German"	},
    {	20107	,	"x-IA5-Swedish"	},
    {	20108	,	"x-IA5-Norwegian"	},
    {	20127	,	"us-ascii"	},
    {	20261	,	"x-cp20261"	},
    {	20269	,	"x-cp20269"	},
    {	20273	,	"IBM273"	},
    {	20277	,	"IBM277"	},
    {	20278	,	"IBM278"	},
    {	20280	,	"IBM280"	},
    {	20284	,	"IBM284"	},
    {	20285	,	"IBM285"	},
    {	20290	,	"IBM290"	},
    {	20297	,	"IBM297"	},
    {	20420	,	"IBM420"	},
    {	20423	,	"IBM423"	},
    {	20424	,	"IBM424"	},
    {	20833	,	"x-EBCDIC-KoreanExtended"	},
    {	20838	,	"IBM-Thai"	},
    {	20866	,	"koi8-r"	},
    {	20871	,	"IBM871"	},
    {	20880	,	"IBM880"	},
    {	20905	,	"IBM905"	},
    {	20924	,	"IBM00924"	},
    {	20932	,	"EUC-JP"	},
    {	20936	,	"x-cp20936"	},
    {	20949	,	"x-cp20949"	},
    {	21025	,	"cp1025"	},
    {	21866	,	"koi8-u"	},
    {	28591	,	"iso-8859-1"	},
    {	28592	,	"iso-8859-2"	},
    {	28593	,	"iso-8859-3"	},
    {	28594	,	"iso-8859-4"	},
    {	28595	,	"iso-8859-5"	},
    {	28596	,	"iso-8859-6"	},
    {	28597	,	"iso-8859-7"	},
    {	28598	,	"iso-8859-8"	},
    {	28599	,	"iso-8859-9"	},
    {	28603	,	"iso-8859-13"	},
    {	28605	,	"iso-8859-15"	},
    {	29001	,	"x-Europa"	},
    {	38598	,	"iso-8859-8-i"	},
    {	50220	,	"iso-2022-jp"	},
    {	50221	,	"csISO2022JP"	},
    {	50222	,	"iso-2022-jp"	},
    {	50225	,	"iso-2022-kr"	},
    {	50227	,	"x-cp50227"	},
    {	51932	,	"euc-jp"	},
    {	51936	,	"EUC-CN"	},
    {	51949	,	"euc-kr"	},
    {	52936	,	"hz-gb-2312"	},
    {	54936	,	"GB18030"	},
    {	57002	,	"x-iscii-de"	},
    {	57003	,	"x-iscii-be"	},
    {	57004	,	"x-iscii-ta"	},
    {	57005	,	"x-iscii-te"	},
    {	57006	,	"x-iscii-as"	},
    {	57007	,	"x-iscii-or"	},
    {	57008	,	"x-iscii-ka"	},
    {	57009	,	"x-iscii-ma"	},
    {	57010	,	"x-iscii-gu"	},
    {	57011	,	"x-iscii-pa"	},
    {	65000	,	"utf-7"	},
    {	65001	,	"utf-8"	}
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
            CStringA sAnsiText; sAnsiText = sText;
            int nStart = 0;
            int nLenth = sAnsiText.GetLength();
            int nFindRes = -1;
            CString sFindString = _T("{\\*filename ");
            int nFindStringLen = sFindString.GetLength();
            CString sFindEnd = _T("\\*end}");
            int nFindEndLen = sFindEnd.GetLength();
            while( -1 != (nFindRes = sText.Find( sFindString, nStart )) )
            {
                oFileWriter.Write( (BYTE*)sAnsiText.GetBuffer() + nStart, nFindRes - nStart );
                sText.ReleaseBuffer();

                int nRightBound = 0;
                nRightBound = sText.Find( sFindEnd, nStart + nFindStringLen );

                CString sTargetFilename = sText.Mid( nFindRes + nFindStringLen, nRightBound - nFindRes - nFindStringLen );

                DecodeFromFile( sTargetFilename, oFileWriter );

                nStart = nRightBound + nFindEndLen;
            }
            oFileWriter.Write( (BYTE*)sAnsiText.GetBuffer() + nStart, nLenth - nStart );
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

    static std::string GetCodepageName( int nCodepage )
    {
        int nCodePagesLength =  140;

        for( int i = 0; i < nCodePagesLength; i++ )
            if( aCodePagesNames[i].id == nCodepage )
                return aCodePagesNames[i].name;

        return "";
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
