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

#ifndef COMPOUNDFILE_H
#define COMPOUNDFILE_H

#include "Sector.h"
#include "DirectoryEntry.h"
#include "../HWPStream.h"
#include "../../../DesktopEditor/common/File.h"

namespace HWP
{
class CCompoundFile
{
	NSFile::CFileBinary m_oFile;
	int m_nMinorVersion;
	int m_nMajorVersion;
	int m_nSectorSize;
	int m_nShortSectorSize;
	int m_nNumDirectory;    // Support only in version 4
	int m_nNumSAT;
	int m_nFirstSecIDDirectory;
	int m_nMiniStreamCutoffSize;
	int m_nFirstSecIDSSAT;
	int m_nNumSSAT;
	int m_nFirstSecIDMSAT;
	int m_nNumMSAT;

	VECTOR<TSector> m_arSectors;
	VECTOR<int> m_arSATs;
	VECTOR<int> m_arSSATSecIDs;
	VECTOR<int> m_arDirectorySecIDs;
	VECTOR<int> m_arSStreamSecIDs;
	VECTOR<int> m_arSStreams;
	VECTOR<CDirectoryEntry*> m_arDirectoryEntries;
public:
	CCompoundFile(const HWP_STRING& sFileName);
	~CCompoundFile();

	const CDirectoryEntry* GetEntry(const HWP_STRING& sFileName) const;
	bool GetComponent(const HWP_STRING& sEntryName, CHWPStream& oBuffer);

	VECTOR<CDirectoryEntry*> GetChildEntries(const CDirectoryEntry* pBaseEntry) const;
	VECTOR<CDirectoryEntry*> GetChildEntries(const HWP_STRING& sBaseEntryName) const;

	bool Read(const CDirectoryEntry& oEntry, CHWPStream& oBuffer);
	bool Open();
	void Close();
private:
	void AddSiblings(VECTOR<int>& arIndexs, int nCurrentIndex) const;

	VECTOR<int> GetSecIDsFromSAT(int nSecID, int nSatIndex, int nSecIDSSAT);

	void ReadDirectorySector(int nSecID);
	void ReadSSATSector(int nSecID);
	void ReadMSATSector(int nSecID);

	void ParseSectors(int nSecID, CHWPStream& oBuffer);
	void ParseSATSector(int nSecID, CHWPStream& oBuffer);
	void ParseDirectorySector(CHWPStream& oBuffer);
	void ParseSSATSector(CHWPStream& oBuffer);
	void ParseMSATSector(CHWPStream& oBuffer);
	bool ParseHeader(CHWPStream& oBuffer);

	bool CheckSignature(CHWPStream& oBuffer);

	ESectorType LookupSectorType(CHWPStream& oBuffer);
};
}

#endif // COMPOUNDFILE_H
