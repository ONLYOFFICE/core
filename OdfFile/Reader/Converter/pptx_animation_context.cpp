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

#include "pptx_animation_context.h"

#include <xml/simple_xml_writer.h>
#include <boost/algorithm/string.hpp>

#include "../../DataTypes/clockvalue.h"

#define SET_PAR_ANIMATION_ATTRIBUTE(attribute, value)							\
	if(impl_->par_animation_levels_.size())										\
	{																			\
		Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();	\
		back->attribute = value;												\
	}

#define SET_SEQ_ANIMATION_ATTRIBUTE(attribute, value)							\
	if (impl_->par_animation_levels_.size())									\
	{																			\
		Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();	\
		if (back->AnimSeq.size())												\
			back->AnimSeq.back()->attribute = value;							\
	}

#define END_BEHAVIOUR_ELEMENT(element_description)								\
	if (impl_->par_animation_levels_.size())									\
	{																			\
		Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();	\
		if(impl_->element_description->ShapeID &&								\
			*impl_->element_description->ShapeID != 0)							\
			back->AnimationActionArray.push_back(impl_->element_description);	\
	}																			\
	impl_->element_description = nullptr


namespace cpdoccore {
namespace oox {

	pptx_animation_context::Impl::Impl()
	{
		clear();
	}

	void pptx_animation_context::Impl::clear()
	{
		par_animation_levels_.clear();
		root_animation_element_		= nullptr;
		animate_motion_description_ = nullptr;
		set_description_			= nullptr;
		anim_effect_description_	= nullptr;
		anim_description_			= nullptr;
		anim_clr_description_		= nullptr;
		anim_scale_description_		= nullptr;
		anim_rotate_description_	= nullptr;
		audio_description_			= nullptr;
	}

	pptx_animation_context::pptx_animation_context()
		: impl_(new pptx_animation_context::Impl())
	{

	}

	void pptx_animation_context::start_par_animation()
	{
		if (!impl_->root_animation_element_)
		{
			impl_->root_animation_element_ = boost::make_shared<Impl::_par_animation>();
			impl_->par_animation_levels_.push_back(impl_->root_animation_element_);
		}
		else
		{
			impl_->par_animation_levels_.push_back(boost::make_shared<Impl::_par_animation>());
		}
	}

	void pptx_animation_context::set_par_animation_presentation_node_type(const std::wstring& value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(NodeType, value);
	}

	void pptx_animation_context::set_par_animation_direction(const std::wstring& value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(Direction, value);
	}

	void pptx_animation_context::set_par_animation_restart(const std::wstring& value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(Restart, value);
	}

	void pptx_animation_context::set_par_animation_duration(int value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(Duration, value);
	}

	void pptx_animation_context::set_par_animation_delay(const std::wstring& value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(Delay, value);
	}

	void pptx_animation_context::set_par_animation_end(const std::wstring& value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(End, value);
	}

	void pptx_animation_context::set_par_animation_preset_class(const std::wstring& value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(PresetClass, value);
	}

	void pptx_animation_context::set_par_animation_preset_id(int value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(PresetID, value);
	}

	void pptx_animation_context::set_par_animation_preset_subtype(int value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(PresetSubtype, value);
	}

	void pptx_animation_context::set_par_animation_fill(const std::wstring& value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(Fill, value);
	}

	void pptx_animation_context::set_par_animation_accelerate(int value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(Accelerate, value);
	}

	void pptx_animation_context::set_par_animation_decelerate(int value)
	{
		SET_PAR_ANIMATION_ATTRIBUTE(Decelerate, value);
	}

	void pptx_animation_context::end_par_animation()
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr end = impl_->par_animation_levels_.back();
			impl_->par_animation_levels_.pop_back();
			if (!impl_->par_animation_levels_.size())
				return;

			if (!end->AnimationActionArray.size() &&
				!end->AnimParArray.size()  &&
				!end->AnimSeq.size())
				return;
			
			Impl::_par_animation_ptr back = impl_->par_animation_levels_.back();

			if (back->AnimSeq.size())
				back->AnimSeq.back()->AnimParArray.push_back(end);
			else
				back->AnimParArray.push_back(end);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// p:seq
	void pptx_animation_context::start_seq_animation()
	{
		if (impl_->par_animation_levels_.size())
		{
			impl_->par_animation_levels_.back()->AnimSeq.push_back(boost::make_shared<Impl::_seq_animation>());
		}
	}

	void pptx_animation_context::set_seq_animation_presentation_node_type(const std::wstring& value)
	{
		SET_SEQ_ANIMATION_ATTRIBUTE(PresentationNodeType, value);
	}

	void pptx_animation_context::set_seq_animation_direction(const std::wstring& value)
	{
		SET_SEQ_ANIMATION_ATTRIBUTE(Direction, value);
	}

	void pptx_animation_context::set_seq_animation_restart(const std::wstring& value)
	{
		SET_SEQ_ANIMATION_ATTRIBUTE(Restart, value);
	}

	void pptx_animation_context::set_seq_animation_dur(int value)
	{
		SET_SEQ_ANIMATION_ATTRIBUTE(Duration, value);
	}

	void pptx_animation_context::set_seq_animation_delay(const std::wstring& value)
	{
		SET_SEQ_ANIMATION_ATTRIBUTE(Delay, value);
	}

	void pptx_animation_context::set_seq_animation_end(const std::wstring& value)
	{
		SET_SEQ_ANIMATION_ATTRIBUTE(End, value);
	}

	void pptx_animation_context::set_seq_animation_target_element(const std::wstring& value)
	{
		SET_SEQ_ANIMATION_ATTRIBUTE(TargetEl, value);
	}

	void pptx_animation_context::end_seq_animation()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// p:set
	void pptx_animation_context::start_set()
	{
		impl_->set_description_ = boost::make_shared<Impl::_set>();
	}

	void pptx_animation_context::set_set_direction(const std::wstring& value)
	{
		impl_->set_description_->Direction = value;
	}


	void pptx_animation_context::set_set_restart(const std::wstring& value)
	{
		impl_->set_description_->Restart = value;
	}

	void pptx_animation_context::set_set_duration(int value)
	{
		impl_->set_description_->Duration = value;
	}

	void pptx_animation_context::set_set_delay(const std::wstring& value)
	{
		impl_->set_description_->Delay = value;
	}

	void pptx_animation_context::set_set_end(const std::wstring& value)
	{
		impl_->set_description_->End = value;
	}

	void pptx_animation_context::set_set_auto_rev(const std::wstring& value)
	{
		impl_->set_description_->AutoRev = value;
	}

	void pptx_animation_context::set_set_fill(const std::wstring& value)
	{
		impl_->set_description_->Fill = value;
	}

	void pptx_animation_context::set_set_shape_id(size_t value)
	{
		impl_->set_description_->ShapeID = value;
	}

	void pptx_animation_context::set_set_attribute_name(const std::wstring& value)
	{
		impl_->set_description_->AttributeName = value;
	}

	void pptx_animation_context::set_set_to_value(const std::wstring& value)
	{
		impl_->set_description_->ToValue = value;
	}

	void pptx_animation_context::end_set()
	{
		END_BEHAVIOUR_ELEMENT(set_description_);
	}

	//////////////////////////////////////////////////////////////////////////
	// p:animEffect
	void pptx_animation_context::start_anim_effect()
	{
		impl_->anim_effect_description_ = boost::make_shared<Impl::_anim_effect>();
	}

	void pptx_animation_context::set_anim_effect_filter(const std::wstring& value)
	{
		impl_->anim_effect_description_->Filter = value;
	}

	void pptx_animation_context::set_anim_effect_transition(const std::wstring& value)
	{
		impl_->anim_effect_description_->Transition = value;
	}

	void pptx_animation_context::set_anim_effect_duration(int value)
	{
		impl_->anim_effect_description_->Duration = value;
	}

	void pptx_animation_context::set_anim_effect_delay(const std::wstring& value)
	{
		impl_->anim_effect_description_->Delay = value;
	}

	void pptx_animation_context::set_anim_effect_accel(int value)
	{
		impl_->anim_effect_description_->Accel = value;
	}

	void pptx_animation_context::set_anim_effect_decel(int value)
	{
		impl_->anim_effect_description_->Decel = value;
	}

	void pptx_animation_context::set_anim_effect_shape_id(size_t value)
	{
		impl_->anim_effect_description_->ShapeID = value;
	}

	void pptx_animation_context::end_anim_effect()
	{

		END_BEHAVIOUR_ELEMENT(anim_effect_description_);
	}

	//////////////////////////////////////////////////////////////////////////
	// p:animMotion
	void pptx_animation_context::start_animate_motion()
	{
		impl_->animate_motion_description_ = boost::make_shared<Impl::_animate_motion>();
	}

	void pptx_animation_context::set_animate_motion_presentation_node_type(const std::wstring& value)
	{
		impl_->animate_motion_description_->PresentationNodeType = value;
	}

	void pptx_animation_context::set_animate_motion_direction(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilDirection = value;
	}

	void pptx_animation_context::set_animate_motion_restart(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilRestart = value;
	}

	void pptx_animation_context::set_animate_motion_dur(int value)
	{
		impl_->animate_motion_description_->SmilDurMs = value;
	}

	void pptx_animation_context::set_animate_motion_delay(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilBegin = value;
	}

	void pptx_animation_context::set_animate_motion_end(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilEnd = value;
	}

	void pptx_animation_context::set_animate_motion_fill(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilFill = value;
	}

	void pptx_animation_context::set_animate_motion_shape_id(size_t value)
	{
		impl_->animate_motion_description_->ShapeID = value;
	}

	void pptx_animation_context::set_animate_motion_svg_path(const std::wstring& value)
	{
		impl_->animate_motion_description_->SvgPath = value;
	}

	void pptx_animation_context::end_animate_motion()
	{
		END_BEHAVIOUR_ELEMENT(animate_motion_description_);
	}

	//////////////////////////////////////////////////////////////////////////
	// p:anim
	void pptx_animation_context::start_animate()
	{
		impl_->anim_description_ = boost::make_shared<Impl::_anim>();
		impl_->anim_description_->KeypointArray = std::vector<Impl::_anim::_keypoint>();
	}

	void pptx_animation_context::set_animate_calc_mode(const std::wstring& value)
	{
		impl_->anim_description_->CalcMode = value;
	}

	void pptx_animation_context::set_animate_value_type(const std::wstring& value)
	{
		impl_->anim_description_->ValueType = value;
	}

	void pptx_animation_context::set_animate_shape_id(size_t value)
	{
		impl_->anim_description_->ShapeID = value;
	}

	void pptx_animation_context::set_animate_duration(int value)
	{
		impl_->anim_description_->Duration = value;
	}

	void pptx_animation_context::set_animate_attribute_name(const std::wstring& value)
	{
		impl_->anim_description_->AttributeName = value;
	}

	void pptx_animation_context::set_animate_from(const std::wstring& value)
	{
		impl_->anim_description_->From = value;
	}

	void pptx_animation_context::set_animate_to(const std::wstring& value)
	{
		impl_->anim_description_->To = value;
	}

	void pptx_animation_context::set_animate_by(const std::wstring& value)
	{
		impl_->anim_description_->By = value;
	}

	void pptx_animation_context::set_animate_additive(const std::wstring& value)
	{
		impl_->anim_description_->Additive = value;
	}

	void pptx_animation_context::set_animate_auto_reverse(bool value)
	{
		impl_->anim_description_->AutoReverse = value;
	}

	void pptx_animation_context::set_animate_delay(const std::wstring& value)
	{
		impl_->anim_description_->Delay = value;
	}

	void pptx_animation_context::add_animate_keypoint(int time, const std::wstring& value, _CP_OPT(std::wstring) formula)
	{
		impl_->anim_description_->KeypointArray->push_back(Impl::_anim::_keypoint(time, value, formula));
	}

	void pptx_animation_context::end_animate()
	{
		END_BEHAVIOUR_ELEMENT(anim_description_);
	}

	//////////////////////////////////////////////////////////////////////////
	// p:animClr
	void pptx_animation_context::start_animate_color()
	{
		impl_->anim_clr_description_ = boost::make_shared<Impl::_anim_clr>();
	}

	void pptx_animation_context::set_animate_color_color_space(const std::wstring& value)
	{
		impl_->anim_clr_description_->ColorSpace = value;
	}

	void pptx_animation_context::set_animate_color_dir(const std::wstring& value)
	{
		impl_->anim_clr_description_->Direction = value;
	}

	void pptx_animation_context::set_animate_color_duration(int value)
	{
		impl_->anim_clr_description_->Duration = value;
	}

	void pptx_animation_context::set_animate_color_delay(const std::wstring& value)
	{
		impl_->anim_clr_description_->Delay = value;
	}

	void pptx_animation_context::set_animate_color_fill(const std::wstring& value)
	{
		impl_->anim_clr_description_->Fill = value;
	}

	void pptx_animation_context::set_animate_color_auto_rev(bool value)
	{
		impl_->anim_clr_description_->AutoRev = value;
	}

	void pptx_animation_context::set_animate_color_attribute_name(const std::wstring& value)
	{
		impl_->anim_clr_description_->AttributeName = value;
	}

	void pptx_animation_context::set_animate_color_to_value(const std::wstring& value)
	{
		impl_->anim_clr_description_->ToValue = value;
	}

	void pptx_animation_context::set_animate_color_by_value(const std::wstring& value)
	{
		impl_->anim_clr_description_->ByValue = pptx_animation_context::Impl::_anim_clr::color();

		if (boost::algorithm::starts_with(value, L"#"))
		{
			if (value.size() != std::wstring(L"#rrggbb").size())
				return;

			const std::wstring str = value.substr(1); // Remove # character

			int r = 0;
			int g = 0;
			int b = 0;

			std::wistringstream(str.substr(0, 2)) >> std::hex >> r;
			std::wistringstream(str.substr(2, 2)) >> std::hex >> g;
			std::wistringstream(str.substr(4, 2)) >> std::hex >> b;

			impl_->anim_clr_description_->ByValue->type_ = pptx_animation_context::Impl::_anim_clr::color::rgb;
			impl_->anim_clr_description_->ByValue->v1 = r;
			impl_->anim_clr_description_->ByValue->v2 = g;
			impl_->anim_clr_description_->ByValue->v3 = b;
		}
		else if (boost::algorithm::starts_with(value, L"hsl"))
		{
			std::wstring str = value;
			boost::algorithm::erase_all(str, L"hsl");
			boost::algorithm::erase_all(str, L"(");
			boost::algorithm::erase_all(str, L")");
			boost::algorithm::erase_all(str, L"%");
			std::vector<std::wstring> arr;
			boost::algorithm::split(arr, str, boost::is_any_of(","));

			int h = 0;
			int s = 0;
			int l = 0;

			std::wistringstream(arr[0]) >> h;
			std::wistringstream(arr[1]) >> s;
			std::wistringstream(arr[2]) >> l;

			impl_->anim_clr_description_->ByValue->type_ = pptx_animation_context::Impl::_anim_clr::color::hsl;
			impl_->anim_clr_description_->ByValue->v1 = h * 60000;
			impl_->anim_clr_description_->ByValue->v2 = s * 1000;
			impl_->anim_clr_description_->ByValue->v3 = l * 1000;
		}
	}

	void pptx_animation_context::set_animate_color_shape_id(size_t value)
	{
		impl_->anim_clr_description_->ShapeID = value;
	}

	void pptx_animation_context::end_animate_color()
	{
		END_BEHAVIOUR_ELEMENT(anim_clr_description_);
	}

	//////////////////////////////////////////////////////////////////////////
	// p:animScale
	void pptx_animation_context::start_animate_scale()
	{
		impl_->anim_scale_description_ = boost::make_shared<Impl::_anim_scale>();
	}

	void pptx_animation_context::set_animate_scale_shape_id(size_t value)
	{
		impl_->anim_scale_description_->ShapeID = value;
	}

	void pptx_animation_context::set_animate_scale_duration(int value)
	{
		impl_->anim_scale_description_->Duration = value;
	}

	void pptx_animation_context::set_animate_scale_fill(const std::wstring& value)
	{
		impl_->anim_scale_description_->Fill = value;
	}

	void pptx_animation_context::set_animate_scale_from(int x, int y)
	{
		impl_->anim_scale_description_->From = Impl::_anim_scale::vec2(x, y);
	}

	void pptx_animation_context::set_animate_scale_to(int x, int y)
	{
		impl_->anim_scale_description_->To = Impl::_anim_scale::vec2(x, y);
	}

	void pptx_animation_context::set_animate_scale_by(int x, int y)
	{
		impl_->anim_scale_description_->By = Impl::_anim_scale::vec2(x, y);
	}

	void pptx_animation_context::set_animate_scale_delay(const std::wstring& value)
	{
		impl_->anim_scale_description_->Delay = value;
	}

	void pptx_animation_context::set_animate_scale_attribute_name(const std::wstring& value)
	{
		impl_->anim_scale_description_->AttributeName = value;
	}

	void pptx_animation_context::set_animate_scale_auto_reverse(bool value)
	{
		impl_->anim_scale_description_->AutoReverse = value;
	}

	void pptx_animation_context::end_animate_scale()
	{
		END_BEHAVIOUR_ELEMENT(anim_scale_description_);
	}

	//////////////////////////////////////////////////////////////////////////
	// p:animRot
	void pptx_animation_context::start_animate_rotate()
	{
		impl_->anim_rotate_description_ = boost::make_shared<Impl::_anim_rotate>();
	}

	void pptx_animation_context::set_animate_rotate_shape_id(size_t value)
	{
		impl_->anim_rotate_description_->ShapeID = value;
	}

	void pptx_animation_context::set_animate_rotate_duration(int value)
	{
		impl_->anim_rotate_description_->Duration = value;
	}

	void pptx_animation_context::set_animate_rotate_fill(const std::wstring& value)
	{
		impl_->anim_rotate_description_->Fill = value;
	}

	void pptx_animation_context::set_animate_rotate_by(int value)
	{
		impl_->anim_rotate_description_->By = value;
	}

	void pptx_animation_context::set_animate_rotate_attribute_name(const std::wstring& value)
	{
		impl_->anim_rotate_description_->AttributeName = value;
	}

	void pptx_animation_context::set_animate_rotate_delay(const std::wstring& value)
	{
		impl_->anim_rotate_description_->Delay = value;
	}

	void pptx_animation_context::set_animate_rotate_auto_reverse(bool value)
	{
		impl_->anim_rotate_description_->AutoReverse = value;
	}

	void pptx_animation_context::end_animate_rotate()
	{
		END_BEHAVIOUR_ELEMENT(anim_rotate_description_);
	}

	//////////////////////////////////////////////////////////////////////////
	// p:audio
	void pptx_animation_context::start_anim_audio()
	{
		impl_->audio_description_ = boost::make_shared<Impl::_audio>();
	}

	void pptx_animation_context::end_anim_audio()
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->AnimationActionArray.push_back(impl_->audio_description_);
		}
		impl_->audio_description_ = nullptr;
	}

	void pptx_animation_context::add_anim_audio(const std::wstring& rId, const std::wstring& name)
	{
		impl_->audio_description_->RId = rId;
		impl_->audio_description_->Name = name;
	}

	void pptx_animation_context::serialize(std::wostream& strm)
	{
		if (!impl_->root_animation_element_)
			return;

		if (!impl_->root_animation_element_->AnimationActionArray.size() &&
			!impl_->root_animation_element_->AnimParArray.size())
		{
			if (!impl_->root_animation_element_->AnimSeq.size() ||
				!impl_->root_animation_element_->AnimSeq.back()->AnimParArray.size())
				return;
		}

		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:timing")
			{
				if (impl_->root_animation_element_)
				{
					CP_XML_NODE(L"p:tnLst")
					{
						impl_->root_animation_element_->serialize(CP_XML_STREAM());
					}
				}
			}
		}
	}

	void pptx_animation_context::clear()
	{
		impl_->clear();
	}

	void pptx_animation_context::set_is_slide_animation(bool is_slide_animation)
	{
		impl_->IsSlideAnimation = is_slide_animation;
	}

	bool pptx_animation_context::get_is_slide_animation()
	{
		return impl_->IsSlideAnimation;
	}

	const pptx_animation_context::Impl::_par_animation_ptr& pptx_animation_context::get_root_par_animation() const
	{
		return impl_->root_animation_element_;
	}

	static _CP_OPT(std::wstring) serialize_duration(const _CP_OPT(int) duration)
	{
		_CP_OPT(std::wstring) res = boost::none;

		if (duration)
		{
			if (*duration == -1)
				res = L"indefinite";
			else
				res = std::to_wstring(duration.value());
		}

		return res;
	}

	void pptx_animation_context::Impl::_par_animation::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:par")
			{
				CP_XML_NODE(L"p:cTn")
				{
					CP_XML_ATTR_OPT(L"presetClass"	, PresetClass);
					CP_XML_ATTR_OPT(L"presetID"		, PresetID);
					CP_XML_ATTR_OPT(L"presetSubtype", PresetSubtype);
					CP_XML_ATTR_OPT(L"fill"			, Fill);
					CP_XML_ATTR_OPT(L"accel"		, Accelerate);
					CP_XML_ATTR_OPT(L"decel"		, Decelerate);
					
					if (NodeType)
					{
						if (NodeType.value() == L"tmRoot")
						{
							CP_XML_ATTR(L"nodeType", L"tmRoot");
							CP_XML_ATTR(L"dur", L"indefinite");
							CP_XML_ATTR(L"restart", L"never");
						}
						else
						{
							CP_XML_ATTR(L"nodeType", NodeType.value());
						}
					}

					if (Delay)
					{
						CP_XML_NODE(L"p:stCondLst")
						{
							CP_XML_NODE(L"p:cond")
							{
								CP_XML_ATTR(L"delay", Delay.value());
							}
						}
					}
					
					if (AnimParArray.size() || AnimSeq.size() || AnimationActionArray.size())
					{
						CP_XML_NODE(L"p:childTnLst")
						{
							for (size_t i = 0; i < AnimParArray.size(); i++)
								AnimParArray[i]->serialize(CP_XML_STREAM());

							for (size_t i = 0; i < AnimSeq.size(); i++)
								AnimSeq[i]->serialize(CP_XML_STREAM());

							for (size_t i = 0; i < AnimationActionArray.size(); i++)
								AnimationActionArray[i]->serialize(CP_XML_STREAM());
						}
					}
				}
			}
		}
	}

	void pptx_animation_context::Impl::_seq_animation::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:seq")
			{
				CP_XML_ATTR(L"concurrent", 1);
				CP_XML_ATTR(L"nextAc", L"seek");

				CP_XML_NODE(L"p:cTn")
				{
					if (PresentationNodeType)
					{
						CP_XML_ATTR(L"nodeType", PresentationNodeType.value());
						CP_XML_ATTR(L"dur", L"indefinite");
					}
					else
					{
						CP_XML_ATTR_OPT(L"dur", serialize_duration(Duration));
					}						

					if (TargetEl)
					{
						CP_XML_NODE(L"p:stCondLst")
						{
							CP_XML_NODE(L"p:cond")
							{
								CP_XML_ATTR(L"evt", L"onClick");
								CP_XML_NODE(L"p:tgtEl")
								{
									CP_XML_NODE(L"p:spTgt")
									{
										CP_XML_ATTR(L"spid", TargetEl.value());
									}
								}
							}
						}
					}

					if (AnimParArray.size())
					{
						CP_XML_NODE(L"p:childTnLst")
						{
							for (int i = 0; i < AnimParArray.size(); i++)
							{
								AnimParArray[i]->serialize(CP_XML_STREAM());
							}
						}
					}
				}
				CP_XML_NODE(L"p:prevCondLst")
				{
					CP_XML_NODE(L"p:cond")
					{
						CP_XML_ATTR(L"evt", L"onPrev");
						CP_XML_NODE(L"p:tgtEl")
						{
							CP_XML_NODE(L"p:sldTgt");
						}
					}
				}
				CP_XML_NODE(L"p:nextCondLst")
				{
					CP_XML_NODE(L"p:cond")
					{
						CP_XML_ATTR(L"evt", L"onNext");
						CP_XML_NODE(L"p:tgtEl")
						{
							CP_XML_NODE(L"p:sldTgt");
						}
					}
				}
			}
		}
	}


	void pptx_animation_context::Impl::_set::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:set")
			{
				CP_XML_NODE(L"p:cBhvr")
				{
					CP_XML_NODE(L"p:cTn")
					{
						CP_XML_ATTR_OPT(L"dur", serialize_duration(Duration));
						CP_XML_ATTR_OPT(L"fill", Fill);
						CP_XML_ATTR_OPT(L"autoRev", AutoRev);

						if (Delay)
						{
							CP_XML_NODE(L"p:stCondLst")
							{
								CP_XML_NODE(L"p:cond")
								{
									CP_XML_ATTR(L"delay", Delay.value());
								}
							}
						}
					}

					CP_XML_NODE(L"p:tgtEl")
					{
						CP_XML_NODE(L"p:spTgt")
						{
							size_t shapeId = ShapeID ? ShapeID.value() : 0;
							CP_XML_ATTR(L"spid", shapeId);
						}
					}

					if (AttributeName)
					{
						CP_XML_NODE(L"p:attrNameLst")
						{
							CP_XML_NODE(L"p:attrName")
							{
								CP_XML_STREAM() << AttributeName.value();
							}
						}
					}
				}
				if (ToValue)
				{
					CP_XML_NODE(L"p:to")
					{
						CP_XML_NODE(L"p:strVal")
						{
							CP_XML_ATTR(L"val", ToValue.value());
						}
					}
				}
			}
		}
	}


	void pptx_animation_context::Impl::_anim_effect::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:animEffect")
			{
				CP_XML_ATTR_OPT(L"filter"		, Filter);
				CP_XML_ATTR_OPT(L"transition"	, Transition);

				CP_XML_NODE(L"p:cBhvr")
				{
					if (Duration)
					{
						CP_XML_NODE(L"p:cTn")
						{
							CP_XML_ATTR_OPT(L"dur", serialize_duration(Duration));
							CP_XML_ATTR_OPT(L"accel", Accel);
							CP_XML_ATTR_OPT(L"decel", Decel);

							if (Delay)
							{
								CP_XML_NODE(L"p:stCondLst")
								{
									CP_XML_NODE(L"p:cond")
									{
										CP_XML_ATTR(L"delay", Delay.value());
									}
								}
							}
						}
					}
					if (ShapeID)
					{
						CP_XML_NODE(L"p:tgtEl")
						{
							CP_XML_NODE(L"p:spTgt")
							{
								CP_XML_ATTR(L"spid", ShapeID.value());
							}
						}
					}
				}
			}
		}
	}

	void pptx_animation_context::Impl::_animate_motion::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:animMotion")
			{
				CP_XML_ATTR(L"origin", L"layout");
				CP_XML_ATTR(L"path", SvgPath);

				CP_XML_NODE(L"p:cBhvr")
				{
					CP_XML_NODE(L"p:cTn")
					{
						CP_XML_ATTR_OPT(L"dur", SmilDurMs);
						CP_XML_ATTR(L"fill", L"hold");
					}
					CP_XML_NODE(L"p:tgtEl")
					{
						CP_XML_NODE(L"p:spTgt")
						{
							size_t shapeID = ShapeID ? ShapeID.value() : 0;
							CP_XML_ATTR(L"spid", shapeID);
						}
					}
				}
			}
		}
	}

	void pptx_animation_context::Impl::_anim::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:anim")
			{								   
				CP_XML_ATTR_OPT(L"calcmode", CalcMode);
				CP_XML_ATTR_OPT(L"valueType", ValueType);
				CP_XML_ATTR_OPT(L"from", From);
				CP_XML_ATTR_OPT(L"to", To);
				CP_XML_ATTR_OPT(L"by", By);

				CP_XML_NODE(L"p:cBhvr")
				{
					CP_XML_ATTR_OPT(L"additive", Additive);
					
					CP_XML_NODE(L"p:cTn")
					{
						CP_XML_ATTR_OPT(L"dur", serialize_duration(Duration));

						if (AutoReverse)
						{
							int autoRev = AutoReverse.value() ? 1 : 0;
							CP_XML_ATTR(L"autoRev", autoRev);
						}

						if (Delay)
						{
							CP_XML_NODE(L"p:stCondLst")
							{
								CP_XML_NODE(L"p:cond")
								{
									CP_XML_ATTR(L"delay", Delay.value());
								}
							}
						}
					}
					CP_XML_NODE(L"p:tgtEl")
					{
						CP_XML_NODE(L"p:spTgt")
						{
							size_t shapeID = ShapeID ? ShapeID.value() : 0;
							CP_XML_ATTR(L"spid", shapeID);
						}
					}

					if (AttributeName)
					{
						CP_XML_NODE(L"p:attrNameLst")
						{
							CP_XML_NODE(L"p:attrName")
							{
								CP_XML_STREAM() << AttributeName.value();
							}
						}
					}
				}
				CP_XML_NODE(L"p:tavLst")
				{
					if (KeypointArray)
					{
						for (size_t i = 0; i < KeypointArray->size(); i++)
						{
							const int& time = (*KeypointArray)[i].Time;
							const std::wstring& value = (*KeypointArray)[i].Value;
							const _CP_OPT(std::wstring) formula = (*KeypointArray)[i].Fmla;

							CP_XML_NODE(L"p:tav")
							{
								if (formula) 
									CP_XML_ATTR(L"fmla", formula.value());
								CP_XML_ATTR(L"tm", time);
								
								CP_XML_NODE(L"p:val")
								{
									CP_XML_NODE(L"p:strVal")
									{
										CP_XML_ATTR(L"val", value);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void pptx_animation_context::Impl::_anim_clr::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:animClr")
			{
				CP_XML_ATTR_OPT(L"clrSpc", ColorSpace);

				CP_XML_NODE(L"p:cBhvr")
				{
					CP_XML_NODE(L"p:cTn")
					{
						CP_XML_ATTR_OPT(L"dur", serialize_duration(Duration));
						CP_XML_ATTR_OPT(L"fill", Fill);
						CP_XML_ATTR_OPT(L"autoRev", AutoRev);
						CP_XML_ATTR_OPT(L"dir", Direction);

						if (Delay)
						{
							CP_XML_NODE(L"p:stCondLst")
							{
								CP_XML_NODE(L"p:cond")
								{
									CP_XML_ATTR(L"delay", Delay.value());
								}
							}
						}
					}
					CP_XML_NODE(L"p:tgtEl")
					{
						size_t shapeID = ShapeID ? ShapeID.value() : 0;
						CP_XML_NODE(L"p:spTgt")
						{
							CP_XML_ATTR(L"spid", shapeID);
						}
					}
					if (AttributeName)
					{
						CP_XML_NODE(L"p:attrNameLst")
						{
							CP_XML_NODE(L"p:attrName")
							{
								CP_XML_STREAM() << AttributeName.value();
							}
						}
					}
				}
				if (ToValue)
				{
					CP_XML_NODE(L"p:to")
					{
						CP_XML_NODE(L"a:srgbClr")
						{
							CP_XML_ATTR(L"val", ToValue.value());
						}
					}
				}

				if (ByValue)
				{
					CP_XML_NODE(L"p:by")
					{
						switch (ByValue->type_)
						{
						case color::rgb:
						{
							CP_XML_NODE(L"a:srgbClr")
							{
								std::wstringstream ss;
								ss << std::hex
									<< ByValue->v1
									<< ByValue->v2
									<< ByValue->v3;

								CP_XML_ATTR(L"val", ss.str());
							}
						} break;
						case color::hsl:
						{
							CP_XML_NODE(L"p:hsl")
							{
								CP_XML_ATTR(L"h", ByValue->v1);
								CP_XML_ATTR(L"s", ByValue->v2);
								CP_XML_ATTR(L"l", ByValue->v3);
							}
						} break;
						}
					}
				}
			}
		}
	}

	void pptx_animation_context::Impl::_anim_scale::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:animScale")
			{
				CP_XML_NODE(L"p:cBhvr")
				{
					CP_XML_NODE(L"p:cTn")
					{
						CP_XML_ATTR_OPT(L"dur", serialize_duration(Duration));

						if (AutoReverse)	CP_XML_ATTR(L"autoRev", AutoReverse.value());
						if (Fill)			CP_XML_ATTR(L"fill", Fill.value());

						CP_XML_NODE(L"p:stCondLst")
						{
							std::wstring delay = Delay ? Delay.value() : L"0";
							CP_XML_NODE(L"p:cond")
							{
								CP_XML_ATTR(L"delay", delay);
							}
						}
					}
					CP_XML_NODE(L"p:tgtEl")
					{
						CP_XML_NODE(L"p:spTgt")
						{
							size_t shapeID = ShapeID ? ShapeID.value() : 0;
							CP_XML_ATTR(L"spid", shapeID);
						}
					}

					if (AttributeName)
					{
						CP_XML_NODE(L"p:attrNameLst")
						{
							CP_XML_NODE(L"p:attrName")
							{
								CP_XML_STREAM() << AttributeName.value();
							}
						}
					}
				}

				if (From)
				{
					CP_XML_NODE(L"p:from")
					{
						CP_XML_ATTR(L"x", From->x);
						CP_XML_ATTR(L"y", From->y);
					}
				}

				if (To)
				{
					CP_XML_NODE(L"p:to")
					{
						CP_XML_ATTR(L"x", To->x);
						CP_XML_ATTR(L"y", To->y);
					}
				}

				if (By)
				{
					CP_XML_NODE(L"p:by")
					{
						CP_XML_ATTR(L"x", By->x);
						CP_XML_ATTR(L"y", By->y);
					}
				}
			}
		}
	}

	void pptx_animation_context::Impl::_anim_rotate::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:animRot")
			{
				if (By)		CP_XML_ATTR(L"by", By.value());

				CP_XML_NODE(L"p:cBhvr")
				{
					CP_XML_NODE(L"p:cTn")
					{
						CP_XML_ATTR_OPT(L"dur", serialize_duration(Duration));

						if (AutoReverse)	CP_XML_ATTR(L"autoRev", AutoReverse.value());
						if (Fill)			CP_XML_ATTR(L"fill", Fill.value());

						CP_XML_NODE(L"p:stCondLst")
						{
							std::wstring delay = Delay ? Delay.value() : L"0";
							CP_XML_NODE(L"p:cond")
							{
								CP_XML_ATTR(L"delay", delay);
							}
						}
					}
					CP_XML_NODE(L"p:tgtEl")
					{
						CP_XML_NODE(L"p:spTgt")
						{
							size_t shapeID = ShapeID ? ShapeID.value() : 0;
							CP_XML_ATTR(L"spid", shapeID);
						}
					}

					if (AttributeName)
					{
						CP_XML_NODE(L"p:attrNameLst")
						{
							CP_XML_NODE(L"p:attrName")
							{
								CP_XML_STREAM() << AttributeName.value();
							}
						}
					}
				}
			}
		}
	}

	void pptx_animation_context::Impl::_audio::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:audio")
			{
				CP_XML_ATTR(L"isNarration", 0);

				CP_XML_NODE(L"p:cMediaNode")
				{
					CP_XML_ATTR(L"showWhenStopped", 1);

					CP_XML_NODE(L"p:cTn");
					CP_XML_NODE(L"p:tgtEl")
					{
						CP_XML_NODE(L"p:sndTgt")
						{
							CP_XML_ATTR_OPT(L"name", Name);
							CP_XML_ATTR_OPT(L"r:embed", RId);
						}
					}
				}
			}
		}
	}

} // namespace oox
} // namespace cpdoccore