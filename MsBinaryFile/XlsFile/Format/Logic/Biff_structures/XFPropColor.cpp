﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
			}
			else if (wsPropName == L"tint")
			{
				nTintShade = XmlUtils::GetInteger(oReader.GetText()) * 32767.0;
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
