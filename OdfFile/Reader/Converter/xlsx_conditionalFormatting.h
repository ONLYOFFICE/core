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

#include <CPOptional.h>
#include <CPScopedPtr.h>
#include <string>

namespace cpdoccore {
namespace oox {

class xlsx_conditionalFormatting_context
{
public:
    xlsx_conditionalFormatting_context();
    ~xlsx_conditionalFormatting_context();

	void start(const std::wstring& ref);
	void end(){}

	bool start_by(const std::wstring& val, const std::wstring& ref); //return bNew

	void add_rule(int type);

	void set_formula(std::wstring f);

	void set_dataBar(_CP_OPT(unsigned int) min, _CP_OPT(unsigned int) max);
	void set_gradient(bool val);

	void set_dxf	(int dxf_id);
	void set_showVal(bool val);
	void set_time_period(int val);
	void set_stdDev(int val);

	void add_sfv	(int type, std::wstring value);
	void add_color	(std::wstring col);
	
	void set_negative_color(std::wstring col);
	
	void set_axis_position(std::wstring val);
	void set_axis_color(std::wstring val);
	void set_icon_set_type(int type);

    void serialize(std::wostream & _Wostream);
	void serializeEx(std::wostream& _Wostream);
private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}
