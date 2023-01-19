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

#include "style_graphic_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_table_properties.h"
#include "style_presentation.h"

#include "odfcontext.h"
#include <vector>

namespace cpdoccore 
{ 
	namespace oox
	{
		class tabs_context;
	}
	namespace odf_reader 
	{
		graphic_format_properties calc_graphic_properties_content(const style_instance * styleInstance);
		graphic_format_properties calc_graphic_properties_content(const std::vector<const style_instance *> & styleInstances);

		text_format_properties_content_ptr calc_text_properties_content(const style_instance * styleInstance);
		text_format_properties_content_ptr calc_text_properties_content(const std::vector<const style_instance *> & styleInstances);

		paragraph_format_properties calc_paragraph_properties_content(const style_instance * styleInstance);
		paragraph_format_properties calc_paragraph_properties_content(const std::vector<const style_instance *> & styleInstances);

		style_table_cell_properties_attlist calc_table_cell_properties(const style_instance * styleInstance);
		style_table_cell_properties_attlist calc_table_cell_properties(const std::vector<const style_instance *> & styleInstances);

		void calc_tab_stops(const style_instance * styleInstance, oox::tabs_context & context);
		_CP_OPT(int) calc_outline_level(_CP_OPT(int) value, const style_instance * styleInstance);
	}
}
