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

#include "pptx_conversion_context.h"

#include "pptx_package.h"

#include <xml/simple_xml_writer.h>

#include <iostream>

#include "../Format/odf_document.h"
#include "../Format/calcs_styles.h"
#include "../Format/odfcontext.h"
#include "../Format/draw_frame.h"

#include "pptx_default_serializes.h"

namespace cpdoccore { 

namespace odf_reader 
{
    class odf_document;
}

namespace oox {
    
namespace package
{
    class pptx_document;
}

pptx_conversion_context::pptx_conversion_context( odf_reader::odf_document * odfDocument)
	:output_document_		(NULL)
	,odf_document_			(odfDocument)
	,pptx_text_context_		(odf_document_->odf_context(), *this)
	,pptx_table_context_	(*this)
	,pptx_comments_context_	(comments_context_handle_)
	,pptx_slide_context_	(*this/*, pptx_text_context_*/)
	,math_context_			(odf_document_->odf_context().fontContainer(), true)
	,last_idx_placeHolder	(1)
	,last_uniq_big_id		(1)
{
}

pptx_conversion_context::~pptx_conversion_context()
{
}

void pptx_conversion_context::set_output_document(package::pptx_document * document)
{
	output_document_ = document;
}

void pptx_conversion_context::set_font_directory(std::wstring pathFonts)
{
	pptx_slide_context_.get_mediaitems()->set_font_directory(pathFonts);
}

void pptx_conversion_context::process_layouts()
{
	odf_reader::presentation_layouts_instance & layouts = root()->odf_context().styleContainer().presentation_layouts();

	get_text_context().set_process_layouts(true);

	//актуальные
	for (size_t layout_index =0; layout_index < layouts.content.size(); layout_index++)
	{
		start_layout(layout_index);

		odf_reader::style_presentation_page_layout * layout = 
			root()->odf_context().pageLayoutContainer().presentation_page_layout_by_name(layouts.content[layout_index].layout_name);

		if (layout)
		{
			layout->pptx_convert(*this);
		}
		//нужно вытащить footers 
		odf_reader::style_master_page * master = 
			root()->odf_context().pageLayoutContainer().master_page_by_name(layouts.content[layout_index].master_name);

		if (master)
		{
			for (size_t i = 0; i < master->content_.size(); i++)			
			{
				odf_reader::draw_frame* frame = dynamic_cast<odf_reader::draw_frame*>(master->content_[i].get());
				if (frame)
				{
					odf_types::common_presentation_attlist	&common_presentation_attlist_= frame->common_draw_attlists_.shape_with_text_and_styles_.common_presentation_attlist_;
					
					if (common_presentation_attlist_.presentation_class_)
					{
						odf_types::presentation_class::type type = common_presentation_attlist_.presentation_class_->get_type();

						if (type == odf_types::presentation_class::footer		||
							type == odf_types::presentation_class::date_time	||
							type == odf_types::presentation_class::header		||
							type == odf_types::presentation_class::page_number)
						{
							if (frame->idx_in_owner < 0)
								frame->idx_in_owner = last_idx_placeHolder++;

							frame->pptx_convert_placeHolder(*this);
						}
					}
				}
			}
		}
		end_layout();	
	}
	get_text_context().set_process_layouts(false);
}
void pptx_conversion_context::process_master_pages()
{
	odf_reader::presentation_masters_instance & masters = root()->odf_context().styleContainer().presentation_masters();

	process_masters_ = true;
	get_text_context().set_process_layouts(true);

	//берем только актуальные
	odf_reader::office_element_ptr master_notes_;
	
	for (size_t master_index = 0; master_index < masters.content.size(); master_index++)
	{
		start_master(master_index);
		
		odf_reader::style_master_page * master = 
			root()->odf_context().pageLayoutContainer().master_page_by_name(masters.content[master_index].master_name);

		if (master)
		{
			master->pptx_convert(*this);
		
			if (!master_notes_ && master->presentation_notes_)
				master_notes_ = master->presentation_notes_;
		}

		
		end_master();	
	}

	if (master_notes_)
	{
		start_master_notes();
			master_notes_->pptx_convert(*this);
		end_master_notes();
	}
	process_masters_ = false;
	get_text_context().set_process_layouts(false);

}

void pptx_conversion_context::process_styles()
{
 
}
void pptx_conversion_context::process_theme(std::wstring  name)
{
	int current = themes_.size() + 1;

	if (name.empty())
	{
		name = L"User Theme: " + std::to_wstring(current);
	}
 	start_theme(name);
	//
		pptx_serialize_clrScheme	(current_theme().clrSchemeData());
		pptx_serialize_fmtScheme	(current_theme().fmtSchemeData());
		pptx_serialize_fontScheme	(current_theme().fontSchemeData());
	//
	end_theme();

}
void pptx_conversion_context::start_document()
{
    odf_reader::odf_read_context & odfContext = root()->odf_context();
    std::vector<const odf_reader::style_instance *> instances;
    
	instances.push_back(odfContext.styleContainer().style_default_by_type(odf_types::style_family::Presentation));
	instances.push_back(odfContext.styleContainer().style_by_name(L"Default",odf_types::style_family::Presentation,false));

    odf_reader::text_format_properties_content_ptr	textFormatProperties	= calc_text_properties_content(instances);
    odf_reader::paragraph_format_properties			parFormatProperties		= calc_paragraph_properties_content(instances);

	process_masters_ = false;
}

void pptx_conversion_context::end_document()
{
	for (size_t i = 0; i < slideMasters_.size(); i++)
    {
		pptx_xml_slideMaster_ptr& slideM = slideMasters_[i];

        package::slide_content_ptr content = package::slide_content::create();

		slideM->write_to(content->content());
        content->add_rels(slideM->Rels());//media & links rels

        output_document_->get_ppt_files().add_slideMaster(content);//slideMaster.xml

        CP_XML_WRITER(presentation_.slideMastersData())//presentation.xml
        {
            CP_XML_NODE(L"p:sldMasterId")
            {
                CP_XML_ATTR(L"id", 0x80000000 + last_uniq_big_id++); 
                CP_XML_ATTR(L"r:id", slideM->rId());            
            }
        }
	}
	if (!slideMasters_.empty())
		presentation_.slidesProperties() << slideMasters_[0]->Sizes().str();

////////////////////////////////////////////////////////////////////////////////////////////////////
	for (size_t i = 0; i < slides_.size(); i++)
    {
		pptx_xml_slide_ptr& slide = slides_[i];

        package::slide_content_ptr content = package::slide_content::create();

		slide->write_to(content->content());
        content->add_rels(slide->Rels());//media & links rels

        output_document_->get_ppt_files().add_slide(content);//slide.xml

        CP_XML_WRITER(presentation_.slidesData())//presentation.xml
        {
            CP_XML_NODE(L"p:sldId")
            {
                CP_XML_ATTR(L"id", 0x100 + i); 
                CP_XML_ATTR(L"r:id", slide->rId());            
            }
        }
    }
//----------------------------------------------------------------------------------
	for (size_t i = 0; i < slideLayouts_.size(); i++)
    {
		pptx_xml_slideLayout_ptr& slideL = slideLayouts_[i];

        package::slide_content_ptr content = package::slide_content::create();

		slideL->write_to(content->content());
        content->add_rels(slideL->Rels());//media & links rels

        output_document_->get_ppt_files().add_slideLayout(content);//slideMaster.xml
	}
//----------------------------------------------------------------------------------
	for (size_t i = 0; i < notes_.size(); i++)
    {
		pptx_xml_slideNotes_ptr& slideN = notes_[i];

        package::slide_content_ptr content = package::slide_content::create();

		slideN->write_to(content->content());
        content->add_rels(slideN->Rels());//media & links rels

        output_document_->get_ppt_files().add_notes(content);
	}
	if (slideNotesMaster_)
	{
		package::slide_content_ptr content = package::slide_content::create();

		slideNotesMaster_->write_to(content->content());
        content->add_rels(slideNotesMaster_->Rels());//media & links rels

        output_document_->get_ppt_files().add_notesMaster(content);
		
		CP_XML_WRITER(presentation_.slideNotesMastersData())//presentation.xml
		{
			CP_XML_NODE(L"p:notesMasterId")
			{
				CP_XML_ATTR(L"r:id", slideNotesMaster_->rId());            
			}
		}
	}
	//else
		pptx_serialize_size(current_presentation().slidesNotesProperties(), 6858000, 9144000, L"p:notesSz");

////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (size_t i = 0; i < charts_.size(); i++)
    {
		package::chart_content_ptr content = package::chart_content::create();

		charts_[i]->serialize(content->content());
		charts_[i]->dump_rels(content->get_rel_file()->get_rels());

		output_document_->get_ppt_files().add_charts(content);
	
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (size_t i=0; i < themes_.size(); i++)
    {
		output_document_->get_ppt_files().add_theme(themes_[i]);
	
	}
    package::ppt_comments_files_ptr comments = package::ppt_comments_files::create(comments_context_handle_.content());
    
	output_document_->get_ppt_files().set_presentation (presentation_);       
	output_document_->get_ppt_files().set_comments (comments);
	output_document_->get_ppt_files().set_authors_comments (authors_comments_);
	output_document_->get_ppt_files().set_media (get_mediaitems());

	output_document_->get_content_types_file().set_media(get_mediaitems());
}

void pptx_conversion_context::start_body()
{}

void pptx_conversion_context::end_body()
{}
pptx_xml_slideNotesMaster & pptx_conversion_context::current_notesMaster()
{
    if (slideNotesMaster_)
    {
        return *slideNotesMaster_;
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
pptx_xml_slideNotes & pptx_conversion_context::current_notes()
{
    if (!notes_.empty())
    {
        return *notes_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
pptx_xml_slide & pptx_conversion_context::current_slide()
{
    if (!slides_.empty())
    {
        return *slides_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
pptx_xml_slideLayout & pptx_conversion_context::current_layout()
{
    if (!slideLayouts_.empty())
    {
        return *slideLayouts_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
pptx_xml_theme & pptx_conversion_context::current_theme()
{
    if (!themes_.empty())
    {
        return *themes_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
pptx_xml_presentation & pptx_conversion_context::current_presentation()
{
	return presentation_;
}

oox_chart_context & pptx_conversion_context::current_chart()
{
    if (!charts_.empty())
    {
        return *charts_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
pptx_xml_slideMaster & pptx_conversion_context::current_master()
{
    if (!slideMasters_.empty())
    {
        return *slideMasters_.back().get();
    }
    else
    {
        throw std::runtime_error("internal error");
    }
}
void pptx_conversion_context::create_new_slide(std::wstring const & name)
{
	pptx_xml_slide_ptr s = pptx_xml_slide::create(name,slides_.size() + 1);
    slides_.push_back(s);
}
void pptx_conversion_context::create_new_slideNotes()
{
	pptx_xml_slideNotes_ptr s = pptx_xml_slideNotes::create( notes_.size() + 1);
    notes_.push_back(s);
}
void pptx_conversion_context::create_new_slideNotesMaster()
{
	slideNotesMaster_ = pptx_xml_slideNotesMaster::create();
}
void pptx_conversion_context::create_new_slideLayout(int id)
{
	pptx_xml_slideLayout_ptr s = pptx_xml_slideLayout::create(id);
    slideLayouts_.push_back(s);
}
void pptx_conversion_context::create_new_slideMaster(int id)
{
	pptx_xml_slideMaster_ptr s = pptx_xml_slideMaster::create(id);
    slideMasters_.push_back(s);
}
bool pptx_conversion_context::start_page(const std::wstring & pageName,	const std::wstring & pageStyleName,
																		const std::wstring & pageLayoutName,
																		const std::wstring & pageMasterName)
{
    create_new_slide(pageName);
	get_slide_context().start_slide();//pageName, pageStyleName);

	current_master_page_name_ = pageMasterName;
	current_layout_page_name_ = pageLayoutName;
	
	//const std::wstring masterPageNameLayout = root()->odf_context().pageLayoutContainer().page_layout_name_by_style(current_master_page_name_);

	std::pair<int,std::wstring> layout_id =
					root()->odf_context().styleContainer().presentation_layouts().add_or_find(pageLayoutName,pageMasterName);

	current_slide().Rels().add(relationship(layout_id.second, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout",
		std::wstring(L"../slideLayouts/slideLayout")  + std::to_wstring(layout_id.first) + L".xml"));

    return true;
}

bool pptx_conversion_context::start_layout(int layout_index)
{
	if  (layout_index >=0)
	{
		odf_reader::presentation_layouts_instance & layouts = root()->odf_context().styleContainer().presentation_layouts();

		create_new_slideLayout(layouts.content[layout_index].Id);
		
		get_slide_context().start_slide();//layouts.content[layout_index].layout_name, L"");//?????

		current_master_page_name_ = layouts.content[layout_index].master_name;
		current_layout_page_name_ = L"";
		
		std::pair<int,std::wstring> master_id = //std::pair<int,std::wstring>(1,L"smId1");
				root()->odf_context().styleContainer().presentation_masters().add_or_find(layouts.content[layout_index].master_name);

		root()->odf_context().styleContainer().presentation_masters().add_layout_to(layouts.content[layout_index].master_name,layouts.content[layout_index]);

		current_layout().Rels().add(relationship(L"smId1"/*master_id.second*/, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster",
			std::wstring(L"../slideMasters/slideMaster")  + std::to_wstring(master_id.first) + L".xml"));

		//
	}
	else//общий шаблон (насильно пропишем к темам несоответствующие шалоны)
	{
	}

//layout type

//<xsd:enumeration value="title"/>
//1375 <xsd:enumeration value="tx"/>
//1376 <xsd:enumeration value="twoColTx"/>
//1377 <xsd:enumeration value="tbl"/>
//1378 <xsd:enumeration value="txAndChart"/>
//1379 <xsd:enumeration value="chartAndTx"/>
//1380 <xsd:enumeration value="dgm"/>
//1381 <xsd:enumeration value="chart"/>
//1382 <xsd:enumeration value="txAndClipArt"/>
//1383 <xsd:enumeration value="clipArtAndTx"/>
//1384 <xsd:enumeration value="titleOnly"/>
//1385 <xsd:enumeration value="blank"/>
//1386 <xsd:enumeration value="txAndObj"/>
//1387 <xsd:enumeration value="objAndTx"/>
//1388 <xsd:enumeration value="objOnly"/>
//1389 <xsd:enumeration value="obj"/>
//1390 <xsd:enumeration value="txAndMedia"/>
//1391 <xsd:enumeration value="mediaAndTx"/>
//1392 <xsd:enumeration value="objOverTx"/>
//1393 <xsd:enumeration value="txOverObj"/>
//1394 <xsd:enumeration value="txAndTwoObj"/>
//1395 <xsd:enumeration value="twoObjAndTx"/>
//1396 <xsd:enumeration value="twoObjOverTx"/>
//1397 <xsd:enumeration value="fourObj"/>
//1398 <xsd:enumeration value="vertTx"/>
//1399 <xsd:enumeration value="clipArtAndVertTx"/>
//1400 <xsd:enumeration value="vertTitleAndTx"/>
//1401 <xsd:enumeration value="vertTitleAndTxOverChart"/>
//1402 <xsd:enumeration value="twoObj"/>
//1403 <xsd:enumeration value="objAndTwoObj"/>
//1404 <xsd:enumeration value="twoObjAndObj"/>
//1405 <xsd:enumeration value="cust"/>---------------------------------- !!!!!!!!!!!!!
//1406 <xsd:enumeration value="secHead"/>
//1407 <xsd:enumeration value="twoTxTwoObj"/>
//1408 <xsd:enumeration value="objTx"/>
//1409 <xsd:enumeration value="picTx"/>
 
	return true;
}
bool pptx_conversion_context::start_master(int master_index)
{
	odf_reader::presentation_masters_instance & masters = root()->odf_context().styleContainer().presentation_masters();

	create_new_slideMaster(masters.content[master_index].Id);
	
	get_slide_context().start_slide();

	current_master_page_name_ = L"";
	current_layout_page_name_ = L"";
	
	process_theme(masters.content[master_index].master_name);//add default theme - одинаковые но под разными именами
	current_master().add_theme(current_theme().id(), L"tId1");	

	for (size_t i = 0; i < masters.content[master_index].layouts.size(); i++)
	{
		current_master().add_layout(masters.content[master_index].layouts[i].Id, masters.content[master_index].layouts[i].rId, 0x80000000 + last_uniq_big_id++);
	}

//----------------------------------------------------------------------------------
//размеры страниц в презентации
	const std::wstring pageProperties = root()->odf_context().pageLayoutContainer().page_layout_name_by_style(masters.content[master_index].master_name);

	odf_reader::page_layout_instance *pages_layouts = root()->odf_context().pageLayoutContainer().page_layout_by_name(pageProperties);
	
	if (pages_layouts)
		pages_layouts->pptx_serialize(current_master().Sizes(), *this);

	return true;
}
void pptx_conversion_context::end_page()
{
	if (!get_comments_context().empty())
    {
        std::wstringstream strm;
        get_comments_context().serialize(strm);        
		
		const std::pair<std::wstring, std::wstring> commentsName =
            comments_context_handle_.add_comments_xml(strm.str(), get_comments_context().get_comments() );

		get_slide_context().add_rels(false, commentsName.second, L"../comments/" + commentsName.first, typeComment);
    } 

	get_slide_context().serialize_background(current_slide().Background());
	get_slide_context().serialize_objects	(current_slide().Data());
	get_slide_context().serialize_animations(current_slide().Timing());
	
	get_slide_context().dump_rels(current_slide().Rels());//hyperlinks, mediaitems, ...

	get_slide_context().end_slide();
}
bool pptx_conversion_context::start_page_notes()
{
	create_new_slideNotes( );

	current_slide().Rels().add(relationship(notes_.back()->rId(), L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide",
								L"../notesSlides/notesSlide"  + std::to_wstring(notes_.size()) + L".xml"));

	get_slide_context().start_slide();
	
	current_notes().Rels().add(relationship(L"nId1", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide",
								L"../slides/slide"  + std::to_wstring(slides_.size()) + L".xml"));
	
	return true;
}

void pptx_conversion_context::end_page_notes()
{
	get_slide_context().serialize_background(current_notes().Background());
	get_slide_context().serialize_objects(current_notes().Data());

	get_slide_context().dump_rels(current_notes().Rels());//hyperlinks, mediaitems, ...

	get_slide_context().end_slide();
}
bool pptx_conversion_context::start_master_notes()
{
	create_new_slideNotesMaster( );
	
	get_slide_context().start_slide();

	process_theme(L"");//add default theme - одинаковые но под разными именами
	current_notesMaster().add_theme(current_theme().id(), L"tId1");	

	get_slide_context().start_slide();
	return true;
}

void pptx_conversion_context::end_master_notes()
{
	get_slide_context().serialize_background(current_notesMaster().Background());
	get_slide_context().serialize_objects(current_notesMaster().Data());

	get_slide_context().dump_rels(current_notesMaster().Rels());//hyperlinks, mediaitems, ...

	get_slide_context().end_slide();
	
	for (size_t i = 0; i < notes_.size(); i++)
	{
		notes_[i]->Rels().add(relationship(L"nmId1", 
			L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster",
			L"../notesMasters/notesMaster1.xml"));
	}
}
void pptx_conversion_context::end_layout()
{
	get_slide_context().serialize_objects(current_layout().Data());
	
	get_slide_context().dump_rels(current_layout().Rels());//hyperlinks, mediaitems, ...

	get_slide_context().end_slide();
}

std::pair<int,int> pptx_conversion_context::add_author_comments(std::wstring author)
{
	if (!authors_comments_)
	{
		authors_comments_ = pptx_xml_authors_comments::create();
		if (!authors_comments_)return std::pair<int,int>(-1,-1);
	}

	return authors_comments_->add_or_find(author);
}

void pptx_conversion_context::end_master()
{
	get_slide_context().serialize_background	(current_master().Background(),true);	
	get_slide_context().serialize_objects		(current_master().Data());
	get_slide_context().serialize_HeaderFooter	(current_master().DataExtra());
	
	get_slide_context().dump_rels(current_master().Rels());//hyperlinks, mediaitems, ...

	get_slide_context().end_slide();
}
void pptx_conversion_context::start_theme(std::wstring & name)
{
	themes_.push_back(pptx_xml_theme::create(name,themes_.size()+1));
}
void pptx_conversion_context::end_theme()
{
}
void pptx_conversion_context::start_office_presentation()
{
}

void pptx_conversion_context::end_office_presentation()
{
}
void pptx_conversion_context::start_chart(std::wstring name)
{
	charts_.push_back(oox_chart_context_ptr(new oox_chart_context(get_mediaitems(), name)));
	//добавляем новую форму для диаграммы
	 //в ней будет информационная часть - и она пишется каждый раз в свою xml (их - по числу диаграмм)
	//этот контекст нужно передавать в файл

}
void pptx_conversion_context::end_chart()
{
	//current_chart().set_drawing_link(current_sheet().get_drawing_link());
	//излишняя инфа
}
void pptx_conversion_context::add_jsaProject(const std::string &content)
{
	if (content.empty()) return;
	
	output_document_->get_ppt_files().add_jsaProject(content);
	output_document_->get_content_types_file().add_or_find_default(L"bin");
}
}
}
