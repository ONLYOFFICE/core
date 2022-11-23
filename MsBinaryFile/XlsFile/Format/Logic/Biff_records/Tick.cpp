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

#include "Tick.h"

namespace XLS
{

Tick::Tick()
{
	iReadingOrder = 0;
}


Tick::~Tick()
{
}


BaseObjectPtr Tick::clone()
{
	return BaseObjectPtr(new Tick(*this));
}



void Tick::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> tktMajor >> tktMinor >> tlt >> wBkgMode >> rgb;
	record.skipNunBytes(16); // reserved
	record >> flags >> icv >> trot;

	fAutoCo			= GETBIT(flags, 0);
	fAutoMode		= GETBIT(flags, 1);
	rot				= GETBITS(flags, 2, 4);
	fAutoRot		= GETBIT(flags, 5);
	iReadingOrder	= GETBITS(flags, 14, 15);

	_rott = rot;
}

int Tick::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:majorTickMark")
		{
			switch(tktMajor)
			{
			case 0: CP_XML_ATTR(L"val", L"none");	break;
			case 1:	CP_XML_ATTR(L"val", L"in");		break;
			case 2:	CP_XML_ATTR(L"val", L"out");	break;
			case 3:	CP_XML_ATTR(L"val", L"cross");	break;
			}
			
		}
		CP_XML_NODE(L"c:minorTickMark")
		{
			switch(tktMinor)
			{
			case 0: CP_XML_ATTR(L"val", L"none");	break;
			case 1:	CP_XML_ATTR(L"val", L"in");		break;
			case 2:	CP_XML_ATTR(L"val", L"out");	break;
			case 3:	CP_XML_ATTR(L"val", L"cross");	break;
			}
			
		}
		CP_XML_NODE(L"c:tickLblPos")
		{
			switch(tlt)
			{
			case 0: CP_XML_ATTR(L"val", L"none");	break;
			case 1:	CP_XML_ATTR(L"val", L"low");	break;
			case 2:	CP_XML_ATTR(L"val", L"high");	break;
			case 3:	CP_XML_ATTR(L"val", L"nextTo");	break;
			}
		}
	}
	return 0;
}

} // namespace XLS

