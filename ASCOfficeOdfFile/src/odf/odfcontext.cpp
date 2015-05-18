
#include "odfcontext.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

namespace cpdoccore { 
namespace odf {

style_instance::style_instance(
    styles_container * Container,
    const std::wstring & Name,
    style_family::type Type,
    style_content * Content,
    bool IsAutomatic,
    bool IsDefault,
    const std::wstring & ParentStyleName,
    const std::wstring & NextStyleName,
    const std::wstring & DataStyleName
    ) : container_(Container),
    name_(Name),
    style_type_(Type),
    content_(Content),
    is_automatic_(IsAutomatic),
    is_default_(IsDefault),
    parent_name_(ParentStyleName),
    next_name_(NextStyleName),
    parent_(Container->style_by_name(ParentStyleName, style_type_,false)),//????
    next_(Container->style_by_name(NextStyleName, style_type_,false)),//?????
    data_style_name_(DataStyleName)
{
}

std::wostream & style_instance::dbg_dump(std::wostream & _Wostream)
{
    _Wostream << L"[STYLE]\n";
    _Wostream << L"\tname: '" << name() << L"'\n";
    _Wostream << L"\ttype: '" << style_family(type()) << L"'\n";
    _Wostream << L"\tcontent: '" << content() << L"'\n";
    _Wostream << L"\tparent: '" << parent() << L"'\n";
    _Wostream << L"\tparent_name: '" << parent_name() << L"'\n";
    _Wostream << L"\tnext: '" << next() << L"'\n";
    _Wostream << L"\tnext_name: '" << next_name() << L"'\n";
    _Wostream << L"\tauto: '" << is_automatic() << L"'\n";
    _Wostream << L"\tdefault: '" << is_default() << L"'\n";
    _Wostream << L"\n";

    return _Wostream;
    
}

std::wostream & styles_container::dbg_dump(std::wostream & _Wostream)
{
    BOOST_FOREACH(style_instance_ptr & elm, instances_)
    {
        elm->dbg_dump(_Wostream);        
    }
    return _Wostream;
}
    
style_instance * styles_container::hyperlink_style()
{
    if (hyperlink_style_pos_ > 0)
        return instances_[hyperlink_style_pos_].get();
    else
        return NULL;
}

void styles_container::add_style(const std::wstring & Name,
               style_family::type Type,
               style_content * Content,
               bool IsAutomatic,
               bool IsDefault,
               const std::wstring & ParentStyleName_,
               const std::wstring & NextStyleName,
               const std::wstring & DataStyleName)
{
	std::wstring ParentStyleName = ParentStyleName_;

	if (Name == ParentStyleName)
	{
		ParentStyleName = L"";//иначе в коде возможно зацикливание.
	}
    style_instance_ptr newStyle = style_instance_ptr( 
        new style_instance(this, Name, Type, Content, IsAutomatic, IsDefault, ParentStyleName, NextStyleName, DataStyleName) 
        );

    instances_.push_back(newStyle);
    int pos = static_cast<int>(instances_.size() - 1);
    
    if (!Name.empty())
    {
        std::wstring n = Name + L":" + boost::lexical_cast<std::wstring>( style_family(Type) );
        map_[n] = pos;

        // TODO: как правильно??
        std::wstring lName = Name;
        boost::algorithm::to_lower(lName);
        //if ( boost::algorithm::contains(lName, L"internet_20_link") )
        if (lName == L"internet_20_link")///???????????????
            hyperlink_style_pos_ = pos;

    }

    if (IsDefault)
        default_map_[Type] = pos;
        
}

const std::wstring & style_instance::name() const
{
    return name_;
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
        parent_ = container_->style_by_name(parent_name_, type(),false);
    
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
    else if (container_)
        next_ = container_->style_by_name(next_name_, type(),false);
    
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

const std::wstring & style_instance::data_style_name() const
{
    return data_style_name_;
}

style_instance * styles_container::style_by_name(const std::wstring & Name, style_family::type Type,bool object_in_styles) const
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
	int index =0;
	for (index=0;index<content.size();index++)
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
		item.rId = std::wstring(L"lrId") + boost::lexical_cast<std::wstring>(item.Id);
	
		content.push_back(item);
		index = content.size()-1;

	}
	return std::pair<int,std::wstring>(content[index].Id,content[index].rId);
}

std::pair<int,std::wstring> presentation_masters_instance::add_or_find(const std::wstring & master_name)
{
	bool find = false;
	int index =0;
	for (index=0;index<content.size();index++)
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
		item.rId = std::wstring(L"smId") + boost::lexical_cast<std::wstring>(item.Id);
	
		content.push_back(item);
		index = content.size()-1;

	}
	return std::pair<int,std::wstring>(content[index].Id,content[index].rId);
}

void presentation_masters_instance::add_layout_to(const std::wstring & master_name, presentation_layouts_instance::_layout & layout)
{
	bool find = false;
	int index =0;
	for (index=0;index<content.size();index++)
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

const std::wstring styles_container::master_page_name_by_name(const std::wstring & StyleName) const
{
    map_wstring_wstring::const_iterator res = master_page_name_.find(StyleName);
    if (res != master_page_name_.end())
        return res->second;
    return L"";
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
    return style_page_layout_->style_page_layout_attlist_.get_style_name();
}

style_page_layout_properties * page_layout_instance::properties() const
{    
    return dynamic_cast<style_page_layout_properties *>(style_page_layout_->style_page_layout_properties_.get());
}

void page_layout_instance::docx_convert_serialize(std::wostream & strm, oox::docx_conversion_context & Context)
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

    properties()->docx_convert_serialize(strm, Context);   
}
void page_layout_instance::pptx_convert(oox::pptx_conversion_context & Context)
{
    properties()->pptx_convert(Context);   
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
    static const std::wstring Symbol = L"Wingdings";
    if (name_ == L"StarSymbol")
        return Symbol;
    else
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
    if (textListStyle)
    {
        instances_.push_back( list_style_instance_ptr(new list_style_instance(textListStyle)) );
        list_style_names_[ textListStyle->get_style_name() ] = static_cast<int>(instances_.size() - 1);
    }
}

void list_style_container::add_list_style(text_list_style * textListStyle, 
                                          const std::wstring & NewName)
{
    if (textListStyle)
    {
        instances_.push_back( list_style_instance_ptr(new list_style_instance(textListStyle, NewName)) );
        list_style_names_[NewName] = static_cast<int>(instances_.size() - 1);
    }
}

text_list_style * list_style_container::list_style_by_name(const std::wstring & Name)
{
    if (list_style_names_.count(Name) > 0)
        return instances_[list_style_names_.at(Name)]->get_text_list_style();
    return NULL;
}

int list_style_container::id_by_name(const std::wstring & Name)
{
    if (list_style_names_.count(Name) > 0)
        return list_style_names_.at(Name) + 1;
    else
        return 0;
}

}
}
