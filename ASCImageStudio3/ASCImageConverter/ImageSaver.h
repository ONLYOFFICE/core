#pragma once
#include "xmlutils.h"
#include "atldefine.h"
#include "BaseThread.h"
#include "../Common/ImageConverterDefines.h"

#include "../Common/ImageFileFormatChecker.h"

#include "AtlDefine.h"
#include "MediaFormatDefine.h"
#include "StringUtils.h"
#include "EventsHandler.h"
#include "TransformConstants.h"
#include "../../Common/OfficeFileErrorDescription.h"

struct Source{
	CString sFilename;
	CString sTransforms;
	int nPage;
	Source()
	{
		nPage = -1;
	}
};
struct CErrorItem{
	long nID;
	CString sFilename;
	int nSourcesComplete;
	int nSourcesAll;
	CErrorItem()
	{
		Clear();
	}
	void Clear()
	{
		nID = NO_ERROR;
		sFilename = _T("");
		nSourcesComplete = 0;
		nSourcesAll = 0;
	}
};

class ConvertItem{
public:	CAtlArray<Source> aSources;
public:	CAtlArray<CString> aDestinations;

public: ConvertItem()
		{
		}
public: ConvertItem( const ConvertItem& oItem )
		{
			aSources.RemoveAll();
			aSources.Append( oItem.aSources );
			aDestinations.RemoveAll();
			aDestinations.Append( oItem.aDestinations );
		}
public: ConvertItem& operator=( const ConvertItem& oItem )
		{
			aSources.RemoveAll();
			aSources.Append( oItem.aSources );
			aDestinations.RemoveAll();
			aDestinations.Append( oItem.aDestinations );
			return (*this);
		}

public: BOOL IsValid()
		{
			BOOL bResult = TRUE;
			if( aDestinations.GetCount() == 0 )
				bResult = FALSE;
			if( aDestinations.GetCount() == 0  )
				bResult = FALSE;
			return bResult;
		}
};

[ event_source(native) ]
class ImageSaver : public CBaseThread
{
//----------------------------- Public Members --------------------------------
public: __event void OnError( LONG nValue );
public: __event void OnError2( CErrorItem& oError );
public: __event void OnComplete();
public: __event bool OnGetTask( ConvertItem& oNewTask );
public: __event void OnProgress();
public: __event void OnCompleteFile( CString& sFile );
//----------------------------- Public Methods --------------------------------
public: ImageSaver(): CBaseThread(0)
		{
			m_piLogo = NULL;
			m_bEnableWM = false;
			m_nLeftIndent = 0;
			m_nRightIndent = 0;
			m_nTopIndent = 0;
			m_nBottomIndent = 0;
			m_dCurProcess = 0;
			m_old_photo_masks_folder.Empty();
			m_nAnimationWidth = -1;
			m_nAnimationHeight = -1;
			m_piImageVideoFile = NULL;
			CoCreateInstance(ImageVideoFile::CLSID_CImageVideoFile3, NULL, CLSCTX_INPROC, ImageVideoFile::IID_IImageVideoFile4, (void**)(&m_piImageVideoFile));
			if( NULL != m_piImageVideoFile )
			{
				VARIANT vtResizeType;
				vtResizeType.vt = VT_I4;
				vtResizeType.lVal = 0; //не применять resize на открытие
				m_piImageVideoFile->SetAdditionalParam( _T("resizetype"), vtResizeType );
				VARIANT vtWithAlpha;
				vtWithAlpha.vt = VT_BOOL;
				vtWithAlpha.lVal = VARIANT_TRUE; //оставлять прозрачность
				m_piImageVideoFile->SetAdditionalParam( _T("drawwithalpha"), vtWithAlpha );
			}
			m_sCurApngFile = _T("");
			m_nLastFrame = -1;
			m_piCache = NULL;
		}
public: ~ImageSaver()
		{
			Stop();
			RELEASEINTERFACE( m_piLogo );
			if( NULL != m_piImageVideoFile )
				m_piImageVideoFile->CloseFile();
			RELEASEINTERFACE(m_piImageVideoFile);
			RELEASEINTERFACE( m_piCache );
		}
public: void OnProgressPdf( LONG nPercent )
		{
			m_dCurProcess = 0.9 + 0.1 * nPercent / 1000000;
			OnProgress( );
		}
public: void SetThreadPriority(INT32 nPriority)
{
	if( NULL != m_hThread )
		::SetThreadPriority( m_hThread, nPriority );
}
public: void SetLogo(MediaCore::IAVSUncompressedVideoFrame* piLogo) //Todo сделать через addRef Release
{
	RELEASEINTERFACE(m_piLogo);
	if( NULL != piLogo )
	{
		piLogo->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_piLogo );
	}
}
public: void SetLogoParam(INT nLogoParam)
{
	m_nLogoParam = nLogoParam;
}
public: void SetWM(bool bEnableWM)
{
	m_bEnableWM = bEnableWM;
}
public: void SetCache( ImageStudio::IAVSImageCache* piCache )
{
	RELEASEINTERFACE( m_piCache );
	piCache->QueryInterface( __uuidof(ImageStudio::IAVSImageCache), (void**)&m_piCache );

	//перегрузим мету дату
	//m_piCache->

}
public: void SetLeftIndent(INT nLeft)
{
	m_nLeftIndent = nLeft;
}
public: void SetRightIndent(INT nRight)
{
	m_nRightIndent = nRight;
}
public: void SetTopIndent(INT nTop)
{
	m_nTopIndent = nTop;
}
public: void SetBottomIndent(INT nBottom)
{
	m_nBottomIndent = nBottom;
}
public: double GetProcess()
		{
			return m_dCurProcess;
		}
private: void LoadMetaDataFromImageFile(BSTR bsFilePath, CString& sMetaDataXML)
		 {
			sMetaDataXML = _T("");
			ImageFile::IImageMetaData *pImageMetaData;
			::CoCreateInstance( ImageFile::CLSID_ImageFile3 , NULL, CLSCTX_INPROC, ImageFile::IID_IImageMetaData, (void**)&pImageMetaData );

			if ( !pImageMetaData )
				return;

			pImageMetaData->MetaDataFromFile(bsFilePath);
	
			::SysFreeString(bsFilePath);

			BSTR bsMetaDataXML = NULL;
			pImageMetaData->GetMetaData(&bsMetaDataXML);

			if (bsMetaDataXML)
			{
				CW2T convert(bsMetaDataXML);
				sMetaDataXML = convert;

				SysFreeString(bsMetaDataXML);
			}


			RELEASEINTERFACE(pImageMetaData);
		 }
//----------------------------- Private Methods --------------------------------
private: DWORD ThreadProc()
		{
			CoInitialize( NULL );
			if( NULL == m_piImageVideoFile )
			{
				OnError( IMAGECONVERTER_UNEXPECTED_ERROR );
				return 1;
			}
			HRESULT hRes = S_OK;

			ConvertItem oNewItem;
			while( true == OnGetTask( oNewItem ) )
			{
				hRes = S_OK;
				m_dCurProcess = 0;
				CAtlArray<CString> m_aTempPdfDirectories;//массив адресов темповых папок
				CAtlArray<int> m_aTempPdfIndexes; //массив индексов destination соответствующих темповым папкам
				int nSourcesCount = oNewItem.aSources.GetCount();
				m_sAnimationBackground = _T("16777215");//белый
				
				ImageFile::IImageFile3Ptr piImageFile; //для tiff
				piImageFile.CreateInstance(__uuidof(ImageFile::ImageFile3));
				
				if ( NULL == piImageFile)
				{
					OnError( IMAGECONVERTER_ERROR_IMAGEFILE );
					return 1;
				}

				int nProgressCount = ( 2 + oNewItem.aDestinations.GetCount() ) * nSourcesCount ;//число вызовов OnProgress
				double nProgressStep = 0.9 / nProgressCount;//шаг прогресса 0-90 картинки 90 - 100 сохранение pdf
				int k = 0;
				CErrorItem oErrorItem;
				oErrorItem.nSourcesAll = nSourcesCount;
				oErrorItem.nSourcesComplete = k;
				int nFirstRealSource = -1; //первый source который мы начали выполнять
				for( k = 0; k < nSourcesCount; k++ )
				{
					oErrorItem.nSourcesComplete = k;
					Source oCurrentSource = oNewItem.aSources[k];

					//проверяем наличие файла
					if( ::GetFileAttributes( oCurrentSource.sFilename ) == DWORD(-1) )
					{
						hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_FILE_NOT_EXIST;
						oErrorItem.sFilename = oCurrentSource.sFilename;
						OnError2( oErrorItem );
						continue;
					}
					else
					{ //проверяем доступен ли файл для чтения
						HANDLE hFile = CreateFile((LPCWSTR)oCurrentSource.sFilename,// file to open
							GENERIC_READ,          // open for reading
						   FILE_SHARE_READ,       // share for reading
						   NULL,                  // default security
						   OPEN_EXISTING,         // existing file only
						   FILE_ATTRIBUTE_NORMAL, // normal file
						   NULL);                 // no attr. template
						if( INVALID_HANDLE_VALUE == hFile )
						{
							hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_FILE_ACCESS_READ;
							oErrorItem.sFilename = oCurrentSource.sFilename;
							OnError2( oErrorItem );
							//переходим к слующей задаче( выходим из source )
							continue;
						}
						RELEASEHANDLE( hFile );
					}
					if( -1 == nFirstRealSource )
						nFirstRealSource = k; //первый существующий файл

					bool bMultiSource = false;
					if( nSourcesCount - nFirstRealSource > 1 )
						bMultiSource = true;

					CheckSuspend();
					if( FALSE == m_bRunThread && false == bMultiSource )//для MultiSource всегда надо дописывать последний фрейм
						break;

					IUnknown* piImageLoad = NULL;
					VARIANT vImage;
					bool bSaveAsFromat = false;
					for( int i = 0; i < oNewItem.aDestinations.GetCount(); i++ )
						if( -1 != oNewItem.aDestinations[0].Find(_T("SaveAsFormat")) )
						{
							bSaveAsFromat = true;
							break;
						}
					ImageStudio::IImageTransforms* piOldTransform = NULL;//Для того чтобы между load и применением эффектов не было вызовов SetSource (ошибки при resize svg)
					
					CString strMetaDataXML;
					if( NULL != m_piCache && true == bSaveAsFromat )
					{
						BSTR bstrFilename = oCurrentSource.sFilename.AllocSysString();
						m_piCache->raw_LoadImageW( bstrFilename, -1, -1, &piImageLoad );
							
						//ImageStudio::IAVSImageCache2 *piCache2=NULL;
						//m_piCache->QueryInterface( __uuidof(ImageStudio::IAVSImageCache2), (void**)&piCache2);
						//if (piCache2)
						//{
						//	piCache2->GetMetadataXML(bstrFilename,&bstrMetaData);
						//	RELEASEINTERFACE(piCache2);
						//}else
						{
							LoadMetaDataFromImageFile(bstrFilename,strMetaDataXML);
						}
						SysFreeString( bstrFilename );
					}
					else
					{
						CImageFileFormatChecker checker; 
						if( oCurrentSource.sFilename == m_sCurApngFile || checker.isPngFile(oCurrentSource.sFilename))
						{// Png открываем с помощью ImageVideoFile
							HRESULT hRes = S_OK;
							//если файл открыт, то не переоткрываем
							if( oCurrentSource.sFilename != m_sCurApngFile )
							{
								m_piImageVideoFile->CloseFile();
								BSTR bstrFilePath  = oCurrentSource.sFilename.AllocSysString();
								hRes = m_piImageVideoFile->OpenFile( bstrFilePath );

								SysFreeString( bstrFilePath );

								m_sCurApngFile = oCurrentSource.sFilename;
								m_nLastFrame = -1;
							}
							if( true == SUCCEEDED( hRes ) )
							{
								//если запрошен не следующий фрейм, то делаем seek
								if( m_nLastFrame + 1 != oCurrentSource.nPage )
								{
									VARIANT vtSetAddPar;
									vtSetAddPar.vt = VT_I4;
									vtSetAddPar.lVal = oCurrentSource.nPage;
									hRes = m_piImageVideoFile->SetAdditionalParam( _T("seektoframe"), vtSetAddPar );

									m_nLastFrame = oCurrentSource.nPage - 1;//-1 потому что далее будет m_nLastFrame++;
								}
								if( true == SUCCEEDED( hRes ) )
								{
									//читаем frame
									hRes = m_piImageVideoFile->ReadVideo( &piImageLoad );
									if( true == SUCCEEDED( hRes ) )
										m_nLastFrame++;
								}
							}
						}
						else
						{
							BSTR bstrFilename = oCurrentSource.sFilename.AllocSysString();
							LoadMetaDataFromImageFile(bstrFilename,strMetaDataXML);
							SysFreeString(bstrFilename);

							//imageStudio открывает и сохраняет файл
							CString sTransformLoad = _T("<ImageFile-LoadImage sourcepath=\"")+ ModifyToValidXml( oCurrentSource.sFilename ) +_T("\" ");
							if( -1 != oCurrentSource.nPage )
								sTransformLoad.AppendFormat( _T(" frame=\"%d\""), oCurrentSource.nPage );
							sTransformLoad.Append( _T(" />") );

							piOldTransform = ApplyTransform2( &piImageLoad, NULL, sTransformLoad );
						}
					}

					if( NULL == piImageLoad )
					{
						BSTR bstrFilename = oCurrentSource.sFilename.AllocSysString();
						HRESULT hRes = piImageFile->LoadImage2( bstrFilename, &piImageLoad );
						SysFreeString( bstrFilename );
						if( NULL == piImageLoad )
						{
							RELEASEINTERFACE(piOldTransform);
							hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_IMAGEFILE;
							oErrorItem.sFilename = oCurrentSource.sFilename;
							OnError2( oErrorItem );
							continue;
						}
					}
					m_dCurProcess += nProgressStep;
					OnProgress( );
					CheckSuspend();
					if( FALSE == m_bRunThread && false == bMultiSource )
					{
						RELEASEINTERFACE(piOldTransform);
						RELEASEINTERFACE( piImageLoad );
						break;
					}
					//Применяем эффекты
					if( false == oCurrentSource.sTransforms.IsEmpty() )
					{
						CString sFullTransform =_T("<transforms>");
						sFullTransform.Append( oCurrentSource.sTransforms );
						sFullTransform.Append(_T("</transforms>"));

						ApplyTransform( &piImageLoad, NULL, sFullTransform, piOldTransform );
						if( NULL == piImageLoad)
						{
							RELEASEINTERFACE(piOldTransform);
							hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_IMAGESTUDIO;
							oErrorItem.sFilename = oCurrentSource.sFilename;
							OnError2( oErrorItem );
							continue;
						}
					}
					RELEASEINTERFACE(piOldTransform);

					SolveAlpha( piImageLoad );
					m_dCurProcess += nProgressStep;
					OnProgress( );
					CheckSuspend();
					if( FALSE == m_bRunThread && false == bMultiSource )
					{
						RELEASEINTERFACE( piImageLoad );
						break;
					}

					//рисуем watermark и сохраняем все. Если jpeg то применяем эффект quality
					XmlUtils::CXmlReader oXmlReader;

					bool bBreakAfterDestination = false;
					for( int i = 0 ; i < oNewItem.aDestinations.GetCount(); i++ )
					{
						//if( nFirstRealSource == k || ( -1 == oNewItem.aDestinations[i].Find(_T("SaveAsTiff")) &&
						//	-1 == oNewItem.aDestinations[i].Find(_T("SaveAsGif"))  ))
						if( nFirstRealSource == k )
						{//для tiff и gif проверяем при записи первого
							//все остальные каждый раз
							CString sDestFilename;
							XmlUtils::CXmlReader oXmlReader;
							if( TRUE == oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] ) )
								sDestFilename = oXmlReader.ReadNodeAttribute( _T("destinationpath") );
							//проверяем доступен ли файл для записи, если он существует
							if( ::GetFileAttributes( sDestFilename ) != DWORD(-1) )
							{
								HANDLE hFile = CreateFile((LPCWSTR)sDestFilename,// file to open
									GENERIC_WRITE,          // open for reading
								   FILE_SHARE_READ,       // share for reading
								   NULL,                  // default security
								   OPEN_EXISTING,         // existing file only
								   FILE_ATTRIBUTE_NORMAL, // normal file
								   NULL);                 // no attr. template
								if( INVALID_HANDLE_VALUE == hFile )
								{
									hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_FILE_ACCESS_WRITE;
									oErrorItem.sFilename = sDestFilename;
									OnError2( oErrorItem );
									//переходим к слующей задаче( выходим из destinations потом из source )
									bBreakAfterDestination = true;//source
									break;
								}
								RELEASEHANDLE( hFile );
							}
						}
						CString sDestPath = _T("");
						//вынимаем имя файла из xml
						XmlUtils::CXmlReader oXmlReader;
						if( TRUE == oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] ) )
							sDestPath = oXmlReader.ReadNodeAttribute( _T("destinationpath") );

						CString sFullTransform;
						if( -1 != oNewItem.aDestinations[i].Find(_T("SaveAsJpeg")) )
						{//для Jpeg отдельно ставим quality
						//инфу
							oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] );
							CString sQuality = oXmlReader.ReadNodeAttribute( _T("quality"), _T("") );
							if( _T("") != sQuality )
							{
								sFullTransform.AppendFormat( _T("<ImageTransform-EffectJpegCompression quality=\"%s\"/>"), ModifyToValidXml( sQuality));
							}

							sFullTransform.Append( oNewItem.aDestinations[i] );
							if (_T("") != strMetaDataXML)
							{
								sFullTransform = sFullTransform.Left(sFullTransform.GetLength()-2)+_T(">");
								sFullTransform = sFullTransform + strMetaDataXML;
								sFullTransform.Append(_T("</ImageFile-SaveAsJpeg>"));
							}
						}
						else if( -1 != oNewItem.aDestinations[i].Find(_T("SaveAsPdf")) )
						{//Pdf сначала конвертируем в jpg
							//читаем свойства
							XmlUtils::CXmlReader oXmlReader;
							oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] );
							CString sQuality = oXmlReader.ReadNodeAttribute( _T("jpegquality"), _T("") );
							if( _T("") != sQuality )
								sFullTransform.AppendFormat( _T("<ImageTransform-EffectJpegCompression quality=\"%s\"/>"), ModifyToValidXml( sQuality));
							CString sFormat = oXmlReader.ReadNodeAttribute( _T("jpegformat"), _T("") );
							if( _T("") == sFormat )
								sFormat = _T("8888");
							CString sDestPath = oXmlReader.ReadNodeAttribute( _T("destinationpath"), _T("") );
							if( _T("") == sDestPath )
								continue;
							CString sTempDirectory;
							//создаем темповую папку в той же папке где и выходной файл
							if( nFirstRealSource == k )
							{
								int nIndex = sDestPath.ReverseFind( '\\' );
								sDestPath = sDestPath.Left( nIndex );
								TCHAR szTempName[1024];  
								//получаем уникальное имя для темповой папки
								UINT uRetVal = ::GetTempFileName(sDestPath, NULL, 0, szTempName);
								if (0 == uRetVal)
								{
									hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_FILE_SAVE;
									//вынимаем имя файла из xml
									XmlUtils::CXmlReader oXmlReader;
									if( TRUE == oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] ) )
										oErrorItem.sFilename = oXmlReader.ReadNodeAttribute( _T("destinationpath") );
									OnError2( oErrorItem );
									break;
								}
								sTempDirectory = CString( szTempName );
								DeleteFile( sTempDirectory );
								CreateDirectory( sTempDirectory ,NULL) ;
								m_aTempPdfDirectories.Add( sTempDirectory );
								m_aTempPdfIndexes.Add( i );
							}
							else if( m_aTempPdfDirectories.GetCount() > 0 )
								sTempDirectory = m_aTempPdfDirectories[ m_aTempPdfDirectories.GetCount() - 1 ];
							else
								continue;
							CString sTempImageName = sTempDirectory;
							sTempImageName.AppendFormat( _T("\\Image%d.jpg"), k );
							sFullTransform.Append( _T("<ImageFile-SaveAsJpeg format=\"") + sFormat + _T("\" destinationpath=\"") + ModifyToValidXml( sTempImageName ) + _T("\" />") );
							//меняем имя файла в xml для PdfEditor
							if( TRUE == oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] ) )
							{
								CString sDestFilename = oXmlReader.ReadNodeAttribute( _T("destinationpath") );
								if( TRUE == oXmlReader.ReadNode( _T("Pages") ) )
								{
									CString sPages;
									oXmlReader.ReadNodeList( _T("Page") );
									for( int j = 0; j < oXmlReader.GetLengthList(); j++ )
									{
										XML::IXMLDOMNodePtr oSubNode;
										oXmlReader.GetNode( j, oSubNode );
										CString sPage = oXmlReader.ReadNodeXml(j);
										XmlUtils::CXmlReader oXmlSubReader;
										if( TRUE == oXmlSubReader.OpenFromXmlNode( oSubNode ) )
											if( TRUE == oXmlSubReader.ReadNode( _T("*") ) )
											{
												CString sPath = oXmlSubReader.ReadNodeAttribute( _T("path"), _T("") );
												int nPage = Strings::ToInteger( oXmlSubReader.ReadNodeAttribute( _T("page"), _T("-1") ) );
												if( sPath == oCurrentSource.sFilename && ( -1 == oCurrentSource.nPage ||  nPage == oCurrentSource.nPage ) )
												{
													if( 0 == sPage.Replace( ModifyToValidXml( oCurrentSource.sFilename ), ModifyToValidXml( sTempImageName ) ) )
														sPage.Replace( ModifyToValidXmlAttr( oCurrentSource.sFilename ), ModifyToValidXml( sTempImageName ) );
												}
											}
										sPages.Append( sPage );
									}
									if( false == sPages.IsEmpty() )
										oNewItem.aDestinations.SetAt(i, _T("<ImageFile-SaveAsPdf destinationpath=\"") + ModifyToValidXml( sDestFilename ) + _T("\"><Pages>") + sPages + _T("</Pages></ImageFile-SaveAsPdf>") );

								}
							}
						}
						else if( -1 != oNewItem.aDestinations[i].Find(_T("SaveAsFormat")) )
							sFullTransform = GetFormatTransform( oCurrentSource.sFilename ,piImageLoad, oNewItem.aDestinations[i] );
						else if( (-1 == oNewItem.aDestinations[i].Find(_T("SaveAsTiff")) && -1 == oNewItem.aDestinations[i].Find(_T("SaveAsGif")) && -1 == oNewItem.aDestinations[i].Find(_T("SaveAsPng"))) || false == bMultiSource )
						{
							sFullTransform.Append( oNewItem.aDestinations[i] );
							//if (-1 != oNewItem.aDestinations[i].Find(_T("SaveAsTiff")) && _T("") != strMetaDataXML)
							//{
								//sFullTransform = sFullTransform.Left(sFullTransform.GetLength()-2)+_T(">");
								//sFullTransform = sFullTransform + strMetaDataXML;
								//sFullTransform.Append(_T("</ImageFile-SaveAsTiff>"));
							//}		
						}

						if( false == sFullTransform.IsEmpty() )
						{
							sFullTransform =_T("<transforms>") + sFullTransform+ _T("</transforms>");

							//накладываем регистрацию
							AddRegistration( &piImageLoad );
							if( NULL == piImageLoad )
							{
								hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_IMAGESTUDIO;
								//вынимаем имя файла из xml
								XmlUtils::CXmlReader oXmlReader;
								if( TRUE == oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] ) )
									oErrorItem.sFilename = oXmlReader.ReadNodeAttribute( _T("destinationpath") );
								OnError2( oErrorItem );
								continue;
							}

							//сохраняем файл
							ApplyTransform( &piImageLoad, NULL, sFullTransform );
							if( NULL == piImageLoad )
							{
								hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_FILE_SAVE;
								oErrorItem.sFilename = sDestPath;
								OnError2( oErrorItem );
								continue;
							}
						}
						bool bLast = false;	
						//значение m_bRunThread может поменяться во время выполнения if
						bool bRunThread = m_bRunThread;
						if( true == bMultiSource && -1 != oNewItem.aDestinations[i].Find(_T("SaveAsTiff")) )
						{
							CString sFileName;
							//выдергиваем path
							XmlUtils::CXmlReader oXmlReader;
							if( TRUE == oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] ) )
								sFileName = oXmlReader.ReadNodeAttribute( _T("destinationpath"), _T("") );
							if( _T("") == sFileName )
								continue;

							//накладываем регистрацию
							AddRegistration( &piImageLoad );
							if( NULL == piImageLoad )
							{
								hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_IMAGESTUDIO;
								//вынимаем имя файла из xml
								XmlUtils::CXmlReader oXmlReader;
								if( TRUE == oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] ) )
									oErrorItem.sFilename = oXmlReader.ReadNodeAttribute( _T("destinationpath") );
								OnError2( oErrorItem );
								continue;
							}
							VARIANT_BOOL vbSUCCESS = VARIANT_TRUE;
							HRESULT hRes = S_OK;
							//Tiff сохраняем ImageFile3
							//ImageFile::IImageMetaData* piMetaData = NULL;
							//if( SUCCEEDED( piImageFile->QueryInterface( __uuidof( ImageFile::IImageMetaData ), (void**)&piMetaData ) ) )
							//{
							//	piMetaData->SetMetaData(strMetaDataXML.AllocSysString());
							//}
							//RELEASEINTERFACE( piMetaData );
							if( nFirstRealSource == k )
							{
								if( TRUE == oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] ) )
								{
									piImageFile->TiffCompression = Strings::ToInteger( oXmlReader.ReadNodeAttribute( _T("compression"), _T("0") ) );
								}
								if( true == bMultiSource && true == bRunThread )
									piImageFile->TiffMultipaged = VARIANT_TRUE;
								//сохраняем файл и первый фрейм
								BSTR bstrFilename = sFileName.AllocSysString();
								hRes = piImageFile->raw_SaveImage2((IUnknown**)&piImageLoad, IMAGEFORMAT_TIF, bstrFilename, &vbSUCCESS);

								SysFreeString( bstrFilename );
								if( nSourcesCount - 1 == k || false == bRunThread )
									bLast = true;
							}
							else if( nSourcesCount - 1 != k && true == bRunThread )
								hRes = piImageFile->raw_SaveFrame2((IUnknown**)&piImageLoad, VARIANT_FALSE, &vbSUCCESS);//сохраняем промежуточный фрейм
							else
							{
								bLast = true;
								hRes = piImageFile->raw_SaveFrame2((IUnknown**)&piImageLoad, VARIANT_TRUE, &vbSUCCESS);//сохраняем последний фрейм
							}
							if( VARIANT_TRUE != vbSUCCESS )
							{
								hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_TIFF_SAVE;
								oErrorItem.sFilename = sFileName;
								OnError2( oErrorItem );
								//переходим к слующей задаче( выходим из destinations потом из source )
								bBreakAfterDestination = true;//source
								break; //destinations
							}
							if( true == bLast )
								OnCompleteFile( sFileName );
						}
						else if( true == bMultiSource && ( -1 != oNewItem.aDestinations[i].Find(_T("SaveAsGif")) || -1 != oNewItem.aDestinations[i].Find(_T("SaveAsPng")) ) )
						{
							CString sFileName;
							//выдергиваем path
							XmlUtils::CXmlReader oXmlReader;
							if( TRUE == oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] ) )
								sFileName = oXmlReader.ReadNodeAttribute( _T("destinationpath"), _T("") );
							if( _T("") == sFileName )
								continue;
							VARIANT_BOOL vbSUCCESS = VARIANT_TRUE;
							//Giff сохраняем ImageFile3
							if( nFirstRealSource == k )
							{
								if( TRUE == oXmlReader.OpenFromXmlString( oNewItem.aDestinations[i] ) )
								{
									ImageFile::IImageMetaData* piMetaData = NULL;
									if( SUCCEEDED( piImageFile->QueryInterface( __uuidof( ImageFile::IImageMetaData ), (void**)&piMetaData ) ) )
									{
										CString sDuration = oXmlReader.ReadNodeAttribute( _T("duration"), _T("1000") );
										if( _T("") != sDuration )
										{
											double dDuration = Strings::ToDouble( sDuration );
											VARIANT vtDuration;
											vtDuration.vt = VT_R8;
											vtDuration.dblVal = dDuration;
											piMetaData->raw_SetAdditionalParam( L"AnimationDuration", vtDuration );
										}
										CString sWidth = oXmlReader.ReadNodeAttribute( _T("animationwidth") );
										if( _T("") != sWidth )
										{
											long nAnimationWidth = Strings::ToInteger( sWidth );
											m_nAnimationWidth = nAnimationWidth;

											VARIANT vtAnimationWidth;
											vtAnimationWidth.vt = VT_I4;
											vtAnimationWidth.lVal = nAnimationWidth;
											piMetaData->raw_SetAdditionalParam( L"AnimationWidth", vtAnimationWidth );
										}
										CString sHeight = oXmlReader.ReadNodeAttribute( _T("animationheight") );
										if( _T("") != sHeight )
										{
											long nAnimationHeight = Strings::ToInteger( sHeight );
											m_nAnimationHeight = nAnimationHeight;

											VARIANT vtAnimationHeight;
											vtAnimationHeight.vt = VT_I4;
											vtAnimationHeight.lVal = nAnimationHeight;
											piMetaData->raw_SetAdditionalParam( L"AnimationHeight", vtAnimationHeight );
										}
										CString sAnimationBackground = oXmlReader.ReadNodeAttribute( _T("animationbackground") );
										if( _T("") != sAnimationBackground )
											m_sAnimationBackground = sAnimationBackground;

									}
									RELEASEINTERFACE( piMetaData );
								}
								if( true == bMultiSource && true == bRunThread )
									piImageFile->TiffMultipaged = VARIANT_TRUE;

								long nOutputFormat = IMAGEFORMAT_GIF;
								if( -1 != oNewItem.aDestinations[i].Find(_T("SaveAsPng")) )
									nOutputFormat = IMAGEFORMAT_PNG;

								ImageFile::IImageMetaData* piMetaData = NULL;
								if( SUCCEEDED( piImageFile->QueryInterface( __uuidof( ImageFile::IImageMetaData ), (void**)&piMetaData ) ) )
								{
									//если не заданы animationwidth или animationheight берем как у первого фрейма
									if( -1 == m_nAnimationWidth || -1 == m_nAnimationHeight )
									{
										MediaCore::IAVSUncompressedVideoFrame* piFrame = NULL;
										if( SUCCEEDED( piImageLoad->QueryInterface( __uuidof(MediaCore::IAVSUncompressedVideoFrame), (void**)&piFrame ) ) )
										{
											m_nAnimationWidth = piFrame->Width;
											VARIANT vtAnimationWidth;
											vtAnimationWidth.vt = VT_I4;
											vtAnimationWidth.lVal = m_nAnimationWidth;
											piMetaData->raw_SetAdditionalParam( L"AnimationWidth", vtAnimationWidth );

											m_nAnimationHeight = piFrame->Height;
											VARIANT vtAnimationHeight;
											vtAnimationHeight.vt = VT_I4;
											vtAnimationHeight.lVal = m_nAnimationHeight;
											piMetaData->raw_SetAdditionalParam( L"AnimationHeight", vtAnimationHeight );

											RELEASEINTERFACE( piFrame );
										}
									}
									//piMetaData->SetMetaData(strMetaDataXML.AllocSysString());
									RELEASEINTERFACE( piMetaData );
								}
								//нормализуем: shrink, если размеры больше чем m_nAnimationWidth, m_nAnimationHeight; фон белый
								IUnknown* piNormalizedImage = NULL;
								piNormalizedImage = NormalizeImage( piImageLoad );
								RELEASEINTERFACE( piImageLoad );

								//накладываем регистрацию
								AddRegistration( &piNormalizedImage );
								if( NULL == piNormalizedImage )
								{
									hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_IMAGESTUDIO;
									oErrorItem.sFilename = sDestPath;
									OnError2( oErrorItem );
								}

								if( NULL != piNormalizedImage )
								{
									//сохраняем файл и первый фрейм
									BSTR bstrFilename = sFileName.AllocSysString();
									hRes = piImageFile->raw_SaveImage2((IUnknown**)&piNormalizedImage, nOutputFormat, bstrFilename, &vbSUCCESS);
									SysFreeString( bstrFilename );
								}
								else
									vbSUCCESS = VARIANT_FALSE;

								RELEASEINTERFACE( piNormalizedImage );
								if( nSourcesCount - 1 == k || false == bRunThread )
									bLast = true;
							}
							else if( nSourcesCount - 1 != k && true == bRunThread )
							{
								IUnknown* piNormalizedImage = NULL;
								piNormalizedImage = NormalizeImage( piImageLoad );
								RELEASEINTERFACE( piImageLoad );

								//накладываем регистрацию
								AddRegistration( &piNormalizedImage );
								if( NULL == piNormalizedImage )
								{
									hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_IMAGESTUDIO;
									oErrorItem.sFilename = sDestPath;
									OnError2( oErrorItem );
								}

								if( NULL != piNormalizedImage )
									hRes = piImageFile->raw_SaveFrame2((IUnknown**)&piNormalizedImage, VARIANT_FALSE, &vbSUCCESS);//сохраняем промежуточный фрейм
								else
									vbSUCCESS = VARIANT_FALSE;
								RELEASEINTERFACE( piNormalizedImage );
							}
							else
							{
								bLast = true;
								IUnknown* piNormalizedImage = NULL;
								piNormalizedImage = NormalizeImage( piImageLoad );
								RELEASEINTERFACE( piImageLoad );

								//накладываем регистрацию
								AddRegistration( &piNormalizedImage );
								if( NULL == piNormalizedImage )
								{
									hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_IMAGESTUDIO;
									oErrorItem.sFilename = sDestPath;
									OnError2( oErrorItem );
								}

								if( NULL != piNormalizedImage )
									hRes = piImageFile->raw_SaveFrame2((IUnknown**)&piNormalizedImage, VARIANT_TRUE, &vbSUCCESS);//сохраняем последний фрейм
								else
									vbSUCCESS = VARIANT_FALSE;
								RELEASEINTERFACE( piNormalizedImage );
							}
							if( VARIANT_TRUE != vbSUCCESS )
							{
								hRes = oErrorItem.nID = IMAGECONVERTER_ERROR_FILE_SAVE;
								oErrorItem.sFilename = sFileName;
								OnError2( oErrorItem );
								//переходим к слующей задаче( выходим из destinations потом из source )
								bBreakAfterDestination = true;//source
								break; //destinations
							}
							if( true == bLast )
								OnCompleteFile( sFileName );
						}
						else if( -1 == oNewItem.aDestinations[i].Find(_T("SaveAsPdf")) )
							OnCompleteFile( sDestPath );
						RELEASEINTERFACE( piImageLoad );
						m_dCurProcess += nProgressStep;
						OnProgress( );
						CheckSuspend();
						if( FALSE == m_bRunThread && ( false == bMultiSource || true == bLast) )
						{
							bBreakAfterDestination = true;
							break;
						}
					}

					RELEASEINTERFACE( piImageLoad );

					if( true == bBreakAfterDestination )
						break;
				}
				//сохраняем Pdf
			if( SUCCEEDED(hRes) )
				for( int i = 0; i < m_aTempPdfDirectories.GetCount(); i++ )
				{
					CString sDestFilename;
					//сохраняем файл, если только поток не остановлен
					if( TRUE == m_bRunThread )
					{
						//читаем имя выходного файла, готовим xml PDFEditor
						int nDestIndex = m_aTempPdfIndexes[i];
						XmlUtils::CXmlReader oXmlReader;
						CString sPdfEditorXml = oNewItem.aDestinations[ nDestIndex ];
						CString sPages = oXmlReader.ReadNodeXml();
						if( TRUE == oXmlReader.OpenFromXmlString( sPdfEditorXml ) )
						{
							sDestFilename = oXmlReader.ReadNodeAttribute( _T("destinationpath") );
							if( TRUE == oXmlReader.ReadNode( _T("Pages") ) )
								sPages = oXmlReader.ReadNodeXml();
							if( "" != sPages )
								sPdfEditorXml = _T("<PDFEditor>") + sPages + _T("</PDFEditor>");
						}
						if( "" != sDestFilename && "" != sPdfEditorXml )
						{
							//сохраняем PDF
							OfficePDFFile::IPDFFilePtr piPdfFile = NULL;
							piPdfFile.CreateInstance( __uuidof( OfficePDFFile::CPDFFile ) );
							if( NULL != piPdfFile )
							{
								//цепляем события
								CEventPdf< OfficePDFFile::_IAVSOfficeFileTemplateEvents, ImageSaver >* oEventPdf = new CEventPdf< OfficePDFFile::_IAVSOfficeFileTemplateEvents, ImageSaver >(this);
								oEventPdf->AddRef();
								oEventPdf->Advise( piPdfFile );

								BSTR bstrFilename = sDestFilename.AllocSysString();
								BSTR bstrPdfEditor = sPdfEditorXml.AllocSysString();
								hRes = piPdfFile->raw_PDFFromXml( bstrFilename, bstrPdfEditor, L"" );
								SysFreeString( bstrFilename );
								SysFreeString( bstrPdfEditor );
								//отцепляем события
								oEventPdf->UnAdvise( piPdfFile );
								RELEASEINTERFACE(oEventPdf);
							}
							else
								hRes = IMAGECONVERTER_ERROR_PDFFILE_CONTROL;
						}
						else
							hRes = IMAGECONVERTER_ERROR_XML_FORMAT;
					}

					//удаляем темповую директорию
					CString sModifiedPath = m_aTempPdfDirectories[i];
					sModifiedPath.AppendChar( '\0' );
					SHFILEOPSTRUCT shfo={0};
					shfo.wFunc=FO_DELETE;
					shfo.pFrom = sModifiedPath;
					shfo.fFlags=FOF_NOCONFIRMATION | FOF_SILENT ;
					shfo.fAnyOperationsAborted=FALSE;
					shfo.hNameMappings=NULL;
					shfo.lpszProgressTitle=NULL;
					int nRemoveRes = SHFileOperation(&shfo);


					if( 0 != nRemoveRes )
					{
						oErrorItem.nID = IMAGECONVERTER_ERROR_REMOVE_TEMP_DIR;
						oErrorItem.sFilename = m_aTempPdfDirectories[i];
						OnError2( oErrorItem );
						break;
					}
					if( FAILED( hRes ) )
					{
						if( AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR == hRes )
							oErrorItem.nID = IMAGECONVERTER_ERROR_FILE_SAVE;
						else
							oErrorItem.nID = IMAGECONVERTER_ERROR_SAVE_PDF_ADDITIONAL + hRes;
						oErrorItem.sFilename = sDestFilename;
						OnError2( oErrorItem );
						break;
					}
					OnCompleteFile( sDestFilename );
				}
				CheckSuspend();
				if( FALSE == m_bRunThread )
					break;
				m_dCurProcess = 0;
			}
			m_dCurProcess = 0;
			RELEASEINTERFACE( m_piLogo );
			m_piImageVideoFile->CloseFile();
			if( true == m_bRunThread )
			{
				OnComplete();
				m_bRunThread = false;
			}
			CoUninitialize();
			return 0;
		}
public: static CString ModifyToValidXml( CString sInputString )
		 {
			 CString sResult = sInputString;
			 sResult.Replace( _T("&"), _T("&amp;") );
			 sResult.Replace( _T("<"), _T("&lt;") );
			 sResult.Replace( _T(">"), _T("&gt;") );
			 sResult.Replace( _T("'"), _T("&apos;") );
			 sResult.Replace( _T("\""), _T("&quot;") );
			 return sResult;
		 }
public: static CString ModifyToValidXmlAttr( CString sInputString )
		 {
			 //bstrXMLString = _T("<xml sttr=\"'>\">'\"</xml>"); MS parser допускает
			 CString sResult = sInputString;
			 sResult.Replace( _T("&"), _T("&amp;") );
			 sResult.Replace( _T("<"), _T("&lt;") );
			 sResult.Replace( _T("\""), _T("&quot;") );
			 return sResult;
		 }
public: static CString ModifyToValidXmlVal( CString sInputString )
		 {
			 //bstrXMLString = _T("<xml sttr=\"'>\">'\"</xml>"); MS parser допускает
			 CString sResult = sInputString;
			 sResult.Replace( _T("&"), _T("&amp;") );
			 sResult.Replace( _T("<"), _T("&lt;") );
			 sResult.Replace( _T(">"), _T("&gt;") );
			 return sResult;
		 }
public: void  SetAdditionalParam(BSTR ParamName, BSTR ParamValue)
		{
			if( ParamName == CString( _T("OldPhotoMasksFolder") ) )
			{
				m_old_photo_masks_folder = CString(ParamValue);
			}
		}
private: Gdiplus::Bitmap* BitmapFromResource(HINSTANCE hInstance,
						   LPCTSTR szResName, LPCTSTR szResType)
		{
			HRSRC hrsrc = FindResource(hInstance, szResName, szResType);
			if(!hrsrc) return 0;
			// "ненастоящий" HGLOBAL - см. описание LoadResource
			HGLOBAL hgTemp = LoadResource(hInstance, hrsrc);
			DWORD sz = SizeofResource(hInstance, hrsrc);
			void* ptrRes = LockResource(hgTemp);
			HGLOBAL hgRes = GlobalAlloc(GMEM_MOVEABLE, sz);
			if(!hgRes) return 0;
			void* ptrMem = GlobalLock(hgRes);
			// Копируем растровые данные
			CopyMemory(ptrMem, ptrRes, sz);
			GlobalUnlock(hgRes);
			IStream *pStream;
			// TRUE означает освободить память при последнем Release
			HRESULT hr = CreateStreamOnHGlobal(hgRes, TRUE, &pStream);
			if(FAILED(hr))
			{
				GlobalFree(hgRes);
				return 0;
			}
			// Используем загрузку из IStream
			Gdiplus::Bitmap *image = Gdiplus::Bitmap::FromStream(pStream);
			pStream->Release();
			return image;
		 }
		 //делает белый фон у абсолютно прозрачных картинок, чтобы при сохранении jpg фон был белым
private: void SolveAlpha( IUnknown* piImage )
		 {
			 if( NULL == piImage )
				 return;
			 MediaCore::IAVSUncompressedVideoFrame* piFrame = NULL;
			 piImage->QueryInterface( __uuidof(MediaCore::IAVSUncompressedVideoFrame), (void**)&piFrame );
			 unsigned char* pBuffer = NULL;
			 piFrame->get_Buffer( &pBuffer );
			 for( int i = 0; i + 3 < piFrame->BufferSize; i+=4 )
			 {
				 if( 0 == pBuffer[i+3] )
				 {
					 pBuffer[ i ] = 255;
					 pBuffer[i+1] = 255;
					 pBuffer[i+2] = 255;
				 }
			 }
			 RELEASEINTERFACE( piFrame );
		 }
private: IUnknown* NormalizeImage( IUnknown* piImage )
		{
			MediaCore::IAVSUncompressedVideoFramePtr piSource = NULL;
			if( FAILED( piImage->QueryInterface( __uuidof(MediaCore::IAVSUncompressedVideoFrame), (void**)&piSource ) ) )
				return NULL;

			IUnknown* piResult = NULL;
			long nSourceWidth = piSource->Width;
			long nSourceHeight = piSource->Height;
			if( nSourceWidth == m_nAnimationWidth && nSourceHeight == m_nAnimationHeight )
			{
				piImage->QueryInterface( __uuidof(MediaCore::IAVSUncompressedVideoFrame), (void**)&piResult );
				return piResult;
			}

			//надо создавать новый битмап
			MediaCore::IAVSUncompressedVideoFramePtr piResultImage = NULL;
			if( FAILED( piResultImage.CreateInstance( __uuidof( MediaCore::CAVSUncompressedVideoFrame ) ) ) )
				return NULL;

			piResultImage->Width = m_nAnimationWidth;
			piResultImage->Height = m_nAnimationHeight;
			piResultImage->put_Stride(0, m_nAnimationWidth * 4);
			piResultImage->AspectRatioX = m_nAnimationWidth;
			piResultImage->AspectRatioY = m_nAnimationHeight;
			piResultImage->DataSize = 4 * m_nAnimationWidth * m_nAnimationHeight;
			piResultImage->ColorSpace = CSP_BGRA;
			if( FAILED(piResultImage->AllocateBuffer(-1) ) )
				return NULL;
			m_sAnimationBackground;
			int nColor = Strings::ToInteger( m_sAnimationBackground );
			byte bRed = GetRValue( nColor );
			byte bGreen = GetGValue( nColor );
			byte bBlue = GetBValue( nColor );
			for( int i = 0; i + 3 < piResultImage->BufferSize; i+=4 )
			{
				piResultImage->Buffer[ i ] = bBlue;
				piResultImage->Buffer[ i + 1 ] = bGreen;
				piResultImage->Buffer[ i + 2 ] = bRed;
				piResultImage->Buffer[ i + 3 ] = 0;
			}

			ImageStudio::IImageTransformsPtr piTransform = NULL;
			if( FAILED( piTransform.CreateInstance( ImageStudio::CLSID_ImageTransforms) ) )
				return NULL;

			VARIANT vtSource1;
			vtSource1.vt = VT_UNKNOWN;
			vtSource1.punkVal = piResultImage;
			piTransform->SetSource( 0, vtSource1);

			VARIANT vtSource2;
			vtSource2.vt = VT_UNKNOWN;
			vtSource2.punkVal = piImage;
			piTransform->SetSource( 1, vtSource2);

			VARIANT vtResult;
			 
			CString sTransformXml;
			if( m_nAnimationWidth < nSourceWidth || m_nAnimationHeight < nSourceHeight ) 
				sTransformXml.AppendFormat( _T("<transforms><ImagePaint-DrawImageFromBuffer left=\"0\" top=\"0\" right=\"1\" bottom=\"1\" buffer=\"1\" metric=\"2\" scaletype=\"257\" /></transforms>"), m_sAnimationBackground );
			else
			{
				long nLeft = m_nAnimationWidth / 2 - nSourceWidth / 2;
				long nTop = m_nAnimationHeight / 2 - nSourceHeight / 2;
				long nRight = nLeft + nSourceWidth;
				long nBottom = nTop + nSourceHeight;
				sTransformXml.AppendFormat( _T("<transforms><ImagePaint-DrawImageFromBuffer left=\"%d\" top=\"%d\" right=\"%d\" bottom=\"%d\" buffer=\"1\" metric=\"0\" scaletype=\"257\" /></transforms>"), nLeft, nTop, nRight, nBottom, m_sAnimationBackground );
			}

			BSTR bstrTransformXml = sTransformXml.AllocSysString();
			if( VARIANT_TRUE == piTransform->SetXml( bstrTransformXml ) )
				if( VARIANT_TRUE == piTransform->Transform() )
					if( SUCCEEDED( piTransform->GetResult( 0, &vtResult ) ) && NULL != vtResult.punkVal )
						piResult = vtResult.punkVal;
			SysFreeString( bstrTransformXml );
			SolveAlpha( piResult );
			return piResult;
		}
private: void AddRegistration( IUnknown** piFrame )
		 {
			 //ставим Logo		
			 if( NULL != m_piLogo && NULL != piFrame )
			 {
				CString sOtherTransform;

			 	INT nLeft;
			 	INT nRight;
			 	INT nTop;
			 	INT nBottom;
			 	MediaCore::IAVSUncompressedVideoFrame* piNotFullTransformImage = NULL;
			 	(*piFrame)->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&piNotFullTransformImage );
			 	
			 	if(m_nLogoParam & IMAGECONVERTER_LOGOPARAM_HOR_LEFT)
			 		nLeft = m_nLeftIndent;
			 	else if(m_nLogoParam & IMAGECONVERTER_LOGOPARAM_HOR_RIGHT)
			 		nLeft = piNotFullTransformImage->Width - m_piLogo->Width - m_nRightIndent;
			 	else
			 		nLeft = piNotFullTransformImage->Width / 2 - m_piLogo->Width / 2;
			 	
			 	if(m_nLogoParam & IMAGECONVERTER_LOGOPARAM_VERT_TOP)
			 		nTop = m_nTopIndent;
			 	else if(m_nLogoParam & IMAGECONVERTER_LOGOPARAM_VERT_BOTTOM)
			 		nTop = piNotFullTransformImage->Height  - m_piLogo->Height  - m_nBottomIndent;
			 	else
			 		nTop = piNotFullTransformImage->Height / 2 - m_piLogo->Height / 2;
			 	RELEASEINTERFACE( piNotFullTransformImage);

			 	nRight = nLeft + m_piLogo->Width;
			 	nBottom = nTop + m_piLogo->Height;
			 	sOtherTransform.AppendFormat( _T("<ImagePaint-DrawImageFromBuffer left=\"%d\" top=\"%d\" right=\"%d\" bottom=\"%d\" buffer=\"1\" metric=\"0\" scaletype=\"256\" scalecolor=\"255\" alpha=\"255\"/>"),nLeft,nTop,nRight,nBottom);

				CString sFullTransform =_T("<transforms>");
				sFullTransform.Append( sOtherTransform );
				sFullTransform.Append(_T("</transforms>"));

				ApplyTransform( piFrame, m_piLogo, sFullTransform );
			 }

			 //добавляем текст для незарегистрированной версии
			 if( true == m_bEnableWM && NULL != piFrame )
			 {
				 //рисуем из ресурсов
				 //MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;
				 //piImageLoad->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pMediaData );

				 ////вытаскиваем MediaData
				 //BYTE* pBufferPtr = 0;
				 //long nCreatedBufferSize = 0;
				 //pMediaData->get_Buffer(&pBufferPtr);
				 //pMediaData->get_BufferSize(&nCreatedBufferSize);
				 //pMediaData->put_Plane(0, pBufferPtr);
				 ////создаем Gdi+ битмап
				 //Gdiplus::Bitmap*  tmp_buf_image = new Gdiplus::Bitmap(pMediaData->Width, pMediaData->Height, 
				 //								4*pMediaData->Width, PixelFormat32bppARGB, 
				 //								pBufferPtr);
				 //Gdiplus::Graphics tmpGr(tmp_buf_image);
				 ////вытаскиваем из ресурсов картинку
				 //Gdiplus::Bitmap*  pbLogo = BitmapFromResource(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IBD_NON_ACTIV), _T("EMF"));
				 ////опрделяем где должен находиться логотип
				 //int nImageWidth = pMediaData->Width;
				 //int nImageHeight = pMediaData->Height;
				 //int nLogoWidth = pbLogo->GetWidth();
				 //int nLogoHeight = pbLogo->GetHeight();
				 //int nX = 0;
				 //int nY = 0;
				 //int nWidth = 0;
				 //int nHeight = 0;
				 //if( nLogoWidth * nImageHeight > nImageWidth * nLogoHeight )
				 //{
				 //	nHeight = nImageWidth * nLogoHeight / nLogoWidth;
				 //	nWidth = nImageWidth;
				 //	nX = 0;
				 //	nY = nImageHeight / 2 - nHeight / 2;
				 //}
				 //else
				 //{
				 //	nWidth = nImageHeight * nLogoWidth / nLogoHeight;
				 //	nHeight = nImageHeight;
				 //	nY = 0;
				 //	nX = nImageWidth / 2 - nWidth / 2;
				 //}
				 ////рисуем логотип
				 //tmpGr.DrawImage(pbLogo, nX, nY,  nWidth, nHeight);
				 ////очищаем
				 //RELEASEINTERFACE(pMediaData);
				 //delete tmp_buf_image;
				 //delete pbLogo;

				 //рисуем текст
				 MediaCore::IAVSUncompressedVideoFrame* piPreSaveImage = NULL;
				 (*piFrame)->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&piPreSaveImage );
				 int nWidth = piPreSaveImage->Width;
				 int nHeight = piPreSaveImage->Height;
				 RELEASEINTERFACE(piPreSaveImage);

				 //1024х683 unregistered : 60 Arial bold #000000 alpha - 70 Edge: Size-5 #000000 alpha - 80
				 // avs4you : 50 Arial bold #000000 alpha - 70 Edge: Size-5 #000000 alpha - 80
				 double dRatio = (double)nHeight / 683;
				 if( nWidth * 683 < nHeight * 1024 )
					 dRatio = (double)nWidth / 1024;

				 int nEdgeSize = (int)(5 * dRatio);
				 if( nEdgeSize < 2 )
					 nEdgeSize = 2;
				 int nLine1Size = (int)(60 * dRatio);
				 if( nLine1Size < 6 )
					 nLine1Size = 6;
				 int nLine1Left = 0;
				 int nLine1Top = 0;
				 int nLine1Right = nWidth;
				 int nLine1Bottom = nHeight / 2;
				 int nLine2Size = (int)(50 * dRatio);
				 if( nLine2Size < 8 )
					 nLine2Size = 8;
				 int nLine2Left = 0;
				 int nLine2Top = nHeight / 2;
				 int nLine2Right = nWidth;
				 int nLine2Bottom = nHeight;

				 //сдвигаем границы чтобы подогнать под заданную дизайнерами картинку
				 int nLine1Margin = (int)(20 * dRatio);
				 int nLine2Margin = (int)(20 * dRatio);
				 nLine1Bottom -= nLine1Margin;
				 nLine2Top += nLine1Margin;
				 if( !( nLine1Right - nLine1Left < 90 && nLine2Right - nLine2Left < 90 ) )
				 {
					 CString sLogoTransform = _T("<transforms>");
					 sLogoTransform.AppendFormat( _T("<ImagePaint-DrawAnimatedText rect-left=\"%d\" rect-top=\"%d\" rect-right=\"%d\" rect-bottom=\"%d\" metric=\"0\" text=\"UNREGISTERED VERSION\" text-typeeffects=\"0\" text-fillmode=\"0\" font-italic=\"0\" font-underline=\"0\" font-strikeout=\"0\" font-angle=\"0\" font-stringalignmenthorizontal=\"1\" font-stringalignmentvertical=\"2\" marginhorizontal=\"0\" marginvertical=\"0\" brush-color2=\"16777215\" brush-alpha2=\"179\" brush-texturealpha=\"255\" brush-texturepath=\"\" brush-texturemode=\"1\" brush-rectable=\"0\" brush-rect-left=\"0\" brush-rect-top=\"0\" brush-rect-width=\"0\" brush-rect-height=\"0\" shadow-distancex=\"3\" shadow-distancey=\"3\" shadow-blursize=\"3\" shadow-color=\"0\" shadow-alpha=\"120\" brush-type=\"0\" edge-visible=\"-1\" font-name=\"Arial\" brush-color1=\"16777215\" shadow-visible=\"0\" brush-alpha1=\"179\" edge-color=\"0\" edge-dist=\"%d\" edge-alpha=\"204\" font-bold=\"-1\" font-size=\"%d\"/>"), nLine1Left, nLine1Top, nLine1Right, nLine1Bottom, nEdgeSize, nLine1Size);
					 sLogoTransform.AppendFormat( _T("<ImagePaint-DrawAnimatedText rect-left=\"%d\" rect-top=\"%d\" rect-right=\"%d\" rect-bottom=\"%d\" metric=\"0\" text=\"www.avs4you.com\" text-typeeffects=\"0\" text-fillmode=\"0\" font-italic=\"0\" font-underline=\"0\" font-strikeout=\"0\" font-angle=\"0\" font-stringalignmenthorizontal=\"1\" font-stringalignmentvertical=\"0\" marginhorizontal=\"0\" marginvertical=\"0\" brush-color2=\"16777215\" brush-alpha2=\"179\" brush-texturealpha=\"255\" brush-texturepath=\"\" brush-texturemode=\"1\" brush-rectable=\"0\" brush-rect-left=\"0\" brush-rect-top=\"0\" brush-rect-width=\"0\" brush-rect-height=\"0\" shadow-distancex=\"3\" shadow-distancey=\"3\" shadow-blursize=\"3\" shadow-color=\"0\" shadow-alpha=\"120\" brush-type=\"0\" edge-visible=\"-1\" font-name=\"Arial\" brush-color1=\"16777215\" shadow-visible=\"0\" brush-alpha1=\"179\" edge-color=\"0\" edge-dist=\"%d\" edge-alpha=\"204\" font-bold=\"-1\" font-size=\"%d\"/>"), nLine2Left, nLine2Top, nLine2Right, nLine2Bottom, nEdgeSize,nLine2Size);
					 sLogoTransform.Append( _T("</transforms>") );
						
					 ApplyTransform( piFrame, NULL, sLogoTransform );
				 }
			 }
		 }
private: ImageStudio::IImageTransforms* ApplyTransform2( IUnknown** piFrame, IUnknown* piFrame2, CString& sTransform, ImageStudio::IImageTransforms* piCurTransform = NULL )
		 {
			 HRESULT hRes;
			 ImageStudio::IImageTransforms* piTransform = NULL;
			 if( NULL != piCurTransform )
				 piCurTransform->QueryInterface(ImageStudio::IID_IImageTransforms, (void**)&piTransform);
			 else
			 {
				 hRes = CoCreateInstance( ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)(&piTransform));
				 if( NULL == piTransform )
					 return NULL;
				 if (!m_old_photo_masks_folder.IsEmpty())
				 {
					 CString sParamName = _T("OldPhotoMasksFolder");
					 BSTR  tmpBSTRStr1 = sParamName.AllocSysString();
					 BSTR  tmpBSTRStr2 = m_old_photo_masks_folder.AllocSysString();
					 piTransform->SetAdditionalParam(tmpBSTRStr1, tmpBSTRStr2);
					 SysFreeString( tmpBSTRStr1 );
					 SysFreeString( tmpBSTRStr2 );
				 }
				 VARIANT vImage1;
				 vImage1.vt = VT_UNKNOWN;
				 vImage1.punkVal = *piFrame;
				 piTransform->SetSource(0, vImage1);
			 }

			 if( NULL != piFrame2 )
			 {
				 VARIANT vImage2;
				 vImage2.vt = VT_UNKNOWN;
				 vImage2.punkVal =  piFrame2;
				 piTransform->SetSource(1, vImage2);
			 }

			 VARIANT vResult;vResult.punkVal = NULL;
			 BSTR bstrSave= sTransform.AllocSysString();
			 if( VARIANT_TRUE == piTransform->SetXml( bstrSave ) )
				 if( VARIANT_TRUE == piTransform->Transform() )
					 piTransform->GetResult( 0, &vResult );
			 SysFreeString( bstrSave );

			 RELEASEINTERFACE( (*piFrame) );
			 *piFrame = vResult.punkVal;
			 return piTransform;
		 }
private: void ApplyTransform( IUnknown** piFrame, IUnknown* piFrame2, CString& sTransform, ImageStudio::IImageTransforms* piCurTransform = NULL )
		 {
			ImageStudio::IImageTransforms* piOldTransform = ApplyTransform2(piFrame, piFrame2, sTransform, piCurTransform);
			RELEASEINTERFACE(piOldTransform);
		 }
private: CString GetFormatTransform( CString sSourcepath, IUnknown* piImageLoad, CString sXml )
		 {
			 CString sResult = _T("");
			 MediaCore::IAVSUncompressedVideoFrame* piFrame = NULL;
			 if( FAILED( piImageLoad->QueryInterface( __uuidof(MediaCore::IAVSUncompressedVideoFrame), (void**)&piFrame ) ) || NULL == piFrame )
				 return  sResult;
			 long nSourceWidth = piFrame->Width;
			 long nSourceHeight = piFrame->Height;
			 RELEASEINTERFACE( piFrame );

			 XmlUtils::CXmlReader oXmlReader;
			 if( TRUE == oXmlReader.OpenFromXmlString( sXml ) )
			 {
				 CString sDestpath = oXmlReader.ReadNodeAttribute( _T("destinationpath") );

				 bool bCrop = false;
				 bool bResize = false;
				 double dCropXPer = 0;
				 double dCropYPer = 0;
				 double dCropWidthPer = 100;
				 double dCropHeightPer = 100;
				 XML::IXMLDOMNodePtr oSubNode;
				 oXmlReader.GetNode( oSubNode );
				 XmlUtils::CXmlReader oXmlSubReader;
				 if( true == oXmlSubReader.OpenFromXmlNode( oSubNode ) )
				 {
					 if( TRUE == oXmlSubReader.ReadNode( _T("Crop") ) )
						  if( TRUE == oXmlSubReader.ReadNode( _T("ImageTransform-TransformResize") ) )
						  {
							  bCrop = true;
							  bResize = true;
							  dCropXPer = Strings::ToDouble( oXmlSubReader.ReadNodeAttribute( _T("src-x") ) );
							  dCropYPer = Strings::ToDouble( oXmlSubReader.ReadNodeAttribute( _T("src-y") ) );
							  dCropWidthPer = Strings::ToDouble( oXmlSubReader.ReadNodeAttribute( _T("src-width") ) );
							  dCropHeightPer = Strings::ToDouble( oXmlSubReader.ReadNodeAttribute( _T("src-height") ) );
						  }
				 }
				 if( TRUE == oXmlReader.ReadNode( _T("imageinfo") ) )
				 {
					 long nDestinationFiletype = Strings::ToInteger( oXmlReader.ReadNodeAttribute(_T("filetype"), _T("0")) );
					 if( 0 == nDestinationFiletype )
					 {
						CImageFileFormatChecker checker(sSourcepath);
						nDestinationFiletype = checker.eFileType;

					 }
					 long nPresetWidth = Strings::ToInteger( oXmlReader.ReadNodeValue(_T("Width")) );
					 long nPresetHeight = Strings::ToInteger( oXmlReader.ReadNodeValue(_T("Height")) );
					 //Сохранять ориентацию пресета (можно поворачивать пресет)
					 int nPresetKeepOrientation = Strings::ToInteger( oXmlReader.ReadNodeValue(_T("KeepOrientation"), _T("1")) );
					 //Сохранять пропорции
					 int nPresetKeepProportion = Strings::ToInteger( oXmlReader.ReadNodeValue(_T("KeepProportion"), _T("1")) );
					 //Использовать поля
					 int nPresetUseStripes = Strings::ToInteger( oXmlReader.ReadNodeValue(_T("UseStripes"), _T("0")) );
					 //Если картинка меньше чем пресет, то все-равно вписывать в размеры
					 int nPresetForceEnlarge = Strings::ToInteger( oXmlReader.ReadNodeValue(_T("ForceEnlarge"), _T("0")) );

					 long nWorkWidth = nSourceWidth;
					 long nWorkHeight = nSourceHeight;
					 int nResizeType = c_nResizeStretchSuper;

					 bool bResize = true;
					 if( true == bCrop )
					 {
						 nWorkWidth = (long)(dCropWidthPer * nSourceWidth / 100);
						 nWorkHeight = (long)(dCropHeightPer * nSourceHeight / 100);
					 }
					 if( 0 == nPresetWidth || 0 == nPresetHeight )
					 {
						 nPresetWidth = nWorkWidth;
						 nPresetHeight = nWorkHeight;
					 }
					 if( 0 == nPresetKeepOrientation && (nWorkWidth > nPresetWidth || nWorkHeight > nPresetHeight) )
					 {
						 if( (nWorkWidth - nWorkHeight) * (nPresetWidth - nPresetHeight) < 0 )
						 {
							 long nTemp = nPresetWidth;
							 nPresetWidth = nPresetHeight;
							 nPresetHeight = nTemp;
						 }
					 }
					 if( (0 != nPresetForceEnlarge) || nWorkWidth > nPresetWidth || nWorkHeight > nPresetHeight  )
					 {
						 bResize = true;
						 if( 0 == nPresetKeepProportion || 0 != nPresetUseStripes )
						 {
							 nWorkWidth = nPresetWidth;
							 nWorkHeight = nPresetHeight;
							 if( 0 != nPresetUseStripes )
								nResizeType = c_nResizeShrinkSuper;
						 }
						 else
						 {
							 if( nWorkWidth * nPresetHeight > nPresetWidth * nWorkHeight )
							 {
								 nWorkHeight = nWorkHeight * nPresetWidth / nWorkWidth;
								 nWorkWidth = nPresetWidth;
							 }
							 else
							 {
								 nWorkWidth = nWorkWidth * nPresetHeight / nWorkHeight;
								 nWorkHeight = nPresetHeight;
							 }
						 }
					 }
					 if( true == bResize )
					 {
						 sResult.AppendFormat(_T("<ImageTransform-TransformResize  width=\"%d\" height=\"%d\" metric=\"0\" cropzoom=\"0\" backcolor=\"0\" type=\"%d\""), nWorkWidth, nWorkHeight, nResizeType);
						 if( true == bCrop )
							 sResult.AppendFormat(_T(" src-x=\"%f\" src-y=\"%f\" src-width=\"%f\" src-height=\"%f\" src-metric=\"1\""), dCropXPer, dCropYPer, dCropWidthPer, dCropHeightPer );
						 sResult.Append(_T("/>"));
					 }
					 switch( nDestinationFiletype )
					 {
					 case IMAGEFORMAT_BMP: sResult.AppendFormat(_T("<ImageFile-SaveAsBmp destinationpath=\"%s\"/>"), sDestpath);break;
					 case IMAGEFORMAT_GIF: sResult.AppendFormat(_T("<ImageFile-SaveAsGif destinationpath=\"%s\"/>"), sDestpath);break;
					 case IMAGEFORMAT_JPE: sResult.AppendFormat(_T("<ImageFile-SaveAsJpeg destinationpath=\"%s\"/>"), sDestpath);break;
					 case IMAGEFORMAT_PNG: sResult.AppendFormat(_T("<ImageFile-SaveAsPng destinationpath=\"%s\"/>"), sDestpath);break;
					 case IMAGEFORMAT_TIF: sResult.AppendFormat(_T("<ImageFile-SaveAsTiff destinationpath=\"%s\"/>"), sDestpath);break;
					 case IMAGEFORMAT_WMF: sResult.AppendFormat(_T("<ImageFile-SaveAsWmf destinationpath=\"%s\"/>"), sDestpath);break;
					 case IMAGEFORMAT_EMF: sResult.AppendFormat(_T("<ImageFile-SaveAsEmf destinationpath=\"%s\"/>"), sDestpath);break;
					 case IMAGEFORMAT_PCX: sResult.AppendFormat(_T("<ImageFile-SaveAsPcx destinationpath=\"%s\"/>"), sDestpath);break;
					 case IMAGEFORMAT_TGA: sResult.AppendFormat(_T("<ImageFile-SaveAsTga destinationpath=\"%s\"/>"), sDestpath);break;
					 case IMAGEFORMAT_RAS: sResult.AppendFormat(_T("<ImageFile-SaveAsRas destinationpath=\"%s\"/>"), sDestpath);break;
					 default: sResult.AppendFormat(_T("<ImageFile-SaveAsPng destinationpath=\"%s\"/>"), sDestpath);break;
					 }
				 }
			 }
			 return sResult;
		 }
//----------------------------- Private Members -------------------------------	

private: INT m_nLogoParam;
private: MediaCore::IAVSUncompressedVideoFrame* m_piLogo;
private: BSTR m_bstrMetaData;
private: bool m_bEnableWM;
private: INT m_nLeftIndent;
private: INT m_nRightIndent;
private: INT m_nTopIndent;
private: INT m_nBottomIndent;
private: CString  m_old_photo_masks_folder;
private: DOUBLE m_dCurProcess;
private: long m_nAnimationWidth;
private: long m_nAnimationHeight;
private: CString m_sAnimationBackground;
private: ImageVideoFile::IImageVideoFile4* m_piImageVideoFile;
private: CString m_sCurApngFile;
private: long m_nLastFrame;
private: ImageStudio::IAVSImageCache* m_piCache;
};
