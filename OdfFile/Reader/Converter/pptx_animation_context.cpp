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
#include <CPOptional.h>

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
			_CP_OPT(std::wstring)						PresetID;

			_par_animation_array						AnimParArray;
			_seq_animation_ptr							AnimSeq;
			_animation_element_array					AnimationActionArray;

			void serialize(std::wostream & strm) override;
		};

		struct _seq_animation : _animation_element
		{
			_CP_OPT(std::wstring)							PresentationNodeType;
			_CP_OPT(std::wstring)							SmilDirection;
			_CP_OPT(std::wstring)							SmilRestart;
			_CP_OPT(int)									SmilDurMs;
			_CP_OPT(std::wstring)							SmilBegin;
			_CP_OPT(std::wstring)							SmilEnd;

			_par_animation_array							AnimParArray;

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
			_CP_OPT(std::wstring)							SmilTargetElement;
			_CP_OPT(std::wstring)							SvgPath;

			void serialize(std::wostream & strm) override;
		};

		_par_animation_ptr							root_animation_element_;
		_par_animation_array						par_animation_levels_;

		_animate_motion_ptr							animate_motion_description_;

		void clear()
		{
			par_animation_levels_.clear();
			root_animation_element_ = nullptr;
		}

		Impl()
		{
			clear();
			/*root_animation_element_ = boost::make_shared<_par_animation>();
			root_animation_element_->PresentationNodeType = L"timing-root";
			_seq_animation_ptr main_sequence = boost::make_shared<_seq_animation>();
			main_sequence->PresentationNodeType = L"main-sequence";
			_par_animation_ptr par1 = boost::make_shared<_par_animation>();
			par1->SmilBegin = L"next";
			_par_animation_ptr par2 = boost::make_shared<_par_animation>();
			par2->SmilBegin = L"0s";
			_par_animation_ptr par3 = boost::make_shared<_par_animation>();
			par3->SmilBegin = L"0s";
			par3->PresentationNodeType = L"on-click";

			_animate_motion_ptr motion = boost::make_shared<_animate_motion>();
			motion->SmilDurMs = 2000;
			motion->SmilTargetElement = L"id1";
			motion->AnimSubItem = L"text";
			motion->SvgPath = L"M0.00178571428571429-0.00831746031746032h0.714285714285714";

			par3->AnimationActions.push_back(motion);
			par2->AnimPar = par3;
			par1->AnimPar = par2;
			main_sequence->AnimParArray = _animation_element_array();
			main_sequence->AnimParArray->push_back(par1);
			root_animation_element_->AnimSeq->push_back(main_sequence);*/

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

	void pptx_animation_context::set_seq_animation_smil_direction(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->SmilDirection = value;
			}
		}
	}

	void pptx_animation_context::set_seq_animation_smil_restart(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->SmilRestart = value;
			}
		}
	}

	void pptx_animation_context::set_seq_animation_smil_dur(int value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->SmilDurMs = value;
			}
		}
	}

	void pptx_animation_context::set_seq_animation_smil_begin(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->SmilBegin = value;
			}
		}
	}

	void pptx_animation_context::set_seq_animation_smil_end(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			if (back->AnimSeq)
			{
				back->AnimSeq->SmilEnd = value;
			}
		}
	}

	void pptx_animation_context::end_seq_animation()
	{
		
	}

	void pptx_animation_context::start_animate_motion()
	{
		impl_->animate_motion_description_ = boost::make_shared<Impl::_animate_motion>();
	}

	void pptx_animation_context::set_animate_motion_presentation_node_type(const std::wstring& value)
	{
		impl_->animate_motion_description_->PresentationNodeType = value;
	}

	void pptx_animation_context::set_animate_motion_smil_direction(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilDirection = value;
	}

	void pptx_animation_context::set_animate_motion_smil_restart(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilRestart = value;
	}

	void pptx_animation_context::set_animate_motion_smil_dur(int value)
	{
		impl_->animate_motion_description_->SmilDurMs = value;
	}

	void pptx_animation_context::set_animate_motion_smil_begin(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilBegin = value;
	}

	void pptx_animation_context::set_animate_motion_smil_end(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilEnd = value;
	}

	void pptx_animation_context::set_animate_motion_smil_fill(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilFill = value;
	}

	void pptx_animation_context::set_animate_motion_smil_target_element(const std::wstring& value)
	{
		impl_->animate_motion_description_->SmilTargetElement = value;
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

	void pptx_animation_context::set_par_animation_preset_id(const std::wstring& value)
	{
		if (impl_->par_animation_levels_.size())
		{
			Impl::_par_animation_ptr& back = impl_->par_animation_levels_.back();
			back->PresetID = value;
		}
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
							CP_XML_ATTR(L"spid", SmilTargetElement);
						}
					}
				}
			}
		}
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
						if (NodeType && NodeType.value() == L"tmRoot")
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
						{
							AnimSeq->serialize(CP_XML_STREAM());
						}
						
						for (size_t i = 0; i < AnimationActionArray.size(); i++)
						{
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
				CP_XML_NODE(L"p:cTn")
				{
					if (PresentationNodeType && PresentationNodeType.value() == L"main-sequence")
					{
						CP_XML_ATTR(L"dur", L"indefinite");
						CP_XML_ATTR(L"nodeType", L"mainSeq");
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
			}
		}
	}

} // namespace oox
} // namespace cpdoccore