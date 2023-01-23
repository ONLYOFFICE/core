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
    odf_types::common_xlink_attlist common_xlink_attlist_;
    
	office_element_ptr office_binary_data_;
    office_element_ptr_array content_;

};

CP_REGISTER_OFFICE_ELEMENT2(draw_image)
//////////////////////////////////////////////////////////////////////////////////////////////////////////

 //объект рисования не нужен .. нужно только место для фрэйма - сам чарт в другом месте
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
{    //для  draw-text-box, draw-image, draw-object, draw-object-ole, draw-applet, draw-floating-frame, draw-plugin
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
		// в content перенести нельзя - иначе событи будет добавляться не к этому объекту а следующему
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

	odf_types::union_common_draw_attlists  common_draw_attlists_;  

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
