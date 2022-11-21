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

#include "CrtLayout12.h"
#include "../Biff_structures/FrtHeader.h"

namespace XLS
{

CrtLayout12::CrtLayout12()
{
}


CrtLayout12::~CrtLayout12()
{
}


BaseObjectPtr CrtLayout12::clone()
{
	return BaseObjectPtr(new CrtLayout12(*this));
}

void CrtLayout12::readFields(CFRecord& record)
{
	FrtHeader frtHeader;
	record >> frtHeader;

	unsigned short flags;
	record >> dwCheckSum >> flags >> wXMode >> wYMode >> wWidthMode >> wHeightMode >> x >> y >> dx >> dy;
	autolayouttype = GETBITS(flags, 1, 4);
	record.skipNunBytes(2); // reserved2
}

int CrtLayout12::serialize (std::wostream & _stream)
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

