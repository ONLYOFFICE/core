#pragma once
#include "../Common/StringUtils.h"
#include "../Common/ATLDefine.h"
#include "UniversalConverterUtils.h"
#include "../Common/OfficeFileTemplate.h"
#include "../Common/FileWriter.h"
#include "../Common/XmlUtils.h"

#define BUF_SIZE 2048
#define ONE_INCH 2.54

class Convert
{
public:	static  CString ToString(int i)
		{
			CString result;
			result.Format( _T("%i"), i);
			return result;
		}
public:	static  CString ToStringHex( int i, int nLen )
		{
			CString result;
			result.Format( _T("%x"), i);
			for( int i = result.GetLength(); i < nLen; i++ )
				result.Insert( 0 , '0' );
			result.MakeUpper();
			return result;
		}
public:	 static  int ToInt32(CString str, int base = 10)
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
 public: static bool MultybyteToUnicode(CStringA sInputStr, CStringW& sOutputStr, int nCodepage)
		{
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
public: static CStringW Utf8ToUnicode(CStringA sInputStr)//todo
		{
			CStringW sResult;
			int nBufSize = MultiByteToWideChar(CP_UTF8, 0, sInputStr, -1, NULL, NULL);
			LPWSTR p = new WCHAR[ nBufSize + 1 ];
			MultiByteToWideChar(CP_UTF8, 0, sInputStr, -1, p, nBufSize);
			p[ nBufSize ] = 0;
			sResult.Append( p );
			delete p;
			return sResult;
		}
public: static CStringA UnicodeToUtf8(CStringW sInputStr)//todo
		{
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
public: static CString MergeXml( XmlUtils::CXmlReader& oXmlReader1, XmlUtils::CXmlReader& oXmlReader2 )
		{
			CString sResult;
			CString sNodeName1 = oXmlReader1.ReadNodeName();
			CString sNodeName2 = oXmlReader2.ReadNodeName();
			if( sNodeName1 == sNodeName2 )
			{
				//node name
				sResult.AppendFormat( _T("<%s"), sNodeName1);
				//attribute
				XML::IXMLDOMNodePtr pXmlNode;
				if( TRUE == oXmlReader2.GetNode( pXmlNode ) )
				{
					for( int i = 0; i < pXmlNode->attributes->length ; i++ )
					{
						XML::IXMLDOMNodePtr pXmlAttribute = pXmlNode->attributes->Getitem( i );
						CString sAttrXml = pXmlAttribute->xml;
						sResult.AppendFormat( _T(" %s"), sAttrXml );
					}
				}
				oXmlReader1.ReadNodeList( _T("*") );
				oXmlReader2.ReadNodeList( _T("*") );

				int nChildCount1 = oXmlReader1.GetLengthList();
				int nChildCount2 = oXmlReader2.GetLengthList();
				if( nChildCount1 > 0 || nChildCount2 > 0 )
				{
					sResult.Append( _T(">") );
					CAtlArray<int> aIndexesMergedIn2;
					for( int i = 0; i < nChildCount1; i++ )
					{
						CString sSubNodeName1 = oXmlReader1.ReadNodeName( i );
						bool bExist = false;
						int j = 0;
						for( j = 0; j < nChildCount2; j++ )
							if( sSubNodeName1 == oXmlReader2.ReadNodeName( j ) )
							{
								bExist = true;
								aIndexesMergedIn2.Add( j );
								break;
							}
						if( true == bExist )
						{
							XML::IXMLDOMNodePtr pXmlSubNode1;
							XML::IXMLDOMNodePtr pXmlSubNode2;

							oXmlReader1.GetNode( i, pXmlSubNode1 );
							oXmlReader2.GetNode( j, pXmlSubNode2 );

							XmlUtils::CXmlReader oXmlSubReader1;
							XmlUtils::CXmlReader oXmlSubReader2;

							oXmlSubReader1.OpenFromXmlNode( pXmlSubNode1 );
							oXmlSubReader2.OpenFromXmlNode( pXmlSubNode2 );

							sResult.Append( MergeXml( oXmlSubReader1, oXmlSubReader2 ) );
						}
						else
							sResult.Append( oXmlReader1.ReadNodeXml(i) );
					}
					int nCurIndex = 0;
					for( int j = 0; j < nChildCount2; j++ )
					{
						bool bMergedAbove = false;
						for( int k = 0; k < (int)aIndexesMergedIn2.GetCount(); k++ )
							if( j == aIndexesMergedIn2[k] )
							{
								bMergedAbove = true;
								break;
							}
						if( false == bMergedAbove )
						{
							CString sSubNodeName1 = oXmlReader2.ReadNodeName( j );
							bool bExist = false;
							int i = 0;
							for( i = 0; i < nChildCount1; i++ )
								if( sSubNodeName1 == oXmlReader1.ReadNodeName( i ) )
								{
									bExist = true;
									break;
								}
							if( true == bExist )
							{
								XML::IXMLDOMNodePtr pXmlSubNode1;
								XML::IXMLDOMNodePtr pXmlSubNode2;

								oXmlReader1.GetNode( i, pXmlSubNode1 );
								oXmlReader2.GetNode( j, pXmlSubNode2 );

								XmlUtils::CXmlReader oXmlSubReader1;
								XmlUtils::CXmlReader oXmlSubReader2;

								oXmlSubReader1.OpenFromXmlNode( pXmlSubNode1 );
								oXmlSubReader2.OpenFromXmlNode( pXmlSubNode2 );

								sResult.Append( MergeXml( oXmlSubReader1, oXmlSubReader2 ) );
							}
							else
								sResult.Append( oXmlReader2.ReadNodeXml(j) );
						}
					}
					sResult.AppendFormat( _T("</%s>"), sNodeName1 );
				}
				else
					sResult.Append( _T("/>") );
			}
			return sResult;
		}
public: static bool SaveResourceToFile( int nID, CString sType, CString sTarget )
		{
			HRSRC hRes = 0;
			HGLOBAL hData = 0;
			void* pData;	
			HMODULE hModule = _AtlBaseModule.GetModuleInstance();

			hRes = FindResource( hModule, MAKEINTRESOURCE(nID), sType );

			if(hRes == NULL)
				return false;

			DWORD dwSize = SizeofResource(hModule, hRes);

			if(dwSize == NULL)
				return false;

			hData = LoadResource(hModule, hRes);

			if(hData == NULL)
				return false;

			pData = LockResource(hData);

			if(pData == NULL)
			{
				FreeResource( hData );
				return false;
			}
			HANDLE hFile = ::CreateFile( sTarget, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);
			if( INVALID_HANDLE_VALUE == hFile )
			{
				FreeResource( hData );
				return false;
			}
			DWORD dwBytesWritten = 0;
			::WriteFile( hFile, pData, dwSize, &dwBytesWritten, NULL);	
			CloseHandle( hFile );

			FreeResource( hData );
			return true;
		}
public: static float String2Percent( CString sValue )
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
public: static int String2Twips( CString sValue )
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
public: static int px2Twip(int px)
		{
			return 15 * px; //из наблюдений за word
		}
public: static int pc2Twip(double pc)
		{
			return (int)( 12 * pc / 20 );
		}
public: static int mm2Twip(double mm)
		{
			return cm2Twip( mm / 10 );
		}
public: static int cm2Twip(double cm)
		{
			return in2Twip( cm / ONE_INCH );
		}
public: static int in2Twip(double in)
		{
			return (int)( 20 * 72 * in  );
		}
public: static int pt2Twip(double pt)
		{
			return (int)(pt * 20);
		}
public: static float Twip2pt(int pt)
		{
			return (float)(pt / 20.0);
		}
public: static int pt2HalfPt(float pt)
		{
			return (int)(pt * 2);
		}
public: static int Twips2Emu(int pt)
		{
			return (int)(pt * 635);
		}
public: static int Emu2Twips(int pt)
		{
			return (int)(pt / 635);
		}
public: static float Emu2Pt(int emu)
		{
			return (float)(1.0 * emu / (635 * 20.0));
		}
public: static void WriteDataToFileBinary(CString& sFilename, byte* pbData, int nLength)
		{
			if( NULL == pbData )
				return;
			HANDLE hFile = ::CreateFile( sFilename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);

			DWORD dwBytesWritten;
			::WriteFile(hFile,pbData ,nLength, &dwBytesWritten, NULL);	
			CloseHandle( hFile );
		}
public: static void WriteDataToFile(CString& sFilename, CString& sData)
		{
			HANDLE hFile = ::CreateFile( sFilename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, 0);

			DWORD dwBytesWritten;

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
			::WriteFile(hFile,buf2 ,nLengthData, &dwBytesWritten, NULL);	
			sData.ReleaseBuffer();
			delete[] buf2;
			CloseHandle( hFile );
		}
public: static void WriteDataToBinary( CString sData, BYTE** ppData, long& nSize)
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
public: static void ReadDataFromFile(CString sName, CString& sData)
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
public: static CString DecodeHex( CString sText )
		{
			CString sHexText;
			for( int i = 0; i < sText.GetLength(); i++ )
			{
				TBYTE byteChar = sText[i];
				sHexText.AppendFormat( _T("%x"), byteChar );
			}
			return sHexText;
		}
public: static CString EncodeHex( CString sHexText )
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
public: static byte ToByte( TCHAR cChar )
		{
			return (byte)(cChar > 'F' ? cChar - 0x57 : cChar > '9' ? cChar - 0x37 : cChar - 0x30);
		}
public: static bool IsAlpha( int nChar )
		{
			return ( nChar >= 'a' && nChar <= 'z' || nChar >= 'A' && nChar <= 'Z' );
		}
public: static bool IsDigit( int nChar )
		{
			return nChar >= '0' && nChar <= '9';
		}
public: static CString Preserve( CString sText )
		{
			CString sResult = sText;
			//обрезавем лишние пробелы
			sResult.Trim();
			//удаляем дублирующие пробелы
			while( sResult.Replace( _T("  "), _T(" ") ) > 0 )
				;
			return sResult;
		}
public: static int GetFolderSize( CString sFolder )
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
public: static int CharsetToCodepage( int nCharset )
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
public: static int CodepageToCharset( int nCodepage )
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
public: static bool IsMacCharset( int nCharset )
		{
			return nCharset == 77 || nCharset == 78 || nCharset == 79 || nCharset == 80 ||
					nCharset == 81 || nCharset == 83 || nCharset == 84 || nCharset == 85 ||
					nCharset == 86 || nCharset == 87 || nCharset == 88 || nCharset == 89;
		}
public: static bool IsMacCodepage( int nCodepage )
		{
			return nCodepage == 10000 || nCodepage == 10001 || nCodepage == 10003 || nCodepage == 10008 ||
					nCodepage == 10002 || nCodepage == 10005 || nCodepage == 10004 || nCodepage == 10006 ||
					nCodepage == 10081 || nCodepage == 10021 || nCodepage == 10029 || nCodepage == 10007;
		}
private: static const int aCodePages[][2];
private: static const int nCodePagesLength;
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
public: static CString Encode( CString sFilename )
		{
			return _T("{\\*filename ") + sFilename + _T("\\*end}");
		}
public: static void Decode( CString& sText, NFileWriter::CBufferedFileWriter& oFileWriter ) //сразу записывает в файл
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
				oFileWriter.Write( (LPBYTE)sAnsiText.GetBuffer() + nStart, nFindRes - nStart );
				sText.ReleaseBuffer();

				int nRightBound = 0;
				nRightBound = sText.Find( sFindEnd, nStart + nFindStringLen );

				CString sTargetFilename = sText.Mid( nFindRes + nFindStringLen, nRightBound - nFindRes - nFindStringLen );

				DecodeFromFile( sTargetFilename, oFileWriter );

				nStart = nRightBound + nFindEndLen;
			}
			oFileWriter.Write( (LPBYTE)sAnsiText.GetBuffer() + nStart, nLenth - nStart );
			sText.ReleaseBuffer();
		}
private: static void DecodeFromFile( CString& sFilename, NFileWriter::CBufferedFileWriter& oFileWriter )
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
