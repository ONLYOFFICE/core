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

#include "math_table_elements.h"
#include "math_layout_elements.h"
#include "math_limit_elements.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtable::ns		= L"math";
const wchar_t * math_mtable::name	= L"mtable";
//----------------------------------------------------------------------------------------------------

void math_mtable::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mtable::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mtable::serialize(std::wostream & _Wostream)
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

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtr::ns	= L"math";
const wchar_t * math_mtr::name	= L"mtr";
//----------------------------------------------------------------------------------------------------

void math_mtr::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mtr::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mtr::serialize(std::wostream & _Wostream)
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

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mlabeledtr::ns		= L"math";
const wchar_t * math_mlabeledtr::name	= L"mlabeledtr";
//----------------------------------------------------------------------------------------------------

void math_mlabeledtr::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mlabeledtr::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mlabeledtr::serialize(std::wostream & _Wostream)
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

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtd::ns	= L"math";
const wchar_t * math_mtd::name	= L"mtd";
//----------------------------------------------------------------------------------------------------
void math_mtd::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mtd::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mtd::serialize(std::wostream & _Wostream)
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

//----------------------------------------------------------------------------------------------------
const wchar_t * math_maligngroup::ns	= L"math";
const wchar_t * math_maligngroup::name	= L"maligngroup";
//----------------------------------------------------------------------------------------------------

void math_maligngroup::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_maligngroup::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_maligngroup::serialize(std::wostream & _Wostream)
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


//----------------------------------------------------------------------------------------------------
const wchar_t * math_malignmark::ns		= L"math";
const wchar_t * math_malignmark::name	= L"malignmarke";
//----------------------------------------------------------------------------------------------------

void math_malignmark::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_malignmark::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_malignmark::serialize(std::wostream & _Wostream)
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
} // odf_writer
} // cpdoccore
