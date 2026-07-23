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

#include <string>
#include <vector>
#include <map>

#include <CPSharedPtr.h>


namespace cpdoccore {
namespace odf_writer {

class odf_conversion_context;

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class odf_settings_context;
typedef shared_ptr<odf_settings_context>::Type odf_settings_context_ptr;

class odf_settings_context
{
public:
    odf_settings_context();
	void set_odf_context(odf_conversion_context * Context);

	void process_office_settings(office_element_ptr root );

	void start_view	();
	void end_view	();
	void set_current_view(int id);
	void add_property(const std::wstring &name, const std::wstring &type, const std::wstring &value);

	void start_table(std::wstring name);
	void end_table	();

	office_element_ptr create_property(const std::wstring &name, const std::wstring &type, const std::wstring &value);
	
	void add_config_content_item(const std::wstring &name, const std::wstring &type, const std::wstring &value);
	void add_common_views_property(const std::wstring &name, const std::wstring &type, const std::wstring &value);

	void set_modify_info(const std::wstring& crypt, const std::wstring& algorithm, const std::wstring& solt, const std::wstring& hash, int iteration_count);
private:
	struct _table
	{
		std::wstring name;
		std::vector<office_element_ptr> content;
	};
	struct _view
	{
		std::vector<_table>				tables;
		std::vector<office_element_ptr>	content;
	};

	std::vector<_view>				views_;

	std::vector<office_element_ptr>	common_views_content_;
	std::vector<office_element_ptr>	config_content_;
	odf_conversion_context*			odf_context_;

	int								current_table_;
	int								current_view_;

};


}
}
