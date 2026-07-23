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
#include <iosfwd>
#include <boost/noncopyable.hpp>
#include "../../Common/CPScopedPtr.h"

#include "../../DataTypes/officevaluetype.h"

namespace cpdoccore {
	namespace odf_reader
	{
		class odf_read_context;
	}
namespace oox {

class num_format_context : boost::noncopyable
{
public:
	num_format_context(odf_reader::odf_read_context & odfContext, bool bShield = false);
    ~num_format_context();

	bool get_shield();

    void start_format(const std::wstring & style_name);
    std::wostream & output();
    void end_format();

    std::wstring get_last_format() const;
	std::wstring get_last_date_format() const;
	std::wstring get_last_time_format() const;

	odf_types::office_value_type::type type() const;
	void type(odf_types::office_value_type::type t);

    void start_complex_format(std::wstring const & name);
    void add_format(std::wstring const & cond, std::wstring const & format);
    void end_complex_format();

	std::wstring find_complex_format(std::wstring const & name, odf_types::office_value_type::type & type);

	//std::wstring default_data_style(int type);
	odf_reader::odf_read_context & odf_context_;

private:
	std::map<std::wstring, std::pair<odf_types::office_value_type::type, std::wstring>> mapFormats;
		
	class Impl;
    _CP_SCOPED_PTR(Impl) impl_;

    class Impl2;
    _CP_SCOPED_PTR(Impl2) impl2_;
};

}
}
