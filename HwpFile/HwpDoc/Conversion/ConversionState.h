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

#ifndef CONVERSIONSTATE_H
#define CONVERSIONSTATE_H

#include "Types.h"

#include "../Paragraph/CtrlHeadFoot.h"
#include "../Paragraph/CtrlSectionDef.h"
#include "../Paragraph/CtrlPageNumPos.h"
#include "../Paragraph/CtrlNewNumber.h"
#include "../Paragraph/CtrlColumnDef.h"
#include "../Paragraph/CtrlField.h"

namespace HWP
{
struct TConversionState
{
	bool m_bOpenedP;
	bool m_bOpenedR;
	bool m_bIsNote;
	bool m_bInTable;

	struct TLastNode
	{
		unsigned int m_unParaIndex;
		enum class ELastNodeType
		{
			Empty,
			Paragraph,
			Table
		} m_eType;

		TLastNode()
		    : m_unParaIndex(0), m_eType(ELastNodeType::Empty)
		{}

		void Clear()
		{
			m_unParaIndex = 0;
			m_eType = ELastNodeType::Empty;
		}
	} m_oLastNode;

	bool m_bInTextBox; // TODO:: Used to prevent new shape appearing in wps:txbx (needs further investigation)

	unsigned short m_ushLastCharShapeId;
	unsigned short m_ushSecdIndex;
	unsigned int m_unParaIndex;

	VECTOR<const CCtrlHeadFoot*> m_arCtrlsHeadFoot; //only for hwpx
	std::stack<int> m_arOpenedBookmarks;

	const CCtrlSectionDef* m_pSectionDef;
	const CCtrlColumnDef*  m_pColumnDef;
	const CCtrlPageNumPos* m_pPageNum;
	const CCtrlNewNumber*  m_pNewNumber;

	VECTOR<TRelationship>* m_pRelationships;

	enum class EBreakType
	{
		Page,
		Column,
		TextWrapping,
		None
	} m_eBreakType;

	std::map<unsigned int, const CCtrlField*> m_mOpenField;

	TConversionState()
	    : m_bOpenedP(false), m_bOpenedR(false), m_bIsNote(false), m_bInTable(false), m_bInTextBox(false), m_ushLastCharShapeId(-1), m_ushSecdIndex(0), m_unParaIndex(0),
	      m_pSectionDef(nullptr), m_pColumnDef(nullptr), m_pPageNum(nullptr), m_pNewNumber(nullptr), m_pRelationships(nullptr), m_eBreakType(EBreakType::None)
	{}
};
}

#endif // CONVERSIONSTATE_H
