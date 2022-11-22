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

#include "ExternName.h"

namespace XLS
{

ExternName::ExternName(const unsigned short supporting_link_type, bool bOle)
:	supbook_cch(supporting_link_type), cf(-1), bOleVirtualPath(bOle)
{
}


ExternName::~ExternName()
{
}


BaseObjectPtr ExternName::clone()
{
	return BaseObjectPtr(new ExternName(*this));
}

void ExternName::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;

	fBuiltIn	= GETBIT(flags, 0);
	fWantAdvise = GETBIT(flags, 1);
	fWantPict	= GETBIT(flags, 2);
	fOle		= GETBIT(flags, 3);
	fOleLink	= GETBIT(flags, 4);
	cf			= GETBITS(flags, 5, 14);
	fIcon		= GETBIT(flags, 15);

	if(0x3A01 == supbook_cch) // UDF reference on a XLL or COM add-in.
	{
		body = BiffStructurePtr(new AddinUdf);
	}
	else
	{
		if(fOle && !fOleLink)
		{
			body = BiffStructurePtr(new ExternDdeLinkNoOper);
		}
		if(!fOle && fOleLink) 
		{
			body = BiffStructurePtr(new ExternOleDdeLink);
		}
		if(!fOle && !fOleLink)
		{
			if (bOleVirtualPath)
			{
				body = BiffStructurePtr(new ExternOleDdeLink);
			}
			else
			{
				body = BiffStructurePtr(new ExternDocName);
			}
		}
	}
	body->load(record);


//cache
	switch(cf)
	{
	case 0:		// is text
		break;
	case 5:		//csv (,)
	case 6:		//Microsoft Symbolic Link (SYLK).
	case 8:		//biff8
	case 44:	//unicode text
	case 63:	//biff12
		break;
	}
}

} // namespace XLS

