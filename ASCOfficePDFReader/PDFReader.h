// PDFReader.h : Declaration of the CPDFReader

#pragma once
#include "resource.h"       // main symbols

#include "../Common/OfficeFileErrorDescription.h"
#include "../Common/OfficeDefines.h"

#include "PDFDoc.h"
#include "GlobalParams.h"
#include "ErrorConstants.h"
#include "ExtractImageOutputDev.h"
#include "RendererOutputDev.h"

#include "ImageOutputDev.h"
#include "SBitmap.h"

//#define   AVSGraphics                 AVSOfficeEditor
//#define _IAVSDocumentPainterEvents  _IAVSOfficeViewerEvents 
//#define  IAVSDocumentPainter         IAVSOfficeViewer
//#define  IAVSDocumentRenderer        IAVSCommandsRenderer
//#define _IAVSDocumentRendererEvents _IAVSCommandsRendererEvents
//#define  CAVSDocumentPainter         CAVSOfficeViewer

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


//#define _MEMORY_DEBUG
#ifdef _MEMORY_DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)

#define new DEBUG_NEW

#endif

// IPDFReader
[
	object,
	uuid("908EA939-E01D-47E0-B182-625826C2D7B1"),
	dual,	helpstring("IPDFReader Interface"),
	pointer_default(unique)
]
__interface IPDFReader : IDispatch
{

	[id(10)]  HRESULT LoadPDF([in]BSTR bsSrcPath, [in]BSTR bsOwnerPassword = NULL, [in]BSTR bsUserPassword = NULL, [in]BSTR bsOptions = NULL);
	[id(11)]  HRESULT Close();

	[id(100)] HRESULT GetPagesCount([out, retval]int *pnCount);
	[id(101)] HRESULT GetPDFVersion([out, retval]double *pdVersion);
	[id(102)] HRESULT GetPDFPermissions([out, retval]int *pnPermissions);
	[id(103)] HRESULT GetPDFInfo([out, retval]BSTR *pbsInfo);
	[id(104)] HRESULT SetTempFolder([in] BSTR bsTempFolder);
	[id(105)] HRESULT GetTempFolder([out, retval] BSTR *pbsTempFolder);
	
	[id(200)] HRESULT ExtractImages([in]BSTR bsDstPath, [in]BSTR bsPrefix);
	[id(201)] HRESULT DisplayPage([in]int nPageIndex, [in]BSTR bsPath, [in]BSTR bsXmlOptions, [out]double *pdWidth, [out]double *pdHeight, [in]BOOL *pbBreak);
	[id(202)] HRESULT DisplayAllPages([in]BSTR bsPath, [out]BSTR *pbsDstXml, [in]BSTR bsXmlOption, [in]BOOL *pbBreak);
	[id(203)] HRESULT DisplayPage2([in]IUnknown *punkRenderer, [in]int nPageIndex, [in]BSTR bsPath, [in]BSTR bsXmlOptions, [out]double *pdWidth, [out]double *pdHeight, [in]BOOL *pbBreak);

	[id(300)] HRESULT GetCommandRenderer([out, retval] IUnknown** ppRenderer);
	[id(301)] HRESULT SetCommandRenderer([in] IUnknown* pRenderer);

	[id(10001)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in]  VARIANT   ParamValue);
	[id(10002)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out] VARIANT *	ParamValue);
};



// CPDFReader

[
	coclass,
	default(IPDFReader),
	threading(apartment),
	vi_progid("AVSOfficePDFReader.PDFReader"),
	progid("AVSOfficePDFReader.PDFReader.1"),
	version(1.0),
	uuid("A329E783-2E52-4C96-A032-ADD2A5DADD0D"),
	helpstring("PDFReader Class")
]
class ATL_NO_VTABLE CPDFReader :
	public IPDFReader
{
private:

	PDFDoc                    *m_pPDFDocument;
	GlobalParams              *m_pGlobalParams;
	AVSGraphics::IASCRenderer *m_piDocumentRenderer;
	CStringW                   m_sTempFolder;
	CFontList                  m_oFontList;


public:
	CPDFReader(): m_piDocumentRenderer(NULL)
	{
	}



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
#ifdef _MEMORY_DEBUG
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW );
#endif

		m_sTempFolder   = _T(""); 
		m_pPDFDocument  = NULL;
		m_oFontList.Clear();

		m_pGlobalParams = new GlobalParams( "" );
		m_pGlobalParams->SetCMapsDir();

		return S_OK;
	}

	void FinalRelease()
	{
		m_oFontList.Clear();

		if ( m_pPDFDocument )
			delete m_pPDFDocument;

		if ( m_pGlobalParams )
		{
			delete m_pGlobalParams;
			m_pGlobalParams = NULL;
		}


		RELEASEINTERFACE( m_piDocumentRenderer );
	}

public:

	STDMETHOD(LoadPDF)(BSTR bsSrcPath, BSTR bsOwnerPassword, BSTR bsUserPassword, BSTR bsOptions)
	{
		USES_CONVERSION;

		if ( m_pPDFDocument )
			delete m_pPDFDocument;

		StringExt *seOwner = NULL, *seUser = NULL;
		//char *sSrcPath = W2A(bsSrcPath);
		char *sOwnerPassword = W2A(bsOwnerPassword);
		char *sUserPassword = W2A(bsUserPassword);
		if ( sOwnerPassword )
			seOwner = new StringExt( sOwnerPassword );
		if ( sUserPassword )
			seUser = new StringExt( sUserPassword );

		CString sOptions( bsOptions );

		m_pPDFDocument = new PDFDoc( m_pGlobalParams, CString( bsSrcPath ), seOwner, seUser );
		if ( !m_pPDFDocument || !m_pPDFDocument->CheckValidation() )
		{
			// Специальная ветка, для случаев, когда файл очень большой (около 1GB)
			// и функция MapViewOfFile не срабатывает
			if ( m_pPDFDocument )
				delete m_pPDFDocument;

			m_pPDFDocument = new PDFDoc( m_pGlobalParams, bsSrcPath, CString( bsSrcPath ).GetLength(), seOwner, seUser );
		}

		m_oFontList.Clear();

		if ( sOptions != _T("CheckPassword") )
		{
			if ( _T("DontDeleteTempFolder") != sOptions )
			{
				m_pGlobalParams->CreateTempFolder( bsSrcPath, m_sTempFolder );
				m_pGlobalParams->SetupBaseFonts( _T("C:\\Windows\\Fonts") );
			}
		}

		if ( seUser )
			delete seUser;
		if ( seOwner )
			delete seOwner;

		int nErrorCode = 0;
		if ( m_pPDFDocument && ( nErrorCode = m_pPDFDocument->GetErrorCode() ) == ErrorNone )
			return S_OK;
		else
		{
			switch( nErrorCode )
			{
			case ErrorOpenFile:
				return AVS_OFFICEPDFREADER_ERROR_OPEN_PDF;
			case ErrorBadCatalog:
				return AVS_OFFICEPDFREADER_ERROR_BAD_CATALOG;
			case ErrorDamaged:
				return AVS_OFFICEPDFREADER_ERROR_DAMAGED_PDF;
			case ErrorEncrypted:
				return AVS_OFFICEPDFREADER_ERROR_ENCRYPTED_PDF;
			default:
				return AVS_OFFICEPDFREADER_ERROR_OPEN_PDF;
			}
		}
	}
	STDMETHOD(Close)()
	{
		if ( m_pPDFDocument )
		{
			//if ( m_pGlobalParams )
			//	m_pGlobalParams->DeleteTempFolder();

			delete m_pPDFDocument;
			m_pPDFDocument = NULL;
		}

		return S_OK;
	}
	STDMETHOD(GetPagesCount)(int *pnCount)
	{
		if ( !m_pPDFDocument )
		{
			*pnCount = 0;
			return S_OK;
		}
		*pnCount = m_pPDFDocument->GetPagesCount();
		return S_OK;
	}
	STDMETHOD(GetPDFVersion)(double *pdVersion)
	{
		if ( !m_pPDFDocument )
		{
			*pdVersion = 0;
			return S_OK;
		}
		*pdVersion = m_pPDFDocument->GetPDFVersion();
		return S_OK;
	}
	STDMETHOD(GetPDFPermissions)(int *pnPermissions)
	{
		if ( !m_pPDFDocument )
		{
			*pnPermissions = 0;
			return S_FALSE;
		}

		*pnPermissions = 0;
		
		if ( m_pPDFDocument->CheckPrint() )
			*pnPermissions += PERMISSION_PRINT;
		if ( m_pPDFDocument->CheckCopy() )
			*pnPermissions += PERMISSION_COPY;
		if ( m_pPDFDocument->CheckChange() )
			*pnPermissions += PERMISSION_CHANGE;
			

		return S_OK;
	}
	STDMETHOD(GetPDFInfo)(BSTR *pbsInfo)
	{
		return S_OK;
	}
	STDMETHOD(SetTempFolder)(BSTR bsTempFolder)
	{
		m_sTempFolder = CStringW( bsTempFolder );
		return S_OK;
	}
	STDMETHOD(GetTempFolder)(BSTR *pbsTempFolder)
	{
		if ( m_pGlobalParams )
		{
			*pbsTempFolder = ::SysAllocString( (OLECHAR*)m_pGlobalParams->GetTempFolder() );
		}
		return S_OK;
	}
	STDMETHOD(ExtractImages)(BSTR bsDstPath, BSTR bsPrefix)
	{
		CStringA sPath(bsDstPath);

		ExtractImageOutputDev *pOutputDev = new ExtractImageOutputDev( m_pGlobalParams, sPath.GetBuffer(), TRUE );

		if ( !pOutputDev )
		{
			return AVS_OFFICEPDFREADER_ERROR_NOT_ENOUGH_MEMORY;
		}

		for ( int nIndex = 1; nIndex <= m_pPDFDocument->GetPagesCount(); nIndex++ )
		{
			m_pPDFDocument->DisplayPage( pOutputDev, nIndex, 72, 72, 0, FALSE, FALSE, FALSE  );
		}

		delete pOutputDev;

		return S_OK;
	}

	STDMETHOD(DisplayPage)(int nPageIndex, BSTR bsPath, BSTR bsXmlOptions, double *pdWidth, double *pdHeight, BOOL *pbBreak)
	{
		const double c_dInch = 25.399; // Миллиметров в дюйме
		const double c_dXResolution = 154.0;
		const double c_dYResolution = 154.0;

		double dKoefX = c_dInch / c_dXResolution;
		double dKoefY = c_dInch / c_dYResolution;

		USES_CONVERSION;
		if ( !m_pPDFDocument )
			return S_OK;

		int nMode = 0; // 0 - пустое открытие, 1 - открытие с отрисовкой на Renderer
		CString sXmlOptions( bsXmlOptions );

		if ( _T("<OpenEmptyPage />") == sXmlOptions )
			nMode = 0;
		else 
			nMode = 1;

		if ( 0 == nMode )
		{
			*pdWidth  = PDFCoordsToMM( m_pPDFDocument->GetPageCropWidth( nPageIndex ) );
			*pdHeight = PDFCoordsToMM( m_pPDFDocument->GetPageCropHeight( nPageIndex ) );
		}
		else if ( 1 == nMode )
		{
			//#ifdef _DEBUG
			if ( m_piDocumentRenderer )
			{
				IASCRenderer *pRenderer = NULL;
				m_piDocumentRenderer->QueryInterface( __uuidof(AVSGraphics::IASCRenderer), (void**)&pRenderer  );

				RendererOutputDev oRendererOut( m_pGlobalParams, pRenderer, &m_oFontList );
				oRendererOut.NewPDF( m_pPDFDocument->GetXRef() );
				oRendererOut.SetBreak( pbBreak );
				m_pPDFDocument->DisplayPage( &oRendererOut, nPageIndex, 72.0, 72.0, 0 , FALSE, TRUE, FALSE );

				RELEASEINTERFACE( pRenderer );
			}
			//#else

			//SColor pPaperColor;
			//pPaperColor[0] = pPaperColor[1] = pPaperColor[2] = 0xff;
			//SImageOutputDev *pSImage = new SImageOutputDev( m_pGlobalParams, colorModeRGB8, 1, FALSE, pPaperColor );
			//pSImage->NewPDF( m_pPDFDocument->GetXRef() );
			//char pFile[512];
			//m_pPDFDocument->DisplayPage( pSImage, nPageIndex, c_dXResolution, c_dYResolution, 0, FALSE, TRUE, FALSE );

			//pSImage->GetBitmap()->WriteJPGFile( _T("C:\\1\\"), m_pGlobalParams->GetTempFolder() );

			//*pdHeight = (float)pSImage->GetBitmapHeight() * dKoefX;
			//*pdWidth  = (float)pSImage->GetBitmapWidth() * dKoefY;

			//delete pSImage;
			//#endif
		}
		return S_OK;
	}
	STDMETHOD(DisplayAllPages)(BSTR bsPath, BSTR *pbsDstXml, BSTR bsXmlOptions, BOOL *pbBreak)
	{
		//if ( !m_pPDFDocument )
		//	return S_OK;

		//SColor pPaperColor;
		//pPaperColor[0] = pPaperColor[1] = pPaperColor[2] = 0xff;
		//SImageOutputDev *pSImage = new SImageOutputDev( m_pGlobalParams, colorModeRGB8, 1, FALSE, pPaperColor );
		//pSImage->NewPDF( m_pPDFDocument->GetXRef() );
		//wchar_t wsFile[512];
		//for( int nIndex = 1; nIndex <= m_pPDFDocument->GetPagesCount(); nIndex++ )
		//{
		//	m_pPDFDocument->DisplayPage( pSImage, nIndex, 72, 72, 0, FALSE, TRUE, FALSE );
		//	CStringW wsPath( bsPath );
		//	_swprintf( wsFile, _T("\\%06d.jpeg"), nIndex );
		//	wsPath += wsFile;
		//	pSImage->GetBitmap()->WriteJPGFile( wsPath.GetBuffer(), m_pGlobalParams->GetTempFolder() );
		//}
		//delete pSImage;
		return S_OK;
	}
	STDMETHOD(DisplayPage2)(IUnknown *punkRenderer, int nPageIndex, BSTR bsPath, BSTR bsXmlOptions, double *pdWidth, double *pdHeight, BOOL *pbBreak)
	{
		const double c_dInch = 25.399; // Миллиметров в дюйме
		const double c_dXResolution = 154.0;
		const double c_dYResolution = 154.0;

		double dKoefX = c_dInch / c_dXResolution;
		double dKoefY = c_dInch / c_dYResolution;

		USES_CONVERSION;
		if ( !m_pPDFDocument )
			return S_OK;

		int nMode = 0; // 0 - пустое открытие, 1 - открытие с отрисовкой на Renderer
		CString sXmlOptions( bsXmlOptions );

		if ( _T("<OpenEmptyPage />") == sXmlOptions )
			nMode = 0;
		else 
			nMode = 1;

		if ( 0 == nMode )
		{
			int nRotate = m_pPDFDocument->GetPageRotate( nPageIndex );

			while ( nRotate >= 360 )
				nRotate -= 360;
		
			while ( nRotate < 0 )
				nRotate += 360;

			if ( 0 != nRotate && 180 != nRotate  )
			{
				*pdHeight = PDFCoordsToMM( m_pPDFDocument->GetPageCropWidth( nPageIndex ) );
				*pdWidth  = PDFCoordsToMM( m_pPDFDocument->GetPageCropHeight( nPageIndex ) );
			}
			else
			{
				*pdWidth  = PDFCoordsToMM( m_pPDFDocument->GetPageCropWidth( nPageIndex ) );
				*pdHeight = PDFCoordsToMM( m_pPDFDocument->GetPageCropHeight( nPageIndex ) );
			}
		}
		else if ( 1 == nMode )
		{
			//#ifdef _DEBUG
			if ( punkRenderer )
			{
				IASCRenderer *pRenderer = NULL;
				punkRenderer->QueryInterface( __uuidof(AVSGraphics::IASCRenderer), (void**)&pRenderer  );

				RendererOutputDev oRendererOut( m_pGlobalParams, pRenderer, &m_oFontList );
				oRendererOut.NewPDF( m_pPDFDocument->GetXRef() );
				oRendererOut.SetBreak( pbBreak );
				m_pPDFDocument->DisplayPage( &oRendererOut, nPageIndex, 72.0, 72.0, 0 , FALSE, TRUE, FALSE );

				RELEASEINTERFACE( pRenderer );
			}
			//#else

			//SColor pPaperColor;
			//pPaperColor[0] = pPaperColor[1] = pPaperColor[2] = 0xff;
			//SImageOutputDev *pSImage = new SImageOutputDev( m_pGlobalParams, colorModeRGB8, 1, FALSE, pPaperColor );
			//pSImage->NewPDF( m_pPDFDocument->GetXRef() );
			//char pFile[512];
			//m_pPDFDocument->DisplayPage( pSImage, nPageIndex, c_dXResolution, c_dYResolution, 0, FALSE, TRUE, FALSE );

			//pSImage->GetBitmap()->WriteJPGFile( _T("C:\\1\\out.jpg"), m_pGlobalParams->GetTempFolder() );

			//*pdHeight = (float)pSImage->GetBitmapHeight() * dKoefX;
			//*pdWidth  = (float)pSImage->GetBitmapWidth() * dKoefY;

			//delete pSImage;
			//#endif
		}
		return S_OK;
	}
	STDMETHOD(GetCommandRenderer)(IUnknown** ppRenderer)
	{
		if( NULL == ppRenderer )
			return S_FALSE;
		(*ppRenderer) = NULL;
		if( NULL == m_piDocumentRenderer )
			return S_OK;
		return m_piDocumentRenderer->QueryInterface(__uuidof(AVSGraphics::IASCRenderer), (void**)&ppRenderer);
	}
	STDMETHOD(SetCommandRenderer)(IUnknown* pRenderer)
	{
		RELEASEINTERFACE( m_piDocumentRenderer );
		if( NULL != pRenderer )
			pRenderer->QueryInterface(__uuidof( AVSGraphics::IASCRenderer), (void**)&m_piDocumentRenderer);
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT   ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT *	ParamValue)
	{
		if ( NULL == ParamValue )
			return S_FALSE;

		CString sParamName; 
		sParamName = ParamName;

		if ( _T("ImagesCount") == sParamName )
		{
			//ExtractImageOutputDev *pOutputDev = new ExtractImageOutputDev( m_pGlobalParams, NULL, TRUE, TRUE );

			//if ( !pOutputDev )
			//{
			//	return AVS_OFFICEPDFREADER_ERROR_NOT_ENOUGH_MEMORY;
			//}

			//for ( int nIndex = 1; nIndex <= m_pPDFDocument->GetPagesCount(); nIndex++ )
			//{
			//	m_pPDFDocument->DisplayPage( pOutputDev, nIndex, 72, 72, 0, FALSE, FALSE, FALSE  );
			//}

			//(*ParamValue).vt    = VT_I8;
			//(*ParamValue).llVal = pOutputDev->GetImagesCount();

			//delete pOutputDev;
		}

		return S_OK;
	}

};

