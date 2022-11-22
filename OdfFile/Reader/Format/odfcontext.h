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

#include "styles.h"
#include "templates.h"

#include <vector>
#include <boost/unordered_map.hpp>
#include "../../DataTypes/stylefamily.h"
#include "../../DataTypes/noteclass.h"

#include "styles_lite_container.h"

#include <iosfwd>

namespace cpdoccore { 

	std::string DecodeBase64(const std::wstring & value);

namespace odf_reader {

class styles_container;
class style_instance;

typedef boost::shared_ptr<style_instance>	style_instance_ptr;
typedef boost::shared_ptr<styles_container> styles_container_ptr;

class style_instance
{
public:
    style_instance(
        styles_container				* Container,
        const std::wstring				& Name,
        const std::wstring				& DisplayName,
        odf_types::style_family::type	Type,
        style_content					* Content,
        bool							IsAutomatic,
        bool							IsDefault,
        const std::wstring				& ParentStyleName,
        const std::wstring				& NextStyleName,
        const std::wstring				& DataStyleName,
        const std::wstring				& PercentageDataStyleName,
		const std::wstring				& StyleClass,
        _CP_OPT(std::wstring)			ListStyleName,
		_CP_OPT(int)					ListLevel,
		_CP_OPT(int)					OutlineLevel
        );

	const std::wstring				& display_name()const;
	const std::wstring				& name()		const;
    odf_types::style_family::type	type()			const;
    style_content					* content()		const;
    style_instance					* parent()		const;
    const std::wstring				& parent_name()	const;
    style_instance					* next()		const;
    const std::wstring				& next_name()	const;
	const styles_container			* container()	const { return container_; }
    const std::wstring				& data_style_name() const;
	const std::wstring				& percentage_data_style_name() const;
	const std::wstring				& style_class() const;
    _CP_OPT(std::wstring)			list_style_name() const;
 
	bool							is_automatic()	const;
    bool							is_default()	const;

	_CP_OPT(int)					list_level()	const;
	_CP_OPT(int)					outline_level()	const;
private:
    styles_container				* container_;

    std::wstring					name_;
	std::wstring					display_name_;

    odf_types::style_family::type	style_type_;
    style_content					* content_;

    bool							is_automatic_;    
    bool							is_default_;

    std::wstring					style_class_;
    std::wstring					parent_name_;
    std::wstring					next_name_;
    mutable style_instance			* parent_;
    mutable style_instance			* next_;   
    std::wstring					data_style_name_;
	std::wstring					percentage_data_style_name_;
    _CP_OPT(std::wstring)			list_style_name_;
	_CP_OPT(int)					list_level_;
	_CP_OPT(int)					outline_level_;
};

class presentation_layouts_instance
{
public:
	struct _layout
	{
		int Id;
		std::wstring rId;
		std::wstring layout_name;
		std::wstring master_name;
	};
	std::vector<_layout> content;

	std::pair<int,std::wstring> add_or_find(const std::wstring & layout_name,const std::wstring & master_name);
};
class presentation_masters_instance
{
public:
	struct _master
	{
		int Id;
		std::wstring rId;
		std::wstring master_name;
		
		std::vector<presentation_layouts_instance::_layout> layouts;
	};
	std::vector<_master> content;

	void add_layout_to(const std::wstring & master_name,presentation_layouts_instance::_layout & layout);
	
	std::pair<int,std::wstring> add_or_find(const std::wstring & master_name);
};
///////////////////////////////////////////////////////////////
class styles_container
{
public:
    styles_container() : hyperlink_style_pos_(-1)
    {}

    typedef std::vector<style_instance_ptr> instances_array;
    
	void add_style(	const std::wstring				& Name,
					const std::wstring				& DisplayName,
					style_content					*Content,
					bool							IsAutomatic,
					bool							IsDefault,
					const std::wstring				& ParentStyleName,
					const std::wstring				& NextStyleName,
					const std::wstring				& DataStyleName,
					const std::wstring				& PercentageDataStyleName,
					const std::wstring				& StyleClass,
					_CP_OPT(std::wstring)			ListStyleName = boost::none,
					_CP_OPT(int)					ListLevel = boost::none,
					_CP_OPT(int)					OutlineLevel = boost::none);

    void add_master_page_name(const std::wstring & StyleName, const std::wstring & MasterPageName);

    style_instance * hyperlink_style();
    
	style_instance * style_by_name			(const std::wstring & Name, odf_types::style_family::type Type, bool object_in_styles) const;
	style_instance * style_by_display_name	(const std::wstring & Name, odf_types::style_family::type Type, bool object_in_styles) const;

	style_instance * style_default_by_type	(odf_types::style_family::type Type) const;
    
	const _CP_OPT(std::wstring) master_page_name_by_name(const std::wstring & StyleName) const;

    instances_array & instances() { return instances_; } 

	presentation_layouts_instance & presentation_layouts() { return presentation_layouts_; } 
	presentation_masters_instance & presentation_masters() { return presentation_masters_; } 

private:
	presentation_layouts_instance presentation_layouts_;
	presentation_masters_instance presentation_masters_;
   
	instances_array instances_;
    
    typedef boost::unordered_map<std::wstring, int> map_wstring_int_t;
   
	map_wstring_int_t map_;		//by style name
    map_wstring_int_t map2_;	//by style display name

    typedef boost::unordered_map<std::wstring, std::wstring> map_wstring_wstring;
    map_wstring_wstring master_page_name_;

    typedef boost::unordered_map<odf_types::style_family::type, int> map_style_family_int;
    map_style_family_int default_map_;
   
	int hyperlink_style_pos_;
};

class page_layout_instance;
typedef boost::shared_ptr<page_layout_instance> page_layout_instance_ptr;

class page_layout_instance
{
public:
    page_layout_instance(const style_page_layout * StylePageLayout);
    const std::wstring & name() const;
    style_page_layout_properties * properties() const; 
    
	void docx_serialize				(std::wostream & strm, oox::docx_conversion_context & Context);
	void pptx_serialize				(std::wostream & strm, oox::pptx_conversion_context & Context);
 	void xlsx_serialize				(std::wostream & strm, oox::xlsx_conversion_context & Context);
   
    const style_page_layout * style_page_layout_;

};

class header_footer
{
public:
    header_footer() {};
};

class page_layout_container
{
public:
    typedef std::vector<page_layout_instance_ptr>	instances_array;

    void add_page_layout(const style_page_layout * StylePageLayout);
    void add_master_page(const std::wstring & StyleName, const std::wstring & PageLayoutName,style_master_page* MasterPage);
	void add_presentation_page_layout(const std::wstring & StyleName, style_presentation_page_layout* StylePageLayout);
	
	const std::wstring page_layout_name_by_style(const std::wstring & StyleName) const;
  
	instances_array & instances() { return instances_; };
   
	std::vector<style_master_page*> & master_pages() { return master_pages_; }

    const page_layout_instance * page_layout_by_style(const std::wstring & StyleName) const; 
    page_layout_instance * page_layout_by_name(const std::wstring & Name) const; 
  
	const page_layout_instance * page_layout_first() const;

    style_master_page * master_page_by_name(const std::wstring & Name);
	style_presentation_page_layout * presentation_page_layout_by_name(const std::wstring & Name); 
   
	bool compare_page_properties(const std::wstring & master1, const std::wstring & master2);
	
	void add_linenumbering(odf_reader::text_linenumbering_configuration *linenumbering) { linenumberingcConfiguration = linenumbering; }
	odf_reader::text_linenumbering_configuration *linenumbering() { return linenumberingcConfiguration; }
private:
	std::vector<style_presentation_page_layout*> presentation_page_layouts_;
    instances_array instances_;
    std::vector<style_master_page*> master_pages_;
   
	boost::unordered_map<std::wstring, int> page_layout_names_;
    std::vector<std::wstring> master_page_names_array_;
    boost::unordered_map<std::wstring, std::wstring> master_page_names_;
    
    boost::unordered_map<std::wstring, int> master_page_names_2_;    

	boost::unordered_map<std::wstring, int> presentation_page_layout_names_;

	odf_reader::text_linenumbering_configuration *linenumberingcConfiguration = NULL;
};


class font_instance;
typedef boost::shared_ptr<font_instance> font_instance_ptr;

class font_instance
{
public:
    font_instance( const std::wstring & StyleName,
        const std::wstring & Name,
        const std::wstring & Charset,
        const std::wstring & Family,
        const std::wstring & Pitch,
        const std::wstring & AltName);

    const std::wstring & style_name() const;
    const std::wstring & name() const;
    const std::wstring & charset() const;
    const std::wstring & family() const;
    const std::wstring & pitch() const;
    const std::wstring & alt_name() const;

private:
    std::wstring style_name_;
    std::wstring name_;
    std::wstring charset_;
    std::wstring family_;
    std::wstring pitch_;
    std::wstring alt_name_;

};

class fonts_container
{
public:
    typedef std::vector<font_instance_ptr> instances_array;

	font_instance * font_by_style_name(const std::wstring & StyleName);
    font_instance * font_by_name(const std::wstring & Name);
    instances_array & instances() { return instances_; }
    void add_font( font_instance_ptr FontInstance );

private:
    boost::unordered_map<std::wstring, int> font_names_;
    boost::unordered_map<std::wstring, int> font_style_names_;
    instances_array instances_;

};

class list_style_instance;
typedef boost::shared_ptr<list_style_instance> list_style_instance_ptr;

class list_style_instance
{
public:
    list_style_instance(text_list_style * textListStyle) : text_list_style_(textListStyle) {}
    list_style_instance(text_list_style * textListStyle, const std::wstring & Name) : text_list_style_(textListStyle), name_(Name) {}
    
	text_list_style * get_text_list_style() { return text_list_style_; }
    std::wstring get_style_name() 
    { 
        if (!name_.empty())
            return name_;
        else if (text_list_style_)
            return text_list_style_->attr_.style_name_;
        else
            return L"";
    }
    
private:
    text_list_style		*text_list_style_;
    std::wstring		name_;
};

class list_style_container
{
public:
    typedef std::vector<list_style_instance_ptr> instances_array;

	void add_list_style(text_list_style * textListStyle);
    void add_list_style(text_list_style * textListStyle, const std::wstring & NewName);

    text_list_style * list_style_by_name(const std::wstring & Name);

	text_outline_style * outline_style();
	int id_outline();
 
	 /// получить Id, которое будет использоваться в numId и abstractNumId
    int id_by_name(const std::wstring & Name);
    
	instances_array & instances() { return instances_; }
   
	bool empty() const { return instances_.empty() && !outline_; };

	void add_outline_style(text_outline_style *textOutlineStyle);

private:
    // ассоциации имени стиля и порядкового номера в массиве instances_
    boost::unordered_map<std::wstring, int> list_style_names_;
    instances_array							instances_;

	text_outline_style						*outline_ = NULL;
	int										outline_id_ = 0;
};

class notes_configuration
{
public:
	const text_notes_configuration * getConfiguration(odf_types::noteclass::type noteType) const;
	void add(odf_types::noteclass::type noteType, const text_notes_configuration * conf);
private:
    boost::unordered_map<odf_types::noteclass::type, const text_notes_configuration *> type_to_name_;
        
};

class odf_read_context
{
public:
    styles_container &		styleContainer()		{ return major_style_container_; }
    page_layout_container & pageLayoutContainer()	{ return page_layout_container_; }
    fonts_container &		fontContainer()			{ return fonts_container_; }
    list_style_container &	listStyleContainer()	{ return list_style_container_; }
    
	notes_configuration &	noteConfiguration()		{ return notes_configuration_; }
   
	styles_lite_container&	numberStyles()			{ return number_style_container_; }
    styles_lite_container&	drawStyles()			{ return draw_style_container_; }
	styles_lite_container&	Templates()				{ return template_container_; }
	settings_container&		Settings()				{ return settings_container_; }
	doc_props_container&	DocProps()				{ return doc_props_container_; }

private:
    styles_container		major_style_container_;
    page_layout_container	page_layout_container_;
    fonts_container			fonts_container_;
    list_style_container	list_style_container_;
    notes_configuration		notes_configuration_;
   
	styles_lite_container	number_style_container_;
    styles_lite_container	draw_style_container_;
    styles_lite_container	template_container_;
    
	settings_container		settings_container_;
	doc_props_container		doc_props_container_;
};


}
}
