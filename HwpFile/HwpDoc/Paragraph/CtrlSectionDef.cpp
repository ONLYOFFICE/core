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

#include "CtrlSectionDef.h"

namespace HWP
{
CCtrlSectionDef::CCtrlSectionDef(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID), m_pPage(nullptr)
{}

CCtrlSectionDef::CCtrlSectionDef(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID), m_pPage(nullptr)
{
	int nAttr;
	oBuffer.ReadInt(nAttr);

	m_bHideHeader = CHECK_FLAG(nAttr, 0x01);
	m_bHideFooter = CHECK_FLAG(nAttr, 0x02);
	m_bHideMasterPage = CHECK_FLAG(nAttr, 0x04);
	m_bHideBorder = CHECK_FLAG(nAttr, 0x08);
	m_bHideFill = CHECK_FLAG(nAttr, 0x10);
	m_bHidePageNumPos = CHECK_FLAG(nAttr, 0x20);
	m_bShowFirstBorder = CHECK_FLAG(nAttr, 0x100);
	m_bShowFirstFill = CHECK_FLAG(nAttr, 0x200);
	m_chTextDirection = (HWP_BYTE)((nAttr >> 16) & 0x07);
	m_bHideEmptyLine = CHECK_FLAG(nAttr, 0x20000);
	m_chPageStartOn = (HWP_BYTE)((nAttr >> 20) & 0x03);

	oBuffer.ReadShort(m_shSpaceColumns);
	oBuffer.ReadShort(m_shLineGrid);
	oBuffer.ReadShort(m_shCharGrid);
	oBuffer.ReadInt(m_nTabStop);
	m_nOutlineNumberingID = oBuffer.ReadShort();
	oBuffer.ReadShort(m_shPageNum);
	oBuffer.ReadShort(m_shFigure);
	oBuffer.ReadShort(m_shTable);
	oBuffer.ReadShort(m_shEquation);

	if (nVersion >= 5015)
		oBuffer.ReadShort(m_shLang);

	m_bFullFilled = true;
}

CCtrlSectionDef::CCtrlSectionDef(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
	: CCtrl(sCtrlID), m_pPage(nullptr)
{
	std::string sType;

	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::TextDirection, eType) == sAttributeName)
		{
			if (EHanType::HWPX == eType)
			{
				sType = oReader.GetTextA();

				if ("HORIZONTAL" == sType)
					m_chTextDirection = 0;
				else if ("VERTICAL" == sType)
					m_chTextDirection = 1;
			}
			else
				m_chTextDirection = oReader.GetInt();
		}
		else if (GetAttributeName(EAttribute::SpaceColumns, eType) == sAttributeName)
			m_shSpaceColumns = oReader.GetInt();
		else if (GetAttributeName(EAttribute::TabStop, eType) == sAttributeName)
			m_nTabStop = oReader.GetInt();
		else if (GetAttributeName(EAttribute::OutlineShapeId, eType) == sAttributeName)
			m_nOutlineNumberingID = oReader.GetInt();
		else if (EHanType::HWPML == eType && "LineGrid" == sAttributeName)
			m_shLineGrid = oReader.GetInt();
		else if (EHanType::HWPML == eType && "CharGrid" == sAttributeName)
			m_shCharGrid = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if (GetNodeName(ENode::StartNumber, eType) == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::PageStartsOn, eType) == sAttributeName)
				{
					sType = oReader.GetTextA();

					if (GetValueName(EValue::Both, eType) == sType)
						m_chPageStartOn = 0;
					else if (GetValueName(EValue::Even, eType) == sType)
						m_chPageStartOn = 1;
					else if (GetValueName(EValue::Odd, eType) == sType)
						m_chPageStartOn = 2;
				}
				else if (GetAttributeName(EAttribute::Page, eType) == sAttributeName)
					m_shPageNum = oReader.GetInt();
				else if (GetAttributeName(EAttribute::Figure, eType) == sAttributeName)
					m_shFigure = oReader.GetInt();
				else if (GetAttributeName(EAttribute::Table, eType) == sAttributeName)
					m_shTable = oReader.GetInt();
				else if (GetAttributeName(EAttribute::Equation, eType) == sAttributeName)
					m_shEquation = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if (EHanType::HWPX == eType && "hp:grid" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("lineGrid" == sAttributeName)
					m_shLineGrid = oReader.GetInt();
				else if ("charGrid" == sAttributeName)
					m_shCharGrid = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if (GetNodeName(ENode::Hide, eType) == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::HideHeader, eType) == sAttributeName)
					m_bHideHeader = oReader.GetBool();
				else if (GetAttributeName(EAttribute::HideFooter, eType) == sAttributeName)
					m_bHideFooter = oReader.GetBool();
				else if (GetAttributeName(EAttribute::HideMasterPage, eType) == sAttributeName)
					m_bHideMasterPage = oReader.GetBool();
				else if (GetAttributeName(EAttribute::HideBorder, eType) == sAttributeName)
				{
					if (EHanType::HWPX == eType)
					{
						sType = oReader.GetTextA();

						if ("HIDE_FIRST" == sType)
						{
							m_bHideBorder = true;
							m_bShowFirstBorder = false;
						}
						else if ("SHOW_FIRST" == sType)
						{
							m_bHideBorder = true;
							m_bShowFirstBorder = true;
						}
						else if ("SHOW_ALL" == sType)
						{
							m_bHideBorder = false;
							m_bShowFirstBorder = false;
						}
					}
					else
					{
						m_bHideBorder = oReader.GetBool();
						m_bShowFirstBorder = m_bHideBorder;
					}
				}
				else if (GetAttributeName(EAttribute::HideFill, eType) == sAttributeName)
				{
					if (EHanType::HWPX == eType)
					{
						sType = oReader.GetTextA();

						if ("HIDE_FIRST" == sType)
						{
							m_bHideFill = true;
							m_bShowFirstFill = false;
						}
						else if ("SHOW_FIRST" == sType)
						{
							m_bHideFill = true;
							m_bShowFirstFill = true;
						}
						else if ("SHOW_ALL" == sType)
						{
							m_bHideFill = false;
							m_bShowFirstFill = false;
						}
					}
					else
					{
						m_bHideFill = oReader.GetBool();
						m_bShowFirstFill = m_bHideBorder;
					}
				}
				else if (GetAttributeName(EAttribute::HidePageNumPos, eType) == sAttributeName)
					m_bHidePageNumPos = oReader.GetBool();
				else if (GetAttributeName(EAttribute::HideEmptyLine, eType) == sAttributeName)
					m_bHideEmptyLine = oReader.GetBool();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if (GetNodeName(ENode::PagePropertie, eType) == sNodeName)
			m_pPage = new CPage(oReader, eType);
		else if (GetNodeName(ENode::FootNotePropertie, eType) == sNodeName ||
		         GetNodeName(ENode::EndNotePropertie, eType)  == sNodeName)
			m_arNoteShapes.push_back(new CNoteShape(oReader, eType));
		else if (GetNodeName(ENode::PageBorderFill, eType) == sNodeName)
			m_arBorderFills.push_back(new CPageBorderFill(oReader, eType));
		else if (GetNodeName(ENode::MasterPage, eType) == sNodeName)
		{
			//TODO:: Add implementation
		}
	}
	END_WHILE

	m_bFullFilled = true;
}

CCtrlSectionDef::~CCtrlSectionDef()
{
	if (nullptr != m_pPage)
		delete m_pPage;
}

ECtrlObjectType CCtrlSectionDef::GetCtrlType() const
{
	return ECtrlObjectType::SectionDef;
}

void CCtrlSectionDef::SetPage(CPage* pPage)
{
	m_pPage = pPage;
}

void CCtrlSectionDef::AddHeadFoot(CCtrlHeadFoot* pHeadFoot)
{
	m_arHeaderFooter.push_back(pHeadFoot);
}

void CCtrlSectionDef::AddParagraph(CHWPPargraph* pParagraph)
{
	m_arParas.push_back(pParagraph);
}

void CCtrlSectionDef::AddNoteShape(CNoteShape* pNoteShape)
{
	m_arNoteShapes.push_back(pNoteShape);
}

void CCtrlSectionDef::AddPageBorderFill(CPageBorderFill* pPageBorderFill)
{
	m_arBorderFills.push_back(pPageBorderFill);
}

const CPage* CCtrlSectionDef::GetPage() const
{
	return m_pPage;
}

VECTOR<const CCtrlHeadFoot*> CCtrlSectionDef::GetHeaderFooters() const
{
	VECTOR<const CCtrlHeadFoot*> arHeaderFooters(m_arHeaderFooter.size());

	for (unsigned int unIndex = 0; unIndex < m_arHeaderFooter.size(); ++unIndex)
		arHeaderFooters[unIndex] = dynamic_cast<const CCtrlHeadFoot*>(m_arHeaderFooter[unIndex]);

	return arHeaderFooters;
}
}
