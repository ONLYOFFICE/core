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

#include "XFPropColor.h"
#include "../../../../Common/Utils/simple_xml_writer.h"

#include "../../../../../OOXML/Base/Unit.h"
#include "../../../../../DesktopEditor/xml/include/xmlutils.h"

namespace XLS
{


BiffStructurePtr XFPropColor::clone()
{
	return BiffStructurePtr(new XFPropColor(*this));
}


void XFPropColor::load(CFRecord& record)
{
	unsigned char flags;
	record >> flags;
	
	fValidRGBA	= GETBIT(flags, 0);
	xclrType	= GETBITS(flags, 1, 7);
	
	record >> icv >> nTintShade >> dwRgba;
}

void XFPropColor::save(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
		fValidRGBA = true;
	unsigned char flags = 0;

	SETBIT(flags, 0, fValidRGBA)
	SETBITS(flags, 1, 7, xclrType)

	record << flags;
	record << icv << nTintShade << dwRgba;
}

int XFPropColor::serialize(std::wostream & stream, const std::wstring &node_name)
{
	if (xclrType > 3 )return 0;//not set

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(node_name)
		{
			switch(xclrType)
			{
				case 0: CP_XML_ATTR(L"auto",	1);		break;
				case 1: CP_XML_ATTR(L"indexed", icv);	break;
				case 3: CP_XML_ATTR(L"theme",	icv);	break;
				default:
					CP_XML_ATTR(L"rgb", dwRgba.strRGB);	break;
			}
			if (nTintShade != 0)
			{
				CP_XML_ATTR(L"tint", nTintShade/ 32767.0);
			}
		}
	}
	return 0;
}

int XFPropColor::deserialize(XmlUtils::CXmlLiteReader& oReader)
{
	if (oReader.GetAttributesCount() > 0 && oReader.MoveToFirstAttribute() == true)
	{
		std::wstring wsPropName = oReader.GetName();
		nTintShade = 0;
		fValidRGBA = false;
		while (!wsPropName.empty())
		{			
			if (wsPropName == L"auto" && oReader.GetText() == L"1")
				xclrType = 0;
			else if (wsPropName == L"indexed")
			{
				xclrType = 1;
				icv = XmlUtils::GetInteger(oReader.GetText());
			}
			else if (wsPropName == L"theme")
			{
				xclrType = 3;
				icv = XmlUtils::GetInteger(oReader.GetText());
			}
			else if (wsPropName == L"rgb")
			{
				xclrType = 2;
				dwRgba.Parse(oReader.GetText());
				fValidRGBA = true;
			}
            if (wsPropName == L"tint")
			{
                nTintShade = XmlUtils::GetDouble(oReader.GetText()) * 32767.0;
			}
            else
            {
                nTintShade = 0;
            }
			if (!oReader.MoveToNextAttribute())
				break;

			wsPropName = oReader.GetName();
		}
		oReader.MoveToElement();
	}
	return 0;
}


} // namespace XLS
