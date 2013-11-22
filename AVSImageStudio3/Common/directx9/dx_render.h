
#pragma once

#include <directx9\dx_core.h>
#include <directx9\dx_shaders.h>

namespace ImageStudio
{
	namespace dx9
	{
		static const DWORD c_dwFVF = (D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0));

		struct TCustomVertex
		{
			float x, y, z;     // координаты вершины
			float rhw;         // признак преобразованной вершины
			float u, v;        // координаты текстуры
		};

		
		static volatile IDirect3D9*       g_pDX9;                // глобальный экземпляр библиотеки DirerctX9.
		static volatile IDirect3DDevice9* g_pDevice;             // глобальное устройство
		static volatile HWND              g_hWindow;             // окно связанное с глобальным устройством
		static volatile LONG              g_lCountLocalDevices;  // счётчик созданных локальных устройств
		static volatile DWORD             g_nMaxTextureWidth;    // максимальная ширина текстуры
		static volatile DWORD             g_nMaxTextureHeight;   // максимальная высота текстуры
		static volatile DWORD             g_nPixelShaderVersion; // максимальная версия пиксельного шейдера
		static volatile DWORD             g_nMaxAnisotropy;      // максимальный уровень анизотропной фильтрации
		static volatile BOOL              g_bHardwareVertexProc; // аппаратная обработка вершин


		class CRender
		{
		protected:
			class CInitMutex
			{
			protected:
				HANDLE m_hMutex;

			public:
				CInitMutex()
				{
					m_hMutex = 0;
				}

				~CInitMutex()
				{
					Leave();
				}


				void Enter()
				{
					do
					{
						m_hMutex = ::CreateMutex( NULL, TRUE, _T("AVSInitRenderDX9") );
					}while( !m_hMutex );
				}

				void Leave()
				{
					if( m_hMutex )
					{
						::ReleaseMutex( m_hMutex );
						m_hMutex = 0;
					}
				}
			};

		
		protected:
			static const DWORD c_nWndClientSize = 64;

		private:
			IDirect3DDevice9*       m_pDevice; // локальная копия устройства
			IDirect3DTexture9*      m_pTarget; // текстура, в которую была сделана последняя отрисовка
			IDirect3DVertexBuffer9* m_pBuffer; // буфер вершин
			
			BOOL m_bInitialize; // флаг наличия инициализации
			BOOL m_bLostDevice; // потеря устройства (обнаруживается через анализ кода возврата функций)

			int m_nTargetWidth;  // ширина текущего рендертаргета (нужно для буфера вершин)
			int m_nTargetHeight; // высота текущего рендертаргета (нужно для буфера вершин)

			CSampler::EFilter m_eDefaultFilter;

			CTablePSs g_oShaderTable;

		public:
			CRender()
			{
				m_pDevice = NULL;
				m_pTarget = NULL;
				m_pBuffer = NULL;

				m_bInitialize = FALSE;
				m_bLostDevice = FALSE;

				m_nTargetWidth  = 0;
				m_nTargetHeight = 0;

				m_eDefaultFilter = CSampler::c_nDefault;
			}

			~CRender()
			{
				Release();
			}

			
			void Release()
			{
				if( !m_bInitialize )
					return;

				CInitMutex mutex;
				mutex.Enter();

				ReleaseLocalDevice();

				if( !g_lCountLocalDevices )
				{
					ReleaseGlobalDevice();
					ReleaseRenderWindow();
					ReleaseDirectX();
				}
			}

			IDirect3DDevice9* Initialize()
			{
				if( m_pDevice )
				{
					if( !m_bLostDevice )
						return m_pDevice;

					CInitMutex mutex;
					mutex.Enter();

					DXTRACE_MSG(_T("Device is lost!"));

					ReleaseLocalDevice();

					if( !g_pDevice )
						return NULL;

					HRESULT hr = ((IDirect3DDevice9*)g_pDevice)->TestCooperativeLevel();
					if( hr == D3D_OK )
					{
						return InitLocalDevice();
					}

					if( hr == D3DERR_DEVICENOTRESET )
					{
						if( ResetGlobalDevice( (IDirect3DDevice9*)g_pDevice ) )
						{
							return InitLocalDevice();
						}
					}

					ReleaseGlobalDevice();
					
					g_pDevice = CreateGlobalDevice();
					
					return InitLocalDevice();
				}

				if( m_bInitialize )
				{
					if( !g_pDX9 )
						return NULL;
					
					DXTRACE_MSG(_T("m_bInitialize = TRUE and local device is bad"));

					CInitMutex mutex;
					mutex.Enter();

					ReleaseGlobalDevice();

					g_pDevice = CreateGlobalDevice();
					
					return InitLocalDevice();
				}

				CInitMutex mutex;
				mutex.Enter();

				if( g_lCountLocalDevices < 0 )
				{
					// инициализация была проведена ранее, но неудачно
					DXTRACE_MSG(_T("g_lCount < 0, local device is bad!"));
					
					m_bInitialize = TRUE;
					
					return NULL;
				}

				if( !g_pDX9 )
				{
					DXTRACE_MSG(_T("Initialize global device"));

					BOOL bSuccess = FALSE;

					do
					{
						g_pDX9 = CreateDirectX();
						if( !g_pDX9 )
							break;

						g_hWindow = CreateRenderWindow();
						if( !g_hWindow )
							break;

						g_pDevice = CreateGlobalDevice();
						if( !g_pDevice )
							break;

						bSuccess = TRUE;
					}while( 0 );
						
					if( !bSuccess )
					{
						ReleaseGlobalDevice();
						ReleaseDirectX();
						ReleaseRenderWindow();

						g_lCountLocalDevices = -1;
						
						DXTRACE_MSG(_T("Bad initialize global device!!!"));
						
						return NULL;
					}
				}

				return InitLocalDevice();
			}

			
			IDirect3DDevice9* IsValid()
			{
				IDirect3DDevice9* pDevice = GetDevice();
				if( !pDevice )
					return NULL;

				HRESULT hr = pDevice->TestCooperativeLevel();
				if( FAILED(hr) )
				{
					m_bLostDevice = TRUE;
					return NULL;
				}

				return pDevice;
			}

			BOOL CheckError( HRESULT code )
			{
				if( code == D3D_OK )
					return FALSE;

				//if( //code == D3DERR_DEVICEHUNG ||
				//	code == D3DERR_DEVICELOST ||
				//	code == D3DERR_DEVICENOTRESET ||
				//	code == D3DERR_INVALIDDEVICE ||
				//	//code == D3DERR_DEVICEREMOVED ||
				//	code == D3DERR_DRIVERINTERNALERROR ||
				//	code == D3DERR_DRIVERINVALIDCALL ||
				//	code == D3DERR_NOTAVAILABLE ||
				//	code == D3DERR_INVALIDCALL )
				//	//code == S_PRESENT_MODE_CHANGED ||
				//	//code == S_PRESENT_OCCLUDED )
				{
					DXTRACE_ERR(_T("DEVICE IS LOST!"), code);
					m_bLostDevice = TRUE;
				};

				return TRUE;
			}

			void ReleaseTarget()
			{
				RELEASEINTERFACE(m_pTarget);
			}
			void ReleaseBuffer()
			{
				RELEASEINTERFACE(m_pBuffer);
				m_nTargetWidth  = 0;
				m_nTargetHeight = 0;
			}

			
			BOOL LockDevice()
			{
				if( IsBadDevice() )
					return NULL;

				//m_oMutex.Enter();

				// !!!очень опасный участок кода!!!
				HRESULT hr = D3D_OK;
				do
				{
					if( SUCCEEDED(m_pDevice->BeginScene()) )
						break;

					hr = m_pDevice->TestCooperativeLevel();
				}while(SUCCEEDED(hr));
				
				
				if( FAILED(hr)/*CheckError( hr )*/ )
				{
					DXTRACE_ERR( 0, hr );
					//m_oMutex.Leave();
					return FALSE;
				}

				return TRUE;
			}

			void UnlockDevice()
			{
				HRESULT hr = m_pDevice->EndScene();
				if( FAILED(hr)/*CheckError( hr )*/ )
				{
					DXTRACE_ERR( 0, hr );
				}
					
				//m_oMutex.Leave();
			}


			
			IDirect3DTexture9* CreateTexture( int nWidth, int nHeight, D3DFORMAT nFormat, D3DPOOL pool )
			{
				if( IsBadDevice() )
					return NULL;
				
				IDirect3DTexture9* pTexture = NULL;
				HRESULT hr = D3DXCreateTexture( m_pDevice, nWidth, nHeight, 1, 0, nFormat, pool, &pTexture );
				if( CheckError(hr) )
				{
					DXTRACE_ERR(0, hr );
					return NULL;
				}
				
				return pTexture;
			}

			BOOL Draw( IPixelShader* pShader, CSampler* pSampler = NULL )
			{
				if( pShader )
				{
					if( !pShader->SetToDevice( m_pDevice ) )
						return FALSE;
				}
				else
				{
					m_pDevice->SetPixelShader( NULL );
					
					if( pSampler )
						pSampler->SetToDevice( m_pDevice );
				}

				HRESULT hr = m_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 );
				if( CheckError(hr) )
				{
					DXTRACE_ERR(0, hr);
					return FALSE;
				}

				return TRUE;
			}
			void ResetPixelShader()
			{
				m_pDevice->SetPixelShader( 0 );
			}
			
			BOOL CreateTarget( int nWidth, int nHeight, /*D3DCOLOR nBackColor = 0,*/ D3DFORMAT nFormat = D3DFMT_A8R8G8B8 )
			{
				if( IsBadDevice() )
					return FALSE;

				IDirect3DTexture9* pTexture = NULL;
				IDirect3DSurface9* pTarget  = NULL;

				BOOL bSuccess = FALSE;
				
				do
				{
					HRESULT hr = D3DXCreateTexture( m_pDevice, nWidth, nHeight, 1, D3DUSAGE_RENDERTARGET, nFormat, D3DPOOL_DEFAULT, &pTexture );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					hr = pTexture->GetSurfaceLevel( 0, &pTarget );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					hr = m_pDevice->SetRenderTarget( 0, pTarget );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					if( m_nTargetWidth != nWidth || m_nTargetHeight != nHeight || !m_pBuffer )
					{
						IDirect3DVertexBuffer9* pBuffer = CreateVertexBuffer( nWidth, nHeight );
						if( !pBuffer )
							break;

						RELEASEINTERFACE(m_pBuffer);
						m_pBuffer = pBuffer;

						m_nTargetWidth  = nWidth;
						m_nTargetHeight = nHeight;
					}

					hr = m_pDevice->SetStreamSource( 0, m_pBuffer, 0, sizeof(TCustomVertex) );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					bSuccess = TRUE;
				}while(0);

				RELEASEINTERFACE(pTarget);

				if( !bSuccess )
				{
					RELEASEINTERFACE(pTexture);
					return FALSE;
				}

				if( m_pTarget )
					m_pTarget->Release();

				m_pTarget = pTexture;

				//m_pDevice->Clear( 0, 0, D3DCLEAR_TARGET, nBackColor, 1, 0 );

				return TRUE;
			}

			IDirect3DTexture9* GetRenderTargetTexture()
			{
				if( m_pTarget )
					m_pTarget->AddRef();

				return m_pTarget;
			}

			void SetRenderTargetTexture( IDirect3DTexture9* pTexture )
			{
				if( m_pTarget )
					m_pTarget->Release();

				m_pTarget = pTexture;
				
				if( pTexture )
					pTexture->AddRef();
			}

			IDirect3DTexture9* TargetToTexture()
			{
				if( IsBadDevice() )
					return NULL;

				if( !m_pTarget )
					return NULL;

				IDirect3DTexture9* pTexture = NULL;
				IDirect3DSurface9* pTarget = NULL;
				IDirect3DSurface9* pResult = NULL;

				BOOL bSuccess = FALSE;

				do
				{
					HRESULT hr = m_pTarget->GetSurfaceLevel( 0, &pTarget );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}


					D3DSURFACE_DESC desc;
					hr = pTarget->GetDesc( &desc );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					pTexture = CreateTexture( desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM );
					if( !pTexture )
					{
						break;
					}

					hr = pTexture->GetSurfaceLevel( 0, &pResult );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					hr = m_pDevice->GetRenderTargetData( pTarget, pResult );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					bSuccess = TRUE;
				}while(0);

				RELEASEINTERFACE(pTarget);
				RELEASEINTERFACE(pResult);

				if( !bSuccess )
				{
					RELEASEINTERFACE(pTexture);
					return NULL;
				}

				return pTexture;
			}

			
			BOOL FillTarget( D3DCOLOR color )
			{
				if( IsBadDevice() )
					return FALSE;

				HRESULT hr = m_pDevice->Clear( 0, 0, D3DCLEAR_TARGET, color, 1, 0 );
				if( CheckError(hr) )
				{
					DXTRACE_ERR(0, hr);
					return FALSE;
				}

				return TRUE;
			}

			
			IDirect3DTexture9* ResizeTexture( IDirect3DTexture9* pSource, int nNewWidth, int nNewHeight )
			{
				D3DSURFACE_DESC desc;
				HRESULT hr = pSource->GetLevelDesc( 0, &desc );
				if( CheckError( hr ) )
				{
					DXTRACE_ERR(0, hr);
					return NULL;
				}

				if( desc.Width == nNewWidth && desc.Height == nNewHeight )
				{
					pSource->AddRef();
					return pSource;
				}

				IDirect3DTexture9* pTexture = NULL;
				IDirect3DSurface9* pTarget = NULL;

				BOOL bSuccess = FALSE;
				
				do
				{
					HRESULT hr = D3DXCreateTexture( m_pDevice, nNewWidth, nNewHeight, 1, D3DUSAGE_RENDERTARGET, desc.Format, D3DPOOL_DEFAULT, &pTexture );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					hr = pTexture->GetSurfaceLevel( 0, &pTarget );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					hr = m_pDevice->SetRenderTarget( 0, pTarget );
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					IDirect3DVertexBuffer9* pBuffer = CreateVertexBuffer( nNewWidth, nNewHeight );
					if( !pBuffer )
						break;

					hr = m_pDevice->SetStreamSource( 0, pBuffer, 0, sizeof(TCustomVertex) );
					
					pBuffer->Release();
					
					if( CheckError(hr) )
					{
						DXTRACE_ERR(0, hr);
						break;
					}

					CSampler sampler;
					sampler.SetTexture( pSource );
					sampler.SetFilter( GetFilter() );

					if( !Draw( NULL, &sampler ) )
						break;

					bSuccess = TRUE;
				}while(0);

				RELEASEINTERFACE(pTarget);

				if( !bSuccess )
				{
					RELEASEINTERFACE(pTexture);
					return NULL;
				}
				
				return pTexture;
			}
			
			BOOL IsBadDevice()
			{
				return 0 == m_pDevice;
			}

			BOOL IsBadTexWidth( int width ) const
			{
				if( width <= 0 )
				{
					ATLTRACE(_T("Width of texture %d <= 0!"), width);
					return TRUE;
				}
				if( (DWORD)width > g_nMaxTextureWidth )
				{
					ATLTRACE(_T("Width of texture %d > %d (max width)!"), width, g_nMaxTextureWidth );
					return TRUE;
				}
				
				return FALSE;
			}

			BOOL IsBadTexHeight( int height ) const
			{
				if( height <= 0 )
				{
					ATLTRACE(_T("Height of texture %d <= 0!"), height);
					return TRUE;
				}
				if( (DWORD)height > g_nMaxTextureHeight )
				{
					ATLTRACE(_T("Height of texture %d > %d (max height)!"), height, g_nMaxTextureHeight );
					return TRUE;
				}
				
				return FALSE;
			}

			
			IDirect3DDevice9* GetDevice()
			{
				return Initialize();
			}
			IDirect3DPixelShader9* GetPixelShader( EPixelShader shader )
			{
				if( IsBadPixelShader( shader ) )
					return NULL;

				return g_oShaderTable.GetShader( shader, m_pDevice );
			}

			BOOL IsBadPixelShader( EPixelShader shader ) const
			{
				return CTablePSs::GetVersion( shader ) > g_nPixelShaderVersion;
			}

			
			BOOL SetDefaultFilter( int filter )
			{
				if( filter >= CSampler::c_nDefault && filter <= CSampler::c_nAnisot16 )
				{
					m_eDefaultFilter = (CSampler::EFilter)filter;
					return TRUE;
				}

				return FALSE;
			}

			CSampler::EFilter GetFilter() const
			{
				switch( m_eDefaultFilter )
				{
				case CSampler::c_nDefault:  return GetAnisotFilter( g_nMaxAnisotropy );
				case CSampler::c_nNearest:  return CSampler::c_nNearest;
				case CSampler::c_nLinear:   return CSampler::c_nLinear;
				case CSampler::c_nAnisot1:  return GetAnisotFilter( min(g_nMaxAnisotropy, 1) );
				case CSampler::c_nAnisot2:  return GetAnisotFilter( min(g_nMaxAnisotropy, 2) );
				case CSampler::c_nAnisot4:  return GetAnisotFilter( min(g_nMaxAnisotropy, 4) );
				case CSampler::c_nAnisot8:  return GetAnisotFilter( min(g_nMaxAnisotropy, 8) );
				case CSampler::c_nAnisot16: return GetAnisotFilter( min(g_nMaxAnisotropy, 16) );
				default: return CSampler::c_nDefault;
				}
			}

		
		private:
			IDirect3DDevice9* InitLocalDevice()
			{
				m_pDevice = (IDirect3DDevice9*)g_pDevice;
				if( m_pDevice )
				{
					DXTRACE_MSG(_T("Initialize local device"));

					m_pDevice->AddRef();

					++g_lCountLocalDevices;
				}

				m_bInitialize = TRUE;

				//m_eDefaultFilter = CSampler::c_nLinear;
				//m_eDefaultPS = IPixelShader::c_nDefault;

				return m_pDevice;
			}

			void ReleaseLocalDevice()
			{
				ReleaseTarget();
				ReleaseBuffer();
				
				g_oShaderTable.Release();
				
				if( m_pDevice )
				{
					DXTRACE_MSG(_T("Release local device"));

					m_pDevice->Release();
					m_pDevice = NULL;

					--g_lCountLocalDevices;
				}

				m_bInitialize = FALSE;
				m_bLostDevice = FALSE;
			}

			IDirect3DVertexBuffer9* CreateVertexBuffer( int nImageWidth, int nImageHeight )
			{
				const DWORD dwBufSize = 6 * sizeof(TCustomVertex);

				IDirect3DVertexBuffer9* pBuffer = NULL;
				HRESULT hr = m_pDevice->CreateVertexBuffer( dwBufSize, D3DUSAGE_WRITEONLY, c_dwFVF, D3DPOOL_MANAGED, &pBuffer, NULL );
				if( CheckError(hr) )
				{
					DXTRACE_ERR(0, hr);
					return NULL;
				}

				TCustomVertex* data = NULL;
				hr = pBuffer->Lock( 0, dwBufSize, (void**)&data, 0 );
				if( CheckError(hr) )
				{
					DXTRACE_ERR(0, hr);
					pBuffer->Release();
					return NULL;
				}

				float fw = float(nImageWidth  - 0.5);
				float fh = float(nImageHeight - 0.5);

				data[0].x   = -0.5f;
				data[0].y   = -0.5f;
				data[0].z   = 0;
				data[0].rhw = 1;
				data[0].u   = 0;
				data[0].v   = 0;

				data[1].x   = fw;
				data[1].y   = fh;
				data[1].z   = 0;
				data[1].rhw = 1;
				data[1].u   = 1;
				data[1].v   = 1;

				data[2].x   = -0.5f;
				data[2].y   = fh;
				data[2].z   = 0;
				data[2].rhw = 1;
				data[2].u   = 0;
				data[2].v   = 1;

				data[3].x   = -0.5f;
				data[3].y   = -0.5f;
				data[3].z   = 0;
				data[3].rhw = 1;
				data[3].u   = 0;
				data[3].v   = 0;

				data[4].x   = fw;
				data[4].y   = -0.5f;
				data[4].z   = 0;
				data[4].rhw = 1;
				data[4].u   = 1;
				data[4].v   = 0;

				data[5].x   = fw;
				data[5].y   = fh;
				data[5].z   = 0;
				data[5].rhw = 1;
				data[5].u   = 1;
				data[5].v   = 1;

				hr = pBuffer->Unlock();
				if( CheckError( hr ) )
				{
					DXTRACE_ERR(0, hr);
					pBuffer->Release();
					return NULL;
				}

				return pBuffer;
			}

			
			static IDirect3D9* CreateDirectX()
			{
				IDirect3D9* pDX9 = NULL;

				try
				{
					pDX9 = Direct3DCreate9( D3D_SDK_VERSION );
				}
				catch(...)
				{
					DXTRACE_MSG(_T("Exception! Can`t create direct3d!"));
				}

				if( !pDX9 )
				{
					DXTRACE_MSG(_T("Error! Can`t create direct3d!"));
					return NULL;
				}

				if( !CheckCaps( pDX9 ) )
				{
					DXTRACE_MSG(_T("Warning! Caps is invalid!"));
					pDX9->Release();
					return NULL;
				}

				return pDX9;
			}

			static BOOL CheckCaps( IDirect3D9* pDX9 )
			{
				// проверка требуемых возможностей видеокарты
				D3DCAPS9 caps;
				HRESULT hr = pDX9->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );
				if( FAILED(hr) )
				{
					DXTRACE_ERR(0, hr);
					return FALSE;
				}

				g_nPixelShaderVersion = caps.PixelShaderVersion;
				g_nMaxTextureWidth    = caps.MaxTextureWidth;
				g_nMaxTextureHeight   = caps.MaxTextureHeight;
				g_nMaxAnisotropy      = caps.MaxAnisotropy;
				g_bHardwareVertexProc = (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) ? TRUE : FALSE;

				/*g_oShaderTable.*/m_dwMaxVersion = caps.PixelShaderVersion & 0x0000ffff;

				return TRUE;
			}

			static HWND CreateRenderWindow()
			{
				LPCTSTR szClassName = _T("AVSDX9RenderWindow");

				HINSTANCE hInstance = ::GetModuleHandle( NULL );
				
				WNDCLASSEX wcex;
				ZeroMemory( &wcex, sizeof(WNDCLASSEX) );

				wcex.cbSize         = sizeof(WNDCLASSEX);
				wcex.lpfnWndProc	= ::DefWindowProc;
				wcex.hInstance		= hInstance;
				wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
				wcex.lpszClassName	= szClassName;
				wcex.style          = CS_NOCLOSE | CS_OWNDC;

				RegisterClassEx( &wcex );

				DWORD dwStyle   = WS_DISABLED | WS_POPUP;
				DWORD dwStyleEx = WS_EX_TOPMOST;

				RECT rect = {0, 0, c_nWndClientSize, c_nWndClientSize};
				AdjustWindowRectEx( &rect, dwStyle, FALSE, dwStyleEx );
				
				HWND hWindow = ::CreateWindowEx( dwStyleEx, szClassName, szClassName, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom, ((HWND)-3), 0, hInstance, 0);
				if( !hWindow )
				{
					DXTRACE_MSG(_T("Can`t create window"));
					return NULL;
				}
				
				//::ShowWindow( hWindow, 1 );
				//::UpdateWindow( hWindow );

				return hWindow;
			}

			static void ReleaseRenderWindow()
			{
				if( g_hWindow )
				{
					DestroyWindow( g_hWindow );
					g_hWindow = NULL;
				}
			}
			static void ReleaseGlobalDevice()
			{
				if( g_pDevice )
				{
					//g_oShaderTable.Release();

					DXTRACE_MSG(_T("Release global device!"));
					((IDirect3DDevice9*)g_pDevice)->Release();
					g_pDevice = NULL;
				}
			}

			static void ReleaseDirectX()
			{
				if( g_pDX9 )
				{
					((IDirect3D9*)g_pDX9)->Release();
					g_pDX9 = NULL;
				}
			}

			static void FillPresentParams( D3DPRESENT_PARAMETERS &d3dpp )
			{
				d3dpp.BackBufferWidth            = 0; //c_nWndClientSize;
				d3dpp.BackBufferHeight           = 0; //c_nWndClientSize;
				d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;
				d3dpp.BackBufferCount            = 0;
				d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
				d3dpp.MultiSampleQuality         = 0;
				d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
				d3dpp.hDeviceWindow              = g_hWindow;
				d3dpp.Windowed                   = TRUE;
				d3dpp.EnableAutoDepthStencil     = FALSE;
				d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
				d3dpp.Flags                      = 0;
				d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
				d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT; //D3DPRESENT_INTERVAL_IMMEDIATE;
			}


			static BOOL InitGlobalDevice( IDirect3DDevice9* pDevice )
			{
				// общие настройки
				pDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
				pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
				pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
				pDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );
				pDevice->SetFVF( c_dwFVF );
				
				return TRUE;
			}

			static BOOL ResetGlobalDevice( IDirect3DDevice9* pDevice )
			{
				D3DPRESENT_PARAMETERS d3dpp;
				FillPresentParams( d3dpp );
				
				HRESULT hr = pDevice->Reset( &d3dpp );
				if( FAILED(hr) )
				{
					DXTRACE_ERR(_T("Error! Can`t reset device!"), hr);
					return FALSE;
				}

				return InitGlobalDevice( pDevice );
			}

			static IDirect3DDevice9* CreateGlobalDevice()
			{
				if( !g_pDX9 )
					return NULL;

				D3DPRESENT_PARAMETERS d3dpp;
				FillPresentParams( d3dpp );

				IDirect3DDevice9* pDevice = NULL;
				HRESULT hr = ((IDirect3D9*)g_pDX9)->CreateDevice( 
						D3DADAPTER_DEFAULT, 
						D3DDEVTYPE_HAL, 
						0, 
						D3DCREATE_NOWINDOWCHANGES | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE |
						(g_bHardwareVertexProc ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING),
						&d3dpp, 
						&pDevice );

				if( FAILED(hr) )
				{
					DXTRACE_ERR(0, hr);
					return NULL;
				}

				if( !InitGlobalDevice( pDevice ) )
				{
					pDevice->Release();
					return NULL;
				}

				return pDevice;
			}

			static CSampler::EFilter GetAnisotFilter( DWORD level )
			{
				switch( level )
				{
				case 1:  return CSampler::c_nAnisot1;
				case 2:  return CSampler::c_nAnisot2;
				case 4:  return CSampler::c_nAnisot4;
				case 8:  return CSampler::c_nAnisot8;
				case 16: return CSampler::c_nAnisot16;
				default: return CSampler::c_nDefault;
				}
			}

		};

	}
}