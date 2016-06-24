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

#include <string>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>

#include "xlsx_drawings.h"

namespace cpdoccore { 

	namespace svg_path
	{
		struct _polyline;
	};
	
namespace oox {

	struct drawing_object_description;
	struct xlsx_table_position;
	class xlsx_table_metrics;
	class mediaitems;
	class xlsx_drawings;
	struct _oox_fill;

	typedef _CP_PTR(xlsx_drawings) xlsx_drawings_ptr;

class xlsx_drawing_context_handle
{
public:
    xlsx_drawing_context_handle(mediaitems & items);
    ~xlsx_drawing_context_handle();
    
    std::pair<std::wstring, std::wstring>	add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings);
    const std::vector<drawing_elm>			& content() const;

    friend class xlsx_drawing_context;
private:
    class Impl;
    _CP_PTR(Impl) impl_;
};



class xlsx_drawing_context
{
public:
    xlsx_drawing_context(xlsx_drawing_context_handle & h);
    ~xlsx_drawing_context();

	void set_odf_packet_path(std::wstring path){odf_packet_path_ = path;}//для анализа картинок

	void start_drawing(std::wstring const & name);
	void end_drawing();
   
	void start_group(std::wstring const & name);
    void end_group();

    void start_image(std::wstring const & path);
    void end_image();
   
	void start_chart(std::wstring const & path);
    void end_chart();

	void start_shape(int type);
	//...пока тока общие свойства ... частные для каждого объекта пооозже
    void end_shape();

	void start_object_ole();
	void end_object_ole();

	void set_rect(double width_pt, double height_pt, double x_pt, double y_pt);

	void set_translate(double x_pt, double y_pt);
	void set_scale(double cx_pt, double cy_pt);
	void set_rotate(double angle);

	void set_anchor(std::wstring anchor, double x_pt, double y_pt, bool group = false);
	void set_property(odf_reader::_property p);
    void set_clipping(const std::wstring & str );
	void set_fill(_oox_fill & fill);

	std::vector<odf_reader::_property> & get_properties();

	std::wstring add_hyperlink(std::wstring const & ref, bool object);

	void set_use_image_replacement();
	
	bool empty() const;
	void clear();

    void serialize(std::wostream & strm);
	std::wstring dump_path(std::vector<svg_path::_polyline> & path, double w,double h);

    xlsx_drawings_ptr get_drawings();
    
	void process_objects(xlsx_table_metrics & table_metrics);

private:
    class Impl;
    _CP_PTR(Impl) impl_;

	void process_objects			(std::vector<drawing_object_description> objects, xlsx_table_metrics & table_metrics, xlsx_drawings_ptr xlsx_drawings_);
	void process_group				(drawing_object_description & obj, xlsx_table_metrics & table_metrics, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_);
  
	void process_image				(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_);
    void process_chart				(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_);
    void process_shape				(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_);

	void process_common_properties	(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_table_metrics & table_metrics);

	void process_position_properties(drawing_object_description & obj, xlsx_table_metrics & table_metrics, xlsx_table_position & from,xlsx_table_position & to);

	int				hlinks_size_;
	std::wstring	odf_packet_path_ ;
	float			dpi_;
    
};

}
}
