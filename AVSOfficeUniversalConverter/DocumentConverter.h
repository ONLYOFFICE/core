#pragma once
#include "xmlutils.h"
#include "AVSOfficeFileCache.h"
#include "BaseThread.h"
#include "../Common/OfficeFileTemplate.h"
#include "../Common/OfficeFileFormats.h"
#include "../Common/OfficeDefines.h"
#include "UniversalConverterEvents.h"
#include "XmlUtils.h"
#include "Registration.h"
#include "StringUtils.h"
#include "Utils.h"

struct Source{
	CString sFilename;
	int nId;
	int nStartPage;
	int nEndPage;
	int nFileType;
	CString sOptions;
	Source()
	{
		nId = -1;
		nStartPage = -1;
		nEndPage = -1;
		nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
	}
};
struct Destination{
	CString sFilename;
	int nFileType;
	CString sOptions;
	Destination()
	{
		nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
	}
};
class DocumentItem{
public:	CAtlArray<Source> aSources;
public:	CAtlArray<Destination> aDestinations;
public: DocumentItem()
		{
		}
public: DocumentItem( const DocumentItem& oDoc )
		{
			aSources.RemoveAll();
			aSources.Append( oDoc.aSources );
			aDestinations.RemoveAll();
			aDestinations.Append( oDoc.aDestinations );
		}
public: DocumentItem& operator=( const DocumentItem& oDoc )
		{
			aSources.RemoveAll();
			aSources.Append( oDoc.aSources );
			aDestinations.RemoveAll();
			aDestinations.Append( oDoc.aDestinations );
			return (*this);
		}

public: BOOL IsValid()
		{
			BOOL bResult = TRUE;
			if( aSources.GetCount() == 0  )
				bResult = FALSE;
			else 
			{
				for( INT32 i=0;i < (INT32)aSources.GetCount() ;i++)
				{
					if( aSources[i].sFilename == _T("") || aSources[i].nId == -1 )
					{
						bResult = FALSE; 
						break;
					}
				}
			}
			if( aDestinations.GetCount() == 0  )
				bResult = FALSE;
			else 
			{
				for( INT32 i=0;i < (INT32)aDestinations.GetCount() ;i++)
				{
					if( aDestinations[i].sFilename == _T("") )
					{
						bResult = FALSE; 
						break;
					}
				}
			}
			return bResult;
		}
};

[ event_source(native), event_receiver(native) ]
class DocumentConverter : public CBaseThread
{
//----------------------------- Public Members --------------------------------

public: __event void OnComplete();
public: __event void OnError( LONG nValue, int nId );
public: __event void OnErrorNoCritical( LONG nValue, int nId );
public: __event void OnDestinationFileStart( int nId );
public: __event void OnDestinationFileComplete( int nId );
public: __event bool OnGetTask( DocumentItem& oNewTask );
public: __event void OnProcessFile( int nId, LONG nProcess );

//----------------------------- Public Methods --------------------------------
public:	DocumentConverter() : CBaseThread( 0 )
		{
			m_piFormatChecker.CreateInstance( __uuidof(OfficeFile::CAVSOfficeFormatChecker) );
			//m_poEventReciver = new COfficeFileConverterEvents<OfficeFile::_IAVSOfficeFileEvents,DocumentConverter>(this);
			m_piFilesCache = NULL;
			Clear();
		}
public: ~DocumentConverter()
		{
			//delete m_poEventReciver;
			Stop();
			RELEASEINTERFACE( m_piFilesCache );
		}
public: void Stop( bool bWait = false )
		{
			if( true == bWait )
			{
				CBaseThread::Stop();
			}
			else
			{
				if (!m_bRunThread)
					return;
				m_bRunThread = FALSE;
				RELEASEHANDLE(m_hThread);
			}
		}
public: VOID SetCache( IAVSOfficeFilesCache* piFilesCache )
		{
			RELEASEINTERFACE( m_piFilesCache );
			if( NULL != piFilesCache )
			{
				piFilesCache->QueryInterface( __uuidof( IAVSOfficeFilesCache ), (void**)&m_piFilesCache );
			}
		}
public: void Clear()
		 {
			m_dCurOperationProcess = 0;
			m_nCurFileID = -1;
			m_nLastProgress = -1;
			m_dMinProgress = 0;
			m_dMaxProgress = 1;
		 }
public: void _OnProgress(LONG nID,  LONG lPercent)
		{
			CheckSuspend();
			if( FALSE == m_bRunThread )
				return;
			
			m_dCurOperationProcess = lPercent;
			long nCurProgress = GetProcess();
			if( nCurProgress > m_nLastProgress )
			{
				m_nLastProgress = nCurProgress;
				OnProcessFile( m_nCurFileID, nCurProgress );
			}
		}
public: void _OnProgressEx( LONG nID, LONG nPercent, SHORT* Cancel )
		{
			if( NULL == Cancel )
				return;
			if( FALSE == m_bRunThread )
				(*Cancel) = c_shProgressCancel;
			else
			{
				(*Cancel) = c_shProgressContinue;
				_OnProgress( nID, nPercent );
			}
		}
public: long GetProcess()
		{
			long nCurProcess ;
			if( m_bRunThread == FALSE )
				return 0;
			else
				nCurProcess = long(c_nMaxProgressPercent * (m_dMinProgress + ( m_dMaxProgress - m_dMinProgress) *  m_dCurOperationProcess / c_nMaxProgressPercent ));
			return nCurProcess;
		}

public: void SetThreadPriority(INT32 nPriority)
		{
			if( NULL != m_hThread )
				::SetThreadPriority( m_hThread, nPriority );
		}
public: VOID SetTempDirectory( CString sTempDir )
		{
			m_sTempDirectory = sTempDir;
		}
//----------------------------- Private Methods --------------------------------
private: DWORD ThreadProc()
		 {
			if ( !CRegistratorClient::IsRegistered() )
				return S_OK;
			CoInitialize( NULL );
			DocumentItem oWorItem;
			while ( true == OnGetTask( oWorItem ) )
			{
				m_dCurOperationProcess = 0;

				if( FALSE == m_bRunThread )
					break;

				OfficeFile::IAVSOfficeFile* piOfficeFile = NULL;
				HRESULT hRes = S_OK;
				Source& oFirstSource = Source();
				try
				{
					CAtlArray< IUnknownPtr > aSaveRenderers;//Для соединения файлов
					CAtlArray< bool > aSaveStoppedConvertation;//Для соединения файлов
					int nSourcesCount = oWorItem.aSources.GetCount();
					int nOldProgressSum = 0;
					int nAllProgressSum = 0;
					//Пробегаемся по всем officefile, узнаем количество страниц, чтобы прогресс был плавным
					for(int nSIndex = 0; nSIndex < nSourcesCount; nSIndex++)
					{
						Source& oCurSource = oWorItem.aSources.GetAt(nSIndex);
						if(-1 != oCurSource.nStartPage && -1 != oCurSource.nEndPage)
							nAllProgressSum += oCurSource.nEndPage - oCurSource.nStartPage + 1;
						else
						{
							OfficeFile::IAVSOfficeFile* piCurOfficeFile = GetOfficeFile(oCurSource);
							if( NULL == piCurOfficeFile )
								throw AVS_ERROR_UNEXPECTED;
							long nCurPageCount = 0;;
							piCurOfficeFile->GetPageCount(&nCurPageCount);
							nAllProgressSum += nCurPageCount;
							RELEASEINTERFACE(piCurOfficeFile);
						}
					}
					for(int nSIndex = 0; nSIndex < nSourcesCount; nSIndex++)
					{
					Source& oSource = oWorItem.aSources.GetAt(nSIndex);
					if(0 == nSIndex)
						oFirstSource = oSource;
					m_nCurFileID = oSource.nId;
					//проверяем наличие файла
					if( ::GetFileAttributes( oSource.sFilename ) == INVALID_FILE_ATTRIBUTES )
						throw AVS_ERROR_FILEACCESS;
					CheckSuspend();

					if( false == CheckPossibleOperation( oWorItem, oSource ) )
						throw AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_CONVERT_THIS_FORMATS;

					//конвертация в docx не нужна, если source и destination форматы совпадают
					if(1 == nSourcesCount)
					{
					for( int i = oWorItem.aDestinations.GetCount() - 1; i >= 0; i-- )
					{
						bool bUseHtml = false;
						if( oSource.nFileType == oWorItem.aDestinations[i].nFileType && 
							( oSource.nFileType != AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF || IsEmptyPdfOptions (oWorItem.aDestinations[i].sOptions) ))
						{
							//копируем файл
							int nRes = Utils::CopyDirOrFile( oSource.sFilename, oWorItem.aDestinations[i].sFilename );
							if( 0 != nRes )
								throw AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_COPY;
							//для html копируем еще и папку с именем *_files
							if( AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == oSource.nFileType )
							{
								TCHAR tDriveSrc[256];
								TCHAR tDirSrc[256];
								TCHAR tFilenameSrc[256];
								_tsplitpath( oSource.sFilename, tDriveSrc, tDirSrc, tFilenameSrc, NULL );
								CString sHtmlFileDirectorySrc = CString(tDriveSrc) + CString(tDirSrc) + CString(tFilenameSrc) + _T("_files");

								TCHAR tDriveDest[256];
								TCHAR tDirDest[256];
								TCHAR tFilenameDest[256];
								_tsplitpath( oWorItem.aDestinations[i].sFilename, tDriveDest, tDirDest, tFilenameDest, NULL );
								CString sHtmlFileDirectoryDest = CString(tDriveDest) + CString(tDirDest) + CString(tFilenameDest) + _T("_files");

								if( 0 != (::GetFileAttributes( sHtmlFileDirectorySrc ) & FILE_ATTRIBUTE_DIRECTORY) )
								{
									nRes = Utils::CopyDirOrFile( sHtmlFileDirectorySrc, sHtmlFileDirectoryDest );
									if( 0 != nRes )
										throw AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_COPY;
								}
							}
							oWorItem.aDestinations.RemoveAt( i );
						}
					}
					}
					if( 0 == oWorItem.aDestinations.GetCount() )
					{
						OnDestinationFileStart( oSource.nId );
						OnDestinationFileComplete( oSource.nId );
						continue;
					}

					HRESULT hResult;
					//Сигнализиуем о начале конвертации нового файла
					OnDestinationFileStart( oSource.nId );

					piOfficeFile = GetOfficeFile(oSource);
					if( NULL == piOfficeFile )
						throw AVS_ERROR_UNEXPECTED;
					long nCurPageCount = 0;
					if(-1 != oSource.nStartPage && -1 != oSource.nEndPage)
						nCurPageCount += oSource.nEndPage - oSource.nStartPage + 1;
					else
						piOfficeFile->GetPageCount(&nCurPageCount);

					//делим destination на стандартные и нет
					CAtlArray< Destination > aStandart;
					CAtlArray< Destination > aNoStandart;
					for( int i = 0; i < (int)oWorItem.aDestinations.GetCount(); i++ )
					{
						//ExtractImages
						if( AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE == oWorItem.aDestinations[i].nFileType )
							aNoStandart.Add( oWorItem.aDestinations[i] );
						// xls, xlsx -> html, mht, xls, xlsx
						else if( 0 != ( AVS_OFFICESTUDIO_FILE_SPREADSHEET & oSource.nFileType ) &&
							( AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == oWorItem.aDestinations[i].nFileType ||
							AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == oWorItem.aDestinations[i].nFileType ||
							0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & oWorItem.aDestinations[i].nFileType )) )
							aNoStandart.Add( oWorItem.aDestinations[i] );
						// html -> mht, mht -> html
						else if( (AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == oSource.nFileType &&
							AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == oWorItem.aDestinations[i].nFileType ) /*||
							( AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == oSource.nFileType &&
							AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == oWorItem.aDestinations[i].nFileType  ) */)
							aNoStandart.Add( oWorItem.aDestinations[i] );
						//else if( AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == oSource.nFileType &&
						//	AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == oWorItem.aDestinations[i].nFileType )
						//	aNoStandart.Add( oWorItem.aDestinations[i] );
						else
							aStandart.Add( oWorItem.aDestinations[i] );
					}
					if(aStandart.GetCount() > 0)
					{
						for(int i = 0, length = aStandart.GetCount(); i < length; i++)
						{
							aSaveRenderers.Add(NULL);
							aSaveStoppedConvertation.Add(false);
						}
					}

					//Для pdf и презентаций добавляем тег в опции destination, что надо использовать CommandRenderer
					if( 0 != (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM & oSource.nFileType) || 0 != (AVS_OFFICESTUDIO_FILE_PRESENTATION & oSource.nFileType) )
					{
						for( int i = 0; i < (int)aStandart.GetCount(); i++ )
							aStandart[i].sOptions.Append( _T("<LoadInCommandRenderer/>") );
						for( int i = 0; i < (int)aNoStandart.GetCount(); i++ )
							aNoStandart[i].sOptions.Append( _T("<LoadInCommandRenderer/>") );
					}

					//выполняем сначала нестандартные
					for( int i = 0; i < (int)aNoStandart.GetCount(); i++ )
					{
						if( false == m_bRunThread )
							break;
						//Extract Images
						if( AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE == aNoStandart[i].nFileType )
						{
							long nFileType = oSource.nFileType;
							if( AVS_OFFICESTUDIO_FILE_UNKNOWN == nFileType )
							{
								BSTR bstrFilename = oSource.sFilename.AllocSysString();
								m_piFormatChecker->GetFileFormat( bstrFilename, &nFileType );
								SysFreeString( bstrFilename );
							}
							//создаем папку для ExtractImage, если необходимо
							DWORD dwExtractDirAttr = ::GetFileAttributes(aNoStandart[i].sFilename);
							if( INVALID_FILE_ATTRIBUTES == dwExtractDirAttr )
							{
								if( 0 == CreateDirectory( aNoStandart[i].sFilename, NULL ) )
									throw AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_CREATE_EXTR_DIR;
							}
							else if( 0 == (FILE_ATTRIBUTE_DIRECTORY & dwExtractDirAttr) )
								throw AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_CREATE_EXTR_DIR;
							
							CString sExtractXml;
							VARIANT_BOOL vbCanConvert = VARIANT_TRUE;
							if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFileType || AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX == nFileType)
							{
								m_dMinProgress = (nOldProgressSum + 0.0 * nCurPageCount) / nAllProgressSum;
								m_dMaxProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
								sExtractXml.Append( _T("<Options>") );
								if( AVS_OFFICESTUDIO_FILE_UNKNOWN != nFileType )
									sExtractXml.AppendFormat( _T("<FileType>%d</FileType>"), nFileType );
								sExtractXml.Append( oSource.sOptions );
								sExtractXml.AppendFormat( _T("<ExtractOption><Images path=\"%s\"/></ExtractOption></Options>"), Utils::PrepareToXML( aNoStandart[i].sFilename ) );

								//на всякий случай делаем новый IAVSOfficeFileConverter
								OfficeFile::IAVSOfficeFileConverterPtr piFileConverter = NULL;
								piFileConverter.CreateInstance( __uuidof( OfficeFile::CAVSOfficeFileConverter ) );
								if( NULL == piFileConverter )
									throw AVS_ERROR_UNEXPECTED;
								BSTR bstrTemp = m_sTempDirectory.AllocSysString();
								piFileConverter->put_TempDirectory( bstrTemp );
								SysFreeString( bstrTemp );
								piFileConverter->put_FileType( nFileType );

								COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents ,DocumentConverter >* poEventReciver = new COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents,DocumentConverter >( this );
								poEventReciver->AddRef();
								poEventReciver->Advise( piFileConverter );

								BSTR bstrFilename = oSource.sFilename.AllocSysString();
								BSTR bstrOptions = sExtractXml.AllocSysString();
								hRes = piFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
								SysFreeString( bstrFilename );
								SysFreeString( bstrOptions );

								poEventReciver->UnAdvise( piFileConverter );
								RELEASEINTERFACE( poEventReciver );

								if( FAILED( hRes ) )
									throw hRes;
							}
							else
							{
								//берем существующий интерфейс конвертера
								OfficeFile::IAVSOfficeFileConverterPtr piFileConverter;
								piOfficeFile->GetInternalInteface( (IUnknown**)&piFileConverter );
								//проверяем не сделан ли load
								VARIANT_BOOL vbIsLoadComplete = VARIANT_FALSE;
								piFileConverter->get_IsLoadComplete( &vbIsLoadComplete );
								if( VARIANT_FALSE == vbIsLoadComplete )
								{
									m_dMinProgress = (nOldProgressSum + 0 * nCurPageCount) / nAllProgressSum;
									m_dMaxProgress = (nOldProgressSum + 0.500 * nCurPageCount) / nAllProgressSum;
									//-----------------Конвертация в docx
									piOfficeFile->put_FileType( nFileType );

									//цепляем события
									COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents ,DocumentConverter >* poEventReciver = new COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents,DocumentConverter >(this);
									poEventReciver->AddRef();
									poEventReciver->Advise( piFileConverter );

									BSTR bstrFilename = oSource.sFilename.AllocSysString();
									CString sOptions = _T("<Options>");
									if( AVS_OFFICESTUDIO_FILE_UNKNOWN != nFileType )
										sOptions.AppendFormat( _T("<FileType>%d</FileType>"), nFileType );
									sOptions.Append( oSource.sOptions );
									sOptions.Append( _T("</Options>") );
									BSTR bstrOptions = sOptions.AllocSysString();
									hRes = piFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
									SysFreeString( bstrFilename );
									SysFreeString( bstrOptions );
									//отцепляем события
									poEventReciver->UnAdvise( piFileConverter );
									RELEASEINTERFACE(poEventReciver);
									if( FAILED( hRes ) )
										throw hRes;
								}
								if( VARIANT_FALSE == vbIsLoadComplete )
								{
									m_dMinProgress = (nOldProgressSum + 0.500 * nCurPageCount) / nAllProgressSum;
									m_dMaxProgress = (nOldProgressSum + 0.600 * nCurPageCount) / nAllProgressSum;
								}
								else
								{
									m_dMinProgress = (nOldProgressSum + 0 * nCurPageCount) / nAllProgressSum;
									m_dMaxProgress = (nOldProgressSum + 0.100 * nCurPageCount) / nAllProgressSum;
								}
								//прогресс
								RaiseProgress();
								if( false == m_bRunThread )
									break;
								//-----------------docx запаковывается в файл
								//получаем папку-результат конвертации в docx
								BSTR bstrTempDir;
								piFileConverter->get_TempDirectory( &bstrTempDir );
								CString sDocxTempDir(bstrTempDir);
								SysFreeString( bstrTempDir );

								//создаем другой конвертер для запаковки docx и extract
								OfficeFile::IAVSOfficeFileConverterPtr piOtherFileConverter;//для сохрание в docx и извлечения картинок
								piOtherFileConverter.CreateInstance( __uuidof( OfficeFile::CAVSOfficeFileConverter ) );

								//сохраняем docx файл
								piOtherFileConverter->put_FileType( AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX );

								//цепляем события
								COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents ,DocumentConverter >* poEventReciver = new COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents,DocumentConverter >(this);
								poEventReciver->AddRef();
								poEventReciver->Advise( piOtherFileConverter );

								CString sTempDocxFile;
								sTempDocxFile = Utils::CreateTempFile( m_sTempDirectory );
								BSTR bstrFilename = sTempDocxFile.AllocSysString();
								bstrTempDir = sDocxTempDir.AllocSysString();
								CString sOptions = _T("<Options>");
								sOptions.AppendFormat( _T("<FileType>%d</FileType>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX );
								if( -1 == oSource.sOptions.Find(_T("ConvertListOptions")) )
									sOptions.Append( _T("<ConvertListOptions list2list=\"false\"/>") );//чтобы в html списки переходили в текст
								sOptions.Append( oSource.sOptions );
								sOptions.Append( _T("</Options>") );
								BSTR bstrOptions = sOptions.AllocSysString();
								hRes = piOtherFileConverter->SaveToFile( bstrFilename, bstrTempDir, L"" );
								SysFreeString( bstrFilename );
								SysFreeString( bstrTempDir );
								SysFreeString( bstrOptions );
								//отцепляем события
								poEventReciver->UnAdvise( piOtherFileConverter );
								RELEASEINTERFACE(poEventReciver);
								if( FAILED( hRes ) )
								{
									Utils::RemoveDirOrFile( sTempDocxFile );
									throw hRes;
								}
								if( VARIANT_FALSE == vbIsLoadComplete )
								{
									m_dMinProgress = (nOldProgressSum + 0.600) / nAllProgressSum;
									m_dMaxProgress = (nOldProgressSum + 1.0) / nAllProgressSum;
								}
								else
								{
									m_dMinProgress = (nOldProgressSum + 0.100 * nCurPageCount) / nAllProgressSum;
									m_dMaxProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
								}
								//прогресс
								RaiseProgress();
								
								//-----------------Извлекаем картинки из docx файла
								//извлекаем из файла картинки
								piOtherFileConverter->put_FileType( AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX );
								bstrFilename = sTempDocxFile.AllocSysString();

								//цепляем события
								poEventReciver = new COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents,DocumentConverter >(this);
								poEventReciver->AddRef();
								poEventReciver->Advise( piOtherFileConverter );

								sExtractXml.Append( _T("<Options>") );
								sExtractXml.AppendFormat( _T("<FileType>%d</FileType>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX );
								sExtractXml.Append( oSource.sOptions );
								sExtractXml.AppendFormat( _T("<ExtractOption><Images path=\"%s\"/></ExtractOption></Options>"), Utils::PrepareToXML( aNoStandart[i].sFilename ) );
								bstrOptions = sExtractXml.AllocSysString();
								hRes = piOtherFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
								SysFreeString( bstrFilename );
								SysFreeString( bstrOptions );

								//отцепляем события
								poEventReciver->UnAdvise( piOtherFileConverter );
								RELEASEINTERFACE(poEventReciver);

								//удаляем временный docx
								Utils::RemoveDirOrFile( sTempDocxFile );
								if( FAILED( hRes ) )
									throw hRes;
							}
							m_dMinProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
							m_dMaxProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
							RaiseProgress();
						}
						else
						{
							//поскольку в результате не получается папка с docx
							//лучше создать новый officeFile
							OfficeFile::IAVSOfficeFileConverterPtr piFileConverter = NULL;
							piFileConverter.CreateInstance( __uuidof( OfficeFile::CAVSOfficeFileConverter ) );
							if( NULL == piFileConverter )
								throw AVS_ERROR_UNEXPECTED;

							//цепляем события
							COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents ,DocumentConverter >* poEventReciver = new COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents,DocumentConverter >(this);
							poEventReciver->AddRef();
							poEventReciver->Advise( piFileConverter );

							BSTR bstrTempDir = m_sTempDirectory.AllocSysString();
							piFileConverter->put_TempDirectory( bstrTempDir );
							SysFreeString( bstrTempDir );

							if( 0 != ( AVS_OFFICESTUDIO_FILE_SPREADSHEET & oSource.nFileType ) && 
								0 != (AVS_OFFICESTUDIO_FILE_SPREADSHEET & oWorItem.aDestinations[i].nFileType ) )
							{
								// XLS -> XLSX
								// XLSX-> XLS
								m_dMinProgress = (nOldProgressSum + 0.0 * nCurPageCount) / nAllProgressSum;
								m_dMaxProgress = (nOldProgressSum + 0.500 * nCurPageCount) / nAllProgressSum;

								//xls* -> xlsx
								piFileConverter->put_FileType( oSource.nFileType );

								BSTR bstrFilename = oSource.sFilename.AllocSysString();
								CString sOptions = _T("<Options>");
								sOptions.AppendFormat( _T("<FileType>%d</FileType>"), oSource.nFileType );
								sOptions.AppendFormat( _T("<NoStandartOpen destinationtype=\"%d\" destinationpath=\"%s\"/>"), AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX , Utils::PrepareToXML(oWorItem.aDestinations[i].sFilename) );
								sOptions.Append( oSource.sOptions );
								sOptions.Append( _T("</Options>") );
								BSTR bstrOptions = sOptions.AllocSysString();
								hRes = piFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
								SysFreeString( bstrFilename );
								SysFreeString( bstrOptions );

								if( false == m_bRunThread )
									break;
								if( FAILED( hRes ) )
									throw hRes;
								m_dMinProgress = (nOldProgressSum + 0.500 * nCurPageCount) / nAllProgressSum;
								m_dMaxProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
								RaiseProgress();

								//xlsx -> xls*
								piFileConverter->put_FileType( oWorItem.aDestinations[i].nFileType );

								bstrFilename = oWorItem.aDestinations[i].sFilename.AllocSysString();
								sOptions = _T("<Options>");
								sOptions.AppendFormat( _T("<FileType>%d</FileType>"), oWorItem.aDestinations[i].nFileType );
								if( -1 == oWorItem.aDestinations[i].sOptions.Find(_T("ConvertListOptions")) )
									sOptions.Append( _T("<ConvertListOptions list2list=\"false\"/>") );//чтобы в html списки переходили в текст
								sOptions.Append( oWorItem.aDestinations[i].sOptions );
								sOptions.Append( _T("</Options>") );
								bstrOptions = sOptions.AllocSysString();
								hRes = piFileConverter->SaveToFile( bstrFilename, L"", bstrOptions );
								SysFreeString( bstrFilename );
								SysFreeString( bstrOptions );
							}
							else if( 0 != ( AVS_OFFICESTUDIO_FILE_SPREADSHEET & oSource.nFileType ) &&
								( AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == oWorItem.aDestinations[i].nFileType ||
								AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == oWorItem.aDestinations[i].nFileType) )
							{
								// XLS , XLSX -> HTML

								CString sTargetFilename;
								bool bRemoveFolder = false;
								CString sTempFolder;
								if( AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == oWorItem.aDestinations[i].nFileType )
								{
									m_dMinProgress = (nOldProgressSum + 0.0 * nCurPageCount) / nAllProgressSum;
									m_dMaxProgress = (nOldProgressSum + 0.500 * nCurPageCount) / nAllProgressSum;
									sTempFolder = Utils::CreateTempDir( m_sTempDirectory );
									if( true == sTempFolder.IsEmpty() )
										throw AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_CREATE_TEMP_DIR;
									bRemoveFolder = true;
									sTargetFilename = Utils::CreateTempFile( sTempFolder ) + _T(".htm");
									if( true == sTargetFilename.IsEmpty() )
										throw AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_CREATE_TEMP_DIR;
								}	
								else
								{
									m_dMinProgress = (nOldProgressSum + 0.0 * nCurPageCount) / nAllProgressSum;;
									m_dMaxProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
									sTargetFilename = oWorItem.aDestinations[i].sFilename;
								}

								//xls* -> html
								piFileConverter->put_FileType( oSource.nFileType );

								BSTR bstrFilename = oSource.sFilename.AllocSysString();
								CString sOptions = _T("<Options>");
								sOptions.AppendFormat( _T("<FileType>%d</FileType>"), oSource.nFileType );
								sOptions.AppendFormat( _T("<NoStandartOpen destinationtype=\"%d\" destinationpath=\"%s\"/>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML , Utils::PrepareToXML(sTargetFilename) );
								sOptions.Append( oSource.sOptions );
								sOptions.Append( _T("</Options>") );
								BSTR bstrOptions = sOptions.AllocSysString();
								hRes = piFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
								SysFreeString( bstrFilename );
								SysFreeString( bstrOptions );
								
								if( AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == oWorItem.aDestinations[i].nFileType )
								{
									m_dMinProgress = (nOldProgressSum + 0.500 * nCurPageCount) / nAllProgressSum;
									m_dMaxProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
									if( false == m_bRunThread )
									{
										Utils::RemoveDirOrFile( sTargetFilename );
										break;
									}
									//html -> mht
									piFileConverter->put_FileType( AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML );

									BSTR bstrFilename = sTargetFilename.AllocSysString();
									CString sOptions = _T("<Options>");
									sOptions.AppendFormat( _T("<FileType>%d</FileType>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML );
									sOptions.AppendFormat( _T("<NoStandartOpen destinationtype=\"%d\" destinationpath=\"%s\"/>"), AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT , Utils::PrepareToXML(oWorItem.aDestinations[i].sFilename) );
									sOptions.Append( oSource.sOptions );
									sOptions.Append( _T("</Options>") );
									BSTR bstrOptions = sOptions.AllocSysString();
									hRes = piFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
									SysFreeString( bstrFilename );
									SysFreeString( bstrOptions );

									if( true == bRemoveFolder )
										Utils::RemoveDirOrFile( sTempFolder );
								}
							}
							else if( (AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == oSource.nFileType &&
								AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == oWorItem.aDestinations[i].nFileType ) /*||
								( AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT == oSource.nFileType &&
								AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == oWorItem.aDestinations[i].nFileType  )*/ )
							{
								m_dMinProgress = (nOldProgressSum + 0.0 * nCurPageCount) / nAllProgressSum;
								m_dMaxProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
								//html -> mht или mht -> html
								piFileConverter->put_FileType( oSource.nFileType );

								BSTR bstrFilename = oSource.sFilename.AllocSysString();
								CString sOptions = _T("<Options>");
								sOptions.AppendFormat( _T("<FileType>%d</FileType>"), oSource.nFileType );
								sOptions.AppendFormat( _T("<NoStandartOpen destinationtype=\"%d\" destinationpath=\"%s\"/>"), oWorItem.aDestinations[i].nFileType , Utils::PrepareToXML(oWorItem.aDestinations[i].sFilename) );
								sOptions.Append( oWorItem.aDestinations[i].sOptions );
								sOptions.Append( _T("</Options>") );
								BSTR bstrOptions = sOptions.AllocSysString();
								hRes = piFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
								SysFreeString( bstrFilename );
								SysFreeString( bstrOptions );
							}
							//else if( ( AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML == oSource.nFileType &&
							//	AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB == oWorItem.aDestinations[i].nFileType  ) )
							//{
							//	//меняем местами source и destination, чтобы вызвался контрол epub
							//	long nSourceType = oWorItem.aDestinations[i].nFileType;
							//	CString sSourcePath = oWorItem.aDestinations[i].sFilename;
							//	long nDestType = oSource.nFileType;
							//	CString sDestPath = oSource.sFilename;
							//	//html -> epub
							//	piFileConverter->put_FileType( nSourceType );

							//	BSTR bstrFilename = sSourcePath.AllocSysString();
							//	CString sOptions = _T("<Options>");
							//	sOptions.AppendFormat( _T("<FileType>%d</FileType>"), nSourceType );
							//	sOptions.AppendFormat( _T("<NoStandartSave destinationtype=\"%d\" destinationpath=\"%s\"/>"), nDestType , Utils::PrepareToXML(sDestPath) );
							//	sOptions.Append( oWorItem.aDestinations[i].sOptions );
							//	sOptions.Append( _T("</Options>") );
							//	BSTR bstrOptions = sOptions.AllocSysString();
							//	hRes = piFileConverter->SaveToFile( bstrFilename, L"", bstrOptions );
							//	SysFreeString( bstrFilename );
							//	SysFreeString( bstrOptions );
							//}
							//отцепляем события
							poEventReciver->UnAdvise( piFileConverter );
							RELEASEINTERFACE(poEventReciver);

							if( FAILED( hRes ) )
								throw hRes;
							m_dMinProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
							m_dMaxProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
							RaiseProgress();
						}
					}
					//выполняем стандартные
					if( aStandart.GetCount() > 0 )
					{
						bool bTryStop = false;
						if( false == m_bRunThread )
							bTryStop = true;

						OfficeFile::IAVSOfficeFileConverter* piFileConverter = NULL;
						piOfficeFile->GetInternalInteface((IUnknown**)&piFileConverter);
						//проверяем не сделан ли load
						VARIANT_BOOL vbIsLoadComplete = VARIANT_FALSE;
						piFileConverter->get_IsLoadComplete( &vbIsLoadComplete );
						m_dMinProgress = (nOldProgressSum + 0.0 * nCurPageCount) / nAllProgressSum;
						m_dMaxProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
						if( VARIANT_FALSE == vbIsLoadComplete )
						{
							m_dMinProgress = (nOldProgressSum + 0.0 * nCurPageCount) / nAllProgressSum;
							m_dMaxProgress = (nOldProgressSum + 0.500 * nCurPageCount) / nAllProgressSum;
							if( -1 != oSource.nFileType )
								piOfficeFile->put_FileType( oSource.nFileType );
							COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents ,DocumentConverter >* poEventReciver = new COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents,DocumentConverter >(this);
							poEventReciver->AddRef();
							poEventReciver->Advise( piFileConverter );

							BSTR bstrFilename = oSource.sFilename.AllocSysString();
							long nFileType;
							piOfficeFile->get_FileType( &nFileType );
							CString sOptions;
							sOptions.Append( _T("<Options>") );
							if( AVS_OFFICESTUDIO_FILE_UNKNOWN != nFileType )
								sOptions.AppendFormat( _T("<FileType>%d</FileType>"), nFileType );
							sOptions.Append( oSource.sOptions );
							if( AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFileType )
								sOptions.Append( _T("<Display/>"));
							sOptions.Append( _T("</Options>") );
							
							BSTR bstrOptions = sOptions.AllocSysString();
							hRes = piFileConverter->LoadFromFile( bstrFilename, L"", bstrOptions );
							SysFreeString( bstrFilename );
							SysFreeString( bstrOptions );

							//отцепляем события
							poEventReciver->UnAdvise( piFileConverter );
							RELEASEINTERFACE(poEventReciver);

							if( FAILED(hRes) )
							{
								RELEASEINTERFACE( piFileConverter );
								throw AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_LOAD_FILE;
							}
							m_dMinProgress = (nOldProgressSum + 0.500 * nCurPageCount) / nAllProgressSum;
							m_dMaxProgress = (nOldProgressSum + 1.0 * nCurPageCount) / nAllProgressSum;
						}
						RELEASEINTERFACE( piFileConverter );

						BSTR bstrTempDir;
						piOfficeFile->get_TempDirectory( &bstrTempDir );
						CString sConvertTempDir( bstrTempDir );
						SysFreeString( bstrTempDir );
				
						////применяем свойства документа
						//CString sSummary;
						//CString sSummaryPath;
						//GetSummaryFromDocx( sConvertTempDir, sSummary, sSummaryPath);

						RaiseProgress();
						for( int i = 0; i < (int)aStandart.GetCount(); i++ )
						{
							CheckSuspend();
							//может отличаться от того что пришел в xml в следствии копирования при наложении рекламы
							CString sRealDestinationPath = sConvertTempDir;
							bool bIsDestinationPathCopy = false;
							//пишем рекламу
							if( 0 == (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM & aStandart[i].nFileType) && 0 == (AVS_OFFICESTUDIO_FILE_IMAGE & aStandart[i].nFileType) &&
								( ( 0 != (AVS_OFFICESTUDIO_FILE_DOCUMENT & oSource.nFileType) ) ||
								( 0 != (AVS_OFFICESTUDIO_FILE_IMAGE & oSource.nFileType) ) ) )
							{
								XmlUtils::CXmlReader oXmlAdvertReader;
								if( _T("") != sRealDestinationPath && TRUE == oXmlAdvertReader.OpenFromXmlString( _T("<Options>") + aStandart[i].sOptions + _T("</Options>") ) )
									if( TRUE == oXmlAdvertReader.ReadRootNode( _T("Options") ) )
										if( TRUE == oXmlAdvertReader.ReadNode( _T("AddObjects") ) )
										{
											CString sAdvertOptions = oXmlAdvertReader.ReadNodeXml();
											DocxFile::IOfficeDocxFilePtr piDocxFile;
											piDocxFile.CreateInstance( __uuidof(DocxFile::COfficeDocxFile) );
											if( NULL != piDocxFile )
											{
												//делаем копию и туда записываем рекламу
												sRealDestinationPath = Utils::CreateTempDir( m_sTempDirectory );
												if( _T("") != sRealDestinationPath )
												{
													//удаляем папку, т.к. иначе произойдет копирование в папку, а не папки
													RemoveDirectory( sRealDestinationPath );
													//копируем
													bIsDestinationPathCopy = true;
													Utils::CopyDirOrFile( sConvertTempDir, sRealDestinationPath );

													//накладываем рекламу
													BSTR bstrDocxTempDir = sRealDestinationPath.AllocSysString();
													BSTR bstrAdvertOptions = sAdvertOptions.AllocSysString();
													piDocxFile->AddObjects( bstrDocxTempDir, bstrAdvertOptions );
													SysFreeString( bstrAdvertOptions );
													SysFreeString( bstrDocxTempDir );
												}
											}
										}
								if( FAILED( hRes ) )
								{
									if( true == bIsDestinationPathCopy )
										Utils::RemoveDirOrFile( sRealDestinationPath );
									throw AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_ADVERTISE;
								}
							}

							////применяем свойства документа
							//SaveSummaryToDocx( sConvertTempDir, aStandart[i].sOptions, sSummary, sSummaryPath );

							if( -1 != aStandart[i].nFileType )
								piOfficeFile->put_FileType( aStandart[i].nFileType );
							OfficeFile::IAVSOfficeFileConverter* piFileConverter = NULL;
							piOfficeFile->GetInternalInteface((IUnknown**)&piFileConverter);
							//цепляем события
							COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents ,DocumentConverter >* poEventReciver = new COfficeFileConverterEvents< OfficeFile::_IAVSOfficeFileConverterEvents,DocumentConverter >(this);
							poEventReciver->AddRef();
							poEventReciver->Advise( piFileConverter );
							BSTR bstrFilename = aStandart[i].sFilename.AllocSysString();
							CString sOptions = _T("<Options>");
							if( -1 == aStandart[i].sOptions.Find(_T("ConvertListOptions")) )
								sOptions.Append( _T("<ConvertListOptions list2list=\"false\"/>") );//чтобы в html списки переходили в текст
							if(-1 != oSource.nStartPage && -1 != oSource.nEndPage)
							{
								sOptions.Append( _T("<DocumentPages>") );
								for(int i = oSource.nStartPage; i <= oSource.nEndPage; i++)
								{
									TCHAR aPageNum[256];
									_itot(i, aPageNum, 10);
									sOptions.Append( CString(_T("<Page>")) + CString(aPageNum) + CString(_T("</Page>")) );
								}
								sOptions.Append( _T("</DocumentPages>") );
							}
							sOptions.Append( aStandart[i].sOptions );
							sOptions.Append( _T("</Options>") );
							BSTR bstrOptions = sOptions.AllocSysString();
							BSTR bstrDestinationPath = sRealDestinationPath.AllocSysString();
							//Выставляем режим сохранения
							VARIANT vtParam;
							vtParam.vt = VT_I4;
							vtParam.lVal = c_nSaveModeNone;
							if(0 == nSIndex)
								vtParam.lVal |= c_nSaveModeStart;
							if(nSourcesCount - 1 == nSIndex || bTryStop)
								vtParam.lVal |= c_nSaveModeCommit;
							if(0 != nSIndex && (nSourcesCount - 1) != nSIndex)
								vtParam.lVal |= c_nSaveModeContinue;
							piFileConverter->SetAdditionalParam(_T("SaveMode"), vtParam);
							//Выставляем сохраненный renderer
							if(0 != nSIndex)
							{
								vtParam.vt = VT_UNKNOWN;
								vtParam.punkVal = aSaveRenderers.GetAt(i);
								piFileConverter->SetAdditionalParam(_T("SaveRenderer"), vtParam);
							}
							if(false == aSaveStoppedConvertation.GetAt(i))
								hRes = piFileConverter->SaveToFile( bstrFilename, bstrDestinationPath, bstrOptions );
							//проверяем если конвертация застоплена(в прогрессе), то запоминаем и больше не сохраняем
							if(nSourcesCount > 0)
							{
								VARIANT vtParam;
								piFileConverter->GetAdditionalParam(_T("SaveRendererStopped"), &vtParam);
								if( VT_I4 == vtParam.vt)
								{
									if(0 != vtParam.lVal)
										aSaveStoppedConvertation.SetAt(i ,true);
								}
							}
							//Сохраняем renderer, чтобы писать в него и следующий source
							if(0 == nSIndex)	
							{
								piFileConverter->GetAdditionalParam(_T("SaveRenderer"), &vtParam);
								if(NULL != vtParam.punkVal)
								{
									aSaveRenderers.SetAt(i, vtParam.punkVal);
									RELEASEINTERFACE(vtParam.punkVal);
								}
							}

							SysFreeString( bstrFilename );
							SysFreeString( bstrOptions );
							SysFreeString( bstrDestinationPath );
							
							//отцепляем события
							poEventReciver->UnAdvise( piFileConverter );
							RELEASEINTERFACE(poEventReciver);
							if( true == bIsDestinationPathCopy )
								Utils::RemoveDirOrFile( sRealDestinationPath );
							if( FAILED(hRes) )
							{
								RELEASEINTERFACE( piFileConverter );
								throw AVS_UNIVERSALDOCUMENTCONVERTER_ERROR_SAVE_FILE;
							}


							//LONG nFileType = AVS_OFFICESTUDIO_FILE_UNKNOWN;
							//piFileConverter->get_FileType( &nFileType );
							//if( AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX != nFileType && AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX != nFileType && AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX != nFileType)
							//{
							//	//применяем свойства документа
							//	SaveSummaryInformation( aStandart[i].sFilename, aStandart[i].sOptions );
							//}
							RELEASEINTERFACE( piFileConverter );
						}
						if( bTryStop )
							break;
					}

					nOldProgressSum += nCurPageCount;
					//говорим о завершении операции
					OnDestinationFileComplete( oSource.nId);
				}
				}
				catch( HRESULT nExeption)
				{
					OnError( nExeption, oFirstSource.nId);
				}
				catch(...)
				{
					OnError( AVS_ERROR_UNEXPECTED, oFirstSource.nId );
				}
				//удаляем OfficeFile::IAVSOfficeFile
				RELEASEINTERFACE( piOfficeFile );
			}
			CheckSuspend();
			m_bRunThread = FALSE;
			OnComplete();
			CoUninitialize();
			return 0;
		 }
private: bool CheckPossibleOperation( DocumentItem& oInputItem,Source& oSource )
		 {
			 long nSourceFormat;
			 if( -1 != oSource.nFileType )
				nSourceFormat = oSource.nFileType;
			 else
			 {
				BSTR bstrFilename = oSource.sFilename.AllocSysString();
				m_piFormatChecker->GetFileFormat( bstrFilename, &nSourceFormat );
				SysFreeString( bstrFilename );
			 }
			 for( int i = 0; i < (int)oInputItem.aDestinations.GetCount(); i++ )
			 {
				 long nDestinationFormat ;
				 if( -1 != oSource.nFileType )
					nDestinationFormat = oInputItem.aDestinations[i].nFileType;
				 else
				 {
					BSTR bstrFilename = oInputItem.aDestinations[i].sFilename.AllocSysString();
					m_piFormatChecker->GetFileFormat( bstrFilename, &nDestinationFormat );
					SysFreeString( bstrFilename );
				 }
				 VARIANT_BOOL bCanConvert = VARIANT_FALSE;
				 m_piFormatChecker->CanConvert( nSourceFormat, nDestinationFormat, &bCanConvert);
				 if( VARIANT_FALSE == bCanConvert )
					 return false;
			 }
			 return true;
		 }
private: void RaiseProgress()
		 {
			 m_dCurOperationProcess = 0;

			 long nCurProgress = GetProcess();
			 if( nCurProgress > m_nLastProgress )
			 {
				 m_nLastProgress = nCurProgress;
				 OnProcessFile( m_nCurFileID, nCurProgress );
			 }
		 }

private: void GetSummaryFromDocx( CString sPath, CString& sSummary, CString& sSummaryPath )
		 {
			XmlUtils::CXmlReader oXmlReader;
			//ищем в Rels где находится свойства
			if( TRUE == oXmlReader.OpenFromFile( sPath + _T("\\[Content_Types].xml") ) )
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
						sSummaryPath = sCorePath;
						if( TRUE == oXmlReader.OpenFromFile( sPath + sCorePath ) )
						{
							//oXmlReader.ReadRootNode();
							//CString sHeader = oXmlReader.ReadNodeXml();
							oXmlReader.ReadRootNode( _T("cp:coreProperties"));
							CString sInternal = oXmlReader.ReadNodeXml();
							sSummary = sInternal;
						}
					}
				}
		 }
 
private: void SaveSummaryToDocx( CString sPath, CString& sOptions, CString& sSummary, CString& sSummaryPath )
		 {
			//читаем Options
			CString sTitle;
			CString sAuthor;
			CString sKeywords;
			CString sSubject;
			CString sRevisionNumber;
			CString sComments;
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromXmlString( sOptions ) )
				if( TRUE == oXmlReader.ReadNode( _T("FileInfo") ) )
				{
					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sProppertyName = oXmlReader.ReadNodeName(i);
						CString sProppertyValue = oXmlReader.ReadNodeText(i);
						if( _T("Title") == sProppertyName )
							sTitle = sProppertyValue;
						else if( _T("Author") == sProppertyName )
							sAuthor = sProppertyValue;
						else if( _T("Keywords") == sProppertyName )
							sKeywords = sProppertyValue;
						else if( _T("Subject") == sProppertyName )
							sSubject = sProppertyValue;
						else if( _T("RevisionNumber") == sProppertyName )
							sRevisionNumber = sProppertyValue;
						else if( _T("Comments") == sProppertyName )
							sComments = sProppertyValue;
					}
				}
				else
					return;

			if( "" == sSummary )
			{
				CString sHeader = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
				sSummary = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\"></cp:coreProperties>");
				CreateDirectory( sPath + _T("\\docProps"), NULL);
				//вносим в ContentTypes
				XmlUtils::CXmlReader oXmlContentReader;
				if( TRUE == oXmlContentReader.OpenFromFile( sPath + _T("\\[Content_Types].xml") ) )
				{
					if( TRUE == oXmlContentReader.ReadRootNode( _T("Types") ) )
					{
						CString sAdditionalContent = CString( _T("<Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/>") );
						CString sContentTypes = oXmlContentReader.ReadNodeXml();
						sContentTypes.Replace( _T("</Types>"), sAdditionalContent + _T("</Types>") );
						XmlUtils::CXmlWriter oXmlContentWriter;
						oXmlContentWriter.SetXmlString( sHeader + sContentTypes );
						oXmlContentWriter.SaveToFile( sPath + _T("\\[Content_Types].xml"), TRUE );
					}
				}
				else
					return;
				//вносим в rels
				XmlUtils::CXmlReader oXmlRelsReader;
				if( TRUE == oXmlRelsReader.OpenFromFile( sPath + _T("\\_rels\\.rels") ) )
				{
					if( TRUE == oXmlRelsReader.ReadRootNode( _T("Relationships") ) )
					{
						//просматриваем документ чтобы получить уникальный ID(просто бурем на 1 больше чем максимальный)
						int nMax = -1;
						oXmlRelsReader.ReadNodeList( _T("Relationship") );
						for( int i = 0; i < oXmlRelsReader.GetLengthList(); i++ )
						{
							CString sRID = oXmlRelsReader.ReadNodeAttribute(i, _T("Id") );
							sRID.MakeLower();
							sRID.Replace( _T("rid"), _T("") );
							int nId = Strings::ToInteger( sRID );
							if( nId > nMax )
								nMax = nId;
						}
						int nID;
						if( -1 != nMax )
							nID = nMax + 1;
						else
							nID = 10001;
						CString sRels = oXmlRelsReader.ReadNodeXml();
						CString sAdditionalContent;
						sAdditionalContent.AppendFormat( _T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/>"), nID );
						sRels.Replace( _T("</Relationships>"), sAdditionalContent + _T("</Relationships>") );
						XmlUtils::CXmlWriter oXmlRelsWriter;
						oXmlRelsWriter.SetXmlString( sHeader + sRels );
						oXmlRelsWriter.SaveToFile( sPath + _T("\\_rels\\.rels"), TRUE );
					}
				}
				else
					return;
				sSummaryPath = _T("\\docProps\\core.xml");
			}

			//заменяем существующие свойства
			CString sResult = sSummary;
			if( TRUE == oXmlReader.OpenFromXmlString( sSummary ) )
				if( TRUE == oXmlReader.ReadRootNode( _T("cp:coreProperties") ) )
				{
					sResult = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\">");

					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sProppertyName = oXmlReader.ReadNodeName( i );
						CString sProppertyValue = oXmlReader.ReadNodeText( i );
						CString sProppertyXml= oXmlReader.ReadNodeXml( i );
						if( _T("dc:title") == sProppertyName )
						{
							if( _T("") == sTitle )
								sResult.Append( sProppertyXml );
						}
						else if( _T("dc:creator") == sProppertyName )
						{
							if( _T("") == sAuthor )
								sResult.Append( sProppertyXml );
						}
						else if( _T("keywords") == sProppertyName || _T("cp:keywords") == sProppertyName )
						{
							if( _T("") == sKeywords )
								sResult.Append( sProppertyXml );
						}
						else if( _T("dc:subject") == sProppertyName )
						{
							if( _T("") == sSubject )
								sResult.Append( sProppertyXml );
						}
						else if( _T("revision") == sProppertyName || _T("cp:revision") == sProppertyName )
						{
							if( _T("") == sRevisionNumber )
								sResult.Append( sProppertyXml );
						}
						else if( _T("dc:description") == sProppertyName )
						{
							if( _T("") == sComments )
								sResult.Append( sProppertyXml );
						}
						else
							sResult.Append( sProppertyXml );
					}
					sResult.Append( _T("</cp:coreProperties>") );
				}
			CString sAdditional;
			if( _T("") != sTitle )
				sAdditional.Append( _T("<dc:title>") + Utils::PrepareToXML( sTitle ) + _T("</dc:title>") );
			if( _T("") != sAuthor )
				sAdditional.Append( _T("<dc:creator>") + Utils::PrepareToXML( sAuthor ) + _T("</dc:creator>") );
			if( _T("") != sKeywords )
				sAdditional.Append( _T("<cp:keywords>") + Utils::PrepareToXML( sKeywords ) + _T("</cp:keywords>") );
			if( _T("") != sSubject )
				sAdditional.Append( _T("<dc:subject>")+ Utils::PrepareToXML( sSubject ) + _T("</dc:subject>") );
			if( _T("") != sRevisionNumber )
				sAdditional.Append( _T("<cp:revision>") + Utils::PrepareToXML( sRevisionNumber ) + _T("</cp:revision>") );
			if( _T("") != sComments )
				sAdditional.Append( _T("<dc:description>") + Utils::PrepareToXML( sComments ) + _T("</dc:description>") );

			if( _T("") !=  sAdditional )
				sResult.Replace( _T("</cp:coreProperties>"), sAdditional + _T("</cp:coreProperties>") );
			XmlUtils::CXmlWriter oXmlWriter;
			oXmlWriter.SetXmlString( sResult );
			oXmlWriter.SaveToFile( sPath + sSummaryPath, TRUE);
			return;
		 }
private: void SaveSummaryInformation( CString sFilename, CString& sOptions )
		 {
			if( ::GetFileAttributes( sFilename ) == INVALID_FILE_ATTRIBUTES || true == sOptions.IsEmpty())
				return;
			HRESULT hRes = S_OK;
			//Открываем файл
			OfficeFile::IAVSOfficeFilePtr piOfficeFile;
			piOfficeFile.CreateInstance( __uuidof(OfficeFile::CAVSOfficeFile) );
			BSTR bstrFilename = sFilename.AllocSysString();
			hRes = piOfficeFile->OpenFile( bstrFilename, L"" );
			SysFreeString( bstrFilename );
			if( FAILED( hRes ) )
				return;
			
	
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromXmlString( sOptions ) )
				if( TRUE == oXmlReader.ReadNode( _T("FileInfo") ) )
				{
					oXmlReader.ReadNodeList( _T("*") );
					for( int i = 0; i < oXmlReader.GetLengthList(); i++ )
					{
						CString sProppertyName = oXmlReader.ReadNodeName(i);
						CString sProppertyValue = oXmlReader.ReadNodeText(i);
						BSTR bstrValue = sProppertyValue.AllocSysString();
						if( _T("Title") == sProppertyName )
							piOfficeFile->put_Title ( bstrValue );
						else if( _T("Author") == sProppertyName )
							piOfficeFile->put_Author( bstrValue );
						else if( _T("Keywords") == sProppertyName )
							piOfficeFile->put_Keywords( bstrValue );
						else if( _T("Subject") == sProppertyName )
							piOfficeFile->put_Subject( bstrValue );
						else if( _T("RevisionNumber") == sProppertyName )
							piOfficeFile->put_RevisionNumber( bstrValue );
						else if( _T("Comments") == sProppertyName )
							piOfficeFile->put_Comments( bstrValue );
						SysFreeString( bstrValue );
					}
				}
			//закрываем файл
			piOfficeFile->CloseFile();
		 }
 private: void WaitFileParsing( OfficeFile::IAVSOfficeFile* piOfficeFile )
		  {
			  //ждем пока идет рисование на файле
			  long nStatus = c_shOfficeFileIdle;
			  piOfficeFile->get_Status( &nStatus );
			  while( c_shOfficeFileParsing == nStatus )
			  {
				  Sleep( 200 );
				  piOfficeFile->get_Status( &nStatus );
			  }
		  }
private: bool IsEmptyPdfOptions( CString sOptions )
		 {
			 bool bResult = true;
			 sOptions = _T("<root>") + sOptions + _T("</root>");
			 XmlUtils::CXmlReader oXmlReader;
			 if( TRUE == oXmlReader.OpenFromXmlString( sOptions ) )
				 if( TRUE == oXmlReader.ReadNode( _T("PDFOptions") ) )
					 if( TRUE == oXmlReader.ReadNodeList( _T("*") ) )
					 {
						if( oXmlReader.GetLengthList() > 0 )
							bResult = false;
					 }
			 if( TRUE == oXmlReader.OpenFromXmlString( sOptions ) )
				 if( TRUE == oXmlReader.ReadNode( _T("Watermarks") ) )
					 if( TRUE == oXmlReader.ReadNodeList( _T("*") ) )
					 {
						if( oXmlReader.GetLengthList() > 0 )
							bResult = false;
					 }
			 return bResult;
		 }
private: OfficeFile::IAVSOfficeFile* GetOfficeFile(Source& oSource)
		 {
			 OfficeFile::IAVSOfficeFile* piOfficeFile = NULL;
			 if( NULL != m_piFilesCache )
			 {
				 m_piFilesCache->GetFile( oSource.nId, (IUnknown**)&piOfficeFile );
			 }
			 if( NULL == piOfficeFile )
			 {
				 CoCreateInstance( __uuidof( OfficeFile::CAVSOfficeFile ), NULL , CLSCTX_INPROC_SERVER, __uuidof(OfficeFile::IAVSOfficeFile), (void **)(&piOfficeFile)  );
				 if( NULL != piOfficeFile )
				 {
					 CString sOptions = _T("<Options>");
					 sOptions.Append( oSource.sOptions );
					 sOptions.Append( _T("</Options>") );
					 // открытие зажмет файл на время конвертации
					 BSTR bstrSourceFilename = oSource.sFilename.AllocSysString();
					 BSTR bstrOptions = sOptions.AllocSysString();
					 HRESULT hResOpen = piOfficeFile->OpenFile( bstrSourceFilename, bstrOptions );
					 SysFreeString( bstrSourceFilename );
					 SysFreeString( bstrOptions );
					 if( FAILED(hResOpen) )
					 {
						 RELEASEINTERFACE( piOfficeFile )
							 throw hResOpen;
					 }
					 if( "" != m_sTempDirectory )
					 {
						 BSTR bstrTempDirectory = m_sTempDirectory.AllocSysString();
						 piOfficeFile->put_TempDirectory( bstrTempDirectory );
						 SysFreeString( bstrTempDirectory );
					 }
					 piOfficeFile->StartParse();
					 m_piFilesCache->AddFile(oSource.nId, piOfficeFile);
				 }
			 }
			 if(NULL != piOfficeFile)
				WaitFileParsing( piOfficeFile );
			 return piOfficeFile;
		 }
//----------------------------- Private Members -------------------------------
private: IAVSOfficeFilesCache* m_piFilesCache;
//private: INT32 m_nOperationCompleteCurFile;
//private: INT32 m_nOperationAllCurFile;
private: int m_nCurFileID;
private: double m_dCurOperationProcess;
private: CString m_sTempDirectory;
private: OfficeFile::IAVSOfficeFormatCheckerPtr m_piFormatChecker;
//private: COfficeFileConverterEvents<OfficeFile::_IAVSOfficeFileEvents,DocumentConverter>* m_poEventReciver;
private: long m_nLastProgress;
//текущий прогресс находится в диапазоне [m_dMinProgress, m_dMaxProgress]
//вычисляем по формуле m_dMinProgress + (m_dMaxProgress - m_dMinProgress) * m_nLastProgress / c_nMaxProgressPercent
private: double m_dMinProgress;
private: double m_dMaxProgress;
};
