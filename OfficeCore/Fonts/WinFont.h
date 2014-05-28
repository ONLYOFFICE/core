#ifndef _WIN_FONT_H
#define _WIN_FONT_H

#include <shlobj.h>

// FreeType
#include "FreeType/ft2build.h"
#include FT_FREETYPE_H
#pragma comment (lib, "Fonts\\FreeType\\freetype242_vs2005.lib")

#include "FreeType/freetype/ftsnames.h"
#include "FreeType/freetype/tttables.h"
#include "FreeType/freetype/ftxf86.h"

#include "List.h"
#include "FontUtils.h"
#include "../../Common/XmlUtils.h"

#include "../../DesktopEditor/freetype_names/FontMaps/FontDictionaryWorker.h"

static _bstr_t g_cpszXML_Font_Name    = L"name";
static _bstr_t g_cpszXML_Font_Bold	  = L"bold";
static _bstr_t g_cpszXML_Font_Italic  = L"italic";
static _bstr_t g_cpszXML_Font_Path    = L"path";
static _bstr_t g_cpszXML_Font_Index   = L"index";

static LONG g_lSizeofLONG			  = sizeof(LONG);
static LONG g_lSizeofBOOL			  = sizeof(BOOL);
static LONG g_lSizeofWCHAR			  = sizeof(WCHAR);
static LONG g_lSizeofULONG            = sizeof(ULONG);
static LONG g_lSizeofUSHORT           = sizeof(USHORT);
static LONG g_lSizeofSHORT            = sizeof(SHORT);

//-------------------------------------------------------------------------------------------------------------------------------
// CWinFontInfo
//-------------------------------------------------------------------------------------------------------------------------------

enum EFontFormat
{
	fontWindowsFNT = 0, // *.fon
	fontTrueType   = 1, // *.ttf
	fontOpenType   = 2, // *.ttf, *.otf (CFF формат)
	fontUnknown    = 3
};

class CWinFontInfo
{
public:
	CWinFontInfo(const CStringW &wsFontName, const CStringW &wsStyle, const CString &wsFontPath, long lIndex, BOOL bBold, BOOL bItalic, BOOL bFixedWidth, BYTE *pPanose, ULONG ulRange1, ULONG ulRange2, ULONG ulRange3, ULONG ulRange4, ULONG ulCodeRange1, ULONG ulCodeRange2, USHORT usWeigth, USHORT usWidth, SHORT sFamilyClass, EFontFormat eFormat, SHORT shAvgCharWidth, SHORT shAscent, SHORT shDescent, SHORT shLineGap, SHORT shXHeight, SHORT shCapHeight)
	{
		m_wsFontName = wsFontName;
		m_wsFontPath = wsFontPath;
		m_wsStyle    = wsStyle;
		m_lIndex     = lIndex;

		m_bBold      = bBold;
		m_bItalic    = bItalic;

		m_bIsFixed   = bFixedWidth;
		
		if ( pPanose )
			memcpy( (void*)m_aPanose, (const void *)pPanose, 10 );
		else
			memset( (void*)m_aPanose, 0x00, 10 );

		m_ulUnicodeRange1  = ulRange1;
		m_ulUnicodeRange2  = ulRange2;
		m_ulUnicodeRange3  = ulRange3;
		m_ulUnicodeRange4  = ulRange4;
		m_ulCodePageRange1 = ulCodeRange1;
		m_ulCodePageRange2 = ulCodeRange2;
		m_usWeigth         = usWeigth;         
		m_usWidth          = usWidth;	

		m_sFamilyClass     = sFamilyClass;

		m_eFontFormat      = eFormat;

		m_shAvgCharWidth   = shAvgCharWidth;
		m_shAscent         = shAscent;
		m_shDescent        = shDescent;
		m_shLineGap        = shLineGap;
		m_shXHeight        = shXHeight;
		m_shCapHeight      = shCapHeight;
	}
	CWinFontInfo(BSTR wsFontName, BSTR wsFontPath, const long& lIndex, const BOOL& bBold, const BOOL& bItalic, const BOOL& bFixedWidth, BYTE *pPanose, const ULONG& ulRange1, const ULONG& ulRange2, const ULONG& ulRange3, const ULONG& ulRange4, const ULONG& ulCodeRange1, const ULONG& ulCodeRange2, const USHORT& usWeigth, const USHORT& usWidth, const SHORT& sFamilyClass, EFontFormat eFormat, const SHORT& shAvgCharWidth, const SHORT& shAscent, const SHORT& shDescent, const SHORT& shLineGap, const SHORT& shXHeight, const SHORT& shCapHeight)
	{
		m_wsFontName = wsFontName;
		m_wsFontPath = wsFontPath;
		m_wsStyle    = L"";
		m_lIndex     = lIndex;

		m_bBold      = bBold;
		m_bItalic    = bItalic;

		m_bIsFixed   = bFixedWidth;

		if ( pPanose )
			memcpy( (void*)m_aPanose, (const void *)pPanose, 10 );
		else
			memset( (void*)m_aPanose, 0x00, 10 );

		m_ulUnicodeRange1  = ulRange1;
		m_ulUnicodeRange2  = ulRange2;
		m_ulUnicodeRange3  = ulRange3;
		m_ulUnicodeRange4  = ulRange4;
		m_ulCodePageRange1 = ulCodeRange1;
		m_ulCodePageRange2 = ulCodeRange2;
		m_usWeigth         = usWeigth;         
		m_usWidth          = usWidth;		

		m_sFamilyClass     = sFamilyClass;
		m_eFontFormat      = eFormat;

		m_shAvgCharWidth   = shAvgCharWidth;
		m_shAscent         = shAscent;
		m_shDescent        = shDescent;
		m_shLineGap        = shLineGap;
		m_shXHeight        = shXHeight;
		m_shCapHeight      = shCapHeight;
	}
	virtual ~CWinFontInfo() 
	{
	}
	BOOL Equals(CWinFontInfo *pFontInfo) 
	{
		return ( m_wsFontName == pFontInfo->m_wsFontName && m_wsStyle == pFontInfo->m_wsStyle && m_wsFontPath == pFontInfo->m_wsFontPath && m_bItalic == pFontInfo->m_bItalic && m_bBold == pFontInfo->m_bBold );
	}

	void WriteToXml(XmlUtils::CXmlWriter *pWriter)
	{
		FromXmlString( m_wsFontName );
		FromXmlString( m_wsFontPath );

		pWriter->WriteNodeBegin( _T("WinFontEntry"), TRUE );
		pWriter->WriteAttribute( _T("name"), m_wsFontName );
		pWriter->WriteAttribute( _T("bold"), (int)m_bBold );
		pWriter->WriteAttribute( _T("italic"), (int)m_bItalic );
		pWriter->WriteAttribute( _T("path"), m_wsFontPath );
		pWriter->WriteAttribute( _T("index"), (int)m_lIndex );
		pWriter->WriteNodeEnd( _T("WinFontEntry"), TRUE );
	}

	inline static BSTR GetAttribute(XML::IXMLDOMNamedNodeMapPtr pAttributes, _bstr_t& bsName)
	{
		IXMLDOMNodePtr tempNode = pAttributes->getNamedItem(bsName);
		
		if (NULL == tempNode)
			return NULL;
		
		BSTR bsValue;
		tempNode->get_text(&bsValue);
		return bsValue;
	}

	static CWinFontInfo *FromXml(XmlUtils::CXmlNode oNode)
	{
		if ( _T("WinFontEntry") == oNode.GetName() )
		{
			XML::IXMLDOMNamedNodeMapPtr pAttributes = oNode.GetAttributes();

			BSTR bsBold		= GetAttribute(pAttributes, g_cpszXML_Font_Bold);
			BSTR bsItalic	= GetAttribute(pAttributes, g_cpszXML_Font_Italic);
			BSTR bsIndex	= GetAttribute(pAttributes, g_cpszXML_Font_Index);
			
			BOOL    bBold   = (BOOL)XmlUtils::GetInteger(bsBold);
			BOOL    bItalic = (BOOL)XmlUtils::GetInteger(bsItalic);
			BSTR sName		= GetAttribute(pAttributes, g_cpszXML_Font_Name);
			BSTR sPath		= GetAttribute(pAttributes, g_cpszXML_Font_Path);
			long    lIndex  = (long)XmlUtils::GetInteger(bsIndex);

			CWinFontInfo* pInfo = new CWinFontInfo( sName, sPath, lIndex, bBold, bItalic, FALSE, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, (EFontFormat)0, 0, 0, 0, 0, 0, 0 );
			
			SysFreeString(bsBold);
			SysFreeString(bsItalic);
			SysFreeString(bsIndex);
			SysFreeString(sName);
			SysFreeString(sPath);

			return pInfo;
		}

		return NULL;
	}

	static CWinFontInfo* FromBuffer(BYTE*& pBuffer, CString strFontDir)
	{
		// name		
		LONG lLen = *((LONG*)pBuffer);
		pBuffer += g_lSizeofLONG;

		BSTR bsName = (BSTR)pBuffer;
		pBuffer += lLen;

		// path
		lLen = *((LONG*)pBuffer);
		pBuffer += g_lSizeofLONG;

		BSTR bsPath = (BSTR)pBuffer;
		pBuffer += lLen;

		// index
		LONG lIndex = *((LONG*)pBuffer);
		pBuffer += g_lSizeofLONG;

		// italic
		BOOL bItalic = *((BOOL*)pBuffer);
		pBuffer += g_lSizeofBOOL;

		// bold
		BOOL bBold = *((BOOL*)pBuffer);
		pBuffer += g_lSizeofBOOL;

		// FixedWidth
		BOOL bFixedWidth = *((BOOL*)pBuffer);
		pBuffer += g_lSizeofBOOL;

		// Panose
		lLen = *((LONG*)pBuffer); // должно быть равно 10
		pBuffer += g_lSizeofLONG;

		BYTE pPanose[10];
		memcpy( (void *)pPanose, (const void *)pBuffer, 10 );
		pBuffer += lLen;

		// ulUnicodeRange1
		ULONG ulRange1 = *((ULONG*)pBuffer);
		pBuffer += g_lSizeofULONG;

		// ulUnicodeRange2
		ULONG ulRange2 = *((ULONG*)pBuffer);
		pBuffer += g_lSizeofULONG;

		// ulUnicodeRange3
		ULONG ulRange3 = *((ULONG*)pBuffer);
		pBuffer += g_lSizeofULONG;

		// ulUnicodeRange4
		ULONG ulRange4 = *((ULONG*)pBuffer);
		pBuffer += g_lSizeofULONG;

		// ulCodePageRange1
		ULONG ulCodeRange1 = *((ULONG*)pBuffer);
		pBuffer += g_lSizeofULONG;

		// ulCodePageRange2
		ULONG ulCodeRange2 = *((ULONG*)pBuffer);
		pBuffer += g_lSizeofULONG;

		// usWeightClass
		USHORT usWeight = *((USHORT*)pBuffer);
		pBuffer += g_lSizeofUSHORT;

		// usWidthClass
		USHORT usWidth = *((USHORT*)pBuffer);
		pBuffer += g_lSizeofUSHORT;

		// sFamilyClass
		SHORT sFamilyClass = *((SHORT*)pBuffer);
		pBuffer += g_lSizeofSHORT;

		// FontFormat
		SHORT sFormat = *((SHORT*)pBuffer);
		pBuffer += g_lSizeofSHORT;

		// AvgCharWidth
		SHORT shAvgCharWidth = *((SHORT*)pBuffer);
		pBuffer += g_lSizeofSHORT;

		// Ascent
		SHORT shAscent = *((SHORT*)pBuffer);
		pBuffer += g_lSizeofSHORT;

		// Descent
		SHORT shDescent = *((SHORT*)pBuffer);
		pBuffer += g_lSizeofSHORT;

		// LineGap
		SHORT shLineGap = *((SHORT*)pBuffer);
		pBuffer += g_lSizeofSHORT;

		// XHeight
		SHORT shXHeight = *((SHORT*)pBuffer);
		pBuffer += g_lSizeofSHORT;

		// CapHeight
		SHORT shCapHeight = *((SHORT*)pBuffer);
		pBuffer += g_lSizeofSHORT;

		CStringW strPath = (CStringW)bsPath;
		strPath = strFontDir + strPath;
		bsPath = strPath.AllocSysString();

		CWinFontInfo* pInfo = new CWinFontInfo( bsName, bsPath, lIndex, bBold, bItalic, bFixedWidth, (BYTE*)pPanose, ulRange1, ulRange2, ulRange3, ulRange4, ulCodeRange1, ulCodeRange2, usWeight, usWidth, sFamilyClass, (EFontFormat)sFormat, shAvgCharWidth, shAscent, shDescent, shLineGap, shXHeight, shCapHeight );

		SysFreeString(bsPath);
		return pInfo;		
	}
	LONG GetBufferLen(CString strDirectory = _T(""))
	{
		CStringW sPath = m_wsFontPath;
		if (0 != strDirectory.GetLength())
		{
			if (0 == sPath.Find(strDirectory))
			{
				sPath = sPath.Mid(strDirectory.GetLength());
			}
		}
		return 4 * g_lSizeofLONG + 3 * g_lSizeofBOOL + (m_wsFontName.GetLength() + sPath.GetLength() + 2) * g_lSizeofWCHAR + 2 * g_lSizeofUSHORT + 6 * g_lSizeofULONG + 10 + 8 * g_lSizeofSHORT;
	}
	void ToBuffer(BYTE*& pBuffer, CString strDirectory = _T(""))
	{
		// name	
		LONG lLen = (m_wsFontName.GetLength() + 1) * g_lSizeofWCHAR;

		*((LONG*)(pBuffer))	= lLen; 
		pBuffer += g_lSizeofLONG;

		memcpy(pBuffer, m_wsFontName.GetBuffer(), lLen);
		pBuffer += lLen;

		// path
		CStringW sPath = m_wsFontPath;
		if (0 != strDirectory.GetLength())
		{
			if (0 == sPath.Find(strDirectory))
			{
				sPath = sPath.Mid(strDirectory.GetLength());
			}
		}

		lLen = (sPath.GetLength() + 1) * g_lSizeofWCHAR;

		*((LONG*)(pBuffer))	= lLen; 
		pBuffer += g_lSizeofLONG;

		memcpy(pBuffer, sPath.GetBuffer(), lLen);
		pBuffer += lLen;

		// index
		*((LONG*)(pBuffer))	= m_lIndex;
		pBuffer += g_lSizeofLONG;

		// italic
		*((BOOL*)(pBuffer))	= m_bItalic;
		pBuffer += g_lSizeofBOOL;

		// bold
		*((BOOL*)(pBuffer))	= m_bBold;
		pBuffer += g_lSizeofBOOL;

		// FixedWidth
		*((BOOL*)pBuffer) = m_bIsFixed;
		pBuffer += g_lSizeofBOOL;

		// Panose
		lLen = 10;

		*((LONG*)(pBuffer))	= lLen; 
		pBuffer += g_lSizeofLONG;

		memcpy( (void *)pBuffer, (const void *)m_aPanose, lLen );
		pBuffer += lLen;

		// ulUnicodeRange1
		*((ULONG*)pBuffer) = m_ulUnicodeRange1;
		pBuffer += g_lSizeofULONG;

		// ulUnicodeRange2
		*((ULONG*)pBuffer) = m_ulUnicodeRange2;
		pBuffer += g_lSizeofULONG;

		// ulUnicodeRange3
		*((ULONG*)pBuffer) = m_ulUnicodeRange3;
		pBuffer += g_lSizeofULONG;

		// ulUnicodeRange4
		*((ULONG*)pBuffer) = m_ulUnicodeRange4;
		pBuffer += g_lSizeofULONG;

		// ulCodePageRange1
		*((ULONG*)pBuffer) = m_ulCodePageRange1;
		pBuffer += g_lSizeofULONG;

		// ulCodePageRange2
		*((ULONG*)pBuffer) = m_ulCodePageRange2;
		pBuffer += g_lSizeofULONG;

		// usWeightClass
		*((USHORT*)pBuffer) = m_usWeigth;
		pBuffer += g_lSizeofUSHORT;

		// usWidthClass
		*((USHORT*)pBuffer) = m_usWidth;
		pBuffer += g_lSizeofUSHORT;

		// sFamilyClass
		*((SHORT*)pBuffer) = m_sFamilyClass;
		pBuffer += g_lSizeofSHORT;

		// FontFormat
		*((SHORT*)pBuffer) = (SHORT)m_eFontFormat;
		pBuffer += g_lSizeofSHORT;

		// AvgCharWidth
		*((SHORT*)pBuffer) = (SHORT)m_shAvgCharWidth;
		pBuffer += g_lSizeofSHORT;

		// Ascent
		*((SHORT*)pBuffer) = (SHORT)m_shAscent;
		pBuffer += g_lSizeofSHORT;

		// Descent
		*((SHORT*)pBuffer) = (SHORT)m_shDescent;
		pBuffer += g_lSizeofSHORT;

		// LineGap
		*((SHORT*)pBuffer) = (SHORT)m_shLineGap;
		pBuffer += g_lSizeofSHORT;

		// XHeight
		*((SHORT*)pBuffer) = (SHORT)m_shXHeight;
		pBuffer += g_lSizeofSHORT;

		// CapHeight
		*((SHORT*)pBuffer) = (SHORT)m_shCapHeight;
		pBuffer += g_lSizeofSHORT;

	}

	static void FromXmlString(CString& strText)
	{
		strText.Replace(_T("&apos;"),	_T("'"));
		strText.Replace(_T("&lt;"),		_T("<"));
		strText.Replace(_T("&gt;"),		_T(">"));
		strText.Replace(_T("&quot;"),	_T("\""));
		strText.Replace(_T("&amp;"),	_T("&"));
	}

public:

	CStringW     m_wsFontName;   // Имя шрифта
	CStringW     m_wsFontPath;   // Путь к файлу с шрифтом
	long         m_lIndex;       // Номер шрифта в файле(если в файле больше 1 шрифта)
	CStringW     m_wsStyle;

	BOOL         m_bBold;            // Bold text
	BOOL         m_bItalic;          // Italic text
	BOOL         m_bIsFixed;         // Моноширинный шрифт?

	BYTE         m_aPanose[10];
	ULONG	     m_ulUnicodeRange1;  // Bits 0-31
	ULONG        m_ulUnicodeRange2;  // Bits 32-63
	ULONG        m_ulUnicodeRange3;  // Bits 64-95
	ULONG        m_ulUnicodeRange4;  // Bits 96-127

	ULONG        m_ulCodePageRange1; // Bits 0-31
	ULONG        m_ulCodePageRange2; // Bits 32-63

	USHORT       m_usWeigth;         
	USHORT       m_usWidth;

	SHORT        m_sFamilyClass;
	EFontFormat  m_eFontFormat;

	SHORT        m_shAvgCharWidth;   // Средняя ширина символов
	SHORT        m_shAscent;         // Ascent
	SHORT        m_shDescent;        // Descent
	SHORT        m_shLineGap;        // Межсимвольный интервал
	SHORT        m_shXHeight;        // Высота буквы 'x' (в нижнем регистре)
	SHORT        m_shCapHeight;      // Высота буквы 'H' (в верхнем регистре)

};

//-------------------------------------------------------------------------------------------------------------------------------
// CWinFontList
//-------------------------------------------------------------------------------------------------------------------------------

class CWinFontList 
{
public:

	CWinFontList(FT_Library pLibrary) 
	{
		m_lDefIndex = -1;

		m_pFonts = new CList();

		// Ищем директорию с фонтами (обычно это C:\Windows\Fonts)
		m_wsWinFontDir[0] = '\0';
		if ( !SHGetSpecialFolderPathW( NULL, m_wsWinFontDir, CSIDL_FONTS, FALSE ) )
			m_wsWinFontDir[0] = '\0';

		OSVERSIONINFO oVersion;
		oVersion.dwOSVersionInfoSize = sizeof(oVersion);
		GetVersionEx( &oVersion );

		wchar_t wsPath[1000];
		if ( oVersion.dwPlatformId == VER_PLATFORM_WIN32_NT ) 
			_tcscpy_s( wsPath, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts\\") );
		else 
			_tcscpy_s( wsPath, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Fonts\\") );

		TCHAR szName[2 * MAX_PATH];
		TCHAR szData[2 * MAX_PATH];
		while ( GetNextNameValue( HKEY_LOCAL_MACHINE, wsPath, szName, szData ) == ERROR_SUCCESS) 
		{
			int nError = 0;
			FT_Face pFace = NULL;
			CStringW wsFilePath( szData );

			if ( nError = FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), 0, &pFace ) )
			{
				wsFilePath = CStringW( m_wsWinFontDir ) + CStringW( _T("\\") ) + CStringW( szData );
				wsFilePath.Replace( _T("\0"), _T("") );
				if ( nError = FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), 0, &pFace ) )
				{
					wsPath[0] = _T('\0');
					continue;
				}
			}

			// TO DO: Шрифты, которые нельзя скейлить (т.е. изменять размер 
			// произвольно) мы не грузим. Возможно в будущем надо будет
			// сделать, чтобы работал и такой вариант. (в Word такие шрифты
			// не используются)
			if ( !( pFace->face_flags & FT_FACE_FLAG_SCALABLE ) )
			{
				FT_Done_Face( pFace );
				wsPath[0] = _T('\0');
				continue;
			}

			int nFacesCount = pFace->num_faces;
			if ( nError = FT_Done_Face( pFace ) )
			{
				wsPath[0] = _T('\0');
				continue;
			}

			for ( int nIndex = 0; nIndex < nFacesCount; nIndex++ )
			{
				if ( FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), nIndex, &pFace ) )
				{
					wsPath[0] = _T('\0');
					continue;
				}

				BOOL bBold   = ( pFace->style_flags & FT_STYLE_FLAG_BOLD ? 1 : 0 );
				BOOL bItalic = pFace->style_flags & FT_STYLE_FLAG_ITALIC;

				
				CStringW wsPostscriptName = CStringW( FT_Get_Postscript_Name( pFace ) );
				//if ( wsPostscriptName.GetLength() <= 0 )
				//	CStringW wsPostscriptName = CStringW( (char*)pFace->family_name );

				int nNamesCount = FT_Get_Sfnt_Name_Count( pFace );
				
				for ( int nNameIndex = 0; nNameIndex < nNamesCount; nNameIndex++ )
				{
					FT_SfntName_ oSfntName;
					FT_Get_Sfnt_Name( pFace, nNameIndex, &oSfntName );


					if ( oSfntName.language_id == 0x0409 && oSfntName.name_id == 1 && oSfntName.platform_id == 3 )
					{
						// Family
						int k= 10;
					}
					if ( oSfntName.language_id == 0x0409 && oSfntName.name_id == 16 && oSfntName.platform_id == 3 )
					{
						// Preffered family
						int k= 10;
					}
				}

				BOOL bFixedWidth = FT_IS_FIXED_WIDTH( pFace );

				TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );
				
				BYTE *pPanose = NULL;
				ULONG ulRange1 = 0, ulRange2 = 0, ulRange3 = 0, ulRange4 = 0, ulCodeRange1 = 0, ulCodeRange2 = 0;
				USHORT usWidth = 0, usWeight = 0;
				SHORT sFamilyClass = 0;

				SHORT shAvgCharWidth = 0, shAscent = 0, shDescent = 0, shLineGap = 0, shXHeight = 0, shCapHeight = 0;
				if ( NULL != pOs2 )
				{
					pPanose        = (BYTE *)pOs2->panose;

					ulRange1       = pOs2->ulUnicodeRange1;
					ulRange2       = pOs2->ulUnicodeRange2;
					ulRange3       = pOs2->ulUnicodeRange3;
					ulRange4       = pOs2->ulUnicodeRange4;
					ulCodeRange1   = pOs2->ulCodePageRange1;
					ulCodeRange2   = pOs2->ulCodePageRange2;

					usWeight       = pOs2->usWeightClass;
					usWidth        = pOs2->usWidthClass;
					
					sFamilyClass   = pOs2->sFamilyClass;

					if ( 0 != pFace->units_per_EM )
					{
						double dKoef = ( 1000 / (double)pFace->units_per_EM );
						shAvgCharWidth = (SHORT)(pOs2->xAvgCharWidth  * dKoef);
						shAscent       = (SHORT)(pOs2->sTypoAscender  * dKoef);
						shDescent      = (SHORT)(pOs2->sTypoDescender * dKoef);
						shLineGap      = (SHORT)(pOs2->sTypoLineGap   * dKoef);
						shXHeight      = (SHORT)(pOs2->sxHeight       * dKoef);
						shCapHeight    = (SHORT)(pOs2->sCapHeight     * dKoef);
					}
					else
					{
						shAvgCharWidth = (SHORT)pOs2->xAvgCharWidth;
						shAscent       = (SHORT)pOs2->sTypoAscender;
						shDescent      = (SHORT)pOs2->sTypoDescender;
						shLineGap      = (SHORT)pOs2->sTypoLineGap;
						shXHeight      = (SHORT)pOs2->sxHeight;
						shCapHeight    = (SHORT)pOs2->sCapHeight;
					}
				}

				if ( true )
				{
					// Специальная ветка для случаев, когда charset может быть задан не через значения
					// ulCodePageRange, а непосредственно через тип Cmap.

					//  Charset Name       Charset Value(hex)  Codepage number   Platform_ID   Encoding_ID   Description
					//  -------------------------------------------------------------------------------------------------
					//
					//  SYMBOL_CHARSET            2 (x02)                             3            0           Symbol
					//  SHIFTJIS_CHARSET        128 (x80)             932             3            2           ShiftJIS
					//  GB2313_CHARSET          134 (x86)             936             3            3           PRC
					//  CHINESEBIG5_CHARSET     136 (x88)             950             3            4           Big5
					//  HANGEUL_CHARSET         129 (x81)             949             3            5           Wansung
					//  JOHAB_CHARSET	        130 (x82)            1361             3            6           Johab

					for( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
					{
						// Symbol
						if ( !( ulCodeRange1 & 0x80000000 ) && 0 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
							ulCodeRange1 |= 0x80000000;

						// ShiftJIS
						if ( !( ulCodeRange1 & 0x00020000 ) && 2 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
							ulCodeRange1 |= 0x00020000;

						// PRC
						if ( !( ulCodeRange1 & 0x00040000 ) && 3 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
							ulCodeRange1 |= 0x00040000;

						// Big5
						if ( !( ulCodeRange1 & 0x00100000 ) && 4 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
							ulCodeRange1 |= 0x00100000;

						// Wansung
						if ( !( ulCodeRange1 & 0x00080000 ) && 5 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
							ulCodeRange1 |= 0x00080000;

						// Johab
						if ( !( ulCodeRange1 & 0x00200000 ) && 6 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
							ulCodeRange1 |= 0x00200000;
					}
				}

				EFontFormat eFormat = GetFontFormat( pFace );

				CWinFontInfo *pWinFontInfo = new CWinFontInfo( CStringW( (char*)pFace->family_name ), CStringW( (char*)pFace->style_name ), wsFilePath.GetBuffer(), nIndex, bBold, bItalic, bFixedWidth, pPanose, ulRange1, ulRange2, ulRange3, ulRange4, ulCodeRange1, ulCodeRange2, usWeight, usWidth, sFamilyClass, eFormat, shAvgCharWidth, shAscent, shDescent, shLineGap, shXHeight, shCapHeight );

				Add( pWinFontInfo );

				FT_Done_Face( pFace );
			}

			wsPath[0] = _T('\0');
		}
	}

	CWinFontList(FT_Library pLibrary, CString sDir)
	{
		m_lDefIndex = -1;

		m_pFonts = new CList();
		m_wsWinFontDir[0] = '\0';

		WIN32_FIND_DATA oFD; 
		
		CString sSpec = sDir + _T("\\*.*");
		HANDLE hRes = FindFirstFile( sSpec, &oFD );
		if( INVALID_HANDLE_VALUE == hRes )
			return;
		do 
		{
			sSpec = oFD.cFileName;
			if( sSpec != _T(".") && sSpec != _T("..") )
			{
				sSpec = sDir + _T("\\") + sSpec;
				if( !( oFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) 
				{
					int nError = 0;
					FT_Face pFace = NULL;
					CStringW wsFilePath = sSpec;

					if ( nError = FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), 0, &pFace ) )
						continue;

					// TO DO: Шрифты, которые нельзя скейлить (т.е. изменять размер 
					// произвольно) мы не грузим. Возможно в будущем надо будет
					// сделать, чтобы работал и такой вариант. (в Word такие шрифты
					// не используются)
					if ( !( pFace->face_flags & FT_FACE_FLAG_SCALABLE ) )
					{
						FT_Done_Face( pFace );
						continue;
					}

					int nFacesCount = pFace->num_faces;
					if ( nError = FT_Done_Face( pFace ) )
					{
						continue;
					}

					for ( int nIndex = 0; nIndex < nFacesCount; nIndex++ )
					{
						if ( FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), nIndex, &pFace ) )
						{
							continue;
						}

						BOOL bBold   = ( pFace->style_flags & FT_STYLE_FLAG_BOLD ? 1 : 0 );
						BOOL bItalic = pFace->style_flags & FT_STYLE_FLAG_ITALIC;


						CStringW wsPostscriptName = CStringW( FT_Get_Postscript_Name( pFace ) );
						//if ( wsPostscriptName.GetLength() <= 0 )
						//	CStringW wsPostscriptName = CStringW( (char*)pFace->family_name );

						int nNamesCount = FT_Get_Sfnt_Name_Count( pFace );

						for ( int nNameIndex = 0; nNameIndex < nNamesCount; nNameIndex++ )
						{
							FT_SfntName_ oSfntName;
							FT_Get_Sfnt_Name( pFace, nNameIndex, &oSfntName );


							if ( oSfntName.language_id == 0x0409 && oSfntName.name_id == 1 && oSfntName.platform_id == 3 )
							{
								// Family
								int k= 10;
							}
							if ( oSfntName.language_id == 0x0409 && oSfntName.name_id == 16 && oSfntName.platform_id == 3 )
							{
								// Preffered family
								int k= 10;
							}
						}

						BOOL bFixedWidth = FT_IS_FIXED_WIDTH( pFace );

						TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );

						BYTE *pPanose = NULL;
						ULONG ulRange1 = 0, ulRange2 = 0, ulRange3 = 0, ulRange4 = 0, ulCodeRange1 = 0, ulCodeRange2 = 0;
						USHORT usWidth = 0, usWeight = 0;
						SHORT sFamilyClass = 0;

						SHORT shAvgCharWidth = 0, shAscent = 0, shDescent = 0, shLineGap = 0, shXHeight = 0, shCapHeight = 0;
						if ( NULL != pOs2 )
						{
							pPanose        = (BYTE *)pOs2->panose;

							ulRange1       = pOs2->ulUnicodeRange1;
							ulRange2       = pOs2->ulUnicodeRange2;
							ulRange3       = pOs2->ulUnicodeRange3;
							ulRange4       = pOs2->ulUnicodeRange4;
							ulCodeRange1   = pOs2->ulCodePageRange1;
							ulCodeRange2   = pOs2->ulCodePageRange2;

							usWeight       = pOs2->usWeightClass;
							usWidth        = pOs2->usWidthClass;

							sFamilyClass   = pOs2->sFamilyClass;

							if ( 0 != pFace->units_per_EM )
							{
								double dKoef = ( 1000 / (double)pFace->units_per_EM );
								shAvgCharWidth = (SHORT)(pOs2->xAvgCharWidth  * dKoef);
								shAscent       = (SHORT)(pOs2->sTypoAscender  * dKoef);
								shDescent      = (SHORT)(pOs2->sTypoDescender * dKoef);
								shLineGap      = (SHORT)(pOs2->sTypoLineGap   * dKoef);
								shXHeight      = (SHORT)(pOs2->sxHeight       * dKoef);
								shCapHeight    = (SHORT)(pOs2->sCapHeight     * dKoef);
							}
							else
							{
								shAvgCharWidth = (SHORT)pOs2->xAvgCharWidth;
								shAscent       = (SHORT)pOs2->sTypoAscender;
								shDescent      = (SHORT)pOs2->sTypoDescender;
								shLineGap      = (SHORT)pOs2->sTypoLineGap;
								shXHeight      = (SHORT)pOs2->sxHeight;
								shCapHeight    = (SHORT)pOs2->sCapHeight;
							}
						}

						if ( true )
						{
							// Специальная ветка для случаев, когда charset может быть задан не через значения
							// ulCodePageRange, а непосредственно через тип Cmap.

							//  Charset Name       Charset Value(hex)  Codepage number   Platform_ID   Encoding_ID   Description
							//  -------------------------------------------------------------------------------------------------
							//
							//  SYMBOL_CHARSET            2 (x02)                             3            0           Symbol
							//  SHIFTJIS_CHARSET        128 (x80)             932             3            2           ShiftJIS
							//  GB2313_CHARSET          134 (x86)             936             3            3           PRC
							//  CHINESEBIG5_CHARSET     136 (x88)             950             3            4           Big5
							//  HANGEUL_CHARSET         129 (x81)             949             3            5           Wansung
							//  JOHAB_CHARSET	        130 (x82)            1361             3            6           Johab

							for( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
							{
								// Symbol
								if ( !( ulCodeRange1 & 0x80000000 ) && 0 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
									ulCodeRange1 |= 0x80000000;

								// ShiftJIS
								if ( !( ulCodeRange1 & 0x00020000 ) && 2 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
									ulCodeRange1 |= 0x00020000;

								// PRC
								if ( !( ulCodeRange1 & 0x00040000 ) && 3 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
									ulCodeRange1 |= 0x00040000;

								// Big5
								if ( !( ulCodeRange1 & 0x00100000 ) && 4 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
									ulCodeRange1 |= 0x00100000;

								// Wansung
								if ( !( ulCodeRange1 & 0x00080000 ) && 5 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
									ulCodeRange1 |= 0x00080000;

								// Johab
								if ( !( ulCodeRange1 & 0x00200000 ) && 6 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
									ulCodeRange1 |= 0x00200000;
							}
						}

						EFontFormat eFormat = GetFontFormat( pFace );

						CWinFontInfo *pWinFontInfo = new CWinFontInfo( CStringW( (char*)pFace->family_name ), CStringW( (char*)pFace->style_name ), wsFilePath.GetBuffer(), nIndex, bBold, bItalic, bFixedWidth, pPanose, ulRange1, ulRange2, ulRange3, ulRange4, ulCodeRange1, ulCodeRange2, usWeight, usWidth, sFamilyClass, eFormat, shAvgCharWidth, shAscent, shDescent, shLineGap, shXHeight, shCapHeight );

						Add( pWinFontInfo );

						FT_Done_Face( pFace );
					}
				}
			}
		} while( FindNextFile( hRes, &oFD ) );		
		FindClose( hRes );
	}
	CWinFontList(CString sWinFontsXml) 
	{
		m_lDefIndex = -1;
	
		m_pFonts = new CList();
		m_wsWinFontDir[0] = '\0';

		XmlUtils::CXmlNode oMainNode;
		oMainNode.FromXmlString( sWinFontsXml );

		if ( _T("WinFontList") == oMainNode.GetName() )
		{
			XmlUtils::CXmlNodes oFonts;
			oMainNode.GetNodes( _T("WinFontEntry"), oFonts );

			for ( int nIndex = 0; nIndex < oFonts.GetCount(); nIndex++ )
			{
				XmlUtils::CXmlNode oFontNode;
				if ( oFonts.GetAt( nIndex, oFontNode ) )
				{
					CWinFontInfo *pWinFontInfo = CWinFontInfo::FromXml( oFontNode );
					if ( NULL != pWinFontInfo )
						Add( pWinFontInfo );
				}
			}
		}
	}
	CWinFontList(BYTE* pBuffer, CString sFontDir) 
	{
		m_lDefIndex = -1;

		m_pFonts = new CList();
		m_wsWinFontDir[0] = '\0';

		LONG lCount = *((LONG*)pBuffer);
		pBuffer += g_lSizeofLONG;

		for (LONG nIndex = 0; nIndex < lCount; ++nIndex)
		{
			CWinFontInfo *pWinFontInfo = CWinFontInfo::FromBuffer( pBuffer, sFontDir );
			Add(pWinFontInfo);
		}
	}

	~CWinFontList() 
	{
		DeleteCList( m_pFonts, CWinFontInfo );
	}


	CList *GetFonts()
	{
		return m_pFonts;
	}
	CWinFontInfo *GetByIndex(int nIndex)
	{
		return (CWinFontInfo *)m_pFonts->GetByIndex( nIndex );
	}


	CWinFontInfo *GetByParams(const CString& sParamsXml)
	{
		if ( m_pFonts->GetLength() <= 0 )
			return NULL;

		//********************************************
		/*
		CString sXml = _T("<FontProperties>\
							<Name value='Tahoma'/>\
							<AltName value='Helvetica'/>\
							<Charset value='EE'/>\
							<FamilyClass name='swiss'/>\
							<Style bold='1' italic='0'/>\
							<FixedWidth value='0'/>\
							<Panose value='001F2A3B4C5D6F738491'/>\
							<UnicodeRange range1='F7FFAFFF' range2='E9DFFFFF' range3='0000003F' range4='00000000' coderange1='003F01FF' coderange2='00000000'/>\
							<Weight value='550'/>\
							<Width value='1000'/>\
							<FontFormat value='1'/>\
							<AvgCharWidth value='267'/>\
							<Ascent value='850'/>\
							<Descent value='-109'/>\
							<LineGap value='217'/>\
							<xHeight value='745'/>\
							<CapHeight value='848'/>\
						</FontProperties>");
		*/
		//********************************************
		CString wsFamilyName, wsFamilyClass, wsAltName;
		unsigned char unCharset = 0;
		BOOL bBold = FALSE, bItalic = FALSE, bFixedWidth = FALSE;
		BYTE pPanose[10];
		ULONG ulRange1 = 0, ulRange2 = 0, ulRange3 = 0, ulRange4 = 0, ulCodeRange1 = 0, ulCodeRange2 = 0;
		USHORT usWeight = 0, usWidth = 0;
		SHORT sFamilyClass = 0;
		EFontFormat eFontFormat = fontTrueType;

		SHORT shAvgCharWidth = 0, shAscent = 0, shDescent = 0, shLineGap = 0, shXHeight = 0, shCapHeight = 0;

		BOOL bIsName = FALSE, bIsAltName = FALSE, bIsStyle = FALSE, bIsFixed = FALSE, bIsPanose = FALSE, bIsRanges = FALSE, bIsWeight = FALSE, bIsWidth = FALSE, bIsFamilyClass = FALSE;

		BOOL bIsAvgWidth = FALSE, bIsAscent = FALSE, bIsDescent = FALSE, bIsLineGap = FALSE, bIsXHeight = FALSE, bIsCapHeight = FALSE;
		// Считываем настройки шрифта
		XmlUtils::CXmlNode oMainNode;
		oMainNode.FromXmlString( sParamsXml );

		if ( _T("FontProperties") == oMainNode.GetName() )
		{
			XmlUtils::CXmlNode oNode;
			
			if ( oMainNode.GetNode( _T("Name"), oNode ) )
			{
				bIsName = TRUE;

				wsFamilyName = oNode.GetAttribute( _T("value") );
			}

			if ( oMainNode.GetNode( _T("AltName"), oNode ) )
			{
				bIsAltName = TRUE;

				wsAltName = oNode.GetAttribute( _T("value") );

				if ( wsAltName.GetLength() <= 0 )
					bIsAltName = FALSE;
			}

			if ( oMainNode.GetNode( _T("Style"), oNode ) )
			{
				bIsStyle = TRUE;

				bBold   = (BOOL)XmlUtils::GetInteger( oNode.GetAttribute( _T("bold") ) );
				bItalic = (BOOL)XmlUtils::GetInteger( oNode.GetAttribute( _T("italic") ) );
			}

			if ( oMainNode.GetNode( _T("FixedWidth"), oNode ) )
			{
				bIsFixed = TRUE;

				bFixedWidth = (BOOL)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );
			}

			if ( oMainNode.GetNode( _T("Panose"), oNode ) )
			{
				CString sValue = oNode.GetAttribute( _T("value") );
				if ( 20 == sValue.GetLength() )
				{
					for ( int nIndex = 0; nIndex < 10; nIndex++ )
					{
						int nChar1 = sValue.GetAt( nIndex * 2 );
						int nChar2 = sValue.GetAt( nIndex * 2 + 1 );

						unsigned char unValue = ((unsigned char)( HexToInt( nChar1 ) << 4 )) | HexToInt( nChar2 );
						pPanose[nIndex] = unValue;

						// Если хоть одно значение не 0, то используем Panose
						if ( 0 != unValue  )
							bIsPanose = TRUE;
					}
				}
			}

			if ( oMainNode.GetNode( _T("UnicodeRange"), oNode ) )
			{
				ulRange1 = (ULONG)HexToInt( oNode.GetAttribute( _T("range1") ) );
				ulRange2 = (ULONG)HexToInt( oNode.GetAttribute( _T("range2") ) );
				ulRange3 = (ULONG)HexToInt( oNode.GetAttribute( _T("range3") ) );
				ulRange4 = (ULONG)HexToInt( oNode.GetAttribute( _T("range4") ) );

				ulCodeRange1 = (ULONG)HexToInt( oNode.GetAttribute( _T("coderange1") ) );
				ulCodeRange2 = (ULONG)HexToInt( oNode.GetAttribute( _T("coderange2") ) );

				if ( !( 0 == ulRange1 && 0 == ulRange2 && 0 == ulRange3 && 0 == ulRange4 && 0 == ulCodeRange1 && 0 == ulCodeRange2 ) )
					bIsRanges = TRUE;
			}

			if ( oMainNode.GetNode( _T("Weight"), oNode ) )
			{
				usWeight = (USHORT)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );

				if ( 0 != usWeight )
					bIsWeight = TRUE;
			}

			if ( oMainNode.GetNode( _T("Width"), oNode ) )
			{
				bIsWidth = TRUE;

				usWidth = (USHORT)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );
			}

			if ( oMainNode.GetNode( _T("FamilyClass"), oNode ) )
			{
				bIsFamilyClass = TRUE;

				wsFamilyClass = oNode.GetAttribute( _T("type"), _T("") );
				sFamilyClass  = (SHORT)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );
			}

			if ( oMainNode.GetNode( _T("FontFormat"), oNode ) )
			{
				// Формат мы всегда сравниваем
				eFontFormat = (EFontFormat)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );
			}

			if ( oMainNode.GetNode( _T("Charset"), oNode ) )
			{
				// Если charset не задан, то считаем его нулевым (Cp-1252)
				unCharset = (unsigned char)HexToInt( oNode.GetAttribute( _T("value") ) );
			}
			else 
				unCharset = UNKNOWN_CHARSET;

			if ( oMainNode.GetNode( _T("AvgCharWidth"), oNode ) )
			{
				bIsAvgWidth = TRUE;

				shAvgCharWidth = (SHORT)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );
			}

			if ( oMainNode.GetNode( _T("Ascent"), oNode ) )
			{
				bIsAscent = TRUE;

				shAscent = (SHORT)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );
			}
			if ( oMainNode.GetNode( _T("Descent"), oNode ) )
			{
				bIsDescent = TRUE;

				shDescent = (SHORT)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );
			}
			if ( oMainNode.GetNode( _T("LineGap"), oNode ) )
			{
				bIsLineGap = TRUE;

				shLineGap = (SHORT)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );
			}
			if ( oMainNode.GetNode( _T("xHeight"), oNode ) )
			{
				bIsXHeight = TRUE;

				shXHeight = (SHORT)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );
			}
			if ( oMainNode.GetNode( _T("CapHeight"), oNode ) )
			{
				bIsCapHeight = TRUE;

				shCapHeight = (SHORT)XmlUtils::GetInteger( oNode.GetAttribute( _T("value") ) );
			}
		}

		bool bIsCorrectFromDict = NSFontDictionary::CorrectParamsFromDictionary(wsFamilyName, bBold, bItalic,
			bFixedWidth, pPanose, ulRange1, ulRange2, ulRange3, ulRange4, ulCodeRange1, ulCodeRange2, usWeight, usWidth,
			sFamilyClass, shAvgCharWidth, shAscent, shDescent, shLineGap, shXHeight, shCapHeight, bIsStyle,
			bIsFixed, bIsPanose, bIsRanges, bIsWeight, bIsWidth, bIsFamilyClass, bIsAvgWidth, bIsAscent, bIsDescent, bIsLineGap, bIsXHeight, bIsCapHeight);

		int nMinIndex   = 0; // Номер шрифта в списке с минимальным весом
		int nMinPenalty = 0; // Минимальный вес

		int nDefPenalty = INT_MAX;

		for ( int nIndex = 0; nIndex < m_pFonts->GetLength(); nIndex++ )
		{
			int nCurPenalty = 0;
			CWinFontInfo *pInfo = (CWinFontInfo *)m_pFonts->GetByIndex( nIndex );

			if ( bIsPanose )
			{
				nCurPenalty += GetPanosePenalty( pInfo->m_aPanose, pPanose );
			}

			ULONG arrCandRanges[6] = { pInfo->m_ulUnicodeRange1, pInfo->m_ulUnicodeRange2, pInfo->m_ulUnicodeRange3, pInfo->m_ulUnicodeRange4, pInfo->m_ulCodePageRange1, pInfo->m_ulCodePageRange2 };
			if ( bIsRanges && bIsCorrectFromDict )
			{
				ULONG arrReqRanges[6]  = { ulRange1, ulRange2, ulRange3, ulRange4, ulCodeRange1, ulCodeRange2 };

				nCurPenalty += GetSigPenalty( arrCandRanges, arrReqRanges, 1000, true );
			}

			if ( bIsFixed )
				nCurPenalty += GetFixedPitchPenalty( pInfo->m_bIsFixed, bFixedWidth );

			if ( bIsAltName && bIsName )
			{
				nCurPenalty += min( GetFaceNamePenalty( pInfo->m_wsFontName, wsFamilyName ), GetFaceNamePenalty( pInfo->m_wsFontName, wsAltName ) );
			}
			else if ( bIsName )
				nCurPenalty += GetFaceNamePenalty( pInfo->m_wsFontName, wsFamilyName );
			else if ( bIsAltName )
				nCurPenalty += GetFaceNamePenalty( pInfo->m_wsFontName, wsAltName );

			if ( bIsWidth )
				nCurPenalty += GetWidthPenalty( pInfo->m_usWidth, usWidth );

			if ( bIsWeight )
				nCurPenalty += GetWeightPenalty( pInfo->m_usWeigth, usWeight );

			if ( bIsStyle )
			{
				nCurPenalty += GetItalicPenalty( pInfo->m_bItalic, bItalic );
				nCurPenalty += GetBoldPenalty( pInfo->m_bBold, bBold );
			}

			if ( bIsFamilyClass )
			{
				if ( _T("") != wsFamilyClass )
					nCurPenalty += GetFamilyUnlikelyPenalty( pInfo->m_sFamilyClass, wsFamilyClass );
				else
					nCurPenalty += GetFamilyUnlikelyPenalty( pInfo->m_sFamilyClass, sFamilyClass );
			}

			nCurPenalty += GetFontFormatPenalty( pInfo->m_eFontFormat, eFontFormat );
			nCurPenalty += GetCharsetPenalty( arrCandRanges, unCharset );


			if ( bIsAvgWidth )
				nCurPenalty += GetAvgWidthPenalty( pInfo->m_shAvgCharWidth, shAvgCharWidth );

			if ( bIsAscent )
				nCurPenalty += GetAscentPenalty( pInfo->m_shAscent, shAscent );

			if ( bIsDescent )
				nCurPenalty += GetDescentPenalty( pInfo->m_shDescent, shDescent );

			if ( bIsLineGap )
				nCurPenalty += GetLineGapPenalty( pInfo->m_shLineGap, shLineGap );

			if ( bIsXHeight )
				nCurPenalty += GetXHeightPenalty( pInfo->m_shXHeight, shXHeight );

			if ( bIsCapHeight )
				nCurPenalty += GetCapHeightPenalty( pInfo->m_shCapHeight, shCapHeight );

			if ( 0 == nIndex )
			{
				nMinIndex   = 0;
				nMinPenalty = nCurPenalty;
			}
			else if ( nCurPenalty < nMinPenalty )
			{
				nMinIndex   = nIndex;
				nMinPenalty = nCurPenalty;
			}

			if ( -1 != m_lDefIndex && nIndex == m_lDefIndex )
			{
				nDefPenalty = nCurPenalty;
			}

			// Нашелся шрифт, удовлетворяющий всем параметрам, дальше искать нет смысла
			if ( 0 == nCurPenalty )
				break;
		}

		if ( -1 != m_lDefIndex && nDefPenalty == nMinPenalty )
			nMinIndex = m_lDefIndex;

		return (CWinFontInfo *)m_pFonts->GetByIndex( nMinIndex );
	}
	CWinFontInfo *GetByParams(wchar_t* wsAltFontName, long lCharset, wchar_t* wsFamily, long lStyle, long lFixed, wchar_t *wsPanose, long lUnicodeRange1, long lUnicodeRange2, long lUnicodeRange3, long lUnicodeRange4, long lCodePageRange1, long lCodePageRange2, long lAvgWidth)
	{
		if ( m_pFonts->GetLength() <= 0 )
			return NULL;

		CString wsFamilyName, wsFamilyClass;
		unsigned char unCharset = 0;
		BOOL bBold = FALSE, bItalic = FALSE, bFixedWidth = FALSE;
		BYTE pPanose[10];
		ULONG ulRange1 = 0, ulRange2 = 0, ulRange3 = 0, ulRange4 = 0, ulCodeRange1 = 0, ulCodeRange2 = 0;
		SHORT shAvgCharWidth = 0;

		BOOL bIsName = FALSE, bIsStyle = FALSE, bIsFixed = FALSE, bIsPanose = FALSE, bIsRanges = FALSE, bIsFamilyClass = FALSE, bIsAvgWidth = FALSE;

		if ( NULL != wsAltFontName )
		{
			bIsName = TRUE;
			wsFamilyName = CString( wsAltFontName );
		}

		if ( lStyle >= 0 )
		{
			bIsStyle = TRUE;

			bBold   = (lStyle & FontConstants::FontStyleBold   ? TRUE : FALSE);
			bItalic = (lStyle & FontConstants::FontStyleItalic ? TRUE : FALSE);
		}

		if ( lFixed >= 0 )
		{
			bIsFixed = TRUE;
			bFixedWidth = ( lFixed > 0 ? TRUE : FALSE );
		}

		if ( NULL != wsPanose )
		{
			if ( 20 == wcslen( wsPanose ) )
			{
				for ( int nIndex = 0; nIndex < 10; nIndex++ )
				{
					int nChar1 = wsPanose[nIndex * 2 ];
					int nChar2 = wsPanose[nIndex * 2 + 1 ];

					unsigned char unValue = ((unsigned char)( HexToInt( nChar1 ) << 4 )) | HexToInt( nChar2 );
					pPanose[nIndex] = unValue;

					// Если хоть одно значение не 0, то используем Panose
					if ( 0 != unValue  )
						bIsPanose = TRUE;
				}
			}
		}

		ulRange1 = (ULONG)lUnicodeRange1;
		ulRange2 = (ULONG)lUnicodeRange2;
		ulRange3 = (ULONG)lUnicodeRange3;
		ulRange4 = (ULONG)lUnicodeRange4;

		ulCodeRange1 = (ULONG)lCodePageRange1;
		ulCodeRange2 = (ULONG)lCodePageRange2;

		if ( !( 0 == ulRange1 && 0 == ulRange2 && 0 == ulRange3 && 0 == ulRange4 && 0 == ulCodeRange1 && 0 == ulCodeRange2 ) )
			bIsRanges = TRUE;

		if ( NULL != wsFamily )
		{
			bIsFamilyClass = TRUE;

			wsFamilyClass = CString(wsFamily);
		}


		if ( lCharset >= 0 )
			unCharset = (unsigned char)lCharset;
		else
			unCharset = UNKNOWN_CHARSET;

		if ( lAvgWidth >= 0 )
		{
			bIsAvgWidth = TRUE;

			shAvgCharWidth = (SHORT)lAvgWidth;
		}


		int nMinIndex   = 0; // Номер шрифта в списке с минимальным весом
		int nMinPenalty = 0; // Минимальный вес

		int nDefPenalty = 0;

		for ( int nIndex = 0; nIndex < m_pFonts->GetLength(); nIndex++ )
		{
			int nCurPenalty = 0;
			CWinFontInfo *pInfo = (CWinFontInfo *)m_pFonts->GetByIndex( nIndex );

			if ( bIsPanose )
			{
				nCurPenalty += GetPanosePenalty( pInfo->m_aPanose, pPanose );
			}

			ULONG arrCandRanges[6] = { pInfo->m_ulUnicodeRange1, pInfo->m_ulUnicodeRange2, pInfo->m_ulUnicodeRange3, pInfo->m_ulUnicodeRange4, pInfo->m_ulCodePageRange1, pInfo->m_ulCodePageRange2 };
			if ( bIsRanges )
			{
				ULONG arrReqRanges[6]  = { ulRange1, ulRange2, ulRange3, ulRange4, ulCodeRange1, ulCodeRange2 };

				nCurPenalty += GetSigPenalty( arrCandRanges, arrReqRanges, 10000 );
			}

			if ( bIsFixed )
				nCurPenalty += GetFixedPitchPenalty( pInfo->m_bIsFixed, bFixedWidth );

			if ( bIsName )
				nCurPenalty += GetFaceNamePenalty( pInfo->m_wsFontName, wsFamilyName );

			if ( bIsStyle )
			{
				nCurPenalty += GetItalicPenalty( pInfo->m_bItalic, bItalic );
				nCurPenalty += GetBoldPenalty( pInfo->m_bBold, bBold );
			}

			if ( bIsFamilyClass )
				nCurPenalty += GetFamilyUnlikelyPenalty( pInfo->m_sFamilyClass, wsFamilyClass );

			nCurPenalty += GetCharsetPenalty( arrCandRanges, unCharset );


			if ( bIsAvgWidth )
				nCurPenalty += GetAvgWidthPenalty( pInfo->m_shAvgCharWidth, shAvgCharWidth );


			if ( 0 == nIndex )
			{
				nMinIndex   = 0;
				nMinPenalty = nCurPenalty;
			}
			else if ( nCurPenalty < nMinPenalty )
			{
				nMinIndex   = nIndex;
				nMinPenalty = nCurPenalty;
			}

			if ( -1 != m_lDefIndex && nIndex == m_lDefIndex )
			{
				nDefPenalty = nCurPenalty;
			}

			// Нашелся шрифт, удовлетворяющий всем параметрам, дальше искать нет смысла
			if ( 0 == nCurPenalty )
				break;
		}

		if ( -1 != m_lDefIndex && nDefPenalty == nMinPenalty )
			nMinIndex = m_lDefIndex;

		return (CWinFontInfo *)m_pFonts->GetByIndex( nMinIndex );
	}
	void SetDefaultFont(long lIndex)
	{
		if ( lIndex >= 0 && lIndex < m_pFonts->GetLength() ) 
			m_lDefIndex = lIndex;
	}
	static CString GetWinFontsXml(FT_Library pLibrary)
	{
		XmlUtils::CXmlWriter oWriter;
		
		oWriter.WriteNodeBegin( _T("WinFontList") );
		

		// Ищем директорию с фонтами (обычно это C:\Windows\Fonts)
		wchar_t wsWinFontDir[MAX_PATH];

		wsWinFontDir[0] = '\0';
		if ( !SHGetSpecialFolderPathW( NULL, wsWinFontDir, CSIDL_FONTS, FALSE ) )
			wsWinFontDir[0] = '\0';

		OSVERSIONINFO oVersion;
		oVersion.dwOSVersionInfoSize = sizeof(oVersion);
		GetVersionEx( &oVersion );

		wchar_t wsPath[1000];
		if ( oVersion.dwPlatformId == VER_PLATFORM_WIN32_NT ) 
			_tcscpy_s( wsPath, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts\\") );
		else 
			_tcscpy_s( wsPath, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Fonts\\") );

		TCHAR szName[2 * MAX_PATH];
		TCHAR szData[2 * MAX_PATH];
		while ( GetNextNameValue( HKEY_LOCAL_MACHINE, wsPath, szName, szData ) == ERROR_SUCCESS) 
		{
			int nError = 0;
			FT_Face pFace = NULL;
			CStringW wsFilePath( szData );

			if ( nError = FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), 0, &pFace ) )
			{
				wsFilePath = CStringW( wsWinFontDir ) + CStringW( _T("\\") ) + CStringW( szData );
				wsFilePath.Replace( _T("\0"), _T("") );
				if ( nError = FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), 0, &pFace ) )
				{
					wsPath[0] = _T('\0');
					continue;
				}
			}

			// TO DO: Шрифты, которые нельзя скейлить (т.е. изменять размер 
			// произвольно) мы не грузим. Возможно в будущем надо будет
			// сделать, чтобы работал и такой вариант. (в Word такие шрифты
			// не используются)
			if ( !( pFace->face_flags & FT_FACE_FLAG_SCALABLE ) )
			{
				FT_Done_Face( pFace );
				wsPath[0] = _T('\0');
				continue;
			}

			int nFacesCount = pFace->num_faces;
			if ( nError = FT_Done_Face( pFace ) )
			{
				wsPath[0] = _T('\0');
				continue;
			}

			for ( int nIndex = 0; nIndex < nFacesCount; nIndex++ )
			{
				if ( FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), nIndex, &pFace ) )
				{
					wsPath[0] = _T('\0');
					continue;
				}

				BOOL bBold   = ( pFace->style_flags & FT_STYLE_FLAG_BOLD ? 1 : 0 );
				BOOL bItalic = pFace->style_flags & FT_STYLE_FLAG_ITALIC;

				
				CStringW wsPostscriptName = CStringW( FT_Get_Postscript_Name( pFace ) );
				//if ( wsPostscriptName.GetLength() <= 0 )
				//	CStringW wsPostscriptName = CStringW( (char*)pFace->family_name );

				int nNamesCount = FT_Get_Sfnt_Name_Count( pFace );
				
				for ( int nNameIndex = 0; nNameIndex < nNamesCount; nNameIndex++ )
				{
					FT_SfntName_ oSfntName;
					FT_Get_Sfnt_Name( pFace, nNameIndex, &oSfntName );


					if ( oSfntName.language_id == 0x0409 && oSfntName.name_id == 1 && oSfntName.platform_id == 3 )
					{
						// Family
						int k= 10;
					}
					if ( oSfntName.language_id == 0x0409 && oSfntName.name_id == 16 && oSfntName.platform_id == 3 )
					{
						// Preffered family
						int k= 10;
					}
				}

				BOOL bFixedWidth = FT_IS_FIXED_WIDTH( pFace );

				TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );
				
				BYTE *pPanose = NULL;
				ULONG ulRange1 = 0, ulRange2 = 0, ulRange3 = 0, ulRange4 = 0, ulCodeRange1 = 0, ulCodeRange2 = 0;
				USHORT usWidth = 0, usWeight = 0;
				SHORT sFamilyClass = 0;
				SHORT shAvgCharWidth = 0, shAscent = 0, shDescent = 0, shLineGap = 0, shXHeight = 0, shCapHeight = 0;
				if ( NULL != pOs2 )
				{
					pPanose        = (BYTE *)pOs2->panose;

					ulRange1       = pOs2->ulUnicodeRange1;
					ulRange2       = pOs2->ulUnicodeRange2;
					ulRange3       = pOs2->ulUnicodeRange3;
					ulRange4       = pOs2->ulUnicodeRange4;
					ulCodeRange1   = pOs2->ulCodePageRange1;
					ulCodeRange2   = pOs2->ulCodePageRange2;

					usWeight       = pOs2->usWeightClass;
					usWidth        = pOs2->usWidthClass;

					sFamilyClass   = pOs2->sFamilyClass;

					if ( 0 != pFace->units_per_EM )
					{
						double dKoef = ( 1000 / (double)pFace->units_per_EM );
						shAvgCharWidth = (SHORT)(pOs2->xAvgCharWidth  * dKoef);
						shAscent       = (SHORT)(pOs2->sTypoAscender  * dKoef);
						shDescent      = (SHORT)(pOs2->sTypoDescender * dKoef);
						shLineGap      = (SHORT)(pOs2->sTypoLineGap   * dKoef);
						shXHeight      = (SHORT)(pOs2->sxHeight       * dKoef);
						shCapHeight    = (SHORT)(pOs2->sCapHeight     * dKoef);
					}
					else
					{
						shAvgCharWidth = (SHORT)pOs2->xAvgCharWidth;
						shAscent       = (SHORT)pOs2->sTypoAscender;
						shDescent      = (SHORT)pOs2->sTypoDescender;
						shLineGap      = (SHORT)pOs2->sTypoLineGap;
						shXHeight      = (SHORT)pOs2->sxHeight;
						shCapHeight    = (SHORT)pOs2->sCapHeight;
					}
				}

				EFontFormat eFormat = GetFontFormat( pFace );

				CWinFontInfo oWinFontInfo( CStringW( (char*)pFace->family_name ), CStringW( (char*)pFace->style_name ), wsFilePath.GetBuffer(), nIndex, bBold, bItalic, bFixedWidth, pPanose, ulRange1, ulRange2, ulRange3, ulRange4, ulCodeRange1, ulCodeRange2, usWeight, usWidth, sFamilyClass, eFormat, shAvgCharWidth, shAscent, shDescent, shLineGap, shXHeight, shCapHeight );
				oWinFontInfo.WriteToXml( &oWriter );

				FT_Done_Face( pFace );
			}

			wsPath[0] = _T('\0');
		}

		oWriter.WriteNodeEnd( _T("WinFontList") );

		return oWriter.GetXmlString();
	}

	static BYTE* GetWinFontsData(FT_Library pLibrary, LONG& lDataSize)
	{
		// Ищем директорию с фонтами (обычно это C:\Windows\Fonts)
		wchar_t wsWinFontDir[MAX_PATH];

		CAtlArray<CWinFontInfo*> arrInfos;

		wsWinFontDir[0] = '\0';
		if ( !SHGetSpecialFolderPathW( NULL, wsWinFontDir, CSIDL_FONTS, FALSE ) )
			wsWinFontDir[0] = '\0';

		OSVERSIONINFO oVersion;
		oVersion.dwOSVersionInfoSize = sizeof(oVersion);
		GetVersionEx( &oVersion );

		wchar_t wsPath[1000];
		if ( oVersion.dwPlatformId == VER_PLATFORM_WIN32_NT ) 
			_tcscpy_s( wsPath, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts\\") );
		else 
			_tcscpy_s( wsPath, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Fonts\\") );

		TCHAR szName[2 * MAX_PATH];
		TCHAR szData[2 * MAX_PATH];
		while ( GetNextNameValue( HKEY_LOCAL_MACHINE, wsPath, szName, szData ) == ERROR_SUCCESS) 
		{
			int nError = 0;
			FT_Face pFace = NULL;
			CStringW wsFilePath( szData );

			if ( nError = FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), 0, &pFace ) )
			{
				wsFilePath = CStringW( wsWinFontDir ) + CStringW( _T("\\") ) + CStringW( szData );
				wsFilePath.Replace( _T("\0"), _T("") );
				if ( nError = FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), 0, &pFace ) )
				{
					wsPath[0] = _T('\0');
					continue;
				}
			}

			// TO DO: Шрифты, которые нельзя скейлить (т.е. изменять размер 
			// произвольно) мы не грузим. Возможно в будущем надо будет
			// сделать, чтобы работал и такой вариант. (в Word такие шрифты
			// не используются)
			if ( !( pFace->face_flags & FT_FACE_FLAG_SCALABLE ) )
			{
				FT_Done_Face( pFace );
				wsPath[0] = _T('\0');
				continue;
			}

			int nFacesCount = pFace->num_faces;
			if ( nError = FT_Done_Face( pFace ) )
			{
				wsPath[0] = _T('\0');
				continue;
			}

			for ( int nIndex = 0; nIndex < nFacesCount; nIndex++ )
			{
				if ( FT_New_FaceW( pLibrary, wsFilePath.GetBuffer(), nIndex, &pFace ) )
				{
					wsPath[0] = _T('\0');
					continue;
				}

				BOOL bBold   = ( pFace->style_flags & FT_STYLE_FLAG_BOLD ? 1 : 0 );
				BOOL bItalic = pFace->style_flags & FT_STYLE_FLAG_ITALIC;

				
				CStringW wsPostscriptName = CStringW( FT_Get_Postscript_Name( pFace ) );
				//if ( wsPostscriptName.GetLength() <= 0 )
				//	CStringW wsPostscriptName = CStringW( (char*)pFace->family_name );

				int nNamesCount = FT_Get_Sfnt_Name_Count( pFace );
				
				for ( int nNameIndex = 0; nNameIndex < nNamesCount; nNameIndex++ )
				{
					FT_SfntName_ oSfntName;
					FT_Get_Sfnt_Name( pFace, nNameIndex, &oSfntName );


					if ( oSfntName.language_id == 0x0409 && oSfntName.name_id == 1 && oSfntName.platform_id == 3 )
					{
						// Family
						int k= 10;
					}
					if ( oSfntName.language_id == 0x0409 && oSfntName.name_id == 16 && oSfntName.platform_id == 3 )
					{
						// Preffered family
						int k= 10;
					}
				}


				BOOL bFixedWidth = FT_IS_FIXED_WIDTH( pFace );

				TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );
				
				BYTE *pPanose = NULL;
				ULONG ulRange1 = 0, ulRange2 = 0, ulRange3 = 0, ulRange4 = 0, ulCodeRange1 = 0, ulCodeRange2 = 0;
				USHORT usWidth = 0, usWeight = 0;
				SHORT sFamilyClass = 0;
				SHORT shAvgCharWidth = 0, shAscent = 0, shDescent = 0, shLineGap = 0, shXHeight = 0, shCapHeight = 0;
				if ( NULL != pOs2 )
				{
					pPanose        = (BYTE *)pOs2->panose;

					ulRange1       = pOs2->ulUnicodeRange1;
					ulRange2       = pOs2->ulUnicodeRange2;
					ulRange3       = pOs2->ulUnicodeRange3;
					ulRange4       = pOs2->ulUnicodeRange4;
					ulCodeRange1   = pOs2->ulCodePageRange1;
					ulCodeRange2   = pOs2->ulCodePageRange2;

					usWeight       = pOs2->usWeightClass;
					usWidth        = pOs2->usWidthClass;

					sFamilyClass   = pOs2->sFamilyClass;

					if ( 0 != pFace->units_per_EM )
					{
						double dKoef = ( 1000 / (double)pFace->units_per_EM );
						shAvgCharWidth = (SHORT)(pOs2->xAvgCharWidth  * dKoef);
						shAscent       = (SHORT)(pOs2->sTypoAscender  * dKoef);
						shDescent      = (SHORT)(pOs2->sTypoDescender * dKoef);
						shLineGap      = (SHORT)(pOs2->sTypoLineGap   * dKoef);
						shXHeight      = (SHORT)(pOs2->sxHeight       * dKoef);
						shCapHeight    = (SHORT)(pOs2->sCapHeight     * dKoef);
					}
					else
					{
						shAvgCharWidth = (SHORT)pOs2->xAvgCharWidth;
						shAscent       = (SHORT)pOs2->sTypoAscender;
						shDescent      = (SHORT)pOs2->sTypoDescender;
						shLineGap      = (SHORT)pOs2->sTypoLineGap;
						shXHeight      = (SHORT)pOs2->sxHeight;
						shCapHeight    = (SHORT)pOs2->sCapHeight;
					}

				}

				EFontFormat eFormat = GetFontFormat( pFace );

				CWinFontInfo* __pInfo = new CWinFontInfo( CStringW( (char*)pFace->family_name ), CStringW( (char*)pFace->style_name ), wsFilePath.GetBuffer(), nIndex, bBold, bItalic, bFixedWidth, pPanose, ulRange1, ulRange2, ulRange3, ulRange4, ulCodeRange1, ulCodeRange2, usWeight, usWidth, sFamilyClass, eFormat, shAvgCharWidth, shAscent, shDescent, shLineGap, shXHeight, shCapHeight );
				arrInfos.Add(__pInfo);

				FT_Done_Face( pFace );
			}

			wsPath[0] = _T('\0');
		}

		lDataSize = g_lSizeofLONG;
		for (size_t i = 0; i < arrInfos.GetCount(); ++i)
		{
			lDataSize += arrInfos[i]->GetBufferLen();
		}

		BYTE* pData = new BYTE[lDataSize];
		BYTE* pDataMem = pData;

		*(LONG*)pDataMem = (LONG)arrInfos.GetCount();
		pDataMem += g_lSizeofLONG;

		for (size_t i = 0; i < arrInfos.GetCount(); ++i)
		{
			CWinFontInfo* pInfo = arrInfos[i];
			
			pInfo->ToBuffer(pDataMem);
			delete pInfo;
		}

		arrInfos.RemoveAll();

		return pData;
	}

	void ToBuffer(BYTE** pDstData, LONG* pLen, CString strDirectory = _T(""))
	{
		LONG lDataSize = g_lSizeofLONG;
		size_t nFontsCount = (size_t)m_pFonts->GetLength();
		for (size_t i = 0; i < nFontsCount; ++i)
		{
			lDataSize += ((CWinFontInfo*)(m_pFonts->GetByIndex((int)i)))->GetBufferLen(strDirectory);
		}

		BYTE* pData = new BYTE[lDataSize];
		BYTE* pDataMem = pData;

		*(LONG*)pDataMem = (LONG)m_pFonts->GetLength();
		pDataMem += g_lSizeofLONG;

		for (size_t i = 0; i < nFontsCount; ++i)
		{
			CWinFontInfo* pInfo = ((CWinFontInfo*)(m_pFonts->GetByIndex((int)i)));
			pInfo->ToBuffer(pDataMem, strDirectory);
		}

		*pDstData = pData;
		*pLen = lDataSize;
	}

	static EFontFormat GetFontFormat(FT_Face pFace)
	{
		CString wsFormat( FT_Get_X11_Font_Format( pFace ) );

		if ( _T("Windows FNT") == wsFormat )
			return fontWindowsFNT;
		else if ( _T("TrueType") == wsFormat ) 
			return fontTrueType;
		else if ( _T("CFF") == wsFormat )
			return fontOpenType;

		return fontUnknown;
	}

private:

	void Add(CWinFontInfo *pFontInfo) 
	{
		for ( int nIndex = 0; nIndex < m_pFonts->GetLength(); ++nIndex ) 
		{
			if ( ((CWinFontInfo *)m_pFonts->GetByIndex(nIndex))->Equals( pFontInfo ) ) 
			{
				if ( pFontInfo )
					delete pFontInfo;
				return;
			}
		}
		m_pFonts->Append( pFontInfo );
	}

	unsigned char HexToInt(int nHex)
	{
		if ( nHex >= '0' && nHex <= '9' ) return (unsigned char)(nHex - '0');
		if ( nHex >= 'a' && nHex <= 'f' ) return (unsigned char)(nHex - 'a' + 10);
		if ( nHex >= 'A' && nHex <= 'F' ) return (unsigned char)(nHex - 'A' + 10);

		return 0;
	}


	__int64 HexToInt(CString sHex)
	{
		__int64 nResult = 0;
		__int64 nMult = 1;
		for ( int nIndex = sHex.GetLength() - 1; nIndex >= 0; nIndex-- )
		{
			nResult += HexToInt( sHex.GetAt( nIndex ) ) * nMult;
			nMult <<= 4;
		}

		return nResult;
	}
	int GetCharsetPenalty(ULONG ulCandRanges[6], unsigned char unReqCharset)
	{
		// Penalty = 65000 (это самый весомый параметр)

		if ( UNKNOWN_CHARSET == unReqCharset )
			return 0;

		unsigned long ulBit = 0;
		unsigned int unLongIndex = 0;
		GetCodePageByCharset( unReqCharset, &ulBit, &unLongIndex );

		int nMult = 1;
		for ( int nIndex = 0; nIndex < (int)ulBit; nIndex++ )
			nMult <<= 1;

		if ( !(ulCandRanges[unLongIndex] & nMult) )
			return 65000;

		return 0;
	}
	int GetSigPenalty(ULONG ulCandRanges[6], ULONG ulReqRanges[6], double dRangeWeight = 1, bool bPenaltyForSuperflouous = false)
	{
		double dPenalty = 0;

		// Для начала просматриваем сколько вообще различных пространств надо.
		// Исходя из их общего количества, находим вес 1 пропущеного пространства.

		unsigned char arrCandidate[192], arrRequest[192];
		memset( arrCandidate, 0x00, 192 );
		memset( arrRequest, 0x00, 192 );

		int nRangesCount = 0; // Количество необходимых пространств
		int nAddCount    = 0; // количество дополнительных(ненужных) пространств у кандидата

		for ( int nIndex = 0; nIndex < 6; nIndex++ )
		{
			for ( unsigned long nBitCount = 0, nBit = 1; nBitCount < 32; nBitCount++, nBit *= 2 )
			{
				BOOL bReqAdd = FALSE;

				if ( ulReqRanges[nIndex] & nBit )
				{
					arrRequest[ nIndex * 32 + nBitCount ] = 1;
					nRangesCount++;
					bReqAdd = TRUE;
				}

				if ( ulCandRanges[nIndex] & nBit )
				{
					arrCandidate[ nIndex * 32 + nBitCount ] = 1;
					if ( !bReqAdd )
						nAddCount++;
				}
			}
		}

		if ( 0 == nRangesCount )
			return 0;

		//double dRangeWeight = 1;//1000.0 / nRangesCount;

		for ( int nIndex = 0; nIndex < 192; nIndex++ )
		{
			if ( 1 == arrRequest[nIndex] && 0 == arrCandidate[nIndex] )
				dPenalty += dRangeWeight;
			else if ( bPenaltyForSuperflouous && 0 == arrRequest[nIndex] && 1 == arrCandidate[nIndex] )
				dPenalty += dRangeWeight; 
		}

		return (int)dPenalty;
	}
	int GetFixedPitchPenalty(BOOL bCandFixed, BOOL bReqFixed)
	{
		int nPenalty = 0;

		// Если запрашивается моноширинный, а кандидат не моноширинный, то вес 15000
		// Если запрашивается не моноширинный, а кандидат моноширинный, то вес 350
		if ( bReqFixed && !bCandFixed )
			nPenalty = 15000;
		if ( !bReqFixed && bCandFixed )
			nPenalty = 350;

		return nPenalty;
	}
	int GetFaceNamePenalty(CString sCandName, CString sReqName)
	{
		// На MSDN написано, что если имена не совпадают, то вес 10000.
		// Мы будем сравнивать сколько совпало символов у запрашиваемого
		// имени и с именем кандидата, без учета решистра, пробелов, запятых
		// и тире.

		sCandName.Remove(' '); sReqName.Remove(' ');
		sCandName.Remove(','); sReqName.Remove(',');
		sCandName.Remove('-'); sReqName.Remove('-');

		sCandName.MakeLower(); sReqName.MakeLower();

		if ( 0 == sReqName.GetLength() )
			return 0;

		if ( 0 == sCandName.GetLength() )
			return 10000;

		if ( sReqName == sCandName )
			return 0;
		else if ( -1 != sReqName.Find( sCandName ) || -1 != sCandName.Find( sReqName ) )
			return 1000;

		return 10000;
	}

	int GetFamilyUnlikelyPenalty(SHORT nCandFamilyClass, SHORT nReqFamilyClass)
	{
		// Requested a roman/modern/swiss family, but the candidate is 
		// decorative/script. Or requested decorative/script, and the 
		// candidate is roman/modern/swiss. Penalty = 50.

		int nReqClassID  = nReqFamilyClass  >> 8;
		int nCandClassID = nCandFamilyClass >> 8;

		if ( 0 == nReqClassID ) // Unknown
			return 0;

		if ( 0 == nCandClassID ) // Unknown
			return 50;

		if ( ( nReqClassID <= 8 && nCandClassID > 8 ) || ( nReqClassID > 8 && nCandClassID <= 8 ) )
			return 50;

		return 0;
	}
	int GetFamilyUnlikelyPenalty(int nCandFamilyClass, CString sReqFamilyClass)
	{
		// Requested a roman/modern/swiss family, but the candidate is 
		// decorative/script. Or requested decorative/script, and the 
		// candidate is roman/modern/swiss. Penalty = 50.

		int nCandClassID = nCandFamilyClass >> 8;

		sReqFamilyClass.MakeLower();
		if ( _T("any") == sReqFamilyClass || _T("unknown") == sReqFamilyClass )
			return 0;
		else if ( 0 == nCandClassID )
			return 50;
		else if ( ( ( _T("swiss") == sReqFamilyClass || _T("roman") == sReqFamilyClass || _T("modern") == sReqFamilyClass ) && nCandClassID > 8 ) || ( ( _T("decorative") == sReqFamilyClass || _T("script") == sReqFamilyClass ) && nCandClassID <= 8 ) )
			return 50;

		return 0;
	}
	int GetWidthPenalty(USHORT usCandWidth, USHORT usReqWidth)
	{
		// Penalty * width difference (Penalty = 50)

		return abs( (int)usCandWidth - (int)usReqWidth ) * 50;
	}
	int GetWeightPenalty(USHORT usCandWeight, USHORT usReqWeight)
	{
		// Penalty * ( weight difference / 10 )  (Penalty = 3)

		return (3 * ( abs( (int)usCandWeight - (int)usReqWeight ) / 10 ));
	}

	int GetItalicPenalty(BOOL bCandItalic, BOOL bReqItalic)
	{
		// Penalty = 4

		if ( bCandItalic != bReqItalic )
			return 4;

		return 0;
	}

	int GetBoldPenalty(BOOL bCandBold, BOOL bReqBold)
	{
		// SmallPenalty
		// Penalty = 1

		if ( bCandBold != bReqBold )
			return 1;

		return 0;
	}

	int GetFontFormatPenalty(EFontFormat eCandFormat, EFontFormat eReqFormat)
	{
		// Вообще, на МSDN написано только про TrueType. Но мы будем сравнивать
		// все типы форматов и при несовпадении даем вес = 4. Если формат не задан
		// то по умолчанию считаем его TrueType.

		if ( eReqFormat == fontUnknown )
		{
			// Считаем, что когда формат не известен, значит это 100% не TrueType.
			if ( eCandFormat == fontTrueType )
				return 4;
			else
				return 0;
		}

		if ( eCandFormat != eReqFormat )
			return 4;

		return 0;
	}
	int GetPanosePenalty(BYTE *pCandPanose, BYTE *pReqPanose)
	{
		int nPenalty = 0;
		for ( int nIndex = 0; nIndex < 10; nIndex++ )
		{
			if ( pCandPanose[nIndex] != pReqPanose[nIndex] && 0 != pReqPanose[nIndex] )
			{
				int nKoef = abs(pCandPanose[nIndex] - pReqPanose[nIndex]);
				switch(nIndex)
				{
				case 0: nPenalty += 1000 * nKoef; break;
				case 1: nPenalty += 100  * nKoef; break;
				case 2: nPenalty += 100  * nKoef; break;
				case 3: nPenalty += 100  * nKoef; break;
				case 4: nPenalty += 100  * nKoef; break;
				case 5: nPenalty += 100  * nKoef; break;
				case 6: nPenalty += 100  * nKoef; break;
				case 7: nPenalty += 100  * nKoef; break;
				case 8: nPenalty += 100  * nKoef; break;
				case 9: nPenalty += 100  * nKoef; break;
				}
			}
		}

		return nPenalty;
	}

	int GetAvgWidthPenalty(SHORT shCandWidth, SHORT shReqWidth)
	{
		if ( 0 == shCandWidth && 0 != shReqWidth )
			return 4000;

		return abs( shCandWidth - shReqWidth ) * 4;
	}
	int GetAscentPenalty(SHORT shCandAscent, SHORT shReqAscent)
	{
		if ( 0 == shCandAscent && 0 != shReqAscent )
			return 100;

		return abs( shCandAscent - shReqAscent ) / 10;
	}
	int GetDescentPenalty(SHORT shCandDescent, SHORT shReqDescent)
	{
		if ( 0 == shCandDescent && 0 != shReqDescent )
			return 100;

		return abs( shCandDescent - shReqDescent ) / 10;
	}
	int GetLineGapPenalty(SHORT shCandLineGap, SHORT shReqLineGap)
	{
		if ( 0 == shCandLineGap && 0 != shReqLineGap )
			return 100;

		return abs( shCandLineGap - shReqLineGap ) / 10;
	}
	int GetXHeightPenalty(SHORT shCandXHeight, SHORT shReqXHeight)
	{
		if ( 0 == shCandXHeight && 0 != shReqXHeight )
			return 50;

		return abs( shCandXHeight - shReqXHeight ) / 20;
	}
	int GetCapHeightPenalty(SHORT shCandCapHeight, SHORT shReqCapHeight)
	{
		if ( 0 == shCandCapHeight && 0 != shReqCapHeight )
			return 50;

		return abs( shCandCapHeight - shReqCapHeight ) / 20;
	}

public:
	CString GetAllStylesByFontName(const CString& strName)
	{
		CString strAllStyles = _T("<styles>");
		int nCount = m_pFonts->GetLength();
		for (int i = 0; i < nCount; ++i)
		{
			CWinFontInfo* pInfo = GetByIndex(i);
			if (pInfo->m_wsFontName == strName)
			{
				LONG lStyle = 0;
				if (pInfo->m_bBold)
					lStyle |= 1;
				if (pInfo->m_bItalic)
					lStyle |= 2;

				CString strPath = pInfo->m_wsFontPath;
				strPath.Replace(_T("'"),	_T("&apos;"));
				strPath.Replace(_T("<"),	_T("&lt;"));
				strPath.Replace(_T(">"),	_T("&gt;"));
				strPath.Replace(_T("\""),	_T("&quot;"));
				strPath.Replace(_T("&"),	_T("&amp;"));

				CString s = _T("");
				s.Format(_T("<font style=\"%d\" path=\"%s\" faceindex=\"%d\"/>"), lStyle, strPath, pInfo->m_lIndex);
				strAllStyles += s;
			}
		}

		strAllStyles += _T("</styles>");
		return strAllStyles;
	}

private:

	CList   *m_pFonts;                 // [CWinFontInfo]
	wchar_t  m_wsWinFontDir[MAX_PATH]; //
	long     m_lDefIndex;              // Номер стандартного шрифта (-1, если не задан)

};

#endif /* _WIN_FONT */