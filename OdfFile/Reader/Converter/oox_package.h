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

#include <vector>

#include <CPSharedPtr.h>
#include <CPNoncopyable.h>
#include <CPOptional.h>

#include "docx_content_type.h"
#include "oox_rels.h"

#include "../../../OOXML/Base/Base.h"
#include "../../../DesktopEditor/common/Directory.h"

namespace NSFonts
{
    class IApplicationFonts;
}

namespace cpdoccore { 
namespace oox {

class mediaitems;
typedef _CP_PTR(mediaitems) mediaitems_ptr;

namespace package {

class element;
typedef shared_ptr<element>::Type element_ptr;
typedef std::vector<element_ptr> element_ptr_array;

class document;


//------------------------------------------------------------------------
class element
{
public:
    virtual ~element() = 0;
    void		set_main_document(document * _document);
    document *	get_main_document();

    virtual void write(const std::wstring & RootPath) = 0;

private:
    document * document_;
};

inline element::~element()
{}

class content_types_file : public element
{
public:
					content_types_file	();
	void			write				(const std::wstring & RootPath);
	bool			add_or_find_default	(const std::wstring & extension);
	bool			add_or_find_override(const std::wstring & fileName);
	void			set_media			(mediaitems & _Mediaitems);
	void			set_media			(mediaitems_ptr & _Mediaitems);
    content_type*	content				();

private:
    content_type	content_type_content_;
    std::wstring	filename_;
};

class simple_element;
typedef boost::shared_ptr<simple_element> simple_element_ptr;

class simple_element : public element
{
public:
    simple_element(const std::wstring & FileName, const std::wstring & Content);
    static simple_element_ptr create(const std::wstring & FileName, const std::wstring & Content);

    simple_element(const std::wstring & FileName, const std::string & Content);
    static simple_element_ptr create(const std::wstring & FileName, const std::string & Content);

	virtual void write(const std::wstring & RootPath);
	std::wstring get_filename() {return file_name_;}

private:
    std::wstring file_name_;
    std::string content_utf8_;

	bool bXml;

};
//------------------------------------------------------------------------
class rels_file;
typedef boost::shared_ptr<rels_file> rels_file_ptr;

class rels_file : public element
{
public:
    rels_file(std::wstring const & FileName);
    void set_file_name(std::wstring const & fileName) { filename_ = fileName;} 
    static rels_file_ptr create(std::wstring const & FileName);

public:
    virtual void write(const std::wstring & RootPath);

			rels & get_rels() { return rels_; }
    const	rels & get_rels() const { return rels_; }

    bool empty() { return rels_.empty(); }

private:
    std::wstring	filename_;
    rels			rels_;
};

//------------------------------------------------------------------------
class rels_files : public element
{
public:
    virtual void write(const std::wstring & RootPath);
    void add_rel_file(rels_file_ptr RelFile) {rels_file_ = RelFile; } ;
    void add(relationship const & r);
    void add(std::wstring const & Id,
        std::wstring const & Type,
        std::wstring const & Target,
        std::wstring const & TargetMode = L"");

    rels_file * get_rel_file() { return rels_file_.get(); }
    
private:
    rels_file_ptr rels_file_;
};

//------------------------------------------------------------------------
class customXml_content;
typedef _CP_PTR(customXml_content) customXml_content_ptr;

class customXml_content : boost::noncopyable
{
public:
	customXml_content(const std::wstring &item, const std::wstring &props) : content_item(item), content_props(props) {}
	static _CP_PTR(customXml_content) create(const std::wstring &item, const std::wstring &props);

    std::wstring	item()	{ return content_item; }
    std::wstring	props() { return content_props; }
	
	friend class	customXml_files;
private:
    std::wstring	content_item;
    std::wstring	content_props;
};
///------------------------------------------------------------------------
class chart_content;
typedef _CP_PTR(chart_content) chart_content_ptr;

class chart_content : boost::noncopyable
{
public:
    chart_content();
    std::wostream & content() { return content_; }
    void add_rel(relationship const & r);
    rels_file_ptr get_rel_file() { return rels_; }
    std::wstring str() { return content_.str(); }
    static _CP_PTR(chart_content) create();
private:
    std::wstringstream	content_;
    rels_file_ptr rels_;
};
//------------------------------------------------------------------------
class docProps_files : public element
{
public:
    docProps_files();

	void set_app(element_ptr Element);
	void set_core(element_ptr Element);

	virtual void write(const std::wstring & RootPath);

private:
	std::wstring create_app();
	std::wstring create_core();

	element_ptr core_;
	element_ptr app_;
};
class document : public element
{
public:
	virtual content_types_file & get_content_types_file() = 0;
	docProps_files	& get_docProps_files() { return docProps_files_; }
	rels_files & get_rels_files() { return rels_files_; }
private:
	docProps_files docProps_files_;
	rels_files rels_files_;
};

class media : public element
{
public:
    media(mediaitems_ptr & _mediaItems, NSFonts::IApplicationFonts *pAppFonts);

public:
    virtual void write(const std::wstring & RootPath);

private:
    mediaitems_ptr				mediaItems_;
    NSFonts::IApplicationFonts* appFonts_;
        
};

class charts : public element
{
public:
    charts(mediaitems_ptr & _chartsItems);

public:
    virtual void write(const std::wstring & RootPath);

private:
    mediaitems_ptr chartsItems_;
        
};
class embeddings : public element
{
public:
    embeddings(mediaitems_ptr & _embeddingsItems);

public:
    virtual void write(const std::wstring & RootPath);

private:
    mediaitems_ptr  embeddingsItems_;
        
};
} // namespace package
} // namespace oox
} // namespace cpdoccore

