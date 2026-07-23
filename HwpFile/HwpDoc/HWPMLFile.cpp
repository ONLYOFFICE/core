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

#include "HWPMLFile.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Base64.h"

namespace HWP
{

CHWPMLFile::CHWPMLFile(const HWP_STRING &wsFilePath)
    : m_wsFilePath(wsFilePath), m_oDocInfo(this)
{}

CHWPMLFile::~CHWPMLFile()
{
	CLEAR_ARRAY(CHWPSection, m_arSections);

	for (BinMap::iterator itBegin = m_mBinDates.begin(); itBegin != m_mBinDates.end(); ++itBegin)
		if (nullptr != itBegin->second)
			delete itBegin->second;
}

bool CHWPMLFile::Open()
{
	if (m_wsFilePath.empty() || !NSFile::CFileBinary::Exists(m_wsFilePath) || !Detect())
		return false;

	CXMLReader oReader;

	oReader.GetReader()->FromFile(m_wsFilePath);
	oReader.ReadNextNode();

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("HEAD" == sNodeName)
			ReadHead(oReader);
		else if ("BODY" == sNodeName)
			ReadBody(oReader);
		else if ("TAIL" == sNodeName)
			ReadTail(oReader);
	}
	END_WHILE

	return true;
}

bool CHWPMLFile::Detect() const
{
	if (m_wsFilePath.empty())
		return false;

	CXMLReader oReader;

	if (nullptr == oReader.GetReader() || !oReader.GetReader()->FromFile(m_wsFilePath))
		return false;

	if (!oReader.ReadNextNode() && "HWPML" != oReader.GetName())
		return false;

	return true;
}

void CHWPMLFile::Close()
{}

const CHWPDocInfo* CHWPMLFile::GetDocInfo() const
{
	return &m_oDocInfo;
}

void CHWPMLFile::ReadHead(CXMLReader &oReader)
{
	m_oDocInfo.ParseHWPML(oReader);
}

void CHWPMLFile::ReadBody(CXMLReader &oReader)
{
	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "SECTION")
		ReadSection(oReader);
	END_WHILE
}

void CHWPMLFile::ReadTail(CXMLReader &oReader)
{
	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "BINDATASTORAGE")
	{
		WHILE_READ_NEXT_NODE_WITH_DEPTH_AND_NAME(oReader, Item)
		{
			if ("BINDATA" == sNodeItemName)
				ReadBinData(oReader);
		}
		END_WHILE
	}
	END_WHILE
}

void CHWPMLFile::ReadSection(CXMLReader &oReader)
{
	CHWPSection* pSection = new CHWPSection();

	if (pSection->Parse(oReader, EHanType::HWPML))
		m_arSections.push_back(pSection);
	else
		delete pSection;
}

void CHWPMLFile::ReadBinData(CXMLReader &oReader)
{
	size_t unSize = 0;
	std::string sEncoding;
	HWP_STRING sId;
	bool bCompress = true;

	START_READ_ATTRIBUTES(oReader)
	{
		if ("Size" == sAttributeName)
			unSize = oReader.GetInt();
		else if ("Encoding" == sAttributeName)
			sEncoding = oReader.GetTextA();
		else if ("Id" == sAttributeName)
			sId = oReader.GetText();
		else if ("Compress" == sAttributeName)
			bCompress = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)

	if (sId.empty())
		return;

	const std::wstring wsImageData{oReader.GetText()};

	int nImageSize = NSBase64::Base64DecodeGetRequiredLength(wsImageData.length());

	BYTE* pBuffer = new(std::nothrow) BYTE[nImageSize];

	if (nullptr == pBuffer)
		return;

	if (FALSE == NSBase64::Base64Decode(wsImageData.c_str(), wsImageData.length(), pBuffer, &nImageSize))
	{
		delete[] pBuffer;
		return;
	}

	BinMap::iterator itFound = m_mBinDates.find(sId);

	if (m_mBinDates.end() != itFound)
	{
		((CHWPStream*)itFound->second)->Clear();
		((CHWPStream*)itFound->second)->SetStream((HWP_BYTE*)pBuffer, nImageSize, false);
	}
	else
		m_mBinDates.insert(std::make_pair(sId, new CHWPStream((HWP_BYTE*)pBuffer, nImageSize, false)));
}

VECTOR<const CHWPSection*> CHWPMLFile::GetSections() const
{
	RETURN_VECTOR_CONST_PTR(CHWPSection, m_arSections);
}

bool CHWPMLFile::GetBinData(const HWP_STRING &sId, CHWPStream &oBuffer) const
{
	BinMap::const_iterator itFound = m_mBinDates.find(sId);

	if (m_mBinDates.cend() == itFound || nullptr == itFound->second)
		return false;

	oBuffer.SetStream(((CHWPStream*)(itFound->second))->GetCurPtr(), ((CHWPStream*)(itFound->second))->SizeToEnd());

	return true;
}
}
