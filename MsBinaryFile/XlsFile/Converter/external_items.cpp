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

#include "external_items.h"

#include "../../../OOXML/Base/Unit.h"
#include "../../Common/Utils/simple_xml_writer.h"

#include "oox_rels.h"
#include "mediaitems_utils.h"

#include "../../../OOXML/Base/Base.h"
#include "../../../OOXML/Base/Unit.h"

#include "../../../DesktopEditor/common/Directory.h"

namespace oox {


external_items::item::item(	std::wstring const & _uri, Type _type, bool _mediaInternal, int _id, std::wstring const & _rId  )
                       : uri(_uri), type(_type), mediaInternal(_mediaInternal), id(_id), rId(_rId)
{    
}

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
	case external_items::typeActiveX:
       return L"activeX";
	case external_items::typeControlProps:
       return L"ctrlProp";
	default:
        return L"";
    }
}

std::wstring external_items::add_image(const std::wstring & image, int bin_id)
{
    const bool isMediaInternal = true;
  
	std::wstring rId = std::wstring(L"picId") + std::to_wstring(count_image + 1);
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
std::wstring external_items::add_control_activeX(std::wstring & oox_target_bin)
{
    const bool isMediaInternal = true;
  
	count_activeX++;  
	
	std::wstring rId = std::wstring(L"ocxId") + std::to_wstring(count_activeX);

	oox_target_bin = std::wstring(L"activeX") + std::to_wstring(count_activeX) + L".bin";
		
	items_.push_back( item(oox_target_bin, typeActiveX, isMediaInternal, -1, rId) );
    return rId;
}
std::wstring external_items::add_control_props(std::wstring & oox_target)
{
    const bool isMediaInternal = true;
  
	count_controls++;  
	
	std::wstring rId = std::wstring(L"ctrlId") + std::to_wstring(count_controls);

	oox_target = std::wstring(L"ctrlProp") + std::to_wstring(count_controls) + L".xml";
		
	items_.push_back( item(oox_target, typeControlProps, isMediaInternal, -1, rId) );
    return rId;
}
std::wstring external_items::add_embedding(std::wstring & oox_target, const std::wstring & info)
{
    const bool isMediaInternal = true;
  
	count_embeddings++;  
	
	std::wstring rId = std::wstring(L"objId") + std::to_wstring(count_embeddings);

	std::wstring lowerInfo = XmlUtils::GetLower(info);

	std::wstring extension;
	
	if ( false == oox_target.empty())							extension = oox_target;
	else if (std::wstring::npos != lowerInfo.find(L"excel"))	extension = L".xls";
	else if (std::wstring::npos != lowerInfo.find(L"word"))		extension = L".doc";
	else extension = L".bin";

	oox_target = std::wstring(L"oleObject") + std::to_wstring(count_embeddings) + extension;
		
	items_.push_back( item(oox_target, typeOleObject, isMediaInternal, -1, rId) );
    return rId;
}
std::wstring external_items::find_image(int _id, std::wstring & oox_target, bool & isInternal)
{
    for (size_t i=0 ; i <items_.size(); i ++)
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
    for (size_t i = 0; i <items_.size(); i ++)
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
    for (size_t i = 0 ; i <items_.size(); i ++)
	{
        Rels.add( relationship(
					items_[i].rId, 
					utils::media::get_rel_type(items_[i].type), 
					items_[i].uri, 
					items_[i].mediaInternal ? L"" : L"External" )
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
std::wstring external_items::activeX_path()
{
	return activeX_path_;
}
void external_items::create_activeX_path(const std::wstring & out_path)
{
	if (!activeX_path_.empty()) return;
	
	std::wstring xl_path = out_path + FILE_SEPARATOR_STR + L"xl";
	NSDirectory::CreateDirectory(xl_path.c_str());
	
    NSDirectory::CreateDirectory((xl_path + FILE_SEPARATOR_STR + L"activeX").c_str());

	activeX_path_ = xl_path + FILE_SEPARATOR_STR + L"activeX" + FILE_SEPARATOR_STR;
}
void external_items::create_embeddings_path(const std::wstring & out_path)
{
	if (!embeddings_path_.empty()) return;
	
	std::wstring xl_path = out_path + FILE_SEPARATOR_STR + L"xl";
	NSDirectory::CreateDirectory(xl_path.c_str());
	
    NSDirectory::CreateDirectory((xl_path + FILE_SEPARATOR_STR + L"embeddings").c_str());

	embeddings_path_ = xl_path + FILE_SEPARATOR_STR + L"embeddings" + FILE_SEPARATOR_STR;
}
std::wstring external_items::embeddings_path()
{
	return embeddings_path_;
}

}
