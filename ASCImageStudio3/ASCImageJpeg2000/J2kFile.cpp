// J2kFile.cpp : Implementation of CJ2kFile

#include "stdafx.h"

#include "J2kFile.h"
#include "J2kIncludes.h"
#include "Utils.h"

extern Image* J2kToImage(unsigned char *pSrc, DWORD nFileSize, DecoderParams *pDecoderParams);
extern Image* J2kToImage(CString sFileName, DecoderParams *pDecoderParams);

extern Image* Jp2ToImage(unsigned char *pSrc, DWORD nFileSize, DecoderParams *pDecoderParams);
extern Image* Jp2ToImage(CString sFileName, DecoderParams *pDecoderParams);

extern Image* JptToImage(unsigned char *pSrc, DWORD nFileSize, DecoderParams *pDecoderParams);

extern Image* Mj2ToImage(unsigned char *pSrc, DWORD nFileSize, DecoderParams *pDecoderParams);

extern void   Image_Destroy(Image *pImage) ;
extern Image* Image_Create(int nComponentsCount, ImageComponentParams *pCompParams, ColorSpace eColorSpace) ;

extern bool   ImageToJ2k(Image *pImage, const char* sFilePath, EncoderParams *pEncoderParams);
extern bool   ImageToJ2p(Image *pImage, const char* sFilePath, EncoderParams *pEncoderParams);

void* Malloc(DWORD dwFlags, size_t nSize) 
{
	void *pMemBlock = HeapAlloc(GetProcessHeap(),dwFlags, nSize );

	if ( !pMemBlock )
	{
		AtlTrace("Memory !!\n");
	}
	return pMemBlock;
}
void* ReAlloc(void* Memory,size_t nSize,size_t nNewSize)
{
	if (!Memory) return Malloc(HEAP_ZERO_MEMORY,nNewSize);

	if (nNewSize> nSize)
	{
		void *pMemBlock = HeapAlloc(GetProcessHeap(),0, nNewSize );

		if ( !pMemBlock )
		{
			AtlTrace("Memory !!\n");
			return NULL;
		}
		memcpy(pMemBlock,Memory,nSize);
		HeapFree(GetProcessHeap(),0,Memory);
		Memory = NULL;
		return pMemBlock;
	}
	else
	{
		return Memory;
	}
}
int check_j2000_type(HANDLE hFile)
{
	if (!hFile || hFile == INVALID_HANDLE_VALUE)return 0;

	BYTE *pBuffer = (BYTE*)HeapAlloc(GetProcessHeap(),0,1024);
	if (pBuffer == NULL) return 0;
	
	DWORD dwBytes=0;
	ReadFile(hFile,pBuffer,1024,&dwBytes,NULL);

	int type =0;
	if ( (32 <= dwBytes) &&(0x00 == pBuffer[0] && 0x00 == pBuffer[1]  && 0x00 == pBuffer[2] && 0x0c == pBuffer[3]
						 && 0x6a == pBuffer[4] && 0x50 == pBuffer[5]  && 0x20 == pBuffer[6] && 0x20 == pBuffer[7]
						 
						 && 0x0d == pBuffer[8] && 0x0a == pBuffer[9]  && 0x87 == pBuffer[10] && 0x0a == pBuffer[11]
						&& 0x00 == pBuffer[12] && 0x00 == pBuffer[13]  && 0x00 == pBuffer[14] /*&&  (0x14 == pBuffer[15] || 0x18 == pBuffer[15] )*/
						 
						 && 0x66 == pBuffer[16] && 0x74 == pBuffer[17]  && 0x79 == pBuffer[18] && 0x70 == pBuffer[19]
						 && 0x6a == pBuffer[20] && 0x70 == pBuffer[21]  && 0x32 == pBuffer[22] && 0x20 == pBuffer[23]
						 
						 && 0x00 == pBuffer[24] && 0x00 == pBuffer[25]  && 0x00 == pBuffer[26] && 0x00 == pBuffer[27]
						 /*&& 0x6a == pBuffer[28] && 0x70 == pBuffer[29]  && 0x32 == pBuffer[30] && 0x20 == pBuffer[31]*/))
	{
		type = 1;
	}
	if ( (4 <= dwBytes) && (0xff == pBuffer[0] && 0x4f == pBuffer[1]  && 0xff == pBuffer[2] && 0x51 == pBuffer[3]))
	{
		type = 2;
	}
	if ( (32 <= dwBytes) &&(0x00 == pBuffer[0] && 0x00 == pBuffer[1]  && 0x00 == pBuffer[2] && 0x0c == pBuffer[3]
						 && 0x6a == pBuffer[4] && 0x50 == pBuffer[5]  && 0x20 == pBuffer[6] && 0x20 == pBuffer[7]
						 
						 && 0x0d == pBuffer[8] && 0x0a == pBuffer[9]  && 0x87 == pBuffer[10] && 0x0a == pBuffer[11]
						 && 0x00 == pBuffer[12] && 0x00 == pBuffer[13]  && 0x00 == pBuffer[14] && 0x18 == pBuffer[15]
						 
						 && 0x66 == pBuffer[16] && 0x74 == pBuffer[17]  && 0x79 == pBuffer[18] && 0x70 == pBuffer[19]
						 && 0x6d == pBuffer[20] && 0x6a == pBuffer[21]  && 0x70 == pBuffer[22] && 0x32 == pBuffer[23]
						 
						 && 0x00 == pBuffer[24] && 0x00 == pBuffer[25]  && 0x00 == pBuffer[26] && 0x00 == pBuffer[27]
						 && 0x6d == pBuffer[28] && 0x6a == pBuffer[29]  && 0x70 == pBuffer[30] && 0x32 == pBuffer[31]))
	{
		type = 3;
	}
	
	HeapFree(GetProcessHeap(),0,pBuffer);	
	SetFilePointer(hFile,0,0,FILE_BEGIN);
	return type;
}
// CJ2kFile

STDMETHODIMP CJ2kFile::J2kToInterface(BSTR bsSrcPath, IUnknown **ppImage, BSTR bsXmlOptions)
{

	Image *pImage = NULL;
	bool bOpenResult = FALSE;

	DecoderParams oParameters;
	
	ApplyDecoderOptions( &oParameters, CString(bsXmlOptions) );// Установим стандартные значения параметров
		
///////////////////////////////////////////////////////////////////////////////////

	CString sSrcPath( bsSrcPath );

	// открываем файл
	HANDLE hFile = CreateFile( sSrcPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if (INVALID_HANDLE_VALUE == hFile)
		return Error(FILEACCESS); // Невозможно открыть файл

	DWORD  nFileSize = GetFileSize( hFile, NULL );

	int type = check_j2000_type(hFile);

	if (nFileSize > 10000 * 1024 ) // большие файлы НЕ НАДО маппить
	{
		if ( !bOpenResult && type == 1)
			bOpenResult = ( NULL != ( pImage = Jp2ToImage(CString(bsSrcPath), &oParameters ) ) );	

		if ( !bOpenResult && type == 2)
			bOpenResult = ( NULL != ( pImage = J2kToImage(CString(bsSrcPath), &oParameters ) ) );
	}


	if ( !bOpenResult )
	{
	// мапим этот файл в память - так быстрее читаются данные из файла ... из МАЛЕНЬКОГО файла
		HANDLE hMapFile  = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
		if (NULL == hMapFile)
		{
			CloseHandle( hFile );
			return Error(MEMORY); // Невозможно создать отображение файла
		}

		// создаем view of file
		VOID* pBaseAddress	= MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
		BYTE* pnFile		= (BYTE*)pBaseAddress;
		unsigned char *pSrc = (unsigned char *) HeapAlloc(GetProcessHeap(),0, nFileSize );
		if ( !pSrc )
		{
			UnmapViewOfFile( pBaseAddress );
			CloseHandle( hMapFile );
			CloseHandle( hFile );
			return Error(MEMORY);
		}
		memcpy( pSrc, pnFile, nFileSize );

		UnmapViewOfFile( pBaseAddress );
		CloseHandle( hMapFile );

		OnProgress( 0, 0 );
		
		if ( !bOpenResult && type == 1)
			bOpenResult = ( NULL != ( pImage = Jp2ToImage( pSrc, nFileSize, &oParameters ) ) );		
		if ( !bOpenResult && type == 2)
			bOpenResult = ( NULL != ( pImage = J2kToImage( pSrc, nFileSize, &oParameters ) ) );
		if ( !bOpenResult && type == 3 )
			bOpenResult = ( NULL != ( pImage = Mj2ToImage( pSrc, nFileSize, &oParameters ) ) );
		if ( !bOpenResult && type == 4 )
			bOpenResult = ( NULL != ( pImage = JptToImage( pSrc, nFileSize, &oParameters ) ) );

		RELEASEHEAP( pSrc );
	}
	CloseHandle( hFile );

	OnProgress( 0, PROGRESS_MAX - 1 ); 

	if ( !bOpenResult )
	{
		Image_Destroy( pImage );
		return Error(FILEFORMAT);
	}

	if ( oParameters.bConvertToRGBA )
	{
		// Подготавливаем pInterface
		IUnknown **pInterface = (IUnknown**)ppImage;
		if ( !pInterface )
		{
			Image_Destroy( pImage );
			return Error(MEMORY);
		}

		int nWidth  = pImage->pComponents[0].nWidth;
		int nHeight = pImage->pComponents[0].nHeight;
		int nBufferSize = 4 /*pImage->nCsiz*/ * nWidth * nHeight;
		
		if ( nBufferSize < 1 )
		{
			Image_Destroy( pImage );		
			return Error(FILEFORMAT);
		}		
		
		// create result interface
		*pInterface = NULL;
		HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)pInterface );
		if ( hRes != S_OK || NULL == *pInterface )
		{
			Image_Destroy( pImage );		
			return Error(CONTROLNOTINSTALLED);
		}
		MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)*pInterface;

		pMediaData->put_ColorSpace( ( 1 << 6) | ( 1 << 31 ) ); // CPS_BGRA | CPS_FLIP
		pMediaData->put_Width( nWidth );
		pMediaData->put_Height( nHeight );
		pMediaData->put_AspectRatioX( nWidth );
		pMediaData->put_AspectRatioY( nHeight );
		pMediaData->put_Interlaced( VARIANT_FALSE );
		pMediaData->put_Stride( 0, 4 * nWidth );
		pMediaData->AllocateBuffer( nBufferSize );

		unsigned char *pBufferPtr = NULL;
		long nCreatedBufferSize = 0;

		pMediaData->get_Buffer( &pBufferPtr );
		pMediaData->get_BufferSize( &nCreatedBufferSize );
		pMediaData->put_Plane( 0, pBufferPtr );

		if ( !pBufferPtr || nCreatedBufferSize != nBufferSize )
		{
			Image_Destroy( pImage );
			return Error(MEMORY);
		}
		*pInterface = pMediaData;

		// Пишем данные в pBufferPtr

		if ( pImage->nCsiz == 3 && pImage->pComponents[0].nXRsiz == pImage->pComponents[1].nXRsiz && pImage->pComponents[1].nXRsiz == pImage->pComponents[2].nXRsiz
			&& pImage->pComponents[0].nYRsiz == pImage->pComponents[1].nYRsiz && pImage->pComponents[1].nYRsiz == pImage->pComponents[2].nYRsiz
			&& pImage->pComponents[0].nPrecision == pImage->pComponents[1].nPrecision && pImage->pComponents[1].nPrecision == pImage->pComponents[2].nPrecision ) 
		{	    
			int nResW = CeilDivPow2( pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2 );
			int nResH = CeilDivPow2( pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2 );


			for ( int nIndex = 0; nIndex < nResW * nResH; nIndex++ ) 
			{
				unsigned char nR = pImage->pComponents[0].pData[nWidth * nResH - ((nIndex) / (nResW) + 1) * nWidth + (nIndex) % (nResW)];
				unsigned char nG = pImage->pComponents[1].pData[nWidth * nResH - ((nIndex) / (nResW) + 1) * nWidth + (nIndex) % (nResW)];
				unsigned char nB = pImage->pComponents[2].pData[nWidth * nResH - ((nIndex) / (nResW) + 1) * nWidth + (nIndex) % (nResW)];

				pBufferPtr[0] = nB;
				pBufferPtr[1] = nG;
				pBufferPtr[2] = nR;
				pBufferPtr[3] = 255;
				pBufferPtr += 4;

			}
		} 
		else if ( pImage->nCsiz >= 4 && pImage->pComponents[0].nXRsiz == pImage->pComponents[1].nXRsiz && pImage->pComponents[1].nXRsiz == pImage->pComponents[2].nXRsiz && pImage->pComponents[2].nXRsiz == pImage->pComponents[3].nXRsiz
			&& pImage->pComponents[0].nYRsiz == pImage->pComponents[1].nYRsiz && pImage->pComponents[1].nYRsiz == pImage->pComponents[2].nYRsiz && pImage->pComponents[2].nYRsiz == pImage->pComponents[3].nYRsiz
			&& pImage->pComponents[0].nPrecision == pImage->pComponents[1].nPrecision && pImage->pComponents[1].nPrecision == pImage->pComponents[2].nPrecision && pImage->pComponents[2].nPrecision == pImage->pComponents[3].nPrecision ) 
		{	    
			int nResW = CeilDivPow2( pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2 );
			int nResH = CeilDivPow2( pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2 );


			for ( int nIndex = 0; nIndex < nResW * nResH; nIndex++ ) 
			{
				unsigned char nR = pImage->pComponents[0].pData[nWidth * nResH - ((nIndex) / (nResW) + 1) * nWidth + (nIndex) % (nResW)];
				unsigned char nG = pImage->pComponents[1].pData[nWidth * nResH - ((nIndex) / (nResW) + 1) * nWidth + (nIndex) % (nResW)];
				unsigned char nB = pImage->pComponents[2].pData[nWidth * nResH - ((nIndex) / (nResW) + 1) * nWidth + (nIndex) % (nResW)];
				unsigned char nA = pImage->pComponents[3].pData[nWidth * nResH - ((nIndex) / (nResW) + 1) * nWidth + (nIndex) % (nResW)];

				pBufferPtr[0] = nB;
				pBufferPtr[1] = nG;
				pBufferPtr[2] = nR;
				pBufferPtr[3] = nA;
				pBufferPtr += 4;

			}
		} 
		else // Grayscale
		{
			int nResW = CeilDivPow2( pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2 );
			int nResH = CeilDivPow2( pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2 );

			for ( int nIndex = 0; nIndex < nResW * nResH; nIndex++ ) 
			{
				unsigned char nG = pImage->pComponents[0].pData[nWidth * nResH - ((nIndex) / (nResW) + 1) * nWidth + (nIndex) % (nResW)];
				pBufferPtr[0] = nG;
				pBufferPtr[1] = nG;
				pBufferPtr[2] = nG;
				pBufferPtr[3] = 255;
				pBufferPtr += 4;
			}
		}
	}
	else
	{
		// Подготавливаем pInterface
		IUnknown **pInterface = (IUnknown**)ppImage;
		if ( !pInterface )
		{
			Image_Destroy( pImage );
			return Error(MEMORY);
		}

		// create result interface
		*pInterface = NULL;
		HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)pInterface );
		if ( hRes != S_OK || NULL == *pInterface )
		{
			Image_Destroy( pImage );			
			return Error(CONTROLNOTINSTALLED);
		}

		MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)*pInterface;

		int nWidth  = pImage->pComponents[0].nWidth;
		int nHeight = pImage->pComponents[0].nHeight;
		int nBufferSize = pImage->nCsiz * nWidth * nHeight;
		if ( nBufferSize < 1 )
		{
			Image_Destroy( pImage );
			return Error(FILEFORMAT);
		}
		pMediaData->put_ColorSpace(( 1 << 31 ) | ( 1 << 32 )); 
		pMediaData->put_Width( nWidth );
		pMediaData->put_Height( nHeight );
		pMediaData->put_AspectRatioX( nWidth );
		pMediaData->put_AspectRatioY( nHeight );
		pMediaData->put_Interlaced( VARIANT_FALSE );
		pMediaData->put_Stride( 0, pImage->nCsiz * nWidth );
		pMediaData->AllocateBuffer( nBufferSize );

		unsigned char *pBufferPtr = NULL;
		long nCreatedBufferSize = 0;

		pMediaData->get_Buffer( &pBufferPtr );
		pMediaData->get_BufferSize( &nCreatedBufferSize );
		pMediaData->put_Plane( 0, pBufferPtr );

		if ( !pBufferPtr || nCreatedBufferSize != nBufferSize )
		{
			Image_Destroy( pImage );
			return Error(MEMORY);
		}
		*pInterface = pMediaData;

		// Пишем данные в pBufferPtr

		bool bFlag = true;

		if ( pImage->nCsiz > 1 )
		{
			for ( int nIndex = 1; nIndex < pImage->nCsiz; nIndex++ )
			{
				if ( pImage->pComponents[nIndex - 1].nXRsiz     != pImage->pComponents[nIndex].nXRsiz ||
					 pImage->pComponents[nIndex - 1].nYRsiz     != pImage->pComponents[nIndex].nYRsiz ||
					 pImage->pComponents[nIndex - 1].nPrecision != pImage->pComponents[nIndex].nPrecision )
				{
					 bFlag = false;
				}
			}
		}

		if ( bFlag ) 
		{	    
			int nResW = CeilDivPow2( pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2 );
			int nResH = CeilDivPow2( pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2 );

			for ( int nIndex = 0; nIndex < nResW * nResH; nIndex++ ) 
			{
				for ( int nComponent = 0; nComponent < pImage->nCsiz; nComponent++ )
				{
					unsigned char unChar = pImage->pComponents[nComponent].pData[nWidth * nResH - ((nIndex) / (nResW) + 1) * nWidth + (nIndex) % (nResW)];
					pBufferPtr[0] = unChar;
					pBufferPtr++;
				}
			}
		} 
		else // Grayscale
		{
			int nResW = CeilDivPow2( pImage->pComponents[0].nWidth, pImage->pComponents[0].nFactorDiv2 );
			int nResH = CeilDivPow2( pImage->pComponents[0].nHeight, pImage->pComponents[0].nFactorDiv2 );

			for ( int nIndex = 0; nIndex < nResW * nResH; nIndex++ ) 
			{
				for ( int nComponent = 0; nComponent < pImage->nCsiz; nComponent++ )
				{
					unsigned char unChar = pImage->pComponents[nComponent].pData[nWidth * nResH - ((nIndex) / (nResW) + 1) * nWidth + (nIndex) % (nResW)];
					pBufferPtr[0] = unChar;
					pBufferPtr++;
				}
			}
		}
	}

	Image_Destroy( pImage );
	OnProgress( 0, PROGRESS_MAX ); 
	return S_OK;
}

STDMETHODIMP CJ2kFile::InterfaceToJ2k(IUnknown **ppImage, BSTR bsDstPath, BSTR bsXmlOptions)
{
	CStringA sFilePath( bsDstPath );

	if (!ppImage || !*ppImage)
		return Error(MEMORY);

	int nWidth = 0;
	int nHeight = 0;
	BYTE* pPixels = 0;

	MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;
	if ( NULL == ppImage || NULL == (*ppImage) )
		return Error(MEMORY);

	MediaCore::IAVSUncompressedVideoFrame* pMediaDataIn = NULL;
	(*ppImage)->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaDataIn));
	if ( NULL == pMediaDataIn )
		return Error(MEMORY);

	LONG lWidth = 0;   pMediaDataIn->get_Width(&lWidth);
	LONG lHeight = 0;  pMediaDataIn->get_Height(&lHeight);
	LONG lAspectX = 0; pMediaDataIn->get_AspectRatioX(&lAspectX);
	LONG lAspectY = 0; pMediaDataIn->get_AspectRatioY(&lAspectY);

	// преобразуем к BGR формату
	if (TRUE)
	{
		MediaFormat::IAVSVideoFormat* pMediaFormat = NULL;
		CoCreateInstance(MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)(&pMediaFormat));
		if (NULL == pMediaFormat)
		{
			pMediaDataIn->Release();
			return Error(MEMORY);
		}

		MediaCore::IAVSVideoFrameTransform* pMediaTransform = NULL;
		CoCreateInstance(MediaCore::CLSID_CAVSVideoFrameTransform, NULL, CLSCTX_ALL, MediaCore::IID_IAVSVideoFrameTransform, (void**)(&pMediaTransform));
		if (NULL == pMediaTransform)
		{
			pMediaDataIn->Release();
			pMediaFormat->Release();
			return Error(MEMORY);
		}

		pMediaFormat->SetDefaultProperties();
		pMediaFormat->put_Width(lWidth);
		pMediaFormat->put_Height(lHeight);
		pMediaFormat->put_AspectRatioX(lAspectX);
		pMediaFormat->put_AspectRatioY(lAspectY);
		pMediaFormat->put_ColorSpace(1 << 6);//CSP_BGRA

		pMediaTransform->SetVideoFormat(pMediaFormat);
		
		IUnknown *pTransformResult = NULL;
		pMediaTransform->TransformFrame(pMediaDataIn, &pTransformResult);
		if (NULL != pTransformResult)
		{
			if ( (*ppImage)==pTransformResult )
			{
				MediaCore::IAVSMediaData *pData = NULL;
				pTransformResult->QueryInterface(MediaCore::IID_IAVSMediaData, (void**)(&pData));
				if (NULL!=pData)
				{
					MediaCore::IAVSMediaData *pmdOutFrame = NULL;
					pData->CreateDuplicate(2, &pmdOutFrame);
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
	}

	pMediaDataIn->Release();

	if (NULL == pMediaData)
		return Error(MEMORY);

	// 
	lWidth = 0;              pMediaData->get_Width(&lWidth);
	lHeight = 0;             pMediaData->get_Height(&lHeight);
	LONG lColorSpace = 0;    pMediaData->get_ColorSpace(&lColorSpace);
	LONG lBufferSize = 0;    pMediaData->get_BufferSize(&lBufferSize);
	BYTE* pSourceBuffer = 0; pMediaData->get_Buffer(&pSourceBuffer);

	EncoderParams oParameters;

	// Далее обрабатываем Xml с параметрами компрессии

	int nFormat = ApplyEncoderOptions( &oParameters, CString(bsXmlOptions) );

	// TO DO: Добавить возможность записи альфа-канала
	ImageComponentParams aComponentParams[3]; // Пока пусть будет максимально три компоненты (RGB)
	Image *pImage = NULL;
	int nComponentsCount = oParameters.nComponentsCount;

	memset( &aComponentParams[0], 0, sizeof(ImageComponentParams) );
	for ( int nIndex = 0; nIndex < nComponentsCount; nIndex++ )
	{
		aComponentParams[nIndex].nPrecision = 8;
		aComponentParams[nIndex].nBPP       = 8;
		aComponentParams[nIndex].nSigned    = 0;
		aComponentParams[nIndex].nXRsiz     = oParameters.nSubSamplingDx;
		aComponentParams[nIndex].nYRsiz     = oParameters.nSubSamplingDy;
		aComponentParams[nIndex].nWidth     = (int)lWidth;
		aComponentParams[nIndex].nHeight    = (int)lHeight;
	}

	// Создаем структуру Image
	pImage = Image_Create( nComponentsCount, &aComponentParams[0], csRGB );
	if ( !pImage )
		return Error(MEMORY);

	pImage->nXOsiz = oParameters.nImageOffsetX0;
	pImage->nYOsiz = oParameters.nImageOffsetY0;
	pImage->nXsiz  = ( !pImage->nXOsiz ) ? (lWidth  - 1) * oParameters.nSubSamplingDx + 1 : pImage->nXOsiz + (lWidth  - 1) * oParameters.nSubSamplingDy + 1;
	pImage->nYsiz  = ( !pImage->nYOsiz ) ? (lHeight - 1) * oParameters.nSubSamplingDy + 1 : pImage->nYOsiz + (lHeight - 1) * oParameters.nSubSamplingDy + 1;

	if ( 3 == nComponentsCount )
	{
		int nIndex = 0;
		for ( int nY = 0; nY < (int)lHeight; nY++ )
		{
			for ( int nX = 0; nX < (int)lWidth; nX++, pSourceBuffer += 4 )
			{
				pImage->pComponents[0].pData[nIndex] = pSourceBuffer[2];
				pImage->pComponents[1].pData[nIndex] = pSourceBuffer[1];
				pImage->pComponents[2].pData[nIndex] = pSourceBuffer[0];
				
				// TO DO: Добавить записись альфа-канала
				//pImage->pComponents[3].pData[nIndex] = 255;//pSourceBuffer[0];
				////pImage->pComponents[4].pData[nIndex] = 128;//pSourceBuffer[0];
				////pImage->pComponents[5].pData[nIndex] = 128;//pSourceBuffer[0];

				nIndex++;
			}
		}
	}
	else if ( 1 == nComponentsCount )
	{
		int nIndex = 0;
		for ( int nY = 0; nY < (int)lHeight; nY++ )
		{
			for ( int nX = 0; nX < (int)lWidth; nX++, pSourceBuffer += 4 )
			{
				pImage->pComponents[0].pData[nIndex] = pSourceBuffer[0];
				nIndex++;
			}
		}
	}
	else
	{
		pMediaData->Release();
		Image_Destroy( pImage );
		return Error(MEMORY);
	}

	bool bRes = false;
	switch ( nFormat )
	{
	case 0: bRes = ImageToJ2k( pImage, sFilePath.GetBuffer(), &oParameters ); break;
	case 1: bRes = ImageToJ2p( pImage, sFilePath.GetBuffer(), &oParameters ); break;
	case -1:
	default: bRes = false;
	}
	Image_Destroy( pImage );


	return bRes? S_OK : Error(FILEFORMAT);
}
