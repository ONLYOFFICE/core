/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "pptx_drawings.h"
#include <string>

namespace cpdoccore {
namespace oox {

class pptx_conversion_context;
struct drawing_object_description;
class mediaitems;
class pptx_drawings;
struct _oox_fill;


class pptx_slide_context
{
public:
    pptx_slide_context(pptx_conversion_context & Context);
	
    void start_slide();
	void end_slide(){}
////////////////////////////////////////////////////////////////////////////////////////
	void start_slide_animation();
	void end_slide_animation(){}
	
	void set_transitionFilter(std::wstring & type,_CP_OPT(std::wstring) & dir, _CP_OPT(std::wstring) & dop, _CP_OPT(int) & time);
	void set_transitionAction(bool val);
	void set_transitionSpeed(std::wstring val);
///////////////////////////////////////////////////////////////////////////////////////////
	void set_rect(double width_pt, double height_pt, double x_pt, double y_pt);

	void set_translate(double x_pt, double y_pt);
	void set_scale(double cx_pt, double cy_pt);
	void set_rotate(double angle);

	void set_name(std::wstring const & name);
	void set_anchor(std::wstring anchor, double x_pt, double y_pt);
	void set_property(odf_reader::_property p);
	std::vector<odf_reader::_property> & get_properties();
    void set_clipping(const std::wstring & str );
	void set_fill(_oox_fill & fill);
	
	void set_placeHolder_type(std::wstring typeHolder);
	void set_placeHolder_idx(int idx);

	std::wstring add_hyperlink(std::wstring const & ref, bool object);

    void start_image(std::wstring const & path);
    void end_image();
   
	void start_chart(std::wstring const & path);
    void end_chart();

 	void start_table();
    void end_table();

	void start_shape(int type);
    void end_shape();
	
 	void start_object_ole();
    void end_object_ole();

	void set_use_image_replacement();

	bool empty() const;
//////////////////////////////////////////////////////////////////////////////////////////////
	void serialize_objects(std::wostream & strm);
	void serialize_HeaderFooter(std::wostream & strm);
	void serialize_background(std::wostream & strm, bool always=false);
	void serialize_animations(std::wostream & strm);

	void dump_rels(rels & Rels);

	void process_drawings();

	mediaitems & get_mediaitems();

	void add_background(_oox_fill & fill);


	void add_rels( bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        mediaitems::Type type);

	void set_footer();
	void set_header();
	
	void set_page_number();
	void set_date_time();
private:
	void process_common_properties(drawing_object_description& pic,_pptx_drawing & drawing);
	void default_set();
    
	void process_shapes();
    void process_images();
	void process_charts();
	void process_tables();

	int hlinks_size_;
	
    class Impl;
    _CP_PTR(Impl) impl_;


//contentPart (Content Part) §19.3.1.14
//cxnSp (Connection Shape) §19.3.1.19
//extLst (Extension List with Modification Flag) §19.3.1.20
//graphicFrame (Graphic Frame) §19.3.1.2
//grpSp (Group Shape) §19.3.1.22
//grpSpPr (Group Shape Properties) §19.3.1.23
//nvGrpSpPr (Non-Visual Properties for a Group Shape) §19.3.1.31
//pic (Picture) §19.3.1.37
//sp (Shape) §19.3.1.43
};


}
}
