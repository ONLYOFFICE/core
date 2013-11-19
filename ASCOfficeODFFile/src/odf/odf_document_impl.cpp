#include "precompiled_cpodf.h"
#include <cpdoccore/odf/odf_document.h>
#include "odf_document_impl.h"

#include <cpdoccore/CPString.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost_string.h>
#include <cpdoccore/xml/sax.h>
#include <cpdoccore/common/boost_filesystem_version.h>

#include "abstract_xml.h"
#include "odfcontext.h"
#include "office_body.h"
#include "office_document.h"
#include "office_elements.h"
#include "office_text.h"
#include "office_spreadsheet.h"
#include "office_chart.h"
#include "office_annotation.h"
#include "style_regions.h"
#include "paragraph_elements.h"
#include "text_elements.h"
#include "office_scripts.h"
#include "table_calculation_settings.h"
#include "number_style.h"
#include "list.h"
#include "font_face.h"
#include "table.h"
#include "styles.h"
#include "draw_common.h"

#include <cpdoccore/common/readdocelement.h>
#include <iostream>
#include "documentcontext.h"


namespace cpdoccore { 
namespace odf {

namespace fs = ::boost::filesystem;

namespace {
content_xml_t_ptr read_file_content(const fs::wpath & Path)
{
    fs::ifstream content_file;
    content_file.open(Path);
    if (content_file)
    {
        xml::sax_ptr sax = xml::create_sax( content_file );
        content_xml_t_ptr result( new content_xml_t() );
        result->read_sax(sax.get());
        return result;
    }
    else
    {
        _CP_LOG(info) << L"[warning] open file error (" <<  BOOST_STRING_PATH(Path) << L")\n";
    }
    return content_xml_t_ptr();
}
}

odf_document::Impl::Impl(const std::wstring & Folder) : context_(new odf_read_context()), base_folder_(Folder)
{
	office_mime_type_ = 0;

    fs::wpath folderPath(Folder);

    fs::wpath content_xml = folderPath / L"content.xml";
    fs::wpath styles_xml = folderPath / L"styles.xml";
    fs::wpath meta_xml = folderPath / L"meta.xml";
    fs::wpath settings_xml = folderPath / L"settings.xml";
	fs::wpath manifest_xml = folderPath / L"META-INF" / L"manifest.xml";

    _CP_LOG(info) << L"[info] read content.xml" << std::endl;
    manifest_xml_ = read_file_content(manifest_xml);

	_CP_LOG(info) << L"[info] read content.xml" << std::endl;
    content_xml_ = read_file_content(content_xml);

	  _CP_LOG(info) << L"[info] read styles.xml" << std::endl;
    styles_xml_ = read_file_content(styles_xml);

    _CP_LOG(info) << L"[info] parse fonts" << std::endl;
    parse_fonts();

    _CP_LOG(info) << L"[info] parse styles" << std::endl;
    parse_styles();

    _CP_LOG(info) << L"[info] parse manifest" << std::endl;
    parse_manifests();

}

void odf_document::Impl::parse_fonts()
{
    do 
    {
        if (!content_xml_)
        {
            _CP_LOG(info) << L"[warning] empty content xml\n";
            break;
        }

        office_document_base * document = dynamic_cast<office_document_base *>( content_xml_->get_content() );
        if (!document)
        {
            _CP_LOG(info) << L"[warning] empty document\n";
            break;
        }

        office_font_face_decls* fontFaceDecls = dynamic_cast<office_font_face_decls*>( document->office_font_face_decls_.get() );
        
        if (!fontFaceDecls)
        {
            _CP_LOG(info) << L"[warning] empty font face decls\n";
            break;
        }

        BOOST_FOREACH(office_element_ptr & elm, fontFaceDecls->style_font_face_)
        {
            style_font_face* fontFace = dynamic_cast<style_font_face*>( elm.get() );
            if (!fontFace)
            {
                _CP_LOG(info) << L"[warning] read font face error\n";
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
                _CP_LOG(info) << L"[warning] duplicate font name (" << fontName << L")\n";
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

		res = entry->media_type_.find(L"application/vnd.oasis.opendocument.text");
		if (res>=0)
		{
			office_mime_type_ = 1;
			break;
		}
		res = entry->media_type_.find(L"application/vnd.oasis.opendocument.spreadsheet");
		if (res>=0)
		{
			office_mime_type_ = 2;
			break;
		}
		res = entry->media_type_.find(L"application/vnd.oasis.opendocument.presentation");
		if (res>=0)
		{
			office_mime_type_ = 3;
			break;
		}
	}
}

void odf_document::Impl::parse_styles()
{
    do
    {
        if (!styles_xml_)
        {
            _CP_LOG(info) << L"[warning] empty styles xml\n";
            break;
        }

        office_document_base * document = dynamic_cast<office_document_base *>( styles_xml_->get_content() );
        if (!document)
        {
            _CP_LOG(info) << L"[warning] empty document\n";
            break;
        }
       
        // parse automatic styles - эти стили используют объекты которые в оазис находятся в этом же документе
		//переопределяем имя - иначе при поиске может возникнуть коллизия.
        do
        {
            office_automatic_styles * automaticStyles = dynamic_cast<office_automatic_styles *>( document->office_automatic_styles_.get() );
            if (!automaticStyles)
            {
                _CP_LOG(info) << L"[warning] error reading automatic styles\n";
                break;
            }

            // parse page layout
            BOOST_FOREACH(office_element_ptr & elm, automaticStyles->style_page_layout_)
            {
                style_page_layout * pageLayout = dynamic_cast<style_page_layout *>(elm.get());
                if (!pageLayout)
                {
                    _CP_LOG(info) << L"[warning] error reading page layout\n";
                    continue;
                }

                context_->pageLayoutContainer().add_page_layout(pageLayout);
            } // end parse page layout

            BOOST_FOREACH(office_element_ptr & elm, automaticStyles->styles_.style_style_)
            {
                style * styleInst = dynamic_cast<style*>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG(info) << L"[warning] error reading style\n";
                    continue;
                }

				context_->styleContainer().add_style(L"common:" + styleInst->style_name_,
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
                    _CP_LOG(info) << L"[warning] error list style\n";
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
                _CP_LOG(info) << L"[warning] error reading master styles\n";
                break;
            }

            BOOST_FOREACH(office_element_ptr & elm, masterStyles->style_master_page_)
            {
                style_master_page * masterPage = dynamic_cast<style_master_page *>(elm.get());
                if (!masterPage)
                {
                    _CP_LOG(info) << L"[warning] error reading master page\n";
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
                _CP_LOG(info) << L"[warning] error reading styles\n";
                break;
            }

            // default styles
            BOOST_FOREACH(office_element_ptr & elm, docStyles->style_default_style_)
            {
                default_style * styleInst = dynamic_cast<default_style *>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG(info) << L"[warning] error reading default style\n";
                    continue;
                }

                context_->styleContainer().add_style(L"",
                    styleInst->style_family_.get_type(), 
                    &(styleInst->style_content_),
                    false,
                    true,
                    L"",
                    L"",
                    L"");                                            
            }

            // common styles
            BOOST_FOREACH(office_element_ptr & elm, docStyles->styles_.style_style_)
            {
                style * styleInst = dynamic_cast<style*>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG(info) << L"[warning] error reading style\n";
                    continue;
                }

                if (styleInst->style_master_page_name_ && !styleInst->style_master_page_name_->empty())
                    context_->styleContainer().add_master_page_name(styleInst->style_name_, *styleInst->style_master_page_name_);

                context_->styleContainer().add_style(styleInst->style_name_,
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
                    _CP_LOG(info) << L"[warning] error list style\n";
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
        }
        while(0); // end parse styles
    }
    while (0);

    do
    {
        if (!content_xml_)
        {
            _CP_LOG(info) << L"[warning] empty content xml\n";
            break;
        }

        office_document_base * document = dynamic_cast<office_document_base *>( content_xml_->get_content() );
        if (!document)
        {
            _CP_LOG(info) << L"[warning] empty document\n";
            break;
        }

        // parse automatic styles
        do 
        {
            office_automatic_styles * automaticStyles = dynamic_cast<office_automatic_styles *>(document->office_automatic_styles_.get());
            if (!automaticStyles)
            {
                _CP_LOG(info) << L"[warning] error reading automatic styles\n";
                break;
            }

            BOOST_FOREACH(office_element_ptr & elm, automaticStyles->styles_.style_style_)
            {
                style * styleInst = dynamic_cast<style*>(elm.get());
                if (!styleInst)
                {
                    _CP_LOG(info) << L"[warning] error reading style\n";
                    continue;
                }

                if (styleInst->style_master_page_name_ && !styleInst->style_master_page_name_->empty())
                    context_->styleContainer().add_master_page_name(styleInst->style_name_, *styleInst->style_master_page_name_);

                context_->styleContainer().add_style(styleInst->style_name_,
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
                    _CP_LOG(info) << L"[warning] error reading list style\n";
                    continue;
                }

                context_->listStyleContainer().add_list_style(listStyle);
            }

            BOOST_FOREACH(const office_element_ptr & elm, automaticStyles->styles_.number_styles_)
            {
                const number_style_base * style = dynamic_cast<const number_style_base *>(elm.get());

                if (!style)
                    continue;

                context_->numberStyles().add(style->get_style_name(), elm);
            }

        }
        while (0); // end parse automatic styles
    }
    while (false);
}

void odf_document::Impl::docx_convert(oox::docx_conversion_context & Context)
{
    Context.process_styles();
    Context.process_fonts();
    Context.process_headers_footers();

    Context.start_document();
	
	if (content_xml_)
        content_xml_->docx_convert(Context);
    
	Context.end_document();

    // мы обрабатываем стили списка после того как сконвертировали документ,
    // так как в процессе конвертации документа у нас могу добавиться стили — 
    // в случае если используется text:start-value (начинаем нумерацию заново)
    Context.process_list_styles();
}

void odf_document::Impl::xlsx_convert(oox::xlsx_conversion_context & Context) 
{
    try
    {
        _CP_LOG(info) << L"[info] convert content" << std::endl;
        Context.start_document();
        if (content_xml_)
            content_xml_->xlsx_convert(Context);
        Context.end_document();

        _CP_LOG(info) << L"[info] process styles" << std::endl;
        Context.process_styles();
       
    }
    catch(boost::exception & ex)
    {
        _CP_LOG(info) << L"\n[error]:\n";
        _CP_LOG(info) << utf8_to_utf16(ansi_to_utf8(boost::diagnostic_information(ex))) << std::endl;
        throw;
    }
    catch(std::exception & ex)
    {
        _CP_LOG(info) << L"\n[error]:\n";
        _CP_LOG(info) << utf8_to_utf16(ansi_to_utf8(ex.what())) << std::endl;
        throw;
    }
    catch(...)
    {
        _CP_LOG(info) << L"\n[error]: undefined\n";
        throw;
    }
}

odf_read_context & odf_document::Impl::odf_context() 
{ 
    return *context_.get(); 
}


const office_element * odf_document::Impl::get_content() const
{
    return content_xml_->get_content();
}

}
}