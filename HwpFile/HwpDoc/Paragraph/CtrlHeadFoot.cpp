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

#include "CtrlHeadFoot.h"

namespace HWP
{
EPageRange GetPageRange(int nValue)
{
	switch(static_cast<EPageRange>(nValue))
	{
		case EPageRange::BOTH: default: return EPageRange::BOTH;
		case EPageRange::EVEN: return EPageRange::EVEN;
		case EPageRange::ODD: return EPageRange::ODD;
	}
}

CCtrlHeadFoot::CCtrlHeadFoot(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlHeadFoot::CCtrlHeadFoot(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion, bool bIsHeader)
	: CCtrl(sCtrlID), m_bIsHeader(bIsHeader)
{
	oBuffer.ReadInt(m_nAttr);
	m_eWhichPage = GetPageRange(m_nAttr & 0x03);
	oBuffer.ReadInt(m_nSerialInSec);
}

CCtrlHeadFoot::CCtrlHeadFoot(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
	: CCtrl(sCtrlID)
{
	m_bIsHeader = L"daeh" == sCtrlID;

	switch (eType)
	{
		case EHanType::HWPX: ReadFromHWPX(oReader); break;
		case EHanType::HWPML:ReadFromHWPML(oReader); break;
		default: break;
	}

	m_bFullFilled = true;
}

void CCtrlHeadFoot::ReadFromHWPX(CXMLReader &oReader)
{
	m_eWhichPage = GetPageRange(oReader.GetAttributeInt("applyPageType"));

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hp:subList")
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if ("hasNumRef" == sAttributeName)
				m_chRefLevelNum = (HWP_BYTE)oReader.GetInt();
			else if ("hasTextRef" == sAttributeName)
				m_chRefLevelText = (HWP_BYTE)oReader.GetInt();
			else if ("textHeight" == sAttributeName)
				m_nTextHeight = oReader.GetInt();
			else if ("textWidth" == sAttributeName)
				m_nTextWidth = oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)

		WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Child, "hp:p")
			m_arParas.push_back(new CHWPPargraph(oReader, EHanType::HWPX));
		END_WHILE
	}
	END_WHILE
}

void CCtrlHeadFoot::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("ApplyPageType" == sAttributeName)
		{
			const std::string sValue{oReader.GetTextA()};

			if ("Both" == sValue)
				m_eWhichPage = EPageRange::BOTH;
			else if ("Even" == sValue)
				m_eWhichPage = EPageRange::EVEN;
			else if ("Odd" == sValue)
				m_eWhichPage = EPageRange::ODD;
		}
		else if ("TextHeight" == sAttributeName)
			m_nTextHeight = oReader.GetInt();
		else if ("TextWidth" == sAttributeName)
			m_nTextWidth = oReader.GetInt();
		//TODO:: Implement HasTextRef and HasNumRef
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "PARALIST")
		WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Child, "P")
			m_arParas.push_back(new CHWPPargraph(oReader, EHanType::HWPML));
		END_WHILE
	END_WHILE
}

ECtrlObjectType CCtrlHeadFoot::GetCtrlType() const
{
	return ECtrlObjectType::HeadFoot;
}

bool CCtrlHeadFoot::IsHeader() const
{
	return m_bIsHeader;
}

void CCtrlHeadFoot::AddParagraph(CHWPPargraph* pParagraph)
{
	m_arParas.push_back(pParagraph);
}

VECTOR<const CHWPPargraph*> CCtrlHeadFoot::GetParagraphs() const
{
	VECTOR<const CHWPPargraph*> arParagraphs(m_arParas.size());

	for (unsigned int unIndex = 0; unIndex < m_arParas.size(); ++unIndex)
		arParagraphs[unIndex] = dynamic_cast<const CHWPPargraph*>(m_arParas[unIndex]);

	return arParagraphs;
}

int CCtrlHeadFoot::ParseListHeaderAppend(CCtrlHeadFoot& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.Skip(2);
	oBuffer.ReadInt(oObj.m_nTextWidth);
	oBuffer.ReadInt(oObj.m_nTextHeight);
	oBuffer.ReadByte(oObj.m_chRefLevelText);
	oBuffer.ReadByte(oObj.m_chRefLevelNum);

	if (nSize - oBuffer.GetDistanceToLastPos())
		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos());

	oObj.m_bFullFilled = true;

	return oBuffer.GetDistanceToLastPos(true);
}
}
