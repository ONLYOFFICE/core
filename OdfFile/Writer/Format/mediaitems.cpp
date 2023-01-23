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

#include "mediaitems.h"
#include "odf_rels.h"

#include <boost/regex.h>
#include <boost/algorithm/string/case_conv.hpp>

#include "../../../OOXML/Base/Unit.h"

#include "mediaitems_utils.h"

namespace cpdoccore { 
namespace odf_writer {

_mediaitems::item::item(const std::wstring & _oox_ref, Type _type, const std::wstring & _odf_ref)
                       : oox_ref(_oox_ref), type(_type), odf_ref(_odf_ref)
{    
}

void _mediaitems::add_or_find(const std::wstring & oox_ref, Type type_,  std::wstring & odf_ref)
{
	std::wstring output_sub_path;
	std::wstring output_fileName;
    size_t number = 0;
	
	if (type_ == typeImage)
	{
		output_sub_path = L"Pictures/";
		number = count_image + 1;
	}
	else if (type_ == typeMedia)
	{
		output_sub_path = L"Media/";
		number = count_media + 1;
	}
	else if (type_ == typeOleObject)
	{
		output_sub_path = L"";
		number = count_object + 1;
	}
	else if (type_ == typeObjectReplacement)
	{
		output_sub_path = L"ObjectReplacements/";
		number = count_image_object + 1;
	}	
	output_fileName = utils::media::create_file_name(oox_ref, type_, number); //guid???
	
	std::wstring input_path = oox_ref;

#if defined (_WIN32) || defined(_WIN64) 
    boost::to_lower(input_path);
#endif

	std::wstring output_path ;

	for (size_t i = 0; i < items_.size(); i++)
    {
		if (items_[i].oox_ref == input_path)
		{
			output_path  = items_[i].odf_ref;
			break;
		}
	}
	if (output_path.empty())
	{
		output_path = ( output_sub_path + output_fileName) ;
		if ( type_ == typeImage)
		{
			count_image++;
		}
		else if ( type_ == typeMedia)
		{
			count_media++;
		}
		else if ( type_ == typeOleObject)
		{
			count_object++;
		}
		else if ( type_ == typeObjectReplacement)
		{
			count_image_object++;
		}
		item item_(input_path, type_, XmlUtils::EncodeXmlString(output_path));
		
		items_.push_back(item_);
	}
	odf_ref = output_path;
}
void _mediaitems::dump_rels(rels & Rels, std::wstring local_path)
{
	for (size_t i = 0; i < items_.size(); i++)
    {
		Rels.add( relationship(
                utils::media::get_rel_type(items_[i].type), 
                local_path + items_[i].odf_ref)
                );

    }        
}


}
}
