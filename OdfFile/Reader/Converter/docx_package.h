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

#include <string>
#include <vector>

#include "oox_rels.h"
#include "docx_content_type.h"
#include "oox_package.h"

namespace cpdoccore { 
namespace oox {
    
class notes_context;
class docx_conversion_context;
class mediaitems;
class headers_footers;
class comments_context;


namespace package {

class element;
typedef shared_ptr<element>::Type element_ptr;
typedef std::vector<element_ptr> element_ptr_array;

class document;

////////////////////////////////////////////////////////////////////////////////////////////////

class docx_content_types_file : public content_types_file
{
public:
    docx_content_types_file();
};

////////////////////////////////////////////////////////////////////////////////////////////////

class headers_footers_elements : public element
{
public:
    headers_footers_elements(headers_footers & HeadersFooters);
    virtual void write(const std::wstring & RootPath);

private:
    headers_footers & headers_footers_;

};

class notes_elements : public element
{
public:
    notes_elements(notes_context & notesContext);
    virtual void write(const std::wstring & RootPath);

private:
    notes_context & notes_context_;

};

class comments_elements : public element
{
public:
    comments_elements(comments_context & commentsContext);
    virtual void write(const std::wstring & RootPath);

private:
    comments_context & comments_context_;

};

class customXml_files : public element
{
public:
	customXml_files(){}

    int add_customXml(customXml_content_ptr customXml);
    virtual void write(const std::wstring & RootPath);
   
    std::vector<customXml_content_ptr> customXmls_;
};
class docx_charts_files : public element
{
public:
	docx_charts_files(){}

    void add_chart(chart_content_ptr chart);
    virtual void write(const std::wstring & RootPath);
   
    std::vector<chart_content_ptr> charts_;
};
////////////////////////////////////////////////
class word_files : public element
{
public:
    word_files();

public:
    virtual void write(const std::wstring & RootPath);
    
	void set_styles		(element_ptr Element);
    void set_document	(element_ptr Element);
    void update_rels	(docx_conversion_context & Context);
    void set_fontTable	(element_ptr Element);
    void set_numbering	(element_ptr Element);
    void set_settings	(element_ptr Element);
	bool has_numbering	();
    void set_media			(mediaitems_ptr		& mediaitems_);
    void set_headers_footers(headers_footers	& HeadersFooters);
    void set_notes			(notes_context		& notesContext);
    void set_comments		(comments_context	& commentsContext);
    void set_drawings		(element_ptr Element);

	void add_charts(chart_content_ptr chart);
	void add_jsaProject(const std::string &content);

	void add_rels(relationship const & r);

private:
	docx_charts_files	charts_files_;
    rels_files			rels_files_;

    element_ptr theme_;
    element_ptr document_;
    element_ptr styles_;
    element_ptr fontTable_;
    element_ptr numbering_;
    element_ptr media_;
    element_ptr embeddings_;
    element_ptr headers_footers_;
    element_ptr notes_;
    element_ptr settings_;
    element_ptr comments_;
    element_ptr jsaProject_;
	element_ptr drawings_;

};

///////////////////////////////////////////////////////////

class docx_document : public document
{
public:
    docx_document();

			word_files			& get_word_files()			{ return word_files_; }
    virtual content_types_file	& get_content_types_file()	{ return content_type_file_; }
   
	void write(const std::wstring & RootPath);
	int add_customXml(customXml_content_ptr customXml);

private:
    docx_content_types_file	content_type_file_;
    word_files				word_files_;
	customXml_files			customXml_files_;
};

}
}
}
