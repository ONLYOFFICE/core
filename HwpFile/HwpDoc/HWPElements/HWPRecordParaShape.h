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

#ifndef HWPRECORDPARASHAPE_H
#define HWPRECORDPARASHAPE_H

#include "HWPRecord.h"
#include "../HWPDocInfo.h"
#include "../Common/XMLReader.h"

namespace HWP
{
enum class EHeadingType
{
	NONE,
	OUTLINE,
	NUMBER,
	BULLET
};

enum class EHorizontalAlign
{
	JUSTIFY,
	LEFT,
	RIGHT,
	CENTER,
	DISTRIBUTE,
	DISTRIBUTE_SPACE
};

enum class EVerticalAlign
{
	BASELINE,
	TOP,
	CENTER,
	BOTTOM
};

class CHWPRecordParaShape : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	EHorizontalAlign m_eAlign;
	HWP_BYTE m_chBreakLatinWord;
	HWP_BYTE m_chBreakNonLatinWord;
	bool m_bSnapToGrid;
	HWP_BYTE m_chCondense;
	bool m_bWidowOrphan;
	bool m_bKeepWithNext;
	bool m_bPageBreakBefore;
	EVerticalAlign m_eVertAlign;
	bool m_bFontLineHeight;
	EHeadingType m_eHeadingType;

	HWP_BYTE m_chHeadingLevel;
	bool m_bConnect;
	bool m_bIgnoreMargin;
	bool m_bParaTailShape;

	int m_nIndent;
	int m_nMarginLeft;
	int m_nMarginRight;
	int m_nMarginPrev;
	int m_nMarginNext;
	int m_nLineSpacing;

	short m_shTabDef;
	short m_shHeadingIdRef;
	short m_shBorderFill;
	short m_shOffsetLeft;
	short m_shOffsetRight;
	short m_shOffsetTop;
	short m_shOffsetBottom;

	HWP_BYTE m_chLineWrap;
	bool m_bAutoSpaceEAsianEng;
	bool m_bAutoSpaceEAsianNum;

	int m_nLineSpacingType;
	void RecursiveParaShape(CXMLReader& oReader);
public:
	CHWPRecordParaShape(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordParaShape(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType);

	EHorizontalAlign GetHorizantalAlign() const;
	EVerticalAlign GetVerticalAlign() const;
	EHeadingType GetHeadingType() const;
	HWP_BYTE GetHeadingLevel() const;
	short GetHeadingIdRef() const;
	short GetBorderFillIdRef() const;
	bool GetPageBreakBefore() const;
	int GetLineSpacingType() const;
	int GetLineSpacing() const;
	int GetMarginPrev() const;
	int GetMarginNext() const;

	int GetIndent() const;
	int GetLeftIndent() const;
	int GetRightIndent() const;

	short GetTabDef() const;

	bool KeepWithNext() const;
};
}

#endif // HWPRECORDPARASHAPE_H
