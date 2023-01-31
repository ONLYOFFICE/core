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

#include "xlsx_num_format_context.h"
#include "xlsx_complex_number_format.h"

#include <sstream>
#include <map>

#include "../Format/odfcontext.h"


namespace cpdoccore {
namespace oox {


class num_format_context::Impl2
{
public:
    complex_number_format complex_number_format_;
};

class num_format_context::Impl
{
public:
    Impl(): in_convert_style_(false)
    {}

    std::wstring current_style_name_;
    std::wstringstream stream_;

    void reset_current()
    {
        current_style_name_ = L"";
        stream_.str(std::wstring());  
        in_convert_style_ = false;
		type_ = odf_types::office_value_type::Custom;
    }

    std::wstring last_format_;
	std::wstring last_date_format_ = L"yyyy.mmmm.dd";
	std::wstring last_time_format_ = L"hh:mm:ss";

	odf_types::office_value_type type_ = odf_types::office_value_type::Custom;

    bool in_convert_style_;
};

num_format_context::num_format_context(odf_reader::odf_read_context & odfContext): odf_context_(odfContext), impl_(new Impl()), impl2_(new Impl2())
{
}

num_format_context::~num_format_context()
{
}

void num_format_context::start_format(const std::wstring & style_name)
{
	impl_->reset_current();
    impl_->current_style_name_ = style_name;
    impl_->in_convert_style_ = true;
}

std::wostream & num_format_context::output()
{
    return impl_->stream_;
}

void num_format_context::end_format()
{    
    impl_->last_format_ = impl_->stream_.str();

	if (impl_->type_ == odf_types::office_value_type::Date)
		impl_->last_date_format_ = impl_->last_format_;
	
	if (impl_->type_ == odf_types::office_value_type::Time)
		impl_->last_time_format_ = impl_->last_format_;
}

std::wstring num_format_context::get_last_format() const
{
    return impl_->last_format_;
}
std::wstring num_format_context::get_last_date_format() const
{
    return impl_->last_date_format_;
}
std::wstring num_format_context::get_last_time_format() const
{
    return impl_->last_time_format_;
}
void num_format_context::start_complex_format(std::wstring const & name)
{
    impl2_->complex_number_format_.reset();
	impl2_->complex_number_format_.name = name;
}
odf_types::office_value_type::type num_format_context::type() const
{
    return impl_->type_.get_type();
}
void num_format_context::type(odf_types::office_value_type::type t)
{
    impl_->type_ = t;
}
void num_format_context::add_format(std::wstring const & cond, std::wstring const & format)
{
    impl2_->complex_number_format_.add_format(cond, format);
}

void num_format_context::end_complex_format()
{
    impl_->last_format_ = impl2_->complex_number_format_.result_value();

	mapFormats.insert(std::make_pair(impl2_->complex_number_format_.name, std::make_pair(impl_->type_.get_type(), impl_->last_format_)));
}
std::wstring num_format_context::find_complex_format(std::wstring const & name, odf_types::office_value_type::type & type)
{
	std::map<std::wstring, std::pair<odf_types::office_value_type::type, std::wstring>>::iterator pFind = mapFormats.find(name);

	if (pFind != mapFormats.end())
	{
		type = pFind->second.first;
		return pFind->second.second;
	}
	else
		return L"";
}
}
}
