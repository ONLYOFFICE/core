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

	void start_table(std::wstring name);
	void end_table	();

	void add_property(std::wstring name, std::wstring type, std::wstring value);
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

	std::vector<office_element_ptr>	config_content_;
	odf_conversion_context*			odf_context_;

	int								current_table_;
	int								current_view_;

};


}
}
