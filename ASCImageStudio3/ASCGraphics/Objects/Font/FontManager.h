// FontManager.h : Declaration of the CFontManager

#pragma once
#include "resource.h"       // main symbols
#include "Registration.h"
#include "Consts.h"
#include "WinFont.h"
#include "Errors.h"
#include "FontUtils.h"

#include "FontEngine.h"
#include "FT_Font.h"
#include "Path.h"

#include FT_ADVANCES_H
#include FT_GLYPH_H

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IFontManager
[
	object,
	uuid("8C0DA859-68A6-4CDB-9C48-8B6D66EFAC1F"),
	dual,	helpstring("IFontManager Interface"),
	pointer_default(unique)
]
__interface IFontManager : IDispatch
{
	//----- Открытие/закрытие шрифта --------------------------------------------------------------------
	[id(001)]  HRESULT Initialize([in] BSTR bsXmlOptions);
	[id(002)]  HRESULT LoadFontFromFile([in]BSTR bsSrcPath, [in] float fEmSize, [in] unsigned int unHorDPI, [in] unsigned int unVerDPI, [in] long lFaceIndex = 0);
	[id(003)]  HRESULT LoadFontByName([in]BSTR bsFamilyName, [in] float fEmSize, [in] long lStyle, [in] unsigned int unHorDPI, [in]unsigned int unVerDPI);
	[id(004)]  HRESULT GetFontType([out, retval] BSTR *pbsType);
	[id(005)]  HRESULT SetDefaultFont([in] BSTR bsFamilyName);
	//----- Функции аналогичные методам классов Font и FontFamily в GDI+ --------------------------------

	[id(101)]  HRESULT GetCellAscent([out, retval] unsigned short *pusAscent);
	[id(102)]  HRESULT GetCellDescent([out, retval] unsigned short *pusDescent);
	[id(103)]  HRESULT GetEmHeight([out, retval] unsigned short *pusEmHeight);
	[id(104)]  HRESULT GetFamilyName([out, retval] BSTR *pbsFamilyName);
	[id(105)]  HRESULT GetLineSpacing([out, retval] unsigned short *pusLineSpacing);
	[id(106)]  HRESULT IsStyleAvailable([in] long lStyle, [out, retval] BOOL *pbAvailable);
	[id(107)]  HRESULT GetHeight([in] float fDPI, [out, retval] float *pfHeight);

	//----- Чтение глобальных параметров шрифта ---------------------------------------------------------
	[id(201)]  HRESULT GetFacesCount([out, retval] long *plFacesCount);
	[id(202)]  HRESULT GetFaceIndex([out, retval] long *plFaceIndex);
	[id(203)]  HRESULT GetGlyphsCount([out, retval] long *plGlyphsCount);
	[id(205)]  HRESULT GetStyleName([out, retval] BSTR *pbsStyleName);
	[id(206)]  HRESULT IsCharAvailable([in] long lUnicode, [out, retval] BOOL *pbAvailable);
	[id(207)]  HRESULT GetUnderlinePosition([out, retval] short *pshUnderPos);
	[id(208)]  HRESULT GetUnderlineThickness([out, retval] short *pshUnderThick);
	[id(209)]  HRESULT GetMaxAdvanceWidth([out, retval] short *pshMaxAdvWidth);
	[id(210)]  HRESULT GetMaxAdvanceHeight([out, retval] short *pshMaxAdvHeight);
	[id(211)]  HRESULT GetBBox([out] long *plMinX, [out] long *plMinY, [out] long *plMaxX, [out] long *plMaxY);
	[id(212)]  HRESULT IsUnicodeRangeAvailable([in] unsigned long ulBit, [in] unsigned int unLongIndex, [out ,retval] VARIANT_BOOL *pvbSuccess);

	//----- Работа с глифами ----------------------------------------------------------------------------

	[id(301)]  HRESULT GetGlyphPath([in] long lUnicode, [out, retval] BSTR *pbsXmlPath);
	[id(302)]  HRESULT GetStringPath([in] BSTR bsString, [out, retval] BSTR *pbsXmlPath);
	[id(303)]  HRESULT GetGlyphBitmap([in] long lUnicode, [out, retval]IUnknown **ppImage); 
	[id(304)]  HRESULT GetStringBitmap([in] BSTR bsString, [out, retval] IUnknown **ppImage);
	[id(305)]  HRESULT LoadString([in] BSTR bsString);
	[id(306)]  HRESULT GetNextChar([out] IUnknown **ppImage, [out] float *pfX, [out] float *pfY, [out, retval] VARIANT_BOOL *pvbSuccess);
	[id(307)]  HRESULT GetCharBBox([out] float *pfLeft, [out] float *pfTop, [out] float *pfRight, [out] float *pfBottom);
	[id(308)]  HRESULT MeasureString([out] float *pfLeft, [out] float *pfTop, [out] float *pfRight, [out] float *pfBottom);
	[id(309)]  HRESULT ApplyTransform([in] float fA, [in] float fB, [in] float fC, [in] float fD, [in] float fE, [in] float fF);

	//----- Работа со шрифтами Windows ------------------------------------------------------------------

	[id(501)]  HRESULT GetWinFontsCount([out, retval] long *plCount);
	[id(502)]  HRESULT GetWinFont([in] long nIndex, [out] BSTR *pbsFontName, [out] BSTR *pbsFontStyle, [out] BSTR *pbsFontPath, [out] long *plIndex, [out] BOOL *pbBold, [out] BOOL *pbItalic);

};



// CFontManager

[
	coclass,
	default(IFontManager),
	threading(apartment),
	vi_progid("AVSOfficeFontManager.FontManager"),
	progid("AVSOfficeFontManager.FontManager.1"),
	version(1.0),
	uuid("0FF37CE6-DD3C-488F-8319-13D9E392F1D8"),
	helpstring("FontManager Class")
]
class ATL_NO_VTABLE CFontManager :
	public IFontManager
{
public:

private:

	float         m_fEmSize;
	long          m_lUnit;

	TGlyph       *m_pGlyphsBuffer; // Для работы со строкой
	unsigned int  m_nGlyphsCount;  // Количество символов в строке
	unsigned int  m_nGlyphIndex;   // Номер текущего символа

	CFontEngine  *m_pFontEngine;   // Класс для работы со шрифтами (установленные шрифты, кэш шрифтов и т.д.) 
	CFont        *m_pFont;         // Текущий шрифт

public:
	CFontManager()
	{
	}



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_pFontEngine = NULL;
		m_pFont       = NULL;

		m_pGlyphsBuffer = NULL;
		m_nGlyphsCount  = 0;
		m_nGlyphIndex   = -1;

		return S_OK;
	}

	void FinalRelease()
	{
		if ( m_pFontEngine )
		{
			delete m_pFontEngine;
		}
	}



private:

public:

	//----- Открытие/закрытие шрифта --------------------------------------------------------------------
	STDMETHOD(Initialize)(BSTR bsXmlOptions)
	{
		m_pFontEngine = new CFontEngine(TRUE);
		return S_OK;
	}
	STDMETHOD(LoadFontFromFile)(BSTR bsSrcPath, float fEmSize, unsigned int unHorDpi, unsigned int unVerDpi, long lFaceIndex)
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
		m_pFont->SetSizeAndDpi( fEmSize, unHorDpi, unVerDpi );

		return S_OK;
	}
	STDMETHOD(LoadFontByName)(BSTR bsFamilyName, float fEmSize, long lStyle, unsigned int unHorDpi, unsigned int unVerDpi)
	{
		if ( !m_pFontEngine )
			return S_FALSE;

		BOOL bBold   = FALSE;
		BOOL bItalic = FALSE;

		switch (lStyle)
		{
		case FontConstants::FontStyle::FontStyleRegular:    bBold = FALSE; bItalic = FALSE; break;
		case FontConstants::FontStyle::FontStyleItalic:     bBold = FALSE; bItalic = TRUE;  break;
		case FontConstants::FontStyle::FontStyleBold:       bBold = TRUE;  bItalic = FALSE; break;
		case FontConstants::FontStyle::FontStyleBoldItalic: bBold = TRUE;  bItalic = TRUE;  break;
		}

		CFontFile *pFontFile = NULL;

		if ( !( pFontFile = m_pFontEngine->GetFontFile( bsFamilyName, bBold, bItalic ) ) )
		{
			pFontFile = m_pFontEngine->LoadFont( bsFamilyName, bBold, bItalic );
		}

		if ( !pFontFile )
			return AVS_OFFICEFONTMANAGER_ERROR_WIN_FONT_NOT_FOUND;

		m_pFont = m_pFontEngine->GetFont( pFontFile );

		if ( !m_pFont ) 
			return S_FALSE;

		UpdateStyles( m_pFont, bBold, bItalic );
		m_pFont->SetDefaultFont( m_pFontEngine->GetDefaultFont( bBold, bItalic ) );
		m_pFont->SetSizeAndDpi( fEmSize, unHorDpi, unVerDpi );

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

		*pfHeight = usLineSpacing * ( m_fEmSize / usEmHeight ) * fKoef;

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
	//----- Работа с глифами ----------------------------------------------------------------------------
	STDMETHOD(GetGlyphPath)(long lUnicode, BSTR *pbsXmlPath)
	{
		if ( !m_pFont )
		{
			CString sXml( _T("") );
			*pbsXmlPath = sXml.AllocSysString();

			return S_FALSE;
		}

		CPath *pPath = m_pFont->GetGlyphPath( lUnicode );
		if ( !pPath )
			return S_FALSE;

		*pbsXmlPath = pPath->ToXmlString().AllocSysString();

		return S_OK;
	}
	STDMETHOD(GetStringPath)(BSTR bsString, BSTR *pbsXmlPath)
	{
		if ( !m_pFont )
		{
			CString sXml( _T("") );
			*pbsXmlPath = sXml.AllocSysString();

			return S_FALSE;
		}

		CStringW wsString( bsString );
		CPath *pPath = m_pFont->GetStringPath( wsString.GetBuffer(), wsString.GetLength()  );
		if ( !pPath )
			return S_FALSE;

		*pbsXmlPath = pPath->ToXmlString().AllocSysString();

		return S_OK;
	}
	STDMETHOD(GetGlyphBitmap)(long lUnicode, IUnknown **ppImage)
	{
		if ( !m_pFont )
			return S_FALSE;

		TGlyphBitmap oBitmap;
		m_pFont->GetGlyph( lUnicode, 0, 0, &oBitmap );


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

		pMediaData->put_ColorSpace( ( 1 << 6) || ( 1 << 31 )  ); // CPS_BGRA | CPS_FLIP
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
				pBufferPtr[0] = nG;
				pBufferPtr[1] = nG;
				pBufferPtr[2] = nG;
				pBufferPtr[3] = 255;
				pBufferPtr += 4;
			}
		}

		return S_OK;
	}
	STDMETHOD(GetStringBitmap)(BSTR bsString, IUnknown **ppImage)
	{
		if ( !m_pFont )
			return S_FALSE;

		CStringW wsString( bsString );
		TGlyphBitmap *pBitmaps = new TGlyphBitmap[ wsString.GetLength() ];
		if ( !pBitmaps )
			return S_FALSE;

		int nHeight = 0;
		int nWidth = 0;

		for ( int nIndex = 0; nIndex < wsString.GetLength(); nIndex++ )
		{
			long lUnicode = wsString.GetAt( nIndex );
			m_pFont->GetGlyph( lUnicode, 0, 0, &(pBitmaps[nIndex]) );
			if ( nHeight < pBitmaps[nIndex].nHeight )
				nHeight = pBitmaps[nIndex].nHeight;

			nWidth += pBitmaps[nIndex].nWidth;
		}

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

		int nBufferSize = 4 * nWidth * nHeight;
		if ( nBufferSize < 1 )
			return S_FALSE;

		pMediaData->put_ColorSpace( ( 1 << 6)  ); // CPS_BGRA 
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

		int nShiftX = 0;
		for ( int nGlyphIndex = 0; nGlyphIndex < wsString.GetLength(); nGlyphIndex++ )
		{
			for ( int nY = 0; nY < nHeight; nY++ ) 
			{
				if ( nHeight - nY < pBitmaps[nGlyphIndex].nHeight )
				{
					for ( int nX = 0; nX < pBitmaps[nGlyphIndex].nWidth; nX++ )
					{
						int nCurY = nY - ( nHeight - pBitmaps[nGlyphIndex].nHeight );
						unsigned char nG = 255 - pBitmaps[nGlyphIndex].pData[pBitmaps[nGlyphIndex].nWidth * nCurY + nX ];
						int nIndex = (nX + nShiftX + nY * nWidth) * 4;
						pBufferPtr[nIndex + 0] = nG;
						pBufferPtr[nIndex + 1] = nG;
						pBufferPtr[nIndex + 2] = nG;
						pBufferPtr[nIndex + 3] = 255;
					}
				}
				else
				{
					for ( int nX = 0; nX < pBitmaps[nGlyphIndex].nWidth; nX++ )
					{
						int nIndex = (nX + nShiftX + nY * nWidth) * 4;
						pBufferPtr[nIndex + 0] = 255;
						pBufferPtr[nIndex + 1] = 255;
						pBufferPtr[nIndex + 2] = 255;
						pBufferPtr[nIndex + 3] = 255;
					}
				}
			}
			nShiftX += pBitmaps[nGlyphIndex].nWidth;
		}

		delete []pBitmaps;
		return S_OK;
	}
	STDMETHOD(LoadString)(BSTR bsString)
	{
		if ( !m_pFont )
			return FALSE;

		CStringW wsBuffer = CStringW( bsString );

		m_nGlyphsCount  = wsBuffer.GetLength();

		if ( m_pGlyphsBuffer )
			delete []m_pGlyphsBuffer;

		m_pGlyphsBuffer = new TGlyph[m_nGlyphsCount];
		m_nGlyphIndex   = -1;

		for ( int nIndex = 0; nIndex < m_nGlyphsCount; nIndex++ )
		{
			m_pGlyphsBuffer[nIndex].lUnicode = wsBuffer.GetAt( nIndex );
		}

		m_pFont->GetString( m_pGlyphsBuffer, m_nGlyphsCount );

		return S_OK;
	}
	STDMETHOD(GetNextChar)(IUnknown **ppImage, float *pfX, float *pfY, VARIANT_BOOL *pvbSuccess)
	{
		*pvbSuccess = VARIANT_FALSE;
		m_nGlyphIndex++;

		if ( m_nGlyphIndex >= m_nGlyphsCount )
		{
			*ppImage = NULL;
			*pfX     = -0xFFFF;
			*pfY     = -0xFFFF;

			return S_OK;
		}

		if ( !m_pFont || !m_pGlyphsBuffer )
			return S_FALSE;

		if ( glyphstateNormal == m_pGlyphsBuffer[m_nGlyphIndex].eState || ( glyphstateDeafault == m_pGlyphsBuffer[m_nGlyphIndex].eState && NULL != m_pFont->GetDefaultFont() ) )
		{
			long lUnicode = m_pGlyphsBuffer[m_nGlyphIndex].lUnicode;

			TGlyphBitmap oBitmap;
			CFont *pCurFont = NULL;

			if ( glyphstateNormal == m_pGlyphsBuffer[m_nGlyphIndex].eState )
				pCurFont = m_pFont;
			else
				pCurFont = m_pFont->GetDefaultFont();

			pCurFont->GetGlyph( lUnicode, 0, 0, &oBitmap );

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

				if ( pCurFont->GetBold() )
					nWidth++;

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

				if ( !pCurFont->GetBold() )
				{
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
					// Здесь (nWidth = oBitmap.nWidth + 1)
					for ( int nY = 0; nY < nHeight; nY++ ) 
					{
						for ( int nX = nWidth - 1; nX >= 0; nX-- )
						{
							int nDstIndex = nY * nWidth * 4 + nX * 4;
							int nSrcIndex = nY * oBitmap.nWidth + nX;

							if ( 0 != nX )
							{
								int nFirstByte, nSecondByte;

								if ( nWidth - 1 == nX )
									nFirstByte = 255;
								else
									nFirstByte = 255 - oBitmap.pData[nSrcIndex];

								nSecondByte = 255 - oBitmap.pData[nSrcIndex - 1];

								pBufferPtr[nDstIndex + 0] = 0;//nG;
								pBufferPtr[nDstIndex + 1] = 0;//nG;
								pBufferPtr[nDstIndex + 2] = 0;//nG;
								pBufferPtr[nDstIndex + 3] = min( 255, 510 - (nFirstByte + nSecondByte) );

							}
							else
							{
								pBufferPtr[nDstIndex + 0] = 0;
								pBufferPtr[nDstIndex + 1] = 0;
								pBufferPtr[nDstIndex + 2] = 0;
								pBufferPtr[nDstIndex + 3] = oBitmap.pData[nSrcIndex];
							}
						}
					}
				}
			}
			else
			{
				*ppImage = NULL;
			}
			*pfX = m_pGlyphsBuffer[m_nGlyphIndex].fX + oBitmap.nX;
			*pfY = m_pGlyphsBuffer[m_nGlyphIndex].fY + oBitmap.nY - oBitmap.nHeight; // Отдаем координату левого верхнего угла

		}
		else
		{
			// Символ не найден, ничего не рисуем
			*ppImage = NULL;

			*pfX = m_pGlyphsBuffer[m_nGlyphIndex].fX;
			*pfY = m_pGlyphsBuffer[m_nGlyphIndex].fY;
		}
		*pvbSuccess = VARIANT_TRUE;

		return S_OK;
	}
	STDMETHOD(GetCharBBox)(float *pfLeft, float *pfTop, float *pfRight, float *pfBottom)
	{
		if ( !m_pGlyphsBuffer || m_nGlyphsCount <= 0 || m_nGlyphIndex < 0 || m_nGlyphIndex >= m_nGlyphsCount )
		{
			*pfBottom = 0;
			*pfRight  = 0;
			*pfLeft   = 0;
			*pfTop    = 0;

			return S_FALSE;
		}

		*pfBottom = m_pGlyphsBuffer[m_nGlyphIndex].fBottom;
		*pfRight  = m_pGlyphsBuffer[m_nGlyphIndex].fRight  + m_pGlyphsBuffer[m_nGlyphIndex].fX;
		*pfLeft   = m_pGlyphsBuffer[m_nGlyphIndex].fLeft   + m_pGlyphsBuffer[m_nGlyphIndex].fX;
		*pfTop    = m_pGlyphsBuffer[m_nGlyphIndex].fTop;

		return S_OK;
	}
	STDMETHOD(GetNextChar2)(BSTR *pbsTempPath, float *pfX, float *pfY, VARIANT_BOOL *pvbSuccess)
	{
		*pvbSuccess = VARIANT_FALSE;

		if ( m_nGlyphIndex >= m_nGlyphsCount )
		{
			CStringW wsTemp = _T("");
			*pbsTempPath = wsTemp.AllocSysString();
			*pfX     = -0xFFFF;
			*pfY     = -0xFFFF;

			return S_OK;
		}

		if ( !m_pFont || !m_pGlyphsBuffer )
			return S_FALSE;

		long lUnicode = m_pGlyphsBuffer[m_nGlyphIndex].lUnicode;

		int nX     = (int)floor( (double)m_pGlyphsBuffer[m_nGlyphIndex].fX );
		int nFracX = (int)floor( (double)(m_pGlyphsBuffer[m_nGlyphIndex].fX - nX) * FONT_FRACTION );
		int nY     = (int)floor( (double)m_pGlyphsBuffer[m_nGlyphIndex].fY );
		int nFracY = (int)floor( (double)(m_pGlyphsBuffer[m_nGlyphIndex].fY - nY) * FONT_FRACTION );

		TGlyphBitmap oBitmap;
		m_pFont->GetGlyph( lUnicode, nFracX, nFracY, &oBitmap );

		int nWidth  = oBitmap.nWidth;
		int nHeight = oBitmap.nHeight;

		CStringW wsFileName = _T( "C:\\Temp.bmp" );
		*pbsTempPath = wsFileName.AllocSysString();

		FILE *pFile = NULL;

		if ( !( pFile = ::_wfopen( wsFileName, _T("wb") ) ) ) 
			return S_FALSE;

		int nSize = 0, nStride;
		BYTE *pBuffer = NULL, *pBufPointer;
		BITMAPFILEHEADER oFileHeader;
		BITMAPINFOHEADER oInfoHeader;



		oFileHeader.bfType      = 'M' * 256 + 'B';
		oFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nHeight * nWidth * 4;
		oFileHeader.bfReserved1 = 0;
		oFileHeader.bfReserved2 = 0;
		oFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		oInfoHeader.biSize          = sizeof(BITMAPINFOHEADER);
		oInfoHeader.biWidth         = nWidth;
		oInfoHeader.biHeight        = nHeight;
		oInfoHeader.biPlanes        = 1;
		oInfoHeader.biBitCount      = 32;
		oInfoHeader.biCompression   = BI_RGB;
		oInfoHeader.biSizeImage     = nHeight * nWidth * 4;
		oInfoHeader.biXPelsPerMeter = 0;
		oInfoHeader.biYPelsPerMeter = 0;
		oInfoHeader.biClrUsed       = 0;
		oInfoHeader.biClrImportant  = 0;

		::fwrite( &oFileHeader,  1, sizeof(BITMAPFILEHEADER), pFile );
		::fwrite( &oInfoHeader,  1, sizeof(BITMAPINFOHEADER), pFile );

		nSize = nHeight * nWidth * 4;

		pBuffer = new BYTE[nSize];
		if ( !pBuffer )
		{
			return S_FALSE; 
		}
		pBufPointer = pBuffer;
		for ( int nY = 0; nY < nHeight; nY++ )
		{
			for ( int nX = 0; nX < nWidth; nX++, pBufPointer += 4 )
			{
				unsigned char nG = oBitmap.pData[nWidth * nY + nX ];
				float fAlpha = nG / 255.f;
				pBufPointer[0] = ( 1 - fAlpha ) * 216;
				pBufPointer[1] = ( 1 - fAlpha ) * 236;
				pBufPointer[2] = ( 1 - fAlpha ) * 236;
				pBufPointer[3] = 255 * fAlpha;
			}
		}
		pBufPointer = pBuffer;
		nStride = nWidth * 4;
		pBufPointer += (nHeight - 1) * nStride;
		nStride = -nStride;

		for( int nRow = nHeight; nRow > 0 ; --nRow, pBufPointer += nStride )
		{
			::fwrite( pBufPointer, 1, abs( nStride ), pFile );
		}
		delete []pBuffer;
		fclose( pFile );

		*pfX = m_pGlyphsBuffer[m_nGlyphIndex].fX - oBitmap.nX;
		*pfY = m_pGlyphsBuffer[m_nGlyphIndex].fY - oBitmap.nY;

		m_nGlyphIndex++;

		*pvbSuccess = VARIANT_TRUE;

		return S_OK;
	}
	STDMETHOD(MeasureString)(float *pfLeft, float *pfTop, float *pfRight, float *pfBottom)
	{
		if ( !m_pGlyphsBuffer || m_nGlyphsCount <= 0 )
		{
			*pfBottom = 0;
			*pfRight  = 0;
			*pfLeft   = 0;
			*pfTop    = 0;

			return S_FALSE;
		}

		// Учитываем, что мы используем перевернутю систему (т.е. матрица преобразования был [1 0 0 -1 0 0])
		// И то что сама система координат тоже перевернута
		float fTop = -0xFFFF, fBottom = 0xFFFF, fLeft = 0xFFFF, fRight = -0xFFFF;
		for ( int nIndex = 0; nIndex < m_nGlyphsCount; nIndex++ )
		{
			if ( fBottom > m_pGlyphsBuffer[nIndex].fBottom )
				fBottom = m_pGlyphsBuffer[nIndex].fBottom;

			if ( fTop < m_pGlyphsBuffer[nIndex].fTop )
				fTop = m_pGlyphsBuffer[nIndex].fTop;
		}

		fLeft  = m_pGlyphsBuffer[0].fLeft;
		fRight = m_pGlyphsBuffer[m_nGlyphsCount - 1].fRight + m_pGlyphsBuffer[m_nGlyphsCount - 1].fX;

		*pfBottom = fBottom;
		*pfRight  = fRight;
		*pfLeft   = fLeft;
		*pfTop    = fTop;

		return S_OK;
	}
	STDMETHOD(ApplyTransform)(float fA, float fB, float fC, float fD, float fE, float fF)
	{
		if ( !m_pFont )
		{
			return S_FALSE;
		}

		m_pFont->ResetTransform();
		m_pFont->ApplyTransform( fA, fB, fC, fD, fE, fF );

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
	STDMETHOD(GetWinFont)(long nIndex, BSTR *pbsFontName, BSTR *pbsFontStyle, BSTR *pbsFontPath, long *plIndex, BOOL *pbBold, BOOL *pbItalic)
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


};

