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

#include "CtrlField.h"

#include "../Common/NodeNames.h"

namespace HWP
{
CCtrlField::CCtrlField(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID), m_eType(EFieldType::Unknown)
{
	UpdateType(sCtrlID);
}

CCtrlField::CCtrlField(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID), m_eType(EFieldType::Unknown)
{
	UpdateType(sCtrlID);

	oBuffer.ReadInt(m_nProperty);
	oBuffer.ReadByte(m_chEtcProperty);
	oBuffer.ReadString(m_sCommand, EStringCharacter::UTF16);
	oBuffer.ReadInt(m_nInstanceID);
}

void CCtrlField::UpdateType(const HWP_STRING& sCtrlID)
{
	if (L"klh%" == sCtrlID)
		m_eType = EFieldType::Hyperlink;
	else if (L"klh " == sCtrlID)
		m_eType = EFieldType::HyperlinkClosing;
	else if (L"kmb%" == sCtrlID)
		m_eType = EFieldType::Bookmark;
	else if (L"kmb " == sCtrlID)
		m_eType = EFieldType::BookmarkClosing;
}

void CCtrlField::ReadType(CXMLReader &oReader, EHanType eType)
{
	const std::string sType{oReader.GetTextA()};

	if (GetValueName(EValue::Hyperlink, eType) == sType)
		m_eType = EFieldType::Hyperlink;
	else if (GetValueName(EValue::Bookmark, eType) == sType)
		m_eType = EFieldType::Bookmark;
}

CCtrlField::CCtrlField(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
	: CCtrl(sCtrlID), m_eType(EFieldType::Unknown)
{
	const std::string sNodeName{oReader.GetName()};
	HWP_STRING wsName;

	if (GetNodeName(ENode::FieldBegin, eType) == sNodeName)
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
				ReadType(oReader, eType);
			else if (GetAttributeName(EAttribute::FieldId, eType) == sAttributeName)
				m_nInstanceID = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Name, eType) == sAttributeName)
				wsName = oReader.GetText();
		}
		END_READ_ATTRIBUTES(oReader)

		if (!wsName.empty() && EFieldType::Bookmark == m_eType)
			AddStringParam(L"bookmarkname", wsName);

		// Occurs only in HWPX
		if (EHanType::HWPX == eType)
		{
			WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hp:parameters")
			{
				WHILE_READ_NEXT_NODE_WITH_DEPTH_AND_NAME(oReader, Child)
				{
					if ("hp:stringParam" == sNodeChildName)
						AddStringParam(oReader.GetAttribute("name"), oReader.GetText());
					else if ("hp:integerParam" == sNodeChildName)
						AddIntegerParam(oReader.GetAttribute("name"), oReader.GetInt());
				}
				END_WHILE
			}
			END_WHILE
		}
	}
	else if (GetNodeName(ENode::FieldEnd, eType) == sNodeName)
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
				ReadType(oReader, eType);
			else if (GetAttributeName(EAttribute::FieldId, eType) == sAttributeName)
				m_nInstanceID = oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)
	}
}

ECtrlObjectType CCtrlField::GetCtrlType() const
{
	return ECtrlObjectType::Field;
}

void CCtrlField::AddStringParam(const HWP_STRING& wsName, const HWP_STRING& wsValue)
{
	m_mStringParams.insert(std::make_pair(wsName, wsValue));
}

void CCtrlField::AddIntegerParam(const HWP_STRING& wsName, int nValue)
{
	m_mIntegerParam.insert(std::make_pair(wsName, nValue));
}

HWP_STRING CCtrlField::GetStringParam(const HWP_STRING& wsName) const
{
	std::map<HWP_STRING, HWP_STRING>::const_iterator itFound = m_mStringParams.find(wsName);

	return ((m_mStringParams.cend() != itFound) ? itFound->second : HWP_STRING());
}

int CCtrlField::GetIntegerParam(const HWP_STRING& wsName) const
{
	std::map<HWP_STRING, int>::const_iterator itFound = m_mIntegerParam.find(wsName);

	return ((m_mIntegerParam.cend() != itFound) ? itFound->second : 0);
}

int CCtrlField::GetInstanceID() const
{
	return m_nInstanceID;
}

EFieldType CCtrlField::GetType() const
{
	return m_eType;
}

HWP_STRING CCtrlField::GetCommand() const
{
	return m_sCommand;
}

}
