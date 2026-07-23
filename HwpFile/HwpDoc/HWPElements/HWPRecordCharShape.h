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

#ifndef HWPRECORDCHARSHAPE_H
#define HWPRECORDCHARSHAPE_H

#include "../HWPDocInfo.h"
#include "../HWPStream.h"
#include "HWPRecord.h"
#include "HwpRecordTypes.h"
#include "../Common/XMLReader.h"

namespace HWP
{
enum class ELang
{
	HANGUL,
	LATIN,
	HANJA,
	JAPANESE,
	OTHER,
	SYMBOL,
	USER,
	MAX
};

enum class EUnderline
{
	NONE,
	BOTTOM,
	CENTER,
	TOP
};

enum class EShadow
{
	NONE,
	DISCRETE,
	CONTINUOUS
};

enum class EAccent
{
	NONE,
	DOT,
	RING,
	CARON,
	TILDE,
	ARAEA,
	TWOARAEA
};

EAccent GetAccent(int nValue);
EAccent GetAccent(const std::string &sValue);

#define MAX_ELEMENTS (int)ELang::MAX

class CHWPRecordCharShape : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	HWP_STRING m_arFontNames[MAX_ELEMENTS];
	short m_arRatios[MAX_ELEMENTS];
	short m_arSpacings[MAX_ELEMENTS];
	short m_arRelSizes[MAX_ELEMENTS];
	short m_arCharOffset[MAX_ELEMENTS];
	int m_nHeight;

	bool m_bItalic;
	bool m_bBold;
	EUnderline m_eUnderline;
	ELineStyle1 m_eUnderLineShape;
	int m_nUnderlineColor;
	ELineStyle3 m_eOutline;
	EShadow m_eShadow;
	bool m_bEmboss;
	bool m_bEngrave;
	bool m_bSuperScript;
	bool m_bSubScript;
	HWP_BYTE m_chStrikeOut;
	EAccent m_eSymMark;
	bool m_bUseFontSpace;
	ELineStyle2 m_eStrikeOutShape;
	bool m_bUseKerning;

	HWP_BYTE m_chShadowOffsetX;
	HWP_BYTE m_chShadowOffsetY;
	int m_nTextColor;
	int m_nShadeColor;
	int m_nShadowColor;
	short m_shBorderFillIDRef;
	int m_nStrikeOutColor;

	void ReadContainerData(CXMLReader& oReader, short arValues[], int nDefaultValue = 0);
public:
	CHWPRecordCharShape(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordCharShape(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType);

	bool Bold() const;
	bool Italic() const;
	bool Underline() const;
	bool StrikeOut() const;
	bool SuperScript() const;
	bool SubScript() const;

	int GetHeight() const;

	EUnderline GetUnderlineType() const;
	ELineStyle1 GetUnderlineStyle() const;
	int GetUnderlineColor() const;

	ELineStyle2 GetStrikeOutType() const;
	int GetStrikeOutColor() const;

	short GetRelSize(ELang eLang) const;
	HWP_STRING GetFontName(ELang eLang) const;
	short GetRatio(ELang eLang) const;
	short GetSpacing(ELang eLang) const;
	int GetTextColor() const;
	int GetShadeColor() const;

	short GetBorderFillID() const;
};
}

#endif // HWPRECORDCHARSHAPE_H
