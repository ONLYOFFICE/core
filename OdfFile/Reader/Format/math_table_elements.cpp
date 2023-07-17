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

#include "math_table_elements.h"
#include "math_layout_elements.h"
#include "math_limit_elements.h"

#include <xml/xmlchar.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtable::ns		= L"math";
const wchar_t * math_mtable::name	= L"mtable";
//----------------------------------------------------------------------------------------------------

void math_mtable::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mtable::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (L"mtr" == Name)
	{
		CP_CREATE_ELEMENT(rows_);

		math_mtr* mtr = rows_.empty() ? NULL : dynamic_cast<math_mtr*>(rows_.back().get());
		if (mtr)
		{
			if (mtr->cells_.size() > 1)
				bMatrix = true;
		}
	}
	else
	{
		CP_CREATE_ELEMENT(content_);
	}
}

void math_mtable::oox_convert(oox::math_context & Context)
{//0* elements
	std::wostream & strm = Context.output_stream();
	
	if (bMatrix) strm << L"<m:m>";
	else strm << L"<m:eqArr>";	

	Context.start_level();
	
	Context.levels.back().bMatrix = bMatrix; 
	Context.levels.back().is_need_e_ = !bMatrix;

		for (size_t i = 0; i < content_.size(); i++)
		{
			office_math_element* math_element = dynamic_cast<office_math_element*>(content_[i].get());			

			math_element->oox_convert(Context);
		}
		for (size_t i = 0; i < rows_.size(); i++)
		{
			office_math_element* math_element = dynamic_cast<office_math_element*>(rows_[i].get());

			math_element->oox_convert(Context);
		}
	if (bMatrix) strm << L"</m:m>";
	else strm << L"</m:eqArr>";

	Context.end_level();
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtr::ns	= L"math";
const wchar_t * math_mtr::name	= L"mtr";
//----------------------------------------------------------------------------------------------------

void math_mtr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mtr::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (L"mtd" == Name)
	{
		CP_CREATE_ELEMENT(cells_);
	}
	else
	{
		CP_CREATE_ELEMENT(content_);
	}
}

void math_mtr::oox_convert(oox::math_context & Context)
{//0* elements
	std::wostream & strm = Context.output_stream();
	
	if (Context.levels.back().is_need_e_)
		strm << L"<m:e>";

	if (Context.levels.back().bMatrix) strm << L"<m:mr>";

	bool bMatrix = Context.levels.back().bMatrix;
	
	Context.start_level();
	Context.levels.back().is_need_e_ = bMatrix;
	Context.levels.back().bMatrix = bMatrix;

	for (size_t i = 0; i < content_.size(); i++)
	{
		office_math_element* math_element = dynamic_cast<office_math_element*>(content_[i].get());
		math_element->oox_convert(Context);
	}
	for (size_t i = 0; i < cells_.size(); i++)
	{
		office_math_element* math_element = dynamic_cast<office_math_element*>(cells_[i].get());
		math_element->oox_convert(Context);
	}
	Context.end_level();

	if (Context.levels.back().bMatrix) strm << L"</m:mr>";

	if (Context.levels.back().is_need_e_)
		strm << L"</m:e>";
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mlabeledtr::ns		= L"math";
const wchar_t * math_mlabeledtr::name	= L"mlabeledtr";
//----------------------------------------------------------------------------------------------------

void math_mlabeledtr::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mlabeledtr::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mlabeledtr::oox_convert(oox::math_context & Context)
{

}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_mtd::ns	= L"math";
const wchar_t * math_mtd::name	= L"mtd";
//----------------------------------------------------------------------------------------------------

void math_mtd::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mtd::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mtd::oox_convert(oox::math_context & Context)
{
	std::wostream & strm = Context.output_stream();

	if (Context.levels.back().is_need_e_)
		strm << L"<m:e>";
	
	Context.start_level();
	
	for (size_t i = 0; i < content_.size(); i++)
	{
		office_math_element* math_element = dynamic_cast<office_math_element*>(content_[i].get());			
		math_element->oox_convert(Context);
	}
	Context.end_level();
	
	if (Context.levels.back().is_need_e_)
		strm << L"</m:e>";
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_maligngroup::ns	= L"math";
const wchar_t * math_maligngroup::name	= L"maligngroup";
//----------------------------------------------------------------------------------------------------

void math_maligngroup::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_maligngroup::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_maligngroup::oox_convert(oox::math_context & Context)
{

}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_malignmark::ns		= L"math";
const wchar_t * math_malignmark::name	= L"malignmarke";
//----------------------------------------------------------------------------------------------------

void math_malignmark::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_malignmark::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_malignmark::oox_convert(oox::math_context & Context)
{

}
}
}
