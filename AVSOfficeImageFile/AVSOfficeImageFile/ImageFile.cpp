#include "stdafx.h"
#include "ImageFile.h"
#include "XML.h"
#include "Docx.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "StringUtils.h"

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}


COfficeImageFile::COfficeImageFile()
	: m_piCommandsRenderer(NULL), m_piEffectPainter(NULL)
{
}


HRESULT COfficeImageFile::FinalConstruct()
{
	m_pOfficeUtils = NULL;
	CoCreateInstance(__uuidof(AVSOfficeUtils::COfficeUtils),
													NULL, 
													CLSCTX_INPROC_SERVER, 
													__uuidof(AVSOfficeUtils::IOfficeUtils), 
													(void **)&(m_pOfficeUtils));

	m_piEffectPainter = NULL;
	m_nSaveMode = c_nSaveModeNone;
	m_piImageFile.CreateInstance(__uuidof(AVSImageFile3::ImageFile3));
	m_nCounter = 1;
	m_bSaveRendererStopped = false;
	return S_OK;
}


void COfficeImageFile::FinalRelease()
{
	RELEASEINTERFACE(m_pOfficeUtils);
	RELEASEINTERFACE(m_piCommandsRenderer);
	RELEASEINTERFACE(m_piEffectPainter);
}


STDMETHODIMP COfficeImageFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	const boost::filesystem::wpath imageFile	= sSrcFileName;
	const boost::filesystem::wpath docxPath		= sDstPath;
	const boost::filesystem::wpath originPath = docxPath / L"Origin";
	createOriginDocx(originPath);

	long nProgress = 50000; //5%
	OnProgress( 0, nProgress );
	SHORT shCancel = c_shProgressContinue;
	OnProgressEx( 0, nProgress, &shCancel );//ничего не делаем если c_shProgressCancel

	try
	{
		if (boost::filesystem::exists(imageFile))
		{
			AVSImageStudio3::IImageTransforms* piTransform = 0;
			const HRESULT hRes = CoCreateInstance(__uuidof(AVSImageStudio3::ImageTransforms), 
																						NULL, 
																						CLSCTX_INPROC,
																						__uuidof(AVSImageStudio3::IImageTransforms), 
																						(void**)(&piTransform));
			if (hRes != S_OK)
			{
				boost::filesystem::remove_all(originPath);
				return hRes;
			}
			long nProgress = 100000; //10%
			OnProgress( 0, nProgress );
			SHORT shCancel = c_shProgressContinue;
			OnProgressEx( 0, nProgress, &shCancel );
			if( c_shProgressCancel == shCancel )
			{
				RELEASEINTERFACE( piTransform );
				boost::filesystem::remove_all(originPath);
				return hRes;
			}


			XML::XElement openXML("transforms",
				XML::XElement("ImageFile-LoadImage",  
					XML::XAttribute("sourcepath", imageFile)
				)
			);

			Docx::Folder docx;
			docx.createFromTemplate(originPath);

			// Определяем формат
			VARIANT vImageFile;
			vImageFile.vt = VT_BSTR;
			vImageFile.bstrVal = sSrcFileName;
			piTransform->SetAdditionalParam(L"ImagePath", vImageFile);
			const long fileType = piTransform->GetAdditionalParam(L"FileType").lVal;

			if (fileType == 0) // Не многостраничное изображение
			{
				VARIANT vImage;
				if (piTransform->SetXml(openXML.ToWString().c_str()))
					if (piTransform->Transform())
						piTransform->GetResult(0, &vImage);
				
				if (vImage.punkVal != NULL)
				{
					AVSMediaCore3::IAVSUncompressedVideoFrame* piFrame = NULL;
					vImage.punkVal->QueryInterface( __uuidof(AVSMediaCore3::IAVSUncompressedVideoFrame), (void**)&piFrame );

					if (piFrame != NULL)
					{
						const long width = piFrame->Width;
						const long height = piFrame->Height;					

						docx.addImage(imageFile, width, height);
						
						RELEASEINTERFACE( piFrame );
					}
					RELEASEINTERFACE( vImage.punkVal );
				}
				piTransform->Release();
				docx.write(docxPath);
			}
			else // многостраничное изображение
			{
				VARIANT vImage;
				if (piTransform->SetXml(openXML.ToWString().c_str()))
					if (piTransform->Transform())
						piTransform->GetResult(0, &vImage);
				vImage.punkVal->Release();
				const long count = piTransform->GetAdditionalParam(L"CountFrames");

				for (int i = 0; i != count; ++i)
				{
					XML::XElement openXML("transforms",
						XML::XElement("ImageFile-LoadImage",  
							XML::XAttribute("sourcepath", imageFile) + 
							XML::XAttribute("frame", i)
						)
					);

					VARIANT vImage;
					if (piTransform->SetXml(openXML.ToWString().c_str()))
						if (piTransform->Transform())
							piTransform->GetResult(0, &vImage);
					vImage.punkVal->Release();

					XML::XElement saveXML = 
						XML::XElement("transforms", 
							XML::XElement("ImageFile-SaveAsJpeg",
								XML::XAttribute("destinationpath", docxPath / (L"temp" + ToWString(i) + L".jpg")) + 
								XML::XAttribute("format", 888)
							)
					);

					VARIANT vFrameNumber;
					vFrameNumber.vt = VT_I4;
					vFrameNumber.lVal = i;
					piTransform->SetAdditionalParam(L"ImageFrame", vFrameNumber);

					if (piTransform->SetXml(saveXML.ToWString().c_str()))
					piTransform->Transform();

					const long width = piTransform->GetAdditionalParam(L"FrameWidth").lVal;
					const long height = piTransform->GetAdditionalParam(L"FrameHeight").lVal;

					if (i == 0)
						docx.addImageInBegin(docxPath / (L"temp" + ToWString(i) + L".jpg"), width, height);
					else
						docx.addImage(docxPath / (L"temp" + ToWString(i) + L".jpg"), width, height);
					if (i != count - 1)
						docx.addPageBreak();

					double dProgress = 1.0 * ( i + 1 ) / count;
					long nProgress = 100000 + long(800000 * dProgress);// от 10% до 90%
					OnProgress( 0, nProgress );
					SHORT shCancel = c_shProgressContinue;
					OnProgressEx( 0, nProgress, &shCancel );
					if( c_shProgressCancel == shCancel )
						break;
				}
				piTransform->Release();
				docx.write(docxPath);
				for (int i = 0; i != count; ++i)
				{
					boost::filesystem::remove(docxPath / (L"temp" + ToWString(i) + L".jpg"));
				}
			}
		}
	}
	catch(...)
	{
		boost::filesystem::remove_all(originPath);
		return AVS_ERROR_UNEXPECTED;
	}
	boost::filesystem::remove_all(originPath);

	nProgress = c_nMaxProgressPercent;// 100%
	OnProgress( 0, nProgress );
	shCancel = c_shProgressContinue;
	OnProgressEx( 0, nProgress, &shCancel );
	return S_OK;
}


STDMETHODIMP COfficeImageFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	if(NULL == m_piCommandsRenderer)
		return S_FALSE;

	LONG nPageCount = 0;
	m_piCommandsRenderer->get_PageCount(&nPageCount);
	if(0 == nPageCount)
		return S_OK;

	AVSImageStudio3::IImageTransformsPtr piImageStudio = NULL;
	piImageStudio.CreateInstance(__uuidof(AVSImageStudio3::ImageTransforms));

	if( NULL == m_piImageFile || NULL == piImageStudio )
		return AVS_ERROR_UNEXPECTED;

	int nDestFormat = AVS_OFFICESTUDIO_FILE_UNKNOWN;
	CAtlArray<int> aDocumentPages;
	bool bDocumentPages = false;
	//читаем опции
	CString sOptions(sXMLOptions);
	XmlUtils::CXmlReader oXmlReader;
	if(TRUE == oXmlReader.OpenFromXmlString(sOptions))
	{
		//тип файла
		CString sType = oXmlReader.ReadNodeValue( _T("FileType"), _T("") );
		if(sType != "")
			nDestFormat = _ttoi( sType );

		//нужные страницы
		XML::IXMLDOMNodePtr pXmlNode;
		oXmlReader.GetNode( pXmlNode );
		XmlUtils::CXmlReader oXmlSubReader;
		oXmlSubReader.OpenFromXmlNode( pXmlNode );
		if( TRUE == oXmlSubReader.ReadNode( _T("DocumentPages") ) )
		{
			oXmlSubReader.ReadNodeList( _T("Page") );
			for( int i = 0; i < oXmlSubReader.GetLengthList(); i++ )
			{
				CString sValue = oXmlSubReader.ReadNodeText(i);
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

		//ищем нужные настройки
		if(AVS_OFFICESTUDIO_FILE_IMAGE_JPG == nDestFormat)
		{
			if(TRUE == oXmlReader.ReadNode( _T("JPGOptions") ))
			{
				CString sQuality = oXmlReader.ReadNodeValue( _T("Quality"), _T("") );
				if("" != sQuality)
					m_piImageFile->JpegQuality = _ttoi(sQuality);
			}
		}
		else if(AVS_OFFICESTUDIO_FILE_IMAGE_TIFF == nDestFormat)
		{
			if(TRUE == oXmlReader.ReadNode( _T("TIFFOptions") ))
			{
				CString sCompression = oXmlReader.ReadNodeValue( _T("Compression"), _T("") );
				if( _T("") != sCompression)
					m_piImageFile->TiffCompression = _ttoi(sCompression);
			}
		}
		else if( AVS_OFFICESTUDIO_FILE_UNKNOWN == nDestFormat)//если формат не задан пытаемся найти по типу опций
		{
			oXmlReader.ReadNodeList( _T("*") );
			for(int i = 0 ; i != oXmlReader.GetLengthList(); ++i)
			{
				CString sNodeName = oXmlReader.ReadNodeName(i);
				if( _T("JPGOptions") == sNodeName)
				{
					if(AVS_OFFICESTUDIO_FILE_UNKNOWN == nDestFormat)
						nDestFormat = AVS_OFFICESTUDIO_FILE_IMAGE_JPG;
					CString sQuality = oXmlReader.ReadNodeValue( _T("Quality"), _T("") );
					if( _T("") != sQuality)
						m_piImageFile->JpegQuality = _ttoi(sQuality);
					break;
				}
				else if( _T("TIFFOptions") == sNodeName)
				{
					if(AVS_OFFICESTUDIO_FILE_UNKNOWN == nDestFormat)
						nDestFormat = AVS_OFFICESTUDIO_FILE_IMAGE_TIFF;
					CString sCompression = oXmlReader.ReadNodeValue( _T("Compression"), _T("") );
					if( _T("") != sCompression)
						m_piImageFile->TiffCompression = _ttoi(sCompression);
					break;
				}
				else if( _T("TGAOptions") == sNodeName)
				{
					if(AVS_OFFICESTUDIO_FILE_UNKNOWN == nDestFormat)
						nDestFormat = AVS_OFFICESTUDIO_FILE_IMAGE_TGA;
					break;
				}
				else if( _T("PNGOptions") == sNodeName)
				{
					if(AVS_OFFICESTUDIO_FILE_UNKNOWN == nDestFormat)
						nDestFormat = AVS_OFFICESTUDIO_FILE_IMAGE_PNG;
					break;
				}
				else if( _T("GIFOptions") == sNodeName)
				{
					if(AVS_OFFICESTUDIO_FILE_UNKNOWN == nDestFormat)
						nDestFormat = AVS_OFFICESTUDIO_FILE_IMAGE_GIF;
					break;
				}
			}
		}
	}
	//если тип не пришел в опциях возвращаем ошибку
	if(AVS_OFFICESTUDIO_FILE_UNKNOWN == nDestFormat)
		return AVS_ERROR_FILEFORMAT;

	CString sImageTarget = CString(sDstFileName);
	int nRealPageCount;
	if(bDocumentPages)
		nRealPageCount = aDocumentPages.GetCount();
	else
		nRealPageCount = nPageCount;
	//создаем counter для прибавки к имени файла
	bool bNeedCounter = true;
	if( AVS_OFFICESTUDIO_FILE_IMAGE_TIFF == nDestFormat || 
		(nRealPageCount < 2 && (c_nSaveModeNone == m_nSaveMode || (0 != (c_nSaveModeStart & m_nSaveMode) && 0 != (c_nSaveModeCommit & m_nSaveMode)))))
		bNeedCounter = false;

	if( AVS_OFFICESTUDIO_FILE_IMAGE_TIFF == nDestFormat && 
		((c_nSaveModeNone == m_nSaveMode && nRealPageCount > 1) || (0 != (c_nSaveModeStart & m_nSaveMode) && (nRealPageCount > 1 || 0 == (c_nSaveModeCommit & m_nSaveMode)) )))
		m_piImageFile->TiffMultipaged = VARIANT_TRUE;

	HRESULT hRes = S_OK;
	VARIANT_BOOL vbSuccess = VARIANT_TRUE;
	int nStartPage = 0;
	int nEndPage = nPageCount - 1;
	if(bDocumentPages)
	{
		nStartPage = aDocumentPages[0];
		nEndPage = aDocumentPages[aDocumentPages.GetCount() - 1];
	}
	bool bNeedStop = false;
	for(int i = 0; i != nPageCount; ++i)
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
				continue;;
		}
		//получаем размер страницы
		double dWidthMm = 0;
		double dHeightMm = 0;
		m_piCommandsRenderer->GetPageSize(i, &dWidthMm, &dHeightMm);
		if(dWidthMm <= 0 || dHeightMm <= 0)
			continue;
		//Создаем новую картинку для рисования
		AVSMediaCore3::IAVSUncompressedVideoFrame* piImage = NULL;
		AVSGraphics::IAVSGraphicsRenderer* piGdiplusRenderer = NULL;
		UpdateGdiPlusRenderer(dWidthMm, dHeightMm, &piImage, &piGdiplusRenderer);
		piGdiplusRenderer->NewPage();

		//рисуем страницу
		hRes = m_piCommandsRenderer->DrawPage(i, piGdiplusRenderer);
		
		if (NULL != m_piEffectPainter)
		{
			m_piEffectPainter->Draw((IUnknown*)piGdiplusRenderer);
		}

		RELEASEINTERFACE(piGdiplusRenderer);
		if(S_OK != hRes)
		{
			RELEASEINTERFACE(piImage);
			continue;
		}

		//задаем имя
		CString sFileName;
		if( true == bNeedCounter )
		{
			TCHAR tExt[256];
			TCHAR tName[256];
			TCHAR tDir[256];
			TCHAR tDrive[256];
			_tsplitpath( sImageTarget, tDrive, tDir, tName, tExt );
			CString sDrive = CString(tDrive);
			CString sDir = CString(tDir);
			CString sName = CString(tName);
			CString sExt = CString(tExt);

			CString sCounter;
			sCounter.AppendFormat( _T("%d"), m_nCounter );
			for( int i = sCounter.GetLength(); i < 3; i++ )//формат счетчика 00*
				sCounter.Insert( 0, '0' );
			sCounter.Insert( 0, '_' );
			sName.Append( sCounter );

			sFileName = sDrive + sDir + sName + sExt;
			m_nCounter++;
		}
		else
			sFileName = sImageTarget;

		//сохраняем в файл
		BSTR bstrFilename = sFileName.AllocSysString();
		if(AVS_OFFICESTUDIO_FILE_IMAGE_JPG == nDestFormat)
			m_piImageFile->raw_SaveImage2((IUnknown**)&piImage, IMAGEFORMAT_JPE, bstrFilename, &vbSuccess );
		else if(AVS_OFFICESTUDIO_FILE_IMAGE_TGA == nDestFormat)
			m_piImageFile->raw_SaveImage2((IUnknown**)&piImage, IMAGEFORMAT_TGA, bstrFilename, &vbSuccess );
		else if(AVS_OFFICESTUDIO_FILE_IMAGE_GIF == nDestFormat)
		{
			vbSuccess = VARIANT_FALSE;
			VARIANT vtFrame;
			vtFrame.vt = VT_UNKNOWN;
			vtFrame.punkVal = piImage;

			piImageStudio->SetSource( 0, vtFrame );

			CString sSaveXml;
			sSaveXml.AppendFormat( _T("<ImageFile-SaveAsGif format=\"8888\" destinationpath=\"%s\" />"), CString(bstrFilename) );
			BSTR bstrSaveXml = sSaveXml.AllocSysString();
			if( VARIANT_TRUE == piImageStudio->SetXml( bstrSaveXml ) )
				if( VARIANT_TRUE == piImageStudio->Transform() )
					if( SUCCEEDED( piImageStudio->GetResult( 0, &vtFrame ) ) )
					{
						RELEASEINTERFACE( vtFrame.punkVal );
						vbSuccess = VARIANT_TRUE;
					}
			SysFreeString( bstrSaveXml );
		}
		else if(AVS_OFFICESTUDIO_FILE_IMAGE_PNG == nDestFormat)
			m_piImageFile->raw_SaveImage2((IUnknown**)&piImage, IMAGEFORMAT_PNG, bstrFilename, &vbSuccess );
		else if(AVS_OFFICESTUDIO_FILE_IMAGE_TIFF == nDestFormat)
		{
			if(c_nSaveModeNone == m_nSaveMode)
			{
				if(nStartPage == i)
				{
					if(true == bNeedStop)
						m_piImageFile->TiffMultipaged = VARIANT_FALSE;

					m_piImageFile->raw_SaveImage2((IUnknown**)&piImage, IMAGEFORMAT_TIF, bstrFilename, &vbSuccess );
				}
				else if(nEndPage == i || true == bNeedStop)
					m_piImageFile->raw_SaveFrame2((IUnknown**)&piImage, VARIANT_TRUE, &vbSuccess );
				else
					m_piImageFile->raw_SaveFrame2((IUnknown**)&piImage, VARIANT_FALSE, &vbSuccess );
			}
			else
			{
				if(nStartPage == i)
				{
					if(0 != (c_nSaveModeStart & m_nSaveMode))
					{
						if(true == bNeedStop)
							m_piImageFile->TiffMultipaged = VARIANT_FALSE;

						m_piImageFile->raw_SaveImage2((IUnknown**)&piImage, IMAGEFORMAT_TIF, bstrFilename, &vbSuccess );
					}
					else if((0 != (c_nSaveModeCommit & m_nSaveMode) && nEndPage == i) || true == bNeedStop)
						m_piImageFile->raw_SaveFrame2((IUnknown**)&piImage, VARIANT_TRUE, &vbSuccess );
					else
						m_piImageFile->raw_SaveFrame2((IUnknown**)&piImage, VARIANT_FALSE, &vbSuccess );
				}
				else if(nEndPage == i)
				{
					if(0 != (c_nSaveModeCommit & m_nSaveMode) || true == bNeedStop)
						m_piImageFile->raw_SaveFrame2((IUnknown**)&piImage, VARIANT_TRUE, &vbSuccess );
					else
						m_piImageFile->raw_SaveFrame2((IUnknown**)&piImage, VARIANT_FALSE, &vbSuccess );
				}
				else
				{
					if(true == bNeedStop)
						m_piImageFile->raw_SaveFrame2((IUnknown**)&piImage, VARIANT_TRUE, &vbSuccess );
					else
						m_piImageFile->raw_SaveFrame2((IUnknown**)&piImage, VARIANT_FALSE, &vbSuccess );
				}
			}
		}
		SysFreeString(bstrFilename);
		RELEASEINTERFACE(piImage);
		if( FAILED( hRes ) || VARIANT_FALSE == vbSuccess )
		{
			if( VARIANT_FALSE == vbSuccess )
				hRes = AVS_ERROR_UNEXPECTED;
			break;
		}
		if(true == bNeedStop)
		{
			m_bSaveRendererStopped = true;
			break;
		}
		//прогресс
		double dProgress = 1.0 * ( i + 1 ) / nRealPageCount;
		long nProgress = long(c_nMaxProgressPercent * dProgress);
		OnProgress(0 , nProgress );
		SHORT shCancel = c_shProgressContinue;
		OnProgressEx( 0, nProgress, &shCancel );
		if( c_shProgressCancel == shCancel )
			bNeedStop = true;
	}
	return hRes;
}


STDMETHODIMP COfficeImageFile::get_CommandRenderer(IUnknown** ppunkRend)
{
	if(NULL == ppunkRend)
		return S_FALSE;
	(*ppunkRend) = NULL;
	if(NULL == m_piCommandsRenderer)
		return S_OK;
	return m_piCommandsRenderer->QueryInterface(__uuidof(AVSGraphics::IAVSDocumentRenderer), (void**)&ppunkRend);
}


STDMETHODIMP COfficeImageFile::put_CommandRenderer(IUnknown* ppunkRend)
{
	RELEASEINTERFACE(m_piCommandsRenderer);
	if(NULL != ppunkRend)
		ppunkRend->QueryInterface(__uuidof( AVSGraphics::IAVSDocumentRenderer), (void**)&m_piCommandsRenderer);
	return S_OK;
}


STDMETHODIMP COfficeImageFile::SetAdditionalParam( BSTR ParamName, VARIANT ParamValue)
{
	CString sParamName = ParamName;
	if ( _T("EffectPainter") == sParamName && VT_UNKNOWN == ParamValue.vt )
	{
		RELEASEINTERFACE( m_piEffectPainter );
		ParamValue.punkVal->QueryInterface( __uuidof( AVSGraphics::IAVSEffectPainter ), (void**)&m_piEffectPainter );
	}
	else if (sParamName == L"SaveMode")
	{
		if( VT_I4 == ParamValue.vt)
			m_nSaveMode = ParamValue.lVal;
	}
	return S_OK;
}
STDMETHODIMP COfficeImageFile::GetAdditionalParam( BSTR ParamName, VARIANT* ParamValue)
{
	CComBSTR ParamStr = ParamName;
	ParamStr.ToLower();
	if (ParamStr == L"saverendererstopped")
	{
		(*ParamValue).vt = VT_I4;
		(*ParamValue).lVal = m_bSaveRendererStopped;
	}
	return S_OK;
}
void COfficeImageFile::UpdateGdiPlusRenderer(double dWidthMm, double dHeightMm, AVSMediaCore3::IAVSUncompressedVideoFrame** piImage, AVSGraphics::IAVSGraphicsRenderer** piRend)
{
	(*piImage) = NULL;
	(*piRend) = NULL;
	CoCreateInstance(__uuidof( AVSMediaCore3::CAVSUncompressedVideoFrame), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSMediaCore3::IAVSUncompressedVideoFrame), (void **)piImage);

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

	CoCreateInstance(__uuidof( AVSGraphics::CAVSGraphicsRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IAVSGraphicsRenderer), (void **)piRend);
	//ставим FontManager
	VARIANT vtVariant;
	vtVariant.vt = VT_UNKNOWN;
	vtVariant.punkVal = NULL;
	(*piRend)->SetAdditionalParam( L"FontManager", vtVariant );

	(*piRend)->put_Width(dWidthMm);
	(*piRend)->put_Height(dHeightMm);
	(*piRend)->CreateFromMediaData(*piImage, 0, 0, nWidthPix, nHeightPix );
}


void COfficeImageFile::createOriginDocx(const boost::filesystem::wpath& path) const
{
	const boost::filesystem::wpath docxFile = path / L"origin.docx";
	boost::filesystem::create_directories(path);

	LoadFromResource(MAKEINTRESOURCE(IDR_DOCUMENT1), _T("Document"), _bstr_t(docxFile.string().c_str()));
	m_pOfficeUtils->ExtractToDirectory(_bstr_t(docxFile.string().c_str()), _bstr_t(path.string().c_str()), NULL, 0);
	boost::filesystem::remove(docxFile);
}


const unsigned long COfficeImageFile::LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const
{
	HMODULE hMod = GetModuleHandle(_T("AVSOfficeImageFile.dll"));
	if (hMod)
	{
		HRSRC hRes = FindResource(hMod, lpResName, lpResType);
		if (hRes)
		{
			HGLOBAL hGlob = LoadResource(hMod, hRes);
			if (hGlob)
			{
				BYTE *lpbArray = (BYTE*)LockResource(hGlob);
				if (lpbArray)
				{
					const DWORD dwFileSize = SizeofResource(hMod, hRes);
					if (dwFileSize != 0)
					{
						int hFile = 0;
						if (!_wsopen_s(&hFile, fileName, (O_BINARY | O_CREAT | _O_RDWR ), _SH_DENYNO, S_IWRITE))
						{
							_write(hFile, lpbArray, dwFileSize);
							_close(hFile);
						}
					}
				}
			}
		}
	}
	return GetLastError();
}
