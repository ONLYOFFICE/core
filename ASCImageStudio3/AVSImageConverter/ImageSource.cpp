// ImageSource.cpp : Implementation of CAVSXMLImageSource

#include "stdafx.h"
#include "ImageSource.h"
#include <xmlutils.h>
#include <Array.h>
#include "mediadatadefine.h"
#include "ImageConverterDefines.h"
#include "ImageSaver.h"
#include "TemporaryCS.h"

STDMETHODIMP CAVSXMLImageSource::LoadImage(BSTR sXMLString, LONG nWidth, LONG nHeight, IUnknown** ppImage)
{
	CTemporaryCS oCS(&m_csThread);
	if(NULL == ppImage || NULL == sXMLString || NULL == m_piTransform || NULL == m_piImageVideoFile )
		return S_FALSE;

	(*ppImage) = NULL;

	//парсим xml
	XmlUtils::CXmlReader oReader;
	if( TRUE == oReader.OpenFromXmlString( CString(sXMLString) ))
		if( TRUE == oReader.ReadRootNode( _T("Source")) )
		{
			CString sFilePath = oReader.ReadNodeAttribute( _T("filepath"), _T("") );
			int nFrame = Strings::ToInteger( oReader.ReadNodeAttribute( _T("page"), _T("0") ) );
			if( _T("") != sFilePath)
			{
				CString sAdditionalTransform = _T("");
				IUnknown* piImageLoad = NULL;
				if( NULL != m_piFilesCache && 0 == nFrame)
				{
					//открываем через FileCache
					BSTR bstrFilename = sFilePath.AllocSysString();
					HRESULT hRes = m_piFilesCache->raw_LoadImage2( bstrFilename, nWidth, nHeight, nWidth, nHeight, &piImageLoad );
					SysFreeString( bstrFilename );
					if( false == SUCCEEDED( hRes ) || NULL == piImageLoad )
						return S_FALSE;
				}
				else
				{
					CImageFileFormatChecker checker;
					if( sFilePath == m_sCurApngFile || checker.isPngFile(sFilePath))
					{
						HRESULT hRes = S_OK;
						//если файл открыт, то не переоткрываем
						if( sFilePath != m_sCurApngFile )
						{
							m_piImageVideoFile->CloseFile();
							BSTR bstrFilePath  = sFilePath.AllocSysString();
							hRes = m_piImageVideoFile->OpenFile( bstrFilePath );
							SysFreeString( bstrFilePath );
							if( false == SUCCEEDED( hRes ) )
								return IMAGECONVERTER_ERROR_IMAGEFILE;

							m_sCurApngFile = sFilePath;
							m_nLastFrame = -1;
						}

						//если запрошен не следующий фрейм, то делаем seek
						if( m_nLastFrame + 1 != nFrame )
						{
							VARIANT vtSetAddPar;
							vtSetAddPar.vt = VT_I4;
							vtSetAddPar.lVal = nFrame;
							hRes = m_piImageVideoFile->SetAdditionalParam( _T("seektoframe"), vtSetAddPar );
							if( false == SUCCEEDED( hRes ) )
								return IMAGECONVERTER_ERROR_IMAGEFILE;
							m_nLastFrame = nFrame - 1;//-1 потому что далее будет m_nLastFrame++;
						}

						//читаем frame
						hRes = m_piImageVideoFile->ReadVideo( &piImageLoad );
						if( false == SUCCEEDED( hRes ) )
							return IMAGECONVERTER_ERROR_IMAGEFILE;

						m_nLastFrame++;

						//закрываем файл чтобы не блокировать его. Хотя чтобы читать все фреймы эффективнее было бы держать его открытым.
						m_piImageVideoFile->CloseFile();
						m_sCurApngFile = _T("");
						m_nLastFrame = -1;
					}
					else
					{
						//открываем через ImageStudio
						VARIANT vImage;
						CString sTransformLoad = _T("<ImageFile-LoadImage sourcepath=\"")+ ImageSaver::ModifyToValidXml( sFilePath ) +_T("\"");
						if( 0 != nFrame )
							sTransformLoad.AppendFormat( _T(" frame=\"%d\""), nFrame);
						sTransformLoad.Append( _T(" />") );
						BSTR bstrLoad = sTransformLoad.AllocSysString();
						if( m_piTransform->SetXml(bstrLoad) )
							if(m_piTransform->Transform())
								m_piTransform->GetResult( 0, &vImage );
						SysFreeString( bstrLoad );
						piImageLoad = vImage.punkVal;
					}

					if( NULL == piImageLoad )
					{
						BSTR bstrFilename = sFilePath.AllocSysString();
						HRESULT hRes = m_piImageFile->LoadImage2( bstrFilename, &piImageLoad );
						SysFreeString( bstrFilename );
						if( NULL == piImageLoad )
							return S_FALSE;
					}

					//resize также как и в IAVSImageFilesCache2
					if (( nWidth > 0 ) && (nHeight > 0))
					{
						sAdditionalTransform.AppendFormat( _T("<ImageTransform-TransformResize width=\"%d\" height=\"%d\" type=\"257\"/>"), nWidth, nHeight );
					}
					else if( (nWidth <= 0) && ( nHeight > 0) )
					{
						MediaCore::IAVSUncompressedVideoFrame* pVideoFrame = NULL;
						HRESULT hRes = piImageLoad->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pVideoFrame );
						if( FAILED( hRes ) || NULL == pVideoFrame )
						{
							RELEASEINTERFACE( piImageLoad );
							return S_FALSE;
						}
						int nNewWidth = nHeight * pVideoFrame->Width / pVideoFrame->Height;
						int nNewHeight = nHeight;
						RELEASEINTERFACE( pVideoFrame );

						sAdditionalTransform.AppendFormat( _T("<ImageTransform-TransformResize width=\"%d\" height=\"%d\" type=\"1\"/>"), nNewWidth, nNewHeight );
					}
					else if( (nWidth > 0) && ( nHeight <= 0) )
					{
						MediaCore::IAVSUncompressedVideoFrame* pVideoFrame = NULL;
						HRESULT hRes = piImageLoad->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pVideoFrame );
						if( FAILED( hRes ) || NULL == pVideoFrame )
						{
							RELEASEINTERFACE( piImageLoad );
							return S_FALSE;
						}
						int nNewWidth = nWidth;
						int nNewHeight = nWidth * pVideoFrame->Height / pVideoFrame->Width;
						RELEASEINTERFACE( pVideoFrame );

						sAdditionalTransform.AppendFormat( _T("<ImageTransform-TransformResize width=\"%d\" height=\"%d\" type=\"1\"/>"), nNewWidth, nNewHeight );
					}
				}
				int nBufferIndex = 0;
				//делаем дополнительные трансформации и resize в соответствии с nWidth, nHeight
				if( (TRUE == oReader.ReadNode( _T("Transforms")) && TRUE == oReader.ReadNodeList( _T("*")) && oReader.GetLengthList() > 0) || ( false == sAdditionalTransform.IsEmpty() ))
				{
					CString sFinalTransform( _T("<transforms>") );
					sFinalTransform.Append( sAdditionalTransform );
					int nNodeCount = oReader.GetLengthList();
					if( nNodeCount > 0 )
					{
						for(INT32 i=0; i < nNodeCount; i++)
						{
							//заменяем эффект DrawImageFromFile на DrawImageFromBuffer( чтобы файл брался из кеша)
							CString sCurXml = oReader.ReadNodeXml(i);
							if( NULL != m_piFilesCache && -1 != sCurXml.Find( _T("ImagePaint-DrawImageFromFile") ) )
							{
								//Расчитываем на такую xml
								//<ImagePaint-DrawImageFromFile metric="0" enableGPU="0" alpha="255" angle="-360" left="49" top="79" ICUse_keep_size="-1" filepath="\\mediaserver\Media\Images\psd\2.psd" right="413" bottom="621"/>
								CString sFilepath = oReader.ReadNodeAttribute(i, _T("filepath"));
								CString sLeft = oReader.ReadNodeAttribute(i, _T("left"));
								CString sTop = oReader.ReadNodeAttribute(i, _T("top"));
								CString sRight = oReader.ReadNodeAttribute(i, _T("right"));
								CString sBottom = oReader.ReadNodeAttribute(i, _T("bottom"));
								CString sWidth = oReader.ReadNodeAttribute(i, _T("width"));
								CString sHeight = oReader.ReadNodeAttribute(i, _T("height"));

								long nWidth = -1;
								long nHeight = -1;
								if( _T("") != sWidth && _T("") != sHeight )
								{
									nWidth = Strings::ToInteger( sWidth );
									nHeight = Strings::ToInteger( sHeight );
								}
								else if( _T("") != sLeft && _T("") != sTop && _T("") != sRight && _T("") != sBottom )
								{
									nWidth = Strings::ToInteger( sRight ) - Strings::ToInteger( sLeft );
									nHeight = Strings::ToInteger( sBottom ) - Strings::ToInteger( sTop );
								}
								if( _T("") != sFilepath && -1 != nWidth && -1 != nHeight )
								{
									IUnknown* piOverlay = NULL;
									//открываем через FileCache
									BSTR bstrFilename = sFilepath.AllocSysString();
									HRESULT hRes = m_piFilesCache->raw_LoadImage2( bstrFilename, nWidth, nHeight, nWidth, nHeight, &piOverlay );
									SysFreeString( bstrFilename );
									if( true == SUCCEEDED( hRes ) && NULL != piOverlay )
									{
										nBufferIndex++;
										VARIANT vtOverlay;vtOverlay.vt = VT_UNKNOWN;vtOverlay.punkVal = piOverlay;
										m_piTransform->raw_SetSource( nBufferIndex, vtOverlay );
										RELEASEINTERFACE( piOverlay );

										CString sReplaceStr = _T("ImagePaint-DrawImageFromBuffer");
										sReplaceStr.AppendFormat( _T(" buffer=\"%d\""), nBufferIndex );
										sCurXml.Replace( _T("ImagePaint-DrawImageFromFile"), sReplaceStr );

									}

								}
							}
							sFinalTransform.Append( sCurXml );
						}
					}
					sFinalTransform.Append( _T("</transforms>") );

					VARIANT vImage;
					vImage.vt = VT_UNKNOWN;
					vImage.punkVal = piImageLoad;
					m_piTransform->SetSource(0, vImage);

					BSTR bstrLoad = sFinalTransform.AllocSysString();
					if( m_piTransform->SetXml( bstrLoad) )
						if( m_piTransform->Transform() )
							m_piTransform->GetResult(0, &vImage);
					SysFreeString( bstrLoad );

					RELEASEINTERFACE( piImageLoad );
					if( NULL == vImage.punkVal ) 
						return S_FALSE;
					piImageLoad = vImage.punkVal;

					//очищаем ImageStudio
					CString sClearXml = _T("<transforms><ImageClear index=\"0\"/>");
					for( int i = 1; i <= nBufferIndex; i++  )
						sClearXml.AppendFormat( _T("<ImageClear index=\"%d\"/>"), i );
					sClearXml.Append( _T("</transforms>") );
					BSTR bstrClear = sClearXml.AllocSysString();
					if( m_piTransform->SetXml(bstrClear) )
						if(m_piTransform->Transform())
							m_piTransform->GetResult( 0, &vImage );
					SysFreeString( bstrClear );
				}
				
				piImageLoad->QueryInterface( IID_IUnknown, (void**)ppImage );
				RELEASEINTERFACE( piImageLoad );

				return S_OK;
			}
		}
	return S_FALSE;
}
STDMETHODIMP CAVSXMLImageSource::get_FilesCache(IUnknown** pVal)
{
	if(  NULL == pVal )
		return S_FALSE;
	(*pVal) = NULL;
	if( NULL == m_piFilesCache )
		return S_OK;

	return m_piFilesCache->QueryInterface( IID_IUnknown , (void**)pVal );
}
STDMETHODIMP CAVSXMLImageSource::put_FilesCache(IUnknown* newVal)
{
	CTemporaryCS oCS(&m_csThread);

	RELEASEINTERFACE(m_piFilesCache);
	if(NULL != newVal)
	{
		ImageStudio::IAVSImageCache* piFileCache = NULL;
		HRESULT hRes = newVal->QueryInterface( ImageStudio::IID_IAVSImageCache, (void**)&piFileCache );
		if( SUCCEEDED(hRes) &&  NULL != piFileCache )
			m_piFilesCache = piFileCache;
		else
			return S_FALSE;
	}
	return S_OK;
}
STDMETHODIMP CAVSXMLImageSource::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	if( CString(ParamName) == CString( _T("OldPhotoMasksFolder") ) )
	{
		m_piTransform->SetAdditionalParam(ParamName, ParamValue);
	}
	return S_OK;
}
STDMETHODIMP CAVSXMLImageSource::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}
