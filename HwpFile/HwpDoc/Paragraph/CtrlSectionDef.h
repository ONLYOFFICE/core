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

#ifndef CTRLSECTIONDEF_H
#define CTRLSECTIONDEF_H

#include "Ctrl.h"
#include "../Section/Page.h"

#include "CtrlHeadFoot.h"
#include "../Section/NoteShape.h"
#include "../Section/PageBorderFill.h"

namespace HWP
{
class CCtrlSectionDef : public CCtrl
{
	bool m_bHideHeader;
	bool m_bHideFooter;
	bool m_bHideMasterPage;
	bool m_bHideBorder;
	bool m_bHideFill;
	bool m_bHidePageNumPos;
	bool m_bShowFirstBorder;
	bool m_bShowFirstFill;
	HWP_BYTE m_chTextDirection;
	bool m_bHideEmptyLine;
	HWP_BYTE m_chPageStartOn;

	short m_shSpaceColumns;
	short m_shLineGrid;
	short m_shCharGrid;
	int m_nTabStop;
	int m_nOutlineNumberingID;
	short m_shPageNum;
	short m_shFigure;
	short m_shTable;
	short m_shEquation;
	short m_shLang;

	CPage *m_pPage;
	VECTOR<CCtrlHeadFoot*> m_arHeaderFooter;
	VECTOR<CNoteShape*> m_arNoteShapes;
	VECTOR<CPageBorderFill*> m_arBorderFills;

	VECTOR<CHWPPargraph*> m_arParas;

public:
	CCtrlSectionDef(const HWP_STRING& sCtrlID);
	CCtrlSectionDef(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlSectionDef(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType);
	~CCtrlSectionDef();

	ECtrlObjectType GetCtrlType() const override;

	void SetPage(CPage* pPage);

	void AddHeadFoot(CCtrlHeadFoot* pHeadFoot);
	void AddParagraph(CHWPPargraph* pParagraph);
	void AddNoteShape(CNoteShape* pNoteShape);
	void AddPageBorderFill(CPageBorderFill* pPageBorderFill);

	const CPage* GetPage() const;
	VECTOR<const CCtrlHeadFoot*> GetHeaderFooters() const;
};
}

#endif // CTRLSECTIONDEF_H
