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

#include "Legend.h"
#include "CrtLayout12.h"

namespace XLS
{

Legend::Legend()
{
}


Legend::~Legend()
{
}


BaseObjectPtr Legend::clone()
{
	return BaseObjectPtr(new Legend(*this));
}


void Legend::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> x >> y >> dx >> dy;
	record.skipNunBytes(1); // unused
	record >> wSpace >> flags;

	fAutoPosition	= GETBIT(flags, 0);
	fAutoPosX		= GETBIT(flags, 2);
	fAutoPosY		= GETBIT(flags, 3);
	fVert			= GETBIT(flags, 4);
	
	fWasDataTable	= GETBIT(flags, 5);	
}

int Legend::serialize(std::wostream & _stream, int size)
{
	CrtLayout12 *crtLayout = dynamic_cast<CrtLayout12 *> (m_CrtLayout12.get());
	
	double x1Kf = x / 4000.;
	double y1Kf = y / 4000.;

	double x2Kf = (4000 - dx - x) / 4000.;
	double y2Kf = (4000 - dy - y) / 4000.;

	if ((crtLayout) && (crtLayout->wXMode == 0 &&  crtLayout->wYMode == 0 && crtLayout->wWidthMode == 0 && crtLayout->wHeightMode == 0))
	{
		fAutoPosX = true;
		fAutoPosY = true;
	}

	CP_XML_WRITER(_stream)    
	{
		if (fAutoPosX && fAutoPosY)
		{
			CP_XML_NODE(L"c:legendPos")
			{
				if (y1Kf > 0.5 && y1Kf > x1Kf)
				{
					CP_XML_ATTR(L"val", "b");
					x = x - (size - 1 ) * dx / 2;
					dx = dx * size;
				}
				else if (x1Kf > 0.5)
				{
					CP_XML_ATTR(L"val", "r");
					y = y - (size - 1 ) * dy / 2;
					dy = dy * size;
				}
				else if (x2Kf > 0.5 && x2Kf > y2Kf)
				{
					CP_XML_ATTR(L"val", "l");
					y = y - (size - 1 ) * dy / 2;
					dy = dy * size;
				}
				else if (y2Kf > 0.5)
				{
					CP_XML_ATTR(L"val", "t");
					x = x - (size - 1 ) * dx / 2;
					dx = dx * size;
				}
				else
				{
					//нету откровенного смещения
					CP_XML_ATTR(L"val", "l");
					dy = dy * size;
				}
			}
			CP_XML_NODE(L"c:layout");
			CP_XML_NODE(L"c:overlay") {CP_XML_ATTR(L"val", 0);}
		}
		else if (m_CrtLayout12)
		{
			m_CrtLayout12->serialize(_stream);
			CP_XML_NODE(L"c:overlay") {CP_XML_ATTR(L"val", 0);}
		}
		else
		{
			CP_XML_NODE(L"c:layout")
			{
				CP_XML_NODE(L"c:manualLayout")
				{
					CP_XML_NODE(L"c:xMode")	{CP_XML_ATTR(L"val", L"edge");}
					CP_XML_NODE(L"c:yMode")	{CP_XML_ATTR(L"val", L"edge");}
					
					if (fAutoPosX == false)	CP_XML_NODE(L"c:x")		{CP_XML_ATTR(L"val", x / 4000.);}
					if (fAutoPosY == false)	CP_XML_NODE(L"c:y")		{CP_XML_ATTR(L"val", y / 4000.);}

					if (fAutoPosX == false)	CP_XML_NODE(L"c:w")		{CP_XML_ATTR(L"val", (dx )/ 4000. /*+ 0.01*/);}
					if (fAutoPosY == false) CP_XML_NODE(L"c:h")		{CP_XML_ATTR(L"val", (dy )/ 4000. /*+ 0.01*/);}
				}
			}
			CP_XML_NODE(L"c:overlay") {CP_XML_ATTR(L"val", 1);}
		}
	}
	
	return 0;
}

} // namespace XLS

