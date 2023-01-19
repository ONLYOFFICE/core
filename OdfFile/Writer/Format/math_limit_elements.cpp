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

#include "math_limit_elements.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>
#include <xml/simple_xml_writer.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

//---------------------------------------------------------------
const wchar_t * math_msub::ns = L"math";
const wchar_t * math_msub::name = L"msub";
//----------------------------------------------------------------------------------------------------
void math_msub::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msub::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_msub::serialize(std::wostream & _Wostream)
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
const wchar_t * math_msup::ns = L"math";
const wchar_t * math_msup::name = L"msup";
//----------------------------------------------------------------------------------------------------
void math_msup::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msup::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_msup::serialize(std::wostream & _Wostream)
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
const wchar_t * math_msubsup::ns = L"math";
const wchar_t * math_msubsup::name = L"msubsup";
//----------------------------------------------------------------------------------------------------
void math_msubsup::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msubsup::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_msubsup::serialize(std::wostream & _Wostream)
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
const wchar_t * math_none::ns = L"math";
const wchar_t * math_none::name = L"none";
//----------------------------------------------------------------------------------------------------
void math_none::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_none::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_none::serialize(std::wostream & _Wostream)
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
const wchar_t * math_mprescripts::ns = L"math";
const wchar_t * math_mprescripts::name = L"mprescripts";

void math_mprescripts::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	//CP_CREATE_ELEMENT(content_); //TODO pich
}

void math_mprescripts::add_child_element(const office_element_ptr & child_element)
{
	//content_.push_back(child_element);
}

void math_mprescripts::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			// pich_qa?
			/*for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}*/
		}
	}
}
//---------------------------------------------------------------
const wchar_t * math_mmultiscripts::ns = L"math";
const wchar_t * math_mmultiscripts::name = L"mmultiscripts";
//----------------------------------------------------------------------------------------------------
void math_mmultiscripts::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mmultiscripts::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mmultiscripts::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			for (size_t i = 0; i < pre_content_.size(); i++)
			{
				if (!pre_content_[i]) continue;
				pre_content_[i]->serialize(CP_XML_STREAM());
			}
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}			
		}
	}
}
//---------------------------------------------------------------
const wchar_t * math_munderover::ns = L"math";
const wchar_t * math_munderover::name = L"munderover";
//----------------------------------------------------------------------------------------------------
void math_munderover::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_munderover::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_munderover::serialize(std::wostream & _Wostream)
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
const wchar_t * math_mover::ns = L"math";
const wchar_t * math_mover::name = L"mover";
//----------------------------------------------------------------------------------------------------
void math_mover::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mover::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_mover::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
	{
		CP_XML_NODE_SIMPLE_NONS()
		{
			CP_XML_ATTR_OPT(L"accent", accent);
			for (size_t i = 0; i < content_.size(); i++)
			{
				if (!content_[i]) continue;
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
//---------------------------------------------------------------
const wchar_t * math_munder::ns = L"math";
const wchar_t * math_munder::name = L"munder";
//----------------------------------------------------------------------------------------------------
void math_munder::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_munder::add_child_element(const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}

void math_munder::serialize(std::wostream & _Wostream)
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
