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
#include "ArtBlip.h"

#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include "../../Reader/PPTDocumentInfo.h"



void CRecordOfficeArtBlip::ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
{
	CMetaFileBuffer oMetaFile;

	CString sExt = L".jpg";

	int pos = pStream->tell();

	if ((oHeader.RecVersion != PSFLAG_CONTAINER) && ((oHeader.RecVersion & 0x0F) != 0x0F))
	{
		int lOffset = 0;

		switch (oHeader.RecType)
		{
			case RECORD_TYPE_ESCHER_BLIP_EMF:
			{
				if		(0x03D4 == oHeader.RecInstance)	lOffset = 16;
				else if (0x03D5 == oHeader.RecInstance)	lOffset = 32;

				StreamUtils::StreamSkip(lOffset, pStream);
				lOffset += 34;

				oMetaFile.m_bIsValid	= TRUE;
				oMetaFile.m_sExtension	= L".emf";
				
				CMetaHeader oMetaHeader;
				oMetaHeader.FromStream(pStream);
				
				Gdiplus::ENHMETAHEADER3 oEmfHeader;
				oMetaHeader.ToEMFHeader(&oEmfHeader);

				oMetaFile.SetHeader(NULL, 0);

				BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
				pStream->read(pData, oHeader.RecLen - lOffset); 

				oMetaFile.SetData(pData, oMetaHeader.cbSave, oMetaHeader.cbSize, (bool)(oMetaHeader.compression != 0xFE) );

			}break;
			case RECORD_TYPE_ESCHER_BLIP_WMF:
			{
				if		(0x0216 == oHeader.RecInstance)	lOffset = 16;
				else if (0x0217 == oHeader.RecInstance)	lOffset = 32;

				StreamUtils::StreamSkip(lOffset, pStream);

				lOffset += 34;

				oMetaFile.m_bIsValid	= TRUE;
				oMetaFile.m_sExtension	= L".wmf";
				
				CMetaHeader oMetaHeader;
				oMetaHeader.FromStream(pStream); 
				Gdiplus::WmfPlaceableFileHeader oWmfHeader;
				oMetaHeader.ToWMFHeader(&oWmfHeader);
				
				LONG lLenHeader = 22;
				BYTE* pMetaHeader = new BYTE[lLenHeader]; // удалится в oMetaFile
				memcpy(pMetaHeader, (void*)(&oWmfHeader), lLenHeader);

				oMetaFile.SetHeader(pMetaHeader, lLenHeader);

				BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
				pStream->read(pData, oHeader.RecLen - lOffset); 

				oMetaFile.SetData(pData, oMetaHeader.cbSave, oMetaHeader.cbSize, (bool)(oMetaHeader.compression != 0xFE) );

			}break;
			case RECORD_TYPE_ESCHER_BLIP_PICT://Medwoche.ppt , (483)
			{
				if		(0x0542 == oHeader.RecInstance)	lOffset = 16;
				else if (0x0543 == oHeader.RecInstance)	lOffset = 32;

				StreamUtils::StreamSkip(lOffset, pStream);

				lOffset += 34;

				oMetaFile.m_bIsValid	= TRUE;
				oMetaFile.m_sExtension	= L".wmf";//L".pct"; - ВРЕМЕННО пока не сделана конвертация pct(pic) хоть во что нито  !!!
				
				CMetaHeader oMetaHeader;
				oMetaHeader.FromStream(pStream); //отдельно вынесенный заголовок.. "форматный" находится в блоке данных

				BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
				pStream->read(pData, oHeader.RecLen - lOffset); 

				oMetaFile.SetData(pData, oMetaHeader.cbSave, oMetaHeader.cbSize, (bool)(oMetaHeader.compression != 0xFE) );
			}break;
			case RECORD_TYPE_ESCHER_BLIP_JPEG:
			{
				if		(0x046A == oHeader.RecInstance || 0x06E2 == oHeader.RecInstance)	lOffset = 17;
				else if (0x046B == oHeader.RecInstance || 0x06E3 == oHeader.RecInstance)	lOffset = 33;
				
				StreamUtils::StreamSkip(lOffset, pStream);
				
				sExt = _T(".jpg");
				break;
			}
			case RECORD_TYPE_ESCHER_BLIP_PNG:
			{
				if		(0x06E0 == oHeader.RecInstance)	lOffset = 17;
				else if (0x06E1 == oHeader.RecInstance)	lOffset = 33;

				StreamUtils::StreamSkip(lOffset, pStream);

				sExt = _T(".png");
				break;
			}
			case RECORD_TYPE_ESCHER_BLIP_DIB:
			{
				if		(0x07A8 == oHeader.RecInstance)	lOffset = 17;
				else if (0x07A9 == oHeader.RecInstance)	lOffset = 33;
				
				StreamUtils::StreamSkip(lOffset, pStream);
			
				sExt = _T(".bmp");
				break;
			}
			case RECORD_TYPE_ESCHER_BLIP_TIFF:
			{
				if		(0x06E4 == oHeader.RecInstance)	lOffset = 17;
				else if (0x06E5 == oHeader.RecInstance)	lOffset = 33;

				StreamUtils::StreamSkip(lOffset, pStream);
				
				sExt = _T(".tif");
				break;
			}
			default:
			{
				break;
			}
		}
		int nImagesCount = 0;
		if (m_oDocumentInfo)
		{
			nImagesCount = m_oDocumentInfo->m_mapStoreImageFile.size();
		}
		//else nImagesCount = generate uniq name

		if (oMetaFile.m_bIsValid)
		{
			CString strFile = CString(L"Image ") + CDirectory::ToString(nImagesCount + 1) + oMetaFile.m_sExtension;

			CFile fileMeta;
			HRESULT hr = fileMeta.CreateFile(m_strMemoryForder + FILE_SEPARATOR_STR + strFile);
		
			if (hr == S_OK)
			{
				oMetaFile.ToFile(&fileMeta);
				fileMeta.CloseFile();
			}
			m_sFileName = string2std_string(strFile);
		}
		else
		{
			BYTE* pImage = new BYTE[oHeader.RecLen - lOffset]; 

			pStream->read(pImage, oHeader.RecLen - lOffset); 

			CString strFile = CString(L"Image ") + CDirectory::ToString(nImagesCount + 1) + sExt;
			
			CFile fileImage;
			HRESULT hr = fileImage.CreateFile(m_strMemoryForder + FILE_SEPARATOR_STR +  strFile);
			if (hr == S_OK)
			{
				if (RECORD_TYPE_ESCHER_BLIP_DIB == oHeader.RecType)
				{
					WORD vtType = 0x4D42;
					fileImage.WriteFile((void*)&vtType, 2);
					DWORD dwLen = oHeader.RecLen - lOffset;
					fileImage.WriteFile((void*)&dwLen, 4);
					DWORD dwRes = 0;
					fileImage.WriteFile((void*)&dwRes, 4);
					DWORD dwOffset = 2;
					fileImage.WriteFile((void*)&dwOffset, 4);
				}
				fileImage.WriteFile((void*)pImage, oHeader.RecLen - lOffset);
				fileImage.CloseFile();
			}
			if (pImage)delete[] pImage;
			pImage = NULL;	

			m_sFileName = string2std_string(strFile);
		}
		int dwOffset = pos - 8;
		
		if (m_oDocumentInfo)
		{
			m_oDocumentInfo->m_mapStoreImageFile[dwOffset ] = m_sFileName;
		}
	}
}
