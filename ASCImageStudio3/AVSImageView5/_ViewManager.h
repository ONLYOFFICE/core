#pragma once

#include "x:\Common\ATLDefine.h"
//#include "x:\Common\WindowsEx.h"
#include "x:\Common\GdiPlusEx.h"
#include <atlutil.h>

#include <Wingdi.h>
#include <windowsx.h>
#include <excpt.h> 
#include <assert.h>

struct ICoordinatSystem
{
	virtual void ScreenPointToImagePoint			( long& X, long& Y )		=	0;
	virtual void ImagePointToScreenPoint			( long& X, long& Y )		=	0;
	virtual void ScreenDistanceToImageDistance		( long& Distance )			=	0;
	virtual void ImageDistanceToScreenDistance		( long& Distance )			=	0; 
};

namespace ImageRendering
{
	namespace Constants
	{
		// рабочее состояние
		static const long	sc_nRefreshForNone										=	-1;
		// нужно перестроить полностью всю картинку для отображения ( zoom, изменения позиций отрисовки )
		static const long	sc_nRefreshForRebuild									=	0;
		// копируем слой сохраненный (рисуется трек или какое то действие не оставляет после себя изменений в картинке)	
		static const long	sc_nRefreshForCommand									=	1;
		// перестраиваем только нужный нам участок ( TODO : оптимизировать операции )
		static const long	sc_nRefreshForRegionRebuild								=	2;

		// режим когда идет отображение слоя прозрачности в виде шахматой доски
		static const unsigned long	sc_RenderModeUseAlphaLayer						=	0x00000001;
		// режим в котором используется дублирующий слой который копируется каждый раз когда нам нужно иметь возможность рисования без записи данных в картинку
		static const unsigned long	sc_RenderModeUseCopyImage						=	0x00000002;
		// использовать class - ImageSurface заранее просчитаны смещение для пикселей по X ( в этом случае не доступен плавный зум, если таковой используется в клиенте )
		static const unsigned long	sc_RenderModeEnableImageSurface					=	0x00000008;
		// для изображений который при масшатабе 1:1 меньша области видимости, выставляется кламп для ZoomToFit не больше 1:1
		static const unsigned long	sc_RenderModeDisabledZoomToFitForSmallImages	=	0x00000016;
		// фон черный, пиксели с альфой будут черными ( режим для картинок с ALPHA = 255 )
		static const unsigned long	sc_RenderModeFastCopyBuffersWithBlackFone		=	0x00000064;

		static const float	sc_nRenderGridMinSizeCell								=	10.0f;		// минимальное значение для ячейки
	}

	namespace NViewSettingsConstants
	{
		const static CComBSTR	g_csViewSettings_Zoom					=	CComBSTR ( L"Zoom" );					
		const static CComBSTR	g_csViewSettings_CenterX				=	CComBSTR ( L"CenterX" );				
		const static CComBSTR	g_csViewSettings_CenterY				=	CComBSTR ( L"CenterY" );				
		const static CComBSTR	g_csViewSettings_EnableRullerUp			=	CComBSTR ( L"EnableRullerUp" );			
		const static CComBSTR	g_csViewSettings_EnableRullerDown		=	CComBSTR ( L"EnableRullerDown" );		
		const static CComBSTR	g_csViewSettings_EnableRullerLeft		=	CComBSTR ( L"EnableRullerLeft" );		
		const static CComBSTR	g_csViewSettings_EnableRullerRight		=	CComBSTR ( L"EnableRullerRight" );		
		const static CComBSTR	g_csViewSettings_ImageWidth				=	CComBSTR ( L"ImageWidth" );				
		const static CComBSTR	g_csViewSettings_ImageHeight			=	CComBSTR ( L"ImageHeight" );			
		const static CComBSTR	g_csViewSettings_PreviewLeft			=	CComBSTR ( L"PreviewLeft" );			
		const static CComBSTR	g_csViewSettings_PreviewRight			=	CComBSTR ( L"PreviewRight" );			
		const static CComBSTR	g_csViewSettings_PreviewTop				=	CComBSTR ( L"PreviewTop" );				
		const static CComBSTR	g_csViewSettings_PreviewBottom			=	CComBSTR ( L"PreviewBottom" );			
		const static CComBSTR	g_csViewSettings_BackClearColor			=	CComBSTR ( L"BackClearColor" );			
		const static CComBSTR	g_csViewSettings_EnableGrid				=	CComBSTR ( L"EnableGid" );			
		const static CComBSTR	g_csViewSettings_RullersType			=	CComBSTR ( L"RullersType" );			
		const static CComBSTR	g_csViewSettings_ZoomToFitOffsetPX		=	CComBSTR ( L"ZoomToFitOffsetPX" );			
		const static CComBSTR	g_csViewSettings_RenderMode				=	CComBSTR ( L"RenderMode" );			

		const static long		g_cnViewSettings_Zoom					=	10000;
		const static long		g_cnViewSettings_CenterX				=	10001;
		const static long		g_cnViewSettings_CenterY				=	10002;
		const static long		g_cnViewSettings_EnableRullerUp			=	10003;
		const static long		g_cnViewSettings_EnableRullerDown		=	10004;
		const static long		g_cnViewSettings_EnableRullerLeft		=	10005;
		const static long		g_cnViewSettings_EnableRullerRight		=	10006;
		const static long		g_cnViewSettings_ImageWidth				=	10007;
		const static long		g_cnViewSettings_ImageHeight			=	10008;
		const static long		g_cnViewSettings_PreviewLeft			=	10009;
		const static long		g_cnViewSettings_PreviewRight			=	10010;
		const static long		g_cnViewSettings_PreviewTop				=	10011;
		const static long		g_cnViewSettings_PreviewBottom			=	10012;
		const static long		g_cnViewSettings_BackClearColor			=	10013;
		const static long		g_cnViewSettings_EnableGrid				=	10014;
		const static long		g_cnViewSettings_RullersType			=	10015;
		const static long		g_cnViewSettings_ZoomToFitOffsetPX		=	10016;
		const static long		g_cnViewSettings_RenderMode				=	10017;
	}
}

namespace ImageRendering
{
	namespace ImageUtils
	{
		inline static void GetEncoderCLSID ( const WCHAR* pFormat, CLSID* pClsid )
		{
			// variables
			UINT nEncoders = 0;
			UINT nSize = 0;
			Gdiplus::ImageCodecInfo* pImageCodecInfo = 0;

			// retrieve encoders info
			Gdiplus::GetImageEncodersSize(&nEncoders, &nSize);

			// check for valid encoders
			if (!nSize)
				throw 0;

			// create encoders info structure of necessary size
			pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(nSize));

			// check for valid encoder
			if (!pImageCodecInfo)
				throw 0;

			// retrieve all encoders
			Gdiplus::GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

			// locate necessary encoder
			for (UINT nEncoder = 0; nEncoder < nEncoders; ++nEncoder)
			{
				// compare MIME strings
				if ( CStringW ( pImageCodecInfo[nEncoder].MimeType ) ==  CStringW ( pFormat ) )
				{
					// save CLSID
					*pClsid = pImageCodecInfo[nEncoder].Clsid;

					// clear memory
					free(pImageCodecInfo);

					// all ok
					return;
				}    
			}

			// clear memory
			free(pImageCodecInfo);

			// codec not found
			throw 0;
		}

		inline static void SaveImageAsPNG ( const WCHAR* pName, Bitmap* pBitmap )
		{
			if ( NULL != pName && NULL != pBitmap )
			{
				CLSID ImageGUID;
				ImageRendering::ImageUtils::GetEncoderCLSID ( L"image/png", &ImageGUID );
				pBitmap->Save ( pName, &ImageGUID );
			}
		}
		inline static bool MemCopyBitmap ( Gdiplus::Bitmap* pSource, Gdiplus::Bitmap* pBitmap )
		{
			if ( NULL == pSource || NULL == pBitmap )
				return false;

			if ( pSource->GetPixelFormat () != pBitmap->GetPixelFormat () )
				return false;

			if ( pSource->GetWidth () != pBitmap->GetWidth () || pSource->GetHeight () != pBitmap->GetHeight () )
				return false;

			BitmapData oBitmapData; 
			BitmapData oBitmapData2; 

			if ( Gdiplus::Ok == pSource->LockBits ( NULL, ImageLockModeRead, pSource->GetPixelFormat (), &oBitmapData ) )
			{
				if ( Gdiplus::Ok == pBitmap->LockBits ( NULL, ImageLockModeWrite, pBitmap->GetPixelFormat (), &oBitmapData2 ) )
				{
					//memmove ( oBitmapData2.Scan0, oBitmapData.Scan0, oBitmapData.Stride * pSource->GetHeight() );
					memcpy ( oBitmapData2.Scan0, oBitmapData.Scan0, oBitmapData.Stride * pSource->GetHeight() );

					pBitmap->UnlockBits ( &oBitmapData2 );
				}

				pSource->UnlockBits ( &oBitmapData );
			}

			return true;
		}
	}

	namespace PixelsUtils
	{
		static const long sc_nInterpolationModeNone				= -1;
		static const long sc_nInterpolationModeNearestNeighbor	= 0;
		static const long sc_nInterpolationModeBilinear			= 1;

		class ImageSurface 
		{
		public:

			ImageSurface ( bool ThreadMode = false )
			{
				m_nSrcX			=	0;
				m_nSrcY			=	0;

				m_nLookUpScale	=	0.0;

				m_pLookUpX		=	NULL;
				m_pLookUpY		=	NULL;
			
				
				m_bThreadMode	=	ThreadMode;

				if ( m_bThreadMode )
				{
					m_bThreadActivate [0]	=	false;
					m_bThreadActivate [1]	=	false;
					
					m_bThreadClose [0]		=	false;
					m_bThreadClose [1]		=	false;

					DWORD dwTemp;
					
					m_hThread [0]  = ::CreateThread ( NULL, 0, &ThreadFunc, (void*)this, 0, &dwTemp );
					SetThreadAffinityMask ( m_hThread [0], 1 );
				
					m_hThread [1]  = ::CreateThread ( NULL, 0, &ThreadFunc2, (void*)this, 0, &dwTemp );
					SetThreadAffinityMask ( m_hThread [1], 2 );

					m_hEvents [ 0 ] = CreateEvent  ( NULL, TRUE, 0, NULL );
					m_hEvents [ 1 ] = CreateEvent  ( NULL, TRUE, 0, NULL );
				}
			}

			virtual ~ImageSurface ( )
			{
				if ( NULL != m_pLookUpX	)
					delete m_pLookUpX;

				if ( NULL != m_pLookUpY	)
					delete m_pLookUpY;
			
				if ( m_bThreadMode )
				{
					m_bThreadClose [0]	=	true;
					m_bThreadClose [1]	=	true;

					WaitForMultipleObjectsEx ( 2, m_hThread, TRUE, INFINITE, FALSE );
					
					CloseHandle (  m_hThread [0] );
					CloseHandle (  m_hThread [1] );
					CloseHandle (  m_hEvents [0] );
					CloseHandle (  m_hEvents [1] );
				}
			}

			
			inline void UpdateResize ( 
				DWORD* pImage, DWORD* pArea,
				long ImageW, long ImageH,					//	размеры исходной картинки
				long AreaW, long AreaH,						//	размеры области куда будет записывать данные
				long ImageOffsetX, long ImageOffsetY,		//	смещение в исходной картинке
				long AreaOffsetX, long  AreaOffsetY,		//	смещение в области которой пишем
				long AreaBeginX, long AreaEndX,				//	
				long AreaBeginY, long AreaEndY,
				double Scale
				)
			{
				UpdateLookUp ( __max ( ImageW, AreaW ), AreaEndX - AreaBeginX, Scale );

				m_nLookUpScale			=	Scale;

				m_pImage				=	pImage;
				m_pArea					=	pArea;

				m_nAreaW				=	AreaW;
				m_nAreaH				=	AreaH;

				m_nAreaBeginX			=	AreaBeginX;
				m_nAreaEndX				=	AreaEndX;
				m_nAreaBeginY			=	AreaBeginY;
				m_nAreaEndY				=	AreaEndY;
				
				m_nImageW				=	ImageW;
				m_nImageH				=	ImageH;		
				m_nImageOffsetX			=	ImageOffsetX;
				m_nImageOffsetY			=	ImageOffsetY;		
				m_nAreaOffsetX			=	AreaOffsetX;
				m_nAreaOffsetY			=	AreaOffsetY;		

				if ( m_bThreadMode )
				{
					double PreMulX			=	m_nLookUpScale * m_nImageOffsetX + AreaOffsetX;
					double PreMulY			=	m_nLookUpScale * m_nImageOffsetY + AreaOffsetY;
					double PreSourceIndX	=	( m_nLookUpScale		* m_nAreaBeginX + PreMulX );
					long PreSourceIndY		=	(long)( m_nLookUpScale	* ( m_nAreaBeginY - 1 ) + PreMulY );
					long SourceSize			=	m_nImageW * m_nImageH;
					double SourceIndY2		=	( m_nLookUpScale	* ( m_nAreaEndY - 1 ) + PreMulY );
					double SourceIndX2		=	( m_nLookUpScale	* ( m_nAreaEndX - 1 ) + PreMulX );
					double nSourceIndD2		=	SourceIndX2 + (long) SourceIndY2 * m_nImageW;
					long SourceInd2			=	(long) nSourceIndD2;
				
					// ошибка дискретизации. фикс
					if ( SourceSize <= SourceInd2 )
					{
						m_nAreaEndY--;
					}

					m_bThreadActivate[0]	=	true;
					m_bThreadActivate[1]	=	true;

					WaitForMultipleObjectsEx ( 2, m_hEvents, TRUE, INFINITE, FALSE );

					PulseEvent ( m_hEvents [ 0 ] );
					PulseEvent ( m_hEvents [ 1 ] );

					return;
				}

				double PreMulX			=	m_nLookUpScale * m_nImageOffsetX + AreaOffsetX;
				double PreMulY			=	m_nLookUpScale * m_nImageOffsetY + AreaOffsetY;
				double PreSourceIndX	=	( m_nLookUpScale		* m_nAreaBeginX + PreMulX );
				long PreSourceIndY		=	(long)( m_nLookUpScale	* ( m_nAreaBeginY - 1 ) + PreMulY );

				long SourceSize			=	m_nImageW * m_nImageH;
				double SourceIndY2		=	( m_nLookUpScale	* ( m_nAreaEndY - 1 ) + PreMulY );
				double SourceIndX2		=	( m_nLookUpScale	* ( m_nAreaEndX - 1 ) + PreMulX );
				double nSourceIndD2		=	SourceIndX2 + (long) SourceIndY2 * m_nImageW;
				long SourceInd2			=	(long) nSourceIndD2;

				double	dY				=	(double)( m_nAreaEndY - m_nAreaBeginY ) * m_nLookUpScale;
				bool	CopyEndLine		=	false;

				// ошибка дискретизации. фикс
				if ( SourceSize <= SourceInd2 )
				{
					double BadY			=	dY - m_nImageH;
					if ( BadY > 0.9999999 )				// ошибка больше единицы, копируем предпоследнею строчку
					{
						CopyEndLine		=	true;
						--m_nAreaEndY;
					}

#ifdef _DEBUG
					if ( FALSE )
					{
						wchar_t buffer[32];
						swprintf ( buffer, 32, L"Digitization error: %f \n", BadY );
						::OutputDebugStringW ( buffer );
					}
#endif

					--m_nAreaEndY;
				}

				long AreaSrcX			=	m_nAreaEndX - m_nAreaBeginX;

				for ( long Y = m_nAreaBeginY; Y < m_nAreaEndY; ++Y )
				{
					long SourceIndY		=	(long)( m_nLookUpScale * Y + PreMulY );
					double dSourceInd	=	PreSourceIndX + (long) SourceIndY * m_nImageW;
					DWORD* pImage2		=	&pImage	 [ (long) dSourceInd ];

					long WriteInd		=	Y * m_nAreaW + AreaBeginX;
					//long WriteInd		=	m_pLookUpX [ Y ];//Y * AreaW + AreaBeginX;

					DWORD* pArea2		=	&pArea [ WriteInd ];

					for ( long X = 0; X < AreaSrcX; ++X )
					{
						//*pArea2		=	* ( pImage2 + (long ) ( X * m_nLookUpScale ) );
						
						//*pArea2		=	* ( pImage2 + m_pLookUpX [ X ] );
						//++pArea2;
						pArea2 [ X ]	=	* ( pImage2 + m_pLookUpX [ X ] );
						//++pArea2;
					}
				}

				if ( CopyEndLine )
				{
					memcpy (	&pArea [ ( m_nAreaEndY - 0 ) * m_nAreaW + AreaBeginX ], 
								&pArea [ ( m_nAreaEndY - 1 ) * m_nAreaW + AreaBeginX ], 
								AreaSrcX * sizeof ( DWORD ) );
				}
			}

		private:

			// обвновляем таблицы коэффициентов
			inline void UpdateLookUp ( long SrcX, long SrcY, double LookUpScale )
			{
				bool UpdateLookUpX	=	false;

				if ( SrcX != m_nSrcX )
				{
					if ( m_nSrcX <= SrcX )
					{
						if ( NULL != m_pLookUpX	)
							delete m_pLookUpX;

						m_nSrcX		=	SrcX;
						m_pLookUpX	=	new long [ m_nSrcX + 1 ];
					}

					m_nSrcX			=	SrcX;

					UpdateLookUpX	=	true;
				}
				
				if ( UpdateLookUpX || m_nLookUpScale != LookUpScale )
				{
					for ( long i = 0; i <= m_nSrcX; ++i )
					{
						m_pLookUpX	[ i ]	=	static_cast < long > ( ((double)i * LookUpScale) );
					}
				}

				/*

				bool UpdateLookUpY	=	false;
				if ( SrcY != m_nAreaW )
				{
				if ( NULL != m_pLookUpY	)
				delete m_pLookUpY;

				m_pLookUpY		=	new long [ m_nAreaW + 1 ];

				UpdateLookUpY	=	true;
				}


				if ( UpdateLookUpY || m_nLookUpScale != LookUpScale )
				{
				for ( long i = m_nAreaBeginY; i < m_nAreaEndY; ++i )
				{
				m_pLookUpY	[ i ]	=	i * m_nAreaW + m_nAreaBeginX;
				}
				}

				*/
			}

			
			static DWORD WINAPI ThreadFunc ( void* pv )
			{
				ImageSurface *pThis = (ImageSurface *)pv;
				if ( NULL != pThis )
				{
					while ( TRUE )
					{
						Sleep ( 1 );
						
						if ( pThis->m_bThreadActivate[0] )
						{
							pThis->AsyncResizeImage ();
						}
						
						if ( pThis->m_bThreadClose[0] )
							return 0;
					}
				}
				
				return 0;
			}
			static DWORD WINAPI ThreadFunc2 ( void* pv )
			{
				ImageSurface *pThis = (ImageSurface *)pv;

				if ( NULL != pThis )
				{
					while ( TRUE )
					{
						Sleep ( 1 );
						
						if ( pThis->m_bThreadActivate [1] )
						{
							pThis->AsyncResizeImage2 ();
						}
						
						if ( pThis->m_bThreadClose[1] )
							return 0;
					}
				}

				return 0;
			}


			
			inline void AsyncResizeImage ()
			{
				DWORD* pImageT			=	m_pImage;
				DWORD* pAreaT			=	m_pArea;

				double PreMulX			=	m_nLookUpScale * m_nImageOffsetX + m_nAreaOffsetX;
				double PreMulY			=	m_nLookUpScale * m_nImageOffsetY + m_nAreaOffsetY;
				double PreSourceIndX	=	( m_nLookUpScale		* m_nAreaBeginX + PreMulX );
				long PreSourceIndY		=	(long)( m_nLookUpScale	* ( m_nAreaBeginY - 1 ) + PreMulY );

				long SourceSize			=	m_nImageW * m_nImageH;
				double SourceIndY2		=	( m_nLookUpScale	* ( m_nAreaEndY - 1 ) + PreMulY );
				double SourceIndX2		=	( m_nLookUpScale	* ( m_nAreaEndX - 1 ) + PreMulX );
				double nSourceIndD2		=	SourceIndX2 + (long) SourceIndY2 * m_nImageW;
				long SourceInd2			=	(long) nSourceIndD2;

				long AreaSrcX			=	m_nAreaEndX - m_nAreaBeginX;

				long EndY = m_nAreaEndY - ( m_nAreaEndY - m_nAreaBeginY ) / 2;
				for ( long Y = m_nAreaBeginY; Y < EndY; ++Y )
				{
					long SourceIndY		=	(long)( m_nLookUpScale * Y + PreMulY );
					double dSourceInd	=	PreSourceIndX + (long) SourceIndY * m_nImageW;
					DWORD* pImage2		=	&pImageT	 [ (long) dSourceInd ];

					long WriteInd		=	Y * m_nAreaW + m_nAreaBeginX;
					//long WriteInd		=	m_pLookUpX [ Y ];//Y * AreaW + AreaBeginX;

					DWORD* pArea2		=	&pAreaT [ WriteInd ];

					for ( long X = 0; X < AreaSrcX; ++X )
					{
						//*pArea2		=	* ( pImage2 + X * m_nLookUpScale );
						//*pArea2		=	* ( pImage2 + m_pLookUpX [ X ] );
						//++pArea2;
						pArea2 [ X ]	=	* ( pImage2 + m_pLookUpX [ X ] );
					}
				}

				m_bThreadActivate [0] = false;

				SetEvent ( m_hEvents [ 0 ] );
				
				// ATLTRACE ( "TR1\n" );
			}
			
			inline void AsyncResizeImage2 ()
			{
				DWORD* pImageT			=	m_pImage;
				DWORD* pAreaT			=	m_pArea;

				double PreMulX			=	m_nLookUpScale * m_nImageOffsetX + m_nAreaOffsetX;
				double PreMulY			=	m_nLookUpScale * m_nImageOffsetY + m_nAreaOffsetY;
				double PreSourceIndX	=	( m_nLookUpScale		* m_nAreaBeginX + PreMulX );
				long PreSourceIndY		=	(long)( m_nLookUpScale	* ( m_nAreaBeginY - 1 ) + PreMulY );

				long SourceSize			=	m_nImageW * m_nImageH;
				double SourceIndY2		=	( m_nLookUpScale	* ( m_nAreaEndY - 1 ) + PreMulY );
				double SourceIndX2		=	( m_nLookUpScale	* ( m_nAreaEndX - 1 ) + PreMulX );
				double nSourceIndD2		=	SourceIndX2 + (long) SourceIndY2 * m_nImageW;
				long SourceInd2			=	(long) nSourceIndD2;

				long AreaSrcX			=	m_nAreaEndX - m_nAreaBeginX;

				long BeginY = m_nAreaBeginY + ( m_nAreaEndY - m_nAreaBeginY ) / 2 ;
				for ( long Y = BeginY; Y < m_nAreaEndY; ++Y )
				{
					long SourceIndY		=	(long)( m_nLookUpScale * Y + PreMulY );
					double dSourceInd	=	PreSourceIndX + (long) SourceIndY * m_nImageW;
					DWORD* pImage2		=	&pImageT	 [ (long) dSourceInd ];

					long WriteInd		=	Y * m_nAreaW + m_nAreaBeginX;
					//long WriteInd		=	m_pLookUpX [ Y ];//Y * AreaW + AreaBeginX;

					DWORD* pArea2		=	&pAreaT [ WriteInd ];

					for ( long X = 0; X < AreaSrcX; ++X )
					{
						//*pArea2		=	* ( pImage2 + X * m_nLookUpScale );
						//*pArea2		=	* ( pImage2 + m_pLookUpX [ X ] );
						//++pArea2;
						
						pArea2 [ X ]	=	* ( pImage2 + m_pLookUpX [ X ] );
					}
				}

				m_bThreadActivate [1] = false;
				
				SetEvent ( m_hEvents [ 1 ] );
				
				// ATLTRACE ( "TR2\n" );
			}
		
		private: 

			DWORD*	m_pImage;

			long	m_nAreaW;
			long	m_nAreaH;
			long	m_nImageW;
			long	m_nImageH;		
			long	m_nImageOffsetX;
			long	m_nImageOffsetY;		
			long	m_nAreaOffsetX;
			long	m_nAreaOffsetY;		
		
			// в целом есть ускорение на несколько ядер
			bool	m_bThreadMode;
			
			bool	m_bThreadClose		[ 2 ];
			bool	m_bThreadActivate	[ 2 ];

			HANDLE	m_hThread			[ 2 ];
			HANDLE	m_hEvents			[ 2 ];

			DWORD*	m_pArea;
			
			long	m_nAreaBeginX;
			long	m_nAreaEndX;
			long	m_nAreaBeginY;
			long	m_nAreaEndY;

			long	m_nSrcX;
			long	m_nSrcY;

			double	m_nLookUpScale;

			long*	m_pLookUpX;
			long*	m_pLookUpY;

		};
		
		struct ImageResize
		{
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

				double dY					=	(double)( EndWriteH - StartWriteH ) * SrcStepHeight;

				bool	CopyEndLine			=	false;

				// ошибка дискретизации. фикс
				if ( SourceSize <= SourceInd2 )
				{
					double BadY			=	dY - SourceHeight;
					if ( BadY > 0.9999999 )				// ошибка больше единицы, копируем предпоследнею строчку
					{
						CopyEndLine		=	true;
						--EndWriteH;
					}

#ifdef _DEBUG
					if ( FALSE )
					{
						wchar_t buffer[32];
						swprintf ( buffer, 32, L"Digitization error: %f \n", BadY );
						::OutputDebugStringW ( buffer );
					}
#endif

					--EndWriteH;
				}

				for ( long j = StartWriteH; j < EndWriteH; ++j )
				{
					long SourceIndY			=	(long)( SrcStepHeight	* j + PreMulY );
					double dSourceInd		=	PreSourceIndX + (long) SourceIndY * SourceWidth;
					long WriteInd			=	j * nWidth + StartWriteW;

					long SourceY			=	(long) dSourceInd;

					for ( long i = StartWriteW; i < EndWriteW; ++i )
					{
						pPixels[WriteInd]	=	pSrc [ (long) dSourceInd ];

						++WriteInd;
						dSourceInd			+=	SrcStepWidth;
					}
				}

				if ( CopyEndLine )
				{
					memcpy (	&pPixels [ ( EndWriteH - 0 ) * nWidth + StartWriteW ], 
								&pPixels [ ( EndWriteH - 1 ) * nWidth + StartWriteW ], 
								( EndWriteW - StartWriteW ) * sizeof ( DWORD ) );
				}
			}

			static void NearestResizeImageDW2 ( DWORD* pSrc, DWORD* pPixels,
				long SourceWidth, long SourceHeight, long nWidth, long nHeight, 
				double SourceOffsetWidth, double SourceOffsetHeight,
				double DestOffsetWidth, double DestOffsetHeight,
				double SrcStepWidth, double SrcStepHeight,
				long StartWriteW, long EndWriteW, long StartWriteH, long EndWriteH /* nChannels = 4 */ )
			{
				double PreMulX				=	SrcStepWidth			* DestOffsetWidth		+ SourceOffsetWidth;
				double PreMulY				=	SrcStepHeight			* DestOffsetHeight		+ SourceOffsetHeight;
				double PreSourceIndX		=	( SrcStepWidth			* StartWriteW			+ PreMulX );
				long PreSourceIndY			=	(long)( SrcStepHeight	* ( StartWriteH - 1 )	+ PreMulY );

				long SourceSize				=	SourceWidth			* SourceHeight;
				double SourceIndY2			=	SrcStepHeight		* ( EndWriteH - 1 )		+ PreMulY;
				double SourceIndX2			=	SrcStepWidth		* ( EndWriteW - 1 )		+ PreMulX;
				double nSourceIndD2			=	SourceIndX2			+ (long) SourceIndY2	* SourceWidth;
				long SourceInd2				=	(long) nSourceIndD2;

				// ошибка дискретизации. фикс
				if ( SourceSize <= SourceInd2 )
				{
					EndWriteH--;
				}

				long HalfHeight				=	EndWriteH / 2;

				for ( long j = StartWriteH; j < HalfHeight; ++j )
				{
					long SourceIndY			=	(long)( SrcStepHeight	* j + PreMulY );
					double dSourceInd		=	PreSourceIndX + (long) SourceIndY * SourceWidth;
					long WriteInd			=	j * nWidth + StartWriteW;

					for ( long i = StartWriteW; i < EndWriteW; ++i )
					{
						pPixels[WriteInd]	=	pSrc [ (long) dSourceInd ];

						++WriteInd;
						dSourceInd			+=	SrcStepWidth;
					}
				}

				for ( long j = HalfHeight; j < EndWriteH; ++j )
				{
					long SourceIndY			=	(long)( SrcStepHeight	* j + PreMulY );
					double dSourceInd		=	PreSourceIndX + (long) SourceIndY * SourceWidth;
					long WriteInd			=	j * nWidth + StartWriteW;

					for ( long i = StartWriteW; i < EndWriteW; ++i )
					{
						pPixels[WriteInd]	=	pSrc [ (long) dSourceInd ];

						++WriteInd;
						dSourceInd			+=	SrcStepWidth;
					}
				}
			}

			static void NearestResizeImageOriginal ( 
				DWORD* pSrc, 
				DWORD* pPixels,
				long SourceWidth, long SourceHeight, 
				long nWidth, long nHeight, 
				double SourceOffsetWidth, double SourceOffsetHeight,
				double DestOffsetWidth, double DestOffsetHeight,
				double SrcStepWidth, double SrcStepHeight,
				long StartWriteW, long EndWriteW, 
				long StartWriteH, long EndWriteH )
			{
				double	PreMulX						=	SrcStepWidth * DestOffsetWidth		+ SourceOffsetWidth;
				double	PreMulY						=	SrcStepHeight * DestOffsetHeight	+ SourceOffsetHeight;
				double	PreSourceIndX				=	SrcStepWidth * StartWriteW			+ PreMulX;
				long	PreSourceIndY				=	static_cast<long> ( SrcStepHeight * ( StartWriteH - 1 ) + PreMulY );

				double nIndX						=	SrcStepWidth * ( StartWriteW ) + PreMulX;

				int	WriteOff =	( EndWriteH - StartWriteH ) % 2;
				EndWriteH -= WriteOff;

				for ( long j = StartWriteH; j < EndWriteH; ++j )
				{
					long SourceIndY				=	static_cast<long> ( SrcStepHeight * j + PreMulY );
					long WriteInd				=	static_cast<long> ( j * nWidth + StartWriteW );

					double SourceIndX			=	nIndX;
					long SourceMoveY			=	SourceIndY * SourceWidth;

					for ( long i = StartWriteW; i < EndWriteW; ++i )
					{
						long nSourceInd			=	static_cast<long> ( SourceIndX + SourceMoveY );

						pPixels [ WriteInd ]	=	pSrc [ nSourceInd ];

						++WriteInd;

						SourceIndX				+=	SrcStepWidth;
					}
				}
			}

			static void NearestResizeImageDoubleOptimize ( 
				DWORD* pSrc, 
				DWORD* pPixels,
				long SourceWidth, long SourceHeight, 
				long nWidth, long nHeight, 
				double SourceOffsetWidth, double SourceOffsetHeight,
				double DestOffsetWidth, double DestOffsetHeight,
				double SrcStepWidth, double SrcStepHeight,
				long StartWriteW, long EndWriteW, 
				long StartWriteH, long EndWriteH )
			{
				double	PreMulX					=	SrcStepWidth * DestOffsetWidth		+ SourceOffsetWidth;
				double	PreMulY					=	SrcStepHeight * DestOffsetHeight	+ SourceOffsetHeight;
				double	PreSourceIndX			=	SrcStepWidth * StartWriteW			+ PreMulX;
				long	PreSourceIndY			=	static_cast<long> ( SrcStepHeight * ( StartWriteH - 1 ) + PreMulY );

				double nIndX					=	SrcStepWidth * ( StartWriteW ) + PreMulX;

				long Width						=	static_cast<long> ( ( EndWriteH - StartWriteH ) * 0.5 );

				for ( long j = StartWriteH; j < EndWriteH - Width; ++j )
				{
					long SourceIndY				=	static_cast<long> ( SrcStepHeight * j + PreMulY );
					long WriteInd				=	static_cast<long> ( j * nWidth + StartWriteW );

					double SourceIndX			=	nIndX;
					long SourceMoveY			=	SourceIndY * SourceWidth;

					for ( long i = StartWriteW; i < EndWriteW; ++i )
					{
						long nSourceInd			=	static_cast<long> ( SourceIndX + SourceMoveY );

						pPixels [ WriteInd ]	=	pSrc [ nSourceInd ];

						++WriteInd;

						SourceIndX				+=	SrcStepWidth;
					}

				}

				for ( long j = StartWriteH + Width; j < EndWriteH; ++j )
				{
					long SourceIndY				=	static_cast<long> ( SrcStepHeight * j + PreMulY );
					long WriteInd				=	static_cast<long> ( j * nWidth + StartWriteW );

					double SourceIndX			=	nIndX;
					long SourceMoveY			=	SourceIndY * SourceWidth;

					for ( long i = StartWriteW; i < EndWriteW; ++i )
					{
						long nSourceInd			=	static_cast<long> ( SourceIndX + SourceMoveY );

						pPixels [ WriteInd ]	=	pSrc [ nSourceInd ];

						++WriteInd;

						SourceIndX				+=	SrcStepWidth;
					}
				}
			}

			static void NearestResizeCopySameBlocks ( 
				DWORD* pSrc, 
				DWORD* pPixels,
				long SourceWidth, long SourceHeight, 
				long nWidth, long nHeight, 
				double SourceOffsetWidth, double SourceOffsetHeight,
				double DestOffsetWidth, double DestOffsetHeight,
				double SrcStepWidth, double SrcStepHeight,
				long StartWriteW, long EndWriteW, 
				long StartWriteH, long EndWriteH )
			{
				double	SameLines					=	1.0f / SrcStepHeight;
				long	BytesLine					=	nWidth * sizeof ( DWORD );
				long	MoveOffset					=	(long)(long)DestOffsetHeight % (long)SameLines;

				double	PreMulX						=	SrcStepWidth * DestOffsetWidth		+ SourceOffsetWidth;
				double	PreMulY						=	SrcStepHeight * ( DestOffsetHeight	) + SourceOffsetHeight;
				double	PreSourceIndX				=	SrcStepWidth * StartWriteW			+ PreMulX;
				long	PreSourceIndY				=	static_cast<long> ( SrcStepHeight * ( StartWriteH - 1 ) + PreMulY );

				double nIndX						=	SrcStepWidth * ( StartWriteW ) + PreMulX;

				if ( MoveOffset > 0 )
				{
					for ( long j = StartWriteH; j < EndWriteH; j += (long)SameLines )
					{
						long SourceIndY				=	static_cast<long> ( SrcStepHeight * ( j ) + PreMulY );
						long WriteInd				=	static_cast<long> ( j * nWidth + StartWriteW );

						double SourceIndX			=	nIndX;
						long SourceMoveY			=	SourceIndY * SourceWidth;

						for ( long i = StartWriteW; i < EndWriteW; ++i )
						{
							long nSourceInd			=	static_cast<long> ( SourceIndX + SourceMoveY );

							pPixels [ WriteInd ]	=	pSrc [ nSourceInd ];

							++WriteInd;

							SourceIndX				+=	SrcStepWidth;
						}

						long CopyStart	= j + 1;
						long CopyEnd	= j + static_cast<long> ( SameLines - MoveOffset );

						if ( j != StartWriteH && MoveOffset > 0 )
						{
							CopyStart -= MoveOffset;
						}

						if ( CopyEnd >= EndWriteH )
							CopyEnd = EndWriteH - 1;

						long BeginRead		=	j * nWidth + StartWriteW;

						if ( CopyStart >= EndWriteH )
							continue;

						for ( long CopyLine = CopyStart; CopyLine <= CopyEnd; ++CopyLine )
						{
							long BeginWrite		=	CopyLine * nWidth + StartWriteW;
							memcpy ( &pPixels [ BeginWrite ], &pPixels [ BeginRead ], BytesLine );
						}
					}

					long WriteBeginP = (long) max ( 0, EndWriteH - SameLines );
					for ( long j = WriteBeginP; j < EndWriteH; ++j )
					{
						long SourceIndY				=	static_cast<long> ( SrcStepHeight * j + PreMulY );
						long WriteInd				=	static_cast<long> ( j * nWidth + StartWriteW );

						double SourceIndX			=	nIndX;
						long SourceMoveY			=	SourceIndY * SourceWidth;

						for ( long i = StartWriteW; i < EndWriteW; ++i )
						{
							long nSourceInd			=	static_cast<long> ( SourceIndX + SourceMoveY );

							pPixels [ WriteInd ]	=	pSrc [ nSourceInd ];

							++WriteInd;

							SourceIndX				+=	SrcStepWidth;
						}
					}
				}
				else
				{
					for ( long j = StartWriteH; j < EndWriteH; j += (long)SameLines )
					{
						long SourceIndY				=	static_cast<long> ( SrcStepHeight * ( j ) + PreMulY );
						long WriteInd				=	static_cast<long> ( j * nWidth + StartWriteW );

						double SourceIndX			=	nIndX;
						long SourceMoveY			=	SourceIndY * SourceWidth;

						for ( long i = StartWriteW; i < EndWriteW; ++i )
						{
							long nSourceInd			=	static_cast<long> ( SourceIndX + SourceMoveY );

							pPixels [ WriteInd ]	=	pSrc [ nSourceInd ];

							++WriteInd;

							SourceIndX				+=	SrcStepWidth;
						}

						long CopyStart	= j + 1;
						long CopyEnd	= j + static_cast<long> ( SameLines - MoveOffset );

						if ( CopyEnd >= EndWriteH )
							CopyEnd = EndWriteH - 2;

						long BeginRead		=	j * nWidth + StartWriteW;

						if ( CopyStart >= EndWriteH )
							continue;

						for ( long CopyLine = CopyStart; CopyLine <= CopyEnd; ++CopyLine )
						{
							long BeginWrite		=	CopyLine * nWidth + StartWriteW;
							memcpy ( &pPixels [ BeginWrite ], &pPixels [ BeginRead ], BytesLine );
						}
					}
				}
			}

			static void BilinearResizeImage ( 
				DWORD* pSrc, 
				DWORD* pPixels,
				long SourceWidth, long SourceHeight,
				long nWidth, long nHeight, 
				double SourceOffsetWidth, double SourceOffsetHeight,
				double DestOffsetWidth, double DestOffsetHeight,
				double SrcStepWidth, double SrcStepHeight,
				long StartWriteW, long EndWriteW, 
				long StartWriteH, long EndWriteH )
			{
				BYTE* pSource = ( BYTE* )((void*)pSrc);
				BYTE* pImage = ( BYTE* )((void*)pPixels);

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

						BYTE* pColor03				=	&pSource [ SelectColor03 ];
						BYTE* pColor01				=	&pSource [ SelectColor01 ];

						BYTE Blue01					=	(BYTE) ( pColor01[0] + ( pColor01[4] - pColor01[0] ) * FractionX );
						BYTE Blue02					=	(BYTE) ( pColor03[0] + ( pColor03[4] - pColor03[0] ) * FractionX );
						BYTE Green01				=	(BYTE) ( pColor01[1] + ( pColor01[5] - pColor01[1] ) * FractionX );
						//00
						BYTE Green02				=	(BYTE) ( pColor03[1] + ( pColor03[5] - pColor03[1] ) * FractionX );
						BYTE Red01					=	(BYTE) ( pColor01[2] + ( pColor01[6] - pColor01[2] ) * FractionX );
						BYTE Red02					=	(BYTE) ( pColor03[2] + ( pColor03[6] - pColor03[2] ) * FractionX );
						//00

						pImage [ nWriteInd + 0 ]	=	(BYTE) ( Blue01		+ FractionY * ( Blue02 - Blue01 ) );
						pImage [ nWriteInd + 1 ]	=	(BYTE) ( Green01	+ FractionY * ( Green02 - Green01 ) );
						pImage [ nWriteInd + 2 ]	=	(BYTE) ( Red01		+ FractionY * ( Red02 - Red01 ) );
						pImage [ nWriteInd + 3 ]	=	0xFF;
					}
				}
			}

			static void NearestResizeCopySameBlocksF ( 
				DWORD* pSrc, 
				DWORD* pPixels,
				long SourceWidth, long SourceHeight, 
				long nWidth, long nHeight, 
				float SourceOffsetWidth, float SourceOffsetHeight,
				float DestOffsetWidth, float DestOffsetHeight,
				float SrcStepWidth, float SrcStepHeight,
				long StartWriteW, long EndWriteW, 
				long StartWriteH, long EndWriteH )
			{
				// TODO : данные вариант работает в два раза быстрее чем обычный на double - long

				long	SameLines				=	static_cast<long> ( 1.0f / SrcStepHeight );
				long	BytesLine				=	nWidth * sizeof ( DWORD );

				float	PreMulX					=	SrcStepWidth * DestOffsetWidth		+ SourceOffsetWidth;
				float	PreMulY					=	SrcStepHeight * DestOffsetHeight	+ SourceOffsetHeight;
				float	PreSourceIndX			=	SrcStepWidth * StartWriteW			+ PreMulX;
				long	PreSourceIndY			=	static_cast<long> ( SrcStepHeight * ( StartWriteH - 1 ) + PreMulY );

				float	nIndX					=	SrcStepWidth * ( StartWriteW ) + PreMulX;

				long	Width					=	static_cast<long> ( ( EndWriteH - StartWriteH ) * 0.5 );

				long nSourceInd					=	0;

				for ( long j = StartWriteH; j < EndWriteH - Width; j += SameLines )
				{
					long SourceIndY				=	static_cast<long> ( SrcStepHeight * j + PreMulY );
					long WriteInd				=	j * nWidth + StartWriteW;

					//float SourceIndX			=	nIndX;
					long SourceMoveY			=	SourceIndY * SourceWidth;

					float fSourceInd			=	nIndX;//SourceIndX;// + SourceMoveY;

					for ( long i = StartWriteW; i < EndWriteW; ++i )
					{
						//long nSourceInd		=	static_cast<long> ( SourceIndX + SourceMoveY );

						__asm
						{
							FLD         dword ptr [ fSourceInd ] // загрузка значения float
							FISTTP      dword ptr [ nSourceInd ] // сохранение значения int в режиме truncate
						}


						pPixels [ WriteInd ]	=	pSrc [ nSourceInd + SourceMoveY ];

						++WriteInd;

						fSourceInd				+=	SrcStepWidth;
						//SourceIndX			+=	SrcStepWidth;
					}

					int WriteLines = j + SameLines;

					for ( long m = j + 1; m < WriteLines; ++m )
					{
						long BeginWrite		=	m * nWidth + StartWriteW;
						long BeginRead		=	j * nWidth + StartWriteW;
						memcpy ( &pPixels [ BeginWrite ], &pPixels [ BeginRead ], BytesLine );
					}
				}

				for ( long j = StartWriteH + Width; j < EndWriteH; j += SameLines )
				{
					long SourceIndY				=	static_cast<long> ( SrcStepHeight * j + PreMulY );
					long WriteInd				=	j * nWidth + StartWriteW;

					//float SourceIndX			=	nIndX;
					long SourceMoveY			=	SourceIndY * SourceWidth;

					float fSourceInd			=	nIndX;//SourceIndX;// + SourceMoveY;

					for ( long i = StartWriteW; i < EndWriteW; ++i )
					{
						//long nSourceInd			=	static_cast<long> ( SourceIndX + SourceMoveY );

						__asm
						{
							fld         dword ptr [ fSourceInd ] // загрузка значения float
							fisttp      dword ptr [ nSourceInd ] // сохранение значения int в режиме truncate
						}

						pPixels [ WriteInd ]	=	pSrc [ nSourceInd + SourceMoveY ];

						++WriteInd;

						fSourceInd				+=	SrcStepWidth;
						//SourceIndX			+=	SrcStepWidth;
					}

					int WriteLines = j + SameLines;

					if ( WriteLines >= EndWriteH )
						WriteLines = EndWriteH - 1;

					for ( long m = j + 1; m < WriteLines; ++m )
					{
						long BeginWrite		=	m * nWidth + StartWriteW;
						long BeginRead		=	j * nWidth + StartWriteW;
						memcpy ( &pPixels [ BeginWrite ], &pPixels [ BeginRead ], BytesLine );
					}
				}
			}

		};
	}

#if _MSC_VER >= 1500 // 1500 == VC++ 9.0
	
	namespace Debug
	{
		class TicksCounter
		{
		public:

			TicksCounter ()
			{
				m_nTicksCount	=	0;
			}

			inline void Start ()
			{
				m_nTicksCount			=	__rdtsc();
			}

			inline void Stop ()
			{
				m_nTicksCount			=	__rdtsc() - m_nTicksCount;
			}

			inline __int64 GetCountTicks ()
			{
				return m_nTicksCount;
			}

		private:

			unsigned __int64	m_nTicksCount;
		};
	}

#endif	
}

namespace ImageRendering
{	
	class CRullers
	{
	public:
		static const long sc_nRullerTypePixels			=	0;
		static const long sc_nRullerTypeCentimeters		=	1;
		static const long sc_nRullerTypeInches			=	2;
		static const long sc_nRullerTypeCustom			=	3;
	public:

		CRullers () : m_pFontH ( NULL ), m_pFontV ( NULL ), m_pPenMinDash ( NULL )
		{			  
			m_bAlignCenter			=	false;
			m_nFontSize				=	11;

			m_nRullerSize			=	15;		
			m_nRullerLineMaxSize	=	25 * 4;
			m_nRullerLineMinSize	=	5;

			m_pFontH				=	new Gdiplus::Font ( L"Ms Shell Dlg", (Gdiplus::REAL)m_nFontSize, FontStyleRegular, Gdiplus::UnitPixel );
			m_pFontV				=	new Gdiplus::Font ( L"Ms Shell Dlg", (Gdiplus::REAL)m_nFontSize, FontStyleRegular, Gdiplus::UnitPixel );

			m_pPenMinDash			=	new Gdiplus::Pen ( Gdiplus::Color::Black, 1.0f );

			m_nRullerType			=	sc_nRullerTypePixels;

			m_nPixelsWCM			=	100;
			m_nPixelsHCM			=	100;

			m_nPixelsRange			=	0.5;
		}

		virtual ~CRullers ()
		{
			RELEASEOBJECT ( m_pFontH );
			RELEASEOBJECT ( m_pFontV );
			RELEASEOBJECT ( m_pPenMinDash );
		}


		inline void OnPaint				( HDC hDC, int nWindowWidth, int nWindowHeight, int nImageWidth, int nImageHeight, const double& nZoom, const double& OffsetX, const double& OffsetY, Gdiplus::Graphics* pGraphics )
		{
			if ( nZoom <= 0.0 )
				return;

			if ( false == m_nRullersUp && false == m_nRullersDown && false == m_nRullersRight && false == m_nRullersLeft )
				return;

			double dOffsetX		=	OffsetX;
			double dOffsetY		=	OffsetY;

			if ( m_bAlignCenter )
			{
				dOffsetX		=	OffsetX	+ (double)nImageWidth * nZoom * 0.5;
				dOffsetY		=	OffsetY	+ (double)nImageHeight * nZoom * 0.5;
			}

			if ( sc_nRullerTypeCentimeters	==	m_nRullerType )
				OnPaintRullesCM ( hDC, nWindowWidth, nWindowHeight, nImageWidth, nImageHeight, nZoom, dOffsetX, dOffsetY, pGraphics );

			if ( sc_nRullerTypePixels		==	m_nRullerType )
				OnPaintRullesPX ( hDC, nWindowWidth, nWindowHeight, nImageWidth, nImageHeight, nZoom, dOffsetX, dOffsetY, pGraphics );

			if ( sc_nRullerTypeInches		==	m_nRullerType )
				OnPaintRullesIN ( hDC, nWindowWidth, nWindowHeight, nImageWidth, nImageHeight, nZoom, dOffsetX, dOffsetY, pGraphics );

			if ( sc_nRullerTypeCustom		==	m_nRullerType )
				OnPaintRullesCustom ( hDC, nWindowWidth, nWindowHeight, nImageWidth, nImageHeight, nZoom, dOffsetX, dOffsetY, pGraphics );
		}


		inline void SetRullerUp			( bool RullerValue )
		{
			m_nRullersUp	=	RullerValue;
		}

		inline void SetRullerDown		( bool RullerValue )
		{
			m_nRullersDown	=	RullerValue;
		}

		inline void SetRullerRight		( bool RullerValue )
		{
			m_nRullersRight	=	RullerValue;
		}

		inline void SetRullerLeft		( bool RullerValue )
		{
			m_nRullersLeft	=	RullerValue;
		}



		inline long GetRullerType		( )
		{
			return m_nRullerType;
		}

		inline void SetRullerType		( long Value )
		{
			m_nRullerType	=	Value;
		}


		inline void SetCustomUnitW		( int Value )
		{
			m_nPixelsWCM	=	Value;
		}

		inline void SetCustomUnitH		( int Value )
		{
			m_nPixelsHCM	=	Value;
		}

		inline void SetCustomUnitRange	( double Value )
		{
			m_nPixelsRange	=	Value;
		}

		inline void SetAlignCenter		( bool Value )
		{
			m_bAlignCenter		=	Value;
		}


		inline const int& GetRullerSize	( ) const
		{
			return m_nRullerSize;
		}

	private:

		inline void OnPaintRullesPX		( HDC hDC, int nWindowWidth, int nWindowHeight, int nImageWidth, int nImageHeight,	const double& nZoom, const double& OffsetX, const double& OffsetY, Gdiplus::Graphics* pGraphics )
		{
			if ( nZoom <= 0.0 )
				return;

			double InvertZoom	=	1.0 / nZoom;

			Gdiplus::SolidBrush		oBlackSolidBrush ( Gdiplus::Color::Black );
			Gdiplus::StringFormat	oStringFormat;

			if ( oBlackSolidBrush.GetLastStatus () != Gdiplus::Ok )
				return;

			Gdiplus::SolidBrush		oWhitekSolidBrush ( Gdiplus::Color::White );
			if ( oWhitekSolidBrush.GetLastStatus () != Gdiplus::Ok )
				return;

			// Fill rullers regions
			if ( m_nRullersUp )	// UP
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, 0, nWindowWidth, m_nRullerSize );

			if ( m_nRullersLeft )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, m_nRullerSize, m_nRullerSize, nWindowHeight );

			if ( m_nRullersDown )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, nWindowHeight - m_nRullerSize, nWindowWidth, m_nRullerSize );

			if ( m_nRullersRight )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, nWindowWidth - m_nRullerSize, 0, m_nRullerSize, nWindowHeight );

			double UnZoom			=	1.0 / nZoom;

			int nPixelsMinDashCount	=	100;													// длина
			int nPixelsMinDashStep	=	0;
			int nPixelDashStepMax	=	100;
			if ( nZoom < 1.0 )
			{
				nPixelDashStepMax	=	static_cast <int> ( nPixelDashStepMax / nZoom );
				nPixelsMinDashCount	=	100;//nPixelDashStepMax;
				nPixelsMinDashStep	=	10;
			}
			else
			{
				nPixelDashStepMax	=	static_cast <int> ( 100.0 / nZoom );
				nPixelsMinDashCount	=	100;
				nPixelsMinDashStep	=	static_cast <int> ( nZoom );
				if ( nZoom < 10 )
					nPixelsMinDashStep	=	10;
			}

			// draw rullers text
			if ( m_nRullersUp )	// UP
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), 0, m_nRullerSize, nWindowWidth, m_nRullerSize );	// огранка со стороны картинки

				int nBeginX			=	static_cast <int> ( ( ( - OffsetX * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndX			=	static_cast <int> ( ( nWindowWidth - OffsetX ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					// int LinePosX	=	static_cast <int> ( nBeginX * nZoom + OffsetX );											// позиция следующей черты
					for ( Gdiplus::REAL j = static_cast <Gdiplus::REAL> ( nBeginX * nZoom + OffsetX ); j > 0.0f; j -= (Gdiplus::REAL)nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, j, 0.0f, j, (Gdiplus::REAL) ( m_nRullerSize * 0.25f ) );
				}

				for ( int i = nBeginX; i <= nEndX; i += nPixelDashStepMax )															// для максимальных делений
				{
					int LinePosX	=	static_cast <int> ( i * nZoom + OffsetX );													// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, LinePosX + j, 0, LinePosX + j, static_cast <int> ( m_nRullerSize * 0.25f ) );
					}

					if ( LinePosX < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, LinePosX, 0, LinePosX, m_nRullerSize );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%d", i );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosX + oTextBounding.Width >= nWindowWidth - m_nRullerSize )
							continue;
					}

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( (Gdiplus::REAL)LinePosX + 2.0f, (Gdiplus::REAL)m_nRullerSize - (Gdiplus::REAL)m_nFontSize - 2.0f, 50.0f, (Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush );
				}
			}

			if ( m_nRullersLeft )	// LEFT
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), m_nRullerSize, 0, m_nRullerSize, nWindowHeight );	// огранка со стороны картинки

				int nBeginY			=	static_cast <int> ( ( ( - OffsetY * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndY			=	static_cast <int> ( ( nWindowHeight - OffsetY ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetY ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, 0, j, static_cast <int> ( m_nRullerSize * 0.25f ), j );
				}

				for ( int i = nBeginY; i <= nEndY; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosY	=	static_cast <int> ( i * nZoom + OffsetY );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, 0, LinePosY + j, static_cast <int> ( m_nRullerSize * 0.25f ), LinePosY + j );
					}

					if ( LinePosY < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, 0, LinePosY, m_nRullerSize, LinePosY );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%d", i );

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosY - oTextBounding.Width <= m_nRullerSize )
							continue;
					}

					Gdiplus::Matrix oMatrix;
					oMatrix.Translate ( (Gdiplus::REAL)m_nRullerSize - (Gdiplus::REAL)m_nFontSize - 2.0f, (Gdiplus::REAL)LinePosY + 2.0f );
					oMatrix.Rotate ( -90.0f );
					pGraphics->SetTransform ( &oMatrix );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( 
						0.0f,
						0.0f,
						50.0f, 
						(Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 

					pGraphics->SetTransform ( &Gdiplus::Matrix () );
				}
			}

			if ( m_nRullersDown )	// DOWN
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), 0, nWindowHeight - m_nRullerSize, nWindowWidth, nWindowHeight - m_nRullerSize );	// огранка со стороны картинки

				int nBeginX				=	static_cast <int> ( ( ( - OffsetX * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndX				=	static_cast <int> ( ( nWindowWidth - OffsetX ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetX ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, j, nWindowHeight, j, nWindowHeight - static_cast <int> ( m_nRullerSize * 0.25f ) );
				}

				for ( int i = nBeginX; i <= nEndX; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosX	=	static_cast <int> ( i * nZoom + OffsetX );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, LinePosX + j, nWindowHeight, LinePosX + j, nWindowHeight - static_cast <int> ( m_nRullerSize * 0.25f ) );
					}

					if ( LinePosX < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, LinePosX, nWindowHeight, LinePosX, nWindowHeight - m_nRullerSize );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%d", i );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosX + oTextBounding.Width >= nWindowWidth - m_nRullerSize )
							continue;
					}

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( (Gdiplus::REAL)LinePosX + 2.0f, (Gdiplus::REAL)nWindowHeight - (Gdiplus::REAL)m_nFontSize - 2.0f, 50.0f, (Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 
				}
			}

			if ( m_nRullersRight )	// RIGHT
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), nWindowWidth - m_nRullerSize, 0, nWindowWidth - m_nRullerSize, nWindowHeight );	// огранка со стороны картинки

				int nBeginY			=	static_cast <int> ( ( ( - OffsetY * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndY			=	static_cast <int> ( ( nWindowHeight - OffsetY ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetY ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, nWindowWidth, j, nWindowWidth - static_cast <int> ( m_nRullerSize * 0.25f ), j );
				}

				for ( int i = nBeginY; i <= nEndY; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosY	=	static_cast <int> ( i * nZoom + OffsetY );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, 
							nWindowWidth, 
							LinePosY + j, 
							nWindowWidth - static_cast <int> ( m_nRullerSize * 0.25f ), 
							LinePosY + j );
					}

					if ( LinePosY < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, nWindowWidth, LinePosY, nWindowWidth - m_nRullerSize, LinePosY );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%d", i );

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosY - oTextBounding.Width <= m_nRullerSize )
							continue;
					}

					Gdiplus::Matrix oMatrix;
					oMatrix.Translate ( (Gdiplus::REAL)nWindowWidth - /*(Gdiplus::REAL)m_nRullerSize -*/ (Gdiplus::REAL)m_nFontSize - 2.0f, (Gdiplus::REAL)LinePosY + 2.0f );
					oMatrix.Rotate ( -90.0f );
					pGraphics->SetTransform ( &oMatrix );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( 
						0.0f,
						0.0f,
						50.0f, 
						(Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 

					pGraphics->SetTransform ( &Gdiplus::Matrix () );
				}
			}

			FillRectangles ( pGraphics, nWindowWidth, nWindowHeight );
		}

		inline void OnPaintRullesCM		( HDC hDC, int nWindowWidth, int nWindowHeight, int nImageWidth, int nImageHeight,	const double& nZoom, const double& OffsetX, const double& OffsetY, Gdiplus::Graphics* pGraphics )
		{
			if ( nZoom <= 0.0 )
				return;

			// 1 дюйм = 2,540005 см

			int nPixelsWCM	=	(int) ( GetDeviceCaps ( hDC, LOGPIXELSX ) / 2.540005 );
			int nPixelsHCM	=	(int) ( GetDeviceCaps ( hDC, LOGPIXELSY ) / 2.540005 );

			double InvertZoom	=	1.0 / nZoom;

			Gdiplus::SolidBrush		oBlackSolidBrush ( Gdiplus::Color::Black );
			Gdiplus::StringFormat	oStringFormat;

			if ( oBlackSolidBrush.GetLastStatus () != Gdiplus::Ok )
				return;

			Gdiplus::SolidBrush		oWhitekSolidBrush ( Gdiplus::Color::White );
			if ( oWhitekSolidBrush.GetLastStatus () != Gdiplus::Ok )
				return;

			// Fill rullers regions
			if ( m_nRullersUp )	// UP
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, 0, nWindowWidth, m_nRullerSize );

			if ( m_nRullersLeft )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, m_nRullerSize, m_nRullerSize, nWindowHeight );

			if ( m_nRullersDown )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, nWindowHeight - m_nRullerSize, nWindowWidth, m_nRullerSize );

			if ( m_nRullersRight )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, nWindowWidth - m_nRullerSize, 0, m_nRullerSize, nWindowHeight );

			double UnZoom			=	1.0 / nZoom;

			int nPixelsMinDashCount	=	nPixelsWCM;													// длина
			int nPixelsMinDashStep	=	0;
			int nPixelDashStepMax	=	nPixelsWCM;
			if ( nZoom < 1.0 )
			{
				nPixelDashStepMax	=	static_cast <int> ( nPixelDashStepMax / nZoom );
				nPixelsMinDashCount	=	nPixelsWCM;//nPixelDashStepMax;
				nPixelsMinDashStep	=	nPixelsWCM / 2;
			}
			else
			{
				nPixelDashStepMax	=	static_cast <int> ( nPixelsWCM / nZoom );
				nPixelsMinDashCount	=	nPixelsWCM;
				nPixelsMinDashStep	=	static_cast <int> ( nZoom );
				if ( nZoom < nPixelsWCM * 2 )
					nPixelsMinDashStep	=	nPixelsWCM / 2;
			}

			// draw rullers text
			if ( m_nRullersUp )	// UP
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), 0, m_nRullerSize, nWindowWidth, m_nRullerSize );	// огранка со стороны картинки

				int nBeginX			=	static_cast <int> ( ( ( - OffsetX * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndX			=	static_cast <int> ( ( nWindowWidth - OffsetX ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					// int LinePosX	=	static_cast <int> ( nBeginX * nZoom + OffsetX );											// позиция следующей черты
					for ( Gdiplus::REAL j = static_cast <Gdiplus::REAL> ( nBeginX * nZoom + OffsetX ); j > 0.0f; j -= (Gdiplus::REAL)nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, j, 0.0f, j, (Gdiplus::REAL) ( m_nRullerSize * 0.25f ) );
				}

				for ( int i = nBeginX; i <= nEndX; i += nPixelDashStepMax )															// для максимальных делений
				{
					int LinePosX	=	static_cast <int> ( i * nZoom + OffsetX );													// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, LinePosX + j, 0, LinePosX + j, static_cast <int> ( m_nRullerSize * 0.25f ) );
					}

					if ( LinePosX < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, LinePosX, 0, LinePosX, m_nRullerSize );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosX + oTextBounding.Width >= nWindowWidth - m_nRullerSize )
							continue;
					}

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( (Gdiplus::REAL)LinePosX + 2.0f, (Gdiplus::REAL)m_nRullerSize - (Gdiplus::REAL)m_nFontSize - 2.0f, 50.0f, (Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush );
				}
			}

			if ( m_nRullersLeft )	// LEFT
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), m_nRullerSize, 0, m_nRullerSize, nWindowHeight );	// огранка со стороны картинки

				int nBeginY			=	static_cast <int> ( ( ( - OffsetY * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndY			=	static_cast <int> ( ( nWindowHeight - OffsetY ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetY ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, 0, j, static_cast <int> ( m_nRullerSize * 0.25f ), j );
				}

				for ( int i = nBeginY; i <= nEndY; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosY	=	static_cast <int> ( i * nZoom + OffsetY );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, 0, LinePosY + j, static_cast <int> ( m_nRullerSize * 0.25f ), LinePosY + j );
					}

					if ( LinePosY < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, 0, LinePosY, m_nRullerSize, LinePosY );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosY - oTextBounding.Width <= m_nRullerSize )
							continue;
					}

					Gdiplus::Matrix oMatrix;
					oMatrix.Translate ( (Gdiplus::REAL)m_nRullerSize - (Gdiplus::REAL)m_nFontSize - 2.0f, (Gdiplus::REAL)LinePosY + 2.0f );
					oMatrix.Rotate ( -90.0f );
					pGraphics->SetTransform ( &oMatrix );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( 
						0.0f,
						0.0f,
						50.0f, 
						(Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 

					pGraphics->SetTransform ( &Gdiplus::Matrix () );
				}
			}

			if ( m_nRullersDown )	// DOWN
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), 0, nWindowHeight - m_nRullerSize, nWindowWidth, nWindowHeight - m_nRullerSize );	// огранка со стороны картинки

				int nBeginX				=	static_cast <int> ( ( ( - OffsetX * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndX				=	static_cast <int> ( ( nWindowWidth - OffsetX ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetX ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, j, nWindowHeight, j, nWindowHeight - static_cast <int> ( m_nRullerSize * 0.25f ) );
				}

				for ( int i = nBeginX; i <= nEndX; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosX	=	static_cast <int> ( i * nZoom + OffsetX );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, LinePosX + j, nWindowHeight, LinePosX + j, nWindowHeight - static_cast <int> ( m_nRullerSize * 0.25f ) );
					}

					if ( LinePosX < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, LinePosX, nWindowHeight, LinePosX, nWindowHeight - m_nRullerSize );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosX + oTextBounding.Width >= nWindowWidth - m_nRullerSize )
							continue;
					}

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( (Gdiplus::REAL)LinePosX + 2.0f, (Gdiplus::REAL)nWindowHeight - (Gdiplus::REAL)m_nFontSize - 2.0f, 50.0f, (Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 
				}
			}

			if ( m_nRullersRight )	// RIGHT
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), nWindowWidth - m_nRullerSize, 0, nWindowWidth - m_nRullerSize, nWindowHeight );	// огранка со стороны картинки

				int nBeginY			=	static_cast <int> ( ( ( - OffsetY * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndY			=	static_cast <int> ( ( nWindowHeight - OffsetY ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetY ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, nWindowWidth, j, nWindowWidth - static_cast <int> ( m_nRullerSize * 0.25f ), j );
				}

				for ( int i = nBeginY; i <= nEndY; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosY	=	static_cast <int> ( i * nZoom + OffsetY );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, 
							nWindowWidth, 
							LinePosY + j, 
							nWindowWidth - static_cast <int> ( m_nRullerSize * 0.25f ), 
							LinePosY + j );
					}

					if ( LinePosY < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, nWindowWidth, LinePosY, nWindowWidth - m_nRullerSize, LinePosY );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosY - oTextBounding.Width <= m_nRullerSize )
							continue;
					}

					Gdiplus::Matrix oMatrix;
					oMatrix.Translate ( (Gdiplus::REAL)nWindowWidth - /*(Gdiplus::REAL)m_nRullerSize -*/ (Gdiplus::REAL)m_nFontSize - 2.0f, (Gdiplus::REAL)LinePosY + 2.0f );
					oMatrix.Rotate ( -90.0f );
					pGraphics->SetTransform ( &oMatrix );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( 
						0.0f,
						0.0f,
						50.0f, 
						(Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 

					pGraphics->SetTransform ( &Gdiplus::Matrix () );
				}
			}

			FillRectangles ( pGraphics, nWindowWidth, nWindowHeight );
		}

		inline void OnPaintRullesIN		( HDC hDC, int nWindowWidth, int nWindowHeight, int nImageWidth, int nImageHeight,	const double& nZoom, const double& OffsetX, const double& OffsetY, Gdiplus::Graphics* pGraphics )
		{
			if ( nZoom <= 0.0 )
				return;

			int nPixelsWCM		=	GetDeviceCaps ( hDC, LOGPIXELSX );
			int nPixelsHCM		=	GetDeviceCaps ( hDC, LOGPIXELSY );

			double InvertZoom	=	1.0 / nZoom;

			Gdiplus::SolidBrush		oBlackSolidBrush ( Gdiplus::Color::Black );
			Gdiplus::StringFormat	oStringFormat;

			if ( oBlackSolidBrush.GetLastStatus () != Gdiplus::Ok )
				return;

			Gdiplus::SolidBrush		oWhitekSolidBrush ( Gdiplus::Color::White );
			if ( oWhitekSolidBrush.GetLastStatus () != Gdiplus::Ok )
				return;

			// Fill rullers regions
			if ( m_nRullersUp )	// UP
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, 0, nWindowWidth, m_nRullerSize );

			if ( m_nRullersLeft )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, m_nRullerSize, m_nRullerSize, nWindowHeight );

			if ( m_nRullersDown )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, nWindowHeight - m_nRullerSize, nWindowWidth, m_nRullerSize );

			if ( m_nRullersRight )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, nWindowWidth - m_nRullerSize, 0, m_nRullerSize, nWindowHeight );

			double UnZoom			=	1.0 / nZoom;

			int nPixelsMinDashCount	=	nPixelsWCM;													// длина
			int nPixelsMinDashStep	=	0;
			int nPixelDashStepMax	=	nPixelsWCM;
			if ( nZoom < 1.0 )
			{
				nPixelDashStepMax	=	static_cast <int> ( nPixelDashStepMax / nZoom );
				nPixelsMinDashCount	=	nPixelsWCM;//nPixelDashStepMax;
				nPixelsMinDashStep	=	nPixelsWCM / 2;
			}
			else
			{
				nPixelDashStepMax	=	static_cast <int> ( nPixelsWCM / nZoom );
				nPixelsMinDashCount	=	nPixelsWCM;
				nPixelsMinDashStep	=	static_cast <int> ( nZoom );
				if ( nZoom < nPixelsWCM * 2 )
					nPixelsMinDashStep	=	nPixelsWCM / 2;
			}

			// draw rullers text
			if ( m_nRullersUp )	// UP
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), 0, m_nRullerSize, nWindowWidth, m_nRullerSize );	// огранка со стороны картинки

				int nBeginX			=	static_cast <int> ( ( ( - OffsetX * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndX			=	static_cast <int> ( ( nWindowWidth - OffsetX ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					// int LinePosX	=	static_cast <int> ( nBeginX * nZoom + OffsetX );											// позиция следующей черты
					for ( Gdiplus::REAL j = static_cast <Gdiplus::REAL> ( nBeginX * nZoom + OffsetX ); j > 0.0f; j -= (Gdiplus::REAL)nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, j, 0.0f, j, (Gdiplus::REAL) ( m_nRullerSize * 0.25f ) );
				}

				for ( int i = nBeginX; i <= nEndX; i += nPixelDashStepMax )															// для максимальных делений
				{
					int LinePosX	=	static_cast <int> ( i * nZoom + OffsetX );													// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, LinePosX + j, 0, LinePosX + j, static_cast <int> ( m_nRullerSize * 0.25f ) );
					}

					if ( LinePosX < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, LinePosX, 0, LinePosX, m_nRullerSize );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosX + oTextBounding.Width >= nWindowWidth - m_nRullerSize )
							continue;
					}

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( (Gdiplus::REAL)LinePosX + 2.0f, (Gdiplus::REAL)m_nRullerSize - (Gdiplus::REAL)m_nFontSize - 2.0f, 50.0f, (Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush );
				}
			}

			if ( m_nRullersLeft )	// LEFT
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), m_nRullerSize, 0, m_nRullerSize, nWindowHeight );	// огранка со стороны картинки

				int nBeginY			=	static_cast <int> ( ( ( - OffsetY * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndY			=	static_cast <int> ( ( nWindowHeight - OffsetY ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetY ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, 0, j, static_cast <int> ( m_nRullerSize * 0.25f ), j );
				}

				for ( int i = nBeginY; i <= nEndY; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosY	=	static_cast <int> ( i * nZoom + OffsetY );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, 0, LinePosY + j, static_cast <int> ( m_nRullerSize * 0.25f ), LinePosY + j );
					}

					if ( LinePosY < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, 0, LinePosY, m_nRullerSize, LinePosY );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosY - oTextBounding.Width <= m_nRullerSize )
							continue;
					}

					Gdiplus::Matrix oMatrix;
					oMatrix.Translate ( (Gdiplus::REAL)m_nRullerSize - (Gdiplus::REAL)m_nFontSize - 2.0f, (Gdiplus::REAL)LinePosY + 2.0f );
					oMatrix.Rotate ( -90.0f );
					pGraphics->SetTransform ( &oMatrix );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( 
						0.0f,
						0.0f,
						50.0f, 
						(Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 

					pGraphics->SetTransform ( &Gdiplus::Matrix () );
				}
			}

			if ( m_nRullersDown )	// DOWN
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), 0, nWindowHeight - m_nRullerSize, nWindowWidth, nWindowHeight - m_nRullerSize );	// огранка со стороны картинки

				int nBeginX				=	static_cast <int> ( ( ( - OffsetX * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndX				=	static_cast <int> ( ( nWindowWidth - OffsetX ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetX ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, j, nWindowHeight, j, nWindowHeight - static_cast <int> ( m_nRullerSize * 0.25f ) );
				}

				for ( int i = nBeginX; i <= nEndX; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosX	=	static_cast <int> ( i * nZoom + OffsetX );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, LinePosX + j, nWindowHeight, LinePosX + j, nWindowHeight - static_cast <int> ( m_nRullerSize * 0.25f ) );
					}

					if ( LinePosX < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, LinePosX, nWindowHeight, LinePosX, nWindowHeight - m_nRullerSize );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosX + oTextBounding.Width >= nWindowWidth - m_nRullerSize )
							continue;
					}

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( (Gdiplus::REAL)LinePosX + 2.0f, (Gdiplus::REAL)nWindowHeight - (Gdiplus::REAL)m_nFontSize - 2.0f, 50.0f, (Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 
				}
			}

			if ( m_nRullersRight )	// RIGHT
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), nWindowWidth - m_nRullerSize, 0, nWindowWidth - m_nRullerSize, nWindowHeight );	// огранка со стороны картинки

				int nBeginY			=	static_cast <int> ( ( ( - OffsetY * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndY			=	static_cast <int> ( ( nWindowHeight - OffsetY ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetY ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, nWindowWidth, j, nWindowWidth - static_cast <int> ( m_nRullerSize * 0.25f ), j );
				}

				for ( int i = nBeginY; i <= nEndY; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosY	=	static_cast <int> ( i * nZoom + OffsetY );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, 
							nWindowWidth, 
							LinePosY + j, 
							nWindowWidth - static_cast <int> ( m_nRullerSize * 0.25f ), 
							LinePosY + j );
					}

					if ( LinePosY < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, nWindowWidth, LinePosY, nWindowWidth - m_nRullerSize, LinePosY );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosY - oTextBounding.Width <= m_nRullerSize )
							continue;
					}

					Gdiplus::Matrix oMatrix;
					oMatrix.Translate ( (Gdiplus::REAL)nWindowWidth - /*(Gdiplus::REAL)m_nRullerSize -*/ (Gdiplus::REAL)m_nFontSize - 2.0f, (Gdiplus::REAL)LinePosY + 2.0f );
					oMatrix.Rotate ( -90.0f );
					pGraphics->SetTransform ( &oMatrix );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( 
						0.0f,
						0.0f,
						50.0f, 
						(Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 

					pGraphics->SetTransform ( &Gdiplus::Matrix () );
				}
			}

			FillRectangles ( pGraphics, nWindowWidth, nWindowHeight );
		}

		inline void OnPaintRullesCustom ( HDC hDC, int nWindowWidth, int nWindowHeight, int nImageWidth, int nImageHeight,	const double& nZoom, const double& OffsetX, const double& OffsetY, Gdiplus::Graphics* pGraphics )
		{
			if ( nZoom <= 0.0 )
				return;

			int nPixelsWCM		=	m_nPixelsWCM;
			int nPixelsHCM		=	m_nPixelsHCM;

			double InvertZoom	=	1.0 / nZoom;

			Gdiplus::SolidBrush		oBlackSolidBrush ( Gdiplus::Color::Black );
			Gdiplus::StringFormat	oStringFormat;

			if ( oBlackSolidBrush.GetLastStatus () != Gdiplus::Ok )
				return;

			Gdiplus::SolidBrush		oWhitekSolidBrush ( Gdiplus::Color::White );
			if ( oWhitekSolidBrush.GetLastStatus () != Gdiplus::Ok )
				return;

			// Fill rullers regions
			if ( m_nRullersUp )	// UP
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, 0, nWindowWidth, m_nRullerSize );

			if ( m_nRullersLeft )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, m_nRullerSize, m_nRullerSize, nWindowHeight );

			if ( m_nRullersDown )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, 0, nWindowHeight - m_nRullerSize, nWindowWidth, m_nRullerSize );

			if ( m_nRullersRight )
				pGraphics->FillRectangle ( &oWhitekSolidBrush, nWindowWidth - m_nRullerSize, 0, m_nRullerSize, nWindowHeight );

			double UnZoom			=	1.0 / nZoom;

			int nPixelsMinDashCount	=	nPixelsWCM;													// длина
			int nPixelsMinDashStep	=	0;
			int nPixelDashStepMax	=	nPixelsWCM;
			if ( nZoom < 1.0 )
			{
				nPixelDashStepMax	=	static_cast <int> ( nPixelDashStepMax / nZoom );
				nPixelsMinDashCount	=	nPixelsWCM;//nPixelDashStepMax;
				nPixelsMinDashStep	=	static_cast <int> ( nPixelsWCM * m_nPixelsRange );
			}
			else
			{
				nPixelDashStepMax	=	static_cast <int> ( nPixelsWCM / nZoom );
				nPixelsMinDashCount	=	nPixelsWCM;
				nPixelsMinDashStep	=	static_cast <int> ( nZoom );
				if ( nZoom < nPixelsWCM * m_nPixelsRange )
					nPixelsMinDashStep	=	(int) ( nPixelsWCM * m_nPixelsRange );
			}

			// draw rullers text
			if ( m_nRullersUp )	// UP
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), 0, m_nRullerSize, nWindowWidth, m_nRullerSize );	// огранка со стороны картинки

				int nBeginX			=	static_cast <int> ( ( ( - OffsetX * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndX			=	static_cast <int> ( ( nWindowWidth - OffsetX ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					// int LinePosX	=	static_cast <int> ( nBeginX * nZoom + OffsetX );											// позиция следующей черты
					for ( Gdiplus::REAL j = static_cast <Gdiplus::REAL> ( nBeginX * nZoom + OffsetX ); j > 0.0f; j -= (Gdiplus::REAL)nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, j, 0.0f, j, (Gdiplus::REAL) ( m_nRullerSize * 0.25f ) );
				}

				for ( int i = nBeginX; i <= nEndX; i += nPixelDashStepMax )															// для максимальных делений
				{
					int LinePosX	=	static_cast <int> ( i * nZoom + OffsetX );													// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, LinePosX + j, 0, LinePosX + j, static_cast <int> ( m_nRullerSize * 0.25f ) );
					}

					if ( LinePosX < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, LinePosX, 0, LinePosX, m_nRullerSize );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosX + oTextBounding.Width >= nWindowWidth - m_nRullerSize )
							continue;
					}

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( (Gdiplus::REAL)LinePosX + 2.0f, (Gdiplus::REAL)m_nRullerSize - (Gdiplus::REAL)m_nFontSize - 2.0f, 50.0f, (Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush );
				}
			}

			if ( m_nRullersLeft )	// LEFT
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), m_nRullerSize, 0, m_nRullerSize, nWindowHeight );	// огранка со стороны картинки

				int nBeginY			=	static_cast <int> ( ( ( - OffsetY * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndY			=	static_cast <int> ( ( nWindowHeight - OffsetY ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetY ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, 0, j, static_cast <int> ( m_nRullerSize * 0.25f ), j );
				}

				for ( int i = nBeginY; i <= nEndY; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosY	=	static_cast <int> ( i * nZoom + OffsetY );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, 0, LinePosY + j, static_cast <int> ( m_nRullerSize * 0.25f ), LinePosY + j );
					}

					if ( LinePosY < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, 0, LinePosY, m_nRullerSize, LinePosY );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosY - oTextBounding.Width <= m_nRullerSize )
							continue;
					}

					Gdiplus::Matrix oMatrix;
					oMatrix.Translate ( (Gdiplus::REAL)m_nRullerSize - (Gdiplus::REAL)m_nFontSize - 2.0f, (Gdiplus::REAL)LinePosY + 2.0f );
					oMatrix.Rotate ( -90.0f );
					pGraphics->SetTransform ( &oMatrix );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( 
						0.0f,
						0.0f,
						50.0f, 
						(Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 

					pGraphics->SetTransform ( &Gdiplus::Matrix () );
				}
			}

			if ( m_nRullersDown )	// DOWN
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), 0, nWindowHeight - m_nRullerSize, nWindowWidth, nWindowHeight - m_nRullerSize );	// огранка со стороны картинки

				int nBeginX				=	static_cast <int> ( ( ( - OffsetX * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndX				=	static_cast <int> ( ( nWindowWidth - OffsetX ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetX ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, j, nWindowHeight, j, nWindowHeight - static_cast <int> ( m_nRullerSize * 0.25f ) );
				}

				for ( int i = nBeginX; i <= nEndX; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosX	=	static_cast <int> ( i * nZoom + OffsetX );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, LinePosX + j, nWindowHeight, LinePosX + j, nWindowHeight - static_cast <int> ( m_nRullerSize * 0.25f ) );
					}

					if ( LinePosX < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, LinePosX, nWindowHeight, LinePosX, nWindowHeight - m_nRullerSize );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosX + oTextBounding.Width >= nWindowWidth - m_nRullerSize )
							continue;
					}

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( (Gdiplus::REAL)LinePosX + 2.0f, (Gdiplus::REAL)nWindowHeight - (Gdiplus::REAL)m_nFontSize - 2.0f, 50.0f, (Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 
				}
			}

			if ( m_nRullersRight )	// RIGHT
			{
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), nWindowWidth - m_nRullerSize, 0, nWindowWidth - m_nRullerSize, nWindowHeight );	// огранка со стороны картинки

				int nBeginY			=	static_cast <int> ( ( ( - OffsetY * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
				int nEndY			=	static_cast <int> ( ( nWindowHeight - OffsetY ) * UnZoom );

				if ( OffsetX > 0.0 )
				{
					for ( int j = static_cast <int> ( OffsetY ); j > 0; j -= nPixelsMinDashStep )
						pGraphics->DrawLine ( m_pPenMinDash, nWindowWidth, j, nWindowWidth - static_cast <int> ( m_nRullerSize * 0.25f ), j );
				}

				for ( int i = nBeginY; i <= nEndY; i += nPixelDashStepMax )							// для максимальных делений
				{
					int LinePosY	=	static_cast <int> ( i * nZoom + OffsetY );					// позиция следующей черты

					for ( int j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
					{
						pGraphics->DrawLine ( m_pPenMinDash, 
							nWindowWidth, 
							LinePosY + j, 
							nWindowWidth - static_cast <int> ( m_nRullerSize * 0.25f ), 
							LinePosY + j );
					}

					if ( LinePosY < m_nRullerSize )
						continue;

					pGraphics->DrawLine ( m_pPenMinDash, nWindowWidth, LinePosY, nWindowWidth - m_nRullerSize, LinePosY );

					wchar_t Number [ 32 ];
					swprintf_s ( Number, 32, L"%.2f", (double)i / (double)nPixelsWCM );

					//oStringFormat.SetFormatFlags ( Gdiplus::StringFormatFlagsDirectionVertical );

					Gdiplus::RectF oTextBounding;
					Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );
					if ( Gdiplus::Ok == pGraphics->MeasureString ( Number, (INT)wcslen ( Number ), m_pFontH, oLayoutRect, &oStringFormat, &oTextBounding ) )
					{
						if ( LinePosY - oTextBounding.Width <= m_nRullerSize )
							continue;
					}

					Gdiplus::Matrix oMatrix;
					oMatrix.Translate ( (Gdiplus::REAL)nWindowWidth - /*(Gdiplus::REAL)m_nRullerSize -*/ (Gdiplus::REAL)m_nFontSize - 2.0f, (Gdiplus::REAL)LinePosY + 2.0f );
					oMatrix.Rotate ( -90.0f );
					pGraphics->SetTransform ( &oMatrix );

					pGraphics->DrawString ( Number, (INT)wcslen ( Number ), m_pFontH, 
						Gdiplus::RectF ( 
						0.0f,
						0.0f,
						50.0f, 
						(Gdiplus::REAL)m_nFontSize + 2.0f ),
						&oStringFormat, &oBlackSolidBrush ); 

					pGraphics->SetTransform ( &Gdiplus::Matrix () );
				}
			}

			FillRectangles ( pGraphics, nWindowWidth, nWindowHeight );
		}


		inline void FillRectangles ( Gdiplus::Graphics* pGraphics, int nWindowWidth, int nWindowHeight )
		{
			if ( m_nRullersUp || m_nRullersLeft )
			{
				Gdiplus::RectF oRectangle ( Gdiplus::RectF ( 0.0f, 0.0f, (Gdiplus::REAL)m_nRullerSize, (Gdiplus::REAL)m_nRullerSize ) );

				pGraphics->FillRectangle ( new Gdiplus::SolidBrush ( Gdiplus::Color::White ), oRectangle );
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), 0, m_nRullerSize, m_nRullerSize, m_nRullerSize );
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), m_nRullerSize, 0, m_nRullerSize, m_nRullerSize );
			}
			if ( m_nRullersUp || m_nRullersRight )
			{
				Gdiplus::RectF oRectangle ( Gdiplus::RectF ( 
					(Gdiplus::REAL)nWindowWidth		- (Gdiplus::REAL)m_nRullerSize,
					0.0f, 
					(Gdiplus::REAL)m_nRullerSize, 
					(Gdiplus::REAL)m_nRullerSize ));

				pGraphics->FillRectangle ( new Gdiplus::SolidBrush ( Gdiplus::Color::White ), oRectangle );
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), nWindowWidth - m_nRullerSize, 0, nWindowWidth - m_nRullerSize, m_nRullerSize );
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), nWindowWidth, m_nRullerSize, nWindowWidth - m_nRullerSize, m_nRullerSize );
			}
			if ( m_nRullersDown || m_nRullersRight )
			{
				Gdiplus::RectF oRectangle ( Gdiplus::RectF (
					(Gdiplus::REAL)nWindowWidth		- (Gdiplus::REAL)m_nRullerSize, 
					(Gdiplus::REAL)nWindowHeight	- (Gdiplus::REAL)m_nRullerSize, 
					(Gdiplus::REAL)m_nRullerSize, 
					(Gdiplus::REAL)m_nRullerSize ) );

				pGraphics->FillRectangle ( new Gdiplus::SolidBrush ( Gdiplus::Color::White ), oRectangle );
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), nWindowWidth - m_nRullerSize, nWindowHeight - m_nRullerSize, nWindowWidth - m_nRullerSize, nWindowHeight );
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), nWindowWidth - m_nRullerSize, nWindowHeight, nWindowWidth, nWindowHeight );
			}
			if ( m_nRullersDown || m_nRullersLeft )
			{
				Gdiplus::RectF oRectangle ( Gdiplus::RectF ( 
					0.0f, 
					(Gdiplus::REAL)nWindowHeight	- (Gdiplus::REAL)m_nRullerSize, 
					(Gdiplus::REAL)m_nRullerSize, 
					(Gdiplus::REAL)m_nRullerSize ) );

				pGraphics->FillRectangle ( new Gdiplus::SolidBrush ( Gdiplus::Color::White ), oRectangle );
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), 0, nWindowHeight - m_nRullerSize, m_nRullerSize, nWindowHeight - m_nRullerSize );
				pGraphics->DrawLine ( new Gdiplus::Pen ( Gdiplus::Color::Black ), m_nRullerSize, nWindowHeight - m_nRullerSize, m_nRullerSize, nWindowHeight );
			}
		}

	private:

		bool					m_bAlignCenter;
		int						m_nFontSize;

		Gdiplus::Font*			m_pFontH;
		Gdiplus::Font*			m_pFontV;

		Gdiplus::Pen*			m_pPenMinDash;

		int						m_nRullerSize;				// ширина линейки
		int						m_nRullerLineMaxSize;		// длина больших делений
		int						m_nRullerLineMinSize;		// длина малых делений

		bool					m_nRullersUp;
		bool					m_nRullersDown;
		bool					m_nRullersRight;
		bool					m_nRullersLeft;

		long					m_nRullerType;

		int						m_nPixelsWCM;
		int						m_nPixelsHCM;

		double					m_nPixelsRange;
	};

	class CGridRender
	{
	public:

		CGridRender ()
		{
			m_bFullFillLines	=	false;
			m_bAlignCenter		=	false;
			m_nSize				=	1;
			m_pLinesPen			=	NULL;

			SetPenLines ();
		}

		virtual ~CGridRender ()
		{
			RELEASEOBJECT ( m_pLinesPen );
		}

		inline void OnPaint2			( Gdiplus::Graphics* pGraphics, const int& nWindowWidth,
			const int& nWindowHeight, const int& nImageWidth, const int& nImageHeight,
			const double& nZoom, const double& OffsetX, const double& OffsetY,
			const double& nStartDX, const double& nStartDY,
			const double& nEndDX, const double& nEndDY )
		{
			if ( NULL == m_pLinesPen )
				return;

			double UnZoom				=	1.0 / nZoom;

			int nPixelsMinDashCount		=	100;													// длина
			float nPixelsMinDashStep	=	0.0f;
			int nPixelDashStepMax		=	100;

			if ( nZoom < 1.0 )
			{
				nPixelDashStepMax	=	static_cast <int> ( nPixelDashStepMax / nZoom );
				nPixelsMinDashCount	=	100; // nPixelDashStepMax;
				nPixelsMinDashStep	=	(float)m_nSize;
			}
			else
			{
				nPixelDashStepMax	=	static_cast <int> ( 100.0 / nZoom );
				nPixelsMinDashCount	=	100;
				nPixelsMinDashStep	=	static_cast <float> ( nZoom );

				if ( nZoom < m_nSize )
					nPixelsMinDashStep	=	(float)m_nSize;
			}

			int nBeginX			=	static_cast <int> ( ( ( - OffsetX * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
			int nEndX			=	static_cast <int> ( ( nWindowWidth - OffsetX ) * UnZoom );

			int nBeginY			=	static_cast <int> ( ( ( - OffsetY * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
			int nEndY			=	static_cast <int> ( ( nWindowHeight - OffsetY ) * UnZoom );

			double BeginLineX	=	__max ( OffsetX, 0.0 );
			double BeginLineY	=	__max ( OffsetY, 0.0 );
			double EndLineX		=	__min ( (double)nWindowWidth, OffsetX + nZoom * nImageWidth );
			double EndLineY		=	__min ( (double)nWindowHeight, OffsetY + nZoom * nImageHeight );

			for ( int i = nBeginX; i <= nEndX; i += nPixelDashStepMax )															// для максимальных делений
			{
				float LinePosX	=	static_cast <float> ( i * nZoom + OffsetX );													// позиция следующей черты

				if ( ( LinePosX ) >= BeginLineX && ( LinePosX ) <= EndLineX )
				{
					pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL)LinePosX, (Gdiplus::REAL)BeginLineY, (Gdiplus::REAL)LinePosX, (Gdiplus::REAL)EndLineY );
				}

				for ( float j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
				{
					if ( ( LinePosX + j ) >= BeginLineX && ( LinePosX + j ) <= EndLineX )
					{
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL) ( LinePosX + j ), (Gdiplus::REAL)BeginLineY, (Gdiplus::REAL) ( LinePosX + j ), (Gdiplus::REAL)EndLineY );
					}
				}
			}

			for ( int i = nBeginY; i <= nEndY; i += nPixelDashStepMax )							// для максимальных делений
			{
				float LinePosY	=	static_cast <float> ( i * nZoom + OffsetY );					// позиция следующей черты

				if ( ( LinePosY ) >= BeginLineY && ( LinePosY ) <= EndLineY )
				{
					pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL)BeginLineX, (Gdiplus::REAL)LinePosY, (Gdiplus::REAL)EndLineX, (Gdiplus::REAL)LinePosY );
				}

				for ( float j = nPixelsMinDashStep; j < nPixelsMinDashCount - 1; j += nPixelsMinDashStep )
				{
					if ( ( LinePosY + j ) >= BeginLineY && ( LinePosY + j ) <= EndLineY )
					{
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL)BeginLineX, (Gdiplus::REAL) ( LinePosY + j ), (Gdiplus::REAL)EndLineX, (Gdiplus::REAL) ( LinePosY + j ) );
					}
				}
			}
		}

		inline void OnPaint				( Gdiplus::Graphics* pGraphics, const int& nWindowWidth,
			const int& nWindowHeight, const int& nImageWidth, const int& nImageHeight,
			const double& nZoom, const double& OffsetX, const double& OffsetY,
			const double& nStartDX, const double& nStartDY,
			const double& nEndDX, const double& nEndDY )
		{
			double UnZoom				=	1.0 / nZoom;

			int nPixelsMinDashCount		=	100;													// длина
			float nPixelsMinDashStep	=	0.0f;
			int nPixelDashStepMax		=	100;

			if ( nZoom < 1.0 )
			{
				nPixelDashStepMax	=	static_cast <int> ( nPixelDashStepMax / nZoom );
				nPixelsMinDashCount	=	100; // nPixelDashStepMax;
				nPixelsMinDashStep	=	(float)m_nSize;
			}
			else
			{
				nPixelDashStepMax	=	static_cast <int> ( 100.0 / nZoom );
				nPixelsMinDashCount	=	100;
				nPixelsMinDashStep	=	static_cast <float> ( nZoom );

				if ( nZoom < m_nSize )
					nPixelsMinDashStep	=	(float)m_nSize;
			}

			int nBeginX			=	static_cast <int> ( ( ( - OffsetX * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
			int nEndX			=	static_cast <int> ( ( nWindowWidth - OffsetX ) * UnZoom );

			int nBeginY			=	static_cast <int> ( ( ( - OffsetY * UnZoom ) / nPixelDashStepMax ) ) * nPixelDashStepMax;
			int nEndY			=	static_cast <int> ( ( nWindowHeight - OffsetY ) * UnZoom );

			double BeginLineX	=	__max ( OffsetX, 0.0 );
			double BeginLineY	=	__max ( OffsetY, 0.0 );
			double EndLineX		=	__min ( (double)nWindowWidth, OffsetX + nZoom * nImageWidth );
			double EndLineY		=	__min ( (double)nWindowHeight, OffsetY + nZoom * nImageHeight );

			float DrawStep		=	static_cast <float> ( __max ( ImageRendering::Constants::sc_nRenderGridMinSizeCell, m_nSize * nZoom ) );		// минимальный размер CELL

			// центрируем сетку относительно цента отображения
			if ( m_bAlignCenter )
			{
				if ( m_bFullFillLines )			// рисуем сетку по всем видимой области
				{
					float fOffsetX2		=	static_cast <float> ( OffsetX - ( ( long ) ( OffsetX / DrawStep ) ) * DrawStep );
					float fOffsetY2		=	static_cast <float> ( OffsetY - ( ( long ) ( OffsetY / DrawStep ) ) * DrawStep );

					float fOffsetX		=	static_cast <float> ( fOffsetX2 + nZoom * nImageWidth * 0.5 );
					float fOffsetY		=	static_cast <float> ( fOffsetY2 + nZoom * nImageHeight * 0.5 );

					BeginLineX			=	0.0;
					BeginLineY			=	0.0;

					if ( EndLineX < nWindowWidth )
						EndLineX		=	(float)nWindowWidth;

					if ( EndLineY < nWindowHeight )
						EndLineY		=	(float)nWindowHeight;

					for ( float LinePosX = fOffsetX; LinePosX <= EndLineX; LinePosX += DrawStep )												
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL) ( LinePosX  ), (Gdiplus::REAL)BeginLineY, (Gdiplus::REAL) ( LinePosX  ), (Gdiplus::REAL)EndLineY );

					for ( float LinePosY = fOffsetY; LinePosY <= EndLineY; LinePosY += DrawStep )							// для максимальных делений
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL)BeginLineX, (Gdiplus::REAL) ( LinePosY ), (Gdiplus::REAL)EndLineX, (Gdiplus::REAL) ( LinePosY  ) );


					for ( float LinePosX = fOffsetX - DrawStep; LinePosX >= 0.0f; LinePosX -= DrawStep )												
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL) ( LinePosX ), (Gdiplus::REAL)BeginLineY, (Gdiplus::REAL) ( LinePosX  ), (Gdiplus::REAL)EndLineY );

					for ( float LinePosY = fOffsetY - DrawStep; LinePosY >= 0.0f; LinePosY -= DrawStep )							// для максимальных делений
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL)BeginLineX, (Gdiplus::REAL) ( LinePosY ), (Gdiplus::REAL)EndLineX, (Gdiplus::REAL) ( LinePosY  ) );
				}
				else
				{
					float fOffsetX		=	static_cast<float> ( OffsetX + nZoom * nImageWidth	* 0.5 );
					float fOffsetY		=	static_cast<float> ( OffsetY + nZoom * nImageHeight	* 0.5 );

					float fOffsetX2		=	static_cast<float> ( OffsetX );
					float fOffsetY2		=	static_cast<float> ( OffsetY );

					for ( float LinePosX = fOffsetX; LinePosX <= EndLineX; LinePosX += DrawStep )												
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL) ( LinePosX  ), (Gdiplus::REAL)BeginLineY, (Gdiplus::REAL) ( LinePosX  ), (Gdiplus::REAL)EndLineY );

					for ( float LinePosY = fOffsetY; LinePosY <= EndLineY; LinePosY += DrawStep )					
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL)BeginLineX, (Gdiplus::REAL) ( LinePosY ), (Gdiplus::REAL)EndLineX, (Gdiplus::REAL) ( LinePosY  ) );


					for ( float LinePosX = fOffsetX - DrawStep; LinePosX >= fOffsetX2; LinePosX -= DrawStep )												
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL) ( LinePosX ), (Gdiplus::REAL)BeginLineY, (Gdiplus::REAL) ( LinePosX  ), (Gdiplus::REAL)EndLineY );

					for ( float LinePosY = fOffsetY - DrawStep; LinePosY >= fOffsetY2; LinePosY -= DrawStep )	
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL)BeginLineX, (Gdiplus::REAL) ( LinePosY ), (Gdiplus::REAL)EndLineX, (Gdiplus::REAL) ( LinePosY  ) );
				}
			}
			else
			{
				if ( m_bFullFillLines )			// рисуем сетку по всем видимой области
				{
					float fOffsetX		=	static_cast <float> ( OffsetX - ( (long) ( OffsetX / DrawStep ) ) * DrawStep );
					float fOffsetY		=	static_cast <float> ( OffsetY - ( (long) ( OffsetY / DrawStep ) ) * DrawStep );

					BeginLineX			=	0.0;
					BeginLineY			=	0.0;

					if ( EndLineX < nWindowWidth )
						EndLineX		=	(float)nWindowWidth;

					if ( EndLineY < nWindowHeight )
						EndLineY		=	(float)nWindowHeight;

					for ( float LinePosX = fOffsetX; LinePosX <= EndLineX; LinePosX += DrawStep )												
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL) ( LinePosX ), (Gdiplus::REAL)BeginLineY, (Gdiplus::REAL) ( LinePosX  ), (Gdiplus::REAL)EndLineY );

					for ( float LinePosY = fOffsetY; LinePosY <= EndLineY; LinePosY += DrawStep )							// для максимальных делений
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL)BeginLineX, (Gdiplus::REAL) ( LinePosY ), (Gdiplus::REAL)EndLineX, (Gdiplus::REAL) ( LinePosY  ) );
				}
				else
				{
					for ( float LinePosX = (float)OffsetX; LinePosX <= EndLineX; LinePosX += DrawStep )												
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL) ( LinePosX  ), (Gdiplus::REAL)BeginLineY, (Gdiplus::REAL) ( LinePosX  ), (Gdiplus::REAL)EndLineY );

					for ( float LinePosY = (float)OffsetY; LinePosY <= EndLineY; LinePosY += DrawStep )							// для максимальных делений
						pGraphics->DrawLine ( m_pLinesPen, (Gdiplus::REAL)BeginLineX, (Gdiplus::REAL) ( LinePosY ), (Gdiplus::REAL)EndLineX, (Gdiplus::REAL) ( LinePosY  ) );
				}
			}
		}

	public:

		inline void SetCellSize			( long Size )
		{
			m_nSize				=	Size;
		}

		inline void SetFullFillLines	( bool Value )
		{
			m_bFullFillLines	=	Value;
		}

		inline void SetAlignCenter		( bool Value )
		{
			m_bAlignCenter		=	Value;
		}

		inline void SetPenLines			( DWORD ForeColor = 0x0, DWORD BackColor = 0xFF000000, float Width = 1 )
		{
			RELEASEOBJECT ( m_pLinesPen );

			m_pLinesPen			=	new Gdiplus::Pen ( new Gdiplus::HatchBrush ( Gdiplus::HatchStyle50Percent, Gdiplus::Color ( ForeColor ), Gdiplus::Color ( BackColor ) ), Width );
		}


	private:

		bool			m_bFullFillLines;
		bool			m_bAlignCenter;
		Gdiplus::Pen*	m_pLinesPen;
		int				m_nSize;
	};

	class CChessBrush
	{
	public:

		CChessBrush () 
		{
			m_pBrush		=	NULL;

			m_nDarkColor	=	0xFFCCCCCC;
			m_nLightColor	=	0xFFFFFFFF;
			m_nSizesCell	=	16;

			CreateBrush ();
		}

		~CChessBrush () 
		{
			Clear ();
		}



		inline void OnPaint ( Gdiplus::Graphics* pGraphics, Gdiplus::Rect rc )
		{
			rc.Width	= ( (long)rc.Width / 2 ) * 2;
			rc.Height	= ( (long)rc.Height / 2 ) * 2;

			if ( NULL != m_pBrush )
			{
				pGraphics->FillRectangle ( m_pBrush, rc );
			}
			else
			{
				pGraphics->FillRectangle ( new HatchBrush ( Gdiplus::HatchStyleLargeCheckerBoard, Gdiplus::Color ( m_nLightColor ),
					Gdiplus::Color ( m_nDarkColor ) ),  rc );
			}
		}

		inline DWORD GetDarkColor	()	{	return m_nDarkColor;	}
		inline DWORD GetLightColor	()	{	return m_nLightColor;	}
		inline DWORD GetSizesCell	()	{	return m_nSizesCell;	}
		inline void SetDarkColor	( DWORD Value )	{	m_nDarkColor	=	Value; Clear (); CreateBrush (); }
		inline void SetLightColor	( DWORD Value )	{	m_nLightColor	=	Value; Clear (); CreateBrush (); }
		inline void SetSizesCell	( DWORD Value )	{	m_nSizesCell	=	Value; Clear (); CreateBrush (); }

	private:

		inline void Clear ()
		{
			if ( NULL != m_pBrush )
			{
				delete m_pBrush;
				m_pBrush	=	NULL;
			}
		}

		inline void CreateBrush ()
		{
			Gdiplus::Bitmap oBitmap ( m_nSizesCell * 2, m_nSizesCell * 2, PixelFormat32bppRGB );

			if ( Gdiplus::Ok == oBitmap.GetLastStatus() )
			{
				Gdiplus::Graphics oGraphics ( &oBitmap );
				if ( Gdiplus::Ok == oGraphics.GetLastStatus() )
				{
					Gdiplus::REAL fWidthCell = (Gdiplus::REAL)m_nSizesCell;

					oGraphics.FillRectangle ( new SolidBrush ( Color ( m_nDarkColor  ) ), Gdiplus::RectF ( 0.0f, 0.0f, fWidthCell, fWidthCell ) );
					oGraphics.FillRectangle ( new SolidBrush ( Color ( m_nDarkColor  ) ), Gdiplus::RectF ( fWidthCell, fWidthCell, fWidthCell, fWidthCell ) );
					oGraphics.FillRectangle ( new SolidBrush ( Color ( m_nLightColor ) ), Gdiplus::RectF ( 0.0f, fWidthCell, fWidthCell, fWidthCell ) );
					oGraphics.FillRectangle ( new SolidBrush ( Color ( m_nLightColor ) ), Gdiplus::RectF ( fWidthCell, 0.0f, fWidthCell, fWidthCell ) );

					if ( Gdiplus::Ok == oGraphics.GetLastStatus() )
					{
						if ( Gdiplus::Ok == oBitmap.GetLastStatus() )
						{
							m_pBrush = new Gdiplus::TextureBrush ( &oBitmap );
						}
					}
				}
			}
		}

	private:

		Gdiplus::TextureBrush*	m_pBrush;

		DWORD					m_nDarkColor;
		DWORD					m_nLightColor;
		DWORD					m_nSizesCell;
	};


}

namespace ImageRendering
{
	class CViewManager : public ICoordinatSystem
	{
#pragma region MACROSES

#define PROPERTY_SET(NAME,TYPE)			virtual void Set##NAME ( TYPE Value )				{	m_n##NAME	=	Value;	Update (); }
#define PROPERTY_GET_CONST(NAME,TYPE)	virtual const TYPE& Get##NAME () const				{	return	m_n##NAME;	}
#define PROPERTY_SIMPLE(NAME,TYPE) PROPERTY_SET (NAME,TYPE) PROPERTY_GET_CONST (NAME,TYPE)

#pragma endregion

	private:
		class CGdiPlusInit
		{
		public:
			CGdiPlusInit ()
			{
				Start();
			}

			~CGdiPlusInit ()
			{
				Stop();
			}

			void Start()
			{
				Gdiplus::GdiplusStartup(&m_token, &m_input, NULL);
			}

			void Stop()
			{
				Gdiplus::GdiplusShutdown(m_token);
			}

		private:

			ULONG_PTR m_token;
			Gdiplus::GdiplusStartupInput m_input;
		};


	public:

		CViewManager ()
		{
			m_pImage				=	NULL;
			m_hWnd          		=   NULL;

			m_nChannels				=	4;
			m_nImageWidth			=	0;
			m_nImageHeight			=	0;

			// размеры области вывода
			m_nWindowWidth			=	0;
			m_nWindowHeight			=	0;

			m_bUpdateWindowSizes	=	false;

			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForNone;

			memset ( RENDER_FLAGS, 0, 512 );

			RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeUseAlphaLayer					]	=	TRUE;
			RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeUseCopyImage						]	=	TRUE;
			RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeEnableImageSurface				]	=	TRUE;
			RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeDisabledZoomToFitForSmallImages	]	=	TRUE;
			RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeFastCopyBuffersWithBlackFone		]	=	FALSE;

			//m_nRenderMode			=	ImageRendering::Constants::sc_RenderModeUseAlphaLayer						| 
			//							ImageRendering::Constants::sc_RenderModeUseCopyImage						|
			//							ImageRendering::Constants::sc_RenderModeEnableImageSurface					|
			//							ImageRendering::Constants::sc_RenderModeDisabledZoomToFitForSmallImages;
			//// специальный режим включается в ручную в клиентской программе
			//m_nRenderMode			=	m_nRenderMode & ( ~ImageRendering::Constants::sc_RenderModeFastCopyBuffersWithBlackFone );


			m_nInterpolationMode	=	ImageRendering::PixelsUtils::sc_nInterpolationModeNearestNeighbor;
			m_bZoomToWindow			=	false;

			m_nCenterX				=	0;
			m_nCenterY				=	0;

			m_nZoom					=	1.0;

			m_nPreviewLeft			=	0.0;
			m_nPreviewRight			=	0.0;
			m_nPreviewTop			=	0.0;
			m_nPreviewBottom		=	0.0;

			m_nTransformLeft		=	0.0;
			m_nTransformRight		=	0.0;
			m_nTransformTop			=	0.0;
			m_nTransformBottom		=	0.0;

			m_nBackClearColor		=	0xFFD0D0D0;

			m_nEnableGrid			=	false;
			m_nGridMode				=	0;		// 0 - pixels, 1 - сантиметры, 2 - дюймы

			m_nRullersUp			=	false;
			m_nRullersDown			=	false;
			m_nRullersRight			=	false;
			m_nRullersLeft			=	false;

			m_pGraphics				=	NULL;
			m_pBitmap				=	NULL;
			m_pBitmap2				=	NULL;
			m_pGraphics2			=	NULL;
			m_pBitmapAlpha			=	NULL;
			m_pBitmapCopy			=	NULL;
			m_pRefImage				=	NULL;

			m_nScaleInnerImage		=	1.0;
			m_nZoomToFitOffsetPX	=	15.0;

			m_nImageWidthReal		=	-1;
			m_nImageHeightReal		=	-1;
			
			m_pbScreenBuffer		=	NULL;
		}

		virtual ~CViewManager ()
		{
			Clear ();
		}


		inline void Clear ()
		{
			RELEASEOBJECT ( m_pGraphics );
			RELEASEOBJECT ( m_pBitmap );
			RELEASEOBJECT ( m_pBitmap2 );
			RELEASEOBJECT ( m_pGraphics2 );
			RELEASEOBJECT ( m_pBitmapAlpha );
			RELEASEOBJECT ( m_pBitmapCopy );
		}

		inline void SetHWND ( HWND hwnd )
		{
			m_hWnd = hwnd;
		}

		inline HWND GetHWND()            {return m_hWnd;}

		inline bool BeginPaint ( HDC hDC, RECT& WindowRC, bool bZoomToWindow = false )
		{
			if ( FALSE == IsWindow ( m_hWnd ) )
				return false;

			m_hDC						=	hDC;
			m_RECT						=	WindowRC;
			m_pbScreenBuffer			=	NULL;

			int nWindowWidth			=	abs ( m_RECT.right	- m_RECT.left );
			int nWindowHeight			=	abs ( m_RECT.bottom	- m_RECT.top  );

			// ATLTRACE ( _T("Window : Width - %d, Height - %d \n"), nWindowWidth, nWindowHeight );

			if ( m_nWindowWidth != nWindowWidth || m_nWindowHeight != nWindowHeight )
			{
				m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
				m_bUpdateWindowSizes	=	true;

				m_nWindowWidth			=	nWindowWidth;
				m_nWindowHeight			=	nWindowHeight;
			}

			if ( m_nWindowWidth <= 0 || m_nWindowHeight <= 0 || NULL == m_pImage )
			{
				FillRect ( hDC, &m_RECT, GetStockBrush ( DKGRAY_BRUSH ) );
				return false;
			}

			if ( bZoomToWindow || m_bZoomToWindow )
			{
				ZoomToWindow ();
			}

			ComputeRectangle ();

			if ( m_bUpdateWindowSizes )
			{
				RELEASEOBJECT ( m_pBitmap );
				m_pBitmap				=	new	Gdiplus::Bitmap ( m_nWindowWidth, m_nWindowHeight, PixelFormat32bppARGB );

				RELEASEOBJECT ( m_pBitmap2 );
				m_pBitmap2				=	new	Gdiplus::Bitmap ( m_nWindowWidth, m_nWindowHeight, PixelFormat32bppARGB );

				RELEASEOBJECT ( m_pGraphics2 );
				m_pGraphics2			=	new	Gdiplus::Graphics ( m_pBitmap2 );

				if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeUseCopyImage ] )
				{
					RELEASEOBJECT ( m_pBitmapCopy );
					m_pBitmapCopy			=	new Gdiplus::Bitmap ( m_nWindowWidth, m_nWindowHeight, PixelFormat32bppARGB );
				}

				m_bUpdateWindowSizes	=	false;
				m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
			}

			if ( m_nZoom > 0.0 )
			{
				if ( ImageRendering::Constants::sc_nRefreshForRebuild == m_nRefreshImage )
				{
					if ( NULL != m_pGraphics2 )
					{
						if ( Gdiplus::Ok == m_pGraphics2->GetLastStatus () )
						{
							m_pGraphics2->SetCompositingQuality ( Gdiplus::CompositingQualityHighSpeed );
							m_pGraphics2->SetSmoothingMode ( Gdiplus::SmoothingModeHighSpeed );

							// подготовим кусок изображения в зависимости от центра и зума
							if ( ImageRendering::PixelsUtils::sc_nInterpolationModeNone != m_nInterpolationMode )
							{
								if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeUseAlphaLayer ] )
								{
									long nStartDX	=	GetStartDX ();
									long nEndDX		=	GetEndDX ();
									long nStartDY	=	GetStartDY ();
									long nEndDY		=	GetEndDY ();

									double OffSetX		=	0.0;
									double OffSetY		=	0.0;
									if ( m_nScaleInnerImage != 1.0 )
									{
										GetScalePaintingBound ( m_nZoom, OffSetX, OffSetY, nStartDX, nStartDY, nEndDX, nEndDY );
									}

									if ( m_bZoomToWindow )
									{
										m_pGraphics2->Clear ( m_nBackClearColor );

										if ( ImageRendering::PixelsUtils::sc_nInterpolationModeBilinear == m_nInterpolationMode )
										{
											m_oChessBrush.OnPaint ( m_pGraphics2, Gdiplus::Rect ( nStartDX, nStartDY, nEndDX - nStartDX, nEndDY - nStartDY ) );
										}
										else
										{
											m_oChessBrush.OnPaint ( m_pGraphics2, Gdiplus::Rect ( nStartDX, nStartDY, nEndDX - nStartDX, nEndDY - nStartDY ) );
										}
									}
									else
									{
										Gdiplus::RectF UpRect		( 0.0f, 0.0f,					(Gdiplus::REAL)m_nWindowWidth,	(Gdiplus::REAL)nStartDY );
										Gdiplus::RectF DownRect		( 0.0f, (Gdiplus::REAL)nEndDY /*- 12.0f*/,	(Gdiplus::REAL)m_nWindowWidth,	(Gdiplus::REAL)m_nWindowHeight - (Gdiplus::REAL)nEndDY /*+ 24.0f */);
										Gdiplus::RectF LeftRect		( 0.0f, 0.0f,					(Gdiplus::REAL)nStartDX,		(Gdiplus::REAL)m_nWindowHeight );
										Gdiplus::RectF RightRect	( (Gdiplus::REAL)nEndDX /*- 12.0f*/, 0.0f, (Gdiplus::REAL)GetStartDX () + (Gdiplus::REAL)m_nAlphaWidth /*+ 24.0f*/,	(Gdiplus::REAL)m_nWindowHeight );

										Gdiplus::SolidBrush oSolidBrush ( m_nBackClearColor );

										if ( ImageRendering::PixelsUtils::sc_nInterpolationModeBilinear == m_nInterpolationMode )
										{
											m_oChessBrush.OnPaint ( m_pGraphics2, Gdiplus::Rect ( m_nStartDX, m_nStartDY, m_nAlphaWidth - 1, m_nAlphaHeight - 1 ) );

											DownRect	=	Gdiplus::RectF	( 0.0f, (Gdiplus::REAL)nEndDY - 2.0f, (Gdiplus::REAL)m_nWindowWidth, (Gdiplus::REAL)m_nWindowHeight - (Gdiplus::REAL)nEndDY + 4.0f );
											RightRect	=	Gdiplus::RectF	( (Gdiplus::REAL)nEndDX - 2.0f, 0.0f, (Gdiplus::REAL)GetStartDX () + (Gdiplus::REAL)m_nAlphaWidth, (Gdiplus::REAL)m_nWindowHeight + 4.0f );
										}
										else
										{
											m_oChessBrush.OnPaint ( m_pGraphics2, Gdiplus::Rect ( m_nStartDX, m_nStartDY, m_nAlphaWidth + 1, m_nAlphaHeight + 1 ) );
										}

										if ( UpRect.Height > 0 )
										{
											if ( m_nRullersUp &&  UpRect.Height > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, UpRect );
											else 
												if ( false == m_nRullersUp )
													m_pGraphics2->FillRectangle ( &oSolidBrush, UpRect );
										}
										
										if ( DownRect.Height > 0 )
										{
											if ( m_nRullersDown && DownRect.Height > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, DownRect );
											else 
												if ( m_nRullersDown == false )
													m_pGraphics2->FillRectangle ( &oSolidBrush, DownRect );
										}

										if ( LeftRect.Width > 0 )
										{
											if ( m_nRullersLeft && LeftRect.Width > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, LeftRect );
											else 
												if ( false == m_nRullersLeft )
													m_pGraphics2->FillRectangle ( &oSolidBrush, LeftRect );
										}
										
										if ( RightRect.Width > 0 )
										{
											if ( m_nRullersRight && RightRect.Width > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, RightRect );
											else 
												if ( m_nRullersRight == false )
													m_pGraphics2->FillRectangle ( &oSolidBrush, RightRect );
										}
									}
								}
								else
								{
									m_pGraphics2->Clear ( m_nBackClearColor );
								}

								PrepareFrontImage ();

								m_pGraphics2->SetCompositingQuality ( Gdiplus::CompositingQualityHighSpeed );
								m_pGraphics2->SetSmoothingMode ( Gdiplus::SmoothingModeHighSpeed );

								// используем быстрое копирование картинки ( специальный режим, если критична скорость )
								if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeFastCopyBuffersWithBlackFone ] )
								{
									ImageUtils::MemCopyBitmap ( m_pBitmap, m_pBitmap2 );
								}
								else
								{
									m_pGraphics2->DrawImage ( m_pBitmap, 0, 0, m_pBitmap->GetWidth (), m_pBitmap->GetHeight () );
								}

								//CStringW ImageResize; ImageResize.Format ( L"ImageNearest : %f ", m_nDebugFastResize );
								//Gdiplus::SolidBrush	oBlackSolidBrush ( Gdiplus::Color::White );
								//Gdiplus::StringFormat	oStringFormat;
								//m_pGraphics2->DrawString ( ImageResize, -1, new Gdiplus::Font ( L"Times", 10 ), Gdiplus::RectF ( 20, 20, 500, 200 ), &oStringFormat, &oBlackSolidBrush );

								if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeUseCopyImage ] )
								{
									if ( NULL != m_pBitmapCopy )
									{
										ImageUtils::MemCopyBitmap ( m_pBitmap2, m_pBitmapCopy );
									}
								}
							}
						}
					}
				}

				if ( ImageRendering::PixelsUtils::sc_nInterpolationModeNone != m_nInterpolationMode )
				{
					if ( ImageRendering::Constants::sc_nRefreshForCommand == m_nRefreshImage )
					{
						if ( NULL != m_pGraphics2 )
						{
							if ( Gdiplus::Ok == m_pGraphics2->GetLastStatus () )
							{
								if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeUseCopyImage ] )
								{
									if ( NULL != m_pBitmapCopy )
									{
										ImageUtils::MemCopyBitmap ( m_pBitmapCopy, m_pBitmap2 );
									}

									m_pGraphics2->SetCompositingQuality ( Gdiplus::CompositingQualityHighSpeed );
									m_pGraphics2->SetSmoothingMode ( Gdiplus::SmoothingModeHighSpeed );
								}
							}
						}
					}
				}
			}	

			return true;
		}

		inline bool BeginPaint ( HDC hDC, RECT& WindowRC, BYTE* pScreenBuffer, bool bZoomToWindow = false )
		{
			if ( FALSE == IsWindow ( m_hWnd ) )
				return false;

			m_hDC						=	hDC;
			m_RECT						=	WindowRC;
			m_pbScreenBuffer			=	pScreenBuffer;

			int nWindowWidth			=	abs ( m_RECT.right	- m_RECT.left );
			int nWindowHeight			=	abs ( m_RECT.bottom	- m_RECT.top  );

			// ATLTRACE ( _T("Window : Width - %d, Height - %d \n"), nWindowWidth, nWindowHeight );

			if ( m_nWindowWidth != nWindowWidth || m_nWindowHeight != nWindowHeight )
			{
				m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
				m_bUpdateWindowSizes	=	true;

				m_nWindowWidth			=	nWindowWidth;
				m_nWindowHeight			=	nWindowHeight;
			}

			if ( m_nWindowWidth <= 0 || m_nWindowHeight <= 0 || NULL == m_pImage )
			{
				// FillRect ( hDC, &m_RECT, GetStockBrush ( DKGRAY_BRUSH ) );
				return false;
			}

			if ( bZoomToWindow || m_bZoomToWindow )
			{
				ZoomToWindow ();
			}

			ComputeRectangle ();

			if ( NULL != m_pbScreenBuffer )
			{
				RELEASEOBJECT ( m_pBitmap2 );
				m_pBitmap2				=	new	Gdiplus::Bitmap ( m_nWindowWidth, m_nWindowHeight, m_nWindowWidth * 4, PixelFormat32bppARGB, m_pbScreenBuffer );

				RELEASEOBJECT ( m_pGraphics2 );
				m_pGraphics2			=	new	Gdiplus::Graphics ( m_pBitmap2 );
			}

			if ( m_bUpdateWindowSizes )
			{
				RELEASEOBJECT ( m_pBitmap );
				m_pBitmap				=	new	Gdiplus::Bitmap ( m_nWindowWidth, m_nWindowHeight, PixelFormat32bppARGB );

				if ( NULL == m_pbScreenBuffer )
				{
					RELEASEOBJECT ( m_pBitmap2 );
					m_pBitmap2				=	new	Gdiplus::Bitmap ( m_nWindowWidth, m_nWindowHeight, PixelFormat32bppARGB );

					RELEASEOBJECT ( m_pGraphics2 );
					m_pGraphics2			=	new	Gdiplus::Graphics ( m_pBitmap2 );
				}
				
				if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeUseCopyImage ] )
				{
					RELEASEOBJECT ( m_pBitmapCopy );
					m_pBitmapCopy			=	new Gdiplus::Bitmap ( m_nWindowWidth, m_nWindowHeight, PixelFormat32bppARGB );
				}

				m_bUpdateWindowSizes	=	false;
				m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
			}
			
			if ( m_nZoom > 0.0 )
			{
				if ( ImageRendering::Constants::sc_nRefreshForRebuild == m_nRefreshImage )
				{
					if ( NULL != m_pGraphics2 )
					{
						if ( Gdiplus::Ok == m_pGraphics2->GetLastStatus () )
						{
							m_pGraphics2->SetCompositingQuality ( Gdiplus::CompositingQualityHighSpeed );
							m_pGraphics2->SetSmoothingMode ( Gdiplus::SmoothingModeHighSpeed );

							// подготовим кусок изображения в зависимости от центра и зума
							if ( ImageRendering::PixelsUtils::sc_nInterpolationModeNone != m_nInterpolationMode )
							{
								if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeUseAlphaLayer ] )
								{
									long nStartDX	=	GetStartDX ();
									long nEndDX		=	GetEndDX ();
									long nStartDY	=	GetStartDY ();
									long nEndDY		=	GetEndDY ();

									double OffSetX		=	0.0;
									double OffSetY		=	0.0;
									if ( m_nScaleInnerImage != 1.0 )
									{
										GetScalePaintingBound ( m_nZoom, OffSetX, OffSetY, nStartDX, nStartDY, nEndDX, nEndDY );
									}

									if ( m_bZoomToWindow )
									{
										m_pGraphics2->Clear ( m_nBackClearColor );

										if ( ImageRendering::PixelsUtils::sc_nInterpolationModeBilinear == m_nInterpolationMode )
										{
											m_oChessBrush.OnPaint ( m_pGraphics2, Gdiplus::Rect ( nStartDX, nStartDY, nEndDX - nStartDX, nEndDY - nStartDY ) );
										}
										else
										{
											m_oChessBrush.OnPaint ( m_pGraphics2, Gdiplus::Rect ( nStartDX, nStartDY, nEndDX - nStartDX, nEndDY - nStartDY ) );
										}
									}
									else
									{
										Gdiplus::RectF UpRect		( 0.0f, 0.0f,					(Gdiplus::REAL)m_nWindowWidth,	(Gdiplus::REAL)nStartDY );
										Gdiplus::RectF DownRect		( 0.0f, (Gdiplus::REAL)nEndDY /*- 12.0f*/,	(Gdiplus::REAL)m_nWindowWidth,	(Gdiplus::REAL)m_nWindowHeight - (Gdiplus::REAL)nEndDY /*+ 24.0f */);
										Gdiplus::RectF LeftRect		( 0.0f, 0.0f,					(Gdiplus::REAL)nStartDX,		(Gdiplus::REAL)m_nWindowHeight );
										Gdiplus::RectF RightRect	( (Gdiplus::REAL)nEndDX /*- 12.0f*/, 0.0f, (Gdiplus::REAL)GetStartDX () + (Gdiplus::REAL)m_nAlphaWidth /*+ 24.0f*/,	(Gdiplus::REAL)m_nWindowHeight );

										Gdiplus::SolidBrush oSolidBrush ( m_nBackClearColor );

										if ( ImageRendering::PixelsUtils::sc_nInterpolationModeBilinear == m_nInterpolationMode )
										{
											m_oChessBrush.OnPaint ( m_pGraphics2, Gdiplus::Rect ( m_nStartDX, m_nStartDY, m_nAlphaWidth - 1, m_nAlphaHeight - 1 ) );

											DownRect	=	Gdiplus::RectF	( 0.0f, (Gdiplus::REAL)nEndDY - 2.0f, (Gdiplus::REAL)m_nWindowWidth, (Gdiplus::REAL)m_nWindowHeight - (Gdiplus::REAL)nEndDY + 4.0f );
											RightRect	=	Gdiplus::RectF	( (Gdiplus::REAL)nEndDX - 2.0f, 0.0f, (Gdiplus::REAL)GetStartDX () + (Gdiplus::REAL)m_nAlphaWidth, (Gdiplus::REAL)m_nWindowHeight + 4.0f );
										}
										else
										{
											m_oChessBrush.OnPaint ( m_pGraphics2, Gdiplus::Rect ( m_nStartDX, m_nStartDY, m_nAlphaWidth + 1, m_nAlphaHeight + 1 ) );
										}

										if ( UpRect.Height > 0 )
										{
											if ( m_nRullersUp &&  UpRect.Height > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, UpRect );
											else 
												if ( false == m_nRullersUp )
													m_pGraphics2->FillRectangle ( &oSolidBrush, UpRect );
										}
										
										if ( DownRect.Height > 0 )
										{
											if ( m_nRullersDown && DownRect.Height > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, DownRect );
											else 
												if ( m_nRullersDown == false )
													m_pGraphics2->FillRectangle ( &oSolidBrush, DownRect );
										}

										if ( LeftRect.Width > 0 )
										{
											if ( m_nRullersLeft && LeftRect.Width > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, LeftRect );
											else 
												if ( false == m_nRullersLeft )
													m_pGraphics2->FillRectangle ( &oSolidBrush, LeftRect );
										}
										
										if ( RightRect.Width > 0 )
										{
											if ( m_nRullersRight && RightRect.Width > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, RightRect );
											else 
												if ( m_nRullersRight == false )
													m_pGraphics2->FillRectangle ( &oSolidBrush, RightRect );
										}
									}
								}
								else
								{
									m_pGraphics2->Clear ( m_nBackClearColor );
								}

								PrepareFrontImage ();

								m_pGraphics2->SetCompositingQuality ( Gdiplus::CompositingQualityHighSpeed );
								m_pGraphics2->SetSmoothingMode ( Gdiplus::SmoothingModeHighSpeed );

								if ( NULL != pScreenBuffer )
								{
									// используем быстрое копирование картинки ( специальный режим, если критична скорость )
									if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeFastCopyBuffersWithBlackFone ] )
									{
										ImageUtils::MemCopyBitmap ( m_pBitmap, m_pBitmap2 );

										/*

										// FOR CLEAR ALPHA

										for ( int i = 0; i < m_nWindowHeight; ++i )
										{
											for ( int j = 0; j < m_nWindowWidth; ++j )
											{
												BYTE Buffer = pScreenBuffer [ ( i * m_nWindowWidth + j ) * 4 + 3 ];
												if ( Buffer != 255 )
												{
													* ( ((DWORD*) pScreenBuffer) + i * m_nWindowWidth + j ) = m_nBackClearColor;
												}
											}
										}
										
										*/

										///////////////////////////////////////////////////////////////
										
										long nStartDX	=	GetStartDX ();
										long nEndDX		=	GetEndDX ();
										long nStartDY	=	GetStartDY ();
										long nEndDY		=	GetEndDY ();

										double OffSetX		=	0.0;
										double OffSetY		=	0.0;
										if ( m_nScaleInnerImage != 1.0 )
										{
											GetScalePaintingBound ( m_nZoom, OffSetX, OffSetY, nStartDX, nStartDY, nEndDX, nEndDY );
										}

										Gdiplus::RectF UpRect		( 0.0f, 0.0f,					(Gdiplus::REAL)m_nWindowWidth,	(Gdiplus::REAL)nStartDY );
										Gdiplus::RectF DownRect		( 0.0f, (Gdiplus::REAL)nEndDY /*- 12.0f*/,	(Gdiplus::REAL)m_nWindowWidth,	(Gdiplus::REAL)m_nWindowHeight - (Gdiplus::REAL)nEndDY /*+ 24.0f */);
										Gdiplus::RectF LeftRect		( 0.0f, 0.0f,					(Gdiplus::REAL)nStartDX,		(Gdiplus::REAL)m_nWindowHeight );
										Gdiplus::RectF RightRect	( (Gdiplus::REAL)nEndDX /*- 12.0f*/, 0.0f, (Gdiplus::REAL)GetStartDX () + (Gdiplus::REAL)m_nAlphaWidth /*+ 24.0f*/,	(Gdiplus::REAL)m_nWindowHeight );

										Gdiplus::SolidBrush oSolidBrush ( m_nBackClearColor );

										if ( UpRect.Height > 0 )
										{
											if ( m_nRullersUp &&  UpRect.Height > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, UpRect );
											else 
												if ( false == m_nRullersUp )
													m_pGraphics2->FillRectangle ( &oSolidBrush, UpRect );
										}
										
										if ( DownRect.Height > 0 )
										{
											if ( m_nRullersDown && DownRect.Height > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, DownRect );
											else 
												if ( m_nRullersDown == false )
													m_pGraphics2->FillRectangle ( &oSolidBrush, DownRect );
										}

										if ( LeftRect.Width > 0 )
										{
											if ( m_nRullersLeft && LeftRect.Width > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, LeftRect );
											else 
												if ( false == m_nRullersLeft )
													m_pGraphics2->FillRectangle ( &oSolidBrush, LeftRect );
										}
										
										if ( RightRect.Width > 0 )
										{
											if ( m_nRullersRight && RightRect.Width > m_oRullers.GetRullerSize() )
												m_pGraphics2->FillRectangle ( &oSolidBrush, RightRect );
											else 
												if ( m_nRullersRight == false )
													m_pGraphics2->FillRectangle ( &oSolidBrush, RightRect );
										}

										///////////////////////////////////////////////////////////////
									}
									else
									{
										m_pGraphics2->DrawImage ( m_pBitmap, 0, 0, m_pBitmap->GetWidth (), m_pBitmap->GetHeight () );
									}
								}
								else
								{
									m_pGraphics2->DrawImage ( m_pBitmap, 0, 0, m_pBitmap->GetWidth (), m_pBitmap->GetHeight () );
								}
								
								//CStringW ImageResize; ImageResize.Format ( L"ImageNearest : %f ", m_nDebugFastResize );
								//Gdiplus::SolidBrush	oBlackSolidBrush ( Gdiplus::Color::White );
								//Gdiplus::StringFormat	oStringFormat;
								//m_pGraphics2->DrawString ( ImageResize, -1, new Gdiplus::Font ( L"Times", 10 ), Gdiplus::RectF ( 20, 20, 500, 200 ), &oStringFormat, &oBlackSolidBrush );

								if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeUseCopyImage ] )
								{
									if ( NULL != m_pBitmapCopy )
									{
										ImageUtils::MemCopyBitmap ( m_pBitmap2, m_pBitmapCopy );
									}
								}
							}
						}
					}
				}

				if ( ImageRendering::PixelsUtils::sc_nInterpolationModeNone != m_nInterpolationMode )
				{
					if ( ImageRendering::Constants::sc_nRefreshForCommand == m_nRefreshImage )
					{
						if ( NULL != m_pGraphics2 )
						{
							if ( Gdiplus::Ok == m_pGraphics2->GetLastStatus () )
							{
								if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeUseCopyImage ] )
								{
									if ( NULL != m_pBitmapCopy )
									{
										ImageUtils::MemCopyBitmap ( m_pBitmapCopy, m_pBitmap2 );
									}

									m_pGraphics2->SetCompositingQuality ( Gdiplus::CompositingQualityHighSpeed );
									m_pGraphics2->SetSmoothingMode ( Gdiplus::SmoothingModeHighSpeed );
								}
							}
						}
					}
				}
			}	

			return true;
		}

		inline bool EndPaint ()
		{
			if ( FALSE == IsWindow ( m_hWnd ) )
				return false;

			if ( m_nZoom > 0.0 )
			{
				if ( NULL != m_pGraphics2 )
				{
					if ( Gdiplus::Ok == m_pGraphics2->GetLastStatus () )
					{
						if ( ImageRendering::Constants::sc_nRefreshForRebuild == m_nRefreshImage )
						{
							RenderGrid ();
							RenderRullers ();
						}
					}
				}
			}

			if ( ImageRendering::Constants::sc_nRefreshForRebuild == m_nRefreshImage )
			{
				m_nRefreshImage		=	ImageRendering::Constants::sc_nRefreshForNone;
			}

			if ( NULL == m_pbScreenBuffer )
			{
				RenderBackBitmapNonCopyMode ();
			}

			return true;
		}

		inline Gdiplus::Bitmap* GetDrawImage ()		// получить изображение которое будет отрисовано с учетом zomm, положения центра
		{
			return m_pBitmap;
		}
		inline Gdiplus::Graphics* GetPainter ()
		{
			return m_pGraphics2;
		}


		inline void SetImage ( const BYTE* pImage, int Width, int Height, double ScaleInnerImage = 1.0, int RealWidth = -1, int RealHeight = -1, BYTE nChannels = 4 )
		{
			m_pImage				=	const_cast<BYTE*>(pImage);

			m_nImageWidth			=	Width;
			m_nImageHeight			=	Height;
			m_nChannels				=	nChannels;
			m_nScaleInnerImage		=	ScaleInnerImage;

			m_nImageWidthReal		=	RealWidth;
			m_nImageHeightReal		=	RealHeight;

			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;

			//ATLTRACE ( _T("ImW : %d, ImH : %d, ImageWidth : %f, ImageHeight : %f, ScaleInnerImage : %f\n"), Width, Height, (double)m_nImageWidth * m_nScaleInnerImage, (double)m_nImageHeight * m_nScaleInnerImage, m_nScaleInnerImage );

			//RELEASEOBJECT ( m_pRefImage )
			//m_pRefImage = new Gdiplus::Bitmap ( m_nImageWidth, m_nImageHeight, m_nImageWidth * 4, PixelFormat32bppRGB, m_pImage );

			/*

			CLSID ImageGUID;
			ImageRendering::ImageUtils::GetEncoderCLSID ( L"image/png", &ImageGUID );
			Gdiplus::Bitmap SetBitmap ( m_nImageWidth, m_nImageHeight, 4 * m_nImageWidth, PixelFormat32bppARGB, m_pImage );

			static long i = 0;
			i++;
			wchar_t buffer [256];
			wsprintfW ( buffer, L"C:\\SetImage_%i.png", i);
			SetBitmap.Save ( buffer , &ImageGUID );

			*/
		}

		inline void ZoomToWindow ()
		{
			long nViewWidth		=	m_nWindowWidth;
			long nViewHeight	=	m_nWindowHeight;

			if ( m_nRullersLeft )
				nViewWidth -= m_oRullers.GetRullerSize ();// + 1;
			if ( m_nRullersRight )
				nViewWidth -= m_oRullers.GetRullerSize ();

			if ( m_nRullersUp )
				nViewHeight -= m_oRullers.GetRullerSize ();// + 1;
			if ( m_nRullersDown )
				nViewHeight -= m_oRullers.GetRullerSize ();

			if ( nViewWidth <= 0 || nViewHeight <= 0 || NULL == m_pImage )
				return;

			double CenterX		=	0.5 * m_nImageWidth		* m_nScaleInnerImage;
			double CenterY		=	0.5 * m_nImageHeight	* m_nScaleInnerImage;

			double ZoomTo		=	min ( ( (double)nViewWidth - m_nZoomToFitOffsetPX ) / ( m_nScaleInnerImage * (double) m_nImageWidth ), ( (double) nViewHeight - m_nZoomToFitOffsetPX ) / ( m_nScaleInnerImage * (double) m_nImageHeight ) );
			
			// кламп по масштабу для малых изображений по сравнению с областью видимости
			if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeDisabledZoomToFitForSmallImages ] )
			{
				ZoomTo			=	min ( 1.0, ZoomTo );
			}

			if ( m_nRullersUp && false == m_nRullersDown )
				CenterY			=	( 0.5 * m_nImageHeight * m_nScaleInnerImage - m_oRullers.GetRullerSize () / ZoomTo * 0.5 );

			if ( false == m_nRullersUp && m_nRullersDown )
				CenterY			=	( 0.5 * m_nImageHeight * m_nScaleInnerImage + m_oRullers.GetRullerSize () / ZoomTo * 0.5 );

			if ( ZoomTo != m_nZoom || CenterX != m_nCenterX || CenterY != m_nCenterY )
			{
				m_nZoom			=	ZoomTo;

				m_nCenterX		=	CenterX;
				m_nCenterY		=	CenterY;

				m_nRefreshImage	=	ImageRendering::Constants::sc_nRefreshForRebuild;
			}
		}


		
		inline bool GetScreenImage ( BYTE* pToCopyMemory )
		{
			if ( m_pBitmap2 )
			{
				BitmapData oBitmapData; 
				if ( Gdiplus::Ok == m_pBitmap2->LockBits ( NULL, ImageLockModeRead, PixelFormat32bppARGB, &oBitmapData ) )
				{
					memcpy ( pToCopyMemory, oBitmapData.Scan0, m_nWindowWidth * m_nWindowHeight * 4 );

					m_pBitmap2->UnlockBits ( &oBitmapData );

					return true;
				}
			}

			return false;
		}

	public:

		PROPERTY_GET_CONST	( Zoom,					double );
		PROPERTY_GET_CONST	( CenterX,				double );
		PROPERTY_GET_CONST	( CenterY,				double );
		PROPERTY_GET_CONST	( EnableGrid,			bool );

		PROPERTY_GET_CONST	( PreviewLeft,			double );
		PROPERTY_GET_CONST	( PreviewRight,			double );
		PROPERTY_GET_CONST	( PreviewTop,			double );
		PROPERTY_GET_CONST	( PreviewBottom,		double );

		PROPERTY_GET_CONST	( TransformLeft,		double );
		PROPERTY_GET_CONST	( TransformRight,		double );
		PROPERTY_GET_CONST	( TransformTop,			double );
		PROPERTY_GET_CONST	( TransformBottom,		double );

		PROPERTY_GET_CONST	( ImageWidth,			int );
		PROPERTY_GET_CONST	( ImageHeight,			int );

		PROPERTY_GET_CONST	( RullersUp,			bool );
		PROPERTY_GET_CONST	( RullersDown,			bool );
		PROPERTY_GET_CONST	( RullersRight,			bool );
		PROPERTY_GET_CONST	( RullersLeft,			bool );

		PROPERTY_GET_CONST	( BackClearColor,		DWORD );
		//PROPERTY_GET_CONST	( RenderMode,			long );
		PROPERTY_GET_CONST	( RefreshImage,			long );
		PROPERTY_GET_CONST	( InterpolationMode,	long );
		PROPERTY_GET_CONST	( ScaleInnerImage,		double );
		PROPERTY_GET_CONST	( ZoomToFitOffsetPX,	double );

		inline long GetStartDX				( )
		{
			if ( m_nRullersUp )
			{
				if ( m_nStartDX < m_oRullers.GetRullerSize () )
					return m_oRullers.GetRullerSize ();

				return __min ( m_nStartDX + m_oRullers.GetRullerSize (), m_nStartDX );
			}

			return m_nStartDX;
		}

		inline long GetEndDX				( )
		{
			if ( m_nRullersDown )
			{
				if ( m_nWindowWidth - m_oRullers.GetRullerSize () < m_nEndDX )
					return m_nWindowWidth - m_oRullers.GetRullerSize (); 
			}

			return m_nEndDX;
		}

		inline long GetStartDY				( )
		{
			if ( m_nRullersLeft )
			{
				if ( m_nStartDY < m_oRullers.GetRullerSize () )
					return m_oRullers.GetRullerSize ();

				return __min ( m_nStartDY + m_oRullers.GetRullerSize (), m_nStartDY );
			}

			return m_nStartDY;
		}

		inline long GetEndDY				( )
		{
			if ( m_nRullersDown )
			{
				if ( m_nWindowHeight - m_oRullers.GetRullerSize () < m_nEndDY )
					return m_nWindowHeight - m_oRullers.GetRullerSize (); 
			}

			return m_nEndDY;
		}


		inline void SetZoom					( double Zoom, bool ForseZoomToWindow = true )
		{		
			if ( m_nZoom < 0.0 )
				return;

			m_nZoom					=	Zoom;

			if ( ForseZoomToWindow )
			{
				double ZoomToWindow		=	min ( (double)m_nWindowWidth / (double) m_nImageWidth, (double) m_nWindowHeight / (double) m_nImageHeight );

				if ( ZoomToWindow > m_nScaleInnerImage * m_nZoom )
				{
					m_nCenterX			=	0.5 * m_nImageWidth;
					m_nCenterY			=	0.5 * m_nImageHeight;
				}
			}

			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}

		inline void SetCenterX				( double Value )
		{
			m_nCenterX				=	Value;

			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;

			// ATLTRACE ( _T("SetCenterX - %f, "), m_nCenterX );
		}

		inline void SetCenterY				( double Value )
		{
			m_nCenterY				=	Value;

			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;

			// ATLTRACE ( _T("SetCenterY - %f, \n"), m_nCenterY );
		}

		inline void SetEnableRullerUp		( bool RullerValue )
		{
			m_nRullersUp			=	RullerValue;
			m_oRullers.SetRullerUp ( RullerValue );
			m_oRullers.SetRullerUp ( RullerValue );
			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}

		inline void SetEnableRullerDown		( bool RullerValue )
		{
			m_nRullersDown			=	RullerValue;

			m_oRullers.SetRullerDown ( RullerValue );
			m_oRullers.SetRullerDown ( RullerValue );
			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}

		inline void SetEnableRullerRight	( bool RullerValue )
		{
			m_nRullersRight			=	RullerValue;

			m_oRullers.SetRullerRight ( RullerValue );
			m_oRullers.SetRullerRight ( RullerValue );
			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}

		inline void SetEnableRullerLeft		( bool RullerValue )
		{
			m_nRullersLeft			=	RullerValue;

			m_oRullers.SetRullerLeft ( RullerValue );
			m_oRullers.SetRullerLeft ( RullerValue );
			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}

		inline void SetRenderMode			( int Mode, int Value)
		{
			RENDER_FLAGS [ Mode ]	=	Value;
		}
		inline void SetRefresh				( long RefreshImage )
		{
			m_nRefreshImage			=	RefreshImage;
		}

		inline void SetInterpolationMode	( long InterpolationMode )
		{
			m_nInterpolationMode	=	InterpolationMode;
			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}

		inline void SetZoomToWindow			( bool bZoomToWindow )
		{
			m_bZoomToWindow			=	bZoomToWindow;
			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}

		inline void SetEnableGrid			( bool EnableGrid )
		{
			m_nEnableGrid			=	EnableGrid;
			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}

		inline void SetBackClearColor		( DWORD Color )
		{
			m_nBackClearColor		=	Color;
			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}
		inline bool GetZoomToWindow			( )
		{
			return m_bZoomToWindow;
		}

		inline ImageRendering::CRullers&	GetRullers ()	
		{
			return	m_oRullers;
		}
		inline ImageRendering::CGridRender&	GetGridRender ()
		{
			return m_oGridRender;
		}
		inline ImageRendering::CChessBrush&	GetChessBrush ()
		{
			return m_oChessBrush;
		}

		inline void SetRullersType			( long Type )
		{
			m_oRullers.SetRullerType ( Type );
		}
		inline long GetRullersType			()
		{
			return m_oRullers.GetRullerType ();
		}

		inline void SetZoomToFitOffsetPX	( double Value )
		{
			m_nZoomToFitOffsetPX	=	Value;
			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}


	public:

		// ICoordinatSystem	//
		virtual void ICoordinatSystem::ScreenPointToImagePoint ( long& X, long& Y )
		{
			X	=	static_cast <long> ( ( X - 0.5 * m_nWindowWidth  + m_nCenterX * m_nZoom ) / m_nZoom );
			Y	=	static_cast <long> ( ( Y - 0.5 * m_nWindowHeight + m_nCenterY * m_nZoom ) / m_nZoom ); 
		}

		virtual void ICoordinatSystem::ImagePointToScreenPoint ( long& X, long& Y )
		{
			X	=	static_cast <long> ( X * m_nZoom + 0.5 * m_nWindowWidth  - m_nCenterX * m_nZoom );
			Y	=	static_cast <long> ( Y * m_nZoom + 0.5 * m_nWindowHeight - m_nCenterY * m_nZoom ); 	
		}

		virtual void ICoordinatSystem::ScreenDistanceToImageDistance ( long& Distance )
		{
			if ( m_nZoom < 0.0 )
				return;

			Distance	=	static_cast <long> ( Distance / m_nZoom );
		}

		virtual void ICoordinatSystem::ImageDistanceToScreenDistance ( long& Distance )
		{
			if ( m_nZoom < 0.0 )
				return;

			Distance	=	static_cast <long> ( Distance * m_nZoom );
		}

	private:

		inline void Update ()
		{
			double ZoomToWindow		=	min ( (double)m_nWindowWidth / (double) m_nImageWidth, (double) m_nWindowHeight / (double) m_nImageHeight );
			if ( ZoomToWindow >= m_nScaleInnerImage * m_nZoom )
			{
				m_nCenterX			=	m_nImageWidth	* 0.5;
				m_nCenterY			=	m_nImageHeight	* 0.5;
			}

			m_nRefreshImage			=	ImageRendering::Constants::sc_nRefreshForRebuild;
		}

		inline void ComputeRectangle ()
		{
			double ScaleZoom				=	m_nScaleInnerImage * m_nZoom;

			m_nPreviewLeft					=	m_nCenterX - 0.5 * m_nWindowWidth	/ ScaleZoom;
			m_nPreviewRight					=	m_nCenterX + 0.5 * m_nWindowWidth	/ ScaleZoom;
			m_nPreviewTop					=	m_nCenterY - 0.5 * m_nWindowHeight	/ ScaleZoom;
			m_nPreviewBottom				=	m_nCenterY + 0.5 * m_nWindowHeight	/ ScaleZoom;

			double ImageTransformWidth		=	m_nImageWidth	* ScaleZoom;
			double ImageTransformHeight		=	m_nImageHeight	* ScaleZoom;

			m_nTransformLeft				=	m_nWindowWidth	* 0.5	- m_nCenterX * m_nZoom;
			m_nTransformRight				=	m_nTransformLeft + ImageTransformWidth;
			m_nTransformTop					=	m_nWindowHeight	* 0.5	- m_nCenterY * m_nZoom;
			m_nTransformBottom				=	m_nTransformTop	+ ImageTransformHeight;

			// ATLTRACE ( _T("Transform Image rect : %f, %f, %f, %f \n"), m_nTransformLeft, m_nTransformTop, m_nTransformRight, m_nTransformBottom );

			int nWidth						=	m_nWindowWidth;
			int nHeight						=	m_nWindowHeight;

			double WriteWidth				=	m_nImageWidth	* ScaleZoom;
			double WriteHeight				=	m_nImageHeight	* ScaleZoom;

			double InvertZoom				=	1.0 / ScaleZoom;

			m_nStartDX						=	static_cast<long> ( m_nTransformLeft );
			if ( m_nStartDX < 0 )				
				m_nStartDX = 0;

			m_nStartDY						=	static_cast<long> ( m_nTransformTop );
			if ( m_nStartDY < 0 )				
				m_nStartDY = 0;

			m_nEndDX						=	static_cast<long> ( m_nTransformRight );
			if ( m_nEndDX > nWidth )
				m_nEndDX = nWidth;

			m_nEndDY						=	static_cast<long> ( m_nTransformBottom );
			if ( m_nEndDY > nHeight )
				m_nEndDY = nHeight;

			//long nWriteWidth				=	( static_cast<long> ( ImageTransformWidth ) / 2 ) * 2;
			//long nWriteHeight				=	( static_cast<long> ( ImageTransformHeight ) / 2 ) * 2;

			long StartDX					=	static_cast<long> ( m_nTransformLeft );
			if ( StartDX < 0 )				
				StartDX = 0;

			long StartDY					=	static_cast<long> ( m_nTransformTop );
			if ( StartDY < 0 )				
				StartDY = 0;

			//long EndDX					=	static_cast<long> ( m_nTransformRight );
			//if ( EndDX > nWidth )
			//	EndDX = nWidth;

			//long EndDY					=	static_cast<long> ( m_nTransformBottom );
			//if ( EndDY > nHeight )
			//	EndDY = nHeight;

			WriteWidth						=	m_nImageWidth	* ScaleZoom;
			WriteHeight						=	m_nImageHeight	* ScaleZoom;

			//WriteWidth						=	( (long)WriteWidth / 2 ) * 2;
			//WriteHeight						=	( (long)WriteHeight / 2 ) * 2;

			long EndDX						=	(long) ( m_nTransformLeft + WriteWidth );
			if ( EndDX > nWidth )
				EndDX = nWidth;

			long EndDY						=	(long) ( m_nTransformTop + WriteHeight );
			if ( EndDY > nHeight )
				EndDY = nHeight;

			//EndDX							=	( static_cast<long> ( EndDX ) / 2 ) * 2;
			//EndDY							=	( static_cast<long> ( EndDY ) / 2 ) * 2;

			m_nAlphaWidth					=	EndDX - StartDX;
			m_nAlphaHeight					=	EndDY - StartDY;
		}


		inline void GetPaintingBound ( double Zoom, double ScaleZoom, double& OffSetX, double& OffSetY, double& InvertZoom, long& X, long& Y, long& XW, long& YH )
		{
			double MultyZoom	=	m_nScaleInnerImage * Zoom;

			int nWidth			=	m_nWindowWidth;
			int nHeight			=	m_nWindowHeight;

			double WriteWidth	=	(double)m_nImageWidth	* MultyZoom;
			double WriteHeight	=	(double)m_nImageHeight	* MultyZoom;

			InvertZoom			=	1.0 / MultyZoom;

			OffSetX				=	nWidth	* 0.5	-	m_nCenterX * Zoom;
			OffSetY				=	nHeight * 0.5	-	m_nCenterY * Zoom;

			X					=	(long)OffSetX;
			if ( X < 0 )	
				X = 0;

			Y					=	(long)OffSetY;
			if ( Y < 0 )				
				Y = 0;

			XW					=	(long) ( OffSetX + WriteWidth );
			if ( XW > nWidth )
				XW = nWidth;

			YH					=	(long) ( OffSetY + WriteHeight );
			if ( YH > nHeight )
				YH = nHeight;
		}

		inline void GetScalePaintingBound ( double Zoom, double& OffSetX, double& OffSetY, long& X, long& Y, long& XW, long& YH )
		{
			long nViewWidth		=	m_nWindowWidth;
			long nViewHeight	=	m_nWindowHeight;

			if ( m_nRullersLeft )
				nViewWidth -= m_oRullers.GetRullerSize ();// + 1;
			if ( m_nRullersRight )
				nViewWidth -= m_oRullers.GetRullerSize ();

			if ( m_nRullersUp )
				nViewHeight -= m_oRullers.GetRullerSize ();// + 1;
			if ( m_nRullersDown )
				nViewHeight -= m_oRullers.GetRullerSize ();

			double ZoomRight = m_nZoom;
			if ( m_bZoomToWindow )
			{
				ZoomRight		=	min ( ( (double)nViewWidth - m_nZoomToFitOffsetPX ) / ( (double) m_nImageWidthReal ), ( (double) nViewHeight - m_nZoomToFitOffsetPX ) / ( (double) m_nImageHeightReal ) );
				
				// кламп по масштабу для малых изображений по сравнению с областью видимости
				if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeDisabledZoomToFitForSmallImages ] )
				{
					ZoomRight		=	min ( 1.0, ZoomRight );
				}
			}

			int nWidth			=	m_nWindowWidth;
			int nHeight			=	m_nWindowHeight;

			double WriteWidth	=	(double)m_nImageWidthReal	* ZoomRight;
			double WriteHeight	=	(double)m_nImageHeightReal	* ZoomRight;

			OffSetX				=	nWidth	* 0.5	-	m_nCenterX * ZoomRight;
			OffSetY				=	nHeight * 0.5	-	m_nCenterY * ZoomRight;

			X					=	(long)OffSetX;
			if ( X < 0 )	
				X = 0;

			Y					=	(long)OffSetY;
			if ( Y < 0 )				
				Y = 0;

			XW					=	(long) ( OffSetX + WriteWidth );
			if ( XW > nWidth )
				XW = nWidth;

			YH					=	(long) ( OffSetY + WriteHeight );
			if ( YH > nHeight )
				YH = nHeight;
		}


		inline void PrepareFrontImage ()
		{
			if ( ImageRendering::Constants::sc_nRefreshForRebuild == m_nRefreshImage )
			{
				long StartDX		=	0;
				long StartDY		=	0;
				long EndDX			=	0;
				long EndDY			=	0;

				double InvertZoom	=	1.0;
				double OffSetX		=	0.0;
				double OffSetY		=	0.0;

				GetPaintingBound ( m_nZoom, m_nScaleInnerImage, OffSetX, OffSetY, InvertZoom, StartDX, StartDY, EndDX, EndDY );

				double ScaleZoom	=	m_nScaleInnerImage * m_nZoom;

				double SrcOffsetX				=	0.0;
				double SrcOffsetY				=	0.0;

				//ATLTRACE ( _T("_NoneImage : %d %d %d %d, Zoom : %f\n"), StartDX, StartDY, EndDX - StartDX, EndDY- StartDY, m_nZoom );

				if ( m_nScaleInnerImage != 1.0 )
				{
					GetScalePaintingBound ( m_nZoom, OffSetX, OffSetY, StartDX, StartDY, EndDX, EndDY );

					//ATLTRACE ( _T("ScaleImage : %d %d %d %d \n"), StartDX, StartDY, EndDX - StartDX, EndDY- StartDY );
				}

				if ( NULL != m_pBitmap )
				{
					int ImageWidth					=	m_pBitmap->GetWidth ();
					int ImageHeight					=	m_pBitmap->GetHeight ();

					BitmapData oBitmapData; 
					if ( Gdiplus::Ok == m_pBitmap->LockBits ( NULL, ImageLockModeWrite, PixelFormat32bppARGB, &oBitmapData ) )
					{
						if ( m_nStartDX >= 0 && m_nStartDY >= 0 && m_nEndDX <= m_nWindowWidth && m_nEndDY <= m_nWindowHeight )
						{
							memset ( oBitmapData.Scan0, 0x0, m_nWindowWidth * m_nWindowHeight * 4 );	
						}

						unsigned long* pSourceRGBA	=	(DWORD*) m_pImage;
						unsigned long* pPixelsRGBA	=	(DWORD*) oBitmapData.Scan0;

						if ( ImageRendering::PixelsUtils::sc_nInterpolationModeNearestNeighbor == m_nInterpolationMode )
						{
							if ( 1.0 == ScaleZoom )
							{
								DWORD* pSource			=	(DWORD*) m_pImage;
								DWORD* pPixels			=	(DWORD*) oBitmapData.Scan0;

								long WriteH				=	EndDY - StartDY;
								long WriteW				=	EndDX - StartDX;

								long Strafe				=	EndDX - StartDX;
								long CopyBytes			=	WriteW * sizeof ( DWORD );

								pPixels					+=	StartDX + StartDY * ImageWidth;

								pSource					+=	static_cast<long> ( m_nImageWidth * static_cast<long> ( - min ( OffSetY, 0.0 ) ) );
								pSource					+=	static_cast<long> ( -min ( OffSetX, 0.0 ) );

								for ( long Line = 0; Line < WriteH; ++Line )
								{
									memcpy_s ( pPixels, CopyBytes, pSource, CopyBytes );

									pPixels				+=	ImageWidth;
									pSource				+=	m_nImageWidth;
								}
							}

							//if ( ScaleZoom > 1.0 )
							//{
							//	PixelsUtils::ImageResize::NearestResizeImageDW /*NearestResizeImageDoubleOptimize*/ /*NearestResizeCopySameBlocks*/ ( 
							//		pSourceRGBA, pPixelsRGBA, 
							//		m_nImageWidth, m_nImageHeight, m_nWindowWidth, m_nWindowHeight,
							//		SrcOffsetX, SrcOffsetY,													// Source offset
							//		-(long) ( OffSetX ), -(long) ( OffSetY ),								// Destination offset
							//		InvertZoom, InvertZoom,													// Source step
							//		StartDX, EndDX, StartDY, EndDY );										// Destination write start~end pixels
							//}

							if ( ScaleZoom < 1.0 ||  ScaleZoom > 1.0 )
							{
#if _MSC_VER >= 1500 // 1500 == VC++ 9.0
								//#ifdef _DEBUG
								ImageRendering::Debug::TicksCounter Optimize;
								ImageRendering::Debug::TicksCounter NoneOptimize;

								Optimize.Start ();
								//#endif					
#endif		
								if ( 0 == ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeEnableImageSurface ] ) )
								{
									PixelsUtils::ImageResize::NearestResizeImageDW ( 
										pSourceRGBA, pPixelsRGBA, 
										m_nImageWidth, m_nImageHeight, m_nWindowWidth, m_nWindowHeight,
										SrcOffsetX, SrcOffsetY,													// Source offset
										-(long) ( OffSetX ), -(long) ( OffSetY ),								// Destination offset
										InvertZoom, InvertZoom,													// Source step
										StartDX, EndDX, StartDY, EndDY );										// Destination write start~end pixels
								}

#if _MSC_VER >= 1500 // 1500 == VC++ 9.0
								//#ifdef _DEBUG
								Optimize.Stop ();

								//memset ( oBitmapData.Scan0, 0x0, m_nWindowWidth * m_nWindowHeight * 4 );	

								NoneOptimize.Start ();
								//#endif					
#endif		
								//inline void UpdateResize ( 
								//	DWORD* pImage, DWORD* pArea,
								//	long ImageW, long ImageH,					//	размеры исходной картинки
								//	long AreaW, long AreaH,						//	размеры области куда будет записывать данные
								//	long ImageOffsetX, long ImageOffsetY,		//	смещение в исходной картинке
								//	long AreaBeginX, long AreaEndX,				//	смещения в области которой пишем
								//	long AreaBeginY, long AreaEndY,
								//	double Scale

								if ( RENDER_FLAGS [ ImageRendering::Constants::sc_RenderModeEnableImageSurface ] )
								{
									//ONLY FOR IMAGE EDITOR
									m_oImageSurface.UpdateResize ( 
										pSourceRGBA, pPixelsRGBA, 
										m_nImageWidth, m_nImageHeight,
										m_nWindowWidth, m_nWindowHeight,
										-(long) ( OffSetX ), -(long) ( OffSetY ),								// Destination offset
										SrcOffsetX, SrcOffsetY,													// Source offset
										StartDX, EndDX, StartDY, EndDY,											// Source step
										InvertZoom );															// Destination write start~end pixels
								}

								//PixelsUtils::ImageResize::NearestResizeImageDW ( 
								//	pSourceRGBA, pPixelsRGBA, 
								//	m_nImageWidth, m_nImageHeight, m_nWindowWidth, m_nWindowHeight,
								//	SrcOffsetX, SrcOffsetY,													// Source offset
								//	-(long) ( OffSetX ), -(long) ( OffSetY ),								// Destination offset
								//	InvertZoom, InvertZoom,													// Source step
								//	StartDX, EndDX, StartDY, EndDY );										// Destination write start~end pixels
#if _MSC_VER >= 1500 // 1500 == VC++ 9.0
								//#ifdef _DEBUG
								NoneOptimize.Stop ();
								//	ATLTRACE(_T("Fast than more : %f %s\n"), (double)Optimize.GetCountTicks () / (double)NoneOptimize.GetCountTicks () * 100.0, _T("%") );

								//m_nDebugFastResize	=	 (double)NoneOptimize.GetCountTicks () / (double)Optimize.GetCountTicks ();
								//
								//wchar_t message [256];
								//swprintf ( message, 256, L"Fast than more : %f %s\n", m_nDebugFastResize, _T("") );
								//OutputDebugStringW ( message );
							
								//#endif		
#endif		
							}
						}

						if ( ImageRendering::PixelsUtils::sc_nInterpolationModeBilinear == m_nInterpolationMode )
						{
							PixelsUtils::ImageResize::BilinearResizeImage (
								pSourceRGBA, pPixelsRGBA, 
								m_nImageWidth, m_nImageHeight, m_nWindowWidth, m_nWindowHeight,
								SrcOffsetX, SrcOffsetY,													// Source offset
								-(long) ( OffSetX ), -(long) ( OffSetY ),								// Destination offset
								InvertZoom, InvertZoom,													// Source step
								StartDX, EndDX, StartDY, EndDY );										// Destination write start~end pixels
						}

						m_pBitmap->UnlockBits ( &oBitmapData );
					}

					/*

					CLSID ImageGUID;
					ImageRendering::ImageUtils::GetEncoderCLSID ( L"image/png", &ImageGUID );
					m_pBitmap->Save ( L"C:\\ImageTetsBefore.png", &ImageGUID );

					*/
				}
			}
		}



		inline void RenderGrid ()
		{
			if ( m_nEnableGrid )
			{
				double WriteWidth			=	m_nImageWidth	* m_nZoom;
				double WriteHeight			=	m_nImageHeight	* m_nZoom;

				double OffsetX				=	m_nWindowWidth	* 0.5 - m_nCenterX * m_nZoom;
				double OffsetY				=	m_nWindowHeight * 0.5 - m_nCenterY * m_nZoom;

				m_oGridRender.OnPaint (	m_pGraphics2, 
					m_nWindowWidth, m_nWindowHeight, 
					m_nImageWidth, m_nImageHeight,
					m_nZoom, 
					OffsetX, OffsetY, 
					m_nStartDX, m_nStartDY,
					m_nEndDX, m_nEndDY );
			}
		}

		inline void RenderRullers ()
		{
			double WriteWidth				=	m_nImageWidth	* m_nZoom;
			double WriteHeight				=	m_nImageHeight	* m_nZoom;

			double OffsetX					=	m_nWindowWidth	* 0.5 - m_nCenterX * m_nZoom;
			double OffsetY					=	m_nWindowHeight * 0.5 - m_nCenterY * m_nZoom;

			m_oRullers.OnPaint ( m_hDC, m_nWindowWidth, m_nWindowHeight, m_nImageWidth, m_nImageHeight, m_nZoom, OffsetX, OffsetY, m_pGraphics2 );
		}



		inline void RenderBackBitmapNonCopyMode ()
		{
			if ( NULL == m_pBitmap2 )
				return;

			LONG nWidth								=	m_pBitmap2->GetWidth();
			LONG nHeight							=	m_pBitmap2->GetHeight();

			BITMAPINFO oBitmapInfo;

			oBitmapInfo.bmiHeader.biSize			=	sizeof ( BITMAPINFOHEADER );
			oBitmapInfo.bmiHeader.biHeight			=	- nHeight;							// нужно что бы FLIP не происходил
			oBitmapInfo.bmiHeader.biWidth			=	nWidth;
			oBitmapInfo.bmiHeader.biPlanes			=	1;
			oBitmapInfo.bmiHeader.biBitCount		=	32;
			oBitmapInfo.bmiHeader.biCompression		=	BI_RGB;
			oBitmapInfo.bmiHeader.biSizeImage		=	0;
			oBitmapInfo.bmiHeader.biXPelsPerMeter	=	1;
			oBitmapInfo.bmiHeader.biYPelsPerMeter	=	1;
			oBitmapInfo.bmiHeader.biClrUsed			=	0;
			oBitmapInfo.bmiHeader.biClrImportant	=	0;

			BitmapData oBitmapData; 

			if ( Gdiplus::Ok == m_pBitmap2->LockBits ( NULL, ImageLockModeRead, PixelFormat32bppARGB, &oBitmapData ) )
			{
				SetDIBitsToDevice ( m_hDC, 0, 0, nWidth, nHeight, 0, 0, 0, nHeight, oBitmapData.Scan0, &oBitmapInfo, DIB_RGB_COLORS );

				m_pBitmap2->UnlockBits ( &oBitmapData );
			}
		}


	private:

		CGdiPlusInit					m_oCGraphics;

		BYTE							RENDER_FLAGS [ 512 ];				// режимы отображения 

		bool							m_bUpdateWindowSizes;

		long							m_nRefreshImage;					// тип обновления
		//unsigned long					m_nRenderMode;						
		long							m_nInterpolationMode;
		bool							m_bZoomToWindow;

		HWND							m_hWnd;
		HDC								m_hDC;
		RECT							m_RECT;

		// размеры картинки
		BYTE*							m_pImage;		
		int								m_nImageWidth;
		int								m_nImageHeight;
		int								m_nImageWidthReal;
		int								m_nImageHeightReal;

		// размеры области вывода
		int								m_nWindowWidth;
		int								m_nWindowHeight;

		int								m_nStartDX;
		int								m_nEndDX;
		int								m_nStartDY;
		int								m_nEndDY; 

		int								m_nAlphaWidth;
		int								m_nAlphaHeight;

		// координаты превью для картинки
		double							m_nPreviewLeft;
		double							m_nPreviewRight;
		double							m_nPreviewTop;
		double							m_nPreviewBottom;

		// координаты трансформированой с учетом цента и зума картинки
		double							m_nTransformLeft;
		double							m_nTransformRight;
		double							m_nTransformTop;
		double							m_nTransformBottom;

		// смещение относительно центра картинки
		double							m_nCenterX;
		double							m_nCenterY;
		double							m_nZoom;

		BYTE							m_nChannels;	//

		DWORD							m_nBackClearColor;			// цвет для фона

		// настройки разметки
		bool							m_nEnableGrid;
		int								m_nGridMode;				// 0 - pixels, 1 - сантиметры, 2 - дюймы

		bool							m_nRullersUp;
		bool							m_nRullersDown;
		bool							m_nRullersRight;
		bool							m_nRullersLeft;

		ImageRendering::CRullers		m_oRullers;
		ImageRendering::CGridRender		m_oGridRender;
		ImageRendering::CChessBrush		m_oChessBrush;

		Gdiplus::Graphics*				m_pGraphics;
		Gdiplus::Graphics*				m_pGraphics2;
		Gdiplus::Bitmap*				m_pBitmap;
		Gdiplus::Bitmap*				m_pBitmap2;
		Gdiplus::Bitmap*				m_pBitmapAlpha;				//	подложка
		Gdiplus::Bitmap*				m_pBitmapCopy;
		Gdiplus::Bitmap*				m_pRefImage;

		double							m_nScaleInnerImage;
		double							m_nZoomToFitOffsetPX;

		ImageRendering::PixelsUtils::ImageSurface	m_oImageSurface;

		BYTE*							m_pbScreenBuffer;			// буфер на котором рисуем картинку с параметрами

		double							m_nDebugFastResize;
	};

	class EnumerateViewProperties
	{
#pragma region MACROSES

#define EPROPERTY_SET(NAME,TYPE) if ( NViewSettingsConstants::g_csViewSettings_##NAME == Name )\
		{\
		pViewManager->Set##NAME(TYPE);\
		return true;\
	}\

#define EPROPERTY_GET(NAME,PVAL,CAST_TYPE,VT_TYPE) if ( NViewSettingsConstants::g_csViewSettings_##NAME == Name )\
		{\
		pValue->vt		=	##VT_TYPE;\
		pValue->##PVAL	=	static_cast<##CAST_TYPE> ( pViewManager->Get##NAME () );\
		return true;\
	}\

#pragma endregion

	public:

		static bool SetProperty ( ImageRendering::CViewManager* pViewManager, BSTR Name, VARIANT Value )
		{
			if ( NULL == pViewManager )
				return false;

			CComBSTR PropertyName	=	CComBSTR ( Name );

			EPROPERTY_SET ( Zoom,				( Value.dblVal ) );
			EPROPERTY_SET ( CenterX,			( Value.dblVal ) );
			EPROPERTY_SET ( CenterY,			( Value.dblVal ) );
			EPROPERTY_SET ( EnableRullerUp,		( Value.boolVal == VARIANT_TRUE ) ? true : false );
			EPROPERTY_SET ( EnableRullerDown,	( Value.boolVal == VARIANT_TRUE ) ? true : false );
			EPROPERTY_SET ( EnableRullerLeft,	( Value.boolVal == VARIANT_TRUE ) ? true : false );
			EPROPERTY_SET ( EnableRullerRight,	( Value.boolVal == VARIANT_TRUE ) ? true : false );
			EPROPERTY_SET ( BackClearColor,		( Value.ulVal ) );
			EPROPERTY_SET ( EnableGrid,			( Value.boolVal == VARIANT_TRUE ) ? true : false );
			EPROPERTY_SET ( RullersType,		( Value.lVal ) );
			EPROPERTY_SET ( ZoomToFitOffsetPX,	( Value.dblVal ) );
			//EPROPERTY_SET ( RenderMode,			( Value.ulVal ) );

			return true;
		}

		static bool GetProperty ( ImageRendering::CViewManager* pViewManager, BSTR Name, VARIANT* pValue )
		{
			if ( NULL == pViewManager )
				return false;

			CComBSTR PropertyName	=	CComBSTR ( Name );

			EPROPERTY_GET ( Zoom,				dblVal, DOUBLE, VT_R8 );
			EPROPERTY_GET ( CenterX,			dblVal, DOUBLE, VT_R8 );
			EPROPERTY_GET ( CenterY,			dblVal, DOUBLE, VT_R8 );
			EPROPERTY_GET ( ImageWidth,			lVal,	LONG,	VT_I4 );
			EPROPERTY_GET ( ImageHeight,		lVal,	LONG,	VT_I4 );
			EPROPERTY_GET ( PreviewLeft,		lVal,	LONG,	VT_I4 );
			EPROPERTY_GET ( PreviewRight,		lVal,	LONG,	VT_I4 );
			EPROPERTY_GET ( PreviewTop,			lVal,	LONG,	VT_I4 );
			EPROPERTY_GET ( PreviewBottom,		lVal,	LONG,	VT_I4 );
			EPROPERTY_GET ( BackClearColor,		lVal,	LONG,	VT_UI4 );
			EPROPERTY_GET ( RullersType,		lVal,	LONG,	VT_UI4 );
			EPROPERTY_GET ( ZoomToFitOffsetPX,	dblVal, DOUBLE, VT_R8 );
			//EPROPERTY_GET ( RenderMode,			ulVal,	ULONG,	VT_UI4 );

			return true;
		}
	};
}	
