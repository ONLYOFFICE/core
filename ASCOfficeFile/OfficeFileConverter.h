#pragma once
#include "OfficeFileEvents.h"
#include "../Common/OfficeFileTemplate.h"
#include "../Common/StringUtils.h"
#include "../Common/MediaFormatDefine.h"
#include "StorageConverters.h"
#include "../Common/RSA/XMLDecoder.h"
#include "../Common/GdiPlusEx.h"

//#define AVS_REMOVE_REG
#ifdef AVS_REMOVE_REG
#include "Registration.h"
#endif

#define ONE_INCH 2.54
#define TO_FLOAT 10000

#define CONVERTER_IDLE 0
#define CONVERTER_LOAD_PROG 1
#define CONVERTER_LOAD_COMP 2
#define CONVERTER_SAVE_PROG 3
#define CONVERTER_SAVE_COMP 4

// IAVSOfficeFile
[
	object,
	uuid("4b1363f0-d80d-11de-8a39-0800200c9a66"),
	dual,	helpstring("IAVSOfficeFileTemplate Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeFileConverter : public IAVSOfficeFileTemplate
{
	[id(10), propget] HRESULT FileType( [out, retval] LONG* pnType );
	[id(10), propput] HRESULT FileType( [in] LONG nType );

	[id(20), propget] HRESULT DrawingXml( [out, retval] BSTR* pbstrXml );

	[id(60), propget] HRESULT CommandRenderer( [out, retval] IUnknown** ppunkRend );
	[id(60), propput] HRESULT CommandRenderer( [in] IUnknown* punkRend );

	[id(70), propget] HRESULT Watermark( [out, retval] BSTR* pbstrVal );
	[id(70), propput] HRESULT Watermark( [in] BSTR bstrVal );

	[id(80)] HRESULT DrawWatermark( [in] IUnknown* punkImage,[in] LONG nWidth,[in] LONG nHeight );

	[id(90), propget] HRESULT TempDirectory( [out, retval] BSTR* pbstrDir );
	[id(90), propput] HRESULT TempDirectory( [in] BSTR bstrDir );

	[id(100), propget] HRESULT IsLoadComplete( [out, retval] VARIANT_BOOL* pvIsLoadComplete );
	[id(110), propget] HRESULT Status([out, retval] LONG* pVal);

	[id(120)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(130)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// _IAVSOfficeFileConverterEvents
[
	dispinterface,
	uuid("ae195a70-f231-11de-8a39-0800200c9a66"),
	helpstring("_IAVSOfficeFileConverterEvents Interface")
]

__interface _IAVSOfficeFileConverterEvents
{
	[id(1), helpstring("method OnProgress")] HRESULT OnProgress([in] LONG nID, [in] LONG nPercent);
	[id(2), helpstring("method OnProgressEx")] HRESULT OnProgressEx( [in] LONG nID, [in] LONG nPercent, [in, out, ref] SHORT* Cancel );
	[id(10), helpstring("method OnCompletePage")] HRESULT OnCompletePage();
};

[
	coclass,
	default(IAVSOfficeFileConverter, _IAVSOfficeFileConverterEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeFile.AVSDocume"),
	progid("AVSOfficeFile.AVSDocu.1"),
	version(1.0),
	uuid("6dec3f60-d80c-11de-8a39-0800200c9a66"),
	helpstring("AVSOfficeFile Class")
]
class ATL_NO_VTABLE CAVSOfficeFileConverter : public IAVSOfficeFileConverter
{

private: long m_nRendererError; //ошибка при рендринге
private: HANDLE m_hViewerStop; //окончание рендеринга в файл

private: int m_nFileType;
private: int m_nOpenFileType;
private: CString m_sDrawingXML;
private: CString m_sMetadata;
private: AVSGraphics::IASCDocumentRenderer* m_piCommandsRenderer;
private: CString m_sWatermark;

private: bool m_bIsInternalTemDir;
private: CString m_sTempDirectory;
private: CString m_sRootTempDirectory; //директория в которой размещена темповая директория

private: CRITICAL_SECTION m_csThread;
private: StorageConverters m_oStorageConverters;
private: CCommandRendererEvents< AVSGraphics::_IASCDocumentRendererEvents, CAVSOfficeFileConverter >* m_oCommandRendererEvent;
private: LPSTREAM m_pStream;
private: long m_nStatus; //CONVERTER_IDLE - свободен; CONVERTER_WORK - занят
#ifdef AVS_REMOVE_REG
private: CRegistratorServer* m_poRegServer;
#endif
private: double m_dMaxProgress; //верняя граница
private: double m_dMinProgress; //нижняя граница
private: bool m_bCanceled; //если SHORT* Cancel в OnProgressDocumentEx станет c_shProgressCancel
private: long m_nLastProgress;
private: long m_nLastProgressEx;
private: IUnknown* m_piDecoderKey;
private: int m_nMaxPageLoad;
private: int m_nSaveMode;//Для сохранения нескольких файлов в один
private: IUnknownPtr m_piSaveRenderer;//Для сохранения нескольких файлов в один
private: bool m_bSaveRendererStopped;//Для сохранения нескольких файлов в один
private: CGdiPlusInit m_oGdiPlusInit;
public:
	CAVSOfficeFileConverter():m_piCommandsRenderer(NULL),m_oCommandRendererEvent(NULL),m_piDecoderKey(NULL)
	{
		m_sWatermark = "";
		m_piSaveRenderer = NULL;
		CoCreateInstance( __uuidof(AVSGraphics::CASCDocumentRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IASCDocumentRenderer), (void **)(&m_piCommandsRenderer)  );

		m_hViewerStop = CreateEvent( NULL, FALSE, FALSE, NULL);
#ifdef AVS_REMOVE_REG
		m_poRegServer = new CRegistratorServer();
#endif
		Clear();
		m_nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		m_oCommandRendererEvent = new CCommandRendererEvents< AVSGraphics::_IASCDocumentRendererEvents, CAVSOfficeFileConverter >(this);
		m_oCommandRendererEvent->AddRef();
	}
	__event __interface _IAVSOfficeFileConverterEvents;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		InitializeCriticalSection(&m_csThread);
		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEINTERFACE( m_oCommandRendererEvent );
		RELEASEINTERFACE( m_piCommandsRenderer );
		RELEASEHANDLE( m_hViewerStop );
		Clear();
		DeleteCriticalSection(&m_csThread);
#ifdef AVS_REMOVE_REG
		RELEASEOBJECT( m_poRegServer );
#endif
	}
public:
	void OnProgressDocumentEx( LONG nID, LONG nPercent, SHORT* Cancel )
	{
		long nCurProgress = GetProgress( nPercent );
		if( nCurProgress > m_nLastProgressEx )
		{
			m_nLastProgressEx = nCurProgress;
			OnProgressEx( nID, nCurProgress, Cancel );
		}
		//останавливаем конвертацию если количество отрендереных страниц превысило порог
		if( -1 != m_nMaxPageLoad )
		{
			long nPageCount = 0;
			if( NULL != m_piCommandsRenderer )
				m_piCommandsRenderer->get_PageCount( &nPageCount );
			if( nPageCount >= m_nMaxPageLoad )
				(*Cancel) = c_shProgressCancel;//останавливаем конвертацию
		}

		if( c_shProgressContinue != (*Cancel) )
			m_bCanceled = true;
	}
	void OnProgressDocument( LONG nID, LONG nPercent)
	{
		long nCurProgress = GetProgress( nPercent );
		if( nCurProgress > m_nLastProgress )
		{
			m_nLastProgress = nCurProgress;
			OnProgress( nID, nCurProgress );
		}
	}
	void OnCompletePageDocument()
	{
		OnCompletePage();
	}
	void OnErrorViewer(LONG lError)
	{
		m_nRendererError = lError;
		//сигнализируем об окончании
		SetEvent( m_hViewerStop );
	}
	void OnStopViewer()
	{
		//сигнализируем об окончании
		SetEvent( m_hViewerStop );
	}

	void OnProgressViewer(LONG lProgressPage)
	{
		OnProgress( 0, GetProgress( lProgressPage * 10000 ) );
	}
	void OnNewPageViewer(double dWidthMm, double dHeightMm)
	{
		if( -1 != m_nMaxPageLoad )
		{
			long nPageCount = 0;
			if( NULL != m_piCommandsRenderer )
				m_piCommandsRenderer->get_PageCount( &nPageCount );
			//if( nPageCount >= m_nMaxPageLoad && NULL != m_piOfficeViewer )
				//m_piOfficeViewer->Stop();
		}
	}
	void OnCompletePageViewer()
	{
	}
	void OnProgressParse(  LONG lType, LONG lProgress )
	{
	}
public:
	STDMETHOD(LoadFromFile)( BSTR bstrSrcFileName,  BSTR bstrDstPath,  BSTR bstrXMLOptions)
	{
		// инициализируем gdiplus
		if (!m_oGdiPlusInit.IsValid())
		{
			if (!m_oGdiPlusInit.Init())
				return AVS_ERROR_UNEXPECTED;
		}
		m_bCanceled = false;
		m_nLastProgress = -1;
		m_nLastProgressEx = -1;
		//проверяем наличие файла
		if( ::GetFileAttributes( bstrSrcFileName ) == INVALID_FILE_ATTRIBUTES )
			return AVS_ERROR_FILEACCESS;

		//CTemporaryCS oCS(&m_csThread);
		CString sFilename(bstrSrcFileName);
		CString sDstPath(bstrDstPath);
		CString sOptions(bstrXMLOptions);
		Clear();
		m_nStatus = CONVERTER_LOAD_PROG;
		int nOpenFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		AVSOfficeFormatChecker avsOfficeFormatChecker;

		bool bNoStandartOpen = false;
		long nNoStandartType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		CString sNoStandartPath;

		//проверяем bstrXMLOptions на наличие FileType
		//bool bFileTypeInOptions = false;
		XmlUtils::CXmlReader oOptionsReader;
		if( TRUE == oOptionsReader.OpenFromXmlString( bstrXMLOptions ) )
			if( TRUE == oOptionsReader.ReadRootNode( _T("Options") ) )
			{
				CString sFileType = oOptionsReader.ReadNodeValue( _T("FileType") );
				if( _T("") != sFileType )
				{
					nOpenFileType = Strings::ToInteger( sFileType );
					//bFileTypeInOptions = true;
				}
				CString sMaxPageLoad = oOptionsReader.ReadNodeValue( _T("MaxPageLoad") );
				if( _T("") != sMaxPageLoad )
					m_nMaxPageLoad = Strings::ToInteger( sMaxPageLoad );

				if( TRUE == oOptionsReader.ReadNode( _T("NoStandartOpen") ) )
				{
					bNoStandartOpen = true;
					sNoStandartPath = oOptionsReader.ReadNodeAttribute( _T("destinationpath") );
					nNoStandartType = Strings::ToInteger( oOptionsReader.ReadNodeAttribute( _T("destinationtype") ) );
				}
			}
		if( AVS_OFFICESTUDIO_FILE_UNKNOWN == nOpenFileType )
			nOpenFileType = avsOfficeFormatChecker.GetFileFormat( sFilename );

		if( _T("") != sDstPath )
			m_sTempDirectory = sDstPath;
		else 
		{
			m_sTempDirectory = Utils::CreateTempDir( m_sRootTempDirectory );
			if( _T("") == m_sTempDirectory )
			{
				m_nStatus = CONVERTER_IDLE;
				return AVS_OFFICEFILE_ERROR_CREATE_TEMP;
			}
			m_bIsInternalTemDir = true;
		}
		CString sXMLOptions( bstrXMLOptions );

		//выбираем нужный контрол
		IAVSOfficeFileTemplate* piConverter = NULL;
		if( FALSE == m_oStorageConverters.GetConverter( nOpenFileType , &piConverter) )
		{
			m_nStatus = CONVERTER_IDLE;
			return AVS_ERROR_FILEFORMAT;
		}
		//цепляем события
		COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >* oEventDocuments = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >(this);
		oEventDocuments->AddRef();
		oEventDocuments->Advise( piConverter );
		COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >* oEventDocuments2 = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >(this);
		oEventDocuments2->AddRef();
		oEventDocuments2->Advise( piConverter );

		HRESULT hRes = S_OK;
		bool bDisplay = true;//что делать с pdf решается в опциях ( либо Display, либо ExtractImages)
		if( -1 != sXMLOptions.Find( _T("Display") ) )
			bDisplay = true;
		else
			bDisplay = false;
		m_dMinProgress = 0;
		m_dMaxProgress = 1;
		//для pdf ставим CommandRender
		if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nOpenFileType && true == bDisplay )
		{
			RELEASEINTERFACE( m_piCommandsRenderer );
			CoCreateInstance( __uuidof(AVSGraphics::CASCDocumentRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IASCDocumentRenderer), (void **)(&m_piCommandsRenderer)  );
			AVSOfficePDFFile::IPDFFile* piPdfFile = NULL;	
			piConverter->QueryInterface( __uuidof(AVSOfficePDFFile::IPDFFile), (void**)&piPdfFile );
			if( NULL != piPdfFile )
				piPdfFile->put_CommandRenderer( m_piCommandsRenderer );
			RELEASEINTERFACE(piPdfFile);
			m_oCommandRendererEvent->Advise( m_piCommandsRenderer );

			// начинаем конвертацию
			BSTR bstrTempFolder = m_sTempDirectory.AllocSysString();
			BSTR bstrFilename = sFilename.AllocSysString();
			hRes = piConverter->LoadFromFile( bstrFilename, bstrTempFolder ,bstrXMLOptions );
			SysFreeString( bstrTempFolder );
			SysFreeString( bstrFilename );
			//отцепляем события от CommandsRenderer
			m_oCommandRendererEvent->UnAdvise( m_piCommandsRenderer );
		}	
		else if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU == nOpenFileType )
		{
			if( NULL != m_piCommandsRenderer )
				RELEASEINTERFACE( m_piCommandsRenderer );
			CoCreateInstance( __uuidof(AVSGraphics::CASCDocumentRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IASCDocumentRenderer), (void **)(&m_piCommandsRenderer)  );
			OfficeDjVu::IDjVuFile* piDjVuFile = NULL;	
			piConverter->QueryInterface( __uuidof(OfficeDjVu::IDjVuFile), (void**)&piDjVuFile );
			if( NULL != piDjVuFile )
			{
				piDjVuFile->put_DocumentRenderer( m_piCommandsRenderer );
			}
			RELEASEINTERFACE(piDjVuFile);
			m_oCommandRendererEvent->Advise( m_piCommandsRenderer );

			// начинаем конвертацию
			BSTR bstrTempFolder = m_sTempDirectory.AllocSysString();
			BSTR bstrFilename = sFilename.AllocSysString();
			hRes = piConverter->LoadFromFile( bstrFilename, bstrTempFolder ,bstrXMLOptions );
			SysFreeString( bstrTempFolder );
			SysFreeString( bstrFilename );
			//отцепляем события от CommandsRenderer
			m_oCommandRendererEvent->UnAdvise( m_piCommandsRenderer );
		}
		else if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == nOpenFileType )
		{
			if( NULL != m_piCommandsRenderer )
				RELEASEINTERFACE( m_piCommandsRenderer );
			CoCreateInstance( __uuidof(AVSGraphics::CASCDocumentRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IASCDocumentRenderer), (void **)(&m_piCommandsRenderer)  );
			OfficeXPS::IXPSFile* piXpsFile = NULL;	
			piConverter->QueryInterface( __uuidof(OfficeXPS::IXPSFile), (void**)&piXpsFile );
			if( NULL != piXpsFile )
			{
				piXpsFile->put_DocumentRenderer( m_piCommandsRenderer );
			}
			RELEASEINTERFACE(piXpsFile);
			m_oCommandRendererEvent->Advise( m_piCommandsRenderer );

			// начинаем конвертацию
			BSTR bstrTempFolder = m_sTempDirectory.AllocSysString();
			BSTR bstrFilename = sFilename.AllocSysString();
			hRes = piConverter->LoadFromFile( bstrFilename, bstrTempFolder ,bstrXMLOptions );
			SysFreeString( bstrTempFolder );
			SysFreeString( bstrFilename );

			//отцепляем события от CommandsRenderer
			m_oCommandRendererEvent->UnAdvise( m_piCommandsRenderer );
		}
		else if( 0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & nOpenFileType) && 
			( false == bNoStandartOpen || AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nNoStandartType) )
		{
			if( AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nNoStandartType )
			{
				m_dMinProgress = 0.000;
				m_dMaxProgress = 0.500;
			}
			else
			{
				m_dMinProgress = 0.000;
				m_dMaxProgress = 0.333;
			}
			SHORT shCancel = c_shProgressContinue;
			OnProgressDocumentEx( 0 , 0, &shCancel );
			if( c_shProgressContinue == shCancel )
			{
				//преобразуем в папаку с XLSX
				CString sXlsxPath = Utils::CreateTempDir( m_sTempDirectory );

				if( true == sXlsxPath.IsEmpty() )
					hRes == AVS_OFFICEFILE_ERROR_CREATE_TEMP;
				IAVSOfficeFileTemplate* piXlsxConverter = NULL;
				if( SUCCEEDED( hRes ) )
				{
					if( AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS == nOpenFileType )
					{
						if( FALSE == m_oStorageConverters.GetConverter( AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS , &piXlsxConverter) )
							hRes = AVS_ERROR_UNEXPECTED;
					}
					else
					{
						if( FALSE == m_oStorageConverters.GetConverter( AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX , &piXlsxConverter) )
							hRes = AVS_ERROR_UNEXPECTED;
					}
				}
				if( SUCCEEDED( hRes ) )
				{
					//цепляем события
					COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >* oSubEventDocuments = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >(this);
					oSubEventDocuments->AddRef();
					oSubEventDocuments->Advise( piXlsxConverter );
					COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >* oSubEventDocuments2 = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >(this);
					oSubEventDocuments2->AddRef();
					oSubEventDocuments2->Advise( piXlsxConverter );

					BSTR bstrTempFolder = sXlsxPath.AllocSysString();
					BSTR bstrFilename = sFilename.AllocSysString();
					hRes = piConverter->LoadFromFile( bstrFilename, bstrTempFolder , L"" );
					SysFreeString( bstrTempFolder );
					SysFreeString( bstrFilename );

					//отцепляем события
					oSubEventDocuments->UnAdvise( piXlsxConverter );
					RELEASEINTERFACE(oSubEventDocuments);
					oSubEventDocuments2->UnAdvise( piXlsxConverter );
					RELEASEINTERFACE(oSubEventDocuments2);

				}
				//Преобразум xlsx -> html -> docx
				RELEASEINTERFACE( piConverter );
				if( false == m_bCanceled )
				{
					if( AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == nNoStandartType )
					{
						m_dMinProgress = 0.500;
						m_dMaxProgress = 1;
					}
					else
					{
						m_dMinProgress = 0.333;
						m_dMaxProgress = 0.666;
					}
					OnProgressDocumentEx( 0 , 0, &shCancel );
					if( c_shProgressContinue == shCancel && SUCCEEDED( hRes ) )
					{
						//XLSX -> HTML
						CString sHtmlDir = _T("");
						CString sHtmlFile = _T("");
						if( false == bNoStandartOpen )
						{
							sHtmlDir = Utils::CreateTempDir( m_sTempDirectory );
							if( true == sHtmlDir.IsEmpty() )
								hRes == AVS_OFFICEFILE_ERROR_CREATE_TEMP;
							sHtmlFile = sHtmlDir + _T("\\Temp.htm");
						}
						else
							sHtmlFile = sNoStandartPath;

						if( SUCCEEDED( hRes ) )
						{
							IAVSOfficeFileTemplate* piConverter = NULL;
							if( FALSE == SUCCEEDED (CoCreateInstance( __uuidof( WSHtmlFile::CWSHtmlFile ) , NULL, CLSCTX_INPROC_SERVER, __uuidof(IAVSOfficeFileTemplate), (void **)(&piConverter) ) ) )
								hRes = AVS_ERROR_FILEFORMAT;
							if( SUCCEEDED( hRes ) )
							{
								//цепляем события
								COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >* oSubEventDocuments = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >(this);
								oSubEventDocuments->AddRef();
								oSubEventDocuments->Advise( piConverter );
								COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >* oSubEventDocuments2 = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >(this);
								oSubEventDocuments2->AddRef();
								oSubEventDocuments2->Advise( piConverter );

								BSTR bstrTempFolder = sXlsxPath.AllocSysString();
								BSTR bstrFilename = sHtmlFile.AllocSysString();
								hRes = piConverter->SaveToFile( bstrFilename, bstrTempFolder , L"<Options><FileType>257</FileType><HTML><SingleFile val = \"true\"/></HTML></Options>" );
								SysFreeString( bstrTempFolder );
								SysFreeString( bstrFilename );

								//отцепляем события
								oSubEventDocuments->UnAdvise( piConverter );
								RELEASEINTERFACE(oSubEventDocuments);
								oSubEventDocuments2->UnAdvise( piConverter );
								RELEASEINTERFACE(oSubEventDocuments2);
							}
							RELEASEINTERFACE( piConverter );
						}
						if( false == bNoStandartOpen && false == m_bCanceled )
						{
							m_dMinProgress = 0.666;
							m_dMaxProgress = 1;
							OnProgressDocumentEx( 0 , 0, &shCancel );
							
							//HTML -> DOCX
							if( c_shProgressContinue == shCancel && SUCCEEDED( hRes ) )
							{
								IAVSOfficeFileTemplate* piConverter = NULL;
								if( FALSE == m_oStorageConverters.GetConverter( AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML , &piConverter) )
									hRes = AVS_ERROR_FILEFORMAT;
								if( SUCCEEDED( hRes ) )
								{
									//цепляем события
									COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >* oSubEventDocuments = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >(this);
									oSubEventDocuments->AddRef();
									oSubEventDocuments->Advise( piConverter );
									COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >* oSubEventDocuments2 = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >(this);
									oSubEventDocuments2->AddRef();
									oSubEventDocuments2->Advise( piConverter );

									BSTR bstrTempFolder = m_sTempDirectory.AllocSysString();
									BSTR bstrFilename = sHtmlFile.AllocSysString();
									hRes = piConverter->LoadFromFile( bstrFilename, bstrTempFolder , L"<Options><FileType>70</FileType></Options>" );
									SysFreeString( bstrTempFolder );
									SysFreeString( bstrFilename );

									//отцепляем события
									oSubEventDocuments->UnAdvise( piConverter );
									RELEASEINTERFACE(oSubEventDocuments);
									oSubEventDocuments2->UnAdvise( piConverter );
									RELEASEINTERFACE(oSubEventDocuments2);
								}
								RELEASEINTERFACE( piConverter );
							}

						}
						m_dMinProgress = 1;
						m_dMaxProgress = 1;
						OnProgressDocumentEx( 0 , 0, &shCancel );

						Utils::RemoveDirOrFile( sHtmlDir );
					}
				}
				Utils::RemoveDirOrFile( sXlsxPath );
			}
		}
		else
		{
			// начинаем конвертацию
			BSTR bstrTempFolder = m_sTempDirectory.AllocSysString();
			BSTR bstrFilename = sFilename.AllocSysString();
			hRes = piConverter->LoadFromFile( bstrFilename, bstrTempFolder ,bstrXMLOptions );
			SysFreeString( bstrTempFolder );
			SysFreeString( bstrFilename );

			if(SUCCEEDED(hRes))
			{
				//Считываем метаданные
				if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nOpenFileType)
				{
					OfficeEpub::IOfficeEpubFile* piEpubFile = NULL;
					piConverter->QueryInterface( __uuidof(OfficeEpub::IOfficeEpubFile), (void**)&piEpubFile );
					if(NULL != piEpubFile)
					{
						VARIANT vtMetadata;
						if(SUCCEEDED(piEpubFile->GetAdditionalParam(_T("Metadata"), &vtMetadata)) && VT_BSTR == vtMetadata.vt)
						{
							m_sMetadata = CString(vtMetadata.bstrVal);
							SysFreeString(vtMetadata.bstrVal);
						}
						RELEASEINTERFACE(piEpubFile);
					}
				}
				else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nOpenFileType)
				{
					OfficeFB2::IFB2File* piFb2File = NULL;
					piConverter->QueryInterface( __uuidof(OfficeFB2::IFB2File), (void**)&piFb2File );
					if(NULL != piFb2File)
					{
						VARIANT vtMetadata;
						if(SUCCEEDED(piFb2File->GetAdditionalParam(_T("Metadata"), &vtMetadata)) && VT_BSTR == vtMetadata.vt)
						{
							m_sMetadata = CString(vtMetadata.bstrVal);
							SysFreeString(vtMetadata.bstrVal);
						}
						RELEASEINTERFACE(piFb2File);
					}
				}
				else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI == nOpenFileType)
				{
					EBOOKWriter::IOfficeMobiFile* piMobiFile = NULL;
					piConverter->QueryInterface( __uuidof(EBOOKWriter::IOfficeMobiFile), (void**)&piMobiFile );
					if(NULL != piMobiFile)
					{
						VARIANT vtMetadata;
						if(SUCCEEDED(piMobiFile->GetAdditionalParam(_T("Metadata"), &vtMetadata)) && VT_BSTR == vtMetadata.vt)
						{
							m_sMetadata = CString(vtMetadata.bstrVal);
							SysFreeString(vtMetadata.bstrVal);
						}
						RELEASEINTERFACE(piMobiFile);
					}
				}
			}
		}
		//отцепляем события
		oEventDocuments->UnAdvise( piConverter );
		RELEASEINTERFACE(oEventDocuments);
		oEventDocuments2->UnAdvise( piConverter );
		RELEASEINTERFACE(oEventDocuments2);

		if( FAILED( hRes ) )
		{
			m_nStatus = CONVERTER_IDLE;
			RELEASEINTERFACE(piConverter);
			return hRes;
		}
		//для PPT и PPTX 
		if( AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT == nOpenFileType )
		{
			AVSOfficePPTFile::IAVSOfficePPTFile* piPptFile = NULL;
			piConverter->QueryInterface( __uuidof(AVSOfficePPTFile::IAVSOfficePPTFile), (void**)&piPptFile );
			if( NULL != piPptFile )
			{
				BSTR bstrDrawingXml;
				hRes = piPptFile->get_DrawingXml(&bstrDrawingXml);
				if( FAILED( hRes ) )
				{
					m_nStatus = CONVERTER_IDLE;
					RELEASEINTERFACE(piConverter);
					RELEASEINTERFACE(piPptFile);
					return hRes;
				}
				m_sDrawingXML = CString (bstrDrawingXml);
				VARIANT vDecoder;
				BSTR bstrAdditionalParamName = g_csBlowfishKeyParamName.AllocSysString();
				hRes = piPptFile->GetAdditionalParam( bstrAdditionalParamName, &vDecoder );
				SysFreeString( bstrAdditionalParamName );
				if( FAILED( hRes ) )
				{
					m_nStatus = CONVERTER_IDLE;
					RELEASEINTERFACE(piConverter);
					RELEASEINTERFACE(piPptFile);
					return hRes;
				}
				RELEASEINTERFACE( m_piDecoderKey );
				vDecoder.punkVal->QueryInterface( __uuidof( IUnknown ), (void**)&m_piDecoderKey );
				RELEASEINTERFACE( vDecoder.punkVal );
			}
			RELEASEINTERFACE(piPptFile);
		}
		else if( AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX == nOpenFileType )
		{
			AVSOfficePPTXFile::IAVSOfficePPTXFile* piPptxFile = NULL;	
			piConverter->QueryInterface( __uuidof(AVSOfficePPTXFile::IAVSOfficePPTXFile), (void**)&piPptxFile );
			if( NULL != piPptxFile )
			{
				BSTR bstrDrawingXml;
				hRes = piPptxFile->get_DrawingXml(&bstrDrawingXml);
				if( FAILED( hRes ) )
				{
					m_nStatus = CONVERTER_IDLE;
					RELEASEINTERFACE(piConverter);
					RELEASEINTERFACE(piPptxFile);
					return hRes;
				}
				m_sDrawingXML = CString (bstrDrawingXml);
				VARIANT vDecoder;
				BSTR bstrAdditionalParamName = g_csBlowfishKeyParamName.AllocSysString();
				hRes = piPptxFile->GetAdditionalParam( bstrAdditionalParamName, &vDecoder );
				if( FAILED( hRes ) )
				{
					m_nStatus = CONVERTER_IDLE;
					RELEASEINTERFACE(piConverter);
					RELEASEINTERFACE(piPptxFile);
					return hRes;
				}
				RELEASEINTERFACE( m_piDecoderKey );
				vDecoder.punkVal->QueryInterface( __uuidof( IUnknown ), (void**)&m_piDecoderKey );
				RELEASEINTERFACE( vDecoder.punkVal );
			}
			RELEASEINTERFACE(piPptxFile);
		}
		RELEASEINTERFACE(piConverter);
		if( FAILED( hRes ) )
		{
			m_nStatus = CONVERTER_IDLE;
			return hRes;
		}

		m_nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		m_nOpenFileType = nOpenFileType;
		m_nStatus = CONVERTER_LOAD_COMP;
		return S_OK;
	}
	STDMETHOD(SaveToFile)( BSTR bstrDstFileName,  BSTR bstrSrcPath,  BSTR bstrXMLOptions)
	{
		// инициализируем gdiplus
		if (!m_oGdiPlusInit.IsValid())
		{
			if (!m_oGdiPlusInit.Init())
				return AVS_ERROR_UNEXPECTED;
		}
		m_bCanceled = false;
		m_bSaveRendererStopped = false;
		m_nLastProgress = -1;
		m_nLastProgressEx = -1;
		DWORD dwFileAttr = ::GetFileAttributes( bstrDstFileName );
		//if( dwFileAttr != INVALID_FILE_ATTRIBUTES && 0 == (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY ) )
		//{ 
		//	//проверяем доступен ли файл для записи
		//	HANDLE hFile = CreateFile( bstrDstFileName, GENERIC_EXECUTE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		//	if( INVALID_HANDLE_VALUE == hFile )
		//		return AVS_ERROR_FILEACCESS;
		//	RELEASEHANDLE( hFile );
		//}
		//CTemporaryCS oCS(&m_csThread);
		CString sOptions(bstrXMLOptions);
		CString sSourceDirectory;
		if( "" != CString(bstrSrcPath) )
			sSourceDirectory = CString( bstrSrcPath );
		else
			sSourceDirectory = m_sTempDirectory;
		m_nStatus = CONVERTER_SAVE_PROG;
		int nDestFormat;
		bool bLoadInCommandRenderer = false;
		long nDocxRendererMode = -1;
		long nEpubRendererMode = -1;
		bool bNoStandartSave = false;
		int nHtmlRendererMode = -1;
		//проверяем bstrXMLOptions на наличие FileType
		bool bFileTypeInOptions = false;
		CString sWatermark = _T("");
		XmlUtils::CXmlReader oOptionsReader;
		if( TRUE == oOptionsReader.OpenFromXmlString( bstrXMLOptions ) )
			if( TRUE == oOptionsReader.ReadRootNode( _T("Options") ) )
			{
				CString sXml = oOptionsReader.ReadNodeXml();
				CString sFileType = oOptionsReader.ReadNodeValue( _T("FileType") );
				if( _T("") != sFileType )
				{
					nDestFormat = Strings::ToInteger( sFileType );
					bFileTypeInOptions = true;
				}
				CString sDocxRendererMode = oOptionsReader.ReadNodeValue( _T("DocxRendererMode") );
				if( _T("") != sDocxRendererMode )
					nDocxRendererMode = Strings::ToInteger( sDocxRendererMode );

				CString sEpubRendererMode = oOptionsReader.ReadNodeValue( _T("EpubRendererMode") );
				if( _T("") != sEpubRendererMode )
					nEpubRendererMode = Strings::ToInteger( sEpubRendererMode );

				CString sHtmlRendererMode = oOptionsReader.ReadNodeValue( _T("HtmlRendererMode") );
				if( _T("") != sHtmlRendererMode )
					nHtmlRendererMode = Strings::ToInteger( sHtmlRendererMode );

				XML::IXMLDOMNodePtr pXmlNode = NULL;
				oOptionsReader.GetNode( pXmlNode );
				XmlUtils::CXmlReader oSubReader;
				if( TRUE == oSubReader.OpenFromXmlNode( pXmlNode ) )
					if( TRUE == oSubReader.ReadNode( _T("Watermarks") ) )
					{
						XML::IXMLDOMNodePtr pXmlNode = NULL;
						oSubReader.GetNode(pXmlNode);
						if(NULL != pXmlNode)
						{
							IXMLDOMNodeListPtr piChilds = pXmlNode->GetchildNodes();
							long nLength = 0;
							if(NULL != piChilds && SUCCEEDED(piChilds->get_length(&nLength)) && nLength > 0)
								sWatermark = oSubReader.ReadNodeXml();
						}
					}

				XmlUtils::CXmlReader oSubReader2;
				if( TRUE == oSubReader2.OpenFromXmlNode( pXmlNode ) )
					if( TRUE == oSubReader2.ReadNode( _T("HtmlOptions") ) )
						if(_T("0") == oSubReader2.ReadNodeValue(_T("ToolbarEmbed")))
						{
							nDestFormat = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRMenu;
							bFileTypeInOptions = true;
						}

				if( -1 != sXml.Find( _T("LoadInCommandRenderer") ) )
					bLoadInCommandRenderer = true;

				if( -1 != sXml.Find( _T("NoStandartSave") ) )
					bNoStandartSave = true;
			}
		AVSOfficeFormatChecker avsOfficeFormatChecker;
		if( false == bFileTypeInOptions )
		{
			if( AVS_OFFICESTUDIO_FILE_UNKNOWN == m_nFileType )
				nDestFormat = avsOfficeFormatChecker.GetFileFormat( CString( bstrDstFileName ) );
			else
				nDestFormat = m_nFileType;
		}
		//для Epub ставим определенный режим DocxRenderer
		if( AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nDestFormat )
		{
			if( -1 != nEpubRendererMode )
				nDocxRendererMode = nEpubRendererMode;
			else
				nDocxRendererMode = TextFormatting::c_nWordProcessing;
		}

		//оборачиваем опции
		CString sFileConverterOptions;
		sFileConverterOptions.AppendFormat( _T("<Options><FileType>%d</FileType>"), nDestFormat);
		XmlUtils::CXmlReader oXmlReader;
		if( TRUE == oXmlReader.OpenFromXmlString( sOptions ) )
		{
			oXmlReader.ReadNodeList( _T("*") );
			for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
			{
				CString sNodeName = oXmlReader.ReadNodeName( i );
				if( _T("WaterMarks") != sNodeName )
					sFileConverterOptions.Append( oXmlReader.ReadNodeXml( i ) );
			}
		}
		sFileConverterOptions.Append( _T("</Options>") );

		if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM & nDestFormat || AVS_OFFICESTUDIO_FILE_IMAGE & nDestFormat ||
			AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nDestFormat || AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nDestFormat ||
			AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI == nDestFormat || ( ( 0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nDestFormat) ) && true == bLoadInCommandRenderer ) )
		{
			long nPageCount = 0;
			m_piCommandsRenderer->get_PageCount( &nPageCount );
			AVSGraphics::IASCDocumentRendererPtr piCommandsRenderer = NULL;
			bool bRenderDoc = false;
			if( nPageCount > 0 )
				m_piCommandsRenderer->QueryInterface( __uuidof(AVSGraphics::IASCDocumentRenderer), (void**)&piCommandsRenderer);
			else
			{
				// TODO: DoctRenderer
			}

			//берем номера нужных страниц из Options
			CAtlArray<int> aDocumentPages;
			bool bDocumentPages = false;
			//выбираем нужные страницы 
			oXmlReader.OpenFromXmlString( sOptions );
			if( TRUE == oXmlReader.ReadNode( _T("DocumentPages") ) )
			{
				oXmlReader.ReadNodeList( _T("Page") );
				for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
				{
					CString sValue = oXmlReader.ReadNodeText(i);
					if( _T("") != sValue )
						aDocumentPages.Add( Strings::ToInteger(sValue) );
				}
				if( aDocumentPages.GetCount() > 0 )
				{
					//сортируем по возрастанию
					qsort (aDocumentPages.GetData(), aDocumentPages.GetCount(), sizeof(int), compare);
					bDocumentPages = true;
				}
			}
			piCommandsRenderer->get_PageCount( &nPageCount );
			//для HTMLR создается пустой документ
			if( 0 ==  nPageCount && AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR != nDestFormat && AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRMenu != nDestFormat && AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRCanvas != nDestFormat)
				return S_OK;

			if( aDocumentPages.GetCount() == 0 )
				for( int i = 0; i < nPageCount; i++ )
					aDocumentPages.Add( i );

			//выбираем нужный конвертер
			IAVSOfficeFileTemplate* piConverter = NULL;
			EBOOKWriter::IAVSEBOOKWriterPtr piEbookConverter = NULL;
			HTMLRenderer::IASCHTMLRendererPtr piHtmlRenderer = NULL;
			HTMLRenderer::IASCHTMLRenderer2Ptr piHtmlRenderer2 = NULL;
			AVSOfficePDFWriter::IPDFRendererPtr piPDFRenderer = NULL;
			if(c_nSaveModeNone == m_nSaveMode || 0 != (c_nSaveModeStart & m_nSaveMode))
			{
				if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR == nDestFormat || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRMenu == nDestFormat)
				{
					if(FAILED(piHtmlRenderer.CreateInstance( __uuidof(HTMLRenderer::CASCHTMLRenderer))) || NULL == piHtmlRenderer)
						return AVS_ERROR_FILEFORMAT;
					if(c_nSaveModeNone != m_nSaveMode)
						m_piSaveRenderer = piHtmlRenderer;
				}
				else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRCanvas == nDestFormat)
				{
					if(FAILED(piHtmlRenderer2.CreateInstance( __uuidof(HTMLRenderer::CASCHTMLRenderer3))) || NULL == piHtmlRenderer2)
						return AVS_ERROR_FILEFORMAT;
					if(c_nSaveModeNone != m_nSaveMode)
						m_piSaveRenderer = piHtmlRenderer2;
				}
				else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nDestFormat || AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nDestFormat || AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI == nDestFormat)
				{
					if(FAILED(piEbookConverter.CreateInstance(__uuidof(EBOOKWriter::CAVSEBOOKWriter))) || NULL == piEbookConverter )
						return AVS_ERROR_FILEFORMAT;
					if(c_nSaveModeNone != m_nSaveMode)
						m_piSaveRenderer = piEbookConverter;
				}
				else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nDestFormat)
				{
					if(FAILED(piPDFRenderer.CreateInstance(__uuidof(AVSOfficePDFWriter::CPDFWriter))) || NULL == piPDFRenderer )
						return AVS_ERROR_FILEFORMAT;
					if(c_nSaveModeNone != m_nSaveMode)
						m_piSaveRenderer = piPDFRenderer;
				}
				else
				{
					if( FALSE == m_oStorageConverters.GetConverter( nDestFormat , &piConverter))
						return AVS_ERROR_FILEFORMAT;
					if(c_nSaveModeNone != m_nSaveMode)
						m_piSaveRenderer = piConverter;
				}
			}
			else
			{
				if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR == nDestFormat || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRMenu == nDestFormat)
				{
					if(NULL != m_piSaveRenderer)
						m_piSaveRenderer->QueryInterface(__uuidof(HTMLRenderer::IASCHTMLRenderer), (void**)&piHtmlRenderer);
					if(NULL == piHtmlRenderer)
						return AVS_ERROR_FILEFORMAT;
				}
				else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRCanvas == nDestFormat)
				{
					if(NULL != m_piSaveRenderer)
						m_piSaveRenderer->QueryInterface(__uuidof(HTMLRenderer::IASCHTMLRenderer2), (void**)&piHtmlRenderer2);
					if(NULL == piHtmlRenderer2)
						return AVS_ERROR_FILEFORMAT;
				}
				else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nDestFormat || AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nDestFormat || AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI == nDestFormat)
				{
					if(NULL != m_piSaveRenderer)
						m_piSaveRenderer->QueryInterface(__uuidof(EBOOKWriter::IAVSEBOOKWriter), (void**)&piEbookConverter);
					if(NULL == piEbookConverter)
						return AVS_ERROR_FILEFORMAT;
				}
				else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nDestFormat)
				{
					if(NULL != m_piSaveRenderer)
						m_piSaveRenderer->QueryInterface(__uuidof(AVSOfficePDFWriter::IPDFRenderer), (void**)&piPDFRenderer);
					if(NULL == piPDFRenderer)
						return AVS_ERROR_FILEFORMAT;
				}
				else
				{
					if(NULL != m_piSaveRenderer)
						m_piSaveRenderer->QueryInterface(__uuidof(IAVSOfficeFileTemplate), (void**)&piConverter);
					if(NULL == piConverter)
						return AVS_ERROR_FILEFORMAT;
				}
			}

			//ставим CommandRenderer
			if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nDestFormat )
			{
				sFileConverterOptions = CheckPdfOutline( sFileConverterOptions, aDocumentPages );
				//выставляем dpi
				VARIANT vtDPI;
				vtDPI.vt = VT_R8;
				vtDPI.dblVal = 72;
				piCommandsRenderer->SetAdditionalParam( _T("DPIX"), vtDPI );
				vtDPI.dblVal = 72;
				piCommandsRenderer->SetAdditionalParam( _T("DPIY"), vtDPI );
			}
			else if( AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nDestFormat || AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nDestFormat || AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI == nDestFormat )
			{
				if( NULL != piEbookConverter )
				{
					piEbookConverter->put_CommandRenderer( piCommandsRenderer );
				}
				//выставляем dpi
				VARIANT vtDPI;
				vtDPI.vt = VT_R8;
				vtDPI.dblVal = 96;
				piCommandsRenderer->SetAdditionalParam( _T("DPIX"), vtDPI );
				vtDPI.dblVal = 96;
				piCommandsRenderer->SetAdditionalParam( _T("DPIY"), vtDPI );
			}
			else if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR == nDestFormat || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRMenu == nDestFormat )
			{
				//выставляем dpi
				VARIANT vtDPI;
				vtDPI.vt = VT_R8;
				vtDPI.dblVal = 96;
				piCommandsRenderer->SetAdditionalParam( _T("DPIX"), vtDPI );
				vtDPI.dblVal = 96;
				piCommandsRenderer->SetAdditionalParam( _T("DPIY"), vtDPI );
			}
			else if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRCanvas == nDestFormat )
			{
				//выставляем dpi
				VARIANT vtDPI;
				vtDPI.vt = VT_R8;
				vtDPI.dblVal = 96;
				piCommandsRenderer->SetAdditionalParam( _T("DPIX"), vtDPI );
				vtDPI.dblVal = 96;
				piCommandsRenderer->SetAdditionalParam( _T("DPIY"), vtDPI );
			}

			if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR == nDestFormat || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRMenu == nDestFormat )
			{
				HRESULT hRes = S_OK;
				if( NULL != piHtmlRenderer )
				{
					if(c_nSaveModeNone == m_nSaveMode || 0 != (c_nSaveModeStart & m_nSaveMode))
					{
					if(  AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == m_nOpenFileType )
					{
						VARIANT vSourceType;
						vSourceType.vt = VT_I4;
						vSourceType.lVal = 1;//c_nPDFWriter
						piHtmlRenderer->SetAdditionalParam( _T("SourceRendererType"), vSourceType);
					}
					if( -1 != nHtmlRendererMode )
						piHtmlRenderer->put_Mode(nHtmlRendererMode);
					else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRMenu == nDestFormat)
						piHtmlRenderer->put_Mode(12);//С меню

					piHtmlRenderer->CreateOfficeFile( bstrDstFileName );
					}
					long m_nPageCount = 0;
					if(bDocumentPages)
						m_nPageCount = aDocumentPages.GetCount();
					else
						piCommandsRenderer->get_PageCount( &m_nPageCount );
					for( int i = 0; i < m_nPageCount; i++ )
					{
						if( bDocumentPages )
						{
							//смотрим нужно ли сохранять эту страницу
							bool bSaveThisPage = false;
							for( int j = 0; j < aDocumentPages.GetCount(); j++ )
							{
								if( i <= aDocumentPages[j] )
								{
									if( i == aDocumentPages[j] )
										bSaveThisPage = true;
									break;//дальше можно не смотреть, aDocumentPages отсортирован по-возрастанию
								}
							}
							if( false == bSaveThisPage )
								continue;
						}
						if( true == m_bCanceled )
							break;
						piHtmlRenderer->NewPage();
						piHtmlRenderer->BeginCommand(1);
						hRes = piCommandsRenderer->DrawPage( i, piHtmlRenderer );
						piHtmlRenderer->EndCommand(1);
						SHORT shCancel = c_shProgressContinue;
						OnProgressDocumentEx( 0, long(c_nMaxProgressPercent * ( 1.0 * ( i + 1 ) / m_nPageCount )), &shCancel );
						if( FAILED( hRes ) )
							m_bCanceled = true;
					}
					if(m_bCanceled || c_nSaveModeNone == m_nSaveMode || 0 != (c_nSaveModeCommit & m_nSaveMode))
					{
						m_bSaveRendererStopped = true;
						piHtmlRenderer->CloseFile();
					}
				}
			}
			else if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRCanvas == nDestFormat )
			{
				HRESULT hRes = S_OK;
				if( NULL != piHtmlRenderer2 )
				{
					if(c_nSaveModeNone == m_nSaveMode || 0 != (c_nSaveModeStart & m_nSaveMode))
					{
						VARIANT vSourceType;
						if(  AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == m_nOpenFileType )
						{
							vSourceType.vt = VT_I4;
							vSourceType.lVal = 1;//c_nPDFWriter
							piHtmlRenderer2->SetAdditionalParam( _T("SourceRendererType"), vSourceType);
						}
						vSourceType.vt = VT_I4;
						vSourceType.lVal = m_nOpenFileType;
						piHtmlRenderer2->SetAdditionalParam( _T("SourceType"), vSourceType);
						if( -1 != nHtmlRendererMode )
							piHtmlRenderer2->put_Mode(nHtmlRendererMode);

						piHtmlRenderer2->CreateOfficeFile( bstrDstFileName );
					}
					long m_nPageCount = 0;
					if(bDocumentPages)
						m_nPageCount = aDocumentPages.GetCount();
					else
						piCommandsRenderer->get_PageCount( &m_nPageCount );
					for( int i = 0; i < m_nPageCount; i++ )
					{
						if( bDocumentPages )
						{
							//смотрим нужно ли сохранять эту страницу
							bool bSaveThisPage = false;
							for( int j = 0; j < aDocumentPages.GetCount(); j++ )
							{
								if( i <= aDocumentPages[j] )
								{
									if( i == aDocumentPages[j] )
										bSaveThisPage = true;
									break;//дальше можно не смотреть, aDocumentPages отсортирован по-возрастанию
								}
							}
							if( false == bSaveThisPage )
								continue;
						}
						if( true == m_bCanceled )
							break;
						piHtmlRenderer2->NewPage();
						piHtmlRenderer2->BeginCommand(1);
						hRes = piCommandsRenderer->DrawPage( i, piHtmlRenderer2 );
						piHtmlRenderer2->EndCommand(1);
						SHORT shCancel = c_shProgressContinue;
						OnProgressDocumentEx( 0, long(c_nMaxProgressPercent * ( 1.0 * ( i + 1 ) / m_nPageCount )), &shCancel );
						if( FAILED( hRes ) )
							m_bCanceled = true;
					}
					if(m_bCanceled || c_nSaveModeNone == m_nSaveMode || 0 != (c_nSaveModeCommit & m_nSaveMode))
					{
						m_bSaveRendererStopped = true;
						piHtmlRenderer2->CloseFile();
					}
				}
			}
			else if(AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nDestFormat || AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 == nDestFormat || AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI == nDestFormat)
			{
				HRESULT hRes = S_OK;
				if( NULL != piEbookConverter )
				{
					if(c_nSaveModeNone == m_nSaveMode || 0 != (c_nSaveModeStart & m_nSaveMode))
					{
					//Определяет нужно ли работать со встроенными шрифтами
					if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == m_nOpenFileType || AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == m_nOpenFileType)
						piEbookConverter->put_SrcFileType(1);
					else
						piEbookConverter->put_SrcFileType(0);

					//Опции конвертации
					BSTR bstrOptions = sFileConverterOptions.AllocSysString();
					piEbookConverter->Initialize(bstrOptions);
					SysFreeString(bstrOptions);

					//Метаданные
					BSTR bstrMetadata = m_sMetadata.AllocSysString();
					piEbookConverter->SetMetadata(bstrMetadata);
					SysFreeString(bstrMetadata);

					//Output format
					long nEBookFiletype = 0;
					switch(nDestFormat)
					{
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB:nEBookFiletype=0;break;
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2:nEBookFiletype=1;break;
						case AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI:nEBookFiletype=2;break;
					}

					piEbookConverter->CreateOfficeFile (bstrDstFileName, nEBookFiletype);
					}
					//Рисуем страницы CommandsRenderer'ом
					long m_nPageCount = 0;
					if(bDocumentPages)
						m_nPageCount = aDocumentPages.GetCount();
					else
						piCommandsRenderer->get_PageCount( &m_nPageCount );
					for( int i = 0; i < m_nPageCount; i++ )
					{
						if( bDocumentPages )
						{
							//смотрим нужно ли сохранять эту страницу
							bool bSaveThisPage = false;
							for( int j = 0; j < aDocumentPages.GetCount(); j++ )
							{
								if( i <= aDocumentPages[j] )
								{
									if( i == aDocumentPages[j] )
										bSaveThisPage = true;
									break;//дальше можно не смотреть, aDocumentPages отсортирован по-возрастанию
								}
							}
							if( false == bSaveThisPage )
								continue;
						}
						if( true == m_bCanceled )
							break;
						piEbookConverter->NewPage();
						piEbookConverter->BeginCommand(1);
						//В случае презентаций сохраняем картинки,а их записываем в ebook
						if(0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & m_nOpenFileType))
						{
							double dPageWidthMm = 0;
							double dPageHeightMm = 0;
							piCommandsRenderer->GetPageSize( i, &dPageWidthMm, &dPageHeightMm);
							//Создаем новую картинку для рисования
							MediaCore::IAVSUncompressedVideoFrame* piImage = NULL;
							AVSGraphics::IASCGraphicsRenderer* piGdiplusRenderer = NULL;
							UpdateGdiPlusRenderer(dPageWidthMm, dPageHeightMm, &piImage, &piGdiplusRenderer);

							//рисуем страницу
							hRes = m_piCommandsRenderer->DrawPage(i, piGdiplusRenderer);

							RELEASEINTERFACE(piGdiplusRenderer);
							hRes = piEbookConverter->DrawImage(piImage, 0, 0, dPageWidthMm, dPageHeightMm);
							RELEASEINTERFACE(piImage);
						}
						else
							hRes = piCommandsRenderer->DrawPage( i, piEbookConverter );
						piEbookConverter->EndCommand(1);
						SHORT shCancel = c_shProgressContinue;
						OnProgressDocumentEx( 0, long(c_nMaxProgressPercent * ( 1.0 * ( i + 1 ) / m_nPageCount )), &shCancel );
						if( FAILED( hRes ) )
							m_bCanceled = true;
					}
					if(m_bCanceled || c_nSaveModeNone == m_nSaveMode || 0 != (c_nSaveModeCommit & m_nSaveMode))
					{
						m_bSaveRendererStopped = true;
						piEbookConverter->CloseFile();
					}
				}
			}
			else if(AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nDestFormat)
			{
				HRESULT hRes = S_OK;
				AVSOfficePDFWriter::IPDFWriter * piPDFWriter = NULL;
				piPDFRenderer->QueryInterface( __uuidof( AVSOfficePDFWriter::IPDFWriter ), (void**)&piPDFWriter );
				if( NULL !=  piPDFWriter && NULL != piPDFRenderer )
				{
					if(c_nSaveModeNone == m_nSaveMode || 0 != (c_nSaveModeStart & m_nSaveMode))
						piPDFWriter->CreatePDF();

					long m_nPageCount = 0;
					if(bDocumentPages)
						m_nPageCount = aDocumentPages.GetCount();
					else
						piCommandsRenderer->get_PageCount( &m_nPageCount );
					for( int i = 0; i < m_nPageCount; i++ )
					{
						if( bDocumentPages )
						{
							//смотрим нужно ли сохранять эту страницу
							bool bSaveThisPage = false;
							for( int j = 0; j < aDocumentPages.GetCount(); j++ )
							{
								if( i <= aDocumentPages[j] )
								{
									if( i == aDocumentPages[j] )
										bSaveThisPage = true;
									break;//дальше можно не смотреть, aDocumentPages отсортирован по-возрастанию
								}
							}
							if( false == bSaveThisPage )
								continue;
						}
						if( true == m_bCanceled )
							break;
						piPDFRenderer->NewPage();
						piPDFRenderer->BeginCommand(1);
						//рисуем страницу
						hRes = m_piCommandsRenderer->DrawPage(i, piPDFRenderer);
						piPDFRenderer->EndCommand(1);
						SHORT shCancel = c_shProgressContinue;
						OnProgressDocumentEx( 0, long(c_nMaxProgressPercent * ( 1.0 * ( i + 1 ) / m_nPageCount )), &shCancel );
						if( FAILED( hRes ) )
							m_bCanceled = true;
					}
					if(m_bCanceled || c_nSaveModeNone == m_nSaveMode || 0 != (c_nSaveModeCommit & m_nSaveMode))
					{
						m_bSaveRendererStopped = true;
						BSTR bstrFileConverterOptions = sFileConverterOptions.AllocSysString();
						PdfReadWriterOptions( bstrFileConverterOptions, piPDFWriter );
						SysFreeString(bstrFileConverterOptions);
						piPDFWriter->SaveToFile( bstrDstFileName );
						piPDFWriter->DeletePDF();
					}
				}
			}
			else if( 0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & nDestFormat) && true == bLoadInCommandRenderer  )
			{
				//выставляем dpi
				VARIANT vtDPI;
				vtDPI.vt = VT_R8;
				vtDPI.dblVal = 96;
				piCommandsRenderer->SetAdditionalParam( _T("DPIX"), vtDPI );
				vtDPI.dblVal = 96;
				piCommandsRenderer->SetAdditionalParam( _T("DPIY"), vtDPI );

				if( AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == nDestFormat )
				{
					if( false == bRenderDoc )
					{
						m_dMinProgress = 0;
						m_dMaxProgress = 1;
					}
					else
					{
						m_dMinProgress = 0.500;
						m_dMaxProgress = 1;
					}
					HRESULT hRes = S_OK;
					//цепляем события
					COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >* oEventDocuments = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >(this);
					oEventDocuments->AddRef();
					oEventDocuments->Advise( piConverter );
					COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >* oEventDocuments2 = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >(this);
					oEventDocuments2->AddRef();
					oEventDocuments2->Advise( piConverter );

					//конвертируем docx в конечный формат
					BSTR bstrPath = m_sTempDirectory.AllocSysString();
					BSTR bstrOptions = sFileConverterOptions.AllocSysString();
					hRes = piConverter->SaveToFile( bstrDstFileName, bstrPath, bstrOptions );
					SysFreeString(bstrOptions);
					SysFreeString(bstrPath);

					//отцепляем события
					oEventDocuments->UnAdvise( piConverter );
					RELEASEINTERFACE(oEventDocuments);
					oEventDocuments2->UnAdvise( piConverter );
					RELEASEINTERFACE(oEventDocuments2);

					if( FAILED( hRes ) )
						return hRes;
				}
				else
				{
				DocxRenderer::IAVSDocxRendererPtr piDocxRenderer;
				piDocxRenderer.CreateInstance( __uuidof( DocxRenderer::CAVSDocxRenderer ) );
				if( NULL != piDocxRenderer )
				{
					if(  AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS == m_nOpenFileType )
					{
						VARIANT vSourceType;
						vSourceType.vt = VT_I4;
						vSourceType.lVal = 1;//c_nPDFWriter
						piDocxRenderer->SetAdditionalParam( _T("SourceRendererType"), vSourceType);
					}
					CString sNewTempFolder = Utils::CreateTempDir( m_sTempDirectory );
					if( _T("") != sNewTempFolder )
					{
						CString sDocxRendererOptions = _T("<DocxRenderer");
						sDocxRendererOptions.AppendFormat( _T(" destinationpath=\"%s\""), sNewTempFolder );
						if( -1 != nDocxRendererMode )
							sDocxRendererOptions.AppendFormat( _T(" textformatting=\"%d\""), nDocxRendererMode );
						sDocxRendererOptions.Append( _T("/>") );
						//выставляем опции
						BSTR bstrDocxRendererOptions = sDocxRendererOptions.AllocSysString();
						piDocxRenderer->Initialize( bstrDocxRendererOptions );
						SysFreeString( bstrDocxRendererOptions );

						if( false == bRenderDoc )
						{
							m_dMinProgress = 0;
							m_dMaxProgress = 0.500;
						}
						else
						{
							m_dMinProgress = 0.500;
							m_dMaxProgress = 0.750;
						}

						HRESULT hRes = S_OK;
						long m_nPageCount = 0;
						if(bDocumentPages)
							m_nPageCount = aDocumentPages.GetCount();
						else
							piCommandsRenderer->get_PageCount( &m_nPageCount );
						for( int i = 0; i < m_nPageCount; i++ )
						{
							if( bDocumentPages )
							{
								//смотрим нужно ли сохранять эту страницу
								bool bSaveThisPage = false;
								for( int j = 0; j < aDocumentPages.GetCount(); j++ )
								{
									if( i <= aDocumentPages[j] )
									{
										if( i == aDocumentPages[j] )
											bSaveThisPage = true;
										break;//дальше можно не смотреть, aDocumentPages отсортирован по-возрастанию
									}
								}
								if( false == bSaveThisPage )
									continue;
							}
							if( true == m_bCanceled )
								break;
							piDocxRenderer->NewPage();
							piDocxRenderer->BeginCommand(1);
							hRes = piCommandsRenderer->DrawPage( i, piDocxRenderer );
							piDocxRenderer->EndCommand(1);
							SHORT shCancel = c_shProgressContinue;
							OnProgressDocumentEx( 0, long(c_nMaxProgressPercent * ( 1.0 * ( i + 1 ) / m_nPageCount )), &shCancel );
							if( FAILED( hRes ) )
								m_bCanceled = true;
						}
						if( false == m_bCanceled )
						{
							hRes = piDocxRenderer->Save();
							if( FAILED( hRes ) )
								m_bCanceled = true;

							if( false == bRenderDoc )
							{
								m_dMinProgress = 0.500;
								m_dMaxProgress = 1;
							}
							else
							{
								m_dMinProgress = 0.750;
								m_dMaxProgress = 1;
							}

							if( false == m_bCanceled )
							{
								//цепляем события
								COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >* oEventDocuments = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >(this);
								oEventDocuments->AddRef();
								oEventDocuments->Advise( piConverter );
								COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >* oEventDocuments2 = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >(this);
								oEventDocuments2->AddRef();
								oEventDocuments2->Advise( piConverter );
								//m_poEventDocuments->Advise( piConverter );

								//конвертируем docx в конечный формат
								BSTR bstrPath = sNewTempFolder.AllocSysString();
								BSTR bstrOptions = sFileConverterOptions.AllocSysString();
								hRes = piConverter->SaveToFile( bstrDstFileName, bstrPath, bstrOptions );
								SysFreeString(bstrOptions);
								SysFreeString(bstrPath);
								//отцепляем события
								//m_poEventDocuments->UnAdvise( piConverter );
								oEventDocuments->UnAdvise( piConverter );
								RELEASEINTERFACE(oEventDocuments);
								oEventDocuments2->UnAdvise( piConverter );
								RELEASEINTERFACE(oEventDocuments2);

								RELEASEINTERFACE(piConverter);
							}
						}
						Utils::RemoveDirOrFile( sNewTempFolder );
						if( FAILED( hRes ) )
							return hRes;
					}
				}
				}
			}
			else
			{
				m_dMinProgress = 0;
				m_dMaxProgress = 1;
				//цепляем события
				OnProgressDocument(0, 0);
				COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >* poEventViewer = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >(this);
				poEventViewer->AddRef();
				poEventViewer->Advise( piConverter );
				COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >* oEventDocuments2 = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >(this);
				oEventDocuments2->AddRef();
				oEventDocuments2->Advise( piConverter );
				//сохраняем
				BSTR bstrPath = sSourceDirectory.AllocSysString();
				BSTR bstrOptions = sFileConverterOptions.AllocSysString();
				HRESULT hRes = piConverter->SaveToFile( bstrDstFileName, bstrPath, bstrOptions );
				SysFreeString( bstrOptions );
				SysFreeString( bstrPath );

				//проверяем завершен ли документ
				if(c_nSaveModeNone != m_nSaveMode)
				{
					/*
					AVSOfficeImageFile::IOfficeImageFilePtr piImageFile = NULL;
					piConverter->QueryInterface( __uuidof(AVSOfficeImageFile::IOfficeImageFile), (void**)&piImageFile );
					if( NULL != piImageFile )
					{
						VARIANT vtParam;
						piImageFile->GetAdditionalParam(_T("SaveRendererStopped"), &vtParam);
						if( VT_I4 == vtParam.vt)
						{
							if(0 != vtParam.lVal)
								m_bSaveRendererStopped = true;
						}
					}
					*/
				}

				//отцепляем события
				poEventViewer->UnAdvise( piConverter );
				RELEASEINTERFACE(poEventViewer);
				oEventDocuments2->UnAdvise( piConverter );
				RELEASEINTERFACE(oEventDocuments2);
				RELEASEINTERFACE( piConverter );

				if( FAILED( hRes ) )
					return hRes;
			}
		}
		else
		{
			m_dMinProgress = 0;
			m_dMaxProgress = 1;
			//if( false == AVSOfficeFormatChecker::CanConvert( m_nOpenFileType, nDestFormat ) )
			//	return AVS_OFFICEFILE_ERROR_UNSUPPORTED_LOAD_FORMAT;
			if( false == bNoStandartSave )
			{
				//если файл не был загружен
				if( CString(bstrSrcPath) == "" && "" == sSourceDirectory )
					return AVS_OFFICEFILE_ERROR_SAVE_EMPTY;
				//если это пустая папка
				if( ::GetFileAttributes( sSourceDirectory ) == INVALID_FILE_ATTRIBUTES )
					return AVS_OFFICEFILE_ERROR_SAVE_EMPTY;
			}
			//пытаемся конвертировать
			IAVSOfficeFileTemplate* piConverter = NULL;
			if( FALSE == m_oStorageConverters.GetConverter( nDestFormat , &piConverter) )
				return AVS_ERROR_FILEFORMAT;
			//цепляем события
			COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >* oEventDocuments = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents, CAVSOfficeFileConverter >(this);
			oEventDocuments->AddRef();
			oEventDocuments->Advise( piConverter );
			COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >* oEventDocuments2 = new COfficeEventDocuments< _IAVSOfficeFileTemplateEvents2, CAVSOfficeFileConverter >(this);
			oEventDocuments2->AddRef();
			oEventDocuments2->Advise( piConverter );
			//m_poEventDocuments->Advise( piConverter );

			// конвертируем в docx структуру
			HRESULT hRes = S_OK;
			BSTR bstrPath = sSourceDirectory.AllocSysString();
			BSTR bstrOptions = sFileConverterOptions.AllocSysString();
			hRes = piConverter->SaveToFile( bstrDstFileName, bstrPath, bstrOptions );
			SysFreeString(bstrOptions);
			SysFreeString(bstrPath);
			//отцепляем события
			//m_poEventDocuments->UnAdvise( piConverter );
			oEventDocuments->UnAdvise( piConverter );
			RELEASEINTERFACE(oEventDocuments);
			oEventDocuments2->UnAdvise( piConverter );
			RELEASEINTERFACE(oEventDocuments2);

			RELEASEINTERFACE(piConverter);
			if( FAILED( hRes ) )
				return hRes;
		}
		m_nStatus = CONVERTER_SAVE_COMP;
		return S_OK;
	}

	STDMETHOD(get_FileType)(LONG* pnType)
	{
		//CTemporaryCS oCS(&m_csThread);
		if( NULL == pnType )
			return S_FALSE;
		if( AVS_OFFICESTUDIO_FILE_UNKNOWN != m_nFileType )
			(*pnType) = m_nFileType;
		else
			(*pnType) = m_nOpenFileType;
		return S_OK;
	}
	STDMETHOD(put_FileType)(LONG nType)
	{
		//CTemporaryCS oCS(&m_csThread);
		m_nFileType = nType;
		return S_OK;
	}
	STDMETHOD(get_DrawingXml)( BSTR* pbstrXml )
	{		
		return S_OK;
	}

	STDMETHOD(get_CommandRenderer)( IUnknown** ppunkRend )
	{
		//CTemporaryCS oCS(&m_csThread);
		if( NULL == ppunkRend )
			return S_FALSE;
		(*ppunkRend) = NULL;
		if( NULL == m_piCommandsRenderer )
			return S_OK;
		HRESULT hRes = m_piCommandsRenderer->QueryInterface( __uuidof( AVSGraphics::IASCDocumentRenderer ), (void**)ppunkRend );
		return hRes;
	}
	STDMETHOD(put_CommandRenderer)( IUnknown* ppunkRend )
	{
		//CTemporaryCS oCS(&m_csThread);
		RELEASEINTERFACE(m_piCommandsRenderer);
		if( NULL != ppunkRend )
			ppunkRend->QueryInterface(__uuidof( AVSGraphics::IASCDocumentRenderer ), (void**)&m_piCommandsRenderer );
		else
			CoCreateInstance( __uuidof(AVSGraphics::CASCDocumentRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IASCDocumentRenderer), (void **)(&m_piCommandsRenderer)  );
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
		return S_OK;
	}
	STDMETHOD(DrawWatermark)( IUnknown* punkImage, LONG nWidth, LONG nHeight)
	{
		return S_OK;
	}
	STDMETHOD(get_TempDirectory)( BSTR* pbstrDir )
	{
		//CTemporaryCS oCS(&m_csThread);
		if( NULL == pbstrDir )
			return S_FALSE;
		(*pbstrDir) = m_sTempDirectory.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_TempDirectory)( BSTR bstrDir )
	{
		//CTemporaryCS oCS(&m_csThread);
		CString sNewTempDir( bstrDir );
		if( m_sRootTempDirectory != sNewTempDir && true == m_bIsInternalTemDir )
			Clear();
		m_sRootTempDirectory = sNewTempDir;
		return S_OK;
	}
	STDMETHOD(get_IsLoadComplete)( VARIANT_BOOL* pvIsLoadComplete )
	{
		if( NULL == pvIsLoadComplete )
			return S_FALSE;
		if( m_nStatus < CONVERTER_LOAD_COMP )
			(*pvIsLoadComplete) = VARIANT_FALSE;
		else
			(*pvIsLoadComplete) = VARIANT_TRUE;
		return S_OK;
	}
	STDMETHOD(get_Status)( LONG* pVal)
	{
		if( NULL == pVal )
			return S_FALSE;
		(*pVal) = m_nStatus;
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CComBSTR ParamStr = ParamName;
		ParamStr.ToLower();
		if (ParamStr == L"savemode")
		{
			if( VT_I4 == ParamValue.vt)
				m_nSaveMode = ParamValue.lVal;
		}
		else if (ParamStr == L"saverenderer")
		{
			if( VT_UNKNOWN == ParamValue.vt && NULL != ParamValue.punkVal)
				ParamValue.punkVal->QueryInterface( __uuidof(IUnknown) , (void**)(&m_piSaveRenderer) );
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CComBSTR ParamStr = ParamName;
		ParamStr.ToLower();
		if (ParamStr == L"decoderkey")
		{
			(*ParamValue).vt = VT_UNKNOWN;
			(*ParamValue).punkVal = NULL;
			if( NULL != m_piDecoderKey )
				m_piDecoderKey->QueryInterface( __uuidof(IUnknown) , (void**)(&(*ParamValue).punkVal) );
		}
		else if (ParamStr == L"saverenderer")
		{
			(*ParamValue).vt = VT_UNKNOWN;
			(*ParamValue).punkVal = NULL;
			if( NULL != m_piSaveRenderer )
				m_piSaveRenderer->QueryInterface( __uuidof(IUnknown) , (void**)(&(*ParamValue).punkVal) );
		}
		else if (ParamStr == L"saverendererstopped")
		{
			(*ParamValue).vt = VT_I4;
			(*ParamValue).lVal = m_bSaveRendererStopped;
		}
		return S_OK;
	}
	private: void Clear()
	{
		m_nStatus = CONVERTER_IDLE;
		m_nRendererError = 0;
		m_nOpenFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
		m_sDrawingXML = _T("");
		m_sMetadata = _T("");
		RELEASEINTERFACE( m_piDecoderKey );
		if( true == m_bIsInternalTemDir )
			Utils::RemoveDirOrFile( m_sTempDirectory );
		m_sTempDirectory = _T("");
		m_bIsInternalTemDir = false;
		m_bCanceled = false;
		m_nLastProgress = -1;
		m_nLastProgressEx = -1;
		m_nMaxPageLoad = -1;
		m_nSaveMode = c_nSaveModeNone;
		m_piSaveRenderer = NULL;
		m_bSaveRendererStopped = false;
	}
	private: CString CheckPdfOutline( CString Options, CAtlArray<int>& aPages )
	{
		CAtlArray<int> aSortedPages;
		aSortedPages.Append( aPages );
		//сортируем методом пузырька))
		for( int i = 0 ; i < aSortedPages.GetCount(); i++ )
			for( int j = i ; j < aSortedPages.GetCount(); j++ )
				if( aSortedPages[i] > aSortedPages[j] )
				{
					int nTemp = aSortedPages[i];
					aSortedPages[i] = aSortedPages[j];
					aSortedPages[j] = nTemp;
				}

		CString sResult = _T("<Options>");
		XmlUtils::CXmlReader oXmlReader;
		if( TRUE ==  oXmlReader.OpenFromXmlString( Options ) )
		{
			oXmlReader.ReadNodeList( _T("*") );
			for( int i = 0; i < oXmlReader.GetLengthList(); i++)
			{
				CString sNodeName = oXmlReader.ReadNodeName(i);
				if( _T("PDFOptions")  == sNodeName )
				{
					sResult.Append( _T("<PDFOptions>") );
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode( i , oNode );
					XmlUtils::CXmlReader oXmlSubReader;
					oXmlSubReader.OpenFromXmlNode( oNode );
					oXmlSubReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlSubReader.GetLengthList(); i++ )
					{
						CString sNodeName = oXmlSubReader.ReadNodeName(i);
						if( _T("Outline") == sNodeName )
						{
							sResult.Append( _T("<Outline>") );
							XML::IXMLDOMNodePtr oNode;
							oXmlSubReader.GetNode( i , oNode );
							sResult.Append( RecursiveCheckPdfOutline( oNode, aSortedPages) );
							sResult.Append( _T("</Outline>") );
						}
						else
							sResult.Append(oXmlSubReader.ReadNodeXml(i));
					}
					sResult.Append( _T("</PDFOptions>") );
				}
				else
					sResult.Append( oXmlReader.ReadNodeXml(i) );
			}
		}
		sResult.Append( _T("</Options>") );
		return sResult;
	}
	private: CString RecursiveCheckPdfOutline( XML::IXMLDOMNodePtr oNode, CAtlArray<int>& aPages )
	{
		CString sResult;
		XmlUtils::CXmlReader oXmlReader;
		if( TRUE ==  oXmlReader.OpenFromXmlNode( oNode ) )
		{
			oXmlReader.ReadNodeList( _T("Chapter") );
			for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
			{
				CString sName = oXmlReader.ReadNodeAttribute(i, _T("name") );
				CString sPage = oXmlReader.ReadNodeAttribute(i, _T("page") );
				if( _T("") == sPage )
					continue;
				int nPage = Strings::ToInteger( sPage );
				int nExist = -1;
				for( int i = 0; i < aPages.GetCount(); i++ )
					if( aPages[i] == nPage )
					{
						nExist = i;
						break;
					}
				if( -1 != nExist )
				{
					sResult.AppendFormat( _T("<Chapter name=\"%s\" page=\"%d\" >"), Utils::PrepareToXML( sName ), nExist );
					XML::IXMLDOMNodePtr oNode;
					oXmlReader.GetNode( i , oNode );
					sResult.Append( RecursiveCheckPdfOutline( oNode, aPages ) );
					sResult.Append( _T("</Chapter>") );
				}
			}
		}
		return sResult;
	}
	private: static int compare (const void * a, const void * b)
	{
	  return ( *(int*)a - *(int*)b );
	}
private: long GetProgress( long nCurProgress )
		 {
			 return (long)(c_nMaxProgressPercent * (m_dMinProgress + ( m_dMaxProgress - m_dMinProgress ) * nCurProgress / c_nMaxProgressPercent));
		 }
private: long PdfReadWriterOptions(BSTR bsXMLOptions, AVSOfficePDFWriter::IPDFWriter *pWriter)
	{
		HRESULT hRes = S_OK;
		if ( NULL == pWriter )
			return S_OK;

		//<Options>
		//	<FileType>513</FileType>
		//	<PDFOptions>
		//		<Outline>
		//			<Chapter name="toPage2" page="1" /> 
		//			<Chapter name="toPage3" page="2" /> 
		//			<Chapter name="toPage1" page="0" /> 
		//			<Chapter name="toPage4" page="3" /> 
		//		</Outline>
		//		<Encryption>
		//			<OwnerPassword value="Password" /> 
		//			<UserPassword value="Password" print="false" modify="false" copy="false" /> 
		//		</Encryption>
		//	</PDFOptions>
		//</Options>

		CString sXml( bsXMLOptions );

		XmlUtils::CXmlNode oOptionsNode;
		oOptionsNode.FromXmlString( sXml );
		if ( -1 != oOptionsNode.GetName().Find( _T("Options") ) )
		{
			XmlUtils::CXmlNode oPDFOptionsNode;
			oOptionsNode.GetNode( _T("PDFOptions") , oPDFOptionsNode);
			if ( -1 != oPDFOptionsNode.GetName().Find( _T("PDFOptions") ) )
			{
				XmlUtils::CXmlNode oOutlineNode, oEncryptionNode;
				if ( oPDFOptionsNode.GetNode( _T("Outline"), oOutlineNode ) )
				{
					CString sOutline = oOutlineNode.GetXml();
					BSTR bsOutline = sOutline.AllocSysString();
					if ( FAILED( hRes = pWriter->MakeOutlineFromXml( bsOutline ) ) )
					{
						::SysFreeString( bsOutline );
						return hRes;
					}
				}
				if ( oPDFOptionsNode.GetNode( _T("Encryption"), oEncryptionNode ) )
				{
					XmlUtils::CXmlNode oUser, oOwner;
					CString sOwner, sUser;
					long nPermissions = 0;
					if ( oEncryptionNode.GetNode( _T("OwnerPassword"), oOwner ) )
					{
						sOwner = oOwner.GetAttribute( _T("value"), _T("") );
					}
					if ( oEncryptionNode.GetNode( _T("UserPassword"), oUser ) )
					{
						sUser = oUser.GetAttribute( _T("value"), _T("") );						
						CString sPrint  = oUser.GetAttribute( _T("print") );
						CString sModify = oUser.GetAttribute( _T("modify") );
						CString sCopy   = oUser.GetAttribute( _T("copy") );

						// 3 бит
						if ( -1 == sPrint.Find( _T("0") ) ) 
							nPermissions += 4;

						// 4 бит
						if ( -1 == sModify.Find( _T("0") ) )
							nPermissions += 8;

						// 5 бит
						if ( -1 == sCopy.Find( _T("0") ) )
							nPermissions += 16;
					}

					BSTR bsOwner = sOwner.AllocSysString();
					BSTR bsUser  = sUser.AllocSysString();
					if ( FAILED( hRes = pWriter->SetPDFPassword( bsOwner, bsUser ) ) )
					{
						::SysFreeString( bsOwner );
						::SysFreeString( bsUser );
						return hRes;
					}
					::SysFreeString( bsOwner );
					::SysFreeString( bsUser );

					if ( FAILED( hRes = pWriter->SetPDFPermission( nPermissions ) ) )
						return hRes;
				}

			}
		}
		return S_OK;
	}
private: void UpdateGdiPlusRenderer(double dWidthMm, double dHeightMm, MediaCore::IAVSUncompressedVideoFrame** piImage, AVSGraphics::IASCGraphicsRenderer** piRend)
		 {
			 (*piImage) = NULL;
			 (*piRend) = NULL;
			 CoCreateInstance(__uuidof( MediaCore::CAVSUncompressedVideoFrame), NULL ,CLSCTX_INPROC_SERVER, __uuidof(MediaCore::IAVSUncompressedVideoFrame), (void **)piImage);

			 //задаем dpi без привязки к монитору
			 int nLOGPIXELSX = 96;
			 int nLOGPIXELSY = 96;

			 //размер исходной картинки
			 long nWidthPix = (long)(nLOGPIXELSX * dWidthMm / (10 * ONE_INCH));
			 long nHeightPix = (long)(nLOGPIXELSY * dWidthMm * dHeightMm / (10 * dWidthMm * ONE_INCH));

			 (*piImage)->Width = nWidthPix;
			 (*piImage)->Height = nHeightPix;
			 (*piImage)->put_Stride(0, nWidthPix * 4);

			 (*piImage)->AspectRatioX = nWidthPix;
			 (*piImage)->AspectRatioY = nHeightPix;

			 (*piImage)->DataSize = 4 * nWidthPix * nHeightPix;

			 (*piImage)->ColorSpace = 64 + (1 << 31);
			 (*piImage)->AllocateBuffer(-1);

			 memset((*piImage)->Buffer, 255, (*piImage)->BufferSize);

			 CoCreateInstance(__uuidof( AVSGraphics::CASCGraphicsRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IASCGraphicsRenderer), (void **)piRend);
			 //ставим FontManager
			 VARIANT vtVariant;
			 vtVariant.vt = VT_UNKNOWN;
			 vtVariant.punkVal = NULL;
			 (*piRend)->SetAdditionalParam( L"FontManager", vtVariant );

			 (*piRend)->put_Width(dWidthMm);
			 (*piRend)->put_Height(dHeightMm);
			 (*piRend)->CreateFromMediaData(*piImage, 0, 0, nWidthPix, nHeightPix );
		 }
};
