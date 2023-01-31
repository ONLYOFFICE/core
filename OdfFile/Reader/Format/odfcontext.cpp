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

#include "odfcontext.h"

namespace cpdoccore { 

	using namespace odf_types;
	
	std::string DecodeBase64(const std::wstring & value1)
	{
		int nLength = 0;
		unsigned char *pData = NULL;
		std::string result;

		std::string value(value1.begin(), value1.end());

		NSFile::CBase64Converter::Decode(value.c_str(), value.length(), pData, nLength);
		if (pData)
		{
			result = std::string((char*)pData, nLength);
			delete []pData; pData = NULL;
		}
		return result;
	}
namespace odf_reader {

style_instance::style_instance(
		styles_container   *Container,
		const std::wstring  &Name,
		const std::wstring	&DisplayName,
		style_family::type	Type,
		style_content      *Content,
		bool IsAutomatic,
		bool IsDefault,
		const std::wstring		& ParentStyleName,
		const std::wstring		& NextStyleName,
		const std::wstring		& DataStyleName,
		const std::wstring		& PercentageDataStyleName,
		const std::wstring		& StyleClass,
		_CP_OPT(std::wstring)	ListStyleName,
		_CP_OPT(int)			ListLevel,
		_CP_OPT(int)			OutlineLevel
	) :
	container_		(Container),
    name_			(Name),
	display_name_	(DisplayName),
    style_type_		(Type),
    content_		(Content),
    is_automatic_	(IsAutomatic),
    is_default_		(IsDefault),
    next_name_		(NextStyleName),
	style_class_	(StyleClass),
    next_			(Container->style_by_name(NextStyleName, style_type_, false)),
    data_style_name_(DataStyleName),
	percentage_data_style_name_(PercentageDataStyleName),
	list_style_name_(ListStyleName),
	list_level_		(ListLevel),
	outline_level_	(OutlineLevel)
{
	parent_name_ = ParentStyleName;
	if (parent_name_ == L"Textformatvorlage")//http://ask.libreoffice.org/en/question/35136/textformatvorlage-style/
	{
		parent_name_ = L"Standard";
	}
    parent_ = Container->style_by_name(parent_name_, style_type_, false);
}
   
style_instance * styles_container::hyperlink_style()
{
    if (hyperlink_style_pos_ > 0 && hyperlink_style_pos_ < (int)instances_.size())
        return instances_[hyperlink_style_pos_].get();
    else
        return NULL;
}

void styles_container::add_style(	const std::wstring & Name,
									const std::wstring & DisplayName,
									style_content * Content,
									bool IsAutomatic,
									bool IsDefault,
									const std::wstring		& ParentStyleName_,
									const std::wstring		& NextStyleName,
									const std::wstring		& DataStyleName,
									const std::wstring		& PercentageDataStyleName,
									const std::wstring		& StyleClass,
									_CP_OPT(std::wstring)	ListStyleName,
									_CP_OPT(int)			ListLevel,
									_CP_OPT(int)			OutlineLevel)
{
	std::wstring ParentStyleName = ParentStyleName_;

	style_family::type Type = Content ? Content->style_family_.get_type() : style_family::None;
	
	if (Name == ParentStyleName)
	{
		ParentStyleName = L"";//иначе в коде возможно зацикливание.
	}
    style_instance_ptr newStyle = style_instance_ptr( new style_instance(this, Name, DisplayName, Type, Content, IsAutomatic, IsDefault, 
							ParentStyleName, NextStyleName, DataStyleName, PercentageDataStyleName, StyleClass, ListStyleName, ListLevel, OutlineLevel));

    instances_.push_back(newStyle);
    int pos = static_cast<int>(instances_.size() - 1);
    
    if (!Name.empty())
    {
        std::wstring n = Name + L":" + boost::lexical_cast<std::wstring>( style_family(Type) );
        map_[n] = pos;

        // TODO: как правильно??
        std::wstring lName = XmlUtils::GetLower(Name);
        //if ( boost::algorithm::contains(lName, L"internet_20_link") )
        if (lName == L"internet_20_link")///???????????????
            hyperlink_style_pos_ = pos;
    }

	if (!DisplayName.empty())
	{
        std::wstring n = DisplayName + L":" + boost::lexical_cast<std::wstring>( style_family(Type) );
        map2_[n] = pos;
	}

    if (IsDefault)
        default_map_[Type] = pos;
        
}

const std::wstring & style_instance::name() const
{
    return name_;
}
const std::wstring & style_instance::display_name() const
{
    return display_name_;
}
style_family::type style_instance::type() const
{
    return style_type_;
}

style_content * style_instance::content() const
{
    return content_;
}

style_instance * style_instance::parent() const
{
    if (parent_)
        return parent_;
    else if (container_)
        parent_ = container_->style_by_name(parent_name_, type(), false);
    
    return parent_;
}

const std::wstring & style_instance::parent_name() const
{
    return parent_name_;
}

style_instance * style_instance::next() const
{
    if (next_)
        return next_;
    else if (container_ && next_name_.empty() == false)
        next_ = container_->style_by_name(next_name_, type(), false);
    
    return next_;
}

const std::wstring & style_instance::next_name() const
{
    return next_name_;
}

bool style_instance::is_automatic() const
{
    return is_automatic_;
}

bool style_instance::is_default() const
{
    return is_default_;
}
const std::wstring & style_instance::percentage_data_style_name() const
{
    return percentage_data_style_name_;
}
const std::wstring & style_instance::data_style_name() const
{
    return data_style_name_;
}
_CP_OPT(std::wstring) style_instance::list_style_name() const
{
    return list_style_name_;
}
const std::wstring & style_instance::style_class() const
{
    return style_class_;
}
_CP_OPT(int) style_instance::list_level() const
{
    return list_level_;
}
_CP_OPT(int) style_instance::outline_level() const
{
    return outline_level_;
}

style_instance * styles_container::style_by_name(const std::wstring & Name, style_family::type Type, bool object_in_styles) const
{
    std::wstring n = L"";
	if (object_in_styles) n = L"common:";
	n = n + Name + L":" + boost::lexical_cast<std::wstring>( style_family(Type) );
   
	map_wstring_int_t::const_iterator res = map_.find(n);
    
    if (res != map_.end())
    {
		int index = res->second;
        return instances_[index].get();
    }
    else if (object_in_styles)
	{
		//try automatic
		n = Name + L":" + boost::lexical_cast<std::wstring>( style_family(Type) );
	   
		map_wstring_int_t::const_iterator res = map_.find(n);
	    
		if (res != map_.end())
		{
			int index = res->second;
			return instances_[index].get();
		}
	}
	return NULL;
}
style_instance * styles_container::style_by_display_name(const std::wstring & Name, style_family::type Type, bool object_in_styles) const
{
    std::wstring n = L"";
	if (object_in_styles) n = L"common:";
	n = n + Name + L":" + boost::lexical_cast<std::wstring>( style_family(Type) );
    
	map_wstring_int_t::const_iterator res = map2_.find(n);
    
    if (res != map2_.end())
    {
		int index = res->second;
        return instances_[index].get();
    }
    else
        return NULL;
}
void styles_container::add_master_page_name(const std::wstring & StyleName, const std::wstring & MasterPageName)
{
    master_page_name_[StyleName] = MasterPageName;
}

std::pair<int,std::wstring> presentation_layouts_instance::add_or_find(const std::wstring & layout_name,const std::wstring & master_name)
{
	bool find = false;
	size_t index =0;
	
	for (index = 0; index < content.size(); index++)
	{
		if (content[index].layout_name == layout_name && content[index].master_name == master_name)
		{
			find = true;
			break;
		}
	}
	if (!find)
	{
		presentation_layouts_instance::_layout item;
		item.layout_name = layout_name;
		item.master_name = master_name;
		item.Id = content.size() +1;
		item.rId = std::wstring(L"lrId") + std::to_wstring(item.Id);
	
		content.push_back(item);
		index = content.size()-1;

	}
	return std::pair<int,std::wstring>(content[index].Id,content[index].rId);
}

std::pair<int,std::wstring> presentation_masters_instance::add_or_find(const std::wstring & master_name)
{
	bool find = false;
	size_t  index =0;
	for (index = 0; index < content.size(); index++)
	{
		if (content[index].master_name == master_name)
		{
			find = true;
			break;
		}
	}
	if (!find)
	{
		presentation_masters_instance::_master item;
		item.master_name = master_name;
		item.Id = content.size() +1;
		item.rId = std::wstring(L"smId") + std::to_wstring(item.Id);
	
		content.push_back(item);
		index = content.size()-1;

	}
	return std::pair<int,std::wstring>(content[index].Id,content[index].rId);
}

void presentation_masters_instance::add_layout_to(const std::wstring & master_name, presentation_layouts_instance::_layout & layout)
{
	bool find = false;
	size_t index = 0;
	for (index = 0; index < content.size(); index++)
	{
		if (content[index].master_name == master_name)
		{
			find = true;
			break;
		}
	}
	if (find)
	{
		content[index].layouts.push_back(layout);
	}
}

const _CP_OPT(std::wstring) styles_container::master_page_name_by_name(const std::wstring & StyleName) const
{
	_CP_OPT(std::wstring) master_page;

    map_wstring_wstring::const_iterator res = master_page_name_.find(StyleName);
    if (res != master_page_name_.end())
        master_page =  res->second;
    return master_page;
}

style_instance * styles_container::style_default_by_type(style_family::type Type) const
{
    map_style_family_int::const_iterator res = default_map_.find(Type);
    if (res != default_map_.end())
        return instances_[res->second].get();
    else
        return NULL;
}

page_layout_instance::page_layout_instance(const style_page_layout * StylePageLayout) : style_page_layout_(StylePageLayout)
{        
}

const std::wstring & page_layout_instance::name() const
{     
    return style_page_layout_->style_name_;
}

style_page_layout_properties * page_layout_instance::properties() const
{    
    return dynamic_cast<style_page_layout_properties *>(style_page_layout_->style_page_layout_properties_.get());
}

void page_layout_instance::xlsx_serialize(std::wostream & strm, oox::xlsx_conversion_context & Context)
{
    const style_header_style * headerStyle = dynamic_cast<style_header_style *>(style_page_layout_->style_header_style_.get());
    const style_footer_style * footerStyle = dynamic_cast<style_footer_style *>(style_page_layout_->style_footer_style_.get());

	style_header_footer_properties * headerProp = headerStyle ? dynamic_cast<style_header_footer_properties *>(headerStyle->style_header_footer_properties_.get()) : NULL;
	style_header_footer_properties * footerProp = footerStyle ? dynamic_cast<style_header_footer_properties *>(footerStyle->style_header_footer_properties_.get()) : NULL;

    if (headerProp)
    {
        const style_header_footer_properties_attlist & attr = headerProp->style_header_footer_properties_attlist_;
        _CP_OPT(double) header;
		
		if (attr.fo_min_height_)	header = attr.fo_min_height_->get_value_unit(length::pt);
		else if (attr.svg_height_)	header = attr.svg_height_->get_value_unit(length::pt);
        
		Context.get_table_context().set_header_page(header);
    }

    if (footerProp)
    {
        const style_header_footer_properties_attlist & attr = footerProp->style_header_footer_properties_attlist_;
        _CP_OPT(double) footer;
		
		if (attr.fo_min_height_)	footer = attr.fo_min_height_->get_value_unit(length::pt);
		else if (attr.svg_height_)	footer = attr.svg_height_->get_value_unit(length::pt);
       
		Context.get_table_context().set_footer_page(footer);
    }

	style_page_layout_properties * props = properties();
	if (props)
		props->xlsx_serialize(strm, Context);   
}

void page_layout_instance::docx_serialize(std::wostream & strm, oox::docx_conversion_context & Context)
{
    const style_header_style * headerStyle = dynamic_cast<style_header_style *>(style_page_layout_->style_header_style_.get());
    const style_footer_style * footerStyle = dynamic_cast<style_footer_style *>(style_page_layout_->style_footer_style_.get());

	style_header_footer_properties * headerProp = headerStyle ? dynamic_cast<style_header_footer_properties *>(headerStyle->style_header_footer_properties_.get()) : NULL;
	style_header_footer_properties * footerProp = footerStyle ? dynamic_cast<style_header_footer_properties *>(footerStyle->style_header_footer_properties_.get()) : NULL;

	Context.get_header_footer_context().reset();

    if (headerProp)
    {
        const style_header_footer_properties_attlist & attr = headerProp->style_header_footer_properties_attlist_;
        _CP_OPT(length) top = attr.fo_min_height_ ? attr.fo_min_height_ : attr.svg_height_;
        Context.get_header_footer_context().set_header(top);
    }

    if (footerProp)
    {
        const style_header_footer_properties_attlist & attr = footerProp->style_header_footer_properties_attlist_;
        _CP_OPT(length) bottom = attr.fo_min_height_ ? attr.fo_min_height_ : attr.svg_height_;
        Context.get_header_footer_context().set_footer(bottom);
    }

	if ( style_page_layout_->style_page_usage_.get_type() == page_usage::Mirrored )
	{
		Context.set_settings_property(odf_reader::_property(L"mirrorMargins",true));
	}
	
	style_page_layout_properties * props = properties();
    if (props)
		props->docx_serialize(strm, Context);   
}
void page_layout_instance::pptx_serialize(std::wostream & strm, oox::pptx_conversion_context & Context)
{
	style_page_layout_properties * props = properties();
    if (props)
		props->pptx_serialize(strm, Context);
}

void page_layout_container::add_page_layout(const style_page_layout * StylePageLayout)
{
    page_layout_instance_ptr instance = page_layout_instance_ptr( new page_layout_instance(StylePageLayout) );
    instances_.push_back(instance);
    const int pos = static_cast<int>(instances_.size() - 1);
    
    page_layout_names_[ instance->name() ] = pos;

}

void page_layout_container::add_master_page(const std::wstring & StyleName, const std::wstring & PageLayoutName, style_master_page* MasterPage)
{
    master_page_names_array_.push_back(StyleName);
    master_page_names_[StyleName] = PageLayoutName;
    
    master_pages_.push_back(MasterPage);
    const int pos = static_cast<int>(master_pages_.size() - 1);
    master_page_names_2_[StyleName] = pos;
}

void page_layout_container::add_presentation_page_layout(const std::wstring & StyleName, style_presentation_page_layout* StylePageLayout)
{
	presentation_page_layouts_.push_back(StylePageLayout);
	
    const int pos = static_cast<int>(presentation_page_layouts_.size() - 1);
	presentation_page_layout_names_[ StyleName ] = pos;

}


const std::wstring page_layout_container::page_layout_name_by_style(const std::wstring & StyleName) const
{
    if (master_page_names_.count(StyleName) > 0)
        return master_page_names_.at(StyleName);
    return L"";
}

const page_layout_instance * page_layout_container::page_layout_by_style(const std::wstring & StyleName) const
{
    if (master_page_names_.count(StyleName) > 0)
        if (page_layout_names_.count(master_page_names_.at(StyleName)) )
            return instances_[ page_layout_names_.at( master_page_names_.at(StyleName) ) ].get();
    return NULL;
}

page_layout_instance * page_layout_container::page_layout_by_name(const std::wstring & Name) const
{
    if (page_layout_names_.count(Name))
        return instances_[ page_layout_names_.at( Name ) ].get();
    return NULL;
}

const page_layout_instance * page_layout_container::page_layout_first() const
{
    if (master_page_names_array_.empty())
        return NULL;

    return page_layout_by_style(master_page_names_array_[0]);
}
bool page_layout_container::compare_page_properties(const std::wstring & master1, const std::wstring & master2)
{
	const page_layout_instance *page_layout1 = page_layout_by_style(master1);
	const page_layout_instance *page_layout2 = page_layout_by_style(master2);

	if (!page_layout1 || !page_layout2) return true;
	if (!page_layout1->style_page_layout_ || !page_layout1->style_page_layout_) return true;

	style_page_layout_properties *props1 = dynamic_cast<style_page_layout_properties*>(page_layout1->style_page_layout_->style_page_layout_properties_.get());
	style_page_layout_properties *props2 = dynamic_cast<style_page_layout_properties*>(page_layout2->style_page_layout_->style_page_layout_properties_.get());
	
	if (!props1 || !props2) return true;

	if (props1 == props2) return true;

	return props1->attlist_.compare(props2->attlist_);
}

style_presentation_page_layout * page_layout_container::presentation_page_layout_by_name(const std::wstring & Name)
{
	style_presentation_page_layout * res = NULL;

    if (presentation_page_layout_names_.count(Name))
	{
		int ind = presentation_page_layout_names_.at( Name ) ;
        res = presentation_page_layouts_[ind];
	}
    
	return res;
}
style_master_page * page_layout_container::master_page_by_name(const std::wstring & Name)
{
	style_master_page * res = NULL;
   
	if (master_page_names_2_.count(Name))
        res = master_pages_[ master_page_names_2_.at( Name ) ];

	return res;
}

font_instance::font_instance( const std::wstring & StyleName,
                             const std::wstring & Name,
                             const std::wstring & Charset,
                             const std::wstring & Family,
                             const std::wstring & Pitch,
                             const std::wstring & AltName) : style_name_(StyleName), name_(Name), charset_(Charset), family_(Family), pitch_(Pitch), alt_name_(AltName)
{}

const std::wstring & font_instance::style_name() const 
{
    return style_name_;
}

const std::wstring & font_instance::name() const
{
	return name_;
}

const std::wstring & font_instance::charset() const
{
    return charset_;
}

const std::wstring & font_instance::family() const
{
    return family_;
}

const std::wstring & font_instance::pitch() const
{
    return pitch_;
}

const std::wstring & font_instance::alt_name() const
{
    return alt_name_;
}

font_instance * fonts_container::font_by_style_name(const std::wstring & StyleName)
{
    if (font_style_names_.count(StyleName) > 0)
        return instances_[font_style_names_.at(StyleName)].get();
    return NULL;
}

font_instance * fonts_container::font_by_name(const std::wstring & Name)
{
    if (font_names_.count(Name) > 0)
        return instances_[font_names_.at(Name)].get();
    return NULL;
}

void fonts_container::add_font( font_instance_ptr FontInstance ) 
{
    instances_.push_back(FontInstance);
    if (FontInstance)
    {
        font_style_names_[FontInstance->style_name()] = static_cast<int>(instances_.size() - 1);
        font_names_[FontInstance->name()] = static_cast<int>(instances_.size() - 1);
    }
}

void list_style_container::add_list_style(text_list_style * textListStyle)
{
    if (!textListStyle) return;
 
	instances_.push_back( list_style_instance_ptr(new list_style_instance(textListStyle)) );
	list_style_names_[ textListStyle->attr_.style_name_ ] = static_cast<int>(instances_.size() - 1);
}

void list_style_container::add_list_style(text_list_style * textListStyle, const std::wstring & NewName)
{
    if (!textListStyle) return;

	instances_.push_back( list_style_instance_ptr(new list_style_instance(textListStyle, NewName)) );
	list_style_names_[NewName] = static_cast<int>(instances_.size() - 1);
}
void list_style_container::add_outline_style(text_outline_style *textOutlineStyle)
{
    if (!textOutlineStyle) return;

	outline_ = textOutlineStyle;
	outline_id_ = instances_.size();
}
text_list_style * list_style_container::list_style_by_name(const std::wstring & Name)
{
    if (list_style_names_.count(Name) > 0)
        return instances_[list_style_names_.at(Name)]->get_text_list_style();
    return NULL;
}

text_outline_style * list_style_container::outline_style()
{
    return outline_;
}
int list_style_container::id_outline()
{
    return outline_id_ + 1;
}

int list_style_container::id_by_name(const std::wstring & Name)
{
    if (list_style_names_.count(Name) > 0)
        return list_style_names_.at(Name) + 1;
    else
        return 0;
}
const text_notes_configuration * notes_configuration::getConfiguration(odf_types::noteclass::type noteType) const
{
	if (type_to_name_.count(noteType))
		return type_to_name_.at(noteType);
	else
		return NULL;
}

void notes_configuration::add(odf_types::noteclass::type noteType, const text_notes_configuration * conf)
{
	type_to_name_[noteType] = conf; 
}

}
}
