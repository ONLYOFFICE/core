#pragma once
#include "stdafx.h"
#include "File.h"
#include "GdiplusEx.h"
using namespace Gdiplus;

#include "libdjvu\GBitmap.h"

namespace NSImage
{
	class CImage
	{
	private:
		CGdiPlusInit m_oInit;

	public:
		CImage()
		{
			m_oInit.Init();
		}
		~CImage()
		{
		}

	public:
		void GetEncoderClsid(const CString& strFormat, CLSID* pClsid)
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
				if (((CString)pImageCodecInfo[nEncoder].MimeType) == strFormat)
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

		void SaveImage8bit(BITMAPINFO* pInfo, GP<GBitmap>& pBitmap, CString strFile)
		{
			if (2 == pInfo->bmiHeader.biClrUsed)
			{
				pInfo->bmiHeader.biBitCount = 1;
				return SaveImage1bit(pInfo, pBitmap, strFile);
			}
			
			CFile oFile;
			oFile.CreateFile(strFile);

			BITMAPFILEHEADER hdr;
			hdr.bfType			= 'MB';
			hdr.bfReserved1		= 0;
			hdr.bfReserved2		= 0;

			DWORD dwHeaderSize	= sizeof(BITMAPINFOHEADER) +	sizeof(RGBQUAD) * pInfo->bmiHeader.biClrUsed;

			DWORD dwBitCount	= pInfo->bmiHeader.biWidth * pInfo->bmiHeader.biBitCount;
			DWORD dwDataSize	= (((dwBitCount + 31) / 32) * 4) * pInfo->bmiHeader.biHeight;

			hdr.bfOffBits		= sizeof(BITMAPFILEHEADER) + dwHeaderSize;
			hdr.bfSize			= hdr.bfOffBits + dwDataSize;

			oFile.WriteFile(&hdr, sizeof(hdr));
			oFile.WriteFile(pInfo, dwHeaderSize);
			oFile.WriteFile(pBitmap->operator[](0), dwDataSize);

			oFile.CloseFile();
		}
		void SaveImage1bit(BITMAPINFO* pInfo, GP<GBitmap>& pBitmap, CString strFile)
		{
			/*pInfo->bmiHeader.biWidth	= 64;
			pInfo->bmiHeader.biHeight	= 64;

			LONG l = 64 * 64 / 8;

			CFile oFile;
			oFile.CreateFile(strFile);

			BITMAPFILEHEADER hdr;
			hdr.bfType			= 'MB';
			hdr.bfReserved1		= 0;
			hdr.bfReserved2		= 0;

			DWORD dwHeaderSize	= sizeof(BITMAPINFOHEADER) +	sizeof(RGBQUAD) * pInfo->bmiHeader.biClrUsed;
			hdr.bfOffBits		= sizeof(BITMAPFILEHEADER) + dwHeaderSize;
			hdr.bfSize			= hdr.bfOffBits + l;

			oFile.WriteFile(&hdr, sizeof(hdr));
			oFile.WriteFile(pInfo, dwHeaderSize);

			oFile.WriteReserved(l);
			oFile.CloseFile();*/

			CFile oFile;
			oFile.CreateFile(strFile);

			BITMAPFILEHEADER hdr;
			hdr.bfType			= 'MB';
			hdr.bfReserved1		= 0;
			hdr.bfReserved2		= 0;

			DWORD dwHeaderSize	= sizeof(BITMAPINFOHEADER) +	sizeof(RGBQUAD) * pInfo->bmiHeader.biClrUsed;
			
			LONG lWidth			= pInfo->bmiHeader.biWidth;
			LONG lHeight		= pInfo->bmiHeader.biHeight;

			//pInfo->bmiHeader.biWidth = (pInfo->bmiHeader.biWidth >> 3) << 3;
			
			DWORD dwBitCount	= pInfo->bmiHeader.biWidth * pInfo->bmiHeader.biBitCount;
			DWORD dwDataSize	= (((dwBitCount + 31) / 32) * 4) * pInfo->bmiHeader.biHeight;
			
			hdr.bfOffBits		= sizeof(BITMAPFILEHEADER) + dwHeaderSize;
			hdr.bfSize			= hdr.bfOffBits + dwDataSize;

			oFile.WriteFile(&hdr, sizeof(hdr));
			oFile.WriteFile(pInfo, dwHeaderSize);

			BYTE* p1Bit = new BYTE[dwDataSize];

			_8BitTo1Bit(pBitmap, p1Bit, lWidth, lHeight, (((dwBitCount + 31) / 32) * 4));
			oFile.WriteFile(p1Bit, dwDataSize);
			RELEASEARRAYOBJECTS(p1Bit);

			oFile.CloseFile();
		}
		void SaveImage24bit(GPixel* pBits, LONG lWidth, LONG lHeight, CString strFile)
		{
			LONG lPixels = lWidth * lHeight;
			
			BYTE* pPixels = new BYTE[4 * lPixels];
			BYTE* pPixMem = pPixels;
			
			for (LONG j = lHeight - 1; j >= 0; --j)
			{
				GPixel* pLine = pBits + j * lWidth;
				for (LONG i = 0; i < lWidth; ++i)
				{
					*pPixMem++ = pLine->b;
					*pPixMem++ = pLine->g;
					*pPixMem++ = pLine->r;
					*pPixMem++ = 255; ++pLine;
				}
			}
		
			Gdiplus::Bitmap oBitmap(lWidth, lHeight, 4 * lWidth, PixelFormat32bppRGB, pPixels);

			CLSID oEncoder; GetEncoderClsid(_T("image/png"), &oEncoder);
			EncoderParameters oSettings;

			ULONG nQuality							= 100;
			oSettings.Count							= 1;
			oSettings.Parameter[0].Guid				= EncoderQuality;
			oSettings.Parameter[0].Type				= EncoderParameterValueTypeLong;
			oSettings.Parameter[0].NumberOfValues	= 1;
			oSettings.Parameter[0].Value			= &nQuality;

			BSTR bsFile = strFile.AllocSysString();
			oBitmap.Save(bsFile, &oEncoder, NULL/*&oSettings*/);
			SysFreeString(bsFile);

			RELEASEARRAYOBJECTS(pPixels);
		}

		void _8BitTo1Bit(GP<GBitmap>& pBitmap, BYTE* p1Bit, LONG lWidth, LONG lHeight, LONG lStride)
		{
			for (LONG j = 0; j < lHeight; ++j)
			{
				BYTE* p8 = pBitmap->operator [](j);
				
				BYTE* pMem = p1Bit + j * lStride;
				int nBitIndex = 0;

				for (LONG i = 0; i < lWidth; ++i, ++p8)
				{
					if (0 != *p8)
					{
						*pMem |= (1 << (7 - nBitIndex));
						//SetBit(pMem, 7-nBitIndex, true);
					}
					else
					{
						//SetBit(pMem, 7-nBitIndex, false);
						*pMem &= ~(1 << (7 - nBitIndex));
					}

					++nBitIndex;
					if (8 == nBitIndex)
					{
						nBitIndex = 0;
						++pMem;
					}
				}
			}
		}

		inline void SetBit(BYTE* value, int pos, bool newVal)
		{
			// variables
			int mask = (1 << pos);
			BYTE val = *value;

			// check for new value
			if (newVal)
				val |= mask;
			else
				val &= ~mask;

			// update value
			*value = val;
		}
	};
}