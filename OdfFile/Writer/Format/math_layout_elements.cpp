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

#include "math_layout_elements.h"
#include "math_token_elements.h"
#include "style_text_properties.h"
#include "math_limit_elements.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

//---------------------------------------------------------------
const wchar_t * math_mrow::ns = L"math";
const wchar_t * math_mrow::name = L"mrow";
//----------------------------------------------------------------------------------------------------
math_mrow::math_mrow()
{
	next_element_to_prev_ = false;
}

void math_mrow::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mrow::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mrow::serialize(std::wostream & _Wostream)
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
			//CP_XML_ATTR_OPT(L"math:bool", next_element_to_prev_); pich_qa?
		}
	}
}

//---------------------------------------------------------------
const wchar_t * math_mfrac::ns = L"math";
const wchar_t * math_mfrac::name = L"mfrac";
//----------------------------------------------------------------------------------------------------


void math_mfrac::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mfrac::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mfrac::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			CP_XML_ATTR_OPT(L"math:linethickness", linethickness);
			CP_XML_ATTR_OPT(L"bevelled", bevelled);
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}


//---------------------------------------------------------------
const wchar_t * math_msqrt::ns = L"math";
const wchar_t * math_msqrt::name = L"msqrt";
//----------------------------------------------------------------------------------------------------

void math_msqrt::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msqrt::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_msqrt::serialize(std::wostream & _Wostream)
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
		}
	}
}

//---------------------------------------------------------------
const wchar_t * math_mroot::ns = L"math";
const wchar_t * math_mroot::name = L"mroot";
//----------------------------------------------------------------------------------------------------

void math_mroot::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mroot::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mroot::serialize(std::wostream & _Wostream)
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
		}
	}
}

//---------------------------------------------------------------
const wchar_t * math_mstyle::ns = L"math";
const wchar_t * math_mstyle::name = L"mstyle";
//----------------------------------------------------------------------------------------------------

void math_mstyle::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mstyle::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mstyle::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			CP_XML_ATTR_OPT(L"math:mathsize", mathsize_);
			CP_XML_ATTR_OPT(L"mathcolor", color_);
			CP_XML_ATTR_OPT(L"math:fontweight", fontweight_);
			common_attlist_.serialize(CP_GET_XML_NODE());
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}
		}				
	}
}

//--------------------------------------------------
	
//---------------------------------------------------------------
const wchar_t * math_menclose::ns = L"math";
const wchar_t * math_menclose::name = L"menclose";
//----------------------------------------------------------------------------------------------------

void math_menclose::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_menclose::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_menclose::serialize(std::wostream & _Wostream)
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
		}		
	}
}

//---------------------------------------------------------------
const wchar_t * math_mfenced::ns = L"math";
const wchar_t * math_mfenced::name = L"mfenced";
//----------------------------------------------------------------------------------------------------

void math_mfenced::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mfenced::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mfenced::serialize(std::wostream & _Wostream)
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
		}
	}
}

//---------------------------------------------------------------
const wchar_t * math_mpadded::ns = L"mpadded";
const wchar_t * math_mpadded::name = L"mpadded";
//----------------------------------------------------------------------------------------------------

void math_mpadded::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mpadded::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mpadded::serialize(std::wostream & _Wostream)
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
		}
	}
}

}
}
