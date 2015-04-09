#pragma once

#include "../../../Common/FileWriter.h"

#include "UniversalConverterUtils.h"

#define BUF_SIZE 2048
#define ONE_INCH 2.54

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
	static COLORREF ToColor(const CString& strValue)
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
	static BOOL ToBoolean(const CString& strValue)
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
	static CString FromBoolean(BOOL Value)
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
	static bool MultybyteToUnicode(CStringA sInputStr, CStringW& sOutputStr, int nCodepage)
	{
		//todooo ѕ≈–≈ѕ»—ј“№ 
		sOutputStr = _T("");
		int nBufSize = MultiByteToWideChar(nCodepage, 0, sInputStr, -1, NULL, NULL);
		LPWSTR p = new WCHAR[ nBufSize + 1 ];
		int nRes = MultiByteToWideChar(nCodepage, MB_ERR_INVALID_CHARS, sInputStr, -1, p, nBufSize);
		p[ nBufSize ] = 0;
		if( 0 != nRes )
		{
			sOutputStr.Append( p );
			delete p;
			return true;
		}
		else
			return false;
	}
	static CStringW Utf8ToUnicode(CStringA sInputStr)//todo
	{
		//todooo ѕ≈–≈ѕ»—ј“№ 
		CStringW sResult;
		int nBufSize = MultiByteToWideChar(CP_UTF8, 0, sInputStr, -1, NULL, NULL);
		LPWSTR p = new WCHAR[ nBufSize + 1 ];
		MultiByteToWideChar(CP_UTF8, 0, sInputStr, -1, p, nBufSize);
		p[ nBufSize ] = 0;
		sResult.Append( p );
		delete p;
		return sResult;
	}
	static CStringA UnicodeToUtf8(CStringW sInputStr)//todo
	{
		//todooo ѕ≈–≈ѕ»—ј“№ 
		CStringA sResult;
		int nBufSize = WideCharToMultiByte(CP_UTF8, 0, sInputStr, -1, NULL, NULL, NULL, NULL);
		LPSTR p = new CHAR[ nBufSize + 1 ];
		WideCharToMultiByte(CP_UTF8, 0, sInputStr, -1, p, nBufSize, NULL, NULL);
		p[ nBufSize ] = 0;
		sResult.Append( p );
		delete p;
		return sResult;
	}
};

class RtfUtility
{
public: 

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
		static void WriteDataToFileBinary(CString& sFilename, byte* pbData, int nLength)
		{
			if( NULL == pbData )
				return;

			CFile file;
			if (file.CreateFileW(sFilename) != S_OK) return;

			DWORD dwBytesWritten;
			file.WriteFile(pbData ,nLength);	
			file.CloseFile();
		}
		static void WriteDataToFile(CString& sFilename, CString& sData)
		{
			CFile file;

			if (file.CreateFileW(sFilename) != S_OK) return;

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
		static void ReadDataFromFile(CString sName, CString& sData)
		{
			HANDLE hFile = ::CreateFile(sName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, 0);
			DWORD dwBytesRead = 0;
			BYTE byteBuffer[ BUF_SIZE ];

			::ReadFile(hFile,byteBuffer ,BUF_SIZE, &dwBytesRead, NULL);
			while( 0 != dwBytesRead )
			{
				for( int i = 0; i < (int)dwBytesRead; i++ )
				{
					int n1 =(byteBuffer[i]&0xF0) >> 4;
					int n2 = byteBuffer[i]&0x0F;
					sData.AppendFormat( _T("%x"), n1);
					sData.AppendFormat( _T("%x"), n2);
				}
				::ReadFile(hFile,byteBuffer ,BUF_SIZE, &dwBytesRead, NULL);
			}
			CloseHandle( hFile );
		}
		static CString DecodeHex( CString sText )
		{
			CString sHexText;
			for( int i = 0; i < sText.GetLength(); i++ )
			{
				TBYTE byteChar = sText[i];
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
		static byte ToByte( TCHAR cChar )
		{
			return (byte)(cChar > 'F' ? cChar - 0x57 : cChar > '9' ? cChar - 0x37 : cChar - 0x30);
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
			//удал€ем дублирующие пробелы
			while( sResult.Replace( _T("  "), _T(" ") ) > 0 )
				;
			return sResult;
		}
		static int GetFolderSize( CString sFolder )
		{
			HANDLE Handle;
			WIN32_FIND_DATA FindData;
			DWORDLONG Result=0;

			int nCount = 0;
			Handle = FindFirstFile(( sFolder + _T("\\*.*") ), &FindData);
			if (Handle == INVALID_HANDLE_VALUE)
				return 0;
			do
			{
				if(( CString( FindData.cFileName) != _T("."))&&(CString(FindData.cFileName) != _T("..")))
					nCount++;
			}
			while(FindNextFile(Handle, &FindData) != 0);

			FindClose(Handle);
			return nCount;
		}
	static int CharsetToCodepage( int nCharset )
	{
		CHARSETINFO Info;
		DWORD* dwAcp = (DWORD*)nCharset;
		if( TRUE == TranslateCharsetInfo(dwAcp, &Info, TCI_SRCCHARSET) )
			return Info.ciACP;

		for( int i = 0; i < nCodePagesLength; i++ )
			if( aCodePages[i][0] == nCharset )
				return aCodePages[i][1];
		return 1252;//ANSI
	}
	static int CodepageToCharset( int nCodepage )
	{
		CHARSETINFO Info;
		DWORD* dwAcp = (DWORD*)nCodepage;
		if( TRUE == TranslateCharsetInfo(dwAcp, &Info, TCI_SRCCODEPAGE) )
			return Info.ciCharset;

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
private: 
	static const int aCodePages[][2];
	static const int nCodePagesLength;
};
__declspec(selectany) const int RtfUtility::aCodePages[][2] = {
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
__declspec(selectany) const int RtfUtility::nCodePagesLength =  sizeof( aCodePages ) / ( sizeof( int ) );
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
		HANDLE hFile = ::CreateFile( sFilename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, 0);
		DWORD dwBytesRead = 0;
		BYTE byteBuffer[ BUF_SIZE ];

		char aLookup[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

		::ReadFile(hFile,byteBuffer ,BUF_SIZE, &dwBytesRead, NULL);
		while( 0 != dwBytesRead )
		{
			for( int i = 0; i < (int)dwBytesRead; i++ )
			{
				byte byteData = byteBuffer[ i ];
				byte byteFirst = aLookup[ byteData / 0x10 ];
				byte byteSecond = aLookup[ byteData % 0x10 ];
				oFileWriter.Write( &byteFirst, 1 );
				oFileWriter.Write( &byteSecond, 1 );
			}
			::ReadFile( hFile, byteBuffer, BUF_SIZE, &dwBytesRead, NULL );
		}
		CloseHandle( hFile );
	 }
};
