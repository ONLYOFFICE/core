#pragma once

#include "object_package.h"
#include "odf_style_context.h"

namespace cpdoccore { 
namespace odf {

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//class styles_container;
//class style_instance;

//typedef boost::shared_ptr<style_instance> style_instance_ptr;
//typedef boost::shared_ptr<styles_container> styles_container_ptr;

//class style_instance
//{
//public:
//    style_instance(
//        styles_container * Container,
//        const std::wstring & Name,
//        style_family::type Type,
//        style_content * Content,
//        bool IsAutomatic,
//        bool IsDefault,
//        const std::wstring & ParentStyleName,
//        const std::wstring & NextStyleName,
//        const std::wstring & DataStyleName
//        );
//
//
//    const std::wstring & name() const;
//    style_family::type type() const;
//    style_content * content() const;
//    style_instance * parent() const;
//    const std::wstring & parent_name() const;
//    style_instance * next() const;
//    const std::wstring & next_name() const;
//    bool is_automatic() const;
//    bool is_default() const;
//    const styles_container * container() const { return container_; }
//    const std::wstring & data_style_name() const;
//
//private:
//    styles_container * container_;
//    std::wstring name_;
//    style_family::type style_type_;
//    style_content * content_;
//
//    bool is_automatic_;    
//    bool is_default_;
//
//    std::wstring parent_name_;
//    std::wstring next_name_;
//    mutable style_instance * parent_;
//    mutable style_instance * next_;   
//    std::wstring data_style_name_;
//};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class styles_container
//{
//public:
//    styles_container() : hyperlink_style_pos_(-1)
//    {}
//
//    typedef std::vector<style_instance_ptr> instances_array;
//    
//	void add_style(const std::wstring & Name,
//        style_family::type Type,
//        style_content * Content,
//        bool IsAutomatic,
//        bool IsDefault,
//        const std::wstring & ParentStyleName,
//        const std::wstring & NextStyleName,
//        const std::wstring & DataStyleName);
//
//    void add_master_page_name(const std::wstring & StyleName, const std::wstring & MasterPageName);
//
//    style_instance * hyperlink_style();
//    style_instance * style_by_name(const std::wstring & Name, style_family::type Type,bool object_in_styles) const;
//    style_instance * style_default_by_type(style_family::type Type) const;
//    
//	const std::wstring master_page_name_by_name(const std::wstring & StyleName) const;
//
//    instances_array & instances() { return instances_; } 
//
//	presentation_layouts_instance & presentation_layouts() { return presentation_layouts_; } 
//	presentation_masters_instance & presentation_masters() { return presentation_masters_; } 
//
//private:
//	//presentation_layouts_instance presentation_layouts_;
//	//presentation_masters_instance presentation_masters_;
//   
//	instances_array instances_;
//    
//    typedef boost::unordered_map<std::wstring, int> map_wstring_int_t;
//    map_wstring_int_t map_;
//
//    typedef boost::unordered_map<std::wstring, std::wstring> map_wstring_wstring;
//    map_wstring_wstring master_page_name_;
//
//    typedef boost::unordered_map<style_family::type, int> map_style_family_int;
//    map_style_family_int default_map_;
//    int hyperlink_style_pos_;
//};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class odf_conversion_context : boost::noncopyable
{
public:
    odf_conversion_context(package::odf_document * outputDocument);
    virtual ~odf_conversion_context();

	virtual void start_document() = 0 ;
	void end_document();

	office_element_ptr & getCurrentElement();

	std::vector<office_element_ptr>  content_;
	std::vector<office_element_ptr>  content_styles_;
	std::vector<office_element_ptr>  styles_;
	
    void add_rel(relationship const & r);

	package::odf_document * output_document_;

	odf_style_context & styles_context(){return style_context_;}
	

private:
	rels	rels_;
	void process_styles();

public:

	odf_style_context		style_context_;

    //styles_container		& styleContainer()		{ return major_style_container_; }
	//page_layout_container & pageLayoutContainer()	{ return page_layout_container_; }
	//fonts_container		& fontContainer()		{ return fonts_container_; }
	//list_style_container	& listStyleContainer()	{ return list_style_container_; }

	//notes_configuration &	noteConfiguration()		{ return notes_configuration_; }

	//styles_lite_container &	numberStyles()		{ return number_style_container_; }
	//styles_lite_container &	drawStyles()		{ return draw_style_container_; }
	//styles_lite_container &	Templates()			{ return template_container_; }

private:
    //styles_container		major_style_container_;
	//page_layout_container	page_layout_container_;
	//fonts_container		fonts_container_;
	//list_style_container	list_style_container_;
	//notes_configuration	notes_configuration_;

	//styles_lite_container	number_style_container_;
	//styles_lite_container	draw_style_container_;
	//styles_lite_container	template_container_;
};

}
}