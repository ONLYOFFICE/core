#pragma once

#include <ImageTransformsCoreConstants.h>

#ifndef   IMAGE_EDITOR
#include <ImageTransformsCoreGDIPLUS.h>
#include <ImagePaint.h>
#include <ChromoKey.h>
#endif


#include <TransformConstants.h>
#include <Utilities.h>

#ifndef   IMAGE_EDITOR
#include <ImagePaintEffects.h>
#endif

#include <ImagePaintComposeIPP.h>

namespace ImageStudio
{
	namespace Transforms
	{
		namespace Core
		{
			namespace BGRA
			{
#ifndef   IMAGE_EDITOR

				namespace Filters
				{

					enum ETypeFilters
					{
						c_nFilterNon         = 0,
						c_nFilterGauss3x3    = 1,
						c_nFilterLowpass3x3  = 2,

						c_nFilterGauss5x5    = 101,
						c_nFilterLowpass5x5  = 102,
					};
					
					typedef IppStatus (__stdcall *IppFilterFunc)(const Ipp8u* pSrc, int srcStep, Ipp8u* pDst, int dstStep, IppiSize roiSize, IppiMaskSize mask);
					typedef IppStatus (__stdcall *IppCopyReplicateBorderFunc)(const Ipp8u* pSrc,  int srcStep, IppiSize srcRoiSize, Ipp8u* pDst, int dstStep, IppiSize dstRoiSize, int topBorderWidth, int leftBorderWidth);
					
					int ApplyFilter( IppFilterFunc filter, IppiMaskSize eMask, BYTE* pSrcDst, int nWidth, int nHeight, int nStride, int nBytesPerPixel )
					{
						if( !filter )
							return c_nErrorNone;

						if( nBytesPerPixel != 1 && nBytesPerPixel != 4 )
							return c_nErrorInvalidFormat;

						if( !pSrcDst || nWidth <= 0 || nHeight <= 0 || nStride < nWidth * nBytesPerPixel )
							return c_nErrorInvalidSourceImage;

						IppCopyReplicateBorderFunc border = nBytesPerPixel != 1 ? ippiCopyReplicateBorder_8u_C4R : ippiCopyReplicateBorder_8u_C1R;
						
						int offset = (eMask != ippMskSize5x1 && eMask != ippMskSize5x5) ? 1 : 2;
					
						IppiSize ImageSize = {nWidth, nHeight};
						IppiSize BufSize   = {nWidth + offset*2, nHeight + offset*2};

						CBuffer<BYTE> oBuffer( BufSize.width * BufSize.height * nBytesPerPixel );
						if( oBuffer.IsEmpty() )
							return c_nErrorNoMemory;

						BYTE* pBuffer = oBuffer.GetPtr();
						
						border( pSrcDst, nStride, ImageSize, pBuffer, BufSize.width * nBytesPerPixel, BufSize, offset, offset );
						int nRetCode = filter( pBuffer + (BufSize.width + 1) * offset * nBytesPerPixel, BufSize.width * nBytesPerPixel, pSrcDst, nStride, ImageSize, eMask );
						
						return c_nErrorNone;
					}
					int ApplyBlurFilter( int eFilter, int nBytesPerPixel, BYTE* pSrc, int nSrcStep, BYTE* pDst, int nDstStep, int nWidth, int nHeight )
					{
						if( c_nFilterNon == eFilter )
							return c_nErrorNone;

						if( nBytesPerPixel != 1 && nBytesPerPixel != 4 )
							return c_nErrorInvalidFormat;
						
						if( !pSrc || !pDst || nWidth <= 0 || nHeight <= 0 || nSrcStep < nWidth*nBytesPerPixel || nDstStep < nWidth*nBytesPerPixel )
							return c_nErrorInvalidSourceImage;
						
						IppCopyReplicateBorderFunc border = nBytesPerPixel != 1 ? ippiCopyReplicateBorder_8u_C4R : ippiCopyReplicateBorder_8u_C1R;
						IppFilterFunc filter = NULL;
						int offset = 0;

						switch( eFilter )
						{
						case c_nFilterGauss3x3:
							filter = nBytesPerPixel != 1 ? ippiFilterGauss_8u_C4R : ippiFilterGauss_8u_C1R;
							offset = 1;
							break;
						case c_nFilterGauss5x5:
							filter = nBytesPerPixel != 1 ? ippiFilterGauss_8u_C4R : ippiFilterGauss_8u_C1R;
							offset = 2;
							break;
						case c_nFilterLowpass3x3:
							filter = nBytesPerPixel != 1 ? ippiFilterLowpass_8u_AC4R : ippiFilterLowpass_8u_C1R;
							offset = 1;
							break;
						case c_nFilterLowpass5x5:
							filter = nBytesPerPixel != 1 ? ippiFilterLowpass_8u_AC4R : ippiFilterLowpass_8u_C1R;
							offset = 2;
							break;
						default:
							return c_nErrorInvalidFormat;
						}

						IppiSize ImageSize = {nWidth, nHeight};
						IppiSize BufSize   = {nWidth + offset*2, nHeight + offset*2};

						BYTE* pBuffer = new BYTE[BufSize.width * BufSize.height * nBytesPerPixel];
						if( !pBuffer )
							return c_nErrorNoMemory;

						border( pSrc, nSrcStep, ImageSize, pBuffer, BufSize.width * nBytesPerPixel, BufSize, offset, offset );
						filter( pBuffer + (BufSize.width + 1) * offset * nBytesPerPixel, BufSize.width * nBytesPerPixel, pDst, nDstStep, ImageSize, offset != 1 ? ippMskSize5x5 : ippMskSize3x3 );
						
						delete [] pBuffer;
						
						return c_nErrorNone;
					}

				}
#endif

				double Clamp(double v)
				{
					return (v < 0) ? 0 : (v > 1) ? 1 : v; 
				}
				inline double ClampByte(double v)
				{
					return (v < 0) ? 0 : (v > 255) ? 255 : v;
				}
				
				inline DWORD AlphaMul( int dst, int src, int alpha, const int nMaxAlpha )
				{
					return DWORD((src - dst) * alpha + dst * nMaxAlpha + nMaxAlpha/2);
				}
				#define AlphaComp( dst, src, alpha ) int(ImageStudio::Transforms::Core::BGRA::AlphaMul( dst, src, alpha, 255 ) * 0x8081U >> (7 + 16))
				#define AlphaCompEx( dst, src, alpha, max_alpha ) int(ImageStudio::Transforms::Core::BGRA::AlphaMul( dst, src, alpha, max_alpha ) / max_alpha)

				inline BYTE Byte(double value)
				{
					return (value < 0) ? 0 : (value > 255) ? 255 : (BYTE)value; 
				}
				inline BYTE Lightness(double r, double g, double b)
				{ 
					return (BYTE)(0.5*(max(r, max(g, b)) + min(r, min(g, b)))); 
				}
				inline double Intensity( int r, int g, int b )
				{
					//return r * 0.299 + g * 0.587 + b * 0.114;
					
					static const float c_Table[256*3] = {
						0x00*0.114f, 0x01*0.114f, 0x02*0.114f, 0x03*0.114f, 0x04*0.114f, 0x05*0.114f, 0x06*0.114f, 0x07*0.114f, 
						0x08*0.114f, 0x09*0.114f, 0x0A*0.114f, 0x0B*0.114f, 0x0C*0.114f, 0x0D*0.114f, 0x0E*0.114f, 0x0F*0.114f, 
						0x10*0.114f, 0x11*0.114f, 0x12*0.114f, 0x13*0.114f, 0x14*0.114f, 0x15*0.114f, 0x16*0.114f, 0x17*0.114f, 
						0x18*0.114f, 0x19*0.114f, 0x1A*0.114f, 0x1B*0.114f, 0x1C*0.114f, 0x1D*0.114f, 0x1E*0.114f, 0x1F*0.114f, 
						0x20*0.114f, 0x21*0.114f, 0x22*0.114f, 0x23*0.114f, 0x24*0.114f, 0x25*0.114f, 0x26*0.114f, 0x27*0.114f, 
						0x28*0.114f, 0x29*0.114f, 0x2A*0.114f, 0x2B*0.114f, 0x2C*0.114f, 0x2D*0.114f, 0x2E*0.114f, 0x2F*0.114f, 
						0x30*0.114f, 0x31*0.114f, 0x32*0.114f, 0x33*0.114f, 0x34*0.114f, 0x35*0.114f, 0x36*0.114f, 0x37*0.114f, 
						0x38*0.114f, 0x39*0.114f, 0x3A*0.114f, 0x3B*0.114f, 0x3C*0.114f, 0x3D*0.114f, 0x3E*0.114f, 0x3F*0.114f, 
						0x40*0.114f, 0x41*0.114f, 0x42*0.114f, 0x43*0.114f, 0x44*0.114f, 0x45*0.114f, 0x46*0.114f, 0x47*0.114f, 
						0x48*0.114f, 0x49*0.114f, 0x4A*0.114f, 0x4B*0.114f, 0x4C*0.114f, 0x4D*0.114f, 0x4E*0.114f, 0x4F*0.114f, 
						0x50*0.114f, 0x51*0.114f, 0x52*0.114f, 0x53*0.114f, 0x54*0.114f, 0x55*0.114f, 0x56*0.114f, 0x57*0.114f, 
						0x58*0.114f, 0x59*0.114f, 0x5A*0.114f, 0x5B*0.114f, 0x5C*0.114f, 0x5D*0.114f, 0x5E*0.114f, 0x5F*0.114f, 
						0x60*0.114f, 0x61*0.114f, 0x62*0.114f, 0x63*0.114f, 0x64*0.114f, 0x65*0.114f, 0x66*0.114f, 0x67*0.114f, 
						0x68*0.114f, 0x69*0.114f, 0x6A*0.114f, 0x6B*0.114f, 0x6C*0.114f, 0x6D*0.114f, 0x6E*0.114f, 0x6F*0.114f, 
						0x70*0.114f, 0x71*0.114f, 0x72*0.114f, 0x73*0.114f, 0x74*0.114f, 0x75*0.114f, 0x76*0.114f, 0x77*0.114f, 
						0x78*0.114f, 0x79*0.114f, 0x7A*0.114f, 0x7B*0.114f, 0x7C*0.114f, 0x7D*0.114f, 0x7E*0.114f, 0x7F*0.114f, 
						0x80*0.114f, 0x81*0.114f, 0x82*0.114f, 0x83*0.114f, 0x84*0.114f, 0x85*0.114f, 0x86*0.114f, 0x87*0.114f, 
						0x88*0.114f, 0x89*0.114f, 0x8A*0.114f, 0x8B*0.114f, 0x8C*0.114f, 0x8D*0.114f, 0x8E*0.114f, 0x8F*0.114f, 
						0x90*0.114f, 0x91*0.114f, 0x92*0.114f, 0x93*0.114f, 0x94*0.114f, 0x95*0.114f, 0x96*0.114f, 0x97*0.114f, 
						0x98*0.114f, 0x99*0.114f, 0x9A*0.114f, 0x9B*0.114f, 0x9C*0.114f, 0x9D*0.114f, 0x9E*0.114f, 0x9F*0.114f, 
						0xA0*0.114f, 0xA1*0.114f, 0xA2*0.114f, 0xA3*0.114f, 0xA4*0.114f, 0xA5*0.114f, 0xA6*0.114f, 0xA7*0.114f, 
						0xA8*0.114f, 0xA9*0.114f, 0xAA*0.114f, 0xAB*0.114f, 0xAC*0.114f, 0xAD*0.114f, 0xAE*0.114f, 0xAF*0.114f, 
						0xB0*0.114f, 0xB1*0.114f, 0xB2*0.114f, 0xB3*0.114f, 0xB4*0.114f, 0xB5*0.114f, 0xB6*0.114f, 0xB7*0.114f, 
						0xB8*0.114f, 0xB9*0.114f, 0xBA*0.114f, 0xBB*0.114f, 0xBC*0.114f, 0xBD*0.114f, 0xBE*0.114f, 0xBF*0.114f, 
						0xC0*0.114f, 0xC1*0.114f, 0xC2*0.114f, 0xC3*0.114f, 0xC4*0.114f, 0xC5*0.114f, 0xC6*0.114f, 0xC7*0.114f, 
						0xC8*0.114f, 0xC9*0.114f, 0xCA*0.114f, 0xCB*0.114f, 0xCC*0.114f, 0xCD*0.114f, 0xCE*0.114f, 0xCF*0.114f, 
						0xD0*0.114f, 0xD1*0.114f, 0xD2*0.114f, 0xD3*0.114f, 0xD4*0.114f, 0xD5*0.114f, 0xD6*0.114f, 0xD7*0.114f, 
						0xD8*0.114f, 0xD9*0.114f, 0xDA*0.114f, 0xDB*0.114f, 0xDC*0.114f, 0xDD*0.114f, 0xDE*0.114f, 0xDF*0.114f, 
						0xE0*0.114f, 0xE1*0.114f, 0xE2*0.114f, 0xE3*0.114f, 0xE4*0.114f, 0xE5*0.114f, 0xE6*0.114f, 0xE7*0.114f, 
						0xE8*0.114f, 0xE9*0.114f, 0xEA*0.114f, 0xEB*0.114f, 0xEC*0.114f, 0xED*0.114f, 0xEE*0.114f, 0xEF*0.114f, 
						0xF0*0.114f, 0xF1*0.114f, 0xF2*0.114f, 0xF3*0.114f, 0xF4*0.114f, 0xF5*0.114f, 0xF6*0.114f, 0xF7*0.114f, 
						0xF8*0.114f, 0xF9*0.114f, 0xFA*0.114f, 0xFB*0.114f, 0xFC*0.114f, 0xFD*0.114f, 0xFE*0.114f, 0xFF*0.114f, 

						0x00*0.587f, 0x01*0.587f, 0x02*0.587f, 0x03*0.587f, 0x04*0.587f, 0x05*0.587f, 0x06*0.587f, 0x07*0.587f, 
						0x08*0.587f, 0x09*0.587f, 0x0A*0.587f, 0x0B*0.587f, 0x0C*0.587f, 0x0D*0.587f, 0x0E*0.587f, 0x0F*0.587f, 
						0x10*0.587f, 0x11*0.587f, 0x12*0.587f, 0x13*0.587f, 0x14*0.587f, 0x15*0.587f, 0x16*0.587f, 0x17*0.587f, 
						0x18*0.587f, 0x19*0.587f, 0x1A*0.587f, 0x1B*0.587f, 0x1C*0.587f, 0x1D*0.587f, 0x1E*0.587f, 0x1F*0.587f, 
						0x20*0.587f, 0x21*0.587f, 0x22*0.587f, 0x23*0.587f, 0x24*0.587f, 0x25*0.587f, 0x26*0.587f, 0x27*0.587f, 
						0x28*0.587f, 0x29*0.587f, 0x2A*0.587f, 0x2B*0.587f, 0x2C*0.587f, 0x2D*0.587f, 0x2E*0.587f, 0x2F*0.587f, 
						0x30*0.587f, 0x31*0.587f, 0x32*0.587f, 0x33*0.587f, 0x34*0.587f, 0x35*0.587f, 0x36*0.587f, 0x37*0.587f, 
						0x38*0.587f, 0x39*0.587f, 0x3A*0.587f, 0x3B*0.587f, 0x3C*0.587f, 0x3D*0.587f, 0x3E*0.587f, 0x3F*0.587f, 
						0x40*0.587f, 0x41*0.587f, 0x42*0.587f, 0x43*0.587f, 0x44*0.587f, 0x45*0.587f, 0x46*0.587f, 0x47*0.587f, 
						0x48*0.587f, 0x49*0.587f, 0x4A*0.587f, 0x4B*0.587f, 0x4C*0.587f, 0x4D*0.587f, 0x4E*0.587f, 0x4F*0.587f, 
						0x50*0.587f, 0x51*0.587f, 0x52*0.587f, 0x53*0.587f, 0x54*0.587f, 0x55*0.587f, 0x56*0.587f, 0x57*0.587f, 
						0x58*0.587f, 0x59*0.587f, 0x5A*0.587f, 0x5B*0.587f, 0x5C*0.587f, 0x5D*0.587f, 0x5E*0.587f, 0x5F*0.587f, 
						0x60*0.587f, 0x61*0.587f, 0x62*0.587f, 0x63*0.587f, 0x64*0.587f, 0x65*0.587f, 0x66*0.587f, 0x67*0.587f, 
						0x68*0.587f, 0x69*0.587f, 0x6A*0.587f, 0x6B*0.587f, 0x6C*0.587f, 0x6D*0.587f, 0x6E*0.587f, 0x6F*0.587f, 
						0x70*0.587f, 0x71*0.587f, 0x72*0.587f, 0x73*0.587f, 0x74*0.587f, 0x75*0.587f, 0x76*0.587f, 0x77*0.587f, 
						0x78*0.587f, 0x79*0.587f, 0x7A*0.587f, 0x7B*0.587f, 0x7C*0.587f, 0x7D*0.587f, 0x7E*0.587f, 0x7F*0.587f, 
						0x80*0.587f, 0x81*0.587f, 0x82*0.587f, 0x83*0.587f, 0x84*0.587f, 0x85*0.587f, 0x86*0.587f, 0x87*0.587f, 
						0x88*0.587f, 0x89*0.587f, 0x8A*0.587f, 0x8B*0.587f, 0x8C*0.587f, 0x8D*0.587f, 0x8E*0.587f, 0x8F*0.587f, 
						0x90*0.587f, 0x91*0.587f, 0x92*0.587f, 0x93*0.587f, 0x94*0.587f, 0x95*0.587f, 0x96*0.587f, 0x97*0.587f, 
						0x98*0.587f, 0x99*0.587f, 0x9A*0.587f, 0x9B*0.587f, 0x9C*0.587f, 0x9D*0.587f, 0x9E*0.587f, 0x9F*0.587f, 
						0xA0*0.587f, 0xA1*0.587f, 0xA2*0.587f, 0xA3*0.587f, 0xA4*0.587f, 0xA5*0.587f, 0xA6*0.587f, 0xA7*0.587f, 
						0xA8*0.587f, 0xA9*0.587f, 0xAA*0.587f, 0xAB*0.587f, 0xAC*0.587f, 0xAD*0.587f, 0xAE*0.587f, 0xAF*0.587f, 
						0xB0*0.587f, 0xB1*0.587f, 0xB2*0.587f, 0xB3*0.587f, 0xB4*0.587f, 0xB5*0.587f, 0xB6*0.587f, 0xB7*0.587f, 
						0xB8*0.587f, 0xB9*0.587f, 0xBA*0.587f, 0xBB*0.587f, 0xBC*0.587f, 0xBD*0.587f, 0xBE*0.587f, 0xBF*0.587f, 
						0xC0*0.587f, 0xC1*0.587f, 0xC2*0.587f, 0xC3*0.587f, 0xC4*0.587f, 0xC5*0.587f, 0xC6*0.587f, 0xC7*0.587f, 
						0xC8*0.587f, 0xC9*0.587f, 0xCA*0.587f, 0xCB*0.587f, 0xCC*0.587f, 0xCD*0.587f, 0xCE*0.587f, 0xCF*0.587f, 
						0xD0*0.587f, 0xD1*0.587f, 0xD2*0.587f, 0xD3*0.587f, 0xD4*0.587f, 0xD5*0.587f, 0xD6*0.587f, 0xD7*0.587f, 
						0xD8*0.587f, 0xD9*0.587f, 0xDA*0.587f, 0xDB*0.587f, 0xDC*0.587f, 0xDD*0.587f, 0xDE*0.587f, 0xDF*0.587f, 
						0xE0*0.587f, 0xE1*0.587f, 0xE2*0.587f, 0xE3*0.587f, 0xE4*0.587f, 0xE5*0.587f, 0xE6*0.587f, 0xE7*0.587f, 
						0xE8*0.587f, 0xE9*0.587f, 0xEA*0.587f, 0xEB*0.587f, 0xEC*0.587f, 0xED*0.587f, 0xEE*0.587f, 0xEF*0.587f, 
						0xF0*0.587f, 0xF1*0.587f, 0xF2*0.587f, 0xF3*0.587f, 0xF4*0.587f, 0xF5*0.587f, 0xF6*0.587f, 0xF7*0.587f, 
						0xF8*0.587f, 0xF9*0.587f, 0xFA*0.587f, 0xFB*0.587f, 0xFC*0.587f, 0xFD*0.587f, 0xFE*0.587f, 0xFF*0.587f, 

						0x00*0.299f, 0x01*0.299f, 0x02*0.299f, 0x03*0.299f, 0x04*0.299f, 0x05*0.299f, 0x06*0.299f, 0x07*0.299f, 
						0x08*0.299f, 0x09*0.299f, 0x0A*0.299f, 0x0B*0.299f, 0x0C*0.299f, 0x0D*0.299f, 0x0E*0.299f, 0x0F*0.299f, 
						0x10*0.299f, 0x11*0.299f, 0x12*0.299f, 0x13*0.299f, 0x14*0.299f, 0x15*0.299f, 0x16*0.299f, 0x17*0.299f, 
						0x18*0.299f, 0x19*0.299f, 0x1A*0.299f, 0x1B*0.299f, 0x1C*0.299f, 0x1D*0.299f, 0x1E*0.299f, 0x1F*0.299f, 
						0x20*0.299f, 0x21*0.299f, 0x22*0.299f, 0x23*0.299f, 0x24*0.299f, 0x25*0.299f, 0x26*0.299f, 0x27*0.299f, 
						0x28*0.299f, 0x29*0.299f, 0x2A*0.299f, 0x2B*0.299f, 0x2C*0.299f, 0x2D*0.299f, 0x2E*0.299f, 0x2F*0.299f, 
						0x30*0.299f, 0x31*0.299f, 0x32*0.299f, 0x33*0.299f, 0x34*0.299f, 0x35*0.299f, 0x36*0.299f, 0x37*0.299f, 
						0x38*0.299f, 0x39*0.299f, 0x3A*0.299f, 0x3B*0.299f, 0x3C*0.299f, 0x3D*0.299f, 0x3E*0.299f, 0x3F*0.299f, 
						0x40*0.299f, 0x41*0.299f, 0x42*0.299f, 0x43*0.299f, 0x44*0.299f, 0x45*0.299f, 0x46*0.299f, 0x47*0.299f, 
						0x48*0.299f, 0x49*0.299f, 0x4A*0.299f, 0x4B*0.299f, 0x4C*0.299f, 0x4D*0.299f, 0x4E*0.299f, 0x4F*0.299f, 
						0x50*0.299f, 0x51*0.299f, 0x52*0.299f, 0x53*0.299f, 0x54*0.299f, 0x55*0.299f, 0x56*0.299f, 0x57*0.299f, 
						0x58*0.299f, 0x59*0.299f, 0x5A*0.299f, 0x5B*0.299f, 0x5C*0.299f, 0x5D*0.299f, 0x5E*0.299f, 0x5F*0.299f, 
						0x60*0.299f, 0x61*0.299f, 0x62*0.299f, 0x63*0.299f, 0x64*0.299f, 0x65*0.299f, 0x66*0.299f, 0x67*0.299f, 
						0x68*0.299f, 0x69*0.299f, 0x6A*0.299f, 0x6B*0.299f, 0x6C*0.299f, 0x6D*0.299f, 0x6E*0.299f, 0x6F*0.299f, 
						0x70*0.299f, 0x71*0.299f, 0x72*0.299f, 0x73*0.299f, 0x74*0.299f, 0x75*0.299f, 0x76*0.299f, 0x77*0.299f, 
						0x78*0.299f, 0x79*0.299f, 0x7A*0.299f, 0x7B*0.299f, 0x7C*0.299f, 0x7D*0.299f, 0x7E*0.299f, 0x7F*0.299f, 
						0x80*0.299f, 0x81*0.299f, 0x82*0.299f, 0x83*0.299f, 0x84*0.299f, 0x85*0.299f, 0x86*0.299f, 0x87*0.299f, 
						0x88*0.299f, 0x89*0.299f, 0x8A*0.299f, 0x8B*0.299f, 0x8C*0.299f, 0x8D*0.299f, 0x8E*0.299f, 0x8F*0.299f, 
						0x90*0.299f, 0x91*0.299f, 0x92*0.299f, 0x93*0.299f, 0x94*0.299f, 0x95*0.299f, 0x96*0.299f, 0x97*0.299f, 
						0x98*0.299f, 0x99*0.299f, 0x9A*0.299f, 0x9B*0.299f, 0x9C*0.299f, 0x9D*0.299f, 0x9E*0.299f, 0x9F*0.299f, 
						0xA0*0.299f, 0xA1*0.299f, 0xA2*0.299f, 0xA3*0.299f, 0xA4*0.299f, 0xA5*0.299f, 0xA6*0.299f, 0xA7*0.299f, 
						0xA8*0.299f, 0xA9*0.299f, 0xAA*0.299f, 0xAB*0.299f, 0xAC*0.299f, 0xAD*0.299f, 0xAE*0.299f, 0xAF*0.299f, 
						0xB0*0.299f, 0xB1*0.299f, 0xB2*0.299f, 0xB3*0.299f, 0xB4*0.299f, 0xB5*0.299f, 0xB6*0.299f, 0xB7*0.299f, 
						0xB8*0.299f, 0xB9*0.299f, 0xBA*0.299f, 0xBB*0.299f, 0xBC*0.299f, 0xBD*0.299f, 0xBE*0.299f, 0xBF*0.299f, 
						0xC0*0.299f, 0xC1*0.299f, 0xC2*0.299f, 0xC3*0.299f, 0xC4*0.299f, 0xC5*0.299f, 0xC6*0.299f, 0xC7*0.299f, 
						0xC8*0.299f, 0xC9*0.299f, 0xCA*0.299f, 0xCB*0.299f, 0xCC*0.299f, 0xCD*0.299f, 0xCE*0.299f, 0xCF*0.299f, 
						0xD0*0.299f, 0xD1*0.299f, 0xD2*0.299f, 0xD3*0.299f, 0xD4*0.299f, 0xD5*0.299f, 0xD6*0.299f, 0xD7*0.299f, 
						0xD8*0.299f, 0xD9*0.299f, 0xDA*0.299f, 0xDB*0.299f, 0xDC*0.299f, 0xDD*0.299f, 0xDE*0.299f, 0xDF*0.299f, 
						0xE0*0.299f, 0xE1*0.299f, 0xE2*0.299f, 0xE3*0.299f, 0xE4*0.299f, 0xE5*0.299f, 0xE6*0.299f, 0xE7*0.299f, 
						0xE8*0.299f, 0xE9*0.299f, 0xEA*0.299f, 0xEB*0.299f, 0xEC*0.299f, 0xED*0.299f, 0xEE*0.299f, 0xEF*0.299f, 
						0xF0*0.299f, 0xF1*0.299f, 0xF2*0.299f, 0xF3*0.299f, 0xF4*0.299f, 0xF5*0.299f, 0xF6*0.299f, 0xF7*0.299f, 
						0xF8*0.299f, 0xF9*0.299f, 0xFA*0.299f, 0xFB*0.299f, 0xFC*0.299f, 0xFD*0.299f, 0xFE*0.299f, 0xFF*0.299f, 
					};
					
					return c_Table[b] + c_Table[g + 256] + c_Table[r + 512];
				}
				
				inline int Lightness2( int r, int g, int b )
				{
					int nMax, nMin;

					if( r > g )
					{
						nMax = max(r, b);
						nMin = min(g, b);
					}
					else
					{
						nMax = max(g, b);
						nMin = min(r, b);
					}
					return (nMax + nMin) >> 1;
				}
				inline int Intensity2(int r, int g, int b)
				{
					// Y' = (R * 77 + G * 150 + B * 29 + 128) / 256;

					return (r * 77 + g * 150 + b * 29 + 128) >> 8;
				}
				inline DWORD Intensity3( int r, int g, int b)
				{
					// Y = (R * 76 + G * 150 + B * 29 + 128) / 255;

					return ((DWORD)r * (76 * 0x8081U) + (DWORD)g * (150 * 0x8081U) + (DWORD)b * (29 * 0x8081U) + (128 * 0x8081U)) >> 23;
				}
				inline int Glow( int color, int alpha, int offset )
				{
					// color[0; 255], alpha[0; 256], offset[0; 255]
					//
					
					color += ((color - ((color * color + color) >> 8)) * alpha >> 8) + offset;
					return min(color, 255);
				}

				
				Gdiplus::Color GetColor( int color, int alpha )
				{
					return Gdiplus::Color(min(max(0, alpha), 255), GetRValue(color), GetGValue(color), GetBValue(color));
				}


				int BGRA_ApplyColorTable( BYTE* pPixels, int nWidth, int nHeight, BYTE vTable[256] )
				{
					int nSize = nWidth * nHeight;

					for( int i = 0; i < nSize; ++i, pPixels += 4 )
					{
						pPixels[0] = vTable[pPixels[0]];
						pPixels[1] = vTable[pPixels[1]];
						pPixels[2] = vTable[pPixels[2]];
					}

					return c_nErrorNone;
				}

				int BGRA_ApplyColorTableEx( BYTE* pPixels, int nWidth, int nHeight, BYTE vTable[3][256] )
				{
					int nSize = nWidth * nHeight;

					for( int i = 0; i < nSize; ++i, pPixels += 4 )
					{
						pPixels[0] = vTable[0][pPixels[0]];
						pPixels[1] = vTable[1][pPixels[1]];
						pPixels[2] = vTable[2][pPixels[2]];
					}

					return c_nErrorNone;
				}

				
				int BGRA_AdjustRecovery(BYTE* pPixels, int nWidth, int nHeight, double dFrame, int nRecovery)
				{
					dFrame = Clamp( dFrame );

					//double dFactor = nRecovery * dFrame / (1000.0 * 255.0);

					//BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					//for( ; pPixels != pMaxPtr; pPixels += 4 )
					//{
					//	int nB = pPixels[0];
					//	int nG = pPixels[1];
					//	int nR = pPixels[2];

					//	double dK = 1 - dFactor * Intensity( nR, nG, nB );

					//	nR = int(nR * dK + 0.5);
					//	nG = int(nG * dK + 0.5);
					//	nB = int(nB * dK + 0.5);

					//	pPixels[0] = IntToByte(nB);
					//	pPixels[1] = IntToByte(nG);
					//	pPixels[2] = IntToByte(nR);
					//}

					/* Целочисленный вариант */
					double dFactor = nRecovery * dFrame * (1024.0 / (1000.0 * 255.0));
					
					int table[256];
					for( int i = 0; i < 256; i++ )
					{
						table[i] = int(1024.0 - i * dFactor);
					}
					
					BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					for( ; pPixels != pMaxPtr; pPixels += 4 )
					{
						int nB = pPixels[0];
						int nG = pPixels[1];
						int nR = pPixels[2];

						int nK = table[Intensity2(nR, nG, nB)];

						nR = (nR * nK + 512) >> 10;
						nG = (nG * nK + 512) >> 10;
						nB = (nB * nK + 512) >> 10;

						pPixels[0] = IntToByte(nB);
						pPixels[1] = IntToByte(nG);
						pPixels[2] = IntToByte(nR);
					}

					return ImageStudio::Transforms::Core::c_nErrorNone;
				}
				int BGRA_AdjustFillLight(BYTE* pPixels, int nWidth, int nHeight, double dFrame, int nFillLight)
				{
					dFrame = Clamp( dFrame );

					//float fFillLight = float(nFillLight);
					//int nIndex = 0;

					//float fKoef = fFillLight / 100.0f ;

					//
					//float fR = 0, fG = 0, fB = 0;			

					//for (int nY = 0; nY < nHeight; ++nY)
					//{
					//	for (int nX = 0; nX < nWidth; ++nX, nIndex += 4)
					//	{
					//		fR = (float)pPixels[nIndex+2];
					//		fG = (float)pPixels[nIndex+1];
					//		fB = (float)pPixels[nIndex+0];

					//		float fYKoef = ( fR + fG + fB ) / (3.0f * 255.0f);

					//		//if ( fYKoef < 0.25 )
					//		//{
					//		//	fR += fKoef * 100 * fR / 255.0f;
					//		//	fG += fKoef * 100 * fG / 255.0f;
					//		//	fB += fKoef * 100 * fB / 255.0f;
					//		//}
					//		if ( fYKoef < 0.75f )
					//		{
					//			float fKof = (fYKoef - 0) / 0.75f;

					//			fR += fKoef * 100 * fR / 255.0f * (1 - fKof);
					//			fG += fKoef * 100 * fG / 255.0f * (1 - fKof);
					//			fB += fKoef * 100 * fB / 255.0f * (1 - fKof);


					//		}

					//		pPixels[nIndex+2] = ImageStudio::Transforms::Core::BGRA::Byte(fR);
					//		pPixels[nIndex+1] = ImageStudio::Transforms::Core::BGRA::Byte(fG);
					//		pPixels[nIndex+0] = ImageStudio::Transforms::Core::BGRA::Byte(fB);
					//	}
					//}

					//double dFactor = dFrame * nFillLight / 255.0;
					//double dOffset = 1 + dFactor;
					//dFactor /= 255.0;

					//BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					//for( ; pPixels != pMaxPtr; pPixels += 4 )
					//{
					//	int nB = pPixels[0];
					//	int nG = pPixels[1];
					//	int nR = pPixels[2];

					//	double dK = dOffset - dFactor * Intensity(nR, nG, nB);

					//	nR = int(nR * dK + 0.5);
					//	nG = int(nG * dK + 0.5);
					//	nB = int(nB * dK + 0.5);

					//	pPixels[0] = IntToByte(nB);
					//	pPixels[1] = IntToByte(nG);
					//	pPixels[2] = IntToByte(nR);
					//}

					/* Целочисленный вариант */
					double dFactor = dFrame * nFillLight * (1024.0 / 255.0);
					double dOffset = 1024 + dFactor;
					dFactor /= 255.0;

					int table[256];
					for( int i = 0; i < 256; i++ )
					{
						table[i] = Round( dOffset - i * dFactor );
					}

					BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					for( ; pPixels != pMaxPtr; pPixels += 4 )
					{
						int nB = pPixels[0];
						int nG = pPixels[1];
						int nR = pPixels[2];

						int nK = table[Intensity2(nR, nG, nB)];

						nR = (nR * nK + 512) >> 10;
						nG = (nG * nK + 512) >> 10;
						nB = (nB * nK + 512) >> 10;

						pPixels[0] = IntToByte(nB);
						pPixels[1] = IntToByte(nG);
						pPixels[2] = IntToByte(nR);
					}

					return ImageStudio::Transforms::Core::c_nErrorNone;
				}

				int BGRA_AdjustExposure(BYTE* pPixels, int nWidth, int nHeight, double dFrame, int nExposure)
				{
					dFrame = Clamp( dFrame );

					/* Вариант 1(оригинал) */
					//float fExposure = min( 255, max( 0, nExposure / 100.0f * 255.0f));
					//
					//int nIndex = 0;
					//
					//float fR = 0, fG = 0, fB = 0;			

					//for (int nY = 0; nY < nHeight; ++nY)
					//{
					//	for (int nX = 0; nX < nWidth; ++nX, nIndex += 4)
					//	{
					//		fR = (float)pPixels[nIndex+2];
					//		fG = (float)pPixels[nIndex+1];
					//		fB = (float)pPixels[nIndex+0];

					//		float fY =  0.299f  * fR + 0.587f  * fG + 0.114f  * fB + 0.0f;
					//		float fU = -0.1687f * fR - 0.3313f * fG + 0.5f    * fB + 128.0f;
					//		float fV =  0.5f    * fR - 0.4187f * fG - 0.0813f * fB + 128.0f;

					//		if ( fY > (255 - fExposure) )
					//			fY = 255;
					//		else
					//			fY = 255 * fY / ( 255 - fExposure + 1);

					//		pPixels[nIndex+2] = ImageStudio::Transforms::Core::BGRA::Byte( fY                           + 1.402f   * ( fV - 128) );
					//		pPixels[nIndex+1] = ImageStudio::Transforms::Core::BGRA::Byte( fY - 0.34414f * ( fU - 128 ) - 0.71414f * ( fV - 128) );
					//		pPixels[nIndex+0] = ImageStudio::Transforms::Core::BGRA::Byte( fY + 1.772f   * ( fU - 128 ) );
					//	}
					//}

					/* Вариант 2 */
					//nExposure = max(0, min(100, nExposure));
					//double dExposure = nExposure * dFrame * (255.0 / 100.0);
					//double dFactor = dExposure / (256 - dExposure);

					//dExposure = 255 - dExposure;

					//BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					//for( ; pPixels != pMaxPtr; pPixels += 4 )
					//{
					//	int nB = pPixels[0];
					//	int nG = pPixels[1];
					//	int nR = pPixels[2];

					//	double dY = Intensity( nR, nG, nB );

					//	if( dY > dExposure )
					//		dY = 255 - dY;
					//	else
					//		dY = dY * dFactor + dFactor;

					//	int offset = int(dY + 0.5);
					//	
					//	pPixels[0] = min(nB + offset, 255);
					//	pPixels[1] = min(nG + offset, 255);
					//	pPixels[2] = min(nR + offset, 255);
					//}

					/* Вариант 3(целочисленный) */
					nExposure = max(0, min(100, nExposure));
					double dExposure = nExposure * dFrame * 255 / 100.0;
					double dFactor = dExposure / (256 - dExposure);

					dExposure = 255 - dExposure;

					int table[256];
					
					for( int i = 0; i < 256; i++ )
					{
						double dY = i;

						if( dY >= dExposure )
							dY = 255 - dY;
						else
							dY *= dFactor + dFactor;

						table[i] = int(dY + 0.5);
					}

					BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					for( ; pPixels != pMaxPtr; pPixels += 4 )
					{
						int nB = pPixels[0];
						int nG = pPixels[1];
						int nR = pPixels[2];

						int offset = table[Intensity2(nR, nG, nB)];
						
						pPixels[0] = min(nB + offset, 255);
						pPixels[1] = min(nG + offset, 255);
						pPixels[2] = min(nR + offset, 255);
					}

					/* кокой-то старый вариант */
					//int nIndex = 0;

					//float fR, fG, fB;

					//float fExposure = min( 100, max( -100, nExposure ));
					//float expos = pow(2.0f, ((float)fExposure)/100.0f);

					//BYTE TableExpl[256];

					//for (int i = 0; i < 256; ++i)
					//{
					//	float val = ((float)i)/255.0f;
					//	if (val<=0.04045) val /= 12.92f; else val = pow((val+0.055f)/1.055f,2.4f);
					//	val *= expos;
					//	if (val<=0.0031308) val *= 12.92f; else val = 1.055*pow(val,1.0f/2.4f)-0.055;
					//	TableExpl[i] = ImageStudio::Transforms::Core::BGRA::Byte( 255.0f*val );
					//}

					//for (int nY = 0; nY < nHeight; ++nY)
					//{
					//	for (int nX = 0; nX < nWidth; ++nX, nIndex += 4)
					//	{
					//		/ *fR = ((float)pPixels[nIndex+2])/255.0f;
					//		fG = ((float)pPixels[nIndex+1])/255.0f;
					//		fB = ((float)pPixels[nIndex+0])/255.0f;

					//		if (fR<=0.04045) fR /= 12.92f; else fR = pow((fR+0.055f)/1.055f,2.4f);
					//		if (fG<=0.04045) fG /= 12.92f; else fG = pow((fG+0.055f)/1.055f,2.4f);
					//		if (fB<=0.04045) fB /= 12.92f; else fB = pow((fB+0.055f)/1.055f,2.4f);

					//		fR *= expos;
					//		fG *= expos;
					//		fB *= expos;

					//		if (fR<=0.0031308) fR *= 12.92f; else fR = 1.055*pow(fR,1.0f/2.4f)-0.055;
					//		if (fG<=0.0031308) fG *= 12.92f; else fG = 1.055*pow(fG,1.0f/2.4f)-0.055;
					//		if (fB<=0.0031308) fB *= 12.92f; else fB = 1.055*pow(fB,1.0f/2.4f)-0.055;
					//		

					//		pPixels[nIndex+2] = ImageStudio::Transforms::Core::BGRA::Byte( 255.0f*fR );
					//		pPixels[nIndex+1] = ImageStudio::Transforms::Core::BGRA::Byte( 255.0f*fG );
					//		pPixels[nIndex+0] = ImageStudio::Transforms::Core::BGRA::Byte( 255.0f*fB );* /

					//		pPixels[nIndex+2] = TableExpl[pPixels[nIndex+2]];
					//		pPixels[nIndex+1] = TableExpl[pPixels[nIndex+1]];
					//		pPixels[nIndex+0] = TableExpl[pPixels[nIndex+0]];
					//	}
					//}

					return ImageStudio::Transforms::Core::c_nErrorNone;
				}
				int BGRA_AdjustBlacks(BYTE* pPixels, int nWidth, int nHeight, double dFrame, int nBlack)
				{
					dFrame = Clamp( dFrame );

					/* Вариант 1 (оригинал) */
					//float fBlack = min( 255, max( 0, nBlack / 100.0f * 255.0f));
					//
					//int nIndex = 0;
					//
					//float fR = 0, fG = 0, fB = 0;			

					//for (int nY = 0; nY < nHeight; ++nY)
					//{
					//	for (int nX = 0; nX < nWidth; ++nX, nIndex += 4)
					//	{
					//		fR = (float)pPixels[nIndex+2];
					//		fG = (float)pPixels[nIndex+1];
					//		fB = (float)pPixels[nIndex+0];

					//		float fY =  0.299f  * fR + 0.587f  * fG + 0.114f  * fB + 0.0f;
					//		float fU = -0.1687f * fR - 0.3313f * fG + 0.5f    * fB + 128.0f;
					//		float fV =  0.5f    * fR - 0.4187f * fG - 0.0813f * fB + 128.0f;

					//		if ( fY < fBlack )
					//			fY = 0;
					//		else
					//			fY = 255 * ( fY - fBlack ) / ( 255 - fBlack + 1);
					//		
					//		pPixels[nIndex+2] = ImageStudio::Transforms::Core::BGRA::Byte( fY                           + 1.402f   * ( fV - 128) );
					//		pPixels[nIndex+1] = ImageStudio::Transforms::Core::BGRA::Byte( fY - 0.34414f * ( fU - 128 ) - 0.71414f * ( fV - 128) );
					//		pPixels[nIndex+0] = ImageStudio::Transforms::Core::BGRA::Byte( fY + 1.772f   * ( fU - 128 ) );
					//	}
					//}

					nBlack = max(0, min(100, nBlack));
					double dBlack = nBlack * dFrame * (255.0 / 100.0);

					/* Вариант 2 */
					//double dFactor = 255.0 / (256 - dBlack);
					//double dOffset = dBlack * dFactor;
					//
					//dFactor -= 1;

					//BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					//for( ; pPixels != pMaxPtr; pPixels += 4 )
					//{
					//	int nB = pPixels[0];
					//	int nG = pPixels[1];
					//	int nR = pPixels[2];

					//	double dY = Intensity( nR, nG, nB );
					//	
					//	if( dY > dBlack )
					//		dY = dY * dFactor - dOffset;
					//	else
					//		dY = -dY;

					//	int offset = int(dY + 0.5);

					//	pPixels[0] = max(nB + offset, 0);
					//	pPixels[1] = max(nG + offset, 0);
					//	pPixels[2] = max(nR + offset, 0);
					//}

					/* Целочисленный вариант */
					int table[256];
					for( int i = 0; i < 256; i++ )
					{
						int offset = 0;
						if( i > dBlack )
						{
							offset = int(255 * (i - dBlack) / (256 - dBlack) + 0.5);
						}
						table[i] = offset - i;
					}

					BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					for( ; pPixels != pMaxPtr; pPixels += 4 )
					{
						int nB = pPixels[0];
						int nG = pPixels[1];
						int nR = pPixels[2];

						int offset = table[Intensity2( nR, nG, nB )];

						pPixels[0] = max(nB + offset, 0);
						pPixels[1] = max(nG + offset, 0);
						pPixels[2] = max(nR + offset, 0);
					}

					return ImageStudio::Transforms::Core::c_nErrorNone;
				}
				int BGRA_AdjustHighLight(BYTE* pPixels, int nWidth, int nHeight, double dFrame, int nFillLight, int nExposure)
				{
					dFrame = Clamp( dFrame );

					//float fFillLight = nFillLight / 100.0f;
					//float fExposure = nExposure / 100.0f;

					//int nIndex = 0;
					//
					//float fR = 0, fG = 0, fB = 0, fY = 0;

					//float fVar = 0.3f * (1.0f + (100.0f - ((float)nFillLight * 1.1f) ) * 2.0f / 100.0f);
					//

					//for (int nY = 0; nY < nHeight; ++nY)
					//{
					//	for (int nX = 0; nX < nWidth; ++nX, nIndex += 4)
					//	{
					//		fR = (float)pPixels[nIndex+2];
					//		fG = (float)pPixels[nIndex+1];
					//		fB = (float)pPixels[nIndex+0];

					//		fY = (fR + fG + fB) / 3.0f;

					//		float fCurve = (50.0f / (fY + 10.0f) - fVar); // this is our "tone cuve" (actually factor by which Amount control is pre-amplified)
					//		
					//		if ( fCurve < 0 )
					//			fCurve = 0;

					//		fCurve = min( 0.33333f, fCurve);
					//		
					//		float fKoef = (1 + fCurve) * fFillLight * fY;

					//		fR += fKoef + fExposure * ( fR * fCurve * fFillLight - fR - fKoef);
					//		fG += fKoef + fExposure * ( fG * fCurve * fFillLight - fG - fKoef);
					//		fB += fKoef + fExposure * ( fB * fCurve * fFillLight - fB - fKoef);

					//		pPixels[nIndex+2] = ImageStudio::Transforms::Core::BGRA::Byte(fR);
					//		pPixels[nIndex+1] = ImageStudio::Transforms::Core::BGRA::Byte(fG);
					//		pPixels[nIndex+0] = ImageStudio::Transforms::Core::BGRA::Byte(fB);
					//	}
					//}

					double dFillLight = dFrame * nFillLight / 100.0;
					double dExposure  = dFrame * nExposure  / 100.0;

					double dVar = 0.9 - dFillLight * (2.0/3.0);

					float fTable[256 * 2];
					for( int i = 0; i < 256; i++ )
					{
						double dY = i;
						double dCurve = 50.0 / (dY + 10) - dVar;

						if( dCurve < 0 ) dCurve = 0;
						if( dCurve > (1.0/3.0) ) dCurve = (1.0/3.0);

						fTable[i * 2 + 0] = float(dExposure  * (dCurve * dFillLight - 1) + 1); // factor
						fTable[i * 2 + 1] = float(dFillLight * (dCurve + 1) * (1 - dExposure) * dY); // offset
					}

					BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					for( ; pPixels != pMaxPtr; pPixels += 4 )
					{
						int nB = pPixels[0];
						int nG = pPixels[1];
						int nR = pPixels[2];

						int index = Intensity2(nR, nG, nB );
						double dFactor = fTable[index * 2 + 0];
						double dOffset = fTable[index * 2 + 1];

						nR = int(nR * dFactor + dOffset);
						nG = int(nG * dFactor + dOffset);
						nB = int(nB * dFactor + dOffset);

						pPixels[0] = IntToByte(nB);
						pPixels[1] = IntToByte(nG);
						pPixels[2] = IntToByte(nR);
					}

					return ImageStudio::Transforms::Core::c_nErrorNone;
				}
				int BGRA_AdjustShadows(BYTE* pPixels, int nWidth, int nHeight, double dFrame, int nBlacks, int nRecovery = 0)
				{
					dFrame = Clamp( dFrame );
					
					//float fBlacks = nBlacks / 100.0f;
					//float fRecovery = nRecovery / 100.0f;

					//// initialize pixel index
					//int nIndex = 0;
					//
					//float fR = 0, fG = 0, fB = 0, fY = 0;

					//float fVar = (1.0f + (100.0f - (float)nBlacks) * 2.0f / 100.0f);

					//for (int nY = 0; nY < nHeight; ++nY)
					//{
					//	for (int nX = 0; nX < nWidth; ++nX, nIndex += 4)
					//	{
					//		fR = (float)pPixels[nIndex+2];
					//		fG = (float)pPixels[nIndex+1];
					//		fB = (float)pPixels[nIndex+0];

					//		fY = ((fR + fG + fB) / 3.0f);

					//		if ( fY < 0.00001f && fY > -0.00001f )
					//			fY = 0.01f;

					//		float fRevY = (255.0f - fY);

					//		float fCurve = (50.0f / (fRevY + 10.0f) - 0.3f * fVar);
					//		
					//		if ( fCurve < 0 )
					//			fCurve = 0;

					//		fCurve = min( 0.33333f, fCurve);

					//		float fTemp = fCurve * fRecovery;

					//		float fRKoef = 1.0f - (fRevY * fCurve * fBlacks + ( fY - fR ) * fTemp) / fY;
					//		float fGKoef = 1.0f - (fRevY * fCurve * fBlacks + ( fY - fG ) * fTemp) / fY;
					//		float fBKoef = 1.0f - (fRevY * fCurve * fBlacks + ( fY - fB ) * fTemp) / fY;

					//		pPixels[nIndex+2] = ImageStudio::Transforms::Core::BGRA::Byte(fR * fRKoef);
					//		pPixels[nIndex+1] = ImageStudio::Transforms::Core::BGRA::Byte(fG * fGKoef);
					//		pPixels[nIndex+0] = ImageStudio::Transforms::Core::BGRA::Byte(fB * fBKoef);

					//	}
					//}

					double dBlacks   = dFrame * nBlacks   / 100.0f;
					double dRecovery = dFrame * nRecovery / 100.0f;

					double dVar = 0.9 - dBlacks * 0.6;

					float fTable[256 * 2];
					for( int i = 0; i < 256; i++ )
					{
						double dY = (i > 0) ? i : 0.000001;
						double dCurve = 50.0 / (255 - dY + 10) - dVar;

						if( dCurve < 0 ) dCurve = 0;
						if( dCurve > (1.0/3.0) ) dCurve = (1.0/3.0);

						fTable[i * 2 + 0] = float(dCurve * dRecovery / dY); // factor
						fTable[i * 2 + 1] = float(1 - dCurve * (dBlacks * (255 / dY - 1) + dRecovery)); // offset
					}

					BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					for( ; pPixels != pMaxPtr; pPixels += 4 )
					{
						int nB = pPixels[0];
						int nG = pPixels[1];
						int nR = pPixels[2];

						int index = Intensity2( nR, nG, nB );
						double dFactor = fTable[index * 2 + 0];
						double dOffset = fTable[index * 2 + 1];

						nR = int(nR * (nR * dFactor + dOffset));
						nG = int(nG * (nG * dFactor + dOffset));
						nB = int(nB * (nB * dFactor + dOffset));

						pPixels[0] = IntToByte(nB);
						pPixels[1] = IntToByte(nG);
						pPixels[2] = IntToByte(nR);
					}

					return ImageStudio::Transforms::Core::c_nErrorNone;
				}

				
				BOOL BGRA_ResizeCenter(BYTE* pBGRASource, BYTE* pBGRAResult, int nWidth, int nHeight, double dXFactor, double dYFactor, double dXCenter, double dYCenter)
				{
					if( abs(dXFactor) < 0.0001 || abs(dYFactor) < 0.0001 )
						return TRUE;

					int offset = 4 * nWidth * (nHeight - 1);

					Gdiplus::Bitmap oSourceImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBGRASource + offset );
					if( Gdiplus::Ok != oSourceImage.GetLastStatus() )
						return FALSE;

					Gdiplus::Bitmap oResultImage( nWidth, nHeight, -4*nWidth, PixelFormat32bppARGB, pBGRAResult + offset );
					if( Gdiplus::Ok != oResultImage.GetLastStatus() )
						return FALSE;
					
					Gdiplus::Graphics oRender( &oResultImage );
					if( Gdiplus::Ok != oRender.GetLastStatus() )
						return FALSE;
					
					oRender.SetInterpolationMode( Gdiplus::InterpolationModeBilinear );
					
					double dRectWidth  = nWidth  * dXFactor;
					double dRectHeight = nHeight * dYFactor;
					
					Gdiplus::RectF oDstRect( 
						float(dXCenter * (1 - dXFactor)),  
						float(dYCenter * (1 - dYFactor)), 
						float(dRectWidth), 
						float(dRectHeight)
					);

					oRender.DrawImage( &oSourceImage, oDstRect, 0, 0, float(nWidth), float(nHeight), Gdiplus::UnitPixel ); 
					
					return TRUE;
				}


				void BgrToHls( const BYTE* pBgrColor, double* pHlsColor )
				{
					// //hue        = [0..360);
					// //lightness  = [0..1];
					// //saturation = [0..1];
					//
					static const float c_fTable[511] = { // таблица для замены деления умножением
						0,
						1/001.f, 1/002.f, 1/003.f, 1/004.f, 1/005.f, 1/006.f, 1/007.f, 1/008.f, 1/009.f, 1/010.f,
						1/011.f, 1/012.f, 1/013.f, 1/014.f, 1/015.f, 1/016.f, 1/017.f, 1/018.f, 1/019.f, 1/020.f,
						1/021.f, 1/022.f, 1/023.f, 1/024.f, 1/025.f, 1/026.f, 1/027.f, 1/028.f, 1/029.f, 1/030.f,
						1/031.f, 1/032.f, 1/033.f, 1/034.f, 1/035.f, 1/036.f, 1/037.f, 1/038.f, 1/039.f, 1/040.f,
						1/041.f, 1/042.f, 1/043.f, 1/044.f, 1/045.f, 1/046.f, 1/047.f, 1/048.f, 1/049.f, 1/050.f,
						1/051.f, 1/052.f, 1/053.f, 1/054.f, 1/055.f, 1/056.f, 1/057.f, 1/058.f, 1/059.f, 1/060.f,
						1/061.f, 1/062.f, 1/063.f, 1/064.f, 1/065.f, 1/066.f, 1/067.f, 1/068.f, 1/069.f, 1/070.f,
						1/071.f, 1/072.f, 1/073.f, 1/074.f, 1/075.f, 1/076.f, 1/077.f, 1/078.f, 1/079.f, 1/080.f,
						1/081.f, 1/082.f, 1/083.f, 1/084.f, 1/085.f, 1/086.f, 1/087.f, 1/088.f, 1/089.f, 1/090.f,
						1/091.f, 1/092.f, 1/093.f, 1/094.f, 1/095.f, 1/096.f, 1/097.f, 1/098.f, 1/099.f, 1/100.f,

						1/101.f, 1/102.f, 1/103.f, 1/104.f, 1/105.f, 1/106.f, 1/107.f, 1/108.f, 1/109.f, 1/110.f,
						1/111.f, 1/112.f, 1/113.f, 1/114.f, 1/115.f, 1/116.f, 1/117.f, 1/118.f, 1/119.f, 1/120.f,
						1/121.f, 1/122.f, 1/123.f, 1/124.f, 1/125.f, 1/126.f, 1/127.f, 1/128.f, 1/129.f, 1/130.f,
						1/131.f, 1/132.f, 1/133.f, 1/134.f, 1/135.f, 1/136.f, 1/137.f, 1/138.f, 1/139.f, 1/140.f,
						1/141.f, 1/142.f, 1/143.f, 1/144.f, 1/145.f, 1/146.f, 1/147.f, 1/148.f, 1/149.f, 1/150.f,
						1/151.f, 1/152.f, 1/153.f, 1/154.f, 1/155.f, 1/156.f, 1/157.f, 1/158.f, 1/159.f, 1/160.f,
						1/161.f, 1/162.f, 1/163.f, 1/164.f, 1/165.f, 1/166.f, 1/167.f, 1/168.f, 1/169.f, 1/170.f,
						1/171.f, 1/172.f, 1/173.f, 1/174.f, 1/175.f, 1/176.f, 1/177.f, 1/178.f, 1/179.f, 1/180.f,
						1/181.f, 1/182.f, 1/183.f, 1/184.f, 1/185.f, 1/186.f, 1/187.f, 1/188.f, 1/189.f, 1/190.f,
						1/191.f, 1/192.f, 1/193.f, 1/194.f, 1/195.f, 1/196.f, 1/197.f, 1/198.f, 1/199.f, 1/200.f,

						1/201.f, 1/202.f, 1/203.f, 1/204.f, 1/205.f, 1/206.f, 1/207.f, 1/208.f, 1/209.f, 1/210.f,
						1/211.f, 1/212.f, 1/213.f, 1/214.f, 1/215.f, 1/216.f, 1/217.f, 1/218.f, 1/219.f, 1/220.f,
						1/221.f, 1/222.f, 1/223.f, 1/224.f, 1/225.f, 1/226.f, 1/227.f, 1/228.f, 1/229.f, 1/230.f,
						1/231.f, 1/232.f, 1/233.f, 1/234.f, 1/235.f, 1/236.f, 1/237.f, 1/238.f, 1/239.f, 1/240.f,
						1/241.f, 1/242.f, 1/243.f, 1/244.f, 1/245.f, 1/246.f, 1/247.f, 1/248.f, 1/249.f, 1/250.f,
						1/251.f, 1/252.f, 1/253.f, 1/254.f, 1/255.f, 1/254.f, 1/253.f, 1/252.f, 1/251.f, 1/250.f,
						1/249.f, 1/248.f, 1/247.f, 1/246.f, 1/245.f, 1/244.f, 1/243.f, 1/242.f, 1/241.f, 1/240.f,
						1/239.f, 1/238.f, 1/237.f, 1/236.f, 1/235.f, 1/234.f, 1/233.f, 1/232.f, 1/231.f, 1/230.f,
						1/229.f, 1/228.f, 1/227.f, 1/226.f, 1/225.f, 1/224.f, 1/223.f, 1/222.f, 1/221.f, 1/220.f,
						1/219.f, 1/218.f, 1/217.f, 1/216.f, 1/215.f, 1/214.f, 1/213.f, 1/212.f, 1/211.f, 1/210.f,
						1/209.f, 1/208.f, 1/207.f, 1/206.f, 1/205.f, 1/204.f, 1/203.f, 1/202.f, 1/201.f, 1/200.f,

						1/199.f, 1/198.f, 1/197.f, 1/196.f, 1/195.f, 1/194.f, 1/193.f, 1/192.f, 1/191.f, 1/190.f,
						1/189.f, 1/188.f, 1/187.f, 1/186.f, 1/185.f, 1/184.f, 1/183.f, 1/182.f, 1/181.f, 1/180.f,
						1/179.f, 1/178.f, 1/177.f, 1/176.f, 1/175.f, 1/174.f, 1/173.f, 1/172.f, 1/171.f, 1/170.f,
						1/169.f, 1/168.f, 1/167.f, 1/166.f, 1/165.f, 1/164.f, 1/163.f, 1/162.f, 1/161.f, 1/160.f,
						1/159.f, 1/158.f, 1/157.f, 1/156.f, 1/155.f, 1/154.f, 1/153.f, 1/152.f, 1/151.f, 1/150.f,
						1/149.f, 1/148.f, 1/147.f, 1/146.f, 1/145.f, 1/144.f, 1/143.f, 1/142.f, 1/141.f, 1/140.f,
						1/139.f, 1/138.f, 1/137.f, 1/136.f, 1/135.f, 1/134.f, 1/133.f, 1/132.f, 1/131.f, 1/130.f,
						1/129.f, 1/128.f, 1/127.f, 1/126.f, 1/125.f, 1/124.f, 1/123.f, 1/122.f, 1/121.f, 1/120.f,
						1/119.f, 1/118.f, 1/117.f, 1/116.f, 1/115.f, 1/114.f, 1/113.f, 1/112.f, 1/111.f, 1/110.f,
						1/109.f, 1/108.f, 1/107.f, 1/106.f, 1/105.f, 1/104.f, 1/103.f, 1/102.f, 1/101.f, 1/100.f,

						1/099.f, 1/098.f, 1/097.f, 1/096.f, 1/095.f, 1/094.f, 1/093.f, 1/092.f, 1/091.f, 1/090.f,
						1/089.f, 1/088.f, 1/087.f, 1/086.f, 1/085.f, 1/084.f, 1/083.f, 1/082.f, 1/081.f, 1/080.f,
						1/079.f, 1/078.f, 1/077.f, 1/076.f, 1/075.f, 1/074.f, 1/073.f, 1/072.f, 1/071.f, 1/070.f,
						1/069.f, 1/068.f, 1/067.f, 1/066.f, 1/065.f, 1/064.f, 1/063.f, 1/062.f, 1/061.f, 1/060.f,
						1/059.f, 1/058.f, 1/057.f, 1/056.f, 1/055.f, 1/054.f, 1/053.f, 1/052.f, 1/051.f, 1/050.f,
						1/049.f, 1/048.f, 1/047.f, 1/046.f, 1/045.f, 1/044.f, 1/043.f, 1/042.f, 1/041.f, 1/040.f,
						1/039.f, 1/038.f, 1/037.f, 1/036.f, 1/035.f, 1/034.f, 1/033.f, 1/032.f, 1/031.f, 1/030.f,
						1/029.f, 1/028.f, 1/027.f, 1/026.f, 1/025.f, 1/024.f, 1/023.f, 1/022.f, 1/021.f, 1/020.f,
						1/019.f, 1/018.f, 1/017.f, 1/016.f, 1/015.f, 1/014.f, 1/013.f, 1/012.f, 1/011.f, 1/010.f,
						1/009.f, 1/008.f, 1/007.f, 1/006.f, 1/005.f, 1/004.f, 1/003.f, 1/002.f, 1/001.f, 0,
					};
					
					int nBlue  = pBgrColor[0];
					int nGreen = pBgrColor[1];
					int nRed   = pBgrColor[2];

					int nMin, nMax;
					
					double dHue = 240;
					int nMid = nRed - nGreen;

					// compute minimal and maximal components values
					if( nMid >= 0/*nRed > nGreen*/ )
					{
						if( nRed < nBlue )	
						{ 
							nMax = nBlue + nGreen;
							nMin = nBlue - nGreen;
						}
						else
						{ 
							nMid = nGreen - nBlue;
							if( nMid < 0/*nGreen < nBlue*/ )
							{
								nMax = nRed + nGreen;
								nMin = nRed - nGreen;
								dHue = 360;
							}
							else
							{
								nMax = nRed + nBlue;
								nMin = nRed - nBlue;
								dHue = 0;
							}
						}
					}
					else
					{
						if( nGreen < nBlue ) 
						{ 
							nMax = nBlue + nRed; 
							nMin = nBlue - nRed;
						}
						else
						{
							nMid = nBlue - nRed;
							if( nMid < 0/*nBlue < nRed*/ )
							{
								nMax = nGreen + nBlue;
								nMin = nGreen - nBlue;
								dHue = 120;
							}
							else
							{
								nMax = nGreen + nRed;
								nMin = nGreen - nRed;
								dHue = 120;
							}
						}
					}

					// compute lightness value
					pHlsColor[0] = nMid * c_fTable[nMin] * 60 + dHue;
					pHlsColor[1] = nMax * (1.0 / 510.0);
					pHlsColor[2] = nMin * c_fTable[nMax];

					//static const int table2[8][2] = {
					//	{  0,  60},
					//	{120, -60},
					//	{360, -60},
					//	{120,  60},
					//	{0, 0},
					//	{0, 0},
					//	{240,  60},
					//	{240, -60}
					//};

					//int color[3] = {pBgrColor[2], pBgrColor[1], pBgrColor[0]};
					//int index = 0;

					//if( color[0] < color[1] )
					//{
					//	int t = color[0]; color[0] = color[1]; color[1] = t;
					//	index |= 1;
					//}

					//if( color[1] < color[2] )
					//{
					//	int t = color[1]; color[1] = color[2]; color[2] = t;
					//	index |= 2;
					//}

					//if( color[0] < color[1] )
					//{
					//	int t = color[0]; color[0] = color[1]; color[1] = t;
					//	index |= 4;
					//}

					//int max = color[0];
					//int mid = color[1];
					//int min = color[2];

					//pHlsColor[2] = c_fTable[max + min] * (max - min);
					//pHlsColor[1] = (max + min) * (1.0 / 510);
					//pHlsColor[0] = c_fTable[max - min] * (mid - min) * table2[index][1] + table2[index][0];
				}

				void HlsToBgr( const double* pHlsColor, BYTE* pBgrColor )
				{
					double dLightness  = pHlsColor[1] * 255;
					double dSaturation = pHlsColor[2];

					// check achromatic case
					if( dSaturation <= IPP_EPS23 )
					{
						int c = RealToInt(dLightness);
						pBgrColor[0] = c;
						pBgrColor[1] = c;
						pBgrColor[2] = c;
						return;
					}

					dSaturation *= 127.5 - abs(dLightness - 127.5);

					double dHue = pHlsColor[0] * (1.0/30.0);

					int i = (int)(dHue) >> 1;
					switch( i )
					{
					case 0:  pBgrColor[0] = RealToInt( dLightness - dSaturation); pBgrColor[2] = RealToInt( dLightness + dSaturation); pBgrColor[1] = RealToInt( dLightness + dSaturation * (dHue - 1)); break;
					case 1:  pBgrColor[0] = RealToInt( dLightness - dSaturation); pBgrColor[1] = RealToInt( dLightness + dSaturation); pBgrColor[2] = RealToInt( dLightness + dSaturation * (3 - dHue)); break;
					case 2:  pBgrColor[2] = RealToInt( dLightness - dSaturation); pBgrColor[1] = RealToInt( dLightness + dSaturation); pBgrColor[0] = RealToInt( dLightness + dSaturation * (dHue - 5)); break;
					case 3:  pBgrColor[2] = RealToInt( dLightness - dSaturation); pBgrColor[0] = RealToInt( dLightness + dSaturation); pBgrColor[1] = RealToInt( dLightness + dSaturation * (7 - dHue)); break;
					case 4:  pBgrColor[1] = RealToInt( dLightness - dSaturation); pBgrColor[0] = RealToInt( dLightness + dSaturation); pBgrColor[2] = RealToInt( dLightness + dSaturation * (dHue - 9)); break;
					default: pBgrColor[1] = RealToInt( dLightness - dSaturation); pBgrColor[2] = RealToInt( dLightness + dSaturation); pBgrColor[0] = RealToInt( dLightness + dSaturation * (11 - dHue)); break;
					}

					//// green
					//double dGreen = dLightness;
					//if      (dHue < 2) dGreen += dSaturation * (dHue - 1);
					//else if (dHue < 6) dGreen += dSaturation;
					//else if (dHue < 8) dGreen += dSaturation * (7 - dHue);
					//else               dGreen -= dSaturation;

					//// red
					//dHue += 4;
					//if( dHue >= 12 )
					//{
					//	dHue -= 12;
					//}

					//double dRed = dLightness;
					//if      (dHue < 2) dRed += dSaturation * (dHue - 1);
					//else if (dHue < 6) dRed += dSaturation;
					//else if (dHue < 8) dRed += dSaturation * (7 - dHue);
					//else               dRed -= dSaturation;

					//// blue
					//dHue -= 8;
					//if( dHue < 0 )
					//{
					//	dHue += 12;
					//}

					//double dBlue = dLightness;
					//if      (dHue < 2) dBlue += dSaturation * (dHue - 1);
					//else if (dHue < 6) dBlue += dSaturation;
					//else if (dHue < 8) dBlue += dSaturation * (7 - dHue);
					//else               dBlue -= dSaturation;


					//pBgrColor[0] = RealToInt( dBlue );
					//pBgrColor[1] = RealToInt( dGreen );
					//pBgrColor[2] = RealToInt( dRed );
				}

				
				void ClearImage( BYTE* pPixels, int nWidth, int nHeight, int nStride, int nColor, int alpha )
				{
					Ipp8u arrColors[4] = {
						GetBValue(nColor),
						GetGValue(nColor),
						GetRValue(nColor),
						IntToByte(alpha)
					};

					IppiSize oImageSize = {nWidth, nHeight};

					ippiSet_8u_C4R( arrColors, pPixels, nStride, oImageSize );
				}
#ifndef   IMAGE_EDITOR				
				int BGRA_GetPixelColor(BYTE* pBGRA, int nWidth, int nHeight, double dPixelX, double dPixelY, BOOL bBillinear, void* pPixelBGRA)
				{
					/* @desc: 
					int BGRA_GetPixelColor(BYTE* pBGRA, int nWidth, int nHeight, double dPixelX, double dPixelY, BOOL bBillinear, void* pPixelBGRA)
					{
						вычисляет цвет пикселя в исходной картинке (pBGRA, nWidth, nHeight)
						в точке с координатами (dPixelX, dPixelY)
						с учетом интерполяции (bBillinear) или без неё
						результат вычислений складывает в заранее отведенный массив (pPixelBGRA)
					}
					*/

					int nX = (int)floor(dPixelX);
					int nY = (int)floor(dPixelY);

					if (nX < 0 || nX >= nWidth || nY < 0 || nY >= nHeight)
						return c_nErrorPixelNotFound;

					BYTE* pPixelResult = (BYTE*)pPixelBGRA;

					if (!bBillinear)
					{
						BYTE* pPixel = pBGRA + 4*(nX + nY*nWidth);

						for (int index = 0; index < 4; ++index)
						{
							*pPixelResult = *pPixel;
							
							pPixel++;
							pPixelResult++;
						}
					}
					else
					{
						dPixelX = max(0, dPixelX - 0.5);
						dPixelY = max(0, dPixelY - 0.5);

						int nX = (int)floor(dPixelX);
						int nY = (int)floor(dPixelY);

						int nOffsetR = 4;
						int nOffsetB = 4*nWidth;

						if (nX == nWidth - 1)
						{ 
							dPixelX = nX; 
							nOffsetR = 0;
						}
						if (nY == nHeight - 1)
						{
							dPixelY = nY;
							nOffsetB = 0;
						}

						BYTE* pPixelLT = pBGRA + 4*(nX + nY*nWidth);
						BYTE* pPixelRT = pPixelLT + nOffsetR;
						BYTE* pPixelRB = pPixelRT + nOffsetB;
						BYTE* pPixelLB = pPixelLT + nOffsetB;

						double dX = dPixelX - nX;
						double dY = dPixelY - nY;

						double dLT = (1 - dX)*(1 - dY);
						double dRT = dX*(1 - dY);
						double dRB = dX*dY;
						double dLB = (1 - dX)*dY;

						for (int index = 0; index < 4; ++index)
						{
							*pPixelResult = (BYTE)(dLT*(*pPixelLT) + dRT*(*pPixelRT) + dRB*(*pPixelRB) + dLB*(*pPixelLB));
							
							pPixelLT++;
							pPixelRT++;
							pPixelRB++;
							pPixelLB++;
							pPixelResult++;
						}
					}

					return c_nErrorNone;
				}

				int BGRA_CreateGdiplusImage(BYTE* pBGRA, int nWidth, int nHeight, Gdiplus::Bitmap*& pBitmap)
				{
					/* @desc: 
					int BGRA_CreateGdiplusImage(BYTE* pBGRA, int nWidth, int nHeight, Gdiplus::Bitmap*& pBitmap)
					{
						для заданной BGRA32-картинки (pBGRA, nWidth, nHeight) создаются (Gdiplus::Bitmap* pBitmap)
						для дальнейших отрисовок и прочих gdiplus манипуляций
					}
					*/

					if( pBitmap )
					{
						delete pBitmap;
						pBitmap = NULL;
					}

					if (!pBGRA || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					pBitmap = new Gdiplus::Bitmap(nWidth, nHeight, 4*nWidth, PixelFormat32bppARGB, pBGRA);
					if (NULL == pBitmap)
						return c_nErrorGdiplusFailure;

					return c_nErrorNone;
				}
				int BGRA_CreateGdiplusGraphics(BYTE* pBGRA, int nWidth, int nHeight, Gdiplus::Bitmap*& pBitmap, Gdiplus::Graphics*& pGraphics)
				{
					/* @desc: 
					int BGRA_CreateGdiplusGraphics(BYTE* pBGRA, int nWidth, int nHeight, Gdiplus::Bitmap*& pBitmap, Gdiplus::Graphics*& pGraphics)
					{
						для заданной BGRA32-картинки (pBGRA, nWidth, nHeight) создаются (Gdiplus::Bitmap* pBitmap) и (Gdiplus::Graphics* pGraphics) объекты
						для дальнейших отрисовок и прочих gdiplus манипуляций
					}
					*/

					if( pGraphics )
					{
						delete pGraphics;
						pGraphics = NULL;
					}
					if( pBitmap )
					{
						delete pBitmap;
						pBitmap = NULL;
					}

					int nResult = BGRA_CreateGdiplusImage(pBGRA, nWidth, nHeight, pBitmap);
					if (c_nErrorNone != nResult)
						return nResult;

					pGraphics = Gdiplus::Graphics::FromImage(pBitmap);
					if (NULL == pGraphics)
					{
						delete pBitmap;
						pBitmap = NULL;

						return c_nErrorGdiplusFailure;
					}

					return c_nErrorNone;
				}
				int BGRA_CreatePartialSums(BYTE* pBGRA, int nWidth, int nHeight, int*& pSums)
				{
					/* @desc: 
					int BGRA_CreatePartialSums(BYTE* pBGRA, int nWidth, int nHeight, int*& pSums)
					{
						для картинки (pBGRA, nWidth, nHeight) считается матрица сумм (от левого верхнего угла)
						результат сумм для цветовых плоскостей записывается в указатель (pSums*)
					}
					*/

					if (!pBGRA || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					int* pBuffer = new int[nWidth * nHeight * 4];
					if( !pBuffer )
						return c_nErrorNoMemory;

					pSums = pBuffer;

					int nSumB = 0;
					int nSumG = 0;
					int nSumR = 0;
					int nSumA = 0;

					for( int nX = nWidth; nX > 0; --nX, pBuffer += 4, pBGRA += 4 )
					{
						nSumB += pBGRA[0];
						nSumG += pBGRA[1];
						nSumR += pBGRA[2];
						nSumA += pBGRA[3];

						pBuffer[0] = nSumB;
						pBuffer[1] = nSumG;
						pBuffer[2] = nSumR;
						pBuffer[3] = nSumA;
					}

					int nStep = nWidth * 4;

					for( int nY = nHeight - 1; nY > 0; --nY )
					{
						nSumB = nSumG = nSumR = nSumA = 0;

						for( int nX = nWidth; nX > 0; --nX, pBuffer += 4, pBGRA += 4 )
						{
							nSumB += pBGRA[0];
							nSumG += pBGRA[1];
							nSumR += pBGRA[2];
							nSumA += pBGRA[3];

							pBuffer[0] = pBuffer[0 - nStep] + nSumB;
							pBuffer[1] = pBuffer[1 - nStep] + nSumG;
							pBuffer[2] = pBuffer[2 - nStep] + nSumR;
							pBuffer[3] = pBuffer[3 - nStep] + nSumA;
						}
					}

					return c_nErrorNone;
				}

#endif
				typedef bool (*BgraInterpolationFunc)(BYTE*, const BYTE*, int, int, int, double, double);

				bool BGRA_Nearest( BYTE* pResult, const BYTE* pSource, int nWidth, int nHeight, int nStride, double dX, double dY )
				{
					int nX = int(dX + 1.5);
					if( nX <= 0 )	  return false;
					if( nX > nWidth ) return false;

					int nY = int(dY + 1.5);
					if( nY <= 0 )	  return false;
					if( nY > nHeight) return false;

					*(PDWORD(pResult)) = *(PDWORD(pSource + (nY - 1) * nStride + (nX - 1) * 4));
					
					return true;
				}
				bool BGRA_Billinear( BYTE* pResult, const BYTE* pSource, int nWidth, int nHeight, int nStride, double dX, double dY )
				{
					const DWORD c_nMaxFactor = 65536;

					int nX = int(dX);
					if( nX < 0 || nX >= nWidth )
						return false;

					int nY = int(dY);
					if( nY < 0 || nY >= nHeight ) 
						return false;

					int nFactorX = int((dX - nX) * c_nMaxFactor);
					int nFactorY = int((dY - nY) * c_nMaxFactor);

					const BYTE* pPixels[4];

					pPixels[0] = pSource + nY * nStride + nX * 4;
					pPixels[1] = pPixels[0] + 4;
					pPixels[2] = pPixels[0] + nStride;
					pPixels[3] = pPixels[2] + 4;

					if( nFactorX < 0 )
					{
						nFactorX   = -nFactorX;
						pPixels[1] = pResult;
						pPixels[3] = pResult;
					}
					if( nFactorY < 0 )
					{
						nFactorY   = -nFactorY;
						pPixels[2] = pResult;
						pPixels[3] = pResult;
					}
					if( nX == nWidth - 1 )
					{
						pPixels[1] = pResult;
						pPixels[3] = pResult;
					}
					if( nY == nHeight - 1 )
					{
						pPixels[2] = pResult;
						pPixels[3] = pResult;
					}

					for( int i = 0; i < 4; i++ )
					{
						int c1 = AlphaCompEx( pPixels[0][i], pPixels[1][i], nFactorX, c_nMaxFactor );
						int c2 = AlphaCompEx( pPixels[2][i], pPixels[3][i], nFactorX, c_nMaxFactor );
						pResult[i] = AlphaCompEx( c1, c2, nFactorY, c_nMaxFactor );
					}

					return true;
				}
				
				int BGRA_RemoveColorFromImage(BYTE* pBGRA, int nWidth, int nHeight, int nColor)
				{
					/* @desc: 
					int BGRA_RemoveColorFromImage(BYTE* pBGRA, int nWidth, int nHeight, int nColor)
					{
						для всех пикселов картинки (pBGRA, nWidth, nHeight) с заданным цветом (nColor) 
						выставляем цвет прозрачности в 0, исходная прозрачность пикселя при этом игнорируется
					}
					*/

					if (!pBGRA || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					// если нечего удалять - ничего не делаем
					if (nColor < 0)
						return c_nErrorNone;

					DWORD dwRemoveColor = DWORD(GetBValue(nColor)) | (DWORD(GetGValue(nColor)) << 8) | (DWORD(GetRValue(nColor)) << 16);
					DWORD* pPixel = (DWORD*)pBGRA;

					// проверяем все пикселы
					for( int count = nWidth*nHeight; count > 0; --count, ++pPixel )
					{
						DWORD dwCurColor = *pPixel & 0x00ffffff;

						if( dwCurColor != dwRemoveColor )
							continue;
						
						*pPixel = dwCurColor;
					}

					return c_nErrorNone;
				}
				int BGRA_Rotate( BYTE* pSourceBGRA, int nSourceWidth, int nSourceHeight, int nSourceStride, BYTE* pResultBGRA, int nResultWidth, int nResultHeight, int nResultStride, double dResultCenterX, double dResultCenterY, double dAngleInDegrees, double dSourceScaleX, double dSourceScaleY, DWORD nAlpha, BOOL bInterpolate )
				{
					/* @desc: 
					int BGRA_Rotate(BYTE* pSourceBGRA, int nSourceWidth, int nSourceHeight, BYTE* pResultBGRA, int nResultWidth, int nResultHeight, double dResultCenterX, double dResultCenterY, double dAngleInDegrees, double dSourceScaleX, double dSourceScaleY, BOOL bInterpolate)
					{
						осуществляется поворот source-картинки (pSourceBGRA, nSourceWidth, nSourceHeight) 
						на заданный угол (dAngleInDegrees) относительно точки (dResultCenterX, dResultCenterY) в result-картинке
						с учетом дополнительного коэффициента сжатия (dSourceScale) и интерполяции (bIbterpolate)
						результат поворота записывается в уже созданную result-картинку (pResultBGRA, nResultWidth, nResultHeight)

						алгоритм работы такой: пробегаемся по result-картинке и, выполняя обратное преобразование, 
						ищем соответствующий ему цвет в source-картинке

						этот способ медленный, зато универсальный и качественный
						(ipp работает не стабильно - проверено неоднократно)
					}
					*/

					if (!pSourceBGRA || !pResultBGRA || nSourceWidth <= 0 || nSourceHeight <= 0 || nResultWidth <= 0 || nResultHeight <= 0)
						return c_nErrorInvalidSourceImage;

					if( abs( dSourceScaleX ) < 0.0001 || abs( dSourceScaleY ) < 0.0001 )
						return c_nErrorNone;

					if( nAlpha > 255 )
						nAlpha = 255;

					if( nSourceStride < 0 )
						pSourceBGRA -= (nSourceHeight - 1) * nSourceStride;

					if( nResultStride < 0 )
						pResultBGRA -= (nResultHeight - 1) * nResultStride;

					// Реализация через Gdiplus - присутствует зубчатость на границах при повороте
					Gdiplus::Bitmap oResultImage( nResultWidth, nResultHeight, nResultStride, PixelFormat32bppARGB, pResultBGRA );
					if( Gdiplus::Ok == oResultImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oResultImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							oRender.SetSmoothingMode( Gdiplus::SmoothingModeAntiAlias );
							oRender.SetInterpolationMode( bInterpolate ? Gdiplus::InterpolationModeBilinear : Gdiplus::InterpolationModeNearestNeighbor );

							oRender.TranslateTransform( float(nResultWidth / -2.0f), float(nResultHeight / -2.0f), MatrixOrderAppend );
							oRender.RotateTransform( float(-dAngleInDegrees), MatrixOrderAppend );
							oRender.ScaleTransform( float(dSourceScaleX), float(dSourceScaleY), MatrixOrderAppend );
							oRender.TranslateTransform( float(dResultCenterX), float(dResultCenterY), MatrixOrderAppend );
							
							Gdiplus::Bitmap oSourceImage( nSourceWidth, nSourceHeight, nSourceStride, PixelFormat32bppARGB, pSourceBGRA );
							if( Gdiplus::Ok == oSourceImage.GetLastStatus() )
							{
								Gdiplus::RectF oDstRect( 
									float((nResultWidth  - nSourceWidth)  / 2.0f),
									float((nResultHeight - nSourceHeight) / 2.0f),
									float(nSourceWidth),
									float(nSourceHeight)
								);

								oRender.DrawImage( &oSourceImage, oDstRect, 0, 0, float(nSourceWidth), float(nSourceHeight), UnitPixel );

								return c_nErrorNone;
							}
						}
					}

					// расчитываем рабочую область на result
					
					//dAngleInDegrees *= -IPP_PI180;
					//double dSin = sin( dAngleInDegrees );
					//double dCos = cos( dAngleInDegrees );

					//double dSourceCenterX = (nSourceWidth - 1)  / 2.0;
					//double dSourceCenterY = (nSourceHeight - 1) / 2.0;

					//int left   = 0;
					//int top    = 0;
					//int right  = 0;
					//int bottom = 0;

					//if( TRUE )
					//{
					//	double dLeft   = dResultCenterX;
					//	double dRight  = dResultCenterX;
					//	double dTop    = dResultCenterY;
					//	double dBottom = dResultCenterY;

					//	double dPX[4] = {0, nSourceWidth - 1, nSourceWidth - 1, 0};
					//	double dPY[4] = {0, 0, nSourceHeight - 1, nSourceHeight - 1};

					//	for( int i = 0; i < 4; i++ )
					//	{
					//		double dX = dPX[i] - dSourceCenterX;
					//		double dY = dPY[i] - dSourceCenterY;

					//		dPX[i] = (dY * dSin + dX * dCos) * dSourceScaleX + dResultCenterX;
					//		dPY[i] = (dY * dCos - dX * dSin) * dSourceScaleY + dResultCenterY;

					//		if( dPX[i] < dLeft )   dLeft   = dPX[i];
					//		else
					//		if( dPX[i] > dRight )  dRight  = dPX[i];

					//		if( dPY[i] < dTop )    dTop    = dPY[i];
					//		else
					//		if( dPY[i] > dBottom ) dBottom = dPY[i];
					//	}

					//	left   = int(dLeft)   - 1;
					//	right  = int(dRight)  + 1;
					//	top    = int(dTop)    - 1;
					//	bottom = int(dBottom) + 1;

					//	if( left   >= nResultWidth )  return c_nErrorNone;
					//	if( right  <  0 )             return c_nErrorNone;
					//	if( top    >= nResultHeight ) return c_nErrorNone;
					//	if( bottom <  0 )             return c_nErrorNone;

					//	if( left   <  0 )             left   = 0;
					//	if( right  >= nResultWidth )  right  = nResultWidth - 1;
					//	if( top    <  0 )             top    = 0;
					//	if( bottom >= nResultHeight ) bottom = nResultHeight - 1;
					//}

					//double dScaleFactorX = 1 / dSourceScaleX;
					//double dScaleFactorY = 1 / dSourceScaleY;

					//BgraInterpolationFunc pInterFunc = bInterpolate ? BGRA_Billinear : BGRA_Nearest;

					//// пробегаем по всем пикселям result-картинки
					//BYTE* pResult = pResultBGRA + (top * nResultWidth + left) * 4;
					//int   nStride = (nResultWidth - (right - left + 1)) * 4;

					//dResultCenterY = top  - dResultCenterY;
					//dResultCenterX = left - dResultCenterX;
					//
					//top  = bottom - top  + 1;
					//left = right  - left + 1;

					//double dNewY = dResultCenterY;
					//for( bottom = top; bottom > 0; dNewY += 1, --bottom, pResult += nStride )
					//{
					//	double dNewX = dResultCenterX;
					//	for( right = left; right > 0; dNewX += 1, --right, pResult += 4 )
					//	{
					//		double dSourceX = (dNewX * dCos - dNewY * dSin) * dScaleFactorX + dSourceCenterX;
					//		double dSourceY = (dNewX * dSin + dNewY * dCos) * dScaleFactorY + dSourceCenterY;

					//		BYTE pPixelColor[4];
					//		*((DWORD*)pPixelColor) = *((DWORD*)pResult);
					//		
					//		if( pInterFunc( pPixelColor, pSourceBGRA, nSourceWidth, nSourceHeight, nSourceWidth * 4, dSourceX, dSourceY ) )
					//		{
					//			// alpha-blend двух пикселов - исходного и результирующего
					//			int alpha = AlphaComp( 0, pPixelColor[3], nAlpha );
					//			pResult[0] = AlphaComp( pResult[0], pPixelColor[0], alpha );
					//			pResult[1] = AlphaComp( pResult[1], pPixelColor[1], alpha );
					//			pResult[2] = AlphaComp( pResult[2], pPixelColor[2], alpha );
					//			pResult[3] = AlphaComp( pResult[3], 255, alpha );
					//		}
					//	}
					//}

					return c_nErrorNone;
				}
				int BGRA_RotateSimple( BYTE* pBGRA, int nWidth, int nHeight, int nStride, int eType)
				{
					// eType: 1 - 90, 2 - 180, 3 - 270

					if (NULL == pBGRA)
						return c_nErrorInvalidSourceImage;

					if( eType != 1 && eType != 2 && eType != 3 )
						return c_nErrorNone;

					DWORD* pBuffer = new DWORD[nWidth * nHeight];
					if( !pBuffer )
						return c_nErrorNoMemory;

					::memcpy( pBuffer, pBGRA, nWidth * nHeight * 4 );

					DWORD* pResult = (DWORD*)pBGRA;

					if( nStride < 0 )
					{
						if( eType == 3 ) eType = 1;
						else if( eType == 1 ) eType = 3;
					}

					if( 1 == eType ) // 90
					{
						for( int nY = 0; nY < nWidth; ++nY )
						{
							for( int nX = 0; nX < nHeight; ++nX, ++pResult )
							{
								pResult[0] = pBuffer[nX * nWidth + nWidth - nY - 1];
							}
						}
					}
					else
					if( 2 == eType ) // 180
					{
						for( int nY = 0; nY < nHeight; ++nY )
						{
							for( int nX = 0; nX < nWidth; ++nX, ++pResult )
							{
								pResult[0] = pBuffer[(nHeight - nY - 1) * nWidth + nWidth - nX - 1];
							}
						}
					}
					else
					if( 3 == eType ) // 270
					{
						for( int nY = 0; nY < nWidth; ++nY )
						{
							for( int nX = 0; nX < nHeight; ++nX, ++pResult )
							{
								pResult[0] = pBuffer[(nHeight - nX - 1) * nWidth + nY];
							}
						}
					}


					delete [] pBuffer;

					return c_nErrorNone;
				}

				int BGRA_Flip(BYTE* pBGRA, int nWidth, int nHeight)
				{
					/* @desc: 
					int BGRA_Flip(BYTE* pBGRA, int nWidth, int nHeight)
					{
						осуществляется вертикальное отражение картинки (pBGRA, nWidth, nHeight) 
					}
					*/

					if (NULL == pBGRA)
						return c_nErrorInvalidSourceImage;

					DWORD* pTop    = PDWORD(pBGRA);
					DWORD* pBottom = PDWORD(pBGRA + (nHeight - 1) * nWidth);

					for( int nRow = nHeight / 2; nRow > 0; --nRow, pBottom -= nWidth * 2 )
					{
						for( int nPos = nWidth; nPos > 0; --nPos, pTop += 1, pBottom += 1 )
						{
							DWORD pixel = *pTop;
							*pTop    = *pBottom;
							*pBottom = pixel;
						}
					}

					return c_nErrorNone;
				}
#ifndef   IMAGE_EDITOR
				int BGRA_FilterAverage(BYTE* pBGRA, int nWidth, int nHeight, int nSizeX, int nSizeY)
				{
					/* @desc: 
					int BGRA_FilterAverage(BYTE* pBGRA, int nWidth, int nHeight, int nSizeX, int nSizeY)
					{
						к картинке (pBGRA, nWidth, nHeight) применяется преобразование усреднения 
						пикселов с размером матрицы (2*nSizeX + 1, 2*nSizeY + 1)
					}
					*/

					if (!pBGRA || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					if (nSizeX < 1 && nSizeY < 1)
						return c_nErrorNone;

					int* pSums = NULL;

					int nResult = BGRA_CreatePartialSums(pBGRA, nWidth, nHeight, pSums);

					if (c_nErrorNone != nResult)
						return nResult;

					// используя частичные суммы подсчитываем среднее арифметическое для каждого пикселя картинки
					for (int nY = 0; nY < nHeight; ++nY)
					{
						int nTop = max(0, nY - nSizeY) - 1;
						int nBottom = min(nHeight - 1, nY + nSizeY);

						for (int nX = 0; nX < nWidth; ++nX)
						{
							int nLeft = max(0, nX - nSizeX) - 1;
							int nRight = min(nWidth - 1, nX + nSizeX);

							int nCellSize = (nRight - nLeft)*(nBottom - nTop);

							for (int nChannel = 0; nChannel < 4; ++nChannel)
							{
								int nSum = pSums[4*(nRight + nBottom*nWidth) + nChannel];

								if (nLeft >= 0)
									nSum -= pSums[4*(nLeft + nBottom*nWidth) + nChannel];
								
								if (nLeft >= 0 && nTop >= 0)
									nSum += pSums[4*(nLeft + nTop*nWidth) + nChannel];
								
								if (nTop >= 0)
									nSum -= pSums[4*(nRight + nTop*nWidth) + nChannel];

								pBGRA[4*(nX + nY*nWidth) + nChannel] = (BYTE)(nSum/nCellSize);
							}
						}
					}

					// удаляем ненужную память
					delete[] pSums;

					return c_nErrorNone;
				}
				int BGRA_Fog( BYTE* pPixels, int nWidth, int nHeight, int nColor, double dTransparency )
				{
					BYTE TableB[256];
					BYTE TableG[256];
					BYTE TableR[256];

					double R = GetRValue(nColor) / 255.0;
					double G = GetGValue(nColor) / 255.0;
					double B = GetBValue(nColor) / 255.0;

					R = 1 - (1 - R) * dTransparency;
					G = 1 - (1 - G) * dTransparency;
					B = 1 - (1 - B) * dTransparency;

					for( int i = 0; i < 256; i++ )
					{
						TableB[i] = RealToByte( i * B );
						TableG[i] = RealToByte( i * G );
						TableR[i] = RealToByte( i * R );
					}

					BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;
					for( ; pPixels != pMaxPtr; pPixels += 4 )
					{
						pPixels[0] = TableB[pPixels[0]];
						pPixels[1] = TableG[pPixels[1]];
						pPixels[2] = TableR[pPixels[2]];
					}
					
					return c_nErrorNone;
				}
				
				int BGRA_DrawMovObject(BYTE* pBGRA, int nWidth, int nHeight, int nColor, int nAlpha, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, ImageStudio::Paint::Compose::CAlphaComposer* pComposer)
				{
					/* @desc: 
					int BGRA_DrawMovObject(BYTE* pBGRA, int nWidth, int nHeight, int nColor, int nAlpha, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, ImageStudio::Paint::Compose::CAlphaComposer* pComposer)
					{
						к картинке (pBGRA, nWidth, nHeight) применяется преобразование AlphaComposition - фигурное смешение в цветом (nColor) и прозрачностью (nAlpha)

						nColor - COLORREF цвет, с которым будет происходить смешение
						nAlpha - прозрачность, с которой будет происходить смешение
						dCompleteness - уровень смешения цветов; изменяется от 0 (нет смешения) до 1 (полный AlphaBlend)
						nType - тип смешения; см. ImageStudio::Paint::Compose::Constants константы

						pComposer - композер цветов, выполняющий смешение
					}
					*/

					if (!pBGRA || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					// проверяем простейшие случаи, когда эффект либо не применяется вообще, либо применяется полностью
					if (nAlpha == 0 || dCompleteness < 0.0001)
						return c_nErrorNone;
					
					double dAlpha = IntToByte( nAlpha ) / 255.0;

					if (dCompleteness >= 0.999 || nWidth < 2 || nHeight < 2)
						return BGRA_Fog( pBGRA, nWidth, nHeight, nColor, dAlpha );

					if (NULL != pComposer)
					{
						if (pComposer->Transform(pBGRA, nWidth, nHeight, nColor, nAlpha, nTileSizeX, nTileSizeY, nType, dCompleteness))
							return c_nErrorNone;
					}
					else
					{
						ImageStudio::Paint::Compose::CAlphaComposer oComposer;

						if (oComposer.Transform(pBGRA, nWidth, nHeight, nColor, nAlpha, nTileSizeX, nTileSizeY, nType, dCompleteness))
							return c_nErrorNone;
					}

					return BGRA_Fog( pBGRA, nWidth, nHeight, nColor, dAlpha * dCompleteness );
				}
				int BGRA_ImageComposition(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, ImageStudio::Paint::Compose::CImageComposer* pComposer)
				{
					/* @desc: 
					int BGRA_ImageComposition(BYTE* pBGRASource1, BYTE* pBGRASource2, BYTE* pBGRAResult, int nWidth, int nHeight, int nTileSizeX, int nTileSizeY, int nType, double dCompleteness, ImageStudio::Paint::Compose::CImageComposer* pComposer)
					{
						к картинкам (pBGRASource1, pBGRASource2, nWidth, nHeight) применяется преобразование ImageComposition - фигурное смешение одной картинки с другой
						результат смешения записывается в картинку (pBGRAResult, nWidth, nHeight)

						предполагается, что все три картинки имеют одинаковый размер (nWidth, nHeight)

						dCompleteness - уровень смешения картинок; изменяется от 0 (нет смешения, pBGRAResult содержит pBGRASource1) до 1 (полное смешение, pBGRAResult содержит pBGRASource2)
						nType - тип смешения; см. ImageStudio::Paint::Compose::Constants константы

						pComposer - композер изоражений, выполняющий смешение
					}
					*/

					if (!pBGRASource1 || !pBGRASource2 || !pBGRAResult || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					// проверяем простейшие случаи
					if (dCompleteness < 0.0001)
					{
						memcpy(pBGRAResult, pBGRASource1, 4*nWidth*nHeight);
						return c_nErrorNone;
					}
					else if (dCompleteness >= 0.999)
					{
						memcpy(pBGRAResult, pBGRASource2, 4*nWidth*nHeight);
						return c_nErrorNone;
					}

					if (NULL != pComposer)
					{
						if (pComposer->Transform(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness))
							return c_nErrorNone;
					}
					else
					{
						ImageStudio::Paint::Compose::CImageComposer oComposer;

						if (oComposer.Transform(pBGRASource1, pBGRASource2, pBGRAResult, nWidth, nHeight, nTileSizeX, nTileSizeY, nType, dCompleteness))
							return c_nErrorNone;
					}

					return c_nErrorImageCopositionFailure;
				}
				
				int BGRA_DrawLine(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dX1, double dY1, double dX2, double dY2)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_DrawLine( &oRender, oSettings, dX1, dY1, dX2, dY2);
						}
					}

					return nResult;
				}
				int BGRA_DrawRectangle(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_DrawRectangle( &oRender, oSettings, dLeft, dTop, dRight, dBottom);
						}
					}

					return nResult;
				}
				int BGRA_FillRectangle(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_FillRectangle( &oRender, oSettings, dLeft, dTop, dRight, dBottom);
						}
					}

					return nResult;
				}
				int BGRA_DrawEllipse(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_DrawEllipse( &oRender, oSettings, dLeft, dTop, dRight, dBottom);
						}
					}

					return nResult;
				}
				int BGRA_FillEllipse(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_FillEllipse( &oRender, oSettings, dLeft, dTop, dRight, dBottom);
						}
					}

					return nResult;
				}
				int BGRA_FillInverseRectangle(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// подсчитываем "объемлющий" прямоугольник
							Gdiplus::RectF oRectImage(-1.0f, -1.0f, (float)nWidth + 2.0f, (float)nHeight + 2.0f); // чтобы не было полос от антиалиасинга - делаем rect чуть больше картинки

							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_FillInverseRectangle( &oRender, oSettings, oRectImage, dLeft, dTop, dRight, dBottom);
						}
					}

					return nResult;
				}
				int BGRA_FillInverseEllipse(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// подсчитываем "объемлющий" прямоугольник
							Gdiplus::RectF oRectImage(-1.0f, -1.0f, (float)nWidth + 2.0f, (float)nHeight + 2.0f); // чтобы не было полос от антиалиасинга - делаем rect чуть больше картинки

							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_FillInverseEllipse( &oRender, oSettings, oRectImage, dLeft, dTop, dRight, dBottom);
						}
					}

					return nResult;
				}
				int BGRA_DrawPolyline(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, Gdiplus::PointF* pPoints, int nCount)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_DrawPolyline( &oRender, oSettings, pPoints, nCount);
						}
					}

					return nResult;
				}
				int BGRA_DrawPolygon(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, Gdiplus::PointF* pPoints, int nCount)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_DrawPolygon( &oRender, oSettings, pPoints, nCount);
						}
					}

					return nResult;
				}
				int BGRA_FillPolygon(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, Gdiplus::PointF* pPoints, int nCount)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_FillPolygon( &oRender, oSettings, pPoints, nCount);
						}
					}

					return nResult;
				}
				int BGRA_DrawPie(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_DrawPie( &oRender, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
						}
					}

					return nResult;
				}
				int BGRA_FillPie(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_FillPie( &oRender, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
						}
					}

					return nResult;
				}
				int BGRA_DrawSector(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle, double dRatio)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_DrawSector( &oRender, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle, dRatio);
						}
					}

					return nResult;
				}
				int BGRA_FillSector(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle, double dRatio)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_FillSector( &oRender, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle, dRatio);
						}
					}

					return nResult;
				}
				int BGRA_DrawSimplePie(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius;
					double dTop = dCenterY - dRadius;
					double dRight = dCenterX + dRadius;
					double dBottom = dCenterY + dRadius;

					return BGRA_DrawPie(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
				}
				int BGRA_FillSimplePie(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius;
					double dTop = dCenterY - dRadius;
					double dRight = dCenterX + dRadius;
					double dBottom = dCenterY + dRadius;

					return BGRA_FillPie(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
				}
				int BGRA_DrawSimpleSector(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius1, double dRadius2, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius1;
					double dTop = dCenterY - dRadius1;
					double dRight = dCenterX + dRadius1;
					double dBottom = dCenterY + dRadius1;

					return BGRA_DrawSector(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle, dRadius2/max(0.0001, dRadius1));
				}
				int BGRA_FillSimpleSector(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dCenterX, double dCenterY, double dRadius1, double dRadius2, double dStartAngle, double dSweepAngle)
				{
					double dLeft = dCenterX - dRadius1;
					double dTop = dCenterY - dRadius1;
					double dRight = dCenterX + dRadius1;
					double dBottom = dCenterY + dRadius1;

					return BGRA_FillSector(pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle, dRadius2/max(0.0001, dRadius1));
				}
				int BGRA_DrawBorder(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, double dLeft, double dTop, double dRight, double dBottom, BOOL bLinearGradient )
				{
					if( NULL == pBGRA )
						return c_nErrorInvalidPointer;

					BYTE* pBuffer = new BYTE[nWidth*nHeight*4];
					if( NULL == pBuffer )
						return c_nErrorNoMemory;

					::memcpy( pBuffer, pBGRA, nWidth*nHeight*4 );

					if( FALSE == bLinearGradient )
					{
						int nResult = BGRA::BGRA_FillInverseRectangle( pBGRA, nWidth, nHeight, nStride, oSettings, dLeft, dTop, dRight, dBottom );
						if( c_nErrorNone != nResult )
						{
							delete [] pBuffer;
							return nResult;
						}
					}

					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							if( bLinearGradient )
							{
								Gdiplus::Color oColorExt(GetRValue(oSettings.GetBrush().GetColor1()), GetGValue(oSettings.GetBrush().GetColor1()), GetBValue(oSettings.GetBrush().GetColor1()));
								Gdiplus::Color oColorInt(GetRValue(oSettings.GetBrush().GetColor2()), GetGValue(oSettings.GetBrush().GetColor2()), GetBValue(oSettings.GetBrush().GetColor2()));

								Gdiplus::LinearGradientBrush oBrushTop   ( Gdiplus::PointF(0, 0),              Gdiplus::PointF(0, float(dTop)),        oColorExt, oColorInt);
								Gdiplus::LinearGradientBrush oBrushBottom( Gdiplus::PointF(0, float(dBottom)), Gdiplus::PointF(0, float(nHeight + 1)), oColorInt, oColorExt);
								Gdiplus::LinearGradientBrush oBrushLeft  ( Gdiplus::PointF(0, 0),              Gdiplus::PointF(float(dLeft), 0),       oColorExt, oColorInt);
								Gdiplus::LinearGradientBrush oBrushRight ( Gdiplus::PointF(float(dRight), 0),  Gdiplus::PointF(float(nWidth + 1), 0),  oColorInt, oColorExt);

								// отрисовываем цветной border
								oRender.FillRectangle( &oBrushTop, -1.0f, -1.0f, float(nWidth + 2), float(dTop + 2));
								oRender.FillRectangle( &oBrushBottom, -1.0f, float(dBottom - 1), float(nWidth + 2), float(nHeight - dBottom + 2));
								oRender.FillRectangle( &oBrushLeft, -1.0f, -1.0f, float(dLeft + 2), float(nHeight + 2));
								oRender.FillRectangle( &oBrushRight, float(dRight - 1), -1.0f, float(nWidth - dRight + 2), float(nHeight + 2));
							}

							BYTE* pCopyBGRA = pBuffer;

							if( nStride < 0 )
								pCopyBGRA += (nHeight - 1) * nWidth * 4;

							Gdiplus::Bitmap oCopyImage( nWidth, nHeight, nWidth * (nStride < 0 ? -4 : 4), PixelFormat32bppARGB, pCopyBGRA );
							if( Gdiplus::Ok == oRender.GetLastStatus() )
							{
								Gdiplus::RectF oDstRect( REAL(dLeft), REAL(dTop), REAL(dRight - dLeft), REAL(dBottom - dTop) );

								oRender.DrawImage( &oCopyImage, oDstRect, 0, 0, REAL(nWidth), REAL(nHeight), Gdiplus::UnitPixel );

								nResult = c_nErrorNone;
							}

						}
					}
					
					delete [] pBuffer;

					return nResult;
				}
				int BGRA_DrawText(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop, bool bAlign, int nAlignType, double dMargin)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							if ( !bAlign )
								nResult = GDIPLUS::GDIPLUS_DrawText( &oRender, oSettings, strText, dLeft, dTop);
							else
							{
								float fLeft       = 0.0; 
								float fTop        = 0.0;
								float fTextWidth  = 0.0;
								float fTextHeight = 0.0;

								Gdiplus::RectF oRect;
								BSTR bsText = strText.AllocSysString();
								
								oSettings.GetBrush().GetTextRect(bsText, &oRender, oSettings.GetFont().GetFont(),  0, 0, &oRect);

								::SysFreeString(bsText);

								fTextWidth  = oRect.GetRight();
								fTextHeight = oRect.GetBottom();

								switch (nAlignType)
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

								nResult = GDIPLUS::GDIPLUS_DrawText( &oRender, oSettings, strText, fLeft, fTop);

							}
						}
					}

					return nResult;
				}
				int BGRA_DrawText(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop, double dRight, double dBottom)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_DrawText( &oRender, oSettings, strText, dLeft, dTop, dRight, dBottom);
						}
					}

					return nResult;
				}
				int BGRA_DrawTextScale(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop, double dRight, double dBottom)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_DrawTextScale(&oRender, oSettings, strText, dLeft, dTop, dRight, dBottom);
						}
					}

					return nResult;
				}
				int BGRA_DrawCredits(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::CPaintStateManager& oSettings, const CString& strText, double dLeft, double dTop, double dWidth, double dHeight, double dCompleteness, int nType)
				{
					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// сообственно отрисовка
							nResult = GDIPLUS::GDIPLUS_DrawCredits( &oRender, oSettings, strText, (float)dLeft, (float)dTop, (float)dWidth, (float)dHeight, dCompleteness, nType);
						}
					}

					return nResult;
				}

				int BGRA_DrawImageFromFile(BYTE* pCanvasBGRA, int nCanvasWidth, int nCanvasHeight, int nCanvasStride, BYTE* pSourceBGRA, int nSourceWidth, int nSourceHeight, int nSourceStride, int nType, int nBackColor, int nAlpha, double dCompleteness, double dLeft, double dTop, double dRight, double dBottom, double dAngle = 0 )
				{
					if( NULL == pCanvasBGRA || NULL == pSourceBGRA )
						return c_nErrorInvalidPointer;

					if( nCanvasWidth <= 0 || nCanvasHeight <= 0 || nSourceWidth <= 0 || nSourceHeight <= 0 )
						return c_nErrorInvalidFormat;

					int alpha = int(max(0, min(nAlpha, 255)) * dCompleteness + 0.5);
					if( alpha <= 0 )
						return c_nErrorNone;

					if( nCanvasStride < 0 )
						pCanvasBGRA -= (nCanvasHeight - 1) * nCanvasStride;

					Gdiplus::Bitmap oCanvas( nCanvasWidth, nCanvasHeight, nCanvasStride, PixelFormat32bppARGB, pCanvasBGRA );
					if( Gdiplus::Ok == oCanvas.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oCanvas );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							BYTE* pBuffer = NULL;

							if( alpha < 255 )
							{
								int nPixels = nSourceWidth * nSourceHeight;
								pBuffer = new BYTE[nPixels*4];
								if( pBuffer )
								{
									BYTE* pSrc = pSourceBGRA;
									BYTE* pDst = pBuffer;
									for( ; nPixels > 0; nPixels -= 1, pSrc += 4, pDst += 4 )
									{
										pDst[0] = pSrc[0];
										pDst[1] = pSrc[1];
										pDst[2] = pSrc[2];
										pDst[3] = pSrc[3] * alpha / 255;
									}

									pSourceBGRA = pBuffer;
									nSourceStride = nSourceWidth * (nSourceStride < 0 ? -4 : 4);
								}
							}
							else
							{
								alpha = 255;
							}

							if( nSourceStride < 0 )
								pSourceBGRA -= (nSourceHeight - 1) * nSourceStride;
							
							Gdiplus::Bitmap oImage( nSourceWidth, nSourceHeight, nSourceStride, PixelFormat32bppARGB, pSourceBGRA );
							if( Gdiplus::Ok == oImage.GetLastStatus() )
							{
								// compute output rectangle
								REAL rLeft   = REAL(min(dLeft, dRight));
								REAL rTop    = REAL(min(dTop, dBottom));
								REAL rRight  = REAL(max(dLeft, dRight));
								REAL rBottom = REAL(max(dTop, dBottom));

								REAL rNaturalLeft   = rLeft;
								REAL rNaturalTop    = rTop;
								REAL rNaturalWidth  = rRight - rLeft;
								REAL rNaturalHeight = rBottom - rTop;

								double dNewWidth  = abs(dRight - dLeft);
								double dNewHeight = abs(dBottom - dTop);
								double dWidth     = nSourceWidth;
								double dHeight    = nSourceHeight;
								double dScaleX    = dNewWidth / dWidth;
								double dScaleY    = dNewHeight / dHeight;
								double dZoom      = min(1.0, min(dScaleX, dScaleY));

								switch( nType )
								{
								case c_nResizeStretchNearest:
								case c_nResizeStretchBillinear:
								case c_nResizeStretchBicubic:
									break;
								case c_nResizeShrinkNearest:
								case c_nResizeShrinkBillinear:
								case c_nResizeShrinkBicubic:
									if (dScaleX > dScaleY)
									{
										// полосы слева и справа
										REAL rOffset   = REAL((dNewWidth - (dWidth * dNewHeight / dHeight)) / 2);
										rNaturalLeft  += rOffset;
										rNaturalWidth -= rOffset * 2;
									}
									else
									{
										// полосы сверху и снизу
										REAL rOffset    = REAL((dNewHeight - (dHeight * dNewWidth / dWidth)) / 2);
										rNaturalTop    += rOffset;
										rNaturalHeight -= rOffset * 2;
									}
									break;
								case c_nResizeCropNearest:
								case c_nResizeCropBillinear:
								case c_nResizeCropBicubic:
									rNaturalLeft   = 0; 
									rNaturalTop    = 0;
									rNaturalWidth  = REAL(nSourceWidth); 
									rNaturalHeight = REAL(nSourceHeight);
									
									if( dScaleX > dScaleY )
									{
										// ширина - на месте
										double dNewHeightReal = dNewHeight;
										       dNewHeight    *= dHeight / dWidth;
										double dOffset        = (dNewHeight - dNewHeightReal) / 2;
										       dOffset       *= dHeight / dNewHeight;
										
										REAL rOffset    = (REAL)dOffset;
										//REAL rOffset  = (REAL)(((dWidth * dNewHeight / dHeight) - dWidth) / 2);
										rNaturalTop    += rOffset;
										rNaturalHeight -= rOffset * 2;
									}
									else
									{
										// высота - на месте
										double dNewWidthReal = dNewWidth;
										       dNewWidth    *= dWidth / dHeight;
										double dOffset       = (dNewWidth - dNewWidthReal) / 2;
										       dOffset      *= dWidth / dNewWidth;
										
										REAL rOffset   = (REAL)dOffset;
										//REAL rOffset = (REAL)(((dHeight * dNewWidth / dWidth) - dHeight) / 2);
										rNaturalLeft  += rOffset;
										rNaturalWidth -= rOffset * 2;
									}
									
									Gdiplus::RectF oDstRect(rLeft, rTop, rRight - rLeft, rBottom - rTop);
									if( dAngle != 0 )
									{
										float dx = oDstRect.X + oDstRect.Width / 2;
										float dy = oDstRect.Y + oDstRect.Height / 2;
										
										oRender.TranslateTransform( -dx, -dy, Gdiplus::MatrixOrderAppend );
										oRender.RotateTransform( float(-dAngle), Gdiplus::MatrixOrderAppend );
										oRender.TranslateTransform( dx, dy, Gdiplus::MatrixOrderAppend );
									}
									
									oRender.DrawImage( &oImage, oDstRect, rNaturalLeft, rNaturalTop, rNaturalWidth, rNaturalHeight, UnitPixel);
								
									if( pBuffer )
										delete [] pBuffer;

									return c_nErrorNone;
								}

								if( (rLeft < rNaturalLeft || rTop < rNaturalTop) && nBackColor >= 0 )
								{
									Gdiplus::SolidBrush brush( Gdiplus::Color( BYTE(alpha), GetRValue(nBackColor), GetGValue(nBackColor), GetBValue(nBackColor) ) );
									oRender.FillRectangle( &brush, Gdiplus::RectF(rLeft, rTop, rRight - rLeft, rBottom - rTop) );
								}

								Gdiplus::RectF oDstRect(rNaturalLeft, rNaturalTop, rNaturalWidth, rNaturalHeight);
								if( dAngle != 0 )
								{
									float dx = oDstRect.X + oDstRect.Width / 2;
									float dy = oDstRect.Y + oDstRect.Height / 2;
									
									oRender.TranslateTransform( -dx, -dy, Gdiplus::MatrixOrderAppend );
									oRender.RotateTransform( float(-dAngle), Gdiplus::MatrixOrderAppend );
									oRender.TranslateTransform( dx, dy, Gdiplus::MatrixOrderAppend );
								}

								oRender.DrawImage( &oImage, oDstRect, 0, 0, REAL(nSourceWidth), REAL(nSourceHeight), UnitPixel );

								if( pBuffer )
									delete [] pBuffer;
							}
						}
					}

					return c_nErrorNone;
				}


				int BGRA_DrawText(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::Text::CTextPainter* pPainter, double dFrame)
				{
					if( !pPainter )
						return c_nErrorInvalidPointer;

					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// отрисовываем текст
							pPainter->TextDraw( dFrame, oRender );

							nResult = c_nErrorNone;
						}
					}

					return nResult;
				}

				int BGRA_DrawText2(BYTE* pBGRA, int nWidth, int nHeight, int nStride, ImageStudio::Paint::Text::CTextPainter2* pPainter, double dFrame)
				{
					if( !pPainter )
						return c_nErrorInvalidPointer;

					int nResult = c_nErrorGdiplusFailure;

					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;

					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						if( Gdiplus::Ok == oRender.GetLastStatus() )
						{
							// отрисовываем текст
							pPainter->Draw( dFrame, oRender );

							nResult = c_nErrorNone;
						}
					}

					return nResult;
				}

				BOOL BGRA_DrawFilmsPerf(BYTE* pBGRA, int nWidth, int nHeight, int nStride, int nBlendColor, int nBkColor, float dX, float dY, float dWidth, float dHeight, float dRadius, int nAlpha)
				{
					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;
					
					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						
						oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
						oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
						oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
						
						Gdiplus::GraphicsPath oPath;

						oPath.AddLine(dX + dRadius, dY, dX + dWidth - (dRadius * 2), dY);
						oPath.AddArc(dX + dWidth - (dRadius * 2), dY, dRadius * 2, dRadius * 2, 270, 90);
						oPath.AddLine(dX + dWidth, dY + dRadius, dX + dWidth, dY + dHeight - (dRadius * 2));
						oPath.AddArc(dX + dWidth - (dRadius * 2), dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2,0,90);
						oPath.AddLine(dX + dWidth - (dRadius * 2), dY + dHeight, dX + dRadius, dY + dHeight);
						oPath.AddArc(dX, dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2, 90, 90);
						oPath.AddLine(dX, dY + dHeight - (dRadius * 2), dX, dY + dRadius);
						oPath.AddArc(dX, dY, dRadius * 2, dRadius * 2, 180, 90);
						oPath.CloseFigure();

						Gdiplus::PathGradientBrush oBrush( &oPath );
						
						oBrush.SetCenterPoint(Gdiplus::PointF(dX + dWidth / 2, dY + dHeight / 2));
						oBrush.SetCenterColor(Gdiplus::Color( nAlpha, GetRValue(nBkColor), GetGValue(nBkColor), GetBValue(nBkColor)));
						
						Gdiplus::Color oColor( nAlpha, GetRValue(nBkColor), GetGValue(nBkColor), GetBValue(nBkColor));
						int nColorCount = 1;
						oBrush.SetSurroundColors(&oColor, &nColorCount);

						oRender.FillPath(&oBrush, &oPath);

						Gdiplus::Pen oPen(Gdiplus::Color( nAlpha, GetRValue(nBlendColor), GetGValue(nBlendColor), GetBValue(nBlendColor)), 1);
						
						oRender.DrawArc(  &oPen, dX + dWidth - (dRadius * 2), dY,                           dRadius * 2, dRadius * 2, 315, 45);
						oRender.DrawLine( &oPen, dX + dWidth,                 dY + dRadius - 1,             dX + dWidth, dY + dHeight - dRadius + 1);
						oRender.DrawArc(  &oPen, dX + dWidth - (dRadius * 2), dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2,0,90);
						oRender.DrawLine( &oPen, dX + dWidth - dRadius + 1,   dY + dHeight,                 dX + dRadius - 1, dY + dHeight);
						oRender.DrawArc(  &oPen, dX,                          dY + dHeight - (dRadius * 2), dRadius * 2, dRadius * 2, 90, 45);
					}
					return TRUE;
				}

				BOOL BGRA_DrawFilmsBorder(BYTE* pBGRA, int nWidth, int nHeight, int nStride, int nFilmColorMid, int nFilmColorBorder, float dBorderWidth, float dBorderHeight, BOOL bVertical, BOOL bZoom, int nAlpha, BOOL bMoveDivider, int nSpeedKoef)
				{
					if( nStride < 0 )
						pBGRA -= (nHeight - 1) * nStride;
					
					Gdiplus::Bitmap oImage( nWidth, nHeight, nStride, PixelFormat32bppARGB, pBGRA );
					if( Gdiplus::Ok == oImage.GetLastStatus() )
					{
						Gdiplus::Graphics oRender( &oImage );
						
						oRender.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
						oRender.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
						oRender.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);

						Gdiplus::Color clMid( nAlpha, GetRValue(nFilmColorMid), GetGValue(nFilmColorMid), GetBValue(nFilmColorMid));
						Gdiplus::Color clBorder(nAlpha, GetRValue(nFilmColorBorder), GetGValue(nFilmColorBorder), GetBValue(nFilmColorBorder));
						
						if ( bVertical )
						{
							Gdiplus::SolidBrush *oSolidBrush = new Gdiplus::SolidBrush(clMid);
							Gdiplus::LinearGradientBrush* oGradBrush1 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( -1.0f, -1.0f), Gdiplus::PointF( dBorderWidth + 1.0f, -1.0f), clBorder, clMid);									
							Gdiplus::LinearGradientBrush* oGradBrush2 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( float(nWidth) + 1.0f, -1.0f), Gdiplus::PointF( float(nWidth) - dBorderWidth -1.0f, -1.0f), clBorder, clMid);									
							
							if ( !bZoom )
							{
								int nStartY = nSpeedKoef;
								if ( !bMoveDivider )
									nStartY = 0;
								int nStartY1;
								int nStartY2;
								nStartY1 = int(-1.0 + nStartY) % (nHeight + 1);
								nStartY2 = int(+1.0 + nHeight - dBorderHeight + nStartY) % (nHeight + 1);
								if ( nSpeedKoef < 0 )
								{
									if (nStartY1 < 0)
										nStartY1 += nHeight;
									if (nStartY2 < 0)
										nStartY2 += nHeight;
								}
								if (nStartY1 > nHeight - dBorderHeight )
								{
									oRender.FillRectangle(oSolidBrush, dBorderWidth - 1, float(nStartY1), float( nWidth - 2 * dBorderWidth + 1), float(dBorderHeight + 1.0));
									oRender.FillRectangle(oSolidBrush, dBorderWidth - 1, -1.0,            float( nWidth - 2 * dBorderWidth + 1), float(dBorderHeight + 1.0 - (nHeight - nStartY1) ));

								}
								else
									oRender.FillRectangle(oSolidBrush, dBorderWidth - 1, float(nStartY1), float( nWidth - 2 * dBorderWidth + 1), float(dBorderHeight + 1.0));
								if (nStartY2 > nHeight - dBorderHeight)
								{
									oRender.FillRectangle(oSolidBrush, dBorderWidth - 1, float(nStartY2), float( nWidth - 2 * dBorderWidth + 1), float(dBorderHeight + 1.0));
									oRender.FillRectangle(oSolidBrush, dBorderWidth - 1, -1.0,            float( nWidth - 2 * dBorderWidth + 1), float(dBorderHeight + 1.0 - (nHeight - nStartY2)));
								}
								else
									oRender.FillRectangle(oSolidBrush, dBorderWidth - 1, float(nStartY2), float( nWidth - 2 * dBorderWidth + 1), float(dBorderHeight + 1.0));
							}
							else
							{
								oRender.FillRectangle(oSolidBrush, -1.0, -1.0, float(nWidth + 2.0), float(nHeight + 2.0));
							}
							oRender.FillRectangle(oGradBrush1, -1.0, -1.0, dBorderWidth + 1, float(nHeight) + 1);
							
							oRender.FillRectangle(oGradBrush2, nWidth - dBorderWidth - 1, -1.0, dBorderWidth + 1, float(nHeight) + 1);

							delete oGradBrush1;
							delete oGradBrush2;
							delete oSolidBrush;
						}
						else
						{
							Gdiplus::SolidBrush *oSolidBrush = new Gdiplus::SolidBrush(clMid);
							if ( !bZoom )
							{
								int nStartX = nSpeedKoef;
								if ( !bMoveDivider )
									nStartX = 0;
								int nStartX1;
								int nStartX2;
								nStartX1 = int(-1 + nStartX) % (nWidth + 1);
								nStartX2 = int( 1 + nWidth - dBorderWidth + nStartX) % (nWidth + 1);
								if ( nSpeedKoef < 0 )
								{
									if (nStartX1 < 0)
										nStartX1 += nWidth;
									if (nStartX2 < 0)
										nStartX2 += nWidth;
								}

								if (nStartX1 > nWidth - dBorderWidth )
								{
									oRender.FillRectangle(oSolidBrush, float(nStartX1), dBorderHeight, dBorderWidth + 1, float(nHeight) - 2 * dBorderHeight);
									oRender.FillRectangle(oSolidBrush, -1.0f,     dBorderHeight, dBorderWidth + 1 - float(nWidth - nStartX1), float(nHeight) - 2 * dBorderHeight);
								}
								else
									oRender.FillRectangle(oSolidBrush, float(nStartX1), dBorderHeight, dBorderWidth + 1, float(nHeight) - 2 * dBorderHeight);
								if (nStartX2 > nWidth - dBorderWidth)
								{
									oRender.FillRectangle(oSolidBrush, float(nStartX2), dBorderHeight, dBorderWidth + 1, float(nHeight) - 2 * dBorderHeight);
									oRender.FillRectangle(oSolidBrush, -1.0f,     dBorderHeight, dBorderWidth + 1 - float(nWidth - nStartX2), float(nHeight) - 2 * dBorderHeight);
								}
								else
									oRender.FillRectangle(oSolidBrush, float(nStartX2), dBorderHeight, dBorderWidth + 1, float(nHeight) - 2 * dBorderHeight);
							}
							else
							{
								oRender.FillRectangle(oSolidBrush, -1.0f, -1.0f, float(nWidth) + 2, float(nHeight) + 2);
							}
							Gdiplus::LinearGradientBrush* oGradBrush1 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( -1, -1), Gdiplus::PointF( -1, dBorderHeight + 1), clBorder, clMid);									
							oRender.FillRectangle(oGradBrush1, -1.0f, -1.0f, float(nWidth) + 2, dBorderHeight + 1);
							
							Gdiplus::LinearGradientBrush* oGradBrush2 = new Gdiplus::LinearGradientBrush(Gdiplus::PointF( -1, float(nHeight) + 1), Gdiplus::PointF( -1, float(nHeight) - dBorderHeight - 1), clBorder, clMid);									
							oRender.FillRectangle(oGradBrush2, -1.0f, float(nHeight) - dBorderHeight - 1, float(nWidth) + 2, dBorderHeight + 2);

							delete oGradBrush1;
							delete oGradBrush2;
							delete oSolidBrush;
						}
					}
					return TRUE;
				}

				BOOL BGRA_DrawFilmFull(BYTE* pBGRA, int nWidth, int nHeight, int nStride, double dTime, int nSpeed, double dWideFactor, BOOL bVertical, BOOL bDrawBorder, int nBkColor, int nFilmColorMid, int nFilmColorBorder, int nBlendColor, BOOL bZoom, int nAlpha, BOOL bMoveDivider, double dSpeedFactor)
				{
					int nCurSpeed = nSpeed;

					if ( nCurSpeed < -100 || nCurSpeed > 100 )
						nCurSpeed = 0;

					double dFactor = dWideFactor / 200;
					
					if ( dFactor < 0 || dFactor > 0.5 )
						dFactor = 4.21 / 35.;

					if( bVertical )
					{
						double dUnit                  = nWidth * dFactor / 4.21; // классическое значение dWideFactor = 4.21 / 35. 
						const double dWidthPerf       = 2.8 * dUnit;
						const double dHeightPerf      = 1.98 * dUnit;
						const double dRadiusPerf      = 0.5 * dUnit;
						const double dStepPerf        = (4.75 * dUnit < 10? 10 : 4.75 * dUnit);
						const double dBorderWidth     = 1.01 * dUnit;
						const double dMidLineWidth    = 0.4 * dUnit;
						const double dRadiusFactor    = 1.3 * sqrt(2.) / 2 * dRadiusPerf;
						//double dStartY = int(nCurSpeed * dStepPerf / 100 * dTime/10) % int(dStepPerf);
						int nSpeedKoef = int(nCurSpeed * dStepPerf / 100 * dTime / 1000  * dSpeedFactor);
						double dStartY = nSpeedKoef % int(dStepPerf);

						const double dFullBorderWidth = dBorderWidth + dWidthPerf + dMidLineWidth;

						if ( bDrawBorder)
							BGRA_DrawFilmsBorder(pBGRA, nWidth, nHeight, nStride, nFilmColorMid, nFilmColorBorder, float(dFullBorderWidth), float(0.1 * dFullBorderWidth), bVertical, bZoom, nAlpha, bMoveDivider, nSpeedKoef);

						for ( double dCurY = dStartY - 2 * dStepPerf; dCurY < nHeight + dStepPerf; dCurY += dStepPerf)
						{
							BGRA_DrawFilmsPerf(pBGRA, nWidth, nHeight, nStride, nBlendColor, nBkColor, float(dBorderWidth),                       float(dCurY), float(dWidthPerf), float(dHeightPerf), float(dRadiusPerf), nAlpha);
							BGRA_DrawFilmsPerf(pBGRA, nWidth, nHeight, nStride, nBlendColor, nBkColor, float(nWidth - dBorderWidth - dWidthPerf), float(dCurY), float(dWidthPerf), float(dHeightPerf), float(dRadiusPerf), nAlpha);
						}
						
					}
					else
					{
						double dUnit                  = nHeight * dFactor / 4.21; // классическое значение dWideFactor = 4.21 / 35.
						const double dWidthPerf       = 2.8 * dUnit;
						const double dHeightPerf      = 1.98 * dUnit;
						const double dRadiusPerf      = 0.5 * dUnit;
						const double dStepPerf        = (4.75 * dUnit < 10? 10 : 4.75 * dUnit);
						const double dBorderWidth     = 1.01 * dUnit;
						const double dMidLineWidth    = 0.4 * dUnit;
						const double dRadiusFactor    = 1.3 * sqrt(2.) / 2 * dRadiusPerf;
						//double dStartX = int(nCurSpeed * dStepPerf / 100 * dTime/10) % int(dStepPerf);
						int nSpeedKoef = int(nCurSpeed * dStepPerf / 100 * dTime / 1000 * dSpeedFactor);
						double dStartX = nSpeedKoef % int(dStepPerf);

						const double dFullBorderWidth = dBorderWidth + dWidthPerf + dMidLineWidth;

						if ( bDrawBorder )
							BGRA_DrawFilmsBorder(pBGRA, nWidth, nHeight, nStride, nFilmColorMid, nFilmColorBorder, float(0.1 * dFullBorderWidth), float(dFullBorderWidth), bVertical, bZoom, nAlpha, bMoveDivider, nSpeedKoef);

						for ( double dCurX = dStartX - 2 * dStepPerf; dCurX < nWidth + dStepPerf; dCurX += dStepPerf)
						{
							BGRA_DrawFilmsPerf(pBGRA, nWidth, nHeight, nStride, nBlendColor, nBkColor, float(dCurX), float(dBorderWidth                       ), float(dHeightPerf), float(dWidthPerf), float(dRadiusPerf), nAlpha);
							BGRA_DrawFilmsPerf(pBGRA, nWidth, nHeight, nStride, nBlendColor, nBkColor, float(dCurX), float(nHeight - dBorderWidth - dWidthPerf), float(dHeightPerf), float(dWidthPerf), float(dRadiusPerf), nAlpha);
						}
					}

					return TRUE;
				}				
				int BGRA_DrawFilm(BYTE* pBGRA, int nWidth, int nHeight, int nStride, double dFrame,double dTime, int nSpeed, double dWideFactor, BOOL bVertical, BOOL bDrawBorder, int nBkColor, int nFilmColorMid, int nFilmColorBorder, int nBlendColor, BOOL bZoom, int nAlpha, BOOL bMoveDivider, double dSpeedFactor)
				{
					int nSize = 4 * nWidth * nHeight;
					
					int nCurAlpha = bZoom ? 255 : int(nAlpha * dFrame);
					
					CBuffer<BYTE> pBuffer( nSize );
					if( pBuffer.IsEmpty() )
						return c_nErrorNoMemory;

					::memcpy(pBuffer.GetPtr(), pBGRA, nSize);

					BGRA_DrawFilmFull(pBuffer, nWidth, nHeight, nStride, dTime, nSpeed, dWideFactor, bVertical, bDrawBorder, nBkColor, nFilmColorMid, nFilmColorBorder, nBlendColor, bZoom, nCurAlpha, bMoveDivider, dSpeedFactor);

					if ( bZoom )
					{
						BYTE* pSourceBuffer = new BYTE[nSize];
						::memcpy(pSourceBuffer, pBGRA, nSize);
						double dKoef = 1 - dFrame;
						double dPercent = dWideFactor / 100;
						double dFactor = (1./(1 - dPercent) * dKoef + 1 *(1-dKoef));
						double dFactor2 = ((1 - dPercent) * (1 - dKoef) + 1 * dKoef);
						BGRA_ResizeCenter(pBuffer, pBGRA, nWidth, nHeight, dFactor, dFactor, nWidth / 2, nHeight / 2);
						BGRA_ResizeCenter(pSourceBuffer, pBGRA, nWidth, nHeight, dFactor2, dFactor2, nWidth / 2, nHeight / 2);
						delete []pSourceBuffer;
					}
					else
					{
						//CAlphaTable table;
						//if( table.Calculate( nCurAlpha ) > 0 )
						//{
						//	memecpy( pBGRA, pBuffer, nSize );
						//}

						for ( int nIndex = 0; nIndex < nSize; nIndex ++ )
						{
							//pBGRA[nIndex] = BYTE((1 - dFrame) * pBGRA[nIndex] + dFrame * pBuffer[nIndex]);
							pBGRA[nIndex] = BYTE(((255 - nCurAlpha) * pBGRA[nIndex] + nCurAlpha * pBuffer[nIndex]) / 255);
						} 
					}
					
					return c_nErrorNone;
				}


				int BGRA_DrawPuzzle(BYTE* pBGRA, int nWidth, int nHeight, int nStride, double dFrame, int nRow, int nCol, ImageStudio::Paint::Effects::Puzzle* pEffect, double dSideFactor, int nType)
				{
					pEffect->Draw(pBGRA, nWidth, nHeight, nStride, dFrame, nRow, nCol, dSideFactor, nType);
					return c_nErrorNone;

				}
#endif	
				void BGRA_GetSimpleHistogram( BYTE* pPixels, int nWidth, int nHeight, DWORD xHistogram[256] )
				{
					ZeroMemory( xHistogram, 256 * sizeof(DWORD) );

					int nSize = nWidth * nHeight;

					for( int i = 0; i < nSize; ++i, pPixels += 4 )
					{
						xHistogram[pPixels[0]] += 1;
						xHistogram[pPixels[1]] += 1;
						xHistogram[pPixels[2]] += 1;
					}
				}

				void BGRA_GetHistogram( BYTE* pPixels, int nWidth, int nHeight, DWORD xHistogram[3][256] )
				{
					ZeroMemory( xHistogram, 256 * sizeof(DWORD) );

					int nSize = nWidth * nHeight;

					for( int i = 0; i < nSize; ++i, pPixels += 4 )
					{
						xHistogram[0][pPixels[0]] += 1;
						xHistogram[1][pPixels[1]] += 1;
						xHistogram[2][pPixels[2]] += 1;
					}
				}

				int BGRA_GetAutoContrast( BYTE* pPixels, int nWidth, int nHeight, double* Level)
				{
					// variables
					DWORD xHistogram[256]; 
					
					BGRA_GetSimpleHistogram( pPixels, nWidth, nHeight, xHistogram );
					
					// variables
					DWORD nContrastRange = 0;
					DWORD nContrastLevel = MulDiv(nWidth * nHeight * 4, 5, 1000);

					// compute minimal scaling value
					DWORD nContrastMax = 0;
					DWORD nContrastMin = 0;
					DWORD nContrastSum = 0;
					
					for( int i = 0; i < 256; ++i )
					{
						if( nContrastSum >= nContrastLevel )
						{
							nContrastMin = i;
							break;
						}

						nContrastSum += xHistogram[i];
					}

					// compute maximal scaling value
					nContrastSum = 0;
					for( int i = 255; i >= 0; --i )
					{
						if( nContrastSum >= nContrastLevel )
						{
							nContrastMax = i;
							break;
						}

						nContrastSum += xHistogram[i];
					}

					// clamp ranges values
					nContrastMin = min(nContrastMin, 254);
					nContrastMax = max(nContrastMax, nContrastMin + 1);
					nContrastRange = nContrastMax - nContrastMin;

					*Level = nContrastRange / 255.0;

					return c_nErrorNone;
				}
				int BGRA_GetAutoLevels( BYTE* pPixels, int nWidth, int nHeight, double* LevelR, double* LevelG, double* LevelB)
				{
					// variables
					DWORD xHistogram[3][256];

					BGRA_GetHistogram( pPixels, nWidth, nHeight, xHistogram );

					// variables
					DWORD nContrastLevel = MulDiv(nWidth * nHeight * 4 / 3, 5, 1000);
					DWORD nContrastSum[3], nContrastMin[3], nContrastMax[3];

					// compute minimal scaling value
					nContrastMin[0] = nContrastMin[1] = nContrastMin[2] = 0;
					nContrastSum[0] = nContrastSum[1] = nContrastSum[2] = 0;
					for( int i = 0; i < 256; ++i )
					{
						if (!nContrastMin[0] && nContrastSum[0] >= nContrastLevel) nContrastMin[0] = i;
						if (!nContrastMin[1] && nContrastSum[1] >= nContrastLevel) nContrastMin[1] = i;
						if (!nContrastMin[2] && nContrastSum[2] >= nContrastLevel) nContrastMin[2] = i;

						nContrastSum[0] += xHistogram[0][i];
						nContrastSum[1] += xHistogram[1][i];
						nContrastSum[2] += xHistogram[2][i];
					}

					// compute maximal scaling value
					nContrastMax[0] = nContrastMax[1] = nContrastMax[2] = 0;
					nContrastSum[0] = nContrastSum[1] = nContrastSum[2] = 0;
					
					for( int i = 255; i >= 0; --i )
					{
						if (!nContrastMax[0] && nContrastSum[0] >= nContrastLevel) nContrastMax[0] = i;
						if (!nContrastMax[1] && nContrastSum[1] >= nContrastLevel) nContrastMax[1] = i;
						if (!nContrastMax[2] && nContrastSum[2] >= nContrastLevel) nContrastMax[2] = i;

						nContrastSum[0] += xHistogram[0][i];
						nContrastSum[1] += xHistogram[1][i];
						nContrastSum[2] += xHistogram[2][i];
					}

					// clamp ranges values
					nContrastMin[0] = min(nContrastMin[0], 254); nContrastMax[0] = max(nContrastMax[0], nContrastMin[0] + 1);
					nContrastMin[1] = min(nContrastMin[1], 254); nContrastMax[1] = max(nContrastMax[1], nContrastMin[0] + 1);
					nContrastMin[2] = min(nContrastMin[2], 254); nContrastMax[2] = max(nContrastMax[2], nContrastMin[0] + 1);

					*LevelR = (nContrastMin[2]+1000*nContrastMax[2])/5000.0;
					*LevelG = (nContrastMin[1]+1000*nContrastMax[1])/5000.0;
					*LevelB = (nContrastMin[0]+1000*nContrastMax[0])/5000.0;
				
					return c_nErrorNone;
				}

				int BGRA_GetAutoPosterize( BYTE* pPixels, int nWidth, int nHeight, double* Levels)
				{
					double tmpVals[3];
					
					BGRA_GetAutoLevels( pPixels, nWidth, nHeight, &tmpVals[0], &tmpVals[1], &tmpVals[2] );
					
					*Levels = (tmpVals[0] + tmpVals[1] + tmpVals[2]) / 3.0;

					return c_nErrorNone;
				}

				int BGRA_GetAutoSaturation( BYTE* pPixels, int nWidth, int nHeight, double* Level)
				{
					double dColor[4] = {0, 0, 0, 0};

					for( int i = nWidth * nHeight; i > 0; --i, pPixels += 4)
					{
						// original color -> Hue-Saturation-Lighteness
						BgrToHls( pPixels, dColor );

						dColor[3] += dColor[2];
					}

					*Level = dColor[3] / (nWidth * nHeight);

					return c_nErrorNone;
				}

				int BGRA_GetAutoHue( BYTE* pPixels, int nWidth, int nHeight, double* Angle)
				{
					double dColor[4] = {0, 0, 0, 0};
					double dCountPixels = 0;

					int nSize = nWidth * nHeight;

					for( int i = 0; i < nSize; ++i, pPixels += 4)
					{
						int a = pPixels[3];
						if( a > 0 )
						{
							BgrToHls( pPixels, dColor );

							dColor[3] += dColor[0] * ByteDiv255[a];
							dCountPixels += ByteDiv255[a];
						}
					}

					if( dCountPixels > 0 )
						dColor[3] /= dCountPixels;

					*Angle = dColor[3];
					
					return c_nErrorNone;
				}

				int BGRA_GetAutoGamma( BYTE* pPixels, int nWidth, int nHeight, double* Level)
				{
					*Level = 0;
					
					return c_nErrorNone;
				}

				int BGRA_GetAutoBrightness( BYTE* pPixels, int nWidth, int nHeight, double* Level)
				{
					*Level = 0;
					double dOverallSum = 0;
					double dCountPixels = 0;

					for( int count = nWidth * nHeight; count > 0; count -= 0x8100 )
					{
						int nRowSize = min(count, 0x8100);

						int sumGray = 0;
						int sumA = 0;

						for( int i = 0; i < nRowSize; ++i, pPixels += 4 )
						{
							int a = pPixels[3];
							if( a > 0 )
							{
								sumGray += a * Intensity3( pPixels[2], pPixels[1], pPixels[0] );
								sumA += a;
							}
						}

						dOverallSum += (double)sumGray / 255;
						dCountPixels += (double)sumA / 255;
					}

					if( dCountPixels > 0 )
						dOverallSum /= dCountPixels;

					int nSeparatePoint = 0;
					dOverallSum -= 128 + nSeparatePoint;
					/*if ( dOverallSum < 0 )
						dOverallSum = dOverallSum * 255 / (128 + nSeparatePoint);
					else
						dOverallSum = dOverallSum * 255 / (127 - nSeparatePoint);*/

					*Level = (int)(-dOverallSum);
					
					return c_nErrorNone;
				}

				
				int BGRA_AdjustBrightness( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Level)
				{
					Frame = Clamp(Frame);

					Level = min(255, max(-255, Level));
					Level = int(Level * Frame + (Level < 0 ? -0.5 : 0.5));
					
					if( !Level )
						return c_nErrorNone;

					BYTE Table[256];

					// compose table
					for (int nIndex = 0; nIndex < 256; ++nIndex)
					{
						//if( Level > 0 )
						//{
						//	Table[nIndex] = BYTE(((unsigned)(255 - nIndex) * Level + nIndex * 255 + 127) * 0x8081u >> 23);
						//}
						//else
						//{
						//	Table[nIndex] = BYTE((unsigned)(nIndex * (255 + Level) + 127) * 0x8081u >> 23);
						//}

						Table[nIndex] = IntToByte( nIndex + Level );
					}

					// apply table
					return BGRA_ApplyColorTable( pPixels, nWidth, nHeight, Table );
				}
				int BGRA_AdjustBrightnessEx( BYTE* pPixels, int nWidth, int nHeight, double Frame, long LevelR, long LevelG, long LevelB )
				{
					if (!LevelR && !LevelG && !LevelB)
						return c_nErrorNone;

					// clamp value
					Frame = Clamp(Frame);

					LevelR = min(255, max(-255, LevelR));
					LevelG = min(255, max(-255, LevelG));
					LevelB = min(255, max(-255, LevelB));

					LevelR = int(LevelR * Frame + (LevelR < 0 ? -0.5 : 0.5));
					LevelG = int(LevelG * Frame + (LevelG < 0 ? -0.5 : 0.5));
					LevelB = int(LevelB * Frame + (LevelB < 0 ? -0.5 : 0.5));

					BYTE Table[3][256]; // blue, green, red

					// compose table
					for (int i = 0; i < 256; ++i)
					{
						Table[2][i] = IntToByte( i + LevelR );
						Table[1][i] = IntToByte( i + LevelG );
						Table[0][i] = IntToByte( i + LevelB );
					}

					// apply table
					return BGRA_ApplyColorTableEx( pPixels, nWidth, nHeight, Table );
				}

				int BGRA_AdjustContrast( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Level )
				{
					// check for no-action effect
					if (fabs(Level - 1.0) < 0.001)
						return c_nErrorNone;

					// clamp value
					Frame = Clamp(Frame);

					// correct contrast level value
					Level = min(100, max(-100, Level));
					
					// compute contrast level
					double dLevel = Frame * Level / 100.0 + 1;
					if( dLevel > 1 )
						dLevel = pow( 255, dLevel - 1 );

					BYTE Table[256];

					// compose table
					for (int i = 0; i < 256; ++i)
					{
						Table[i] = IntToByte( int(127.5 + (i - 127.5)*dLevel) );
					}

					// apply table
					return BGRA_ApplyColorTable( pPixels, nWidth, nHeight, Table );
				}

				int BGRA_AdjustGamma( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Level )
				{
					if (fabs(Level - 1.0) < 0.001)
						return c_nErrorNone;

					// clamp value
					Frame = Clamp(Frame);

					// correct contrast level value
					double dLevel = min(100, max(-100, Level));

					if( dLevel <= 0 )
						dLevel = 1 / max(0.0001, Frame * dLevel / 100 + 1);
					else
						dLevel = 1 / pow(255, Frame * dLevel / 100);

					double dFactor = 255 / pow(255, dLevel);

					// compute gamma levels
					BYTE Table[256];

					for( int i = 0; i < 256; ++i )
					{
						Table[i] = Byte(dFactor * pow(i, dLevel));
					}

					// apply table
					return BGRA_ApplyColorTable( pPixels, nWidth, nHeight, Table );
				}

				int BGRA_AdjustLevels( BYTE* pPixels, int nWidth, int nHeight, double Frame, double LevelR, double LevelG, double LevelB)
				{
					if( Frame <= 0 )
						return c_nErrorNone;

					if( Frame > 1 )
						Frame = 1;

					int nContrastMin[3], nContrastMax[3];

					nContrastMax[0] = (int)(LevelB * 5000.0 / 1000.0);
					nContrastMin[0] = (int)(LevelB * 5000.0 - nContrastMax[0] * 1000.0);
					nContrastMax[1] = (int)(LevelG * 5000.0 / 1000.0);
					nContrastMin[1] = (int)(LevelG * 5000.0 - nContrastMax[1] * 1000.0);
					nContrastMax[2] = (int)(LevelR * 5000.0 / 1000.0);
					nContrastMin[2] = (int)(LevelR * 5000.0 - nContrastMax[2] * 1000.0);

					double dFactors[3] = {
						255.0 / (nContrastMax[0] - nContrastMin[0]),
						255.0 / (nContrastMax[1] - nContrastMin[1]),
						255.0 / (nContrastMax[2] - nContrastMin[2])
					};
					
					BYTE Table[3][256]; // blue, green, red

					// compose table
					for( int i = 0; i < 256; ++i )
					{
						Table[2][i] = int(ClampByte((i - nContrastMin[2]) * dFactors[2]) * Frame + i * (1 - Frame) + 0.5);
						Table[1][i] = int(ClampByte((i - nContrastMin[1]) * dFactors[1]) * Frame + i * (1 - Frame) + 0.5);
						Table[0][i] = int(ClampByte((i - nContrastMin[0]) * dFactors[0]) * Frame + i * (1 - Frame) + 0.5);
					}

					// apply table
					return BGRA_ApplyColorTableEx( pPixels, nWidth, nHeight, Table );
				}
#ifndef IMAGE_EDITOR
				int BGRA_AdjustEqualize( BYTE* pPixels, int nWidth, int nHeight, double Frame )
				{
					Frame = Clamp( Frame );

					int alpha = int(Frame * 255);

					// calculate help table
					BYTE Table[256];
					for( int i = 0; i < 256; i++ )
					{
						Table[i] = AlphaComp( i, min(255, (i * i + 64) >> 7), alpha );
					}
					
					// apply table
					return BGRA_ApplyColorTable( pPixels, nWidth, nHeight, Table );
				}

				int BGRA_AdjustGrayscale( BYTE* pPixels, int nWidth, int nHeight, double Frame, BOOL Desaturate )
				{
					CAlphaTable table;
					if( table.Calculate( Frame ) < 0 )
						return c_nErrorNone;

					int nSize = nWidth * nHeight;

					if( table.IsEmpty() )
					{
						if( Desaturate )
						{
							for( int i = 0; i < nSize; ++i, pPixels += 4 )
							{
								pPixels[2] = pPixels[1] = pPixels[0] = Lightness2( pPixels[2], pPixels[1], pPixels[0] );
							}
						}
						else
						{
							for( int i = 0; i < nSize; ++i, pPixels += 4 )
							{
								pPixels[2] = pPixels[1] = pPixels[0] = Intensity2( pPixels[2], pPixels[1], pPixels[0] );
							}
						}
					}
					else
					{
						if( Desaturate )
						{
							for( int i = 0; i < nSize; ++i, pPixels += 4 )
							{
								int b = pPixels[0];
								int g = pPixels[1];
								int r = pPixels[2];
								int clr = Lightness2( r, g, b );

								pPixels[0] = table.Blend( b, clr );
								pPixels[1] = table.Blend( g, clr );
								pPixels[2] = table.Blend( r, clr );
							}
						}
						else
						{
							for( int i = 0; i < nSize; ++i, pPixels += 4 )
							{
								int b = pPixels[0];
								int g = pPixels[1];
								int r = pPixels[2];
								int clr = Intensity2( r, g, b );

								pPixels[0] = table.Blend( b, clr );
								pPixels[1] = table.Blend( g, clr );
								pPixels[2] = table.Blend( r, clr );
							}
						}
					}

					return c_nErrorNone;
				}

#endif
				int BGRA_AdjustInvert( BYTE* pPixels, int nWidth, int nHeight, double Frame, BOOL bOverGray )
				{
					Frame = Clamp( Frame );

					int alpha = int(Frame * 255);
					if( alpha <= 0 )
						return c_nErrorNone;

					// table
					BYTE Table[256];

					// compose table
					for( int i = 0; i < 256; ++i )
					{
						Table[i] = AlphaComp( i, 255 - i, alpha );
					}

					// apply table
					return BGRA_ApplyColorTable( pPixels, nWidth, nHeight, Table );
				}

				int BGRA_AdjustAutobrightness( BYTE* pPixels, int nWidth, int nHeight, double Frame )
				{
					double AutoBright = 0;

					BGRA_GetAutoBrightness( pPixels, nWidth, nHeight, &AutoBright);

					return BGRA_AdjustBrightness( pPixels, nWidth, nHeight, Frame, long(AutoBright));
				}

				int BGRA_AdjustAutocontrast( BYTE* pPixels, int nWidth, int nHeight, double Frame )
				{
					double AutoContr = 0;

					BGRA_GetAutoContrast( pPixels, nWidth, nHeight, &AutoContr);

					return BGRA_AdjustContrast( pPixels, nWidth, nHeight, Frame, AutoContr);
				}

				int BGRA_AdjustAutolevels( BYTE* pPixels, int nWidth, int nHeight, double Frame )
				{
					double AutoLR = 0;
					double AutoLG = 0;
					double AutoLB = 0;
					
					BGRA_GetAutoLevels( pPixels, nWidth, nHeight, &AutoLR, &AutoLG, &AutoLB );
					
					return BGRA_AdjustLevels( pPixels, nWidth, nHeight, Frame, AutoLR, AutoLG, AutoLB );
				}

				int BGRA_AdjustHue( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Angle )
				{
					// clamp values
					Angle *= Clamp(Frame);
					Angle  = min(max(-360, Angle), 360);

					double dColor[4] = {0, 0, 0, Angle};

					// balance each pixel
					for (int nCount = nWidth*nHeight; nCount > 0; --nCount, pPixels += 4)
					{
						// original color -> Hue-Saturation-Lighteness
						BgrToHls( pPixels, dColor );

						// compute new saturation value
						dColor[0] += dColor[3];

						// clamp value
						if( dColor[0] >= 360 ) dColor[0] -= 360;
						if( dColor[0] < 0 )    dColor[0] += 360;

						// compute new color components
						HlsToBgr( dColor, pPixels );
					}

					return c_nErrorNone;
				}

				int BGRA_AdjustSaturation( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Level )
				{
					// clamp values
					Frame = Clamp(Frame);
					Level = min(max(-100, Level), 100)*Frame/100.0;

					double dColor[4] = {0, 0, 0, Level + 1};

					// balance each pixel
					for (int nCount = nWidth*nHeight; nCount > 0; --nCount, pPixels += 4)
					{
						// original color -> Hue-Saturation-Lighteness
						BgrToHls( pPixels, dColor );

						// compute new saturation value
						dColor[2] *= dColor[3];

						// clamp value
						if( dColor[2] > 1 ) dColor[2] = 1;

						// compute new color components
						HlsToBgr( dColor, pPixels );
					}

					return c_nErrorNone;
				}

				int BGRA_AdjustPosterize( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Levels )
				{
					Frame = Clamp( Frame );

					int alpha = int(Frame * 255);
					if( alpha <= 0 )
						return c_nErrorNone;
					
					Levels = max(2, min(255, Levels));

					// variables
					BYTE Table[256];

					double dKoef = (Levels - 1) / 255.0;
					int nMaxValue = int(Levels - 1);

					// calculate table
					for( int i = 0; i < 256; ++i )
					{
						int nValue = (int)(0.5 + i * dKoef);

						if (nValue > nMaxValue)
							nValue = nMaxValue;

						Table[i] = AlphaComp( i, 255 * nValue / nMaxValue, alpha );
					}
					
					// apply table
					return BGRA_ApplyColorTable( pPixels, nWidth, nHeight, Table );
				}

				int BGRA_AdjustTemperature( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Level )
				{
					if( Frame <= 0 )
						return c_nErrorNone;

					if( Frame > 1 )
						Frame = 1;

					// clamp values
					Level = max(-100, min(100, Level));
					Level *= Frame;

					// color adjustment coeffs
					int nDeltaRed   = int(floor(0.3  * Level));
					int nDeltaGreen = int(floor(0.05 * Level));
					int nDeltaBlue  = int(floor(-0.2 * Level));

					BYTE Table[3][256]; // blue, green, red

					// compose table
					for( int i = 0; i < 256; ++i )
					{
						Table[2][i] = IntToByte(i + nDeltaRed);
						Table[1][i] = IntToByte(i + nDeltaGreen);
						Table[0][i] = IntToByte(i + nDeltaBlue);
					}

					// apply table
					return BGRA_ApplyColorTableEx( pPixels, nWidth, nHeight, Table );
				}
#ifndef IMAGE_EDITOR
				int BGRA_AdjustColorize( BYTE* pPixels, int nWidth, int nHeight, double Frame, int Type, int Red, int Green, int Blue )
				{
					CAlphaTable table;
					if( table.Calculate( Frame ) < 0 )
						return c_nErrorNone;

					if( Type < 0 || Type > 3 )
						return c_nErrorNone;

					Blue  = IntToByte( Blue );
					Green = IntToByte( Green );
					Red   = IntToByte( Red );
					
					// color tables
					BYTE oTableB[256];
					BYTE oTableG[256];
					BYTE oTableR[256];

					for( int i = 0; i < 256; i++ )
					{
						switch( Type )
						{
						case 0:
							oTableB[i] = AlphaComp( 0, Blue,  i );
							oTableG[i] = AlphaComp( 0, Green, i );
							oTableR[i] = AlphaComp( 0, Red,   i );
							break;
						case 1:
							oTableB[i] = AlphaComp( Blue,  0,  i );
							oTableG[i] = AlphaComp( Green, 0,  i );
							oTableR[i] = AlphaComp( Red,   0,  i );
							break;
						case 2:
							oTableB[i] = AlphaComp( i, Blue,  i );
							oTableG[i] = AlphaComp( i, Green, i );
							oTableR[i] = AlphaComp( i, Red,   i );
							break;
						case 3:
							{
								int a = 127 - abs(i - 127);
								oTableB[i] = AlphaComp( i, Blue,  a );
								oTableG[i] = AlphaComp( i, Green, a );
								oTableR[i] = AlphaComp( i, Red,   a );
							}
							break;
						}
					}

					BYTE* pMaxPtr = pPixels + nWidth * nHeight * 4;

					if( table.IsEmpty() )
					{
						// apply tables
						for( ; pPixels != pMaxPtr; pPixels += 4 )
						{
							int index = Intensity2( pPixels[2], pPixels[1], pPixels[0]);

							pPixels[0] = oTableB[index];
							pPixels[1] = oTableG[index];
							pPixels[2] = oTableR[index];
						}
					}
					else
					{
						// apply tables
						for( ; pPixels != pMaxPtr; pPixels += 4 )
						{
							int b = pPixels[0];
							int g = pPixels[1];
							int r = pPixels[2];
							int index = Intensity2( r, g, b);

							pPixels[0] = table.Blend( b, oTableB[index] );
							pPixels[1] = table.Blend( g, oTableG[index] );
							pPixels[2] = table.Blend( r, oTableR[index] );
						}
					}

					return c_nErrorNone;
				}

				int BGRA_AdjustThreshold( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Level, BOOL Grayscale )
				{
					Frame = Clamp( Frame );

					int alpha = int(Frame * 255);
					if( alpha <= 0 )
						return c_nErrorNone;

					// check for grayscale image
					if( Grayscale )
					{
						BGRA_AdjustGrayscale( pPixels, nWidth, nHeight, Frame, FALSE );
					}

					BYTE Table[256];

					// compose table
					for( int i = 0; i < 256; ++i )
					{
						Table[i] = AlphaComp( i, i < Level ? 0 : 255, alpha );
					}

					// apply table
					return BGRA_ApplyColorTable( pPixels, nWidth, nHeight, Table );
				}
#endif
				
				int BGRA_AdjustSepia( BYTE* pPixels, int nWidth, int nHeight, float fLevel, float fRed, float fGreen, float fBlue )
				{
					//New Red   = R*.393 + G*.769 + B*.189
					//New Green = R*.349 + G*.686 + B*.168
					//New Blue  = R*.272 + G*.534 + B*.131

					fRed   *= 256;
					fGreen *= 256;
					fBlue  *= 256;
					
					//int matrix[3][3] = { 
					//	int(fRed   * (1 - fLevel * (1 - 0.393f))),
					//	int(fRed   * fLevel * 0.769f),
					//	int(fRed   * fLevel * 0.189f),
					//	int(fGreen * fLevel * 0.349f),
					//	int(fGreen * (1 - fLevel * (1 - 0.686f))),
					//	int(fGreen * fLevel * 0.168f),
					//	int(fBlue  * fLevel * 0.272f),
					//	int(fBlue  * fLevel * 0.534f),
					//	int(fBlue  * (1 - fLevel * (1 - 0.131f))),
					//};

					// MMX версия (прирост скорости 75% от стандартной версии)

					_CRT_ALIGN(8)
					short factors[] = { 
						short(fBlue  * (1 - fLevel * (1 - 0.131f))),
						short(fGreen * fLevel * 0.168f),
						short(fRed   * fLevel * 0.189f),
						0,
						short(fBlue  * fLevel * 0.534f),
						short(fGreen * (1 - fLevel * (1 - 0.686f))),
						short(fRed   * fLevel * 0.769f),
						0,
						short(fBlue  * fLevel * 0.272f),
						short(fGreen * fLevel * 0.349f),
						short(fRed   * (1 - fLevel * (1 - 0.393f))),
						0
					};

					__asm
					{
						// загрузка констант
						mov       eax, 800080h
						movd      mm6, eax
						punpckldq mm6, mm6

						movq      mm0, factors[0]
						movq      mm1, factors[8]
						movq      mm2, factors[16]
				
						// загрузка счётчика и указателей
						mov       eax, nWidth
						imul      eax, nHeight
						mov       ecx, eax
						mov       edi, pPixels
						add       edi, 4

						// основной цикл
						ALIGN 8
						main_loop:

						movzx     eax, byte ptr [edi - 4]
						movzx     edx, byte ptr [edi - 3]
						movd      mm3, eax
						punpcklwd mm3, mm3
						punpckldq mm3, mm3
						movd      mm4, edx
						punpcklwd mm4, mm4
						punpckldq mm4, mm4
						movzx     eax, byte ptr [edi - 2]
						movzx     edx, byte ptr [edi - 1]
						movd      mm5, eax
						punpcklwd mm5, mm5
						punpckldq mm5, mm5

						pmullw    mm3, mm0
						pmullw    mm4, mm1
						paddusw   mm3, mm4
						pmullw    mm5, mm2
						paddusw   mm3, mm6
						paddusw   mm3, mm5
						psrlw     mm3, 8

						movd      mm7, edx
						packuswb  mm3, mm3
						psllq     mm7, 24
						add       edi, 4
						por       mm3, mm7
						dec       ecx
						movd      [edi - 8], mm3
					
						jnz       main_loop

						EMMS
					}

					//// стандартная версия
					//for( int count = nWidth * nHeight; count > 0; --count, pPixels += 4 )
					//{
					//	int b = pPixels[0];
					//	int g = pPixels[1];
					//	int r = pPixels[2];

					//	pPixels[2] = min(255, (r * matrix[0][0] + g * matrix[0][1] + b * matrix[0][2] + 128) >> 8);
					//	pPixels[1] = min(255, (r * matrix[1][0] + g * matrix[1][1] + b * matrix[1][2] + 128) >> 8);
					//	pPixels[0] = min(255, (r * matrix[2][0] + g * matrix[2][1] + b * matrix[2][2] + 128) >> 8);
					//}

					return c_nErrorNone;
				}


				int BGRA_AdjustWhiteBalance( BYTE* pPixels, int nWidth, int nHeight, double dFrame, int nRed, int nGreen, int nBlue )
				{
					dFrame = Clamp( dFrame );

					int nSize = nWidth * nHeight;

					double dMeanR = max(0, min(255, nRed));   //dSumR / nSize;
					double dMeanG = max(0, min(255, nGreen)); //dSumG / nSize;
					double dMeanB = max(0, min(255, nBlue));  //dSumB / nSize;
					
					double dMean = /*dMeanB * 0.114 + dMeanG * 0.587 + dMeanR * 0.299;*/ (dMeanB + dMeanG + dMeanR) / 3;

					BYTE vTable[3][256];

					double dFactorR = dMeanR > 0 ? (dMean / dMeanR) : 1;
					double dFactorG = dMeanG > 0 ? (dMean / dMeanG) : 1;
					double dFactorB = dMeanB > 0 ? (dMean / dMeanB) : 1;

					for( int i = 0; i < 256; i++ )
					{
						vTable[0][i] = RealToByte( i * dFactorB );
						vTable[1][i] = RealToByte( i * dFactorG );
						vTable[2][i] = RealToByte( i * dFactorR );
					}

					BGRA_ApplyColorTableEx( pPixels, nWidth, nHeight, vTable );

					return c_nErrorNone;
				}

				int BGRA_AdjustAutoWhiteBalance( BYTE* pPixels, int nWidth, int nHeight, double dFrame )
				{
					int nSize = nWidth * nHeight;

					BYTE table[3][256];

					double dFactorB = 0;
					double dFactorG = 0;
					double dFactorR = 0;

					double dOffsetB = 0;
					double dOffsetG = 0;
					double dOffsetR = 0;

					double dSumR = 0;
					double dSumG = 0;
					double dSumB = 0;
					double dMean = 0;

					double dCountPixels = 0;

					BYTE* src = pPixels;
					for( int i = 0; i < nSize; ++i, src += 4 )
					{
						int b = src[0];
						int g = src[1];
						int r = src[2];
						int a = src[3];

						if( a > 0 )
						{
							double alpha = a * (1.0/255.0);

							dSumB += b * alpha;
							dSumG += g * alpha;
							dSumR += r * alpha;

							dCountPixels += alpha;
						}
					}

					if( dCountPixels < 0.001 )
						return c_nErrorNone;

					dSumB /= dCountPixels;
					dSumG /= dCountPixels;
					dSumR /= dCountPixels;

					dMean = /*(dSumR * 0.299 + dSumG * 0.587 + dSumB * 0.114);*/ (dSumB + dSumG + dSumR) / 3;

					//dFactorB = dSumB > 0.001 ? (dMean / dSumB ) : 1;
					//dFactorG = dSumG > 0.001 ? (dMean / dSumG ) : 1;
					//dFactorR = dSumR > 0.001 ? (dMean / dSumR ) : 1;

					//if( dMean < 127.5 )
					{
						dFactorB = dSumB > 0 ? (dMean / dSumB) : 1;
						dFactorG = dSumG > 0 ? (dMean / dSumG) : 1;
						dFactorR = dSumR > 0 ? (dMean / dSumR) : 1;

						dOffsetB = 0;
						dOffsetG = 0;
						dOffsetR = 0;
					}
					//else
					//{
					//	dFactorB = (255 - dMean) / (255 - dSumB);
					//	dFactorG = (255 - dMean) / (255 - dSumG);
					//	dFactorR = (255 - dMean) / (255 - dSumR);

					//	dOffsetB = 255 - 255 * dFactorB;
					//	dOffsetG = 255 - 255 * dFactorG;
					//	dOffsetR = 255 - 255 * dFactorR;
					//}
					
					for( int i = 0; i < 256; ++i )
					{
						table[0][i] = RealToByte( i * dFactorB + dOffsetB );
						table[1][i] = RealToByte( i * dFactorG + dOffsetG );
						table[2][i] = RealToByte( i * dFactorR + dOffsetR );
					}

					BGRA_ApplyColorTableEx( pPixels, nWidth, nHeight, table );


					double xHistogramm[256];
					for( int i = 0; i < 256; ++i )
					{
						xHistogramm[i] = 0;
					}

					dSumB = 0;
					dSumG = 0;
					dSumR = 0;

					dCountPixels = 0;

					src = pPixels;
					for( int i = 0; i < nSize; ++i, src += 4 )
					{
						int b = src[0];
						int g = src[1];
						int r = src[2];
						int a = src[3];

						if( a > 0 )
						{
							int index = Intensity3( r, g, b );
							
							double alpha = a * (1.0 / 255.0);
							
							dCountPixels += alpha;
							xHistogramm[index] += alpha;
						}
					}

					double dLimit = dCountPixels * 0.001;
					double dSumma = 0;
					double dCount = 0;

					for( int i = 0; i < 256; ++i )
					{
						dSumma += xHistogramm[255 - i] * (255 - i);
						dCount += xHistogramm[255 - i];

						if( dCount >= dLimit )
							break;
					}

					if( dCount < 0.001 )
						return c_nErrorNone;

					double dFactor = 255 / (dSumma / dCount);

					for( int i = 0; i < 256; ++i )
					{
						table[0][i] = RealToByte( i * dFactor );
					}

					BGRA_ApplyColorTable( pPixels, nWidth, nHeight, table[0] );


					//DWORD nThreshold = DWORD(dSumma / dCount + 0.5);

					//dSumR = 0;
					//dSumG = 0;
					//dSumB = 0;
					//dCount = 0;

					//src = pPixels;
					//for( int i = 0; i < nSize; ++i, src += 4 )
					//{
					//	int b = src[0];
					//	int g = src[1];
					//	int r = src[2];
					//	int a = src[3];

					//	if( a > 0 )
					//	{
					//		DWORD gray = Intensity3( r, g, b );

					//		if( gray < nThreshold )
					//			continue;

					//		double alpha = a * (1.0/255.0);

					//		dSumB += b * alpha;
					//		dSumG += g * alpha;
					//		dSumR += r * alpha;
					//		dCount += alpha;
					//	}
					//}

					//dSumB /= dCount;
					//dSumG /= dCount;
					//dSumR /= dCount;

					//dFactorR = dSumR > 0.001 ? (255 / dSumR ) : 1;
					//dFactorG = dSumG > 0.001 ? (255 / dSumG ) : 1;
					//dFactorB = dSumB > 0.001 ? (255 / dSumB ) : 1;

					//for( int i = 0; i < 256; ++i )
					//{
					//	table[0][i] = RealToByte( i * dFactorB );
					//	table[1][i] = RealToByte( i * dFactorG );
					//	table[2][i] = RealToByte( i * dFactorR );
					//}

					//BGRA_ApplyColorTableEx( pPixels, nWidth, nHeight, table );

					return c_nErrorNone;
				}

				int BGRA_AdjustVibrance( BYTE* pPixels, int nWidth, int nHeight, double dFrame, double dLevel )
				{
					dFrame = Clamp(dFrame);
					dLevel = dLevel / 100.0 * dFrame;

					int nSize = nWidth * nHeight;

					BYTE* src = pPixels;

					for( int i = 0; i < nSize; ++i, src += 4 )
					{
						int b = src[0];
						int g = src[1];
						int r = src[2];

						double gray = Intensity( r, g , b );
						
						b = int( (b - gray) * dLevel + b + 0.5 );
						g = int( (g - gray) * dLevel + g + 0.5 );
						r = int( (r - gray) * dLevel + r + 0.5 );

						src[0] = IntToByte( b );
						src[1] = IntToByte( g );
						src[2] = IntToByte( r );
					}
					
					return c_nErrorNone;
				}


				
				BOOL BGRA_IPPEffectGaussianBlur(LPBYTE pPixels, int nWidth, int nHeight, double Frame, long Size)
				{
					if(Size < 1)
						return TRUE;

					IppiSize roiSize;
					roiSize.width	= nWidth;
					roiSize.height	= nHeight;
					
					int nCount = 2*Size+1;
					Ipp32s* pKernel = new Ipp32s[nCount];

					IppiSize roiBufferExtVSize;
					roiBufferExtVSize.width = nWidth;
					roiBufferExtVSize.height = nHeight + 2*Size;
					Ipp8u* pBufferExtV = new Ipp8u[4*roiBufferExtVSize.width*roiBufferExtVSize.height];

					IppiSize roiBufferExtHSize;
					roiBufferExtHSize.width	= nWidth + 2*Size;
					roiBufferExtHSize.height = nHeight;
					Ipp8u* pBufferExtH = new Ipp8u[4*roiBufferExtHSize.width*roiBufferExtHSize.height];

					if (pKernel && pBufferExtV && pBufferExtH)
					{
						for (int i = 0; i < nCount; ++i)
							pKernel[i] = 1;

						ippiCopyReplicateBorder_8u_C4R(pPixels, 4*nWidth, roiSize, pBufferExtV, 4*roiBufferExtVSize.width, roiBufferExtVSize, Size, 0);
						ippiFilterColumn_8u_C4R((Ipp8u*)(pBufferExtV + Size*(4*roiBufferExtVSize.width)), 4*roiBufferExtVSize.width, pPixels, 4*nWidth, roiSize, pKernel, nCount, Size, nCount);

						ippiCopyReplicateBorder_8u_C4R(pPixels, 4*nWidth, roiSize, pBufferExtH, 4*roiBufferExtHSize.width, roiBufferExtHSize, 0, Size);
						ippiFilterRow_8u_C4R((Ipp8u*)(pBufferExtH + 4*Size), 4*roiBufferExtHSize.width, pPixels, 4*nWidth, roiSize, pKernel, nCount, Size, nCount);
						
						ippiCopyReplicateBorder_8u_C4R(pPixels, 4*nWidth, roiSize,  pBufferExtV, 4*roiBufferExtVSize.width, roiBufferExtVSize, Size, 0);
						ippiFilterColumn_8u_C4R((Ipp8u*)(pBufferExtV + Size*(4*roiBufferExtVSize.width)), 4*roiBufferExtVSize.width, pPixels, 4*nWidth, roiSize, pKernel, nCount, Size, nCount);
						
						ippiCopyReplicateBorder_8u_C4R(pPixels, 4*nWidth, roiSize, pBufferExtH, 4*roiBufferExtHSize.width, roiBufferExtHSize, 0, Size);
						ippiFilterRow_8u_C4R((Ipp8u*)(pBufferExtH + 4*Size), 4*roiBufferExtHSize.width, pPixels, 4*nWidth, roiSize, pKernel, nCount, Size, nCount);
					}
					
					if (pBufferExtH)
						delete[] pBufferExtH;
					if (pBufferExtV)
						delete pBufferExtV;
					if (pKernel)
						delete pKernel;

					/*
					IppiSize kernelSize;
					kernelSize.width  = 2*Size+1;
					kernelSize.height  = kernelSize.width;
					
					int nCount = kernelSize.width*kernelSize.height;
					Ipp32s* pKernel = new Ipp32s[nCount];
						
					pKernel[0] = 1;
					for (int i=1; i<nCount;i++)
						pKernel[i] = 1;

					IppiPoint anchor;
					anchor.x = Size;
					anchor.y = Size;

					IppiSize roiBufferSize;
					roiBufferSize.width = m_nWidth + 2*Size;
					roiBufferSize.height = m_nHeight + 2*Size;
					Ipp8u *pBuffer = new Ipp8u[4*roiBufferSize.width*roiBufferSize.height];
					
					
					ippiCopyReplicateBorder_8u_C4R((Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, pBuffer, 4*roiBufferSize.width, roiBufferSize, Size, Size);	
					ippiFilter_8u_C4R((Ipp8u*)(pBuffer + 4*Size*(roiBufferSize.width+1)), 4*roiBufferSize.width, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, pKernel, kernelSize, anchor, nCount);

					ippiCopyReplicateBorder_8u_C4R((Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, pBuffer, 4*roiBufferSize.width, roiBufferSize, Size, Size);	
					ippiFilter_8u_C4R((Ipp8u*)(pBuffer + 4*Size*(roiBufferSize.width+1)), 4*roiBufferSize.width, (Ipp8u*)m_pPixels, 4*m_nWidth, roiSize, pKernel, kernelSize, anchor, nCount);

					delete pBuffer;
					*/
					return TRUE;
				}
				void IPP_BGRA_GaussianBlurHorizontal( Ipp16u* pMapBGR, Ipp8u* pDstBGRA, int nWidth, int nHeight, int nDstStride, int nMapStride, int nBlurSize )
				{
					Ipp32u nFactor = nBlurSize * 2 + 1;
					nFactor *= nFactor;

					for( ; nHeight > 0; --nHeight, pDstBGRA += nDstStride, pMapBGR += nMapStride )
					{
						Ipp8u*  pDst = pDstBGRA;
						Ipp16u* pMap = pMapBGR;

						// подсчёт начальной суммы
						Ipp32u nB = Ipp32u(pMap[0] * (nBlurSize + 1));
						Ipp32u nG = Ipp32u(pMap[1] * (nBlurSize + 1));
						Ipp32u nR = Ipp32u(pMap[2] * (nBlurSize + 1));

						int nLength = nBlurSize;
						for( ; nLength > 0; --nLength )
						{
							pMap += 3;
							nB += pMap[0];
							nG += pMap[1];
							nR += pMap[2];
						}

						// расчёт новых пикселов в строке
						pMap = pMapBGR;
						nLength = nWidth;
						
						for( ;; )
						{
							pDst[0] = BYTE(nB / nFactor);
							pDst[1] = BYTE(nG / nFactor);
							pDst[2] = BYTE(nR / nFactor);

							// проверка на выход из цикла
							if( nLength <= 1 )
							{
								break;
							}
							pDst += 4; 

							// вычитаем крайнее левое значение
							int index = 3 * ((nLength > nWidth - nBlurSize) ? nWidth - nLength : nBlurSize);
							nB -= pMap[0 - index];
							nG -= pMap[1 - index];
							nR -= pMap[2 - index];

							// переходим на следующий пиксел
							--nLength;
							pMap += 3;
							
							// прибавляем следующее крайнее правое значение
							index = 3 * (nLength > nBlurSize ? nBlurSize : nLength - 1);
							nB += pMap[0 + index];
							nG += pMap[1 + index];
							nR += pMap[2 + index];
						}
					}
				}

				void IPP_BGRA_GaussianBlurVertical(Ipp8u* pSrcBGRA, Ipp16u* pMapBGR, int nWidth, int nHeight, int nSrcStride, int nMapStride, int nBlurSize )
				{
					for( ; nWidth > 0; --nWidth, pSrcBGRA += 4, pMapBGR += 3 )
					{
						Ipp8u*  pSrc = pSrcBGRA;
						Ipp16u* pMap = pMapBGR;

						// подсчёт начальной суммы
						Ipp16u nB = Ipp16u(pSrc[0] * (nBlurSize + 1));
						Ipp16u nG = Ipp16u(pSrc[1] * (nBlurSize + 1));
						Ipp16u nR = Ipp16u(pSrc[2] * (nBlurSize + 1));

						int nLength = nBlurSize;
						while( nLength-- > 0 )
						{
							pSrc += nSrcStride;
							nB += pSrc[0];
							nG += pSrc[1];
							nR += pSrc[2];
						}

						// расчёт новых пикселов в строке
						pSrc = pSrcBGRA;
						nLength = nHeight;
						
						for( ;; )
						{
							pMap[0] = nB;
							pMap[1] = nG;
							pMap[2] = nR;

							// проверка на выход из цикла
							if( nLength <= 1 )
							{
								break;
							}
							pMap += nMapStride;

							// вычитаем крайнее верхнее значение
							int index = nSrcStride * ((nLength > nHeight - nBlurSize) ? nHeight - nLength : nBlurSize);
							nB -= pSrc[0 - index];
							nG -= pSrc[1 - index];
							nR -= pSrc[2 - index];

							// переходим на следующий пиксел
							--nLength;
							pSrc += nSrcStride;
							
							// прибавляем следующее крайнее нижнее значение
							index = nSrcStride * (nLength > nBlurSize ? nBlurSize : nLength - 1);
							nB += pSrc[0 + index];
							nG += pSrc[1 + index];
							nR += pSrc[2 + index];
						}
					}
				}

				BOOL BGRA_IPPEffectGaussianBlur2( BYTE* pPixels, int nWidth, int nHeight, double dFrame, long Size )
				{
					// определяем размер блюра
					if( Size < 1 || nWidth <= 1 || nHeight <= 1 )
						return TRUE;
					
					// создаём вспомогательный массив для ускоренного расчёта блюра
					Ipp16u* pMap = new Ipp16u[nWidth*nHeight*3];
					if( !pMap )
						return TRUE;

					int nImgStride = nWidth * 4;
					int nMapStride = nWidth * 3;

					// первый цикл
					IPP_BGRA_GaussianBlurVertical( pPixels, pMap, nWidth, nHeight, nImgStride, nMapStride, min(Size, nHeight - 1) );
					IPP_BGRA_GaussianBlurHorizontal( pMap, pPixels, nWidth, nHeight, nImgStride, nMapStride, min(Size, nWidth - 1) );

					// второй цикл
					IPP_BGRA_GaussianBlurVertical( pPixels, pMap, nWidth, nHeight, nImgStride, nMapStride, min(Size, nHeight - 1) );
					IPP_BGRA_GaussianBlurHorizontal( pMap, pPixels, nWidth, nHeight, nImgStride, nMapStride, min(Size, nWidth - 1) );

					delete [] pMap;
					
					return TRUE;
				}								
				
				int BGRA_EffectBlur( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Level )
				{
					// clamp values
					Frame = Clamp(Frame);
					Level = min(100, max(0, Level));

					IppiSize roiSize;
					roiSize.width	= nWidth;
					roiSize.height	= nHeight;

					IppiSize kernelSize;
					kernelSize.width  = 3;
					kernelSize.height  = 3;	
					Ipp32f* pKernel = new Ipp32f[kernelSize.width*kernelSize.height];
					Ipp8u *pDst = new Ipp8u[4*(nWidth + 2)*(nHeight + 2)];

					if (pKernel && pDst)
					{
						double dMatrixKoef = Frame / (20 * (1 - Frame * Level / 100) + 9);
						pKernel[0] = (float)dMatrixKoef;
						pKernel[1] = (float)dMatrixKoef;
						pKernel[2] = (float)dMatrixKoef;
						pKernel[3] = (float)dMatrixKoef;
						pKernel[4] = (float)(1 - 8 * dMatrixKoef);
						pKernel[5] = (float)dMatrixKoef;
						pKernel[6] = (float)dMatrixKoef;
						pKernel[7] = (float)dMatrixKoef;
						pKernel[8] = (float)dMatrixKoef;
					
						IppiPoint anchor;
						anchor.x = 1;
						anchor.y = 1;

						IppiSize roiDstSize;
						roiDstSize.width = nWidth + 2;
						roiDstSize.height = nHeight + 2;
						
						ippiCopyReplicateBorder_8u_AC4R((Ipp8u*)pPixels, 4*nWidth, roiSize, pDst, 4*nWidth + 8, roiDstSize, 1, 1);
					
						ippiFilter32f_8u_AC4R((Ipp8u*)(pDst + 4 + 4*roiDstSize.width), 4*roiDstSize.width, (Ipp8u*)pPixels, 4*nWidth, roiSize, pKernel, kernelSize, anchor);
					}

					if (pKernel)
						delete[] pKernel;
					if (pDst)
						delete[] pDst;
					
					return c_nErrorNone;
				}

				int BGRA_EffectSharpen( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Level )
				{
					// clamp values
					Frame = Clamp(Frame);
					Level = sqrt(Clamp(Level / 100.0));

					IppiSize roiSize;
					roiSize.width	= nWidth;
					roiSize.height	= nHeight;

					IppiSize kernelSize;
					kernelSize.width  = 3;
					kernelSize.height  = 3;	
					Ipp32f* pKernel = new Ipp32f[kernelSize.width*kernelSize.height];
					Ipp8u* pDst = new Ipp8u[4*(nWidth + 2)*(nHeight + 2)];

					if (pKernel && pDst)
					{
						double dMatrixKoef = Frame / (23 * (1 - Level) + 1);
						pKernel[0] = (float)(-dMatrixKoef);
						pKernel[1] = (float)(-dMatrixKoef);
						pKernel[2] = (float)(-dMatrixKoef);
						pKernel[3] = (float)(-dMatrixKoef);
						pKernel[4] = (float)(dMatrixKoef * 8 + 1);
						pKernel[5] = (float)(-dMatrixKoef);
						pKernel[6] = (float)(-dMatrixKoef);
						pKernel[7] = (float)(-dMatrixKoef);
						pKernel[8] = (float)(-dMatrixKoef);

						IppiPoint anchor;
						anchor.x = 1;
						anchor.y = 1;

						IppiSize roiDstSize;
						roiDstSize.width = nWidth + 2;
						roiDstSize.height = nHeight + 2;
						
						ippiCopyReplicateBorder_8u_AC4R((Ipp8u*)pPixels, 4*nWidth, roiSize, pDst, 4*nWidth + 8, roiDstSize, 1, 1);
						
						ippiFilter32f_8u_AC4R((Ipp8u*)(pDst + 4 + 4*roiDstSize.width),	4*roiDstSize.width, (Ipp8u*)pPixels, 4*nWidth, roiSize, pKernel, kernelSize, anchor);
					}

					if (pKernel)
						delete[] pKernel;
					if (pDst)
						delete[] pDst;

					return c_nErrorNone;
				}

				int BGRA_EffectMosaic( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Size, BOOL bSimple )
				{
					// clamp values
					Frame = Clamp(Frame);
					Size = (int)(min(255, max(1, Size*Frame)));

					
					IppiSize roiSize;
					roiSize.width	= nWidth;
					roiSize.height	= nHeight;

					int nTilesX = nWidth/Size;
					int nTilesY = nHeight/Size;

					IppiSize roiCellSize;
					roiCellSize.width	= Size;
					roiCellSize.height	= Size;
					IppiSize roiRightBorderCellSize = roiCellSize;
					roiRightBorderCellSize.width	= nWidth - nTilesX*Size;
					IppiSize roiBottomBorderCellSize = roiCellSize;
					roiBottomBorderCellSize.height	= nHeight - nTilesY*Size;
				    
					LPBYTE pPixelsP1 = pPixels;
					LPBYTE pPixelsP2 = pPixels;
					int nStep = 4*nWidth;
					int nStepH = 4*Size;
					int nStepV = nStep*Size;

					if (bSimple)
					{	//значение левого верхнего угла
						Ipp8u min[3];
						if (0 != roiBottomBorderCellSize.height)
						{
							for (int i = 0; i < nTilesX; i++)
							{
								pPixelsP2 = pPixelsP1;
								for (int j = 0; j < nTilesY; j++)
								{
									ippiMin_8u_AC4R(pPixelsP2, nStep, roiCellSize, min);
									ippiSet_8u_AC4R(min, pPixelsP2, nStep, roiCellSize);
									pPixelsP2 += nStepV;
								}

								ippiMin_8u_AC4R(pPixelsP2, nStep, roiBottomBorderCellSize, min);
								ippiSet_8u_AC4R(min, pPixelsP2, nStep, roiBottomBorderCellSize);
								
								pPixelsP1 += nStepH;
							}
						}
						else
						{
							for (int i = 0; i < nTilesX; i++)
							{
								pPixelsP2 = pPixelsP1;
								for (int j = 0; j < nTilesY; j++)
								{
									ippiMin_8u_AC4R(pPixelsP2, nStep, roiCellSize, min);
									ippiSet_8u_AC4R(min, pPixelsP2, nStep, roiCellSize);
									pPixelsP2 += nStepV;
								}
								pPixelsP1 += nStepH;
							}
						}
						if (0 != roiRightBorderCellSize.width)
						{
							pPixelsP2 = pPixelsP1;
							for (int j = 0; j < nTilesY; j++)
							{
								ippiMin_8u_AC4R(pPixelsP2, nStep, roiRightBorderCellSize, min);
								ippiSet_8u_AC4R(min, pPixelsP2, nStep, roiRightBorderCellSize);
								pPixelsP2 += nStepV;
							}

							IppiSize oRightBottomCellSize = {roiRightBorderCellSize.width, roiBottomBorderCellSize.height};

							ippiMin_8u_AC4R(pPixelsP2, nStep, oRightBottomCellSize, min);
							ippiSet_8u_AC4R(min, pPixelsP2, nStep, oRightBottomCellSize);
						}
					}
					else
					{//среднее значение по ячейке
						Ipp64f sum[3];
						Ipp8u value[3];
						int nPixelInCell = roiCellSize.height * roiCellSize.width;

						if( 0 != nPixelInCell )
						{
							if (0 != roiBottomBorderCellSize.height)
							{
								int nPixelInBottomCell = roiBottomBorderCellSize.height * roiBottomBorderCellSize.width;
								for (int i = 0; i < nTilesX; i++)
								{
									pPixelsP2 = pPixelsP1;
									for (int j = 0; j < nTilesY; j++)
									{
										ippiSum_8u_AC4R(pPixelsP2, nStep, roiCellSize, sum);

										value[0] = (Ipp8u)(sum[0]/nPixelInCell);
										value[1] = (Ipp8u)(sum[1]/nPixelInCell);
										value[2] = (Ipp8u)(sum[2]/nPixelInCell);
										
										ippiSet_8u_AC4R(value, pPixelsP2, nStep, roiCellSize);
										pPixelsP2 += nStepV;
									}

									if( 0 != nPixelInBottomCell )
									{
										ippiSum_8u_AC4R(pPixelsP2, nStep, roiBottomBorderCellSize, sum);

										value[0] = Ipp8u(sum[0]/nPixelInBottomCell);
										value[1] = Ipp8u(sum[1]/nPixelInBottomCell);
										value[2] = Ipp8u(sum[2]/nPixelInBottomCell);
										ippiSet_8u_AC4R(value, pPixelsP2, nStep, roiBottomBorderCellSize);
									}
									
									pPixelsP1 += nStepH;
								}
							}
							else
							{
								for (int i = 0; i < nTilesX; i++)
								{
									pPixelsP2 = pPixelsP1;
									for (int j = 0; j < nTilesY; j++)
									{
										ippiSum_8u_AC4R(pPixelsP2, nStep, roiCellSize, sum);
										value[0] = Ipp8u(sum[0]/nPixelInCell);
										value[1] = Ipp8u(sum[1]/nPixelInCell);
										value[2] = Ipp8u(sum[2]/nPixelInCell);
										ippiSet_8u_AC4R(value, pPixelsP2, nStep, roiCellSize);
										pPixelsP2 += nStepV;
									}
									pPixelsP1 += nStepH;
								}
							}
						}
						if (0 != roiRightBorderCellSize.width)
						{
							int nPixelInRightCell = roiRightBorderCellSize.height * roiRightBorderCellSize.width;
							pPixelsP2 = pPixelsP1;
							if( 0 != nPixelInRightCell )
							{
								for (int j = 0; j < nTilesY; j++)
								{
									ippiSum_8u_AC4R(pPixelsP2, nStep, roiRightBorderCellSize, sum);
									value[0] = Ipp8u(sum[0]/nPixelInRightCell);
									value[1] = Ipp8u(sum[1]/nPixelInRightCell);
									value[2] = Ipp8u(sum[2]/nPixelInRightCell);
									ippiSet_8u_AC4R(value, pPixelsP2, nStep, roiRightBorderCellSize);
									pPixelsP2 += nStepV;
								}
							}

							IppiSize oRightBottomCellSize = {roiRightBorderCellSize.width, roiBottomBorderCellSize.height};
							int nPixelInRightBottomCell = roiRightBorderCellSize.width * roiBottomBorderCellSize.height;
							if( 0 != nPixelInRightBottomCell )
							{
								ippiSum_8u_AC4R(pPixelsP2, nStep, oRightBottomCellSize, sum);
								value[0] = Ipp8u(sum[0]/nPixelInRightBottomCell);
								value[1] = Ipp8u(sum[1]/nPixelInRightBottomCell);
								value[2] = Ipp8u(sum[2]/nPixelInRightBottomCell);
								ippiSet_8u_AC4R(value, pPixelsP2, nStep, oRightBottomCellSize);
							}
						}		
					}

					return c_nErrorNone;
				}

				int BGRA_EffectGaussianBlur( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Size )
				{
					// clamp values
					Frame = Clamp(Frame);
					Size = int(Size*Frame + 0.5);

					if( Size <= 0 )
						return c_nErrorNone;

					if( Size > 128 )
						Size = 128;

					//if( Size < 10 )
					//{
					//	BGRA_IPPEffectGaussianBlur( pPixels, nWidth, nHeight, Frame, Size );
					//}
					//else
					//{
						BGRA_IPPEffectGaussianBlur2( pPixels, nWidth, nHeight, Frame, Size );
					//}

					return c_nErrorNone;
				}
#ifndef   IMAGE_EDITOR
				int BGRA_EffectNoise( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Level, BOOL bMono, DWORD nBaseRnd )
				{
					// clamp values
					Frame = Clamp(Frame);
					Level = (int)(min(1000, max(1, Level*Frame)));

					// стандартная версия работает в несколько раз быстрее, чем аналог ipp
					CRandom rnd( nBaseRnd );
					int offset = int(Level);

					int nMin = 0   - offset;
					int nMax = 255 + offset;

					CBuffer<BYTE> oTable( nMax - nMin );
					if( oTable.IsEmpty() )
						return c_nErrorNoMemory;

					BYTE* pTable = oTable.GetPtr();
					for( int i = nMin; i < nMax; ++i, ++pTable )
					{
						if( i < 0 )	  { pTable[0] = 0;   continue; }
						if( i > 255 ) { pTable[0] = 255; continue; }
							          
						pTable[0] = i;
					}

					pTable = oTable.GetPtr() - nMin;

					int nSize = nWidth * nHeight;
					
					if( bMono )
					{
						for( int i = 0; i < nSize; ++i, pPixels += 4 )
						{
							int disp = rnd.Rand( offset * 2 ) - offset;

							pPixels[0] = pTable[pPixels[0] + disp];
							pPixels[1] = pTable[pPixels[1] + disp];
							pPixels[2] = pTable[pPixels[2] + disp];
						}
					}
					else
					{
						for( int i = 0; i < nSize; ++i, pPixels += 4 )
						{
							pPixels[0] = pTable[pPixels[0] + rnd.Rand( offset * 2 ) - offset];
							pPixels[1] = pTable[pPixels[1] + rnd.Rand( offset * 2 ) - offset];
							pPixels[2] = pTable[pPixels[2] + rnd.Rand( offset * 2 ) - offset];
						}
					}

					return c_nErrorNone;
				}

				int BGRA_EffectDiffuse( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Distance, DWORD nBaseRnd )
				{
					CAlphaTable table;
					if( table.Calculate( Frame ) < 0 )
						return c_nErrorNone;

					// clamp values
					if( Distance < 1 )
						return c_nErrorNone;

					// обычная версия без интерполяции работает втрое быстрее ipp
					CBuffer<BYTE> oBuffer( nWidth * nHeight * 4 );
					if( oBuffer.IsEmpty() )
						return c_nErrorNoMemory;

					oBuffer.Fill( pPixels, nWidth * nHeight * 4 );

					int dist = int(Distance);
					BYTE* pBackup = oBuffer.GetPtr();

					CRandom rnd( nBaseRnd );

					for( int nY = 0; nY < nHeight; ++nY )
					{
						for( int nX = 0; nX < nWidth; ++nX, pPixels += 4 )
						{
							int x = nX + rnd.Rand( dist * 2 ) - dist;
							int y = nY + rnd.Rand( dist * 2 ) - dist;

							//if( x < 0 || x >= nWidth ||	y < 0 || y >= nHeight )
							//	continue;

							if( x < 0 ) x = 0; else
							if( x >= nWidth ) x = nWidth - 1;

							if( y < 0 ) y = 0; else
							if( y >= nHeight ) y = nHeight - 1;

							x += y * nWidth;

							if( table.IsEmpty() )
							{
								*(DWORD*)(pPixels) = *(DWORD*)(pBackup + x * 4);
								continue;
							}

							for( int i = 0; i < 4; i++ )
							{
								pPixels[i] = table.Blend( pPixels[i], pBackup[x * 4 + i] );
							}
						}
					}

					return c_nErrorNone;
				}

				int BGRA_EffectMotionBlur( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Angle, double Distance )
				{
					// clamp values
					Frame = Clamp(Frame);

					// compute number of pixels to process
					int nAmount = max(0, min(int(Distance * Frame + 0.5), 255));
					if( !nAmount )
						return c_nErrorNone;

					// allocate memory for offsets
					CBuffer<int> oOffset( nAmount );
					if( oOffset.IsEmpty() )
						return c_nErrorNoMemory;

					Angle *= IPP_PI180;
					double dCos = cos(Angle);
					double dSin = sin(Angle);

					// compute offsets
					int nOffsetX = int((nAmount - 1) * dCos + 0.5);
					int nOffsetY = int((nAmount - 1) * dSin + 0.5);
					int nMaxX = abs(nOffsetX);
					int nMaxY = abs(nOffsetY);

					int* offset = oOffset.GetPtr();
					for( int nPoint = 0; nPoint < nAmount; ++nPoint )
					{
						int x = int(nPoint * dCos + 0.5);
						int y = int(nPoint * dSin + 0.5) * (nWidth + nMaxX);
						offset[nPoint] = x + y;
					}

					CBuffer<BYTE> oBackBuffer((nWidth + nMaxX) * (nHeight + nMaxY) * 4);
					if( oBackBuffer.IsEmpty() )
						return c_nErrorNoMemory;
					
					IppiSize SrcSize = {nWidth, nHeight};
					IppiSize DstSize = {nWidth + nMaxX, nHeight + nMaxY};

					int dx = nOffsetX < 0 ? nMaxX : 0;
					int dy = nOffsetY < 0 ? nMaxY : 0;

					ippiCopyReplicateBorder_8u_C4R( pPixels, nWidth * 4, SrcSize, oBackBuffer.GetPtr(), DstSize.width * 4, DstSize, dy, dx );
					
					BYTE* pBackup = oBackBuffer.GetPtr() + (dy * DstSize.width + dx) * 4;

					// for each pixel - compute new color value
					int nStep = DstSize.width; 
					nHeight *= nStep;
					
					for( int nY = 0; nY < nHeight; nY += nStep )
					{
						for( int nX = 0; nX < nWidth; ++nX, pPixels += 4 )
						{
							int index = nX + nY;

							// initialize color values
							DWORD a = 0;
							DWORD r = 0;
							DWORD g = 0;
							DWORD b = 0;

							// compute new pixel color
							for( int nPoint = 0; nPoint < nAmount; ++nPoint )
							{
								// compute source pixel index
								DWORD color = ((DWORD*)pBackup)[offset[nPoint] + index];

								b += color & 0xff; color >>= 8;
								g += color & 0xff; color >>= 8;
								r += color & 0xff; color >>= 8;
								a += color;
							}

							// save resulting color
							pPixels[0] = BYTE(b / nAmount);
							pPixels[1] = BYTE(g / nAmount);
							pPixels[2] = BYTE(r / nAmount);
							pPixels[3] = BYTE(a / nAmount);
						}
					}

					return c_nErrorNone;
				}

				int BGRA_EffectEmboss( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Angle, double Distance, double Amount, BOOL bGrayScale )
				{
					Frame = Clamp( Frame );

					CAlphaTable table;
					if( table.Calculate( Frame ) < 0 )
						return c_nErrorNone;

					//Amount = min(max(Frame*Amount, 1), 100)/10.0;
					Amount = max(1, min(100, Frame*Amount)) / 10.0;
					
					double dX = Frame * Distance * cos(Angle * IPP_PI180) / 2;
					double dY = Frame * Distance * sin(Angle * IPP_PI180) / 2;

					dX += dX > 0 ? 0.999 : -0.999;
					dY += dY > 0 ? 0.999 : -0.999;

					int nDeltaX = int(dX);
					int nDeltaY = int(dY);

					if( !nDeltaX && !nDeltaY )
					{
						nDeltaX = 1;
					}

					IppiSize IncSize = {abs(nDeltaX), abs(nDeltaY)};
					IppiSize SrcSize = {nWidth, nHeight};
					IppiSize DstSize = {nWidth + IncSize.width * 2, nHeight + IncSize.height * 2};

					int nFactor = int(Amount * 16);
					BYTE HelpTable[512];

					for( int i = 0; i < 256; i++ )
					{
						int c = ((i - 127) * nFactor / 16) + 127;
						
						if( c < 0 )   c = 0; else
						if( c > 255 ) c = 255;

						HelpTable[i * 2 + 1] = HelpTable[i * 2 + 0] = c;
					}
					
					int nPlanes = bGrayScale ? 1 : 4;
					int nOffset = (nDeltaY * DstSize.width + nDeltaX) * nPlanes;
					int nStride = (DstSize.width - nWidth) * nPlanes;

					CBuffer<BYTE> oBackup(DstSize.width * DstSize.height * nPlanes);
					if( !oBackup.GetPtr() )
						return c_nErrorNoMemory;

					BYTE* pBackup = oBackup.GetPtr() + (IncSize.height * DstSize.width + IncSize.width) * nPlanes;

					if( bGrayScale )
					{
						BYTE* pDst = pBackup;
						BYTE* pSrc = pPixels;

						// grayscale
						for( int nY = nHeight; nY > 0; --nY, pDst += nStride )
						{
							for( int nX = nWidth; nX > 0; --nX, pDst += 1, pSrc += 4 )
							{
								pDst[0] = Intensity2( pSrc[2], pSrc[1], pSrc[0] );
							}
						}

						//copy border
						if( IncSize.height > 0 )
						{
							BYTE* pSrc1 = pBackup;
							BYTE* pSrc2 = pSrc1 + DstSize.width * (nHeight - 1);
							BYTE* pDst1 = pSrc1 - DstSize.width;
							BYTE* pDst2 = pSrc2 + DstSize.width;

							for( int nY = IncSize.height; nY > 0; --nY, pDst1 -= DstSize.width, pDst2 += DstSize.width )
							{
								::memcpy( pDst1, pSrc1, nWidth );
								::memcpy( pDst2, pSrc2, nWidth );
							}
						}

						if( IncSize.width > 0 )
						{
							BYTE* pDst1 = pBackup - IncSize.width;
							BYTE* pDst2 = pBackup + nWidth;

							for( int nY = nHeight; nY > 0; --nY, pDst1 += DstSize.width, pDst2 += DstSize.width )
							{
								::memset( pDst1, pDst1[IncSize.width], IncSize.width );
								::memset( pDst2, pDst2[-1], IncSize.width );
							}
						}

						if( IncSize.width > 0 && IncSize.height > 0 )
						{
							pSrc = pBackup;
							pDst = oBackup.GetPtr();

							ippiSet_8u_C1R( pSrc[0], pDst, DstSize.width, IncSize );
							ippiSet_8u_C1R( pSrc[nWidth - 1], pDst + nWidth + IncSize.width, DstSize.width, IncSize );
							
							pSrc += DstSize.width * (nHeight - 1);
							pDst += DstSize.width * (nHeight + IncSize.height);

							ippiSet_8u_C1R( pSrc[0], pDst, DstSize.width, IncSize );
							ippiSet_8u_C1R( pSrc[nWidth - 1], pDst + nWidth + IncSize.width, DstSize.width, IncSize );
						}

						// собственно эффект
						if( table.IsEmpty() )
						{
							for( int nY = 0; nY < nHeight; ++nY, pBackup += nStride )
							{
								for( int nX = 0; nX < nWidth; ++nX, pPixels += 4, pBackup += 1 )
								{
									int c = HelpTable[pBackup[nOffset] - pBackup[0 - nOffset] + 255];

									pPixels[2] = pPixels[1] = pPixels[0] = c;
								}
							}
						}
						else
						{
							for( int nY = 0; nY < nHeight; ++nY, pBackup += nStride )
							{
								for( int nX = 0; nX < nWidth; ++nX, pPixels += 4, pBackup += 1 )
								{
									int c = HelpTable[pBackup[nOffset] - pBackup[0 - nOffset] + 255];
									
									pPixels[0] = table.Blend( pPixels[0], c );
									pPixels[1] = table.Blend( pPixels[1], c );
									pPixels[2] = table.Blend( pPixels[2], c );
								}
							}
						}

						return c_nErrorNone;
					}
					
					ippiCopyReplicateBorder_8u_C4R( pPixels, nWidth * 4, SrcSize, oBackup.GetPtr(), DstSize.width * 4, DstSize, IncSize.width, IncSize.height);

					if( table.IsEmpty() )
					{
						for( int nY = 0; nY < nHeight; ++nY, pBackup += nStride )
						{
							for( int nX = 0; nX < nWidth; ++nX, pPixels += 4, pBackup += 4 )
							{
								pPixels[0] = HelpTable[pBackup[0 + nOffset] - pBackup[0 - nOffset] + 255];
								pPixels[1] = HelpTable[pBackup[1 + nOffset] - pBackup[1 - nOffset] + 255];
								pPixels[2] = HelpTable[pBackup[2 + nOffset] - pBackup[2 - nOffset] + 255];
							}
						}
					}
					else
					{
						for( int nY = 0; nY < nHeight; ++nY, pBackup += nStride )
						{
							for( int nX = 0; nX < nWidth; ++nX, pPixels += 4, pBackup += 4 )
							{
								pPixels[0] = table.Blend( pBackup[0], HelpTable[pBackup[0 + nOffset] - pBackup[0 - nOffset] + 255] );
								pPixels[1] = table.Blend( pBackup[1], HelpTable[pBackup[1 + nOffset] - pBackup[1 - nOffset] + 255] );
								pPixels[2] = table.Blend( pBackup[2], HelpTable[pBackup[2 + nOffset] - pBackup[2 - nOffset] + 255] );
							}
						}
					}

					return c_nErrorNone;
				}
#endif
				int BGRA_EffectMinimal( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Size )
				{
					// clamp values
					Frame = Clamp(Frame);
					Size = Round(Size * Frame);
					if( Size <= 0 )
						return c_nErrorNone;

					if( Size > 255 )
						Size = 255;

					IppiSize SrcSize = {nWidth, nHeight};
					IppiSize BufSize = {nWidth + Size * 2, nHeight + Size * 2};
					IppiPoint anchor = {Size, Size};
					IppiSize maskSize = {Size * 2 + 1, Size * 2 + 1};

					CBuffer<BYTE> buffer(BufSize.width * BufSize.height * 4);
					if( buffer.IsEmpty() )
						return c_nErrorNoMemory;

					BYTE values[3] = {255, 255, 255};
					ippiCopyConstBorder_8u_AC4R( pPixels, nWidth * 4, SrcSize, buffer.GetPtr(), BufSize.width * 4, BufSize, Size, Size, values );

					ippiFilterMin_8u_AC4R( (buffer.GetPtr() + (BufSize.width + 1) * Size * 4), BufSize.width * 4, pPixels, nWidth * 4, SrcSize, maskSize, anchor);
					
					return c_nErrorNone;
				}

				int BGRA_EffectMaximal( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Size )
				{
					// clamp values
					Frame = Clamp(Frame);
					Size = Round(Size * Frame);
					if( Size <= 0 )
						return c_nErrorNone;

					if( Size > 255 )
						Size = 255;

					IppiSize SrcSize = {nWidth, nHeight};
					IppiSize BufSize = {nWidth + Size * 2, nHeight + Size * 2};
					IppiPoint anchor = {Size, Size};
					IppiSize maskSize = {Size * 2 + 1, Size * 2 + 1};

					CBuffer<BYTE> buffer(BufSize.width * BufSize.height * 4);
					if( buffer.IsEmpty() )
						return c_nErrorNoMemory;

					BYTE values[3] = {0, 0, 0};
					ippiCopyConstBorder_8u_AC4R( pPixels, nWidth * 4, SrcSize, buffer.GetPtr(), BufSize.width * 4, BufSize, Size, Size, values );

					ippiFilterMax_8u_AC4R( (buffer.GetPtr() + (BufSize.width + 1) * Size * 4), BufSize.width * 4, pPixels, nWidth * 4, SrcSize, maskSize, anchor);
					
					return c_nErrorNone;
				}

				int BGRA_EffectDeinterlace( BYTE* pPixels, int nWidth, int nHeight, double Frame, BOOL bEven, BOOL bInterpolate )
				{
					// clamp values
					Frame = Clamp(Frame);

					Ipp8u alpha = Ipp8u((1.0 - Frame) * 255);

					IppiSize roiSize = {nWidth, 1};
					
					int nStep = nWidth * 4;
					int nDoubleStep = nStep * 2;
					int nCount = 0;
					Ipp8u* pPrev = NULL;
					Ipp8u* pCurr = NULL;
					Ipp8u* pNext = NULL;
					
					if (bInterpolate)
					{
						Ipp8u* pBuffer = new Ipp8u[nWidth * 4];

						if (pBuffer)
						{
							if (bEven)
							{			
								nCount = nHeight / 2 - 1;
								pCurr  = pPixels;
								pPrev  = pPixels - nStep;
								pNext  = pPixels + nStep;

								// нулевая строка
								ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pNext, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);
								
								pPrev += nDoubleStep;
								pNext += nDoubleStep;
								pCurr += nDoubleStep;
							}
							else
							{
								nCount = (nHeight - 1) / 2;
								pCurr  = pPixels + nStep;
								pPrev  = pPixels;
								pNext  = pPixels + nDoubleStep;
							}

							for (int i = 0; i < nCount; i++)
							{
								ippiAdd_8u_AC4RSfs(pPrev, nStep, pNext, nStep, pBuffer, nStep, roiSize, 1);
								ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pBuffer, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);

								pPrev += nDoubleStep;
								pNext += nDoubleStep;
								pCurr += nDoubleStep;
							}

							delete[] pBuffer;
						}
					}
					else
					{
						if (bEven)
						{			
							nCount = nHeight / 2;
							pCurr  = pPixels;
							pNext  = pPixels + nStep;
						}
						else
						{
							nCount = (nHeight - 1) / 2;
							pCurr  = pPixels + nStep;
							pNext  = pPixels + nDoubleStep;	
						}

						for( int i = nCount; i > 0; --i )
						{
							ippiAlphaCompC_8u_AC4R(pCurr, nStep, alpha, pNext, nStep, 255, pCurr, nStep, roiSize, ippAlphaOver);

							pNext += nDoubleStep;
							pCurr += nDoubleStep;
						}
					}

					return c_nErrorNone;
				}

				int BGRA_EffectMedian( BYTE* pPixels, int nWidth, int nHeight, double Frame, long MedianType )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;

					switch (MedianType)
					{
					case c_nMedianHorizontal3:
						{
							IppiSize roiBuffer;
							roiBuffer.width = nWidth + 2;
							roiBuffer.height = nHeight;

							Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

							if (pBuffer)
							{
								ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 0, 1);
								ippiFilterMedianHoriz_8u_AC4R((Ipp8u*)(pBuffer + 4), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize,ippMskSize3x1);

								Ipp8u alpha = Ipp8u((1.0 - Frame) * 255);
								ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 4), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
							
								delete[] pBuffer;
							}
						}
						break;
					case c_nMedianHorizontal5:
						{
							IppiSize roiBuffer;
							roiBuffer.width = nWidth + 4;
							roiBuffer.height = nHeight;

							Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

							if (pBuffer)
							{
								ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 0, 2);
								ippiFilterMedianHoriz_8u_AC4R((Ipp8u*)(pBuffer + 8), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize,ippMskSize5x1);

								Ipp8u alpha = Ipp8u((1.0 - Frame) * 255);
								ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 8), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
				                		
								delete[] pBuffer;
							}
						}
						break;
					case c_nMedianVertical3:
						{
							IppiSize roiBuffer;
							roiBuffer.width = nWidth;
							roiBuffer.height = nHeight + 2;

							Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

							if (pBuffer)
							{
								ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 1, 0);
								ippiFilterMedianVert_8u_AC4R((Ipp8u*)(pBuffer + 4*roiBuffer.width), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize,ippMskSize1x3);

								Ipp8u alpha = Ipp8u((1.0 - Frame) * 255);
								ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 4*roiBuffer.width), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
						
								delete[] pBuffer;
							}
						}
						break;
					case c_nMedianVertical5:
						{
							IppiSize roiBuffer;
							roiBuffer.width = nWidth;
							roiBuffer.height = nHeight + 4;

							Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

							if (pBuffer)
							{
								ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 2, 0);
								ippiFilterMedianVert_8u_AC4R((Ipp8u*)(pBuffer + 8*roiBuffer.width), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize,ippMskSize1x5);

								Ipp8u alpha = Ipp8u((1.0 - Frame) * 255);
								ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 8*roiBuffer.width), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
						
								delete[] pBuffer;
							}
						}
						break;
					case c_nMedianCross3:
						{
							IppiSize roiBuffer;
							roiBuffer.width = nWidth + 2;
							roiBuffer.height = nHeight + 2;

							Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

							if (pBuffer)
							{
								ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 1, 1);
								ippiFilterMedianCross_8u_AC4R((Ipp8u*)(pBuffer + 4 + 4*roiBuffer.width), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize, ippMskSize3x3);

								Ipp8u alpha = Ipp8u((1.0 - Frame) * 255);
								ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 4 + 4*roiBuffer.width), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
						
								delete[] pBuffer;
							}
						}
						break;
					case c_nMedianCross5:		
						{
							IppiSize roiBuffer;
							roiBuffer.width = nWidth + 4;
							roiBuffer.height = nHeight + 4;

							Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

							if (pBuffer)
							{
								ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 2, 2);		
								ippiFilterMedianCross_8u_AC4R((Ipp8u*)(pBuffer + 8 + 8*roiBuffer.width), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize,ippMskSize5x5);

								Ipp8u alpha = Ipp8u((1.0 - Frame) * 255);
								ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 8 + 8*roiBuffer.width), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
						
								delete[] pBuffer;
							}
						}
						break;

					default:

						IppiSize roiBuffer;
						roiBuffer.width = nWidth + 2*MedianType;
						roiBuffer.height = nHeight + 2*MedianType;

						Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];

						if (pBuffer)
						{
							ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, MedianType, MedianType);
							
							IppiPoint anchor = {MedianType, MedianType};
							IppiSize  maskSize = {2*MedianType + 1, 2*MedianType + 1};
							ippiFilterMedian_8u_AC4R((Ipp8u*)(pBuffer + 4*MedianType + 4*MedianType*roiBuffer.width), 4*roiBuffer.width, pPixels, 4*nWidth, roiSize, maskSize, anchor);

							Ipp8u alpha = Ipp8u((1.0 - Frame) * 255);
							ippiAlphaCompC_8u_AC4R((Ipp8u*)(pBuffer + 4*MedianType + 4*MedianType*roiBuffer.width), 4*roiBuffer.width, alpha, pPixels, 4*nWidth, 255, (Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);
						
							delete[] pBuffer;
						}
					}
					
					return c_nErrorNone;
				}
#ifndef   IMAGE_EDITOR
				int BGRA_EffectChromoKeyRange(BYTE* pBGRA, int nWidth, int nHeight, int nColor1, int nThreshold1, int nColor2, int nThreshold2, double dFrame, BOOL bComposeAlpha, int EdgesSmooth)
				{
					/* @desc: 
					{
						к картинке (pBGRA, nWidth, nHeight) применяется преобразование ChromoKey - удаление цветов,
						попадающих в цветовой диапазон, заданный как интервал (nColor1, nColor2)

						для всех пикселов картинки с цветом, попадающим в интервал (COLORREF nColor1, COLORREF nColor2)
						альфа канал будет занулен, т.е. пикселы станут прозрачными

						dFrame - кадр анимации; измеряется в пределах 0..1;
							0 - эффектне применяется
							1 - эффект применяется "в полную силу"

						bComposeAlpha - флаг, отвечающий за то, смешивать ли прозрачность исходной картинки в chromo маской

						EdgesSmooth	- фильтр для сглаживания граней
					}
					*/

					if (!pBGRA || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					ImageStudio::Colors::Color oColor1; oColor1.FromCOLORREF(nColor1);
					ImageStudio::Colors::Color oColor2; oColor2.FromCOLORREF(nColor2);

					float fHue1, fLightness1, fSaturation1; oColor1.ToHLS(fHue1, fLightness1, fSaturation1);
					float fHue2, fLightness2, fSaturation2; oColor2.ToHLS(fHue2, fLightness2, fSaturation2);

					int nHComponent1 = (int)(fHue1*255.0f/360.0f);
					int nHComponent2 = (int)(fHue2*255.0f/360.0f);

					CChromoKey oChromo;

					oChromo.SetImage(pBGRA, nWidth, nHeight);
					oChromo.DoChromoKey(dFrame, nHComponent1, nThreshold1, nHComponent2, nThreshold2, bComposeAlpha);

					if ( EdgesSmooth > 0 )
					{
						//
						CBuffer<BYTE> AlphaMask ( nWidth * nHeight );
						if( AlphaMask.IsEmpty() )
							return Core::c_nErrorNoMemory;

						{
							BYTE* pAlphaMask	=	AlphaMask.GetPtr();
							BYTE* pSrc			=	pBGRA;

							for( int count		=	nWidth * nHeight; count > 0; --count, pSrc += 4 )
							{
								*pAlphaMask++	=	pSrc[3];
							}
						}

						{

							// Blur
							BYTE* pAlphaMask = AlphaMask.GetPtr();
							Filters::ApplyBlurFilter ( EdgesSmooth, 1, pAlphaMask, nWidth, pAlphaMask, nWidth, nWidth, nHeight );

							BYTE* pSrc = pBGRA;
							for( int count = nWidth * nHeight; count > 0; --count, pSrc += 4, pAlphaMask += 1 )
							{
								pSrc[3] = pAlphaMask [0];
							}
						}
					}

					return c_nErrorNone;
				}
				int BGRA_EffectChromoKeyLevel(BYTE* pBGRA, int nWidth, int nHeight, int nColor, DWORD nThreshold, double dFrame, BOOL bComposeAlpha, int EdgesSmooth)
				{
					/* @desc: 
					{
						к картинке (pBGRA, nWidth, nHeight) применяется преобразование ChromoKey - удаление цветов,
						попадающих в цветовой диапазон, заданный как интервал (nColor - nThreshold, nColor + nThreshold)

						для всех пикселов картинки с цветом, попадающим в указанный цветовой интервал,
						альфа канал будет занулен, т.е. пикселы станут прозрачными

						dFrame - кадр анимации; измеряется в пределах 0..1;
							0 - эффектне применяется
							1 - эффект применяется "в полную силу"

						bComposeAlpha - флаг, отвечающий за то, смешивать ли прозрачность исходной картинки в chromo маской
						
						EdgesSmooth	- фильтр для сглаживания граней
					}
					*/

					if (!pBGRA || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					//int nMaxBound = Round(dFrame * 456 * nThreshold / 100);
					double nMaxBound = dFrame * 0.007 * nThreshold / 100;

					int bgr[3] = {
						GetBValue( nColor ),
						GetGValue( nColor ),
						GetRValue( nColor ),
					};

					double clr[3] = {0, 0, 0};

					int summa = bgr[0] + bgr[1] + bgr[2];
					if( summa > 0 )
					{
						clr[0] = double(bgr[0]) / summa;
						clr[1] = double(bgr[1]) / summa;
						clr[2] = double(bgr[2]) / summa;
					}
					
					
					int nSize = nWidth * nHeight;

					//if( !EdgesSmooth  )
					{
						for( int i = 0; i < nSize; ++i, pBGRA += 4 )
						{
							if( !pBGRA[3] )
								continue;

							double B = pBGRA[0];// - bgr[0];
							double G = pBGRA[1];// - bgr[1];
							double R = pBGRA[2];// - bgr[2];
							double S = B + G + R;
							if( S > 0 )
							{
								B /= S;
								G /= S;
								R /= S;
							}

							B -= clr[0];
							G -= clr[1];
							R -= clr[2];

							double dist = B * B + G * G + R * R;

							//int dist = B * B + G * G + R * R;
							
							if( dist <= nMaxBound )
							{
								pBGRA[3] = 0;
							}
						}

						return c_nErrorNone;
					}


					//CBuffer<BYTE> AlphaMask( nSize );
					//if( AlphaMask.IsEmpty() )
					//	return Core::c_nErrorNoMemory;

					//BYTE* pAlphaMask = AlphaMask.GetPtr();

					//for( int i = 0; i < nSize; ++i, pBGRA += 4, ++pAlphaMask )
					//{
					//	pAlphaMask[0] = 255;

					//	if( !pBGRA[3] )
					//		continue;

					//	int B = pBGRA[0] - bgr[0];
					//	int G = pBGRA[1] - bgr[1];
					//	int R = pBGRA[2] - bgr[2];

					//	int dist = B * B + G * G + R * R;
					//	
					//	if( dist <= nMaxBound )
					//	{
					//		pAlphaMask[0] = 0;
					//	}
					//}

					//// Blur
					//pAlphaMask -= nSize;
					//pBGRA -= nSize * 4;

					//Filters::ApplyBlurFilter( EdgesSmooth, 1, pAlphaMask, nWidth, pAlphaMask, nWidth, nWidth, nHeight );

					//for( int i = 0; i < nSize; ++i, pBGRA += 4, pAlphaMask += 1 )
					//{
					//	pBGRA[3] = BYTE((pBGRA[3] * pAlphaMask[0] + 128) * 0x8081U >> 23);
					//}

					//return c_nErrorNone;
				}
				int BGRA_EffectCinema(BYTE* pBGRA, int nWidth, int nHeight, int nStride, Paint::Effects::Cinema* pEffect, double dTime, double dFrame, int nBrightness, int nContrast )
				{
					if (!pBGRA || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					if( pEffect )
					{
						BGRA_AdjustBrightness( pBGRA, nWidth, nHeight, dFrame, nBrightness);
						BGRA_AdjustContrast( pBGRA, nWidth, nHeight, dFrame, nContrast);
						pEffect->Draw( pBGRA, nWidth, nHeight, nStride, dTime, dFrame );
					}

					return c_nErrorNone;
				}
				int BGRA_EffectGlass( BYTE* pPixels, int nWidth, int nHeight, double Frame, long nRadius )
				{
					// clamp values
					Frame = Clamp(Frame);
					nRadius = int( nRadius * Frame + 0.5 );

					if( nRadius < 1 )
						return c_nErrorNone;

					if( nRadius > 50 )
						nRadius = 50;

					// max cell size 2800x2800 for integer and 4096x4096 for dword
					struct TChoice
					{
						int count;
						int blue;
						int green;
						int red;
						int cycle;
					};
					
					CBuffer<TChoice> oChoices( 256 );
					if( oChoices.IsEmpty() )
						return c_nErrorNoMemory;
					
					int nCountChoices = (nRadius * 2) * (nRadius * 2);

					CBuffer<BYTE> oIntensities( nCountChoices );
					if( oIntensities.IsEmpty() )
						return c_nErrorNoMemory;

					TChoice* pChoices     = oChoices.GetPtr();
					BYTE*    pIntensities = oIntensities.GetPtr();

					CRandom rnd( 1000 );

					::memset( pChoices, 0, sizeof(TChoice)*256 );
					int nCycles = 0;

					for( int nY = 0; nY < nHeight; nY += nRadius )
					{
						int top    = nY;
						int bottom = nY + nRadius - 1;

						if( bottom >= nHeight)
							bottom = nHeight - 1;

						for( int nX = 0; nX < nWidth; nX += nRadius, nCycles += 1 )
						{
							int left  = nX;
							int right = nX + nRadius - 1;

							if( right >= nWidth )
								right = nWidth - 1;

							nCountChoices = 0;
							//::memset( pChoices, 0, sizeof(TChoice)*256 );

							for( int j = top; j <= bottom + nRadius; ++j )
							{
								if( j >= nHeight )
									break;

								for( int i = left; i <= right + nRadius; ++i )
								{
									if( i >= nWidth )
										break;
									
									BYTE* pPixel = pPixels + (j * nWidth + i)*4;

									int intensity = Intensity2( pPixel[2], pPixel[1], pPixel[0]);

									pIntensities[nCountChoices++] = intensity;

									TChoice& choice = pChoices[intensity];
									if( choice.cycle != nCycles )
									{
										choice.count = 1;
										choice.blue  = pPixel[0];
										choice.green = pPixel[1];
										choice.red   = pPixel[2];
										choice.cycle = nCycles;
										continue;
									}
									
									choice.count += 1;
									choice.blue  += pPixel[0];
									choice.green += pPixel[1];
									choice.red   += pPixel[2];
								}
							}

							for( int j = top; j <= bottom; ++j )
							{
								for( int i = left; i <= right; ++i )
								{
									BYTE*    pPixel = pPixels + (j * nWidth + i)*4;
									TChoice& choice = pChoices[ pIntensities[rnd.Rand( nCountChoices )]];

									pPixel[0] = choice.blue  / choice.count;
									pPixel[1] = choice.green / choice.count;
									pPixel[2] = choice.red   / choice.count;
								}
							}
						}
					}

					return c_nErrorNone;
				}
				int BGRA_EffectGlassMosaic( BYTE* pPixels, int nWidth, int nHeight, int nStride, int eQuality, double dSizeX, double dSizeY, double dOffsetX, double dOffsetY, double Curvature, double Frame, double Angle, double Smooth = 1.3 )
				{
					Frame = Clamp( Frame );
					Curvature = max(-100, min(100, Curvature));
					
					double sinVal = sin(Angle * Frame * IPP_PI180);
					double cosVal = cos(Angle * Frame * IPP_PI180);
					dSizeX = IPP_PI / (dSizeX > 1 ? dSizeX : 1);
					dSizeY = IPP_PI / (dSizeY > 1 ? dSizeY : 1);
					
					Curvature *= abs(Curvature) * 0.1 * Frame;

					dOffsetX = nWidth  / 2.0 + dOffsetX * Frame;
					dOffsetY = nHeight / 2.0 - dOffsetY * Frame;

					DWORD nLevel = 4;
					switch( eQuality )
					{
					case 0: nLevel = 2; break;
					case 1: nLevel = 3; break;
					case 2: nLevel = 4; break;
					case 3: nLevel = 5; break;
					case 4: nLevel = 6; break;
					default: eQuality = 2;
					}
					
					DWORD nSamples = nLevel * nLevel + 1;
					CBuffer<Ipp32fc> oPoints(nSamples);
					if( oPoints.IsEmpty() )
						return c_nErrorNoMemory;

					Smooth = max(0, min(Smooth, 10));
					Smooth = (Smooth - 1) * Frame + 1;

					Ipp32fc* pTable = oPoints.GetPtr();
					for( DWORD sample = 0; sample < nSamples; ++sample, ++pTable )
					{
						double y = double(sample) / nSamples;
						double x = y * nLevel;

						x -= (int)x;

						pTable->re = float((sinVal * y + cosVal * x) * Smooth);
						pTable->im = float((cosVal * y - sinVal * x) * Smooth);
					}

					// буфер изображения
					CBuffer<BYTE> oBackup( nWidth*nHeight*4 );
					BYTE* pBackup = oBackup.GetPtr();
					if( !pBackup )
						return c_nErrorNoMemory;
					
					memcpy( pBackup, pPixels, nWidth*nHeight*4 );

					// вспомогательная таблица
					struct TTangents
					{
						double tan1, ang1;
						double tan2, ang2;
					};

					TTangents* pHelpTable = (TTangents*)ippiMalloc_32fc_C4( (nWidth + nHeight) * nSamples, 1, (int*)&Angle );
					if( !pHelpTable )
						return c_nErrorNoMemory;
					
					//CBuffer<TTangents> oHelpTable( (nWidth + nHeight) * nSamples );
					//if( oHelpTable.IsEmpty() )
					//	return c_nErrorNoMemory;

					TTangents* pHelpX = pHelpTable;
					for( int i = 0; i < nWidth; ++i )
					{
						double pos = i - dOffsetX;
						
						Ipp32fc* pTable = oPoints.GetPtr();
						for( DWORD sample = 0; sample < nSamples; ++sample, ++pHelpX, ++pTable )
						{
							double ux = pos + pTable->re;
							pHelpX->ang1 = ux * cosVal;
							pHelpX->tan1 = ::tan( pHelpX->ang1 * dSizeX );
							
							pHelpX->ang2 = -ux * sinVal;
							pHelpX->tan2 = ::tan( pHelpX->ang2 * dSizeY );
						}
					}
					
					TTangents* pHelpY = pHelpTable + nWidth * nSamples;
					for( int i = 0; i < nHeight; ++i )
					{
						double pos = i - dOffsetY;

						Ipp32fc* pTable = oPoints.GetPtr();
						for( DWORD sample = 0; sample < nSamples; ++sample, ++pHelpY, ++pTable )
						{
							double vy = pos - pTable->im;
							pHelpY->ang1 = vy * sinVal;
							pHelpY->tan1 = ::tan( pHelpY->ang1 * dSizeX );

							pHelpY->ang2 = vy * cosVal;
							pHelpY->tan2 = ::tan( pHelpY->ang2 * dSizeY );
						}
					}

					// begin effect
					pHelpY = pHelpTable + nWidth * nSamples;
					for( int nY = 0; nY < nHeight; ++nY, pHelpY += nSamples )
					{
						pHelpX = pHelpTable;
						for( int nX = 0; nX < nWidth; ++nX, pHelpY -= nSamples, pPixels += 4 )
						{
							Ipp32sc* pTable = (Ipp32sc*)oPoints.GetPtr();
							
							for( DWORD i = 0; i < nSamples; ++i, ++pHelpX, ++pHelpY, ++pTable )
							{
								double sx = Curvature * (pHelpY->tan1 + pHelpX->tan1) / (1 - pHelpY->tan1 * pHelpX->tan1) + pHelpY->ang1 + pHelpX->ang1;
								double ty = Curvature * (pHelpY->tan2 + pHelpX->tan2) / (1 - pHelpY->tan2 * pHelpX->tan2) + pHelpY->ang2 + pHelpX->ang2;

								// undo rotate
								double dx = abs(cosVal * sx - sinVal * ty + dOffsetX);
								double dy = abs(sinVal * sx + cosVal * ty + dOffsetY);
								
								pTable->re = int(dx);
								pTable->im = int(dy);
							}

							DWORD b = 0;
							DWORD g = 0;
							DWORD r = 0;
							DWORD a = 0;

							pTable = (Ipp32sc*)oPoints.GetPtr();
							
							for( int i = nSamples; i > 0; --i, ++pTable )
							{
								int xSample = pTable->re;
								if( xSample >= nWidth )
								{
									xSample %= nWidth * 2;
									if( xSample >= nWidth )
										xSample = nWidth * 2 - 1 - xSample;
								}

								int ySample = pTable->im;
								if( ySample >= nHeight )
								{
									ySample %= nHeight * 2;
									if( ySample >= nHeight )
										ySample = nHeight * 2 - 1 - ySample;
								}

								// interpolate
								DWORD nColor = ((DWORD*)pBackup)[(xSample + ySample*nWidth)];
								b += nColor & 0xff; nColor >>= 8;
								g += nColor & 0xff; nColor >>= 8;
								r += nColor & 0xff; nColor >>= 8;
								a += nColor;

								//BYTE* src = pBackup + (xSample + ySample*nWidth)*4;
								//b += src[0];
								//g += src[1];
								//r += src[2];
								//a += src[3];
							}

							switch( eQuality )
							{
							case 0:	pPixels[0] = BYTE(b * 0xcccdU >> 18); pPixels[1] = BYTE(g * 0xcccdU >> 18); pPixels[2] = BYTE(r * 0xcccdU >> 18); pPixels[3] = BYTE(a * 0xcccdU >> 18);	break; // 5
							case 1:	pPixels[0] = BYTE(b * 0xcccdU >> 19); pPixels[1] = BYTE(g * 0xcccdU >> 19); pPixels[2] = BYTE(r * 0xcccdU >> 19); pPixels[3] = BYTE(a * 0xcccdU >> 19);	break; // 10
							case 2:	pPixels[0] = BYTE(b * 0xf0f1U >> 20); pPixels[1] = BYTE(g * 0xf0f1U >> 20); pPixels[2] = BYTE(r * 0xf0f1U >> 20); pPixels[3] = BYTE(a * 0xf0f1U >> 20);	break; // 17
							case 3:	pPixels[0] = BYTE(b / 26); pPixels[1] = BYTE(g / 26); pPixels[2] = BYTE(r / 26); pPixels[3] = BYTE(a / 26);	break; // 26
							case 4:	pPixels[0] = BYTE(b / 37); pPixels[1] = BYTE(g / 37); pPixels[2] = BYTE(r / 37); pPixels[3] = BYTE(a / 37);	break; // 37
							}

							//pPixels[0] = BYTE(b / nSamples);
							//pPixels[1] = BYTE(g / nSamples);
							//pPixels[2] = BYTE(r / nSamples);
							//pPixels[3] = BYTE(a / nSamples);
						}
					}

					ippiFree( pHelpTable );

					//float halfW = nWidth / 2.0f;
					//float halfH = nHeight / 2.0f;
					//float sinVal = (float)sin(Angle * M_PI / 180.0);
					//float cosVal = (float)cos(Angle * M_PI / 180.0);
					//if (Size<1)
					//	Size=10;
					//float scale = (float)(M_PI / (double)Size);
					//float intensity = (float)Curvature;

					//intensity = intensity * intensity / 10.0f * ((intensity>0.0f)?1.0f:-1.0f);

					//int aaLevel = 4;
					//int aaSamples = aaLevel * aaLevel + 1;
					//PointF* aaPoints = new PointF[aaSamples];

					//for (int i = 0; i < aaSamples; ++i)
					//{
					//	double x = (double)(i * aaLevel) / (double)aaSamples;
					//	double y = (double)i / (double)aaSamples;

					//	x -= (int)x;

					//	aaPoints[i] = PointF((float)(cosVal * x + sinVal * y), (float)(cosVal * y - sinVal * x));
					//}

					//// begin effect
					//BYTE* pBackup = new BYTE[nWidth*nHeight*4];
					//memcpy( pBackup, pPixels, nWidth*nHeight*4 );

					//for (int y = 0; y < nHeight; y++)
					//{
					//	float j = y - halfH;
					//	
					//	for (int x = 0; x < nWidth; x++)
					//	{
					//		int b = 0;
					//		int g = 0;
					//		int r = 0;
					//		int a = 0;
					//		float i = x - halfW;

					//		for (int p = 0; p < aaSamples; ++p)
					//		{
					//			PointF pt = aaPoints[p];

					//			float u = i + pt.X;
					//			float v = j - pt.Y;

					//			float s =  cosVal * u + sinVal * v;
					//			float t = -sinVal * u + cosVal * v;

					//			s += intensity * (float)tan(s * scale);
					//			t += intensity * (float)tan(t * scale);
					//			u = cosVal * s - sinVal * t;
					//			v = sinVal * s + cosVal * t;

					//			int xSample = (int)(halfW + u);
					//			int ySample = (int)(halfH + v);

					//			xSample = (xSample + nWidth) % nWidth;
					//			if (xSample < 0) 
					//			{
					//				xSample = (xSample + nWidth) % nWidth;
					//			}

					//			ySample = (ySample + nHeight) % nHeight;
					//			if (ySample < 0) 
					//			{
					//				ySample = (ySample + nHeight) % nHeight;
					//			}

					//			int  nSrcIndex = 4*(xSample + ySample*nWidth);

					//			b += pBackup[nSrcIndex+0];
					//			g += pBackup[nSrcIndex+1];
					//			r += pBackup[nSrcIndex+2];
					//			a += pBackup[nSrcIndex+3];
					//		}

					//		int  nDestIndex = 4*(x + y*nWidth);

					//		pPixels[nDestIndex+0] = (BYTE)((float)b / (float)aaSamples);
					//		pPixels[nDestIndex+1] = (BYTE)((float)g / (float)aaSamples);
					//		pPixels[nDestIndex+2] = (BYTE)((float)r / (float)aaSamples);
					//		pPixels[nDestIndex+3] = (BYTE)((float)a / (float)aaSamples);
					//		
					//	}
					//}

					//delete[] pBackup;
					//delete[] aaPoints;

					return c_nErrorNone;
				}
				int BGRA_EffectWaterColor( BYTE* pPixels, int nWidth, int nHeight, double Frame, int BrushSize, int Coarseness)
				{
					struct TGrayPeak
					{
						DWORD count;
						DWORD blue;
						DWORD green;
						DWORD red;
					};

					
					BrushSize  = Round( BrushSize  * Frame );
					Coarseness = Round( Coarseness * Frame );

					BrushSize  = min(8,  max(1, BrushSize));
					Coarseness = min(50, max(3, Coarseness));

					CBuffer<TGrayPeak> oPeaks( Coarseness + 1 );
					CBuffer<BYTE> oBackup( nWidth * nHeight * 4 );
					CBuffer<BYTE> oBuffer( nWidth * nHeight );

					if( oBackup.IsEmpty() || oBuffer.IsEmpty() || oPeaks.IsEmpty() )
						return c_nErrorNoMemory;

					TGrayPeak* pPeaks = oPeaks.GetPtr();
					BYTE* pBackup = oBackup.GetPtr();
					BYTE* pBuffer = oBuffer.GetPtr();

					memcpy( pBackup, pPixels, nWidth*nHeight*4 );
					for( int count = nWidth*nHeight*4; count > 0; count -= 4, pBackup += 4 )
					{
						*pBuffer++ = Intensity2(pBackup[2], pBackup[1], pBackup[0]);
					}
					
					pBackup = oBackup.GetPtr();
					pBuffer = oBuffer.GetPtr();
					BYTE* pResult = pPixels;

					for( int nY = 0; nY < nHeight; ++nY )
					{
						int top = nY - BrushSize;
						if (top < 0)
							top = 0;

						int bottom = nY + BrushSize + 1;
						if (bottom > nHeight)
							bottom = nHeight;

						top *= nWidth;
						bottom *= nWidth;

						for( int nX = 0; nX < nWidth; ++nX, pResult += 4 )
						{
							int left = nX - BrushSize;
							if (left < 0)
								left = 0;

							int right = nX + BrushSize + 1;
							if (right > nWidth)
								right = nWidth;

							memset( pPeaks, 0, sizeof(TGrayPeak) * (Coarseness + 1) );
							DWORD nMaxPeaks = 0;
							int   nMaxIndex = 0;

							for( int row = top; row < bottom; row += nWidth )
							{
								for( int pos = left; pos < right; pos += 1 )
								{
									int pixel = row + pos;
									int index = (int(pBuffer[pixel]) + 1) * Coarseness >> 8;

									TGrayPeak& peak = pPeaks[index];
									if( ++peak.count >= nMaxPeaks )
									{
										nMaxPeaks = peak.count;
										nMaxIndex = index;
									}

									peak.blue  += pBackup[pixel*4 + 0];
									peak.green += pBackup[pixel*4 + 1];
									peak.red   += pBackup[pixel*4 + 2];
								}
							}

							pResult[0] = BYTE(pPeaks[nMaxIndex].blue  / nMaxPeaks);
							pResult[1] = BYTE(pPeaks[nMaxIndex].green / nMaxPeaks);
							pResult[2] = BYTE(pPeaks[nMaxIndex].red   / nMaxPeaks);
						}
					}

					//// clamp values
					//Frame = Clamp(Frame);
					//int brushSize = min(8, max(1, BrushSize));
					//int coarseness = min(50, max(3, Coarseness));
					//int arrayLens = 1 + coarseness;
					//int*		  intensityCount   = new int[arrayLens];
					//unsigned int* avgRed		   = new unsigned int[arrayLens];
					//unsigned int* avgGreen		   = new unsigned int[arrayLens];
					//unsigned int* avgBlue		   = new unsigned int[arrayLens];
					////unsigned int* avgAlpha		   = new unsigned int[arrayLens];
					//
					//BYTE maxIntensity = coarseness;

					//int nX, nY, nIndex;
					//nIndex = 0;

					//BYTE* pBackup = new BYTE[nWidth*nHeight*4];
					//memcpy( pBackup, pPixels, nWidth*nHeight*4 );

					//for (nY = 0; nY < nHeight; ++nY)
					//{
					//	int top = nY - brushSize;
					//	int bottom = nY + brushSize + 1;

					//	if (top < 0)
					//		top = 0;

					//	if (bottom > nHeight)
					//		bottom = nHeight;

					//	for (nX = 0; nX < nWidth; ++nX, nIndex += 4)
					//	{
					//		memset(intensityCount,0,sizeof(int)*arrayLens);
					//		memset(avgRed,0,sizeof(unsigned int)*arrayLens);
					//		memset(avgGreen,0,sizeof(unsigned int)*arrayLens);
					//		memset(avgBlue,0,sizeof(unsigned int)*arrayLens);
					//		//memset(avgAlpha,0,sizeof(unsigned int)*arrayLens);

					//		int left = nX - brushSize;
					//		int right = nX + brushSize + 1;

					//		if (left < 0)
					//			left = 0;

					//		if (right > nWidth)
					//			right = nWidth;

					//		for (int j = top; j < bottom; ++j)
					//		{
					//			if (j < 0 || j >= nHeight)
					//				continue;

					//			for (int i = left; i < right; ++i)
					//			{
					//				int  nSourceIndex = 4*(i + j*nWidth);

					//				//BYTE alphaVal = pBackup[nSourceIndex + 3];
					//				BYTE redVal = pBackup[nSourceIndex + 2];
					//				BYTE greenVal = pBackup[nSourceIndex + 1];
					//				BYTE blueVal = pBackup[nSourceIndex + 0];
					//				BYTE intensity = Intensity(redVal, greenVal, blueVal);

					//				float tmpFlVal = (float)(intensity) * (float)(maxIntensity);
					//				BYTE current_intens = (BYTE)( tmpFlVal/ 255.0f);
					//				++intensityCount[current_intens];
					//				
					//				avgRed[current_intens] += redVal;
					//				avgGreen[current_intens] += greenVal;
					//				avgBlue[current_intens] += blueVal;
					//				//avgAlpha[current_intens] += alphaVal;
					//			}
					//		}

					//		BYTE chosenIntensity = 0;
					//		float maxInstance = 0.0;

					//		for (BYTE i = 0; i <= maxIntensity; ++i)
					//		{
					//			if (intensityCount[i] > maxInstance)
					//			{
					//				chosenIntensity = i;
					//				maxInstance = (float)intensityCount[i];
					//			}
					//		}

					//		if (maxInstance<0.1f)
					//			maxInstance = 1.0f;

					//		BYTE R = (BYTE)(((float)avgRed[chosenIntensity]) / maxInstance);
					//		BYTE G = (BYTE)(((float)avgGreen[chosenIntensity]) / maxInstance);
					//		BYTE B = (BYTE)(((float)avgBlue[chosenIntensity]) / maxInstance);
					//		//BYTE A = (BYTE)(((float)avgAlpha[chosenIntensity]) / maxInstance);

					//		//pPixels[nIndex + 3] = A;
					//		pPixels[nIndex + 2] = R;
					//		pPixels[nIndex + 1] = G;
					//		pPixels[nIndex + 0] = B;

					//	}
					//}

					//delete[] pBackup;
					//
					//delete[] intensityCount;
					//delete[] avgRed;
					//delete[] avgGreen;
					//delete[] avgBlue;
					////delete[] avgAlpha;

					return c_nErrorNone;
				}


				int BGRA_EffectSetAlphaChannel( BYTE* pPixels, int nWidth, int nHeight, double Frame, BYTE* pBGRAMask, int nMaskWidth, int nMaskHeight, int nType, double dDstX, double dDstY, double dDstWidth, double dDstHeight )
				{
					Frame = Clamp( Frame );

					int nX1 = Round( dDstX );
					int nY1 = Round( dDstY );
					int nX2 = Round( dDstX + dDstWidth );
					int nY2 = Round( dDstY + dDstHeight );

					if( nX2 == nX1 || nY2 == nY1 )
						return c_nErrorNone;

					if( nX2 < nX1 ) Exchange( &nX2, &nX1 );
					if( nY2 < nY1 ) Exchange( &nY2, &nY1 );

					if( nX2 <= 0 || nX1 >= nWidth ||
						nY2 <= 0 || nY1 >= nHeight )
						return c_nErrorNone;

					if( nMaskWidth == nWidth && nMaskHeight == nHeight &&
						nX1 == 0 && nY1 == 0 && nX2 == nWidth && nY2 == nHeight )
					{
						int nSize = nWidth * nHeight;

						if( 0 == nType ) // альфаканал из маски
						{
							for( int i = 0; i < nSize; ++i, pPixels += 4, pBGRAMask += 4 )
							{
								pPixels[3] = (BYTE)((DWORD(pPixels[3]) *  pBGRAMask[3] + 128) * 0x8081U >> 23);
							}
						}
						else
						if( 1 == nType ) // альфаканал - оттенок серого
						{
							for( int i = 0; i < nSize; ++i, pPixels += 4, pBGRAMask += 4 )
							{
								pPixels[3] = (BYTE)((DWORD(pPixels[3]) * Intensity3( pBGRAMask[2], pBGRAMask[1], pBGRAMask[0] ) + 128) * 0x8081U >> 23);
							}
						}

						return c_nErrorNone;
					}


					CBuffer<BYTE> oBuffer( nMaskWidth * nMaskHeight );
					if( oBuffer.IsEmpty() )
						return c_nErrorNoMemory;

					BYTE* src = pBGRAMask;
					BYTE* dst = oBuffer.GetPtr();

					int nSize = nMaskWidth * nMaskHeight;

					if( 0 == nType )
					{
						for( int i = 0; i < nSize; ++i, src += 4, dst += 1 )
						{
							dst[0] = src[3];
						}
					}
					else
					if( 1 == nType )
					{
						for( int i = 0; i < nSize; ++i, src += 4, dst += 1 )
						{
							dst[0] = (BYTE)Intensity3( src[2], src[1], src[0] );
						}
					}
					else
					{
						return c_nErrorNone;
					}

					int nNewWidth  = nX2 - nX1;
					int nNewHeight = nY2 - nY1;
					
					CBuffer<BYTE> oNewBuffer( nNewWidth * nNewHeight );
					if( oNewBuffer.IsEmpty() )
						return c_nErrorNoMemory;

					//IppiSize SrcSize = {nMaskWidth, nMaskHeight};
					//IppiRect SrcRect = {0, 0, nMaskWidth, nMaskHeight};
					//IppiSize DstSize = {nNewWidth, nNewHeight};

					double xFactor = double(nMaskWidth)  / double(nNewWidth);
					double yFactor = double(nMaskHeight) / double(nNewHeight);

					src = oBuffer.GetPtr();
					dst = oNewBuffer.GetPtr();

					for( int nRow = 0; nRow < nNewHeight; ++nRow )
					{
						int y = int(nRow * yFactor + 0.5);

						for( int nPos = 0; nPos < nNewWidth; ++nPos, dst += 1 )
						{
							int x = int(nPos * xFactor + 0.5);

							dst[0] = src[nMaskWidth * y + x];
						}
					}

					//ippiResize_8u_C1R( oBuffer.GetPtr(), SrcSize, nMaskWidth * 4, SrcRect,
					//	oNewBuffer.GetPtr(), nNewWidth * 4, DstSize, xFactor, yFactor, IPPI_INTER_NN );

					int nMinX = max(nX1, 0);
					int nMaxX = min(nX2, nWidth);
					int nMinY = max(nY1, 0);
					int nMaxY = min(nY2, nHeight);

					src = oNewBuffer.GetPtr() + (nX1 - nMinX);
					dst = pPixels + nMinX * 4;

					int nW = nMaxX - nMinX;
					int nH = nMaxY - nMinY;
					int nSrcStride = nNewWidth - nW;
					int nDstStride = (nWidth - nW) * 4;

					for( int y = 0; y < nH; ++y, src += nSrcStride, dst += nDstStride )
					{
						for( int x = 0; x < nW; ++x, src += 1, dst += 4 )
						{
							dst[3] = (BYTE)((DWORD(dst[3]) *  src[0] + 128) * 0x8081U >> 23);
						}
					}

					return c_nErrorNone;
				}


#endif
				int BGRA_EffectGlow( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Level )
				{
					// эффект glow основан на том, что каждая цветовая компонента
					// является альфой по отношению к самой себе
					// c' = c * (alpha + 1) / 256;  // c[0; 255], alpha[0; 255]
					
					Frame = max(0, min(1, Frame));
					Level = max(0, min(100, Level));

					int alpha  = int(256 * Frame * pow(Level / 100.0, 0.4) + 0.5);
					int offset = int(255 * Level / 100) * alpha >> 8;

					for( int count = nWidth * nHeight; count > 0; --count, pPixels += 4 )
					{
						pPixels[0] = Glow( pPixels[0], alpha, offset );
						pPixels[1] = Glow( pPixels[1], alpha, offset );
						pPixels[2] = Glow( pPixels[2], alpha, offset );
					}

					return c_nErrorNone;
				}
#ifndef IMAGE_EDITOR
				int BGRA_EffectPencilSketch( BYTE* pPixels, int nWidth, int nHeight, double Frame, int Level, int nBrightness, int nContrast, int nLengthFactor )
				{
					CAlphaTable table;
					if( table.Calculate( Frame ) < 0 )
						return c_nErrorNone;

					//изменяем яркость и контрастность
					BGRA_AdjustBrightness( pPixels, nWidth, nHeight, Frame, nBrightness);
					BGRA_AdjustContrast( pPixels, nWidth, nHeight, Frame, nContrast);
					
					// проверка входных данных
					Level = max(0, min(100, Level));
					int nLevel = int(Level * 256 / 100);

					// создаём необходимые буфера
					int nImageSize = nWidth * nHeight;

					CBuffer<BYTE> oGrayscale( nImageSize );
					CBuffer<BYTE> oBuffer( (nWidth + 4) * (nHeight + 4) );
					CBuffer<BYTE> oFilter( nImageSize );
							
					if( oGrayscale.IsEmpty() || oBuffer.IsEmpty() || oFilter.IsEmpty() )
						return c_nErrorNoMemory;

					// получаем изображение в оттенках серого
					BYTE* pSrc = pPixels;
					BYTE* pDst = oGrayscale.GetPtr();

					for( int count = nImageSize; count > 0; --count, pSrc += 4, pDst += 1 )
					{
						pDst[0] = Intensity2( pSrc[2], pSrc[1], pSrc[0] );
					}

					// получаем копию серого изображения для фильтрации
					IppiSize SrcSize = {nWidth, nHeight};
					IppiSize BufSize = {nWidth + 4, nHeight + 4};

					pSrc = oGrayscale.GetPtr();
					pDst = oBuffer.GetPtr();

					ippiCopyReplicateBorder_8u_C1R( pSrc, nWidth, SrcSize, pDst, BufSize.width, BufSize, 2, 2 );

					// получаем отфильтрованное изображение
					pSrc = oBuffer.GetPtr() + (BufSize.width + 1) * 2;
					pDst = oFilter.GetPtr();

					ippiFilterLaplace_8u_C1R( pSrc, BufSize.width, pDst, SrcSize.width, SrcSize, ippMskSize3x3 );

					CRandom rnd( ::GetTickCount() );

					// смешиваем отфильтрованное изображение с серым
					pSrc = oFilter.GetPtr();
					pDst = oGrayscale.GetPtr();
					BYTE* pBuf = oBuffer.GetPtr() + (BufSize.width + 1) * 2;

					//nLengthFactor = max(0, min(nLengthFactor, 100 ));
					//double length = (20 - 4) * nLengthFactor / 100.0 + 4;
					//int nXShiftHalf =  max( 4, int( length * nWidth / 640.0));
					//int nYShiftHalf =  max( 4, int( length * nHeight / 480.0));
					int nXShiftHalf =  max( 4, int( 8 * nWidth / 640.0));
					int nYShiftHalf =  max( 4, int( 8 * nHeight / 480.0));

					double dFactorX = double(nLevel) / nXShiftHalf;
					double dFactorY = double(nLevel) / nYShiftHalf;

					for( int nRow = 0; nRow < nHeight; ++nRow, pBuf += 4  )
					{
						for( int nPos = 0; nPos < nWidth; ++nPos, ++pBuf, ++pSrc, ++pDst )
						{
							if( pSrc[0] < 128  )
							{
								if( rnd.Rand( 256 ) < (Level * (128 - pSrc[0]) >> 6) )
								{
									int nX = nPos + nXShiftHalf;
									int nY = nRow + nYShiftHalf;

									if( nY >= nHeight )
									{
										nY = nHeight - 1;
										if ( nX - nPos > nY - nRow )
											nX = nPos + (nY - nRow);
									}
									if( nX >= nWidth )
									{
										nX = nWidth - 1;
										if ( nY - nRow > nX - nPos )
											nY = nRow + (nX - nPos);
									}

									BYTE* ptr = pDst + (nX - nPos) + (nY - nRow) * SrcSize.width;
									int nMaxX = max(nPos - nXShiftHalf, 0);
									int nMaxY = max(nRow - nYShiftHalf, 0);

									for( ; nX > nMaxX && nY > nMaxY; --nX, --nY, ptr -= 1 + SrcSize.width )
									{
										int alpha = int((nXShiftHalf - abs(nX - nPos)) * dFactorX/*nLevel / nXShiftHalf*/ + (nYShiftHalf - abs(nY - nRow)) * dFactorY/*nLevel / nYShiftHalf*/) / 2 * (256 - pBuf[0]) / 256;
										ptr[0] = AlphaCompEx( ptr[0], pSrc[0], alpha - (pSrc[0] * alpha >> 8), 256 );
									}
								}
								pDst[0] = AlphaComp( 255, 0,  pSrc[0]);
							}
						}
					}

					// смешиваем полученное изображение с исходным
					pSrc = oGrayscale.GetPtr();
					pDst = pPixels;

					if( table.IsEmpty() )
					{
						for( int count = nImageSize; count > 0; --count, pSrc += 1, pDst += 4)
						{
							pDst[2] = pDst[1] = pDst[0] = pSrc[0];
						}
					}
					else
					{
						for( int count = nImageSize; count > 0; --count, pSrc += 1, pDst += 4)
						{
							pDst[0] = table.Blend( pDst[0], pSrc[0] );
							pDst[1] = table.Blend( pDst[1], pSrc[0] );
							pDst[2] = table.Blend( pDst[2], pSrc[0] );
						}
					}

					return c_nErrorNone;
				}

				int BGRA_EffectTVSimulation( BYTE* pPixels, int nWidth, int nHeight, int nStride, Paint::Effects::TVSimulator* pEffect, double dTime, double dFrame )
				{
					if (!pPixels || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					if( pEffect )
						pEffect->Draw( pPixels, nWidth, nHeight, nStride, dTime, dFrame );

					return c_nErrorNone;
				}

				int BGRA_EffectGrayOut( BYTE* pPixels, int nWidth, int nHeight, double Frame, int Color, int Filter, int LevelR, int LevelG, int LevelB, int Threshold, int nType )
				{
					Frame = Clamp( Frame );

					if( Frame < 0.001 )
						return c_nErrorNone;

					LevelR = (255 * LevelR / 100);
					LevelG = (255 * LevelG / 100);
					LevelB = (255 * LevelB / 100);

					// best quality, matrix 5x5
					//
					CBuffer<BYTE> oGrayMask( nWidth * nHeight );
					if( oGrayMask.IsEmpty() )
						return Core::c_nErrorNoMemory;

					if( nType )
					{
						ImageStudio::Colors::Color oColor; oColor.FromCOLORREF(Color);
						
						float fHue, fLightness, fSaturation; 
						oColor.ToHLS( fHue, fLightness, fSaturation );

						int nColorHue = int(fHue * 255.0f / 360.0f + 0.5f);
						int nColorSat = int(fSaturation * 255.0f + 0.5f);
						int nColorLig = int(fLightness * 255.0f + 0.5f);

						if( nColorSat < 10 )
						{
							return BGRA_AdjustGrayscale( pPixels, nWidth, nHeight, Frame, FALSE );
						}
						
						int fThreshold = int(Threshold * 255.0f / 100.f + 0.5f);

						LPBYTE pHLS = new BYTE[nWidth * nHeight * 4];
						if (NULL == pHLS)
							return c_nErrorNoMemory;

						IppiSize SrcSize = {nWidth, nHeight};
						ippiBGRToHLS_8u_AC4R( pPixels, nWidth * 4, pHLS, nWidth * 4, SrcSize );

						BYTE* pGrayMask = oGrayMask.GetPtr();
						BYTE* pSrc = pHLS;
						for( int count = nWidth*nHeight; count > 0; --count, pSrc += 4, pGrayMask += 1 )
						{
							if( abs(pSrc[1] - nColorLig) >= fThreshold )
							{
								*pGrayMask = 0;
								continue;
							}

							*pGrayMask = ((abs( pSrc[0] - nColorHue ) > 5) ) ? 0 : 255;
						}

						delete [] pHLS;
					}
					else
					{
						int blue  = (Color >> 16) & 255;
						int green = (Color >> 8) & 255;
						int red   = Color & 255;

						BYTE* pGrayMask = oGrayMask.GetPtr();
						BYTE* pSrc = pPixels;
						for( int count = nWidth*nHeight; count > 0; --count, pSrc += 4 )
						{
							*pGrayMask++ = (abs(pSrc[0] - blue) > LevelB || abs(pSrc[1] - green) > LevelG || abs(pSrc[2] - red) > LevelR) ? 0 : 255;
						}
					}

					// блурим
					BYTE* pGrayMask = oGrayMask.GetPtr();
					Filters::ApplyBlurFilter( Filter, 1, pGrayMask, nWidth, pGrayMask, nWidth, nWidth, nHeight );
					
					// наносим серый цвет согласно маске
					DWORD factors[3] = {
						DWORD(306 * Frame + 0.5),
						DWORD(601 * Frame + 0.5),
						DWORD(117 * Frame + 0.5),
					};

					BYTE* pSrc = pPixels;
					for( int count = nWidth * nHeight; count > 0; --count, pSrc += 4, pGrayMask += 1 )
					{
						if( 255 == pGrayMask[0] )
							continue;

						DWORD alpha = 255 - pGrayMask[0];
						DWORD matrix[2][3];
						matrix[0][0] = (factors[0] * alpha + 128) / 255;
						matrix[0][1] = (factors[1] * alpha + 128) / 255;
						matrix[0][2] = (factors[2] * alpha + 128) / 255;
						matrix[1][0] = 1024 - matrix[0][1] - matrix[0][2];
						matrix[1][1] = 1024 - matrix[0][0] - matrix[0][2];
						matrix[1][2] = 1024 - matrix[0][0] - matrix[0][1];
						
						DWORD b = pSrc[0];
						DWORD g = pSrc[1];
						DWORD r = pSrc[2];

						pSrc[2] = (BYTE)((r * matrix[1][0] + g * matrix[0][1] + b * matrix[0][2] + 512) >> 10);
						pSrc[1] = (BYTE)((r * matrix[0][0] + g * matrix[1][1] + b * matrix[0][2] + 512) >> 10);
						pSrc[0] = (BYTE)((r * matrix[0][0] + g * matrix[0][1] + b * matrix[1][2] + 512) >> 10);
					}

					return c_nErrorNone;
				}

				int BGRA_EffectWind( BYTE* pPixels, int nWidth, int nHeight, double Frame, long nLength, long nDensity )
				{
					//nLength  = max(-100, min(100, nLength));
					nDensity = max(0, min(100, nDensity));

					nLength  = int(nLength  * Frame + 0.5);
					nDensity = int(nDensity * Frame + 0.5);

					if( !nLength || !nDensity )
						return c_nErrorNone;
					
					CBuffer<BYTE> oBackup(nWidth * nHeight * 4);
					CBuffer<BYTE> oBuffer(nWidth * nHeight);
					if( oBuffer.IsEmpty() || oBackup.IsEmpty() )
						return c_nErrorNoMemory;

					BYTE* pBuffer = oBuffer.GetPtr();
					BYTE* pSource = oBackup.GetPtr();

					memcpy( pSource, pPixels, nWidth * nHeight * 4 );

					for( int i = nWidth * nHeight; i > 0; --i, pBuffer += 1, pSource += 4 )
					{
						pBuffer[0] = Intensity2( pSource[2], pSource[1], pSource[0] );
					}

					pBuffer = oBuffer.GetPtr();
					pSource = oBackup.GetPtr();
					
					Filters::ApplyFilter( ippiFilterLaplace_8u_C1R, ippMskSize3x3, pBuffer, nWidth, nHeight, nWidth, 1 );

					CRandom rnd( ::GetTickCount() );

					if( nLength > 0 )
					{
						for( int nRow = 0; nRow < nHeight; nRow++ )
						{
							for( int nPos = 0; nPos < nWidth; ++nPos, pBuffer += 1, pSource += 4 )
							{
								if( pBuffer[0] > 0 )
								{
									if( rnd.Rand( 256 ) > nDensity )
										continue;

									int length = nLength + (rnd.Rand( nLength ) >> 1);
									int nRight = nPos + length;
									if( nRight >= nWidth )
										nRight = nWidth - 1;

									BYTE* pDst = pPixels + (nRow * nWidth + nPos + 1) * 4;
									for( ; nRight > nPos; --nRight, pDst += 4 )
									{
										int alpha = (nRight - nPos) * 128 / length;
										pDst[0] = AlphaCompEx( pDst[0], pSource[0], alpha, 256 );
										pDst[1] = AlphaCompEx( pDst[1], pSource[1], alpha, 256 );
										pDst[2] = AlphaCompEx( pDst[2], pSource[2], alpha, 256 );
									}
								}
							}
						}
					}
					else
					{
						nLength = abs(nLength);

						pSource += (nWidth - 1) * 4;
						pBuffer += nWidth - 1;
						for( int nRow = 0; nRow < nHeight; ++nRow, pBuffer += nWidth * 2, pSource += nWidth * 2 * 4 )
						{
							for( int nPos = nWidth - 1; nPos >= 0; --nPos, pBuffer -= 1, pSource -= 4 )
							{
								if( pBuffer[0] > 0 )
								{
									if( rnd.Rand( 256 ) > nDensity )
										continue;

									int length = nLength + (rnd.Rand( nLength ) >> 1);
									int nLeft  = nPos - length;
									if( nLeft < 0 )
										nLeft = 0;

									BYTE* pDst = pPixels + (nRow * nWidth + nPos - 1) * 4;
									for( ; nLeft < nPos; ++nLeft, pDst -= 4 )
									{
										int alpha = (nPos - nLeft) * 128 / length;
										pDst[0] = AlphaCompEx( pDst[0], pSource[0], alpha, 256 );
										pDst[1] = AlphaCompEx( pDst[1], pSource[1], alpha, 256 );
										pDst[2] = AlphaCompEx( pDst[2], pSource[2], alpha, 256 );
									}
								}
							}
						}
					}

					return c_nErrorNone;
				}
				int BGRA_EffectNewsprint( BYTE* pPixels, int nWidth, int nHeight, double Frame, int Level, int Filter )
				{
					CAlphaTable table;
					if( table.Calculate( Frame ) < 0 )
						return c_nErrorNone;

					Level = max(0, min(100, Level));

					IppiSize SrcSize = {nWidth, nHeight};
					IppiSize BufSize = {nWidth + 4, nHeight + 4};
					
					CBuffer<BYTE> oBuffer( BufSize.width * BufSize.height * 4 );
					
					if( oBuffer.IsEmpty() )
						return c_nErrorNoMemory;

					ippiCopyReplicateBorder_8u_C4R( pPixels, SrcSize.width*4, SrcSize, oBuffer.GetPtr(), BufSize.width*4, BufSize, 2, 2 );

					//// рабочая матрица
					//int conv[5][5] = {
					//	{-1,  -1,  -1,  -1, -1},
					//	{-1,  -1,  -1,  -1, -1},
					//	{-1,  -1,  30,  -1, -1},
					//	{-1,  -1,  -1,  -1, -1},
					//	{-1,  -1,  -5,  -1, -1},
					//};

					BYTE* pBackup = oBuffer.GetPtr() + (BufSize.width + 1) * 2 *4;
					int   nStep   = BufSize.width * 4;
					int   level   = (Level * 254 + 50) / 100;

					for( int nY = nHeight; nY > 0; --nY, pBackup += 4*4 )
					{
						for( int nX = nWidth; nX > 0; --nX, pBackup += 4, pPixels += 4 )
						{
							int b = 0, g = 0, r = 0;
							
							for( int v = 0; v < 5; v++ )
							{
								for( int u = 0; u < 5; u++ )
								{
									BYTE* pixel = pBackup + (v - 2) * nStep + (u - 2) * 4;
									
									b -= pixel[0];
									g -= pixel[1];
									r -= pixel[2];
								}
							}

							b -= pBackup[nStep * 2 + 0] * 4;
							g -= pBackup[nStep * 2 + 1] * 4;
							r -= pBackup[nStep * 2 + 2] * 4;

							b += pBackup[0] * 31;
							g += pBackup[1] * 31;
							r += pBackup[2] * 31;

							r = IntToByte( r );
							g = IntToByte( g );
							b = IntToByte( b );

							int clr = Intensity2( r, g, b ) > level ? 255 : 0;

							if( table.IsEmpty() )
							{
								pPixels[2] = pPixels[1] = pPixels[0] = clr;
							}
							else
							{
								pPixels[0] = table.Blend( pPixels[0], clr );
								pPixels[1] = table.Blend( pPixels[1], clr );
								pPixels[2] = table.Blend( pPixels[2], clr );
							}
						}
					}

					return BGRA_EffectBlur( pPixels - nWidth * nHeight * 4, nWidth, nHeight, 1, 1 );
				}				
				int BGRA_EffectSnow( BYTE* pPixels, int nWidth, int nHeight, int nStride, Paint::Effects::Snow* pEffect, double dTime, double dFrame, BOOL bSnowDrift, double dSnowDriftSpeedFactor )
				{
					if (!pPixels || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					if( pEffect )
						pEffect->Draw( pPixels, nWidth, nHeight, nStride, dTime, dFrame, bSnowDrift, dSnowDriftSpeedFactor );

					return c_nErrorNone;
				}

				int BGRA_EffectParticles( BYTE* pPixels, int nWidth, int nHeight, Paint::Effects::Particles* pEffect, double dTime, double dFrame )
				{
					if (!pPixels || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					if( pEffect )
						pEffect->Draw( pPixels, nWidth, nHeight, dTime, dFrame );

					return c_nErrorNone;
				}

				int BGRA_EffectJpegCompression( BYTE* pBGRA, int nWidth, int nHeight, double dFrame, int nQuality )				
				{
					// определяем матрицу косинусов ДКП
					static const double c_dCos[64] = {
						0.70710678,  0.98078528,  0.92387953,  0.83146961,  0.70710678,  0.55557023,  0.38268343,  0.19509032, 
						0.70710678,  0.83146961,  0.38268343, -0.19509032, -0.70710678, -0.98078528, -0.92387953, -0.55557023, 
						0.70710678,  0.55557023, -0.38268343, -0.98078528, -0.70710678,  0.19509032,  0.92387953,  0.83146961, 
						0.70710678,  0.19509032, -0.92387953, -0.55557023,  0.70710678,  0.83146961, -0.38268343, -0.98078528, 
						0.70710678, -0.19509032, -0.92387953,  0.55557023,  0.70710678, -0.83146961, -0.38268343,  0.98078528, 
						0.70710678, -0.55557023, -0.38268343,  0.98078528, -0.70710678, -0.19509032,  0.92387953, -0.83146961, 
						0.70710678, -0.83146961,  0.38268343,  0.19509032, -0.70710678,  0.98078528, -0.92387953,  0.55557023, 
						0.70710678, -0.98078528,  0.92387953, -0.83146961,  0.70710678, -0.55557023,  0.38268343, -0.19509032, 
					};

					static const BYTE Y[101] = {
						255, 255, 250, 167, 125, 100,  83,  71,  63, 56, 
						 50,  45,  42,  38,  36,  33,  31,  29,  28, 26,
						 25,  24,  23,  22,  21,  20,  19,  19,  18, 17, 
						 17,  16,  16,  15,  15,  14,  14,  14,  13, 13, 
						 13,  12,  12,  12,  11,  11,  11,  11,  10, 10,

					    121, 119, 116, 114, 111, 109, 106, 104, 102, 99,
						 97,  94,  92,  90,  87,  85,  82,  80,  77, 75,
						 73,  70,  68,  65,  63,  61,  58,  56,  53, 51,
						 48,  46,  44,  41,  39,  36,  34,  31,  29, 27,
						 24,  22,  19,  17,  15,  12,  10,   7,   5,  2,
						  1
					};

					static const BYTE UV[101] = {
						255, 255, 255, 255, 213, 170, 142, 121, 106, 94,
						 85,  77,  71,  65,  61,  57,  53,  50,  47, 45,
						 43,  40,  39,  37,  35,  34,  33,  31,  30, 29,
						 28,  27,  27,  26,  24,  24,  23,  23,  22, 22,
						 21,  21,  20,  20,  19,  19,  18,  18,  18, 17,

						 99,  97,  95,  93,  91,  89,  87,  85,  83, 81,
						 79,  77,  75,  73,  71,  69,  67,  65,  63, 61,
						 59,  57,  55,  53,  51,  50,  48,  46,  44, 42,
						 40,  38,  36,  34,  32,  30,  28,  26,  24, 22,
						 20,  18,  16,  14,  12,  10,   8,   6,   4,  2,
						  1
					};

					int arQuantY[64] = { 
						16, 11, 10, 16, 24, 40, 51, 61,
						12, 12, 14, 19, 26, 58, 60, 55,
						14, 13, 16, 24, 40, 57, 69, 56,
						14, 17, 22, 29, 51, 87, 80, 62,
						18, 22, 37, 56, 68,109,103, 77,
						24, 35, 55, 64, 81,104,113, 92,
						49, 64, 78, 87,103,121,120,101,
						72, 92, 95, 98,112,100,103, 99
					};
					
					int arQuantUV[64] = { 
						17, 18, 24, 47, 99, 99, 99, 99,
						18, 21, 26, 66, 99, 99, 99, 99,
						24, 26, 56, 99, 99, 99, 99, 99,
						47, 66, 99, 99, 99, 99, 99, 99,
						99, 99, 99, 99, 99, 99, 99, 99,
						99, 99, 99, 99, 99, 99, 99, 99,
						99, 99, 99, 99, 99, 99, 99, 99,
						99, 99, 99, 99, 99, 99, 99, 99
					};

					double arReQuantY[64];
					double arReQuantUV[64];

					// определяем паретр quality
					int nAQuality = ColorBlend( 100, nQuality, Clamp(dFrame) );
					
					double dKoefY = 1.0, dKoefUV = 1.0; // = 1 при quality = 50

					if ( nAQuality < 50 )
					{
						dKoefY  = double( Y[nAQuality]) / 10.0;
						dKoefUV = double(UV[nAQuality]) / 17.0;
					}
					else 
					{
						dKoefY  = double( Y[nAQuality]) / 121.0;
						dKoefUV = double(UV[nAQuality]) / 99.0;
					}

					// определяем матрицу квантования
					for( int i = 0; i < 64; i++)
					{
						arQuantY[i] = max(1, min(255, int(arQuantY[i] * dKoefY)));
						arQuantUV[i] = max(1, min(255, int(arQuantUV[i] * dKoefUV)));

						arReQuantY[i] = 0.25 / arQuantY[i];
						arReQuantUV[i] = 0.25 / arQuantUV[i];
					}

					// вспомогательные массивы для хранения YUV
					double pY[64];
					double pU[64];
					double pV[64];

					double pTempY[64];
					double pTempU[64];
					double pTempV[64];

					for( int nRow = 0; nRow < nHeight; nRow += 8 )
					{
						BYTE* pBlock = pBGRA + nRow * nWidth * 4;
						int   nMaxY  = min(nHeight - nRow, 8);

						for( int nPos = 0; nPos < nWidth; nPos += 8, pBlock += 8 * 4 )
						{
							int nMaxX = min(nWidth - nPos, 8);

							BYTE* pPixel = pBlock;
							
							// 1 шаг переводим RGB -> YUV
							int nY, nX;

							for( nY = 0; nY < nMaxY; nY++, pPixel += nWidth * 4 )
							{
								for( nX = 0; nX < nMaxX; nX++ )
								{
									int B = pPixel[nX * 4 + 0];
									int G = pPixel[nX * 4 + 1];
									int R = pPixel[nX * 4 + 2];
									
									pY[nY * 8 + nX] =  0.299  * R + 0.587  * G + 0.114  * B;
									pU[nY * 8 + nX] = -0.1687 * R - 0.3313 * G + 0.5    * B;
									pV[nY * 8 + nX] =  0.5    * R - 0.4187 * G - 0.0813 * B;
								}

								while( nX < 8 )
								{
									pY[nY * 8 + nX] = pY[nY * 8 + nMaxX - 1];
									pU[nY * 8 + nX] = pU[nY * 8 + nMaxX - 1];
									pV[nY * 8 + nX] = pV[nY * 8 + nMaxX - 1];
									
									nX += 1;
								}
							}

							while( nY < 8 )
							{
								for( nX = 0; nX < 8; nX++ )
								{
									pY[nY * 8 + nX] = pY[nX + (nMaxY - 1)*8];
									pU[nY * 8 + nX] = pU[nX + (nMaxY - 1)*8];
									pV[nY * 8 + nX] = pV[nX + (nMaxY - 1)*8];
								}
								nY += 1;
							}

							// 2 шаг проводим сегментацию
							for( nY = 0; nY < 8; nY += 2 )
							{
								for( nX = 0; nX < 8; nX += 2 )
								{
									int i = nY * 8 + nX;

									double dU = 0.25 * (pU[i] + pU[i + 1] + pU[i + 8] + pU[i + 9]);
									double dV = 0.25 * (pV[i] + pV[i + 1] + pV[i + 8] + pV[i + 9]);

									pU[i] = pU[i + 1] = pU[i + 8] = pU[i + 9] = dU;
									pV[i] = pV[i + 1] = pV[i + 8] = pV[i + 9] = dV;
								}
							}

							// 3 шаг к каждому блоку 8х8 применяем Дискретное Косинус-Пробразование (DCT)
							for (nY = 0; nY < 8; nY++ )
							{
								for (nX = 0; nX < 8; nX++ )
								{
									double dSumY = 0;
									double dSumU = 0;
									double dSumV = 0;

									for( int nK = 0; nK < 8; nK++ )                        
									{
										double factor = c_dCos[nX + nK * 8];
										dSumY += pY[nY * 8 + nK] * factor;
										dSumU += pU[nY * 8 + nK] * factor;
										dSumV += pV[nY * 8 + nK] * factor;
									}

									pTempY[nY * 8 + nX] = dSumY;
									pTempU[nY * 8 + nX] = dSumU;
									pTempV[nY * 8 + nX] = dSumV;

								}
							}

							for (nY = 0; nY < 8; nY++ )
							{
								for (nX = 0; nX < 8; nX++ )
								{
									double dSumY = 0;
									double dSumU = 0;
									double dSumV = 0;

									for( int nK = 0; nK < 8; nK++ )                        
									{
										double factor = c_dCos[nY + nK * 8];
										dSumY += pTempY[nX + nK * 8] * factor;
										dSumU += pTempU[nX + nK * 8] * factor;
										dSumV += pTempV[nX + nK * 8] * factor;
									}
									
									int index = nY * 8 + nX;

									// 4 шаг, квантование
									dSumY *= arReQuantY[index];
									dSumU *= arReQuantUV[index];
									dSumV *= arReQuantUV[index];

									// округляем до ближайшего целого числа
									dSumY = Round( dSumY );
									dSumU = Round( dSumU );
									dSumV = Round( dSumV );

									// 5 шаг, обратное квантование
									pY[index] = dSumY * arQuantY[index];
									pU[index] = dSumU * arQuantUV[index];
									pV[index] = dSumV * arQuantUV[index];
								}      
							}

							// 6 шаг к каждому блоку 8х8 применяем обратное Дискретное Косинус-Пробразование (DCT)
							for (nY = 0; nY < 8; nY++ )
							{
								for (nX = 0; nX < 8; nX++ )
								{
									double dSumY = 0;
									double dSumU = 0;
									double dSumV = 0;

									for( int nK = 0; nK < 8; nK++ )                        
									{
										double factor = c_dCos[nX * 8 + nK];
										dSumY += pY[nY * 8 + nK] * factor;
										dSumU += pU[nY * 8 + nK] * factor;
										dSumV += pV[nY * 8 + nK] * factor;
									}

									pTempY[nY * 8 + nX] = dSumY;
									pTempU[nY * 8 + nX] = dSumU;
									pTempV[nY * 8 + nX] = dSumV;

								}
							}

							pPixel = pBlock;
							
							for (nY = 0; nY < nMaxY; nY++, pPixel += nWidth * 4 )
							{
								for (nX = 0; nX < nMaxX; nX++ )
								{
									double dSumY = 0;
									double dSumU = 0;
									double dSumV = 0;

									for( int nK = 0; nK < 8; nK++ )                        
									{
										double factor = c_dCos[nY * 8 + nK];
										dSumY += pTempY[nX + nK * 8] * factor;
										dSumU += pTempU[nX + nK * 8] * factor;
										dSumV += pTempV[nX + nK * 8] * factor;
									}
									
									// сразу делаем 7 шаг : сдвиг на 128
									dSumY *= 0.25;
									dSumU *= 0.25;
									dSumV *= 0.25;

									// 8 шаг переводим YUV -> RGB
									pPixel[nX * 4 + 0] = IntToByte( int(dSumY + 1.772   * dSumU) );
									pPixel[nX * 4 + 1] = IntToByte( int(dSumY - 0.34414 * dSumU - 0.71414 * dSumV) );
									pPixel[nX * 4 + 2] = IntToByte( int(dSumY + 1.402   * dSumV) );
								}      
							}
						}
					}

					return c_nErrorNone;
				}

				
				int BGRA_EffectSoftFocus( BYTE* pPixels, int nWidth, int nHeight, double dFrame, double dBlurSize, double dIntensity, int Type = 0 )
				{
					dFrame = Clamp( dFrame * dIntensity / 100 );

					int nImageSize = nWidth * nHeight * 4;
					CBuffer<BYTE> oBuffer( nImageSize );
					if( oBuffer.IsEmpty() )
						return c_nErrorNoMemory;
					
					::memcpy( oBuffer.GetPtr(), pPixels, nImageSize );

					BGRA_EffectGaussianBlur( oBuffer.GetPtr(), nWidth, nHeight, 1, int(dBlurSize) );

					BYTE table[256];

					for( int i = 0; i < 256; i++ )
					{
						int clr = 255;
						switch( Type )
						{
						case 1: clr = i; break;
						case 2: clr = 255 - i; break;
						case 3: clr = abs(i*2 - 255); break;
						case 4: clr = 255 - abs(i*2 - 255); break;
						
						case 5: clr = Div255(i * i); break;
						case 6: clr = 255 - Div255(i * i); break;
						case 7: clr = Div255((255 - i) * (255 - i)); break;
						case 8: clr = 255 - Div255((255 - i) * (255 - i)); break;
						}

						table[i] = int(clr * dFrame + 0.5);
					}

					BYTE* pBuffer = oBuffer.GetPtr();
					for( int nCount = nWidth * nHeight; nCount > 0; --nCount, pPixels += 4, pBuffer += 4 )
					{
						int gray1 = Intensity2( pBuffer[2], pBuffer[1], pBuffer[0] );
						int gray2 = Intensity2( pPixels[2], pPixels[1], pPixels[0] );

						if( gray2 <= gray1 )
							continue;
							
						int alpha  = table[gray2];
						
						pPixels[0] = AlphaComp( pPixels[0], pBuffer[0], alpha );
						pPixels[1] = AlphaComp( pPixels[1], pBuffer[1], alpha );
						pPixels[2] = AlphaComp( pPixels[2], pBuffer[2], alpha );
					}
					
					return c_nErrorNone;
				}

				int BGRA_EffectOldPhoto( BYTE* pPixels, int nWidth, int nHeight, Paint::Effects::OldPhoto* pMaskPainter, double dFrame, int nMask, int whiteBlack,bool vFlip )
				{
					if (!pPixels || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					if( pMaskPainter )
					{
						pMaskPainter->Draw( pPixels	, nWidth, nHeight, nMask, whiteBlack, vFlip);
					}

					return c_nErrorNone;
				}
#endif
				int BGRA_EffectVignette( BYTE* pPixels, int nWidth, int nHeight, double dFrame, double dCenterX, double dCenterY, double dRadius1X, double dRadius1Y, double dRadius2X, double dRadius2Y, double dAngle, double dIntensity )
				{
					dIntensity *= dFrame * 255;
					if( abs(dIntensity) < 0.5 )
						return c_nErrorNone;

					dRadius1X = abs(dRadius1X) + 0.0001;
					dRadius1Y = abs(dRadius1Y) + 0.0001;
					dRadius2X = abs(dRadius2X) + 0.0001;
					dRadius2Y = abs(dRadius2Y) + 0.0001;

					if( dRadius1X > dRadius2X )
					{
						Exchange( &dRadius1X, &dRadius2X );
					}

					if( dRadius1Y > dRadius2Y )
					{
						Exchange( &dRadius1Y, &dRadius2Y );
					}

					double dRadius1XY = dRadius1X * dRadius1Y;
					double dRadius2XY = dRadius2X * dRadius2Y;

					dRadius1X *= dRadius1X;
					dRadius1Y *= dRadius1Y;
					dRadius2X *= dRadius2X;
					dRadius2Y *= dRadius2Y;

					double dRcpR1X = 1 / dRadius1X;
					double dRcpR1Y = 1 / dRadius1Y;
					double dRcpR2X = 1 / dRadius2X;
					double dRcpR2Y = 1 / dRadius2Y;
					
					BYTE oTable[256];

					int nMaxLight = Round(dIntensity);
					for( int i = 0; i < 256; i++ )
					{
						oTable[i] = IntToByte(i + nMaxLight);
					}

					dAngle *= IPP_PI180;
					double dSin = sin( dAngle );
					double dCos = cos( dAngle );

					for( int nY = 0; nY < nHeight; ++nY )
					{
						double dPosY = nY - dCenterY;
						
						for( int nX = 0; nX < nWidth; ++nX, pPixels += 4 )
						{
							double dPosX = nX - dCenterX;

							double x = dPosX * dCos - dPosY * dSin;
							double y = dPosX * dSin + dPosY * dCos;

							double xx = x * x;
							double yy = y * y;

							if( (xx * dRcpR1X + yy * dRcpR1Y) <= 1 )
							{
								continue;
							}

							if( (xx * dRcpR2X + yy * dRcpR2Y) >= 1 )
							{
								pPixels[0] = oTable[pPixels[0]];
								pPixels[1] = oTable[pPixels[1]];
								pPixels[2] = oTable[pPixels[2]];
								continue;
							}

							double factor1 = dRadius1XY / sqrt( xx * dRadius1Y + yy * dRadius1X );
							double factor2 = dRadius2XY / sqrt( xx * dRadius2Y + yy * dRadius2X );

							double x1 = x * factor1;
							double y1 = y * factor1;

							double x2 = x * factor2;
							double y2 = y * factor2;

							x2 -= x1;
							y2 -= y1;
							x  -= x1;
							y  -= y1;

							double t = sin( sqrt((x * x + y * y) / (x2 * x2 + y2 * y2)) * IPP_PI2 );
							t = t * t * dIntensity;

							int b = pPixels[0];
							int g = pPixels[1];
							int r = pPixels[2];

							b = int(b + t);
							g = int(g + t);
							r = int(r + t);

							pPixels[0] = IntToByte( b );
							pPixels[1] = IntToByte( g );
							pPixels[2] = IntToByte( r );
						}
					}
					
					return c_nErrorNone;
				}

				int BGRA_EffectPostCrop( BYTE* pPixels, int nWidth, int nHeight, double dFrame, double dAmount, double dMidpoint, double dRoundness, double dFeather )
				{
					dFrame = Clamp( dFrame );

					dAmount = max(-100, min(100, dAmount)) / 100.0 * 255;

					int nMaxLight = Round( dAmount );
					if( !nMaxLight )
						return c_nErrorNone;

					BYTE oTable[256];

					for( int i = 0; i < 256; i++ )
					{
						oTable[i] = IntToByte(i + nMaxLight);
					}

					dFeather  = max(0, min(100, dFeather )) / 100.0;
					dMidpoint = max(0, min(100, dMidpoint)) / 100.0;

					dFeather  = dFeather  * 0.99 + 0.01;
					dMidpoint = (dMidpoint - 0.5) * 0.5;
					
					double dCenterX = (nWidth  - 1) * 0.5;
					double dCenterY = (nHeight - 1) * 0.5;
					
					double dMaxSize = max(dCenterX, dCenterY);
					double dMinSize = min(dCenterX, dCenterY);

					double dOffset = dMinSize * dMidpoint;
					
					dMaxSize += dOffset;
					dMinSize += dOffset;

					double dHalfWidth  = dCenterX + dOffset;
					double dHalfHeight = dCenterY + dOffset;


					double dMinRoundness = 1 - min( abs(min(dRoundness, 0)), 100 ) / 105.0;
					double dMaxRoundness = min( max(dRoundness, 0), 100 ) / 100.0;

					double dW = dHalfWidth  - dMaxSize * dMinRoundness;
					double dH = dHalfHeight - dMaxSize * dMinRoundness;

					if( dW < 0 ) dW = 0;
					if( dH < 0 ) dH = 0;
					
					double dRoundRadius = (dMinSize - min(dW, dH)) * dFeather;

					double dMinX = dHalfWidth  - dRoundRadius;
					double dMaxX = dHalfWidth  + dRoundRadius;
					double dMinY = dHalfHeight - dRoundRadius;
					double dMaxY = dHalfHeight + dRoundRadius;
					
					double dRoundFactor = (dRoundRadius > 0.001) ? (1 / (dRoundRadius * 2)) : 0;

					double dRadiusX = dHalfWidth  - dW;
					double dRadiusY = dHalfHeight - dH;

					double dSumRadius = (dRadiusX + dRadiusY) * 0.5;

					dRadiusX += (dSumRadius - dRadiusX) * dMaxRoundness;
					dRadiusY += (dSumRadius - dRadiusY) * dMaxRoundness;

					dRadiusX *= dRadiusX;
					dRadiusY *= dRadiusY;

					double dRcpRadiusX = dRadiusX > IPP_EPS52 ? (1 / dRadiusX) : IPP_MAXABS_64F;
					double dRcpRadiusY = dRadiusY > IPP_EPS52 ? (1 / dRadiusY) : IPP_MAXABS_64F;

					double dRadiusXY = dRadiusX * dRadiusY;

					for( int nRow = 0; nRow < nHeight; ++nRow )
					{
						double y = abs(nRow - dCenterY);

						for( int nPos = 0; nPos < nWidth; ++nPos, pPixels += 4 )
						{
							double x = abs(nPos - dCenterX);

							if( y >= dH )
							{
								if( x <= dW )
								{
									if( y < dMinY )
										continue;

									if( y > dMaxY )
									{
										pPixels[0] = oTable[pPixels[0]];
										pPixels[1] = oTable[pPixels[1]];
										pPixels[2] = oTable[pPixels[2]];
										continue;
									}
									
									double t = (y - dMinY) * dRoundFactor;

									t = sin( t * IPP_PI2 );
									t = t * t * dAmount;

									int disp = Round( t );

									pPixels[0] = IntToByte( pPixels[0] + disp );
									pPixels[1] = IntToByte( pPixels[1] + disp );
									pPixels[2] = IntToByte( pPixels[2] + disp );
								}
								else
								{
									double dX0 = x - dW; // x != 0
									double dY0 = y - dH; // y != 0

									double xx = dX0 * dX0;
									double yy = dY0 * dY0;

									double dX = dRadiusY * xx + dRadiusX * yy;
									double dY = dRadiusX * yy + dRadiusY * xx;

									if( dX > IPP_EPS52 ) dX = sqrt( (dRadiusXY * xx) / dX );
									if( dY > IPP_EPS52 ) dY = sqrt( (dRadiusXY * yy) / dY );

									dX0 -= dX;
									dY0 -= dY;

									double dR = sqrt( dX0 * dX0 + dY0 * dY0 );
									double t = 0;

									if( (xx * dRcpRadiusX + yy * dRcpRadiusY) < 1 )
									{
										if( dR > dRoundRadius )
										{
											continue;
										}

										t = dRoundRadius - dR;
									}
									else
									{
										if( dR > dRoundRadius )
										{
											pPixels[0] = oTable[pPixels[0]];
											pPixels[1] = oTable[pPixels[1]];
											pPixels[2] = oTable[pPixels[2]];
											continue;
										}

										t = dRoundRadius + dR;
									}

									t *= dRoundFactor;

									t = sin( t * IPP_PI2 );
									t = t * t * dAmount;

									int disp = Round( t );

									pPixels[0] = IntToByte( pPixels[0] + disp );
									pPixels[1] = IntToByte( pPixels[1] + disp );
									pPixels[2] = IntToByte( pPixels[2] + disp );
								}
							}
							else
							{
								if( x > dW )
								{
									if( x < dMinX )
										continue;

									if( x > dMaxX )
									{
										pPixels[0] = oTable[pPixels[0]];
										pPixels[1] = oTable[pPixels[1]];
										pPixels[2] = oTable[pPixels[2]];
										continue;
									}
									
									double t = (x - dMinX) * dRoundFactor;

									t = sin( t * IPP_PI2 );
									t = t * t * dAmount;

									int disp = Round( t );

									pPixels[0] = IntToByte( pPixels[0] + disp );
									pPixels[1] = IntToByte( pPixels[1] + disp );
									pPixels[2] = IntToByte( pPixels[2] + disp );
								}
							}
						}
					}
					
					return c_nErrorNone;
				}

#ifndef IMAGE_EDITOR
				int BGRA_EffectZoomingSphere( BYTE* pPixels, int nWidth, int nHeight, double Frame, double Power )
				{
					Frame = Clamp( Frame );

					if( Power < 1 )	 Power = 1;
					if( Power > 11 ) Power = 11;

					CBuffer<BYTE> oBuffer(nWidth*nHeight*4);
					CBuffer<float> oMapX(nWidth*nHeight);
					CBuffer<float> oMapY(nWidth*nHeight);

					if( oBuffer.IsEmpty() || oMapX.IsEmpty() || oMapY.IsEmpty() )
						return c_nErrorNoMemory;

					double dCenterX = (nWidth  - 1) / 2.0;
					double dCenterY = (nHeight - 1) / 2.0;
					double dFactorY = 1.0 / dCenterY;
					double dFactorX = 1.0 / dCenterX;

					float* pMapX = oMapX.GetPtr();
					float* pMapY = oMapY.GetPtr();
					
					const size_t nDispMapX = 0;
					const size_t nDispMapY = ((size_t)pMapY - (size_t)pMapX)/sizeof(float);

					double dExp = (Power - 1) * Frame;

					int nHalfWidth  = ((DWORD)nWidth  + 1) / 2;
					int nHalfHeight = ((DWORD)nHeight + 1) / 2;

					// площадь треугольника: S = |(x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)| / 2;
					double dS = abs(dCenterX * dCenterY);

					int nOffset1 = 0;
					int nOffset2 = (nHeight - 1) * nWidth;
					int nOffset3 = (nWidth  - 1);
					int nOffset4 = (nHeight - 1) * nWidth + (nWidth  - 1);

					for( int nRow = 0; nRow < nHalfHeight; ++nRow )
					{
						float* pMap1 = pMapX + nOffset1 + nRow * nWidth;
						float* pMap2 = pMapX + nOffset2 - nRow * nWidth;
						float* pMap3 = pMapX + nOffset3 + nRow * nWidth;
						float* pMap4 = pMapX + nOffset4 - nRow * nWidth;
						
						for( int nPos = 0; nPos < nHalfWidth; ++nPos, ++pMap1, ++pMap2, --pMap3, --pMap4 )
						{
							double dY = nRow;
							double dX = nPos;

							double dLenX = dX - dCenterX;
							double dLenY = dY - dCenterY;
							double dLength = dLenX * dLenX + dLenY * dLenY;

							if( dLength < 0.00001 )
							{
								pMap1[nDispMapX] = float(dX);
								pMap1[nDispMapY] = float(dY);
								
								continue;
							}

							double x = dX * dCenterY;
							double y = dY * dCenterX;
							double d = x - y;

							double dS1 = abs( y );
							double dS2 = abs( d ); //abs(dY * dCenterX - dX * dCenterY);
							double dS3 = abs( x - dCenterX * dCenterY );

							if( abs(dS - (dS1 + dS2 + dS3)) < IPP_EPS23 )
							{
								//x = d / (dCenterY - dY);
								//y = 0;
								x = d / dLenY + dCenterX;
								y = dCenterY;
							}
							else
							{
								//x = 0;
								//y = d / (dX - dCenterX);
								x = dCenterX;
								y = d / dLenX - dCenterY;
							}

							//y -= dCenterY;
							//x -= dCenterX;
							d = x * x  + y * y;
							d = sqrt( dLength / d );
							
							d = (1 - (1 - d) * pow( sin( IPP_PI2 * (1 - d) ), dExp )) / d;

							dX = dLenX * d + dCenterX;
							dY = dLenY * d + dCenterY;

							if( dX < 0 ) dX = 0;
							if( dY < 0 ) dY = 0;

							pMap1[nDispMapX] = float(dX);
							pMap2[nDispMapX] = float(dX);
							pMap3[nDispMapX] = float(nWidth  - 1 - dX);
							pMap4[nDispMapX] = float(nWidth  - 1 - dX);

							pMap1[nDispMapY] = float(dY);
							pMap3[nDispMapY] = float(dY);
							pMap2[nDispMapY] = float(nHeight - 1 - dY);
							pMap4[nDispMapY] = float(nHeight - 1 - dY);

							//pMap1[nDispMapX] = float(dX);
							//pMap1[nDispMapY] = float(dY);

							//pMap2[nDispMapX] = float(dX);
							//pMap2[nDispMapY] = float(nHeight - 1 - dY);

							//pMap3[nDispMapX] = float(nWidth  - 1 - dX);
							//pMap3[nDispMapY] = float(dY);

							//pMap4[nDispMapX] = float(nWidth  - 1 - dX);
							//pMap4[nDispMapY] = float(nHeight - 1 - dY);
						}
					}

					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};
					IppiSize DstSize = {nWidth, nHeight};


					ippiRemap_8u_C4R( pPixels, SrcSize, nWidth*4, SrcRect, 
						oMapX.GetPtr(), sizeof(Ipp32f)*nWidth, 
						oMapY.GetPtr(), sizeof(Ipp32f)*nWidth, 
						oBuffer.GetPtr(), nWidth*4, DstSize, IPPI_INTER_LINEAR);

					memcpy( pPixels, oBuffer.GetPtr(), nWidth * nHeight * 4 );

					
					return c_nErrorNone;
				}

				int BGRA_EffectColorExchange(BYTE* pPixels, int nWidth, int nHeight, double Frame, int nColor1, int nColor2 )
				{
					if( nColor1 < 0 || nColor2 < 0 || nColor1 == nColor2 )
						return c_nErrorNone;

					//BYTE bgr1[4] = {
					//	BYTE((nColor1 & 0x00ff0000) >> 24),
					//	BYTE((nColor1 & 0x0000ff00) >> 16),
					//	BYTE((nColor1 & 0x000000ff) >> 8),
					//	0
					//};

					//BYTE bgr2[4] = {
					//	BYTE((nColor2 & 0x00ff0000) >> 24),
					//	BYTE((nColor2 & 0x0000ff00) >> 16),
					//	BYTE((nColor2 & 0x000000ff) >> 8),
					//	0
					//};

					//double hls1[3];
					//double hls2[3];

					//BgrToHls( bgr1, hls1 );
					//BgrToHls( bgr2, hls2 );

					DWORD dwSrcAlpha = DWORD(255 * Clamp( Frame ) + 0.5);
					DWORD dwDstAlpha = 255 - dwSrcAlpha;

					dwSrcAlpha *= 0x8081U;
					dwDstAlpha *= 0x8081U;
					
					int nB1 = (nColor1 & 0x00ff0000) >> 16;
					int nG1 = (nColor1 & 0x0000ff00) >> 8;
					int nR1 = (nColor1 & 0x000000ff);

					int nB2 = (nColor2 & 0x00ff0000) >> 16;
					int nG2 = (nColor2 & 0x0000ff00) >> 8;
					int nR2 = (nColor2 & 0x000000ff);

					const int nMaxRadius = 50;

					int nSize = nWidth * nHeight;

					for( int i = 0; i < nSize; ++i, pPixels += 4 )
					{
						int b = pPixels[0] - nB1;
						int g = pPixels[1] - nG1;
						int r = pPixels[2] - nR1;

						int s = b * b + g * g + r * r;
						if( s > (nMaxRadius * nMaxRadius) )
							continue;

						if( !s )
						{
							b = nB2;
							g = nG2;
							r = nR2;
						}
						else
						{
							double dLen = sin(sqrt( double(s) ) / nMaxRadius * IPP_PI2);
							//double dBaseB = b / dLen + pPixels[0];
							//double dBaseG = g / dLen + pPixels[1];
							//double dBaseR = r / dLen + pPixels[2];

							b = RealToByte(b + (nB1 - nB2) * dLen + nB2);
							g = RealToByte(g + (nG1 - nG2) * dLen + nG2);
							r = RealToByte(r + (nR1 - nR2) * dLen + nR2);
						}

						if( dwSrcAlpha < (255 * 0x8081) )
						{
							b = (DWORD(b) * dwSrcAlpha + pPixels[0] * dwDstAlpha + 128 * 0x8081U) >> 23;
							g = (DWORD(g) * dwSrcAlpha + pPixels[1] * dwDstAlpha + 128 * 0x8081U) >> 23;
							r = (DWORD(r) * dwSrcAlpha + pPixels[2] * dwDstAlpha + 128 * 0x8081U) >> 23;
						}

						pPixels[0] = b;
						pPixels[1] = g;
						pPixels[2] = r;
					}

					return c_nErrorNone;
				}
				int BGRA_EffectShapeMosaic(BYTE* pPixels, int nWidth, int nHeight, double Frame, int nMosaicSize )
				{
					nMosaicSize = Round(nMosaicSize * Clamp(Frame) );
					
					if( nMosaicSize < 4 )
						return c_nErrorNone;

					if( nMosaicSize > 4096 )
						nMosaicSize = 4096;

					int nDispX = 0;
					int nDispY = 0;

					if( nMosaicSize < nWidth )
					{
						nDispX = (nWidth  - nMosaicSize) / 2 % nMosaicSize;
						if( nDispX > 0 )
							nDispX -= nMosaicSize;
					}

					if( nMosaicSize < nHeight )
					{
						nDispY = (nHeight - nMosaicSize) / 2 % nMosaicSize;
						if( nDispY > 0 )
							nDispY -= nMosaicSize;
					}

					int nCountTilesX = (abs(nDispX) + nWidth  + nMosaicSize - 1) / nMosaicSize;
					int nCountTilesY = (abs(nDispY) + nHeight + nMosaicSize - 1) / nMosaicSize;

					CBuffer<BYTE> pBuffer( nCountTilesX * nCountTilesY * 3 );
					if( pBuffer.IsEmpty() )
						return c_nErrorNoMemory;

					BYTE* pClr = pBuffer.GetPtr();
					

					for( int nPosTileY = nDispY; nPosTileY < nHeight; nPosTileY += nMosaicSize )
					{
						int nTileY = max(nPosTileY, 0);
						int nTileH = min(nPosTileY + nMosaicSize, nHeight) - nTileY;
						
						BYTE* pSrcY = pPixels + nTileY * nWidth * 4;

						for( int nPosTileX = nDispX; nPosTileX < nWidth; nPosTileX += nMosaicSize )
						{
							int nTileX = max(nPosTileX, 0);
							int nTileW = min(nPosTileX + nMosaicSize, nWidth) - nTileX;
							
							double dSumR = 0;
							double dSumG = 0;
							double dSumB = 0;
							double dSumA = 0;

							int step = (nWidth - nTileW) * 4;
							BYTE* src = pSrcY + nTileX * 4;

							for( int y = 0; y < nTileH; ++y, src += step )
							{
								for( int x = 0; x < nTileW; ++x, src += 4 )
								{
									if( src[3] > 0 )
									{
										double alpha = src[3] * (1.0/255.0);
										dSumR += src[2] * alpha;
										dSumG += src[1] * alpha;
										dSumB += src[0] * alpha;
										dSumA += alpha;
									}
								}
							}

							if( dSumA <= 0 )
							{
								pClr[0] = pClr[1] = pClr[2] = 0;
								pClr += 3;
								continue;
							}

							dSumR /= dSumA;							
							dSumG /= dSumA;							
							dSumB /= dSumA;

							int R = int( dSumR + 0.5 );
							int G = int( dSumG + 0.5 );
							int B = int( dSumB + 0.5 );

							pClr[0] = B;
							pClr[1] = G;
							pClr[2] = R;
							pClr += 3;

							BYTE* dst = pSrcY + nTileX * 4;

							for( int y = 0; y < nTileH; ++y, dst += step )
							{
								for( int x = 0; x < nTileW; ++x, dst += 4 )
								{
									if( dst[3] > 0 )
									{	
										dst[0] = B;
										dst[1] = G;
										dst[2] = R;
									}
								}
							}
						}
					}

					// отрисовка граней

					pClr = pBuffer.GetPtr();

					for( int nPosTileY = nDispY; nPosTileY < nHeight; nPosTileY += nMosaicSize )
					{
						int nTileY = max(nPosTileY, 0);
						int nTileH = min(nPosTileY + nMosaicSize, nHeight) - nTileY;
						
						BYTE* pSrcY = pPixels + nTileY * nWidth * 4;

						for( int nPosTileX = nDispX; nPosTileX < nWidth; nPosTileX += nMosaicSize, pClr += 3 )
						{
							int nTileX = max(nPosTileX, 0);
							int nTileW = min(nPosTileX + nMosaicSize, nWidth) - nTileX;
						
							BYTE* src = pSrcY + nTileX * 4;

							const DWORD a1 = 70;
							const DWORD a2 = 40;

							int nL = Intensity2( pClr[2], pClr[1], pClr[0] );

							DWORD nB1 = pClr[0] * (255 - a1) * 0x8081U;
							DWORD nG1 = pClr[1] * (255 - a1) * 0x8081U;
							DWORD nR1 = pClr[2] * (255 - a1) * 0x8081U;
							DWORD nB2 = pClr[0] * (255 - a2) * 0x8081U;
							DWORD nG2 = pClr[1] * (255 - a2) * 0x8081U;
							DWORD nR2 = pClr[2] * (255 - a2) * 0x8081U;

							// top
							if( nTileY > 0 )
							{
								int l = Intensity2( 
									pClr[2 - nCountTilesX * 3], 
									pClr[1 - nCountTilesX * 3], 
									pClr[0 - nCountTilesX * 3] );

								if( nL != l )
								{
									DWORD b = nB1;
									DWORD g = nG1;
									DWORD r = nR1;

									if( nL > l )
									{
										b += 255 * a1 * 0x8081U;
										g += 255 * a1 * 0x8081U;
										r += 255 * a1 * 0x8081U;
									}

									b >>= 23;
									g >>= 23;
									r >>= 23;

									BYTE* border = pSrcY + nTileX * 4;
									for( int i = 0; i < nTileW - 1; ++i, border += 4 )
									{
										if( border[3] > 0 )
										{
											border[0] = (BYTE)b;
											border[1] = (BYTE)g;
											border[2] = (BYTE)r;
										}
									}

									if( nTileH > 1 )
									{
										DWORD b = nB2;
										DWORD g = nG2;
										DWORD r = nR2;

										if( nL > l )
										{
											b += 255 * a2 * 0x8081U;
											g += 255 * a2 * 0x8081U;
											r += 255 * a2 * 0x8081U;
										}

										b >>= 23;
										g >>= 23;
										r >>= 23;

										BYTE* border = pSrcY + nTileX * 4 + nWidth * 4 + 4;
										for( int i = 0; i < nTileW - 3; ++i, border += 4 )
										{
											if( border[3] > 0 )
											{
												border[0] = (BYTE)b;
												border[1] = (BYTE)g;
												border[2] = (BYTE)r;
											}
										}
									}
								}
							}

							// left
							if( nTileX > 0 )
							{
								int l = Intensity2( 
									pClr[2 - 3], 
									pClr[1 - 3], 
									pClr[0 - 3] );

								if( nL != l )
								{
									DWORD b = nB1;
									DWORD g = nG1;
									DWORD r = nR1;

									if( nL > l )
									{
										b += 255 * a1 * 0x8081U;
										g += 255 * a1 * 0x8081U;
										r += 255 * a1 * 0x8081U;
									}

									b >>= 23;
									g >>= 23;
									r >>= 23;

									BYTE* border = pSrcY + nTileX * 4 + nWidth * 4;
									for( int i = 0; i < nTileH - 1; ++i, border += nWidth * 4 )
									{
										if( border[3] > 0 )
										{
											border[0] = (BYTE)b;
											border[1] = (BYTE)g;
											border[2] = (BYTE)r;
										}
									}

									if( nTileW > 1 )
									{
										DWORD b = nB2;
										DWORD g = nG2;
										DWORD r = nR2;

										if( nL > l )
										{
											b += 255 * a2 * 0x8081U;
											g += 255 * a2 * 0x8081U;
											r += 255 * a2 * 0x8081U;
										}

										b >>= 23;
										g >>= 23;
										r >>= 23;

										BYTE* border = pSrcY + nTileX * 4 + nWidth * 4 + nWidth * 4 + 4;
										for( int i = 0; i < nTileH - 3; ++i, border += nWidth * 4 )
										{
											if( border[3] > 0 )
											{
												border[0] = (BYTE)b;
												border[1] = (BYTE)g;
												border[2] = (BYTE)r;
											}
										}
									}
								}
							}

							// right
							if( nTileX < nWidth - nMosaicSize )
							{
								int l = Intensity2( 
									pClr[2 + 3], 
									pClr[1 + 3], 
									pClr[0 + 3] );

								if( nL != l )
								{
									DWORD b = nB1;
									DWORD g = nG1;
									DWORD r = nR1;

									if( nL > l )
									{
										b += 255 * a1 * 0x8081U;
										g += 255 * a1 * 0x8081U;
										r += 255 * a1 * 0x8081U;
									}

									b >>= 23;
									g >>= 23;
									r >>= 23;

									BYTE* border = pSrcY + nTileX * 4 + (nTileW - 1) * 4;
									for( int i = 0; i < nTileH - 1; ++i, border += nWidth * 4 )
									{
										if( border[3] > 0 )
										{
											border[0] = (BYTE)b;
											border[1] = (BYTE)g;
											border[2] = (BYTE)r;
										}
									}

									if( nTileW > 1 )
									{
										DWORD b = nB2;
										DWORD g = nG2;
										DWORD r = nR2;

										if( nL > l )
										{
											b += 255 * a2 * 0x8081U;
											g += 255 * a2 * 0x8081U;
											r += 255 * a2 * 0x8081U;
										}

										b >>= 23;
										g >>= 23;
										r >>= 23;

										BYTE* border = pSrcY + nTileX * 4 + (nTileW - 1) * 4 + nWidth * 4 - 4;
										for( int i = 0; i < nTileH - 1; ++i, border += nWidth * 4 )
										{
											if( border[3] > 0 )
											{
												border[0] = (BYTE)b;
												border[1] = (BYTE)g;
												border[2] = (BYTE)r;
											}
										}
									}
								}
							}

							// bottom
							if( nTileY < nHeight - nMosaicSize )
							{

								int l = Intensity2( 
									pClr[2 + nCountTilesX * 3], 
									pClr[1 + nCountTilesX * 3], 
									pClr[0 + nCountTilesX * 3] );

								if( nL != l )
								{
									DWORD b = nB1;
									DWORD g = nG1;
									DWORD r = nR1;

									if( nL > l )
									{
										b += 255 * a1 * 0x8081U;
										g += 255 * a1 * 0x8081U;
										r += 255 * a1 * 0x8081U;
									}

									b >>= 23;
									g >>= 23;
									r >>= 23;

									BYTE* border = pSrcY + nTileX * 4 + (nTileH - 1) * nWidth * 4 + 4;
									for( int i = 0; i < nTileW - 1; ++i, border += 4 )
									{
										if( border[3] > 0 )
										{
											border[0] = (BYTE)b;
											border[1] = (BYTE)g;
											border[2] = (BYTE)r;
										}
									}

									if( nTileH > 1 )
									{
										DWORD b = nB2;
										DWORD g = nG2;
										DWORD r = nR2;

										if( nL > l )
										{
											b += 255 * a2 * 0x8081U;
											g += 255 * a2 * 0x8081U;
											r += 255 * a2 * 0x8081U;
										}

										b >>= 23;
										g >>= 23;
										r >>= 23;

										BYTE* border = pSrcY + nTileX * 4 + (nTileH - 1) * nWidth * 4 + 4 - nWidth * 4 + 4;
										for( int i = 0; i < nTileW - 1; ++i, border += 4 )
										{
											if( border[3] > 0 )
											{
												border[0] = (BYTE)b;
												border[1] = (BYTE)g;
												border[2] = (BYTE)r;
											}
										}
									}
								}
							}
							
						}
					}

					
					
					return c_nErrorNone;
				}
				int BGRA_EffectCanvas( BYTE* pPixels, int nWidth, int nHeight, double dFrame, double dAlpha, DWORD BaseRnd )
				{
					dAlpha = Clamp( dAlpha / 100  );
					dFrame = Clamp( dFrame );

					dAlpha *= dFrame;
					if( dAlpha < 0.001 )
						return c_nErrorNone;

					CRandom rnd( BaseRnd );

					int nSize = nWidth * nHeight;

					CBuffer<BYTE> oMask( nSize );
					if( oMask.IsEmpty() )
						return c_nErrorNoMemory;

					::memset( oMask.GetPtr(), 0, nSize );

					//BYTE* pMask = oMask.GetPtr();

					const DWORD alpha = 215;
					const DWORD color = 255;

					for( int y = 0; y < nHeight; y += 2 )
					{
						for( int x = 0; x < nWidth; x += 2 )
						{
							BYTE* pMask = oMask.GetPtr() + y * nWidth + x;

							if( pMask[0] > 0 )
								continue;

							if( rnd.Rand( 15 ) > 0 )
								continue;

							int nLenSize = rnd.Rand( 20 ) + 3;
							int nDir = rnd.Rand( 2 );

							if( nDir > 0 ) // horizontal
							{
								if( x + nLenSize > nWidth )
								{
									nLenSize = nWidth - x;
								}

								for( int i = 0; i < nLenSize; ++i )
								{
									pMask[i] = rnd.Rand( 32 ) + 48;
								}

								if( y < nHeight - 1 )
								{
									for( int i = 0; i < nLenSize; ++i )
									{
										pMask[nWidth + i] = BYTE(((rnd.Rand( 32 ) + color - 32) * alpha + pMask[i] * (255 - alpha) + 128) * 0x8081U >> 23);
									}
								}
							}
							else
							{
								if( y + nLenSize > nHeight )
								{
									nLenSize = nHeight - y;
								}

								for( int i = 0; i < nLenSize; ++i )
								{
									pMask[i * nWidth] = rnd.Rand( 32 ) + 48;
								}

								if( x < nWidth - 1 )
								{
									for( int i = 0; i < nLenSize; ++i )
									{
										pMask[i * nWidth + 1] = BYTE(((rnd.Rand( 32 ) + color - 32) * alpha + pMask[i * nWidth] * (255 - alpha) + 128) * 0x8081U >> 23);
									}
								}
							}
						}
					}

					DWORD dwBaseAlpha = DWORD(dAlpha * 255 + 0.5);
					BYTE* pMask = oMask.GetPtr();

					for( int i = 0; i < nSize; ++i, ++pMask, pPixels += 4 )
					{
						if( !pMask[0] )
							continue;

						DWORD dwColor = 255 - pMask[0];
						DWORD dwAlpha = (pMask[0] * dwBaseAlpha + 128) * 0x8081 >> 23;

						pPixels[0] = BYTE((dwColor * dwAlpha + pPixels[0] * (255 - dwAlpha) + 128) * 0x8081U >> 23);
						pPixels[1] = BYTE((dwColor * dwAlpha + pPixels[1] * (255 - dwAlpha) + 128) * 0x8081U >> 23);
						pPixels[2] = BYTE((dwColor * dwAlpha + pPixels[2] * (255 - dwAlpha) + 128) * 0x8081U >> 23);
					}

					
					return c_nErrorNone;
				}

				
				int BGRA_TransformFlip(BYTE* pPixels, int nWidth, int nHeight, double Frame, BOOL Vertical, int nBackColor, int nBackAlpha )
				{
					// clamp values
					Frame = Clamp(Frame);

					if( Frame < 0.0001 )
						return c_nErrorNone;

					if( Frame > 0.9999 )
					{
						LPDWORD pSrc = (LPDWORD)pPixels;
						LPDWORD pDst = (LPDWORD)pPixels;
						
						if( Vertical )
						{
							pSrc += nWidth * (nHeight - 1);

							for( int nY = nHeight / 2; nY > 0; --nY, pSrc -= nWidth * 2 )
							{
								for( int nX = nWidth; nX > 0; --nX, ++pSrc, ++pDst )
								{
									DWORD buf = *pDst;
									*pDst = *pSrc;
									*pSrc = buf;
								}
							}
						}
						else
						{
							pSrc += nWidth - 1;

							for( int nY = nHeight; nY > 0; --nY, pDst += nWidth - nWidth / 2, pSrc += nWidth + nWidth / 2 )
							{
								for( int nX = nWidth / 2; nX > 0; --nX, --pSrc, ++pDst )
								{
									DWORD buf = *pDst;
									*pDst = *pSrc;
									*pSrc = buf;
								}
							}
						}

						return c_nErrorNone;
					}


					CBuffer<BYTE> oBackup(nWidth*nHeight*4);
					if( oBackup.IsEmpty() )
						return c_nErrorNoMemory;
					
					BYTE* pBackup = oBackup.GetPtr();
					memcpy( pBackup, pPixels, nWidth*nHeight*4 );

					// версия с Gdi+ работает немного медленей чем обычная реализация, зато качественней за счёт интерполяции!
					Gdiplus::Bitmap oSourceImage( nWidth, nHeight, nWidth*4, PixelFormat32bppARGB, pBackup );
					Gdiplus::Bitmap oResultImage( nWidth, nHeight, nWidth*4, PixelFormat32bppARGB, pPixels );
					if( oResultImage.GetLastStatus() != Gdiplus::Ok ||
						oSourceImage.GetLastStatus() != Gdiplus::Ok )
					{
						return c_nErrorNoMemory;
					}
					
					Gdiplus::Graphics oRender( &oResultImage );
					if( oRender.GetLastStatus() != Gdiplus::Ok )
					{
						return c_nErrorNoMemory;
					}

					oRender.Clear( GetColor( nBackColor, nBackAlpha ) );

					Gdiplus::PointF offset( float(nWidth / 2.0), float(nHeight / 2.0) );
					Gdiplus::PointF scale( 1.0f, 1.0f );

					if( Vertical )
						scale.Y = float(1 - Frame * 2);
					else
						scale.X = float(1 - Frame * 2);

					if( abs(scale.X) < 0.0001 || abs(scale.Y) < 0.0001 )
					{
						return c_nErrorNone;
					}
					
					oRender.SetInterpolationMode( Gdiplus::InterpolationModeBilinear );
					
					oRender.TranslateTransform( -offset.X, -offset.Y, MatrixOrderAppend );
					oRender.ScaleTransform( scale.X, scale.Y, MatrixOrderAppend );
					oRender.TranslateTransform( offset.X, offset.Y, MatrixOrderAppend );

					oRender.DrawImage( &oSourceImage, 0.f, 0.f );


					return c_nErrorNone;
				}
				int BGRA_TransformFlip2(BYTE* pPixels, int nWidth, int nHeight, double Frame, int nDir, int nBackColor, int nBackAlpha )
				{
					// dir: (1, 2, 3, 4, 5, 6, 7, 8) 2 - вверх, 4 - вправо, 6 - вниз, 8 - влево

					if( Frame < 0.0001 )
						return c_nErrorNone;

					if( nDir < 1 || nDir > 8 )
						return c_nErrorNone;

					if( Frame > 0.9999 )
					{
						if( nDir != 4 && nDir != 8 )
						{
							LPDWORD pSrc = (LPDWORD)pPixels;
							LPDWORD pDst = (LPDWORD)pPixels;

							pSrc += nWidth * (nHeight - 1);

							for( int nY = nHeight / 2; nY > 0; --nY, pSrc -= nWidth * 2 )
							{
								for( int nX = nWidth; nX > 0; --nX, ++pSrc, ++pDst )
								{
									DWORD buf = *pDst;
									*pDst = *pSrc;
									*pSrc = buf;
								}
							}
						}

						if( nDir != 2 && nDir != 6 )
						{
							LPDWORD pSrc = (LPDWORD)pPixels;
							LPDWORD pDst = (LPDWORD)pPixels;

							pSrc += nWidth - 1;

							for( int nY = nHeight; nY > 0; --nY, pDst += nWidth - nWidth / 2, pSrc += nWidth + nWidth / 2 )
							{
								for( int nX = nWidth / 2; nX > 0; --nX, --pSrc, ++pDst )
								{
									DWORD buf = *pDst;
									*pDst = *pSrc;
									*pSrc = buf;
								}
							}
						}

						return c_nErrorNone;
					}

					int nSize = nWidth * nHeight * 4;

					CBuffer<BYTE> oSource1(nSize);
					CBuffer<BYTE> oSource2(nSize);

					if( oSource1.IsEmpty() || oSource2.IsEmpty() )
						return c_nErrorNoMemory;
					
					::memcpy( oSource1.GetPtr(), pPixels, nSize);
					::memcpy( oSource2.GetPtr(), pPixels, nSize);

					if( nDir != 4 && nDir != 8 )
					{
						LPDWORD pSrc = (LPDWORD)oSource2.GetPtr();
						LPDWORD pDst = (LPDWORD)oSource2.GetPtr();

						pSrc += nWidth * (nHeight - 1);

						for( int nY = nHeight / 2; nY > 0; --nY, pSrc -= nWidth * 2 )
						{
							for( int nX = nWidth; nX > 0; --nX, ++pSrc, ++pDst )
							{
								DWORD buf = *pDst;
								*pDst = *pSrc;
								*pSrc = buf;
							}
						}
					}

					if( nDir != 2 && nDir != 6 )
					{
						LPDWORD pSrc = (LPDWORD)oSource2.GetPtr();
						LPDWORD pDst = (LPDWORD)oSource2.GetPtr();

						pSrc += nWidth - 1;

						for( int nY = nHeight; nY > 0; --nY, pDst += nWidth - nWidth / 2, pSrc += nWidth + nWidth / 2 )
						{
							for( int nX = nWidth / 2; nX > 0; --nX, --pSrc, ++pDst )
							{
								DWORD buf = *pDst;
								*pDst = *pSrc;
								*pSrc = buf;
							}
						}
					}

					//if( nDir != 4 && nDir != 8 )
					//{
					//	LPDWORD pSrc = (LPDWORD)pPixels;
					//	LPDWORD pDst = (LPDWORD)oSource2.GetPtr();

					//	pSrc += nWidth * (nHeight - 1);

					//	for( int nY = 0; nY < nHeight; ++nY, pSrc -= nWidth * 2 )
					//	{
					//		for( int nX = 0; nX < nWidth; ++nX, ++pSrc, ++pDst )
					//		{
					//			*pDst = *pSrc;
					//		}
					//	}
					//}
					//
					//if( nDir != 2 && nDir != 6 )
					//{
					//	LPDWORD pSrc = (LPDWORD)pPixels;
					//	LPDWORD pDst = (LPDWORD)oSource2.GetPtr();

					//	pSrc += nWidth - 1;

					//	for( int nY = 0; nY < nHeight; ++nY, pSrc += nWidth * 2 )
					//	{
					//		for( int nX = 0; nX < nWidth; ++nX, --pSrc, ++pDst )
					//		{
					//			*pDst = *pSrc;
					//		}
					//	}
					//}

					BOOL bResult = FALSE;
					
					if( nDir == 2 || nDir == 4 || nDir == 6 || nDir == 8 )
					{
						BOOL bHorizontal = (nDir == 4 || nDir == 8) ? TRUE : FALSE;
						BOOL bFarward = (nDir == 4 || nDir == 6) ? TRUE : FALSE;

						bResult = ImageStudio::Paint::Compose::IPP::IPP_TwistLinear( 
							oSource1.GetPtr(), oSource2.GetPtr(), pPixels, nWidth, nHeight, Frame, bHorizontal, bFarward, GetColor( nBackColor, nBackAlpha ).GetValue() );
					}
					else
					{
						int nDiagonal = 0;
						switch( nDir )
						{
						case 1: nDiagonal = 2; break;
						case 3: nDiagonal = 0; break;
						case 5: nDiagonal = 1; break;
						case 7: nDiagonal = 3; break;
						}

						bResult = ImageStudio::Paint::Compose::IPP::IPP_TwistDiagonal(
							oSource1.GetPtr(), oSource2.GetPtr(), pPixels, nWidth, nHeight, Frame, nDiagonal, GetColor( nBackColor, nBackAlpha ).GetValue() );
					}

					return bResult ? c_nErrorNone : c_nErrorNoMemory;
				}
				int BGRA_TransformPerspective(BYTE* pPixels, int nWidth, int nHeight, double Frame, BOOL Vertical, double Angle, int nBackColor, int nBackAlpha )
				{
					// clamp values
					Frame = Clamp(Frame);
					Angle = Frame*min(max(-90, Angle), 90);

					// check for simpliest case
					if (fabs(Angle) < 0.001)
						return c_nErrorNone;

					// begin effect
					BYTE* pBackup = new BYTE[nWidth*nHeight*4];
					memcpy( pBackup, pPixels, nWidth*nHeight*4 );
					
					// clear image
					ClearImage( pPixels, nWidth, nHeight, nWidth*4, nBackColor, nBackAlpha );

					// variables
					int nY1, nY2, nX1, nX2;
					Ipp8u* pSrc = pBackup;
					Ipp8u* pDst = pPixels;
					IppiRect srcRoi = {0, 0, nWidth, nHeight};
					double quadSrc[4][2], quadDst[4][2];
					quadSrc[0][0] = quadSrc[3][0] = 0;
					quadSrc[1][0] = quadSrc[2][0] = nWidth - 1;
					quadSrc[0][1] = quadSrc[1][1] = 0;
					quadSrc[2][1] = quadSrc[3][1] = nHeight - 1;

					// check for transform center line (vertical or horizontal)
					if (Vertical)
					{
						// compute resulting x coordinates
						nX1 = (int)(0.5*nWidth*fabs(Angle)/90);
						nX2 = nWidth - 1 - nX1;

						// compute resulting y coordinates
						nY1 = (int)(0.25*nHeight*fabs(Angle)/90);
						nY2 = nHeight - 1 - nY1;

						quadDst[0][0] = quadDst[3][0] = nX1;
						quadDst[1][0] = quadDst[2][0] = nX2;
						
						if (Angle < 0)
						{
							quadDst[0][1] = 0;
							quadDst[3][1] = nHeight - 1;
							quadDst[1][1] = nY1;
							quadDst[2][1] = nY2;
						}
						else
						{
							quadDst[0][1] = nY1;
							quadDst[3][1] = nY2;
							quadDst[1][1] = 0;
							quadDst[2][1] = nHeight - 1;
						}
					}
					else
					{
						// compute resulting x coordinates
						nX1 = (int)(0.25*nWidth*fabs(Angle)/90);
						nX2 = nWidth - 1 - nX1;

						// compute resulting y coordinates
						nY1 = (int)(0.5*nHeight*fabs(Angle)/90);
						nY2 = nHeight - 1 - nY1;

						quadDst[0][1] = quadDst[1][1] = nY1;
						quadDst[2][1] = quadDst[3][1] = nY2;
						
						if (Angle > 0)
						{
							quadDst[0][0] = nX1;
							quadDst[1][0] = nX2;
							quadDst[2][0] = nWidth - 1;
							quadDst[3][0] = 0;
						}
						else
						{
							quadDst[0][0] = 0;
							quadDst[1][0] = nWidth - 1;
							quadDst[2][0] = nX2;
							quadDst[3][0] = nX1;
						}
					}

					// apply perspective correction
					IppiSize ImageSize = {nWidth, nHeight};
					ippiWarpPerspectiveQuad_8u_C4R(pSrc, ImageSize, nWidth*4, srcRoi, quadSrc, pDst, nWidth*4, srcRoi, quadDst, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);

					delete [] pBackup;

					return c_nErrorNone;
				}

				int BGRA_TransformSkew(BYTE* pPixels, int nWidth, int nHeight, double Frame, BOOL Vertical, double Angle, int nBackColor, int nBackAlpha)
				{
					// clamp values
					Frame = Clamp(Frame);
					Angle = Frame*min(max(-90, Angle), 90);

					// check for simpliest case
					if (fabs(Angle) < 0.001)
						return c_nErrorNone;

					// begin effect
					BYTE* pBackup = new BYTE[nWidth*nHeight*4];
					memcpy( pBackup, pPixels, nWidth*nHeight*4 );
					
					// clear image
					ClearImage( pPixels, nWidth, nHeight, nWidth*4, nBackColor, nBackAlpha );

					// variables
					int nY1, nY2, nX1, nX2;
					Ipp8u* pSrc = pBackup;
					Ipp8u* pDst = pPixels;
					IppiRect srcRoi = {0, 0, nWidth, nHeight};
					double quadSrc[4][2], quadDst[4][2];
					quadSrc[0][0] = quadSrc[3][0] = 0;
					quadSrc[1][0] = quadSrc[2][0] = nWidth - 1;
					quadSrc[0][1] = quadSrc[1][1] = 0;
					quadSrc[2][1] = quadSrc[3][1] = nHeight - 1;

					// compute resulting x coordinates
					nX1 = (int)(0.5*nWidth*fabs(Angle)/90);
					nX2 = nWidth - 1 - nX1;

					// compute resulting y coordinates
					nY1 = (int)(0.5*nHeight*fabs(Angle)/90);
					nY2 = nHeight - 1 - nY1;

					// check for transform direction
					if (Vertical)
					{
						quadDst[0][0] = quadDst[3][0] = nX1;
						quadDst[1][0] = quadDst[2][0] = nX2;
						
						if (Angle > 0)
						{
							quadDst[0][1] = nY1;
							quadDst[1][1] = 0;
							quadDst[2][1] = nY2;
							quadDst[3][1] = nHeight - 1;
						}
						else
						{
							quadDst[0][1] = 0;
							quadDst[1][1] = nY1;
							quadDst[2][1] = nHeight - 1;
							quadDst[3][1] = nY2;
						}
					}
					else
					{
						quadDst[0][1] = quadDst[1][1] = nY1;
						quadDst[2][1] = quadDst[3][1] = nY2;
						
						if (Angle > 0)
						{
							quadDst[0][0] = nX1;
							quadDst[1][0] = nWidth - 1;
							quadDst[2][0] = nX2;
							quadDst[3][0] = 0;
						}
						else
						{
							quadDst[0][0] = 0;
							quadDst[1][0] = nX2;
							quadDst[2][0] = nWidth - 1;
							quadDst[3][0] = nX1;
						}
					}

					// apply skew correction
					IppiSize ImageSize = {nWidth, nHeight};
					ippiWarpAffineQuad_8u_C4R(pSrc, ImageSize, nWidth*4, srcRoi, quadSrc, pDst, nWidth*4, srcRoi, quadDst, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);

					delete [] pBackup;

					return c_nErrorNone;
				}

				int BGRA_TransformShift(BYTE* pPixels, int nWidth, int nHeight, int nStride, double Frame, long X, long Y, int nBackColor, int nBackAlpha)
				{
					// clamp values
					Frame = Clamp(Frame);
					X = (int)(Frame * X);
					Y = (int)(Frame * (nStride < 0 ? -Y : Y));

					// check for simpliest case
					if (X == 0 && Y == 0)
						return c_nErrorNone;
					
					if (X >= nWidth || Y >= nHeight || X + nWidth <= 0 || Y + nHeight <= 0)
					{
						ClearImage( pPixels, nWidth, nHeight, nWidth*4, nBackColor, nBackAlpha );
						return c_nErrorNone;
					}

					// begin effect
					BYTE* pBackup = new BYTE[nWidth*nHeight*4];
					if( !pBackup )
						return c_nErrorNoMemory;
					
					memcpy( pBackup, pPixels, nWidth*nHeight*4 );
					
					// clear image
					ClearImage( pPixels, nWidth, nHeight, nWidth*4, nBackColor, nBackAlpha );

					IppiSize dstRoi = {nWidth - abs(X), nHeight - abs(Y)};

					Ipp8u* pSrc = pBackup;
					Ipp8u* pDst = pPixels;

					if (X >= 0 && Y >= 0)
					{
						pDst += (nWidth * Y + X) * 4;
					}
					else if (X < 0 && Y >= 0)
					{
						pSrc -= X * 4;
						pDst += nWidth * Y * 4;
					}
					else if (X >= 0 && Y < 0)
					{
						pSrc -= Y * nWidth * 4;
						pDst += X * 4;
					}
					else if (X < 0 && Y < 0)
					{
						pSrc -= (Y * nWidth + X) * 4;
					}
					
					ippiCopy_8u_C4R( pSrc, nWidth * 4, pDst, nWidth * 4, dstRoi);

					delete [] pBackup;

					return c_nErrorNone;
				}

				int BGRA_TransformRotate(BYTE* pPixels, int nWidth, int nHeight, double Frame, double Angle, int nBackColor, int nBackAlpha)
				{
					// clamp values
					Frame = Clamp(Frame);
					Angle = Frame*min(max(-360, Angle), 360);

					// check for simpliest case
					if (fabs(Angle) < 0.001 || Angle > 359.999 || Angle < -359.999)
						return c_nErrorNone;
					
					Ipp8u* pDst = pPixels;
					
					// check whether we flip image on 180 degrees
					IppiSize ImageSize = {nWidth, nHeight};
					if (fabs(fabs(Angle) - 180) < 0.001)
					{
						ippiMirror_8u_C4IR(pDst, nWidth*4, ImageSize, ippAxsBoth);
						return c_nErrorNone;
					}
					
					// begin effect
					BYTE* pBackup = new BYTE[nWidth*nHeight*4];
					memcpy( pBackup, pPixels, nWidth*nHeight*4 );
					
					// clear image
					ClearImage( pPixels, nWidth, nHeight, nWidth*4, nBackColor, nBackAlpha );
					
					// variables
					Ipp8u* pSrc = pBackup;
					IppiRect srcRoi = {0, 0, nWidth, nHeight};
					double dfCenterX = 0.5 * (nWidth - 1);
					double dfCenterY = 0.5 * (nHeight - 1);
					double quad[4][2];

					ippiGetRotateQuad(srcRoi, quad, Angle, 0, 0);

					double dLeft	= min(min(quad[0][0], quad[1][0]), min(quad[2][0], quad[3][0]));
					double dRight	= max(max(quad[0][0], quad[1][0]), max(quad[2][0], quad[3][0]));
					double dTop		= min(min(quad[0][1], quad[1][1]), min(quad[2][1], quad[3][1]));
					double dBottom	= max(max(quad[0][1], quad[1][1]), max(quad[2][1], quad[3][1]));

					double dResize = 1.0/max((int)(dRight - dLeft)/(double)nWidth, (int)(dBottom - dTop)/(double)nHeight);
					
					Ipp8u* pTmp = new Ipp8u[4*nWidth*nHeight];

					if (pTmp)
					{
						ClearImage( pTmp, nWidth, nHeight, nWidth*4, nBackColor, nBackAlpha );

						ippiResizeCenter_8u_C4R(pSrc, ImageSize, nWidth*4, srcRoi, pTmp, nWidth*4, ImageSize, dResize, dResize, dfCenterX, dfCenterY, IPPI_INTER_LINEAR);
						ippiRotateCenter_8u_C4R(pTmp, ImageSize, nWidth*4, srcRoi, pDst, nWidth*4, srcRoi, Angle, dfCenterX, dfCenterY, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE);
						
						delete[] pTmp;
					}

					delete [] pBackup;
					
					return c_nErrorNone;
				}

				int BGRA_TransformRotateMedia(BYTE* pPixels, int nWidth, int nHeight, int nStride, IUnknown** pMediaData, double Frame, double Angle, int nBackColor, int nBackAlpha, BOOL bNewSize )
				{
					// clamp values
					Frame = Clamp(Frame);
					Angle *= Frame;

					int nCount = Floor( Angle / 360 );
					Angle -= nCount * 360;

					// check for simpliest case
					if( Angle < 0.001 || Angle > 359.999 )
						return c_nErrorNone;

					int nType = 0;
					if( bNewSize )
					{
						if( abs(Angle -  90) < 0.001 ) nType = 1;
						if( abs(Angle - 270) < 0.001 ) nType = 2;
						if( abs(Angle - 180) < 0.001 ) nType = 3;
					}

					if( nType )
					{
						int nNewWidth  = (nType != 3) ? nHeight : nWidth;
						int nNewHeight = (nType != 3) ? nWidth : nHeight;

						IMediaFrame* pFrame = NULL;
						CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pFrame) );
						if( !pFrame )
							return c_nErrorNoMemory;

						pFrame->put_Width( nNewWidth );
						pFrame->put_Height( nNewHeight );
						pFrame->put_ColorSpace( CSP_BGRA );
						pFrame->SetDefaultStrides();
						pFrame->AllocateBuffer( -1 );
						
						BYTE* pDst = NULL;
						pFrame->get_Buffer( &pDst );
						if( !pDst )
						{
							pFrame->Release();
							return c_nErrorNoMemory;
						}

						long lBufSize = 0;
						pFrame->get_BufferSize( &lBufSize );
						pFrame->put_DataSize( lBufSize );

						pFrame->put_AspectRatioX( nNewWidth );
						pFrame->put_AspectRatioY( nNewHeight );

						*pMediaData = pFrame;

						ClearImage( pDst, nNewWidth, nNewHeight, nNewWidth*4, nBackColor, nBackAlpha );

						int nFactorX = 0;
						int nFactorY = 0;
						int nOffset  = 0;

						switch( nType )
						{
						case 1:
							nFactorX = nStride;
							nFactorY = -4;
							nOffset  = (nWidth - 1) * 4;
							break;
						case 2:
							nFactorX = -nStride;
							nFactorY = 4;
							nOffset  = (nHeight - 1 ) * nStride;
							break;
						case 3:
							nFactorX = -4;
							nFactorY = -nStride;
							nOffset  = (nWidth - 1) * 4 + (nHeight - 1) * nStride;
						}

						if( nStride < 0 )
						{
							pPixels -= nStride * (nHeight - 1);
						}

						for( int nRow = 0; nRow < nNewHeight; ++nRow )
						{
							for( int nPos = 0; nPos < nNewWidth; ++nPos, pDst += 4 )
							{
								int nIndex = nRow * nFactorY + nPos * nFactorX + nOffset;

								*((DWORD*)(pDst)) = *((DWORD*)(pPixels + nIndex));
							}
						}

						return c_nErrorNone;
					}
					else
					{
						// variables
						Ipp8u* pSrc = pPixels;
						IppiRect srcRoi = {0, 0, nWidth , nHeight};
						double quad[4][2];

						ippiGetRotateQuad(srcRoi, quad, Angle, 0, 0);

						double dLeft	= min(min(quad[0][0], quad[1][0]), min(quad[2][0], quad[3][0]));
						double dRight	= max(max(quad[0][0], quad[1][0]), max(quad[2][0], quad[3][0]));
						double dTop		= min(min(quad[0][1], quad[1][1]), min(quad[2][1], quad[3][1]));
						double dBottom	= max(max(quad[0][1], quad[1][1]), max(quad[2][1], quad[3][1]));

						int nNewWidth  = bNewSize ? Ceil(dRight - dLeft) : nWidth;
						int nNewHeight = bNewSize ? Ceil(dBottom - dTop) : nHeight;

						IMediaFrame* pFrame = NULL;
						CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pFrame) );
						if( !pFrame )
							return c_nErrorNoMemory;

						pFrame->put_Width( nNewWidth );
						pFrame->put_Height( nNewHeight );
						pFrame->put_ColorSpace( CSP_BGRA | (nStride < 0 ? CSP_VFLIP : 0) );
						pFrame->SetDefaultStrides();
						pFrame->AllocateBuffer( -1 );
						
						BYTE* pDst = NULL;
						pFrame->get_Buffer( &pDst );
						if( !pDst )
						{
							pFrame->Release();
							return c_nErrorNoMemory;
						}

						long lBufSize = 0;
						pFrame->get_BufferSize( &lBufSize );
						pFrame->put_DataSize( lBufSize );

						pFrame->put_AspectRatioX( nNewWidth );
						pFrame->put_AspectRatioY( nNewHeight );

						*pMediaData = pFrame;
												
						IppiSize dstSize = {nNewWidth, nNewHeight};
						IppiRect dstRoi = {0, 0, nNewWidth, nNewHeight};

						ClearImage( pDst, nNewWidth, nNewHeight, nNewWidth*4, nBackColor, nBackAlpha );

						double xShift = 0, yShift = 0;
						ippiGetRotateShift(0.5*(nWidth - 1), 0.5*(nHeight - 1), Angle, &xShift, &yShift);
						xShift += 0.5*(nNewWidth - nWidth);
						yShift += 0.5*(nNewHeight - nHeight);

						IppiSize ImageSize = {nWidth, nHeight};
						ippiRotate_8u_C4R(pSrc, ImageSize, nWidth*4, srcRoi, pDst, nNewWidth*4, dstRoi, Angle, xShift, yShift, IPPI_INTER_LINEAR | IPPI_SMOOTH_EDGE );
					}
					
					return c_nErrorNone;
				}

				int BGRA_TransformResample(BYTE* pPixels, int nWidth, int nHeight, double Frame, long Times)
				{
					//// clamp values
					Frame = Clamp(Frame);
					Times = min(max(1, Times), max(1, min(nWidth, nHeight)));

					if (fabs(Frame) < 0.001 || Times < 2)
						return c_nErrorNone;
					
					// begin effect
					BYTE* pBackup = new BYTE[nWidth*nHeight*4];
					memcpy( pBackup, pPixels, nWidth*nHeight*4 );
					
					// compute koeff
					double dKoef = 1.0 + Frame*(Times - 1);

					double dResize = 1.0 / dKoef;
					IppiSize ResizedRoi = {(int)(nWidth * dResize) , (int)(nHeight * dResize)};
					Ipp8u* pSrc = pBackup;
					Ipp8u* pDst = pPixels;
					IppiRect srcRoi = {0, 0, nWidth, nHeight};
					int nSrcOffset, nDstOffset;
					int nTileStep = ResizedRoi.width * 4;
					int nFullTilesX = 2 * ((nWidth - ResizedRoi.width)/(2 * ResizedRoi.width)) + 1;
					int nFullTilesY = 2 * ((nHeight - ResizedRoi.height)/(2 * ResizedRoi.height)) + 1;
					int nXBottom, nXTop, nYBottom, nYTop;

					nXBottom = nXTop = (nWidth - nFullTilesX * ResizedRoi.width);
					nYBottom = nYTop = (nHeight - nFullTilesY * ResizedRoi.height);
					nXTop /= 2; nYTop /= 2;
					nXBottom -= nXTop; 	nYBottom -= nYTop;

					nDstOffset = (nYTop * nWidth + nXTop)*4;

					IppiSize ImageSize = {nWidth, nHeight};
					ippiResize_8u_C4R(pSrc, ImageSize, nWidth*4 , srcRoi, pDst+nDstOffset, nWidth*4, ResizedRoi, dResize, dResize, IPPI_INTER_NN);
					nSrcOffset = nDstOffset;

					for (int i = 1; i < nFullTilesX; ++i)
					{
						nDstOffset += nTileStep;
						ippiCopy_8u_C4R(pDst + nSrcOffset, nWidth*4, pDst + nDstOffset, nWidth*4, ResizedRoi);
					}

					if (nXBottom > 0)
					{	
						nDstOffset += nTileStep;
						ResizedRoi.width = nXBottom;
						ippiCopy_8u_C4R(pDst + nSrcOffset, nWidth*4, pDst + nDstOffset, nWidth*4, ResizedRoi);
					}
					if (nXTop > 0)
					{
						nDstOffset = nSrcOffset - 4 * nXTop;
						nSrcOffset = nSrcOffset + nTileStep - 4 * nXTop;
						ResizedRoi.width = nXTop;
						ippiCopy_8u_C4R(pDst + nSrcOffset, nWidth*4, pDst + nDstOffset, nWidth*4, ResizedRoi);
					}
					else
						nDstOffset = nSrcOffset;
					
					nTileStep = ResizedRoi.height * nWidth*4;
					nSrcOffset = nDstOffset;
					ResizedRoi.width = nWidth;

					for (int i = 1; i < nFullTilesY; ++i)
					{
						nDstOffset += nTileStep;
						ippiCopy_8u_C4R(pDst + nSrcOffset, nWidth*4, pDst + nDstOffset, nWidth*4, ResizedRoi);
					}

					if (nYBottom > 0)
					{	
						nDstOffset += nTileStep;
						ResizedRoi.height = nYBottom;
						ippiCopy_8u_C4R(pDst + nSrcOffset, nWidth*4, pDst + nDstOffset, nWidth*4, ResizedRoi);
					}
					if (nYTop > 0)
					{
						nDstOffset = 0;
						ResizedRoi.height = nYTop;
						nSrcOffset = nSrcOffset + nTileStep - nWidth*4 * nYTop;
						ippiCopy_8u_C4R(pDst + nSrcOffset, nWidth*4, pDst + nDstOffset, nWidth*4, ResizedRoi);
					}

					delete [] pBackup;
					
					return c_nErrorNone;
				}
				int BGRA_TransformZoom( BYTE* pPixels, int nWidth, int nHeight, double dScaleX, double dScaleY, double dOffsetX, double dOffsetY, int nBackColor, int nBackAlpha )
				{
					if( dScaleX < 0.00001 || dScaleY < 0.00001 )
						return c_nErrorNone;

					CBuffer<Ipp8u>  oBuffer(nWidth * nHeight * 4);
					CBuffer<Ipp32f> oMapXY(nWidth * 2 + nHeight);
					
					if( oBuffer.IsEmpty() || oMapXY.IsEmpty() )
						return c_nErrorNoMemory;

					IppiRect SrcRect = {0, 0, nWidth, nHeight};
					IppiSize SrcSize = {nWidth, nHeight};
					IppiSize DstSize = {nWidth, 1};

					ClearImage( oBuffer.GetPtr(), nWidth, nHeight, nWidth*4, nBackColor, nBackAlpha );

					// расчёт координат по X
					double dInc = 1.0 / dScaleX;
					double dPos = (nWidth / 2.0 * (dScaleX - 1) - dOffsetX) * dInc;

					Ipp32f* pTable = oMapXY.GetPtr();
					for( int i = nWidth; i > 0; --i, ++pTable, dPos += dInc )
					{
						*pTable = float(dPos);
					}

					// расчёт координат по Y
					dInc = 1.0 / dScaleY;
					dPos = (nHeight / 2.0 * (dScaleY - 1) - dOffsetY) * dInc;

					pTable += nWidth;
					for( int i = nHeight; i > 0; --i, ++pTable, dPos += dInc )
					{
						*pTable = float(dPos);
					}

					// ресайз
					Ipp8u*  pDst  = oBuffer.GetPtr();
					Ipp32f* pMapX = oMapXY.GetPtr();
					Ipp32f* pMapY = oMapXY.GetPtr() + nWidth;
					Ipp32f* pBufY = oMapXY.GetPtr() + nWidth * 2;
					
					for( int nY = 0; nY < nHeight; ++nY, pDst += nWidth * 4 )
					{
						for( int nX = 0; nX < nWidth; ++nX )
						{
							pMapY[nX] = pBufY[nY];
						}

						ippiRemap_8u_C4R( pPixels, SrcSize, nWidth * 4, SrcRect, 
							pMapX, sizeof(Ipp32f)*nWidth, 
							pMapY, sizeof(Ipp32f)*nWidth, 
							pDst, nWidth * 4, DstSize, IPPI_INTER_LINEAR);
					}

					::memcpy( pPixels, oBuffer.GetPtr(), nWidth*nHeight*4 );
					
					return c_nErrorNone;
				}


				int BGRA_TransformMirror(BYTE* pBGRA, int nWidth, int nHeight, int nStride, int nType, double dSize)
				{
					/* @desc: 
					{
						отрисовывает в укзанной картинке (pBGRA, nWidth, nHeight) ее размытую копию 
						переменная nType указывает на то, с какой стороны будет произведена зеркальная отрисовка картинки
						переменная nSize указывает на размер отраженной картинки

						этот способ медленный, зато универсальный и качественный
						(ipp работает не стабильно - проверено неоднократно)
					}
					*/

					if (!pBGRA || nWidth < 1 || nHeight < 1)
						return c_nErrorInvalidSourceImage;

					if (nType == c_nMirrorHorizontalLeft || nType == c_nMirrorHorizontalRight)
					{
						int nMirrorLevel = (int)min(nWidth - 2, dSize*(nWidth - 1));

						if (nMirrorLevel <= 0)
							return c_nErrorNone;

						BYTE* pLine = new BYTE[4*nHeight];
						if (NULL == pLine)
							return c_nErrorNoMemory;

						int nSourceX, nSourceIndex;

						for (int nLevel = 0; nLevel < nMirrorLevel; ++nLevel)
						{
							// копируем строку для обработки
							for (int nY = 0; nY < nHeight; ++nY)
							{
								if (nType == c_nMirrorHorizontalLeft)
									nSourceX = nWidth - 1 - (nWidth - 1 - nMirrorLevel)*nLevel/nMirrorLevel;
								else
									nSourceX = (nWidth - 1 - nMirrorLevel)*nLevel/nMirrorLevel;

								nSourceIndex = 4*(nSourceX + nY*nWidth);

								memcpy(pLine + 4*nY, pBGRA + nSourceIndex, 4);
							}

							// фильтруем строку - размываем ее в зависимости от удаления от границы зеркального отражения
							BGRA_FilterAverage(pLine, nHeight, 1, 10*(nMirrorLevel + 1 - nLevel)/nMirrorLevel, 0);

							// выставляем цвета отфильтрованной линии в результирующую картинку
							if (nType == c_nMirrorHorizontalLeft)
							{
								for (int nY = 0; nY < nHeight; ++nY)
									memcpy(pBGRA + 4*(nLevel + nY*nWidth), pLine + 4*nY, 4);
							}
							else
							{
								for (int nY = 0; nY < nHeight; ++nY)
									memcpy(pBGRA + 4*(nWidth - 1 - nLevel + nY*nWidth), pLine + 4*nY, 4);
							}
						}

						delete[] pLine;
					}
					else if (nType == c_nMirrorVerticalTop || nType == c_nMirrorVerticalBottom)
					{
						if( nStride > 0 )
						{
							nType = (nType == c_nMirrorVerticalTop ? c_nMirrorVerticalBottom : c_nMirrorVerticalTop);
						}

						int nMirrorLevel = (int)min(nHeight - 2, dSize*(nHeight - 1));

						if (nMirrorLevel <= 0)
							return c_nErrorNone;

						BYTE* pLine = new BYTE[4*nWidth];
						if (NULL == pLine)
							return c_nErrorNoMemory;

						int nSourceY, nSourceIndex;

						for (int nLevel = 0; nLevel < nMirrorLevel; ++nLevel)
						{
							// копируем строку для обработки
							for (int nX = 0; nX < nWidth; ++nX)
							{
								if (nType == c_nMirrorVerticalTop)
									nSourceY = (nHeight - 1 - nMirrorLevel)*nLevel/nMirrorLevel;
								else
									nSourceY = nHeight - 1 - (nHeight - 1 - nMirrorLevel)*nLevel/nMirrorLevel;

								nSourceIndex = 4*(nX + nSourceY*nWidth);

								memcpy(pLine + 4*nX, pBGRA + nSourceIndex, 4);
							}

							// фильтруем строку - размываем ее в зависимости от удаления от границы зеркального отражения
							BGRA_FilterAverage(pLine, nWidth, 1, 10*(nMirrorLevel + 1 - nLevel)/nMirrorLevel, 0);

							// выставляем цвета отфильтрованной линии в результирующую картинку
							if (nType == c_nMirrorVerticalTop)
							{
								for (int nX = 0; nX < nWidth; ++nX)
									memcpy(pBGRA + 4*(nX + (nHeight - 1 - nLevel)*nWidth), pLine + 4*nX, 4);
							}
							else
							{
								for (int nX = 0; nX < nWidth; ++nX)
									memcpy(pBGRA + 4*(nX + nLevel*nWidth), pLine + 4*nX, 4);
							}
						}

						delete[] pLine;
					}

					return c_nErrorNone;
				}
#endif
				int BGRA_TransformMirrorIPP(BYTE* pPixels, int nWidth, int nHeight, double Frame, int Type, double Level)
				{
					// clamp values
					Frame = Clamp(Frame);
					Level = min(max(0, Level), 100);

					if (fabs(Frame) < 0.001 || fabs(Level) < 0.001 )
						return c_nErrorNone;
					
					// variables
					Ipp8u* pDst = pPixels;
					IppiRect srcRoi = {0, 0, nWidth, nHeight};
					IppiSize ImageSize = {nWidth, nHeight};
					IppiSize dstRoiSize = {nWidth, nHeight};
					int nMirror = 0;
					double dResize = 0.01*Level*Frame;
					
					if (Type == c_nMirrorVerticalTop)
					{
						nMirror = (int)(dResize*(nHeight - 2));
						srcRoi.height -= nMirror;
						dstRoiSize.height = nMirror;
						dResize = (double)dstRoiSize.height/srcRoi.height;

						ippiResize_8u_C4R(pDst, ImageSize, nWidth*4, srcRoi, pDst + srcRoi.height * nWidth*4, nWidth*4, dstRoiSize, 1, dResize, IPPI_INTER_LINEAR);

						pDst = pDst + srcRoi.height * nWidth*4;
						ippiMirror_8u_C4IR(pDst, nWidth*4, dstRoiSize, ippAxsHorizontal);	

						IppiSize MaskSize = {10, 3};
						IppiPoint anchor = {MaskSize.width/2, 1};
						dstRoiSize.width -= (MaskSize.width - 1);
						dstRoiSize.height -= (MaskSize.height - 1);
						ippiFilterBox_8u_C4IR(pDst + anchor.y*nWidth*4 + anchor.x*4 , nWidth*4, dstRoiSize, MaskSize, anchor);
					}
					else if (Type == c_nMirrorVerticalBottom)
					{
						nMirror = (int)(dResize*(nHeight - 2));
						srcRoi.height -= nMirror;
						srcRoi.y = nMirror;
						dstRoiSize.height = nMirror;
						dResize = (double)dstRoiSize.height/srcRoi.height;

						ippiResize_8u_C4R(pDst, ImageSize, nWidth*4, srcRoi, pDst, nWidth*4, dstRoiSize, 1, dResize, IPPI_INTER_LINEAR);

						ippiMirror_8u_C4IR( pDst, nWidth*4, dstRoiSize, ippAxsHorizontal );		
					
						IppiSize MaskSize = {10, 3};
						IppiPoint anchor = {MaskSize.width/2, 1};
						dstRoiSize.width -= (MaskSize.width - 1);
						dstRoiSize.height -= (MaskSize.height - 1);
						ippiFilterBox_8u_C4IR(pDst + anchor.y*nWidth*4 + anchor.x*4 , nWidth*4, dstRoiSize, MaskSize, anchor);
					}
					if (Type == c_nMirrorHorizontalRight)
					{
						nMirror = (int)(dResize*(nWidth - 2));
						srcRoi.width -= nMirror;
						dstRoiSize.width = nMirror;
						dResize = (double)dstRoiSize.width/srcRoi.width;

						ippiResize_8u_C4R(pDst, ImageSize, nWidth*4, srcRoi, pDst + srcRoi.width * 4, nWidth*4, dstRoiSize, dResize, 1, IPPI_INTER_LINEAR);
						
						pDst += srcRoi.width * 4;
						ippiMirror_8u_C4IR(pDst, nWidth*4, dstRoiSize, ippAxsVertical);

						IppiSize MaskSize = {3, 10};
						IppiPoint anchor= {1, MaskSize.height / 2};
						dstRoiSize.width -= (MaskSize.width - 1);
						dstRoiSize.height -= (MaskSize.height - 1);
						ippiFilterBox_8u_C4IR(pDst  + anchor.y*nWidth*4 + anchor.x*4, nWidth*4, dstRoiSize, MaskSize, anchor);
					}
					else if (Type == c_nMirrorHorizontalLeft)
					{
						nMirror = (int)(dResize*(nWidth - 2));
						srcRoi.width -= nMirror;
						srcRoi.x = nMirror;
						dstRoiSize.width = nMirror;
						dResize = (double)dstRoiSize.width/srcRoi.width;

						ippiResize_8u_C4R(pDst, ImageSize, nWidth*4 , srcRoi, pDst, nWidth*4, dstRoiSize, dResize, 1, IPPI_INTER_LINEAR);

						ippiMirror_8u_C4IR(pDst, nWidth*4, dstRoiSize, ippAxsVertical);

						IppiSize MaskSize = {3, 10};
						IppiPoint anchor= {1, MaskSize.height/2};
						dstRoiSize.width -= (MaskSize.width - 1);
						dstRoiSize.height -= (MaskSize.height - 1);
						ippiFilterBox_8u_C4IR(pDst + anchor.y*nWidth*4 + anchor.x*4, nWidth*4, dstRoiSize, MaskSize, anchor);
					}


					return c_nErrorNone;
				}	
#ifndef IMAGE_EDITOR
				int BGRA_TransformResizeMedia(BYTE* pPixels, int nWidth, int nHeight, int nStride, IUnknown** pResult, long nNewWidth, long nNewHeight, long Color, long Type, double dAspect, double dCropZoom, int alpha = 255, int nSrcX = 0, int nSrcY = 0, int nSrcWidth = 0, int nSrcHeight = 0 )
				{
					if( nSrcX < 0 ) nSrcX = 0;
					if( nSrcY < 0 ) nSrcY = 0;
					if( nSrcWidth  <= 0 ) nSrcWidth  = nWidth;
					if( nSrcHeight <= 0 ) nSrcHeight = nHeight;

					if( nSrcX >= nWidth || nSrcY >= nHeight || nNewWidth <= 0 || nNewHeight <= 0 )
					{
						ClearImage( pPixels, nWidth, nHeight, nWidth*4, Color, alpha );
						return c_nErrorNone;
					}

					if( nSrcX + nSrcWidth > nWidth )
						nSrcWidth = nWidth - nSrcX;

					if( nSrcY + nSrcHeight > nHeight )
						nSrcHeight = nHeight - nSrcY;

					if (!ImageStudioUtils::ByteArrayToMediaData(NULL, nNewWidth, nNewHeight, pResult, nStride < 0))
						return c_nErrorNoMemory;

					if( nStride > 0 )
						pPixels += (nSrcY * nWidth + nSrcX) * 4;
					else
						pPixels += ((nHeight - 1 - nSrcY ) * nWidth + nSrcX) * 4;

					nWidth  = nSrcWidth;
					nHeight = nSrcHeight;

					BOOL bFlipY = FALSE;
					if( nStride < 0 )
					{
						bFlipY = TRUE;
						pPixels += nStride * (nHeight - 1);
						nStride = -nStride;
					}

					IUnknown* pCreatedMediaData = *pResult;

					if(TRUE)
					{
						ImageStudio::Core::MediaData oMediaData;
					
						if (!oMediaData.Create(*pResult, FALSE) || !oMediaData.IsValidBGRA())
							return c_nErrorNoMemory;

						// добавляем одну ссылку с тем, чтобы объект реально не "убился", при выходе за текущие скобки ))
						*pResult = oMediaData.GetMediaData(TRUE);
					
						
						BYTE* pData = oMediaData.GetBuffer();
						int lWidth  = oMediaData.GetWidth();
						int lHeight = oMediaData.GetHeight();

						int NewHeight = lHeight;
						int NewWidth  = lWidth;

						if( NewHeight <= 0 || NewWidth <= 0 ) 
							return c_nErrorNoMemory;
						
						if (nWidth == NewWidth && nHeight == NewHeight)
						{
							int nRowLength = nWidth * 4;

							for( int y = 0; y < nHeight; ++y, pData += nRowLength, pPixels += nStride )
							{
								memcpy( pData, pPixels, nRowLength );
							}
						}
						else
						{
							// variables
							BYTE* pResized = pData;
							IppiRect SrcRect = {0, 0, nWidth, nHeight};
							IppiSize SrcSize = {nWidth, nHeight};
							IppiSize DstSize = {NewWidth, NewHeight};

							double dScaleX = NewWidth  /(double)nWidth;
							double dScaleY = NewHeight /(double)nHeight;

							if (Type == c_nResizeStretchNearest || Type == c_nResizeStretchBillinear || Type == c_nResizeStretchBicubic || Type == c_nResizeStretchSuper )
							{
								int inter = IPPI_INTER_NN;

								switch( Type )
								{
								case c_nResizeStretchBillinear: inter = IPPI_INTER_LINEAR; break;
								case c_nResizeStretchBicubic:   inter = IPPI_INTER_CUBIC; break;
								case c_nResizeStretchSuper:     inter = (dScaleX < 1 && dScaleY < 1) ? IPPI_INTER_SUPER : IPPI_INTER_CUBIC; break;
								}
									
								ippiResize_8u_C4R( pPixels, SrcSize, nStride, SrcRect, pResized, NewWidth*4, DstSize, dScaleX, dScaleY, inter );
							}
							else if (Type == c_nResizeShrinkNearest || Type == c_nResizeShrinkBillinear || Type == c_nResizeShrinkBicubic || Type == c_nResizeShrinkSuper )
							{
								// clear output image first
								ClearImage( pResized, NewWidth, NewHeight, NewWidth*4, Color, alpha );

								double dW = dAspect * nHeight;
								dScaleX *= (dW / nWidth);

								int nNewWidthAsp = int(dScaleX * nWidth);
								if (nNewWidthAsp > NewWidth)
								{
									double dCorrect = (double)NewWidth / nNewWidthAsp;
									dScaleX *= dCorrect;
									dScaleY *= dCorrect;
								}

								double dZoom = min(dScaleX, dScaleY);

								dScaleX = dZoom;
								dScaleY = dZoom;

								int nScaleWidth = (int)(nWidth * dScaleX);
								if( nScaleWidth <= 0 )
								{
									nScaleWidth = 1;
									dScaleX = 1.0 / nWidth;
								}

								int nScaleHeight = (int)(nHeight * dScaleY);
								if( nScaleHeight <= 0 )
								{
									nScaleHeight = 1;
									dScaleY = 1.0 / nHeight;
								}
								
								if( NewHeight > nScaleHeight )
								{
									pResized += 4 * NewWidth * ((NewHeight - nScaleHeight) / 2);
								}
								if( NewWidth > nScaleWidth )
								{
									pResized += 4 * ((NewWidth - nScaleWidth) / 2);
								}

								int inter = IPPI_INTER_NN;

								switch( Type )
								{
								case c_nResizeShrinkBillinear: inter = IPPI_INTER_LINEAR; break;
								case c_nResizeShrinkBicubic:   inter = IPPI_INTER_CUBIC; break;
								case c_nResizeShrinkSuper:     inter = (dScaleX < 1 && dScaleY < 1) ? IPPI_INTER_SUPER : IPPI_INTER_CUBIC; break;
								}

								ippiResize_8u_C4R( pPixels, SrcSize, nStride, SrcRect, pResized, NewWidth*4, DstSize, dScaleX, dScaleY, inter );
							}
							else if (Type == c_nResizeCropNearest || Type == c_nResizeCropBillinear || Type == c_nResizeCropBicubic || Type == c_nResizeCropSuper )
							{
								ClearImage( pResized, NewWidth, NewHeight, NewWidth * 4, Color, alpha );

								// variables
								double dMinScale = min(dScaleX, dScaleY);
								double dMaxScale = max(dScaleX, dScaleY);
								double dFactor = (dMinScale + (dMaxScale - dMinScale) * dCropZoom );

								double dW = nWidth  * dFactor;
								double dH = nHeight * dFactor;

								double dRight  = min(NewWidth,  dW) / 2;
								double dBottom = min(NewHeight, dH) / 2;
								double dLeft   = -dRight;
								double dTop    = -dBottom;

								int nX1 = Round(dLeft   / dFactor + nWidth  / 2.0);
								int nY1 = Round(dTop    / dFactor + nHeight / 2.0);
								int nX2 = Round(dRight  / dFactor + nWidth  / 2.0);
								int nY2 = Round(dBottom / dFactor + nHeight / 2.0);

								if( nX1 < 0 ) nX1 = 0;
								if( nY1 < 0 ) nY1 = 0;
								if( nX2 > nWidth  ) nX2 = nWidth;
								if( nY2 > nHeight ) nY2 = nHeight;

								SrcRect.x = nX1;
								SrcRect.y = nY1;
								SrcRect.width  = nX2 - nX1;
								SrcRect.height = nY2 - nY1;

								if( SrcRect.width > 0 && SrcRect.height > 0 )
								{
									if( dCropZoom < 1 )
									{
										int nX1 = Round(dLeft   + NewWidth  / 2.0);
										int nY1 = Round(dTop    + NewHeight / 2.0);
										int nX2 = Round(dRight  + NewWidth  / 2.0);
										int nY2 = Round(dBottom + NewHeight / 2.0);

										if( nX1 < 0) nX1 = 0;
										if( nY1 < 0) nY1 = 0;
										if( nX2 > NewWidth  ) nX2 = NewWidth;
										if( nY2 > NewHeight ) nY2 = NewHeight;

										pResized += (nY1 * NewWidth + nX1) * 4;
										DstSize.width  = nX2 - nX1;
										DstSize.height = nY2 - nY1;
									}
									else
									{
										DstSize.width  = NewWidth;
										DstSize.height = NewHeight;
									}

									if( DstSize.width > 0 && DstSize.height > 0 )
									{
										double dFactorX = double(DstSize.width)  / SrcRect.width;
										double dFactorY = double(DstSize.height) / SrcRect.height;

										int inter = IPPI_INTER_NN;

										switch( Type )
										{
										case c_nResizeCropBillinear: inter = IPPI_INTER_LINEAR; break;
										case c_nResizeCropBicubic:   inter = IPPI_INTER_CUBIC; break;
										case c_nResizeCropSuper:     inter = (dFactorX < 1 && dFactorY < 1) ? IPPI_INTER_SUPER : IPPI_INTER_CUBIC; break;
										}

										ippiResize_8u_C4R( pPixels, SrcSize, nStride, SrcRect, pResized, NewWidth*4, DstSize, dFactorX, dFactorY, inter );
									}
								}
							}
						}
					}

					if (*pResult != pCreatedMediaData)
						pCreatedMediaData->Release();

					if( *pResult )
					{
						MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
						(*pResult)->QueryInterface( MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame );
						if( pFrame )
						{
							pFrame->put_ColorSpace( bFlipY ? (CSP_BGRA | CSP_VFLIP) : CSP_BGRA );
							pFrame->Release();
						}
					}
					
					return c_nErrorNone;
				}


				int BGRA_TransformTwirl(BYTE* pPixels, int nWidth, int nHeight, double dFrame, double dAngle, double dDegree, double dZoom, int nBackColor, int nBackAlpha )
				{
					// clamp values
					dFrame = Clamp(dFrame);

					dZoom = dFrame * (dZoom - 1) + 1;
					if( dZoom < 0.0001 )
					{
						ClearImage( pPixels, nWidth, nHeight, nWidth * 4, nBackColor, nBackAlpha );
						return c_nErrorNone;
					}

					CBuffer<Ipp8u>  oBuffer( nWidth * nHeight * 4 );
					CBuffer<Ipp32f> oMapXY( nWidth * 2 );

					int nSizeHelp = max(nWidth, nHeight);
					CBuffer<double> oHelp( nSizeHelp * 2 );

					int nSizeTable = nWidth + nHeight;
					CBuffer<double> oTable( (nSizeTable + 1) * 2 );

					if( oMapXY.IsEmpty() || oBuffer.IsEmpty() || oTable.IsEmpty() || oHelp.IsEmpty() )
					{
						return c_nErrorNoMemory;
					}

					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};
					IppiSize DstSize = {nWidth, 1};

					ClearImage( oBuffer.GetPtr(), nWidth, nHeight, nWidth * 4, nBackColor, nBackAlpha );

					dAngle *= dFrame * IPP_PI180;
					dZoom = 1 / dZoom;
					
					// расчитываем таблицу для ускорения расчётов
					double* pTable = oTable.GetPtr();
					double dFactor = 1.0 / nSizeTable;
					
					for( int i = 0; i <= nSizeTable; ++i, pTable += 2 )
					{
						double ang = dAngle * pow( 1 - sqrt(i * dFactor), dDegree );
						
						pTable[0] = sin( ang ) * dZoom;
						pTable[1] = cos( ang ) * dZoom;
					}

					// расчитываем вспомогательную таблицу
					int nMinSize = nSizeHelp / 2;
					int nMaxSize = nSizeHelp - nMinSize;

					dFactor = nSizeTable * 4.0 / (nWidth * nWidth + nHeight * nHeight);

					double* pHelp = oHelp.GetPtr();
					for( int i = -nMinSize; i < nMaxSize; ++i, pHelp += 2 )
					{
						pHelp[1] = i;
						pHelp[0] = i * i * dFactor;
					}

					// главный расчёт
					Ipp32f* pxMapCur = oMapXY.GetPtr();
					Ipp32f* pyMapCur = oMapXY.GetPtr() + nWidth;

					int nHalfWidth  = nWidth  / 2;
					int nHalfHeight = nHeight / 2;

					pTable = oTable.GetPtr();
					pHelp  = oHelp.GetPtr();

					double* pHelpY = pHelp + (nMinSize - nHalfHeight) * 2;
					double* pHelpX = pHelp + (nMinSize - nHalfWidth) * 2;

					BYTE* pDst = oBuffer.GetPtr();

					for( int nY = 0; nY < nHeight; ++nY, pHelpY += 2, pHelpX -= nWidth * 2, pDst += nWidth * 4 )
					{
						for( int nX = 0; nX < nWidth; ++nX, pHelpX += 2, ++pxMapCur, ++pyMapCur )
						{
							double dR = pHelpX[0] + pHelpY[0];
							int nIndex = int(dR);
							double* pItem = pTable + nIndex * 2;
							
							double dSin = pItem[0];
							double dCos = pItem[1];
							
							// интерполяция
							if( nIndex < nSizeTable )
							{
								dR -= nIndex;
								dSin += (pItem[2] - dSin) * dR;
								dCos += (pItem[3] - dCos) * dR;
							}
							
							*pxMapCur = float((pHelpX[1] * dCos - pHelpY[1] * dSin) + nHalfWidth);
							*pyMapCur = float((pHelpX[1] * dSin + pHelpY[1] * dCos) + nHalfHeight);
						}

						pxMapCur -= nWidth;
						pyMapCur -= nWidth;

						ippiRemap_8u_C4R( pPixels, SrcSize, nWidth*4, SrcRect, 
							pxMapCur, sizeof(Ipp32f)*nWidth, 
							pyMapCur, sizeof(Ipp32f)*nWidth, 
							pDst, nWidth*4, DstSize, IPPI_INTER_LINEAR);
					}

					::memcpy( pPixels, oBuffer.GetPtr(), nWidth * nHeight * 4 );

					return c_nErrorNone;
				}

				int BGRA_TransformSphere(BYTE* pPixels, int nWidth, int nHeight, int nStride, double Frame, long CenterX, long CenterY,  long RadiusX, long RadiusY, double Degree, int nBackColor, int nBackAlpha )
				{
					// clamp values
					Frame = Clamp(Frame);
					if (Degree < -1.0)
						Degree = -1.0;
					else if (Degree > 10.0)
						Degree = 10.0;
					
					//Degree *= Frame;

					double dRadiusX = abs(RadiusX * Frame);
					double dRadiusY = abs(RadiusY * Frame);

					IppiPoint nRadius = {
						int(dRadiusX) + 1,
						int(dRadiusY) + 1
					};

					// сферу без радиуса бестолку отрисовывать!
					if( nRadius.x <= 1 || nRadius.y <= 1 )
						return c_nErrorNone;

					// переворачиваем координаты по Y
					if( nStride < 0 )
						CenterY = nHeight - CenterY;

					// расчитываем прямоугольник, описанный возле сферы
					IppiRect BoundRect = {
						CenterX - nRadius.x,
						CenterY - nRadius.y,
						nRadius.x * 2,
						nRadius.y * 2
					};

					if( BoundRect.x >= nWidth || BoundRect.y >= nHeight )
						return c_nErrorNone;

					if( BoundRect.x < 0 )
					{
						if( BoundRect.x + BoundRect.width <= 0 )
							return c_nErrorNone;

						BoundRect.width += BoundRect.x;
						BoundRect.x = 0;
					}

					if( BoundRect.y < 0 )
					{
						if( BoundRect.y + BoundRect.height <= 0 )
							return c_nErrorNone;

						BoundRect.height += BoundRect.y;
						BoundRect.y = 0;
					}

					if( BoundRect.x + BoundRect.width > nWidth )
						BoundRect.width = nWidth - BoundRect.x;

					if( BoundRect.y + BoundRect.height > nHeight )
						BoundRect.height = nHeight - BoundRect.y;

					int nMaxRadius = max( nRadius.x, nRadius.y );
					int nHelpSize  = BoundRect.width + BoundRect.height;

					// создаём необходимые буфера
					CBuffer<Ipp8u>  oBuffer( BoundRect.width * BoundRect.height * 4 );
					CBuffer<Ipp32f> oMapXY( BoundRect.width * 2 );
					CBuffer<Ipp64f> oTable( nHelpSize * 2 + nMaxRadius + 1 );

					if( oBuffer.IsEmpty() || oMapXY.IsEmpty() || oTable.IsEmpty() )
					{
						return c_nErrorNoMemory;
					}

					// закрашиваем фоновым цветом
					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};
					IppiSize DstSize = {BoundRect.width, BoundRect.height};

					ClearImage( oBuffer.GetPtr(), DstSize.width, DstSize.height, DstSize.width * 4, nBackColor, nBackAlpha );

					DstSize.height = 1;

					dRadiusX = nMaxRadius / (dRadiusX * dRadiusX);
					dRadiusY = nMaxRadius / (dRadiusY * dRadiusY);

					Degree /= 10;
					double dFactor = 1.0 / (nMaxRadius + 1);
					//dFactor *= IPP_PI2;
					
					double* pOffset = oTable.GetPtr() + nHelpSize * 2;
					for( int i = 0; i <= nMaxRadius; i++ )
					{
						pOffset[i] = pow( (i + 1) * dFactor, Degree ); // чёткий контур
						//pOffset[i] = pow( sin((i + 1) * dFactor), Degree ); // сглаженный контур
					}

					double* pHelpItem = oTable.GetPtr();
					for( int i = 0; i < BoundRect.width; ++i, pHelpItem += 2 )
					{
						int nX = i + BoundRect.x - CenterX;
						pHelpItem[1] = nX;
						pHelpItem[0] = nX * nX * dRadiusX;
					}
					for( int i = 0; i < BoundRect.height; ++i, pHelpItem += 2 )
					{
						int nY = i + BoundRect.y - CenterY;
						pHelpItem[1] = nY;
						pHelpItem[0] = nY * nY * dRadiusY;
					}

					Ipp32f* pMapX = oMapXY.GetPtr();
					Ipp32f* pMapY = oMapXY.GetPtr() + BoundRect.width;

					double* pHelpY = oTable.GetPtr() + BoundRect.width * 2;
					BYTE* pDst = oBuffer.GetPtr();
					
					for( int nRow = BoundRect.height; nRow > 0; --nRow, pHelpY += 2, pDst += BoundRect.width * 4 )
					{
						double* pHelpX = oTable.GetPtr();
						for( int nPos = BoundRect.width; nPos > 0; --nPos, ++pMapX, ++pMapY, pHelpX += 2 )
						{
							double dR = pHelpX[0] + pHelpY[0];

							if( dR < nMaxRadius )
							{
								// интерполяция
								int index = int(dR);
								double* pItem = pOffset + index;
								dR = (dR - index) * (pItem[1] - pItem[0]) + pItem[0];
								
								*pMapX = float(pHelpX[1] * dR + CenterX);
								*pMapY = float(pHelpY[1] * dR + CenterY);
							}
							else
							{
								*pMapX = float(pHelpX[1] + CenterX);
								*pMapY = float(pHelpY[1] + CenterY);
							}
						}

						pMapX -= BoundRect.width;
						pMapY -= BoundRect.width;

						// ремэпим
						ippiRemap_8u_C4R( pPixels, SrcSize, nWidth*4, SrcRect, 
							pMapX, sizeof(Ipp32f)*BoundRect.width, 
							pMapY, sizeof(Ipp32f)*BoundRect.width, 
							pDst, BoundRect.width*4, DstSize, IPPI_INTER_LINEAR);
					}
					

					// копируем полученную картинку обратно
					Ipp8u* pDstPixels = pPixels + (BoundRect.y * nWidth + BoundRect.x) * 4;
					
					DstSize.height = BoundRect.height;

					ippiCopy_8u_C4R( oBuffer.GetPtr(), BoundRect.width*4, pDstPixels, nWidth*4, DstSize );
					
					return c_nErrorNone;
				}

				int BGRA_TransformCylinder(BYTE* pPixels, int nWidth, int nHeight, int nStride, double Frame, long Center, long Radius, double Degree, long nType, int nBackColor, int nBackAlpha )
				{
					// clamp values
					Frame = Clamp(Frame);
					Degree = max(-1, min(10, Degree));
					Degree *= Frame;
					
					double dRadius = Radius * Frame;
					int nMaxRadius = int(abs(dRadius)) + 1;
					if( nMaxRadius <= 1 )
						return c_nErrorNone;

					IppiRect BoundRect = {0, 0, nWidth, nHeight};

					BOOL bHor = (c_nTransformCylinderHoriz == nType) ? TRUE : FALSE;
					if( bHor )
					{
						if( nStride < 0 )
							Center = nHeight - Center;

						BoundRect.y = Center - nMaxRadius;
						BoundRect.height = nMaxRadius * 2;

						if( BoundRect.y >= nHeight )
							return c_nErrorNone;

						if( BoundRect.y < 0 )
						{
							if( BoundRect.y + BoundRect.height <= 0 )
								return c_nErrorNone;

							BoundRect.height += BoundRect.y;
							BoundRect.y = 0;
						}

						if( BoundRect.y + BoundRect.height > nHeight )
						{
							BoundRect.height = nHeight - BoundRect.y;
						}
					}
					else
					{
						BoundRect.x = Center - nMaxRadius;
						BoundRect.width = nMaxRadius * 2;

						if( BoundRect.x >= nWidth )
							return c_nErrorNone;

						if( BoundRect.x < 0 )
						{
							if( BoundRect.x + BoundRect.width <= 0 )
								return c_nErrorNone;

							BoundRect.width += BoundRect.x;
							BoundRect.x = 0;
						}

						if( BoundRect.x + BoundRect.width > nWidth )
						{
							BoundRect.width = nWidth - BoundRect.x;
						}
					}

					// создаём дополнительные буфера
					CBuffer<Ipp8u>  oBuffer( BoundRect.width * BoundRect.height * 4 );
					CBuffer<Ipp32f> oMapXY( BoundRect.width * 2 + BoundRect.height );

					if( oBuffer.IsEmpty() || oMapXY.IsEmpty() )
						return c_nErrorNoMemory;
					
					// закрашиваем фоновым цветом
					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};
					IppiSize DstSize = {BoundRect.width, BoundRect.height};

					ClearImage( oBuffer.GetPtr(), DstSize.width, DstSize.height, DstSize.width * 4, nBackColor, nBackAlpha );

					DstSize.height = 1;

					dRadius = 1.0/(nMaxRadius * nMaxRadius);
					Degree = Degree / 10;

					// вычисляем таблицы
					float* pTable = oMapXY.GetPtr() + (bHor ? BoundRect.width * 2 : 0);
					int    nMax   = bHor ? BoundRect.height : BoundRect.width;
					double dValue = (bHor ? BoundRect.y : BoundRect.x) - Center;
					
					for( int i = 0; i < nMax; ++i, ++dValue )
					{
						pTable[i] = float(dValue * pow( dValue * dValue * dRadius + 0.0001, Degree ) + Center);
					}

					pTable = oMapXY.GetPtr() + (bHor ? 0 : BoundRect.width * 2);
					nMax   = bHor ? BoundRect.width : BoundRect.height;
					dValue = 0;
					
					for( int i = 0; i < nMax; ++i, ++dValue )
					{
						pTable[i] = float(dValue);
					}

					// основные преобразования картинки
					Ipp32f* pMapX = oMapXY.GetPtr();
					Ipp32f* pMapY = oMapXY.GetPtr() + BoundRect.width;
					Ipp32f* pBufY = oMapXY.GetPtr() + BoundRect.width * 2;
					BYTE*   pDst  = oBuffer.GetPtr();

					for( int nY = BoundRect.height; nY > 0 ; --nY, ++pBufY, pDst += BoundRect.width*4 )
					{
						for( int nX = 0; nX < BoundRect.width; nX++ )
						{
							pMapY[nX] = pBufY[0];
						}
						
						// ремэпим
						ippiRemap_8u_C4R( pPixels, SrcSize, nWidth * 4, SrcRect, 
							pMapX, sizeof(Ipp32f)*BoundRect.width, 
							pMapY, sizeof(Ipp32f)*BoundRect.width, 
							pDst, BoundRect.width * 4, DstSize, IPPI_INTER_LINEAR);
					}

					Ipp8u* pDstPixels = pPixels + (BoundRect.y * nWidth + BoundRect.x) * 4;
					
					DstSize.height = BoundRect.height;
					
					ippiCopy_8u_C4R( oBuffer.GetPtr(), BoundRect.width*4, pDstPixels, nWidth*4, DstSize );
						
					return c_nErrorNone;
				}
				int BGRA_TransformWave( BYTE* pPixels, int nWidth, int nHeight, double dPhase, double dPhaseOffset, double dAmplitude, BOOL bVertical )
				{
					// создаём временный буфер
					BYTE* pBuffer = new BYTE[nWidth*nHeight*4];
					if( !pBuffer )
						return c_nErrorNoMemory;

					// обрезаем амплитуду, если она не вписывается в пределы
					if( bVertical )
					{
						double w = (nWidth - 1) / 2;
						if( dAmplitude > w )
							dAmplitude = w;
						if( dAmplitude < -w )
							dAmplitude = -w;
					}
					else
					{
						double h = (nHeight - 1) / 2;
						if( dAmplitude > h )
							dAmplitude = h;
						if( dAmplitude < -h )
							dAmplitude = -h;
					}

					int nSize = nWidth * nHeight;

					float* pfMapX = new float[nSize];
					if( !pfMapX )
						return c_nErrorNoMemory;

					float* pfMapY = new float[nSize];
					if( !pfMapY )
					{
						delete [] pfMapX;
						return c_nErrorNoMemory;
					}

					float fAbsAmpl = float(abs(dAmplitude));

					if( !bVertical )
					{
						dPhase /= nWidth;

						float fMaxOffset = float(nHeight - 1) - fAbsAmpl;
						
						float* pX = pfMapX;
						float* pY = pfMapY;
						
						for( int nPos = 0; nPos < nWidth; ++nPos )
						{
							*pX++ = float(nPos);
							*pY++ = float(dAmplitude * sin( nPos * dPhase - dPhaseOffset ));
						}

						for( int nCountRows = nHeight - 1; nCountRows > 0; --nCountRows )
						{
							for( int nRowLength = nWidth; nRowLength > 0; --nRowLength, ++pY, ++pX )
							{
								*pX = pX[-nWidth];
								*pY = pY[-nWidth] + 1;

								if( pY[-nWidth] < fAbsAmpl )
								{
									pY[-nWidth] += fAbsAmpl;
									pY[-nWidth] *= 0.5f;
								}
								
								if( pY[0] > fMaxOffset )
								{
									pY[0] -= 0.5f;
								}
							}
						}
					}
					else
					{
						dPhase /= nHeight;

						float fMaxOffset = float(nWidth - 1) - fAbsAmpl;

						float* pX = pfMapX;
						float* pY = pfMapY;
						
						IppiSize RowMapSize = {nWidth, 1};
						for( int nRow = 0; nRow < nHeight; ++nRow )
						{
							*pY++ = float(nRow);
							*pX++ = float(dAmplitude * sin( nRow * dPhase - dPhaseOffset ));
							
							for( int nPos = 1; nPos < nWidth; ++nPos, ++pX, ++pY )
							{
								*pY = float(nRow);
								*pX = pX[-1] + 1;
								
								if( pX[-1] < fAbsAmpl )
								{
									pX[-1] += fAbsAmpl;
									pX[-1] *= 0.5f;
								}
								
								if( pX[0] > fMaxOffset )
								{
									pX[0] -= 0.5f;
								}
							}
						}
					}

					IppiSize ImageSize = { nWidth, nHeight };
					IppiRect ImageRect = { 0, 0, nWidth, nHeight };
					
					ippiRemap_8u_C4R( pPixels, ImageSize, nWidth*4, ImageRect, pfMapX, nWidth*sizeof(float), pfMapY, nWidth*sizeof(float), pBuffer, nWidth*4, ImageSize, IPPI_INTER_LINEAR );

					memcpy( pPixels, pBuffer, nWidth*nHeight*4 );

					delete [] pBuffer;
					
					delete [] pfMapX;
					delete [] pfMapY;

					return c_nErrorNone;
				}
				int BGRA_TransformRipple( BYTE* pPixels, int nWidth, int nHeight, double dOmega, double dOffset, double dFrame )
				{
					Ipp32f* pxMap = new Ipp32f[nWidth*nHeight];
					Ipp32f* pyMap = new Ipp32f[nWidth*nHeight];
					Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];

					//int nSizeHelp = max(nWidth, nHeight);
					//double* pHelp = new double[nSizeHelp * 2];

					int nHalfWidth  = nWidth  / 2 + 2;
					int nHalfHeight = nHeight / 2 + 2;

					CBuffer<float> table( nHalfWidth * nHalfHeight );
					if( table.IsEmpty() )
						return c_nErrorNoMemory;

					CBuffer<int> help( nWidth );
					if( help.IsEmpty() )
						return c_nErrorNoMemory;

					double dHypot = nWidth * nWidth + nHeight * nHeight;
					double dAmpl  = dFrame * sqrt( dHypot );
					dOmega  *= IPP_2PI;
					dOffset *= IPP_2PI;

					dOmega /= sqrt(dHypot);

					for( int i = 0; i < nHalfWidth; i++ )
					{
						help[i] = i * i;
					}
					
					float* pTable = table.GetPtr();
					
					for( int nY = 0; nY < nHalfHeight; ++nY )
					{
						double y = nY * nY;

						for( int nX = 0; nX < nHalfWidth; ++nX, ++pTable )
						{
							pTable[0] = float(sin( dOmega * sqrt( y + help[nX] ) - dOffset));
						}
					}

					for( int i = 0; i < nWidth; i++ )
					{
						help[i] = i - (nHalfWidth - 2);
					}

					if (pxMap && pyMap && pBuffer)
					{
						::memcpy( pBuffer, pPixels, nWidth * nHeight * 4 );

						//double* pItem = pHelp;
						//for( int i = 0; i < nSizeHelp; ++i, pItem += 2 )
						//{
						//	int temp = i - nSizeHelp / 2;
						//	pItem[0] = (temp * temp);
						//	pItem[1] = (temp - 1) * (temp - 1);
						//}

						int right  = nWidth  - 1;
						int bottom = nHeight - 1;
						
						//double* pI = pHelp + (nSizeHelp / 2 - nWidth  / 2) * 2;
						//double* pJ = pHelp + (nSizeHelp / 2 - nHeight / 2) * 2;
						
						for( int j = 0; j < nHeight; j += 1/*, pJ += 2, pI -= nWidth * 2*/ )
						{
							int nY = (j - (nHalfHeight - 2)) * nHalfWidth;

							for( int i = 0; i < nWidth; i += 1/*, pI += 2*/ )
							{
								//double dTemp = sin( dOmega * sqrt(pI[0] + pJ[0]) - dOffset);
								//
								//double dx = abs( i + dAmpl * (dTemp - sin( dOmega * sqrt(pI[1] + pJ[0]) - dOffset)));
								//double dy = abs( j + dAmpl * (dTemp - sin( dOmega * sqrt(pI[0] + pJ[1]) - dOffset)));

								double dTemp = table[abs(nY) + abs(help[i])];
								
								double dx = abs( i + dAmpl * (dTemp - table[abs(nY) + abs(help[i] - 1)]));
								double dy = abs( j + dAmpl * (dTemp - table[abs(nY - nHalfWidth) + abs(help[i])]));

								if( dx > right )
									dx = right * 2 - dx;

								if( dy > bottom )
									dy = bottom * 2 - dy;

								*pxMap++ = float(dx);
								*pyMap++ = float(dy);
							}
						}

						pxMap -= nWidth * nHeight;
						pyMap -= nWidth * nHeight;

						IppiSize SrcSize = {nWidth, nHeight};
						IppiRect SrcRect = {0, 0, nWidth, nHeight};

						// все пикселы должны быть валидны
						//Ipp8u backColor[4] = {0, 0, 0, 255};
						//ippiSet_8u_C4R( backColor, pPixels, 4*nWidth, SrcSize );
						
						ippiRemap_8u_C4R(pBuffer, SrcSize, 4*nWidth, SrcRect, 
							pxMap, sizeof(Ipp32f) * nWidth, 
							pyMap, sizeof(Ipp32f) * nWidth, 
							pPixels, 4*nWidth, SrcSize, IPPI_INTER_LINEAR );
					}

					//if (pHelp)
					//	delete [] pHelp;
					if (pyMap)
						delete []pyMap;
					if (pxMap)
						delete []pxMap;
					if (pBuffer)
						delete []pBuffer;

					return c_nErrorNone;
				}
				int BGRA_TransformLinear( BYTE* pSrcPixels, int nSrcStride, BYTE* pDstPixels, int nDstStride, int nWidth, int nHeight, double dFrame, double dAngle, double dScaleX, double dScaleY, double dShiftX, double dShiftY, int nBackColor, int nBackAlpha )
				{
					dFrame = Clamp( dFrame );

					dScaleX = (dScaleX - 1) * dFrame + 1;
					dScaleY = (dScaleY - 1) * dFrame + 1;

					if( abs(dScaleX) < IPP_EPS_32F || abs(dScaleY) < IPP_EPS_32F )
					{
						ClearImage( pDstPixels, nWidth, nHeight, nDstStride, nBackColor, nBackAlpha );
						return c_nErrorNone;
					}

					

					dShiftX *= dFrame;
					dShiftY *= dFrame;
					dAngle  *= dFrame * IPP_PI180;

					double dSin = sin( dAngle );
					double dCos = cos( dAngle );
					double dCenterX = (nWidth  - 1) * 0.5;
					double dCenterY = (nHeight - 1) * 0.5;

					dShiftX += dCenterX;
					dShiftY += dCenterY;

					double m[3][2] = {1, 0, 0, 1, 0, 0
						// dCos    / dScaleX,
						//-dSin    / dScaleY,
						// dSin    / dScaleX,
						// dCos    / dScaleY,
						// (dCos * dCenterX + dSin * dCenterY - dShiftX) / dScaleX,
						// dShiftY / dScaleY,
					};

					BYTE vColor[4] = {
						GetBValue(nBackColor),
						GetGValue(nBackColor),
						GetRValue(nBackColor),
						IntToByte(nBackAlpha),
					};

					nDstStride -= nWidth * 4;

					double dDstY = 0;
					for( int nRow = 0; nRow < nHeight; ++nRow, ++dDstY, pDstPixels += nDstStride )
					{
						double dXY = m[1][0] * dDstY + m[2][0];
						double dYY = m[1][1] * dDstY + m[2][1];
						
						double dDstX = 0;
						for( int nPos = 0; nPos < nWidth; ++nPos, ++dDstX, pDstPixels += 4 )
						{
							double dX = dDstX * m[0][0] + dXY;
							double dY = dDstX * m[0][1] + dYY;

							int nCornersX, nCornersY;

							if( dX <= -1 )
								nCornersX = 0;
							else if( dX < 0 )
								nCornersX = 6;
							else if( dX < nWidth - 1 )
								nCornersX = 15;
							else if( dX < nWidth )
								nCornersX = 9;
							else
								nCornersX = 0;

							if( dY <= -1 )
								nCornersY = 0;
							else if( dY < 0 )
								nCornersY = 12;
							else if( dY < nHeight - 1 )
								nCornersY = 15;
							else if( dY < nHeight )
								nCornersY = 3;
							else
								nCornersY = 0;

							int nCorners = nCornersX & nCornersY;
							if( !nCorners )
							{
								*((DWORD*)pDstPixels) = *((DWORD*)vColor);
								continue;
							}

							int nX = Floor( dX );
							int nY = Floor( dY );

							double dX2 = dX - nX;
							double dY2 = dY - nY;

							BYTE* pBase = pSrcPixels + nY * nSrcStride + nX * 4;

							BYTE* p00 = (nCorners & 0x01) ? pBase : vColor;
							BYTE* p01 = (nCorners & 0x02) ? pBase + 4 : vColor;
							BYTE* p10 = (nCorners & 0x04) ? pBase + 4 + nSrcStride : vColor;
							BYTE* p11 = (nCorners & 0x08) ? pBase + nSrcStride : vColor;

							double dRX = 1 - dX2;
							double dRY = 1 - dY2;

							for( int i = 0; i < 4; i++ )
							{
								pDstPixels[i] = BYTE((p00[i] * dX2 + p01[i] * dRX) * dY2 + (p10[i] * dX2 + p11[i] * dRX) * dRY);
							}
						}
					}

					return c_nErrorNone;
				}
				
				int BGRA_TransformGlassTile( BYTE* pPixels, int nWidth, int nHeight, int nSize, double dFrame )
				{
					dFrame = Clamp( dFrame );

					int dCellSize = int(nSize * dFrame + 0.5);
					if( dCellSize <= 0 )
						return c_nErrorNone;

					CBuffer<float> oMapX( nWidth );
					CBuffer<float> oMapY( nWidth );
					CBuffer<BYTE> oBuffer(nWidth * nHeight * 4 );

					if( oMapX.IsEmpty() || oMapY.IsEmpty() || oBuffer.IsEmpty() )
						return c_nErrorNoMemory;

					memset( oBuffer.GetPtr(), 0, nWidth * nHeight * 4 );

					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};
					IppiSize DstSize = {nWidth, 1};
				
					
					double dCenterX = (nWidth  - 1) * 0.5;
					double dCenterY = (nHeight - 1) * 0.5;
					double dCellFactor = 1.0 / dCellSize;

					BYTE* pBuffer = oBuffer.GetPtr();
					
					for( int nRow = 0; nRow < nHeight; ++nRow, pBuffer += nWidth * 4 )
					{
						float* pMapX = oMapX.GetPtr();
						float* pMapY = oMapY.GetPtr();

						for( int nPos = 0; nPos < nWidth; ++nPos )
						{
							double x0 = nPos - dCenterX;
							double y0 = nRow - dCenterY;

							double x = x0;
							double y = y0;

							x *= dCellFactor;
							y *= dCellFactor;

							x = floor( x );
							y = floor( y );

							x += 0.5;
							y += 0.5;

							x *= dCellSize;
							y *= dCellSize;

							x0 = x;
							y0 = y;

							x0 += dCenterX;
							y0 += dCenterY;

							x = nPos - x0;
							y = nRow - y0;

							x *= 2.0;
							y *= 2.0;

							x += x0;
							y += y0;

							if( x < 0 ) x = abs(x); else
							if( x > nWidth - 1 ) x = (nWidth - 1) * 2 - x;

							if( y < 0 ) y = abs(y); else
							if( y > nHeight - 1 ) y = (nHeight - 1) * 2 - y;

							*pMapX++ = float(x);
							*pMapY++ = float(y);
						}

						ippiRemap_8u_C4R(pPixels, SrcSize, 4*nWidth, SrcRect, 
							oMapX.GetPtr(), sizeof(Ipp32f) * nWidth, 
							oMapY.GetPtr(), sizeof(Ipp32f) * nWidth, 
							pBuffer, 4*nWidth, DstSize, IPPI_INTER_LINEAR );

					}

					memcpy( pPixels, oBuffer.GetPtr(), nWidth * nHeight * 4 );

					return c_nErrorNone;
				}
				
				int BGRA_TransformBrokenGlass( BYTE* pPixels, int nWidth, int nHeight, int nDebris, double dFrame )
				{
					dFrame = Clamp( dFrame );

					if( nWidth <= 1 || nHeight <= 1 )
						return c_nErrorNone;

					double dCenterX = (nWidth  - 1) * 0.5;
					double dCenterY = (nHeight - 1) * 0.5;

					int nCountLines = max( 3, min(nDebris, 30));

					srand( 1000 );
					
					CBuffer<int> nRndNumbers(nCountLines);
					if( nRndNumbers.IsEmpty() )
						return c_nErrorNoMemory;


					for( int i = 0; i < nCountLines; i++ )
					{
						nRndNumbers[i] = rand();
					}

					QuickSort( nRndNumbers.GetPtr(), nCountLines );
				
					struct TLocal
					{
						double angle;
						double factor;
					};
					
					CBuffer<TLocal> aLines(nCountLines);
					if( aLines.IsEmpty() )
						return c_nErrorNoMemory;

					for( int i = 0; i < nCountLines; i++ )
					{
						aLines[i].angle = nRndNumbers[i] * (IPP_2PI / RAND_MAX);
						aLines[i].factor = 1.0 - (rand() / double(RAND_MAX) * 0.3) * dFrame;
					}

					CBuffer<float> oMapX( nWidth * nHeight );
					CBuffer<float> oMapY( nWidth * nHeight );
					CBuffer<BYTE> oBuffer(nWidth * nHeight * 4 );

					if( oMapX.IsEmpty() || oMapY.IsEmpty() || oBuffer.IsEmpty() )
						return c_nErrorNoMemory;

					memset( oBuffer.GetPtr(), 0, nWidth * nHeight * 4 );

					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};
					IppiSize DstSize = {nWidth, nHeight};
				
					BYTE* pBuffer = oBuffer.GetPtr();

					float* pMapX = oMapX.GetPtr();
					float* pMapY = oMapY.GetPtr();

					for( int nRow = 0; nRow < nHeight; ++nRow )
					{
						for( int nPos = 0; nPos < nWidth; ++nPos )
						{
							double x = nPos - dCenterX;
							double y = nRow - dCenterY;

							double ang = ::atan2( y, x );
							if( ang < 0 )
								ang += IPP_2PI;

							int index = 0;
							
							for( int i = 0; i < nCountLines; i++ )
							{
								if( ang < aLines[i].angle )
								{
									index = i;
									break;
								}
							}

							x *= aLines[index].factor;
							y *= aLines[index].factor;

							x += dCenterX;
							y += dCenterY;
							
							*pMapX++ = float(x);
							*pMapY++ = float(y);
						}
					}

					ippiRemap_8u_C4R(pPixels, SrcSize, 4*nWidth, SrcRect, 
						oMapX.GetPtr(), sizeof(Ipp32f) * nWidth, 
						oMapY.GetPtr(), sizeof(Ipp32f) * nWidth, 
						pBuffer, 4*nWidth, DstSize, IPPI_INTER_LINEAR );

					memcpy( pPixels, oBuffer.GetPtr(), nWidth * nHeight * 4 );

					return c_nErrorNone;
				}


				int BGRA_FilterPrewitt( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Type )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;

					IppiSize roiBuffer;
					roiBuffer.width = nWidth + 2;
					roiBuffer.height = nHeight + 2;

					Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
					if (!pBuffer)
						return c_nErrorNoMemory;

					ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 1, 1);

					if (c_nFilterPrewittHoriz == Type)
					{
						ippiFilterPrewittHoriz_8u_AC4R(
							(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
							pPixels, 4*nWidth, roiSize);
					}
					else if (c_nFilterPrewittVertical == Type)
					{
						ippiFilterPrewittVert_8u_AC4R(
							(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
							pPixels, 4*nWidth, roiSize);
					}
					else
					{
						delete []pBuffer;
						
						return c_nErrorNone;
					}

					Ipp8u alpha = Ipp8u((1.0-Frame) * 255);
					ippiAlphaCompC_8u_AC4R( 
						(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, alpha,
						pPixels, 4*nWidth, 255, 
						pPixels, 4*nWidth, roiSize, ippAlphaOver);

					delete [] pBuffer;
					
					return c_nErrorNone;
				}
				int BGRA_FilterScharr( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Type )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;

					IppiSize roiBuffer;
					roiBuffer.width = nWidth + 2;
					roiBuffer.height = nHeight + 2;

					Ipp8u* pChanel = new Ipp8u[roiBuffer.width*roiBuffer.height];
					if (!pChanel)
						return c_nErrorNoMemory;


					ippiCopy_8u_C4C1R(pPixels, 4*nWidth, 
						(Ipp8u*)(pChanel + roiBuffer.width + 1), roiBuffer.width, roiSize);

					ippiCopyReplicateBorder_8u_C1R(
						(Ipp8u*)(pChanel + roiBuffer.width + 1), roiBuffer.width, roiSize, 
						pChanel, roiBuffer.width, roiBuffer, 1, 1);

					Ipp16s* pExtChanel = new Ipp16s[nWidth*nHeight];
					if (!pExtChanel)
					{
						delete []pChanel;
						return c_nErrorNoMemory;
					}
					if (c_nFilterScharrHoriz == Type)
					{
						ippiFilterScharrHoriz_8u16s_C1R(
							(Ipp8u*)(pChanel+roiBuffer.width + 1), roiBuffer.width, 
							pExtChanel, 2*nWidth, roiSize);
					}
					else if (c_nFilterScharrVertical == Type)
					{
						ippiFilterScharrVert_8u16s_C1R(
							(Ipp8u*)(pChanel+roiBuffer.width + 1), roiBuffer.width, 
							pExtChanel, 2*nWidth, roiSize);
					}
					else
					{
						delete []pChanel;
						delete []pExtChanel;
						return c_nErrorNone;
					}

					ippiConvert_16s8u_C1R(pExtChanel, 2*nWidth, pChanel, nWidth, roiSize);

					delete []pExtChanel;
					if (Frame>c_dFrameMax)
					{
						ippiCopy_8u_C1C4R(pChanel, nWidth, pPixels, 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pPixels + 1), 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pPixels + 2), 4*nWidth, roiSize);
					}
					else
					{
						Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];
						if (!pBuffer)
						{
							delete []pChanel;
							return c_nErrorNoMemory;
						}
						ippiCopy_8u_C1C4R(pChanel, nWidth, pBuffer, 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pBuffer + 1), 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pBuffer + 2), 4*nWidth, roiSize);

						Ipp8u alpha = Ipp8u(Frame * 255);	
						ippiAlphaCompC_8u_AC4R( 
							pBuffer, 4*nWidth, alpha,
							pPixels, 4*nWidth, 255, 
							pPixels, 4*nWidth, roiSize, ippAlphaOver);

						delete []pBuffer;
					}

					delete []pChanel;

					return c_nErrorNone;
				}
				int BGRA_FilterSobel( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Type )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;

					IppiSize roiBuffer;
					IppiMaskSize mask;

					int nOffset;
					switch (Type)
					{
					case c_nFilterSobelHoriz3x3:
					case c_nFilterSobelVertical3x3:
					case c_nFilterSobelCross3x3:
					case c_nFilterSobelSecondHoriz3x3:
					case c_nFilterSobelSecondVertical3x3:
						{
							roiBuffer.width = nWidth + 2;
							roiBuffer.height = nHeight + 2;
							nOffset = roiBuffer.width + 1;
							mask = ippMskSize3x3;
						}
						break;
					case c_nFilterSobelHoriz5x5:
					case c_nFilterSobelVertical5x5:
					case c_nFilterSobelCross5x5:
					case c_nFilterSobelSecondHoriz5x5:
					case c_nFilterSobelSecondVertical5x5:
						{
							roiBuffer.width = nWidth + 4;
							roiBuffer.height = nHeight + 4;
							nOffset = 2*roiBuffer.width + 2;
							mask = ippMskSize5x5;
						}
						break;
					}

					Ipp8u* pChanel = new Ipp8u[roiBuffer.width*roiBuffer.height];
					if (!pChanel)
						return c_nErrorNoMemory;

					if (ippMskSize3x3==mask)
					{
						ippiCopy_8u_C4C1R(pPixels, 4*nWidth, 
							(Ipp8u*)(pChanel + nOffset), roiBuffer.width, roiSize);

						ippiCopyReplicateBorder_8u_C1R(
							(Ipp8u*)(pChanel + nOffset), roiBuffer.width, roiSize, 
							pChanel, roiBuffer.width, roiBuffer, 1, 1);
					}
					else
					{
						ippiCopy_8u_C4C1R(pPixels, 4*nWidth, 
							(Ipp8u*)(pChanel + nOffset), roiBuffer.width, roiSize);

						ippiCopyReplicateBorder_8u_C1R(
							(Ipp8u*)(pChanel + nOffset), roiBuffer.width, roiSize, 
							pChanel, roiBuffer.width, roiBuffer, 2, 2);
					}

					Ipp16s* pExtChanel = new Ipp16s[nWidth*nHeight];
					if (!pExtChanel)
					{
						delete []pChanel;
						return c_nErrorNoMemory;
					}

					switch (Type)
					{
					case c_nFilterSobelHoriz3x3:
					case c_nFilterSobelHoriz5x5:
						ippiFilterSobelHoriz_8u16s_C1R(
							(Ipp8u*)(pChanel+nOffset), roiBuffer.width, 
							pExtChanel, 2*nWidth, roiSize, mask);
						break;
					case c_nFilterSobelVertical3x3:
					case c_nFilterSobelVertical5x5:
						ippiFilterSobelVert_8u16s_C1R(
							(Ipp8u*)(pChanel+nOffset), roiBuffer.width, 
							pExtChanel, 2*nWidth, roiSize, mask);
						break;
					case c_nFilterSobelCross3x3:
					case c_nFilterSobelCross5x5:
						ippiFilterSobelCross_8u16s_C1R(
							(Ipp8u*)(pChanel+nOffset), roiBuffer.width, 
							pExtChanel, 2*nWidth, roiSize, mask);
						break;
					case c_nFilterSobelSecondHoriz3x3:
					case c_nFilterSobelSecondHoriz5x5:
						ippiFilterSobelHorizSecond_8u16s_C1R(
							(Ipp8u*)(pChanel+nOffset), roiBuffer.width, 
							pExtChanel, 2*nWidth, roiSize, mask);
						break;
					case c_nFilterSobelSecondVertical3x3:
					case c_nFilterSobelSecondVertical5x5:
						ippiFilterSobelVertSecond_8u16s_C1R(
							(Ipp8u*)(pChanel+nOffset), roiBuffer.width, 
							pExtChanel, 2*nWidth, roiSize, mask);
						break;
					default:
						{
							delete []pChanel;
							delete []pExtChanel;
							return c_nErrorNone;
						}
					}

					ippiConvert_16s8u_C1R(pExtChanel, 2*nWidth, pChanel, nWidth, roiSize);

					delete []pExtChanel;
					if (Frame > 0.999)
					{
						ippiCopy_8u_C1C4R(pChanel, nWidth, pPixels, 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pPixels + 1), 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pPixels + 2), 4*nWidth, roiSize);
					}
					else
					{
						Ipp8u* pBuffer = new Ipp8u[4*nWidth*nHeight];
						if (!pBuffer)
						{
							delete []pChanel;
							return c_nErrorNoMemory;
						}
						ippiCopy_8u_C1C4R(pChanel, nWidth, pBuffer, 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pBuffer + 1), 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(pChanel, nWidth, (Ipp8u*)(pBuffer + 2), 4*nWidth, roiSize);

						Ipp8u alpha = Ipp8u(Frame * 255);
						ippiAlphaCompC_8u_AC4R( 
							pBuffer, 4*nWidth, alpha,
							pPixels, 4*nWidth, 255, 
							pPixels, 4*nWidth, roiSize, ippAlphaOver);

						delete []pBuffer;
					}

					delete []pChanel;

					return c_nErrorNone;
				}

				int BGRA_FilterSobelEx( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Type )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;

					IppiSize roiBuffer;
					roiBuffer.width = nWidth + 2;
					roiBuffer.height = nHeight + 2;

					Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
					if (!pBuffer)
						return c_nErrorNoMemory;

					ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 1, 1);

					if (c_nFilterSobelExtHoriz == Type)
					{
						ippiFilterSobelHoriz_8u_AC4R(
							(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
							pPixels, 4*nWidth, roiSize);
					}
					else if (c_nFilterSobelExtVertical == Type)
					{
						ippiFilterSobelVert_8u_AC4R(
							(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
							pPixels, 4*nWidth, roiSize);
					}
					else
					{
						delete []pBuffer;
						return c_nErrorNone;
					}

					Ipp8u alpha = Ipp8u((1.0-Frame) * 255);
					ippiAlphaCompC_8u_AC4R( 
						(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, alpha,
						pPixels, 4*nWidth, 255, 
						pPixels, 4*nWidth, roiSize, ippAlphaOver);

					delete [] pBuffer;

					return c_nErrorNone;
				}

				int BGRA_FilterRoberts( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Type )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;

					IppiSize roiBuffer;
					roiBuffer.width = nWidth + 2;
					roiBuffer.height = nHeight + 2;

					Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
					if (!pBuffer)
						return c_nErrorNoMemory;

					ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, 1, 1);

					if (c_nFilterRobertsUp == Type)
					{
						ippiFilterRobertsUp_8u_AC4R(
							(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
							pPixels, 4*nWidth, roiSize);
					}
					else if (c_nFilterRobertsDown == Type)
					{
						ippiFilterRobertsDown_8u_AC4R(
							(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, 
							pPixels, 4*nWidth, roiSize);
					}
					else
					{
						delete []pBuffer;
						return c_nErrorNone;
					}

					Ipp8u alpha = Ipp8u((1.0-Frame) * 255);
					ippiAlphaCompC_8u_AC4R( 
						(Ipp8u*)(pBuffer+4*roiBuffer.width + 4), 4*roiBuffer.width, alpha,
						pPixels, 4*nWidth, 255, 
						pPixels, 4*nWidth, roiSize, ippAlphaOver);

					delete [] pBuffer;

					return c_nErrorNone;
				}
				int BGRA_FilterLaplace( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Type )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;

					IppiSize roiBuffer;
					IppiMaskSize mask;
					int nOffset;
					int nBorder;
					if (c_nFilterLaplace3x3 == Type)
					{
						nBorder = 1;
						roiBuffer.width = nWidth + 2;
						roiBuffer.height = nHeight + 2;
						mask = ippMskSize3x3;
						nOffset = 4*roiBuffer.width + 4;
					}
					else if (c_nFilterLaplace5x5 == Type)
					{	
						nBorder = 2;
						roiBuffer.width = nWidth + 4;
						roiBuffer.height = nHeight + 4;
						mask = ippMskSize5x5;
						nOffset = 8*roiBuffer.width + 8;
					}
					else
						return c_nErrorNone;

					Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
					if (!pBuffer)
						return c_nErrorNoMemory;

					ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, nBorder, nBorder);

					ippiFilterLaplace_8u_AC4R(
						(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, 
						pPixels, 4*nWidth, roiSize, mask);

					Ipp8u alpha = Ipp8u((1.0-Frame) * 255);
					ippiAlphaCompC_8u_AC4R( 
						(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, alpha,
						pPixels, 4*nWidth, 255, 
						pPixels, 4*nWidth, roiSize, ippAlphaOver);

					delete [] pBuffer;

					return c_nErrorNone;
				}
				int BGRA_FilterHipass( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Type )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;

					IppiSize roiBuffer;
					IppiMaskSize mask;
					int nOffset;
					int nBorder;
					if (c_nFilterHipass3x3 == Type)
					{
						nBorder = 1;
						roiBuffer.width = nWidth + 2;
						roiBuffer.height = nHeight + 2;
						mask = ippMskSize3x3;
						nOffset = 4*roiBuffer.width + 4;
					}
					else if (c_nFilterHipass5x5 == Type)
					{	
						nBorder = 2;
						roiBuffer.width = nWidth + 4;
						roiBuffer.height = nHeight + 4;
						mask = ippMskSize5x5;
						nOffset = 8*roiBuffer.width + 8;
					}
					else
						return c_nErrorNone;

					Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
					if (!pBuffer)
						return c_nErrorNoMemory;

					ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, nBorder, nBorder);

					ippiFilterHipass_8u_AC4R(
						(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, 
						pPixels, 4*nWidth, roiSize, mask);

					Ipp8u alpha = Ipp8u((1.0-Frame) * 255);
					ippiAlphaCompC_8u_AC4R( 
						(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, alpha,
						pPixels, 4*nWidth, 255, 
						pPixels, 4*nWidth, roiSize, ippAlphaOver);

					delete [] pBuffer;

					return c_nErrorNone;
				}

				int BGRA_FilterLowpass( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Type )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;

					IppiSize roiBuffer;
					IppiMaskSize mask;
					int nOffset;
					int nBorder;
					if (c_nFilterLowpass3x3 == Type)
					{
						nBorder = 1;
						roiBuffer.width = nWidth + 2;
						roiBuffer.height = nHeight + 2;
						mask = ippMskSize3x3;
						nOffset = 4*roiBuffer.width + 4;
					}
					else if (c_nFilterLowpass5x5 == Type)
					{	
						nBorder = 2;
						roiBuffer.width = nWidth + 4;
						roiBuffer.height = nHeight + 4;
						mask = ippMskSize5x5;
						nOffset = 8*roiBuffer.width + 8;
					}
					else
						return c_nErrorNone;

					Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
					if (!pBuffer)
						return c_nErrorNoMemory;

					ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, nBorder, nBorder);

					ippiFilterLowpass_8u_AC4R(
						(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, 
						pPixels, 4*nWidth, roiSize, mask);

					Ipp8u alpha = Ipp8u((1.0-Frame) * 255);
					ippiAlphaCompC_8u_AC4R( 
						(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, alpha,
						pPixels, 4*nWidth, 255, 
						pPixels, 4*nWidth, roiSize, ippAlphaOver);

					delete [] pBuffer;
					
					return c_nErrorNone;
				}

				int BGRA_FilterBlur( BYTE* pPixels, int nWidth, int nHeight, double Frame, long Size )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;

					IppiSize roiBuffer = {nWidth + 2*Size, nHeight + 2*Size};
					IppiSize maskSize = {2*Size+1,2*Size+1};	
					IppiPoint anchor = {Size,Size};
					int nOffset = (4*roiBuffer.width + 4)*Size;

					Ipp8u* pBuffer = new Ipp8u[4*roiBuffer.width*roiBuffer.height];
					if (!pBuffer)
						return c_nErrorNoMemory;

					ippiCopyReplicateBorder_8u_AC4R(pPixels, 4*nWidth, roiSize, pBuffer, 4*roiBuffer.width,roiBuffer, Size, Size);

					ippiFilterBox_8u_AC4R(
						(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, 
						pPixels, 4*nWidth, roiSize, maskSize, anchor);

					Ipp8u alpha = Ipp8u((1.0-Frame) * 255);	
					ippiAlphaCompC_8u_AC4R( 
						(Ipp8u*)(pBuffer+nOffset), 4*roiBuffer.width, alpha,
						pPixels, 4*nWidth, 255, 
						pPixels, 4*nWidth, roiSize, ippAlphaOver);

					delete [] pBuffer;

					return c_nErrorNone;
				}

				int BGRA_FilterCanny( BYTE* pPixels, int nWidth, int nHeight, double Frame, double LowThresh, double HighThresh )
				{
					// clamp values
					Frame = Clamp(Frame);

					IppiSize roiSize;
					roiSize.width = nWidth;
					roiSize.height = nHeight;
					Ipp8u *pGrayScaleBuf = new Ipp8u[nWidth*nHeight];
					Ipp16s *pDXBuf = new Ipp16s[nWidth*nHeight];
					Ipp16s *pDYBuf = new Ipp16s[nWidth*nHeight];
					
					ippiCopy_8u_C4C1R(pPixels, 4*nWidth, 
						pGrayScaleBuf, nWidth, roiSize);


					ippiSobel3x3_Dx_8u16s_C1R(pGrayScaleBuf, nWidth, pDXBuf, 2*nWidth, roiSize);
					ippiSobel3x3_Dy_8u16s_C1R(pGrayScaleBuf, nWidth, pDYBuf, 2*nWidth, IPCV_ORIGIN_TL, roiSize);

					int nBufSize;
					ippiCannyGetSize(roiSize, &nBufSize);
					LPBYTE pBuffer = new BYTE[nBufSize];
					ippiCanny_16s8u_C1R(pDXBuf, 2*nWidth,
										pDYBuf, 2*nWidth,
										pGrayScaleBuf, nWidth, roiSize, 
										float(LowThresh), float(HighThresh),
										pBuffer);

					delete [] pBuffer;

					if (Frame>0.999)
					{
						ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
											pPixels, 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
											(Ipp8u*)(pPixels+1), 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
											(Ipp8u*)(pPixels+2), 4*nWidth, roiSize);
					}
					else
					{
						Ipp8u *pTemp = new Ipp8u[4*nWidth*nHeight];
						ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
											pTemp, 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
											(Ipp8u*)(pTemp+1), 4*nWidth, roiSize);
						ippiCopy_8u_C1C4R(	pGrayScaleBuf, nWidth, 
											(Ipp8u*)(pTemp+2), 4*nWidth, roiSize);

						Ipp8u alpha = Ipp8u(Frame * 255);
						ippiAlphaCompC_8u_AC4R( 
							(Ipp8u*)pTemp, 4*nWidth, alpha,
							pPixels, 4*nWidth, 255, 
							(Ipp8u*)pPixels, 4*nWidth, roiSize, ippAlphaOver);

						delete [] pTemp;
					}
					
					delete [] pGrayScaleBuf;
					delete [] pDXBuf;
					delete [] pDYBuf;

					return c_nErrorNone;
				}


				int BGRA_DrawImageFromFileEx( BYTE* pSourceBGRA, int nSourceWidth, int nSourceHeight, int nSourceStride, BYTE* pResultBGRA, int nResultWidth, int nResultHeight, int nResultStride, double dSrcX, double dSrcY, double dSrcWidth, double dSrcHeight, double dDstX, double dDstY, double dDstWidth, double dDstHeight, int nSrcColorKey, int nScaleType, double dCropScale, double dAngle, double dAlpha, double dSrcAspect )
				{
					if( !dDstWidth  ) dDstWidth  = nResultWidth;
					if( !dDstHeight ) dDstHeight = nResultHeight;
					if( !dSrcWidth  ) dSrcWidth  = nSourceWidth;
					if( !dSrcHeight ) dSrcHeight = nSourceHeight;

					int nDstLeft   = Round( dDstX );
					int nDstTop    = Round( dDstY );
					int nDstRight  = Round( dDstX + dDstWidth );
					int nDstBottom = Round( dDstY + dDstHeight );

					if( nDstLeft >= nDstRight || nDstTop >= nDstBottom || 
						nDstLeft >= nResultWidth  || nDstRight  <= 0 ||
						nDstTop  >= nResultHeight || nDstBottom <= 0 )
					{
						return c_nErrorNone;
					}

					nDstLeft   = max(nDstLeft, 0);
					nDstTop    = max(nDstTop, 0);
					nDstRight  = min(nDstRight, nResultWidth);
					nDstBottom = min(nDstBottom, nResultHeight);

					int nResizeWidth  = nDstRight  - nDstLeft;
					int nResizeHeight = nDstBottom - nDstTop;

					MediaCore::IAVSUncompressedVideoFrame* pResizeFrame = NULL;

					int nRetCode = BGRA_TransformResizeMedia( pSourceBGRA, nSourceWidth, nSourceHeight, nSourceStride, (IUnknown**)&pResizeFrame, nResizeWidth, nResizeHeight, 0, nScaleType, dSrcAspect, dCropScale, 0, Round( dSrcX ), Round( dSrcY ), Round( dSrcWidth ), Round( dSrcHeight ) ); 
					if( nRetCode != c_nErrorNone )
						return nRetCode;

					BYTE* pResizeBuffer = NULL;
					pResizeFrame->get_Buffer( &pResizeBuffer );

					DWORD alpha = RealToByte( dAlpha ) * 0x8081;
					int nSize = nResizeWidth * nResizeHeight;
					for( int i = 0; i < nSize; i++ )
					{
						pResizeBuffer[i * 4 + 3] = BYTE((pResizeBuffer[i * 4 + 3] * alpha + (128 * 0x8081)) >> 23);
					}
					
					BYTE* pResult = pResultBGRA + nDstLeft * 4;
					
					if( nResultStride < 0 )
						pResult -= nResultStride * (nResultHeight - 1 - nDstTop);
					else
						pResult += nResultStride * nDstTop;

					BYTE* pSource = pResizeBuffer;
					int nResizeStride = nResizeWidth * 4;
					
					if( nSourceStride < 0 )
					{
						pSource += nResizeStride * (nResizeHeight - 1);
						nResizeStride = -nResizeStride;
					}

					Gdiplus::Bitmap oResultImage( nResizeWidth, nResizeHeight, nResultStride, PixelFormat32bppARGB, pResult );
					if( Gdiplus::Ok == oResultImage.GetLastStatus() )
					{
						Gdiplus::Bitmap oSourceImage( nResizeWidth, nResizeHeight, nResizeStride, PixelFormat32bppARGB, pSource );
						if( Gdiplus::Ok == oSourceImage.GetLastStatus() )
						{
							Gdiplus::Graphics oRender( &oResultImage );
							if( Gdiplus::Ok == oRender.GetLastStatus() )
							{
								oRender.SetInterpolationMode( Gdiplus::InterpolationModeBilinear );
								oRender.SetSmoothingMode( Gdiplus::SmoothingModeAntiAlias );

								Gdiplus::Matrix matrix( 1, 0, 0, 1, 0, 0 );
								matrix.RotateAt( float(-dAngle), Gdiplus::PointF( float(nResizeWidth / 2.0), float(nResizeHeight / 2.0)), Gdiplus::MatrixOrderAppend );
								oRender.SetTransform( &matrix );

								oRender.DrawImage( &oSourceImage, 0, 0 );
							}
						}
					}

					pResizeFrame->Release();

					return c_nErrorNone;
				}

#endif
				int BGRA_AdjustClarity( BYTE* pPixels, int nWidth, int nHeight, double dFrame, double dLevel, double dRadius, double dThreshold )
				{
					// clamp values
					dFrame = Clamp(dFrame);

					int nSize = nWidth * nHeight;

					CBuffer<BYTE> oBuffer( nSize * 4 );
					if( oBuffer.IsEmpty() )
						return c_nErrorNoMemory;

					::memcpy( oBuffer.GetPtr(), pPixels, nSize * 4 );

					BOOL bSuccess = BGRA_IPPEffectGaussianBlur2( oBuffer.GetPtr(), nWidth, nHeight, dFrame, long(dRadius * dFrame + 0.5) );
					if( !bSuccess )
						return c_nErrorNoMemory;

					
					BYTE* src = oBuffer.GetPtr();
					BYTE* dst = pPixels;

					double dFactor = dLevel * dFrame / 100.0;
					double dBorder = max(0, dThreshold);


					for( int i = 0; i < nSize; ++i, src += 4, dst += 4 )
					{
						int b = dst[0];
						int g = dst[1];
						int r = dst[2];

						int nSubB = src[0] - b;
						int nSubG = src[1] - g;
						int nSubR = src[2] - r;

						if( abs(nSubB) > dBorder )
						{
							double inc = (dBorder - abs(nSubB)) * dFactor * (nSubB > 0 ? 1 : -1);
							dst[0] = RealToByte( b + inc );
						}

						if( abs(nSubG) > dBorder )
						{
							double inc = (dBorder - abs(nSubG)) * dFactor * (nSubG > 0 ? 1 : -1);
							dst[1] = RealToByte( g + inc );
						}

						if( abs(nSubR) > dBorder )
						{
							double inc = (dBorder - abs(nSubR)) * dFactor * (nSubR > 0 ? 1 : -1);
							dst[2] = RealToByte( r + inc );
						}
					}

					return c_nErrorNone;
				}

				int BGRA_EffectWaterMirrow(BYTE* pPixels, int nWidth, int nHeight, double Frame, BYTE* pMask, double m[9], int alpha1, int alpha2 )
				{
					// clamp values
					Frame = Clamp(Frame);

					double dAlpha1 = (alpha1 / 100.0) * Frame;
					double dAlpha2 = (alpha2 / 100.0) * Frame;

					if( dAlpha1 < 0.001 && dAlpha2 < 0.001 )
						return c_nErrorNone;

					double dMinLen = 0;
					double dMaxLen = 0;
					BOOL bInit = FALSE;

					BYTE* p = pMask;

					for( int y = 0; y < nHeight; ++y )
					{
						double dY = y * m[7] + m[8];

						for( int x = 0; x < nWidth; ++x )
						{
							if( 0 == *p++ )
								continue;

							double len = abs(x * m[6] + dY);

							if( bInit )
							{
								if( len > dMaxLen ) dMaxLen = len;
								if( len < dMinLen ) dMinLen = len;
							}
							else
							{
								dMaxLen = len;
								dMinLen = len;
								bInit = TRUE;
							}
						}
					}

					double dLength = dMaxLen - dMinLen;

					if( dLength < IPP_EPS23 )
						return c_nErrorNone;


					double del = m[0] * m[3] - m[1] * m[2];
					if( abs(del) < IPP_EPS52 )
						return c_nErrorNone;

					double t[6] = {
						 m[3] / del,
						-m[1] / del,
						-m[2] / del,
						 m[0] / del,
						(m[2] * m[5] - m[4] * m[3]) / del,
						(m[4] * m[1] - m[0] * m[5]) / del
					};

					int nSize = nWidth * nHeight;

					CBuffer<BYTE>  oBuffer( nSize * 4);
					CBuffer<float> oMapX( nSize );
					CBuffer<float> oMapY( nSize );

					if( oBuffer.IsEmpty() || oMapX.IsEmpty() || oMapY.IsEmpty() )
						return c_nErrorNoMemory;

					::memset( oBuffer.GetPtr(), 0, nSize * 4 );

					float* pMapX = oMapX.GetPtr();
					float* pMapY = oMapY.GetPtr();

					p = pMask;

					for( int y = 0; y < nHeight; ++y )
					{
						for( int x = 0; x < nWidth; ++x, ++pMapX, ++pMapY )
						{
							if( 0 == *p++ )
							{
								pMapX[0] = -1;
								pMapY[0] = -1;
								continue;
							}

							pMapX[0] = float(x * t[0] + y * t[2] + t[4]);
							pMapY[0] = float(x * t[1] + y * t[3] + t[5]);
						}
					}

					IppiSize SrcSize = {nWidth, nHeight};
					IppiRect SrcRect = {0, 0, nWidth, nHeight};

					ippiRemap_8u_C4R( pPixels, SrcSize, 4*nWidth, SrcRect, 
						oMapX.GetPtr(), sizeof(float) * nWidth, 
						oMapY.GetPtr(), sizeof(float) * nWidth, 
						oBuffer.GetPtr(), 4*nWidth, SrcSize, IPPI_INTER_LINEAR );

					
					double dFactor = 1.0 / dLength;

					p = pMask;
					BYTE* src = oBuffer.GetPtr();
					BYTE* dst = pPixels;

					double dL = 0;

					for( int y = 0; y < nHeight; ++y )
					{
						double dY = y * m[7] + m[8];

						for( int x = 0; x < nWidth; ++x, src += 4, dst += 4 )
						{
							if( 0 == *p++ )
								continue;

							double len = (abs(x * m[6] + dY) - dMinLen) * dFactor;
							 
							double SrcAlpha = Clamp((dAlpha2 - dAlpha1) * len + dAlpha1) * src[3] * (1.0/255.0);
							double DstAlpha = (1 - SrcAlpha); // * dst[3] * (1.0/255.0);

							dst[0] = BYTE(src[0] * SrcAlpha + dst[0] * DstAlpha + 0.5);
							dst[1] = BYTE(src[1] * SrcAlpha + dst[1] * DstAlpha + 0.5);
							dst[2] = BYTE(src[2] * SrcAlpha + dst[2] * DstAlpha + 0.5);
							//dst[3] = BYTE(Clamp( SrcAlpha + DstAlpha ) * 255 + 0.5);
						}
					}

					return c_nErrorNone;
				}


				
				namespace Anaglyph
				{
					struct TKoef
					{
						short i[4]; // index
						short f[4]; // factor
					};
					
					void FillKoefTables( TKoef* pTableL, TKoef* pTableR, int nSizeTable, int nWidth, int nShift )
					{
						pTableR += (nSizeTable - 1);

						for( int i = 0; i < nSizeTable; ++i, ++pTableL, --pTableR )
						{
							double l = (sqrt( double((i + 1) * 8) / nSizeTable + 1 ) - 1) / 2 * nShift + (nWidth - nShift - 1);
							double r = (nWidth - 1) - l;

							double arrFactors[2] = {l, r};
							TKoef* arrTables[2] = {pTableL, pTableR};

							for( int n = 0; n < 2; n++ )
							{
								TKoef* pTable = arrTables[n];
								double t = arrFactors[n];
								
								int x = int(t);
								t -= x;
								
								if( t < 0 )
								{
									x -= 1;
									t += 1;
								}

								double t2 = t * t;
								double t3 = t * t * t;

								double k0 = (3 * t2 - t3 - 2 * t) / 6;
								double k1 = (t3 - t) * 0.5 - t2 + 1;
								double k2 = (t2 - t3) * 0.5 + t;
								double k3 = (t3 - t) / 6;

								pTable->f[0] = short(k0 * 16384 + (k0 < 0 ? 0.5 : -0.5));
								pTable->f[1] = short(k1 * 16384 + (k1 < 0 ? 0.5 : -0.5));
								pTable->f[2] = short(k2 * 16384 + (k2 < 0 ? 0.5 : -0.5));
								pTable->f[3] = short(k3 * 16384 + (k3 < 0 ? 0.5 : -0.5));

								int i0 = x - 1;
								int i1 = x;
								int i2 = x + 1;
								int i3 = x + 2;

								pTable->i[0] = short((i0 < 0) ? 0 : ((i0 > nWidth - 1) ? nWidth - 1 : i0));
								pTable->i[1] = short((i1 < 0) ? 0 : ((i1 > nWidth - 1) ? nWidth - 1 : i1));
								pTable->i[2] = short((i2 < 0) ? 0 : ((i2 > nWidth - 1) ? nWidth - 1 : i2));
								pTable->i[3] = short((i3 < 0) ? 0 : ((i3 > nWidth - 1) ? nWidth - 1 : i3));
							}
						}
					}

					void CopyBorders( BYTE* pSrcRow, BYTE* pDstRowL, BYTE* pDstRowR, int nRowLength, int nShift )
					{
						pDstRowR += nShift * 3;
						nRowLength -= nShift;
						
						for( int i = 0; i < nRowLength; ++i, pDstRowL += 3, pDstRowR += 3, pSrcRow += 4 )
						{
							pDstRowL[0] = pSrcRow[0 + nShift * 4];
							pDstRowL[1] = pSrcRow[1 + nShift * 4];
							pDstRowL[2] = pSrcRow[2 + nShift * 4];

							pDstRowR[0] = pSrcRow[0];
							pDstRowR[1] = pSrcRow[1];
							pDstRowR[2] = pSrcRow[2];
						}
					}
					void Interpolate( BYTE* pSrc, BYTE* pDst, TKoef* pMap, int nCount )
					{
						for( int i = 0; i < nCount; ++i, pDst += 3, pMap += 1 )
						{
							int b = 0;
							int g = 0;
							int r = 0;

							for( int k = 0; k < 4; k++ )
							{
								BYTE* s = pSrc + int(pMap->i[k]) * 4;
								int f = pMap->f[k];

								b += s[0] * f;
								g += s[1] * f;
								r += s[2] * f;
							}

							b = (b + 8192) >> 14;
							g = (g + 8192) >> 14;
							r = (r + 8192) >> 14;

							pDst[0] = BYTE(b < 0 ? 0 : (b > 255 ? 255 : b));
							pDst[1] = BYTE(g < 0 ? 0 : (g > 255 ? 255 : g));
							pDst[2] = BYTE(r < 0 ? 0 : (r > 255 ? 255 : r));
						}
					}

					void Compose( BYTE* pDst, BYTE* pSrcL, BYTE* pSrcR, int nLength, int nMixType )
					{
						switch( nMixType )
						{
						case 0:
							for( int i = 0; i < nLength; ++i, pSrcL += 3, pSrcR += 3, pDst += 4 )
							{
								pDst[0] = pSrcR[0];
								pDst[1] = pSrcR[1];
								pDst[2] = pSrcL[2];
							}
							break;

						case 1:
							for( int i = 0; i < nLength; ++i, pSrcL += 3, pSrcR += 3, pDst += 4 )
							{
								//int b1 = pSrcL[0];
								//int g1 = pSrcL[1];
								//int r1 = pSrcL[2];
								//int b2 = pSrcR[0];
								//int g2 = pSrcR[1];
								//int r2 = pSrcR[2];
								//
								//int b = (-/*0.0547*/ 3585 * r1 - /*0.0615*/ 4030 * g1 + /*0.0128*/  839 * b1) + (-/*0.0651*/ 4266 * r2 - /*0.1287*/ 8434 * g2 + /*1.2971*/85006 * b2);
								//int g = (-/*0.0458*/ 3002 * r1 - /*0.0484*/ 3172 * g1 - /*0.0257*/ 1684 * b1) + ( /*0.3756*/24615 * r2 + /*0.7333*/48058 * g2 + /*0.0111*/  727 * b2);
								//int r = ( /*0.4154*/27224 * r1 + /*0.4710*/30867 * g1 + /*0.1669*/10938 * b1) + (-/*0.0109*/  714 * r2 - /*0.0364*/ 2386 * g2 - /*0.0060*/  393 * b2);
								
								int c = pSrcL[0];
								int b = c *  839;
								int g = c * -1684;
								int r = c *  10938;

								c = pSrcL[1];
								b += c * -4030;
								g += c * -3172;
								r += c *  30867;

								c = pSrcL[2];
								b += c * -3585;
								g += c * -3002;
								r += c *  27224;

								c = pSrcR[0];
								b += c *  85006;
								g += c *  727;
								r += c * -393;

								c = pSrcR[1];
								b += c * -8434;
								g += c *  48058;
								r += c * -2386;

								c = pSrcR[2];
								b += c * -4266;
								g += c *  24615;
								r += c * -714;

								pDst[0] = IntToByte( (b + 32768) >> 16 );
								pDst[1] = IntToByte( (g + 32768) >> 16 );
								pDst[2] = IntToByte( (r + 32768) >> 16 );
							}
							break;

						case 2:
							for( int i = 0; i < nLength; ++i, pSrcL += 3, pSrcR += 3, pDst += 4 )
							{
								pDst[0] = pSrcR[0];
								pDst[1] = pSrcL[1];
								pDst[2] = pSrcL[2];
							}
							break;
						}
					}

					void PostProcL( BYTE* pRow, int nWidth )
					{
						BYTE* src = pRow + (nWidth - 2) * 3;

						int old = Intensity2( src[2 + 3], src[1 + 3], src[0 + 3] );

						for( int x = 1; x < nWidth; ++x, src -= 3 )
						{
							int gray = Intensity2( src[2], src[1], src[0] );
							int factor = 256 - (gray - old);

							old = gray;

							int b = (src[0 + 3] * factor + 128) >> 8;
							int g = (src[1 + 3] * factor + 128) >> 8;
							int r = (src[2 + 3] * factor + 128) >> 8;
							
							src[0 + 3] = min( b, 255 );
							src[1 + 3] = min( g, 255 );
							src[2 + 3] = min( r, 255 );
						}
					}

					void PostProcR( BYTE* pRow, int nWidth )
					{
						BYTE* src = pRow + 3;

						int old = Intensity2( src[2 - 3], src[1 - 3], src[0 - 3] );

						for( int x = 1; x < nWidth; ++x, src += 3 )
						{
							int gray = Intensity2( src[2], src[1], src[0] );
							int factor = 256 - (gray - old);

							old = gray;

							int b = (src[0 - 3] * factor + 128) >> 8;
							int g = (src[1 - 3] * factor + 128) >> 8;
							int r = (src[2 - 3] * factor + 128) >> 8;
							
							src[0 - 3] = min( b, 255 );
							src[1 - 3] = min( g, 255 );
							src[2 - 3] = min( r, 255 );
						}
					}
				}
				
				int BGRA_EffectAnaglyph3d( BYTE* pPixels, int nWidth, int nHeight, double Frame, double dShiftPercent, int nMixType )
				{
					// nMixType: 0 - normal, 1 - extended, 2 - alternative

					Frame = Clamp(Frame);

					dShiftPercent = Clamp( dShiftPercent * Frame / 100 ) / 2;

					int nShift = int(nWidth * dShiftPercent + 0.5);

					if( nShift > nWidth / 2 )
						nShift = nWidth / 2;

					if( nShift < 1 )
						return c_nErrorNone;

					int nShift2 = nShift * 2;
					
					CBuffer<BYTE> LeftRow( nWidth * 3 );
					CBuffer<BYTE> RightRow( nWidth * 3 );
					CBuffer<Anaglyph::TKoef> KoefTableL( nShift2 );
					CBuffer<Anaglyph::TKoef> KoefTableR( nShift2 );

					if( LeftRow.IsEmpty() || RightRow.IsEmpty() || KoefTableL.IsEmpty() || KoefTableR.IsEmpty() )
						return c_nErrorNoMemory;

					// заполняем таблицу коэффтциентов для заполнения пустых краёв анаглифа
					Anaglyph::FillKoefTables( KoefTableL.GetPtr(), KoefTableR.GetPtr(), nShift2, nWidth, nShift );

					// основной цикл
					BYTE* pSrcPixels = pPixels;
					
					for( int y = 0; y < nHeight; ++y, pSrcPixels += nWidth * 4 )
					{
						// копируем строку пикселей в левую и правую часть анаглифа со сдвигом
						Anaglyph::CopyBorders( pSrcPixels, LeftRow.GetPtr(), RightRow.GetPtr(), nWidth, nShift );

						// заполняем пустые края в левой и правой части анаглифа
						Anaglyph::Interpolate( pSrcPixels, LeftRow.GetPtr() + (nWidth - nShift2) * 3, KoefTableL.GetPtr(), nShift2 );
						Anaglyph::Interpolate( pSrcPixels, RightRow.GetPtr(), KoefTableR.GetPtr(), nShift2 );

						// постобработка левой и правой части
						Anaglyph::PostProcL( LeftRow.GetPtr(), nWidth );
						Anaglyph::PostProcR( RightRow.GetPtr(), nWidth );

						// смешиваем левую и правую части анаглифа
						Anaglyph::Compose( pSrcPixels, LeftRow.GetPtr(), RightRow.GetPtr(), nWidth, nMixType );
					}

					return c_nErrorNone;
				}

			}
		}
	}
}		

