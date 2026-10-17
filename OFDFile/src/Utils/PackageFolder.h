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

#ifndef PACKAGEFOLDER_H
#define PACKAGEFOLDER_H

#include "../../../OfficeUtils/src/ZipFolder.h"

#include <algorithm>
#include <map>

namespace OFD
{
class CPackageFolder : public IFolder
{
	IFolder*                             m_pFolder;
	std::wstring                         m_wsRootPath;
	std::map<std::wstring, std::wstring> m_mFiles;

	std::wstring GetKey(const std::wstring& wsPath) const
	{
		std::wstring wsKey{wsPath};
		std::replace(wsKey.begin(), wsKey.end(), L'\\', L'/');

		if (!m_wsRootPath.empty() && 0 == wsKey.compare(0, m_wsRootPath.length(), m_wsRootPath) &&
		    (wsKey.length() == m_wsRootPath.length() || L'/' == wsKey[m_wsRootPath.length()]))
			wsKey.erase(0, m_wsRootPath.length());

		wsKey = NSSystemPath::NormalizePath(wsKey, true);

		if (!wsKey.empty() && L'/' == wsKey[0])
			wsKey.erase(0, 1);

		std::transform(wsKey.begin(), wsKey.end(), wsKey.begin(), [](wchar_t wchValue){ return (L'A' <= wchValue && L'Z' >= wchValue) ? (wchar_t)(wchValue + (L'a' - L'A')) : wchValue; });

		return wsKey;
	}

	std::wstring Resolve(const std::wstring& wsPath)
	{
		if (wsPath.empty() || m_pFolder->exists(wsPath))
			return wsPath;

		const std::map<std::wstring, std::wstring>::const_iterator itFound{m_mFiles.find(GetKey(wsPath))};

		return (m_mFiles.cend() != itFound) ? itFound->second : wsPath;
	}
public:
	explicit CPackageFolder(IFolder* pFolder)
		: m_pFolder(pFolder)
	{
		if (nullptr == m_pFolder)
			return;

		m_wsRootPath = m_pFolder->getFullFilePath(L"");
		std::replace(m_wsRootPath.begin(), m_wsRootPath.end(), L'\\', L'/');

		for (const std::wstring& wsFile : m_pFolder->getFiles(L"", true))
			m_mFiles.emplace(GetKey(wsFile), wsFile);
	}

	virtual ~CPackageFolder()
	{
		if (nullptr != m_pFolder)
			delete m_pFolder;
	}

	virtual IFolderType getType() override
	{
		return (nullptr != m_pFolder) ? m_pFolder->getType() : iftFolder;
	}
	virtual std::wstring getFullFilePath(const std::wstring& path) override
	{
		return (nullptr != m_pFolder) ? m_pFolder->getFullFilePath(Resolve(path)) : path;
	}
	virtual std::wstring getLocalFilePath(const std::wstring& path) override
	{
		return (nullptr != m_pFolder) ? m_pFolder->getLocalFilePath(Resolve(path)) : path;
	}
	virtual bool read(const std::wstring& path, CBuffer*& buffer) override
	{
		if (nullptr != m_pFolder)
			return m_pFolder->read(Resolve(path), buffer);

		buffer = nullptr;
		return false;
	}
	virtual void write(const std::wstring& path, BYTE* data, DWORD length) override
	{
		if (nullptr != m_pFolder)
			m_pFolder->write(path, data, length);
	}
	virtual void move(const std::wstring& src, const std::wstring& dst) override
	{
		if (nullptr != m_pFolder)
			m_pFolder->move(src, dst);
	}
	virtual bool exists(const std::wstring& path) override
	{
		return (nullptr != m_pFolder) && m_pFolder->exists(Resolve(path));
	}
	virtual void remove(const std::wstring& path) override
	{
		if (nullptr != m_pFolder)
			m_pFolder->remove(path);
	}
	virtual void createDirectory(const std::wstring& path) override
	{
		if (nullptr != m_pFolder)
			m_pFolder->createDirectory(path);
	}
	virtual void removeDirectory(const std::wstring& path) override
	{
		if (nullptr != m_pFolder)
			m_pFolder->removeDirectory(path);
	}
	virtual std::vector<std::wstring> getFiles(const std::wstring& path, bool recursion) override
	{
		return (nullptr != m_pFolder) ? m_pFolder->getFiles(path, recursion) : std::vector<std::wstring>();
	}
	virtual CBuffer* finalize() override
	{
		return (nullptr != m_pFolder) ? m_pFolder->finalize() : nullptr;
	}
	virtual XmlUtils::CXmlNode getNodeFromFile(const std::wstring& path) override
	{
		return (nullptr != m_pFolder) ? m_pFolder->getNodeFromFile(Resolve(path)) : XmlUtils::CXmlNode();
	}
	virtual bool getReaderFromFile(const std::wstring& path, XmlUtils::CXmlLiteReader& oReader) override
	{
		return (nullptr != m_pFolder) && m_pFolder->getReaderFromFile(Resolve(path), oReader);
	}
};
}

#endif // PACKAGEFOLDER_H
