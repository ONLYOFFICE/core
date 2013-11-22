#pragma once
#include "resource.h"       // main symbols
#include "ASCUncompressedVideoFrame.h"
#include "ASCVideoFrameTransform.h" 
#include <math.h>

#import "../Redist/ASCMediaFormatSettings3.dll"	named_guids rename_namespace("MediaFormat"), exclude("tagRECT")
typedef IAVSUncompressedVideoFrame IMediaFrame;

class CDrawLogo
{
public:
	class MediaTransform
	{
	public:

		MediaTransform()
		{
			m_pFormat = NULL;
			m_pTransform = NULL;
		}
		~MediaTransform()
		{
			if (m_pFormat) m_pFormat->Release();
			if (m_pTransform) m_pTransform->Release();
		}

		MediaTransform( const MediaTransform& out )
		{
			m_pFormat = out.m_pFormat;
			m_pTransform = out.m_pTransform;

			if( m_pFormat )	m_pFormat->AddRef();
			if( m_pTransform ) m_pTransform->AddRef();
		}

		MediaTransform& operator = (const MediaTransform& out)
		{
			if( m_pFormat ) m_pFormat->Release();
			if( m_pTransform ) m_pTransform->Release();

			m_pFormat = out.m_pFormat;
			m_pTransform = out.m_pTransform;

			if( m_pFormat )	m_pFormat->AddRef();
			if( m_pTransform ) m_pTransform->AddRef();

			return *this;
		}

		void Clear()
		{
			if (m_pFormat) 
			{
				m_pFormat->Release();
				m_pFormat = NULL;
			}
			if (m_pTransform) 
			{
				m_pTransform->Release();
				m_pTransform = NULL;
			}
		}


		IMediaFrame* Convert( IMediaFrame* pSource, int nOutColorSpace )
		{
			if( !pSource )
				return NULL;

			if( !CheckInterfaces() )
				return NULL;

			// настраиваем формат для конвертации
			long lWidth;
			long lHeight;
			long lColorSpace;
			long lAspectX;
			long lAspectY;

			pSource->get_Width(&lWidth);
			pSource->get_Height(&lHeight);
			pSource->get_ColorSpace(&lColorSpace);
			pSource->get_AspectRatioX(&lAspectX);
			pSource->get_AspectRatioY(&lAspectY);

			//m_pFormat->SetDefaultProperties();
			//m_pFormat->put_ColorSpace(CSP_BGRA | CSP_VFLIP);

			m_pFormat->put_Width(lWidth);
			m_pFormat->put_Height(lHeight);
			m_pFormat->put_ColorSpace(nOutColorSpace);
			m_pFormat->put_AspectRatioX(lAspectX);
			m_pFormat->put_AspectRatioY(lAspectY);

			m_pTransform->SetVideoFormat(m_pFormat);

			// собственно конвертация
			IMediaFrame* pResult = NULL;
			m_pTransform->TransformFrame(pSource, (IUnknown**)&pResult);

			return pResult;
		}

	protected:
		
		BOOL CheckInterfaces()
		{
			// пытаемся создать интерфейс трансформера
			if (!m_pTransform)
			{
				CoCreateInstance(__uuidof(CAVSVideoFrameTransform), NULL, CLSCTX_ALL, __uuidof (IAVSVideoFrameTransform), (void**)(&m_pTransform));
				if( !m_pTransform )		
					return FALSE;
			}

			// пытаемся создать интерфейс формата
			if (!m_pFormat)
			{
				CoCreateInstance(MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)(&m_pFormat));
				if( !m_pFormat )
					return FALSE;
			}

			return TRUE;
		}
		
	protected:
		
		MediaFormat::IAVSVideoFormat* m_pFormat;			// интерфейс, хранящий настройки формата BGR32
		IAVSVideoFrameTransform* m_pTransform;	// интерфейс, необходимый для конвертации media data в формат BGR32
	};



protected:
	static const int MAX_PLANES = 4;

	class TRect
	{
	public:
		int x, y;
		int w, h;

	public:
		TRect()
		{
			Clear();
		}
		TRect( int nX, int nY, int nWidth, int nHeight )
		{
			x = nX;
			y = nY;
			w = nWidth;
			h = nHeight;
		}
		void Clear()
		{
			x = 0;
			y = 0;
			w = 0;
			h = 0;
		}

		
		static bool Intersect( const TRect& in1, const TRect& in2, TRect* out )
		{
			int nMinX = max(in1.x, in2.x);
			int nMinY = max(in1.y, in2.y);
			int nMaxX = min(in1.x + in1.w, in2.x + in2.w);
			int nMaxY = min(in1.y + in1.h, in2.y + in2.h);

			bool result = (nMaxX > nMinX) | (nMaxY > nMinY );

			if( out )
			{
				if( result )
				{
					out->x = nMinX;
					out->y = nMinY;
					out->w = nMaxX - nMinX;
					out->h = nMaxY - nMinY;
				}
				else
				{
					out->x = 0;
					out->y = 0;
					out->w = 0;
					out->h = 0;
				}
			}
			
			return result;
		}

		bool Intersect( const TRect& rect )
		{
			return Intersect( rect, *this, this );
		}
	};

	class TColorSpaceInfo
	{
		BYTE m_nCountPlanes;
		BYTE m_nMaxShrinkX;
		BYTE m_nMaxShrinkY;
		BYTE m_bUsingAlpha;

		BYTE m_nBytesPerSample[MAX_PLANES];

		BYTE m_nShrinkX[MAX_PLANES];
		BYTE m_nShrinkY[MAX_PLANES];

	public:
		TColorSpaceInfo()
		{
			Clear();
		}

		TColorSpaceInfo( int nColorSpace )
		{
			Load( nColorSpace );
		}
		TColorSpaceInfo( IMediaFrame* pFrame )
		{
			Load( pFrame );
		}


		void Clear()
		{
			memset( this, 0, sizeof *this );
		}

		
		void Load( long nColorSpace )
		{
			Clear();

			switch( nColorSpace & CSP_COLOR_MASK )
			{
			case CSP_PLANAR:
			case CSP_I420:
			case CSP_YV12:
				m_nCountPlanes = 3;
				m_nMaxShrinkX = 1;
				m_nMaxShrinkY = 1;
				m_nBytesPerSample[0] = 1;
				m_nBytesPerSample[1] = 1;
				m_nBytesPerSample[2] = 1;
				m_nShrinkX[1] = 1;
				m_nShrinkX[2] = 1;
				m_nShrinkY[1] = 1;
				m_nShrinkY[2] = 1;
				break;

			case CSP_YUY2:
			case CSP_UYVY:
			case CSP_YVYU:
				m_nCountPlanes = 1;
				m_nMaxShrinkX = 1;
				m_nBytesPerSample[0] = 4;
				m_nShrinkX[0] = 1;
				break;
			
			case CSP_BGRA:
			case CSP_ABGR:
			case CSP_RGBA:
			case CSP_ARGB:
				m_nCountPlanes = 1;
				m_bUsingAlpha = 1;
				m_nBytesPerSample[0] = 4;
				break;

			case CSP_BGR:
				m_nCountPlanes = 1;
				m_nBytesPerSample[0] = 3;
				break;

			case CSP_RGB555:
			case CSP_RGB565:
				m_nCountPlanes = 1;
				m_nBytesPerSample[0] = 2;
				break;
			}
		}

		void Load( IMediaFrame* pFrame )
		{
			if( !pFrame )
			{
				Clear();
				return;
			}

			long lColorSpace; 
			pFrame->get_ColorSpace( &lColorSpace );
			Load( lColorSpace );
		}

		
		int GetCountPlanes() const
		{
			return m_nCountPlanes;
		}

		BOOL IsUsingAlpha() const
		{
			return m_bUsingAlpha;
		}
		
		int AlignX( int pos ) const
		{
			return (pos >> m_nMaxShrinkX) << m_nMaxShrinkX;
		}

		int AlignY( int pos ) const
		{
			return (pos >> m_nMaxShrinkY) << m_nMaxShrinkY;
		}

		int AlignWidth( int width ) const
		{
			return AlignX( width + (1L << m_nMaxShrinkX) - 1 );
		}

		int AlignHeight( int height ) const
		{
			return AlignY( height + (1L << m_nMaxShrinkY) - 1 );
		}

		int ShrinkX( int plane, int pos ) const
		{
			return pos >> m_nShrinkX[plane];
		}

		int ShrinkY( int plane, int pos ) const
		{
			return pos >> m_nShrinkY[plane];
		}

		int GetLenSamples( int plane, int samples ) const
		{
			return samples * m_nBytesPerSample[plane];
		}
	};

	class TImageInfo
	{
		BYTE* m_pBuffer[MAX_PLANES];
		long  m_nStride[MAX_PLANES];

		long m_nWidth;
		long m_nHeight;
		long m_nColorSpace;
		BOOL m_bValid;

		TColorSpaceInfo m_CspInfo;

	public:
		TImageInfo()
		{
			Clear();
		}

		TImageInfo( IMediaFrame* pFrame )
		{
			Load( pFrame );
		}

		void Clear()
		{
			for( int i = 0; i < MAX_PLANES; i++ )
			{
				m_pBuffer[i] = NULL;
				m_nStride[i] = 0;
			}

			m_nWidth = 0;
			m_nHeight = 0;
			m_nColorSpace = 0;
			m_bValid = FALSE;

			m_CspInfo.Clear();
		}

		
		void Load( IMediaFrame* pFrame )
		{
			if( !pFrame )
			{
				Clear();
				return;
			}

			for( int i = 0; i < MAX_PLANES; i++ )
			{
				pFrame->get_Plane( i, &m_pBuffer[i] );
				pFrame->get_Stride( i, &m_nStride[i] );
			}

			pFrame->get_Width( &m_nWidth );
			pFrame->get_Height( &m_nHeight );
			pFrame->get_ColorSpace( &m_nColorSpace );

			m_CspInfo.Load( m_nColorSpace );

			m_nWidth  = m_CspInfo.AlignX( m_nWidth );
			m_nHeight = m_CspInfo.AlignY( m_nHeight );

			m_bValid = (
				m_pBuffer[0] && 
				m_CspInfo.GetCountPlanes() > 0 &&
				m_nWidth > 0 && m_nHeight > 0) ? TRUE : FALSE;
		}

		BOOL CutRect( const TRect& SrcRect, TRect& CutRect, TRect& BoundRect ) const
		{
			CutRect.x = 0;
			CutRect.y = 0;
			CutRect.w = m_nWidth;
			CutRect.h = m_nHeight;

			if( !CutRect.Intersect( SrcRect ) )
				return FALSE;

			BoundRect.x = m_CspInfo.AlignX( CutRect.x );
			BoundRect.y = m_CspInfo.AlignY( CutRect.y );
			BoundRect.w = m_CspInfo.AlignWidth( CutRect.x + CutRect.w ) - BoundRect.x;
			BoundRect.h = m_CspInfo.AlignHeight( CutRect.y + CutRect.h ) - BoundRect.y;

			return TRUE;
		}


		long GetStride( int plane ) const
		{
			return m_nStride[plane];
		}

		BYTE* GetBuffer( int plane ) const
		{
			return m_pBuffer[plane];
		}

		BYTE* GetBufferOffset( int plane, const TRect& rect ) const
		{
			int x = m_CspInfo.ShrinkX( plane, rect.x );
			int y = m_CspInfo.ShrinkY( plane, IsFlipY() ? (m_nHeight - rect.y - rect.h) : rect.y );

			return m_pBuffer[plane] + y * m_nStride[plane] + m_CspInfo.GetLenSamples( plane, x );
		}

		BOOL IsValid() const
		{
			return m_bValid;
		}
		BOOL IsFlipY() const
		{
			return m_nColorSpace < 0;
		}
		BOOL IsBGR() const
		{
			return (m_nColorSpace & CSP_BGR) == CSP_BGR;
		}
		BOOL IsBGRA() const
		{
			return (m_nColorSpace & CSP_BGRA) == CSP_BGRA;
		}
		
		int GetWidth() const
		{
			return m_nWidth;
		}

		int GetHeight() const
		{
			return m_nHeight;
		}
		int GetColorSpace() const
		{
			return m_nColorSpace;
		}
		int GetRowLength( int plane, int width ) const
		{
			return m_CspInfo.GetLenSamples( plane, m_CspInfo.ShrinkX( plane, width ) );
		}
		int GetRowCount( int plane, int height ) const
		{
			return m_CspInfo.ShrinkY( plane, height );
		}

	};


	struct TBgraData
	{
		BYTE* m_pBuffer;
		long  m_nWidth;
		long  m_nHeight;
		long  m_nStride;
	};


protected:
	IMediaFrame* m_pLogoFrame;
	
	MediaTransform m_oTransform;

protected:
	static BOOL UnpackBgraFrame( IMediaFrame* pFrame, TBgraData& data )
	{
		long lColorSpace; pFrame->get_ColorSpace( &lColorSpace );
		if( (lColorSpace & CSP_BGRA) != CSP_BGRA )
			return FALSE;

		pFrame->get_Plane( 0, &data.m_pBuffer );
		pFrame->get_Width( &data.m_nWidth );
		pFrame->get_Height( &data.m_nHeight );
		pFrame->get_Stride( 0, &data.m_nStride );

		if( !data.m_pBuffer || 
			data.m_nWidth <= 0 || 
			data.m_nHeight <= 0 ||
			abs(data.m_nStride) < (data.m_nWidth * 4))
		{
			return FALSE;
		}

		if( lColorSpace < 0 )
		{
			data.m_pBuffer += (data.m_nHeight - 1) * data.m_nStride;
			data.m_nStride = -data.m_nStride;
		}

		return TRUE;
	}

	static void BlendRowBgr( BYTE* dst, BYTE* src, int width )
	{
		for( int i = 0; i < width; ++i, dst += 3, src += 4 )
		{
			DWORD src_alpha = src[3];
			if( !src_alpha )
				continue;

			dst[0] = (BYTE)((dst[0] * (255 - src_alpha) + src[0] * src_alpha + 127) * 0x8081U >> 23);
			dst[1] = (BYTE)((dst[1] * (255 - src_alpha) + src[1] * src_alpha + 127) * 0x8081U >> 23);
			dst[2] = (BYTE)((dst[2] * (255 - src_alpha) + src[2] * src_alpha + 127) * 0x8081U >> 23);
		}
	}

	static void BlendRowBgra( BYTE* dst, BYTE* src, int width )
	{
		for( int i = 0; i < width; ++i, dst += 4, src += 4 )
		{
			DWORD src_alpha = src[3];

			//src_alpha = i % 255;	// test

			if( !src_alpha )
				continue;

			if( src_alpha > 128 )
				int a = 0;

			DWORD dst_alpha = (dst[3] * (255 - src_alpha) + 127) * 0x8081U >> 23;

			dst[0] = (BYTE)((dst[0] * dst_alpha + src[0] * src_alpha + 127) * 0x8081U >> 23);
			dst[1] = (BYTE)((dst[1] * dst_alpha + src[1] * src_alpha + 127) * 0x8081U >> 23);
			dst[2] = (BYTE)((dst[2] * dst_alpha + src[2] * src_alpha + 127) * 0x8081U >> 23);
		}
	}


public:
	CDrawLogo()
	{
		m_pLogoFrame = NULL;
	}

	~CDrawLogo()
	{
		if( m_pLogoFrame ) 
			m_pLogoFrame->Release();
	}

	CDrawLogo( const CDrawLogo& out ) 
		: m_oTransform( out.m_oTransform )
	{
		m_pLogoFrame = out.m_pLogoFrame;
		if( m_pLogoFrame )
			m_pLogoFrame->AddRef();
	}

	CDrawLogo& operator = (const CDrawLogo& out)
	{
		m_oTransform = out.m_oTransform;

		m_pLogoFrame = out.m_pLogoFrame;
		if( m_pLogoFrame )
			m_pLogoFrame->AddRef();

		return *this;
	}

	
	void Clear()
	{
		if( m_pLogoFrame ) 
		{
			m_pLogoFrame->Release();
			m_pLogoFrame = NULL;
		}

		m_oTransform.Clear();
	}

	
	BOOL SetLogo( IMediaFrame* pFrame )
	{
		if( m_pLogoFrame )
		{
			m_pLogoFrame->Release();
			m_pLogoFrame = NULL;
		}
		
		if( !pFrame )
			return FALSE;

		TBgraData bgra;
		if( !UnpackBgraFrame( pFrame, bgra ) )
		{
			m_pLogoFrame = m_oTransform.Convert( pFrame, CSP_BGRA );
			if( !m_pLogoFrame )
				return FALSE;
		}
		else
		{
			m_pLogoFrame = pFrame;
			m_pLogoFrame->AddRef();
		}

		return TRUE;
	}

	void DrawLogo( IMediaFrame* pCanvas, int nX, int nY )
	{
		if( !pCanvas || !m_pLogoFrame )
			return;

		TBgraData LogoData;
		if( !UnpackBgraFrame( m_pLogoFrame, LogoData ) )
		{
			return;
		}

		TImageInfo SrcInfo( pCanvas );
		if( !SrcInfo.IsValid() )
			return;

		TRect CutRect;
		TRect BoundRect;

		if( !SrcInfo.CutRect( TRect( nX, nY, LogoData.m_nWidth, LogoData.m_nHeight ), CutRect, BoundRect ) )
			return;

		if( SrcInfo.IsBGR() )
		{
			BYTE* pDstBuffer = SrcInfo.GetBuffer( 0 );
			long  lDstStride = SrcInfo.GetStride( 0 );

			if( SrcInfo.IsFlipY() )
			{
				pDstBuffer += CutRect.x * 3 + (SrcInfo.GetHeight() - 1 - CutRect.y) * lDstStride;
				lDstStride = -lDstStride;
			}
			else
			{
				pDstBuffer += CutRect.x * 3 + CutRect.y * lDstStride;
			}

			BYTE* pSrcBuffer = LogoData.m_pBuffer + (CutRect.x - nX) * 4 + (CutRect.y - nY) * LogoData.m_nStride;
			long lSrcStride = LogoData.m_nStride;

			for( int row = 0; row < CutRect.h; ++row, pSrcBuffer += lSrcStride, pDstBuffer += lDstStride )
			{
				BlendRowBgr( pDstBuffer, pSrcBuffer, CutRect.w );
			}

			return;
		}

		if( SrcInfo.IsBGRA() )
		{
			BYTE* pDstBuffer = SrcInfo.GetBuffer( 0 );
			long  lDstStride = SrcInfo.GetStride( 0 );

			if( SrcInfo.IsFlipY() )
			{
				pDstBuffer += CutRect.x * 4 + (SrcInfo.GetHeight() - 1 - CutRect.y) * lDstStride;
				lDstStride = -lDstStride;
			}
			else
			{
				pDstBuffer += CutRect.x * 4 + CutRect.y * lDstStride;
			}

			BYTE* pSrcBuffer = LogoData.m_pBuffer + (CutRect.x - nX) * 4 + (CutRect.y - nY) * LogoData.m_nStride;
			long lSrcStride = LogoData.m_nStride;

			for( int row = 0; row < CutRect.h; ++row, pSrcBuffer += lSrcStride, pDstBuffer += lDstStride )
			{
				BlendRowBgra( pDstBuffer, pSrcBuffer, CutRect.w );
			}

			return;
		}
		
		IMediaFrame* pFrame = NULL;
		CoCreateInstance( __uuidof(CAVSUncompressedVideoFrame), NULL, CLSCTX_ALL, __uuidof(IAVSUncompressedVideoFrame), (void**)(&pFrame) );
		if( !pFrame )
			return;

		pFrame->put_Width( BoundRect.w );
		pFrame->put_Height( BoundRect.h );
		pFrame->put_ColorSpace( SrcInfo.GetColorSpace() );

		for( int i = 0; i < MAX_PLANES; i++ )
		{
			pFrame->put_Plane( i, SrcInfo.GetBufferOffset( i, BoundRect ) );
			pFrame->put_Stride( i, SrcInfo.GetStride( i ) );
		}

		MediaTransform oFrameTransform;
		IMediaFrame* pDstFrame = oFrameTransform.Convert( pFrame, CSP_BGR );
		if( pDstFrame )
		{
			BYTE* pDstBuffer; pDstFrame->get_Plane( 0, &pDstBuffer );
			long  lDstStride; pDstFrame->get_Stride( 0, &lDstStride );

			if( CutRect.x > BoundRect.x )
			{
				pDstBuffer += (CutRect.x - BoundRect.x) * 3;
			}
			if( CutRect.y > BoundRect.y )
			{
				pDstBuffer += (CutRect.y - BoundRect.y) * lDstStride;
			}

			BYTE* pSrcBuffer = LogoData.m_pBuffer + (CutRect.x - nX) * 4 + (CutRect.y - nY) * LogoData.m_nStride;
			long lSrcStride = LogoData.m_nStride;

			for( int row = 0; row < CutRect.h; ++row, pSrcBuffer += lSrcStride, pDstBuffer += lDstStride )
			{
				BlendRowBgr( pDstBuffer, pSrcBuffer, CutRect.w );
			}

			IMediaFrame* pTmpFrame = oFrameTransform.Convert( pDstFrame, SrcInfo.GetColorSpace() );
			if( pTmpFrame )
			{
				for( int i = 0; i < MAX_PLANES; i++ )
				{
					BYTE* pSrcPlane; pTmpFrame->get_Plane( i, &pSrcPlane );
					long  lSrcPitch; pTmpFrame->get_Stride( i, &lSrcPitch );

					if( !pSrcPlane || !lSrcPitch )
						break;
					
					BYTE* pDstPlane; pFrame->get_Plane( i, &pDstPlane );
					long  lDstPitch; pFrame->get_Stride( i, &lDstPitch );

					long lRowLength = SrcInfo.GetRowLength( i, BoundRect.w );
					long lRowCount = SrcInfo.GetRowCount( i, BoundRect.h );

					for( int row = 0; row < lRowCount; ++row, pSrcPlane += lSrcPitch, pDstPlane += lDstPitch )
					{
						memcpy( pDstPlane, pSrcPlane, lRowLength );
					}
				}
				
				pTmpFrame->Release();
			}

			pDstFrame->Release();
		}

		for( int i = 0; i < MAX_PLANES; i++ )
		{
			pFrame->put_Plane( i, NULL );
		}
		pFrame->Release();
	}
};
