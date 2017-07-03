/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "external_items.h"

#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>

#include <utils.h>

#include "oox_rels.h"
#include "mediaitems_utils.h"

#include "../../../Common/DocxFormat/Source/Base/Base.h"
#include "../../../DesktopEditor/common/Directory.h"

namespace oox {


external_items::item::item(	std::wstring const & _uri, Type _type, bool _mediaInternal, int _id, std::wstring const & _rId  )
                       : uri(_uri), type(_type), mediaInternal(_mediaInternal), id(_id), rId(_rId)
{    
}

//std::wstring external_items::add_or_find(const std::wstring & href, Type type, bool & isInternal)
//{
//    std::wstring ref;
//    return add_or_find(href, type, isInternal, ref);
//}

std::wstring static get_default_file_name(external_items::Type type)
{
    switch (type)
    {
    case external_items::typeImage:
        return L"image";
    case external_items::typeChart:
        return L"chart";
    case external_items::typeMedia:
        return L"media";
	default:
        return L"";
    }
}
//std::wstring external_items::create_file_name(const std::wstring & uri, external_items::Type type, size_t Num)
//{
//	if (uri.empty()) return L"";
//
//	std::wstring sExt;
//	int n = uri.rfind(L".");
//	if (n>0) sExt = uri.substr(n);
//	else if (n==0)
//	{
//
//	}
//	//todooo проверить
//   
//	return get_default_file_name(type) + std::to_wstring(Num) + sExt;
//}



std::wstring external_items::add_image(const std::wstring & image, int bin_id)
{
    const bool isMediaInternal = true;
  
	std::wstring rId = std::wstring(L"picId") + std::to_wstring(count_image+1);
	count_image++;  
		
	items_.push_back( item(image, typeImage, isMediaInternal, bin_id, rId) );

    return rId;
}
std::wstring external_items::add_chart(std::wstring & oox_target)
{
    const bool isMediaInternal = true;
  
	count_charts++;  
	
	std::wstring rId = std::wstring(L"chartId") + std::to_wstring(count_charts);
	oox_target = std::wstring(L"charts/chart") + std::to_wstring(count_charts) + L".xml";
		
	items_.push_back( item(oox_target, typeChart, isMediaInternal, -1, rId) );

    return rId;
}
std::wstring external_items::find_image(int _id, std::wstring & oox_target, bool & isInternal)
{
    for (int i=0 ; i <items_.size(); i ++)
	{
		if (items_[i].type == typeImage && items_[i].id == _id)
		{
			oox_target	= items_[i].uri;
			isInternal	= items_[i].mediaInternal;
			return items_[i].rId;
		}
	}
	return L"";
}
std::wstring external_items::find_image(const std::wstring & oox_target, bool & isInternal)
{
    for (int i=0 ; i <items_.size(); i ++)
	{
		if (items_[i].type == typeImage && items_[i].uri == oox_target)
		{
			isInternal = items_[i].mediaInternal;
			return items_[i].rId;
		}
	}
	return L"";
}
void external_items::dump_rels(rels & Rels)
{
    size_t i = 0;
    BOOST_FOREACH(item & elm, items_)
    {
        Rels.add( relationship(
					elm.rId, 
					utils::media::get_rel_type(elm.type), 
					elm.uri, 
					elm.mediaInternal ? L"" : L"External" )
                );
    }        
}
	
std::wstring external_items::media_path()
{
	return media_path_;
}
void external_items::create_media_path(const std::wstring & out_path)
{
	if (!media_path_.empty()) return;

	std::wstring xl_path = out_path + FILE_SEPARATOR_STR + L"xl";
    NSDirectory::CreateDirectory(xl_path.c_str());
	
    NSDirectory::CreateDirectory((xl_path + FILE_SEPARATOR_STR + L"media").c_str());

	media_path_ = xl_path + FILE_SEPARATOR_STR + L"media" + FILE_SEPARATOR_STR;
}


}
