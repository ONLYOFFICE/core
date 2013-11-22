
#pragma once

#include <gdiplusex.h>
#include <structures.h>


#include <AtlDefine.h>
#include <MediaFormatDefine.h>

#define IMAGE_STUDIO_TEXT_RENDER

namespace Windows
{
	// класс, позвол€ющий просто делать отрисовку с использованием back buffer'а
	// используетс€ gdi hbitmap дл€ back buffer'а
	class CDoubleBuffer
	{
	protected:
		
		Structures::RECTI m_rect;
		HDC m_hForeDC;
		HDC m_hBackDC;
		HBITMAP m_hBackBitmap;
		HGDIOBJ m_hBackBitmapOld;
		BYTE* m_pBackBitmapPixels;

#ifdef IMAGE_STUDIO_TEXT_RENDER
		MediaCore::IAVSUncompressedVideoFrame* m_pMediaData;
#endif

	public:
		
		CDoubleBuffer()
		{
			m_hForeDC = 0;
			m_hBackDC = 0;
			m_hBackBitmap = 0;
			m_hBackBitmapOld = 0;
			m_pBackBitmapPixels = 0;

			m_pMediaData		=	NULL;
		}
		~CDoubleBuffer()
		{
			Destroy();
		}
		
		void Destroy()
		{
#ifdef IMAGE_STUDIO_TEXT_RENDER
			RELEASEINTERFACE ( m_pMediaData );
			m_pBackBitmapPixels = NULL;
#else if
			if (m_pBackBitmapPixels)
			{
				delete[] m_pBackBitmapPixels;
				m_pBackBitmapPixels = 0;
			}
#endif

			if (m_hBackDC)
			{
				SelectObject(m_hBackDC, m_hBackBitmapOld);
				DeleteObject(m_hBackBitmap);
				DeleteDC(m_hBackDC);
			}

			m_hForeDC = 0;
			m_hBackDC = 0;
			m_hBackBitmap = 0;
			m_hBackBitmapOld = 0;
		}
		BOOL Create(HDC hDC, RECT rect, BOOL bCreateBackPixelBuffer)
		{
			if (!hDC || rect.right == rect.left || rect.bottom == rect.top)
			{
				Destroy();
				return TRUE;
			}

			BOOL bReCreated = FALSE;
			Structures::RECTI rectI(rect);

			// clear resources if painting settings has changed
			if (rectI != m_rect)
				Destroy();

			m_hForeDC = hDC;
			m_rect = rectI;

			// reallocate pixel buffer only if the new size is greater than the old
			if (bCreateBackPixelBuffer)
			{
				if (!m_pBackBitmapPixels)
				{
#ifdef IMAGE_STUDIO_TEXT_RENDER
					
					if ( CreateMediaData ( ) )
					{
						m_pMediaData->get_Buffer ( &m_pBackBitmapPixels );
					}
#else if
					m_pBackBitmapPixels = new BYTE[4*m_rect.GetWidth()*m_rect.GetHeight()]; 
#endif
					bReCreated = TRUE;
				}
			}

			// create back buffer device context for smooth blitting
			if (!m_hBackBitmap)
			{
				m_hBackBitmap = CreateCompatibleBitmap(m_hForeDC, m_rect.GetWidth(), m_rect.GetHeight());
				m_hBackDC = CreateCompatibleDC(m_hForeDC);
				m_hBackBitmapOld = SelectObject(m_hBackDC, m_hBackBitmap);
				bReCreated = TRUE;
			}

			return bReCreated;
		}
		void Render()
		{
			if (!Valid())
				return;

			BitBlt(m_hForeDC, m_rect.left, m_rect.top, m_rect.GetWidth(), m_rect.GetHeight(), m_hBackDC, 0, 0, SRCCOPY);
		}
		void RenderBackBitmapPixels()
		{
			if (!Valid() || !m_pBackBitmapPixels)
				return;

			BITMAPINFO oBitmapInfo;

			oBitmapInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
			oBitmapInfo.bmiHeader.biHeight			= m_rect.GetHeight();
			oBitmapInfo.bmiHeader.biWidth			= m_rect.GetWidth();
			oBitmapInfo.bmiHeader.biPlanes			= 1;
			oBitmapInfo.bmiHeader.biBitCount		= 32;
			oBitmapInfo.bmiHeader.biCompression		= BI_RGB;
			oBitmapInfo.bmiHeader.biSizeImage		= 0;
			oBitmapInfo.bmiHeader.biXPelsPerMeter	= 1;
			oBitmapInfo.bmiHeader.biYPelsPerMeter	= 1;
			oBitmapInfo.bmiHeader.biClrUsed			= 0;
			oBitmapInfo.bmiHeader.biClrImportant	= 0;

			SetDIBitsToDevice(m_hBackDC, 0, 0, m_rect.GetWidth(), m_rect.GetHeight(), 0, 0, 0, m_rect.GetHeight(), m_pBackBitmapPixels, &oBitmapInfo, DIB_RGB_COLORS);
		}
	
		BOOL Valid()
		{
			if (!m_hForeDC || !m_hBackDC || !m_hBackBitmap)
				return FALSE;

			return TRUE;
		}
		HDC GetForeDC()
		{
			return m_hForeDC;
		}
		HDC GetBackDC()
		{
			return m_hBackDC;
		}
		Structures::RECTI GetRect()
		{
			return m_rect;
		}
		HBITMAP GetBackBitmap()
		{
			return m_hBackBitmap;
		}
		BYTE* GetBackBitmapPixels()
		{
			return m_pBackBitmapPixels;
		}

#ifdef IMAGE_STUDIO_TEXT_RENDER

		bool CreateMediaData ( )
		{
			RELEASEINTERFACE ( m_pMediaData );
			if ( SUCCEEDED ( CoCreateInstance ( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&m_pMediaData) ) ) )
			{
				if ( NULL == m_pMediaData )
					return false;

				m_pMediaData->put_ColorSpace	( CSP_BGRA );
				m_pMediaData->put_Width			( m_rect.GetWidth() );
				m_pMediaData->put_Height		( m_rect.GetHeight() );
				m_pMediaData->put_AspectRatioX	( m_rect.GetWidth() );
				m_pMediaData->put_AspectRatioY	( m_rect.GetHeight() );

				m_pMediaData->SetDefaultStrides	( );
				m_pMediaData->AllocateBuffer	( -1 );

				return true;
			}

			return false;
		}

		MediaCore::IAVSUncompressedVideoFrame* GetSurface ()
		{
			return m_pMediaData;
		}
#endif

	};
	
	//  лассы, позвол€ющие реализовывать схему с "замедленным обновлением" (delayed updates)
	// например дл€ создани€ визуальных компонент с задержкой в обновлении back-buffer'а
	// ƒл€ реализации этой схемы надо:
	//  1. породить обновл€емую компоненту от CUpdateReceiver и реализовать OnUpdate()
	//  2. в обновл€емой компоненте завести таймер и вызывать Update(this) по таймеру
	//     скорость таймера должна быть в 2 раза больше, чем скорость желаемого update'а
	//  3. в функции Update() обновл€ть состо€ние, обновл€ть State, чистить Request и 
	//     осуществл€ть перерисовку компоненты и т.п.
	namespace Update
	{
		// текущие состо€ни€ объекта - битова€ маска!
		// этот список надо составл€ть самому
		const int c_nStateNonInitialized = 0;
		const int c_nStateInitialized = 1;

		// команды дл€ обновлени€ чего-либо - битова€ маска!
		// этот список надо составл€ть самому
		const int c_nRequestUpdateNone = 0;
		const int c_nRequestUpdateAll = 1;
		const int c_nRequestRefresh = 1024;
		const int c_nRequestDelayUpdate = 2048;

		class CUpdateManager;
		class CUpdateReceiver
		{
		public:

			virtual void OnUpdate(CUpdateManager* pUpdateManager) = 0;
		};
		class CUpdateManager
		{
		public:
		
			int m_nState;
			int m_nRequest;
		
		public:
			
			CUpdateManager()
			{
				m_nState = c_nStateNonInitialized;
				m_nRequest = c_nRequestUpdateNone;
			}
				
			int StateGet()
			{
				return m_nState;
			}
			BOOL IsState(int State)
			{
				return (m_nState & State);
			}
			void StateSet(int State)
			{
				m_nState = State;
			}
			void StateAdd(int State)
			{
				m_nState |= State;
			}
			void StateRemove(int State)
			{
				m_nState = (m_nState & ~State);
			}
			
			int RequestGet()
			{
				return m_nRequest;
			}
			BOOL IsRequest(int Request)
			{
				return (m_nRequest & Request);
			}
			void RequestSet(int Request)
			{
				m_nRequest = Request;
			}
			void RequestAdd(int Request)
			{
				m_nRequest |= Request;
			}
			void RequestRemove(int Request)
			{
				m_nRequest = (m_nRequest & ~Request);
			}
			
			void Update(CUpdateReceiver* pUpdateReceiver)
			{
				// check for delayed update
				if (IsRequest(c_nRequestDelayUpdate))
					return;

				// update receiver
				if (pUpdateReceiver)
					pUpdateReceiver->OnUpdate(this);
			}
		};
	}
}