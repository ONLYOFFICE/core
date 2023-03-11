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

#include <boost/lexical_cast.hpp>
 
#include "office_spreadsheet.h"
#include "office_scripts.h"
#include "office_chart.h"

#include "office_elements_create.h"

#include "odf_conversion_context.h"
#include "odf_rels.h"

#include "styles.h"
#include "style_paragraph_properties.h"

#include "../../DesktopEditor/graphics/pro/Fonts.h"

#include "../../Common/utils.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

	namespace utils
	{

		void calculate_size_font_symbols(_font_metrix & metrix, NSFonts::IApplicationFonts *appFonts)
		{
			std::pair<float, float> appr = _graphics_utils_::calculate_size_symbol_asc(metrix.font_name, metrix.font_size, metrix.italic, metrix.bold, appFonts);

			if (appr.first < 0.01 || appr.second < 0.01)
			{
				appr.first = _graphics_utils_::calculate_size_symbol_win(metrix.font_name, metrix.font_size, false/*metrix.italic*/, false/*metrix.bold*/);
				appr.first = ((int)(appr.first + 0.5) + 2 * (int)appr.first) / 3.;
			}

			if (appr.first > 0)
			{
				//pixels to pt
				metrix.approx_symbol_size = appr.first;///1.1;//"1.2" волшебное число оО
				metrix.IsCalc = true;
			}

		}
	}
//////////////////////////////////////////////////////////////////////////////////////////////////

odf_conversion_context::odf_conversion_context(_office_type_document type_, package::odf_document * outputDocument) 
	: type (type_), chart_context_ (this), page_layout_context_(this), math_context_(this), font_metrix_()
{ 
	output_document_	= outputDocument;
	current_object_		= 0;

    applicationFonts_	= NSFonts::NSApplication::Create();

}
odf_conversion_context::~odf_conversion_context()
{
	output_document_ = NULL;

    if (applicationFonts_)
        delete applicationFonts_;
	
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		if (objects_[i]) delete objects_[i];
		objects_[i] = NULL;
	}
	objects_.clear();
}
void odf_conversion_context::set_fonts_directory(std::wstring pathFonts)
{
    if (applicationFonts_)
        applicationFonts_->InitializeFromFolder(pathFonts);
}
void odf_conversion_context::calculate_font_metrix(std::wstring name, double size, bool italic, bool bold)
{
	if (font_metrix_.IsCalc) return;

	if (size < 1)
		size = 12;

	font_metrix_.font_size = size;
	font_metrix_.italic = italic;
	font_metrix_.bold = bold;
	font_metrix_.font_name = name;

	////////////////////////////////////////////
	utils::calculate_size_font_symbols(font_metrix_, applicationFonts_);
}
double odf_conversion_context::convert_symbol_width(double val)
{
	//width = ((int)((column_width * Digit_Width + 5) / Digit_Width * 256 )) / 256.;
	//width = (int)(((256. * width + ((int)(128. / Digit_Width ))) / 256. ) * Digit_Width ); //in pixels
	//
	//_dxR = dxR / 1024. * width * 9525.;  // to emu

	val = ((int)((val * font_metrix_.approx_symbol_size + 5) / font_metrix_.approx_symbol_size * 256)) / 256.;

	double pixels = (int)(((256. * val + ((int)(128. / font_metrix_.approx_symbol_size))) / 256.) * font_metrix_.approx_symbol_size); //in pixels

	return pixels * 0.75; //* 9525. * 72.0 / (360000.0 * 2.54);
}

odf_style_context* odf_conversion_context::styles_context()
{
	if (!objects_.empty())
		return objects_[current_object_]->style_context.get();
	else
		return NULL;
}
odf_settings_context* odf_conversion_context::settings_context()
{
	if (!objects_.empty())
		return objects_[current_object_]->settings_context.get();
	else
		return NULL;
}

odf_page_layout_context* odf_conversion_context::page_layout_context()
{
	return &page_layout_context_;
}

odf_chart_context* odf_conversion_context::chart_context()
{
	return &chart_context_;
}

odf_math_context* odf_conversion_context::math_context()
{
	return &math_context_;
}

odf_number_styles_context* odf_conversion_context::numbers_styles_context()	
{
	if (objects_.size() > 0 && objects_[current_object_]->style_context)
			return &(objects_[current_object_]->style_context->numbers_styles());
	else return NULL;
}

_mediaitems* odf_conversion_context::mediaitems()				
{
	return &objects_[current_object_]->mediaitems;
}

void odf_conversion_context::end_document()
{
	rels rels_;
	for (size_t i = 0; i < objects_.size(); i++)
	{
		_object & object = *objects_[i];
		bool isRoot = (i == 0 ? true : false);
	
		if (object.content == NULL)continue;

		process_styles	(object, isRoot);
		process_settings(object, isRoot);

		package::content_content_ptr content_root_ = package::content_content::create();		

		if (objects_.back()->scripts)
			objects_.back()->scripts->serialize(content_root_->styles());	

		object.content->serialize(content_root_->content());	

		for (size_t i = 0; i < object.content_styles.size(); i++)
		{
			object.content_styles[i]->serialize(content_root_->styles());
		}
		package::content_simple_ptr content_style_ = package::content_simple::create();
		for (size_t i = 0; i < object.styles.size(); i++)
		{// мастер-пейджы, заданные заливки (градиенты, битмапы), дефолтные стили, колонтитулы, разметки, заметки,...
			object.styles[i]->serialize(content_style_->content());
		}
		package::content_simple_ptr content_settings_ = package::content_simple::create();
		object.settings->serialize(content_settings_->content());
////////////////////////////
		package::object_files *object_files =  new package::object_files();
		if (object_files)
		{
			object_files->set_content	(content_root_, object.content_ext);
			object_files->set_styles	(content_style_);
			object_files->set_mediaitems(object.mediaitems);
			object_files->set_settings	(content_settings_);
			
			if (!isRoot)object_files->local_path = object.name + L"/";
			
			object.mediaitems.dump_rels(rels_, object_files->local_path);

			rels_.add(relationship(std::wstring(L"text/xml"), object_files->local_path + L"styles.xml"));
			rels_.add(relationship(std::wstring(L"text/xml"), object_files->local_path + L"content.xml"));
			rels_.add(relationship(std::wstring(L"text/xml"), object_files->local_path + L"meta.xml"));
			rels_.add(relationship(std::wstring(L"text/xml"), object_files->local_path + L"settings.xml"));

			if (isRoot)
			{
				object_files->local_path = L"/";
				rels_.add(relationship(std::wstring(L"application/vnd.oasis.opendocument.") + output_document_->get_type(), object_files->local_path));
			}
			else
			{
				switch (object.content->get_type())
				{
				case typeOfficeText:
					rels_.add(relationship(std::wstring(L"application/vnd.oasis.opendocument.text"), object_files->local_path)); break;
				case typeOfficeSpreadsheet:
					rels_.add(relationship(std::wstring(L"application/vnd.oasis.opendocument.spreadsheet"), object_files->local_path)); break;
				case typeOfficePresentation:
					rels_.add(relationship(std::wstring(L"application/vnd.oasis.opendocument.presentation"), object_files->local_path)); break;
				case typeOfficeChart:
					rels_.add(relationship(std::wstring(L"application/vnd.oasis.opendocument.chart"), object_files->local_path)); break;
				case typeMath:
					rels_.add(relationship(std::wstring(L"application/vnd.oasis.opendocument.formula"), object_files->local_path)); break;
				default:
					rels_.add(relationship(std::wstring(L"application/vnd.oasis.opendocument.") + object.content->get_name(), object_files->local_path));
					break;
				}
			}

			output_document_->add_object(package::element_ptr(object_files), isRoot);
		}
	}
	output_document_->set_rels(rels_);
}
void odf_conversion_context::start_chart()
{
	create_object();
	create_element(L"office", L"chart", objects_.back()->content, this, true);

	chart_context_.set_styles_context(odf_conversion_context::styles_context());
	chart_context_.start_chart(get_current_object_element());
}
void odf_conversion_context::start_spreadsheet()
{
	create_object();
	create_element(L"office", L"spreadsheet", objects_.back()->content, this, true);
}
void odf_conversion_context::start_text()
{
	create_object();
	create_element(L"office", L"text", objects_.back()->content, this, true);
}
void odf_conversion_context::start_presentation()
{
	create_object();
	create_element(L"office", L"presentation", objects_.back()->content, this, true);

	create_element(L"office", L"scripts", objects_.back()->scripts, this);
}
void odf_conversion_context::create_object(bool bAddContentExt)
{
	_object *obj = new _object();

	if (obj)
	{
		obj->content_ext = bAddContentExt;
		obj->style_context = boost::make_shared<odf_style_context>();
		obj->settings_context = boost::make_shared<odf_settings_context>();

		obj->name = get_next_name_object();

		obj->style_context->set_odf_context(this);
		obj->settings_context->set_odf_context(this);

		objects_.push_back(obj);

		current_object_ = objects_.size() - 1;
	}
}
void odf_conversion_context::end_chart()
{
	chart_context_.end_chart();

	end_object();
	chart_context_.set_styles_context(styles_context());
}
void odf_conversion_context::start_text_context()
{
	odf_text_context_ptr new_text_context_ = boost::shared_ptr<odf_text_context>(new odf_text_context(this, styles_context()));
	if (!new_text_context_)return;

	text_context_.push_back(new_text_context_);
}
void odf_conversion_context::end_text_context()
{
	if (false == text_context_.empty())
	{
		text_context_.pop_back();
	}
}
void odf_conversion_context::start_drawing_context()
{
	odf_drawing_context_ptr new_drawing_context_ = boost::shared_ptr<odf_drawing_context>(new odf_drawing_context(this));
	if (!new_drawing_context_)return;

	new_drawing_context_->set_styles_context(styles_context());

	drawing_context_.push_back(new_drawing_context_);
}
void odf_conversion_context::end_drawing_context()
{
	if (drawing_context_.empty()) return;

	office_element_ptr & elm = drawing_context()->get_root_element();

	if (elm)
	{
		text_context()->start_element(elm);
		text_context()->end_element();
	}

	drawing_context()->clear();
	drawing_context_.pop_back();
}

bool odf_conversion_context::start_math()
{
	if (false == math_context_.isEmpty()) return false;

	if (false == math_context_.in_text_box_)
	{
		drawing_context()->start_drawing();
		drawing_context()->set_anchor(anchor_type::AsChar);
	}
	drawing_context()->start_object(get_next_name_object(), !math_context_.in_text_box_); 
			//имитация рисованного объекта - высота-ширина ????

	create_object(false);
	create_element(L"math", L"math", objects_.back()->content, this, true);
	
	math_context_.set_styles_context(odf_conversion_context::styles_context());
	math_context_.start_math(get_current_object_element());

	return true;
}
void odf_conversion_context::end_math()
{
	math_context_.end_math();

	end_object();
	math_context_.set_styles_context(styles_context());
	
	calculate_font_metrix(L"Cambria Math", 12, false, false); // смотреть по формуле - перевычислять только если есть изменения
	
	int count_symbol_height = 3; //сосчитать в math_context_
	int count_symbol_width = 10;

	_CP_OPT(double)width = convert_symbol_width(count_symbol_width);
	_CP_OPT(double)height = convert_symbol_width(count_symbol_height);

	//if (false == math_context_.in_text_box_)
	//	drawing_context()->set_size(width, height); 
	
	drawing_context()->end_object(!math_context_.in_text_box_);

	if (false == math_context_.in_text_box_)
	{
		drawing_context()->end_drawing();
	}
}
void odf_conversion_context::end_text()
{
	end_object();
}
void odf_conversion_context::end_spreadsheet()
{
	end_object();
}
void odf_conversion_context::end_presentation()
{
	end_object();
}
void odf_conversion_context::end_object()
{
	current_object_ = 0;//main
}

office_element_ptr & odf_conversion_context::get_current_object_element()
{
	if (objects_.empty())
	{
		create_object();
	}

	return objects_[current_object_]->content;
}

std::wstring odf_conversion_context::get_next_name_object()
{
	return std::wstring(L"Object ") + boost::lexical_cast<std::wstring>(objects_.size());
}
void odf_conversion_context::process_settings(_object & object, bool isRoot)
{
	create_element(L"office", L"settings", object.settings, this, true);
	
	object.settings_context->process_office_settings(object.settings);
	
	if (isRoot)
	{	
	}
}

void odf_conversion_context::process_styles(_object & object, bool isRoot)
{		
	create_element(L"office", L"font-face-decls", object.content_styles, this, true);

	for (boost::unordered_map<std::wstring, int>::const_iterator it = object.mapFonts.begin(); it != object.mapFonts.end(); ++it)
	{
		office_element_ptr font_elm;
		create_element(L"style", L"font-face", font_elm, this, true);

		style_font_face* font = dynamic_cast<style_font_face*>(font_elm.get());
		
		if (font)
		{
			font->style_name_		= it->first;
			font->svg_font_family_	= it->first;

			object.content_styles.back()->add_child_element(font_elm);
		}
	}

	object.styles.push_back(object.content_styles.back()); //копия

	create_element(L"office", L"styles", object.styles, this, true);//общие стили
	
	object.style_context->process_office_styles(object.styles.back());
	page_layout_context()->process_office_styles(object.styles.back());

	if (isRoot)
	{	
		create_element(L"office", L"automatic-styles", object.styles, this, true);
		object.style_context->process_automatic_for_styles(object.styles.back());
		page_layout_context()->process_automatic_for_styles(object.styles.back());

		create_element(L"office", L"master-styles", object.styles, this, true);
		page_layout_context()->process_master_styles(object.styles.back());	
	}


	create_element(L"office", L"automatic-styles", object.content_styles, this, true);
	object.style_context->process_automatic_styles(object.content_styles.back());
}

office_element_ptr odf_conversion_context::start_tabs()
{
	create_element(L"style", L"tab-stops", temporary_.elm, this, true);
	return temporary_.elm;
}
std::wstring odf_conversion_context::add_image(const std::wstring & image_file_name, bool bExternal)
{
	if (image_file_name.empty()) return L"";
	
	if (bExternal)
	{
		return image_file_name;
	}
	else
	{
		std::wstring odf_ref_name;
		mediaitems()->add_or_find(image_file_name, _mediaitems::typeImage, odf_ref_name);

		return odf_ref_name;
	}
}
std::wstring odf_conversion_context::add_media(const std::wstring & file_name, bool bExternal)
{
	if (file_name.empty()) return L"";
	
	if (bExternal)
	{
		return file_name;
	}
	else
	{
		std::wstring odf_ref_name;
		mediaitems()->add_or_find(file_name, _mediaitems::typeMedia, odf_ref_name);

		return odf_ref_name;
	}
}
std::wstring odf_conversion_context::add_imageobject(const std::wstring & file_name)
{
	if (file_name.empty()) return L"";
	
	std::wstring odf_ref_name ;	
	mediaitems()->add_or_find(file_name,_mediaitems::typeObjectReplacement, odf_ref_name);

	return odf_ref_name;
}
std::wstring odf_conversion_context::add_oleobject(const std::wstring & file_name)
{
	if (file_name.empty()) return L"";
	
	std::wstring odf_ref_name ;	
	mediaitems()->add_or_find(file_name,_mediaitems::typeOleObject, odf_ref_name);

	return odf_ref_name;
}
void odf_conversion_context::add_tab(_CP_OPT(int) type, _CP_OPT(length) _length, _CP_OPT(int) leader)
{
	if (!temporary_.elm) return;

	office_element_ptr elm;
	create_element(L"style", L"tab-stop", elm, this, true);

	style_tab_stop* tab = dynamic_cast<style_tab_stop*>(elm.get());
	if (tab)
	{
		if (_length)
			tab->style_position_ = length(_length->get_value_unit(length::cm), length::cm);

		if (type)
		{
			switch(*type)
			{
				case 0:	tab->style_type_ = style_type::Left;	break;//tabjcBar     = 0,
				case 1: tab->style_type_ = style_type::Center;	break;//tabjcCenter  = 1,
				case 2: tab->style_type_ = style_type::Left;	break;//tabjcClear   = 2,
				case 3: tab->style_type_ = style_type::Left;	break;//tabjcDecimal = 3,
				case 4: tab->style_type_ = style_type::Right;	break;//tabjcEnd     = 4,
				case 5: tab->style_type_ = style_type::Left;	break;//tabjcNum     = 5,
				case 6: tab->style_type_ = style_type::Left;	break;//tabjcStart   = 6,
				case 7: tab->style_type_ = style_type::Right;	break;//tabjcRight   = 7,
				case 8: tab->style_type_ = style_type::Left;	break;//tabjcLeft    = 8
			}
		}
		if (leader)
		{
			switch(*leader)
			{
			case 0: tab->style_leader_type_	= line_type::Single ; 
					tab->style_leader_style_= line_style::Dotted;
				break; //	tabtlcDot        = 0,
			case 1: tab->style_leader_type_ = line_type::Single ; 
					tab->style_leader_style_= line_style::Wave;
				break; //	tabtlcHeavy      = 1,
			case 2: tab->style_leader_type_ = line_type::Single ; 
					tab->style_leader_style_= line_style::Dash;
				break; //	tabtlcHyphen     = 2,
			case 3: tab->style_leader_type_ = line_type::Single ; 
					tab->style_leader_style_= line_style::LongDash;
				break; //	tabtlcMiddleDot  = 3,
            case 4: tab->style_leader_type_ = line_type::None;
				break; //	tabtlcNone       = 4,
			case 5: tab->style_leader_type_ = line_type::Single; 
					tab->style_leader_style_= line_style::Solid;
				break; //	tabtlcUnderscore = 5
			}
		}
		temporary_.elm->add_child_element(elm);
	}
}

void odf_conversion_context::end_tabs()
{
	temporary_.elm			= office_element_ptr();
	temporary_.style_elm	= office_element_ptr();
	temporary_.style_name	= L"";

}
void odf_conversion_context::add_font(const std::wstring& font_name)
{
	if (objects_.empty())return;

	if (objects_[current_object_]->mapFonts.find(font_name) == objects_[current_object_]->mapFonts.end())
	{
		objects_[current_object_]->mapFonts.insert(std::make_pair(font_name, 1));
	}
	else
	{
		objects_[current_object_]->mapFonts[font_name]++;
	}
}
}
}
