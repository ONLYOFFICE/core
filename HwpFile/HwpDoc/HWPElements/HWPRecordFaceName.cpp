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

#include "HWPRecordFaceName.h"

#include "../Common/NodeNames.h"

namespace HWP
{
EAltType GetAltType(int nValue)
{
	switch(static_cast<EAltType>(nValue))
	{
		case EAltType::UNKNOWN:
		default: return EAltType::UNKNOWN;
		case EAltType::FFT: return EAltType::FFT;
		case EAltType::HFT: return EAltType::HFT;
	}
}

CHWPRecordFaceName::CHWPRecordFaceName(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	HWP_BYTE chDataBits;
	oBuffer.ReadByte(chDataBits);

	m_bBasicFaceExists = CHECK_FLAG(chDataBits, 0x20);
	m_bAttrExists = CHECK_FLAG(chDataBits, 0x40);
	m_bSubstExists = CHECK_FLAG(chDataBits, 0x80);

	oBuffer.ReadString(m_sFaceName, EStringCharacter::UTF16);

	if (m_bSubstExists)
	{
		char chSubsType;
		oBuffer.ReadByte(chSubsType);
		m_eSubstType = GetAltType(chSubsType & 0x0F);

		oBuffer.ReadString(m_sSubstFace, EStringCharacter::UTF16);
	}

	if (m_bAttrExists)
	{
		oBuffer.ReadByte(m_chFamilyType);
		oBuffer.ReadByte(m_chSerifStyle);
		m_shWeight = oBuffer.ReadByte();
		m_shPropotion = oBuffer.ReadByte();
		m_shContrast = oBuffer.ReadByte();
		m_shStrokeVariation = oBuffer.ReadByte();
		m_shArmStyle = oBuffer.ReadByte();
		m_shLetterform = oBuffer.ReadByte();
		m_shMidLine = oBuffer.ReadByte();
		m_shXHeight = oBuffer.ReadByte();
	}

	if (m_bBasicFaceExists)
		oBuffer.ReadString(m_sBasicFaceName, EStringCharacter::UTF16);
}

CHWPRecordFaceName::CHWPRecordFaceName(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType)
	: CHWPRecord(EHWPTag::HWPTAG_FACE_NAME, 0, 0), m_pParent(&oDocInfo)
{
	m_sFaceName = oReader.GetAttribute(GetAttributeName(EAttribute::FaceName, eType));

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if (GetNodeName(ENode::SubFont, eType) == sNodeName)
		{
			m_bSubstExists = true;

			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::FaceName, eType) == sAttributeName)
					m_sSubstFace = oReader.GetText();
				else if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
				{
					std::string sType{oReader.GetTextA()};
					TO_LOWER(sType);

					if ("ttf" == sType)
						m_eSubstType = EAltType::FFT;
					else if ("hft" == sType)
						m_eSubstType = EAltType::HFT;
					else
						m_eSubstType = EAltType::UNKNOWN;
				}
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if (GetNodeName(ENode::TypeInfo, eType) == sNodeName)
		{
			m_bAttrExists = true;

			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::FamilyType, eType) == sAttributeName)
				{
					m_sBasicFaceName = oReader.GetText();

					if (!m_sBasicFaceName.empty())
						m_bBasicFaceExists = true;
				}
				else if (GetAttributeName(EAttribute::SerifStyle, eType) == sAttributeName)
					m_chSerifStyle = (HWP_BYTE)oReader.GetInt();
				else if (GetAttributeName(EAttribute::Weight, eType) == sAttributeName)
					m_shWeight = oReader.GetInt();
				else if (GetAttributeName(EAttribute::Proportion, eType) == sAttributeName)
					m_shPropotion = oReader.GetInt();
				else if (GetAttributeName(EAttribute::Contrast, eType) == sAttributeName)
					m_shContrast = oReader.GetInt();
				else if (GetAttributeName(EAttribute::StrokeVariation, eType) == sAttributeName)
					m_shStrokeVariation = oReader.GetInt();
				else if (GetAttributeName(EAttribute::ArmStyle, eType) == sAttributeName)
					m_shArmStyle = oReader.GetInt();
				else if (GetAttributeName(EAttribute::Letterform, eType) == sAttributeName)
					m_shLetterform = oReader.GetInt();
				else if (GetAttributeName(EAttribute::Midline, eType) == sAttributeName)
					m_shMidLine = oReader.GetInt();
				else if (GetAttributeName(EAttribute::XHeight, eType) == sAttributeName)
					m_shXHeight = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
	}
	END_WHILE
}

HWP_STRING CHWPRecordFaceName::GetFaceName() const
{
	return m_sFaceName;
}
}
