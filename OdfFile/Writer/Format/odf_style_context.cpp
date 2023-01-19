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


#include "odf_style_context.h"
#include "ods_conversion_context.h"
//#include "odf_text_context.h" ??????

#include "logging.h"
#include "styles.h"

#include "style_paragraph_properties.h"
#include "style_text_properties.h"

#include <iostream>

namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {

static int style_family_counts_[1024]={};//согласно количеству разных стилей


void calc_paragraph_properties_content(std::vector<style_paragraph_properties*> & parProps, paragraph_format_properties * result)
{
	if (result == NULL)return;
    if (parProps.empty()) return;

    for (int i = (int)parProps.size() - 1; i >= 0; i--)
    {
        if (parProps[i])
            result->apply_from(parProps[i]->content_);
    }
}
void calc_text_properties_content(std::vector<style_text_properties*> & textProps, text_format_properties_content * result)
{
	if (result == NULL)return;
    if (textProps.empty()) return;

    for (int i = (int)textProps.size() - 1; i >= 0; i--)
    {
        if (textProps[i])
            result->apply_from(textProps[i]->content_);
    }
}
odf_style_context::odf_style_context()
{    
	//memset(style_family_counts_,0,sizeof(style_family_counts_)); 
}

void odf_style_context::set_odf_context(odf_conversion_context * Context)
{
	odf_context_ = Context;
	
	number_styles_context_.set_odf_context(Context);
	table_styles_context_.set_odf_context(Context);
	lists_styles_context_.set_odf_context(Context);
}


odf_style_state_ptr odf_style_context::last_state(style_family::type family)
{
	for (int i = (int)style_state_list_.size() - 1; i >= 0; i--)
	{
		if (style_state_list_[i]->get_family_type() == family || family == style_family::None )
		{
			return style_state_list_[i];
		}
	}
	return odf_style_state_ptr();
}
void odf_style_context::create_style(std::wstring oox_name, style_family::type family, bool automatic, bool root, int oox_id)
{
	std::wstring odf_name = oox_name;
	
	if (odf_name.empty()) odf_name = find_free_name(family);

	office_element_ptr elm;
	create_element(L"style", L"style", elm, odf_context_);
	
	odf_style_state_ptr state = boost::shared_ptr<odf_style_state>(new odf_style_state(elm, family));
	style_state_list_.push_back( state ); 

///////////////////////////////////////	
	style_state_list_.back()->set_name		(odf_name);
	style_state_list_.back()->set_automatic	(automatic);
	style_state_list_.back()->set_root		(root);

	style_state_list_.back()->style_oox_id_ = oox_id;

	style_family_counts_[(int)family]++;
}

void odf_style_context::add_style(office_element_ptr elm, bool automatic, bool root, style_family::type family)
{
	odf_style_state_ptr state = boost::shared_ptr<odf_style_state>(new odf_style_state(elm, family));

	style_state_list_.push_back( state); 
	
	style_state_list_.back()->set_automatic(automatic);
	style_state_list_.back()->set_root(root);

	style_family_counts_[(int)family]++;

}

void odf_style_context::create_default_style(style_family::type family)
{
	office_element_ptr elm;
	create_element(L"style", L"default-style", elm, odf_context_);
	
	odf_style_state_ptr state = boost::shared_ptr<odf_style_state>(new odf_style_state(elm, family));
	style_state_list_.push_back( state ); 

///////////////////////////////////////	
	style_state_list_.back()->set_automatic(false);
	style_state_list_.back()->set_root(true);
	style_state_list_.back()->set_default(true);
}
void odf_style_context::reset_defaults()
{
	current_default_styles_.clear();
}

void odf_style_context::process_automatic_for_styles(office_element_ptr root )
{//автоматические стили для эементнов стилей
	for (size_t i =0; i < style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->writable_ == false) continue;

		if (style_state_list_[i]->automatic_== true && style_state_list_[i]->root_== true && style_state_list_[i]->odf_style_)
			root->add_child_element(style_state_list_[i]->odf_style_);
	}
}
void odf_style_context::process_automatic_styles(office_element_ptr root )
{//автоматические стили для элементов
	for (size_t i =0; i < style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->writable_ == false) continue;

		if (/*it->automatic_== true && */style_state_list_[i]->root_== false && style_state_list_[i]->odf_style_)
			root->add_child_element(style_state_list_[i]->odf_style_);
	}
	lists_styles_context_.process_styles(root, true);
}

void odf_style_context::process_office_styles(office_element_ptr root )
{
	number_styles_context_.process_styles(root );
	
	for (size_t i =0; i < style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->writable_ == false) continue;

		if (style_state_list_[i]->automatic_== false && style_state_list_[i]->root_ == true && style_state_list_[i]->odf_style_)
			root->add_child_element(style_state_list_[i]->odf_style_);
	}
	
	lists_styles_context_.process_styles(root, false);
}
std::wstring odf_style_context::find_odf_style_name(int oox_id_style, style_family::type family, bool root, bool automatic)
{
	for (size_t i = 0; i < style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->odf_style_)
		{
			if (style_state_list_[i]->get_family_type() == family	 && 
				style_state_list_[i]->root_				== root		 &&
				style_state_list_[i]->automatic_		== automatic &&
				style_state_list_[i]->conditional_		== false)
			{
				if (oox_id_style >= 0 && style_state_list_[i]->style_oox_id_ == oox_id_style)	
					return style_state_list_[i]->get_name();
			}
				
		}
	}
	return L"";
}
//office_element_ptr odf_style_context::find_odf_style(int oox_id_style, style_family::type family, bool root, _CP_OPT(bool) automatic)
//{
//	//for (std::list<odf_style_state>::iterator it = style_state_list_.begin(); it != style_state_list_.end(); ++it)
//	for (size_t i=0;i<style_state_list_.size(); i++)
//	{
//		if (style_state_list_[i]->odf_style_)
//		{
//			if (style_state_list_[i]->get_family_type() == family && 
//				style_state_list_[i]->root_				== root &&
//				(!automatic || (automatic && style_state_list_[i]->automatic_ == *automatic)) &&
//				style_state_list_[i]->conditional_		== false)
//			{
//				if (oox_id_style >=0 && style_state_list_[i]->style_oox_id_ == oox_id_style)
//					return style_state_list_[i]->get_office_element();
//			}
//				
//		}
//	}
//	return office_element_ptr();
//}
std::wstring odf_style_context::find_conditional_style_name(int oox_id_style, style_family::type family)
{
	for (size_t i =0; i < style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->odf_style_)
		{
			if (style_state_list_[i]->get_family_type()== family 
						&& style_state_list_[i]->conditional_ == true)
			{
				if (oox_id_style >=0 && style_state_list_[i]->style_oox_id_ == oox_id_style)
					return style_state_list_[i]->get_name();
			}
				
		}
	}
	return L"";
}
office_element_ptr odf_style_context::find_conditional_style(int oox_id_style, style_family::type family)
{
	for (size_t i = 0; i < style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->odf_style_)
		{
			if (style_state_list_[i]->get_family_type() == family 
						&& style_state_list_[i]->conditional_ == true)
			{
				if (oox_id_style >=0 && style_state_list_[i]->style_oox_id_ == oox_id_style)
					return style_state_list_[i]->get_office_element();
			}
				
		}
	}
	return office_element_ptr();
}
bool odf_style_context::find_odf_style_state(int oox_id_style, style_family::type family, odf_style_state_ptr & state, bool root, bool automatic)
{
	for (size_t i = 0; i < style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->odf_style_)
		{
			if (style_state_list_[i]->get_family_type() == family		&& 
				style_state_list_[i]->root_				== root			&&
				style_state_list_[i]->automatic_		== automatic	&&
				style_state_list_[i]->conditional_		== false		&&
				style_state_list_[i]->default_			== false)
			{
				if (oox_id_style >= 0 && style_state_list_[i]->style_oox_id_ == oox_id_style)
				{
					state = style_state_list_[i];
					return true;
				}
			}
				
		}
	}
	return false;
}
bool odf_style_context::find_odf_default_style_state(style_family::type family, odf_style_state_ptr & state)
{
	for (size_t i=0;i<style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->default_ == false) continue;

		if (style_state_list_[i]->odf_style_ && style_state_list_[i]->get_family_type() == family)
		{
			state = style_state_list_[i];
			return true;
		}
	}
	return false;
}

bool odf_style_context::find_odf_style_state(std::wstring style_name, style_family::type family, odf_style_state_ptr & state)
{
	for (size_t i = 0; i < style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->odf_style_)
		{
			if (style_state_list_[i]->get_family_type() == family && style_state_list_[i]->get_name() == style_name)
			{
				state = style_state_list_[i];

				return true;
			}
				
		}
	}
	return false;
}

bool odf_style_context::find_odf_style(std::wstring style_name, style_family::type family, style *& style_)
{
	for (size_t i = 0; i < style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->odf_style_)
		{
			if (style_state_list_[i]->get_family_type() == family && style_state_list_[i]->get_name() == style_name)
			{
				style_ = dynamic_cast<style*>(style_state_list_[i]->odf_style_.get());

				return true;
			}
				
		}
	}
	return false;
}
office_element_ptr odf_style_context::find_odf_style_default(style_family::type family)
{
	for (size_t i=0; i < current_default_styles_.size(); i++)
	{
		if (current_default_styles_[i]->odf_style_)
		{
			if (current_default_styles_[i]->get_family_type() == family)	return current_default_styles_[i]->get_office_element();
		}
	}
	return office_element_ptr();
}
std::wstring odf_style_context::find_odf_style_name_default(style_family::type family)
{
	for (size_t i = 0; i < current_default_styles_.size(); i++)
	{
		if (current_default_styles_[i]->odf_style_)
		{
			if (current_default_styles_[i]->get_family_type() == family)
				return current_default_styles_[i]->get_name();
		}
	}
	return L"";
}
std::wstring odf_style_context::get_name_family(style_family::type family)
{
	switch(family)
	{
        case style_family::None:		return L"none";
		case style_family::Paragraph:	return L"P";
        case style_family::Text:		return L"T";
        case style_family::Section:		return L"Sect";
        case style_family::Table:		return L"ta";
        case style_family::TableColumn: return L"co";
        case style_family::TableRow:	return L"ro";
        case style_family::TableCell:	return L"ce";
        case style_family::TablePage:	return L"tbl";//??
        case style_family::Chart:		return L"ch";
        case style_family::Default:		return L"def";
        case style_family::DrawingPage: return L"dp";
        case style_family::Graphic:		return L"gr";
        case style_family::Presentation:return L"pr";
        case style_family::Control:		return L"ctrl";//??
        case style_family::Ruby:		return L"rb";//??
 
		case style_family::Gradient:	return L"Gradinent";
        case style_family::Hatch:		return L"Hatch";
		case style_family::Opacity:		return L"Opacity";
		case style_family::FillImage:	return L"FillImage";
		case style_family::Marker:		return L"Marker";
		case style_family::StokeDash:	return L"StokeDash";

		case style_family::PageLayout:				return L"Mpm";
		case style_family::MasterPage:				return L"MasterPage";
		case style_family::HandoutMaster:			return L"Handout";
		case style_family::LayerSet:				return L"";//нету
		case style_family::PresentationPageLayout:	return L"AL";
		case style_family::LineNumbering:			return L"Line_20_numbering";

	}
	return L"UnknownStyle";
}


std::wstring odf_style_context::find_free_name(style_family::type  family)
{
	std::wstring name = get_name_family(family);
	int count = style_family_counts_[(int)family];

	//доооолго .. проще хранить
	//for (size_t i=0;i<style_state_list_.size(); i++)
	//{
	//	if ((style_state_list_[i]->odf_style_) && (style_state_list_[i]->get_family_type() == family))
	//	{
	//		count++;
	//	}
	//}
	name = name + boost::lexical_cast<std::wstring>(count + 1);
	return name;
}
office_element_ptr & odf_style_context::add_or_find(std::wstring name, style_family::type family, bool automatic , bool root, int oox_id)
{
	for (size_t i = 0; i < style_state_list_.size(); i++)
	{
		if (style_state_list_[i]->odf_style_)
		{
			if (style_state_list_[i]->get_family_type() == family)
			{
				if (oox_id >= 0 && style_state_list_[i]->style_oox_id_ == oox_id)			
					return style_state_list_[i]->get_office_element();
				
				if ((name.length() >= 0 && style_state_list_[i]->odf_style_->get_name() == name)
					&& style_state_list_[i]->root_ == root)	return style_state_list_[i]->get_office_element();
				//во избежания имена генерим уникальные (в принципе для root и остальных - можно одинаковые)
			}
				
		}
	}
	create_style(name,family, automatic, root,oox_id);
	
	return style_state_list_.back()->get_office_element();
}
void odf_style_context::calc_text_properties(std::wstring style_name, odf_types::style_family::type family, text_format_properties_content * result)
{
    std::vector<style_text_properties*> textProps;
	
	while (!style_name.empty())
    {
		style *style_ = NULL;
		if (!find_odf_style(style_name, family, style_) || !style_)break;

       if (style_text_properties * textProp = style_->content_.get_style_text_properties())
			textProps.push_back(textProp);
        		
	   style_name = style_->style_parent_style_name_ ? *style_->style_parent_style_name_ : L"";

    }   
	calc_text_properties_content(textProps, result);
}

void odf_style_context::calc_paragraph_properties(std::wstring style_name, style_family::type family, paragraph_format_properties * result)
{
    std::vector<style_paragraph_properties*> parProps;
	
	while (!style_name.empty())
    {
		style *style_ = NULL;
		if (!find_odf_style(style_name, family, style_) || !style_)break;

       if (style_paragraph_properties * parProp = style_->content_.get_style_paragraph_properties())
			parProps.push_back(parProp);
        		
	   style_name = style_->style_parent_style_name_ ? *style_->style_parent_style_name_ : L"";

    }   
	calc_paragraph_properties_content(parProps, result);
}


}
}
