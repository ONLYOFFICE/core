/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
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

#include "../../../../DesktopEditor/common/Directory.h"

#include "../../Reader/PPTDocumentInfo.h"

using namespace PPT;

CRecordOfficeArtBlip::CRecordOfficeArtBlip()
{
}

CRecordOfficeArtBlip::~CRecordOfficeArtBlip()
{
}

void CRecordOfficeArtBlip::ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
{
	if ((oHeader.RecVersion == PSFLAG_CONTAINER) || ((oHeader.RecVersion & 0x0F) == 0x0F)) return;
	
	CRYPT::ECMADecryptor *pDecryptor = m_pDocumentInfo ? m_pDocumentInfo->m_arUsers[0]->m_pDecryptor : NULL;

	CMetaFileBuffer	oMetaFile;
	std::wstring sExt = L".jpg";
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
			oMetaHeader.FromStream(pStream, pDecryptor);
			
			Gdiplus::ENHMETAHEADER3 oEmfHeader;
			oMetaHeader.ToEMFHeader(&oEmfHeader);

			oMetaFile.SetHeader(NULL, 0);

			int nDataSize = oHeader.RecLen - lOffset;
			BYTE* pData = (nDataSize > 0 && nDataSize < 0xffffff ) ? (new BYTE[nDataSize]) : NULL;
			
			if (pData)
			{
				pStream->read(pData, oHeader.RecLen - lOffset);
				if (pDecryptor)
				{
					pDecryptor->Decrypt((char*)pData, oHeader.RecLen - lOffset, 0);
				}
				oMetaFile.SetData(pData, oMetaHeader.cbSave, oMetaHeader.cbSize, (bool)(oMetaHeader.compression != 0xFE));
			}
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
			oMetaHeader.FromStream(pStream, pDecryptor); 
			
			Gdiplus::WmfPlaceableFileHeader oWmfHeader;
			oMetaHeader.ToWMFHeader(&oWmfHeader);
			
			LONG lLenHeader = 22;
			BYTE* pMetaHeader = new BYTE[lLenHeader]; // удалится в oMetaFile
			memcpy(pMetaHeader, (void*)(&oWmfHeader), lLenHeader);

			oMetaFile.SetHeader(pMetaHeader, lLenHeader);

			BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
			pStream->read(pData, oHeader.RecLen - lOffset); 
			if (pDecryptor)
			{
				pDecryptor->Decrypt((char*)pData, oHeader.RecLen - lOffset, 0);
			}
			oMetaFile.SetData(pData, oMetaHeader.cbSave, oMetaHeader.cbSize, (bool)(oMetaHeader.compression != 0xFE) );

		}break;
		case RECORD_TYPE_ESCHER_BLIP_PICT://Medwoche.ppt , (483), synergieInspiratie.ppt
		{
			if		(0x0542 == oHeader.RecInstance)	lOffset = 16;
			else if (0x0543 == oHeader.RecInstance)	lOffset = 32;

			StreamUtils::StreamSkip(lOffset, pStream);

			lOffset += 34;

			sExt = L".pct";

			CMetaHeader oMetaHeader;
			oMetaHeader.FromStream(pStream, pDecryptor); 

			BYTE* pData = new BYTE[oHeader.RecLen - lOffset];
			pStream->read(pData, oHeader.RecLen - lOffset); 
			if (pDecryptor)
			{
				pDecryptor->Decrypt((char*)pData, oHeader.RecLen - lOffset, 0);
			}
			oMetaFile.SetData(pData, oMetaHeader.cbSave, oMetaHeader.cbSize, (bool)(oMetaHeader.compression != 0xFE) );
		}break;
		case RECORD_TYPE_ESCHER_BLIP_JPEG:
		{
			if		(0x046A == oHeader.RecInstance || 0x06E2 == oHeader.RecInstance)	lOffset = 17;
			else if (0x046B == oHeader.RecInstance || 0x06E3 == oHeader.RecInstance)	lOffset = 33;
			
			StreamUtils::StreamSkip(lOffset, pStream);
			
			sExt = L".jpg";
			break;
		}
		case RECORD_TYPE_ESCHER_BLIP_PNG:
		{
			if		(0x06E0 == oHeader.RecInstance)	lOffset = 17;
			else if (0x06E1 == oHeader.RecInstance)	lOffset = 33;

			StreamUtils::StreamSkip(lOffset, pStream);

			sExt = L".png";
			break;
		}
		case RECORD_TYPE_ESCHER_BLIP_DIB:
		{
			if		(0x07A8 == oHeader.RecInstance)	lOffset = 17;
			else if (0x07A9 == oHeader.RecInstance)	lOffset = 33;
			
			StreamUtils::StreamSkip(lOffset, pStream);
		
			sExt = L".bmp";
			break;
		}
		case RECORD_TYPE_ESCHER_BLIP_TIFF:
		{
			if		(0x06E4 == oHeader.RecInstance)	lOffset = 17;
			else if (0x06E5 == oHeader.RecInstance)	lOffset = 33;

			StreamUtils::StreamSkip(lOffset, pStream);
			
			sExt = L".tif";
			break;
		}
		default:
		{
			break;
		}
	}
	int nImagesCount = 0;
	if (m_pDocumentInfo)
	{
		nImagesCount = m_pDocumentInfo->m_mapStoreImageFile.size();
	}
	//else nImagesCount = generate uniq name

	if (oMetaFile.m_bIsValid)
	{
		std::wstring strFile = L"Image " +std::to_wstring(nImagesCount + 1) + oMetaFile.m_sExtension;

		NSFile::CFileBinary fileMeta;
	
		if (fileMeta.CreateFileW(m_pCommonInfo->tempPath + FILE_SEPARATOR_STR + strFile))
		{
			oMetaFile.ToFile(&fileMeta);
			fileMeta.CloseFile();
		}
		m_fileName = strFile;
	}
	else if (oHeader.RecType == RECORD_TYPE_ESCHER_BLIP_PICT)
	{
		unsigned char* newData = NULL;
		unsigned int newDataSize = oMetaFile.ToBuffer(newData);

		std::wstring strFile = L"Image " + std::to_wstring(nImagesCount + 1) + sExt;
		
		NSFile::CFileBinary fileImage;
		if (newData && fileImage.CreateFileW(m_pCommonInfo->tempPath + FILE_SEPARATOR_STR + strFile))
		{
			fileImage.WriteFile(newData, newDataSize);
			fileImage.CloseFile();
		}
		if (newData)
			delete[]newData;
		try
		{
			CBgraFrame bgraFrame;
			if (bgraFrame.OpenFile(m_pCommonInfo->tempPath + FILE_SEPARATOR_STR + strFile))
			{
				NSFile::CFileBinary::Remove(m_pCommonInfo->tempPath + FILE_SEPARATOR_STR + strFile);

				strFile = L"Image " + std::to_wstring(nImagesCount + 1) + L".png";
				bgraFrame.SaveFile(m_pCommonInfo->tempPath + FILE_SEPARATOR_STR + strFile, 4); // png
			}
		}
		catch (...)
		{
		}
		m_fileName = strFile;
	}
	else
	{
		BYTE* pImage = new BYTE[oHeader.RecLen - lOffset]; 

		pStream->read(pImage, oHeader.RecLen - lOffset); 
		if (pDecryptor)
		{
			pDecryptor->Decrypt((char*)pImage, oHeader.RecLen - lOffset, 0);
		}
		
		size_t lOffset2 = 0;
		if (oHeader.RecType == RECORD_TYPE_ESCHER_BLIP_PNG)
		{
			std::string test((char*)pImage, (std::min)((int)oHeader.RecLen - lOffset, 4096));
			if (std::string::npos != (lOffset2 = test.find("GIF89")))
			{//gif in png chuncks - todooo from read header, chunks ....
				sExt = L".gif";
				lOffset += lOffset2;
			}
			else lOffset2 = 0;
		}

		std::wstring strFile = L"Image " + std::to_wstring(nImagesCount + 1) + sExt;
		
		NSFile::CFileBinary fileImage;

		if (fileImage.CreateFileW(m_pCommonInfo->tempPath + FILE_SEPARATOR_STR + strFile))
		{
			if (RECORD_TYPE_ESCHER_BLIP_DIB == oHeader.RecType)
			{
				WORD vtType = 0x4D42;
				fileImage.WriteFile((BYTE*)&vtType, 2);
				_UINT32 dwLen = oHeader.RecLen - lOffset;
				fileImage.WriteFile((BYTE*)&dwLen, 4);
				_UINT32 dwRes = 0;
				fileImage.WriteFile((BYTE*)&dwRes, 4);
				_UINT32 dwOffset = 2;
				fileImage.WriteFile((BYTE*)&dwOffset, 4);
			}
			fileImage.WriteFile(pImage + lOffset2, oHeader.RecLen - lOffset);
			fileImage.CloseFile();
		}
		if (pImage)delete[] pImage;
		pImage = NULL;	

		m_fileName = strFile;
	}
}

CRecordBitmapBlip::CRecordBitmapBlip()
{

}

CRecordBitmapBlip::~CRecordBitmapBlip()
{

}

void CRecordBitmapBlip::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    return CUnknownRecord::ReadFromStream(oHeader, pStream);
}
