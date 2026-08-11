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

#include "HWPXFile.h"

#include <cwctype>

namespace HWP
{
CHWPXFile::CHWPXFile(const HWP_STRING& sFileName)
	: m_sFileName(sFileName), m_pZipFolder(nullptr), m_oDocInfo(this),
	  m_eCryptResult(EHwpxCryptResult::OK)
{}

CHWPXFile::~CHWPXFile()
{
	Close();
	CLEAR_ARRAY(CHWPSection, m_arSections);
}

VECTOR<const CHWPSection*> CHWPXFile::GetSections() const
{
	RETURN_VECTOR_CONST_PTR(CHWPSection, m_arSections);
}

bool CHWPXFile::Detect()
{
	BYTE *pBuffer = NULL;
	DWORD dwBufferSize = 0;

	if (!NSFile::CFileBinary::ReadAllBytes(m_sFileName, &pBuffer, dwBufferSize))
		return false;

	m_pZipFolder = new CZipFolderMemory(pBuffer, dwBufferSize);

	ReadManifest();

	return GetFileHeader();
}

void CHWPXFile::SetPassword(const HWP_STRING& sPassword)
{
	m_oCrypt.SetPassword(sPassword);
}

bool CHWPXFile::Encrypted() const
{
	return m_oCrypt.Encrypted();
}

EHwpxCryptResult CHWPXFile::GetCryptResult() const
{
	return m_eCryptResult;
}

void CHWPXFile::ReadManifest()
{
	CXMLReader oReader;

	if (nullptr == m_pZipFolder || nullptr == oReader.GetReader())
		return;

	if (!m_pZipFolder->getReaderFromFile(L"META-INF/manifest.xml", *oReader.GetReader()) || !oReader.ReadNextNode())
		return;

	m_oCrypt.ParseManifest(oReader);
}

bool CHWPXFile::Open()
{
	if (!NSFile::CFileBinary::Exists(m_sFileName) || !Detect() || !ReadDocInfo())
		return false;

	for (const std::wstring& wsPath : GetPathsToSections())
		ReadSection(wsPath);

	return true;
}

void CHWPXFile::Close()
{
	if (nullptr != m_pZipFolder)
	{
		delete m_pZipFolder;
		m_pZipFolder = nullptr;
	}
}

VECTOR<HWP_STRING> CHWPXFile::GetPathsToSections() const
{
	VECTOR<std::wstring> arPaths{m_pZipFolder->getFiles(L"Contents", false)};

	if (arPaths.empty())
		return VECTOR<HWP_STRING>{};

	struct TPathInfo
	{
		std::wstring m_wsPath;
		unsigned short m_ushIndex;

		bool operator<(const TPathInfo& oPathInfo) const
		{
			return m_ushIndex < oPathInfo.m_ushIndex;
		}
	};

	VECTOR<TPathInfo> arPathsToSectors;
	size_t unFoundPos, unNumberStart, unNumberEnd;

	for (const std::wstring& wsPath : arPaths)
	{
		unFoundPos = wsPath.find(L"/Contents/section");
		if (std::wstring::npos ==unFoundPos)
			continue;

		unNumberStart = unFoundPos + 17;
		unNumberEnd   = unNumberStart;

		while (unNumberEnd < wsPath.length() && iswdigit(wsPath[unNumberEnd]))
			++unNumberEnd;

		if (unNumberEnd == unNumberStart ||
		    unNumberEnd >= wsPath.length() ||
		    L".xml" != wsPath.substr(unNumberEnd, 4))
			continue;

		arPathsToSectors.push_back({wsPath, static_cast<unsigned short>(std::stoi(wsPath.substr(unNumberStart, unNumberEnd - unNumberStart)))});
	}

	std::sort(arPathsToSectors.begin(), arPathsToSectors.end());

	VECTOR<HWP_STRING> arResult{arPathsToSectors.size()};

	for (unsigned int unIndex = 0; unIndex < arPathsToSectors.size(); ++unIndex)
		arResult[unIndex] = arPathsToSectors[unIndex].m_wsPath;

	return arResult;
}

bool CHWPXFile::GetFileHeader()
{
	CXMLReader oReader;

	if (!GetDocument(L"version.xml", oReader))
		return false;

	return m_oFileHeader.Parse(oReader);
}

const CHWPDocInfo* CHWPXFile::GetDocInfo() const
{
	return &m_oDocInfo;
}

bool CHWPXFile::GetChildStream(const HWP_STRING& sFileName, CHWPStream& oBuffer)
{
	if (nullptr == m_pZipFolder || !m_pZipFolder->exists(sFileName))
		return false;

	CZipFolderMemory::CBuffer *pFileBuffer = nullptr;
	m_pZipFolder->read(sFileName, pFileBuffer);

	if (nullptr == pFileBuffer)
		return false;

	if (!m_oCrypt.Encrypted(sFileName))
	{
		pFileBuffer->UnsetDestroy();

		oBuffer.SetStream((HWP_BYTE*)pFileBuffer->Buffer, pFileBuffer->Size);

		delete pFileBuffer;

		return true;
	}

	m_eCryptResult = m_oCrypt.Decrypt(sFileName, (const HWP_BYTE*)pFileBuffer->Buffer, pFileBuffer->Size, oBuffer);

	delete pFileBuffer;

	return EHwpxCryptResult::OK == m_eCryptResult;
}

bool CHWPXFile::ReadDocInfo()
{
	CXMLReader oContentReader;

	if (!GetDocument(L"Contents/content.hpf", oContentReader))
		return false;

	if (m_oDocInfo.ReadContentHpf(oContentReader))
	{
		CXMLReader oHeaderReader;

		if (!GetDocument(L"Contents/header.xml", oHeaderReader))
			return false;

		return m_oDocInfo.ParseHWPX(oHeaderReader);
	}

	return false;
}

bool CHWPXFile::ReadSection(const HWP_STRING& sName)
{
	CXMLReader oReader;

	if (!GetDocument(sName, oReader))
		return false;

	CHWPSection* pSection = new CHWPSection();
	const bool bResult = pSection->Parse(oReader, EHanType::HWPX);

	if (bResult)
		m_arSections.push_back(pSection);
	else
		delete pSection;

	return bResult;
}

bool CHWPXFile::GetDocument(const HWP_STRING& sEntryName, CXMLReader& oReader)
{
	if (nullptr == m_pZipFolder || nullptr == oReader.GetReader())
		return false;

	if (!m_oCrypt.Encrypted(sEntryName))
	{
		if (m_pZipFolder->getReaderFromFile(sEntryName, *oReader.GetReader()))
			return oReader.ReadNextNode();

		return false;
	}

	CHWPStream oStream;

	if (!GetChildStream(sEntryName, oStream))
		return false;

	if (!oReader.GetReader()->FromStringA(std::string(oStream.GetCurPtr(), oStream.GetSize())))
		return false;

	return oReader.ReadNextNode();
}
}
