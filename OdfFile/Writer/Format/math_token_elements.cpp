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
			common_attlist_.serialize(CP_GET_XML_NODE());
			
			if (text_)
			{
				CP_XML_CONTENT(XmlUtils::EncodeXmlString(*text_));
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
			common_attlist_.serialize(CP_GET_XML_NODE());

			CP_XML_ATTR_OPT(L"accent", accent_);
			CP_XML_ATTR_OPT(L"fence", fence_);
			CP_XML_ATTR_OPT(L"form", form_);
			CP_XML_ATTR_OPT(L"stretchy", stretchy_);

			if (text_)
			{
				CP_XML_CONTENT(XmlUtils::EncodeXmlString(*text_));
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
			common_attlist_.serialize(CP_GET_XML_NODE());
			if (text_)
			{
				CP_XML_CONTENT(XmlUtils::EncodeXmlString(*text_));
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
			common_attlist_.serialize(CP_GET_XML_NODE());
			if (text_)
			{
				CP_XML_CONTENT(XmlUtils::EncodeXmlString(*text_));
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
