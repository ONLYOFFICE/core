#pragma once
#include FT_TRUETYPE_TABLES_H

struct TCharBuffer
{
	char *sBuffer;  // Буфер
	int   nSize;    // Реальный размер буфера
	int   nLen;     // Фактический размер буфера (т.е. размер используемой части буфера)

	TCharBuffer(int _initsize = 32)
	{
		nSize   = _initsize;
		nLen    = 0;

		sBuffer = (char*)MemUtilsMalloc( nSize );
		memset( sBuffer, 0x00, nSize );
	}
	~TCharBuffer()
	{
		if ( sBuffer )
			MemUtilsFree( sBuffer );
	}

	void Write(char nChar)
	{
		if ( nLen + 1 >= nSize )
		{
			nSize *= 2;
			sBuffer = (char*)MemUtilsRealloc( (void*)sBuffer, nSize );
			memset( sBuffer + nLen, 0x00, nSize - nLen );
		}

		sBuffer[nLen++] = nChar;
	}

	void Write(char *sString, int nStringLen = -1)
	{
		if ( nStringLen < 0 )
			nStringLen = strlen( sString );

		while ( nStringLen + nLen >= nSize )
		{
			nSize *= 2;
			sBuffer = (char*)MemUtilsRealloc( (void*)sBuffer, nSize );
			memset( sBuffer + nLen, 0x00, nSize - nLen );
		}

		memcpy( sBuffer + nLen, sString, nStringLen );
		nLen += nStringLen;
	}
	void Write(unsigned char unChar)
	{
		Write( (char)unChar );
	}

	void Write16(int nValue)
	{
		Write( (unsigned char)( (nValue >> 8) & 0xFF ) );
		Write( (unsigned char)( nValue & 0xFF ) );
	}
	void Write32(int nValue)
	{
		Write( (unsigned char)( (nValue >> 24) & 0xFF ) );
		Write( (unsigned char)( (nValue >> 16) & 0xFF ) );
		Write( (unsigned char)( (nValue >> 8) & 0xFF ) );
		Write( (unsigned char)( nValue & 0xFF ) );
	}
};

static void CharBufferWrite(void *pBuffer, char *sData, int nLen) 
{
	TCharBuffer *pCharBuffer = (TCharBuffer*)pBuffer;
	pCharBuffer->Write( sData, nLen );
}

static void FileWrite(void* pFile, char *sData, int nLen) 
{
	::fwrite( sData, 1, nLen, (FILE*)pFile );
	::fflush( (FILE*)pFile );
}

namespace Utils
{
	inline static int     GetDigit   (TCHAR c)
	{
		if (c >= '0' && c <= '9')
			return (int)(c - '0');
		if (c >= 'a' && c <= 'f')
			return 10 + (int)(c - 'a');
		if (c >= 'A' && c <= 'F')
			return 10 + (int)(c - 'A');

		return 0;
	}
	inline static bool     IsDigit   (TCHAR c)
	{
		if (c >= '0' && c <= '9')
			return true;
		return false;
	}
	inline static __int64 GetHex     (const CString& string)
	{
		__int64 nResult = 0;
		int nLen = string.GetLength();
		for ( int nIndex = 0; nIndex < nLen; ++nIndex )
		{
			nResult += GetDigit( string[nIndex] ) << ( 4 * ( nLen - 1 - nIndex ) );
		}

		return nResult;
	}
	inline static int     GetColor   (const CString& string)
	{
		// variables
		int blue = 0;
		int green = 0;
		int red = 0;

		CString color = string; color = color.Trim();
				
		if (color.Find(_T("0x"))!=-1)
			color.Delete(0,2);
		if (color.Find(_T("#"))!=-1)
			color.Delete(0,1);

		while (color.GetLength() < 6)
			color = _T("0") + color;

		red = 16*GetDigit(color[0]) + GetDigit(color[1]);
		green = 16*GetDigit(color[2]) + GetDigit(color[3]);
		blue = 16*GetDigit(color[4]) + GetDigit(color[5]);

		return RGB(red, green, blue);
	}
	inline static BOOL    GetBoolean (const CString& string)
	{
		CString s = string; s.MakeLower();

		return (s == _T("true"));
	}
	inline static bool    GetBoolean2(const CString& string)
	{
		CString sTemp = string; sTemp.MakeLower();

		return ( _T("true") == sTemp || _T("1") == sTemp || _T("t") == sTemp || _T("on") == sTemp );
	}
	inline static int     GetInteger (const CString& string)
	{
		return _ttoi(string);
	}
	inline static int     GetInteger (const char* string)
	{
		return atoi(string);
	}
	inline static double  GetDouble  (const CString& string)
	{
		double d = 0;
		_stscanf(string, _T("%lf"), &d);
		return d;
	}
	inline static double  GetDouble  (const char* string)
	{
		double d = 0;
		sscanf(string, "%lf", &d);
		return d;
	}
	inline static float   GetFloat  (const CString& string)
	{
		float f = 0;
		_stscanf(string, _T("%f"), &f);
		return f;
	}
	inline static float   GetFloat   (const char* string)
	{
		float f = 0;
		sscanf(string, "%f", &f);
		return f;
	}
	inline static int     GetInteger (BSTR string)
	{
		return _wtoi(string);
	}
	inline static size_t  GetUInteger(BSTR string)
	{
		return (size_t)_wtoi(string);
	}
	inline static double  GetDouble  (BSTR string)
	{
		double d = 0;
		swscanf(string, _T("%lf"), &d);
		return d;
	}
	inline static float   GetFloat   (BSTR string)
	{
		float f = 0;
		swscanf(string, _T("%f"), &f);
		return f;
	}
	inline static void    GetDouble  (BSTR string, double* p)
	{
		*p = 0;
		swscanf(string, _T("%lf"), *p);
	}
	inline static void    GetFloat   (BSTR string, float* p)
	{
		*p = 0;
		swscanf(string, _T("%f"), *p);
	}
	inline static void    GetInteger (BSTR string, int* p)
	{
		*p = 0;
		swscanf(string, _T("%d"), *p);
	}

	inline CString BoolToString  (const bool  & value)
	{
		CString sResult = ( value ? _T("true") : _T("false") );
		return sResult;
	}
	inline CString IntToString   (const int   & value)
	{
		CString str = _T("");
		str.Format(_T("%d"), value);
		return str;
	}
	inline CString UIntToString  (const size_t   & value)
	{
		CString str = _T("");
		str.Format(_T("%u"), value);
		return str;
	}
	inline CString FloatToString (const float & value)
	{
		CString str = _T("");
		str.Format(_T("%f"), value);
		return str;
	}
	inline CString DoubleToString(const double& value)
	{
		CString str = _T("");
		str.Format(_T("%lf"), value);
		return str;
	}
	inline static CString GetLower(const CString& string)
	{
		//CString s = string; s.MakeLower();
		//return s;
		CString sResult;

		for( int nIndex = 0; nIndex < string.GetLength(); nIndex++)
			sResult += wchar_t( towlower(string[nIndex]) );

		return sResult;
	}
	inline static CString GetUpper(const CString& string)
	{
		CString sResult;

		for( int nIndex = 0; nIndex < string.GetLength(); nIndex++)
			sResult += wchar_t( towupper(string[nIndex]) );

		return sResult;
	}
}

// Функция взята из Graphics.dll
static int SetCMapForCharCode(FT_Face pFace, long lUnicode, int *pnCMapIndex)
{
	*pnCMapIndex = -1;

	if ( 0 == pFace->num_charmaps )
		return lUnicode;

	int nCharIndex = 0;

	if ( !pFace )
		return nCharIndex;

	for ( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
	{
		FT_CharMap pCharMap = pFace->charmaps[nIndex];

		if ( FT_Set_Charmap( pFace, pCharMap ) )
			continue;

		FT_Encoding pEncoding = pCharMap->encoding;

		if ( FT_ENCODING_UNICODE == pEncoding )
		{
			if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
			{
				*pnCMapIndex = nIndex;
				return nCharIndex;
			}
		}
		else if ( FT_ENCODING_NONE == pEncoding || FT_ENCODING_MS_SYMBOL == pEncoding || FT_ENCODING_APPLE_ROMAN == pEncoding )
		{
			FT_ULong  charcode;
			FT_UInt   gindex;


			charcode = FT_Get_First_Char( pFace, &gindex );                   
			while ( gindex != 0 )                                            
			{                                                               
				charcode = FT_Get_Next_Char( pFace, charcode, &gindex );       
				if ( charcode == lUnicode )
				{
					nCharIndex = gindex;
					*pnCMapIndex = nIndex;
					break;
				}
			}                                                               

			if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
			{
				*pnCMapIndex = nIndex;
			}
		}

		//else if ( FT_ENCODING_ADOBE_STANDARD == pEncoding )
		//{
		//	if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
		//		return nCharIndex;

		//}
		//else if ( FT_ENCODING_ADOBE_CUSTOM == pEncoding )
		//{
		//	if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
		//		return nCharIndex;

		//}
		//else if ( FT_ENCODING_ADOBE_EXPERT == pEncoding )
		//{
		//	if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
		//		return nCharIndex;

		//}
	}

	return nCharIndex;
}

static int GetSymbolicCmapIndex(FT_Face pFace)
{
	TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );
	if ( NULL == pOs2 || 0xFFFF == pOs2->version )
		return -1;

	// Проверяем установлен ли 31 бит
	if ( !( pOs2->ulCodePageRange1 & 0x80000000 ) && !( pOs2->ulCodePageRange1 == 0 && pOs2->ulCodePageRange2 == 0 ) )
		return -1;


	for( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
	{
		// Symbol
		if ( 0 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
			return nIndex;
	}

	return -1;
}