/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

