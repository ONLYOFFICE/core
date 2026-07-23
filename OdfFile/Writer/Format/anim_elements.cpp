/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
		CP_CREATE_ELEMENT(anim_seq_array_);// more than 1 element - interconnected animation (between shapes)
	else
		CP_CREATE_ELEMENT(content_);
}
void anim_par::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

	if (type_ == typeAnimPar)
		anim_par_.push_back(child_element);
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
			par_attlist_.serialize(CP_GET_XML_NODE());

			for (size_t i = 0; i < anim_par_.size(); i++)
			{
				anim_par_[i]->serialize(CP_XML_STREAM());
			}

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
//------------------------------------------------------------------------------------------------
const wchar_t* anim_set::ns = L"anim";
const wchar_t* anim_set::name = L"set";

void anim_set::serialize(std::wostream& _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			common_attlist_.serialize(CP_GET_XML_NODE());
			set_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
//------------------------------------------------------------------------------------------------
const wchar_t* anim_animate::ns = L"anim";
const wchar_t* anim_animate::name = L"animate";

void anim_animate::serialize(std::wostream& _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			common_attlist_.serialize(CP_GET_XML_NODE());
			animate_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
//------------------------------------------------------------------------------------------------
const wchar_t* anim_animate_motion::ns = L"anim";
const wchar_t* anim_animate_motion::name = L"animateMotion";

void anim_animate_motion::serialize(std::wostream& _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			common_attlist_.serialize(CP_GET_XML_NODE());
			animate_motion_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

//------------------------------------------------------------------------------------------------
const wchar_t* anim_animate_color::ns = L"anim";
const wchar_t* anim_animate_color::name = L"animateColor";

void anim_animate_color::serialize(std::wostream& _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			common_attlist_.serialize(CP_GET_XML_NODE());
			color_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

//------------------------------------------------------------------------------------------------
const wchar_t* anim_animate_transform::ns = L"anim";
const wchar_t* anim_animate_transform::name = L"animateTransform";

void anim_animate_transform::serialize(std::wostream& _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE()
		{
			common_attlist_.serialize(CP_GET_XML_NODE());
			animate_transform_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}

}
}
