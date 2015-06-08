

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
 
#include "office_spreadsheet.h"
#include "office_chart.h"

#include "office_elements_create.h"

#include "odf_conversion_context.h"
#include "odf_rels.h"

#include "style_paragraph_properties.h"

#include "../../DesktopEditor/fontengine/ApplicationFonts.h"


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

//////////////////////////////////////////////////////////////////////////////////////////////////

odf_conversion_context::odf_conversion_context(package::odf_document * outputDocument) : chart_context_ (this), page_layout_context_(this)
{ 
	output_document_ = outputDocument;

	current_object_ =0;

    applicationFonts_ = new CApplicationFonts();

}
odf_conversion_context::~odf_conversion_context()
{
    if (applicationFonts_)
        delete applicationFonts_;
}

void odf_conversion_context::set_fonts_directory(std::wstring pathFonts)
{
    if (applicationFonts_)
        applicationFonts_->InitializeFromFolder(pathFonts);
}

odf_style_context* odf_conversion_context::styles_context()
{
	if (objects_.size() > 0)
		return objects_[current_object_].style_context.get();
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

odf_number_styles_context* odf_conversion_context::numbers_styles_context()	
{
	if (objects_.size() > 0 && objects_[current_object_].style_context)
			return &(objects_[current_object_].style_context->numbers_styles());
	else return NULL;
}

_mediaitems* odf_conversion_context::mediaitems()				
{
	return &objects_[current_object_].mediaitems;
}

void odf_conversion_context::end_document()
{
	rels	rels_;
	for (long i=0; i< objects_.size(); i++)
	{
		_object & object = objects_[i];
		bool isRoot = (i==0? true : false);
	
		if (object.content == NULL)continue;

		process_styles(object,isRoot);

		package::content_content_ptr content_root_ = package::content_content::create();		
		object.content->serialize(content_root_->content());		
		BOOST_FOREACH(const office_element_ptr & elm, object.content_styles)
		{
			elm->serialize(content_root_->styles());
		}

		package::content_simple_ptr content_style_ = package::content_simple::create();
		BOOST_FOREACH(const office_element_ptr & elm, object.styles)
		{// мастер-пейджы, заданные заливки (градиенты, битмапы), дефолтные стили, колонтитулы, разметки, заметки,...

			elm->serialize(content_style_->content());
		}
////////////////////////////
		package::object_files *object_files =  new package::object_files();
		if (object_files)
		{
			object_files->set_content	(content_root_);
			object_files->set_styles	(content_style_);
			object_files->set_media		(object.mediaitems);
			object_files->set_pictures	(object.mediaitems);

			if (!isRoot)object_files->local_path = object.name + L"/";
			
			object.mediaitems.dump_rels(rels_,object_files->local_path);

			rels_.add(relationship(std::wstring(L"text/xml"), object_files->local_path + L"styles.xml"));
			rels_.add(relationship(std::wstring(L"text/xml"), object_files->local_path + L"content.xml"));
			rels_.add(relationship(std::wstring(L"text/xml"), object_files->local_path + L"meta.xml"));

			if (isRoot)object_files->local_path = L"/";
			rels_.add(relationship(std::wstring(L"application/vnd.oasis.opendocument.") + object.content->get_name(), object_files->local_path));

			output_document_->add_object(package::element_ptr(object_files),isRoot);
		}

	}

	output_document_->set_rels(rels_);
}
void odf_conversion_context::start_chart()
{
	create_object();
	create_element(L"office", L"chart", objects_.back().content, this,true);

	chart_context_.set_styles_context(styles_context());
	chart_context_.start_chart(get_current_object_element());
}
void odf_conversion_context::start_spreadsheet()
{
	create_object();
	create_element(L"office", L"spreadsheet", objects_.back().content, this,true);
}
void odf_conversion_context::start_text()
{
	create_object();
	create_element(L"office", L"text", objects_.back().content, this,true);
}
void odf_conversion_context::create_object()
{
	_object obj;
	
	obj.style_context = boost::make_shared<odf_style_context>();
	obj.style_context->set_odf_context(this);
	obj.name = get_next_name_object();
	
	objects_.push_back(obj);

	current_object_ = objects_.size()-1;
}
void odf_conversion_context::end_chart()
{
	chart_context_.end_chart();

	end_object();
	chart_context_.set_styles_context(styles_context());
}
void odf_conversion_context::end_text()
{
	end_object();
}
void odf_conversion_context::end_spreadsheet()
{
	end_object();
}
void odf_conversion_context::end_object()
{
	current_object_ = 0;
}

office_element_ptr & odf_conversion_context::get_current_object_element()
{
	return objects_[current_object_].content;
}

std::wstring odf_conversion_context::get_next_name_object()
{
	return std::wstring(L"Object ") + boost::lexical_cast<std::wstring>(objects_.size());
}

void odf_conversion_context::process_styles(_object & object, bool isRoot)
{
	create_element(L"office", L"styles", object.styles, this,true);//общие стили
	object.style_context->process_office_styles(object.styles.back());
	page_layout_context()->process_office_styles(object.styles.back());
	
	if (isRoot)
	{	
		create_element(L"office", L"font-face-decls", object.styles, this,true);
		
		create_element(L"office", L"automatic-styles", object.styles, this,true);
		object.style_context->process_automatic_for_styles(object.styles.back());
		page_layout_context()->process_automatic_for_styles(object.styles.back());

		create_element(L"office", L"master-styles", object.styles, this,true);
		page_layout_context()->process_master_styles(object.styles.back());
		
		create_element(L"office", L"font-face-decls", object.content_styles, this,true);
	}

	create_element(L"office", L"automatic-styles", object.content_styles, this,true);
	object.style_context->process_automatic_styles(object.content_styles.back());

}
office_element_ptr odf_conversion_context::start_tabs()
{
	create_element(L"style", L"tab-stops", temporary_.elm, this,true);
	return temporary_.elm;
}

void odf_conversion_context::add_tab(_CP_OPT(int) type, _CP_OPT(length) _length, _CP_OPT(int) leader)
{
	if (!temporary_.elm) return;

	office_element_ptr elm;
	create_element(L"style", L"tab-stop", elm, this,true);

	style_tab_stop* tab = dynamic_cast<style_tab_stop*>(elm.get());
	if (tab)
	{
		if (_length)
			tab->style_position_ = length(_length->get_value_unit(length::cm),length::cm);

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
}
}
