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

#include "oox_package.h"

#include "../../Common/CPNoncopyable.h"

#include "xlsx_drawings.h"
#include "xlsx_comments.h"

namespace NSFonts
{
    class IApplicationFonts;
}

namespace cpdoccore { 
namespace oox {
namespace package {

class xlsx_content_types_file : public content_types_file
{
public:
    xlsx_content_types_file();
};

class sheet_content : noncopyable
{
public:
    sheet_content();
    std::wstringstream & content() { return content_; }
    void add_rel(relationship const & r);
	void add_rels(rels & r);
    rels_file_ptr get_rel_file() { return rels_; }
    static _CP_PTR(sheet_content) create();
private:
    std::wstringstream content_;
    rels_file_ptr rels_;
};
typedef _CP_PTR(sheet_content) sheet_content_ptr;
//------------------------------------------------------------------------
class pivot_cache_content;
typedef _CP_PTR(pivot_cache_content) pivot_cache_content_ptr;

class pivot_cache_content : boost::noncopyable
{
public:
    pivot_cache_content();
    static _CP_PTR(pivot_cache_content) create();

    std::wstringstream	& definitions() { return definitions_; }
	std::wstringstream	& records()		{ return records_; }
	rels			& get_rels()	{ return definitions_rels_file_->get_rels(); }

	friend class	xl_pivot_cache_files;
private:
    std::wstringstream	records_;
    std::wstringstream	definitions_;
	rels_file_ptr		definitions_rels_file_;
};
//------------------------------------------------------------------------
class pivot_table_content;
typedef _CP_PTR(pivot_table_content) pivot_table_content_ptr;

class pivot_table_content : boost::noncopyable
{
public:
    pivot_table_content();
    static _CP_PTR(pivot_table_content) create();

    std::wostream	& content()		{ return content_; }
	rels			& get_rels()	{ return rels_file_->get_rels(); }

    std::wstring	str() { return content_.str(); }
	
	friend class	xl_pivot_table_files;
private:
    std::wstringstream	content_;
	rels_file_ptr		rels_file_;
};
//------------------------------------------------------------------------
class external_links_content;
typedef _CP_PTR(external_links_content) external_links_content_ptr;

class external_links_content : boost::noncopyable
{
public:
    external_links_content();
    static external_links_content_ptr create();

	std::wstring	& rId()			{ return rId_;}
    std::wostream	& content()		{ return content_; }
	rels			& get_rels()	{ return rels_file_->get_rels(); }

    std::wstring	str() { return content_.str(); }
	
	friend class	xl_external_links_files;
private:
    std::wstringstream	content_;
	rels_file_ptr		rels_file_;
	std::wstring		rId_;
};
//------------------------------------------------------------------------
class sheets_files  : public element
{
public:
    sheets_files();

    void add_sheet(sheet_content_ptr sheet);

    void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }

    virtual void write(const std::wstring & RootPath);

public:    
    std::vector<sheet_content_ptr> sheets_;
    rels_files * rels_;

};

class xl_table_part_files  : public element
{
public:
	xl_table_part_files(){}

    void add_table_part(const std::wstring & table_part);
	virtual void write(const std::wstring & RootPath);
    
    std::vector<std::wstring> table_parts_;

};
class xl_charts_files  : public element
{
public:
	xl_charts_files(){}

    void add_chart(chart_content_ptr chart);
	virtual void write(const std::wstring & RootPath);
    
    std::vector<chart_content_ptr> charts_;

};
class xl_external_links_files  : public element
{
public:
	xl_external_links_files(){}
	 
	void			add_external_links(external_links_content_ptr content);
	virtual void	write(const std::wstring & RootPath);
    
	void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }  
private:
    std::vector<external_links_content_ptr> external_links_;
    rels_files * rels_;
};
class xl_pivot_table_files  : public element
{
public:
	xl_pivot_table_files(){}
	 
	void			add_pivot_table(pivot_table_content_ptr pivot_table);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<pivot_table_content_ptr> pivot_tables_;
};
class xl_pivot_cache_files  : public element
{
public:
	xl_pivot_cache_files(){}

	void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }
	 
	void			add_pivot_cache(pivot_cache_content_ptr pivot_cache);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<pivot_cache_content_ptr> pivot_caches_;

    rels_files * rels_;
};
//-------------------------------------------------------------------------------------------------------------
class xl_comments;
typedef _CP_PTR(xl_comments) xl_comments_ptr;
class xl_comments: public element
{
public:
    virtual void write(const std::wstring & RootPath);

    xl_comments(const std::vector<comment_elm> & elms) : comments_ ( elms )
    {
    }

    static xl_comments_ptr create(const std::vector<comment_elm> & elms);

private:
    const std::vector<comment_elm> & comments_;
};
//-----------------------------------------------------------------------------------------------------
class xl_drawings;
typedef _CP_PTR(xl_drawings) xl_drawings_ptr;
class xl_drawings: public element
{
public:
    virtual void write(const std::wstring & RootPath);
   
	//void set_rels(rels_files * rels)
 //   {
 //       rels_ = rels;
 //   }
	//void set_vml_rels(rels_files * rels)
 //   {
 //       vml_rels_ = rels;
 //   }
    xl_drawings(const std::vector<drawing_elm> & elms, const std::vector<drawing_elm> & vml_elms) : drawings_(elms), vml_drawings_(vml_elms)
    {
    }

    static xl_drawings_ptr create(const std::vector<drawing_elm> & elms, const std::vector<drawing_elm> & vml_elms);

private:
    const std::vector<drawing_elm> & drawings_;
	const std::vector<drawing_elm> & vml_drawings_;
	
	//rels_files * rels_;
	//rels_files * vml_rels_;
};
//----------------------------------------------------------------------------------------------------------
class xl_control_props_files : public element
{
public:
	xl_control_props_files(){}

    void			add_control_props(simple_element_ptr  props);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<simple_element_ptr> control_props_;
};
//----------------------------------------------------------------------------------------------------------

class xl_files : public element
{
public:
    xl_files();

    virtual void write(const std::wstring & RootPath);

    void set_workbook		(element_ptr Element);
    void set_styles			(element_ptr Element);
    void set_sharedStrings	(element_ptr Element);
	void set_connections	(element_ptr Element);
    void add_sheet			(sheet_content_ptr sheet);
    void set_media			(mediaitems_ptr & _Mediaitems);
    void set_drawings		(element_ptr Element);
	void set_vml_drawings	(element_ptr Element);
	void set_comments		(element_ptr Element);
    void add_charts			(chart_content_ptr chart);
    void add_pivot_cache	(pivot_cache_content_ptr cache);
	void add_pivot_table	(pivot_table_content_ptr table);
	void add_jsaProject		(const std::string &content);
	void add_control_props	(simple_element_ptr Element);
	void add_table_part		(const std::wstring &content);
	void add_external_links	(external_links_content_ptr content);

private:
    rels_files				rels_files_;
    sheets_files			sheets_files_;
    xl_charts_files			charts_files_;
	xl_pivot_cache_files	pivot_cache_files_;
	xl_pivot_table_files	pivot_table_files_;
  	xl_control_props_files	control_props_files_;
	xl_table_part_files		table_part_files_;
 	xl_external_links_files	external_links_files_;

	element_ptr		theme_;
    element_ptr		workbook_;

    element_ptr		styles_;
    element_ptr		sharedStrings_;
    element_ptr		media_;
	element_ptr		embeddings_;
	element_ptr		drawings_;
	element_ptr		vml_drawings_;
	element_ptr		comments_;
	element_ptr		connections_;
    element_ptr		jsaProject_;
};

class xlsx_document : public document
{
public:
    xlsx_document();

public:
    virtual void write(const std::wstring & RootPath);

    virtual content_types_file	& get_content_types_file()	{ return content_type_file_; }
    xl_files					& get_xl_files()			{ return xl_files_; }
private:
    xlsx_content_types_file content_type_file_;
    xl_files				xl_files_;
};


}
}
}
