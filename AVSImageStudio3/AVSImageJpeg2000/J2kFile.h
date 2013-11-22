// J2kFile.h : Declaration of the CJ2kFile

#pragma once
#include "resource.h"       // main symbols

#include "Registration.h"
#include "XmlUtils.h"
#include "Types.h"

#define PROGRESS_MAX 1000000


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IJ2kFile
[
	object,
	uuid("D938D2B5-A639-4C55-B0AE-1A51D2DC718C"),
	dual,	helpstring("IJ2kFile Interface"),
	pointer_default(unique)
]
__interface IJ2kFile : IDispatch
{
	[id(1001)] HRESULT J2kToInterface([in]BSTR bsSrcPath, [out]IUnknown **ppImage, [in]BSTR sXmlOptions);
	[id(1002)] HRESULT InterfaceToJ2k([in]IUnknown **ppImage,  [in]BSTR bsSrcPath, [in]BSTR sXmlOptions);
	[id(1003)] HRESULT MakeSavePreview([in]IUnknown **ppSrcImage, [out]IUnknown **ppDstImage, [in]BSTR sXmlOptions);
	[id(1004)] HRESULT GetFileInfo([in]BSTR bsSrcPath, [out]BSTR *pbsXmlFileInfo);

	[id(10001), helpstring("method SetAdditionalParam")] HRESULT SetAdditionalParam([in] BSTR bsParamName, [in]  VARIANT   vParamValue);
	[id(10002), helpstring("method GetAdditionalParam")] HRESULT GetAdditionalParam([in] BSTR bsParamName, [out] VARIANT *pvParamValue);

};

// _IAVSOfficeFileTemplateEvents
[
	dispinterface,
	uuid("8B2E4132-B1C2-443e-80A1-4F052FD66D9D"),
	helpstring("_IAVSOfficeFileTemplateEvents Interface")
]

__interface _IAVSJ2kFileEvents
{
	//Max Value nPercent == 1000000
	//Example 23,56 % == 235600
	[id(1), helpstring("method OnProgress")] HRESULT OnProgress([in] LONG nID, [in] LONG nPercent);
};


// CJ2kFile

[
	coclass,
	default(IJ2kFile),
	threading(apartment),
	event_source(com),
	vi_progid("AVSImageJpeg2000.J2kFile"),
	progid("AVSImageJpeg2000.J2kFile.1"),
	version(1.0),
	uuid("1221E292-1767-4F25-BC16-23A5A7E3246F"),
	helpstring("J2kFile Class")
]
class ATL_NO_VTABLE CJ2kFile :
	public IJ2kFile
{
public:

	__event __interface _IAVSJ2kFileEvents;

public:
	CJ2kFile()
	{
#ifdef _DEBUG
          _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
          _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW );
#endif
	}



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	long ApplyDecoderOptions(DecoderParams *pParameters, CString sXml)
	{

		memset( pParameters, 0, sizeof(DecoderParams) );
		pParameters->eLimitDecoding = declimNoLimitation;
		pParameters->bConvertToRGBA = true;
		pParameters->nReduce = 0;
		pParameters->nLayer  = 0;

		if ( sXml.GetLength() > 0 )
		{
			XmlUtils::CXmlNode oMainNode;
			oMainNode.FromXmlString( sXml );

			if ( -1 != oMainNode.GetName().Find( _T("Jpeg2000-Options") ) )
			{
				XmlUtils::CXmlNode oOpenNode;
				if ( oMainNode.GetNode( _T("OpenOptions"), oOpenNode ) )
				{
					XmlUtils::CXmlNode oCurNode;
					CString sValue;

					// ReduceFactor
					if ( oOpenNode.GetNode( _T("ConvertToRGBA"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("value"), _T("0") );
						pParameters->bConvertToRGBA = ( _wtoi( sValue.GetBuffer() ) ? true : false );
					}

					// ReduceFactor
					if ( oOpenNode.GetNode( _T("ReduceFactor"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("value"), _T("0") );
						pParameters->nReduce = _wtoi( sValue.GetBuffer() ); 
					}

					// LayersCount
					if ( oOpenNode.GetNode( _T("LayersCount"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("value"), _T("0") );
						pParameters->nLayer = _wtoi( sValue.GetBuffer() ); 
					}
				}
			}
		}

		return 0;
	}
	long ApplyEncoderOptions(EncoderParams *pParameters, CString sXml)
	{
		int nFormat = 0; // J2k
		// Сначала выставляем стандартные значения параметров
		memset( pParameters, 0, sizeof(EncoderParams) );
		pParameters->nComponentsCount     = 3;
		pParameters->nResolutionsCount    = 6;
		pParameters->nCodeBlockHeightInit = 64;
		pParameters->nCodeBlockWidthInit  = 64;
		pParameters->eProgOrder           = poLRCP;
		pParameters->nROIComponentIndex   = -1;     // Нет ROI
		pParameters->nSubSamplingDx       = 1;
		pParameters->nSubSamplingDy       = 1;
		pParameters->bTileSizeOn          = false;
		pParameters->sComment             = "Manufactured by Online Media Technologies Ltd.";

		if ( sXml.GetLength() > 0 )
		{
			XmlUtils::CXmlNode oMainNode;
			oMainNode.FromXmlString( sXml );

			if ( -1 != oMainNode.GetName().Find( _T("Jpeg2000-Options") ) )
			{
				XmlUtils::CXmlNode oSaveNode;
				if ( oMainNode.GetNode( _T("SaveOptions"), oSaveNode ) )
				{
					XmlUtils::CXmlNode oCurNode;
					CString sValue;

					// ComponentsCount
					if ( oSaveNode.GetNode( _T("ComponentsCount"), oCurNode ) )
					{
						pParameters->nComponentsCount = XmlUtils::GetInteger( oCurNode.GetAttribute( _T("value"), _T("3") ) );
					}


					// Format
					if ( oSaveNode.GetNode( _T("Format"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("value"), _T("0") );
						nFormat = _wtoi( sValue.GetBuffer() ); 
					}

					// Tile
					if ( oSaveNode.GetNode( _T("Tile"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("width"), _T("32") );
						pParameters->nXTsiz = max( 32, _wtoi( sValue.GetBuffer() ) );
						sValue = oCurNode.GetAttribute( _T("height"), _T("32") );
						pParameters->nYTsiz = max( 32, _wtoi( sValue.GetBuffer() ) );

						pParameters->bTileSizeOn = true;

						sValue = oCurNode.GetAttribute( _T("offsetx"), _T("0") );
						pParameters->nXTOsiz = _wtoi( sValue.GetBuffer() );
						sValue = oCurNode.GetAttribute( _T("offsety"), _T("0") );
						pParameters->nYTOsiz = _wtoi( sValue.GetBuffer() );
					}

					// Resolution
					if ( oSaveNode.GetNode( _T("Resolution"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("levelscount"), _T("6") );
						pParameters->nResolutionsCount = max( 1, min ( J2K_MAXRLVLS, _wtoi( sValue.GetBuffer() ) ) );
					}

					// PrecinctDimension
					if ( oSaveNode.GetNode( _T("PrecinctDimension"), oCurNode ) )
					{
						sValue = oCurNode.GetText();
						wchar_t *wsTemp = sValue.GetBuffer();

						char nSeparateChar = 0;
						int nPrecinctsCount = 0;
						do 
						{
							nSeparateChar = 0;
							swscanf( wsTemp, _T("[%d,%d]%c"), &pParameters->anPrecinctWidthInit[nPrecinctsCount], &pParameters->anPrecinctHeightInit[nPrecinctsCount], &nSeparateChar );
							pParameters->nCodingStyle |= 0x01;
							nPrecinctsCount++;
							if ( J2K_MAXRLVLS == nPrecinctsCount )
								break;
							wsTemp = wcspbrk( wsTemp, _T("]") ) + 2;
						}
						while ( ',' == nSeparateChar );
						pParameters->nPrecinctCount = nPrecinctsCount;
					}

					// CodeBlockDimension
					if ( oSaveNode.GetNode( _T("CodeBlockDimension"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("width"), _T("64") );
						pParameters->nCodeBlockWidthInit = max( 4, min( 4096, _wtoi( sValue.GetBuffer() ) ) ); 

						sValue = oCurNode.GetAttribute( _T("height"), _T("64") );
						pParameters->nCodeBlockHeightInit = max( 4, min( 4096, _wtoi( sValue.GetBuffer() ) ) ); 
					}

					// ProgressionOrder
					if ( oSaveNode.GetNode( _T("ProgressionOrder"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("value"), _T("0") );
						int nProgOrder = max( 0, min( 4, _wtoi( sValue.GetBuffer() ) ) );
						switch( nProgOrder )
						{
						case 0: pParameters->eProgOrder = poLRCP; break;
						case 1: pParameters->eProgOrder = poRLCP; break;
						case 2: pParameters->eProgOrder = poRPCL; break;
						case 3: pParameters->eProgOrder = poPCRL; break;
						case 4: pParameters->eProgOrder = poCPRL; break;
						}
					}

					// Subsampling
					if ( oSaveNode.GetNode( _T("Subsampling"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("dx"), _T("1") );
						pParameters->nSubSamplingDx = max( 1, min( J2K_MAXSSFTR, _wtoi( sValue.GetBuffer() ) ) );

						sValue = oCurNode.GetAttribute( _T("dy"), _T("1") );
						pParameters->nSubSamplingDy = max( 1, min( J2K_MAXSSFTR, _wtoi( sValue.GetBuffer() ) ) );
					}

					// SOPmarker
					if ( oSaveNode.GetNode( _T("SOPmarker"), oCurNode ) )
					{
						// Пока отключим, потом надо будет добавить - для этого нужно выделять память для стрктуры ImageInfo
						//pParameters->nCodingStyle |= 0x02;
					}

					// EPHmarker
					if ( oSaveNode.GetNode( _T("EPHmarker"), oCurNode ) )
					{
						pParameters->nCodingStyle |= 0x04;
					}

					// ROI
					if ( oSaveNode.GetNode( _T("ROI"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("componentindex"), _T("-1") );
						pParameters->nROIComponentIndex = max( -1, _wtoi( sValue.GetBuffer() ) );

						sValue = oCurNode.GetAttribute( _T("shift"), _T("0") );
						pParameters->nROIShift = max( 0, min( 37, _wtoi( sValue.GetBuffer() ) ) );
					}

					// Comment
					if ( oSaveNode.GetNode( _T("Comment"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("value") );
						// TO DO: Неправильное копирование строки
						USES_CONVERSION;
						pParameters->sComment = W2A( sValue.GetBuffer() );
					}

					// Compression
					if ( oSaveNode.GetNode( _T("Compression"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("lossless"), _T("1") );
						pParameters->nTransformId = _wtoi( sValue.GetBuffer() ) ? 0 : 1;
					}

					// Rate-Disto
					if ( oSaveNode.GetNode( _T("Rate-Disto"), oCurNode ) )
					{
						sValue = oCurNode.GetText();

						wchar_t *wsTemp = sValue.GetBuffer();
						
						while ( swscanf( wsTemp, _T("%f"), &pParameters->afRates[pParameters->nLayersCount] ) == 1 ) 
						{
							pParameters->nLayersCount++;
							while ( *wsTemp && *wsTemp != _T(',') ) 
							{
								wsTemp++;
							}
							if ( !*wsTemp )
								break;
							wsTemp++;
						}
						pParameters->nDistoAlloc = 1;	
					}

					// Fixed-Quality
					if ( oSaveNode.GetNode( _T("Fixed-Quality"), oCurNode ) )
					{
						sValue = oCurNode.GetText();

						wchar_t *wsTemp = sValue.GetBuffer();

						while ( swscanf( wsTemp, _T("%f"), &pParameters->afDistoratio[pParameters->nLayersCount] ) == 1 ) 
						{
							pParameters->nLayersCount++;
							while ( *wsTemp && *wsTemp != ',' ) 
							{
								wsTemp++;
							}
							if ( !*wsTemp )
								break;
							wsTemp++;
						}
						pParameters->nFixedQuality = 1;
					}

					// Fixed-Layer
					if ( oSaveNode.GetNode( _T("Fixed-Layer"), oCurNode ) )
					{
						sValue = oCurNode.GetAttribute( _T("layerscount"), _T("1") );
						int nLayersCount = max( 1, _wtoi( sValue.GetBuffer() ) );
						pParameters->nLayersCount = nLayersCount;

						int nResolutionsCount = pParameters->nResolutionsCount;
						int nMatrixWidth = nResolutionsCount * 3;
						pParameters->pMatrix = (int*)Malloc(0, nLayersCount * nMatrixWidth * sizeof(int) );

						sValue = oCurNode.GetText();
						wchar_t *wsTemp = sValue.GetBuffer();

						for ( int nLayerIndex = 0; nLayerIndex < nLayersCount; nLayerIndex++ ) 
						{
							int *pRow = &pParameters->pMatrix[nLayerIndex * nMatrixWidth];
							int *pCol = pRow;

							pParameters->afRates[nLayerIndex] = 1;
							
							swscanf( wsTemp, _T("%d,"), &pCol[0] );
							wsTemp += 2;

							if ( pCol[0] > 9 )
								wsTemp++;

							pCol[1] = 0;
							pCol[2] = 0;

							for ( int nResolutionIndex = 1; nResolutionIndex < nResolutionsCount; nResolutionIndex++ ) 
							{
								pCol += 3;
								swscanf( wsTemp, _T("%d,%d,%d"), &pCol[0], &pCol[1], &pCol[2] );
								wsTemp += 6;
								if ( pCol[0] > 9 )
									wsTemp++;
								if ( pCol[1] > 9 )
									wsTemp++;
								if ( pCol[2] > 9 )
									wsTemp++;
							}
							if ( nLayerIndex < nLayersCount - 1 )
								wsTemp++;
						}
						
						pParameters->nFixedAlloc = 1;

					}
				}

			}
		}


		// Проверим кооректность введенных параметров

		// Параметры nDistoAlloc, nFixedQuality, nFixedAlloc нельзя использовать вместе
		if ( ( pParameters->nDistoAlloc || pParameters->nFixedAlloc || pParameters->nFixedQuality  ) && ( !( pParameters->nDistoAlloc ^ pParameters->nFixedAlloc ^ pParameters->nFixedQuality ) ) ) 
			return -1;

		// Если параметры не заданы, тогда по умолчанию ставим компрессию без потерь
		if ( 0 == pParameters->nLayersCount )
		{
			pParameters->afRates[0]   = 0;
			pParameters->nLayersCount = 1;
			pParameters->nDistoAlloc  = 1;
		}
		if ( pParameters->nXTOsiz > pParameters->nImageOffsetX0 || pParameters->nYTOsiz > pParameters->nImageOffsetY0 )
			return -1;

		for ( int nIndex = 0; nIndex < pParameters->nPOCsCount; nIndex++ )
		{
			if ( -1 == pParameters->aoPOC[nIndex].ePpoc )
			{
				// TO DO: Выдать ошибку, что порядок не задан
			}
		}

		return nFormat;
	}

public:

	STDMETHOD(J2kToInterface)(BSTR bsSrcPath, IUnknown **ppImage, BSTR bsXmlOptions);
	STDMETHOD(InterfaceToJ2k)(IUnknown **ppImage, BSTR bsDstPath, BSTR bsXmlOptions);
	STDMETHOD(MakeSavePreview)(IUnknown **ppSrcImage, IUnknown **pDstImage, BSTR bsXmlOptions)
	{
		return S_OK;
	}
	STDMETHOD(GetFileInfo)(BSTR bsSrcPath, BSTR *pbsXmlFileInfo)
	{
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR bsParamName, VARIANT vParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR bsParamName, VARIANT* pvParamValue)
	{
		return S_OK;
	}

	HRESULT Error(long Code)
	{
		LPCOLESTR lpszDesc;
		HRESULT hRes;
		switch(Code)
		{
			case NOERROR:				return S_OK;
			case BUSY:					hRes=AVS_ERROR_BUSY;lpszDesc=c_pszErrorBusy;break;
			case MEMORY:				hRes=AVS_ERROR_MEMORY;lpszDesc=c_pszErrorMemory;break;
			case FILEACCESS:			hRes=AVS_ERROR_FILEACCESS;lpszDesc=c_pszErrorFileAccess;break;			
			case INVALIDARGUMENT:		hRes=AVS_ERROR_INVALIDARGUMENT;lpszDesc=c_pszErrorInvalidArgument;break;
			case CONTROLNOTINSTALLED:	hRes=AVS_ERROR_CONTROLNOTINSTALLED;lpszDesc=c_pszErrorControlNotInstalled;break;
			case INDEXOUTOFRANGE:		hRes=AVS_ERROR_INDEXOUTOFRANGE;lpszDesc=c_pszErrorIndexOutOfRange;break;			
			case FILEFORMAT:			hRes=AVS_ERROR_FILEFORMAT;lpszDesc=c_pszErrorFileFormat;break;
			case UNEXPECTED:			
			default:					hRes=AVS_ERROR_UNEXPECTED;lpszDesc=c_pszErrorUnexpected;break;
		}

		AtlTrace(L"ERROR %d (%s)", Code, lpszDesc);

		return AtlReportError(GetObjectCLSID(), lpszDesc, __uuidof(IJ2kFile), hRes);
	}




};

