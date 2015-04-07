
#include "BlipFactory.h"

namespace GdiPlusHelper
{
	inline static int CompareStrings (const wchar_t* str1, const wchar_t* str2)
	{
		CString cstr1; cstr1 = str1;
		CString cstr2; cstr2 = str2;

		if (cstr1 == cstr2)
			return 0;

		return 1;
	}

	//inline static void GetEncoderCLSID (const wchar_t* pFormat, CLSID* pClsid)
	//{
	//	// variables
	//	UINT nEncoders = 0;
	//	UINT nSize = 0;
	//	Gdiplus::ImageCodecInfo* pImageCodecInfo = 0;

	//	// retrieve encoders info
	//	Gdiplus::GetImageEncodersSize(&nEncoders, &nSize);

	//	// check for valid encoders
	//	if (!nSize)
	//		throw 0;

	//	// create encoders info structure of necessary size
	//	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(nSize));

	//	// check for valid encoder
	//	if (!pImageCodecInfo)
	//		throw 0;

	//	// retrieve all encoders
	//	Gdiplus::GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

	//	// locate necessary encoder
	//	for (UINT nEncoder = 0; nEncoder < nEncoders; ++nEncoder)
	//	{
	//		// compare MIME strings
	//		if (CompareStrings(pImageCodecInfo[nEncoder].MimeType, pFormat) == 0)
	//		{
	//			// save CLSID
	//			*pClsid = pImageCodecInfo[nEncoder].Clsid;

	//			// clear memory
	//			free(pImageCodecInfo);

	//			// all ok
	//			return;
	//		}    
	//	}

	//	// clear memory
	//	free(pImageCodecInfo);

	//	// codec not found
	//	throw 0;
	//}
}

namespace OfficeArt
{
	OfficeArtBlip* BlipFactory::GetBlipWithPngTransform()
	{
		//CString strTempPath;
		//if (::GetTempPath(_MAX_PATH, strTempPath.GetBuffer(_MAX_PATH)) != 0)
		//	strTempPath.ReleaseBuffer();
		//else
		//	strTempPath = _T(".");

		//CString strTempFile;
		//if (::GetTempFileName(strTempPath, _T("file"), 0, strTempFile.GetBuffer(_MAX_PATH)) != 0)
		//{
		//	CString tempFile; tempFile.Format (_T("%s%s"), strTempFile, _T(".png"));

		//	CGdiPlusInit m_oInitGdiplus;		
		//	if (m_oInitGdiplus.Init())
		//	{
		//		Gdiplus::Bitmap oBitmap (m_sFile.c_str());
		//		if (Gdiplus::Ok == oBitmap.GetLastStatus())
		//		{
		//			CLSID guid;
		//			GdiPlusHelper::GetEncoderCLSID (L"image/png", &guid);

		//			if (Gdiplus::Ok == oBitmap.Save (tempFile, &guid))
		//			{
		//				if (Gdiplus::Ok == oBitmap.GetLastStatus())
		//				{
		//					m_sFile			=	std::wstring (tempFile);
		//					m_bDeleteFile	=	TRUE;

		//					return GetOfficeArtBlip();
		//				}
		//			}
		//		}
		//	}
		//}

		return NULL;
	}
}