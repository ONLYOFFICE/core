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

#ifndef HWPFILE_H
#define HWPFILE_H

#include "HwpFileHeader.h"
#include "OLEdoc/CompoundFile.h"
#include "HWPElements/HWPRecordBinData.h"
#include "HWPSection.h"
#include "HWPXCrypt.h"

namespace HWP
{
class CHWPFile
{
	HWP_STRING    m_sFileName;
	CCompoundFile  m_oOleFile;
	CHwpFileHeader m_oFileHeader;
	int m_nVersion;
	CHWPDocInfo       m_oDocInfo;
	VECTOR<CHWPSection*> m_arBodyTexts;
	VECTOR<CHWPSection*> m_arViewTexts;

	HWP_STRING       m_sPassword;
	EHwpxCryptResult m_eCryptResult;
	bool             m_bPasswordDecrypt;
	bool             m_bAesEncrypt;
	HWP_BYTE         m_aDesKey[8];
	HWP_BYTE         m_aAesKey[16];
public:
	CHWPFile(const HWP_STRING& sFileName);
	~CHWPFile();

	VECTOR<const CHWPSection*> GetSections();
	const CCompoundFile* GetOleFile() const;

	void SetPassword(const HWP_STRING& sPassword);
	bool Encrypted() const;
	EHwpxCryptResult GetCryptResult() const;

	bool Detect();
	bool Open();
	void Close();

	bool GetFileHeader();
	const CHWPDocInfo* GetDocInfo() const;
	bool GetDocInfo(int nVersion);
	bool GetComponent(const HWP_STRING& sEntryName, CHWPStream& oBuffer);
	bool GetChildStream(const HWP_STRING& sEntryName, ECompressed eCompressed, CHWPStream& oBuffer);
private:
	CDirectoryEntry* FindChildEntry(const HWP_STRING& sBasePath, const CDirectoryEntry& oBaseEntry, const HWP_STRING& sEntryName) const;
	HWP_STRING SaveChildEntry(const HWP_STRING& sRootPath, const HWP_STRING& sEntryName, ECompressed eCompressed);

	bool Unzip(CHWPStream& oInput, CHWPStream& oBuffer);
	bool Decrypt(CHWPStream& oInput, CHWPStream& oBuffer);

	static bool DerivePasswordKey(const HWP_STRING& sPassword, HWP_BYTE* pKey);
	static bool DeriveAesKey(const HWP_STRING& sPassword, HWP_BYTE* pKey);
	bool DecryptPassword(CHWPStream& oInput, CHWPStream& oBuffer);
	bool DecryptPasswordDes(CHWPStream& oInput, CHWPStream& oBuffer);
	bool DecryptPasswordAes(CHWPStream& oInput, CHWPStream& oBuffer);
	bool VerifyPassword();
	bool DecodeStream(CHWPStream& oRaw, CHWPStream& oOut);

	bool GetBodyText(int nVersion);
	bool GetViewText(int nVersion);
};
}

#endif // HWPFILE_H
