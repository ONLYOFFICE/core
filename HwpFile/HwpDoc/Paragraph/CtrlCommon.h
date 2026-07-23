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

#ifndef CTRLCOMMON_H
#define CTRLCOMMON_H

#include "../HWPStream.h"
#include "CapParagraph.h"
#include "Ctrl.h"

namespace HWP
{

enum class EVRelTo
{
	PAPER,
	PAGE,
	PARA
};

enum class EHRelTo
{
	PAPER,
	PAGE,
	COLUMN,
	PARA
};

enum class EWidthRelTo
{
	PAPER,
	PAGE,
	COLUMN,
	PARA,
	ABSOLUTE
};

enum class EHeightRelTo
{
	PAPER,
	PAGE,
	ABSOLUTE
};

enum class EVertAlign
{
	TOP,
	CENTER,
	BOTTOM,
	INSIDE,
	OUTSIDE
};

EVertAlign GetVertAlign(int nValue);
EVertAlign GetVertAlign(const std::string& sValue, EHanType eType);

enum class EHorzAlign
{
	LEFT,
	CENTER,
	RIGHT,
	INSIDE,
	OUTSIDE
};

enum class ETextWrap
{
	SQUARE,
	TOP_AND_BOTTOM,
	BEHIND_TEXT,
	IN_FRONT_OF_TEXT
};

class CCtrlCommon : public CCtrl
{
	int m_nObjAttr;
	bool m_bTreatAsChar;
	bool m_bAffectLSpacing;
	EVRelTo m_eVertRelTo;
	EVertAlign m_eVertAlign;
	EHRelTo m_eHorzRelTo;
	EHorzAlign m_eHorzAlign;
	bool m_bFlowWithText;
	bool m_bAllowOverlap;
	EWidthRelTo m_eWidthRelTo;
	EHeightRelTo m_eHeightRelTo;
	ETextWrap m_eTextWrap;
	HWP_BYTE m_chTextFlow;
	HWP_BYTE m_chNumeringType;

	int m_nVertOffset;
	int m_nHorzOffset;
	int m_nWidth;
	int m_nHeight;
	int m_nZOrder;
	short m_arOutMargin[4];
	short m_arInMargin [4];
	int m_nObjInstanceID;
	int m_nBlockPageBreak;
	HWP_STRING m_sObjDesc;

	VECTOR<CHWPPargraph*> m_arParas;
	int m_nCaptionAttr;
	int m_nCaptionWidth;
	int m_nCaptionSpacing;
	int m_nCaptionMaxW;
	VECTOR<CCapParagraph*> m_arCaption;

	EVertAlign m_eTextVerAlign;

	friend class CCtrlGeneralShape;
	friend class CCtrlShapeVideo;
	friend class CCtrlEqEdit;
	friend class CCtrlTable;

	void ReadAttributes(CXMLReader& oReader, EHanType eType);
public:
	CCtrlCommon();
	CCtrlCommon(const HWP_STRING& sCtrlID);
	CCtrlCommon(const CCtrlCommon& oCtrlCommon);
	CCtrlCommon(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlCommon(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType);
	virtual ~CCtrlCommon();

	ECtrlObjectType GetCtrlType() const override;

	void SetTextVerAlign(EVertAlign eVertAlign);

	void ParseChildren(CXMLReader& oReader, EHanType eType);

	void AddParagraph(CHWPPargraph* pParagraph);
	void AddCaption(CCapParagraph* pCapPara);

	CHWPPargraph* GetLastPara();
	unsigned int GetCountParagraphs() const;
	const CHWPPargraph* GetParagraphs(unsigned int unIndex) const;

	short GetLeftOutMargin() const;
	short GetTopOutMargin() const;
	short GetRightOutMargin() const;
	short GetBottomOutMargin() const;

	short GetLeftInMargin() const;
	short GetTopInMargin() const;
	short GetRightInMargin() const;
	short GetBottomInMargin() const;

	bool GetTreatAsChar() const;
	bool GetFlowWithText() const;
	int GetHorzOffset() const;
	int GetVertOffset() const;
	EVRelTo GetVertRelTo() const;
	EHRelTo GetHorzRelTo() const;
	int GetCaptionWidth() const;
	bool CaptionsEmpty() const;
	HWP_STRING GetDesc() const;
	EWidthRelTo GetWidthRelTo() const;
	EHeightRelTo GetHeightRelTo() const;
	int GetWidth() const;
	int GetHeight() const;
	ETextWrap GetTextWrap() const;
	HWP_BYTE GetTextFlow() const;
	int GetZOrder() const;

	bool HaveCaption() const;
	VECTOR<const CCapParagraph*> GetCaptionParas() const;

	static int ParseCtrl(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCaption(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLCOMMON_H
