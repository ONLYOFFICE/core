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
	num_format_context(odf_reader::odf_read_context & odfContext);
    ~num_format_context();

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
