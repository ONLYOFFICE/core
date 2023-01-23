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

#include "Bar.h"
#include "Chart3d.h"

namespace XLS
{

Bar::Bar()
{
}


Bar::~Bar()
{
}


BaseObjectPtr Bar::clone()
{
	return BaseObjectPtr(new Bar(*this));
}


void Bar::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> pcOverlap >> pcGap >> flags;
	
	fTranspose	= GETBIT(flags, 0);
	fStacked	= GETBIT(flags, 1);
	f100		= GETBIT(flags, 2);
	fHasShadow	= GETBIT(flags, 3);
}

int	Bar::serialize(std::wostream & _stream)
{
	Chart3d *chart3D = dynamic_cast<Chart3d *>(m_chart3D.get());
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:barDir")
		{
			if (fTranspose) CP_XML_ATTR (L"val" , L"bar");
			else			CP_XML_ATTR (L"val" , L"col");
		}
		if (fStacked || f100)
		{
			CP_XML_NODE(L"c:grouping")
			{
				if (f100)			CP_XML_ATTR (L"val" , L"percentStacked");				
				else if (fStacked)	CP_XML_ATTR (L"val" , L"stacked");
			}
		}
		else
		{
			CP_XML_NODE(L"c:grouping")
			{
				if ((chart3D) && (chart3D->fCluster))
					CP_XML_ATTR (L"val" , L"clustered"); 
				else		
					CP_XML_ATTR (L"val" , L"standard");
			}
		}

		CP_XML_NODE(L"c:gapWidth")
		{
			CP_XML_ATTR (L"val" , pcGap);
		}
		if (!chart3D && pcOverlap != 0)
		{
			CP_XML_NODE(L"c:overlap")
			{
				CP_XML_ATTR (L"val" , -pcOverlap);
			}
		}

		if ((chart3D) && (chart3D->pcGap != 150))
		{
			CP_XML_NODE(L"c:gapDepth")
			{
				CP_XML_ATTR (L"val" , chart3D->pcGap);
			}
		}
	}
	return 0;
}

} // namespace XLS

