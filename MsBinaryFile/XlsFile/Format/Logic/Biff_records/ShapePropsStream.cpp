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

#include "ShapePropsStream.h"

#include "../../../../../OOXML/PPTXFormat/Logic/SpPr.h"
#include "../../../../../DesktopEditor/common/File.h"

namespace XLS
{

ShapePropsStream::ShapePropsStream()
{
}


ShapePropsStream::~ShapePropsStream()
{
}


BaseObjectPtr ShapePropsStream::clone()
{
	return BaseObjectPtr(new ShapePropsStream(*this));
}

void ShapePropsStream::readFields(CFRecord& record)
{
	record >> frtHeader;
	record >> wObjContext;

	record.skipNunBytes(2);

	record >> dwChecksum;
	
	_UINT32	cb=0;
	record >> cb;

	if (cb > 0 && cb + record.getRdPtr() <= record.getDataSize())
	{
		xml_ = std::string( record.getCurData<char>(), cb);
		record.skipNunBytes(cb);
	}
}

void ShapePropsStream::writeFields(CFRecord& record)
{
	FrtHeader header(rt_ShapePropsStream);
	record << header << wObjContext;
	record.reserveNunBytes(2);
	record << dwChecksum;
	_UINT32	cb= xml_.size();
	record << cb;
	auto limit = 8200;
	if(cb <= limit)
		limit = cb;
	for(auto i = 0; i < limit; i++)
		record << xml_[i];
}

std::wstring ShapePropsStream::getChartSpPr() const
{
	std::string::size_type spPos = xml_.find("<a:spPr");
	if (spPos == std::string::npos)
		return L"";

	std::wstring buf = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(
		(BYTE*)(xml_.c_str() + spPos), (LONG)(xml_.size() - spPos));

	XmlUtils::CXmlLiteReader reader;

	if (!reader.FromString(buf))		return L"";
	if (!reader.ReadNextNode())			return L"";
	if (reader.GetName() != L"a:spPr")	return L"";	// truncated or unexpected content

	PPTX::Logic::SpPr props;
	props.fromXML(reader);

	NSBinPptxRW::CXmlWriter writer;
	writer.m_lDocType = XMLWRITER_DOC_TYPE_CHART;
	props.toXmlWriter(&writer);

	return writer.GetXmlString();
}

} // namespace XLS

