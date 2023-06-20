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

namespace cpdoccore {
namespace oox {

	class pptx_animation_context::Impl
	{
	public:
		struct _par_animation;
		struct _seq_animation;
		struct _animate_action;

		struct _par_animation
		{
			bool IsRoot;
			_CP_OPT(std::wstring) Begin;
			_CP_OPT(std::wstring) Fill;
			_CP_OPT(std::wstring) Accelerate;
			_CP_OPT(std::wstring) Decelerate;
			_CP_OPT(std::wstring) NodeType;
			_CP_OPT(std::wstring) PresetClass;
			_CP_OPT(std::wstring) PresedID;

			_CP_OPT(std::vector<_seq_animation>) Sequence;
			_CP_OPT(std::vector<_seq_animation>) Actions;
		};

		struct _seq_animation
		{
			bool IsMainSequence;

			_CP_OPT(std::vector<_par_animation>) Parallels;
		};

		struct _animation_action
		{
			enum type
			{
				typeNone = 0,
				typeAnimateMotion,
				typeAnimateColor
			};

			type Type;
			_CP_OPT(float) Duration;
			_CP_OPT(std::wstring) TargetId;
			_CP_OPT(std::wstring) Path;

		};

		Impl()
		{
			
		}

	private:
		_par_animation rootAnimation_;
	};

	pptx_animation_context::pptx_animation_context()
	{

	}

	void pptx_animation_context::serialize(std::wostream & strm)
	{
		CP_XML_WRITER(strm)
		{
			CP_XML_NODE(L"p:timing")
			{
				CP_XML_NODE(L"p:tnLst")
				{
					CP_XML_NODE(L"p:par")
					{
						CP_XML_NODE(L"p:cTn")
						{
							CP_XML_ATTR(L"nodeType", L"tmRoot");
							CP_XML_ATTR(L"id", 1);
							CP_XML_ATTR(L"dur", L"indefinite");
							CP_XML_ATTR(L"restart", L"never");
						}
					}
				}
			}
		}
	}

} // namespace oox
} // namespace cpdoccore