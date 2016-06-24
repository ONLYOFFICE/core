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

#include <iostream>

#include "odf_document_impl.h"

#include <cpdoccore/odf/odf_document.h>
#include <cpdoccore/CPString.h>
#include <cpdoccore/xml/sax.h>
#include <cpdoccore/common/readdocelement.h>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "abstract_xml.h"
#include "odfcontext.h"

#include "office_body.h"
#include "office_document.h"
#include "office_elements.h"
#include "office_text.h"
#include "office_spreadsheet.h"
#include "office_presentation.h"
#include "office_chart.h"
#include "office_annotation.h"
#include "office_settings.h"
#include "office_scripts.h"
#include "office_event_listeners.h"

#include "styles.h"
#include "style_regions.h"
#include "style_presentation.h"

#include "templates.h"

#include "math_elements.h"

#include "paragraph_elements.h"
#include "text_elements.h"

#include "table_calculation_settings.h"
#include "number_style.h"
#include "list.h"
#include "font_face.h"
#include "table.h"
#include "draw_common.h"

#include "draw_page.h"

#include "documentcontext.h"

#include "../progressCallback.h"
#define PROGRESSEVENT_ID 0

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

namespace {
content_xml_t_ptr read_file_content(const std::wstring & Path)
{
    xml::sax_ptr Reader = xml::create_sax( Path.c_str());
    content_xml_t_ptr result( new content_xml_t() );
    
	const std::wstring namespacePrefix	= Reader->namespacePrefix();
	const std::wstring localName		= Reader->nodeLocalName();
	
	result->add_child_element(Reader.get(), namespacePrefix, localName);		

    return result;
}
}

odf_document::Impl::Impl(const std::wstring & folderPath, const ProgressCallback* CallBack) : 
			context_(new odf_read_context()), base_folder_(folderPath), pCallBack(CallBack), bUserStopConvert (0)
{
	office_mime_type_ = 0;
	encrypted = false;

    std::wstring content_xml	= folderPath + FILE_SEPARATOR_STR + L"content.xml";
    std::wstring styles_xml		= folderPath + FILE_SEPARATOR_STR + L"styles.xml";
    std::wstring meta_xml		= folderPath + FILE_SEPARATOR_STR + L"meta.xml";
    std::wstring settings_xml	= folderPath + FILE_SEPARATOR_STR + L"settings.xml";
	std::wstring manifest_xml	= folderPath + FILE_SEPARATOR_STR + L"META-INF" + FILE_SEPARATOR_STR + L"manifest.xml";

    _CP_LOG << L"[info] read manifest.xml" << std::endl;
    manifest_xml_ = read_file_content(manifest_xml);

      _CP_LOG << L"[info] read settings.xml" << std::endl;
    settings_xml_ = read_file_content(settings_xml);

    _CP_LOG << L"[info] read content.xml" << std::endl;
    content_xml_ = read_file_content(content_xml);

      _CP_LOG << L"[info] read styles.xml" << std::endl;
    styles_xml_ = read_file_content(styles_xml);

    _CP_LOG << L"[info] parse fonts" << std::endl;
    parse_fonts();

    _CP_LOG << L"[info] parse styles" << std::endl;
    parse_styles();

    _CP_LOG << L"[info] parse manifest" << std::endl;
    parse_manifests();

    _CP_LOG << L"[info] parse settings" << std::endl;
    parse_settings();

	UpdateProgress(400000);
}
bool odf_document::Impl::UpdateProgress(long nComplete)
{
	if (pCallBack)
	{
		pCallBack->OnProgress (pCallBack->caller, PROGRESSEVENT_ID, nComplete);

		bUserStopConvert = 0;
		pCallBack->OnProgressEx (pCallBack->caller, PROGRESSEVENT_ID, nComplete, &bUserStopConvert);

		if (bUserStopConvert !=0 ) return true;
	}

	return false;
}
void odf_document::Impl::parse_fonts()
{
    do 
    {
        if (!content_xml_)
        {
            _CP_LOG << L"[warning] empty content xml\n";
            break;
        }

        office_document_base * document = dynamic_cast<office_document_base *>( content_xml_->get_content() );
        if (!document)
        {
            _CP_LOG << L"[warning] empty document\n";
            break;
        }

        office_font_face_decls* fontFaceDecls = dynamic_cast<office_font_face_decls*>( document->office_font_face_decls_.get() );
        
        if (!fontFaceDecls)
        {
            _CP_LOG << L"[warning] empty font face decls\n";
            break;
        }

        BOOST_FOREACH(office_element_ptr & elm, fontFaceDecls->style_font_face_)
        {
            style_font_face* fontFace = dynamic_cast<style_font_face*>( elm.get() );
            if (!fontFace)
            {
                _CP_LOG << L"[warning] read font face error\n";
                continue;
            }

            const std::wstring styleName = fontFace->style_name_.get_value_or(L"");
            const std::wstring fontNameTmp = fontFace->svg_font_family_.get_value_or(L"");
            std::wstring fontName = L"";

            std::vector<std::wstring> fontNames;
            boost::algorithm::split(fontNames, fontNameTmp, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);
            if (fontNames.empty() || fontNames[0].empty())
                fontName = styleName;
            else
                fontName = fontNames[0];

            std::wstring altName = fontNames.size() >= 2 ? fontNames[1] : L"";

            const std::wstring charset = fontFace->style_font_charset_.get_value_or(L"") == L"x-symbol" ? L"02" : L"00";
            const std::wstring fontFamily 
                = fontFace->style_font_family_generic_.get_value_or( font_family_generic(font_family_generic::System) ).get_type() 
                == font_family_generic::System ?
                L"auto" : boost::lexical_cast<std::wstring>(*fontFace->style_font_family_generic_);
            const std::wstring fontPitch = fontFace->style_font_pitch_ ? boost::lexical_cast<std::wstring>(*fontFace->style_font_pitch_) : L"";

            boost::algorithm::trim_if(fontName, boost::algorithm::is_any_of("'"));
            
            font_instance_ptr fontInstance( new font_instance (styleName, fontName, charset, fontFamily, fontPitch, altName) );

            context_->fontContainer().add_font( fontInstance );
            /*if  (!context_.fontContainer().font_by_name(fontStyleName))
            {
                // два раза не добавляем?
                
            }
            else
            {
                _CP_LOG << L"[warning] duplicate font name (" << fontName << L")\n";
            }*/
        }
    }
    while (0);
}
void odf_document::Impl::parse_manifests()
{
	if (!manifest_xml_)return;

    office_document_base * document = dynamic_cast<office_document_base *>( manifest_xml_->get_content() );
    
	if (!document)return;

	int res =-1;
	BOOST_FOREACH(office_element_ptr & elm, document->manifests_)
    {
		manifest_entry * entry = dynamic_cast<manifest_entry *>(elm.get());
		if (!entry)continue;

		if (entry->full_path_==L"content.xml" && entry->encryption_) encrypted = true;

		if (entry->full_path_==L"/")
		{
			res = entry->media_type_.find(L"application/vnd.oasis.opendocument.text");
			if (res>=0)
			{
				office_mime_type_ = 1;
			}
			res = entry->media_type_.find(L"application/vnd.oasis.opendocument.spreadsheet");
			if (res>=0)
			{
				office_mime_type_ = 2;
			}
			res = entry->media_type_.find(L"application/vnd.oasis.opendocument.presentation");
			if (res>=0)
			{
				office_mime_type_ = 3;
			}
		}
	}
}

void odf_document::Impl::parse_settings()
{
	if (!settings_xml_)return;
   
	office_document_base * document = dynamic_cast<office_document_base *>( settings_xml_->get_content() );
	if (!document)	return;

	office_settings * settings = dynamic_cast<office_settings*>(document->office_settings_.get());
	if (!settings)	return;

	BOOST_FOREACH(office_element_ptr & elm, settings->content_)
    {
		settings_config_item_set * item_set = dynamic_cast<settings_config_item_set *>(elm.get());
		if (!item_set)continue;

		if (item_set->config_name_ == L"ooo:configuration-settings")
		{
			BOOST_FOREACH(office_element_ptr & elm_sett, item_set->content_)
			{		
				settings_config_item * sett = dynamic_cast<settings_config_item *>(elm_sett.get());
				if (!sett)continue;

				context_->Settings().add(sett->config_name_, elm_sett);
			}
		}
	}
}

void odf_document::Impl::parse_styles()
{
    do
    {
        if (!styles_xml_)
        {
            _CP_LOG << L"[warning] empty styles xml\n";
            break;
        }

        office_document_base * document = dynamic_cast<office_document_base *>( styles_xml_->get_content() );
        if (!document)
        {
            _CP_LOG << L"[warning] empty document\n";
            break;
        }
       
        // parse automatic styles - эти стили используют объекты которые в оазис находятся в этом же документе
		//переопределяем имя - иначе при поиске может возникнуть коллизия.
        do
        {
            office_automatic_styles * automaticStyles = dynamic_cast<office_automatic_styles *>( document->office_automatic_styles_.get() );
            if (!automaticStyles)
            {
                _CP_LOG << L"[warning] error reading automatic styles\n";
                break;
            }

            // parse page layout
            BOOST_FOREACH(office_element_ptr & elm, automaticStyles->style_page_layout_)
            {
                style_page_layout * pageLayout = dynamic_cast<style_page_layout *>(elm.get());
                if (!pageLayout)
                {
                    _CP_LOG << L"[warning] error reading page layout\n";
                    continue;
                }

                context_->pageLayoutContainer().add_page_layout(pageLayout);
            } // end parse page layout

            BOOST_FOREACH(office_element_ptr & elm, automaticStyles->styles_.style_style_)
            {
                style * styleInst = dynamic_cast<style*>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG << L"[warning] error reading style\n";
                    continue;
                }

				context_->styleContainer().add_style
					(	L"common:" + styleInst->style_name_,
						styleInst->style_display_name_.get_value_or(L""),
						styleInst->style_family_.get_type(),
						&(styleInst->style_content_),
						true,
						false,
						styleInst->style_parent_style_name_.get_value_or(L""),
						styleInst->style_next_style_name_.get_value_or(L""),
						styleInst->style_data_style_name_.get_value_or(L"")
                    );
            }
            // list styles
            BOOST_FOREACH(office_element_ptr & elm, automaticStyles->styles_.text_list_style_)
            {
                text_list_style * listStyle = dynamic_cast<text_list_style *>(elm.get());
                if (!listStyle)
                {
                    _CP_LOG << L"[warning] error list style\n";
                    continue;
                }

                context_->listStyleContainer().add_list_style(listStyle);
            }
        }
        while(0); 

        // parse master pages
        do
        {
            office_master_styles * masterStyles = dynamic_cast<office_master_styles *>( document->office_master_styles_.get() );
            if (!masterStyles)
            {
                _CP_LOG << L"[warning] error reading master styles\n";
                break;
            }

            BOOST_FOREACH(office_element_ptr & elm, masterStyles->style_master_page_)
            {
                style_master_page * masterPage = dynamic_cast<style_master_page *>(elm.get());
                if (!masterPage)
                {
                    _CP_LOG << L"[warning] error reading master page\n";
                    continue;
                }

                const std::wstring styleName = masterPage->style_master_page_attlist_.style_name_.get_value_or(style_ref(L"")).style_name();
                const std::wstring pageLayoutName = masterPage->style_master_page_attlist_.style_page_layout_name_.get_value_or(style_ref(L"")).style_name();

                context_->pageLayoutContainer().add_master_page(styleName, pageLayoutName, masterPage);
            }        
        }
        while(0); // end parse master pages

        // parse styles
        do 
        {
            office_styles * docStyles = dynamic_cast<office_styles *>(document->office_styles_.get());
            if (!docStyles)
            {
                _CP_LOG << L"[warning] error reading styles\n";
                break;
            }

            // default styles
            BOOST_FOREACH(office_element_ptr & elm, docStyles->style_default_style_)
            {
                default_style * styleInst = dynamic_cast<default_style *>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG << L"[warning] error reading default style\n";
                    continue;
                }

                context_->styleContainer().add_style(L"",
					L"",
                    styleInst->style_family_.get_type(), 
                    &(styleInst->style_content_),
                    false,
                    true,
                    L"",
                    L"",
                    L"");                                            
            }
            BOOST_FOREACH(office_element_ptr & elm, docStyles->style_presentation_page_layout_)
            {
                style_presentation_page_layout * pageLayout = dynamic_cast<style_presentation_page_layout *>(elm.get());

                if (!pageLayout)
                    continue;

				context_->pageLayoutContainer().add_presentation_page_layout(pageLayout->style_name_.get_value_or(L""),pageLayout);
            }
        
            // common styles
            BOOST_FOREACH(office_element_ptr & elm, docStyles->styles_.style_style_)
            {
                style * styleInst = dynamic_cast<style*>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG << L"[warning] error reading style\n";
                    continue;
                }

                if (styleInst->style_master_page_name_ && !styleInst->style_master_page_name_->empty())
                    context_->styleContainer().add_master_page_name(styleInst->style_name_, *styleInst->style_master_page_name_);

                context_->styleContainer().add_style(styleInst->style_name_,
					styleInst->style_display_name_.get_value_or(L""),
                    styleInst->style_family_.get_type(),
                    &(styleInst->style_content_),
                    false,
                    false,
                    styleInst->style_parent_style_name_.get_value_or(L""),
                    styleInst->style_next_style_name_.get_value_or(L""),
                    styleInst->style_data_style_name_.get_value_or(L"")
                    );
            }

            // list styles
            BOOST_FOREACH(office_element_ptr & elm, docStyles->styles_.text_list_style_)
            {
                text_list_style * listStyle = dynamic_cast<text_list_style *>(elm.get());
                if (!listStyle)
                {
                    _CP_LOG << L"[warning] error list style\n";
                    continue;
                }

                context_->listStyleContainer().add_list_style(listStyle);
            }

            BOOST_FOREACH(const office_element_ptr & elm, docStyles->text_notes_configuration_)
            {
                const text_notes_configuration * conf = dynamic_cast<const text_notes_configuration *>(elm.get());
                if (!conf)
                    continue;
                
                context_->noteConfiguration().add( conf->noteclass_.get_type(), conf );
                //text::note_configuration
                                
            }

            BOOST_FOREACH(const office_element_ptr & elm, docStyles->styles_.number_styles_)
            {
                const number_style_base * style = dynamic_cast<const number_style_base *>(elm.get());

                if (!style)
                    continue;

                context_->numberStyles().add(style->get_style_name(), elm);
            }
           BOOST_FOREACH(const office_element_ptr & elm, docStyles->draw_styles_.draw_gradient_)
            {
                draw_gradient * style = dynamic_cast<draw_gradient *>(elm.get());

                if (!style)
                    continue;

				context_->drawStyles().add(L"gradient:" + style->get_style_name(), elm);
            }
            BOOST_FOREACH(const office_element_ptr & elm, docStyles->draw_styles_.draw_fill_image_)
            {
                draw_fill_image * style = dynamic_cast<draw_fill_image *>(elm.get());

                if (!style)
                    continue;

				context_->drawStyles().add(L"bitmap:" + style->get_style_name(), elm);
            }
			BOOST_FOREACH(const office_element_ptr & elm, docStyles->draw_styles_.draw_opacity_)
            {
                draw_opacity * style = dynamic_cast<draw_opacity *>(elm.get());

                if (!style)
                    continue;

				context_->drawStyles().add(L"opacity:" + style->get_style_name(), elm);
            }
			BOOST_FOREACH(const office_element_ptr & elm, docStyles->draw_styles_.draw_hatch_)
            {
                draw_hatch * style = dynamic_cast<draw_hatch *>(elm.get());

                if (!style)
                    continue;

				context_->drawStyles().add(L"hatch:" + style->get_style_name(), elm);
            }
            BOOST_FOREACH(const office_element_ptr & elm, docStyles->templates_.table_templates_)
            {
				table_table_template * style = dynamic_cast<table_table_template *>(elm.get());

                if (!style)
                    continue;

				context_->Templates().add(L"table:" + style->get_text_style_name(), elm);
            }
		}
        while(0); // end parse styles
    }
    while (0);

    do
    {
        if (!content_xml_)
        {
            _CP_LOG << L"[warning] empty content xml\n";
            break;
        }

        office_document_base * document = dynamic_cast<office_document_base *>( content_xml_->get_content() );
        if (!document)
        {
            _CP_LOG << L"[warning] empty document\n";
            break;
        }

        // parse automatic styles
        do 
        {
            office_automatic_styles * automaticStyles = dynamic_cast<office_automatic_styles *>(document->office_automatic_styles_.get());
            if (!automaticStyles)
            {
                _CP_LOG << L"[warning] error reading automatic styles\n";
                break;
            }

            BOOST_FOREACH(office_element_ptr & elm, automaticStyles->styles_.style_style_)
            {
                style * styleInst = dynamic_cast<style*>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG << L"[warning] error reading style\n";
                    continue;
                }

                if (styleInst->style_master_page_name_ && !styleInst->style_master_page_name_->empty())
                    context_->styleContainer().add_master_page_name(styleInst->style_name_, *styleInst->style_master_page_name_);

                context_->styleContainer().add_style(styleInst->style_name_,
					styleInst->style_display_name_.get_value_or(L""),
                    styleInst->style_family_.get_type(),
                    &(styleInst->style_content_),
                    true,
                    false,
                    styleInst->style_parent_style_name_.get_value_or(L""),
                    styleInst->style_next_style_name_.get_value_or(L""),
                    styleInst->style_data_style_name_.get_value_or(L"")                    
                    );
            }

            BOOST_FOREACH(office_element_ptr & elm, automaticStyles->styles_.text_list_style_)
            {
                text_list_style * listStyle = dynamic_cast<text_list_style *>(elm.get());
                if (!listStyle)
                {
                    _CP_LOG << L"[warning] error reading list style\n";
                    continue;
                }

                context_->listStyleContainer().add_list_style(listStyle);
            }

            BOOST_FOREACH(const office_element_ptr & elm, automaticStyles->styles_.number_styles_)
            {
                const number_style_base * style = dynamic_cast<const number_style_base *>(elm.get());

                if (!style)  continue;

                context_->numberStyles().add(style->get_style_name(), elm);
            }

        }
        while (0); // end parse automatic styles
    }
    while (false);
}

bool odf_document::Impl::docx_convert(oox::docx_conversion_context & Context)
{
 	if (bUserStopConvert !=0 ) return false;

	Context.process_styles();	
	if (UpdateProgress(450000)) return false;

    Context.process_fonts();
    Context.process_headers_footers();
	if (UpdateProgress(500000)) return false;

    Context.start_document();
	
	if (content_xml_)
        content_xml_->docx_convert(Context);

	if (UpdateProgress(800000)) return false;
    
	Context.end_document();

    // мы обрабатываем стили списка после того как сконвертировали документ,
    // так как в процессе конвертации документа у нас могу добавиться стили Ч
    // в случае если используется text:start-value (начинаем нумерацию заново)
    Context.process_list_styles();
	if (UpdateProgress(850000)) return false;

	return true;
}
bool odf_document::Impl::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
 	if (bUserStopConvert !=0 ) return false;

	try
    {
        _CP_LOG << L"[info] convert content" << std::endl;
       
		Context.start_document();
 		if (UpdateProgress(450000)) return false;
      
		if (content_xml_)
            content_xml_->xlsx_convert(Context);
		if (UpdateProgress(700000)) return false;

        Context.end_document();
		if (UpdateProgress(750000)) return false;

        _CP_LOG << L"[info] process styles" << std::endl;
        Context.process_styles();
 		if (UpdateProgress(800000)) return false;
      
    }
    catch(boost::exception & ex)
    {
        _CP_LOG << L"\n[error]:\n";
        _CP_LOG << utf8_to_utf16(ansi_to_utf8(boost::diagnostic_information(ex))) << std::endl;
        throw;
    }
    catch(std::exception & ex)
    {
        _CP_LOG << L"\n[error]:\n";
        _CP_LOG << utf8_to_utf16(ansi_to_utf8(ex.what())) << L"\n";
        throw;
    }
    catch(...)
    {
        _CP_LOG << L"\n[error]: undefined\n";
        throw;
    }

	return true;
}

bool odf_document::Impl::pptx_convert(oox::pptx_conversion_context & Context) 
{
	if (bUserStopConvert !=0 ) return false;

    try
    {
        _CP_LOG << L"[info] convert content" << std::endl;
	
		Context.start_document();
		if (UpdateProgress(450000)) return false;
       
		if (content_xml_)
            content_xml_->pptx_convert(Context);
		if (UpdateProgress(700000)) return false;
		
		Context.process_layouts();
		if (UpdateProgress(750000)) return false;
		
		Context.process_master_pages();
		if (UpdateProgress(800000)) return false;

        Context.end_document();
		if (UpdateProgress(850000)) return false;
	}
    catch(boost::exception & ex)
    {
        _CP_LOG << L"\n[error]:\n";
        _CP_LOG << utf8_to_utf16(ansi_to_utf8(boost::diagnostic_information(ex))) << std::endl;
        throw;
    }
    catch(std::exception & ex)
    {
        _CP_LOG << L"\n[error]:\n";
        _CP_LOG << utf8_to_utf16(ansi_to_utf8(ex.what())) << std::endl;
        throw;
    }
    catch(...)
    {
        _CP_LOG << L"\n[error]: undefined\n";
        throw;
    }
	return true;
}

odf_read_context & odf_document::Impl::odf_context() 
{ 
    return *context_.get(); 
}


const office_element * odf_document::Impl::get_content() const
{
    return content_xml_->get_content();
}

office_element * odf_document::Impl::get_content()
{
    return content_xml_->get_content();
}

}
}
