#include "stdafx.h"

#include "GState.h"
#include "GFont.h"
#include "SMathExt.h"
#include "ImageOutputDev.h"
#include "File.h"
#include "CMap.h"

#include "../ASCOfficeHtmlFile/ASCOfficeHtmlFile/prj/Base64.h"
#include "../Common/XmlUtils.h"

#include "FontFileTrueType.h"
#include "FontFileType1C.h"
#include "CharCodeToUnicode.h"

#include "RendererOutputDev.h"


CFontList::CFontList()
{
	InitializeCriticalSection(&m_oCS);
	m_oFontMap.RemoveAll();
}
CFontList::~CFontList()
{
	DeleteCriticalSection(&m_oCS);
	Clear();
}

void CFontList::LoadFromFile(CStringW wsDirPath)
{
	return;
	//Clear();

	//CStringW wsFilePath = wsDirPath + CStringW( _T("/FontList.rsc") );
	//XmlUtils::CXmlNode oMainNode;

	//if ( !oMainNode.FromXmlFile( wsFilePath ) )
	//	return;

	//if ( _T("PDFFontList") == oMainNode.GetName() )
	//{
	//	XmlUtils::CXmlNodes oFonts;
	//	oMainNode.GetNodes( _T("Font"), oFonts );
	//	for ( int nIndex = 0; nIndex < oFonts.GetCount(); nIndex++ )
	//	{
	//		XmlUtils::CXmlNode oFont;
	//		Ref oRef;
	//		CStringW sFilePath;
	//		int nLength = 0;
	//		unsigned short *pCodeToGid = NULL, *pCodeToUnicode = NULL;

	//		if ( oFonts.GetAt( nIndex, oFont ) )
	//		{
	//			XmlUtils::CXmlNode oNode;
	//			CStringW sValue;

	//			if ( oFont.GetNode( _T("ID"), oNode ) )
	//			{
	//				sValue = oNode.GetAttribute( _T("num") );
	//				oRef.nNum = XmlUtils::GetInteger( sValue );
	//				sValue = oNode.GetAttribute( _T("gen") );
	//				oRef.nGen = XmlUtils::GetInteger( sValue );
	//			}

	//			if ( oFont.GetNode( _T("FilePath"), oNode ) ) 
	//			{
	//				sFilePath = oNode.GetAttribute( _T("value") );
	//			}
	//			if ( oFont.GetNode( _T("CodeToGid"), oNode ) )
	//			{
	//				sValue = oNode.GetAttribute( _T("length") );
	//				nLength = XmlUtils::GetInteger( sValue );
	//				pCodeToGid = (unsigned short *)MemUtilsMalloc( sizeof(unsigned short) * nLength );
	//				if ( !pCodeToGid )
	//					return;

	//				XmlUtils::CXmlNodes oArray;
	//				oNode.GetNodes( _T("Entry"), oArray );
	//				for ( int nCurIndex = 0; nCurIndex < oArray.GetCount() && nCurIndex < nLength; nCurIndex++ )
	//				{
	//					XmlUtils::CXmlNode oArrayItem;
	//					if ( oArray.GetAt( nCurIndex, oArrayItem ) )
	//					{
	//						sValue = oArrayItem.GetAttribute( _T("value") );
	//						pCodeToGid[nCurIndex] = XmlUtils::GetInteger( sValue );
	//					}
	//					else
	//						pCodeToGid[nCurIndex] = 0;
	//				}
	//			}
	//			if ( oFont.GetNode( _T("CodeToUnicode"), oNode ) )
	//			{
	//				sValue = oNode.GetAttribute( _T("length") );
	//				nLength = XmlUtils::GetInteger( sValue );
	//				pCodeToUnicode = (unsigned short *)MemUtilsMalloc( sizeof(unsigned short) * nLength );
	//				if ( !pCodeToUnicode )
	//					return;

	//				XmlUtils::CXmlNodes oArray;
	//				oNode.GetNodes( _T("Entry"), oArray );
	//				for ( int nCurIndex = 0; nCurIndex < oArray.GetCount() && nCurIndex < nLength; nCurIndex++ )
	//				{
	//					XmlUtils::CXmlNode oArrayItem;
	//					if ( oArray.GetAt( nCurIndex, oArrayItem ) )
	//					{
	//						sValue = oArrayItem.GetAttribute( _T("value") );
	//						pCodeToUnicode[nCurIndex] = XmlUtils::GetInteger( sValue );
	//					}
	//					else
	//						pCodeToUnicode[nCurIndex] = 0;
	//				}
	//			}

	//		}

	//		Add( oRef, sFilePath, pCodeToGid, pCodeToUnicode, nLength );
	//	}
	//}
}
void CFontList::SaveToFile(CStringW wsDirPath)
{
	return;
	//CStringW wsFilePath = wsDirPath + CStringW( _T("/FontList.rsc") );
	//XmlUtils::CXmlWriter oWriter;

	//oWriter.WriteNodeBegin( _T("PDFFontList") );

	//for ( int nIndex = 0; nIndex < (int)m_arrList.GetCount(); nIndex++ )
	//{
	//	TFontEntry oEntry = m_arrList.GetAt( nIndex );
	//	oWriter.WriteNodeBegin( _T("Font") );


	//	oWriter.WriteNodeBegin( _T("ID"), TRUE );
	//	oWriter.WriteAttribute( _T("num"), oEntry.oRef.nNum );
	//	oWriter.WriteAttribute( _T("gen"), oEntry.oRef.nGen );
	//	oWriter.WriteNodeEnd( _T("ID"), TRUE, TRUE );

	//	oWriter.WriteNodeBegin( _T("FilePath"), TRUE );
	//	oWriter.WriteAttribute( _T("value"), oEntry.wsFilePath );
	//	oWriter.WriteNodeEnd( _T("FilePath"), TRUE, TRUE );

	//	if ( NULL != oEntry.pCodeToGID )
	//	{
	//		oWriter.WriteNodeBegin( _T("CodeToGid"), TRUE );
	//		oWriter.WriteAttribute( _T("length"), oEntry.nLen );
	//		oWriter.WriteNodeEnd( _T("CodeToGid"), TRUE, FALSE );
	//		for ( int nCurIndex = 0; nCurIndex < oEntry.nLen; nCurIndex++ )
	//		{
	//			oWriter.WriteNodeBegin( _T("Entry"), TRUE );
	//			oWriter.WriteAttribute( _T("value"), oEntry.pCodeToGID[nCurIndex] );
	//			oWriter.WriteNodeEnd( _T("Entry"), TRUE, TRUE );
	//		}
	//		oWriter.WriteNodeEnd( _T("CodeToGid") );
	//	}

	//	if ( NULL != oEntry.pCodeToUnicode )
	//	{
	//		oWriter.WriteNodeBegin( _T("CodeToUnicode"), TRUE );
	//		oWriter.WriteAttribute( _T("length"), oEntry.nLen );
	//		oWriter.WriteNodeEnd( _T("CodeToUnicode"), TRUE, FALSE );
	//		for ( int nCurIndex = 0; nCurIndex < oEntry.nLen; nCurIndex++ )
	//		{
	//			oWriter.WriteNodeBegin( _T("Entry"), TRUE );
	//			oWriter.WriteAttribute( _T("value"), oEntry.pCodeToUnicode[nCurIndex] );
	//			oWriter.WriteNodeEnd( _T("Entry"), TRUE, TRUE );
	//		}
	//		oWriter.WriteNodeEnd( _T("CodeToUnicode") );
	//	}

	//	oWriter.WriteNodeEnd( _T("Font") );
	//}
	//oWriter.WriteNodeEnd( _T("PDFFontList") );

	//oWriter.SaveToFile( wsFilePath );
}
BOOL CFontList::Find(Ref oRef, TFontEntry *pEntry)
{
	CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

	BOOL bResult = ( NULL != ( pEntry = m_oFontMap.Lookup(oRef) ) );

	if ( bResult )
	{
		// Шрифт нашелся, но пока им пользоваться нельзя, потому что он загружается в параллельном потоке
		while ( !pEntry->bAvailable )
			Sleep(10);
	}

	RELEASEOBJECT(pCS);

	return bResult;
}
BOOL CFontList::Find2(Ref oRef, TFontEntry **ppEntry)
{
	CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

	BOOL bResult = ( NULL != ( (*ppEntry) = m_oFontMap.Lookup(oRef) ) );

	if ( bResult )
	{
		// Шрифт нашелся, но пока им пользоваться нельзя, потому что он загружается в параллельном потоке
		while ( !(*ppEntry)->bAvailable )
			Sleep(10);
	}

	if ( !bResult )
	{
		(*ppEntry) = Add( oRef, _T(""), NULL, NULL, 0, 0 );
		(*ppEntry)->bAvailable = FALSE;
	}

	RELEASEOBJECT(pCS);

	return bResult;
}

TFontEntry *CFontList::Add(Ref oRef, CStringW wsFileName, unsigned short *pCodeToGID, unsigned short *pCodeToUnicode, unsigned int unLenGID, unsigned int unLenUnicode)
{
	// Данная функция приходит только из Find2, поэтому проверять есть ли данный шрифт уже не надо
	CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

	TFontEntry *pNewEntry = new TFontEntry;
	pNewEntry->oRef.nGen      = oRef.nGen;
	pNewEntry->oRef.nNum      = oRef.nNum;
	pNewEntry->wsFilePath     = wsFileName;
	pNewEntry->pCodeToGID     = pCodeToGID;
	pNewEntry->pCodeToUnicode = pCodeToUnicode; 
	pNewEntry->unLenGID       = unLenGID;
	pNewEntry->unLenUnicode   = unLenUnicode;
	
	m_oFontMap.Add( oRef, pNewEntry );

	RELEASEOBJECT(pCS);

	return pNewEntry;
}


void CFontList::Clear()
{
	for ( int nIndex = 0; nIndex < m_oFontMap.GetSize(); nIndex++ )
	{
		TFontEntry *pEntry = m_oFontMap.GetValueAt( nIndex );
		if ( NULL != pEntry )
		{
			MemUtilsFree( pEntry->pCodeToGID );
			MemUtilsFree( pEntry->pCodeToUnicode );
		}
		delete pEntry;
	}
	m_oFontMap.RemoveAll();
}
BOOL CFontList::GetFont(Ref *pRef, TFontEntry *pEntry)
{
	int nIndex = m_oFontMap.FindKey( *pRef );
	
	if( -1 == nIndex )
		return FALSE;

	*pEntry = *m_oFontMap.GetValueAt( nIndex );

	return TRUE;
}

RendererOutputDev::RendererOutputDev(GlobalParams *pGlobalParams, IASCRenderer *pRenderer, CFontList *pFontList)
{
	m_pGlobalParams = pGlobalParams;
	m_pFontList     = pFontList;

	m_bTiling       = FALSE;

	m_pBufferTextClip = NULL;
	m_pClip           = NULL;

	m_nRendererType = 0;
	m_pRenderer     = pRenderer;

	m_bUseAxialShaded  = FALSE;
	m_bUseRadialShaded = FALSE;

	if ( NULL != m_pRenderer )
	{
		m_pRenderer->put_PenColor( 0 );
		m_pRenderer->put_PenAlpha( 255 );
		m_pRenderer->put_PenSize( 1 );

		m_pRenderer->put_ShadowVisible( 0 );
		m_pRenderer->put_ShadowColor( 0 );

		m_pRenderer->put_FontName( _T("Arial") );
		m_pRenderer->put_FontStyle( 0 );
		m_pRenderer->put_FontSize( 10.0 );

		IPDFWriter *pInterface = NULL;
		m_pRenderer->QueryInterface( __uuidof(IPDFWriter), (void**)&pInterface );
		if ( NULL != pInterface )
			m_nRendererType = 1;
		RELEASEINTERFACE( pInterface );

		LONG lType = c_nUnknownRenderer;
		m_pRenderer->get_Type( &lType );
		if ( c_nSWFRenderer == lType )
		{
			m_bUseAxialShaded  = TRUE;
			m_bUseRadialShaded = TRUE;
		}
	}

	m_pFontManager = NULL;
	::CoCreateInstance( __uuidof(AVSGraphics::CASCFontManager), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IASCFontManager), (void **)&m_pFontManager );
	if ( NULL != m_pFontManager )
		m_pFontManager->Initialize( _T("<FontManagerOptions><WinList load='0' /></FontManagerOptions>") );

	m_pbBreak = NULL;

	m_bTransparentGroup = FALSE;
	m_bTransparentGroupSoftMask = FALSE;
	m_pTransparentGroupSoftMask = NULL;
	
	//m_oFontList.LoadFromFile( m_pGlobalParams->GetTempFolder() );
	//// Тестовый пример
	//m_pRenderer->NewPage();
	//m_pRenderer->BeginCommand( c_nPageType );

	//m_pRenderer->SetWidth( 100.0f );
	//m_pRenderer->SetHeight( 100.0f );

	//m_oFont.SetFontSize( 1.0 );
	//m_oPen.SetSize( 0.3 );
	//m_oPen.SetAlpha( 128 );
	//m_pRenderer->SetFont( m_oFont.ToXmlString().AllocSysString() );
	//m_pRenderer->SetShadow( m_oShadow.ToXmlString().AllocSysString() );
	//m_pRenderer->SetPen( m_oPen.ToXmlString().AllocSysString() );

	//m_pRenderer->PathCommandMoveTo( 0, 50 );
	//m_pRenderer->PathCommandLineTo( 100, 50 );
	//m_pRenderer->DrawPath( c_nStroke );

	//m_pRenderer->PathCommandMoveTo( 50, 0 );
	//m_pRenderer->PathCommandLineTo( 50, 100 );
	//m_pRenderer->DrawPath( c_nStroke );
	//
	//m_oPen.SetSize( 0.3 );
	//m_oPen.SetAlpha( 255 );
	//m_pRenderer->SetPen( m_oPen.ToXmlString().AllocSysString() );

	//IAVSOfficeRendererTemplate2 *pRenderer2 = NULL;
	//m_pRenderer->QueryInterface( __uuidof(AVSOfficeEditor::IAVSOfficeRendererTemplate2), (void**)&pRenderer2 );

	//double pNewTm[6], arrMatrix[6];
	//double dAscent  = 0.905;
	//double dDescent = -0.211; 
	//double dKoef = ( dAscent - fabs( dDescent ) ) * 1;

	//double pCTM[6] = { 10, 0, 0, 10, 50, 50};
	//pNewTm[0] =  1;//pTm[0];
	//pNewTm[1] =  0;//pTm[1];
	//pNewTm[2] = -0;//pTm[2];
	//pNewTm[3] = -1;//pTm[3];
	//pNewTm[4] =  0 + 0 * dKoef;
	//pNewTm[5] =  0 + 1 * dKoef;

	//arrMatrix[0] =   pNewTm[0] * pCTM[0] + pNewTm[1] * pCTM[2];
	//arrMatrix[1] = -(pNewTm[0] * pCTM[1] + pNewTm[1] * pCTM[3]);
	//arrMatrix[2] =   pNewTm[2] * pCTM[0] + pNewTm[3] * pCTM[2];
	//arrMatrix[3] = -(pNewTm[2] * pCTM[1] + pNewTm[3] * pCTM[3]);
	//arrMatrix[4] =   pNewTm[4] * pCTM[0] + pNewTm[5] * pCTM[2] + pCTM[4];
	//arrMatrix[5] = -(pNewTm[4] * pCTM[1] + pNewTm[5] * pCTM[3] + pCTM[5]) + 100;

	//double  dAcsentFactor = ( ( fabs(-0.324) + fabs(1.005) ) - ( dAscent + fabs( dDescent ) ) ) / 2 + dAscent;  
	//double dAscentShiftX = -arrMatrix[2] * dAcsentFactor;
	//double dAscentShiftY = -arrMatrix[3] * dAcsentFactor;

	////arrMatrix[4] += dAscentShiftX;
	////arrMatrix[5] += dAscentShiftY;


	//pRenderer2->SetTransform( arrMatrix[0], arrMatrix[1], arrMatrix[2], arrMatrix[3], arrMatrix[4], arrMatrix[5] );

	//m_pRenderer->PathCommandMoveTo( -0.664, -0.324 );
	//m_pRenderer->PathCommandLineTo(  2.000, -0.324 );
	//m_pRenderer->PathCommandLineTo(  2.000,  1.005 );
	//m_pRenderer->PathCommandLineTo( -0.664,  1.005 );
	//m_pRenderer->PathCommandClose();
	//m_pRenderer->DrawPath( c_nStroke );


	//m_pRenderer->CommandDrawText( _T("A"), 0, 0, 0, 0, 0 );

	//double dAlpha =  0*(3.14 / 180.f) * 160;
	//pRenderer2->SetTransform( cos( dAlpha ), -sin( dAlpha ), -sin( dAlpha ), -cos( dAlpha ), 50, 50  );
	//m_pRenderer->PathCommandMoveTo( 0, 0 );
	//m_pRenderer->PathCommandLineTo( 10, 0 );
	//m_pRenderer->PathCommandLineTo( 10, 10 );
	//m_pRenderer->PathCommandLineTo( 0, 10 );
	//m_pRenderer->PathCommandClose();
	//m_pRenderer->DrawPath( c_nStroke );


	//m_pRenderer->EndCommand( c_nPageType );
	//RELEASEINTERFACE( pRenderer2 );
}

RendererOutputDev::~RendererOutputDev()
{
	//m_oFontList.SaveToFile( m_pGlobalParams->GetTempFolder() );

	m_pRenderer = NULL;

	if ( m_pClip )
		delete m_pClip;

	if ( m_pBufferTextClip )
		delete m_pBufferTextClip;

	if ( m_pTransparentGroupSoftMask )
		delete []m_pTransparentGroupSoftMask;

	RELEASEINTERFACE( m_pFontManager );
}
void RendererOutputDev::StartPage(int nPageIndex, GrState *pGState)
{
	//m_pRenderer->NewPage();
	//m_pRenderer->BeginCommand( c_nPageType );

	// Переводим пункты в миллиметры
	double dPageHeight = PDFCoordsToMM( pGState->GetPageHeight() );
	double dPageWidth  = PDFCoordsToMM( pGState->GetPageWidth() );

	m_arrMatrix[0] = 1; m_arrMatrix[1] = 0;
	m_arrMatrix[2] = 0; m_arrMatrix[3] = 1;
	m_arrMatrix[4] = 0; m_arrMatrix[5] = 0;

	m_pRenderer->put_Width( dPageWidth );
	m_pRenderer->put_Height( dPageHeight );

	m_bTransparentGroup = FALSE;
	m_bTransparentGroupSoftMask = FALSE;

	if ( m_pTransparentGroupSoftMask )
		delete []m_pTransparentGroupSoftMask;
	m_pTransparentGroupSoftMask = NULL;
}

void RendererOutputDev::EndPage()
{
	//m_pRenderer->EndCommand( c_nPageType );
}

void RendererOutputDev::SaveGState(GrState *pGState)
{
	UpdateAll(pGState);
}
void RendererOutputDev::RestoreGState(GrState *pGState)
{
	UpdateAll( pGState );
}

void RendererOutputDev::UpdateCTM(GrState *pGState, double dMatrix11, double dMatrix12, double dMatrix21, double dMatrix22, double dMatrix31, double dMatrix32)
{
}
void RendererOutputDev::UpdateLineDash(GrState *pGState)
{
	double *pDash  = NULL;
	int     nSize  = 0;
	double  dStart = 0;
	pGState->GetLineDash( &pDash, &nSize, &dStart );

	if ( 0 == nSize  ) // Solid
	{
		m_pRenderer->put_PenDashStyle( 0 );
		m_pRenderer->put_PenDashOffset( 0 );
	}
	else
	{
		SAFEARRAYBOUND sabBound;
		sabBound.cElements = ( nSize == 1 ? 2 : nSize );
		sabBound.lLbound   = 0;

		SAFEARRAY *psaDash = SafeArrayCreate( VT_R8, 1, &sabBound );

		if ( 1 == nSize )
		{
			double adDash[2] = { pDash[0], pDash[0] };
			::memcpy( psaDash->pvData, adDash, 2 * sizeof(double) );
		}
		else
		{
			::memcpy( psaDash->pvData, pDash, nSize * sizeof(double) );
		}

		m_pRenderer->PenDashPattern( psaDash );
		m_pRenderer->put_PenDashStyle( 5 );
		m_pRenderer->put_PenDashOffset( dStart );

		SafeArrayDestroy( psaDash );
	}
}
void RendererOutputDev::UpdateFlatness(GrState *pGState)
{
}
void RendererOutputDev::UpdateLineJoin(GrState *pGState)
{
	int nJoinStyle = pGState->GetLineJoin();
	if ( 1 == nJoinStyle )
		nJoinStyle = 2;
	else if ( 2 == nJoinStyle )
		nJoinStyle = 1;

	m_pRenderer->put_PenLineJoin( nJoinStyle );
}
void RendererOutputDev::UpdateLineCap(GrState *pGState)
{
	int nCapStyle = pGState->GetLineCap();
	if ( 1 == nCapStyle )
		nCapStyle = 2;
	else if ( 2 == nCapStyle )
		nCapStyle = 1;

	m_pRenderer->put_PenLineStartCap( nCapStyle );
	m_pRenderer->put_PenLineEndCap( nCapStyle );
}
void RendererOutputDev::UpdateMiterLimit(GrState *pGState)
{
}
void RendererOutputDev::UpdateLineWidth(GrState *pGState)
{
	m_pRenderer->put_PenSize( PDFCoordsToMM( pGState->GetLineWidth() ) );
}
void RendererOutputDev::UpdateStrokeAdjust(GrState *pGState)
{
}
void RendererOutputDev::UpdateFillColor(GrState *pGState)
{
	GrColor *pColor = pGState->GetFillColor();
	GrColorSpace *pColorSpace = pGState->GetFillColorSpace();

	DWORD dwColor = pColorSpace->GetDwordColor(pColor);

	m_pRenderer->put_BrushColor1( dwColor );
	m_pRenderer->put_BrushColor2( dwColor );
}
void RendererOutputDev::UpdateStrokeColor(GrState *pGState)
{
	GrColor *pColor = pGState->GetStrokeColor();
	GrColorSpace *pColorSpace = pGState->GetStrokeColorSpace();

	DWORD dwColor = pColorSpace->GetDwordColor( pColor );

	m_pRenderer->put_PenColor( dwColor );
}
void RendererOutputDev::UpdateBlendMode(GrState *pGState)
{
}
void RendererOutputDev::UpdateFillOpacity(GrState *pGState)
{
	m_pRenderer->put_BrushAlpha1( min( 255, max( 0, int(pGState->GetFillOpacity() * 255) ) ) );
	m_pRenderer->put_BrushAlpha2( min( 255, max( 0, int(pGState->GetFillOpacity() * 255) ) ) );
}
void RendererOutputDev::UpdateStrokeOpacity(GrState *pGState)
{
	m_pRenderer->put_PenAlpha( min( 255, max( 0, int(pGState->GetStrokeOpacity() * 255) ) ) );
}

void RendererOutputDev::UpdateAll(GrState *pGState)
{
	UpdateCTM( pGState, pGState->GetCTM()[0], pGState->GetCTM()[1], pGState->GetCTM()[2], pGState->GetCTM()[3], pGState->GetCTM()[4], pGState->GetCTM()[5] );
	UpdateLineDash( pGState );
	UpdateFlatness( pGState );
	UpdateLineJoin( pGState );
	UpdateLineCap( pGState );
	UpdateMiterLimit( pGState );
	UpdateLineWidth( pGState );
	UpdateStrokeAdjust( pGState );
	UpdateFillColorSpace( pGState );
	UpdateFillColor( pGState );
	UpdateStrokeColorSpace( pGState );
	UpdateStrokeColor( pGState );
	UpdateBlendMode( pGState );
	UpdateFillOpacity( pGState );
	UpdateStrokeOpacity( pGState );
	UpdateFont( pGState );
	UpdateClip( pGState );
}
void RendererOutputDev::UpdateRender(GrState *pGState)
{

}
void RendererOutputDev::UpdateFont(GrState *pGState)
{
	// Проверяем наличие списка со шрифтами
	if ( NULL == m_pFontList )
		return;

	GrFont *pFont = pGState->GetFont();
	if ( NULL == pFont )
		return;

	m_pRenderer->put_FontSize( pGState->GetFontSize() );
	//m_oFont.Size = pGState->GetFontSize();

	CStringW wsFileName = _T("");
	CStringW wsFontName = _T("");
	TFontEntry *pEntry = NULL;
	if ( !m_pFontList->Find2( (*pFont->GetID()), &pEntry ) )
	{
		GrFontType eFontType = pFont->GetType();
		if ( fontType3 == eFontType ) // FontType3 обрабатывается отдельной командой
		{
			pEntry->bAvailable = TRUE;
			return;
		}

		CStringW wsTempFileName = _T("");
		DisplayFontParam *pDisplayFontParam = NULL;

		Ref oEmbRef;
		if ( pFont->GetEmbeddedFontFileRef( &oEmbRef ) ) 
		{
			CStringW wsExt;
			switch( pFont->GetType() )
			{
			case fontType1:       wsExt = _T(".pfb_t1");    break; 
			case fontType1C:      wsExt = _T(".pfb_t1c");   break;
			case fontType1COT:    wsExt = _T(".pfb_t1cot"); break;
			case fontTrueType:    wsExt = _T(".ttf");       break;
			case fontTrueTypeOT:  wsExt = _T(".otf");       break;
			case fontCIDType0:    wsExt = _T(".cid_0");     break;
			case fontCIDType0C:   wsExt = _T(".cid_0c");    break;
			case fontCIDType0COT: wsExt = _T(".cid_0cot");  break;
			case fontCIDType2:    wsExt = _T(".cid_2");     break;
			case fontCIDType2OT:  wsExt = _T(".cid_2ot");   break;
			}

			FILE *pTempFile = NULL;
			if ( !OpenTempFile( &wsTempFileName, &pTempFile, _T("wb"), wsExt.GetBuffer(), m_pGlobalParams->GetTempFolder() ) ) 
			{
				// TO DO: Eerror "Couldn't create temporary font file"
				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}

				pEntry->bAvailable = TRUE;
				return;
			}
			Object oReferenceObject, oStreamObject;
			oReferenceObject.InitRef( oEmbRef.nNum, oEmbRef.nGen );
			oReferenceObject.Fetch( m_pXref, &oStreamObject );
			oReferenceObject.Free();
			if ( !oStreamObject.IsStream() ) 
			{
				// TO DO: Error "Embedded font object is wrong type"
				oStreamObject.Free();
				fclose( pTempFile );

				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}

				pEntry->bAvailable = TRUE;
				return;
			}
			oStreamObject.StreamReset();
			int nChar;
			while ( ( nChar = oStreamObject.StreamGetChar() ) != EOF ) 
			{
				fputc( nChar, pTempFile );
			}
			oStreamObject.StreamClose();
			oStreamObject.Free();
			fclose( pTempFile );
			wsFileName = wsTempFileName;

			if ( fontType1 == pFont->GetType() || fontType1C == pFont->GetType() || fontType1COT == pFont->GetType() )
			{
				// Дописываем файл afm
				wchar_t wsDrive[MAX_PATH], wsDir[MAX_PATH], wsFilename[MAX_PATH];
				_wsplitpath( wsFileName.GetBuffer(), wsDrive, wsDir, wsFilename, NULL );
				CStringW wsAFMpath = CStringW( wsDrive ) + CStringW( wsDir ) + CStringW( wsFilename ) + CStringW( _T(".afm") );

				FILE *pFile = _wfopen( wsAFMpath.GetBuffer(), L"wb+" );
				if ( pFile )
				{
					Ref *pRef = pFont->GetID();
					Object oRefObject, oFontObject;
					oRefObject.InitRef( pRef->nNum, pRef->nGen );
					oRefObject.Fetch( m_pXref, &oFontObject );
					oRefObject.Free();

					if ( oFontObject.IsDict() )
					{
						CStringA sFontName, sFontFamily, sFontStretch;
						int nFontWeight = 0, nItalicAngle = 0, nAscent = 0, nDescent = 0, nLeading = 0;
						int nCapHeight = 0, nXHeight = 0, nStemV = 0, nStemH = 0, nAvgWidth = 0, nMaxWidth = 0, nMissingWidth = 0;
						Array *pBBox = NULL;
						int arrBBox[4] = {0, 0, 0, 0};


						Dict *pFontDict = oFontObject.GetDict();
						Object oFontDescriptor;
						if ( pFontDict->Search("FontDescriptor", &oFontDescriptor)->IsDict() )
						{
							Object oDictItem;
							// FontName
							oFontDescriptor.DictLookup("FontName", &oDictItem);
							if ( oDictItem.IsName() ) sFontName = CStringA( oDictItem.GetName() );
							oDictItem.Free();

							// FontFamily
							oFontDescriptor.DictLookup("FontFamily", &oDictItem);
							if ( oDictItem.IsName() ) sFontFamily = CStringA( oDictItem.GetName() );
							oDictItem.Free();

							// FontStretch
							oFontDescriptor.DictLookup("FontStretch", &oDictItem);
							if ( oDictItem.IsName() ) sFontStretch = CStringA( oDictItem.GetName() ); 								
							oDictItem.Free();

							// FontWeight
							oFontDescriptor.DictLookup("FontWeight", &oDictItem);
							if ( oDictItem.IsInt() ) nFontWeight = oDictItem.GetInt();
							oDictItem.Free();

							// FontBBox
							oFontDescriptor.DictLookup("FontBBox", &oDictItem);
							if ( oDictItem.IsArray() ) pBBox = oDictItem.GetArray();
							if ( 4 == pBBox->GetCount() )
							{
								for ( int nIndex = 0; nIndex < 4; nIndex++ )
								{
									Object oArrayItem;
									pBBox->Get( nIndex, &oArrayItem );
									if ( oArrayItem.IsInt() )
										arrBBox[nIndex] = oArrayItem.GetInt();

									oArrayItem.Free();
								}
							}
							oDictItem.Free();

							// ItalicAngle
							oFontDescriptor.DictLookup("ItalicAngle", &oDictItem);
							if ( oDictItem.IsInt() ) nItalicAngle = oDictItem.GetInt();
							oDictItem.Free();

							// Ascent
							oFontDescriptor.DictLookup("Ascent", &oDictItem);
							if ( oDictItem.IsInt() ) nAscent = oDictItem.GetInt();
							oDictItem.Free();

							// Leading
							oFontDescriptor.DictLookup("Leading", &oDictItem);
							if ( oDictItem.IsInt() ) nLeading = oDictItem.GetInt();
							oDictItem.Free();

							// CapHeight
							oFontDescriptor.DictLookup("CapHeight", &oDictItem);
							if ( oDictItem.IsInt() ) nCapHeight = oDictItem.GetInt();
							oDictItem.Free();

							// XHeight
							oFontDescriptor.DictLookup("XHeight", &oDictItem);
							if ( oDictItem.IsInt() ) nXHeight = oDictItem.GetInt();
							oDictItem.Free();

							// StemV
							oFontDescriptor.DictLookup("StemV", &oDictItem);
							if ( oDictItem.IsInt() ) nStemV = oDictItem.GetInt();
							oDictItem.Free();

							// StemH
							oFontDescriptor.DictLookup("StemH", &oDictItem);
							if ( oDictItem.IsInt() ) nStemH = oDictItem.GetInt();
							oDictItem.Free();

							// Descent
							oFontDescriptor.DictLookup("Descent", &oDictItem);
							if ( oDictItem.IsInt() ) nDescent = oDictItem.GetInt();
							oDictItem.Free();

							// AvgWidth
							oFontDescriptor.DictLookup("AvgWidth", &oDictItem);
							if ( oDictItem.IsInt() ) nAvgWidth = oDictItem.GetInt();
							oDictItem.Free();

							// MaxWidth
							oFontDescriptor.DictLookup("MaxWidth", &oDictItem);
							if ( oDictItem.IsInt() ) nMaxWidth = oDictItem.GetInt();
							oDictItem.Free();

							// MissingWidth
							oFontDescriptor.DictLookup("MissingWidth", &oDictItem);
							if ( oDictItem.IsInt() ) nMissingWidth = oDictItem.GetInt();
							oDictItem.Free();

						}
						oFontDescriptor.Free();

						fprintf( pFile, "StartFontMetrics 3.0\n" );
						if ( "" != sFontName    ) fprintf( pFile, "FontName %s\n",   sFontName.GetBuffer()   );
						if ( "" != sFontFamily  ) fprintf( pFile, "FamilyName %s\n", sFontFamily.GetBuffer() );
						if ( nFontWeight >= 550 ) fprintf( pFile, "Weight Bold\n");

						fprintf( pFile, "ItalicAngle %d\n", nItalicAngle );

						fprintf( pFile, "FontBBox %d %d %d %d\n", arrBBox[0], arrBBox[1], arrBBox[2], arrBBox[3] );

						fprintf( pFile, "CapHeight %d\n", nCapHeight );
						fprintf( pFile, "XHeight %d\n", nXHeight );
						fprintf( pFile, "Ascender %d\n", nAscent );
						fprintf( pFile, "Descender %d\n", nDescent );
						fprintf( pFile, "StdHW %d\n", nStemH );
						fprintf( pFile, "StdHV %d\n", nStemV );

						int nFirstChar = 0;
						Object oDictItem;
						pFontDict->Search("FirstChar", &oDictItem);
						if ( oDictItem.IsInt() ) nFirstChar = oDictItem.GetInt();
						oDictItem.Free();

						int nLastChar = nFirstChar;
						pFontDict->Search("LastChar", &oDictItem);
						if ( oDictItem.IsInt() ) nLastChar = oDictItem.GetInt();
						oDictItem.Free();

						Array *pWidths = NULL;
						pFontDict->Search("Widths", &oDictItem );
						if ( oDictItem.IsArray() ) pWidths = oDictItem.GetArray();

						int nCount = nLastChar - nFirstChar + 1;
						Gr8BitFont *pT1Font = (Gr8BitFont *)pFont;

						if ( NULL != pWidths )
						{
							int nWidthsCount = pWidths->GetCount();
							fprintf( pFile, "StartCharMetrics %d\n", nWidthsCount);
							for ( int nIndex = 0; nIndex < nWidthsCount; nIndex++ )
							{
								int nWidth = nMissingWidth;
								Object oArrayItem;
								pWidths->Get( nIndex, &oArrayItem );
								if ( oArrayItem.IsInt() ) nWidth = oArrayItem.GetInt();
								oArrayItem.Free();

								char **ppEncoding = pT1Font->GetEncoding();


								if ( NULL != ppEncoding && NULL != ppEncoding[nIndex] )
									fprintf( pFile, "C %d ; WX %d ; N %s ;\n", nIndex + nFirstChar, nWidth, ppEncoding[nIndex] );
								else
									fprintf( pFile, "C %d ; WX %d ;\n", nIndex + nFirstChar, nWidth );
							}
							fprintf( pFile, "EndCharMetrics\n" );
						}
						oDictItem.Free();
					}

					fclose( pFile );
				}
			}

			// Загрузим сам файл со шрифтом, чтобы точно определить его тип
			BSTR bsFilePath = wsFileName.AllocSysString();
			HRESULT hRes = S_OK;
			if ( FAILED ( hRes = m_pFontManager->LoadFontFromFile( bsFilePath, 10, 0, 0, 0 ) ) )
			{
				::SysFreeString( bsFilePath );
				pEntry->bAvailable = TRUE;
				return;
			}
			::SysFreeString( bsFilePath );
			BSTR bsFontType = NULL;
			m_pFontManager->GetFontType( &bsFontType );
			CString sFontType = bsFontType;
			::SysFreeString( bsFontType );

			if ( _T("TrueType") == sFontType )
			{
				if (   eFontType != fontType1COT   && eFontType != fontTrueType 
					&& eFontType != fontTrueTypeOT && eFontType != fontCIDType0COT 
					&& eFontType != fontCIDType2   && eFontType != fontCIDType2OT )
				{
					if ( eFontType == fontType1 || eFontType == fontType1C )
						eFontType = fontType1COT;
					else if ( eFontType == fontCIDType0 || eFontType == fontCIDType0C )
						eFontType = fontCIDType0COT;
				}
			}
			else if ( _T("Type 1") == sFontType )
			{
				if ( eFontType != fontType1 && eFontType != fontType1C )
				{
					eFontType = fontType1;
				}
			}
			else if ( _T("CID Type 1") == sFontType )
			{
				if (   eFontType != fontCIDType0   && eFontType != fontCIDType0C 
					&& eFontType != fontCIDType2OT && eFontType != fontCIDType0COT )
				{
					eFontType = fontCIDType0;
				}
			}
			else if ( _T("CFF") == sFontType )
			{
				if (   eFontType != fontType1C      && eFontType != fontType1COT 
					&& eFontType != fontTrueTypeOT  && eFontType != fontCIDType0C 
					&& eFontType != fontCIDType0COT && eFontType != fontCIDType2OT )
				{
					if ( eFontType == fontType1 || eFontType == fontTrueType )
						eFontType = fontType1C;
					else if ( eFontType == fontCIDType0 || eFontType == fontCIDType2 )
						eFontType = fontCIDType0C;
				}
			}
		} 
		else if ( _T("") == ( wsFileName = pFont->GetExternalFontFilePath() ) ) // Если есть внешний FontFile, тогда используем его
		{
			// Ищем DisplayFont
			if ( pFont->IsCIDFont() ) 
			{
				if ( m_pGlobalParams && ((GrCIDFont *)pFont)->GetCollection() ) 
				{
					pDisplayFontParam = m_pGlobalParams->GetDisplayCIDFont( pFont->GetBaseName(), ((GrCIDFont *)pFont)->GetCollection() );
				}
			} 
			else 
			{
				if ( m_pGlobalParams && pFont->GetBaseName() ) 
				{
					pDisplayFontParam = m_pGlobalParams->GetDisplayFont( pFont->GetBaseName() );
				}
				//if ( m_pGlobalParams && !pDisplayFontParam ) 
				//{
				//	int nSubstIndex = -1;

				//	if ( pGrFont->IsFixedWidth() ) 
				//	{
				//		nSubstIndex = 8;
				//	} 
				//	else if ( pGrFont->IsSerif() ) 
				//	{
				//		nSubstIndex = 4;
				//	} 
				//	else 
				//	{
				//		nSubstIndex = 0;
				//	}
				//	if ( pGrFont->IsBold() ) 
				//	{
				//		nSubstIndex += 2;
				//	}
				//	if ( pGrFont->IsItalic() ) 
				//	{
				//		nSubstIndex += 1;
				//	}
				//	StringExt *seSubstName = new StringExt( c_arrSImageOutSubstFonts[nSubstIndex].sName );
				//	pDisplayFontParam = m_pGlobalParams->GetDisplayFont( seSubstName );
				//	delete seSubstName;
				//	pID->SetSubstIndex( nSubstIndex );
				//}
			}

			if ( pDisplayFontParam )
			{
				switch ( pDisplayFontParam->m_eType ) 
				{
				case displayFontT1:
					wsFileName = *(pDisplayFontParam->m_oT1.pwsFileName);
					eFontType  = pFont->IsCIDFont() ? fontCIDType0 : fontType1;
					break;
				case displayFontTT:
					wsFileName = *(pDisplayFontParam->m_oTT.pwsFileName);
					eFontType  = pFont->IsCIDFont() ? fontCIDType2 : fontTrueType;
					break;
				}
			}
			if ( !pDisplayFontParam ) 
			{
				// TO DO: Вставить функцию m_pFontManager->GetWinFontByParams();

				if ( m_pFontManager )
				{
					Ref *pRef = pFont->GetID();
					Object oRefObject, oFontObject;
					oRefObject.InitRef( pRef->nNum, pRef->nGen );
					oRefObject.Fetch( m_pXref, &oFontObject );
					oRefObject.Free();

					if ( oFontObject.IsDict() )
					{
						CStringW sFontName, sFontFamily, sFontStretch;
						int nFontWeight = 0, nItalicAngle = 0, nAscent = 0, nDescent = 0, nLeading = 0;
						int nCapHeight = 0, nXHeight = 0, nStemV = 0, nStemH = 0, nAvgWidth = 0, nMaxWidth = 0, nMissingWidth = 0;
						Array *pBBox = NULL;
						int arrBBox[4] = {0, 0, 0, 0};


						Dict *pFontDict = oFontObject.GetDict();
						Object oFontDescriptor;
						if ( pFontDict->Search("FontDescriptor", &oFontDescriptor)->IsDict() )
						{
							Object oDictItem;
							// FontName
							oFontDescriptor.DictLookup("FontName", &oDictItem);
							if ( oDictItem.IsName() ) sFontName = CStringW( oDictItem.GetName() );
							oDictItem.Free();

							// FontFamily
							oFontDescriptor.DictLookup("FontFamily", &oDictItem);
							if ( oDictItem.IsName() ) sFontFamily = CStringW( oDictItem.GetName() );
							oDictItem.Free();

							// FontStretch
							oFontDescriptor.DictLookup("FontStretch", &oDictItem);
							if ( oDictItem.IsName() ) sFontStretch = CStringW( oDictItem.GetName() ); 								
							oDictItem.Free();

							// FontWeight
							oFontDescriptor.DictLookup("FontWeight", &oDictItem);
							if ( oDictItem.IsInt() ) nFontWeight = oDictItem.GetInt();
							oDictItem.Free();

							// FontBBox
							oFontDescriptor.DictLookup("FontBBox", &oDictItem);
							if ( oDictItem.IsArray() ) pBBox = oDictItem.GetArray();
							if ( 4 == pBBox->GetCount() )
							{
								for ( int nIndex = 0; nIndex < 4; nIndex++ )
								{
									Object oArrayItem;
									pBBox->Get( nIndex, &oArrayItem );
									if ( oArrayItem.IsInt() )
										arrBBox[nIndex] = oArrayItem.GetInt();

									oArrayItem.Free();
								}
							}
							oDictItem.Free();

							// ItalicAngle
							oFontDescriptor.DictLookup("ItalicAngle", &oDictItem);
							if ( oDictItem.IsInt() ) nItalicAngle = oDictItem.GetInt();
							oDictItem.Free();

							// Ascent
							oFontDescriptor.DictLookup("Ascent", &oDictItem);
							if ( oDictItem.IsInt() ) nAscent = oDictItem.GetInt();
							oDictItem.Free();

							// Leading
							oFontDescriptor.DictLookup("Leading", &oDictItem);
							if ( oDictItem.IsInt() ) nLeading = oDictItem.GetInt();
							oDictItem.Free();

							// CapHeight
							oFontDescriptor.DictLookup("CapHeight", &oDictItem);
							if ( oDictItem.IsInt() ) nCapHeight = oDictItem.GetInt();
							oDictItem.Free();

							// XHeight
							oFontDescriptor.DictLookup("XHeight", &oDictItem);
							if ( oDictItem.IsInt() ) nXHeight = oDictItem.GetInt();
							oDictItem.Free();

							// StemV
							oFontDescriptor.DictLookup("StemV", &oDictItem);
							if ( oDictItem.IsInt() ) nStemV = oDictItem.GetInt();
							oDictItem.Free();

							// StemH
							oFontDescriptor.DictLookup("StemH", &oDictItem);
							if ( oDictItem.IsInt() ) nStemH = oDictItem.GetInt();
							oDictItem.Free();

							// Descent
							oFontDescriptor.DictLookup("Descent", &oDictItem);
							if ( oDictItem.IsInt() ) nDescent = oDictItem.GetInt();
							oDictItem.Free();

							// AvgWidth
							oFontDescriptor.DictLookup("AvgWidth", &oDictItem);
							if ( oDictItem.IsInt() ) nAvgWidth = oDictItem.GetInt();
							oDictItem.Free();

							// MaxWidth
							oFontDescriptor.DictLookup("MaxWidth", &oDictItem);
							if ( oDictItem.IsInt() ) nMaxWidth = oDictItem.GetInt();
							oDictItem.Free();

							// MissingWidth
							oFontDescriptor.DictLookup("MissingWidth", &oDictItem);
							if ( oDictItem.IsInt() ) nMissingWidth = oDictItem.GetInt();
							oDictItem.Free();

						}
						oFontDescriptor.Free();

						XmlUtils::CXmlWriter oWriter;

						oWriter.WriteNodeBegin( _T("FontProperties") );

						oWriter.WriteNodeBegin( _T("Name"), TRUE );
						oWriter.WriteAttribute( _T("value"), sFontName );
						oWriter.WriteNodeEnd( _T("Name"), TRUE, TRUE );

						if ( 0 != nItalicAngle )
						{
							oWriter.WriteNodeBegin( _T("Style"), TRUE );
							oWriter.WriteAttribute( _T("italic"), 1 );
							oWriter.WriteNodeEnd( _T("Style"), TRUE, TRUE );
						}

						oWriter.WriteNodeBegin( _T("AvgCharWidth"), TRUE );
						oWriter.WriteAttribute( _T("value"), nAvgWidth );
						oWriter.WriteNodeEnd( _T("AvgCharWidth"), TRUE, TRUE );

						oWriter.WriteNodeBegin( _T("Ascent"), TRUE );
						oWriter.WriteAttribute( _T("value"), nAscent );
						oWriter.WriteNodeEnd( _T("Ascent"), TRUE, TRUE );

						oWriter.WriteNodeBegin( _T("Descent"), TRUE );
						oWriter.WriteAttribute( _T("value"), nDescent );
						oWriter.WriteNodeEnd( _T("Descent"), TRUE, TRUE );

						oWriter.WriteNodeBegin( _T("LineGap"), TRUE );
						oWriter.WriteAttribute( _T("value"), nLeading );
						oWriter.WriteNodeEnd( _T("LineGap"), TRUE, TRUE );

						oWriter.WriteNodeBegin( _T("xHeight"), TRUE );
						oWriter.WriteAttribute( _T("value"), nXHeight );
						oWriter.WriteNodeEnd( _T("xHeight"), TRUE, TRUE );

						oWriter.WriteNodeBegin( _T("CapHeight"), TRUE );
						oWriter.WriteAttribute( _T("value"), nCapHeight );
						oWriter.WriteNodeEnd( _T("CapHeight"), TRUE, TRUE );

						oWriter.WriteNodeEnd( _T("FontProperties") );

						BSTR bsParams = oWriter.GetXmlString().AllocSysString();
						BSTR bsPath = NULL;
						LONG lIndex = 0;
						m_pFontManager->GetWinFontByParams( bsParams, &bsPath, &lIndex );
						::SysFreeString( bsParams );

						wsFileName = bsPath;
						::SysFreeString( bsPath );
						eFontType  = pFont->IsCIDFont() ? fontCIDType2 : fontTrueType;
					}
					else
					{
						pEntry->bAvailable = TRUE;
						if ( _T("") != wsTempFileName ) 
							_wunlink( wsTempFileName.GetBuffer() );

						return;
					}
				}
				else
				{
					StringExt seName("Arial Unicode MS");
					pDisplayFontParam = m_pGlobalParams->GetDisplayFont( &seName );

					if ( !pDisplayFontParam )
					{
						StringExt seArial("Arial");
						pDisplayFontParam = m_pGlobalParams->GetDisplayFont( &seArial );

						if ( !pDisplayFontParam )
						{
							StringExt seHelvetica("Helvetica");
							pDisplayFontParam = m_pGlobalParams->GetDisplayFont( &seHelvetica );
						}
					}

					// В крайнем случае, в данном шрифте просто не пишем ничего
					if ( !pDisplayFontParam )
					{
						pEntry->bAvailable = TRUE;
						if ( _T("") != wsTempFileName ) 
							_wunlink( wsTempFileName.GetBuffer() );

						return;
					}

					switch ( pDisplayFontParam->m_eType ) 
					{
					case displayFontT1:
						wsFileName = *(pDisplayFontParam->m_oT1.pwsFileName);
						eFontType  = pFont->IsCIDFont() ? fontCIDType0 : fontType1;
						break;
					case displayFontTT:
						wsFileName = *(pDisplayFontParam->m_oTT.pwsFileName);
						eFontType  = pFont->IsCIDFont() ? fontCIDType2 : fontTrueType;
						break;
					}
				}
			}

			wchar_t wsDrive[MAX_PATH], wsDir[MAX_PATH], wsFilename[MAX_PATH], wsExt[MAX_PATH];
			_wsplitpath( wsFileName.GetBuffer(), wsDrive, wsDir, wsFilename, wsExt );

			// Копируем файл, для создания уникального имени, чтобы связать с файлом с кодировкой
			FILE *pTempFile = NULL;

			if ( !pFont->IsCIDFont() )
			{
				wsExt[0] = '.';
				wsExt[1] = 'n';
				wsExt[2] = 'o';
				wsExt[3] = 'n';
				wsExt[4] = '\0';
			}
			else if ( pFont->IsCIDFont() )
			{
				wsExt[0] = '.';
				wsExt[1] = 'c';
				wsExt[2] = 'i';
				wsExt[3] = 'd';
				wsExt[4] = '_';
				wsExt[5] = 'n';
				wsExt[6] = 'o';
				wsExt[7] = 'n';
				wsExt[8] = '\0';
			}

			if ( !OpenTempFile( &wsTempFileName, &pTempFile, _T("wb"), wsExt, m_pGlobalParams->GetTempFolder() ) ) 
			{
				// TO DO: Eerror "Couldn't create temporary font file"
				if ( _T("") != wsTempFileName ) 
				{
					_wunlink( wsTempFileName.GetBuffer() );
				}

				pEntry->bAvailable = TRUE;
				return;
			}
			fclose( pTempFile );

			if ( CopyFile( wsFileName.GetBuffer(), wsTempFileName.GetBuffer(), FALSE ) )
			{
				wsFileName = wsTempFileName;
			}
			else
			{
				_wunlink( wsTempFileName.GetBuffer() );
				wsTempFileName = _T("");
			}

			if ( _T("") != wsTempFileName )
			{
				_wsplitpath( wsFileName.GetBuffer(), wsDrive, wsDir, wsFilename, wsExt );

				// Записываем файл с кодировкой. (Специально для перезаписи в PDF)
				CStringW wsEncodingPath = CStringW( wsDrive ) + CStringW( wsDir ) + CStringW( wsFilename ) + CStringW( _T(".enc") );

				XmlUtils::CXmlWriter oXmlWriter;

				Ref *pRef = pFont->GetID();
				Object oRefObject, oFontObject;
				oRefObject.InitRef( pRef->nNum, pRef->nGen );
				oRefObject.Fetch( m_pXref, &oFontObject );
				oRefObject.Free();

				if ( oFontObject.IsDict() )
				{
					Dict *pFontDict = oFontObject.GetDict();

					int nEncodingType = -1; // Объекта Encoding нет
					int nBaseEncoding = -1;

					Object oDictItem;
					pFontDict->Search("Encoding", &oDictItem);
					if ( oDictItem.IsDict() )
					{
						nEncodingType = 1; // Encoding - идет отдельным объектом

						Object oTemp;
						oDictItem.DictLookup("BaseEncoding", &oTemp);

						if ( oTemp.IsName("MacRomanEncoding") ) 
							nBaseEncoding = 0;
						else if ( oTemp.IsName("MacExpertEncoding") ) 
							nBaseEncoding = 1;
						else if ( oTemp.IsName("WinAnsiEncoding") ) 
							nBaseEncoding = 2;

						oTemp.Free();
					} 
					else if ( oDictItem.IsName("MacRomanEncoding") ) 
					{
						nEncodingType = 0;
						nBaseEncoding = 0;
					} 
					else if ( oDictItem.IsName("MacExpertEncoding") ) 
					{
						nEncodingType = 0;
						nBaseEncoding = 1;
					} 
					else if ( oDictItem.IsName("WinAnsiEncoding") ) 
					{
						nEncodingType = 0;
						nBaseEncoding = 2;
					}
					else
					{
						nEncodingType = -1;
					}

					oXmlWriter.WriteNodeBegin( _T("Encoding"), TRUE);
					oXmlWriter.WriteAttribute( _T("type"), nEncodingType);
					oXmlWriter.WriteAttribute( _T("base"), nBaseEncoding);
					oXmlWriter.WriteNodeEnd( _T("Encoding"), TRUE, FALSE);

					// Differences
					if ( oDictItem.IsDict() ) 
					{
						Object oDifferences;
						oDictItem.DictLookup("Differences", &oDifferences);
						if ( oDifferences.IsArray() ) 
						{
							int nArrayLen = oDifferences.ArrayGetLength();
							oXmlWriter.WriteNodeBegin( _T("Differences"), TRUE);
							oXmlWriter.WriteAttribute( _T("count"), nArrayLen);
							oXmlWriter.WriteNodeEnd( _T("Differences"), TRUE, FALSE);

							for ( int nIndex = 0; nIndex < nArrayLen; ++nIndex )
							{
								Object oTemp;
								oDifferences.ArrayGet( nIndex, &oTemp);
								if ( oTemp.IsInt() ) 
								{
									int nCode = oTemp.GetInt();
									oXmlWriter.WriteNodeBegin( _T("Code"), TRUE);
									oXmlWriter.WriteAttribute( _T("value"), nCode);
									oXmlWriter.WriteNodeEnd( _T("Code"), TRUE, TRUE);
								} 
								else if ( oTemp.IsName() ) 
								{
									USES_CONVERSION;
									char *sName = oTemp.GetName();
									oXmlWriter.WriteNodeBegin( _T("Name"), TRUE);
									oXmlWriter.WriteAttribute( _T("value"), A2W(sName) );
									oXmlWriter.WriteNodeEnd( _T("Name"), TRUE, TRUE);
								} 
								else 
								{
									// TO DO: Error "Wrong type in font encoding resource differences"
								}
								oTemp.Free();
							}

							oXmlWriter.WriteNodeEnd( _T("Differences") );
						}
						oDifferences.Free();
					}
					oDictItem.Free();

					oXmlWriter.WriteNodeEnd( _T("Encoding") );

				}

				oFontObject.Free();

				oXmlWriter.SaveToFile( wsEncodingPath );
			}
		}

		// Здесь мы грузим кодировки

		unsigned short *pCodeToGID = NULL, *pCodeToUnicode = NULL;
		int nLen = 0;
		CFontFileTrueType *pTTFontFile  = NULL;
		CFontFileType1C   *pT1CFontFile = NULL;

		switch ( eFontType ) 
		{
		case fontType1:
		case fontType1C:
		case fontType1COT:

			if ( _T("") != wsFileName )
			{
				char **ppEncoding = ((Gr8BitFont *)pFont)->GetEncoding();
				if ( !ppEncoding )
					break;

				if ( !m_pFontManager )
					break;

				BSTR bsFileName = wsFileName.AllocSysString();
				m_pFontManager->LoadFontFromFile( bsFileName, 1, 0, 0, 0 );
				::SysFreeString( bsFileName );

				pCodeToGID = (unsigned short *)MemUtilsMallocArray( 256, sizeof(unsigned short) );
				if ( !pCodeToGID )
					break;

				USES_CONVERSION;
				nLen = 256;
				for ( int nIndex = 0; nIndex < 256; ++nIndex ) 
				{
					pCodeToGID[nIndex] = 0;

					char *sName = NULL;
					if ( ( sName = ppEncoding[nIndex] ) ) 
					{
						unsigned short ushGID = 0;
						m_pFontManager->GetNameIndex( A2W(sName), &ushGID );
						pCodeToGID[nIndex] = ushGID;
					}
				}
			}
			break;

		case fontTrueType:
		case fontTrueTypeOT:

			if ( ( pTTFontFile = CFontFileTrueType::LoadFromFile( wsFileName.GetBuffer() ) ) ) 
			{
				pCodeToGID = ((Gr8BitFont *)pFont)->GetCodeToGIDMap( pTTFontFile );
				nLen = 256;

				//// Перезаписываем файл, оставляя только необходимую информацию
				//wsTempFileName = _T("");
				//FILE *pTempFile = NULL;
				//if ( !OpenTempFile( &wsTempFileName, &pTempFile, _T("wb"), _T(".ttf"), m_pGlobalParams->GetTempFolder() ) ) 
				//{
				//	delete pTTFontFile;
				//	MemUtilsFree( pCodeToGID );
				//	break;
				//}

				//USES_CONVERSION;
				//pTTFontFile->WriteTTF( &FileWrite, pTempFile, W2A(wsFontName) );
				//delete pTTFontFile;
				//fclose( pTempFile );

				//_wunlink( wsFileName.GetBuffer() );
				//wsFileName = wsTempFileName;

				//BSTR bsFileName = wsFileName.AllocSysString();
				//m_pFontManager->LoadFontFromFile( bsFileName, 1, 0, 0, 0 );
				//::SysFreeString( bsFileName );

				delete pTTFontFile;
				pTTFontFile = NULL;
			} 
			else 
			{
				pCodeToGID = NULL;
				nLen = 0;
			}

			break;
		case fontCIDType0:
		case fontCIDType0C:

			//if ( ( pT1CFontFile = СFontFileType1C::LoadFromFile( wsFileName.GetBuffer() ) ) ) 
			//{
			//	pCodeToGID = pT1CFontFile->GetCIDToGIDMap( &nLen );
			//	delete pT1CFontFile;
			//} 
			//else 
			//{
			pCodeToGID = NULL;
			nLen = 0;
			//}
			break;
		case fontCIDType0COT:

			if ( ( pTTFontFile = CFontFileTrueType::LoadFromFile( wsFileName.GetBuffer() ) ) ) 
			{
				if ( pTTFontFile->IsOpenTypeCFF() ) 
				{
					pCodeToGID = pTTFontFile->GetCIDToGIDMap( &nLen );
				}
				delete pTTFontFile;
				pTTFontFile = NULL;
			}
			else
			{
				pCodeToGID = NULL;
				nLen = 0;
			}

			break;
		case fontCIDType2:
		case fontCIDType2OT:

			pCodeToGID = NULL;
			nLen = 0;
			if ( _T("") != wsFileName && pDisplayFontParam ) 
			{
				// Создаем карту CID-to-GID
				CharCodeToUnicode *pCodeToUnicode = NULL;
				if ( ( pCodeToUnicode = ((GrCIDFont *)pFont)->GetToUnicode() ) ) 
				{
					if ( ( pTTFontFile = CFontFileTrueType::LoadFromFile( wsFileName.GetBuffer() ) ) ) 
					{
						// Ищем Unicode Cmap
						CSimpleArray<int> arrCMapIndex;
						for ( int nCMapIndex = 0; nCMapIndex < pTTFontFile->GetCmapsCount(); ++nCMapIndex )  
						{
							if ( ( pTTFontFile->GetCmapPlatform(nCMapIndex) == 3 && pTTFontFile->GetCmapEncoding(nCMapIndex) == 1 ) || pTTFontFile->GetCmapPlatform(nCMapIndex) == 0 ) 
							{
								arrCMapIndex.Add( nCMapIndex );
							}
						}
						if ( arrCMapIndex.GetSize() > 0 ) 
						{
							// CID -> Unicode -> GID
							nLen = pCodeToUnicode->GetLength();
							pCodeToGID = (unsigned short *)MemUtilsMallocArray( nLen, sizeof(unsigned short) );
							for ( int nCode = 0; nCode < nLen; ++nCode ) 
							{
								Unicode arrUnicodeBuffer[8];
								if ( pCodeToUnicode->MapToUnicode( nCode, arrUnicodeBuffer, 8 ) > 0 ) 
								{
									pCodeToGID[nCode] = pTTFontFile->MapCodeToGID( arrCMapIndex[0], arrUnicodeBuffer[0] );
									for ( int nIndex = 1; nIndex < arrCMapIndex.GetSize(); nIndex++ )
									{
										if ( 0 == pCodeToGID[nCode] )
											pCodeToGID[nCode] = pTTFontFile->MapCodeToGID( arrCMapIndex[nIndex], arrUnicodeBuffer[0] );
										else 
											break;
									}
								} 
								else 
								{
									pCodeToGID[nCode] = nCode;
								}
							}
						}
						delete pTTFontFile;
						pTTFontFile = NULL;
					}
					pCodeToUnicode->Release();
				} 
				else 
				{
					if ( ((GrCIDFont *)pFont)->GetCIDToGID() ) 
					{
						nLen = ((GrCIDFont *)pFont)->GetCIDToGIDLen();
						pCodeToGID = (unsigned short *)MemUtilsMallocArray( nLen, sizeof(unsigned short));
						if ( !pCodeToGID )
							break;

						memcpy( pCodeToGID, ((GrCIDFont *)pFont)->GetCIDToGID(), nLen * sizeof(unsigned short) );
					}
				}
			} 
			else 
			{
				if ( ((GrCIDFont *)pFont)->GetCIDToGID() ) 
				{
					nLen = ((GrCIDFont *)pFont)->GetCIDToGIDLen();
					pCodeToGID = (unsigned short *)MemUtilsMallocArray( nLen, sizeof(unsigned short));
					if ( !pCodeToGID )
						break;

					memcpy( pCodeToGID, ((GrCIDFont *)pFont)->GetCIDToGID(), nLen * sizeof(unsigned short) );
				}
			}

			break;
		default:
			// Такого не должно произойти

			if ( _T("") != wsTempFileName ) 
			{
				_wunlink( wsTempFileName.GetBuffer() );
			}
			break;
		}

		// Составляем таблицу Code -> Unicode 

		int nToUnicodeLen = 0;
		if ( pFont->IsCIDFont() )
		{
			GrCIDFont *pCIDFont = (GrCIDFont *)pFont;
			CharCodeToUnicode *pToUnicode = pCIDFont->GetToUnicode();
			if ( NULL != pToUnicode )
			{
				nToUnicodeLen = pToUnicode->GetLength();
				pCodeToUnicode = (unsigned short *)MemUtilsMallocArray( nToUnicodeLen, sizeof(unsigned short) );

				if ( pCodeToUnicode )
				{
					for ( int nIndex = 0; nIndex < nToUnicodeLen; ++nIndex )
					{
						Unicode aUnicode[2];
						if ( pToUnicode->MapToUnicode( nIndex, aUnicode, 2 ) )
							pCodeToUnicode[nIndex] = (unsigned short)aUnicode[0];
						else
							pCodeToUnicode[nIndex] = 0;
					}
				}

				pToUnicode->Release();
			}
		}
		else
		{
			CharCodeToUnicode *pToUnicode = ((Gr8BitFont *)pFont)->GetToUnicode();
			if ( NULL != pToUnicode )
			{
				nToUnicodeLen = pToUnicode->GetLength();
				pCodeToUnicode = (unsigned short *)MemUtilsMallocArray( nToUnicodeLen, sizeof(unsigned short) );

				if ( pCodeToUnicode )
				{
					for ( int nIndex = 0; nIndex < nToUnicodeLen; ++nIndex )
					{
						Unicode nUnicode = 0;
						if ( pToUnicode->MapToUnicode( nIndex, &nUnicode, 1 ) )
							pCodeToUnicode[nIndex] = (unsigned short)nUnicode;
						else
							pCodeToUnicode[nIndex] = nIndex;
					}
				}
				pToUnicode->Release();
			}
		}

		if ( _T("") != wsFileName )
		{
			// Записываем файл с настройками шрифта (Специально для перезаписи в PDF)
			wchar_t wsDrive[MAX_PATH], wsDir[MAX_PATH], wsFilename[MAX_PATH];
			_wsplitpath( wsFileName.GetBuffer(), wsDrive, wsDir, wsFilename, NULL );
			CStringW wsEncodingPath = CStringW( wsDrive ) + CStringW( wsDir ) + CStringW( wsFilename ) + CStringW( _T(".enc") );

			GrFontType eFontType = pFont->GetType();
			if ( fontType1 == eFontType || fontType1C == eFontType || fontType1COT == eFontType || fontTrueType == eFontType || fontTrueTypeOT == eFontType )
			{
				// Запись информации для простых шрифтов

				XmlUtils::CXmlWriter oXmlWriter;

				Ref *pRef = pFont->GetID();
				Object oRefObject, oFontObject;
				oRefObject.InitRef( pRef->nNum, pRef->nGen );
				oRefObject.Fetch( m_pXref, &oFontObject );
				oRefObject.Free();

				if ( oFontObject.IsDict() )
				{
					Dict *pFontDict = oFontObject.GetDict();

					int nEncodingType = -1; // Объекта Encoding нет
					int nBaseEncoding = -1;

					Object oDictItem;
					pFontDict->Search("Encoding", &oDictItem);
					if ( oDictItem.IsDict() )
					{
						nEncodingType = 1; // Encoding - идет отдельным объектом

						Object oTemp;
						oDictItem.DictLookup("BaseEncoding", &oTemp);

						if ( oTemp.IsName("MacRomanEncoding") ) 
							nBaseEncoding = 0;
						else if ( oTemp.IsName("MacExpertEncoding") ) 
							nBaseEncoding = 1;
						else if ( oTemp.IsName("WinAnsiEncoding") ) 
							nBaseEncoding = 2;

						oTemp.Free();
					} 
					else if ( oDictItem.IsName("MacRomanEncoding") ) 
					{
						nEncodingType = 0;
						nBaseEncoding = 0;
					} 
					else if ( oDictItem.IsName("MacExpertEncoding") ) 
					{
						nEncodingType = 0;
						nBaseEncoding = 1;
					} 
					else if ( oDictItem.IsName("WinAnsiEncoding") ) 
					{
						nEncodingType = 0;
						nBaseEncoding = 2;
					}
					else
					{
						nEncodingType = -1;
					}

					oXmlWriter.WriteNodeBegin( _T("PDF-resources") );
					oXmlWriter.WriteNodeBegin( _T("Encoding"), TRUE);
					oXmlWriter.WriteAttribute( _T("type"), nEncodingType);
					oXmlWriter.WriteAttribute( _T("base"), nBaseEncoding);
					oXmlWriter.WriteNodeEnd( _T("Encoding"), TRUE, FALSE);

					// Differences
					if ( oDictItem.IsDict() ) 
					{
						Object oDifferences;
						oDictItem.DictLookup("Differences", &oDifferences);
						if ( oDifferences.IsArray() ) 
						{
							int nArrayLen = oDifferences.ArrayGetLength();
							oXmlWriter.WriteNodeBegin( _T("Differences"), TRUE);
							oXmlWriter.WriteAttribute( _T("count"), nArrayLen);
							oXmlWriter.WriteNodeEnd( _T("Differences"), TRUE, FALSE);

							for ( int nIndex = 0; nIndex < nArrayLen; ++nIndex )
							{
								Object oTemp;
								oDifferences.ArrayGet( nIndex, &oTemp);
								if ( oTemp.IsInt() ) 
								{
									int nCode = oTemp.GetInt();
									oXmlWriter.WriteNodeBegin( _T("Code"), TRUE);
									oXmlWriter.WriteAttribute( _T("value"), nCode);
									oXmlWriter.WriteNodeEnd( _T("Code"), TRUE, TRUE);
								} 
								else if ( oTemp.IsName() ) 
								{
									USES_CONVERSION;
									char *sName = oTemp.GetName();
									oXmlWriter.WriteNodeBegin( _T("Name"), TRUE);
									oXmlWriter.WriteAttribute( _T("value"), A2W(sName) );
									oXmlWriter.WriteNodeEnd( _T("Name"), TRUE, TRUE);
								} 
								else 
								{
									// TO DO: Error "Wrong type in font encoding resource differences"
								}
								oTemp.Free();
							}

							oXmlWriter.WriteNodeEnd( _T("Differences") );
						}
						oDifferences.Free();
					}
					oDictItem.Free();

					oXmlWriter.WriteNodeEnd( _T("Encoding") );

					pFontDict->Search("BaseFont", &oDictItem);
					if ( oDictItem.IsName() )
					{
						USES_CONVERSION;
						oXmlWriter.WriteNodeBegin( _T("FontBase"), TRUE );
						oXmlWriter.WriteAttribute( _T("value"), A2W( oDictItem.GetName() ) );

						wsFontName = CStringW( oDictItem.GetName() );
						oXmlWriter.WriteNodeEnd( _T("FontBase"), TRUE, TRUE );
					}
					oDictItem.Free();

					pFontDict->Search("ToUnicode", &oDictItem);
					if ( oDictItem.IsStream() )
					{
						oXmlWriter.WriteNodeBegin( _T("ToUnicode") );

						StringExt *seBuffer = new StringExt();
						if ( NULL != seBuffer )
						{
							oDictItem.StreamReset();
							int nChar = 0;
							while ( ( nChar = oDictItem.StreamGetChar() ) != EOF ) 
							{
								seBuffer->Append( nChar );
							}
							oDictItem.StreamClose();

							CBase64 oBase64;
							oBase64.Encode( (BYTE *)seBuffer->GetBuffer(), seBuffer->GetLength() );
							oXmlWriter.WriteString( CString(oBase64.GetString().c_str()) );

							delete seBuffer;
						}

						oXmlWriter.WriteNodeEnd( _T("ToUnicode") );
					}
					oDictItem.Free();


					oXmlWriter.WriteNodeBegin( _T("FontDescriptor") );

					Object oFontDescriptor;
					if ( pFontDict->Search("FontDescriptor", &oFontDescriptor)->IsDict() )
					{
						// FontName
						oFontDescriptor.DictLookup("FontName", &oDictItem);
						if ( oDictItem.IsName() )
						{
							oXmlWriter.WriteNodeBegin( _T("FontName"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), CStringW( oDictItem.GetName() ) );
							oXmlWriter.WriteNodeEnd( _T("FontName"), TRUE );
						}
						oDictItem.Free();

						// FontFamily
						oFontDescriptor.DictLookup("FontFamily", &oDictItem);
						if ( oDictItem.IsName() )
						{
							oXmlWriter.WriteNodeBegin( _T("FontFamily"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), CStringW( oDictItem.GetName() ) );
							oXmlWriter.WriteNodeEnd( _T("FontFamily"), TRUE );
						}
						oDictItem.Free();

						// FontStretch
						CStringA sFontStretch;
						oFontDescriptor.DictLookup("FontStretch", &oDictItem);
						if ( oDictItem.IsName() )
						{
							oXmlWriter.WriteNodeBegin( _T("FontStretch"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), CStringW( oDictItem.GetName() ) );
							oXmlWriter.WriteNodeEnd( _T("FontStretch"), TRUE );
						}
						oDictItem.Free();

						// FontWeight
						oFontDescriptor.DictLookup("FontWeight", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("FontWeight"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("FontWeight"), TRUE );
						}
						oDictItem.Free();

						// FontWeight
						oFontDescriptor.DictLookup("Flags", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("Flags"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("Flags"), TRUE );
						}
						oDictItem.Free();

						// FontBBox
						oFontDescriptor.DictLookup("FontBBox", &oDictItem);
						if ( oDictItem.IsArray() )
						{
							Array *pBBox = oDictItem.GetArray();
							if ( NULL != pBBox && 4 == pBBox->GetCount() )
							{
								oXmlWriter.WriteNodeBegin( _T("FontBBox"), TRUE );
								for ( int nIndex = 0; nIndex < 4; nIndex++ )
								{
									Object oArrayItem;
									pBBox->Get( nIndex, &oArrayItem );
									if ( oArrayItem.IsInt() )
									{
										CString sValue;
										sValue.Format( _T("value%d"), nIndex );
										oXmlWriter.WriteAttribute( sValue, oArrayItem.GetInt() );
									}

									oArrayItem.Free();
								}
								oXmlWriter.WriteNodeEnd( _T("FontBBox"), TRUE );
							}
						}
						oDictItem.Free();

						// ItalicAngle
						oFontDescriptor.DictLookup("ItalicAngle", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("ItalicAngle"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("ItalicAngle"), TRUE );
						}
						oDictItem.Free();

						// Ascent
						oFontDescriptor.DictLookup("Ascent", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("Ascent"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("Ascent"), TRUE );
						}
						oDictItem.Free();

						// Leading
						oFontDescriptor.DictLookup("Leading", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("Leading"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("Leading"), TRUE );
						}
						oDictItem.Free();

						// CapHeight
						oFontDescriptor.DictLookup("CapHeight", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("CapHeight"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("CapHeight"), TRUE );
						}
						oDictItem.Free();

						// XHeight
						oFontDescriptor.DictLookup("XHeight", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("XHeight"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("XHeight"), TRUE );
						}
						oDictItem.Free();

						// StemV
						oFontDescriptor.DictLookup("StemV", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("StemV"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("StemV"), TRUE );
						}
						oDictItem.Free();

						// StemH
						oFontDescriptor.DictLookup("StemH", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("StemH"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("StemH"), TRUE );
						}
						oDictItem.Free();

						// Descent
						oFontDescriptor.DictLookup("Descent", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("Descent"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("Descent"), TRUE );
						}
						oDictItem.Free();

						// AvgWidth
						oFontDescriptor.DictLookup("AvgWidth", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("AvgWidth"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("AvgWidth"), TRUE );
						}
						oDictItem.Free();

						// MaxWidth
						oFontDescriptor.DictLookup("MaxWidth", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("MaxWidth"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("MaxWidth"), TRUE );
						}
						oDictItem.Free();

						// MissingWidth
						oFontDescriptor.DictLookup("MissingWidth", &oDictItem);
						if ( oDictItem.IsInt() )
						{
							oXmlWriter.WriteNodeBegin( _T("MissingWidth"), TRUE );
							oXmlWriter.WriteAttribute( _T("value"), oDictItem.GetInt() );
							oXmlWriter.WriteNodeEnd( _T("MissingWidth"), TRUE );
						}
						oDictItem.Free();

					}
					oFontDescriptor.Free();

					oXmlWriter.WriteNodeEnd( _T("FontDescriptor") );


					int nFirstChar = 0;
					pFontDict->Search("FirstChar", &oDictItem);
					if ( oDictItem.IsInt() ) nFirstChar = oDictItem.GetInt();
					oDictItem.Free();

					int nLastChar = 0;
					pFontDict->Search("LastChar", &oDictItem);
					if ( oDictItem.IsInt() ) nLastChar = oDictItem.GetInt();
					oDictItem.Free();

					Array *pWidths = NULL;
					pFontDict->Search("Widths", &oDictItem );
					if ( oDictItem.IsArray() )
					{
						oXmlWriter.WriteNodeBegin( _T("Widths"), TRUE );
						oXmlWriter.WriteAttribute( _T("FirstChar"), nFirstChar );
						oXmlWriter.WriteAttribute( _T("LastChar"), nLastChar );
						oXmlWriter.WriteNodeEnd( _T("Widths"), TRUE, FALSE );

						Array *pWidths = oDictItem.GetArray();
						int nWidthsCount = pWidths->GetCount();
						for ( int nIndex = 0; nIndex < nWidthsCount; nIndex++ )
						{
							Object oArrayItem;
							pWidths->Get( nIndex, &oArrayItem );
							if ( oArrayItem.IsInt() )
							{
								oXmlWriter.WriteNodeBegin( _T("Width"), TRUE );
								oXmlWriter.WriteAttribute( _T("value"), oArrayItem.GetInt() );
								oXmlWriter.WriteNodeEnd( _T("Width"), TRUE );
							}
							oArrayItem.Free();
						}
						oXmlWriter.WriteNodeEnd( _T("Widths") );
					}
					oDictItem.Free();



					oXmlWriter.WriteNodeEnd( _T("PDF-resources") );
				}
				oFontObject.Free();

				oXmlWriter.SaveToFile( wsEncodingPath );
			}
			else if ( fontCIDType0 == eFontType || fontCIDType0C == eFontType || fontCIDType0COT == eFontType || fontCIDType2 == eFontType || fontCIDType2OT == eFontType )
			{
				// Пишем файл с кодировкой CMap
				//CStringW wsCMapPath = CStringW( wsDrive ) + CStringW( wsDir ) + CStringW( wsFilename ) + CStringW( _T(".cmap") );
				//
				//if ( pFont->IsCIDFont() )
				//{
				//	GrCIDFont *pCIDFont = (GrCIDFont *)pFont;
				//	if ( NULL != pCIDFont->GetCMap() )
				//		pCIDFont->GetCMap()->ToXml( wsCMapPath );
				//}


				XmlUtils::CXmlWriter oXmlWriter;

				Ref *pRef = pFont->GetID();
				Object oRefObject, oFontObject;
				oRefObject.InitRef( pRef->nNum, pRef->nGen );
				oRefObject.Fetch( m_pXref, &oFontObject );
				oRefObject.Free();

				if ( oFontObject.IsDict() )
				{
					USES_CONVERSION;

					Dict *pFontDict = oFontObject.GetDict();

					oXmlWriter.WriteNodeBegin( _T("PDF-resources") );

					Object oDictItem;
					pFontDict->Search("BaseFont", &oDictItem);
					if ( oDictItem.IsName() )
					{
						oXmlWriter.WriteNodeBegin( _T("Type0"), TRUE );
						oXmlWriter.WriteAttribute( _T("value"), wchar_t( oDictItem.GetName() ) );
						wsFontName = CStringW( oDictItem.GetName() );
						oXmlWriter.WriteNodeEnd( _T("Type0"), TRUE, FALSE );
					}
					else
					{
						oXmlWriter.WriteNodeBegin( _T("Type0") );
					}
					oDictItem.Free();

					pFontDict->Search("ToUnicode", &oDictItem);
					if ( oDictItem.IsStream() )
					{
						oXmlWriter.WriteNodeBegin( _T("ToUnicode") );

						StringExt *seBuffer = new StringExt();
						oDictItem.StreamReset();
						int nChar = 0;
						while ( ( nChar = oDictItem.StreamGetChar() ) != EOF ) 
						{
							seBuffer->Append( nChar );
						}
						oDictItem.StreamClose();

						CBase64 oBase64;
						oBase64.Encode( (BYTE *)seBuffer->GetBuffer(), seBuffer->GetLength() );
						oXmlWriter.WriteString( CString(oBase64.GetString().c_str()) );

						oXmlWriter.WriteNodeEnd( _T("ToUnicode") );
					}
					oDictItem.Free();

					pFontDict->Search("Encoding", &oDictItem);
					if ( oDictItem.IsName() )
					{
						oXmlWriter.WriteNodeBegin( _T("Encoding"), TRUE );
						oXmlWriter.WriteAttribute( _T("name"), A2W(oDictItem.GetName()) );
						oXmlWriter.WriteNodeEnd( _T("Encoding"), TRUE, TRUE );
					}
					else if ( oDictItem.IsStream() )
					{
						oXmlWriter.WriteNodeBegin( _T("Encoding") );

						Dict *pEncodingDict = oDictItem.StreamGetDict();
						if ( NULL != pEncodingDict )
						{
							Object oEncItem;
							pEncodingDict->Search("CMapName", &oEncItem);
							if ( oEncItem.IsName() )
							{
								oXmlWriter.WriteNodeBegin( _T("CMapName"), TRUE );
								oXmlWriter.WriteAttribute( _T("name"), A2W(oEncItem.GetName()) );
								oXmlWriter.WriteNodeEnd( _T("CMapName"), TRUE, TRUE );
							}
							oEncItem.Free();

							pEncodingDict->Search("CIDSystemInfo", &oEncItem);
							if ( oEncItem.IsDict() )
							{
								Dict *pCIDInfo = oEncItem.GetDict();

								if ( NULL != pCIDInfo )
								{
									oXmlWriter.WriteNodeBegin( _T("CIDSystemInfo") );

									Object oCIDInfoItem;
									pCIDInfo->Search("Registry", &oCIDInfoItem);
									if ( oCIDInfoItem.IsString() )
									{
										oXmlWriter.WriteNodeBegin( _T("Registry"), TRUE);
										oXmlWriter.WriteAttribute( _T("string"), A2W(oCIDInfoItem.GetString()->GetBuffer()) );
										oXmlWriter.WriteNodeEnd( _T("Registry"), TRUE, TRUE);
									}
									oCIDInfoItem.Free();

									pCIDInfo->Search("Ordering", &oCIDInfoItem);
									if ( oCIDInfoItem.IsString() )
									{
										oXmlWriter.WriteNodeBegin( _T("Ordering"), TRUE);
										oXmlWriter.WriteAttribute( _T("string"), A2W(oCIDInfoItem.GetString()->GetBuffer()) );
										oXmlWriter.WriteNodeEnd( _T("Ordering"), TRUE, TRUE);
									}
									oCIDInfoItem.Free();

									pCIDInfo->Search("Supplement", &oCIDInfoItem);
									if ( oCIDInfoItem.IsInt() )
									{
										oXmlWriter.WriteNodeBegin( _T("Supplement"), TRUE);
										oXmlWriter.WriteAttribute( _T("integer"), oCIDInfoItem.GetInt() );
										oXmlWriter.WriteNodeEnd( _T("Supplement"), TRUE, TRUE);
									}
									oCIDInfoItem.Free();

									oXmlWriter.WriteNodeEnd( _T("CIDSystemInfo") );
								}
							}
							oEncItem.Free();

							pEncodingDict->Search("WMode", &oEncItem);
							if ( oEncItem.IsInt() )
							{
								oXmlWriter.WriteNodeBegin( _T("WMode"), TRUE );
								oXmlWriter.WriteAttribute( _T("integer"), oEncItem.GetInt() );
								oXmlWriter.WriteNodeEnd( _T("WMode"), TRUE, TRUE );
							}
							oEncItem.Free();

							pEncodingDict->Search("UseCMap", &oEncItem);
							if ( oEncItem.IsName() )
							{
								oXmlWriter.WriteNodeBegin( _T("UseCMap"), TRUE );
								oXmlWriter.WriteAttribute( _T("name"), A2W(oEncItem.GetName()) );
								oXmlWriter.WriteNodeEnd( _T("UseCMap"), TRUE, TRUE );
							}
							else if ( oEncItem.IsStream() )
							{
								oXmlWriter.WriteNodeBegin( _T("UseCMap") );

								StringExt *seBuffer = new StringExt();
								oEncItem.StreamReset();
								int nChar = 0;
								while ( ( nChar = oEncItem.StreamGetChar() ) != EOF ) 
								{
									seBuffer->Append( nChar );
								}
								oEncItem.StreamClose();

								CBase64 oBase64;
								oBase64.Encode( (BYTE *)seBuffer->GetBuffer(), seBuffer->GetLength() );
								oXmlWriter.WriteString( CString(oBase64.GetString().c_str()) );

								oXmlWriter.WriteNodeEnd( _T("UseCMap") );
							}
							oEncItem.Free();
						}

						oXmlWriter.WriteNodeBegin( _T("Stream") );

						StringExt *seBuffer = new StringExt();
						oDictItem.StreamReset();
						int nChar = 0;
						while ( ( nChar = oDictItem.StreamGetChar() ) != EOF ) 
						{
							seBuffer->Append( nChar );
						}
						oDictItem.StreamClose();

						CBase64 oBase64;
						oBase64.Encode( (BYTE *)seBuffer->GetBuffer(), seBuffer->GetLength() );
						oXmlWriter.WriteString( CString(oBase64.GetString().c_str()) );

						oXmlWriter.WriteNodeEnd( _T("Stream") );

						oXmlWriter.WriteNodeEnd( _T("Encoding") );
					}
					oDictItem.Free();

					pFontDict->Search("DescendantFonts", &oDictItem);
					if ( oDictItem.IsArray() )
					{
						Array *pArray =  oDictItem.GetArray();
						if ( 1 == pArray->GetCount() )
						{
							Object oDescFont;
							pArray->Get( 0, &oDescFont );

							if ( oDescFont.IsDict() )
							{
								Dict *pDescFont = oDescFont.GetDict();
								if ( NULL != pDescFont )
								{
									oXmlWriter.WriteNodeBegin( _T("DescendantFonts") );

									Object oFontItem;
									pDescFont->Search("Subtype", &oFontItem);
									if ( oFontItem.IsName() )
									{
										oXmlWriter.WriteNodeBegin( _T("Subtype"), TRUE );
										oXmlWriter.WriteAttribute( _T("name"), A2W(oFontItem.GetName()) );
										oXmlWriter.WriteNodeEnd( _T("Subtype"), TRUE, TRUE );
									}
									oFontItem.Free();

									pDescFont->Search("BaseFont", &oFontItem);
									if ( oFontItem.IsName() )
									{
										oXmlWriter.WriteNodeBegin( _T("BaseFont"), TRUE );
										oXmlWriter.WriteAttribute( _T("name"), wchar_t(oFontItem.GetName()) );
										oXmlWriter.WriteNodeEnd( _T("BaseFont"), TRUE, TRUE );
									}
									oFontItem.Free();

									pDescFont->Search("CIDSystemInfo", &oFontItem);
									if ( oFontItem.IsDict() )
									{
										Dict *pCIDInfo = oFontItem.GetDict();
										if ( NULL != pCIDInfo )
										{
											oXmlWriter.WriteNodeBegin( _T("CIDSystemInfo") );

											Object oCIDInfoItem;
											pCIDInfo->Search("Registry", &oCIDInfoItem);
											if ( oCIDInfoItem.IsString() )
											{
												oXmlWriter.WriteNodeBegin( _T("Registry"), TRUE);
												oXmlWriter.WriteAttribute( _T("string"), A2W(oCIDInfoItem.GetString()->GetBuffer()) );
												oXmlWriter.WriteNodeEnd( _T("Registry"), TRUE, TRUE);
											}
											oCIDInfoItem.Free();

											pCIDInfo->Search("Ordering", &oCIDInfoItem);
											if ( oCIDInfoItem.IsString() )
											{
												oXmlWriter.WriteNodeBegin( _T("Ordering"), TRUE);
												oXmlWriter.WriteAttribute( _T("string"), A2W(oCIDInfoItem.GetString()->GetBuffer()) );
												oXmlWriter.WriteNodeEnd( _T("Ordering"), TRUE, TRUE);
											}
											oCIDInfoItem.Free();

											pCIDInfo->Search("Supplement", &oCIDInfoItem);
											if ( oCIDInfoItem.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("Supplement"), TRUE);
												oXmlWriter.WriteAttribute( _T("integer"), oCIDInfoItem.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("Supplement"), TRUE, TRUE);
											}
											oCIDInfoItem.Free();

											oXmlWriter.WriteNodeEnd( _T("CIDSystemInfo") );
										}
									}
									oFontItem.Free();

									pDescFont->Search("FontDescriptor", &oFontItem);
									if ( oFontItem.IsDict() )
									{
										Dict *pFontDescriptor = oFontItem.GetDict();
										if ( NULL != pFontDescriptor )
										{
											oXmlWriter.WriteNodeBegin( _T("FontDescriptor") );
											Object oItemFD;

											pFontDescriptor->Search("FontName", &oItemFD);
											if ( oItemFD.IsName() )
											{
												oXmlWriter.WriteNodeBegin( _T("FontName"), TRUE);
												oXmlWriter.WriteAttribute( _T("name"), wchar_t( oItemFD.GetName() ) );
												oXmlWriter.WriteNodeEnd( _T("FontName"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("FontStretch", &oItemFD);
											if ( oItemFD.IsName() )
											{
												oXmlWriter.WriteNodeBegin( _T("FontStretch"), TRUE);
												oXmlWriter.WriteAttribute( _T("name"), wchar_t( oItemFD.GetName() ) );
												oXmlWriter.WriteNodeEnd( _T("FontStretch"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("FontWeight", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("FontWeight"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("FontWeight"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("Flags", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("Flags"), TRUE);
												oXmlWriter.WriteAttribute( _T("integer"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("Flags"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("FontBBox", &oItemFD);
											if ( oItemFD.IsArray() )
											{
												Array *pBBox = oItemFD.GetArray();
												if ( NULL != pBBox && 4 == pBBox->GetCount() )
												{
													oXmlWriter.WriteNodeBegin( _T("FontBBox"), TRUE );
													for ( int nIndex = 0; nIndex < 4; nIndex++ )
													{
														Object oArrayItem;
														pBBox->Get( nIndex, &oArrayItem );
														if ( oArrayItem.IsInt() )
														{
															CString sValue;
															sValue.Format( _T("value%d"), nIndex );
															oXmlWriter.WriteAttribute( sValue, oArrayItem.GetInt() );
														}

														oArrayItem.Free();
													}
													oXmlWriter.WriteNodeEnd( _T("FontBBox"), TRUE );
												}
											}
											oItemFD.Free();

											pFontDescriptor->Search("ItalicAngle", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("ItalicAngle"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("ItalicAngle"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("Ascent", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("Ascent"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("Ascent"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("Descent", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("Descent"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("Descent"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("Leading", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("Leading"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("Leading"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("CapHeight", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("CapHeight"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("CapHeight"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("XHeight", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("XHeight"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("XHeight"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("StemV", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("StemV"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("StemV"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("StemH", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("StemH"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("StemH"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("AvgWidth", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("AvgWidth"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("AvgWidth"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("MaxWidth", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("MaxWidth"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("MaxWidth"), TRUE, TRUE);
											}
											oItemFD.Free();

											pFontDescriptor->Search("MissingWidth", &oItemFD);
											if ( oItemFD.IsInt() )
											{
												oXmlWriter.WriteNodeBegin( _T("MissingWidth"), TRUE);
												oXmlWriter.WriteAttribute( _T("number"), oItemFD.GetInt() );
												oXmlWriter.WriteNodeEnd( _T("MissingWidth"), TRUE, TRUE);
											}
											oItemFD.Free();

											// TO DO: Тут надо реализовать чтени полей /Style, /Lang, /FD, /CIDSet

											oXmlWriter.WriteNodeEnd( _T("FontDescriptor") );
										}
									}
									oFontItem.Free();

									pDescFont->Search("DW", &oFontItem);
									if ( oFontItem.IsInt() )
									{
										oXmlWriter.WriteNodeBegin( _T("DW"), TRUE);
										oXmlWriter.WriteAttribute( _T("integer"), oFontItem.GetInt() );
										oXmlWriter.WriteNodeEnd( _T("DW"), TRUE, TRUE);
									}
									oFontItem.Free();

									pDescFont->Search("W", &oFontItem);
									if ( oFontItem.IsArray() )
									{
										Array *pArray = oFontItem.GetArray();
										if ( NULL != pArray )
										{
											oXmlWriter.WriteNodeBegin( _T("W") );

											for ( int nIndex = 0; nIndex < pArray->GetCount(); nIndex++ )
											{
												Object oArrayItem;
												pArray->Get( nIndex, &oArrayItem );
												if ( oArrayItem.IsInt() )
												{
													oXmlWriter.WriteNodeBegin( _T("Int"), TRUE);
													oXmlWriter.WriteAttribute( _T("value"), oArrayItem.GetInt() );
													oXmlWriter.WriteNodeEnd( _T("Int"), TRUE, TRUE);
												}
												else if ( oArrayItem.IsArray() )
												{
													Array *pWArray = oArrayItem.GetArray();
													if ( NULL != pWArray )
													{
														oXmlWriter.WriteNodeBegin( _T("Array") );
														for ( int nWIndex = 0; nWIndex < pWArray->GetCount(); nWIndex++ )
														{
															Object oWArrayItem;
															pWArray->Get( nWIndex, &oWArrayItem );
															if ( oWArrayItem.IsInt() )
															{
																oXmlWriter.WriteNodeBegin( _T("Int"), TRUE);
																oXmlWriter.WriteAttribute( _T("value"), oWArrayItem.GetInt() );
																oXmlWriter.WriteNodeEnd( _T("Int"), TRUE, TRUE);
															}
															oWArrayItem.Free();
														}
														oXmlWriter.WriteNodeEnd( _T("Array") );
													}
												}
												oArrayItem.Free();
											}

											oXmlWriter.WriteNodeEnd( _T("W") );
										}
									}
									oFontItem.Free();

									pDescFont->Search("DW2", &oFontItem );
									if ( oFontItem.IsArray() )
									{
										Array *pArray = oFontItem.GetArray();
										if ( NULL != pArray && 2 == pArray->GetCount() )
										{
											oXmlWriter.WriteNodeBegin( _T("DW2"), TRUE);

											Object oArrayItem;
											pArray->Get( 0, &oArrayItem );
											if ( oArrayItem.IsInt() )
											{
												oXmlWriter.WriteAttribute( _T("value0"), oArrayItem.GetInt() );
											}
											oArrayItem.Free();

											pArray->Get( 1, &oArrayItem );
											if ( oArrayItem.IsInt() )
											{
												oXmlWriter.WriteAttribute( _T("value1"), oArrayItem.GetInt() );
											}
											oArrayItem.Free();

											oXmlWriter.WriteNodeEnd( _T("DW2"), TRUE, TRUE);
										}
									}
									oFontItem.Free();

									pDescFont->Search("W2", &oFontItem);
									if ( oFontItem.IsArray() )
									{
										Array *pArray = oFontItem.GetArray();
										if ( NULL != pArray )
										{
											oXmlWriter.WriteNodeBegin( _T("W2") );

											for ( int nIndex = 0; nIndex < pArray->GetCount(); nIndex++ )
											{
												Object oArrayItem;
												pArray->Get( nIndex, &oArrayItem );
												if ( oArrayItem.IsInt() )
												{
													oXmlWriter.WriteNodeBegin( _T("Int"), TRUE);
													oXmlWriter.WriteAttribute( _T("value"), oArrayItem.GetInt() );
													oXmlWriter.WriteNodeEnd( _T("Int"), TRUE, TRUE);
												}
												else if ( oArrayItem.IsArray() )
												{
													Array *pWArray = oArrayItem.GetArray();
													if ( NULL != pWArray )
													{
														oXmlWriter.WriteNodeBegin( _T("Array") );
														for ( int nWIndex = 0; nWIndex < pWArray->GetCount(); nWIndex++ )
														{
															Object oWArrayItem;
															pWArray->Get( nWIndex, &oWArrayItem );
															if ( oWArrayItem.IsInt() )
															{
																oXmlWriter.WriteNodeBegin( _T("Int"), TRUE);
																oXmlWriter.WriteAttribute( _T("value"), oWArrayItem.GetInt() );
																oXmlWriter.WriteNodeEnd( _T("Int"), TRUE, TRUE);
															}
															oWArrayItem.Free();
														}
														oXmlWriter.WriteNodeEnd( _T("Array") );
													}
												}
												oArrayItem.Free();
											}

											oXmlWriter.WriteNodeEnd( _T("W2") );
										}
									}
									oFontItem.Free();

									pDescFont->Search("CIDToGIDMap", &oFontItem);
									if ( oFontItem.IsName() )
									{
										oXmlWriter.WriteNodeBegin( _T("CIDToGIDMap"), TRUE);
										oXmlWriter.WriteAttribute( _T("name"), A2W( oFontItem.GetName() ) );
										oXmlWriter.WriteNodeEnd( _T("CIDToGIDMap"), TRUE, TRUE);
									}
									else if ( oFontItem.IsStream() )
									{
										oXmlWriter.WriteNodeBegin( _T("CIDToGIDMap") );

										StringExt *seBuffer = new StringExt();
										oFontItem.StreamReset();
										int nChar = 0;
										while ( ( nChar = oFontItem.StreamGetChar() ) != EOF ) 
										{
											seBuffer->Append( nChar );
										}
										oFontItem.StreamClose();

										CBase64 oBase64;
										oBase64.Encode( (BYTE *)seBuffer->GetBuffer(), seBuffer->GetLength() );
										oXmlWriter.WriteString( CString(oBase64.GetString().c_str()) );

										oXmlWriter.WriteNodeEnd( _T("CIDToGIDMap") );
									}
									oFontItem.Free();


									oXmlWriter.WriteNodeEnd( _T("DescendantFonts") );
								}
							}
							oDescFont.Free();
						}
					}
					oDictItem.Free();

					oXmlWriter.WriteNodeEnd( _T("Type0") );
					oXmlWriter.WriteNodeEnd( _T("PDF-resources") );
				}
				oFontObject.Free();

				oXmlWriter.SaveToFile( wsEncodingPath );
			}
		}

		// Обрежем индекс у FontName, если он есть
		if ( wsFontName.GetLength() > 7 )
		{
			BOOL bIsIndex = TRUE;
			if ( '+' != wsFontName.GetAt(6) )
				bIsIndex = FALSE;

			if ( bIsIndex )
			{
				for ( int nIndex = 0; nIndex < 6; nIndex++ )
				{
					int nChar = wsFontName.GetAt(nIndex);
					if ( nChar < 'A' || nChar > 'Z' )
					{
						bIsIndex = FALSE;
						break;
					}
				}
			}

			if ( bIsIndex )
			{
				wsFontName.Delete( 0, 7 );
			}
		}

		pEntry->wsFilePath     = wsFileName;
		pEntry->wsFontName     = wsFontName;
		pEntry->pCodeToGID     = pCodeToGID;
		pEntry->pCodeToUnicode = pCodeToUnicode;
		pEntry->unLenGID       = (unsigned int)nLen;
		pEntry->unLenUnicode   = (unsigned int)nToUnicodeLen;
		pEntry->bAvailable     = TRUE;
	}
	else if ( NULL != pEntry )
	{
		wsFileName = pEntry->wsFilePath;
		wsFontName = pEntry->wsFontName;
	}

	if ( _T("") != wsFileName )
	{
		BSTR bsFontPath = wsFileName.AllocSysString();
		BSTR bsFontName = wsFontName.AllocSysString();
		m_pRenderer->put_FontPath( bsFontPath );
		m_pRenderer->put_FontName( bsFontName );
		::SysFreeString( bsFontPath );
		::SysFreeString( bsFontName );
	}
}

void RendererOutputDev::Stroke(GrState *pGState)
{
	if ( m_bTransparentGroup )
		return;

	DoPath( pGState, pGState->GetPath(), pGState->GetPageHeight(), pGState->GetCTM() );
	m_pRenderer->DrawPath( c_nStroke );

	m_pRenderer->EndCommand( c_nPathType );
}
void RendererOutputDev::Fill(GrState *pGState)
{
	if ( m_bTransparentGroup )
		return;

	DoPath( pGState, pGState->GetPath(), pGState->GetPageHeight(), pGState->GetCTM() );
	m_pRenderer->DrawPath( c_nWindingFillMode );

	m_pRenderer->EndCommand( c_nPathType );
}
void RendererOutputDev::EoFill(GrState *pGState)
{
	if ( m_bTransparentGroup )
		return;

	DoPath( pGState, pGState->GetPath(), pGState->GetPageHeight(), pGState->GetCTM() );
	m_pRenderer->DrawPath( c_nEvenOddFillMode );

	m_pRenderer->EndCommand( c_nPathType );
}
void RendererOutputDev::FillStroke(GrState *pGState) 
{
	if ( m_bTransparentGroup )
		return;

	DoPath( pGState, pGState->GetPath(), pGState->GetPageHeight(), pGState->GetCTM() );
	m_pRenderer->DrawPath( c_nStroke | c_nWindingFillMode );

	m_pRenderer->EndCommand( c_nPathType );
}
void RendererOutputDev::EoFillStroke(GrState *pGState) 
{
	if ( m_bTransparentGroup )
		return;

	DoPath( pGState, pGState->GetPath(), pGState->GetPageHeight(), pGState->GetCTM() );
	m_pRenderer->DrawPath( c_nStroke | c_nEvenOddFillMode );

	m_pRenderer->EndCommand( c_nPathType );
}


void RendererOutputDev::TilingPatternFill(GrState *pGState, Object *pStream, int nPaintType, Dict *pResourcesDict, double *pMatrix, double *pBBox, int nX0, int nY0, int nX1, int nY1, double dXStep, double dYStep) 
{

}
void RendererOutputDev::StartTilingFill  (GrState *pGState)
{
	m_pRenderer->BeginCommand( c_nComplexFigureType );
	m_bTiling = TRUE;
}
void RendererOutputDev::EndTilingFill     ()
{
	m_pRenderer->EndCommand( c_nComplexFigureType );
	m_bTiling = FALSE;
}
BOOL RendererOutputDev::FunctionShadedFill(GrState *pGState, GrFunctionShading *pShading)
{ 
	return FALSE; 
}
BOOL RendererOutputDev::AxialShadedFill   (GrState *pGState, GrAxialShading    *pShading)
{ 
	m_pRenderer->BeginCommand( c_nPDFGradientType );

	double arrMatrix[6];
	double *pCTM = pGState->GetCTM();
	arrMatrix[0] =  pCTM[0];
	arrMatrix[1] = -pCTM[1];
	arrMatrix[2] =  pCTM[2];
	arrMatrix[3] = -pCTM[3];
	arrMatrix[4] =  pCTM[4];
	arrMatrix[5] = -pCTM[5] + pGState->GetPageHeight();

	double dShiftX = 0, dShiftY = 0;
	DoTransform( arrMatrix, &dShiftX, &dShiftY );

	double dFirstX, dFirstY, dSecondX, dSecondY;
	pShading->GetCoords( &dFirstX, &dFirstY, &dSecondX, &dSecondY );
	
	BOOL bExtendFirst  = pShading->GetExtendStart();
	BOOL bExtendSecond = pShading->GetExtendEnd  ();

	GrColorSpace *pColorSpace = pGState->GetFillColorSpace();

	LONG lOldType;
	m_pRenderer->get_BrushType( &lOldType );

	NSStructures::CBrush oBrush;
	oBrush.Rect.X      = PDFCoordsToMM( dFirstX + dShiftX );
	oBrush.Rect.Y      = PDFCoordsToMM( dFirstY + dShiftY );
	oBrush.Rect.Width  = PDFCoordsToMM( dSecondX - dFirstX );
	oBrush.Rect.Height = PDFCoordsToMM( dSecondY - dFirstY );
	oBrush.Type        = 2;
	oBrush.TextureMode = bExtendFirst ? 1 : 0 + bExtendSecond ? 2 : 0;

	BSTR bsXml = oBrush.ToXmlString().AllocSysString();
	m_pRenderer->SetBrush( bsXml );
	::SysFreeString( bsXml );

	double dXMin, dYMin, dXMax, dYMax;
	pGState->GetUserClipBBox( &dXMin, &dYMin, &dXMax, &dYMax);

	oBrush.Rect.X      = PDFCoordsToMM( dXMin + dShiftX );
	oBrush.Rect.Y      = PDFCoordsToMM( dYMin + dShiftY );
	oBrush.Rect.Width  = PDFCoordsToMM( dXMax - dXMin );
	oBrush.Rect.Height = PDFCoordsToMM( dYMax - dYMin );
	oBrush.Type        = 2;
	oBrush.TextureMode = 5;

	bsXml = oBrush.ToXmlString().AllocSysString();
	m_pRenderer->SetBrush( bsXml );
	::SysFreeString( bsXml );

	// Присылаем 16 цветов
	double dT0 = pShading->GetDomain0();
	double dT1 = pShading->GetDomain1();
	GrColor oColor;
	for ( int nIndex = 0; nIndex < 16; nIndex++ )
	{
		double dT = dT0 + nIndex * ( dT1 - dT0 ) / 15;
		pShading->GetColor( dT, &oColor );

		oBrush.Color1      = pColorSpace->GetDwordColor( &oColor );
		oBrush.Type        = 2;
		oBrush.TextureMode = 10;

		bsXml = oBrush.ToXmlString().AllocSysString();
		m_pRenderer->SetBrush( bsXml );
		::SysFreeString( bsXml );
	}

	m_pRenderer->EndCommand( c_nPDFGradientType );

	UpdateFillColor( pGState );
	UpdateFillOpacity( pGState );
	
	m_pRenderer->put_BrushType( lOldType );

	return TRUE;  
}
BOOL RendererOutputDev::RadialShadedFill  (GrState *pGState, GrRadialShading   *pShading)
{
	m_pRenderer->BeginCommand( c_nPDFGradientType );

	double arrMatrix[6];
	double *pCTM = pGState->GetCTM();
	arrMatrix[0] =  pCTM[0];
	arrMatrix[1] = -pCTM[1];
	arrMatrix[2] =  pCTM[2];
	arrMatrix[3] = -pCTM[3];
	arrMatrix[4] =  pCTM[4];
	arrMatrix[5] = -pCTM[5] + pGState->GetPageHeight();

	double dShiftX = 0, dShiftY = 0;
	DoTransform( arrMatrix, &dShiftX, &dShiftY );

	double dFirstX, dFirstY, dFirstRad, dSecondX, dSecondY, dSecondRad;
	pShading->GetCoords( &dFirstX, &dFirstY, &dFirstRad, &dSecondX, &dSecondY, &dSecondRad );
	
	BOOL bExtendFirst  = pShading->GetExtendFirst ();
	BOOL bExtendSecond = pShading->GetExtendSecond();

	LONG lOldType;
	m_pRenderer->get_BrushType( &lOldType );

	NSStructures::CBrush oBrush;
	oBrush.Rect.X      = PDFCoordsToMM( dFirstX + dShiftX );
	oBrush.Rect.Y      = PDFCoordsToMM( dFirstY + dShiftY );
	oBrush.Rect.Width  = PDFCoordsToMM( dFirstRad );
	oBrush.Rect.Height = PDFCoordsToMM( dFirstRad );
	oBrush.Type        = 1;
	oBrush.TextureMode = bExtendFirst ? 1 : 0;

	BSTR bsXml = oBrush.ToXmlString().AllocSysString();
	m_pRenderer->SetBrush( bsXml );
	::SysFreeString( bsXml );

	oBrush.Rect.X      = PDFCoordsToMM( dSecondX + dShiftX );
	oBrush.Rect.Y      = PDFCoordsToMM( dSecondY + dShiftY );
	oBrush.Rect.Width  = PDFCoordsToMM( dSecondRad );
	oBrush.Rect.Height = PDFCoordsToMM( dSecondRad );
	oBrush.Type        = 1;
	oBrush.TextureMode = bExtendSecond ? 1 : 0;

	bsXml = oBrush.ToXmlString().AllocSysString();
	m_pRenderer->SetBrush( bsXml );
	::SysFreeString( bsXml );

	double dXMin, dYMin, dXMax, dYMax;
	pGState->GetUserClipBBox( &dXMin, &dYMin, &dXMax, &dYMax);

	oBrush.Rect.X      = PDFCoordsToMM( dXMin + dShiftX );
	oBrush.Rect.Y      = PDFCoordsToMM( dYMin + dShiftY );
	oBrush.Rect.Width  = PDFCoordsToMM( dXMax - dXMin );
	oBrush.Rect.Height = PDFCoordsToMM( dYMax - dYMin );
	oBrush.Type        = 1;
	oBrush.TextureMode = 5;

	bsXml = oBrush.ToXmlString().AllocSysString();
	m_pRenderer->SetBrush( bsXml );
	::SysFreeString( bsXml );

	// Присылаем 15 цветов
	double dT0 = pShading->GetDomain0();
	double dT1 = pShading->GetDomain1();
	GrColor oColor;
	GrColorSpace *pColorSpace = pGState->GetFillColorSpace();
	for ( int nIndex = 0; nIndex < 15; nIndex++ )
	{
		double dT = dT0 + nIndex * ( dT1 - dT0 ) / 14;
		pShading->GetColor( dT, &oColor );

		oBrush.Color1      = pColorSpace->GetDwordColor( &oColor );
		oBrush.Type        = 1;
		oBrush.TextureMode = 10;

		bsXml = oBrush.ToXmlString().AllocSysString();
		m_pRenderer->SetBrush( bsXml );
		::SysFreeString( bsXml );
	}

	m_pRenderer->EndCommand( c_nPDFGradientType );

	UpdateFillColor( pGState );
	UpdateFillOpacity( pGState );
	
	m_pRenderer->put_BrushType( lOldType );

	return TRUE; 
}
void RendererOutputDev::StartShadedFill   (GrState *pGState)
{
	m_pRenderer->BeginCommand( c_nComplexFigureType );
}
void RendererOutputDev::EndShadedFill     ()
{
	m_pRenderer->EndCommand( c_nComplexFigureType );
}
void RendererOutputDev::Clip(GrState *pGState)
{
	UpdateClip( pGState );
}
void RendererOutputDev::EoClip(GrState *pGState)
{
	UpdateClip( pGState );
}
void RendererOutputDev::ClipToStrokePath(GrState *pGState)
{
	UpdateClip( pGState );
}
void RendererOutputDev::ClipToPath(GrState *pGState, GrPath *pPath, double *pMatrix, BOOL bEO)
{
	if ( m_bTransparentGroup )
		return;

	int nClipFlag = bEO ? c_nClipRegionTypeEvenOdd : c_nClipRegionTypeWinding;
	nClipFlag |= c_nClipRegionIntersect;

	m_pRenderer->BeginCommand( c_nClipType );
	m_pRenderer->put_ClipMode( nClipFlag );
	DoPath( pGState, pPath, pGState->GetPageHeight(), pMatrix );
	m_pRenderer->EndCommand( c_nPathType );
	m_pRenderer->EndCommand( c_nClipType );
}
void RendererOutputDev::ClipToText(BSTR bsFontName, BSTR bsFontPath, double dFontSize, long lFontStyle, double *pMatrix, CString sText, double dX, double dY, double dWidth, double dHeight, double dBaseLineOffset)
{
	if ( m_bTransparentGroup )
		return;

	m_pRenderer->put_FontName( bsFontName );
	m_pRenderer->put_FontPath( bsFontPath );
	m_pRenderer->put_FontSize( dFontSize );
	m_pRenderer->put_FontStyle( lFontStyle );

	BSTR bsText = sText.AllocSysString();

	double dShiftX = 0, dShiftY = 0;
	DoTransform( pMatrix, &dShiftX, &dShiftY, TRUE );

	m_pRenderer->BeginCommand( c_nClipType );
	m_pRenderer->put_ClipMode( c_nClipRegionTypeWinding | c_nClipRegionUnion );
	m_pRenderer->PathCommandEnd();
	m_pRenderer->put_FontStringGID( TRUE );
	m_pRenderer->PathCommandTextEx( NULL, bsText, NULL, PDFCoordsToMM( dX ), PDFCoordsToMM( dY ), PDFCoordsToMM( dWidth ), PDFCoordsToMM( dHeight ), PDFCoordsToMM( dBaseLineOffset ), 0 );
	m_pRenderer->EndCommand( c_nClipType );

	::SysFreeString( bsText );
}
void RendererOutputDev::BeginStringOperator(GrState *pGState) 
{
	if ( m_bTransparentGroup )
		return;

	m_pRenderer->BeginCommand( c_nTextType );

	int nRenderMode = pGState->GetRenderMode();

	// Обработка Clip
	if ( nRenderMode >= 4 )
	{
		if ( m_pBufferTextClip )
			delete m_pBufferTextClip;

		m_pBufferTextClip = new GrTextClip();
	}

	// Обработка Stroke
	if ( 1 == nRenderMode || 2 == nRenderMode || 5 == nRenderMode || 6 == nRenderMode )
	{
		//Painter::CPen oPen;

		//m_pRenderer->put_PenSize( PDFCoordsToMM( pGState->GetFontSize() ) * 0.05 );
		//m_pRenderer->put_PenAlpha(  255 );
		//oPen.SetColor( m_oPen.GetColor() );

		//BSTR bsPen = oPen.ToXmlString().AllocSysString();
		//m_pRenderer->SetPen( bsPen );
		//::SysFreeString( bsPen );
	}
}
void RendererOutputDev::EndStringOperator(GrState *pGState) 
{
	if ( m_bTransparentGroup )
		return;

	int nRenderMode = pGState->GetRenderMode();

	// Добавляем в Clipping Path текст
	if ( nRenderMode >= 4 )
	{
		if ( m_pBufferTextClip )
			pGState->GetClip()->AppendTextClip( m_pBufferTextClip );

		for ( int nIndex = 0; nIndex < m_pBufferTextClip->GetTextsCount(); nIndex++ )
		{
			BSTR bsFontName, bsFontPath;
			LONG lFontStyle;
			double dFontSize = 10, dX = 0, dY = 0, dWidth = 0, dHeight = 0, dBaseLineOffset = 0;
			BSTR bsText = m_pBufferTextClip->GetText( nIndex, &dX, &dY, &dWidth, &dHeight, &dBaseLineOffset, &bsFontName, &bsFontPath, &dFontSize, &lFontStyle );
			double *pMatrix = m_pBufferTextClip->GetMatrix( nIndex );

			ClipToText( bsFontName, bsFontPath, dFontSize, lFontStyle, pMatrix, CString( bsText ), dX, dY, dWidth, dHeight, dBaseLineOffset );
		}

		UpdateFont( pGState );
	}

	// Возвращаем параметры для Stroke
	if ( 1 == nRenderMode || 2 == nRenderMode || 5 == nRenderMode || 6 == nRenderMode )
	{
		//BSTR bsPen = m_oPen.ToXmlString().AllocSysString();
		//m_pRenderer->SetPen( bsPen );
		//::SysFreeString( bsPen );
	}

	m_pRenderer->EndCommand( c_nTextType );
}
void RendererOutputDev::DrawString(GrState *pGState, StringExt *seString)
{
	if ( m_bTransparentGroup )
		return;

	// Проверяем наличие списка со шрифтами
	if ( NULL == m_pFontList )
		return;

	// Проверяем наличие текущего шрифта 
	TFontEntry oEntry;
	if ( !m_pFontList->GetFont( pGState->GetFont()->GetID(), &oEntry ) )
		return;

	int nRendererMode = pGState->GetRenderMode();

	if ( 3 == nRendererMode ) // Невидимый текс
		return;

	double *pCTM  = pGState->GetCTM();
	double *pTm   = pGState->GetTextMatrix();
	GrFont *pFont = pGState->GetFont();

	CString sUnicodeText, sGIDText, sSrcCodeText;

	for ( int nIndex = 0; nIndex < seString->GetLength(); nIndex++ )
	{
		char nChar = seString->GetAt( nIndex );

		if ( NULL != oEntry.pCodeToUnicode )
		{
			unsigned short unUnicode = oEntry.pCodeToUnicode[nChar];
			sUnicodeText += (CString)( wchar_t(unUnicode) );
		}

		if ( NULL != oEntry.pCodeToGID )
		{
			unsigned short unGID;
			if ( 0 != ( unGID = oEntry.pCodeToGID[nChar] ) )
				sGIDText += (CString)( wchar_t( unGID ) );
		}
		else
		{
			int nCurCode = ( 0 == nChar ? 65534 : nChar );
			sGIDText += (CString)( wchar_t( nCurCode ) );
		}

	}

	BSTR bsUnicodeText = sUnicodeText.AllocSysString();
	BSTR bsGIDText     = sGIDText.AllocSysString();
	BSTR bsSrcCodeText = NULL;

	m_pRenderer->CommandDrawTextEx( bsUnicodeText, bsGIDText, bsSrcCodeText, PDFCoordsToMM( 100 ), PDFCoordsToMM( 100 ), 0, PDFCoordsToMM( 0 ), PDFCoordsToMM( 0 ), 0 );

	::SysFreeString( bsUnicodeText );
	::SysFreeString( bsGIDText );

	return;
}
void RendererOutputDev::DrawChar(GrState *pGState, double dX, double dY, double dDx, double dDy, double dOriginX, double dOriginY, CharCode nCode, int nBytesCount, Unicode *pUnicode, int nUnicodeLen)
{
	if ( m_bTransparentGroup )
		return;

	// Проверяем наличие списка со шрифтами
	if ( NULL == m_pFontList )
		return;

	// Проверяем наличие текущего шрифта 
	TFontEntry oEntry;
	if ( !m_pFontList->GetFont( pGState->GetFont()->GetID(), &oEntry ) )
		return;

	int   nRenderMode = pGState->GetRenderMode();

	if ( 3 == nRenderMode ) // Невидимый текст
	{
		return;
	}

	double *pCTM      = pGState->GetCTM();
	double *pTm       = pGState->GetTextMatrix();
	GrFont *pFont     = pGState->GetFont();
	//double  dTfs      = pGState->GetFontSize();
	//double  dTh       = pGState->GetHorizScaling();
	//double  dTrise    = pGState->GetRise();
	//double *pFontBBox = pGState->GetFont()->GetFontBBox(); 

	//double  dAcsentFactor = ( ( fabs(pFontBBox[1]) + fabs(pFontBBox[3]) ) - ( pFont->GetAscent() + fabs( pFont->GetDescent() ) ) ) / 2 + pFont->GetAscent();  
	double pNewTm[6], arrMatrix[6];
	//double dKoef = 0;//( pFont->GetAscent() - fabs( pFont->GetDescent() ) ) * dTfs;

	double dTextScale = min( sqrt( pTm[2] * pTm[2] + pTm[3] * pTm[3] ), sqrt( pTm[0] * pTm[0] + pTm[1] * pTm[1] ) );
	double dITextScale = 1 / dTextScale;

	//pTm[0] *= dITextScale;
	//pTm[1] *= dITextScale;
	//pTm[2] *= dITextScale;
	//pTm[3] *= dITextScale;

	//double dOldSize = m_oFont.Size;
	//m_oFont.Size *= dTextScale;
	double dOldSize = 10.0;
	m_pRenderer->get_FontSize( &dOldSize );
	m_pRenderer->put_FontSize( dOldSize * dTextScale );

	pNewTm[0] =  pTm[0] * dITextScale;
	pNewTm[1] =  pTm[1] * dITextScale;
	pNewTm[2] = -pTm[2] * dITextScale;
	pNewTm[3] = -pTm[3] * dITextScale;
	//pNewTm[0] =  pTm[0];
	//pNewTm[1] =  pTm[1];
	//pNewTm[2] = -pTm[2];
	//pNewTm[3] = -pTm[3];
	pNewTm[4] =  dX ;//+ pTm[2] * dKoef;
	pNewTm[5] =  dY ;//+ pTm[3] * dKoef;

	arrMatrix[0] =   pNewTm[0] * pCTM[0] + pNewTm[1] * pCTM[2];
	arrMatrix[1] = -(pNewTm[0] * pCTM[1] + pNewTm[1] * pCTM[3]);
	arrMatrix[2] =   pNewTm[2] * pCTM[0] + pNewTm[3] * pCTM[2];
	arrMatrix[3] = -(pNewTm[2] * pCTM[1] + pNewTm[3] * pCTM[3]);
	arrMatrix[4] =   pNewTm[4] * pCTM[0] + pNewTm[5] * pCTM[2] + pCTM[4];
	arrMatrix[5] = -(pNewTm[4] * pCTM[1] + pNewTm[5] * pCTM[3] + pCTM[5]) + pGState->GetPageHeight();

	//double dAscentShiftX = arrMatrix[2] * pFont->GetDescent();
	//double dAscentShiftY = arrMatrix[3] * pFont->GetDescent();

	//arrMatrix[4] += dAscentShiftX;
	//arrMatrix[5] += dAscentShiftY;

	double dShiftX = 0, dShiftY = 0;
	DoTransform( arrMatrix, &dShiftX, &dShiftY, TRUE );

	// Здесь мы посылаем координаты текста в пунктах

	double dPageHeight = pGState->GetPageHeight();

	BSTR bsUnicodeText = NULL, bsGIDText = NULL, bsSrcCodeText = NULL;

	if ( NULL != oEntry.pCodeToUnicode && nCode < oEntry.unLenUnicode )
	{
		unsigned short unUnicode = oEntry.pCodeToUnicode[nCode];
		bsUnicodeText = ((CString)( wchar_t( unUnicode ) )).AllocSysString();
	}
	else
	{
		if ( pGState->GetFont()->IsCIDFont() )
		{
			// Значит кодировка была Identity-H или Identity-V, что означает, что иходные коды и есть юникодные значения
			bsUnicodeText = ((CString)( wchar_t( nCode ) )).AllocSysString();
		}
		else
		{
			// Договорились, что если нельзя точно составить юникодные значения, тогда отдаем NULL
			bsUnicodeText = NULL;
		}
	}

	if ( NULL != oEntry.pCodeToGID && nCode < oEntry.unLenGID )
	{
		unsigned short unGID;
		if ( 0 == ( unGID = oEntry.pCodeToGID[nCode] ) )
			bsGIDText = NULL;
		else
			bsGIDText = ((CString)( wchar_t( unGID ) )).AllocSysString();
	}
	else
	{
		int nCurCode = ( 0 == nCode ? 65534 : nCode );
		bsGIDText = ((CString)( wchar_t( nCurCode ) )).AllocSysString();
	}	

	if ( 1 == m_nRendererType )
	{
		int nCurCode = ( 0 == nCode ? 65534 : nCode );
		if ( pGState->GetFont()->IsCIDFont() )
		{
			// Мы посылаем и сам CID и внутренний Code с его длинной
			XmlUtils::CXmlWriter oWriter;
			char pBuffer[10];
			oWriter.WriteNodeBegin( _T("PDF-Text"), TRUE );
			_itoa( nCurCode, pBuffer, 10 );
			oWriter.WriteAttribute( _T("cid"), CString(pBuffer) );
			_itoa( pUnicode[0], pBuffer, 10 );
			oWriter.WriteAttribute( _T("code"), CString(pBuffer) );
			_itoa( nUnicodeLen, pBuffer, 10 );
			oWriter.WriteAttribute( _T("len"), CString(pBuffer) );
			oWriter.WriteNodeEnd( _T("PDF-Text"), TRUE, TRUE );
			bsSrcCodeText = oWriter.GetXmlString().AllocSysString();
		}
		else
		{
			// Мы посылаем и сам CID и внутренний Code с его длинной
			XmlUtils::CXmlWriter oWriter;
			char pBuffer[10];
			oWriter.WriteNodeBegin( _T("PDF-Text"), TRUE );
			_itoa( nCurCode, pBuffer, 10 );
			oWriter.WriteAttribute( _T("code"), CString(pBuffer) );
			oWriter.WriteNodeEnd( _T("PDF-Text"), TRUE, TRUE );
			bsSrcCodeText = oWriter.GetXmlString().AllocSysString();
		}
	}
	else
	{
		bsSrcCodeText = ::SysAllocString( _T("") );
	}

	float fAscent = pGState->GetFontSize();

	if ( nRenderMode == 0 || nRenderMode == 2 || nRenderMode == 4 || nRenderMode == 6 )
	{
		m_pRenderer->CommandDrawTextEx( bsUnicodeText, bsGIDText, bsSrcCodeText, PDFCoordsToMM( 0 + dShiftX ), PDFCoordsToMM( dShiftY ), PDFCoordsToMM( dDx ), PDFCoordsToMM( dDy ), PDFCoordsToMM( 0 ), 0 );
	}

	if ( nRenderMode == 1 || nRenderMode == 2 || nRenderMode == 5 || nRenderMode == 6 )
	{
		m_pRenderer->BeginCommand( c_nStrokeTextType );

		m_pRenderer->PathCommandEnd();
		//m_pRenderer->PathCommandText( bsText, PDFCoordsToMM( 0 + dShiftX ), PDFCoordsToMM( /*-fabs(pFont->GetFontBBox()[3]) * dTfs*/ + dShiftY ), PDFCoordsToMM( 0 ), PDFCoordsToMM( 0 ), PDFCoordsToMM( 0 ) );


		// Временно
		//m_pRenderer->PathCommandTextEx( bsText, PDFCoordsToMM( 0 + dShiftX ), PDFCoordsToMM( /*-fabs(pFont->GetFontBBox()[3]) * dTfs*/ + dShiftY ), PDFCoordsToMM( 0 ), PDFCoordsToMM( 0 ), PDFCoordsToMM( 0 ), 0, bsStringGID );
		//m_pRenderer->PathCommandTextEx( bsUnicodeText, bsGIDText, bsSrcCodeText, PDFCoordsToMM( 0 + dShiftX ), PDFCoordsToMM( /*-fabs(pFont->GetFontBBox()[3]) * dTfs*/0 + dShiftY ), PDFCoordsToMM( dDx ), PDFCoordsToMM( dDy ), PDFCoordsToMM( 0 ), 0 );
		//-----------


		//m_pRenderer->PathCommandText( bsUnicodeText, PDFCoordsToMM( 0 + dShiftX ), PDFCoordsToMM( /*-fabs(pFont->GetFontBBox()[3]) * dTfs*/ + dShiftY ), PDFCoordsToMM( dDx ), PDFCoordsToMM( dDy ), PDFCoordsToMM( 0 ) );
		//m_pRenderer->DrawPath( c_nStroke );

		m_pRenderer->EndCommand( c_nStrokeTextType );
	}

	if ( 4 <= nRenderMode )
	{
		BSTR bsTempFontName, bsTempFontPath;
		double dTempFontSize;
		LONG lTempFontStyle;
		m_pRenderer->get_FontName( &bsTempFontName );
		m_pRenderer->get_FontPath( &bsTempFontPath );
		m_pRenderer->get_FontSize( &dTempFontSize );
		m_pRenderer->get_FontStyle( &lTempFontStyle );


		m_pBufferTextClip->ClipToText( (CString)bsTempFontName, (CString)bsTempFontPath, dTempFontSize, lTempFontStyle, arrMatrix, (CString)bsGIDText/* ( wchar_t( *pUnicode ) )*/, 0 + dShiftX, /*-fabs(pFont->GetFontBBox()[3]) * dTfs*/ + dShiftY, 0, 0, 0 );

		if ( bsTempFontName ) ::SysFreeString( bsTempFontName );
		if ( bsTempFontPath ) ::SysFreeString( bsTempFontPath );	
	}

	if ( bsUnicodeText ) ::SysFreeString( bsUnicodeText );
	if ( bsGIDText     ) ::SysFreeString( bsGIDText     );	
	if ( bsSrcCodeText ) ::SysFreeString( bsSrcCodeText );

	m_pRenderer->put_FontSize( dOldSize );
}

BOOL RendererOutputDev::BeginType3Char(GrState *pGState, double dX, double dY, double dDx, double dDy, CharCode nCode, Unicode *pUnicode, int nUnicodeLen)
{
	return FALSE;
}
void RendererOutputDev::EndType3Char(GrState *pGState) 
{
	return;
}

void RendererOutputDev::Type3D0(GrState *pGState, double dWx, double dWy) 
{
	return;
}

void RendererOutputDev::Type3D1(GrState *pGState, double dWx, double dWy, double dBLx, double dBLy, double dTRx, double dTRy) 
{
	return;
}

void RendererOutputDev::DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, BOOL bInvert, BOOL bInlineImage)
{
	if ( pGState->GetFillColorSpace()->IsNonMarking() ) 
	{
		return;
	}

	double dPageHeight = pGState->GetPageHeight();

	// Подготавливаем pInterface
	IUnknown *pInterface = NULL;
	HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
	if ( hRes != S_OK || NULL == pInterface )
		return;

	MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)pInterface;

	int nBufferSize = 4 * nWidth * nHeight;
	if ( nBufferSize < 1 )
		return;

	pMediaData->put_ColorSpace( 1 << 6 | 1 << 31 ); // CPS_BGRA | CPS_FLIP
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
		return;

	pInterface = pMediaData;

	// Пишем данные в pBufferPtr
	ImageStream *pImageStream = new ImageStream( pStream, nWidth, 1, 1 );

	pImageStream->Reset();

	unsigned char unAlpha = m_bTransparentGroup ? 255.0 * pGState->GetFillOpacity() : 255;
	unsigned char unPixel = 0;
	for( int nY = nHeight - 1; nY >= 0; nY-- )
	{
		unsigned char *pMask = NULL;
		int nX = 0;
		for ( nX = 0, pMask = pImageStream->GetNextLine(); nX < nWidth; nX++ )
		{
			int nIndex = 4 * ( nX + nY * nWidth );
			unsigned char unPixel = *pMask++ ^ bInvert;
			pBufferPtr[nIndex + 0] = unPixel ? 255 : 0;
			pBufferPtr[nIndex + 1] = unPixel ? 255 : 0;
			pBufferPtr[nIndex + 2] = unPixel ? 255 : 0;
			pBufferPtr[nIndex + 3] = unPixel ? 0 : unAlpha;
		}
	}

	delete pImageStream;

	double arrMatrix[6];
	double *pCTM = pGState->GetCTM();

	//  Исходное предобразование
	//              |1  0  0|   |pCTM[0] pCTM[1] 0| 
	// arrMattrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
	//              |0  1  1|   |pCTM[4] pCTM[5] 1|

	arrMatrix[0] =     pCTM[0];
	arrMatrix[1] =  -  pCTM[1];
	arrMatrix[2] =    -pCTM[2];
	arrMatrix[3] =  -(-pCTM[3]);
	arrMatrix[4] =     pCTM[2] + pCTM[4];
	arrMatrix[5] =  -( pCTM[3] + pCTM[5] ) + dPageHeight;

	double dShiftX = 0, dShiftY = 0; 
	DoTransform( arrMatrix, &dShiftX, &dShiftY, TRUE );
	m_pRenderer->DrawImage( pInterface, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM( 1 ), PDFCoordsToMM( 1 ) );

	RELEASEINTERFACE( pInterface );
}
void RendererOutputDev::DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pMaskColors, BOOL bInlineImg)
{
	double dPageHeight = pGState->GetPageHeight();

	// Подготавливаем pInterface
	IUnknown *pInterface = NULL;
	HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
	if ( hRes != S_OK || NULL == pInterface )
		return;

	MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)pInterface;

	int nBufferSize = 4 * nWidth * nHeight;
	if ( nBufferSize < 1 )
		return;

	pMediaData->put_ColorSpace( 1 << 6 | 1 << 31 ); // CPS_BGRA | CPS_FLIP
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
		return;

	pInterface = pMediaData;

	// Пишем данные в pBufferPtr
	ImageStream *pImageStream = new ImageStream( pStream, nWidth, pColorMap->GetComponentsCount(), pColorMap->GetBitsPerComponent() );

	pImageStream->Reset();

	unsigned char unAlpha = m_bTransparentGroup ? 255.0 * pGState->GetFillOpacity() : 255;

	unsigned char unPixel[32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for( int nY = nHeight - 1; nY >= 0; nY-- )
	{
		for ( int nX = 0; nX < nWidth; nX++ )
		{
			int nIndex = 4 * ( nX + nY * nWidth );
			pImageStream->GetPixel( unPixel );
			GrRGB oRGB;
			pColorMap->GetRGB( unPixel, &oRGB );
			pBufferPtr[nIndex + 0] = ColorToByte( oRGB.b );
			pBufferPtr[nIndex + 1] = ColorToByte( oRGB.g );
			pBufferPtr[nIndex + 2] = ColorToByte( oRGB.r );
			pBufferPtr[nIndex + 3] = unAlpha;
		}
	}

	delete pImageStream;

	double arrMatrix[6];
	double *pCTM = pGState->GetCTM();
	//  Исходное предобразование
	//             |1  0  0|   |pCTM[0] pCTM[1] 0| 
	// arrMatrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
	//             |0  1  1|   |pCTM[4] pCTM[5] 1|
	arrMatrix[0] =     pCTM[0];
	arrMatrix[1] =  -  pCTM[1];
	arrMatrix[2] =    -pCTM[2];
	arrMatrix[3] =  -(-pCTM[3]);
	arrMatrix[4] =     pCTM[2] + pCTM[4];
	arrMatrix[5] =  -( pCTM[3] + pCTM[5] ) + dPageHeight;

	double dShiftX = 0, dShiftY = 0; 
	DoTransform( arrMatrix, &dShiftX, &dShiftY, TRUE );
	m_pRenderer->DrawImage( pInterface, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM( 1 ), PDFCoordsToMM( 1 ) );

	RELEASEINTERFACE( pInterface );
}
void RendererOutputDev::DrawMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, BOOL bMaskInvert)
{	
	// Вообще, размеры маски и самой картинки могут не совпадать (в этом случае мы должны срезайзить до размеров картинки)
	// TO DO: Сделать, когда появится файл
	if ( nWidth != nMaskWidth || nHeight != nMaskHeight )
		DrawImage( pGState, pRef, pStream, nWidth, nHeight, pColorMap, NULL, FALSE );

	double dPageHeight = pGState->GetPageHeight();

	// Подготавливаем pInterface
	IUnknown *pInterface = NULL;
	HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
	if ( hRes != S_OK || NULL == pInterface )
		return;

	MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)pInterface;

	int nBufferSize = 4 * nWidth * nHeight;
	if ( nBufferSize < 1 )
		return;

	pMediaData->put_ColorSpace( 1 << 6 | 1 << 31 ); // CPS_BGRA | CPS_FLIP
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
		return;

	pInterface = pMediaData;

	// Пишем данные в pBufferPtr
	ImageStream *pImageStream = new ImageStream( pStream, nWidth, pColorMap->GetComponentsCount(), pColorMap->GetBitsPerComponent() );

	ImageStream *pMask = new ImageStream( pMaskStream, nMaskWidth, 1, 1 );

	pMask->Reset();
	pImageStream->Reset();

	unsigned char unPixel[4] = { 0, 0, 0, 0};
	unsigned char unMask = 0;
	for( int nY = nHeight - 1; nY >= 0; nY-- )
	{
		for ( int nX = 0; nX < nWidth; nX++ )
		{
			int nIndex = 4 * ( nX + nY * nWidth );
			pImageStream->GetPixel( unPixel );
			pMask->GetPixel( &unMask );
			GrRGB oRGB;
			pColorMap->GetRGB( unPixel, &oRGB );
			pBufferPtr[nIndex + 0] = ColorToByte( oRGB.b );
			pBufferPtr[nIndex + 1] = ColorToByte( oRGB.g );
			pBufferPtr[nIndex + 2] = ColorToByte( oRGB.r );

			if ( unMask && !bMaskInvert )
				pBufferPtr[nIndex + 3] = 0;
			else
				pBufferPtr[nIndex + 3] = 255;
		}
	}

	delete pMask;
	delete pImageStream;

	double arrMatrix[6];
	double *pCTM = pGState->GetCTM();
	//  Исходное предобразование
	//             |1  0  0|   |pCTM[0] pCTM[1] 0| 
	// arrMatrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
	//             |0  1  1|   |pCTM[4] pCTM[5] 1|
	arrMatrix[0] =     pCTM[0];
	arrMatrix[1] =  -  pCTM[1];
	arrMatrix[2] =    -pCTM[2];
	arrMatrix[3] =  -(-pCTM[3]);
	arrMatrix[4] =     pCTM[2] + pCTM[4];
	arrMatrix[5] =  -( pCTM[3] + pCTM[5] ) + dPageHeight;

	double dShiftX = 0, dShiftY = 0; 
	DoTransform( arrMatrix, /*pRenderer2, */&dShiftX, &dShiftY, TRUE );
	m_pRenderer->DrawImage( pInterface, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM( 1 ), PDFCoordsToMM( 1 ) );

	RELEASEINTERFACE( pInterface );
}
void RendererOutputDev::DrawSoftMaskedImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, Stream *pMaskStream, int nMaskWidth, int nMaskHeight, GrImageColorMap *pMaskColorMap)
{
	double dPageHeight = pGState->GetPageHeight();

	// Подготавливаем pInterface
	IUnknown *pInterface = NULL;
	HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
	if ( hRes != S_OK || NULL == pInterface )
		return;

	MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)pInterface;

	int nBufferSize = 4 * nWidth * nHeight;
	if ( nBufferSize < 1 )
		return;

	pMediaData->put_ColorSpace( 1 << 6 | 1 << 31 ); // CPS_BGRA | CPS_FLIP
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
		return;

	pInterface = pMediaData;

	// Пишем данные в pBufferPtr
	ImageStream *pImageStream = new ImageStream( pStream, nWidth, pColorMap->GetComponentsCount(), pColorMap->GetBitsPerComponent() );
	pImageStream->Reset();

	double dAlphaKoef = m_bTransparentGroup ? pGState->GetFillOpacity() : 1;
	unsigned char unPixel[4] = { 0, 0, 0, 0};
	for( int nY = nHeight - 1; nY >= 0; nY-- )
	{
		for ( int nX = 0; nX < nWidth; nX++ )
		{
			int nIndex = 4 * ( nX + nY * nWidth );
			pImageStream->GetPixel( unPixel );
			GrRGB oRGB;
			pColorMap->GetRGB( unPixel, &oRGB );
			pBufferPtr[nIndex + 0] = ColorToByte( oRGB.b );
			pBufferPtr[nIndex + 1] = ColorToByte( oRGB.g );
			pBufferPtr[nIndex + 2] = ColorToByte( oRGB.r );
			pBufferPtr[nIndex + 3] = 255;
		}
	}
	delete pImageStream;

	if ( nWidth != nMaskWidth || nHeight != nMaskHeight )
	{
		// TO DO: Здесь сделан элементарный вариант масштабирования маски.
		//        Надо улучшить алгоритм.

		bool bResize = true;

		if ( 0 != nWidth && 0 != nMaskHeight && 0 != nHeight && 0 != nMaskWidth )
		{
			ImageStream *pSMaskStream = new ImageStream( pMaskStream, nMaskWidth, pMaskColorMap->GetComponentsCount(), pMaskColorMap->GetBitsPerComponent() );
			unsigned char *pAlpha = new unsigned char[ nMaskWidth * nMaskHeight ];

			if ( pMaskStream && pAlpha )
			{
				pSMaskStream->Reset();

				unsigned char unAlpha = 0;
				for( int nY = 0; nY < nMaskHeight; nY++ )
				{
					for ( int nX = 0; nX < nMaskWidth; nX++ )
					{
						int nIndex = ( nX + nY * nMaskWidth );
						pSMaskStream->GetPixel( &unAlpha );
						GrGray oGray;
						pMaskColorMap->GetGray( &unAlpha, &oGray );
						pAlpha[nIndex] = ColorToByte( oGray );
					}
				}
				delete pSMaskStream;

				double dScaleWidth  = (double)nMaskWidth  / (double)nWidth;
				double dScaleHeight = (double)nMaskHeight / (double)nHeight;

				for( int nY = nHeight - 1; nY >= 0; nY-- )
				{
					for ( int nX = 0; nX < nWidth; nX++ )
					{
						int nIndex = 4 * ( nY * nWidth + nX );

						int nNearestMatch =  ( ((int)( (nHeight - 1 - nY ) * dScaleHeight ) * nMaskWidth ) + ((int)( nX * dScaleWidth ) ) );

						pBufferPtr[nIndex + 3] = (unsigned char)(pAlpha[nNearestMatch] * dAlphaKoef);
					}
				}

				delete pAlpha;
			}
			else
			{
				if ( pAlpha )
					delete pAlpha;

				if ( pMaskStream )
					delete pMaskStream;

				bResize = false;
			}
		}
		else 
			bResize = false;

		if ( !bResize )
		{
			for( int nY = nHeight - 1; nY >= 0; nY-- )
			{
				for ( int nX = 0; nX < nWidth; nX++ )
				{
					int nIndex = 4 * ( nY * nWidth + nX );
					pBufferPtr[nIndex + 3] = (unsigned char)(255.0 * dAlphaKoef);
				}
			}
		}
	}
	else
	{
		ImageStream *pSMaskStream = new ImageStream( pMaskStream, nMaskWidth, pMaskColorMap->GetComponentsCount(), pMaskColorMap->GetBitsPerComponent() );
		pSMaskStream->Reset();

		unsigned char unAlpha = 0;
		for( int nY = nHeight - 1; nY >= 0; nY-- )
		{
			for ( int nX = 0; nX < nWidth; nX++ )
			{
				int nIndex = 4 * ( nX + nY * nWidth );
				pSMaskStream->GetPixel( &unAlpha );
				GrGray oGray;
				pMaskColorMap->GetGray( &unAlpha, &oGray );
				pBufferPtr[nIndex + 3] = ColorToByte( oGray ) * dAlphaKoef;
			}
		}
		delete pSMaskStream;
	}

	double arrMatrix[6];
	double *pCTM = pGState->GetCTM();
	//  Исходное предобразование
	//              |1  0  0|   |pCTM[0] pCTM[1] 0| 
	// arrMattrix = |0 -1  0| * |pCTM[2] pCTM[3] 0|
	//              |0  1  1|   |pCTM[4] pCTM[5] 1|
	arrMatrix[0] =     pCTM[0];
	arrMatrix[1] =  -  pCTM[1];
	arrMatrix[2] =    -pCTM[2];
	arrMatrix[3] =  -(-pCTM[3]);
	arrMatrix[4] =     pCTM[2] + pCTM[4];
	arrMatrix[5] =  -( pCTM[3] + pCTM[5] ) + dPageHeight;

	double dShiftX = 0, dShiftY = 0; 
	DoTransform( arrMatrix, &dShiftX, &dShiftY, TRUE );
	m_pRenderer->DrawImage( pInterface, 0 + dShiftX, 0 + dShiftY, PDFCoordsToMM( 1 ), PDFCoordsToMM( 1 ) );

	RELEASEINTERFACE( pInterface );
}

void RendererOutputDev::BeginTransparencyGroup(GrState *pGState, double *pBBox, GrColorSpace *pBlendingColorSpace, BOOL bIsolated, BOOL bKnockout, BOOL bForSoftMask)
{
	m_bTransparentGroup = TRUE;
	m_bTransparentGroupSoftMask = bForSoftMask;
}
void RendererOutputDev::EndTransparencyGroup(GrState *pGState)
{
	m_bTransparentGroup = FALSE;
	m_bTransparentGroupSoftMask = FALSE;

	if ( m_pTransparentGroupSoftMask )
		delete []m_pTransparentGroupSoftMask;
	
	m_pTransparentGroupSoftMask = NULL;
}
void RendererOutputDev::PaintTransparencyGroup(GrState *pGState, double *pBBox)
{
}
void RendererOutputDev::SetSoftMask(GrState *pGState, double *pBBox, BOOL bAlpha, Function *pTransferFunc, GrColor *pBackdropColor)
{
}
void RendererOutputDev::ClearSoftMask(GrState *pGState)
{
}

void RendererOutputDev::NewPDF(XRef *pXref)
{
	m_pXref = pXref;
}

void RendererOutputDev::Transform(double *pMatrix, double dUserX, double dUserY, double *pdDeviceX, double *pdDeviceY) 
{
	*pdDeviceX = dUserX * pMatrix[0] + dUserY * pMatrix[2] + pMatrix[4];
	*pdDeviceY = dUserX * pMatrix[1] + dUserY * pMatrix[3] + pMatrix[5];
}

void RendererOutputDev::DoPath(GrState *pGState, GrPath *pPath, double dPageHeight, double *pCTM)
{
	if ( m_bTransparentGroup )
		return;

	double arrMatrix[6];
	//double *pCTM = pGState->GetCTM();
	arrMatrix[0] =  pCTM[0];
	arrMatrix[1] = -pCTM[1];
	arrMatrix[2] =  pCTM[2];
	arrMatrix[3] = -pCTM[3];
	arrMatrix[4] =  pCTM[4];
	arrMatrix[5] = -pCTM[5] + dPageHeight;

	double dShiftX = 0, dShiftY = 0;
	DoTransform( arrMatrix, &dShiftX, &dShiftY );

	m_pRenderer->BeginCommand( c_nPathType );
	m_pRenderer->PathCommandEnd();

	int nSubPathCount = pPath->GetSubpathsCount();

	for ( int nSubPathIndex = 0; nSubPathIndex < nSubPathCount; ++nSubPathIndex ) 
	{
		GrSubpath *pSubpath = pPath->GetSubpath( nSubPathIndex );
		int nPointsCount = pSubpath->GetPointsCount();

		m_pRenderer->PathCommandMoveTo( PDFCoordsToMM( pSubpath->GetX(0) + dShiftX ), PDFCoordsToMM( pSubpath->GetY(0) + dShiftY ) );

		int nCurPointIndex = 1;
		while ( nCurPointIndex < nPointsCount ) 
		{
			if ( pSubpath->GetCurve(nCurPointIndex) ) 
			{
				m_pRenderer->PathCommandCurveTo( PDFCoordsToMM( pSubpath->GetX( nCurPointIndex ) + dShiftX ), PDFCoordsToMM( pSubpath->GetY( nCurPointIndex ) + dShiftY ), PDFCoordsToMM( pSubpath->GetX( nCurPointIndex + 1 ) + dShiftX ), PDFCoordsToMM( pSubpath->GetY( nCurPointIndex + 1 ) + dShiftY ), PDFCoordsToMM( pSubpath->GetX( nCurPointIndex + 2 ) + dShiftX ), PDFCoordsToMM( pSubpath->GetY( nCurPointIndex + 2 ) + dShiftY ) );
				nCurPointIndex += 3;
			} 
			else 
			{
				m_pRenderer->PathCommandLineTo( PDFCoordsToMM( pSubpath->GetX( nCurPointIndex ) + dShiftX ), PDFCoordsToMM( pSubpath->GetY( nCurPointIndex ) + dShiftY ) );
				++nCurPointIndex;
			}
		}
		if ( pSubpath->IsClosed() ) 
		{
			m_pRenderer->PathCommandClose();
		}
	}
}

void RendererOutputDev::UpdateClip(GrState *pGState)
{
	if ( m_bTransparentGroup )
		return;

	if ( m_bTiling )
		return;

	UpdateClipAttack(pGState);
}

void RendererOutputDev::UpdateClipAttack(GrState *pGState)
{
	GrClip *pClip = pGState->GetClip();

	int nPathIndex = -1;
	//if ( m_pClip && m_pClip->IsEqual( pClip, nPathIndex ) )
	//{
	//	return;
	//}

	if ( -1 == nPathIndex )
	{
		m_pRenderer->BeginCommand( c_nResetClipType );
		m_pRenderer->EndCommand( c_nResetClipType );

		for ( int nIndex = 0; nIndex < pClip->GetPathsCount(); nIndex++ )
		{
			GrPath *pPath   = pClip->GetPath( nIndex );
			int     nFlag   = pClip->GetFlag( nIndex );
			double *pMatrix = pClip->GetMatrix( nIndex );

			int     nClipFlag = GrClipEOFlag == nFlag ? c_nClipRegionTypeEvenOdd : c_nClipRegionTypeWinding;
			nClipFlag |= c_nClipRegionIntersect;

			m_pRenderer->BeginCommand( c_nClipType );
			m_pRenderer->put_ClipMode( nClipFlag );
			DoPath( pGState, pPath, pGState->GetPageHeight(), pMatrix );
			m_pRenderer->EndCommand( c_nPathType );
			m_pRenderer->EndCommand( c_nClipType );
		}

		for ( int nIndex = 0; nIndex < pClip->GetTextsCount(); nIndex++ )
		{
			BSTR bsFontName, bsFontPath;
			LONG lFontStyle;
			double dFontSize = 10, dX = 0, dY = 0, dWidth = 0, dHeight = 0, dBaseLineOffset = 0;
			BSTR bsText = pClip->GetText( nIndex, &dX, &dY, &dWidth, &dHeight, &dBaseLineOffset, &bsFontName, &bsFontPath, &dFontSize, &lFontStyle );
			int     nFlag   = pClip->GetFlag( nIndex );

			m_pRenderer->put_FontName( bsFontName );
			m_pRenderer->put_FontPath( bsFontPath );
			m_pRenderer->put_FontSize( dFontSize );
			m_pRenderer->put_FontStyle( lFontStyle );

			double dShiftX = 0, dShiftY = 0;
			DoTransform( pClip->GetTextMatrix( nIndex ), &dShiftX, &dShiftY, TRUE );

			m_pRenderer->BeginCommand( c_nClipType );
			m_pRenderer->put_ClipMode( c_nClipRegionTypeWinding | c_nClipRegionUnion );
			m_pRenderer->PathCommandEnd();
			m_pRenderer->put_FontStringGID( TRUE );
			m_pRenderer->PathCommandTextEx( NULL, bsText, NULL, PDFCoordsToMM( dX ), PDFCoordsToMM( dY ), PDFCoordsToMM( dWidth ), PDFCoordsToMM( dHeight ), PDFCoordsToMM( dBaseLineOffset ), 0 );
			m_pRenderer->EndCommand( c_nClipType );
		}
	}
	else
	{
		for ( int nIndex = nPathIndex; nIndex < pClip->GetPathsCount(); nIndex++ )
		{
			GrPath *pPath   = pClip->GetPath( nIndex );
			int     nFlag   = pClip->GetFlag( nIndex );
			double *pMatrix = pClip->GetMatrix( nIndex );

			int     nClipFlag = GrClipEOFlag == nFlag ? c_nClipRegionTypeEvenOdd : c_nClipRegionTypeWinding;
			nClipFlag |= c_nClipRegionIntersect;

			m_pRenderer->BeginCommand( c_nClipType );
			m_pRenderer->put_ClipMode( nClipFlag );
			DoPath( pGState, pPath, pGState->GetPageHeight(), pMatrix );
			m_pRenderer->EndCommand( c_nPathType );
			m_pRenderer->EndCommand( c_nClipType );

		}
	}

	if ( m_pClip )
		delete m_pClip;

	m_pClip = pClip->Copy();

	UpdateFont( pGState );
}


void RendererOutputDev::DoTransform(double *pMatrix, double *pdShiftX, double *pdShiftY, BOOL bText)
{
	if ( 1 == pMatrix[0] && 0 == pMatrix[1] && 0 == pMatrix[2] && 1 == pMatrix[3] && !bText )
	{
		if ( 0 == pMatrix[4] && 0 == pMatrix[5] )
		{
			m_pRenderer->ResetTransform();
			m_arrMatrix[0] = 1; m_arrMatrix[1] = 0;
			m_arrMatrix[2] = 0; m_arrMatrix[3] = 1;
			m_arrMatrix[4] = 0; m_arrMatrix[5] = 0;
		}
		else
		{
			*pdShiftX = pMatrix[4];
			*pdShiftY = pMatrix[5];
			m_pRenderer->ResetTransform();
			m_arrMatrix[0] = 1;          m_arrMatrix[1] = 0;
			m_arrMatrix[2] = 0;          m_arrMatrix[3] = 1;
			m_arrMatrix[4] = 0;          m_arrMatrix[5] = 0;
		}
	}
	else if ( m_arrMatrix[0] == pMatrix[0] && m_arrMatrix[1] == pMatrix[1] && m_arrMatrix[2] == pMatrix[2] && m_arrMatrix[3] == pMatrix[3] && !bText )
	{
		double dNewX = pMatrix[4], dNewY = pMatrix[5];
		double dIDet = 1 / ( pMatrix[0] * pMatrix[3] - pMatrix[1] * pMatrix[2] );

		*pdShiftX = ((dNewX - m_arrMatrix[4]) * m_arrMatrix[3] - (dNewY - m_arrMatrix[5]) * m_arrMatrix[1]) * dIDet;
		*pdShiftY = ((dNewY - m_arrMatrix[5]) * m_arrMatrix[0] - (dNewX - m_arrMatrix[4]) * m_arrMatrix[2]) * dIDet;
	}
	else
	{
		m_pRenderer->SetTransform( pMatrix[0], pMatrix[1], pMatrix[2], pMatrix[3], PDFCoordsToMM( pMatrix[4] ), PDFCoordsToMM( pMatrix[5] ) );
		m_arrMatrix[0] = pMatrix[0]; m_arrMatrix[1] = pMatrix[1];
		m_arrMatrix[2] = pMatrix[2]; m_arrMatrix[3] = pMatrix[3];
		m_arrMatrix[4] = pMatrix[4]; m_arrMatrix[5] = pMatrix[5];
	}
	return;
}

