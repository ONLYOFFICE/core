
#pragma once

#include <directx9\dx_core.h>

namespace ImageStudio
{
	namespace dx9
	{
		class IShader
		{
		public:
			virtual const DWORD* GetCode() const = 0;
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const = 0;
		};

		class IPixelShader : public IShader
		{
		protected:
			IDirect3DPixelShader9* m_pShader;
		
		public:
			IPixelShader()
			{
				m_pShader = NULL;
			}

			~IPixelShader()
			{
				Release();
			}

			void Release()
			{
				RELEASEINTERFACE(m_pShader);
			}

			
			BOOL Set( IDirect3DPixelShader9* pShader )
			{
				if( m_pShader )
					m_pShader->Release();

				m_pShader = pShader;
				if( pShader )
					pShader->AddRef();

				return pShader != NULL;
			}

			BOOL Create( IDirect3DDevice9* pDevice )
			{
				Release();

				const DWORD* pCode = GetCode();
				if( !pCode )
					return FALSE;

				try
				{
					IDirect3DPixelShader9* pShader = 0;
					HRESULT hr = pDevice->CreatePixelShader( pCode, &pShader );
					if( SUCCEEDED(hr) )
						m_pShader = pShader;
				}
				catch(...)
				{
					ATLTRACE(_T("Exception! Can`t create pixel shader!"));
				}

				return m_pShader != NULL;
			}

			BOOL SetToDevice( IDirect3DDevice9* pDevice ) const
			{
				if( !m_pShader )
					return FALSE;

				BOOL bRetCode = FALSE;

				if( SetConstants( pDevice ) )
				{
					HRESULT hr = pDevice->SetPixelShader( m_pShader );
					bRetCode = SUCCEEDED(hr);
				}

				return bRetCode;
			};

			
			DWORD GetVersion() const
			{
				return GetVersionFromCode( GetCode() );
			}

			static DWORD GetVersionFromCode( const DWORD* pCode )
			{
				if( pCode )
					return pCode[0] & 0x0000ffff;

				return 0;
			}

		};

		
		class PS_Ripple : public IPixelShader
		{
		public:
			float m_fOmega;
			float m_fDisp;
			float m_fAmpl;
			float m_fWidth;
			float m_fHeight;

			IDirect3DTexture9* m_pTexture;
			CSampler::EFilter  m_eFilter;

		public:
			PS_Ripple()
			{
				m_fOmega   = 0;
				m_fDisp    = 0;
				m_fAmpl    = 0;
				m_fWidth   = 0;
				m_fHeight  = 0;

				m_pTexture = 0;
				m_eFilter  = CSampler::c_nDefault;
			}


			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x00fafffe, 0x47554244, 0x00000028, 0x000003b0, 0x00000000, 0x00000001, 0x0000003c, 
					0x00000026, 0x00000040, 0x00000008, 0x00000310, 0x00000170, 0x705c3a64, 0x69725f73, 0x656c7070, 
					0x7478742e, 0xababab00, 0x00000028, 0xffff0000, 0x00000524, 0xffff0000, 0x0000053c, 0xffff0000, 
					0x00000554, 0xffff0000, 0x0000056c, 0xffff0000, 0x00000578, 0x00000028, 0x00000584, 0x00000029, 
					0x00000594, 0x00000029, 0x000005a0, 0x00000028, 0x000005b4, 0x0000002b, 0x000005c4, 0x0000002f, 
					0x000005d4, 0x00000030, 0x000005e8, 0x00000031, 0x000005fc, 0x00000033, 0x0000060c, 0x00000033, 
					0x00000618, 0x00000033, 0x00000624, 0x00000033, 0x00000630, 0x00000033, 0x0000063c, 0x00000033, 
					0x00000648, 0x00000034, 0x00000654, 0x00000034, 0x00000660, 0x00000035, 0x00000674, 0x00000035, 
					0x00000688, 0x00000035, 0x00000694, 0x00000035, 0x000006a8, 0x00000035, 0x000006b8, 0x00000035, 
					0x000006cc, 0x00000035, 0x000006e0, 0x00000035, 0x000006f4, 0x00000035, 0x00000708, 0x00000037, 
					0x0000071c, 0x00000038, 0x0000072c, 0x0000003a, 0x0000073c, 0x0000003a, 0x00000750, 0x0000003a, 
					0x0000075c, 0x0000003a, 0x0000076c, 0x0000003c, 0x0000077c, 0x0000003c, 0x0000078c, 0x6e69614d, 
					0x66696400, 0x65737566, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x00000175, 
					0x00000180, 0x00000005, 0x00040001, 0x00010001, 0x00000190, 0x00000025, 0x00010000, 0x00030002, 
					0x67656d6f, 0xabab0061, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x00000013, 0xffffffff, 
					0x0000ffff, 0x657a6973, 0xababab00, 0x00000006, 0x00010000, 0xffffffff, 0x75706e69, 0x61620074, 
					0xab006573, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x000001f2, 0x000001f8, 0x00000005, 
					0x00020001, 0x00010001, 0x00000208, 0x00000003, 0x00010000, 0xffffffff, 0x7074756f, 0xab007475, 
					0x00000024, 0x00010000, 0x00030002, 0xab003170, 0x00000008, 0x00010000, 0xffffffff, 0x00000009, 
					0x00010000, 0xffffffff, 0x0000001e, 0xffff0000, 0xffffffff, 0x0000001f, 0x0001ffff, 0xffffffff, 
					0xab003270, 0x00000007, 0x00010000, 0xffffffff, 0x00000022, 0xffff0000, 0xffffffff, 0x00000023, 
					0x0001ffff, 0xffffffff, 0x00736f70, 0x00030001, 0x00030001, 0x00000001, 0x00000000, 0x0000000a, 
					0xffffffff, 0x0000ffff, 0x0000000b, 0xffffffff, 0x0001ffff, 0x0000000c, 0xffffffff, 0x0002ffff, 
					0x0000000e, 0xffff0000, 0xffffffff, 0x00000010, 0x0001ffff, 0xffffffff, 0x00000012, 0xffffffff, 
					0xffff0002, 0x00000014, 0x00010000, 0xffff0002, 0x0000001d, 0x00010000, 0xffff0002, 0x00000000, 
					0x00000170, 0x00000198, 0x00000001, 0x000001a8, 0x00000000, 0x000001b4, 0x000001bc, 0x00000001, 
					0x000001cc, 0x00000000, 0x000001d8, 0x00000180, 0x00000001, 0x000001e0, 0x00000170, 0x000001ec, 
					0x00000210, 0x00000001, 0x00000220, 0x00000170, 0x0000022c, 0x00000198, 0x00000001, 0x00000234, 
					0x00000170, 0x00000240, 0x000001f8, 0x00000004, 0x00000244, 0x00000170, 0x00000274, 0x000001f8, 
					0x00000003, 0x00000278, 0x00000170, 0x0000029c, 0x000002a0, 0x00000008, 0x000002b0, 0x7263694d, 
					0x666f736f, 0x52282074, 0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 
					0x34302e35, 0x2e30302e, 0x34303932, 0xababab00, 0x004cfffe, 0x42415443, 0x0000001c, 0x000000fb, 
					0xffff0200, 0x00000005, 0x0000001c, 0x00000101, 0x000000f4, 0x00000080, 0x00000003, 0x00000001, 
					0x00000088, 0x00000000, 0x00000098, 0x00020002, 0x00000001, 0x000000a0, 0x000000b0, 0x000000c0, 
					0x00010002, 0x00000001, 0x000000c8, 0x000000b0, 0x000000d8, 0x00000002, 0x00000001, 0x000000c8, 
					0x000000b0, 0x000000de, 0x00030002, 0x00000001, 0x000000e4, 0x000000b0, 0x65736142, 0x00786554, 
					0x000c0004, 0x00010001, 0x00000001, 0x00000000, 0x6c706d61, 0xababab00, 0x00030001, 0x00020001, 
					0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x70736964, 0xababab00, 
					0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x67656d6f, 0x69730061, 0xab00657a, 0x00030001, 
					0x00040001, 0x00000001, 0x00000000, 0x325f7370, 0x4d00305f, 0x6f726369, 0x74666f73, 0x29522820, 
					0x44334420, 0x53203958, 0x65646168, 0x6f432072, 0x6c69706d, 0x35207265, 0x2e34302e, 0x322e3030, 
					0x00343039, 0x05000051, 0xa00f0004, 0xbab609ba, 0x3d2aaaa4, 0xbf000000, 0x3f800000, 0x05000051, 
					0xa00f0005, 0xbf000000, 0xbf800000, 0x3e22f983, 0x3e800000, 0x05000051, 0xa00f0006, 0x40c90fdb, 
					0xc0490fdb, 0xb4878163, 0x37cfb5a1, 0x0200001f, 0x80000000, 0xb0030000, 0x0200001f, 0x90000000, 
					0xa00f0800, 0x03000002, 0x80030000, 0xb0e40000, 0xa0000005, 0x02000001, 0x80030001, 0xa0e40003, 
					0x04000004, 0x80030001, 0x80e40000, 0x80e40001, 0xa0550005, 0x03000005, 0x80030000, 0x80e40000, 
					0xa0e40003, 0x03000005, 0x80030000, 0x80e40000, 0x80e40000, 0x04000004, 0x80080002, 0x80000001, 
					0x80000001, 0x80550000, 0x04000004, 0x80080001, 0x80550001, 0x80550001, 0x80000000, 0x03000002, 
					0x80080000, 0x80550000, 0x80000000, 0x02000007, 0x80080002, 0x80ff0002, 0x02000006, 0x80010000, 
					0x80ff0002, 0x02000007, 0x80080001, 0x80ff0001, 0x02000006, 0x80020000, 0x80ff0001, 0x02000007, 
					0x80080000, 0x80ff0000, 0x02000006, 0x80040000, 0x80ff0000, 0x02000001, 0x80080000, 0xa0000000, 
					0x04000004, 0x80070000, 0x80e40000, 0x80ff0000, 0xa0000001, 0x04000004, 0x80070000, 0x80e40000, 
					0xa0aa0005, 0xa0ff0005, 0x02000013, 0x80070000, 0x80e40000, 0x04000004, 0x80070000, 0x80e40000, 
					0xa0000006, 0xa0550006, 0x03000005, 0x80070000, 0x80e40000, 0x80e40000, 0x04000004, 0x80070001, 
					0x80e40000, 0xa0aa0006, 0xa0ff0006, 0x04000004, 0x80070001, 0x80e40000, 0x80e40001, 0xa0000004, 
					0x04000004, 0x80070001, 0x80e40000, 0x80e40001, 0xa0550004, 0x04000004, 0x80070001, 0x80e40000, 
					0x80e40001, 0xa0aa0004, 0x04000004, 0x80070000, 0x80e40000, 0x80e40001, 0xa0ff0004, 0x03000002, 
					0x80010000, 0x81000000, 0x80aa0000, 0x03000002, 0x80020000, 0x81550000, 0x80aa0000, 0x04000004, 
					0x80030000, 0xa0e40002, 0x80e40000, 0xb0e40000, 0x02000023, 0x80030000, 0x80e40000, 0x03000002, 
					0x80010000, 0x80000000, 0xa0aa0003, 0x03000002, 0x80020000, 0x80550000, 0xa0ff0003, 0x03000042, 
					0x800f0000, 0x80e40000, 0xa0e40800, 0x02000001, 0x800f0800, 0x80e40000, 0x0000ffff, 
				};
				
				return shader;
			}


			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float vOmega[4] = {m_fOmega, 0, 0, 0};
				float vDisp[4]  = {-m_fDisp,  0, 0, 0};
				float vAmpl[4]  = {m_fAmpl / m_fWidth, m_fAmpl / m_fHeight, 0, 0};
				float vSize[4]  = {m_fWidth, m_fHeight, 0/*0.5f / m_fWidth*/, 0/*0.5f / m_fHeight*/};

				pDevice->SetPixelShaderConstantF( 0, vOmega, 1 );
				pDevice->SetPixelShaderConstantF( 1, vDisp, 1 );
				pDevice->SetPixelShaderConstantF( 2, vAmpl, 1 );
				pDevice->SetPixelShaderConstantF( 3, vSize, 1 );

				CSampler sampler;
				sampler.SetTexture( m_pTexture );
				sampler.SetFilter( m_eFilter );
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_Twirl : public IPixelShader
		{
		public:
			float m_fAngle;
			float m_fDegree;
			float m_fZoom;
			float m_fWidth;
			float m_fHeight;

			D3DCOLOR m_nBackColor;
			IDirect3DTexture9* m_pTexture;
			CSampler::EFilter  m_eFilter;
		
		public:
			PS_Twirl()
			{
				m_fAngle   = 0;
				m_fDegree  = 0;
				m_fZoom    = 1;
				m_fWidth   = 0;
				m_fHeight  = 0;
				
				m_nBackColor = 0;
				m_pTexture = 0;
				m_eFilter  = CSampler::c_nDefault;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x00c7fffe, 0x47554244, 0x00000028, 0x000002e4, 0x00000000, 0x00000001, 0x00000038, 
					0x0000001b, 0x0000003c, 0x00000008, 0x00000244, 0x00000114, 0x705c3a64, 0x77745f73, 0x2e6c7269, 
					0x00747874, 0x00000028, 0xffff0000, 0x00000464, 0xffff0000, 0x0000047c, 0xffff0000, 0x00000494, 
					0xffff0000, 0x000004ac, 0xffff0000, 0x000004c4, 0xffff0000, 0x000004d0, 0x00000028, 0x000004dc, 
					0x00000028, 0x000004f0, 0x0000002a, 0x00000504, 0x0000002a, 0x00000518, 0x0000002a, 0x00000524, 
					0x0000002a, 0x00000530, 0x0000002a, 0x0000053c, 0x0000002a, 0x00000550, 0x0000002a, 0x00000560, 
					0x0000002b, 0x00000570, 0x0000002b, 0x00000584, 0x0000002b, 0x00000590, 0x0000002b, 0x000005a4, 
					0x0000002f, 0x000005b8, 0x0000002f, 0x000005c8, 0x00000030, 0x000005dc, 0x00000030, 0x000005ec, 
					0x00000032, 0x00000600, 0x00000032, 0x00000614, 0x00000034, 0x00000628, 0x00000034, 0x00000638, 
					0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 
					0x00000119, 0x00000124, 0x00000005, 0x00040001, 0x00010001, 0x00000134, 0x0000001a, 0x00010000, 
					0x00030002, 0x74636166, 0xab00726f, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x0000000b, 
					0xffffffff, 0x0000ffff, 0x00676e61, 0x0000000e, 0xffffffff, 0x0000ffff, 0x6e697364, 0xababab00, 
					0x00000012, 0x0000ffff, 0xffffffff, 0x75706e69, 0x61620074, 0xab006573, 0x00030001, 0x00020001, 
					0x00000001, 0x00000000, 0x000001a6, 0x000001ac, 0x00000005, 0x00020001, 0x00010001, 0x000001bc, 
					0x00000004, 0x00010000, 0xffffffff, 0x7074756f, 0xab007475, 0x00000019, 0x00010000, 0x00030002, 
					0xab003170, 0x00000006, 0xffff0000, 0xffffffff, 0x00000007, 0x0001ffff, 0xffffffff, 0x00000017, 
					0xffff0000, 0xffffffff, 0x00000018, 0x0001ffff, 0xffffffff, 0xab003270, 0x00000014, 0xffff0000, 
					0xffffffff, 0x00000016, 0x0001ffff, 0xffffffff, 0x00000000, 0x00000114, 0x0000013c, 0x00000001, 
					0x0000014c, 0x00000000, 0x00000158, 0x00000160, 0x00000001, 0x00000170, 0x00000114, 0x0000017c, 
					0x00000160, 0x00000001, 0x00000180, 0x00000114, 0x0000018c, 0x00000160, 0x00000001, 0x00000194, 
					0x00000114, 0x000001a0, 0x000001c4, 0x00000001, 0x000001d4, 0x00000114, 0x000001e0, 0x0000013c, 
					0x00000001, 0x000001e8, 0x00000114, 0x000001f4, 0x000001ac, 0x00000004, 0x000001f8, 0x00000114, 
					0x00000228, 0x000001ac, 0x00000002, 0x0000022c, 0x7263694d, 0x666f736f, 0x52282074, 0x33442029, 
					0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 0x34303932, 
					0xababab00, 0x004ffffe, 0x42415443, 0x0000001c, 0x00000105, 0xffff0200, 0x00000006, 0x0000001c, 
					0x00000101, 0x000000fe, 0x00000094, 0x00000003, 0x00000001, 0x0000009c, 0x00000000, 0x000000ac, 
					0x00000002, 0x00000001, 0x000000b4, 0x000000c4, 0x000000d4, 0x00010002, 0x00000001, 0x000000b4, 
					0x000000c4, 0x000000db, 0x00020002, 0x00000001, 0x000000b4, 0x000000c4, 0x000000e2, 0x00030002, 
					0x00000001, 0x000000e8, 0x000000c4, 0x000000f8, 0x00040002, 0x00000001, 0x000000e8, 0x000000c4, 
					0x65736142, 0x00786554, 0x000c0004, 0x00010001, 0x00000001, 0x00000000, 0x6c676e61, 0xabab0065, 
					0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
					0x72676564, 0x66006565, 0x6f746361, 0x69730072, 0x0031657a, 0x00030001, 0x00040001, 0x00000001, 
					0x00000000, 0x657a6973, 0x73700032, 0x305f325f, 0x63694d00, 0x6f736f72, 0x28207466, 0x44202952, 
					0x39584433, 0x61685320, 0x20726564, 0x706d6f43, 0x72656c69, 0x302e3520, 0x30302e34, 0x3039322e, 
					0xabab0034, 0x05000051, 0xa00f0005, 0x00000000, 0x3f800000, 0x3e22f983, 0x3f000000, 0x05000051, 
					0xa00f0006, 0x40c90fdb, 0xc0490fdb, 0x00000000, 0x00000000, 0x05000051, 0xa00f0007, 0xb5d00d01, 
					0xb7b60b61, 0x3b2aaaab, 0x39888889, 0x05000051, 0xa00f0008, 0xbcaaaaab, 0xbe000000, 0x3f800000, 
					0x3f000000, 0x0200001f, 0x80000000, 0xb0030000, 0x0200001f, 0x90000000, 0xa00f0800, 0x04000004, 
					0x80010002, 0xb0000000, 0xa0000003, 0xa0aa0003, 0x04000004, 0x80020002, 0xb0550000, 0xa0550003, 
					0xa0ff0003, 0x0400005a, 0x80080000, 0x80e40002, 0x80e40002, 0xa0000005, 0x02000007, 0x80080000, 
					0x80ff0000, 0x02000006, 0x80080000, 0x80ff0000, 0x02000001, 0x80080001, 0xa0000002, 0x04000004, 
					0x80080001, 0x80ff0000, 0x80ff0001, 0xa0550005, 0x03000020, 0x80080000, 0x80ff0001, 0xa0000001, 
					0x03000005, 0x80080000, 0x80ff0000, 0xa0000000, 0x04000004, 0x80080000, 0x80ff0000, 0xa0aa0005, 
					0xa0ff0005, 0x02000013, 0x80080000, 0x80ff0000, 0x04000004, 0x80080001, 0x80ff0000, 0xa0000006, 
					0xa0550006, 0x04000025, 0x80030000, 0x80ff0001, 0xa0e40007, 0xa0e40008, 0x03000005, 0x80080000, 
					0x80550002, 0x80550000, 0x04000004, 0x80010001, 0x80000002, 0x80000000, 0x81ff0000, 0x03000005, 
					0x80080000, 0x80550002, 0x80000000, 0x04000004, 0x80020001, 0x80000002, 0x80550000, 0x80ff0000, 
					0x04000004, 0x80010000, 0x80000001, 0xa0000004, 0xa0aa0004, 0x04000004, 0x80020000, 0x80550001, 
					0xa0550004, 0xa0ff0004, 0x03000042, 0x800f0000, 0x80e40000, 0xa0e40800, 0x02000001, 0x800f0800, 
					0x80e40000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float angle  = m_fAngle;
				float degree = m_fDegree;
				float zoom   = m_fZoom < 0 ? (min(m_fZoom, -0.0001f)) : (max(m_fZoom, 0.0001f));
				float w      = m_fWidth;
				float h      = m_fHeight;
				float factor = float(-2 / sqrt(double(w * w + h * h)));

				float vAngle[4]  = {angle,  0, 0, 0};
				float vDegree[4] = {degree, 0, 0, 0};
				float vFactor[4] = {factor, 0, 0, 0};
				float vSize1[4]  = {w, h, -w/2, -h/2};
				float vSize2[4]  = {1/(zoom * w), 1/(zoom * h), (0/w + 1)/2, (0/h + 1)/2};

				pDevice->SetPixelShaderConstantF( 0, vAngle, 1 );
				pDevice->SetPixelShaderConstantF( 1, vDegree, 1 );
				pDevice->SetPixelShaderConstantF( 2, vFactor, 1 );
				pDevice->SetPixelShaderConstantF( 3, vSize1, 1 );
				pDevice->SetPixelShaderConstantF( 4, vSize2, 1 );

				CSampler sampler;
				sampler.SetAddressBorder( m_nBackColor );
				sampler.SetTexture( m_pTexture );
				sampler.SetFilter( m_eFilter );
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_Zoom : public IPixelShader
		{
		public:
			float m_fZoomX;
			float m_fZoomY;
			float m_fDispX;
			float m_fDispY;
			float m_fWidth;
			float m_fHeight;
			
			D3DCOLOR m_nBackColor;
			IDirect3DTexture9* m_pTexture;
			CSampler::EFilter  m_eFilter;
			BOOL m_bResample;
		
		public:
			PS_Zoom()
			{
				m_fZoomX   = 1;
				m_fZoomY   = 1;
				m_fDispX   = 0;
				m_fDispY   = 0;
				m_fWidth   = 1;
				m_fHeight  = 1;

				m_nBackColor = 0;
				m_pTexture = 0;
				m_eFilter  = CSampler::c_nDefault;
				m_bResample = FALSE;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x006afffe, 0x47554244, 0x00000028, 0x00000170, 0x00000000, 0x00000001, 0x00000038, 
					0x00000006, 0x0000003c, 0x00000004, 0x00000120, 0x0000006c, 0x705c3a64, 0x6f7a5f73, 0x742e6d6f, 
					0xab007478, 0x00000028, 0xffff0000, 0x00000274, 0xffff0000, 0x00000280, 0x00000024, 0x0000028c, 
					0x00000024, 0x000002a0, 0x00000026, 0x000002b4, 0x00000026, 0x000002c4, 0x6e69614d, 0x66696400, 
					0x65737566, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x00000071, 0x0000007c, 
					0x00000005, 0x00040001, 0x00010001, 0x0000008c, 0x00000005, 0x00010000, 0x00030002, 0x75706e69, 
					0x61620074, 0xab006573, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x000000b6, 0x000000bc, 
					0x00000005, 0x00020001, 0x00010001, 0x000000cc, 0x00000000, 0x00010000, 0xffffffff, 0x7074756f, 
					0xab007475, 0x00000004, 0x00010000, 0x00030002, 0x00736f70, 0x00000002, 0xffff0000, 0xffffffff, 
					0x00000003, 0x0001ffff, 0xffffffff, 0x00000000, 0x0000006c, 0x00000094, 0x00000001, 0x000000a4, 
					0x0000006c, 0x000000b0, 0x000000d4, 0x00000001, 0x000000e4, 0x0000006c, 0x000000f0, 0x00000094, 
					0x00000001, 0x000000f8, 0x0000006c, 0x00000104, 0x000000bc, 0x00000002, 0x00000108, 0x7263694d, 
					0x666f736f, 0x52282074, 0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 
					0x34302e35, 0x2e30302e, 0x34303932, 0xababab00, 0x0030fffe, 0x42415443, 0x0000001c, 0x0000008b, 
					0xffff0200, 0x00000002, 0x0000001c, 0x00000101, 0x00000084, 0x00000044, 0x00000003, 0x00000001, 
					0x0000004c, 0x00000000, 0x0000005c, 0x00000002, 0x00000001, 0x00000064, 0x00000074, 0x65736142, 
					0x00786554, 0x000c0004, 0x00010001, 0x00000001, 0x00000000, 0x6d6f6f7a, 0xababab00, 0x00030001, 
					0x00040001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x325f7370, 
					0x4d00305f, 0x6f726369, 0x74666f73, 0x29522820, 0x44334420, 0x53203958, 0x65646168, 0x6f432072, 
					0x6c69706d, 0x35207265, 0x2e34302e, 0x322e3030, 0x00343039, 0x0200001f, 0x80000000, 0xb0030000, 
					0x0200001f, 0x90000000, 0xa00f0800, 0x04000004, 0x80010000, 0xb0000000, 0xa0000000, 0xa0aa0000, 
					0x04000004, 0x80020000, 0xb0550000, 0xa0550000, 0xa0ff0000, 0x03000042, 0x800f0000, 0x80e40000, 
					0xa0e40800, 0x02000001, 0x800f0800, 0x80e40000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float zx = CheckScale(m_fZoomX);
				float zy = CheckScale(m_fZoomY);

				float dx = 0.5f / m_fWidth;
				float dy = 0.5f / m_fHeight;

				dx *= (zx > 1) ? (zx - 1) : ((zx < -1) ? (zx + 1) : 0); 
				dy *= (zy > 1) ? (zy - 1) : ((zy < -1) ? (zy + 1) : 0);

				if( m_fDispX < 0 ) dx = -dx;
				if( m_fDispY < 0 ) dy = -dy;

				float vZoom[4] = {
					1 / zx, 
					1 / zy,
					(m_fWidth  * (zx - 1) - m_fDispX * 2) / (2 * zx * m_fWidth)  + dx,
					(m_fHeight * (zy - 1) - m_fDispY * 2) / (2 * zy * m_fHeight) + dy,
				};

				pDevice->SetPixelShaderConstantF( 0, vZoom, 1 );

				CSampler sampler;
				
				if( m_bResample )
				{
					sampler.SetAddressWrap();
					sampler.SetFilterPoint();
				}
				else
				{
					sampler.SetAddressBorder( m_nBackColor );
					sampler.SetFilter( m_eFilter );
				}

				sampler.SetTexture( m_pTexture );
				sampler.SetToDevice( pDevice, 0 );

				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_LinearTransform : public IPixelShader
		{
		public:
			float m_matrix[3][2];

			D3DCOLOR m_nBackColor;
			IDirect3DTexture9* m_pTexture;
			CSampler::EFilter  m_eFilter;
		
		public:
			PS_LinearTransform()
			{
				m_matrix[0][0] = 1; 
				m_matrix[0][1] = 0;
				m_matrix[1][0] = 0;
				m_matrix[1][1] = 1;
				m_matrix[2][0] = 0;
				m_matrix[2][1] = 0;

				m_nBackColor = 0;
				m_pTexture = 0;
				m_eFilter  = CSampler::c_nDefault;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x0074fffe, 0x47554244, 0x00000028, 0x00000198, 0x00000000, 0x00000001, 0x00000040, 
					0x0000000a, 0x00000044, 0x00000004, 0x00000148, 0x00000094, 0x705c3a64, 0x756d5f73, 0x616d5f6c, 
					0x78697274, 0x2e327833, 0x00747874, 0x00000028, 0xffff0000, 0x000002b0, 0xffff0000, 0x000002bc, 
					0x00000026, 0x000002c8, 0x00000026, 0x000002d8, 0x00000027, 0x000002ec, 0x00000026, 0x000002fc, 
					0x00000027, 0x0000030c, 0x00000027, 0x00000320, 0x00000029, 0x00000330, 0x00000029, 0x00000340, 
					0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 
					0x00000099, 0x000000a4, 0x00000005, 0x00040001, 0x00010001, 0x000000b4, 0x00000009, 0x00010000, 
					0x00030002, 0x75706e69, 0x61620074, 0xab006573, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 
					0x000000de, 0x000000e4, 0x00000005, 0x00020001, 0x00010001, 0x000000f4, 0x00000000, 0x00010000, 
					0xffffffff, 0x7074756f, 0xab007475, 0x00000008, 0x00010000, 0x00030002, 0x00736f70, 0x00000005, 
					0xffff0000, 0xffffffff, 0x00000007, 0x0001ffff, 0xffffffff, 0x00000000, 0x00000094, 0x000000bc, 
					0x00000001, 0x000000cc, 0x00000094, 0x000000d8, 0x000000fc, 0x00000001, 0x0000010c, 0x00000094, 
					0x00000118, 0x000000bc, 0x00000001, 0x00000120, 0x00000094, 0x0000012c, 0x000000e4, 0x00000002, 
					0x00000130, 0x7263694d, 0x666f736f, 0x52282074, 0x33442029, 0x20395844, 0x64616853, 0x43207265, 
					0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 0x34303932, 0xababab00, 0x0035fffe, 0x42415443, 
					0x0000001c, 0x0000009e, 0xffff0200, 0x00000003, 0x0000001c, 0x00000101, 0x00000097, 0x00000058, 
					0x00000003, 0x00000001, 0x00000060, 0x00000000, 0x00000070, 0x00000002, 0x00000001, 0x00000074, 
					0x00000084, 0x00000094, 0x00010002, 0x00000001, 0x00000074, 0x00000084, 0x65736142, 0x00786554, 
					0x000c0004, 0x00010001, 0x00000001, 0x00000000, 0xab00786d, 0x00030001, 0x00030001, 0x00000001, 
					0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x7000796d, 0x5f325f73, 0x694d0030, 
					0x736f7263, 0x2074666f, 0x20295228, 0x58443344, 0x68532039, 0x72656461, 0x6d6f4320, 0x656c6970, 
					0x2e352072, 0x302e3430, 0x39322e30, 0xab003430, 0x0200001f, 0x80000000, 0xb0030000, 0x0200001f, 
					0x90000000, 0xa00f0800, 0x03000005, 0x80080000, 0xb0550000, 0xa0550000, 0x04000004, 0x80080001, 
					0xb0000000, 0xa0000000, 0x80ff0000, 0x03000005, 0x80080000, 0xb0550000, 0xa0550001, 0x03000002, 
					0x80010000, 0x80ff0001, 0xa0aa0000, 0x04000004, 0x80080000, 0xb0000000, 0xa0000001, 0x80ff0000, 
					0x03000002, 0x80020000, 0x80ff0000, 0xa0aa0001, 0x03000042, 0x800f0000, 0x80e40000, 0xa0e40800, 
					0x02000001, 0x800f0800, 0x80e40000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float mx[4] = {m_matrix[0][0], m_matrix[1][0], m_matrix[2][0], 0};
				float my[4] = {m_matrix[0][1], m_matrix[1][1], m_matrix[2][1], 0};

				pDevice->SetPixelShaderConstantF( 0, mx, 1 );
				pDevice->SetPixelShaderConstantF( 1, my, 1 );

				CSampler sampler;
				sampler.SetAddressBorder( m_nBackColor );
				sampler.SetTexture( m_pTexture );
				sampler.SetFilter( m_eFilter );
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_Sphere : public IPixelShader
		{
		public:
			float m_fCenterX;
			float m_fCenterY;
			float m_fRadiusX;
			float m_fRadiusY;
			float m_fDegree;
			float m_fWidth;
			float m_fHeight;

			D3DCOLOR m_nBackColor;
			IDirect3DTexture9* m_pTexture;
			CSampler::EFilter  m_eFilter;
		
		public:
			PS_Sphere()
			{
				m_fCenterX = 0;
				m_fCenterY = 0;
				m_fRadiusX = 0;
				m_fRadiusY = 0;
				m_fDegree  = 1;
				m_fWidth   = 1;
				m_fHeight  = 1;
				m_nBackColor = 0;
				m_pTexture = 0;
				m_eFilter  = CSampler::c_nDefault;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x0091fffe, 0x47554244, 0x00000028, 0x0000020c, 0x00000000, 0x00000001, 0x0000003c, 
					0x0000000f, 0x00000040, 0x00000005, 0x000001a8, 0x000000b8, 0x705c3a64, 0x70735f73, 0x65726568, 
					0x7478742e, 0xababab00, 0x00000028, 0xffff0000, 0x00000358, 0xffff0000, 0x00000370, 0xffff0000, 
					0x0000037c, 0x00000026, 0x00000388, 0x00000028, 0x00000398, 0x00000028, 0x000003a8, 0x00000028, 
					0x000003bc, 0x00000028, 0x000003c8, 0x0000002c, 0x000003d4, 0x0000002a, 0x000003e4, 0x0000002e, 
					0x000003f4, 0x0000002a, 0x00000404, 0x00000031, 0x00000418, 0x00000031, 0x00000428, 0x00000031, 
					0x00000438, 0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 
					0x00000000, 0x000000bd, 0x000000c8, 0x00000005, 0x00040001, 0x00010001, 0x000000d8, 0x0000000e, 
					0x00010000, 0x00030002, 0x74636166, 0xab00726f, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 
					0x00000007, 0xffffffff, 0x0000ffff, 0x00000008, 0xffffffff, 0x0000ffff, 0x75706e69, 0x61620074, 
					0xab006573, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x00000132, 0x00000138, 0x00000005, 
					0x00020001, 0x00010001, 0x00000148, 0x00000001, 0x00010000, 0xffffffff, 0x7074756f, 0xab007475, 
					0x0000000d, 0x00010000, 0x00030002, 0x00736f70, 0x00000003, 0x00010000, 0xffffffff, 0x0000000a, 
					0x00010000, 0xffffffff, 0x0000000b, 0x00010000, 0xffffffff, 0x00000000, 0x000000b8, 0x000000e0, 
					0x00000001, 0x000000f0, 0x000000b8, 0x000000fc, 0x00000104, 0x00000002, 0x00000114, 0x000000b8, 
					0x0000012c, 0x00000150, 0x00000001, 0x00000160, 0x000000b8, 0x0000016c, 0x000000e0, 0x00000001, 
					0x00000174, 0x000000b8, 0x00000180, 0x00000138, 0x00000003, 0x00000184, 0x7263694d, 0x666f736f, 
					0x52282074, 0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 
					0x2e30302e, 0x34303932, 0xababab00, 0x0042fffe, 0x42415443, 0x0000001c, 0x000000d2, 0xffff0200, 
					0x00000004, 0x0000001c, 0x00000101, 0x000000cb, 0x0000006c, 0x00000003, 0x00000001, 0x00000074, 
					0x00000000, 0x00000084, 0x00000002, 0x00000001, 0x0000008c, 0x0000009c, 0x000000ac, 0x00020002, 
					0x00000001, 0x000000b4, 0x0000009c, 0x000000c4, 0x00010002, 0x00000001, 0x0000008c, 0x0000009c, 
					0x65736142, 0x00786554, 0x000c0004, 0x00010001, 0x00000001, 0x00000000, 0x746e6563, 0xab007265, 
					0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
					0x72676564, 0xab006565, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x69646172, 0x70007375, 
					0x5f325f73, 0x694d0030, 0x736f7263, 0x2074666f, 0x20295228, 0x58443344, 0x68532039, 0x72656461, 
					0x6d6f4320, 0x656c6970, 0x2e352072, 0x302e3430, 0x39322e30, 0xab003430, 0x05000051, 0xa00f0003, 
					0x00000000, 0xbf800000, 0x00000000, 0x00000000, 0x0200001f, 0x80000000, 0xb0030000, 0x0200001f, 
					0x90000000, 0xa00f0800, 0x03000002, 0x80030000, 0xb0e40000, 0xa1e40000, 0x03000005, 0x80030001, 
					0x80e40000, 0xa0e40001, 0x0400005a, 0x80080000, 0x80e40001, 0x80e40001, 0xa0000003, 0x02000007, 
					0x80080000, 0x80ff0000, 0x02000006, 0x80080000, 0x80ff0000, 0x03000020, 0x80080001, 0x80ff0000, 
					0xa0000002, 0x03000002, 0x80080000, 0x80ff0000, 0xa0550003, 0x03000005, 0x80030001, 0x80e40000, 
					0x80ff0001, 0x04000058, 0x80030000, 0x80ff0000, 0x80e40000, 0x80e40001, 0x03000002, 0x80030000, 
					0x80e40000, 0xa0e40000, 0x03000042, 0x800f0000, 0x80e40000, 0xa0e40800, 0x02000001, 0x800f0800, 
					0x80e40000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float fw = 1.0f / m_fWidth;
				float fh = 1.0f / m_fHeight;
				float rx = max(0.0001f, m_fRadiusX);
				float ry = max(0.0001f, m_fRadiusY);

				float center[4] = {m_fCenterX * fw, m_fCenterY * fh, 0, 0};
				float radius[4] = {1.0f / (rx * fw), 1.0f / (ry * fh), 0, 0};
				float degree[4] = {m_fDegree, 0, 0, 0};

				pDevice->SetPixelShaderConstantF( 0, center, 1 );
				pDevice->SetPixelShaderConstantF( 1, radius, 1 );
				pDevice->SetPixelShaderConstantF( 2, degree, 1 );

				CSampler sampler;
				sampler.SetAddressBorder( m_nBackColor );
				sampler.SetTexture( m_pTexture );
				sampler.SetFilter( m_eFilter );
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_CylinderVer : public IPixelShader
		{
		public:
			float m_fCenter;
			float m_fRadius;
			float m_fDegree;
			float m_fWidth;

			D3DCOLOR m_nBackColor;
			IDirect3DTexture9* m_pTexture;
			CSampler::EFilter  m_eFilter;
		
		public:
			PS_CylinderVer()
			{
				m_fCenter  = 0;
				m_fRadius  = 0;
				m_fDegree  = 1;
				m_fWidth   = 1;
				m_nBackColor = 0;
				m_pTexture = 0;
				m_eFilter  = CSampler::c_nDefault;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x0096fffe, 0x47554244, 0x00000028, 0x00000220, 0x00000000, 0x00000001, 0x00000040, 
					0x0000000e, 0x00000044, 0x00000005, 0x000001bc, 0x000000b4, 0x705c3a64, 0x79635f73, 0x646e696c, 
					0x765f7265, 0x742e7265, 0xab007478, 0x00000028, 0xffff0000, 0x0000035c, 0xffff0000, 0x00000374, 
					0xffff0000, 0x00000380, 0x00000026, 0x0000038c, 0x00000028, 0x0000039c, 0x0000002a, 0x000003ac, 
					0x0000002c, 0x000003bc, 0x0000002e, 0x000003d0, 0x00000030, 0x000003e0, 0x0000002c, 0x000003f0, 
					0x00000033, 0x00000404, 0x00000036, 0x00000414, 0x00000036, 0x00000420, 0x00000036, 0x00000430, 
					0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 
					0x000000b9, 0x000000c4, 0x00000005, 0x00040001, 0x00010001, 0x000000d4, 0x0000000d, 0x00010000, 
					0x00030002, 0x74636166, 0xab00726f, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x00000004, 
					0xffffffff, 0x0000ffff, 0x00000005, 0xffffffff, 0x0000ffff, 0x00000007, 0xffffffff, 0x0000ffff, 
					0x75706e69, 0x61620074, 0xab006573, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x0000013a, 
					0x00000140, 0x00000005, 0x00020001, 0x00010001, 0x00000150, 0x00000001, 0x00010000, 0xffffffff, 
					0x0000000a, 0xffff0000, 0xffffffff, 0x7074756f, 0xab007475, 0x0000000c, 0x00010000, 0x00030002, 
					0xabab0078, 0x00000003, 0xffffffff, 0x0000ffff, 0x00000008, 0xffffffff, 0x0000ffff, 0x00000009, 
					0xffffffff, 0x0000ffff, 0x00000000, 0x000000b4, 0x000000dc, 0x00000001, 0x000000ec, 0x000000b4, 
					0x000000f8, 0x00000100, 0x00000003, 0x00000110, 0x000000b4, 0x00000134, 0x00000158, 0x00000002, 
					0x00000168, 0x000000b4, 0x00000180, 0x000000dc, 0x00000001, 0x00000188, 0x000000b4, 0x00000194, 
					0x00000100, 0x00000003, 0x00000198, 0x7263694d, 0x666f736f, 0x52282074, 0x33442029, 0x20395844, 
					0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 0x34303932, 0xababab00, 
					0x003efffe, 0x42415443, 0x0000001c, 0x000000c1, 0xffff0200, 0x00000004, 0x0000001c, 0x00000101, 
					0x000000ba, 0x0000006c, 0x00000003, 0x00000001, 0x00000074, 0x00000000, 0x00000084, 0x00000002, 
					0x00000001, 0x0000008c, 0x0000009c, 0x000000ac, 0x00020002, 0x00000001, 0x0000008c, 0x0000009c, 
					0x000000b3, 0x00010002, 0x00000001, 0x0000008c, 0x0000009c, 0x65736142, 0x00786554, 0x000c0004, 
					0x00010001, 0x00000001, 0x00000000, 0x746e6563, 0xab007265, 0x00030000, 0x00010001, 0x00000001, 
					0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x72676564, 0x72006565, 0x75696461, 
					0x73700073, 0x305f325f, 0x63694d00, 0x6f736f72, 0x28207466, 0x44202952, 0x39584433, 0x61685320, 
					0x20726564, 0x706d6f43, 0x72656c69, 0x302e3520, 0x30302e34, 0x3039322e, 0xabab0034, 0x05000051, 
					0xa00f0003, 0xbf800000, 0x00000000, 0x00000000, 0x00000000, 0x0200001f, 0x80000000, 0xb0030000, 
					0x0200001f, 0x90000000, 0xa00f0800, 0x03000002, 0x80080001, 0xb0000000, 0xa1000000, 0x03000005, 
					0x80080000, 0x80ff0001, 0xa0000001, 0x03000005, 0x80080003, 0x80ff0000, 0x80ff0000, 0x04000004, 
					0x80080000, 0x80ff0000, 0x80ff0000, 0xa0000003, 0x03000020, 0x80080002, 0x80ff0003, 0xa0000002, 
					0x03000005, 0x80080002, 0x80ff0001, 0x80ff0002, 0x04000058, 0x80080000, 0x80ff0000, 0x80ff0001, 
					0x80ff0002, 0x03000002, 0x80010000, 0x80ff0000, 0xa0000000, 0x02000001, 0x80020000, 0xb0550000, 
					0x03000042, 0x800f0000, 0x80e40000, 0xa0e40800, 0x02000001, 0x800f0800, 0x80e40000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float center = m_fCenter / m_fWidth;
				float radius = 1.0f / (max(0.0001f, m_fRadius) / m_fWidth);

				float vCenter[4] = {center, 0, 0, 0};
				float vRadius[4] = {radius, 0, 0, 0};
				float vDegree[4] = {m_fDegree, 0, 0, 0};

				pDevice->SetPixelShaderConstantF( 0, vCenter, 1 );
				pDevice->SetPixelShaderConstantF( 1, vRadius, 1 );
				pDevice->SetPixelShaderConstantF( 2, vDegree, 1 );

				CSampler sampler;
				sampler.SetAddressBorder( m_nBackColor );
				sampler.SetTexture( m_pTexture );
				sampler.SetFilter( m_eFilter );
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_CylinderHor : public IPixelShader
		{
		public:
			float m_fCenter;
			float m_fRadius;
			float m_fDegree;
			float m_fHeight;

			D3DCOLOR m_nBackColor;
			IDirect3DTexture9* m_pTexture;
			CSampler::EFilter  m_eFilter;
		
		public:
			PS_CylinderHor()
			{
				m_fCenter  = 0;
				m_fRadius  = 0;
				m_fDegree  = 1;
				m_fHeight  = 1;
				m_nBackColor = 0;
				m_pTexture = 0;
				m_eFilter  = CSampler::c_nDefault;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x0096fffe, 0x47554244, 0x00000028, 0x00000220, 0x00000000, 0x00000001, 0x00000040, 
					0x0000000e, 0x00000044, 0x00000005, 0x000001bc, 0x000000b4, 0x705c3a64, 0x79635f73, 0x646e696c, 
					0x685f7265, 0x742e726f, 0xab007478, 0x00000028, 0xffff0000, 0x0000035c, 0xffff0000, 0x00000374, 
					0xffff0000, 0x00000380, 0x00000026, 0x0000038c, 0x00000028, 0x0000039c, 0x0000002a, 0x000003ac, 
					0x0000002c, 0x000003bc, 0x0000002e, 0x000003d0, 0x00000030, 0x000003e0, 0x0000002c, 0x000003f0, 
					0x00000033, 0x00000404, 0x00000036, 0x00000414, 0x00000036, 0x00000420, 0x00000036, 0x00000430, 
					0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 
					0x000000b9, 0x000000c4, 0x00000005, 0x00040001, 0x00010001, 0x000000d4, 0x0000000d, 0x00010000, 
					0x00030002, 0x74636166, 0xab00726f, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x00000004, 
					0xffffffff, 0x0000ffff, 0x00000005, 0xffffffff, 0x0000ffff, 0x00000007, 0xffffffff, 0x0000ffff, 
					0x75706e69, 0x61620074, 0xab006573, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x0000013a, 
					0x00000140, 0x00000005, 0x00020001, 0x00010001, 0x00000150, 0x00000001, 0x00010000, 0xffffffff, 
					0x0000000a, 0x0001ffff, 0xffffffff, 0x7074756f, 0xab007475, 0x0000000c, 0x00010000, 0x00030002, 
					0xabab0079, 0x00000003, 0xffffffff, 0x0000ffff, 0x00000008, 0xffffffff, 0x0000ffff, 0x00000009, 
					0xffffffff, 0x0000ffff, 0x00000000, 0x000000b4, 0x000000dc, 0x00000001, 0x000000ec, 0x000000b4, 
					0x000000f8, 0x00000100, 0x00000003, 0x00000110, 0x000000b4, 0x00000134, 0x00000158, 0x00000002, 
					0x00000168, 0x000000b4, 0x00000180, 0x000000dc, 0x00000001, 0x00000188, 0x000000b4, 0x00000194, 
					0x00000100, 0x00000003, 0x00000198, 0x7263694d, 0x666f736f, 0x52282074, 0x33442029, 0x20395844, 
					0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 0x34303932, 0xababab00, 
					0x003efffe, 0x42415443, 0x0000001c, 0x000000c1, 0xffff0200, 0x00000004, 0x0000001c, 0x00000101, 
					0x000000ba, 0x0000006c, 0x00000003, 0x00000001, 0x00000074, 0x00000000, 0x00000084, 0x00000002, 
					0x00000001, 0x0000008c, 0x0000009c, 0x000000ac, 0x00020002, 0x00000001, 0x0000008c, 0x0000009c, 
					0x000000b3, 0x00010002, 0x00000001, 0x0000008c, 0x0000009c, 0x65736142, 0x00786554, 0x000c0004, 
					0x00010001, 0x00000001, 0x00000000, 0x746e6563, 0xab007265, 0x00030000, 0x00010001, 0x00000001, 
					0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x72676564, 0x72006565, 0x75696461, 
					0x73700073, 0x305f325f, 0x63694d00, 0x6f736f72, 0x28207466, 0x44202952, 0x39584433, 0x61685320, 
					0x20726564, 0x706d6f43, 0x72656c69, 0x302e3520, 0x30302e34, 0x3039322e, 0xabab0034, 0x05000051, 
					0xa00f0003, 0xbf800000, 0x00000000, 0x00000000, 0x00000000, 0x0200001f, 0x80000000, 0xb0030000, 
					0x0200001f, 0x90000000, 0xa00f0800, 0x03000002, 0x80080001, 0xb0550000, 0xa1000000, 0x03000005, 
					0x80080000, 0x80ff0001, 0xa0000001, 0x03000005, 0x80080003, 0x80ff0000, 0x80ff0000, 0x04000004, 
					0x80080000, 0x80ff0000, 0x80ff0000, 0xa0000003, 0x03000020, 0x80080002, 0x80ff0003, 0xa0000002, 
					0x03000005, 0x80080002, 0x80ff0001, 0x80ff0002, 0x04000058, 0x80080000, 0x80ff0000, 0x80ff0001, 
					0x80ff0002, 0x03000002, 0x80020000, 0x80ff0000, 0xa0000000, 0x02000001, 0x80010000, 0xb0000000, 
					0x03000042, 0x800f0000, 0x80e40000, 0xa0e40800, 0x02000001, 0x800f0800, 0x80e40000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float center = m_fCenter / m_fHeight;
				float radius = 1.0f / (max(0.0001f, m_fRadius) / m_fHeight);

				float vCenter[4] = {center, 0, 0, 0};
				float vRadius[4] = {radius, 0, 0, 0};
				float vDegree[4] = {m_fDegree, 0, 0, 0};

				pDevice->SetPixelShaderConstantF( 0, vCenter, 1 );
				pDevice->SetPixelShaderConstantF( 1, vRadius, 1 );
				pDevice->SetPixelShaderConstantF( 2, vDegree, 1 );

				CSampler sampler;
				sampler.SetAddressBorder( m_nBackColor );
				sampler.SetTexture( m_pTexture );
				sampler.SetFilter( m_eFilter );
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_WaveVer : public IPixelShader
		{
		public:
			float m_fAmpl;
			float m_fPhase;
			float m_fDisp;
			float m_fWidth;

			D3DCOLOR m_nBackColor;
			IDirect3DTexture9* m_pTexture;
			CSampler::EFilter  m_eFilter;
		
		public:
			PS_WaveVer()
			{
				m_fAmpl    = 0;
				m_fPhase   = 0;
				m_fDisp    = 1;
				m_fWidth   = 1;
				m_pTexture = 0;
				m_eFilter  = CSampler::c_nDefault;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x00c4fffe, 0x47554244, 0x00000028, 0x000002d8, 0x00000000, 0x00000001, 0x0000003c, 
					0x0000001a, 0x00000040, 0x00000007, 0x0000024c, 0x00000110, 0x705c3a64, 0x61775f73, 0x765f6576, 
					0x742e7265, 0xab007478, 0x00000028, 0xffff0000, 0x00000410, 0xffff0000, 0x00000428, 0xffff0000, 
					0x00000440, 0xffff0000, 0x00000458, 0xffff0000, 0x00000470, 0xffff0000, 0x0000047c, 0x00000026, 
					0x00000488, 0x00000026, 0x00000494, 0x00000026, 0x000004a8, 0x00000026, 0x000004bc, 0x00000026, 
					0x000004c8, 0x00000026, 0x000004dc, 0x00000026, 0x000004f0, 0x00000028, 0x00000504, 0x0000002d, 
					0x00000510, 0x0000002e, 0x00000520, 0x0000002b, 0x00000530, 0x0000002b, 0x00000540, 0x00000029, 
					0x00000554, 0x00000033, 0x00000564, 0x00000031, 0x00000574, 0x00000034, 0x00000584, 0x00000031, 
					0x00000594, 0x00000039, 0x000005a8, 0x00000039, 0x000005b4, 0x00000039, 0x000005c4, 0x6e69614d, 
					0x66696400, 0x65737566, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x00000115, 
					0x00000120, 0x00000005, 0x00040001, 0x00010001, 0x00000130, 0x00000019, 0x00010000, 0x00030002, 
					0x73616870, 0xabab0065, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x00000006, 0xffffffff, 
					0x0000ffff, 0x61736261, 0x006c706d, 0x0000000d, 0xffffffff, 0x0000ffff, 0x75706e69, 0x61620074, 
					0xab006573, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x00000192, 0x00000198, 0x00000005, 
					0x00020001, 0x00010001, 0x000001a8, 0x00000004, 0x00010000, 0xffffffff, 0x6478616d, 0x00707369, 
					0x00000012, 0xffffffff, 0x0000ffff, 0x7074756f, 0xab007475, 0x00000018, 0x00010000, 0x00030002, 
					0xabab0078, 0x0000000c, 0xffffffff, 0x0000ffff, 0x0000000e, 0xffffffff, 0x0000ffff, 0x0000000f, 
					0xffffffff, 0x0000ffff, 0x00000011, 0xffffffff, 0x0000ffff, 0x00000013, 0xffffffff, 0x0000ffff, 
					0x00000015, 0xffffffff, 0x0000ffff, 0x00000016, 0xffff0000, 0xffffffff, 0x00000000, 0x00000110, 
					0x00000138, 0x00000001, 0x00000148, 0x00000000, 0x00000154, 0x0000015c, 0x00000001, 0x0000016c, 
					0x00000110, 0x00000178, 0x0000015c, 0x00000001, 0x00000180, 0x00000110, 0x0000018c, 0x000001b0, 
					0x00000001, 0x000001c0, 0x00000110, 0x000001cc, 0x0000015c, 0x00000001, 0x000001d4, 0x00000110, 
					0x000001e0, 0x00000138, 0x00000001, 0x000001e8, 0x00000110, 0x000001f4, 0x0000015c, 0x00000007, 
					0x000001f8, 0x7263694d, 0x666f736f, 0x52282074, 0x33442029, 0x20395844, 0x64616853, 0x43207265, 
					0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 0x34303932, 0xababab00, 0x003dfffe, 0x42415443, 
					0x0000001c, 0x000000be, 0xffff0200, 0x00000004, 0x0000001c, 0x00000101, 0x000000b7, 0x0000006c, 
					0x00000003, 0x00000001, 0x00000074, 0x00000000, 0x00000084, 0x00000002, 0x00000001, 0x0000008c, 
					0x0000009c, 0x000000ac, 0x00020002, 0x00000001, 0x0000008c, 0x0000009c, 0x000000b1, 0x00010002, 
					0x00000001, 0x0000008c, 0x0000009c, 0x65736142, 0x00786554, 0x000c0004, 0x00010001, 0x00000001, 
					0x00000000, 0x6c706d61, 0xababab00, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x00000000, 
					0x00000000, 0x00000000, 0x00000000, 0x70736964, 0x61687000, 0x70006573, 0x5f325f73, 0x694d0030, 
					0x736f7263, 0x2074666f, 0x20295228, 0x58443344, 0x68532039, 0x72656461, 0x6d6f4320, 0x656c6970, 
					0x2e352072, 0x302e3430, 0x39322e30, 0xab003430, 0x05000051, 0xa00f0003, 0x3e22f983, 0x3f000000, 
					0x40c90fdb, 0xc0490fdb, 0x05000051, 0xa00f0004, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 
					0x05000051, 0xa00f0005, 0xb5d00d01, 0xb7b60b61, 0x3b2aaaab, 0x39888889, 0x05000051, 0xa00f0006, 
					0xbcaaaaab, 0xbe000000, 0x3f800000, 0x3f000000, 0x0200001f, 0x80000000, 0xb0030000, 0x0200001f, 
					0x90000000, 0xa00f0800, 0x02000001, 0x80080000, 0xa0000001, 0x04000004, 0x80080000, 0xb0550000, 
					0x80ff0000, 0xa0000002, 0x04000004, 0x80080000, 0x80ff0000, 0xa0000003, 0xa0550003, 0x02000013, 
					0x80080000, 0x80ff0000, 0x04000004, 0x80080001, 0x80ff0000, 0xa0aa0003, 0xa0ff0003, 0x04000025, 
					0x80020000, 0x80ff0001, 0xa0e40005, 0xa0e40006, 0x04000004, 0x80080002, 0xa0000000, 0x80550000, 
					0xb0000000, 0x02000023, 0x80080000, 0xa0000000, 0x03000002, 0x80080001, 0x80ff0002, 0x80ff0000, 
					0x03000005, 0x80080003, 0x80ff0001, 0xa0550003, 0x03000002, 0x80080001, 0x80ff0002, 0x81ff0000, 
					0x04000058, 0x80080001, 0x80ff0001, 0x80ff0002, 0x80ff0003, 0x03000002, 0x80080000, 0x81ff0000, 
					0xa0000004, 0x03000002, 0x80080002, 0x80ff0001, 0x80ff0000, 0x03000002, 0x80080000, 0x81ff0001, 
					0x80ff0000, 0x03000005, 0x80080002, 0x80ff0002, 0xa0550003, 0x04000058, 0x80010000, 0x80ff0000, 
					0x80ff0001, 0x80ff0002, 0x02000001, 0x80020000, 0xb0550000, 0x03000042, 0x800f0000, 0x80e40000, 
					0xa0e40800, 0x02000001, 0x800f0800, 0x80e40000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float vAmpl [4] = {m_fAmpl / m_fWidth,  0, 0, 0};
				float vPhase[4] = {m_fPhase, 0, 0, 0};
				float vDisp [4] = {m_fDisp,  0, 0, 0};

				pDevice->SetPixelShaderConstantF( 0, vAmpl,  1 );
				pDevice->SetPixelShaderConstantF( 1, vPhase, 1 );
				pDevice->SetPixelShaderConstantF( 2, vDisp,  1 );

				CSampler sampler;
				sampler.SetAddressBorder( 0 );
				sampler.SetTexture( m_pTexture );
				sampler.SetFilter( m_eFilter );
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_WaveHor : public IPixelShader
		{
		public:
			float m_fAmpl;
			float m_fPhase;
			float m_fDisp;
			float m_fHeight;

			D3DCOLOR m_nBackColor;
			IDirect3DTexture9* m_pTexture;
			CSampler::EFilter  m_eFilter;
		
		public:
			PS_WaveHor()
			{
				m_fAmpl    = 0;
				m_fPhase   = 0;
				m_fDisp    = 1;
				m_fHeight  = 1;
				m_pTexture = 0;
				m_eFilter  = CSampler::c_nDefault;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x00c4fffe, 0x47554244, 0x00000028, 0x000002d8, 0x00000000, 0x00000001, 0x0000003c, 
					0x0000001a, 0x00000040, 0x00000007, 0x0000024c, 0x00000110, 0x705c3a64, 0x61775f73, 0x685f6576, 
					0x742e726f, 0xab007478, 0x00000028, 0xffff0000, 0x00000410, 0xffff0000, 0x00000428, 0xffff0000, 
					0x00000440, 0xffff0000, 0x00000458, 0xffff0000, 0x00000470, 0xffff0000, 0x0000047c, 0x00000026, 
					0x00000488, 0x00000026, 0x00000494, 0x00000026, 0x000004a8, 0x00000026, 0x000004bc, 0x00000026, 
					0x000004c8, 0x00000026, 0x000004dc, 0x00000026, 0x000004f0, 0x00000028, 0x00000504, 0x0000002d, 
					0x00000510, 0x0000002e, 0x00000520, 0x0000002b, 0x00000530, 0x0000002b, 0x00000540, 0x00000029, 
					0x00000554, 0x00000033, 0x00000564, 0x00000031, 0x00000574, 0x00000034, 0x00000584, 0x00000031, 
					0x00000594, 0x00000039, 0x000005a8, 0x00000039, 0x000005b4, 0x00000039, 0x000005c4, 0x6e69614d, 
					0x66696400, 0x65737566, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x00000115, 
					0x00000120, 0x00000005, 0x00040001, 0x00010001, 0x00000130, 0x00000019, 0x00010000, 0x00030002, 
					0x73616870, 0xabab0065, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x00000006, 0xffffffff, 
					0x0000ffff, 0x61736261, 0x006c706d, 0x0000000d, 0xffffffff, 0x0000ffff, 0x75706e69, 0x61620074, 
					0xab006573, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x00000192, 0x00000198, 0x00000005, 
					0x00020001, 0x00010001, 0x000001a8, 0x00000004, 0x00010000, 0xffffffff, 0x6478616d, 0x00707369, 
					0x00000012, 0xffffffff, 0x0000ffff, 0x7074756f, 0xab007475, 0x00000018, 0x00010000, 0x00030002, 
					0xabab0079, 0x0000000c, 0xffffffff, 0x0000ffff, 0x0000000e, 0xffffffff, 0x0000ffff, 0x0000000f, 
					0xffffffff, 0x0000ffff, 0x00000011, 0xffffffff, 0x0000ffff, 0x00000013, 0xffffffff, 0x0000ffff, 
					0x00000015, 0xffffffff, 0x0000ffff, 0x00000016, 0x0000ffff, 0xffffffff, 0x00000000, 0x00000110, 
					0x00000138, 0x00000001, 0x00000148, 0x00000000, 0x00000154, 0x0000015c, 0x00000001, 0x0000016c, 
					0x00000110, 0x00000178, 0x0000015c, 0x00000001, 0x00000180, 0x00000110, 0x0000018c, 0x000001b0, 
					0x00000001, 0x000001c0, 0x00000110, 0x000001cc, 0x0000015c, 0x00000001, 0x000001d4, 0x00000110, 
					0x000001e0, 0x00000138, 0x00000001, 0x000001e8, 0x00000110, 0x000001f4, 0x0000015c, 0x00000007, 
					0x000001f8, 0x7263694d, 0x666f736f, 0x52282074, 0x33442029, 0x20395844, 0x64616853, 0x43207265, 
					0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 0x34303932, 0xababab00, 0x003dfffe, 0x42415443, 
					0x0000001c, 0x000000be, 0xffff0200, 0x00000004, 0x0000001c, 0x00000101, 0x000000b7, 0x0000006c, 
					0x00000003, 0x00000001, 0x00000074, 0x00000000, 0x00000084, 0x00000002, 0x00000001, 0x0000008c, 
					0x0000009c, 0x000000ac, 0x00020002, 0x00000001, 0x0000008c, 0x0000009c, 0x000000b1, 0x00010002, 
					0x00000001, 0x0000008c, 0x0000009c, 0x65736142, 0x00786554, 0x000c0004, 0x00010001, 0x00000001, 
					0x00000000, 0x6c706d61, 0xababab00, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x00000000, 
					0x00000000, 0x00000000, 0x00000000, 0x70736964, 0x61687000, 0x70006573, 0x5f325f73, 0x694d0030, 
					0x736f7263, 0x2074666f, 0x20295228, 0x58443344, 0x68532039, 0x72656461, 0x6d6f4320, 0x656c6970, 
					0x2e352072, 0x302e3430, 0x39322e30, 0xab003430, 0x05000051, 0xa00f0003, 0x3e22f983, 0x3f000000, 
					0x40c90fdb, 0xc0490fdb, 0x05000051, 0xa00f0004, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 
					0x05000051, 0xa00f0005, 0xb5d00d01, 0xb7b60b61, 0x3b2aaaab, 0x39888889, 0x05000051, 0xa00f0006, 
					0xbcaaaaab, 0xbe000000, 0x3f800000, 0x3f000000, 0x0200001f, 0x80000000, 0xb0030000, 0x0200001f, 
					0x90000000, 0xa00f0800, 0x02000001, 0x80080000, 0xa0000001, 0x04000004, 0x80080000, 0xb0000000, 
					0x80ff0000, 0xa0000002, 0x04000004, 0x80080000, 0x80ff0000, 0xa0000003, 0xa0550003, 0x02000013, 
					0x80080000, 0x80ff0000, 0x04000004, 0x80080001, 0x80ff0000, 0xa0aa0003, 0xa0ff0003, 0x04000025, 
					0x80020000, 0x80ff0001, 0xa0e40005, 0xa0e40006, 0x04000004, 0x80080002, 0xa0000000, 0x80550000, 
					0xb0550000, 0x02000023, 0x80080000, 0xa0000000, 0x03000002, 0x80080001, 0x80ff0002, 0x80ff0000, 
					0x03000005, 0x80080003, 0x80ff0001, 0xa0550003, 0x03000002, 0x80080001, 0x80ff0002, 0x81ff0000, 
					0x04000058, 0x80080001, 0x80ff0001, 0x80ff0002, 0x80ff0003, 0x03000002, 0x80080000, 0x81ff0000, 
					0xa0000004, 0x03000002, 0x80080002, 0x80ff0001, 0x80ff0000, 0x03000002, 0x80080000, 0x81ff0001, 
					0x80ff0000, 0x03000005, 0x80080002, 0x80ff0002, 0xa0550003, 0x04000058, 0x80020000, 0x80ff0000, 
					0x80ff0001, 0x80ff0002, 0x02000001, 0x80010000, 0xb0000000, 0x03000042, 0x800f0000, 0x80e40000, 
					0xa0e40800, 0x02000001, 0x800f0800, 0x80e40000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float vAmpl [4] = {m_fAmpl / m_fHeight,  0, 0, 0};
				float vPhase[4] = {m_fPhase, 0, 0, 0};
				float vDisp [4] = {m_fDisp,  0, 0, 0};

				pDevice->SetPixelShaderConstantF( 0, vAmpl,  1 );
				pDevice->SetPixelShaderConstantF( 1, vPhase, 1 );
				pDevice->SetPixelShaderConstantF( 2, vDisp,  1 );

				CSampler sampler;
				sampler.SetAddressBorder( 0 );
				sampler.SetTexture( m_pTexture );
				sampler.SetFilter( m_eFilter );
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_HLS : public IPixelShader
		{
		public:
			float m_fShiftHue;
			float m_fShiftSat;

			IDirect3DTexture9* m_pTexture;
		
		public:
			PS_HLS()
			{
				m_pTexture = 0;
				m_fShiftHue = 0;
				m_fShiftSat = 0;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x01a0fffe, 0x47554244, 0x00000028, 0x00000648, 0x00000000, 0x00000001, 0x00000038, 
					0x00000045, 0x0000003c, 0x0000000d, 0x00000544, 0x00000264, 0x705c3a64, 0x6c685f73, 0x78742e73, 
					0xabab0074, 0x00000028, 0xffff0000, 0x00000768, 0xffff0000, 0x00000780, 0xffff0000, 0x00000798, 
					0xffff0000, 0x000007b0, 0xffff0000, 0x000007c8, 0xffff0000, 0x000007e0, 0xffff0000, 0x000007ec, 
					0x00000025, 0x000007f8, 0x0000002c, 0x00000808, 0x0000002b, 0x00000818, 0x0000002b, 0x00000828, 
					0x0000002c, 0x00000838, 0x0000003d, 0x00000848, 0x0000002d, 0x00000858, 0x0000002b, 0x00000868, 
					0x00000038, 0x00000878, 0x00000034, 0x00000884, 0x00000038, 0x00000894, 0x00000035, 0x000008a4, 
					0x0000002e, 0x000008b4, 0x0000003b, 0x000008c4, 0x0000003e, 0x000008d8, 0x0000003f, 0x000008ec, 
					0x00000040, 0x00000900, 0x00000042, 0x00000914, 0x00000042, 0x00000924, 0x00000053, 0x00000938, 
					0x00000055, 0x00000948, 0x00000054, 0x00000958, 0x00000057, 0x0000096c, 0x00000057, 0x0000097c, 
					0x00000066, 0x00000990, 0x00000066, 0x0000099c, 0x00000079, 0x000009b0, 0x0000004c, 0x000009c4, 
					0x0000004c, 0x000009d4, 0x0000004c, 0x000009e0, 0x0000004c, 0x000009f0, 0x0000004c, 0x000009fc, 
					0x0000005c, 0x00000a0c, 0x00000063, 0x00000a1c, 0x00000063, 0x00000a30, 0x00000063, 0x00000a3c, 
					0x00000063, 0x00000a4c, 0x0000006a, 0x00000a5c, 0x0000006a, 0x00000a6c, 0x00000070, 0x00000a80, 
					0x00000068, 0x00000a94, 0x00000069, 0x00000aa8, 0x0000006f, 0x00000abc, 0x00000072, 0x00000ad0, 
					0x00000073, 0x00000ae4, 0x00000079, 0x00000af8, 0x00000079, 0x00000b0c, 0x0000007c, 0x00000b1c, 
					0x0000007b, 0x00000b30, 0x0000007e, 0x00000b44, 0x0000007f, 0x00000b58, 0x00000085, 0x00000b6c, 
					0x00000085, 0x00000b7c, 0x00000085, 0x00000b90, 0x00000087, 0x00000ba0, 0x00000088, 0x00000bb4, 
					0x00000074, 0x00000bc8, 0x0000008a, 0x00000bdc, 0x00000080, 0x00000bf0, 0x0000008b, 0x00000c04, 
					0x0000008c, 0x00000c18, 0x00000080, 0x00000c2c, 0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 
					0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x00000269, 0x00000274, 0x00000005, 0x00040001, 
					0x00010001, 0x00000284, 0x00000044, 0x00010000, 0x00030002, 0x00444441, 0x00030000, 0x00010001, 
					0x00000001, 0x00000000, 0x00000013, 0xffffffff, 0x0000ffff, 0x0058414d, 0x0000000a, 0xffffffff, 
					0x0000ffff, 0x004e494d, 0x0000000b, 0xffffffff, 0x0000ffff, 0x00425553, 0x0000000d, 0xffffffff, 
					0x0000ffff, 0x00667562, 0x00000020, 0x00010000, 0x00030002, 0x00000035, 0x00010000, 0x00030002, 
					0x0000003c, 0x00010000, 0x00030002, 0x65736163, 0xabab0031, 0x00000031, 0xffffffff, 0x0000ffff, 
					0x00000037, 0xffffffff, 0x0000ffff, 0x0000003d, 0xffffffff, 0x0000ffff, 0x65736163, 0xabab0032, 
					0x0000002e, 0xffffffff, 0x0000ffff, 0x00000036, 0xffffffff, 0x0000ffff, 0x0000003e, 0xffffffff, 
					0x0000ffff, 0x00726c63, 0x00000007, 0x00010000, 0x00030002, 0x0000000c, 0x00010000, 0xffff0002, 
					0x00000032, 0xffffffff, 0x0001ffff, 0x00000033, 0xffffffff, 0x0001ffff, 0x00000038, 0xffffffff, 
					0x0000ffff, 0x00000039, 0xffffffff, 0x0000ffff, 0x0000003f, 0x0001ffff, 0xffffffff, 0x00000040, 
					0xffffffff, 0x0002ffff, 0x00000041, 0xffff0000, 0xffffffff, 0x00000042, 0xffffffff, 0x0002ffff, 
					0x00000043, 0xffffffff, 0xffff0002, 0x74636166, 0xab00726f, 0x00000011, 0xffffffff, 0x0000ffff, 
					0x00736c68, 0x00000015, 0xffffffff, 0x0000ffff, 0x00000016, 0xffffffff, 0x0000ffff, 0x00000017, 
					0xffffffff, 0x0000ffff, 0x00000018, 0xffffffff, 0x0000ffff, 0x00000019, 0xffffffff, 0x0000ffff, 
					0x0000001a, 0xffffffff, 0x0000ffff, 0x0000001b, 0xffffffff, 0x0000ffff, 0x0000001c, 0xffffffff, 
					0x0000ffff, 0x0000001e, 0xffffffff, 0x0000ffff, 0x00000026, 0xffffffff, 0x0002ffff, 0x00000027, 
					0xffffffff, 0x0002ffff, 0x0000002b, 0xffffffff, 0x0002ffff, 0x75706e69, 0x61620074, 0xab006573, 
					0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x000004ae, 0x000004b4, 0x00000005, 0x00020001, 
					0x00010001, 0x000004c4, 0x00000005, 0x00010000, 0xffffffff, 0x00706d74, 0x00030001, 0x00030001, 
					0x00000001, 0x00000000, 0x00000010, 0xffff0000, 0xffffffff, 0x00000012, 0x0001ffff, 0xffffffff, 
					0x00000014, 0x00010000, 0xffff0002, 0x0000002d, 0xffffffff, 0x0002ffff, 0x0000002f, 0xffffffff, 
					0x0000ffff, 0x00000030, 0xffffffff, 0x0001ffff, 0x00000000, 0x00000264, 0x0000028c, 0x00000001, 
					0x0000029c, 0x00000264, 0x000002a8, 0x000002ac, 0x00000001, 0x000002bc, 0x00000264, 0x000002c8, 
					0x000002ac, 0x00000001, 0x000002cc, 0x00000264, 0x000002d8, 0x000002ac, 0x00000001, 0x000002dc, 
					0x00000264, 0x000002e8, 0x000002ac, 0x00000001, 0x000002ec, 0x00000264, 0x000002f8, 0x00000274, 
					0x00000003, 0x000002fc, 0x00000264, 0x00000320, 0x000002ac, 0x00000003, 0x00000328, 0x00000264, 
					0x0000034c, 0x000002ac, 0x00000003, 0x00000354, 0x00000264, 0x00000378, 0x00000274, 0x0000000b, 
					0x0000037c, 0x00000264, 0x00000400, 0x000002ac, 0x00000001, 0x00000408, 0x00000264, 0x00000414, 
					0x00000274, 0x0000000c, 0x00000418, 0x00000264, 0x000004a8, 0x000004cc, 0x00000001, 0x000004dc, 
					0x00000264, 0x000004e8, 0x000004ec, 0x00000006, 0x000004fc, 0x7263694d, 0x666f736f, 0x52282074, 
					0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 
					0x34303932, 0xababab00, 0x0037fffe, 0x42415443, 0x0000001c, 0x000000a4, 0xffff0200, 0x00000003, 
					0x0000001c, 0x00000101, 0x0000009d, 0x00000058, 0x00000003, 0x00000001, 0x00000060, 0x00000000, 
					0x00000070, 0x00000002, 0x00000001, 0x00000078, 0x00000088, 0x00000098, 0x00010002, 0x00000001, 
					0x00000078, 0x00000088, 0x65736142, 0x00786554, 0x000c0004, 0x00010001, 0x00000001, 0x00000000, 
					0x65756864, 0xababab00, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 
					0x00000000, 0x00000000, 0x74617364, 0x5f737000, 0x00305f32, 0x7263694d, 0x666f736f, 0x52282074, 
					0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 
					0x34303932, 0xababab00, 0x05000051, 0xa00f0002, 0x00000000, 0x42f00000, 0x43700000, 0xc3b40000, 
					0x05000051, 0xa00f0003, 0x42700000, 0x43b40000, 0xbf800000, 0x3f800000, 0x05000051, 0xa00f0004, 
					0x3f000000, 0xbf000000, 0x3d088889, 0x40e00000, 0x05000051, 0xa00f0005, 0xc0000000, 0xc0c00000, 
					0xc1000000, 0x00000000, 0x05000051, 0xa00f0006, 0x3d088889, 0xc1000000, 0x40800000, 0x00000000, 
					0x0200001f, 0x80000000, 0xb0030000, 0x0200001f, 0x90000000, 0xa00f0800, 0x03000042, 0x800f0000, 
					0xb0e40000, 0xa0e40800, 0x0300000a, 0x80080001, 0x80550000, 0x80000000, 0x0300000b, 0x80080002, 
					0x80000000, 0x80550000, 0x0300000b, 0x80080003, 0x80ff0002, 0x80aa0000, 0x0300000a, 0x80080004, 
					0x80aa0000, 0x80ff0001, 0x03000002, 0x80070001, 0x80e40000, 0x81ff0003, 0x03000002, 0x80080002, 
					0x80ff0003, 0x81ff0004, 0x03000002, 0x80040002, 0x81550000, 0x80000000, 0x02000006, 0x80080001, 
					0x80ff0002, 0x03000002, 0x80010002, 0x81aa0000, 0x80550000, 0x03000005, 0x80080001, 0x80ff0001, 
					0xa0000003, 0x03000002, 0x80020002, 0x81000000, 0x80aa0000, 0x03000002, 0x80080005, 0x80ff0003, 
					0x80ff0004, 0x04000004, 0x80070002, 0x80e40002, 0x80ff0001, 0xa0e40002, 0x04000058, 0x80080001, 
					0x80aa0001, 0x80aa0002, 0x80000000, 0x04000058, 0x80080001, 0x80550001, 0x80550002, 0x80ff0001, 
					0x04000058, 0x80080001, 0x80000001, 0x80000002, 0x80ff0001, 0x03000002, 0x80080003, 0x80ff0001, 
					0xa0550003, 0x04000058, 0x80080001, 0x80ff0001, 0x80ff0001, 0x80ff0003, 0x03000002, 0x80080001, 
					0x80ff0001, 0xa0000000, 0x03000002, 0x80080003, 0x80ff0001, 0xa0550003, 0x04000058, 0x80080003, 
					0x80ff0001, 0x80ff0001, 0x80ff0003, 0x03000002, 0x80080001, 0x80ff0003, 0xa0ff0002, 0x04000058, 
					0x80080001, 0x80ff0001, 0x80ff0001, 0x80ff0003, 0x02000001, 0x80020000, 0xa0aa0004, 0x04000004, 
					0x800f0003, 0x80ff0001, 0x80550000, 0xa0e40005, 0x04000004, 0x80080001, 0x80ff0001, 0xa0000006, 
					0xa0550006, 0x03000002, 0x80080004, 0x80ff0005, 0xa0aa0003, 0x02000023, 0x80080004, 0x80ff0004, 
					0x03000002, 0x80080004, 0x81ff0004, 0xa0ff0003, 0x02000006, 0x80080004, 0x80ff0004, 0x03000005, 
					0x80180002, 0x80ff0002, 0x80ff0004, 0x03000005, 0x80180002, 0x80ff0002, 0xa0000001, 0x04000004, 
					0x80080004, 0x80ff0005, 0xa0000004, 0xa0550004, 0x02000023, 0x80080004, 0x80ff0004, 0x03000002, 
					0x80080004, 0x81ff0004, 0xa0000004, 0x03000005, 0x80080007, 0x80ff0002, 0x80ff0004, 0x03000005, 
					0x80080002, 0x80ff0007, 0xa0ff0004, 0x04000004, 0x80080008, 0x80ff0005, 0xa0000004, 0x80ff0002, 
					0x04000004, 0x80080002, 0x80ff0007, 0x81ff0003, 0x80ff0008, 0x04000004, 0x80080004, 0x80ff0005, 
					0xa0000004, 0x81ff0007, 0x04000004, 0x80080005, 0x80ff0005, 0xa0000004, 0x80ff0007, 0x04000004, 
					0x80080006, 0x80ff0007, 0x80ff0003, 0x80ff0004, 0x04000058, 0x80080006, 0x80000003, 0x80ff0005, 
					0x80ff0006, 0x04000058, 0x80080006, 0x80550003, 0x80ff0002, 0x80ff0006, 0x04000058, 0x80080001, 
					0x80ff0001, 0xa0550006, 0xa0aa0006, 0x03000002, 0x800f0002, 0x80e40003, 0x80ff0001, 0x04000004, 
					0x80080001, 0x80ff0007, 0x81ff0002, 0x80ff0008, 0x04000004, 0x80080003, 0x80ff0007, 0x80ff0002, 
					0x80ff0004, 0x04000058, 0x80080003, 0x80000002, 0x80ff0005, 0x80ff0003, 0x04000058, 0x80080003, 
					0x80550002, 0x80ff0001, 0x80ff0003, 0x03000002, 0x80080001, 0x80ff0002, 0xa0aa0005, 0x04000058, 
					0x80080001, 0x80ff0001, 0xa0550006, 0xa0aa0006, 0x03000002, 0x800f0001, 0x80e40002, 0x80ff0001, 
					0x04000004, 0x80080002, 0x80ff0007, 0x80ff0001, 0x80ff0004, 0x04000004, 0x80080001, 0x80ff0007, 
					0x81ff0001, 0x80ff0008, 0x04000058, 0x80020000, 0x80aa0003, 0x80ff0004, 0x80ff0006, 0x04000058, 
					0x80080002, 0x80000001, 0x80ff0005, 0x80ff0002, 0x04000058, 0x80010000, 0x80aa0002, 0x80ff0004, 
					0x80ff0003, 0x04000058, 0x80080001, 0x80550001, 0x80ff0001, 0x80ff0002, 0x04000058, 0x80040000, 
					0x80aa0001, 0x80ff0004, 0x80ff0001, 0x02000001, 0x800f0800, 0x80e40000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float hue = m_fShiftHue - int(m_fShiftHue / 360) * 360;

				float vShiftHue[4] = {hue,  0, 0, 0};
				float vShiftSat[4] = {m_fShiftSat, 0, 0, 0};

				pDevice->SetPixelShaderConstantF( 0, vShiftHue, 1 );
				pDevice->SetPixelShaderConstantF( 1, vShiftSat, 1 );

				CSampler sampler;
				sampler.SetAddressBorder( 0 );
				sampler.SetTexture( m_pTexture );
				sampler.SetFilterPoint();
				sampler.SetToDevice( pDevice );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_Filter3x3 : public IPixelShader
		{
		public:
			float m_matrix[3][3];
			float m_factor;
			float m_offset;
			float m_fWidth;
			float m_fHeight;

			IDirect3DTexture9* m_pTexture;
		
		public:
			PS_Filter3x3()
			{
				m_matrix[0][0] = 1;
				m_matrix[0][1] = 1;
				m_matrix[0][2] = 1;
				m_matrix[1][0] = 1;
				m_matrix[1][1] = 1;
				m_matrix[1][2] = 1;
				m_matrix[2][0] = 1;
				m_matrix[2][1] = 1;
				m_matrix[2][2] = 1;

				m_factor  = 1;
				m_offset  = 0;
				m_fWidth  = 1;
				m_fHeight = 1;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x00b0fffe, 0x47554244, 0x00000028, 0x00000288, 0x00000000, 0x00000001, 0x00000040, 
					0x0000001f, 0x00000044, 0x00000004, 0x00000238, 0x0000013c, 0x705c3a64, 0x69665f73, 0x7265746c, 
					0x3378335f, 0x7478742e, 0xababab00, 0x00000028, 0xffff0000, 0x0000055c, 0xffff0000, 0x00000568, 
					0x0000003a, 0x00000574, 0x00000039, 0x00000584, 0x0000003b, 0x00000594, 0x0000003c, 0x000005a4, 
					0x0000003d, 0x000005b4, 0x0000003e, 0x000005c4, 0x0000003f, 0x000005d4, 0x00000040, 0x000005e4, 
					0x00000041, 0x000005f4, 0x0000003a, 0x00000604, 0x00000039, 0x00000614, 0x0000003b, 0x00000624, 
					0x0000003c, 0x00000634, 0x0000003d, 0x00000644, 0x0000003e, 0x00000654, 0x0000003f, 0x00000664, 
					0x00000040, 0x00000674, 0x00000041, 0x00000684, 0x0000003a, 0x00000694, 0x0000003a, 0x000006a4, 
					0x0000003b, 0x000006b8, 0x0000003c, 0x000006cc, 0x0000003d, 0x000006e0, 0x0000003e, 0x000006f4, 
					0x0000003f, 0x00000708, 0x00000040, 0x0000071c, 0x00000041, 0x00000730, 0x00000043, 0x00000744, 
					0x00000043, 0x00000758, 0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 0x00030001, 0x00040001, 
					0x00000001, 0x00000000, 0x00000141, 0x0000014c, 0x00000005, 0x00040001, 0x00010001, 0x0000015c, 
					0x0000001e, 0x00010000, 0x00030002, 0x75706e69, 0x61620074, 0xab006573, 0x00030001, 0x00020001, 
					0x00000001, 0x00000000, 0x00000186, 0x0000018c, 0x00000005, 0x00020001, 0x00010001, 0x0000019c, 
					0x00000000, 0x00010000, 0xffffffff, 0x7074756f, 0xab007475, 0x0000001d, 0x00010000, 0x00030002, 
					0x006d7573, 0x00000015, 0x00010000, 0x00030002, 0x00000016, 0x00010000, 0x00030002, 0x00000017, 
					0x00010000, 0x00030002, 0x00000018, 0x00010000, 0x00030002, 0x00000019, 0x00010000, 0x00030002, 
					0x0000001a, 0x00010000, 0x00030002, 0x0000001b, 0x00010000, 0x00030002, 0x0000001c, 0x00010000, 
					0x00030002, 0x00000000, 0x0000013c, 0x00000164, 0x00000001, 0x00000174, 0x0000013c, 0x00000180, 
					0x000001a4, 0x00000001, 0x000001b4, 0x0000013c, 0x000001c0, 0x00000164, 0x00000001, 0x000001c8, 
					0x0000013c, 0x000001d4, 0x0000014c, 0x00000008, 0x000001d8, 0x7263694d, 0x666f736f, 0x52282074, 
					0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 
					0x34303932, 0xababab00, 0x00a4fffe, 0x42415443, 0x0000001c, 0x0000025b, 0xffff0200, 0x00000014, 
					0x0000001c, 0x00000101, 0x00000254, 0x000001ac, 0x00000003, 0x00000001, 0x000001b4, 0x00000000, 
					0x000001c4, 0x00090002, 0x00000001, 0x000001c8, 0x000001d8, 0x000001e8, 0x000a0002, 0x00000001, 
					0x000001c8, 0x000001d8, 0x000001ec, 0x000b0002, 0x00000001, 0x000001c8, 0x000001d8, 0x000001f0, 
					0x000c0002, 0x00000001, 0x000001c8, 0x000001d8, 0x000001f4, 0x000d0002, 0x00000001, 0x000001c8, 
					0x000001d8, 0x000001f8, 0x000e0002, 0x00000001, 0x000001c8, 0x000001d8, 0x000001fc, 0x000f0002, 
					0x00000001, 0x000001c8, 0x000001d8, 0x00000200, 0x00100002, 0x00000001, 0x000001c8, 0x000001d8, 
					0x00000204, 0x00110002, 0x00000001, 0x000001c8, 0x000001d8, 0x00000208, 0x00120002, 0x00000001, 
					0x000001c8, 0x00000210, 0x00000220, 0x00000002, 0x00000001, 0x00000224, 0x00000210, 0x00000234, 
					0x00010002, 0x00000001, 0x00000224, 0x00000210, 0x00000238, 0x00020002, 0x00000001, 0x00000224, 
					0x00000210, 0x0000023c, 0x00030002, 0x00000001, 0x00000224, 0x00000210, 0x00000240, 0x00040002, 
					0x00000001, 0x00000224, 0x00000210, 0x00000244, 0x00050002, 0x00000001, 0x00000224, 0x00000210, 
					0x00000248, 0x00060002, 0x00000001, 0x00000224, 0x00000210, 0x0000024c, 0x00070002, 0x00000001, 
					0x00000224, 0x00000210, 0x00000250, 0x00080002, 0x00000001, 0x00000224, 0x00000210, 0x65736142, 
					0x00786554, 0x000c0004, 0x00010001, 0x00000001, 0x00000000, 0x00303064, 0x00030001, 0x00020001, 
					0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00313064, 0x00323064, 
					0x00303164, 0x00313164, 0x00323164, 0x00303264, 0x00313264, 0x00323264, 0x74636166, 0xab00726f, 
					0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x0030306d, 0x00030000, 0x00010001, 0x00000001, 
					0x00000000, 0x0031306d, 0x0032306d, 0x0030316d, 0x0031316d, 0x0032316d, 0x0030326d, 0x0031326d, 
					0x0032326d, 0x325f7370, 0x4d00305f, 0x6f726369, 0x74666f73, 0x29522820, 0x44334420, 0x53203958, 
					0x65646168, 0x6f432072, 0x6c69706d, 0x35207265, 0x2e34302e, 0x322e3030, 0x00343039, 0x0200001f, 
					0x80000000, 0xb0030000, 0x0200001f, 0x90000000, 0xa00f0800, 0x03000002, 0x80030008, 0xb0e40000, 
					0xa0e4000a, 0x03000002, 0x80030007, 0xb0e40000, 0xa0e40009, 0x03000002, 0x80030006, 0xb0e40000, 
					0xa0e4000b, 0x03000002, 0x80030005, 0xb0e40000, 0xa0e4000c, 0x03000002, 0x80030004, 0xb0e40000, 
					0xa0e4000d, 0x03000002, 0x80030003, 0xb0e40000, 0xa0e4000e, 0x03000002, 0x80030002, 0xb0e40000, 
					0xa0e4000f, 0x03000002, 0x80030001, 0xb0e40000, 0xa0e40010, 0x03000002, 0x80030000, 0xb0e40000, 
					0xa0e40011, 0x03000042, 0x800f0008, 0x80e40008, 0xa0e40800, 0x03000042, 0x800f0007, 0x80e40007, 
					0xa0e40800, 0x03000042, 0x800f0006, 0x80e40006, 0xa0e40800, 0x03000042, 0x800f0005, 0x80e40005, 
					0xa0e40800, 0x03000042, 0x800f0004, 0x80e40004, 0xa0e40800, 0x03000042, 0x800f0003, 0x80e40003, 
					0xa0e40800, 0x03000042, 0x800f0002, 0x80e40002, 0xa0e40800, 0x03000042, 0x800f0001, 0x80e40001, 
					0xa0e40800, 0x03000042, 0x800f0000, 0x80e40000, 0xa0e40800, 0x03000005, 0x800f0008, 0x80e40008, 
					0xa0000001, 0x04000004, 0x800f0007, 0x80e40007, 0xa0000000, 0x80e40008, 0x04000004, 0x800f0006, 
					0x80e40006, 0xa0000002, 0x80e40007, 0x04000004, 0x800f0005, 0x80e40005, 0xa0000003, 0x80e40006, 
					0x04000004, 0x800f0004, 0x80e40004, 0xa0000004, 0x80e40005, 0x04000004, 0x800f0003, 0x80e40003, 
					0xa0000005, 0x80e40004, 0x04000004, 0x800f0002, 0x80e40002, 0xa0000006, 0x80e40003, 0x04000004, 
					0x800f0001, 0x80e40001, 0xa0000007, 0x80e40002, 0x04000004, 0x800f0000, 0x80e40000, 0xa0000008, 
					0x80e40001, 0x04000004, 0x801f0000, 0x80e40000, 0xa0000012, 0xa0550012, 0x02000001, 0x800f0800, 
					0x80e40000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float dx = 1 / m_fWidth;
				float dy = 1 / m_fHeight;
				float disp[9][4] = {
					-dx, -dy, 0, 0,
					  0, -dy, 0, 0,
					 dx, -dy, 0, 0,
					-dx,   0, 0, 0,
					  0,   0, 0, 0,
					 dx,   0, 0, 0,
					-dx,  dy, 0, 0,
					  0,  dy, 0, 0,
					 dx,  dy, 0, 0,
				};
				float factor[4] = {m_factor, m_offset, 0, 0};
				float filter[9][4] = {
					m_matrix[0][0], 0, 0, 0,
					m_matrix[0][1], 0, 0, 0,
					m_matrix[0][2], 0, 0, 0,
					m_matrix[1][0], 0, 0, 0,
					m_matrix[1][1], 0, 0, 0,
					m_matrix[1][2], 0, 0, 0,
					m_matrix[2][0], 0, 0, 0,
					m_matrix[2][1], 0, 0, 0,
					m_matrix[2][2], 0, 0, 0,
				};

				pDevice->SetPixelShaderConstantF( 0, &filter[0][0], 9 );
				pDevice->SetPixelShaderConstantF( 9, &disp[0][0], 9 );
				pDevice->SetPixelShaderConstantF( 18, factor, 1 );

				CSampler sampler;
				sampler.SetAddressClamp();
				sampler.SetTexture( m_pTexture );
				sampler.SetFilterPoint();
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};


		class PS_FilterCenter3x3 : public IPixelShader
		{
		public:
			float m_fCenter;
			float m_fBorder;
			float m_fWidth;
			float m_fHeight;
			float m_fSumFactor;
			float m_fSumOffset;

			IDirect3DTexture9* m_pTexture;
		
		public:
			PS_FilterCenter3x3()
			{
				m_fCenter = 1;
				m_fBorder = 0;
				m_fWidth  = 1;
				m_fHeight = 1;
				m_fSumFactor = 1;
				m_fSumOffset = 0;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x008bfffe, 0x47554244, 0x00000028, 0x000001f4, 0x00000000, 0x00000001, 0x00000044, 
					0x00000012, 0x00000048, 0x00000004, 0x000001a4, 0x000000d8, 0x705c3a64, 0x69665f73, 0x7265746c, 
					0x6e65635f, 0x5f726574, 0x2e337833, 0x00747874, 0x00000028, 0xffff0000, 0x00000380, 0xffff0000, 
					0x0000038c, 0x0000002d, 0x00000398, 0x0000002e, 0x000003a8, 0x0000002f, 0x000003b8, 0x00000030, 
					0x000003c8, 0x0000002d, 0x000003d8, 0x0000002b, 0x000003e8, 0x0000002e, 0x000003f8, 0x0000002f, 
					0x00000408, 0x00000030, 0x00000418, 0x0000002d, 0x00000428, 0x0000002d, 0x00000438, 0x0000002e, 
					0x0000044c, 0x0000002f, 0x00000460, 0x00000030, 0x00000474, 0x00000032, 0x00000488, 0x00000032, 
					0x0000049c, 0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 
					0x00000000, 0x000000dd, 0x000000e8, 0x00000005, 0x00040001, 0x00010001, 0x000000f8, 0x00000011, 
					0x00010000, 0x00030002, 0x75706e69, 0x61620074, 0xab006573, 0x00030001, 0x00020001, 0x00000001, 
					0x00000000, 0x00000122, 0x00000128, 0x00000005, 0x00020001, 0x00010001, 0x00000138, 0x00000000, 
					0x00010000, 0xffffffff, 0x7074756f, 0xab007475, 0x00000010, 0x00010000, 0x00030002, 0x006d7573, 
					0x0000000c, 0x00010000, 0x00030002, 0x0000000d, 0x00010000, 0x00030002, 0x0000000e, 0x00010000, 
					0x00030002, 0x0000000f, 0x00010000, 0x00030002, 0x00000000, 0x000000d8, 0x00000100, 0x00000001, 
					0x00000110, 0x000000d8, 0x0000011c, 0x00000140, 0x00000001, 0x00000150, 0x000000d8, 0x0000015c, 
					0x00000100, 0x00000001, 0x00000164, 0x000000d8, 0x00000170, 0x000000e8, 0x00000004, 0x00000174, 
					0x7263694d, 0x666f736f, 0x52282074, 0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 
					0x2072656c, 0x34302e35, 0x2e30302e, 0x34303932, 0xababab00, 0x0052fffe, 0x42415443, 0x0000001c, 
					0x00000111, 0xffff0200, 0x00000007, 0x0000001c, 0x00000101, 0x0000010a, 0x000000a8, 0x00000003, 
					0x00000001, 0x000000b0, 0x00000000, 0x000000c0, 0x00000002, 0x00000001, 0x000000c4, 0x000000d4, 
					0x000000e4, 0x00010002, 0x00000001, 0x000000c4, 0x000000d4, 0x000000e7, 0x00020002, 0x00000001, 
					0x000000c4, 0x000000d4, 0x000000ea, 0x00030002, 0x00000001, 0x000000c4, 0x000000d4, 0x000000ed, 
					0x00040002, 0x00000001, 0x000000c4, 0x000000f4, 0x00000104, 0x00050002, 0x00000001, 0x000000c4, 
					0x000000f4, 0x65736142, 0x00786554, 0x000c0004, 0x00010001, 0x00000001, 0x00000000, 0xab003164, 
					0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
					0x64003264, 0x34640033, 0x63616600, 0x00726f74, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 
					0x6d6d7573, 0x73700061, 0x305f325f, 0x63694d00, 0x6f736f72, 0x28207466, 0x44202952, 0x39584433, 
					0x61685320, 0x20726564, 0x706d6f43, 0x72656c69, 0x302e3520, 0x30302e34, 0x3039322e, 0xabab0034, 
					0x0200001f, 0x80000000, 0xb0030000, 0x0200001f, 0x90000000, 0xa00f0800, 0x03000002, 0x80030003, 
					0xb0e40000, 0xa0e40000, 0x03000002, 0x80030002, 0xb0e40000, 0xa0e40001, 0x03000002, 0x80030001, 
					0xb0e40000, 0xa0e40002, 0x03000002, 0x80030000, 0xb0e40000, 0xa0e40003, 0x03000042, 0x800f0004, 
					0x80e40003, 0xa0e40800, 0x03000042, 0x800f0003, 0xb0e40000, 0xa0e40800, 0x03000042, 0x800f0002, 
					0x80e40002, 0xa0e40800, 0x03000042, 0x800f0001, 0x80e40001, 0xa0e40800, 0x03000042, 0x800f0000, 
					0x80e40000, 0xa0e40800, 0x03000005, 0x800f0004, 0x80e40004, 0xa0550004, 0x04000004, 0x800f0003, 
					0x80e40003, 0xa0000004, 0x80e40004, 0x04000004, 0x800f0002, 0x80e40002, 0xa0550004, 0x80e40003, 
					0x04000004, 0x800f0001, 0x80e40001, 0xa0550004, 0x80e40002, 0x04000004, 0x800f0000, 0x80e40000, 
					0xa0550004, 0x80e40001, 0x04000004, 0x801f0000, 0x80e40000, 0xa0000005, 0xa0550005, 0x02000001, 
					0x800f0800, 0x80e40000, 0x0000ffff 
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float dx = 0.5f / m_fWidth;
				float dy = 0.5f / m_fHeight;
				float disp[4][4] = {
					-dx,     -dy * 2, 0, 0,
					 dx * 2, -dy,     0, 0,
					 dx,      dy * 2, 0, 0,
					-dx * 2,  dy,     0, 0,
				};

				float factor[4] = {m_fCenter, m_fBorder * 2, 0, 0};
				float summa [4] = {m_fSumFactor, m_fSumOffset, 0, 0};

				pDevice->SetPixelShaderConstantF( 0, &disp[0][0], 4 );
				pDevice->SetPixelShaderConstantF( 4, factor, 1 );
				pDevice->SetPixelShaderConstantF( 5, summa, 1 );

				CSampler sampler;
				sampler.SetAddressClamp();
				sampler.SetTexture( m_pTexture );
				sampler.SetFilterLinear();
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_PerspectiveTransform : public IPixelShader
		{
		public:
			float m_matrix[3][3];
			
			float m_width;
			float m_height;

			D3DCOLOR m_nBackColor;
			IDirect3DTexture9* m_pTexture;
		
		public:
			PS_PerspectiveTransform()
			{
				m_matrix[0][0] = 1; 
				m_matrix[0][1] = 0;
				m_matrix[0][2] = 0;
				m_matrix[1][0] = 0;
				m_matrix[1][1] = 1;
				m_matrix[1][2] = 0;
				m_matrix[2][0] = 0;
				m_matrix[2][1] = 0;
				m_matrix[2][2] = 1;

				m_width = 1;
				m_height = 1;

				m_nBackColor = 0;
				m_pTexture = 0;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0200, 0x012ffffe, 0x47554244, 0x00000028, 0x00000484, 0x00000000, 0x00000001, 0x00000040, 
					0x0000002c, 0x00000044, 0x00000009, 0x000003d0, 0x000001a4, 0x705c3a64, 0x65705f73, 0x65707372, 
					0x76697463, 0x78742e65, 0xabab0074, 0x00000028, 0xffff0000, 0x00000638, 0xffff0000, 0x00000650, 
					0xffff0000, 0x0000065c, 0x0000002a, 0x00000668, 0x00000030, 0x00000678, 0x00000030, 0x00000688, 
					0x00000036, 0x00000698, 0x00000030, 0x000006a4, 0x00000036, 0x000006b4, 0x0000002b, 0x000006c4, 
					0x00000031, 0x000006d4, 0x00000031, 0x000006e4, 0x00000037, 0x000006f4, 0x00000031, 0x00000700, 
					0x00000037, 0x00000710, 0x0000002c, 0x00000720, 0x00000032, 0x00000730, 0x00000032, 0x00000740, 
					0x00000038, 0x00000750, 0x00000032, 0x0000075c, 0x00000038, 0x0000076c, 0x0000002d, 0x0000077c, 
					0x00000033, 0x0000078c, 0x00000033, 0x0000079c, 0x00000039, 0x000007ac, 0x00000033, 0x000007b8, 
					0x00000039, 0x000007c8, 0x0000002e, 0x000007d8, 0x00000034, 0x000007e8, 0x00000034, 0x000007f8, 
					0x0000003a, 0x00000808, 0x00000034, 0x00000814, 0x0000003a, 0x00000824, 0x0000003c, 0x00000834, 
					0x0000003d, 0x00000844, 0x0000003e, 0x00000854, 0x0000003f, 0x00000864, 0x00000040, 0x00000874, 
					0x0000003d, 0x00000884, 0x0000003e, 0x00000894, 0x0000003f, 0x000008a4, 0x00000040, 0x000008b4, 
					0x00000042, 0x000008c4, 0x00000042, 0x000008d4, 0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 
					0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x000001a9, 0x000001b4, 0x00000005, 0x00040001, 
					0x00010001, 0x000001c4, 0x0000002b, 0x00010000, 0x00030002, 0x00726c63, 0x00000021, 0x00010000, 
					0x00030002, 0x00000026, 0x00010000, 0x00030002, 0x00000027, 0x00010000, 0x00030002, 0x00000028, 
					0x00010000, 0x00030002, 0x00000029, 0x00010000, 0x00030002, 0x75706e69, 0x61620074, 0xab006573, 
					0x00030001, 0x00030001, 0x00000001, 0x00000000, 0x0000022e, 0x00000234, 0x00000005, 0x00030001, 
					0x00010001, 0x00000244, 0x00000001, 0x00010000, 0xffff0002, 0x7074756f, 0xab007475, 0x0000002a, 
					0x00010000, 0x00030002, 0x30736f70, 0xababab00, 0x00000003, 0x00010000, 0xffff0002, 0x00000004, 
					0xffff0002, 0xffffffff, 0x00000005, 0xffff0000, 0xffffffff, 0x00000007, 0x0001ffff, 0xffffffff, 
					0x00000008, 0x00010000, 0xffffffff, 0x31736f70, 0xababab00, 0x00000009, 0x00010000, 0xffff0002, 
					0x0000000a, 0xffff0002, 0xffffffff, 0x0000000b, 0xffff0000, 0xffffffff, 0x0000000d, 0x0001ffff, 
					0xffffffff, 0x0000000e, 0x00010000, 0xffffffff, 0x32736f70, 0xababab00, 0x0000000f, 0x00010000, 
					0xffff0002, 0x00000010, 0xffff0002, 0xffffffff, 0x00000011, 0xffff0000, 0xffffffff, 0x00000013, 
					0x0001ffff, 0xffffffff, 0x00000014, 0x00010000, 0xffffffff, 0x33736f70, 0xababab00, 0x00000015, 
					0x00010000, 0xffff0002, 0x00000016, 0xffff0002, 0xffffffff, 0x00000017, 0xffff0000, 0xffffffff, 
					0x00000019, 0x0001ffff, 0xffffffff, 0x0000001a, 0x00010000, 0xffffffff, 0x34736f70, 0xababab00, 
					0x0000001b, 0x00010000, 0xffff0002, 0x0000001c, 0xffff0002, 0xffffffff, 0x0000001d, 0xffff0000, 
					0xffffffff, 0x0000001f, 0x0001ffff, 0xffffffff, 0x00000020, 0x00010000, 0xffffffff, 0x00000000, 
					0x000001a4, 0x000001cc, 0x00000001, 0x000001dc, 0x000001a4, 0x000001e8, 0x000001b4, 0x00000005, 
					0x000001ec, 0x000001a4, 0x00000228, 0x0000024c, 0x00000001, 0x0000025c, 0x000001a4, 0x00000268, 
					0x000001cc, 0x00000001, 0x00000270, 0x000001a4, 0x0000027c, 0x00000234, 0x00000005, 0x00000284, 
					0x000001a4, 0x000002c0, 0x00000234, 0x00000005, 0x000002c8, 0x000001a4, 0x00000304, 0x00000234, 
					0x00000005, 0x0000030c, 0x000001a4, 0x00000348, 0x00000234, 0x00000005, 0x00000350, 0x000001a4, 
					0x0000038c, 0x00000234, 0x00000005, 0x00000394, 0x7263694d, 0x666f736f, 0x52282074, 0x33442029, 
					0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 0x34303932, 
					0xababab00, 0x005cfffe, 0x42415443, 0x0000001c, 0x0000013b, 0xffff0200, 0x00000007, 0x0000001c, 
					0x00000101, 0x00000134, 0x000000a8, 0x00000003, 0x00000001, 0x000000b0, 0x00000000, 0x000000c0, 
					0x00000002, 0x00000003, 0x000000c4, 0x000000d4, 0x00000104, 0x00030002, 0x00000001, 0x00000108, 
					0x00000118, 0x00000128, 0x00040002, 0x00000001, 0x00000108, 0x00000118, 0x0000012b, 0x00050002, 
					0x00000001, 0x00000108, 0x00000118, 0x0000012e, 0x00060002, 0x00000001, 0x00000108, 0x00000118, 
					0x00000131, 0x00070002, 0x00000001, 0x00000108, 0x00000118, 0x65736142, 0x00786554, 0x000c0004, 
					0x00010001, 0x00000001, 0x00000000, 0x0074616d, 0x00030003, 0x00030003, 0x00000001, 0x00000000, 
					0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3f800000, 0x00000000, 0x00000000, 
					0x00000000, 0x00000000, 0x3f800000, 0x00000000, 0xab003070, 0x00030001, 0x00030001, 0x00000001, 
					0x00000000, 0x00000000, 0x00000000, 0x3f800000, 0x00000000, 0x70003170, 0x33700032, 0x00347000, 
					0x325f7370, 0x4d00305f, 0x6f726369, 0x74666f73, 0x29522820, 0x44334420, 0x53203958, 0x65646168, 
					0x6f432072, 0x6c69706d, 0x35207265, 0x2e34302e, 0x322e3030, 0x00343039, 0x05000051, 0xa00f0008, 
					0x3e4ccccd, 0x00000000, 0x00000000, 0x00000000, 0x0200001f, 0x80000000, 0xb0070000, 0x0200001f, 
					0x90000000, 0xa00f0800, 0x03000002, 0x80070000, 0xb0e40000, 0xa0e40003, 0x03000008, 0x80010002, 
					0x80e40000, 0xa0e40002, 0x03000008, 0x80010001, 0x80e40000, 0xa0e40000, 0x02000006, 0x80080000, 
					0x80000002, 0x03000008, 0x80020001, 0x80e40000, 0xa0e40001, 0x03000005, 0x80030005, 0x80e40001, 
					0x80ff0000, 0x03000002, 0x80070000, 0xb0e40000, 0xa0e40004, 0x03000008, 0x80010002, 0x80e40000, 
					0xa0e40002, 0x03000008, 0x80010001, 0x80e40000, 0xa0e40000, 0x02000006, 0x80080000, 0x80000002, 
					0x03000008, 0x80020001, 0x80e40000, 0xa0e40001, 0x03000005, 0x80030004, 0x80e40001, 0x80ff0000, 
					0x03000002, 0x80070000, 0xb0e40000, 0xa0e40005, 0x03000008, 0x80010002, 0x80e40000, 0xa0e40002, 
					0x03000008, 0x80010001, 0x80e40000, 0xa0e40000, 0x02000006, 0x80080000, 0x80000002, 0x03000008, 
					0x80020001, 0x80e40000, 0xa0e40001, 0x03000005, 0x80030003, 0x80e40001, 0x80ff0000, 0x03000002, 
					0x80070000, 0xb0e40000, 0xa0e40006, 0x03000008, 0x80010002, 0x80e40000, 0xa0e40002, 0x03000008, 
					0x80010001, 0x80e40000, 0xa0e40000, 0x02000006, 0x80080000, 0x80000002, 0x03000008, 0x80020001, 
					0x80e40000, 0xa0e40001, 0x03000005, 0x80030002, 0x80e40001, 0x80ff0000, 0x03000002, 0x80070000, 
					0xb0e40000, 0xa0e40007, 0x03000008, 0x80010006, 0x80e40000, 0xa0e40002, 0x03000008, 0x80010001, 
					0x80e40000, 0xa0e40000, 0x02000006, 0x80080000, 0x80000006, 0x03000008, 0x80020001, 0x80e40000, 
					0xa0e40001, 0x03000005, 0x80030001, 0x80e40001, 0x80ff0000, 0x03000042, 0x800f0000, 0x80e40005, 
					0xa0e40800, 0x03000042, 0x800f0004, 0x80e40004, 0xa0e40800, 0x03000042, 0x800f0003, 0x80e40003, 
					0xa0e40800, 0x03000042, 0x800f0002, 0x80e40002, 0xa0e40800, 0x03000042, 0x800f0001, 0x80e40001, 
					0xa0e40800, 0x03000002, 0x800f0000, 0x80e40000, 0x80e40004, 0x03000002, 0x800f0000, 0x80e40003, 
					0x80e40000, 0x03000002, 0x800f0000, 0x80e40002, 0x80e40000, 0x03000002, 0x800f0000, 0x80e40001, 
					0x80e40000, 0x03000005, 0x800f0000, 0x80e40000, 0xa0000008, 0x02000001, 0x800f0800, 0x80e40000, 
					0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float m[3][4] = {
					m_matrix[0][0], m_matrix[1][0], m_matrix[2][0], 0,
					m_matrix[0][1], m_matrix[1][1], m_matrix[2][1], 0,
					m_matrix[0][2], m_matrix[1][2], m_matrix[2][2], 0,
				};
				pDevice->SetPixelShaderConstantF( 0, &m[0][0], 3 );

				float w = 0.4f / m_width;
				float h = 0.4f / m_height;

				float p[5][4] = { 
					 0,  0, 1, 0,
					 0, -h, 1, 0,
					 w,  0, 1, 0,
					 0,  h, 1, 0,
					-w,  0, 1, 0,
				};
				pDevice->SetPixelShaderConstantF( 3, &p[0][0], 5 );

				CSampler sampler;
				sampler.SetAddressBorder( m_nBackColor );
				sampler.SetTexture( m_pTexture );
				sampler.SetFilterLinear();
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_Minimal : public IPixelShader
		{
		public:
			int m_count;
			float m_dx;
			float m_dy;

			IDirect3DTexture9* m_pTexture;
		
		public:
			PS_Minimal()
			{
				m_count = 1;
				m_dx = 0;
				m_dy = 0;

				m_pTexture = 0;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0300, 0x0090fffe, 0x47554244, 0x00000028, 0x00000208, 0x00000000, 0x00000001, 0x0000003c, 
					0x00000011, 0x00000040, 0x00000004, 0x000001b8, 0x000000c8, 0x705c3a64, 0x696d5f73, 0x616d696e, 
					0x78742e6c, 0xabab0074, 0x00000028, 0xffff0000, 0x00000348, 0xffff0000, 0x00000360, 0xffff0000, 
					0x0000036c, 0x00000026, 0x00000378, 0x00000026, 0x00000388, 0x00000026, 0x00000394, 0x00000026, 
					0x000003a0, 0x0000002b, 0x000003a8, 0x0000002b, 0x000003bc, 0x0000002b, 0x000003cc, 0x0000002c, 
					0x000003dc, 0x0000002c, 0x000003f0, 0x0000002c, 0x00000400, 0x00000029, 0x00000410, 0x00000029, 
					0x00000420, 0x00000029, 0x00000424, 0x00000029, 0x00000430, 0x6e69614d, 0x66696400, 0x65737566, 
					0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x000000cd, 0x000000d8, 0x00000005, 
					0x00040001, 0x00010001, 0x000000e8, 0x0000000f, 0x00010000, 0xffff0002, 0x00000010, 0xffffffff, 
					0x0003ffff, 0x00726c63, 0x00000003, 0x00010000, 0x00030002, 0x00000004, 0x00010000, 0xffff0002, 
					0x00000009, 0x00010000, 0xffff0002, 0x0000000c, 0x00010000, 0xffff0002, 0xabab0069, 0x00020000, 
					0x00010001, 0x00000001, 0x00000000, 0x00000005, 0xffffffff, 0x0000ffff, 0x0000000d, 0xffffffff, 
					0x0000ffff, 0x75706e69, 0x61620074, 0xab006573, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 
					0x0000017e, 0x00000184, 0x00000005, 0x00020001, 0x00010001, 0x00000194, 0x00000001, 0x00010000, 
					0xffffffff, 0x00000000, 0x000000c8, 0x000000f0, 0x00000002, 0x00000100, 0x000000c8, 0x00000118, 
					0x000000d8, 0x00000004, 0x0000011c, 0x000000c8, 0x0000014c, 0x00000150, 0x00000002, 0x00000160, 
					0x000000c8, 0x00000178, 0x0000019c, 0x00000001, 0x000001ac, 0x7263694d, 0x666f736f, 0x52282074, 
					0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 
					0x34303932, 0xababab00, 0x003ffffe, 0x42415443, 0x0000001c, 0x000000c7, 0xffff0300, 0x00000003, 
					0x0000001c, 0x00000101, 0x000000c0, 0x00000058, 0x00000003, 0x00000001, 0x00000060, 0x00000000, 
					0x00000070, 0x00000001, 0x00000001, 0x00000078, 0x00000088, 0x00000098, 0x00000002, 0x00000001, 
					0x000000a0, 0x000000b0, 0x65736142, 0x00786554, 0x000c0004, 0x00010001, 0x00000001, 0x00000000, 
					0x6e756f63, 0xabab0074, 0x00020000, 0x00010001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 
					0x00000001, 0x00000000, 0x69727473, 0xab006564, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 
					0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x335f7370, 0x4d00305f, 0x6f726369, 0x74666f73, 
					0x29522820, 0x44334420, 0x53203958, 0x65646168, 0x6f432072, 0x6c69706d, 0x35207265, 0x2e34302e, 
					0x322e3030, 0x00343039, 0x05000051, 0xa00f0001, 0x00000000, 0x3f800000, 0x00000000, 0x00000000, 
					0x0200001f, 0x80000005, 0x90030000, 0x0200001f, 0x90000000, 0xa00f0800, 0x03000042, 0x800f0001, 
					0x90e40000, 0xa0e40800, 0x02000001, 0x80070000, 0x80e40001, 0x02000001, 0x80080000, 0xa0000001, 
					0x01000026, 0xf0e40000, 0x04000004, 0x80030002, 0x80ff0000, 0xa1e40000, 0x90e40000, 0x03000042, 
					0x800f0002, 0x80e40002, 0xa0e40800, 0x0300000a, 0x80070003, 0x80e40002, 0x80e40000, 0x04000004, 
					0x80030000, 0x80ff0000, 0xa0e40000, 0x90e40000, 0x03000042, 0x800f0002, 0x80e40000, 0xa0e40800, 
					0x0300000a, 0x80070000, 0x80e40002, 0x80e40003, 0x03000002, 0x80080000, 0x80ff0000, 0xa0550001, 
					0x00000027, 0x02000001, 0x80070800, 0x80e40000, 0x02000001, 0x80080800, 0x80ff0001, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float stride[4] = {m_dx, m_dy, 0, 0};
				pDevice->SetPixelShaderConstantF( 0, stride, 1 );

				int count[4] = {max(0, min(255, m_count)), 0, 0, 0};
				pDevice->SetPixelShaderConstantI( 0, count, 1 );

				CSampler sampler;
				sampler.SetTexture( m_pTexture );
				sampler.SetAddressClamp();
				sampler.SetFilterPoint();
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_Maximal : public IPixelShader
		{
		public:
			int m_count;
			float m_dx;
			float m_dy;

			IDirect3DTexture9* m_pTexture;
		
		public:
			PS_Maximal()
			{
				m_count = 1;
				m_dx = 0;
				m_dy = 0;

				m_pTexture = 0;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0300, 0x0090fffe, 0x47554244, 0x00000028, 0x00000208, 0x00000000, 0x00000001, 0x0000003c, 
					0x00000011, 0x00000040, 0x00000004, 0x000001b8, 0x000000c8, 0x705c3a64, 0x616d5f73, 0x616d6978, 
					0x78742e6c, 0xabab0074, 0x00000028, 0xffff0000, 0x00000348, 0xffff0000, 0x00000360, 0xffff0000, 
					0x0000036c, 0x00000026, 0x00000378, 0x00000026, 0x00000388, 0x00000026, 0x00000394, 0x00000026, 
					0x000003a0, 0x0000002b, 0x000003a8, 0x0000002b, 0x000003bc, 0x0000002b, 0x000003cc, 0x0000002c, 
					0x000003dc, 0x0000002c, 0x000003f0, 0x0000002c, 0x00000400, 0x00000029, 0x00000410, 0x00000029, 
					0x00000420, 0x00000029, 0x00000424, 0x00000029, 0x00000430, 0x6e69614d, 0x66696400, 0x65737566, 
					0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x000000cd, 0x000000d8, 0x00000005, 
					0x00040001, 0x00010001, 0x000000e8, 0x0000000f, 0x00010000, 0xffff0002, 0x00000010, 0xffffffff, 
					0x0003ffff, 0x00726c63, 0x00000003, 0x00010000, 0x00030002, 0x00000004, 0x00010000, 0xffff0002, 
					0x00000009, 0x00010000, 0xffff0002, 0x0000000c, 0x00010000, 0xffff0002, 0xabab0069, 0x00020000, 
					0x00010001, 0x00000001, 0x00000000, 0x00000005, 0xffffffff, 0x0000ffff, 0x0000000d, 0xffffffff, 
					0x0000ffff, 0x75706e69, 0x61620074, 0xab006573, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 
					0x0000017e, 0x00000184, 0x00000005, 0x00020001, 0x00010001, 0x00000194, 0x00000001, 0x00010000, 
					0xffffffff, 0x00000000, 0x000000c8, 0x000000f0, 0x00000002, 0x00000100, 0x000000c8, 0x00000118, 
					0x000000d8, 0x00000004, 0x0000011c, 0x000000c8, 0x0000014c, 0x00000150, 0x00000002, 0x00000160, 
					0x000000c8, 0x00000178, 0x0000019c, 0x00000001, 0x000001ac, 0x7263694d, 0x666f736f, 0x52282074, 
					0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 
					0x34303932, 0xababab00, 0x003ffffe, 0x42415443, 0x0000001c, 0x000000c7, 0xffff0300, 0x00000003, 
					0x0000001c, 0x00000101, 0x000000c0, 0x00000058, 0x00000003, 0x00000001, 0x00000060, 0x00000000, 
					0x00000070, 0x00000001, 0x00000001, 0x00000078, 0x00000088, 0x00000098, 0x00000002, 0x00000001, 
					0x000000a0, 0x000000b0, 0x65736142, 0x00786554, 0x000c0004, 0x00010001, 0x00000001, 0x00000000, 
					0x6e756f63, 0xabab0074, 0x00020000, 0x00010001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 
					0x00000001, 0x00000000, 0x69727473, 0xab006564, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 
					0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x335f7370, 0x4d00305f, 0x6f726369, 0x74666f73, 
					0x29522820, 0x44334420, 0x53203958, 0x65646168, 0x6f432072, 0x6c69706d, 0x35207265, 0x2e34302e, 
					0x322e3030, 0x00343039, 0x05000051, 0xa00f0001, 0x00000000, 0x3f800000, 0x00000000, 0x00000000, 
					0x0200001f, 0x80000005, 0x90030000, 0x0200001f, 0x90000000, 0xa00f0800, 0x03000042, 0x800f0001, 
					0x90e40000, 0xa0e40800, 0x02000001, 0x80070000, 0x80e40001, 0x02000001, 0x80080000, 0xa0000001, 
					0x01000026, 0xf0e40000, 0x04000004, 0x80030002, 0x80ff0000, 0xa1e40000, 0x90e40000, 0x03000042, 
					0x800f0002, 0x80e40002, 0xa0e40800, 0x0300000b, 0x80070003, 0x80e40000, 0x80e40002, 0x04000004, 
					0x80030000, 0x80ff0000, 0xa0e40000, 0x90e40000, 0x03000042, 0x800f0002, 0x80e40000, 0xa0e40800, 
					0x0300000b, 0x80070000, 0x80e40003, 0x80e40002, 0x03000002, 0x80080000, 0x80ff0000, 0xa0550001, 
					0x00000027, 0x02000001, 0x80070800, 0x80e40000, 0x02000001, 0x80080800, 0x80ff0001, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float stride[4] = {m_dx, m_dy, 0, 0};
				pDevice->SetPixelShaderConstantF( 0, stride, 1 );

				int count[4] = {max(0, min(255, m_count)), 0, 0, 0};
				pDevice->SetPixelShaderConstantI( 0, count, 1 );

				CSampler sampler;
				sampler.SetTexture( m_pTexture );
				sampler.SetAddressClamp();
				sampler.SetFilterPoint();
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_MotionBlur : public IPixelShader
		{
		public:
			int m_count;
			float m_dx;
			float m_dy;

			IDirect3DTexture9* m_pTexture;
		
		public:
			PS_MotionBlur()
			{
				m_count = 0;
				m_dx = 0;
				m_dy = 0;

				m_pTexture = 0;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0300, 0x00a8fffe, 0x47554244, 0x00000028, 0x00000268, 0x00000000, 0x00000001, 0x00000040, 
					0x00000011, 0x00000044, 0x00000006, 0x000001f0, 0x000000f0, 0x705c3a64, 0x6f6d5f73, 0x6e6f6974, 
					0x756c625f, 0x78742e72, 0xabab0074, 0x00000028, 0xffff0000, 0x000003bc, 0xffff0000, 0x000003d4, 
					0xffff0000, 0x000003e0, 0x00000026, 0x000003ec, 0x00000026, 0x000003f8, 0x00000028, 0x00000408, 
					0x00000028, 0x00000418, 0x00000028, 0x00000424, 0x00000028, 0x00000430, 0x0000002c, 0x00000438, 
					0x0000002c, 0x0000044c, 0x0000002c, 0x0000045c, 0x0000002a, 0x0000046c, 0x0000002a, 0x0000047c, 
					0x0000002a, 0x00000480, 0x0000002f, 0x0000048c, 0x0000002f, 0x00000498, 0x6e756f63, 0xabab0074, 
					0x00020000, 0x00010001, 0x00000001, 0x00000000, 0x00000003, 0xffffffff, 0x0000ffff, 0x6e69614d, 
					0x726c6300, 0xababab00, 0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x00000005, 0x00010000, 
					0x00030002, 0x00000006, 0x00010000, 0x00030002, 0x0000000b, 0x00010000, 0x00030002, 0x0000000e, 
					0x00010000, 0x00030002, 0x74636166, 0xab00726f, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 
					0x00000004, 0xffffffff, 0x0000ffff, 0xabab0069, 0x00000007, 0xffffffff, 0x0000ffff, 0x0000000c, 
					0xffffffff, 0x0000ffff, 0x75706e69, 0x61620074, 0xab006573, 0x00030001, 0x00020001, 0x00000001, 
					0x00000000, 0x00000182, 0x00000188, 0x00000005, 0x00020001, 0x00010001, 0x00000198, 0x00000001, 
					0x00010000, 0xffffffff, 0x7074756f, 0x64007475, 0x75666669, 0xab006573, 0x000001c3, 0x000000fc, 
					0x00000005, 0x00040001, 0x00010001, 0x000001cc, 0x00000010, 0x00010000, 0x00030002, 0x00000000, 
					0x000000cc, 0x000000d4, 0x00000001, 0x000000e4, 0x000000f0, 0x000000f5, 0x000000fc, 0x00000004, 
					0x0000010c, 0x000000f0, 0x0000013c, 0x00000144, 0x00000001, 0x00000154, 0x000000f0, 0x00000160, 
					0x000000d4, 0x00000002, 0x00000164, 0x000000f0, 0x0000017c, 0x000001a0, 0x00000001, 0x000001b0, 
					0x000000f0, 0x000001bc, 0x000001d4, 0x00000001, 0x000001e4, 0x7263694d, 0x666f736f, 0x52282074, 
					0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 
					0x34303932, 0xababab00, 0x0044fffe, 0x42415443, 0x0000001c, 0x000000db, 0xffff0300, 0x00000004, 
					0x0000001c, 0x00000101, 0x000000d4, 0x0000006c, 0x00000003, 0x00000001, 0x00000074, 0x00000000, 
					0x00000084, 0x00000002, 0x00000001, 0x0000008c, 0x0000009c, 0x00000084, 0x00000001, 0x00000001, 
					0x0000008c, 0x000000ac, 0x000000bc, 0x00010002, 0x00000001, 0x000000c4, 0x0000009c, 0x65736142, 
					0x00786554, 0x000c0004, 0x00010001, 0x00000001, 0x00000000, 0x6e756f63, 0xabab0074, 0x00020000, 
					0x00010001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
					0x00000000, 0x00000001, 0x00000000, 0x69727473, 0xab006564, 0x00030001, 0x00020001, 0x00000001, 
					0x00000000, 0x335f7370, 0x4d00305f, 0x6f726369, 0x74666f73, 0x29522820, 0x44334420, 0x53203958, 
					0x65646168, 0x6f432072, 0x6c69706d, 0x35207265, 0x2e34302e, 0x322e3030, 0x00343039, 0x05000051, 
					0xa00f0002, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x0200001f, 0x80000005, 0x90030000, 
					0x0200001f, 0x90000000, 0xa00f0800, 0x02000001, 0x80080000, 0xa0000000, 0x03000002, 0x80080004, 
					0x80ff0000, 0xa0000002, 0x03000042, 0x800f0003, 0x90e40000, 0xa0e40800, 0x02000001, 0x800f0001, 
					0x80e40003, 0x02000001, 0x80080000, 0xa0550002, 0x01000026, 0xf0e40000, 0x04000004, 0x80030000, 
					0x80ff0000, 0xa0e40001, 0x90e40000, 0x03000042, 0x800f0002, 0x80e40000, 0xa0e40800, 0x03000002, 
					0x800f0001, 0x80e40001, 0x80e40002, 0x03000002, 0x80080000, 0x80ff0000, 0xa0000002, 0x00000027, 
					0x02000001, 0x800f0000, 0x80e40001, 0x02000006, 0x80080001, 0x80ff0004, 0x03000005, 0x800f0800, 
					0x80e40000, 0x80ff0001, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float stride[8] = {float(m_count), 0, 0, 0, m_dx, m_dy, 0, 0};
				pDevice->SetPixelShaderConstantF( 0, stride, 2 );

				int count[4] = {max(0, min(255, m_count)), 0, 0, 0};
				pDevice->SetPixelShaderConstantI( 0, count, 1 );

				CSampler sampler;
				sampler.SetTexture( m_pTexture );
				sampler.SetAddressClamp();
				sampler.SetFilterLinear();
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_GaussinBlur : public IPixelShader
		{
		public:
			int m_count;
			float m_dx;
			float m_dy;

			IDirect3DTexture9* m_pTexture;
		
		public:
			PS_GaussinBlur()
			{
				m_count = 0;
				m_dx = 0;
				m_dy = 0;

				m_pTexture = 0;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0300, 0x009efffe, 0x47554244, 0x00000028, 0x00000240, 0x00000000, 0x00000001, 0x00000038, 
					0x00000015, 0x0000003c, 0x00000005, 0x000001dc, 0x000000e4, 0x705c3a64, 0x6c625f73, 0x742e7275, 
					0xab007478, 0x00000028, 0xffff0000, 0x00000394, 0xffff0000, 0x000003ac, 0xffff0000, 0x000003b8, 
					0x00000026, 0x000003c4, 0x00000026, 0x000003d4, 0x00000026, 0x000003e0, 0x00000026, 0x000003ec, 
					0x0000002b, 0x000003f4, 0x0000002b, 0x00000404, 0x0000002b, 0x00000418, 0x0000002b, 0x00000428, 
					0x0000002c, 0x00000438, 0x0000002c, 0x0000044c, 0x0000002c, 0x0000045c, 0x0000002c, 0x0000046c, 
					0x0000002c, 0x00000478, 0x0000002f, 0x0000047c, 0x0000002f, 0x00000488, 0x0000002f, 0x0000049c, 
					0x0000002f, 0x000004a8, 0x0000002f, 0x000004b8, 0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 
					0x00030001, 0x00040001, 0x00000001, 0x00000000, 0x000000e9, 0x000000f4, 0x00000005, 0x00040001, 
					0x00010001, 0x00000104, 0x00000013, 0x00010000, 0xffff0002, 0x00000014, 0xffffffff, 0x0003ffff, 
					0x6e756f63, 0xabab0074, 0x00020000, 0x00010001, 0x00000001, 0x00000000, 0x00000010, 0xffffffff, 
					0x0000ffff, 0x00726c63, 0x00000003, 0x00010000, 0x00030002, 0x00000004, 0x00010000, 0xffff0002, 
					0x0000000a, 0x00010000, 0xffff0002, 0x0000000d, 0x00010000, 0xffff0002, 0xabab0069, 0x00000005, 
					0xffffffff, 0x0000ffff, 0x75706e69, 0x61620074, 0xab006573, 0x00030001, 0x00020001, 0x00000001, 
					0x00000000, 0x000001a2, 0x000001a8, 0x00000005, 0x00020001, 0x00010001, 0x000001b8, 0x00000001, 
					0x00010000, 0xffffffff, 0x00000000, 0x000000e4, 0x0000010c, 0x00000002, 0x0000011c, 0x00000000, 
					0x00000134, 0x0000013c, 0x00000001, 0x0000014c, 0x000000e4, 0x00000158, 0x000000f4, 0x00000004, 
					0x0000015c, 0x000000e4, 0x0000018c, 0x0000013c, 0x00000001, 0x00000190, 0x000000e4, 0x0000019c, 
					0x000001c0, 0x00000001, 0x000001d0, 0x7263694d, 0x666f736f, 0x52282074, 0x33442029, 0x20395844, 
					0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 0x34303932, 0xababab00, 
					0x0044fffe, 0x42415443, 0x0000001c, 0x000000db, 0xffff0300, 0x00000004, 0x0000001c, 0x00000101, 
					0x000000d4, 0x0000006c, 0x00000003, 0x00000001, 0x00000074, 0x00000000, 0x00000084, 0x00000002, 
					0x00000001, 0x0000008c, 0x0000009c, 0x00000084, 0x00000001, 0x00000001, 0x0000008c, 0x000000ac, 
					0x000000bc, 0x00010002, 0x00000001, 0x000000c4, 0x0000009c, 0x65736142, 0x00786554, 0x000c0004, 
					0x00010001, 0x00000001, 0x00000000, 0x6e756f63, 0xabab0074, 0x00020000, 0x00010001, 0x00000001, 
					0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 
					0x00000000, 0x69727473, 0xab006564, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x335f7370, 
					0x4d00305f, 0x6f726369, 0x74666f73, 0x29522820, 0x44334420, 0x53203958, 0x65646168, 0x6f432072, 
					0x6c69706d, 0x35207265, 0x2e34302e, 0x322e3030, 0x00343039, 0x05000051, 0xa00f0002, 0x00000000, 
					0x3f800000, 0x40000000, 0x00000000, 0x0200001f, 0x80000005, 0x90030000, 0x0200001f, 0x90000000, 
					0xa00f0800, 0x03000042, 0x800f0001, 0x90e40000, 0xa0e40800, 0x02000001, 0x80070000, 0x80e40001, 
					0x02000001, 0x80080000, 0xa0000002, 0x01000026, 0xf0e40000, 0x03000002, 0x80080003, 0x80ff0000, 
					0xa0550002, 0x04000004, 0x80030002, 0x80ff0003, 0xa0e40001, 0x90e40000, 0x03000042, 0x800f0002, 
					0x80e40002, 0xa0e40800, 0x03000002, 0x80070002, 0x80e40000, 0x80e40002, 0x04000004, 0x80030000, 
					0x80ff0003, 0xa1e40001, 0x90e40000, 0x03000042, 0x800f0000, 0x80e40000, 0xa0e40800, 0x03000002, 
					0x80070000, 0x80e40002, 0x80e40000, 0x02000001, 0x80080000, 0x80ff0003, 0x00000027, 0x02000001, 
					0x80080000, 0xa0000000, 0x04000004, 0x80080000, 0xa0aa0002, 0x80ff0000, 0xa0550002, 0x02000006, 
					0x80080000, 0x80ff0000, 0x03000005, 0x80070800, 0x80e40000, 0x80ff0000, 0x02000001, 0x80080800, 
					0x80ff0001, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float stride[8] = {float(m_count), 0, 0, 0, m_dx, m_dy, 0, 0};
				pDevice->SetPixelShaderConstantF( 0, stride, 2 );

				int count[4] = {max(0, min(255, m_count)), 0, 0, 0};
				pDevice->SetPixelShaderConstantI( 0, count, 1 );

				CSampler sampler;
				sampler.SetTexture( m_pTexture );
				sampler.SetAddressClamp();
				sampler.SetFilterPoint();
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_GlassMosaic : public IPixelShader
		{
		public:
			int m_samples;
			float m_level;
			float m_smooth;
			float m_curvature;
			float m_rescale;
			float m_factorX;
			float m_factorY;
			float m_scaleX;
			float m_scaleY;
			float m_dispX;
			float m_dispY;
			float m_width;
			float m_height;
			float m_sin;
			float m_cos;

			IDirect3DTexture9* m_pTexture;
		
		public:
			PS_GlassMosaic()
			{
				m_samples = 1;
				m_level   = 1;
				m_smooth  = 1;
				m_curvature = 1;
				m_rescale = 1;
				m_factorX = 1;
				m_factorY = 1;
				m_scaleX  = 1;
				m_scaleY  = 1;
				m_dispX   = 0;
				m_dispY   = 0;
				m_width   = 1;
				m_height  = 1;
				m_sin     = 0;
				m_cos     = 1;

				m_pTexture = 0;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0300, 0x0100fffe, 0x47554244, 0x00000028, 0x000003c8, 0x00000000, 0x00000001, 0x00000040, 
					0x0000002e, 0x00000044, 0x00000008, 0x00000328, 0x000001d8, 0x705c3a64, 0x6c675f73, 0x5f737361, 
					0x61736f6d, 0x742e6369, 0xab007478, 0x00000028, 0xffff0000, 0x00000610, 0xffff0000, 0x00000628, 
					0xffff0000, 0x00000640, 0xffff0000, 0x00000658, 0xffff0000, 0x00000670, 0xffff0000, 0x0000067c, 
					0x00000037, 0x00000688, 0x00000037, 0x00000698, 0x00000037, 0x000006a8, 0x00000037, 0x000006b4, 
					0x00000037, 0x000006c0, 0x0000003b, 0x000006c8, 0x0000003c, 0x000006d8, 0x0000003f, 0x000006e4, 
					0x0000003f, 0x000006f4, 0x00000041, 0x00000708, 0x00000044, 0x0000071c, 0x00000044, 0x00000728, 
					0x00000043, 0x0000073c, 0x00000046, 0x00000750, 0x00000046, 0x00000764, 0x00000046, 0x00000770, 
					0x00000046, 0x00000780, 0x00000048, 0x00000790, 0x00000048, 0x000007a4, 0x00000048, 0x000007b0, 
					0x00000048, 0x000007c4, 0x00000048, 0x000007d4, 0x00000048, 0x000007e8, 0x00000048, 0x000007fc, 
					0x00000048, 0x00000810, 0x00000048, 0x00000824, 0x00000048, 0x00000838, 0x00000048, 0x00000844, 
					0x00000048, 0x00000850, 0x00000048, 0x00000860, 0x0000004b, 0x00000874, 0x0000004a, 0x00000888, 
					0x0000004d, 0x0000089c, 0x0000004d, 0x000008a8, 0x0000004d, 0x000008bc, 0x0000004d, 0x000008cc, 
					0x00000039, 0x000008dc, 0x00000039, 0x000008ec, 0x00000039, 0x000008f0, 0x00000050, 0x000008fc, 
					0x70736964, 0xababab00, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x00000026, 0x00010000, 
					0xffffffff, 0x6e69614d, 0xab006900, 0x00020000, 0x00010001, 0x00000001, 0x00000000, 0x00000009, 
					0xffffffff, 0x0000ffff, 0x0000002a, 0xffffffff, 0x0000ffff, 0x75706e69, 0x61620074, 0xab006573, 
					0x0000020e, 0x000001bc, 0x00000005, 0x00020001, 0x00010001, 0x00000214, 0x00000004, 0x00010000, 
					0xffffffff, 0x7074756f, 0x64007475, 0x75666669, 0xab006573, 0x00030001, 0x00040001, 0x00000001, 
					0x00000000, 0x0000023f, 0x00000248, 0x00000005, 0x00040001, 0x00010001, 0x00000258, 0x0000002d, 
					0x00010000, 0x00030002, 0xab003070, 0x00000007, 0x00010000, 0xffffffff, 0xab003170, 0x0000000b, 
					0x00010000, 0xffffffff, 0x0000000c, 0xffffffff, 0xffff0000, 0x0000000f, 0x00010000, 0xffffffff, 
					0x00000024, 0x0001ffff, 0xffffffff, 0x00000025, 0xffff0000, 0xffffffff, 0xab003270, 0x0000000e, 
					0x00010000, 0xffffffff, 0x00000011, 0x0001ffff, 0xffffffff, 0x00000012, 0xffff0000, 0xffffffff, 
					0x00000023, 0x00010000, 0xffffffff, 0x006d7573, 0x00000008, 0x00010000, 0x00030002, 0x00000029, 
					0x00010000, 0x00030002, 0x0000002c, 0x00010000, 0x00030002, 0x00000000, 0x000001b4, 0x000001bc, 
					0x00000001, 0x000001cc, 0x000001d8, 0x000001dd, 0x000001e0, 0x00000002, 0x000001f0, 0x000001d8, 
					0x00000208, 0x0000021c, 0x00000001, 0x0000022c, 0x000001d8, 0x00000238, 0x00000260, 0x00000001, 
					0x00000270, 0x000001d8, 0x0000027c, 0x000001bc, 0x00000001, 0x00000280, 0x000001d8, 0x0000028c, 
					0x000001bc, 0x00000005, 0x00000290, 0x000001d8, 0x000002cc, 0x000001bc, 0x00000004, 0x000002d0, 
					0x000001d8, 0x00000300, 0x00000248, 0x00000003, 0x00000304, 0x7263694d, 0x666f736f, 0x52282074, 
					0x33442029, 0x20395844, 0x64616853, 0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 
					0x34303932, 0xababab00, 0x0081fffe, 0x42415443, 0x0000001c, 0x000001cd, 0xffff0300, 0x0000000b, 
					0x0000001c, 0x00000101, 0x000001c6, 0x000000f8, 0x00000003, 0x00000001, 0x00000100, 0x00000000, 
					0x00000110, 0x00000002, 0x00000001, 0x00000118, 0x00000128, 0x00000138, 0x00060002, 0x00000001, 
					0x00000118, 0x00000140, 0x00000150, 0x00080002, 0x00000001, 0x00000118, 0x00000128, 0x00000157, 
					0x00010002, 0x00000001, 0x00000118, 0x00000128, 0x0000015d, 0x00050002, 0x00000001, 0x00000118, 
					0x00000128, 0x00000164, 0x00070002, 0x00000001, 0x0000016c, 0x0000017c, 0x0000018c, 0x00000001, 
					0x00000001, 0x00000194, 0x000001a4, 0x000001b4, 0x00020002, 0x00000001, 0x00000118, 0x00000128, 
					0x000001ba, 0x00040002, 0x00000001, 0x00000118, 0x00000128, 0x000001bf, 0x00030002, 0x00000001, 
					0x00000118, 0x00000128, 0x65736142, 0x00786554, 0x000c0004, 0x00010001, 0x00000001, 0x00000000, 
					0x76727563, 0xababab00, 0x00030001, 0x00020001, 0x00000001, 0x00000000, 0x3f800000, 0x3f800000, 
					0x00000000, 0x00000000, 0x70736964, 0xababab00, 0x3f000000, 0x3f000000, 0x00000000, 0x00000000, 
					0x74636166, 0x6c00726f, 0x6c657665, 0x73657200, 0x00657a69, 0x61746f72, 0xab006574, 0x00030001, 
					0x00040001, 0x00000001, 0x00000000, 0x3f800000, 0x00000000, 0x00000000, 0x3f800000, 0x706d6173, 
					0x0073656c, 0x00020000, 0x00010001, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 
					0x00000000, 0x6c616373, 0x69730065, 0x7300657a, 0x746f6f6d, 0x73700068, 0x305f335f, 0x63694d00, 
					0x6f736f72, 0x28207466, 0x44202952, 0x39584433, 0x61685320, 0x20726564, 0x706d6f43, 0x72656c69, 
					0x302e3520, 0x30302e34, 0x3039322e, 0xabab0034, 0x05000051, 0xa00f0009, 0xbf000000, 0x3f800000, 
					0x00000000, 0x00000000, 0x05000051, 0xa00f000a, 0x00000000, 0x3f000000, 0xbf000000, 0xbab609ba, 
					0x05000051, 0xa00f000b, 0x3f000000, 0x3e800000, 0x3f000000, 0x3d2aaaa4, 0x05000051, 0xa00f000c, 
					0x40c90fdb, 0xc0490fdb, 0xb4878163, 0x37cfb5a1, 0x0200001f, 0x80000005, 0x90030000, 0x0200001f, 
					0x90000000, 0xa00f0800, 0x03000002, 0x80030000, 0x90e40000, 0xa1e40006, 0x03000005, 0x80030005, 
					0x80e40000, 0xa0e40004, 0x02000001, 0x800f0001, 0xa000000a, 0x02000001, 0x80080000, 0xa000000a, 
					0x01000026, 0xf0e40000, 0x03000005, 0x80030000, 0x80ff0000, 0xa0e40001, 0x02000013, 0x80040000, 
					0x80000000, 0x03000005, 0x80030000, 0x80550000, 0xa0ed0007, 0x04000004, 0x80030000, 0x80aa0000, 
					0xa0e80007, 0x80e40000, 0x04000004, 0x80030002, 0x80e40000, 0xa0e40003, 0x80e40005, 0x02000001, 
					0x80030000, 0xa0e4000a, 0x0400005a, 0x80020004, 0x80e40002, 0xa0ee0007, 0x80000000, 0x0400005a, 
					0x80010004, 0x80e40002, 0xa0e40007, 0x80000000, 0x04000004, 0x800f0002, 0x80440004, 0xa0440002, 
					0x80550000, 0x02000013, 0x800f0002, 0x80e40002, 0x03000002, 0x800f0002, 0x80e40002, 0xa0aa000a, 
					0x03000005, 0x800f0002, 0x80e40002, 0xa0440008, 0x04000004, 0x800f0002, 0x80e40002, 0xa000000b, 
					0xa0a5000b, 0x02000013, 0x800f0002, 0x80e40002, 0x04000004, 0x800f0002, 0x80e40002, 0xa000000c, 
					0xa055000c, 0x03000005, 0x800f0002, 0x80e40002, 0x80e40002, 0x04000004, 0x800f0003, 0x80e40002, 
					0xa0aa000c, 0xa0ff000c, 0x04000004, 0x800f0003, 0x80e40002, 0x80e40003, 0xa0ff000a, 0x04000004, 
					0x800f0003, 0x80e40002, 0x80e40003, 0xa0ff000b, 0x04000004, 0x800f0003, 0x80e40002, 0x80e40003, 
					0xa0000009, 0x04000004, 0x800f0002, 0x80e40002, 0x80e40003, 0xa0550009, 0x02000006, 0x80010003, 
					0x80aa0002, 0x02000006, 0x80020003, 0x80ff0002, 0x03000005, 0x80030002, 0x80e40002, 0x80e40003, 
					0x04000004, 0x80030002, 0xa0e40000, 0x80e40002, 0x80e40004, 0x0400005a, 0x80020000, 0xa0ed0007, 
					0x80e40002, 0x80000000, 0x0400005a, 0x80010000, 0xa0e80007, 0x80e40002, 0x80000000, 0x02000001, 
					0x80030002, 0xa0e40006, 0x04000004, 0x80030000, 0x80e40000, 0xa0e40005, 0x80e40002, 0x03000042, 
					0x800f0002, 0x80e40000, 0xa0e40800, 0x03000002, 0x800f0001, 0x80e40001, 0x80e40002, 0x03000002, 
					0x80080000, 0x80ff0000, 0xa0550009, 0x00000027, 0x02000001, 0x800f0000, 0x80e40001, 0x03000005, 
					0x800f0800, 0x80e40000, 0xa0550001, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				int samples[4] = {m_samples, 0, 0, 0};
				float curv  [4] = {m_curvature, m_curvature, 0, 0};
				float level [4] = {float(m_level / m_samples), float(1.0 / m_samples), 0, 0};
				float scale [4] = {m_scaleX , m_scaleY, 0, 0};
				float smooth[4] = {m_smooth, m_smooth, 0, 0};
				float size  [4] = {m_width, m_height, 0, 0};
				float resize[4] = {1 / (m_width * CheckScale(m_rescale)), 1 / (m_height * CheckScale(m_rescale)), 0, 0};
				float disp  [4] = {0.5f + m_dispX / m_width, 0.5f - m_dispY / m_height, 0, 0};
				float rotate[4] = {m_cos, m_sin, -m_sin, m_cos};
				float factor[4] = {m_factorX, m_factorY, 0, 0};
				
				pDevice->SetPixelShaderConstantI( 0, samples, 1 );
				pDevice->SetPixelShaderConstantF( 0, curv,   1 );
				pDevice->SetPixelShaderConstantF( 1, level,  1 );
				pDevice->SetPixelShaderConstantF( 2, scale,  1 );
				pDevice->SetPixelShaderConstantF( 3, smooth, 1 );
				pDevice->SetPixelShaderConstantF( 4, size,   1 );
				pDevice->SetPixelShaderConstantF( 5, resize, 1 );
				pDevice->SetPixelShaderConstantF( 6, disp,   1 );
				pDevice->SetPixelShaderConstantF( 7, rotate, 1 );
				pDevice->SetPixelShaderConstantF( 8, factor, 1 );

				CSampler sampler;
				sampler.SetTexture( m_pTexture );
				sampler.SetAddressMirrow();
				sampler.SetFilterPoint();
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		class PS_Newsprint : public IPixelShader
		{
		public:
			float m_level;
			float m_alpha;
			float m_width;
			float m_height;

			IDirect3DTexture9* m_pTexture;
		
		public:
			PS_Newsprint()
			{
				m_level  = 1;
				m_alpha  = 1;
				m_width  = 1;
				m_height = 1;

				m_pTexture = 0;
			}

			
			static const DWORD* GetStaticCode()
			{
				static const DWORD shader[] = {
					0xffff0300, 0x00cdfffe, 0x47554244, 0x00000028, 0x000002fc, 0x00000000, 0x00000001, 0x0000003c, 
					0x00000024, 0x00000040, 0x00000005, 0x00000298, 0x00000160, 0x705c3a64, 0x656e5f73, 0x72707377, 
					0x2e746e69, 0x00747874, 0x00000028, 0xffff0000, 0x000004c4, 0xffff0000, 0x000004dc, 0xffff0000, 
					0x000004f4, 0xffff0000, 0x00000500, 0x0000002c, 0x0000050c, 0x0000002c, 0x0000051c, 0x0000002c, 
					0x0000052c, 0x0000002c, 0x0000053c, 0x0000002c, 0x0000054c, 0x0000002c, 0x0000055c, 0x0000002c, 
					0x0000056c, 0x00000027, 0x0000057c, 0x0000002c, 0x0000058c, 0x0000002c, 0x000005a0, 0x0000002c, 
					0x000005b4, 0x0000002c, 0x000005c8, 0x0000002c, 0x000005d8, 0x0000002c, 0x000005e8, 0x0000002c, 
					0x000005fc, 0x0000002c, 0x0000060c, 0x0000002c, 0x0000061c, 0x0000002c, 0x00000630, 0x0000002c, 
					0x00000640, 0x0000002c, 0x00000650, 0x0000002c, 0x00000664, 0x0000002c, 0x00000674, 0x0000002c, 
					0x00000684, 0x0000002c, 0x00000698, 0x0000002c, 0x000006a8, 0x0000002c, 0x000006b8, 0x00000033, 
					0x000006cc, 0x00000034, 0x000006dc, 0x00000034, 0x000006ec, 0x00000036, 0x00000700, 0x00000036, 
					0x00000710, 0x00000036, 0x00000724, 0x6e69614d, 0x66696400, 0x65737566, 0xababab00, 0x00030001, 
					0x00040001, 0x00000001, 0x00000000, 0x00000165, 0x00000170, 0x00000005, 0x00040001, 0x00010001, 
					0x00000180, 0x00000022, 0x00010000, 0xffff0002, 0x00000023, 0xffffffff, 0x0003ffff, 0x00667562, 
					0x0000000b, 0x00010000, 0x00030002, 0x00726c63, 0x0000000c, 0x00010000, 0xffff0002, 0x0000000d, 
					0x00010000, 0xffff0002, 0x0000000e, 0x00010000, 0xffff0002, 0x00000011, 0x00010000, 0xffff0002, 
					0x00000014, 0x00010000, 0xffff0002, 0x00000017, 0x00010000, 0xffff0002, 0x0000001a, 0x00010000, 
					0xffff0002, 0x0000001d, 0x00010000, 0xffff0002, 0x75706e69, 0x61620074, 0xab006573, 0x00030001, 
					0x00020001, 0x00000001, 0x00000000, 0x0000022a, 0x00000230, 0x00000005, 0x00020001, 0x00010001, 
					0x00000240, 0x00000002, 0x00010000, 0xffffffff, 0x65746e69, 0x7469736e, 0xabab0079, 0x00030000, 
					0x00010001, 0x00000001, 0x00000000, 0x0000001e, 0xffffffff, 0x0000ffff, 0x00000020, 0xffffffff, 
					0x0000ffff, 0x00000000, 0x00000160, 0x00000188, 0x00000002, 0x00000198, 0x00000160, 0x000001b0, 
					0x00000170, 0x00000001, 0x000001b4, 0x00000160, 0x000001c0, 0x00000170, 0x00000008, 0x000001c4, 
					0x00000160, 0x00000224, 0x00000248, 0x00000001, 0x00000258, 0x00000160, 0x00000264, 0x00000270, 
					0x00000002, 0x00000280, 0x7263694d, 0x666f736f, 0x52282074, 0x33442029, 0x20395844, 0x64616853, 
					0x43207265, 0x69706d6f, 0x2072656c, 0x34302e35, 0x2e30302e, 0x34303932, 0xababab00, 0x0061fffe, 
					0x42415443, 0x0000001c, 0x0000014f, 0xffff0300, 0x00000004, 0x0000001c, 0x00000101, 0x00000148, 
					0x0000006c, 0x00000003, 0x00000001, 0x00000074, 0x00000000, 0x00000084, 0x00090002, 0x00000001, 
					0x0000008c, 0x0000009c, 0x000000ac, 0x00080002, 0x00000001, 0x0000008c, 0x0000009c, 0x000000b2, 
					0x00000002, 0x00000008, 0x000000b8, 0x000000c8, 0x65736142, 0x00786554, 0x000c0004, 0x00010001, 
					0x00000001, 0x00000000, 0x68706c61, 0xabab0061, 0x00030000, 0x00010001, 0x00000001, 0x00000000, 
					0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x6576656c, 0x7470006c, 0xababab00, 0x00030001, 
					0x00030001, 0x00000008, 0x00000000, 0x00000000, 0x00000000, 0x3f800000, 0x00000000, 0x00000000, 
					0x00000000, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x3f800000, 0x00000000, 0x00000000, 
					0x00000000, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x3f800000, 0x00000000, 0x00000000, 
					0x00000000, 0x3f800000, 0x00000000, 0x00000000, 0x00000000, 0x3f800000, 0x00000000, 0x00000000, 
					0x00000000, 0x3f800000, 0x00000000, 0x335f7370, 0x4d00305f, 0x6f726369, 0x74666f73, 0x29522820, 
					0x44334420, 0x53203958, 0x65646168, 0x6f432072, 0x6c69706d, 0x35207265, 0x2e34302e, 0x322e3030, 
					0x00343039, 0x05000051, 0xa00f000a, 0x41f00000, 0x3e991687, 0x3f1645a2, 0x3de978d5, 0x05000051, 
					0xa00f000b, 0x80000000, 0x3f800000, 0x00000000, 0x00000000, 0x0200001f, 0x80000005, 0x90030000, 
					0x0200001f, 0x90000000, 0xa00f0800, 0x03000002, 0x80030000, 0x90e40000, 0xa0e40002, 0x03000042, 
					0x800f0001, 0x80e40000, 0xa0e40800, 0x03000002, 0x80030000, 0x90e40000, 0xa0e40001, 0x03000042, 
					0x800f0002, 0x80e40000, 0xa0e40800, 0x03000002, 0x80030000, 0x90e40000, 0xa0e40000, 0x03000042, 
					0x800f0000, 0x80e40000, 0xa0e40800, 0x03000005, 0x80070003, 0x80e40000, 0xa0aa0000, 0x03000042, 
					0x800f0000, 0x90e40000, 0xa0e40800, 0x04000004, 0x80070003, 0x80e40000, 0xa000000a, 0x80e40003, 
					0x04000004, 0x80070002, 0x80e40002, 0xa0aa0001, 0x80e40003, 0x04000004, 0x80070002, 0x80e40001, 
					0xa0aa0002, 0x80e40002, 0x03000002, 0x80030001, 0x90e40000, 0xa0e40003, 0x03000042, 0x800f0001, 
					0x80e40001, 0xa0e40800, 0x04000004, 0x80070002, 0x80e40001, 0xa0aa0003, 0x80e40002, 0x03000002, 
					0x80030001, 0x90e40000, 0xa0e40004, 0x03000042, 0x800f0001, 0x80e40001, 0xa0e40800, 0x04000004, 
					0x80070002, 0x80e40001, 0xa0aa0004, 0x80e40002, 0x03000002, 0x80030001, 0x90e40000, 0xa0e40005, 
					0x03000042, 0x800f0001, 0x80e40001, 0xa0e40800, 0x04000004, 0x80070002, 0x80e40001, 0xa0aa0005, 
					0x80e40002, 0x03000002, 0x80030001, 0x90e40000, 0xa0e40006, 0x03000042, 0x800f0001, 0x80e40001, 
					0xa0e40800, 0x04000004, 0x80070002, 0x80e40001, 0xa0aa0006, 0x80e40002, 0x03000002, 0x80030001, 
					0x90e40000, 0xa0e40007, 0x03000042, 0x800f0001, 0x80e40001, 0xa0e40800, 0x04000004, 0x80170001, 
					0x80e40001, 0xa0aa0007, 0x80e40002, 0x03000008, 0x80080001, 0x80e40001, 0xa0f9000a, 0x03000002, 
					0x80080001, 0x81ff0001, 0xa0000008, 0x04000058, 0x80080001, 0x80ff0001, 0xa000000b, 0xa055000b, 
					0x03000002, 0x80070001, 0x81e40000, 0x80ff0001, 0x04000004, 0x80070800, 0xa0000009, 0x80e40001, 
					0x80e40000, 0x02000001, 0x80080800, 0x80ff0000, 0x0000ffff
				};

				return shader;
			}

		
			virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
			{
				float w = 1 / m_width;
				float h = 1 / m_height;

				float pt[8][4] = {
					w * (-1.5f), h * (-1.5f), -4, 0,
					w * (    0), h * (-1.5f), -2, 0,
					w * ( 1.5f), h * (-1.5f), -4, 0,
					w * (-1.5f), h * ( 0.5f), -4, 0,
					w * ( 1.5f), h * ( 0.5f), -4, 0,
					w * (-1.5f), h * ( 2.0f), -2, 0,
					w * (    0), h * (2.0f - 1.0f / 6.0f), -6, 0,
					w * ( 1.5f), h * ( 2.0f), -2, 0
				};

				float level[4] = {m_level, 0, 0, 0};
				float alpha[4] = {m_alpha, 0, 0, 0};

				pDevice->SetPixelShaderConstantF( 0, &pt[0][0], 8 );
				pDevice->SetPixelShaderConstantF( 8, level, 1 );
				pDevice->SetPixelShaderConstantF( 9, alpha, 1 );

				CSampler sampler;
				sampler.SetTexture( m_pTexture );
				sampler.SetAddressMirrow();
				sampler.SetFilterLinear();
				sampler.SetToDevice( pDevice, 0 );
				
				return TRUE;
			};

			virtual const DWORD* GetCode() const
			{
				return GetStaticCode();
			}

		};

		
		namespace Test
		{
			class ITestShader : public IPixelShader
			{
			protected:
				ID3DXBuffer* m_pCode;
				ID3DXBuffer* m_pErrorMsg;
				ID3DXConstantTable* m_pConstTable;

			public:			
				LPCTSTR m_szSrcFile;
				LPCSTR	m_szProfile;
				LPCSTR  m_szMainFunc;
				DWORD   m_dwFlags;

				LPCTSTR m_szCodeFile;
				LPCTSTR m_szAsmFile;

			public:
				ITestShader( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
				{
					m_pCode       = NULL;
					m_pErrorMsg   = NULL;
					m_pConstTable = NULL;

					m_szSrcFile   = szSrcFile;
					m_szProfile   = szProfile;
					m_szMainFunc  = "Main";
					m_dwFlags     = D3DXSHADER_DEBUG;

					m_szCodeFile  = _T("d:\\code.txt");
					m_szAsmFile   = _T("d:\\asm.txt");
				}

				~ITestShader()
				{
					Release();
				}

				void Release()
				{
					RELEASEINTERFACE(m_pCode);
					RELEASEINTERFACE(m_pErrorMsg);
					RELEASEINTERFACE(m_pConstTable);
					RELEASEINTERFACE(m_pShader);
				}

				
				virtual const DWORD* GetCode() const
				{
					if( !m_pCode )
						return NULL;

					return (LPDWORD)m_pCode->GetBufferPointer();
				}

				LPCSTR GetErrorMsg() const
				{
					if( !m_pErrorMsg )
						return NULL;

					return (LPCSTR)m_pErrorMsg->GetBufferPointer();
				}

				ID3DXConstantTable* GetTable() const
				{
					return m_pConstTable;
				}

				BOOL CheckDefaults( IDirect3DDevice9* pDevice ) const
				{
					if( !pDevice || !m_pConstTable )
						return FALSE;
					
					HRESULT hr = m_pConstTable->SetDefaults( pDevice );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						return FALSE;
					}
					return TRUE;
				}

				
				BOOL Create( IDirect3DDevice9* pDevice )
				{
					Release();

					if( !pDevice )
						return FALSE;

					HRESULT hr = D3DXCompileShaderFromFile( m_szSrcFile, 0, 0, m_szMainFunc, m_szProfile, m_dwFlags, &m_pCode, &m_pErrorMsg, &m_pConstTable );

					LPCSTR msg = GetErrorMsg();
					
					if( FAILED(hr) || msg )
					{
						DXTraceA( __FILE__, (DWORD)__LINE__, hr, msg, FALSE );
						return FALSE;
					}

					const DWORD* pCode = GetCode();
					if( !pCode )
						return NULL;

					hr = pDevice->CreatePixelShader( pCode, &m_pShader );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						return FALSE;
					}

					CodeToFile( m_szCodeFile );
					Disassemble( m_szAsmFile );

					return TRUE;
				}

			
			private:
				BOOL CodeToFile( LPCTSTR szFileName ) const
				{
					if( !m_pCode )
						return FALSE;

					FILE* pFile = ::_tfopen( szFileName, _T("wb") );
					if( !pFile )
						return FALSE;

					LPCSTR szPrefix = "static const DWORD shader[] = {";
					::fwrite( szPrefix, ::strlen(szPrefix), 1, pFile ); 

					DWORD dwBufSize = m_pCode->GetBufferSize();
					BYTE* pBuffer = (BYTE*)m_pCode->GetBufferPointer();

					BYTE rn[2] = {0x0d, 0x0a}; // перевод строки

					BYTE bt[12];
					bt[0] = '0';
					bt[1] = 'x';
					bt[10] = ',';
					bt[11] = ' ';

					for( DWORD i = 0; i < dwBufSize; i += 4, pBuffer += 4 )
					{
						if( !(i & 31) )
						{
							::fwrite( rn, 2, 1, pFile );
						}

						bt[2] = (pBuffer[3] >> 4); bt[2] += (bt[2] < 10) ? '0' : 'a' - 10;
						bt[3] = (pBuffer[3] & 15); bt[3] += (bt[3] < 10) ? '0' : 'a' - 10;
						bt[4] = (pBuffer[2] >> 4); bt[4] += (bt[4] < 10) ? '0' : 'a' - 10;
						bt[5] = (pBuffer[2] & 15); bt[5] += (bt[5] < 10) ? '0' : 'a' - 10;
						bt[6] = (pBuffer[1] >> 4); bt[6] += (bt[6] < 10) ? '0' : 'a' - 10;
						bt[7] = (pBuffer[1] & 15); bt[7] += (bt[7] < 10) ? '0' : 'a' - 10;
						bt[8] = (pBuffer[0] >> 4); bt[8] += (bt[8] < 10) ? '0' : 'a' - 10;
						bt[9] = (pBuffer[0] & 15); bt[9] += (bt[9] < 10) ? '0' : 'a' - 10;

						::fwrite( bt, 12, 1, pFile );
					}

					::fwrite( rn, 2, 1, pFile );

					LPCSTR szPosfix = "};";
					::fwrite( szPosfix, strlen(szPosfix), 1, pFile );

					::fwrite( rn, 2, 1, pFile );
					::fwrite( rn, 2, 1, pFile );

					::fprintf( pFile, "size code = %d bytes", dwBufSize );				

					::fclose( pFile );

					return TRUE;
				}

				BOOL Disassemble( LPCTSTR szFileName ) const
				{
					if( !m_pCode )
						return FALSE;

					ID3DXBuffer* pDisasm = NULL;
					D3DXDisassembleShader( GetCode(), FALSE, NULL, &pDisasm );
					if( !pDisasm )
						return FALSE;

					FILE* pFile = ::_tfopen( szFileName, _T("wb") );
					if( !pFile )
					{
						pDisasm->Release();
						return FALSE;
					}

					::fwrite( pDisasm->GetBufferPointer(), pDisasm->GetBufferSize(), 1, pFile );
					::fclose( pFile );

					pDisasm->Release();

					return TRUE;
				}

				
			protected:
				BOOL SetConstant( IDirect3DDevice9* pDevice, LPCSTR szName, float fValue ) const
				{
					D3DXHANDLE hHandle = m_pConstTable->GetConstantByName( 0, szName );
					if( !hHandle )
					{
						DXTRACE_MSG(_T("Can`t set constant to shader!"));
						return FALSE;
					}

					HRESULT hr = m_pConstTable->SetFloat( pDevice, hHandle, fValue );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						return FALSE;
					}

					return TRUE;
				}

				BOOL SetConstant( IDirect3DDevice9* pDevice, LPCSTR szName, const float* pValues, DWORD dwCount ) const
				{
					D3DXHANDLE hHandle = m_pConstTable->GetConstantByName( 0, szName );
					if( !hHandle )
					{
						DXTRACE_MSG(_T("Can`t set constant to shader!"));
						return FALSE;
					}

					HRESULT hr = m_pConstTable->SetFloatArray( pDevice, hHandle, pValues, dwCount );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						return FALSE;
					}

					return TRUE;
				}

				BOOL SetConstant( IDirect3DDevice9* pDevice, LPCSTR szName, int nValue ) const
				{
					D3DXHANDLE hHandle = m_pConstTable->GetConstantByName( 0, szName );
					if( !hHandle )
					{
						DXTRACE_MSG(_T("Can`t set constant to shader!"));
						return FALSE;
					}

					HRESULT hr = m_pConstTable->SetInt( pDevice, hHandle, nValue );
					if( FAILED(hr) )
					{
						DXTRACE_ERR(0, hr);
						return FALSE;
					}

					return TRUE;
				}

			};

			
			class Test_PS_Ripple : public ITestShader
			{
			public:
				float m_fOmega;
				float m_fDisp;
				float m_fAmpl;
				float m_fWidth;
				float m_fHeight;

				IDirect3DTexture9* m_pTexture;
				CSampler::EFilter  m_eFilter;

			public:
				Test_PS_Ripple( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_2_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_fOmega   = 0;
					m_fDisp    = 0;
					m_fAmpl    = 0;
					m_fWidth   = 0;
					m_fHeight  = 0;
					m_pTexture = 0;
					m_eFilter  = CSampler::c_nDefault;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float ampl[2] = {m_fAmpl / m_fWidth, m_fAmpl / m_fHeight};
					float size[4] = {m_fWidth, m_fHeight, 0.5f / m_fWidth, 0.5f / m_fHeight};

					SetConstant( pDevice, "omega", m_fOmega );
					SetConstant( pDevice, "disp",  -m_fDisp );
					SetConstant( pDevice, "ampl", ampl, 2 );
					SetConstant( pDevice, "size", size, 4 );

					CSampler sampler;
					sampler.SetTexture( m_pTexture );
					sampler.SetFilter( m_eFilter );
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}
			};
			class Test_PS_Twirl : public ITestShader
			{
			public:
				float m_fAngle;
				float m_fDegree;
				float m_fZoom;
				float m_fWidth;
				float m_fHeight;

				D3DCOLOR m_nBackColor;
				IDirect3DTexture9* m_pTexture;
				CSampler::EFilter  m_eFilter;

			public:
				Test_PS_Twirl( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_fAngle   = 0;
					m_fDegree  = 0;
					m_fZoom    = 1;
					m_fWidth   = 0;
					m_fHeight  = 0;
					m_nBackColor = 0;
					m_pTexture = 0;
					m_eFilter  = CSampler::c_nDefault;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float angle  = m_fAngle;
					float degree = m_fDegree;
					float zoom   = CheckScale( m_fZoom );
					float w      = m_fWidth;
					float h      = m_fHeight;
					float factor = float(-2 / sqrt(double(w * w + h * h)));

					float size1[4] = {w, h, -w/2, -h/2};
					float size2[4] = {1/(zoom * w), 1/(zoom * h), (1/w + 1)/2, (1/h + 1)/2};

					SetConstant( pDevice, "angle", angle );
					SetConstant( pDevice, "degree", degree );
					SetConstant( pDevice, "factor", factor );
					SetConstant( pDevice, "size1", size1, 4 );
					SetConstant( pDevice, "size2", size2, 4 );

					CSampler sampler;
					sampler.SetAddressBorder( m_nBackColor );
					sampler.SetTexture( m_pTexture );
					sampler.SetFilter( m_eFilter );
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_Zoom : public ITestShader
			{
			public:
				float m_fZoomX;
				float m_fZoomY;
				float m_fDispX;
				float m_fDispY;
				float m_fWidth;
				float m_fHeight;

				D3DCOLOR m_nBackColor;
				IDirect3DTexture9* m_pTexture;
				CSampler::EFilter  m_eFilter;
			
			public:
				Test_PS_Zoom( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_fZoomX   = 1;
					m_fZoomY   = 1;
					m_fDispX   = 0;
					m_fDispY   = 0;
					m_fWidth   = 1;
					m_fHeight  = 1;
					m_nBackColor = 0;
					m_pTexture = 0;
					m_eFilter  = CSampler::c_nDefault;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float zx = CheckScale(m_fZoomX);
					float zy = CheckScale(m_fZoomY);

					// правочные смещения
					float dx = 0.5f / m_fWidth;
					float dy = 0.5f / m_fHeight;
					if( m_fDispX < 0 && abs(zx) > 1 ) dx = -dx;
					if( m_fDispY < 0 && abs(zy) > 1 ) dy = -dy;

					float zoom[4] = {
						1 / zx, 
						1 / zy,
						(m_fWidth  * (zx - 1) - m_fDispX * 2) / (2 * zx * m_fWidth)  + dx,
						(m_fHeight * (zy - 1) - m_fDispY * 2) / (2 * zy * m_fHeight) + dy
					};

					SetConstant( pDevice, "zoom", zoom, 4 );

					CSampler sampler;
					sampler.SetTexture( m_pTexture );
					sampler.SetAddressBorder( m_nBackColor );
					sampler.SetFilter( m_eFilter );
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_LinearTransform : public ITestShader
			{
			public:
				float m_matrix[3][2];

				D3DCOLOR m_nBackColor;
				IDirect3DTexture9* m_pTexture;
				CSampler::EFilter  m_eFilter;
			
			public:
				Test_PS_LinearTransform( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_2_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_matrix[0][0] = 1; 
					m_matrix[0][1] = 0;
					m_matrix[1][0] = 0;
					m_matrix[1][1] = 1;
					m_matrix[2][0] = 0;
					m_matrix[2][1] = 0;

					m_nBackColor = 0;
					m_pTexture = 0;
					m_eFilter  = CSampler::c_nDefault;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float mx[3] = {m_matrix[0][0], m_matrix[1][0], m_matrix[2][0]};
					float my[3] = {m_matrix[0][1], m_matrix[1][1], m_matrix[2][1]};

					SetConstant( pDevice, "my", mx, 3 );
					SetConstant( pDevice, "my", my, 3 );

					CSampler sampler;
					sampler.SetTexture( m_pTexture );
					sampler.SetAddressBorder( m_nBackColor );
					sampler.SetFilter( m_eFilter );
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_Sphere : public ITestShader
			{
			public:
				float m_fCenterX;
				float m_fCenterY;
				float m_fRadiusX;
				float m_fRadiusY;
				float m_fDegree;
				float m_fWidth;
				float m_fHeight;

				D3DCOLOR m_nBackColor;
				IDirect3DTexture9* m_pTexture;
				CSampler::EFilter  m_eFilter;

			public:
				Test_PS_Sphere( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_fCenterX = 0;
					m_fCenterY = 0;
					m_fRadiusX = 0;
					m_fRadiusY = 0;
					m_fDegree  = 1;
					m_fWidth   = 1;
					m_fHeight  = 1;
					m_nBackColor = 0;
					m_pTexture = 0;
					m_eFilter  = CSampler::c_nDefault;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float fw = 1.0f / m_fWidth;
					float fh = 1.0f / m_fHeight;
					float rx = max(0.0001f, m_fRadiusX);
					float ry = max(0.0001f, m_fRadiusY);

					float center[2] = {m_fCenterX * fw, m_fCenterY * fh};
					float radius[2] = {1.0f / (rx * fw), 1.0f / (ry * fh)};

					SetConstant( pDevice, "degree", m_fDegree );
					SetConstant( pDevice, "center", center, 2 );
					SetConstant( pDevice, "radius", radius, 2 );

					CSampler sampler;
					sampler.SetAddressBorder( m_nBackColor );
					sampler.SetTexture( m_pTexture );
					sampler.SetFilter( m_eFilter );
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_CylinderVer : public ITestShader
			{
			public:
				float m_fCenter;
				float m_fRadius;
				float m_fDegree;
				float m_fWidth;

				D3DCOLOR m_nBackColor;
				IDirect3DTexture9* m_pTexture;
				CSampler::EFilter  m_eFilter;

			public:
				Test_PS_CylinderVer( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_fCenter  = 0;
					m_fRadius  = 0;
					m_fDegree  = 1;
					m_fWidth   = 1;
					m_nBackColor = 0;
					m_pTexture = 0;
					m_eFilter  = CSampler::c_nDefault;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float center = m_fCenter / m_fWidth;
					float radius = 1.0f / (max(0.0001f, m_fRadius) / m_fWidth);

					SetConstant( pDevice, "degree", m_fDegree );
					SetConstant( pDevice, "center", center );
					SetConstant( pDevice, "radius", radius );

					CSampler sampler;
					sampler.SetAddressBorder( m_nBackColor );
					sampler.SetTexture( m_pTexture );
					sampler.SetFilter( m_eFilter );
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_CylinderHor : public ITestShader
			{
			public:
				float m_fCenter;
				float m_fRadius;
				float m_fDegree;
				float m_fHeight;

				D3DCOLOR m_nBackColor;
				IDirect3DTexture9* m_pTexture;
				CSampler::EFilter  m_eFilter;

			public:
				Test_PS_CylinderHor( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_2_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_fCenter  = 0;
					m_fRadius  = 0;
					m_fDegree  = 1;
					m_fHeight  = 1;
					m_nBackColor = 0;
					m_pTexture = 0;
					m_eFilter  = CSampler::c_nDefault;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float center = m_fCenter / m_fHeight;
					float radius = 1.0f / (max(0.0001f, m_fRadius) / m_fHeight);

					SetConstant( pDevice, "degree", m_fDegree );
					SetConstant( pDevice, "center", center );
					SetConstant( pDevice, "radius", radius );

					CSampler sampler;
					sampler.SetAddressBorder( m_nBackColor );
					sampler.SetTexture( m_pTexture );
					sampler.SetFilter( m_eFilter );
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_WaveVer : public ITestShader
			{
			public:
				float m_fAmpl;
				float m_fPhase;
				float m_fDisp;
				float m_fWidth;

				IDirect3DTexture9* m_pTexture;
				CSampler::EFilter  m_eFilter;

			public:
				Test_PS_WaveVer( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_fAmpl    = 0;
					m_fPhase   = 0;
					m_fDisp    = 1;
					m_fWidth   = 1;
					m_pTexture = 0;
					m_eFilter  = CSampler::c_nDefault;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					SetConstant( pDevice, "ampl",  m_fAmpl / m_fWidth );
					SetConstant( pDevice, "phase", m_fPhase );
					SetConstant( pDevice, "disp",  m_fDisp );

					CSampler sampler;
					sampler.SetAddressBorder( 0 );
					sampler.SetTexture( m_pTexture );
					sampler.SetFilter( m_eFilter );
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}
			};

			class Test_PS_WaveHor : public ITestShader
			{
			public:
				float m_fAmpl;
				float m_fPhase;
				float m_fDisp;
				float m_fHeight;

				IDirect3DTexture9* m_pTexture;
				CSampler::EFilter  m_eFilter;

			public:
				Test_PS_WaveHor( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_fAmpl    = 0;
					m_fPhase   = 0;
					m_fDisp    = 1;
					m_fHeight  = 1;
					m_pTexture = 0;
					m_eFilter  = CSampler::c_nDefault;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					SetConstant( pDevice, "ampl",  m_fAmpl / m_fHeight );
					SetConstant( pDevice, "phase", m_fPhase );
					SetConstant( pDevice, "disp",  m_fDisp );

					CSampler sampler;
					sampler.SetAddressBorder( 0 );
					sampler.SetTexture( m_pTexture );
					sampler.SetFilter( m_eFilter );
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}
			};

			class Test_PS_HLS : public ITestShader
			{
			public:
				IDirect3DTexture9* m_pTexture;

				float m_fShiftHue;
				float m_fShiftSat;

			public:
				Test_PS_HLS( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_2_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_pTexture = 0;
					m_fShiftHue = 0;
					m_fShiftSat = 0;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					// hue (-360 .. +360)
					float hue = m_fShiftHue - int(m_fShiftHue / 360) * 360;

					SetConstant( pDevice, "dhue", hue );
					SetConstant( pDevice, "dsat", m_fShiftSat );

					CSampler sampler;
					sampler.SetAddressBorder( 0 );
					sampler.SetTexture( m_pTexture );
					sampler.SetFilterPoint();
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}
			};

			class Test_PS_Filter3x3 : public ITestShader
			{
			public:
				IDirect3DTexture9* m_pTexture;

				float m_matrix[3][3];
				float m_factor;
				float m_offset;
				float m_fWidth;
				float m_fHeight;

			public:
				Test_PS_Filter3x3( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_2_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_matrix[0][0] = 1;
					m_matrix[0][1] = 1;
					m_matrix[0][2] = 1;
					m_matrix[1][0] = 1;
					m_matrix[1][1] = 1;
					m_matrix[1][2] = 1;
					m_matrix[2][0] = 1;
					m_matrix[2][1] = 1;
					m_matrix[2][2] = 1;

					m_factor  = 1;
					m_offset  = 0;
					m_fWidth  = 1;
					m_fHeight = 1;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float dx = 1 / m_fWidth;
					float dy = 1 / m_fHeight;
					float disp[9][2] = {
						-dx, -dy,
						  0, -dy,
						 dx, -dy,
						-dx,   0,
						  0,   0,
						 dx,   0,
						-dx,  dy,
						  0,  dy,
						 dx,  dy,
					};
					float factor[2] = {m_factor, m_offset};

					SetConstant( pDevice, "m00", m_matrix[0][0] );
					SetConstant( pDevice, "m01", m_matrix[0][1] );
					SetConstant( pDevice, "m02", m_matrix[0][2] );
					SetConstant( pDevice, "m10", m_matrix[1][0] );
					SetConstant( pDevice, "m11", m_matrix[1][1] );
					SetConstant( pDevice, "m12", m_matrix[1][2] );
					SetConstant( pDevice, "m20", m_matrix[2][0] );
					SetConstant( pDevice, "m21", m_matrix[2][1] );
					SetConstant( pDevice, "m22", m_matrix[2][2] );

					SetConstant( pDevice, "d00", &disp[0][0], 2 );
					SetConstant( pDevice, "d01", &disp[1][0], 2 );
					SetConstant( pDevice, "d02", &disp[2][0], 2 );
					SetConstant( pDevice, "d10", &disp[3][0], 2 );
					SetConstant( pDevice, "d11", &disp[4][0], 2 );
					SetConstant( pDevice, "d12", &disp[5][0], 2 );
					SetConstant( pDevice, "d20", &disp[6][0], 2 );
					SetConstant( pDevice, "d21", &disp[7][0], 2 );
					SetConstant( pDevice, "d22", &disp[8][0], 2 );

					SetConstant( pDevice, "factor", factor, 2 );

					CSampler sampler;
					sampler.SetAddressClamp();
					sampler.SetTexture( m_pTexture );
					sampler.SetFilterPoint();
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}
			};

			class Test_PS_FilterCenter3x3 : public ITestShader
			{
			public:
				IDirect3DTexture9* m_pTexture;

				float m_fCenter;
				float m_fBorder;
				float m_fWidth;
				float m_fHeight;
				float m_fSumFactor;
				float m_fSumOffset;

			public:
				Test_PS_FilterCenter3x3( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_2_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_fCenter = 1;
					m_fBorder = 0;
					m_fWidth  = 1;
					m_fHeight = 1;
					m_fSumFactor = 1;
					m_fSumOffset = 0;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float dx = 0.5f / m_fWidth;
					float dy = 0.5f / m_fHeight;
					float disp[4][2] = {
						-dx,     -dy * 2,
						 dx * 2, -dy,
						 dx,      dy * 2,
						-dx * 2,  dy,
					};

					float factor[2] = {m_fCenter, m_fBorder * 2};
					float summa [2] = {m_fSumFactor, m_fSumOffset};

					SetConstant( pDevice, "factor", factor, 2 );
					SetConstant( pDevice, "summa", summa, 2 );
					SetConstant( pDevice, "d1", &disp[0][0], 2 );
					SetConstant( pDevice, "d2", &disp[1][0], 2 );
					SetConstant( pDevice, "d3", &disp[2][0], 2 );
					SetConstant( pDevice, "d4", &disp[3][0], 2 );
					
					CSampler sampler;
					sampler.SetAddressClamp();
					sampler.SetTexture( m_pTexture );
					sampler.SetFilterLinear();
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}
			};

			class Test_PS_PerspectiveTransform : public ITestShader
			{
			public:
				float m_matrix[3][3];
				
				float m_width;
				float m_height;

				D3DCOLOR m_nBackColor;
				IDirect3DTexture9* m_pTexture;
			
			public:
				Test_PS_PerspectiveTransform( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_2_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_matrix[0][0] = 1; 
					m_matrix[0][1] = 0;
					m_matrix[0][2] = 0;
					m_matrix[1][0] = 0;
					m_matrix[1][1] = 1;
					m_matrix[1][2] = 0;
					m_matrix[2][0] = 0;
					m_matrix[2][1] = 0;
					m_matrix[2][2] = 1;

					m_width = 1;
					m_height = 1;

					m_nBackColor = 0;
					m_pTexture = 0;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float w = 0.4f / m_width;
					float h = 0.4f / m_height;

					float p0[3] = { 0,  0, 1};
					float p1[3] = { 0, -h, 1};
					float p2[3] = { w,  0, 1};
					float p3[3] = { 0,  h, 1};
					float p4[3] = {-w,  0, 1};

					SetConstant( pDevice, "p0", p0, 3 );
					SetConstant( pDevice, "p1", p1, 3 );
					SetConstant( pDevice, "p2", p2, 3 );
					SetConstant( pDevice, "p3", p3, 3 );
					SetConstant( pDevice, "p4", p4, 3 );
					
					SetConstant( pDevice, "mat", &m_matrix[0][0], 9 );

					CSampler sampler;
					sampler.SetTexture( m_pTexture );
					sampler.SetAddressBorder( m_nBackColor );
					sampler.SetFilterLinear();
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_Minimal : public ITestShader
			{
			public:
				int m_count;
				float m_dx;
				float m_dy;

				IDirect3DTexture9* m_pTexture;
			
			public:
				Test_PS_Minimal( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_count = 1;
					m_dx = 0;
					m_dy = 0;

					m_pTexture = 0;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float stride[2] = {m_dx, m_dy};

					SetConstant( pDevice, "count", m_count );
					SetConstant( pDevice, "stride", stride, 2 );

					CSampler sampler;
					sampler.SetTexture( m_pTexture );
					sampler.SetAddressClamp();
					sampler.SetFilterPoint();
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_Maximal : public ITestShader
			{
			public:
				int m_count;
				float m_dx;
				float m_dy;

				IDirect3DTexture9* m_pTexture;
			
			public:
				Test_PS_Maximal( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_count = 1;
					m_dx = 0;
					m_dy = 0;

					m_pTexture = 0;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float stride[2] = {m_dx, m_dy};

					SetConstant( pDevice, "count", m_count );
					SetConstant( pDevice, "stride", stride, 2 );

					CSampler sampler;
					sampler.SetTexture( m_pTexture );
					sampler.SetAddressClamp();
					sampler.SetFilterPoint();
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_MotionBlur : public ITestShader
			{
			public:
				int m_count;
				float m_dx;
				float m_dy;

				IDirect3DTexture9* m_pTexture;
			
			public:
				Test_PS_MotionBlur( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_count = 0;
					m_dx = 0;
					m_dy = 0;

					m_pTexture = 0;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float stride[2] = {m_dx, m_dy};

					SetConstant( pDevice, "count", m_count );
					SetConstant( pDevice, "stride", stride, 2 );

					CSampler sampler;
					sampler.SetTexture( m_pTexture );
					sampler.SetAddressClamp();
					sampler.SetFilterLinear();
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_GaussinBlur : public ITestShader
			{
			public:
				int m_count;
				float m_dx;
				float m_dy;

				IDirect3DTexture9* m_pTexture;
			
			public:
				Test_PS_GaussinBlur( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_count = 0;
					m_dx = 0;
					m_dy = 0;

					m_pTexture = 0;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float stride[2] = {m_dx, m_dy};

					SetConstant( pDevice, "count", m_count );
					SetConstant( pDevice, "stride", stride, 2 );

					CSampler sampler;
					sampler.SetTexture( m_pTexture );
					sampler.SetAddressClamp();
					sampler.SetFilterPoint();
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};


			class Test_PS_GlassMosaic : public ITestShader
			{
			public:
				int m_samples;
				float m_level;
				float m_smooth;
				float m_curvature;
				float m_rescale;
				float m_factorX;
				float m_factorY;
				float m_scaleX;
				float m_scaleY;
				float m_dispX;
				float m_dispY;
				float m_width;
				float m_height;
				float m_sin;
				float m_cos;

				IDirect3DTexture9* m_pTexture;
			
			public:
				Test_PS_GlassMosaic( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_3_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_samples = 1;
					m_level   = 1;
					m_smooth  = 1;
					m_curvature = 1;
					m_rescale = 1;
					m_factorX = 1;
					m_factorY = 1;
					m_scaleX  = 1;
					m_scaleY  = 1;
					m_dispX   = 0;
					m_dispY   = 0;
					m_width   = 1;
					m_height  = 1;
					m_sin     = 0;
					m_cos     = 1;

					m_pTexture = 0;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float curv  [2] = {m_curvature, m_curvature};
					float level [2] = {float(m_level / m_samples), float(1.0 / m_samples)};
					float scale [2] = {m_scaleX , m_scaleY};
					float smooth[2] = {m_smooth, m_smooth};
					float size  [2] = {m_width, m_height};
					float resize[2] = {1 / (m_width * CheckScale(m_rescale)), 1 / (m_height * CheckScale(m_rescale))};
					float disp  [2] = {0.5f + m_dispX / m_width, 0.5f - m_dispY / m_height};
					float rotate[4] = {m_cos, m_sin, -m_sin, m_cos};
					float factor[2] = {m_factorX, m_factorY};
					
					SetConstant( pDevice, "samples", m_samples );
					SetConstant( pDevice, "curv",   curv,   2 );
					SetConstant( pDevice, "level",  level,  2 );
					SetConstant( pDevice, "scale",  scale,  2 );
					SetConstant( pDevice, "smooth", smooth, 2 );
					SetConstant( pDevice, "size",   size,   2 );
					SetConstant( pDevice, "resize", resize, 2 );
					SetConstant( pDevice, "disp",   disp,   2 );
					SetConstant( pDevice, "rotate", rotate, 4 );
					SetConstant( pDevice, "factor", factor, 2 );

					CSampler sampler;
					sampler.SetTexture( m_pTexture );
					sampler.SetAddressMirrow();
					sampler.SetFilterPoint();
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

			class Test_PS_Newsprint : public ITestShader
			{
			public:
				float m_level;
				float m_alpha;
				float m_width;
				float m_height;

				IDirect3DTexture9* m_pTexture;
			
			public:
				Test_PS_Newsprint( LPCTSTR szSrcFile, LPCSTR szProfile = "ps_2_0" )
					: ITestShader( szSrcFile, szProfile )
				{
					m_level  = 1;
					m_alpha  = 1;
					m_width  = 1;
					m_height = 1;

					m_pTexture = 0;
				}

				
				virtual BOOL SetConstants( IDirect3DDevice9* pDevice ) const
				{
					if( !CheckDefaults( pDevice ) )
						return FALSE;

					float w = 1 / m_width;
					float h = 1 / m_height;

					float pt[8][3] = {
						w * (-1.5f), h * (-1.5f), -4,
						w * (    0), h * (-1.5f), -2,
						w * ( 1.5f), h * (-1.5f), -4,
						w * (-1.5f), h * ( 0.5f), -4,
						w * ( 1.5f), h * ( 0.5f), -4,
						w * (-1.5f), h * ( 2.0f), -2,
						w * (    0), h * (2.0f - 1.0f / 6.0f), -6,
						w * ( 1.5f), h * ( 2.0f), -2,
					};

					SetConstant( pDevice, "level", m_level );
					SetConstant( pDevice, "alpha", m_alpha );
					SetConstant( pDevice, "pt", &pt[0][0], 24 );
					
					CSampler sampler;
					sampler.SetTexture( m_pTexture );
					sampler.SetAddressMirrow();
					sampler.SetFilterLinear();
					sampler.SetToDevice( pDevice, 0 );
					
					return TRUE;
				}

			};

		}


		enum EPixelShader
		{
			c_nPS_Ripple,
			c_nPS_Twirl,
			c_nPS_Zoom,
			c_nPS_LinearTransform,
			c_nPS_Sphere,
			c_nPS_CylinderVer,
			c_nPS_CylinderHor,
			c_nPS_WaveVer,
			c_nPS_WaveHor,
			c_nPS_HLS,
			c_nPS_Filter3x3,
			c_nPS_FilterCenter3x3,
			c_nPS_PerspectiveTransform,
			c_nPS_Minimal,
			c_nPS_Maximal,
			c_nPS_MotionBlur,
			c_nPS_GaussinBlur,
			c_nPS_GlassMosaic,
			c_nPS_Newsprint,

			c_nPS_All,
			c_nPS_None = -1,
		};

		
		class CTablePSs
		{
		protected:
			IDirect3DPixelShader9* m_pShaders[c_nPS_All];

		public:
			volatile DWORD m_dwMaxVersion;
		
		public:
			CTablePSs()
			{
				m_dwMaxVersion = 0;

				for( int i = 0; i < c_nPS_All; i++ )
				{
					m_pShaders[i] = NULL;
				}
			}

			~CTablePSs()
			{
				Release();
			}

			void Release()
			{
				for( int i = 0; i < c_nPS_All; i++ )
				{
					RELEASEINTERFACE(m_pShaders[i]);
				}
			}

			
			IDirect3DPixelShader9* GetShader( EPixelShader shader, IDirect3DDevice9* pDevice )
			{
				if( DWORD(shader) >= c_nPS_All )
					return NULL;
				
				IDirect3DPixelShader9* pShader = NULL;

				pShader = m_pShaders[shader];

				if( !pShader && pDevice )
				{
					const DWORD* code = GetStaticCode( shader );
					if( code )
					{
						HRESULT hr = pDevice->CreatePixelShader( code, &pShader );
						if( FAILED(hr) )
						{
							DXTRACE_ERR(0, hr);
							pShader = NULL;
						}
						else
						{
							m_pShaders[shader] = pShader;
						}
					}
				}

				return pShader;
			}

			static DWORD GetVersion( EPixelShader shader )
			{
				if( DWORD(shader) >= c_nPS_All )
					return NULL;

				return IPixelShader::GetVersionFromCode( GetStaticCode( shader ) );
			}


		private:
			static const DWORD* GetStaticCode( int shader )
			{
				static const DWORD* table[] = {
					PS_Ripple::GetStaticCode(),
					PS_Twirl::GetStaticCode(),
					PS_Zoom::GetStaticCode(),
					PS_LinearTransform::GetStaticCode(),
					PS_Sphere::GetStaticCode(),
					PS_CylinderVer::GetStaticCode(),
					PS_CylinderHor::GetStaticCode(),
					PS_WaveVer::GetStaticCode(),
					PS_WaveHor::GetStaticCode(),
					PS_HLS::GetStaticCode(),
					PS_Filter3x3::GetStaticCode(),
					PS_FilterCenter3x3::GetStaticCode(),
					PS_PerspectiveTransform::GetStaticCode(),
					PS_Minimal::GetStaticCode(),
					PS_Maximal::GetStaticCode(),
					PS_MotionBlur::GetStaticCode(),
					PS_GaussinBlur::GetStaticCode(),
					PS_GlassMosaic::GetStaticCode(),
					PS_Newsprint::GetStaticCode(),
				};

				return table[shader];
			}
		};

		static volatile DWORD m_dwMaxVersion;

		//static CTablePSs g_oShaderTable; // глобальная таблица пиксельных шейдеров
	}
}