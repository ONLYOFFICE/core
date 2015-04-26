#include "PdfWriterLib.h"

#include "../../../DesktopEditor/graphics/structures.h"
#include "../../../DesktopEditor/graphics/Image.h"

#include "../../../Common/FileDownloader.h""
#include "OnlineOfficeBinToPdf.h"

#include "../../../Common/Base64.h"

#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/File.h"

#include "../../../Common/DocxFormat/Source/Base/Types_32.h"

#include "../../../DesktopEditor/common/Directory.h"

CPdfWriterLib::CPdfWriterLib()
{
	m_pFontManager = NULL;

    m_bIsFontsInitialize = false;
	
	m_pFindFonts = NULL;

    //m_hSynchMutex = CreateMutex(NULL, false, _T("AVSPDFWriterMutex") );

	m_pDocument         = NULL;
	m_pCurrentFont      = NULL;
	m_pCurrentExtGState = NULL;
	m_pCurrentXObject   = NULL;
	m_pXObjectList      = NULL;
	m_pOutlineList      = NULL;
	m_pDestList           = NULL;
	m_pCurrentDestination = NULL;
	m_pAnnotList          = NULL;
	m_pCurrentAnnotation  = NULL;

	m_pFontEncodingList   = NULL;

    m_bStartSubPath = false;
    m_bGStateChange = false;

	m_oCommandParams.dAngle  =  0;
	m_oCommandParams.dHeight = -1;
	m_oCommandParams.dLeft   = -1;
	m_oCommandParams.dTop    = -1;
	m_oCommandParams.dWidth  = -1;
	m_oCommandParams.nFlag   =  0;
	m_oCommandParams.oMatrix.fA = 1.0f;
	m_oCommandParams.oMatrix.fB = 0.0f;
	m_oCommandParams.oMatrix.fC = 0.0f;
	m_oCommandParams.oMatrix.fD = 1.0f;
	m_oCommandParams.oMatrix.fE = 0.0f;
	m_oCommandParams.oMatrix.fF = 0.0f;

    m_oShadow.Visible = false;

	m_oFont.Name = CString( _T("Arial") );
	m_oFont.Size = 12.0;

	m_dCharSpace = 0.0;

	m_pCurCommand = NULL;

	m_pCurPath = NULL;

    m_bClipPath = false;

	m_wsTempDir = _T("");

    m_bIsWhiteBackImage     = false;
    m_bUseTextureRect       = false;
    m_bUseImageTransform    = true;
    m_bUseImageTextureAlpha = false;

	m_XForm					=	NULL;
    m_IsStrokeShape			=	false;
}

CPdfWriterLib::~CPdfWriterLib()
{
	FreePDF();

	RELEASEOBJECT(m_pFindFonts);
	m_pFontManager = NULL;//освободится ниже
	RELEASEOBJECT(m_pFontManager);
}

void CPdfWriterLib::SetFontDir(std::wstring& sFontDir)
{
	m_pFontManager = NULL;//освободится ниже

    m_oFontsApplication.InitializeFromFolder(sFontDir);
    m_pFontManager = m_oFontsApplication.GenerateFontManager();

    m_bIsFontsInitialize = true;
}

void CPdfWriterLib::SetThemesPlace(const std::wstring & sThemesPlace)
{
	m_sThemesPlace = sThemesPlace;
}
void CPdfWriterLib::InitializeFonts(bool bIsUseSystem, std::vector<std::wstring> arAdditionalFolders)
{
    m_pFontManager = NULL;//освободится ниже
    
    CArray<std::wstring> fonts;
    if (bIsUseSystem)
    {
        fonts = m_oFontsApplication.GetSetupFontFiles();
    }
    
    for (std::vector<std::wstring>::iterator iter = arAdditionalFolders.begin(); iter != arAdditionalFolders.end(); ++iter)
    {
        NSDirectory::GetFiles2(*iter, fonts);
    }
    
    m_oFontsApplication.InitializeFromArrayFiles(fonts);
    
    m_pFontManager = m_oFontsApplication.GenerateFontManager();
    
    m_bIsFontsInitialize = true;
}

HRESULT CPdfWriterLib::SetAdditionalParam (CString ParamName, VARIANT	ParamValue)
{
	CString sParamName; sParamName = ParamName;

	if ( _T("TempDirectory") == sParamName && VT_BSTR == ParamValue.vt )
	{
		m_wsTempDir = CString( ParamValue.bstrVal );
		DocSetTempDir( m_pDocument, (wchar_t*)m_wsTempDir.c_str() );
	}
	else if ( _T("WhiteBackImage") == sParamName && VT_BOOL == ParamValue.vt )
	{
        m_bIsWhiteBackImage = (ParamValue.boolVal == VARIANT_TRUE) ? true : false;
	}
	else if ( _T("BaseTransform") == sParamName && VT_ARRAY == ParamValue.vt )
	{
		//// check for valid pixel array size
		//if ( 1 != SafeArrayGetDim(*ParamValue.pparray) )
		//	return S_FALSE;

		//VARTYPE vType;
		//// check for valid array element type
        //if ( S_OK != (SafeArrayGetVartype(*ParamValue.pparray, &vType)) || VT_R8 != vType )
		//	return S_FALSE;

		//double* pTemp = (double* (*ParamValue.pparray)->pvData;
		//m_oBaseTransform.fA = pTemp[0]; m_oBaseTransform.fB = pTemp[1];
		//m_oBaseTransform.fC = pTemp[2]; m_oBaseTransform.fD = pTemp[3];
		//m_oBaseTransform.fE = pTemp[4]; m_oBaseTransform.fF = pTemp[5];
	}
	else if ( _T("BrushFillBoundsEnable") == sParamName && VT_BOOL == ParamValue.vt )
	{
        m_bUseTextureRect = (ParamValue.boolVal == VARIANT_TRUE) ? true : false;
	}
	// NOTE: пока градиенты выставим в виде svg-xml
	// TODO: потом убрать
	else if (_T("Fill-LinearGradient") == sParamName && VT_BSTR == ParamValue.vt)
	{
        if (!CreateLinearGradientFromSvgXml(std::wstring(ParamValue.bstrVal), true))
			return S_FALSE;
	}
	// NOTE: пока градиенты выставим в виде svg-xml
	// TODO: потом убрать
	else if (_T("Stroke-LinearGradient") == sParamName && VT_BSTR == ParamValue.vt)
	{
        if (!CreateLinearGradientFromSvgXml(std::wstring(ParamValue.bstrVal), false))
			return S_FALSE;
	}
	// NOTE: пока градиенты выставим в виде svg-xml
	// TODO: потом убрать
	else if (_T("Fill-RadialGradient") == sParamName && VT_BSTR == ParamValue.vt)
	{
        if (!CreateRadialGradientFromSvgXml(std::wstring(ParamValue.bstrVal), true))
			return S_FALSE;
	}
	// NOTE: пока градиенты выставим в виде svg-xml
	// TODO: потом убрать
	else if (_T("Stroke-RadialGradient") == sParamName && VT_BSTR == ParamValue.vt)
	{
        if (!CreateRadialGradientFromSvgXml(std::wstring(ParamValue.bstrVal), false))
			return S_FALSE;
	}


	return S_OK;
}
HRESULT CPdfWriterLib::GetAdditionalParam (CString ParamName, VARIANT *	ParamValue)
{
	return S_OK;
}
HRESULT CPdfWriterLib::CreatePDF ()
{
	if ( m_pFontManager && !m_bIsFontsInitialize )
	{
#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
        m_pFontManager->Init( L"", true, true );
#else
		m_pFontManager->Initialize();
#endif
        m_bIsFontsInitialize = true;
	}
	m_pDocument = New();
	if ( !m_pDocument )
		return AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM;

	// m_pDocument->patterns = &m_oPatterns;

	// Создаем стек команд
	m_pCurCommand = new TCommandType();
	m_pCurCommand->nType = c_nNone;
	m_pCurCommand->nFlag = 0;
	m_pCurCommand->pPrev = NULL;

	m_pFindFonts = new CFindFonts();

    m_bClipPath = false;
	m_wsTempDir = _T("");

	m_oTransform.Reset();
	m_oRendererState.Reset();
    m_bFirstMoveTo = true;

	return S_OK;
}
HRESULT CPdfWriterLib::DeletePDF ()
{
	FreePDF();
	return S_OK;
}
HRESULT CPdfWriterLib::SaveToFile (CString bsPath)
{
	SetState( rendstateNone );

	CString sFileName( bsPath );

	// Проверяем не пустой ли документ мы пытаемся записать.
	if ( m_pDocument->pPageList->nCount <= 0 )
		return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

    std::wstring wsFileName = string2std_string(sFileName);
    unsigned long nRet = SaveDocToFile( m_pDocument, wsFileName);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::SetPDFPageMode (long nMode)
{
	unsigned long nRet = SetPageMode( m_pDocument, (EPageMode)nMode);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetPDFPageMode (long *pnMode)
{
	*pnMode = (long)GetPageMode( m_pDocument );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetPDFPageLayout (long nLayout)
{
	unsigned long nRet = SetPageLayout( m_pDocument, (EPageLayout)nLayout);
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetPDFPageLayout (long *pnLayout)
{
	*pnLayout = (long)GetPageLayout( m_pDocument );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetPDFViewerPreference (long nValue )
{
	unsigned long nRet = SetViewerPreference( m_pDocument, nValue );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetPDFViewerPreference (long *pnValue)
{
	*pnValue = (long)GetViewerPreference( m_pDocument );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetPDFCompressionMode (long nMode)
{
	unsigned long nRet = SetCompressionMode( m_pDocument, nMode );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetPDFOpenAction (long nAction)
{
	if ( !m_pCurrentDestination )
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_DESTINATION_NOT_FOUND;
	unsigned long nRet = SetOpenAction( m_pDocument, m_pCurrentDestination );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetLastErrorCode (long *pnErrorCode)
{
	*pnErrorCode = ErrorGetCode( &m_pDocument->oError );

	return S_OK;
}
HRESULT CPdfWriterLib::GetLastErrorString (CString *pbsErrorString)
{
	const char *sErrorString = ErrorGetString( &m_pDocument->oError );
	CString sErr(sErrorString);

    *pbsErrorString = sErr;
	return S_OK;
}
//-----------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::AddNewPage ()
{		
	if ( !m_pDocument )
		return AVS_OFFICEPDFWRITER_ERROR_INVALID_DOCUMENT;

	// новая страница становится текущей
	Page page = AddPage( m_pDocument );
	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

HRESULT CPdfWriterLib::GetPagesCount (long *pnCount )
{
	*pnCount = m_pDocument->pPageList->nCount;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;		
}
HRESULT CPdfWriterLib::SetCurrentPage (long nIndex)
{
	if ( nIndex > m_pDocument->pPageList->nCount )
		return S_FALSE;

	Page pTempPage = (Page)ListItemAt( m_pDocument->pPageList, nIndex );
	DocSetCurrentPage( m_pDocument, pTempPage );

	return S_OK;
}

HRESULT CPdfWriterLib::GetCurrentPageIndex (long *pnIndex)
{
	*pnIndex = ListFind( m_pDocument->pPageList, m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetPageWidth (float fWidth)
{
	unsigned long nRet = PageSetWidth( m_pDocument->pCurPage, fWidth );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetPageHeight (float fHeight)
{
	unsigned long nRet = PageSetHeight( m_pDocument->pCurPage, fHeight );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetWidth (float *pfWidth)
{
	*pfWidth = PageGetWidth( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetHeight (float *pfHeight)
{
	*pfHeight = PageGetHeight( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetSize (long nSize, long nDirection)
{
	unsigned long nRet = PageSetSize(m_pDocument->pCurPage, (EPageSizes)nSize, (EPageDirection)nDirection);
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetRotate (long nAngle)
{
	unsigned long nRet = PageSetRotate( m_pDocument->pCurPage, nAngle );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::AddLabel (long nPageNum, long nNumStyle, long nFirstPage, CString bsPrefix )
{
	const char* sPrefix = NULL;
#if defined(_WIN32) || defined (_WIN64)
    USES_CONVERSION;
    sPrefix = W2A(bsPrefix);
#else
    std::string ansiStr(bsPrefix.begin(), bsPrefix.end());
    sPrefix = ansiStr.c_str();
#endif
	unsigned long nRet = AddPageLabel( m_pDocument, nPageNum, (PageNumStyle)nNumStyle, nFirstPage, sPrefix );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}



//-----------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::SetLineWidth (float fLineWidth)
{
	unsigned long nRet = PageSetLineWidth( m_pDocument->pCurPage, fLineWidth );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetLineWidth (float *pfLineWidth)
{
	*pfLineWidth = PageGetLineWidth( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetLineCap (long nLineCap)
{
	unsigned long nRet = PageSetLineCap( m_pDocument->pCurPage, (ELineCapStyle)nLineCap);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetLineCap (long *pnLineCap)
{
	*pnLineCap = PageGetLineCap( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetLineJoin (long nLineJoin)
{
	unsigned long nRet = PageSetLineJoin( m_pDocument->pCurPage, (LineJoinStyle)nLineJoin);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetLineJoin (long *pnLineJoin)
{
	*pnLineJoin = PageGetLineJoin( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetMiterLimit (float fMiterLimit)
{
	unsigned long nRet = PageSetMiterLimit( m_pDocument->pCurPage, fMiterLimit);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetMiterLimit (float *pfMiterLimit)
{
	*pfMiterLimit = PageGetMiterLimit( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
//HRESULT CPdfWriterLib::SetDash (SAFEARRAY **ppnDashPtn, long   nNumParam, float   fPhase)
//{
//	double *pArray = new double[nNumParam];
//	if ( 0 != nNumParam )
//	{
//		// check for valid pixel array size
//		if ( 1 != SafeArrayGetDim(*ppnDashPtn) )
//			return false;

//		VARTYPE vType;
//		// check for valid array element type
//		if ( S_OK != (SafeArrayGetVartype(*ppnDashPtn, &vType)) || VT_R4 != vType )
//			return S_OK;

//		float* pTemp = (float* (*ppnDashPtn)->pvData;
//		for ( int nIndex = 0 ; nIndex < nNumParam; nIndex++ )
//		{
//			pArray[nIndex] = *(pTemp + nIndex);
//		}
//	}

//	unsigned long nRet = PageSetDash( m_pDocument->pCurPage, pArray, nNumParam, fPhase);
//	delete[] pArray;
//	if ( OK != nRet )
//		return nRet;
//	return S_OK;
//}

//HRESULT CPdfWriterLib::GetDash (SAFEARRAY **ppnDashPtn, long *pnNumParam, float *pfPhase)
//{
//	DashMode oMode = {{0, 0, 0, 0, 0, 0, 0, 0}, 0, 0};		
//	oMode = PageGetDash( m_pDocument->pCurPage );

//	// проверяем не появилась ли ошибка
//	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
//	if ( OK != nRet )
//		return nRet;

//	if ( 0 == oMode.nNumPoints )
//	{
//		*ppnDashPtn = NULL;
//		*pnNumParam = 0;
//		*pfPhase = 0;
//		return S_OK;
//	}
//	float* pTemp = new float[oMode.nNumPoints];
//	for ( int nIndex = 0; nIndex < oMode.nNumPoints; nIndex++ )
//		pTemp[nIndex] = (float)oMode.afPtn[nIndex];

//	SAFEARRAYBOUND bounds[1];

//	bounds[0].lLbound = 0;
//	bounds[0].cElements = oMode.nNumPoints;

//	(*ppnDashPtn) = SafeArrayCreate(VT_R4, 1, bounds);

//	if ( !(*ppnDashPtn) )
//	{
//		delete[] pTemp;
//		return S_OK;
//	}

//	memcpy( (*ppnDashPtn)->pvData, pTemp, oMode.nNumPoints * sizeof(float) );
//	delete[] pTemp;
//	*pnNumParam = (long)oMode.nNumPoints;
//	*pfPhase    = (float)oMode.fPhase;
//	return S_OK;
//}
HRESULT CPdfWriterLib::SetFlat (float   fFlatness)
{
	unsigned long nRet = PageSetFlat( m_pDocument->pCurPage, fFlatness );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetFlat (float *pfFlatness)
{
	*pfFlatness = PageGetFlat( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::NewExtGraphicsState ()
{
	m_pCurrentExtGState = CreateExtGState( m_pDocument );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::ExtGraphicsStateSetAlphaStroke (float fValue)
{
	unsigned long nRet = ExtGStateSetAlphaStroke( m_pCurrentExtGState, fValue );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::ExtGraphicsStateSetAlphaFill (float fValue)
{
	unsigned long nRet = ExtGStateSetAlphaFill( m_pCurrentExtGState, fValue );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::ExtGraphicsStateSetBlendMode (long nBlendMode)
{
	unsigned long nRet = ExtGStateSetBlendMode( m_pCurrentExtGState, (EBlendMode)nBlendMode );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::ExtGraphicsStateSetStrokeAdjustment (int nFlag)
{
    bool bFlag = ( 0 == nFlag ? false : true);

	unsigned long nRet = ExtGStateSetStrokeAdjustment( m_pCurrentExtGState, bFlag );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::SetExtGraphicsState ()
{
	if ( !m_pCurrentExtGState )
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_EXTGSTATE_NOT_FOUND;

	unsigned long nRet = PageSetExtGState( m_pDocument->pCurPage, m_pCurrentExtGState );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GSave ()
{
	unsigned long nRet = PageGSave( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GRestore ()
{
    bool bIsPrevGState = false;

	if ( m_pDocument->pCurPage && m_pDocument->pCurPage )
	{
		PageAttr pAttr = (PageAttr)m_pDocument->pCurPage->pAttr;
		if ( pAttr->pGState->pPrev )
            bIsPrevGState = true;
	}
	if ( bIsPrevGState )
	{
		unsigned long nRet = PageGRestore( m_pDocument->pCurPage );
		if ( OK != nRet )
			return nRet;
	}
	return S_OK;
}
HRESULT CPdfWriterLib::GetGStateDepth (long *pnDepth)
{
	*pnDepth = PageGetGStateDepth( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetGMode (long *pnGMode)
{
	*pnGMode = PageGetGMode( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::Concat (float fA, float fB, float fC, float fD, float fX, float fY)
{
	unsigned long nRet = PageConcat( m_pDocument->pCurPage, fA, fB, fC, fD, fX, fY);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::MoveTo (float fX, float fY)
{
	unsigned long nRet = PageMoveTo( m_pDocument->pCurPage, fX, fY);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetCurrentPosition (float *pfX, float *pfY)
{
	TPoint oCurPos = {0,0};

	unsigned long nRet = PageGetCurrentPos2( m_pDocument->pCurPage, &oCurPos );
	if ( OK != nRet )
	{
		*pfX = 0.0f;
		*pfY = 0.0f;
		return nRet;
	}

	*pfX = oCurPos.fX;
	*pfY = oCurPos.fY;
	return S_OK;
}
HRESULT CPdfWriterLib::LineTo (float fX, float fY)
{
	unsigned long nRet = PageLineTo( m_pDocument->pCurPage, fX, fY);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::CurveTo (float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
{
	unsigned long nRet = PageCurveTo( m_pDocument->pCurPage, fX1, fY1, fX2, fY2, fX3, fY3);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::CurveTo2 (float fX2, float fY2, float fX3, float fY3)
{
	unsigned long nRet = PageCurveTo2( m_pDocument->pCurPage, fX2, fY2, fX3, fY3);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::CurveTo3 (float fX1, float fY1, float fX3, float fY3)
{
	unsigned long nRet = PageCurveTo3( m_pDocument->pCurPage, fX1, fY1, fX3, fY3);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::ClosePath ()
{
	unsigned long nRet = PageClosePath( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::AppendRectangle (float fX, float fY, float fWidth, float fHeight)
{
	unsigned long nRet = PageRectangle( m_pDocument->pCurPage, fX, fY, fWidth, fHeight );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}

HRESULT CPdfWriterLib::Stroke ()
{
	unsigned long nRet = PageStroke( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetStrokingColorSpace (long *pnColorSpace)
{
	*pnColorSpace = (long)PageGetStrokingColorSpace( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::ClosePathStroke ()
{
	unsigned long nRet = PageClosePathStroke( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::Fill ()
{
	unsigned long nRet = PageFill( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetFillingColorSpace (long *pnColorSpace)
{
	*pnColorSpace = (long)PageGetFillingColorSpace( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::EoFill ()
{
	unsigned long nRet = PageEoFill( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::FillStroke ()
{
	unsigned long nRet = PageFillStroke( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::EoFillStroke ()
{
	unsigned long nRet = PageEoFillStroke( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::ClosePathFillStroke ()
{
	unsigned long nRet = PageClosePathFillStroke( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::ClosePathEoFillStroke ()
{
	unsigned long nRet = PageClosePathEoFillStroke( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::EndPath ()
{
	unsigned long nRet = PageEndPath( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}

HRESULT CPdfWriterLib::Clip ()
{
	unsigned long nRet = PageClip( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::EoClip ()
{
	unsigned long nRet = PageEoclip( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}

HRESULT CPdfWriterLib::BeginText ()
{
	unsigned long nRet = PageBeginText( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::EndText ()
{
	unsigned long nRet = PageEndText( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}

HRESULT CPdfWriterLib::SetCharSpace (float fValue)
{
	unsigned long nRet = PageSetCharSpace( m_pDocument->pCurPage, fValue );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetCharSpace (float *pfValue)
{
	*pfValue = PageGetCharSpace( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetWordSpace (float fValue)
{
	unsigned long nRet = PageSetWordSpace( m_pDocument->pCurPage, fValue );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetWordSpace (float *pfValue)
{
	*pfValue = PageGetWordSpace( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetHorizontalScalling (float fValue)
{
	unsigned long nRet = PageSetHorizontalScalling( m_pDocument->pCurPage, fValue  );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetHorizontalScalling (float *pfValue)
{
	*pfValue = PageGetHorizontalScalling( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetTextLeading (float fValue)
{
	unsigned long nRet = PageSetTextLeading( m_pDocument->pCurPage, fValue  );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetTextLeading (float *pfValue)
{
	*pfValue = PageGetTextLeading( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetFontAndSizeToGState (float fSize)
{
	unsigned long nRet = PageSetFontAndSize( m_pDocument->pCurPage, m_pCurrentFont, fSize);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetCurrentFontFromGState (CString *pbsFontName, CString *pbsEncodingName)
{
	FontDict pFont = PageGetCurrentFont( m_pDocument->pCurPage );
	if ( !pFont )
	{
		*pbsFontName = _T("");
		*pbsEncodingName = _T("");
	}
	else
	{
		CString sFontName(FontGetFontName( pFont ));
		CString sEncodingName(((FontAttr)pFont->pAttr)->pEncoder->sName);

		*pbsFontName     = sFontName;
		*pbsEncodingName = sEncodingName;
	}

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetCurrentFontSizeFromGState (float *pfSize)
{
	*pfSize = PageGetCurrentFontSize( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetTextRenderingMode (long nMode)
{
	unsigned long nRet = PageSetTextRenderingMode( m_pDocument->pCurPage, (ETextRenderingMode)nMode );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetTextRenderingMode (long *pnMode)
{
	*pnMode = (long)PageGetTextRenderingMode( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetTextRise (float fValue)
{
	unsigned long nRet = PageSetTextRise( m_pDocument->pCurPage, fValue );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetTextRise (float *pfValue)
{
	*pfValue = PageGetTextRise( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::MoveTextPos (float fX, float fY)
{
	unsigned long nRet = PageMoveTextPos( m_pDocument->pCurPage, fX, fY );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::MoveTextPos2 (float fX, float fY)
{
	unsigned long nRet = PageMoveTextPos2( m_pDocument->pCurPage, fX, fY );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetCurrentTextPosition (float *pfX, float *pfY)
{
	TPoint oCurPos = {0,0};

	oCurPos = PageGetCurrentTextPos( m_pDocument->pCurPage);

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
	{
		*pfX = 0.0f;
		*pfY = 0.0f;
		return nRet;
	}

	*pfX = oCurPos.fX;
	*pfY = oCurPos.fY;
	return S_OK;
}
HRESULT CPdfWriterLib::SetTextMatrix (float   fA, float   fB, float   fC, float   fD, float   fX, float   fY)
{
	unsigned long nRet = PageSetTextMatrix( m_pDocument->pCurPage, fA, fB, fC, fD, fX, fY );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::GetTextMatrix (float *pfA, float *pfB, float *pfC, float *pfD, float *pfX, float *pfY)
{
	TransMatrix oMatrix = { 1, 0, 0, 1, 0, 0};

	oMatrix = PageGetTextMatrix( m_pDocument->pCurPage );
	*pfA = oMatrix.fA;
	*pfB = oMatrix.fB;
	*pfC = oMatrix.fC;
	*pfD = oMatrix.fD;
	*pfX = oMatrix.fX;
	*pfY = oMatrix.fY;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}	
HRESULT CPdfWriterLib::MoveToNextLine ()
{
	unsigned long nRet = PageMoveToNextLine( m_pDocument->pCurPage );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::ShowText (CString bsText)
{
	
	const char* sText = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sText = W2A( bsText  );
#else
    std::string ansiStr(bsText.begin(), bsText.end());
    sText = ansiStr.c_str();
#endif
	// TO DO: ShowText: надо нормально конвертировать текст, иизмерять его длину
	unsigned long nRet = PageShowText( m_pDocument->pCurPage, (BYTE *)sText, strlen(sText), NULL, 0 );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::ShowTextNextLine (CString bsText)
{
	const char* sText = NULL;
	
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sText = W2A(bsText);
#else
    std::string ansiStr(bsText.begin(), bsText.end());
    sText = ansiStr.c_str();
#endif
	// TO DO: ShowText: надо нормально конвертировать текст, иизмерять его длину
	unsigned long nRet = PageShowTextNextLine( m_pDocument->pCurPage, (BYTE *)sText, strlen(sText), NULL, 0 );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::ShowTextNextLineEx (float fWordSpace, float fCharSpace, CString bsText)
{
	const char* sText = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sText = W2A(bsText);
#else
    std::string ansiStr(bsText.begin(), bsText.end());
    sText = ansiStr.c_str();
#endif
	// TO DO: ShowText: надо нормально конвертировать текст, иизмерять его длину
	unsigned long nRet = PageShowTextNextLineEx( m_pDocument->pCurPage, fWordSpace, fCharSpace, (BYTE *)sText, strlen(sText), NULL, 0 );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::SetGrayStroke (float fGray)
{
	unsigned long nRet = PageSetGrayStroke( m_pDocument->pCurPage, fGray );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::GetGrayStroke (float *pfGray)
{
	*pfGray = PageGetGrayStroke( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}	
HRESULT CPdfWriterLib::SetGrayFill (float fGray)
{
	unsigned long nRet = PageSetGrayFill( m_pDocument->pCurPage, fGray );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::GetGrayFill (float *pfGray)
{
	*pfGray = PageGetGrayFill( m_pDocument->pCurPage );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetRGBStroke (float   fR, float   fG, float   fB)
{
	unsigned long nRet = PageSetRGBStroke( m_pDocument->pCurPage, fR, fG, fB);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::GetRGBStroke (float *pfR, float *pfG, float *pfB)
{
	RGBColor oRGB = { 0, 0, 0};
	oRGB = PageGetRGBStroke( m_pDocument->pCurPage );
	*pfR = oRGB.r;
	*pfG = oRGB.g;
	*pfB = oRGB.b;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}	
HRESULT CPdfWriterLib::SetRGBFill (float fR, float fG, float fB)
{
	//unsigned long nRet = PageSetShadingFill( m_pDocument->pCurPage, fR, fG, fB);
	//if ( OK != nRet )
	//	return nRet;
	//return S_OK;

	unsigned long nRet = PageSetRGBFill( m_pDocument->pCurPage, fR, fG, fB);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::GetRGBFill (float *pfR, float *pfG, float *pfB)
{
	RGBColor oColor = PageGetRGBFill( m_pDocument->pCurPage );
	*pfR = oColor.r;
	*pfG = oColor.g;
	*pfB = oColor.b;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetCMYKStroke (float   fC, float   fM, float   fY, float   fK)
{
	unsigned long nRet = PageSetCMYKStroke( m_pDocument->pCurPage, fC, fM, fY, fK);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::GetCMYKStroke (float *pfC, float *pfM, float *pfY, float *pfK)
{
	CMYKColor oCMYK = { 0, 0, 0, 0};
	oCMYK = PageGetCMYKStroke( m_pDocument->pCurPage );
	*pfC = oCMYK.c;
	*pfM = oCMYK.m;
	*pfY = oCMYK.y;
	*pfK = oCMYK.k;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}	
HRESULT CPdfWriterLib::SetCMYKFill (float   fC, float   fM, float   fY, float   fK)
{
	unsigned long nRet = PageSetCMYKFill( m_pDocument->pCurPage, fC, fM, fY, fK);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}	
HRESULT CPdfWriterLib::GetCMYKFill (float *pfC, float *pfM, float *pfY, float *pfK)
{
	CMYKColor oCMYK = { 0, 0, 0, 0};
	oCMYK = PageGetCMYKFill( m_pDocument->pCurPage );
	*pfC = oCMYK.c;
	*pfM = oCMYK.m;
	*pfY = oCMYK.y;
	*pfK = oCMYK.k;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::ExecuteXObject ()
{
	unsigned long nRet = PageExecuteXObject( m_pDocument->pCurPage, m_pCurrentXObject );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}

HRESULT CPdfWriterLib::SetExtGraphicsState2 (CString bsXmlOptions)
{
	CString wsXml( bsXmlOptions );

	XmlUtils::CXmlNode oMainNode;
	oMainNode.FromXmlString( wsXml );

	if ( _T("ExtGState") == oMainNode.GetName() )
	{
		float fAlphaStroke = -1, fAlphaFill = -1;
		BlendMode eMode = BMEOF;
		int nStrokeAdjustment = -1;

		CString sValue;
		XmlUtils::CXmlNode oNode;
		if ( oMainNode.GetNode( _T("AlphaStroke"), oNode ) )
		{
			sValue = oNode.GetAttribute( _T("value"), _T("-1") );
			fAlphaStroke = (float)XmlUtils::GetDouble( sValue );
		}
		if ( oMainNode.GetNode( _T("AlphaFill"), oNode ) )
		{
			sValue = oNode.GetAttribute( _T("value"), _T("-1") );
			fAlphaFill = (float)XmlUtils::GetDouble( sValue );
		}
		if ( oMainNode.GetNode( _T("BlendMode"), oNode ) )
		{
			sValue = oNode.GetAttribute( _T("value"), _T("12") );
			eMode = (BlendMode)XmlUtils::GetInteger( sValue );
		}
		if ( oMainNode.GetNode( _T("StrokeAdjustment"), oNode ) )
		{
			sValue = oNode.GetAttribute( _T("value"), _T("-1") );
			nStrokeAdjustment = XmlUtils::GetInteger( sValue );
		}

		m_pCurrentExtGState = GetExtGState( m_pDocument, fAlphaStroke, fAlphaFill, eMode, nStrokeAdjustment );
		return PageSetExtGState( m_pDocument->pCurPage, m_pCurrentExtGState );
	}


	return S_OK;
}
//-----------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::AppendCircle (float fX, float fY, float fRad)
{
	unsigned long nRet = PageCircle( m_pDocument->pCurPage, fX, fY, fRad );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::AppendEllipse (float fX, float fY, float fXRad, float fYRad)
{
	unsigned long nRet = PageEllipse( m_pDocument->pCurPage, fX, fY, fXRad, fYRad );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::AppendArc (float fX, float fY, float fRad, float fAngle1, float fAngle2)
{
	unsigned long nRet = PageArc( m_pDocument->pCurPage, fX, fY, fRad, fAngle1, fAngle2);
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::AppendEllipseArc (float fX, float fY, float fXRad, float fYRad, float fAngle1, float fAngle2, bool bClockDirection /*= false*/)
{
	unsigned long nRet = PageEllipseArc2( m_pDocument->pCurPage, fX, fY, fXRad, fYRad, fAngle1, fAngle2, bClockDirection );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::TextOut_ (float fXPos, float fYPos, std::wstring & bsText)
{	

	if ( !m_pCurrentFont )
		return S_OK;

	FontAttr pFontAttr = (FontAttr)m_pCurrentFont->pAttr;
	if ( FontDefCID != pFontAttr->pFontDef->eType )
	{
		// В данном случае все символы имеют значение от 0 до 255, т.е. на них нужен 1 байт

        char* sText = new char[bsText.length() + 1];
        sText[bsText.length()] = '\0';

        bool *pZeros = new bool[bsText.length()];
        bool bCodedString = true;
        for ( int nIndex = 0; nIndex < bsText.length(); nIndex++ )
		{
			if ( (unsigned int)bsText[nIndex] >= 256 )
			{
                bCodedString = false;
			}

			if ( 0xFFFE == (unsigned int)bsText[nIndex] )
			{
				pZeros[nIndex] = 1;
			}
			else
			{
				pZeros[nIndex] = 0;
			}
		}
		if ( bCodedString )
            wcstombs( sText, bsText.c_str(), bsText.length() );
		else
		{
			delete []sText; sText = NULL;

#if defined(_WIN32) || defined (_WIN64)
			USES_CONVERSION;
			sText = W2A( bsText.c_str() );
#else
            std::string ansiStr(bsText.begin(), bsText.end());
            sText = (char*)ansiStr.c_str();
#endif
		}
        BYTE *pString = new BYTE[bsText.length()];
        for ( int nIndex = 0; nIndex < bsText.length(); nIndex++ )
		{
			if ( 0 == pZeros[nIndex] )
				pString[nIndex] = sText[nIndex];
			else
				pString[nIndex] = '\0';
		}
		delete []pZeros;

        unsigned long nRet = PageTextOut( m_pDocument->pCurPage, fXPos, fYPos, pString, bsText.length(), NULL, 0 );
		delete []pString;

		if ( bCodedString )
			delete []sText;

		if ( OK != nRet )
			return nRet;

	}
	else
	{
		// В CID шрифтах количество байт на каждый символ зависит от самого символа
		// и от кодировки CMap, заданной в шрифте.

		EncoderRecPtr pEncoder = pFontAttr->pEncoder;
		if ( EncoderTypeDoubleByteStream != pEncoder->eType )
			return S_OK;

		CMapEncoderStreamAttr pEncoderAttr = (CMapEncoderStreamAttr)pEncoder->pAttr;
//todooo по нормальному в  анси

		const wchar_t* wsText = bsText.c_str();
		int nSrcLen = wcslen( wsText );
		int nDstLen = 0;

		// Для начала рассчитаем длину выходной строки
		for ( int nIndex = 0; nIndex < nSrcLen; nIndex++ )
		{
			int nCID = (int)wsText[nIndex];
			if ( 0xFFFE == nCID )
				nCID = 0;

			int nUsedLen = 0;
			char *sBuffer = new char[4];
			CMapEncoderStreamGetCodeByCID( pEncoder, nCID, (char **)&sBuffer, &nUsedLen );
			delete []sBuffer;
			nDstLen += nUsedLen;
		}

		BYTE *pText = new BYTE[nDstLen + 1];
		pText[nDstLen] = '\0';

		for ( int nIndex = 0, nIndex2 = 0; nIndex < nSrcLen; nIndex++ )
		{
			int nCID = (int)wsText[nIndex];
			if ( 0xFFFE == nCID )
				nCID = 0;

			int nUsedLen = 0;
			char *sBuffer = new char [4];
			CMapEncoderStreamGetCodeByCID( pEncoder, nCID, (char **)&sBuffer, &nUsedLen );

			for ( int nCurIndex = 0; nCurIndex < nUsedLen; nCurIndex++ )
			{
				pText[nIndex2 + nCurIndex] = sBuffer[4 - nUsedLen + nCurIndex];
			}
			delete []sBuffer;

			nIndex2 += nUsedLen;
		}

		unsigned int *pCIDs = new unsigned int[nSrcLen];
		for ( int nIndex = 0; nIndex < nSrcLen; nIndex++ )
		{
			pCIDs[nIndex] = (unsigned int)wsText[nIndex];
		}

		unsigned long nRet = PageTextOut( m_pDocument->pCurPage, fXPos, fYPos, pText, nDstLen, pCIDs, nSrcLen );

		delete []pCIDs;
		delete []pText;

		if ( OK != nRet )
			return nRet;

	}
	return S_OK;
}
HRESULT CPdfWriterLib::TextRect (float fLeft, float fTop, float fRight, float fBottom, CString bsText, long nAlign, unsigned int *pnLen)
{
	const char* sText = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sText = W2A(bsText);
#else
    std::string ansiStr(bsText.begin(), bsText.end());
    sText = ansiStr.c_str();
#endif		
	unsigned long nRet = PageTextRect( m_pDocument->pCurPage, fLeft, fTop, fRight, fBottom, sText, (ETextAlignment)nAlign, pnLen, NULL, 0 );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::SetSlideShow (long nType, float fDispTime, float fTransTime)
{
	unsigned long nRet = PageSetSlideShow( m_pDocument->pCurPage, (ETransitionStyle)nType, fDispTime, fTransTime );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::GetTextWidth (std::wstring bsText, float *pfResult)
{
	const char* sText = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sText = W2A(bsText.c_str());
#else
    std::string ansiStr(bsText.begin(), bsText.end());
    sText = ansiStr.c_str();
#endif
	*pfResult = PageTextWidth( m_pDocument->pCurPage, (BYTE *)sText, strlen(sText), NULL, 0 );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::MeasureText (CString bsText, float fWidth, bool bWordWrap, float *pfRealWidth, long *pnLength)
{
	const char* sText = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sText = W2A(bsText);
#else
    std::string ansiStr(bsText.begin(), bsText.end());
    sText = ansiStr.c_str();
#endif
    bool bWW = ( bWordWrap == 0 ? false : true);
	*pnLength = PageMeasureText( m_pDocument->pCurPage, sText, NULL, 0, fWidth, bWW, pfRealWidth );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

//-----------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::LoadT1FFromFile ( CString bsAFMFileName, CString bsDataFileName, CString *bsFontName)
{
    std::wstring wsAFMFileName = string2std_string(bsAFMFileName);

    std::wstring wsData = string2std_string(bsDataFileName);


	// Чтобы не изменять интерфейс, при перезаписи PDF, храним тип шрифта в его расширении
	short nType = 0;
    if ( wsData.length() > 0 )
	{
        CString wsExt;
        int res = bsDataFileName.ReverseFind(_T('.'));
        if (res>=0) wsExt = bsDataFileName.Mid(res);
        wsExt.MakeLower();

		if ( _T(".pfb_t1cot") == wsExt )
			nType = 2;
		else if ( _T(".pfb_t1c") == wsExt )
			nType = 1;
		else 
			nType = 0;
	}

    const char* sFontName = LoadType1FontFromFile( m_pDocument, wsAFMFileName, wsData, nType );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( AVS_OFFICEPDFWRITER_ERROR_FONT_EXISTS == nRet )
		ResetError( m_pDocument );
	else if ( OK != nRet )
	{
		(*bsFontName) = _T("");
		return nRet;
	}


	CString sTemp(sFontName);
	(*bsFontName) = sTemp;

	return S_OK;
}
HRESULT CPdfWriterLib::LoadTTFFromFile ( CString bsFileName, bool bEmbedding, CString bsEncodingName, CString *bsFontName)
{
    std::wstring sFileName = string2std_string( bsFileName );
	if ( !FileExist( bsFileName ) )
		return AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR;

	const char* sEncodingName = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sEncodingName =  W2A( bsEncodingName.GetBuffer());
#else
    std::string ansiStr(bsEncodingName.begin(), bsEncodingName.end());
    sEncodingName = ansiStr.c_str();
#endif
	if ( NULL != sEncodingName && strlen( sEncodingName ) < 1 )
		sEncodingName = NULL;

    bool bEmbed = ( bEmbedding == 0 ? false : true);

    const char *sFontName = ( bsFontName == NULL ? LoadTTFontFromFile( m_pDocument, sFileName, bEmbed, sEncodingName ) : LoadTTFontFromFile( m_pDocument, sFileName, bEmbed, sEncodingName, "_Embedded" ) );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( AVS_OFFICEPDFWRITER_ERROR_FONT_EXISTS == nRet )
		ResetError( m_pDocument );
	else if ( OK != nRet )
	{
		(*bsFontName) = _T("");

		return nRet;
	}

	CString sTemp(sFontName);
	(*bsFontName) = sTemp;

	return S_OK;
}
HRESULT CPdfWriterLib::LoadTTCFromFile ( CString bsFileName, long nIndex, bool bEmbedding, CString *bsFontName)
{
    std::wstring sFileName = string2std_string( bsFileName );

    const char* sFontName = LoadTTFontFromFile2( m_pDocument, sFileName, (unsigned int)nIndex,  bEmbedding );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
	{
		(*bsFontName) = _T("");
		return nRet;
	}

	CString sTemp(sFontName);
	(*bsFontName) = sTemp;

	return S_OK;
}
HRESULT CPdfWriterLib::SetCurrentFont ( CString bsFontName, CString bsEncodingName, bool bNameIsUnicodeArray, CString bsToUnicodeName)
{
	const char* sFontName      = NULL;
	const char* sEncodingName  = NULL;
	const char* sToUnicodeName = NULL;

#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sFontName      = W2A(bsFontName);
	sEncodingName  = W2A(bsEncodingName);
	sToUnicodeName = W2A(bsToUnicodeName);
#else
    std::string ansiFontName(bsFontName.begin(), bsFontName.end());
    std::string ansiEncodingName(bsEncodingName.begin(), bsEncodingName.end());
    std::string ansiToUnicodeName(bsToUnicodeName.begin(), bsToUnicodeName.end());
    sFontName = ansiFontName.c_str();
    sEncodingName = ansiEncodingName.c_str();
    sToUnicodeName = ansiToUnicodeName.c_str();
#endif
	if ( UtilsStrLen( sEncodingName, LIMIT_MAX_NAME_LEN ) <= 0 )
		sEncodingName = NULL;

	if ( UtilsStrLen( sToUnicodeName, LIMIT_MAX_NAME_LEN ) <= 0 )
		sToUnicodeName = NULL;

	if ( !bNameIsUnicodeArray )
		m_pCurrentFont = GetFont( m_pDocument, sFontName, sEncodingName, sToUnicodeName );
	else
	{
        bool bNewEncoding;
		EncoderRecPtr pCurEncoder = FindEncoderForString( m_pDocument, bsEncodingName, &bNewEncoding );

		m_pCurrentFont = GetFont( m_pDocument, sFontName, pCurEncoder->sName, sToUnicodeName );

		//CStringW oArray = (CStringW)bsEncodingName;
		//int nCount = oArray.GetLength();
		//unsigned short *pUnicode = new unsigned short[nCount];
		//for ( int nIndex = 0; nIndex < nCount; nIndex++ )
		//{
		//	pUnicode[nIndex] = (unsigned short)oArray.GetAt(nIndex);
		//}
		//m_pCurrentFont = GetFont( m_pDocument, sFontName, sEncodingName, pUnicode, nCount );
		//delete []pUnicode;
	}

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetCurrentFont ( CString *pbsFontName, CString *pbsEncodingName )
{
	if ( !m_pCurrentFont )
	{
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_FONT_NOT_FOUND;
	}
	else
	{
		CString sFontName(FontGetFontName( m_pCurrentFont ));
		CString sEncodingName(((FontAttr)m_pCurrentFont->pAttr)->pEncoder->sName);

		*pbsFontName     = sFontName;
		*pbsEncodingName = sEncodingName;
	}

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetFontUnicodeWidth (long nCode, long *pnUnicode)
{
	if ( !m_pCurrentFont )
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_FONT_NOT_FOUND;
	*pnUnicode = (long)FontGetUnicodeWidth( m_pCurrentFont, (unsigned int)nCode );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetFontBBox         (float *pfLeft, float *pfBottom, float *pfRight, float *pfTop)
{
	if ( !m_pCurrentFont )
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_FONT_NOT_FOUND;
	Box oBox = { 0, 0, 0, 0};
	oBox = FontGetBBox( m_pCurrentFont );
	*pfLeft   = oBox.fLeft;
	*pfBottom = oBox.fBottom;
	*pfRight  = oBox.fRight;
	*pfTop    = oBox.fTop;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetFontAscent       (long *pnAscent)	
{
	if ( !m_pCurrentFont )
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_FONT_NOT_FOUND;

	*pnAscent = (long)FontGetAscent( m_pCurrentFont );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetFontDescent      (long *pnDescent)
{
	if ( !m_pCurrentFont )
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_FONT_NOT_FOUND;
	*pnDescent = (long)FontGetDescent( m_pCurrentFont );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetFontXHeight      (long *pnXHeight)
{
	if ( !m_pCurrentFont )
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_FONT_NOT_FOUND;
	*pnXHeight = (long)FontGetXHeight( m_pCurrentFont );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetFontCapHeight    (long *pnCapHeight)
{
	if ( !m_pCurrentFont )
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_FONT_NOT_FOUND;
	*pnCapHeight = (long)FontGetCapHeight( m_pCurrentFont );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

//-----------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::SetCurrentEncoder ( CString bsEncodingName )
{	
	const char *sEncodingName = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sEncodingName = W2A(bsEncodingName);
#else
    std::string ansiEncodingName(bsEncodingName.begin(), bsEncodingName.end());
    sEncodingName = ansiEncodingName.c_str();
#endif
	unsigned long nRet = DocSetCurrentEncoder( m_pDocument, sEncodingName);
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetCurrentEncoder ( CString *pbsEncodingName )
{
	EncoderRecPtr pCurEncoder = DocGetCurrentEncoder( m_pDocument );
	if ( !pCurEncoder )
		*pbsEncodingName = _T("");
	else
	{
		CString sEncodingName(pCurEncoder->sName);

        *pbsEncodingName = sEncodingName;
	}

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

HRESULT CPdfWriterLib::GetEncoderUnicode (long nCode, long *pnUnicode)
{
	EncoderRecPtr pCurEncoder = DocGetCurrentEncoder( m_pDocument );
	*pnUnicode = EncoderGetUnicode( pCurEncoder, nCode );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetEncoderByteType (CString bsText, long nIndex, long *pnByteType)
{
	const char *sText = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sText = W2A(bsText);
#else
    std::string ansiStr(bsText.begin(), bsText.end());
    sText = ansiStr.c_str();
#endif
	EncoderRecPtr pCurEncoder = DocGetCurrentEncoder( m_pDocument);
	*pnByteType = (long)EncoderGetByteType( pCurEncoder, sText, nIndex );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetEncoderType (long *pnEncoderType)
{
	EncoderRecPtr pCurEncoder = DocGetCurrentEncoder( m_pDocument);
	*pnEncoderType = (long)EncoderGetType( pCurEncoder );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetEncoderWritingMode (long *pnWritingMode)
{
	EncoderRecPtr pCurEncoder = DocGetCurrentEncoder( m_pDocument);
	*pnWritingMode = (long)EncoderGetWritingMode( pCurEncoder );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

//-----------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::CreateNewDestination ()
{
	if ( !m_pDestList )
		m_pDestList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	m_pCurrentDestination = PageCreateDestination( m_pDocument->pCurPage );
	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
		return nRet;

	if ( OK != ( nRet = ListAdd( m_pDestList, m_pCurrentDestination ) ) )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::DestSetXYZ (float fLeft, float fTop, float fZoom)
{
	unsigned long nRet = DestinationSetXYZ( m_pCurrentDestination, fLeft, fTop, fZoom );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::DestSetFit ()
{
	unsigned long nRet = DestinationSetFit( m_pCurrentDestination );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::DestSetFitH (float fTop)
{
	unsigned long nRet = DestinationSetFitH( m_pCurrentDestination, fTop );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::DestSetFitV (float fLeft)
{
	unsigned long nRet = DestinationSetFitV( m_pCurrentDestination, fLeft );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::DestSetFitR (float fLeft, float fBottom, float fRight, float fTop)
{
	unsigned long nRet = DestinationSetFitR( m_pCurrentDestination, fLeft, fBottom, fRight, fTop );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::DestSetFitB ()
{
	unsigned long nRet = DestinationSetFitB( m_pCurrentDestination );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::DestSetFitBH (float fTop)
{
	unsigned long nRet = DestinationSetFitBH( m_pCurrentDestination, fTop );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::DestSetFitBV (float fLeft)
{
	unsigned long nRet = DestinationSetFitBV( m_pCurrentDestination, fLeft );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}

HRESULT CPdfWriterLib::SetCurrentDest (long nIndex)
{
	Destination pTemp = (Destination)ListItemAt( m_pDestList, nIndex );
	m_pCurrentDestination = pTemp;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

HRESULT CPdfWriterLib::GetCurrentDestIndex (long *pnIndex)
{
	if ( !m_pCurrentDestination || !m_pDestList )
	{
		*pnIndex = -1;
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_DESTINATION_NOT_FOUND;
	}
	else
		*pnIndex = ListFind( m_pDestList, m_pCurrentDestination );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
//------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::CreateTextAnnot (float fLeft, float fBottom, float fRight, float fTop, CString bsText, CString bsEncoder)
{
	const char* sEncoder = NULL;
	const char* sText = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sText = W2A(bsText);
	sEncoder = W2A(bsEncoder);
#else
    std::string ansiStr(bsText.begin(), bsText.end());
    sText = ansiStr.c_str();

    std::string ansiEncoder(bsEncoder.begin(), bsEncoder.end());
    sEncoder = ansiEncoder.c_str();
#endif
	EncoderRecPtr pEncoder = NULL;
	if ( UtilsStrLen( sEncoder, LIMIT_MAX_NAME_LEN ) > 0 )
		pEncoder = GetEncoder( m_pDocument, sEncoder );

	TRect oRect = { fLeft, fBottom, fRight, fTop};

	if ( !m_pAnnotList )
		m_pAnnotList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	m_pCurrentAnnotation = PageCreateTextAnnot( m_pDocument->pCurPage, oRect, sText, pEncoder);
	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
		return nRet;

	if ( OK != ( nRet = ListAdd( m_pAnnotList, m_pCurrentAnnotation) ) )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::CreateLinkAnnot (float fLeft, float fBottom, float fRight, float fTop)
{
	unsigned long nRet = OK;

	if ( !m_pAnnotList )
	{
		m_pAnnotList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
			return nRet;
	}

	TRect oRect = { fLeft, fBottom, fRight, fTop};
	if ( !m_pCurrentDestination )
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_DESTINATION_NOT_FOUND;

	m_pCurrentAnnotation = PageCreateLinkAnnot( m_pDocument->pCurPage, oRect, m_pCurrentDestination );
	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
		return nRet;

	if ( OK != ( nRet = ListAdd( m_pAnnotList, m_pCurrentAnnotation) ) )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::CreateUriLinkAnnot (float fLeft, float fBottom, float fRight, float fTop, CString bsUrl)
{
	unsigned long nRet = OK;
	if ( !m_pAnnotList )
	{
		m_pAnnotList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
			return nRet;
	}

	const char* sUrl = NULL;

#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sUrl = W2A(bsUrl);
#else
    std::string ansiUrl (bsUrl.begin(), bsUrl.end());
    sUrl = ansiUrl.c_str();
#endif	
	TRect oRect = { fLeft, fBottom, fRight, fTop};

	m_pCurrentAnnotation = PageCreateUriLinkAnnot( m_pDocument->pCurPage, oRect, sUrl );
	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
		return nRet;

	if ( OK != ( nRet = ListAdd( m_pAnnotList, m_pCurrentAnnotation) ) )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::ArbAnnotationSetBorderStyle (long nSubtype, float fWidth, long nDashOn, long nDashOff, long nDashPhase)
{
	unsigned long nRet = AnnotationSetBorderStyle( m_pCurrentAnnotation, (BSSubtype)nSubtype, fWidth, (unsigned short)nDashOn, (unsigned short)nDashOff, (unsigned short)nDashPhase );
	if( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::LinkAnnotationSetBorderStyle (float fWidth, long nDashOn, long nDashOff )
{
	unsigned long nRet = LinkAnnotSetBorderStyle( m_pCurrentAnnotation, fWidth, (unsigned short)nDashOn, (unsigned short)nDashOff );
	if( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::LinkAnnotationSetHighlightMode (long nMode)
{
	unsigned long nRet = LinkAnnotSetHighlightMode( m_pCurrentAnnotation, (AnnotHighlightMode)nMode );
	if( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::TextAnnotationSetIcon (long nIcon)
{
	unsigned long nRet = TextAnnotSetIcon( m_pCurrentAnnotation, (AnnotIcon)nIcon );
	if( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::TextAnnotationSetOpened (bool bOpened)
{
	unsigned long nRet = TextAnnotSetOpened( m_pCurrentAnnotation, bOpened );
	if( OK != nRet )
		return nRet;
	return S_OK;
}

HRESULT CPdfWriterLib::SetCurrentAnnot (long nIndex)
{
	Annotation pTemp = (Annotation)ListItemAt( m_pAnnotList, nIndex );
	m_pCurrentAnnotation = pTemp;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

HRESULT CPdfWriterLib::GetCurrentAnnotIndex (long *pnIndex)
{
	if ( !m_pCurrentAnnotation || !m_pAnnotList )
	{
		*pnIndex = -1;
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_ANNOTATION_NOT_FOUND;
	}
	else
		*pnIndex = ListFind( m_pAnnotList, m_pCurrentAnnotation );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::MakeAnnotationFromXml (CString bsXML)
{
	HRESULT hRes = S_OK;

	// Запоминаем текущую страницу
	long nStartPageIndex = 0;
    if ( S_OK != ( hRes = GetCurrentPageIndex( &nStartPageIndex ) ) )
		return hRes;

	// Считываем количество страниц (для контроля)
	long nPagesCount = 0;
    if ( S_OK != ( hRes = GetPagesCount( &nPagesCount ) ) )
		return hRes;

	float fPageHeight = 0;
	float fPageWidth  = 0;

    if ( S_OK != ( hRes = GetWidth( &fPageWidth ) ) )
		return hRes;
    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;


	XmlUtils::CXmlNode oLinker;
	oLinker.FromXmlString( CString( bsXML ) );

	if ( -1 != oLinker.GetName().Find( _T("linker") ) )
	{
		XmlUtils::CXmlNodes oLinks;
		oLinker.GetNodes( _T("link"), oLinks );

		for ( int nIndex = 0; nIndex < oLinks.GetCount(); nIndex++ )
		{
			XmlUtils::CXmlNode oLink;
			oLinks.GetAt( nIndex, oLink );

			XmlUtils::CXmlNode oSource;
			XmlUtils::CXmlNode oTarget;

			float fSrcX = 0;
			float fSrcY = 0;
			float fSrcH = 0;
			float fSrcW = 0;
			int nSrcPageIndex = nStartPageIndex;

			if ( oLink.GetNode( _T("source"), oSource ) )
			{
				CString sValue;

				sValue = oSource.GetAttribute( _T("page"), _T("0") );
                long nPagesCountRead = _wtoi( sValue.GetBuffer());
                nSrcPageIndex = (std::max)( (long)0, (std::min)( nPagesCount - 1, nPagesCountRead) );

				sValue = oSource.GetAttribute( _T("x"), _T("0") );
				fSrcX = (float)_wtof( sValue.GetBuffer() );

				sValue = oSource.GetAttribute( _T("y"), _T("0") );
				fSrcY = (float)_wtof( sValue.GetBuffer() );

				sValue = oSource.GetAttribute( _T("width"), _T("0") );
				fSrcW = (float)_wtof( sValue.GetBuffer() );

				sValue = oSource.GetAttribute( _T("height"), _T("0") );
				fSrcH = (float)_wtof( sValue.GetBuffer() );
			}

			float fDstX = 0;
			float fDstY = 0;
			float fDstH = 0;
			float fDstW = 0;
			int nDstPageIndex = nStartPageIndex;
			CString sURL = _T("");
            bool bLinkURL = false;

			if ( oLink.GetNode( _T("target"), oTarget ) )
			{
				CString sValue;

				sValue = oTarget.GetAttribute( _T("url"), _T("") );
				if ( sValue.GetLength() > 0 )
				{
					sURL = sValue;
                    bLinkURL = true;
				}
				else
				{
					sValue = oTarget.GetAttribute( _T("page"), _T("0") );
                    nDstPageIndex = (std::max)( 0, (std::min)( (int)nPagesCount - 1, _wtoi( sValue.GetBuffer() ) ) );

					sValue = oTarget.GetAttribute( _T("x"), _T("0") );
					fDstX = (float)_wtof( sValue.GetBuffer() );

					sValue = oTarget.GetAttribute( _T("y"), _T("0") );
					fDstY = (float)_wtof( sValue.GetBuffer() );

					sValue = oTarget.GetAttribute( _T("width"), _T("0") );
					fDstW = (float)_wtof( sValue.GetBuffer() );

					sValue = oTarget.GetAttribute( _T("height"), _T("0") );
					fDstH = (float)_wtof( sValue.GetBuffer() );
				}
			}

			if ( bLinkURL )
			{
                if ( S_OK != ( hRes = SetCurrentPage( nSrcPageIndex ) ) )
					return hRes;

                if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
                    return hRes;

                if ( S_OK != ( hRes = CreateUriLinkAnnot( fSrcX, fPageHeight - ( fSrcY + fSrcH ), fSrcX + fSrcW, fPageHeight - fSrcY, sURL  ) )  )
					return hRes;

                if ( S_OK != ( hRes  = ArbAnnotationSetBorderStyle( 0, 0, 0, 0, 0 ) ) )
					return hRes;
			}
			else
			{
				// Создаем объект Destination
                if ( S_OK != ( hRes = SetCurrentPage( nDstPageIndex ) ) )
					return hRes;

                if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
					return hRes;

                if ( S_OK != ( hRes = CreateNewDestination() ) )
					return hRes;

                if ( S_OK != ( hRes = DestSetXYZ( 0, fPageHeight - fDstY, 0  ) ) )
					return hRes;

				// Создаем ссылку на текущий объект Destionation
                if ( S_OK != ( hRes = SetCurrentPage( nSrcPageIndex ) ) )
					return hRes;

                if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
					return hRes;

                if ( S_OK != ( hRes = CreateLinkAnnot( fSrcX, fPageHeight - ( fSrcY + fSrcH ), fSrcX + fSrcW, fPageHeight - fSrcY ) ) )
					return hRes;

                if ( S_OK != ( hRes  = ArbAnnotationSetBorderStyle( 0, 0, 0, 0, 0 ) ) )
					return hRes;

			}
		}
	}

	// Восстанавливаем текущую страницу
    if ( S_OK != ( hRes = SetCurrentPage( nStartPageIndex ) ) )
		return hRes;

	return S_OK;
}
//------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::CreateNewOutline (long nParentIndex, CString bsTitle, CString bsEncoder)
{
	unsigned long nRet = OK;

	const char* sTitle = NULL;
	const char* sEncoder = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sTitle = W2A(bsTitle);
	sEncoder = W2A(bsEncoder);
#else
    std::string ansiEncoder(bsEncoder.begin(), bsEncoder.end());
    sEncoder = ansiEncoder.c_str();

    std::string ansiTitle(bsTitle.begin(), bsTitle.end());
    sTitle = ansiTitle.c_str();
#endif	

	EncoderRecPtr pEncoder = NULL;
	if ( UtilsStrLen( sEncoder, LIMIT_MAX_NAME_LEN ) > 0 )
	{
		pEncoder = GetEncoder( m_pDocument, sEncoder );
		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
			return nRet;
	}

	if ( !m_pOutlineList )
	{
		m_pOutlineList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
			return nRet;
	}

	Outline pRoot = NULL;
	if ( nParentIndex < m_pOutlineList->nCount && nParentIndex >= 0 )
	{
		pRoot = (Outline)ListItemAt( m_pOutlineList, nParentIndex );
		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
			return nRet;
	}

	m_pCurrentOutline = CreateOutline( m_pDocument, pRoot, sTitle, pEncoder );
	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
		return nRet;

	if ( OK != ( nRet = ListAdd( m_pOutlineList, m_pCurrentOutline ) ) )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::OutlineSetCurrentDestination ()
{
	unsigned long nRet = OutlineSetDestination( m_pCurrentOutline, m_pCurrentDestination );
	if( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::SetOutlineOpened (bool bOpened)
{
	unsigned long nRet = OutlineSetOpened( m_pCurrentOutline, bOpened );
	if( OK != nRet )
		return nRet;
	return S_OK;
}

HRESULT CPdfWriterLib::SetCurrentOutline (long nIndex)
{
	Outline pTemp = (Outline)ListItemAt( m_pOutlineList, nIndex );
	m_pCurrentOutline = pTemp;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

HRESULT CPdfWriterLib::GetCurrentOutlineIndex (long *pnIndex)
{
	if ( !m_pCurrentOutline || !m_pOutlineList )
	{
		*pnIndex = -1;
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_OUTLINE_NOT_FOUND;
	}
	else
		*pnIndex = ListFind( m_pOutlineList, m_pCurrentOutline );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::MakeOutlineFromXml (CString bsXML)
{
	HRESULT hRes = S_OK;
	// текущая страница изменится, поэтому ее запоминаем и восстанавливаем
	long nCurPageIndex = 0;

    if ( S_OK != ( hRes = GetCurrentPageIndex( &nCurPageIndex ) ) )
		return hRes;
	CString sXml(bsXML);

	XmlUtils::CXmlLiteReader oReader;				
	oReader.FromString(sXml);

	CString nameNode;
	if (oReader.ReadNextNode())
		nameNode = oReader.GetName();

	if ( -1 != nameNode.Find( _T("PDF-Chapters") ) || -1 != nameNode.Find( _T("Outline") ) )
        if ( S_OK != ( hRes = ReadOutlineChilds( oReader.GetOuterXml(), -1 ) ) )
			return hRes;

    if ( S_OK !=  ( hRes = SetCurrentPage( nCurPageIndex ) ) )
		return hRes;

	return S_OK;
}
//------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::LoadJpegFromFile (CString bsFileName)
{
	unsigned long nRet = OK;
	CString sFileName( bsFileName );

	if ( !m_pXObjectList )
	{
		m_pXObjectList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
			return nRet;
	}

	m_pCurrentXObject = LoadJpegImageFromFile( m_pDocument, string2std_string(sFileName));

	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
		return nRet;

	if ( OK != ( nRet = ListAdd( m_pXObjectList, m_pCurrentXObject) ) ) 
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::LoadJpxFromFile (CString bsFileName, int nFlag)
{
	unsigned long nRet = OK;

	CString sFileName( bsFileName );

	if ( !m_pXObjectList )
	{
		m_pXObjectList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
			return nRet;
	}

	// Параметр nFlag соответсвуют значению SMaskInData в записи о загружаемом Jpx
	// nFlag: 0, даже если в Jpx есть прозрачность, игнорируем ее
	//        1, наличие в Jpx SMask
	//        2, наличие в Jpx отдельных каналов с прозрачностью

    long nOpacityFlag = (std::max)( 0, (std::min) ( 2, nFlag ));

	m_pCurrentXObject = LoadJpxImageFromFile( m_pDocument, string2std_string(sFileName), nOpacityFlag );
	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
		return nRet;

	if ( OK != ( nRet = ListAdd( m_pXObjectList, m_pCurrentXObject) ) ) 
		return nRet;

	return S_OK;
}
//HRESULT CPdfWriterLib::LoadImageFromFile (CString bsFileName,     long nWidth, long nHeight, long nColorSpace)
//{
//	unsigned long nRet = OK;
//
//	CString sFileName( bsFileName );
//
//	if ( !m_pXObjectList )
//	{
//		m_pXObjectList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
//		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
//			return nRet;
//	}
//
//	m_pCurrentXObject = LoadRawImageFromFile( m_pDocument, sFileName.GetBuffer(), nWidth, nHeight, (ColorSpace)nColorSpace );
//	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
//		return nRet;
//
//	if ( OK != ( nRet = ListAdd( m_pXObjectList, m_pCurrentXObject) ) ) 
//		return nRet;
//
//	return S_OK;
//}
//HRESULT CPdfWriterLib::LoadImageFromArray (SAFEARRAY **ppImage, long nWidth, long nHeight, long nColorSpace, long nBitsPerComponent)
//{
//	unsigned long nRet = OK;
//
//	BYTE *pArray;
//	if ( nHeight && nWidth )
//	{
//		// check for valid pixel array size
//		if ( 1 != SafeArrayGetDim(*ppImage) )
//			return AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE;
//
//		VARTYPE vType;
//		// check for valid array element type
//		if ( S_OK != (SafeArrayGetVartype(*ppImage, &vType)) || VT_UI1 != vType )
//			return AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE;
//
//		pArray = (BYTE* (*ppImage)->pvData;
//	}
//	if ( !m_pXObjectList )
//	{
//		m_pXObjectList = ListNew( m_pDocument->oMMgr, DEF_ITEMS_PER_BLOCK );
//		if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
//			return nRet;
//	}
//
//	m_pCurrentXObject = LoadRawImageFromMem( m_pDocument, pArray, nWidth, nHeight, (ColorSpace)nColorSpace, nBitsPerComponent );
//	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
//		return nRet;
//
//	if ( OK != ( nRet = ListAdd( m_pXObjectList, m_pCurrentXObject) ) ) 
//		return nRet;
//
//	return S_OK;
//}
//
HRESULT CPdfWriterLib::GetImageBitsPerComponent (long *pnBitsPerComponent)
{
	*pnBitsPerComponent = ImageGetBitsPerComponent( m_pCurrentXObject );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetImageColorSpace (long *pnColorSpace)
{
	EColorSpace eCS = ImageGetColorSpace2( m_pCurrentXObject );
	*pnColorSpace = (long)eCS;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetImageHeight (long *pnHeight)
{
	*pnHeight = ImageGetHeight( m_pCurrentXObject );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::GetImageWidth (long *pnWidth)
{
	*pnWidth = ImageGetWidth( m_pCurrentXObject );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetImageColorMask (long nRMin, long nRMax, long nGMin, long nGMax, long nBMin, long nBMax)
{
	unsigned long nRet = ImageSetColorMask( m_pCurrentXObject, nRMin, nRMax, nGMin, nGMax, nBMin, nBMax );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::SetImageMaskImage (long nMaskImageIndex)
{
	unsigned long nRet = OK;
	XObject pTemp = (XObject)ListItemAt( m_pXObjectList, nMaskImageIndex );
	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
		return nRet;

	if ( OK != ( nRet = ImageSetMaskImage( m_pCurrentXObject, pTemp ) ) )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetCurrentImage (long nIndex)
{
	XObject pTemp = (XObject)ListItemAt( m_pXObjectList, nIndex );
	m_pCurrentXObject = pTemp;

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

HRESULT CPdfWriterLib::GetCurrentImageIndex (long *pnIndex)
{
	if ( !m_pCurrentXObject || !m_pXObjectList )
	{
		*pnIndex = -1;
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_IMAGE_NOT_FOUND;
	}
	else
		*pnIndex = ListFind( m_pXObjectList, m_pCurrentXObject );

	// проверяем не появилась ли ошибка
	unsigned long nRet = ErrorGetCode( &m_pDocument->oError );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

//------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::SetEncryptOn ()
{
	unsigned long nRet = DocSetEncryptOn( m_pDocument );
	if ( OK != nRet )
		return nRet;
	return S_OK;
}
HRESULT CPdfWriterLib::SetPDFPassword (CString bsOwnerPassword, CString bsUserPassword)
{	
	const char* sOwnerPassword = NULL;
	const char* sUserPassword  = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sOwnerPassword = W2A( bsOwnerPassword );
	sUserPassword  = W2A( bsUserPassword );
#else
    std::string ansiOwnerPassword(bsOwnerPassword.begin(), bsOwnerPassword.end());
    sOwnerPassword = ansiOwnerPassword.c_str();

    std::string ansiUserPassword(bsUserPassword.begin(), bsUserPassword.end());
    sUserPassword = ansiUserPassword.c_str();
#endif
	if ( UtilsStrLen(sOwnerPassword, LIMIT_MAX_NAME_LEN ) <= 0 )
		sOwnerPassword = NULL;

	if ( UtilsStrLen(sUserPassword, LIMIT_MAX_NAME_LEN ) <= 0 )
		sUserPassword = NULL;

	// Паролей нет, ничего не делаем
	if ( NULL == sOwnerPassword && NULL == sUserPassword )
		return S_OK;

	// Если пароли одинаковые, тогда устанавливаем только пользовательский пароль
	if ( sOwnerPassword && sUserPassword && 0 == UtilsStrCmp( sOwnerPassword, sUserPassword ) )
		sUserPassword = NULL;

	unsigned long nRet = SetPassword( m_pDocument, sOwnerPassword, sUserPassword );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetPDFPermission (long nPermission)
{
	unsigned long nRet = SetPermission( m_pDocument, (unsigned long)nPermission);
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetPDFEncryptionMode (long nMode, long nKeyLen)
{
	unsigned long nRet = SetEncryptionMode( m_pDocument, (EEncryptMode)nMode, nKeyLen );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetEncryptOff ()
{
	unsigned long nRet = DocSetEncryptOff( m_pDocument );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

//------------------------------------------------------------------------------------------------
HRESULT CPdfWriterLib::GetInfoItem (long nDataType, CString *pbsItem)
{
	unsigned long nRet = OK;
	const char* sItem = GetInfoAttr( m_pDocument, (InfoType)nDataType);
	if ( OK != ( nRet = ErrorGetCode( &m_pDocument->oError ) ) )
	{
		*pbsItem = _T("");
		return nRet;
	}

	CString sTemp(sItem);
    *pbsItem = sTemp;
	return S_OK;
}
HRESULT CPdfWriterLib::SetInfoItem (long nDataType, CString bsData)
{	
	const char *sData = NULL;
#if defined(_WIN32) || defined (_WIN64)
	USES_CONVERSION;
	sData = W2A(bsData);
#else
    std::string ansiData(bsData.begin(), bsData.end());
    sData = ansiData.c_str();
#endif
	unsigned long nRet = SetInfoAttr( m_pDocument, (InfoType)nDataType, sData );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
HRESULT CPdfWriterLib::SetInfoDateItem (long nType, long nYear, long nMonth, long nDay, long nHour, long nMinutes, long nSeconds, long nInd, long nOffHour, long nOffMinutes)
{
	TDate oDate;
	oDate.nYear = nYear;
	oDate.nMinutes = nMinutes;
	oDate.nMonth = nMonth;
	oDate.nDay = nDay;
	oDate.nHour = nHour;
	oDate.nSeconds = nSeconds;
	oDate.nInd = nInd;
	oDate.nOffHour = nOffHour;
	oDate.nOffMinutes = nOffMinutes;

	unsigned long nRet = SetInfoDateAttr( m_pDocument, (InfoType)nType, oDate );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}

namespace NSCommonReader
{
    inline _INT32 ReadInt(BYTE*& pData, int& nOffset)
    {
#ifdef _ARM_ALIGN_
        _INT32 ret = 0;
        memcpy(&ret, pData, sizeof(_INT32));
        pData   += 4;
        nOffset += 4;
        return ret;
#else
        _INT32 ret = *((_INT32*)pData);
        pData   += 4;
        nOffset += 4;
        return ret;
#endif
    }
    
    inline USHORT ReadUSHORT(BYTE*& pData, int& nOffset)
    {
#ifdef _ARM_ALIGN_
        USHORT ret = 0;
        memcpy(&ret, pData, sizeof(USHORT));
        pData   += 2;
        nOffset += 2;
        return ret;
#else
        USHORT ret = *((USHORT*)pData);
        pData   += 2;
        nOffset += 2;
        return ret;
#endif
    }
    
    inline std::wstring ReadString16(BYTE*& pData, int& nOffset, int nLen)
    {
        std::wstring wsTempString;
#ifdef _ARM_ALIGN_
        
        if (sizeof(wchar_t) == 4)
        {
#if !defined(_WIN32) && !defined(_WIN64)
            int len = nLen / 2;
            UTF16* buf = new UTF16[len];
            memcpy(buf, pData, nLen);
            wsTempString = stringUtf16ToWString(buf, len);
            RELEASEARRAYOBJECTS(buf);
#endif
        }
        else
        {
            int len = nLen / 2;
            wchar_t* buf = new wchar_t[len];
            memcpy(buf, pData, nLen);
            wsTempString = std::wstring(buf, len);
            RELEASEARRAYOBJECTS(buf);
        }
        
#else
        
        if (sizeof(wchar_t) == 4)
        {
#if !defined(_WIN32) && !defined(_WIN64)
            wsTempString = stringUtf16ToWString((UTF16*)pData, nLen/2);
#endif
        }
        else
        {
            wsTempString = std::wstring((wchar_t*)pData, nLen/2);
        }
        
#endif
        pData += nLen;
        nOffset += nLen;
        return wsTempString;
    }
}

void CPdfWriterLib::OnlineWordToPdfInternal(BYTE* dstArray, LONG len, const std::wstring& sHtmlPlace, std::wstring& sHypers, int& nCountPages, CString sTempLogo, LONG nReg)
{
    using namespace NOnlineOfficeBinToPdf;

    NOnlineOfficeBinToPdf::CommandType eCommand = NOnlineOfficeBinToPdf::ctError;
    
    bool bIsPathOpened = false;
    int curindex = 0;

    _INT32* m = NULL;
    USHORT* ms = NULL;
    int _sLen = 0;
    _INT32 gradientType = 0;
        
    double m1 = 0;
    double m2 = 0;
    double m3 = 0;
    double m4 = 0;
    double m5 = 0;
    double m6 = 0;
    
    BYTE* p = dstArray;
    {
        BYTE* current = p;
        while (curindex < len)
        {
            eCommand = (CommandType)(*current);
            current++;
            curindex++;
            switch (eCommand)
            {
                case ctPageWidth:
                    //ATLTRACE2 ("CommandType::ctPageWidth:\n");
                    put_Width (NSCommonReader::ReadInt(current, curindex) / 100000.0);
                    break;
                case ctPageHeight:
                    //ATLTRACE2 ("CommandType::ctPageHeight:\n");
                    put_Height (NSCommonReader::ReadInt(current, curindex) / 100000.0);
                    break;
                case ctPageStart:
                    //ATLTRACE2 ("\nCommandType::ctPageStart:\n");
                    NewPage();
                    BeginCommand(1);
                    ++nCountPages;
                    break;
                case ctPageEnd:
                    //ATLTRACE2 ("CommandType::ctPageEnd:\n");
                    if (bIsPathOpened)
                    {
                        PathCommandEnd();
                        EndCommand(4);
                    }
                    
                    bIsPathOpened = false;
                    
                    if (!nReg)
                    {
                        double ww;
                        get_Width(&ww);
                        double wh;
                        get_Height(&wh);
                        
                        double dR = ww - 13;
                        double dB = wh - 5;
                        double k = 1.0;
                        
                        if (ww <= wh)
                        {
                            // ориентация книжная.
                            double k1 = ww / 210;
                            double k2 = wh / 297;
                            k = (std::min)(k1, k2);
                        }
                        else
                        {
                            double k1 = ww / 297;
                            double k2 = wh / 210;
                            k = (std::min)(k1, k2);
                        }
                        double dW = 46 * k;
                        double dH = 15 * k;
                        
                        double dKoef = 72 / 25.4;
                        
                        CString sTempHyper;
                        sTempHyper.Format(_T("<link><source x=\"%i\" y=\"%i\" width=\"%i\" height=\"%i\" page=\"%i\"/>"), (int)((dR - dW) * dKoef), (int)((dB - dH) * dKoef), (int)(dW * dKoef), (int)(dH * dKoef), nCountPages - 1);
                        
                        sHypers += sTempHyper;
                        sHypers += _T("<target url=\"www.onlyoffice.com\"/></link>");
                        
                        ResetTransform();
                        
                        bool bWriteTextFile = true;
                        if (sTempLogo.IsEmpty())
                        {
                            sTempLogo = FileSystem::Directory::GetTempPath();
                            bWriteTextFile = WriteFile_ (sTempLogo, (BYTE*) cpLogo1);
                        }
                        if (bWriteTextFile)
                        {
                            DrawImageFromFile (string2std_string(sTempLogo), dR - dW, dB - dH, dW, dH);
                        }
                    }
                    
                    EndCommand(1);
                    break;
                case ctPenColor:
                    //ATLTRACE2 ("CommandType::ctPenColor:\n");
                    put_PenColor (NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctPenAlpha:
                    //ATLTRACE2 ("CommandType::ctPenAlpha:\n");
                    put_PenAlpha (*current);
                    current++;
                    curindex++;
                    break;
                case ctPenSize:
                    //ATLTRACE2 ("CommandType::ctPenSize:\n");
                    put_PenSize (NSCommonReader::ReadInt(current, curindex) / 100000.0);
                    break;
                case ctPenLineJoin:
                    //ATLTRACE2 ("CommandType::ctPenLineJoin:\n");
                    put_PenLineJoin (*current);
                    current++;
                    curindex++;
                    break;
                case ctBrushType:
                    //ATLTRACE2 ("CommandType::ctBrushType:\n");
                    put_BrushType (NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctBrushColor1:
                    //ATLTRACE2 ("CommandType::ctBrushColor1:\n");
                    put_BrushColor1 (NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctBrushAlpha1:
                    //ATLTRACE2 ("CommandType::ctBrushAlpha1:\n");
                    put_BrushAlpha1 (*current);
                    current++;
                    curindex++;
                    break;
                case ctBrushColor2:
                    //ATLTRACE2 ("CommandType::ctBrushColor2:\n");
                    put_BrushColor1 (NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctBrushAlpha2:
                    //ATLTRACE2 ("CommandType::ctBrushAlpha2:\n");
                    put_BrushAlpha2 (*current);
                    current++;
                    curindex++;
                    break;
                case ctBrushRectable:
                    //ATLTRACE2 ("CommandType::ctBrushRectable:\n");
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    
                    BrushRect(0, m1, m2, m3, m4);
                    break;
                case ctBrushRectableEnabled:
                {
                    //ATLTRACE2 ("CommandType::ctBrushRectableEnabled:\n");
                    
                    VARIANT bEn;
                    bEn.vt = VT_BOOL;
                    bEn.boolVal = (1 == *current) ? VARIANT_TRUE : VARIANT_FALSE;
                    
                    SetAdditionalParam(L"BrushFillBoundsEnable", bEn);
                    
                    current += 1;
                    curindex += 1;
                }
                    break;
                case ctBrushTexturePath:
                {
                    _sLen = 2 * NSCommonReader::ReadUSHORT(current, curindex);
                    
                    std::wstring wsTempString = NSCommonReader::ReadString16(current, curindex, _sLen);
                    
                    CString base64TempFile;
                    if (0 == wsTempString.find(_T("data:")))
                    {
                        try
                        {
                            int nFind = wsTempString.find(_T(","));
                            wsTempString =  wsTempString.substr(nFind + 1);
                            
                            base64TempFile = FileSystem::Directory::GetTempPath();
                            
                            std::string sBase64MultyByte(wsTempString.begin(), wsTempString.end());
                            
                            int nBuffLen	= Base64::Base64DecodeGetRequiredLength(sBase64MultyByte.length());
                            BYTE *byteIm	= new BYTE[nBuffLen+64];
                            
                            if (Base64::Base64Decode(sBase64MultyByte.c_str(), sBase64MultyByte.length(),
                                                     byteIm, &nBuffLen))
                            {
                                WriteFile_ (base64TempFile, byteIm);
                                wsTempString = string2std_string(base64TempFile);
                            }
                            else throw;
                        }
                        catch (...)
                        {
                            //ATLTRACE2 ("CommandType::ctBrushTexturePath:base64Decoder failed!\n");
                        }
                    }
                    else
                    {
                        if (0 != wsTempString.find(_T("http:")) &&
                            0 != wsTempString.find(_T("https:")) &&
                            0 != wsTempString.find(_T("ftp:")) &&
                            0 != wsTempString.find(_T("file:")))
                        {
                            if (0 == wsTempString.find(_T("theme")))
                            {
                                if (m_sThemesPlace.length() > 0)
                                    wsTempString = m_sThemesPlace + _T("/") + wsTempString;
                            }
                            else
                            {
                                if (sHtmlPlace.length() > 0)
                                    wsTempString = sHtmlPlace + _T("/") + wsTempString;

                                std::wstring sSvgExt(_T(".svg"));
                                if (0 == wsTempString.compare(wsTempString.length() - sSvgExt.length(), std::wstring::npos, sSvgExt))
                                {
                                    std::wstring sTestPath = wsTempString.substr(0, wsTempString.length() - sSvgExt.length());
                                    if (NSFile::CFileBinary::Exists(sTestPath + _T(".emf")))
                                        wsTempString = sTestPath + _T(".emf");
                                    else if (NSFile::CFileBinary::Exists(sTestPath + _T(".wmf")))
                                        wsTempString = sTestPath + _T(".wmf");
                                }
                            }
                            
                            string_replaceAll(wsTempString, _T("\\"), _T("/") );
                        }
                        
                        if (0 == wsTempString.find(_T("file:///")))
                        {
                            string_replaceAll(wsTempString, _T("file:///"), _T(""));
                            string_replaceAll(wsTempString, _T("\\"), _T("/") );
                        }
                    }

                    
                    put_BrushTexturePath(wsTempString);
                    
                    if (!base64TempFile.IsEmpty())
                    {
                        ::CDirectory::DeleteFile(base64TempFile);
                    }
                }
                    break;
                    
                case ctBrushGradient:
                {
                    current++;
                    curindex++;
                    
                    CString strAttrMain = L"";
                    CString strColors = L"";
                    bool bIsLinear = true;
                    
                    while (true)
                    {
                        BYTE _command = *current;
                        current++;
                        curindex++;
                        
                        if (251 == _command)
                            break;
                        
                        switch (_command)
                        {
                            case 0:
                            {
                                current += 5;
                                curindex += 5;
                                
                                double d1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                
                                strAttrMain.Format (L"x1 = '%f' y1 = '%f' x2 = '%f' y2 = '%f' gradientUnits='userSpaceOnUse' ", d1, d2, d3, d4);
                                break;
                            }
                            case 1:
                            {
                                bIsLinear = false;
                                
                                current++;
                                curindex++;
                                
                                double d1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d5 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d6 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                
                                strAttrMain.Format (L"cx = '%f' cy = '%f' r0 = '%f' r1 = '%f' rx = '%f' ry = '%f' gradientUnits='userSpaceOnUse' ", d1, d2, d5, d6, d1, d2);
                                break;
                            }
                            case 2:
                            {
                                int nCountColors = NSCommonReader::ReadInt(current, curindex);
                                
                                for (int nI = 0; nI < nCountColors; ++nI)
                                {
                                    int pos = NSCommonReader::ReadInt(current, curindex);
                                    
                                    double dPos = pos / 100000.0;
                                    BYTE _r = *current++;
                                    BYTE _g = *current++;
                                    BYTE _b = *current++;
                                    BYTE _a = *current++;
                                    
                                    curindex += 4;
                                    
                                    int _color = ((_b << 16) & 0xFF0000) | ((_g << 8) & 0xFF00) | _r;
                                    
                                    CString sColor;
                                    sColor.Format (L"<stop stop-color='%d' stop-opacity='%f' offset='%f' />", _color, _a / 255.0, dPos);
                                    strColors += sColor;
                                }
                                
                                break;
                            }
                            default:
                                break;
                        };
                    }
                    
                    std::wstring strXml;
                    if (bIsLinear)
                    {
                        strXml = L"<linearGradient " + strAttrMain + L">" + strColors + L"</linearGradient>";
                        CreateLinearGradientFromSvgXml(strXml, true);
                    }
                    else
                    {
                        strXml = L"<radialGradient " + strAttrMain + L">" + strColors + L"</radialGradient>";
                        CreateRadialGradientFromSvgXml(strXml, true);
                    }
                }
                    
                    break;
                    
                case ctBrushTextureMode:
                {
                    //ATLTRACE2 ("CommandType::ctBrushTextureMode:\n");
                    
                    LONG mode = (LONG)(*current);
                    put_BrushTextureMode(mode);
                    
                    current += 1;
                    curindex += 1;
                }
                    break;
                case ctBrushTextureAlpha:
                {
                    //ATLTRACE2 ("CommandType::ctBrushTextureAlpha:\n");
                    
                    LONG txalpha = (LONG)(*current);
                    put_BrushTextureAlpha(txalpha);
                    
                    current += 1;
                    curindex += 1;
                }
                    break;
                case ctSetTransform:
                    //ATLTRACE2 ("CommandType::ctSetTransform:\n");
                    
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m5 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m6 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    
                    SetTransform(m1, m2, m3, m4, m5, m6);
                    break;
                case ctPathCommandStart:
                    //ATLTRACE2 ("CommandType::ctPathCommandStart:\n");
                    if (bIsPathOpened)
                    {
                        PathCommandEnd();
                        EndCommand(4);
                        BeginCommand(4);
                        PathCommandStart();
                    }
                    else
                    {
                        BeginCommand(4);
                        PathCommandStart();
                    }
                    bIsPathOpened = true;
                    break;
                case ctPathCommandMoveTo:
                    //ATLTRACE2 ("CommandType::ctPathCommandMoveTo:\n");
                    
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    PathCommandMoveTo(m1, m2);
                    break;
                case ctPathCommandLineTo:
                    //ATLTRACE2 ("CommandType::ctPathCommandLineTo:\n");
                    
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    PathCommandLineTo(m1, m2);
                    break;
                case ctPathCommandCurveTo:
                    //ATLTRACE2 ("CommandType::ctPathCommandCurveTo:\n");
                    
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m5 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m6 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    
                    PathCommandCurveTo(m1, m2, m3, m4, m5, m6);
                    break;
                case ctPathCommandClose:
                    //ATLTRACE2 ("CommandType::ctPathCommandClose:\n");
                    PathCommandClose();
                    break;
				case ctPathCommandEnd:
					if (bIsPathOpened)
                    {
                        PathCommandEnd();
                        EndCommand(4);
                        bIsPathOpened = false;
                    }
					break;
				case ctDrawPath:
                    //ATLTRACE2 ("CommandType::ctDrawPath\n");
                    
                    DrawPath(NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctDrawImageFromFile:
                {
                    //ATLTRACE2 ("CommandType::ctDrawImageFromFile:\n");
                    _sLen = NSCommonReader::ReadInt(current, curindex);
                    std::wstring wsTempString = NSCommonReader::ReadString16(current, curindex, _sLen);
					
                    CString base64TempFile;
                    if (0 == wsTempString.find(_T("data:")))
                    {
                        try
                        {
                            int nFind = wsTempString.find(_T(","));
                            wsTempString = wsTempString.substr(nFind + 1);
                            
                            base64TempFile = FileSystem::Directory::GetTempPath();
                            
                            std::string sBase64MultyByte(wsTempString.begin(), wsTempString.end());
                            
                            int nBuffLen	= Base64::Base64DecodeGetRequiredLength(sBase64MultyByte.length());
                            BYTE *byteIm	= new BYTE[nBuffLen+64];
                            
                            if (Base64::Base64Decode(sBase64MultyByte.c_str(), sBase64MultyByte.length(), byteIm, &nBuffLen))
                            {
                                WriteFile_ (base64TempFile, byteIm);
                                wsTempString = string2std_string(base64TempFile);
                            }
                            else throw;
                        }
                        catch (...)
                        {
                            //ATLTRACE2 ("CommandType::ctDrawImageFromFile:base64Decoder failed!\n");
                        }
                    }
                    else
                    {
                        if (0 != wsTempString.find(_T("http:")) &&
                            0 != wsTempString.find(_T("https:")) &&
                            0 != wsTempString.find(_T("ftp:")) &&
                            0 != wsTempString.find(_T("file:")))
                        {
                            if (0 == wsTempString.find(_T("theme")))
                            {
								if (m_sThemesPlace.length() > 0)
                                    wsTempString = m_sThemesPlace + _T("/") + wsTempString;
                            }
                            else
                            {
                                if (sHtmlPlace.length() > 0)
                                    wsTempString = sHtmlPlace + _T("/") + wsTempString;

                                std::wstring sSvgExt(_T(".svg"));
                                if (0 == wsTempString.compare(wsTempString.length() - sSvgExt.length(), std::wstring::npos, sSvgExt))
                                {
                                    std::wstring sTestPath = wsTempString.substr(0, wsTempString.length() - sSvgExt.length());
                                    if (NSFile::CFileBinary::Exists(sTestPath + _T(".emf")))
                                        wsTempString = sTestPath + _T(".emf");
                                    else if (NSFile::CFileBinary::Exists(sTestPath + _T(".wmf")))
                                        wsTempString = sTestPath + _T(".wmf");
                                }
                            }
                            
                            string_replaceAll(wsTempString, _T("\\"), _T("/") );
                        }
                        
                        if (0 == wsTempString.find(_T("file:///")))
                        {
                            string_replaceAll(wsTempString, _T("file:///"), _T(""));
                            string_replaceAll(wsTempString, _T("\\"), _T("/") );
                        }
                    }
                    
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    
                    try
                    {
                        DrawImageFromFile(wsTempString, m1, m2, m3, m4);
                    }
                    catch (...)
                    {
                        //ATLTRACE2 ("DrawImageFromFile failed!\n");
                    }
                    
                    if (!base64TempFile.IsEmpty())
                    {
                        ::CDirectory::DeleteFile(base64TempFile);
                    }
                }
                    break;
                case ctFontName:
                {
                    //ATLTRACE2 ("CommandType::ctFontName\n");
                    
                    _sLen = 2 * (int)NSCommonReader::ReadUSHORT(current, curindex);
                    
                    std::wstring wsTempString = NSCommonReader::ReadString16(current, curindex, _sLen);
                    
                    put_FontName (wsTempString);
                }
                    break;
                case ctFontSize:
                    //ATLTRACE2 ("CommandType::ctFontSize:\n");
                    
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    put_FontSize (m1);
                    break;
                case ctFontStyle:
                    //ATLTRACE2 ("CommandType::ctFontStyle:\n");
                    
                    put_FontStyle (NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctDrawText:
                {
                    _sLen = 2 * (int)NSCommonReader::ReadUSHORT(current, curindex);
                    
                    std::wstring wsTempString = NSCommonReader::ReadString16(current, curindex, _sLen);
                    
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    
                    CommandDrawText(wsTempString, m1, m2, 0, 0, 0);
                }
                    break;
                case ctBeginCommand:
                    //ATLTRACE2 ("CommandType::ctBeginCommand:\n");
                    if (bIsPathOpened)
                    {
                        PathCommandEnd();
                        EndCommand(4);
                        bIsPathOpened = false;
                    }
                    BeginCommand((DWORD)(NSCommonReader::ReadInt(current, curindex)));
                    break;
                case ctGradientFill:
                {
                    gradientType	=	NSCommonReader::ReadInt(current, curindex);
                    
                    std::wstring sXml, sXmlStop;
                    
                    if (0 == gradientType)	//	linearGradient
                    {
                        double x1	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double x2	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double y1	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double y2	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        
                        int stops	=	(_INT32)(*current);
                        current		+=	4;
                        curindex	+=	4;
                        
                        sXml = string_format(_T("<linearGradient x1='%f' x2='%f' y1='%f' y2='%f'>"), x1, x2, y1, y2);
                        
                        for (int i = 0; i < stops; ++i)
                        {
                            _INT32 color		=	static_cast<_INT32>(*current);
                            double opacity	=	static_cast<double>(static_cast<_INT32>(*(current + 1))) / 255.0;
                            double offset	=	static_cast<double>(static_cast<_INT32>(*(current + 2))) / 255.0;
                            
                            current			+=	6 * 4;		//	4 + 1 + 1
                            curindex		+=	6 * 4;
                            
                            sXmlStop= string_format(_T("<stop stop-color='%d' offset='%f' stop-opacity='%f' />"), color, offset);
                            sXml += sXmlStop;
                        }
                        
                        sXml += _T("</linearGradient>");
                        
                        if (stops)
                        {
                            CreateLinearGradientFromSvgXml(sXml, true);
                        }
                    }
                    else if (1 == gradientType)
                    {
                        double cx	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double cy	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double r	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double fx	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double fy	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        
                        int stops	=	NSCommonReader::ReadInt(current, curindex);
                        
                        std::wstring sXml= string_format(_T("<radialGradient cx='%f' cy='%f' r='%f' fx='%f' fy='%f' >"), cx, cy, r, fx, fy);
                        
                        for (int i = 0; i < stops; ++i)
                        {
                            _INT32 color		=	static_cast<_INT32>(*current);
                            double opacity	=	static_cast<double>(static_cast<_INT32>(*(current + 1))) / 255.0;
                            double offset	=	static_cast<double>(static_cast<_INT32>(*(current + 2))) / 255.0;
                            
                            current			+=	6 * 4;		//	4 + 1 + 1
                            curindex		+=	6 * 4;
                            
                            sXmlStop = string_format(_T("<stop stop-color='%d' offset='%f' stop-opacity='%f' />"), color, offset);
                            sXml += sXmlStop;
                        }
                        
                        sXml += _T("</radialGradient>"); 
                        
                        if (stops)
                        {
                            CreateRadialGradientFromSvgXml(sXml, true);
                        }
                    }
                }
                    break;
                case ctGradientFillXML:
                {
                    gradientType	=	NSCommonReader::ReadInt(current, curindex);
                    _sLen		=	(int)NSCommonReader::ReadInt(current, curindex);
                    
                    std::wstring wsTempString = NSCommonReader::ReadString16(current, curindex, _sLen);
                    
                    if (0 == gradientType)			//	linearGradient
                        CreateLinearGradientFromSvgXml(wsTempString, true);
                    else if (1 == gradientType)	//	radialGradient	
                        CreateRadialGradientFromSvgXml(wsTempString, true);
                }
                    break;		
                case ctGradientStroke:
                {
                    gradientType	=	NSCommonReader::ReadInt(current, curindex);
                    
                    CString sXml, sXmlStop;
                    
                    if (0 == gradientType)	//	linearGradient
                    {
                        double x1	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double x2	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double y1	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double y2	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        
                        int stops	=	NSCommonReader::ReadInt(current, curindex);
                        
                        sXml.Format(_T("<linearGradient x1='' x2='' y1='' y2=''>"), x1, x2, y1, y2);
                        
                        for (int i = 0; i < stops; ++i)
                        {
                            _INT32 color		=	static_cast<_INT32>(*current);
                            double opacity	=	static_cast<double>(static_cast<_INT32>(*(current + 1))) / 255.0;
                            double offset	=	static_cast<double>(static_cast<_INT32>(*(current + 2))) / 255.0;
                            
                            current			+=	6 * 4;		//	4 + 1 + 1
                            curindex		+=	6 * 4;
                            
                            sXmlStop.Format(_T("<stop stop-color='%d' offset='%f' stop-opacity='%f' />"), color, offset);
                            sXml += sXmlStop;
                        }
                        
                        sXml += _T("</linearGradient>"); 
                        
                        if (stops)
                        {
                            CreateLinearGradientFromSvgXml(string2std_string(sXml), false);
                        }
                    }
                    else if (1 == gradientType)
                    {
                        double cx	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double cy	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double r	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double fx	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        double fy	=	NSCommonReader::ReadInt(current, curindex) / 100000.0;
                        
                        int stops	=	NSCommonReader::ReadInt(current, curindex);
                        
                        sXml.Format(_T("<radialGradient cx='%f' cy='%f' r='%f' fx='%f' fy='%f' >"), cx, cy, r, fx, fy);
                        
                        for (int i = 0; i < stops; ++i)
                        {
                            _INT32 color		=	static_cast<_INT32>(*current);
                            double opacity	=	static_cast<double>(static_cast<_INT32>(*(current + 1))) / 255.0;
                            double offset	=	static_cast<double>(static_cast<_INT32>(*(current + 2))) / 255.0;
                            
                            current			+=	6 * 4;		//	4 + 1 + 1
                            curindex		+=	6 * 4;
                            
                            sXmlStop.Format(_T("<stop stop-color='%d' offset='%f' stop-opacity='%f' />"), color, offset);
                            sXml += sXmlStop;
                        }
                        
                        sXml += _T("</radialGradient>"); 
                        
                        if (stops)
                        {
                            CreateRadialGradientFromSvgXml(string2std_string(sXml), false);
                        }
                    }
                }
                    break;		
                case ctGradientStrokeXML:
					{
						gradientType	=	NSCommonReader::ReadInt(current, curindex);
						_sLen		=	(int)NSCommonReader::ReadInt(current, curindex);

						std::wstring wsTempString = NSCommonReader::ReadString16(current, curindex, _sLen);

						if (0 == gradientType)			//	linearGradient
                            CreateLinearGradientFromSvgXml(wsTempString, false);
						else if (1 == gradientType)		//	radialGradient
                            CreateRadialGradientFromSvgXml(wsTempString, false);
					}
                    break;							
                    
                case ctEndCommand:
                    //ATLTRACE2 ("CommandType::ctEndCommand:\n");
                    if (bIsPathOpened)
                    {
                        PathCommandEnd();
                        EndCommand(4);
                        bIsPathOpened = false;
                    }
                    EndCommand((DWORD)(NSCommonReader::ReadInt(current, curindex)));
                    PathCommandEnd();
                    break;
                default:
                    //ATLTRACE2 ("CommandType:DEFAULT!\n");
                    break;
            }; // switch (eCommand)
        } // while (curindex < len)
    }
}

HRESULT CPdfWriterLib::OnlineWordToPdf (std::wstring sPathXml, std::wstring sDstFile, LONG nReg)
{
	using namespace NOnlineOfficeBinToPdf;
	HRESULT hRes = S_OK;

    CString sTempLogo;

	std::wstring sHtmlPlace = FileSystem::Directory::GetFolderPath(std_string2string(sPathXml));

	try
	{
		hRes = CreatePDF();
        if (S_OK != (hRes))
			throw "CreatePDF failed!";

		hRes = SetPDFCompressionMode(15);
        if (S_OK != (hRes))
			throw "SetPDFCompressionMode(15) failed!";

		int nCountPages = 0;

        std::wstring sHypers =_T("<linker>");

		// read file
		CString sOpenPathName = FileSystem::Directory::GetLongPathName_(std_string2string(sPathXml));

		CFile oFile;

		if (oFile.OpenFile(sOpenPathName) != S_OK)
			return S_FALSE;

		ULONG64 dwFileSize = oFile.GetFileSize();
		
		BYTE *fileContent = new BYTE[dwFileSize];
		if (fileContent == NULL) 
		{
			oFile.CloseFile();
			return S_FALSE;
		}
		oFile.ReadFile(fileContent, dwFileSize);
		oFile.CloseFile();

		// here we can to divide file into some lines of text (divided by '\n' symbol)
		// -> std::vector<CStringA> asResFile;
		// -> for (size_t nLine = 0; nLine < asResFile.size(); ++nLine)

		int pos_end = 0; 
		int pos_start = 0 ;
		while(pos_end < dwFileSize)
		{
			if (fileContent[pos_end] == '\n')
			{
				long	line_length = pos_end - pos_start;
				LPCSTR	line_str	= (LPCSTR)(fileContent + pos_start);

				int		len			= Base64::Base64DecodeGetRequiredLength(line_length);
				BYTE *	dstArray	= new BYTE[len + 64];
				
				if (Base64::Base64Decode(line_str, line_length, dstArray, &len))
				{
					OnlineWordToPdfInternal(dstArray, len, sHtmlPlace, sHypers, nCountPages, sTempLogo, nReg);
					RELEASEARRAYOBJECTS(dstArray);
				}
				pos_start = pos_end+1;
			}
			pos_end++;
			//
		}
		if (pos_start < pos_end)
		{
			long	line_length = pos_end - pos_start;
			LPCSTR	line_str	= (LPCSTR)(fileContent + pos_start);

			int		len			= Base64::Base64DecodeGetRequiredLength(line_length);
			BYTE *	dstArray	= new BYTE[len + 64];
			
			if (Base64::Base64Decode(line_str, line_length, dstArray, &len))
			{
                OnlineWordToPdfInternal(dstArray, len, sHtmlPlace, sHypers, nCountPages, sTempLogo, nReg);
                RELEASEARRAYOBJECTS(dstArray);
			}
		}
		sHypers += _T("</linker>");

		if (!nReg)
		{
			BeginCommand(8);
            CommandDrawText(sHypers, 0, 0, 0, 0, 0);
			EndCommand(8);
		}

        if (sTempLogo != _T(""))
		{
            ::CDirectory::DeleteFile(sTempLogo);
		}

		SaveToFile(std_string2string(sDstFile));
	}
	catch (char *pcError)
	{
        //ATLTRACE2 (pcError);
	}
	//if (NULL != piWinFonts)
	//{
	//	piWinFonts->Release();
	//	piWinFonts = NULL;
	//}

	return hRes;
}

HRESULT CPdfWriterLib::DrawCurrentImage (float fX, float fY, float fWidth, float fHeight)
{
	if ( !m_pCurrentXObject )
		return AVS_OFFICEPDFWRITER_ERROR_CURRENT_IMAGE_NOT_FOUND;
	unsigned long nRet = PageDrawImage( m_pDocument->pCurPage, m_pCurrentXObject, fX, fY, fWidth, fHeight );
	if ( OK != nRet )
		return nRet;

	return S_OK;
}
void CPdfWriterLib::FreePDF()
{
	m_pCurrentFont      = NULL;
	m_pCurrentExtGState = NULL;
	m_pCurrentXObject   = NULL;

	if ( m_pOutlineList )
		ListFree2( m_pOutlineList );
	
	m_pOutlineList = NULL;
	m_pCurrentOutline = NULL;

	if ( m_pDestList )
		ListFree2( m_pDestList );
	m_pDestList = NULL;
	m_pCurrentDestination = NULL;

	if ( m_pAnnotList )
		ListFree2( m_pAnnotList );
	m_pAnnotList = NULL;
	m_pCurrentAnnotation = NULL;

	if ( m_pXObjectList )
		ListFree2( m_pXObjectList );
	m_pXObjectList = NULL;
	m_pCurrentXObject = NULL;

	if ( m_pFontEncodingList )
		ListFree( m_pFontEncodingList );
	m_pFontEncodingList = NULL;

	// Документ освобождаем в конце, т.к. все списки строились
	// через m_pDocument->oMMgr

	if ( m_pDocument )
		Free( m_pDocument );
	m_pDocument         = NULL;

	while( m_pCurCommand )
	{
		TCommandType *pPrev = m_pCurCommand->pPrev;
		delete m_pCurCommand;
		m_pCurCommand = pPrev;
	}

	RELEASEOBJECT(m_pCurPath);
}
TFontEncodingPair *CPdfWriterLib::FindFontEncodingPair( char* sFontName, char* sEncodingName/* = NULL */)
{
	if ( NULL == m_pFontEncodingList )
		return NULL;

	for ( int nIndex = 0; nIndex < m_pFontEncodingList->nCount; nIndex++ )
	{
		TFontEncodingPair *pPair = (TFontEncodingPair *)ListItemAt( m_pFontEncodingList, nIndex );
		if ( NULL == sEncodingName )
		{
			if ( 0 == UtilsStrCmp( pPair->sRealFontName, sFontName ) )
				return pPair;
		}
		else
		{
			if ( 0 == UtilsStrCmp( pPair->sRealFontName, sFontName ) && 0 == UtilsStrCmp( pPair->sEncodingName, sEncodingName ) )
				return pPair;
		}
	}
	return NULL;
}
inline static PDF::Matrix BuildFromSvgMatrix(CString sXml)
{
	if (sXml.GetLength())
	{		
		int st		=	0;
		int del		=	sXml.Tokenize(_T("("), st).GetLength();

		CString sTransform = sXml.Mid(del + 1, sXml.GetLength() - del - 2);
		if (sTransform.GetLength())
		{					
			std::vector<double> numbers;
			if (ParseUtils::DoubleValues(sTransform, numbers))
			{
				if (6 == numbers.size())
				{
					return PDF::Matrix(numbers[0], numbers[1], numbers[4], numbers[2], numbers[3], numbers[5], 0, 0, 1);								
				}
			}
		}
	}

	return PDF::Matrix();
}

bool CPdfWriterLib::ApplyFillGradient()
{		
	PDF::Rect pageBounds	=	PageBounds();		
	PDF::Rect pathBounds	=	PathBounds();

	PDF::XForm* form		=	NULL;
	int maskRefId			=	-1;
	CString formName		=	L"";
	CString stateName		=	L"";
	CString alphaExt		=	L"";

	PDF::Pattern* pattern = m_oPatternState.GetFill();
	if (pattern)
	{
        //ATLTRACE2(_T("ApplyFillGradient\n"));

		//	%	fill 

		//	/Pattern cs
		//	/P12345 scn

		CString sVal;
		sVal.Format(L"/P%d scn\012", pattern->GetRefIndex());

		if (PDF::ShadingPatternType == pattern->GetType())
		{
			// NOTE! прозрачность задается через сам градиент или маску 
			if (m_pCurrentExtGState)
			{
				ExtGState disableAlpha		=	CreateExtGState(m_pDocument);
				if (disableAlpha)
				{								
					disableAlpha->sBuffer	=	new CString(L"<< /Type /ExtGState\012	/CA 1\012	/ca 1\012>>");
					alphaExt				=	CString(PageGetExtGStateName(m_pDocument->pCurPage, disableAlpha));
				}
			}

			PDF::Shading* shading = (static_cast<PDF::ShadingPattern*>(pattern))->GetTopShading();
			if (shading)
			{
				shading->SetTransform(PDF::Matrix(m_oCommandParams.oMatrix.fA, m_oCommandParams.oMatrix.fC, MMToPDFCoords(m_oCommandParams.oMatrix.fE), 
					m_oCommandParams.oMatrix.fB, m_oCommandParams.oMatrix.fD, MMToPDFCoords(m_oCommandParams.oMatrix.fF), 
					0, 0, 1));

				shading->CalculateCoords(pathBounds, pageBounds.Height);
			}

			PDF::Pattern* alpha = pattern->GetAlphaPattern();
			if (alpha)
			{
				if (PDF::ShadingPatternType == alpha->GetType())
				{
					PDF::Shading* shading = (static_cast<PDF::ShadingPattern*>(alpha))->GetTopShading();
					if (shading)
					{
						shading->SetTransform(PDF::Matrix(m_oCommandParams.oMatrix.fA, m_oCommandParams.oMatrix.fC, MMToPDFCoords(m_oCommandParams.oMatrix.fE), 
							m_oCommandParams.oMatrix.fB, m_oCommandParams.oMatrix.fD, MMToPDFCoords(m_oCommandParams.oMatrix.fF), 
							0, 0, 1));

						shading->CalculateCoords(pathBounds, pageBounds.Height);

						PDF::PatternAlphaGroup* alpha	=	m_oPatterns.GetAlphaGroupTop();
						if (alpha)
						{
							form = new PDF::XForm();
							if (form)
							{
								ExtGState smask			=	CreateExtGState(m_pDocument);
								if (smask)
								{								
									CString str; str.Format(L"<< /Type /ExtGState\012   /SMask %d 0 R\012   /ca 1\012>>", alpha->GetMask()->GetId());
									smask->sBuffer		=	new CString(str);

									stateName			=	CString(PageGetExtGStateName(m_pDocument->pCurPage, smask));

									Proxy proxyForm		=	GenXRef<PDF::XForm>(form, OCLASS_XFORM);

									form->SetBound(pageBounds);

									alpha->SetFormId(form->GetId());
									alpha->GetMask()->SetStateId(smask->pHeader.nObjId & (~OTYPE_INDIRECT));

									m_oPatterns.AddXForm(form);

									XrefEntry entry		=	XrefGetEntry(m_pDocument->pXref, m_pDocument->pXref->pEntries->nCount - 1);
									if (entry)
									{
										XObject obj		=  (XObject)entry->pObj;
										if (obj)
										{
											formName	=	CString(PageGetXObjectName(m_pDocument->pCurPage, obj));
										}
									}

									RebuildResources();

									maskRefId			=	 alpha->GetMask()->GetStateId();
								}
							}
						}
					}
				}
			}
		}

		PageAttr attr				=	(PageAttr)m_pDocument->pCurPage->pAttr;
		if (attr)
		{
			m_strPatternFill		=	CString (L"/Pattern cs\012") + sVal; 
			if (alphaExt.GetLength())
			{
				m_strPatternFill	=	L"/" + alphaExt + L" gs \012" + m_strPatternFill;
			}

			StreamWriteStr(attr->pStream, CStringA(m_strPatternFill));
		}

		if (form)
		{
			// q /s13 gs /x14 Do Q
			// к x-N объекту применяется маска s-N

			m_MaskWithFormCommands.Format(L"q\012/%ls gs /%ls Do\012Q\012", stateName, formName);

			m_XForm		=	form;
		}

		m_oPatternState.SetFill(NULL);

        return true;
	}

    return false;
}
bool CPdfWriterLib::ApplyStrokeGradient()
{			
	PDF::Rect pageBounds	=	PageBounds();		
	PDF::Rect pathBounds	=	PathBounds();

	PDF::XForm* form		=	NULL;
	int maskRefId			=	-1;
	CString formName		=	L"";
	CString stateName		=	L"";
	CString alphaExt		=	L"";

	PDF::Pattern* pattern = m_oPatternState.GetStroke();
	if (pattern)
	{
        //ATLTRACE2(_T("ApplyStrokeGradient\n"));

		//	%	stroke 

		//	/Pattern CS
		//	/P12345 SCN

		CString sVal;
		sVal.Format(L"/P%d SCN\012", pattern->GetRefIndex());

		if (PDF::ShadingPatternType == pattern->GetType())
		{
			// NOTE! прозрачность задается через сам градиент или маску 
			if (m_pCurrentExtGState)
			{
				ExtGState disableAlpha		=	CreateExtGState(m_pDocument);
				if (disableAlpha)
				{								
					disableAlpha->sBuffer	=	new CString(L"<< /Type /ExtGState\012	/CA 1\012	/ca 1\012>>");
					alphaExt				=	CString(PageGetExtGStateName(m_pDocument->pCurPage, disableAlpha));
				}
			}


			PDF::Shading* shading = (static_cast<PDF::ShadingPattern*>(pattern))->GetTopShading();
			if (shading)
			{
				shading->SetTransform(PDF::Matrix(m_oCommandParams.oMatrix.fA, m_oCommandParams.oMatrix.fC, MMToPDFCoords(m_oCommandParams.oMatrix.fE), 
					m_oCommandParams.oMatrix.fB, m_oCommandParams.oMatrix.fD, MMToPDFCoords(m_oCommandParams.oMatrix.fF), 
					0, 0, 1));

				shading->CalculateCoords(pathBounds, pageBounds.Height);
			}

			PDF::Pattern* alpha = pattern->GetAlphaPattern();
			if (alpha)
			{
				if (PDF::ShadingPatternType == alpha->GetType())
				{
					PDF::Shading* shading = (static_cast<PDF::ShadingPattern*>(alpha))->GetTopShading();
					if (shading)
					{
						shading->SetTransform(PDF::Matrix(m_oCommandParams.oMatrix.fA, m_oCommandParams.oMatrix.fC, MMToPDFCoords(m_oCommandParams.oMatrix.fE), 
							m_oCommandParams.oMatrix.fB, m_oCommandParams.oMatrix.fD, MMToPDFCoords(m_oCommandParams.oMatrix.fF), 
							0, 0, 1));

						shading->CalculateCoords(pathBounds, pageBounds.Height);

						PDF::PatternAlphaGroup* alpha	=	m_oPatterns.GetAlphaGroupTop();
						if (alpha)
						{
							form = new PDF::XForm();
							if (form)
							{
								ExtGState smask			=	CreateExtGState(m_pDocument);
								if (smask)
								{								
									CString str; str.Format(L"<< /Type /ExtGState\012   /SMask %d 0 R\012   /ca 1\012>>", alpha->GetMask()->GetId());
									smask->sBuffer		=	new CString(str);

									stateName			=	CString(PageGetExtGStateName(m_pDocument->pCurPage, smask));

									Proxy proxyForm		=	GenXRef<PDF::XForm>(form, OCLASS_XFORM);

									form->SetBound(pageBounds);

									alpha->SetFormId(form->GetId());
									alpha->GetMask()->SetStateId(smask->pHeader.nObjId & (~OTYPE_INDIRECT));

									m_oPatterns.AddXForm(form);

									XrefEntry entry		=	XrefGetEntry(m_pDocument->pXref, m_pDocument->pXref->pEntries->nCount - 1);
									if (entry)
									{
										XObject obj		=  (XObject)entry->pObj;
										if (obj)
										{
											formName	=	CString(PageGetXObjectName(m_pDocument->pCurPage, obj));
										}
									}

									RebuildResources();

									maskRefId			=	 alpha->GetMask()->GetStateId();
								}
							}
						}
					}
				}
			}
		}

		PageAttr attr			=	(PageAttr)m_pDocument->pCurPage->pAttr;
		if (attr)
		{
			m_strPatternStroke	=	CString (L"/Pattern CS\012") + sVal; 
			if (alphaExt.GetLength())
			{
				m_strPatternFill	=	L"/" + alphaExt + L" gs \012" + m_strPatternStroke;
			}
			
			StreamWriteStr(attr->pStream, CStringA(m_strPatternStroke));
		}

		if (form)
		{
			// q /s13 gs /x14 Do Q
			// к x-N объекту применяется маска s-N

			m_MaskWithFormCommands.Format(L"q /%ls gs /%ls Do Q", stateName, formName);

			m_XForm	=	form;
		}

		m_oPatternState.SetStroke(NULL);

        return true;
	}

    return false;
}

bool CPdfWriterLib::CreateLinearGradientFromSvgXml(const std::wstring & sXml, bool fill)
{
    //ATLTRACE2(_T("CreateLinearGradientFromSvgXml\n"));

	m_oBrush.TexturePath	=	L"";
	m_oBrush.Type			=	1000;

	int FormId		=	-1;

	XrefEntry entry	=	XrefGetEntry(m_pDocument->pXref, m_pDocument->pXref->pEntries->nCount - 2);
	if (entry)
	{
		XObject obj	=	(XObject)entry->pObj;
		if (obj)
		{
			FormId	=	(obj->pHeader.nObjId & (~OTYPE_INDIRECT));
		}
	}

	XmlUtils::CXmlNode oXml;
    if (oXml.FromXmlString(std_string2string(sXml)))
	{
		PDF::ShadingPattern* alphaShading	=	NULL;

		PDF::ShadingPattern* shading		=	new PDF::ShadingPattern();
		if (shading)
		{
			PDF::AxialShading* axial		=	new PDF::AxialShading();
			if (axial)
			{
				// единицы измерения (координаты документа или нормализованые)

                bool userSpaceOnUse			=	(L"userSpaceOnUse" == oXml.GetAttribute(L"gradientUnits"));
                bool isSvgElement			=	(L"svg" == oXml.GetAttribute(L"sourceType"));

				// направление

				double x1					=	_wtof(oXml.GetAttributeOrValue(L"x1", L"0"));
				double y1					=	_wtof(oXml.GetAttributeOrValue(L"y1", L"0"));	
				double x2					=	_wtof(oXml.GetAttributeOrValue(L"x2", L"1"));
				double y2					=	_wtof(oXml.GetAttributeOrValue(L"y2", L"0"));

                if (false == isSvgElement)
				{
					if (userSpaceOnUse)
					{
						x1					=	MMToPDFCoords(x1);
						y1					=	MMToPDFCoords(y1);
						x2					=	MMToPDFCoords(x2);
						y2					=	MMToPDFCoords(y2);	
					}
				}
			
				axial->SetGradientVector(PDF::float4(x1, y1, x2, y2), !userSpaceOnUse);			

				// матрица

				PDF::Matrix matrix			=	BuildFromSvgMatrix(oXml.GetAttributeOrValue(L"gradientTransform"));
				axial->SetTransform(matrix);

				float width					=	0.0f;	GetWidth(&width);
				float height				=	0.0f;	GetHeight(&height);	

				GenXRef<PDF::ShadingPattern>(shading, OCLASS_PATTERN_REF);

				XmlUtils::CXmlNodes oNodes;
				if (oXml.GetNodes(L"stop", oNodes))
				{
					std::vector<PDF::ExponentialInterpolationFunction*> functions;

					std::vector<PDF::float3> colors;
					std::vector<float> offsets;
					std::vector<float> opacity;	

                    bool alphaPatternEnable = false;

					for (int i = 0; i < oNodes.GetCount(); ++i)
					{									
						XmlUtils::CXmlNode oNode;
						if(oNodes.GetAt(i, oNode))
						{
							CString sColor = _T("0");
							oNode.ReadAttributeBase(L"stop-color", sColor);
                            long color	= _wtoi(sColor.GetBuffer());
 
							CString sOff = _T("0");
							oNode.ReadAttributeBase(L"offset", sOff);
							float off	=	_wtof(sOff.GetBuffer())	;					
							
							CString sAlpha	= _T("0");
							oNode.ReadAttributeBase(L"stop-opacity", sAlpha);
                            float alpha = (float)((std::min)((std::max)(_wtof(sAlpha.GetBuffer()), 0.0), 1.0));
							
                            colors.push_back(PDF::float3((0xFF & color) / 255.0f, (0xFF & (color >> 8)) / 255.0f, (0xFF & (color >> 16)) / 255.0f));
                            offsets.push_back((std::min)((std::max)(off, 0.0f), 1.0f));
							opacity.push_back(alpha);
							
							if (fabs(alpha - 1.0) > DBL_EPSILON)
							{
                                alphaPatternEnable	=	true;
							}
						}
					}

                    if (colors.size())//  && false == isSvgElement)
					{
						if (offsets[0] > DBL_EPSILON)
						{
							PDF::float3 color	=	colors[0];
							float alpha			=	opacity[0];

							colors.insert	(colors.begin()	, color);
							offsets.insert	(offsets.begin(), 0.0f);
							opacity.insert	(opacity.begin(), alpha);
						}

						long count = offsets.size() - 1;

						if (fabs(offsets[count] - 1.0) > DBL_EPSILON)
						{
							PDF::float3 color	=	colors[count];
							float alpha			=	opacity[count];

							colors.push_back(color);
							offsets.push_back(1.0f);
							opacity.push_back(alpha);
						}
					}

					// простой градиент из двух цветов
					
					if (2 == colors.size() && 2 == offsets.size())
					{
						PDF::ExponentialInterpolationFunction* func = new PDF::ExponentialInterpolationFunction();
						if (func)
						{
							func->AddC(3, colors[0].f1, colors[0].f2, colors[0].f3);
							func->AddC(3, colors[1].f1, colors[1].f2, colors[1].f3);

							func->AddOffset(PDF::float2(offsets[0], offsets[1]));										
							func->AddDomain(PDF::float2(0.0f, 1.0f));	
							func->AddN(1);

							GenXRef<PDF::ExponentialInterpolationFunction>(func, OCLASS_FUNC_REF);

							axial->AddFunc(func);

							if (alphaPatternEnable)
							{
								alphaShading	=	new PDF::ShadingPattern();
								if (alphaShading)
								{
									PDF::AxialShading* alphaAxial	=	new PDF::AxialShading();
									if (alphaAxial)
									{	
										alphaAxial->SetColorSpace(L"DeviceGray");
										alphaAxial->SetGradientVector(PDF::float4(x1, y1, x2, y2), !userSpaceOnUse);			
										alphaAxial->SetTransform(matrix);	

										PDF::ExponentialInterpolationFunction* alphaFunc = new PDF::ExponentialInterpolationFunction();
										if (alphaFunc)
										{
											alphaFunc->AddC(1, opacity[0]);
											alphaFunc->AddC(1, opacity[1]);

											alphaFunc->AddOffset(PDF::float2(offsets[0], offsets[1]));										
											alphaFunc->AddDomain(PDF::float2(0.0f, 1.0f));	
											alphaFunc->AddN(1);

											GenXRef<PDF::ShadingPattern>(alphaShading, OCLASS_PATTERN_REF);
											GenXRef<PDF::ExponentialInterpolationFunction>(alphaFunc, OCLASS_FUNC_REF);

											alphaAxial->AddFunc(alphaFunc);
											alphaShading->AddShading(alphaAxial);
										}
									}
								}
							}
						}
					}
					else
					{
						// составной градиент из нескольких цветов

						PDF::StitchFunction* stitch = new PDF::StitchFunction();
						if (stitch)
						{
							stitch->AddDomain(PDF::float2(0.0f, 1.0f));		

							for (size_t i = 0; i < colors.size() - 1; ++i)
							{
								PDF::ExponentialInterpolationFunction* func = new PDF::ExponentialInterpolationFunction();
								if (func)
								{
									func->AddC(3, colors[i].f1, colors[i].f2, colors[i].f3);
									func->AddC(3, colors[i + 1].f1, colors[i + 1].f2, colors[i + 1].f3);

									func->AddOffset(PDF::float2(offsets[i], offsets[i + 1]));										
									func->AddDomain(PDF::float2(0.0f, 1.0f));	
									func->AddN(1);

									GenXRef<PDF::ExponentialInterpolationFunction>(func, OCLASS_FUNC_REF);

									axial->AddFunc(func);

									stitch->AddFunc(func);
									stitch->AddEncode(PDF::float2(0.0f, 1.0f));

									if (i >= 1)
									{
										stitch->AddBounds(offsets[i]);
									}
								}
							}

							if (stitch->IsValid())
							{
								GenXRef<PDF::StitchFunction>(stitch, OCLASS_FUNC_REF);
								axial->AddFunc(stitch);

								if (alphaPatternEnable)
								{
									alphaShading	=	new PDF::ShadingPattern();
									if (alphaShading)
									{
										PDF::AxialShading* alphaAxial	=	new PDF::AxialShading();
										if (alphaAxial)
										{	
											alphaAxial->SetColorSpace(L"DeviceGray");
											alphaAxial->SetGradientVector(PDF::float4(x1, y1, x2, y2), !userSpaceOnUse);			
											alphaAxial->SetTransform(matrix);

											PDF::StitchFunction* alphaStitch = new PDF::StitchFunction();
											if (alphaStitch)
											{
												alphaStitch->AddDomain(PDF::float2(0.0f, 1.0f));		

												int count = opacity.size();
												for (size_t i = 0; i < count - 1; ++i)
												{
													PDF::ExponentialInterpolationFunction* alphaFunc = new PDF::ExponentialInterpolationFunction();
													if (alphaFunc)
													{
														alphaFunc->AddC(1, opacity[i]);
														alphaFunc->AddC(1, opacity[(i + 1)]);

														alphaFunc->AddOffset(PDF::float2(offsets[i], offsets[i + 1]));										
														alphaFunc->AddDomain(PDF::float2(0.0f, 1.0f));	
														alphaFunc->AddN(1);

														GenXRef<PDF::ExponentialInterpolationFunction>(alphaFunc, OCLASS_FUNC_REF);

														alphaAxial->AddFunc(alphaFunc);

														alphaStitch->AddFunc(alphaFunc);
														alphaStitch->AddEncode(PDF::float2(0.0f, 1.0f));

														if (i >= 1)
														{
															alphaStitch->AddBounds(offsets[i]);
														}
													}
												}
											}
																					
											GenXRef<PDF::StitchFunction>(alphaStitch, OCLASS_FUNC_REF);
											alphaAxial->AddFunc(alphaStitch);

											GenXRef<PDF::ShadingPattern>(alphaShading, OCLASS_PATTERN_REF);
											alphaShading->AddShading(alphaAxial);
										}
									}
								}
							}
							else
							{
								RELEASEOBJECT(stitch);
							}
						}
					}
				}

				shading->AddShading(axial);

				if (fill)
					m_oPatternState.SetFill(shading);						
				else
					m_oPatternState.SetStroke(shading);						

				m_oPatterns.Add(shading);
				
				if (alphaShading)
				{
					shading->SetAlphaPattern(alphaShading);

					// как маска будет отрисована ( паттерно в ч.б цветовом пространстве )

					PDF::PatternAlphaGroup* mask = new PDF::PatternAlphaGroup();
					if (mask)
					{						
						mask->SetFillId(alphaShading->GetId());
						mask->SetBound(PDF::Rect(0, 0, width, height));
						mask->SetFormId(FormId);

						GenXRef<PDF::PatternAlphaGroup>(mask, OCLASS_MASK_FORM);

						// определение маски
						
						if (mask->GetMask())
						{				
							GenXRef<PDF::SoftMask>(mask->GetMask(), OCLASS_MASK_REF);
							mask->GetMask()->SetGroupId(mask->GetId());
						}

						m_oPatterns.AddMask(mask);
					}

					m_oPatterns.Add(alphaShading);
				}

				RebuildResources();

                return true;
			}
		}
	}

    return false;
}
bool CPdfWriterLib::CreateRadialGradientFromSvgXml(const std::wstring & sXml, bool fill)
{
    //ATLTRACE2(_T("CreateRadialGradientFromSvgXml\n"));

	m_oBrush.TexturePath	=	L"";
	m_oBrush.Type			=	1000;

	int FormId		=	-1;

	XrefEntry entry	=	XrefGetEntry(m_pDocument->pXref, m_pDocument->pXref->pEntries->nCount - 2);
	if (entry)
	{
		XObject obj	=	(XObject)entry->pObj;
		if (obj)
		{
			FormId	=	(obj->pHeader.nObjId & (~OTYPE_INDIRECT));
		}
	}

	XmlUtils::CXmlNode oXml;
    if (oXml.FromXmlString(std_string2string(sXml)))
	{
		PDF::ShadingPattern* alphaShading	=	NULL;

		PDF::ShadingPattern* shading	=	new PDF::ShadingPattern();
		if (shading)
		{
			PDF::RadialShading* radial	=	new PDF::RadialShading();
			if (radial)
			{
				// единицы измерения (координаты документа или нормализованые)

                bool userSpaceOnUse		=	(L"userSpaceOnUse" == oXml.GetAttribute(L"gradientUnits"));
                bool isSvgElement		=	(L"svg" == oXml.GetAttribute(L"sourceType"));

				// направление

				double cx				=	_wtof(oXml.GetAttributeOrValue(L"cx", L"0"));
				double cy				=	_wtof(oXml.GetAttributeOrValue(L"cy", L"0"));	
				double r				=	_wtof(oXml.GetAttributeOrValue(L"r",  L"0"));
				double r0				=	_wtof(oXml.GetAttributeOrValue(L"r1", L"0"));
				double r1				=	_wtof(oXml.GetAttributeOrValue(L"r0", L"0"));
				double fx				=	_wtof(oXml.GetAttributeOrValue(L"fx", L"0"));
				double fy				=	_wtof(oXml.GetAttributeOrValue(L"fy", L"0"));

				if (oXml.GetAttribute(L"rx").GetLength())
					fx					=	_wtof(oXml.GetAttributeOrValue(L"rx", L"0"));
				if (oXml.GetAttribute(L"ry").GetLength())
					fy					=	_wtof(oXml.GetAttributeOrValue(L"ry", L"0"));

                if (false == isSvgElement)
				{
					if (userSpaceOnUse)
					{
						cx				=	MMToPDFCoords(cx);
						cy				=	MMToPDFCoords(cy);
						r				=	MMToPDFCoords(r);
						r0				=	MMToPDFCoords(r0);
						r1				=	MMToPDFCoords(r1);
						fx				=	MMToPDFCoords(fx);	
						fy				=	MMToPDFCoords(fy);	
					}

					if (oXml.GetAttribute(L"r0").GetLength() && oXml.GetAttribute(L"r1").GetLength()) 
					{
						if (fabs(r0 - r1) < 0.0001)
							r1 = r0 + 0.0001;

						radial->SetGradientCircles(PDF::float2(cx, cy), PDF::float2(fx, fy), PDF::float2(r1 , r0),!userSpaceOnUse);			
					}
					else
					{
						radial->SetGradientCircles(PDF::float2(cx, cy), PDF::float2(fx, fy), PDF::float2(r * 0.5, r * 0.5 * 2),!userSpaceOnUse);			
					}
				}
				else
				{
					radial->SetGradientCircles(PDF::float2(cx, cy), PDF::float2(fx, fy), PDF::float2(0, r),!userSpaceOnUse);			
				}

				// матрица

				PDF::Matrix matrix		=	BuildFromSvgMatrix(oXml.GetAttributeOrValue(L"gradientTransform"));
				radial->SetTransform(matrix);
	
				float width				=	0.0f;	GetWidth(&width);
				float height			=	0.0f;	GetHeight(&height);	

				GenXRef<PDF::ShadingPattern>(shading, OCLASS_PATTERN_REF);

				XmlUtils::CXmlNodes oNodes;
				if (oXml.GetNodes(L"stop", oNodes))
				{
					std::vector<PDF::ExponentialInterpolationFunction*> functions;

					std::vector<PDF::float3> colors;
					std::vector<float> offsets;
					std::vector<float> opacity;								

                    bool alphaPatternEnable = false;

					for (int i = 0; i < oNodes.GetCount(); ++i)
					{									
						XmlUtils::CXmlNode oNode;
						if (oNodes.GetAt(i, oNode))
						{
							CString sColor = _T("0");
							oNode.ReadAttributeBase(L"stop-color", sColor);
                            long color	= _wtoi(sColor.GetBuffer());
 
							CString sOff = _T("0");
							oNode.ReadAttributeBase(L"offset", sOff);
							float offset	=_wtof(sOff.GetBuffer());						
							
							CString sAlpha	= _T("0");
							oNode.ReadAttributeBase(L"stop-opacity", sAlpha);
                            float alpha = (float)((std::min)((std::max)(_wtof(sAlpha.GetBuffer()), 0.0), 1.0));


							// если есть несколько нулевых смещение идущих друг за другом, отсечем их как не нужные

							if (offset < DBL_EPSILON && colors.size())
							{
								colors.clear();
								offsets.clear();
								opacity.clear();
							}
							CString sOffset = L"0";
							oNode.ReadAttributeBase(L"offset", sOffset);
							float fVal = _wtof(sOffset.GetBuffer());
						
							colors.push_back(PDF::float3((0xFF & color) / 255.0f, (0xFF & (color >> 8)) / 255.0f, (0xFF & (color >> 16)) / 255.0f));
                            offsets.push_back((std::min)((std::max)(fVal, 0.0f), 1.0f));
							opacity.push_back(alpha);

							if (fabs(alpha - 1.0) > DBL_EPSILON)
							{
                                alphaPatternEnable	=	true;
							}
						}
					}

					if (colors.size())
					{
						if (offsets[0] > DBL_EPSILON)
						{
							PDF::float3 color	=	colors[0];
							float alpha			=	opacity[0];

							colors.insert	(colors.begin()	, color);
							offsets.insert	(offsets.begin(), 0.0f);
							opacity.insert	(opacity.begin(), alpha);
						}

						long count = offsets.size() - 1;

						if (fabs(offsets[count] - 1.0) > DBL_EPSILON)
						{
							PDF::float3 color	=	colors[count];
							float alpha			=	opacity[count];

							colors.push_back(color);
							offsets.push_back(1.0f);
							opacity.push_back(alpha);
						}
					}

					// простой градиент из двух цветов
					
					if (2 == colors.size() && 2 == offsets.size())
					{
						PDF::ExponentialInterpolationFunction* func = new PDF::ExponentialInterpolationFunction();
						if (func)
						{
							func->AddC(3, colors[0].f1, colors[0].f2, colors[0].f3);
							func->AddC(3, colors[1].f1, colors[1].f2, colors[1].f3);

							func->AddOffset(PDF::float2(offsets[0], offsets[1]));										
							func->AddDomain(PDF::float2(0.0f, 1.0f));	
							func->AddN(1);

							GenXRef<PDF::ExponentialInterpolationFunction>(func, OCLASS_FUNC_REF);

							radial->AddFunc(func);
						}

						if (alphaPatternEnable)
						{
							alphaShading	=	new PDF::ShadingPattern();
							if (alphaShading)
							{
								PDF::RadialShading* alphaRadial	= new PDF::RadialShading();
								if (alphaRadial)
								{	
									alphaRadial->SetColorSpace(L"DeviceGray");
									alphaRadial->SetGradientCircles(radial->GetC0(), radial->GetC1(), radial->GetR(), !userSpaceOnUse);			
									alphaRadial->SetTransform(matrix);	

									PDF::ExponentialInterpolationFunction* alphaFunc = new PDF::ExponentialInterpolationFunction();
									if (alphaFunc)
									{
										alphaFunc->AddC(1, offsets[0]);
										alphaFunc->AddC(1, offsets[1]);

										alphaFunc->AddOffset(PDF::float2(offsets[0], offsets[1]));										
										alphaFunc->AddDomain(PDF::float2(0.0f, 1.0f));	
										alphaFunc->AddN(1);

										GenXRef<PDF::ShadingPattern>(alphaShading, OCLASS_PATTERN_REF);
										GenXRef<PDF::ExponentialInterpolationFunction>(alphaFunc, OCLASS_FUNC_REF);

										alphaRadial->AddFunc(alphaFunc);
										alphaShading->AddShading(alphaRadial);
									}
								}
							}
						}
					}
					else
					{
						// составной градиент из нескольких цветов
						PDF::StitchFunction* stitch = new PDF::StitchFunction();
						if (stitch)
						{
							stitch->AddDomain(PDF::float2(0.0f, 1.0f));	

							for (size_t i = 0; i < colors.size() - 1; ++i)
							{
								PDF::ExponentialInterpolationFunction* func = new PDF::ExponentialInterpolationFunction();
								if (func)
								{
									func->AddC(3, colors[i].f1, colors[i].f2, colors[i].f3);
									func->AddC(3, colors[i + 1].f1, colors[i + 1].f2, colors[i + 1].f3);

									func->AddOffset(PDF::float2(offsets[i], offsets[i + 1]));										
									func->AddDomain(PDF::float2(0.0f, 1.0f));	
									func->AddN(1);

									GenXRef<PDF::ExponentialInterpolationFunction>(func, OCLASS_FUNC_REF);
								
									radial->AddFunc(func);

									stitch->AddFunc(func);
									stitch->AddEncode(PDF::float2(0.0f, 1.0f));

									if (i >= 1)
									{
										stitch->AddBounds(offsets[i]);
									}
								}
							}

							if (stitch->IsValid())
							{
								GenXRef<PDF::StitchFunction>(stitch, OCLASS_FUNC_REF);
								radial->AddFunc(stitch);

								if (alphaPatternEnable)
								{
									alphaShading	=	new PDF::ShadingPattern();
									if (alphaShading)
									{
										PDF::RadialShading* alphaRadial	= new PDF::RadialShading();
										if (alphaRadial)
										{	
											alphaRadial->SetColorSpace(L"DeviceGray");
											alphaRadial->SetGradientCircles(radial->GetC0(), radial->GetC1(), radial->GetR(), !userSpaceOnUse);			
											alphaRadial->SetTransform(matrix);

											PDF::StitchFunction* alphaStitch = new PDF::StitchFunction();
											if (alphaStitch)
											{
												alphaStitch->AddDomain(PDF::float2(0.0f, 1.0f));		

												int count = opacity.size();
												for (size_t i = 0; i < count - 1; ++i)
												{
													PDF::ExponentialInterpolationFunction* alphaFunc = new PDF::ExponentialInterpolationFunction();
													if (alphaFunc)
													{
														alphaFunc->AddC(1, opacity[i]);
														alphaFunc->AddC(1, opacity[(i + 1)]);

														alphaFunc->AddOffset(PDF::float2(offsets[i], offsets[i + 1]));										
														alphaFunc->AddDomain(PDF::float2(0.0f, 1.0f));	
														alphaFunc->AddN(1);

														GenXRef<PDF::ExponentialInterpolationFunction>(alphaFunc, OCLASS_FUNC_REF);

														alphaRadial->AddFunc(alphaFunc);

														alphaStitch->AddFunc(alphaFunc);
														alphaStitch->AddEncode(PDF::float2(0.0f, 1.0f));

														if (i >= 1)
														{
															alphaStitch->AddBounds(offsets[i]);
														}
													}
												}
											}
																					
											GenXRef<PDF::StitchFunction>(alphaStitch, OCLASS_FUNC_REF);
											alphaRadial->AddFunc(alphaStitch);

											GenXRef<PDF::ShadingPattern>(alphaShading, OCLASS_PATTERN_REF);
											alphaShading->AddShading(alphaRadial);
										}
									}
								}
							}
							else
							{
								RELEASEOBJECT(stitch);
							}
						}
					}
				}

				shading->AddShading(radial);

				if (fill)
					m_oPatternState.SetFill(shading);						
				else
					m_oPatternState.SetStroke(shading);						

				m_oPatterns.Add(shading);

				if (alphaShading)
				{
					shading->SetAlphaPattern(alphaShading);

					// как маска будет отрисована ( паттерно в ч.б цветовом пространстве )

					PDF::PatternAlphaGroup* mask = new PDF::PatternAlphaGroup();
					if (mask)
					{						
						mask->SetFillId(alphaShading->GetId());
						mask->SetBound(PDF::Rect(0, 0, width, height));
						mask->SetFormId(FormId);

						GenXRef<PDF::PatternAlphaGroup>(mask, OCLASS_MASK_FORM);

						// определение маски
						
						if (mask->GetMask())
						{				
							GenXRef<PDF::SoftMask>(mask->GetMask(), OCLASS_MASK_REF);
							mask->GetMask()->SetGroupId(mask->GetId());
						}

						m_oPatterns.AddMask(mask);
					}

					m_oPatterns.Add(alphaShading);
				}

				RebuildResources();

                return true;
			}
		}
	}

    return false;
}

bool CPdfWriterLib::RebuildResources()
{
	Dict element			=	(Dict)(GetElement(m_pDocument->pCurPage, "Resources")->pValue);
	if (element)
	{
		Dict patternDict	=	DictNew(m_pDocument->oMMgr);
		if (patternDict)
		{

			DictAdd(element, "Pattern", patternDict);

			RELEASEOBJECT(patternDict->sBuffer);
			patternDict->sBuffer = new CString(m_oPatterns.Defines());

            return true;
		}
	}

    return false;
}

template <typename T> Proxy CPdfWriterLib::GenXRef(T* object, int Type)
{
	Proxy ref = ProxyNew(m_pDocument->oMMgr, object);
	if (ref)
	{
		ref->pHeader.nObjClass = Type;	
		XrefAdd(m_pDocument->pXref, ref);
		object->SetId(ref->pHeader.nObjId & (~OTYPE_INDIRECT));	
	}

	return ref;
}

PDF::Rect CPdfWriterLib::PathBounds()
{
	PDF::Rect rect;
	
	if (m_pCurPath)
	{
		double x = rect.X, y = rect.Y, w = rect.Width, h = rect.Height;
		m_pCurPath->GetBounds(x, y, w, h);

		rect.X		=	MMToPDFCoords(x);
		rect.Y		=	MMToPDFCoords(y);
		rect.Width	=	MMToPDFCoords(w);
		rect.Height	=	MMToPDFCoords(h);
	}

	if (m_bUseTextureRect)
	{
		rect.X		=	MMToPDFCoords(m_oBrush.Rect.X);
		rect.Y		=	MMToPDFCoords(m_oBrush.Rect.Y);
		rect.Width	=	MMToPDFCoords(m_oBrush.Rect.Width) + rect.X * 2;
		rect.Height	=	MMToPDFCoords(m_oBrush.Rect.Height)+ rect.Y * 2;

		rect.X		=	0;
		rect.Y		=	0;
	}

	return rect;
}
PDF::Rect CPdfWriterLib::PageBounds()
{
	PDF::Rect rect;
	
	GetHeight(&rect.Height);	
	GetWidth(&rect.Width);	

	return rect;
}
unsigned long CPdfWriterLib::FontFromFileToPdfFont()
{
	unsigned long nRet = OK;

	if ( _T("") != m_oFont.Path )
	{
		std::wstring wsFontName, wsEncodingName, wsToUnicodeName;
		
		if ( m_oFontLoader.Find( m_oFont.Path, &wsFontName, &wsEncodingName, &wsToUnicodeName ) )
		{
			if ( wsFontName.length() <= 0 )
			{
                nRet = SetCurrentFont( _T("Helvetica"), _T(""), false, _T("") );

				return nRet;
			}

            nRet = SetCurrentFont( std_string2string(wsFontName), std_string2string(wsEncodingName), false, std_string2string(wsToUnicodeName) );

			return nRet;
		}

        CString sExt;
        int res = m_oFont.Path.rfind(_T("."));
        if (res >= 0) sExt = std_string2string(m_oFont.Path.substr(res));
        sExt.MakeLower();

        int res2 = (std::max)(m_oFont.Path.rfind(_T("\\")), m_oFont.Path.rfind(_T("/")));
        if (res2 >= 0)wsFontName = m_oFont.Path.substr(res2 + 1, res>=0 ? (res-res2) : m_oFont.Path.length()-res2-1);

        CString bsPDFFontName;

        bool bEmb = true, bCIDfont = false;
		if ( _T(".non") == sExt )
            bEmb = false;
		else if ( _T(".cid_non") == sExt )
		{
            bEmb = false;
            bCIDfont = true;
		}
		else if ( _T(".n022003l") == sExt )
			bsPDFFontName = CString( _T(FONT_COURIER) );
		else if ( _T(".n022004l") == sExt )
			bsPDFFontName = CString( _T(FONT_COURIER_BOLD) );
		else if ( _T(".n022024l") == sExt )
			bsPDFFontName = CString( _T(FONT_COURIER_BOLD_OBLIQUE) );
		else if ( _T(".n022023l") == sExt )
			bsPDFFontName = CString( _T(FONT_COURIER_OBLIQUE) );
		else if ( _T(".n019003l") == sExt )
			bsPDFFontName = CString( _T(FONT_HELVETICA) );
		else if ( _T(".n019004l") == sExt )
			bsPDFFontName = CString( _T(FONT_HELVETICA_BOLD) );
		else if ( _T(".n019024l") == sExt )
			bsPDFFontName = CString( _T(FONT_HELVETICA_BOLD_OBLIQUE) );
		else if ( _T(".n019023l") == sExt )
			bsPDFFontName = CString( _T(FONT_HELVETICA_OBLIQUE) );
		else if ( _T(".s050000l") == sExt )
			bsPDFFontName = CString( _T(FONT_SYMBOL) );
		else if ( _T(".n021004l") == sExt )
			bsPDFFontName = CString( _T(FONT_TIMES_BOLD) );
		else if ( _T(".n021024l") == sExt )
			bsPDFFontName = CString( _T(FONT_TIMES_BOLD_ITALIC) );
		else if ( _T(".n021023l") == sExt )
			bsPDFFontName = CString( _T(FONT_TIMES_ITALIC) );
		else if ( _T(".n021003l") == sExt )
			bsPDFFontName = CString( _T(FONT_TIMES_ROMAN) );
		else if ( _T(".d050000l") == sExt )
			bsPDFFontName = CString( _T(FONT_ZAPF_DINGBATS) );
		else if ( _T(".pfb") == sExt || _T(".pfa") == sExt || _T(".pfb_t1") == sExt || _T(".pfb_t1c") == sExt || _T(".pfb_t1cot") == sExt )
		{
			// Значит шрифт не один из 14 стандартных

			CStringW wsPFBpath(m_oFont.Path.c_str());
			
			// Ищем файл *.afm, с тем же именем, что и *.pfb
            CStringW wsAFMpath;
            int res = wsPFBpath.ReverseFind(_T('.'));

            if (res >=0)
                wsAFMpath = wsPFBpath.Mid(0,res);
            else
                wsAFMpath = wsPFBpath;

            wsAFMpath += CString( _T(".afm") );

			unsigned long nRes = LoadT1FFromFile( wsAFMpath, wsPFBpath, &bsPDFFontName );

			if ( OK != nRes )
			{
				ResetError( m_pDocument );
                SetCurrentFont( _T(FONT_HELVETICA), _T("CP1251"), false, _T("") );
				return AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND;
			}
		}
		else if ( _T(".ttf") == sExt || _T(".otf") == sExt )
		{
			CString sTTFpath(m_oFont.Path.c_str());

            unsigned long nRes = LoadTTFFromFile( sTTFpath, true, _T(""), &bsPDFFontName );

			if ( OK != nRes )
			{
				ResetError( m_pDocument );
                SetCurrentFont( _T(FONT_HELVETICA), _T("CP1251"), false, _T("") );
				return AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND;
			}
		}
		else if ( _T(".cid_0") == sExt || _T(".cid_0c") == sExt || _T(".cid_0cot") == sExt || _T(".cid_2") == sExt || _T(".cid_2ot") == sExt )
		{
            bCIDfont = true;
		}
		else
		{
			ResetError( m_pDocument );
            SetCurrentFont( _T(FONT_HELVETICA), _T("CP1251"), false, _T("") );
			return AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND;
		}

		// Шрифт загружен, нужно установить кодировку, которая должна лежать рядом с файлом, с таким же названием и расширением ".enc"
        CString wsEncodingPath;

        int res1 = m_oFont.Path.rfind(_T("."));

        if (res1 >= 0 ) wsEncodingPath = std_string2string(m_oFont.Path.substr(0,res1));
        else            wsEncodingPath = std_string2string(m_oFont.Path);

        wsEncodingPath += _T(".enc");

		if ( !FileExist( wsEncodingPath ) )
            return SetCurrentFont( bsPDFFontName, _T(""), false, _T("") );

		XmlUtils::CXmlNode oMainNode;
		oMainNode.FromXmlFile( wsEncodingPath );
		if ( _T("PDF-resources") == oMainNode.GetName() )
		{
			if ( !bCIDfont )
			{
				CString wsEncodingName, wsToUnicodeName;

				XmlUtils::CXmlNode oNode;

				if ( oMainNode.GetNode( _T("FontBase"), oNode ) )
				{
					CString sValue = oNode.GetAttribute( _T("value"), _T("Helvetica") );
					if ( !bEmb )
					{
                        bsPDFFontName =  sValue;
					}
					else if ( _T("") == CString( bsPDFFontName ) )
					{
                        bsPDFFontName =  sValue;
					}
				}

				if ( oMainNode.GetNode( _T("Encoding"), oNode ) )
				{
					CString sValue;
					sValue = oNode.GetAttribute( _T("type"), _T("-1") );
					int nEncodingType = XmlUtils::GetInteger( sValue );
					sValue = oNode.GetAttribute( _T("base"), _T("-1") );
					int nBaseEncoding = XmlUtils::GetInteger( sValue );

					if ( -1 != nEncodingType )
					{
						char *sBaseName;
						switch ( nBaseEncoding )
						{
						case 0:  sBaseName = ENCODING_MAC_ROMAN;     break;
						case 1:  sBaseName = ENCODING_MAC_EXPERT;    break;
						case 2:  sBaseName = ENCODING_WIN_ANSI;      break;
						default: sBaseName = ENCODING_FONT_SPECIFIC; break;
						}

						EncoderRecPtr pEncoding = BasicEncoderNew( m_pDocument->oMMgr, sBaseName );

						const char *sName = NULL;
				#if defined(_WIN32) || defined (_WIN64)
						USES_CONVERSION;
						sName = W2A( wsFontName.c_str() );
				#else
						std::string ansiFontName(wsFontName.begin(), wsFontName.end());
						sName = ansiFontName.c_str();
				#endif
						strcpy( pEncoding->sName, sName );

						if ( 1 == nEncodingType )
						{
							BasicEncoderAttr pAttr = (BasicEncoderAttr)pEncoding->pAttr;
                            pAttr->bHasDifferences = true;

							XmlUtils::CXmlNode oDiff;
							if( oNode.GetNode( _T("Differences"), oDiff ) )
							{
								int nCount = XmlUtils::GetInteger( oDiff.GetAttribute( _T("count"), _T("0") ) );
								XmlUtils::CXmlNodes oDiffNodes;
								oDiff.GetNodes( _T("*"), oDiffNodes );

								int nCode = 0;
								for ( int nIndex = 0; nIndex < nCount; nIndex++ )
								{
									XmlUtils::CXmlNode oCurNode;
									oDiffNodes.GetAt( nIndex, oCurNode );
									if ( _T("Code") == oCurNode.GetName() )
									{
										nCode = XmlUtils::GetInteger( oCurNode.GetAttribute( _T("value"), _T("0") ) );
									}
									else if ( _T("Name") == oCurNode.GetName() )
									{
										if ( nCode < BASIC_ENCODER_MAX_CHAR_INDEX )
										{
											pAttr->anDifferences[nCode] = 1;

                                            CString wsName = oCurNode.GetAttribute( _T("value"), _T(".notdef") ) ;
                                            char * sName = NULL;
#ifdef defined(_WIN32) || defined(_WIN64)
                                            sName = W2A( wsName );
#else
                                            std::wstring ws = string2std_string(wsName);
                                            std::string s(ws.begin(), ws.end());
                                            sName = (char*)s.c_str();
#endif
											pAttr->anUnicodeMap[nCode]  = GlyphNameToUnicode( sName ); 
											strcpy( (char *)pAttr->asGlyphNameMap[nCode], sName );
											nCode++;
										}
									}
								}
							}
						}

						unsigned long nRes = DocRegisterEncoder( m_pDocument, pEncoding );
						if ( AVS_OFFICEPDFWRITER_ERROR_DUPLICATE_REGISTRATION == nRes )
							ResetError( m_pDocument );
						else if ( OK != nRes )
							return nRes;

                        wsEncodingName = std_string2string(wsFontName);
					}
				}

				if ( oMainNode.GetNode( _T("ToUnicode"), oNode ) ) 
				{
					CStringA sValue =  oNode.GetText();

					int	sz_out = Base64::Base64DecodeGetRequiredLength(sValue.GetLength());
					BYTE*  buf_out = new BYTE[sz_out + 64];

					if ((buf_out) && (Base64::Base64Decode(sValue.GetBuffer(),sValue.GetLength(), buf_out, &sz_out)))
					{
						PToUnicode pToUnicode = GetToUnicode( m_pDocument, buf_out, sz_out );
						wsToUnicodeName = CStringW( pToUnicode->sName );
						
						delete []buf_out;
					}					
				}

				if ( _T("") != bsPDFFontName )
					m_oFontLoader.Add( m_oFont.Path, string2std_string(bsPDFFontName), string2std_string(wsEncodingName), 
													string2std_string(wsToUnicodeName) );
				else
					m_oFontLoader.Add( m_oFont.Path, _T(""), string2std_string(wsEncodingName),string2std_string(wsToUnicodeName) );


                nRet = SetCurrentFont( bsPDFFontName, wsEncodingName, false, wsToUnicodeName );


				if ( OK != nRet )
					return nRet;

				if ( oMainNode.GetNode( _T("FontDescriptor"), oNode ) )
				{
					FontAttr pFontAttr = (FontAttr)m_pCurrentFont->pAttr;

					XmlUtils::CXmlNode oCurNode;
					if ( oNode.GetNode( _T("Ascent"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nAscent = XmlUtils::GetInteger( sValue );
					}
					if ( oNode.GetNode( _T("Descent"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nDescent = XmlUtils::GetInteger( sValue );
					}
					if ( oNode.GetNode( _T("Flags"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nFlags = XmlUtils::GetInteger( sValue );
					}
					if ( oNode.GetNode( _T("FontBBox"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value0") );
						short nValue0 = XmlUtils::GetInteger( sValue );
						sValue = oCurNode.GetAttribute( _T("value1") );
						short nValue1 = XmlUtils::GetInteger( sValue );
						sValue = oCurNode.GetAttribute( _T("value2") );
						short nValue2 = XmlUtils::GetInteger( sValue );
						sValue = oCurNode.GetAttribute( _T("value3") );
						short nValue3 = XmlUtils::GetInteger( sValue );
						pFontAttr->pFontDef->oFontBox = ToBox( nValue0, nValue1, nValue2, nValue3 );
					}
					if ( oNode.GetNode( _T("ItalicAngle"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nItalicAngle = XmlUtils::GetInteger( sValue );
					}
					if ( oNode.GetNode( _T("StemV"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nStemV = XmlUtils::GetInteger( sValue );
					}
					if ( oNode.GetNode( _T("AvgWidth"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nAvgWidth = XmlUtils::GetInteger( sValue );
					}
					if ( oNode.GetNode( _T("MaxWidth"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nMaxWidth = XmlUtils::GetInteger( sValue );
					}
					if ( oNode.GetNode( _T("MissingWidth"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nMissingWidth = XmlUtils::GetInteger( sValue );
					}
					if ( oNode.GetNode( _T("StemH"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nStemH = XmlUtils::GetInteger( sValue );
					}
					if ( oNode.GetNode( _T("XHeight"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nXHeight = XmlUtils::GetInteger( sValue );
					}
					if ( oNode.GetNode( _T("CapHeight"), oCurNode ) )
					{
						CString sValue = oCurNode.GetAttribute( _T("value") );
						pFontAttr->pFontDef->nCapHeight = XmlUtils::GetInteger( sValue );
					}
				}
				if ( oMainNode.GetNode( _T("Widths"), oNode ) )
				{
					FontAttr pFontAttr = (FontAttr)m_pCurrentFont->pAttr;

					pFontAttr->oExternWidths.bUse = true;

					CString sValue;
					sValue = oNode.GetAttribute( _T("FirstChar"), _T("0") );
					pFontAttr->oExternWidths.nFirstChar = XmlUtils::GetInteger( sValue );

					sValue = oNode.GetAttribute( _T("LastChar"), _T("0") );
					pFontAttr->oExternWidths.nLastChar = XmlUtils::GetInteger( sValue );

					XmlUtils::CXmlNodes oWidthNodes;
					oNode.GetNodes( _T("Width"), oWidthNodes );
					pFontAttr->oExternWidths.anWidths = (short *)GetMem( m_pDocument->oMMgr, sizeof(short) * oWidthNodes.GetCount()  );
					for ( int nIndex = 0; nIndex < oWidthNodes.GetCount(); nIndex++ )
					{
						XmlUtils::CXmlNode oWidthNode;
						oWidthNodes.GetAt( nIndex, oWidthNode );
						sValue = oWidthNode.GetAttribute( _T("value") );
						pFontAttr->oExternWidths.anWidths[nIndex] = XmlUtils::GetInteger( sValue );
					}
				}

				return nRet;
			}
			else //if ( bCIDfont )
			{
				CString wsEncodingName, wsToUnicodeName;
				XmlUtils::CXmlNode oType0Node;

				if ( oMainNode.GetNode( _T("Type0"), oType0Node ) )
				{
                    bsPDFFontName = oType0Node.GetAttribute( _T("value") );

					XmlUtils::CXmlNode oNode;

					if ( oType0Node.GetNode( _T("ToUnicode"), oNode ) ) 
					{						
						//todooo был ранее переход в анси .. нужен ли?
						CStringA sValue =  oNode.GetText();

						int		sz_out = Base64::Base64DecodeGetRequiredLength(sValue.GetLength());
						BYTE*  buf_out = new BYTE[sz_out + 64];

						if ((buf_out) && (Base64::Base64Decode(sValue.GetBuffer(),sValue.GetLength(), buf_out, &sz_out)))
						{
							PToUnicode pToUnicode = GetToUnicode( m_pDocument, buf_out, sz_out );
							wsToUnicodeName = CStringW( pToUnicode->sName );
							
							delete []buf_out;
						}
					}

					if ( oType0Node.GetNode( _T("Encoding"), oNode ) )
					{
						int nType = 0;
						wsEncodingName = oNode.GetAttribute( _T("name") );
						StreamRecPtr pStream = NULL, pUseCMapStream = NULL;

						CStringW wsCMapName, wsRegistry, wsOrdering, wsUseCMap;
						int nSupplement = 0, nWMode = 0;

						if ( _T("") == wsEncodingName )
						{
							nType = 1;
                            CString wsValue;

							XmlUtils::CXmlNode oEncNode;

							if ( oNode.GetNode( _T("CMapName"), oEncNode ) )
							{
								wsCMapName = oEncNode.GetAttribute( _T("name") );
								wsEncodingName = wsCMapName;
							}
							if ( oNode.GetNode( _T("CIDSystemInfo"), oEncNode ) )
							{
								XmlUtils::CXmlNode oInfo;
								if ( oEncNode.GetNode( _T("Registry"), oInfo ) )
								{
									wsRegistry = oInfo.GetAttribute( _T("string") );
								}
								if ( oEncNode.GetNode( _T("Ordering"), oInfo ) )
								{
									wsOrdering = oInfo.GetAttribute( _T("string") );
								}
								if ( oEncNode.GetNode( _T("Supplement"), oInfo ) )
								{
                                    wsValue = oInfo.GetAttribute( _T("integer") );
                                    nSupplement = XmlUtils::GetInteger( wsValue );
								}
							}
							if ( oNode.GetNode( _T("WMode"), oEncNode ) )
							{
                                wsValue = oEncNode.GetAttribute( _T("integer") );
                                nWMode = XmlUtils::GetInteger( wsValue );
							}
							if ( oNode.GetNode( _T("UseCMap"), oEncNode ) )
							{
								wsUseCMap = oEncNode.GetAttribute( _T("name") );
								
								const char *sT = NULL;
                                CString bsText = oEncNode.GetText();
						#if defined(_WIN32) || defined (_WIN64)
								USES_CONVERSION;
                                sT = W2A( bsText  );
						#else
                                std::string ansiText(bsText.begin(), bsText.end());
                                sT = ansiText.c_str();
						#endif								
								CStringA sText = sT;
								if ( sText.GetLength() > 0 )
								{
									int	sz_out = Base64::Base64DecodeGetRequiredLength(sText.GetLength());
									BYTE*  buf_out = new BYTE[sz_out + 64];

									if ((buf_out) && (Base64::Base64Decode(sText.GetBuffer(),sText.GetLength(), buf_out, &sz_out)))
									{
										pUseCMapStream = TempFileStreamNew( m_pDocument->oMMgr );
										if ( pUseCMapStream )
										{
											StreamSeek( pUseCMapStream, 0, SeekSet );
											StreamWrite( pUseCMapStream, buf_out, sz_out , true );
										}
										delete []buf_out;
									}
								}
							}
							if ( oNode.GetNode( _T("Stream"), oEncNode ) )
							{
								const char *sT= NULL;
                                CString bsText = oEncNode.GetText();
                        #if defined(_WIN32) || defined (_WIN64)
                                USES_CONVERSION;
                                sT = W2A( bsText  );
                        #else
                                std::string ansiText(bsText.begin(), bsText.end());
                                sT = ansiText.c_str();
                        #endif
								CStringA sText = sT;
								if ( sText.GetLength() > 0 )
								{
									int	sz_out = Base64::Base64DecodeGetRequiredLength(sText.GetLength());
									BYTE*  buf_out = new BYTE[sz_out + 64];

									if ((buf_out) && (Base64::Base64Decode(sText.GetBuffer(),sText.GetLength(), buf_out, &sz_out)))
									{
										pStream = TempFileStreamNew( m_pDocument->oMMgr );
										if ( pStream )
										{
											StreamSeek( pUseCMapStream, 0, SeekSet );
											StreamWrite( pStream, buf_out, sz_out, true );
										}
										delete []buf_out;
									}
								}
							}
						}

						char *sT = NULL;
                #if defined(_WIN32) || defined (_WIN64)
                        USES_CONVERSION;
                        sT = W2A( wsEncodingName );
                #else
                        std::string ansiText(wsEncodingName.begin(), wsEncodingName.end());
                        sT = (char*)ansiText.c_str();
                #endif
						EncoderRecPtr pEncoder = CMapEncoderStreamNew( m_pDocument->oMMgr, nType, sT , pStream );
						if ( !pEncoder )
							return AVS_OFFICEPDFWRITER_ERROR_FAILD_TO_ALLOC_MEM;

						//// Ищем Cmap файл и парсим его

						//CString wsCMapPath = CString(wsDrive) + CString(wsDir) + CString(wsFileName) + CString(_T(".cmap"));

						//if ( FileExist( wsCMapPath ) )
						//	CMapEncoderStreamLoadVector( pEncoder, wsCMapPath );
                        CStringA sCMapName(wsCMapName);
                        CStringA sRegistry(wsRegistry);
                        CStringA sOrdering(wsOrdering);
                        CStringA sUseCMap(wsUseCMap);

                        CMapEncoderStreamSetAttrs( pEncoder, sCMapName.GetBuffer() , sRegistry.GetBuffer(), sOrdering.GetBuffer(), nSupplement, (WritingMode)nWMode, sUseCMap.GetBuffer(), pUseCMapStream );
						nRet = DocRegisterEncoder( m_pDocument, pEncoder );
						if ( nRet == AVS_OFFICEPDFWRITER_ERROR_DUPLICATE_REGISTRATION )
							ResetError( m_pDocument);
					}

					if ( oType0Node.GetNode( _T("DescendantFonts"), oNode ) )
					{
						XmlUtils::CXmlNode oDescNode;
						int nCIDtype = 0; // 0 - Type 0, 2 - Type 2
                        CString wsBaseFont, wsValue;

						if ( oNode.GetNode( _T("Subtype"), oDescNode ) )
						{
                            wsValue = oDescNode.GetAttribute( _T("name") );
                            if      ( _T("CIDFontType0") == wsValue ) nCIDtype = 0;
                            else if ( _T("CIDFontType2") == wsValue ) nCIDtype = 2;
						}
						if ( oNode.GetNode( _T("BaseFont"), oDescNode ) )
						{
							wsBaseFont = oDescNode.GetAttribute( _T("name") );
						}
						// Создаем FontDef с данным именем
						char *sName = NULL;
				#if defined(_WIN32) || defined (_WIN64)
						USES_CONVERSION;
						sName = W2A( bsPDFFontName/*wsBaseFont*/ );
				#else
                        std::string ansiName(bsPDFFontName.begin(), bsPDFFontName.end());
                        sName = (char*)ansiName.c_str();
				#endif						
						FontDef pFontDef = CIDFontDefNew( m_pDocument->oMMgr, sName, NULL );

						// Добовляем в него CIDSystemInfo
						if ( oNode.GetNode( _T("CIDSystemInfo"), oDescNode ) )
						{
							XmlUtils::CXmlNode oInfo;
							CStringW wsRegistry, wsOrdering;
							int nSupplement = 0;

							if ( oDescNode.GetNode( _T("Registry"), oInfo ) )
							{
								wsRegistry = oInfo.GetAttribute( _T("string") );
							}
							if ( oDescNode.GetNode( _T("Ordering"), oInfo ) )
							{
								wsOrdering = oInfo.GetAttribute( _T("string") );
							}
							if ( oDescNode.GetNode( _T("Supplement"), oInfo ) )
							{
                                wsValue = oInfo.GetAttribute( _T("integer") );
                                nSupplement = XmlUtils::GetInteger( wsValue );
							}

#ifdef defined(_WIN32) || defined(_WIN64)
                            CIDFontDefSetInfoSys( pFontDef, W2A( wsRegistry.GetBuffer() ), W2A( wsOrdering.GetBuffer() ), nSupplement );
#else
                            std::wstring ws1 = string2std_string(wsRegistry);
                            std::string s1(ws1.begin(), ws1.end());

                            std::wstring ws2 = string2std_string(wsOrdering);
                            std::string s2(ws2.begin(), ws2.end());

                            CIDFontDefSetInfoSys( pFontDef, (char*)s1.c_str(), (char*)s2.c_str(), nSupplement );
#endif
						}

						// Записываем всевозможные настройки FontDescriptor
						if ( oNode.GetNode( _T("FontDescriptor"), oDescNode ) )
						{
							XmlUtils::CXmlNode oFDNode;

							if ( oDescNode.GetNode( _T("FontName"), oFDNode ) )
							{
								CString wsFontName = oFDNode.GetAttribute( _T("name") );
							}
							if ( oDescNode.GetNode( _T("FontStretch"), oFDNode ) )
							{
								CString wsStretch = oFDNode.GetAttribute( _T("name") );
							}
							if ( oDescNode.GetNode( _T("FontWeight"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                int nWeight = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("Flags"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("integer") );
                                pFontDef->nFlags = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("FontBBox"), oFDNode ) )
							{
								short shValue[4];
                                wsValue = oFDNode.GetAttribute( _T("value0") );
                                shValue[0] = XmlUtils::GetInteger( wsValue );
                                wsValue = oFDNode.GetAttribute( _T("value1") );
                                shValue[1] = XmlUtils::GetInteger( wsValue );
                                wsValue = oFDNode.GetAttribute( _T("value2") );
                                shValue[2] = XmlUtils::GetInteger( wsValue );
                                wsValue = oFDNode.GetAttribute( _T("value3") );
                                shValue[3] = XmlUtils::GetInteger( wsValue );

								pFontDef->oFontBox = ToBox( shValue[0], shValue[1], shValue[2], shValue[3] );
							}
							if ( oDescNode.GetNode( _T("ItalicAngle"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                pFontDef->nItalicAngle = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("Ascent"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                pFontDef->nAscent = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("Descent"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                pFontDef->nDescent = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("Leading"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                int nLeading = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("CapHeight"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                pFontDef->nCapHeight = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("XHeight"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                pFontDef->nXHeight = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("StemV"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                pFontDef->nStemV = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("StemH"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                pFontDef->nStemH = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("AvgWidth"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                pFontDef->nAvgWidth = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("MaxWidth"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                pFontDef->nMaxWidth = XmlUtils::GetInteger( wsValue );
							}
							if ( oDescNode.GetNode( _T("MissingWidth"), oFDNode ) )
							{
                                wsValue = oFDNode.GetAttribute( _T("number") );
                                pFontDef->nMissingWidth = XmlUtils::GetInteger( wsValue );
							}
						}

						// Записываем файл с шрифтом и устанавливаем его тип

						CIDFontFileType eType;

						if      ( _T(".cid_0"   ) == sExt ) eType = cidType0;
						else if ( _T(".cid_0c"  ) == sExt ) eType = cidType0C;
						else if ( _T(".cid_0cot") == sExt ) eType = cidType0COT;
						else if ( _T(".cid_2"   ) == sExt ) eType = cidType2;
						else if ( _T(".cid_2ot" ) == sExt ) eType = cidType2OT;
						else
						{
							if ( 0 == nCIDtype )
								eType = cidType0;
							else
								eType = cidType2;
						}

						if ( bEmb )
						{
							if ( OK != ( nRet = CIDFontDefLoadFontData( pFontDef, m_oFont.Path, eType ) ) )
								return nRet;
						}
						else
						{
							if ( OK != ( nRet = CIDFontDefLoadFontData( pFontDef, _T(""), eType ) ) )
								return nRet;
						}

						// Пишим ширины 
						if ( oNode.GetNode( _T("DW"), oDescNode ) )
						{
                            wsValue = oDescNode.GetAttribute( _T("integer") );
                            int nDW = XmlUtils::GetInteger( wsValue );
							CIDFontDefSetDW( pFontDef, nDW );
						}
						if ( oNode.GetNode( _T("W"), oDescNode ) )
						{
							int nPrevType = -1; // -1 - ничего, 0 - cid, 1 - cid_last, 2 - array, 3 - width
							int nCid_first = 0, nCid_last = 0, nWidth = 0;
							CIDWidth oWidth;

							XmlUtils::CXmlNodes oNodesW;
							// Забираем все ноды
							oDescNode.GetNodes( _T("*"), oNodesW );
							for ( int nIndex = 0; nIndex < oNodesW.GetCount(); nIndex++ )
							{
								XmlUtils::CXmlNode oNodeW;
								if ( oNodesW.GetAt( nIndex, oNodeW ) )
								{
									if ( _T("Int") == oNodeW.GetName() )
									{
                                        wsValue = oNodeW.GetAttribute( _T("value") );
										if ( -1 == nPrevType || 2 == nPrevType || 3 == nPrevType )
										{
											nPrevType = 0;
                                            nCid_first = XmlUtils::GetInteger( wsValue );
										}
										else if ( 0 == nPrevType )
										{
											nPrevType = 1;
                                            nCid_last = XmlUtils::GetInteger( wsValue );
										}
										else // if ( 1 == nPrevType )
										{
											nPrevType = 3;
                                            nWidth = XmlUtils::GetInteger( wsValue );

											oWidth.unCid_First = nCid_first;
											oWidth.unCid_Last  = nCid_last;
											oWidth.shWidth     = nWidth;

											CIDFontDefAddWidth( pFontDef, oWidth );
										}
									}
									else if ( _T("Array") == oNodeW.GetName() )
									{
										nPrevType = 2;

										XmlUtils::CXmlNodes oArrayNodes;
										oNodeW.GetNodes( _T("Int"), oArrayNodes );
										for ( int nArrayIndex = 0; nArrayIndex < oArrayNodes.GetCount(); nArrayIndex++ )
										{
											XmlUtils::CXmlNode oArrayItem;
											if ( oArrayNodes.GetAt( nArrayIndex, oArrayItem ) )
											{
                                                wsValue = oArrayItem.GetAttribute( _T("value") );
                                                nWidth = XmlUtils::GetInteger( wsValue );

												oWidth.unCid_First = nCid_first;
												oWidth.unCid_Last  = nCid_first;
												oWidth.shWidth     = nWidth;

												nCid_first++;

												CIDFontDefAddWidth( pFontDef, oWidth );
											}
										}
									}
								}
							}
						}
						if ( oNode.GetNode( _T("DW2"), oDescNode ) )
						{
							int anDW2[2];
                            wsValue = oDescNode.GetAttribute( _T("value0") );
                            anDW2[0] = XmlUtils::GetInteger( wsValue );
                            wsValue = oDescNode.GetAttribute( _T("value1") );
                            anDW2[1] = XmlUtils::GetInteger( wsValue );
							CIDFontDefSetDW2( pFontDef, anDW2 );
						}
						if ( oNode.GetNode( _T("W2"), oDescNode ) )
						{
							int nPrevType = -1; // -1 - ничего, 0 - cid, 1 - cid_last, 2 - array, 3 - W, 4 - Vx, 5 - Vy
							int nCid_first = 0, nCid_last = 0, nWidth = 0, nVx = 0, nVy = 0;
							CIDWidth2 oWidth2;

							XmlUtils::CXmlNodes oNodesW;
							// Забираем все ноды
							oDescNode.GetNodes( _T("*"), oNodesW );
							for ( int nIndex = 0; nIndex < oNodesW.GetCount(); nIndex++ )
							{
								XmlUtils::CXmlNode oNodeW;
								if ( oNodesW.GetAt( nIndex, oNodeW ) )
								{
									if ( _T("Int") == oNodeW.GetName() )
									{
                                        wsValue = oNodeW.GetAttribute( _T("value") );
										if ( -1 == nPrevType || 2 == nPrevType || 5 == nPrevType )
										{
											nPrevType = 0;
                                            nCid_first = XmlUtils::GetInteger( wsValue );
										}
										else if ( 0 == nPrevType )
										{
											nPrevType = 1;
                                            nCid_last = XmlUtils::GetInteger( wsValue );
										}
										else if ( 1 == nPrevType )
										{
											nPrevType = 3;
                                            nWidth = XmlUtils::GetInteger( wsValue );
										}
										else if ( 3 == nPrevType )
										{
											nPrevType = 4;
                                            nVx = XmlUtils::GetInteger( wsValue );
										}
										else // if ( 4 == nPrevType )
										{
											nPrevType = 5;
                                            nVy = XmlUtils::GetInteger( wsValue );

											oWidth2.unCid_First = nCid_first;
											oWidth2.unCid_Last  = nCid_last;
											oWidth2.shW         = nWidth;
											oWidth2.shVx        = nVx;
											oWidth2.shVy        = nVy;

											CIDFontDefAddWidth2( pFontDef, oWidth2 );
										}
									}
									else if ( _T("Array") == oNodeW.GetName() )
									{
										nPrevType = 2;

										int nWidthPrevType = -1; // -1 - ничего, 0 - W, 1 - Vx, 2 - Vy

										XmlUtils::CXmlNodes oArrayNodes;
										oNodeW.GetNodes( _T("Int"), oArrayNodes );
										for ( int nArrayIndex = 0; nArrayIndex < oArrayNodes.GetCount(); nArrayIndex++ )
										{
											XmlUtils::CXmlNode oArrayItem;
											if ( oArrayNodes.GetAt( nArrayIndex, oArrayItem ) )
											{
                                                wsValue = oArrayItem.GetAttribute( _T("value") );

												if ( -1 == nWidthPrevType || 2 == nWidthPrevType )
												{
													nWidthPrevType = 0;
                                                    nWidth = XmlUtils::GetInteger( wsValue );
												}
												else if ( 0 == nWidthPrevType )
												{
													nWidthPrevType = 1;
                                                    nVx = XmlUtils::GetInteger( wsValue );
												}
												else // if ( 1 == nWidthPrevType )
												{
													nWidthPrevType = 2;
                                                    nVy = XmlUtils::GetInteger( wsValue );

													oWidth2.unCid_First = nCid_first;
													oWidth2.unCid_Last  = nCid_first;
													oWidth2.shW         = nWidth;
													oWidth2.shVx        = nVx;
													oWidth2.shVy        = nVy;

													nCid_first++;

													CIDFontDefAddWidth2( pFontDef, oWidth2 );
												}
											}
										}
									}
								}
							}
						}
						if ( oNode.GetNode( _T("CIDToGIDMap"), oDescNode ) )
						{
                            wsValue = oDescNode.GetAttribute( _T("name"), _T("") );
                            if (wsValue.GetLength() > 0 )
							{
                                CStringA sValue(wsValue);
                                CIDFontDefSetCIDtoGID( pFontDef, sValue.GetBuffer() );
							}
							else
							{								
                                CString wsText = oDescNode.GetText();
                                if ( wsText.GetLength() > 0 )
								{
                                    CStringA sText(wsText);
                                    int		sz_out = Base64::Base64DecodeGetRequiredLength(sText.GetLength());
									BYTE*  buf_out = new BYTE[sz_out + 64];

                                    if ((buf_out) && (Base64::Base64Decode(sText.GetBuffer(),sText.GetLength(), buf_out, &sz_out)))
									{
										CIDFontDefSetCIDtoGID( pFontDef, buf_out, sz_out );

										delete []buf_out;
									}									
								}
							}
						}
                        CStringA sEncodingName(wsEncodingName);
                        CStringA sToUnicodeName (wsToUnicodeName);

                        EncoderRecPtr pEncoder = GetEncoder( m_pDocument, sEncodingName );
                        PToUnicode pToUnicode = GetToUnicode( m_pDocument, sToUnicodeName );

                        FontDict pFont = Type0FontNew2( m_pDocument->oMMgr, pFontDef, pEncoder, m_pDocument->pXref, pToUnicode);

						// Регистрируем FontDef и Font в документе
						if ( pFontDef )
							ListAdd( m_pDocument->pFontDefList, pFontDef );

						if ( pFont )
							ListAdd( m_pDocument->pFontManager, pFont );	

						if ( _T("") != bsPDFFontName )
							m_oFontLoader.Add( m_oFont.Path, string2std_string(bsPDFFontName),
												string2std_string(wsEncodingName), string2std_string(wsToUnicodeName) );
						else
							m_oFontLoader.Add( m_oFont.Path, _T(""), string2std_string(wsEncodingName), string2std_string(wsToUnicodeName));


                        nRet = SetCurrentFont( bsPDFFontName, wsEncodingName, false, wsToUnicodeName );

						return nRet;
					}
				}
			}
		}

		if ( _T("") != bsPDFFontName )
            nRet = SetCurrentFont( bsPDFFontName, _T(""), false, _T(""));

		return nRet;
	}

	return OK;
}
unsigned long CPdfWriterLib::GdiFontToPdfFont(CString bsEncodingName/* = L"CP1251"*/, bool bNameIsUnicodeArray/* = false*/)
{
	CString bsDefaultFont     = L"Helvetica";
	CString bsDefaultEncoding = L"CP1251"; 
	
    bool bBold   = m_oFont.Bold;
    bool bItalic = m_oFont.Italic;

	std::wstring wsDisplayName;
	std::wstring wsFontName;

	// получаем фонт по его названию
	std::wstring bsFileName;
	if ( !m_pFindFonts->Find( m_oFont.Name, &bsFileName, &bBold, &bItalic ) )
	{
        bool bOldBold   = bBold;
        bool bOldItalic = bItalic;

		if ( !GetFontFile( &m_oFont, m_oFont.Name, wsDisplayName, wsFontName, &bBold, &bItalic ) )
		{
			m_pFindFonts->Add( m_oFont.Name, _T(""), bOldBold, bOldItalic, bBold, bItalic );

			SetCurrentFont( bsDefaultFont, bsDefaultEncoding, bNameIsUnicodeArray, _T("") );
			return AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND;
		}

		m_pFindFonts->Add( m_oFont.Name, wsFontName, bOldBold, bOldItalic, bBold, bItalic );

	}
	else
	{
		if ( bsFileName.length() <= 0 || bsFileName.length() >= 1000 )
		{
			SetCurrentFont( bsDefaultFont, bsDefaultEncoding, bNameIsUnicodeArray, _T("") );
			return AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND;
		}
		else
		{
            //2 ???? 4 ???? todoooo
            wsFontName = bsFileName;
        }
	}

	m_oFont.Bold   = bBold;
	m_oFont.Italic = bItalic;

	// пока фонт не включаем в документ 
	// TO DO: 1. Сначала пытаемся включить фонт в документ
	//        2. Если этого сделать нельзя, тогда загружаем фонт не включая его в документ.
	CString bsPDFFontName	= _T("Embedded");
	CString bsFontFile		= std_string2string(wsFontName);

    unsigned long nRes = LoadTTFFromFile( bsFontFile, true, bsEncodingName, &bsPDFFontName );
	if ( OK == nRes )
	{
		nRes = SetCurrentFont( bsPDFFontName, bsEncodingName, bNameIsUnicodeArray, _T("") );
		return nRes;
	}
	else if ( AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR == nRes )
		ResetError( m_pDocument );
	else
	{
		ResetError( m_pDocument );
		SetCurrentFont( bsDefaultFont, bsDefaultEncoding, bNameIsUnicodeArray, _T("") );
		return nRes;
	}

	// Скорее всего фонт не загрузился из-за того, что sFontFile содержит не полный путь
	char *pValue;
	size_t len;
//todooo
//	errno_t err = _dupenv_s( &pValue, &len, "windir" );

    CString sFontFilePath(pValue);
	free( pValue );
	
	sFontFilePath += CString(FILE_SEPARATOR_STR) + _T("Fonts") + FILE_SEPARATOR_STR;
	sFontFilePath += bsFontFile;
	bsFontFile = sFontFilePath;

    nRes = LoadTTFFromFile( bsFontFile, true, bsEncodingName, &bsPDFFontName );
	if ( OK == nRes )
	{
		nRes = SetCurrentFont( bsPDFFontName, bsEncodingName, bNameIsUnicodeArray, _T(""));
		return nRes;
	}
	else 
	{
		ResetError( m_pDocument );
		SetCurrentFont( bsDefaultFont, bsDefaultEncoding, bNameIsUnicodeArray, _T("") );
		return AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND;
	}
}
unsigned long CPdfWriterLib::GdiFontToPdfFont2(CString bsToUnicodeName/* = NULL*/, CString bsEncodingName/* = NULL*/)
{
	CString bsDefaultFont     = L"Helvetica";

    bool bBold   = m_oFont.Bold;
    bool bItalic = m_oFont.Italic;

	std::wstring wsDisplayName;
	std::wstring wsFontName;

	// получаем фонт по его названию
	std::wstring bsFileName;

    bool bPath = ( m_oFont.Path != _T("") ? true : false );
	if ( bPath )
		m_oFont.Name = m_oFont.Path;

	if ( !m_pFindFonts->Find( m_oFont.Name, &bsFileName, &bBold, &bItalic ) )
	{
        bool bOldBold   = bBold;
        bool bOldItalic = bItalic;

		if ( !bPath )
		{
			if ( !GetFontFile2( &m_oFont, m_oFont.Name, wsDisplayName,  wsFontName, &bBold, &bItalic, m_pFontManager ) )
			{
				m_pFindFonts->Add( m_oFont.Name, std::wstring(_T("")), bOldBold, bOldItalic, bBold, bItalic );

                SetCurrentFont( bsDefaultFont, _T(""), false, _T(""));
				return AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND;
			}
		}
		else
		{
            int nTempLen = (std::min)( (size_t)499, m_oFont.Path.length() );

			wsFontName		= std::wstring(m_oFont.Path.begin() , m_oFont.Path.begin() + nTempLen );
			wsDisplayName	= std::wstring(m_oFont.Path.begin() , m_oFont.Path.begin() + nTempLen );

		}

		m_pFindFonts->Add( m_oFont.Name, std::wstring(wsFontName), bOldBold, bOldItalic, bBold, bItalic );

	}
	else
	{
		CString sFName = std_string2string( bsFileName );
		if ( sFName.GetLength() <= 0 || sFName.GetLength() >= 1000 )
		{
            SetCurrentFont( bsDefaultFont, _T(""), false, _T(""));
			return AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND;
		}
		else
		{
            //2 ???? 4 ???? todoooo
            wsFontName = string2std_string(sFName);
		}
	}
	m_oFont.Bold   = bBold;
	m_oFont.Italic = bItalic;

	// пока фонт не включаем в документ 
	// TO DO: 1. Сначала пытаемся включить фонт в документ
	//        2. Если этого сделать нельзя, тогда загружаем фонт не включая его в документ.
	CString bsPDFFontName	= _T("Embedded");
	CString bsFontFile		= std_string2string(wsFontName);

	if (bsFontFile.GetLength() < 1) bsFontFile = std_string2string(m_oFont.Name);

    unsigned long nRes = LoadTTFFromFile( bsFontFile, true, bsToUnicodeName, &bsPDFFontName );
	if ( OK == nRes )
	{
        nRes = SetCurrentFont( bsPDFFontName, bsEncodingName, false, bsToUnicodeName );
		return nRes;
	}
	else if ( AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR == nRes )
		ResetError( m_pDocument );
	else
	{
		ResetError( m_pDocument );
        SetCurrentFont( bsDefaultFont, _T(""), false, _T("") );
		return nRes;
	}

	// Скорее всего фонт не загрузился из-за того, что sFontFile содержит не полный путь
	char *pValue = NULL;
	size_t len=0;

#if defined(_WIN32) || defined(_WIN64)
    errno_t err = _dupenv_s( &pValue, &len, "windir" );
#endif
	CString sFontFilePath;
	if (pValue)
	{
		sFontFilePath = CString(pValue);
		free( pValue );
	}
	
	sFontFilePath += CString(FILE_SEPARATOR_STR) + _T("Fonts") + FILE_SEPARATOR_STR;
	sFontFilePath += bsFontFile;

    nRes = LoadTTFFromFile(sFontFilePath, true, bsEncodingName, &bsPDFFontName );
	if ( OK == nRes )
	{
        nRes = SetCurrentFont( bsPDFFontName, bsEncodingName, false, bsToUnicodeName );
		return nRes;
	}
	else 
	{
		ResetError( m_pDocument );
        SetCurrentFont( bsDefaultFont, _T(""), false, _T(""));
		return AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND;
	}
}
unsigned long CPdfWriterLib::GdiDashToPDFDash( int nDashStyle/* = 0*/, double *pDashPattern/* = NULL*/, int nDashPatternSize/* = 0*/, float fDashOffset/* = 0 */)
{
	unsigned long nRet = OK;

	float fWidth = PageGetLineWidth( m_pDocument->pCurPage );

	// Константы для стилей от 1 до 4 вычислены на основе MicrosoftOfficePowerPoint
	if ( 0 == nDashStyle ) // Solid
	{
		nRet = PageSetDash( m_pDocument->pCurPage, NULL, 0, 0 );
	}
	else if ( 1 == nDashStyle )
	{
		double arrDashPtn[2] = { 4 * fWidth, 3 * fWidth };
		nRet = PageSetDash( m_pDocument->pCurPage, arrDashPtn, 2, fDashOffset );
	}
	else if ( 2 == nDashStyle )
	{
		double arrDashPtn[2] = { fWidth, fWidth };
		nRet = PageSetDash( m_pDocument->pCurPage, arrDashPtn, 2, fDashOffset );
	}
	else if ( 3 == nDashStyle )
	{
		double arrDashPtn[4] = { 4 * fWidth, 3 * fWidth, fWidth, 3 * fWidth };
		nRet = PageSetDash( m_pDocument->pCurPage, arrDashPtn, 4, fDashOffset );
	}
	else if ( 4 == nDashStyle )
	{
		double arrDashPtn[6] = { 8 * fWidth, 3 * fWidth, fWidth, 3 * fWidth, fWidth, 3 * fWidth };
		nRet = PageSetDash( m_pDocument->pCurPage, arrDashPtn, 6, fDashOffset );
	}
	else if ( 5 == nDashStyle ) // Custom
	{
		nRet = PageSetDash( m_pDocument->pCurPage, pDashPattern, nDashPatternSize, fDashOffset );
	}

	return nRet;
}


unsigned long CPdfWriterLib::ReadOutlineChilds(CString sXml, int nRootIndex)
{
	HRESULT hRes = S_OK;
	XmlUtils::CXmlNode oNode;
	oNode.FromXmlString(sXml);

	XmlUtils::CXmlNodes oNodes;
	oNode.GetNodes( _T("Chapter"), oNodes);

	for ( int nIndex = 0; nIndex < oNodes.GetCount(); nIndex++ )
	{
		XmlUtils::CXmlNode oCurNode;
		oNodes.GetAt( nIndex, oCurNode );
		if ( -1 != oCurNode.GetName().Find( _T("Chapter") ) )
		{
			CString sName = oCurNode.GetAttribute( _T("name") );
			CStringA sPage = (CStringA)oCurNode.GetAttribute( _T("page") );
			int nPageNum = UtilsAToI( sPage.GetBuffer() );

            if ( S_OK != ( hRes = SetCurrentPage( nPageNum ) ) )
				return hRes;

			CString sNode = oNode.GetXml();

            if ( S_OK !=  ( hRes = CreateNewOutline( nRootIndex, sName, _T("")) ) )
				return hRes;
            if ( S_OK !=  ( hRes = CreateNewDestination() ) )
				return hRes;
            if ( S_OK !=  ( hRes = OutlineSetCurrentDestination() ) )
				return hRes;
            if ( S_OK !=  ( hRes = SetOutlineOpened( false ) ) )
				return hRes;

			long nCurrentIndex = -1;
            if ( S_OK !=  ( hRes = GetCurrentOutlineIndex(&nCurrentIndex) ) )
				return hRes;
            if ( S_OK !=  ( hRes = ReadOutlineChilds( oCurNode.GetXml(), nCurrentIndex ) ) )
				return hRes;
		}
	}
	return S_OK;
}
unsigned long CPdfWriterLib::UpdateGState()
{
	HRESULT hRes = S_OK;

	if ( ! ( NULL != m_pCurCommand->pPrev && c_nClipType == m_pCurCommand->pPrev->nType ) )
	{
        if ( S_OK != ( hRes = GSave() ) )
			return hRes;

        if ( S_OK != ( hRes = UpdateCoordSpace() ) )
			return hRes;

		long nColor = m_oBrush.Color1;
		float fR = ( nColor & 0xFF ) / 255.0f; 
		float fG = ( ( nColor >> 8 ) & 0xFF ) / 255.0f; 
		float fB = ( (nColor >> 16) ) / 255.0f; 

        if ( S_OK != ( hRes = SetRGBFill( fR, fG, fB ) ) )
			return hRes;

		float fLineWidth = MMToPDFCoords( m_oPen.Size );
        if ( S_OK != ( hRes = SetLineWidth( fLineWidth ) ) )
			return hRes;

		float  fDashOffset      = m_oPen.DashOffset;
		int    nDashStyle       = m_oPen.DashStyle;

		double *pDashPattern     = m_oPen.DashPattern;
		int     nDashPatternSize = m_oPen.Count;

        if ( S_OK != ( hRes = GdiDashToPDFDash( nDashStyle, pDashPattern, nDashPatternSize, fDashOffset ) ) )
			return hRes;

		//if ( 0 != nLineCap )
        //	if ( S_OK != ( hRes = SetLineCap( nLineCap ) ) )
		//		return hRes;

		//int nLineJoin = m_oPen.LineJoin;
		//if ( 0 != nLineJoin )
        //	if ( S_OK != ( hRes = SetLineJoin( nLineJoin ) ) )
		//		return hRes;

		nColor = m_oPen.Color;
		fR = ( nColor & 0xFF ) / 255.0f; 
		fG = ( ( nColor >> 8 ) & 0xFF ) / 255.0f; 
		fB = ( (nColor >> 16) ) / 255.0f; 

        if (S_OK != (hRes = SetRGBStroke(fR, fG, fB)))
			return hRes;

		if (c_BrushTypeTexture == m_oBrush.Type && c_BrushTextureModeStretch == m_oBrush.TextureMode)
		{
			if (255 != m_oBrush.TextureAlpha)
			{
				XmlUtils::CXmlWriter oWriter;

				oWriter.WriteNodeBegin(_T("ExtGState"));

				if (255 != m_oBrush.Alpha1)
				{
                    oWriter.WriteNodeBegin(_T("AlphaFill"), true);
					oWriter.WriteAttribute(_T("value"), (double)m_oBrush.TextureAlpha/255.0f);
                    oWriter.WriteNodeEnd(_T("AlphaFill"), true, true);
				}

				oWriter.WriteNodeEnd(_T("ExtGState"));

				CString bsXml = oWriter.GetXmlString();
                if (S_OK != (hRes = SetExtGraphicsState2(bsXml)))
				{
					return hRes;
				}
			}
		}
		else 
		{
			if (255 != m_oBrush.Alpha1 || 255 != m_oPen.Alpha)
			{
				XmlUtils::CXmlWriter oWriter;

				oWriter.WriteNodeBegin( _T("ExtGState") );

				if (255 != m_oBrush.Alpha1)
				{
                    oWriter.WriteNodeBegin( _T("AlphaFill"), true);
					oWriter.WriteAttribute( _T("value"), (double)m_oBrush.Alpha1/255.0f);
                    oWriter.WriteNodeEnd( _T("AlphaFill"), true, true);
				}

				if ( 255 != m_oPen.Alpha )
				{
                    oWriter.WriteNodeBegin(_T("AlphaStroke"), true);
					oWriter.WriteAttribute(_T("value"), (double)m_oPen.Alpha/255.0f);
                    oWriter.WriteNodeEnd(_T("AlphaStroke"), true, true);
				}

				oWriter.WriteNodeEnd(_T("ExtGState"));

				CString sXml = oWriter.GetXmlString();
                if (S_OK != (hRes = SetExtGraphicsState2(sXml)))
				{
					return hRes;
				}
			}
		}
	}
	else
	{
        if (S_OK != (hRes = UpdateCoordSpace()))
			return hRes;
	}

	return S_OK;
}
unsigned long CPdfWriterLib::UpdateCoordSpace()
{
	// Здесь предполагается, что текущая матрица CTM единичная
	// Поэтому этой функцией надо пользоваться аккуратно

	HRESULT hRes = S_OK;

	float fPageHeight = 0.0f;
	float fPageWidth  = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;

    if ( S_OK != ( hRes = GetWidth( &fPageWidth ) ) )
		return hRes;

	DWORD  nFlags  = m_oCommandParams.nFlag;
	if ( !( nFlags == 16 ) )
	{

		double dAngle  = m_oCommandParams.dAngle;
		double dLeft   = m_oCommandParams.dLeft;
		double dHeight = m_oCommandParams.dHeight;
		double dWidth  = m_oCommandParams.dWidth;
		double dTop    = m_oCommandParams.dTop;

		if ( 0 == nFlags && 0 == dAngle )
			return S_OK;

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
            //if ( S_OK != ( hRes = Concat( dA, dB, dC, dD, dE, dF ) ) )
			//	return hRes;
		}
		else if ( bFlipX && !bFlipY )
		{
			dA = -dA;
			dC = -dC;
			dE = -dE + 2 * dX0;
            //if ( S_OK != ( hRes = Concat( -dA, dB, -dC, dD, -dE + 2 * dX0, dF ) ) )
			//	return hRes;
		}
		else if ( !bFlipX && bFlipY )
		{
			dB = -dB;
			dD = -dD;
			dF = -dF + 2 * dY0;

            //if ( S_OK != ( hRes = Concat( dA, -dB, dC, -dD, dE, -dF + 2 * dY0 ) ) )
			//	return hRes;
		}
		else
		{
			dA = -dA;
			dB = -dB;
			dC = -dC;
			dD = -dD;
			dE = -dE + 2 * dX0;
			dF = -dF + 2 * dY0;
            //if ( S_OK != ( hRes = Concat( -dA, -dB, -dC, -dD, -dE + 2 * dX0, -dF + 2 * dY0 ) ) )
			//	return hRes;
		}

        if ( S_OK != ( hRes = Concat( dA, dB, dC, dD, dE, dF ) ) )
			return hRes;

		//if ( (NULL != m_pCurCommand->pPrev && c_nClipType == m_pCurCommand->pPrev->nType) || c_nClipType == m_pCurCommand->nType )
		m_oClipCTM.Set( dA, dB, dC, dD, dE, dF );
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

		//if ( (NULL != m_pCurCommand->pPrev && c_nClipType == m_pCurCommand->pPrev->nType) || c_nClipType == m_pCurCommand->nType )
		m_oClipCTM.Set( fA, -fB, -fC, fD, MMToPDFCoords( fE ) + fC * fPageHeight, fPageHeight - fPageHeight * fD - MMToPDFCoords( fF ) );

	}
	return S_OK;

}
unsigned long CPdfWriterLib::UpdateCoordSpace2(CMatrix *pTransform)
{
	// Здесь предполагается, что текущая матрица CTM единичная
	// Поэтому этой функцией надо пользоваться аккуратно

	HRESULT hRes = S_OK;

	float fPageHeight = 0.0f;
	float fPageWidth  = 0.0f;

    if ( S_OK != ( hRes = GetHeight( &fPageHeight ) ) )
		return hRes;

    if ( S_OK != ( hRes = GetWidth( &fPageWidth ) ) )
		return hRes;


	float fA = (float)pTransform->get_A();
	float fB = (float)pTransform->get_B();
	float fC = (float)pTransform->get_C();
	float fD = (float)pTransform->get_D();
	float fE = (float)pTransform->get_E();
	float fF = (float)pTransform->get_F();

    if ( S_OK != ( hRes = Concat( fA, -fB, -fC, fD, MMToPDFCoords( fE ) + fC * fPageHeight, fPageHeight - fPageHeight * fD - MMToPDFCoords( fF ) ) ) )
		return hRes;

	m_oClipCTM.Set( fA, -fB, -fC, fD, MMToPDFCoords( fE ) + fC * fPageHeight, fPageHeight - fPageHeight * fD - MMToPDFCoords( fF ) );

	return S_OK;

}
bool CPdfWriterLib::FileExist(CString sFilePath )
{
	return NSFile::CFileBinary::Exists(string2std_string(sFilePath)) || CFileDownloader::IsNeedDownload(sFilePath) ;
}

// Устанавливаем текущее состояние рендерера
bool CPdfWriterLib::SetState ( ERendererState eState )
{
	m_oRendererState.set_NewState( eState, m_oTransform );

	if ( rendstatePath == m_oRendererState.get_PrevState() && rendstatePath != m_oRendererState.get_State() )
	{
		// TODO: перенести сюда запись пата
	}
	else if ( rendstateClipPath == m_oRendererState.get_PrevState() && rendstateClipReset == m_oRendererState.get_State() )
	{
		// Набили клип и его удалили, в данном случае ничего не делаем. Обнуляем клип
	}
	else if ( rendstateClipPath == m_oRendererState.get_PrevState() && rendstateClipPath != m_oRendererState.get_State() )
	{
		// TODO: Перенести сюда набивку клипа
	}
	else if ( rendstateClipReset == m_oRendererState.get_PrevState() && rendstateClipReset != m_oRendererState.get_State() )
	{
		// TODO: Reset клипа
	}
	else if ( rendstateText == m_oRendererState.get_PrevState() && ( rendstateText != m_oRendererState.get_State() || m_oRendererState.IsMatrixChanged() ) )
	{
		// TODO: Пока оставим так, но в будущем сделать, чтобы можно было менять матрицу сразу у текста

		int nTextsCount = m_oContiniousText.GetSize();

		// Если матрица преобразования изменилась, тогда последний текстовый
		// элемент не нужно записывать. С него нужно начать новый текст.
		if ( rendstateText == m_oRendererState.get_State() && m_oRendererState.IsMatrixChanged() )
			nTextsCount--;	

		if ( nTextsCount > 0 )
		{
			//CSynchAccess oSynchAccess = m_hSynchMutex;

			// Загрузка шрифтов проиходит через m_oFont, поэтому далее этот параметр изменится
			NSStructures::CFont CurFont = m_oFont;

			// Запоминаем некоторые настройки, чтобы потом их восстановить
			float fOldLineWidth = 0.0f;
			float fFillR, fFillG, fFillB, fStrokeR, fStrokeG, fStrokeB;

            if ( S_OK != ( GetLineWidth( &fOldLineWidth ) ) )
                return false;

            if ( S_OK != ( GetRGBFill( &fFillR, &fFillG, &fFillB ) ) )
                return false;

            if ( S_OK != ( GetRGBStroke( &fStrokeR, &fStrokeG, &fStrokeB ) ) )
                return false;


            bool bDefaultFont = false;
			bool bWasHorScale = false;
			bool bFirst = true;
			NSStructures::CFont oPrevFont;
			CContiniousText::TColor oPrevColor;
			PToUnicode pPrevToUnicode = NULL;
			double dPrevCharSpacing = 0;

			long nAscent = 0;
			long nDescent = 0;
			long nXHeight = 0;
			double dItalicAngle = 0;
			float fPageHeight = 0.0f;
			float fFontSize = 0;

			float fXHeight = (float)nXHeight / 1000;
			float fAscent  = (float)nAscent / 1000;
			float fDescent = fabs( (float)nDescent / 1000 );
			float fR = 0;
			float fG = 0;
			float fB = 0;

            if ( S_OK != ( GetHeight( &fPageHeight ) ) )
                return false;

			// Делаем сохранение состояния
            //ATLTRACE2( _T("GSave: DrawText\n") );
            if ( S_OK != ( GSave() ) )
                return false;

			// Выставляем систему координат
			CMatrix oTransform = m_oContiniousText.get_Matrix();
            if ( S_OK != ( UpdateCoordSpace2( &oTransform ) ) )
                return false;

			// Устанавливаем альфу для данного текста
			float fAlphaFill   = (float)m_oContiniousText.get_Color( 0 )->unA / 255.0f;
			float fAlphaStroke = (float)m_oContiniousText.get_Color( 0 )->unA / 255.0f;
			m_pCurrentExtGState = GetExtGState( m_pDocument, fAlphaStroke, fAlphaFill );
			if ( OK != ( PageSetExtGState( m_pDocument->pCurPage, m_pCurrentExtGState ) ) )
                return false;

            if ( S_OK != ( BeginText() ) )
                return false;

			// Выясним как нам рисовать текст (пока сделаем всегда fill)
			// TODO: В будущем надо добавить данную настройку в ContiniusText, и следить
			//       за ее изменениями, как, например, при изменении шрифта
			ETextRenderingMode eRenderingMode = ETextRenderingMode::Fill; // Fill (по умолчанию)
			//if ( c_nStrokeTextType == m_pCurCommand->nType )
			//	eRenderingMode = ETextRenderingMode::Stroke; // Stroke

            if ( S_OK != ( SetTextRenderingMode( (long)eRenderingMode ) ) )
                return false;

			/*
			if ( ETextRenderingMode::Fill == eRenderingMode )
			{
			// Если данный фонт не нашли с атрибутом Bold, тогда заливаем и обводим текст, тем самым утолщяя его
			if ( m_oFont.Bold )
			{
            if ( S_OK != ( hRes = SetTextRenderingMode((long)ETextRenderingMode::FillThenStroke) ) )
			{
			::SysFreeString( bsCodedString );
			return hRes;
			}
			}
			else
            if ( S_OK != ( hRes = SetTextRenderingMode((long)ETextRenderingMode::Fill) ) )
			{
			::SysFreeString( bsCodedString );
			return hRes;
			}
			}
			else
			{
            if ( S_OK != ( hRes = SetTextRenderingMode( (long)eRenderingMode ) ) )
			{
			::SysFreeString( bsCodedString );
			return hRes;
			}
			}
			*/


			for ( int nIndex = 0; nIndex < nTextsCount; nIndex++ )
			{
				const CContiniousText::TText    *pText    = m_oContiniousText.get_Text   ( nIndex );
				const NSStructures::CFont       *pFont    = m_oContiniousText.get_Font   ( nIndex );
				const CContiniousText::TColor   *pColor   = m_oContiniousText.get_Color  ( nIndex );
				const double                     dCharSp  = m_oContiniousText.get_CharSp ( nIndex );

				if ( !pText || !pFont || !pColor )
					continue;

				// Ищем кодировку ToUnicode для данной строки
				PToUnicode pToUnicode = FindToUnicodeForString( m_pDocument, pText->sText );
				if ( !pToUnicode )
					continue;

				// Кодируем нашу строку в соответствии с текущей ToUnicode
                std::wstring sCodedString;
                ToUnicodeWCharToString( pToUnicode, pText->sText, sCodedString );

				bool bFontChange = false, bSizeChange = false, bColorChange = false, bCharSpChange = false;

				if ( bFirst )
				{
					// Для первого текста выставляем все настройки
					bFontChange   = true;
					bSizeChange   = true;
					bColorChange  = true;
					bCharSpChange = true;

					bFirst        = false;
				}
				else
				{
					if (  ( CString( pToUnicode->sName ) != CString( pPrevToUnicode->sName ) ) || ( oPrevFont.Path != pFont->Path ) || ( _T("") == pFont->Path && ( oPrevFont.Name != pFont->Name || oPrevFont.Bold != pFont->Bold || oPrevFont.Italic != pFont->Italic ) ) )
						bFontChange = true;
					if ( fabs( oPrevFont.Size - pFont->Size ) > 0.001 )
						bSizeChange = true;
					if ( oPrevColor.unA != pColor->unA || oPrevColor.unB != pColor->unB || oPrevColor.unG != pColor->unG || oPrevColor.unR != pColor->unR )
						bColorChange = true;
					if ( fabs( dPrevCharSpacing - dCharSp ) > 0.001  )
						bCharSpChange = true;
				}

				// Запоминаем настройки
				oPrevColor       = *pColor;
				oPrevFont        = *pFont;
				pPrevToUnicode   = pToUnicode;
				dPrevCharSpacing = dCharSp;

				m_oFont = *pFont;
                bool bOldBold     = m_oFont.Bold;
                bool bOldItalic   = m_oFont.Italic;

				if ( bSizeChange )
					fFontSize = (float)m_oFont.Size;

				if ( bFontChange )
				{
					

					// Запоминаем значения Bold и Italic, потому что в функции GdiFontToPdfFont они могут измениться

					HRESULT hRes = S_OK;
					CString strUnicodeName = CString(pToUnicode->sName);
                    if ( S_OK != ( hRes = GdiFontToPdfFont2( strUnicodeName, _T("")) ) )
					{
						if ( AVS_OFFICEPDFWRITER_ERROR_FONT_NOT_FOUND == hRes )
						{
							ResetError( m_pDocument );
							sCodedString = std_string2string( pText->sText );
                            bDefaultFont = true;
						}
						else
						{
							continue;
						}
					}
					else
					{
                        bDefaultFont = false;

						if ( bWasHorScale )
						{
							// Отменяем горизонтальное сжатие/растяжение
                            if ( S_OK != ( SetHorizontalScalling( 100 ) ) )
								continue;

							bWasHorScale = false;
						}
					}

					if ( !m_oFont.Italic )
						dItalicAngle = 0;
					else
						dItalicAngle = 3.141592f / 12;

                    if ( S_OK != ( GetFontAscent( &nAscent ) ) )
						continue;

                    if ( S_OK != ( GetFontDescent( &nDescent ) ) )
						continue;

                    if ( S_OK != ( GetFontXHeight( &nXHeight ) ) )
						continue;

					fXHeight = (float)nXHeight / 1000;
					fAscent  = (float)nAscent / 1000;
					fDescent = fabs( (float)nDescent / 1000 );

					// Данная ветка сделана специально для конвертации Djvu -> PDF
					if ( _T("AVSEmptyFont") == pFont->Name )
					{						
						float fVKoef;

						if ( fabs( pFont->Size ) < 0.01 )
							fVKoef = 1;
						else		
							fVKoef = pText->fHeight / pFont->Size;

						// Если данный фонт не нашли с атрибутом Italic, тогда наколняем текст сами на угол dItalicAngle
                        if ( S_OK != ( SetTextMatrix( 1, (float)tan( 0.0 ), (float)fVKoef * tan( dItalicAngle ), fVKoef * 1, 0, 0 ) ) )
							continue;
					}
					else
					{
						// Если данный фонт не нашли с атрибутом Italic, тогда наколняем текст сами на угол dItalicAngle
                        if ( S_OK != ( SetTextMatrix( 1, (float)tan( 0.0 ), (float)tan( dItalicAngle ), 1, 0, 0 ) ) )
							continue;
					}
				}

				if ( bColorChange )
				{
					fR = ( pColor->unR ) / 255.0f; 
					fG = ( pColor->unG ) / 255.0f; 
					fB = ( pColor->unB ) / 255.0f; 

					// Выставляем цвет текста и линий
                    if ( S_OK != ( SetRGBFill( fR, fG, fB ) ) )
						continue;

                    if ( S_OK != ( SetRGBStroke( fR, fG, fB ) ) )
						continue;
				}

				if ( bCharSpChange )
				{
					// Устанавливаем межсимвольный интервал
                    if ( S_OK != ( SetCharSpace( (float)dCharSp ) ) )
						continue;
				}

				if ( bFontChange || bSizeChange )
				{
					// Устанавливаем текущий шрифт и размер шрифта
                    if ( S_OK != ( SetFontAndSizeToGState( fFontSize ) ) )
						continue;
				}


				// Если шрифт не нашли, и используется стандартный шрифт, значит, ширина слов будет неверной.
				// Чтобы слова не наползали друг на друга скейлим текст к его реальной ширине.
				if ( bDefaultFont )
				{
					bWasHorScale = true;

					// Измеряем размер
					float fTextWidth = 0.0f;
                    if ( S_OK != ( GetTextWidth( sCodedString, &fTextWidth ) ) )
					{
						continue;
					}

					float fKoef;
					if ( fabs( fTextWidth ) < 0.01 )
						fKoef = 1;
					else
						fKoef = pText->fWidth / fTextWidth * 100; // Scale задается в процентах

					if ( fKoef > 0.001 )
					{
                        if ( S_OK != ( SetHorizontalScalling( fKoef ) ) )
						{
							continue;
						}
					}
				}

				// Выводим текст
                if ( S_OK != ( TextOut_( (float)pText->fX, fPageHeight - (float) (pText->fY + pText->fBaseLineOffset), sCodedString ) ) )
				{
					continue;
				}



				if ( bFontChange )
				{
					m_oFont.Bold = bOldBold;
					m_oFont.Italic = bOldItalic;
				}
			}

			// Заканчиваем запись текста
            if ( S_OK != ( EndText() ) )
                return false;

			// Восстанавливаем систему координат
            //ATLTRACE2( _T("GRestore: DrawText\n") );
            if ( S_OK != ( GRestore() ) )
                return false;

			// Восстанавливаем старые настройки
            if ( S_OK != ( SetLineWidth( fOldLineWidth ) ) )
                return false;

            if ( S_OK != ( SetRGBFill( fFillR, fFillG, fFillB ) ) )
                return false;

            if ( S_OK != ( SetRGBStroke( fStrokeR, fStrokeG, fStrokeB ) ) )
                return false;

			// Восстанавливаем текущий шрифт
			m_oFont = CurFont;
		}

		if ( rendstateText == m_oRendererState.get_State() && m_oRendererState.IsMatrixChanged() )
			m_oContiniousText.LeaveLast();
		else
			m_oContiniousText.Reset();
	}

    return true;
}
bool CPdfWriterLib::ApplyTileFill()
{
    //todooo проверить
	Aggplus::CImage image(m_oBrush.TexturePath);

    if (image.m_Status != Aggplus::Ok) return false;
	
	LONG width  =	image.m_dwWidth;
	LONG height	=	image.m_dwHeight;

    m_bUseImageTextureAlpha		=	true;

	if ((m_bUseImageTextureAlpha || m_bIsWhiteBackImage))
	{
		BYTE* buffer	=	image.GetData();

		LONG lCountPix = width * height;

		if (m_bIsWhiteBackImage)
		{
			while (lCountPix > 0)
			{
				if (0 == buffer[3])
					memset(buffer, 0xFF, 4);

				--lCountPix;
				buffer += 4;
			}
		}
		else if (m_bUseImageTextureAlpha)
		{
			double alpha =	((double)m_oBrush.TextureAlpha / 255.0);

			while (lCountPix > 0)
			{
				buffer[3] *= alpha;
				--lCountPix;
				buffer += 4;
			}
		}
	}

    m_bUseImageTextureAlpha	=	false;

    if (LoadImageFromInterface(&image, true) == S_OK)
	{
		if (m_pCurrentXObject)
		{
			int RefImageId = m_pCurrentXObject->pHeader.nObjId;

			PDF::Rect pageBounds	=	PageBounds();		
			PDF::Rect pathBounds	=	PathBounds();

			PDF::TilePattern* shading = new PDF::TilePattern();
			if (shading)
			{
				// create pdf ref index
				
				Proxy ref = GenXRef<PDF::TilePattern>(shading, OCLASS_PATTERN_REF);

				// настройка заливки

				shading->SetRefIdX(RefImageId & (~OTYPE_INDIRECT));
				shading->SetBox(PDF::Rect(0.0f, 0.0f, floor ( MMToPDFCoords(PxToMM(width)) ), floor ( MMToPDFCoords(PxToMM(height))) ) );
				shading->SetId(ref->pHeader.nObjId & (~OTYPE_INDIRECT));
				shading->SetTransform(PDF::Matrix(m_oCommandParams.oMatrix.fA, m_oCommandParams.oMatrix.fC,
					MMToPDFCoords(m_oCommandParams.oMatrix.fE), 
					m_oCommandParams.oMatrix.fB, m_oCommandParams.oMatrix.fD,
					MMToPDFCoords(m_oCommandParams.oMatrix.fF),
					0, 0, 1));

				shading->CalculateCoords(pathBounds, pageBounds.Height);

				m_oPatternState.SetTiling(shading);					
				m_oPatterns.Add(shading);

				RebuildResources();

				// заливка для фигуры

				CString sVal;
				sVal.Format(L"/P%d scn\012", shading->GetRefIndex());

				PageAttr attr = (PageAttr)m_pDocument->pCurPage->pAttr;
				if (attr)
				{
					StreamWriteStr(attr->pStream, "/Pattern cs\012");
					StreamWriteStr(attr->pStream, CStringA(sVal));
				}

				m_oPatternState.SetFill(NULL);
				
				image.Destroy();
                return true;
			}
		}
	}

	image.Destroy();

    return false;
}
