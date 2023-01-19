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

#include <CPOptional.h>

#include "office_elements_create.h"
#include "../../DataTypes/common_attlists.h"

namespace cpdoccore {
namespace odf_writer
{

class odf_conversion_context;

class odf_controls_context
{
public:
	odf_controls_context(odf_conversion_context *odf_context);
    ~odf_controls_context();

	std::wstring start_control(int type, bool items_set = false);
	std::wstring start_control_sdt(int type);

	void end_control();

	office_element_ptr & get_root_element();

	bool is_exist_content();

	void finalize(office_element_ptr & root_elm);
	
	void set_name			(const std::wstring & name);
	void set_label			(const std::wstring & val);
	void set_linkedCell		(const std::wstring & val);
	void set_listFillRange	(const std::wstring & val);
	void set_macro			(const std::wstring & val);
	void set_disabled		(bool val);
	void set_printable		(bool val);
	void set_textHAlign		(int val);
	void set_textVAlign		(int val);
	void set_min_value		(int val);
	void set_max_value		(int val);
	void set_page_step		(int val);
	void set_step			(int val);
	void set_value			(const std::wstring & val);
	void set_horiz			(bool val);
	void set_check_state	(int val);
	void set_drop_down		(bool val);
	void set_drop_size		(int val);
	void add_item			(const std::wstring & val);
	void add_property		(const std::wstring & name, odf_types::office_value_type::type type, const std::wstring & value);

	void set_size( _CP_OPT(double) & width_pt, _CP_OPT(double) & height_pt);

private:	
    class Impl;
    _CP_PTR(Impl) impl_;
};
typedef shared_ptr<odf_controls_context>::Type	odf_controls_context_ptr;

}
}