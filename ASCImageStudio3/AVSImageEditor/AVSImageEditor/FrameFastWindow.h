#pragma once

#include <TemporaryCS.h>
#include <Gdiplus.h>
#include <TimeMeasurer.h>

#define	INCLUDE_FAST_WINDOW

#include "DXINCLUDE.h"

#ifdef INCLUDE_FAST_WINDOW

namespace ImageRendering
{
	class FastGridRender
	{
	public:

	};

	class FastRullersRender
	{
	public:

	};

	class CTextureBatch
	{
	public:

		CTextureBatch ()
		{
			m_nCountX	=	0;
			m_nCountY	=	0;
		}

		virtual ~CTextureBatch ()
		{
			Clear ();
		}


		inline void Create ( long CountX, long CountY, IDirect3DDevice9* pD3DDevice, long Size = 512 )
		{
			// Clear ();

			m_nCountX	=	CountX;
			m_nCountY	=	CountY;
			m_nSize		=	Size;
			
			long nMaxCount = m_nCountX * m_nCountY;

			for ( long i = (long)m_Textures.GetCount (); i < nMaxCount; ++i )
			{
				IDirect3DTexture9* pTexture = NULL;
				if ( FAILED ( pD3DDevice->CreateTexture ( Size, Size, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, 0 ) ) )
				{
					Clear ();
				}

				m_Textures.Add ( pTexture );
			}

			return;

			///*

			for ( long i = 0; i < ( m_nCountX + 0 ) * ( m_nCountY + 0 ); ++i )
			{
				IDirect3DTexture9* pTexture = NULL;
				if ( FAILED ( pD3DDevice->CreateTexture ( Size, Size, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture, 0 ) ) )
				{
					Clear ();
				}

				//D3DSURFACE_DESC	Surface;
				//if ( SUCCEEDED ( pTexture->GetLevelDesc ( 0, &Surface ) ) )
				//{
				//	D3DLOCKED_RECT LockRect;
				//	if ( SUCCEEDED ( pTexture->LockRect ( 0, &LockRect, 0, 0 ) ) )
				//	{
				//		memset ( LockRect.pBits, 0x7F, Surface.Width * Surface.Height * 4 );
				//		pTexture->UnlockRect ( 0 );
				//	}
				//}			

				m_Textures.Add ( pTexture );
			}

			//*/
		}

		inline void Clear ()
		{
			for ( long i = 0; i < m_nCountX * m_nCountY; ++i )
			{
				m_Textures [i]->Release ();
			}

			m_Textures.RemoveAll ();

			m_nCountX	=	0;
			m_nCountY	=	0;
		}

		inline void SetImage ( long IndX, long IndY, const BYTE* pImage, long ImageW, long ImagH, long OffSetX, long OffSetY, long OffSetW = 0, long OffSetH = 0 )
		{
			IDirect3DTexture9* pTexture = m_Textures.GetAt ( IndY * ( m_nCountX )+ IndX );

			D3DSURFACE_DESC	Surface;
			if ( SUCCEEDED ( pTexture->GetLevelDesc ( 0, &Surface ) ) )
			{
				D3DLOCKED_RECT LockRect;
				if ( SUCCEEDED ( pTexture->LockRect ( 0, &LockRect, 0, 0 ) ) )
				{
					DWORD* pPixel	=	(DWORD*)LockRect.pBits;
					DWORD* pSource	=	(DWORD*)pImage;

					memset ( pPixel, 0, Surface.Width * Surface.Height * 4 );

					DWORD CopyBytes	=	__min ( OffSetW * 4, (long)(Surface.Width * 4) );
					DWORD CopyLines	=	__min ( OffSetH, (long)(Surface.Height) );

					for ( DWORD i = 0; i < CopyLines; ++i )
					{
						memcpy ( &pPixel [ i * Surface.Width ], &pSource [ ( i + OffSetY ) * ImageW + OffSetX ], CopyBytes );
					}

					pTexture->UnlockRect ( 0 );
				}
			}
		}

		IDirect3DTexture9* GetTexture ( long IndX, long IndY )
		{
			return m_Textures.GetAt ( IndY * ( m_nCountX )+ IndX );
		}

	private:	

		CAtlArray <IDirect3DTexture9*>	m_Textures;

		long							m_nCountX;
		long							m_nCountY;

		long							m_nSize;
	};

}

namespace ImageRendering
{
	class FrameFastWindow
	{
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

	public:

		FrameFastWindow ()
		{
			m_nIsInitialized		=	-1;
			m_nIsTexturesComplete	=	false;

			m_nBackClearColor		=	0xFFD0D0D0;
			m_bEnableAlphaLayer		=	true;

			m_pD3D					=	NULL;
			m_pD3DDevice			=	NULL;

			m_nZoom					=	1.0f;
			m_nZoomSmooth			=	1.0f;

			m_nCenterX				=	1.0f;
			m_nCenterY				=	1.0f;

			m_nWindowWidth			=	-1;
			m_nWindowHeight			=	-1;

			m_pTexureChess			=	NULL;

			m_hWnd					=	NULL;
			m_pSurface				=	NULL;

			m_pImage				=	NULL;
			m_nWidth				=	0;
			m_nHeight				=	0;

			m_bUpdateTexture		=	false;
			m_nScale				=	1.0;

			m_nAsyncUpdateRender	=	0;

			m_nBatchSize			=	256;

			m_bZoomToWindow			=	false;

			m_hThread				=	NULL;

			InitializeCriticalSection ( &m_BlockingCS );
		}

		virtual ~FrameFastWindow ()
		{
			Clear ();

			DeleteCriticalSection ( &m_BlockingCS );
		}

		inline bool Create ( HWND hWnd )
		{
			if ( 1 == m_nIsInitialized && m_hWnd == hWnd )
				return true;

			InterlockedExchange ( &m_nSetImageBlock, 1L );

			Clear ();

			m_hWnd	=	hWnd;
			m_HDC	=	GetDC ( m_hWnd );

			if ( NULL == ( m_pD3D = Direct3DCreate9 ( D3D_SDK_VERSION ) ) )
			{
				m_nIsInitialized	=	-1;

				Clear ();

				return false;
			}

			D3DPRESENT_PARAMETERS d3dpp; 
			ZeroMemory ( &d3dpp, sizeof ( d3dpp ) );

			d3dpp.hDeviceWindow				=	m_hWnd;
			d3dpp.FullScreen_RefreshRateInHz=	0;
			d3dpp.BackBufferCount			=	1;
			d3dpp.Windowed					=	TRUE;
			d3dpp.SwapEffect				=	D3DSWAPEFFECT_COPY; //	D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat			=	D3DFMT_X8R8G8B8;	//	D3DFMT_UNKNOWN;
			d3dpp.EnableAutoDepthStencil	=	TRUE;
			d3dpp.AutoDepthStencilFormat	=	D3DFMT_D16;
			d3dpp.Windowed					=	TRUE;
			//d3dpp.PresentationInterval	=	D3DPRESENT_INTERVAL_IMMEDIATE;
			d3dpp.Flags						=	D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
			d3dpp.MultiSampleType			=	D3DMULTISAMPLE_NONE;
			d3dpp.PresentationInterval		=	0;

			D3DCAPS9 d3dCaps;
			if ( FAILED ( m_pD3D->GetDeviceCaps ( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps ) ) )
			{
				m_nIsInitialized	=	-1;

				Clear ();

				return false;
			}

			DWORD dwBehaviorFlags = 0;

			if ( d3dCaps.VertexProcessingCaps != 0 )
			{
				dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
			}
			else
			{
				dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

				m_nIsInitialized	=	-1;

				Clear ();

				return false;
			}

			// Create the D3DDevice
			if ( FAILED ( m_pD3D->CreateDevice ( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, dwBehaviorFlags | D3DCREATE_MULTITHREADED, &d3dpp, &m_pD3DDevice ) ) )
			{
				m_nIsInitialized	=	-1;

				Clear ();

				return false;
			}

			// отрубаем тесты
			m_pD3DDevice->SetRenderState ( D3DRS_LIGHTING,	FALSE );
			m_pD3DDevice->SetRenderState ( D3DRS_CULLMODE,	D3DCULL_NONE );
			m_pD3DDevice->SetRenderState ( D3DRS_ZENABLE,	D3DZB_FALSE );
			// отключаем фильтрацию
			m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
			m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
			m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT );

			SetFiltering ( 3 );

			m_nIsInitialized	=	1;

			if ( FAILED ( m_pD3DDevice->CreateTexture ( 256, 256, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexureChess, 0 ) ) )
			{
				m_nIsInitialized	=	-1;

				Clear ();

				return false;
			}

			D3DSURFACE_DESC d3dsd;
			m_pTexureChess->GetLevelDesc ( 0, &d3dsd );

			D3DLOCKED_RECT  Rect;
			if ( SUCCEEDED ( m_pTexureChess->LockRect ( 0, &Rect, 0, 0 ) ) )
			{
				DWORD* pPixel = (DWORD*)Rect.pBits;

				CChessBrush ChessBrush;

				Gdiplus::Bitmap oBitmap (  d3dsd.Width, d3dsd.Height, 4 * d3dsd.Width, PixelFormat32bppARGB, (BYTE*)Rect.pBits ); 
				Gdiplus::Graphics oGraphic ( &oBitmap );
				ChessBrush.OnPaint ( &oGraphic, Gdiplus::Rect ( 0, 0, d3dsd.Width, d3dsd.Height ) );

				m_pTexureChess->UnlockRect ( 0 );
			}


			if ( m_pImage && m_nWidth > 1 && m_nHeight > 1 )
			{
				SetImage ( m_pImage, m_nWidth, m_nHeight, m_nScale );
			}

			InterlockedExchange ( &m_nAsyncUpdateRender, 0L );
			InterlockedExchange ( &m_nSetImageBlock, 0L );
			InterlockedExchange ( &m_nSetEnableRender, 0L );

			m_nZoomSmooth			=	m_nZoom;

			unsigned int nThreadID	=	0;
			m_hThread				=	(HANDLE) _beginthreadex ( 0, 0, &FrameFastWindow::AsyncRender, (void*)this, 0, &nThreadID );
			SetThreadPriority ( m_hThread, THREAD_PRIORITY_LOWEST );

			m_oTimer.Reset ();

			return true;
		}

		inline void Clear ()
		{
			if ( NULL == m_hThread || -1 == m_nIsInitialized )
				return;

			m_nAsyncUpdateRender	=	1;

			WaitForSingleObject ( m_hThread, INFINITE );

			m_oTextureBatch.Clear ();

			if ( NULL != m_pTexureChess )
			{
				m_pTexureChess->Release ();
				m_pTexureChess = NULL;
			}

			if ( NULL != m_pD3D )
			{
				m_pD3D->Release ();
				m_pD3D = NULL;
			}

			if ( NULL != m_pD3DDevice )
			{
				m_pD3DDevice->Release ();
				m_pD3DDevice = NULL;
			}

			ReleaseDC ( m_hWnd, m_HDC );
		}

		inline void ClearImage	( )
		{
			CTemporaryCS TemporaryCS ( &m_BlockingCS );

			m_oTextureBatch.Clear ();
		}

		inline bool Update		( )
		{
			CTemporaryCS TemporaryCS ( &m_BlockingCS );

			typedef D3DXVECTOR2 float2;

			if ( m_pD3DDevice )
			{
				RECT m_RECT;
				GetWindowRect ( m_hWnd, &m_RECT );

				int nWindowWidth			=	abs ( m_RECT.right	- m_RECT.left );
				int nWindowHeight			=	abs ( m_RECT.bottom	- m_RECT.top  );

				m_pD3DDevice->Clear ( 0, NULL, D3DCLEAR_TARGET /*| D3DCLEAR_ZBUFFER*/, m_nBackClearColor, 1.0f, 0 );
				m_pD3DDevice->BeginScene();

				if ( false == m_bUpdateTexture )
				{
					if ( m_pImage && m_nWidth > 1 && m_nHeight > 1 )
					{
						if ( TRUE )	//	update zoom
						{
							if ( abs ( m_nZoom - m_nZoomSmooth ) < 0.0025f )
								m_nZoomSmooth	=	m_nZoom;

							if ( m_nZoom > m_nZoomSmooth )
							{
								m_nZoomSmooth += m_oTimer.GetTimeInterval() * 5.0f * abs ( m_nZoom - m_nZoomSmooth );
								if ( m_nZoom < m_nZoomSmooth )
								{
									m_nZoomSmooth	=	m_nZoom;
								}
							}
							if ( m_nZoom < m_nZoomSmooth )
							{
								m_nZoomSmooth -= m_oTimer.GetTimeInterval() * 5.0f * abs ( m_nZoom - m_nZoomSmooth );
								if ( m_nZoom > m_nZoomSmooth )
								{
									m_nZoomSmooth	=	m_nZoom;
								}
							}
						}

						/*

						m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
						m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );

						DrawColorQuad ( D3DXVECTOR2 ( 0.0f, 0.0f ), D3DXVECTOR2 ( (float)m_nWindowWidth, (float)m_nWindowHeight ), 
						0xFFEFEFEF, 0xFFCFCFCF, 0xFFAFAFAF, 0xFF9F9F9F );

						*/

						if ( m_bEnableAlphaLayer )
						{
							/*
							// шахматный стиль подложки на всю видимую область
							float ChessLeft		=	0.0f; 
							float ChessTop		=	0.0f; 

							float ChessRight	=	nWindowWidth * 2; 
							float ChessBottom	=	nWindowHeight * 2;
							*/

							// шахматный стиль подложки на область изображения
							float ChessLeft		=	__max ( (float) nWindowWidth  * 0.5f - m_nCenterX * m_nZoomSmooth, 0.0f );
							float ChessTop		=	__max ( (float) nWindowHeight * 0.5f - m_nCenterY * m_nZoomSmooth, 0.0f );

							float ChessRight	=	__min ( (float) nWindowWidth  * 0.5f - m_nCenterX * m_nZoomSmooth + (float)m_nChessWidth * m_nZoomSmooth * m_nScale,	(float) nWindowWidth );
							float ChessBottom	=	__min ( (float) nWindowHeight * 0.5f - m_nCenterY * m_nZoomSmooth + (float)m_nChessHeight * m_nZoomSmooth * m_nScale,	(float) nWindowHeight );

							m_pD3DDevice->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
							m_pD3DDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );

							m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
							m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
							m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );

							SetFiltering ( 1 );

							m_pD3DDevice->SetTexture ( 0, m_pTexureChess );

							DrawTextureQuad (	D3DXVECTOR2 ( ChessLeft, ChessTop ),	D3DXVECTOR2 ( ChessRight, ChessBottom ),
								D3DXVECTOR2 ( 0.0f, 0.0f ),	D3DXVECTOR2 ( ( ChessRight - ChessLeft ) / 256.0f, ( ChessBottom - ChessTop ) / 256.0f ) );
						}

						m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
						m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

						//Gdiplus::RectF ZonePaint = RectF ( ChessLeft, ChessTop, ChessRight - ChessLeft, ChessBottom - ChessTop );

						double OffSetX = (float)m_nBatchSize * m_nZoomSmooth * m_nScale;
						double OffSetY = (float)m_nBatchSize * m_nZoomSmooth * m_nScale;

						//D3DXMATRIX ScaleMatrix;
						//D3DXMatrixIdentity ( &ScaleMatrix );
						//D3DXMatrixScaling ( &ScaleMatrix, m_nZoomSmooth, m_nZoomSmooth, m_nZoomSmooth );

						//m_pD3DDevice->SetTransform ( D3DTS_VIEW, &ScaleMatrix );

						m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR ); 
						m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR );
						m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_MIRROR );

						int FilteringImage = 2;

						SetFiltering ( FilteringImage );

						if ( 2 == FilteringImage || 1 == FilteringImage )
						{
							for ( int j = 0; j < m_nBatchesH; ++j )
							{ 
								for ( int i = 0; i < m_nBatchesW; ++i )
								{
									float2 v1 = float2 ( (float)(nWindowWidth	* 0.5 - m_nCenterX * m_nZoomSmooth + OffSetX * ( i )), 
										(float)(nWindowHeight	* 0.5 - m_nCenterY * m_nZoomSmooth + OffSetY * ( j )) );
									float2 v2 = float2 ( (float)(nWindowWidth	* 0.5 - m_nCenterX * m_nZoomSmooth + OffSetX * ( i + 1 )),	
										(float)(nWindowHeight	* 0.5 - m_nCenterY * m_nZoomSmooth + OffSetY * ( j + 1 ) ));

									D3DXVECTOR2 Tx01 = D3DXVECTOR2
										( 
										0.0f,
										0.0f
										);

									D3DXVECTOR2 Tx02 = D3DXVECTOR2
										( 
										1.0f,
										1.0f
										);

									//v1.x = (int)v1.x;
									//v1.y = (int)v1.y;
									//v2.x = (int)v2.x;
									//v2.y = (int)v2.y;

									//v1.x -= 0.5f;
									//v1.y -= 0.5f;
									//v2.x += 0.5f;
									//v2.y += 0.5f;

									//Gdiplus::RectF ZoneCur = RectF ( v1.x, v1.y, v2.x - v1.x, v1.y - v2.y );
									//if ( ZonePaint.IntersectsWith ( ZoneCur ) )
									{
										m_pD3DDevice->SetTexture ( 0, m_oTextureBatch.GetTexture ( i, j ) );
										DrawTextureQuad ( v1, v2, Tx01, Tx02 );
									}
								}
							}
						}

						m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
					}
				}

				m_pD3DDevice->EndScene();

				RECT SourceRect;

				LONG OffSet = 0;// 16;

				SourceRect.left		=	0;//OffSet;
				SourceRect.top		=	0;//OffSet;
				SourceRect.right	=	m_nWindowWidth;//	- OffSet-1;		//	1600;
				SourceRect.bottom	=	m_nWindowHeight;//	- OffSet-1;	//	1200;

				RECT DestRect;

				DestRect.left		=	0;//OffSet;
				DestRect.top		=	0;//OffSet;
				DestRect.right		=	m_nWindowWidth;//	- OffSet-1;		//	1600;
				DestRect.bottom		=	m_nWindowHeight;//	- OffSet-1;	//	1200;

				//

				m_pD3DDevice->Present ( &SourceRect, &DestRect, NULL, NULL );
			}

			m_oTimer.Reset ();

			return true;
		}


		inline bool SetImage	( const BYTE* pImage, int Width, int Height, float Scale )
		{
			if ( NULL == pImage || Width < 1 || Height < 1 )
				return false;

			InterlockedExchange ( &m_nSetImageBlock, 1L );

			CTemporaryCS TemporaryCS ( &m_BlockingCS );

			if ( 1 != m_nIsInitialized )
			{
				return false;
			}

			m_nZoomSmooth	=	m_nZoom;

			m_bUpdateTexture =	true;

			m_nChessWidth	=	(float)Width;
			m_nChessHeight	=	(float)Height;

			m_pImage		=	pImage;
			m_nWidth		=	Width;
			m_nHeight		=	Height;
			m_nScale		=	Scale;

			m_nBatchesW		=	m_nWidth / m_nBatchSize;
			m_nBatchesH		=	m_nHeight / m_nBatchSize;

			if ( m_nWidth < m_nBatchSize )
			{
				++m_nBatchesW;
			}
			else
			{
				if ( 0.0 != floor ( (double)m_nWidth / (double)m_nBatchSize ) )
					++m_nBatchesW;
			}

			if ( m_nHeight < m_nBatchSize )
			{
				++m_nBatchesH;
			}
			else
			{
				if ( 0.0 != floor ( (double)m_nHeight / (double)m_nBatchSize ) )
					++m_nBatchesH;
			}

			m_oTextureBatch.Create ( m_nBatchesW, m_nBatchesH, m_pD3DDevice, m_nBatchSize );

			//	inline void SetImage ( long IndX, long IndY, BYTE* pImage, long ImageW, long ImagH, long OffSetX, long OffSetY, long OffSetW = 0, long OffSetH = 0 )

			for ( int j = 0; j < m_nBatchesH; ++j )
			{ 
				for ( int i = 0; i < m_nBatchesW; ++i )
				{
					long OffSetW	=	m_nBatchSize;
					if ( ( i + 1 ) * m_nBatchSize > m_nWidth )
					{
						OffSetW		=	m_nBatchSize - ( ( i + 1 ) * m_nBatchSize - m_nWidth );
					}

					long OffSetH	=	m_nBatchSize;
					if ( ( j + 1 ) * m_nBatchSize > m_nHeight )
					{
						OffSetH		=	m_nBatchSize - ( ( j + 1 ) * m_nBatchSize - m_nHeight );
					}

					m_oTextureBatch.SetImage ( i, j, pImage, m_nWidth, m_nHeight, m_nBatchSize * i, m_nBatchSize * j, OffSetW, OffSetH );
				}
			}

			m_bUpdateTexture	=	false;

			InterlockedExchange ( &m_nSetImageBlock, 0L );

			return true;
		}

		inline void SetZoom ( float Zoom, bool ZoomForce = false )
		{
			m_nZoom		=	Zoom;

			if ( ZoomForce )
			{
				m_nZoomSmooth = m_nZoom;
			}
		}

		inline void SetCenterX ( float X )
		{
			m_nCenterX	=	X;
		}

		inline void SetCenterY ( float Y )
		{
			m_nCenterY	=	Y;
		}

		inline void SetBackClearColor ( DWORD Color )
		{
			m_nBackClearColor	=	Color;
		}
		inline void SetEnableAlphaLayer ( bool Enable )
		{
			m_bEnableAlphaLayer	=	Enable;
		}

		inline void SetFiltering ( int Filtering )
		{
			//D3DTEXF_NONE            = 0,    // filtering disabled (valid for mip filter only)
			//D3DTEXF_POINT           = 1,    // nearest
			//D3DTEXF_LINEAR          = 2,    // linear interpolation
			//D3DTEXF_ANISOTROPIC     = 3,    // anisotropic
			//D3DTEXF_PYRAMIDALQUAD   = 6,    // 4-sample tent
			//D3DTEXF_GAUSSIANQUAD    = 7,    // 4-sample gaussian
			//D3DTEXF_FORCE_DWORD     = 0x7fffffff,   // force 32-bit size enum

			if ( NULL != m_pD3DDevice )
			{
				m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, (D3DTEXTUREFILTERTYPE)Filtering );
				m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, (D3DTEXTUREFILTERTYPE)Filtering );
				m_pD3DDevice->SetSamplerState ( 0, D3DSAMP_MIPFILTER, (D3DTEXTUREFILTERTYPE)Filtering );
			}
		}

		inline void SetEnableRender ( bool Enable )
		{
			CTemporaryCS TemporaryCS ( &m_BlockingCS );

			if ( Enable )
			{
				InterlockedExchange ( &m_nSetEnableRender, 1L );
			}
			else
			{
				InterlockedExchange ( &m_nSetEnableRender, 0L );
				//Clear ();
				//m_oTextureBatch.Clear ();
			}
		}

		inline long IsInitialized ()
		{
			return m_nIsInitialized;
		}
		inline bool IsEnable ()
		{
			if ( 1 == m_nIsInitialized )
			{
				return true;
			}

			return false;
		}

		inline bool Reset ()
		{
			CTemporaryCS TemporaryCS ( &m_BlockingCS );

			if ( 1 == m_nIsInitialized )
			{
				RECT m_RECT;
				GetWindowRect ( m_hWnd, &m_RECT );

				int nWindowWidth			=	abs ( m_RECT.right	- m_RECT.left );
				int nWindowHeight			=	abs ( m_RECT.bottom	- m_RECT.top  );

				if ( nWindowWidth > m_nWindowWidth || nWindowHeight > m_nWindowHeight )
				{
					m_nWindowWidth			=	nWindowWidth;
					m_nWindowHeight			=	nWindowHeight;

					D3DPRESENT_PARAMETERS d3dpp; 
					ZeroMemory ( &d3dpp, sizeof ( d3dpp ) );
					//d3dpp.Windowed				=	TRUE;
					//d3dpp.SwapEffect				=	D3DSWAPEFFECT_COPY; //D3DSWAPEFFECT_DISCARD;
					//d3dpp.BackBufferFormat		=	D3DFMT_UNKNOWN;
					//d3dpp.EnableAutoDepthStencil	=	TRUE;
					//d3dpp.AutoDepthStencilFormat	=	D3DFMT_D16;
					//d3dpp.hDeviceWindow			=	m_hWnd;

					d3dpp.hDeviceWindow				=	m_hWnd;
					d3dpp.FullScreen_RefreshRateInHz=	0;
					d3dpp.BackBufferCount			=	1;
					d3dpp.Windowed					=	TRUE;
					d3dpp.SwapEffect				=	D3DSWAPEFFECT_DISCARD;// D3DSWAPEFFECT_COPY; //	D3DSWAPEFFECT_DISCARD;
					d3dpp.BackBufferFormat			=	D3DFMT_X8R8G8B8;	//	D3DFMT_UNKNOWN;
					d3dpp.EnableAutoDepthStencil	=	TRUE;
					d3dpp.AutoDepthStencilFormat	=	D3DFMT_D16;
					d3dpp.Windowed					=	TRUE;
					//d3dpp.PresentationInterval	=	0;
					d3dpp.PresentationInterval		=	D3DPRESENT_INTERVAL_IMMEDIATE;
					d3dpp.Flags						=	D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
					d3dpp.MultiSampleType			=	D3DMULTISAMPLE_NONE;

					LONG WindowStyle = GetWindowLongW ( m_hWnd, GWL_STYLE );
					if ( WindowStyle & WS_POPUP )
					{
						d3dpp.Windowed					=	FALSE;
					}

					if ( FAILED ( IDirect3DDevice9_Reset ( m_pD3DDevice, &d3dpp ) ) )
					{
						return false;
					}

					// Create OffScreen Surface

					RELEASEINTERFACE ( m_pSurface );
					if ( FAILED ( m_pD3DDevice->CreateOffscreenPlainSurface ( m_nWindowWidth, m_nWindowHeight, D3DFMT_X8R8G8B8, 
						//D3DPOOL_SCRATCH
						D3DPOOL_SYSTEMMEM,
						&m_pSurface, NULL ) ) )
					{
						return false;
					}

					return true;
				}
			}

			return false;
		}


		inline void SetZoomToWindow ( bool Value )
		{
			m_bZoomToWindow	 = Value;
		}


	private:

		inline void DrawTextureQuad ( D3DXVECTOR2 Vertex1, D3DXVECTOR2 Vertex2, D3DXVECTOR2 TecCoord1, D3DXVECTOR2 TecCoord2 )
		{
			struct VFTexturePoint2D
			{
				float			m_VertexX;
				float			m_VertexY;
				float			m_VertexZ;
				float			m_VertexW;

				float			m_Tex0;
				float			m_Tex1;
			};

			m_pD3DDevice->SetFVF ( D3DFVF_XYZRHW | D3DFVF_TEX1 );

			VFTexturePoint2D Vertexes[4];

			Vertexes[0].m_VertexX	=	Vertex1.x;
			Vertexes[0].m_VertexY	=	Vertex1.y;
			Vertexes[0].m_VertexZ	=	0.0f;
			Vertexes[0].m_VertexW	=	1.0f;

			Vertexes[1].m_VertexX	=	Vertex2.x;
			Vertexes[1].m_VertexY	=	Vertex1.y;
			Vertexes[1].m_VertexZ	=	0.0f;
			Vertexes[1].m_VertexW	=	1.0f;

			Vertexes[2].m_VertexX	=	Vertex2.x;
			Vertexes[2].m_VertexY	=	Vertex2.y;
			Vertexes[2].m_VertexZ	=	0.0f;
			Vertexes[2].m_VertexW	=	1.0f;

			Vertexes[3].m_VertexX	=	Vertex1.x;
			Vertexes[3].m_VertexY	=	Vertex2.y;
			Vertexes[3].m_VertexZ	=	0.0f;
			Vertexes[3].m_VertexW	=	1.0f;

			Vertexes[0].m_Tex0		=	TecCoord1.x;
			Vertexes[0].m_Tex1		=	TecCoord1.y;

			Vertexes[1].m_Tex0		=	TecCoord2.x;
			Vertexes[1].m_Tex1		=	TecCoord1.y;

			Vertexes[2].m_Tex0		=	TecCoord2.x;
			Vertexes[2].m_Tex1		=	TecCoord2.y;

			Vertexes[3].m_Tex0		=	TecCoord1.x;
			Vertexes[3].m_Tex1		=	TecCoord2.y;

			m_pD3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLEFAN, 2, Vertexes, sizeof(VFTexturePoint2D) );
		}

		inline void DrawColorQuad ( D3DXVECTOR2 Vertex1, D3DXVECTOR2 Vertex2, DWORD Color01, DWORD Color02, DWORD Color03, DWORD Color04 )
		{
			struct VFColorPoint2D
			{
				float			m_VertexX;
				float			m_VertexY;
				float			m_VertexZ;
				float			m_VertexW;

				unsigned long	m_Color;
			};

			m_pD3DDevice->SetFVF ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

			VFColorPoint2D Vertexes[4];

			Vertexes[0].m_VertexX	=	Vertex1.x;
			Vertexes[0].m_VertexY	=	Vertex1.y;
			Vertexes[0].m_VertexZ	=	0.0f;
			Vertexes[0].m_VertexW	=	1.0f;

			Vertexes[1].m_VertexX	=	Vertex2.x;
			Vertexes[1].m_VertexY	=	Vertex1.y;
			Vertexes[1].m_VertexZ	=	0.0f;
			Vertexes[1].m_VertexW	=	1.0f;

			Vertexes[2].m_VertexX	=	Vertex2.x;
			Vertexes[2].m_VertexY	=	Vertex2.y;
			Vertexes[2].m_VertexZ	=	0.0f;
			Vertexes[2].m_VertexW	=	1.0f;

			Vertexes[3].m_VertexX	=	Vertex1.x;
			Vertexes[3].m_VertexY	=	Vertex2.y;
			Vertexes[3].m_VertexZ	=	0.0f;
			Vertexes[3].m_VertexW	=	1.0f;

			Vertexes[0].m_Color		=	Color01;
			Vertexes[1].m_Color		=	Color02;
			Vertexes[2].m_Color		=	Color03;
			Vertexes[3].m_Color		=	Color04;

			m_pD3DDevice->DrawPrimitiveUP ( D3DPT_TRIANGLEFAN, 2, Vertexes, sizeof(VFColorPoint2D) );
		}


		static unsigned int __stdcall AsyncRender ( void* pArguments )
		{
#ifdef _DEBUG
			ATLTRACE (_T("START ASYNC RENDER\n"));
#endif
			FrameFastWindow* pFrameFastWindow = static_cast<FrameFastWindow*> ( pArguments );
			if ( NULL != pFrameFastWindow )
			{
				while ( true )
				{
					Sleep ( 1 );

					if ( pFrameFastWindow->m_nAsyncUpdateRender )
					{
#ifdef _DEBUG
						ATLTRACE (_T("STOP ASYNC RENDER\n"));
#endif
						return 0;
					}

					if ( FALSE == pFrameFastWindow->m_nSetImageBlock && TRUE == pFrameFastWindow->m_nSetEnableRender )
					{
						//if ( false == pFrameFastWindow->m_bZoomToWindow )
						pFrameFastWindow->Update ();
					}
					else
					{
						//Sleep ( 10 );
					}
				}
			}

#ifdef _DEBUG
			ATLTRACE (_T("STOP ASYNC RENDER\n"));
#endif
			return 0;
		}

	public:

		long						m_nIsInitialized;
		bool						m_bIsEnabled;
		bool						m_nIsTexturesComplete;

		DWORD						m_nBackClearColor;
		bool						m_bEnableAlphaLayer;

		IDirect3D9*					m_pD3D;				
		IDirect3DDevice9*			m_pD3DDevice;

		long						m_nAsyncUpdateRender;
		long						m_nSetImageBlock;
		long						m_nSetEnableRender;

		const BYTE*					m_pImage;
		int							m_nWidth;
		int							m_nHeight; 

		IDirect3DTexture9*			m_pTexureChess;
		float						m_nChessWidth;
		float						m_nChessHeight;

		CTextureBatch				m_oTextureBatch;
		long						m_nBatchSize;
		long						m_nBatchesW;
		long						m_nBatchesH;

		IDirect3DSurface9*			m_pSurface;

		int							m_nWindowWidth;
		int							m_nWindowHeight;

		long						m_bUpdateTexture;
		float						m_nScale;

		float						m_nZoom;
		float						m_nCenterX;
		float						m_nCenterY;
		bool						m_bZoomToWindow;

		HWND						m_hWnd;
		HDC							m_HDC;
		HANDLE						m_hThread;
		CRITICAL_SECTION			m_BlockingCS;

		// SMOOTH

		float						m_nZoomSmooth;

		CTimeMeasurer				m_oTimer;
	};
}

#endif