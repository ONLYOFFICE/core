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

#ifndef HWPPARGRAPH_H
#define HWPPARGRAPH_H

#include "LineSeg.h"
#include "RangeTag.h"
#include "Ctrl.h"
#include "../Common/XMLReader.h"
#include "../HanType.h"

namespace HWP
{
enum class EParagraphType
{
	Normal,
	Cap,
	Cell
};

class CHWPPargraph : public IRef
{
	short m_shParaShapeID; // HWPTAG_PARA_HEADER
	short m_shParaStyleID; // HWPTAG_PARA_HEADER
	HWP_BYTE m_chBreakType;    // HWPTAG_PARA_HEADER

	CLineSeg *m_pLineSegs; // HWPTAG_PARA_LINE_SEG
	VECTOR<TRangeTag> m_arRangeTags;  // HWPTAG_PARA_RANGE_TAG

	VECTOR<CCtrl*> m_arP; //HWPTAG_PARA_TEXT

	bool ParseHWPParagraph(CXMLReader& oReader, int nCharShapeID, EHanType eType);
public:
	CHWPPargraph();
	CHWPPargraph(CXMLReader& oReader, EHanType eType);
	virtual ~CHWPPargraph();

	virtual EParagraphType GetType() const;

	void SetLineSeg(CLineSeg* pLineSeg);

	void AddRangeTag(const TRangeTag& oRangeTag);

	void AddCtrl(CCtrl* pCtrl);
	void AddCtrls(const LIST<CCtrl*>& arCtrls);

	bool SetCtrl(CCtrl* pCtrl, unsigned int unIndex);

	VECTOR<CCtrl*>& GetCtrls();
	VECTOR<const CCtrl*> GetCtrls() const;

	unsigned int GetCountCtrls() const;
	short GetShapeID() const;
	short GetStyleID() const;
	HWP_BYTE GetBreakType() const;

	const CLineSeg* GetLineSeg() const;

	VECTOR<TRangeTag> GetRangeTags() const;

	static CHWPPargraph* Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int Parse(CHWPPargraph& oPara, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	CCtrl* FindFirstElement(const HWP_STRING& sID, bool bFullfilled, unsigned int& nIndex) const;
	CCtrl* FindLastElement(const HWP_STRING& sID);

	int IndexOf(CCtrl* pCtrl);
};
}

#endif // HWPPARGRAPH_H
