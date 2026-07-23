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

#ifndef HWPRECORDNUMBERING_H
#define HWPRECORDNUMBERING_H

#include "../HWPDocInfo.h"
#include "../HWPStream.h"
#include "HWPRecord.h"
#include "../Common/XMLReader.h"

namespace HWP
{

struct TParaHeadInfo
{
	HWP_BYTE m_chAlign;
	bool m_bUseInstWidth;
	bool m_bAutoIndent;
	HWP_BYTE m_chTextOffsetType;
	short m_shWidthAdjust;
	short m_shTextOffset;
	int m_nCharShape;
	int m_nStartNumber;
};

struct TNumbering : public TParaHeadInfo
{
	HWP_STRING m_sNumFormat;
};

class CHWPRecordNumbering : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	TNumbering m_arNumbering[7];
	short m_shStart;
	HWP_STRING m_arExtLevelFormat[3];
	int m_arExtLevelStart[3];
public:
	CHWPRecordNumbering(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordNumbering(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType);

	short GetStart() const;
	HWP_STRING GetNumFormat(unsigned short ushIndex) const;
	HWP_BYTE GetAlign(unsigned short ushIndex) const;
	int GetStartNumber(unsigned short ushIndex) const;
	int GetCharShape(unsigned short ushIndex) const;
};
}

#endif // HWPRECORDNUMBERING_H
