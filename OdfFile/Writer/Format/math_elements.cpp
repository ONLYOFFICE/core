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
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{			
			CP_XML_ATTR(L"xmlns", L"http://www.w3.org/1998/Math/MathML");
			
			semantics_->serialize(CP_XML_STREAM());
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
