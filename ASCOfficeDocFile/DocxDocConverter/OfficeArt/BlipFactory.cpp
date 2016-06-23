/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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