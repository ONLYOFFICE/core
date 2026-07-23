/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "draw_base.h"

namespace cpdoccore { 
namespace odf_writer {

/// draw-image-attlist
class draw_image_attlist
{
public:
    _CP_OPT(std::wstring) draw_filter_name_;
	
	void serialize(CP_ATTR_NODE);   
};
 //draw-frame-attlist
class draw_frame_attlist
{
public:
    _CP_OPT(std::wstring) draw_copy_of_;
	
	void serialize(CP_ATTR_NODE);   
};
 //draw-chart-attlist
class draw_chart_attlist
{
public:

    //_CP_OPT(std::wstring) draw_filter_name_;
	void serialize(CP_ATTR_NODE);   

};
/////////////////////////////////////////////////////////////////////////////////////////
class draw_image : public office_element_impl<draw_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeDrawImage;    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	draw_image_attlist draw_image_attlist_;
    _CP_OPT(std::wstring) draw_mime_type_;
    odf_types::common_xlink_attlist common_xlink_attlist_;
    
	office_element_ptr office_binary_data_;
    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(draw_image)
//////////////////////////////////////////////////////////////////////////////////////////////////////////

 // drawing object is not needed.. only place for frame is needed - the chart itself is elsewhere
class draw_chart : public office_element_impl<draw_chart>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeDrawChart;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	draw_chart_attlist draw_chart_attlist_;
    odf_types::common_xlink_attlist common_xlink_attlist_;
   
	//office_element_ptr title_;
    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(draw_chart)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class draw_frame : public draw_base
{    // for draw-text-box, draw-image, draw-object, draw-object-ole, draw-applet, draw-floating-frame, draw-plugin
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const ElementType type = typeDrawFrame;    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	int idx_in_owner ;

    draw_frame_attlist						draw_frame_attlist_;

    office_element_ptr						office_event_listeners_;
		// cannot move to content - otherwise event will be added to the next object, not this one
    office_element_ptr						draw_glue_point_;
    office_element_ptr						draw_image_map_;
    //office_element_ptr draw_chart_map_;

    office_element_ptr draw_contour_; // draw-contour-polygon or draw-contour-path
};

CP_REGISTER_OFFICE_ELEMENT2(draw_frame)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class draw_g : public office_element_impl<draw_g>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeDrawG;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	odf_types::union_common_draw_attlists   common_draw_attlists_;  
    _CP_OPT(std::wstring)                   xml_id_;

    office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_g)
// draw-text-box-attlist
class draw_text_box_attlist
{
public:
    _CP_OPT(std::wstring) draw_chain_next_name_;
    _CP_OPT(odf_types::length) draw_corner_radius_;
    _CP_OPT(odf_types::length_or_percent) fo_min_width_;
    _CP_OPT(odf_types::length_or_percent) fo_min_height_;
    _CP_OPT(odf_types::length_or_percent) fo_max_width_;
    _CP_OPT(odf_types::length_or_percent) fo_max_height_;        

	void serialize(CP_ATTR_NODE);   
};


class draw_text_box : public office_element_impl<draw_text_box>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeDrawTextBox;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	draw_text_box_attlist attlist_;
    office_element_ptr_array content_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_text_box)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// draw:object
class draw_object : public office_element_impl<draw_object>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeDrawObject;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

    odf_types::common_xlink_attlist common_xlink_attlist_;
	//draw:notify-on-update-of-ranges
};

CP_REGISTER_OFFICE_ELEMENT2(draw_object)

// draw:object-ole
class draw_object_ole : public office_element_impl<draw_object>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeDrawObjectOle;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	odf_types::common_xlink_attlist	common_xlink_attlist_;
	_CP_OPT(std::wstring)			draw_class_id_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_object_ole)

// draw:plugin
class draw_plugin : public office_element_impl<draw_plugin>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeDrawPlugin;
    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

	odf_types::common_xlink_attlist	common_xlink_attlist_;
	_CP_OPT(std::wstring)			draw_mime_type_;

    office_element_ptr_array		content_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_plugin)

}
}
