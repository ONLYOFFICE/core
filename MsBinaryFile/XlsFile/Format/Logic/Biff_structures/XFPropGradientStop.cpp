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

#include "XFPropGradientStop.h"
#include "Xnum.h"

#include "../../../../../OOXML/Base/Unit.h"
#include "../../../../../DesktopEditor/xml/include/xmlutils.h"

namespace XLS
{


BiffStructurePtr XFPropGradientStop::clone()
{
	return BiffStructurePtr(new XFPropGradientStop(*this));
}


void XFPropGradientStop::load(CFRecord& record)
{
	record.skipNunBytes(2); // unused

    if(record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        record >> numPosition;
    }
    else
    {
        Xnum numPosition_;

        record >> numPosition_;
        numPosition = numPosition_.data.value;
    }
    record >> color;
}

void XFPropGradientStop::save(CFRecord& record)
{
	record.reserveNunBytes(2); // unused

	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		record << numPosition;
	}
	else
	{
		Xnum numPosition_;

		numPosition_.data.value = numPosition;
		record << numPosition_;
	}
	record << color;
}


int XFPropGradientStop::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"stop")
		{	
			CP_XML_ATTR(L"position", numPosition);
			color.serialize(CP_XML_STREAM(), L"color");
		}
	}
	return 0;
}

int XFPropGradientStop::deserialize(XmlUtils::CXmlLiteReader& oReader)
{
	if (oReader.GetAttributesCount() > 0 && oReader.MoveToFirstAttribute() == true)
	{
		std::wstring wsPropName = oReader.GetName();

		if (!wsPropName.empty() && wsPropName == L"position")
		{
			numPosition = XmlUtils::GetDouble(oReader.GetText());
		}
		oReader.MoveToElement();
	}

	if (!oReader.IsEmptyNode())
	{
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			std::wstring wsPropName = oReader.GetName();

			if (wsPropName == L"color")
				color.deserialize(oReader);
		}
	}

	return 0;
}


} // namespace XLS
