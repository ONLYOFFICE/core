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

#include "logging.h"

#include <boost/algorithm/string.hpp>

#include "../../../ASCOfficeOdfFile/formulasconvert/formulasconvert.h"

#include <iostream>

#include "odf_math_context.h"
#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_drawing_context.h"

#include "odf_conversion_context.h"

#include "ods_table_state.h"//???

#include "math_elements.h"
#include "math_layout_elements.h"



namespace cpdoccore
{
using namespace odf_types;

namespace odf_writer
{
	static const std::wstring default_MS_series_colors[] =
	{
		L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934",
		//todooo - продолжить .... пока копи€ первых
		L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934",
		L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934",
		L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934",
		L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934",
		L"#355a86", L"#883533", L"#6e963c", L"#594573", L"#327a8d", L"#3d679a", L"#9d3e3b", L"#7e9945", L"#674f84", L"#398ba2", L"#cb7934"
	};


	struct 	odf_cash_state
	{
		std::wstring ref;
		std::wstring format_code;

		bool categories;
		bool label;

		std::vector<std::wstring>	data_str;
	};
	struct 	odf_math_state
	{
		odf_math_state() { clear(); }
		void clear()
		{
			elements_.clear();
			math_width_pt = math_height_pt = 0;
		}
		double math_width_pt;
		double math_height_pt; //chart_height_pt

		std::vector<odf_element_state>	elements_;
	};

	struct odf_math_level_state
	{
		style_text_properties		*text_properties_;	
		style_paragraph_properties	*paragraph_properties_;	

		office_element_ptr	elm;
	};

	struct odf_category_state
	{
		std::wstring ref;
		std::wstring format_code;
		int type;

	};

	class odf_math_context::Impl
	{
		public: 
			Impl(odf_conversion_context *odf_context) : odf_context_(odf_context)
			{							
				current_paragraph_properties = NULL;
				current_text_properties = NULL;

				
				//некоторые свойства дл€ объектов графики не поддерживаюис€ в редакторах Libre && OpenOffice.net
											  //в MS Office и в нашем - проблем таких нет.
			}
			~Impl();
			odf_math_state						current_math_state_;
			void								clear_current();
		
			style_paragraph_properties			*current_paragraph_properties;
			style_text_properties				*current_text_properties;
			std::vector<odf_math_level_state>	current_level_;	//посто€нно мен€ющийс€ список уровней наследовани€
			std::vector<odf_math_state>			math_list_;		//все элементы .. дл€ удобства разделение по "топам"
			odf_conversion_context				*odf_context_;
			office_math							*root_element_;
	};

	void odf_math_context::Impl::clear_current()
	{
		current_math_state_.clear();
	}

	static formulasconvert::oox2odf_converter formulas_converter_math;

	odf_math_context::odf_math_context(odf_conversion_context *odf_context)
		: impl_(new  odf_math_context::Impl(odf_context))
	{
	}

	odf_math_context::~odf_math_context()
	{
	}


	odf_drawing_context * odf_math_context::drawing_context()
	{
		return impl_->odf_context_->drawing_context();
	}

	odf_text_context	* odf_math_context::text_context()
	{
		return impl_->odf_context_->text_context();
	}

	void odf_math_context::start_math(office_element_ptr & root)
	{
		impl_->root_element_ = dynamic_cast<office_math*>(root.get());

		office_element_ptr math_elm;
		create_element(L"math", L"math", math_elm, impl_->odf_context_);		


		root->add_child_element(math_elm);

		size_t level = impl_->current_level_.size();

		odf_element_state		state(math_elm, NULL, NULL, level);		
		odf_math_level_state	level_state = { NULL, NULL, math_elm };

		drawing_context()->start_element(math_elm);

		impl_->current_level_.push_back(level_state);
		impl_->current_math_state_.elements_.push_back(state);
	}

	void odf_math_context::start_element(office_element_ptr & elm)
	{		
		size_t level = impl_->current_level_.size();

		odf_element_state		state(elm, NULL, NULL, level);
		odf_math_level_state	level_state = { NULL, NULL, elm };
		
		drawing_context()->start_element(elm, NULL);		

		impl_->current_level_.push_back(level_state);
		impl_->current_math_state_.elements_.push_back(state);
	}

	void odf_math_context::end_element()
	{
		impl_->current_level_.pop_back();
	}
	
	void odf_math_context::end_math()
	{
		if (impl_->current_math_state_.elements_.size() < 1) return;

		

		end_element();
		///////////////////


		

		impl_->clear_current();
	}


	//void odf_math_context::start_group_series()
	//{
	//}

}
}