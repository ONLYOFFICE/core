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

#include "anim_elements.h"

#include <boost/make_shared.hpp>
#include <xml/xmlchar.h>
#include <xml/attributes.h>
#include <xml/simple_xml_writer.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

///////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * anim_par::ns = L"anim";
const wchar_t * anim_par::name = L"par";


void anim_par::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"anim", L"par") 
		CP_CREATE_ELEMENT(anim_par_);
	else if	CP_CHECK_NAME(L"anim", L"seq") 
		CP_CREATE_ELEMENT(anim_seq_array_);//более 1 элемента- взаимосвязанная анимация (между фигурами)
	else
		CP_CREATE_ELEMENT(content_);
}
void anim_par::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

	if (type_ == typeAnimPar)
		anim_par_ = child_element;
	else if (type_ == typeAnimSeq)
		anim_seq_array_.push_back(child_element);
	else
		content_.push_back(child_element);
}
void anim_par::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			attlist_.serialize(CP_GET_XML_NODE());

			if (anim_par_)
				anim_par_->serialize(CP_XML_STREAM());
			
			for (size_t i = 0; i < anim_seq_array_.size(); i++)
			{
				anim_seq_array_[i]->serialize(CP_XML_STREAM());
			}
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * anim_seq::ns = L"anim";
const wchar_t * anim_seq::name = L"seq";

void anim_seq::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			attlist_.serialize(CP_GET_XML_NODE());
			
			for (size_t i = 0; i < anim_par_array_.size(); i++)
			{
				anim_par_array_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

void anim_seq::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	if	CP_CHECK_NAME(L"anim", L"par") 
		CP_CREATE_ELEMENT(anim_par_array_);
}
void anim_seq::add_child_element( const office_element_ptr & child)
{
	anim_par_array_.push_back(child);
}
////////////////////////////////////////////////////////////////
void anim_transition_filter_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"smil:subtype",	smil_subtype_);
    CP_XML_ATTR_OPT(L"smil:type",		smil_type_);
	CP_XML_ATTR_OPT(L"smil:fadeColor",	smil_fadeColor_);
	CP_XML_ATTR_OPT(L"smil:mode",		smil_mode_);

}
void anim_audio_attlist::serialize(CP_ATTR_NODE)
{
	CP_XML_ATTR_OPT_ENCODE_STRING(L"xlink:href",		xlink_href_);
	CP_XML_ATTR_OPT(L"anim:audio-level",anim_audio_level_);
}
const wchar_t * anim_transitionFilter::ns	= L"anim";
const wchar_t * anim_transitionFilter::name = L"transitionFilter";

void anim_transitionFilter::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			common_attlist_.serialize(CP_GET_XML_NODE());
			filter_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
//------------------------------------------------------------------------------------------------
const wchar_t * anim_audio::ns = L"anim";
const wchar_t * anim_audio::name = L"audio";

void anim_audio::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			common_attlist_.serialize(CP_GET_XML_NODE());
			audio_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

}
}
