#pragma once

#include <TransformConstants.h>
#include <ImageTransformsCore.h>
#include <ImageAnimations.h>

#include <ImageCore.h>
#include <MetaDataSaver.h>
#include <directx9\dx_effects.h>



namespace ImageStudio
{
	namespace Transforms
	{
		double GetMetric( int nMetric, int nSize )
		{
			switch( nMetric )
			{
			case ImageStudio::Serialize::c_nMetricPercents: return nSize * 0.01;
			case ImageStudio::Serialize::c_nMetricNormalizedCoordinates: return nSize;
			case ImageStudio::Serialize::c_nMetricAlternativeNormalizedCoordinates: return nSize;
			default: return 1;
			}
		}

		BOOL GetMetricMultiplier( IMediaFrame* pFrame, int nMetric, double& dMetricX, double& dMetricY )
		{
			if( !pFrame )
				return FALSE;

			long lWidth  = 0; pFrame->get_Width( &lWidth );
			long lHeight = 0; pFrame->get_Height( &lHeight );

			dMetricX = GetMetric( nMetric, lWidth );
			dMetricY = GetMetric( nMetric, lHeight );

			return TRUE;
		}
		BOOL GetMetricMultiplier( IUnknown* pInterface, int nMetric, double& dMetricX, double& dMetricY )
		{
			if( !pInterface )
				return FALSE;

			IMediaFrame* pFrame = NULL;
			pInterface->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame );
			if( !pFrame )
				return FALSE;

			BOOL bRetCode = GetMetricMultiplier( pFrame, nMetric, dMetricX, dMetricY );

			pFrame->Release();
		
			return TRUE;
		}
		BOOL GetMetricMultiplier( Gdiplus::Image* pImage, int nMetric, double& dMetricX, double& dMetricY )
		{
			if( !pImage )
				return FALSE;

			int nWidth = pImage->GetWidth();
			int nHeight = pImage->GetHeight();

			dMetricX = GetMetric( nMetric, nWidth );
			dMetricY = GetMetric( nMetric, nHeight );

			return TRUE;
		}

		IMediaFrame* ResizeAspectFrame( IMediaFrame* pSrcFrame )
	{
		if( !pSrcFrame )
			return NULL;

		BYTE* pBGRA = NULL;
		int nWidth = 0;
		int nHeight = 0;
		int nStride = 0;
		long lAspectX = 0;
		long lAspectY = 0;
		
		if( Core::c_nErrorNone != Core::COM::COM_GetBGRAImageFromInterface( pSrcFrame, pBGRA, nWidth, nHeight, &nStride, &lAspectX, &lAspectY ) )
			return NULL;

		if( lAspectX > 0 && lAspectY > 0 )
		{
			int nNewWidth = (nHeight * lAspectX + lAspectY - 1) / lAspectY;
			if( nNewWidth != nWidth )
			{
				BYTE* pBuffer = NULL;
				IMediaFrame* pResult = CreateBgraFrame( nNewWidth, nHeight, &pBuffer );
				if( !pResult )
					return NULL;

				if( nStride < 0 )
					pResult->put_ColorSpace( CSP_VFLIP | CSP_BGRA );
				
				IppiSize SrcSize = {nWidth, nHeight};
				IppiSize DstSize = {nNewWidth, nHeight};
				IppiRect SrcRect = {0, 0, nWidth, nHeight};

				ippiResize_8u_C4R( pBGRA, SrcSize, nWidth * 4, SrcRect, pBuffer, nNewWidth * 4, DstSize, double(nNewWidth)/nWidth, 1, IPPI_INTER_CUBIC );
			
				return pResult;
			}
		}

		pSrcFrame->AddRef();
		return pSrcFrame;
	}

		DWORD GetBaseRandom( int base )
		{
			if( base < 0 )
				return ::rand();

			return (DWORD)base;
		}

		class ImageCache
		{
		protected:
			typedef ATL::CAtlMap<CString, IUnknown*> ImageMap;

		protected:
			ImageMap m_mapImages;

		public:
			ImageCache()
			{
			}

			~ImageCache()
			{
				RemoveAll();
			}
			
			IUnknown* GetImage( const CString& sFilePath )
			{
				if( sFilePath.GetLength() <= 0 )
					return NULL;

				ImageMap::CPair* pPair = m_mapImages.Lookup( sFilePath );
				IUnknown* pInterface = NULL;

				if( pPair )
				{
					pInterface = pPair->m_value;
					if( pInterface )
					{
						return pInterface;
					}

					m_mapImages.RemoveKey( sFilePath );
				}

				Core::COM::COM_LoadImage( &pInterface, sFilePath, 0 );

				if( pInterface )
				{
					m_mapImages.SetAt( sFilePath, pInterface );
				}

				return pInterface;
			}
			
			void Clear()
			{
				POSITION pos = m_mapImages.GetStartPosition();

				while( pos )
				{
					POSITION temp = pos;
					IUnknown* pInterface = m_mapImages.GetNextValue( pos );

					if( pInterface )
					{
						if( pInterface->Release() < 1 )
						{
							pInterface = NULL;
							m_mapImages.RemoveAtPos( temp );
						}
						else
						{
							pInterface->AddRef();
						}
					}
				}
			}
			void RemoveAll()
			{
				POSITION pos = m_mapImages.GetStartPosition();

				while( pos )
				{
					IUnknown* pInterface = m_mapImages.GetNextValue( pos );

					if( pInterface )
					{
						pInterface->Release();
						pInterface = NULL;
					}
				}

				m_mapImages.RemoveAll();
			}
		};

		class AnimImageInfo
		{
		protected:
			int*  m_pFrameDelays;
			SIZE* m_pSizeFrames;
			int   m_nCountFrames;
			int   m_nLoopCount;
			int   m_nFileType; // tiff = 1, gif = 2 or any = 0

		public:
			int m_nCurFrame;
		
		public:
			AnimImageInfo()
			{
				m_pFrameDelays = NULL;
				m_pSizeFrames  = NULL;
				m_nCountFrames = 0;
				m_nLoopCount   = 0;
				m_nFileType    = 0; // any
				m_nCurFrame    = 0;
			}

			~AnimImageInfo()
			{
				if( m_pFrameDelays )
					delete [] m_pFrameDelays;

				if( m_pSizeFrames )
					delete [] m_pSizeFrames;
			}

			void Clear()
			{
				if( m_pFrameDelays )
				{
					delete [] m_pFrameDelays;
					m_pFrameDelays = NULL;
				}

				if( m_pSizeFrames )
				{
					delete [] m_pSizeFrames;
					m_pSizeFrames = NULL;
				}

				m_nCountFrames = 0;
				m_nLoopCount   = 0;
				m_nFileType    = 0; // any;
			}

			void SetAnimImageSwf( LPCWCH wsFilePath )
			{
				
			}

			void SetAnimImage( LPCWCH wsFilePath )
			{
				Clear();

				//FILE* pFile = ::_wfopen( wsFilePath, L"rb" );
				//if( !pFile )
				//	return;
				//
				//CxImage img;

				//long nFilePos = ftell( pFile );

				//img.SetEscape( -1 );
				//if( !img.Decode( pFile, 0 ) )
				//{
				//	fclose( pFile );
				//	return;
				//}

				//int m_nCountFrames = 1;

				//DWORD dwFileType = img.GetType();
				//switch( dwFileType )
				//{
				//case CXIMAGE_FORMAT_GIF:
				//case CXIMAGE_FORMAT_ICO:
				//case CXIMAGE_FORMAT_TIF:
				//case CXIMAGE_FORMAT_MNG:
				//	fseek( pFile, nFilePos, SEEK_SET );
				//	img.SetEscape( 0 );
				//	img.SetFrame( 0 );
				//	img.SetRetreiveAllFrames( true );
				//	img.Decode( pFile, dwFileType );
				//	m_nCountFrames = img.GetNumFrames();
				//	break;
				//}

				//if( dwFileType == CXIMAGE_FORMAT_TIF )
				//	m_nFileType = 1; // tiff
				//else
				//if( dwFileType == CXIMAGE_FORMAT_GIF )
				//	m_nFileType = 2; // gif

				//m_nLoopCount = 0;//((CxImageGIF*)(&img))->GetLoops();

				//if( m_nCountFrames <= 0 )
				//{
				//	fclose( pFile );
				//	return;
				//}

				//m_pFrameDelays = new int[m_nCountFrames];
				//m_pSizeFrames = new SIZE[m_nCountFrames];

				//if( m_pFrameDelays && m_pSizeFrames )
				//{
				//	::memset( m_pFrameDelays, 0, m_nCountFrames * sizeof(int) );
				//	::memset( m_pSizeFrames, 0, m_nCountFrames * sizeof(SIZE) );
				//	
				//	if( m_nCountFrames > 1 )
				//	{
				//		for( int i = 0; i < m_nCountFrames; i++ )
				//		{
				//			CxImage* frame = img.GetFrame( i );
				//			if( !frame )
				//				continue;

				//			m_pFrameDelays[i] = frame->GetFrameDelay();
				//			m_pSizeFrames[i].cx = frame->GetWidth();
				//			m_pSizeFrames[i].cy = frame->GetHeight();
				//		}
				//	}
				//	else
				//	{
				//		m_pSizeFrames[0].cx = img.GetWidth();
				//		m_pSizeFrames[0].cy = img.GetHeight();
				//	}
				//}

				//fclose( pFile );

				try
				{
					Gdiplus::Bitmap bitmap( wsFilePath );
					if( bitmap.GetLastStatus() != Gdiplus::Ok )
					{
						SetAnimImageSwf( wsFilePath );
						return;
					}

					GUID format;
					const GUID* pId = NULL;
					int nFrames = 0;

					// retrieve image format
					bitmap.GetRawFormat(&format);

					// check for format type
					if( format == Gdiplus::ImageFormatTIFF )
					{
						pId = &FrameDimensionPage;
						m_nFileType = 1; // tiff
					}
					else if( format == Gdiplus::ImageFormatGIF )
					{
						pId = &FrameDimensionTime;
						m_nFileType = 2; // gif
					}

					if( pId )
						nFrames = bitmap.GetFrameCount( pId );

					if( nFrames <= 0 )
						return;
					
					m_pFrameDelays = new int[nFrames];
					if( !m_pFrameDelays )
						return;

					::memset( m_pFrameDelays, 0, nFrames * sizeof(int) );

					m_nCountFrames = nFrames;

					int nSizeTag = bitmap.GetPropertyItemSize( PropertyTagLoopCount );
					if( nSizeTag > 0 )
					{
						PropertyItem* pProp = (PropertyItem*)(malloc(nSizeTag));
						if( pProp )
						{
							bitmap.GetPropertyItem( PropertyTagLoopCount, nSizeTag, pProp );

							m_nLoopCount = *(short*)(pProp->value);
							
							free(pProp);
						}
					}

					nSizeTag = bitmap.GetPropertyItemSize( PropertyTagFrameDelay );
					if( nSizeTag > 0 )
					{
						PropertyItem* pProp = (PropertyItem*)(malloc(nSizeTag));
						if( pProp )
						{
							bitmap.GetPropertyItem( PropertyTagFrameDelay, nSizeTag, pProp );
							if( nFrames * 4 == pProp->length )
							{
								for( int i = 0; i < nFrames; ++i )
								{
									m_pFrameDelays[i] = *((long*)(pProp->value) + i);
								}
							}

							free(pProp);
						}
					}

					if( m_nCountFrames )
					{
						m_pSizeFrames = new SIZE[m_nCountFrames];
						if( m_pSizeFrames )
						{
							const GUID* pGuidFrame = NULL;

							switch( m_nFileType )
							{
							case 1: pGuidFrame = &FrameDimensionPage; break;
							case 2: pGuidFrame = &FrameDimensionTime; break;
							}

							if( pGuidFrame )
							{
								for( int i = 0; i < m_nCountFrames; i++ )
								{
									bitmap.SelectActiveFrame( pGuidFrame, i );
									
									m_pSizeFrames[i].cx = bitmap.GetWidth();
									m_pSizeFrames[i].cy = bitmap.GetHeight();
								}
							}
							else
							{
								m_pSizeFrames[0].cx = bitmap.GetWidth();
								m_pSizeFrames[0].cy = bitmap.GetHeight();
							}
						}
					}
				}
				catch(...)
				{
				}
			}

			
			int GetCountFrames() const
			{
				return m_nCountFrames;
			}

			BOOL IsBadFrame( int nFrame ) const
			{
				return nFrame < 0 || nFrame >= m_nCountFrames;
			}

			int GetFrameDelay( int nFrame ) const
			{
				if( IsBadFrame( nFrame ) || !m_pFrameDelays )
					return 0;

				return m_pFrameDelays[nFrame];
			}

			int GetFrameWidth( int nFrame ) const
			{
				if( IsBadFrame( nFrame ) || !m_pSizeFrames )
					return -1;

				return m_pSizeFrames[nFrame].cx;
			}

			int GetFrameWidth() const
			{
				return GetFrameWidth( m_nCurFrame );
			}

			int GetFrameHeight( int nFrame ) const
			{
				if( IsBadFrame( nFrame ) || !m_pSizeFrames )
					return -1;

				return m_pSizeFrames[nFrame].cy;
			}

			int GetFrameHeight() const
			{
				return GetFrameHeight( m_nCurFrame );
			}

			int GetLoopCount() const
			{
				return m_nLoopCount;
			}
			
			int GetFileType() const
			{
				return m_nFileType;
			}
		};

		class ImageCLSID
		{
		protected:
			enum
			{
				Init_Bmp = (1<<1),
				Init_Jpg = (1<<2),
				Init_Gif = (1<<3),
				Init_Tif = (1<<4),
				Init_Png = (1<<5),
			};

			CLSID m_oCLSID[5];

			DWORD m_dwInit;

		public:
			enum
			{
				CLSID_Bmp = 0,
				CLSID_Jpg = 1,
				CLSID_Gif = 2,
				CLSID_Tif = 3,
				CLSID_Png = 4,
			};

			ImageCLSID()
			{
				m_dwInit = 0;
			}

			CLSID* GetCLSID( int type )
			{
				if( type < 0 || type > 4 )
					return NULL;

				if( !(m_dwInit & 1) )
				{
					m_dwInit |= 1;
					
					if( !GetEncoderClsid() )
						return NULL;
				}

				if( (m_dwInit & (1 << (type + 1))) )
					return &m_oCLSID[type];

				return NULL;
			}

		protected:
			BOOL GetEncoderClsid()
			{
				UINT  num = 0;          // number of image encoders
				UINT  size = 0;         // size of the image encoder array in bytes

				Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

				try
				{
					GetImageEncodersSize( &num, &size );
					if( !size )
						return FALSE;  // Failure

					pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
					if( !pImageCodecInfo )
						return FALSE;  // Failure

					GetImageEncoders( num, size, pImageCodecInfo );

					for( UINT i = 0; i < num; ++i )
					{
						const WCHAR* pStrType = pImageCodecInfo[i].MimeType;
						CLSID* pClsid = &pImageCodecInfo[i].Clsid;

						if( !(m_dwInit & Init_Bmp) )
						{
							if( 0 == wcscmp( pStrType, L"image/bmp" ) )
							{
								m_oCLSID[CLSID_Bmp] = *pClsid;
								m_dwInit |= Init_Bmp;
								continue;
							}
						}
						else if( !(m_dwInit & Init_Jpg) )
						{
							if( 0 == wcscmp( pStrType, L"image/jpeg" ) )
							{
								m_oCLSID[CLSID_Jpg] = *pClsid;
								m_dwInit |= Init_Jpg;
								continue;
							}
						}
						else if( !(m_dwInit & Init_Gif) )
						{
							if( 0 == wcscmp( pStrType, L"image/gif" ) )
							{
								m_oCLSID[CLSID_Gif] = *pClsid;
								m_dwInit |= Init_Gif;
								continue;
							}
						}
						else if( !(m_dwInit & Init_Tif) )
						{
							if( 0 == wcscmp( pStrType, L"image/tiff" ) )
							{
								m_oCLSID[CLSID_Tif] = *pClsid;
								m_dwInit |= Init_Tif;
								continue;
							}
						}
						else if( !(m_dwInit & Init_Png) )
						{
							if( 0 == wcscmp( pStrType, L"image/png" ) )
							{
								m_oCLSID[CLSID_Png] = *pClsid;
								m_dwInit |= Init_Png;
								continue;
							}
						}
					}

					free(pImageCodecInfo);
				}
				catch(...)
				{
					m_dwInit = 0;
					return FALSE;
				}

				return TRUE;
			}
		};

		
		class MetaFrame
		{
		// враппер на метаданные видеофрейма
		
		public:
			double m_dDuration;
			double m_dTimeStamp;

			long m_lAspectX;
			long m_lAspectY;

			VARIANT_BOOL m_vbInterlaced;
			VARIANT_BOOL m_vbDiscontinuity;

		public:
			MetaFrame()
			{
				Clear();
			}

			MetaFrame( IMediaFrame* pFrame )
			{
				FromMediaFrame( pFrame );
			}

			MetaFrame( IUnknown* pInterface )
			{
				FromInterface( pInterface );
			}

			
			void Clear()
			{
				m_dDuration = 0;
				m_dTimeStamp = 0;

				m_lAspectX = 0;
				m_lAspectY = 0;

				m_vbInterlaced = VARIANT_FALSE;
				m_vbDiscontinuity = VARIANT_FALSE;
			}

			BOOL FromMediaFrame( IMediaFrame* pFrame )
			{
				if( pFrame )
				{
					try
					{
						pFrame->get_Duration( &m_dDuration );
						pFrame->get_TimeStamp( &m_dTimeStamp );

						pFrame->get_AspectRatioX( &m_lAspectX );
						pFrame->get_AspectRatioY( &m_lAspectY );

						pFrame->get_Interlaced( &m_vbInterlaced );
						pFrame->get_IsDiscontinuity( &m_vbDiscontinuity );

						long lWidth; pFrame->get_Width( &lWidth );
						if( m_lAspectX == lWidth )
							m_lAspectX = 0;

						long lHeight; pFrame->get_Height( &lHeight );
						if( m_lAspectY == lHeight )
							m_lAspectY = 0;

						return TRUE;
					}
					catch(...)
					{
						ATLTRACE(_T("Exception! Is bad UncompressedVideoFrame ptr! (reading metadata)"));
					}
				}

				Clear();

				return FALSE;
			}

			BOOL FromInterface( IUnknown* pInterface )
			{
				if( pInterface )
				{
					IMediaFrame* pFrame = NULL;
					
					try
					{
						pInterface->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame );
					}
					catch(...)
					{
						ATLTRACE(_T("Exception! Is bad interface ptr! (query to UncompressedVideoFrame)"));
					}
					
					if( pFrame )
					{
						BOOL bSuccess = FromMediaFrame( pFrame );

						pFrame->Release();

						return bSuccess;
					}
				}

				Clear();

				return FALSE;
			}

			BOOL ToMediaFrame( IMediaFrame* pFrame ) const
			{
				if( pFrame )
				{
					try
					{
						pFrame->put_Duration( m_dDuration );
						pFrame->put_TimeStamp( m_dTimeStamp );

						pFrame->put_AspectRatioX( m_lAspectX );
						pFrame->put_AspectRatioY( m_lAspectY );

						pFrame->put_Interlaced( m_vbInterlaced );
						pFrame->put_IsDiscontinuity( m_vbDiscontinuity );

						return TRUE;
					}
					catch(...)
					{
						ATLTRACE(_T("Exception! Is bad UncompressedVideoFrame ptr! (writing metadata)"));
					}
				}

				return FALSE;
			}

			BOOL ToInterface( IUnknown* pInterface ) const
			{
				if( pInterface )
				{
					IMediaFrame* pFrame = NULL;
					
					try
					{
						pInterface->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame );
					}
					catch(...)
					{
						ATLTRACE(_T("Exception! Is bad interface ptr! (query to UncompressedVideoFrame)"));
					}
					
					if( pFrame )
					{
						BOOL bSuccess = ToMediaFrame( pFrame );

						pFrame->Release();

						return bSuccess;
					}
				}

				return FALSE;
			}
		
			static void Copy( IMediaFrame* src, IMediaFrame* dst )
			{
				MetaFrame data( src );
				data.ToMediaFrame( dst );
			}
		};
		
		class CSourceItem
		{
		protected:
			IMediaFrame*       m_pFrame;
			Gdiplus::Bitmap*   m_pBitmap;
			IO::CMetaFile*     m_pMetafile;

		public:
			BOOL m_bReadOnly;
			BOOL m_bUpdateMetafile;

		public:
			CSourceItem()
			{
				m_pFrame    = NULL;
				m_pBitmap   = NULL;
				m_pMetafile = NULL;
				
				m_bReadOnly = FALSE;
				m_bUpdateMetafile = FALSE;
			}

			~CSourceItem()
			{
				Clear();
			}

			void Clear()
			{
				ReleaseBitmap();
				ReleaseFrame();
				ReleaseMetafile();
			}

			void SetFrame( IMediaFrame* pFrame )
			{
				Clear();

				m_pFrame = pFrame;
			}

			void SetBitmap( Gdiplus::Bitmap* pBitmap, IMediaFrame* pMetadata )
			{
				Clear();

				m_pFrame = pMetadata;
				m_pBitmap = pBitmap;
			}
			void SetBitmap( Gdiplus::Bitmap* pBitmap, BOOL bUseOwnMetadata = TRUE )
			{
				IMediaFrame* pMetadata = bUseOwnMetadata ? m_pFrame : NULL;
				if( pMetadata )
					pMetadata->AddRef();

				SetBitmap( pBitmap, pMetadata );
			}

			void SetMetafile( IO::CMetaFile* pMetafile )
			{
				Clear();

				m_pMetafile = pMetafile;

				m_bUpdateMetafile = TRUE;
			}

			BOOL IsEmpty() const
			{
				return !m_pFrame && !m_pMetafile;
			}

			BOOL IsMetafile() const
			{
				return m_pMetafile != NULL;
			}

			
			IMediaFrame* GetFrame()
			{
				ReleaseBitmap();

				if( m_pMetafile )
				{
					if( m_bUpdateMetafile )
					{
						SetFrame( m_pMetafile->ConvertToBitmap() );
					}
					else
					{
						ReleaseMetafile();
					}
				}

				return m_pFrame;
			}

			IMediaFrame* GetResultFrame()
			{
				ReleaseBitmap();

				if( m_pMetafile )
				{
					if( m_bUpdateMetafile )
					{
						m_bUpdateMetafile = FALSE;

						ReleaseFrame();
					
						m_pFrame = m_pMetafile->ConvertToBitmap();
					}
				}

				return m_pFrame;
			}


			BYTE* CheckFrameForWriting( int nWidth, int nHeight, int &nStride )
			{
				ReleaseBitmap();
				ReleaseMetafile();
				
				IMediaFrame* frame = m_pFrame;

				if( !frame )
				{
					CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&frame) );
					if( !frame )
						return NULL;
				}

				long lAspectX, lAspectY;
				frame->get_AspectRatioX( &lAspectX );
				frame->get_AspectRatioY( &lAspectY );

				long lWidth, lHeight;
				frame->get_Width( &lWidth );
				frame->get_Height( &lHeight );

				if( m_bReadOnly )
				{
					frame = NULL;
					CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&frame) );
					if( !frame )
						return NULL;

					MetaFrame::Copy( m_pFrame, frame );
				}

				frame->put_Width( nWidth );
				frame->put_Height( nHeight );
				frame->put_ColorSpace( CSP_BGRA );
				frame->raw_SetDefaultStrides();
				frame->raw_AllocateBuffer( -1 );

				BYTE* pBuffer = 0;
				frame->get_Buffer( &pBuffer );
				if( !pBuffer )
				{
					frame->Release();
					return NULL;
				}

				long lStride;
				frame->get_Stride( 0, &lStride );
				nStride = lStride;

				if( lAspectX <= 0 || lAspectX == lWidth  ) frame->put_AspectRatioX( nWidth );
				if( lAspectY <= 0 || lAspectY == lHeight ) frame->put_AspectRatioY( nHeight );

				if( m_pFrame != frame )
				{
					ReleaseFrame();
					m_pFrame = frame;
				}

				return pBuffer;
			}

			Gdiplus::Bitmap* GetBitmap()
			{
				return m_pBitmap;
			}
			IO::CMetaFile* GetMetafile()
			{
				return m_pMetafile;
			}

	
			BOOL GetMetric( int nMetric, double &dMetricX, double &dMetricY ) const
			{
				long lWidth, lHeight;

				if( m_pMetafile )
				{
					lWidth  = m_pMetafile->GetWidth();
					lHeight = m_pMetafile->GetHeight();
				}
				else
				if( m_pFrame )
				{
					m_pFrame->get_Width( &lWidth );
					m_pFrame->get_Height( &lHeight );
				}
				else
				{
					return FALSE;
				}

				if( lWidth <= 0 || lHeight <= 0 )
					return FALSE;
				
				dMetricX = ImageStudio::Transforms::GetMetric( nMetric, lWidth );
				dMetricY = ImageStudio::Transforms::GetMetric( nMetric, lHeight );

				return TRUE;
			}
			int GetCountPixels() const
			{
				long lWidth, lHeight;

				if( m_pMetafile )
				{
					lWidth  = m_pMetafile->GetWidth();
					lHeight = m_pMetafile->GetHeight();
				}
				else
				if( m_pFrame )
				{
					m_pFrame->get_Width( &lWidth );
					m_pFrame->get_Height( &lHeight );
				}
				else
				{
					return 0;
				}
				
				return lWidth * lHeight;
			}
			
			static IMediaFrame* BitmapToBgraFrame( Gdiplus::Bitmap* pBitmap, IMediaFrame* pMetadata )
			{
				if( !pBitmap )
					return NULL;

				int nWidth  = pBitmap->GetWidth();
				int nHeight = pBitmap->GetHeight();
				
				if( nWidth <= 0 || nHeight <= 0 )
					return NULL;

				IMediaFrame* pResult = CreateFrame( pMetadata, FALSE, TRUE );
				if( !pResult )
					return NULL;

				
				pResult->put_ColorSpace( CSP_BGRA );
				pResult->put_Width( nWidth );
				pResult->put_Height( nHeight );

				pResult->raw_SetDefaultStrides();
				pResult->raw_AllocateBuffer( -1 );

				long lAspectX = 0; pResult->get_AspectRatioX( &lAspectX );
				if( lAspectX <= 0 )
					pResult->put_AspectRatioX( nWidth );
				
				long lAspectY = 0; pResult->get_AspectRatioY( &lAspectY );
				if( lAspectY <= 0 )
					pResult->put_AspectRatioY( nHeight );


				BYTE* pBuffer = NULL; pResult->get_Buffer( &pBuffer );
				long nStride = 0; pResult->get_Stride( 0, &nStride );

				Gdiplus::BitmapData data;
				
				if( Gdiplus::Ok != pBitmap->LockBits( NULL, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data ) )
				{
					pResult->Release();
					return NULL;
				}

				BYTE* pSrc = (BYTE*)data.Scan0;
				BYTE* pDst = pBuffer;

				for( int y = 0; y < nHeight; ++y, pDst += nStride, pSrc += data.Stride )
				{
					::memcpy( pDst, pSrc, nWidth * 4 );
				}
					
				pBitmap->UnlockBits( &data );

				return pResult;
			}

			static IMediaFrame* CreateFrame( IMediaFrame* pSource, BOOL bCopyData, BOOL bCopyMetadataOnly )
			{
				IMediaFrame* pResult = NULL;

				if( bCopyData )
				{
					if( pSource )
					{
						MediaCore::IAVSMediaData* pClone = NULL;
						pSource->raw_CreateDuplicate( DUBLICATE_TYPE_COPY, &pClone );
						if( pClone )
						{
							pClone->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pResult );
							pClone->Release();
						}
					}
				}
				else
				{
					CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pResult) );
					if( pResult && pSource )
					{
						long lValue;
						pSource->get_AspectRatioX( &lValue ); pResult->put_AspectRatioX( lValue );
						pSource->get_AspectRatioY( &lValue ); pResult->put_AspectRatioY( lValue );
						pSource->get_ColorSpace( &lValue ); pResult->put_ColorSpace( (lValue & ~CSP_COLOR_MASK) | CSP_BGRA );

						if( !bCopyMetadataOnly )
						{
							pSource->get_Width( &lValue ); pResult->put_Width( lValue );
							pSource->get_Height( &lValue ); pResult->put_Height( lValue );

							pResult->raw_SetDefaultStrides();
							pResult->raw_AllocateBuffer( -1 );
						}

						double dValue;
						pSource->get_Duration( &dValue ); pResult->put_Duration( dValue );
						pSource->get_TimeStamp( &dValue ); pResult->put_TimeStamp( dValue );

						VARIANT_BOOL vbValue;
						pSource->get_Interlaced( &vbValue ); pResult->put_Interlaced( vbValue );
						pSource->get_IsDiscontinuity( &vbValue ); pResult->put_IsDiscontinuity( vbValue );
					}
				}

				return pResult;
			}

			static IMediaFrame* Clone( IMediaFrame* pSource )
			{
				return CreateFrame( pSource, TRUE, FALSE );
			}

		protected:
			void ReleaseFrame()
			{
				RELEASEINTERFACE(m_pFrame);
				
				m_bReadOnly = FALSE;
			}

			void ReleaseBitmap()
			{
				if( m_pBitmap )
				{
					delete m_pBitmap;
					m_pBitmap = NULL;
				}
			}
			void ReleaseMetafile()
			{
				if( m_pMetafile )
				{
					delete m_pMetafile;
					m_pMetafile = NULL;
				}

				m_bUpdateMetafile = FALSE;
			}

		};

		class CSourceArray
		{
		protected:
			static const int c_nCountSourcesDefault = 16;
			static const int c_nMaxCountSources = 65536;

			
		protected:
			CSourceItem* m_pSources; // буффер с ресурсами
			int m_nCountSources; // количество ресурсов

			MediaFormat::IAVSVideoFormat*       m_pFormat;
			MediaCore::IAVSVideoFrameTransform* m_pTransform;

		public:
			CSourceArray()
			{
				m_pSources = new CSourceItem[c_nCountSourcesDefault];
				m_nCountSources = m_pSources ? c_nCountSourcesDefault : 0;

				m_pFormat = NULL;
				m_pTransform = NULL;
			}
			~CSourceArray()
			{
				Release();

				RELEASEINTERFACE(m_pFormat);
				RELEASEINTERFACE(m_pTransform);
			}

			
			void Release()
			{
				if( m_pSources )
				{
					delete [] m_pSources;
					
					m_pSources = NULL;
					m_nCountSources = 0;
				}
			}

			BOOL Resize( int size )
			{
				if( size <= 0 || size > c_nMaxCountSources )
					return FALSE;

				if( size == m_nCountSources )
					return TRUE;

				CSourceItem* pNewBuf = new CSourceItem[size];
				if( !pNewBuf )
					return FALSE;

				int nCopyCount = min(size, m_nCountSources);
				if( nCopyCount > 0 )
				{
					::memcpy( pNewBuf, m_pSources, sizeof(CSourceItem) * nCopyCount );
				}

				if( nCopyCount < size )
				{
					::memset( pNewBuf + nCopyCount, 0, (size - nCopyCount) * sizeof(CSourceItem) );
				}

				for( int i = nCopyCount; i < m_nCountSources; i++ )
				{
					m_pSources[i].Clear();
				};

				char* pBuffer = reinterpret_cast<char*>(m_pSources);
				delete [] pBuffer;

				m_pSources = pNewBuf;
				m_nCountSources = size;

				return TRUE;
			}

			
			BOOL IsBadIndex( int nIndex ) const
			{
				return nIndex < 0 || nIndex >= m_nCountSources;
			}

			BOOL IsGoodIndex( int nIndex ) const
			{
				return !IsBadIndex( nIndex );
			}

			BOOL IsEmptySource( int nIndex ) const
			{
				if( IsBadIndex( nIndex ) )
					return TRUE;

				return m_pSources[nIndex].IsEmpty();
			}
			BOOL IsValidSource( int nIndex ) const
			{
				return !IsEmptySource( nIndex );
			}

			BOOL Clear( int nIndex )
			{
				if( IsBadIndex( nIndex ) )
					return FALSE;

				m_pSources[nIndex].Clear();
				
				return TRUE;
			}

			BOOL Copy( int nSrcIndex, int nDstIndex )
			{
				if( nSrcIndex == nDstIndex )
					return TRUE;

				if( IsBadIndex( nSrcIndex ) || IsBadIndex( nDstIndex ) )
					return FALSE;

				m_pSources[nDstIndex].Clear();

				IMediaFrame* pSource = m_pSources[nSrcIndex].GetFrame();
				if( pSource )
				{
					//pSource->AddRef();
					
					m_pSources[nDstIndex].SetFrame( CSourceItem::CreateFrame( pSource, TRUE, FALSE ) );
					m_pSources[nDstIndex].m_bReadOnly = FALSE;
				}

				return TRUE;
			}

			
			BOOL SetSource( int nIndex, IUnknown* pSource, BOOL bReplace, BOOL bReadOnly )
			{
				if( IsBadIndex( nIndex ) )
					return FALSE;

				m_pSources[nIndex].Clear();

				if( !pSource )
					return TRUE;
				
				IMediaFrame* pFrame = NULL;
				pSource->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pFrame) );
				
				if( bReplace )
				{
					pSource->Release();

					m_pSources[nIndex].SetFrame( pFrame );
					m_pSources[nIndex].m_bReadOnly = bReadOnly;
				}
				else
				{
					if( bReadOnly )
					{
						m_pSources[nIndex].SetFrame( ConvertToBgra( pFrame ) );

						if( pFrame )
							pFrame->Release();
					}
					else
					{
						m_pSources[nIndex].SetFrame( pFrame );
					}
					
					m_pSources[nIndex].m_bReadOnly = FALSE;
				}


				return TRUE;
			}

			IUnknown* GetResultFrame( int nIndex )
			{
				if( IsBadIndex( nIndex ) )
					return NULL;

				IUnknown* pResult = m_pSources[nIndex].GetResultFrame();
				if( pResult )
				{
					pResult->AddRef();
				}

				//m_pSources[nIndex].pFrame = NULL;

				return pResult;
			}

			
			BOOL Set( int nIndex, IUnknown* pSource )
			{
				return SetSource( nIndex, pSource, TRUE, FALSE );
			}
			IMediaFrame* Get( int nIndex )
			{
				if( IsBadIndex( nIndex ) )
					return NULL;

				IMediaFrame* pSource = m_pSources[nIndex].GetFrame();
				if( !pSource )
					return NULL;
				
				long lColorSpace = 0; pSource->get_ColorSpace( &lColorSpace );
				if( (lColorSpace & CSP_COLOR_MASK) != CSP_BGRA )
				{
					IMediaFrame* pResult = ConvertToBgra( pSource );

					m_pSources[nIndex].SetFrame( pResult );
					m_pSources[nIndex].m_bReadOnly = FALSE;

					return pResult;
				}

				if( m_pSources[nIndex].m_bReadOnly )
				{
					IMediaFrame* pResult = CSourceItem::CreateFrame( pSource, TRUE, FALSE );
					
					m_pSources[nIndex].SetFrame( pResult );
					m_pSources[nIndex].m_bReadOnly = FALSE;

					return pResult;
				}

				return pSource;
			}

			IMediaFrame* CloneToBgra( int nIndex, BOOL bCopyData )
			{
				if( IsBadIndex( nIndex ) )
					return NULL;

				IMediaFrame* pSource = m_pSources[nIndex].GetFrame();
				if( !pSource )
					return NULL;

				long lColorSpace = 0; pSource->get_ColorSpace( &lColorSpace );
				if( (lColorSpace & CSP_COLOR_MASK) != CSP_BGRA )
				{
					if( bCopyData )
						return ConvertToBgra( pSource );
				}

				return CSourceItem::CreateFrame( pSource, bCopyData, FALSE );
			}

			
			Gdiplus::Bitmap* GetBitmap( int nIndex )
			{
				if( IsBadIndex( nIndex ) )
					return NULL;

				Gdiplus::Bitmap* pResult = m_pSources[nIndex].GetBitmap();
				if( pResult )
					return pResult;

				IMediaFrame* pFrame = m_pSources[nIndex].GetFrame();
				if( !pFrame )
					return NULL;

				long lColorSpace = 0; pFrame->get_ColorSpace( &lColorSpace );
				if( (lColorSpace & CSP_COLOR_MASK) != CSP_BGRA )
				{
					pFrame = ConvertToBgra( pFrame );
					if( !pFrame )
						return NULL;

					m_pSources[nIndex].SetFrame( pFrame );
					m_pSources[nIndex].m_bReadOnly = FALSE;
				}
				else
				{
					if( m_pSources[nIndex].m_bReadOnly )
					{
						pFrame = CSourceItem::Clone( pFrame );
						
						m_pSources[nIndex].SetFrame( pFrame );
						m_pSources[nIndex].m_bReadOnly = FALSE;
					}
				}

				long lWidth   = 0; pFrame->get_Width( &lWidth );
				long lHeight  = 0; pFrame->get_Height( &lHeight );
				long lStride  = 0; pFrame->get_Stride( 0, &lStride );
				long lBufSize = 0; pFrame->get_BufferSize( &lBufSize );
				BYTE* pBuffer = 0; pFrame->get_Buffer( &pBuffer );

				if( !pBuffer || lWidth <= 0 || lHeight <= 0 || lStride < lWidth * 4 || lBufSize < lStride * lHeight )
				{
					return NULL;
				}

				if( lColorSpace & CSP_VFLIP )
				{
					pBuffer += lStride * (lHeight - 1);
					lStride = -lStride;
				}

				pResult = new Gdiplus::Bitmap( lWidth, lHeight, lStride, PixelFormat32bppARGB, pBuffer );

				m_pSources[nIndex].SetBitmap( pResult );

				return pResult;
			}

			IDirect3DTexture9* GetTexture( int nIndex, IDirect3DDevice9* pDevice, HRESULT &nRetCode )
			{
				nRetCode = D3D_OK;

				if( !pDevice )
					return NULL;

				IMediaFrame* pFrame = m_pSources[nIndex].GetFrame();
				if( !pFrame )
					return NULL;

				long lColorSpace = 0; pFrame->get_ColorSpace( &lColorSpace );
				if( (lColorSpace & CSP_COLOR_MASK) != CSP_BGRA )
				{
					pFrame = ConvertToBgra( pFrame );
					if( !pFrame )
						return NULL;

					m_pSources[nIndex].SetFrame( pFrame );
					m_pSources[nIndex].m_bReadOnly = FALSE;
				}

				long lWidth   = 0; pFrame->get_Width( &lWidth );
				long lHeight  = 0; pFrame->get_Height( &lHeight );
				long lStride  = 0; pFrame->get_Stride( 0, &lStride );
				long lBufSize = 0; pFrame->get_BufferSize( &lBufSize );
				BYTE* pBuffer = 0; pFrame->get_Buffer( &pBuffer );

				if( !pBuffer || lWidth <= 0 || lHeight <= 0 || lStride < lWidth * 4 || lBufSize < lStride * lHeight )
				{
					return NULL;
				}

				if( lColorSpace & CSP_VFLIP )
				{
					pBuffer += lStride * (lHeight - 1);
					lStride = -lStride;
				}

				IDirect3DTexture9* pSource = NULL;
				IDirect3DTexture9* pResult = NULL;
				
				HRESULT hr = D3D_OK;
				BOOL bSuccess = FALSE;
				do
				{
					hr = D3DXCreateTexture( pDevice, lWidth, lHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pSource );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					D3DLOCKED_RECT data;
					hr = pSource->LockRect( 0, &data, NULL, 0 );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					if( !MemCopy( pBuffer, (BYTE*)data.pBits, lStride, data.Pitch, lHeight, lWidth * 4 ) )
						break;
				
					pSource->UnlockRect( 0 );

					hr = D3DXCreateTexture( pDevice, lWidth, lHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pResult );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					hr = pDevice->UpdateTexture( pSource, pResult );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					bSuccess = TRUE;
				}while(0);

				RELEASEINTERFACE(pSource);
				nRetCode = hr;

				if( !bSuccess )
				{
					RELEASEINTERFACE(pResult);
					return NULL;
				}

				return pResult;
			}

			BOOL SetTexture( int nIndex, IDirect3DTexture9* pSource, HRESULT &nRetCode )
			{
				nRetCode = D3D_OK;

				if( !pSource || IsBadIndex( nIndex ) )
					return FALSE;

				HRESULT hr = D3D_OK;
				BOOL bSuccess = FALSE;
				do
				{
					D3DSURFACE_DESC desc;
					hr = pSource->GetLevelDesc( 0, &desc );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}
					
					D3DLOCKED_RECT data;
					hr = pSource->LockRect( 0, &data, NULL, /*(desc.Pool != D3DPOOL_SYSTEMMEM ?*/ D3DLOCK_READONLY /*: 0)*/ );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					int nStride = 0;
					BYTE* pBuffer = m_pSources[nIndex].CheckFrameForWriting( desc.Width, desc.Height, nStride );
					if( !pBuffer )
						break;

					if( !MemCopy( (BYTE*)data.pBits, pBuffer, data.Pitch, nStride, desc.Height, desc.Width * 4 ) )
						break;

					pSource->UnlockRect( 0 );
				
					bSuccess = TRUE;
				}while(0);

				nRetCode = hr;
				return bSuccess;
			}

			
			BOOL GetMetric( int nIndex, int nMetric, double& dMetricX, double& dMetricY ) const
			{
				if( IsBadIndex( nIndex ) )
					return FALSE;

				return m_pSources[nIndex].GetMetric( nMetric, dMetricX, dMetricY );
			}

			BOOL GetCountPixels( int nIndex ) const
			{
				if( IsBadIndex( nIndex ) )
					return FALSE;

				return m_pSources[nIndex].GetCountPixels();
			}

		
			BOOL IsMetafile( int nIndex ) const
			{
				if( IsBadIndex( nIndex ) )
					return FALSE;

				return m_pSources[nIndex].IsMetafile();
			}

			IO::CMetaFile* GetMetafile( int nIndex )
			{
				if( IsBadIndex( nIndex ) )
					return NULL;

				return m_pSources[nIndex].GetMetafile();
			}

			BOOL SetMetafile( int nIndex, IO::CMetaFile* pMetafile )
			{
				if( IsBadIndex( nIndex ) )
					return FALSE;

				m_pSources[nIndex].SetMetafile( pMetafile );
				
				return TRUE;
			}

			void UpdateMetafile( int nIndex )
			{
				if( IsMetafile( nIndex ) )
				{
					m_pSources[nIndex].m_bUpdateMetafile = TRUE;
				}
			}

		protected:
			BOOL CheckTransform()
			{
				if( m_pTransform )
					return TRUE;

				CoCreateInstance( MediaCore::CLSID_CAVSVideoFrameTransform, NULL, CLSCTX_ALL, MediaCore::IID_IAVSVideoFrameTransform, (void**)(&m_pTransform) );

				return m_pTransform != NULL;
			}

			BOOL CheckFormat()
			{
				if( m_pFormat )
					return TRUE;

				CoCreateInstance( MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)(&m_pFormat) );
				
				if( m_pFormat )
				{
					m_pFormat->SetDefaultProperties();
					m_pFormat->put_ColorSpace( CSP_BGRA /*| CSP_VFLIP*/ );
					return TRUE;
				}

				return FALSE;
			}

			IMediaFrame* ConvertToBgra( IMediaFrame* pSource )
			{
				if( !pSource )
					return NULL;

				if( !CheckFormat() || !CheckTransform() )
					return NULL;

				// настраиваем формат для конвертации
				long lWidth;   pSource->get_Width( &lWidth );
				long lHeight;  pSource->get_Height( &lHeight );
				long lAspectX; pSource->get_AspectRatioX( &lAspectX );
				long lAspectY; pSource->get_AspectRatioY( &lAspectY );
				
				m_pFormat->put_Width( lWidth );
				m_pFormat->put_Height( lHeight );
				m_pFormat->put_AspectRatioX( lAspectX );
				m_pFormat->put_AspectRatioY( lAspectY );

				IUnknown* pResultInterface = NULL;

				m_pTransform->raw_SetVideoFormat( m_pFormat );
				m_pTransform->raw_TransformFrame( pSource, &pResultInterface );

				// если трансформ не сделал дупликата - делаем его руками
				if (pSource == pResultInterface) // здесь (pResult != NULL)
				{
					pResultInterface->Release();
					pResultInterface = NULL;
					
					pSource->raw_CreateDuplicate(DUBLICATE_TYPE_COPY, (MediaCore::IAVSMediaData**)(&pResultInterface));
				}

				if( !pResultInterface )
					return NULL;

				IMediaFrame* pResult = NULL;
				pResultInterface->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pResult) );
				pResultInterface->Release();

				if( !pResult )
					return NULL;

				// если исходный формат был не BGRA (т.е. не содержал alpha канал), то его надо заполнить руками значением 255
				long lColorSpace; pSource->get_ColorSpace( &lColorSpace );

				if( (lColorSpace & CSP_COLOR_MASK) != CSP_BGRA )
				{
					BYTE* pBuffer; pResult->get_Buffer( &pBuffer );
					if( pBuffer )
					{
						long lSize  = lWidth * lHeight;
						long lExtra = lSize & 7;

						lSize >>= 3;

						BYTE* dst = pBuffer;

						for( long i = 0; i < lSize; ++i, dst += 4 * 8 )
						{
							dst[ 3] = 255;
							dst[ 7] = 255;
							dst[11] = 255;
							dst[15] = 255;
							dst[19] = 255;
							dst[23] = 255;
							dst[27] = 255;
							dst[31] = 255;
						}

						for( long i = 0; i < lExtra; ++i, dst += 4 )
						{
							dst[3] = 255;
						}
					}
				}

				return pResult;
			}

		};

		
		class SerializedObjectTransformerPaintTextStatic
		{
		public:

			SerializedObjectTransformerPaintTextStatic()
			{
				m_pTransformer = NULL;
				m_bTransformerValid = TRUE;
			}
			virtual ~SerializedObjectTransformerPaintTextStatic()
			{
				Destroy();
			}
			
			BOOL IsValid()
			{
				if (NULL == m_pTransformer)
					return FALSE;

				return TRUE;
			}
			
			BOOL Transform_SetBrush(ImageStudio::Serialize::Paint::TextStatic::SetBrush* pAction, double dTime)
			{
				if (!pAction)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				BSTR bstrPath = pAction->Brush.TexturePath.AllocSysString();
				m_pTransformer->put_brushTexturePath(bstrPath); SysFreeString(bstrPath);
				m_pTransformer->put_brushTextureAlpha(pAction->Brush.TextureAlpha);
				m_pTransformer->put_brushTextureMode(pAction->Brush.TextureMode);
				m_pTransformer->put_brushColor1(pAction->Brush.Color1);
				m_pTransformer->put_brushColor2(pAction->Brush.Color2);
				m_pTransformer->put_brushAlpha1(pAction->Brush.Alpha1);
				m_pTransformer->put_brushAlpha2(pAction->Brush.Alpha2);
				m_pTransformer->put_brushType(pAction->Brush.Type);

				return TRUE;
			}
			BOOL Transform_SetFont(ImageStudio::Serialize::Paint::TextStatic::SetFont* pAction, double dTime)
			{
				if (!pAction)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				BSTR bstrName = pAction->Font.Name.AllocSysString();
				m_pTransformer->put_fontName(bstrName); SysFreeString(bstrName);
				m_pTransformer->put_fontSize(pAction->Font.Size);
				m_pTransformer->put_fontBold(pAction->Font.Bold ? VARIANT_TRUE : VARIANT_FALSE);
				m_pTransformer->put_fontItalic(pAction->Font.Italic ? VARIANT_TRUE : VARIANT_FALSE);
				m_pTransformer->put_fontUnderline(pAction->Font.Underline ? VARIANT_TRUE : VARIANT_FALSE);
				m_pTransformer->put_fontStrikeout(pAction->Font.Strikeout ? VARIANT_TRUE : VARIANT_FALSE);

				return TRUE;
			}
			BOOL Transform_SetFormat(ImageStudio::Serialize::Paint::TextStatic::SetFormat* pAction, double dTime)
			{
				if (!pAction)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				m_pTransformer->put_formatRotate(pAction->Angle);
				m_pTransformer->put_formatAlignVertical(pAction->Format.StringAlignmentVertical);
				m_pTransformer->put_formatAlignHorizontal(pAction->Format.StringAlignmentHorizontal);

				return TRUE;
			}
			BOOL Transform_SetShadow(ImageStudio::Serialize::Paint::TextStatic::SetShadow* pAction, double dTime)
			{
				if (!pAction)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				m_pTransformer->put_shadowVisible(pAction->Shadow.Visible ? VARIANT_TRUE : VARIANT_FALSE);
				m_pTransformer->put_shadowDistanceX(int(pAction->Shadow.DistanceX));
				m_pTransformer->put_shadowDistanceY(int(pAction->Shadow.DistanceY));
				m_pTransformer->put_shadowColor(pAction->Shadow.Color);
				m_pTransformer->put_shadowAlpha(pAction->Shadow.Alpha);
				m_pTransformer->put_shadowBlur(int(pAction->Shadow.BlurSize));

				return TRUE;
			}
			BOOL Transform_SetEdge(ImageStudio::Serialize::Paint::TextStatic::SetEdge* pAction, double dTime)
			{
				if (!pAction)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				m_pTransformer->put_edgeVisible(pAction->Edge.Visible ? VARIANT_TRUE : VARIANT_FALSE);
				m_pTransformer->put_edgeDistance(int(pAction->Edge.Dist));
				m_pTransformer->put_edgeColor(pAction->Edge.Color);
				m_pTransformer->put_edgeAlpha(pAction->Edge.Alpha);

				return TRUE;
			}
			BOOL Transform_DrawInRect(ImageStudio::Serialize::Paint::TextStatic::DrawInRect* pAction, double dTime, IUnknown* pSource, IUnknown*& pResult)
			{
				if (!pAction || !pSource)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(dTime);
				if (dCompleteness <= 0.0001)
					return Core::c_nErrorNone; //(Core::c_nErrorNone == ImageStudio::Transforms::Core::COM::COM_DulicateImage(pSource, pResult, TRUE));
				
				if (!Create())
					return FALSE;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pSource, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				pResult = pSource;

				BSTR bstrText = pAction->Text.AllocSysString();

				// font
				if (TRUE)
				{
					if (pAction->Font.IsValidName())
					{
						BSTR bstrName = pAction->Font.GetName().AllocSysString();
						m_pTransformer->put_fontName(bstrName);
						SysFreeString(bstrName);
					}

					if (pAction->Font.IsValidSize())						m_pTransformer->put_fontSize(pAction->Font.GetSize());
					if (pAction->Font.IsValidBold())						m_pTransformer->put_fontBold(pAction->Font.GetBold() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidItalic())						m_pTransformer->put_fontItalic(pAction->Font.GetItalic() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidUnderline())					m_pTransformer->put_fontUnderline(pAction->Font.GetUnderline() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidStrikeout())					m_pTransformer->put_fontStrikeout(pAction->Font.GetStrikeout() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidStringAlignmentVertical())		m_pTransformer->put_formatAlignVertical(pAction->Font.GetStringAlignmentVertical());
					if (pAction->Font.IsValidStringAlignmentHorizontal())	m_pTransformer->put_formatAlignHorizontal(pAction->Font.GetStringAlignmentHorizontal());
					if (pAction->Font.IsValidAngle())						m_pTransformer->put_formatRotate((long)pAction->Font.GetAngle());
				}

				// brush
				if (TRUE)
				{
					if (pAction->Brush.IsValidTexturePath())
					{
						BSTR bstrPath = pAction->Brush.GetTexturePath().AllocSysString();
						m_pTransformer->put_brushTexturePath(bstrPath);
						SysFreeString(bstrPath);
					}

					if (pAction->Brush.IsValidTextureAlpha())	m_pTransformer->put_brushTextureAlpha(pAction->Brush.GetTextureAlpha());
					if (pAction->Brush.IsValidTextureMode())	m_pTransformer->put_brushTextureMode(pAction->Brush.GetTextureMode());
					if (pAction->Brush.IsValidColor1())			m_pTransformer->put_brushColor1(pAction->Brush.GetColor1());
					if (pAction->Brush.IsValidColor2())			m_pTransformer->put_brushColor2(pAction->Brush.GetColor2());
					if (pAction->Brush.IsValidAlpha1())			m_pTransformer->put_brushAlpha1(pAction->Brush.GetAlpha1());
					if (pAction->Brush.IsValidAlpha2())			m_pTransformer->put_brushAlpha2(pAction->Brush.GetAlpha2());
					if (pAction->Brush.IsValidType())			m_pTransformer->put_brushType(pAction->Brush.GetType());
				}

				// shadow
				if (TRUE)
				{
					if (pAction->Shadow.IsValidVisible())		m_pTransformer->put_shadowVisible(pAction->Shadow.GetVisible() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Shadow.IsValidDistanceX())		m_pTransformer->put_shadowDistanceX(long(pAction->Shadow.GetDistanceX()));
					if (pAction->Shadow.IsValidDistanceY())		m_pTransformer->put_shadowDistanceY(long(pAction->Shadow.GetDistanceY()));
					if (pAction->Shadow.IsValidBlurSize())		m_pTransformer->put_shadowBlur(long(pAction->Shadow.GetBlurSize()));
					if (pAction->Shadow.IsValidColor())			m_pTransformer->put_shadowColor(pAction->Shadow.GetColor());
					if (pAction->Shadow.IsValidAlpha())			m_pTransformer->put_shadowAlpha(pAction->Shadow.GetAlpha());
				}

				// edge
				if (TRUE)
				{
					if (pAction->Edge.IsValidVisible())			m_pTransformer->put_edgeVisible(pAction->Edge.GetVisible() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Edge.IsValidDist())			m_pTransformer->put_edgeDistance(long(pAction->Edge.GetDist()));
					if (pAction->Edge.IsValidColor())			m_pTransformer->put_edgeColor(pAction->Edge.GetColor());
					if (pAction->Edge.IsValidAlpha())			m_pTransformer->put_edgeAlpha(pAction->Edge.GetAlpha());
				}

				// отрисовываем полупрозначный объект, если completeness не 1
				long nBrushAlpha1  = 0; m_pTransformer->get_brushAlpha1(&nBrushAlpha1);
				long nBrushAlpha2  = 0; m_pTransformer->get_brushAlpha2(&nBrushAlpha2);
				long nTextureAlpha = 0; m_pTransformer->get_brushTextureAlpha(&nTextureAlpha);
				long nShadowAlpha  = 0; m_pTransformer->get_shadowAlpha(&nShadowAlpha);
				long nEdgeAlpha    = 0; m_pTransformer->get_edgeAlpha(&nEdgeAlpha);
				double dFontSize   = 0; m_pTransformer->get_fontSize(&dFontSize);
				
				m_pTransformer->put_brushAlpha1((long)(dCompleteness*nBrushAlpha1));
				m_pTransformer->put_brushAlpha2((long)(dCompleteness*nBrushAlpha2));
				m_pTransformer->put_brushTextureAlpha((long)(dCompleteness*nTextureAlpha));
				m_pTransformer->put_shadowAlpha((long)(dCompleteness*nShadowAlpha));
				m_pTransformer->put_edgeAlpha((long)(dCompleteness*nEdgeAlpha));

				// в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dLeft   = pAction->Left   * dMetricX;
				double dRight  = pAction->Right  * dMetricX;
				double dTop    = pAction->Top    * dMetricY;
				double dBottom = pAction->Bottom * dMetricY;
				
				double dNewFontSize = dFontSize * dMetricX;

				dLeft   += dLeft   > 0 ? 0.5 : -0.5;
				dTop    += dTop    > 0 ? 0.5 : -0.5;
				dRight  += dRight  > 0 ? 0.5 : -0.5;
				dBottom += dBottom > 0 ? 0.5 : -0.5;

				m_pTransformer->put_fontSize( dNewFontSize );
				m_pTransformer->DrawMedia( &pResult, bstrText, long(dLeft), long(dTop), long(dRight), long(dBottom) );
				m_pTransformer->put_fontSize( dFontSize );

				// TODO: удаляем исходный интерфейс
				pSource->Release();

				// возвращаем старое значение alpha
				m_pTransformer->put_brushAlpha1(nBrushAlpha1);
				m_pTransformer->put_brushAlpha2(nBrushAlpha2);
				m_pTransformer->put_brushTextureAlpha(nTextureAlpha);
				m_pTransformer->put_shadowAlpha(nShadowAlpha);
				m_pTransformer->put_edgeAlpha(nEdgeAlpha);
	
				SysFreeString(bstrText);

				return TRUE;
			}
			BOOL Transform_DrawOnEllipse(ImageStudio::Serialize::Paint::TextStatic::DrawOnEllipse* pAction, double dTime, IUnknown* pSource, IUnknown*& pResult)
			{
				if (!pAction || !pSource)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(dTime);
				if (dCompleteness <= 0.0001)
					return (Core::c_nErrorNone == ImageStudio::Transforms::Core::COM::COM_DulicateImage(pSource, pResult, TRUE));
				
				if (!Create())
					return FALSE;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pSource, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				pResult = pSource;

				BSTR bstrText = pAction->Text.AllocSysString();

				// font
				if (TRUE)
				{
					if (pAction->Font.IsValidName())
					{
						BSTR bstrName = pAction->Font.GetName().AllocSysString();
						m_pTransformer->put_fontName(bstrName);
						SysFreeString(bstrName);
					}

					if (pAction->Font.IsValidSize())						m_pTransformer->put_fontSize(pAction->Font.GetSize());
					if (pAction->Font.IsValidBold())						m_pTransformer->put_fontBold(pAction->Font.GetBold() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidItalic())						m_pTransformer->put_fontItalic(pAction->Font.GetItalic() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidUnderline())					m_pTransformer->put_fontUnderline(pAction->Font.GetUnderline() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidStrikeout())					m_pTransformer->put_fontStrikeout(pAction->Font.GetStrikeout() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidStringAlignmentVertical())		m_pTransformer->put_formatAlignVertical(pAction->Font.GetStringAlignmentVertical());
					if (pAction->Font.IsValidStringAlignmentHorizontal())	m_pTransformer->put_formatAlignHorizontal(pAction->Font.GetStringAlignmentHorizontal());
					if (pAction->Font.IsValidAngle())						m_pTransformer->put_formatRotate((long)pAction->Font.GetAngle());
				}

				// brush
				if (TRUE)
				{
					if (pAction->Brush.IsValidTexturePath())
					{
						BSTR bstrPath = pAction->Brush.GetTexturePath().AllocSysString();
						m_pTransformer->put_brushTexturePath(bstrPath);
						SysFreeString(bstrPath);
					}

					if (pAction->Brush.IsValidTextureAlpha())	m_pTransformer->put_brushTextureAlpha(pAction->Brush.GetTextureAlpha());
					if (pAction->Brush.IsValidTextureMode())	m_pTransformer->put_brushTextureMode(pAction->Brush.GetTextureMode());
					if (pAction->Brush.IsValidColor1())			m_pTransformer->put_brushColor1(pAction->Brush.GetColor1());
					if (pAction->Brush.IsValidColor2())			m_pTransformer->put_brushColor2(pAction->Brush.GetColor2());
					if (pAction->Brush.IsValidAlpha1())			m_pTransformer->put_brushAlpha1(pAction->Brush.GetAlpha1());
					if (pAction->Brush.IsValidAlpha2())			m_pTransformer->put_brushAlpha2(pAction->Brush.GetAlpha2());
					if (pAction->Brush.IsValidType())			m_pTransformer->put_brushType(pAction->Brush.GetType());
				}

				// shadow
				if (TRUE)
				{
					if (pAction->Shadow.IsValidVisible())		m_pTransformer->put_shadowVisible(pAction->Shadow.GetVisible() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Shadow.IsValidDistanceX())		m_pTransformer->put_shadowDistanceX(long(pAction->Shadow.GetDistanceX()));
					if (pAction->Shadow.IsValidDistanceY())		m_pTransformer->put_shadowDistanceY(long(pAction->Shadow.GetDistanceY()));
					if (pAction->Shadow.IsValidBlurSize())		m_pTransformer->put_shadowBlur(long(pAction->Shadow.GetBlurSize()));
					if (pAction->Shadow.IsValidColor())			m_pTransformer->put_shadowColor(pAction->Shadow.GetColor());
					if (pAction->Shadow.IsValidAlpha())			m_pTransformer->put_shadowAlpha(pAction->Shadow.GetAlpha());
				}

				// edge
				if (TRUE)
				{
					if (pAction->Edge.IsValidVisible())			m_pTransformer->put_edgeVisible(pAction->Edge.GetVisible() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Edge.IsValidDist())			m_pTransformer->put_edgeDistance(long(pAction->Edge.GetDist()));
					if (pAction->Edge.IsValidColor())			m_pTransformer->put_edgeColor(pAction->Edge.GetColor());
					if (pAction->Edge.IsValidAlpha())			m_pTransformer->put_edgeAlpha(pAction->Edge.GetAlpha());
				}

				// отрисовываем полупрозначный объект, если completeness не 1
				long nBrushAlpha1  = 0; m_pTransformer->get_brushAlpha1(&nBrushAlpha1);
				long nBrushAlpha2  = 0; m_pTransformer->get_brushAlpha2(&nBrushAlpha2);
				long nTextureAlpha = 0; m_pTransformer->get_brushTextureAlpha(&nTextureAlpha);
				long nShadowAlpha  = 0; m_pTransformer->get_shadowAlpha(&nShadowAlpha);
				long nEdgeAlpha    = 0; m_pTransformer->get_edgeAlpha(&nEdgeAlpha);
				double dFontSize   = 0; m_pTransformer->get_fontSize(&dFontSize);
				
				m_pTransformer->put_brushAlpha1((long)(dCompleteness*nBrushAlpha1));
				m_pTransformer->put_brushAlpha2((long)(dCompleteness*nBrushAlpha2));
				m_pTransformer->put_brushTextureAlpha((long)(dCompleteness*nTextureAlpha));
				m_pTransformer->put_shadowAlpha((long)(dCompleteness*nShadowAlpha));
				m_pTransformer->put_edgeAlpha((long)(dCompleteness*nEdgeAlpha));

				// в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dLeft        = pAction->Left   * dMetricX;
				double dRight       = pAction->Right  * dMetricX;
				double dTop         = pAction->Top    * dMetricY;
				double dBottom      = pAction->Bottom * dMetricY;
				double dNewFontSize = dFontSize       * dMetricX;
				double dStartAngle  = pAction->StartAngle;
				double dSweepAngle  = pAction->SweepAngle;

				dLeft   += dLeft   > 0 ? 0.5 : -0.5;
				dTop    += dTop    > 0 ? 0.5 : -0.5;
				dRight  += dRight  > 0 ? 0.5 : -0.5;
				dBottom += dBottom > 0 ? 0.5 : -0.5;

				m_pTransformer->put_fontSize( dNewFontSize );
				m_pTransformer->DrawMediaEllipse( &pResult, bstrText, long(dLeft), long(dTop), long(dRight), long(dBottom), dStartAngle, dSweepAngle );
				m_pTransformer->put_fontSize( dFontSize );

				// возвращаем старое значение alpha
				m_pTransformer->put_brushAlpha1(nBrushAlpha1);
				m_pTransformer->put_brushAlpha2(nBrushAlpha2);
				m_pTransformer->put_brushTextureAlpha(nTextureAlpha);
				m_pTransformer->put_shadowAlpha(nShadowAlpha);
				m_pTransformer->put_edgeAlpha(nEdgeAlpha);
	
				SysFreeString(bstrText);

				return TRUE;
			}
			BOOL Transform_DrawOnPolyline(ImageStudio::Serialize::Paint::TextStatic::DrawOnPolyline* pAction, double dTime, IUnknown* pSource, IUnknown*& pResult)
			{
				if (!pAction || !pSource)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(dTime);
				int nNumPoints = pAction->Points.GetSize();
				if (dCompleteness <= 0.0001 || nNumPoints < 2)
					return (Core::c_nErrorNone == ImageStudio::Transforms::Core::COM::COM_DulicateImage(pSource, pResult, TRUE));
				
				if (!Create())
					return FALSE;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pSource, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				pResult = pSource;

				BSTR bstrText = pAction->Text.AllocSysString();
				nNumPoints &= ~1;

				SAFEARRAYBOUND oSafeArrayBounds;
				oSafeArrayBounds.lLbound = 0;
				oSafeArrayBounds.cElements = nNumPoints;

				SAFEARRAY* pSafeArray = SafeArrayCreate(VT_R8, 1, &oSafeArrayBounds);

				double* pDstPoints = (double*)(pSafeArray->pvData);
				double* pSrcPoints = (double*)(pAction->Points.GetData());

				for (int nPoint = 0; nPoint < nNumPoints; nPoint += 2)
				{
					pDstPoints[nPoint]     = dMetricX * pSrcPoints[nPoint];
					pDstPoints[nPoint + 1] = dMetricY * pSrcPoints[nPoint + 1];
				}

				// font
				if (TRUE)
				{
					if (pAction->Font.IsValidName())
					{
						BSTR bstrName = pAction->Font.GetName().AllocSysString();
						m_pTransformer->put_fontName(bstrName);
						SysFreeString(bstrName);
					}

					if (pAction->Font.IsValidSize())						m_pTransformer->put_fontSize(pAction->Font.GetSize());
					if (pAction->Font.IsValidBold())						m_pTransformer->put_fontBold(pAction->Font.GetBold() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidItalic())						m_pTransformer->put_fontItalic(pAction->Font.GetItalic() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidUnderline())					m_pTransformer->put_fontUnderline(pAction->Font.GetUnderline() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidStrikeout())					m_pTransformer->put_fontStrikeout(pAction->Font.GetStrikeout() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidStringAlignmentVertical())		m_pTransformer->put_formatAlignVertical(pAction->Font.GetStringAlignmentVertical());
					if (pAction->Font.IsValidStringAlignmentHorizontal())	m_pTransformer->put_formatAlignHorizontal(pAction->Font.GetStringAlignmentHorizontal());
					if (pAction->Font.IsValidAngle())						m_pTransformer->put_formatRotate((long)pAction->Font.GetAngle());
				}

				// brush
				if (TRUE)
				{
					if (pAction->Brush.IsValidTexturePath())
					{
						BSTR bstrPath = pAction->Brush.GetTexturePath().AllocSysString();
						m_pTransformer->put_brushTexturePath(bstrPath);
						SysFreeString(bstrPath);
					}

					if (pAction->Brush.IsValidTextureAlpha())	m_pTransformer->put_brushTextureAlpha(pAction->Brush.GetTextureAlpha());
					if (pAction->Brush.IsValidTextureMode())	m_pTransformer->put_brushTextureMode(pAction->Brush.GetTextureMode());
					if (pAction->Brush.IsValidColor1())			m_pTransformer->put_brushColor1(pAction->Brush.GetColor1());
					if (pAction->Brush.IsValidColor2())			m_pTransformer->put_brushColor2(pAction->Brush.GetColor2());
					if (pAction->Brush.IsValidAlpha1())			m_pTransformer->put_brushAlpha1(pAction->Brush.GetAlpha1());
					if (pAction->Brush.IsValidAlpha2())			m_pTransformer->put_brushAlpha2(pAction->Brush.GetAlpha2());
					if (pAction->Brush.IsValidType())			m_pTransformer->put_brushType(pAction->Brush.GetType());
				}

				// shadow
				if (TRUE)
				{
					if (pAction->Shadow.IsValidVisible())		m_pTransformer->put_shadowVisible(pAction->Shadow.GetVisible() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Shadow.IsValidDistanceX())		m_pTransformer->put_shadowDistanceX(long(pAction->Shadow.GetDistanceX()));
					if (pAction->Shadow.IsValidDistanceY())		m_pTransformer->put_shadowDistanceY(long(pAction->Shadow.GetDistanceY()));
					if (pAction->Shadow.IsValidBlurSize())		m_pTransformer->put_shadowBlur(long(pAction->Shadow.GetBlurSize()));
					if (pAction->Shadow.IsValidColor())			m_pTransformer->put_shadowColor(pAction->Shadow.GetColor());
					if (pAction->Shadow.IsValidAlpha())			m_pTransformer->put_shadowAlpha(pAction->Shadow.GetAlpha());
				}

				// edge
				if (TRUE)
				{
					if (pAction->Edge.IsValidVisible())			m_pTransformer->put_edgeVisible(pAction->Edge.GetVisible() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Edge.IsValidDist())			m_pTransformer->put_edgeDistance(long(pAction->Edge.GetDist()));
					if (pAction->Edge.IsValidColor())			m_pTransformer->put_edgeColor(pAction->Edge.GetColor());
					if (pAction->Edge.IsValidAlpha())			m_pTransformer->put_edgeAlpha(pAction->Edge.GetAlpha());
				}

				// отрисовываем полупрозначный объект, если completeness не 1
				long nBrushAlpha1 = 0; m_pTransformer->get_brushAlpha1(&nBrushAlpha1);
				long nBrushAlpha2 = 0; m_pTransformer->get_brushAlpha2(&nBrushAlpha2);
				long nTextureAlpha = 0; m_pTransformer->get_brushTextureAlpha(&nTextureAlpha);
				long nShadowAlpha = 0; m_pTransformer->get_shadowAlpha(&nShadowAlpha);
				long nEdgeAlpha = 0; m_pTransformer->get_edgeAlpha(&nEdgeAlpha);
				double dFontSize = 0; m_pTransformer->get_fontSize(&dFontSize);
				
				m_pTransformer->put_brushAlpha1((long)(dCompleteness*nBrushAlpha1));
				m_pTransformer->put_brushAlpha2((long)(dCompleteness*nBrushAlpha2));
				m_pTransformer->put_brushTextureAlpha((long)(dCompleteness*nTextureAlpha));
				m_pTransformer->put_shadowAlpha((long)(dCompleteness*nShadowAlpha));
				m_pTransformer->put_edgeAlpha((long)(dCompleteness*nEdgeAlpha));

				// отрисовываем текст
				m_pTransformer->DrawMediaPolyline(&pResult, &pSafeArray, bstrText);

				// возвращаем старое значение alpha
				m_pTransformer->put_brushAlpha1(nBrushAlpha1);
				m_pTransformer->put_brushAlpha2(nBrushAlpha2);
				m_pTransformer->put_brushTextureAlpha(nTextureAlpha);
				m_pTransformer->put_shadowAlpha(nShadowAlpha);
				m_pTransformer->put_edgeAlpha(nEdgeAlpha);

				SafeArrayDestroy(pSafeArray);

				SysFreeString(bstrText);

				return TRUE;
			}
		
		protected:

			void Destroy()
			{
				if (NULL != m_pTransformer)
					m_pTransformer->Release();

				m_pTransformer = NULL;
			}
			BOOL Create()
			{
				if (NULL != m_pTransformer)
					return TRUE;
				if (!m_bTransformerValid)
					return FALSE;

				// пытаемся создать интерфейс трансформера
				if (S_OK != CoCreateInstance(ImagePaint::CLSID_ImageTextStatic3, NULL, CLSCTX_ALL, ImagePaint::IID_IImageTextStatic3, (void**)&m_pTransformer))
				{
					m_pTransformer = NULL;
					m_bTransformerValid = FALSE;
				}

				return IsValid();
			}
			
		protected:
						
			ImagePaint::IImageTextStatic3* m_pTransformer; // интерфейс трансформера, который будет производить операции
			BOOL m_bTransformerValid; // флаг, обозначающий - производилась ли попытка создать интерфейс трансформера или нет
		};
		class SerializedObjectTransformerPaintTextDynamic
		{
		public:

			SerializedObjectTransformerPaintTextDynamic()
			{
				m_pTransformer = NULL;
				m_bTransformerValid = TRUE;
			}
			virtual ~SerializedObjectTransformerPaintTextDynamic()
			{
				Destroy();
			}
			
			BOOL IsValid()
			{
				if (NULL == m_pTransformer)
					return FALSE;

				return TRUE;
			}
			
			BOOL Transform_SetBrush(ImageStudio::Serialize::Paint::TextDynamic::SetBrush* pAction, double dTime)
			{
				if (!pAction)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				BSTR bstrPath = pAction->Brush.TexturePath.AllocSysString();
				m_pTransformer->put_brushTextureLoad(bstrPath); SysFreeString(bstrPath);
				m_pTransformer->put_brushTextureAlpha((BYTE)pAction->Brush.TextureAlpha);
				m_pTransformer->put_brushTextureMode(pAction->Brush.TextureMode);
				m_pTransformer->put_brushColor1(pAction->Brush.Color1);
				m_pTransformer->put_brushColor2(pAction->Brush.Color2);
				m_pTransformer->put_brushAlpha1((BYTE)pAction->Brush.Alpha1);
				m_pTransformer->put_brushAlpha2((BYTE)pAction->Brush.Alpha2);
				m_pTransformer->put_brushType(pAction->Brush.Type);

				return TRUE;
			}
			BOOL Transform_SetFont(ImageStudio::Serialize::Paint::TextDynamic::SetFont* pAction, double dTime)
			{
				if (!pAction)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				BSTR bstrName = pAction->Font.Name.AllocSysString();
				m_pTransformer->put_fontName(bstrName); SysFreeString(bstrName);
				m_pTransformer->put_fontSize((long)pAction->Font.Size);
				m_pTransformer->put_fontBold(pAction->Font.Bold ? VARIANT_TRUE : VARIANT_FALSE);
				m_pTransformer->put_fontItalic(pAction->Font.Italic ? VARIANT_TRUE : VARIANT_FALSE);
				m_pTransformer->put_fontUnderline(pAction->Font.Underline ? VARIANT_TRUE : VARIANT_FALSE);
				m_pTransformer->put_fontStrikeout(pAction->Font.Strikeout ? VARIANT_TRUE : VARIANT_FALSE);

				return TRUE;
			}
			BOOL Transform_SetShadow(ImageStudio::Serialize::Paint::TextDynamic::SetShadow* pAction, double dTime)
			{
				if (!pAction)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				m_pTransformer->put_shadowVisible(pAction->Shadow.Visible ? VARIANT_TRUE : VARIANT_FALSE);
				m_pTransformer->put_shadowDistanceX(int(pAction->Shadow.DistanceX));
				m_pTransformer->put_shadowDistanceY(int(pAction->Shadow.DistanceY));
				m_pTransformer->put_shadowColor(pAction->Shadow.Color);
				m_pTransformer->put_shadowAlpha((BYTE)pAction->Shadow.Alpha);
				m_pTransformer->put_shadowBlur(int(pAction->Shadow.BlurSize));

				return TRUE;
			}
			BOOL Transform_SetEdge(ImageStudio::Serialize::Paint::TextDynamic::SetEdge* pAction, double dTime)
			{
				if (!pAction)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				m_pTransformer->put_edgeVisible(pAction->Edge.Visible ? VARIANT_TRUE : VARIANT_FALSE);
				m_pTransformer->put_edgeDistance(int(pAction->Edge.Dist));
				m_pTransformer->put_edgeColor(pAction->Edge.Color);
				m_pTransformer->put_edgeAlpha((BYTE)pAction->Edge.Alpha);

				return TRUE;
			}
			BOOL Transform_Start(ImageStudio::Serialize::Paint::TextDynamic::Start* pAction, double dTime, IUnknown* pSource)
			{
				if (!pAction || !pSource)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pSource, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				BSTR bstrText = pAction->Text.AllocSysString();

				m_pTransformer->put_formatRotate(pAction->Angle);
				
				// в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dDiagonal = sqrt( dMetricX * dMetricX + dMetricY * dMetricY );
				double dX        = pAction->X    * dMetricX;
				double dY        = pAction->Y    * dMetricY;
				double dStep     = pAction->Step * dDiagonal;

				dX += dX > 0 ? 0.5 : -0.5;
				dY += dY > 0 ? 0.5 : -0.5;

				int nCategory = pAction->Category;
				int nEffect   = pAction->Effect;
				int nEffectID = pAction->EffectID;

				m_pTransformer->Start( bstrText, nCategory, nEffect, nEffectID, long(dX), long(dY), long(dStep) );

				SysFreeString(bstrText);

				return TRUE;
			}
			BOOL Transform_Stop(ImageStudio::Serialize::Paint::TextDynamic::Stop* pAction, double dTime)
			{
				if (!pAction)
					return FALSE;
				if (pAction->GetCompleteness(dTime) <= 0.0001)
					return TRUE;
				if (!Create())
					return FALSE;

				m_pTransformer->Stop();

				return TRUE;
			}
			BOOL Transform_Render(ImageStudio::Serialize::Paint::TextDynamic::Render* pAction, double dTime, IUnknown* pSource, IUnknown*& pResult)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(dTime);
				if (dCompleteness <= 0.0001)
					return (Core::c_nErrorNone == ImageStudio::Transforms::Core::COM::COM_DulicateImage(pSource, pResult, TRUE));

				if (!Create())
					return FALSE;

				// font
				if (TRUE)
				{
					if (pAction->Font.IsValidName())
					{
						BSTR bstrName = pAction->Font.GetName().AllocSysString();
						m_pTransformer->put_fontName(bstrName);
						SysFreeString(bstrName);
					}

					if (pAction->Font.IsValidSize())				m_pTransformer->put_fontSize((int)pAction->Font.GetSize());
					if (pAction->Font.IsValidBold())				m_pTransformer->put_fontBold(pAction->Font.GetBold() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidItalic())				m_pTransformer->put_fontItalic(pAction->Font.GetItalic() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidUnderline())			m_pTransformer->put_fontUnderline(pAction->Font.GetUnderline() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Font.IsValidStrikeout())			m_pTransformer->put_fontStrikeout(pAction->Font.GetStrikeout() ? VARIANT_TRUE : VARIANT_FALSE);
				}

				// brush
				if (TRUE)
				{
					if (pAction->Brush.IsValidTexturePath())
					{
						BSTR bstrPath = pAction->Brush.GetTexturePath().AllocSysString();
						m_pTransformer->put_brushTextureLoad(bstrPath);
						SysFreeString(bstrPath);
					}

					if (pAction->Brush.IsValidTextureAlpha())	m_pTransformer->put_brushTextureAlpha(pAction->Brush.GetTextureAlpha());
					if (pAction->Brush.IsValidTextureMode())	m_pTransformer->put_brushTextureMode(pAction->Brush.GetTextureMode());
					if (pAction->Brush.IsValidColor1())			m_pTransformer->put_brushColor1(pAction->Brush.GetColor1());
					if (pAction->Brush.IsValidColor2())			m_pTransformer->put_brushColor2(pAction->Brush.GetColor2());
					if (pAction->Brush.IsValidAlpha1())			m_pTransformer->put_brushAlpha1(pAction->Brush.GetAlpha1());
					if (pAction->Brush.IsValidAlpha2())			m_pTransformer->put_brushAlpha2(pAction->Brush.GetAlpha2());
					if (pAction->Brush.IsValidType())			m_pTransformer->put_brushType(pAction->Brush.GetType());
				}

				// shadow
				if (TRUE)
				{
					if (pAction->Shadow.IsValidVisible())		m_pTransformer->put_shadowVisible(pAction->Shadow.GetVisible() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Shadow.IsValidDistanceX())		m_pTransformer->put_shadowDistanceX(long(pAction->Shadow.GetDistanceX()));
					if (pAction->Shadow.IsValidDistanceY())		m_pTransformer->put_shadowDistanceY(long(pAction->Shadow.GetDistanceY()));
					if (pAction->Shadow.IsValidBlurSize())		m_pTransformer->put_shadowBlur(long(pAction->Shadow.GetBlurSize()));
					if (pAction->Shadow.IsValidColor())			m_pTransformer->put_shadowColor(pAction->Shadow.GetColor());
					if (pAction->Shadow.IsValidAlpha())			m_pTransformer->put_shadowAlpha(pAction->Shadow.GetAlpha());
				}

				// edge
				if (TRUE)
				{
					if (pAction->Edge.IsValidVisible())			m_pTransformer->put_edgeVisible(pAction->Edge.GetVisible() ? VARIANT_TRUE : VARIANT_FALSE);
					if (pAction->Edge.IsValidDist())			m_pTransformer->put_edgeDistance(long(pAction->Edge.GetDist()));
					if (pAction->Edge.IsValidColor())			m_pTransformer->put_edgeColor(pAction->Edge.GetColor());
					if (pAction->Edge.IsValidAlpha())			m_pTransformer->put_edgeAlpha(pAction->Edge.GetAlpha());
				}

				// отрисовываем текст
				m_pTransformer->RenderMedia(pSource, &pResult, dCompleteness);

				return TRUE;
			}
			
		protected:

			void Destroy()
			{
				if (NULL != m_pTransformer)
					m_pTransformer->Release();

				m_pTransformer = NULL;
			}
			BOOL Create()
			{
				if (NULL != m_pTransformer)
					return TRUE;
				if (!m_bTransformerValid)
					return FALSE;

				// пытаемся создать интерфейс трансформера
				if (S_OK != CoCreateInstance(ImagePaint::CLSID_ImageTextDynamic3, NULL, CLSCTX_ALL, ImagePaint::IID_IImageTextDynamic3, (void**)&m_pTransformer))
				{
					m_pTransformer = NULL;
					m_bTransformerValid = FALSE;
				}

				return IsValid();
			}
			
		protected:
						
			ImagePaint::IImageTextDynamic3* m_pTransformer; // интерфейс трансформера, который будет производить операции
			BOOL m_bTransformerValid; // флаг, обозначающий - производилась ли попытка создать интерфейс трансформера или нет
		};
				
		class SerializedObjectTransformerEvents
		{
		public:
			
			virtual BOOL OnProgress(double dPercents)
			{
				return TRUE;
			}
		};
		class SerializedObjectTransformer
		{
		protected:
			// схема работы трансформера следующая: после применения подтрансформа исходный интерфейс удаляется
			// 
			// трансформы:                |     T    T    T    T      |
			// счетчики интерфейсов:    1 |   1 -> 2 -> 3 -> 4 -> 5   | 1
			// корректировка счетчиков:   |       -1   -1   -1   -1   |
			// 
			// каждый подтрансформ преобразует интерфейс IN(ref=1) в интерфейс OUT(ref=1) либо IN(ref=1) в IN (ref=2),
			// после IN(1) удаляется - чтобы не копились ссылки
			//

			CSourceArray m_oSources; // буффер картинок, для применения эффектов над несколькими картинками (compose и т.п.) контроллируется пользователем

			ImageCache m_oImageCache; // кэш с текстурами для брашей и т.д.
			
			SerializedObjectTransformerPaintTextStatic  m_oImagePaintTextStatic;
			SerializedObjectTransformerPaintTextDynamic m_oImagePaintTextDynamic;
			
			// кэш объектов (необходим для быстрого выполнения одних и тех же эффектов)
			ImageStudio::Caching::СCacheManager m_oCache;

			// статус отрисовщика
			ImageStudio::Paint::CPaintStateManager m_oPaintSettings;

			// смешивальшик изображений
			ImageStudio::Paint::Compose::CImageComposer m_oImageComposer;
				
			// приемник эвентов (прогресс, запрос на выполнение эффектов и пр.)
			SerializedObjectTransformerEvents* m_pEventReceiver;
			
			// временная метка, по которой будут выполнятся комманды
			double m_dTime;
			
			// указатель на ВНЕШНИЙ интерфейс ImageCache
			IMemoryStorage* m_pMemoryStorage;

			// указатель на ВНУТРЕННИЙ интерфейс ImageRaw для чтения RAW файлов
			ImageRaw::IImageRaw3* m_pRawFile;

			FileCache::IAVSImageFilesCache* m_pFileCache;

			ImageCLSID m_oImageCLSID;

			dx9::CRender m_oRender;
			BOOL m_bUseVideoCard;
			int  m_nLastSourceInVideoCard;

			static const int c_nMinImageSizeForDirectX = 0;//128*128;

		protected:
			void ClearCache(ImageStudio::Serialize::SerializedObjectList& oTransforms)
			{
				ATL::CAtlArray<int> arrIDsToKeep;

				int nTransformCount = oTransforms.GetCount();
				for (int nTransform = 0; nTransform < nTransformCount; ++nTransform)
				{
					ImageStudio::Serialize::SerializedObject* pTransform = oTransforms[nTransform];
					if (NULL == pTransform)
						continue;

					arrIDsToKeep.Add(pTransform->GetSessionID());
				}

				m_oCache.Clear(arrIDsToKeep);
			}
			
			#define SerializedObjectTransformer_ProcessSerializedObject(Class, Transform, Function, Success) \
				{ \
					if (NULL != Transform) \
					{ \
						ImageStudio::Serialize::##Class* pTransformClass = (ImageStudio::Serialize::##Class*)Transform; \
						Success = ##Function(pTransformClass); \
					} \
				}
			
			BOOL Process_Group(ImageStudio::Serialize::SerializedGroup* pAction)
			{
				// ничего не делаем с группой... просто ее игнорируем, т.к., по идее, ее здесь быть не должно
				
				return TRUE;
			}
			BOOL Process_BufferClear(ImageStudio::Serialize::SerializedBufferClear* pAction)
			{
				if (!pAction)
					return FALSE;

				return m_oSources.Clear( pAction->Index );
			}
			BOOL Process_BufferCopy(ImageStudio::Serialize::SerializedBufferCopy* pAction)
			{
				if (!pAction)
					return FALSE;

				return m_oSources.Copy( pAction->IndexFrom, pAction->IndexTo );
			}
			BOOL Process_SetAdditionalParams(ImageStudio::Serialize::Core::AdditionalParams* pAction)
			{
				if (!pAction)
					return FALSE;
				
				return TRUE;
			}
			BOOL Process_SetParamsUseVideoCard(ImageStudio::Serialize::SerializedSetParamsUseVideoCard* pAction)
			{
				if (!pAction)
					return FALSE;

				m_oRender.SetDefaultFilter( pAction->nInterpolation );
				
				switch(pAction->nUseType)
				{
				case 0: m_bUseVideoCard = 0; return /*SaveResultFromVideoCard() ?*/ 1 ;//: -1;
				case 1: m_bUseVideoCard = 1; return 1;
				case 2: m_bUseVideoCard = 2; return 1;
				default: return 0;
				};
			}
			
			BOOL Process_ImageClear(ImageStudio::Serialize::Effects::ImageClear* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				// если нечего удалять - ничего не делаем
				return m_oSources.Clear( pAction->Index );
			}
			BOOL Process_ImageCopy(ImageStudio::Serialize::Effects::ImageCopy* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->IndexFrom ) || 
					m_oSources.IsBadIndex( pAction->IndexTo ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				// если нечего копировать - ничего не делаем
				if( m_oSources.IsEmptySource( pAction->IndexFrom ) )
					return TRUE;

				// проверяем, если буффер назначения не содержит картинки, то нам надо просто сделать дубликат картинки и все...
				if( m_oSources.IsEmptySource( pAction->IndexTo ) )
				{
					return m_oSources.Set( pAction->IndexTo, m_oSources.CloneToBgra( pAction->IndexFrom, TRUE ) );
				}
				else
				{
					// если же буффер назначения не нулевой, то надо отрисовать одну картинку поверх другой с учетом маски 
					// (маска в данном случае определяет alpha смешение цветов пикселов двух картинок)

					int nSourceWidth = 0;
					int nSourceHeight = 0;
					int nSourceStride = 0;
					BYTE* pSourcePixels = NULL;
					if (ImageStudio::Transforms::Core::c_nErrorNone != Core::COM::COM_GetBGRAImageFromInterface( m_oSources.Get( pAction->IndexFrom ), pSourcePixels, nSourceWidth, nSourceHeight, &nSourceStride))
						return FALSE;

					int nResultWidth = 0;
					int nResultHeight = 0;
					int nResultStride = 0;
					BYTE* pResultPixels = NULL;
					if (ImageStudio::Transforms::Core::c_nErrorNone != Core::COM::COM_GetBGRAImageFromInterface( m_oSources.Get( pAction->IndexTo ), pResultPixels, nResultWidth, nResultHeight, &nResultStride))
						return FALSE;

					if( nSourceStride < 0 )
						pSourcePixels -= (nSourceHeight - 1) * nSourceStride;

					if( nResultStride < 0 )
						pResultPixels -= (nResultHeight - 1) * nResultStride;

					// обновляем маску перед тем, как будем смешивать цвета..
					// pAction сам кэширует маску, так что лишних операций тут не произойдет
					pAction->UpdateMask( nSourceWidth, nSourceHeight );

					for( ;; )
					{

						if( ImageStudio::Serialize::c_nMaskAllPixels == pAction->MaskType )
						{
							break;
						}

						// производим попиксельное смешение цветов двух картинок
						// при этом если картинки разных размеров, то отрисуем source прямо поверх result из точки (0, 0)

						int width  = min(nSourceWidth,  nResultWidth);
						int height = min(nSourceHeight, nResultHeight);

						BYTE* pSrcPixels = pSourcePixels;
						BYTE* pDstPixels = pResultPixels;

						if( ImageStudio::Serialize::c_nMaskAllPixels == pAction->MaskType )
						{
							for( int row = height; row > 0; --row, pSrcPixels += nSourceStride, pDstPixels += nResultStride )
							{
								memcpy( pDstPixels, pSrcPixels, width*4 );
							}

							break;
						}

						BYTE* pMaskPixels = pAction->m_oMask.GetPixels();
						
						if( pAction->m_oMask.GetWidth()  != nSourceWidth || 
							pAction->m_oMask.GetHeight() != nSourceHeight || 
							NULL == pMaskPixels )
						{
							break;
						}

						int nSrcStep = nSourceStride - nSourceWidth * 4;
						int nDstStep = nResultStride - nResultWidth * 4;
						int nMaskStep = nSourceWidth - width;
						
						for( int row = height; row > 0; --row, pSrcPixels += nSrcStep, pDstPixels += nDstStep, pMaskPixels += nMaskStep )
						{
							for( int pos = width; pos > 0; --pos, pSrcPixels += 4, pDstPixels += 4, pMaskPixels += 1 )
							{
								// вариант 1
								//double dAlpha1 = pMaskPixels[0] * (1.0 / 255.0);
								//double dAlpha2 = 1.0 - dAlpha1;

								//pDstPixels[0] = (BYTE)(dAlpha1*pSrcPixels[0] + dAlpha2*pDstPixels[0]);
								//pDstPixels[1] = (BYTE)(dAlpha1*pSrcPixels[1] + dAlpha2*pDstPixels[1]);
								//pDstPixels[2] = (BYTE)(dAlpha1*pSrcPixels[2] + dAlpha2*pDstPixels[2]);
								//pDstPixels[3] = (BYTE)(dAlpha1*pSrcPixels[3] + dAlpha2*pDstPixels[3]);

								// вариант 2 (работает в 7.5 раз быстрее чем вариант 1)
								int alpha = pMaskPixels[0];
								pDstPixels[0] = AlphaComp( pDstPixels[0], pSrcPixels[0], alpha );
								pDstPixels[1] = AlphaComp( pDstPixels[1], pSrcPixels[1], alpha );
								pDstPixels[2] = AlphaComp( pDstPixels[2], pSrcPixels[2], alpha );
								pDstPixels[3] = AlphaComp( pDstPixels[3], pSrcPixels[3], alpha );
							}
						}

						break;
					}
				}

				return TRUE;
			}
			
			BOOL Process_Adjust_Brightness(ImageStudio::Serialize::Effects::AdjustBrightness* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustBrightness( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_BrightnessEx(ImageStudio::Serialize::Effects::AdjustBrightnessEx* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustBrightnessEx( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->LevelRed, pAction->LevelGreen, pAction->LevelBlue );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Contrast(ImageStudio::Serialize::Effects::AdjustContrast* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustContrast( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Gamma(ImageStudio::Serialize::Effects::AdjustGamma* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustGamma( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Equalize(ImageStudio::Serialize::Effects::AdjustEqualize* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustEqualize( m_oSources.Get( pAction->SourceId ), dCompleteness );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Grayscale(ImageStudio::Serialize::Effects::AdjustGrayscale* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				double dFrame = dCompleteness * pAction->Intensity / 100;

				int nResult = Core::COM::COM_AdjustGrayscale( m_oSources.Get( pAction->SourceId ), dFrame, pAction->Desaturate );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Invert(ImageStudio::Serialize::Effects::AdjustInvert* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				double dFrame = dCompleteness * pAction->Intensity / 100;

				int nResult = Core::COM::COM_AdjustInvert( m_oSources.Get( pAction->SourceId ), dFrame, pAction->bOverGray );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Autobrightness(ImageStudio::Serialize::Effects::AdjustAutobrightness* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustAutobrightness( m_oSources.Get( pAction->SourceId ), dCompleteness );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Autocontrast(ImageStudio::Serialize::Effects::AdjustAutocontrast* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustAutocontrast( m_oSources.Get( pAction->SourceId ), dCompleteness );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Autolevels(ImageStudio::Serialize::Effects::AdjustAutolevels* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustAutolevels( m_oSources.Get( pAction->SourceId ), dCompleteness );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Hue(ImageStudio::Serialize::Effects::AdjustHue* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				//dx9::EffectHLS hls;
				//hls.m_dShiftHueAngle = pAction->Angle;

				//int nRetCode = UpdateVideoEffect( &hls, pAction->SourceId, dCompleteness );
				//if( nRetCode != 0 )
				//	return nRetCode;

				int nResult = Core::COM::COM_AdjustHue( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Angle );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Saturation(ImageStudio::Serialize::Effects::AdjustSaturation* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				//dx9::EffectHLS hls;
				//hls.m_dShiftSaturation = pAction->Level;
				//hls.m_dFactorSaturation = pAction->Factor;

				//int nRetCode = UpdateVideoEffect( &hls, pAction->SourceId, dCompleteness );
				//if( nRetCode != 0 )
				//	return nRetCode;

				int nResult = Core::COM::COM_AdjustSaturation( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Posterize(ImageStudio::Serialize::Effects::AdjustPosterize* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustPosterize( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Levels );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Temperature(ImageStudio::Serialize::Effects::AdjustTemperature* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustTemperature( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Colorize(ImageStudio::Serialize::Effects::AdjustColorize* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;
				
				int nResult = Core::COM::COM_AdjustColorize( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type, pAction->LevelRed, pAction->LevelGreen, pAction->LevelBlue );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Threshold(ImageStudio::Serialize::Effects::AdjustThreshold* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustThreshold( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level, pAction->Grayscale );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Sepia(ImageStudio::Serialize::Effects::AdjustSepia* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nLevel      = max(0, min(pAction->Level, 100));
				int nDarkness   = max(0, min(pAction->Darkness, 100));
				int nRedLevel   = max(0, min(pAction->RedLevel, 100));
				int nGreenLevel = max(0, min(pAction->GreenLevel, 100));
				int nBlueLevel  = max(0, min(pAction->BlueLevel, 100));

				dCompleteness *= 2 - dCompleteness; // квадратичное изменение времени в интревале 0..1
				
				float fLevel      = 0.01f * float(dCompleteness * nLevel);
				float fDarkness   = 0.01f * float(dCompleteness * nDarkness);
				float fRedLevel   = 0.01f * float(dCompleteness * (100 - nRedLevel));
				float fGreenLevel = 0.01f * float(dCompleteness * (100 - nGreenLevel));
				float fBlueLevel  = 0.01f * float(dCompleteness * (100 - nBlueLevel));

				fLevel      = fLevel; // * 0.7f + 0.3f;
				fDarkness   = 1 - 0.5f * fDarkness;
				fRedLevel   = fDarkness * (1 - fRedLevel);
				fGreenLevel = fDarkness * (1 - fGreenLevel);
				fBlueLevel  = fDarkness * (1 - fBlueLevel);

				int nResult = Core::COM::COM_AdjustSepia( m_oSources.Get( pAction->SourceId ), fLevel, fRedLevel, fGreenLevel, fBlueLevel );
				
				return (Core::c_nErrorNone == nResult);
			}

			BOOL Process_Adjust_WhiteBalance(ImageStudio::Serialize::Effects::AdjustWhiteBalance* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustWhiteBalance( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->nRed, pAction->nGreen, pAction->nBlue );
				
				return (Core::c_nErrorNone == nResult);
			}

			BOOL Process_Adjust_AutoWhiteBalance(ImageStudio::Serialize::Effects::AdjustAutoWhiteBalance* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustAutoWhiteBalance( m_oSources.Get( pAction->SourceId ), dCompleteness );

				return (Core::c_nErrorNone == nResult);
			}

			BOOL Process_Adjust_Clarity(ImageStudio::Serialize::Effects::AdjustClarity* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustClarity( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level, pAction->Radius, pAction->Threshold );
				
				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Vibrance(ImageStudio::Serialize::Effects::AdjustVibrance* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustVibrance( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level );
				
				return (Core::c_nErrorNone == nResult);
			}

			
			BOOL Process_Adjust_Recovery(ImageStudio::Serialize::Effects::AdjustRecovery* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustRecovery( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->nLevel );
				
				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_FillLight(ImageStudio::Serialize::Effects::AdjustFillLight* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustFillLight( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->nLevel );
				
				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Exposure(ImageStudio::Serialize::Effects::AdjustExposure* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustExposure( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->nLevel );
				
				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Blacks(ImageStudio::Serialize::Effects::AdjustBlacks* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustBlacks( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->nLevel );
				
				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_HighLight(ImageStudio::Serialize::Effects::AdjustHighLight* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustHighLight( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->nFillLight, pAction->nExposure );
				
				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Adjust_Shadows(ImageStudio::Serialize::Effects::AdjustShadows* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_AdjustShadows( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->nBlacks, pAction->nRecovery );
				
				return (Core::c_nErrorNone == nResult);
			}

			
			BOOL Process_Effect_Blur(ImageStudio::Serialize::Effects::EffectBlur* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.0001 ) 
					return TRUE;

				dx9::EffectBlur blur;
				blur.m_dLevel = pAction->Level;

				int nRetCode = UpdateVideoEffect( &blur, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_EffectBlur( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Sharpen(ImageStudio::Serialize::Effects::EffectSharpen* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				dx9::EffectSharpen sharpen;
				sharpen.m_dLevel = pAction->Level;

				int nRetCode = UpdateVideoEffect( &sharpen, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_EffectSharpen( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Mosaic(ImageStudio::Serialize::Effects::EffectMosaic* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nMetric = (2 == pAction->Metric) ? 0 : pAction->Metric;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), nMetric, dMetricX, dMetricY ) )
					return FALSE;

				double dSize = pAction->Size * dMetricX;
				dSize += dSize > 0 ? 0.5 : -0.5;

				int nResult = Core::COM::COM_EffectMosaic( m_oSources.Get( pAction->SourceId ), dCompleteness, long(dSize), pAction->Simple );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_GaussianBlur(ImageStudio::Serialize::Effects::EffectGaussianBlur* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.0001 ) 
					return TRUE;

				int nMetric = (2 == pAction->Metric) ? 0 : pAction->Metric;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), nMetric, dMetricX, dMetricY ) )
					return FALSE;

				int nSize = Round( pAction->Size * dMetricX );

				dx9::EffectGaussinBlur blur;
				blur.m_nSize = nSize;

				int nRetCode = UpdateVideoEffect( &blur, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_EffectGaussianBlur( m_oSources.Get( pAction->SourceId ), dCompleteness, nSize );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Noise(ImageStudio::Serialize::Effects::EffectNoise* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectNoise( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level, pAction->Monochrome, GetBaseRandom( pAction->BaseRandom ) + DWORD(m_dTime) );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Diffuse(ImageStudio::Serialize::Effects::EffectDiffuse* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nMetric = (2 == pAction->Metric) ? 0 : pAction->Metric;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, nMetric, dMetricX, dMetricY ) )
					return FALSE;

				double dDistance = pAction->Distance * dMetricX;

				int nResult = Core::COM::COM_EffectDiffuse( m_oSources.Get( pAction->SourceId ), dCompleteness, dDistance, GetBaseRandom( pAction->BaseRandom ) + DWORD(m_dTime) );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_MotionBlur(ImageStudio::Serialize::Effects::EffectMotionBlur* pAction)
			{
				if (!pAction)
					return FALSE;

				if( m_oSources.IsBadIndex( pAction->SourceId ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nMetric = (2 == pAction->Metric) ? 0 : pAction->Metric;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, nMetric, dMetricX, dMetricY ) )
					return FALSE;

				double dDistance = pAction->Distance * dMetricX;
				double dAngle    = -pAction->Angle;

				dx9::EffectMotionBlur motion;
				motion.m_dDistance = dDistance;
				motion.m_dAngle = dAngle;

				int nRetCode = UpdateVideoEffect( &motion, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_EffectMotionBlur( m_oSources.Get( pAction->SourceId ), dCompleteness, dAngle, dDistance );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Emboss(ImageStudio::Serialize::Effects::EffectEmboss* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nMetric = (2 == pAction->Metric) ? 0 : pAction->Metric;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), nMetric, dMetricX, dMetricY ) )
					return FALSE;

				double dAngle    = pAction->Angle;
				double dDistance = pAction->Distance * dMetricX;
				double dAmount   = pAction->Amount;// * dMetricX;

				int nResult = Core::COM::COM_EffectEmboss( m_oSources.Get( pAction->SourceId ), dCompleteness, dAngle, dDistance, dAmount, pAction->bGrayScale );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_KenBurns(ImageStudio::Serialize::Effects::EffectKenBurns* pAction)
			{
				if ( NULL == pAction && ( NULL == m_oSources.Get(0) ) )
					return FALSE;

				double dCompleteness = pAction->GetCompleteness ( m_dTime );
				if ( dCompleteness < 0.000001 ) 
					return TRUE;
				
				double CX	= 0.0;
				double CY	= 0.0;
				
				double Zoom	= 1.0;
				
				Gdiplus::RectF TransformRegion;
				if ( FALSE == pAction->GetTransform ( pAction->GetCompletenessStage ( m_dTime ), TransformRegion ) )
					return FALSE;
			
				IUnknown* pClone = m_oSources.CloneToBgra ( 0, TRUE );
				if ( NULL == pClone )
					return FALSE;

				double dMetricX = 1, dMetricY = 1;
				GetMetricMultiplier ( m_oSources.Get( 0 ), pAction->Metric, dMetricX, dMetricY );
				
				float Width		=	(float) m_oSources.Get(0)->Width;
				float Height	=	(float) m_oSources.Get(0)->Height;

				float dLeft		=	0.0;
				float dRight	=	0.0;
				float dTop		=	0.0;
				float dBottom	=	0.0;

				if ( ImageStudio::Serialize::c_nMetricPixels == pAction->Metric )
				{
					dLeft		=	TransformRegion.X;
					dRight		=	TransformRegion.Width	+ TransformRegion.X;
					dTop		=	TransformRegion.Y;
					dBottom		=	TransformRegion.Height	+ TransformRegion.Y;
				}

				if ( ImageStudio::Serialize::c_nMetricPercents == pAction->Metric )
				{
					dLeft		=	( TransformRegion.X ) * 0.01f * Width;
					dRight		=	( TransformRegion.Width	+ TransformRegion.X ) * 0.01f * Width;
					dTop		=	( TransformRegion.Y ) * 0.01f * Height;
					dBottom		=	( TransformRegion.Height + TransformRegion.Y ) * 0.01f * Height;
				}

				if ( ImageStudio::Serialize::c_nMetricNormalizedCoordinates == pAction->Metric )
				{
					if ( (float)abs ( TransformRegion.Width ) < 0.0001f || (float)abs ( TransformRegion.Height ) < 0.0001f )
						return FALSE;

					dLeft		=	( TransformRegion.X ) * Width;
					dRight		=	( TransformRegion.Width	+ TransformRegion.X ) * Width;
					dTop		=	( TransformRegion.Y ) * Height;
					dBottom		=	( TransformRegion.Height + TransformRegion.Y ) * Height;
				}

				if ( ImageStudio::Serialize::c_nMetricAlternativeNormalizedCoordinates == pAction->Metric )
				{
					dLeft		=	( TransformRegion.X ) * (float)dMetricX;
					dRight		=	( TransformRegion.Width		+ TransformRegion.X ) * (float)dMetricX;
					dTop		=	( TransformRegion.Y ) * (float)dMetricY;
					dBottom		=	( TransformRegion.Height	+ TransformRegion.Y ) * (float)dMetricY;
				}

				BYTE* pCanvasBGRA = NULL;
				int nCanvasWidth = 0;
				int nCanvasHeight = 0;
				int nCanvasStride = 0;

				// вытаскиваем из интерфейса указатель на память в формате BGRA
				int nStatus		=	Core::COM::COM_GetBGRAImageFromInterface ( m_oSources.Get(0), pCanvasBGRA, nCanvasWidth, nCanvasHeight, &nCanvasStride);
				if (nStatus != Core::c_nErrorNone)
				{
					RELEASEINTERFACE ( pClone );
					return nStatus;
				}

				int nResult		=	Core::c_nErrorNone;

				Gdiplus::Bitmap oCanvas( nCanvasWidth, nCanvasHeight, nCanvasStride, PixelFormat32bppARGB, pCanvasBGRA );
				if( Gdiplus::Ok == oCanvas.GetLastStatus() )
				{
					Gdiplus::Graphics oRender( &oCanvas );
					if( Gdiplus::Ok == oRender.GetLastStatus() )
					{
						nStatus		=	Core::COM::COM_GetBGRAImageFromInterface ( pClone, pCanvasBGRA, nCanvasWidth, nCanvasHeight, &nCanvasStride);
						if (nStatus != Core::c_nErrorNone)
						{
							RELEASEINTERFACE ( pClone );
							return nStatus;
						}

						Gdiplus::Bitmap oCanvas2 ( nCanvasWidth, nCanvasHeight, nCanvasStride, PixelFormat32bppARGB, pCanvasBGRA );
						if( Gdiplus::Ok == oCanvas2.GetLastStatus() )
						{
							oRender.Clear ( 0xFF000000 );

							oRender.DrawImage ( &oCanvas2,  Gdiplus::RectF ( 0.0f, 0.0f, (float) Width, (float) Height ), dLeft, dTop, dRight - dLeft, dBottom - dTop, UnitPixel );
						}
					}
				}

				RELEASEINTERFACE ( pClone );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Minimal(ImageStudio::Serialize::Effects::EffectMinimal* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nMetric = (2 == pAction->Metric) ? 0 : pAction->Metric;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, nMetric, dMetricX, dMetricY ) )
					return FALSE;

				int nSize = Round( pAction->Size * dMetricX );

				dx9::EffectMinimal minimal;
				minimal.m_nSize = nSize;

				int nRetCode = UpdateVideoEffect( &minimal, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_EffectMinimal( m_oSources.Get( pAction->SourceId ), dCompleteness, nSize );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Maximal(ImageStudio::Serialize::Effects::EffectMaximal* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nMetric = (2 == pAction->Metric) ? 0 : pAction->Metric;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, nMetric, dMetricX, dMetricY ) )
					return FALSE;

				int nSize = Round( pAction->Size * dMetricX );

				dx9::EffectMaximal maximal;
				maximal.m_nSize = nSize;

				int nRetCode = UpdateVideoEffect( &maximal, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_EffectMaximal( m_oSources.Get( pAction->SourceId ), dCompleteness, nSize );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Deinterlace(ImageStudio::Serialize::Effects::EffectDeinterlace* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.000001)
					return TRUE;

				int nResult = Core::COM::COM_EffectDeinterlace( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Even, pAction->Interpolate );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Median(ImageStudio::Serialize::Effects::EffectMedian* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.000001)
					return TRUE;

				int nResult = Core::COM::COM_EffectMedian( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_ChromoKey_Range(ImageStudio::Serialize::Effects::EffectChromoKeyRange* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.000001)
					return TRUE;

				if( m_oSources.IsBadIndex( pAction->GetBufferIndex() ) )
					return FALSE;

				int nResult = Core::COM::COM_EffectChromoKeyRange(m_oSources.Get( pAction->GetBufferIndex() ), pAction->GetColor1(), pAction->GetThreshold1(), pAction->GetColor2(), pAction->GetThreshold2(), dCompleteness, (1 == pAction->GetComposeAlpha()), pAction->GetEdgesSmooth () );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_ChromoKey_Level(ImageStudio::Serialize::Effects::EffectChromoKeyLevel* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.000001)
					return TRUE;

				if( m_oSources.IsBadIndex( pAction->GetBufferIndex() ) )
					return FALSE;

				int nCountColors = pAction->arrColors.GetSize();
				for( int i = 0; i < nCountColors; ++i )
				{
					Core::COM::COM_EffectChromoKeyLevel( m_oSources.Get( pAction->GetBufferIndex() ), pAction->arrColors[i], pAction->GetThreshold(), dCompleteness, (1 == pAction->GetComposeAlpha()), pAction->GetEdgesSmooth () );
				}

				return Core::c_nErrorNone;
			}
			BOOL Process_Effect_Cinema(ImageStudio::Serialize::Effects::EffectCinema* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dTime = pAction->GetRelativeTime(m_dTime);

				int nSessionID = pAction->GetSessionID();
				ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
				if (NULL == pCacheItem || ImageStudio::Caching::c_nTypePointer != pCacheItem->GetType())
				{
					ImageStudio::Paint::Effects::Cinema* pEffect = new ImageStudio::Paint::Effects::Cinema( );
			
					pEffect->Create( *pAction );

					m_oCache.Add(nSessionID, pEffect);

					pCacheItem = pEffect;
				}

				ImageStudio::Paint::Effects::Cinema* pPainter = (ImageStudio::Paint::Effects::Cinema*)pCacheItem;

				// отрисовываем текст
				int nResult = Core::COM::COM_EffectCinema(m_oSources.Get( pAction->SourceId ), pPainter, dTime, dCompleteness, pAction->m_nBrightness, pAction->m_nContrast);
				
				return TRUE;
			}

			BOOL Process_Effect_Glass(ImageStudio::Serialize::Effects::EffectGlass* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectGlass( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Radius );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_GlassMosaic(ImageStudio::Serialize::Effects::EffectGlassMosaic* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;
				
				double dSizeX = pAction->SizeX * dMetricX;
				double dSizeY = pAction->SizeY * dMetricY;
				double dOffsetX = pAction->OffsetX * dMetricX;
				double dOffsetY = pAction->OffsetY * dMetricY;

				dx9::EffectGlassMosaic mosaic;
				mosaic.m_dSizeX     = dSizeX;
				mosaic.m_dSizeY     = dSizeY;
				mosaic.m_dOffsetX   = dOffsetX;
				mosaic.m_dOffsetY   = dOffsetY;
				mosaic.m_dCurvature = pAction->Curvature;
				mosaic.m_dAngle     = pAction->Angle;
				mosaic.m_dSmooth    = pAction->Smooth / 10;
				mosaic.m_eQuality   = pAction->Quality;
				mosaic.m_dScale     = pAction->Scale;
				mosaic.m_dMaxFactor = pAction->MaxFactor;

				int nRetCode = UpdateVideoEffect( &mosaic, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_EffectGlassMosaic( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Angle, dSizeX, dSizeY, dOffsetX, dOffsetY, pAction->Curvature, pAction->Smooth / 10, pAction->Quality );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_PencilSketch(ImageStudio::Serialize::Effects::EffectPencilSketch* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectPencilSketch( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level, pAction->m_nBrightness, pAction->m_nContrast, pAction->m_nLengthFactor );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_WaterColor(ImageStudio::Serialize::Effects::EffectWaterColor* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectWaterColor( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->BrushSize, pAction->Coarseness );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Glow(ImageStudio::Serialize::Effects::EffectGlow* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectGlow( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Size );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_TVSimulation(ImageStudio::Serialize::Effects::EffectTVSimulation* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dTime = pAction->GetRelativeTime(m_dTime);

				int nSessionID = pAction->GetSessionID();
				ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
				if (NULL == pCacheItem || ImageStudio::Caching::c_nTypePointer != pCacheItem->GetType())
				{
					ImageStudio::Paint::Effects::TVSimulator* pEffect = new ImageStudio::Paint::Effects::TVSimulator();

					double dMetricX, dMetricY;
					GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY );

					pEffect->Create( *pAction, dMetricX, dMetricY );
			
					m_oCache.Add(nSessionID, pEffect);

					pCacheItem = pEffect;
				}

				ImageStudio::Paint::Effects::TVSimulator* pPainter = (ImageStudio::Paint::Effects::TVSimulator*)pCacheItem;

				// отрисовываем текст
				int nResult = Core::COM::COM_EffectTVSimulation( m_oSources.Get( pAction->SourceId ), pPainter, dTime, dCompleteness);

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_GrayOut(ImageStudio::Serialize::Effects::EffectGrayOut* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectGrayOut( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Color, pAction->Filter, pAction->LevelRed, pAction->LevelGreen, pAction->LevelBlue, pAction->Threshold, pAction->Type );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Wind(ImageStudio::Serialize::Effects::EffectWind* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->nMetric, dMetricX, dMetricY ) )
					return FALSE;

				int nLength = int(pAction->dLength * dMetricX);

				int nResult = Core::COM::COM_EffectWind( m_oSources.Get( pAction->SourceId ), dCompleteness, nLength, pAction->nDensity );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Newsprint(ImageStudio::Serialize::Effects::EffectNewsprint* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				dx9::EffectNewsprint newsprint;
				newsprint.m_dLevel = pAction->Level;

				int nRetCode = UpdateVideoEffect( &newsprint, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_EffectNewsprint( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Level, pAction->Filter );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Snow(ImageStudio::Serialize::Effects::EffectSnow* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dTime = pAction->GetRelativeTime(m_dTime);

				int nSessionID = pAction->GetSessionID();
				ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
				if (NULL == pCacheItem || ImageStudio::Caching::c_nTypePointer != pCacheItem->GetType())
				{
					ImageStudio::Paint::Effects::Snow* pEffect = new ImageStudio::Paint::Effects::Snow();

					pEffect->Create( *pAction );
			
					m_oCache.Add(nSessionID, pEffect);

					pCacheItem = pEffect;
				}

				ImageStudio::Paint::Effects::Snow* pPainter = (ImageStudio::Paint::Effects::Snow*)pCacheItem;

				// отрисовываем текст
				int nResult = Core::COM::COM_EffectSnow( m_oSources.Get( pAction->SourceId ), pPainter, dTime, dCompleteness, pAction->m_bSnowDrift, pAction->m_dSnowDriftSpeedFactor);

				return (Core::c_nErrorNone == nResult);
			}

			BOOL Process_Effect_JpegCompression(ImageStudio::Serialize::Effects::EffectJpegCompression* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectJpegCompression( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->m_nQuality);

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_SoftFocus(ImageStudio::Serialize::Effects::EffectSoftFocus* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				double dBlurSize = pAction->BlurSize * dMetricX;
				dBlurSize += dBlurSize > 0 ? 0.5 : -0.5;

				return Core::COM::COM_EffectSoftFocus( m_oSources.Get( pAction->SourceId ), dCompleteness, dBlurSize, pAction->Intensity, pAction->Type );
			}
			BOOL Process_Effect_OldPhoto(ImageStudio::Serialize::Effects::EffectOldPhoto* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nSessionID = pAction->GetSessionID();
				ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
				if (NULL == pCacheItem || ImageStudio::Caching::c_nTypePointer != pCacheItem->GetType())
				{
					ImageStudio::Paint::Effects::OldPhoto* pEffect = new ImageStudio::Paint::Effects::OldPhoto();
					
					m_oCache.Add(nSessionID, pEffect);

					pCacheItem = pEffect;
				}

				ImageStudio::Paint::Effects::OldPhoto* pPainter = (ImageStudio::Paint::Effects::OldPhoto*)pCacheItem;


				int nResult = Core::COM::COM_EffectOldPhoto( m_oSources.Get( pAction->SourceId ), dCompleteness, pPainter, pAction->m_nMask, pAction->m_nClarity, pAction->m_WhiteBlack );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Vignette(ImageStudio::Serialize::Effects::EffectVignette* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, pAction->nMetric, dMetricX, dMetricY ) )
					return FALSE;

				double dCenterX  = dMetricX * pAction->dCenterX;
				double dCenterY  = dMetricY * pAction->dCenterY;
				double dRadius1X = dMetricX * pAction->dRadius1X;
				double dRadius1Y = dMetricY * pAction->dRadius1Y;
				double dRadius2X = dMetricX * pAction->dRadius2X;
				double dRadius2Y = dMetricY * pAction->dRadius2Y;
				
				int nResult = Core::COM::COM_EffectVignette( m_oSources.Get( pAction->SourceId ), dCompleteness, dCenterX, dCenterY, dRadius1X, dRadius1Y, dRadius2X, dRadius2Y, pAction->dAngle, pAction->dIntensity );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_PostCrop(ImageStudio::Serialize::Effects::EffectPostCrop* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectPostCrop( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->dAmount, pAction->dMidpoint, pAction->dRoundness, pAction->dFeather );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_SetAlphaChannel(ImageStudio::Serialize::Effects::EffectSetAlphaChannel* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				IUnknown* pMask = m_oSources.Get( pAction->nSrcBuffer );
				if( !pMask )
				{
					Core::COM::COM_LoadImage( &pMask, pAction->sFilePath, 0, FALSE );
				}
				else
				{
					pMask->AddRef();
				}

				if( !pMask )
					return TRUE;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, pAction->nMetric, dMetricX, dMetricY ) )
				{
					pMask->Release();
					return FALSE;
				}

				double dX = pAction->dX * dMetricX;
				double dY = pAction->dY * dMetricY;
				double dW = pAction->dWidth  * dMetricX;
				double dH = pAction->dHeight * dMetricY;

				int nResult = Core::COM::COM_EffectSetAlphaChannel( m_oSources.Get( pAction->SourceId ), pMask, dCompleteness, pAction->nType, dX, dY, dW, dH );

				pMask->Release();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_ZoomingSphere(ImageStudio::Serialize::Effects::EffectZoomingSphere* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectZoomingSphere( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Power );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_WaterMirrow(ImageStudio::Serialize::Effects::EffectWaterMirrow* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, 2, dMetricX, dMetricY ) )
					return FALSE;

				double m[9]; // матрица отражения

				if( !pAction->UpdateMask( (int)dMetricX, (int)dMetricY, m ) )
					return TRUE;

				int nResult = Core::COM::COM_EffectWaterMirrow( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->m_oMask.GetPixels(), m, pAction->AlphaMax, pAction->AlphaMin );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_ShapeMosaic(ImageStudio::Serialize::Effects::EffectShapeMosaic* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, pAction->nMetric, dMetricX, dMetricY ) )
					return FALSE;

				int nMosaicSize = Round( dMetricX * pAction->dMosaicSize );

				int nResult = Core::COM::COM_EffectShapeMosaic( m_oSources.Get( pAction->SourceId ), dCompleteness, nMosaicSize );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Particles(ImageStudio::Serialize::Effects::EffectParticles* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dTime = pAction->GetRelativeTime(m_dTime);

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, pAction->nMetric, dMetricX, dMetricY ) )
					return FALSE;

				int nSessionID = pAction->GetSessionID();
				ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
				if (NULL == pCacheItem || ImageStudio::Caching::c_nTypePointer != pCacheItem->GetType())
				{
					ImageStudio::Paint::Effects::Particles* pEffect = new ImageStudio::Paint::Effects::Particles();

					IMediaFrame* pFrame = NULL;
					Core::COM::COM_LoadImage( (IUnknown**)&pFrame, pAction->sFilePath, 0, 0 );

					pEffect->SetSorceFrame( pFrame, RealToByte( pAction->nTransparency / 100.0 * 255.0 ) );

					pEffect->m_dDirBaseAngle = pAction->dBaseDirAngle;
					pEffect->m_dDirDispAngle = pAction->dDispDirAngle;
					pEffect->m_dMinSize      = pAction->dMinSize * dMetricX;
					pEffect->m_dMaxSize      = pAction->dMaxSize * dMetricX;
					pEffect->m_dMinLifeTime  = pAction->dMinLifeTime;
					pEffect->m_dMaxLifeTime  = pAction->dMaxLifeTime;
					pEffect->m_dMinRotTime   = pAction->dMinRotTime;
					pEffect->m_dMaxRotTime   = pAction->dMaxRotTime;
					pEffect->m_dMinRotAngle  = pAction->dMinRotAngle;
					pEffect->m_dMaxRotAngle  = pAction->dMaxRotAngle;
					pEffect->m_nAbsRot       = pAction->bAbsRot;
					pEffect->m_nRotType      = pAction->nRotType;
			
					pEffect->CreateParticles( pAction->nMaxCountParticles );
					
					m_oCache.Add(nSessionID, pEffect);

					pCacheItem = pEffect;
				}

				ImageStudio::Paint::Effects::Particles* pPainter = (ImageStudio::Paint::Effects::Particles*)pCacheItem;

				// отрисовываем текст
				int nResult = Core::COM::COM_EffectParticles( m_oSources.Get( pAction->SourceId ), pPainter, dTime, dCompleteness );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Canvas(ImageStudio::Serialize::Effects::EffectCanvas* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectCanvas( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Alpha, GetBaseRandom( pAction->BaseRandom ) );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Glow2(ImageStudio::Serialize::Effects::EffectGlow2* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				double dBlurSize = pAction->Size * dMetricX;

				return Core::COM::COM_EffectSoftFocus( m_oSources.Get( pAction->SourceId ), dCompleteness, dBlurSize, 100, 0 );
			}
			BOOL Process_Effect_ColorExchange(ImageStudio::Serialize::Effects::EffectColorExchange* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				int nResult = Core::COM::COM_EffectColorExchange( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Color1, pAction->Color2 );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Effect_Anaglyph3d(ImageStudio::Serialize::Effects::EffectAnaglyph3d* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, 2, dMetricX, dMetricY ) )
					return FALSE;

				int nResult = Core::COM::COM_EffectAnaglyph3d( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->nDepth / 10.0, pAction->nType );

				return (Core::c_nErrorNone == nResult);
			}
			
			BOOL Process_Transform_Flip(ImageStudio::Serialize::Effects::TransformFlip* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				dx9::EffectZoom zoom;
				zoom.m_dZoomX = pAction->Vertical ?  1 : -1;
				zoom.m_dZoomY = pAction->Vertical ? -1 :  1;
				zoom.m_nBackColor = dx9::GetColor( pAction->BackColor, pAction->BackAlpha );

				int nRetCode = UpdateVideoEffect( &zoom, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_TransformFlip( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Vertical, pAction->BackColor, pAction->BackAlpha );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Flip2(ImageStudio::Serialize::Effects::TransformFlip2* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_TransformFlip2( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Dir, pAction->BackColor, pAction->BackAlpha );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Perspective(ImageStudio::Serialize::Effects::TransformPerspective* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				dx9::EffectPerspective perspective;
				perspective.m_bVertical    = pAction->Vertical;
				perspective.m_dAngle       = pAction->Angle;
				perspective.m_nBackColor   = dx9::GetColor( pAction->BackColor, pAction->BackAlpha );
				
				perspective.m_bAlternative = pAction->Alternative;
				perspective.m_oPoints[0].x = pAction->X1;
				perspective.m_oPoints[0].y = pAction->Y1;
				perspective.m_oPoints[1].x = pAction->X2;
				perspective.m_oPoints[1].y = pAction->Y2;
				perspective.m_oPoints[2].x = pAction->X3;
				perspective.m_oPoints[2].y = pAction->Y3;
				perspective.m_oPoints[3].x = pAction->X4;
				perspective.m_oPoints[3].y = pAction->Y4;
				
				int nRetCode = UpdateVideoEffect( &perspective, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_TransformPerspective( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Vertical, pAction->Angle, pAction->BackColor, pAction->BackAlpha );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Skew(ImageStudio::Serialize::Effects::TransformSkew* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				dx9::EffectSkew skew;
				skew.m_dAngle     = pAction->Angle;
				skew.m_bVertical  = pAction->Vertical;
				skew.m_nBackColor = dx9::GetColor( pAction->BackColor, pAction->BackAlpha );

				int nRetCode = UpdateVideoEffect( &skew, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_TransformSkew( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Vertical, pAction->Angle, pAction->BackColor, pAction->BackAlpha );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Shift(ImageStudio::Serialize::Effects::TransformShift* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				dx9::EffectZoom zoom;
				zoom.m_dZoomX = 1;
				zoom.m_dZoomY = 1;
				zoom.m_dDispX = pAction->OffsetX;
				zoom.m_dDispY = pAction->OffsetY;
				zoom.m_nMetric = pAction->Metric;
				zoom.m_nBackColor = dx9::GetColor( pAction->BackColor, pAction->BackAlpha );

				int nRetCode = UpdateVideoEffect( &zoom, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// в зависимости от типа метрики пересчитываем координаты
				double dOffsetX = pAction->OffsetX * dMetricX;
				double dOffsetY = pAction->OffsetY * dMetricY;

				dOffsetX += dOffsetX > 0 ? 0.5 : -0.5;
				dOffsetY += dOffsetY > 0 ? 0.5 : -0.5;

				int nResult = Core::COM::COM_TransformShift( m_oSources.Get( pAction->SourceId ), dCompleteness, long(dOffsetX), long(dOffsetY), pAction->BackColor, pAction->BackAlpha );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Rotate(ImageStudio::Serialize::Effects::TransformRotate* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				dx9::EffectRotate rotate;
				rotate.m_dAngle = pAction->Angle;
				rotate.m_nType  = pAction->Size;
				rotate.m_nBackColor = dx9::GetColor( pAction->BackColor, pAction->BackAlpha );

				int nRetCode = UpdateVideoEffect( &rotate, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::c_nErrorNone;

				IMediaFrame* pSrcFrame =  m_oSources.Get( pAction->SourceId );

				if (pAction->Size == 1)
				{
					nResult = Core::COM::COM_TransformRotate( pSrcFrame, dCompleteness, pAction->Angle, pAction->BackColor, pAction->BackAlpha );
				}
				else
				{
					IUnknown* pResult = NULL;
					nResult = Core::COM::COM_TransformRotateMedia( pSrcFrame, &pResult, dCompleteness, pAction->Angle, pAction->BackColor, pAction->BackAlpha, 0 == pAction->Size );
					
					if( pResult )
					{
						m_oSources.Set( pAction->SourceId, pResult );
					}
				}

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_RotateAdvanced(ImageStudio::Serialize::Effects::TransformRotateAdvanced* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				//if (pAction->IndexFrom < 0 || pAction->IndexFrom >= c_nInterfacesCount || pAction->IndexTo < 0 || pAction->IndexTo >= c_nInterfacesCount)
				//	return FALSE;

				IUnknown* pSource = m_oSources.Get( pAction->IndexFrom );
				IUnknown* pResult = m_oSources.Get( pAction->IndexTo );

				if (NULL == pSource || NULL == pResult)
					return TRUE;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pResult, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// в зависимости от типа метрики пересчитываем координаты
				double dCenterX = pAction->CenterX * dMetricX;
				double dCenterY = pAction->CenterY * dMetricY;
				double dAngle   = pAction->Angle * dCompleteness;
				double dScale   = pAction->Scale * dCompleteness;

				int nResult = Core::COM::COM_Rotate( pSource, pResult, dCenterX, dCenterY, dAngle, dScale, dScale, pAction->Alpha, pAction->Interpolate );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_RotateSimple(ImageStudio::Serialize::Effects::TransformRotateSimple* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_RotateSimple( m_oSources.Get( pAction->SourceId ), pAction->Type );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Resample(ImageStudio::Serialize::Effects::TransformResample* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				double scale = max(1, pAction->Times);

				dx9::EffectZoom zoom;
				zoom.m_dZoomX = 1 / scale;
				zoom.m_dZoomY = 1 / scale;
				zoom.m_bResample = TRUE;

				int nRetCode = UpdateVideoEffect( &zoom, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_TransformResample( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Times );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Zoom(ImageStudio::Serialize::Effects::TransformZoom* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nZoom = pAction->Zoom; //min(max(pAction->Zoom, -100), 100);
				double dZoom = nZoom / 100.0;
				if( nZoom > 0 )
					dZoom *= 10;

				dZoom += 1;

				dx9::EffectZoom zoom;
				zoom.m_dZoomX = dZoom;
				zoom.m_dZoomY = dZoom;
				zoom.m_dDispX = pAction->dx;
				zoom.m_dDispY = pAction->dy;
				zoom.m_nMetric = pAction->Metric;
				zoom.m_nBackColor = dx9::GetColor( pAction->BackColor, pAction->BackAlpha );

				int nRetCode = UpdateVideoEffect( &zoom, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				
				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				dZoom = (dZoom - 1) * dCompleteness + 1;

				if( dZoom < 0.00001 )
					dZoom = 0.00001;

				if( dMetricX != 1.0 ) dMetricX *= 0.5;
				if( dMetricY != 1.0 ) dMetricY *= 0.5;

				double dx = pAction->dx * dMetricX * dCompleteness;
				double dy = pAction->dy * dMetricY * dCompleteness;

				int nResult = Core::COM::COM_TransformZoom( m_oSources.Get( pAction->SourceId ), dZoom, dx, dy, pAction->BackColor, pAction->BackAlpha );
				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_ZoomLinear(ImageStudio::Serialize::Effects::TransformZoomLinear* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001 )
					return TRUE;

				dx9::EffectZoom zoom;
				zoom.m_dZoomX = pAction->dZoomX;
				zoom.m_dZoomY = pAction->dZoomY;
				zoom.m_dDispX = -pAction->dx;
				zoom.m_dDispY = pAction->dy;
				zoom.m_nMetric = pAction->Metric;
				zoom.m_nBackColor = dx9::GetColor( pAction->BackColor, pAction->BackAlpha );

				int nRetCode = UpdateVideoEffect( &zoom, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// корректировка входных данных
				if( dMetricX != 1.0 ) dMetricX *= 0.5;
				if( dMetricY != 1.0 ) dMetricY *= 0.5;

				double dx = pAction->dx * dMetricX * dCompleteness;
				double dy = pAction->dy * dMetricY * dCompleteness;
				double dScaleX = 1 + (pAction->dZoomX - 1) * dCompleteness;
				double dScaleY = 1 + (pAction->dZoomY - 1) * dCompleteness;

				// получаем данные картинки-источника из интерфейса
				ImageStudio::Core::MediaData oSrcMediaData;
				if (!oSrcMediaData.Create( m_oSources.Get( pAction->SourceId ), FALSE) || !oSrcMediaData.IsValidBGRA())
					return FALSE;

				BYTE* pSrcPixels = oSrcMediaData.GetBuffer();
				int   nSrcWidth  = oSrcMediaData.GetWidth();
				int   nSrcHeight = oSrcMediaData.GetHeight();
				int   nSrcStride = oSrcMediaData.GetStride();

				// создаём картинку-приёмник на основе интерфейса-источника
				ImageStudio::Core::MediaData oDstMediaData;
				if (!oDstMediaData.Create( m_oSources.Get( pAction->SourceId ), FALSE) || !oDstMediaData.IsValidBGRA())
					return FALSE;

				BYTE* pDstPixels = oDstMediaData.GetBuffer();
				int   nDstWidth  = oDstMediaData.GetWidth();
				int   nDstHeight = oDstMediaData.GetHeight();
				int   nDstStride = oDstMediaData.GetStride();
				
				// заполняем картинку-приёмник фоновым цветом
				Core::BGRA::ClearImage( pDstPixels, nDstWidth, nDstHeight, nDstWidth * 4, pAction->BackColor, pAction->BackAlpha );

				// определяем границы и позицию рабочей области на источнике и приёмнике
				double dSrcScaleW = nSrcWidth * dScaleX;
				double dSrcScaleH = nSrcHeight * dScaleY;
				double dSrcScaleX = (nDstWidth - dSrcScaleW)*0.5 - dx * dScaleX;
				double dSrcScaleY = (nDstHeight - dSrcScaleH)*0.5 - dy * dScaleY;

				Gdiplus::RectF oSrcScaleRect( (float)dSrcScaleX, (float)dSrcScaleY, (float)dSrcScaleW, (float)dSrcScaleH );
				Gdiplus::RectF oOutRect( 0, 0, (float)nDstWidth, (float)nDstHeight );

				if( oOutRect.Intersect( oSrcScaleRect ) )
				{
					IppiRect oSrcRect;
					oSrcRect.x      = int((oOutRect.X - oSrcScaleRect.X) / dScaleX + 0.5);
					oSrcRect.y      = int((oOutRect.Y - oSrcScaleRect.Y) / dScaleY + 0.5);
					oSrcRect.width  = int(oOutRect.Width / dScaleX + 0.5);
					oSrcRect.height = int(oOutRect.Height / dScaleY + 0.5);

					IppiRect oDstRect;
					oDstRect.x      = int(oOutRect.X + 0.5f);
					oDstRect.y      = int(oOutRect.Y + 0.5f);
					oDstRect.width  = int(oOutRect.Width + 0.5f);
					oDstRect.height = int(oOutRect.Height + 0.5f);

					// коррекция источника
					if( --oSrcRect.x < 0 ) oSrcRect.x = 0;
					if( --oSrcRect.y < 0 ) oSrcRect.y = 0;
					if( ++oSrcRect.width  > nSrcWidth  ) oSrcRect.width  = nSrcWidth;
					if( ++oSrcRect.height > nSrcHeight ) oSrcRect.height = nSrcHeight;

					// коррекция приёмника
					pDstPixels += oDstRect.x*4 + oDstRect.y*nDstStride; 

					// сообственно само масштабирование со сдвигом
					// !!! не применять ippiResizeCenter(). Она глючит!
					IppiSize oSrcSize = {nSrcWidth, nSrcHeight};
					IppiSize oDstSize = {oDstRect.width, oDstRect.height};
					
					ippiResize_8u_C4R( pSrcPixels, oSrcSize, nSrcStride, oSrcRect, pDstPixels, nDstStride, oDstSize, dScaleX, dScaleY, IPPI_INTER_LINEAR);
				}

				// получаем результирующий интерфейс
				m_oSources.Set( 0, oDstMediaData.GetMediaData(TRUE) );
				
				oSrcMediaData.Destroy();
				oDstMediaData.Destroy();

				return m_oSources.IsValidSource( 0 );
			}
			BOOL Process_Transform_Mirror(ImageStudio::Serialize::Effects::TransformMirror* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_TransformMirror( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type, pAction->Level/100.0);

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Resize(ImageStudio::Serialize::Effects::TransformResize* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				double dMetricX, dMetricY;
				if( !m_oSources.GetMetric( pAction->SourceId, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				double dNewWidth  = pAction->Width  * dMetricX;
				double dNewHeight = pAction->Height * dMetricY;

				double dSrcMetricX = dMetricX;
				double dSrcMetricY = dMetricY;

				if( pAction->SrcMetric >= 0 )
				{
					if( !m_oSources.GetMetric( pAction->SourceId, pAction->SrcMetric, dSrcMetricX, dSrcMetricY ) )
						return FALSE;
				}

				int nSrcX = int(pAction->SrcX * dSrcMetricX + 0.5);
				int nSrcY = int(pAction->SrcY * dSrcMetricY + 0.5);
				int nSrcW = int(pAction->SrcWidth  * dSrcMetricX + 0.5);
				int nSrcH = int(pAction->SrcHeight * dSrcMetricY + 0.5);

				
				IO::CMetaFile* pMetaFile =  m_oSources.GetMetafile( pAction->SourceId );
				if( pMetaFile )
				{
					BOOL bResult = ResizeMetaFile( pMetaFile, long(dNewWidth + 0.5), long(dNewHeight + 0.5), pAction->BackColor, pAction->Type, pAction->CropZoom, pAction->BackAlpha, nSrcX, nSrcY, nSrcW, nSrcH );
					if( bResult )
						m_oSources.UpdateMetafile( pAction->SourceId );

					return bResult;

				}

				IUnknown* pResult = NULL;

				int nResult = Core::COM::COM_TransformResizeMedia( m_oSources.Get( pAction->SourceId ), &pResult, long(dNewWidth + 0.5), long(dNewHeight + 0.5), pAction->BackColor, pAction->Type, pAction->CropZoom, pAction->BackAlpha, nSrcX, nSrcY, nSrcW, nSrcH );

				if( pResult )
				{
					m_oSources.Set( 0, pResult );
				}

				return (Core::c_nErrorNone == nResult);
			}

			BOOL Process_Transform_Twirl(ImageStudio::Serialize::Effects::TransformTwirl* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				dx9::EffectTwirl twirl;
				twirl.m_dAngle     = pAction->Angle;
				twirl.m_dDegree    = pAction->Degree;
				twirl.m_dZoom      = pAction->Zoom;
				twirl.m_nBackColor = dx9::GetColor( pAction->BackColor, pAction->BackAlpha );

				int nRetCode = UpdateVideoEffect( &twirl, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_TransformTwirl( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Angle, pAction->Degree, pAction->Zoom, pAction->BackColor, pAction->BackAlpha );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Sphere(ImageStudio::Serialize::Effects::TransformSphere* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				dx9::EffectSphere sphere;
				sphere.m_dCenterX = pAction->CenterX;
				sphere.m_dCenterY = pAction->CenterY;
				sphere.m_dRadiusX = pAction->RadiusX;
				sphere.m_dRadiusY = pAction->RadiusY;
				sphere.m_dDegree  = pAction->Degree;
				sphere.m_nMetric  = pAction->Metric;
				sphere.m_nBackColor = dx9::GetColor( pAction->BackColor, pAction->BackAlpha );

				int nRetCode = UpdateVideoEffect( &sphere, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// в зависимости от типа метрики пересчитываем координаты
				double dCenterX = pAction->CenterX * dMetricX;
				double dCenterY = pAction->CenterY * dMetricY;
				double dRadiusX = pAction->RadiusX * dMetricX;
				double dRadiusY = pAction->RadiusY * dMetricY;
				double dAngle   = pAction->Degree;

				dCenterX += dCenterX > 0 ? 0.5 : -0.5;
				dCenterY += dCenterY > 0 ? 0.5 : -0.5;
				dRadiusX += dRadiusX > 0 ? 0.5 : -0.5;
				dRadiusY += dRadiusY > 0 ? 0.5 : -0.5;

				int nResult = Core::COM::COM_TransformSphere( m_oSources.Get( pAction->SourceId ), dCompleteness, long(dCenterX), long(dCenterY), long(dRadiusX), long(dRadiusY), dAngle, pAction->BackColor, pAction->BackAlpha );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Cylinder(ImageStudio::Serialize::Effects::TransformCylinder* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				dx9::EffectCylinder cylinder;
				cylinder.m_dCenter = pAction->Center;
				cylinder.m_dRadius = pAction->Radius;
				cylinder.m_dDegree = pAction->Degree;
				cylinder.m_nMetric = pAction->Metric;
				cylinder.m_bVertical = pAction->Type == c_nTransformCylinderVert ? TRUE : FALSE;
				cylinder.m_nBackColor = dx9::GetColor( pAction->BackColor, pAction->BackAlpha );

				int nRetCode = UpdateVideoEffect( &cylinder, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;


				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// в зависимости от типа метрики пересчитываем координаты
				double dMetric;
				int nType = pAction->Type;

				if( nType == c_nTransformCylinderVert )
				{
					dMetric = dMetricX;
				}
				else
				{
					dMetric = dMetricY;
					nType = c_nTransformCylinderHoriz;
				}

				double dCenter = pAction->Center * dMetric;
				double dRadius = pAction->Radius * dMetric;
				double dAngle  = pAction->Degree;

				dCenter += dCenter > 0 ? 0.5 : -0.5;
				dRadius += dRadius > 0 ? 0.5 : -0.5;

				int nResult = Core::COM::COM_TransformCylinder( m_oSources.Get( pAction->SourceId ), dCompleteness, long(dCenter), long(dRadius), dAngle, nType, pAction->BackColor, pAction->BackAlpha );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Wave(ImageStudio::Serialize::Effects::TransformWave* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.0001 ) 
					return TRUE;

				BOOL   bVertical    = pAction->bVertical != 0 ? TRUE : FALSE;
				double dPhase       = pAction->dNumPeaks * IPP_2PI;
				double dPhaseOffset = pAction->dPeakOffset + pAction->dSpeed * pAction->GetRelativeTime( m_dTime ) / 1000;

				dPhaseOffset -= int(dPhaseOffset);
				dPhaseOffset *= IPP_2PI;

				
				dx9::EffectWave wave;
				wave.m_dAmplitude = pAction->dAmplitude;
				wave.m_dPhase     = dPhase;
				wave.m_dOffset    = dPhaseOffset;
				wave.m_nMetric    = pAction->nMetric;
				wave.m_bVertical  = bVertical;

				int nRetCode = UpdateVideoEffect( &wave, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->nMetric, dMetricX, dMetricY ) )
					return FALSE;

				double dAmplitude = pAction->dAmplitude * dCompleteness * (bVertical ? dMetricX : dMetricY);

				int nResult = Core::COM::COM_TransformWave( m_oSources.Get( pAction->SourceId ), dPhase, dPhaseOffset, dAmplitude, bVertical );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_Ripple(ImageStudio::Serialize::Effects::TransformRipple* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.0001 ) 
					return TRUE;
				
				double dOmega  = pAction->dOmega;
				double dOffset = pAction->dOffset * dCompleteness + pAction->dSpeed * pAction->GetRelativeTime( m_dTime );
				if( pAction->bRevers )
					dOffset *= -1;

				dx9::EffectRipple ripple;
				ripple.m_dOmega  = dOmega;
				ripple.m_dOffset = dOffset;

				int nRetCode = UpdateVideoEffect( &ripple, pAction->SourceId, dCompleteness, pAction->bUseVideoCard );
				if( nRetCode != 0 )
					return nRetCode;

				int nResult = Core::COM::COM_TransformRipple( m_oSources.Get( pAction->SourceId ), dOmega, dOffset, dCompleteness );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_RotateSpecial(ImageStudio::Serialize::Effects::TransformRotateSpecial* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.0001 ) 
					return TRUE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->nMetric, dMetricX, dMetricY ) )
					return FALSE;
				
				int nAngle = int(pAction->dAngle + 0.5) % 360;
				if( nAngle < 0 )
					nAngle += 360;


				if(      nAngle < 90  ) nAngle = 0;
				else if( nAngle < 180 ) nAngle = 90;
				else if( nAngle < 270 ) nAngle = 180;
				else if( nAngle < 360 ) nAngle = 270;

				double dLeft   = pAction->dLeft   * dMetricX;
				double dTop    = pAction->dTop    * dMetricY;
				double dWidth  = pAction->dWidth  * dMetricX;
				double dHeight = pAction->dHeight * dMetricY;

				BYTE* pSource = NULL;
				int nWidth  = 0;
				int nHeight = 0;

				int nStatus = Core::COM::COM_GetBGRAImageFromInterface( m_oSources.Get( pAction->SourceId ), pSource, nWidth, nHeight );
				if (nStatus != Core::c_nErrorNone)
					return FALSE;

				MediaCore::IAVSUncompressedVideoFrame* pSourceFrame = m_oSources.Get( pAction->SourceId );

				long lAspectX = 0;
				long lAspectY = 0;
				long lColorSpace = 0;
				pSourceFrame->get_AspectRatioX( &lAspectX );
				pSourceFrame->get_AspectRatioY( &lAspectY );
				pSourceFrame->get_ColorSpace( &lColorSpace );

				if( !lAspectX || !lAspectY )
				{
					lAspectX = 1;
					lAspectY = 1;
				}

				double dFactorX = nWidth * lAspectY / double(nHeight * lAspectX);
				double dFactorY = 1;

				long lResultWidth  = int(dWidth);
				long lResultHeight = int(dHeight);

				if( lResultWidth < 1 )
					lResultWidth = 1;

				if( lResultHeight < 1 )
					lResultHeight = 1;

				MediaCore::IAVSUncompressedVideoFrame* pResultFrame = NULL;
				::CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_INPROC, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pResultFrame );
				if( !pResultFrame )
					return FALSE;

				pResultFrame->raw_AllocateBuffer( lResultWidth * lResultHeight * 4 );
				
				BYTE* pResult = NULL;
				pResultFrame->get_Buffer( &pResult );
				if( !pResult )
				{
					pResultFrame->Release();
					return FALSE;
				}

				pResultFrame->put_Width( lResultWidth );
				pResultFrame->put_Height( lResultHeight );
				pResultFrame->put_ColorSpace( lColorSpace );
				pResultFrame->put_Stride( 0, lResultWidth * 4 );
				pResultFrame->put_AspectRatioX( lResultWidth );
				pResultFrame->put_AspectRatioY( lResultHeight );

				Ipp8u color[4] = {0, 0, 0, 255};
				IppiSize ResultSize = {lResultWidth, lResultHeight};

				ippiSet_8u_C4R( color, pResult, lResultWidth*4, ResultSize );

				double dResultCenterX = lResultWidth / 2.0 + dLeft;
				double dResultCenterY = lResultHeight / 2.0 + dTop;

				double dScale = pAction->dScale;

				Core::COM::COM_Rotate( pSourceFrame, pResultFrame, dResultCenterX, dResultCenterY, nAngle, dScale * dFactorX, dScale * dFactorY, 255, 1 ); 

				m_oSources.Set( 0, pResultFrame );

				return TRUE;
			}
			BOOL Process_Transform_Linear(ImageStudio::Serialize::Effects::TransformLinear* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.0001 ) 
					return TRUE;
				
				IUnknown* pSrcFrame = m_oSources.Get( pAction->SourceId );
				IUnknown* pDstFrame = 0;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pSrcFrame, pAction->nMetric, dMetricX, dMetricY ) )
					return FALSE;
				
				int nResult = Core::COM::COM_TransformLinear( 
					pSrcFrame, &pDstFrame, 
					dCompleteness,
					pAction->dAngle,
					pAction->dScaleX,
					pAction->dScaleY,
					pAction->dShiftX * dMetricX,
					pAction->dShiftY * dMetricY,
					pAction->nBackColor, pAction->nBackAlpha );

				if( Core::c_nErrorNone != nResult )
					return FALSE;

				m_oSources.Set( pAction->SourceId, pDstFrame );

				return TRUE;
			}
			BOOL Process_Transform_GlassTile(ImageStudio::Serialize::Effects::TransformGlassTile* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.0001 ) 
					return TRUE;

				
				IMediaFrame* pSrcFrame = m_oSources.Get( pAction->SourceId );
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pSrcFrame, pAction->nMetric, dMetricX, dMetricY ) )
					return FALSE;

				int nSize = int(pAction->dSize * dMetricX + 0.5);
				
				int nResult = Core::COM::COM_TransformGlassTile( pSrcFrame, nSize, dCompleteness );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Transform_BrokenGlass(ImageStudio::Serialize::Effects::TransformBrokenGlass* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.0001 ) 
					return TRUE;

				
				IMediaFrame* pSrcFrame = m_oSources.Get( pAction->SourceId );
				
				int nResult = Core::COM::COM_TransformBrokenGlass( pSrcFrame, pAction->nDebris, dCompleteness );

				return (Core::c_nErrorNone == nResult);
			}
			
			BOOL Process_Filter_Prewitt(ImageStudio::Serialize::Effects::FilterPrewitt* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_FilterPrewitt( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Filter_Scharr(ImageStudio::Serialize::Effects::FilterScharr* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_FilterScharr( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Filter_Sobel(ImageStudio::Serialize::Effects::FilterSobel* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_FilterSobel( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type, pAction->Extended );

				return (Core::c_nErrorNone == nResult);
			}

			BOOL Process_Filter_Roberts(ImageStudio::Serialize::Effects::FilterRoberts* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_FilterRoberts( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Filter_Laplace(ImageStudio::Serialize::Effects::FilterLaplace* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_FilterLaplace( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Filter_Hipass(ImageStudio::Serialize::Effects::FilterHipass* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_FilterHipass( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Filter_Lowpass(ImageStudio::Serialize::Effects::FilterLowpass* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_FilterLowpass( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Filter_Blur(ImageStudio::Serialize::Effects::FilterBlur* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_FilterBlur( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->Type );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Filter_Canny(ImageStudio::Serialize::Effects::FilterCanny* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_FilterCanny( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->LevelLow, pAction->LevelHigh );

				return (Core::c_nErrorNone == nResult);
			}
			
			BOOL Process_File_LoadImage(ImageStudio::Serialize::File::FileLoadImage* pAction)
			{
				if (!pAction)
					return FALSE;

				IUnknown* pResult = NULL;

				IO::CMetaFile* pMetaFile = NULL;

				Core::COM::COM_LoadImage( &pResult, pAction->SourcePath, pAction->nFrame, pAction->bVerFlip, &pMetaFile );

				if( pMetaFile )
					return m_oSources.SetMetafile( 0, pMetaFile );
				
				m_oSources.Set( 0, pResult );

				return m_oSources.IsValidSource( 0 );
			}

			BOOL Process_File_LoadRaw(ImageStudio::Serialize::File::FileLoadRaw* pAction)
			{
				if (!pAction)
					return FALSE;

				//создаем XML
				pAction->XMLToString();

				IUnknown* pResult = NULL;

				Core::COM::COM_LoadRaw( &pResult, pAction->SourcePath, pAction->bVerFlip, pAction->strSettingXML );
				
				m_oSources.Set( 0, pResult );

				return m_oSources.IsValidSource( 0 );
			}

			BOOL Process_File_LoadScreenshot(ImageStudio::Serialize::File::FileLoadScreenshot* pAction)
			{
				m_oSources.Clear( 0 );

				BOOL bReturnCode = FALSE;
				
				HDC hdcScreen     = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
				HDC hdcCompatible = CreateCompatibleDC(hdcScreen);

				// compute screen size
				int nWidth = GetDeviceCaps(hdcScreen, HORZRES);
				int nHeight = GetDeviceCaps(hdcScreen, VERTRES);

				// create a compatible bitmap for hdcScreen. 
				HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight); 
				if( hbmScreen )
				{
					// select the bitmaps into the compatible DC
					HGDIOBJ hObject = SelectObject(hdcCompatible, hbmScreen);

					// copy color data for the entire display into a bitmap that is selected into a compatible DC.
					if( BitBlt(hdcCompatible, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY) ) 
					{
						// create pixel array
						BYTE* pPixels = new BYTE[4*nWidth*nHeight];
						if( pPixels )
						{
							// compose bitmap info
							BITMAPINFO info;
							info.bmiHeader.biBitCount      = 32;
							info.bmiHeader.biClrImportant  = 0;
							info.bmiHeader.biClrUsed       = 0;
							info.bmiHeader.biCompression   = BI_RGB;
							info.bmiHeader.biHeight        = -nHeight;
							info.bmiHeader.biPlanes        = 1;
							info.bmiHeader.biSize          = sizeof(BITMAPINFO);
							info.bmiHeader.biSizeImage     = 4*nWidth*nHeight;
							info.bmiHeader.biWidth         = nWidth;
							info.bmiHeader.biXPelsPerMeter = 0;
							info.bmiHeader.biYPelsPerMeter = 0;

							// extract pixels from HBITMAP
							if( nHeight == ::GetDIBits(hdcCompatible, hbmScreen, 0, nHeight, pPixels, &info, DIB_RGB_COLORS))
							{
								IUnknown* pResult = NULL;

								bReturnCode = ImageStudioUtils::ByteArrayToMediaData( pPixels, nWidth, nHeight, &pResult, FALSE );

								m_oSources.Set( 0, pResult );
							}

							delete [] pPixels;
						}
					}

					// delete objects
					DeleteObject(hbmScreen);
				}
				
				DeleteDC(hdcCompatible);
				DeleteDC(hdcScreen);

				return bReturnCode;
			}
			BOOL Process_File_SaveAsBmp(ImageStudio::Serialize::File::FileSaveAsBmp* pAction)
			{
				if (!pAction)
					return FALSE;

				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( m_oSources.Get( pAction->SourceId ), FALSE))
					return FALSE;

				ImageStudio::IO::Bitmap oWriter;

				return oWriter.ToFile(&oImage, pAction->DestinationPath, pAction->Format);
			}
			BOOL Process_File_SaveAsGif(ImageStudio::Serialize::File::FileSaveAsGif* pAction)
			{
				if (!pAction)
					return FALSE;

				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( m_oSources.Get( pAction->SourceId ), FALSE))
					return FALSE;

				ImageStudio::IO::Gif oWriter;

				return oWriter.ToFile(&oImage, pAction->DestinationPath, pAction->Format);
			}
			BOOL Process_File_SaveAsJpeg(ImageStudio::Serialize::File::FileSaveAsJpeg* pAction)
			{
				if (!pAction)
					return FALSE;

				ULONG nQuality = max(0, min(100, pAction->Quality));

				EncoderParameters oSettings;

				oSettings.Count = 1;
				oSettings.Parameter[0].Guid = EncoderQuality;
				oSettings.Parameter[0].Type = EncoderParameterValueTypeLong;
				oSettings.Parameter[0].NumberOfValues = 1;
				oSettings.Parameter[0].Value = &nQuality;

				return SaveAsGdiplus( pAction->SourceId, pAction->DestinationPath, ImageCLSID::CLSID_Jpg, &oSettings, pAction->dpi,pAction->MetaDataXML );
			}

			BOOL Process_File_SaveAsPng(ImageStudio::Serialize::File::FileSaveAsPng* pAction)
			{
				if (!pAction)
					return FALSE;

				return SaveAsGdiplus( pAction->SourceId, pAction->DestinationPath, ImageCLSID::CLSID_Png, NULL, pAction->dpi/*,pAction->MetaDataXML*/);
			}
			BOOL Process_File_SaveAsTiff(ImageStudio::Serialize::File::FileSaveAsTiff* pAction)
			{
				if (!pAction)
					return FALSE;

				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( m_oSources.Get( pAction->SourceId ), FALSE))
					return FALSE;

				ImageStudio::IO::Tiff oWriter;

				return oWriter.ToFile(&oImage, pAction->DestinationPath, pAction->Format, pAction->Compression/*,pAction->MetaDataXML*/);
			}
			BOOL Process_File_SaveAsWmf(ImageStudio::Serialize::File::FileSaveAsWmf* pAction)
			{
				if (!pAction)
					return FALSE;

				int nResult = Core::COM::COM_SaveImageAsMetafile( m_oSources.Get( pAction->SourceId ), pAction->DestinationPath, FALSE );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_File_SaveAsEmf(ImageStudio::Serialize::File::FileSaveAsEmf* pAction)
			{
				if (!pAction)
					return FALSE;

				int nResult = Core::COM::COM_SaveImageAsMetafile( m_oSources.Get( pAction->SourceId ), pAction->DestinationPath, TRUE );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_File_SaveAsPcx(ImageStudio::Serialize::File::FileSaveAsPcx* pAction)
			{
				if (!pAction)
					return FALSE;

				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( m_oSources.Get( pAction->SourceId ), FALSE))
					return FALSE;

				oImage.RotateFlip( Gdiplus::RotateNoneFlipY );
				
				int nFileFormat = pAction->Format;
				if( ImageStudio::IO::c_nFormat_Uncompressed_24bpp == nFileFormat )
				{
					nFileFormat = ImageStudio::IO::c_nFormat_Uncompressed_32bpp;
				}

				ImageStudio::IO::Pcx oWriter;

				return oWriter.ToFile(&oImage, pAction->DestinationPath, nFileFormat);
			}
			BOOL Process_File_SaveAsTga(ImageStudio::Serialize::File::FileSaveAsTga* pAction)
			{
				if (!pAction)
					return FALSE;

				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( m_oSources.Get( pAction->SourceId ), FALSE))
					return FALSE;

				oImage.RotateFlip( Gdiplus::RotateNoneFlipY );
				
				int nFileFormat = pAction->Format;
				if( ImageStudio::IO::c_nFormat_Uncompressed_24bpp == nFileFormat )
				{
					nFileFormat = ImageStudio::IO::c_nFormat_Uncompressed_32bpp;
				}

				ImageStudio::IO::Tga oWriter;

				return oWriter.ToFile(&oImage, pAction->DestinationPath, nFileFormat);
			}
			BOOL Process_File_SaveAsRas(ImageStudio::Serialize::File::FileSaveAsRas* pAction)
			{
				if (!pAction)
					return FALSE;

				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( m_oSources.Get( pAction->SourceId ), FALSE))
					return FALSE;

				oImage.RotateFlip( Gdiplus::RotateNoneFlipY );
				
				int nFileFormat = pAction->Format;
				if( ImageStudio::IO::c_nFormat_Uncompressed_24bpp == nFileFormat )
				{
					nFileFormat = ImageStudio::IO::c_nFormat_Uncompressed_32bpp;
				}

				ImageStudio::IO::Ras oWriter;

				return oWriter.ToFile(&oImage, pAction->DestinationPath, nFileFormat);
			}
			BOOL Process_File_SaveAsIPod(ImageStudio::Serialize::File::FileSaveAsIPod* pAction)
			{
				if (!pAction)
					return FALSE;

				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( m_oSources.Get( pAction->SourceId ), FALSE))
					return FALSE;

				ImageStudio::IO::IPod oWriter;

				return oWriter.ToFile(&oImage, pAction->DestinationPath, pAction->Format);
			}
			BOOL Process_File_SaveAsPsd(ImageStudio::Serialize::File::FileSaveAsPsd* pAction)
			{
				if (!pAction)
					return FALSE;

				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( m_oSources.Get( pAction->SourceId ), FALSE))
					return FALSE;

				oImage.RotateFlip( Gdiplus::RotateNoneFlipY );
				
				int nFileFormat = pAction->Format;
				if( ImageStudio::IO::c_nFormat_Uncompressed_24bpp == nFileFormat )
				{
					nFileFormat = ImageStudio::IO::c_nFormat_Uncompressed_32bpp;
				}

				ImageStudio::IO::Psd oWriter;

				return oWriter.ToFile(&oImage, pAction->DestinationPath, nFileFormat);
			}
			BOOL Process_File_SaveAsJp2(ImageStudio::Serialize::File::FileSaveAsJp2* pAction)
			{
				if (!pAction)
					return FALSE;

				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( m_oSources.Get( pAction->SourceId ), FALSE))
					return FALSE;

				oImage.RotateFlip( Gdiplus::RotateNoneFlipY );
				
				int nFileFormat = pAction->nFormat;
				if( ImageStudio::IO::c_nFormat_Uncompressed_24bpp == nFileFormat )
				{
					nFileFormat = ImageStudio::IO::c_nFormat_Uncompressed_32bpp;
				}

				ImageStudio::IO::Jp2 oWriter;

				return oWriter.ToFile( &oImage, pAction->sDestinationPath, nFileFormat, pAction->bJp2, pAction->bLossLess, pAction->nQuality, pAction->nFileSize );
			}

			
			BOOL Process_Paint_SetPen(ImageStudio::Serialize::Paint::Common::SetPen* pAction)
			{
				if (!pAction)
					return FALSE;

				m_oPaintSettings.SetPen(pAction->Pen);

				return TRUE;
			}
			BOOL Process_Paint_SetBrush(ImageStudio::Serialize::Paint::Common::SetBrush* pAction)
			{
				if (!pAction)
					return FALSE;

				m_oPaintSettings.SetBrush(pAction->Brush, TRUE);

				return TRUE;
			}
			BOOL Process_Paint_SetFont(ImageStudio::Serialize::Paint::Common::SetFont* pAction)
			{
				if (!pAction)
					return FALSE;

				m_oPaintSettings.SetFont(pAction->Font);

				return TRUE;
			}
			BOOL Process_Paint_SetFormat(ImageStudio::Serialize::Paint::Common::SetFormat* pAction)
			{
				if (!pAction)
					return FALSE;

				m_oPaintSettings.SetFormat(pAction->Format);

				return TRUE;
			}
			BOOL Process_Paint_DrawLine(ImageStudio::Serialize::Paint::Common::DrawLine* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetPen(pAction->Pen);

				Painter::CPen& oPen = m_oPaintSettings.GetPen();
				oPen.ScaleAlpha( dCompleteness );
				if( pAction->Pen.IsUseMetric() )
					oPen.Size *= dMetricX;

				double dX1 = dMetricX * pAction->X1;
				double dY1 = dMetricY * pAction->Y1;
				double dX2 = dMetricX * pAction->X2;
				double dY2 = dMetricY * pAction->Y2;

				// отрисовываем объект
				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				int nResult = Core::GDIPLUS::GDIPLUS_DrawLine( &oRender, m_oPaintSettings, dX1, dY1, dX2, dY2);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawRectangle(ImageStudio::Serialize::Paint::Common::DrawRectangle* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.000001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetPen(pAction->Pen);
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);
				
				Painter::CPen& oPen = m_oPaintSettings.GetPen();
				oPen.ScaleAlpha( dCompleteness );
				if( pAction->Pen.IsUseMetric() )
					oPen.Size *= dMetricX;

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// отрисовываем объект
				int nResult = Core::c_nErrorNone;

				// подсчитываем метрику - в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dLeft   = pAction->Left   * dMetricX;
				double dTop    = pAction->Top    * dMetricY;
				double dRight  = pAction->Right  * dMetricX;
				double dBottom = pAction->Bottom * dMetricY;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				if (pAction->Solid)
					nResult = Core::GDIPLUS::GDIPLUS_FillRectangle( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom);
				else
					nResult = Core::GDIPLUS::GDIPLUS_DrawRectangle( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawEllipse(ImageStudio::Serialize::Paint::Common::DrawEllipse* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetPen(pAction->Pen);
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CPen& oPen = m_oPaintSettings.GetPen();
				oPen.ScaleAlpha( dCompleteness );
				if( pAction->Pen.IsUseMetric() )
					oPen.Size *= dMetricX;

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// подсчитываем метрику - в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dLeft   = pAction->Left   * dMetricX;
				double dTop    = pAction->Top    * dMetricY;
				double dRight  = pAction->Right  * dMetricX;
				double dBottom = pAction->Bottom * dMetricY;

				// отрисовываем объект
				int nResult = Core::c_nErrorNone;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				if (pAction->Solid)
					nResult = Core::GDIPLUS::GDIPLUS_FillEllipse( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom);
				else
					nResult = Core::GDIPLUS::GDIPLUS_DrawEllipse( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawInvRectangle(ImageStudio::Serialize::Paint::Common::DrawInvRectangle* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// подсчитываем метрику - в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dLeft   = pAction->Left   * dMetricX;
				double dTop    = pAction->Top    * dMetricY;
				double dRight  = pAction->Right  * dMetricX;
				double dBottom = pAction->Bottom * dMetricY;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				// отрисовываем объект
				Gdiplus::RectF oRectImage(-1.0f, -1.0f, (float)pBitmap->GetWidth() + 2.0f, (float)pBitmap->GetHeight() + 2.0f); // чтобы не было полос от антиалиасинга - делаем rect чуть больше картинки

				int nResult = Core::GDIPLUS::GDIPLUS_FillInverseRectangle( &oRender, m_oPaintSettings, oRectImage, dLeft, dTop, dRight, dBottom);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawInvEllipse(ImageStudio::Serialize::Paint::Common::DrawInvEllipse* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// подсчитываем метрику - в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dLeft   = pAction->Left   * dMetricX;
				double dTop    = pAction->Top    * dMetricY;
				double dRight  = pAction->Right  * dMetricX;
				double dBottom = pAction->Bottom * dMetricY;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				Gdiplus::RectF oRectImage(-1.0f, -1.0f, (float)pBitmap->GetWidth() + 2.0f, (float)pBitmap->GetHeight() + 2.0f); // чтобы не было полос от антиалиасинга - делаем rect чуть больше картинки

				// отрисовываем объект
				int nResult = Core::GDIPLUS::GDIPLUS_FillInverseEllipse( &oRender, m_oPaintSettings, oRectImage, dLeft, dTop, dRight, dBottom);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawPie(ImageStudio::Serialize::Paint::Common::DrawPie* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetPen(pAction->Pen);
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CPen& oPen = m_oPaintSettings.GetPen();
				oPen.ScaleAlpha( dCompleteness );
				if( pAction->Pen.IsUseMetric() )
					oPen.Size *= dMetricX;

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// подсчитываем метрику - в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dLeft   = pAction->Left   * dMetricX;
				double dTop    = pAction->Top    * dMetricY;
				double dRight  = pAction->Right  * dMetricX;
				double dBottom = pAction->Bottom * dMetricY;
				double dStartAngle = pAction->StartAngle;
				double dSweepAngle = pAction->SweepAngle;

				// отрисовываем объект
				int nResult = Core::c_nErrorNone;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				if (pAction->Solid)
					nResult = Core::GDIPLUS::GDIPLUS_FillPie( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
				else
					nResult = Core::GDIPLUS::GDIPLUS_DrawPie( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);			
			}
			BOOL Process_Paint_DrawSector(ImageStudio::Serialize::Paint::Common::DrawSector* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetPen(pAction->Pen);
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CPen& oPen = m_oPaintSettings.GetPen();
				oPen.ScaleAlpha( dCompleteness );
				if( pAction->Pen.IsUseMetric() )
					oPen.Size *= dMetricX;

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// подсчитываем метрику - в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dLeft   = pAction->Left   * dMetricX;
				double dTop    = pAction->Top    * dMetricY;
				double dRight  = pAction->Right  * dMetricX;
				double dBottom = pAction->Bottom * dMetricY;

				// отрисовываем объект
				int nResult = Core::c_nErrorNone;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				if (pAction->Solid)
					nResult = Core::GDIPLUS::GDIPLUS_FillSector( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom, pAction->StartAngle, pAction->SweepAngle, pAction->RadiusRatio);
				else
					nResult = Core::GDIPLUS::GDIPLUS_DrawSector( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom, pAction->StartAngle, pAction->SweepAngle, pAction->RadiusRatio);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawSimplePie(ImageStudio::Serialize::Paint::Common::DrawSimplePie* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetPen(pAction->Pen);
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CPen& oPen = m_oPaintSettings.GetPen();
				oPen.ScaleAlpha( dCompleteness );
				if( pAction->Pen.IsUseMetric() )
					oPen.Size *= dMetricX;

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// подсчитываем метрику - в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dMetricDiagonal = 1.0; 
				
				if (fabs(dMetricX - 1.0) > 0.001 || fabs(dMetricY - 1.0) > 0.001)
					dMetricDiagonal = sqrt(dMetricX * dMetricX + dMetricY * dMetricY);

				double dLeft   = dMetricX*pAction->CenterX - dMetricDiagonal*pAction->Radius;
				double dTop    = dMetricY*pAction->CenterY - dMetricDiagonal*pAction->Radius;
				double dRight  = dMetricX*pAction->CenterX + dMetricDiagonal*pAction->Radius;
				double dBottom = dMetricY*pAction->CenterY + dMetricDiagonal*pAction->Radius;

				// отрисовываем объект
				int nResult = Core::c_nErrorNone;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				if (pAction->Solid)
					nResult = Core::GDIPLUS::GDIPLUS_FillPie( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom, pAction->StartAngle, pAction->SweepAngle);
				else
					nResult = Core::GDIPLUS::GDIPLUS_DrawPie( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom, pAction->StartAngle, pAction->SweepAngle);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawSimpleSector(ImageStudio::Serialize::Paint::Common::DrawSimpleSector* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetPen(pAction->Pen);
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CPen& oPen = m_oPaintSettings.GetPen();
				oPen.ScaleAlpha( dCompleteness );
				if( pAction->Pen.IsUseMetric() )
					oPen.Size *= dMetricX;

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// подсчитываем метрику - в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dMetricDiagonal = 1.0; 

				if (fabs(dMetricX - 1.0) > 0.001 || fabs(dMetricY - 1.0) > 0.001)
					dMetricDiagonal = sqrt(dMetricX * dMetricX + dMetricY * dMetricY);

				double dLeft   = dMetricX*pAction->CenterX - dMetricDiagonal*pAction->Radius1;
				double dTop    = dMetricY*pAction->CenterY - dMetricDiagonal*pAction->Radius1;
				double dRight  = dMetricX*pAction->CenterX + dMetricDiagonal*pAction->Radius1;
				double dBottom = dMetricY*pAction->CenterY + dMetricDiagonal*pAction->Radius1;
				double dRadius = pAction->Radius2 / max(0.0001, pAction->Radius1);

				// отрисовываем объект
				int nResult = Core::c_nErrorNone;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				if (pAction->Solid)
					nResult = Core::GDIPLUS::GDIPLUS_FillSector( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom, pAction->StartAngle, pAction->SweepAngle, dRadius);
				else
					nResult = Core::GDIPLUS::GDIPLUS_DrawSector( &oRender, m_oPaintSettings, dLeft, dTop, dRight, dBottom, pAction->StartAngle, pAction->SweepAngle, dRadius);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawPolyline(ImageStudio::Serialize::Paint::Common::DrawPolyline* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nPointsCount = pAction->Points.GetSize() / 2;
				if (nPointsCount <= 0)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetPen(pAction->Pen);

				Painter::CPen& oPen = m_oPaintSettings.GetPen();
				oPen.ScaleAlpha( dCompleteness );
				if( pAction->Pen.IsUseMetric() )
					oPen.Size *= dMetricX;

				CBuffer<Gdiplus::PointF> pPoints(nPointsCount);
				if( pPoints.IsEmpty() )
					return FALSE;

				if( Core::c_nErrorNone != Core::COM::COM_ComputePolyline( pAction->Points.GetData(), pPoints, nPointsCount, dMetricX, dMetricY ) )
					return FALSE;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				// отрисовываем объект
				int nResult = Core::GDIPLUS::GDIPLUS_DrawPolyline( &oRender, m_oPaintSettings, pPoints, nPointsCount );

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawPolygon(ImageStudio::Serialize::Paint::Common::DrawPolygon* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nPointsCount = pAction->Points.GetSize()/2;
				if (nPointsCount < 1)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetPen(pAction->Pen);
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CPen& oPen = m_oPaintSettings.GetPen();
				oPen.ScaleAlpha( dCompleteness );
				if( pAction->Pen.IsUseMetric() )
					oPen.Size *= dMetricX;

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				int nResult = Core::c_nErrorNone;

				CBuffer<Gdiplus::PointF> pPoints(nPointsCount);
				if( pPoints.IsEmpty() )
					return FALSE;

				if( Core::c_nErrorNone != Core::COM::COM_ComputePolyline( pAction->Points.GetData(), pPoints, nPointsCount, dMetricX, dMetricY ) )
					return FALSE;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				// отрисовываем объект
				if (pAction->Solid)
					nResult = Core::GDIPLUS::GDIPLUS_FillPolygon( &oRender, m_oPaintSettings, pPoints, nPointsCount );
				else
					nResult = Core::GDIPLUS::GDIPLUS_DrawPolygon( &oRender, m_oPaintSettings, pPoints, nPointsCount );

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawBorder(ImageStudio::Serialize::Paint::Common::DrawBorder* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// подсчитываем метрику - в зависимости от типа метрики рисуем объект с соответствующими координатами
				double dSize = pAction->Size * dCompleteness;

				// отрисовываем объект
				int nResult = Core::COM::COM_DrawBorder( m_oSources.Get( pAction->SourceId ), m_oPaintSettings, pAction->Type, dSize, dMetricX, dMetricY);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawMovObject(ImageStudio::Serialize::Paint::Common::DrawMovObject* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				double dImageWidth = 0;
				double dImageHeight = 0;
				GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), 2, dImageWidth, dImageHeight );

				int nDivisions = max(1, pAction->Divisions);
				int nTileSizeX = int(dImageWidth ) / nDivisions;
				int nTileSizeY = int(dImageHeight) / nDivisions;

				// производим композицию с цветом
				return (Core::c_nErrorNone == Core::COM::COM_DrawMovObject( m_oSources.Get( pAction->SourceId ), pAction->Color, pAction->Alpha, nTileSizeX, nTileSizeY, pAction->Type, dCompleteness, NULL));
			}
			BOOL Process_Paint_DrawCredits(ImageStudio::Serialize::Paint::Common::DrawCredits* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetFont(pAction->Font);
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);
				
				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				//oBrush.ScaleAlpha1( dCompleteness );
				//oBrush.ScaleAlpha2( dCompleteness );
				//oBrush.ScaleTextureAlpha( dCompleteness );

				// подсчитываем метрику - в зависимости от типа метрики рисуем объект соответствующего размера
				Painter::CFont& oFont = m_oPaintSettings.GetFont();
				double dFontSize = oFont.GetFontSize() * dMetricX;
				oFont.SetFontSize( dFontSize );

				double dImageWidth  = 0;
				double dImageHeight = 0;
				GetMetricMultiplier( pBitmap, 2, dImageWidth, dImageHeight );

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				// отрисовываем текст
				int nResult = Core::GDIPLUS::GDIPLUS_DrawCredits( &oRender, m_oPaintSettings, pAction->Text, 0, 0, float(dImageWidth), float(dImageHeight), dCompleteness, pAction->Type);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawTextSimple(ImageStudio::Serialize::Paint::Common::DrawTextSimple* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetFont(pAction->Font);
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// в зависимости от типа метрики рисуем объект соответствующего размера
				Painter::CFont& oFont = m_oPaintSettings.GetFont();
				double dFontSize = oFont.GetFontSize() * dMetricX;
				oFont.SetFontSize( dFontSize );

				double dLeft   = dMetricX * pAction->Left;
				double dTop    = dMetricY * pAction->Top;
				double dMargin = dMetricX * pAction->nMargin;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				int nResult = Core::c_nErrorNone;

				// сообственно отрисовка
				if ( !pAction->bAlign )
					nResult = Core::GDIPLUS::GDIPLUS_DrawText( &oRender, m_oPaintSettings, pAction->Text, dLeft, dTop);
				else
				{
					int nWidth  = pBitmap->GetWidth();
					int nHeight = pBitmap->GetHeight();

					float fLeft       = 0.0; 
					float fTop        = 0.0;
					float fTextWidth  = 0.0;
					float fTextHeight = 0.0;

					Gdiplus::RectF oRect;
					BSTR bsText = pAction->Text.AllocSysString();
					
					m_oPaintSettings.GetBrush().GetTextRect(bsText, &oRender, m_oPaintSettings.GetFont().GetFont(),  0, 0, &oRect);

					::SysFreeString(bsText);

					fTextWidth  = oRect.GetRight();
					fTextHeight = oRect.GetBottom();

					switch (pAction->nAlignType)
					{
					case 0: fLeft = float(dMargin);                              fTop = float(dMargin);                                break;
					case 1: fLeft = float(nWidth) / 2  - fTextWidth / 2;         fTop = float(dMargin);                                break;
					case 2: fLeft = float(nWidth) - fTextWidth - float(dMargin); fTop = float(dMargin);                                break;
					case 3: fLeft = float(dMargin);                              fTop = float(nHeight) / 2  - fTextHeight / 2;         break;
					case 4: fLeft = float(nWidth) / 2  - fTextWidth / 2;         fTop = float(nHeight) / 2  - fTextHeight / 2;         break;
					case 5: fLeft = float(nWidth) - fTextWidth - float(dMargin); fTop = float(nHeight) / 2  - fTextHeight / 2;         break;
					case 6: fLeft = float(dMargin);                              fTop = float(nHeight) - fTextHeight - float(dMargin); break;
					case 7: fLeft = float(nWidth) / 2  - fTextWidth / 2;         fTop = float(nHeight) - fTextHeight - float(dMargin); break;
					case 8: fLeft = float(nWidth) - fTextWidth - float(dMargin); fTop = float(nHeight) - fTextHeight - float(dMargin); break;
					}

					nResult = Core::GDIPLUS::GDIPLUS_DrawText( &oRender, m_oPaintSettings, pAction->Text, fLeft, fTop);
				}

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawTextInRect(ImageStudio::Serialize::Paint::Common::DrawTextInRect* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->Metric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();
				m_oPaintSettings.SetFont(pAction->Font);
				m_oPaintSettings.SetBrush(pAction->Brush, FALSE);

				Painter::CBrush& oBrush = m_oPaintSettings.GetBrush();
				oBrush.ScaleAlpha1( dCompleteness );
				oBrush.ScaleAlpha2( dCompleteness );
				oBrush.ScaleTextureAlpha( dCompleteness );

				// в зависимости от типа метрики рисуем объект соответствующего размера
				Painter::CFont& oFont = m_oPaintSettings.GetFont();
				double dFontSize = oFont.GetFontSize() * dMetricX;
				oFont.SetFontSize( dFontSize );

				// отрисовываем текст
				int nResult = Core::c_nErrorNone;
				
				double dLeft   = pAction->Left   * dMetricX;
				double dTop    = pAction->Top    * dMetricY;
				double dRight  = pAction->Right  * dMetricX;
				double dBottom = pAction->Bottom * dMetricY;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				if (pAction->Scale)
					nResult = Core::GDIPLUS::GDIPLUS_DrawTextScale( &oRender, m_oPaintSettings, pAction->Text, dLeft, dTop, dRight, dBottom);
				else
					nResult = Core::GDIPLUS::GDIPLUS_DrawText( &oRender, m_oPaintSettings, pAction->Text, dLeft, dTop, dRight, dBottom);

				// восстанавливаем старое состояние отрисовщика
				m_oPaintSettings.RestoreState();

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawImageFromFile(ImageStudio::Serialize::Paint::Common::DrawImageFromFile* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				IUnknown* pInterfaceImage = NULL;

				// отрабатываем m_oCache - если картинку уже загружали ранее - используем текущий интерфейс
				int nSessionID = pAction->GetSessionID();
				ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
				if (NULL != pCacheItem && ImageStudio::Caching::c_nTypeInterface == pCacheItem->GetType())
				{
					pInterfaceImage = ((ImageStudio::Caching::CCacheItemInterface*)pCacheItem)->GetInterface();

					//if (NULL != pInterfaceImage)
					//{
					//	ULONG nRef = pInterfaceImage->AddRef();
					//	if( nRef > 10 )
					//	{
					//		int a = 0;
					//	}
					//}
				}
				else
				{
					if( m_pFileCache )
					{
						m_pFileCache->raw_LoadImageW( _bstr_t( pAction->FilePath ), -1, -1, &pInterfaceImage );

						if( pInterfaceImage )
						//if( Core::c_nErrorNone == Core::COM::COM_LoadImage( &pInterfaceImage, pAction->FilePath ) )
						{
							ImageStudio::Caching::CCacheItemInterface* pCacheInterface = new ImageStudio::Caching::CCacheItemInterface(pInterfaceImage);
							m_oCache.Add(nSessionID, pCacheInterface);
							pInterfaceImage->Release();
						}

					}
					else
					{
						pInterfaceImage = m_oImageCache.GetImage( pAction->FilePath );
						if( pInterfaceImage )
						//if( Core::c_nErrorNone == Core::COM::COM_LoadImage( &pInterfaceImage, pAction->FilePath ) )
						{
							ImageStudio::Caching::CCacheItemInterface* pCacheInterface = new ImageStudio::Caching::CCacheItemInterface(pInterfaceImage);
							m_oCache.Add(nSessionID, pCacheInterface);
						}
					}
				}

				if( !pInterfaceImage )
					return FALSE;

				double dMetricX = 1, dMetricY = 1;
				GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY );

				double dLeft    = pAction->Left   * dMetricX;
				double dRight   = pAction->Right  * dMetricX;
				double dTop     = pAction->Top    * dMetricY;
				double dBottom  = pAction->Bottom * dMetricY;

				if ((0 != pAction->dWidthMetric) && (0 != pAction->dHeightMetric) && (NULL != m_oSources.Get(0)))
				{
					long lSrcWidth = m_oSources.Get(0)->Width;
					long lSrcHeight = m_oSources.Get(0)->Height;

					dLeft	= (dLeft	/ pAction->dWidthMetric)  * lSrcWidth;
					dRight	= (dRight	/ pAction->dWidthMetric)  * lSrcWidth;
					dTop	= (dTop		/ pAction->dHeightMetric) * lSrcHeight;
					dBottom = (dBottom	/ pAction->dHeightMetric) * lSrcHeight;
				}
				
				int nResult = Core::COM::COM_DrawImageFromFile( m_oSources.Get( pAction->SourceId ), pInterfaceImage, pAction->BackColor, pAction->nDrawImageScaleType, pAction->nDrawImageScaleBackColor, pAction->Alpha, dCompleteness, dLeft, dTop, dRight, dBottom, pAction->Angle, pAction->bAlign, pAction->nAlignType, pAction->nMarginX, pAction->nMarginY, pAction->nSizeType, pAction->nWidth, pAction->nHeight );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawImageFromFileEx(ImageStudio::Serialize::Paint::Common::DrawImageFromFileEx* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				IUnknown* pSrcInterface = NULL;
				IUnknown* pDstInterface = NULL;

				if( m_oSources.IsValidSource( pAction->nSrcBuffer ) )
				{
					pSrcInterface = m_oSources.Get( pAction->nSrcBuffer );
				}
				else
				{
					if( m_pMemoryStorage && pAction->sObjectId.GetLength() )
					{
						m_pMemoryStorage->Get( _bstr_t( pAction->sObjectId ), &pSrcInterface );
					}
					else
					{
						// отрабатываем m_oCache - если картинку уже загружали ранее - используем текущий интерфейс
						int nSessionID = pAction->GetSessionID();
						ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
						if (NULL != pCacheItem &&  ImageStudio::Caching::c_nTypeInterface == pCacheItem->GetType())
						{
							pSrcInterface = ((ImageStudio::Caching::CCacheItemInterface*)pCacheItem)->GetInterface();
						}
						else
						{
							if( m_pFileCache )
							{
								m_pFileCache->raw_LoadImageW( _bstr_t( pAction->sFilePath ), -1, -1, &pSrcInterface );

								if( pSrcInterface )
								{
									ImageStudio::Caching::CCacheItemInterface* pCacheInterface = new ImageStudio::Caching::CCacheItemInterface(pSrcInterface);
									m_oCache.Add(nSessionID, pCacheInterface);
									pSrcInterface->Release();
								}

							}
							else
							{
								pSrcInterface = m_oImageCache.GetImage( pAction->sFilePath );
								if( pSrcInterface )
								{
									ImageStudio::Caching::CCacheItemInterface* pCacheInterface = new ImageStudio::Caching::CCacheItemInterface(pSrcInterface);
									m_oCache.Add(nSessionID, pCacheInterface);
								}
							}
						}
					}
				}

				if( !pSrcInterface )
					return FALSE;

				pDstInterface = m_oSources.Get( pAction->SourceId );
				if( !pDstInterface )
					return FALSE;

				double dSrcMetricX, dSrcMetricY;
				double dDstMetricX, dDstMetricY;

				GetMetricMultiplier( pSrcInterface, pAction->nSrcMetric, dSrcMetricX, dSrcMetricY );
				GetMetricMultiplier( pDstInterface, pAction->nDstMetric, dDstMetricX, dDstMetricY );

				double dSrcX      = dSrcMetricX * pAction->dSrcX;
				double dSrcY      = dSrcMetricY * pAction->dSrcY;
				double dSrcWidth  = dSrcMetricX * pAction->dSrcWidth;
				double dSrcHeight = dSrcMetricY * pAction->dSrcHeight;

				double dDstX      = dDstMetricX * pAction->dDstX;
				double dDstY      = dDstMetricY * pAction->dDstY;
				double dDstWidth  = dDstMetricX * pAction->dDstWidth;
				double dDstHeight = dDstMetricY * pAction->dDstHeight;

				int nResult = Core::COM::COM_DrawImageFromFileEx( pSrcInterface, pDstInterface, dSrcX, dSrcY, dSrcWidth, dSrcHeight, dDstX, dDstY, dDstWidth, dDstHeight, pAction->nSrcColorKey, pAction->nScaleType, pAction->dCropScale, pAction->dAngle, pAction->dAlpha );
				
				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawImageFromBuffer(ImageStudio::Serialize::Paint::Common::DrawImageFromBuffer* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::c_nErrorNone;

				if ( m_oSources.IsValidSource( pAction->Buffer ) )
				{
					// в зависимости от типа метрики рисуем объект с соответствующими координатами
					double dMetricX = 1, dMetricY = 1;
					if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY ) )
						return FALSE;

					double dLeft    = pAction->Left   * dMetricX;
					double dRight   = pAction->Right  * dMetricX;
					double dTop     = pAction->Top    * dMetricY;
					double dBottom  = pAction->Bottom * dMetricY;

					nResult = Core::COM::COM_DrawImageFromFile( m_oSources.Get( pAction->SourceId ), m_oSources.Get( pAction->Buffer ), -1, pAction->nDrawImageScaleType, pAction->nDrawImageScaleBackColor, pAction->Alpha, dCompleteness, dLeft, dTop, dRight, dBottom, pAction->Angle, pAction->bAlign, pAction->nAlignType, pAction->nMarginX, pAction->nMarginY, pAction->nSizeType, pAction->nWidth, pAction->nHeight );
				}
	
				return (Core::c_nErrorNone == nResult);
			}	
			BOOL Process_Paint_DrawTimer(ImageStudio::Serialize::Paint::Common::DrawTimer* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->m_nMetric, dMetricX, dMetricY ) )
					return FALSE;

				double dAbsTime = pAction->GetRelativeTime( m_dTime );
				double dTimeLength = pAction->GetTimeLength( m_dTime );
				double dAbsCompleteness = dTimeLength > 0 ? dAbsTime / dTimeLength : 1;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();

				m_oPaintSettings.SetFont  ( pAction->m_oFont );
				m_oPaintSettings.SetBrush ( pAction->m_oBrush, FALSE );
				m_oPaintSettings.SetShadow( pAction->m_oShadow );
				m_oPaintSettings.SetEdge  ( pAction->m_oEdge );

				// вычисляем время
				__int64 nMS1 = pAction->m_oHours.m_nStart * 3600000 + pAction->m_oMinutes.m_nStart * 60000 + pAction->m_oSeconds.m_nStart * 1000 + pAction->m_oMilliseconds.m_nStart;
				__int64 nMS2 = pAction->m_oHours.m_nEnd   * 3600000 + pAction->m_oMinutes.m_nEnd   * 60000 + pAction->m_oSeconds.m_nEnd   * 1000 + pAction->m_oMilliseconds.m_nEnd;
				
				if( pAction->m_bUnlimited )
				{
					nMS2 = nMS1 + __int64( dAbsTime );
				}

				__int64 nMSResult = nMS2 - nMS1;
				
				nMSResult = __int64(nMSResult * (pAction->m_bUnlimited ? 1 : (pAction->m_nTimerType ? dAbsCompleteness : dCompleteness)) + nMS1 + 0.5);

				int nHours   = int(nMSResult / 3600000); nMSResult %= 3600000;
				int nMinutes = int(nMSResult / 60000);   nMSResult %= 60000;
				int nSeconds = int(nMSResult / 1000);    nMSResult %= 1000;
				int nMillis  = int(nMSResult);

				CString sTime = _T("");
				pAction->m_oHours       .AddToString( sTime, nHours );
				pAction->m_oMinutes     .AddToString( sTime, nMinutes );
				pAction->m_oSeconds     .AddToString( sTime, nSeconds );
				pAction->m_oMilliseconds.AddToString( sTime, nMillis );
				
				// отрисовка текста
				BSTR text = sTime.AllocSysString();

				ImageStudio::Paint::Text::CTextPainter2 oTextRender;

				// в зависимости от типа метрики рисуем объект соответствующего размера
				Painter::CFont& oFont = m_oPaintSettings.GetFont();
				double dNewFontSize = oFont.Size * dMetricX;
				oFont.SetFontSize(dNewFontSize);

				Gdiplus::Font* pFont = m_oPaintSettings.GetFont().GetFont();

				// инициализируем отрисовщик текста (в соответствие с фонтом и строкой, которую будем отрисовывать)
				double dLeft    = dMetricX * pAction->m_dLeft;
				double dTop     = dMetricY * pAction->m_dTop;
				double dRight   = dMetricX * pAction->m_dRight;
				double dBottom  = dMetricY * pAction->m_dBottom;

				double dImageWidth  = 0;
				double dImageHeight = 0;
				GetMetricMultiplier( pBitmap, 2, dImageWidth, dImageHeight );

				Gdiplus::RectF oLayoutRect( float(dLeft), float(dTop), float(dRight - dLeft), float(dBottom - dTop) );
				oLayoutRect.Inflate( 20000, 20000 );

				::srand( 1000 );
				
				oTextRender.CreateText( text, *pFont, oLayoutRect, 1, 1 );
				oTextRender.CreateEffects( pAction->m_nAnimationID[0], pAction->m_nAnimationID[2], pAction->m_nTypeEffects, FALSE );
				
				oTextRender.m_fAngle = float(pAction->m_oFont.IsValidAngle() ? pAction->m_oFont.GetAngle() : 0);

				Painter::CEdgeText& oEdge = m_oPaintSettings.GetEdge();
				double dNewDistance = oEdge.Dist * dMetricX;
				oEdge.SetDistance( dNewDistance );

				Painter::CShadow& oShadow = m_oPaintSettings.GetShadow();
				double dNewDistanceX = oShadow.DistanceX * dMetricX;
				oShadow.SetDistanceX( dNewDistanceX );

				double dNewDistanceY = oShadow.DistanceY * dMetricY;
				oShadow.SetDistanceY( dNewDistanceY );

				double dNewBlurSize = oShadow.BlurSize * dMetricX;
				if( dNewBlurSize > 100 ) 
					dNewBlurSize = 100;
				oShadow.SetBlurSize( dNewBlurSize );

				Painter::CBrush& oFillBrush = m_oPaintSettings.GetBrush();
				// получаем текстурный браш, если он указан
				IUnknown* pTextureInterface = m_oImageCache.GetImage( oFillBrush.TexturePath );
				if( pTextureInterface )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					if( Core::c_nErrorNone == Core::COM::COM_GetBGRAImageFromInterface( pTextureInterface, pBGRA, nWidth, nHeight ) )
					{
						Painter::CTexture* pTexture = new Painter::CTexture();
						if( pTexture )
						{
							Gdiplus::Rect oImageRect( 0, 0, nWidth, nHeight );

							ImageStudio::Serialize::Paint::Structures::Brush& brush = pAction->m_oBrush;
							if( brush.GetRectable() )
							{
								Gdiplus::RectF oRect = brush.GetRect();
								Gdiplus::Rect oBrushRect( 
									int(oRect.X * nWidth + 0.5),
									int(oRect.Y * nHeight + 0.5),
									int(oRect.Width * nWidth + 0.5),
									int(oRect.Height * nHeight + 0.5) );

								if( oBrushRect.Intersect( oImageRect ) )
								{
									pBGRA += (oBrushRect.Y * nWidth + oBrushRect.X) * 4;
									oImageRect.Width  = oBrushRect.Width;
									oImageRect.Height = oBrushRect.Height;
								}
							}

							pTexture->Path          = oFillBrush.TexturePath.MakeLower();
							pTexture->Bitmap        = NULL;
							pTexture->BitmapData    = pBGRA;
							pTexture->bFlipVertical = FALSE;
							pTexture->bAttach       = TRUE;
							pTexture->nWidth        = oImageRect.Width;
							pTexture->nHeight       = oImageRect.Height;
							pTexture->nStride       = nWidth*4;

							oTextRender.m_oTextureManager.Add( pTexture );
							oTextRender.pImage = pTextureInterface;
							pTextureInterface->AddRef();
						}
					}
				}

				::SysFreeString( text );

				oTextRender.SetFillBrush( oFillBrush );
				oTextRender.SetEdge( oEdge );
				oTextRender.SetShadow( oShadow );

				oTextRender.m_nFillMode = pAction->m_nFillMode;
				
				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;
				
				oTextRender.Draw( dCompleteness, oRender );

				m_oPaintSettings.RestoreState();

				return TRUE;
			}	
			BOOL Process_Paint_DrawImageFromStorage(ImageStudio::Serialize::Paint::Common::DrawImageFromStorage* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				if( !m_pMemoryStorage )
					return TRUE;

				IUnknown* pInterfaceImage = NULL;
				_bstr_t id( pAction->ObjectId );

				m_pMemoryStorage->Get( id, &pInterfaceImage );
				if( !pInterfaceImage )
					return TRUE;

				double dMetricX = 1, dMetricY = 1;
				GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY );

				double dLeft    = pAction->Left   * dMetricX;
				double dRight   = pAction->Right  * dMetricX;
				double dTop     = pAction->Top    * dMetricY;
				double dBottom  = pAction->Bottom * dMetricY;
				
				int nResult = Core::COM::COM_DrawImageFromFile( m_oSources.Get( pAction->SourceId ), pInterfaceImage, pAction->BackColor, pAction->nDrawImageScaleType, pAction->nDrawImageScaleBackColor, pAction->Alpha, dCompleteness, dLeft, dTop, dRight, dBottom, pAction->Angle, pAction->bAlign, pAction->nAlignType, pAction->nMarginX, pAction->nMarginY, pAction->nSizeType, pAction->nWidth, pAction->nHeight );

				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawFilm(ImageStudio::Serialize::Paint::Common::DrawFilm* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				int nResult = Core::COM::COM_DrawFilm( m_oSources.Get( pAction->SourceId ), dCompleteness, pAction->GetRelativeTime(m_dTime), pAction->m_nSpeed, pAction->m_dWideFactor, pAction->m_bVertical, pAction->m_bDrawBorder, pAction->m_nBkColor, pAction->m_nFilmMidColor, pAction->m_nFilmBorderColor, pAction->m_nBlendcolor, pAction->m_bZoom, pAction->m_nAlpha, pAction->m_bMoveDivider, pAction->m_dSpeedFactor );

				return (Core::c_nErrorNone == nResult);
			}

			BOOL Process_Paint_DrawPuzzle(ImageStudio::Serialize::Paint::Common::DrawPuzzle* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;


				int nSessionID = pAction->GetSessionID();
				ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
				if (NULL == pCacheItem || ImageStudio::Caching::c_nTypePointer != pCacheItem->GetType())
				{
					ImageStudio::Paint::Effects::Puzzle* pEffect = new ImageStudio::Paint::Effects::Puzzle( );
			
					double dMetricX, dMetricY;
					if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->m_nMetric, dMetricX, dMetricY ) )
					{
						delete pEffect;
						return FALSE;
					}

					double dSide = pAction->m_dSideFactor * dMetricX;
					pEffect->Create( *pAction, dSide, GetBaseRandom( pAction->BaseRandom ) );

					m_oCache.Add(nSessionID, pEffect);

					pCacheItem = pEffect;
				}

				ImageStudio::Paint::Effects::Puzzle* pPainter = (ImageStudio::Paint::Effects::Puzzle*)pCacheItem;

				// отрисовываем текст
				int nResult = Core::COM::COM_DrawPuzzle( m_oSources.Get( pAction->SourceId ), pPainter, dCompleteness, pAction->m_nRow, pAction->m_nCol, pAction->m_dSideFactor, pAction->m_nType);
				
				return (Core::c_nErrorNone == nResult);
			}
			BOOL Process_Paint_DrawFog(ImageStudio::Serialize::Paint::Common::DrawFog* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if( dCompleteness < 0.000001 ) 
					return TRUE;

				double dAlpha = pAction->Alpha / 255.0 * dCompleteness;
	
				int nResult = Core::COM::COM_DrawFog( m_oSources.Get( pAction->SourceId ), pAction->Color, dAlpha );
				
				return Core::c_nErrorNone == nResult;
			}
			BOOL Process_Paint_DrawPath(ImageStudio::Serialize::Paint::Common::DrawGraphicPath* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.000001)
					return TRUE;

				BYTE* pBGRA = NULL;
				int nWidth = 0;
				int nHeight = 0;

				// вытаскиваем из интерфейса указатель на память в формате BGRA
				int nStatus = ImageStudio::Transforms::Core::COM::COM_GetBGRAImageFromInterface( m_oSources.Get( pAction->SourceId ), pBGRA, nWidth, nHeight);
				if (nStatus != Core::c_nErrorNone)
					return FALSE;

				return pAction->Draw(m_oSources.Get( pAction->SourceId ));
			}
			BOOL Process_Paint_AutoShape(ImageStudio::Serialize::Paint::Common::DrawAutoShape* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.000001)
					return TRUE;

				BYTE* pBGRA = NULL;
				int nWidth = 0;
				int nHeight = 0;

				// вытаскиваем из интерфейса указатель на память в формате BGRA
				int nStatus = ImageStudio::Transforms::Core::COM::COM_GetBGRAImageFromInterface( m_oSources.Get( pAction->SourceId ), pBGRA, nWidth, nHeight);
				if (nStatus != Core::c_nErrorNone)
					return FALSE;

				return pAction->Draw(m_oSources.Get( pAction->SourceId ), dCompleteness);
			}
			BOOL Process_Paint_DrawTextEx(ImageStudio::Serialize::Paint::Common::DrawTextEx* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.000001)
					return TRUE;

				BYTE* pBGRA = NULL;
				int nWidth = 0;
				int nHeight = 0;

				// вытаскиваем из интерфейса указатель на память в формате BGRA
				int nStatus = ImageStudio::Transforms::Core::COM::COM_GetBGRAImageFromInterface( m_oSources.Get( pAction->SourceId ), pBGRA, nWidth, nHeight);
				if (nStatus != Core::c_nErrorNone)
					return FALSE;

				return pAction->Draw(m_oSources.Get( pAction->SourceId ), 1.0);
			}

			BOOL Process_Paint_DrawCountdown(ImageStudio::Serialize::Paint::Common::DrawCountdown* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness < 0.0001)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->m_nMetric, dMetricX, dMetricY ) )
					return FALSE;

				// подготавливаем отрисовщик
				m_oPaintSettings.SaveState();

				m_oPaintSettings.SetFont  ( pAction->m_oFont );
				m_oPaintSettings.SetBrush ( pAction->m_oBrush, FALSE );
				m_oPaintSettings.SetShadow( pAction->m_oShadow );
				m_oPaintSettings.SetEdge  ( pAction->m_oEdge );

				// вычисляем счётчик
				double dAbsTime = pAction->GetRelativeTime( m_dTime );
				double dTimeLength = pAction->GetTimeLength( m_dTime );
				double dAbsCompleteness = dTimeLength > 0 ? dAbsTime / dTimeLength : 1;

				int nCurNumber = Round(pAction->m_nStartNumber + dAbsCompleteness * (pAction->m_nFinishNumber - pAction->m_nStartNumber));
				
				CString sNumber = _T("");
				sNumber.Format( _T("%d"), nCurNumber );

				// отрисовка текста
				BSTR text = sNumber.AllocSysString();

				ImageStudio::Paint::Text::CTextPainter2 oTextRender;

				// в зависимости от типа метрики рисуем объект соответствующего размера
				Painter::CFont& oFont = m_oPaintSettings.GetFont();
				double dNewFontSize = oFont.Size * dMetricX;
				oFont.SetFontSize(dNewFontSize);

				Gdiplus::Font* pFont = m_oPaintSettings.GetFont().GetFont();

				// инициализируем отрисовщик текста (в соответствие с фонтом и строкой, которую будем отрисовывать)
				double dLeft    = dMetricX * pAction->m_dLeft;
				double dTop     = dMetricY * pAction->m_dTop;
				double dRight   = dMetricX * pAction->m_dRight;
				double dBottom  = dMetricY * pAction->m_dBottom;

				double dImageWidth  = 0;
				double dImageHeight = 0;
				GetMetricMultiplier( pBitmap, 2, dImageWidth, dImageHeight );

				Gdiplus::RectF oLayoutRect( float(dLeft), float(dTop), float(dRight - dLeft), float(dBottom - dTop) );
				oLayoutRect.Inflate( 20000, 20000 );

				::srand( 1000 );
				
				oTextRender.CreateText( text, *pFont, oLayoutRect, 1, 1 );
				oTextRender.CreateEffects( pAction->m_nAnimationID[0], pAction->m_nAnimationID[2], pAction->m_nTypeEffects, FALSE );
				
				oTextRender.m_fAngle = float(pAction->m_oFont.IsValidAngle() ? pAction->m_oFont.GetAngle() : 0);

				Painter::CEdgeText& oEdge = m_oPaintSettings.GetEdge();
				double dNewDistance = oEdge.Dist * dMetricX;
				oEdge.SetDistance( dNewDistance );

				Painter::CShadow& oShadow = m_oPaintSettings.GetShadow();
				double dNewDistanceX = oShadow.DistanceX * dMetricX;
				oShadow.SetDistanceX( dNewDistanceX );

				double dNewDistanceY = oShadow.DistanceY * dMetricY;
				oShadow.SetDistanceY( dNewDistanceY );

				double dNewBlurSize = oShadow.BlurSize * dMetricX;
				if( dNewBlurSize > 100 ) 
					dNewBlurSize = 100;
				oShadow.SetBlurSize( dNewBlurSize );

				Painter::CBrush& oFillBrush = m_oPaintSettings.GetBrush();
				// получаем текстурный браш, если он указан
				IUnknown* pTextureInterface = m_oImageCache.GetImage( oFillBrush.TexturePath );
				if( pTextureInterface )
				{
					BYTE* pBGRA = NULL;
					int nWidth = 0;
					int nHeight = 0;

					if( Core::c_nErrorNone == Core::COM::COM_GetBGRAImageFromInterface( pTextureInterface, pBGRA, nWidth, nHeight ) )
					{
						Painter::CTexture* pTexture = new Painter::CTexture();
						if( pTexture )
						{
							Gdiplus::Rect oImageRect( 0, 0, nWidth, nHeight );

							ImageStudio::Serialize::Paint::Structures::Brush& brush = pAction->m_oBrush;
							if( brush.GetRectable() )
							{
								Gdiplus::RectF oRect = brush.GetRect();
								Gdiplus::Rect oBrushRect( 
									int(oRect.X * nWidth + 0.5),
									int(oRect.Y * nHeight + 0.5),
									int(oRect.Width * nWidth + 0.5),
									int(oRect.Height * nHeight + 0.5) );

								if( oBrushRect.Intersect( oImageRect ) )
								{
									pBGRA += (oBrushRect.Y * nWidth + oBrushRect.X) * 4;
									oImageRect.Width  = oBrushRect.Width;
									oImageRect.Height = oBrushRect.Height;
								}
							}

							pTexture->Path          = oFillBrush.TexturePath.MakeLower();
							pTexture->Bitmap        = NULL;
							pTexture->BitmapData    = pBGRA;
							pTexture->bFlipVertical = FALSE;
							pTexture->bAttach       = TRUE;
							pTexture->nWidth        = oImageRect.Width;
							pTexture->nHeight       = oImageRect.Height;
							pTexture->nStride       = nWidth*4;

							oTextRender.m_oTextureManager.Add( pTexture );
							oTextRender.pImage = pTextureInterface;
							pTextureInterface->AddRef();
						}
					}
				}

				::SysFreeString( text );

				oTextRender.SetFillBrush( oFillBrush );
				oTextRender.SetEdge( oEdge );
				oTextRender.SetShadow( oShadow );

				oTextRender.m_nFillMode = pAction->m_nFillMode;
				
				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;
				
				oTextRender.Draw( dCompleteness, oRender );

				m_oPaintSettings.RestoreState();

				return TRUE;
			}	
			
			BOOL Process_Paint_TextStatic_SetBrush(ImageStudio::Serialize::Paint::TextStatic::SetBrush* pAction)
			{
				if (m_oImagePaintTextStatic.Transform_SetBrush(pAction, m_dTime))
				{
					m_oPaintSettings.SetBrush(pAction->Brush, TRUE);

					return TRUE;
				}

				return FALSE;
			}
			BOOL Process_Paint_TextStatic_SetFont(ImageStudio::Serialize::Paint::TextStatic::SetFont* pAction)
			{
				if (m_oImagePaintTextStatic.Transform_SetFont(pAction, m_dTime))
				{
					m_oPaintSettings.SetFont(pAction->Font);

					return TRUE;
				}

				return FALSE;
			}
			BOOL Process_Paint_TextStatic_SetFormat(ImageStudio::Serialize::Paint::TextStatic::SetFormat* pAction)
			{
				if (m_oImagePaintTextStatic.Transform_SetFormat(pAction, m_dTime))
				{
					m_oPaintSettings.SetFormat(pAction->Format);

					return TRUE;
				}

				return FALSE;
			}
			BOOL Process_Paint_TextStatic_SetShadow(ImageStudio::Serialize::Paint::TextStatic::SetShadow* pAction)
			{
				if (m_oImagePaintTextStatic.Transform_SetShadow(pAction, m_dTime))
				{
					m_oPaintSettings.SetShadow(pAction->Shadow);

					return TRUE;
				}

				return FALSE;
			}
			BOOL Process_Paint_TextStatic_SetEdge(ImageStudio::Serialize::Paint::TextStatic::SetEdge* pAction)
			{
				if (m_oImagePaintTextStatic.Transform_SetEdge(pAction, m_dTime))
				{
					m_oPaintSettings.SetEdge(pAction->Edge);

					return TRUE;
				}

				return FALSE;
			}
			BOOL Process_Paint_TextStatic_DrawInRect(ImageStudio::Serialize::Paint::TextStatic::DrawInRect* pAction)
			{
				IUnknown* pResult = NULL;
				IUnknown* pSource = m_oSources.Get( pAction->SourceId );
				if( !pSource )
					return TRUE;

				pSource->AddRef();
				m_oSources.Clear( 0 );

				if (!m_oImagePaintTextStatic.Transform_DrawInRect(pAction, m_dTime, pSource, pResult))
					return FALSE;

				m_oSources.Set( 0, pResult );

				return TRUE;
			}
			BOOL Process_Paint_TextStatic_DrawOnEllipse(ImageStudio::Serialize::Paint::TextStatic::DrawOnEllipse* pAction)
			{
				IUnknown* pResult = NULL;

				if (!m_oImagePaintTextStatic.Transform_DrawOnEllipse(pAction, m_dTime, m_oSources.Get( pAction->SourceId ), pResult))
					return FALSE;

				m_oSources.Set( 0, pResult );

				return TRUE;
			}
			BOOL Process_Paint_TextStatic_DrawOnPolyline(ImageStudio::Serialize::Paint::TextStatic::DrawOnPolyline* pAction)
			{
				IUnknown* pResult = NULL;

				if (!m_oImagePaintTextStatic.Transform_DrawOnPolyline(pAction, m_dTime, m_oSources.Get( pAction->SourceId ), pResult))
					return FALSE;

				m_oSources.Set( 0, pResult );

				return TRUE;
			}
		
			BOOL Process_Paint_TextDynamic_SetBrush(ImageStudio::Serialize::Paint::TextDynamic::SetBrush* pAction)
			{
				if (m_oImagePaintTextDynamic.Transform_SetBrush(pAction, m_dTime))
				{
					m_oPaintSettings.SetBrush(pAction->Brush, TRUE);

					return TRUE;
				}

				return FALSE;
			}
			BOOL Process_Paint_TextDynamic_SetFont(ImageStudio::Serialize::Paint::TextDynamic::SetFont* pAction)
			{
				if (m_oImagePaintTextDynamic.Transform_SetFont(pAction, m_dTime))
				{
					m_oPaintSettings.SetFont(pAction->Font);

					return TRUE;
				}

				return FALSE;
			}
			BOOL Process_Paint_TextDynamic_SetShadow(ImageStudio::Serialize::Paint::TextDynamic::SetShadow* pAction)
			{
				if (m_oImagePaintTextDynamic.Transform_SetShadow(pAction, m_dTime))
				{
					m_oPaintSettings.SetShadow(pAction->Shadow);

					return TRUE;
				}

				return FALSE;
			}
			BOOL Process_Paint_TextDynamic_SetEdge(ImageStudio::Serialize::Paint::TextDynamic::SetEdge* pAction)
			{
				if (m_oImagePaintTextDynamic.Transform_SetEdge(pAction, m_dTime))
				{
					m_oPaintSettings.SetEdge(pAction->Edge);

					return TRUE;
				}

				return FALSE;
			}
			BOOL Process_Paint_TextDynamic_Start(ImageStudio::Serialize::Paint::TextDynamic::Start* pAction)
			{
				return m_oImagePaintTextDynamic.Transform_Start(pAction, m_dTime, m_oSources.Get( pAction->SourceId ));
			}
			BOOL Process_Paint_TextDynamic_Stop(ImageStudio::Serialize::Paint::TextDynamic::Stop* pAction)
			{
				return m_oImagePaintTextDynamic.Transform_Stop(pAction, m_dTime);
			}
			BOOL Process_Paint_TextDynamic_Render(ImageStudio::Serialize::Paint::TextDynamic::Render* pAction)
			{
				if (!pAction)
					return FALSE;

				IUnknown* pResult = NULL;

				if (!m_oImagePaintTextDynamic.Transform_Render(pAction, m_dTime, m_oSources.Get( pAction->SourceId ), pResult))
					return FALSE;

				m_oSources.Set( 0, pResult );

				return TRUE;
			}
			
			BOOL Process_Compose_Render(ImageStudio::Serialize::Compose::Render* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = 0;
				if (pAction->Time > 0)
					dCompleteness = m_dTime/pAction->Time;
				else
					dCompleteness = pAction->GetCompleteness(m_dTime);

				if (dCompleteness < 0.0001)
					return TRUE;

				m_oImageComposer.m_dwBaseRnd = GetBaseRandom( pAction->BaseRandom );
				
				int nRetCode = Core::COM::COM_ImageComposition(m_oSources.Get( 1 ), m_oSources.Get( 2 ), m_oSources.Get( 0 ), pAction->TilesWidth, pAction->TilesHeight, pAction->EffectID, dCompleteness, &m_oImageComposer);

				return Core::c_nErrorNone == nRetCode;
			}
			
			BOOL Process_Paint_Text_AnimatedText(ImageStudio::Serialize::Paint::Text::AnimatedText* pAction)
			{
				if (!pAction)
					return FALSE;

				double dCompleteness = pAction->GetCompletenessStage(m_dTime);
				//if (dCompleteness < 0.0001 || dCompleteness > 2.9999)
				//	return TRUE;

				if (dCompleteness <= 0 || dCompleteness >= 3)
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->GetMetric(), dMetricX, dMetricY ) )
					return FALSE;

				// отрабатываем m_oCache - если TextPainter уже создавали ранее ранее - используем текущий класс
				int nSessionID = pAction->GetSessionID();
				ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
				if (NULL == pCacheItem || ImageStudio::Caching::c_nTypePointer != pCacheItem->GetType() || pAction->GetAnimationID(1) != ImageStudio::Serialize::Paint::Text::c_nAnimatedTextRectangle )
				{
					// подготавливаем отрисовщик
					m_oPaintSettings.SaveState();

					m_oPaintSettings.SetFont(pAction->GetFont());
					m_oPaintSettings.SetBrush(pAction->GetBrush(), FALSE);
					m_oPaintSettings.SetShadow(pAction->GetShadow());
					m_oPaintSettings.SetEdge(pAction->GetEdge());

					// в зависимости от типа метрики рисуем объект соответствующего размера
					Painter::CFont& oFont = m_oPaintSettings.GetFont();
					double dNewFontSize = oFont.Size * dMetricX;
					oFont.SetFontSize(dNewFontSize);
					
					// инициализируем отрисовщик текста (в соответствие с фонтом и строкой, которую будем отрисовывать)
					BSTR   bstrText = pAction->GetText().AllocSysString();
					int    nLength  = pAction->GetText().GetLength();
					
					double dLeft    = dMetricX * pAction->GetRectLeft();
					double dTop     = dMetricY * pAction->GetRectTop();
					double dRight   = dMetricX * pAction->GetRectRight();
					double dBottom  = dMetricY * pAction->GetRectBottom();

					Gdiplus::Font* pFont = m_oPaintSettings.GetFont().GetFont();

					int nAlignVertical   = pAction->GetFont().IsValidStringAlignmentVertical() ? pAction->GetFont().GetStringAlignmentVertical() : Gdiplus::StringAlignmentNear;
					int nAlignHorizontal = pAction->GetFont().IsValidStringAlignmentHorizontal() ? pAction->GetFont().GetStringAlignmentHorizontal() : Gdiplus::StringAlignmentNear;
					double dRotateAngle  = pAction->GetFont().IsValidAngle() ? pAction->GetFont().GetAngle() : 0;
					
					double dImageWidth  = 0;
					double dImageHeight = 0;
					GetMetricMultiplier( pBitmap, 2, dImageWidth, dImageHeight );

					int nFrameWidth  = (pAction->m_bUseFrameOnly > 0) ? Round(dImageWidth)  : 0;
					int nFrameHeight = (pAction->m_bUseFrameOnly > 0) ? Round(dImageHeight) : 0;

					if ( pAction->IsEvaluateRect() )
					{
						Gdiplus::Bitmap oImage( int(dImageWidth), int(dImageHeight), PixelFormat32bppARGB);
						if( Gdiplus::Ok == oImage.GetLastStatus() )
						{
							Gdiplus::Graphics oRender( &oImage );
							if( Gdiplus::Ok == oRender.GetLastStatus() )
							{

								Gdiplus::RectF oRect;							
								m_oPaintSettings.GetBrush().GetTextRect(bstrText, &oRender, m_oPaintSettings.GetFont().GetFont(),  0, 0, &oRect);
								
								dBottom = dTop  + oRect.Height;
								dRight  = dLeft + oRect.Width;
							}
						}
					}


					int nAlignType = int(pAction->GetAlignType());
					
					if ( nAlignType >= 0 )
					{
						double dRectHeight = dBottom - dTop;
						double dRectWidth  = dRight - dLeft;

						double dXMargin  = (double)pAction->GetMarginX();
						double dYMargin  = (double)pAction->GetMarginY();

		
						if ( 0 != dRotateAngle )
						{
							double dXOld = dRectWidth / 2;
							double dYOld = dRectHeight / 2;


							double dAngle = dRotateAngle * 3.141592653589793 / 180;

							double dX1 = fabs( dXOld * cos(dAngle) - dYOld * sin(dAngle) );
							double dY1 = fabs( dXOld * sin(dAngle) + dYOld * cos(dAngle) );

							double dX2 = fabs( dXOld * cos(dAngle) + dYOld * sin(dAngle) );
							double dY2 = fabs( dXOld * sin(dAngle) - dYOld * cos(dAngle) );


							dXMargin += max( dX1, dX2 ) - dXOld;
							dYMargin += max( dY1, dY2 ) - dYOld;

						}

						switch (nAlignType)
						{
						case 0: dLeft = dXMargin;                            dTop = dYMargin;                              break;
						case 1: dLeft = dImageWidth / 2  - dRectWidth / 2;   dTop = dYMargin;                              break;
						case 2: dLeft = dImageWidth - dRectWidth - dXMargin; dTop = dYMargin;                              break;
						case 3: dLeft = dXMargin;                            dTop = dImageHeight / 2  - dRectHeight / 2;   break;
						case 4: dLeft = dImageWidth / 2  - dRectWidth / 2;   dTop = dImageHeight / 2  - dRectHeight / 2;   break;
						case 5: dLeft = dImageWidth - dRectWidth - dXMargin; dTop = dImageHeight / 2  - dRectHeight / 2;   break;
						case 6: dLeft = dXMargin;                            dTop = dImageHeight - dRectHeight - dYMargin; break;
						case 7: dLeft = dImageWidth / 2  - dRectWidth / 2;   dTop = dImageHeight - dRectHeight - dYMargin; break;
						case 8: dLeft = dImageWidth - dRectWidth - dXMargin; dTop = dImageHeight - dRectHeight - dYMargin; break;
						}
 
						dRight  = dLeft + dRectWidth;
						dBottom = dTop + dRectHeight;

					}

					Gdiplus::RectF oLayoutRect( float(dLeft), float(dTop), float(dRight - dLeft), float(dBottom - dTop) );

					
					Painter::CEdgeText& oEdge = m_oPaintSettings.GetEdge();
					double dNewDistance = oEdge.Dist * dMetricX;
					oEdge.SetDistance( dNewDistance );

					Painter::CShadow& oShadow = m_oPaintSettings.GetShadow();
					double dNewDistanceX = oShadow.DistanceX * dMetricX;
					double dNewDistanceY = oShadow.DistanceY * dMetricY;
					oShadow.SetDistanceX( dNewDistanceX );
					oShadow.SetDistanceY( dNewDistanceY );

					double dNewBlurSize = oShadow.BlurSize * dMetricX;
					if( dNewBlurSize > 100 ) 
						dNewBlurSize = 100;
					oShadow.SetBlurSize( dNewBlurSize );

					Painter::CBrush& oFillBrush = m_oPaintSettings.GetBrush();
					
					// получаем текстурный браш, если он указан
					Painter::CTexture* pTexture = NULL;
					IUnknown* pTextureInterface = m_oImageCache.GetImage( oFillBrush.TexturePath );
					if( pTextureInterface )
					{
						BYTE* pBGRA = NULL;
						int nWidth = 0;
						int nHeight = 0;

						if( Core::c_nErrorNone == Core::COM::COM_GetBGRAImageFromInterface( pTextureInterface, pBGRA, nWidth, nHeight ) )
						{
							pTexture = new Painter::CTexture();
							if( pTexture )
							{
								Gdiplus::Rect oImageRect( 0, 0, nWidth, nHeight );

								ImageStudio::Serialize::Paint::Structures::Brush& brush = pAction->GetBrush();
								if( brush.GetRectable() )
								{
									Gdiplus::RectF oRect = brush.GetRect();
									Gdiplus::Rect oBrushRect( 
										int(oRect.X * nWidth + 0.5),
										int(oRect.Y * nHeight + 0.5),
										int(oRect.Width * nWidth + 0.5),
										int(oRect.Height * nHeight + 0.5) );

									if( oBrushRect.Intersect( oImageRect ) )
									{
										pBGRA += (oBrushRect.Y * nWidth + oBrushRect.X) * 4;
										oImageRect.Width  = oBrushRect.Width;
										oImageRect.Height = oBrushRect.Height;
									}
								}

								pTexture->Path          = oFillBrush.TexturePath.MakeLower();
								pTexture->Bitmap        = NULL;
								pTexture->BitmapData    = pBGRA;
								pTexture->bFlipVertical = FALSE;
								pTexture->bAttach       = TRUE;
								pTexture->nWidth        = oImageRect.Width;
								pTexture->nHeight       = oImageRect.Height;
								pTexture->nStride       = nWidth*4;

								pTextureInterface->AddRef();
							}
						}
					}

					if( pAction->GetAnimationID(1) == ImageStudio::Serialize::Paint::Text::c_nAnimatedTextRectangle )
					{
						ImageStudio::Paint::Text::CTextPainter2* pPainter = new ImageStudio::Paint::Text::CTextPainter2();
						if( pPainter )
						{
							pPainter->CreateText( bstrText, *pFont, oLayoutRect, nAlignHorizontal, nAlignVertical, pAction->m_bSimpleLine, nFrameWidth, nFrameHeight );
							pPainter->CreateEffects( pAction->GetAnimationID(0), pAction->GetAnimationID(2), pAction->m_nTypeEffects );
							
							pPainter->m_oTextureManager.Add( pTexture );
							pPainter->pImage = pTextureInterface;
							
							pPainter->SetFillBrush( oFillBrush );
							pPainter->SetEdge( oEdge );
							pPainter->SetShadow( oShadow );
							pPainter->m_nFillMode = pAction->m_nFillMode;
							pPainter->m_fAngle = float(dRotateAngle);

							pPainter->m_dStartX = pAction->m_dTextDirX1;
							pPainter->m_dStartY = pAction->m_dTextDirY1;
							pPainter->m_dEndX = pAction->m_dTextDirX2;
							pPainter->m_dEndY = pAction->m_dTextDirY2;
						}

						m_oCache.Add( nSessionID, pPainter );
						pCacheItem = pPainter;

						// восстанавливаем старое состояние отрисовщика
						m_oPaintSettings.RestoreState();
					}
					else
					{
						ImageStudio::Paint::Text::CTextPainter* pPainter = (ImageStudio::Paint::Text::CTextPainter*)pCacheItem;
						if( NULL == pPainter )
						{
							pPainter = new ImageStudio::Paint::Text::CTextPainter();

							m_oCache.Add( nSessionID, pPainter );
							pCacheItem = pPainter;
						}

						if( pPainter )
						{
							m_oPaintSettings.GetTextureManager().Add( pTexture );

							pPainter->SetPaintState(&m_oPaintSettings);
							pPainter->SetTextureManager(&m_oPaintSettings.GetTextureManager());
							
							// располагаем статический текст так, как указано в pAction
							if (pAction->GetAnimationID(1) == ImageStudio::Serialize::Paint::Text::c_nAnimatedTextLine)
							{
								pPainter->TextCreate(bstrText, nLength, *pFont);
								pPainter->TextPlaceLine(dLeft, dTop);
							}
							else if (pAction->GetAnimationID(1) == ImageStudio::Serialize::Paint::Text::c_nAnimatedTextEllipse)
							{
								pPainter->TextCreate(bstrText, nLength, *pFont);
								pPainter->TextPlaceEllipse(dLeft, dTop, dRight, dBottom, pAction->GetSettingsEllipse().GetAngleStart());
							}
							else if (pAction->GetAnimationID(1) == ImageStudio::Serialize::Paint::Text::c_nAnimatedTextPolyline)
							{
								// TODO: сделать поддержку метрики в координатах
								pPainter->TextCreate(bstrText, nLength, *pFont);
								pPainter->TextPlacePolyline(pAction->GetSettingsPolyline().GetPoints());
							}
							else
							{
								pPainter->TextCreate(bstrText, nLength, *pFont, 0, 0, dRight - dLeft, dBottom - dTop, nAlignVertical, nAlignHorizontal, pAction->IsTextClipped());
								pPainter->TextPlaceRectangle(dLeft, dTop, dRight, dBottom, nAlignVertical, nAlignHorizontal, dRotateAngle);
							}

							// создаем анимацию текст (если таковая указана)
							pPainter->TextTransform(pAction->GetAnimationID(0), pAction->GetAnimationID(1), pAction->GetAnimationID(2), 0, 1);
						}
					}

					SysFreeString( bstrText );
				}

				int nResult = Core::c_nErrorNone;

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				// отрисовываем текст
				if( pAction->GetAnimationID(1) == ImageStudio::Serialize::Paint::Text::c_nAnimatedTextRectangle )
				{
					ImageStudio::Paint::Text::CTextPainter2* pPainter = (ImageStudio::Paint::Text::CTextPainter2*)pCacheItem;
					pPainter->Draw( dCompleteness, oRender );
				}
				else
				{
					ImageStudio::Paint::Text::CTextPainter* pPainter = (ImageStudio::Paint::Text::CTextPainter*)pCacheItem;
					pPainter->TextDraw( dCompleteness, oRender );

					// восстанавливаем старое состояние отрисовщика
					m_oPaintSettings.RestoreState();
				}

				return (Core::c_nErrorNone == nResult);
			}

			BOOL Process_Paint_Text_WritingText(ImageStudio::Serialize::Paint::Text::EffectWritingText* pAction)
			{
				if (!pAction)
					return FALSE;

				//double dCompleteness = pAction->GetCompletenessStage(m_dTime);

				//if (dCompleteness <= 0 || dCompleteness >= 3)
				//	return TRUE;
				double dCompleteness = pAction->GetCompleteness(m_dTime);
				if (dCompleteness <= 0 )
					return TRUE;

				Gdiplus::Bitmap* pBitmap = m_oSources.GetBitmap( pAction->SourceId );
				if( !pBitmap )
					return FALSE;
				
				double dMetricX, dMetricY;
				if( !GetMetricMultiplier( pBitmap, pAction->GetMetric(), dMetricX, dMetricY ) )
					return FALSE;

				// отрабатываем m_oCache - если TextPainter уже создавали ранее ранее - используем текущий класс
				int nSessionID = pAction->GetSessionID();
				ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
				if (NULL == pCacheItem || ImageStudio::Caching::c_nTypePointer != pCacheItem->GetType() )
				{
					// подготавливаем отрисовщик
					m_oPaintSettings.SaveState();

					m_oPaintSettings.SetFont(pAction->GetFont());
					m_oPaintSettings.SetEdge(pAction->GetEdge());

					// в зависимости от типа метрики рисуем объект соответствующего размера
					Painter::CFont& oFont = m_oPaintSettings.GetFont();
					double dNewFontSize = oFont.Size * dMetricX;
					oFont.SetFontSize(dNewFontSize);
					
					// инициализируем отрисовщик текста (в соответствие с фонтом и строкой, которую будем отрисовывать)
					BSTR   bstrText = pAction->GetText().AllocSysString();
					int    nLength  = pAction->GetText().GetLength();
					
					double dLeft    = dMetricX * pAction->GetRectLeft();
					double dTop     = dMetricY * pAction->GetRectTop();
					double dRight   = dMetricX * pAction->GetRectRight();
					double dBottom  = dMetricY * pAction->GetRectBottom();

					Gdiplus::Font* pFont = m_oPaintSettings.GetFont().GetFont();

					int nAlignVertical   = pAction->GetFont().IsValidStringAlignmentVertical() ? pAction->GetFont().GetStringAlignmentVertical() : Gdiplus::StringAlignmentNear;
					int nAlignHorizontal = pAction->GetFont().IsValidStringAlignmentHorizontal() ? pAction->GetFont().GetStringAlignmentHorizontal() : Gdiplus::StringAlignmentNear;
					double dRotateAngle  = pAction->GetFont().IsValidAngle() ? pAction->GetFont().GetAngle() : 0;
					
					//double dImageWidth  = 0;
					//double dImageHeight = 0;
					//GetMetricMultiplier( pBitmap, 2, dImageWidth, dImageHeight );

					//int nFrameWidth  = Round(dImageWidth);
					//int nFrameHeight = Round(dImageHeight);

					Gdiplus::RectF oLayoutRect( float(dLeft), float(dTop), float(dRight - dLeft), float(dBottom - dTop) );

					Painter::CEdgeText& oEdge = m_oPaintSettings.GetEdge();
					double dNewDistance = oEdge.Dist * dMetricX;
					oEdge.SetDistance( dNewDistance );

					ImageStudio::Paint::Text::CTextPainter2* pPainter = new ImageStudio::Paint::Text::CTextPainter2();
					if( pPainter )
					{
						pPainter->CreateText( bstrText, *pFont, oLayoutRect, nAlignHorizontal, nAlignVertical, 0x80000000, 0/*nFrameWidth*/, 0/*nFrameHeight*/ );
						pPainter->CreateEffects( 0, 0, 0 );
						
						pPainter->SetEdge( oEdge );
						pPainter->m_fAngle = float(dRotateAngle);

						pPainter->m_bFillText = FALSE;
						pPainter->m_bDrawEdge = TRUE;
						pPainter->m_bDrawShadow = FALSE;
						pPainter->m_nTypeWrittingEdge = pAction->GetType() ? 2 : 1;
					}

					m_oCache.Add( nSessionID, pPainter );
					pCacheItem = pPainter;

					// восстанавливаем старое состояние отрисовщика
					m_oPaintSettings.RestoreState();

					SysFreeString( bstrText );
				}

				Gdiplus::Graphics oRender( pBitmap );
				if( Gdiplus::Ok != oRender.GetLastStatus() )
					return FALSE;

				// отрисовываем текст
				ImageStudio::Paint::Text::CTextPainter2* pPainter = (ImageStudio::Paint::Text::CTextPainter2*)pCacheItem;
				pPainter->Draw( dCompleteness, oRender );

				return Core::c_nErrorNone;
			}
						
			BOOL Process_Paint_DrawImageFromFileAnimate ( ImageStudio::Serialize::Paint::Animations::DrawImageFromFileAnimate* pAction )
			{
				return TRUE;
				
				//if (!pAction)
				//	return FALSE;

				//double dCompleteness = pAction->GetCompleteness(m_dTime);
				//if (dCompleteness < 0.0001)
				//	return TRUE;
				//
				//IUnknown* pInterfaceImage = NULL;

				//// отрабатываем m_oCache - если картинку уже загружали ранее - используем текущий интерфейс
				//int nSessionID = pAction->GetSessionID();
				//ImageStudio::Caching::CCacheItem* pCacheItem = m_oCache.GetAt(nSessionID);
				//if (NULL != pCacheItem && ImageStudio::Caching::c_nTypeInterface == pCacheItem->GetType())
				//{
				//	pInterfaceImage = ((ImageStudio::Caching::CCacheItemInterface*)pCacheItem)->GetInterface();
				//}
				//else
				//{
				//	if( m_pFileCache )
				//	{
				//		m_pFileCache->raw_LoadImageW( _bstr_t( pAction->FilePath ), -1, -1, &pInterfaceImage );

				//		if( pInterfaceImage )
				//		//if( Core::c_nErrorNone == Core::COM::COM_LoadImage( &pInterfaceImage, pAction->FilePath ) )
				//		{
				//			ImageStudio::Caching::CCacheItemInterface* pCacheInterface = new ImageStudio::Caching::CCacheItemInterface(pInterfaceImage);
				//			m_oCache.Add(nSessionID, pCacheInterface);
				//			pInterfaceImage->Release();
				//		}

				//	}
				//	else
				//	{
				//		pInterfaceImage = m_oImageCache.GetImage( pAction->FilePath );
				//		if( pInterfaceImage )
				//		//if( Core::c_nErrorNone == Core::COM::COM_LoadImage( &pInterfaceImage, pAction->FilePath ) )
				//		{
				//			ImageStudio::Caching::CCacheItemInterface* pCacheInterface = new ImageStudio::Caching::CCacheItemInterface(pInterfaceImage);
				//			m_oCache.Add(nSessionID, pCacheInterface);
				//		}
				//	}
				//}

				//if( !pInterfaceImage )
				//	return FALSE;

				//double dMetricX = 1, dMetricY = 1;
				//GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY );
				//	
				//double dLeft    =	pAction->Left   * dMetricX;
				//double dRight   =	pAction->Right  * dMetricX;
				//double dTop     =	pAction->Top    * dMetricY;
				//double dBottom  =	pAction->Bottom * dMetricY;

				//ImageStudio::Paint::Animations::CImageDrawerEx oDrawerEx;
				//
				//return	oDrawerEx.OnProcess ( m_dTime, dLeft, dRight, dTop, dBottom, dCompleteness, m_oSources.Get( pAction->SourceId ), pInterfaceImage, pAction );
			}

			BOOL Process_Paint_DrawPathAnimate ( ImageStudio::Serialize::Paint::Animations::DrawGraphicPathAnimate* pAction )
			{
				return TRUE;
				
				//if (!pAction)
				//	return FALSE;
			
				////if ( m_dTime < pAction->m_Animation.m_nTimeDelay ||	m_dTime > pAction->m_Animation.m_nTimeDelay + pAction->m_Animation.m_nTimeDuration || 0.0 == pAction->m_Animation.m_nTimeDuration )
				////	return TRUE;

				//double dCompleteness = pAction->GetCompleteness(m_dTime);
				//if (dCompleteness < 0.000001)
				//	return TRUE;

				//double dMetricX, dMetricY;
				//if( !GetMetricMultiplier( m_oSources.Get( pAction->SourceId ), pAction->Metric, dMetricX, dMetricY ) )
				//	return FALSE;

				//ImageStudio::Paint::Animations::CGraphicsPathDrawerEx oDrawerEx;

				//return  oDrawerEx.OnProcess ( m_dTime, m_oSources.Get( pAction->SourceId ), &m_oPaintSettings, pAction );
			}
			
			virtual int OnTransform(ImageStudio::Serialize::SerializedObject* pTransform)
			{
				if (!pTransform)
					return TRUE;

				int nCommand = pTransform->ID;

				int bSuccess = FALSE; // 0 - error, 1 - ok, -1 - repeat from last safe point

				// надо выставить source interface для работы - это всегда первый интерфейс в буффере
				// но (!) он не обязательно должен быть ненулевым, т.к. есть эффекты-source'ы, которые
				// генерируют картинку самостоятельно... пример: ImageFile-LoadImage

				switch (nCommand)
				{
				case ImageStudio::Serialize::c_nSerializeNone:
					return TRUE;
				
				case  ImageStudio::Serialize::c_nSerializeGroup:						SerializedObjectTransformer_ProcessSerializedObject(SerializedGroup, pTransform, Process_Group, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeBufferClear:					SerializedObjectTransformer_ProcessSerializedObject(SerializedBufferClear, pTransform, Process_BufferClear, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeBufferCopy:					SerializedObjectTransformer_ProcessSerializedObject(SerializedBufferCopy, pTransform, Process_BufferCopy, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeSetAdditionalParams:			SerializedObjectTransformer_ProcessSerializedObject(Core::AdditionalParams, pTransform, Process_SetAdditionalParams, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeSetParamsUseVideoCard:		SerializedObjectTransformer_ProcessSerializedObject(SerializedSetParamsUseVideoCard, pTransform, Process_SetParamsUseVideoCard, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializeImageClear:					SerializedObjectTransformer_ProcessSerializedObject(Effects::ImageClear, pTransform, Process_ImageClear, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeImageCopy:					SerializedObjectTransformer_ProcessSerializedObject(Effects::ImageCopy, pTransform, Process_ImageCopy, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializeAdjustBrightness:				SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustBrightness, pTransform, Process_Adjust_Brightness, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustBrightnessEx:			SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustBrightnessEx, pTransform, Process_Adjust_BrightnessEx, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustContrast:				SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustContrast, pTransform, Process_Adjust_Contrast, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustGamma:					SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustGamma, pTransform, Process_Adjust_Gamma, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustEqualize:				SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustEqualize, pTransform, Process_Adjust_Equalize, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustGrayscale:				SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustGrayscale, pTransform, Process_Adjust_Grayscale, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustInvert:					SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustInvert, pTransform, Process_Adjust_Invert, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustAutobrightness:			SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustAutobrightness, pTransform, Process_Adjust_Autobrightness, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustAutocontrast:			SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustAutocontrast, pTransform, Process_Adjust_Autocontrast, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustAutolevels:				SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustAutolevels, pTransform, Process_Adjust_Autolevels, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustHue:					SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustHue, pTransform, Process_Adjust_Hue, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustSaturation:				SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustSaturation, pTransform, Process_Adjust_Saturation, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustPosterize:				SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustPosterize, pTransform, Process_Adjust_Posterize, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustTemperature:			SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustTemperature, pTransform, Process_Adjust_Temperature, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustColorize:				SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustColorize, pTransform, Process_Adjust_Colorize, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustThreshold:				SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustThreshold, pTransform, Process_Adjust_Threshold, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustSepia:				    SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustSepia, pTransform, Process_Adjust_Sepia, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializeAdjustRecovery:			    SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustRecovery, pTransform, Process_Adjust_Recovery, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustFillLight:			    SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustFillLight, pTransform, Process_Adjust_FillLight, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustExposure:			    SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustExposure, pTransform, Process_Adjust_Exposure, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustBlacks:				    SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustBlacks, pTransform, Process_Adjust_Blacks, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustHighLight:			    SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustHighLight, pTransform, Process_Adjust_HighLight, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustShadows:			    SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustShadows, pTransform, Process_Adjust_Shadows, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustWhiteBalance:		    SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustWhiteBalance, pTransform, Process_Adjust_WhiteBalance, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustClarity:			    SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustClarity, pTransform, Process_Adjust_Clarity, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustVibrance:			    SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustVibrance, pTransform, Process_Adjust_Vibrance, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeAdjustAutoWhiteBalance:		SerializedObjectTransformer_ProcessSerializedObject(Effects::AdjustAutoWhiteBalance, pTransform, Process_Adjust_AutoWhiteBalance, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializeEffectBlur:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectBlur, pTransform, Process_Effect_Blur, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectSharpen:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectSharpen, pTransform, Process_Effect_Sharpen, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectMosaic:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectMosaic, pTransform, Process_Effect_Mosaic, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectGaussianBlur:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectGaussianBlur, pTransform, Process_Effect_GaussianBlur, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectNoise:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectNoise, pTransform, Process_Effect_Noise, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectDiffuse:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectDiffuse, pTransform, Process_Effect_Diffuse, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectMotionBlur:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectMotionBlur, pTransform, Process_Effect_MotionBlur, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectEmboss:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectEmboss, pTransform, Process_Effect_Emboss, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectKenBurns:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectKenBurns, pTransform, Process_Effect_KenBurns, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectMinimal:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectMinimal, pTransform, Process_Effect_Minimal, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectMaximal:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectMaximal, pTransform, Process_Effect_Maximal, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectDeinterlace:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectDeinterlace, pTransform, Process_Effect_Deinterlace, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectMedian:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectMedian, pTransform, Process_Effect_Median, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectChromoKeyRange:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectChromoKeyRange, pTransform, Process_Effect_ChromoKey_Range, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectChromoKeyLevel:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectChromoKeyLevel, pTransform, Process_Effect_ChromoKey_Level, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectCinema:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectCinema, pTransform, Process_Effect_Cinema, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectGlass:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectGlass, pTransform, Process_Effect_Glass, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectGlassMosaic:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectGlassMosaic, pTransform, Process_Effect_GlassMosaic, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectPencilSketch:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectPencilSketch, pTransform, Process_Effect_PencilSketch, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectWaterColor:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectWaterColor, pTransform, Process_Effect_WaterColor, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectGlow:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectGlow, pTransform, Process_Effect_Glow, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectTVSimulation:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectTVSimulation, pTransform, Process_Effect_TVSimulation, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectGrayOut:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectGrayOut, pTransform, Process_Effect_GrayOut, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectWind:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectWind, pTransform, Process_Effect_Wind, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectNewsprint:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectNewsprint, pTransform, Process_Effect_Newsprint, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectSnow:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectSnow, pTransform, Process_Effect_Snow, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectJpegCompression:		SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectJpegCompression, pTransform, Process_Effect_JpegCompression, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectSoftFocus:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectSoftFocus, pTransform, Process_Effect_SoftFocus, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectOldPhoto:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectOldPhoto, pTransform, Process_Effect_OldPhoto, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectVignette:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectVignette, pTransform, Process_Effect_Vignette, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectPostCrop:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectPostCrop, pTransform, Process_Effect_PostCrop, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectSetAlphaChannel:		SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectSetAlphaChannel, pTransform, Process_Effect_SetAlphaChannel, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectZoomingSphere:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectZoomingSphere, pTransform, Process_Effect_ZoomingSphere, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectWaterMirrow:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectWaterMirrow, pTransform, Process_Effect_WaterMirrow, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectShapeMosaic:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectShapeMosaic, pTransform, Process_Effect_ShapeMosaic, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectParticles:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectParticles, pTransform, Process_Effect_Particles, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectCanvas:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectCanvas, pTransform, Process_Effect_Canvas, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectGlow2:					SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectGlow2, pTransform, Process_Effect_Glow2, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectColorExchange:			SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectColorExchange, pTransform, Process_Effect_ColorExchange, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectAnaglyph3d:				SerializedObjectTransformer_ProcessSerializedObject(Effects::EffectAnaglyph3d, pTransform, Process_Effect_Anaglyph3d, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectFog:	                SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawFog, pTransform, Process_Paint_DrawFog, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializeTransformFlip:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformFlip, pTransform, Process_Transform_Flip, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformFlip2:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformFlip2, pTransform, Process_Transform_Flip2, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformPerspective:			SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformPerspective, pTransform, Process_Transform_Perspective, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformSkew:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformSkew, pTransform, Process_Transform_Skew, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformShift:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformShift, pTransform, Process_Transform_Shift, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformRotate:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformRotate, pTransform, Process_Transform_Rotate, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformRotateAdvanced:		SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformRotateAdvanced, pTransform, Process_Transform_RotateAdvanced, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformRotateSimple:		SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformRotateSimple, pTransform, Process_Transform_RotateSimple, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformResample:			SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformResample, pTransform, Process_Transform_Resample, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformZoom:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformZoom, pTransform, Process_Transform_Zoom, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformZoomLinear:			SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformZoomLinear, pTransform, Process_Transform_ZoomLinear, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformMirror:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformMirror, pTransform, Process_Transform_Mirror, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformResize:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformResize, pTransform, Process_Transform_Resize, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformTwirl:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformTwirl, pTransform, Process_Transform_Twirl, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformSphere:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformSphere, pTransform, Process_Transform_Sphere, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformCylinder:			SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformCylinder, pTransform, Process_Transform_Cylinder, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformWave:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformWave, pTransform, Process_Transform_Wave, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformRipple:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformRipple, pTransform, Process_Transform_Ripple, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformRotateSpecial:		SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformRotateSpecial, pTransform, Process_Transform_RotateSpecial, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformLinear:				SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformLinear, pTransform, Process_Transform_Linear, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformGlassTile:			SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformGlassTile, pTransform, Process_Transform_GlassTile, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeTransformBrokenGlass:			SerializedObjectTransformer_ProcessSerializedObject(Effects::TransformBrokenGlass, pTransform, Process_Transform_BrokenGlass, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializeFilterPrewitt:				SerializedObjectTransformer_ProcessSerializedObject(Effects::FilterPrewitt, pTransform, Process_Filter_Prewitt, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFilterScharr:					SerializedObjectTransformer_ProcessSerializedObject(Effects::FilterScharr, pTransform, Process_Filter_Scharr, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFilterSobel:					SerializedObjectTransformer_ProcessSerializedObject(Effects::FilterSobel, pTransform, Process_Filter_Sobel, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFilterRoberts:				SerializedObjectTransformer_ProcessSerializedObject(Effects::FilterRoberts, pTransform, Process_Filter_Roberts, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFilterLaplace:				SerializedObjectTransformer_ProcessSerializedObject(Effects::FilterLaplace, pTransform, Process_Filter_Laplace, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFilterHipass:					SerializedObjectTransformer_ProcessSerializedObject(Effects::FilterHipass, pTransform, Process_Filter_Hipass, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFilterLowpass:				SerializedObjectTransformer_ProcessSerializedObject(Effects::FilterLowpass, pTransform, Process_Filter_Lowpass, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFilterBlur:					SerializedObjectTransformer_ProcessSerializedObject(Effects::FilterBlur, pTransform, Process_Filter_Blur, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFilterCanny:					SerializedObjectTransformer_ProcessSerializedObject(Effects::FilterCanny, pTransform, Process_Filter_Canny, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializeFileLoadImage:				SerializedObjectTransformer_ProcessSerializedObject(File::FileLoadImage, pTransform, Process_File_LoadImage, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileLoadScreenshot:			SerializedObjectTransformer_ProcessSerializedObject(File::FileLoadScreenshot, pTransform, Process_File_LoadScreenshot, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileLoadRaw:				    SerializedObjectTransformer_ProcessSerializedObject(File::FileLoadRaw, pTransform, Process_File_LoadRaw, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsBmp:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsBmp, pTransform, Process_File_SaveAsBmp, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsGif:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsGif, pTransform, Process_File_SaveAsGif, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsJpeg:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsJpeg, pTransform, Process_File_SaveAsJpeg, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsPng:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsPng, pTransform, Process_File_SaveAsPng, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsTiff:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsTiff, pTransform, Process_File_SaveAsTiff, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsWmf:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsWmf, pTransform, Process_File_SaveAsWmf, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsEmf:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsEmf, pTransform, Process_File_SaveAsEmf, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsPcx:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsPcx, pTransform, Process_File_SaveAsPcx, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsTga:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsTga, pTransform, Process_File_SaveAsTga, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsRas:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsRas, pTransform, Process_File_SaveAsRas, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsIPod:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsIPod, pTransform, Process_File_SaveAsIPod, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsPsd:				SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsPsd, pTransform, Process_File_SaveAsPsd, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeFileSaveAsJp2:                SerializedObjectTransformer_ProcessSerializedObject(File::FileSaveAsJp2, pTransform, Process_File_SaveAsJp2, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializePaintSetPen:					SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::SetPen, pTransform, Process_Paint_SetPen, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintSetBrush:				SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::SetBrush, pTransform, Process_Paint_SetBrush, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintSetFont:					SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::SetFont, pTransform, Process_Paint_SetFont, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintSetFormat:				SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::SetFormat, pTransform, Process_Paint_SetFormat, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawLine:				SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawLine, pTransform, Process_Paint_DrawLine, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawRectangle:			SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawRectangle, pTransform, Process_Paint_DrawRectangle, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawEllipse:				SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawEllipse, pTransform, Process_Paint_DrawEllipse, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawInvRectangle:		SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawInvRectangle, pTransform, Process_Paint_DrawInvRectangle, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawInvEllipse:			SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawInvEllipse, pTransform, Process_Paint_DrawInvEllipse, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawPie:					SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawPie, pTransform, Process_Paint_DrawPie, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawSector:				SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawSector, pTransform, Process_Paint_DrawSector, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawSimplePie:			SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawSimplePie, pTransform, Process_Paint_DrawSimplePie, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawSimpleSector:		SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawSimpleSector, pTransform, Process_Paint_DrawSimpleSector, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawPolyline:			SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawPolyline, pTransform, Process_Paint_DrawPolyline, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawPolygon:				SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawPolygon, pTransform, Process_Paint_DrawPolygon, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawBorder:				SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawBorder, pTransform, Process_Paint_DrawBorder, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawMovObject:			SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawMovObject, pTransform, Process_Paint_DrawMovObject, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawCredits:				SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawCredits, pTransform, Process_Paint_DrawCredits, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawTextSimple:			SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawTextSimple, pTransform, Process_Paint_DrawTextSimple, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawTextInRect:			SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawTextInRect, pTransform, Process_Paint_DrawTextInRect, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawImageFromFile:		SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawImageFromFile, pTransform, Process_Paint_DrawImageFromFile, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawImageFromFileEx:		SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawImageFromFileEx, pTransform, Process_Paint_DrawImageFromFileEx, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawImageFromBuffer:		SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawImageFromBuffer, pTransform, Process_Paint_DrawImageFromBuffer, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawTimer:        		SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawTimer, pTransform, Process_Paint_DrawTimer, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawImageFromStorage:   	SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawImageFromStorage, pTransform, Process_Paint_DrawImageFromStorage, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawFilm:                SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawFilm, pTransform, Process_Paint_DrawFilm, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawPuzzle:              SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawPuzzle, pTransform, Process_Paint_DrawPuzzle, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawPath:	            SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawGraphicPath, pTransform, Process_Paint_DrawPath, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawAutoShape:	        SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawAutoShape, pTransform, Process_Paint_AutoShape, bSuccess) break;
                case  ImageStudio::Serialize::c_nSerializePaintDrawTextEx:              SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawTextEx, pTransform, Process_Paint_DrawTextEx, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintDrawCountdown:      		SerializedObjectTransformer_ProcessSerializedObject(Paint::Common::DrawCountdown, pTransform, Process_Paint_DrawCountdown, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializePaintTextStaticSetBrush:		SerializedObjectTransformer_ProcessSerializedObject(Paint::TextStatic::SetBrush, pTransform, Process_Paint_TextStatic_SetBrush, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextStaticSetFont:		SerializedObjectTransformer_ProcessSerializedObject(Paint::TextStatic::SetFont, pTransform, Process_Paint_TextStatic_SetFont, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextStaticSetFormat:		SerializedObjectTransformer_ProcessSerializedObject(Paint::TextStatic::SetFormat, pTransform, Process_Paint_TextStatic_SetFormat, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextStaticSetShadow:		SerializedObjectTransformer_ProcessSerializedObject(Paint::TextStatic::SetShadow, pTransform, Process_Paint_TextStatic_SetShadow, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextStaticSetEdge:		SerializedObjectTransformer_ProcessSerializedObject(Paint::TextStatic::SetEdge, pTransform, Process_Paint_TextStatic_SetEdge, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextStaticDrawInRect:	SerializedObjectTransformer_ProcessSerializedObject(Paint::TextStatic::DrawInRect, pTransform, Process_Paint_TextStatic_DrawInRect, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextStaticDrawOnEllipse:	SerializedObjectTransformer_ProcessSerializedObject(Paint::TextStatic::DrawOnEllipse, pTransform, Process_Paint_TextStatic_DrawOnEllipse, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextStaticDrawOnPolyline:SerializedObjectTransformer_ProcessSerializedObject(Paint::TextStatic::DrawOnPolyline, pTransform, Process_Paint_TextStatic_DrawOnPolyline, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializePaintTextDynamicSetBrush:		SerializedObjectTransformer_ProcessSerializedObject(Paint::TextDynamic::SetBrush, pTransform, Process_Paint_TextDynamic_SetBrush, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextDynamicSetFont:		SerializedObjectTransformer_ProcessSerializedObject(Paint::TextDynamic::SetFont, pTransform, Process_Paint_TextDynamic_SetFont, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextDynamicSetShadow:	SerializedObjectTransformer_ProcessSerializedObject(Paint::TextDynamic::SetShadow, pTransform, Process_Paint_TextDynamic_SetShadow, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextDynamicSetEdge:		SerializedObjectTransformer_ProcessSerializedObject(Paint::TextDynamic::SetEdge, pTransform, Process_Paint_TextDynamic_SetEdge, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextDynamicStart:		SerializedObjectTransformer_ProcessSerializedObject(Paint::TextDynamic::Start, pTransform, Process_Paint_TextDynamic_Start, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextDynamicStop:			SerializedObjectTransformer_ProcessSerializedObject(Paint::TextDynamic::Stop, pTransform, Process_Paint_TextDynamic_Stop, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializePaintTextDynamicRender:		SerializedObjectTransformer_ProcessSerializedObject(Paint::TextDynamic::Render, pTransform, Process_Paint_TextDynamic_Render, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializeComposeRender:				SerializedObjectTransformer_ProcessSerializedObject(Compose::Render, pTransform, Process_Compose_Render, bSuccess) break;

				case  ImageStudio::Serialize::c_nSerializePaintAnimatedText:			SerializedObjectTransformer_ProcessSerializedObject(Paint::Text::AnimatedText, pTransform, Process_Paint_Text_AnimatedText, bSuccess) break;
				case  ImageStudio::Serialize::c_nSerializeEffectWritingText:			SerializedObjectTransformer_ProcessSerializedObject(Paint::Text::EffectWritingText, pTransform, Process_Paint_Text_WritingText, bSuccess) break;

				case ImageStudio::Serialize::c_nSerializePaintDrawImageFromFileAnimate:	SerializedObjectTransformer_ProcessSerializedObject(Paint::Animations::DrawImageFromFileAnimate, pTransform, Process_Paint_DrawImageFromFileAnimate, bSuccess) break;
				case ImageStudio::Serialize::c_nSerializePaintDrawPathAnimate:			SerializedObjectTransformer_ProcessSerializedObject(Paint::Animations::DrawGraphicPathAnimate, pTransform, Process_Paint_DrawPathAnimate, bSuccess) break;

				default:
					return FALSE;
				}

				// надо сохранить результат применения трансформа в interface buffer, причем всегда в первый элемент
				// это нужно для того, чтобы эффекты выполнялись по очереди... и ссылки на интерфейсы не копились
				// при этом, надо учитывать то, что у нас результат работы всех трансформов попадает всегда в interface source 
				// а не в result, т.к. некоторые эффекты (типа SetPen) вообще не работают с картинками, они просто конфигурационные...

				return bSuccess;
			}

			virtual BOOL OnProgress(double dPercents)
			{
				if (!m_pEventReceiver)
					return TRUE;

				return m_pEventReceiver->OnProgress(dPercents);
			}

			
			BOOL CheckRaw()
			{
				if( m_pRawFile )
					return TRUE;

				::CoCreateInstance( ImageRaw::CLSID_CImageRaw3, NULL, CLSCTX_INPROC, ImageRaw::IID_IImageRaw3, (void**)&m_pRawFile );
				
				return m_pRawFile != NULL;
			}

			BOOL SaveAsGdiplus( int nSrcId, CString path, int type, const Gdiplus::EncoderParameters* pEncoderSettings = NULL, int dpi = 0,CString sMetaData=_T("") )
			{
				if( m_oSources.IsBadIndex( nSrcId ) )
					return FALSE;

				if( path.GetLength() <= 0 )
					return TRUE;

				CLSID* pClsid = m_oImageCLSID.GetCLSID( type );
				if( !pClsid )
					return FALSE;

				IMediaFrame* pFrame = ResizeAspectFrame( m_oSources.Get( nSrcId ) );
				if( !pFrame )
					return FALSE;

				////////////////////////////////////////////////////////////////////////////
				//meta-data
				CMetaDataSaver oMetaDataSaver;
				oMetaDataSaver.SetXML(sMetaData);
				////////////////////////////////////////////////////////////////////////////

				BYTE* pBGRA = NULL;
				int nWidth = 0;
				int nHeight = 0;
				int nStride = 0;
				BOOL bSuccess = FALSE;
				BOOL bSuccessMetaData = FALSE;
				
				if( Core::c_nErrorNone == Core::COM::COM_GetBGRAImageFromInterface( pFrame, pBGRA, nWidth, nHeight, &nStride ) )
				{
					if( nStride < 0 )
					{
						pBGRA -= (nHeight - 1) * nStride;
					}

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						if( dpi != 0 )
						{
							oImage.SetResolution( (float)dpi, (float)dpi );
						}
						bSuccessMetaData =oMetaDataSaver.SaveToBitmap(&oImage);

						if( Gdiplus::Ok == oImage.Save( CStringW( path ), pClsid, pEncoderSettings ) )
						{
							bSuccess = TRUE;
							
						}
					}
				}

				pFrame->Release();
				
				return bSuccess;
			}

		
			BOOL SaveResultFromVideoCard()
			{
				if( m_nLastSourceInVideoCard < 0 )
					return TRUE;

				IDirect3DTexture9* pTexture = m_oRender.TargetToTexture();
				if( !pTexture )
				{
					m_nLastSourceInVideoCard = -1;
					return FALSE;
				}

				HRESULT hr;
				BOOL bRetCode = m_oSources.SetTexture( m_nLastSourceInVideoCard, pTexture, hr );
				
				pTexture->Release();
				m_nLastSourceInVideoCard = -1;

				if( !bRetCode )
				{
					m_oRender.CheckError( hr );
					return FALSE;
				}

				return TRUE;
			}

			int UpdateVideoEffect( dx9::IEffect* pEffect, int nSrcId, double dCompleteness, BOOL bLocalUseVideoCard )
			{
				
				// сохранение результата предудущей линейки действий на видеокарте
				if( m_nLastSourceInVideoCard != nSrcId )
				{
					if( !SaveResultFromVideoCard() )
						return -1; // откат до последнего удачно выполненого эффекта
				}

				IDirect3DTexture9* pTexture = NULL;

				do
				{
					if( !(m_bUseVideoCard && bLocalUseVideoCard) )
						break;

					// проверка размеров изображения
					if( m_nLastSourceInVideoCard < 0 && 
						m_oSources.GetCountPixels( nSrcId ) < c_nMinImageSizeForDirectX )
					{
						break;
					}

					IDirect3DDevice9* pDevice = m_oRender.IsValid();
					if( !pDevice )
						break;

					int nRetCode = pEffect->Prepare( m_oRender, dCompleteness );
					if( nRetCode > 0 )
						return 1;

					if( nRetCode < 0 )
						break;

					if( m_nLastSourceInVideoCard < 0 )
					{
						HRESULT hr = D3D_OK;
						pTexture = m_oSources.GetTexture( nSrcId, pDevice, hr );
						//m_oRender.SetRenderTargetTexture( pTexture );
						m_oRender.CheckError( hr );
					}
					else
					{
						pTexture = m_oRender.GetRenderTargetTexture();
					}

				}while(0);

				if( pTexture )
				{
					BOOL bRetCode = pEffect->Apply( m_oRender, pTexture );
					pTexture->Release();
					if( bRetCode )
					{
						m_nLastSourceInVideoCard = nSrcId;
						return 1;
					}
				}

				if( !SaveResultFromVideoCard() )
					return -1; // откат до последнего удачно выполненого эффекта

				return 0;
			}

		public:
			
			SerializedObjectTransformer()
			{
				m_pEventReceiver = NULL;

				m_dTime = 0;

				m_pMemoryStorage = NULL;

				m_pRawFile = NULL;

				m_pFileCache = NULL;

				m_bUseVideoCard = 2;

				m_nLastSourceInVideoCard = -1;

				m_nSVGWidth = -1;
				m_nSVGHeight = -1;
			}
			~SerializedObjectTransformer()
			{
				Destroy();

				RELEASEINTERFACE(m_pMemoryStorage);

				RELEASEINTERFACE(m_pRawFile);

				RELEASEINTERFACE(m_pFileCache);
			}
			void Destroy()
			{
				//ClearSources();

				m_oImageCache.RemoveAll();
			}
			
			BOOL SetMediaData(int nIndex, IUnknown* pSource)
			{
				return m_oSources.SetSource( abs(nIndex), pSource, FALSE, (nIndex >= 0) );
			}
			IUnknown* GetMediaData(int nIndex)
			{
				return m_oSources.GetResultFrame( nIndex );
			}
			
			void SetTime(double dTime)
			{
				m_dTime = dTime;
			}
			void SetEventReceiver(SerializedObjectTransformerEvents* pEventReceiver)
			{
				m_pEventReceiver = pEventReceiver;
			}
			void SetMemoryStorage( IMemoryStorage* pMemoryStorage )
			{
				RELEASEINTERFACE(m_pMemoryStorage);

				if( pMemoryStorage )
				{
					pMemoryStorage->AddRef();
					m_pMemoryStorage = pMemoryStorage;
				}
			}
			void SetFileCache( FileCache::IAVSImageFilesCache* pCache )
			{
				RELEASEINTERFACE(m_pFileCache);

				if( pCache )
				{
					pCache->AddRef();
					m_pFileCache = pCache;
				}
			}
			void ResizeSourceBuffer( int size )
			{
				m_oSources.Resize( size );
			}
			
			BOOL Process(ImageStudio::Serialize::SerializedObjectList& oTransforms)
			{
				// в соответствие с выбранной схемой на входе трансформа надо 
				// сделать фиктивное увеличение счетчика ссылок на входной интерфейс
				// в случае отсутствия подтрансформов - это реализует схему IN(1) -> IN(2)
				// а в случае существования подтрансформов - это гарантирует то, что
				// входной интерфейс не удалится в процессе работы трансформера
				
				int nLastSafePoint = -1;
				m_nLastSourceInVideoCard = -1;

				ImageStudio::Serialize::SerializedObject oEmptyObject;

				// обрабатывает последовательно все трансформы
				int nTransformCount = oTransforms.GetCount();
				for (int nTransform = 0; nTransform <= nTransformCount; ++nTransform)
				{
					ImageStudio::Serialize::SerializedObject* pTransform = NULL;
					
					if( nTransform != nTransformCount )
						pTransform = oTransforms[nTransform];
					else
						pTransform = &oEmptyObject;

					if( !pTransform->bUseVideoCard )
					{
						if( !SaveResultFromVideoCard() )
						{
							nTransform = nLastSafePoint;
							continue;
						}

						nLastSafePoint = nTransform;
					}
					
					// return code: 0 - error, 1 - ok, -1 - repeat from last safe point
					int nRetCode = OnTransform( pTransform );
					if( nRetCode < 0 )
					{
						nTransform = nLastSafePoint;
						continue;
					}

					if( m_nLastSourceInVideoCard < 0 )
						nLastSafePoint = nTransform;

					OnProgress(100.0*(nTransform + 1)/(nTransformCount + 1));
				}

				// очищаем кэш по покончании трансформа
				ClearCache(oTransforms);

				// очищаем кеш с текстурами, что бы не копились ссылки
				m_oImageCache.Clear();

				return TRUE;
			}

			static BOOL ResizeMetaFile( IO::CMetaFile* pMetaFile, long nNewWidth, long nNewHeight, long Color, long Type, double dCropZoom, int alpha = 255, int nSrcX = 0, int nSrcY = 0, int nSrcWidth = 0, int nSrcHeight = 0 )
			{
				if( !pMetaFile || nNewWidth <= 0 || nNewHeight <= 0 )
					return FALSE;

				int nWidth  = pMetaFile->GetWidth();
				int nHeight = pMetaFile->GetHeight();

				if( nWidth <= 0 || nHeight <= 0 )
					return FALSE;

				if( nSrcWidth <= 0 )
					nSrcWidth = nWidth;

				if( nSrcHeight <= 0 )
					nSrcHeight = nHeight;

				if( nSrcX != 0 || nSrcY != 0 || nSrcWidth != nWidth || nSrcHeight != nHeight )
				{
					Gdiplus::Rect oSrcRect( 0, 0, nWidth, nHeight );
					Gdiplus::Rect oDstRect( nSrcX, nSrcY, nSrcWidth, nSrcHeight );

					if( !oDstRect.Intersect( oSrcRect ) )
						return FALSE;

					double dScaleX = double(nWidth ) / oDstRect.Width;
					double dScaleY = double(nHeight) / oDstRect.Height;
					double dX = -(oDstRect.X + (oDstRect.Width  - 1) * 0.5) * dScaleX + (nWidth  - 1) * 0.5;
					double dY = -(oDstRect.Y + (oDstRect.Height - 1) * 0.5) * dScaleY + (nHeight - 1) * 0.5;

					double m[6] = {dScaleX, 0, 0, dScaleY, dX, dY};
					pMetaFile->SetTransform( m );
				}
				
				pMetaFile->SetSize( nNewWidth, nNewHeight );
				
				if( Type == c_nResizeStretchNearest || Type == c_nResizeStretchBillinear || Type == c_nResizeStretchBicubic || Type == c_nResizeStretchSuper )
				{
				}
				else
				if( Type == c_nResizeShrinkNearest || Type == c_nResizeShrinkBillinear || Type == c_nResizeShrinkBicubic || Type == c_nResizeShrinkSuper )
				{
					double dScaleX = double(nNewWidth)  / nWidth;
					double dScaleY = double(nNewHeight) / nHeight;
					double dScale  = min(dScaleX, dScaleY);

					double dW = dScale * nWidth;
					double dH = dScale * nHeight;

					dScaleX = dW / nNewWidth;
					dScaleY = dH / nNewHeight;

					double m[6] = {
						dScaleX, 
						0, 
						0, 
						dScaleY, 
						(nNewWidth  - 1) / 2 * (1 - dScaleX),
						(nNewHeight - 1) / 2 * (1 - dScaleY)
					};

					pMetaFile->AddTransform( m );
				}
				else
				if( Type == c_nResizeCropNearest || Type == c_nResizeCropBillinear || Type == c_nResizeCropBicubic || Type == c_nResizeCropSuper )
				{
					double dScaleX = double(nNewWidth)  / nWidth;
					double dScaleY = double(nNewHeight) / nHeight;
					
					double dMinScale = min(dScaleX, dScaleY);
					double dMaxScale = max(dScaleX, dScaleY);
					double dScale = (dMinScale + (dMaxScale - dMinScale) * dCropZoom);

					double dW = dScale * nWidth;
					double dH = dScale * nHeight;

					dScaleX = dW / nNewWidth;
					dScaleY = dH / nNewHeight;

					double m[6] = {
						dScaleX, 
						0, 
						0, 
						dScaleY, 
						(nNewWidth  - 1) / 2 * (1 - dScaleX),
						(nNewHeight - 1) / 2 * (1 - dScaleY)
					};

					pMetaFile->AddTransform( m );
				}

				return TRUE;
			}

			long m_nSVGWidth, m_nSVGHeight;
			BOOL LoadSVGFromXML( BSTR sXML )
			{
				ImageStudio::IO::CMetaFile svg;
				if( svg.LoadSVGFromXML( sXML ) )
				{
					svg.SetSize( m_nSVGWidth, m_nSVGHeight );

					ImageStudio::IO::CMetaFile* pSVG = new ImageStudio::IO::CMetaFile( svg );
					if( pSVG )
					{
						if( m_oSources.SetMetafile( 0, pSVG ) )
							return TRUE;
						
						delete pSVG;
					}
				}

				return FALSE;

			}
		};

	}
}
