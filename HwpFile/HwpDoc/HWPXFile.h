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

#ifndef HWPXFILE_H
#define HWPXFILE_H

#include "HWPSection.h"
#include "HwpFileHeader.h"
#include "HWPXCrypt.h"

#include "../../OfficeUtils/src/ZipFolder.h"

namespace HWP
{
class CHWPXFile
{
	HWP_STRING m_sFileName;
	CZipFolderMemory *m_pZipFolder;
	CHwpFileHeader m_oFileHeader;
	CHWPDocInfo m_oDocInfo;
	VECTOR<CHWPSection*> m_arSections;
	CHwpxCrypt m_oCrypt;
	EHwpxCryptResult m_eCryptResult;
public:
	CHWPXFile(const HWP_STRING& sFileName);
	~CHWPXFile();

	VECTOR<const CHWPSection*> GetSections() const;
	const CHWPDocInfo* GetDocInfo() const;
	bool GetChildStream(const HWP_STRING& sFileName, CHWPStream& oBuffer);

	void SetPassword(const HWP_STRING& sPassword);
	bool Encrypted() const;
	EHwpxCryptResult GetCryptResult() const;

	bool Detect();
	bool Open();
	void Close();
private:
	VECTOR<HWP_STRING> GetPathsToSections() const;
	bool GetFileHeader();
	void ReadManifest();
	bool ReadDocInfo();
	bool ReadSection(const HWP_STRING& sName);
	bool GetDocument(const HWP_STRING& sEntryName, CXMLReader& oReader);
};
}

#endif // HWPXFILE_H
