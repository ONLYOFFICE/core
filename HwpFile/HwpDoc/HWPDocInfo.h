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

#ifndef HWPDOCINFO_H
#define HWPDOCINFO_H

#include "HanType.h"
#include "HWPStream.h"
#include "HWPElements/HWPRecord.h"
#include "Common/XMLReader.h"

#include <map>

namespace HWP
{
enum class ECompatDoc
{
	HWP,
	OLD_HWP,
	MS_WORD,
	UNKNOWN
};

class CHWPFile;
class CHWPXFile;
class CHWPMLFile;
class CHWPDocInfo
{
	EHanType    m_eHanType;
	CHWPXFile  *m_pParentHWPX;
	CHWPFile   *m_pParentHWP;
	CHWPMLFile *m_pParentHWPML;
	VECTOR<CHWPRecord*> m_arRecords;

	std::map<HWP_STRING, CHWPRecord*> m_mBinDatas;
	VECTOR<CHWPRecord*> m_arFaseNames;
	VECTOR<CHWPRecord*> m_arBorderFills;
	VECTOR<CHWPRecord*> m_arCharShapes;
	VECTOR<CHWPRecord*> m_arNumberings;
	VECTOR<CHWPRecord*> m_arBullets;
	VECTOR<CHWPRecord*> m_arParaShapes;
	VECTOR<CHWPRecord*> m_arStyles;
	VECTOR<CHWPRecord*> m_arTabDefs;

	ECompatDoc m_eCompatibleDoc;
public:
	CHWPDocInfo(EHanType eHanType);
	CHWPDocInfo(CHWPXFile* pHWPXFile);
	CHWPDocInfo(CHWPFile* pHWPFile);
	CHWPDocInfo(CHWPMLFile* pHWPMLFile);

	~CHWPDocInfo();

	bool Parse(CHWPStream& oBuffer, int nVersion);
	bool ParseHWPX(CXMLReader& oReader);
	bool ParseHWPML(CXMLReader& oReader);
	bool ReadContentHpf(CXMLReader& oReader);

	const CHWPRecord* GetRecord(int nIndex) const;
	const CHWPRecord* GetFaceName(int nIndex) const;
	const CHWPRecord* GetBorderFill(int nIndex) const;
	const CHWPRecord* GetCharShape(int nIndex) const;
	const CHWPRecord* GetNumbering(int nIndex) const;
	const CHWPRecord* GetBullet(int nIndex) const;
	const CHWPRecord* GetParaShape(int nIndex) const;
	const CHWPRecord* GetStyle(int nIndex) const;
	const CHWPRecord* GetTabDef(int nIndex) const;

	CHWPFile* GetParentHWP();

	const CHWPRecord* GetBinData(const HWP_STRING& sID) const;
	EHanType GetHanType() const;
	ECompatDoc GetCompatibleDoc() const;
private:
	bool ReadRefList(CXMLReader& oReader);
	bool ReadRefListElement(CXMLReader& oReader, EHanType eType);
};
}

#endif // HWPDOCINFO_H
