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
#include <xml/simple_xml_writer.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

//---------------------------------------------------------------
const wchar_t * math_msub::ns = L"math";
const wchar_t * math_msub::name = L"msub";
//----------------------------------------------------------------------------------------------------
void math_msub::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msub::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	//<msub> base subscript </msub>

	CP_CREATE_ELEMENT(content_);
}

void math_msub::oox_convert(oox::math_context & Context)
{//2 elements
	if (content_.size() != 2)
	{
		return;//todooo
	}
    std::wostream & strm = Context.output_stream();
	
	office_math_element* math_element = NULL;

	strm << L"<m:sSub>";

		strm << L"<m:e>";
			Context.is_need_e_ = false;

			math_element = dynamic_cast<office_math_element*>(content_[0].get());
			math_element->oox_convert(Context);		
		strm << L"</m:e>";
		
		strm << L"<m:sub>";
			Context.is_need_e_ = false; //??

			math_element = dynamic_cast<office_math_element*>(content_[1].get());
			math_element->oox_convert(Context);		
		strm << L"</m:sub>";

	strm << L"</m:sSub>";
}
//---------------------------------------------------------------
const wchar_t * math_msup::ns = L"math";
const wchar_t * math_msup::name = L"msup";
//----------------------------------------------------------------------------------------------------
void math_msup::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msup::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msup::oox_convert(oox::math_context & Context)
{//2 elements
	if (content_.size() != 2)
	{
		return;//todooo
	}
    std::wostream & strm = Context.output_stream();
	
	office_math_element* math_element = NULL;

	strm << L"<m:sSup>";

		strm << L"<m:e>";
			Context.is_need_e_ = false;

			math_element = dynamic_cast<office_math_element*>(content_[0].get());
			math_element->oox_convert(Context);		
		strm << L"</m:e>";
		
		strm << L"<m:sup>";
			math_element = dynamic_cast<office_math_element*>(content_[1].get());
			math_element->oox_convert(Context);		
		strm << L"</m:sup>";

	strm << L"</m:sSup>";
}

//---------------------------------------------------------------
const wchar_t * math_msubsup::ns = L"math";
const wchar_t * math_msubsup::name = L"msubsup";
//----------------------------------------------------------------------------------------------------
void math_msubsup::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_msubsup::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_msubsup::oox_convert(oox::math_context & Context)
{//3 elements
    std::wostream & strm = Context.output_stream();
	
	office_math_element* math_element = NULL;

	strm << L"<m:sSubSup>";
	
	Context.is_need_e_ = false;

		strm << L"<m:e>";
			math_element = dynamic_cast<office_math_element*>(content_[0].get());
			math_element->oox_convert(Context);		
		strm << L"</m:e>";
		
		strm << L"<m:sub>";
			math_element = dynamic_cast<office_math_element*>(content_[1].get());
			math_element->oox_convert(Context);		
		strm << L"</m:sub>";

		strm << L"<m:sup>";
			math_element = dynamic_cast<office_math_element*>(content_[2].get());
			math_element->oox_convert(Context);		
		strm << L"</m:sup>";

	strm << L"</m:sSubSup>";
}
//---------------------------------------------------------------
const wchar_t * math_none::ns = L"math";
const wchar_t * math_none::name = L"none";
//----------------------------------------------------------------------------------------------------
void math_none::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_none::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_none::oox_convert(oox::math_context & Context)
{

}

//---------------------------------------------------------------
const wchar_t * math_mprescripts::ns = L"math";
const wchar_t * math_mprescripts::name = L"mprescripts";

void math_mprescripts::oox_convert(oox::math_context & Context)
{
}
//---------------------------------------------------------------
const wchar_t * math_mmultiscripts::ns = L"math";
const wchar_t * math_mmultiscripts::name = L"mmultiscripts";
//----------------------------------------------------------------------------------------------------
void math_mmultiscripts::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mmultiscripts::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if (Name == L"mprescripts")
	{
		bSeparate = true;
	}
	else
	{
		if (bSeparate)
			CP_CREATE_ELEMENT(pre_content_);
		else
			CP_CREATE_ELEMENT(content_);
	}
}

void math_mmultiscripts::oox_convert(oox::math_context & Context)
{//1* elements
	std::wostream & strm = Context.output_stream();

	strm << L"<m:sPre>";
		strm << L"<m:sub>";
		for (size_t i = 0; i < pre_content_.size() / 2; i++)
		{
			office_math_element* math_element = dynamic_cast<office_math_element*>(pre_content_[i].get());
			math_element->oox_convert(Context);
		}
		strm << L"</m:sub>";
		strm << L"<m:sup>";
			for (size_t i = pre_content_.size() / 2; i < pre_content_.size(); i++)
			{
				office_math_element* math_element = dynamic_cast<office_math_element*>(pre_content_[i].get());
				math_element->oox_convert(Context);
			}
		strm << L"</m:sup>";
		strm << L"<m:e>";
			for (size_t i = 0; i < content_.size(); i++)
			{
				office_math_element* math_element = dynamic_cast<office_math_element*>(content_[i].get());
				math_element->oox_convert(Context);
			}
		strm << L"</m:e>";
	strm << L"</m:sPre>";
}
//---------------------------------------------------------------
const wchar_t * math_munderover::ns = L"math";
const wchar_t * math_munderover::name = L"munderover";
//----------------------------------------------------------------------------------------------------
void math_munderover::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_munderover::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_munderover::oox_convert(oox::math_context & Context)
{//3 elements (+1)
	if (content_.size() < 4)
	{
		return;
	}
	std::wstringstream strmTemp; 
	strmTemp.clear();
	content_[0]->text_to_stream(strmTemp);

	std::wstring sBase = strmTemp.str();

	std::wostream & strm = Context.output_stream();

	office_math_element* math_element = NULL;

	if (sBase.size() < 3)
	{	
		strm << L"<m:nary>";

			strm << L"<m:naryPr>";
			{
				strm << L"<m:chr m:val=\"";
					content_[0]->text_to_stream(strm);
				strm << L"\"/>";
			}
			strm << L"</m:naryPr>";
			strm << L"<m:sub>";
			{
				math_element = dynamic_cast<office_math_element*>(content_[1].get());
				math_element->oox_convert(Context);		
			}
			strm << L"</m:sub>";
			
			strm << L"<m:sup>";
			{
				math_element = dynamic_cast<office_math_element*>(content_[2].get());
				math_element->oox_convert(Context);		
			}
			strm << L"</m:sup>";

			strm << L"<m:e>";
			{
				math_element = dynamic_cast<office_math_element*>(content_[3].get());
				math_element->oox_convert(Context);		
			}
			strm << L"</m:e>";
		strm << L"</m:nary>";
	}
	else if (sBase == L"lim")
	{
		strm << L"<m:limUpp>";
		strm << L"<m:limUppPr/>";
			strm << L"<m:e>";
				strm << L"<m:limLow>";	
					strm << L"<m:limLowPr/>";
					strm << L"<m:e>";
						math_element = dynamic_cast<office_math_element*>(content_[0].get());
						math_element->oox_convert(Context);		
					strm << L"</m:e>";
					strm << L"<m:lim>";
						math_element = dynamic_cast<office_math_element*>(content_[1].get());
						math_element->oox_convert(Context);		
					strm << L"</m:lim>";
				strm << L"</m:limLow>";
			strm << L"</m:e>";
			strm << L"<m:lim>";
				math_element = dynamic_cast<office_math_element*>(content_[2].get());
				math_element->oox_convert(Context);		
			strm << L"</m:lim>";
		strm << L"</m:limUpp>";
	
		math_element = dynamic_cast<office_math_element*>(content_[3].get());
		math_element->oox_convert(Context);		
	}
}
//---------------------------------------------------------------
const wchar_t * math_mover::ns = L"math";
const wchar_t * math_mover::name = L"mover";
//----------------------------------------------------------------------------------------------------
void math_mover::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_mover::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_mover::oox_convert(oox::math_context & Context)
{//2 elements
	std::wostream & strm = Context.output_stream();

	office_math_element* math_element = NULL;

	strm << L"<m:limUpp>";	
		strm << L"<m:limUppPr/>";
		strm << L"<m:e>";
	
		Context.is_need_e_ = false;

			math_element = dynamic_cast<office_math_element*>(content_[0].get());
			math_element->oox_convert(Context);		
		strm << L"</m:e>";
		strm << L"<m:lim>";
			math_element = dynamic_cast<office_math_element*>(content_[1].get());
			math_element->oox_convert(Context);		
		strm << L"</m:lim>";
	strm << L"</m:limUpp>";
}
//---------------------------------------------------------------
const wchar_t * math_munder::ns = L"math";
const wchar_t * math_munder::name = L"munder";
//----------------------------------------------------------------------------------------------------
void math_munder::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_munder::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void math_munder::oox_convert(oox::math_context & Context)
{//2 elements
	std::wostream & strm = Context.output_stream();

	bool need_e = Context.is_need_e_;
	if (need_e)
	{
		Context.output_stream() << L"<m:e>";
	}
	Context.is_need_e_ = false;

	office_math_element* math_element = NULL;
	strm << L"<m:limLow>";	
		strm << L"<m:limLowPr/>";
		strm << L"<m:e>";
			math_element = dynamic_cast<office_math_element*>(content_[0].get());
			math_element->oox_convert(Context);		
		strm << L"</m:e>";
		strm << L"<m:lim>";
			math_element = dynamic_cast<office_math_element*>(content_[1].get());
			math_element->oox_convert(Context);		
		strm << L"</m:lim>";
	strm << L"</m:limLow>";

	if (need_e)
	{
		Context.output_stream() << L"</m:e>";
	}
	Context.is_need_e_ = need_e;
}
}
}
