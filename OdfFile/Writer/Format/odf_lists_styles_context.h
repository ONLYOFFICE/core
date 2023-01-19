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
#pragma once

#include <string>
#include <vector>
#include <boost/unordered_map.hpp>

#include "office_elements_create.h"


namespace cpdoccore {
namespace odf_writer {

class ods_conversion_context;
class style_list_level_properties;
class style_list_level_label_alignment;
class style_text_properties;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

struct list_format_state
{
	int								oox_based_number;
	int								restart_number = 0;
	std::vector<office_element_ptr> elements;

	std::wstring					odf_list_style_name;
	bool							automatic;
};

class odf_lists_styles_context
{
public:
    odf_lists_styles_context();
	void set_odf_context(odf_conversion_context * Context);

	void start_style(bool bMaster, int number = -1);
		int start_style_level(int level, int type);
			style_list_level_properties			* get_list_level_properties();
			style_list_level_label_alignment	* get_list_level_alignment_properties();
			style_text_properties				* get_text_properties();

			void set_bullet_image_size(double size);
			void set_bullet_image	(std::wstring ref);
			
			void set_bullet_char	(std::wstring val);
			
			void set_numeric_format (std::wstring val);
			
			void set_start_number	(int val);
			void set_restart_number (int val);
			void set_text_style_name(std::wstring val);
		void end_style_level();
	void end_style();

	void process_styles(office_element_ptr root, bool automatic);

	std::wstring get_style_name(int oox_style_num = -1);
private:
	std::vector<list_format_state> lists_format_array_;	
	std::map<int, size_t> lists_format_map_;

//-----------------------------------
	odf_conversion_context *odf_context_;
};
}
}
