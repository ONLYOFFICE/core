/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
class sheet_content;
typedef _CP_PTR(sheet_content) sheet_content_ptr;

class sheet_content : boost::noncopyable
{
public:
    sheet_content();
    std::wostream & content() { return content_; }
    void add_rel(relationship const & r);
	void add_rels(rels & r);
    rels_file_ptr get_rel_file() { return rels_; }
    std::wstring str() { return content_.str(); }
    static _CP_PTR(sheet_content) create();

private:
    std::wstringstream content_;
    rels_file_ptr rels_;
};
//------------------------------------------------------------------------
class pivot_cache_content;
typedef _CP_PTR(pivot_cache_content) pivot_cache_content_ptr;

class pivot_cache_content : boost::noncopyable
{
public:
    pivot_cache_content();
    static _CP_PTR(pivot_cache_content) create();

    std::wostream	& definitions() { return definitions_; }
	std::wostream	& records()		{ return records_; }
	rels			& get_rels()	{ return definitions_rels_file_->get_rels(); }

    std::wstring	str_d() { return definitions_.str(); }
    std::wstring	str_r() { return records_.str(); }
	
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

    std::vector<sheet_content_ptr> sheets_;
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
///////////////////////////////////////////////////////////

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

class xl_files : public element
{
public:
    xl_files();

public:
    virtual void write(const std::wstring & RootPath);

    void set_workbook		(element_ptr Element);
    void set_styles			(element_ptr Element);
    void set_sharedStrings	(element_ptr Element);
	void set_connections	(element_ptr Element);
    void add_sheet			(sheet_content_ptr sheet);
    void set_media			(external_items & _Mediaitems);    
    void set_drawings		(element_ptr Element);
	void set_vml_drawings	(element_ptr Element);
	void set_comments		(element_ptr Element);
    void add_charts			(chart_content_ptr chart);
    void add_pivot_cache	(pivot_cache_content_ptr cache);
	void add_pivot_table	(pivot_table_content_ptr table);
private:
    rels_files				rels_files_;
    sheets_files			sheets_files_;
    xl_charts_files			charts_files_;
	xl_pivot_cache_files	pivot_cache_files_;
	xl_pivot_table_files	pivot_table_files_;

	element_ptr				theme_;
    element_ptr				workbook_;

	element_ptr		connections_;
    element_ptr		styles_;
    element_ptr		sharedStrings_;
    element_ptr		media_;
    element_ptr		drawings_;
	element_ptr		vml_drawings_;
	element_ptr		comments_;

};

class xlsx_document : public document
{
public:
    xlsx_document();

public:
    virtual void write(const std::wstring & RootPath);
    virtual content_types_file & content_type() { return content_type_; }
    xl_files & get_xl_files() { return xl_files_; }

private:
    xlsx_content_types_file content_type_;
    xl_files				xl_files_;
    docProps_files			docProps_files_;
    rels_files				rels_files_;

};


}
}