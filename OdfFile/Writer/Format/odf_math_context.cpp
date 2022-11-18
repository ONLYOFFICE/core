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

#include "../../Formulas/formulasconvert.h"

#include <iostream>

#include "odf_math_context.h"
#include "odf_text_context.h"
#include "odf_style_context.h"
#include "odf_drawing_context.h"

#include "odf_conversion_context.h"

#include "ods_table_state.h"

#include "math_elements.h"
#include "math_layout_elements.h"
#include "math_table_elements.h"
#include "math_limit_elements.h"
#include "math_token_elements.h"
//#include "math_elementaries.h"



namespace cpdoccore
{
using namespace odf_types;

namespace odf_writer
{
	struct 	odf_math_state
	{
		odf_math_state() { }
		void clear()
		{
			elements_.clear();
		}
		std::vector<odf_element_state>	elements_;
	};

	struct odf_math_level_state
	{
		style_text_properties		*text_properties_;	
		style_paragraph_properties	*paragraph_properties_;	

		office_element_ptr	elm;
	};

	class odf_math_context::Impl
	{
		public: 
			Impl(odf_conversion_context *odf_context) : odf_context_(odf_context)
			{							
				styles_context_ = NULL;				
				current_paragraph_properties = NULL;
				current_text_properties = NULL;
			}
			odf_math_state						current_math_state_;
			void								clear_current();
		
			style_paragraph_properties			*current_paragraph_properties;
			style_text_properties				*current_text_properties;
			std::vector<odf_math_level_state>	current_level_;	//постоянно меняющийся список уровней наследования
			std::vector<odf_math_state>			math_list_;		//все элементы .. для удобства разделение по "топам"
			odf_conversion_context				*odf_context_;
			office_math							*root_element_;
			
			odf_style_context					*styles_context_;
	};

	void odf_math_context::Impl::clear_current()
	{
		current_math_state_.clear();
	}

	static formulasconvert::oox2odf_converter formulas_converter_math;

	odf_math_context::odf_math_context(odf_conversion_context *odf_context)
		: impl_(new  odf_math_context::Impl(odf_context)), lvl_of_me(0), counter(0), annotation_flag(true), matrix_row_counter(0)
	{
		mo = { L'+', L'-', L'±', L'∓', L'∙', L'×', L'∗', L'÷', L'/', L'≂', L'⊕', L'⊖', L'⊙', L'⊗', L'⊘', L'∘', L'¬', L'∧', L'∨',		// un/bi operators
				L'=', L'≠', L'<', L'≤', L'>', L'≥', L'≪', L'≫', L'≈', L'~', L'≃', L'≡', L'∝', L'∥', L'⟂', L'|', L'∤', L'→', L'⊷',	// relations
				L'⊶', L'≝', L'⇐', L'⇔', L'⇒', L'≺', L'≻',  L'≼', L'≽', L'≾', L'≿',  L'⊀', L'⊁',										// relationships over sets
				L'∈', L'∉', L'∋', L'∩', L'∪', L'/', L'⊂', L'⊆', L'⊃', L'⊇', L'⊄', L'⊈', L'⊅', L'⊉',						//
				L'∞', L'∂', L'∇', L'∃', L'∄', L'∀', L'ħ', L'ƛ', L'ℜ', L'ℑ', L'℘', L'ℒ', L'ℱ', L'←', L'→', L'↑', L'↓',					// others
				L'…', L'⋯', L'⋮', L'⋰', L'⋱', L'∞', L'→',
				L'∫', L'∬', L'∭', L'∮', L'∯', L'∰',
				L'∑', L'∏', L'∐', L'⋃', L'⋂', L'⋀', L'⋁', L'≝', L'≞', L'≜',
				L'˙', L'¨', L'⃛', L'̂', L'̌', L'́'//, L'̆', L'', L'', L'', L'', L'',
		};

		diak_symbols = { {L"̇", L"˙"}, {L"̈", L"¨"}, {L"⃛", L"⃛"}, {L"̂", L"&#708;"}, {L"̌", L"ˇ"}, {L"́", L"´"}, {L"̀", L"&#715;"}, {L"̆", L"˘"}, {L"̃", L"~"}, {L"̅", L"¯"},
						{L"̿", L"═"}, {L"⏞", L"⏞"}, {L"⃖", L"←"}, {L"⃗", L"→"}, {L"⃡", L"↔"}, {L"⃐", L"↼"}, {L"⃑", L"⇀"}, {L"̲", L"&#45;"}
						//{L'', L''}, { L'', L'' }, { L'', L'' }, { L'', L'' }, { L'', L'' }, { L'', L'' }, { L'', L'' }
		};

		annotation_diak_symbols = { {L"˙",L"dot"}, {L"¨",L"ddot"}, {L"⃛",L"dddot"}, {L"&#708;", L"hat"}, {L"ˇ",L"check"}, {L"´",L"acute"}, {L"&#715;",L"grave"}, {L"˘",L"breve"},
			                        {L"~",L"tilde"},{L"¯",L"overline"},{L"→",L"vec"}, {L"⇀",L"harpoon"}, {L"&#45;",L"unnderline"}/*, {L"",L""}, {L"",L""}, {L"",L""}, {L"",L""}, {L"",L""}, {L"",L""},{L"",L""}, 
			                        {L"",L""}*/
		};

		annotation_operators = { {L"∫",L"int"}, {L"∬", L"iint "}, {L"∭", L"iiint "}, {L"∮", L"lint "}, {L"∯", L"llint "},
								 {L"∰", L"lllint "}, {L"∑", L"sum "}, {L"∏", L"prod "}, {L"∐", L"coprod "}
								
		};

		annotation_brackets_begin = { {L"(", L"("}, {L"[", L"["}, {L"{", L"lbrace"}, {L"⟨", L"langle"}, {L"〈", L"langle"}, {L"⌊", L"lfloor"}, {L"⌈", L"lceil"}, {L"|", L"lline"}, {L"‖", L"ldline"},
									  {L"]", L"]"}, {L"⟦", L"ldbracket"}
		};
		annotation_brackets_end   = { {L")", L")"}, {L"]", L"]"}, {L"}", L"rbrace"}, {L"⟩", L"rangle"}, {L"〉", L"rangle"},{L"⌋", L"rfloor"}, {L"⌉", L"rceil"}, {L"|", L"rline"}, {L"‖", L"rdline"},
									  {L"[", L"["}, {L"⟧", L"rdbracket"}
		};
		//debug_stream.open(debug_fileName);
	}

	odf_math_context::~odf_math_context()
	{
		//debug_stream.close();
	}

	void odf_math_context::set_styles_context(odf_style_context * style_context)
	{
		impl_->styles_context_ = style_context;

		impl_->odf_context_->drawing_context()->set_styles_context(style_context);
	}

	odf_drawing_context * odf_math_context::drawing_context()
	{
		return impl_->odf_context_->drawing_context();
	}

	odf_text_context * odf_math_context::text_context()
	{
		return impl_->odf_context_->text_context();
	}

	void odf_math_context::start_math(office_element_ptr & root)
	{
		impl_->root_element_ = dynamic_cast<office_math*>(root.get());

		if (!impl_->root_element_) return;

		impl_->root_element_->create_child_element(L"math", L"semantics");

		size_t level = impl_->current_level_.size();

		odf_math_level_state	level_state = { NULL, NULL, root };
		odf_element_state state(root, L"", office_element_ptr(), level);

		impl_->current_level_.push_back(level_state);
		impl_->current_math_state_.elements_.push_back(state);
	}

	bool odf_math_context::start_element(office_element_ptr & elm)
	{		

		if (!elm)
			return false;
		impl_->current_level_.back().elm->add_child_element(elm);

		size_t level = impl_->current_level_.size();

		odf_element_state		state(elm, L"", office_element_ptr(), level);
		odf_math_level_state	level_state = { NULL, NULL, elm };
		
		impl_->current_level_.push_back(level_state);
		impl_->current_math_state_.elements_.push_back(state);
		return true;
	}

	void odf_math_context::end_element()
	{
		impl_->current_level_.pop_back();
	}
	bool odf_math_context::isEmpty()
	{
		return impl_->current_level_.empty();
	}
	void odf_math_context::end_math()
	{
		if (impl_->current_math_state_.elements_.empty()) return;

		

		end_element();
		///////////////////


		

		impl_->clear_current();
	}
}
}
