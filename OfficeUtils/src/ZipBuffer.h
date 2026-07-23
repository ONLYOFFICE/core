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
#ifndef _ZIPBUFFER_H_
#define _ZIPBUFFER_H_

#include <string>
#include <vector>

#include "../../Common/kernel_config.h"
#include "../../DesktopEditor/common/Types.h"

// Works with an archive in memory
class KERNEL_DECL CZipBuffer
{
private:
	// Archived data
	BYTE* m_zipFile;
	// Length of archived data
	DWORD m_sizeZip;
	// File
	struct CFile
	{
		// Relative path of the file in the archive
		std::string m_sPath;
		// File data
		BYTE* m_pData;
		// File length
		DWORD m_nLength;
		CFile(const std::string& sPath, BYTE* pData, DWORD nLength) : m_sPath(sPath), m_pData(pData), m_nLength(nLength) {}
	};
	// Vector of files in the archive
	std::vector<CFile> m_arrFiles;

	friend class CZipFolderMemory;
public:
	// Creates an archive in memory
	CZipBuffer()
	{
		create();
	}
	// Opens an archive in memory, the passed data must be freed after class usage
	CZipBuffer(BYTE* buffer, DWORD size)
	{
		open(buffer, size);
	}
	// Closes the archive and frees memory
	~CZipBuffer()
	{
		close();
	}

	// Creates an archive in memory
	void create();
	// Opens an archive in memory, the passed data must be freed after class usage
	void open(BYTE* buffer, DWORD size);
	// Closes the archive and frees memory
	void close();
	// Moves a file within the archive
	void move(const std::string& sSrc, const std::string& sDst);

	// Returns a vector of paths in the archive
	std::vector<std::string> getPaths();
	// Saves the archive to the passed memory, the returned data must be freed
	void save(BYTE*& data, DWORD& length);
	// Returns a file by relative path in the archive, the returned data will be freed after class usage
	void getFile(const std::string& sPath, BYTE*& data, DWORD& length);
	// Adds a file by relative path in the archive, the passed data will be freed after class usage
	void addFile   (const std::string& sPath, BYTE* data, DWORD length);
	// Removes a file by relative path in the archive
	bool removeFile(const std::string& sPath);
};

#endif // _ZIPBUFFER_H_
