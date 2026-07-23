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

#include <iosfwd>
#include "../../Common/CPSharedPtr.h"
#include "../../Common/CPScopedPtr.h"
#include "../../Common/CPNoncopyable.h"

#include "oox_rels.h"

namespace cpdoccore {
namespace oox {

class pptx_xml_slide;
typedef _CP_PTR(pptx_xml_slide) pptx_xml_slide_ptr;

class pptx_xml_slide: noncopyable
{
public:
    pptx_xml_slide(std::wstring const & name,std::wstring const & id);
    ~pptx_xml_slide();

    std::wstring name() const;
    std::wstring rId() const;
   
    std::wostream & Data();
    std::wostream & Background();
    std::wostream & Timing();
	
	rels & Rels();
    
    void write_to(std::wostream & strm);

    static pptx_xml_slide_ptr create(std::wstring const & name,int id);

    // NOTE: Temp solution
    // TODO: remove this method
    void remove_timing_redundant_space();
    
    void set_show(bool show_);

private:
	std::wstring name_;
    std::wstringstream  strmData_;
    std::wstringstream  strmBackground_;
    std::wstringstream  strmTiming_;
	std::wstring rId_;
    bool show;

	rels rels_;
};
//------------------------------------------------------------------------------------
class pptx_xml_slideNotes;
typedef _CP_PTR(pptx_xml_slideNotes) pptx_xml_slideNotes_ptr;

class pptx_xml_slideNotes: noncopyable
{
public:
    pptx_xml_slideNotes(std::wstring const & id);
    ~pptx_xml_slideNotes();

	std::wstring rId() const;
   
    std::wostream & Data();
    std::wostream & Background();
	
	rels & Rels();
    
    void write_to(std::wostream & strm);

    static pptx_xml_slideNotes_ptr create(int id);

private:
    std::wstringstream  strmData_;
    std::wstringstream  strmBackground_;
	std::wstring rId_;

	rels rels_;
};
//------------------------------------------------------------------------------------
class pptx_xml_presentation: noncopyable
{
public:
	pptx_xml_presentation(){}
	~pptx_xml_presentation(){}

	std::wstringstream  & slidesData()				{return slidesData_;}
    std::wstringstream  & slideMastersData()		{return slideMastersData_;}
    std::wstringstream  & slideNotesMastersData()	{return slideNotesMastersData_;}

    std::wstringstream  & slidesProperties()		{return slidesProperties_;}
    std::wstringstream  & slidesNotesProperties()	{return slidesNotesProperties_;}

	void write_to(std::wostream & strm);

private:
    std::wstringstream  slidesProperties_;
	std::wstringstream  slidesNotesProperties_;

    std::wstringstream  slidesData_;
    std::wstringstream  slideMastersData_;
    std::wstringstream  slideNotesMastersData_;
	std::wstringstream  slideHandoutMasterData_;
};

//------------------------------------------------------------------------------------
class pptx_xml_slideLayout;
typedef _CP_PTR(pptx_xml_slideLayout) pptx_xml_slideLayout_ptr;

class pptx_xml_slideLayout: noncopyable
{
public:
    pptx_xml_slideLayout(std::wstring const & id);
    ~pptx_xml_slideLayout();
public:
    std::wstring rId() const;
   
    std::wostream & Data();
	rels & Rels();

	//slideTiming
    
    void write_to(std::wostream & strm);
    void set_name(const std::wstring& layout_name);

    static pptx_xml_slideLayout_ptr create(int id);

private:
    std::wstringstream  strmData_;
	std::wstring rId_;
    std::wstring name;

	rels rels_;
};
//------------------------------------------------------------------------------------
class pptx_xml_slideMaster;
typedef _CP_PTR(pptx_xml_slideMaster) pptx_xml_slideMaster_ptr;

class pptx_xml_slideMaster: noncopyable
{
public:
    pptx_xml_slideMaster(std::wstring const & rId, int id);
    ~pptx_xml_slideMaster();

    std::wstring rId() const;
   
    std::wostream		& Data();
	std::wostream		& DataExtra();
	std::wostream		& Background();
	std::wstringstream	& Sizes();
	rels & Rels();

	void add_layout(int id, const std::wstring & rId, const unsigned int & uniqId);
 	void add_theme(int id, const std::wstring & tId);
   
    void write_to(std::wostream & strm);

    static pptx_xml_slideMaster_ptr create(int id);

private:
    std::wstringstream  strmData_;
	std::wstringstream  strmDataExtra_;
    std::wstringstream  strmBackground_;
    std::wstringstream  strmSizes_;
	
	std::vector<std::pair<std::wstring, unsigned int>> layoutsId_;
	std::wstring rId_;
	int id_;

	std::wstring themeId_;

	rels rels_;
};
//------------------------------------------------------------------------------------
class pptx_xml_slideNotesMaster;
typedef _CP_PTR(pptx_xml_slideNotesMaster) pptx_xml_slideNotesMaster_ptr;

class pptx_xml_slideNotesMaster: noncopyable
{
public:
    pptx_xml_slideNotesMaster(std::wstring const & rId, int id);
    ~pptx_xml_slideNotesMaster();

    std::wstring rId() const;
   
    std::wostream & Data();
	std::wostream & Background();
	std::wostream & Sizes();
	rels & Rels();

 	void add_theme(int id, const std::wstring & tId);
   
    void write_to(std::wostream & strm);

    static pptx_xml_slideNotesMaster_ptr create();

private:
    std::wstringstream  strmData_;
    std::wstringstream  strmBackground_;
    std::wstringstream  strmSizes_;
	
	std::wstring rId_;
	int id_;

	std::wstring themeId_;

	rels rels_;
};
//------------------------------------------------------------------------------------
class pptx_xml_theme;
typedef _CP_PTR(pptx_xml_theme) pptx_xml_theme_ptr;

class pptx_xml_theme: noncopyable
{
public:
	pptx_xml_theme(std::wstring const & name, int id){name_ = name; id_ = id;}
	~pptx_xml_theme(){}
public:
	std::wstring & name(){return name_;}
  	int & id(){return id_;}
 
	std::wstringstream & clrSchemeData(){return clrSchemeData_;}
	std::wstringstream & fontSchemeData(){return fontSchemeData_;}
	std::wstringstream & fmtSchemeData(){return fmtSchemeData_;}

    void write_to(std::wostream & strm);

    static pptx_xml_theme_ptr create(std::wstring const & name, int id);

private:
    std::wstringstream  clrSchemeData_;
    std::wstringstream  fmtSchemeData_;
    std::wstringstream  fontSchemeData_;
	
	std::wstring name_;
	int id_;
};
//------------------------------------------------------------------------------------
class pptx_xml_authors_comments;
typedef _CP_PTR(pptx_xml_authors_comments) pptx_xml_authors_comments_ptr;

class pptx_xml_authors_comments: noncopyable
{
public:
	pptx_xml_authors_comments(){}
	~pptx_xml_authors_comments(){}
public:

	struct _author_elm
	{
		std::wstring name;
		int last_idx;
	};
    void write_to(std::wostream & strm);

	std::pair<int,int> add_or_find(std::wstring name);//returns 2 indices - author - and last idx from author
	//since index tracking goes across all slides, comments, ....
	
	static pptx_xml_authors_comments_ptr create();

	std::vector<_author_elm> list_;
};
}
}
