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

#include "Ctrl.h"

#include "CtrlColumnDef.h"
#include "CtrlAutoNumber.h"
#include "CtrlHeadFoot.h"
#include "CtrlNewNumber.h"
#include "CtrlNote.h"
#include "CtrlPageNumPos.h"
#include "CtrlField.h"

#include "../Common/NodeNames.h"

namespace HWP
{
CCtrl::CCtrl()
	:  m_bFullFilled(false)
{}

CCtrl::CCtrl(const HWP_STRING& sCtrlID)
	: m_sCtrlID(sCtrlID), m_bFullFilled(false)
{}

CCtrl::~CCtrl()
{}

void CCtrl::SetID(const HWP_STRING& sCtrlID)
{
	m_sCtrlID = sCtrlID;
}

HWP_STRING CCtrl::GetID() const
{
	return m_sCtrlID;
}

bool CCtrl::FullFilled() const
{
	return m_bFullFilled;
}

void CCtrl::SetFullFilled()
{
	m_bFullFilled = true;
}

bool CCtrl::Equals(CCtrl* pFirstCtrl, CCtrl* pSecondCtrl)
{
	if (nullptr == pFirstCtrl || nullptr == pSecondCtrl)
		return false;

	return pFirstCtrl->m_sCtrlID == pSecondCtrl->m_sCtrlID &&
	       pFirstCtrl->m_bFullFilled == pSecondCtrl->m_bFullFilled;
}

CCtrl* CCtrl::GetCtrl(CXMLReader& oReader, EHanType eType)
{
	const std::string sNodeName{oReader.GetName()};

	if (GetNodeName(ENode::ColumnDef, eType) == sNodeName)
		return new CCtrlColumnDef(L"dloc", oReader, eType);
	else if (GetNodeName(ENode::Header, eType) == sNodeName)
		return new CCtrlHeadFoot(L"daeh", oReader, eType);
	else if (GetNodeName(ENode::Footer, eType) == sNodeName)
		return new CCtrlHeadFoot(L"toof", oReader, eType);
	else if (GetNodeName(ENode::FootNote, eType) == sNodeName)
		return new CCtrlNote(L"  nf", oReader, eType);
	else if (GetNodeName(ENode::EndNote, eType) == sNodeName)
		return new CCtrlNote(L"  ne", oReader, eType);
	else if (GetNodeName(ENode::AutoNum, eType) == sNodeName)
		return new CCtrlAutoNumber(L"onta", oReader, eType);
	else if (GetNodeName(ENode::NewNum, eType) == sNodeName)
		return new CCtrlNewNumber(L"onwn", oReader, eType);
	else if (GetNodeName(ENode::PageNum, eType) == sNodeName)
		return new CCtrlPageNumPos(L"pngp", oReader, eType);
	else if (GetNodeName(ENode::FieldBegin, eType) == sNodeName ||
	         GetNodeName(ENode::FieldEnd,   eType) == sNodeName)
		return new CCtrlField(L"", oReader, eType);

	return nullptr;
}
}
