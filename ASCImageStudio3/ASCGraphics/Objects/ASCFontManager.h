// FontManager.h : Declaration of the CFontManager

#pragma once
#include "../Interfaces/IASCFontManager.h"
#include "../Interfaces/IASCGraphicsPath.h"
#include "../Interfaces/IASCWinFonts.h"
#include "../Objects/ASCWinFonts.h"

#include "Font/FontConsts.h"
#include "Font/WinFont.h"
#include "Font/FontErrors.h"
#include "Font/FontUtils.h"

#include "Font/FontEngine.h"
#include "Font/FT_Font.h"
#include "Font/FontPath.h"
#include "Font/GlyphString.h"
#include "Font/FontFile.h"

#include "Font/WinFontStorage.h"

#include "ASCGlyphImage.h"
#include "ASCGraphicsPath.h"

#include "TimeMeasurer.h"
#include "../Interfaces/XmlUtils.h"
#include "../../../Common/OfficeDrawing/File.h"

#include FT_ADVANCES_H
#include FT_GLYPH_H

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CFontManager

[
	coclass,
	default(IAVSFontManager),
	threading(apartment),
	vi_progid("AVSOfficeFontManager.FontManager"),
	progid("AVSOfficeFontManager.FontManager.1"),
	version(1.0),
	uuid("0FF37CE6-DD3C-488F-8319-13D9E392F1D8"),
	helpstring("FontManager Class")
]
class ATL_NO_VTABLE CAVSFontManager :
	public IAVSFontManager, public IAVSFontManager2
{
	friend class CGraphics;
public:

private:

	long          m_lUnit;

	CGlyphString  m_oGlyphString;    // Для работы со строкой

	CFontEngine  *m_pFontEngine;     // Класс для работы со шрифтами (установленные шрифты, кэш шрифтов и т.д.) 
	CFont        *m_pFont;           // Текущий шрифт

	BOOL          m_bStringGID;      // В строках LoadString: FALSE - юникодные значение, TRUE - номера глифов
	BOOL          m_bUseDefaultFont; // Использовать ли стандартный шрифт при рисовании недостающих в шрифте символов

	double        m_dCharSpacing;    // Межсимвольный интервал (добавляется после каждого символа, включая последний символ)

	class CMatrix
	{
	public:

		CMatrix()
		{
			Reset();
		}
		~CMatrix()
		{
		}

		void Reset()
		{
			m_dA = 1; m_dB = 0;
			m_dC = 0; m_dD = 1;
			m_dE = 0; m_dF = 0;
		}

		bool SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
		{
			if ( NotEqual( m_dA, dA ) || NotEqual( m_dB, dB ) || NotEqual( m_dC, dC ) ||
				 NotEqual( m_dD, dD ) || NotEqual( m_dE, dE ) || NotEqual( m_dF, dF ) )
			{
				m_dA = dA; m_dB = dB;
				m_dC = dC; m_dD = dD;
				m_dE = dE; m_dF = dF;

				return true;
			}

			return false;
		}

	private:

		inline bool NotEqual(double dA, double dB) const
		{
			double dDiff = ( dA - dB );
			dDiff = dDiff >= 0 ? dDiff : -dDiff;
			if ( dDiff <= 0.001 )
				return false;

			return true;
		}

	public:

		double m_dA;
		double m_dB;
		double m_dC;
		double m_dD;
		double m_dE;
		double m_dF;

	} m_oTextMatrix;

public:
	CAVSFontManager()
	{
	}



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		ATLTRACE2( _T("FontManager: Construct ++++++++++++++++++++++++++++++++++++++++++\n") );
		m_pFontEngine = NULL;
		m_pFont       = NULL;

		m_lUnit   = 0;

		m_dCharSpacing    = 0.0;

		m_bStringGID      = FALSE;
		m_bUseDefaultFont = FALSE;

		return S_OK;
	}

	void FinalRelease()
	{
		ATLTRACE2( _T("FontManager: Release --------------------------------------------\n") );

		if ( m_pFontEngine )
		{
			delete m_pFontEngine;
		}
	}



private:

	double UpdateSize(double dOldSize, double dDpi, double dNewDpi)
	{
		if ( 0 == dNewDpi ) dNewDpi = 72.0;
		if ( 0 == dDpi    ) dDpi    = 72.0;

		return dOldSize * dDpi / dNewDpi;
	}
	double UpdateDpi(double dDpi, double dSize, double dNewSize)
	{
		if ( 0 == dNewSize ) dNewSize = 10.0;
		if ( 0 == dDpi     ) dDpi     = 72.0;

		return dDpi * dSize / dNewSize;
	}
	double XToMM(double dX)
	{
		if ( !m_pFont )
			return 0;

		return dX * 25.4 / m_pFont->GetHorDpi();
	}
	double YToMM(double dY)
	{
		if ( !m_pFont )
			return 0;

		return dY * 25.4 / m_pFont->GetVerDpi();
	}

public:

	//----- Открытие/закрытие шрифта --------------------------------------------------------------------
	STDMETHOD(Initialize)(BSTR bsXmlOptions)
	{
		BOOL bLoadWinList = TRUE;
		CString wsLoadDir = _T("");

		CStringW wsXml( bsXmlOptions );

		if (_T("") != wsXml)
		{
			XmlUtils::CXmlNode oMainNode;
			if ( oMainNode.FromXmlString( wsXml ) )
			{
				if ( -1 != oMainNode.GetName().Find( _T("FontManagerOptions") ) )
				{
					XmlUtils::CXmlNode oNode;
					if ( oMainNode.GetNode( _T("WinList"), oNode ) )
					{
						CStringW wsLoad = oNode.GetAttribute( _T("load"), _T("1") );
						bLoadWinList = (BOOL)_wtoi( wsLoad.GetBuffer() );
					}

					if ( oMainNode.GetNode( _T("FontDir"), oNode ) )
						wsLoadDir = oNode.GetAttribute( _T("path"), _T("") );
				}
				else
				{
				}
			}
		}

		WinFontsStatusStorage oSS;
		CWinFontsStatusStorage oStatusStorage( STATUS_STORAGE_NAME );

		//CString sXml;
		//do
		//{
		//	if ( STIF_BROKEN == oSS.m_sStatus || STIF_ERROR == oSS.m_sStatus )
		//	{
		//		// TO DO: делаем обычный Initialize
		//		break;
		//	}
		//	else if ( STIF_CREATING == oSS.m_sStatus )
		//	{
		//		Sleep ( 100 );
		//	}
		//	else if ( STIF_AVAILABLE == oSS.m_sStatus )
		//	{
		//		CWinFontsInfoStorage oInfoStorage( STATUS_STORAGE_NAME, oSS.m_lLength );

		//		WinFontsInfoStorage oInfo;
		//		oInfoStorage.ReadStruct( &oInfo );

		//		oInfo.m_lCount++;
		//		oInfoStorage.WriteCount( &oInfo );

		//		sXml = CString( oInfo.m_strXml );
		//	}
		//}
		//while ( STIF_CREATING == oSS.m_sStatus );

		//m_pFontEngine = new CFontEngine(TRUE, TRUE, bLoadWinList, sXml);

		BYTE* pData = NULL;
		do
		{
			bool bGetMaster = false;
			oStatusStorage.GetStatus( &bGetMaster, &oSS );
			if ( STIF_BROKEN == oSS.m_sStatus || STIF_ERROR == oSS.m_sStatus )
			{
				// TO DO: делаем обычный Initialize
				break;
			}
			else if ( STIF_CREATING == oSS.m_sStatus )
			{
				Sleep ( 100 );
			}
			else if ( STIF_AVAILABLE == oSS.m_sStatus )
			{
				CWinFontsInfoStorage oInfoStorage( STATUS_STORAGE_NAME, oSS.m_lLength );

				WinFontsInfoStorage oInfo;
				oInfoStorage.ReadStruct( &oInfo );

				pData = oInfo.m_pBuffer;
			}
		}
		while ( STIF_CREATING == oSS.m_sStatus );

		m_pFontEngine = new CFontEngine(TRUE, TRUE, bLoadWinList, pData, wsLoadDir);
		RELEASEARRAYOBJECTS(pData);

		return S_OK;
	}
	STDMETHOD(LoadFontFromFile)(BSTR bsSrcPath, float fEmSize, double dHorDpi, double dVerDpi, long lFaceIndex)
	{
		if ( !m_pFontEngine )
			return S_FALSE;

		CFontFile *pFontFile = NULL;
		if ( !( pFontFile = m_pFontEngine->GetFontFile( bsSrcPath, lFaceIndex ) ) )
		{
			pFontFile = m_pFontEngine->LoadFont( bsSrcPath, lFaceIndex );
		}

		if ( !pFontFile )
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_OPEN_ERROR;

		m_pFont = m_pFontEngine->GetFont( pFontFile );
		
		if ( !m_pFont ) 
			return S_FALSE;

		UpdateStyles( m_pFont, FALSE, FALSE );
		m_pFont->SetDefaultFont( m_pFontEngine->GetDefaultFont( FALSE, FALSE ) );
		fEmSize = UpdateSize( (double) fEmSize, dVerDpi, (unsigned int)dVerDpi );
		m_pFont->SetSizeAndDpi( fEmSize, (unsigned int)dHorDpi, (unsigned int)dVerDpi );

		m_pFont->SetStringGID( m_bStringGID );
		m_pFont->SetUseDefaultFont( m_bUseDefaultFont );
		m_pFont->SetCharSpacing( m_dCharSpacing );
		
		m_oTextMatrix.Reset();
		m_oGlyphString.ResetCTM();
		m_pFont->SetTextMatrix( 1, 0, 0, 1, 0, 0 );

		return S_OK;
	}
	STDMETHOD(LoadFontByName)(BSTR bsFamilyName, float fEmSize, long lStyle, double dHorDpi, double dVerDpi)
	{
		if ( !m_pFontEngine )
			return S_FALSE;

		BOOL bBold   = FALSE;
		BOOL bItalic = FALSE;

		if ( lStyle & FontConstants::FontStyle::FontStyleBold )
			bBold = TRUE;

		if ( lStyle & FontConstants::FontStyle::FontStyleItalic )
			bItalic = TRUE;

		CFontFile *pFontFile = NULL;

		if ( !( pFontFile = m_pFontEngine->GetFontFile( bsFamilyName, bBold, bItalic ) ) )
		{
			pFontFile = m_pFontEngine->LoadFont( bsFamilyName, bBold, bItalic );
		}

		BOOL bDefaultFont = FALSE;
		if ( !pFontFile )
		{
			bDefaultFont = TRUE;
			m_pFont = m_pFontEngine->GetDefaultFont( bBold, bItalic );
		}
		else
		{
			m_pFont = m_pFontEngine->GetFont( pFontFile );
			m_pFont->SetDefaultFont( m_pFontEngine->GetDefaultFont( bBold, bItalic ) );
		}

		if ( !m_pFont ) 
			return S_FALSE;

		UpdateStyles( m_pFont, bBold, bItalic );
		fEmSize = UpdateSize( (double) fEmSize, dVerDpi, (unsigned int)dVerDpi );
		m_pFont->SetSizeAndDpi( fEmSize, (unsigned int)dHorDpi, (unsigned int)dVerDpi );

		m_pFont->SetStringGID( m_bStringGID );
		m_pFont->SetUseDefaultFont( m_bUseDefaultFont );
		m_pFont->SetCharSpacing( m_dCharSpacing );

		m_oTextMatrix.Reset();
		m_oGlyphString.ResetCTM();
		m_pFont->SetTextMatrix( 1, 0, 0, 1, 0, 0 );

		if ( TRUE == bDefaultFont )
			return AVS_OFFICEFONTMANAGER_ERROR_DEFAULT_FONT_WAS_LOAD;
		else
			return S_OK;
	}
	STDMETHOD(GetFontType)(BSTR *pbsType)
	{
		CStringW wsType = _T("");
		if ( !m_pFont )
		{
			*pbsType = wsType.AllocSysString();
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		wsType = m_pFont->GetFontFormat();
		*pbsType = wsType.AllocSysString();

		return S_OK;
	}
	STDMETHOD(SetDefaultFont)(BSTR bsFamilyName)
	{
		if ( !m_pFontEngine )
			return S_FALSE;

		m_pFontEngine->SetDefaultFont( bsFamilyName );

		return S_OK;
	}
	//----- Функции аналогичные методам классов Font и FontFamily в GDI+ --------------------------------
	STDMETHOD(GetCellAscent)(unsigned short *pusAscent)
	{
		if ( !m_pFont )
		{
			*pusAscent = 0;
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		*pusAscent = m_pFont->GetAscender();

		return S_OK;
	}
	STDMETHOD(GetCellDescent)(unsigned short *pusDescent)
	{
		if ( !m_pFont )
		{
			*pusDescent = 0;
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		*pusDescent = fabs( m_pFont->GetDescender() );

		return S_OK;
	}
	STDMETHOD(GetEmHeight)(unsigned short *pusEmHeight)
	{
		if ( !m_pFont )
		{
			*pusEmHeight = 0;
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		*pusEmHeight = m_pFont->GetUnitsPerEm();

		return S_OK;
	}
	STDMETHOD(GetFamilyName)(BSTR *pbsFamilyName)
	{
		if ( !m_pFont )
		{
			CString sFamilyName( _T("") );
			*pbsFamilyName = sFamilyName.AllocSysString();
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		CString sFamilyName( m_pFont->GetFamilyName() );
		*pbsFamilyName = sFamilyName.AllocSysString();

		return S_OK;
	}

	STDMETHOD(GetLineSpacing)(unsigned short *pusLineSpacing)
	{
		if ( !m_pFont )
		{
			*pusLineSpacing = 0;
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		*pusLineSpacing = m_pFont->GetLineSpacing();

		return S_OK;
	}
	STDMETHOD(IsStyleAvailable)(long lStyle, BOOL *pbAvailable)
	{
		if ( !m_pFontEngine || !m_pFont )
		{
			*pbAvailable = FALSE;
			return S_OK;
		}

		*pbAvailable = m_pFontEngine->IsStyleAvailable( CStringW( m_pFont->GetFamilyName() ).GetBuffer(), lStyle );

		return S_OK;
	}



	STDMETHOD(GetHeight)(float fDPI, float *pfHeight)
	{
		if ( !m_pFont )
		{
			*pfHeight = 0.0f;
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		unsigned short usEmHeight = 0;
		unsigned short usLineSpacing = 0;

		HRESULT hRes = S_OK;
		if ( FAILED( hRes = GetEmHeight( &usEmHeight ) ) || usEmHeight <= 0.0001 )
			return hRes;

		if ( FAILED( hRes = GetLineSpacing( &usLineSpacing ) ) )
			return hRes;

		float fKoef = 1.0f;
		switch( m_lUnit )
		{
		case FontConstants::UnitPoint:      fKoef = fDPI / 72;   break;
		case FontConstants::UnitInch:       fKoef = fDPI;        break;
		case FontConstants::UnitDocument:   fKoef = fDPI / 300;  break;
		case FontConstants::UnitMillimeter: fKoef = fDPI / 25.4; break;
		}

		*pfHeight = usLineSpacing * ( m_pFont->GetFontSize() / usEmHeight ) * fKoef;

		return S_OK;
	}
	STDMETHOD(GetFamilyNameEx)(BSTR bsOptions, BSTR *pbsFamilyName)
	{
		HRESULT hRes = S_OK;

		if ( FAILED( hRes = GetFamilyName( pbsFamilyName ) ) ) 
		{
			if ( NULL != (*pbsFamilyName) ) ::SysFreeString( (*pbsFamilyName) );
			return hRes;
		}

		XmlUtils::CXmlNode oMainNode;
		oMainNode.FromXmlString( bsOptions );

		if ( _T("DeletePDFPrefix") == oMainNode.GetName() )
		{
			CString sFamilyName( (*pbsFamilyName) );
			// Удаляем префикс, который встречается в шрифтах внедренных в PDF
			// Его вид : BAAAAA+FamilyName (6 латинских букв в верхнем регистре и '+')
			// Пример  : EOODIA+Poetica

			if ( sFamilyName.GetLength() > 7 )
			{
				BOOL bPrefix = TRUE;
				for ( int nIndex = 0; nIndex < 6; nIndex++ )
				{
					wchar_t nChar = sFamilyName.GetAt( nIndex );
					if ( nChar < wchar_t('A') || nChar > wchar_t('Z') )
					{
						bPrefix = FALSE;
						break;
					}
				}

				if ( wchar_t( sFamilyName.GetAt( 6 ) ) != wchar_t('+') )
					bPrefix = FALSE;

				if ( bPrefix )
				{
					if ( NULL != (*pbsFamilyName) ) ::SysFreeString( (*pbsFamilyName) );
					sFamilyName.Delete( 0, 7 );
					(*pbsFamilyName) = sFamilyName.AllocSysString();
				}
			}
			else
			{
				// Ничего не делаем
			}
		}
		else
		{
			// ничего не делаем, оставляес bsFamilyName как есть
		}


		return S_OK;
	}
	//----- Чтение глобальных параметров шрифта ---------------------------------------------------------
	STDMETHOD(GetFacesCount)(long *plFacesCount)
	{
		if ( !m_pFont )
		{
			*plFacesCount = 0;
			return S_FALSE;
		}

		*plFacesCount = m_pFont->GetFacesCount();

		return S_OK;
	}
	STDMETHOD(GetFaceIndex)(long *plFaceIndex)
	{
		if ( !m_pFont )
		{
			*plFaceIndex = 0;
			return S_FALSE;
		}

		*plFaceIndex = m_pFont->GetFaceIndex();

		return S_OK;
	}
	STDMETHOD(GetGlyphsCount)(long *plGlyphsCount)
	{
		if ( !m_pFont )
		{
			*plGlyphsCount = 0;
			return S_FALSE;
		}

		*plGlyphsCount = m_pFont->GetGlyphsCount();

		return S_OK;
	}
	STDMETHOD(GetStyleName)(BSTR *pbsStyleName)
	{
		if ( !m_pFont )
		{
			CString sStyleName( _T("") );
			*pbsStyleName = sStyleName.AllocSysString();

			return S_FALSE;
		}

		CString sStyleName( m_pFont->GetStyleName() );
		*pbsStyleName = sStyleName.AllocSysString();

		return S_OK;
	}


	STDMETHOD(IsCharAvailable)(long lUnicode, BOOL *pbAvailable)
	{
		if ( !m_pFont )
			return S_FALSE;

		*pbAvailable = m_pFont->IsCharAvailable( lUnicode );

		return S_OK;
	}

	STDMETHOD(GetUnderlinePosition)(short *pshUnderPos)
	{
		if ( !m_pFont )
		{
			*pshUnderPos = 0;
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		*pshUnderPos = m_pFont->GetUnderlinePosition();

		return S_OK;
	}
	STDMETHOD(GetUnderlineThickness)(short *pshUnderThick)
	{
		if ( !m_pFont )
		{
			*pshUnderThick = 0;
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		*pshUnderThick = m_pFont->GetUnderlineThickness();

		return S_OK;
	}
	STDMETHOD(GetMaxAdvanceWidth)(short *pshMaxAdvWidth)
	{
		if ( !m_pFont )
		{
			*pshMaxAdvWidth = 0;
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		*pshMaxAdvWidth = m_pFont->GetMaxAdvanceWidth();

		return S_OK;
	}
	STDMETHOD(GetMaxAdvanceHeight)(short *pshMaxAdvHeight)
	{
		if ( !m_pFont )
		{
			*pshMaxAdvHeight = 0;
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		*pshMaxAdvHeight = m_pFont->GetMaxAdvanceHeight();

		return S_OK;
	}
	STDMETHOD(GetBBox)(long *plMinX, long *plMinY, long *plMaxX, long *plMaxY)
	{
		if ( !m_pFont )
		{
			*plMinX = 0;
			*plMinY = 0;
			*plMaxX = 0;
			*plMaxY = 0;
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;
		}

		m_pFont->GetBBox( plMinX, plMinY, plMaxX, plMaxY );

		return S_OK;
	}
	STDMETHOD(IsUnicodeRangeAvailable)(unsigned long ulBit, unsigned int unLongIndex, VARIANT_BOOL *pvbSuccess)
	{
		*pvbSuccess = VARIANT_FALSE;
		if ( !m_pFont )
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;

		if ( m_pFont->IsUnicodeRangeAvailable( ulBit, unLongIndex ) > 0 )
			*pvbSuccess = VARIANT_TRUE;

		return S_OK;
	}
	STDMETHOD(GetNameIndex)(BSTR bsName, unsigned short *pushGID)
	{
		if ( !m_pFont )
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;

		USES_CONVERSION;
		*pushGID = m_pFont->GetNameIndex( W2A(bsName) );

		return S_OK;
	}
	STDMETHOD(GetCodeByGID)(unsigned short ushGID, unsigned long *pulCharCode)
	{
		if ( !m_pFont )
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;

		//*pulCharCode = m_pFont->GetCodeByGID( ushGID );

		return S_OK;
	}
	STDMETHOD(GetPanose)(SAFEARRAY **ppsaPanose)
	{
		if ( !m_pFont )
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;

		signed char *sPanose = new signed char[10];
		if ( !sPanose )
			return AVS_OFFICEFONTMANAGER_ERROR_NOT_ENOUGH_MEMORY;

		m_pFont->GetPanose( (char **)&sPanose );

		SAFEARRAYBOUND saBound;
		saBound.lLbound	  = 0;
		saBound.cElements = 10;

		SAFEARRAY *psaArray = SafeArrayCreate( VT_I1, 1, &saBound );
		memcpy( psaArray->pvData, sPanose, 10 * sizeof(signed char) );
		delete []sPanose;

		*ppsaPanose = psaArray;

		return S_OK;
	}
	STDMETHOD(IsFixedWidth)(BOOL *pbIsFixedWidth)
	{
		if ( !m_pFont )
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;

		*pbIsFixedWidth = ( m_pFont->IsFixedWidth() ? TRUE : FALSE );

		return S_OK;
	}
	//----- Работа с глифами ----------------------------------------------------------------------------
	STDMETHOD(LoadString)(BSTR bsString, float fX, float fY)
	{
		if ( !m_pFont )
			return FALSE;

		CStringW wsBuffer = CStringW( bsString );

		m_oGlyphString.SetString( wsBuffer, fX, fY );
		m_pFont->GetString( &m_oGlyphString );

		return S_OK;
	}
	STDMETHOD(LoadString2)(BSTR bsString, float fX, float fY)
	{
		if ( !m_pFont )
		{
			m_oGlyphString.Reset();
			return FALSE;
		}

		CStringW wsBuffer = CStringW( bsString );

		m_oGlyphString.SetString( wsBuffer, fX, fY );
		m_pFont->GetString2( &m_oGlyphString );

		return S_OK;
	}
	STDMETHOD(GetStringPath)(BSTR *pbsXmlPath)
	{
		if ( !m_pFont || m_oGlyphString.GetLength() <= 0 )
		{
			CString sXml( _T("") );
			*pbsXmlPath = sXml.AllocSysString();

			return S_FALSE;
		}


		CFontPath *pOverallPath = new CFontPath();
		if ( !pOverallPath )
			return AVS_OFFICEFONTMANAGER_ERROR_NOT_ENOUGH_MEMORY;

		for ( int nIndex = 0; nIndex < m_oGlyphString.GetLength(); nIndex++ )
		{
			TGlyph *pCurGlyph = m_oGlyphString.GetAt(nIndex);
			CFontPath *pPath = NULL;
			if ( glyphstateNormal == pCurGlyph->eState || ( glyphstateDeafault == pCurGlyph->eState && NULL != m_pFont->GetDefaultFont() ) )
			{
				if ( glyphstateNormal == pCurGlyph->eState )
				{
					pPath = m_pFont->GetGlyphPath( pCurGlyph->lUnicode );
				}
				else
				{
					pPath = m_pFont->GetDefaultFont()->GetGlyphPath( pCurGlyph->lUnicode );
				}
			}

			if ( !pPath )
				continue;

			pPath->Reverse();
			pPath->Offset( m_oGlyphString.GetAt(nIndex)->fX + m_oGlyphString.m_fX, m_oGlyphString.GetAt(nIndex)->fY + m_oGlyphString.m_fY );
			pOverallPath->Append( pPath );
			delete pPath;
		}

		*pbsXmlPath = pOverallPath->ToXmlString().AllocSysString();
		delete pOverallPath;

		return S_OK;
	}
	STDMETHOD(GetStringPath2)(IAVSGraphicsBase **ppPath)
	{
		if ( !m_pFont || m_oGlyphString.GetLength() <= 0 )
			return S_FALSE;

		CFontPath *pOverallPath = new CFontPath();
		if ( !pOverallPath )
			return AVS_OFFICEFONTMANAGER_ERROR_NOT_ENOUGH_MEMORY;

		for ( int nIndex = 0; nIndex < m_oGlyphString.GetLength(); nIndex++ )
		{
			TGlyph *pCurGlyph = m_oGlyphString.GetAt(nIndex);
			CFontPath *pPath = NULL;
			if ( glyphstateNormal == pCurGlyph->eState || ( glyphstateDeafault == pCurGlyph->eState && NULL != m_pFont->GetDefaultFont() ) )
			{
				if ( glyphstateNormal == pCurGlyph->eState )
				{
					pPath = m_pFont->GetGlyphPath( pCurGlyph->lUnicode );
				}
				else
				{
					pPath = m_pFont->GetDefaultFont()->GetGlyphPath( pCurGlyph->lUnicode );
				}
			}

			if ( !pPath )
				continue;

			pPath->Reverse();
			pPath->Offset( m_oGlyphString.GetAt(nIndex)->fX + m_oGlyphString.m_fX, m_oGlyphString.GetAt(nIndex)->fY + m_oGlyphString.m_fY );
			pOverallPath->Append( pPath );
			delete pPath;
		}

		if ( !ppPath )
		{
			delete pOverallPath;
			return S_FALSE;
		}

		*ppPath = NULL;
		HRESULT hRes = CoCreateInstance( __uuidof(CAVSGraphicsPath), NULL, CLSCTX_ALL, __uuidof(IAVSGraphicsPath), (void**)ppPath );
		if ( hRes != S_OK || NULL == *ppPath )
		{
			delete pOverallPath;
			return S_FALSE;
		}

		pOverallPath->ToInterface( (IAVSGraphicsPath **)ppPath );
		delete pOverallPath;

		return S_OK;
	}
	STDMETHOD(GetNextChar)(IUnknown **ppImage, float *pfX, float *pfY, VARIANT_BOOL *pvbSuccess)
	{
		*pvbSuccess = VARIANT_FALSE;
		BOOL bNotLast = TRUE;

		TGlyph oCurGlyph;
		if ( !(m_oGlyphString.GetNext( &oCurGlyph )) )
		{
			*ppImage = NULL;
			*pfX     = -0xFFFF;
			*pfY     = -0xFFFF;

			return S_OK;
		}

		if ( !m_pFont )
			return S_FALSE;

		if ( glyphstateNormal == oCurGlyph.eState || ( glyphstateDeafault == oCurGlyph.eState && NULL != m_pFont->GetDefaultFont() ) )
		{
			long lUnicode = oCurGlyph.lUnicode;

			TGlyphBitmap oBitmap;
			CFont *pCurFont = NULL;

			if ( glyphstateNormal == oCurGlyph.eState )
				pCurFont = m_pFont;
			else
				pCurFont = m_pFont->GetDefaultFont();

			if ( false == oCurGlyph.bBitmap )
				pCurFont->GetGlyph( lUnicode, 0, 0, &oBitmap );
			else
				oBitmap = oCurGlyph.oBitmap;

			if ( oBitmap.nWidth > 0 && oBitmap.nHeight > 0 )
			{
				// Подготавливаем pInterface
				IUnknown **pInterface = (IUnknown**)ppImage;
				if ( !pInterface )
					return S_FALSE;

				// create result interface
				*pInterface = NULL;
				HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)pInterface );
				if ( hRes != S_OK || NULL == *pInterface )
					return S_FALSE;

				MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)*pInterface;

				int nWidth  = oBitmap.nWidth;
				int nHeight = oBitmap.nHeight;

				int nBufferSize = 4 * nWidth * nHeight;
				if ( nBufferSize < 1 )
					return S_FALSE;

				pMediaData->put_ColorSpace( 1 << 6 ); // CPS_BGRA
				pMediaData->put_Width( nWidth );
				pMediaData->put_Height( nHeight );
				pMediaData->put_AspectRatioX( nWidth );
				pMediaData->put_AspectRatioY( nHeight );
				pMediaData->put_Interlaced( VARIANT_FALSE );
				pMediaData->put_Stride( 0, 4 * nWidth );
				pMediaData->AllocateBuffer( nBufferSize );

				unsigned char *pBufferPtr = NULL;
				long nCreatedBufferSize = 0;

				pMediaData->get_Buffer( &pBufferPtr );
				pMediaData->get_BufferSize( &nCreatedBufferSize );
				pMediaData->put_Plane( 0, pBufferPtr );

				if ( !pBufferPtr || nCreatedBufferSize != nBufferSize )
					return S_FALSE;

				*pInterface = pMediaData;

				// Пишем данные в pBufferPtr

				for ( int nY = 0; nY < nHeight; nY++ ) 
				{
					for ( int nX = 0; nX < nWidth; nX++ )
					{
						unsigned char nG = 255 - oBitmap.pData[nWidth * nY + nX ];

						//pBufferPtr[0] = nG;
						//pBufferPtr[1] = nG;
						//pBufferPtr[2] = nG;
						//pBufferPtr[3] = 255;

						pBufferPtr[0] = 0;//nG;
						pBufferPtr[1] = 0;//nG;
						pBufferPtr[2] = 0;//nG;
						pBufferPtr[3] = 255 - nG;

						pBufferPtr += 4;
					}
				}
			}
			else
			{
				*ppImage = NULL;
			}
			*pfX = m_oGlyphString.m_fX + oCurGlyph.fX + oBitmap.nX;
			*pfY = m_oGlyphString.m_fY + oCurGlyph.fY - oBitmap.nY; // Отдаем координату левого верхнего угла

		}
		else
		{
			// Символ не найден, ничего не рисуем
			*ppImage = NULL;

			*pfX = m_oGlyphString.m_fX + oCurGlyph.fX;
			*pfY = m_oGlyphString.m_fY + oCurGlyph.fY;
		}
		*pvbSuccess = VARIANT_TRUE;

		return S_OK;
	}
	STDMETHOD(GetNextChar2)(IAVSGlyphImage **ppImage, float *pfX, float *pfY, VARIANT_BOOL *pvbSuccess)
	{
		*pvbSuccess = VARIANT_FALSE;
		BOOL bNotLast = TRUE;

		TGlyph oCurGlyph;
		if ( !(m_oGlyphString.GetNext( &oCurGlyph )) )
		{
			*ppImage = NULL;
			*pfX     = -0xFFFF;
			*pfY     = -0xFFFF;

			return S_OK;
		}

		if ( !m_pFont )
		{
			return S_FALSE;
		}

		if ( glyphstateNormal == oCurGlyph.eState || ( glyphstateDeafault == oCurGlyph.eState && NULL != m_pFont->GetDefaultFont() ) )
		{
			long lUnicode = oCurGlyph.lUnicode;

			TGlyphBitmap oBitmap;
			CFont *pCurFont = NULL;

			if ( glyphstateNormal == oCurGlyph.eState )
				pCurFont = m_pFont;
			else
				pCurFont = m_pFont->GetDefaultFont();

			if ( false == oCurGlyph.bBitmap )
				pCurFont->GetGlyph( lUnicode, 0, 0, &oBitmap );
			else
				oBitmap = oCurGlyph.oBitmap;

			if ( oBitmap.nWidth > 0 && oBitmap.nHeight > 0 )
			{
				if ( !ppImage )
					return S_FALSE;

				// create result interface
				*ppImage = NULL;
				HRESULT hRes = CoCreateInstance( __uuidof(CAVSGlyphImage), NULL, CLSCTX_ALL, __uuidof(IAVSGlyphImage), (void**)ppImage );
				if ( hRes != S_OK || NULL == *ppImage )
					return S_FALSE;

				IAVSGlyphImage2* pGlyph2 = NULL;
				(*ppImage)->QueryInterface(__uuidof(IAVSGlyphImage2), (void**)&pGlyph2);

				if (NULL != pGlyph2)
					pGlyph2->Create(oBitmap.pData, oBitmap.nX, oBitmap.nY, oBitmap.nWidth, oBitmap.nHeight, FALSE/*oBitmap.bFreeData*/);

				RELEASEINTERFACE(pGlyph2);

				//CAVSGlyphImage* pClassImage = dynamic_cast<CAVSGlyphImage*>(*ppImage);
				//if (NULL != pClassImage)
				//	pClassImage->Create( &oBitmap );
			}
			else
			{
				*ppImage = NULL;
			}
			*pfX = m_oGlyphString.m_fX + oCurGlyph.fX + oBitmap.nX;
			*pfY = m_oGlyphString.m_fY + oCurGlyph.fY - oBitmap.nY; // Отдаем координату левого верхнего угла

		}
		else
		{
			// Символ не найден, ничего не рисуем
			*ppImage = NULL;

			*pfX = m_oGlyphString.m_fX + oCurGlyph.fX;
			*pfY = m_oGlyphString.m_fY + oCurGlyph.fY;
		}
		*pvbSuccess = VARIANT_TRUE;

		return S_OK;
	}
	STDMETHOD(GetCharBBox)(float *pfX, float *pfY, float *pfWidth, float *pfHeight)
	{
		if ( m_oGlyphString.GetLength() <= 0 )
		{
			*pfX      = 0;
			*pfY      = 0;
			*pfWidth  = 0;
			*pfHeight = 0;

			return S_FALSE;
		}

		float fLeft = 0, fTop = 0, fBottom = 0, fRight = 0;

		m_oGlyphString.GetBBox( &fLeft, &fTop, &fRight, &fBottom );
		
		*pfX = fLeft;
		*pfY = fTop;

		*pfWidth  = fabs( (fRight - fLeft) );
		*pfHeight = fabs( (fTop - fBottom) );

		return S_OK;
	}
	STDMETHOD(GetUnderline)(float *pfStartX, float *pfStartY, float *pfEndX, float *pfEndY, float *pfSize)
	{
		if ( m_oGlyphString.GetLength() <= 0 || !m_pFont )
		{
			*pfStartX = 0;
			*pfStartY = 0;
			*pfEndX = 0;
			*pfEndY = 0;
			*pfSize = 0;

			return S_FALSE;
		}

		float fStartX = m_oGlyphString.m_fX;
		float fStartY = m_oGlyphString.m_fY;
		float fEndX   = m_oGlyphString.m_fEndX;
		float fEndY   = m_oGlyphString.m_fEndY;

		short shUnderlinePos  = m_pFont->GetUnderlinePosition();
		short shUnderlineSize = m_pFont->GetUnderlineThickness();

		float fKoef = (float)(m_pFont->GetFontSize() / 1000 * m_pFont->GetVerDpi() / 72.0f);
		float fUnderlinePos  = shUnderlinePos  * fKoef;
		float fUnderLineSize = shUnderlineSize * fKoef;

		fStartY -= fUnderlinePos;
		fEndY   -= fUnderlinePos;

		m_oGlyphString.Transform( &fStartX, &fStartY );
		m_oGlyphString.Transform( &fEndX, &fEndY );

		*pfStartX = fStartX;
		*pfStartY = fStartY;

		*pfEndX = fEndX;
		*pfEndY = fEndY;

		*pfSize = fUnderLineSize;

		return S_OK;
	}
	STDMETHOD(MeasureString)(float *pfX, float *pfY, float *pfWidth, float *pfHeight)
	{
		if ( m_oGlyphString.GetLength() <= 0 )
		{
			*pfX      = 0;
			*pfY      = 0;
			*pfWidth  = 0;
			*pfHeight = 0;

			return S_FALSE;
		}

		float fTop = 0xFFFF, fBottom = -0xFFFF, fLeft = 0xFFFF, fRight = -0xFFFF;
		for ( int nIndex = 0; nIndex < m_oGlyphString.GetLength(); nIndex++ )
		{
			float fT, fB, fL, fR;
			m_oGlyphString.GetBBox( &fL, &fT, &fR, &fB, nIndex );

			if ( fBottom < fB )
				fBottom = fB;

			if ( fTop > fT )
				fTop = fT;

			if ( fLeft > fL )
				fLeft = fL;

			if ( fRight < fR )
				fRight = fR;
		}

		*pfX = fLeft;
		*pfY = fTop;

		*pfWidth  = fabs( (fRight - fLeft) );
		*pfHeight = fabs( (fTop - fBottom) );

		return S_OK;
	}
	STDMETHOD(MeasureString2)(float *pfX, float *pfY, float *pfWidth, float *pfHeight)
	{
		if ( m_oGlyphString.GetLength() <= 0 )
		{
			*pfX      = 0;
			*pfY      = 0;
			*pfWidth  = 0;
			*pfHeight = 0;

			return S_FALSE;
		}

		float fTop = 0xFFFF, fBottom = -0xFFFF, fLeft = 0xFFFF, fRight = -0xFFFF;

		m_oGlyphString.GetBBox2( &fLeft, &fTop, &fRight, &fBottom );

		*pfX = fLeft;
		*pfY = fTop;

		*pfWidth  = fabs( (fRight - fLeft) );
		*pfHeight = fabs( (fTop - fBottom) );

		return S_OK;
	}
	STDMETHOD(SetFontMatrix)(float fA, float fB, float fC, float fD, float fE, float fF)
	{
		if ( !m_pFont )
		{
			return S_FALSE;
		}

		m_pFont->SetFontMatrix( fA, fB, fC, fD, fE, fF );

		return S_OK;
	}
	STDMETHOD(SetTextMatrix)(float fA, float fB, float fC, float fD, float fE, float fF)
	{
		if ( !m_pFont )
		{
			return S_FALSE;
		}

		//if ( m_oTextMatrix.SetTransform( fA, fB, fC, fD, fE, fF ) )
		//{
		//	m_pFont->SetTextMatrix( fA, fB, fC, fD, fE, fF );
		//	m_oGlyphString.SetCTM( fA, fB, fC, fD, fE, fF );
		//}

		if ( m_oTextMatrix.SetTransform( fA, fB, fC, fD, 0, 0 ) )
		{
			m_pFont->SetTextMatrix( fA, fB, fC, fD, 0, 0 );
			m_oGlyphString.SetCTM( fA, fB, fC, fD, 0, 0 );
			m_oGlyphString.SetTrans( fE, fF );
		}
		else
		{
			m_oGlyphString.SetTrans( fE, fF );
		}

		return S_OK;
	}
	STDMETHOD(SetStringGID)(BOOL bStringGID)
	{
		m_bStringGID = bStringGID;

		if ( !m_pFont )
			return S_OK;

		m_pFont->SetStringGID( m_bStringGID );

		return S_OK;
	}
	STDMETHOD(GetStringGID)(BOOL *pbStringGID)
	{
		if ( !pbStringGID )
			return S_FALSE;

		*pbStringGID = m_bStringGID;

		return S_OK;
	}
	STDMETHOD(SetCharSpacing)(double dSpacing)
	{
		m_dCharSpacing = dSpacing;

		if ( m_pFont )
			m_pFont->SetCharSpacing( m_dCharSpacing );

		return S_OK;
	}
	STDMETHOD(GetCharSpacing)(double *pdSpacing)
	{
		if ( !pdSpacing )
			return S_FALSE;

		*pdSpacing = m_dCharSpacing;

		return S_OK;
	}
	//----- Работа со шрифтами Windows ------------------------------------------------------------------
	STDMETHOD(GetWinFontsCount)(long *plCount)
	{
		if ( !m_pFontEngine )
		{
			*plCount = 0;
			return S_OK;
		}

		*plCount = m_pFontEngine->GetInstalledFontsCount();
		return S_OK;
	}
	STDMETHOD(GetWinFontByIndex)(long nIndex, BSTR *pbsFontName, BSTR *pbsFontStyle, BSTR *pbsFontPath, long *plIndex, BOOL *pbBold, BOOL *pbItalic)
	{	
		if ( !m_pFontEngine )
		{
			*pbsFontName  = CStringW( _T("") ).AllocSysString();
			*pbsFontStyle = CStringW( _T("") ).AllocSysString();
			*pbsFontPath  = CStringW( _T("") ).AllocSysString();
			*plIndex = 0;

			return S_OK;
		}

		CWinFontInfo *pFontInfo = m_pFontEngine->GetInstalledFont( nIndex );

		*pbsFontName  = CStringW( pFontInfo->m_wsFontName ).AllocSysString();
		*pbsFontStyle = CStringW( pFontInfo->m_wsStyle ).AllocSysString(); 
		*pbsFontPath  = CStringW( pFontInfo->m_wsFontPath ).AllocSysString();
		*plIndex  = pFontInfo->m_lIndex;
		*pbBold   = pFontInfo->m_bBold;
		*pbItalic = pFontInfo->m_bItalic;

		return S_OK;
	}


	STDMETHOD(GetWinFontByName)(BSTR bsFontName, BSTR *pbsFontPath, long *plIndex)
	{
		return S_OK;
	}
	STDMETHOD(GetWinFontByParams)(BSTR bsFontParams, BSTR *pbsFontPath, long *plIndex)
	{
		CWinFontInfo *pFontInfo = NULL;
		if ( !m_pFontEngine || NULL == ( pFontInfo = m_pFontEngine->GetInstalledFont( bsFontParams ) ) )
		{
			*pbsFontPath = CStringW( _T("") ).AllocSysString();
			*plIndex     = 0;

			return S_FALSE;
		}

		*pbsFontPath = pFontInfo->m_wsFontPath.AllocSysString();
		*plIndex     = pFontInfo->m_lIndex;

		return S_OK;
	}
	//----- IAVSGraphicsBase ----------------------------------------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName = ParamName;
		if ( _T("UseDefaultFont") == sParamName && VT_BOOL == ParamValue.vt )
		{
			m_bUseDefaultFont = ( ParamValue.boolVal == -1 ? TRUE : FALSE );
		}
		else if ( _T("DumpFontsJS") == sParamName && VT_BSTR == ParamValue.vt )
		{
			DumpToJSEditor((CString)ParamValue.bstrVal, false);
		}
		else if (_T("InitializeFromFolder") == sParamName && VT_BSTR == ParamValue.vt)
		{
			CFile oFile;
			if (S_OK == oFile.OpenFile(((CString)ParamValue.bstrVal) + _T("\\font_selection.bin")))
			{
				LONG lSize = (LONG)oFile.GetFileSize();
				BYTE* pData = new BYTE[lSize];
				oFile.ReadFile(pData, (DWORD)lSize);
				
				m_pFontEngine = new CFontEngine(TRUE, TRUE, TRUE, pData, (CString)ParamValue.bstrVal);
				RELEASEARRAYOBJECTS(pData);
			}
			else
			{
				Initialize(L"");
			}
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CString sParamName = ParamName;
		if ( (NULL == m_pFont) && (_T("GetAllStylesByFontName") != sParamName) )
			return S_FALSE;

		if ( _T("GetFontPath") == sParamName && NULL != ParamValue  )
		{
			ParamValue->vt      = VT_BSTR;
			ParamValue->bstrVal = m_pFont->GetFontFile()->GetFontPath().AllocSysString();
		}
		if ( _T("GetFontIndex") == sParamName && NULL != ParamValue )
		{
			ParamValue->vt     = VT_INT;
			ParamValue->intVal = m_pFont->GetFontFile()->GetFontIndex();
		}
		if (_T("GetAllStylesByFontName") == sParamName && NULL != ParamValue && ParamValue->vt == VT_BSTR)
		{
			CString strName = (CString)ParamValue->bstrVal;
			SysFreeString((ParamValue->bstrVal));

			ParamValue->bstrVal = m_pFontEngine->GetInstalledFonts(strName).AllocSysString();
		}

		return S_OK;
	}
	STDMETHOD(CreateDublicate)(IAVSGraphicsBase** ppGraphicsBase)
	{
		return S_OK;
	}
	//---------------------------------------------------------------------------------------------------
	// IAVSFontManager2
	//---------------------------------------------------------------------------------------------------
	STDMETHOD(UseDefaultFont)(BOOL bUseDefFont)
	{
		m_bUseDefaultFont = bUseDefFont;
		return S_OK;
	}
	STDMETHOD(GetFontPath)(BSTR *pbsFontPath)
	{
		if ( NULL == m_pFont )
			return S_FALSE;

		*pbsFontPath = m_pFont->GetFontFile()->GetFontPath().AllocSysString();

		return S_OK;
	}
	STDMETHOD(GetFontIndex)(LONG *plFontIndex)
	{
		if ( NULL == m_pFont )
			return S_FALSE;

		*plFontIndex = m_pFont->GetFontFile()->GetFontIndex();

		return S_OK;
	}
	STDMETHOD(GetWinFontByParams2)(BSTR *pbsFontName, BSTR bsAltFontName, LONG lCharset, BSTR bsFamilyClass, LONG* plStyle, LONG lFixed, BSTR bsPanose, LONG lUnicodeRange1, LONG lUnicodeRange2, LONG lUnicodeRange3, LONG lUnicodeRange4, LONG lCodePageRange1, LONG lCodePageRange2, LONG lAvgWidth)
	{
		CWinFontInfo *pFontInfo = NULL;
		
		long lStyle = -1;
		if ( plStyle )
			lStyle = *plStyle;

		if ( !m_pFontEngine || NULL == ( pFontInfo = m_pFontEngine->GetInstalledFont( (wchar_t*)bsAltFontName, lCharset, (wchar_t*)bsFamilyClass, lStyle, lFixed, (wchar_t*)bsPanose, lUnicodeRange1, lUnicodeRange2, lUnicodeRange3, lUnicodeRange4, lCodePageRange1, lCodePageRange2, lAvgWidth ) ) )
		{
			*pbsFontName = CStringW( _T("") ).AllocSysString();
			return S_FALSE;
		}

		if ( plStyle )
			*plStyle = ( pFontInfo->m_bBold ? 1 : 0 ) + ( pFontInfo->m_bItalic ? 2 : 0 ) ;

		*pbsFontName = pFontInfo->m_wsFontName.AllocSysString();

		return S_OK;
	}

	STDMETHOD(SetFontsList)(BSTR bsXml)
	{
		// TO DO: Сделать данную функцию
		//        Смысл в том, чтобы составить дополнительный список шрифтов CWinFonts
		//        только из тех шрифтов, которые указаны в Xml.

		return S_OK;
	}
	STDMETHOD(UseFontsList)(BOOL bUseList, BSTR bsXmlOptions)
	{
		// TO DO: Используем или нет дополнительный список.
		//        В опциях указывается для чего мы его используем (поиск/загрузка)

		return S_OK;
	}
	STDMETHOD(GetPanose2)(BSTR* bsPanose)
	{
		*bsPanose = NULL;
		if ( !m_pFont )
			return AVS_OFFICEFONTMANAGER_ERROR_FONT_WASNT_LOAD;

		BYTE *sPanose = new BYTE[10];
		if ( !sPanose )
			return AVS_OFFICEFONTMANAGER_ERROR_NOT_ENOUGH_MEMORY;

		m_pFont->GetPanose( (char **)&sPanose );

		CString strMem = _T("");
		strMem.Format(_T("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"), sPanose[0], sPanose[1], sPanose[2], sPanose[3], sPanose[4], sPanose[5],
																		sPanose[6],	sPanose[7], sPanose[8], sPanose[9]);

		*bsPanose = strMem.AllocSysString();

		delete []sPanose;
		return S_OK;

	}
public:

	STDMETHOD(GetStringPath3)(ISimpleGraphicsPath* pPath)
	{
		if (NULL == pPath)
			return S_FALSE;

		if ( !m_pFont || m_oGlyphString.GetLength() <= 0 )
			return S_FALSE;

		CFontPath *pOverallPath = new CFontPath();
		if ( !pOverallPath )
			return AVS_OFFICEFONTMANAGER_ERROR_NOT_ENOUGH_MEMORY;

		for ( int nIndex = 0; nIndex < m_oGlyphString.GetLength(); nIndex++ )
		{
			TGlyph *pCurGlyph = m_oGlyphString.GetAt(nIndex);
			CFontPath *pPath = NULL;
			if ( glyphstateNormal == pCurGlyph->eState || ( glyphstateDeafault == pCurGlyph->eState && NULL != m_pFont->GetDefaultFont() ) )
			{
				if ( glyphstateNormal == pCurGlyph->eState )
				{
					pPath = m_pFont->GetGlyphPath( pCurGlyph->lUnicode );
				}
				else
				{
					pPath = m_pFont->GetDefaultFont()->GetGlyphPath( pCurGlyph->lUnicode );
				}
			}

			if ( !pPath )
				continue;

			pPath->Reverse();
			pPath->ToMM( m_pFont->GetHorDpi(), m_pFont->GetVerDpi() );
			pPath->Offset( XToMM( m_oGlyphString.GetAt(nIndex)->fX ) + m_oGlyphString.m_fX, YToMM( m_oGlyphString.GetAt(nIndex)->fY ) + m_oGlyphString.m_fY );
			pOverallPath->Append( pPath );
			delete pPath;
		}

		pOverallPath->ToInterface( pPath );
		delete pOverallPath;

		return S_OK;
	}

protected:

	class CTextItem
	{
	protected:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		CTextItem()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;
		}
		CTextItem(const CTextItem& oSrc)
		{
			m_pData = NULL;
			*this = oSrc;
		}
		CTextItem& operator=(const CTextItem& oSrc)
		{
			RELEASEMEM(m_pData);

			m_lSize		= oSrc.m_lSize;
			m_lSizeCur	= oSrc.m_lSizeCur;
			m_pData		= (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, oSrc.m_pData, m_lSizeCur * sizeof(wchar_t));
							
			m_pDataCur = m_pData + m_lSizeCur;

			return *this;
		}

		CTextItem(const size_t& nLen)
		{
			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = 0;
			m_pDataCur = m_pData;
		}
		CTextItem(wchar_t* pData, const size_t& nLen)
		{
			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = m_lSize;
			m_pDataCur = m_pData + m_lSize;
		}
		CTextItem(wchar_t* pData, BYTE* pUnicodeChecker = NULL)
		{
			size_t nLen = GetStringLen(pData);

			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = m_lSize;
			m_pDataCur = m_pData + m_lSize;

			if (NULL != pUnicodeChecker)
			{
				wchar_t* pMemory = m_pData;
				while (pMemory < m_pDataCur)
				{
					if (!pUnicodeChecker[*pMemory])
						*pMemory = wchar_t(' ');
					++pMemory;
				}
			}
		}
		virtual ~CTextItem()
		{
			RELEASEMEM(m_pData);
		}

		__forceinline void AddSize(const size_t& nSize)
		{
			if (NULL == m_pData)
			{
				m_lSize = max(nSize, 1000);				
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
		
		__forceinline void operator+=(const CTextItem& oTemp)
		{
			WriteString(oTemp.m_pData, oTemp.m_lSizeCur);
		}
		__forceinline void operator+=(_bstr_t& oTemp)
		{
			size_t nLen = oTemp.length();
			WriteString(oTemp.GetBSTR(), nLen);
		}
		__forceinline void operator+=(CString& oTemp)
		{
			size_t nLen = (size_t)oTemp.GetLength();

			#ifdef _UNICODE
			WriteString(oTemp.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)oTemp;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		__forceinline wchar_t operator[](const size_t& nIndex)
		{
			if (nIndex < m_lSizeCur)
				return m_pData[nIndex];

			return 0;
		}

		__forceinline void SetText(BSTR& bsText)
		{
			ClearNoAttack();
			size_t nLen = GetStringLen(bsText);

			WriteString(bsText, nLen);

			for (size_t i = 0; i < nLen; ++i)
			{
				if (WCHAR(8233) == m_pData[i])
					m_pData[i] = WCHAR(' ');
			}
		}
		__forceinline void AddSpace()
		{
			AddSize(1);
			*m_pDataCur = wchar_t(' ');

			++m_lSizeCur;
			++m_pDataCur;
		}
		__forceinline void CorrectUnicode(const BYTE* pUnicodeChecker)
		{
			if (NULL != pUnicodeChecker)
			{
				wchar_t* pMemory = m_pData;
				while (pMemory < m_pDataCur)
				{
					if (!pUnicodeChecker[*pMemory])
						*pMemory = wchar_t(' ');
					++pMemory;
				}
			}
		}
		__forceinline void RemoveLastSpaces()
		{
			wchar_t* pMemory = m_pDataCur - 1;
			while ((pMemory > m_pData) && (wchar_t(' ') == *pMemory))
			{
				--pMemory;
				--m_lSizeCur;
				--m_pDataCur;
			}

		}
		__forceinline bool IsSpace()
		{
			if (1 != m_lSizeCur)
				return false;
			return (wchar_t(' ') == *m_pData);
		}
		
	public:
		__forceinline void WriteString(wchar_t* pString, const size_t& nLen)
		{
			AddSize(nLen);
			//memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
			memcpy(m_pDataCur, pString, nLen << 1);
			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}
		__forceinline size_t GetCurSize()
		{
			return m_lSizeCur;
		}
		__forceinline size_t GetSize()
		{
			return m_lSize;
		}
		__forceinline void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		__forceinline void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

		__forceinline size_t GetStringLen(wchar_t* pData)
		{
			wchar_t* s = pData;
			for (; *s != 0; ++s);
			return (size_t)(s - pData);
		}

		__forceinline CString GetCString()
		{
			CString str(m_pData, (int)m_lSizeCur);
			return str;
		}
		__forceinline wchar_t* GetBuffer()
		{
			return m_pData;
		}
	};

	class CStringWriter : public CTextItem
	{
	public:
		CStringWriter() : CTextItem()
		{
		}
		virtual ~CStringWriter()
		{
		}

	public:
		
		__forceinline void WriteString(_bstr_t& bsString)
		{
			size_t nLen = bsString.length();
			CTextItem::WriteString(bsString.GetBSTR(), nLen);
		}
		__forceinline void WriteString(CString& sString)
		{
			size_t nLen = (size_t)sString.GetLength();

			#ifdef _UNICODE
			CTextItem::WriteString(sString.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)sString;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		__forceinline void WriteStringC(const CString& sString)
		{
			CString* pPointer = const_cast<CString*>(&sString);
			
			size_t nLen = (size_t)pPointer->GetLength();

			#ifdef _UNICODE
			CTextItem::WriteString(pPointer->GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)sString;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		__forceinline void WriteString(wchar_t* pString, const size_t& nLen)
		{
			CTextItem::WriteString(pString, nLen);
		}
		__forceinline void Write(CStringWriter& oWriter)
		{
			CTextItem::WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
		}
	};

	class CFontInfoJS
	{
	public:		
		CString	m_sName;

		LONG	m_lIndexR;
		LONG	m_lFaceIndexR;
		LONG	m_lIndexI;
		LONG	m_lFaceIndexI;
		LONG	m_lIndexB;
		LONG	m_lFaceIndexB;
		LONG	m_lIndexBI;
		LONG	m_lFaceIndexBI;

		CFontInfoJS()
		{
			m_sName			= _T("");

			m_lIndexR		= -1;
			m_lFaceIndexR	= -1;
			m_lIndexI		= -1;
			m_lFaceIndexI	= -1;
			m_lIndexB		= -1;
			m_lFaceIndexB	= -1;
			m_lIndexBI		= -1;
			m_lFaceIndexBI	= -1;
		}

		CFontInfoJS(const CFontInfoJS& oSrc)
		{
			*this = oSrc;
		}

		CFontInfoJS& operator=(const CFontInfoJS& oSrc)
		{
			m_sName			= oSrc.m_sName;

			m_lIndexR	= oSrc.m_lIndexR;
			m_lIndexI	= oSrc.m_lIndexI;
			m_lIndexB	= oSrc.m_lIndexB;
			m_lIndexBI	= oSrc.m_lIndexBI;

			m_lFaceIndexR	= oSrc.m_lFaceIndexR;
			m_lFaceIndexI	= oSrc.m_lFaceIndexI;
			m_lFaceIndexB	= oSrc.m_lFaceIndexB;
			m_lFaceIndexBI	= oSrc.m_lFaceIndexBI;

			return *this;
		}
	};

	void DumpJSFontFile(const CString& strPath, CString strDirectory, LONG lFontIndex)
	{
		CString strFontId = strPath;

		strFontId.Replace(_T("\\\\"), _T("\\"));
		strFontId.Replace(_T("/"), _T("\\"));
		int nStart = strFontId.ReverseFind('\\');
		int nEnd = strFontId.ReverseFind('.');
		strFontId = strFontId.Mid(nStart + 1, nEnd - nStart - 1);

		CFile oFileFontFile;
		oFileFontFile.OpenFile(strPath);
		DWORD dwCount = (DWORD)oFileFontFile.GetFileSize();
		BYTE* pBuffer = new BYTE[dwCount];
		oFileFontFile.ReadFile(pBuffer, dwCount);
		oFileFontFile.CloseFile();

		CStringWriter oWriter;
		oWriter.WriteString(_T("gFontArraysObj[\"") + strFontId + _T("\"] = ["));
		bstr_t b = L",";

		for (DWORD lIndexByte = 0; lIndexByte < dwCount; ++lIndexByte)
		{
			if (0 != lIndexByte)
				oWriter.WriteString(b);

			CString strNum = _T("");
			strNum.Format(_T("%d"), pBuffer[lIndexByte]);
			oWriter.WriteString(strNum);
		}

		CString str2 = _T("];\ngFontNamesObj[gFontNamesObj.length - 1] = \"") + strFontId + 
			_T("\";\n_STDIO.prepare(\"") + strFontId + _T("\", gFontArraysObj[\"") + strFontId + _T("\"]);");

		oWriter.WriteString(str2);

		RELEASEARRAYOBJECTS(pBuffer);
				
		CString sFilePathDst = strDirectory + _T("\\") + strFontId + _T(".js");

		CFile oFileFontFileJS;
		oFileFontFileJS.CreateFile(sFilePathDst);
		CStringA strA = (CStringA)oWriter.GetCString();
		oFileFontFileJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());
		oFileFontFileJS.CloseFile();
	}

	void DumpJSFontFile2(const CString& strPath, CString strDirectory, LONG lFontIndex)
	{
		CString strFontId = strPath;

		strFontId.Replace(_T("\\\\"), _T("\\"));
		strFontId.Replace(_T("/"), _T("\\"));
		int nStart = strFontId.ReverseFind('\\');
		int nEnd = strFontId.ReverseFind('.');
		strFontId = strFontId.Mid(nStart + 1, nEnd - nStart - 1);

		CFile oFileFontFile;
		oFileFontFile.OpenFile(strPath);
		DWORD dwCount = (DWORD)oFileFontFile.GetFileSize();
		BYTE* pBuffer = new BYTE[dwCount];
		oFileFontFile.ReadFile(pBuffer, dwCount);
		oFileFontFile.CloseFile();

		CString strFileLen = _T("");
		strFileLen.Format(_T("%u"), dwCount);

		CString strFontID =  _T("gFontArraysObj[\"") + strFontId + _T("\"]");

		CStringWriter oWriter;
		CString _t1 = strFontID + _T(" = CreateFontArray(") + strFileLen + _T(");");
		oWriter.WriteString(_t1);
		bstr_t b = L",";

		CString strFontNum = _T("");
		strFontNum.Format(_T("%d"), lFontIndex);
		CString _t2 = _T("function createFont") + strFontNum + _T("(){var p = ") + strFontID + _T(".data;var i = 0;\n");
		oWriter.WriteString(_t2);

		CString sFilePathDst = strDirectory + _T("\\") + strFontId + _T(".js");
		CFile oFileFontFileJS;
		oFileFontFileJS.CreateFile(sFilePathDst);

		LONG lCurrentCount = 0;
		for (DWORD lIndexByte = 0; lIndexByte < dwCount; ++lIndexByte)
		{
			++lCurrentCount;
			CString strWrite = _T("");
			strWrite.Format(_T("p[i++]=%d;"), pBuffer[lIndexByte]);
			oWriter.WriteString(strWrite);

			if (lCurrentCount > 1000000)
			{
				lCurrentCount = 0;
				CStringA strA = (CStringA)oWriter.GetCString();
				oFileFontFileJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());
				oWriter.ClearNoAttack();
			}
		}

		CString _t3 = _T("}\ncreateFont") + strFontNum + _T("();\ngFontNamesObj[gFontNamesObj.length - 1] = \"") + strFontId + 
			_T("\";\n_STDIO.prepare(\"") + strFontId + _T("\", gFontArraysObj[\"") + strFontId + _T("\"]);");

		oWriter.WriteString(_t3);

		RELEASEARRAYOBJECTS(pBuffer);
				
		CStringA strA = (CStringA)oWriter.GetCString();
		oFileFontFileJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());
		oFileFontFileJS.CloseFile();
	}

	void DumpJSFontFile3(const CString& strPath, CString strDirectory, LONG lFontIndex)
	{
		CString strFontId = strPath;

		strFontId.Replace(_T("\\\\"), _T("\\"));
		strFontId.Replace(_T("/"), _T("\\"));
		int nStart = strFontId.ReverseFind('\\');
		int nEnd = strFontId.ReverseFind('.');
		strFontId = strFontId.Mid(nStart + 1, nEnd - nStart - 1);

		CFile oFileFontFile;
		oFileFontFile.OpenFile(strPath);
		DWORD dwCount = (DWORD)oFileFontFile.GetFileSize();

		LONG __lcount = (LONG)(dwCount + 3) / 4;
		double __dcount = (double)__lcount;
		double __dcountsize = sqrt(__dcount);
		
		LONG lSizeX = (LONG)sqrt(__dcount) + 1;
		LONG lSizeY = lSizeX;

		//if (lSizeX * lSizeY < __lcount)
		//	lSizeY += 1;

		MediaCore::IAVSUncompressedVideoFrame* pUnkFrame = NULL;
		CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pUnkFrame);

		pUnkFrame->put_ColorSpace( ( 1 << 6) ); // CPS_BGRA | CPS_FLIP
		pUnkFrame->put_Width( lSizeX );
		pUnkFrame->put_Height( lSizeY );
		pUnkFrame->put_AspectRatioX( lSizeX );
		pUnkFrame->put_AspectRatioY( lSizeY );
		pUnkFrame->put_Interlaced( VARIANT_FALSE );
		pUnkFrame->put_Stride( 0, 4 * lSizeX );
		pUnkFrame->AllocateBuffer( -1 );
		
		BYTE* pBuffer = NULL;
		pUnkFrame->get_Buffer(&pBuffer);
		memset(pBuffer, 0, 4 * lSizeX * lSizeY);

		oFileFontFile.ReadFile(pBuffer, dwCount);
		oFileFontFile.CloseFile();

		ImageStudio::IImageTransforms* pTransform = NULL;
		CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_ALL, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

		VARIANT var;
		var.vt = VT_UNKNOWN;
		var.punkVal = (IUnknown*)pUnkFrame;

		VARIANT_BOOL vbSuccess = VARIANT_FALSE;
		pTransform->SetSource(0, var);
		pTransform->SetXml(L"<ImageFile-SaveAsPng destinationpath=\"C:\\1.png\" format=\"8888\"/>", &vbSuccess);
		pTransform->Transform(&vbSuccess);

		RELEASEINTERFACE(pUnkFrame);
		RELEASEINTERFACE(pTransform);

		if (TRUE)
		{
			if (_T("arial") == strFontId)
			{
				CopyFile(_T("C:\\1.png"), _T("C:\\arial.png"), FALSE);
			}
		}

		CFile oImageFile;
		oImageFile.OpenFile(_T("C:\\1.png"));
		int nInputLen = (int)oImageFile.GetFileSize();
		BYTE* pData = new BYTE[nInputLen];
		oImageFile.ReadFile(pData, nInputLen);
		oImageFile.CloseFile();

		int nOutputLen = Base64EncodeGetRequiredLength(nInputLen, ATL_BASE64_FLAG_NOCRLF);
		BYTE* pOutput = new BYTE[nOutputLen];
		Base64Encode(pData, nInputLen, (LPSTR)pOutput, &nOutputLen, ATL_BASE64_FLAG_NOCRLF);

		/*
		BYTE* pMemory = new BYTE[2 * nOutputLen];
		BYTE* pCurrentMemory = pMemory;
		BYTE* pCurrentOutput = pOutput;
		BYTE* pEnd = pOutput + nOutputLen;

		BYTE dump[3];
		dump[0] = BYTE('\\');
		dump[1] = 0x0D;
		dump[2] = 0x0A;

		int n = 0;
		while (true)
		{
			if ((pCurrentOutput + 76) >= pEnd)
			{
				memcpy(pCurrentMemory, pCurrentOutput, (pEnd - pCurrentOutput));
				pCurrentMemory += (pEnd - pCurrentOutput);
				break;
			}
			memcpy(pCurrentMemory, pCurrentOutput, 76);
			pCurrentMemory += 76;
			pCurrentOutput += 76;

			memcpy(pCurrentMemory, dump, 3);
			pCurrentMemory += 3;
			pCurrentOutput += 2;
		}
		*/
		
		CString sFilePathDst = strDirectory + _T("\\") + strFontId + _T(".js");
		CFile oFileFontFileJS;
		oFileFontFileJS.CreateFile(sFilePathDst);

		CString strNum = _T("");
		strNum.Format(_T("%d"), lFontIndex);
		strNum = _T("__font_image") + strNum;

		CString strNumF = _T("");
		strNumF.Format(_T("g_fontFiles[%d].IsLoaded = true;}\n"), lFontIndex);

		CString str = _T("var ") + strNum + _T("=new Image();\n") + strNum + _T(".onload=function(){\ngFontArraysObj[\"") + strFontId + _T("\"] = CreateFontData(") + strNum + _T(");\n");
		str += (_T("delete ") + strNum + _T(";\ngFontNamesObj[gFontNamesObj.length - 1] = \"") + strFontId + _T("\";\n_STDIO.prepare(\""));
		str += (strFontId + _T("\", gFontArraysObj[\"") + strFontId + _T("\"]);\n") + strNumF);

		CStringA strA = (CStringA)str;
		oFileFontFileJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());

		str = strNum + _T(".src = \"data:image/png;base64,");
		strA = (CStringA)str;
		oFileFontFileJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());

		//oFileFontFileJS.WriteFile((void*)pMemory, (DWORD)(pCurrentMemory - pMemory));
		oFileFontFileJS.WriteFile((void*)pOutput, (DWORD)(nOutputLen));

		str = _T("\";");
		strA = (CStringA)str;
		oFileFontFileJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());		

		oFileFontFileJS.CloseFile();

		//RELEASEARRAYOBJECTS(pMemory);
		RELEASEARRAYOBJECTS(pOutput);
		RELEASEARRAYOBJECTS(pData);
	}

	void DumpJSFontFile4(const CString& strPath, CString strDirectory, LONG lFontIndex)
	{
		CString strFontId = strPath;

		strFontId.Replace(_T("\\\\"), _T("\\"));
		strFontId.Replace(_T("/"), _T("\\"));
		int nStart = strFontId.ReverseFind('\\');
		int nEnd = strFontId.ReverseFind('.');
		strFontId = strFontId.Mid(nStart + 1, nEnd - nStart - 1);

		CFile oFileFontFile;
		oFileFontFile.OpenFile(strPath);
		
		int nInputLen = (int)oFileFontFile.GetFileSize();
		BYTE* pData = new BYTE[nInputLen];
		oFileFontFile.ReadFile(pData, (DWORD)nInputLen);

		int nOutputLen = Base64EncodeGetRequiredLength(nInputLen, ATL_BASE64_FLAG_NOCRLF);
		BYTE* pOutput = new BYTE[nOutputLen];
		Base64Encode(pData, nInputLen, (LPSTR)pOutput, &nOutputLen, ATL_BASE64_FLAG_NOCRLF);

		CString sFilePathDst = strDirectory + _T("\\") + strFontId + _T(".js");
		CFile oFileFontFileJS;
		oFileFontFileJS.CreateFile(sFilePathDst);

		CString strNum = _T("");
		strNum.Format(_T("%d"), lFontIndex);
		strNum = _T("__font_data") + strNum;

		CStringA strA;

		CString t1 = _T("var ") + strNum + _T("=\"");
		strA = (CStringA)t1;
		oFileFontFileJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());
		oFileFontFileJS.WriteFile((void*)pOutput, (DWORD)(nOutputLen));
		
		CString strNumF = _T("");
		strNumF.Format(_T("g_fontFiles[%d].IsLoaded = true;\n"), lFontIndex);

		CString strDecodeLen = _T("");
		strDecodeLen.Format(_T(",%d);\n"), nInputLen);

		CString t2 = _T("\";\ngFontArraysObj[\"") + strFontId + _T("\"] = CreateFontData2(") + strNum + strDecodeLen;
		t2 += (_T("delete ") + strNum + _T(";\ngFontNamesObj[gFontNamesObj.length - 1] = \"") + strFontId + _T("\";\n_STDIO.prepare(\""));
		t2 += (strFontId + _T("\", gFontArraysObj[\"") + strFontId + _T("\"]);\n") + strNumF);

		strA = (CStringA)t2;
		oFileFontFileJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());
	
		oFileFontFileJS.CloseFile();

		RELEASEARRAYOBJECTS(pOutput);
		RELEASEARRAYOBJECTS(pData);
	}
	void DumpJSFontFile5(const CString& strPath, CString strDirectory, LONG lFontIndex)
	{
		CString strFontId = strPath;

		strFontId.Replace(_T("\\\\"), _T("\\"));
		strFontId.Replace(_T("/"), _T("\\"));
		int nStart = strFontId.ReverseFind('\\');
		int nEnd = strFontId.ReverseFind('.');
		strFontId = strFontId.Mid(nStart + 1, nEnd - nStart - 1);

		CFile oFileFontFile;
		oFileFontFile.OpenFile(strPath);
		
		int nInputLen = (int)oFileFontFile.GetFileSize();
		BYTE* pData = new BYTE[nInputLen];
		oFileFontFile.ReadFile(pData, (DWORD)nInputLen);

		int nOutputLen = Base64EncodeGetRequiredLength(nInputLen, ATL_BASE64_FLAG_NOCRLF);
		BYTE* pOutput = new BYTE[nOutputLen];
		Base64Encode(pData, nInputLen, (LPSTR)pOutput, &nOutputLen, ATL_BASE64_FLAG_NOCRLF);

		CString sFilePathDst = strDirectory + _T("\\") + strFontId + _T(".js");
		CFile oFileFontFileJS;
		oFileFontFileJS.CreateFile(sFilePathDst);

		CString strNum = _T("");
		strNum.Format(_T("%d"), lFontIndex);
		strNum = _T("__font_data") + strNum;

		CStringA strA;

		CString t1 = _T("var ") + strNum + _T("=\"");
		strA = (CStringA)t1;
		oFileFontFileJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());
		oFileFontFileJS.WriteFile((void*)pOutput, (DWORD)(nOutputLen));

		CString strRes = _T("");
		strRes.Format(_T("\";\nvar __font_data%d_idx = g_fonts_streams.length;\n\
g_fonts_streams[__font_data%d_idx] = CreateFontData2(__font_data%d,%d);\n\
__font_data%d = null;\n\
g_font_files[%d].SetStreamIndex(__font_data%d_idx);"), lFontIndex, lFontIndex, lFontIndex, nInputLen, lFontIndex, lFontIndex, lFontIndex);
		strA = (CStringA)strRes;
		oFileFontFileJS.WriteFile((void*)strA.GetBuffer(), (DWORD)strA.GetLength());
			
		oFileFontFileJS.CloseFile();

		RELEASEARRAYOBJECTS(pOutput);
		RELEASEARRAYOBJECTS(pData);
	}

	void DumpJSFontFile6(const CString& strPath, CString strDirectory, LONG lFontIndex)
	{
		CString strFontId = strPath;

		strFontId.Replace(_T("\\\\"), _T("\\"));
		strFontId.Replace(_T("/"), _T("\\"));
		int nStart = strFontId.ReverseFind('\\');
		strFontId = strFontId.Mid(nStart + 1);

		CFile oFileFontFile;
		oFileFontFile.OpenFile(strPath);
		
		int nInputLen = (int)oFileFontFile.GetFileSize();
		BYTE* pData = new BYTE[nInputLen];
		oFileFontFile.ReadFile(pData, (DWORD)nInputLen);

		int nOutputLen = Base64EncodeGetRequiredLength(nInputLen, ATL_BASE64_FLAG_NOCRLF);
		BYTE* pOutput = new BYTE[nOutputLen];
		Base64Encode(pData, nInputLen, (LPSTR)pOutput, &nOutputLen, ATL_BASE64_FLAG_NOCRLF);

		CString sFilePathDst = strDirectory + _T("\\") + strFontId + _T(".js");
		CFile oFileFontFileJS;
		oFileFontFileJS.CreateFile(sFilePathDst);

		BYTE utf8_flag[3];
		utf8_flag[2] = 0xEF;
		utf8_flag[1] = 0xBB;
		utf8_flag[0] = 0xBF;
		oFileFontFileJS.WriteFile2((void*)utf8_flag, 3);

		CString strWrite = _T("window[\"");
		strWrite += strFontId;
		strWrite += _T("\"] = \"");

		CString strLen = _T("");
		strLen.Format(_T("%d;"), nInputLen);
		strWrite += strLen;

		oFileFontFileJS.WriteStringUTF8(strWrite);
		oFileFontFileJS.WriteFile((void*)pOutput, (DWORD)(nOutputLen));

		strWrite = _T("\";");
		oFileFontFileJS.WriteStringUTF8(strWrite);
		oFileFontFileJS.CloseFile();

		RELEASEARRAYOBJECTS(pOutput);
		RELEASEARRAYOBJECTS(pData);
	}

	void DumpToJSEditor(CString strDirectory, bool bIsUnionFamily);
};

