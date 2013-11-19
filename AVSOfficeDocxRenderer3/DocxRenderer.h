// DOCXWriter.h : Declaration of the CAVSDOCXWriter

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols

#include "Writers/Writer_Docx.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IAVSDOCXWriter
[object, uuid("089684E7-3A1B-4c49-A562-787887D5050D"), dual, pointer_default(unique)]
__interface IAVSOfficeDocxRenderer3 : IAVSRenderer
{
	[id(11001)]				HRESULT Initialize([in]BSTR bsXMLOptions);
	[id(11002)]				HRESULT SetMetadata([in]BSTR bsXMLOptions);

	[id(12001)]				HRESULT CreateOfficeFile([in]BSTR bstrFileName, [in] LONG lFileType);
	[id(12002)]				HRESULT CloseFile();

	[id(12003), propget]	HRESULT TempDirectory([out, retval]BSTR* pbstrFileName);
	[id(12003), propput]	HRESULT TempDirectory([in]BSTR bstrFileName);

	[id(12004), propget]	HRESULT SrcFileType([out, retval]LONG* plSrcFileType);
	[id(12004), propput]	HRESULT SrcFileType([in]LONG lSrcType);	

	[id(12500), propget] HRESULT CommandRenderer([out, retval] IUnknown** ppunkRend);
	[id(12500), propput] HRESULT CommandRenderer([in] IUnknown* punkRend);
};

// CAVSOfficeDocxRenderer3
[coclass, default(IAVSOfficeDocxRenderer3), threading(apartment), vi_progid("DocxRenderer.DocxRenderer"), progid("DocxRenderer.DocxRenderer.1"), version(1.0), uuid("47D3DD72-6411-4739-8B8F-86563F642191")]
class ATL_NO_VTABLE CAVSOfficeDocxRenderer3 :
	public IAVSOfficeDocxRenderer3, public IAVSRenderer2
{
private:
	NSStructures::CPen		m_oPen;							// настройки всей графики (скопирован ашник из AVSGraphics)
	NSStructures::CBrush	m_oBrush;
	NSStructures::CFont		m_oFont;
	NSStructures::CFormat	m_oFormat;
	NSStructures::CShadow	m_oShadow;
	NSStructures::CEdgeText	m_oEdge;

	AVSGraphics::IAVSGraphicSimpleComverter*	m_pSimpleGraphicsConverter;		// конвертер сложных гафических путей в простые
	AVSGraphics::IAVSFontManager*				m_pFontManager;					// менеджер шрифтов

	NSCommon::CMatrix			m_oBaseTransform;		// матрица перерасчета координатных осей (здесь: миллиметры -> пикселы)
	NSCommon::CMatrix			m_oTransform;			// текущая матрица преобразований рендерера
	NSCommon::CMatrix			m_oFullTransform;		// полная матрица преобразований (m_oBaseTransform * m_oTransform)

	double							m_dTransformAngle;

	LONG							m_lCurrentCommandType;	// текущая команда

	bool							m_bIsMetafileDrawing;

	double							m_dDpiX;				
	double							m_dDpiY;

	LONG							m_lClipMode;
	NSStructures::CFont				m_oInstalledFont;

	double							m_dWidth;
	double							m_dHeight;
	bool							m_bPageAdded;

	NSDocx::CWriter				m_oWriter;

	CRITICAL_SECTION m_oCS;

	CString m_strDstFilePath;
	CString m_strTempDirectory;

	LONG							m_lSrcType;
	LONG							m_lDstType;

	NSDocx::CRenderers*			m_pRenderers;

	DWORD							m_lCommandFlags;

	AVSGraphics::IAVSDocumentRenderer *	m_piCommandsRenderer;

public:
	CAVSOfficeDocxRenderer3()
	{
		#ifdef _DEBUG
		
		/*
		NSMobi::CMobiReader oReaderMobi;
		oReaderMobi.OpenFile(_T("C:\\PPTMemory\\Guide.mobi"));
		oReaderMobi.CloseFile();
		*/

		/*
		m_oWriter.m_oFormat.LoadFromFile(_T("C:\\PPTMemory\\test_format_input.xml"));
		m_oWriter.m_oFormat.SaveToFile(_T("C:\\PPTMemory\\test_format_output.xml"));
		m_lDstType = 1;
		CloseFile();
		*/

		#endif
	}

    DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_strDstFilePath	= _T("");
		m_strTempDirectory	= _T("");

		InitializeCriticalSection(&m_oCS);

		m_dDpiX = 96.0;
		m_dDpiY	= 96.0;

		m_lClipMode = c_nClipRegionTypeWinding;

		m_lCommandFlags = 0;

		m_pSimpleGraphicsConverter = NULL;
		CoCreateInstance(AVSGraphics::CLSID_CAVSGraphicSimpleComverter, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSGraphicSimpleComverter, (void**)&m_pSimpleGraphicsConverter);

		IUnknown* punkRenderer = NULL;
		this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

		m_pSimpleGraphicsConverter->put_Renderer(punkRenderer);
		RELEASEINTERFACE(punkRenderer);

		m_dTransformAngle = 0.0;
		m_bIsMetafileDrawing = false;
		m_pFontManager = NULL;

		m_pRenderers = NULL;
		m_piCommandsRenderer = NULL;

		return S_OK;
	}

	void FinalRelease()
	{
		CloseFile();

		RELEASEINTERFACE(m_piCommandsRenderer);
		DeleteCriticalSection(&m_oCS);
	}

public:
    // IAVSRenderer Methods

	// тип рендерера-----------------------------------------------------------------------------
	STDMETHOD(get_Type)(LONG* lType);

	//-------- Функции для работы со страницей --------------------------------------------------
	STDMETHOD(NewPage)();
	STDMETHOD(get_Height)(double* dHeight);
	STDMETHOD(put_Height)(double dHeight);
	STDMETHOD(get_Width)(double* dWidth);
	STDMETHOD(put_Width)(double dWidth);
	STDMETHOD(get_DpiX)(double* dDpiX);
	STDMETHOD(get_DpiY)(double* dDpiY);
	//-------- Функции для задания настроек текста ----------------------------------------------
	STDMETHOD(SetPen)(BSTR bsXML);

    STDMETHOD(get_PenColor)(LONG* lColor);
	STDMETHOD(put_PenColor)(LONG lColor);
	STDMETHOD(get_PenAlpha)(LONG* lAlpha);
	STDMETHOD(put_PenAlpha)(LONG lAlpha);
	STDMETHOD(get_PenSize)(double* dSize);
	STDMETHOD(put_PenSize)(double dSize);
	STDMETHOD(get_PenDashStyle)(BYTE* val);
	STDMETHOD(put_PenDashStyle)(BYTE val);
	STDMETHOD(get_PenLineStartCap)(BYTE* val);
	STDMETHOD(put_PenLineStartCap)(BYTE val);
	STDMETHOD(get_PenLineEndCap)(BYTE* val);
	STDMETHOD(put_PenLineEndCap)(BYTE val);
	STDMETHOD(get_PenLineJoin)(BYTE* val);
	STDMETHOD(put_PenLineJoin)(BYTE val);
	STDMETHOD(get_PenDashOffset)(double* val);
	STDMETHOD(put_PenDashOffset)(double val);
	STDMETHOD(get_PenAlign)(LONG* val);
	STDMETHOD(put_PenAlign)(LONG val);
	STDMETHOD(get_PenMiterLimit)(double* val);
	STDMETHOD(put_PenMiterLimit)(double val);
	STDMETHOD(PenDashPattern)(SAFEARRAY* pPattern);

	STDMETHOD(SetBrush)(BSTR bsXML);

	STDMETHOD(get_BrushType)(LONG* lType);
	STDMETHOD(put_BrushType)(LONG lType);
	STDMETHOD(get_BrushColor1)(LONG* lColor);
	STDMETHOD(put_BrushColor1)(LONG lColor);
	STDMETHOD(get_BrushAlpha1)(LONG* lAlpha);
	STDMETHOD(put_BrushAlpha1)(LONG lAlpha);
	STDMETHOD(get_BrushColor2)(LONG* lColor);
	STDMETHOD(put_BrushColor2)(LONG lColor);
	STDMETHOD(get_BrushAlpha2)(LONG* lAlpha);
	STDMETHOD(put_BrushAlpha2)(LONG lAlpha);
	STDMETHOD(get_BrushTexturePath)(BSTR* bsPath);
	STDMETHOD(put_BrushTexturePath)(BSTR bsPath);
	STDMETHOD(get_BrushTextureMode)(LONG* lMode);
	STDMETHOD(put_BrushTextureMode)(LONG lMode);
	STDMETHOD(get_BrushTextureAlpha)(LONG* lTxAlpha);
	STDMETHOD(put_BrushTextureAlpha)(LONG lTxAlpha);
	STDMETHOD(get_BrushLinearAngle)(double* dAngle);
	STDMETHOD(put_BrushLinearAngle)(double dAngle);
	STDMETHOD(BrushRect)(BOOL val, double left, double top, double width, double height);

	STDMETHOD(SetFont)(BSTR bsXML);

    STDMETHOD(get_FontName)(BSTR* bsName);
	STDMETHOD(put_FontName)(BSTR bsName);
	STDMETHOD(get_FontPath)(BSTR* bsName);
	STDMETHOD(put_FontPath)(BSTR bsName);
	STDMETHOD(get_FontSize)(double* dSize);
	STDMETHOD(put_FontSize)(double dSize);
	STDMETHOD(get_FontStyle)(LONG* lStyle);
	STDMETHOD(put_FontStyle)(LONG lStyle);
	STDMETHOD(get_FontStringGID)(BOOL* bGID);
	STDMETHOD(put_FontStringGID)(BOOL bGID);
	STDMETHOD(get_FontCharSpace)(double* dSpace);
	STDMETHOD(put_FontCharSpace)(double dSpace);

	STDMETHOD(SetShadow)(BSTR bsXML);

    STDMETHOD(get_ShadowDistanceX)(double* val);
	STDMETHOD(put_ShadowDistanceX)(double val);
	STDMETHOD(get_ShadowDistanceY)(double* val);
	STDMETHOD(put_ShadowDistanceY)(double val);
	STDMETHOD(get_ShadowBlurSize)(double* val);
	STDMETHOD(put_ShadowBlurSize)(double val);
	STDMETHOD(get_ShadowColor)(LONG* val);
	STDMETHOD(put_ShadowColor)(LONG val);
	STDMETHOD(get_ShadowAlpha)(LONG* val);
	STDMETHOD(put_ShadowAlpha)(LONG val);
	STDMETHOD(get_ShadowVisible)(BOOL* val);
	STDMETHOD(put_ShadowVisible)(BOOL val);

	STDMETHOD(SetEdgeText)(BSTR bsXML);

    STDMETHOD(get_EdgeVisible)(LONG* val);
	STDMETHOD(put_EdgeVisible)(LONG val);
	STDMETHOD(get_EdgeColor)(LONG* val);
	STDMETHOD(put_EdgeColor)(LONG val);
	STDMETHOD(get_EdgeAlpha)(LONG* val);
	STDMETHOD(put_EdgeAlpha)(LONG val);
	STDMETHOD(get_EdgeDist)(double* val);
	STDMETHOD(put_EdgeDist)(double val);

    //-------- Функции для вывода текста --------------------------------------------------------
	STDMETHOD(CommandDrawText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset);
	STDMETHOD(CommandDrawTextEx)(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags);
	//-------- Маркеры для команд ---------------------------------------------------------------
	STDMETHOD(BeginCommand)(DWORD lType);
	STDMETHOD(EndCommand)(DWORD lType);
    //-------- Функции для работы с Graphics Path -----------------------------------------------
	STDMETHOD(PathCommandMoveTo)(double fX, double fY);
	STDMETHOD(PathCommandLineTo)(double fX, double fY);
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints);
	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3);
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints);
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
	STDMETHOD(PathCommandClose)();
	STDMETHOD(PathCommandEnd)();
	STDMETHOD(DrawPath)(long nType);
	STDMETHOD(PathCommandStart)();
	STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY);

	STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset);
	STDMETHOD(PathCommandTextEx)(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags);
	
	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags);
	STDMETHOD(SetCommandParams)(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags);
    //-------- Функции для вывода изображений --------------------------------------------------
	STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight);
	STDMETHOD(DrawImageFromFile)(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight);
	//------------------------------------------------------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue);
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue);

    STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF);
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
	STDMETHOD(ResetTransform)(void);
	STDMETHOD(get_ClipMode)(LONG* plMode);
	STDMETHOD(put_ClipMode)(LONG lMode);

	// AVSRenderer2
	STDMETHOD(CommandLong)(LONG lCommand);
	STDMETHOD(CommandDouble)(double dCommand);
	STDMETHOD(CommandString)(BSTR bsCommand);
	STDMETHOD(CommandVariant)(VARIANT vCommand);

	STDMETHOD(CommandLong2)(LONG lType, LONG lCommand);
	STDMETHOD(CommandDouble2)(LONG lType, double dCommand);
	STDMETHOD(CommandString2)(LONG lType, BSTR bsCommand);
	STDMETHOD(CommandVariant2)(LONG lType, VARIANT vCommand);

public:
    //IAVSDocxRenderer Methods
    STDMETHOD(Initialize)(BSTR bsXMLOptions);
	STDMETHOD(SetMetadata)(BSTR bsXMLOptions);
	STDMETHOD(CreateOfficeFile)(BSTR bsFileName, LONG lFileType);
    STDMETHOD(CloseFile)();

	STDMETHOD(get_CommandRenderer)(IUnknown** ppunkRend)
	{
		if(NULL == ppunkRend)
			return S_FALSE;
		(*ppunkRend) = NULL;
		if(NULL == m_piCommandsRenderer)
			return S_OK;
		return m_piCommandsRenderer->QueryInterface(__uuidof(AVSGraphics::IAVSDocumentRenderer), (void**)&ppunkRend);
	}
	STDMETHOD(put_CommandRenderer)(IUnknown* ppunkRend)
	{
		RELEASEINTERFACE(m_piCommandsRenderer);
		if(NULL != ppunkRend)
			ppunkRend->QueryInterface(__uuidof( AVSGraphics::IAVSDocumentRenderer), (void**)&m_piCommandsRenderer);
		return S_OK;
	}

	void BaseInitialize(BSTR bsXMLOptions)
	{
		XmlUtils::CXmlNode oNode;
		if (oNode.FromXmlString((CString)bsXMLOptions))
		{
			XmlUtils::CXmlNode oNodeDocx;
			if (oNode.GetNode(_T("DocxOptions"), oNodeDocx))
			{
				m_oWriter.m_bIsSaveCover		= (1 == oNodeDocx.ReadValueInt(_T("SaveCover"), 0));
				m_oWriter.m_bIsEmbeddedFonts	= (1 == oNodeDocx.ReadValueInt(_T("FontEmbed"), 0));

				m_oWriter.m_bIsSaveCover		= !m_oWriter.m_bIsSaveCover;
				m_oWriter.m_bIsEmbeddedFonts	= !m_oWriter.m_bIsEmbeddedFonts;
			}
		}
	}

	STDMETHOD(put_TempDirectory)(BSTR bsFileName)
	{
		m_strTempDirectory = (CString)bsFileName;
		return S_OK;
	}
	STDMETHOD(get_TempDirectory)(BSTR* pbsFileName)
	{
		if (NULL == pbsFileName)
			return S_FALSE;

		*pbsFileName = m_strTempDirectory.AllocSysString();
		return S_OK;
	}

	STDMETHOD(put_SrcFileType)(LONG lType)
	{
		m_lSrcType = lType;
		return S_OK;
	}
	STDMETHOD(get_SrcFileType)(LONG* plType)
	{
		if (NULL == plType)
			return S_FALSE;

		*plType = m_lSrcType;
		return S_OK;
	}

	AVSINLINE void SendBeginCommandsToFlags()
	{
		if (c_nHeader == m_lCurrentCommandType)
			m_lCommandFlags += 100;
		if (c_nTextGraphicType == m_lCurrentCommandType)
			m_lCommandFlags += 1000;
		if (c_nTable == m_lCurrentCommandType)
			m_lCommandFlags += 10000;
		if (c_nFootnote == m_lCurrentCommandType)
			m_lCommandFlags += 100000;
	}
	AVSINLINE void SendEndCommandsToFlags(LONG lType)
	{
		if (c_nHeader == lType)
			m_lCommandFlags -= 100;
		if (c_nTextGraphicType == lType)
			m_lCommandFlags -= 1000;
		if (c_nTable == lType)
			m_lCommandFlags -= 10000;
		if (c_nFootnote == lType)
			m_lCommandFlags -= 100000;
	}
	AVSINLINE bool CheckNeedCommands()
	{
		//return true;
		return (0 == m_lCommandFlags) ? true : false;
	}

private:
	AVSINLINE void ApplyTransform(double d1, double d2, double d3, double d4, double d5, double d6)
	{
		m_oTransform.SetElements(d1, d2, d3, d4, d5, d6);
		m_dTransformAngle = m_oTransform.z_Rotation();
	}

	void ApplyTransform2(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
	{
		if ((dWidth <= 1) || (dHeight <= 1))
			lFlags = 0;

		BOOL bFlipX = (0 != (c_nParamFlipX & lFlags));
		BOOL bFlipY = (0 != (c_nParamFlipY & lFlags));
		
		double m11 = bFlipX ? -1.0 : 1.0;
		double m22 = bFlipY ? -1.0 : 1.0;
		
		NSCommon::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

		if ((0 != dAngle) || (0 != lFlags))
		{
			double dCentreX = (dLeft + dWidth / 2.0);
			double dCentreY = (dTop + dHeight / 2.0);
			
			oMatrix.Translate(-dCentreX, -dCentreY	, Aggplus::MatrixOrderAppend);
			
			oMatrix.Rotate(dAngle			, Aggplus::MatrixOrderAppend);
			oMatrix.Scale(m11, m22					, Aggplus::MatrixOrderAppend);
			
			oMatrix.Translate(dCentreX, dCentreY	, Aggplus::MatrixOrderAppend);
		}

		m_oTransform = oMatrix;
		m_dTransformAngle = m_oTransform.z_Rotation();
	}

	void _SetFont()
	{
		if (NULL == m_pFontManager)
		{
			CoCreateInstance(__uuidof(AVSGraphics::CAVSFontManager), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IAVSFontManager), (void**)&m_pFontManager);
			m_pFontManager->Initialize(L"");
		}

		double dPix = m_oFont.CharSpace * m_dDpiX / 25.4;
		
		if (m_oInstalledFont.IsEqual(&m_oFont))
		{
			if (1 < m_dWidth)
			{
				m_pFontManager->SetCharSpacing(dPix);
			}
			return;
		}

		m_pFontManager->SetStringGID(m_oFont.StringGID);
		if (1 < m_dWidth)
		{
			m_pFontManager->SetCharSpacing(dPix);
		}

		if (_T("") == m_oFont.Path)
		{
			BSTR bsName = m_oFont.Name.AllocSysString();
			m_pFontManager->LoadFontByName(bsName, (float)m_oFont.Size, m_oFont.GetStyle(), m_dDpiX, m_dDpiY);
			SysFreeString(bsName);
		}
		else
		{
			BSTR bsName = m_oFont.Path.AllocSysString();
			m_pFontManager->LoadFontFromFile(bsName, (float)m_oFont.Size, m_dDpiX, m_dDpiY, 0);
			SysFreeString(bsName);
		}

		m_oInstalledFont = m_oFont;
	}

	AVSINLINE void MoveTo(double& x, double& y)
	{
		m_oWriter.MoveTo(x, y);
	}
	AVSINLINE void LineTo(double& x, double& y)
	{
		m_oWriter.LineTo(x, y);
	}
	AVSINLINE void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
	{
		m_oWriter.CurveTo(x1, y1, x2, y2, x3, y3);
	}
	AVSINLINE void Close()
	{
		m_oWriter.Close();
	}

	AVSINLINE void PathStart()
	{
		m_oWriter.BeginPath();
	}
	AVSINLINE void PathEnd()
	{
		m_oWriter.EndPath();
	}

	// ----------------------------
	bool SaveCover()
	{
		if (NULL != m_piCommandsRenderer)
		{
			long nPageCount = 0;
			m_piCommandsRenderer->get_PageCount(&nPageCount);
			if (nPageCount > 0)
			{
				double dWidthMM = 0;
				double dHeightMM = 0;
				m_piCommandsRenderer->GetPageSize( 0, &dWidthMM, &dHeightMM );
				//
				LONG nHeight = 0;
				LONG nWidth = 0;
				if( 0 != dHeightMM )
				{
					nHeight = 1186;
					nWidth = nHeight * dWidthMM / dHeightMM;
				}
				MediaCore::IAVSUncompressedVideoFrame* piImage = NULL;
				AVSGraphics::IAVSGraphicsRenderer* piGraphicsRenderer = NULL;
				UpdateGdiPlusRenderer( dWidthMM, dHeightMM, &piImage, &piGraphicsRenderer );
				if( NULL != piGraphicsRenderer )
				{
					piGraphicsRenderer->NewPage();
					m_piCommandsRenderer->DrawPage( 0, piGraphicsRenderer );

					IUnknown* punkFrame = NULL;
					piImage->QueryInterface(IID_IUnknown, (void**)&punkFrame);
					this->DrawImage(punkFrame, 0, 0, dWidthMM, dHeightMM);
					RELEASEINTERFACE(punkFrame);
				}

				RELEASEINTERFACE(piGraphicsRenderer);
				RELEASEINTERFACE(piImage);
			}
		}
		return false;
	}

	void UpdateGdiPlusRenderer(double dWidthMm, double dHeightMm, MediaCore::IAVSUncompressedVideoFrame** piImage, AVSGraphics::IAVSGraphicsRenderer** piRend)
	{
		(*piImage) = NULL;
		(*piRend) = NULL;
		CoCreateInstance(__uuidof( MediaCore::CAVSUncompressedVideoFrame), NULL ,CLSCTX_INPROC_SERVER, __uuidof(MediaCore::IAVSUncompressedVideoFrame), (void **)piImage);
		AVSGraphics::IAVSFontManagerPtr piFontManager;
		piFontManager.CreateInstance( __uuidof(AVSGraphics::CAVSFontManager) );
		piFontManager->Initialize( L"" );

		//задаем dpi без привязки к монитору
		int nLOGPIXELSX = 96;
		int nLOGPIXELSY = 96;

		//размер исходной картинки
		long nWidthPix = (long)(nLOGPIXELSX * dWidthMm / (25.4));
		long nHeightPix = (long)(nLOGPIXELSY * dHeightMm / (25.4));

		if (nHeightPix > c_nMaxImageSize)
		{
			nWidthPix = (LONG)((double)nWidthPix * c_nMaxImageSize / nHeightPix);
			nHeightPix = c_nMaxImageSize;
		}

		(*piImage)->put_Width(nWidthPix);
		(*piImage)->put_Height(nHeightPix);
		(*piImage)->put_Stride(0, nWidthPix * 4);

		(*piImage)->put_AspectRatioX(nWidthPix);
		(*piImage)->put_AspectRatioY(nHeightPix);

		(*piImage)->put_DataSize(4 * nWidthPix * nHeightPix);

		(*piImage)->put_ColorSpace(64 + (1 << 31));
		(*piImage)->AllocateBuffer(-1);

		BYTE* pBuffer = NULL;
		(*piImage)->get_Buffer(&pBuffer);
		memset(pBuffer, 255, 4 * nWidthPix * nHeightPix);

		CoCreateInstance(__uuidof( AVSGraphics::CAVSGraphicsRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IAVSGraphicsRenderer), (void **)piRend);
		//ставим FontManager
		VARIANT vtVariant;
		vtVariant.vt = VT_UNKNOWN;
		vtVariant.punkVal = piFontManager;
		(*piRend)->SetAdditionalParam( L"FontManager", vtVariant );

		(*piRend)->put_Width(dWidthMm);
		(*piRend)->put_Height(dHeightMm);
		(*piRend)->CreateFromMediaData(*piImage, 0, 0, nWidthPix, nHeightPix );
	}
};

