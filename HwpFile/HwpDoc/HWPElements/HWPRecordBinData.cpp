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

#include "HWPRecordBinData.h"

#include "../HWPFile.h"
#include "../Common/NodeNames.h"

#include <iomanip>
#include <regex>
#include <sstream>

namespace HWP
{
ECompressed GetCompressed(int nValue)
{
	SWITCH(ECompressed, nValue)
	{
		DEFAULT(ECompressed::FOLLOW_STORAGE);
		CASE(ECompressed::COMPRESS);
		CASE(ECompressed::NO_COMPRESS);
	}
}

EType GetType(int nValue)
{
	SWITCH(EType, nValue)
	{
		DEFAULT(EType::LINK);
		CASE(EType::EMBEDDING);
		CASE(EType::STORAGE);
	}
}

EState GetState(int nValue)
{
	SWITCH(EState, nValue)
	{
		DEFAULT(EState::NEVER_ACCESSED);
		CASE(EState::FOUND_FILE_BY_ACCESS);
		CASE(EState::ACCESS_FAILED);
		CASE(EState::LINK_ACCESS_IGNORED);
	}
}

CHWPRecordBinData::CHWPRecordBinData(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize)
{
	oBuffer.SavePosition();

	short shTypeBits;
	oBuffer.ReadShort(shTypeBits);

	m_eType = ::HWP::GetType(shTypeBits & 0x0F);
	m_eCompressed = ::HWP::GetCompressed(shTypeBits & 0x30);
	m_eState = GetState(shTypeBits & 0x300);

	if (EType::LINK == m_eType)
	{
		oBuffer.ReadString(m_sAPath, EStringCharacter::UTF16);
		oBuffer.ReadString(m_sRPath, EStringCharacter::UTF16);
	}
	if (EType::EMBEDDING == m_eType || EType::STORAGE == m_eType)
	{
		oBuffer.ReadShort(m_shBinDataID);
		m_sItemID = TO_HWP_STRING(m_shBinDataID);
	}
	if (EType::EMBEDDING == m_eType || EType::STORAGE == m_eType)
	{
		oBuffer.ReadString(m_sFormat, EStringCharacter::UTF16);

		std::wostringstream oStringStream;
		oStringStream << L"BIN" << std::setw(4) << std::setfill(L'0') << std::hex << m_shBinDataID << L"." << m_sFormat;
		m_sAPath = oStringStream.str();
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
}

CHWPRecordBinData::CHWPRecordBinData(CXMLReader& oReader, EHanType eType)
	: CHWPRecord(EHWPTag::HWPTAG_BIN_DATA, 0, 0)
{
	HWP_STRING sSubPath;

	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::BinData, eType) == sAttributeName)
			m_sItemID = oReader.GetText();
		else if (EHanType::HWPX == eType && "isEmbeded" == sAttributeName)
		{
			const std::string sType = oReader.GetTextA();

			if ("0" == sType)
				m_eType = EType::LINK;
			else if ("1" == sType)
				m_eType = EType::EMBEDDING;
		}
		else if (EHanType::HWPML == eType && "Type" == sAttributeName)
		{
			const std::string sType = oReader.GetTextA();

			if ("Link" == sType)
				m_eType = EType::LINK;
			else if ("Embedding" == sType)
				m_eType = EType::EMBEDDING;
			else if ("Storage" == sType)
				m_eType = EType::STORAGE;
		}
		else if (GetAttributeName(EAttribute::Href, eType) == sAttributeName)
			m_sAPath = oReader.GetText();
		else if (GetAttributeName(EAttribute::SubPath, eType) == sAttributeName)
			sSubPath = oReader.GetText();
		else if (GetAttributeName(EAttribute::MediaType, eType) == sAttributeName)
		{
			m_sFormat = oReader.GetText();

			std::wregex oRegex(L"image/(.*)");
			m_sFormat = std::regex_replace(m_sFormat, oRegex, L"$1");
		}
	}
	END_READ_ATTRIBUTES(oReader)

	if (EType::LINK != m_eType || sSubPath.empty())
		return;

	m_sAPath = sSubPath;
}

HWP_STRING CHWPRecordBinData::GetPath() const
{
	return m_sAPath;
}

HWP_STRING CHWPRecordBinData::GetItemID() const
{
	return m_sItemID;
}

HWP_STRING CHWPRecordBinData::GetFormat() const
{
	return m_sFormat;
}

EType CHWPRecordBinData::GetType() const
{
	return m_eType;
}

ECompressed CHWPRecordBinData::GetCompressed() const
{
	return m_eCompressed;
}

short CHWPRecordBinData::GetBinDataID() const
{
	return m_shBinDataID;
}
}
