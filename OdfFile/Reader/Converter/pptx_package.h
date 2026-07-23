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
#pragma once

#include "../../Common/CPNoncopyable.h"

#include "oox_package.h"
#include "pptx_comments.h"

namespace NSFonts
{
    class IApplicationFonts;
}

namespace cpdoccore { 
namespace oox {

class pptx_xml_presentation;

class pptx_xml_theme;
typedef _CP_PTR(pptx_xml_theme) pptx_xml_theme_ptr;

class pptx_xml_authors_comments;
typedef _CP_PTR(pptx_xml_authors_comments) pptx_xml_authors_comments_ptr;

namespace package {

class pptx_content_types_file : public content_types_file
{
public:
    pptx_content_types_file();
};


//------------------------------------------------------------------------------
class slide_content;
typedef _CP_PTR(slide_content) slide_content_ptr;

class slide_content : noncopyable
{
public:
    slide_content();
    std::wstringstream & content() { return content_; }
    void			add_rel(relationship const & r);
	void			add_rels(rels & r);
    rels_file_ptr	get_rel_file() { return rels_; }
	std::wstring	str() { return content_.str(); }
    
	static _CP_PTR(slide_content) create();

private:
    std::wstringstream	content_;
    rels_file_ptr		rels_;
};
//------------------------------------------------------------------------------
class slides_files  : public element
{
public:
    slides_files();

    void add_slide(slide_content_ptr sheet);

    void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }

    virtual void write(const std::wstring & RootPath);
	void set_slide_ref(const std::wstring & ref_file_path,const std::wstring & ref_file_name);

    std::vector<slide_content_ptr> slides_;
    rels_files * rels_;

};
//------------------------------------------------------------------------------
class notes_files  : public slides_files
{
public:

    virtual void write(const std::wstring & RootPath);
};
//------------------------------------------------------------------------------
class slideLayouts_files  : public element
{
public:
    slideLayouts_files();

    void add_slide(slide_content_ptr sheet);

    virtual void write(const std::wstring & RootPath);

    std::vector<slide_content_ptr> slides_;
};
//------------------------------------------------------------------------------
class authors_comments_element : public element
{
public:
	authors_comments_element(pptx_xml_authors_comments_ptr & theme);
    virtual void write(const std::wstring & RootPath);

private:
    pptx_xml_authors_comments_ptr & authors_comments_;

};
//------------------------------------------------------------------------------
class slideMasters_files  : public element
{
public:
    slideMasters_files();

    void add_slide(slide_content_ptr sheet);

    void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }

    virtual void write(const std::wstring & RootPath);

    std::vector<slide_content_ptr> slides_;
    rels_files * rels_;
};
//------------------------------------------------------------------------------
class notesMaster_files  : public slideMasters_files
{
public:
    virtual void write(const std::wstring & RootPath);
};
//------------------------------------------------------------------------------
class ppt_comments_files;
typedef _CP_PTR(ppt_comments_files) ppt_comments_files_ptr;

class ppt_comments_files: public element
{
public:
    virtual void write(const std::wstring & RootPath);

    ppt_comments_files(const std::vector<pptx_comment_elm> & elms) : comments_ ( elms )
    {
    }

    static ppt_comments_files_ptr create(const std::vector<pptx_comment_elm> & elms);

private:
    const std::vector<pptx_comment_elm> & comments_;

};
//------------------------------------------------------------------------------
class ppt_charts_files  : public element
{
public:
	ppt_charts_files(){}

    void add_chart(chart_content_ptr chart);
	virtual void write(const std::wstring & RootPath);
    
    std::vector<chart_content_ptr> charts_;

};
//------------------------------------------------------------------------------

class ppt_themes_files  : public element
{
public:
	ppt_themes_files(){}

    void add_theme(pptx_xml_theme_ptr chart);
	virtual void write(const std::wstring & RootPath);
    
    std::vector<pptx_xml_theme_ptr> themes_;

};
//------------------------------------------------------------------------------
class ppt_files : public element
{
public:
    ppt_files();

    virtual void write(const std::wstring & RootPath);

    void set_presentation(pptx_xml_presentation & presentation);
    
	void set_styles(element_ptr Element);
	void set_comments(element_ptr Element);

	void add_slide		(slide_content_ptr sheet);
	void add_slideLayout(slide_content_ptr sheet);
	void add_slideMaster(slide_content_ptr sheet);
	void add_notes		(slide_content_ptr sheet);
	void add_notesMaster(slide_content_ptr sheet);
	
    void set_media(mediaitems_ptr & _mediaitems);
    void add_charts(chart_content_ptr chart);
    void add_theme (pptx_xml_theme_ptr theme);

	void set_authors_comments(pptx_xml_authors_comments_ptr & authors_comments);    
	
	void add_jsaProject	(const std::string &content);
private:
    rels_files				rels_files_;
   
	slides_files			slides_files_;    
	slideLayouts_files		slideLayouts_files_;    
	slideMasters_files		slideMasters_files_;
	notes_files				notes_files_;    
	notesMaster_files		notesMaster_files_;
	
	//slides_files		handoutMasters_files_; 	
	ppt_themes_files	themes_files_;

    ppt_charts_files	charts_files_;
	element_ptr			authors_comments_;	
	element_ptr			presentation_;
    element_ptr			tableStyles_;
	element_ptr			comments_;
    element_ptr			media_;
    element_ptr			embeddings_;
	element_ptr			jsaProject_;
};

//  xlsx_document
class pptx_document : public document
{
public:
    pptx_document();

    virtual void write(const std::wstring & RootPath);

    virtual content_types_file	& get_content_types_file()	{ return content_type_file_; }
    ppt_files					& get_ppt_files()			{ return ppt_files_; }

private:
	ppt_files ppt_files_;
	pptx_content_types_file content_type_file_; 
};

}
}
}
