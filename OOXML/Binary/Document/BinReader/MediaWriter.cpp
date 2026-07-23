/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "MediaWriter.h"
#include "../../../../DesktopEditor/common/Path.h"

namespace Writers
{
	MediaWriter::MediaWriter(std::wstring sDir) : m_sDir(sDir)
	{
		nImageCount = 0;

		OOX::CPath filePath = m_sDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + L"media";

		m_sMediaDir = filePath.GetPath();
	}
	std::wstring MediaWriter::AddImageGetNewPath()
	{
		NSDirectory::CreateDirectories(m_sMediaDir);

		std::wstring sNewImgName = L"image" + std::to_wstring(nImageCount + 1) + L".jpg";
		std::wstring sNewImg = m_sMediaDir + FILE_SEPARATOR_STR + sNewImgName;
		nImageCount++;
		return sNewImg;
	}
	void MediaWriter::AddImage2(FILE* pFile)
	{
		long size = ftell(pFile);
		if(size > 0)
		{
			rewind(pFile);
			BYTE* pData = new BYTE[size];
			_UINT32 dwSizeRead = (_UINT32)fread((void*)pData, 1, size, pFile);
			if(dwSizeRead > 0)
			{
				std::wstring sNewImagePath = AddImageGetNewPath();
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(sNewImagePath);
				oFile.WriteFile(pData, dwSizeRead);
				oFile.CloseFile();
				std::wstring sFilename = NSSystemPath::GetFileName(sNewImagePath);
				m_aImageNames.push_back(sFilename);
			}
			RELEASEARRAYOBJECTS(pData);
		}
	}
	void MediaWriter::AddImage(const std::wstring& sImg)
	{
		OOX::CPath pathNewImg = AddImageGetNewPath();

		NSFile::CFileBinary::Copy(sImg, pathNewImg.GetPath());
		std::wstring sFilename = NSSystemPath::GetFileName(pathNewImg.GetPath()).c_str();
		m_aImageNames.push_back(sFilename);
	}
}
