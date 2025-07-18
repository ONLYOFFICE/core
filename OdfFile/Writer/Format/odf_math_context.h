/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include <set>
#include <CPOptional.h>
#include "office_elements_create.h"
#include <fstream>
//#include "../../DataTypes/mathclass.h"


#define CREATE_MATH_TAG(tag)\
	odf_writer::office_element_ptr elm;\
	odf_writer::create_element(L"math", tag, elm, odf_context());\
	odf_context()->math_context()->current_tag = tag;	
//odf_context()->math_context()->debug_stream << tag << "\n";

#define OPEN_MATH_TAG(elm)\
	odf_context()->math_context()->tagFlag.push_back(odf_context()->math_context()->start_element(elm));\
	if (odf_context()->math_context()->current_tag == L"mn")\
	{\
		typedef odf_writer::math_mn* T;\
		T tmp = dynamic_cast<T>(elm.get());\
		odf_context()->math_context()->symbol_counter += tmp->text_.get().size();\
	}\
	if (odf_context()->math_context()->current_tag == L"mi")\
	{\
		typedef odf_writer::math_mi* T;\
		T tmp = dynamic_cast<T>(elm.get());\
		odf_context()->math_context()->symbol_counter += tmp->text_.get().size();\
	}\
	if (odf_context()->math_context()->current_tag == L"mo")\
	{\
		typedef odf_writer::math_mo* T;\
		T tmp = dynamic_cast<T>(elm.get());\
		odf_context()->math_context()->symbol_counter += tmp->text_.get().size();\
	}\
	if (odf_context()->math_context()->tagFlag.back())\
	{\
		odf_context()->math_context()->counter++;\
	}\
	odf_context()->math_context()->current_tag = L"";

//odf_context()->math_context()->debug_stream << L"open, counter is " << odf_context()->math_context()->counter << "\n";

#define CLOSE_MATH_TAG\
	if(odf_context()->math_context()->tagFlag.back()) \
	{\
		odf_context()->math_context()->end_element();\
		odf_context()->math_context()->counter--; \
	}\
	odf_context()->math_context()->tagFlag.pop_back(); 

	//odf_context()->math_context()->debug_stream /*std::wcout*/ << L"close, counter is " << odf_context()->math_context()->counter << "\n";

namespace cpdoccore {
	namespace odf_writer
	{
		class odf_conversion_context;
		class odf_text_context;
		class odf_drawing_context;

		class odf_style_context;
		typedef shared_ptr<odf_style_context>::Type odf_style_context_ptr;

		class odf_math_context
		{
		public:
			odf_math_context(odf_conversion_context* odf_context);
			~odf_math_context();

			void set_styles_context(odf_style_context_ptr style_context);

			odf_drawing_context* drawing_context();
			odf_text_context* text_context();

			void start_math(office_element_ptr& root);
			void end_math();

			void add_content(const std::wstring& content);

			bool start_element(office_element_ptr& elm); 
			void end_element();

			std::vector<std::vector<std::wstring>> brackets;
			int lvl_of_me = 0;
			int matrix_row_counter = 0;
			std::vector<int> end_counter;
			bool style_flag = 0;
			int counter = 0;
			std::wstring current_tag;
			int symbol_counter = 0;
			double lvl_counter = 0;
			double lvl_up_counter = 0;
			double lvl_down_counter = 0;
			double lvl_max = 0;
			double lvl_min = 0;

			std::wstring font_name;
			double font_size = 0;
			std::wstring font_color;

			std::set<wchar_t> mo;
			std::map<std::wstring, std::wstring> diak_symbols;

			bool isEmpty();
			std::vector<bool> tagFlag;

			bool in_text_box_ = false;
		private:
			class Impl;
			_CP_PTR(Impl) impl_;
		};

	}
}
