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
#include <CPSharedPtr.h>
#include <CPOptional.h>

namespace cpdoccore {
namespace oox {

	class pptx_animation_context
	{
	public:
		class Impl
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
				_CP_OPT(int)								PresetSubtype;
				_CP_OPT(std::wstring)						Fill;
				_CP_OPT(int)								Accelerate;
				_CP_OPT(int)								Decelerate;

				_par_animation_array						AnimParArray;
				_seq_animation_array						AnimSeq;
				_animation_element_array					AnimationActionArray;

				void serialize(std::wostream& strm) override;
			};

			struct _seq_animation : _animation_element
			{
				_CP_OPT(std::wstring)							PresentationNodeType;
				_CP_OPT(std::wstring)							Direction;
				_CP_OPT(std::wstring)							Restart;
				_CP_OPT(int)									Duration; // in ms
				_CP_OPT(std::wstring)							Delay;
				_CP_OPT(std::wstring)							End;
				_CP_OPT(std::wstring)							TargetEl;

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
				_CP_OPT(std::wstring)							AutoRev;
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
				_CP_OPT(std::wstring)							Delay;
				_CP_OPT(int)									Accel;
				_CP_OPT(int)									Decel;
				_CP_OPT(int)									Duration; // in ms
				_CP_OPT(size_t)									ShapeID;

				void serialize(std::wostream& strm) override;
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

				void serialize(std::wostream& strm) override;
			};

			struct _anim_clr;
			typedef shared_ptr<_anim_clr>::Type					_anim_clr_ptr;
			struct _anim_clr : _animation_element
			{
				struct color
				{
					enum type
					{
						rgb,
						hsl
					} type_;

					int v1, v2, v3;
				};


				_CP_OPT(std::wstring)							PresentationNodeType;
				_CP_OPT(std::wstring)							Direction;
				_CP_OPT(std::wstring)							Restart;
				_CP_OPT(int)									Duration; // in ms
				_CP_OPT(std::wstring)							Begin;
				_CP_OPT(std::wstring)							End;

				_CP_OPT(std::wstring)							Fill;
				_CP_OPT(bool)									AutoRev;
				_CP_OPT(size_t)									ShapeID;
				_CP_OPT(std::wstring)							AttributeName;
				_CP_OPT(std::wstring)							Delay;
				_CP_OPT(std::wstring)							ToValue;
				_CP_OPT(color)									ByValue;
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
				_CP_OPT(vec2)									By;
				_CP_OPT(std::wstring)							Delay;
				_CP_OPT(std::wstring)							AttributeName;
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
				_CP_OPT(std::wstring)							AttributeName;
				_CP_OPT(std::wstring)							Delay;
				_CP_OPT(bool)									AutoReverse;

				void serialize(std::wostream& strm) override;
			};

			struct _audio;
			typedef shared_ptr<_audio>::Type					_audio_ptr;
			struct _audio : _animation_element
			{
				_CP_OPT(std::wstring)							Name;
				_CP_OPT(std::wstring)							RId;

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
			_audio_ptr									audio_description_;

			void clear();

			bool IsSlideAnimation;

			Impl();
		};

	public:
		pptx_animation_context();

		void start_par_animation();
			void set_par_animation_presentation_node_type(const std::wstring& value);
			void set_par_animation_direction(const std::wstring& value);
			void set_par_animation_restart(const std::wstring& value);
			void set_par_animation_duration(int value);
			void set_par_animation_delay(const std::wstring& value);
			void set_par_animation_end(const std::wstring& value);
			void set_par_animation_preset_class(const std::wstring& value);
			void set_par_animation_preset_id(int value);
			void set_par_animation_preset_subtype(int value);
			void set_par_animation_fill(const std::wstring& value);
			void set_par_animation_accelerate(int value);
			void set_par_animation_decelerate(int value);
		void end_par_animation();

		void start_seq_animation();
			void set_seq_animation_presentation_node_type(const std::wstring& value);
			void set_seq_animation_direction(const std::wstring& value);
			void set_seq_animation_restart(const std::wstring& value);
			void set_seq_animation_dur(int value);
			void set_seq_animation_delay(const std::wstring& value);
			void set_seq_animation_end(const std::wstring& value);
			void set_seq_animation_target_element(const std::wstring& value);
		void end_seq_animation();

		void start_set();
			void set_set_direction(const std::wstring& value);
			void set_set_restart(const std::wstring& value);
			void set_set_duration(int value);
			void set_set_delay(const std::wstring& value);
			void set_set_end(const std::wstring& value);
			void set_set_auto_rev(const std::wstring& value);
			void set_set_fill(const std::wstring& value);
			void set_set_shape_id(size_t value);
			void set_set_attribute_name(const std::wstring& value);
			void set_set_to_value(const std::wstring& value);
		void end_set();

		void start_anim_effect();
			void set_anim_effect_filter(const std::wstring& value);
			void set_anim_effect_transition(const std::wstring& value);
			void set_anim_effect_duration(int value);
			void set_anim_effect_delay(const std::wstring& value);
			void set_anim_effect_accel(int value);
			void set_anim_effect_decel(int value);
			void set_anim_effect_shape_id(size_t value);
		void end_anim_effect();

		void start_animate_motion();
			void set_animate_motion_presentation_node_type(const std::wstring& value);
			void set_animate_motion_direction(const std::wstring& value);
			void set_animate_motion_restart(const std::wstring& value);
			void set_animate_motion_dur(int value);
			void set_animate_motion_delay(const std::wstring& value);
			void set_animate_motion_end(const std::wstring& value);
			void set_animate_motion_fill(const std::wstring& value);
			void set_animate_motion_shape_id(size_t value);
			void set_animate_motion_svg_path(const std::wstring& value);
		void end_animate_motion();

		void start_animate();
			void set_animate_calc_mode(const std::wstring& value);
			void set_animate_value_type(const std::wstring& value);
			void set_animate_shape_id(size_t value);
			void set_animate_duration(int value);
			void set_animate_attribute_name(const std::wstring& value);
			void set_animate_from(const std::wstring& value);
			void set_animate_to(const std::wstring& value);
			void set_animate_by(const std::wstring& value);
			void set_animate_additive(const std::wstring& value);
			void set_animate_auto_reverse(bool value);
			void set_animate_delay(const std::wstring& value);
			void add_animate_keypoint(int time, const std::wstring& value, _CP_OPT(std::wstring) formula);
		void end_animate();
		
		void start_animate_color();
			void set_animate_color_color_space(const std::wstring& value);
			void set_animate_color_dir(const std::wstring& value);
			void set_animate_color_duration(int value);
			void set_animate_color_delay(const std::wstring& value);
			void set_animate_color_fill(const std::wstring& value);
			void set_animate_color_auto_rev(bool value);
			void set_animate_color_attribute_name(const std::wstring& value);
			void set_animate_color_to_value(const std::wstring& value);
			void set_animate_color_by_value(const std::wstring& value);
			void set_animate_color_shape_id(size_t value);
		void end_animate_color();

		void start_animate_scale();
			void set_animate_scale_shape_id(size_t value);
			void set_animate_scale_duration(int value);
			void set_animate_scale_fill(const std::wstring& value);
			void set_animate_scale_from(int x, int y);
			void set_animate_scale_to(int x, int y);
			void set_animate_scale_by(int x, int y);
			void set_animate_scale_delay(const std::wstring& value);
			void set_animate_scale_attribute_name(const std::wstring& value);
			void set_animate_scale_auto_reverse(bool value);
		void end_animate_scale();

		void start_animate_rotate();
			void set_animate_rotate_shape_id(size_t value);
			void set_animate_rotate_duration(int value);
			void set_animate_rotate_fill(const std::wstring& value);
			void set_animate_rotate_by(int value);
			void set_animate_rotate_attribute_name(const std::wstring& value);
			void set_animate_rotate_delay(const std::wstring& value);
			void set_animate_rotate_auto_reverse(bool value);
		void end_animate_rotate();

		void start_anim_audio();
			void add_anim_audio(const std::wstring& rId, const std::wstring& name);
		void end_anim_audio();

		
		
		void serialize(std::wostream & strm);
		void clear();

		void set_is_slide_animation(bool is_slide_animation);
		bool get_is_slide_animation();
		const Impl::_par_animation_ptr& get_root_par_animation() const;

	private:
		_CP_PTR(Impl) impl_;
	};

} // namespace oox
} // namespace cpdoccore