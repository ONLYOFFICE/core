/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
		graphic_format_properties_ptr calc_graphic_properties_content(const style_instance* styleInstance, bool noParentStandard = false);
		graphic_format_properties_ptr calc_graphic_properties_content(const std::vector<const style_instance *> & styleInstances, bool noParentStandard = false);

		text_format_properties_ptr calc_text_properties_content(const style_instance * styleInstance);
		text_format_properties_ptr calc_text_properties_content(const std::vector<const style_instance *> & styleInstances);

		paragraph_format_properties calc_paragraph_properties_content(const style_instance * styleInstance);
		paragraph_format_properties calc_paragraph_properties_content(const std::vector<const style_instance *> & styleInstances);

		style_table_cell_properties_attlist calc_table_cell_properties(const style_instance * styleInstance);
		style_table_cell_properties_attlist calc_table_cell_properties(const std::vector<const style_instance *> & styleInstances);

		void calc_tab_stops(const style_instance * styleInstance, oox::tabs_context & context);
		_CP_OPT(int) calc_outline_level(_CP_OPT(int) value, const style_instance * styleInstance);
	}
}
