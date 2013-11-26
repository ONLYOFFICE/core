// JBig2File.h : Declaration of the CJBig2File

#pragma once
#include "resource.h"       // main symbols

#include "jbig2encoder.h"

#include "../../Common/xmlutils.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IJBig2File
[
	object,
	uuid("A6BB29F1-8B71-45DE-8511-6A1F2FCB5730"),
	dual,	helpstring("IJBig2File Interface"),
	pointer_default(unique)
]
__interface IJBig2File : IDispatch
{
	[id(101)] HRESULT JBig2ToInterface([in]BSTR bsSrcPath, [out]IUnknown **ppImage, [in]BSTR sXmlOptions);
	[id(102)] HRESULT InterfaceToJBig2([in]IUnknown **ppImage,  [in]BSTR bsDstPath, [in]BSTR sXmlOptions);

	[id(10001), helpstring("method SetAdditionalParam")] HRESULT SetAdditionalParam([in] BSTR bsParamName, [in]  VARIANT   vParamValue);
	[id(10002), helpstring("method GetAdditionalParam")] HRESULT GetAdditionalParam([in] BSTR bsParamName, [out] VARIANT *pvParamValue);


};



// CJBig2File

[
	coclass,
	default(IJBig2File),
	threading(apartment),
	vi_progid("AVSImageJBig2.JBig2File"),
	progid("AVSImageJBig2.JBig2File.1"),
	version(1.0),
	uuid("F5F18515-2A50-434B-BFA5-37DF0F812E52"),
	helpstring("JBig2File Class")
]
class ATL_NO_VTABLE CJBig2File :
	public IJBig2File
{
public:
	CJBig2File()
	{
	}



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(JBig2ToInterface)(BSTR bsSrcPath, IUnknown **ppImage, BSTR sXmlOptions)
	{
		return S_OK;
	}
	STDMETHOD(InterfaceToJBig2)(IUnknown **ppImage, BSTR bsDstPath, BSTR sXmlOptions)
	{
		XmlUtils::CXmlNode oMainNode;
		oMainNode.FromXmlString( sXmlOptions );

		CString sBaseName, sOutputTreshold;
		BOOL bDuplicateLineRemoval = FALSE, bPDFMode = FALSE;
		BOOL bSymbolMode = FALSE, bRefine = FALSE;
		BOOL bUpscale2x = FALSE, bUpscale4x = FALSE, bSegment = FALSE;
		double dTreshold = 0.85;
		int nBwTreshold = 188;

		if ( _T("JBig2-LoadOptions") == oMainNode.GetName() )
		{
			XmlUtils::CXmlNode oNode;

			if ( oMainNode.GetNode( _T("Basename"), oNode  ) )
			{
				sBaseName = oNode.GetAttribute( _T("value") );
			}

			if ( oMainNode.GetNode( _T("DuplicateLineRemoval"), oNode ) )
			{
				bDuplicateLineRemoval = TRUE;
			}

			if ( oMainNode.GetNode( _T("PDFMode"), oNode ) )
			{
				bPDFMode = TRUE;
			}

			if ( oMainNode.GetNode( _T("SymbolMode"), oNode ) )
			{
				bSymbolMode = TRUE;
			}

			// Refine не в Symbolmode ничего не делает
			if ( bSymbolMode && oMainNode.GetNode( _T("Refine"), oNode ) )
			{
				bRefine = TRUE;
			}

			if ( oMainNode.GetNode( _T("Upscale2x"), oNode ) )
			{
				bUpscale2x = TRUE;
			}

			// ќдновременно bUpscale2x и bUpscale4x не используютс€
			if ( !bUpscale2x && oMainNode.GetNode( _T("Upscale4x"), oNode ) )
			{
				bUpscale4x = TRUE;
			}

			if ( oMainNode.GetNode( _T("OutputTreshold"), oNode ) )
			{
				sOutputTreshold = oNode.GetAttribute( _T("value") );
			}

			if ( oMainNode.GetNode( _T("Segment"), oNode ) )
			{
				bSegment = TRUE;
			}

			if ( oMainNode.GetNode( _T("Treshold"), oNode ) )
			{
				dTreshold   = min( 0.9, max( 0.4, XmlUtils::GetDouble( oNode.GetAttribute( _T("dval"), _T("0.85") ) ) ) );
				nBwTreshold = min( 255, max(   0, XmlUtils::GetInteger( oNode.GetAttribute( _T("ival"), _T("188") ) ) ) );
			}
		}



		// check for valid input parameters
		if ( !ppImage || !*ppImage )
			return S_FALSE;

		int nWidth = 0;
		int nHeight = 0;
		BYTE* pPixels = 0;

		MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;
		if ( NULL == ppImage || NULL == (*ppImage) )
			return S_FALSE;

		MediaCore::IAVSUncompressedVideoFrame* pMediaDataIn = NULL;
		(*ppImage)->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaDataIn));
		if ( NULL == pMediaDataIn )
			return S_FALSE;

		LONG lWidth      = 0; pMediaDataIn->get_Width(&lWidth);
		LONG lHeight     = 0; pMediaDataIn->get_Height(&lHeight);
		LONG lAspectX    = 0; pMediaDataIn->get_AspectRatioX(&lAspectX);
		LONG lAspectY    = 0; pMediaDataIn->get_AspectRatioY(&lAspectY);
		LONG lColorSpace = 0; pMediaDataIn->get_ColorSpace(&lColorSpace);

		LONG lBufferSize    = 0; 
		BYTE *pSourceBuffer = 0; 

		PIX  *pSource       = NULL;

		// преобразуем к BGR формату
		if ( (1 << 6) != lColorSpace )
		{
			MediaFormat::IAVSVideoFormat* pMediaFormat = NULL;
			CoCreateInstance(MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)(&pMediaFormat));
			if (NULL == pMediaFormat)
			{
				pMediaDataIn->Release();
				return S_FALSE;
			}

			MediaCore::IAVSVideoFrameTransform* pMediaTransform = NULL;
			CoCreateInstance(MediaCore::CLSID_CAVSVideoFrameTransform, NULL, CLSCTX_ALL, MediaCore::IID_IAVSVideoFrameTransform, (void**)(&pMediaTransform));
			if (NULL == pMediaTransform)
			{
				pMediaDataIn->Release();
				pMediaFormat->Release();
				return S_FALSE;
			}

			pMediaFormat->SetDefaultProperties();
			pMediaFormat->Width = lWidth;
			pMediaFormat->Height = lHeight;
			pMediaFormat->AspectRatioX = lAspectX;
			pMediaFormat->AspectRatioY = lAspectY;
			pMediaFormat->ColorSpace = 1<< 6;//CSP_BGRA

			pMediaTransform->SetVideoFormat(pMediaFormat);
			
			IUnknown *pTransformResult = NULL;
			pMediaTransform->raw_TransformFrame(pMediaDataIn, &pTransformResult);
			if (NULL != pTransformResult)
			{
				if ((*ppImage)==pTransformResult)
				{
					MediaCore::IAVSMediaData *pData = NULL;
					pTransformResult->QueryInterface(MediaCore::IID_IAVSMediaData, (void**)(&pData));
					if (NULL!=pData)
					{
						MediaCore::IAVSMediaData *pmdOutFrame = NULL;
						pData->raw_CreateDuplicate(2, &pmdOutFrame);
						pData->Release();
						if (NULL!=pmdOutFrame)
						{
							pmdOutFrame->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
							pmdOutFrame->Release();
						}
					}					
				}
				else
				{
					pTransformResult->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
				}
				pTransformResult->Release();
			}

			pMediaFormat->Release();
			pMediaTransform->Release();

			pMediaDataIn->Release();

			if ( NULL == pMediaData )
				return S_FALSE;

			lWidth        = 0; pMediaData->get_Width(&lWidth);
			lHeight       = 0; pMediaData->get_Height(&lHeight);
			lColorSpace   = 0; pMediaData->get_ColorSpace(&lColorSpace);
			lBufferSize   = 0; pMediaData->get_BufferSize(&lBufferSize);
			pSourceBuffer = 0; pMediaData->get_Buffer(&pSourceBuffer);

			pSource = pixCreate( lWidth, lHeight, 32 );
			if ( !pSource )
				return S_FALSE;

			for ( int nY = 0; nY < lHeight; nY++ )
			{
				for ( int nX = 0; nX < lWidth; nX++, pSourceBuffer += 4 )
				{
					pixSetRGBPixel( pSource, nX, nY, pSourceBuffer[ 2 ], pSourceBuffer[ 1 ], pSourceBuffer[ 0 ] );
				}
			}
			pMediaData->Release();
		}
		else
		{

			if ( NULL == pMediaDataIn )
				return S_FALSE;

			lBufferSize   = 0; pMediaDataIn->get_BufferSize(&lBufferSize);
			pSourceBuffer = 0; pMediaDataIn->get_Buffer(&pSourceBuffer);

			pSource = pixCreate( lWidth, lHeight, 32 );
			if ( !pSource )
				return S_FALSE;

			for ( int nY = 0; nY < lHeight; nY++ )
			{
				for ( int nX = 0; nX < lWidth; nX++, pSourceBuffer += 4 )
				{
					pixSetRGBPixel( pSource, nX, nY, pSourceBuffer[ 2 ], pSourceBuffer[ 1 ], pSourceBuffer[ 0 ] );
				}
			}
			pMediaDataIn->Release();
		}

		jbig2ctx *pContext = jbig2_init( dTreshold, 0.5, 0, 0, !bPDFMode, bRefine ? 10 : -1 );

		// ѕока сделаем запись одной картинки в JBig2
		// TO DO: надо будет сделать запись нескольких картинок в 1 JBig2 файл

		// ”бираем ColorMap
		PIX *pPixL = NULL;
		if ( NULL == ( pPixL = pixRemoveColormap( pSource, REMOVE_CMAP_BASED_ON_SRC ) ) ) 
		{
			pixDestroy( &pSource );
			jbig2_destroy( pContext );
			return 1;
		}
		pixDestroy( &pSource );

		PIX *pPixT = NULL;
		if ( pPixL->d > 1 ) 
		{
			PIX *pGray = NULL;

			if ( pPixL->d > 8 ) 
			{
				pGray = pixConvertRGBToGrayFast( pPixL );
				if ( !pGray )
				{
					pixDestroy( &pSource );
					jbig2_destroy( pContext );
					return S_FALSE;
				}
			} 
			else 
			{
				pGray = pixClone( pPixL );
			}

			if ( bUpscale2x ) 
			{
				pPixT = pixScaleGray2xLIThresh( pGray, nBwTreshold );
			} 
			else if ( bUpscale4x ) 
			{
				pPixT = pixScaleGray4xLIThresh( pGray, nBwTreshold );
			} 
			else 
			{
				pPixT = pixThresholdToBinary( pGray, nBwTreshold );
			}

			pixDestroy( &pGray );
		} 
		else 
		{
			pPixT = pixClone( pPixL );
		}

		if ( _T("") != sOutputTreshold ) 
		{
			USES_CONVERSION;
			pixWrite( W2A( sOutputTreshold.GetBuffer() ), pPixT, IFF_BMP );
		}

		if ( bSegment && pPixL->d > 1 ) 
		{
			PIX *pGraphics = segment_image( pPixT, pPixL );
			if ( pGraphics ) 
			{
				char *sFilename;
				asprintf( &sFilename, "%s.%04d.%s", sBaseName.GetBuffer(), 0, ".bmp" );
				pixWrite( sFilename, pGraphics, IFF_BMP );
				free( sFilename );
			} 
			if ( !pPixT ) 
			{
				// Ќичего не делаем
				return S_OK;
			}
		}

		pixDestroy( &pPixL );

		if ( !bSymbolMode ) 
		{
			int nLength = 0;
			uint8_t *pBuffer = jbig2_encode_generic( pPixT, !bPDFMode, 0, 0, bDuplicateLineRemoval, &nLength );

			HRESULT hRes = S_OK;
			FILE *pFile = _wfopen( bsDstPath, _T("wb") );
			if ( pFile && pBuffer )
			{
				::fwrite( pBuffer, nLength, 1, pFile );
				::fclose( pFile );
				hRes = S_OK;
			}
			else
				hRes = S_FALSE;

			pixDestroy( &pPixT );
			if ( pBuffer ) free( pBuffer );
			jbig2_destroy( pContext );

			return hRes;
		}


		int nNumPages = 1;
		jbig2_add_page( pContext, pPixT );
		pixDestroy( &pPixT );

		int nLength = 0;
		uint8_t *pBuffer = jbig2_pages_complete( pContext, &nLength );
		if ( !pBuffer )
		{
			jbig2_destroy( pContext );
			return S_FALSE;
		}

		if ( bPDFMode ) 
		{
			CString sFileName = sBaseName + _T(".sym");
			const int nFileD = _wopen( sFileName, O_WRONLY | O_TRUNC | O_CREAT /*| WINBINARY*/, 0600 );
			if ( nFileD < 0 )
			{
				free( pBuffer );
				jbig2_destroy( pContext );
				return S_FALSE;
			}
			write( nFileD, pBuffer, nLength );
			close( nFileD );
		}
		free( pBuffer );

		for ( int nIndex = 0; nIndex < nNumPages; ++nIndex ) 
		{
			pBuffer = jbig2_produce_page( pContext, nIndex, -1, -1, &nLength );
			if ( bPDFMode ) 
			{
				CString sFileName = sBaseName + _T(".0000");
				const int nFileD = _wopen( sFileName, O_WRONLY | O_TRUNC | O_CREAT /*| WINBINARY*/, 0600 );

				if ( nFileD < 0 )
				{
					free( pBuffer );
					jbig2_destroy( pContext );
					return S_FALSE;
				}
				write( nFileD, pBuffer, nLength );
				close( nFileD );
			} 
			free( pBuffer );
		}

		jbig2_destroy( pContext );

		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR bsParamName, VARIANT   vParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR bsParamName, VARIANT *pvParamValue)
	{
		return S_OK;
	}

};