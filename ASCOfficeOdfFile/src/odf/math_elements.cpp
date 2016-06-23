/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//---------------------------------------------------------------
const wchar_t * office_math::ns = L"math";
const wchar_t * office_math::name = L"math";

//----------------------------------------------------------------------------------------------------

void office_math::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void office_math::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME1(L"semantics")
    {
        CP_CREATE_ELEMENT(semantics_);
    }
}


void office_math::docx_convert(oox::docx_conversion_context & Context) 
{
	if (semantics_)
		semantics_->docx_convert(Context);
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_semantics::ns = L"math";
const wchar_t * math_semantics::name = L"semantics";
//----------------------------------------------------------------------------------------------------

void math_semantics::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void math_semantics::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME1(L"annotation")
    {
        CP_CREATE_ELEMENT(annotation_);
    }
	else
        CP_CREATE_ELEMENT(content_);

}


void math_semantics::docx_convert(oox::docx_conversion_context & Context) 
{
	Context.start_math_formula();

	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }

	Context.end_math_formula();
}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_annotation::ns = L"math";
const wchar_t * math_annotation::name = L"annotation";
//----------------------------------------------------------------------------------------------------

void math_annotation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
// ver 2	
   CP_APPLY_ATTR(L"math:encoding", encoding_);

// ver 3
    if (!encoding_)	CP_APPLY_ATTR(L"encoding", encoding_);
	
}

void math_annotation::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);

}

void math_annotation::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

void math_annotation::docx_convert(oox::docx_conversion_context & Context) 
{

}

//----------------------------------------------------------------------------------------------------
const wchar_t * math_annotation_xml::ns = L"math";
const wchar_t * math_annotation_xml::name = L"annotation-xml";
//----------------------------------------------------------------------------------------------------

void math_annotation_xml::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
// ver 2	
   CP_APPLY_ATTR(L"math:encoding", encoding_);

// ver 3
    if (!encoding_)	CP_APPLY_ATTR(L"encoding", encoding_);
	
}

void math_annotation_xml::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);

}

void math_annotation_xml::add_text(const std::wstring & Text) 
{
    text_ = Text;
}

void math_annotation_xml::docx_convert(oox::docx_conversion_context & Context) 
{

}
//----------------------------------------------------------------------------------------------------

}
}
