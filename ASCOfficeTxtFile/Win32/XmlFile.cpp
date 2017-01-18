/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#include "stdafx.h"
#include "XmlFile.h"

#include "../TxtXmlFormatLib/Source/TxtXmlFile.h"

#include "../../OfficeUtils/src/OfficeUtils.h"

CXmlFile::CXmlFile()
{
}


HRESULT CXmlFile::FinalConstruct()
{
	return S_OK;
}


void CXmlFile::FinalRelease()
{
}


bool CXmlFile::Progress(long ID, long Percent)
{
	SHORT res = 0;
	m_lPercent = Percent;
	OnProgressEx(ID, Percent, &res);
	return (res != 0);
}


STDMETHODIMP CXmlFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	CTxtXmlFile file;

	file.txt_LoadFromFile(sSrcFileName, sDstPath, sXMLOptions);

	return S_OK;
}


STDMETHODIMP CXmlFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
  return S_OK;
}

//const unsigned long CXmlFile::LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const
//{
//	HMODULE hMod = GetModuleHandle(L"ASCOfficeTxtFile.dll");
//	if (hMod)
//	{
//		HRSRC hRes = FindResource(hMod, lpResName, lpResType);
//		if (hRes)
//		{
//			HGLOBAL hGlob = LoadResource(hMod, hRes);
//			if (hGlob)
//			{
//				BYTE *lpbArray = (BYTE*)LockResource(hGlob);
//				if (lpbArray)
//				{
//					const DWORD dwFileSize = SizeofResource(hMod, hRes);
//					if (dwFileSize != 0)
//					{
//						int hFile = 0;
//						if (!_wsopen_s(&hFile, fileName, (O_BINARY | O_CREAT | _O_RDWR ), _SH_DENYNO, S_IWRITE))
//						{
//							_write(hFile, lpbArray, dwFileSize);
//							_close(hFile);
//						}
//					}
//				}
//			}
//		}
//	}
//	return GetLastError();
//}