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
#pragma once

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include "pptx_text_context.h"
#include "pptx_slide_context.h"

#include "pptx_output_xml.h"
#include "pptx_comments_context.h"
#include "oox_chart_context.h"
#include "pptx_table_context.h"

#include "mediaitems.h"

namespace NSFonts
{
    class IApplicationFonts;
}

namespace cpdoccore {

namespace odf_reader 
{
    class odf_document;
    class office_element;
}

namespace oox {

namespace package
{
    class pptx_document;
}

class pptx_conversion_context : boost::noncopyable
{
public:
    pptx_conversion_context(odf_reader::odf_document * odfDocument);
	~pptx_conversion_context();

	void set_output_document(package::pptx_document * document);
    void set_font_directory	(std::wstring pathFonts);

	void start_document();
    void end_document();

    void start_chart(std::wstring name);
    void end_chart();
	
	void start_body();
    void end_body();

    void start_office_presentation();
    void end_office_presentation();

    bool start_page(const std::wstring & pageName, 
							const std::wstring & pageStyleName,
							const std::wstring & pageLayoutName,
							const std::wstring & pageMasterName);
	void end_page();
	
    bool start_page_notes();
	void end_page_notes();

    bool start_master_notes();
	void end_master_notes();

	bool start_layout(	int layout_index);
	void end_layout();

	bool start_master(int master_index);
 	void end_master();
   
	void start_theme(std::wstring & name);
	void end_theme();

	void add_jsaProject(const std::string &content);

	std::pair<int,int> add_author_comments(std::wstring author);

	pptx_slide_context & get_slide_context() { return pptx_slide_context_; }
	pptx_comments_context & get_comments_context() { return pptx_comments_context_; }

    odf_reader::odf_document * root()
    {
        return odf_document_;
    }

    pptx_xml_slide					& current_slide();
	pptx_xml_slideLayout			& current_layout();
	pptx_xml_slideMaster			& current_master();
    pptx_xml_theme					& current_theme();
	pptx_xml_slideNotes				& current_notes();
	pptx_xml_slideNotesMaster		& current_notesMaster();

    pptx_xml_presentation	& current_presentation();//собственно она одна
  
	oox_chart_context	& current_chart();
	math_context		& get_math_context()	{ return math_context_; }
	pptx_text_context	& get_text_context()	{ return pptx_text_context_; }

	pptx_table_context	& get_table_context()	{ return pptx_table_context_; }

    mediaitems_ptr & get_mediaitems() { return pptx_slide_context_.get_mediaitems(); }

    //void start_hyperlink(const std::wstring & styleName);
    //void end_hyperlink(std::wstring const & href);
	bool process_masters_;

	void process_layouts();
	void process_styles();
	
	void process_master_pages();
	void process_theme(std::wstring name);

	int last_idx_placeHolder;
private:
	int		last_uniq_big_id;

    void	create_new_slide(std::wstring const & name);
	void	create_new_slideLayout(int id);
	void	create_new_slideMaster(int id);
    void	create_new_slideNotes();
    void	create_new_slideNotesMaster();

	package::pptx_document		* output_document_; 
    odf_reader::odf_document	* odf_document_;    

    pptx_slide_context		pptx_slide_context_;
	pptx_text_context		pptx_text_context_;
	pptx_table_context		pptx_table_context_;
	pptx_comments_context	pptx_comments_context_;
	math_context			math_context_;
	
	std::vector<oox_chart_context_ptr> charts_;
	
	std::vector<pptx_xml_slide_ptr>			slides_;
	std::vector<pptx_xml_slideNotes_ptr>	notes_;
	std::vector<pptx_xml_slideMaster_ptr>	slideMasters_;
	std::vector<pptx_xml_slideLayout_ptr>	slideLayouts_;
	std::vector<pptx_xml_theme_ptr>			themes_;
	
	pptx_xml_slideNotesMaster_ptr			slideNotesMaster_;
	pptx_xml_authors_comments_ptr			authors_comments_;
	pptx_xml_presentation					presentation_;

	std::wstring current_master_page_name_;
	std::wstring current_layout_page_name_;

    pptx_comments_context_handle comments_context_handle_;


};

}
}
