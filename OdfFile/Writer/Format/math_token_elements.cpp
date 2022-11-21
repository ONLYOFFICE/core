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

#include "math_token_elements.h"
#include "style_text_properties.h"

//#include "odf_document.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>
#include <xml/simple_xml_writer.h>
#include <xml/utils.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

//---------------------------------------------------------------
const wchar_t * math_mi::ns = L"math";
const wchar_t * math_mi::name = L"mi";
//----------------------------------------------------------------------------------------------------
void math_mi::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mi::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mi::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
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

void math_mi::add_text(const std::wstring & Text)
{
	text_ = Text;
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mo::ns = L"math";
const wchar_t * math_mo::name = L"mo";
//----------------------------------------------------------------------------------------------------
void math_mo::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mo::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mo::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			if (text_)
			{
				CP_XML_ATTR_OPT(L"accent", accent_);
				CP_XML_ATTR_OPT(L"fence", fence_);
				CP_XML_ATTR_OPT(L"form", form_);
				CP_XML_ATTR_OPT(L"stretchy", stretchy_);
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

void math_mo::add_text(const std::wstring & Text)
{
	text_ = Text;
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mn::ns = L"math";
const wchar_t * math_mn::name = L"mn";
//----------------------------------------------------------------------------------------------------
void math_mn::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mn::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mn::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
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

void math_mn::add_text(const std::wstring & Text)
{
	text_ = Text;
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_ms::ns = L"math";
const wchar_t * math_ms::name = L"ms";
//----------------------------------------------------------------------------------------------------
void math_ms::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_ms::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_ms::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			CP_XML_ATTR_OPT(L"math:text", text_);
			common_attlist_.serialize(CP_GET_XML_NODE());
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}			
		}
	}
}
//----------------------------------------------------------------------------------------------------
const wchar_t * math_mspace::ns = L"math";
const wchar_t * math_mspace::name = L"mspace";
//----------------------------------------------------------------------------------------------------
void math_mspace::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mspace::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mspace::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			CP_XML_ATTR_OPT(L"math:text", text_);
			common_attlist_.serialize(CP_GET_XML_NODE());
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtext::ns = L"math";
const wchar_t * math_mtext::name = L"mtext";
//----------------------------------------------------------------------------------------------------
void math_mtext::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mtext::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mtext::add_text(const std::wstring & Text)
{
	text_ = Text;
}

void math_mtext::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
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
//----------------------------------------------------------------------------------------------------
const wchar_t * math_mglyph::ns = L"math";
const wchar_t * math_mglyph::name = L"mglyph";
//----------------------------------------------------------------------------------------------------
void math_mglyph::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mglyph::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mglyph::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			CP_XML_ATTR_OPT(L"math:text", text_);
			common_attlist_.serialize(CP_GET_XML_NODE());
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
} // odf_writer
} // cpdoccore
}
