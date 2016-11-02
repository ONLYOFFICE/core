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

#include "office_body.h"

#include "serialize_elements.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

#include <cpdoccore/odf/odf_document.h>
#include "odfcontext.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {



// office:body
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_body::ns = L"office";
const wchar_t * office_body::name = L"body";

std::wostream & office_body::text_to_stream(std::wostream & _Wostream) const
{
    if (content_)
        content_->text_to_stream(_Wostream);
    return _Wostream;
}

office_body::office_body()
{}

void office_body::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    
}

void office_body::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void office_body::add_text(const std::wstring & Text)
{
    // TODO : error
}

void office_body::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.start_body();

    if (content_)
        content_->xlsx_convert(Context);

    Context.end_body();
}

void office_body::docx_convert(oox::docx_conversion_context & Context)
{
    std::vector<style_master_page*> & masterPages = Context.root()->odf_context().pageLayoutContainer().master_pages();
    if (!masterPages.empty())
    {
        Context.set_master_page_name(masterPages[0]->style_master_page_attlist_.style_name_.get_value_or(style_ref(L"Standard")).style_name() );
    }

	const page_layout_instance * layout = Context.root()->odf_context().pageLayoutContainer().page_layout_first();
	
	if (layout)   //два раза - чтобы дефолтовые настройки всегда были
	{
		//Context.add_page_properties(layout->name());
		Context.add_page_properties(layout->name());
	}
	else
	{
		Context.add_page_properties(L""); // 
		Context.add_page_properties(L"");
	}
//backcolor (for all pages) 
    if (page_layout_instance * firtsPageLayout = Context.root()->odf_context().pageLayoutContainer().page_layout_by_name(Context.get_page_properties()))
	{
        if (style_page_layout_properties * prop = firtsPageLayout->properties())
		{
			if (prop->docx_back_serialize(Context.output_stream(), Context))
			{
				Context.set_settings_property(odf_reader::_property(L"displayBackgroundShape",true));
			}
		}
	}
//content
	Context.start_body();
    
	if (content_)
        content_->docx_convert(Context);

	Context.get_headers_footers().set_enable_write(true);

	if (!Context.get_section_context().dump_.empty() && !Context.get_table_context().in_table())
	{
		Context.output_stream() << Context.get_section_context().dump_;
		Context.get_section_context().dump_.clear();
	}
	else
	{
		if (page_layout_instance * lastPageLayout = Context.root()->odf_context().pageLayoutContainer().page_layout_by_name(Context.get_page_properties()))
		{
			lastPageLayout->docx_convert_serialize(Context.output_stream(), Context);
			//Context.remove_page_properties();
		}
	}

    Context.end_body();    
}
void office_body::pptx_convert(oox::pptx_conversion_context & Context)
{
    Context.start_body();

    if (content_)
        content_->pptx_convert(Context);

    Context.end_body();
}
}
}
