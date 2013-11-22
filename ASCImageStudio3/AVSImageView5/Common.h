#pragma once

#include <comdef.h>

#include <gdiplus.h>

/*
#include <MediaFormatDefine.h>
#include <MediaDataDefine.h>

#include <emmintrin.h>		// MMX, SSE, SSE2 intrinsic support

const DWORD _MMX_FEATURE_BIT	= 0x00800000;
const DWORD _SSE_FEATURE_BIT	= 0x02000000;								    
const DWORD _SSE2_FEATURE_BIT	= 0x04000000;

*/

namespace Core
{
	/*
	class CheckFeatures
	{
	public:

	CheckFeatures ()
	{
	m_FeatureMMX	= false;
	m_FeatureSSE	= false;
	m_FeatureSSE2	= false;
	}

	void Analyze ()
	{
	m_FeatureMMX	= IsMMX ();
	m_FeatureSSE	= IsSSE ();
	m_FeatureSSE2	= IsSSE2 ();

	#ifdef _DEBUG
	ATLTRACE("MMX : %s SSE : %s SSE2 : %s\n",	m_FeatureMMX == true ?	_T("available") : _T("disable"),
	m_FeatureSSE == true ?	_T("available") : _T("disable"),
	m_FeatureSSE2 == true ?	_T("available") : _T("disable")	);
	#endif
	}

	//void SetAlphaChanelValue ( byte Value, long PixelsCount, unsigned long* pSrc )
	//{
	//	if ( m_FeatureSSE2 )
	//	{
	//		if ( PixelsCount < 4 )
	//			return;

	//		unsigned mask	= ( Value << 24 | 0x00FFFFFF );
	//		__m128i nmask	= _mm_set_epi32 ( mask, mask, mask, mask );
	//		__m128i* source	= (__m128i*)pSrc;

	//		long count		= PixelsCount / 4;

	//		for ( long i = 0; i < count; ++i )
	//		{
	//			__m128i xmm1 = _mm_loadu_si128 ( source );
	//			__m128i xmm2 = _mm_min_epu8 ( nmask, xmm1 );
	//			__m128i xmm3 = _mm_and_si128 ( xmm1, xmm2 );
	//			_mm_store_si128 ( source, xmm3 );
	//			++source;
	//		}
	//	}
	//	else
	//	{
	//		unsigned long AlphaMask	= (unsigned long)(Value<<24);
	//		for ( long i = 0; i < PixelsCount; ++i )
	//		{
	//			if ( pSrc[i] > AlphaMask )
	//				pSrc[i]	= ( pSrc[i] & 0x00FFFFFF ) | AlphaMask;
	//		}
	//	}
	//}

	bool FeatureMMX () 
	{ 
	return m_FeatureMMX;
	}

	bool FeatureSSE () 
	{
	return m_FeatureSSE;
	}

	bool FeatureSSE2 () 
	{
	return m_FeatureSSE2;
	}

	private:

	static bool IsMMX()
	{
	static bool bMMX = _IsFeature(_MMX_FEATURE_BIT);
	return(bMMX);
	}

	static bool IsSSE()
	{
	static bool bSSE = _IsFeature(_SSE_FEATURE_BIT);
	return(bSSE);
	}

	static bool IsSSE2()
	{
	static bool bSSE2 = _IsFeature(_SSE2_FEATURE_BIT);
	return(bSSE2);
	}






	static bool _IsFeature(DWORD dwRequestFeature)
	{
	// This	bit	flag can get set on	calling	cpuid
	// with	register eax set to	1
	DWORD dwFeature	= 0;
	__try {
	_asm {
	mov	eax,1
	cpuid
	mov	dwFeature,edx
	}
	} __except ( EXCEPTION_EXECUTE_HANDLER)	{
	return false;
	}
	if ((dwRequestFeature == _MMX_FEATURE_BIT) &&
	(dwFeature & _MMX_FEATURE_BIT)) {
	__try {
	__asm {
	pxor mm0, mm0	// executing MMX instruction
	emms
	}
	} __except (EXCEPTION_EXECUTE_HANDLER) {
	return (0);
	}
	return(true);
	}
	else if ((dwRequestFeature == _SSE2_FEATURE_BIT) &&
	(dwFeature & _SSE2_FEATURE_BIT)) {
	__try {
	__asm {
	xorpd xmm0, xmm0 // executing SSE2 instruction
	}
	} __except (EXCEPTION_EXECUTE_HANDLER) {
	return (0);
	}
	return(true);
	}
	else if ((dwRequestFeature == _SSE_FEATURE_BIT) &&
	(dwFeature & _SSE_FEATURE_BIT)) {
	__try {
	__asm {
	xorps xmm0, xmm0 // executing SSE instruction
	}
	} __except (EXCEPTION_EXECUTE_HANDLER) {
	return (0);
	}
	return(true);
	}

	return false;
	}

	private:

	bool m_FeatureMMX;
	bool m_FeatureSSE;
	bool m_FeatureSSE2;
	};
	*/
	class MediaTransform
	{
	public:

		MediaTransform()
		{
			m_pFormat			= NULL;
			m_pTransform		= NULL;
			m_bInterfacesValid	= TRUE;
		}

		virtual ~MediaTransform()
		{
			Destroy();
		}


		bool ConvertToBGRA ( IUnknown* pSource, IUnknown*& pResult )
		{
			pResult = NULL;

			if ( NULL == pSource )
				return false;

			if ( false == Create() )
				return false;

			MediaCore::IAVSUncompressedVideoFrame* pSourceFrame = NULL;
			pSource->QueryInterface ( &pSourceFrame );
			if ( NULL == pSourceFrame )
				return false;

			// настраиваем формат для конвертации
			long lWidth = 0; pSourceFrame->get_Width ( &lWidth );
			long lHeight = 0; pSourceFrame->get_Height ( &lHeight );
			long lAspectX = 0; pSourceFrame->get_AspectRatioX ( &lAspectX );
			long lAspectY = 0; pSourceFrame->get_AspectRatioY ( &lAspectY );
			long lColorSpace = 0; pSourceFrame->get_ColorSpace ( &lColorSpace );

			m_pFormat->put_Width ( lWidth );
			m_pFormat->put_Height ( lHeight );
			m_pFormat->put_AspectRatioX ( lAspectX );
			m_pFormat->put_AspectRatioY ( lAspectY );

			// преобразуем к BGRA32 формату и создаем дубликат (всегда!)
			m_pTransform->SetVideoFormat ( m_pFormat );
			m_pTransform->raw_TransformFrame ( pSourceFrame, &pResult );

			// если трансформ не сделал дупликата - делаем его руками
			if ( pSourceFrame == pResult ) // здесь (pResult != NULL)
			{
				pResult->Release();
				pResult = NULL;

				pSourceFrame->raw_CreateDuplicate(DUBLICATE_TYPE_COPY, (MediaCore::IAVSMediaData**)(&pResult));
			}

			// если исходный формат был не BGRA (т.е. не содержал alpha канал), то его надо заполнить руками значением 255
			if ( ( lColorSpace != CSP_BGRA ) && ( lColorSpace != ( CSP_BGRA | CSP_VFLIP ) ) && ( NULL != pResult ) )
			{
				BYTE* pResultBuffer = NULL; ((MediaCore::IAVSMediaData*)pResult)->get_Buffer(&pResultBuffer);

				if (NULL != pResultBuffer)
				{
					BYTE* pResultAlpha = pResultBuffer + 3;
					for ( long index = 4 * lWidth * lHeight - 1; index >= 0; index -= 4, pResultAlpha += 4 )
						*pResultAlpha = 255;
				}
			}

			pSourceFrame->Release();

			return ( NULL != pResult );
		}

		bool ConvertToI420 ( IUnknown* pSource, IUnknown*& pResult )
		{
			pResult = NULL;

			if ( NULL == pSource )
				return false;

			if ( false == CreateToI420() )
				return false;

			MediaCore::IAVSUncompressedVideoFrame* pSourceFrame = NULL;
			pSource->QueryInterface ( &pSourceFrame );
			if ( NULL == pSourceFrame )
				return false;

			// настраиваем формат для конвертации
			long lWidth = 0; pSourceFrame->get_Width ( &lWidth );
			long lHeight = 0; pSourceFrame->get_Height ( &lHeight );
			long lAspectX = 0; pSourceFrame->get_AspectRatioX ( &lAspectX );
			long lAspectY = 0; pSourceFrame->get_AspectRatioY ( &lAspectY );
			long lColorSpace = 0; pSourceFrame->get_ColorSpace ( &lColorSpace );

			m_pFormat->put_Width ( lWidth );
			m_pFormat->put_Height ( lHeight );
			m_pFormat->put_AspectRatioX ( lAspectX );
			m_pFormat->put_AspectRatioY ( lAspectY );

			// преобразуем к I420 формату и создаем дубликат (всегда!)
			m_pTransform->SetVideoFormat ( m_pFormat );
			m_pTransform->raw_TransformFrame ( pSourceFrame, &pResult );

			// если трансформ не сделал дупликата - делаем его руками
			if ( pSourceFrame == pResult ) // здесь (pResult != NULL)
			{
				pResult->Release();
				pResult = NULL;

				pSourceFrame->raw_CreateDuplicate ( DUBLICATE_TYPE_COPY, (MediaCore::IAVSMediaData**)(&pResult) );
			}

			pSourceFrame->Release ();

			return (NULL != pResult);
		}

		bool ConvertToBitmap ( IUnknown* pSource, Bitmap*& pBitmap )
		{
			pBitmap = NULL;

			if ( NULL == pSource )
				return false;

			MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;
			pSource->QueryInterface ( &pMediaData );
			pSource->Release ();

			if ( NULL == pMediaData )
				return false;

			LONG ColorSpace = 0; pMediaData->get_ColorSpace(&ColorSpace);

			/*
			const long CONST_CSP_VFLIP			= CSP_VFLIP;
			const long CONST_CSP_BGRA			= CSP_BGRA;
			const long CONST_CSP_BGRA_CSP_VFLIP	= CSP_BGRA | CSP_VFLIP;
			*/

			if ( ( ColorSpace != CSP_BGRA ) && ( ColorSpace != ( CSP_BGRA | CSP_VFLIP ) ) )
			{
				IUnknown* pInterface = NULL;	// Этот интерфейс надо очищать

				MediaTransform::ConvertToBGRA ( pSource, *&pInterface );

				MediaCore::IAVSUncompressedVideoFrame* pUncompressed = NULL;
				pInterface->QueryInterface ( &pUncompressed );
				pInterface->Release ();

				if ( NULL == pUncompressed )
				{
					pInterface->Release();
					return false;
				}

				LONG lWidth = 0;			pUncompressed->get_Width ( &lWidth );
				LONG lHeight = 0;			pUncompressed->get_Height ( &lHeight );
				LONG lStride = 0;			pUncompressed->get_Stride ( 0, &lStride );
				BYTE* pSourceBuffer = 0;	pUncompressed->get_Buffer ( &pSourceBuffer );
				LONG nSize = 0;				pUncompressed->get_BufferSize ( &nSize );

				pBitmap = new Bitmap ( lWidth, lHeight, PixelFormat32bppARGB );
				Rect rectLock ( 0, 0, lWidth, lHeight );
				if ( pBitmap )
				{
					BitmapData data; 

					if ( Gdiplus::Ok == pBitmap->LockBits ( &rectLock, ImageLockModeWrite, PixelFormat32bppARGB, &data ) )
					{
						memcpy ( data.Scan0, pSourceBuffer, nSize );
						pBitmap->UnlockBits ( &data );

						if ( ColorSpace & CSP_VFLIP )
							pBitmap->RotateFlip ( RotateNoneFlipY );
					}
				}

				/*
				ImageFile::IImageFile3Ptr ptrImageFile;
				ptrImageFile.CreateInstance ( ImageFile::CLSID_ImageFile3 );
				ptrImageFile->SaveImage2 ( (IUnknown**)&pInterface, 4, L"c:\\DebugTestImage.png" );
				*/

				pInterface->Release();
				return true;
			}
			else
			{
				LONG lWidth = 0;			pMediaData->get_Width ( &lWidth );
				LONG lHeight = 0;			pMediaData->get_Height ( &lHeight );
				LONG lStride = 0;			pMediaData->get_Stride ( 0, &lStride );
				BYTE* pSourceBuffer = 0;	pMediaData->get_Buffer ( &pSourceBuffer );
				LONG nSize = 0;				pMediaData->get_BufferSize ( &nSize );
				//BYTE* pPlane0 = NULL;		pMediaData->get_Plane ( 0, &pPlane0 );

				pBitmap = new Bitmap ( lWidth, lHeight, PixelFormat32bppARGB );
				Rect rectLock  ( 0, 0, lWidth, lHeight );

				if ( pBitmap )
				{
					BitmapData data;

					if ( Gdiplus::Ok == pBitmap->LockBits ( &rectLock, ImageLockModeWrite, PixelFormat32bppARGB, &data ) )
					{
						memcpy ( data.Scan0, pSourceBuffer, nSize );
						pBitmap->UnlockBits ( &data );

						if ( ColorSpace & CSP_VFLIP )
							pBitmap->RotateFlip ( RotateNoneFlipY );

						return true;
					}
				}
			}

			return false;
		}

	protected:

		bool IsValid()
		{
			if ( NULL == m_pFormat || NULL == m_pTransform )
				return false;

			return true;
		}
		void Destroy ()
		{
			RELEASEINTERFACE ( m_pFormat );
			RELEASEINTERFACE ( m_pTransform );
		}

		bool Create()
		{
			if ( NULL != m_pTransform && NULL != m_pFormat )
				return false;

			if ( false == m_bInterfacesValid )
				return false;

			// пытаемся создать интерфейс трансформера
			if (NULL == m_pTransform)
			{
				if ( FAILED ( CoCreateInstance ( MediaCore::CLSID_CAVSVideoFrameTransform, NULL, CLSCTX_ALL,
					MediaCore::IID_IAVSVideoFrameTransform, (void**)(&m_pTransform) ) ) )
				{
					m_pTransform		= NULL;
					m_bInterfacesValid	= FALSE;
				}
			}

			// пытаемся создать интерфейс формата
			if ( NULL == m_pFormat )
			{
				if ( FAILED ( CoCreateInstance ( MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL,
					MediaFormat::IID_IAVSVideoFormat, (void**)(&m_pFormat) ) ) )
				{
					m_pFormat			= NULL;
					m_bInterfacesValid	= FALSE;
				}
				else
				{
					m_pFormat->SetDefaultProperties ();
					m_pFormat->put_ColorSpace ( CSP_BGRA /* | CSP_VFLIP*/ );
				}
			}

			return IsValid ();
		}

		bool CreateToI420()
		{
			if ( NULL != m_pTransform && NULL != m_pFormat )
				return true;

			if ( false == m_bInterfacesValid )
				return false;

			// пытаемся создать интерфейс трансформера
			if ( NULL == m_pTransform )
			{
				if ( FAILED ( CoCreateInstance ( MediaCore::CLSID_CAVSVideoFrameTransform, NULL, CLSCTX_ALL,
					MediaCore::IID_IAVSVideoFrameTransform, (void**)(&m_pTransform) ) ) )
				{
					m_pTransform = NULL;
					m_bInterfacesValid = FALSE;
				}
			}

			// пытаемся создать интерфейс формата
			if ( NULL == m_pFormat )
			{
				if ( FAILED ( CoCreateInstance ( MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL,
					MediaFormat::IID_IAVSVideoFormat, (void**)(&m_pFormat) ) ) )
				{
					m_pFormat = NULL;
					m_bInterfacesValid = FALSE;
				}
				else
				{
					m_pFormat->SetDefaultProperties();
					m_pFormat->put_ColorSpace ( CSP_I420 | CSP_VFLIP );
				}
			}

			return IsValid();
		}



	protected:

		MediaFormat::IAVSVideoFormat*		m_pFormat;				// интерфейс, хранящий настройки формата BGR32
		MediaCore::IAVSVideoFrameTransform* m_pTransform;			// интерфейс, необходимый для конвертации media data в формат BGR32
		bool								m_bInterfacesValid;		// флаг, обозначающий - производилась ли попытка создать интерфейс трансформера или нет
	};

	// TODO : сделать оптимизированый вариант
	class PixelsOperations
	{
	public:

		static const long sc_nInterpolationModeNearestNeighbor	= 0;
		static const long sc_nInterpolationModeBilinear			= 1;
		static const long sc_nInterpolationModeBicubic			= 2;

	public: 
		static void MemsetDW ( DWORD* pSrc, long Count, DWORD Color /* nChannels = 4 */ )
		{
			DWORD* pPixelsRGBA	=	(DWORD*) pSrc;
			for ( long i = 0; i < Count; ++i )
				pSrc[i] = Color;
		}

		static void BilinearResizeImageBT ( BYTE* pSrc, BYTE* pPixels,
			long SourceWidth, long SourceHeight, long nWidth, long nHeight, 
			double SourceOffsetWidth, double SourceOffsetHeight,
			double DestOffsetWidth, double DestOffsetHeight,
			double SrcStepWidth, double SrcStepHeight,
			long StartWriteW, long EndWriteW, long StartWriteH, long EndWriteH /* nChannels = 4 */ )
		{
			for ( long j = StartWriteH; j < EndWriteH-1; ++j )
			{
				for ( long i = StartWriteW; i < EndWriteW-1; ++i )
				{
					long nWriteInd				=	( i + j * nWidth ) << 2;	// * nChannels;	// индекс по которому пишем

					double SourceIndX			=	( SrcStepWidth	* ( i + DestOffsetWidth ) + SourceOffsetWidth );
					double SourceIndY			=	( SrcStepHeight	* ( j + DestOffsetHeight ) + SourceOffsetHeight );

					// long FloorIndX			=	floor ( SourceIndX );
					// long FloorIndY			=	floor ( SourceIndY );
					// double FractionX			=	SourceIndX - FloorIndX;
					// double FractionY			=	SourceIndY - FloorIndY;

					long FloorIndX				=	(long) ( SourceIndX );
					long FloorIndY				=	(long) ( SourceIndY );
					double FractionX			=	/*abs*/ ( SourceIndX - FloorIndX );
					double FractionY			=	/*abs*/ ( SourceIndY - FloorIndY );

					long SelectColor03			=	( FloorIndX + SourceWidth * FloorIndY + SourceWidth ) << 2; // * nChannels;
					long SelectColor01			=	( FloorIndX + SourceWidth * FloorIndY ) << 2;	// * nChannels;

					BYTE* pColor03				=	&pSrc [ SelectColor03 ];
					BYTE* pColor01				=	&pSrc [ SelectColor01 ];

					BYTE Blue01					=	(BYTE) ( pColor01[0] + ( pColor01[4] - pColor01[0] ) * FractionX );
					BYTE Blue02					=	(BYTE) ( pColor03[0] + ( pColor03[4] - pColor03[0] ) * FractionX );
					BYTE Green01				=	(BYTE) ( pColor01[1] + ( pColor01[5] - pColor01[1] ) * FractionX );
					//00
					BYTE Green02				=	(BYTE) ( pColor03[1] + ( pColor03[5] - pColor03[1] ) * FractionX );
					BYTE Red01					=	(BYTE) ( pColor01[2] + ( pColor01[6] - pColor01[2] ) * FractionX );
					BYTE Red02					=	(BYTE) ( pColor03[2] + ( pColor03[6] - pColor03[2] ) * FractionX );
					//00

					pPixels [ nWriteInd + 0 ]	=	(BYTE) ( Blue01		+ FractionY * ( Blue02 - Blue01 ) );
					pPixels [ nWriteInd + 1 ]	=	(BYTE) ( Green01	+ FractionY * ( Green02 - Green01 ) );
					pPixels [ nWriteInd + 2 ]	=	(BYTE) ( Red01		+ FractionY * ( Red02 - Red01 ) );
					pPixels [ nWriteInd + 3 ]	=	0xFF;
				}
			}
		}


		static void BilinearResizeImageDW ( DWORD* pSrc, DWORD* pPixels,
			long SourceWidth, long SourceHeight, long nWidth, long nHeight, 
			double SourceOffsetWidth, double SourceOffsetHeight,
			double DestOffsetWidth, double DestOffsetHeight,
			double SrcStepWidth, double SrcStepHeight,
			long StartWriteW, long EndWriteW, long StartWriteH, long EndWriteH /* nChannels = 4 */ )
		{
			for ( long j = StartWriteH; j < EndWriteH-1; ++j )
			{
				for ( long i = StartWriteW; i < EndWriteW-1; ++i )
				{
					long nWriteInd				=	( i + j * nWidth );	//* nChannels;	// индекс по которому пишем

					double SourceIndX			=	( SrcStepWidth	* ( i + DestOffsetWidth ) + SourceOffsetWidth );
					double SourceIndY			=	( SrcStepHeight	* ( j + DestOffsetHeight ) + SourceOffsetHeight );

					// long FloorIndX			=	floor ( SourceIndX );
					// long FloorIndY			=	floor ( SourceIndY );
					// double FractionX			=	SourceIndX - FloorIndX;
					// double FractionY			=	SourceIndY - FloorIndY;

					long FloorIndX				=	(long) ( SourceIndX );
					long FloorIndY				=	(long) ( SourceIndY );
					double FractionX			=	/*abs*/ ( SourceIndX - FloorIndX );
					double FractionY			=	/*abs*/ ( SourceIndY - FloorIndY );

					long SelectColor01			=	( FloorIndX + SourceWidth * FloorIndY );	//* nChannels;
					long SelectColor03			=	( FloorIndX + SourceWidth * FloorIndY + SourceWidth );	// * nChannels;

					DWORD pColor01				=	pSrc [ SelectColor01 ];
					DWORD pColor02				=	pSrc [ SelectColor01 + 1 ];
					DWORD pColor03				=	pSrc [ SelectColor03 ];
					DWORD pColor04				=	pSrc [ SelectColor03 + 1 ];

					//__int64	Color00			=	pColor01 + ( pColor03 - pColor01 ) * FractionX;
					//__int64	Color11			=	pColor02 + ( pColor04 - pColor02 ) * FractionX;
					//__int64	ColorResult		=	Color00 + ( Color11 - Color00 ) * FractionY;
					//pPixels [ nWriteInd ]		=	ColorResult;

					//BYTE* pColor01				=	(BYTE*)pSrc [ SelectColor01 ];
					//BYTE* pColor03				=	(BYTE*)pSrc [ SelectColor03 ];

					//BYTE Blue01					=	(BYTE) ( pColor01[0] + ( pColor01[4] - pColor01[0] ) * FractionX );
					//BYTE Blue02					=	(BYTE) ( pColor03[0] + ( pColor03[4] - pColor03[0] ) * FractionX );
					//BYTE Green01					=	(BYTE) ( pColor01[1] + ( pColor01[5] - pColor01[1] ) * FractionX );
					////00
					//BYTE Green02					=	(BYTE) ( pColor03[1] + ( pColor03[5] - pColor03[1] ) * FractionX );
					//BYTE Red01					=	(BYTE) ( pColor01[2] + ( pColor01[6] - pColor01[2] ) * FractionX );
					//BYTE Red02					=	(BYTE) ( pColor03[2] + ( pColor03[6] - pColor03[2] ) * FractionX );
					////00

					//BYTE Blue						=	(BYTE) ( Blue01		+ FractionY * ( Blue02 - Blue01 ) );
					//BYTE Green					=	(BYTE) ( Green01	+ FractionY * ( Green02 - Green01 ) );
					//BYTE Red						=	(BYTE) ( Red01		+ FractionY * ( Red02 - Red01 ) );

					//((BYTE*)pPixels) [ nWriteInd + 0 ]	=	Blue;
					//((BYTE*)pPixels) [ nWriteInd + 1 ]	=	Green;
					//((BYTE*)pPixels) [ nWriteInd + 2 ]	=	Red;
					//((BYTE*)pPixels) [ nWriteInd + 3 ]	=	0xFF;
				}
			}
		}


		static void NearestResizeImageBT ( BYTE* pSrc, BYTE* pPixels,
			long SourceWidth, long SourceHeight, long nWidth, long nHeight, 
			double SourceOffsetWidth, double SourceOffsetHeight,
			double DestOffsetWidth, double DestOffsetHeight,
			double SrcStepWidth, double SrcStepHeight,
			long StartWriteW, long EndWriteW, long StartWriteH, long EndWriteH /* nChannels = 4 */ )
		{
			for ( double j = StartWriteH; j < EndWriteH; ++j )
			{
				for ( double i = StartWriteW; i < EndWriteW; ++i )
				{
					double SourceIndX			=	( SrcStepWidth	* ( i + DestOffsetWidth ) + SourceOffsetWidth );
					double SourceIndY			=	( SrcStepHeight	* ( j + DestOffsetHeight ) + SourceOffsetHeight );

					double nSourceIndD			=	SourceIndX + (long) SourceIndY * SourceWidth;

					long SourceInd				=	((long) nSourceIndD );
					long nWriteInd				=	((long)( i + j * nWidth )) << 2;	// * nChannels;

					pPixels [ nWriteInd		]	=	pSrc [ SourceInd	 ];
					pPixels [ nWriteInd + 1 ]	=	pSrc [ SourceInd + 1 ];
					pPixels [ nWriteInd + 2 ]	=	pSrc [ SourceInd + 2 ];
					pPixels [ nWriteInd + 3 ]	=	0xFF;
				}
			}
		}


		static void NearestResizeImageDW ( DWORD* pSrc, DWORD* pPixels,
			long SourceWidth, long SourceHeight, long nWidth, long nHeight, 
			double SourceOffsetWidth, double SourceOffsetHeight,
			double DestOffsetWidth, double DestOffsetHeight,
			double SrcStepWidth, double SrcStepHeight,
			long StartWriteW, long EndWriteW, long StartWriteH, long EndWriteH /* nChannels = 4 */ )
		{
			double PreMulX				=	SrcStepWidth * DestOffsetWidth + SourceOffsetWidth;
			double PreMulY				=	SrcStepHeight * DestOffsetHeight + SourceOffsetHeight;
			double PreSourceIndX		=	( SrcStepWidth	* StartWriteW + PreMulX );
			long PreSourceIndY			=	(long)( SrcStepHeight	* ( StartWriteH - 1 ) + PreMulY );

			long SourceSize				=	SourceWidth * SourceHeight;
			double SourceIndY2			=	( SrcStepHeight	* ( EndWriteH - 1 ) + PreMulY );
			double SourceIndX2			=	( SrcStepWidth	* ( EndWriteW - 1 ) + PreMulX );
			double nSourceIndD2			=	SourceIndX2 + (long) SourceIndY2 * SourceWidth;
			long SourceInd2				=	(long) nSourceIndD2;
			
			// ошибка дискретизации. фикс
			if ( SourceSize <= SourceInd2 )
			{
				EndWriteH--;
			}

			for ( long j = StartWriteH; j < EndWriteH; ++j )
			{
				long SourceIndY			=	(long)( SrcStepHeight	* j + PreMulY );
				double dSourceInd		=	PreSourceIndX + (long) SourceIndY * SourceWidth;
				long WriteInd			=	j * nWidth + StartWriteW;

				for ( long i = StartWriteW; i < EndWriteW; ++i )
				{
					pPixels[WriteInd]	=	pSrc [ (long) dSourceInd ];

					++WriteInd;
					dSourceInd			+=	SrcStepWidth;

					///*	

					// OLD STYLE

					//double SourceIndY	=	( SrcStepHeight	* j + PreMulY );
					//double SourceIndX	=	( SrcStepWidth	* i + PreMulX );
					//double nSourceIndD	=	SourceIndX + (long) SourceIndY * SourceWidth;

					//long SourceInd		=	(long) nSourceIndD;
					//long nWriteInd		=	(long)( i + j * nWidth );
					//
					//pPixels [ nWriteInd ]	=	pSrc [ SourceInd ];

					
					//*/
				}
			}
		}

		//static void NearestResizeImageDW ( 
		//	DWORD* pSrc, 
		//	DWORD* pPixels,
		//	long SourceWidth, long SourceHeight, 
		//	long nWidth, long nHeight, 
		//	double SourceOffsetWidth, double SourceOffsetHeight,
		//	double DestOffsetWidth, double DestOffsetHeight,
		//	double SrcStepWidth, double SrcStepHeight,
		//	long StartWriteW, long EndWriteW, 
		//	long StartWriteH, long EndWriteH )
		//{
		//	double	PreMulX					=	SrcStepWidth * DestOffsetWidth		+ SourceOffsetWidth;
		//	double	PreMulY					=	SrcStepHeight * DestOffsetHeight	+ SourceOffsetHeight;
		//	double	PreSourceIndX			=	SrcStepWidth * StartWriteW			+ PreMulX;
		//	long	PreSourceIndY			=	static_cast<long> ( SrcStepHeight * ( StartWriteH - 1 ) + PreMulY );

		//	double nIndX					=	SrcStepWidth * ( StartWriteW ) + PreMulX;

		//	long Width						=	static_cast<long> ( ( EndWriteH - StartWriteH ) * 0.5 );

		//	for ( long j = StartWriteH; j < EndWriteH - Width; ++j )
		//	{
		//		long SourceIndY				=	static_cast<long> ( SrcStepHeight * j + PreMulY );
		//		long WriteInd				=	static_cast<long> ( j * nWidth + StartWriteW );

		//		double SourceIndX			=	nIndX;
		//		long SourceMoveY			=	SourceIndY * SourceWidth;

		//		for ( long i = StartWriteW; i < EndWriteW; ++i )
		//		{
		//			long nSourceInd			=	static_cast<long> ( SourceIndX + SourceMoveY );

		//			pPixels [ WriteInd ]	=	pSrc [ nSourceInd ];

		//			++WriteInd;

		//			SourceIndX				+=	SrcStepWidth;
		//		}
		//	}

		//	for ( long j = StartWriteH + Width; j < EndWriteH; ++j )
		//	{
		//		long SourceIndY				=	static_cast<long> ( SrcStepHeight * j + PreMulY );
		//		long WriteInd				=	static_cast<long> ( j * nWidth + StartWriteW );

		//		double SourceIndX			=	nIndX;
		//		long SourceMoveY			=	SourceIndY * SourceWidth;

		//		for ( long i = StartWriteW; i < EndWriteW; ++i )
		//		{
		//			long nSourceInd			=	static_cast<long> ( SourceIndX + SourceMoveY );

		//			pPixels [ WriteInd ]	=	pSrc [ nSourceInd ];

		//			++WriteInd;

		//			SourceIndX				+=	SrcStepWidth;
		//		}
		//	}
		//}

		/*
		static void NearestResizeImageDW_SSE2 ( DWORD* pSrc, DWORD* pPixels,
		long SourceWidth, long SourceHeight, long nWidth, long nHeight, 
		double SourceOffsetWidth, double SourceOffsetHeight,
		double DestOffsetWidth, double DestOffsetHeight,
		double SrcStepWidth, double SrcStepHeight,
		long StartWriteW, long EndWriteW, long StartWriteH, long EndWriteH  )
		{
		double PreMulX					=	SrcStepWidth * DestOffsetWidth + SourceOffsetWidth;
		double PreMulY					=	SrcStepHeight * DestOffsetHeight + SourceOffsetHeight;

		__m128d XMMMPreMul				=	_mm_set_pd ( PreMulX, PreMulY );
		__m128d XMMMSrcStep				=	_mm_set_pd ( SrcStepWidth, SrcStepHeight );
		__m128d XMMMSourceOffset		=	_mm_set_pd ( SourceOffsetWidth, SourceOffsetHeight );

		for ( long j = StartWriteH; j < EndWriteH; ++j )
		{
		for ( long i = StartWriteW; i < EndWriteW; ++i )
		{
		__m128d XMMMInd			=	_mm_set_pd ( i, j ); 
		__m128d XMMMSourceInd	=	_mm_mul_pd ( XMMMInd, XMMMSrcStep );
		XMMMSourceInd			=	_mm_add_pd ( XMMMSourceInd, XMMMPreMul );

		long SourceInd			=	(long) XMMMSourceInd.m128d_f64[1] + 
		(long) XMMMSourceInd.m128d_f64[0] * SourceWidth;
		long nWriteInd			=	i + j * nWidth;

		pPixels [ nWriteInd	]	=	pSrc [ SourceInd ];
		}
		}
		}
		*/
	};
}