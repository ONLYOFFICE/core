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

#include "Pos.h"
#include "Frame.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

Pos::Pos()
{
	m_iLinkObject	= -1;
	m_iLayoutTarget	= 0; //not set, 1 - outer, 2 -inner
}


Pos::~Pos()
{
}


BaseObjectPtr Pos::clone()
{
	return BaseObjectPtr(new Pos(*this));
}

void Pos::readFields(CFRecord& record)
{
	pGlobalWorkbookInfoPtr = record.getGlobalWorkbookInfo();

	record >> mdTopLt >> mdBotRt;
	record >> x1;
	record.skipNunBytes(2); // unused1
	record >> y1;
	record.skipNunBytes(2); // unused2
	record >> x2;
	record.skipNunBytes(2); // unused3
	record >> y2;
	record.skipNunBytes(2); // unused4
}
void Pos::writeFields(CFRecord& record)
{
    record << mdTopLt << mdBotRt;
    record << x1;
    record.reserveNunBytes(2); // unused1
    record << y1;
    record.reserveNunBytes(2); // unused2
    record << x2;
    record.reserveNunBytes(2); // unused3
    record << y2;
    record.reserveNunBytes(2); // unused4
}

int Pos::serialize(std::wostream & _stream)
{
	bool bAutoPosition	= true;
	bool bAutoSize		= true;

	if (m_Frame)
	{
		Frame* Frame_ = dynamic_cast<Frame*>(m_Frame.get());

		bAutoPosition	= Frame_->fAutoPosition;
		bAutoSize		= Frame_->fAutoSize;
	}

	double x = x1 / 4000.;
	double y = y1 / 4000.;

	double w = x2 / 4000.;
	double h = y2 / 4000.;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:layout")
		{
			if (bAutoSize && bAutoPosition)
			{
			}
			else
			{
				CP_XML_NODE(L"c:manualLayout")
				{
					if (m_iLayoutTarget > 0)
					{
						CP_XML_NODE(L"c:layoutTarget"){CP_XML_ATTR(L"val", m_iLayoutTarget == 1 ? L"outer" : L"inner");}
					}
					//if (m_iLinkObject == 1 && mdTopLt == 2 &&  mdBotRt == 2 && x == 0) 
					//	x += 0.5 - (w > 0 ? w / 2 : 0);
					//if (m_iLinkObject == 2) x += 0.5	+ (w > 0 ? w : 0);
					//if (m_iLinkObject == 3) y += 0	+ (h > 0 ? h : 0);

					CP_XML_NODE(L"c:xMode")	{CP_XML_ATTR(L"val", L"edge");}
					CP_XML_NODE(L"c:yMode")	{CP_XML_ATTR(L"val", L"edge");}

					//if (x < 0) x = 0;
					//if (y < 0) y = 0;
					
					if (!bAutoPosition)
					{
						if (x >= 0 && x < 1) CP_XML_NODE(L"c:x")		{CP_XML_ATTR(L"val", x);}
						if (y >= 0 && y < 1) CP_XML_NODE(L"c:y")		{CP_XML_ATTR(L"val", y);}
					}
					
					if (!bAutoSize && m_iLinkObject != 1 && m_iLinkObject != 2 && m_iLinkObject != 3) 
						//title, axis title vert, axis title horiz
					{
						if (w > 0 && w < 1) CP_XML_NODE(L"c:w")		{CP_XML_ATTR(L"val", w);}
						if (h > 0 && h < 1) CP_XML_NODE(L"c:h")		{CP_XML_ATTR(L"val", h);}
					}
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

