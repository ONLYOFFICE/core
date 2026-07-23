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

#ifndef CHWPMLFILE_H
#define CHWPMLFILE_H

#include "Common/XMLReader.h"
#include "HWPSection.h"

namespace HWP
{
class CHWPMLFile
{
	using BinMap  = std::map <HWP_STRING, CHWPStream*>;

	HWP_STRING           m_wsFilePath;
	CHWPDocInfo          m_oDocInfo;
	VECTOR<CHWPSection*> m_arSections;
	BinMap m_mBinDates;

	void ReadHead(CXMLReader& oReader);
	void ReadBody(CXMLReader& oReader);
	void ReadTail(CXMLReader& oReader);
	
	void ReadSection(CXMLReader& oReader);
	void ReadBinData(CXMLReader& oReader);
public:
	CHWPMLFile(const HWP_STRING& wsFilePath);
	~CHWPMLFile();

	bool Open();
	bool Detect() const;
	void Close();

	const CHWPDocInfo* GetDocInfo() const;
	VECTOR<const CHWPSection*> GetSections() const;
	bool GetBinData(const HWP_STRING& sId, CHWPStream& oBuffer) const;
};
}

#endif // CHWPMLFILE_H
