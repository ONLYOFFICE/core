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

#include "office_settings.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>

namespace cpdoccore { 
namespace odf_writer {

// office:scripts
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_settings::ns = L"office";
const wchar_t * office_settings::name = L"settings";

void office_settings::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void office_settings::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void office_settings::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

// config_item_set
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_set::ns = L"config";
const wchar_t * settings_config_item_set::name = L"config-item-set";

void settings_config_item_set::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void settings_config_item_set::serialize(std::wostream & _Wostream)
{
	if (config_name_.empty()) return;

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR( L"config:name", XmlUtils::EncodeXmlString(config_name_));

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void settings_config_item_set::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
// config_item
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item::ns = L"config";
const wchar_t * settings_config_item::name = L"config-item";

void settings_config_item::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
}

void settings_config_item::serialize(std::wostream & _Wostream)
{
	if (config_name_.empty() || config_type_.empty()) return;

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR( L"config:name", XmlUtils::EncodeXmlString(config_name_));
			CP_XML_ATTR( L"config:type", config_type_);

			CP_XML_STREAM() << XmlUtils::EncodeXmlString(content_);
		}
	}
}
void settings_config_item::add_child_element( const office_element_ptr & child_element)
{
}
// config_item_map_indexed
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_map_indexed::ns = L"config";
const wchar_t * settings_config_item_map_indexed::name = L"config-item-map-indexed";


void settings_config_item_map_indexed::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void settings_config_item_map_indexed::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING( L"config:name", config_name_);

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void settings_config_item_map_indexed::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
// config_item_map_named
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_map_named::ns = L"config";
const wchar_t * settings_config_item_map_named::name = L"config-item-map-named";

void settings_config_item_map_named::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void settings_config_item_map_named::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING( L"config:name", config_name_);

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void settings_config_item_map_named::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
// config_item_map_entry
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * settings_config_item_map_entry::ns = L"config";
const wchar_t * settings_config_item_map_entry::name = L"config-item-map-entry";

void settings_config_item_map_entry::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void settings_config_item_map_entry::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT_ENCODE_STRING( L"config:name", config_name_);

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
void settings_config_item_map_entry::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

}
}
