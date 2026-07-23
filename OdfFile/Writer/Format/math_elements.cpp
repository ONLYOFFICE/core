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

#include "math_elements.h"

#include <xml/xmlchar.h>

#include <xml/attributes.h>
#include <xml/utils.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

//---------------------------------------------------------------
const wchar_t * office_math_element::ns = L"math";
const wchar_t * office_math_element::name = L"math-element";
//---------------------------------------------------------------
const wchar_t * office_math::ns = L"math";
const wchar_t * office_math::name = L"math";

//----------------------------------------------------------------------------------------------------

void office_math::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(semantics_);	
}

void office_math::add_child_element(const office_element_ptr & child_element)
{
	if (!child_element) return;

	if (typeMathSemantics == child_element->get_type())
	{
		semantics_ = child_element;
	}
	else if (semantics_)
		semantics_->add_child_element(child_element);
}

void office_math::serialize(std::wostream & _Wostream)
{
	if (false == content_.empty())
	{
		_Wostream << content_;
	}
	else
	{
		CP_XML_WRITER(_Wostream)
		{
			CP_XML_NODE_SIMPLE_NONS()
			{
				CP_XML_ATTR(L"xmlns", L"http://www.w3.org/1998/Math/MathML");

				if (semantics_)
				{
					semantics_->serialize(CP_XML_STREAM());
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_semantics::ns = L"math";
const wchar_t * math_semantics::name = L"semantics";
//----------------------------------------------------------------------------------------------------

void math_semantics::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_semantics::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_semantics::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}
			if (annotation_)
				annotation_->serialize(CP_XML_STREAM());		
		}
	}
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_annotation::ns = L"math";
const wchar_t * math_annotation::name = L"annotation";
//----------------------------------------------------------------------------------------------------

void math_annotation::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_annotation::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_annotation::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			CP_XML_ATTR_OPT(L"encoding", encoding_);
			if (text_)
			{
				CP_XML_CONTENT(text_.get());
				common_attlist_.serialize(CP_GET_XML_NODE());
			}
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

void math_annotation::add_text(const std::wstring& Text)
{
	text_ = Text;
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_annotation_xml::ns = L"math";
const wchar_t * math_annotation_xml::name = L"annotation-xml";
//----------------------------------------------------------------------------------------------------


void math_annotation_xml::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_annotation_xml::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_annotation_xml::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{		
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}
			CP_XML_ATTR_OPT(L"math:text", text_);
			CP_XML_ATTR_OPT(L"math:encoding", encoding_);
		}
	}
}
//----------------------------------------------------------------------------------------------------

}
}
