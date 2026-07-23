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
#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include <string>
#include <map>
#include "../common/File.h"
#include "../common/StringBuilder.h"
#include <iostream>

// class for server version (disable local files and etc)
class CServerInstance
{
private:
	bool m_bIsEnabled;
	std::map<std::wstring, bool> m_arMapTmpFiles;

	CServerInstance()
	{
		m_bIsEnabled = false;
	}

public:
	static CServerInstance& getInstance()
	{
		static CServerInstance server;
		return server;
	}

	void Enable(const bool& enabled)
	{
		m_bIsEnabled = enabled;
	}

	bool IsEnable()
	{
		return m_bIsEnabled;
	}

	void AddTmpFile(const std::wstring& sFolder)
	{
		std::wstring sDirectory = sFolder + L"/media";
		NSStringUtils::string_replace(sDirectory, L"\\", L"/");
		std::map<std::wstring, bool>::iterator findDir = m_arMapTmpFiles.find(sDirectory);
		if (findDir == m_arMapTmpFiles.end())
			m_arMapTmpFiles.insert(std::make_pair(sDirectory, true));
	}

	void RemoveTmpFile(const std::wstring& sFolder)
	{
		std::wstring sDirectory = sFolder + L"/media";
		NSStringUtils::string_replace(sDirectory, L"\\", L"/");
		std::map<std::wstring, bool>::iterator findDir = m_arMapTmpFiles.find(sDirectory);
		if (findDir != m_arMapTmpFiles.end())
			m_arMapTmpFiles.erase(findDir);
	}

	bool CheckTmpDirectory(const std::wstring& sFile)
	{
		std::wstring sDirectory = NSFile::GetDirectoryName(sFile);
		NSStringUtils::string_replace(sDirectory, L"\\", L"/");
		std::map<std::wstring, bool>::iterator findDir = m_arMapTmpFiles.find(sDirectory);
		return (findDir != m_arMapTmpFiles.end()) ? true : false;
	}
};


#endif // SERVER_SETTINGS_H
