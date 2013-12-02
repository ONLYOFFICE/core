// AVSOfficeFile.h : Declaration of the CAVSOfficeFile

#pragma once
#include "resource.h"       // main symbols
#include "..\Common\OfficeFileErrorDescription.h"
#include "..\Common\OfficeDefines.h"
#include "..\AVSOfficeUniversalConverter\Utils.h"
#include "XmlUtils.h"
#include "FormatChecker.h"
#include "OfficeFileEvents.h"
#include "..\Common\TemporaryCS.h"
#include "OfficeFileConverter.h"
// шифрование XML
#include "../Common/RSA/XMLDecoder.h"
#include <math.h>

#define PIX_TO_MM( Value, DPI )		float( 1.0 * Value * (10 * ONE_INCH ) / DPI )

#define PROGRESS_LOAD 0.5
#define PROGRESS_COMMAND 0.5

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


#define STRING_TO_BOOL( sArg, bRes )\
	if( _T("") != sArg )\
	{\
		sArg.MakeLower();\
		if( _T("0") == sArg || _T("false") == sArg )\
			bRes = false;\
		else\
			bRes = true;\
	}


// _IAVSOfficeFileTemplateEvents
[
	dispinterface,
	uuid("2c0401b0-bfa6-11de-8a39-0800200c9a66"),
	helpstring("_IAVSOfficeFileTemplateEvents Interface")
]
__interface _IAVSOfficeFileEvents
{
	////Max Value nPercent == 1000000
	////Example 23,56 % == 235600
	//[id(1), helpstring("method OnProgress")] HRESULT OnProgress([in] LONG nID, [in] LONG nPercent);

	[id(0)] HRESULT OnError(LONG lError);
	[id(1)] HRESULT OnCompleteParseFile();

	[id(5)] HRESULT OnProgressParseFile([in] LONG nID, LONG lProgressPage, [in, out, ref] SHORT* Cancel);
	[id(6)] HRESULT OnProgressParsePage(LONG lProgress);
	[id(7)] HRESULT OnCompleteParsePage();
};

// CAVSOfficeFile
[
	coclass,
	default(IAVSOfficeFile, _IAVSOfficeFileEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeFile.AVSDocume"),
	progid("AVSOfficeFile.AVSDocu.1"),
	version(1.0),
	uuid("69cd4f40-bf08-11de-8a39-0800200c9a66"),
	helpstring("AVSOfficeFile Class")
]
class ATL_NO_VTABLE CAVSOfficeFile : public IAVSOfficeFile
{
private: long m_nRendererError; //ошибка при рендринге
private: HANDLE m_hViewerStop; //окончание рендеринга в CommandRenderer

private: AVSGraphics::IAVSDocumentPainter* m_piOfficeViewer;
//private: AVSGraphics::IAVSDocumentRenderer* m_piWatermarkRenderer;
private: IAVSOfficeFileConverter* m_piFileConverter;
private: COfficeEventViewer< AVSGraphics::_IAVSDocumentPainterEvents, CAVSOfficeFile >* m_oEventViewer ;

private: CString m_sFileInfo;
private: bool m_bIsEdited;

private: CString m_sFilename;

private: CString m_sTitle;
private: CString m_sSubject;
private: CString m_sAuthor;
private: CString m_sKeywords;
private: CString m_sComments;
private: CString m_sTemplate;
private: CString m_sLastAuthor;
private: CString m_sRevisionNumber;
private: CString m_sAppName;
private: CString m_sCategory;
private: CString m_sSource;

private: int m_nWidth;
private: int m_nHeight;
private: int m_nResizeMode;
private: long m_nMaxFilesCount;
private: CString m_sWatermark;
private: CString m_sPassword;
private: CRITICAL_SECTION m_csThread;

private: int m_nLOGPIXELSX;
private: int m_nLOGPIXELSY;

private: CXMLDecoder m_oDecoder;
private: bool m_bPdfLoad;
private: bool m_bStopParse;
private: bool m_bParseFileComplete;
private: long m_nPermissions;
private: long m_nStatus;
private: CAtlMap<long, CString> m_mapThumbnails;
private: CString m_sThumbnailsPath;
private: CString m_sTempPath;
private: CString m_sOpenOptions;
private: long m_nLastProgress;
private: int m_nImagesCount;
private: int m_nLastError;
private: CAtlArray<CString> m_aRelsWalked;
private: CAtlArray<CString> m_aImagesToExtract;
public:
	CAVSOfficeFile():m_piFileConverter(NULL),m_piOfficeViewer(NULL)
	{
		m_nMaxFilesCount = 0;
		m_nResizeMode = 1;
		m_nWidth = -1;
		m_nHeight = -1;
		m_bStopParse = false;
		m_sPassword = _T("");
		m_nStatus = c_shOfficeFileIdle;
		CoCreateInstance( __uuidof(CAVSOfficeFileConverter), NULL ,CLSCTX_INPROC_SERVER, __uuidof(IAVSOfficeFileConverter), (void **)(&m_piFileConverter)  );
		m_oEventViewer = new COfficeEventViewer< AVSGraphics::_IAVSDocumentPainterEvents, CAVSOfficeFile >(this);
		m_oEventViewer->AddRef();

		m_hViewerStop = CreateEvent( NULL, FALSE, FALSE, NULL);
		Clear();
		//dpi монитора чтобы картинка на мониторе смотрелась как на листке бумаги
		HDC hDCScreen = GetDC(NULL);
		m_nLOGPIXELSX = GetDeviceCaps(hDCScreen, LOGPIXELSX);
		m_nLOGPIXELSY = GetDeviceCaps(hDCScreen, LOGPIXELSY);
		ReleaseDC(NULL, hDCScreen);
	}
	__event __interface _IAVSOfficeFileEvents;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
#ifdef _DEBUG
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW );
		_CrtSetBreakAlloc(-1);
#endif
		InitializeCriticalSection(&m_csThread);
		return S_OK;
	}

	void FinalRelease()
	{
		CloseFile();
		RELEASEINTERFACE( m_piOfficeViewer );
		RELEASEINTERFACE( m_piFileConverter );
		RELEASEINTERFACE( m_oEventViewer );
		RELEASEHANDLE( m_hViewerStop );
		DeleteCriticalSection(&m_csThread);
	}
public:
	void OnProgressDocumentEx( LONG nID, LONG nPercent, SHORT* pnCancel)
	{
		if( 0 == pnCancel )
			return;
		if( true == m_bStopParse )
			(*pnCancel) = c_shProgressCancel;
		else
		{
			(*pnCancel) = c_shProgressContinue;
			OnProgressDocument( nID, nPercent );
		}
	}
	void OnProgressDocument( LONG nID, LONG nPercent)
	{
		if( true == m_bStopParse )
			return;
		SHORT nCancel;
		long nCurProgress;
		if( true == m_bPdfLoad )
			nCurProgress = nPercent;
		else
			nCurProgress = PROGRESS_LOAD * nPercent;
		if( nCurProgress > m_nLastProgress )
		{
			m_nLastProgress = nCurProgress;
			OnProgressParseFile( nID, nCurProgress, &nCancel );
		}
	}
	void OnCompletePageDocument()
	{
		if( true == m_bStopParse )
			return;
		OnCompletePageViewer();
	}
	void OnErrorViewer(LONG lError)
	{
		m_bParseFileComplete = true;
		m_nStatus = c_shOfficeFileOpen;
		m_nLastError = lError;
		if( true != m_bStopParse )
			OnError(lError);
		SetEvent( m_hViewerStop );
	}
	void OnStopViewer()
	{
		//отцепляем события
		m_oEventViewer->UnAdvise( m_piOfficeViewer );
		//передаем конвертеру
		AVSGraphics::IAVSDocumentRendererPtr punkRenderer = NULL;
		m_piFileConverter->get_CommandRenderer( (IUnknown**)&punkRenderer );
		if( NULL != m_piOfficeViewer )
			m_piOfficeViewer->RemoveRenderer( punkRenderer );

		m_bParseFileComplete = true;
		m_nStatus = c_shOfficeFileParsingComplete;
		if( true != m_bStopParse )
			OnCompleteParseFile();
		SetEvent( m_hViewerStop );
	}

	void OnProgressViewer(LONG lProgressPage)
	{
		if( true == m_bStopParse )
			return;
		SHORT nCancel = 0;
		long nCurProgress = c_nMaxProgressPercent * (PROGRESS_LOAD + PROGRESS_COMMAND * lProgressPage  / 100 );
		if( nCurProgress > m_nLastProgress )
		{
			m_nLastProgress = nCurProgress;
			OnProgressParseFile( 0, nCurProgress, &nCancel );
		}
		if( 0 != nCancel && NULL != m_piOfficeViewer)
			m_piOfficeViewer->Stop();
	}
	void OnProgressParse(  LONG lType, LONG lProgress )
	{
		if( true != m_bStopParse )
			OnProgressParsePage( lProgress );
	}
	void OnNewPageViewer(double dWidthMm, double dHeightMm)
	{
	}
	void OnCompletePageViewer()
	{
		if( true != m_bStopParse )
			OnCompleteParsePage();
	}
public:
	STDMETHOD(OpenFile)( BSTR bstrFilename, BSTR bstrOptions )
	{
		CTemporaryCS oCS(&m_csThread);
		CString strOptins = m_oDecoder.DecryptXML( bstrOptions );
		CComBSTR cbstrOpt(strOptins);
		//очишаем все что было открыто до этого
		CloseFile();

		//проверяем наличие файла
		if( ::GetFileAttributes( bstrFilename ) == DWORD(-1) )
			return AVS_ERROR_FILEACCESS;

		m_sFilename = bstrFilename;
		LONG nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;

		//проверяем bstrOptions
		XmlUtils::CXmlReader oXmlOptionsReader;
		if( TRUE == oXmlOptionsReader.OpenFromXmlString( cbstrOpt.m_str ) )
			if( TRUE == oXmlOptionsReader.ReadNodeList( _T("*") ) )
			{
				for( int i = 0; i < oXmlOptionsReader.GetLengthList(); i++ )
				{
					CString sNodeName = oXmlOptionsReader.ReadNodeName( i );
					if( _T("FileType") == sNodeName )
					{
						CString sFileType = oXmlOptionsReader.ReadNodeText( i );
						if( _T("") != sFileType )
							nFileType = Strings::ToInteger( sFileType );
					}
					else if( _T("Password") == sNodeName )
					{
						m_sPassword = oXmlOptionsReader.ReadNodeText( i );
					}
					else
					{
						//все опции о которых не занет OfficeFile копируем.
						m_sOpenOptions.Append( oXmlOptionsReader.ReadNodeXml( i ) );
					}
				}
			}
		if( AVS_OFFICESTUDIO_FILE_UNKNOWN == nFileType )
		{
			m_piFileConverter->get_FileType( &nFileType );
			AVSOfficeFormatChecker avsOfficeFormatChecker;
			if( AVS_OFFICESTUDIO_FILE_UNKNOWN == nFileType )
				m_piFileConverter->put_FileType( avsOfficeFormatChecker.GetFileFormat( m_sFilename ) );
			////заполняем info
			//LoadStructuredStorageInformation( CString( bstrFilename ) );
			//проверяем на пароль
			m_piFileConverter->get_FileType( &nFileType );
		}
		HRESULT hRes = S_OK;

		//pdf проверяем на наличие пароля
		if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFileType )
		{
			AVSOfficePDFFile::IPDFFilePtr piPdfFile;
			if( SUCCEEDED( piPdfFile.CreateInstance( __uuidof(AVSOfficePDFFile::CPDFFile) ) ) )
			{
				//Pdf опции
				CString sPdfOptions = _T("<Options>");
				sPdfOptions.Append( _T("<CheckPassword/>") );
				sPdfOptions.AppendFormat( _T("<FileType>%d</FileType>"), nFileType );
				if( false == m_sPassword.IsEmpty() )
					sPdfOptions.AppendFormat( _T("<Password>%s</Password>"), Utils::PrepareToXML( m_sPassword ) );
				sPdfOptions.Append( m_sOpenOptions );
				sPdfOptions.Append( _T("</Options>") );

				CString sTempDir = Utils::CreateTempDir( m_sTempPath );
				BSTR bstrTempDir = sTempDir.AllocSysString();
				BSTR bstrPdfOptions = sPdfOptions.AllocSysString();
				hRes = piPdfFile->LoadFromFile( bstrFilename, bstrTempDir, bstrPdfOptions );
				SysFreeString( bstrPdfOptions );
				SysFreeString( bstrTempDir );
				Utils::RemoveDirOrFile( sTempDir );

				if( SUCCEEDED( hRes ) ) //значит пароль верный
				{
					//получаем permissions
					VARIANT vtPermissions;
					if( SUCCEEDED( piPdfFile->GetAdditionalParam( _T("Permissions"), &vtPermissions ) ) )
						m_nPermissions = vtPermissions.lVal;

					VARIANT vtImagesCount; vtImagesCount.lVal = 0;
					if( SUCCEEDED( piPdfFile->GetAdditionalParam( _T("ImagesCount"), &vtImagesCount ) ) )
						m_nImagesCount = vtImagesCount.lVal;
				}
			}
		}
		if( SUCCEEDED( hRes ) )
			m_nStatus = c_shOfficeFileOpen;
		return hRes;
	}
	STDMETHOD(CloseFile)()
	{
		CTemporaryCS oCS(&m_csThread);
		long nLastStatus = m_nStatus;
		StopParse();
		if( nLastStatus >= c_shOfficeFileParsing )
		{
			RELEASEINTERFACE( m_piFileConverter );
			CoCreateInstance( __uuidof(CAVSOfficeFileConverter), NULL ,CLSCTX_INPROC_SERVER, __uuidof(IAVSOfficeFileConverter), (void **)(&m_piFileConverter)  );
			RELEASEINTERFACE( m_piOfficeViewer );
			if( NULL != m_piFileConverter )
			{
				CComBSTR bstrTemp( m_sTempPath );
				m_piFileConverter->put_TempDirectory( bstrTemp.m_str );
			}
		}
		Clear();
		m_nStatus = c_shOfficeFileIdle;
		return S_OK;
	}
	STDMETHOD(StartParse)()
	{
		CTemporaryCS oCS(&m_csThread);
		if( m_nStatus >= c_shOfficeFileParsing )
			return S_OK;
		if( "" == m_sFilename )
			return S_FALSE;
		m_nStatus = c_shOfficeFileParsing;
		m_bStopParse = false;
		DWORD dwTemp;
		HANDLE hParseThread = CreateThread(NULL, 0, &_ThreadProc, (void*)this, 0, &dwTemp);
		if( NULL == hParseThread )
		{
			m_nStatus = c_shOfficeFileIdle;
			return S_FALSE;
		}
		else
		{
			RELEASEHANDLE( hParseThread );
			return S_OK;
		}
	}
	STDMETHOD(StopParse)()
	{
		CTemporaryCS oCS(&m_csThread);
		m_bStopParse = true;
		LONG nCurFileType;
		m_piFileConverter->get_FileType( &nCurFileType );
		HRESULT hRes = S_OK;
		if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF != nCurFileType && AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU != nCurFileType && AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS != nCurFileType && c_shOfficeFileParsing == m_nStatus && NULL != m_piOfficeViewer )
			hRes = m_piOfficeViewer->Stop();
		while( c_shOfficeFileParsing == m_nStatus )
			Sleep( 200 );
		return hRes;
	}
	STDMETHOD(SuspendParse)()
	{
		CTemporaryCS oCS(&m_csThread);
		LONG nCurFileType;
		m_piFileConverter->get_FileType( &nCurFileType );
		if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF != nCurFileType && AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU != nCurFileType && AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS != nCurFileType && c_shOfficeFileParsing == m_nStatus && NULL != m_piOfficeViewer )
			return m_piOfficeViewer->Suspend();
		else 
			return S_OK;
	}
	STDMETHOD(ResumeParse)()
	{
		CTemporaryCS oCS(&m_csThread);
		LONG nCurFileType;
		m_piFileConverter->get_FileType( &nCurFileType );
		if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF != nCurFileType && AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU != nCurFileType && AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS != nCurFileType && c_shOfficeFileParsing == m_nStatus && NULL != m_piOfficeViewer )
			return m_piOfficeViewer->Resume();
		else
			return S_OK;

	}
	STDMETHOD(ReadPage)( LONG nPageNumber, IUnknown** ppunkMediaData )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == ppunkMediaData )
			return S_FALSE;
		(*ppunkMediaData) = NULL;
		AVSGraphics::IAVSDocumentRendererPtr piCommandRenderer = NULL;
		m_piFileConverter->get_CommandRenderer( (IUnknown**)&piCommandRenderer );
		if( NULL == piCommandRenderer )
			return S_FALSE;
		double fPageWidthMm = 0;
		double fPageHeightMm = 0;
		if( SUCCEEDED( piCommandRenderer->GetPageSize(nPageNumber, &fPageWidthMm, &fPageHeightMm) ) )
		{
			AVSGraphics::IAVSGraphicsRenderer* piGdiplusRenderer = NULL;
			//AVSOfficeEditor::IAVSGdiplusRenderer* piGdiplusRenderer = NULL;
			MediaCore::IAVSUncompressedVideoFrame* piImage = NULL;
			UpdateGdiPlusRenderer( fPageWidthMm, fPageHeightMm, &piImage ,&piGdiplusRenderer);
			if( m_nWidth > 0 && m_nHeight > 0 )
			{
				VARIANT vtDPI;
				vtDPI.vt = VT_R8;
				vtDPI.dblVal = ONE_INCH * 10 * m_nWidth / fPageWidthMm ;
				piCommandRenderer->SetAdditionalParam( _T("DPIX"), vtDPI );
				vtDPI.dblVal = ONE_INCH * 10 * m_nHeight / fPageHeightMm ;
				piCommandRenderer->SetAdditionalParam( _T("DPIY"), vtDPI );
			}

			piCommandRenderer->DrawPage( nPageNumber, piGdiplusRenderer );

			//ставим watermark
			LONG nPageWidth = LONG( TO_FLOAT * fPageWidthMm );
			LONG nPageHeight = LONG( TO_FLOAT * fPageHeightMm );

			m_piFileConverter->DrawWatermark( piGdiplusRenderer, nPageWidth, nPageHeight );

			RELEASEINTERFACE(piGdiplusRenderer);
			if( NULL != piImage )
				piImage->QueryInterface( __uuidof(IUnknown), (void**)ppunkMediaData);
			RELEASEINTERFACE(piImage);
		}
		return S_OK;
	}
	STDMETHOD(DrawPage)( LONG nPageNumber, IUnknown* punkMediaData )
	{
		CTemporaryCS oCS(&m_csThread);
		AVSGraphics::IAVSDocumentRendererPtr piCommandRenderer = NULL;
		m_piFileConverter->get_CommandRenderer( (IUnknown**)&piCommandRenderer );
		if( NULL != piCommandRenderer )
		{
			CAtlMap<long, CString>::CPair *pPair = NULL;
			pPair = m_mapThumbnails.Lookup( nPageNumber );
			bool bLoadFromCache = false;
			if( NULL != pPair )
			{//загружаем Thumbnail из файла
				HANDLE hThumbnailHandle = CreateFile ( pPair->m_value, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
				if ( INVALID_HANDLE_VALUE != hThumbnailHandle )
				{
					long nCacheBufferSize = 0;
					DWORD dwBytesRead = 0;
					if ( TRUE == ReadFile( hThumbnailHandle, &nCacheBufferSize, ( DWORD ) sizeof(long), &dwBytesRead, NULL ) )
						if ( dwBytesRead == sizeof( long ) && nCacheBufferSize > 0 )
						{
							MediaCore::IAVSUncompressedVideoFrame * piFrame = NULL;
							if( SUCCEEDED( punkMediaData->QueryInterface( __uuidof( MediaCore::IAVSUncompressedVideoFrame ), (void**)&piFrame ) ) )
							{
								long nMinBufSize = nCacheBufferSize < piFrame->BufferSize ? nCacheBufferSize : piFrame->BufferSize;
								if ( TRUE == ReadFile ( hThumbnailHandle, piFrame->Buffer, ( DWORD ) nMinBufSize, &dwBytesRead, NULL ) )
								{
									if ( dwBytesRead == nMinBufSize )
										bLoadFromCache = true;
								}
								RELEASEINTERFACE( piFrame );
							}
						}
					RELEASEHANDLE( hThumbnailHandle );
				}
			}
			if( false == bLoadFromCache )
			{//создаем новый
				AVSGraphics::IAVSGraphicsRenderer* piGdiplusRenderer = NULL;
				CoCreateInstance( __uuidof( AVSGraphics::CAVSGraphicsRenderer ), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IAVSGraphicsRenderer), (void **)(&piGdiplusRenderer)  );
				//AVSOfficeEditor::IAVSGdiplusRenderer* piGdiplusRenderer = NULL;
				//CoCreateInstance( __uuidof( AVSOfficeEditor::CAVSGdiplusRenderer ), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSOfficeEditor::IAVSGdiplusRenderer), (void **)(&piGdiplusRenderer)  );
				if( NULL != piGdiplusRenderer )
				{
					MediaCore::IAVSUncompressedVideoFrame* piGraphicFrame = NULL;
					punkMediaData->QueryInterface( __uuidof( MediaCore::IAVSUncompressedVideoFrame ), (void**)&piGraphicFrame );

						//ставим FontManager
						VARIANT vtVariant;
						vtVariant.vt = VT_UNKNOWN;
						vtVariant.punkVal = NULL;
						piGdiplusRenderer->SetAdditionalParam( L"FontManager", vtVariant );

					piGdiplusRenderer->CreateFromMediaData( punkMediaData, 0, 0, piGraphicFrame->Width, piGraphicFrame->Height );
					RELEASEINTERFACE( piGraphicFrame );
					//Делаем фон белым
					MediaCore::IAVSUncompressedVideoFrame * piFrame = NULL;
					FLOAT fPageWidthMm = 0;
					FLOAT fPageHeightMm = 0;
					if( SUCCEEDED( punkMediaData->QueryInterface( __uuidof( MediaCore::IAVSUncompressedVideoFrame ), (void**)&piFrame ) ) )
					{
						//memset( piFrame->Buffer, 255, piFrame->BufferSize);
						fPageWidthMm = PIX_TO_MM( piFrame->Width, m_nLOGPIXELSX );
						fPageHeightMm = PIX_TO_MM( piFrame->Height, m_nLOGPIXELSY );
						double fPageWidthMm2 = 0;
						double fPageHeightMm2 = 0;
						if( SUCCEEDED( piCommandRenderer->GetPageSize(nPageNumber, &fPageWidthMm2, &fPageHeightMm2) ) )
						{
							if( piFrame->Width > 0 && piFrame->Height > 0 )
							{
								VARIANT vtDPI;
								vtDPI.vt = VT_R8;
								vtDPI.dblVal = ONE_INCH * 10 * piFrame->Width / fPageWidthMm2 ;
								piCommandRenderer->SetAdditionalParam( _T("DPIX"), vtDPI );
								vtDPI.dblVal = ONE_INCH * 10 * piFrame->Height / fPageHeightMm2 ;
								piCommandRenderer->SetAdditionalParam( _T("DPIY"), vtDPI );
							}
						}
						//рисуем страницу
						piCommandRenderer->DrawPage( nPageNumber, piGdiplusRenderer );
						//ставим watermark
						LONG nPageWidth = LONG( TO_FLOAT * fPageWidthMm );
						LONG nPageHeight = LONG( TO_FLOAT * fPageWidthMm );
						m_piFileConverter->DrawWatermark( piGdiplusRenderer, nPageWidth, nPageHeight );
						RELEASEINTERFACE( piGdiplusRenderer );

						//сохраняем thumbnail в файл
						if( _T("") == m_sThumbnailsPath )
							m_sThumbnailsPath = Utils::CreateTempDir( m_sTempPath );
						//сохраняем в файл медиа дату: long на размер буфера, все остальное - буфер
						CString sThumbnailFilename = Utils::CreateTempFile( m_sThumbnailsPath );
						bool bError = true;
						HANDLE hThumbnailHandle = CreateFile ( sThumbnailFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
						if ( INVALID_HANDLE_VALUE != hThumbnailHandle )
						{
							DWORD dwBytesWrite = 0;
							long nBufSize = piFrame->BufferSize;
							if ( TRUE == WriteFile ( hThumbnailHandle, &nBufSize, ( DWORD ) sizeof(long), &dwBytesWrite, NULL ) )
								if ( dwBytesWrite == sizeof( long ) )
									if ( TRUE == WriteFile ( hThumbnailHandle, piFrame->Buffer, ( DWORD ) piFrame->BufferSize, &dwBytesWrite, NULL ) )
										if ( dwBytesWrite == piFrame->BufferSize )
											bError = false;
							RELEASEHANDLE( hThumbnailHandle );
						}

						if( false == bError )
							m_mapThumbnails.SetAt( nPageNumber, sThumbnailFilename );
						else
							Utils::RemoveDirOrFile( sThumbnailFilename );
						RELEASEINTERFACE( piFrame );
					}
				}
			}
		}
		return S_OK;
	}
	STDMETHOD(PrintPage)( LONG nPageNumber, LONG* pHDC, BSTR bstrXMLMode )
	{
		AVSGraphics::IAVSDocumentRendererPtr piCommandRenderer = NULL;
		m_piFileConverter->get_CommandRenderer( (IUnknown**)&piCommandRenderer );
		AVSGraphics::IAVSEffectPainterPtr piEffectPainter = NULL;
		piEffectPainter.CreateInstance( __uuidof(AVSGraphics::CAVSEffectPainter) );
		if( NULL == piCommandRenderer || NULL == piEffectPainter )
			return S_FALSE;

		long nPageCount = 0;
		piCommandRenderer->get_PageCount( &nPageCount );
		if( -1 != nPageNumber && ( nPageNumber < 0 || nPageNumber >= nPageCount || NULL == pHDC ) )
			return S_FALSE;

		HDC hPrintDC = *((HDC*)pHDC);

		AVSGraphics::IAVSGraphicsRendererPtr piHDCRenderer;
		piHDCRenderer.CreateInstance( __uuidof(AVSGraphics::CAVSGraphicsRenderer) );
		//AVSOfficeEditor::IAVSHDCRendererPtr piHDCRenderer;
		//piHDCRenderer.CreateInstance( __uuidof(AVSOfficeEditor::CAVSHDCRenderer) );
		if( NULL == piHDCRenderer )
			return AVS_ERROR_UNEXPECTED;

		int nMode = -1;
		int nWidthPix = -1;
		int nHeightPix = -1;
		bool bZoomEnable = false;
		bool bRotateEnable = false;
		bool bPrintableArea = false;

		//душифруем xml
		CString strXml = m_oDecoder.DecryptXML( bstrXMLMode );
		CString sWaterMarks;
		XmlUtils::CXmlReader oXmlReader;
		if( TRUE == oXmlReader.OpenFromXmlString( strXml ) )
			if( TRUE == oXmlReader.ReadRootNode( _T("PrintSettings") ) )
			{
				CString sWidth = oXmlReader.ReadNodeValue( _T("Width") );
				if( _T("") != sWidth )
					nWidthPix = Strings::ToInteger( sWidth );
				CString sHeight = oXmlReader.ReadNodeValue( _T("Height") );
				if( _T("") != sHeight )
					nHeightPix = Strings::ToInteger( sHeight );

				CString sMode = oXmlReader.ReadNodeValue( _T("Mode") );
				if( _T("") != sMode )
					nMode = Strings::ToInteger( sMode );
				XML::IXMLDOMNodePtr oNode;
				oXmlReader.GetNode( oNode );
				oXmlReader.ReadNode( _T("Mode") );

				CString sZoomEnable = oXmlReader.ReadNodeAttribute( _T("ZoomEnable") );
				STRING_TO_BOOL( sZoomEnable, bZoomEnable );
				CString sRotateEnable = oXmlReader.ReadNodeAttribute( _T("RotateEnable") );
				STRING_TO_BOOL( sRotateEnable, bRotateEnable );
				CString sPrintableArea = oXmlReader.ReadNodeAttribute( _T("PrintableAreaEnable") );
				STRING_TO_BOOL( sPrintableArea, bPrintableArea );

				oXmlReader.OpenFromXmlNode( oNode );
				oXmlReader.ReadNode( _T("Watermarks") );
				sWaterMarks = oXmlReader.ReadNodeXml();
			}

		if( -1 == nPageNumber )
		{//виртуальная печать
			long nPrintDpiX = GetDeviceCaps( hPrintDC, LOGPIXELSX);
			long nPrintDpiY = GetDeviceCaps( hPrintDC, LOGPIXELSY);
			long nPrintOffsetX = GetDeviceCaps( hPrintDC, PHYSICALOFFSETX);
			long nPrintOffsetY = GetDeviceCaps( hPrintDC, PHYSICALOFFSETY);
			//for( int i = 0; i < nPageCount; i++ )
			//{
			//	//проверяем не выходит ли каждая страница за границы печати
			//	double dLeftPix = 0;
			//	double dTopPix = 0;
			//	double dWidthPix = 0;
			//	double dHeightPix = 0;
			//	double dAngel = 0;

			//	PrepareForPrint( i, pHDC,
			//					nWidthPix, nHeightPix, nMode, bZoomEnable, bRotateEnable, bPrintableArea,
			//					dLeftPix, dTopPix, dWidthPix, dHeightPix, dAngel );
			//	//piHDCRenderer->

			//}
		}
		else
		{//реальная печать
			double dLeftPix = 0;
			double dTopPix = 0;
			double dWidthPix = 0;
			double dHeightPix = 0;
			double dAngel = 0;
			double dPrintWidthMM;
			double dPrintHeightMM;

			PrepareForPrint( nPageNumber, pHDC,
							nWidthPix, nHeightPix, nMode, bZoomEnable, bRotateEnable, bPrintableArea,
							dLeftPix, dTopPix, dWidthPix, dHeightPix, dAngel, dPrintWidthMM, dPrintHeightMM );

			//распечатываем документ
			piHDCRenderer->CreateFromHDC( pHDC, NULL, dPrintWidthMM, dPrintHeightMM ,dLeftPix, dTopPix , dWidthPix, dHeightPix, dAngel );
			piCommandRenderer->DrawPage( nPageNumber ,piHDCRenderer );
			BSTR bstrWaterMarks = sWaterMarks.AllocSysString();
			if( SUCCEEDED( piEffectPainter->SetXml( bstrWaterMarks ) ) )
				piEffectPainter->Draw( piHDCRenderer );
			SysFreeString( bstrWaterMarks );

			piHDCRenderer->BitBlt( pHDC );
		}

		return S_OK;
	}
bool PrepareForPrint( LONG nPageNumber, LONG* pHDC,
					 long nWidthPix, long nHeightPix, int nMode, bool bZoomEnable, bool bRotateEnable, bool bPrintableArea,
					 double& dLeftPix, double& dTopPix, double& dWidthPix, double& dHeightPix, double& dAngel, double& fPrintWidthMM, double& fPrintHeightMM )
{
		AVSGraphics::IAVSDocumentRendererPtr piCommandRenderer = NULL;
		m_piFileConverter->get_CommandRenderer( (IUnknown**)&piCommandRenderer );
		if( NULL == piCommandRenderer )
			return S_FALSE;
		double fPageWidth = 0;
		double fPageHeight = 0;
		piCommandRenderer->GetPageSize( nPageNumber, &fPageWidth, &fPageHeight );
		if( 0 == fPageWidth || fPageHeight == 0 )
			return false;

		HDC hPrintDC = *((HDC*)pHDC);

		long nPrintDpiX = GetDeviceCaps( hPrintDC, LOGPIXELSX);
		long nPrintDpiY = GetDeviceCaps( hPrintDC, LOGPIXELSY);
		long nPrintOffsetX = GetDeviceCaps( hPrintDC, PHYSICALOFFSETX);
		long nPrintOffsetY = GetDeviceCaps( hPrintDC, PHYSICALOFFSETY);
		long nPrintWidthPix; //всей страницы
		long nPrintHeightPix;
		long nPrintPageWidthPix; //только области печати
		long nPrintPageHeightPix;
		if( -1 == nWidthPix || -1 == nHeightPix )
		{
			nPrintWidthPix  = GetDeviceCaps( hPrintDC, PHYSICALWIDTH);
			nPrintHeightPix  = GetDeviceCaps( hPrintDC, PHYSICALHEIGHT);
			nPrintPageWidthPix  = GetDeviceCaps( hPrintDC, HORZRES );
			nPrintPageHeightPix  = GetDeviceCaps( hPrintDC, VERTRES);
		}
		else
		{
			nPrintWidthPix  = nWidthPix;
			nPrintHeightPix  = nHeightPix;
			nPrintPageWidthPix  = nWidthPix;
			nPrintPageHeightPix  = nHeightPix;
		}
		if( true == bPrintableArea ) 
		{//печатать нужно только в области печати
			//приравниваем высоту страницы к высоте области печати
			nPrintWidthPix = nPrintPageWidthPix;
			nPrintHeightPix = nPrintPageHeightPix;
			//обнуляем поправки на непечатаемую область
			nPrintOffsetX = 0;
			nPrintOffsetY = 0;
		}
		//подсчитываем размеры страницы в милиметрах
		fPrintWidthMM  = 10 * ONE_INCH * nPrintWidthPix / nPrintDpiX;
		fPrintHeightMM  = 10 * ONE_INCH * nPrintHeightPix / nPrintDpiX;

		if( c_nPrintType100  == nMode )
		{
			dWidthPix = nPrintDpiX * fPageWidth / ( 10 * ONE_INCH );
			dHeightPix = nPrintDpiX * fPageHeight / ( 10 * ONE_INCH );
			if( true == bRotateEnable && ( nPrintWidthPix < dWidthPix || nPrintHeightPix < dHeightPix ))
			{
				if( nPrintWidthPix < dHeightPix || nPrintHeightPix < dWidthPix )
				{
					//выбираем лучший вариант по площади
					double dWidth1 = nPrintWidthPix < dWidthPix ? nPrintWidthPix : dWidthPix;
					double dHeight1 = nPrintHeightPix < dHeightPix ? nPrintHeightPix : dHeightPix;

					double dWidth2 = nPrintWidthPix < dHeightPix ? nPrintWidthPix : dHeightPix;
					double dHeight2 = nPrintHeightPix < dWidthPix ? nPrintHeightPix : dWidthPix;

					if( dWidth1 * dHeight1 >= dWidth2 * dHeight2 )
					{
						dLeftPix = 0;
						dTopPix = 0;
					}
					else
					{
						dLeftPix = nPrintWidthPix - ( dHeightPix + dWidthPix ) / 2;
						dTopPix = dWidthPix / 2 - dHeightPix / 2;
						dAngel = M_PI_2;//90
					}
				}
				else
				{
					//если не вписывается, но вписывается повернутое
					dLeftPix = nPrintWidthPix - (dHeightPix + dWidthPix ) / 2;
					dTopPix = nPrintHeightPix / 2 - dHeightPix / 2;
					dAngel = M_PI_2;//90
				}
			}
			else
			{
				if( dWidthPix < nPrintWidthPix ) //если размеры позволяют, то распологаем по центру
					dLeftPix = nPrintWidthPix / 2 - dWidthPix / 2;
				else
					dLeftPix = 0;
				dTopPix = 0;
			}
		}
		else if( c_nPrintTypeStrech  == nMode )
		{
			if( bRotateEnable && (fPageWidth / fPageHeight - 1 ) * ( fPrintWidthMM / fPrintHeightMM - 1 ) < 0 )
			{//переворачиваем
				dWidthPix = nPrintHeightPix;
				dHeightPix = nPrintWidthPix;
				dLeftPix = nPrintWidthPix / 2 - dWidthPix / 2;
				dTopPix = nPrintHeightPix / 2 - dHeightPix / 2;
				dAngel = M_PI_2;//90
			}
			else
			{
				dWidthPix = nPrintWidthPix;
				dHeightPix = nPrintHeightPix;
				dLeftPix = 0;
				dTopPix = 0;
			}
		}
		else
		{
			if( bZoomEnable && bRotateEnable )
			{
				bool bRotate = false;
				if( (fPageWidth / fPageHeight - 1 ) * ( fPrintWidthMM / fPrintHeightMM - 1 ) < 0 )
				{//переворачиваем
					double dTemp = fPrintWidthMM;
					fPrintWidthMM = fPrintHeightMM;
					fPrintHeightMM = dTemp;
					dAngel = M_PI_2;//90
					bRotate = true;
				}
				float fFitX = 0;
				float fFitY = 0;
				float fFitWidth = 0;
				float fFitHeight = 0;
				FitToPage( fPageWidth, fPageHeight, fPrintWidthMM, fPrintHeightMM, fFitX, fFitY, fFitWidth, fFitHeight );

				dWidthPix = nPrintDpiX * fFitWidth / ( 10 * ONE_INCH );
				dHeightPix = nPrintDpiY * fFitHeight / ( 10 * ONE_INCH );
				if( true == bRotate )
				{
					dLeftPix = nPrintWidthPix / 2 - dWidthPix / 2;
					dTopPix = nPrintHeightPix / 2 - dHeightPix / 2;
				}
				else
				{
					dLeftPix = nPrintDpiX * fFitX / ( 10 * ONE_INCH );
					dTopPix = nPrintDpiY * fFitY / ( 10 * ONE_INCH );
				}
			}
			else if( bZoomEnable )
			{
				float fFitX = 0;
				float fFitY = 0;
				float fFitWidth = 0;
				float fFitHeight = 0;
				FitToPage( fPageWidth, fPageHeight, fPrintWidthMM, fPrintHeightMM, fFitX, fFitY, fFitWidth, fFitHeight );
				dWidthPix = nPrintDpiX * fFitWidth / ( 10 * ONE_INCH );
				dHeightPix = nPrintDpiY * fFitHeight / ( 10 * ONE_INCH );
				dLeftPix = nPrintDpiX * fFitX / ( 10 * ONE_INCH );
				dTopPix = nPrintDpiY * fFitY / ( 10 * ONE_INCH );
				dAngel = 0;
			}
			else if( bRotateEnable )
			{
				//проверяем выходит ли картинка за габариты
				if( fPrintWidthMM < fPageWidth || fPrintHeightMM < fPageHeight )
				{
					//проверяем выходит ли повернутая картинка за габариты
					if( fPrintHeightMM < fPageWidth || fPrintWidthMM < fPageHeight )
					{
						//выбираем, где больше площадь у повернутого или нет
						float fFitX1 = 0;
						float fFitY1 = 0;
						float fFitWidth1 = 0;
						float fFitHeight1 = 0;
						FitToPage( fPageWidth, fPageHeight, fPrintWidthMM, fPrintHeightMM, fFitX1, fFitY1, fFitWidth1, fFitHeight1 );

						float fFitX2 = 0;
						float fFitY2 = 0;
						float fFitWidth2 = 0;
						float fFitHeight2 = 0;
						FitToPage( fPageWidth, fPageHeight, fPrintHeightMM, fPrintWidthMM, fFitX2, fFitY2, fFitWidth2, fFitHeight2 );
						if( fFitWidth1 * fFitHeight1 < fFitWidth2 * fFitHeight2 )
						{//поворачиваем
							dAngel = M_PI_2;//90
							dWidthPix = nPrintDpiX * fFitWidth2 / ( 10 * ONE_INCH );
							dHeightPix = nPrintDpiY * fFitHeight2 / ( 10 * ONE_INCH );
							dLeftPix = nPrintWidthPix / 2 - dWidthPix / 2;
							dTopPix = nPrintHeightPix / 2 - dHeightPix / 2;
						}
						else
						{
							dAngel = 0;
							dWidthPix = nPrintDpiX * fFitWidth1 / ( 10 * ONE_INCH );
							dHeightPix = nPrintDpiY * fFitHeight1 / ( 10 * ONE_INCH );
							dLeftPix = nPrintDpiX * fFitX1 / ( 10 * ONE_INCH );
							dTopPix = nPrintDpiY * fFitY1 / ( 10 * ONE_INCH );
						}
					}
					else
					{//поворачиваем
						dWidthPix = nPrintDpiX * fPageWidth / ( 10 * ONE_INCH );
						dHeightPix = nPrintDpiY * fPageHeight / ( 10 * ONE_INCH );
						dLeftPix = nPrintWidthPix - (dHeightPix + dWidthPix ) / 2;
						dTopPix = nPrintHeightPix / 2 - dHeightPix / 2;
						dAngel = M_PI_2;//90
					}
				}
				else
				{
					dWidthPix = nPrintDpiX * fPageWidth / ( 10 * ONE_INCH );
					dHeightPix = nPrintDpiY * fPageHeight / ( 10 * ONE_INCH );
					dLeftPix = nPrintWidthPix / 2 - dWidthPix / 2; //по центру по горизонтали
					dTopPix = 0; //сверху повертикали
					dAngel = 0;
				}
			}
			else
			{
				//проверяем выходит ли картинка за габариты
				if( fPrintWidthMM < fPageWidth || fPrintHeightMM < fPageHeight )
				{
					float fFitX = 0;
					float fFitY = 0;
					float fFitWidth = 0;
					float fFitHeight = 0;
					FitToPage( fPageWidth, fPageHeight, fPrintWidthMM, fPrintHeightMM, fFitX, fFitY, fFitWidth, fFitHeight );
					dWidthPix = nPrintDpiX * fFitWidth / ( 10 * ONE_INCH );
					dHeightPix = nPrintDpiY * fFitHeight / ( 10 * ONE_INCH );
					dLeftPix = nPrintDpiX * fFitX / ( 10 * ONE_INCH );
					dTopPix = nPrintDpiY * fFitY / ( 10 * ONE_INCH );
				}
				else
				{
					dWidthPix = nPrintDpiX * fPageWidth / ( 10 * ONE_INCH );
					dHeightPix = nPrintDpiY * fPageHeight / ( 10 * ONE_INCH );
					dLeftPix = nPrintWidthPix / 2 - dWidthPix / 2; //по центру по горизонтали
					dTopPix = 0; //сверху повертикали
				}
			}
		}
		dLeftPix -= nPrintOffsetX;
		dTopPix -= nPrintOffsetY;

	return true;
}
	STDMETHOD(GetPageCount)( LONG* pnPageCount )
	{
		CTemporaryCS oCS(&m_csThread);
		AVSGraphics::IAVSDocumentRendererPtr piCommandRenderer = NULL;
		m_piFileConverter->get_CommandRenderer( (IUnknown**)&piCommandRenderer );
		if( NULL == pnPageCount || NULL == piCommandRenderer )
			return S_FALSE;
		(*pnPageCount) = 0;
		return piCommandRenderer->get_PageCount(pnPageCount);
	}
	STDMETHOD(GetPageSize)( LONG lPageNumber, DOUBLE* pfWidth, DOUBLE* pfHeight )
	{
		CTemporaryCS oCS(&m_csThread);
		AVSGraphics::IAVSDocumentRendererPtr piCommandRenderer = NULL;
		m_piFileConverter->get_CommandRenderer( (IUnknown**)&piCommandRenderer );
		if( NULL == pfWidth || NULL == pfHeight || NULL == piCommandRenderer )
			return S_FALSE;
		(*pfWidth) = 0;
		(*pfHeight) = 0;
		return piCommandRenderer->GetPageSize(lPageNumber, pfWidth, pfHeight);
	}
	STDMETHOD(GetInternalInteface)( IUnknown** ppunkFileConverter )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == ppunkFileConverter )
			return S_FALSE;
		(*ppunkFileConverter) = NULL;
		return m_piFileConverter->QueryInterface(IID_IUnknown, (void**)ppunkFileConverter);
	}


	STDMETHOD(get_FileType)(LONG* pnType)
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == pnType )
			return S_FALSE;
		return m_piFileConverter->get_FileType(pnType);
	}
	STDMETHOD(put_FileType)(LONG nType)
	{
		CTemporaryCS oCS(&m_csThread);
		return m_piFileConverter->put_FileType(nType);
	}
	STDMETHOD(get_OfficeViewer)( IUnknown** ppunkVal )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == ppunkVal )
			return S_FALSE;
		return m_piOfficeViewer->QueryInterface( __uuidof( IUnknown ), (void**)ppunkVal );
	}
	STDMETHOD(put_OfficeViewer)( IUnknown* punkVal )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == punkVal )
			return S_FALSE;
		RELEASEINTERFACE(m_piOfficeViewer);
		AVSGraphics::IAVSDocumentPainter* piOfficeView = NULL;
		return punkVal->QueryInterface( __uuidof( AVSGraphics::IAVSDocumentPainter ) , (void**)&m_piOfficeViewer );
	}
	STDMETHOD(get_FileInfo)( BSTR* pbstrInfo )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == pbstrInfo )
			return S_FALSE;
		(*pbstrInfo) = m_sFileInfo.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_FileInfo)( BSTR bstrInfo )
	{
		CTemporaryCS oCS(&m_csThread);
		m_sFileInfo = CString(bstrInfo);
		return S_OK;
	}

	STDMETHOD(get_IsEdited)( VARIANT_BOOL* pvIsEdited)
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == pvIsEdited )
			return S_FALSE;
		if( true == m_bIsEdited )
			(*pvIsEdited) = VARIANT_TRUE;
		else
			(*pvIsEdited) = VARIANT_FALSE;
		return S_OK;
	}

	STDMETHOD(get_Title)(  BSTR* pbstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == pbstrVal )
			return S_FALSE;
		(*pbstrVal) = NULL;
		(*pbstrVal) = m_sTitle.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Title)(  BSTR bstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		m_sTitle = CString( bstrVal );
		SaveStructuredStorageInformation(m_sFilename, PIDSI_TITLE, m_sTitle);
		return S_OK;
	}
	STDMETHOD(get_Subject)(  BSTR* pbstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == pbstrVal )
			return S_FALSE;
		(*pbstrVal) = NULL;
		(*pbstrVal) = m_sSubject.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Subject)(  BSTR bstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		m_sSubject = CString( bstrVal );
		SaveStructuredStorageInformation(m_sFilename, PIDSI_SUBJECT, m_sSubject);
		return S_OK;
	}
	STDMETHOD(get_Keywords)(  BSTR* pbstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == pbstrVal )
			return S_FALSE;
		(*pbstrVal) = NULL;
		(*pbstrVal) = m_sKeywords.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Keywords)(  BSTR bstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		m_sKeywords = CString( bstrVal );
		SaveStructuredStorageInformation(m_sFilename, PIDSI_KEYWORDS, m_sKeywords);
		return S_OK;
	}
	STDMETHOD(get_Comments)(  BSTR* pbstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == pbstrVal )
			return S_FALSE;
		(*pbstrVal) = NULL;
		(*pbstrVal) = m_sComments.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Comments)(  BSTR bstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		m_sComments = CString( bstrVal );
		SaveStructuredStorageInformation(m_sFilename, PIDSI_COMMENTS, m_sComments);
		return S_OK;
	}
	STDMETHOD(get_Author)(  BSTR* pbstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == pbstrVal )
			return S_FALSE;
		(*pbstrVal) = NULL;
		(*pbstrVal) = m_sAuthor.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Author)(  BSTR bstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		m_sAuthor = CString( bstrVal );
		SaveStructuredStorageInformation(m_sFilename, PIDSI_AUTHOR, m_sAuthor);
		return S_OK;
	}
	STDMETHOD(get_RevisionNumber)(  BSTR* pbstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		if( NULL == pbstrVal )
			return S_FALSE;
		(*pbstrVal) = NULL;
		(*pbstrVal) = m_sRevisionNumber.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_RevisionNumber)(  BSTR bstrVal )
	{
		CTemporaryCS oCS(&m_csThread);
		m_sRevisionNumber = CString( bstrVal );
		SaveStructuredStorageInformation(m_sFilename, PIDSI_REVNUMBER, m_sRevisionNumber);
		return S_OK;
	}
	STDMETHOD(get_Width)( LONG* pnVal )
	{
		if( NULL == pnVal )
			return false;
		(*pnVal) = m_nWidth;
		return S_OK;
	}
	STDMETHOD(put_Width)( LONG nVal )
	{
		m_nWidth = nVal;
		return S_OK;
	}
	STDMETHOD(get_Height)( LONG* pnVal )
	{
		if( NULL == pnVal )
			return false;
		(*pnVal) = m_nHeight;
		return S_OK;
	}
	STDMETHOD(put_Height)( LONG nVal )
	{
		m_nHeight = nVal;
		return S_OK;
	}
	STDMETHOD(get_ResizeMode)( LONG* pnVal )
	{
		if( NULL == pnVal )
			return false;
		(*pnVal) = m_nResizeMode;
		return S_OK;
	}
	STDMETHOD(put_ResizeMode)( LONG nVal )
	{
		m_nResizeMode = nVal;
		return S_OK;
	}
	STDMETHOD(get_Watermark)( BSTR* pbstrVal )
	{
		if( NULL == pbstrVal )
			return S_FALSE;
		(*pbstrVal) = m_sWatermark.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Watermark)( BSTR bstrVal )
	{
		m_sWatermark = CString(bstrVal);
		m_piFileConverter->put_Watermark(bstrVal);
		return S_OK;
	}
	STDMETHOD(get_IsParseFileComplete)( VARIANT_BOOL* pvIsParseFileComplete )
	{
		if( NULL == pvIsParseFileComplete )
			return S_FALSE;
		if( false == m_bParseFileComplete )
			(*pvIsParseFileComplete) = VARIANT_FALSE;
		else
			(*pvIsParseFileComplete) = VARIANT_TRUE;
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)( BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (g_csBlowfishKeyParamName == sParamName)
		{		
			if (!m_oDecoder.SetBlowfishKey(ParamValue.punkVal))
				return S_FALSE;
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)( BSTR ParamName, VARIANT* ParamValue)
	{
		if (NULL == ParamValue)
			return S_FALSE;

		CString sParamName; sParamName = ( CString ) ParamName; sParamName.MakeLower();
		if ( L"imagescount" == sParamName )
		{
			ParamValue->vt = VT_I4;
			ParamValue->lVal = m_nImagesCount;
		}
		else if( L"lasterror" == sParamName )
		{
			ParamValue->vt = VT_I4;
			ParamValue->lVal = m_nLastError;
		}
		return S_OK;
	}
	STDMETHOD(get_MaxFilesCount)( LONG *pnVal)
	{
		if( NULL == pnVal )
			return S_FALSE;
		(*pnVal) = m_nMaxFilesCount;
		return S_OK;
	}
	STDMETHOD(put_MaxFilesCount)( LONG nVal)
	{
		m_nMaxFilesCount = nVal;
		return S_OK;
	}
	STDMETHOD(get_TempDirectory)( BSTR* pbstrDir )
	{
		return m_piFileConverter->get_TempDirectory(pbstrDir);
	}
	STDMETHOD(put_TempDirectory)( BSTR bstrDir )
	{
		m_sTempPath = CString( bstrDir );
		return m_piFileConverter->put_TempDirectory(bstrDir);;
	}
	STDMETHOD(get_Password)( BSTR* pbstrPassword )
	{
		if( NULL == pbstrPassword )
			return S_FALSE;
		(*pbstrPassword) = m_sPassword.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Password)( BSTR bstrPassword )
	{
		m_sPassword = CString( bstrPassword );
		return S_OK;
	}
	STDMETHOD(get_Permissions)( LONG* pnPermissions )
	{
		if( NULL == pnPermissions )
			return S_FALSE;
		(*pnPermissions) = m_nPermissions;
		return S_OK;
	}
	STDMETHOD(get_Status)( LONG* pVal)
	{
		if( NULL == pVal )
			return S_FALSE;
		(*pVal) = m_nStatus;
		return S_OK;
	}
	STDMETHOD(get_CommandRenderer)( IUnknown** ppunkRend )
	{
		return m_piFileConverter->get_CommandRenderer( ppunkRend );
	}
	private: void UpdateGdiPlusRenderer(float dWidthMm, float dHeightMm,MediaCore::IAVSUncompressedVideoFrame** piImage,AVSGraphics::IAVSGraphicsRenderer** piRend )
	{
		(*piImage) = NULL;
		(*piRend) = NULL;
		CoCreateInstance( __uuidof( MediaCore::CAVSUncompressedVideoFrame ), NULL ,CLSCTX_INPROC_SERVER, __uuidof(MediaCore::IAVSUncompressedVideoFrame ), (void **)piImage  );

		//dpi монитора чтобы картинка на мониторе смотрелась как на листке бумаги
		HDC hDCScreen = GetDC(NULL);
		int nLOGPIXELSX = GetDeviceCaps(hDCScreen, LOGPIXELSX);
		int nLOGPIXELSY = GetDeviceCaps(hDCScreen, LOGPIXELSY);
		ReleaseDC(NULL, hDCScreen);

		//размер исходной картинки
		long nWidthPix = (long)(nLOGPIXELSX * dWidthMm / (10 * ONE_INCH ) );
		long nHeightPix = (long)(nLOGPIXELSY * dWidthMm * dHeightMm / (10 * dWidthMm * ONE_INCH));
		// подгоняем под заданые размеры
		if (( m_nWidth > 0 ) && (m_nHeight > 0))
		{
			if( m_nResizeMode == 1 )
			{
				if( nWidthPix * m_nHeight > m_nWidth * nHeightPix )
				{
					nHeightPix = m_nWidth * nHeightPix / nWidthPix;
					nWidthPix = m_nWidth;
				}
				else
				{
					nWidthPix = m_nHeight * nWidthPix / nHeightPix;
					nHeightPix = m_nHeight;
				}
			}
			else
			{
				nWidthPix = m_nWidth;
				nHeightPix = m_nHeight;
			}
		}

		(*piImage)->Width = nWidthPix;
		(*piImage)->Height = nHeightPix;
		(*piImage)->put_Stride(0, nWidthPix * 4);

		(*piImage)->AspectRatioX = nWidthPix;
		(*piImage)->AspectRatioY = nHeightPix;

		(*piImage)->DataSize = 4 * nWidthPix * nHeightPix;

		(*piImage)->ColorSpace = 64 + (1 << 31);
		(*piImage)->AllocateBuffer(-1);

		memset((*piImage)->Buffer, 255, (*piImage)->BufferSize);

		CoCreateInstance( __uuidof( AVSGraphics::CAVSGraphicsRenderer ), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IAVSGraphicsRenderer), (void **)piRend  );
		//ставим FontManager
		VARIANT vtVariant;
		vtVariant.vt = VT_UNKNOWN;
		vtVariant.punkVal = NULL;
		(*piRend)->SetAdditionalParam( L"FontManager", vtVariant );

		(*piRend)->put_Width((float)dWidthMm);
        (*piRend)->put_Height((float)dHeightMm);

		(*piRend)->CreateFromMediaData( (*piImage), 0, 0, nWidthPix, nHeightPix );
	}
	private: void Clear()
	{
		if(NULL != m_piFileConverter)
		{
			m_piFileConverter->put_CommandRenderer( NULL );
			m_piFileConverter->put_Watermark( NULL );
		}
		m_nRendererError = 0;
		m_nLastProgress = -1;
		m_sFileInfo = _T("");
		m_bIsEdited = false;
		m_bPdfLoad = false;
		m_sFilename = _T("");
		m_sTitle = _T("");
		m_sTitle = _T("");
		m_sSubject = _T("");
		m_sCategory = _T("");
		m_sKeywords = _T("");
		m_sComments = _T("");
		m_sSource = _T("");
		m_sAuthor = _T("");
		m_sRevisionNumber = _T("");
		//m_sPassword = _T("");
		m_sOpenOptions = _T("");
		m_bParseFileComplete = false;
		m_nPermissions = PERMISSION_PRINT | PERMISSION_CHANGE | PERMISSION_COPY;
		m_nImagesCount = 0;
		m_nLastError = S_OK;
		//чистим папку с thumbnail
		if( _T("") != m_sThumbnailsPath )
		{
			Utils::RemoveDirOrFile( m_sThumbnailsPath );
			m_sThumbnailsPath.Empty();
		}
		m_mapThumbnails.RemoveAll();
	}
	private: void LoadStructuredStorageInformation( CString sFilename )
	{
		//если это docx, pptx, xslx распаковываем и берем информацию оттуда
		LONG nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		m_piFileConverter->get_FileType( &nFileType );
		if( AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFileType || AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX == nFileType || AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nFileType)
		{
			//распаковываем
			BSTR bstrFilename = m_sFilename.AllocSysString();
			CString sOptions = _T("<Options>");
			sOptions.AppendFormat( _T("<FileType>%d</FileType>"), nFileType );
			if( false == m_sPassword.IsEmpty() )
				sOptions.AppendFormat( _T("<Password>%s</Password>"), Utils::PrepareToXML( m_sPassword ) );
			sOptions.Append( m_sOpenOptions );
			sOptions.Append( _T("</Options>") );
			BSTR bstrOptions = sOptions.AllocSysString();
			HRESULT hRes = m_piFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
			SysFreeString(bstrFilename);
			SysFreeString(bstrOptions);
			if( FAILED( hRes ) )
				return;
			BSTR bstrTempDir;
			m_piFileConverter->get_TempDirectory(&bstrTempDir);
			CString sTempDir( bstrTempDir );
			SysFreeString( bstrTempDir );
			if( "" == sTempDir )
				return;
			XmlUtils::CXmlReader oXmlReader;
			//ищем в Rels где находятся свойства
			if( TRUE == oXmlReader.OpenFromFile( sTempDir + _T("\\[Content_Types].xml") ) )
				if( TRUE == oXmlReader.ReadRootNode( _T("Types") ) )
				{
					CString sCorePath;
					oXmlReader.ReadNodeList( _T("Override") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sType = oXmlReader.ReadNodeAttribute(i , _T("ContentType") ); 
						if( _T("application/vnd.openxmlformats-package.core-properties+xml") == sType )
						{
							sCorePath = oXmlReader.ReadNodeAttribute(i , _T("PartName") );
							//ставим 
							if( sCorePath.GetLength() > 0 && ( sCorePath[0] != '/'  || sCorePath[0] != '\\' ) )
								sCorePath.Insert(0, '/');
						}
					}
					if( false == sCorePath.IsEmpty() )
					{
						if( TRUE == oXmlReader.OpenFromFile( sTempDir + sCorePath ) )
							if( TRUE == oXmlReader.ReadRootNode( _T("cp:coreProperties") ) )
							{
								oXmlReader.ReadNodeList( _T("*") );
								for( int i = 0; i < oXmlReader.GetLengthList(); i++)
								{
									CString sNodeName = oXmlReader.ReadNodeName(i);
									CString sValue = oXmlReader.ReadNodeText(i);
									if ( _T("dc:title") == sNodeName )
										m_sTitle = sValue;
									else if( _T("dc:subject") == sNodeName )
										m_sSubject = sValue;
									else if( _T("keywords") == sNodeName || _T("cp:keywords") == sNodeName)
										m_sKeywords = sValue;
									else if( _T("dc:subject") == sNodeName )
										m_sSubject = sValue;
									else if( _T("dc:creator") == sNodeName )
										m_sAuthor = sValue;
									else if( _T("revision") == sNodeName || _T("cp:revision") == sNodeName)
										m_sRevisionNumber = sValue;
									else if( _T("dc:description") == sNodeName )
										m_sComments = sValue;
								}
							}
					}
				}
		}
		else
		{
			//пытаемся открыть поток файла или хранилища
			IStoragePtr piStorage = NULL;
			BSTR bstrFilename = sFilename.AllocSysString();
			HRESULT hRes = StgOpenStorageEx(bstrFilename, STGM_READ|STGM_SHARE_DENY_WRITE,
							STGFMT_ANY, 0, NULL,  NULL, 
							IID_IPropertySetStorage, (void**)&piStorage);
			SysFreeString( bstrFilename );
			if( FAILED(hRes) )
				return;

			IPropertySetStoragePtr piPropertySetStorage = NULL;
			hRes = piStorage->QueryInterface( IID_IPropertySetStorage, (void**)&piPropertySetStorage );
			if( FAILED(hRes) )
				return;
			//открываем набор свойств Summary для чтения
			IPropertyStoragePtr piPropertyStorage;
			hRes = piPropertySetStorage->Open(FMTID_SummaryInformation, STGM_READ|STGM_SHARE_EXCLUSIVE, &piPropertyStorage);
			if( FAILED(hRes) )
				return;

			//инициализируем массив свойств
			PROPSPEC aPropertyID[9];
			PROPVARIANT  aPropertyValue[9];
			//свойства по ID
			for( int i = 0; i < 9; i++ )
				aPropertyID[i].ulKind = PRSPEC_PROPID;
			//свойства по Имени
			//забиваем ID
			aPropertyID[0].propid = PIDSI_TITLE;//#define PIDSI_TITLE               0x00000002L  // VT_LPSTR
			aPropertyID[1].propid = PIDSI_SUBJECT;//#define PIDSI_SUBJECT             0x00000003L  // VT_LPSTR
			aPropertyID[2].propid = PIDSI_AUTHOR;//#define PIDSI_AUTHOR              0x00000004L  // VT_LPSTR
			aPropertyID[3].propid = PIDSI_KEYWORDS;//#define PIDSI_KEYWORDS            0x00000005L  // VT_LPSTR
			aPropertyID[4].propid = PIDSI_COMMENTS;//#define PIDSI_COMMENTS            0x00000006L  // VT_LPSTR
			aPropertyID[5].propid = PIDSI_TEMPLATE;//#define PIDSI_TEMPLATE            0x00000007L  // VT_LPSTR
			aPropertyID[6].propid = PIDSI_LASTAUTHOR;//#define PIDSI_LASTAUTHOR          0x00000008L  // VT_LPSTR
			aPropertyID[7].propid = PIDSI_REVNUMBER;//#define PIDSI_REVNUMBER           0x00000009L  // VT_LPSTR
			aPropertyID[8].propid = PIDSI_APPNAME;//#define PIDSI_APPNAME             0x00000012L  // VT_LPSTR
			//aPropertyID[9].propid = PIDSI_EDITTIME;//#define PIDSI_EDITTIME            0x0000000aL  // VT_FILETIME (UTC)
			//aPropertyID[10].propid = PIDSI_LASTPRINTED;//#define PIDSI_LASTPRINTED         0x0000000bL  // VT_FILETIME (UTC)
			//aPropertyID[11].propid = PIDSI_CREATE_DTM;//#define PIDSI_CREATE_DTM          0x0000000cL  // VT_FILETIME (UTC)
			//aPropertyID[12].propid = PIDSI_LASTSAVE_DTM;//#define PIDSI_LASTSAVE_DTM        0x0000000dL  // VT_FILETIME (UTC)
			//aPropertyID[13].propid = PIDSI_PAGECOUNT;//#define PIDSI_PAGECOUNT           0x0000000eL  // VT_I4
			//aPropertyID[14].propid = PIDSI_WORDCOUNT;//#define PIDSI_WORDCOUNT           0x0000000fL  // VT_I4
			//aPropertyID[15].propid = PIDSI_CHARCOUNT;//#define PIDSI_CHARCOUNT           0x00000010L  // VT_I4
			//aPropertyID[16].propid = PIDSI_DOC_SECURITY;//#define PIDSI_DOC_SECURITY        0x00000013L  // VT_I4
			//#define PIDSI_THUMBNAIL           0x00000011L  // VT_CF
			//считываем свойства

			hRes = piPropertyStorage->ReadMultiple( 9, aPropertyID, aPropertyValue);
			if( FAILED(hRes) )
				return;

			m_sTitle = aPropertyValue[0].pszVal;
			m_sSubject = aPropertyValue[1].pszVal;
			m_sAuthor = aPropertyValue[2].pszVal;
			m_sKeywords = aPropertyValue[3].pszVal;
			m_sComments = aPropertyValue[4].pszVal;
			m_sTemplate = aPropertyValue[5].pszVal;
			m_sLastAuthor = aPropertyValue[6].pszVal;
			m_sRevisionNumber = aPropertyValue[7].pszVal;
			m_sAppName = aPropertyValue[8].pszVal;

			//FILETIME ft1 = aPropertyValue[9].filetime;
			//FILETIME ft2 = aPropertyValue[10].filetime;
			//FILETIME ft3 = aPropertyValue[11].filetime;
			//FILETIME ft4 = aPropertyValue[12].filetime;
			//int n1 = aPropertyValue[13].intVal;
			//int n2 = aPropertyValue[14].intVal;
			//int n3 = aPropertyValue[15].intVal;
			//int n4 = aPropertyValue[16].intVal;
		}
	}
	private: void SaveStructuredStorageInformation( CString sFilename, PROPID oID, CString sValue  )
	{
		IStoragePtr piStorage = NULL;
		//открываем хранилище
		BSTR bstrFilename = sFilename.AllocSysString();
		HRESULT hRes = StgOpenStorageEx(bstrFilename, STGM_SHARE_EXCLUSIVE|STGM_READWRITE,
						STGFMT_ANY, 0, NULL,  NULL, 
						IID_IPropertySetStorage, (void**)&piStorage);
		SysFreeString( bstrFilename );
		if( FAILED(hRes) )
			return;

		IPropertySetStoragePtr piPropertySetStorage = NULL;
		hRes = piStorage->QueryInterface( IID_IPropertySetStorage, (void**)&piPropertySetStorage );
		if( FAILED( hRes ) )
			return;
		//открываем для записи набор Summary свойств
		IPropertyStoragePtr piPropertyStorage;
		hRes = piPropertySetStorage->Open(FMTID_SummaryInformation, STGM_READWRITE|STGM_SHARE_EXCLUSIVE, &piPropertyStorage);
		if( FAILED( hRes ) )
		{
			hRes = piPropertySetStorage->Create(FMTID_SummaryInformation, NULL, PROPSETFLAG_DEFAULT, STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, &piPropertyStorage);
			if( FAILED( hRes ) )
				return;
		}

		//инициализируем свойства
		PROPSPEC aPropertyID[1];
		PROPVARIANT  aPropertyValue[1];
		//свойства по ID
		aPropertyID[0].ulKind = PRSPEC_PROPID;
		aPropertyID[0].propid = oID;

		//заполняем свойства
		aPropertyValue[0].vt = VT_LPWSTR;
		aPropertyValue[0].pwszVal = sValue.GetBuffer();

		//записывам свойства
		hRes = piPropertyStorage->WriteMultiple( 1, aPropertyID, aPropertyValue, PID_FIRST_USABLE);
		if( FAILED( hRes ) )
			return;
		piPropertyStorage->Commit(STGC_DEFAULT);
		sValue.ReleaseBuffer();
	}
	static DWORD WINAPI _ThreadProc(void* pv)
	{
		CAVSOfficeFile *pThis = (CAVSOfficeFile *)pv;
		return pThis->ThreadProc();
	}
	private: DWORD ThreadProc()
	{
		if( NULL == m_piFileConverter )
		{
			m_nLastError = AVS_ERROR_UNEXPECTED;
			OnError( AVS_ERROR_UNEXPECTED );
			return 0;
		}
		CoInitialize( NULL );
		RELEASEINTERFACE( m_piOfficeViewer );
		CoCreateInstance( __uuidof(AVSGraphics::CAVSDocumentPainter), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IAVSDocumentPainter), (void **)(&m_piOfficeViewer)  );
		m_bStopParse = false;
		m_bParseFileComplete = false;
		HRESULT hRes = S_OK;
		long nFileType;
		long nConverterStatus = 0;

		//ждем пока не перейдет в свободное состояние
		m_piFileConverter->get_Status( &nConverterStatus );
		while( CONVERTER_LOAD_PROG == nConverterStatus )
		{
			Sleep( 200 );
			m_piFileConverter->get_Status( &nConverterStatus );
		}
		m_piFileConverter->get_FileType( &nFileType );
		VARIANT_BOOL vbIsLoadComplete = VARIANT_FALSE;
		m_piFileConverter->get_IsLoadComplete( &vbIsLoadComplete );

		//в конвертере CommandsRenderer всегда прогонен во Viewer до конца. 
		AVSGraphics::IAVSDocumentRendererPtr piConverterComRen = NULL;
		m_piFileConverter->get_CommandRenderer( (IUnknown**)&piConverterComRen );
		long nPageCount = 0;
		piConverterComRen->get_PageCount( &nPageCount );
		if( nPageCount > 0 )
		{
			for( int i = 0; i < nPageCount; i++ )
			{
				if( true == m_bStopParse )
					break;
				SHORT nCancel;
				OnCompleteParsePage();
				OnProgressParseFile( 0, (long)( c_nMaxProgressPercent * ( 1.0 * (i + 1) / nPageCount ) ), &nCancel );
			}
			//для pdf и форматов, которые конвертятся в docx считаем количество картинок
			BSTR bstrTempDir = NULL;
			m_piFileConverter->get_TempDirectory( &bstrTempDir );
			UpdateImagesCount( (CString)bstrTempDir, nFileType );
			SysFreeString( bstrTempDir );

			m_bParseFileComplete = true;
			m_nStatus = c_shOfficeFileParsingComplete;
			OnCompleteParseFile();
			CoUninitialize();
			return 0;
		}

		//pdf не делает DrawingXml, для него всегда делаем load
		if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFileType || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU == nFileType || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == nFileType )
		{
			//цепляем события
			COfficeFileConvEvents< _IAVSOfficeFileConverterEvents, CAVSOfficeFile >* oFileConvEvents = new COfficeFileConvEvents< _IAVSOfficeFileConverterEvents, CAVSOfficeFile >(this);
			oFileConvEvents->AddRef();
			oFileConvEvents->Advise( m_piFileConverter );
			m_bPdfLoad = true;
			//Pdf опции
			CString sPdfOptions = _T("<Options>");
			sPdfOptions.AppendFormat( _T("<FileType>%d</FileType>"), nFileType );
			if( false == m_sPassword.IsEmpty() )
				sPdfOptions.AppendFormat( _T("<Password>%s</Password>"), Utils::PrepareToXML( m_sPassword ) );
			sPdfOptions.Append( m_sOpenOptions );
			sPdfOptions.Append( _T("<Display/>") );
			sPdfOptions.Append( _T("</Options>") );
			//конвертируем
			BSTR bstrFilename = m_sFilename.AllocSysString();
			BSTR bstrOptions = sPdfOptions.AllocSysString();
			hRes = m_piFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
			SysFreeString(bstrFilename);
			SysFreeString(bstrOptions);
			if( FAILED( hRes ) )
			{
				m_bParseFileComplete = true;
				//отцепляем события
				oFileConvEvents->UnAdvise( m_piFileConverter );
				RELEASEINTERFACE( oFileConvEvents );
				m_nStatus = c_shOfficeFileOpen;
				m_nLastError = hRes;
				OnError( hRes );
				CoUninitialize();
				return 1;
			}
			VARIANT_BOOL bComplete = VARIANT_FALSE;
			while( VARIANT_FALSE == bComplete)
			{
				m_piFileConverter->get_IsLoadComplete( &bComplete );
				Sleep(200);
			}
			//отцепляем события
			oFileConvEvents->UnAdvise( m_piFileConverter );
			RELEASEINTERFACE( oFileConvEvents );
			m_bPdfLoad = false;
			m_bParseFileComplete = true;
			m_nStatus = c_shOfficeFileParsingComplete;
			//все OnCompleteParsePage должны были быть отправлены в ивентах COfficeFileConvEvents
			OnCompleteParseFile();
			CoUninitialize();
			return 0;
		}
		else
		{
			//проверяем не сделан ли load
			if( VARIANT_FALSE == vbIsLoadComplete )
			{
				//цепляем события
				COfficeFileConvEvents< _IAVSOfficeFileConverterEvents, CAVSOfficeFile >* oFileConvEvents = new COfficeFileConvEvents< _IAVSOfficeFileConverterEvents, CAVSOfficeFile >(this);
				oFileConvEvents->AddRef();
				oFileConvEvents->Advise( m_piFileConverter );

				BSTR bstrFilename = m_sFilename.AllocSysString();
				CString sOptions = _T("<Options>");
				sOptions.AppendFormat( _T("<FileType>%d</FileType>"), nFileType );
				if( false == m_sPassword.IsEmpty() )
					sOptions.AppendFormat( _T("<Password>%s</Password>"), Utils::PrepareToXML( m_sPassword ) );
				sOptions.Append( m_sOpenOptions );
				sOptions.Append( _T("</Options>") );
				BSTR bstrOptions = sOptions.AllocSysString();
				hRes = m_piFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
				SysFreeString(bstrFilename);
				SysFreeString(bstrOptions);

				//отцепляем события
				oFileConvEvents->UnAdvise( m_piFileConverter );
				RELEASEINTERFACE( oFileConvEvents );

				if( FAILED( hRes ) )
				{
					m_bParseFileComplete = true;
					m_nStatus = c_shOfficeFileOpen;
					m_nLastError = hRes;
					OnError( hRes );
					CoUninitialize();
					return 1;
				}
			}
			if( true == m_bStopParse )
			{
				m_bParseFileComplete = true;
				m_nStatus = c_shOfficeFileOpen;
				CoUninitialize();
				return 0;
			}
			//для pdf и форматов, которые конвертятся в docx считаем количество картинок
			BSTR bstrTempDir = NULL;
			m_piFileConverter->get_TempDirectory( &bstrTempDir );
			UpdateImagesCount( (CString)bstrTempDir, nFileType );
			SysFreeString( bstrTempDir );

			////load то сделан а CommandRenderer не создан, а то бы выше вышли бы из функции
			//RELEASEINTERFACE(m_piCommandsRenderer);
			//m_piFileConverter->put_CommandRenderer( NULL );
			//m_piFileConverter->put_Watermark( NULL );

			BSTR bstrDrawingXml;
			hRes = m_piFileConverter->get_DrawingXml(&bstrDrawingXml);
			if( FAILED( hRes ) )
			{
				m_bParseFileComplete = true;
				m_nStatus = c_shOfficeFileOpen;
				m_nLastError = hRes;
				OnError( hRes );
				CoUninitialize();
				return 1;
			}
			if( true == m_bStopParse )
			{
				m_bParseFileComplete = true;
				m_nStatus = c_shOfficeFileOpen;
				CoUninitialize();
				return 0;
			}
			CString sEncodedXml = CString(bstrDrawingXml);
			SysFreeString(bstrDrawingXml);
			//получаем ключ для полученной xml
			VARIANT vEncoder;
			vEncoder.vt = VT_UNKNOWN;
			hRes = m_piFileConverter->GetAdditionalParam( L"decoderkey", &vEncoder);
			//посылаем код для расшифровки
			BSTR bstrAdditionalParamName = g_csBlowfishKeyParamName.AllocSysString();
			hRes = m_piOfficeViewer->SetAdditionalParam( bstrAdditionalParamName, vEncoder);
			SysFreeString(bstrAdditionalParamName);
			RELEASEINTERFACE( vEncoder.punkVal );
			if( FAILED( hRes ) )
			{
				m_bParseFileComplete = true;
				m_nStatus = c_shOfficeFileOpen;
				m_nLastError = hRes;
				OnError( hRes );
				CoUninitialize();
				return 1;
			}
			//ставим защифрованную xml
			BSTR bstrEncodedXml = sEncodedXml.AllocSysString();
			hRes = m_piOfficeViewer->SetXml(bstrEncodedXml);
			SysFreeString(bstrEncodedXml);
			if( FAILED( hRes ) )
			{
				m_bParseFileComplete = true;
				m_nStatus = c_shOfficeFileOpen;
				m_nLastError = hRes;
				OnError( hRes );
				CoUninitialize();
				return 1;
			}
			m_piOfficeViewer->AddRenderer( piConverterComRen );

			if( true == m_bStopParse )
			{
				m_bParseFileComplete = true;
				m_nStatus = c_shOfficeFileOpen;
				CoUninitialize();
				return 0;
			}
			//цепляем события
			m_oEventViewer->Advise( m_piOfficeViewer );

			hRes = m_piOfficeViewer->Start();

			if( FAILED( hRes ) )
			{
				m_bParseFileComplete = true;
				m_nStatus = c_shOfficeFileOpen;
				m_nLastError = hRes;
				OnError( hRes );
				CoUninitialize();
				return 1;
			}
			//ждем окончания
			WaitForSingleObject( m_hViewerStop, INFINITE );
			RELEASEINTERFACE( m_piOfficeViewer );
			CoUninitialize();
			return 0;
		}
	}
private: void FitToPage( float fSourceWidth, float  fSourceHeight, float  fTargetWidth, float fTargetHeight, float& fResX, float& fResY, float& fResWidth, float& fResHeight )
		 {
			if( fSourceWidth * fTargetHeight > fTargetWidth * fSourceHeight )
			{
				fResHeight = fTargetWidth * fSourceHeight / fSourceWidth;
				fResWidth = fTargetWidth;

				fResX = 0;
				fResY = fTargetHeight / 2 - fResHeight / 2;
			}
			else
			{
				fResWidth = fTargetHeight * fSourceWidth / fSourceHeight;
				fResHeight = fTargetHeight;
				fResY = 0;
				fResX = fTargetWidth / 2 - fResWidth / 2;
			}
		 }
 private: void UpdateImagesCount( CString sDirectory, LONG nFiletype )
		 {
			//для PDF параметр считан в методе Open
			//пробуем для всех форматов(работает только для структуры docx)
			if( 0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nFiletype) || 0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFiletype) )
			{
				m_aRelsWalked.RemoveAll();
				m_aImagesToExtract.RemoveAll();
				m_nImagesCount = 0;
				NormalizePath( sDirectory, true );
				UpdateDocxImagesCountFromRels( sDirectory + _T("_rels/.rels"), sDirectory );
			}
		 }
private: void UpdateDocxImagesCountFromRels( CString sSourceRels, CString sSourcePath )
		{
			//нормализуем имя
			TCHAR  buffer[1024]; 
			GetFullPathName( sSourceRels, 1024, buffer, NULL );
			sSourceRels = CString( buffer );

			XmlUtils::CXmlReader oXmlReader;
			//проверяем наличие файла
			if( ::GetFileAttributes( sSourceRels ) != DWORD(-1) )
			{
				for( int i = 0; i < m_aRelsWalked.GetCount(); i++ )
					if( m_aRelsWalked[i] == sSourceRels )
						return;
				m_aRelsWalked.Add( sSourceRels );
				if( TRUE == oXmlReader.OpenFromFile( sSourceRels ) )
					if( TRUE == oXmlReader.ReadRootNode( _T("Relationships") ) )
						if( TRUE == oXmlReader.ReadNodeList( _T("Relationship") ) )
						{
							for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
							{
								CString sTargetMode = oXmlReader.ReadNodeAttribute( i, _T("TargetMode") );
								if( _T("External") != sTargetMode )
								{
									CString sType = oXmlReader.ReadNodeAttribute( i, _T("Type") );
									CString sTargetRel = oXmlReader.ReadNodeAttribute( i, _T("Target") );
									NormalizePath( sTargetRel, false );
									CString sTargetFile = sSourcePath + sTargetRel;

									TCHAR tDrive[256];
									TCHAR tDir[256];
									TCHAR tFilename[256];
									TCHAR tExt[256];
									_tsplitpath( sTargetFile, tDrive, tDir, tFilename, tExt );
									CString sDrive = CString( tDrive );
									CString sDir = CString( tDir );
									CString sFilename = CString( tFilename );
									CString sExt = CString( tExt );

									if( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image") == sType )
									{
										//нормализуем имя
										GetFullPathName( sTargetFile, 1024, buffer, NULL );
										sTargetFile = CString( buffer );

										bool bDuplicate = false;
										for( int j = 0; j < m_aImagesToExtract.GetCount(); j++ )
											if( m_aImagesToExtract[j] == sTargetFile )
											{
												bDuplicate = true;
												break;
											}
										if( false == bDuplicate )
										{
											m_aImagesToExtract.Add( sTargetFile );
											m_nImagesCount++;
										}
									}
									else//пытаемся прочитать rels этого файла
										UpdateDocxImagesCountFromRels( sDrive + sDir + _T("_rels/") + sFilename + sExt + _T(".rels"), sDrive + sDir );
								}	
							}
						}
			}
		}
// bEnd == true (делает чтобы в конце был '/' или '\'), bEnd == false (делает чтобы в начале не было '/' или '\')
private: void NormalizePath( CString& sPath, bool bEnd = true )
		 {
			 int nPathLen = sPath.GetLength();
			 if( nPathLen > 0 ) // sPath должен заканчиваться '/' или '\'
			 {
				 if( true == bEnd )
				 {
					 TCHAR tcLastChar = sPath[ nPathLen - 1 ];
					 if( '\\' != tcLastChar && '/' != tcLastChar )
						 sPath.AppendChar( '/' );
				 }
				 else
				 {
					 TCHAR tcFirstChar = sPath[ nPathLen - 1 ];
					 if( '\\' == tcFirstChar || '/' == tcFirstChar )
						 sPath = sPath.Right( sPath.GetLength() - 1 );
				 }
			 }
		 }
};
