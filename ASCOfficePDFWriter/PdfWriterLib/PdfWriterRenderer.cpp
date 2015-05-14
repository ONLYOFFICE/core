#include "PdfWriterLib.h"

#if defined(_WIN32) || defined (_WIN64)
	#include <atlbase.h>
	#include <atlstr.h>
#else
    #include "../../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../../../DesktopEditor/graphics/Image.h"
//-------- Тип рендерера--------------------------------------------------------------------------
HRESULT CPdfWriterLib::get_Type (LONG *plType)
{
	if ( !plType )
		return S_FALSE;

	*plType = c_nPDFWriter;

	return S_OK;
}
//-------- Функции для работы со страницей -------------------------------------------------------
HRESULT CPdfWriterLib::NewPage ()
{
    m_bStartSubPath = false;
    m_bGStateChange = false;
    m_bClipPath     = false;

	m_dCharSpace    = 0.0;

	m_oClipCTM.Set( 1, 0, 0, 1, 0, 0 );
	m_oBaseTransform.Set( 1, 0, 0, 1, 0, 0 );

	m_oPen.SetDefaultParams();
	m_oFont.SetDefaultParams();
	m_oBrush.SetDefaultParams();
	m_oShadow.SetDefaultParams();
	m_oEdgeText.SetDefaultParams();

	m_oTransform.Reset();
	// Заканчиваем последнее состояние
	SetState( rendstateNone );
	// Сбрасываем состояние
	m_oRendererState.Reset();

    m_bFirstMoveTo = true;

	m_oContiniousText.Reset();

	return AddNewPage();
}
HRESULT CPdfWriterLib::put_Height (const double & dHeight)
{
	HRESULT hRes = S_OK;

     hRes = SetPageHeight( (float) MMToPDFCoords( dHeight )) ;
     if (hRes != S_OK)
		return hRes;

	return S_OK;
}
HRESULT CPdfWriterLib::get_Height (double *pdHeight)
{
	HRESULT hRes = S_OK;

	float fHeight = 0;
    hRes = GetHeight( &fHeight ) ;

    if (hRes != S_OK)
		return hRes;

	*pdHeight = PDFCoordsToMM( (double)fHeight );

	return S_OK;
}
HRESULT CPdfWriterLib::put_Width (const double & dWidth)
{
	HRESULT hRes = S_OK;

    hRes = SetPageWidth( (float)MMToPDFCoords( dWidth ) );

    if ( hRes != S_OK)
		return hRes;

	return S_OK;
}
HRESULT CPdfWriterLib::get_Width (double *pdWidth)
{
	HRESULT hRes = S_OK;

	float fWidth = 0;

    hRes = GetWidth( &fWidth );

    if ( hRes != S_OK)
		return hRes;

	*pdWidth = PDFCoordsToMM( (double)fWidth );

	return S_OK;
}
HRESULT CPdfWriterLib::get_DpiX (double *pdDpiX)
{
	*pdDpiX = 72.0;

	return S_OK;
}
HRESULT CPdfWriterLib::get_DpiY (double *pdDpiY)
{
	*pdDpiY = 72.0;

	return S_OK;
}
//-------- Функции для работы с Pen --------------------------------------------------------------
//HRESULT CPdfWriterLib::SetPen (std::wstring bsXML)
//{
//	CString strXML( bsXML );
//	m_oPen.FromXmlString( strXML );
//	m_bGStateChange = true;
//
//	return S_OK;
//}
HRESULT CPdfWriterLib::get_PenColor (LONG *plColor)
{
	*plColor = m_oPen.Color;

	return S_OK;
}
HRESULT CPdfWriterLib::put_PenColor (const LONG & lColor)
{
    m_bGStateChange = true;
	m_oPen.Color = lColor;

	return S_OK;
}
HRESULT CPdfWriterLib::get_PenAlpha (LONG *plAlpha)
{
	*plAlpha = m_oPen.Alpha;

	return S_OK;
}
HRESULT CPdfWriterLib::put_PenAlpha (const LONG & lAlpha)
{
    m_bGStateChange = true;
	m_oPen.Alpha = lAlpha;

	return S_OK;
}
HRESULT CPdfWriterLib::get_PenSize (double *pdSize)
{
	*pdSize = m_oPen.Size;

	return S_OK;
}
HRESULT CPdfWriterLib::put_PenSize (const double & dSize)
{
    m_bGStateChange = true;
	m_oPen.Size = dSize;

	return S_OK;
}
HRESULT CPdfWriterLib::get_PenDashStyle (BYTE *pnStyle)
{
	*pnStyle = m_oPen.DashStyle;

	return S_OK;
}
HRESULT CPdfWriterLib::put_PenDashStyle (const BYTE & nStyle)
{
    m_bGStateChange = true;
	m_oPen.DashStyle = nStyle;

	return S_OK;
}
HRESULT CPdfWriterLib::get_PenLineStartCap (BYTE *pnStartCap)
{
	*pnStartCap = m_oPen.LineStartCap;

	return S_OK;
}
HRESULT CPdfWriterLib::put_PenLineStartCap (const BYTE & nStartCap)
{
    m_bGStateChange = true;
	m_oPen.LineStartCap = nStartCap;

	return S_OK;
}
HRESULT CPdfWriterLib::get_PenLineEndCap (BYTE *pnEndCap)
{
	*pnEndCap = m_oPen.LineEndCap;

	return S_OK;
}
HRESULT CPdfWriterLib::put_PenLineEndCap (const BYTE & nEndCap)
{
    m_bGStateChange = true;
	m_oPen.LineEndCap = nEndCap;

	return S_OK;
}
HRESULT CPdfWriterLib::get_PenLineJoin (BYTE *pnJointStyle)
{
	*pnJointStyle = m_oPen.LineJoin;

	return S_OK;
}
HRESULT CPdfWriterLib::put_PenLineJoin (const BYTE & nJointStyle)
{
    m_bGStateChange = true;
	m_oPen.LineJoin = nJointStyle;

	return S_OK;
}
HRESULT CPdfWriterLib::get_PenDashOffset (double *pdOffset)
{
	*pdOffset = m_oPen.DashOffset;

	return S_OK;
}
HRESULT CPdfWriterLib::put_PenDashOffset (const double & dOffset)
{
    m_bGStateChange = true;
	m_oPen.DashOffset = dOffset;

	return S_OK;
}
HRESULT CPdfWriterLib::get_PenAlign (LONG *plAlign)
{
	*plAlign = m_oPen.Align;

	return S_OK;
}
HRESULT CPdfWriterLib::put_PenAlign (const LONG & lAlign)
{
    m_bGStateChange = true;
	m_oPen.Align = lAlign;

	return S_OK;
}
HRESULT CPdfWriterLib::get_PenMiterLimit (double *pdMiterLimit)
{
	*pdMiterLimit = m_oPen.MiterLimit;

	return S_OK;
}
HRESULT CPdfWriterLib::put_PenMiterLimit (const double & dMiterLimit)
{
    m_bGStateChange = true;
	m_oPen.MiterLimit = dMiterLimit;

	return S_OK;
}
HRESULT CPdfWriterLib::PenDashPattern (double * pPattern, LONG lCount)
{
    m_bGStateChange = true;
	m_oPen.SetDashPattern(  pPattern, lCount );

	return S_OK;
}
//-------- Функции для работы с Brush ------------------------------------------------------------
//HRESULT CPdfWriterLib::SetBrush (std::wstring bsXML)
//{
//	CString strXML( bsXML );
//	m_oBrush.FromXmlString( strXML );
//	m_bGStateChange = true;
//
//	return S_OK;
//}
HRESULT CPdfWriterLib::get_BrushType (LONG *plType)
{
	*plType = m_oBrush.Type;

	return S_OK;
}
HRESULT CPdfWriterLib::put_BrushType (const LONG & lType)
{
    m_bGStateChange = true;
	m_oBrush.Type = lType;

	return S_OK;
}
HRESULT CPdfWriterLib::get_BrushColor1 (LONG *plColor)
{
	*plColor = m_oBrush.Color1;

	return S_OK;
}
HRESULT CPdfWriterLib::put_BrushColor1 (const LONG & lColor)
{
    m_bGStateChange = true;
	m_oBrush.Color1 = lColor;

	return S_OK;
}
HRESULT CPdfWriterLib::get_BrushAlpha1 (LONG *plAlpha)
{
	*plAlpha = m_oBrush.Alpha1;

	return S_OK;
}
HRESULT CPdfWriterLib::put_BrushAlpha1 (const LONG &  lAlpha)
{
    m_bGStateChange = true;
	m_oBrush.Alpha1 = lAlpha;

	return S_OK;
}
HRESULT CPdfWriterLib::get_BrushColor2 (LONG *plColor)
{
	*plColor = m_oBrush.Color2;

	return S_OK;
}
HRESULT CPdfWriterLib::put_BrushColor2 (const LONG &  lColor)
{
    m_bGStateChange = true;
	m_oBrush.Color2 = lColor;

	return S_OK;
}
HRESULT CPdfWriterLib::get_BrushAlpha2 (LONG *plAlpha)
{
	*plAlpha = m_oBrush.Alpha2;

	return S_OK;
}
HRESULT CPdfWriterLib::put_BrushAlpha2 (const LONG &  lAlpha)
{
    m_bGStateChange = true;
	m_oBrush.Alpha2 = lAlpha;

	return S_OK;
}
HRESULT CPdfWriterLib::get_BrushTexturePath (std::wstring *pbsPath)
{
	*pbsPath = m_oBrush.TexturePath;

	return S_OK;
}
HRESULT CPdfWriterLib::put_BrushTexturePath (const std::wstring & bsPath)
{
    m_bGStateChange = true;
	m_oBrush.TexturePath = bsPath;

	return S_OK;
}
HRESULT CPdfWriterLib::get_BrushTextureMode (LONG *plMode)
{
	*plMode = m_oBrush.TextureMode;

	return S_OK;
}
HRESULT CPdfWriterLib::put_BrushTextureMode (const LONG &  lMode)
{
    m_bGStateChange = true;
	m_oBrush.TextureMode = lMode;

	if (c_BrushTextureModeTile == m_oBrush.TextureMode)
        m_oPatternState.SetTileMode(true);

	return S_OK;
}
HRESULT CPdfWriterLib::get_BrushTextureAlpha (LONG *plTxAlpha)
{
	*plTxAlpha = m_oBrush.TextureAlpha;

	return S_OK;
}
HRESULT CPdfWriterLib::put_BrushTextureAlpha (const LONG &  lTxAlpha)
{
    m_bGStateChange = true;
	m_oBrush.TextureAlpha = lTxAlpha;

	return S_OK;
}
HRESULT CPdfWriterLib::get_BrushLinearAngle (double *pdAngle)
{
	*pdAngle = m_oBrush.LinearAngle;

	return S_OK;
}
HRESULT CPdfWriterLib::put_BrushLinearAngle (const double & dAngle)
{
    m_bGStateChange = true;
	m_oBrush.LinearAngle = dAngle;

	return S_OK;
}
HRESULT CPdfWriterLib::BrushRect (const INT & bVal, const double & dLeft, const double & dTop, const double & dWidth, const double & dHeight)
{
	m_oBrush.Rectable    = bVal;
	m_oBrush.Rect.X      = (float)dLeft;
	m_oBrush.Rect.Y      = (float)dTop;
	m_oBrush.Rect.Width  = (float)dWidth;
	m_oBrush.Rect.Height = (float)dHeight;

	return S_OK;
}
HRESULT CPdfWriterLib::BrushBounds(const double& left, const double& top, const double& width, const double& height)
{
	m_oBrush.Bounds.left	= left;
	m_oBrush.Bounds.top		= top;
	m_oBrush.Bounds.right	= left + width;
	m_oBrush.Bounds.bottom	= top + height;
	return S_OK;
}
//-------- Функции для работы с Шрифтом ----------------------------------------------------------
//HRESULT CPdfWriterLib::SetFont (std::wstring bsXML)
//{
//	CString strXML( bsXML );
//	m_oFont.FromXmlString( strXML );
//
//	//if ( m_bCurFontGIDString && !m_bEncoderGID )
//	//{
//	//	bool bNew = false;
//
//	//	wchar_t wsString[255];
//	//	for ( int nCode = 1; nCode < 256; nCode++ )
//	//	{
//	//		wsString[nCode - 1] = nCode;
//	//	}
//	//	FindEncoderForString( m_pDocument, (std::wstring)wsString ,&bNew);
//	//}
//	//unsigned long unRes = GdiFontToPdfFont(/*L""*/(CStringW(L"qwertyuiop[]';lkjhgfdsazxcvbnm,./QWERTYUIOP{}\":LKJHGFDSAZXCVBNM<>?ЙЦУКЕНГШЩЗХЪЭЖДЛОРПАВЫФЯЧСМИТЬБЮ,.юбьтимсчяфывапролджэъхзщшгнекуцйЁё1234567890-=+_ (*?:%;№!qwertyuiop[]';lkjhgfdsazxcvbnm,./QWERTYUIOP{}\":LKJHGFDSAZXCVBNM<>?ЙЦУКЕНГ ")).AllocSysString(), true);
//	//if ( S_OK != unRes )
//	//	return unRes;
//
//	//if ( !m_pFontEncodingList )
//	//	m_pFontEncodingList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
//
//	//TFontEncodingPair *pPair = new TFontEncodingPair;
//	//const char *sPdfFontName  = FontGetFontName( m_pCurrentFont );
//	//const char *sRealFontName = FontGetFontName( m_pCurrentFont, true );
//	//const char *sEncodingName = FontGetEncodingName( m_pCurrentFont );
//
//	//UtilsMemCpy( (BYTE *)pPair->sPdfFontName,  (BYTE *)sPdfFontName,  LIMIT_MAX_NAME_LEN + 1 );
//	//UtilsMemCpy( (BYTE *)pPair->sRealFontName, (BYTE *)sRealFontName, LIMIT_MAX_NAME_LEN + 1 );
//	//UtilsMemCpy( (BYTE *)pPair->sEncodingName, (BYTE *)sEncodingName, LIMIT_MAX_NAME_LEN + 1 );
//
//	//ListAdd( m_pFontEncodingList, pPair );
//
//	return S_OK;
//
//}
HRESULT CPdfWriterLib::get_FontName (std::wstring *pbsName)
{
	*pbsName = m_oFont.Name;

	return S_OK;
}
HRESULT CPdfWriterLib::put_FontName (const std::wstring & bsName)
{
	m_oFont.Name = bsName;

	return S_OK;
}
HRESULT CPdfWriterLib::get_FontPath (std::wstring *pbsPath)
{
	*pbsPath = m_oFont.Path;

	return S_OK;
}
HRESULT CPdfWriterLib::put_FontPath (const std::wstring & bsName)
{		
	m_oFont.Path = bsName ;

	//todooo переписать нужно ... типо OOX:CPath  
	string_replaceAll(m_oFont.Path, _T("\\\\")	, _T("\\"));
	string_replaceAll(m_oFont.Path, _T("\\\\")	, _T("\\"));

	string_replaceAll(m_oFont.Path, _T("//")	, _T("/"));	
	string_replaceAll(m_oFont.Path, _T("//")	, _T("/"));

	return S_OK;
}
HRESULT CPdfWriterLib::get_FontSize (double *pdSize)
{
	*pdSize = m_oFont.Size;

	return S_OK;
}
HRESULT CPdfWriterLib::put_FontSize (const double & dSize)
{
	m_oFont.Size = dSize;

	return S_OK;
}
HRESULT CPdfWriterLib::get_FontStyle (LONG *plStyle)
{
	*plStyle = m_oFont.GetStyle();

	return S_OK;
}
HRESULT CPdfWriterLib::put_FontStyle (const LONG &  lStyle)
{
	m_oFont.SetStyle( lStyle );
	return S_OK;
}
HRESULT CPdfWriterLib::get_FontStringGID (INT *pbGID)
{
	*pbGID = m_oFont.StringGID;

	return S_OK;
}
HRESULT CPdfWriterLib::put_FontStringGID (const INT & bGID)
{
	m_oFont.StringGID = bGID;

	return S_OK;
}
HRESULT CPdfWriterLib::get_FontCharSpace (double *pdSpace)
{
	if ( !pdSpace )
		return S_FALSE;

	*pdSpace = PDFCoordsToMM( m_dCharSpace );

	return S_OK;
}
HRESULT CPdfWriterLib::put_FontCharSpace (const double & dSpace)
{
	m_dCharSpace = MMToPDFCoords( dSpace );
	return S_OK;
}
HRESULT CPdfWriterLib::get_FontFaceIndex(int* lFaceIndex)
{
	*lFaceIndex = m_oFont.FaceIndex;
	return S_OK;
}
HRESULT CPdfWriterLib::put_FontFaceIndex(const int& lFaceIndex)
{
	m_oFont.FaceIndex = lFaceIndex;
	return S_OK;
}

HRESULT CPdfWriterLib::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
{
	return S_OK;
}
//-------- Функции для работы с Shadow -----------------------------------------------------------
//HRESULT CPdfWriterLib::SetShadow (std::wstring bsXML)
//{
//	CString strXML( bsXML );
//	m_oShadow.FromXmlString( strXML );
//
//	return S_OK;
//}
HRESULT CPdfWriterLib::get_ShadowDistanceX (double *pdX)
{
	*pdX = m_oShadow.DistanceX;

	return S_OK;
}
HRESULT CPdfWriterLib::put_ShadowDistanceX (const double &dX)
{
	m_oShadow.DistanceX = dX;

	return S_OK;
}
HRESULT CPdfWriterLib::get_ShadowDistanceY (double *pdY)
{
	*pdY = m_oShadow.DistanceY;

	return S_OK;
}
HRESULT CPdfWriterLib::put_ShadowDistanceY (const double &dY)
{
	m_oShadow.DistanceY = dY;

	return S_OK;
}
HRESULT CPdfWriterLib::get_ShadowBlurSize (double *pdSize)
{
	*pdSize = m_oShadow.BlurSize;

	return S_OK;
}
HRESULT CPdfWriterLib::put_ShadowBlurSize (const double &dSize)
{
	m_oShadow.BlurSize = dSize;

	return S_OK;
}
HRESULT CPdfWriterLib::get_ShadowColor (LONG *plColor)
{
	*plColor = m_oShadow.Color;

	return S_OK;
}
HRESULT CPdfWriterLib::put_ShadowColor (const LONG &lColor)
{
	m_oShadow.Color = lColor;

	return S_OK;
}
HRESULT CPdfWriterLib::get_ShadowAlpha (LONG *plAlpha)
{
	*plAlpha = m_oShadow.Alpha;

	return S_OK;
}
HRESULT CPdfWriterLib::put_ShadowAlpha (const LONG &lAlpha)
{
	m_oShadow.Alpha = lAlpha;

	return S_OK;
}
HRESULT CPdfWriterLib::get_ShadowVisible (bool *pbVisible)
{
	*pbVisible = m_oShadow.Visible;

	return S_OK;
}
HRESULT CPdfWriterLib::put_ShadowVisible (const bool &bVisible)
{
	m_oShadow.Visible = bVisible;

	return S_OK;
}
//-------- Функции для работы с Edge Text --------------------------------------------------------
//HRESULT CPdfWriterLib::SetEdgeText (std::wstring bsXML)
//{
//	CString strXML( bsXML );
//	m_oEdgeText.FromXmlString( strXML );
//
//	return S_OK;
//}
HRESULT CPdfWriterLib::get_EdgeVisible (LONG *plVisible)
{
	*plVisible = m_oEdgeText.Visible;

	return S_OK;
}
HRESULT CPdfWriterLib::put_EdgeVisible (const LONG &lVisible)
{
	m_oEdgeText.Visible = lVisible;

	return S_OK;
}
HRESULT CPdfWriterLib::get_EdgeColor (LONG *plColor)
{
	*plColor = m_oEdgeText.Color;

	return S_OK;
}
HRESULT CPdfWriterLib::put_EdgeColor (const LONG &lColor)
{
	m_oEdgeText.Color = lColor;

	return S_OK;
}
HRESULT CPdfWriterLib::get_EdgeAlpha (LONG *plAlpha)
{
	*plAlpha = m_oEdgeText.Alpha;

	return S_OK;
}
HRESULT CPdfWriterLib::put_EdgeAlpha (const LONG &lAlpha)
{
	m_oEdgeText.Alpha = lAlpha;

	return S_OK;
}
HRESULT CPdfWriterLib::get_EdgeDist (double *pdDist)
{
	*pdDist = m_oEdgeText.Dist;

	return S_OK;
}
HRESULT CPdfWriterLib::put_EdgeDist (const double &dDist)
{
	m_oEdgeText.Dist = dDist;

	return S_OK;
}
//--------- Функции для вывода текста ------------------------------------------------------------
HRESULT CPdfWriterLib::CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset)
{
	//if (c_nHyperlinkType == m_lCurrentCommandType)
	//	return S_OK;
	//put_BrushType(c_BrushTypeSolid);
	//	
	//_SetFont();

	//Aggplus::CBrush* pBrush = CreateBrush(&m_oBrush);				
	//m_pRenderer->DrawStringC(c, m_pFontManager, pBrush, x, y + baselineOffset);
	//
	//RELEASEOBJECT(pBrush);

	return S_OK;
}
HRESULT CPdfWriterLib::CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
{
	if (gid >= 0)
	{
        m_oFont.StringGID = true;
		return CommandDrawTextCHAR(gid, x, y, w, h, baselineOffset);
	}
	
    m_oFont.StringGID = false;
	return CommandDrawTextCHAR(c, x, y, w, h, baselineOffset);
}


HRESULT CPdfWriterLib::CommandDrawText (const std::wstring & bsText,const double & fX,const  double &fY, 
										const double & fWidth, const double & fHeight, const double & fBaseLineOffset)
{
	HRESULT hRes = S_OK;

	// Работа с сылками тоже проходит с помощью функции CommandDrawText
	if ( c_nHyperlinkType == m_pCurCommand->nType )
	{
		SetState( rendstateNone );

        hRes = MakeAnnotationFromXml( std_string2string(bsText));
        if ( hRes != S_OK)
			return hRes;

		return S_OK;
	}

	if ( c_nClipType != m_pCurCommand->nType )
	{
		m_oContiniousText.Add( std_string2string(bsText), std_string2string(bsText), 
			fX, fY, fWidth, fHeight, fBaseLineOffset, false, m_oFont, m_oBrush, m_oTransform, m_dCharSpace );
		SetState( rendstateText );

		return S_OK;
	}
	else
	{
		SetState( rendstateClipText );
	}


	//CSynchAccess oSynchAccess = m_hSynchMutex;

	// Работа с сылками тоже проходит с помощью функции CommandDrawText
	if ( c_nHyperlinkType == m_pCurCommand->nType )
	{
        hRes = MakeAnnotationFromXml( std_string2string(bsText));
        if ( hRes != S_OK)
			return hRes;

		return S_OK;
	}

	// Переводим в миллиметры
	double fX1				= MMToPDFCoords( fX );
	double fY1				= MMToPDFCoords( fY );
	
	double fWidth1			= MMToPDFCoords( fWidth );
	double fHeight1			= MMToPDFCoords( fHeight );
	double fBaseLineOffset1	= MMToPDFCoords( fBaseLineOffset );

	PToUnicode pToUnicode = FindToUnicodeForString( m_pDocument, bsText );
	if ( !pToUnicode )
		return S_OK;

    std::wstring bsCodedString;
    bool bResult = ToUnicodeWCharToString( pToUnicode, bsText, bsCodedString );

	// Запоминаем значения Bold и Italic, потому что в функции GdiFontToPdfFont они могут измениться
    bool bOldBold   = m_oFont.Bold;
    bool bOldItalic = m_oFont.Italic;

    bool bDefaultFont = false;

	CString name(pToUnicode->sName);//ansi to unicode todooo

    hRes = GdiFontToPdfFont2( name , _T("") ) ;
    if ( hRes != S_OK)
	{
		if ( AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND == hRes )
		{
			ResetError( m_pDocument );
            bsCodedString = std_string2string( bsText );
            bDefaultFont = true;
		}
		else
		{
			return hRes;
		}
	}

    long nAscent = 0;
	long nDescent = 0;
	long nXHeight = 0;

	double dItalicAngle = 3.141592f / 12;

	float fPageHeight = 0.0f;

    hRes = GetHeight( &fPageHeight );

    if ( hRes != S_OK)
		return hRes;

	if ( !m_oFont.Italic )
		dItalicAngle = 0;

	float fFontSize = (float)m_oFont.Size;

    hRes = GetFontAscent( &nAscent );
    if ( hRes != S_OK)
		return hRes;

    hRes = GetFontDescent( &nDescent );
    if ( hRes != S_OK)
		return hRes;

    hRes = GetFontXHeight( &nXHeight );
    if ( hRes != S_OK)
		return hRes;

	// Выясним как нам рисовать текст
	ETextRenderingMode eRenderingMode = ETextRenderingMode::Fill; // Fill (по умолчанию)
	if ( c_nStrokeTextType == m_pCurCommand->nType )
		eRenderingMode = ETextRenderingMode::Stroke; // Stroke
	if ( c_nClipType == m_pCurCommand->nType )
	{
//		ATLTRACE2( _T( "TextClip" ) );
		eRenderingMode = ETextRenderingMode::Clipping; // Clip
	}

	float fXHeight = (float)nXHeight / 1000;
	float fAscent  = (float)nAscent / 1000;
	float fDescent = fabs( (float)nDescent / 1000 );

	long nColor1 = m_oBrush.Color1;
	float fR = ( nColor1 & 0xFF ) / 255.0f; 
	float fG = ( ( nColor1 >> 8 ) & 0xFF ) / 255.0f; 
	float fB = ( nColor1 >> 16 ) / 255.0f; 
	float fTextWidth = 0.0f;
	float fLineWidth = fFontSize / ( bOldBold ? 10.f : 15.0f );
	float fOldLineWidth = 0.0f;

    hRes = GetLineWidth( &fOldLineWidth );
    if ( hRes != S_OK)
		return hRes;

	float fFillR, fFillG, fFillB, fStrokeR, fStrokeG, fStrokeB;
    hRes = GetRGBFill( &fFillR, &fFillG, &fFillB );
    if ( hRes != S_OK)
		return hRes;

    hRes = GetRGBStroke( &fStrokeR, &fStrokeG, &fStrokeB );
    if ( hRes != S_OK)
		return hRes;
	// Рисуем тени

	if ( m_oShadow.Visible )
	{
        //if ( S_OK != ( hRes = GRestore() ) )
		//{
		//	::SysFreeString( bsCodedString );
		//	return hRes;
		//}
        //ATLTRACE2( _T("GSave: DrawText - Shadow\n") );

        hRes = GSave();

        if ( hRes != S_OK)
			return hRes;

        hRes = SetLineWidth( fLineWidth );
        if ( hRes != S_OK)
			return hRes;

		// Выставляем систему координат
        hRes = UpdateCoordSpace(  );
        if ( hRes != S_OK)
			return hRes;


		double dXShadow = fX1 + MMToPDFCoords( m_oShadow.DistanceX );
		double dYShadow = fY1 + MMToPDFCoords( m_oShadow.DistanceY );

		long nAlpha			= m_oShadow.Alpha;
		long nShadowColor	= m_oShadow.Color;

		float fAlpha = (float)nAlpha / 255.0f;
		float fBShadow = ( nShadowColor & 0xFF ) / 255.0f; 
		float fGShadow = ( ( nShadowColor >> 8 ) & 0xFF ) / 255.0f; 
		float fRShadow = ( nShadowColor >> 16 ) / 255.0f; 

		{
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin( _T("ExtGState") );

			if ( 255 != fAlpha )
			{
                oWriter.WriteNodeBegin( _T("AlphaFill"), true );
				oWriter.WriteAttribute( _T("value"), (double)fAlpha );
                oWriter.WriteNodeEnd( _T("AlphaFill"), true, true );
			}

			if ( 255 != fAlpha )
			{
                oWriter.WriteNodeBegin( _T("AlphaStroke"), true );
				oWriter.WriteAttribute( _T("value"), (double)fAlpha );
                oWriter.WriteNodeEnd( _T("AlphaStroke"), true, true );
			}

			oWriter.WriteNodeEnd( _T("ExtGState") );

			CString bsXml = oWriter.GetXmlString();
            hRes = SetExtGraphicsState2( bsXml );

            if ( hRes != S_OK)
                return hRes;

            //if ( S_OK != ( hRes = NewExtGraphicsState() ) )
			//{
			//	::SysFreeString( bsCodedString );
			//	return hRes;
			//}
            ////if ( S_OK != ( hRes = ExtGraphicsStateSetBlendMode( EBlendMode::BMOverlay ) ) )
			////{
			////	::SysFreeString( bsCodedString );
			////	return hRes;
			////}
            //if ( S_OK != ( hRes = ExtGraphicsStateSetAlphaFill( fAlpha ) ) )
			//{
			//	::SysFreeString( bsCodedString );
			//	return hRes;
			//}
            //if ( S_OK != ( hRes = ExtGraphicsStateSetAlphaStroke( fAlpha ) ) )
			//{
			//	::SysFreeString( bsCodedString );
			//	return hRes;
			//}
            //if ( S_OK != ( hRes = SetExtGraphicsState() ) )
			//{
			//	::SysFreeString( bsCodedString );
			//	return hRes;
			//}
		}

		// Выставляем цвет текста и линий
        hRes = SetRGBFill( fRShadow, fGShadow, fBShadow );
        if ( hRes != S_OK)
			return hRes;

        hRes = SetRGBStroke( fRShadow, fGShadow, fBShadow );
        if ( hRes != S_OK)
            return hRes;

        hRes = BeginText();
        if ( hRes != S_OK)
            return hRes;

		// Устанавливаем текущий фонт и размер фонта
        hRes = SetFontAndSizeToGState( fFontSize ) ;
        if ( hRes != S_OK)
			return hRes;

		// Если данный фонт не нашли с атрибутом Italic, тогда наколняем текст сами на угол dItalicAngle
        hRes = SetTextMatrix( 1, (float)tan( 0.0 ), (float)tan( dItalicAngle ), 1, 0, 0 );
        if ( hRes != S_OK)
			return hRes;

        hRes = GetTextWidth( bsCodedString, &fTextWidth );
        if ( hRes != S_OK)
			return hRes;

		if ( eRenderingMode == ETextRenderingMode::Fill  )
		{
			// Если данный фонт не нашли с атрибутом Bold, тогда заливаем и обводим текст, тем самым утолщяя его
			if ( m_oFont.Bold )
			{
                hRes = SetTextRenderingMode((long)ETextRenderingMode::FillThenStroke) ;
                if ( hRes != S_OK)
					return hRes;
			}
			else
                hRes = SetTextRenderingMode((long)ETextRenderingMode::Fill);
                if ( hRes != S_OK)
					return hRes;
		}
		else 
		{
            hRes = SetTextRenderingMode( (long)eRenderingMode ) ;
            if ( hRes != S_OK)
				return hRes;
		}

		// Выводим текст
        hRes = TextOut_( (float)dXShadow, fPageHeight - (float) (dYShadow + fBaseLineOffset), bsCodedString ) ;
        if ( hRes != S_OK)
			return hRes;


        hRes = EndText() ;
        if ( hRes != S_OK)
			return hRes;

		//// Перечеркивание текста. Текст перечеркиваем по середине буквы 'x'. (См. XHeight)
		//if ( m_oFont.Strikeout )
		//{
		//	double dItalicAdd = tan( (double)dItalicAngle ) * fXHeight * fFontSize / 2;
		//	if ( !m_oFont.Italic )
		//		dItalicAdd = 0;
        //	if ( S_OK != ( hRes = MoveTo( (float)dXShadow + dItalicAdd, fPageHeight - (float) ( dYShadow + fBaseLineOffset - fXHeight * fFontSize / 2 ) ) ) )
		//	{
		//		::SysFreeString( bsCodedString );
		//		return hRes;
		//	}
        //	if ( S_OK != ( hRes = LineTo( (float)dXShadow + fTextWidth + dItalicAdd, fPageHeight - (float) ( dYShadow + fBaseLineOffset - fXHeight * fFontSize / 2 ) ) ) )
		//	{
		//		::SysFreeString( bsCodedString );
		//		return hRes;
		//	}
        //	if ( S_OK != ( hRes = Stroke( ) ) )
		//	{
		//		::SysFreeString( bsCodedString );
		//		return hRes;
		//	}
		//}

		//// Подчеркивание текста.
		//if ( m_oFont.Underline )
		//{
		//	float fTextWidth = 0.0f;
		//	float fUnderLineOffset = 3 + fLineWidth / 2;
        //	if ( S_OK != ( hRes = GetTextWidth( bsCodedString, &fTextWidth ) ) )
		//	{
		//		::SysFreeString( bsCodedString );
		//		return hRes;
		//	}
        //	if ( S_OK != ( hRes = MoveTo( (float)dXShadow, fPageHeight - ( (float)dYShadow + fBaseLineOffset + fUnderLineOffset /*fDescent * fFontSize*/ ) ) ) )
		//	{
		//		::SysFreeString( bsCodedString );
		//		return hRes;
		//	}
        //	if ( S_OK != ( hRes = LineTo( (float)dXShadow + fTextWidth, fPageHeight - ( (float)dYShadow + fBaseLineOffset + fUnderLineOffset /*fDescent * fFontSize*/ ) ) ) )
		//	{
		//		::SysFreeString( bsCodedString );
		//		return hRes;
		//	}
        //	if ( S_OK != ( hRes = Stroke( ) ) )
		//	{
		//		::SysFreeString( bsCodedString );
		//		return hRes;
		//	}
		//}

//		ATLTRACE2( _T("GRestore: DrawText - Shadow\n") );
        hRes = GRestore();
        if ( hRes != S_OK)
			return hRes;
	}

    //if ( S_OK != ( hRes = GRestore() ) )
	//{
	//	::SysFreeString( bsCodedString );
	//	return hRes;
	//}

	// Дальше рисуем все, что непосредственно связанно с самим текстом
	// Выставляем цвет текста и линий

	if ( ETextRenderingMode::Fill == eRenderingMode )
	{
        hRes = SetRGBFill( fR, fG, fB ) ;
        if ( hRes != S_OK)
			return hRes;

        hRes = SetRGBStroke( fR, fG, fB ) ;
        if ( hRes != S_OK)
			return hRes;
	}
	else
	{
        hRes = SetRGBFill( fR, fG, fB ) ;
        if ( hRes != S_OK)
			return hRes;

        hRes = SetRGBStroke( fR, fG, fB ) ;
			return hRes;
	}

	// Выставляем систему координат
	if ( c_nClipType != m_pCurCommand->nType )
	{
//		ATLTRACE2( _T("GSave: DrawText\n") );
        hRes = GSave() ;
        if ( hRes != S_OK)
			return hRes;

        hRes = UpdateCoordSpace(  ) ;
        if ( hRes != S_OK)
			return hRes;

        float fAlphaFill   = (float)m_oBrush.Alpha1 / 255.0f;
		float fAlphaStroke = (float)m_oPen.Alpha / 255.0f;
		m_pCurrentExtGState = GetExtGState( m_pDocument, fAlphaStroke, fAlphaFill );
        if ( S_OK != ( hRes = PageSetExtGState( m_pDocument->pCurPage, m_pCurrentExtGState ) ) )
			return hRes;
	}
	else
	{
        if ( S_OK != ( hRes = UpdateCoordSpace(  ) ) )
		{
			return hRes;
		}
	}

    if ( S_OK != ( hRes = BeginText() ) )
	{
		return hRes;
	}

	// Устанавливаем межсимвольный интервал
    if ( S_OK != ( hRes = SetCharSpace( (float)m_dCharSpace ) ) )
	{
		return hRes;
	}

	// Устанавливаем текущий фонт и размер фонта
    if ( S_OK != ( hRes = SetFontAndSizeToGState( fFontSize ) ) )
	{
		return hRes;
	}

	// Если шрифт не нашли, и используется стандартный шрифт, значит, ширина слов будет неверной.
	// Чтобы слова не наползали друг на друга скейлим текст к его реальной ширине.
	if ( bDefaultFont )
	{
		// Измеряем размер
		float fTextWidth = 0.0f;
        if ( S_OK != ( hRes = GetTextWidth( bsCodedString, &fTextWidth ) ) )
		{
			return hRes;
		}

		float fKoef;
		if ( fabs( fTextWidth ) < 0.01 )
			fKoef = 1;
		else
			fKoef = fWidth / fTextWidth * 100; // Scale задается в процентах

		if ( fKoef > 0.001 )
		{
            if ( S_OK != ( hRes = SetHorizontalScalling( fKoef ) ) )
			{
				return hRes;
			}
		}
	}

	if ( _T("AVSEmptyFont") == m_oFont.Name )
	{
		// Данная ветка сделана специально для конвертации Djvu -> PDF

		float fVKoef;

		if ( fabs( m_oFont.Size ) < 0.01 )
			fVKoef = 1;
		else		
			fVKoef = fHeight / m_oFont.Size;

		// Если данный фонт не нашли с атрибутом Italic, тогда наколняем текст сами на угол dItalicAngle
        if ( S_OK != ( hRes = SetTextMatrix( 1, (float)tan( 0.0 ), (float)fVKoef * tan( dItalicAngle ), fVKoef * 1, 0, 0 ) ) )
		{
			return hRes;
		}
	}
	else
	{
		// Если данный фонт не нашли с атрибутом Italic, тогда наколняем текст сами на угол dItalicAngle
        if ( S_OK != ( hRes = SetTextMatrix( 1, (float)tan( 0.0 ), (float)tan( dItalicAngle ), 1, 0, 0 ) ) )
		{
			return hRes;
		}
	}

    if ( S_OK != ( hRes = GetTextWidth( bsCodedString, &fTextWidth ) ) )
	{
		return hRes;
	}

	if ( ETextRenderingMode::Fill == eRenderingMode )
	{
		// Если данный фонт не нашли с атрибутом Bold, тогда заливаем и обводим текст, тем самым утолщяя его
		if ( m_oFont.Bold )
		{
            if ( S_OK != ( hRes = SetTextRenderingMode((long)ETextRenderingMode::FillThenStroke) ) )
			{
				return hRes;
			}
		}
		else
            if ( S_OK != ( hRes = SetTextRenderingMode((long)ETextRenderingMode::Fill) ) )
			{
				return hRes;
			}
	}
	else
	{
        if ( S_OK != ( hRes = SetTextRenderingMode( (long)eRenderingMode ) ) )
		{
			return hRes;
		}
	}

	// Выводим текст
    if ( S_OK != ( hRes = TextOut_( (float)fX, fPageHeight - (float) (fY + fBaseLineOffset), bsCodedString )))
		return hRes;

    if ( S_OK != ( hRes = EndText() ) )
	{
		return hRes;
	}

    //if ( S_OK != ( hRes = SetLineWidth( fLineWidth ) ) )
	//{
	//	return hRes;
	//}

	//// Перечеркивание текста. Текст перечеркиваем по середине буквы 'x'. (См. XHeight)
	//if ( m_oFont.Strikeout )
	//{
	//	double dItalicAdd = tan( (double)dItalicAngle ) * fXHeight * fFontSize / 2;
	//	if ( !m_oFont.Italic )
	//		dItalicAdd = 0;
    //	if ( S_OK != ( hRes = MoveTo( (float)fX + dItalicAdd, fPageHeight - (float) ( fY + fBaseLineOffset - fXHeight * fFontSize / 2 ) ) ) )
	//	{
	//		return hRes;
	//	}
    //	if ( S_OK != ( hRes = LineTo( (float)fX + fTextWidth + dItalicAdd, fPageHeight - (float) ( fY + fBaseLineOffset - fXHeight * fFontSize / 2 ) ) ) )
	//	{
	//		return hRes;
	//	}
    //	if ( S_OK != ( hRes = Stroke( ) ) )
	//	{
	//		return hRes;
	//	}
	//	//MoveTo(dX, dY - fDescent * fFontSize );
	//	//LineTo(dX + dItalicAdd * 2, dY + fFontSize * ( 1 - fDescent ) );
	//	//LineTo(dX + fTextWidth + dItalicAdd * 2, dY + fFontSize * ( 1 - fDescent ) );
	//	//LineTo(dX + fTextWidth, dY - fDescent * fFontSize );
	//	//LineTo(dX, dY - fDescent * fFontSize );
	//	//Stroke();
	//}

	//// Подчеркивание текста.
	//if ( m_oFont.Underline )
	//{
	//	float fTextWidth = 0.0f;
	//	float fUnderLineOffset = 3 + fLineWidth / 2;
    //	if ( S_OK != ( hRes = GetTextWidth( bsCodedString, &fTextWidth ) ) )
	//	{
	//		::SysFreeString( bsCodedString );
	//		return hRes;
	//	}
    //	if ( S_OK != ( hRes = MoveTo( (float)fX, fPageHeight - (float) (fY + fBaseLineOffset + fUnderLineOffset) /*fDescent * fFontSize*/ ) ) )
	//	{
	//		::SysFreeString( bsCodedString );
	//		return hRes;
	//	}
    //	if ( S_OK != ( hRes = LineTo( (float)fX + fTextWidth, fPageHeight - (float) (fY + fBaseLineOffset + fUnderLineOffset) /*fDescent * fFontSize*/ ) ) )
	//	{
	//		::SysFreeString( bsCodedString );
	//		return hRes;
	//	}
    //	if ( S_OK != ( hRes = Stroke( ) ) )
	//	{
	//		::SysFreeString( bsCodedString );
	//		return hRes;
	//	}
	//}


	// Восстанавливаем систему координат
	if ( c_nClipType != m_pCurCommand->nType )
	{
//		ATLTRACE2( _T("GRestore: DrawText\n") );
        if ( S_OK != ( hRes = GRestore() ) )
			return hRes;
	}
	else
	{
		float fDelta = m_oClipCTM.fA * m_oClipCTM.fD - m_oClipCTM.fB * m_oClipCTM.fC;
		if ( 0.0f != fDelta  )
		{
			float fIDelta = 1 / fDelta;
			float fA =  fIDelta * m_oClipCTM.fD;
			float fB =  fIDelta * ( -m_oClipCTM.fB );
			float fC =  fIDelta * ( -m_oClipCTM.fC );
			float fD =  fIDelta * m_oClipCTM.fA;
			float fE =  fIDelta * (m_oClipCTM.fC * m_oClipCTM.fF - m_oClipCTM.fE * m_oClipCTM.fD);
			float fF =  fIDelta * (m_oClipCTM.fE * m_oClipCTM.fB - m_oClipCTM.fA * m_oClipCTM.fF);

			// Делаем матрицу CTM единичной
            if ( S_OK != ( hRes = Concat( fA, fB, fC, fD, fE, fF ) ) )
				return hRes;
		}

		m_oClipCTM.Set( 1, 0, 0, 1, 0, 0 );
	}

	// Восстанавливаем старые настройки
    if ( S_OK != ( hRes = SetLineWidth( fOldLineWidth ) ) )
		return hRes;

    if ( S_OK != ( hRes = SetRGBFill( fFillR, fFillG, fFillB ) ) )
		return hRes;

    if ( S_OK != ( hRes = SetRGBStroke( fStrokeR, fStrokeG, fStrokeB ) ) )
		return hRes;

	m_oFont.Bold   = bOldBold;
	m_oFont.Italic = bOldItalic;

	return S_OK;
}
HRESULT CPdfWriterLib::CommandDrawTextEx (const std::wstring & bsUnicodeText,const std::wstring & bsGidText,
							/*const std::wstring & bsSrcCodeText, */const double & fX1,const  double & fY1, 
							const double & fWidth1, const double & fHeight1, const double & fBaseLineOffset1,const  DWORD & nFlags)
{
	// TODO: Сделать обработку DrawTextEx тоже через Состояния рендерера
	SetState( rendstateNone );

	if ( bsUnicodeText.length()<1/* && !bsSrcCodeText*/ )
		return S_OK;

	//CSynchAccess oSynchAccess = m_hSynchMutex;

	HRESULT hRes = S_OK;

	// Работа с сылками тоже проходит с помощью функции CommandDrawText
	if ( c_nHyperlinkType == m_pCurCommand->nType )
	{
        if ( S_OK != ( hRes = MakeAnnotationFromXml( std_string2string(bsUnicodeText) ) ) )
			return hRes;

		return S_OK;
	}

	// Переводим в миллиметры
	double fX = MMToPDFCoords( fX1 );
	double fY = MMToPDFCoords( fY1 );

	double fWidth = MMToPDFCoords( fWidth1 );
	double fHeight = MMToPDFCoords( fHeight1 );

	double fBaseLineOffset = MMToPDFCoords( fBaseLineOffset1 );

	std::wstring bsCodedString;

	// Запоминаем значения Bold и Italic, потому что в функции GdiFontToPdfFont они могут измениться
    bool bOldBold   = m_oFont.Bold;
    bool bOldItalic = m_oFont.Italic;
    bool bDefaultFont = false;
//todooo непонятка с заданием имени пути к файлу bsSrcCodeText !!! ?????

	// Проверяем как задан шрифт, через путь к файлу, или по имени
	//if ( _T("") != m_oFont.Path )
	//{
		//XmlUtils::CXmlNode oNode;
		//oNode.FromXmlString( CString(bsSrcCodeText) ); //?????

		//// TO DO: Пока если шрифт задан путем, значит он пришел из PDF, как только
		////        появится другой формат, с внедренными шрифтами, доделать тут.
		//if ( _T("PDF-Text") == oNode.GetName() )
		//{
		//	// Загружаем шрифт и кодировки
        //	if ( S_OK != ( hRes = FontFromFileToPdfFont() ) )
		//	{
		//		return hRes;
		//	}

		//	int nCode = 0, nCID = 0;
		//	if ( FontDefCID == ((FontAttr)m_pCurrentFont->pAttr)->pFontDef->eType )
		//	{
		//		int nCID;

		//		nCID  = XmlUtils::GetInteger( oNode.GetAttribute( _T("cid") ) );
		//		nCode = XmlUtils::GetInteger( oNode.GetAttribute( _T("code") ) );
		//		int nLen = XmlUtils::GetInteger( oNode.GetAttribute( _T("len") ) );

		//		char pBuffer[4];

		//		pBuffer[0] = (char) (nCode >> 24);
		//		pBuffer[1] = (char) (nCode >> 16);
		//		pBuffer[2] = (char) (nCode >> 8);
		//		pBuffer[3] = (char) nCode;


		//		CMapEncoderStreamAddCIDtoCode( ((FontAttr)m_pCurrentFont->pAttr)->pEncoder, nCID, pBuffer ,nLen );

		//		bsCodedString = std::wstring(( wchar_t )(nCID));
		//	}
		//	else
		//	{
		//		nCode = XmlUtils::GetInteger( oNode.GetAttribute( _T("code") ) );

		//		bsCodedString = std::wstring( wchar_t(nCode) );
		//	}

		//}
	//	else 
	//	{
	//		return S_OK;
	//	}
	//}
	//else
	{
		PToUnicode pToUnicode = FindToUnicodeForString( m_pDocument, bsUnicodeText );
		if ( !pToUnicode )
			return S_OK;

        std::wstring sCodedString;
        bool bResult = ToUnicodeWCharToString( pToUnicode, bsUnicodeText, sCodedString );

		CString name(pToUnicode->sName);//ansi to unicode todooo
		//todooo
        if ( S_OK != ( hRes = GdiFontToPdfFont2( name, _T("") ) ) )
		{
			if ( AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND == hRes )
			{
				ResetError( m_pDocument );
				sCodedString = std_string2string( bsUnicodeText );
                bDefaultFont = true;
			}
			else
			{
				return hRes;
			}
		}

        bsCodedString = sCodedString;
	}


	long nAscent = 0;
	long nDescent = 0;
	long nXHeight = 0;

	double dItalicAngle = 3.141592f / 12;

	float fPageHeight = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
	{
		return hRes;
	}

	if ( !m_oFont.Italic )
		dItalicAngle = 0;

	float fFontSize = (float)m_oFont.Size;

    if ( S_OK != ( hRes = GetFontAscent( &nAscent ) ) )
	{
		return hRes;
	}

    if ( S_OK != ( hRes = GetFontDescent( &nDescent ) ) )
	{
		return hRes;
	}

    if ( S_OK != ( hRes = GetFontXHeight( &nXHeight ) ) )
	{
		return hRes;
	}

	// Выясним как нам рисовать текст
	ETextRenderingMode eRenderingMode = ETextRenderingMode::Fill; // Fill (по умолчанию)
	if ( c_nStrokeTextType == m_pCurCommand->nType )
		eRenderingMode = ETextRenderingMode::Stroke; // Stroke
	if ( c_nClipType == m_pCurCommand->nType )
	{
//		ATLTRACE2( _T( "TextClip" ) );
		eRenderingMode = ETextRenderingMode::Clipping; // Clip
	}

	float fXHeight = (float)nXHeight / 1000;
	float fAscent  = (float)nAscent / 1000;
	float fDescent = fabs( (float)nDescent / 1000 );

	long nColor1 = m_oBrush.Color1;
	float fR = ( nColor1 & 0xFF ) / 255.0f; 
	float fG = ( ( nColor1 >> 8 ) & 0xFF ) / 255.0f; 
	float fB = ( nColor1 >> 16 ) / 255.0f; 
	float fTextWidth = 0.0f;
	float fLineWidth = fFontSize / ( bOldBold ? 10.f : 15.0f );
	float fOldLineWidth = 0.0f;

    if ( S_OK != ( hRes = GetLineWidth( &fOldLineWidth ) ) )
	{
		return hRes;
	}

	float fFillR, fFillG, fFillB, fStrokeR, fStrokeG, fStrokeB;
    if ( S_OK != ( hRes = GetRGBFill( &fFillR, &fFillG, &fFillB ) ) )
	{
		return hRes;
	}

    if ( S_OK != ( hRes = GetRGBStroke( &fStrokeR, &fStrokeG, &fStrokeB ) ) )
	{
		return hRes;
	}
	// Рисуем тени

	if ( m_oShadow.Visible )
	{
        //if ( S_OK != ( hRes = GRestore() ) )
		//{
		//	return hRes;
		//}
//		ATLTRACE2( _T("GSave: DrawText - Shadow\n") );
        if ( S_OK != ( hRes = GSave() ) )
		{
			return hRes;
		}

        if ( S_OK != ( hRes = SetLineWidth( fLineWidth ) ) )
		{
			return hRes;
		}

		// Выставляем систему координат
        if ( S_OK != ( hRes = UpdateCoordSpace(  ) ) )
		{
			return hRes;
		}


		double dXShadow = fX + MMToPDFCoords( m_oShadow.DistanceX );
		double dYShadow = fY + MMToPDFCoords( m_oShadow.DistanceY );

		long nAlpha			= m_oShadow.Alpha;
		long nShadowColor	= m_oShadow.Color;

		float fAlpha	= (float)nAlpha / 255.0f;
		float fBShadow	= ( nShadowColor & 0xFF ) / 255.0f; 
		float fGShadow	= ( ( nShadowColor >> 8 ) & 0xFF ) / 255.0f; 
		float fRShadow	= ( nShadowColor >> 16 ) / 255.0f; 

		{
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin( _T("ExtGState") );

			if ( 255 != fAlpha )
			{
                oWriter.WriteNodeBegin( _T("AlphaFill"), true );
				oWriter.WriteAttribute( _T("value"), (double)fAlpha );
                oWriter.WriteNodeEnd( _T("AlphaFill"), true, true );
			}

			if ( 255 != fAlpha )
			{
                oWriter.WriteNodeBegin( _T("AlphaStroke"), true );
				oWriter.WriteAttribute( _T("value"), (double)fAlpha );
                oWriter.WriteNodeEnd( _T("AlphaStroke"), true, true );
			}

			oWriter.WriteNodeEnd( _T("ExtGState") );

			CString bsXml = oWriter.GetXmlString();
            if ( S_OK != ( hRes = SetExtGraphicsState2( bsXml ) ) )
			{
				return hRes;
			}

            //if ( S_OK != ( hRes = NewExtGraphicsState() ) )
			//{
			//	::SysFreeString( bsCodedString );
			//	return hRes;
			//}
            ////if ( S_OK != ( hRes = ExtGraphicsStateSetBlendMode( EBlendMode::BMOverlay ) ) )
			////{
			////	::SysFreeString( bsCodedString );
			////	return hRes;
			////}
            //if ( S_OK != ( hRes = ExtGraphicsStateSetAlphaFill( fAlpha ) ) )
			//{
			//	::SysFreeString( bsCodedString );
			//	return hRes;
			//}
            //if ( S_OK != ( hRes = ExtGraphicsStateSetAlphaStroke( fAlpha ) ) )
			//{
			//	::SysFreeString( bsCodedString );
			//	return hRes;
			//}
            //if ( S_OK != ( hRes = SetExtGraphicsState() ) )
			//{
			//	::SysFreeString( bsCodedString );
			//	return hRes;
			//}

		}

		// Выставляем цвет текста и линий
        if ( S_OK != ( hRes = SetRGBFill( fRShadow, fGShadow, fBShadow ) ) )
		{
			return hRes;
		}

        if ( S_OK != ( hRes = SetRGBStroke( fRShadow, fGShadow, fBShadow ) ) )
		{
			return hRes;
		}

        if ( S_OK != ( hRes = BeginText() ) )
		{
			return hRes;
		}

		// Устанавливаем текущий фонт и размер фонта
        if ( S_OK != ( hRes = SetFontAndSizeToGState( fFontSize ) ) )
		{
			return hRes;
		}

		// Если данный фонт не нашли с атрибутом Italic, тогда наколняем текст сами на угол dItalicAngle
        if ( S_OK != ( hRes = SetTextMatrix( 1, (float)tan( 0.0 ), (float)tan( dItalicAngle ), 1, 0, 0 ) ) )
		{
			return hRes;
		}

        if ( S_OK != ( hRes = GetTextWidth( bsCodedString, &fTextWidth ) ) )
		{
			return hRes;
		}

		if ( eRenderingMode == ETextRenderingMode::Fill  )
		{
			// Если данный фонт не нашли с атрибутом Bold, тогда заливаем и обводим текст, тем самым утолщяя его
			if ( m_oFont.Bold )
			{
                if ( S_OK != ( hRes = SetTextRenderingMode((long)ETextRenderingMode::FillThenStroke) ) )
				{
					return hRes;
				}
			}
			else
                if ( S_OK != ( hRes = SetTextRenderingMode((long)ETextRenderingMode::Fill) ) )
				{
					return hRes;
				}
		}
		else 
		{
            if ( S_OK != ( hRes = SetTextRenderingMode( (long)eRenderingMode ) ) )
			{
				return hRes;
			}
		}

		// Выводим текст
        if ( S_OK != ( hRes = TextOut_( (float)dXShadow, fPageHeight - (float) (dYShadow + fBaseLineOffset), bsCodedString ) ) )
		{
			return hRes;
		}


        if ( S_OK != ( hRes = EndText() ) )
		{
			return hRes;
		}

		// Перечеркивание текста. Текст перечеркиваем по середине буквы 'x'. (См. XHeight)
		if ( m_oFont.Strikeout )
		{
			double dItalicAdd = tan( (double)dItalicAngle ) * fXHeight * fFontSize / 2;
			if ( !m_oFont.Italic )
				dItalicAdd = 0;
            if ( S_OK != ( hRes = MoveTo( (float)dXShadow + dItalicAdd, fPageHeight - (float) ( dYShadow + fBaseLineOffset - fXHeight * fFontSize / 2 ) ) ) )
			{
				return hRes;
			}
            if ( S_OK != ( hRes = LineTo( (float)dXShadow + fTextWidth + dItalicAdd, fPageHeight - (float) ( dYShadow + fBaseLineOffset - fXHeight * fFontSize / 2 ) ) ) )
			{
				return hRes;
			}
            if ( S_OK != ( hRes = Stroke( ) ) )
			{
				return hRes;
			}
		}

		// Подчеркивание текста.
		if ( m_oFont.Underline )
		{
			float fTextWidth = 0.0f;
			float fUnderLineOffset = 3 + fLineWidth / 2;
            if ( S_OK != ( hRes = GetTextWidth( bsCodedString, &fTextWidth ) ) )
			{
				return hRes;
			}
            if ( S_OK != ( hRes = MoveTo( (float)dXShadow, fPageHeight - ( (float)dYShadow + fBaseLineOffset + fUnderLineOffset /*fDescent * fFontSize*/ ) ) ) )
			{
				return hRes;
			}
            if ( S_OK != ( hRes = LineTo( (float)dXShadow + fTextWidth, fPageHeight - ( (float)dYShadow + fBaseLineOffset + fUnderLineOffset /*fDescent * fFontSize*/ ) ) ) )
			{
				return hRes;
			}
            if ( S_OK != ( hRes = Stroke( ) ) )
			{
				return hRes;
			}
		}

//		ATLTRACE2( _T("GRestore: DrawText - Shadow\n") );
        if ( S_OK != ( hRes = GRestore() ) )
		{
			return hRes;
		}
	}

    //if ( S_OK != ( hRes = GRestore() ) )
	//{
	//	::SysFreeString( bsCodedString );
	//	return hRes;
	//}

	// Дальше рисуем все, что непосредственно связанно с самим текстом
	// Выставляем цвет текста и линий

	if ( ETextRenderingMode::Fill == eRenderingMode )
	{
        if ( S_OK != ( hRes = SetRGBFill( fR, fG, fB ) ) )
		{
			return hRes;
		}

        if ( S_OK != ( hRes = SetRGBStroke( fR, fG, fB ) ) )
		{
			return hRes;
		}
	}
	else
	{
        if ( S_OK != ( hRes = SetRGBFill( fR, fG, fB ) ) )
		{
			return hRes;
		}


        if ( S_OK != ( hRes = SetRGBStroke( fR, fG, fB ) ) )
		{
			return hRes;
		}
	}

	// Выставляем систему координат
	if ( c_nClipType != m_pCurCommand->nType )
	{
//		ATLTRACE2( _T("GSave: DrawText\n") );
        if ( S_OK != ( hRes = GSave() ) )
		{
			return hRes;
		}
        if ( S_OK != ( hRes = UpdateCoordSpace(  ) ) )
		{
			return hRes;
		}
	}
	else
	{
        if ( S_OK != ( hRes = UpdateCoordSpace(  ) ) )
		{
			return hRes;
		}
	}

    if ( S_OK != ( hRes = BeginText() ) )
	{
		return hRes;
	}

	// Устанавливаем текущий фонт и размер фонта
    if ( S_OK != ( hRes = SetFontAndSizeToGState( fFontSize ) ) )
	{
		return hRes;
	}

	// Если шрифт не нашли, и используется стандартный шрифт, значит, ширина слов будет неверной.
	// Чтобы слова не наползали друг на друга скейлим текст к его реальной ширине.
	if ( bDefaultFont )
	{
		// Измеряем размер
		float fTextWidth = 0.0f;
        if ( S_OK != ( hRes = GetTextWidth( bsCodedString, &fTextWidth ) ) )
		{
			return hRes;
		}

		float fKoef = fWidth / fTextWidth * 100; // Scale задается в процентах

		if ( fKoef > 0.001 )
		{
            if ( S_OK != ( hRes = SetHorizontalScalling( fKoef ) ) )
			{
				return hRes;
			}
		}
	}

	// Если данный фонт не нашли с атрибутом Italic, тогда наколняем текст сами на угол dItalicAngle
    if ( S_OK != ( hRes = SetTextMatrix( 1, (float)tan( 0.0 ), (float)tan( dItalicAngle ), 1, 0, 0 ) ) )
	{
		return hRes;
	}

    if ( S_OK != ( hRes = GetTextWidth( bsCodedString, &fTextWidth ) ) )
	{
		return hRes;
	}

	if ( ETextRenderingMode::Fill == eRenderingMode )
	{
		// Если данный фонт не нашли с атрибутом Bold, тогда заливаем и обводим текст, тем самым утолщяя его
		if ( m_oFont.Bold )
		{
            if ( S_OK != ( hRes = SetTextRenderingMode((long)ETextRenderingMode::FillThenStroke) ) )
			{
				return hRes;
			}
		}
		else
            if ( S_OK != ( hRes = SetTextRenderingMode((long)ETextRenderingMode::Fill) ) )
			{
				return hRes;
			}
	}
	else
	{
        if ( S_OK != ( hRes = SetTextRenderingMode( (long)eRenderingMode ) ) )
		{
			return hRes;
		}
	}

	// Выводим текст
    if ( S_OK != ( hRes = TextOut_( (float)fX, fPageHeight - (float) (fY + fBaseLineOffset), bsCodedString ) ) )
	{
		return hRes;
	}

    if ( S_OK != ( hRes = EndText() ) )
	{
		return hRes;
	}

    if ( S_OK != ( hRes = SetLineWidth( fLineWidth ) ) )
	{
		return hRes;
	}

	// Перечеркивание текста. Текст перечеркиваем по середине буквы 'x'. (См. XHeight)
	if ( m_oFont.Strikeout )
	{
		double dItalicAdd = tan( (double)dItalicAngle ) * fXHeight * fFontSize / 2;
		if ( !m_oFont.Italic )
			dItalicAdd = 0;
        if ( S_OK != ( hRes = MoveTo( (float)fX + dItalicAdd, fPageHeight - (float) ( fY + fBaseLineOffset - fXHeight * fFontSize / 2 ) ) ) )
		{
			return hRes;
		}
        if ( S_OK != ( hRes = LineTo( (float)fX + fTextWidth + dItalicAdd, fPageHeight - (float) ( fY + fBaseLineOffset - fXHeight * fFontSize / 2 ) ) ) )
		{
			return hRes;
		}
        if ( S_OK != ( hRes = Stroke( ) ) )
		{
			return hRes;
		}
		//MoveTo(dX, dY - fDescent * fFontSize );
		//LineTo(dX + dItalicAdd * 2, dY + fFontSize * ( 1 - fDescent ) );
		//LineTo(dX + fTextWidth + dItalicAdd * 2, dY + fFontSize * ( 1 - fDescent ) );
		//LineTo(dX + fTextWidth, dY - fDescent * fFontSize );
		//LineTo(dX, dY - fDescent * fFontSize );
		//Stroke();
	}

	// Подчеркивание текста.
	if ( m_oFont.Underline )
	{
		float fTextWidth = 0.0f;
		float fUnderLineOffset = 3 + fLineWidth / 2;
        if ( S_OK != ( hRes = GetTextWidth( bsCodedString, &fTextWidth ) ) )
		{
			return hRes;
		}
        if ( S_OK != ( hRes = MoveTo( (float)fX, fPageHeight - (float) (fY + fBaseLineOffset + fUnderLineOffset) /*fDescent * fFontSize*/ ) ) )
		{
			return hRes;
		}
        if ( S_OK != ( hRes = LineTo( (float)fX + fTextWidth, fPageHeight - (float) (fY + fBaseLineOffset + fUnderLineOffset) /*fDescent * fFontSize*/ ) ) )
		{
			return hRes;
		}
        if ( S_OK != ( hRes = Stroke( ) ) )
		{
			return hRes;
		}
	}

	// Восстанавливаем систему координат
	if ( c_nClipType != m_pCurCommand->nType )
	{
//		ATLTRACE2( _T("GRestore: DrawText\n") );
        if ( S_OK != ( hRes = GRestore() ) )
			return hRes;
	}
	else
	{
		float fDelta = m_oClipCTM.fA * m_oClipCTM.fD - m_oClipCTM.fB * m_oClipCTM.fC;
		if ( 0.0f != fDelta  )
		{
			float fIDelta = 1 / fDelta;
			float fA =  fIDelta * m_oClipCTM.fD;
			float fB =  fIDelta * ( -m_oClipCTM.fB );
			float fC =  fIDelta * ( -m_oClipCTM.fC );
			float fD =  fIDelta * m_oClipCTM.fA;
			float fE =  fIDelta * (m_oClipCTM.fC * m_oClipCTM.fF - m_oClipCTM.fE * m_oClipCTM.fD);
			float fF =  fIDelta * (m_oClipCTM.fE * m_oClipCTM.fB - m_oClipCTM.fA * m_oClipCTM.fF);

			// Делаем матрицу CTM единичной
            if ( S_OK != ( hRes = Concat( fA, fB, fC, fD, fE, fF ) ) )
				return hRes;
		}

		m_oClipCTM.Set( 1, 0, 0, 1, 0, 0 );
	}

	// Восстанавливаем старые настройки
    if ( S_OK != ( hRes = SetLineWidth( fOldLineWidth ) ) )
		return hRes;

    if ( S_OK != ( hRes = SetRGBFill( fFillR, fFillG, fFillB ) ) )
		return hRes;

    if ( S_OK != ( hRes = SetRGBStroke( fStrokeR, fStrokeG, fStrokeB ) ) )
		return hRes;

	m_oFont.Bold   = bOldBold;
	m_oFont.Italic = bOldItalic;

	return S_OK;
}
//--------- Маркеры для команд -------------------------------------------------------------------
HRESULT CPdfWriterLib::BeginCommand (const DWORD & nFlag)
{
#if defined(_WIN32) || defined(_WIN64)
    wchar_t sTemp[32];
	_itow(nFlag, sTemp, 10);

	if (c_nPathType == nFlag)
        ATLTRACE2(_T("Begin") + CString(_T(" - PathType")) + _T("\n"));
	else if (c_nClipType == nFlag)
		ATLTRACE2(_T("Begin") + CString(_T(" - ClipType")) + _T("\n"));
	else if (c_nResetClipType == nFlag)
		ATLTRACE2(_T("Begin") + CString(_T(" - ResetClipType")) + _T("\n"));
	else if ( c_nPageType == nFlag )
		ATLTRACE2(_T("Begin") + CString(_T(" - PageType")) + _T("\n"));
	else
		ATLTRACE2(_T("Begin") + CString(sTemp) + _T("\n"));
#endif
	if ( nFlag == c_nTableCell || nFlag == c_nHeader || nFlag == c_nColumnCount || nFlag == c_nFootnote
		|| nFlag == c_nShape || nFlag == c_nShapeGroup || nFlag == c_nShapeWrap || nFlag == c_nTable 
		|| nFlag == c_nTableXml || nFlag == c_nBlock || nFlag == c_nBlockHeadingLevel || nFlag == c_nBlockNumberingLevel
		|| nFlag == c_nBaselineShift 
		)
		return S_OK;

	TCommandType *pNewCommand = new TCommandType();
	pNewCommand->nType = nFlag;
	pNewCommand->pPrev = m_pCurCommand;
	m_pCurCommand = pNewCommand;

	HRESULT hRes = S_OK;
	if ( c_nPathType == nFlag )
	{
		SetState( rendstatePath );
		// Сейчас начинаем рисовать Path

		m_BlockCommandsStart = ((PageAttr)m_pDocument->pCurPage->pAttr)->pStream->nSize;		
		
//		ATLTRACE2( _T("GSave: PathType\n") );
        if ( S_OK != ( hRes = UpdateGState() ) )
			return hRes;

		int c = 0;
	}
	else if ( c_nClipType == nFlag )
	{
		// Запоминаем состояние без обрезающего пата
        if ( false == m_bClipPath )
		{
//			ATLTRACE2( _T("GSave: ClipPathType\n") );
            if ( S_OK != ( hRes = GSave() ) )
				return hRes;

            m_bClipPath = true;
		}
	}
	else if ( c_nResetClipType == nFlag )
	{
		// Откатываемся к состоянию без обрезающего пата
        if ( true == m_bClipPath )
		{
			SetState( rendstateClipReset );

//			ATLTRACE2( _T("GRestore: ClipPathType\n") );
            if ( S_OK != ( hRes = GRestore() ) )
				return hRes;

			m_oClipCTM.Set( 1, 0, 0, 1, 0, 0 );

            m_bClipPath = false;
		}
	}

	return S_OK;
}
HRESULT CPdfWriterLib::EndCommand (const DWORD & nFlag)
{
#if defined(_WIN32) || defined(_WIN64)
    wchar_t sTemp[32];
    _itow(nFlag, sTemp, 10);

    if (c_nPathType == nFlag)
        ATLTRACE2( _T("End") + CString( _T(" - PathType") ) + _T("\n") );
    else if (c_nPageType == nFlag)
        ATLTRACE2( _T("End") + CString( _T(" - PageType") ) + _T("\n") );
    else if (c_nResetClipType == nFlag)
        ATLTRACE2( _T("End") + CString( _T(" - ResetClipType") ) + _T("\n") );
    else if ( c_nClipType == nFlag )
        ATLTRACE2( _T("End") + CString( _T(" - ClipType") ) + _T("\n") );
    else
        ATLTRACE2( _T("End") + CString( sTemp ) + _T("\n") );
#endif

	if (nFlag == c_nTableCell || nFlag == c_nHeader || nFlag == c_nColumnCount || nFlag == c_nFootnote
		|| nFlag == c_nShape || nFlag == c_nShapeGroup || nFlag == c_nShapeWrap || nFlag == c_nTable 
		|| nFlag == c_nTableXml || nFlag == c_nBlock || nFlag == c_nBlockHeadingLevel || nFlag == c_nBlockNumberingLevel || nFlag == c_nBaselineShift)
	{
		return S_OK;
	}

	if ( !m_pCurCommand || nFlag != m_pCurCommand->nType  )
		return AVS_OFFICEPDFWRITER_ERROR_BAD_COMMAND_TYPE;

	HRESULT hRes = S_OK;
	if (c_nPathType == nFlag)
	{
		// Если данный пат набивался как клип, тогда мы должны подать команду о завершении
		if ( NULL != m_pCurCommand->pPrev && c_nClipType == m_pCurCommand->pPrev->nType )
		{ 
			if ( c_nClipRegionTypeWinding == m_pCurCommand->nFlag )
			{
                if ( S_OK != ( hRes = Clip() ) )
					return hRes;
			}
			else if ( c_nClipRegionTypeEvenOdd == m_pCurCommand->nFlag )
			{
                if ( S_OK != ( hRes = EoClip() ) )
					return hRes;
			}

            if ( S_OK != ( hRes = EndPath() ) )
				return hRes;

			float fDelta = m_oClipCTM.fA * m_oClipCTM.fD - m_oClipCTM.fB * m_oClipCTM.fC;
			if ( 0.0f != fDelta  )
			{
				float fIDelta = 1 / fDelta;
				float fA =  fIDelta * m_oClipCTM.fD;
				float fB =  fIDelta * ( -m_oClipCTM.fB );
				float fC =  fIDelta * ( -m_oClipCTM.fC );
				float fD =  fIDelta * m_oClipCTM.fA;
				float fE =  fIDelta * (m_oClipCTM.fC * m_oClipCTM.fF - m_oClipCTM.fE * m_oClipCTM.fD);
				float fF =  fIDelta * (m_oClipCTM.fE * m_oClipCTM.fB - m_oClipCTM.fA * m_oClipCTM.fF);

				// Делаем матрицу CTM единичной
                if ( S_OK != ( hRes = Concat( fA, fB, fC, fD, fE, fF ) ) )
					return hRes;
			}

		}
		else
		{
//			ATLTRACE2( _T("GRestore: PathType\n") );
            if ( S_OK != ( hRes = GRestore() ) )
				return hRes;
		}
	}
	else if ( c_nPageType == nFlag )
	{
		SetState( rendstateNone );
	}

	TCommandType *pPrev = m_pCurCommand->pPrev;
	delete m_pCurCommand;
	m_pCurCommand = pPrev;

	return S_OK;
}
//--------- Функции для работы с Graphics Path ---------------------------------------------------
HRESULT CPdfWriterLib::PathCommandStart ()
{
//	ATLTRACE2( _T("Start\n") );

	if ( c_BrushTypeTexture == m_oBrush.Type && m_pCurPath || (m_oPatternState.IsUse() && m_pCurPath))
		m_pCurPath->StartFigure();

	//std::wstring bsError;
	//GetLastErrorString(&bsError);
	//HRESULT hRes = S_OK;
    //if ( S_OK !=  ( hRes = EndPath() ) )
	//	return hRes;
    m_bStartSubPath = true;
	return S_OK;
}	
HRESULT CPdfWriterLib::PathCommandEnd ()
{
//	ATLTRACE2( _T("End\n") );

	if (c_BrushTypeTexture == m_oBrush.Type && m_pCurPath || (m_oPatternState.IsUse() && m_pCurPath))
		m_pCurPath->CloseFigure();

	//HRESULT hRes = S_OK;
    //if ( S_OK !=  ( hRes = EndPath() ) )
	//	return hRes;
    //m_bStartSubPath = true;

	// NOTE : решение хардкорное 

	// градиент имеет прозрачность, поэтому все команды перемещаем в форму
	// TODO : переписать запись команд таким образом что бы они сыпались во временный объект

	if (m_MaskWithFormCommands.GetLength())
	{
		if (m_XForm)
		{
			StreamRecPtr stream			=	((PageAttr)m_pDocument->pCurPage->pAttr)->pStream;
			unsigned int bufferSize =	stream->nSize - m_BlockCommandsStart;
			if (bufferSize && stream)
			{
				CStringA str;
				CHAR* buffer		=	str.GetBuffer(bufferSize + 1);
				if (buffer)
				{
					GRestore();

					StreamSeek(stream, m_BlockCommandsStart, WhenceMode::SeekSet);
					StreamRead(stream, (BYTE*)buffer, &bufferSize);
					StreamSeek(stream, m_BlockCommandsStart, WhenceMode::SeekWrite);

					buffer[bufferSize] = L'\0';

					CString data(str);
					m_XForm->SetStream(data);

					// Example : q /s13 gs /x14 Do Q
					// к /x объекту применяется маска /s

					StreamWriteStr(stream, CStringA(m_MaskWithFormCommands));

					if (m_IsStrokeShape)
					{
					
						if (m_strPatternFill.GetLength())	data.Replace(m_strPatternFill, L"");
						if (m_strPatternStroke.GetLength()) data.Replace(m_strPatternStroke, L"");
						
						data.Replace(L"B\012", L"S\012");
						data.Replace(L"f\012", L"s\012");
					
						data.Replace(L"B*\012", L"S*\012");
						data.Replace(L"f*\012", L"s*\012");

						if (-1 == data.Find(L"Q\012"))		data += L"Q\012";

						StreamWriteStr(stream, CStringA(data));
					}

					str.ReleaseBuffer();
				}
			}
		}

		m_BlockCommandsStart	=	0;			

		m_XForm					=	NULL;
		m_MaskWithFormCommands	=	L"";
        m_IsStrokeShape			=	false;
		
		m_strPatternFill		=	L"";
		m_strPatternStroke		=	L"";
	}

	return S_OK;
}
//
HRESULT CPdfWriterLib::PathCommandMoveTo (const double &fX1,  const double &fY1)
{
//	ATLTRACE2( _T("Move To\n") );

	HRESULT hRes = S_OK;

    if ( true == m_bFirstMoveTo )
	{
		// Текущее состояние - Path
		SetState( /*m_bClip ? rendstateClipPath :*/ rendstatePath );

        m_bFirstMoveTo = false;
	}

	if ( !m_bStartSubPath || m_oPatternState.IsUse())
	{
		if ( c_BrushTypeTexture == m_oBrush.Type || m_oPatternState.IsUse())
		{
			if (m_oPatternState.IsUse())
			{
				if (NULL == m_pCurPath)
				{
					m_pCurPath = new Aggplus::CGraphicsPath();
					m_pCurPath->StartFigure(); //create было todoooo
				}
			}
			else 
			{
				if (m_pCurPath)
					m_pCurPath->_Close();//???? было Delete
				else
					m_pCurPath = new Aggplus::CGraphicsPath();

				m_pCurPath->StartFigure();
			}
		}
	}

	if ( c_BrushTypeTexture == m_oBrush.Type && m_pCurPath || (m_oPatternState.IsUse() && m_pCurPath))
		m_pCurPath->MoveTo( fX1, fY1 );

	double fX = MMToPDFCoords( fX1 );
	double fY = MMToPDFCoords( fY1 );

    m_bStartSubPath = false;
	float fPageHeight = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;
    if ( S_OK != ( hRes = MoveTo( fX, fPageHeight - fY ) ) )
		return hRes;
	return S_OK;
}
HRESULT CPdfWriterLib::PathCommandLineTo (const double &fX1,  const double &fY1)
{
//	ATLTRACE2( _T("Line To\n") );

	if ( c_BrushTypeTexture == m_oBrush.Type && m_pCurPath || (m_oPatternState.IsUse() && m_pCurPath))
		m_pCurPath->LineTo( fX1, fY1 );

	double fX = MMToPDFCoords( fX1 );
	double fY = MMToPDFCoords( fY1 );

	HRESULT hRes = S_OK;
	float fPageHeight = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;
    if ( S_OK != ( hRes = LineTo( fX, fPageHeight - fY ) ) )
		return hRes;
	return S_OK;
}
HRESULT CPdfWriterLib::PathCommandLinesTo (double* points, const int& count)
{
//	ATLTRACE2( _T("Lines To\n") );

	if ( c_BrushTypeTexture == m_oBrush.Type && m_pCurPath || (m_oPatternState.IsUse() && m_pCurPath))
		m_pCurPath->AddLines(points, count );


	HRESULT hRes = S_OK;
	float fPageHeight = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;

	VARTYPE vType;

	//double* pArray = (double* (pPoints)->pvData;
	//int nPointsCount = (pPoints)->rgsabound->cElements / 2;

	if ( m_bStartSubPath )
	{
        //if ( S_OK != ( hRes = GRestore() ) )
		//	return hRes;			
        if ( S_OK != ( hRes = PathCommandMoveTo( (points)[0],  (points)[1] ) ) )
			return hRes;			
	}
	double *points1 = points + 2;
	int count1 = count -1;

	for ( int nIndex = 0; nIndex < count1; nIndex++, points1 += 2 )
        if ( S_OK != ( hRes = LineTo( MMToPDFCoords( points1[0] ), fPageHeight - MMToPDFCoords( points1[1] ) ) ) )
			return hRes;

	return S_OK;
}
HRESULT CPdfWriterLib::PathCommandCurveTo (const double &fX1, const double &fY1, const double &fX2, const double &fY2,
										   const double &fX3, const double &fY3)
{
//	ATLTRACE2( _T("Curve To\n") );

	if ( c_BrushTypeTexture == m_oBrush.Type && m_pCurPath|| (m_oPatternState.IsUse() && m_pCurPath) )
		m_pCurPath->CurveTo( fX1, fY1, fX2, fY2, fX3, fY3 );

	HRESULT hRes = S_OK;
	float fPageHeight = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;
    if ( S_OK != ( hRes = CurveTo( MMToPDFCoords( fX1 ), fPageHeight - MMToPDFCoords( fY1 ), MMToPDFCoords( fX2 ), fPageHeight - MMToPDFCoords( fY2 ), MMToPDFCoords( fX3 ), fPageHeight - MMToPDFCoords( fY3 ) ) ) )
		return hRes;
	return S_OK;
}
HRESULT CPdfWriterLib::PathCommandCurvesTo (double* points, const int& count)
{
//	ATLTRACE2( _T("Curves To\n") );

	if ( c_BrushTypeTexture == m_oBrush.Type && m_pCurPath || (m_oPatternState.IsUse() && m_pCurPath))
		m_pCurPath->AddCurve( points, count );

	HRESULT hRes = S_OK;
	float fPageHeight = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;

	double* pArray = points;
	int nPointsCount = count;

	if ( m_bStartSubPath )
	{
        //if ( S_OK != ( hRes = GRestore() ) )
		//	return hRes;			
        if ( S_OK != ( hRes = PathCommandMoveTo( pArray[0], pArray[1] ) ) )
			return hRes;			
	}

	pArray += 2;
	nPointsCount--;

	for ( int nIndex = 0; nIndex < nPointsCount / 3; nIndex++, pArray += 6 )
	{
        if ( S_OK != ( hRes = CurveTo( MMToPDFCoords( pArray[0] ), fPageHeight - MMToPDFCoords( pArray[1] ), MMToPDFCoords( pArray[2] ), fPageHeight - MMToPDFCoords( pArray[3] ), MMToPDFCoords( pArray[4] ), fPageHeight - MMToPDFCoords( pArray[5] ) ) ) )
			return hRes;
	}
	return S_OK;
}
HRESULT CPdfWriterLib::PathCommandArcTo (const double &fX1, const double &fY1, const double &fWidth1, const double &fHeight1, 
									const double &fStartAngle, const double &fSweepAngle)
{
//	ATLTRACE2( _T("Arc To\n") );

	if ( c_BrushTypeTexture == m_oBrush.Type && m_pCurPath || (m_oPatternState.IsUse() && m_pCurPath))
		m_pCurPath->AddArc( fX1, fY1, fWidth1, fHeight1, fStartAngle, fSweepAngle );

	//todooo тока добавить ?
	double fX = MMToPDFCoords( fX1 );
	double fY = MMToPDFCoords( fY1 );
	double fWidth = MMToPDFCoords( fWidth1 );
	double fHeight = MMToPDFCoords( fHeight1 );

	HRESULT hRes = S_OK;
	float fPageHeight = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;

	if ( m_bStartSubPath )
	{
		double dStartAngle = fStartAngle * 3.141592f / 180;
		float fStartX = fX + fWidth / 2.0f + fWidth / 2 * cos( AngToEllPrm( dStartAngle, fWidth / 2, fHeight / 2 ) );
		float fStartY = fY + fHeight / 2.0f + fHeight / 2 * sin( AngToEllPrm ( dStartAngle, fWidth / 2, fHeight / 2 ) );

		// Аналог PathCommandMoveTo, но без пересчета координат
        m_bStartSubPath = false;

		// В случае, когда эллипс рисуется целиком используется команда AppendEllipse, в которой команда MoveTo уже есть
		if ( !( fSweepAngle >= 360 ) )
            if ( S_OK != ( hRes = MoveTo( fStartX, fPageHeight - fStartY ) ) )
				return hRes;			

	}


    bool bClockDirection = false;
	float fEndAngle = 360 - ( fSweepAngle + fStartAngle );
	float fSrtAngle = 360 - fStartAngle;
	if( fSweepAngle > 0 )
        bClockDirection = true;

	if( fSweepAngle >= 360 ) // Целый эллипс
	{
        if ( S_OK != ( hRes = AppendEllipse( fX + fWidth / 2, fPageHeight - ( fY + fHeight / 2 ), fWidth / 2,  fHeight / 2 ) ) )
			return hRes;
	}
	else // Дуга эллипса
	{
        if ( S_OK !=  ( hRes = AppendEllipseArc( fX + fWidth / 2, fPageHeight - ( fY + fHeight / 2 ), fWidth / 2,  fHeight / 2,  fSrtAngle, fEndAngle, bClockDirection ) ) )
			return hRes;
	}
	return S_OK;
}
HRESULT CPdfWriterLib::PathCommandClose ()
{
//	ATLTRACE2( _T("Close\n") );

	if (c_BrushTypeTexture == m_oBrush.Type && m_pCurPath || (m_oPatternState.IsUse() && m_pCurPath))
		m_pCurPath->CloseFigure();

	HRESULT hRes = S_OK;
    if (S_OK != (hRes = ClosePath()))
		return hRes;

    m_bStartSubPath = true;

	return S_OK;
}
//
HRESULT CPdfWriterLib::DrawPath (const LONG & nType1)
{
    m_bFirstMoveTo	=	true;
    m_IsStrokeShape	=	false;

	// Обводка текста делается в CommandDrawText (а эта команда приходит для согласования с вьювером)
	
	if (c_nStrokeTextType == m_pCurCommand->nType)
		return S_OK;

//	ATLTRACE2(_T("Draw Path\n"));

	LONG nType = nType1;

	// TO DO: Потом надо будет этот случай убрать
	if ( 0 == nType )
		nType = c_nStroke;

    m_bStartSubPath = false;

    bool IsBrushTexture		=	c_BrushTypeTexture == m_oBrush.Type;
    bool IsNoStroke			=	c_nStroke != nType;
    bool IsBrushFileExist	=	FileExist(std_string2string(m_oBrush.TexturePath));

	HRESULT hRes = S_OK;
	if (IsBrushTexture && IsNoStroke && IsBrushFileExist && (m_pCurPath || m_bUseTextureRect))
	{
		if (c_BrushTextureModeStretch == m_oBrush.TextureMode)
		{
			// Меняем текущее состояние с пата на клип по пату и добавляем данный пат к клипу
			
			m_oRendererState.change_State(rendstateClipPath);
			SetState(rendstateNone);

			if (c_nWindingFillMode & nType)
			{
                if (S_OK != (hRes = Clip()))
					return hRes;
			}
			else if (c_nEvenOddFillMode & nType)
			{
                if (S_OK != (hRes = EoClip()))
					return hRes;
			}
			else
			{
				return S_FALSE;
			}

            if (S_OK != (hRes = EndPath()))
				return hRes;

			double fX = 0, fY = 0, fW = 0, fH = 0;

            if (true != m_bUseTextureRect)
			{
				m_pCurPath->GetBounds(fX, fY, fW, fH);
			}
			else
			{
				fX = m_oBrush.Rect.X;
				fY = m_oBrush.Rect.Y;
				fW = m_oBrush.Rect.Width;
				fH = m_oBrush.Rect.Height;
			}

			CString sPath = std_string2string(m_oBrush.TexturePath);
            std::wstring bsPath = m_oBrush.TexturePath;

            m_bUseImageTransform    =	false;
            m_bUseImageTextureAlpha =	true;

            if (S_OK != (hRes = DrawImageFromFile(bsPath, fX, fY, fW, fH)))
			{
				ResetError(m_pDocument);
				return hRes;
			}

            m_bUseImageTransform    =	true;
            m_bUseImageTextureAlpha	=	false;

			return S_OK;
		}
		else if (c_BrushTextureModeTile == m_oBrush.TextureMode)
		{
			ApplyTileFill();
		}

		// TODO: добавить тайловые заливки с реверсным узором
	}
	else if (IsBrushTexture && IsNoStroke && !IsBrushFileExist)
	{
		if (c_nWindingFillMode & nType)
			nType = c_nWindingFillMode;
		else 
			nType = c_nEvenOddFillMode;
	}

	ApplyFillGradient();
	ApplyStrokeGradient();

	if (c_nWindingFillMode != nType && c_nEvenOddFillMode != nType)
	{
        m_IsStrokeShape = true;
	}

	RELEASEOBJECT(m_pCurPath);

	switch (nType)
	{
    case c_nStroke:							if (S_OK != (hRes = Stroke()))		{	/*ATLTRACE2(_T("Stroke\n"));*/			return hRes; } break;
    case c_nWindingFillMode:				if (S_OK != (hRes = Fill()))		{	/*ATLTRACE2(_T("Fill\n"));*/			return hRes; } break;
    case c_nEvenOddFillMode:				if (S_OK != (hRes = EoFill()))		{	/*ATLTRACE2(_T("EoFill\n"));	*/		return hRes; } break;
    case c_nStroke | c_nWindingFillMode:	if (S_OK != (hRes = FillStroke()))	{	/*ATLTRACE2(_T("FillStroke\n"));*/		return hRes; } break;
    case c_nStroke | c_nEvenOddFillMode:	if (S_OK != (hRes = EoFillStroke())){	/*ATLTRACE2(_T("EoFillStroke\n"));*/	return hRes; } break;
	default:
		{
            if (S_OK != (hRes = Stroke()))
			{
//				ATLTRACE2(_T("Stroke\n"));
				return hRes;  
			}
			else
				return AVS_OFFICEPDFWRITER_ERROR_DRAW_PATH;
		}
	}

	return S_OK;
}

HRESULT CPdfWriterLib::PathCommandGetCurrentPoint (double *pfX, double *pfY)
{
//	ATLTRACE2( _T("GetCurrentPoint\n") );

	HRESULT hRes = S_OK;
	float fPageHeight = 0.0f, fX = 0.0f, fY = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;

    if ( S_OK !=  ( hRes = GetCurrentPosition( &fX, &fY ) ) )
		return hRes;

	fY = fPageHeight - fY;

	*pfX = (double)fX;
	*pfY = (double)fY;

	return S_OK;
}

HRESULT CPdfWriterLib::PathCommandText  (const std::wstring & bsText, const double & fX, const double & fY, 
										 const double & fWidth, const double & fHeight, const double & fBaseLineOffset )
{
	// В данная команда может прийти либо для обводки текста, либо для клипа по тексту
	CommandDrawText( bsText, fX, fY, fWidth, fHeight, fBaseLineOffset );
	return S_OK;
}

HRESULT CPdfWriterLib::PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset)
{
	//if (!CheckValidate())
	//	return S_FALSE;

	//_SetFont();

	//// так как пат рисуется в миллиметрах - надо перевести пункты в миллиметры
	//float fSize = (float)(0.3528 * m_oFont.Size);
	//m_pPath->AddStringC(c, m_pFontManager, x, y + baselineOffset);

	return S_OK;
}
HRESULT CPdfWriterLib::PathCommandTextEx (const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
{
	// В данная команда может прийти либо для обводки текста, либо для клипа по тексту
	CommandDrawTextEx( bsUnicodeText, bsGidText/*, bsSrcCodeText*/, x, y, w, h, baselineOffset, lFlags );
	return S_OK;
}
HRESULT CPdfWriterLib::PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
{
	if (gid >= 0)
	{
        m_oFont.StringGID = 1;
		return PathCommandTextCHAR(gid, x, y, w, h, baselineOffset);
	}
	
    m_oFont.StringGID = 0;
	return PathCommandTextCHAR(c, x, y, w, h, baselineOffset);
}

//--------- Функции для вывода изображений -------------------------------------------------------
HRESULT CPdfWriterLib::LoadImageFromInterface (IGrObject *pInterface , bool bAlpha)
{
	Aggplus::CImage *image = dynamic_cast<Aggplus::CImage *>(pInterface);

	if (image == NULL) return S_FALSE;

	LONG lWidth = image->m_dwWidth;
	LONG lHeight = image->m_dwHeight;

	//// преобразуем к BGR формату ???? меняю !!! он и так BGRA

	BYTE* pSourceBuffer = image->GetData();

	BYTE* pImage = new BYTE[ lWidth * lHeight * 3];
	BYTE* pAlpha = new BYTE[ lWidth * lHeight ];
	
	BYTE* pImagePointer = pImage; 
	BYTE* pAlphaPointer = pAlpha;
    bool  bHaveAlpha = false;

	// Разделяем саму картинку и альфа-канал. Одновременно с этим, посчитаем количество цветов в картинке.
	std::vector<TRGBColor>	arrColors;
	std::map<DWORD,int>		mapColors;

	for ( int nIndex = 0; nIndex < lWidth * lHeight; nIndex++, pImagePointer += 3, pSourceBuffer += 4, pAlphaPointer++ )
	{
		pImagePointer[ 0 ] = pSourceBuffer[ 0 ];//2
		pImagePointer[ 1 ] = pSourceBuffer[ 1 ];
		pImagePointer[ 2 ] = pSourceBuffer[ 2 ];//0

		// Картинки с количеством цветом > 256 считаем полноцветными. (формат RGB)
		if ( arrColors.size() <= 256 )
		{
			TRGBColor oColor; 
			oColor.r = pImagePointer[ 2 ];//0
			oColor.g = pImagePointer[ 1 ];
			oColor.b = pImagePointer[ 0 ];//2

			DWORD color = pImagePointer[ 0 ] << 16 + pImagePointer[ 1 ] << 8 + pImagePointer[ 2 ];
			if ( mapColors.end() == mapColors.find(color) )
			{
				mapColors.insert(std::pair<DWORD,int>(color, arrColors.size()));
				arrColors.push_back( oColor );
			}
		}

		pAlphaPointer[ 0 ] = pSourceBuffer[ 3 ];
		if ( 255 != pAlphaPointer[0] )
            bHaveAlpha = true;
	}

	TRGBColor oBlack = {   0,   0,   0 };
	TRGBColor oWhite = { 255, 255, 255 };

    bool bBnW = false; // Черно-белая картинка
	if ( ( ( arrColors.size() == 2 && ( ( arrColors[0] == oBlack && arrColors[1] == oWhite ) || ( arrColors[1] == oBlack && arrColors[0] == oWhite ) ) ) ||
		 ( arrColors.size() == 1 && ( arrColors[0] == oBlack || arrColors[1] == oWhite ) ) ) && !bHaveAlpha )
        bBnW = true;

	unsigned long nRet = OK;
	if ( !m_pXObjectList )
	{
		m_pXObjectList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
		{
			delete[] pAlpha;
			delete[] pImage;
			return nRet;
		}
	}

	//bBnW = true; для теста записи монохрома

	// Проверим нужен ли нам альфа-канал
	if ( bAlpha )
	{
		bool bFakeAlpha = true;
		for ( int nIndex = 0; nIndex < lWidth * lHeight; nIndex++ )
		{
			if ( 255 != pAlpha[nIndex] )
			{
				bFakeAlpha = false;
				break;
			}
		}

		if ( bFakeAlpha )
            bAlpha = false;
	}

	// Проверяем, возможно, такая картинка уже есть в нашем файле
	XObject pOldImage = NULL;

	CRC32 oCRC;
	oCRC.ProcessCRC( pImage, lWidth * lHeight * 3 );
	unsigned int unImageCheckSum = oCRC.m_nCRC32;
	oCRC.Init();
	oCRC.ProcessCRC( pAlpha, lWidth * lHeight );
	unsigned int unAlphaCheckSum = oCRC.m_nCRC32;

	for ( int nIndex = 0; nIndex < m_pXObjectList->nCount; ++nIndex )
	{
		XObject pCurImage = (XObject)ListItemAt( m_pXObjectList, nIndex );
		if ( !ImageValidate( pCurImage ) )
			continue;

		unsigned int unHeight = ImageGetHeight( pCurImage );
		unsigned int unWidth  = ImageGetWidth( pCurImage );
		unsigned int unBPC    = ImageGetBitsPerComponent( pCurImage );
		EColorSpace  eCS      = ImageGetColorSpace2( pCurImage );

		if ( unHeight != lHeight || unWidth != lWidth || CSDeviceRGB != eCS || 8 != unBPC )
			continue;

		if ( !StreamValidate( pCurImage->pStream ) )
			continue;

		if ( bAlpha && pAlpha  )
		{
			XObject pSMask = ImageGetSMask( pCurImage );

			if ( !ImageValidate( pSMask ) )
				continue;

			unsigned int unMaskHeight = ImageGetHeight( pSMask );
			unsigned int unMaskWidth  = ImageGetWidth( pSMask );
			unsigned int unMaskBPC    = ImageGetBitsPerComponent( pSMask );
			EColorSpace  eMaskCS      = ImageGetColorSpace2( pSMask );

			if ( unMaskHeight != lHeight || unMaskWidth != lWidth || CSDeviceGray != eMaskCS || 8 != unMaskBPC )
				continue;

			if ( !StreamValidate( pSMask->pStream ) )
				continue;

			if ( unAlphaCheckSum != pSMask->pStream->nCheckSum )
				continue;
		}

		if ( unImageCheckSum != pCurImage->pStream->nCheckSum )
			continue;

		// Если мы дошли до этого места, значит картинки полностью совпадают и нет смысла загружать картинку заново

		pOldImage = pCurImage;
		break;
	}

	if ( pOldImage )
		m_pCurrentXObject = pOldImage;
	else if ( bBnW )
		m_pCurrentXObject = LoadJbig2ImageFromMem( m_pDocument, pImage, lWidth, lHeight, unImageCheckSum);
	else
	{
		//m_pCurrentXObject = LoadRawImageFromMem ( m_pDocument, pImage, lWidth, lHeight, CSDeviceRGB, 8, bAlpha, pAlpha );
		m_pCurrentXObject = LoadJpegImageFromMem( m_pDocument, pImage, lWidth, lHeight, unImageCheckSum, bAlpha, pAlpha, unAlphaCheckSum );
		//m_pCurrentXObject = LoadJpxImageFromMem ( m_pDocument, pImage, lWidth, lHeight, unImageCheckSum, bAlpha, pAlpha, unAlphaCheckSum );
	}

	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
	{
		delete[] pAlpha;
		delete[] pImage;
		return nRet;
	}

	if ( !pOldImage )
	{
		if ( OK != ( nRet = ListAdd( m_pXObjectList, m_pCurrentXObject) ) ) 
		{
			delete[] pAlpha;
			delete[] pImage;
			return nRet;
		}
	}

	delete[] pAlpha;
	delete[]pImage;
	return S_OK;
}


HRESULT CPdfWriterLib::DrawImage (IGrObject *pInterface, const double& fX1, const double& fY1, const double& fWidth1, const double& fHeight1)
{
	SetState( rendstateImage );

	//BeginCommand( c_nPathType );
	//PathCommandMoveTo( fX, fY );
	//PathCommandLineTo( fX + fWidth, fY + fHeight );
	//DrawPath( c_nWindingFillMode );
	//EndCommand( c_nPathType );

	double fX = MMToPDFCoords( fX1 );
	double fY = MMToPDFCoords( fY1 );
	double fWidth = MMToPDFCoords( fWidth1 );
	double fHeight = MMToPDFCoords( fHeight1 );


	//if ( fWidth < 1 || fHeight < 1 )
	//	return S_OK;

	HRESULT hRes = S_OK;
	float fPageHeight = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;
    if ( S_OK !=  ( hRes = LoadImageFromInterface( pInterface, true ) ) )
		return hRes;

//	ATLTRACE2( _T("GSave: DrawImage\n") );
    if ( S_OK != ( hRes = GSave() ) )
		return hRes;

    //if ( S_OK != ( hRes = UpdateCoordSpace() ) )
	//	return hRes;

    if ( true == m_bUseImageTransform )
	{
		DWORD  nFlags  = m_oCommandParams.nFlag;
		if ( !( nFlags == 16 ) )
		{
			double dAngle  = m_oCommandParams.dAngle;
			double dLeft   = m_oCommandParams.dLeft;
			double dHeight = m_oCommandParams.dHeight;
			double dWidth  = m_oCommandParams.dWidth;
			double dTop    = m_oCommandParams.dTop;

			if ( !(0 == nFlags && 0 == dAngle) )
			{
				// Миллиметры -> координаты PDF (72dpi)

				dLeft   = MMToPDFCoords( dLeft );
				dTop    = MMToPDFCoords( dTop );
				dWidth  = MMToPDFCoords( dWidth );
				dHeight = MMToPDFCoords( dHeight );

				dTop = fPageHeight - dTop;
				dAngle =  dAngle * 3.141592f / 180;

				// Высчитываем центр прямоугольника ( относительно него производится поворот )

				double dX0 = dLeft + dWidth  / 2;
				double dY0 = dTop  - dHeight / 2;

				double dA =  cos( dAngle );
				double dB =  sin( dAngle );
				double dC = -sin( dAngle );
				double dD =  cos( dAngle );
				double dE = dX0 - dX0 * cos ( dAngle ) + dY0 * sin( dAngle );
				double dF = dY0 - dX0 * sin ( dAngle ) - dY0 * cos( dAngle );

                bool bFlipX = nFlags & 1;
                bool bFlipY = nFlags & 2;

				if ( !bFlipX && !bFlipY )
				{
					// Ничего не делаем
				}
				else if ( bFlipX && !bFlipY )
				{
					dA = -dA;
					dC = -dC;
					dE = -dE + 2 * dX0;
				}
				else if ( !bFlipX && bFlipY )
				{
					dB = -dB;
					dD = -dD;
					dF = -dF + 2 * dY0;
				}
				else
				{
					dA = -dA;
					dB = -dB;
					dC = -dC;
					dD = -dD;
					dE = -dE + 2 * dX0;
					dF = -dF + 2 * dY0;
				}

                if ( S_OK != ( hRes = Concat( dA, dB, dC, dD, dE, dF ) ) )
					return hRes;
			}
		}
		else
		{
			float fA = m_oCommandParams.oMatrix.fA;
			float fB = m_oCommandParams.oMatrix.fB;
			float fC = m_oCommandParams.oMatrix.fC;
			float fD = m_oCommandParams.oMatrix.fD;
			float fE = m_oCommandParams.oMatrix.fE;
			float fF = m_oCommandParams.oMatrix.fF;

            if ( S_OK != ( hRes = Concat( fA, -fB, -fC, fD, MMToPDFCoords( fE ) + fC * fPageHeight, fPageHeight - fPageHeight * fD - MMToPDFCoords( fF ) ) ) )
				return hRes;
		}
	}

    if ( S_OK !=  ( hRes = DrawCurrentImage( fX, fPageHeight - fY - fHeight, fWidth, fHeight ) ) )
		return hRes;

//	ATLTRACE2( _T("GRestore: DrawImage\n") );
    if ( S_OK != ( hRes = GRestore() ) )
		return hRes;

	return S_OK;
}

HRESULT CPdfWriterLib::DrawImageFromFile(const std::wstring& bsFilePath, const double& fX, const double& fY, const double& fWidth, const double& fHeight, const BYTE& lAlpha)
{
	NSImageExt::CImageExt oExt;

    bool bRes = oExt.DrawOnRenderer(this, std_string2string(bsFilePath), fX, fY, fWidth, fHeight);

	if (bRes)
		return S_OK;

	Aggplus::CImage image(bsFilePath);
		
	if (image.m_Status != Aggplus::Ok) return S_FALSE;

	if ((m_bUseImageTextureAlpha || m_bIsWhiteBackImage))
	{
		{
			LONG lWidth		= image.m_dwWidth;
			LONG lHeight	= image.m_dwHeight;
			BYTE* pBuffer	= image.GetData();

			LONG lCountPix = lWidth * lHeight;

			if ( m_bIsWhiteBackImage )
			{
				while (lCountPix > 0)
				{
					if (0 == pBuffer[3])
						memset(pBuffer, 0xFF, 4);

					--lCountPix;
					pBuffer += 4;
				}
			}
			else if ( m_bUseImageTextureAlpha )
			{
				while (lCountPix > 0)
				{
					pBuffer[3] *= ((double)m_oBrush.TextureAlpha / 255.0);
					--lCountPix;
					pBuffer += 4;
				}
			}
		}
	}

	HRESULT hRes = S_OK;
	if (S_OK != (hRes = DrawImage(&image, fX, fY, fWidth, fHeight)))
	{
		image.Destroy();
		return hRes;
	}
	image.Destroy();
	
	return S_OK;
}
//--------- Transform ----------------------------------------------------------------------------
HRESULT CPdfWriterLib::GetCommandParams (double *pdAngle, double *pdLeft, double *pdTop, double *pdWidth, double *pdHeight, DWORD *pnFlags )
{
	*pdAngle  = -m_oCommandParams.dAngle;
	*pdLeft   = m_oCommandParams.dLeft;
	*pdHeight = m_oCommandParams.dHeight;
	*pdWidth  = m_oCommandParams.dWidth;
	*pdTop    = m_oCommandParams.dTop;
	*pnFlags  = m_oCommandParams.nFlag;

	return S_OK;
}
HRESULT CPdfWriterLib::SetCommandParams (double   dAngle, double   dLeft, double   dTop, double   dWidth, double   dHeight, DWORD   nFlags )
{
//	if ( 0 != nFlags )
//		ATLTRACE2( _T("SetCommandParams: Flip <--------------------------------------------- \n") );
	HRESULT hRes = S_OK;

	m_oCommandParams.dAngle  = -dAngle;
	m_oCommandParams.dLeft   = dLeft;
	m_oCommandParams.dHeight = dHeight;
	m_oCommandParams.dWidth  = dWidth;
	m_oCommandParams.dTop    = dTop;
	m_oCommandParams.nFlag   = nFlags;

	if ( !(0 == nFlags && 0 == dAngle) )
	{
		LONG lLeft   = dLeft;
		LONG lTop    = dTop;
		LONG lWidth  = dWidth;
		LONG lHeight = dHeight;

		dAngle =  dAngle * 3.141592f / 180;

		// Высчитываем центр прямоугольника ( относительно него производится поворот )

		double dX0 = lLeft + lWidth  / 2;
		double dY0 = lTop  + lHeight / 2;

		// Рассчитываем flip
        bool bFlipX = ( 0 != ( 1 & nFlags ) );
        bool bFlipY = ( 0 != ( 2 & nFlags ) );

		LONG lFlipX = bFlipX ? -1 : 1;
		LONG lFlipY = bFlipY ? -1 : 1;

		// Наше преобразование можно записать в виде произведения матриц:
		// 
		// |  1   0  0 |   | cos(p) sin(p) 0 |   | ScaleX   0     0 |   | 1   0   0 | 
		// |  0   1  0 | * |-sin(p) cos(p) 0 | * |   0    ScaleY  0 | * | 0   1   0 |
		// |-X0 -Y0  1 |   |   0      0    1 |   |   0      0     1 |   | X0  Y0  1 |

		double dCos = cos( dAngle );
		double dSin = sin( dAngle );

		double dA =  lFlipX * dCos;
		double dB =  lFlipY * dSin;
		double dC = -lFlipX * dSin;
		double dD =  lFlipY * dCos;
		double dE = dX0 - lFlipX * dX0 * dCos + lFlipX * dY0 * dSin;
		double dF = dY0 - lFlipY * dX0 * dSin - lFlipY * dY0 * dCos;

		m_oTransform.Set( dA, dB, dC, dD, dE, dF );
	}
	else
	{
		m_oTransform.Set( 1, 0, 0, 1, 0, 0 );
	}

	return S_OK;
}

HRESULT CPdfWriterLib::SetTransform (const double & dA,const  double & dB,const  double & dC,const  double & dD,
									 const  double & dE, const double & dF)
{
//	ATLTRACE2( _T("SetTransform\n") );

	m_oCommandParams.nFlag = 16;

	m_oCommandParams.oMatrix.fA = m_oBaseTransform.fA * dA + m_oBaseTransform.fB * dC;
	m_oCommandParams.oMatrix.fC = m_oBaseTransform.fC * dA + m_oBaseTransform.fD * dC;
	m_oCommandParams.oMatrix.fE = m_oBaseTransform.fE * dA + m_oBaseTransform.fF * dC + dE;

	m_oCommandParams.oMatrix.fB = m_oBaseTransform.fA * dB + m_oBaseTransform.fB * dD;
	m_oCommandParams.oMatrix.fD = m_oBaseTransform.fC * dB + m_oBaseTransform.fD * dD;
	m_oCommandParams.oMatrix.fF = m_oBaseTransform.fE * dB + m_oBaseTransform.fF * dD + dF;

	m_oTransform.Set( dA, dB, dC, dD, dE, dF );

	return S_OK;
}

HRESULT CPdfWriterLib::GetTransform (double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	*pdA = (double)m_oCommandParams.oMatrix.fA;
	*pdB = (double)m_oCommandParams.oMatrix.fB;
	*pdC = (double)m_oCommandParams.oMatrix.fC;
	*pdD = (double)m_oCommandParams.oMatrix.fD;
	*pdE = (double)m_oCommandParams.oMatrix.fE;
	*pdF = (double)m_oCommandParams.oMatrix.fF;

	return S_OK;
}
HRESULT CPdfWriterLib::ResetTransform (void)
{
	m_oCommandParams.nFlag = 16;
	m_oCommandParams.oMatrix.fA = (float)1;
	m_oCommandParams.oMatrix.fB = (float)0;
	m_oCommandParams.oMatrix.fC = (float)0;
	m_oCommandParams.oMatrix.fD = (float)1;
	m_oCommandParams.oMatrix.fE = (float)0;
	m_oCommandParams.oMatrix.fF = (float)0;

	m_oTransform.Set( 1, 0, 0, 1, 0, 0 );

	return S_OK;
}

//--------- Clip ---------------------------------------------------------------------------------
HRESULT CPdfWriterLib::get_ClipMode (LONG* plMode)
{
	*plMode = 0;

	if ( c_nClipType == m_pCurCommand->nType )
		*plMode = m_pCurCommand->nFlag;

	return S_OK;
}
HRESULT CPdfWriterLib::put_ClipMode (const LONG & lMode)
{
	if ( c_nClipType == m_pCurCommand->nType )
		m_pCurCommand->nFlag = lMode;

	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
HRESULT CPdfWriterLib::CommandLong(const LONG& lType, const LONG& lCommand)
{
	return S_OK;
}
HRESULT CPdfWriterLib::CommandDouble(const LONG& lType, const double& dCommand)
{
	return S_OK;
}
HRESULT CPdfWriterLib::CommandString(const LONG& lType, const std::wstring& sCommand)
{
	return S_OK;
}

//--------- for teamlab --------------------------------------------------------------------------
