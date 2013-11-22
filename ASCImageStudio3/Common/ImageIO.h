#pragma once
	
#include <ImageCore.h>
#include <atlfile.h>
#include <Utilities.h>


#include "../../Common/OfficeFileErrorDescription.h"

#include <../ASCGraphics/cximage/CxImage/ximage.h>
#include <../ASCGraphics/OfficeSvmFile/OfficeSvmFile.h>

#ifndef _DEBUG
#pragma comment(lib, "..\\ASCGraphics\\cximage\\jasper\\Release\\jasper.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\jbig\\Release\\jbig.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\jpeg\\Release\\jpeg.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\mng\\Release\\mng.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\png\\Release\\png.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\raw\\Release\\libdcr.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\tiff\\Release\\tiff.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\zlib\\Release\\zlib.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\CxImage\\Release\\cximage.lib")

#pragma comment(lib, "..\\ASCGraphics\\OfficeSvmFile\\Release\\OfficeSvmFile.lib")
#else
#pragma comment(lib, "..\\ASCGraphics\\cximage\\jasper\\Debug\\jasper.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\jbig\\Debug\\jbig.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\jpeg\\Debug\\jpeg.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\mng\\Debug\\mng.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\png\\Debug\\png.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\raw\\Debug\\libdcr.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\tiff\\Debug\\tiff.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\zlib\\Debug\\zlib.lib")
#pragma comment(lib, "..\\ASCGraphics\\cximage\\CxImage\\Debug\\cximage.lib")

#pragma comment(lib, "..\\ASCGraphics\\OfficeSvmFile\\Debug\\OfficeSvmFile.lib")

#endif


namespace ImageStudio
{
	namespace IO
	{
		const int c_nFormat_Unknown = -1;
		const int c_nFormat_Uncompressed_32bpp = 8888;		// bgra 32 bpp
		const int c_nFormat_Uncompressed_24bpp = 888;		// bgr 24 bpp
		const int c_nFormat_Uncompressed_16bpp565 = 565;	// grb 16 bpp
		const int c_nFormat_Uncompressed_16bpp555 = 555;	// grb 16 bpp
		const int c_nFormat_Uncompressed_8bpp = 8;			// 8 bpp paletted
		const int c_nFormat_Uncompressed_1bpp = 1;			// 1 bpp grayscale

		const int c_nFormat_IPod_691200 = 691200;			// 720x480. UYVY. PhotoPod and VideoPod NTSC image
		const int c_nFormat_IPod_307200 = 307200;			// 320x240. RGB_32. iPod nano 3G, iPod classic - 320x240, 32b
		const int c_nFormat_IPod_153601 = 153601;			// 240x320. RGB_565_LE. iPod 5G Video, iPod nano 3G 1.0.3, iPod classic 1.0.3 - 240x320, 16b
		const int c_nFormat_IPod_153600 = 153600;			// 320x240. RGB_565_LE. iPod 5G Video, iPod nano 3G 1.0.3, iPod classic 1.0.3 - 320x240, 16b
		const int c_nFormat_IPod_80000 = 80000;				// 200x200. RGB_565_LE. VideoPod album art big version
		const int c_nFormat_IPod_77440 = 77440;				// 220x176. RGB_565_BE. iPod photo - 220x176, 16b
		const int c_nFormat_IPod_46464 = 46464;				// 176x132. RGB_565_BE. iPod nano - 176x132, 16b
		const int c_nFormat_IPod_39200 = 39200;				// 140x140. RGB_565_LE. PhotoPod album art big version
		const int c_nFormat_IPod_22880 = 22880;				// 130x88.  RGB_565_LE. iPod 5G Video, iPod photo - 130x88, 16b
		const int c_nFormat_IPod_20000 = 20000;				// 100x100. RGB_565_LE. VideoPod album art small version, Nano album art big version
		const int c_nFormat_IPod_12800 = 12800;				// 80x80.   RGB_565_LE. iPod nano 3G, iPod classic - 80x80, 16b
		const int c_nFormat_IPod_8192 = 8192;				// 64x64.   RGB_565_LE. iPod nano 3G, iPod classic - 64x64, 16b
		const int c_nFormat_IPod_6272 = 6272;				// 56x56.   RGB_565_LE. PhotoPod album art small version
		const int c_nFormat_IPod_4100 = 4100;				// 50x41.   RGB_565_LE. iPod 5G Video - 50x41, 16b (list thumbnail)
		const int c_nFormat_IPod_3528 = 3528;				// 42x42.   RGB_565_LE. Nano album art small version
		const int c_nFormat_IPod_3108 = 3108;				// 42x37.   RGB_565_LE. iPod nano - 42x37, 16b (list thumbnail)
		const int c_nFormat_IPod_2520 = 2520;				// 42x30.   RGB_565_LE. iPod photo - 42x30, 16b (list thumbnail)

		const int c_nFormat_Tiff_CompressionNone = 0;		// tiff: no compression - uncompressed
		const int c_nFormat_Tiff_CompressionLzw = 1;		// tiff: lzw compression
		const int c_nFormat_Tiff_CompressionRle = 2;		// tiff: rle compression for black-white images
		const int c_nFormat_Tiff_CompressionCCITT3 = 3;		// tiff: ccitt3 compression for black-white images
		const int c_nFormat_Tiff_CompressionCCITT4 = 4;		// tiff: ccitt4 compression for black-white images

		class CMetaFile
		{
		public:

			CMetaFile()
			{
				m_pAVSMetafile = NULL;

				m_lWidth = 0;
				m_lHeight = 0;

				m_dwBackColor = 0x00ffffff;
				m_lNewWidth = 0;
				m_lNewHeight = 0;
			}

			CMetaFile( const CMetaFile& other )
			{
				Copy( other );
			}

			
			const CMetaFile& operator=( const CMetaFile& other )
			{
				if( m_pAVSMetafile )
					m_pAVSMetafile->Release();

				Copy( other );

				return *this;
			}


			~CMetaFile()
			{
				if( m_pAVSMetafile )
					m_pAVSMetafile->Release();
			}

		public:

			void Reset()
			{
				m_oTransform.Reset();

				m_lWidth  = 0;
				m_lHeight = 0;

				RELEASEINTERFACE( m_pAVSMetafile );

				m_dwBackColor = 0x00ffffff;
				m_lNewWidth = 0;
				m_lNewHeight = 0;
			}
	
			bool LoadFromFile(const CString &sFilePath )
			{
				Reset();

				CoCreateInstance( __uuidof(AVSGraphics::CASCMetafile), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IASCMetafile), (void**)(&m_pAVSMetafile) );
				if ( NULL == m_pAVSMetafile )
					return false;

				AVSGraphics::IAVSImage *pImage = NULL;
				CoCreateInstance( __uuidof(AVSGraphics::CAVSImage), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IAVSImage), (void**)(&pImage) );
				if ( NULL == pImage )
					return false;

				bool bSuccess = LoadAVSImage( pImage, _bstr_t(sFilePath) );
				if( bSuccess )
				{
					pImage->get_Width( &m_lWidth );
					pImage->get_Height( &m_lHeight );

					// Проверяем, чтобы размеры изображения не выходили за разумные ограничения
					UpdateSizes();

					// Задаем размеры матефайла и отрисовываем на нем wmf/emf
					m_pAVSMetafile->put_Width( m_lWidth );
					m_pAVSMetafile->put_Height( m_lHeight );
					
					HRESULT hr = pImage->raw_DrawOnRenderer( (AVSGraphics::IASCRenderer *)m_pAVSMetafile, 0, 0, m_lWidth, m_lHeight );
					if( FAILED(hr) )
						bSuccess = false;
				}

				pImage->Release();

				return bSuccess;
			}

			bool LoadSVGFromXML( BSTR &sXML )
			{
				Reset();

				CoCreateInstance( __uuidof(AVSGraphics::CASCMetafile), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IASCMetafile), (void**)(&m_pAVSMetafile) );
				if ( NULL == m_pAVSMetafile )
					return false;

				AVSGraphics::IAVSImage *pImage = NULL;
				CoCreateInstance( __uuidof(AVSGraphics::CAVSImage), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IAVSImage), (void**)(&pImage) );
				if ( NULL == pImage )
					return false;

				bool bSuccess = LoadSVGImageFromXML( pImage, sXML );
				if( bSuccess )
				{
					pImage->get_Width( &m_lWidth );
					pImage->get_Height( &m_lHeight );

					// Проверяем, чтобы размеры изображения не выходили за разумные ограничения
					UpdateSizes();

					// Задаем размеры матефайла и отрисовываем на нем wmf/emf
					m_pAVSMetafile->put_Width( m_lWidth );
					m_pAVSMetafile->put_Height( m_lHeight );
					
					HRESULT hr = pImage->raw_DrawOnRenderer( (AVSGraphics::IASCRenderer *)m_pAVSMetafile, 0, 0, m_lWidth, m_lHeight );
					if( FAILED(hr) )
						bSuccess = false;
				}

				pImage->Release();

				return bSuccess;
			}


			bool ConvertToBitmap(ImageStudio::Core::Image* pResultImage, LONG lWidth = -1, LONG lHeight = -1)
			{
				if( !m_pAVSMetafile )
					return false;

				AVSGraphics::IASCGraphicsRenderer* pGrRenderer = NULL;

				CoCreateInstance(__uuidof( AVSGraphics::CASCGraphicsRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IASCGraphicsRenderer), (void **)&pGrRenderer );
				if( !pGrRenderer )
					return false;

				IMediaFrame* pImage = ConvertToBitmap( pGrRenderer );

				pGrRenderer->Release();

				if( !pImage )
					return false;

				pImage->put_ColorSpace( CSP_VFLIP | CSP_BGRA );

				BOOL bRetValue = pResultImage->FromMediaData( (IUnknown *)pImage, FALSE );
				
				pImage->Release();

				return bRetValue ? true : false;
			}

			IMediaFrame* ConvertToBitmap()
			{
				if( !m_pAVSMetafile )
					return NULL;

				AVSGraphics::IASCGraphicsRenderer* pGrRenderer = NULL;

				CoCreateInstance(__uuidof( AVSGraphics::CASCGraphicsRenderer), NULL ,CLSCTX_INPROC_SERVER, __uuidof(AVSGraphics::IASCGraphicsRenderer), (void **)&pGrRenderer );
				if( !pGrRenderer )
					return NULL;

				IMediaFrame* pImage = ConvertToBitmap( pGrRenderer );

				pGrRenderer->Release();

				return pImage;
			}

			
			void SetTransform(double dM11, double dM12, double dM21, double dM22, double dX, double dY)
			{
				m_oTransform.Set( dM11, dM12, dM21, dM22, dX, dY );
			}

			void SetTransform( const double m[6] )
			{
				m_oTransform.Set( m );
			}

			void AddTransform( const double m[6] )
			{
				m_oTransform.Multiply( m );
			}


			
			int GetWidth() const
			{
				if( m_lNewWidth > 0 )
					return m_lNewWidth;

				if( m_lNewHeight <= 0 )
					return m_lWidth;

				if( m_lHeight > 0 )
					return int((DWORD(m_lWidth) * DWORD(m_lNewHeight) + DWORD(m_lHeight) / 2) / DWORD(m_lHeight));

				return 0;
			}

			int GetHeight() const
			{
				if( m_lNewHeight > 0 )
					return m_lNewHeight;

				if( m_lNewWidth <= 0 )
					return m_lHeight;

				if( m_lWidth > 0 )
					return int((DWORD(m_lHeight) * DWORD(m_lNewWidth) + DWORD(m_lWidth) / 2) / DWORD(m_lWidth));

				return 0;
			}

			
			void SetSize( int nNewWidth, int nNewHeight )
			{
				int nOldWidth  = GetWidth();
				int nOldHeight = GetHeight();
				
				m_lNewWidth  = nNewWidth;
				m_lNewHeight = nNewHeight;

				int nWidth  = GetWidth();
				int nHeight = GetHeight();

				if( nWidth != nOldWidth || nHeight != nOldHeight )
				{
					if( nOldWidth )	m_oTransform.m[4] *= double(nWidth)  / double(nOldWidth);
					if( nOldHeight )m_oTransform.m[5] *= double(nHeight) / double(nOldHeight);
				}
			}
		
		private:
			void Copy( const CMetaFile& other )
			{
				m_pAVSMetafile = other.m_pAVSMetafile;
				m_lWidth       = other.m_lWidth;
				m_lHeight      = other.m_lHeight;
				m_oTransform   = other.m_oTransform;
				m_dwBackColor  = other.m_dwBackColor;
				m_lNewWidth    = other.m_lNewWidth;
				m_lNewHeight   = other.m_lNewHeight;

				if( m_pAVSMetafile )
					m_pAVSMetafile->AddRef();
			}

			void UpdateSizes()
			{
				if ( m_lWidth >= m_lHeight && m_lWidth > m_cMaxDimension )
				{
					m_lHeight = long(((double)m_lHeight / m_lWidth) * m_cMaxDimension);
					m_lWidth  = m_cMaxDimension;
				}
				else if ( m_lHeight > m_lWidth && m_lHeight > m_cMaxDimension )
				{
					m_lWidth = long(((double)m_lWidth / m_lHeight) * m_cMaxDimension);
					m_lHeight  = m_cMaxDimension;
				}
			}
			bool LoadAVSImage( AVSGraphics::IAVSImage* pImage, BSTR bsFilePath )
			{
				BSTR bsParam = ::SysAllocString( L"LoadOnlyMeta" );
				if( !bsParam )
					return false;

				VARIANT vParam; 
				vParam.vt = VT_BOOL; 
				vParam.boolVal = VARIANT_TRUE;
				
				pImage->raw_SetAdditionalParam( bsParam, vParam );

				::SysFreeString( bsParam );


				pImage->put_FontManager(NULL);
				
				HRESULT hr = pImage->raw_LoadFromFile( bsFilePath );
				//pImage->raw
				if( FAILED(hr) )
					return false;

				LONG lType = -1;
				pImage->get_Type( &lType );

				// Проверяем является ли файл Wmf/Emf/Svg
				return (( 0x1001 == lType ) || ( 0x1002 == lType ) || (0x1004 == lType));
			}

			bool LoadSVGImageFromXML( AVSGraphics::IAVSImage* pImage, BSTR sXML )
			{
				BSTR bsParam = ::SysAllocString( L"LoadOnlyMeta" );
				if( !bsParam )
					return false;

				VARIANT vParam; 
				vParam.vt = VT_BOOL; 
				vParam.boolVal = VARIANT_TRUE;
				
				pImage->raw_SetAdditionalParam( bsParam, vParam );

				::SysFreeString( bsParam );

				pImage->put_FontManager(NULL);
				
				//HRESULT hr = pImage->raw_LoadFromFile( bsFilePath );
				//if( FAILED(hr) )
				//	return false;

				bsParam = ::SysAllocString( L"LoadSVG" );
				if( !bsParam )
					return false;

				vParam.vt = VT_BSTR; 
				vParam.bstrVal = sXML;

				HRESULT hr = pImage->raw_SetAdditionalParam( bsParam, vParam );

				::SysFreeString( bsParam );

				if( FAILED(hr) )
					return false;

				LONG lType = -1;
				pImage->get_Type( &lType );

				// Проверяем является ли файл Wmf/Emf/Svg
				return (( 0x1001 == lType ) || ( 0x1002 == lType ) || (0x1004 == lType));
			}

			IMediaFrame* ConvertToBitmap( AVSGraphics::IASCGraphicsRenderer* pGrRenderer )
			{
				HRESULT hr;

				//ставим FontManager
				BSTR bsParam = ::SysAllocString( L"FontManager" );
				if( bsParam )
				{
					VARIANT vtVariant;
					vtVariant.vt      = VT_UNKNOWN;
					vtVariant.punkVal = NULL;

					pGrRenderer->raw_SetAdditionalParam( bsParam, vtVariant );

					::SysFreeString( bsParam );
				}
				else
					return NULL;

				// Если высота и ширина не заданы, тогда выставляем размеры исходной картинки
				long lWidth  = GetWidth();
				long lHeight = GetHeight();

				// создаём результирующий фрейм
				BYTE* pBuffer = NULL;

				IMediaFrame* pImage = CreateBgraFrame( lWidth, lHeight, &pBuffer );
				if( !pImage )
					return NULL;

				// заполняем фон белым прозрачным цветом
				FillImageBuffer( pBuffer, lWidth * lHeight, m_dwBackColor );

				// Задаем Dpi 96 без привязки к монитору
				double dWidthMm  = lWidth  * 25.4 / 96;
				double dHeightMm = lHeight * 25.4 / 96;

				pGrRenderer->put_Width ( dWidthMm  );
				pGrRenderer->put_Height( dHeightMm );
				
				hr = pGrRenderer->raw_CreateFromMediaData( pImage, 0, 0, lWidth, lHeight );
				if( FAILED(hr) )
				{
					pImage->Release();
					return NULL;
				}

				// Если задана матрица преобразования, тогда передаем ее графиксу
				if ( !m_oTransform.IsIdentity() )
				{
					SAFEARRAYBOUND sabBounds[1];

					sabBounds[0].lLbound   = 0;
					sabBounds[0].cElements = 6;

					SAFEARRAY *pSA = SafeArrayCreate( VT_R8, 1, sabBounds );
					if ( pSA )
					{
						memcpy( pSA->pvData, m_oTransform.m, 6 * sizeof(double) );
						
						VARIANT vtParam;
						vtParam.vt      = VT_ARRAY;
						vtParam.pparray = new SAFEARRAY*;
						if( vtParam.pparray )
						{
							(*vtParam.pparray) = pSA;

							BSTR bsParam = ::SysAllocString( L"BaseTransform" );
							if( bsParam )
							{
								pGrRenderer->raw_SetAdditionalParam( bsParam, vtParam );
								
								::SysFreeString( bsParam );
							}

							delete vtParam.pparray;
						}

						SafeArrayDestroy( pSA );
					}
				}

				hr = m_pAVSMetafile->raw_Draw( pGrRenderer, NULL );
				if( FAILED(hr) )
				{
					pImage->Release();
					return NULL;
				}

				return pImage;
			}



			static void FillImageBuffer( BYTE* pPixels, int nCountPixels, DWORD dwColor )
			{
				DWORD* dst = (DWORD*)pPixels;
				
				for( int i = 0; i < nCountPixels; ++i )
				{
					*dst++ = dwColor;
				}
			}

		private:

			AVSGraphics::IASCMetafile *m_pAVSMetafile;

			LONG                       m_lWidth;
			LONG                       m_lHeight;

			static const LONG          m_cMaxDimension = 3000;

			struct TMatrix
			{
				double m[6];

				TMatrix()
				{
					Reset();
				}

				void Reset()
				{
					m[0] = 1;
					m[1] = 0;
					m[2] = 0;
					m[3] = 1;
					m[4] = 0;
					m[5] = 0;
				}

				void Set( const double other[6]  )
				{
					for( int i = 0; i < 6; i++ )
					{
						m[i] = other[i];
					}
				}

				void Set( double a0, double a1, double a2, double a3, double a4, double a5 )
				{
					m[0] = a0;
					m[1] = a1;
					m[2] = a2;
					m[3] = a3;
					m[4] = a4;
					m[5] = a5;
				}

				bool IsIdentity()
				{
					return !( 
						1 != m[0] || 
						0 != m[1] || 
						0 != m[2] || 
						1 != m[3] || 
						0 != m[4] || 
						0 != m[5] );
				}

				void Multiply( const double other[6] )
				{
					TMatrix tmp(*this);

					tmp.m[0] = m[0] * other[0] + m[2] * other[1];
					tmp.m[1] = m[1] * other[0] + m[3] * other[1];
					tmp.m[2] = m[0] * other[2] + m[2] * other[3];
					tmp.m[3] = m[1] * other[2] + m[3] * other[3];
					tmp.m[4] = m[0] * other[4] + m[2] * other[5] + m[4];
					tmp.m[5] = m[1] * other[4] + m[3] * other[5] + m[5];

					Set( tmp.m );
				}

			}   m_oTransform;

			long  m_lNewWidth;
			long  m_lNewHeight;
			DWORD m_dwBackColor;
		};

		class Utils
		{
		public:
			static int GetCountFrames( FILE* pFile, DWORD nFileFormat )
			{
				CxImage img;

				long nFilePos = ftell( pFile );

				img.SetEscape( -1 );
				if( !img.Decode( pFile, nFileFormat ) )
					return 0;

				DWORD dwFileType = img.GetType();
				switch( dwFileType )
				{
				case CXIMAGE_FORMAT_GIF:
				case CXIMAGE_FORMAT_ICO:
				case CXIMAGE_FORMAT_TIF:
				case CXIMAGE_FORMAT_MNG:
					fseek( pFile, nFilePos, SEEK_SET );
					img.SetEscape( 0 );
					img.SetFrame( -1 );
					img.Decode( pFile, dwFileType );
					return img.GetNumFrames();
				default:
					return 1;
				}
			}
			static int GetCountFrames( BYTE* pBuffer, size_t nBufSize, DWORD nFileFormat )
			{
				CxImage img;

				img.SetEscape( -1 );
				if( !img.Decode( pBuffer, (DWORD)nBufSize, nFileFormat ) )
					return 0;

				DWORD dwFileType = img.GetType();
				switch( dwFileType )
				{
				case CXIMAGE_FORMAT_GIF:
				case CXIMAGE_FORMAT_ICO:
				case CXIMAGE_FORMAT_TIF:
				case CXIMAGE_FORMAT_MNG:
					img.SetEscape( 0 );
					img.SetFrame( -1 );
					img.Decode( pBuffer, (DWORD)nBufSize, dwFileType );
					return img.GetNumFrames();
				default:
					return 1;
				}
			}

			static void GetEncoderClsid(const WCHAR* pFormat, CLSID* pClsid)
			{
				// variables
				UINT nEncoders = 0;
				UINT nSize = 0;
				ImageCodecInfo* pImageCodecInfo = 0;

				// retrieve encoders info
				GetImageEncodersSize(&nEncoders, &nSize);

				// check for valid encoders
				if (!nSize)
					throw 0;

				// create encoders info structure of necessary size
				pImageCodecInfo = (ImageCodecInfo*)(malloc(nSize));

				// check for valid encoder
				if (!pImageCodecInfo)
					throw 0;

				// retrieve all encoders
				GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

				// locate necessary encoder
				for (UINT nEncoder = 0; nEncoder < nEncoders; ++nEncoder)
				{
					// compare MIME strings
					if (ImageStudioUtils::CompareStrings(pImageCodecInfo[nEncoder].MimeType, pFormat) == 0)
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
			static BOOL ConvertToFormat(Gdiplus::Bitmap* pBitmapSource, int nFormat, Gdiplus::Bitmap*& pBitmapResult, BYTE*& pBitmapResultBytes, BYTE*& pBitmapResultPalette)
			{
				pBitmapResult = NULL;
				pBitmapResultBytes = NULL;
				pBitmapResultPalette = NULL;

				if (!pBitmapSource)
					return FALSE;

				int nWidth = pBitmapSource->GetWidth();
				int nHeight = pBitmapSource->GetHeight();

				Gdiplus::Rect oRectLock(0, 0, nWidth, nHeight);
				Gdiplus::BitmapData oBitmapData;

				if (Gdiplus::Ok != pBitmapSource->LockBits(&oRectLock, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &oBitmapData))
					return FALSE;

				BYTE* pSource = (BYTE*)(oBitmapData.Scan0);

				if (nFormat == c_nFormat_Uncompressed_24bpp)
				{
					int nLineSize = ImageStudioUtils::WidthBytes(24*nWidth);

					pBitmapResultBytes = new BYTE[nLineSize*nHeight];

					for (int nY = 0; nY < nHeight; ++nY)
					{
						// reset bit index and bits
						BYTE* pResult = pBitmapResultBytes + nLineSize*nY;
						int nIndex = nY*4*nWidth;

						// convert all colors to 1-bit values
						for (int nX = 0; nX < nWidth; ++nX, nIndex += 4, pResult += 3)
						{
							pResult[2] = pSource[nIndex + 2];
							pResult[1] = pSource[nIndex + 1];
							pResult[0] = pSource[nIndex + 0];
						}
					}

					pBitmapResult = new Gdiplus::Bitmap(nWidth, nHeight, nLineSize, PixelFormat24bppRGB, pBitmapResultBytes);
				}
				else if (nFormat == c_nFormat_Uncompressed_8bpp)
				{
					int nLineSize = ImageStudioUtils::WidthBytes(8*nWidth);

					pBitmapResultBytes = new BYTE[nLineSize*nHeight];
					pBitmapResultPalette = new BYTE[258*4];

					for (int nY = 0; nY < nHeight; ++nY)
					{
						// reset bit index and bits
						BYTE* pResult = pBitmapResultBytes + nLineSize*nY;
						int nIndex = nY*4*nWidth;

						// convert all colors to 1-bit values
						for (int nX = 0; nX < nWidth; ++nX, nIndex += 4, pResult++)
							*pResult = ImageStudioUtils::GetIntensity(pSource[nIndex + 2], pSource[nIndex + 1], pSource[nIndex + 0]);
					}

					// create image
					pBitmapResult = new Gdiplus::Bitmap(nWidth, nHeight, nLineSize, PixelFormat8bppIndexed, pBitmapResultBytes);

					// variables
					UINT* pPalette = (UINT*)pBitmapResultPalette;

					// compose grayscale palette
					*pPalette = PaletteFlagsGrayScale; pPalette++;
					*pPalette = 256; pPalette++;
					for (int nIndex = 0; nIndex < 255; ++nIndex, ++pPalette)
						*pPalette = RGB(nIndex, nIndex, nIndex);

					// set the palette to the image
					pBitmapResult->SetPalette((ColorPalette*)pBitmapResultPalette);
				}
				else if (nFormat == c_nFormat_Uncompressed_1bpp)
				{
					// TODO: dither image if necessary
					// Dither32Image(pSource, nWidth, nHeight);

					int nLineSize = ImageStudioUtils::WidthBytes(nWidth);

					pBitmapResultBytes = new BYTE[nLineSize*nHeight];

					// fill the memory
					for (int nY = 0; nY < nHeight; ++nY)
					{
						// reset bit index and bits
						int nBitIndex = 0;
						BYTE* pResult = pBitmapResultBytes + nLineSize*nY;
						int nIndex = nY*4*nWidth;

						// convert all colors to 1-bit values
						for (int nX = 0; nX < nWidth; ++nX, nIndex += 4)
						{
							// check for pixel color and setup new bit value
							if (ImageStudioUtils::GetLightness(pSource[nIndex + 2], pSource[nIndex + 1], pSource[nIndex + 0]) >= 127)
								ImageStudioUtils::SetBit(pResult, nBitIndex, false);
							else
								ImageStudioUtils::SetBit(pResult, nBitIndex, true);

							// update bit index
							nBitIndex++;

							// check for valid index
							if (nBitIndex == 8)
							{
								nBitIndex = 0;
								pResult++;
							}
						}
					}

					// update image width
					if (nWidth % 8 == 0)
						nWidth -= 1;

					pBitmapResult = new Gdiplus::Bitmap(nWidth, nHeight, nLineSize, PixelFormat1bppIndexed, pBitmapResultBytes);
				}
				else // if (nFormat == c_nFormat_Uncompressed_32bpp || nFormat <= 0)
				{
					pBitmapResultBytes = new BYTE[4*nWidth*nHeight];

					memcpy(pBitmapResultBytes, pSource, 4*nWidth*nHeight);

					//for (long i=3;i<4*nWidth*nHeight;i+=4)
					//	pBitmapResultBytes[i]=0;
					pBitmapResult = new Gdiplus::Bitmap(nWidth, nHeight, nWidth * 4, PixelFormat32bppARGB, pBitmapResultBytes);
				}

				return (Gdiplus::Ok == pBitmapSource->UnlockBits(&oBitmapData));
			}

			static BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage, int nFrame = 0, double* pHorResolution = NULL, double* pVerResolution = NULL, int nFormatType =0)
			{
				if (strFilePath.GetLength()<1)return FALSE;

				if (!pResultImage)
					return FALSE;

				pResultImage->Destroy();

				FILE* pFile = ::_tfopen( strFilePath, _T("rb") );
				if( !pFile ) 
					return FALSE;

				IMediaFrame* pFrame = LoadFromFile( pFile, nFormatType, nFrame, pHorResolution, pVerResolution );

				::fclose( pFile );

				BOOL bRetValue = pResultImage->FromMediaData( (IUnknown *)pFrame, FALSE );
				
				RELEASEINTERFACE( pFrame );

				return bRetValue;

				//pResultImage->Destroy();

				//nFormat = c_nFormat_Unknown;

				//BSTR bstrFilePath = strFilePath.AllocSysString();

				//BOOL bSuccess = FALSE;

				//bSuccess = pResultImage->GetImage()->CreateFromFile(bstrFilePath, nFrame, pHorResolution, pVerResolution );

				//SysFreeString(bstrFilePath);

				//nFormat = c_nFormat_Uncompressed_32bpp;

				//return bSuccess;
			}

			static BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat, const CString& strFormat, Gdiplus::EncoderParameters* pEncoderSettings)
			{
				if (!pSourceImage || !pSourceImage->IsValid())
					return FALSE;

				// find encoder first
				BSTR bstrFormat = strFormat.AllocSysString();
				CLSID oEncoder; ImageStudio::IO::Utils::GetEncoderClsid(bstrFormat, &oEncoder);
				SysFreeString(bstrFormat);

				// convert image to necessary format
				Gdiplus::Bitmap* pBitmap = NULL;
				BYTE* pBitmapBytes = NULL;
				BYTE* pBitmapPalette = NULL;
				if (!ImageStudio::IO::Utils::ConvertToFormat(pSourceImage->GetBitmap(), nFormat, pBitmap, pBitmapBytes, pBitmapPalette) || !pBitmap)
					return FALSE;

				// TODO: update image properties

				// save image to the file
				BSTR bstrFilePath = strFilePath.AllocSysString();
				Gdiplus::Status nStatus = pBitmap->Save(bstrFilePath, &oEncoder, pEncoderSettings);
				SysFreeString(bstrFilePath);

				// clear unnecessary resources
				if (pBitmap != NULL) 
					delete pBitmap;
				if (pBitmapBytes != NULL)
					delete[] pBitmapBytes;
				if (pBitmapPalette != NULL)
					delete[] pBitmapPalette;

				// all done
				return (nStatus == Gdiplus::Ok);
			}
			static BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat, BOOL bFlipVertical, const CString& strFormat, Gdiplus::EncoderParameters* pEncoderSettings)
			{
				if (!pSourceImage || !pSourceImage->IsValid())
					return FALSE;

				// find encoder first
				BSTR bstrFormat = strFormat.AllocSysString();
				CLSID oEncoder; ImageStudio::IO::Utils::GetEncoderClsid(bstrFormat, &oEncoder);
				SysFreeString(bstrFormat);

				// convert image to necessary format
				Gdiplus::Bitmap* pBitmap = NULL;
				BYTE* pBitmapBytes = NULL;
				BYTE* pBitmapPalette = NULL;
				if (!ImageStudio::IO::Utils::ConvertToFormat(pSourceImage->GetBitmap(), nFormat, pBitmap, pBitmapBytes, pBitmapPalette) || !pBitmap)
					return FALSE;

				// TODO: update image properties

				// переворачиваем картинку, если это надо...
				if (bFlipVertical)
					pBitmap->RotateFlip(Gdiplus::RotateNoneFlipY);

				// save image to the file
				BSTR bstrFilePath = strFilePath.AllocSysString();
				Gdiplus::Status nStatus = pBitmap->Save(bstrFilePath, &oEncoder, pEncoderSettings);
				SysFreeString(bstrFilePath);

				// clear unnecessary resources
				if (pBitmap != NULL) 
					delete pBitmap;
				if (pBitmapBytes != NULL)
					delete[] pBitmapBytes;
				if (pBitmapPalette != NULL)
					delete[] pBitmapPalette;

				// all done
				return (nStatus == Gdiplus::Ok);
			}



			static IMediaFrame* LoadFromFile( FILE* pFile, int nFileFormat, int nFrame = 0, double* pDpiX = NULL, double* pDpiY = NULL )
			{
				CxImage img;
				
				if( !nFrame )
				{
					if( !img.Decode( pFile, nFileFormat ) )
						return NULL;

					return CxImageToMediaFrame( img, pDpiX, pDpiY );
				}
				
				
				long nFilePos = ftell( pFile );

				int nFrameCount = GetCountFrames( pFile, nFileFormat );
				if( nFrameCount > 0  )
				{
					if( nFrame >= nFrameCount )
						nFrame = nFrameCount - 1;
				}

				fseek( pFile, nFilePos, SEEK_SET );
				
				img.SetFrame( nFrame );
				img.Decode( pFile, nFileFormat );

				return CxImageToMediaFrame( img, pDpiX, pDpiY );
			}

			static IMediaFrame* LoadFromBuffer( BYTE* pBuffer, size_t nBufSize, int nFileFormat, int nFrame = 0, double* pDpiX = NULL, double* pDpiY = NULL )
			{
				CxImage img;
				
				if( !nFrame )
				{
					if( !img.Decode( pBuffer, (DWORD)nBufSize, nFileFormat ) )
						return NULL;

					return CxImageToMediaFrame( img, pDpiX, pDpiY );
				}
				
				
				int nFrameCount = GetCountFrames( pBuffer, (DWORD)nBufSize, nFileFormat );
				if( nFrameCount > 0  )
				{
					if( nFrame >= nFrameCount )
						nFrame = nFrameCount - 1;
				}

				img.SetFrame( nFrame );
				img.Decode( pBuffer, (DWORD)nBufSize, nFileFormat );

				return CxImageToMediaFrame( img, pDpiX, pDpiY );
			}

			static IMediaFrame* CxImageToMediaFrame( CxImage& img, double* pDpiX = NULL, double* pDpiY = NULL )
			{
				if( !img.IsValid() )
					return NULL;

				if( pDpiX )	*pDpiX = (double)img.GetXDPI();
				if( pDpiY )	*pDpiY = (double)img.GetYDPI();
				
				int nWidth  = img.GetWidth();
				int nHeight = img.GetHeight();
				
				BYTE* pPixels;
				IMediaFrame* pFrame = CreateBgraFrame( nWidth, nHeight, &pPixels );
				if( !pFrame )
					return NULL;

				int nBitsPerPixel = img.GetBpp();
				int nStride = img.GetEffWidth();
				BYTE* pBuffer = img.GetBits();
				RGBQUAD* pPalette = img.GetPalette();
				bool bIsAlphaPalettePresent = img.GetPaletteAlphaEnabled();
				bool bIsAlphaApplied = false;

				if( 1 == nBitsPerPixel )
				{
					RGBQUAD pal[2];

					if( !pPalette )
					{
						for( int i = 0; i < 2; i++ )
						{
							int c = i * 255;
							pal[i].rgbBlue  = c;
							pal[i].rgbGreen = c;
							pal[i].rgbRed   = c;
						}
						pPalette = pal;
					}

					BYTE* src = pBuffer;
					BYTE* dst = pPixels;

					for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
					{
						for( int nPos = 0; nPos < nWidth; ++nPos, dst += 4 )
						{
							int index = (src[nPos >> 3] >> (7 - (nPos & 7)) * 1) & 1;
							dst[0] = pPalette[index].rgbBlue;
							dst[1] = pPalette[index].rgbGreen;
							dst[2] = pPalette[index].rgbRed;
						}
					}
				}
				else
				if( 2 == nBitsPerPixel )
				{
					RGBQUAD pal[4];

					if( !pPalette )
					{
						for( int i = 0; i < 4; i++ )
						{
							int c = (i * 255 + 2) / 3;
							pal[i].rgbBlue  = c;
							pal[i].rgbGreen = c;
							pal[i].rgbRed   = c;
						}
						pPalette = pal;
					}

					BYTE* src = pBuffer;
					BYTE* dst = pPixels;

					for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
					{
						for( int nPos = 0; nPos < nWidth; ++nPos, dst += 4 )
						{
							int index = (src[nPos >> 2] >> (3 - (nPos & 3)) * 2) & 3;
							dst[0] = pPalette[index].rgbBlue;
							dst[1] = pPalette[index].rgbGreen;
							dst[2] = pPalette[index].rgbRed;
						}
					}
				}
				else
				if( 4 == nBitsPerPixel )
				{
					RGBQUAD pal[16];

					if( !pPalette )
					{
						for( int i = 0; i < 16; i++ )
						{
							int c = (i * 255 + 8) / 15;
							pal[i].rgbBlue  = c;
							pal[i].rgbGreen = c;
							pal[i].rgbRed   = c;
						}
						pPalette = pal;
					}

					BYTE* src = pBuffer;
					BYTE* dst = pPixels;

					for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
					{
						for( int nPos = 0; nPos < nWidth; ++nPos, dst += 4 )
						{
							int index = (src[nPos >> 1] >> (1 - (nPos & 1)) * 4) & 15;
							dst[0] = pPalette[index].rgbBlue;
							dst[1] = pPalette[index].rgbGreen;
							dst[2] = pPalette[index].rgbRed;
						}
					}
				}
				else
				if( 8 == nBitsPerPixel )
				{
					BYTE* src = pBuffer;
					BYTE* dst = pPixels;

					nStride -= nWidth;

					if( pPalette )
					{
						if (bIsAlphaPalettePresent)
						{
							for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
							{
								for( int nPos = 0; nPos < nWidth; ++nPos, src += 1, dst += 4 )
								{
									int index = src[0];
									dst[0] = pPalette[index].rgbBlue;
									dst[1] = pPalette[index].rgbGreen;
									dst[2] = pPalette[index].rgbRed;
									dst[3] = pPalette[index].rgbReserved;
								}
							}
							bIsAlphaApplied = true;
						}
						else
						{
							for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
							{
								for( int nPos = 0; nPos < nWidth; ++nPos, src += 1, dst += 4 )
								{
									int index = src[0];
									dst[0] = pPalette[index].rgbBlue;
									dst[1] = pPalette[index].rgbGreen;
									dst[2] = pPalette[index].rgbRed;								
								}
							}
						}
					}
					else
					{
						for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
						{
							for( int nPos = 0; nPos < nWidth; ++nPos, src += 1, dst += 4 )
							{
								int index = src[0];
								dst[0] = index;
								dst[1] = index;
								dst[2] = index;
							}
						}
					}
				}
				else if( 24 == nBitsPerPixel )
				{
					BYTE* src = pBuffer;
					BYTE* dst = pPixels;

					nStride -= nWidth * 3;

					for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
					{
						for( int nPos = 0; nPos < nWidth; ++nPos, src += 3, dst += 4 )
						{
							dst[0] = src[0];
							dst[1] = src[1];
							dst[2] = src[2];
						}
					}
				}
				else if( 32 == nBitsPerPixel )
				{
					BYTE* src = pBuffer;
					BYTE* dst = pPixels;

					nStride -= nWidth * 4;

					for( int nRow = 0; nRow < nHeight; ++nRow, src += nStride )
					{
						for( int nPos = 0; nPos < nWidth; ++nPos, src += 4, dst += 4 )
						{
							dst[0] = src[0];
							dst[1] = src[1];
							dst[2] = src[2];
						}
					}
				}
				else
				{
					pFrame->Release();
					return NULL;
				}
				
				if( img.AlphaIsValid() )
				{
					BYTE* pAlpha  = img.AlphaGetPointer();
					DWORD nMaxAlpha = img.AlphaGetMax();

					if( 255 == nMaxAlpha )
					{
						BYTE* src = pAlpha;
						BYTE* dst = pPixels;
						int nSize = nWidth * nHeight;

						for( int i = 0; i < nSize; ++i, ++src, dst += 4 )
						{
							dst[3] = src[0];
						}
					}
					else
					{
						BYTE table[256];
						for( DWORD i = 0; i < 256; ++i )
						{
							DWORD a = (i * 255 + nMaxAlpha / 2) / nMaxAlpha;
							table[i] = (BYTE)min(a, 255);
						}

						BYTE* src = pAlpha;
						BYTE* dst = pPixels;
						int nSize = nWidth * nHeight;

						for( int i = 0; i < nSize; ++i, ++src, dst += 4 )
						{
							dst[3] = table[src[0]];
						}
					}
				}
				else if (!bIsAlphaApplied)
				{
					BYTE* dst = pPixels;
					int nSize = nWidth * nHeight;

					for( int i = 0; i < nSize; ++i, dst += 4 )
					{
						dst[3] = 255;
					}
				}

				return pFrame;
			}



			static IMediaFrame* CreateBgraFrameFromGdiplusImage( Gdiplus::Bitmap& image, int nFrame = 0, double* pHorResolution = NULL, double* pVerResolution = NULL )
			{
				GUID format;

				// retrieve image format
				if( Gdiplus::Ok != image.GetRawFormat(&format) )
					return NULL;

				if( nFrame > 0 )
				{
					// variables
					long nFrameCount = 0;

					// check for format type
					if (format == ImageFormatTIFF)
						nFrameCount = image.GetFrameCount(&FrameDimensionPage);
					else if (format == ImageFormatGIF)
						nFrameCount = image.GetFrameCount(&FrameDimensionTime);

					// check for frame count
					if (nFrame < nFrameCount)
					{
						// select necessary frame
						if (format == ImageFormatTIFF)
							image.SelectActiveFrame(&FrameDimensionPage, nFrame);
						else if (format == ImageFormatGIF)
							image.SelectActiveFrame(&FrameDimensionTime, nFrame);
					}
				}
				
				if( pHorResolution )
					*pHorResolution = image.GetHorizontalResolution();

				if( pVerResolution )
					*pVerResolution = image.GetVerticalResolution();
				
				int nWidth  = image.GetWidth();
				int nHeight = image.GetHeight();
				if( nWidth <= 0 || nHeight <= 0 )
					return NULL;

				BYTE* pBuffer = NULL;

				IMediaFrame* pFrame = CreateBgraFrame( nWidth, nHeight, &pBuffer );
				if( !pFrame )
					return NULL;

				Gdiplus::BitmapData oBitmapData;
				oBitmapData.Width       = nWidth;
				oBitmapData.Height      = nHeight;
				oBitmapData.Stride      = nWidth * 4;
				oBitmapData.PixelFormat =  PixelFormat32bppARGB;
				oBitmapData.Scan0       = pBuffer;
				oBitmapData.Reserved    = NULL;

				image.LockBits( NULL, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeUserInputBuf, PixelFormat32bppARGB, &oBitmapData );
				image.UnlockBits( &oBitmapData );

				return pFrame;
			}
			static IMediaFrame* CreateBgraFrameFromGdiplusImage( IStream* pStream, bool* pIsRequiredFormat = NULL, int nFrame = 0, double* pHorResolution = NULL, double* pVerResolution = NULL )
			{
				if( pIsRequiredFormat )
					*pIsRequiredFormat = false;

				Gdiplus::Bitmap image( pStream );
				
				if( image.GetLastStatus() != Gdiplus::Ok )
					return NULL;

				if( pIsRequiredFormat )
					*pIsRequiredFormat = true;
				
				return CreateBgraFrameFromGdiplusImage( image, nFrame, pHorResolution, pVerResolution );
			}

			static IMediaFrame* CreateBgraFrameFromGdiplusImage( const CString& sFilePath, bool* pIsRequiredFormat = NULL, int nFrame = 0, double* pHorResolution = NULL, double* pVerResolution = NULL )
			{
				if( pIsRequiredFormat )
					*pIsRequiredFormat = false;

				Gdiplus::Bitmap image( sFilePath.AllocSysString() );
				
				if( image.GetLastStatus() != Gdiplus::Ok )
					return NULL;

				if( pIsRequiredFormat )
					*pIsRequiredFormat = true;
				
				return CreateBgraFrameFromGdiplusImage( image, nFrame, pHorResolution, pVerResolution );
			}

		};
		class Bitmap
		{
		public:
			int nHorResolution;
			int nVerResolution;

			Bitmap()
			{
				nHorResolution = 0;
				nVerResolution = 0;
			}
			
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage, int nFrame = 0, double* pHorResolution = NULL, double* pVerResolution = NULL )
			{
				return Utils::FromFile(strFilePath, nFormat, pResultImage, nFrame, pHorResolution, pVerResolution);
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat)
			{
				return Utils::ToFile(pSourceImage, strFilePath, nFormat, TRUE, _T("image/bmp"), NULL);
			}
		};
		class Gif
		{
		public:
			
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				return Utils::FromFile(strFilePath, nFormat, pResultImage);
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat)
			{
				return Utils::ToFile(pSourceImage, strFilePath, nFormat, TRUE, _T("image/gif"), NULL);
			}
		};
		class Jpeg
		{
		public:
			virtual BOOL FromFile( const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage )
			{
				FILE* pFile = ::_tfopen( strFilePath, _T("rb") );
				if( !pFile ) 
					return FALSE;

				IMediaFrame* pFrame = LoadFromFile( pFile );

				::fclose( pFile );

				BOOL bRetValue = pResultImage->FromMediaData( (IUnknown *)pFrame, FALSE );
				
				RELEASEINTERFACE( pFrame );

				return bRetValue;
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat, int nQuality)
			{
				FILE* pFile = ::_tfopen( strFilePath, _T("wb") );
				if( !pFile ) 
					return FALSE;

				BOOL bSuccess = SaveToFile( pFile, pSourceImage->GetBuffer(), pSourceImage->GetWidth(), pSourceImage->GetHeight(), pSourceImage->GetWidth() * 4, nQuality );

				fclose(pFile);

				return bSuccess;
			}

			
			static BOOL SaveToFile( FILE* pFile, BYTE* pPixels, int nWidth, int nHeight, int nStride, int nQuality )
			{
				return FALSE;

				//struct jpeg_compress_struct cinfo;
				//struct jpeg_error_mgr jerr;

				//::memset( &cinfo, 0, sizeof cinfo );

				//cinfo.err = jpeg_std_error(&jerr);
				//cinfo.err->error_exit = my_error_exit;

				//__try
				//{
				//	jpeg_CreateCompress(&cinfo, JPEG_LIB_VERSION, (size_t) sizeof(struct jpeg_compress_struct));

				//	jpeg_stdio_dest(&cinfo, pFile);

				//	cinfo.image_width = nWidth;
				//	cinfo.image_height = nHeight;
				//	cinfo.input_components = 4;
				//	cinfo.in_color_space = JCS_BGRA;

				//	jpeg_set_defaults(&cinfo);

				//	jpeg_set_quality(&cinfo, nQuality, TRUE );

				//	jpeg_start_compress(&cinfo, TRUE);

				//	while (cinfo.next_scanline < cinfo.image_height) 
				//	{
				//		jpeg_write_scanlines(&cinfo, &pPixels, 1);

				//		pPixels += nStride;
				//	}
				//}
				//__except( EXCEPTION_EXECUTE_HANDLER )
				//{
				//	jpeg_destroy_compress(&cinfo);
				//	return FALSE;
				//}

				//jpeg_finish_compress(&cinfo);
				//jpeg_destroy_compress(&cinfo);

				//return TRUE;
			}

			static BOOL SaveToFile( FILE* pFile, IMediaFrame* pBGRAFrame, int nQuality )
			{
				BYTE* pBuffer;
				int nWidth;
				int nHeight;
				int nStride;

				if( !GetBGRAImageData( pBGRAFrame, pBuffer, nWidth, nHeight, &nStride ) )
					return FALSE;

				return SaveToFile( pFile, pBuffer, nWidth, nHeight, nStride, nQuality );
			}

			static BOOL SaveToFile( LPCWSTR sFilePath, IMediaFrame* pBGRAFrame, int nQuality )
			{
				FILE* pFile = ::_wfopen( sFilePath, L"wb" );
				if( !pFile ) 
					return FALSE;

				BOOL bSuccess = SaveToFile( pFile, pBGRAFrame, nQuality );

				::fclose(pFile);

				return bSuccess;
			}
			static BOOL SaveToFile( LPCSTR sFilePath, IMediaFrame* pBGRAFrame, int nQuality )
			{
				FILE* pFile = ::fopen( sFilePath, "wb" );
				if( !pFile ) 
					return FALSE;

				BOOL bSuccess = SaveToFile( pFile, pBGRAFrame, nQuality );

				::fclose(pFile);

				return bSuccess;
			}

			
			static IMediaFrame* LoadFromFile( FILE* pFile )
			{
				return Utils::LoadFromFile( pFile, CXIMAGE_FORMAT_JPG );
			}

			static IMediaFrame* LoadFromBuffer( BYTE* pBuffer, size_t nBufSize )
			{
				return Utils::LoadFromBuffer( pBuffer, nBufSize, CXIMAGE_FORMAT_JPG );
			}

		};
		class Png
		{
		public:
			
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				return Utils::FromFile(strFilePath, nFormat, pResultImage);
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat)
			{
				return Utils::ToFile(pSourceImage, strFilePath, nFormat, TRUE, _T("image/png"), NULL);
			}

		};
		class Tiff
		{
		public:
			
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				return Utils::FromFile(strFilePath, nFormat, pResultImage);
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat, int nCompression)
			{
				// compute bpp value
				int nSubformat = (nCompression == c_nFormat_Tiff_CompressionCCITT3 || nCompression == c_nFormat_Tiff_CompressionCCITT4 || nCompression == c_nFormat_Tiff_CompressionRle) ? c_nFormat_Uncompressed_1bpp : c_nFormat_Uncompressed_32bpp;

				// setup encoder values
				ULONG Value0, Value1, Value2;

				// create encoder settings structure
				EncoderParameters* pSettings = (EncoderParameters*)malloc(sizeof(EncoderParameters) + 3 * sizeof(EncoderParameter));
				if (!pSettings)
					return FALSE;

				// update value settings
				pSettings->Parameter[0].Guid = EncoderColorDepth;
				pSettings->Parameter[0].Type = EncoderParameterValueTypeLong;
				pSettings->Parameter[0].NumberOfValues = 1;
				pSettings->Parameter[0].Value = &Value0;
				pSettings->Parameter[1].Guid = EncoderCompression;
				pSettings->Parameter[1].Type = EncoderParameterValueTypeLong;
				pSettings->Parameter[1].NumberOfValues = 1;
				pSettings->Parameter[1].Value = &Value1;
				pSettings->Parameter[2].Guid = EncoderSaveFlag;
				pSettings->Parameter[2].Type = EncoderParameterValueTypeLong;
				pSettings->Parameter[2].NumberOfValues = 1;
				pSettings->Parameter[2].Value = &Value2;

				// update values
				Value0 = (nSubformat == c_nFormat_Uncompressed_1bpp) ? 1 : 32;
				Value2 = EncoderValueMultiFrame;

				// update value
				if (nCompression == c_nFormat_Tiff_CompressionNone)			Value1 = EncoderValueCompressionNone;
				else if (nCompression == c_nFormat_Tiff_CompressionLzw)		Value1 = EncoderValueCompressionLZW;
				else if (nCompression == c_nFormat_Tiff_CompressionRle)		Value1 = EncoderValueCompressionRle;
				else if (nCompression == c_nFormat_Tiff_CompressionCCITT3)	Value1 = EncoderValueCompressionCCITT3;
				else if (nCompression == c_nFormat_Tiff_CompressionCCITT4)	Value1 = EncoderValueCompressionCCITT4;

				// update values count
				pSettings->Count = 2; // (m_TiffMultipaged == VARIANT_TRUE) ? 3 : 2;

				BOOL bSuccess = Utils::ToFile(pSourceImage, strFilePath, nSubformat, TRUE,_T("image/tiff"), pSettings);

				// clear memory
				free(pSettings);

				return bSuccess;
			}

			static IMediaFrame* LoadFromFile( FILE* pFile, int nFrame )
			{
				CxImage img( pFile, CXIMAGE_FORMAT_JPG );
				if( !img.IsValid() )
					return NULL;

				return Utils::CxImageToMediaFrame( img );
			}

			static IMediaFrame* LoadFromBuffer( BYTE* pBuffer, size_t nBufSize, int nFrame )
			{
				CxImage img( pBuffer, (DWORD)nBufSize, CXIMAGE_FORMAT_JPG );
				if( !img.IsValid() )
					return NULL;

				return Utils::CxImageToMediaFrame( img );
			}

		};
		class IPod : public ImageStudio::Core::Image
		{
		public:
			
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				if (!pResultImage)
					return FALSE;

				pResultImage->Destroy();

				nFormat = c_nFormat_Unknown;

				FILE* pFile = _tfopen(strFilePath, _T("rb"));
				if (!pFile)
					return FALSE;

				int nWidth = 0;
				int nHeight = 0;

				fseek(pFile, 0, SEEK_END);
				int nSize = ftell(pFile);
				fseek(pFile, 0, SEEK_SET);

				if (nSize == 691200)		{ nHeight = 480; nWidth = 720; nFormat = c_nFormat_IPod_691200; } // TODO: decompress
				else if (nSize == 307200)	{ nHeight = 240; nWidth = 320; nFormat = c_nFormat_IPod_307200; }
				else if (nSize == 153601)	{ nHeight = 320; nWidth = 240; nFormat = c_nFormat_IPod_153601; } 
				else if (nSize == 153600)	{ nHeight = 240; nWidth = 320; nFormat = c_nFormat_IPod_153600; } 
				else if (nSize == 80000)	{ nHeight = 200; nWidth = 200; nFormat = c_nFormat_IPod_80000; } 
				else if (nSize == 77440)	{ nHeight = 176; nWidth = 220; nFormat = c_nFormat_IPod_77440; } // TODO: rotate
				else if (nSize == 46464)	{ nHeight = 132; nWidth = 176; nFormat = c_nFormat_IPod_46464; } 
				else if (nSize == 39200)	{ nHeight = 140; nWidth = 140; nFormat = c_nFormat_IPod_39200; } 
				else if (nSize == 22880)	{ nHeight =  88; nWidth = 130; nFormat = c_nFormat_IPod_22880; } 
				else if (nSize == 20000)	{ nHeight = 100; nWidth = 100; nFormat = c_nFormat_IPod_20000; } 
				else if (nSize == 12800)	{ nHeight =  80; nWidth =  80; nFormat = c_nFormat_IPod_12800; }
				else if (nSize == 8192)		{ nHeight =  64; nWidth =  64; nFormat = c_nFormat_IPod_8192; }
				else if (nSize == 6272)		{ nHeight =  56; nWidth =  56; nFormat = c_nFormat_IPod_6272; } 
				else if (nSize == 4100)		{ nHeight =  41; nWidth =  50; nFormat = c_nFormat_IPod_4100; } 
				else if (nSize == 3528)		{ nHeight =  42; nWidth =  42; nFormat = c_nFormat_IPod_3528; } 
				else if (nSize == 3108)		{ nHeight =  37; nWidth =  42; nFormat = c_nFormat_IPod_3108; } 
				else if (nSize == 2520)		{ nHeight =  30; nWidth =  42; nFormat = c_nFormat_IPod_2520; } 

				BYTE*& pPixels = pResultImage->GetBuffer();

				if (nFormat != c_nFormat_Unknown)
				{
					pPixels = new BYTE[nSize];

					fread(pPixels, 1, nSize, pFile);

					// TODO: perform post-processing

					if (nFormat == c_nFormat_IPod_77440 || nFormat == c_nFormat_IPod_46464)
						FromSwappedBytes(pPixels, nWidth*nHeight);

					if (nFormat == c_nFormat_IPod_307200)
					{
						// формат картинки - 32 бита на пиксел
						if (!pResultImage->GetImage()->CreateFromByteArray(pPixels, nWidth, nHeight, 4*nWidth, PixelFormat32bppRGB, FALSE))
							Destroy();
					}
					else
					{
						// формат картинки - 16 бит на пиксел
						if (!pResultImage->GetImage()->CreateFromByteArray(pPixels, nWidth, nHeight, 2*nWidth, PixelFormat16bppRGB565, FALSE))
							Destroy();
					}
				}

				fclose(pFile);
				
				return (nFormat != c_nFormat_Unknown);
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat)
			{
				if (!pSourceImage || !pSourceImage->IsValid())
					return FALSE;

				int nWidth = pSourceImage->GetWidth();
				int nHeight = pSourceImage->GetHeight();

				Gdiplus::Bitmap* pBitmap = (Gdiplus::Bitmap*)ConvertToFormat((Gdiplus::Image*)(*pSourceImage->GetImage()), nFormat);
				
				if (!pBitmap)
					return FALSE;

				// для некоторых типов картинок опытным путем (!) было получено, что картинки должны быть перевернутыми или повернутыми
				// TODO: может на чтение тоже что-то подобное надо ??
				if (nFormat == c_nFormat_IPod_307200 ||
					nFormat == c_nFormat_IPod_153601 || 
					nFormat == c_nFormat_IPod_153600 || 
					nFormat == c_nFormat_IPod_77440 || 
					nFormat == c_nFormat_IPod_46464 || 
					nFormat == c_nFormat_IPod_12800 ||
					nFormat == c_nFormat_IPod_8192 ||
					nFormat == c_nFormat_IPod_4100 || 
					nFormat == c_nFormat_IPod_3108 || 
					nFormat == c_nFormat_IPod_2520)
					pBitmap->RotateFlip(Gdiplus::RotateNoneFlipY);

				Gdiplus::Rect oRect(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
				Gdiplus::BitmapData oData;

				BOOL bSuccess = FALSE;

				if (nFormat == c_nFormat_IPod_307200)
				{
					// формат картинки - 32 бита на пиксел
					if (pBitmap->LockBits(&oRect, ImageLockModeRead|ImageLockModeWrite, PixelFormat32bppRGB, &oData) == Ok)
					{
						FILE* pFile = _tfopen(strFilePath, _T("wb"));
						
						if (pFile)
						{
							fwrite(oData.Scan0, 4, oData.Height*oData.Width, pFile);

							fclose(pFile);

							bSuccess = TRUE;
						}

						pBitmap->UnlockBits(&oData);
					}
				}
				else if (nFormat != c_nFormat_Unknown && nFormat != c_nFormat_IPod_691200)
				{
					// формат картинки - 16 бит на пиксел
					if (pBitmap->LockBits(&oRect, ImageLockModeRead|ImageLockModeWrite, PixelFormat16bppRGB565, &oData) == Ok)
					{
						FILE* pFile = _tfopen(strFilePath, _T("wb"));
						
						if (pFile)
						{
							// TODO: perform post-processing

							if (nFormat == c_nFormat_IPod_77440 || nFormat == c_nFormat_IPod_46464)
								ToSwappedByte((BYTE*)oData.Scan0, oData.Height*oData.Width);

							fwrite(oData.Scan0, 2, oData.Height*oData.Width, pFile);

							fclose(pFile);

							bSuccess = TRUE;
						}

						pBitmap->UnlockBits(&oData);
					}
				}

				// поворачиваем картинку назад - иначе пикселы будут плохие
				if (nFormat == c_nFormat_IPod_307200 ||
					nFormat == c_nFormat_IPod_153601 || 
					nFormat == c_nFormat_IPod_153600 || 
					nFormat == c_nFormat_IPod_77440 || 
					nFormat == c_nFormat_IPod_46464 || 
					nFormat == c_nFormat_IPod_12800 ||
					nFormat == c_nFormat_IPod_8192 ||
					nFormat == c_nFormat_IPod_4100 || 
					nFormat == c_nFormat_IPod_3108 || 
					nFormat == c_nFormat_IPod_2520)
					pBitmap->RotateFlip(Gdiplus::RotateNoneFlipY);

				return bSuccess;
			}
			
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat)
			{
				return FromFile(strFilePath, nFormat, this);
			}
			virtual BOOL ToFile(const CString& strFilePath, int nFormat)
			{
				return ToFile(this, strFilePath, nFormat);
			}
			
		protected:
			
			void FromSwappedBytes(BYTE* pData, int nSize)
			{
				short* pShort = (short*)pData;

 				for (int index = 0; index < nSize; ++index, ++pShort)
				{
					short nSource = *pShort;

					short nResult = 0;

					ImageStudioUtils::SetBit(&nResult, 0, ImageStudioUtils::GetBitShort(nSource,  8, FALSE));
					ImageStudioUtils::SetBit(&nResult, 1, ImageStudioUtils::GetBitShort(nSource,  9, FALSE));
					ImageStudioUtils::SetBit(&nResult, 2, ImageStudioUtils::GetBitShort(nSource,  10, FALSE));
					ImageStudioUtils::SetBit(&nResult, 3, ImageStudioUtils::GetBitShort(nSource,  11, FALSE));
					ImageStudioUtils::SetBit(&nResult, 4, ImageStudioUtils::GetBitShort(nSource,  12, FALSE));

					ImageStudioUtils::SetBit(&nResult,  5, ImageStudioUtils::GetBitShort(nSource, 13, FALSE));
					ImageStudioUtils::SetBit(&nResult,  6, ImageStudioUtils::GetBitShort(nSource, 14, FALSE));
					ImageStudioUtils::SetBit(&nResult,  7, ImageStudioUtils::GetBitShort(nSource, 15, FALSE));
					ImageStudioUtils::SetBit(&nResult,  8, ImageStudioUtils::GetBitShort(nSource, 0, FALSE));
					ImageStudioUtils::SetBit(&nResult,  9, ImageStudioUtils::GetBitShort(nSource, 1, FALSE));
					ImageStudioUtils::SetBit(&nResult, 10, ImageStudioUtils::GetBitShort(nSource, 2, FALSE));

					ImageStudioUtils::SetBit(&nResult, 11, ImageStudioUtils::GetBitShort(nSource,  3, FALSE));
					ImageStudioUtils::SetBit(&nResult, 12, ImageStudioUtils::GetBitShort(nSource,  4, FALSE));
					ImageStudioUtils::SetBit(&nResult, 13, ImageStudioUtils::GetBitShort(nSource,  5, FALSE));
					ImageStudioUtils::SetBit(&nResult, 14, ImageStudioUtils::GetBitShort(nSource,  6, FALSE));
					ImageStudioUtils::SetBit(&nResult, 15, ImageStudioUtils::GetBitShort(nSource,  7, FALSE));

					*pShort = nResult;
				}
			}
			void ToSwappedByte(BYTE* pData, int nSize)
			{
				short* pShort = (short*)pData;

 				for (int index = 0; index < nSize; ++index, ++pShort)
				{
					short nSource = *pShort;

					short nResult = 0;

					ImageStudioUtils::SetBit(&nResult,  8, ImageStudioUtils::GetBitShort(nSource,  0, FALSE));
					ImageStudioUtils::SetBit(&nResult,  9, ImageStudioUtils::GetBitShort(nSource,  1, FALSE));
					ImageStudioUtils::SetBit(&nResult, 10, ImageStudioUtils::GetBitShort(nSource,  2, FALSE));
					ImageStudioUtils::SetBit(&nResult, 11, ImageStudioUtils::GetBitShort(nSource,  3, FALSE));
					ImageStudioUtils::SetBit(&nResult, 12, ImageStudioUtils::GetBitShort(nSource,  4, FALSE));

					ImageStudioUtils::SetBit(&nResult, 13, ImageStudioUtils::GetBitShort(nSource,  5, FALSE));
					ImageStudioUtils::SetBit(&nResult, 14, ImageStudioUtils::GetBitShort(nSource,  6, FALSE));
					ImageStudioUtils::SetBit(&nResult, 15, ImageStudioUtils::GetBitShort(nSource,  7, FALSE));
					ImageStudioUtils::SetBit(&nResult,  0, ImageStudioUtils::GetBitShort(nSource,  8, FALSE));
					ImageStudioUtils::SetBit(&nResult,  1, ImageStudioUtils::GetBitShort(nSource,  9, FALSE));
					ImageStudioUtils::SetBit(&nResult,  2, ImageStudioUtils::GetBitShort(nSource, 10, FALSE));

					ImageStudioUtils::SetBit(&nResult,  3, ImageStudioUtils::GetBitShort(nSource, 11, FALSE));
					ImageStudioUtils::SetBit(&nResult,  4, ImageStudioUtils::GetBitShort(nSource, 12, FALSE));
					ImageStudioUtils::SetBit(&nResult,  5, ImageStudioUtils::GetBitShort(nSource, 13, FALSE));
					ImageStudioUtils::SetBit(&nResult,  6, ImageStudioUtils::GetBitShort(nSource, 14, FALSE));
					ImageStudioUtils::SetBit(&nResult,  7, ImageStudioUtils::GetBitShort(nSource, 15, FALSE));

					*pShort = nResult;
				}
			}
			Gdiplus::Image* ConvertToFormat(Gdiplus::Image* pImage, int nFormat)
			{
				if (!pImage)
					return NULL;

				if (nFormat == c_nFormat_IPod_691200)   return NULL; // TODO: compress { nHeight = 480; nWidth = 720; nFormat = c_nFormat_691200; } 
				if (nFormat == c_nFormat_IPod_307200)   return GetAspectImage( *pImage, 320, 240); 
				if (nFormat == c_nFormat_IPod_153601)   return GetAspectImage( *pImage, 240, 320); 
				if (nFormat == c_nFormat_IPod_153600)   return GetAspectImage( *pImage, 320, 240); 
				if (nFormat == c_nFormat_IPod_80000)    return GetAspectImage( *pImage, 200, 200); 
				if (nFormat == c_nFormat_IPod_77440)    return GetAspectImage( *pImage, 220, 176); // TODO: rotate
				if (nFormat == c_nFormat_IPod_46464)    return GetAspectImage( *pImage, 176, 132); 
				if (nFormat == c_nFormat_IPod_39200)    return GetAspectImage( *pImage, 140, 140);
				if (nFormat == c_nFormat_IPod_22880)    return GetAspectImage( *pImage, 130, 88); 
				if (nFormat == c_nFormat_IPod_20000)    return GetAspectImage( *pImage, 100, 100); 
				if (nFormat == c_nFormat_IPod_12800)    return GetAspectImage( *pImage, 80, 80);
				if (nFormat == c_nFormat_IPod_8192)     return GetAspectImage( *pImage, 64, 64);
				if (nFormat == c_nFormat_IPod_6272)     return GetAspectImage( *pImage, 56, 56); 
				if (nFormat == c_nFormat_IPod_4100)     return GetAspectImage( *pImage, 50, 41);
				if (nFormat == c_nFormat_IPod_3528)     return GetAspectImage( *pImage, 42, 42); 
				if (nFormat == c_nFormat_IPod_3108)     return GetAspectImage( *pImage, 42, 37); 
				if (nFormat == c_nFormat_IPod_2520)     return GetAspectImage( *pImage, 42, 30); 

				return NULL;
			}
		private:
			Gdiplus::Image* GetAspectImage( Gdiplus::Image& oSrcImage, UINT nOutWidth, UINT nOutHeight )
			{
				UINT nSrcWidth = oSrcImage.GetWidth();
				UINT nSrcHeight = oSrcImage.GetHeight();

				if( nSrcWidth * nOutHeight == nSrcHeight * nOutWidth )
				{
					return oSrcImage.GetThumbnailImage( nOutWidth, nOutHeight );
				}
				
				int nNewWidth = nSrcWidth;
				int nNewHeight = nSrcHeight;
				int nDeltaWidth = int(nSrcWidth) - nOutWidth;
				int nDeltaHeight = int(nSrcHeight) - nOutHeight;

				if( nDeltaWidth > nDeltaHeight )
					nNewHeight = nSrcWidth * nOutHeight / nOutWidth;
				else
					nNewWidth = nSrcHeight * nOutWidth / nOutHeight;

				nDeltaWidth = (nNewWidth - nSrcWidth) / 2;
				nDeltaHeight = (nNewHeight - nSrcHeight) / 2;

				Gdiplus::Bitmap oTempImage( nNewWidth, nNewHeight, oSrcImage.GetPixelFormat() );
				if(true)
				{	Gdiplus::Graphics oTempGraphics( &oTempImage );

					oTempGraphics.FillRectangle( &Gdiplus::SolidBrush( Gdiplus::Color::Black ), 0, 0, nNewWidth, nNewHeight );
					oTempGraphics.DrawImage( &oSrcImage, nDeltaWidth, nDeltaHeight );
				}
								
				return oTempImage.GetThumbnailImage( nOutWidth, nOutHeight );
			}
		};
		class Psd
		{
		public:
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				if (strFilePath.GetLength()<1)return FALSE;

				// выходной формат всегда 32bpp
				nFormat = c_nFormat_Uncompressed_32bpp;

				if (NULL == pResultImage)
					return FALSE;
				
				BOOL bRetValue = FALSE;

				// открываем файл
				HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE == hFile)
					return FALSE; // Невозможно открыть файл

				// мапим этот файл в память - так быстрее читаются данные из файла
				DWORD nFileSize = GetFileSize(hFile, NULL);
				HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
				if (NULL == hMapFile)
				{
					CloseHandle( hFile );
					return FALSE; // Невозможно создать отображение файла
				}

				// создаем view of file
				DWORD nMaxBytesRead	= nFileSize;
				VOID* pBaseAddress = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
				BYTE* pnFile = (BYTE*)pBaseAddress;
				
				while (pnFile)
				{
					DWORD nBytesRead;

					// чтение заголовка 
					CHeaderInfo	oHeader;
					nBytesRead = oHeader.Load(pnFile, nMaxBytesRead);
					if (nBytesRead < 1)
						break; // Заголовок файла не верен

					pnFile += nBytesRead;
					nMaxBytesRead -= nBytesRead;
					
					// создание изображения
					if (!pResultImage->FromByteArray(oHeader.GetWidth(), oHeader.GetHeight() ))
						break; // Не удалось зааллокейтить память под картинку

					ARGB* pclrArr = (ARGB*)pResultImage->GetBuffer();
					if (NULL == pclrArr)
						break; // Инвалидный указатель на данные в файле

					// загрузка изображения
					CImageData oImageData;
					nMaxBytesRead = oImageData.Load(pnFile, nMaxBytesRead, pclrArr, oHeader);
					if (nMaxBytesRead < 1)
						break; // Не могу загрузить графические данные

					bRetValue = TRUE;
					break;
				}
				
				// закрываем файл
				UnmapViewOfFile(pBaseAddress);
				CloseHandle(hMapFile);
				CloseHandle(hFile);

				return bRetValue;

			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat)
			{
				if (NULL == pSourceImage || c_nFormat_Uncompressed_32bpp != nFormat )
					return FALSE;
				
				CAtlFile file;
				if (S_OK != file.Create(strFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS))
					return FALSE; // Не могу создать файл
				
				int nWidth = pSourceImage->GetWidth();
				int nHeight = pSourceImage->GetHeight();

				// сохранение заголовка
				CHeaderInfo oHeader;
				if (false == oHeader.Save(file, nWidth, nHeight))
					return FALSE; // Данные заголовка не верны
				
				// сохранение изображения
				Gdiplus::Bitmap* pBitmap = pSourceImage->GetBitmap();
				if( NULL == pBitmap )
					return FALSE;

				Gdiplus::Rect oRect(0, 0, nWidth, nHeight);
				BitmapData oLockData;

				if( Ok != pBitmap->LockBits(&oRect, ImageLockModeRead, PixelFormat32bppARGB, &oLockData) )
					return FALSE;
				
				CImageData oImageData;
				BOOL bResult = oImageData.Save(file, (ARGB*)oLockData.Scan0, oHeader);

				pBitmap->UnlockBits( &oLockData );
				
				return bResult;
			}
		protected:
			static int  IntFromBytes4( const BYTE* pBytes )
			{
				int nValue = 0;

				nValue = pBytes[0];
				nValue = ( nValue << 8 ) | pBytes[1];
				nValue = ( nValue << 8 ) | pBytes[2];
				nValue = ( nValue << 8 ) | pBytes[3];
				
				return nValue;
			}
			static int  IntFromBytes2( const BYTE* pBytes )
			{
				int nValue = 0;

				nValue = pBytes[0];
				nValue = ( nValue << 8 ) | pBytes[1];
				
				return nValue;
			}
			static void IntToBytes4( int nNumber, BYTE* pBytes )
			{
				pBytes[3] = (BYTE)nNumber; nNumber >>= 8;
				pBytes[2] = (BYTE)nNumber; nNumber >>= 8;
				pBytes[1] = (BYTE)nNumber; nNumber >>= 8;
				pBytes[0] = (BYTE)nNumber;
			}
			static void IntToBytes2( int nNumber, BYTE* pBytes )
			{
				pBytes[1] = (BYTE)nNumber; nNumber >>= 8;
				pBytes[0] = (BYTE)nNumber;
			}
		protected:
			class CHeaderInfo
			{
			public:
				enum EColorMode
				{
					CM_Bitmap		= 0,	// Probably means black & white (???)
					CM_Grayscale	= 1,	// The first value in the color data is the gray value, from 0...10000.
					CM_Indexed		= 2,	// Palette image
					CM_RGB			= 3,	// The first three values in the color data are red, green, and blue.
											// They are full unsigned 16–bit values as in Apple’s RGBColor data structure.
											// Pure red=65535,0,0.
					CM_CMYK			= 4,	// The four values in the color data are cyan, magenta, yellow, and black.
											// They are full unsigned 16–bit values. 0=100% ink
											// Pure cyan=0,65535,65535,65535.
					CM_Multichannel	= 7,	// (???)
					CM_Duotone		= 8,	// (???)
					CM_Lab			= 9		// The first three values in the color data are lightness, a chrominance and b chrominance.
											// Lightness is a 16–bit value from 0...100.
											// The chromanance components are each 16–bit values from –128...127.
											// Gray values are represented by chrominance components of 0.
											// Pure white=100,0,0.
				};

				CHeaderInfo()
				{
					m_nWidth      = 0;
					m_nHeight     = 0;
					m_nPlanes     = 0;
					m_nPlaneDepth = 0;
					m_nColorMode  = 0;
					m_pPalette    = NULL;
				}
				~CHeaderInfo()
				{
					if( m_pPalette )
						delete [] m_pPalette;
				}				
				DWORD Load(BYTE* pnFile, DWORD nMaxOffset)
				{
					DWORD nBytesRead = sizeof(TFileHeader);

					if( nBytesRead > nMaxOffset )
						return 0;

					const TFileHeader& oHeader = *(TFileHeader*)pnFile;
					pnFile += nBytesRead;

					if( 0x38425053 /* 8BPS */ != IntFromBytes4( oHeader.nSignature ))
						return 0;
					
					if( 1 != IntFromBytes2( oHeader.nVersion ))
						return 0;

					m_nWidth  = IntFromBytes4( oHeader.nColumns );
					m_nHeight = IntFromBytes4( oHeader.nRows );
					if( m_nWidth < 1 || m_nHeight < 1 )
						return 0;
					
					m_nPlanes = IntFromBytes2( oHeader.nChannels );
					if( m_nPlanes < 1 || m_nPlanes > 24 )
						return 0;
					
					m_nPlaneDepth = IntFromBytes2( oHeader.nDepth );
					switch( m_nPlaneDepth )
					{
					case 1:  break;
					case 8:  break;
					case 16: break;
					case 32: break;
					default: return 0;
					}
					
					m_nColorMode = IntFromBytes2( oHeader.nMode );
					switch( m_nColorMode )
					{
					case CM_Bitmap:      break;
					case CM_Grayscale:   break;
					case CM_Indexed:     break;
					case CM_RGB:         break;
					case CM_CMYK:        break;
					case CM_Multichannel:break;
					//case CM_Duotone:     break;
					case CM_Lab:         break;
					default: return 0;
					};

					// loading palette
					nBytesRead += 4;
					if( nBytesRead > nMaxOffset )
						return 0;

					DWORD nPaletteLength = IntFromBytes4( pnFile );
					pnFile += 4;
					nBytesRead += nPaletteLength;
					if( nBytesRead > nMaxOffset )
						return 0;

					BYTE* pPalettePlane = pnFile;
					pnFile += nPaletteLength;

					if( CM_Indexed == m_nColorMode )
					{
						if( m_pPalette )
							delete m_pPalette;

						DWORD nNumColors = nPaletteLength / 3;
						m_pPalette = (ARGB*) new ARGB[nNumColors];
						if( NULL == m_pPalette )
							return 0;

						int	nRedOffset = 0;
						int	nGreenOffset = nNumColors;
						int	nBlueOffset = nNumColors * 2;
						ARGB* pClr = m_pPalette;
						
						for (; nNumColors > 0; --nNumColors, ++pPalettePlane )
						{
							*pClr++ = 0xff000000 | RGB( pPalettePlane[nBlueOffset], 
								                        pPalettePlane[nGreenOffset], 
														pPalettePlane[nRedOffset] );
						}
					}
					
					// skip loading extra params
					DWORD nExtraLength;
					
					// image resources
					nBytesRead += 4;
					if( nBytesRead > nMaxOffset )
						return 0;

					nExtraLength = IntFromBytes4(pnFile);
					pnFile += 4;
					nBytesRead += nExtraLength;
					if( nBytesRead > nMaxOffset )
						return 0;
					
					pnFile += nExtraLength;

					// layer and mask information
					nBytesRead += 4;
					if( nBytesRead > nMaxOffset )
						return 0;

					nExtraLength = IntFromBytes4(pnFile);
					pnFile += 4;
					nBytesRead += nExtraLength;
					if( nBytesRead > nMaxOffset )
						return 0;

					return nBytesRead;
				}
				bool Save( CAtlFile& oResultFile, int nWidth, int nHeight )
				{
					if( nWidth < 1 || nHeight < 1 )
						return false;

					m_nWidth      = nWidth;
					m_nHeight     = nHeight;
					m_nPlanes     = 3;
					m_nPlaneDepth = 8;
					m_nColorMode  = CM_RGB;
					
					TFileHeader oHeader;

					oHeader.nSignature[0] = 0x38; // '8'
					oHeader.nSignature[1] = 0x42; // 'B'
					oHeader.nSignature[2] = 0x50; // 'P'
					oHeader.nSignature[3] = 0x53; // 'S'

					oHeader.nVersion[0]	= 0;
					oHeader.nVersion[1]	= 1;

					oHeader.nReserved[0] = 0;
					oHeader.nReserved[1] = 0;
					oHeader.nReserved[2] = 0;
					oHeader.nReserved[3] = 0;
					oHeader.nReserved[4] = 0;
					oHeader.nReserved[5] = 0;

					IntToBytes4( m_nWidth,      oHeader.nColumns );
					IntToBytes4( m_nHeight,     oHeader.nRows );
					IntToBytes2( m_nPlanes,     oHeader.nChannels );
					IntToBytes2( m_nPlaneDepth, oHeader.nDepth );
					IntToBytes2( m_nColorMode,  oHeader.nMode );

					if( S_OK != oResultFile.Write(&oHeader, sizeof(TFileHeader)))
						return false;

					// saving extra params
					DWORD nExtraLength = 0;
					// save image palette
					if( S_OK != oResultFile.Write( &nExtraLength, 4 ))
						return false;
					// save image resources
					if( S_OK != oResultFile.Write( &nExtraLength, 4 ))
						return false;
					// save layer and mask information
					if( S_OK != oResultFile.Write( &nExtraLength, 4 ))
						return false;

					return true;
				}
				int GetWidth() const
				{
					return m_nWidth;
				}
				int GetHeight() const
				{
					return m_nHeight;
				}
				int GetColorMode() const
				{
					return m_nColorMode;
				}
				int GetNumPlanes() const
				{
					return m_nPlanes;
				}
				int GetPlaneDepth() const
				{
					return m_nPlaneDepth;
				}
				ARGB* GetPalette() const
				{
					return m_pPalette;
				}
				
			private:
#pragma pack(push,1)
				struct TFileHeader
				{
					BYTE nSignature[4]; // always equal 8BPS, do not read file if not
					BYTE nVersion[2];   // always equal 1, do not read file if not
					BYTE nReserved[6];  // must be zero
					BYTE nChannels[2];  // numer of channels including any alpha channels, supported range 1 to 24
					BYTE nRows[4];      // height in PIXELS, supported range 1 to 30000
					BYTE nColumns[4];   // width in PIXELS, supported range 1 to 30000
					BYTE nDepth[2];     // number of bpp
					BYTE nMode[2];      // colour mode of the EColorMode
				};
#pragma pack(pop)

			protected:
				int   m_nWidth;
				int   m_nHeight;
				int   m_nPlanes;
				int   m_nPlaneDepth;
				int   m_nColorMode;
				ARGB* m_pPalette;
			};
			class CImageData
			{
			public:
				DWORD Load(BYTE* pnFile, DWORD nMaxOffset, ARGB* pImage, CHeaderInfo &oHeader )
				{
					DWORD nBytesRead = 2;
					if( nBytesRead > nMaxOffset )
						return 0;
					
					int nCompression = IntFromBytes2( pnFile );
					if( nCompression < 0 || nCompression > 1 ) // неизвестное сжатие
					{						
						return 0;
					}
					pnFile += 2;

					int nWidth = oHeader.GetWidth();
					int nHeight = oHeader.GetHeight();
					int nNumPlanes = oHeader.GetNumPlanes();
					int nSizeOfPlaneLine = (nWidth * oHeader.GetPlaneDepth() + 7) >> 3; // value/8;
					int nSizeOfPlane = nSizeOfPlaneLine * nHeight;
					
					if( nCompression ) // RLE сжатие
					{
						// расчитываем смещение каналов внутри файла
						BYTE* arrpPlane[24]; // максимум 24 канала (см документацию к PSD файлам)
						int	  nSizeOfPlanesInfo  = nHeight * nNumPlanes * 2;
						BYTE* pBasePlaneAddress = pnFile + nSizeOfPlanesInfo;		
								
						nBytesRead += nSizeOfPlanesInfo;
						if( nBytesRead > nMaxOffset )
							return 0;

						int	nOffset = 0;
						arrpPlane[0] = pBasePlaneAddress;

						for( int nPlane = 1; nPlane < nNumPlanes; ++nPlane )
						{
							for( int nRow = 0; nRow < nHeight; ++nRow )
							{
								nOffset	+= IntFromBytes2( pnFile );
								pnFile	+= 2;
							}

							arrpPlane[nPlane] = pBasePlaneAddress + nOffset;
						}

						nBytesRead += nOffset;
						if( nBytesRead > nMaxOffset )
						{
							return 0;
						}
					
						// создаём буфер для данных каналов (по одной строке на канал)
						int   nSizeOfPlaneBuffer = nSizeOfPlaneLine * nNumPlanes;
						BYTE* pPlaneBuffer       = (BYTE*) new BYTE[nSizeOfPlaneBuffer];

						if( pPlaneBuffer )
						{
							// создаём пиксельный конвертер
							CPixelConverter* poConverter = CreateConverter( oHeader );
							if( poConverter )
							{
								poConverter->pSourceBuffer = pPlaneBuffer;
								poConverter->nPlaneOffset  = nSizeOfPlaneLine;
								
								// читаем данные построчно в каждый из каналов
								for( int nRow = 0; nRow < nHeight; ++nRow )
								{
									BYTE* pBufferRow = pPlaneBuffer;
									for( int nPlane = 0; nPlane < nNumPlanes; ++nPlane, pBufferRow += nSizeOfPlaneLine )
									{
										BYTE* pBuffer = arrpPlane[nPlane];
										int   nRowPos = 0;

										while( nRowPos < nSizeOfPlaneLine )
										{
											BYTE  nFirstByte;
											BYTE  nSecondByte;
											DWORD nLength = 0;

											nFirstByte  = *pBuffer++;
											nSecondByte = *pBuffer++;

											BYTE* pDst = pBufferRow + nRowPos;
											*pDst      = nSecondByte;
											nLength	   = nFirstByte;

											if( nLength < 128 )
											{
												memcpy( pDst + 1, pBuffer, nLength );
												pBuffer += nLength;
											}
											else //if( nLength > 128 )
											{
												nLength ^= 0xFF;
												nLength++;
												
												::memset( pDst + 1, nSecondByte, nLength );
											}

											nRowPos += nLength + 1;
										}

										arrpPlane[nPlane] = pBuffer;
									}

									// преобразуем в формат RGB
									int nMaxPixels	= nWidth;
									ARGB* pClr      = pImage + nRow * nMaxPixels;

									for( int nPixel = 0; nPixel < nMaxPixels; ++nPixel )
									{
										*pClr++ = poConverter->ToRGB( nPixel );
									}
								}
								delete poConverter;
							}
							delete [] pPlaneBuffer;
						}
					}
					else // несжатые данные
					{
						nBytesRead += nNumPlanes * nSizeOfPlane;
						if( nBytesRead > nMaxOffset )
							return 0;
						
						// создаём пиксельный конвертер
						CPixelConverter* poConverter = CreateConverter( oHeader );
						if( poConverter )
						{
							poConverter->pSourceBuffer = pnFile;
							poConverter->nPlaneOffset = nSizeOfPlane;

							int nMaxPixels	= nWidth * nHeight;
							ARGB* pClr = pImage;

							for( int nPixel = 0; nPixel < nMaxPixels; ++nPixel )
							{
								*pClr++ = poConverter->ToRGB( nPixel );
							}
							delete poConverter;
						}
					}

					return nBytesRead;
				}
				BOOL Save( ATL::CAtlFile& oResultFile, ARGB* pImage, CHeaderInfo &oHeader )
				{
					if( NULL == pImage )
						return FALSE;

					int nWidth = oHeader.GetWidth();
					int nHeight = oHeader.GetHeight();
					int nPixels = nWidth * nHeight;
					short nCompression = 0;

					if (oResultFile.Write(&nCompression, 2) != S_OK)
						return FALSE;
					
					bool  bRet = FALSE;
					int   nMaxSizeOfBuffer = 0x20000; // 128 kb
					int   nCurSizeOfBuffer = 0;
					BYTE* pBuffer = (BYTE*) new BYTE[nMaxSizeOfBuffer];
					if( NULL == pBuffer )
						return FALSE;

					bRet = TRUE;

					for (int nChannel = 0; nChannel < 3; nChannel++)
					{
						int nShift = (2 - nChannel)*8;
						int	nMask  = 0xff << nShift;

						for (int nCountPixel = 0; nCountPixel < nPixels; nCountPixel++)
						{
							pBuffer[nCurSizeOfBuffer++] = BYTE((pImage[nCountPixel] & nMask) >> nShift);

							if (nCurSizeOfBuffer >= nMaxSizeOfBuffer)
							{
								if (oResultFile.Write(pBuffer, nMaxSizeOfBuffer) != S_OK)
									bRet = FALSE;

								nCurSizeOfBuffer = 0;
							}
						}

						if (nCurSizeOfBuffer > 0)
						{
							if (oResultFile.Write(pBuffer, nCurSizeOfBuffer) != S_OK)
								bRet = FALSE;

							nCurSizeOfBuffer = 0;
						}
					}
					
					delete [] pBuffer;
					
					return bRet;
				}
			
			private:
				
				class CPixelConverter
				{
				public:
					BYTE* pSourceBuffer;
					int   nPlaneOffset;	

				public:
							CPixelConverter() : pSourceBuffer( NULL ), nPlaneOffset( 0 ) {}
					virtual ~CPixelConverter() {}; 
					virtual ARGB ToRGB( int nPixel ) = 0;
				};

				class CBitmap1x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE clrs = pSourceBuffer[nPixel>>3]; // Pixel/8
						
						if( clrs & (1 << (7 - (nPixel & 0x7))) )
							return 0xff000000;
						else
							return 0xffffffff;
					}
				};

				class CGrayscale8x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE clr = pSourceBuffer[nPixel];
						
						return 0xff000000 | RGB( clr, clr, clr );
					}
				};
				class CGrayscale16x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE clr = pSourceBuffer[nPixel << 1];
						
						return 0xff000000 | RGB( clr, clr, clr );
					}
				};
				class CGrayscale32x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						union
						{
							float	f;
							int		n;
						}uclr;
						
						uclr.n = IntFromBytes4(pSourceBuffer + (nPixel << 2));
						BYTE clr = BYTE(uclr.f*255 + 0.5f);

						return 0xff000000 | RGB( clr, clr, clr );
					}
				};
				class CRgb8x1 : public CPixelConverter
				{
				public:
					ARGB* pPalette;

				public:
					CRgb8x1( ARGB* pPlt ) : pPalette( pPlt ) {}

					virtual ARGB ToRGB( int nPixel )
					{
						return pPalette[pSourceBuffer[nPixel]];
					}
				};
				class CRgb8x3 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE r = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						BYTE g = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						BYTE b = pSourceBuffer[nPixel];

						return 0xff000000 | RGB( b, g, r );
					}
				};
				class CRgb16x3 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						nPixel <<= 1; // nPixel *= 2;

						BYTE r = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						BYTE g = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						BYTE b = pSourceBuffer[nPixel];

						return 0xff000000 | RGB( b, g, r );
					}
				};
				class CRgb32x3 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						union
						{
							float	f;
							int		n;
						} uclr1, uclr2, uclr3;
						
						nPixel <<= 2; // nPixel *= 4;

						uclr1.n = IntFromBytes4( pSourceBuffer + nPixel ); nPixel += nPlaneOffset;
						uclr2.n = IntFromBytes4( pSourceBuffer + nPixel ); nPixel += nPlaneOffset;
						uclr3.n = IntFromBytes4( pSourceBuffer + nPixel );
						
						BYTE r = BYTE(uclr1.f*255+0.5f);
						BYTE g = BYTE(uclr2.f*255+0.5f);
						BYTE b = BYTE(uclr3.f*255+0.5f);

						return 0xff000000 | RGB( b, g, r );
					}
				};
				class CCmyk : public CPixelConverter
				{
				public:
					float fCoefColor;
				public:
					CCmyk() : fCoefColor(0) {}
				protected:
					static ARGB	CMYKToRGB( float C, float M, float Y, float K )
					{
						int R = int(( 1.0f - ( C *( 1.0f - K ) + K ) ) * 255.0f + 0.5f);
						int G = int(( 1.0f - ( M *( 1.0f - K ) + K ) ) * 255.0f + 0.5f);
						int B = int(( 1.0f - ( Y *( 1.0f - K ) + K ) ) * 255.0f + 0.5f);

						if		( R < 0 )	R = 0;
						else if	( R > 255 ) R = 255;
						if		( G < 0 )	G = 0;
						else if	( G > 255 ) G = 255;
						if		( B < 0 )	B = 0;
						else if	( B > 255 ) B = 255;

						return 0xff000000 | RGB( B, G, R );
					}
				
				};
				class CCmyk8x3 : public CCmyk
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						int		nC, nM, nY;
						float	fC, fM, fY, fK;

						nC = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						nM = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						nY = pSourceBuffer[nPixel];
							
						fC = 1.0f - nC * fCoefColor;
						fM = 1.0f - nM * fCoefColor;
						fY = 1.0f - nY * fCoefColor;
						fK = 0;
						
						return CMYKToRGB( fC, fM, fY, fK );
					}
				};
				class CCmyk8x4 : public CCmyk
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						int		nC, nM, nY, nK;
						float	fC, fM, fY, fK;

						nC = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						nM = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						nY = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						nK = pSourceBuffer[nPixel];
							
						fC = 1.0f - nC * fCoefColor;
						fM = 1.0f - nM * fCoefColor;
						fY = 1.0f - nY * fCoefColor;
						fK = 1.0f - nK * fCoefColor;
						
						return CMYKToRGB( fC, fM, fY, fK );
					}
				};
				class CCmyk16x3 : public CCmyk
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						int		nC, nM, nY;
						float	fC, fM, fY, fK;
						
						nPixel <<= 1; // nPixel *= 2;

						nC = IntFromBytes2( pSourceBuffer + nPixel ); nPixel += nPlaneOffset;
						nM = IntFromBytes2( pSourceBuffer + nPixel ); nPixel += nPlaneOffset;
						nY = IntFromBytes2( pSourceBuffer + nPixel );
							
						fC = 1.0f - nC * fCoefColor;
						fM = 1.0f - nM * fCoefColor;
						fY = 1.0f - nY * fCoefColor;
						fK = 0;
						
						return CMYKToRGB( fC, fM, fY, fK );
					}
				};
				class CCmyk16x4 : public CCmyk
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						int		nC, nM, nY, nK;
						float	fC, fM, fY, fK;

						nPixel <<= 1; // nPixel *= 2;

						nC = IntFromBytes2( pSourceBuffer + nPixel ); nPixel += nPlaneOffset;
						nM = IntFromBytes2( pSourceBuffer + nPixel ); nPixel += nPlaneOffset;
						nY = IntFromBytes2( pSourceBuffer + nPixel ); nPixel += nPlaneOffset;
						nK = IntFromBytes2( pSourceBuffer + nPixel );
							
						fC = 1.0f - nC * fCoefColor;
						fM = 1.0f - nM * fCoefColor;
						fY = 1.0f - nY * fCoefColor;
						fK = 1.0f - nK * fCoefColor;
						
						return CMYKToRGB( fC, fM, fY, fK );
					}
				};
				class CLab : public CPixelConverter
				{
				public:
					float fLabL;
					float fLabA;
					float fLabB;
				public:
					CLab() : fLabL( 0 ), fLabA( 0 ), fLabB( 0 ) {}
				protected:
					static ARGB	LabToRGB( int L, int a, int b )
					{
						// For the conversion we first convert values to XYZ and then to RGB
						// Standards used Observer = 2, Illuminant = D65
						// ref_X = 95.047, ref_Y = 100.000, ref_Z = 108.883

						const float c_fK1 = 1 / 116.0f;
						const float c_fK2 = 1 / 500.0f;
						const float c_fK3 = 1 / 200.0f;
						const float c_fK4 = 1 / 7.787f;
						const float c_fMaxValue = 0.008856f;

						float fY = (L + 16)	* c_fK1;
						float fX = fY + a	* c_fK2;
						float fZ = fY - b	* c_fK3;

						float fValue;

						fValue = pow(fY, 3);
						fY  = (fValue > c_fMaxValue) ? fValue : (fY - 16*c_fK1) * c_fK4;
						fY *= 1.00000f; // 100.000f

						fValue = pow(fX, 3);
						fX  = (fValue > c_fMaxValue) ? fValue : (fX - 16*c_fK1) * c_fK4;
						fX *= 0.95047f; // 95.047f

						fValue = pow(fZ, 3);
						fZ  = (fValue > c_fMaxValue) ? fValue : (fZ - 16*c_fK1) * c_fK4;
						fZ *= 1.08883f; // 108.883f

						// XYZ convert
						const float c_fK = 1 / 2.4f;
						const float c_fMaxValue2 = 0.0031308f;

						float fR = fX * 3.2406f	+ fY * -1.5372f	+ fZ * -0.4986f;
						float fG = fX * -0.9689f+ fY * 1.8758f	+ fZ * 0.0415f;
						float fB = fX * 0.0557f	+ fY * -0.2040f	+ fZ * 1.0570f;
						
						if ( fR > c_fMaxValue2 )
							fR = 1.055f * ( pow(fR, c_fK) ) - 0.055f;
						else
							fR *= 12.92f;

						if ( fG > c_fMaxValue2 )
							fG = 1.055f * ( pow(fG, c_fK) ) - 0.055f;
						else
							fG *= 12.92f;

						if ( fB > c_fMaxValue2 )
							fB = 1.055f * ( pow(fB, c_fK) )- 0.055f;
						else
							fB *= 12.92f;

						int R = (int)(fR * 255 + 0.5f);
						int G = (int)(fG * 255 + 0.5f);
						int B = (int)(fB * 255 + 0.5f);

						if		( R < 0 )	R = 0;
						else if	( R > 255 ) R = 255;
						if		( G < 0 )	G = 0;
						else if	( G > 255 ) G = 255;
						if		( B < 0 )	B = 0;
						else if	( B > 255 ) B = 255;

						return 0xff000000 | RGB( B, G, R );
					}
				};
				class CLab8x3 : public CLab
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						int	nL = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						int	nA = pSourceBuffer[nPixel]; nPixel += nPlaneOffset;
						int	nB = pSourceBuffer[nPixel];

						return LabToRGB( int(nL * fLabL), int(nA * fLabA - 128), int(nB * fLabB - 128) );
					}
				};
				class CLab16x3 : public CLab
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						nPixel <<= 1; // nPixel *= 2;

						int	nL = IntFromBytes2( pSourceBuffer + nPixel ); nPixel += nPlaneOffset;
						int	nA = IntFromBytes2( pSourceBuffer + nPixel ); nPixel += nPlaneOffset;
						int	nB = IntFromBytes2( pSourceBuffer + nPixel );

						return LabToRGB( int(nL * fLabL), int(nA * fLabA - 128), int(nB * fLabB - 128) );
					}
				};

			private:
				static CPixelConverter* CreateConverter( CHeaderInfo& oHeader )
				{
					int nNumPlanes = oHeader.GetNumPlanes();
					int nPlaneDepth = oHeader.GetPlaneDepth();
					CPixelConverter* poConverter = NULL;

					switch( oHeader.GetColorMode() )
					{
					case CHeaderInfo::CM_Bitmap:
						if( 1 == nPlaneDepth )
						{
							poConverter = (CPixelConverter*) new CBitmap1x1();
						}
						break;
					
					case CHeaderInfo::CM_Grayscale:
						switch( nPlaneDepth )
						{
						case 8:  poConverter = (CPixelConverter*) new CGrayscale8x1(); break;
						case 16: poConverter = (CPixelConverter*) new CGrayscale16x1(); break;
						case 32: poConverter = (CPixelConverter*) new CGrayscale32x1(); break;
						}
						break;
					
					case CHeaderInfo::CM_Indexed:
							poConverter = (CPixelConverter*) new CRgb8x1( oHeader.GetPalette() );
						break;

					case CHeaderInfo::CM_RGB:
						if( nNumPlanes > 2 )
						{
							switch( nPlaneDepth )
							{
							case 8:  poConverter = (CPixelConverter*) new CRgb8x3(); break;
							case 16: poConverter = (CPixelConverter*) new CRgb16x3(); break;
							case 32: poConverter = (CPixelConverter*) new CRgb32x3(); break;
							}
						}
						break;
					
					case CHeaderInfo::CM_Multichannel:
					case CHeaderInfo::CM_CMYK:
						if( nNumPlanes > 2 )
						{
							CCmyk* poCmykConverter = NULL;

							if( nNumPlanes < 4 )
							{
								switch( nPlaneDepth )
								{
								case 8:  poCmykConverter = (CCmyk*) new CCmyk8x3(); break;
								case 16: poCmykConverter = (CCmyk*) new CCmyk16x3(); break;
								}
							}
							else
							{
								switch( nPlaneDepth )
								{
								case 8:  poCmykConverter = (CCmyk*) new CCmyk8x4(); break;
								case 16: poCmykConverter = (CCmyk*) new CCmyk16x4(); break;
								}
							}

							if( poCmykConverter )
							{
								poCmykConverter->fCoefColor = 1.0f / pow( float(2), float(nPlaneDepth) );
								poConverter = (CPixelConverter*)poCmykConverter;
							}
						}
						break;
					
					case CHeaderInfo::CM_Lab:
						if( nNumPlanes > 2 )
						{
							CLab* poLabConverter = NULL;

							switch( nPlaneDepth )
							{
							case 8:  poLabConverter = (CLab*) new CLab8x3(); break;
							case 16: poLabConverter = (CLab*) new CLab16x3(); break;
							}

							if( poLabConverter )
							{
								float fCoefColor = 1.0f / pow( float(2), float(nPlaneDepth) );

								poLabConverter->fLabL = fCoefColor * 100;
								poLabConverter->fLabA = fCoefColor * 256;
								poLabConverter->fLabB = fCoefColor * 256;

								poConverter = (CPixelConverter*)poLabConverter;
							}
						}
						break;
					}

					return poConverter;
				}
			};
		};
		class Pcx
		{
		public:
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				// выходной формат всегда 32bpp
				nFormat = c_nFormat_Uncompressed_32bpp;

				if (NULL == pResultImage)
					return FALSE;

				// открываем файл
				HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE == hFile)
					return FALSE; // Невозможно открыть файл

				// мапим этот файл в память - так быстрее читаются данные из файла
				DWORD  nFileSize = GetFileSize(hFile, NULL);
				HANDLE hMapFile  = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
				if (NULL == hMapFile)
				{
					CloseHandle( hFile );
					return FALSE; // Невозможно создать отображение файла
				}

				// создаем view of file
				DWORD nMaxBytesRead	= nFileSize;
				VOID* pBaseAddress	= MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
				BYTE* pnFile		= (BYTE*)pBaseAddress;
				BOOL  bRetValue		= FALSE;
				
				while( pnFile )
				{
					CHeaderInfo oHeader;
					CImageData	oImageData;

					DWORD nBytesRead;

					// чтение заголовка 
					nBytesRead = oHeader.Load(pnFile, nMaxBytesRead);
					if (nBytesRead < 1)
						break; // Заголовок файла не верен

					pnFile += nBytesRead;
					nMaxBytesRead -= nBytesRead;

					// создание изображения
					if (!pResultImage->FromByteArray( oHeader.nWidth, oHeader.nHeight ))
						break; // Не удалось зааллокейтить память под картинку

					ARGB* pclrArr = (ARGB*)pResultImage->GetBuffer();
					if (NULL == pclrArr)
						break; // Инвалидный указатель на данные в файле

					// загрузка изображения
					nMaxBytesRead = oImageData.Load(pnFile, nMaxBytesRead, pclrArr, oHeader );
					if (nMaxBytesRead < 1)
						break; // Не могу загрузить графические данные

					bRetValue = TRUE;
					break;
				}

				// закрываем файл
				UnmapViewOfFile(pBaseAddress);
				CloseHandle(hMapFile);
				CloseHandle(hFile);

				return bRetValue;
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat)
			{
				if (NULL == pSourceImage || c_nFormat_Uncompressed_32bpp != nFormat )
					return FALSE;
				
				CAtlFile file;
				if (S_OK != file.Create(strFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS))
					return FALSE; // Не могу создать файл
				
				int nWidth = pSourceImage->GetWidth();
				int nHeight = pSourceImage->GetHeight();

				// сохранение заголовка
				CHeaderInfo oHeader;
				if (false == oHeader.Save(file, nWidth, nHeight))
					return FALSE; // Данные заголовка не верны
				
				// сохранение изображения
				Gdiplus::Bitmap* pBitmap = pSourceImage->GetBitmap();
				if( NULL == pBitmap )
					return FALSE;

				Gdiplus::Rect oRect(0, 0, nWidth, nHeight);
				BitmapData oLockData;

				if( Ok != pBitmap->LockBits(&oRect, ImageLockModeRead, PixelFormat32bppARGB, &oLockData) )
					return FALSE;
				
				CImageData oImageData;
				BOOL bResult = oImageData.Save(file, (ARGB*)oLockData.Scan0, oHeader);

				pBitmap->UnlockBits( &oLockData );
				
				return bResult;
			}
		protected:
			class CHeaderInfo
			{
			private:
#pragma pack(push,1)
				struct TFileHeader
				{
					BYTE nManufacturer;			// код производителя (редактора)
					BYTE nVersion;				// версия файла
					BYTE bEncoding;				// сжатие данных: 0 - нет, 1 - да
					BYTE nBitsPerPixelPerPlane;	// количество бит на пиксел в слое
					WORD nMinX, nMinY;			// координаты изображения 
					WORD nMaxX, nMaxY;			//
					WORD nScreenHorResolution;	// разрешение монитора по горизонтали
					WORD nScreenVerResolution;	// разрешение монитора по вертикали
					BYTE arrColorMap[48];		// палитра RGB на 16 цветов
					BYTE nReserved;				// номер видеорижима (сейчас не используеться)
					BYTE nPlanes;				// количество слоёв
					WORD nBytesPerLinePerPlane; // количество байт на строку в слое
					WORD bGrayscalePalette;		// итерпритация цветов палитры: 0 - оттенки серого, 1 - RGB
					WORD nScannerHorResolution;	// разрешение сканера по горизонтали
					WORD nScannerVerResolution;	// разрешение сканера по вертикали
					BYTE arrExtraData[54];		// дополнительная информация
				};
#pragma pack(pop)

			public:
				
				int		nWidth;
				int		nHeight;
				BYTE*	pPalette16;
				BYTE*	pPalette256;
				BOOL	bCompress;
				BOOL	bGreyScale;
				int		nPlanes;
				int		nDepth;
				int		nBytesPerLinePerPlane;

			public:
				
				CHeaderInfo()
				{
					nWidth		= 0;
					nHeight		= 0;
					pPalette16	= NULL;
					pPalette256 = NULL;
					bCompress	= FALSE;
					bGreyScale	= FALSE;
					nPlanes		= 0;
					nDepth		= 0;
					nBytesPerLinePerPlane = 0;
				}

				DWORD Load( BYTE* pnFile, DWORD nMaxOffset )
				{
					if( sizeof(TFileHeader) > nMaxOffset )
						return 0;

					const TFileHeader& oHeader = *(TFileHeader*)pnFile;

					// проверка формата
					if( 10 != oHeader.nManufacturer )
						return 0; 

					switch( oHeader.nVersion )
					{
					case 0:	 break;
					case 2:	 break;
					case 3:	 break;
					case 5:	 break;
					default: return 0;
					}

					switch( oHeader.bEncoding )
					{
					case 0:	 break;
					case 1:  break;
					default: return 0;
					}

					switch( oHeader.nBitsPerPixelPerPlane )
					{
					case 1:  break;
					case 4:  break;
					case 8:  break;
					//case 16: break;
					case 24: break;
					//case 32: break;
					default: return 0;
					}

					//switch( oHeader.bGrayscalePalette )
					//{
					//case 0:  break;
					//case 1:  break;
					//case 2:  break;
					//default: return 0;
					//}
					
					if( sizeof(TFileHeader) + 768 + 1 <= nMaxOffset )
					{
						BYTE* pTestByte = pnFile + nMaxOffset - 768 - 1;
						if( 0x0C == *pTestByte || 0x0A == *pTestByte )
						{
							pPalette256 = pTestByte + 1;
						}
					}

					nWidth		= oHeader.nMaxX - oHeader.nMinX + 1;
					nHeight		= oHeader.nMaxY - oHeader.nMinY + 1;
					pPalette16	= (BYTE*)oHeader.arrColorMap;
					bCompress	= (oHeader.bEncoding) ? TRUE : FALSE;
					bGreyScale	= (2 == oHeader.bGrayscalePalette) ? TRUE : FALSE;
					nPlanes		= oHeader.nPlanes;
					nDepth		= oHeader.nBitsPerPixelPerPlane;
					nBytesPerLinePerPlane = oHeader.nBytesPerLinePerPlane;

					return sizeof(TFileHeader);
				}
				BOOL Save(ATL::CAtlFile& oResultFile, int width, int height )
				{
					nWidth = width;
					nHeight = height;
					nDepth = 8;
					nPlanes = 3;
					nBytesPerLinePerPlane = (nWidth + 1) & ~1;
					bCompress = FALSE;
					bGreyScale = FALSE;
					pPalette16 = NULL;
					pPalette256 = NULL;
					
					TFileHeader oHeader;
					ZeroMemory( &oHeader, sizeof(TFileHeader) );

					oHeader.nManufacturer = 10;
					oHeader.nVersion = 5;
					oHeader.bEncoding = bCompress;
					oHeader.nBitsPerPixelPerPlane = nDepth;
					oHeader.nMinX = 0;
					oHeader.nMinY = 0;
					oHeader.nMaxX = nWidth - 1;
					oHeader.nMaxY = nHeight - 1;
					oHeader.nPlanes = nPlanes;
					oHeader.nBytesPerLinePerPlane = nBytesPerLinePerPlane;

					if( S_OK != oResultFile.Write( &oHeader, sizeof(TFileHeader)))
						return FALSE;

					return TRUE;
				}
			};

			class CImageData
			{
			public:
				DWORD Load(BYTE* pnFile, DWORD nMaxOffset, ARGB* pImage, CHeaderInfo &oHeader )
				{
					int	nWidth = oHeader.nWidth;
					int nHeight = oHeader.nHeight;
					int	nBytesPerLinePerPlane = oHeader.nBytesPerLinePerPlane;
					DWORD nBytesPerLine = nBytesPerLinePerPlane * oHeader.nPlanes;
					DWORD nBytesRead = 0;

					CPixelConverter* poConverter = CreateConverter( oHeader );
					if( NULL == poConverter )
						return 0;

					ARGB* pClr = pImage;

					if( oHeader.bCompress )
					{
						BYTE* pBuffer = (BYTE*)new BYTE[nBytesPerLine];
						if( NULL == pBuffer )
						{
							delete poConverter;
							return 0;
						}
						
						poConverter->pChannelBuffer = pBuffer;
						poConverter->nChannelOffset = nBytesPerLinePerPlane;

						// читаем данные построчно
						for( int nRow = 0; nRow < nHeight; nRow++ )
						{
							// декодирование
							DWORD nTotalBytes = 0;
							while( nTotalBytes < nBytesPerLine )
							{
								if( ++nBytesRead > nMaxOffset )
									break;

								BYTE nFirstByte = *pnFile++;
								if( 0xc0 == (nFirstByte & 0xc0) )
								{
									if( ++nBytesRead > nMaxOffset )
										break;

									BYTE nSecondByte = *pnFile++;
									nFirstByte &= ~0xc0;

									memset( pBuffer + nTotalBytes, nSecondByte, nFirstByte );
									nTotalBytes += nFirstByte;
								}
								else
								{
									pBuffer[nTotalBytes++] = nFirstByte;
								}
							}

							// преобразование пикселов
							for( int nPixel = 0; nPixel < nWidth; nPixel++ )
							{
								*pClr++ = poConverter->ToRGB( nPixel );
							}
						}
						delete [] pBuffer;
					}
					else
					{
						// проверка данных на целостность
						nBytesRead += nBytesPerLine * nHeight;
						if( nBytesRead <= nMaxOffset )
						{
							poConverter->pChannelBuffer = pnFile;
							poConverter->nChannelOffset = nBytesPerLinePerPlane;

							// читаем данные построчно 
							for( int nRow = 0; nRow < nHeight; nRow++ )
							{
								// преобразуем пикселы
								for( int nPixel = 0; nPixel < nWidth; nPixel ++ )
								{
									*pClr++ = poConverter->ToRGB( nPixel );
								}
								poConverter->pChannelBuffer += nBytesPerLine;
							}
						}
					}
					delete poConverter;

					return nBytesRead;
				}
				BOOL  Save(ATL::CAtlFile& oResultFile, ARGB* pImage, CHeaderInfo &oHeader)
				{
					if( NULL == pImage )
						return FALSE;

					int nWidth = oHeader.nWidth;
					int nHeight = oHeader.nHeight;
					int	nBytesPerLinePerPlane = oHeader.nBytesPerLinePerPlane;
					int nBytesPerLine = nBytesPerLinePerPlane * oHeader.nPlanes;
					
					BYTE* pBuffer = new BYTE[nBytesPerLine];
					if( !pBuffer )
						return FALSE;
					
					ZeroMemory( pBuffer, nBytesPerLine );
					BYTE* pClr = (BYTE*)pImage;

					for( int nRow = 0; nRow < nHeight; nRow++ )
					{
						for( int nPixel = 0; nPixel < nWidth; ++nPixel, pClr += 4 )
						{
							pBuffer[nPixel] = pClr[2];
							pBuffer[nPixel + nBytesPerLinePerPlane] = pClr[1];
							pBuffer[nPixel + nBytesPerLinePerPlane*2] = pClr[0];
						}

						if( S_OK != oResultFile.Write( pBuffer, nBytesPerLine ) )
						{
							delete [] pBuffer;
							return FALSE;
						}
					}
					
					delete [] pBuffer;
					return TRUE;
				}
			private:
				class CPixelConverter
				{
				public:
					BYTE*	pChannelBuffer;
					int		nChannelOffset;	

				public:
							CPixelConverter() : pChannelBuffer( NULL ), nChannelOffset( 0 ) {}
					virtual ~CPixelConverter() {}; 
					virtual ARGB ToRGB( int nPixel ) = 0;
				};
				class CBitmap1x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE clrs = pChannelBuffer[nPixel>>3]; // Pixel/8
						
						if( clrs & (1 << (7 - (nPixel & 0x7))) )
							return 0xffffffff;
						else
							return 0xff000000;
					}
				};

				class CRgb8x1 : public CPixelConverter
				{
				protected:
					LPBYTE m_pPalette;
				public:
					CRgb8x1( BYTE* pPlt ) : m_pPalette( pPlt ) {}

					virtual ARGB ToRGB( int nPixel )
					{
						int index = pChannelBuffer[nPixel]*3;
						return 0xff000000 | RGB(m_pPalette[index+2], m_pPalette[index+1], m_pPalette[index]);
					}
				};
				class CRgb8x3 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE r = pChannelBuffer[nPixel]; nPixel += nChannelOffset;
						BYTE g = pChannelBuffer[nPixel]; nPixel += nChannelOffset;
						BYTE b = pChannelBuffer[nPixel];

						return 0xff000000 | RGB( b, g, r );
					}
				};
				class CRgb24x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE r = pChannelBuffer[nPixel];
						BYTE g = pChannelBuffer[nPixel + 1];
						BYTE b = pChannelBuffer[nPixel + 2];

						return 0xff000000 | RGB( b, g, r );
					}
				};
				class CRgb4x1 : public CPixelConverter
				{
				protected:
					LPBYTE m_pPalette;
				public:
					CRgb4x1( BYTE* pPlt ) : m_pPalette( pPlt ) {}

					virtual ARGB ToRGB( int nPixel )
					{
						int index = pChannelBuffer[nPixel >> 1];
						if( !(nPixel & 1) )
							index = (index & 0x0f) * 3;
						else
							index = ((index & ~0x0f) >> 4) * 3;
						
						return 0xff000000 | RGB(m_pPalette[index+2], m_pPalette[index+1], m_pPalette[index]);
					}
				};
				class CGrayscale8x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE clr = pChannelBuffer[nPixel];
						
						return 0xff000000 | RGB( clr, clr, clr );
					}
				};

			private:
				static CPixelConverter* CreateConverter( CHeaderInfo& oHeader )
				{
					CPixelConverter* poConverter = NULL;

					switch( oHeader.nDepth )
					{
					case 1:
						if( 1 == oHeader.nPlanes )
						{
							poConverter = (CPixelConverter*) new CBitmap1x1();
						}
						break;

					case 4:
						if( 1 == oHeader.nPlanes )
						{
							poConverter = (CPixelConverter*) new CRgb4x1(oHeader.pPalette16);
						}
						break;

					case 8:
						switch( oHeader.nPlanes )
						{
						case 1:	
							if( oHeader.bGreyScale )
								poConverter = (CPixelConverter*) new CGrayscale8x1();
							else
								poConverter = (CPixelConverter*) new CRgb8x1(oHeader.pPalette256);
							break;
						case 3: 
							poConverter = (CPixelConverter*) new CRgb8x3(); 
							break;
						}
						break;
					
					case 24:
						if( 1 == oHeader.nPlanes )
						{
							poConverter = (CPixelConverter*) new CRgb24x1();
						}
						break;
					}

					return poConverter;
				}
			};

		};
		class Tga
		{
		public:
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				// выходной формат всегда 32bpp
				nFormat = c_nFormat_Uncompressed_32bpp;

				if (NULL == pResultImage)
					return FALSE;

				// открываем файл
				HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE == hFile)
					return FALSE; // Невозможно открыть файл

				// мапим этот файл в память - так быстрее читаются данные из файла
				DWORD  nFileSize = GetFileSize(hFile, NULL);
				HANDLE hMapFile  = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
				if (NULL == hMapFile)
				{
					CloseHandle( hFile );
					return FALSE; // Невозможно создать отображение файла
				}

				// создаем view of file
				DWORD nMaxBytesRead	= nFileSize;
				VOID* pBaseAddress	= MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
				BYTE* pnFile		= (BYTE*)pBaseAddress;
				BOOL  bRetValue		= FALSE;
				
				while( pnFile )
				{
					DWORD nBytesRead;

					// чтение заголовка 
					CHeaderInfo oHeader;
					nBytesRead = oHeader.Load(pnFile, nMaxBytesRead);
					if (nBytesRead < 1)
						break; // Заголовок файла не верен

					pnFile += nBytesRead;
					nMaxBytesRead -= nBytesRead;

					// создание изображения
					if (!pResultImage->FromByteArray( oHeader.GetWidth(), oHeader.GetHeight() ))
						break; // Не удалось зааллокейтить память под картинку

					ARGB* pclrArr = (ARGB*)pResultImage->GetBuffer();
					if (NULL == pclrArr)
						break; // Инвалидный указатель на данные в файле

					// загрузка изображения
					CImageData oImageData;
					nMaxBytesRead = oImageData.Load(pnFile, nMaxBytesRead, pclrArr, oHeader );
					if (nMaxBytesRead < 1)
						break; // Не могу загрузить графические данные

					bRetValue = TRUE;
					break;
				}

				// закрываем файл
				UnmapViewOfFile(pBaseAddress);
				CloseHandle(hMapFile);
				CloseHandle(hFile);

				return bRetValue;
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat)
			{
				if (NULL == pSourceImage || c_nFormat_Uncompressed_32bpp != nFormat )
					return FALSE;
				
				CAtlFile file;
				if (S_OK != file.Create(strFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS))
					return FALSE; // Не могу создать файл
				
				int nWidth = pSourceImage->GetWidth();
				int nHeight = pSourceImage->GetHeight();

				// сохранение заголовка
				CHeaderInfo oHeader;
				if (false == oHeader.Save(file, nWidth, nHeight))
					return FALSE; // Данные заголовка не верны
				
				// сохранение изображения
				Gdiplus::Bitmap* pBitmap = pSourceImage->GetBitmap();
				if( NULL == pBitmap )
					return FALSE;

				Gdiplus::Rect oRect(0, 0, nWidth, nHeight);
				BitmapData oLockData;

				if( Ok != pBitmap->LockBits(&oRect, ImageLockModeRead, PixelFormat32bppARGB, &oLockData) )
					return FALSE;
				
				CImageData oImageData;
				BOOL bResult = oImageData.Save(file, (ARGB*)oLockData.Scan0, oHeader);

				pBitmap->UnlockBits( &oLockData );
				
				return bResult;
			}

		protected:
			class CHeaderInfo
			{
			private:

#pragma pack(push,1)
				struct TFileHeader
				{
					BYTE nIdLength;			// Длина текстовой информации после первого 18-ти байтового блока
					BYTE nPaletteType;		// Тип палитры: 0 - отсутствует, 1 - RGB, 2..255 - неизвестный формат
					BYTE nCompression;		// Сжатие данных: 0 - нет, 1 - RLE, 2 - неизвестный тип сжатия
					WORD nPaletteOffset;	// Смещение первого элемента палитры
					WORD nPaletteLength;	// Количество элементов в палитре
					BYTE nPaletteDepth;		// Количество бит на 1 элемент палитры
					WORD nX;				// Смещение изображения по Х
					WORD nY;				// Смещение изображения по Y
					WORD nWidth;			// Ширина изображения
					WORD nHeight;		// Высота изображения
					BYTE nDepth;			// Количество бит на пиксель
					BYTE nImageId;			// Описание изображения
				};
#pragma pack(pop)

			private:
				int		m_nWidth;
				int		m_nHeight;
				int		m_nDepth;
				int		m_nPaletteSize;
				ARGB*	m_pPalette;
				BOOL	m_bPalette;
				BOOL	m_bGrayScale;
				BOOL	m_bCompressed;
				BOOL	m_bHorFlipping;
				BOOL	m_bVerFlipping;
				BOOL	m_bAlpha;
			public:
				CHeaderInfo()
				{
					m_nWidth		= 0;
					m_nHeight		= 0;
					m_nDepth		= 0;
					m_nPaletteSize	= 0;
					m_pPalette		= NULL;
					m_bPalette		= FALSE;
					m_bGrayScale	= FALSE;
					m_bCompressed	= FALSE;
					m_bHorFlipping	= FALSE;
					m_bVerFlipping	= FALSE;
					m_bAlpha		= FALSE;
				}
				~CHeaderInfo()
				{
					if( m_pPalette ) delete [] m_pPalette;
				}
				DWORD Load(BYTE* pnFile, DWORD nMaxOffset)
				{
					DWORD nBytesRead = sizeof(TFileHeader);

					if( nBytesRead > nMaxOffset )
						return 0;

					const TFileHeader& oHeader = *(TFileHeader*)pnFile;

					// проверка файлового заголовка
					switch( oHeader.nPaletteType )
					{
					case 0: m_bPalette = FALSE; break;
					case 1: m_bPalette = TRUE;	break;
					default: return 0;
					}

					switch( oHeader.nCompression )
					{
					case 1:  m_bCompressed = FALSE; m_bGrayScale = FALSE; break;
					case 2:  m_bCompressed = FALSE; m_bGrayScale = FALSE; break;
					case 3:  m_bCompressed = FALSE; m_bGrayScale = TRUE;  break;
					case 9:  m_bCompressed = TRUE;  m_bGrayScale = FALSE; break;
					case 10: m_bCompressed = TRUE;  m_bGrayScale = FALSE; break;
					case 11: m_bCompressed = TRUE;  m_bGrayScale = TRUE;  break;
					default: return 0;
					}
					
					if( 0 == oHeader.nWidth || 0 == oHeader.nHeight )
						return 0;

					m_nHeight = oHeader.nHeight;
					m_nWidth  = oHeader.nWidth;

					switch( oHeader.nDepth )
					{
					case 8:  break;
					case 15: break;
					case 16: break;
					case 24: break;
					case 32: break;
					default: return 0;
					}
					m_nDepth = oHeader.nDepth;

					m_bHorFlipping	= (oHeader.nImageId & 0x10) ? TRUE : FALSE;
					m_bVerFlipping	= (oHeader.nImageId & 0x20) ? FALSE : TRUE;
					m_bAlpha		= (oHeader.nImageId & 0x0F) ? TRUE : FALSE;

					nBytesRead += oHeader.nIdLength;
					if( nBytesRead > nMaxOffset )
						return 0;
					
					if( m_bPalette )
					{
						// создаём палитру
						if( 0 == oHeader.nPaletteLength )
							return 0;

						int nBytesPerPaletteItem = 0;

						switch( oHeader.nPaletteDepth )
						{
						case 24: nBytesPerPaletteItem = 3; break;
						default: return 0;
						}

						nBytesRead += oHeader.nPaletteOffset * nBytesPerPaletteItem;
						if( nBytesRead > nMaxOffset )
							return 0;

						BYTE* pFilePalette = pnFile + nBytesRead;
						nBytesRead += oHeader.nPaletteLength * nBytesPerPaletteItem;
						if( nBytesRead > nMaxOffset )
							return 0;

						m_nPaletteSize = oHeader.nPaletteLength;

						if( m_pPalette ) 
							delete [] m_pPalette;

						m_pPalette = (ARGB*) new ARGB[m_nPaletteSize];
						if( NULL == m_pPalette )
							return 0;

						for( int nItemCount = 0; nItemCount < m_nPaletteSize; nItemCount++ )
						{
							m_pPalette[nItemCount] = 0xff000000 | RGB( pFilePalette[0], pFilePalette[1], pFilePalette[2] );
							pFilePalette += 3;
						}
					}
					
					return nBytesRead;
				}
				BOOL Save(ATL::CAtlFile& oResultFile, int nWidth, int nHeight)
				{
					if( DWORD(nWidth) > 0xffff || DWORD(nHeight) > 0xffff )
						return FALSE;
					
					m_nWidth		= nWidth;
					m_nHeight		= nHeight;
					m_nDepth		= 32;
					m_nPaletteSize	= 0;
					//m_pPalette		= NULL; // палитра может быть создана (теоретически)
					m_bPalette		= FALSE;
					m_bGrayScale	= FALSE;
					m_bCompressed	= FALSE;
					m_bHorFlipping	= FALSE;
					m_bVerFlipping	= FALSE;
					m_bAlpha		= TRUE;

					CHeaderInfo::TFileHeader oHeader;
					ZeroMemory( &oHeader, sizeof(TFileHeader) );

					oHeader.nPaletteType = 0;
					oHeader.nCompression = 2;
					oHeader.nWidth		 = WORD(m_nWidth);
					oHeader.nHeight		 = WORD(m_nHeight);
					oHeader.nDepth		 = BYTE(m_nDepth);
					oHeader.nImageId	 = 0x28;

					if( S_OK != oResultFile.Write( &oHeader, sizeof(TFileHeader)))
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
				int GetDepth() const 
				{ 
					return m_nDepth; 
				}
				BOOL IsCompressed() const 
				{ 
					return m_bCompressed; 
				}
				BOOL IsPalette() const 
				{ 
					return m_bPalette; 
				}
				BOOL IsGrayScale() const
				{
					return m_bGrayScale;
				}
				BOOL IsHorFlipping() const
				{
					return m_bHorFlipping;
				}
				BOOL IsVerFlipping() const
				{
					return m_bVerFlipping;
				}
				BOOL IsAlpha() const
				{
					return m_bAlpha;
				}
				ARGB* GetPalette() const 
				{ 
					return m_pPalette; 
				}

			};
			class CImageData
			{
			public:
				CImageData() 
				{ 
					m_poPixelConverter = NULL; 
				}
				~CImageData()
				{
					if( m_poPixelConverter )
						delete m_poPixelConverter;
				}
				DWORD Load(BYTE* pnFile, DWORD nMaxOffset, ARGB* pImage, const CHeaderInfo& oHeader )
				{
					int	nWidth = oHeader.GetWidth();
					int	nHeight	= oHeader.GetHeight();
					DWORD nBytesRead = 0;

					CPixelConverter* poConverter = CreatePixelConverter( oHeader );
					if( NULL == poConverter )
						return 0;
					
					int nBytesPerPixel	= (oHeader.GetDepth() + 7) >> 3;  // (value + 7)/8
					int nPixel			= 0;
					int nClrStep		= 1;
					int nClrOffset		= 0;
					int nClrCoef		= nWidth;

					if( oHeader.IsHorFlipping() )
					{
						nClrStep = -1;
						nClrOffset = nWidth - 1;
					}
					if( oHeader.IsVerFlipping() )
					{
						nClrOffset += (nHeight - 1) * nWidth;
						nClrCoef = -nWidth;
					}

					if( oHeader.IsCompressed() )
					{
						DWORD nCount = 0;
						ARGB clrSet = 0;
						
						// считывание по строчкам
						for( int nRow = 0; nRow < nHeight; nRow++ )
						{
							ARGB* pClr	= pImage + nRow * nClrCoef + nClrOffset;
							// считывание по столбцам
							for( int nColumn = 0; nColumn < nWidth; nColumn++ )
							{
								if( 0 == nCount )
								{
									nBytesRead += 1 + nBytesPerPixel;
									if( nBytesRead > nMaxOffset )
										return 0;

									nCount = *pnFile++;
									if( nCount & 128 )
									{
										nCount &= 127;
										nCount++;
									
										poConverter->pChannelBuffer = pnFile;
										clrSet = poConverter->ToRGB( 0 );
										pnFile += nBytesPerPixel;
										nPixel = -1;
									}
									else
									{
										nBytesRead += nBytesPerPixel * nCount;
										if( nBytesRead > nMaxOffset )
											return 0;
										
										nCount++;
										poConverter->pChannelBuffer = pnFile;
										pnFile += nBytesPerPixel * nCount;
										nPixel = 0;
									}
								}
					
								*pClr = ( nPixel < 0 ) ? clrSet : poConverter->ToRGB( nPixel++ );
								pClr += nClrStep;
								--nCount;
							}
						}
					}
					else
					{
						nBytesRead += nWidth * nHeight * nBytesPerPixel;
						if( nBytesRead > nMaxOffset )
							return 0;

						poConverter->pChannelBuffer = pnFile;
						// считывание по строчкам
						for( int nRow = 0; nRow < nHeight; nRow++ )
						{
							ARGB* pClr	= pImage + nRow * nClrCoef + nClrOffset;
							// считывание по столбцам
							for( int nColumn = 0; nColumn < nWidth; nColumn++ )
							{
								*pClr = poConverter->ToRGB( nPixel++ );
								pClr += nClrStep;
							}
						}
					}
					
					return nBytesRead;
				}
				BOOL Save(ATL::CAtlFile& oResultFile, ARGB* pImage, const CHeaderInfo& oHeader)
				{
					if( NULL == pImage )
						return FALSE;

					if( S_OK != oResultFile.Write( pImage, oHeader.GetWidth()*oHeader.GetHeight()*4 ))
						return FALSE;
					
					return TRUE;
				}
			private:
				class CPixelConverter
				{
				public:
					BYTE*	pChannelBuffer;
					int		nChannelOffset;	

				public:
							CPixelConverter() : pChannelBuffer( NULL ), nChannelOffset( 0 ) {}
					virtual ~CPixelConverter() {}; 
					virtual ARGB ToRGB( int nPixel ) = 0;
				};
				class CRgb8x1 : public CPixelConverter
				{
				protected:
					ARGB* m_pPalette;
				public:
					CRgb8x1( ARGB* pPlt ) : m_pPalette( pPlt ) {}

					virtual ARGB ToRGB( int nPixel )
					{
						return m_pPalette[pChannelBuffer[nPixel]];
					}
				};
				class CRgb16x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						WORD clr = *(WORD*)(pChannelBuffer + (nPixel << 1));
						BYTE b = (clr & 0x001f)<<3;
						BYTE g = (clr & 0x03e0)>>2;
						BYTE r = (clr & 0x7c00)>>7;

						b |= (b & 0xe0)>>5;
						g |= (g & 0xe0)>>5;
						r |= (r & 0xe0)>>5;

						return 0xff000000 | RGB( b, g, r );
					}
				};
				class CRgb24x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						nPixel *= 3;
						BYTE b = pChannelBuffer[nPixel];
						BYTE g = pChannelBuffer[nPixel+1];
						BYTE r = pChannelBuffer[nPixel+2];

						return 0xff000000 | RGB( b, g, r );
					}
				};
				class CRgb32x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						return *(ARGB*)(pChannelBuffer + (nPixel << 2)) | 0xff000000;
					}
				};
				class CArgb16x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						WORD clr = *(WORD*)(pChannelBuffer + (nPixel << 1));
						BYTE b = (clr & 0x001f)<<3;
						BYTE g = (clr & 0x03e0)>>2;
						BYTE r = (clr & 0x7c00)>>7;

						b |= (b & 0xe0)>>5;
						g |= (g & 0xe0)>>5;
						r |= (r & 0xe0)>>5;

						return 0xff000000 | RGB( b, g, r ); //| ((clr & 0x8000) ? 0xff000000 : 0);
					}
				};
				class CArgb32x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						return *(ARGB*)(pChannelBuffer + (nPixel << 2));
					}
				};		
				class CGrayScale8x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE clr = pChannelBuffer[nPixel];
						return 0xff000000 | RGB(clr, clr, clr);
					}
				};
			private:
				CPixelConverter* m_poPixelConverter;
			private:
				CPixelConverter* CreatePixelConverter( const CHeaderInfo& oHeader )
				{
					if( m_poPixelConverter )
					{
						delete m_poPixelConverter;
						m_poPixelConverter = NULL;
					}

					CPixelConverter* poConverter = NULL;

					switch( oHeader.GetDepth() )
					{
					case 8:
						if( oHeader.IsPalette() )
							poConverter = (CPixelConverter*) new CRgb8x1( oHeader.GetPalette() );
						else if( oHeader.IsGrayScale() )
							poConverter = (CPixelConverter*) new CGrayScale8x1();
						break;
					
					case 15: poConverter = (CPixelConverter*) new CRgb16x1();
						break;

					case 16: 
						if( oHeader.IsAlpha() )
							poConverter = (CPixelConverter*) new CArgb16x1();
						else
							poConverter = (CPixelConverter*) new CRgb16x1();
						break;
					
					case 24: poConverter = (CPixelConverter*) new CRgb24x1();
						break;

					case 32: 
						if( oHeader.IsAlpha() )
							poConverter = (CPixelConverter*) new CArgb32x1();
						else
							poConverter = (CPixelConverter*) new CRgb32x1();
						break;
					}

					m_poPixelConverter = poConverter;
					return poConverter;
				}
			};
		};
		class Ras
		{
		public:
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				// выходной формат всегда 32bpp
				nFormat = c_nFormat_Uncompressed_32bpp;

				if (NULL == pResultImage)
					return FALSE;

				// открываем файл
				HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE == hFile)
					return FALSE; // Невозможно открыть файл

				// мапим этот файл в память - так быстрее читаются данные из файла
				DWORD  nFileSize = GetFileSize(hFile, NULL);
				HANDLE hMapFile  = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
				if (NULL == hMapFile)
				{
					CloseHandle( hFile );
					return FALSE; // Невозможно создать отображение файла
				}

				// создаем view of file
				DWORD nMaxBytesRead	= nFileSize;
				VOID* pBaseAddress	= MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
				BYTE* pnFile		= (BYTE*)pBaseAddress;
				BOOL  bRetValue		= FALSE;
				
				while( pnFile )
				{
					DWORD nBytesRead;

					// чтение заголовка 
					CHeaderInfo oHeader;
					nBytesRead = oHeader.Load(pnFile, nMaxBytesRead);
					if (nBytesRead < 1)
						break; // Заголовок файла не верен

					pnFile += nBytesRead;
					nMaxBytesRead -= nBytesRead;

					// создание изображения
					if (!pResultImage->FromByteArray( oHeader.GetWidth(), oHeader.GetHeight() ))
						break; // Не удалось зааллокейтить память под картинку

					ARGB* pclrArr = (ARGB*)pResultImage->GetBuffer();
					if (NULL == pclrArr)
						break; // Инвалидный указатель на данные в файле

					// загрузка изображения
					CImageData oImageData;
					nMaxBytesRead = oImageData.Load(pnFile, nMaxBytesRead, pclrArr, oHeader );
					if (nMaxBytesRead < 1)
						break; // Не могу загрузить графические данные

					bRetValue = TRUE;
					break;
				}

				// закрываем файл
				UnmapViewOfFile(pBaseAddress);
				CloseHandle(hMapFile);
				CloseHandle(hFile);

				return bRetValue;
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat)
			{
				if (NULL == pSourceImage || c_nFormat_Uncompressed_32bpp != nFormat )
					return FALSE;
				
				CAtlFile file;
				if (S_OK != file.Create(strFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS))
					return FALSE; // Не могу создать файл
				
				int nWidth = pSourceImage->GetWidth();
				int nHeight = pSourceImage->GetHeight();

				// сохранение заголовка
				CHeaderInfo oHeader;
				if (false == oHeader.Save(file, nWidth, nHeight))
					return FALSE; // Данные заголовка не верны
				
				// сохранение изображения
				Gdiplus::Bitmap* pBitmap = pSourceImage->GetBitmap();
				if( NULL == pBitmap )
					return FALSE;

				Gdiplus::Rect oRect(0, 0, nWidth, nHeight);
				BitmapData oLockData;

				if( 
					Ok != pBitmap->LockBits(&oRect, ImageLockModeRead, PixelFormat32bppARGB, &oLockData) )
					return FALSE;
				
				CImageData oImageData;
				BOOL bResult = oImageData.Save(file, (ARGB*)oLockData.Scan0, oHeader);

				pBitmap->UnlockBits( &oLockData );
				
				return bResult;
			}

		protected:
			class CHeaderInfo
			{
			private:
#pragma pack(push,1)
				struct TFileHeader
				{
					DWORD nMagicNumber;     // Magic (identification) number 
					DWORD nWidth;           // Width of image in pixels 
					DWORD nHeight;          // Height of image in pixels 
					DWORD nDepth;           // Number of bits per pixel 
					DWORD nSize;            // Size of image data in bytes
					DWORD nType;            // Type of raster file 
					DWORD nPaletteType;     // Type of color map 
					DWORD nPaletteLength;   // Size of the color map in bytes 
				};
#pragma pack(pop)

			private:
				int			m_nWidth;
				int			m_nHeight;
				int			m_nDepth;
				int         m_nBytesPerLine;
				int			m_nPaletteSize;
				ARGB*	m_pPalette;
				BOOL		m_bPalette;
				BOOL		m_bCompressed;
			private:
				static DWORD ReverseDword(DWORD dwValue)
				{
					DWORD dwResult;

					((BYTE*)&dwResult)[0] = ((BYTE*)&dwValue)[3];
					((BYTE*)&dwResult)[1] = ((BYTE*)&dwValue)[2];
					((BYTE*)&dwResult)[2] = ((BYTE*)&dwValue)[1];
					((BYTE*)&dwResult)[3] = ((BYTE*)&dwValue)[0];
					
					return dwResult;
				}

			public:
				CHeaderInfo()
				{
					m_nWidth		= 0;
					m_nHeight		= 0;
					m_nDepth		= 0;
					m_nBytesPerLine = 0;
					m_nPaletteSize	= 0;
					m_pPalette		= NULL;
					m_bCompressed	= FALSE;
					m_bPalette		= FALSE;
				}
				~CHeaderInfo()
				{
					if( m_pPalette )
						delete m_pPalette;
				}
				DWORD Load(BYTE* pnFile, DWORD nMaxOffset)
				{
					DWORD nBytesRead = sizeof(TFileHeader);
					if( nBytesRead > nMaxOffset )
						return 0;

					const TFileHeader& oHeader = *(TFileHeader*)pnFile;

					if( 0x59a66a95 != ReverseDword( oHeader.nMagicNumber ) )
						return 0;

					m_nWidth  = ReverseDword( oHeader.nWidth );
					m_nHeight = ReverseDword( oHeader.nHeight );
					if( 0 == m_nWidth || 0 == m_nHeight )
						return 0;

					m_nDepth = ReverseDword( oHeader.nDepth );
					switch( m_nDepth )
					{
					case 1:  break;
					case 8:  break;
					case 24: break;
					case 32: break;
					default: return 0;
					}

					m_nBytesPerLine = ReverseDword(oHeader.nSize) / m_nHeight;
					if( m_nBytesPerLine < 2 )
						return 0;

					switch( ReverseDword(oHeader.nPaletteType) )
					{
					case 0: m_bPalette = FALSE; break;
					case 1: m_bPalette = TRUE; break;
					default: return 0;
					}

					m_bCompressed = FALSE;
					switch( ReverseDword(oHeader.nType) )
					{
					case 0: break; // old format
					case 1: break; // standart format
					case 2: m_bCompressed = TRUE; break;
					case 3: break; // RGB format
					case 4: break; // convert from TIFF format
					case 5: break; // convert from IFF format
					default: return 0;
					}

					if( m_bPalette )
					{
						if( m_pPalette )
						{
							delete m_pPalette;
							m_pPalette = NULL;
						}

						m_nPaletteSize = ReverseDword( oHeader.nPaletteLength );
						if( 0 == m_nPaletteSize )
							return 0;

						pnFile += nBytesRead;
						nBytesRead += m_nPaletteSize;
						if( nBytesRead > nMaxOffset )
							return 0;

						m_nPaletteSize /= 3;
						m_pPalette = (ARGB*)new ARGB[m_nPaletteSize];
						if( NULL == m_pPalette )
							return 0;

						for( int nCount = 0; nCount < m_nPaletteSize; ++nCount, ++pnFile )
						{
							BYTE r = pnFile[0];
							BYTE g = pnFile[m_nPaletteSize];
							BYTE b = pnFile[m_nPaletteSize*2];

							m_pPalette[nCount] = 0xff000000 | RGB( b, g, r );
						}
					}

					return nBytesRead;
				}
				BOOL Save(ATL::CAtlFile& oResultFile, int nWidth, int nHeight)
				{
					if( nWidth < 1 || nHeight < 1 )
						return FALSE;

					m_nWidth        = nWidth;
					m_nHeight       = nHeight;
					m_nDepth        = 24;
					m_nBytesPerLine = ((nWidth * m_nDepth + 7)/8 + 1) & ~1;
					m_bPalette      = FALSE;
					m_bCompressed   = FALSE;

					TFileHeader oHeader;

					oHeader.nMagicNumber   = ReverseDword( 0x59a66a95 );
					oHeader.nWidth         = ReverseDword( m_nWidth );
					oHeader.nHeight        = ReverseDword( m_nHeight );
					oHeader.nDepth         = ReverseDword( m_nDepth );
					oHeader.nSize          = ReverseDword( m_nBytesPerLine * m_nHeight);
					oHeader.nType          = ReverseDword( 1 );
					oHeader.nPaletteType   = ReverseDword( 0 );
					oHeader.nPaletteLength = ReverseDword( 0 );

					if( S_OK != oResultFile.Write( &oHeader, sizeof(TFileHeader) ) )
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
				int GetDepth() const 
				{ 
					return m_nDepth; 
				}
				int GetSizeOfLine() const
				{
					return m_nBytesPerLine;
				}
				BOOL IsPalette() const 
				{
					return m_bPalette;
				}
				BOOL IsCompressed() const
				{
					return m_bCompressed;
				}
				ARGB* GetPalette() const
				{
					return m_pPalette;
				}
			};
			class CImageData
			{
			public:
				CImageData() 
				{ 
					m_poPixelConverter = NULL; 
				}
				~CImageData()
				{
					if( m_poPixelConverter )
						delete m_poPixelConverter;
				}
				DWORD Load(BYTE* pnFile, DWORD nMaxOffset, ARGB* pImage, const CHeaderInfo& oHeader )
				{
					DWORD nBytesRead = 0;
					int	nWidth = oHeader.GetWidth();
					int	nHeight	= oHeader.GetHeight();
					int nBytesPerLine = oHeader.GetSizeOfLine();

					CPixelConverter* poConverter = CreatePixelConverter( oHeader );
					if( NULL == poConverter )
						return 0;

					if( oHeader.IsCompressed() )
					{
						BYTE* pBuffer = (BYTE*)new BYTE[nBytesPerLine];
						if( NULL == pBuffer )
							return 0;
						
						poConverter->pChannelBuffer = pBuffer;

						int nCurrentBufferSize = 0;
						int nRow = nHeight;
						while( nRow > 0 )
						{
							if( ++nBytesRead > nMaxOffset )
								break;

							int  nCount	= 1;
							BYTE nValue = *pnFile++;
							if( 128 == nValue )
							{
								if( ++nBytesRead > nMaxOffset )
									break;

								nCount = *pnFile++;
								if( nCount > 0 )
								{
									if( ++nBytesRead > nMaxOffset )
										break;

									nValue = *pnFile++;
								}
								nCount++;
							}

							while( nCount-- > 0 )
							{
								pBuffer[nCurrentBufferSize++] = nValue;
								if( nCurrentBufferSize >= nBytesPerLine )
								{
									for( int nColumn = 0; nColumn < nWidth; nColumn++ )
									{
										*pImage++ = poConverter->ToRGB( nColumn );
									}
									nCurrentBufferSize = 0;

									if( --nRow < 1 )
										break;
								}
							}
						}
						delete [] pBuffer;

						if( nRow > 0 )
							return 0;
					}
					else
					{
						nBytesRead += nBytesPerLine * nHeight;
						if( nBytesRead > nMaxOffset )
							return 0;

						poConverter->pChannelBuffer = pnFile;

						for( int nRow = 0; nRow < nHeight; nRow++ )
						{
							for( int nColumn = 0; nColumn < nWidth; nColumn++ )
							{
								*pImage++ = poConverter->ToRGB( nColumn );
							}
							poConverter->pChannelBuffer += nBytesPerLine;
						}
					}
					
					return nBytesRead;
				}
				BOOL Save(ATL::CAtlFile& oResultFile, ARGB* pImage, const CHeaderInfo& oHeader)
				{
					if( NULL == pImage )
						return FALSE;

					int nWidth = oHeader.GetWidth();
					int nHeight = oHeader.GetHeight();
					int nBytesPerLine = oHeader.GetSizeOfLine();

					BYTE* pBuffer = (BYTE*) new BYTE[nBytesPerLine];
					if( NULL == pBuffer )
						return FALSE;

					for( int nRow = 0; nRow < nHeight; nRow++ )
					{
						BYTE* pPos = pBuffer;
						for( int nColumn = 0; nColumn < nWidth; ++nColumn )
						{
							BYTE* pData = (BYTE*)pImage++;
							*pPos++ = pData[0];
							*pPos++ = pData[1];
							*pPos++ = pData[2];
						}
						if( S_OK != oResultFile.Write( pBuffer, nBytesPerLine ))
						{
							delete [] pBuffer;
							return FALSE;
						}
					}

					return TRUE;
				}
			private:
				class CPixelConverter
				{
				public:
					BYTE*	pChannelBuffer;
					int		nChannelOffset;	

				public:
							CPixelConverter() : pChannelBuffer( NULL ), nChannelOffset( 0 ) {}
					virtual ~CPixelConverter() {}; 
					virtual ARGB ToRGB( int nPixel ) = 0;
				};
				class CIndexed1x1 : CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE clrs = pChannelBuffer[nPixel>>3]; // Pixel/8
						
						if( clrs & (1 << (7 - (nPixel & 0x7))) )
							return 0xffffffff;
						else
							return 0xff000000;
					}
				};
				class CIndexed8x1 : CPixelConverter
				{
				protected:
					ARGB* m_pPalette;
				public:
					CIndexed8x1( ARGB* pPlt ) : m_pPalette( pPlt ) {}

					virtual ARGB ToRGB( int nPixel )
					{
						return m_pPalette[pChannelBuffer[nPixel]];
					}
				};
				class CGrayScale8x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE clr = pChannelBuffer[nPixel];
						return 0xff000000 | RGB(clr, clr, clr);
					}
				};
				class CBitmap1x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						BYTE clrs = pChannelBuffer[nPixel>>3]; // Pixel/8
						
						if( clrs & (1 << (7 - (nPixel & 0x7))) )
							return 0xff000000;
						else
							return 0xffffffff;
					}
				};
				class CRgb24x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						nPixel *= 3;
						BYTE b = pChannelBuffer[nPixel];
						BYTE g = pChannelBuffer[nPixel+1];
						BYTE r = pChannelBuffer[nPixel+2];

						return 0xff000000 | RGB( b, g, r );
					}
				};
				class CRgb32x1 : public CPixelConverter
				{
				public:
					virtual ARGB ToRGB( int nPixel )
					{
						return *(ARGB*)(pChannelBuffer + (nPixel << 2));
					}
				};
			private:
				CPixelConverter* m_poPixelConverter;
			private:
				CPixelConverter* CreatePixelConverter( const CHeaderInfo& oHeader )
				{
					if( m_poPixelConverter )
					{
						delete m_poPixelConverter;
						m_poPixelConverter = NULL;
					}

					CPixelConverter* poConverter = NULL;

					switch( oHeader.GetDepth() )
					{
					case 1:	
						if( oHeader.IsPalette() )
							poConverter = (CPixelConverter*) new CIndexed1x1();
						else
							poConverter = (CPixelConverter*) new CBitmap1x1();
						break;
					case 8:
						if( oHeader.IsPalette() )
							poConverter = (CPixelConverter*) new CIndexed8x1(oHeader.GetPalette());
						else
							poConverter = (CPixelConverter*) new CGrayScale8x1();
						break;
					case 24: poConverter = (CPixelConverter*) new CRgb24x1();
						break;
					case 32: poConverter = (CPixelConverter*) new CRgb32x1();
						break;
					}

					m_poPixelConverter = poConverter;
					return poConverter;
				}
			};
		};
		class Raw
		{
		public:
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage, const CString& strSettingXML)
			{
				return FALSE;
			}
		};

		class APng
		{
		public:
			//struct png_struct_def
			//{
			//	z_stream zstream1;
			//	z_stream zstream2;
			//	BYTE  * zbuf1;
			//	BYTE  * zbuf2;
			//	size_t   zbuf_size;

			//	UINT   width;
			//	UINT   height;
			//	UINT   rowbytes;
			//	UINT   idat_size;
			//	UINT   pixel_depth;
			//	UINT   bpp;

			//	UINT   next_seq_num;
			//	UINT   num_frames_written;

			//	BYTE  * big_row_buf;
			//	BYTE  * row_buf;
			//	BYTE  * prev_row;
			//	BYTE  * sub_row;
			//	BYTE  * up_row;
			//	BYTE  * avg_row;
			//	BYTE  * paeth_row;
			//	UINT * temp_row;

			//	BYTE    color_type;
			//	BYTE    bit_depth;
			//};

		
		public :
			APng()
			{
				mNumFrames = 0;
				mNumLoops = 0;
				mWidth = 0;
				mHeight = 0;
				hasTRNS = 0;

				m_pBuffer = NULL;
				mUnpackBuffer = NULL;

				mRestoreBuffer = NULL;
				mFrameBuffer = NULL;
			}
			~APng()
			{
				Clear ();
			}

			
			BOOL ParseFile( CString sFileName )
			{
				return FALSE;
			//	UINT i;
			//	UINT numFCTL = 0;

			//	Clear();

			//	CAtlFile oOpenFile;
			//	oOpenFile.Create ( sFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING );

			//	ULONGLONG len64 = 0; oOpenFile.GetSize ( len64 );

			//	if (len64 > 0x3FFFFFFF)
			//		int n = 0;
			////		throw MyError("The PNG image \"%ls\" is too large to read.", filename);

			//	UINT len = (UINT)len64;

			//	m_pBuffer = new BYTE [ len ];

			//	oOpenFile.Read ( m_pBuffer, len );

			//	for (i=0; i<256; i++)
			//		mPalette[i] = 0x010101*i + 0xFF000000;

			//	const BYTE *src = m_pBuffer;
			//	UINT pos = 0;
			//	if (len - pos < 8 || src[0] != 137 || src[1] != 'P' || src[2] != 'N' || src[3] != 'G' || src[4] != 13 || src[5] != 10 || src[6] != 26 || src[7] != 10)
			//		int n = 0;
			////		throw MyError("File \"%ls\" is not a PNG file.", filename);

			//	pos += 8;

			//	if (len - pos < 25)
			//		int n = 0;
			////		throw MyError("File \"%ls\" is an invalid PNG file.", filename);

			//	UINT length = VDReadUnalignedBEU32(&src[pos]);

			//	if (length != 13 || src[pos+4] != 'I' || src[pos+5] != 'H' || src[pos+6] != 'D' || src[pos+7] != 'R')
			//		int n = 0;
			////		throw MyError("IHDR missing in \"%ls\" file.", filename);

			//	pos += 8;

			//	mWidth           = VDReadUnalignedBEU32(&src[pos]);
			//	mHeight          = VDReadUnalignedBEU32(&src[pos+4]);
			//	mBitDepth        = src[pos+8];
			//	mColorType       = src[pos+9];

			//	if (mWidth == 0 || mHeight == 0)
			//		int n = 0;
			////		throw MyError("Image width or height is zero");

			//	if (mBitDepth != 1 && mBitDepth != 2 && mBitDepth != 4 && mBitDepth != 8 && mBitDepth != 16)
			//		int n = 0;
			////		throw MyError("BitDepth = %d is not supported", mBitDepth);

			//	if (mColorType != 0 && mColorType != 2 && mColorType != 3 && mColorType != 4 && mColorType != 6)
			//		int n = 0;
			////		throw MyError("ColorType = %d is not supported", mColorType);

			//	if (((mColorType == PNG_COLOR_TYPE_PALETTE) && mBitDepth > 8) ||
			//		((mColorType == PNG_COLOR_TYPE_RGB ||
			//		mColorType == PNG_COLOR_TYPE_GRAY_ALPHA ||
			//		mColorType == PNG_COLOR_TYPE_RGB_ALPHA) && mBitDepth < 8))
			//		int n = 0;
			////		throw MyError("Invalid color type/bit depth combination");

			//	if (src[pos+10] != PNG_COMPRESSION_TYPE_BASE)
			//		int n = 0;
			////		throw MyError("Unknown compression method %d", src[pos+10]);

			//	if (src[pos+11] != PNG_FILTER_TYPE_BASE)
			//		int n = 0;
			////		throw MyError("Filter method %d is not supported", src[pos+11]);

			//	if (src[pos+12] > PNG_INTERLACE_ADAM7)
			//		int n = 0;
			////		throw MyError("Unknown interlace method %d", src[pos+12]);

			//	switch (mColorType)
			//	{
			//		case PNG_COLOR_TYPE_GRAY:
			//		case PNG_COLOR_TYPE_PALETTE:
			//			mPixelDepth = mBitDepth;
			//			break;
			//		case PNG_COLOR_TYPE_RGB:
			//			mPixelDepth = mBitDepth*3;
			//			break;
			//		case PNG_COLOR_TYPE_GRAY_ALPHA:
			//			mPixelDepth = mBitDepth*2;
			//			break;
			//		case PNG_COLOR_TYPE_RGB_ALPHA:
			//		default:
			//			mPixelDepth = mBitDepth*4;
			//			break;
			//	}

			//	pos += (length+4);

			//	// parse chunks
			//	UINT      idat_pos = -1;
			//	CAtlArray < DOUBLE > presentationTimes;
			////	VDFraction	timebase;
			////	VDFraction	spantotal;

			//	DOUBLE delay;

			////	uint32 spancount = 0;
			////	timebase.Assign(0, 1);
			////	spantotal.Assign(0, 1);
			//	ImageInfo imageinfo = { 0, 0, 0, 0, 0, PNG_DISPOSE_OP_NONE, PNG_BLEND_OP_SOURCE };
			//	for(;;) 
			//	{
			//		if (len - pos < 8)
			//			break;

			//		length = VDReadUnalignedBEU32(&src[pos]);

			//		if (src[pos+4] == 'P' && src[pos+5] == 'L' && src[pos+6] == 'T' && src[pos+7] == 'E')
			//		{
			//			pos += 8;
			//			if (len - pos < length+4)
			//				break;

			//			for (i=0; (i<length/3 && i<256); i++)
			//			{
			//				mPalette[i] &= 0xFF000000;
			//				mPalette[i] += src[pos+i*3] << 16;
			//				mPalette[i] += src[pos+i*3+1] << 8;
			//				mPalette[i] += src[pos+i*3+2];
			//			}
			//			pos += (length+4);
			//		}
			//		else
			//		if (src[pos+4] == 't' && src[pos+5] == 'R' && src[pos+6] == 'N' && src[pos+7] == 'S')
			//		{
			//			pos += 8;
			//			if (len - pos < length+4)
			//				break;

			//			if (mColorType == PNG_COLOR_TYPE_GRAY)
			//			{
			//				mTRNSg = VDReadUnalignedBEU16(&src[pos]);
			//			}
			//			else
			//			if (mColorType == PNG_COLOR_TYPE_RGB)
			//			{
			//				mTRNSr = VDReadUnalignedBEU16(&src[pos]);
			//				mTRNSg = VDReadUnalignedBEU16(&src[pos+2]);
			//				mTRNSb = VDReadUnalignedBEU16(&src[pos+4]);
			//			}
			//			else
			//			if (mColorType == PNG_COLOR_TYPE_PALETTE)
			//			{
			//				for (i=0; (i<length && i<256); i++)
			//				{
			//					mPalette[i] &= 0x00FFFFFF;
			//					mPalette[i] += src[pos+i] << 24;
			//				}
			//			}
			//			hasTRNS = 1;
			//			pos += (length+4);
			//		}
			//		else
			//		if (src[pos+4] == 'a' && src[pos+5] == 'c' && src[pos+6] == 'T' && src[pos+7] == 'L')
			//		{
			//			pos += 8;
			//			if (len - pos < length+4)
			//				break;

			//			mNumFrames  = VDReadUnalignedBEU32(&src[pos]);
			//			mNumLoops   = VDReadUnalignedBEU32(&src[pos+4]);

			//			pos += (length+4);
			//		}
			//		else
			//		if (src[pos+4] == 'f' && src[pos+5] == 'c' && src[pos+6] == 'T' && src[pos+7] == 'L')
			//		{
			//			pos += 8;
			//			if (len - pos < length+4)
			//				break;

			//			imageinfo.mWidth   = VDReadUnalignedBEU32(&src[pos+4]);
			//			imageinfo.mHeight  = VDReadUnalignedBEU32(&src[pos+8]);
			//			imageinfo.mXOffset = VDReadUnalignedBEU32(&src[pos+12]);
			//			imageinfo.mYOffset = VDReadUnalignedBEU32(&src[pos+16]);
			//			WORD delay_num   = VDReadUnalignedBEU16(&src[pos+20]);
			//			WORD delay_den   = VDReadUnalignedBEU16(&src[pos+22]);
			//			imageinfo.mDispose = src[pos+24];
			//			imageinfo.mBlend   = src[pos+25];

			//			if (!delay_num)
			//				delay_num = 1;
			//			if (!delay_den)
			//				delay_den = 100;

			//			int n = 0;

			///*			
			//			delay.Assign(delay_num, delay_den);
			//			timebase += delay;
			//			presentationTimes.push_back(timebase);

			//			if (presentationTimes.size() > 2) {
			//				spantotal += timebase - *(presentationTimes.end() - 3);
			//				++spancount;
			//			}
			//*/
			//			if ((numFCTL==0) && (imageinfo.mDispose == PNG_DISPOSE_OP_PREVIOUS))
			//				imageinfo.mDispose = PNG_DISPOSE_OP_BACKGROUND;

			//			if (!(mColorType & PNG_COLOR_MASK_ALPHA) && !(hasTRNS))
			//				imageinfo.mBlend = PNG_BLEND_OP_SOURCE;

			//			numFCTL++;
			//			pos += (length+4);
			//		}
			//		else
			//		if (src[pos+4] == 'I' && src[pos+5] == 'D' && src[pos+6] == 'A' && src[pos+7] == 'T')
			//		{
			//			if (len - pos < length+12)
			//				break;

			//			if (numFCTL == 0)
			//			{
			//				if (idat_pos == -1)
			//				idat_pos = pos;
			//			}
			//			else
			//			if ((numFCTL == 1) && mImages.IsEmpty())
			//			{
			//				imageinfo.mOffsetAndKey = pos | 0x80000000;
			//				mImages.Add(imageinfo);
			//			}
			//			pos += (length+12);
			//		}
			//		else
			//		if (src[pos+4] == 'f' && src[pos+5] == 'd' && src[pos+6] == 'A' && src[pos+7] == 'T')
			//		{
			//			if (len - pos < length+12)
			//				break;

			//			if (mImages.GetCount() == numFCTL - 1)
			//			{
			//				imageinfo.mOffsetAndKey = pos;
			//				if (mImages.IsEmpty() || (!imageinfo.mXOffset && !imageinfo.mYOffset && imageinfo.mWidth == mWidth && imageinfo.mHeight == mHeight && imageinfo.mBlend == PNG_BLEND_OP_SOURCE))
			//					imageinfo.mOffsetAndKey |= 0x80000000;
			//				mImages.Add(imageinfo);
			//			}
			//			pos += (length+12);
			//		}
			//		else
			//		if (src[pos+4] == 'I' && src[pos+5] == 'E' && src[pos+6] == 'N' && src[pos+7] == 'D')
			//		{
			//			break;
			//		}
			//		else
			//		if (src[pos+4] == 0 && src[pos+5] == 0 && src[pos+6] == 0 && src[pos+7] == 0)
			//		{
			//			break;
			//		}
			//		else
			//		{
			//			if (len - pos < length+12)
			//				break;

			//			pos += (length+12);
			//		}
			//	}

			//	if (mImages.IsEmpty())
			//	{
			//		if (idat_pos > 0)
			//		{
			//			imageinfo.mOffsetAndKey = idat_pos | 0x80000000;
			//			imageinfo.mWidth   = mWidth;
			//			imageinfo.mHeight  = mHeight;
			//			imageinfo.mXOffset = 0;
			//			imageinfo.mYOffset = 0;
			//			imageinfo.mDispose = PNG_DISPOSE_OP_NONE;
			//			imageinfo.mBlend   = PNG_BLEND_OP_SOURCE;
			//			mImages.Add(imageinfo);
			//		}
			//		else
			//			int n = 0;
			////			throw MyError("No video frames detected in PNG file.");
			//	}

			//	if (mImages.GetCount() < mNumFrames)


			//		int n = 0;
			////		VDLogF(kVDLogWarning, L"APNG: File appears to be truncated or damaged. \nLoaded %d frames (should be %d). ", mImages.size(), mNumFrames);

			//	mNumFrames = mImages.GetCount();

			//	int n = 0;

			///*
			//	// compute time to frame mapping
			//	mFrameRate.Assign(10, 1);
			//	if (mNumFrames < 3) 
			//	{
			//		if (mNumFrames == 2)
			//		{
			//			mFrameRate.Assign(1, 1);
			//			mFrameRate /= (presentationTimes.back() - presentationTimes.front());
			//		}
			//	} 
			//	else 
			//	{
			//		vdfastvector<ImageInfo> images;
			//		mFrameRate.Assign(spancount * 2, 1);
			//		mFrameRate /= spantotal;

			//		for (i=0; i<mNumFrames; i++) {
			//			VDFraction t = presentationTimes[i] - presentationTimes.front();
			//			int frame = (mFrameRate * t).asInt();

			//			if (frame > images.size()) {
			//				ImageInfo dummy = { 0, 0, 0, 0, 0, PNG_DISPOSE_OP_NONE, PNG_BLEND_OP_SOURCE };
			//				images.resize(frame, dummy);
			//			}

			//			images.push_back(mImages[i]);
			//		}
			//		mImages.swap(images);
			//	}
			//*/
			///*
			//	mbKeyframeOnly = true;
			//	for(Images::const_iterator it(mImages.begin()), itEnd(mImages.end()); it!=itEnd; ++it) {
			//		if (!(it->mOffsetAndKey & 0x80000000))
			//			mbKeyframeOnly = false;
			//	}
			//*/

			//	png_ptr = (png_struct_def*)malloc(sizeof(png_struct_def));

			//	if (png_ptr != NULL)
			//	{
			//		png_ptr->zbuf_size = PNG_ZBUF_SIZE;
			//		png_ptr->zbuf1 = (BYTE *)malloc(png_ptr->zbuf_size);
			//		png_ptr->zstream1.zalloc = Z_NULL;
			//		png_ptr->zstream1.zfree = Z_NULL;
			//		png_ptr->zstream1.opaque = Z_NULL;

			//		inflateInit(&png_ptr->zstream1);

			//		png_ptr->zstream1.next_out = png_ptr->zbuf1;
			//		png_ptr->zstream1.avail_out = (uInt)png_ptr->zbuf_size;

			//		png_ptr->width = mWidth;
			//		png_ptr->height = mHeight;
			//		png_ptr->color_type = mColorType;
			//		png_ptr->bit_depth = mBitDepth;
			//		png_ptr->pixel_depth = mPixelDepth;
			//		png_ptr->bpp = (png_ptr->pixel_depth + 7) >> 3;
			//		png_ptr->rowbytes = ROWBYTES(png_ptr->pixel_depth, png_ptr->width);
			//		png_ptr->idat_size = 0;

			//		int big_width = ((png_ptr->width + 7) & ~((UINT)7));
			//		size_t big_row_bytes = ROWBYTES(png_ptr->pixel_depth, big_width) + 1 + (png_ptr->bpp);

			//		png_ptr->big_row_buf = (BYTE *)malloc(big_row_bytes + 64);
			//		png_ptr->row_buf = png_ptr->big_row_buf + 32;

			//		png_ptr->temp_row = (UINT *)malloc(mWidth * 4);
			//		png_ptr->prev_row = (BYTE *)malloc(png_ptr->rowbytes + 1);

			//		if (png_ptr->prev_row != NULL)
			//			memset(png_ptr->prev_row, 0, png_ptr->rowbytes + 1);
			//	}

			//	mUnpackBuffer = new BYTE [ png_ptr->height * png_ptr->rowbytes ];

			//	mRestoreBuffer = new UINT [ png_ptr->height * png_ptr->rowbytes ];

			//	mFrameBuffer = new UINT [ png_ptr->height * png_ptr->rowbytes ];
			} 
			void Read( MediaCore::IAVSUncompressedVideoFrame* pFrame, int nIndex )
			{
			//	if ( !pFrame )
			//		return;

			////	if (frame_num < 0)
			////		frame_num = target_sample;

			//	const ImageInfo& imageinfo = mImages[frame_num];

			//	pFrame->put_ColorSpace ( CSP_BGRA );

			//	pFrame->put_Width(mWidth);
			//	pFrame->put_Height(mHeight);
			//	pFrame->put_AspectRatioX(mWidth);
			//	pFrame->put_AspectRatioY(mHeight);

			//	pFrame->put_Stride ( 0, ( 32 / 8 ) * mWidth );

			//	m_png_src = m_pBuffer;
			//	png_pos = imageinfo.mOffsetAndKey & 0x7FFFFFFF;

			//	if (/*!data_len ||*/ !png_pos)
			//		int n = 0;
			////		return getFrameBuffer();


			//	if (imageinfo.mOffsetAndKey & 0x80000000)
			//		ZeroMemory ( mFrameBuffer, 4 * mWidth * mHeight );
			////		VDMemset32Rect(mFrameBuffer.data(), mWidth * sizeof(uint32), 0, mWidth, mHeight);

			//	int w = imageinfo.mWidth;
			//	int h = imageinfo.mHeight;
			//	int x = imageinfo.mXOffset;
			//	int y = imageinfo.mYOffset;

			//	// bounds check positions
			//	if ((UINT)x >= mWidth || (UINT)y >= mHeight)
			//		return;
			////		return NULL;

			//	if ((UINT)(x+w) >= mWidth)
			//		w = mWidth - x;

			//	if ((UINT)(y+h) >= mHeight)
			//		h = mHeight - y;

			//	png_ptr->width = imageinfo.mWidth;
			//	png_ptr->height = imageinfo.mHeight;
			//	png_ptr->rowbytes = ROWBYTES(png_ptr->pixel_depth, png_ptr->width);

			//	memset(png_ptr->prev_row, 0, png_ptr->rowbytes + 1);

			//	if (imageinfo.mDispose == PNG_DISPOSE_OP_PREVIOUS)
			//		CopyMemory ( mRestoreBuffer, &mFrameBuffer[x + y*mWidth], 4 * w * h );
			////		VDMemcpyRect(mRestoreBuffer.data(), w * sizeof(uint32), &mFrameBuffer[x + y*mWidth], mWidth * sizeof(uint32), w*sizeof(uint32), h);

			//	int mask4[2]={240,15};
			//	int shift4[2]={4,0};
			//	int gray4[16]={0xFF000000,0xFF111111,0xFF222222,0xFF333333,0xFF444444,0xFF555555,0xFF666666,0xFF777777,0xFF888888,0xFF999999,0xFFAAAAAA,0xFFBBBBBB,0xFFCCCCCC,0xFFDDDDDD,0xFFEEEEEE,0xFFFFFFFF};

			//	int mask2[4]={192,48,12,3};
			//	int shift2[4]={6,4,2,0};
			//	int gray2[4]={0xFF000000,0xFF555555,0xFFAAAAAA,0xFFFFFFFF};

			//	int mask1[8]={128,64,32,16,8,4,2,1};
			//	int shift1[8]={7,6,5,4,3,2,1,0};
			//	int gray1[2]={0xFF000000,0xFFFFFFFF};

			//	UINT r, g, b, a;
			//	UINT r2, g2, b2, a2;
			//	UINT u, v, al;
			//	UINT step = (png_ptr->bit_depth+7)/8;
			//	BYTE  *sp;
			//	UINT *dp;

			//	png_ptr->zstream1.avail_in = 0;

			//	for (int j=0; j<h; j++)
			//		read_row(mUnpackBuffer + j*png_ptr->rowbytes);

			//	inflateReset(&png_ptr->zstream1);

			//	if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)  // INDEXED
			//	{
			//		for (int j=0; j<h; j++)
			//		{
			//			sp = mUnpackBuffer + j*png_ptr->rowbytes;
			//			if (imageinfo.mBlend == PNG_BLEND_OP_OVER)
			//				dp = png_ptr->temp_row;
			//			else
			//				dp = &mFrameBuffer[(y+j)*mWidth+x];

			//			if (png_ptr->bit_depth == 8)
			//			{
			//				for (int i=0; i<w; i++)
			//					*dp++ = mPalette[*sp++];
			//			}
			//			else
			//			if (png_ptr->bit_depth == 4)
			//			{
			//				for (int i=0; i<w; i++)
			//					*dp++ = mPalette[((*(sp+(i>>1))) & mask4[i&1]) >> shift4[i&1]];
			//			}
			//			else
			//			if (png_ptr->bit_depth == 2)
			//			{
			//				for (int i=0; i<w; i++)
			//					*dp++ = mPalette[((*(sp+(i>>2))) & mask2[i&3]) >> shift2[i&3]];
			//			}
			//			else
			//			if (png_ptr->bit_depth == 1)
			//			{
			//				for (int i=0; i<w; i++)
			//					*dp++ = mPalette[((*(sp+(i>>3))) & mask1[i&7]) >> shift1[i&7]];
			//			}
			//			if (imageinfo.mBlend == PNG_BLEND_OP_OVER)
			//			{
			//				sp = (BYTE *)png_ptr->temp_row;
			//				dp = &mFrameBuffer[(y+j)*mWidth+x];
			//				for (int i=0; i<w; i++)
			//				{
			//					r = *sp++;
			//					g = *sp++;
			//					b = *sp++;
			//					a = *sp++;
			//					if (a == 255)
			//						*dp++ = (a << 24) + (b << 16) + (g << 8) + r;
			//					else
			//					if (a != 0)
			//					{
			//						if (a2 = (*dp)>>24)
			//						{
			//							u = a*255;
			//							v = (255-a)*a2;
			//							al = 255*255-(255-a)*(255-a2);
			//							r2 = ((*dp)&255);
			//							g2 = (((*dp)>>8)&255);
			//							b2 = (((*dp)>>16)&255);
			//							r = (r*u + r2*v)/al;
			//							g = (g*u + g2*v)/al;
			//							b = (b*u + b2*v)/al;
			//							a = al/255;
			//						}
			//						*dp++ = (a << 24) + (b << 16) + (g << 8) + r;
			//					}
			//					else
			//						dp++;
			//				}
			//			}
			//		}
			//	}
			//	else if (png_ptr->color_type == PNG_COLOR_TYPE_RGB)        // TRUECOLOR
			//	{
			//		for (int j=0; j<h; j++)
			//		{
			//			sp = mUnpackBuffer + j*png_ptr->rowbytes;
			//			dp = &mFrameBuffer[(y+j)*mWidth+x];

			//			if (imageinfo.mBlend == PNG_BLEND_OP_SOURCE)
			//			{
			//				if (png_ptr->bit_depth == 8)
			//				{
			//					if (!hasTRNS)
			//					for (int i=0; i<w; i++, sp+=3)
			//						*dp++ = 0xFF000000 + (*sp << 16) + (*(sp+1) << 8) + *(sp+2);
			//					else
			//					for (int i=0; i<w; i++, sp+=3)
			//						if ((*sp != mTRNSr) || (*(sp+1) != mTRNSg) || (*(sp+2) != mTRNSb))
			//							*dp++ = 0xFF000000 + (*sp << 16) + (*(sp+1) << 8) + *(sp+2);
			//						else
			//							*dp++ = 0;
			//				}
			//				else
			//				{
			//					if (!hasTRNS)
			//					for (int i=0; i<w; i++, sp+=6)
			//						*dp++ = 0xFF000000 + (*sp << 16) + (*(sp+2) << 8) + *(sp+4);
			//					else
			//					for (int i=0; i<w; i++, sp+=6, dp++)
			//						if ((VDReadUnalignedBEU16(sp) != mTRNSr) || (VDReadUnalignedBEU16(sp+2) != mTRNSg) || (VDReadUnalignedBEU16(sp+4) != mTRNSb))
			//							*dp++ = 0xFF000000 + (*sp << 16) + (*(sp+2) << 8) + *(sp+4);
			//						else
			//							*dp++ = 0;
			//				}
			//			}
			//			else // PNG_BLEND_OP_OVER
			//			{
			//				if (png_ptr->bit_depth == 8)
			//				{
			//					for (int i=0; i<w; i++, sp+=3, dp++)
			//						if ((*sp != mTRNSr) || (*(sp+1) != mTRNSg) || (*(sp+2) != mTRNSb))
			//							*dp = 0xFF000000 + (*sp << 16) + (*(sp+1) << 8) + *(sp+2);
			//				}
			//				else
			//				{
			//					for (int i=0; i<w; i++, sp+=6, dp++)
			//						if ((VDReadUnalignedBEU16(sp) != mTRNSr) || (VDReadUnalignedBEU16(sp+2) != mTRNSg) || (VDReadUnalignedBEU16(sp+4) != mTRNSb))
			//							*dp = 0xFF000000 + (*sp << 16) + (*(sp+2) << 8) + *(sp+4);
			//				}
			//			}
			//		}
			//	}
			//	else if (png_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)  // TRUECOLOR + ALPHA
			//	{
			//		for (int j=0; j<h; j++)
			//		{
			//			sp = mUnpackBuffer + j*png_ptr->rowbytes;
			//			dp = &mFrameBuffer[(y+j)*mWidth+x];

			//			if (imageinfo.mBlend == PNG_BLEND_OP_SOURCE)
			//			{
			//				if (png_ptr->bit_depth == 8)
			//				{
			//					for (int i=0; i<w; i++, sp+=4)
			//						*dp++ = (*(sp+3) != 0) ? (*(sp+3) << 24) + (*sp << 16) + (*(sp+1) << 8) + *(sp+2) : 0;
			//				}
			//				else
			//				{
			//					for (int i=0; i<w; i++, sp+=8)
			//						*dp++ = (*(sp+6) != 0) ? (*(sp+6) << 24) + (*sp << 16) + (*(sp+2) << 8) + *(sp+4) : 0;
			//				}
			//			}
			//			else // PNG_BLEND_OP_OVER
			//			{
			//				for (int i=0; i<w; i++)
			//				{
			//					b = *sp; sp += step;
			//					g = *sp; sp += step;
			//					r = *sp; sp += step;
			//					a = *sp; sp += step;
			//					if (a == 255)
			//						*dp++ = (a << 24) + (b << 16) + (g << 8) + r;
			//					else
			//					if (a != 0)
			//					{
			//						if (a2 = (*dp)>>24)
			//						{
			//							u = a*255;
			//							v = (255-a)*a2;
			//							al = 255*255-(255-a)*(255-a2);
			//							r2 = ((*dp)&255);
			//							g2 = (((*dp)>>8)&255);
			//							b2 = (((*dp)>>16)&255);
			//							r = (r*u + r2*v)/al;
			//							g = (g*u + g2*v)/al;
			//							b = (b*u + b2*v)/al;
			//							a = al/255;
			//						}
			//						*dp++ = (a << 24) + (b << 16) + (g << 8) + r;
			//					}
			//					else
			//						dp++;
			//				}
			//			}
			//		}
			//	}
			//	else if (png_ptr->color_type == PNG_COLOR_TYPE_GRAY)  // GRAYSCALE 
			//	{
			//		for (int j=0; j<h; j++)
			//		{
			//			sp = mUnpackBuffer + j*png_ptr->rowbytes;
			//			dp = &mFrameBuffer[(y+j)*mWidth+x];

			//			if (imageinfo.mBlend == PNG_BLEND_OP_SOURCE)
			//			{
			//				if (png_ptr->bit_depth == 16)
			//				{
			//					for (int i=0; i<w; i++, sp+=2)
			//						*dp++ = 0xFF000000 + (*sp << 16) + (*sp << 8) + *sp;
			//				}
			//				else
			//				if (png_ptr->bit_depth == 8)
			//				{
			//					for (int i=0; i<w; i++, sp++)
			//						*dp++ = 0xFF000000 + (*sp << 16) + (*sp << 8) + *sp;
			//				}
			//				else
			//				if (png_ptr->bit_depth == 4)
			//				{
			//					for (int i=0; i<w; i++)
			//						*dp++ = gray4[((*(sp+(i>>1))) & mask4[i&1]) >> shift4[i&1]];
			//				}
			//				else
			//				if (png_ptr->bit_depth == 2)
			//				{
			//					for (int i=0; i<w; i++)
			//						*dp++ = gray2[((*(sp+(i>>2))) & mask2[i&3]) >> shift2[i&3]];
			//				}
			//				else
			//				if (png_ptr->bit_depth == 1)
			//				{
			//					for (int i=0; i<w; i++)
			//						*dp++ = gray1[((*(sp+(i>>3))) & mask1[i&7]) >> shift1[i&7]];
			//				}
			//			}
			//			else // PNG_BLEND_OP_OVER
			//			{
			//				if (png_ptr->bit_depth == 16)
			//				{
			//					for (int i=0; i<w; i++, sp+=2, dp++)
			//						if (VDReadUnalignedBEU16(sp) != mTRNSg)
			//							*dp = 0xFF000000 + (*sp << 16) + (*sp << 8) + *sp;
			//				}
			//				else
			//				if (png_ptr->bit_depth == 8)
			//				{
			//					for (int i=0; i<w; i++, sp++, dp++)
			//						if (*sp != mTRNSg)
			//							*dp = 0xFF000000 + (*sp << 16) + (*sp << 8) + *sp;
			//						
			//				}
			//				else
			//				if (png_ptr->bit_depth == 4)
			//				{
			//					for (int i=0; i<w; i++, dp++)
			//					{
			//						g = ((*(sp+(i>>1))) & mask4[i&1]) >> shift4[i&1];
			//						if (g != mTRNSg)
			//							*dp = gray4[g];
			//					}
			//				}
			//				else
			//				if (png_ptr->bit_depth == 2)
			//				{
			//					for (int i=0; i<w; i++, dp++)
			//					{
			//						g = ((*(sp+(i>>2))) & mask2[i&3]) >> shift2[i&3];
			//						if (g != mTRNSg)
			//							*dp = gray2[g];
			//					}
			//				}
			//				else
			//				if (png_ptr->bit_depth == 1)
			//				{
			//					for (int i=0; i<w; i++, dp++)
			//					{
			//						g = ((*(sp+(i>>3))) & mask1[i&7]) >> shift1[i&7];
			//						if (g != mTRNSg)
			//							*dp = gray1[g];
			//					}
			//				}
			//			}
			//		}
			//	}
			//	else // GRAYSCALE + ALPHA
			//	{
			//		for (int j=0; j<h; j++)
			//		{
			//			sp = mUnpackBuffer + j*png_ptr->rowbytes;
			//			dp = &mFrameBuffer[(y+j)*mWidth+x];

			//			if (imageinfo.mBlend == PNG_BLEND_OP_SOURCE)
			//			{
			//				if (png_ptr->bit_depth == 8)
			//				{
			//					for (int i=0; i<w; i++, sp+=2)
			//						*dp++ = (*(sp+1) != 0) ? (*(sp+1) << 24) + (*sp << 16) + (*sp << 8) + *sp : 0;
			//				}
			//				else
			//				{
			//					for (int i=0; i<w; i++, sp+=4)
			//						*dp++ = (*(sp+2) != 0) ? (*(sp+2) << 24) + (*sp << 16) + (*sp << 8) + *sp : 0;
			//				}
			//			}
			//			else // PNG_BLEND_OP_OVER
			//			{
			//				for (int i=0; i<w; i++)
			//				{
			//					g = *sp; sp += step;
			//					a = *sp; sp += step;
			//					if (a == 255)
			//					{
			//						*dp++ = (a << 24) + (g << 16) + (g << 8) + g;
			//					}
			//					else
			//					if (a != 0)
			//					{
			//						if (a2 = (*dp)>>24)
			//						{
			//							u = a*255;
			//							v = (255-a)*a2;
			//							al = 255*255-(255-a)*(255-a2);
			//							g2 = ((*dp)&255);
			//							g = (g*u + g2*v)/al;
			//							a = al/255;
			//						}
			//						*dp++ = (a << 24) + (g << 16) + (g << 8) + g;
			//					}
			//					else
			//						dp++;
			//				}
			//			}
			//		}
			//	}

			///*
			//	const INT fw = mTargetFormat.w;
			//	const INT fh = mTargetFormat.h;


			//	if (!is_preroll) {
			//		VDPixmap srcbm = {0};
			//		srcbm.data		= mFrameBuffer.data();
			//		srcbm.pitch		= fw*sizeof(uint32);
			//		srcbm.w			= fw;
			//		srcbm.h			= fh;
			//		srcbm.format	= nsVDPixmap::kPixFormat_XRGB8888;

			//		VDPixmapBlt(mTargetFormat, srcbm);
			//	}
			//*/

			//	if ( imageinfo.mDispose == PNG_DISPOSE_OP_PREVIOUS )
			//		CopyMemory ( &mFrameBuffer[x + y*mWidth], mRestoreBuffer, w * h );

			//	pFrame->AllocateBuffer ( 4 * mWidth * mHeight );
			//	LPBYTE pBufferTmp = NULL; pFrame->get_Buffer ( &pBufferTmp );

			//	CopyMemory ( pBufferTmp, mFrameBuffer, 4 * mWidth * mHeight );

			//	if ( imageinfo.mDispose == PNG_DISPOSE_OP_BACKGROUND )
			//		ZeroMemory ( &mFrameBuffer[x + y*mWidth], 4 * w * h );

			////	mCachedFrame = frame_num;

			////	return getFrameBuffer();
			}
			void Read( BYTE* pRow )
			{
				//if (png_ptr == NULL) return;

				//png_ptr->zstream1.next_out = png_ptr->row_buf;
				//png_ptr->zstream1.avail_out = (uInt)png_ptr->rowbytes + 1;

				//do
				//{
				//	if (!(png_ptr->zstream1.avail_in))
				//	{
				//		while (png_ptr->idat_size == 0)
				//		{
				//			UINT length = VDReadUnalignedBEU32(&m_png_src[png_pos]);

				//			if (m_png_src[png_pos+4] == 'I' && m_png_src[png_pos+5] == 'D' && m_png_src[png_pos+6] == 'A' && m_png_src[png_pos+7] == 'T')
				//			{
				//				png_pos += 8;
				//				png_ptr->idat_size = length;
				//			}
				//			else
				//			if (m_png_src[png_pos+4] == 'f' && m_png_src[png_pos+5] == 'd' && m_png_src[png_pos+6] == 'A' && m_png_src[png_pos+7] == 'T')
				//			{
				//				png_pos += 12;
				//				png_ptr->idat_size = length-4;
				//			}
				//			else
				//			if (m_png_src[png_pos+4] == 'I' && m_png_src[png_pos+5] == 'E' && m_png_src[png_pos+6] == 'N' && m_png_src[png_pos+7] == 'D')
				//				break;
				//			else
				//			if (m_png_src[png_pos+4] == 0 && m_png_src[png_pos+5] == 0 && m_png_src[png_pos+6] == 0 && m_png_src[png_pos+7] == 0)
				//				break;
				//			else
				//				png_pos += (length+8);
				//		}

				//		if (png_ptr->idat_size == 0)
				//			break;

				//		png_ptr->zstream1.avail_in = (uInt)png_ptr->zbuf_size;
				//		png_ptr->zstream1.next_in = png_ptr->zbuf1;
				//		if (png_ptr->zbuf_size > png_ptr->idat_size)
				//			png_ptr->zstream1.avail_in = (uInt)png_ptr->idat_size;

				//		if (png_ptr->zbuf1 != NULL)
				//		{
				//			memcpy(png_ptr->zbuf1, m_png_src+png_pos, png_ptr->zstream1.avail_in);
				//			png_pos += png_ptr->zstream1.avail_in;
				//		}
				//		png_ptr->idat_size -= png_ptr->zstream1.avail_in;

				//		if (png_ptr->idat_size == 0)
				//			png_pos += 4;
				//	}

				//	int ret = inflate(&png_ptr->zstream1, Z_PARTIAL_FLUSH);

				//	if (ret == Z_STREAM_END)
				//		break;

				//} while (png_ptr->zstream1.avail_out);

				//if (png_ptr->row_buf[0])
				//	read_filter_row(png_ptr->row_buf + 1, png_ptr->prev_row + 1, (int)(png_ptr->row_buf[0]));

				//memcpy(png_ptr->prev_row, png_ptr->row_buf, png_ptr->rowbytes + 1);
				//memcpy(row, png_ptr->row_buf + 1, png_ptr->rowbytes);
			}
			void ReadFilterRow( BYTE* pRow, BYTE* pPrevRow, int nFilter)
			{
				//UINT i;
				//switch (nFilter)
				//{
				//	case PNG_FILTER_VALUE_NONE:
				//		break;
				//	case PNG_FILTER_VALUE_SUB:
				//	{
				//		BYTE * rp = row + png_ptr->bpp;
				//		BYTE * lp = row;

				//		for (i = png_ptr->bpp; i < png_ptr->rowbytes; i++)
				//		{
				//			*rp = (BYTE)(((int)(*rp) + (int)(*lp++)) & 0xff);
				//			rp++;
				//		}
				//		break;
				//	}
				//	case PNG_FILTER_VALUE_UP:
				//	{
				//		BYTE * rp = row;
				//		BYTE * pp = pPrevRow;

				//		for (i = 0; i < png_ptr->rowbytes; i++)
				//		{
				//			*rp = (BYTE)(((int)(*rp) + (int)(*pp++)) & 0xff);
				//			rp++;
				//		}
				//		break;
				//	}
				//	case PNG_FILTER_VALUE_AVG:
				//	{
				//		BYTE * rp = row;
				//		BYTE * pp = pPrevRow;
				//		BYTE * lp = row;

				//		for (i = 0; i < png_ptr->bpp; i++)
				//		{
				//			*rp = (BYTE)(((int)(*rp) + ((int)(*pp++) / 2 )) & 0xff);
				//			rp++;
				//		}

				//		for (i = 0; i < png_ptr->rowbytes - png_ptr->bpp; i++)
				//		{
				//			*rp = (BYTE)(((int)(*rp) + (int)(*pp++ + *lp++) / 2 ) & 0xff);
				//			rp++;
				//		}
				//		break;
				//	}
				//	case PNG_FILTER_VALUE_PAETH:
				//	{
				//		BYTE * rp = row;
				//		BYTE * pp = pPrevRow;
				//		BYTE * lp = row;
				//		BYTE * cp = pPrevRow;

				//		for (i = 0; i < png_ptr->bpp; i++)
				//		{
				//			*rp = (BYTE)(((int)(*rp) + (int)(*pp++)) & 0xff);
				//			rp++;
				//		}

				//		for (i = 0; i < png_ptr->rowbytes - png_ptr->bpp; i++)
				//		{
				//			int a, b, c, pa, pb, pc, p;

				//			a = *lp++;
				//			b = *pp++;
				//			c = *cp++;

				//			p = b - c;
				//			pc = a - c;

				//			pa = abs(p);
				//			pb = abs(pc);
				//			pc = abs(p + pc);

				//			p = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;

				//			*rp = (BYTE)(((int)(*rp) + p) & 0xff);
				//			rp++;
				//		}
				//		break;
				//	}
				//	default:
				//		*row = 0;
				//		break;
				//}
			}

			long GetCountFrames()
			{
				return mNumFrames;
			}

			
			void Clear()
			{
				//RELEASEARRAYOBJECTS( m_pBuffer );
				//RELEASEARRAYOBJECTS( mUnpackBuffer );

				//RELEASEARRAYOBJECTS( mRestoreBuffer );
				//RELEASEARRAYOBJECTS( mFrameBuffer );

				//mImages.RemoveAll();
			}

		
		private :

		//	vdblock<uint8>	mImage;
			UINT		mNumFrames;
			UINT		mNumLoops;
			UINT		mWidth;
			UINT		mHeight;
			UINT		mPixelDepth;
			UINT		hasTRNS;
			WORD		mTRNSr;
			WORD		mTRNSg;
			WORD		mTRNSb;
			BYTE		mBitDepth;
			BYTE		mColorType;

			BOOL		mbKeyframeOnly;
			DOUBLE		mFrameRate;

			struct ImageInfo
			{
				UINT	mOffsetAndKey;
				UINT	mWidth;
				UINT	mHeight;
				UINT	mXOffset;
				UINT	mYOffset;
				BYTE	mDispose;
				BYTE	mBlend;
			};

			CAtlArray <ImageInfo> mImages;

			UINT		mPalette[256];

			LPBYTE		m_pBuffer;
			LPBYTE		mUnpackBuffer;

			//png_struct_def	 *png_ptr;
			UINT		 png_pos;
			const BYTE	*m_png_src;

			LPUINT		mRestoreBuffer;
			LPUINT		mFrameBuffer;
		};
		class Jp2
		{
		public:
			virtual BOOL FromFile(const CString& sFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				BSTR bsFilePath = sFilePath.AllocSysString();

				Jpeg2000::IJ2kFile *pJpeg = NULL;
				::CoCreateInstance( __uuidof(Jpeg2000::CJ2kFile), NULL, CLSCTX_INPROC_SERVER, __uuidof(Jpeg2000::IJ2kFile), (void**)&pJpeg );
				if ( !pJpeg )
					return FALSE;

				IUnknown *pResImage = NULL;
				HRESULT hRes = pJpeg->raw_J2kToInterface( bsFilePath, &pResImage, L"" );
				::SysFreeString( bsFilePath );
				RELEASEINTERFACE( pJpeg );

				if ( S_OK != hRes )
					return FALSE;

				BOOL bRetValue = pResultImage->FromMediaData( pResImage, FALSE );
				RELEASEINTERFACE( pResImage );

				return TRUE;
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& sFilePath, int nFormat, BOOL bJp2,  BOOL bLossLess, int nQuality, int nFileSize )
			{
				IUnknown *pSrcImage = NULL;
				if ( !pSourceImage->ToMediaData( &pSrcImage, FALSE ) )
					return FALSE;

				BSTR bsFilePath = sFilePath.AllocSysString();

				Jpeg2000::IJ2kFile *pJpeg = NULL;
				::CoCreateInstance( __uuidof(Jpeg2000::CJ2kFile), NULL, CLSCTX_INPROC_SERVER, __uuidof(Jpeg2000::IJ2kFile), (void**)&pJpeg );
				if ( !pJpeg )
					return FALSE;

				// Составим Xml c настройками
				XmlUtils::CXmlWriter oXmlWriter;
				oXmlWriter.WriteNodeBegin( _T("Jpeg2000-Options") );
				oXmlWriter.WriteNodeBegin( _T("SaveOptions") );

				oXmlWriter.WriteNodeBegin( _T("Format"), TRUE );
				oXmlWriter.WriteAttribute( _T("value"), (int)bJp2 );
				oXmlWriter.WriteNodeEnd( _T("Format"), TRUE );

				oXmlWriter.WriteNodeBegin( _T("Tile"), TRUE );
				oXmlWriter.WriteAttribute( _T("width"), 512 );
				oXmlWriter.WriteAttribute( _T("height"), 512 );
				oXmlWriter.WriteNodeEnd( _T("Tile"), TRUE );

				if ( bLossLess )
				{
					oXmlWriter.WriteNodeBegin( _T("Compression"), TRUE );
					oXmlWriter.WriteAttribute( _T("lossless"), 1 );
					oXmlWriter.WriteNodeEnd( _T("Compression"), TRUE );

					oXmlWriter.WriteNodeBegin( _T("Rate-Disto") );
					oXmlWriter.WriteString( _T("1,1,1") ); // 3 слоя(лэйера) без сжатия информации
					oXmlWriter.WriteNodeEnd( _T("Rate-Disto") );
				}
				else
				{
					// Если установлен параметр nQuality [0..100], пользуемся им, если нет
					// используем значение nFileSize. Если и значение nFileSize отсутствует или
					// некорректно, тогда пишем с nQuality = 90.
					oXmlWriter.WriteNodeBegin( _T("Compression"), TRUE );
					oXmlWriter.WriteAttribute( _T("lossless"), 0 );
					oXmlWriter.WriteNodeEnd( _T("Compression"), TRUE );

					// Рзамер несжатой картинки
					long lUncFileSize = pSourceImage->GetWidth() * pSourceImage->GetHeight() * 3;

					if ( nQuality >= 0 && nQuality <= 100 )
					{
						oXmlWriter.WriteNodeBegin( _T("Fixed-Quality") );
						// Записываем 3 слоя(лэйра), с одинаковым nQuality
						oXmlWriter.WriteInteger( nQuality );
						oXmlWriter.WriteString( _T(",") );
						oXmlWriter.WriteInteger( nQuality );
						oXmlWriter.WriteString( _T(",") );
						oXmlWriter.WriteInteger( nQuality );
						oXmlWriter.WriteNodeEnd( _T("Fixed-Quality") );
					}
					else if ( nFileSize > 100 && nFileSize <= lUncFileSize )
					{
						double dKoef = (double)lUncFileSize / nFileSize;
						oXmlWriter.WriteNodeBegin( _T("Rate-Disto") );
						// Записываем 3 слоя(лэйра), с одинаковым коэффициентом сжатия
						oXmlWriter.WriteDouble( dKoef );
						oXmlWriter.WriteString( _T(",") );
						oXmlWriter.WriteDouble( dKoef );
						oXmlWriter.WriteString( _T(",") );
						oXmlWriter.WriteDouble( dKoef );
						oXmlWriter.WriteNodeEnd( _T("Rate-Disto") );
					}
					else
					{
						oXmlWriter.WriteNodeBegin( _T("Fixed-Quality") );
						// Записываем 3 слоя(лэйра), с одинаковым nQuality = 90
						oXmlWriter.WriteInteger( 90 );
						oXmlWriter.WriteString( _T(",") );
						oXmlWriter.WriteInteger( 90 );
						oXmlWriter.WriteString( _T(",") );
						oXmlWriter.WriteInteger( 90 );
						oXmlWriter.WriteNodeEnd( _T("Fixed-Quality") );
					}

				}

				oXmlWriter.WriteNodeEnd( _T("SaveOptions") );
				oXmlWriter.WriteNodeEnd( _T("Jpeg2000-Options") );
				BSTR bsXml = oXmlWriter.GetXmlString().AllocSysString();
				HRESULT hRes = pJpeg->raw_InterfaceToJ2k( &pSrcImage, bsFilePath, bsXml );
				::SysFreeString( bsXml );
				::SysFreeString( bsFilePath );
				RELEASEINTERFACE( pJpeg );

				if ( FAILED( hRes ) )
					return FALSE;

				return TRUE;
			}

		};
		class Meta
		{
		public:
			virtual BOOL FromFile(const CString& sFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				CMetaFile oMetaFile;
				if ( !oMetaFile.LoadFromFile( sFilePath ) )
					return FALSE;

				if ( !oMetaFile.ConvertToBitmap( pResultImage ) )
					return FALSE;

				return TRUE;
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& sFilePath, int nFormat, BOOL bJp2,  BOOL bLossLess, int nQuality, int nFileSize )
			{
				return FALSE;
			}

		};
		class Sfw
		{
		public:
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				// выходной формат всегда 32bpp
				nFormat = c_nFormat_Uncompressed_32bpp;

				if (NULL == pResultImage)
					return FALSE;

				// открываем файл
				HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE == hFile)
					return FALSE; // Невозможно открыть файл

				// мапим этот файл в память - так быстрее читаются данные из файла
				DWORD  nFileSize = GetFileSize(hFile, NULL);
				HANDLE hMapFile  = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
				if (NULL == hMapFile)
				{
					CloseHandle( hFile );
					return FALSE; // Невозможно создать отображение файла
				}

				// создаем view of file
				VOID* pBaseAddress	= MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
				BYTE* pnFile		= (BYTE*)pBaseAddress;

				// TO DO: Надо будет избавиться от этого копирования
				BYTE* pBuffer = new BYTE[nFileSize];
				if ( !pBuffer )
				{
					UnmapViewOfFile(pBaseAddress);
					CloseHandle(hMapFile);
					CloseHandle(hFile);
					return FALSE;
				}
				memcpy( pBuffer, pnFile, nFileSize );

				BOOL bRetValue = Sfw_to_Jpeg( pBuffer, nFileSize, pResultImage );

				delete []pBuffer;

				// закрываем файл
				UnmapViewOfFile(pBaseAddress);
				CloseHandle(hMapFile);
				CloseHandle(hFile);

				return bRetValue;
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat)
			{
				return FALSE;
			}

		protected:

			BOOL  Sfw_to_Jpeg(BYTE* pFile, DWORD dwSize, ImageStudio::Core::Image* pResultImage)
			{
				const int m_cnHuffSize = 420;

				static const BYTE m_carrHuffTable[] = {
					0xFF,0xC4,0x01,0xA2,0x00,0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,
					0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,
					0x0B,0x01,0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
					0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x10,0x00,
					0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D,0x01,
					0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,
					0x71,0x14,0x32,0x81,0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,
					0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,0x29,
					0x2A,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,
					0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,
					0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,
					0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,
					0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,
					0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,0xE3,
					0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,
					0xFA,0x11,0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,
					0x02,0x77,0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,
					0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1,0x09,0x23,0x33,
					0x52,0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,
					0x1A,0x26,0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,
					0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,
					0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,
					0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,
					0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,
					0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,
					0xD9,0xDA,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,
					0xF7,0xF8,0xF9,0xFA
				};

				// Инициализируем таблицу конвертации маркеров
				static const BYTE m_arrMarkerTable[256] = {
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xc0,0xfe,0xfe,0xfe,0xc4,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xd8,0xd9,0xda,0xdb,0xfe,0xfe,0xfe,0xfe,
					0xe0,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
					0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
				};
			
				//memset( m_arrMarkerTable, 0xfe, 256 ); // 0xfe - комментарий

				//m_arrMarkerTable[0xa0] = 0xc0; // SOF0
				//m_arrMarkerTable[0xa4] = 0xc4; // DHT
				//m_arrMarkerTable[0xc8] = 0xd8; // SOI
				//m_arrMarkerTable[0xc9] = 0xd9; // EOI
				//m_arrMarkerTable[0xca] = 0xda; // SOS
				//m_arrMarkerTable[0xcb] = 0xdb; // DQT
				//m_arrMarkerTable[0xd0] = 0xe0; // APP0

				if( dwSize < 4 )
					return FALSE;
				
				BYTE* pFileEnd = pFile + dwSize;

				// Ищем начало данных Jpeg
				BYTE *pHeaderStart = NULL;
				for( BYTE* ptr = pFile; ptr < pFileEnd - 3; ++ptr )
				{
					if(( ptr[0] == 0xff && 
						ptr[1] == 0xc8 && 
						ptr[2] == 0xff && 
						ptr[3] == 0xd0 )
						
						||
						
						(ptr[0] == 0xff && 
						ptr[1] == 0x07 && 
						ptr[2] == 0xff && 
						ptr[3] == 0x3f ))
					{
						pHeaderStart = ptr;
						break;
					}
				}

				if ( !pHeaderStart ) 
					return FALSE;

				// Фиксим маркеры SOI и APP0
				if( pHeaderStart[0] != 0xff || pHeaderStart[2] != 0xff )
					return FALSE;

				pHeaderStart[1] = m_arrMarkerTable[pHeaderStart[1]];
				pHeaderStart[3] = m_arrMarkerTable[pHeaderStart[3]];
				
				// JFIF\0\001\0
				//pHeaderStart[6]  = 0x4a;
				//pHeaderStart[7]  = 0x46;
				//pHeaderStart[8]  = 0x49;
				//pHeaderStart[9]  = 0x46;
				//pHeaderStart[10] = 0x00;
				//pHeaderStart[11] = 0x01;
				//pHeaderStart[12] = 0x00;

				BYTE* pBufferPtr = pHeaderStart + 2;
				pBufferPtr += Read_Skip_Length( pBufferPtr ); 

				// Фиксим остальные маркеры
				BOOL bHuffmanFlag = FALSE;
				BYTE* pHeaderEnd = NULL;
				
				while ( pBufferPtr < pFileEnd )
				{
					if( pBufferPtr[0] != 0xff )
						return FALSE;

					pBufferPtr[1] = m_arrMarkerTable[pBufferPtr[1]];

					if( pBufferPtr[1] == 0xc4 )
						bHuffmanFlag = TRUE;
					
					if( pBufferPtr[1] == 0xda )
					{
						pHeaderEnd = pBufferPtr - 1;
						break;
					}

					pBufferPtr += Read_Skip_Length( pBufferPtr );
				}

				// Ищем маркер конца файла EOI
				BYTE* pDataEnd = NULL;
				for( BYTE* ptr = pBufferPtr; ptr < pFileEnd - 1; ++ptr )
				{
					if( ptr[0] == 0xff && 
						ptr[1] == 0xc9 )
					{
						pDataEnd = ptr;
						break;
					}
				}

				if( !pDataEnd ) 
					return FALSE;

				// Фиксим маркеры EOI
				pDataEnd[1] = m_arrMarkerTable[pDataEnd[1]];

				// сохраняем во временный буфер
				size_t nSize1 = size_t(pHeaderEnd - pHeaderStart + 1);
				size_t nSize2 = bHuffmanFlag ? 0 : m_cnHuffSize;
				size_t nSize3 = size_t(pDataEnd - pHeaderEnd + 1);

				size_t lBufSize = nSize1 + nSize3 + nSize2;
				CBuffer<BYTE> oTmpBuffer( lBufSize );
				if( oTmpBuffer.IsEmpty() )
					return FALSE;

				
				::memcpy( oTmpBuffer.GetPtr(), pHeaderStart, nSize1 );
				
				if( nSize2 )
					::memcpy( oTmpBuffer.GetPtr() + nSize1, m_carrHuffTable, nSize2 );
				
				::memcpy( oTmpBuffer.GetPtr() + nSize1 + nSize2, pHeaderEnd + 1, nSize3 );

				//// тестовое сохранение
				//HANDLE hFile = CreateFile(_T("c:\\test_sfw.jpg"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				//if(hFile)
				//{
				//	BOOL res = WriteFile(hFile,oTmpBuffer.GetPtr(),lBufSize,NULL,0);
				//	CloseHandle(hFile);
				//}

				IMediaFrame* pFrame = Jpeg::LoadFromBuffer( oTmpBuffer.GetPtr(), lBufSize );

				BOOL bRetValue = pResultImage->FromMediaData( (IUnknown *)pFrame, FALSE );
				
				RELEASEINTERFACE( pFrame );

				return bRetValue;
			}

			static unsigned long Read_Skip_Length(BYTE* pMarker)
			{
				unsigned long lMSB = (unsigned long)pMarker[2];
				unsigned long lLSB = (unsigned long)pMarker[3];

				return (unsigned long)(256 * lMSB + lLSB + 2);
			}
		};

		class Swf
		{
		public:
			class SwfImageFile
			{
			private:


			public:
				SwfImageFile()
				{

				}

				SwfImageFile( const CString& sFilePath )
				{

				}

				
				~SwfImageFile()
				{
					Release();
				}

				void Release()
				{

				}

				
				BOOL Open( const CString& sFilePath )
				{
					return FALSE;
				}

				IMediaFrame* GetImage( DWORD nImage ) const
				{
					return NULL;
				}

				IMediaFrame* GetFirstValidImage() const
				{

					return NULL;
				}


				DWORD GetCountImages() const
				{
					return 0;
				}

			
			private:
				IMediaFrame* CreateImageFromBuffer( BYTE* pBuffer, long lBufSize ) const
				{
					return NULL;
				}

			};
		
		public:
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage, int nFrame = 0 )
			{
				return FALSE;
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& strFilePath, int nFormat)
			{
				return FALSE;
			}

		};

		
		namespace WebShots
		{
			class Wb1
			{
			private:
				struct THeaderWB1
				{
					union
					{
						struct
						{
							DWORD sign;
							DWORD version;
						};

						unsigned char str[8];
					};
				};

				static const DWORD WB1_SIGN = 0x42425757;

				static const DWORD WB1_VERS0 = 0x30303030;
				static const DWORD WB1_VERS1 = 0x31313131;

				static const BYTE WB1_VERS0_MAGIC = 0xA4;
				static const BYTE WB1_VERS1_MAGIC = 0xF2;

			public:
				IMediaFrame* LoadFromFile( const CString &sFilePath )
				{
					FILE* pFile = ::_tfopen( sFilePath, _T("rb") );
					if( !pFile )
						return NULL;

					MediaCore::IAVSUncompressedVideoFrame* pFrame = LoadFromFile( pFile );

					fclose( pFile );

					return pFrame;
				}

				IMediaFrame* LoadFromFile( FILE* pFile, size_t nFileSize = 0 )
				{
					long lBufSize = 0;
					BYTE* pBuffer = FromFile( &lBufSize, pFile );

					if( !pBuffer )
						return NULL;

					MediaCore::IAVSUncompressedVideoFrame* pFrame = Utils::LoadFromBuffer( pBuffer, lBufSize, 0 );//Utils::CreateBgraFrameFromGdiplusImage( pStream );
					if( pFrame )
						pFrame->put_ColorSpace( CSP_BGRA | CSP_VFLIP );

					delete [] pBuffer;

					return pFrame;
				}

				BOOL FromFile(const CString& sFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
				{
					MediaCore::IAVSUncompressedVideoFrame* pImage = LoadFromFile( sFilePath );
					
					BOOL bRetValue = pResultImage->FromMediaData( (IUnknown *)pImage, FALSE );
					
					RELEASEINTERFACE( pImage );

					return bRetValue;
				}
				BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& sFilePath, int nFormat, BOOL bJp2,  BOOL bLossLess, int nQuality, int nFileSize )
				{
					return FALSE;
				}

			private:
				static BYTE* FromFile( long* pOutBufSize, FILE* pFile, long nFileSize = 0 )
				{
					if( !pFile )
						return NULL;

					THeaderWB1 wb1_hdr;
					const long nSizeOfHeader = sizeof(wb1_hdr);

					if( !nFileSize )
					{
						long start = ::ftell( pFile );
						::fseek( pFile, 0, SEEK_END );
						long end = ::ftell( pFile );
						::fseek( pFile, start, SEEK_SET );

						nFileSize = end - start;
					}

					if( nFileSize < nSizeOfHeader )
						return NULL;

					if( nSizeOfHeader != ::fread( &wb1_hdr, 1, nSizeOfHeader, pFile ) )
						return NULL;

					BYTE buf[200];
					BOOL bIsJpg = FALSE;

					if( (wb1_hdr.str[0] == 0xff && wb1_hdr.str[1] == 0xd8) ||
						
						((wb1_hdr.str[0] == 'A' || wb1_hdr.str[0] == 'a') &&
						 (wb1_hdr.str[1] == 'D' || wb1_hdr.str[1] == 'd') &&
						 (wb1_hdr.str[2] == 'O' || wb1_hdr.str[2] == 'o') &&
						 (wb1_hdr.str[3] == 'B' || wb1_hdr.str[3] == 'b') &&
						 (wb1_hdr.str[4] == 'E' || wb1_hdr.str[4] == 'e')) )
					{
						bIsJpg = TRUE;
						::fseek( pFile, -(long)nSizeOfHeader, SEEK_CUR );
					}
					else
					{
						BYTE magic = 0;

						if( wb1_hdr.sign == WB1_SIGN )
						{
							if( wb1_hdr.version == WB1_VERS0 )
							{
								magic = WB1_VERS0_MAGIC;
							}
							else if( wb1_hdr.version == WB1_VERS1 )
							{
								magic = WB1_VERS1_MAGIC;
							}
						}
		                
						if( !magic )
							return NULL;

						if( 200 != fread( buf, 1, 200, pFile ) )
							return NULL;

						for( int i = 0; i < 100; i++ )
						{
							buf[i] = ~buf[i] ^ buf[i+100] ^ magic;
						}
					}


					if( nFileSize <= 0 )
						return NULL;
					
					long lBufSize = nFileSize - (bIsJpg ? 0 : nSizeOfHeader);
					if( pOutBufSize )
						*pOutBufSize = lBufSize;

					BYTE* pOutBuffer = new BYTE[lBufSize];
					if( !pOutBuffer )
						return NULL;

					BYTE* pReadBuf = pOutBuffer;

					if( !bIsJpg )
					{
						memcpy( pReadBuf, buf, 200 );

						pReadBuf += 200;
						nFileSize -= nSizeOfHeader + 200;
					}

					::fread( pReadBuf, 1, nFileSize, pFile );

					return pOutBuffer;
				}
			};

			class Wbz
			{
			private:

				static const DWORD c_WBZ_SIGN = 0x6791AB43;
				static const DWORD c_WBZ_REC_SIGN = 0x1082CDE1;

				struct THeader
				{
					DWORD dwSignature;
					DWORD dwSizeOfHeader;
					DWORD dwReserv1;
					DWORD dwReserv2;
					DWORD dwSizeOfDataBlocks;
					DWORD dwCountFiles;
				};

				struct TBlock
				{
					DWORD dwSignature;
					DWORD dwSizeOfHeader;
					DWORD dwReserv1;
					char  cFileName[256];
					DWORD dwDataSize1;
					DWORD Reserv2;
					DWORD dwDataSize2;
				};


			public:
				IMediaFrame* LoadFromFile( const CString &sFilePath )
				{
					FILE* pFile = ::_tfopen( sFilePath, _T("rb") );
					if( !pFile )
						return NULL;

					::fseek( pFile, 0, SEEK_END );
					size_t nFileSize = (size_t)::ftell( pFile );
					::fseek( pFile, 0, SEEK_SET );

					const size_t nSizeOfHeader = sizeof(THeader);
					
					THeader header;

					if( nSizeOfHeader != ::fread( &header, 1, nSizeOfHeader, pFile ) )
					{
						fclose( pFile );
						return NULL;
					}

					if( header.dwSignature != c_WBZ_SIGN ||
						nFileSize < header.dwSizeOfHeader + header.dwSizeOfDataBlocks )
					{
						fclose( pFile );
						return NULL;
					}

                    fseek( pFile, header.dwSizeOfHeader, SEEK_SET );

					TBlock block;

					if( sizeof(TBlock) != ::fread( &block, 1, sizeof(TBlock), pFile ) )
					{
						fclose( pFile );
						return NULL;
					}

					if( block.dwSignature != c_WBZ_REC_SIGN )
					{
						::fclose( pFile );
						return NULL;
					}

					::fseek( pFile, long(block.dwSizeOfHeader) - sizeof(TBlock), SEEK_CUR );

					Wb1 wb1;

					IMediaFrame* pResultFrame = wb1.LoadFromFile( pFile, block.dwDataSize1 );

					fclose( pFile );

					return pResultFrame;
				}

				BOOL FromFile(const CString& sFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
				{
					MediaCore::IAVSUncompressedVideoFrame* pImage = LoadFromFile( sFilePath );
					
					BOOL bRetValue = pResultImage->FromMediaData( (IUnknown *)pImage, FALSE );
					
					RELEASEINTERFACE( pImage );

					return bRetValue;
				}
				BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& sFilePath, int nFormat, BOOL bJp2,  BOOL bLossLess, int nQuality, int nFileSize )
				{
					return FALSE;
				}



			};
			class Wbc
			{
			private:
				static const DWORD c_WBC_SIGN = 0x95FA16AB;
				static const DWORD c_WBC_LIST_OFFSET = 0x894;
				static const DWORD c_WBC_REC_SIGN = 0xF071CDE2;

				struct THeader
				{
					DWORD dwSignature;
					DWORD dwSizeOfHeader;
					DWORD dwReserv1;
					char  sCollectionName[128];
					DWORD dwReserv2;
				};

				struct TBlockDesc
				{
					DWORD dwOffset;
					DWORD dwSizeOfData;
					DWORD dwReserv1;
					DWORD dwReserv2;
					DWORD dwReserv3;
					char  sReserv[20];
				};

				struct TBlockHeader
				{
					DWORD dwSignature;
					DWORD dwSizeOfHeader;
					DWORD dwSizeOfData;
					char sFileName[256];
					char sFotoName[384];
					char sCreaterName[256];
					char sFileType[8];
					DWORD dwSizeOfFile;
					DWORD dwSizeOfExtraData;
					DWORD dwReserv1;
					char sReserv[140];
					DWORD dwReserv2;
					DWORD dwReserv3;
					char sFotoId[128];
					char sGropename[96];
					char sCollectionName[128];
				};

			public:
				IMediaFrame* LoadFromFile( const CString &sFilePath )
				{
					FILE* pFile = ::_tfopen( sFilePath, _T("rb") );
					if( !pFile )
						return NULL;

					::fseek( pFile, 0, SEEK_END );
					size_t nFileSize = (size_t)::ftell( pFile );
					::fseek( pFile, 0, SEEK_SET );

					const size_t nSizeOfHeader = sizeof(THeader);
					
					THeader header;

					if( nSizeOfHeader != ::fread( &header, 1, nSizeOfHeader, pFile ) )
					{
						fclose( pFile );
						return NULL;
					}

					if( header.dwSignature != c_WBC_SIGN ||
						nFileSize < header.dwSizeOfHeader )
					{
						fclose( pFile );
						return NULL;
					}

					if( 0 == fseek( pFile, c_WBC_LIST_OFFSET, SEEK_SET ) )
					{
						fclose( pFile );
						return NULL;
					}

					DWORD dwFilesCount = 0;
					if( sizeof(DWORD) != fread( &dwFilesCount, sizeof(DWORD), 1, pFile ) )
					{
						fclose( pFile );
						return NULL;
					}

                    if( (c_WBC_LIST_OFFSET + sizeof(dwFilesCount) + dwFilesCount * sizeof(TBlockDesc)) < header.dwSizeOfHeader )
					{
						fclose( pFile );
						return NULL;
					}

					IMediaFrame* pResultFrame = NULL;

					TBlockDesc* pBlocks = new TBlockDesc[dwFilesCount];
					if( pBlocks )
					{
						if( sizeof(TBlockDesc) * dwFilesCount == fread( pBlocks, sizeof(TBlockDesc), dwFilesCount, pFile ) )
						{
							for( DWORD i = 0; i < dwFilesCount; ++i )
							{
								TBlockHeader block;

	                            if( 0 == fseek( pFile, pBlocks[i].dwOffset, SEEK_SET ) )
								{
									if( sizeof(TBlockHeader) == fread( &block, sizeof(TBlockHeader), 1, pFile ) )
									{
										if( 0 == fseek( pFile, pBlocks[i].dwOffset + header.dwSizeOfHeader, SEEK_SET ) )
										{
											Wb1 wb1;

											pResultFrame = wb1.LoadFromFile( pFile, pBlocks[i].dwSizeOfData );
											if( pResultFrame )
												break;
										}
									}
								}
							}
						}

						delete [] pBlocks;
					}

					fclose( pFile );

					return pResultFrame;
				}

				BOOL FromFile(const CString& sFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
				{
					MediaCore::IAVSUncompressedVideoFrame* pImage = LoadFromFile( sFilePath );
					
					BOOL bRetValue = pResultImage->FromMediaData( (IUnknown *)pImage, FALSE );
					
					RELEASEINTERFACE( pImage );

					return bRetValue;
				}
				BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& sFilePath, int nFormat, BOOL bJp2,  BOOL bLossLess, int nQuality, int nFileSize )
				{
					return FALSE;
				}



			};


		}

		class Ithmb
		{
		private:
			enum EColorSpace
			{
				csARGB,
				csRGB_LE,
				csRGB_BE,
				csRGB_BE_90,
				csRGB555_LE,
				csRGB555_BE,
				csJFIF,
				csUYVY,
				csUYVY_d, // deinterlecing
				csYUV420
			};

			struct TFileHeader
			{
				int nWidth;
				int nHeight;
				int nColorSpace;
				int nImageOffset;
				int nImageSize;

				TFileHeader()
				{
					nWidth = 0;
					nHeight = 0;
					nColorSpace = 0;
					nImageOffset = 0;
					nImageSize = 0;
				}
			};


		private:
			static long GetFileSize( FILE* pFile )
			{
				::fseek( pFile, 0, SEEK_END );
				long lFileSize = ::ftell( pFile );
				::fseek( pFile, 0, SEEK_SET );

				return lFileSize;
			}
			static CString GetFileName( const CString &sFilePath )
			{
				int nPos1 = sFilePath.ReverseFind( _T('\\') );
				int nPos2 = sFilePath.ReverseFind( _T('/') );
				int nPos = max( nPos1, nPos2 );

				if( nPos < 0 )
					return sFilePath;

				return sFilePath.Right( sFilePath.GetLength() - nPos - 1 );
			}

			static BOOL GetFileHeader( const CString& sFilePath, long lFileSize, TFileHeader& result )
			{
				CString sFileName = GetFileName( sFilePath );
				if( sFileName.GetLength() < 5 )
					return FALSE;

				CString sImageType = sFileName.Left( 5 );

				BOOL bLoading = FALSE;

				if( sImageType == _T("F1009") )
				{
					if( 0 == (lFileSize % 2520) ) /* iPod photo */
					{
						result.nWidth  = 42;
						result.nHeight = 30;
						result.nColorSpace = csRGB_LE;
						result.nImageOffset = 0;
						result.nImageSize = 2520;
						
						bLoading = TRUE;
					}
				}
				else
				if( sImageType == _T("F1015") )
				{
					if( 0 == (lFileSize % 22880) ) /* iPod photo */
					{
						result.nWidth  = 130;
						result.nHeight = 88;
						result.nColorSpace = csRGB_LE;
						result.nImageOffset = 0;
						result.nImageSize = 22880;
						
						bLoading = TRUE;
					}
				}
				else
				if( sImageType == _T("F1015") )
				{
					if( 0 == (lFileSize % 22880) ) /* iPod photo */
					{
						result.nWidth  = 130;
						result.nHeight = 88;
						result.nColorSpace = csRGB_LE;
						result.nImageOffset = 0;
						result.nImageSize = 22880;
						
						bLoading = TRUE;
					}
				}
				else
				if( sImageType == _T("F1020") )
				{
					if( 0 == (lFileSize % 77440) ) /* iPod photo */
					{
						result.nWidth  = 220;
						result.nHeight = 176;
						result.nColorSpace = csRGB_BE_90;
						result.nImageOffset = 0;
						result.nImageSize = 77440;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1023") )
				{
					if( 0 == (lFileSize % 46464) ) /* iPod nano 1G/2G */
					{
						result.nWidth  = 176;
						result.nHeight = 132;
						result.nColorSpace = csRGB_BE;
						result.nImageOffset = 0;
						result.nImageSize = 46464;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1024") )
				{
					if( 0 == (lFileSize % 153600) ) /* iPod nano 3G and 4G and 5.5G*/
					{
						result.nWidth  = 320;
						result.nHeight = 240;
						result.nColorSpace = csRGB_LE;
						result.nImageOffset = 0;
						result.nImageSize = 153600;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1064") )
				{
					if( 0 == (lFileSize % 307200) ) /* iPod nano 3G */
					{
						result.nWidth  = 320;
						result.nHeight = 240;
						result.nColorSpace = csARGB;
						result.nImageOffset = 0;
						result.nImageSize = 307200;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1066") )
				{
					if( 0 == (lFileSize % 8192) ) /* iPod nano 3G */
					{
						result.nWidth  = 64;
						result.nHeight = 64;
						result.nColorSpace = csRGB_LE;
						result.nImageOffset = 0;
						result.nImageSize = 8192;
						
						bLoading = TRUE;
					}
					else
					if( 0 == (lFileSize % 307200) ) /* iPod nano 4G */
					{
						result.nWidth  = 320;
						result.nHeight = 240;
						result.nColorSpace = csARGB;
						result.nImageOffset = 0;
						result.nImageSize = 307200;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1067") )
				{
					if( 0 == (lFileSize % 691200) ) /* iPod nano 3G */
					{
						result.nWidth  = 720;
						result.nHeight = 480;
						result.nColorSpace = csYUV420;
						result.nImageOffset = 0;
						result.nImageSize = 691200;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1019") )
				{
					if( 0 == (lFileSize % 691200) ) /* iPod 5.5G */
					{
						result.nWidth  = 720;
						result.nHeight = 480;
						result.nColorSpace = csUYVY_d;
						result.nImageOffset = 0;
						result.nImageSize = 691200;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1079") )
				{
					if( 0 == (lFileSize % 12800) ) /* iPod nano 4G */
					{
						result.nWidth  = 80;
						result.nHeight = 80;
						result.nColorSpace = csRGB_LE;
						result.nImageOffset = 0;
						result.nImageSize = 12800;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1083") )
				{
					if( 0 == (lFileSize % 153600) ) /* iPod nano 4G */
					{
						result.nWidth  = 240;
						result.nHeight = 320;
						result.nColorSpace = csRGB_LE;
						result.nImageOffset = 0;
						result.nImageSize = 153600;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1036") )
				{
					if( 0 == (lFileSize % 4100) ) /* iPod 5G/5.5G */
					{
						result.nWidth  = 50;
						result.nHeight = 41;
						result.nColorSpace = csRGB_LE;
						result.nImageOffset = 0;
						result.nImageSize = 4100;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F3008") )
				{
					if( 0 == (lFileSize % 614400) ) /* iPod nano 4G */
					{
						result.nWidth  = 640;
						result.nHeight = 480;
						result.nColorSpace = csRGB555_LE;
						result.nImageOffset = 0;
						result.nImageSize = 614400;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1081") )
				{
					/*if( 0 == (lFileSize % 614400) )*/ /* iPod nano 4G */
					{
						result.nWidth  = 640;
						result.nHeight = 480;
						result.nColorSpace = csJFIF;
						result.nImageOffset = 0;
						result.nImageSize = lFileSize;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F3013") )
				{
					/*if( 0 == (lFileSize % 614400) )*/ /* iPad (artwork) */
					{
						result.nWidth  = 768;
						result.nHeight = 768;
						result.nColorSpace = csJFIF;
						result.nImageOffset = 0;
						result.nImageSize = lFileSize;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F3019") )
				{
					/*if( 0 == (lFileSize % 614400) )*/ /* iPad (artwork) */
					{
						result.nWidth  = 512;
						result.nHeight = 768;
						result.nColorSpace = csJFIF;
						result.nImageOffset = 0;
						result.nImageSize = lFileSize;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F3004") )
				{
					if( 0 == (lFileSize % 8192) ) /* iPod touch 1G */
					{
						result.nWidth  = 55;
						result.nHeight = 55;
						result.nColorSpace = csRGB555_LE;
						result.nImageOffset = 2;
						result.nImageSize = 6160;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F3009") )
				{
					if( 0 == (lFileSize % 40960) ) /* iPod touch 1G */
					{
						result.nWidth  = 160;
						result.nHeight = 120;
						result.nColorSpace = csRGB555_LE;
						result.nImageOffset = 0;
						result.nImageSize = 38400;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F3011") )
				{
					if( 0 == (lFileSize % 12640) ) /* iPod touch 1G */
					{
						result.nWidth  = 75;
						result.nHeight = 75;
						result.nColorSpace = csRGB555_LE;
						result.nImageOffset = 10;
						result.nImageSize = 12640;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F3034") )
				{
					if( 0 == (lFileSize % 8192) ) /* iPod touch 1G */
					{
						result.nWidth  = 55;
						result.nHeight = 55;
						result.nColorSpace = csRGB555_LE;
						result.nImageOffset = 2;
						result.nImageSize = 6160;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F3039") )
				{
					if( 0 == (lFileSize % 40960) ) /* iPod touch 1G */
					{
						result.nWidth  = 160;
						result.nHeight = 120;
						result.nColorSpace = csRGB555_LE;
						result.nImageOffset = 0;
						result.nImageSize = 38400;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F3041") )
				{
					if( 0 == (lFileSize % 12640) ) /* iPod touch 1G */
					{
						result.nWidth  = 75;
						result.nHeight = 75;
						result.nColorSpace = csRGB555_LE;
						result.nImageOffset = 10;
						result.nImageSize = 12640;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F4031") )
				{
					if( 0 == (lFileSize % 28800) ) /* iPod touch 1G */
					{
						result.nWidth  = 120;
						result.nHeight = 120;
						result.nColorSpace = csRGB555_LE;
						result.nImageOffset = 0;
						result.nImageSize = 28800;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F4032") )
				{
					if( 0 == (lFileSize % 2048) ) /* iPod touch 1G */
					{
						result.nWidth  = 32;
						result.nHeight = 32;
						result.nColorSpace = csRGB555_LE;
						result.nImageOffset = 0;
						result.nImageSize = 2048;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F4040") )
				{
					if( 0 == (lFileSize % 55776) ) /* iPod touch 1G */
					{
						result.nWidth  = 166;
						result.nHeight = 166;
						result.nColorSpace = csRGB555_LE;
						result.nImageOffset = 4;
						result.nImageSize = 55776;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1032") )
				{
					if( 0 == (lFileSize % 3108) ) /* iPod touch 1G */
					{
						result.nWidth  = 42;
						result.nHeight = 37;
						result.nColorSpace = csRGB_LE;
						result.nImageOffset = 0;
						result.nImageSize = 3108;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType == _T("F1028") )
				{
					if( 0 == (lFileSize % 20000) ) /* ??? */
					{
						result.nWidth  = 100;
						result.nHeight = 100;
						result.nColorSpace = csRGB_LE;
						result.nImageOffset = 0;
						result.nImageSize = 20000;
						
						bLoading = TRUE;
					}
				}
				else if( sImageType.Left( 1 ) == _T("T") )
				{
					//if( 0 == (lFileSize % 871780) ) /* ??? */
					if( lFileSize >= 691200 )
					{
						result.nWidth  = 720;
						result.nHeight = 480;
						result.nColorSpace = csUYVY_d;
						result.nImageOffset = 0;
						result.nImageSize = 691200;
						
						bLoading = TRUE;
					}
				}

				return bLoading;
			}

			static BOOL LoadImage_16bppRGB( BYTE* pFileBuffer, int nWidth, int nHeight, BYTE* pPixels, BOOL bLowEnding, BOOL bRGB555 = FALSE, int nOffset = 0, BOOL bRotate = FALSE )
			{
				static const BYTE table[32 + 64] = {
					  0,   8,  16,  25,  33,  41,  49,  58, 
					 66,  74,  82,  90,  99, 107, 115, 123, 
					132, 140, 148, 156, 165, 173, 181, 189, 
					197, 206, 214, 222, 230, 239, 247, 255,
					
					  0,   4,   8,  12,  16,  20,  24,  28, 
					 32,  36,  40,  45,  49,  53,  57,  61, 
					 65,  69,  73,  77,  81,  85,  89,  93, 
					 97, 101, 105, 109, 113, 117, 121, 125, 
					130, 134, 138, 142, 146, 150, 154, 158, 
					162, 166, 170, 174, 178, 182, 186, 190, 
					194, 198, 202, 206, 210, 215, 219, 223, 
					227, 231, 235, 239, 243, 247, 251, 255				
				};
				
				BYTE* src = pFileBuffer;
				BYTE* dst = pPixels;
				
				for( int y = 0; y < nHeight; ++y, src += nOffset )
				{
					for( int x = 0; x < nWidth; ++x, src += 2, dst += 4 )
					{
						DWORD clr = bLowEnding ? (src[0] + src[1] * 256) : (src[0] * 256 + src[1]);

						dst[0] = table[clr & 0x1f];

						if( bRGB555 )
						{
							dst[1] = table[((clr >> 5) & 0x1F)];
							dst[2] = table[(clr >> 10) & 0x1F];
						}
						else
						{
							dst[1] = table[((clr >> 5) & 0x3f) + 32];
							dst[2] = table[clr >> 11];
						}

						dst[3] = 255;
					}
				}

				if( bRotate )
				{
					for( int y = 0; y < nHeight; y++ )
					{
						DWORD* src1 = (DWORD*)(pPixels) + y * nWidth;
						DWORD* src2 = src1 + nWidth - 1;

						for( int x = 0; x < nWidth; x += 2, src1 += 1, src2 -= 1 )
						{
							DWORD tmp = src1[0];
							src1[0] = src2[0];
							src2[0] = tmp;
						}
					}
				}
				
				return TRUE;
			}

			static BOOL LoadImage_16bppUYVY_d( BYTE* pFileBuffer, int nWidth, int nHeight, BYTE* pPixels )
			{
				BYTE* src = pFileBuffer;
				BYTE* dst = pPixels;

				int offset = (nWidth * 2) * (nHeight / 2);
				
				for( int y = 0; y < nHeight; y += 2, dst += nWidth * 4 )
				{
					for( int x = 0; x < nWidth; x += 2, src += 4, dst += 8 )
					{
						int Y = (int(src[1]) - 16) * 298;
						int U = (int(src[0]) - 128);
						int V = (int(src[2]) - 128);

						int R = 128 + 409 * V;
						int G = 128 - 100 * U - 208 * V;
						int B = 128 + 516 * U;

						dst[0] = IntToByte( (Y + B) >> 8 );
						dst[1] = IntToByte( (Y + G) >> 8 );
						dst[2] = IntToByte( (Y + R) >> 8 );
						dst[3] = 255;
						
						Y = (int(src[3]) - 16) * 298;

						dst[4] = IntToByte( (Y + B) >> 8 );
						dst[5] = IntToByte( (Y + G) >> 8 );
						dst[6] = IntToByte( (Y + R) >> 8 );
						dst[7] = 255;

						// вторая строка						
						Y = (int(src[1 + offset]) - 16) * 298;
						U = (int(src[0 + offset]) - 128);
						V = (int(src[2 + offset]) - 128);

						R = 128 + 409 * V;
						G = 128 - 100 * U - 208 * V;
						B = 128 + 516 * U;

						dst[0 + nWidth * 4] = IntToByte( (Y + B) >> 8 );
						dst[1 + nWidth * 4] = IntToByte( (Y + G) >> 8 );
						dst[2 + nWidth * 4] = IntToByte( (Y + R) >> 8 );
						dst[3 + nWidth * 4] = 255;

						Y = (int(src[3 + offset]) - 16) * 298;

						dst[4 + nWidth * 4] = IntToByte( (Y + B) >> 8 );
						dst[5 + nWidth * 4] = IntToByte( (Y + G) >> 8 );
						dst[6 + nWidth * 4] = IntToByte( (Y + R) >> 8 );
						dst[7 + nWidth * 4] = 255;
					}
				}
				
				return TRUE;
			}

			static BOOL LoadImage_16bppUYVY( BYTE* pFileBuffer, int nWidth, int nHeight, BYTE* pPixels )
			{
				BYTE* src = pFileBuffer;
				BYTE* dst = pPixels;

				for( int y = 0; y < nHeight; y += 1 )
				{
					for( int x = 0; x < nWidth; x += 2, src += 4, dst += 8 )
					{
						//dst[0] = src[1];
						//dst[1] = src[1];
						//dst[2] = src[1];
						//dst[3] = 255;

						int Y = (int(src[1]) - 16) * 298;
						int U = (int(src[0]) - 128);
						int V = (int(src[2]) - 128);

						int R = 128 + 409 * V;
						int G = 128 - 100 * U - 208 * V;
						int B = 128 + 516 * U;

						dst[0] = IntToByte( (Y + B) >> 8 );
						dst[1] = IntToByte( (Y + G) >> 8 );
						dst[2] = IntToByte( (Y + R) >> 8 );
						dst[3] = 255;
						
						Y = (int(src[3]) - 16) * 298;

						dst[4] = IntToByte( (Y + B) >> 8 );
						dst[5] = IntToByte( (Y + G) >> 8 );
						dst[6] = IntToByte( (Y + R) >> 8 );
						dst[7] = 255;
					}
				}
				
				return TRUE;
			}

			static BOOL LoadImage_16bppYUV420( BYTE* pFileBuffer, int nWidth, int nHeight, BYTE* pPixels )
			{
				BYTE* src = pFileBuffer;
				BYTE* dst = pPixels;

				int nSize = nWidth * nHeight;

				BYTE* pY = src;
				BYTE* pU = src + nSize;
				BYTE* pV = src + nSize + nSize / 4;

				for( int y = 0; y < nHeight; y += 2, dst += nWidth * 4, pY += nWidth )
				{
					for( int x = 0; x < nWidth; x += 2, dst += 8, pY += 2, pU += 1, pV += 1 )
					{
						int Y = (int(pY[0]) - 16) * 298;
						int U = (int(pU[0]) - 128);
						int V = (int(pV[0]) - 128);

						int R = 128 + 409 * V;
						int G = 128 - 100 * U - 208 * V;
						int B = 128 + 516 * U;

						dst[0] = IntToByte( (Y + B) >> 8 );
						dst[1] = IntToByte( (Y + G) >> 8 );
						dst[2] = IntToByte( (Y + R) >> 8 );
						dst[3] = 255;

						Y = (int(pY[1]) - 16) * 298;

						dst[4] = IntToByte( (Y + B) >> 8 );
						dst[5] = IntToByte( (Y + G) >> 8 );
						dst[6] = IntToByte( (Y + R) >> 8 );
						dst[7] = 255;

						Y = (int(pY[0 + nWidth]) - 16) * 298;

						dst[0 + nWidth * 4] = IntToByte( (Y + B) >> 8 );
						dst[1 + nWidth * 4] = IntToByte( (Y + G) >> 8 );
						dst[2 + nWidth * 4] = IntToByte( (Y + R) >> 8 );
						dst[3 + nWidth * 4] = 255;

						Y = (int(pY[1 + nWidth]) - 16) * 298;

						dst[4 + nWidth * 4] = IntToByte( (Y + B) >> 8 );
						dst[5 + nWidth * 4] = IntToByte( (Y + G) >> 8 );
						dst[6 + nWidth * 4] = IntToByte( (Y + R) >> 8 );
						dst[7 + nWidth * 4] = 255;
					}
				}
				
				return TRUE;
			}

			static BOOL LoadImage_32bppRGB( BYTE* pFileBuffer, int nWidth, int nHeight, BYTE* pPixels )
			{
				BYTE* src = pFileBuffer;
				BYTE* dst = pPixels;

				::memcpy( dst, src, nWidth * nHeight * 4 );

				//for( int y = 0; y < nHeight; ++y )
				//{
				//	for( int x = 0; x < nWidth; ++x, src += 4, dst += 4 )
				//	{
				//		dst[0] = src[0];
				//		dst[1] = src[1];
				//		dst[2] = src[2];
				//		dst[3] = src[3];
				//	}
				//}
				
				return TRUE;
			}

			static BOOL LoadImage_24bppJFIF( BYTE* pFileBuffer, int nFileSize, int nWidth, int nHeight, BYTE* pPixels )
			{
				//IStream* pStream = NULL;
				//
				//HRESULT hr = ::CreateStreamOnHGlobal( NULL, FALSE, &pStream );
				//if( FAILED(hr) || !pStream )
				//{
				//	return FALSE;
				//}

				//ULONG ulCount = 0;

				//pStream->Write( pFileBuffer, nFileSize, &ulCount );

				//LARGE_INTEGER lStartPos;
				//lStartPos.QuadPart = 0;

				//pStream->Seek( lStartPos, 0, 0 );

				//Gdiplus::Bitmap image( pStream );

				//pStream->Release();
				//
				//if( image.GetLastStatus() != Gdiplus::Ok )
				//	return false;

				//Gdiplus::BitmapData oBitmapData;
				//oBitmapData.Width       = nWidth;
				//oBitmapData.Height      = nHeight;
				//oBitmapData.Stride      = nWidth * 4;
				//oBitmapData.PixelFormat =  PixelFormat32bppARGB;
				//oBitmapData.Scan0       = pPixels;
				//oBitmapData.Reserved    = NULL;

				//image.LockBits( NULL, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeUserInputBuf, PixelFormat32bppARGB, &oBitmapData );
				//image.UnlockBits( &oBitmapData );
				
				return TRUE;
			}

		public:
			MediaCore::IAVSUncompressedVideoFrame* LoadFromFile( const CString &sFilePath )
			{
				FILE* pFile = ::_tfopen( sFilePath, _T("rb") );
				if( !pFile )
					return NULL;

				long lFileSize = GetFileSize( pFile );

				TFileHeader header;
				if( !GetFileHeader( sFilePath, lFileSize, header ) )
				{
					fclose( pFile );
					return NULL;
				}

				BYTE* pFileBuffer = new BYTE[header.nImageSize];
				if( !pFileBuffer )
				{
					fclose( pFile );
					return NULL;
				}

				::fseek( pFile, 0/*header.nImageOffset*/, SEEK_SET );
				::fread( pFileBuffer, 1, header.nImageSize, pFile );
				::fclose( pFile );

				BYTE* pImageBuffer = NULL;
				
				MediaCore::IAVSUncompressedVideoFrame* pFrame = CreateBgraFrame( header.nWidth, header.nHeight, &pImageBuffer );
				if( !pFrame )
				{
					delete [] pFileBuffer;
					return NULL;
				}

				BOOL bLoading = FALSE;

				switch( header.nColorSpace )
				{
				case csRGB_LE:
					bLoading = LoadImage_16bppRGB( pFileBuffer, header.nWidth, header.nHeight, pImageBuffer, TRUE, FALSE, header.nImageOffset );
					break;

				case csRGB_BE:
					bLoading = LoadImage_16bppRGB( pFileBuffer, header.nWidth, header.nHeight, pImageBuffer, FALSE, FALSE, header.nImageOffset );
					break;

				case csRGB_BE_90:
					bLoading = LoadImage_16bppRGB( pFileBuffer, header.nWidth, header.nHeight, pImageBuffer, FALSE, FALSE, header.nImageOffset, TRUE );
					break;

				case csRGB555_LE:
					bLoading = LoadImage_16bppRGB( pFileBuffer, header.nWidth, header.nHeight, pImageBuffer, TRUE, TRUE, header.nImageOffset );
					break;

				case csRGB555_BE:
					bLoading = LoadImage_16bppRGB( pFileBuffer, header.nWidth, header.nHeight, pImageBuffer, FALSE, TRUE, header.nImageOffset );
					break;

				case csUYVY:
					bLoading = LoadImage_16bppUYVY( pFileBuffer, header.nWidth, header.nHeight, pImageBuffer );
					break;

				case csUYVY_d:
					bLoading = LoadImage_16bppUYVY_d( pFileBuffer, header.nWidth, header.nHeight, pImageBuffer );
					break;

				case csYUV420:
					bLoading = LoadImage_16bppYUV420( pFileBuffer, header.nWidth, header.nHeight, pImageBuffer );
					break;

				case csARGB:
					bLoading = LoadImage_32bppRGB( pFileBuffer, header.nWidth, header.nHeight, pImageBuffer );
					break;

				case csJFIF:
					{
						IMediaFrame* pTmpFrame = Jpeg::LoadFromBuffer( pFileBuffer, lFileSize );
						if( pTmpFrame )
						{
							pFrame->Release();
							pFrame = pTmpFrame;
							bLoading = TRUE;
						}
						//bLoading = LoadImage_24bppJFIF( pFileBuffer, lFileSize, header.nWidth, header.nHeight, pImageBuffer );
					}
					break;
				};

				delete [] pFileBuffer;

				if( !bLoading )
				{
					pFrame->Release();
					return NULL;
				}

				return pFrame;
			}
			
			virtual BOOL FromFile(const CString& sFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				MediaCore::IAVSUncompressedVideoFrame* pImage = LoadFromFile( sFilePath );
				
				BOOL bRetValue = pResultImage->FromMediaData( (IUnknown *)pImage, FALSE );
				
				RELEASEINTERFACE( pImage );

				return bRetValue;
			}
			virtual BOOL ToFile(ImageStudio::Core::Image* pSourceImage, const CString& sFilePath, int nFormat, BOOL bJp2,  BOOL bLossLess, int nQuality, int nFileSize )
			{
				return FALSE;
			}


		};


		class Svm
		{
		public:
			virtual BOOL FromFile(const CString& strFilePath, int& nFormat, ImageStudio::Core::Image* pResultImage)
			{
				
				BOOL bRetValue = TRUE;

				BSTR bsFilePath = strFilePath.AllocSysString();
				
				OfficeSvmFile *svmFile = new OfficeSvmFile();
				if (!svmFile)return FALSE; 


				int err = svmFile->Open(bsFilePath);				
				::SysFreeString(bsFilePath);

				if (err) return FALSE;

				IUnknown *pResImage =svmFile->GetThumbnail();
				bRetValue = pResultImage->FromMediaData( pResImage, TRUE );
				delete svmFile;
				return bRetValue;
			}
		};		
	}
}