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

#include <boost/noncopyable.hpp>

#include "xlsx_drawings.h"
#include "xlsx_comments.h"

namespace oox {
namespace package {

class xlsx_content_types_file : public content_types_file
{
public:
    xlsx_content_types_file();
};
//------------------------------------------------------------------------
class sheet_content : boost::noncopyable
{
public:
    static _CP_PTR(sheet_content) create();

    sheet_content();

    void add_rel(relationship const & r);
	void add_rels(rels & r);
    rels_file_ptr get_rel_file() { return rels_; }
    
    std::wostream & content() { return content_; }
	std::wstring str() { return content_.str(); }

	void set_rId(std::wstring rid) {rId_ = rid;}
	std::wstring get_rId() {return rId_;}

private:
	std::wstring		rId_;
    std::wstringstream	content_;
    rels_file_ptr		rels_;
};
typedef _CP_PTR(sheet_content) sheet_content_ptr;
//------------------------------------------------------------------------
class pivot_cache_content : boost::noncopyable
{
public:
    pivot_cache_content();
    static _CP_PTR(pivot_cache_content) create();

    std::wstringstream	& definitions() { return definitions_; }
	std::wstringstream	& records()		{ return records_; }
	
	rels & get_rels() { return definitions_rels_file_->get_rels(); }

	friend class	xl_pivot_cache_files;
private:
    std::wstringstream	records_;
    std::wstringstream	definitions_;
	rels_file_ptr		definitions_rels_file_;
};
typedef _CP_PTR(pivot_cache_content) pivot_cache_content_ptr;
//------------------------------------------------------------------------
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
typedef _CP_PTR(pivot_table_content) pivot_table_content_ptr;
//------------------------------------------------------------------------
class table_part_content : boost::noncopyable
{
public:
    table_part_content();
    static _CP_PTR(table_part_content) create();

    std::wostream	& content()		{ return content_; }
	rels			& get_rels()	{ return rels_file_->get_rels(); }

    std::wstring	str() { return content_.str(); }
	
	friend class	xl_table_part_files;
private:
    std::wstringstream	content_;
	rels_file_ptr		rels_file_;
};
typedef _CP_PTR(table_part_content) table_part_content_ptr;
//------------------------------------------------------------------------
class external_content : boost::noncopyable
{
public:
    external_content();
    static _CP_PTR(external_content) create();

    std::wostream	& content() { return content_; }
    rels			& get_rels()	{ return rels_file_->get_rels(); }

    std::wstring	str() { return content_.str(); }
	
	friend class	xl_externals_files;
private:
    std::wstringstream	content_;
	rels_file_ptr		rels_file_;
};
typedef _CP_PTR(external_content) external_content_ptr;
//------------------------------------------------------------------------
class sheets_files  : public element
{
public:
    sheets_files();

    void add_sheet(int type, sheet_content_ptr sheet);

    void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }

    virtual void write(const std::wstring & RootPath);

    std::vector<sheet_content_ptr> worksheets_;
    std::vector<sheet_content_ptr> dialogsheets_;
    std::vector<sheet_content_ptr> macrosheets_;
    std::vector<sheet_content_ptr> chartsheets_;

private:
	void write_(std::vector<sheet_content_ptr> & sheets_, int & id, 
				  const std::wstring & RootPath, const std::wstring & local, const std::wstring & name, 
				  const std::wstring & rels_type, const std::wstring & content_type);
    rels_files * rels_;

};
class xl_charts_files  : public element
{
public:
	xl_charts_files(){}

    void			add_chart(chart_content_ptr chart);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<chart_content_ptr> charts_;

};
class xl_activeX_files : public element
{
public:
	xl_activeX_files(){}

    void			add_activeX(activeX_content_ptr activeX);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<activeX_content_ptr> activeXs_;
};
class xl_customXml_files : public element
{
public:
	xl_customXml_files(){}

    void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }
	void			add_customXml(customXml_content_ptr customXml);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<customXml_content_ptr> customXmls_;
    rels_files * rels_;
};
class xl_theme_files : public element
{
public:
	xl_theme_files(){}

    void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }
	void			add_theme(theme_content_ptr themeXml);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<theme_content_ptr> themes_;
    rels_files * rels_;
};
class xl_query_table_files : public element
{
public:
	xl_query_table_files(){}

    void			add_query_table(simple_element_ptr  query_table);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<simple_element_ptr> query_tables_;
};
class xl_control_props_files : public element
{
public:
	xl_control_props_files(){}

    void			add_control_props(simple_element_ptr  props);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<simple_element_ptr> control_props_;
};
class xl_externals_files  : public element
{
public:
	xl_externals_files(){}

	void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }
	void			add_external(external_content_ptr external);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<external_content_ptr> externals_;
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
class xl_table_part_files  : public element
{
public:
	xl_table_part_files(){}
	 
	void			add_table_part(table_part_content_ptr table);
	virtual void	write(const std::wstring & RootPath);
    
    std::vector<table_part_content_ptr> table_parts_;
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

class xl_drawings;
typedef _CP_PTR(xl_drawings) xl_drawings_ptr;

class xl_drawings: public element
{
public:
	bool vml;
    virtual void write(const std::wstring & RootPath);
    void set_rels(rels_files * rels)
    {
        rels_ = rels;
    }

    xl_drawings(const std::vector<drawing_elm> & elms) : drawings_ ( elms ), vml(false)
    {
    }

    static xl_drawings_ptr create(const std::vector<drawing_elm> & elms);

private:
    const std::vector<drawing_elm> & drawings_;
    rels_files * rels_;

};
//------------------------------------------------------------------------

class xl_files : public element
{
public:
    xl_files();

    virtual void write(const std::wstring & RootPath);

	rels_files  *get_rels() {return &rels_files_;}

    void set_workbook		(element_ptr Element);
    void set_styles			(element_ptr Element);
    void set_sharedStrings	(element_ptr Element);
	void set_connections	(element_ptr Element);
    void add_sheet			(int type, sheet_content_ptr sheet);
    void set_media			(external_items & _Mediaitems);    
    void set_drawings		(element_ptr Element);
	void set_vml_drawings	(element_ptr Element);
	void set_comments		(element_ptr Element);
	void set_macros_enabled	();
   
	void add_theme			(theme_content_ptr theme);
	void add_chart			(chart_content_ptr chart);
    void add_activeX		(activeX_content_ptr activeX);
    void add_external		(external_content_ptr external);
    void add_pivot_cache	(pivot_cache_content_ptr cache);
	void add_pivot_table	(pivot_table_content_ptr table);
    void add_query_table	(simple_element_ptr element);
	void add_control_props	(simple_element_ptr element);
	void add_table_part		(table_part_content_ptr table);
	void add_vba_project	();
	void add_attachedToolbars();
private:
    rels_files				rels_files_;
    sheets_files			sheets_files_;
    xl_theme_files			theme_files_;
    xl_charts_files			charts_files_;
    xl_externals_files		externals_files_;
	xl_pivot_cache_files	pivot_cache_files_;
	xl_pivot_table_files	pivot_table_files_;
	xl_activeX_files		activeXs_files_;
	xl_query_table_files	query_tables_files_;
	xl_control_props_files	control_props_files_;
	xl_table_part_files		table_part_files_;

    element_ptr		workbook_;

	element_ptr		connections_;
	element_ptr		styles_;
    element_ptr		sharedStrings_;
    element_ptr		media_;
    element_ptr		drawings_;
	element_ptr		vml_drawings_;
	element_ptr		comments_;

	bool			bVbaProject;
	bool			bMacrosEnabled;
	bool			bAttachedToolbars;

};

class xlsx_document : public document
{
public:
	xlsx_document();
    virtual content_types_file & content_type() { return content_type_; }

    virtual void write(const std::wstring & RootPath);
   
	xl_files & get_xl_files() { return xl_files_; }
   
	void add_customXml(customXml_content_ptr customXml);

private:
    xlsx_content_types_file content_type_;
    xl_files				xl_files_;
	xl_customXml_files		customXml_files_;
};


}
}