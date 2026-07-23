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
#include "nativecontrol.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../Common/Network/FileTransporter/include/FileTransporter.h"
#include "server.h"

CImagesWorker::CImagesWorker(const std::wstring& sFolder)
{
	m_sFolder = sFolder;
	std::vector<std::wstring> files = NSDirectory::GetFiles(sFolder);
	m_nIndex = (int)files.size() + 1;
}
std::wstring CImagesWorker::GetImageLocal(const std::wstring& sUrl)
{
	if (CServerInstance::getInstance().IsEnable())
	{
		if (!CServerInstance::getInstance().CheckTmpDirectory(sUrl))
			return L"error";
	}
	std::wstring sExt = NSFile::GetFileExtention(sUrl);
	std::wstring sRet = L"image" + std::to_wstring(m_nIndex++) + L"." + sExt;
	m_mapImages.insert(std::make_pair(sUrl, sRet));
	NSFile::CFileBinary::Copy(sUrl, m_sFolder + L"/" + sRet);
	return sRet;
}
std::wstring CImagesWorker::GetImage(const std::wstring& sUrl)
{
	std::wstring sUrlFile = sUrl;
	if (sUrlFile.find(L"file://") == 0)
	{
		sUrlFile = sUrlFile.substr(7);

		// MS Word copy image with url "file://localhost/..." on mac
		if (sUrlFile.find(L"localhost") == 0)
			sUrlFile = sUrlFile.substr(9);

		NSStringUtils::string_replace(sUrlFile, L"%20", L" ");

		if (!NSFile::CFileBinary::Exists(sUrlFile))
			sUrlFile = sUrlFile.substr(1);
	}

	std::map<std::wstring, std::wstring>::iterator find = m_mapImages.find(sUrlFile);
	if (find != m_mapImages.end())
		return find->second;

	if (NSFile::CFileBinary::Exists(sUrlFile))
		return GetImageLocal(sUrlFile);

	bool bIsNeedDownload = false;
	if (true)
	{
		if (sUrlFile.find(L"www.") != std::wstring::npos)
			bIsNeedDownload = true;
		else if (sUrlFile.find(L"http://") != std::wstring::npos)
			bIsNeedDownload = true;
		else if (sUrlFile.find(L"ftp://") != std::wstring::npos)
			bIsNeedDownload = true;
		else if (sUrlFile.find(L"https://") != std::wstring::npos)
			bIsNeedDownload = true;
	}

	if (bIsNeedDownload)
	{
		NSNetwork::NSFileTransport::CFileDownloader oDownloader(sUrl, false);

		std::wstring sTmpFile = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"IMG");
		if (NSFile::CFileBinary::Exists(sTmpFile))
			NSFile::CFileBinary::Remove(sTmpFile);
		sTmpFile = sTmpFile + L".png";

		oDownloader.SetFilePath(sTmpFile);
		oDownloader.Start(0);
		while ( oDownloader.IsRunned() )
		{
			NSThreads::Sleep( 10 );
		}
		bool bIsDownloaded = oDownloader.IsFileDownloaded();

		if (bIsDownloaded)
		{
			CBgraFrame oFrame;
			oFrame.OpenFile(sTmpFile);

			std::wstring sRet = L"image" + std::to_wstring(m_nIndex++) + L".png";
			m_mapImages.insert(std::make_pair(sUrlFile, sRet));

			oFrame.SaveFile(m_sFolder + L"/media/" + sRet, 4);

			NSFile::CFileBinary::Remove(sTmpFile);
			return sRet;
		}
	}

	if (sUrlFile.find(L"image") == 0 || sUrlFile.find(L"display") == 0)
		return sUrlFile;
	return L"error";
}
