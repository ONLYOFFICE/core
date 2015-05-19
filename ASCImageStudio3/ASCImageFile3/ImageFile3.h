#pragma once
	
#include "resource.h"
#include <atlsafe.h>
#include <comdef.h>
#include <stdio.h>
#include <gdiplusex.h>
#include <graydither.h>
#include <math.h>
#include <imagestudioutils.h>
#include "pcx.h"
#include "ras.h"
#include "tga.h"
#include "j2k.h"
#include "MetaData.h"
#include <ImageIO.h>
#include <MemoryUtils.h>

#include "../Common/ImageFileFormatChecker.h"

// TODO: #define DISABLE_REGISTRATION
	
const int c_nBitmapType32bpp = 0;
const int c_nBitmapType24bpp = 1;
const int c_nBitmapType8bppLightness = 2;
const int c_nBitmapType8bppIntensity = 3;
const int c_nBitmapType8bppBrightness = 4;
const int c_nBitmapType1bpp = 5;
const int c_nBitmapType1bppDither = 6;
	
const int c_nTiffCompressionNone = 0;
const int c_nTiffCompressionLzw = 1;
const int c_nTiffCompressionRle = 2;
const int c_nTiffCompressionCCITT3 = 3;
const int c_nTiffCompressionCCITT4 = 4;
	
[object, uuid("7879A2A8-02D0-48a7-929D-1316FC6C97AA"), dual, pointer_default(unique)]
__interface IImageFile3 : IDispatch
{
	[id(11)] HRESULT LoadImage([in] BSTR Path, [out, satype("BYTE")] SAFEARRAY** Image, [out, retval] VARIANT_BOOL* Success);
	[id(12)] HRESULT LoadPicture([in] IPictureDisp* Picture, [out, satype("BYTE")] SAFEARRAY** Image, [out, retval] VARIANT_BOOL* Success);
	[id(13)] HRESULT LoadScreenshot([out, satype("BYTE")] SAFEARRAY** Image, [out, retval] VARIANT_BOOL* Success);
	[id(14)] HRESULT LoadFrame([in] BSTR Path, [in] long Frame, [out, satype("BYTE")] SAFEARRAY** Image, [out, retval] VARIANT_BOOL* Success);

	[id(20)] HRESULT GetFrameCount([in] BSTR Path, [out, retval] long* Count);

	[id(50)] HRESULT SaveAsBmp([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(51)] HRESULT SaveAsGif([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(52)] HRESULT SaveAsJpeg([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(53)] HRESULT SaveAsPng([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(54)] HRESULT SaveAsTiff([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(55)] HRESULT SaveAsWmf([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(56)] HRESULT SaveAsEmf([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(57)] HRESULT SaveAsPcx([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(58)] HRESULT SaveAsTga([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(59)] HRESULT SaveAsRas([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(60)] HRESULT SaveAsJ2k([in, satype("BYTE")] SAFEARRAY** Image, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	
	[id(80)] HRESULT SavePicture([in, satype("BYTE")] SAFEARRAY** Image, [out] IPictureDisp** Picture, [out, retval] VARIANT_BOOL* Success);
	[id(81)] HRESULT SaveFrame([in, satype("BYTE")] SAFEARRAY** Image, [in] VARIANT_BOOL LastFrame, [out, retval] VARIANT_BOOL* Success);

	[id(101), propget] HRESULT JpegQuality([out, retval] long *Quality);
	[id(101), propput] HRESULT JpegQuality([in] long Quality);
	[id(102), propget] HRESULT TiffCompression([out, retval] long *Compression);
	[id(102), propput] HRESULT TiffCompression([in] long Compression);
	[id(103), propget] HRESULT TiffMultipaged([out, retval] VARIANT_BOOL *Multipaged);
	[id(103), propput] HRESULT TiffMultipaged([in] VARIANT_BOOL Multipaged);
	[id(104), propget] HRESULT BitmapType([out, retval] long *Type);
	[id(104), propput] HRESULT BitmapType([in] long Type);

	[id(1000), propget] HRESULT InfoTitle([out, retval] BSTR* Value);
	[id(1000), propput] HRESULT InfoTitle([in] BSTR Value);
	[id(1001), propget] HRESULT InfoDescription([out, retval] BSTR* Value);
	[id(1001), propput] HRESULT InfoDescription([in] BSTR Value);
	[id(1002), propget] HRESULT InfoDateTime([out, retval] BSTR* Value);
	[id(1002), propput] HRESULT InfoDateTime([in] BSTR Value);
	[id(1003), propget] HRESULT InfoAuthor([out, retval] BSTR* Value);
	[id(1003), propput] HRESULT InfoAuthor([in] BSTR Value);
	[id(1004), propget] HRESULT InfoCopyright([out, retval] BSTR* Value);
	[id(1004), propput] HRESULT InfoCopyright([in] BSTR Value);
	[id(1005), propget] HRESULT InfoEquipmentType([out, retval] BSTR* Value);
	[id(1005), propput] HRESULT InfoEquipmentType([in] BSTR Value);
	[id(1006), propget] HRESULT InfoEquipmentModel([out, retval] BSTR* Value);
	[id(1006), propput] HRESULT InfoEquipmentModel([in] BSTR Value);
	[id(1007), propget] HRESULT InfoVerticalResolution([out, retval] double* Value);
	[id(1007), propput] HRESULT InfoVerticalResolution([in] double Value);
	[id(1008), propget] HRESULT InfoHorizontalResolution([out, retval] double* Value);
	[id(1008), propput] HRESULT InfoHorizontalResolution([in] double Value);

	// new version
	[id(10011)] HRESULT LoadImage2([in] BSTR Path, [out] IUnknown** Image, [out, retval] VARIANT_BOOL* Success);
	[id(10012)] HRESULT LoadPicture2([in] IPictureDisp* Picture, [out] IUnknown** Image, [out, retval] VARIANT_BOOL* Success);
	[id(10013)] HRESULT LoadScreenshot2([out] IUnknown** Image, [out, retval] VARIANT_BOOL* Success);
	[id(10014)] HRESULT LoadFrame2([in] BSTR Path, [in] long Frame, [out] IUnknown** Image, [out, retval] VARIANT_BOOL* Success);
	[id(10050)] HRESULT SaveImage2([in] IUnknown** Image, [in] LONG Format, [in] BSTR Path, [out, retval] VARIANT_BOOL* Success);
	[id(10080)] HRESULT SavePicture2([in] IUnknown** Image, [out] IPictureDisp** Picture, [out, retval] VARIANT_BOOL* Success);
	[id(10081)] HRESULT SaveFrame2([in] IUnknown** Image, [in] VARIANT_BOOL LastFrame, [out, retval] VARIANT_BOOL* Success);
};


[
	object,
	uuid("B67C0F8F-C9F5-4398-B10D-3A7AB31A9BC6"), dual,
	helpstring("IImageMetaData Interface"),
	pointer_default(unique)
]
__interface IImageMetaData : IDispatch
{
	[id(1), helpstring("method SetMetaData")]			HRESULT SetMetaData([in] BSTR bsMetaDataXML);
	[id(2), helpstring("method GetMetaData")]			HRESULT GetMetaData([out] BSTR * bsMetaDataXML);
	[id(3), helpstring("method MetaDataFromFile")]      HRESULT MetaDataFromFile([in] BSTR Path);

	[id(101), helpstring("method SetAdditionalParam")]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT	ParamValue);
	[id(102), helpstring("method GetAdditionalParam")]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out] VARIANT *	ParamValue);

};

[coclass, uuid("837801EA-28BC-4777-8484-C57A3BE57BAC"), threading(apartment), vi_progid("AVSImageFile3.ImageFile3"), progid("AVSImageFile3.ImageFile3.1"), version(1.0)]
class ATL_NO_VTABLE ImageFile3 : public IImageFile3, public IImageMetaData
{
protected:

	CGdiPlusInit m_oGdiPlusInit;

	Bitmap* m_bitmap;

	int     m_nMultipagedFormat; // 0 - Tiff, 1 - GIF или APNG
	double  m_dAnimationDuration; //в миллисекундах
	long  m_nAnimationWidth;
	long  m_nAnimationHeight;
	bool  m_bAnimationGifWithAlpha;

	long m_JpegQuality;
	long m_BitmapType;
	long m_TiffCompression;
	VARIANT_BOOL m_TiffMultipaged;
	VARIANT_BOOL m_UpdateSize;

	BYTE* m_backup;
	BYTE* m_palette;

	CString  m_strInfoXML;
	// information
	CString m_InfoTitle;
	CString m_InfoDescription; // Comment
	CString m_InfoDateTime;
	CString m_InfoAuthor;
	CString m_InfoCopyright;
	CString m_InfoEquipmentType;
	CString m_InfoEquipmentModel;

	CString m_InfoJobTime;
	CString m_InfoJobName;
	CString m_InfoSofftwareID;
	CString m_InfoSoftwareVersion;
	CString m_InfoHostComputer;
	CString m_InfoWarning;

	double m_InfoVerticalResolution;
	double m_InfoHorizontalResolution;
	double m_InfoVerticalResolutionStorage;
	double m_InfoHorizontalResolutionStorage;
	short m_InfoResolutionUnit;
	short m_InfoResolutionUnitX;
	short m_InfoResolutionUnitY;
	short m_InfoOrientation;
	short m_InfoImageWidth;
	short m_InfoImageHeight;

	MemoryUtils::CMemoryUtils m_oMemoryUtils;

	CMetaData m_oMetaData;
	
protected:
	
	ImageFile3()
	{
		m_oGdiPlusInit.Init();

		// setup default values
		m_bitmap = 0;

		m_nMultipagedFormat = 0;
		m_dAnimationDuration = 1000;
		m_nAnimationWidth = -1;
		m_nAnimationHeight = -1;
		m_bAnimationGifWithAlpha = false;
		m_JpegQuality = 75;
		m_BitmapType = c_nBitmapType24bpp;
		m_TiffCompression = 0;
		m_TiffMultipaged = VARIANT_FALSE;
		m_UpdateSize = VARIANT_FALSE;

		m_backup = 0;
		m_palette = new BYTE[258*4];

		// setup default information
		m_InfoTitle = _T("");
		m_InfoDescription = _T("");
		m_InfoDateTime = _T("");
		m_InfoAuthor = _T("");
		m_InfoCopyright = _T("");
		m_InfoEquipmentType = _T("");
		m_InfoEquipmentModel = _T("");
		m_InfoVerticalResolution = -1;
		m_InfoHorizontalResolution = -1;
		m_InfoResolutionUnit = 2;
		m_InfoResolutionUnitX = 1;
		m_InfoResolutionUnitY = 1;
		m_InfoOrientation = 1;

		m_strInfoXML = _T("");
	}
	~ImageFile3()
	{
		// delete existing memory
		ClearBitmap();

		ClearBackup();

		if (m_palette)
		{
			delete[] m_palette;
			m_palette = 0;
		}
	}

	void ClearBackup()
	{
		if (m_backup)
		{
			delete[] m_backup;
			m_backup = 0;
		}
	}
	void ClearBitmap()
	{
		if (m_bitmap)
		{
			delete m_bitmap;
			m_bitmap = 0;
		}
	}

	void ArrayToImage(void** Image, Bitmap** pBitmap, int nType = c_nBitmapType32bpp)
	{
		SAFEARRAY** pArray = (SAFEARRAY**) Image;

		// check for valid input parameters
		if (!pArray || !pBitmap)
			throw 0;

		// attach ATL wrapper
		CComSafeArray<BYTE> safeArray; safeArray.Attach(*pArray);

		// check for valid pixel array size
		if (safeArray.GetDimensions() != 3 || safeArray.GetType() != VT_UI1)
		{
			safeArray.Detach();
			throw 0;
		}

		// retrieve image attributes
		long nChannels = safeArray.GetCount(0);
		long nWidth = safeArray.GetCount(1);
		long nHeight = safeArray.GetCount(2);
		BYTE* pSource = (BYTE*)(safeArray.m_psa->pvData);

		// copy memory to image
		if (!MemoryToImage(pSource, pBitmap, nType, nWidth, nHeight, nChannels))
		{
			safeArray.Detach();
			throw 0;
		}

		// detach the array
		safeArray.Detach();
	}
	void InterfaceToImage(void** Image, Bitmap** pBitmap, int nType = c_nBitmapType32bpp)
	{
		IUnknown** pInterface = (IUnknown**)Image;

		// check for valid input parameters
		if (!pInterface || !*pInterface)
			throw 0;

		int nWidth = 0;
		int nHeight = 0;
		BYTE* pPixels = 0;

		MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;
		if (!ImageStudioUtils::MediaDataToByteArray(pInterface, pMediaData, pPixels, nWidth, nHeight))
			throw 0;

		Bitmap* pGdiImage = NULL;

		if (!MemoryToImage(pPixels, &pGdiImage/*pBitmap*/, nType, nWidth, nHeight, 4))
		{
			pMediaData->Release();
			throw 0;
		}

		long nAspectX = pMediaData->GetAspectRatioX();
		long nAspectY = pMediaData->GetAspectRatioY();
		long nNewWidth = nWidth;
		long nNewHeight = nWidth * nAspectY / nAspectX;

		*pBitmap = new Bitmap( nNewWidth, nNewHeight, PixelFormat32bppARGB );
		if( Gdiplus::Ok == (*pBitmap)->GetLastStatus() )
		{
			Gdiplus::Graphics oRender( *pBitmap );
			if( Gdiplus::Ok == oRender.GetLastStatus() )
			{
				oRender.DrawImage( pGdiImage, Rect(0, 0, nNewWidth, nNewHeight), 0, 0, nWidth, nHeight, Gdiplus::Unit::UnitPixel );
				
				delete pGdiImage;
				pMediaData->Release();
				
				return;
			}
			delete pBitmap;
		}

		*pBitmap = pGdiImage;

		pMediaData->Release();
	}
	BOOL MemoryToImage(BYTE* pSource, Bitmap** pBitmap, int nType, long nWidth, long nHeight, long nChannels)
	{
		// check for valid input parameters
		if (!pSource || !pBitmap)
			return FALSE;

		// check for vlaid image size
		if (nChannels != 4 || nWidth < 1 || nHeight < 1)
			return FALSE;

		// clear existing memory
		ClearBackup();

		// create output image
		if (nType == c_nBitmapType32bpp)
		{
			m_backup = new BYTE[4*nWidth*nHeight]; ZeroMemory(m_backup, 4*nWidth*nHeight);

			memcpy(m_backup, pSource, 4*nWidth*nHeight);

			(*pBitmap) = new Bitmap(nWidth, nHeight, nWidth * 4, PixelFormat32bppARGB, m_backup);
		}
		else if (nType == c_nBitmapType24bpp)
		{
			int nLineSize = ImageStudioUtils::WidthBytes(24*nWidth);

			m_backup = new BYTE[nLineSize*nHeight]; ZeroMemory(m_backup, nLineSize*nHeight);

			for (int nY = 0; nY < nHeight; ++nY)
			{
				// reset bit index and bits
				BYTE* pResult = m_backup + nLineSize*nY;
				int nIndex = nY*4*nWidth;

				// convert all colors to 1-bit values
				for (int nX = 0; nX < nWidth; ++nX, nIndex += 4, pResult += 3)
				{
					pResult[2] = pSource[nIndex + 2];
					pResult[1] = pSource[nIndex + 1];
					pResult[0] = pSource[nIndex + 0];
				}
			}

			(*pBitmap) = new Bitmap(nWidth, nHeight, nLineSize, PixelFormat24bppRGB, m_backup);
		}
		else if (nType == c_nBitmapType8bppLightness || nType == c_nBitmapType8bppBrightness || nType == c_nBitmapType8bppIntensity)
		{
			int nLineSize = ImageStudioUtils::WidthBytes(8*nWidth);

			m_backup = new BYTE[nLineSize*nHeight]; ZeroMemory(m_backup, nLineSize*nHeight);

			for (int nY = 0; nY < nHeight; ++nY)
			{
				// reset bit index and bits
				BYTE* pResult = m_backup + nLineSize*nY;
				int nIndex = nY*4*nWidth;

				// convert all colors to 1-bit values
				for (int nX = 0; nX < nWidth; ++nX, nIndex += 4, pResult++)
				{
					if (nType == c_nBitmapType8bppIntensity)
						*pResult = ImageStudioUtils::GetIntensity(pSource[nIndex + 2], pSource[nIndex + 1], pSource[nIndex + 0]);
					else if (nType == c_nBitmapType8bppLightness)
						*pResult = ImageStudioUtils::GetLightness(pSource[nIndex + 2], pSource[nIndex + 1], pSource[nIndex + 0]);
					else if (nType == c_nBitmapType8bppBrightness)
						*pResult = ImageStudioUtils::GetBrightness(pSource[nIndex + 2], pSource[nIndex + 1], pSource[nIndex + 0]);
				}
			}

			// create image
			(*pBitmap) = new Bitmap(nWidth, nHeight, nLineSize, PixelFormat8bppIndexed, m_backup);

			// variables
			UINT* pPalette = (UINT*)m_palette;

			// compose grayscale palette
			*pPalette = PaletteFlagsGrayScale; pPalette++;
			*pPalette = 256; pPalette++;
			for (int nIndex = 0; nIndex < 255; ++nIndex, ++pPalette)
				*pPalette = RGB(nIndex, nIndex, nIndex);

			// set the palette to the image
			(*pBitmap)->SetPalette((ColorPalette*)m_palette);
		}
		else
		{
			// dither image if necessary
			if (nType == c_nBitmapType1bppDither)
				Dither32Image(pSource, nWidth, nHeight);

			int nLineSize = ImageStudioUtils::WidthBytes(nWidth);

			m_backup = new BYTE[nLineSize*nHeight]; ZeroMemory(m_backup, nLineSize*nHeight);

			// fill the memory
			for (int nY = 0; nY < nHeight; ++nY)
			{
				// reset bit index and bits
				int nBitIndex = 0;
				BYTE* pResult = m_backup + nLineSize*nY;
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

			(*pBitmap) = new Bitmap(nWidth, nHeight, nLineSize, PixelFormat1bppIndexed, m_backup);
		}

		if (!(*pBitmap) || (*pBitmap)->GetLastStatus() != Ok)
			return FALSE;

		if ((*pBitmap)->RotateFlip(RotateNoneFlipY) != Ok)
			return FALSE;

		if ((*pBitmap)->GetLastStatus() != Ok)
			return FALSE;

		return TRUE;
	}
	void GetEncoderClsid(const wchar_t* pFormat, CLSID* pClsid)
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
	void CaptureScreen(BYTE*& pixels, Bitmap*& bitmap)
	{
		// check for valid object
		if (pixels || bitmap)
			return;

		// variables
		HDC hdcScreen = CreateDC(_T("DISPLAY"), NULL, NULL, NULL); 
		HDC hdcCompatible = CreateCompatibleDC(hdcScreen); 

		// compute screen size
		int nWidth = GetDeviceCaps(hdcScreen, HORZRES);
		int nHeight = GetDeviceCaps(hdcScreen, VERTRES);

		// create a compatible bitmap for hdcScreen. 
		HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight); 

		// check for valid bitmap
		if (hbmScreen == 0) 
			return;

		// select the bitmaps into the compatible DC
		HGDIOBJ hObject = SelectObject(hdcCompatible, hbmScreen);

		// check for valid selected bitmap
		if (!hObject) 
			return; 
	 
		// copy color data for the entire display into a bitmap that is selected into a compatible DC.
		if (BitBlt(hdcCompatible, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY)) 
		{
			// create pixel array
			pixels = new BYTE[4*nWidth*nHeight];

			// compose bitmap info
			BITMAPINFO info;
			info.bmiHeader.biBitCount = 32;
			info.bmiHeader.biClrImportant = 0;
			info.bmiHeader.biClrUsed = 0;
			info.bmiHeader.biCompression = BI_RGB;
			info.bmiHeader.biHeight = -nHeight;
			info.bmiHeader.biPlanes = 1;
			info.bmiHeader.biSize = sizeof(BITMAPINFO);
			info.bmiHeader.biSizeImage = 4*nWidth*nHeight;
			info.bmiHeader.biWidth = nWidth;
			info.bmiHeader.biXPelsPerMeter = 0;
			info.bmiHeader.biYPelsPerMeter = 0;

			// extract pixels from HBITMAP
			if (::GetDIBits(hdcCompatible, hbmScreen, 0, nHeight, pixels, &info, DIB_RGB_COLORS) != nHeight)
			{
				delete[] pixels;
				pixels = 0;
			}

			// compose resulting image
			if (pixels)
				bitmap = new Bitmap(nWidth, nHeight, nWidth*4, PixelFormat32bppRGB, pixels);
		}

		// restore previous object
		SelectObject(hdcCompatible, hObject);

		// delete objects
		DeleteObject(hbmScreen);
		DeleteDC(hdcCompatible);
		DeleteDC(hdcScreen);
	}
	void UpdateSize(Bitmap*& pImage)
	{
		// check whether we need to correct size
		if (m_UpdateSize != VARIANT_TRUE)
			return;

		// retrieve current resolution values
		double dOldHRes = pImage->GetHorizontalResolution();
		double dOldVRes = pImage->GetVerticalResolution();

		// check for the same resolution values
		if (fabs(dOldHRes - dOldVRes) < 0.001)
			return;

		// check for zero values
		if (fabs(dOldHRes) < 0.001 || fabs(dOldVRes) < 0.001)
			return;

		// compute new resolution values
		double dNewRes = max(dOldHRes, dOldVRes);

		// retrieve current dimension values
		int nOldW = pImage->GetWidth();
		int nOldH = pImage->GetHeight();

		// compute new sizes
		int nNewW = (int)(nOldW*dNewRes/dOldHRes);
		int nNewH = (int)(nOldH*dNewRes/dOldVRes);

		// retrieve thumbnail image
		Bitmap* pNewImage = (Bitmap*)pImage->GetThumbnailImage(nNewW, nNewH);

		// check for valid image
		if (!pNewImage)
			return;

		pNewImage->SetResolution((REAL)dNewRes, (REAL)dNewRes);

		// remove old image
		delete pImage;
		
		// update image
		pImage = pNewImage;
	}
	void UpdateProperties(Bitmap*& bitmap, BOOL bLoad)
	{
		if (bLoad)
		{
			// retrieve info properties
			// ...
			// retrieve bitmap resolution
			m_InfoHorizontalResolution = bitmap->GetHorizontalResolution();
			m_InfoVerticalResolution = bitmap->GetVerticalResolution();

		}
		else
		{
			// set the bitmap resolution in dpi
			bitmap->SetResolution((REAL)m_InfoHorizontalResolution, (REAL)m_InfoVerticalResolution);

			// save info properties
			SetPropertyShort(bitmap, PropertyTagOrientation, m_InfoOrientation);
			SetPropertyShort(bitmap, PropertyTagImageWidth, m_InfoImageWidth);
			
		}

		// remove property load-save errors here
		if (bitmap->GetLastStatus() != Ok)
			return;
	}
	void GetPropertyShort(Bitmap*& pImage, PROPID nId, int& nValue)
	{
		// reset value
		nValue = -1;

		// retrieve property size
		UINT nSize = pImage->GetPropertyItemSize(nId);

		// check for valid size
		if (nSize > 0)
		{
			// allocate memory for tag
			PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
			if ( NULL == pProp )
				return;

			// read tag to buffer
			pImage->GetPropertyItem(nId, nSize, pProp);

			// copy data to buffer
			if ( NULL != pProp->value )
				nValue = *(short*)pProp->value;
			
			// release buffer
			free(pProp);
		}
	}
	void GetPropertyInt(Bitmap*& pImage, PROPID nId, int& nValue)
	{
		// reset value
		nValue = -1;

		// retrieve property size
		UINT nSize = pImage->GetPropertyItemSize(nId);

		// check for valid size
		if (nSize > 0)
		{
			// allocate memory for tag
			PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
			if ( NULL == pProp )
				return;

			// read tag to buffer
			pImage->GetPropertyItem(nId, nSize, pProp);

			// copy data to buffer
			if ( NULL != pProp->value )
				nValue = *(int*)pProp->value;
			
			// release buffer
			free(pProp);
		}
	}
	void GetPropertyStr(Bitmap*& pImage, PROPID nId, CString& sValue)
	{
		// reset value
		sValue = "";

		// retrieve property size
		UINT nSize = pImage->GetPropertyItemSize(nId);

		// check for valid size
		if (nSize > 0)
		{
			// allocate memory for tag
			PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
			if ( NULL == pProp )
				return;

			// read tag to buffer
			pImage->GetPropertyItem(nId, nSize, pProp);

			// copy data to buffer
			if ( NULL != pProp->value )
				sValue = (const char*)pProp->value;
			
			// release buffer
			free(pProp);
		}
	}
	void GetPropertyRational(Bitmap*& pImage, PROPID nId, double& dValue)
	{
		// reset value
		dValue = 0;

		// retrieve property size
		UINT nSize = pImage->GetPropertyItemSize(nId);

		// check for valid size
		if (nSize > 0)
		{
			// allocate memory for tag
			PropertyItem* pProp = (PropertyItem*)(malloc(nSize));
			if ( NULL == pProp )
				return;

			// read tag to buffer
			pImage->GetPropertyItem(nId, nSize, pProp);

			long *values;
			// compute number of values
			if ( NULL != pProp->value )
				values = (long*)pProp->value;
			else
			{
				free(pProp);
				return;
			}
			int count = pProp->length/sizeof(double);

			// compute value
			for (int index = 0; index < count; ++index)
				dValue += values[2*index + 0]/(double)values[2*index + 1];

			// release buffer
			free(pProp);
		}
	}
	void SetPropertyStr(Bitmap*& pImage, PROPID nId, CString& sValue)
	{
		// check for valid string
		if (sValue.GetLength() < 1)
			return;

		// create new property item
		PropertyItem* pProp = new PropertyItem;
		if ( NULL == pProp )
			return;

		// compose property item
		pProp->id = nId;
		pProp->length = (nId == PropertyTagDateTime) ? 20 : sValue.GetLength() + 1;
		pProp->type = PropertyTagTypeASCII; 
		pProp->value = (TCHAR*)(LPCTSTR)sValue;

		// sset property item
		pImage->SetPropertyItem(pProp);

		// delete property item
		delete pProp;
	}
	void SetPropertyShort(Bitmap*& pImage, PROPID nId, short& nValue)
	{
		// create new property item
		PropertyItem* pProp = new PropertyItem;
		if ( NULL == pProp )
			return;

		// compose property item
		pProp->id = nId;
		pProp->length = sizeof(short);
		pProp->type = PropertyTagTypeShort; 
		pProp->value = (short*)(&nValue);

		// sset property item
		pImage->SetPropertyItem(pProp);

		// delete property item
		delete pProp;
	}
	void SetPropertyRational(Bitmap*& pImage, PROPID nId, double& dValue)
	{
		// check for valid string
		if (dValue < 0)
			return;

		// create new property item
		PropertyItem* pProp = new PropertyItem;
		if ( NULL == pProp )
			return;

		// compose property item
		pProp->id = nId;
		pProp->length = sizeof(double);
		pProp->type = PropertyTagTypeRational; 
		pProp->value = (double*)&dValue;

		// sset property item
		pImage->SetPropertyItem(pProp);

		// delete property item
		delete pProp;
	}
	Bitmap* LoadCursor(BSTR Path)
	{
		USES_CONVERSION;

		// load cursor from file
		HCURSOR hCursor = ::LoadCursorFromFile(OLE2T(Path));

		// check handle
		if (!hCursor)
			return 0;		// invalid cursor file

		// retrive cursor information
		ICONINFO infoCursor; ::ZeroMemory(&infoCursor, sizeof(ICONINFO));
		if (!::GetIconInfo(hCursor, &infoCursor) || !infoCursor.hbmMask)
			return 0;		// can't read cursor information

		// retrive cursor size
		Bitmap bmpCursor(infoCursor.hbmMask, 0);
		CSize sizeCursor(bmpCursor.GetWidth(), bmpCursor.GetHeight());

		// create Bitmap object
		Bitmap* bitmap = new Bitmap(sizeCursor.cx, sizeCursor.cy, PixelFormat32bppRGB);
		if (!bitmap || bitmap->GetLastStatus() != Ok)
			return 0;

		// create Graphics object for cursor drawing
		Graphics graphics(bitmap); graphics.Clear(Color::White);
		if (graphics.GetLastStatus() != Ok)
			return 0;

		// retrive device context
		HDC graphicsDC = graphics.GetHDC();
		if (graphics.GetLastStatus() != Ok)
			return 0;

		// draw cursor
		if (!::DrawIcon(graphicsDC, 0, 0, hCursor))
			return 0;

		// release device context
		graphics.ReleaseHDC(graphicsDC);
		if (graphics.GetLastStatus() != Ok)
			return 0;

		// destroy cursor handle
		::DestroyCursor(hCursor);

		// all ok-na
		return bitmap;
	}
	
protected:

	static BOOL ImageToMemory(Bitmap* pBitmap, BYTE* pPixels)
	{
		// pre-flip the image		
		BOOL Flipped = (pBitmap->RotateFlip(RotateNoneFlipY) == Ok);

		// create locking settings
		Rect rectLock(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
		BitmapData* pBitmapData = new BitmapData();

		// variables
		BOOL Success = FALSE;
		int nX, nY, nIndex = 0;
		int nOffset = 0;

		// try to process 24bpp image first

		// lock the bitmap bits
		if ((pBitmap->GetPixelFormat() == PixelFormat1bppIndexed ||
			pBitmap->GetPixelFormat() == PixelFormat4bppIndexed  ||
			pBitmap->GetPixelFormat() == PixelFormat16bppGrayScale ||
			pBitmap->GetPixelFormat() == PixelFormat16bppRGB555 ||
			pBitmap->GetPixelFormat() == PixelFormat16bppRGB565 ||
			pBitmap->GetPixelFormat() == PixelFormat24bppRGB ||
			pBitmap->GetPixelFormat() == PixelFormat32bppRGB ) &&
			pBitmap->LockBits(&rectLock, ImageLockModeRead, PixelFormat24bppRGB, pBitmapData) == Ok)
		{
			// compute stride value
			int nStride = pBitmapData->Stride; if (!Flipped) nStride *= -1;

			// retrieve pointer to the locked bits
			BYTE* pBitmapPixels = (BYTE*)pBitmapData->Scan0;

			int nWidth  = pBitmap->GetWidth();
			int nHeight = pBitmap->GetHeight();

			// copy pixel info
			for (nY = 0; nY < nHeight; ++nY)
			{
				nOffset = (nStride < 0) ? (nHeight - 1 - nY)*(-nStride) : nY*nStride;

				for (nX = 0; nX < nWidth; ++nX, nIndex += 4, nOffset += 3)
				{
					pPixels[nIndex + 0] = pBitmapPixels[nOffset + 0];
					pPixels[nIndex + 1] = pBitmapPixels[nOffset + 1];
					pPixels[nIndex + 2] = pBitmapPixels[nOffset + 2];
					pPixels[nIndex + 3] = 255;
				}
			}

			// unlock the bits
			pBitmap->UnlockBits(pBitmapData);

			// update output flag
			Success = TRUE;
		}
		else
		{
			// lock the bitmap bits
			if (pBitmap->LockBits(&rectLock, ImageLockModeRead, PixelFormat32bppARGB, pBitmapData) == Ok)
			{
				// compute stride value
				int nStride = pBitmapData->Stride; if (!Flipped) nStride *= -1;

				// retrieve pointer to the locked bits
				BYTE* pBitmapPixels = (BYTE*)pBitmapData->Scan0;

				int nWidth  = pBitmap->GetWidth();
				int nHeight = pBitmap->GetHeight();

				// copy pixel info
				for (nY = 0; nY < nHeight; ++nY)
				{
					nOffset = (nStride < 0) ? (nHeight - 1 - nY)*(-nStride) : nY*nStride;

					for (nX = 0; nX < nWidth; ++nX, nIndex += 4, nOffset += 4)
					{
						pPixels[nIndex + 0] = pBitmapPixels[nOffset + 0];
						pPixels[nIndex + 1] = pBitmapPixels[nOffset + 1];
						pPixels[nIndex + 2] = pBitmapPixels[nOffset + 2];
						pPixels[nIndex + 3] = pBitmapPixels[nOffset + 3];
					}
				}

				// unlock the bits
				pBitmap->UnlockBits(pBitmapData);

				// update output flag
				Success = TRUE;
			}
		}

		// delete unnecessary bitmap data pointer
		if (pBitmapData)
			delete pBitmapData;

		// raise error (if necessary)
		return Success;
	}
	static BOOL ImageToArray(Bitmap* pBitmap, void** Image)
	{
		SAFEARRAY** pArray = (SAFEARRAY**) Image;

		// check for valid input parameters
		if (!pBitmap || !pArray)
			return FALSE;

		// destroy array (if non empty)
		ImageStudioUtils::DestroySafeArray(pArray);

		// compute SAFEARRAY bounds
		SAFEARRAYBOUND rgsaBound[3];
			rgsaBound[0].lLbound = 0;
			rgsaBound[0].cElements = 4;
			rgsaBound[1].lLbound = 0;
			rgsaBound[1].cElements = pBitmap->GetWidth();
			rgsaBound[2].lLbound = 0;
			rgsaBound[2].cElements = pBitmap->GetHeight();

		// compose SAFEARRAY with image data
		*pArray = SafeArrayCreate(VT_UI1, 3, rgsaBound);

		// check for valid array
		if (!*pArray)
			return FALSE;

		// retrieve pointer to a pixel data
		BYTE* pPixels = (BYTE*)(*pArray)->pvData;

		return ImageToMemory(pBitmap, pPixels);
	}
	static BOOL PrepareInterface(IUnknown** pInterface, int nWidth, int nHeight, int nChannels, BYTE*& pBufferPtr)
	{
		if (!pInterface)
			return FALSE;

		// create result interface
		*pInterface = NULL;
		HRESULT hRes = CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)pInterface);
		if (hRes != S_OK || NULL == *pInterface)
			return FALSE;
		MediaCore::IAVSUncompressedVideoFrame* pMediaData = (MediaCore::IAVSUncompressedVideoFrame*)*pInterface;
		//(*pInterface)->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
		//if (NULL == pMediaData)
		//	return FALSE;

		// compute safearray's buffer size
		int nBufferSize = nChannels*nWidth*nHeight;
		if (nBufferSize < 1)
			return FALSE;

		// allocate necesasry buffer
		pMediaData->put_ColorSpace(CSP_BGRA | CSP_VFLIP);
		pMediaData->put_Width(nWidth);
		pMediaData->put_Height(nHeight);
		pMediaData->put_AspectRatioX(nWidth);
		pMediaData->put_AspectRatioY(nHeight);
		pMediaData->put_Interlaced(VARIANT_FALSE);
		pMediaData->put_Stride(0, 4*nWidth);
		pMediaData->AllocateBuffer(nBufferSize);
		
		pBufferPtr = 0;
		long nCreatedBufferSize = 0;
		pMediaData->get_Buffer(&pBufferPtr);
		pMediaData->get_BufferSize(&nCreatedBufferSize);
		pMediaData->put_Plane(0, pBufferPtr);

		// check for valid created buffer
		if (!pBufferPtr || nCreatedBufferSize != nBufferSize)
			return FALSE;

		// save interface
		*pInterface = pMediaData;

		return TRUE;
	}
	static BOOL ImageToInterface(Bitmap* pBitmap, void** Image)
	{
		IUnknown** pInterface = (IUnknown**) Image;

		// check for valid input parameters
		if (!pBitmap)
			return FALSE;

		// create result interface
		BYTE* pBufferPtr = 0;
		if (!PrepareInterface(pInterface, pBitmap->GetWidth(), pBitmap->GetHeight(), 4, pBufferPtr))
			return FALSE;

		// copy data to the buffer
		return ImageToMemory(pBitmap, pBufferPtr);
	}
	static BOOL CopyToInterface(SAFEARRAY* psarr, void** Image)
	{
		IUnknown** pInterface = (IUnknown**) Image;

		// check for valid input parameters
		if (!psarr || !pInterface)
			return FALSE;

		// copy psarr to Interface
		if (ImageStudioUtils::SafeArrayToMediaData(psarr, pInterface, FALSE))
			return TRUE;

		return FALSE;
	}
	static BOOL CopyToArray(SAFEARRAY* psarr, void** Image)
	{
		SAFEARRAY** pArray = (SAFEARRAY**) Image;

		// check for valid input parameters
		if (!psarr || !pArray)
			return FALSE;

		// destroy array (if non empty)
		ImageStudioUtils::DestroySafeArray(pArray);

		// copy psarr to pArray
		if (SafeArrayCopy(psarr, pArray) != S_OK)
			return TRUE;

		return FALSE;
	}
	static BYTE* ExtractImageFromInterface(IUnknown** pInterface, MediaCore::IAVSUncompressedVideoFrame*& pMediaData, int& nWidth, int& nHeight)
	{
		nWidth = 0;
		nHeight = 0;
		BYTE* pPixels = 0;

		if (!ImageStudioUtils::MediaDataToByteArray(pInterface, pMediaData, pPixels, nWidth, nHeight))
			throw 0;

		return pPixels;
	}
	BOOL LoadImageTo(_ImageFileFormats eFileType, BSTR Path, ImageStudio::Core::Image & oImage,BOOL & bVerFlip, BOOL bArray)
	{
		BOOL bLoading = FALSE;

		CW2T convert(Path);
		CString sFilePath( convert );


		int nFormat = ImageStudio::IO::c_nFormat_Uncompressed_32bpp;

		try
		{
			if(eFileType == c_Gdiplus 
			|| eFileType == c_Bmp
			|| eFileType == c_Gif 
			|| eFileType == c_Jpg 
			|| eFileType == c_Png 
			|| eFileType == c_Tiff)
			{
				ImageStudio::IO::Bitmap oReaderBmp;
				bLoading = oReaderBmp.FromFile( sFilePath, nFormat, &oImage, 0/*nFrame*/, &m_InfoHorizontalResolution, &m_InfoVerticalResolution  );
			}
			else
				if( eFileType ==_ImageFileFormats::c_Psd )
			{
				ImageStudio::IO::Psd oReaderPsd;
				bLoading = oReaderPsd.FromFile( sFilePath, nFormat, &oImage );
			}
			else
				if( eFileType ==_ImageFileFormats::c_Tga)
			{
				ImageStudio::IO::Tga oReaderTga;
				bLoading = oReaderTga.FromFile( sFilePath, nFormat, &oImage );
			}
			else
				if( eFileType ==_ImageFileFormats::c_Wdp)
			{
				ImageStudio::IO::Wdp oReaderWdp;
				bLoading = oReaderWdp.FromFile( sFilePath, nFormat, &oImage );
			}
			else
				if( eFileType ==_ImageFileFormats::c_Pcx)
			{
				ImageStudio::IO::Pcx oReaderPcx;
				bLoading = oReaderPcx.FromFile( sFilePath, nFormat, &oImage );
			}
			else
				if( eFileType ==_ImageFileFormats::c_Ras)
			{
				ImageStudio::IO::Ras oReaderRas;
				bLoading = oReaderRas.FromFile( sFilePath, nFormat, &oImage );
			}
			else
				if( eFileType ==_ImageFileFormats::c_Jpeg2000)
			{
				ImageStudio::IO::Jp2 oReaderJp2;
				bLoading = oReaderJp2.FromFile( sFilePath, nFormat, &oImage );
				bVerFlip = !(bVerFlip);
			}
			else
				if( eFileType ==_ImageFileFormats::c_WebShots)
			{
				ImageStudio::IO::WebShots::Wb1 oReaderWB1;
				bLoading = oReaderWB1.FromFile( sFilePath, nFormat, &oImage );
			}
			else
				if( eFileType ==_ImageFileFormats::c_iPode)
			{
				ImageStudio::IO::Ithmb oReaderIPod;
				bLoading = oReaderIPod.FromFile( sFilePath, nFormat, &oImage );
			}
			else
				if (eFileType == c_Metafile)
			{
				ImageStudio::IO::Meta oReaderMeta;
				bLoading = oReaderMeta.FromFile( sFilePath, nFormat, &oImage );
				bVerFlip = !(bVerFlip);
			}
			else
				if(	eFileType ==_ImageFileFormats::c_RawFotos)
			{
#ifndef NO_RAW_CHECKER
			const TCHAR * settingsXML = _T("<ImageRaw3 mode=\"0\" xmlns=\"http://tempuri.org/ImageRaw3Options.xsd\"> \
					<SimpleMode ver=\"1\"><CameraWhiteBalance use=\"1\"/><CameraMatrix use=\"0\"/> \
					</SimpleMode></ImageRaw3>");

				ImageStudio::IO::Raw oRenderRaw;
				bLoading = oRenderRaw.FromFile( sFilePath, nFormat, &oImage, settingsXML);
				bVerFlip = !(bVerFlip);
#else
				bLoading = FALSE;
#endif
			}
			else
				if( eFileType ==_ImageFileFormats::c_Sfw)
			{
				#ifndef NO_SWF_CHECKER
				ImageStudio::IO::Sfw oRenderSfw;
				bLoading = oRenderSfw.FromFile( sFilePath, nFormat, &oImage );
				bVerFlip = !(bVerFlip);
#else
				bLoading = FALSE;
#endif
			}
			else
				if( eFileType ==_ImageFileFormats::c_Swf)
			{
#ifndef NO_SWF_CHECKER
				ImageStudio::IO::Swf oRenderSwf;
				bLoading = oRenderSwf.FromFile( sFilePath, nFormat, &oImage );
#else
				bLoading = FALSE;
#endif
			}
			else
				if( eFileType ==_ImageFileFormats::c_Svm)
			{
				ImageStudio::IO::Svm oRenderSvm;
				bLoading = oRenderSvm.FromFile( sFilePath, nFormat, &oImage );
			}
				else
				if( eFileType ==_ImageFileFormats::c_Wbz )
			{
				ImageStudio::IO::WebShots::Wbz oReaderWbz;
				bLoading = oReaderWbz.FromFile( sFilePath, nFormat, &oImage );
			}
			else
				if( eFileType ==_ImageFileFormats::c_Wbc)
			{
				ImageStudio::IO::WebShots::Wbc oReaderWbc;
				bLoading = oReaderWbc.FromFile( sFilePath, nFormat, &oImage );
			}
		}
		catch(...)
		{
			return FALSE;
		}
		
		return bLoading;
	}
	VARIANT_BOOL LoadImageTo(BSTR Path, void** pData, BOOL (*ImageTo)(Bitmap*, void**), BOOL bArray)
	{
		// reset output value
		VARIANT_BOOL Success = VARIANT_FALSE;

		try 
		{
			MetaDataFromFile( Path );		// load metadata from file			
			CStringW wsFilePath( Path );

			const int c_nCountFileFormats = sizeof(tableSupportImageFormats) / sizeof(tableSupportImageFormats[0]);
			
			_ImageFileFormats eFileType = tableSupportImageFormats[0].format;

			int nPos = wsFilePath.ReverseFind( L'.' );
			if( nPos > 0 )
			{
				CStringW sFileEx = wsFilePath.Right( wsFilePath.GetLength() - nPos - 1 );
				sFileEx.MakeLower();

				for( int i = 1; i < c_nCountFileFormats; i++ )
				{
					if( sFileEx == tableSupportImageFormats[i].str )
					{
						eFileType = tableSupportImageFormats[i].format;
						break;
					}
				}
			}
			CImageFileFormatChecker checker(Path);

			BOOL bLoading = FALSE;
			ImageStudio::Core::Image oImage;
			BOOL bVerFlip = FALSE;

			if (checker.eFileType>0 && checker.eFileType == eFileType)
			{
				bLoading = LoadImageTo(eFileType,Path,oImage,bVerFlip,bArray);
			}
			else if (checker.eFileType || eFileType)
			{
				//неоднозначные ситуации не совпадения sync-word с заданным расширением
				if (eFileType)
				{
					bLoading = LoadImageTo(eFileType,Path,oImage,bVerFlip,bArray); //попытка открыть по расширению
				}

				if (!bLoading && checker.eFileType)
				{
					bLoading = LoadImageTo(checker.eFileType,Path,oImage,bVerFlip,bArray); //попытка открыть по сигнатуре
				}
			}
			
			if( oImage.IsValid() )
			{
				int nWidth  = oImage.GetWidth();
				int nHeight = oImage.GetHeight();
				
				if( nWidth * nHeight > 100000000 )
				{
					oImage.Destroy();
					Success = VARIANT_FALSE;
					return Success;
				}

				if( bArray )
					bLoading = oImage.ToSafeArray((SAFEARRAY**)pData);
				else
					bLoading = oImage.ToMediaData((IUnknown**)pData, bVerFlip);
			
				return bLoading ? VARIANT_TRUE : VARIANT_FALSE;
			}
			else
			{
				Gdiplus::Bitmap* bitmap = LoadCursor(Path);

				// check for valid bitmap
				if( bitmap )
				{
					if( bitmap->GetLastStatus() == Gdiplus::Ok )
					{
						if( ImageTo( bitmap, pData ) )
						{
							Success = VARIANT_TRUE;
						}
					}
					
					delete bitmap;
				}
			}
		}
		catch(...)
		{
			// update status flag
			Success = VARIANT_FALSE;
		}
		return Success;
	}
	VARIANT_BOOL LoadPictureTo(IPictureDisp* Picture, void** pData, BOOL (*ImageTo)(Bitmap*, void**))
	{
		// reset output value
		VARIANT_BOOL Success = VARIANT_FALSE;

		// convert IPictureDisp to image
		try
		{	
			// variables
			HRESULT hResult = S_OK;
			IStreamPtr pStream;

			// create memory stream
			if (SUCCEEDED(hResult = CreateStreamOnHGlobal(0, TRUE, &pStream)))
			{
				// obtain IPicture
				IPicturePtr pOlePicture = Picture;

				// save picture to stream
				if (SUCCEEDED(pOlePicture->SaveAsFile(pStream, TRUE, 0)))
				{
					// load image from file
					Bitmap bitmap(pStream);

					// check for last error
					if (bitmap.GetLastStatus() == Ok)
					{
						// retrieve image pixels
						if (ImageTo(&bitmap, pData))
						{
							// update output value
							Success = VARIANT_TRUE;
						}
					}
				}
			}
		}
		catch (...)
		{
			// update status flag
			Success = VARIANT_FALSE;
		}

		// all ok
		return Success;
	}
	VARIANT_BOOL LoadScreenshotTo(void** pData, BOOL (*ImageTo)(Bitmap*, void**))
	{
		// reset output value
		VARIANT_BOOL Success = VARIANT_TRUE;


		// Gdi+ Bitmap object
		BYTE* pixels = 0;
		Bitmap* bitmap = 0;

		// load image from screen
		try 
		{
			// attempt to get desktop screenshot
			CaptureScreen(pixels, bitmap);

			// check pointer
			if (bitmap)
			{
				// retrieve image pixels
				if (ImageTo(bitmap, pData))
				{
					// update output value
					Success = VARIANT_TRUE;
				}
			}
		}
		catch(...)
		{
			// update status flag
			Success = VARIANT_FALSE;
		}

		// free memory
		if (bitmap)
			delete bitmap;
		if (pixels)
			delete[] pixels;

		// all ok
		return Success;
	}
	VARIANT_BOOL LoadFrameTo(BSTR Path, long Frame, void** pData, BOOL (*ImageTo)(Bitmap*, void**), BOOL bArray)
	{
		// reset output value
		VARIANT_BOOL Success = VARIANT_FALSE;


		// check for valid input parameters
		if (Frame < 0)
			return VARIANT_FALSE;

		// check for simpliest case
		if (Frame == 0)
			return LoadImageTo(Path, pData, (bArray)? ImageToArray : ImageToInterface, bArray);

		// Gdi+ Bitmap object
		Bitmap* bitmap = 0;

		// load image from file
		try 
		{
			// attempt to load image with Gdi+
			bitmap = new Bitmap(Path);

			// check for valid bitmap
			if (bitmap && bitmap->GetLastStatus() == Ok)
			{
				// variables
				GUID format;
				long nFrameCount = 0;

				// retrieve image format
				bitmap->GetRawFormat(&format);

				// check for format type
				if (format == ImageFormatTIFF)
					nFrameCount = bitmap->GetFrameCount(&FrameDimensionPage);
				else if (format == ImageFormatGIF)
					nFrameCount = bitmap->GetFrameCount(&FrameDimensionTime);

				// check for frame count
				if (Frame >= 0 && Frame < nFrameCount)
				{
					// select necessary frame
					if (format == ImageFormatTIFF)
						bitmap->SelectActiveFrame(&FrameDimensionPage, Frame);
					else if (format == ImageFormatGIF)
						bitmap->SelectActiveFrame(&FrameDimensionTime, Frame);

					// update bitmap properties and size
					UpdateProperties(bitmap, TRUE);
					UpdateSize(bitmap);

					// retrieve image pixels
					if (ImageTo(bitmap, pData))
					{
						// update output value
						Success = VARIANT_TRUE;
					}
				}
			}
		}
		catch(...)
		{
			// update status flag
			Success = VARIANT_FALSE;
		}

		// free memory
		if (bitmap)
			delete bitmap;

		// all ok
		return Success;
	}
	VARIANT_BOOL SaveImageAs(BSTR Path, int nType, const wchar_t* pImageFormat, EncoderParameters* settings, void** Image, BOOL bArrayOrInterface = TRUE)
	{
		// update result flag
		VARIANT_BOOL Success = VARIANT_TRUE;

		// variables
		Bitmap* bitmap = 0;

		// save image to file
		try 
		{
			// convert array to image
			if (bArrayOrInterface)
				ArrayToImage(Image, &bitmap, nType);
			else
				InterfaceToImage(Image, &bitmap, nType);

			// locate encoder CLSID
			CLSID encoder; GetEncoderClsid(pImageFormat, &encoder);

			// save info properties

			UpdateProperties(bitmap, FALSE);

			if ( !(L"image/png" == pImageFormat) )
				m_oMetaData.SaveToBMP(bitmap);
			//else 
			//	SaveAsPNG(bitmap);


			// save image
			bitmap->Save(Path, &encoder, settings);

			// check for last error
			if (bitmap->GetLastStatus() != Ok)
				throw 0;
		}
		catch(...)
		{
			// update result flag
			Success = VARIANT_FALSE;
		}

		// clear memory
		if (L"image/tiff" == pImageFormat && m_TiffMultipaged == VARIANT_TRUE)
			m_bitmap = bitmap;
		else
			delete bitmap;

		// delete backup
		ClearBackup();

		// all ok
		return Success;
	}
	VARIANT_BOOL SaveImageAsJpeg(BSTR Path, void** Image, BOOL bArrayOrInterface = TRUE)
	{
		// create encoder settings structure
		EncoderParameters settings;

		// setup encoder settings
		settings.Count = 1;
		settings.Parameter[0].Guid = EncoderQuality;
		settings.Parameter[0].Type = EncoderParameterValueTypeLong;
		settings.Parameter[0].NumberOfValues = 1;

		// prepare quality parameter
		ULONG Quality = m_JpegQuality;

		// setup quality setting
		settings.Parameter[0].Value = &Quality;

		return SaveImageAs(Path, c_nBitmapType32bpp, L"image/jpeg", &settings, Image, bArrayOrInterface);
	}
	VARIANT_BOOL SaveImageAsTiff(BSTR Path, void** Image, BOOL bArrayOrInterface = TRUE)
	{
		// update result flag
		VARIANT_BOOL Success = VARIANT_FALSE;

		m_nMultipagedFormat = 0;

		// save image to file
		try 
		{
			// delete existing image
			ClearBitmap();

			// compute bpp value
			int nBpp = (m_TiffCompression == c_nTiffCompressionCCITT3 || m_TiffCompression == c_nTiffCompressionCCITT4 || m_TiffCompression == c_nTiffCompressionRle) ? c_nBitmapType1bppDither : c_nBitmapType32bpp;

			// setup encoder values
			ULONG Value0, Value1, Value2;

			// create encoder settings structure
			EncoderParameters* settings = (EncoderParameters*)malloc(sizeof(EncoderParameters) + 3 * sizeof(EncoderParameter));

			// update value settings
			settings->Parameter[0].Guid = EncoderColorDepth;
			settings->Parameter[0].Type = EncoderParameterValueTypeLong;
			settings->Parameter[0].NumberOfValues = 1;
			settings->Parameter[0].Value = &Value0;
			settings->Parameter[1].Guid = EncoderCompression;
			settings->Parameter[1].Type = EncoderParameterValueTypeLong;
			settings->Parameter[1].NumberOfValues = 1;
			settings->Parameter[1].Value = &Value1;
			settings->Parameter[2].Guid = EncoderSaveFlag;
			settings->Parameter[2].Type = EncoderParameterValueTypeLong;
			settings->Parameter[2].NumberOfValues = 1;
			settings->Parameter[2].Value = &Value2;

			// update values
			Value0 = (nBpp == c_nBitmapType1bppDither || nBpp == c_nBitmapType1bpp) ? 1 : 32;
			Value2 = EncoderValueMultiFrame;

			// update value
			if (m_TiffCompression == c_nTiffCompressionNone)
				Value1 = EncoderValueCompressionNone;
			else if (m_TiffCompression == c_nTiffCompressionLzw)
				Value1 = EncoderValueCompressionLZW;
			else if (m_TiffCompression == c_nTiffCompressionRle)
				Value1 = EncoderValueCompressionRle;
			else if (m_TiffCompression == c_nTiffCompressionCCITT3)
				Value1 = EncoderValueCompressionCCITT3;
			else if (m_TiffCompression == c_nTiffCompressionCCITT4)
				Value1 = EncoderValueCompressionCCITT4;

			// update values count
			settings->Count = (m_TiffMultipaged == VARIANT_TRUE) ? 3 : 2;

			Success = SaveImageAs(Path, nBpp, L"image/tiff", settings, Image, bArrayOrInterface);

			// clear memory
			free(settings);
		}
		catch(...)
		{
			// update result flag
			Success = VARIANT_FALSE;
		}

		// all ok
		return Success;
	}
	VARIANT_BOOL SaveImageAsAnimated(BSTR Path, LONG Format, void** Image, BOOL bArrayOrInterface = TRUE)
	{
		return S_OK;
		
	}
	VARIANT_BOOL SaveImageAsFrame(void** Image, VARIANT_BOOL LastFrame, BOOL bArrayOrInterface = TRUE)
	{
		if ( 0 == m_nMultipagedFormat )
		{
			// update result flag
			VARIANT_BOOL Success = VARIANT_FALSE;

			// check for valid image
			if (!m_bitmap)
				return VARIANT_FALSE;

			// update result flag
			Success = VARIANT_TRUE;

			// variables
			Bitmap* bitmap = 0;

			// save image to file
			try 
			{
				// compute bpp value
				int nBpp = (m_TiffCompression == c_nTiffCompressionCCITT3 || m_TiffCompression == c_nTiffCompressionCCITT4 || m_TiffCompression == c_nTiffCompressionRle) ? c_nBitmapType1bppDither : c_nBitmapType32bpp;

				// convert array to image
				if (bArrayOrInterface)
					ArrayToImage(Image, &bitmap, nBpp);
				else
					InterfaceToImage(Image, &bitmap, nBpp);

				// save info properties
				UpdateProperties(bitmap, FALSE);

				// locate encoder CLSID
				CLSID encoder; GetEncoderClsid(L"image/tiff", &encoder);

				// setup encoder values
				ULONG Value0, Value1, Value2;

				// create encoder settings structure
				EncoderParameters* settings = (EncoderParameters*)malloc(sizeof(EncoderParameters) + 3 * sizeof(EncoderParameter));

				// update value settings
				settings->Parameter[0].Guid = EncoderColorDepth;
				settings->Parameter[0].Type = EncoderParameterValueTypeLong;
				settings->Parameter[0].NumberOfValues = 1;
				settings->Parameter[0].Value = &Value0;
				settings->Parameter[1].Guid = EncoderCompression;
				settings->Parameter[1].Type = EncoderParameterValueTypeLong;
				settings->Parameter[1].NumberOfValues = 1;
				settings->Parameter[1].Value = &Value1;
				settings->Parameter[2].Guid = EncoderSaveFlag;
				settings->Parameter[2].Type = EncoderParameterValueTypeLong;
				settings->Parameter[2].NumberOfValues = 1;
				settings->Parameter[2].Value = &Value2;

				// update values
				Value0 = (nBpp == c_nBitmapType1bpp || nBpp == c_nBitmapType1bppDither) ? 1 : 32;
				Value2 = EncoderValueFrameDimensionPage;

				// update value
				if (m_TiffCompression == c_nTiffCompressionNone)
					Value1 = EncoderValueCompressionNone;
				else if (m_TiffCompression == c_nTiffCompressionLzw)
					Value1 = EncoderValueCompressionLZW;
				else if (m_TiffCompression == c_nTiffCompressionRle)
					Value1 = EncoderValueCompressionRle;
				else if (m_TiffCompression == c_nTiffCompressionCCITT3)
					Value1 = EncoderValueCompressionCCITT3;
				else if (m_TiffCompression == c_nTiffCompressionCCITT4)
					Value1 = EncoderValueCompressionCCITT4;

				// update values count
				settings->Count = 3;

				// save image
				m_bitmap->SaveAdd(bitmap, settings);

				// check for last error
				if (m_bitmap->GetLastStatus() != Ok)
				{
					// clear memory
					free(settings);

					// raise error
					throw 0;
				}

				// close file if necessary
				if (LastFrame == VARIANT_TRUE)
				{
					// update encoder settings
					Value2 = EncoderValueFlush;

					// close the multiframe file
					m_bitmap->SaveAdd(settings);

					// check for last error
					if (m_bitmap->GetLastStatus() != Ok)
					{
						// clear memory
						free(settings);

						// raise error
						throw 0;
					}
				}

				// clear memory
				free(settings);
			}
			catch(...)
			{
				// update result flag
				Success = VARIANT_FALSE;
			}

			// clear memory
			delete bitmap;

			// close multipaged file
			if (LastFrame == VARIANT_TRUE)
				ClearBitmap();

			// all ok
			return Success;
		}
		return S_OK;
		
	}
	VARIANT_BOOL SaveImageAsPicture(void** Image, IPictureDisp** Picture, BOOL bArrayOrInterface = TRUE)
	{
		// update result flag
		VARIANT_BOOL Success = VARIANT_TRUE;

		// variables
		Bitmap* bitmap = 0;

		// convert image to PictureDisp
		try
		{
			// convert array to image
			if (bArrayOrInterface)
				ArrayToImage(Image, &bitmap);
			else
				InterfaceToImage(Image, &bitmap);

			// locate encoder CLSID
			CLSID encoder; GetEncoderClsid(L"image/bmp", &encoder);

			// setup picture
			PICTDESC pict; ::ZeroMemory(&pict, sizeof(PICTDESC));
			pict.cbSizeofstruct = sizeof(PICTDESC); pict.picType = PICTYPE_BITMAP;
			if (bitmap->GetHBITMAP(Color::White, &pict.bmp.hbitmap) != Ok)
				throw 0;

			// create picture
			if (FAILED(OleCreatePictureIndirect(&pict, IID_IPictureDisp, TRUE, (void**)Picture)))
				throw 0;
		}
		catch (...)
		{
			// update result flag
			Success = VARIANT_FALSE;
		}

		// clear memory
		delete bitmap;

		// delete backup
		ClearBackup();

		// all ok
		return Success;
	}
	
	template<class T> // save image as pcx, tga and ras
	VARIANT_BOOL SaveImageAsT(T& ImageFormat, BSTR Path, void** Image, BOOL bArray = TRUE)
	{
		VARIANT_BOOL Success = VARIANT_FALSE;


		int nWidth = 0;
		int nHeight = 0;
		int nChannels = 4;
		BYTE* pPixels = 0;
		MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;

		try 
		{
			if (bArray)
				pPixels = ImageStudioUtils::ExtractImage((SAFEARRAY**)Image, nWidth, nHeight, nChannels);
			else
				pPixels = ExtractImageFromInterface((IUnknown**)Image, pMediaData, nWidth, nHeight);

			if (nWidth > 1 && nHeight > 1 && nChannels == 4 && pPixels)
			{
				if (ImageFormat.Save(Path, nWidth, nHeight, nChannels, pPixels, 1))
					Success = VARIANT_TRUE;
			}
		}
		catch (...)
		{
			Success = VARIANT_FALSE;
		}

		if (NULL != pMediaData)
			pMediaData->Release();

		return Success;
	}
	VARIANT_BOOL SaveImageAsMF(BSTR Path, void** Image, BOOL bEnhancedMetaFile, BOOL bArray = TRUE)
	{
		USES_CONVERSION;

		VARIANT_BOOL Success = VARIANT_TRUE;


		int nWidth = 0;
		int nHeight = 0;
		int nChannels = 4;
		BYTE* pPixels = 0;
		MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;

		try 
		{
			if (bArray)
				pPixels = ImageStudioUtils::ExtractImage((SAFEARRAY**)Image, nWidth, nHeight, nChannels);
			else
				pPixels = ExtractImageFromInterface((IUnknown**)Image, pMediaData, nWidth, nHeight);

			// При записи в Wmf или Emf, некоторые пикселы становились прозрачными.
			// Оказалось, что это пиксели: R = 13, G = 11, B = 12. Достаточно изменить, например,
			// синий канал на 13 и все станет нормально.

			// Непонятно из-за чего это происходит, но теперь все работает хорошо.

			BYTE *pCur = pPixels;
			for ( int nY = 0; nY < nHeight; nY++ )
			{
				for ( int nX = 0; nX < nWidth; nX++, pCur += 4 )
				{
					if ( 12 == pCur[0] && 11 == pCur[1] && 13 == pCur[2] )
						pCur[0] = 13;
				}
			}
			//-------------------------------------------------------------------------------------

			// create file
			HDC metafileDC = 0;
			if (!bEnhancedMetaFile)
				metafileDC = CreateMetaFile(OLE2T(Path));
			else
				metafileDC = CreateEnhMetaFile(0, OLE2T(Path), 0, 0);

			if (!metafileDC)
				throw 0;		// error creating metafile

			// make BITMAPINFO
			BITMAPINFO* pBmpInfo = (BITMAPINFO*)GlobalAlloc(GPTR, sizeof(BITMAPINFOHEADER) + nWidth * nHeight * nChannels);

			// setup bitmap header
			pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			pBmpInfo->bmiHeader.biWidth = nWidth;
			pBmpInfo->bmiHeader.biHeight = nHeight;
			pBmpInfo->bmiHeader.biPlanes = 1;
			pBmpInfo->bmiHeader.biBitCount = nChannels*8;
			pBmpInfo->bmiHeader.biCompression = BI_RGB;
			pBmpInfo->bmiHeader.biSizeImage = pBmpInfo->bmiHeader.biWidth * pBmpInfo->bmiHeader.biHeight * nChannels;

			// setup bitmap data
			CopyMemory(((BYTE*)pBmpInfo) + sizeof(BITMAPINFOHEADER), pPixels, pBmpInfo->bmiHeader.biSizeImage);

			// draw bitmap in metafile
			SetDIBitsToDevice(metafileDC, 0, 0, pBmpInfo->bmiHeader.biWidth, pBmpInfo->bmiHeader.biHeight,
				0, 0, 0, pBmpInfo->bmiHeader.biHeight, ((BYTE*)pBmpInfo) + sizeof(BITMAPINFOHEADER), pBmpInfo, DIB_RGB_COLORS);

			// close file and delete metafile handle
			if (!bEnhancedMetaFile)
				DeleteMetaFile(CloseMetaFile(metafileDC));
			else
				DeleteEnhMetaFile(CloseEnhMetaFile(metafileDC));

			// delete image
			GlobalFree(pBmpInfo);

		}
		catch (...)
		{
			Success = VARIANT_FALSE;
		}

		if (NULL != pMediaData)
			pMediaData->Release();

		return Success;
	}
		
public:
	
	STDMETHOD(GetFrameCount)(BSTR Path, long* Count)
	{
		// reset output value
		*Count = 0;

		// Gdi+ Bitmap object
		Bitmap* bitmap = 0;

		// load image from file
		try 
		{
			// attempt to load image with Gdi+
			bitmap = new Bitmap(Path);

			// check for valid bitmap
			if (bitmap && bitmap->GetLastStatus() == Ok)
			{
				// variables
				GUID format;

				// retrieve image format
				bitmap->GetRawFormat(&format);

				// check for format type
				if (format == ImageFormatTIFF)
					*Count = bitmap->GetFrameCount(&FrameDimensionPage);
				else if (format == ImageFormatGIF)
					*Count = bitmap->GetFrameCount(&FrameDimensionTime);
			}
		}
		catch(...)
		{
		}

		// free memory
		if (bitmap)
			delete bitmap;

		// check for valid count
		if (*Count < 1)
		{
			// variables
			SAFEARRAY* Array = 0;
			VARIANT_BOOL Success = VARIANT_FALSE;

			// try to load image
			LoadImage(Path, &Array, &Success);

			// check for valid image
			if (Success == VARIANT_TRUE)
				*Count = 1;

			// destroy temporary safe array
			ImageStudioUtils::DestroySafeArray(&Array);
		}

		// all ok
		return S_OK;
	}
	STDMETHOD(SaveAsBmp)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAs(Path, m_BitmapType, L"image/bmp", NULL, (void**) Image, TRUE);

		return S_OK;
	}
	STDMETHOD(SaveAsGif)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAs(Path, c_nBitmapType32bpp, L"image/gif", NULL, (void**) Image, TRUE);

		return S_OK;
	}
	STDMETHOD(SaveAsJpeg)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAsJpeg(Path, (void**) Image, TRUE);
		//m_oMetaData.SaveToBMP(Path);

		return S_OK;
	}
	STDMETHOD(SaveAsPng)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAs(Path, m_BitmapType, L"image/png", NULL, (void**) Image, TRUE);
		CString sFilePath(Path);
		m_oMetaData.SaveToPNG(sFilePath);

		return S_OK;
	}
	STDMETHOD(SaveAsTiff)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAsTiff(Path, (void**) Image, TRUE);
		//m_oMetaData.SaveToBMP(sFilePath);

		return S_OK;
	}
	STDMETHOD(SaveFrame)(SAFEARRAY** Image, VARIANT_BOOL LastFrame, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAsFrame((void**) Image, LastFrame, TRUE);

		return S_OK;
	}
	STDMETHOD(SaveFrame2)(IUnknown** Image, VARIANT_BOOL LastFrame, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAsFrame((void**) Image, LastFrame, FALSE);

		return S_OK;
	}
	STDMETHOD(SavePicture)(SAFEARRAY** Image, IPictureDisp** Picture, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAsPicture((void**) Image, Picture, TRUE);

		return S_OK;
	}
	STDMETHOD(SavePicture2)(IUnknown** Image, IPictureDisp** Picture, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAsPicture((void**) Image, Picture, FALSE);

		return S_OK;
	}
	STDMETHOD(SaveImage2)(IUnknown** Image, LONG Format, BSTR Path, VARIANT_BOOL* Success)
	{
		*Success = VARIANT_FALSE;

		switch (Format)
		{
		case IMAGEFORMAT_BMP: { *Success = SaveImageAs(Path, m_BitmapType, L"image/bmp", NULL, (void**) Image, FALSE);  break; }
		case IMAGEFORMAT_GIF: { if( VARIANT_FALSE == m_TiffMultipaged )
								  *Success = SaveImageAs(Path, c_nBitmapType32bpp, L"image/gif", NULL, (void**) Image, FALSE);
							    else
								  *Success = SaveImageAsAnimated(Path, IMAGEFORMAT_GIF, (void**) Image, FALSE);
								break; }
		case IMAGEFORMAT_JPE: { *Success = SaveImageAsJpeg(Path, (void**) Image, FALSE); break; }
		case IMAGEFORMAT_PNG: { if( VARIANT_FALSE == m_TiffMultipaged )
								  *Success = SaveImageAs(Path, m_BitmapType, L"image/png", NULL, (void**) Image, FALSE);
							    else
								  *Success = SaveImageAsAnimated(Path, IMAGEFORMAT_PNG, (void**) Image, FALSE);
								break; }
		case IMAGEFORMAT_TIF: { *Success = SaveImageAsTiff(Path, (void**) Image, FALSE); break; }
		case IMAGEFORMAT_WMF: { *Success = SaveImageAsMF(Path, (void**) Image, FALSE, FALSE); break; }
		case IMAGEFORMAT_EMF: { *Success = SaveImageAsMF(Path, (void**) Image, TRUE, FALSE); break; }
		//case IMAGEFORMAT_PCX: { ImgFilesPCX imgPcx; *Success = SaveImageAsT(imgPcx, Path, (void**) Image, FALSE); break; }
		//case IMAGEFORMAT_TGA: { ImgFilesTGA imgTga; *Success = SaveImageAsT(imgTga, Path, (void**) Image, FALSE); break; }
		//case IMAGEFORMAT_RAS: { ImgFilesRAS imgRas; *Success = SaveImageAsT(imgRas, Path, (void**) Image, FALSE); break; }
		case IMAGEFORMAT_PCX:
			{
				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( *Image, FALSE))
				{	
					*Success = VARIANT_FALSE;
				}
				else
				{
					oImage.RotateFlip( Gdiplus::RotateNoneFlipY );

					ImageStudio::IO::Pcx oWriter;

					BOOL bResult = oWriter.ToFile(&oImage, ATL::CString( Path ), ImageStudio::IO::c_nFormat_Uncompressed_32bpp);
					*Success = bResult ? VARIANT_TRUE : VARIANT_FALSE;
				}
			}
			break;
		case IMAGEFORMAT_TGA:
			{
				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( *Image, FALSE))
				{	
					*Success = VARIANT_FALSE;
				}
				else
				{
					oImage.RotateFlip( Gdiplus::RotateNoneFlipY );

					ImageStudio::IO::Tga oWriter;

					BOOL bResult = oWriter.ToFile(&oImage, ATL::CString( Path ), ImageStudio::IO::c_nFormat_Uncompressed_32bpp);
					*Success = bResult ? VARIANT_TRUE : VARIANT_FALSE;
				}
			}
			break;
		case IMAGEFORMAT_RAS:
			{
				ImageStudio::Core::Image oImage;
				
				if (!oImage.FromMediaData( *Image, FALSE))
				{	
					*Success = VARIANT_FALSE;
				}
				else
				{
					oImage.RotateFlip( Gdiplus::RotateNoneFlipY );

					ImageStudio::IO::Ras oWriter;

					BOOL bResult = oWriter.ToFile(&oImage, ATL::CString( Path ), ImageStudio::IO::c_nFormat_Uncompressed_32bpp);
					*Success = bResult ? VARIANT_TRUE : VARIANT_FALSE;
				}
			}
			break;
		}

		if (Format >= 1024)
		{
			ImageStudio::IO::IPod oFileIPod;

			if (oFileIPod.FromMediaData(*Image, FALSE))
			{
				if (oFileIPod.ToFile(CString(Path), Format))
					*Success = VARIANT_TRUE;
			}
		}

		return S_OK;
	}
	STDMETHOD(SaveAsPcx)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		ImgFilesPCX imgPcx;

		*Success = SaveImageAsT(imgPcx, Path, (void**) Image, TRUE);

		return S_OK;
	}
	STDMETHOD(SaveAsTga)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		ImgFilesTGA imgTga;

		*Success = SaveImageAsT(imgTga, Path, (void**) Image, TRUE);
		CString sFilePath(Path);
		m_oMetaData.SaveToTGA(sFilePath);

		return S_OK;
	}
	STDMETHOD(SaveAsRas)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		ImgFilesRAS imgRas;

		*Success = SaveImageAsT(imgRas, Path, (void**) Image, TRUE);

		return S_OK;
	}
	STDMETHOD(SaveAsWmf)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAsMF(Path, (void**) Image, FALSE, TRUE);

		return S_OK;
	}
	STDMETHOD(SaveAsEmf)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		*Success = SaveImageAsMF(Path, (void**) Image, TRUE, TRUE);

		return S_OK;
	}
	STDMETHOD(SaveAsJ2k)(SAFEARRAY** Image, BSTR Path, VARIANT_BOOL* Success)
	{
		// update result flag
		*Success = VARIANT_FALSE;
		// save image to file
		try 
		{
			// variables
			ImgFilesJ2K imgJ2k;

			// save image to file
			if (imgJ2k.SaveJ2k(Path, Image, m_JpegQuality))
			{
				// update result flag
				*Success = VARIANT_TRUE;
			}
		}
		catch(...)
		{
			// update result flag
			*Success = VARIANT_FALSE;
		}

		// all ok-na
		return S_OK;
	}
			
	// general properties
	STDMETHOD(get_JpegQuality)(long *Quality)
		{ *Quality = m_JpegQuality; return S_OK; }
	STDMETHOD(put_JpegQuality)(long Quality)
		{ m_JpegQuality = Quality; return S_OK; }
	STDMETHOD(get_TiffCompression)(long *Compression)
		{ *Compression = m_TiffCompression; return S_OK; }
	STDMETHOD(put_TiffCompression)(long Compression)
		{ m_TiffCompression = Compression; return S_OK; }
	STDMETHOD(get_TiffMultipaged)(VARIANT_BOOL *Multipaged)
		{ *Multipaged = m_TiffMultipaged; return S_OK; }
	STDMETHOD(put_TiffMultipaged)(VARIANT_BOOL Multipaged)
		{ m_TiffMultipaged = Multipaged; return S_OK; }
	STDMETHOD(get_BitmapType)(long *Type)
		{ *Type = m_BitmapType; return S_OK; }
	STDMETHOD(put_BitmapType)(long Type)
		{ m_BitmapType = Type; return S_OK; }
	
	// information properties
	STDMETHOD(get_InfoTitle)(BSTR* Value)
		{ *Value = m_InfoTitle.AllocSysString(); return S_OK; }
	STDMETHOD(put_InfoTitle)(BSTR Value)
		{ m_InfoTitle = Value; return S_OK; }
	STDMETHOD(get_InfoDescription)(BSTR* Value)
		{ *Value = m_InfoDescription.AllocSysString(); return S_OK; }
	STDMETHOD(put_InfoDescription)(BSTR Value)
		{ m_InfoDescription = Value; return S_OK; }
	STDMETHOD(get_InfoDateTime)(BSTR* Value)
		{ *Value = m_InfoDateTime.AllocSysString(); return S_OK; }
	STDMETHOD(put_InfoDateTime)(BSTR Value)
		{ m_InfoDateTime = Value; return S_OK; }
	STDMETHOD(get_InfoAuthor)(BSTR* Value)
		{ *Value = m_InfoAuthor.AllocSysString(); return S_OK; }
	STDMETHOD(put_InfoAuthor)(BSTR Value)
		{ m_InfoAuthor = Value; return S_OK; }
	STDMETHOD(get_InfoCopyright)(BSTR* Value)
		{ *Value = m_InfoCopyright.AllocSysString(); return S_OK; }
	STDMETHOD(put_InfoCopyright)(BSTR Value)
		{ m_InfoCopyright = Value; return S_OK; }
	STDMETHOD(get_InfoEquipmentType)(BSTR* Value)
		{ *Value = m_InfoEquipmentType.AllocSysString(); return S_OK; }
	STDMETHOD(put_InfoEquipmentType)(BSTR Value)
		{ m_InfoEquipmentType = Value; return S_OK; }
	STDMETHOD(get_InfoEquipmentModel)(BSTR* Value)
		{ *Value = m_InfoEquipmentModel.AllocSysString(); return S_OK; }
	STDMETHOD(put_InfoEquipmentModel)(BSTR Value)
		{ m_InfoEquipmentModel = Value; return S_OK; }
	STDMETHOD(get_InfoVerticalResolution)(double* Value)
		{ *Value = m_InfoVerticalResolution; return S_OK; }
	STDMETHOD(put_InfoVerticalResolution)(double Value)
		{ m_InfoVerticalResolution = Value; return S_OK; }
	STDMETHOD(get_InfoHorizontalResolution)(double* Value)
		{ *Value = m_InfoHorizontalResolution; return S_OK; }
	STDMETHOD(put_InfoHorizontalResolution)(double Value)
		{ m_InfoHorizontalResolution = Value; return S_OK; }
	
	STDMETHOD(LoadImage)(BSTR Path, SAFEARRAY** Image, VARIANT_BOOL* Success)
	{
		*Success = LoadImageTo(Path, (void**) Image, ImageToArray, TRUE);

		return S_OK;
	}
	STDMETHOD(LoadImage2)(BSTR Path, IUnknown** Image, VARIANT_BOOL* Success)
	{
		*Success = LoadImageTo(Path, (void**) Image, ImageToInterface, FALSE);

		return S_OK;
	}
	STDMETHOD(LoadPicture)(IPictureDisp* Picture, SAFEARRAY** Image, VARIANT_BOOL* Success)
	{
		*Success = LoadPictureTo(Picture, (void**) Image, ImageToArray);

		return S_OK;
	}
	STDMETHOD(LoadPicture2)(IPictureDisp* Picture, IUnknown** Image, VARIANT_BOOL* Success)
	{
		*Success = LoadPictureTo(Picture, (void**) Image, ImageToInterface);

		return S_OK;
	}
	STDMETHOD(LoadScreenshot)(SAFEARRAY** Image, VARIANT_BOOL* Success)
	{
		*Success = LoadScreenshotTo((void**) Image, ImageToArray);

		return S_OK;
	}
	STDMETHOD(LoadScreenshot2)(IUnknown** Image, VARIANT_BOOL* Success)
	{
		*Success = LoadScreenshotTo((void**) Image, ImageToInterface);

		return S_OK;
	}
	STDMETHOD(LoadFrame)(BSTR Path, long Frame, SAFEARRAY** Image, VARIANT_BOOL* Success)
	{
		*Success = LoadFrameTo(Path, Frame, (void**) Image, ImageToArray, TRUE);

		return S_OK;
	}
	STDMETHOD(LoadFrame2)(BSTR Path, long Frame, IUnknown** Image, VARIANT_BOOL* Success)
	{
		*Success = LoadFrameTo(Path, Frame, (void**) Image, ImageToInterface, FALSE);

		return S_OK;
	}

	STDMETHOD(SetMetaData)(BSTR bsMetaDataXML)
	{
		CW2T convert(bsMetaDataXML);
		m_strInfoXML = convert;
		m_oMetaData.SetXML(m_strInfoXML);
		return S_OK;
	}
	STDMETHOD(GetMetaData)(BSTR * bsMetaDataXML)
	{
		*bsMetaDataXML = m_strInfoXML.AllocSysString();
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT	ParamValue)
	{
		CComBSTR ParamStr = ParamName;

		if ( ParamStr == L"AnimationDuration" && ParamValue.vt == VT_R8 )
		{
			m_dAnimationDuration = ParamValue.dblVal;
		}
		else if ( ParamStr == L"AnimationWidth" && ParamValue.vt == VT_I4 )
		{
			m_nAnimationWidth = ParamValue.lVal;
		}
		else if ( ParamStr == L"AnimationHeight" && ParamValue.vt == VT_I4 )
		{
			m_nAnimationHeight = ParamValue.lVal;
		}
		else if( ParamStr == L"AnimationAlpha" && ParamValue.vt == VT_BOOL )
		{
			if( VARIANT_FALSE == ParamValue.boolVal )
				m_bAnimationGifWithAlpha = false;
			else
				m_bAnimationGifWithAlpha = true;
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT *	ParamValue)
	{
		if (ParamName == L"Orientation")
		{
			(*ParamValue).iVal = m_oMetaData.GetOrientation();
		}
		return S_OK;
	}

	STDMETHOD(MetaDataFromFile)(BSTR Path)
	{
		m_strInfoXML = _T("");

		CW2T convert(Path);

		CString sPath(convert);
		m_oMetaData.Clear();
		m_oMetaData.FromFile(sPath);

		m_strInfoXML = m_oMetaData.GetXML();
		return S_OK;
	}

};

