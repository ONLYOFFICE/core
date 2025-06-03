/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include <iostream>

#include <boost/algorithm/string.hpp>
#include <xml/simple_xml_writer.h>
#include <xml/utils.h>

#include "pptx_drawing.h"
#include "pptx_drawings.h"
#include "pptx_slide_context.h"

#include "../Format/odf_document.h"
#include "../Format/draw_common.h"

#include "drawing_object_description.h"

namespace cpdoccore {
namespace oox {

class pptx_drawings;
typedef _CP_PTR(pptx_drawings) pptx_drawings_ptr;

class pptx_slide_context::Impl
{
public:  
	struct _transition
	{
		bool					Enabled;
		std::wstring			Type;

		_CP_OPT(std::wstring)	Speed;
		_CP_OPT(int)			Time;
		_CP_OPT(int)			PageTime;
		_CP_OPT(std::wstring)	Dir;
		_CP_OPT(std::wstring)	Param;
		bool					onClick;
	};

	Impl(const std::wstring & _odfPacket) : odfPacket_(_odfPacket)
    {
		mediaitems_ = boost::make_shared<mediaitems>(odfPacket_);

		clear();
	}

    drawing_object_description				object_description_;
	std::vector<drawing_object_description> objects_;

	_CP_OPT(_oox_fill)						background_fill_;

	_transition								transition_;
	bool									use_image_replacement_;
	bool									processing_notes;

	bool header, footer, date_time, slideNum;

	// NOTE: Key - ODF draw:id(xml:id), value - OOX ShapeID
	// NOTE: draw:id is deprecated. Instead prefer using xml:id. 
	// draw:id and xml:id MUST be equal to each other
	typedef std::unordered_map<std::wstring, size_t> id_map;
	id_map id_map_;

    void add_drawing(_pptx_drawing const & d, bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type)
    {
        pptx_drawings_->add(d, isInternal, rid, ref, type);
    }
	void add_additional_rels (bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type)
    {
        pptx_drawings_->add(isInternal, rid, ref, type);
    }
	mediaitems_ptr & get_mediaitems() { return mediaitems_; }

    bool empty() const
    {
        return pptx_drawings_->empty();
    }

	void clear()
	{
		objects_.clear();
		id_map_.clear();
		
		background_fill_		= boost::none;

		header		= false;
		footer		= false;
		date_time	= false;
		slideNum	= false;
		
		// NOTE: In PowerPoint p:nvGrpSpPr MUST have id = 1
		// Otherwise animation are still working correctly but other element's IDs don't match to its animations in animation editor of PowerPoint
		// Reserve "id = 1" for p:nvGrpSpPr
		rId_ = 2;

		pptx_drawings_			= pptx_drawings::create();

		transition_.Enabled = false;
		transition_.Speed	= boost::none;
		transition_.onClick	= true;	

		processing_notes = false;
	}

    size_t next_rId()
    {
		return rId_++;;
    }
    pptx_drawings_ptr get_drawings()
    {
        return pptx_drawings_;
    }
	size_t generate_id(const std::wstring& id)
	{
		size_t result = next_rId();
		id_map_.insert(std::make_pair(id, result));

		return result;
	}
	size_t get_id(const std::wstring& id)
	{
		if (id == L"")
			return next_rId();

		id_map::iterator it = id_map_.find(id);
		if (it == id_map_.end())
			return generate_id(id);

		return it->second;
	}
	size_t find_id(const std::wstring& id)
	{
		id_map::iterator it = id_map_.find(id);
		if (it == id_map_.end())
			return 0;

		return it->second;
	}
	
	std::wstring  odfPacket_;
    
	void process_drawings();

private:
	void process_common_properties(drawing_object_description& obj, _pptx_drawing & drawing);
	void process_crop(const drawing_object_description& obj, _pptx_drawing& drawing, const std::wstring& filename);

	void process_shape	(drawing_object_description& obj, _pptx_drawing & drawing);
    void process_image	(drawing_object_description& obj, _pptx_drawing & drawing);
	void process_chart	(drawing_object_description& obj, _pptx_drawing & drawing);
	void process_table	(drawing_object_description& obj, _pptx_drawing & drawing);
	void process_object	(drawing_object_description& obj, _pptx_drawing & drawing);
	void process_media	(drawing_object_description& obj, _pptx_drawing & drawing);
	
	size_t				rId_;
	mediaitems_ptr		mediaitems_;
    pptx_drawings_ptr	pptx_drawings_;
    pptx_drawings_ptr	pptx_notes_drawings_;
};

void pptx_slide_context::Impl::process_drawings()
{
	for (size_t i = 0; i < objects_.size(); i++)
	{
		_pptx_drawing drawing = _pptx_drawing();

		drawing.type		= objects_[i].type_;			
		drawing.name		= objects_[i].name_;
		drawing.id			= get_id(objects_[i].xml_id_);	
		drawing.lined		= objects_[i].lined_;
		drawing.connector	= objects_[i].connector_;
		
		process_common_properties(objects_[i], drawing);
		
		switch(objects_[i].type_)
		{
			case typeImage:			process_image(objects_[i], drawing);	break;
			case typeChart:			process_chart(objects_[i], drawing);	break;
			case typeShape:			process_shape(objects_[i], drawing);	break;
			case typeTable:			process_table(objects_[i], drawing);	break;
			case typeMedia:			process_media(objects_[i], drawing);	break;
			case typePDF:
			case typeMsObject:	
			case typeOleObject:		process_object(objects_[i], drawing);	break;
		}
	}

	for (size_t i = 0; i < pptx_drawings_->get_drawings().size(); i++)
	{
		_pptx_drawing& drawing = pptx_drawings_->get_drawings()[i];

		if(!drawing.connector)
			continue;

		drawing.start_connection_shape_id = std::to_wstring(find_id(drawing.start_connection_shape_id));
		drawing.end_connection_shape_id = std::to_wstring(find_id(drawing.end_connection_shape_id));
	}
}

pptx_slide_context::pptx_slide_context(pptx_conversion_context & Context)
	: impl_(new pptx_slide_context::Impl(Context.root()->get_folder())),
	pptx_animation_context_()
{    
	hlinks_size_	= 0;
}
void pptx_slide_context::start_slide()
{    
	impl_->clear(); // objects
	
	hlinks_size_	= 0;
	
	default_set(); //for current object
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void pptx_slide_context::start_slide_animation()
{    
	impl_->transition_.Enabled = true;

//default
	impl_->transition_.Type			= L"wipe";	
	impl_->transition_.Time			= boost::none;

	impl_->transition_.Dir			= boost::none;
	impl_->transition_.Param		= boost::none;
	//speed & onClick выставляются ранее
}

void pptx_slide_context::set_transitionFilter(std::wstring & type,_CP_OPT(std::wstring) & dir,_CP_OPT(std::wstring) & dop,_CP_OPT(int) & time)
{
	impl_->transition_.Type		= type;
	impl_->transition_.Time		= time; // не путать длительность перехода с длительностью эффекта перехода (в oo его нет)
	impl_->transition_.Dir		= dir;
	impl_->transition_.Param	= dop;
}

void pptx_slide_context::set_transitionAction(bool val)
{
	impl_->transition_.onClick = val;
}

void pptx_slide_context::set_transitionSpeed(std::wstring val)
{
	if (val == L"medium")	impl_->transition_.Speed = L"med";
	else					impl_->transition_.Speed = val;//fast / slow
}


/////////////////////////////////////////////////////////////////////////////////////////////////
void pptx_slide_context::default_set()
{
	impl_->object_description_.bInner_ = false;
	impl_->object_description_.type_ = typeUnknown;
    impl_->object_description_.xlink_href_	= L"";
    impl_->object_description_.name_		= L"";
	impl_->object_description_.descriptor_  = L"";
	impl_->object_description_.xml_id_		= L"";
	impl_->object_description_.anchor_		= L"";

	impl_->object_description_.additional_.clear();
	
	impl_->object_description_.clipping_string_= L"";
	impl_->object_description_.svg_rect_	= boost::none;
	impl_->object_description_.connector_	= false;
	impl_->object_description_.lined_		= false;

	impl_->object_description_.hidden_ = false;

	impl_->object_description_.start_shape_id			= boost::none;
	impl_->object_description_.start_shape_glue_point	= boost::none;
	impl_->object_description_.end_shape_id				= boost::none;
	impl_->object_description_.end_shape_glue_point		= boost::none;
	impl_->object_description_.draw_type_				= boost::none;

	impl_->object_description_.hlinks_.clear();
	impl_->object_description_.action_.clear();

	impl_->object_description_.additional_.clear();

	impl_->object_description_.fill_.clear();

	impl_->use_image_replacement_ = false;
}

bool pptx_slide_context::is_slide_filepath(const std::wstring& filename)
{
	return boost::algorithm::contains(filename, L"slide") && boost::algorithm::ends_with(filename, L".xml");
}

void pptx_slide_context::set_use_image_replacement()
{
	impl_->use_image_replacement_ = true;
}

void pptx_slide_context::set_is_placeHolder(bool is_placeholder)
{
	impl_->object_description_.additional_.push_back(odf_reader::_property(L"IsPlaceholder", is_placeholder));
}

void pptx_slide_context::set_placeHolder_type(std::wstring typeHolder)
{
	if (typeHolder == L"ftr")	impl_->footer		= true;
	if (typeHolder == L"hdr")	impl_->header		= true;
	if (typeHolder == L"dt")	impl_->date_time	= true;
	if (typeHolder == L"sldNum")impl_->slideNum		= true;

	impl_->object_description_.additional_.push_back(odf_reader::_property(L"PlaceHolderType", typeHolder));
}
void pptx_slide_context::set_placeHolder_idx(int idx)
{
	impl_->object_description_.additional_.push_back(odf_reader::_property(L"PlaceHolderIdx", idx));
}

void pptx_slide_context::set_rect(double width_pt, double height_pt, double x_pt, double y_pt)
{
	impl_->object_description_.svg_rect_ = _rect(width_pt, height_pt, x_pt, y_pt);
}

void pptx_slide_context::set_rotate(double angle, bool translate)
{
	set_property(odf_reader::_property(L"svg:rotate", angle));
	
	if (impl_->object_description_.svg_rect_ && translate)
	{
		_rect &r = impl_->object_description_.svg_rect_.get();
		double new_x = (r.cx / 2 * cos(-angle) - r.cy / 2 * sin(-angle) ) - r.cx / 2;
		double new_y = (r.cx / 2 * sin(-angle) + r.cy / 2 * cos(-angle) ) - r.cy / 2;
		
		r.x += new_x;
		r.y	+= new_y;
	}
}
void pptx_slide_context::set_translate(double x_pt, double y_pt)
{
	if (impl_->object_description_.svg_rect_)
	{
		_rect & r = impl_->object_description_.svg_rect_.get();
		
		r.x += x_pt;
		r.y += y_pt;
	}
}

void pptx_slide_context::set_scale(double cx_pt, double cy_pt)
{
	if (impl_->object_description_.svg_rect_)
	{
		_rect & r = impl_->object_description_.svg_rect_.get();
		
		r.x *= cx_pt;
		r.y *= cy_pt;
	}
}

void pptx_slide_context::set_anchor(std::wstring anchor, double x_pt, double y_pt)
{
	impl_->object_description_.anchor_		= anchor;
	impl_->object_description_.anchor_x_	= x_pt;
    impl_->object_description_.anchor_y_	= y_pt;
}

void pptx_slide_context::set_property(odf_reader::_property p)
{
	impl_->object_description_.additional_.push_back(p);
}

std::vector<odf_reader::_property> & pptx_slide_context::get_properties()
{
	return impl_->object_description_.additional_;
}

void pptx_slide_context::set_clipping(const std::wstring & str)
{
	impl_->object_description_.clipping_string_= str;
}
void pptx_slide_context::set_fill(_oox_fill & fill)
{
	impl_->object_description_.fill_= fill;
}
void pptx_slide_context::set_is_line_shape(bool val)
{
	impl_->object_description_.lined_ = val;
}
void pptx_slide_context::set_is_connector_shape(bool val)
{
	impl_->object_description_.connector_ = val;
}

void pptx_slide_context::set_connector_start_shape(const std::wstring& startShape)
{
	impl_->object_description_.start_shape_id = startShape;
}

void pptx_slide_context::set_connector_end_shape(const std::wstring& endShape)
{
	impl_->object_description_.end_shape_id = endShape;
}

void pptx_slide_context::set_connector_start_glue_point(int gluePoint)
{
	impl_->object_description_.start_shape_glue_point = gluePoint;
}

void pptx_slide_context::set_connector_end_glue_point(int gluePoint)
{
	impl_->object_description_.end_shape_glue_point = gluePoint;
}

void pptx_slide_context::set_connector_draw_type(const std::wstring& drawType)
{
	impl_->object_description_.draw_type_ = drawType;
}

void pptx_slide_context::processing_notes(bool processing_notes)
{
	impl_->processing_notes = processing_notes;
}

bool pptx_slide_context::processing_notes()
{
	return impl_->processing_notes;
}

std::wstring pptx_slide_context::add_hyperlink(std::wstring const & href)
{
	++hlinks_size_;
	std::wstring hId = L"hId" + std::to_wstring(hlinks_size_);
	
	std::wstring href_correct = XmlUtils::EncodeXmlString(href);
	XmlUtils::replace_all( href_correct, L" .", L".");//1 (130).odt

	_hlink_desc desc = {hId, href_correct};
	impl_->object_description_.hlinks_.push_back(desc);

	return hId;
}

void pptx_slide_context::add_background(_oox_fill & fill)
{
	if (fill.bitmap)
	{
		bool isMediaInternal = false;
		std::wstring ref;
		
		fill.bitmap->rId = get_mediaitems()->add_or_find(fill.bitmap->xlink_href_, typeImage, isMediaInternal, ref, oox::document_place);
		add_rels(isMediaInternal, fill.bitmap->rId, ref, typeImage);
		
		std::wstring fileName = impl_->odfPacket_ + FILE_SEPARATOR_STR + fill.bitmap->xlink_href_;
		_image_file_::GetResolution(fileName.c_str(), fill.bitmap->width, fill.bitmap->height, get_mediaitems()->applicationFonts());

		if (fill.bitmap->width && fill.bitmap->height)
		{
			//fill.bitmap->bCrop = odf_reader::parse_clipping(clipping_string_, *fill.bitmap->width, *fill.bitmap->height, fill.bitmap->cropRect);

			if (fill.bitmap->sx_pt && fill.bitmap->sy_pt)
			{
				fill.bitmap->sx = (*fill.bitmap->sx_pt * 100. / *fill.bitmap->width) * 4 / 3;
				fill.bitmap->sy = (*fill.bitmap->sy_pt * 100. / *fill.bitmap->height) * 4 / 3;
			}
		}
	}
	impl_->background_fill_ = fill;
}

void pptx_slide_context::set_name(std::wstring name)
{
	boost::replace_all(name, L"&", L"&amp;");

	impl_->object_description_.name_ = name;
}

void pptx_slide_context::set_id(std::wstring const& id)
{
	impl_->object_description_.xml_id_ = id;
}
void pptx_slide_context::set_hidden(bool val)
{
	impl_->object_description_.hidden_ = val;
}

void pptx_slide_context::start_shape(int type)
{
	if (impl_->object_description_.bInner_) return;

	impl_->object_description_.type_		= typeShape;
	impl_->object_description_.shape_type_	= type; //2,3... 
}

void pptx_slide_context::start_action(std::wstring action)
{
	impl_->object_description_.action_.enabled	= true;

	if (action == L"sound")
	{
		//impl_->object_description_.action_.action = L"ppaction://noaction";
		impl_->object_description_.action_.typeRels = typeAudio;
		impl_->object_description_.action_.highlightClick = true;
	}
	else if (action == L"next-page")
	{
		impl_->object_description_.action_.action = L"ppaction://hlinkshowjump?jump=nextslide";
		impl_->object_description_.action_.highlightClick = true;
	}
	else if (action == L"previous-page")
	{
		impl_->object_description_.action_.action = L"ppaction://hlinkshowjump?jump=previousslide";
		impl_->object_description_.action_.highlightClick = true;
	}
	else if (action == L"first-page")
	{
		impl_->object_description_.action_.action = L"ppaction://hlinkshowjump?jump=firstslide";
		impl_->object_description_.action_.highlightClick = true;
	}
	else if (action == L"last-page")
	{
		impl_->object_description_.action_.action = L"ppaction://hlinkshowjump?jump=lastslide";
		impl_->object_description_.action_.highlightClick = true;
	}
	//ppaction://hlinkshowjump?jump=endshow
	else if (action == L"execute")
	{
		impl_->object_description_.action_.action	= L"ppaction://program";
		impl_->object_description_.action_.typeRels = typeHyperlink;
		impl_->object_description_.action_.highlightClick = true;
	}
	else if (action == L"show")
	{
		impl_->object_description_.action_.action = L"ppaction://hlinksldjump";
		impl_->object_description_.action_.typeRels = typeSlide;
		impl_->object_description_.action_.highlightClick = true;
	}
}
void pptx_slide_context::set_link(std::wstring link, _rels_type typeRels)
{
	++hlinks_size_;
	
	impl_->object_description_.action_.highlightClick = true;

	if (typeRels == typeAudio)
	{
		bool isMediaInternal = true;
		
		impl_->object_description_.action_.hSoundId = get_mediaitems()->add_or_find(link, typeAudio, isMediaInternal, impl_->object_description_.action_.hSoundRef, oox::document_place);		
		impl_->add_additional_rels(isMediaInternal, impl_->object_description_.action_.hSoundId, impl_->object_description_.action_.hSoundRef, typeAudio);
	}
	else
	{
		impl_->object_description_.action_.typeRels	= typeRels;
		
		std::wstring hId = L"hId" + std::to_wstring(hlinks_size_);
		link = XmlUtils::EncodeXmlString(link);
		
		if (typeRels == typeHyperlink)
			XmlUtils::replace_all( link, L" .", L".");		//1 (130).odt

		impl_->object_description_.action_.hId	= hId;
		impl_->object_description_.action_.hRef	= link;

		if (!is_slide_filepath(link))
			impl_->object_description_.action_.action = L"";
	}
}
void pptx_slide_context::end_action()
{
}

void pptx_slide_context::start_table()
{
	impl_->object_description_.type_ = typeTable;
}

void pptx_slide_context::set_text_box()
{
	impl_->object_description_.type_		= typeShape;
	impl_->object_description_.shape_type_	= 2; //2,3... 
}
void pptx_slide_context::set_ms_object(const std::wstring & path, const std::wstring & progId)
{
	impl_->object_description_.type_		= typeMsObject;
	impl_->object_description_.xlink_href_	= path; 
	impl_->object_description_.descriptor_	= progId;
}
void pptx_slide_context::set_ole_object(const std::wstring & path, const std::wstring & progId)
{
	impl_->object_description_.type_		= typeOleObject;
	impl_->object_description_.xlink_href_	= path; 
	impl_->object_description_.descriptor_	= progId;
}
void pptx_slide_context::set_media(const std::wstring & path)
{
	if (path.empty()) return;

	impl_->object_description_.type_		= typeMedia;
	impl_->object_description_.xlink_href_	= path; 

	impl_->object_description_.action_.enabled	= true;
	impl_->object_description_.action_.action	= L"ppaction://media";
}

void pptx_slide_context::set_media_param(std::wstring name, std::wstring value)
{
}

void pptx_slide_context::set_image(const std::wstring & path)
{
	size_t pos_replaicement = path.find(L"ObjectReplacements"); 
	if (pos_replaicement != std::wstring::npos)
	{
		if (path.length() - (pos_replaicement + 18) < 2)
			return; //object without image
	}

	bool bSvg = path.rfind(L".svg") == path.length() - 4;
	bool bPdf = path.rfind(L".pdf") == path.length() - 4;

	if (bSvg)
	{
		impl_->object_description_.xlink_href_ = path; // приоритетная картинка
	}
	else if (bPdf)
	{
		if (impl_->object_description_.type_ == typeImage)
		{
			impl_->object_description_.fill_.type = 2;
			impl_->object_description_.fill_.bitmap = oox::oox_bitmap_fill::create();
			impl_->object_description_.fill_.bitmap->xlink_href_ = impl_->object_description_.xlink_href_;
			impl_->object_description_.fill_.bitmap->bStretch = true;

		}
		impl_->object_description_.type_ = typePDF;
		impl_->object_description_.descriptor_ = L"Acrobat.Document.DC";
		impl_->object_description_.xlink_href_ = path; 
		impl_->use_image_replacement_ = true;
	}

	if (impl_->object_description_.type_ == typeUnknown)
	{
		impl_->object_description_.type_		= typeImage;	
		impl_->object_description_.xlink_href_	= path; 
	}
	else if (impl_->use_image_replacement_)
	{
		impl_->object_description_.fill_.type = 2;
		impl_->object_description_.fill_.bitmap = oox::oox_bitmap_fill::create();
		impl_->object_description_.fill_.bitmap->xlink_href_ = path;
		impl_->object_description_.fill_.bitmap->bStretch = true;
	}
}

bool pptx_slide_context::start_frame()
{
	if (impl_->object_description_.type_ != typeUnknown)
		impl_->object_description_.bInner_ = true;

	return impl_->object_description_.bInner_;
}

void pptx_slide_context::set_chart(const std::wstring & path)
{
	impl_->object_description_.type_		= typeChart;
	impl_->object_description_.xlink_href_	= path; 
}

void pptx_slide_context::end_frame()
{
	if (impl_->object_description_.bInner_)
		impl_->object_description_.bInner_ = false;
	else
	{
		impl_->objects_.push_back(impl_->object_description_);
		default_set();
	}
}


void pptx_slide_context::end_shape()
{
	impl_->objects_.push_back(impl_->object_description_);
	default_set();
}

void pptx_slide_context::end_table()
{
}

bool pptx_slide_context::empty() const
{
    return impl_->empty();
}
bool pptx_slide_context::isDefault()
{
	return impl_->object_description_.type_ == typeUnknown;
}

void pptx_slide_context::process_drawings()
{
	return impl_->process_drawings();
}

void pptx_slide_context::Impl::process_image(drawing_object_description& obj, _pptx_drawing & drawing)
{
	int pos_replaicement = 0, pos_preview = 0;
	
	drawing.fill.bitmap		= oox_bitmap_fill::create();
	drawing.fill.type		= 2;
	
	_CP_OPT(std::wstring) sTextContent;
	_CP_OPT(double) dLuminance, dContrast;
	
	GetProperty(obj.additional_, L"text-content", sTextContent);
	GetProperty(obj.additional_, L"color-mode", drawing.fill.bitmap->color_mode);
	GetProperty(obj.additional_, L"luminance", drawing.fill.bitmap->luminance);
	GetProperty(obj.additional_, L"contrast", drawing.fill.bitmap->contrast);

	if (sTextContent)//в ms office на картинке нельзя сделать надпись - меняем тип на рект с заливкой картинкой
	{
		drawing.type		= typeShape;
		drawing.sub_type	= 2;//rect
	}

	std::wstring fileName = odfPacket_ + FILE_SEPARATOR_STR + obj.xlink_href_;			
	process_crop(obj, drawing, fileName);
	
///////////////////////////////////////////////////////////////////////////////////////////////////			
	std::wstring ref;/// это ссылка на выходной внешний объект
	bool isMediaInternal = false;
	
	drawing.fill.bitmap->rId		= get_mediaitems()->add_or_find(obj.xlink_href_, typeImage, isMediaInternal, ref, oox::document_place);
	drawing.fill.bitmap->isInternal	= isMediaInternal;

	if (drawing.type == typeShape)
	{
		add_additional_rels(isMediaInternal, drawing.fill.bitmap->rId, ref, typeImage);//собственно это не объект, а доп рел и ref объекта
	
		isMediaInternal	= true;
		std::wstring rId = get_mediaitems()->add_or_find(L"", typeShape, isMediaInternal, ref, oox::document_place);
		
		add_drawing(drawing, isMediaInternal, rId, ref, typeShape);//объект

	}
	else if (!drawing.fill.bitmap->rId.empty())
	{
		add_drawing(drawing, isMediaInternal, drawing.fill.bitmap->rId, ref, drawing.type);//объект
	}
}
void pptx_slide_context::Impl::process_chart(drawing_object_description & obj, _pptx_drawing & drawing)
{
    std::wstring ref;
    bool isMediaInternal = true;
   
	drawing.objectId = get_mediaitems()->add_or_find(obj.xlink_href_, obj.type_, isMediaInternal, ref, oox::document_place);  
	
	add_drawing(drawing, isMediaInternal, drawing.objectId, ref, drawing.type);
}

void pptx_slide_context::Impl::process_table(drawing_object_description & obj, _pptx_drawing & drawing)
{
    std::wstring ref;
    bool isMediaInternal = true;
	std::wstring rId = get_mediaitems()->add_or_find(L"", obj.type_, isMediaInternal, ref, oox::document_place);        
   
	add_drawing(drawing, isMediaInternal, rId, ref, drawing.type);

}

void pptx_slide_context::Impl::process_shape(drawing_object_description & obj, _pptx_drawing & drawing)
{
	int PlaceHolderIndex = 1;

	std::wstring ref;
    bool isMediaInternal = true;
	
	if (drawing.fill.bitmap)
	{
		drawing.fill.bitmap->rId = get_mediaitems()->add_or_find(drawing.fill.bitmap->xlink_href_, typeImage, isMediaInternal, ref, oox::document_place);
		
		add_additional_rels(isMediaInternal, drawing.fill.bitmap->rId, ref, typeImage);

		std::wstring fileName = odfPacket_ + FILE_SEPARATOR_STR + drawing.fill.bitmap->xlink_href_;
		process_crop(obj, drawing, fileName);
	}
		
	std::wstring rId = get_mediaitems()->add_or_find(L"", typeShape, isMediaInternal, ref, oox::document_place);
   
////////////////////////////////////////////////////////////////
	_CP_OPT(std::wstring) sPlaceHolderType;
	GetProperty(obj.additional_, L"PlaceHolderType", sPlaceHolderType);
	if (sPlaceHolderType)
	{
		drawing.place_holder_type_ = *sPlaceHolderType;

		_CP_OPT(int) iPlaceHolderIdx;
		GetProperty(obj.additional_, L"PlaceHolderIdx", iPlaceHolderIdx);
		if (iPlaceHolderIdx) drawing.place_holder_idx_ = *iPlaceHolderIdx;
	}

	_CP_OPT(bool) is_placeholder;
	GetProperty(obj.additional_, L"IsPlaceholder", is_placeholder);
	drawing.place_holder_ = is_placeholder.get_value_or(false);

	drawing.sub_type = obj.shape_type_;

	// NOTE: Все идентификаторы объектов могут быть неивестны на момент обрабоки коннектора.
	// Идентификаторы начального и конечного объекта коннектора будут обновлены после обработки всех объектов на слайде.
	drawing.start_connection_shape_id = obj.start_shape_id.get_value_or(L"");
	drawing.end_connection_shape_id = obj.end_shape_id.get_value_or(L"");
	
	drawing.start_connection_index = obj.start_shape_glue_point.get_value_or(0);
	drawing.end_connection_index = obj.end_shape_glue_point.get_value_or(0);

	drawing.connector_prst = obj.draw_type_.get_value_or(L"line");
	
	add_drawing(drawing, isMediaInternal, rId, ref, typeShape);
}
void pptx_slide_context::Impl::process_object(drawing_object_description& obj, _pptx_drawing & drawing)
{
    std::wstring ref, ref_image;
    bool isMediaInternal = true, isMediaInternal_image = true;
   
	drawing.objectId		= get_mediaitems()->add_or_find(obj.xlink_href_, obj.type_, isMediaInternal, ref, oox::document_place);      
	drawing.objectProgId	= obj.descriptor_;

	if (!drawing.fill.bitmap)
	{
		drawing.fill.bitmap = oox::oox_bitmap_fill::create();
		drawing.fill.bitmap->xlink_href_ = L"zero.png";
		
		_image_file_::GenerateZeroImage(odfPacket_ + FILE_SEPARATOR_STR + L"zero.png");
	}
	
	drawing.fill.bitmap->rId = get_mediaitems()->add_or_find(drawing.fill.bitmap->xlink_href_, typeImage, isMediaInternal_image, ref_image, oox::document_place);
	add_additional_rels(isMediaInternal_image, drawing.fill.bitmap->rId, ref_image, typeImage);

	add_drawing(drawing, isMediaInternal, drawing.objectId, ref, drawing.type);	
}
void pptx_slide_context::Impl::process_media(drawing_object_description& obj, _pptx_drawing & drawing)
{
    std::wstring ref, ref_image;
    bool isMediaInternal = true, isMediaInternal_image = true;

	drawing.type = mediaitems::detectMediaType(obj.xlink_href_); //reset from Media to Audio, Video, ... QuickTime? AudioCD? ...   
	
	drawing.objectId	= get_mediaitems()->add_or_find(obj.xlink_href_, drawing.type, isMediaInternal, ref, oox::document_place);    
	drawing.extId		= L"ext" + drawing.objectId;
	drawing.extExternal	= !isMediaInternal;
	
	if (!drawing.fill.bitmap)
	{
		drawing.fill.bitmap = oox::oox_bitmap_fill::create();
		drawing.fill.bitmap->xlink_href_ = L"zero.png";
		
		_image_file_::GenerateZeroImage(odfPacket_ + FILE_SEPARATOR_STR + L"zero.png");
	}
	drawing.fill.bitmap->rId = get_mediaitems()->add_or_find(drawing.fill.bitmap->xlink_href_, typeImage, isMediaInternal_image, ref_image, oox::document_place);		
	add_additional_rels(isMediaInternal_image, drawing.fill.bitmap->rId, ref_image, typeImage);
	
	add_drawing(drawing, false, drawing.objectId, L"NULL", drawing.type);
	add_additional_rels( isMediaInternal, drawing.extId, ref, typeMedia);
}
void pptx_slide_context::Impl::process_common_properties(drawing_object_description & pic, _pptx_drawing & drawing)
{
	if (pic.svg_rect_)
	{
		int val;
		//todooo непонятки с отрицательными значениями
		drawing.x = (int)(0.5 + odf_types::length(pic.svg_rect_->x, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		drawing.y = (int)(0.5 + odf_types::length(pic.svg_rect_->y, odf_types::length::pt).get_value_unit(odf_types::length::emu));

		val = (int)(0.5 + odf_types::length(pic.svg_rect_->cx, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		if ( val >=0 ) drawing.cx = val;
		
		val = (int)(0.5 + odf_types::length(pic.svg_rect_->cy, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		if ( val >=0 ) drawing.cy = val;
	}

	drawing.hidden		= pic.hidden_;
	drawing.additional	= pic.additional_;
	drawing.hlinks		= pic.hlinks_;
	drawing.action		= pic.action_;
	drawing.fill		= pic.fill_;
}

void pptx_slide_context::Impl::process_crop(const drawing_object_description& obj, _pptx_drawing& drawing, const std::wstring& filename)
{	
	if (!drawing.fill.bitmap) return;

	_image_file_::GetResolution(filename.c_str(), drawing.fill.bitmap->width, drawing.fill.bitmap->height, get_mediaitems()->applicationFonts());	

	if (drawing.fill.bitmap->width && drawing.fill.bitmap->height)
	{
		drawing.fill.bitmap->bCrop = odf_reader::parse_clipping(obj.clipping_string_, *drawing.fill.bitmap->width, *drawing.fill.bitmap->height, drawing.fill.bitmap->cropRect);
		
		if (drawing.fill.bitmap->sx_pt && drawing.fill.bitmap->sy_pt)
		{
			drawing.fill.bitmap->sx = (*drawing.fill.bitmap->sx_pt * 100. / *drawing.fill.bitmap->width) * 4 / 3;
			drawing.fill.bitmap->sy = (*drawing.fill.bitmap->sy_pt * 100. / *drawing.fill.bitmap->height) * 4 / 3;
		}
	}

	if (!drawing.fill.bitmap->bTile)
		drawing.fill.bitmap->bStretch = true;
}

void pptx_slide_context::dump_rels(rels & Rels)
{
	impl_->get_drawings()->dump_rels(Rels);
}

mediaitems_ptr & pptx_slide_context::get_mediaitems()
{
	return impl_->get_mediaitems(); 
}

void pptx_slide_context::add_rels( bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type)
{
	impl_->add_additional_rels(isInternal, rid, ref, type);
}

void pptx_slide_context::set_footer()
{
	impl_->footer = true;
}
void pptx_slide_context::set_header()
{
	impl_->header = true;
}
void pptx_slide_context::set_page_number()
{
	impl_->date_time = true;
}
void pptx_slide_context::set_date_time()
{
	impl_->slideNum = true;
}

void pptx_slide_context::generate_id(const std::wstring& id)
{
	impl_->generate_id(id);
}

size_t pptx_slide_context::get_id(const std::wstring& id)
{
	Impl::id_map::iterator it = impl_->id_map_.find(id);
	if (it == impl_->id_map_.end())
		return 0;

	return it->second;
}

void pptx_slide_context::serialize_background(std::wostream & strm, bool always)
{
	if (!always && ( (!impl_->background_fill_) || (impl_->background_fill_->type == 0))) return;
  
	std::wstring background_fill;
	if (impl_->background_fill_)
	{
		std::wstringstream tmp_strm;
		oox_serialize_fill(tmp_strm, impl_->background_fill_.get());
		background_fill = tmp_strm.str();
	}
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:bg")
		{
			CP_XML_NODE(L"p:bgPr")
			{
				if (false == background_fill.empty())
				{
					CP_XML_STREAM() << background_fill;
				}
				else
				{
					CP_XML_NODE(L"a:noFill");
				}
				CP_XML_NODE(L"a:effectLst");
			}
		}
	}
}

void pptx_slide_context::serialize_animations(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		if (impl_->transition_.Enabled)
		{
			CP_XML_NODE(L"p:transition")
			{
				if (impl_->transition_.Speed)
				{
					CP_XML_ATTR(L"spd",impl_->transition_.Speed.get());
				}
/*				if (impl_->transition_.Time)
				{
					CP_XML_ATTR(L"p14:dur", impl_->transition_.Time.get());
				}*/	
				if (impl_->transition_.PageTime)
				{
					CP_XML_ATTR(L"advTm", impl_->transition_.PageTime.get());
				}
				CP_XML_ATTR(L"advClick", impl_->transition_.onClick);
				
				CP_XML_NODE(std::wstring(L"p:" + impl_->transition_.Type))
				{
					if (impl_->transition_.Dir)
					{
						CP_XML_ATTR(L"dir",impl_->transition_.Dir.get());
					}

					if (impl_->transition_.Param)
					{
						if (impl_->transition_.Type == L"wheel")
							CP_XML_ATTR(L"spokes",impl_->transition_.Param.get());
						if (impl_->transition_.Type == L"fade")
							CP_XML_ATTR(L"thruBlk",impl_->transition_.Param.get());
						if (impl_->transition_.Type == L"split")
							CP_XML_ATTR(L"orient",impl_->transition_.Param.get());
					}
				}				
				//p:sndAc
			}
		}

		pptx_animation_context_.serialize(strm);
	}
}

void pptx_slide_context::serialize_HeaderFooter(std::wostream & strm)
{
	if (!impl_->header && !impl_->footer && !impl_->date_time && !impl_->slideNum)return;

    CP_XML_WRITER(strm)
    {
		CP_XML_NODE(L"p:hf")
		{
			if(impl_->footer)		CP_XML_ATTR(L"ftr",1);
			if(impl_->header)		CP_XML_ATTR(L"hdr",1);
			if(impl_->date_time)	CP_XML_ATTR(L"dt",1);
			if(impl_->slideNum)		CP_XML_ATTR(L"sldNum",1);
		}
	}
}
void pptx_slide_context::serialize_objects(std::wostream & strm)
{
	const int nextnvGrpSpPrId = 1;

    CP_XML_WRITER(strm)
    {
// дефолтную обязательную noGroup пока воткнем сюда
		CP_XML_NODE(L"p:nvGrpSpPr")
		{
			CP_XML_NODE(L"p:cNvPr")
			{
				CP_XML_ATTR(L"name", L"noGroup");
				CP_XML_ATTR(L"id", nextnvGrpSpPrId);
			}
			CP_XML_NODE(L"p:cNvGrpSpPr");
			CP_XML_NODE(L"p:nvPr");	
		}
		CP_XML_NODE(L"p:grpSpPr")
		{
			CP_XML_NODE(L"a:xfrm")
			{
				CP_XML_NODE(L"a:off")	{ CP_XML_ATTR(L"x", 0);	CP_XML_ATTR(L"y", 0);	}
				CP_XML_NODE(L"a:ext")	{ CP_XML_ATTR(L"cx",0);	CP_XML_ATTR(L"cy",0);	}
				CP_XML_NODE(L"a:chOff")	{ CP_XML_ATTR(L"x", 0);	CP_XML_ATTR(L"y", 0);	}
				CP_XML_NODE(L"a:chExt")	{ CP_XML_ATTR(L"cx",0);	CP_XML_ATTR(L"cy",0);	}
			}
		}
	}
	impl_->get_drawings()->serialize(strm);   
}

}
}
