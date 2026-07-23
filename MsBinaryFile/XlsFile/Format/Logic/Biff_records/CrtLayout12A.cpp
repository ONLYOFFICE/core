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

#include "CrtLayout12A.h"

namespace XLS
{

CrtLayout12A::CrtLayout12A()
{
}

CrtLayout12A::~CrtLayout12A()
{
}

BaseObjectPtr CrtLayout12A::clone()
{
	return BaseObjectPtr(new CrtLayout12A(*this));
}

void CrtLayout12A::readFields(CFRecord& record)
{
	unsigned short flags, reserved;
	record >> frtheader >> dwCheckSum >> flags;

	fLayoutTargetInner = GETBIT(flags, 0);

	record >> xTL >> yTL >> xBR >> yBR;

	record >> wXMode >> wYMode >> wWidthMode >> wHeightMode;
	
	record >> x >> y >> dx >> dy >> reserved;
}

void CrtLayout12A::writeFields(CFRecord& record)
{
	frtheader.rt = rt_CrtLayout12A;
	unsigned short flags = 0;
	SETBIT(flags, 0, fLayoutTargetInner)

	record << frtheader << dwCheckSum << flags;
	record << xTL << yTL << xBR << yBR;
	record << wXMode << wYMode << wWidthMode << wHeightMode;
	record << x << y << dx << dy;
	record.reserveNunBytes(2);
}

int CrtLayout12A::serialize (std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
    {
		CP_XML_NODE(L"c:layout")
		{
			if (wXMode == 0 &&  wYMode == 0 && wWidthMode == 0 && wHeightMode == 0)
			{
			}
			else
			{
				CP_XML_NODE(L"c:manualLayout")
				{
					CP_XML_NODE(L"c:layoutTarget"){CP_XML_ATTR(L"val", fLayoutTargetInner ? L"inner" : L"outer");}

					if (wXMode > 0)
					{
						CP_XML_NODE(L"c:xMode")	
						{
							if (wXMode == 1)	CP_XML_ATTR(L"val", L"factor");
							else				CP_XML_ATTR(L"val", L"edge");
						}
					}
					if (wYMode > 0)
					{
						CP_XML_NODE(L"c:yMode")
						{
							if (wYMode == 1)	CP_XML_ATTR(L"val", L"factor");
							else				CP_XML_ATTR(L"val", L"edge");
						}
					}

					if (wXMode > 0)
					{
						CP_XML_NODE(L"c:x")		{CP_XML_ATTR(L"val", x.data.value);}
					}
					if (wYMode > 0)
					{
						CP_XML_NODE(L"c:y")		{CP_XML_ATTR(L"val", y.data.value);}
					}
					
					if (wWidthMode > 0)
					{
						CP_XML_NODE(L"c:wMode")	
						{
							if (wWidthMode == 1)CP_XML_ATTR(L"val", L"factor");
							else				CP_XML_ATTR(L"val", L"edge");
						}
					}
					if (wHeightMode > 0)
					{
						CP_XML_NODE(L"c:hMode")
						{
							if (wHeightMode == 1)CP_XML_ATTR(L"val", L"factor");
							else				CP_XML_ATTR(L"val", L"edge");
						}
					}
					if (wWidthMode > 0)
					{
						CP_XML_NODE(L"c:w") {CP_XML_ATTR(L"val", dx.data.value);}
					}
					if (wHeightMode > 0)
					{
						CP_XML_NODE(L"c:h")	 {CP_XML_ATTR(L"val", dy.data.value);}
					}
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

