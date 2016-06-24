/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "HeaderFooter.h"
#include <Logic/Biff_structures/FrtHeader.h>
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{

HeaderFooter::HeaderFooter()
{
}


HeaderFooter::~HeaderFooter()
{
}


BaseObjectPtr HeaderFooter::clone()
{
	return BaseObjectPtr(new HeaderFooter(*this));
}


void HeaderFooter::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_HeaderFooter);
	record << frtHeader;
	_GUID_ guid_num;
	if(!STR::bstr2guid(guidSView, guid_num))
	{
		throw;// EXCEPT::LE::AttributeDataWrong(L"guid", L"HeaderFooter", guidSView);
	}
	record << guid_num;
	unsigned short flags = 0;
	SETBIT(flags, 0, fHFDiffOddEven);
	SETBIT(flags, 1, fHFDiffFirst);
	SETBIT(flags, 2, fHFScaleWithDoc);
	SETBIT(flags, 3, fHFAlignMargins);
	record << flags;
	cchHeaderEven = strHeaderEven.getSize();
	cchFooterEven = strFooterEven.getSize();
	cchHeaderFirst = strHeaderFirst.getSize();
	cchFooterFirst = strFooterFirst.getSize();
	record << cchHeaderEven << cchFooterEven << cchHeaderFirst << cchFooterFirst;
	if(cchHeaderEven)
	{
		record << strHeaderEven;
	}
	if(cchFooterEven)
	{
		record << strFooterEven;
	}
	if(cchHeaderFirst)
	{
		record << strHeaderFirst;
	}
	if(cchFooterFirst)
	{
		record << strFooterFirst;
	}
}


void HeaderFooter::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_HeaderFooter);
	record >> frtHeader;
	unsigned short flags;
	_GUID_ guid_num;
	record >> guid_num >> flags;
	guidSView = STR::guid2bstr(guid_num);
	fHFDiffOddEven = GETBIT(flags, 0);
	fHFDiffFirst = GETBIT(flags, 1);
	fHFScaleWithDoc = GETBIT(flags, 2);
	fHFAlignMargins = GETBIT(flags, 3);

	record >> cchHeaderEven >> cchFooterEven >> cchHeaderFirst >> cchFooterFirst;
	if(cchHeaderEven)
	{
		record >> strHeaderEven;
	}
	if(cchFooterEven)
	{
		record >> strFooterEven;
	}
	if(cchHeaderFirst)
	{
		record >> strHeaderFirst;
	}
	if(cchFooterFirst)
	{
		record >> strFooterFirst;
	}
}

} // namespace XLS

