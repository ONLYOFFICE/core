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

#include "DXFN.h"
#include <Binary/CFRecord.h>

#include <utils.h>

namespace XLS
{


void DXFNum::setIsUserDefined(const bool is_user_defined)
{
	is_user_defined_ = is_user_defined;
}


const bool DXFNum::getIsUserDefined() const
{
    return true == is_user_defined_;
}


BiffStructurePtr DXFNum::clone()
{
	return BiffStructurePtr(new DXFNum(*this));
}


void DXFNum::store(CFRecord& record)
{
	if(is_user_defined_)
	{
		record << user_defined;
	}
	else
	{
		record << fmt_id;
	}
}


void DXFNum::load(CFRecord& record)
{
	if ((is_user_defined_) && (*is_user_defined_))
	{
		record >> user_defined;
	}
	else
	{
		record >> fmt_id;
	}
}


int DXFNum::serialize(std::wostream & stream)
{
	if (parent->ifmtNinch && parent->fIfmtUser) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"numFmt")
		{	
			if (!parent->ifmtNinch)
				CP_XML_ATTR(L"numFmtId", fmt_id.ifmt);
 			if (!parent->fIfmtUser)
 				CP_XML_ATTR(L"formatCode", /*xml::utils::replace_text_to_xml*/(user_defined.fmt.value()));
		}
	}
	return 0;
}



} // namespace XLS

