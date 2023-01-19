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

private:
	std::wstring name_;
    std::wstringstream  strmData_;
    std::wstringstream  strmBackground_;
    std::wstringstream  strmTiming_;
	std::wstring rId_;

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

    static pptx_xml_slideLayout_ptr create(int id);

private:
    std::wstringstream  strmData_;
	std::wstring rId_;

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

	std::pair<int,int> add_or_find(std::wstring name);//возвращает 2 индекса - автора - и последний idx от автора
	//так как учет индексов идет по всем слайдам, замечаниям, ....
	
	static pptx_xml_authors_comments_ptr create();

	std::vector<_author_elm> list_;
};
}
}
