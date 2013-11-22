#pragma once
	
#include <ImageStudioUtils.h>
	
namespace ImageStudio
{
	typedef MediaCore::IAVSUncompressedVideoFrame IMediaFrame;

	static IMediaFrame* CreateFrame( int nWidth, int nHeight, int nColorSpace, BYTE** pResultBuffer = NULL )
	{
		if( nWidth <= 0 || nHeight <= 0 )
			return NULL;

		IMediaFrame* pFrame = NULL;
		CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pFrame) );
		if( !pFrame )
			return NULL;

		pFrame->put_Width( nWidth );
		pFrame->put_Height( nHeight );
		pFrame->put_ColorSpace( nColorSpace );
		pFrame->raw_SetDefaultStrides();
		pFrame->raw_AllocateBuffer( -1 );
		
		BYTE* pBuffer = NULL;
		pFrame->get_Buffer( &pBuffer );
		if( !pBuffer )
		{
			pFrame->Release();
			return NULL;
		}

		if( pResultBuffer )
		{
			*pResultBuffer = pBuffer;
		}

		long lBufSize;
		pFrame->get_BufferSize( &lBufSize );
		pFrame->put_DataSize( lBufSize );
		
		pFrame->put_AspectRatioX( nWidth );
		pFrame->put_AspectRatioY( nHeight );

		return pFrame;		
	}

	static IMediaFrame* CreateFrame( IMediaFrame* pSource, BYTE** pResultBuffer = NULL )
	{
		if( !pSource )
			return NULL;

		long lWidth, lHeight, lColorSpace;
		pSource->get_Width( &lWidth );
		pSource->get_Height( &lHeight );
		pSource->get_ColorSpace( &lColorSpace );

		IMediaFrame* pResult = CreateFrame( lWidth, lHeight, lColorSpace, pResultBuffer );
		if( !pResult )
			return NULL;

		long lAspectX;
		pSource->get_AspectRatioX( &lAspectX );
		pResult->put_AspectRatioX( lAspectX );

		long lAspectY;
		pSource->get_AspectRatioY( &lAspectY );
		pResult->put_AspectRatioY( lAspectY );

		double dDuration;
		pSource->get_Duration( &dDuration );
		pResult->put_Duration( dDuration );

		double dTimeStamp;
		pSource->get_TimeStamp( &dTimeStamp );
		pResult->put_TimeStamp( dTimeStamp );

		VARIANT_BOOL vbInterlaced;
		pSource->get_Interlaced( &vbInterlaced );
		pResult->put_Interlaced( vbInterlaced );

		VARIANT_BOOL vbDiscontinity;
		pSource->get_IsDiscontinuity( &vbDiscontinity );
		pResult->put_IsDiscontinuity( vbDiscontinity );

		return pResult;
	}

	static IMediaFrame* CreateFrame( IUnknown* pSource, BYTE** pResultBuffer = NULL )
	{
		if( !pSource )
			return NULL;

		IMediaFrame* pFrame = NULL;
		pSource->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame );
		if( !pFrame )
			return NULL;

		IMediaFrame* pResult = CreateFrame( pFrame, pResultBuffer );

		pFrame->Release();

		return pResult;
	}


	static IMediaFrame* CreateBgraFrame( int nWidth, int nHeight, BYTE** ppResultBuffer = NULL )
	{
		return CreateFrame( nWidth, nHeight, CSP_BGRA, ppResultBuffer );
	}

	
	static BOOL GetBGRAImageData( IMediaFrame* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight, int* pStride = NULL, long* pAspectX = NULL, long* pAspectY = NULL)
	{
		if (NULL == pSource)
			return FALSE;

		// запрашиваем настройки картинки
		long lWidth = 0;
		long lHeight = 0;
		long lColorSpace = 0;
		long lStride = 0;
		long lBufferSize = 0;
		BYTE* pSourceBuffer = NULL;

		pSource->get_Width(&lWidth);
		pSource->get_Height(&lHeight);
		pSource->get_ColorSpace(&lColorSpace);
		pSource->get_Stride(0, &lStride);
		pSource->get_BufferSize(&lBufferSize);
		pSource->get_Buffer(&pSourceBuffer);

		// проверяем совместимость кадра и формата
		if (CSP_BGRA != (CSP_COLOR_MASK & lColorSpace) || lStride < 4*lWidth || lWidth <= 0 || lHeight <= 0 || lBufferSize < 4*lWidth*lHeight || !pSourceBuffer)
		{
			return FALSE;
		}

		if( pStride )
			*pStride = (lColorSpace & CSP_VFLIP) ? -lStride : lStride;

		if( pAspectX ) pSource->get_AspectRatioX( pAspectX );
		if( pAspectY ) pSource->get_AspectRatioY( pAspectY );

		// все хорошо, возвращаем наверх нужные параметры
		pBGRA = pSourceBuffer;
		nWidth = lWidth;
		nHeight = lHeight;

		return TRUE;
	}
	static BOOL GetBGRAImageData( IUnknown* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight, int* pStride = NULL, long* pAspectX = NULL, long* pAspectY = NULL)
	{
		if (NULL == pSource)
			return FALSE;

		// вытаскиваем из интерфейса uncompressed frame
		IMediaFrame* pFrame = NULL;
		pSource->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pFrame));
		if (!pFrame)
			return FALSE;

		BOOL bSuccess = GetBGRAImageData( pFrame, pBGRA, nWidth, nHeight, pStride, pAspectX, pAspectY );
		
		pFrame->Release();
		
		return bSuccess;
	}
	
	namespace Core
	{
		class MediaData
		{
		public:
			
			MediaData()
			{
				m_bAttached = FALSE;

				m_pMediaData = NULL;
			}
			virtual ~MediaData()
			{
				Destroy();
			}
			
			void Destroy()
			{
				if (m_pMediaData != NULL)
				{
					if (!m_bAttached)
						m_pMediaData->Release();
					m_pMediaData = NULL;
				}
			}
			
			LONG GetWidth()
			{
				if (!IsValid())
					return 0;

				LONG lWidth = 0;
				
				m_pMediaData->get_Width(&lWidth);

				return lWidth;
			}
			LONG GetHeight()
			{
				if (!IsValid())
					return 0;

				LONG lHeight = 0;
				
				m_pMediaData->get_Height(&lHeight);

				return lHeight;
			}
			LONG GetColorSpace()
			{
				if (!IsValid())
					return 0;

				LONG lColorSpace = 0;
				
				m_pMediaData->get_ColorSpace(&lColorSpace);

				return lColorSpace;
			}
			LONG GetStride()
			{
				if (!IsValid())
					return 0;

				LONG lStride = 0;
				
				m_pMediaData->get_Stride(0, &lStride);

				return lStride;
			}
			LONG GetBufferSize()
			{
				if (!IsValid())
					return 0;

				LONG lBufferSize = 0;
				
				m_pMediaData->get_BufferSize(&lBufferSize);

				return lBufferSize;
			}
			BYTE* GetBuffer()
			{
				if (!IsValid())
					return NULL;

				BYTE* pSourceBuffer = 0;
				
				m_pMediaData->get_Buffer(&pSourceBuffer);

				return pSourceBuffer;
			}
			MediaCore::IAVSUncompressedVideoFrame*& GetMediaData(BOOL bAddReference)
			{
				if (bAddReference)
				{
					if (m_pMediaData != NULL)
						m_pMediaData->AddRef();
				}
				return m_pMediaData;
			}
			
			BOOL IsValid()
			{
				if (m_pMediaData == NULL)
					return FALSE;

				return TRUE;
			}
			BOOL IsValidBGRA()
			{
				if (!IsValid())
					return FALSE;

				LONG lWidth = 0; m_pMediaData->get_Width(&lWidth);
				LONG lHeight = 0; m_pMediaData->get_Height(&lHeight);
				LONG lColorSpace = 0; m_pMediaData->get_ColorSpace(&lColorSpace);
				LONG lStride = 0; m_pMediaData->get_Stride(0, &lStride);
				LONG lBufferSize = 0; m_pMediaData->get_BufferSize(&lBufferSize);
				BYTE* pSourceBuffer = 0; m_pMediaData->get_Buffer(&pSourceBuffer);

				if (CSP_BGRA != (CSP_COLOR_MASK & lColorSpace) || 4*lWidth != lStride || lWidth < 1 || lHeight < 1 || lBufferSize != 4*lWidth*lHeight || !pSourceBuffer)
					return FALSE;

				return TRUE;
			}
		
			BOOL Create(IUnknown* pInterface, BOOL bAttach)
			{
				Destroy();

				m_bAttached = bAttach;

				return ImageStudioUtils::MediaDataToBGRAMediaData(&pInterface, m_pMediaData, TRUE);
			}
			
		protected:

			BOOL m_bAttached;

			MediaCore::IAVSUncompressedVideoFrame* m_pMediaData; // для картинок, основанных на интерфейсе (чтобы не копировать память)
		};
		class MediaBuffer
		{
		public:
			
			MediaBuffer()
			{
				m_pData = NULL;
				m_nSize = 0;
			}
			virtual ~MediaBuffer()
			{
				Destroy();
			}
			
			void Destroy()
			{
				if (m_pData != NULL)
				{
					delete[] m_pData;
					m_pData = NULL;
				}

				m_nSize = 0;
			}
		
			LONG GetBufferSize()
			{
				return m_nSize;
			}
			BYTE* GetBuffer()
			{
				return m_pData;
			}
			
			BOOL IsValid()
			{
				if (m_pData == NULL)
					return FALSE;

				return TRUE;
			}
		
			BOOL Create(LONG nSize)
			{
				if (m_nSize == nSize)
					return TRUE;

				Destroy();

				if (nSize < 1)
					return FALSE;

				m_pData = new BYTE[nSize];

				if (!m_pData)
					return FALSE;

				m_nSize = nSize;

				return TRUE;
			}
			BOOL Create(BYTE* pData, LONG nSize)
			{
				if (!pData)
					return FALSE;

				if (!Create(nSize))
					return FALSE;

				memcpy(m_pData, pData, nSize);

				m_nSize = nSize;

				return TRUE;
			}
			
		protected:
		
			BYTE* m_pData;
			LONG m_nSize;
		};
		
		class Mask
		{
		public:

			Mask()
			{
				m_nWidth = -1;
				m_nHeight = -1;
			}
			virtual ~Mask()
			{
				Destroy();
			}

			BOOL IsValid()
			{
				if (m_nWidth < 1 || m_nHeight < 1 || !m_oPixels.IsValid())
					return FALSE;

				return TRUE;
			}
			int GetWidth() const
			{
				return m_nWidth;
			}
			int GetHeight() const
			{
				return m_nHeight;
			}
			BYTE* GetPixels()
			{
				return m_oPixels.GetBuffer();
			}
			BYTE GetPixel(int nX, int nY, BYTE nDefaultValue)
			{
				if (nX < 0 || nY < 0 || nX >= m_nWidth || nY >= m_nHeight || !m_oPixels.IsValid())
					return nDefaultValue;

				return *((BYTE*)m_oPixels.GetBuffer() + nY*m_nWidth + nX);
			}
			void SetPixel(int nX, int nY, BYTE nValue)
			{
				if (nX < 0 || nY < 0 || nX >= m_nWidth || nY >= m_nHeight || !m_oPixels.IsValid())
					return;

				*((BYTE*)m_oPixels.GetBuffer() + nY*m_nWidth + nX) = nValue;
			}
			
			void Destroy()
			{
				m_nWidth = -1;
				m_nHeight = -1;
				m_oPixels.Destroy();
			}
			BOOL Create(int nWidth, int nHeight)
			{
				if (m_nWidth == nWidth && m_nHeight == nHeight)
					return TRUE;

				m_nWidth = nWidth;
				m_nHeight = nHeight;
				
				if (m_nWidth < 1 || m_nHeight < 1)
				{
					m_oPixels.Destroy();
					return TRUE;
				}

				return m_oPixels.Create(nWidth*nHeight);
			}
			
			BOOL Fill(BYTE nValue)
			{
				if (!IsValid())
					return FALSE;

				BYTE* pPixels = m_oPixels.GetBuffer();
				int nSize = m_oPixels.GetBufferSize() - 1;

				while (nSize >= 0)
				{
					*pPixels = nValue;

					pPixels++;
					nSize--;
				}

				return TRUE;
			}
			BOOL FillRegion(Gdiplus::Region& oRegion, BYTE nValue)
			{
				BYTE* pPixels = m_oPixels.GetBuffer();
				if (!pPixels)
					return FALSE;

				// проверяем все пикселы в маске
				for (int nY = 0; nY < m_nHeight; ++nY)
				{
					for (int nX = 0; nX < m_nWidth; ++nX, ++pPixels)
					{
						// маска перевернутая должна быть (также как и все наши картинки..чтоб им было неладно)
						if (oRegion.IsVisible(nX, /*m_nHeight - 1 -*/ nY)) 
							*pPixels = nValue;
					}
				}

				return TRUE;
			}
			BOOL FillGraphicsPath(Gdiplus::GraphicsPath& oPath, BYTE nValue, Gdiplus::Matrix* pTransform = NULL )
			{
				// используем gdiplus::regions для определения маскирующих пикселов
				Gdiplus::Region oRegion(Gdiplus::RectF(0.0f, 0.0f, (float)m_nWidth, (float)m_nHeight));
				oRegion.Intersect(&oPath);

				if( pTransform )
				{
					oRegion.Transform( pTransform );
				}

				return FillRegion(oRegion, nValue);
			}
			BOOL FillRectangle(double dLeft, double dTop, double dRight, double dBottom, BYTE nValue, Gdiplus::Matrix* pTransform = NULL)
			{
				// создаем маскирующий объект
				Gdiplus::GraphicsPath oPath(Gdiplus::FillModeAlternate);
				Gdiplus::RectF oRect((float)dLeft, (float)dTop, (float)(dRight - dLeft), (float)(dBottom - dTop));
				oPath.AddRectangle(oRect);

				return FillGraphicsPath(oPath, nValue, pTransform);
			}
			BOOL FillEllipse(double dLeft, double dTop, double dRight, double dBottom, BYTE nValue, Gdiplus::Matrix* pTransform = NULL)
			{
				// создаем маскирующий объект
				Gdiplus::GraphicsPath oPath(Gdiplus::FillModeAlternate);
				oPath.AddEllipse((float)dLeft, (float)dTop, (float)(dRight - dLeft), (float)(dBottom - dTop));

				return FillGraphicsPath(oPath, nValue, pTransform);
			}
			BOOL FillPolygon(Gdiplus::PointF* pPoints, int nPointsCount, BYTE nValue, Gdiplus::Matrix* pTransform = NULL)
			{
				if (!pPoints || nPointsCount < 3)
					return FALSE;

				// создаем маскирующий объект
				Gdiplus::GraphicsPath oPath(Gdiplus::FillModeAlternate);
				oPath.AddPolygon(pPoints, nPointsCount);

				return FillGraphicsPath(oPath, nValue, pTransform);
			}
			
		protected:

			int m_nWidth;
			int m_nHeight;
			ImageStudio::Core::MediaBuffer m_oPixels;
		};


		class Image
		{
		public:
			
			Image()
			{
				m_pPixels = NULL;
			}

			virtual ~Image()
			{
				Destroy();
			}
			virtual void Destroy()
			{
				m_oImage.Destroy();

				m_oMediaData.Destroy();

				if (m_pPixels)
				{
					delete[] m_pPixels;
					m_pPixels = NULL;
				}
			}
			
			virtual BOOL IsValid()
			{
				return m_oImage.IsValid();
			}

			virtual int GetWidth()
			{
				return m_oImage.GetWidth();
			}
			virtual int GetHeight()
			{
				return m_oImage.GetHeight();
			}
			virtual GdiPlusEx::CGdiPlusImage* GetImage()
			{
				return &m_oImage;
			}
			virtual Gdiplus::Bitmap* GetBitmap()
			{
				return (Gdiplus::Bitmap*)m_oImage;
			}
			virtual ImageStudio::Core::MediaData& GetMediaData()
			{
				return m_oMediaData;
			}
			virtual BYTE*& GetBuffer()
			{
				return m_pPixels;
			}
			
			virtual BOOL RotateFlip(int nRotateFlip)
			{
				return (Gdiplus::Ok == GetBitmap()->RotateFlip((Gdiplus::RotateFlipType)nRotateFlip));
			}
			
			virtual BOOL FromByteArray(int nWidth, int nHeight)
			{
				Destroy();

				if (nWidth < 1 || nHeight < 1)
					return FALSE;

				m_pPixels = new BYTE[4*nWidth*nHeight];
				if (NULL == m_pPixels)
					return FALSE;

				return m_oImage.CreateFromByteArray(m_pPixels, nWidth, nHeight, 4*nWidth, PixelFormat32bppARGB, FALSE);
			}
			virtual BOOL FromSafeArray(SAFEARRAY** pSafeArray, BOOL CreateCopy)
			{
				Destroy();

				return m_oImage.CreateFromSafeArray(pSafeArray, CreateCopy);
			}
			virtual BOOL FromMediaData(IUnknown* pInterface, BOOL bCreateCopy)
			{
				Destroy();

				if (!m_oMediaData.Create(pInterface, FALSE))
					return FALSE;

				int nWidth = m_oMediaData.GetWidth();
				int nHeight = m_oMediaData.GetHeight();
				BYTE* pPixels = m_oMediaData.GetBuffer();

				BOOL bSuccess = m_oImage.CreateFromByteArray(pPixels, nWidth, nHeight, 4*nWidth, PixelFormat32bppARGB, bCreateCopy);

				if (bCreateCopy)
					m_oMediaData.Destroy();

				return bSuccess;
			}
			
			virtual BOOL ToSafeArray(SAFEARRAY** pSafeArray)
			{
				return ImageStudioUtils::GdiPlusImageToSafeArray(&m_oImage, pSafeArray);
			}
			virtual BOOL ToMediaData(IUnknown** pInterface, BOOL bFlipVertical)
			{
				return ImageStudioUtils::GdiPlusImageToMediaData(&m_oImage, pInterface, bFlipVertical);
			}
			
		protected:
			
			BYTE* m_pPixels; // дополнительный буффер для пикселов
			
			ImageStudio::Core::MediaData m_oMediaData; // для картинок, основанных на интерфейсе (чтобы не копировать память)

			GdiPlusEx::CGdiPlusImage m_oImage; // gdi+ картинка - собственно самое основное
		};

	}
}