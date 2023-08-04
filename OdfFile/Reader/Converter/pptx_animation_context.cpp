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

#include <vector>
#include <xml/simple_xml_writer.h>

#include "../../DataTypes/clockvalue.h"

namespace cpdoccore {
namespace oox {

	class pptx_animation_context::Impl
	{
	public:

		struct _animation_element;
		typedef shared_ptr<_animation_element>::Type	_animation_element_ptr;
		typedef std::vector<_animation_element_ptr>		_animation_element_array;
		struct _animation_element
		{
			virtual void serialize(std::wostream& strm) = 0;
		};
		
		struct _par_animation;
		typedef shared_ptr<_par_animation>::Type		_par_animation_ptr;
		typedef std::vector<_par_animation_ptr>			_par_animation_array;

		struct _seq_animation;
		typedef shared_ptr<_seq_animation>::Type		_seq_animation_ptr;
		typedef std::vector<_seq_animation_ptr>			_seq_animation_array;

		struct _par_animation : _animation_element
		{
			_CP_OPT(std::wstring)						NodeType;
			_CP_OPT(std::wstring)						Direction;
			_CP_OPT(std::wstring)						Restart;
			_CP_OPT(int)								Duration; // in ms
			_CP_OPT(std::wstring)						Delay;
			_CP_OPT(std::wstring)						End;
			_CP_OPT(std::wstring)						PresetClass;
			_CP_OPT(int)								PresetID;

			_par_animation_array						AnimParArray;
			_seq_animation_ptr							AnimSeq;
			_animation_element_array					AnimationActionArray;

			void serialize(std::wostream & strm) override;
		};

		struct _seq_animation : _animation_element
		{
			_CP_OPT(std::wstring)							PresentationNodeType;
			_CP_OPT(std::wstring)							Direction;
			_CP_OPT(std::wstring)							Restart;
			_CP_OPT(int)									Duration; // in ms
			_CP_OPT(std::wstring)							Delay;
			_CP_OPT(std::wstring)							End;

			_par_animation_array							AnimParArray;

			void serialize(std::wostream& strm) override;
		};

		struct _set;
		typedef shared_ptr<_set>::Type						_set_ptr;
		struct _set : _animation_element
		{
			_CP_OPT(std::wstring)							Direction;
			_CP_OPT(std::wstring)							Restart;
			_CP_OPT(int)									Duration; // in ms
			_CP_OPT(std::wstring)							Delay;
			_CP_OPT(std::wstring)							End;
			_CP_OPT(std::wstring)							Fill;
			_CP_OPT(size_t)									ShapeID;
			_CP_OPT(std::wstring)							AttributeName;
			_CP_OPT(std::wstring)							ToValue;

			void serialize(std::wostream& strm) override;
		};

		struct _anim_effect;
		typedef shared_ptr<_anim_effect>::Type				_anim_effect_ptr;
		struct _anim_effect : _animation_element
		{
			_CP_OPT(std::wstring)							Filter;
			_CP_OPT(std::wstring)							Transition;
			_CP_OPT(int)									Duration; // in ms
			_CP_OPT(size_t)									ShapeID;

			void serialize(std::wostream & strm) override;
		};

		struct _animate_motion;
		typedef shared_ptr<_animate_motion>::Type			_animate_motion_ptr;
		typedef std::vector<_animate_motion_ptr>			_animate_motion_array;
		struct _animate_motion : _animation_element 
		{
			_CP_OPT(std::wstring)							PresentationNodeType;
			_CP_OPT(std::wstring)							SmilDirection;
			_CP_OPT(std::wstring)							SmilRestart;
			_CP_OPT(int)									SmilDurMs;
			_CP_OPT(std::wstring)							SmilBegin;
			_CP_OPT(std::wstring)							SmilEnd;

			_CP_OPT(std::wstring)							SmilFill;
			_CP_OPT(std::wstring)							AnimSubItem;
			_CP_OPT(size_t)									ShapeID;
			_CP_OPT(std::wstring)							SvgPath;

			void serialize(std::wostream & strm) override;
		};

		struct _anim_clr;
		typedef shared_ptr<_anim_clr>::Type					_anim_clr_ptr;
		struct _anim_clr : _animation_element
		{
			_CP_OPT(std::wstring)							PresentationNodeType;
			_CP_OPT(std::wstring)							Direction;
			_CP_OPT(std::wstring)							Restart;
			_CP_OPT(int)									Duration; // in ms
			_CP_OPT(std::wstring)							Begin;
			_CP_OPT(std::wstring)							End;

			_CP_OPT(std::wstring)							Fill;
			_CP_OPT(size_t)									ShapeID;
			_CP_OPT(std::wstring)							AttributeName;
			_CP_OPT(std::wstring)							Delay;
			_CP_OPT(std::wstring)							ToValue;
			_CP_OPT(std::wstring)							ColorSpace;

			void serialize(std::wostream& strm) override;
		};

		struct _anim;
		typedef shared_ptr<_anim>::Type						_anim_ptr;
		struct _anim : _animation_element
		{
			struct _keypoint
			{
				int						Time;
				std::wstring			Value;
				_CP_OPT(std::wstring)	Fmla; // Formula

				_keypoint(int time, const std::wstring& value, _CP_OPT(std::wstring) formula)
					: Time(time), Value(value), Fmla(formula)
				{}
			};

			_CP_OPT(std::wstring)							CalcMode;
			_CP_OPT(std::wstring)							ValueType;
			_CP_OPT(size_t)									ShapeID;
			_CP_OPT(int)									Duration; // in ms
			_CP_OPT(std::wstring)							AttributeName;
			_CP_OPT(std::wstring)							From;
			_CP_OPT(std::wstring)							To;
			_CP_OPT(std::wstring)							By;
			_CP_OPT(std::wstring)							Additive;
			_CP_OPT(bool)									AutoReverse;
			_CP_OPT(std::wstring)							Delay;
			_CP_OPT(std::vector<_keypoint>)					KeypointArray;

			void serialize(std::wostream& strm) override;
		};

		struct _anim_scale;
		typedef shared_ptr<_anim_scale>::Type					_anim_scale_ptr;
		struct _anim_scale : _animation_element
		{
			struct vec2
			{
				int x, y;

				vec2(int x, int y)
					: x(x), y(y)
				{}
			};

			_CP_OPT(size_t)									ShapeID;
			_CP_OPT(int)									Duration; // in ms
			_CP_OPT(std::wstring)							Fill;
			_CP_OPT(vec2)									From;
			_CP_OPT(vec2)									To;
			//_CP_OPT(std::wstring)							By;
			_CP_OPT(std::wstring)							Delay;
			_CP_OPT(bool)									AutoReverse;

			void serialize(std::wostream& strm) override;
		};

		struct _anim_rotate;
		typedef shared_ptr<_anim_rotate>::Type					_anim_rotate_ptr;
		struct _anim_rotate : _animation_element
		{
			_CP_OPT(size_t)									ShapeID;
			_CP_OPT(int)									Duration; // in ms
			_CP_OPT(std::wstring)							Fill;
			_CP_OPT(int)									By;
			_CP_OPT(std::wstring)							Delay;
			_CP_OPT(bool)									AutoReverse;

			void serialize(std::wostream& strm) override;
		};

		_par_animation_ptr							root_animation_element_;
		_par_animation_array						par_animation_levels_;

		_animate_motion_ptr							animate_motion_description_;
		_set_ptr									set_description_;
		_anim_effect_ptr							anim_effect_description_;
		_anim_ptr									anim_description_;
		_anim_clr_ptr								anim_clr_description_;
		_anim_scale_ptr								anim_scale_description_;
		_anim_rotate_ptr							anim_rotate_description_;

		void clear()
		{
			par_animation_levels_.clear();
			root_animation_element_			= nullptr;
			animate_motion_description_		= nullptr;
			set_description_				= nullptr;
			anim_effect_description_		= nullptr;
			anim_description_				= nullptr;
			anim_clr_description_			= nullptr;
			anim_scale_description_			= nullptr;
			anim_rotate_description_ = nullptr;
		}

		Impl()
		{
			clear();
		}
	};

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
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->NodeType = value;
		}
	}

	void pptx_animation_context::set_par_animation_direction(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->Direction = value;
		}
	}

	void pptx_animation_context::set_par_animation_restart(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->Restart = value;
		}
	}

	void pptx_animation_context::set_par_animation_duration(int value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->Duration = value;
		}
	}

	void pptx_animation_context::set_par_animation_delay(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->Delay = value;
		}
	}

	void pptx_animation_context::set_par_animation_end(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->End = value;
		}
	}

	void pptx_animation_context::set_par_animation_preset_class(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->PresetClass = value;
		}
	}

	void pptx_animation_context::set_par_animation_preset_id(int value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->PresetID = value;
		}
	}

	void pptx_animation_context::end_par_animation()
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr end = impl_->par_animation_levels_.back();
			impl_->par_animation_levels_.pop_back();
			if (!impl_->par_animation_levels_.size())
				return;

			Impl::_par_animation_ptr back = impl_->par_animation_levels_.back();

			if (back->AnimSeq)
				back->AnimSeq->AnimParArray.push_back(end);
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
			impl_->par_animation_levels_.back()->AnimSeq = boost::make_shared<Impl::_seq_animation>();
		}
	}

	void pptx_animation_context::set_seq_animation_presentation_node_type(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->PresentationNodeType = value;
			}
		}
	}

	void pptx_animation_context::set_seq_animation_direction(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->Direction = value;
			}
		}
	}

	void pptx_animation_context::set_seq_animation_restart(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->Restart = value;
			}
		}
	}

	void pptx_animation_context::set_seq_animation_dur(int value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->Duration = value;
			}
		}
	}

	void pptx_animation_context::set_seq_animation_delay(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->Delay = value;
			}
		}
	}

	void pptx_animation_context::set_seq_animation_end(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->End = value;
			}
		}
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
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->AnimationActionArray.push_back(impl_->set_description_);
		}
		impl_->set_description_ = nullptr;
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

	void pptx_animation_context::set_anim_effect_shape_id(size_t value)
	{
		impl_->anim_effect_description_->ShapeID = value;
	}

	void pptx_animation_context::end_anim_effect()
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->AnimationActionArray.push_back(impl_->anim_effect_description_);
		}
		impl_->anim_effect_description_ = nullptr;
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
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->AnimationActionArray.push_back(impl_->animate_motion_description_);
		}
		impl_->animate_motion_description_ = nullptr;
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
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->AnimationActionArray.push_back(impl_->anim_description_);
		}
		impl_->anim_description_ = nullptr;
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

	void pptx_animation_context::set_animate_color_duration(int value)
	{
		impl_->anim_clr_description_->Duration = value;
	}

	void pptx_animation_context::set_animate_color_delay(const std::wstring& value)
	{
		impl_->anim_clr_description_->Delay = value;
	}

	void pptx_animation_context::set_animate_color_attribute_name(const std::wstring& value)
	{
		impl_->anim_clr_description_->AttributeName = value;
	}

	void pptx_animation_context::set_animate_color_to_value(const std::wstring& value)
	{
		impl_->anim_clr_description_->ToValue = value;
	}

	void pptx_animation_context::set_animate_color_shape_id(size_t value)
	{
		impl_->anim_clr_description_->ShapeID = value;
	}

	void pptx_animation_context::end_animate_color()
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->AnimationActionArray.push_back(impl_->anim_clr_description_);
		}
		impl_->anim_clr_description_ = nullptr;
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

	void pptx_animation_context::set_animate_scale_delay(const std::wstring& value)
	{
		impl_->anim_scale_description_->Delay = value;
	}

	void pptx_animation_context::set_animate_scale_auto_reverse(bool value)
	{
		impl_->anim_scale_description_->AutoReverse = value;
	}

	void pptx_animation_context::end_animate_scale()
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->AnimationActionArray.push_back(impl_->anim_scale_description_);
		}
		impl_->anim_scale_description_ = nullptr;
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
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->AnimationActionArray.push_back(impl_->anim_rotate_description_);
		}
		impl_->anim_rotate_description_ = nullptr;
	}

	void pptx_animation_context::serialize(std::wostream& strm)
	{
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

	void pptx_animation_context::Impl::_par_animation::serialize(std::wostream& strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:par")
			{
				CP_XML_NODE(L"p:cTn")
				{
					if (PresetClass)
						CP_XML_ATTR(L"presetClass", PresetClass.value());
					if (PresetID)
						CP_XML_ATTR(L"presetID", PresetID.value());

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
							CP_XML_ATTR(L"fill", L"hold");
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
					
					CP_XML_NODE(L"p:childTnLst")
					{
						for(size_t i = 0; i < AnimParArray.size(); i++)
							AnimParArray[i]->serialize(CP_XML_STREAM());
						
						if (AnimSeq)
							AnimSeq->serialize(CP_XML_STREAM());
						
						for (size_t i = 0; i < AnimationActionArray.size(); i++)
							AnimationActionArray[i]->serialize(CP_XML_STREAM());
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
				CP_XML_NODE(L"p:cTn")
				{
					if (PresentationNodeType)
					{
						CP_XML_ATTR(L"nodeType", PresentationNodeType.value());
						CP_XML_ATTR(L"dur", L"indefinite");
					}
					else if (Duration)		
						CP_XML_ATTR(L"dur",	Duration.value());

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
						if (Duration)	CP_XML_ATTR(L"dur", Duration.value());
						if (Fill)		CP_XML_ATTR(L"fill", Fill.value());
						
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

					CP_XML_NODE(L"p:attrNameLst")
					{
						if (AttributeName)
						{
							CP_XML_NODE(L"p:attrName")
							{
								CP_XML_STREAM() << AttributeName.value();
							}
						}
					}
				}
				CP_XML_NODE(L"p:to")
				{
					if (ToValue)
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
				if (Filter)			CP_XML_ATTR(L"filter", Filter.value());
				if (Transition)		CP_XML_ATTR(L"transition", Transition.value());

				CP_XML_NODE(L"p:cBhvr")
				{
					if (Duration)
					{
						CP_XML_NODE(L"p:cTn")
						{
							CP_XML_ATTR(L"dur", Duration.value());
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
						if (SmilDurMs)
							CP_XML_ATTR(L"dur", SmilDurMs);
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
				if (CalcMode)		CP_XML_ATTR(L"calcmode", CalcMode.value());
				if (ValueType)		CP_XML_ATTR(L"valueType", ValueType.value());
				if(From)			CP_XML_ATTR(L"from", From.value());
				if (To)				CP_XML_ATTR(L"to", To.value());
				if (By)				CP_XML_ATTR(L"by", By.value());

				CP_XML_NODE(L"p:cBhvr")
				{
					if (Additive)	CP_XML_ATTR(L"additive", Additive.value());
					
					CP_XML_NODE(L"p:cTn")
					{
						int duration = Duration ? Duration.value() : 1;
						CP_XML_ATTR(L"dur", duration);

						if (AutoReverse)
						{
							int autoRev = AutoReverse.value() ? 1 : 0;
							CP_XML_ATTR(L"autoRev", autoRev);
						}

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
				if (ColorSpace)		CP_XML_ATTR(L"clrSpc", ColorSpace.value());

				CP_XML_NODE(L"p:cBhvr")
				{
					CP_XML_NODE(L"p:cTn")
					{
						int duration = Duration ? Duration.value() : 1;
						CP_XML_ATTR(L"dur", duration);
						CP_XML_ATTR(L"fill", L"hold");

						CP_XML_NODE(L"p:stCondLst")
						{
							if (Delay)
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
					CP_XML_NODE(L"p:attrNameLst")
					{
						if (AttributeName)
						{
							CP_XML_NODE(L"p:attrName")
							{
								CP_XML_STREAM() << AttributeName.value();
							}
						}
					}
				}
				CP_XML_NODE(L"p:to")
				{
					if (ToValue)
					{
						CP_XML_NODE(L"a:srgbClr")
						{
							CP_XML_ATTR(L"val", ToValue.value());
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
						int duration = Duration ? Duration.value() : 1;
						CP_XML_ATTR(L"dur", duration);

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
				}
				CP_XML_NODE(L"p:from")
				{
					if (From)
					{
						CP_XML_ATTR(L"x", From->x);
						CP_XML_ATTR(L"y", From->y);
					}
				}
				CP_XML_NODE(L"p:to")
				{
					if (To)
					{
						CP_XML_ATTR(L"x", To->x);
						CP_XML_ATTR(L"y", To->y);
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
						int duration = Duration ? Duration.value() : 1;
						CP_XML_ATTR(L"dur", duration);

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
				}
			}
		}
	}

} // namespace oox
} // namespace cpdoccore