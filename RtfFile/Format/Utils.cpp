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

#include "Utils.h"

//------------------------------------------------------------------------------------------------------

std::wstring Convert::ToString(_INT32 i)
{
	return std::to_wstring( i );
}
std::wstring Convert::ToStringHex(_INT32 i, size_t nLen )
{
	std::wstring result = XmlUtils::ToString(i, L"%X");

	for (size_t i = result.length(); i < nLen; i++)
	{
		result.insert(result.begin(), '0');
	}
	return result;
}
int Convert::ToInt32(std::wstring str, int base)
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
		swscanf(str.c_str(), L"%X", &nResult);
	else if(8 == base)
		swscanf(str.c_str(), L"%o", &nResult);
	else
		swscanf(str.c_str(), L"%d", &nResult);
#endif
	return nResult;
}

//------------------------------------------------------------------------------------------------------

std::wstring RtfUtility::convertDateTime (int dt)
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
int RtfUtility::convertDateTime (const std::wstring & dt_)
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

		short	Min		= (short)date_time_.time_of_day().minutes();
		short	Hour	= (short)date_time_.time_of_day().hours();
		short	Day		= (short)date_time_.date().day();
		short	Month	= (short)date_time_.date().month().as_number();
		int		Year	= (short)date_time_.date().year() - 1900;

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

std::wstring RtfUtility::RtfInternalEncoder::Encode( std::wstring sFilename )
{
	return L"{\\*filename " + sFilename + L"\\*end}";
}
void RtfUtility::RtfInternalEncoder::Decode( std::wstring& sText, NFileWriter::CBufferedFileWriter& oFileWriter ) //сразу записывает в файл
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
void RtfUtility::RtfInternalEncoder::DecodeFromFile( std::wstring& sFilename, NFileWriter::CBufferedFileWriter& oFileWriter )
{
	NSFile::CFileBinary file;

	if (false == file.OpenFile(sFilename)) return;

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

float RtfUtility::String2Percent( std::wstring sValue )
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
int RtfUtility::String2Twips( std::wstring sValue )
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
int RtfUtility::px2Twip(int px)
{
	return 15 * px; //из наблюдений за word
}
int RtfUtility::pc2Twip(double pc)
{
	return (int)( 12 * pc / 20 );
}
int RtfUtility::mm2Twip(double mm)
{
	return cm2Twip( mm / 10 );
}
int RtfUtility::cm2Twip(double cm)
{
	return in2Twip( cm / ONE_INCH );
}
int RtfUtility::in2Twip(double in)
{
	return (int)( 20 * 72 * in  );
}
int RtfUtility::pt2Twip(double pt)
{
	return (int)(pt * 20);
}
float RtfUtility::Twip2pt(int pt)
{
	return (float)(pt / 20.0);
}
int RtfUtility::pt2HalfPt(float pt)
{
	return (int)(pt * 2);
}
int RtfUtility::Twips2Emu(int pt)
{
	return (int)(pt * 635);
}
int RtfUtility::Emu2Twips(int pt)
{
	return (int)(pt / 635);
}
float RtfUtility::Emu2Pt(int emu)
{
	return (float)(1.0 * emu / (635 * 20.0));
}
void RtfUtility::WriteDataToFileBinary(std::wstring& sFilename, BYTE* pbData, size_t nLength)
{
	if( NULL == pbData )
		return;

	NSFile::CFileBinary file;
	if (false == file.CreateFileW(sFilename)) return;

	file.WriteFile(pbData, (DWORD)nLength);
	file.CloseFile();
}
void RtfUtility::WriteDataToFile(std::wstring& sFilename, std::wstring& sData)
{
	NSFile::CFileBinary file;

	if (false == file.CreateFileW(sFilename)) return;

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
void RtfUtility::DecodeHexString( std::string sHexText, BYTE *&pData )
{
	if (sHexText.empty()) return;
	if (!pData) return;

	for( size_t i = 0; i < sHexText.length() -1 ; i+=2 )
	{
		int byte1 = ToByte( sHexText[i] );
		int byte2 = ToByte(sHexText[i + 1] );
		int cChar = (byte1 << 4) + byte2;
		pData[i/2] = (BYTE)cChar;
	}
}
BYTE RtfUtility::ToByte( wchar_t cChar )
{
	return (BYTE)(cChar > 'F' ? cChar - 0x57 : cChar > '9' ? cChar - 0x37 : cChar - 0x30);
}
bool RtfUtility::IsAlpha( int nChar )
{
	return ( nChar >= 'a' && nChar <= 'z' || nChar >= 'A' && nChar <= 'Z' );
}
bool RtfUtility::IsDigit( int nChar )
{
	return nChar >= '0' && nChar <= '9';
}
std::wstring RtfUtility::Preserve( std::wstring sText )
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
int RtfUtility::CharsetToCodepage( int nCharset )
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
std::wstring RtfUtility::convert_string_icu(std::string::const_iterator start, std::string::const_iterator end, int nCodepage)
{
	if (start == end) return L"";

	std::string sCodePage;
	std::map<int, std::string>::const_iterator pFind = NSUnicodeConverter::mapEncodingsICU.find(nCodepage);
	if (pFind != NSUnicodeConverter::mapEncodingsICU.end())
	{
		sCodePage = pFind->second;
	}
	unsigned int insize = (unsigned int)(end - start);
	const char* inptr = (const char*)start.operator ->();

	if (!sCodePage.empty())
	{
		NSUnicodeConverter::CUnicodeConverter oConverter;
		return oConverter.toUnicode(inptr, insize, sCodePage.c_str(), true);
	}
	else if (nCodepage != 0)
	{
		NSUnicodeConverter::CUnicodeConverter oConverter;
		return oConverter.toUnicode(inptr, insize, nCodepage, true);
	}
	else //сф_850000158725_R7_M194_МО_Q194.rtf
	{//текущая локаль

		std::locale loc("");
		std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);

		std::wstring result;
		result.resize(insize);

		facet.widen(inptr, inptr + insize, &result[0]);
		return result;
	}
}
std::string RtfUtility::convert_string_icu(std::wstring::const_iterator start, std::wstring::const_iterator end, int nCodepage)
{
	std::string sCodePage;
	std::map<int, std::string>::const_iterator pFind = NSUnicodeConverter::mapEncodingsICU.find(nCodepage);
	if (pFind != NSUnicodeConverter::mapEncodingsICU.end())
	{
		sCodePage = pFind->second;
	}

	if (sCodePage.empty() && nCodepage > 0)
		sCodePage = "CP" + std::to_string(nCodepage);

	unsigned int insize = (unsigned int)(end - start);
	const wchar_t* inptr = (const wchar_t*)start.operator ->();

	NSUnicodeConverter::CUnicodeConverter oConverter;
	return oConverter.fromUnicode(inptr, insize, sCodePage.c_str());
}
int RtfUtility::CodepageToCharset( int nCodepage )
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
bool RtfUtility::IsMacCharset( int nCharset )
{
	return nCharset == 77 || nCharset == 78 || nCharset == 79 || nCharset == 80 ||
			nCharset == 81 || nCharset == 83 || nCharset == 84 || nCharset == 85 ||
			nCharset == 86 || nCharset == 87 || nCharset == 88 || nCharset == 89;
}
bool RtfUtility::IsMacCodepage( int nCodepage )
{
	return nCodepage == 10000 || nCodepage == 10001 || nCodepage == 10003 || nCodepage == 10008 ||
			nCodepage == 10002 || nCodepage == 10005 || nCodepage == 10004 || nCodepage == 10006 ||
			nCodepage == 10081 || nCodepage == 10021 || nCodepage == 10029 || nCodepage == 10007;
}

