// PDFFile.h : Declaration of the CPDFFile

#pragma once
#include "resource.h"       // main symbols
#include "..\Common\OfficeFileTemplate.h"
#include "..\Common\OfficeFileErrorDescription.h"
#include "..\Common\RSA\XMLEncoder.h"
#include "..\Common\XmlUtils.h"
#include "..\Common\TemporaryCS.h"


#define MAX_PERCENT 1000000
#define BUF_SIZE 1024

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

//#define   AVSGraphics                 AVSOfficeEditor
//#define _IAVSDocumentPainterEvents  _IAVSOfficeViewerEvents 
//#define  IAVSDocumentPainter         IAVSOfficeViewer
//#define  IASCDocumentRenderer        IAVSCommandsRenderer
//#define _IASCDocumentRendererEvents _IAVSCommandsRendererEvents
//#define  CAVSDocumentPainter         CAVSOfficeViewer


// IPDFFile
[
	object,
	uuid("6FA6A386-C2AE-455C-ABCB-C9439224FC05"),
	dual,	helpstring("IPDFFile Interface"),
	pointer_default(unique)
]
__interface IPDFFile : IAVSOfficeFileTemplate
{
	[id(10)]               HRESULT PDFFromXml( [in] BSTR bsDstPath, [in] BSTR bsSrcXML, [in] BSTR bsXMLOptions);
	[id(100 + 0), propget] HRESULT DrawingXml( [out, retval] BSTR* pbsXml );

	[id(101), propget] HRESULT CommandRenderer([out, retval] IUnknown** ppRenderer);
	[id(101), propput] HRESULT CommandRenderer([in] IUnknown* pRenderer);

	[id(10001)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in]  VARIANT   ParamValue);
	[id(10002)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out] VARIANT *	ParamValue);

};


// CPDFFile

[
	coclass,
	default(IPDFFile),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficePDFFile.PDFFile"),
	progid("AVSOfficePDFFile.PDFFile.1"),
	version(1.0),
	uuid("A07E6910-A9C0-4123-8FCE-F39DC7925BDA"),
	helpstring("PDFFile Class")
]
class ATL_NO_VTABLE CPDFFile :	public IPDFFile, public IAVSOfficePages
{
public:

	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;

protected:

private:

	AVSOfficePDFWriter::IPDFRenderer      *m_piPdfRenderer;
	AVSGraphics::IASCDocumentRenderer     *m_piDocumentRenderer;

	AVSOfficePDFReader::IPDFReader        *m_piReader;

	HANDLE                                 m_hViewerStop;

	CString                                m_sDrawingXML; // Используется как и при сохранении PDF, так при загрузке для вьювера

	CXMLEncoder                            m_oEncoder; // Для кодирования Xml

	int                                    m_nRendererError;

	int                                    m_nPermissions; // Ограничения открываемого документа
	int                                    m_nImagesCount; // Количество картинок в последнем открытом PDF

	class CPDFPagesSizes
	{
	private:

		struct TPDFPageSize
		{
			double dWidth;
			double dHeight;
		};

	public:
		CPDFPagesSizes()
		{
			Clear();
		}
		~CPDFPagesSizes()
		{
			Clear();
		}
		void Add(double dWidth, double dHeight)
		{
			TPDFPageSize oSize;
			oSize.dWidth  = dWidth;
			oSize.dHeight = dHeight;

			m_arrSizes.Add( oSize );
			m_nPagesCount++;
		}

		void Clear()
		{
			m_nPagesCount = 0;
			m_arrSizes.RemoveAll();
		}

		int GetCount()
		{
			return m_nPagesCount;
		}

		double GetWidth(int nPageIndex)
		{
			nPageIndex = max( 0, min( m_nPagesCount - 1, nPageIndex ) );
			return (m_arrSizes.GetData()[nPageIndex]).dWidth;
		}
		double GetHeight(int nPageIndex)
		{
			nPageIndex = max( 0, min( m_nPagesCount - 1, nPageIndex ) );
			return (m_arrSizes.GetData()[nPageIndex]).dHeight;
		}

	private:


		CSimpleArray<TPDFPageSize> m_arrSizes;
		int                        m_nPagesCount;
	}         m_oPDFSizes;
	BSTR                                   m_bsEmpty;
	CRITICAL_SECTION                       m_oCS;


private:

	void OnProgressDocument( LONG nID, LONG nPercent)
	{
		// TO DO: Тут нужно отдвать процент завершённой работы
		// OnProgress( nID, nPercent );
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
		// TO DO: Тут нужно отдвать процент завершённой работы
		//OnProgress( 0, lProgressPage * MAX_PERCENT / 100 );
	}
	void OnNewPageViewer(double dWidthMm, double dHeightMm)
	{
	}
	void OnCompletePageViewer()
	{
	}


public:
	CPDFFile(): m_piPdfRenderer(NULL), m_piDocumentRenderer(NULL), m_piReader(NULL)
	{
		CoCreateInstance( __uuidof( AVSOfficePDFWriter::CPDFWriter ), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSOfficePDFWriter::IPDFRenderer), (void **)(&m_piPdfRenderer)  );

		m_hViewerStop = CreateEvent( NULL, FALSE, FALSE, NULL);
	}



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		InitializeCriticalSection(&m_oCS);
		m_nImagesCount = 1;
		m_nPermissions = 0;

		m_bsEmpty = ::SysAllocString( _T("") );

		return S_OK;
	}

	void FinalRelease()
	{
		DeleteCriticalSection(&m_oCS);
		SysFreeString( m_bsEmpty );
		RELEASEHANDLE( m_hViewerStop );
		RELEASEINTERFACE( m_piPdfRenderer );
		RELEASEINTERFACE( m_piDocumentRenderer );
		RELEASEINTERFACE( m_piReader );
	}

protected:

	long ReadWriterOptions(BSTR bsXMLOptions, AVSOfficePDFWriter::IPDFWriter *pWriter)
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

public:

//-----------------------------------------------------------------------------------------------------
//
// Интерфейс IPDFFile
//
//-----------------------------------------------------------------------------------------------------
	STDMETHOD(LoadFromFile)(BSTR bsSrcFileName, BSTR bsDstPath, BSTR bsXMLOptions)
	{
		HRESULT hRes = S_OK;

		RELEASEINTERFACE( m_piReader );
		m_oPDFSizes.Clear();

		m_piReader = NULL;
		CoCreateInstance( __uuidof( AVSOfficePDFReader::CPDFReader ), NULL, CLSCTX_INPROC_SERVER, __uuidof(AVSOfficePDFReader::IPDFReader), (void **)&m_piReader);
		if ( !m_piReader )
		{
			RELEASEINTERFACE( m_piDocumentRenderer );
			return S_FALSE;
		}

		m_piReader->SetTempFolder( bsDstPath );

		CString sDstPath(bsDstPath);
		CString sXml(bsXMLOptions);
		XmlUtils::CXmlNode oMainNode;
		oMainNode.FromXmlString(sXml);

		if ( -1 != oMainNode.GetName().Find( _T("Options") ) )
		{
			BSTR bsLoadOptions = NULL;
			XmlUtils::CXmlNode oCheckPasswordNode;
			if ( oMainNode.GetNode( _T("CheckPassword"), oCheckPasswordNode ) )
				bsLoadOptions = ::SysAllocString( _T("CheckPassword") );
			else
				bsLoadOptions = ::SysAllocString( _T("") );

			// Сначала попробуем открыть без пароля
			if ( FAILED( hRes = m_piReader->LoadPDF( bsSrcFileName, _T(""), _T(""), bsLoadOptions ) ) )
			{
				CString sPassword = _T("");
				XmlUtils::CXmlNode oPasswordNode;
				if ( oMainNode.GetNode( _T("Password"), oPasswordNode ) )
				{
					sPassword = oPasswordNode.GetText();
				}

				BSTR bsPassword = sPassword.AllocSysString();
				if ( CString(bsLoadOptions) == _T("") )
				{
					::SysFreeString( bsLoadOptions );
					bsLoadOptions = ::SysAllocString( _T("DontDeleteTempFolder") );
				}

				if ( FAILED( hRes = m_piReader->LoadPDF( bsSrcFileName, bsPassword, bsPassword, bsLoadOptions ) ) )
				{
					if ( AVS_OFFICEPDFREADER_ERROR_ENCRYPTED_PDF == hRes  )
						hRes = AVS_ERROR_PASSWORD;

					::SysFreeString( bsPassword );
					RELEASEINTERFACE( m_piDocumentRenderer );
					return hRes;
				}
				::SysFreeString( bsPassword );
			}

			// Считываем ограничения
			if ( FAILED( hRes = m_piReader->GetPDFPermissions( &m_nPermissions ) ) )
			{
				RELEASEINTERFACE( m_piDocumentRenderer );
				return hRes;
			}

			// Пусть всегда будет 1 картинка, чтобы кнока "Extract Images" не была disable.
			// В будущем, как вариант, можно читать картинки не из потока, а из ресурсов
			// страниц, так будет гораздо быстрее.

			m_nImagesCount = 1;
			//VARIANT vValue;
			//if ( FAILED( hRes = m_piReader->GetAdditionalParam( _T("ImagesCount"), &vValue )  ) )
			//	return hRes;

			//if ( VT_I8 == vValue.vt )
			//	m_nImagesCount = vValue.llVal;

			XmlUtils::CXmlNode oExtractNode;
			oMainNode.GetNode(_T("ExtractOption"), oExtractNode);
			if ( -1 != oExtractNode.GetName().Find(_T("ExtractOption")) )
			{
				XmlUtils::CXmlNode oImagesNode;
				// Пока предполагаем, что там может быть только одно поле Images
				oExtractNode.GetNode( _T("Images"), oImagesNode);
				if ( -1 != oImagesNode.GetName().Find( _T("Images") ) )
				{
					CString sExtrImagesDstPath = oImagesNode.GetAttribute( _T("path"), sDstPath);
					BSTR bsDestPathExtrImages = sExtrImagesDstPath.AllocSysString();
					m_piReader->ExtractImages( bsDestPathExtrImages, _T("") );
					::SysFreeString( bsDestPathExtrImages );
				}
			}

			XmlUtils::CXmlNode oDisplayNode;
			if ( oMainNode.GetNode( _T("Display"), oDisplayNode ) )
			{
				// Начали рисовать
				SHORT shCancel = 0;
				OnProgressEx( 0, 0, &shCancel );
				if ( c_shProgressCancel == shCancel )
				{
					RELEASEINTERFACE( m_piReader );
					return S_OK;
				}
				//if ( m_piDocumentRenderer )
				//{
				//	m_piReader->SetCommandRenderer( m_piDocumentRenderer );
				//}
				OnProgress( 0, 0 );
				int nPagesCount = 0;
				m_piReader->GetPagesCount( &nPagesCount );
				BSTR bsXmlOptions = ::SysAllocString( _T("<OpenEmptyPage />") );

				SAFEARRAYBOUND sabBounds[1];

				sabBounds[0].lLbound = 0;
				sabBounds[0].cElements = 2 * nPagesCount;

				SAFEARRAY *psaPagesSizes = SafeArrayCreate( VT_R8, 1, sabBounds );
				double *pSizes = (double*)psaPagesSizes->pvData;
				if ( !psaPagesSizes )
				{
					return S_FALSE;
				}


				for ( int nPageIndex = 1; nPageIndex <= nPagesCount; ++nPageIndex )
				{
					double dWidth  = 0;
					double dHeight = 0;

					if ( FAILED( m_piReader->DisplayPage2( m_piDocumentRenderer, nPageIndex, m_bsEmpty, bsXmlOptions, &dWidth, &dHeight, NULL ) ) )
					{
						// TO DO: Event "Can't show page"
						continue;
					}

					int nIndex = 2 * ( nPageIndex - 1 );
					pSizes[0 + nIndex] = dWidth;
					pSizes[1 + nIndex] = dHeight;

					m_oPDFSizes.Add( dWidth, dHeight );

					// Отправляем событие, о количестве обработанных страниц
					double dProgress = double(nPageIndex) / double(nPagesCount); 
					LONG lPercent = dProgress * MAX_PERCENT;
					OnProgressEx( 0, lPercent, &shCancel );
					if ( c_shProgressCancel == shCancel )
					{
						RELEASEINTERFACE( m_piDocumentRenderer );
						RELEASEINTERFACE( m_piReader );
						return S_OK;
					}
					OnProgress( 0, lPercent );
				}

				IUnknown* punkInst = NULL;
				this->QueryInterface(IID_IUnknown, (void**)&punkInst);
				m_piDocumentRenderer->SetDocumentInstance( punkInst, psaPagesSizes );
				RELEASEINTERFACE(punkInst);

				SafeArrayDestroy( psaPagesSizes );

				::SysFreeString( bsXmlOptions );
			}

			if ( bsLoadOptions )
				::SysFreeString( bsLoadOptions );
		}

		RELEASEINTERFACE( m_piDocumentRenderer );
		return S_OK;
	}
	STDMETHOD(SaveToFile)(BSTR bsDstFileName, BSTR bsSrcPath, BSTR bsXMLOptions)
	{
		HRESULT hRes = S_OK;

		if( NULL == m_piDocumentRenderer )
			return S_FALSE;

		LONG nPagesCount = 0;
		if ( FAILED( hRes = m_piDocumentRenderer->get_PageCount( &nPagesCount ) ) )
			return hRes;

		if( 0 == nPagesCount )
			return S_OK;

		// Создаем пустой документ
		AVSOfficePDFWriter::IPDFWriter * piPdfWriter = NULL;
		m_piPdfRenderer->QueryInterface( __uuidof( AVSOfficePDFWriter::IPDFWriter ), (void**)&piPdfWriter );

		if ( NULL == piPdfWriter )
			return S_FALSE;

		if ( FAILED( hRes = piPdfWriter->CreatePDF() ) )
		{
			RELEASEINTERFACE( piPdfWriter );
			return hRes;
		}

		VARIANT vTempPath;
		vTempPath.vt      = VT_BSTR;
		vTempPath.bstrVal = bsSrcPath;
		if ( FAILED( hRes = piPdfWriter->SetAdditionalParam( _T("TempDirectory"), vTempPath ) ) )
		{
			RELEASEINTERFACE( piPdfWriter );
			return hRes;
		}

#ifndef _DEBUG
		if ( FAILED( hRes = piPdfWriter->SetPDFCompressionMode( 15 ) ) )
		{
			RELEASEINTERFACE( piPdfWriter );
			return hRes;
		}
#endif

		CString sImageTarget = CString( bsDstFileName );

		for( int nIndex = 0; nIndex != nPagesCount; ++nIndex )
		{
			// Создаем новую страницу
			if ( FAILED( hRes = m_piPdfRenderer->NewPage() ) )
			{
				RELEASEINTERFACE( piPdfWriter );
				return hRes;
			}

			// Рисуем страницу
			if ( FAILED( hRes = m_piDocumentRenderer->DrawPage( nIndex, m_piPdfRenderer ) ) )
			{
				RELEASEINTERFACE( piPdfWriter );
				return hRes;
			}

			// Прогресс
			SHORT shCancel = 0;
			OnProgressEx( 0 , ( float( nIndex + 1 ) / ( nPagesCount + 1 ) ) * MAX_PERCENT, &shCancel );
			if ( c_shProgressCancel == shCancel )
			{
				// сохраняем документ до страницы, на которой пришла "отмена"
				break;
			}
			OnProgress( 0 , ( float( nIndex + 1 ) / ( nPagesCount + 1 ) ) * MAX_PERCENT );
		}

		// Дополнительная обработка PDF. Пока реализуется только построение меню.
		if ( FAILED( hRes = ReadWriterOptions( bsXMLOptions, piPdfWriter ) ) )
		{
			RELEASEINTERFACE( piPdfWriter );
			return hRes;
		}

		// Сохраняем PDF
		
		if ( FAILED( hRes = piPdfWriter->SaveToFile( bsDstFileName ) ) )
		{
			RELEASEINTERFACE( piPdfWriter );
			return hRes;
		}

		if ( FAILED( hRes = piPdfWriter->DeletePDF() ) )
		{
			RELEASEINTERFACE( piPdfWriter );
			return hRes;
		}

		RELEASEINTERFACE( piPdfWriter );

		OnProgress( 0 , MAX_PERCENT );

		return S_OK;
	}

	STDMETHOD(PDFFromXml)(BSTR bsDstFileName, BSTR bsSrcXML, BSTR bsXMLOptions)
	{
		HRESULT hRes = S_OK;
		AVSOfficePDFWriter::IPDFWriter *piPDF = NULL;
		CoCreateInstance( __uuidof( AVSOfficePDFWriter::CPDFWriter ), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSOfficePDFWriter::IPDFWriter), (void **)(&piPDF)  );	
		if ( !piPDF )
			return S_FALSE;

		if ( FAILED( hRes = piPDF->CreatePDF() ) )
		{
			RELEASEINTERFACE( piPDF );
			return hRes;
		}
		if ( FAILED( hRes = piPDF->SetPDFCompressionMode( 15 ) ) )
		{
			RELEASEINTERFACE( piPDF );
			return hRes;
		}
		SHORT shCancel = 0;
		OnProgressEx( 0, 0, &shCancel );
		OnProgress( 0, 0 );

		// Читаем XML
		CString sSrcXml(bsSrcXML);
		XmlUtils::CXmlNode oMainNode;
		oMainNode.FromXmlString( sSrcXml );

		if ( -1 != oMainNode.GetName().Find( _T("PDFEditor") ) )
		{
			XmlUtils::CXmlNode oPagesNode;
			oMainNode.GetNode( _T("Pages"), oPagesNode);
			XmlUtils::CXmlNodes oNodes;
			oPagesNode.GetNodes( _T("Page"), oNodes);

			for ( int nIndex = 0; nIndex < oNodes.GetCount(); nIndex++ )
			{
				// Отправляем событие, о количестве обработанных страниц
				double dProgress = double(nIndex) / double(oNodes.GetCount()); 
				LONG lPercent = dProgress * MAX_PERCENT;
				OnProgressEx( 0, lPercent, &shCancel );
				OnProgress( 0, lPercent );

				XmlUtils::CXmlNode oCurNode;
				oNodes.GetAt( nIndex, oCurNode );
				if ( -1 != oCurNode.GetName().Find( _T("Page") ) )
				{
					// Добавляем страницу
					if ( FAILED( hRes = piPDF->AddNewPage() ) )
					{
						RELEASEINTERFACE( piPDF );
						return hRes;
					}
					// Выставляем размеры страницы
					CString sWidth  = oCurNode.GetAttribute( _T("width") );
					CString sHeight = oCurNode.GetAttribute( _T("height") );

					double dWidth  = _wtof( sWidth.GetBuffer()  );
					double dHeight = _wtof( sHeight.GetBuffer() );

					if ( FAILED( hRes = piPDF->SetPageHeight( (float)dHeight ) ) )
					{
						RELEASEINTERFACE( piPDF );
						return hRes;
					}
					if ( FAILED( hRes = piPDF->SetPageWidth( (float)dWidth ) ) )
					{
						RELEASEINTERFACE( piPDF );
						return hRes;
					}

					XmlUtils::CXmlNodes oCurNodes;
					oCurNode.GetNodes( _T("Image"), oCurNodes);

					for (int nPageContentIndex = 0; nPageContentIndex < oCurNodes.GetCount(); nPageContentIndex++)
					{
						XmlUtils::CXmlNode oPageContentNode;
						oCurNodes.GetAt( nPageContentIndex, oPageContentNode );
						if ( -1 != oPageContentNode.GetName().Find( _T("Image") ) )
						{
							CString sImagePath   = oPageContentNode.GetAttribute( _T("path") );
							CString sImageX      = oPageContentNode.GetAttribute( _T("x") );
							CString sImageY      = oPageContentNode.GetAttribute( _T("y") );
							CString sImageWidth  = oPageContentNode.GetAttribute( _T("width") );
							CString sImageHeight = oPageContentNode.GetAttribute( _T("height") );

							double dImageX      = _wtof( sImageX.GetBuffer() );
							double dImageY      = _wtof( sImageY.GetBuffer() );
							double dImageWidth  = _wtof( sImageWidth.GetBuffer() );
							double dImageHeight = _wtof( sImageHeight.GetBuffer() );

							//AVSImageFile::IImageFile3 *pImageFile = NULL;
							//CoCreateInstance( __uuidof( AVSImageFile::ImageFile3 ), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSImageFile::IImageFile3), (void **)(&pImageFile)  );

							BSTR bsPath = sImagePath.AllocSysString();
							//IUnknown *pImage = NULL;
							//pImageFile->LoadImage2( bsPath, &pImage );
							//piPDF->LoadImageFromInterface( &pImage, 0 );
							//RELEASEINTERFACE( pImageFile );
							if ( FAILED( hRes = piPDF->LoadJpegFromFile( bsPath ) ) )
							{
								RELEASEINTERFACE( piPDF );
								::SysFreeString( bsPath );
								return hRes;
							}
							::SysFreeString( bsPath );
							if ( FAILED( hRes = piPDF->DrawCurrentImage( (float)dImageX, (float)(dHeight - dImageY - dImageHeight), (float)dImageWidth, (float)dImageHeight ) ) )
							{
								RELEASEINTERFACE( piPDF );
								return hRes;
							}


						}

					}


				}
			}
			XmlUtils::CXmlNode oOutlineNode;
			oMainNode.GetNode( _T("Outline"), oOutlineNode);
			CString sOutlineXML = oOutlineNode.GetXml();
			BSTR bsOutline = sOutlineXML.AllocSysString();
			if ( FAILED( hRes = piPDF->MakeOutlineFromXml( bsOutline ) ) )
			{
				RELEASEINTERFACE( piPDF );
				::SysFreeString( bsOutline );
				return hRes;
			}
			::SysFreeString( bsOutline );
		}		

		// 
		OnProgressEx( 0, MAX_PERCENT, &shCancel );
		OnProgress( 0, MAX_PERCENT );

		if ( FAILED( hRes = piPDF->SaveToFile( bsDstFileName ) ) )
		{
			RELEASEINTERFACE( piPDF );
			return hRes;
		}
		
		RELEASEINTERFACE( piPDF );
		return hRes;
	}
	STDMETHOD(get_DrawingXml)(BSTR *pbsXml)
	{
		if ( NULL == pbsXml || m_sDrawingXML.GetLength() <= 0 )
			return S_FALSE;

		// Предполагаем, что у нас Xml самого простого вида, т.е. с одним треком
		CStringA sXml = m_oEncoder.EncryptXML( m_sDrawingXML );
		*pbsXml = sXml.AllocSysString();

		return S_OK;
	}
	STDMETHOD(get_CommandRenderer)(IUnknown** ppRenderer)
	{
		if( NULL == ppRenderer )
			return S_FALSE;
		(*ppRenderer) = NULL;
		if( NULL == m_piDocumentRenderer )
			return S_OK;
		return m_piDocumentRenderer->QueryInterface(__uuidof(AVSGraphics::IASCDocumentRenderer), (void**)&ppRenderer);
	}
	STDMETHOD(put_CommandRenderer)(IUnknown* pRenderer)
	{
		RELEASEINTERFACE( m_piDocumentRenderer );
		if( NULL != pRenderer )
			pRenderer->QueryInterface(__uuidof( AVSGraphics::IASCDocumentRenderer), (void**)&m_piDocumentRenderer);
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

		if ( g_csBlowfishKeyParamName == sParamName )
		{		
			ParamValue->punkVal = m_oEncoder.GetBlowfishKey();
		}		
		else if ( _T("Permissions") == sParamName )
		{
			ParamValue->intVal = m_nPermissions;
		}
		else if ( _T("ImagesCount") == sParamName )
		{
			ParamValue->intVal = m_nImagesCount;
		}
		return S_OK;
	}

//-----------------------------------------------------------------------------------------------------
//
// Интерфейс IAVSOfficePages
//
//-----------------------------------------------------------------------------------------------------
	STDMETHOD(get_PagesCount)(LONG* plCount)
	{
		*plCount = m_oPDFSizes.GetCount();

		return S_OK;
	}
	STDMETHOD(GetPageInfo)(int nPageIndex, double *pdWidth, double *pdHeight, double *pdDpiX, double *pdDpiY)
	{
		*pdWidth  = m_oPDFSizes.GetWidth( nPageIndex );
		*pdHeight = m_oPDFSizes.GetHeight( nPageIndex );
		*pdDpiX   = 72.0;
		*pdDpiY   = 72.0;

		return S_OK;
	}
	STDMETHOD(DrawPage)(int nPageIndex, IUnknown* punkRenderer, BOOL *pbBreak)
	{
		if ( NULL == punkRenderer || NULL == m_piReader )
			return S_FALSE;

		//AVSGraphics::IAVSRenderer *piRenderer = NULL;
		//punkRenderer->QueryInterface(__uuidof( AVSGraphics::IAVSRenderer), (void**)&piRenderer);
		//m_piReader->SetCommandRenderer( piRenderer );
		//double dWidth, dHeight;
		//m_piReader->DisplayPage( nPageIndex + 1, m_bsEmpty, m_bsEmpty, &dWidth, &dHeight, (long *)pbBreak, IUnknown *piRenderer );
		//RELEASEINTERFACE( piRenderer );

		double dWidth, dHeight;
		m_piReader->DisplayPage2( punkRenderer, nPageIndex + 1, m_bsEmpty, m_bsEmpty, &dWidth, &dHeight, (long *)pbBreak );

		return S_OK;
	}
};

