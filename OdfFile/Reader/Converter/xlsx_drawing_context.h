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

#include <string>

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

	typedef _CP_PTR(mediaitems)		mediaitems_ptr;
	typedef _CP_PTR(xlsx_drawings)	xlsx_drawings_ptr;

class xlsx_drawing_context_handle
{
public:
    xlsx_drawing_context_handle(mediaitems_ptr & items);
    ~xlsx_drawing_context_handle();
    
    std::pair<std::wstring, std::wstring>	add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings, _rels_type const & type_ = typeDefault);
 	std::pair<std::wstring, std::wstring>	add_drawing_vml(std::wstring const & content, xlsx_drawings_ptr drawings);
   
	const std::vector<drawing_elm> & content()		const;
	const std::vector<drawing_elm> & content_vml()	const;

    friend class xlsx_drawing_context;

    size_t next_rId()
    {
        return next_rId_++;
    }
private:
	size_t	next_rId_;

	class Impl;
    _CP_PTR(Impl) impl_;
};



typedef _CP_PTR(xlsx_drawing_context_handle) xlsx_drawing_context_handle_ptr;

class xlsx_drawing_context
{
public:
    xlsx_drawing_context(xlsx_drawing_context_handle_ptr & h, bool in_text = false);
    ~xlsx_drawing_context();

	bool isDefault();

	void set_odf_packet_path(std::wstring path){odf_packet_path_ = path;}//for image analysis

	void start_drawing	(std::wstring const & name);
	void end_drawing	();
   
	void start_group(std::wstring const & name);
    void end_group	();

	void start_shape(int type);
    void end_shape();

	bool start_frame();
		void set_image		(const std::wstring & path);
		void set_chart		(const std::wstring & path);
		void set_ole_object	(const std::wstring & path, const std::wstring & progId);
		void set_ms_object	(const std::wstring & path, const std::wstring & progId);
		void set_text_box	();
	void end_frame();

	void start_control(const std::wstring & ctrlPropId, int type);
	void end_control();

	void start_comment(int base_col, int base_row);
	void end_comment();

	void set_rect(double width_pt, double height_pt, double x_pt, double y_pt);

	void set_translate	(double x_pt, double y_pt);
	void set_scale		(double cx_pt, double cy_pt);
	void set_rotate		(double angle, bool translate = false);

	void set_rel_anchor	(_INT32 owner_cx, _INT32 owner_cy);
	
	void set_anchor_start(std::wstring anchor, double x_pt, double y_pt, bool group = false);
	void set_anchor_end	(std::wstring anchor, double x_pt, double y_pt, bool group = false);
	
	void set_fill(_oox_fill & fill);

	void set_is_line_shape(bool val);
	void set_is_connector_shape(bool val);

	void set_property	(odf_reader::_property p);
	std::vector<odf_reader::_property> & get_properties();

	std::wstring add_hyperlink(std::wstring const & ref);

	void set_use_image_replacement();
	
	bool empty() const;
	bool vml_empty() const;
	
	void clear();

	void serialize(std::wostream & strm, const std::wstring& ns = L"xdr", bool local = false);
	void serialize_vml(std::wostream & strm);

	std::wstring dump_path(std::vector<svg_path::_polyline> & path, double w,double h);

    xlsx_drawings_ptr get_drawings();
    
	void process_objects(xlsx_table_metrics & table_metrics);

	void start_action(std::wstring action);
		void set_link(std::wstring link, _rels_type typeRels = typeHyperlink); 
	void end_action();

private:
    class Impl;
    _CP_PTR(Impl) impl_;

	void process_group				(drawing_object_description & obj, xlsx_table_metrics & table_metrics, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_);
	void process_group_objects		(std::vector<drawing_object_description> objects, xlsx_table_metrics & table_metrics, xlsx_drawings_ptr xlsx_drawings_);
  
	void process_image				(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_);
    void process_chart				(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_);
    void process_shape				(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_);
    void process_object				(drawing_object_description & obj, xlsx_table_metrics & table_metrics, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_);

	void process_common_properties	(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_table_metrics & table_metrics);

	void process_position_properties(drawing_object_description & obj, xlsx_table_metrics & table_metrics, xlsx_table_position & from,xlsx_table_position & to, bool byEnd = true);

	int				hlinks_size_;
	std::wstring	odf_packet_path_ ;
	float			dpi_;

	bool			in_text_ = false;
    
};
typedef _CP_PTR(xlsx_drawing_context) xlsx_drawing_context_ptr;

}
}
