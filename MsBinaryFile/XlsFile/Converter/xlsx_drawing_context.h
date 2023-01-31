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

#include <vector>
#include <iosfwd>
#include <string.h>

#include <boost/shared_array.hpp>

#include "../Format/Logic/Biff_structures/ODRAW/OfficeArtFOPTE.h"

#include "xlsx_drawings.h"

#include "ShapeType.h"	

const std::wstring standart_color[56] = {
		L"000000",L"FFFFFF",L"FF0000",L"00FF00",L"0000FF",L"FFFF00",L"FF00FF",L"00FFFF",
		L"800000",L"008000",L"000080",L"808000",L"800080",L"008080",L"C0C0C0",L"808080",
		L"9999FF",L"993366",L"FFFFCC",L"CCFFFF",L"660066",L"FF8080",L"0066CC",L"CCCCFF",
		L"000080",L"FF00FF",L"FFFF00",L"00FFFF",L"800080",L"800000",L"008080",L"0000FF",
		L"00CCFF",L"CCFFFF",L"CCFFCC",L"FFFF99",L"99CCFF",L"FF99CC",L"CC99FF",L"FFCC99",
		L"3366FF",L"33CCCC",L"99CC00",L"FFCC00",L"FF9900",L"FF6600",L"666699",L"969696",
		L"003366",L"339966",L"003300",L"333300",L"993300",L"993366",L"333399",L"333333"	};

namespace PPTX
{
	namespace Logic
	{
		class Xfrm;
	}
}
namespace oox {

	class external_items;
	class xlsx_conversion_context;

class xlsx_drawing_context_handle
{
public:
    xlsx_drawing_context_handle	(size_t & next_vml_file_id, external_items & items);
    ~xlsx_drawing_context_handle();
    
    std::pair<std::wstring, std::wstring> add_drawing_xml	(std::wstring const & content, xlsx_drawings_rels_ptr rels);
	std::pair<std::wstring, std::wstring> add_drawing_vml	(std::wstring const & content, xlsx_drawings_rels_ptr rels);
    
	const std::vector<drawing_elm> & content()		const;
	const std::vector<drawing_elm> & content_vml()	const;

    friend class xlsx_drawing_context;
private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

struct _color
{
	_color(unsigned char nR, unsigned char  nG, unsigned char  nB) 
	{
		SetRGB(nR, nG, nB);
	}
	_color(){}
	int				nRGB = 0;
	std::wstring	sRGB;
	int				index = -1;
	bool			bScheme = false;

	void SetRGB(unsigned char nR, unsigned char  nG, unsigned char  nB);

	unsigned char  GetB() { return (unsigned char )(nRGB);}
	unsigned char  GetG() { return (unsigned char )(nRGB>>8);}
	unsigned char  GetR() { return (unsigned char )(nRGB>>16);}

};

struct _rect
{
	_rect() : x(0), y(0), cx(0), cy(0){}

    int   x;
    int   y;
    int   cx;
    int   cy;

};
enum _fill_type
{
	fillNone,
	fillSolid,
	fillPattern,
	fillTexture,
	fillGradient,
	fillGradientOne,
	fillUndefined
};
enum _texture_mode
{
	textureTill,
	textureStretch
};
enum _line_typeDash
{
	lineSolid,
	lineDash,
	lineDot,
	lineDashDot,
	lineDashDotDot
};
	
class _drawing_state;
typedef _CP_PTR(_drawing_state) _drawing_state_ptr;

class _drawing_state
{
public:
	_drawing_state() : parent_drawing_states(NULL)
	{
	}

	external_items::Type	type;
	bool					hidden = false;
	std::wstring			name;
	std::wstring			description;

	std::wstring			xmlAlternative;
	std::wstring			xmlGeomAlternative;
	std::wstring			xmlTxBodyAlternative;
	std::wstring			xmlFillAlternative;
	std::wstring			xmlEffectAlternative;
	std::wstring			xmlStyleAlternative;

	std::wstring			objectId;
	std::wstring			objectProgId;

	int						type_control = -1;

	struct _anchor
	{
		int					colFrom = -1;
		int					xFrom = 0;
		int					rwFrom = -1;
		int					yFrom = 0;
		int					colTo = 0;
		int					xTo = 0;
		int					rwTo = 0;
		int					yTo = 0;

		_rect				absolute;
	}						sheet_anchor;

	_rect					child_anchor;
	_rect					group_anchor;
	_rect					absolute_anchor;

	int						type_anchor = 0;
	
	bool					vml_HF_mode_ = false;
	
	std::wstring			shape;
	std::wstring			vml_shape;

	int						id = -1;
	MSOSPT					shape_id = msosptRectangle;
//----------------------------------------------	
	bool					flipV = false;
	bool					flipH = false;
	double					rotation = 0;
//-----------------------------------------------
	std::vector<ODRAW::MSOPATHINFO>	custom_segments;
	std::vector<ODRAW::MSOSG>		custom_guides;
	std::vector<ODRAW::MSOPOINT>	custom_verticles;
	std::vector<ODRAW::ADJH>		custom_adjustHandles;
	std::vector<ODRAW::MSOPOINT>	custom_connection;
	std::vector<OSHARED::FixedPoint>custom_connectionDir;
	std::vector<ODRAW::MSORECT>		custom_inscribe;

	_rect							custom_rect;
	std::vector<_CP_OPT(int)>		custom_adjustValues;
	int								custom_path= -1;
	size_t							custom_x_limo = 0x80000000;
	size_t							custom_y_limo = 0x80000000;
//-----------------------------------------------
	std::wstring					hyperlink;
	struct _text
	{
		_text()
		{
			margins.left = margins.right = 0x00016530;
			margins.top = margins.bottom = 0x0000b298;
		}
		std::wstring	content;		//c форматированием oox
		std::wstring	vml_content;	//c форматированием vml
		int				wrap = 0; //square
		int				align = 0; //noset
		int				vert_align = 0; //noset
		int				vertical = 0; //horiz
		RECT			margins;
		bool			fit_shape = false;
	}text;
	
	struct _wordart
	{
		bool			is = false;
		std::wstring	text;	
		std::wstring	font;	
		int				size = 0;;
		bool			bold = false;
		bool			italic = false;
		bool			underline = false;
		bool			strike = false;
		bool			vertical = false;
		double			spacing = 1.;
	}wordart;
	
	bool				bTextBox = false;
	bool				bCustom = false;

	struct _shadow
	{
		_shadow () : color(0x7f, 0x7f, 0x7f), highlight(0x7f, 0x7f, 0x7f) {}
		
		bool			enabled = false;
		int				type = 0x00000000; //msoshadowOffset
		bool			is = false;
		_color			color;
		_color			highlight;
		double			opacity;
		double			offsetX = 0x00006338;
		double			offsetY = 0x00006338;
		double			scaleX2X = 1.;
		double			scaleY2Y = 1.;
		double			originX = 0;
		double			originY = 0;

	}shadow;
	
	struct _fill
	{
		_fill() 
		{
			color.SetRGB(0xff, 0xff, 0xff);
			memset(texture_crop, 0, 4 * sizeof(double));
		}
		_color			color;
		_color			color2;
		double			opacity = 0;
		double			opacity2 = 0;
		_fill_type		type = fillSolid; 

		int				focus = 0;
		double			angle = 0;
		
		std::wstring	texture_target;
		std::wstring	picture_target;
		double			texture_crop[4];
		bool			texture_crop_enabled = false;
		_texture_mode	texture_mode;

		_CP_OPT(int)	contrast;
		_CP_OPT(int)	brightness;
		_CP_OPT(bool)	grayscale;
		_CP_OPT(int)	biLevel;

		std::vector<std::pair<double, _color>> colorsPosition;
	}fill;

	void clear_fill()
	{
		fill.type = fillSolid; 
		fill.color.SetRGB(0xff, 0xff, 0xff);
		fill.angle = fill.opacity = fill.opacity2 = fill.focus = 0; 
		memset(fill.texture_crop, 0, 4 * sizeof(double));
		fill.texture_crop_enabled = false;
		fill.colorsPosition.clear();
		fill.picture_target.clear();
		fill.texture_target.clear();
		fill.contrast = fill.brightness = fill.grayscale = boost::none;
	}
	struct _arrow
	{
		_arrow() { enabled = false; start = end = 0; start_length = end_length = end_width = start_width = 2;}
		
		bool	enabled;
		int		start;
		int		end;
		int		start_width;
		int		end_width;
		int		start_length;
		int		end_length;
	};
	struct _line
	{
		_line() : join(-1), endcap(-1), miter(0), width(0) { fill.color.SetRGB(0, 0, 0);  }
		std::wstring	style;
        int             width;
		_line_typeDash	typeDash;
		_fill			fill;
		_arrow			arrow;
		int				miter;
		int				join;
		int				endcap;
	}line;	
	struct _object
	{
		int				id = 0;
		bool			bVisible = true;
		int				col = 0;
		int				row = 0;
		bool			bAutoLine = false;
		bool			bAutoPict = false;
		std::wstring	macro;
		std::wstring	link;
		_CP_OPT(int)	x_val;
		_CP_OPT(int)	x_min;
		_CP_OPT(int)	x_max;
		_CP_OPT(int)	x_inc;
		_CP_OPT(int)	x_page;
		_CP_OPT(int)	x_sel;
		_CP_OPT(int)	x_sel_type;
		_CP_OPT(int)	lct	;
		std::wstring	fmlaRange;
		_CP_OPT(int)	drop_style;
		_CP_OPT(int)	drop_lines;
	}object;
//for group
	std::vector<_drawing_state_ptr>		drawing_states;
	std::vector<_drawing_state_ptr>*	parent_drawing_states;
};
struct _hlink_desc
{
    std::wstring	sId;
    std::wstring	sLink;
	std::wstring	sDisplay;
	bool			bExternal;
};

class xlsx_drawing_context
{
public:
    xlsx_drawing_context(xlsx_conversion_context & Context);
	~xlsx_drawing_context(){}

	external_items::Type	getType();
	int						getLevel();

	xlsx_drawings_rels_ptr get_rels();
	xlsx_drawings_rels_ptr get_vml_HF_rels();
	xlsx_drawings_rels_ptr get_vml_rels();
	xlsx_drawings_rels_ptr get_sheet_rels();
	
	bool empty();	
	bool empty_vml();
	bool empty_vml_HF();

	void start_group();
	bool start_drawing	(int type);	
//--------------------------------------------------------------------------------------
		void start_image();
		void start_shape(int type);
		void start_chart();
		void start_comment();
		void start_control(int type);

		void set_alternative_drawing(const std::wstring & xml_data);
		void set_xfrm_from_anchor(PPTX::Logic::Xfrm *xfrm, _drawing_state_ptr state);
		void reset_alternative_drawing();

        void set_id			(int id);
		void set_FlipH		();
		void set_FlipV		();
		void set_shape_id	(int id);
		void set_hidden		(bool val);
//--------------------------------------------------------------------------------------
		void set_mode_HF		(bool val);
		bool get_mode_HF		();
//--------------------------------------------------------------------------------------
        void set_name				(const std::wstring & str);
        void set_description		(const std::wstring & str);
        void set_macro				(const std::wstring & str);
		void set_ole_object			(const std::wstring & id, const std::wstring & info);
		void set_control_activeX	(const std::wstring & id);
		
        void set_picture_crop_top	(double val);
        void set_picture_crop_bottom(double val);
        void set_picture_crop_left	(double val);
        void set_picture_crop_right	(double val);
		void set_picture_name		(const std::wstring & str);
		void set_picture_grayscale	(bool val);
		void set_picture_brightness	(int val);
		void set_picture_contrast	(int val);
		void set_picture_biLevel	(int val);
		void set_picture_transparent(int nColor, const std::wstring & sColor);
		void set_picture			(const std::wstring & str);

        void set_rotation			(double val);

        void set_fill_color			(int nColor, const std::wstring & sColor, bool background = false);
		void set_fill_color			(int index, int type, bool background = false);
 		void set_fill_opacity		(double val, bool background = false);       
		void set_fill_type			(_fill_type val);
		void set_fill_angle			(double val);
		void set_fill_texture_mode	(int val);
        void set_fill_texture		(const std::wstring & str);
		void add_fill_colors		(double position, const std::wstring & color);
		void add_fill_colors		(double position, int index, int type);		
		void set_fill_focus			(int val);

		int get_fill_type			();
		void clear_fill				();

		void set_line_color			(int nColor, const std::wstring & color);
		void set_line_color			(int index, int type);
        void set_line_type			(int val);
        void set_line_style			(int val);
        void set_line_width			(int val);
		void set_line_dash			(int val);
		void set_line_arrow			(bool val);
		void set_arrow_start		(int val);
		void set_arrow_end			(int val);
		void set_arrow_start_width	(int val);
		void set_arrow_end_width	(int val);
		void set_arrow_start_length	(int val);
		void set_arrow_end_length	(int val);
		void set_line_miter			(int val);
		void set_line_join			(int val);
		void set_line_endcap		(int val);

		void set_shadow_enabled		(bool val);
		void set_shadow_type		(int val);
		void set_shadow_opacity		(double val);
		void set_shadow_color		(int nColor, const std::wstring & color);
		void set_shadow_highlight	(int nColor, const std::wstring & color);
		void set_shadow_originX		(double val);
		void set_shadow_originY		(double val);
		void set_shadow_offsetX		(int val);
		void set_shadow_offsetY		(int val);
		void set_shadow_scaleX2X	(double val);
		void set_shadow_scaleY2Y	(double val);

		void set_fill_old_version	(_UINT32 val);
		void set_line_old_version	(_UINT32 val);
		void set_flag_old_version	(_UINT16 val, _UINT16 val2);

		void set_absolute_anchor	(double x, double y, double cx, double cy);
        void set_child_anchor		(int x, int y, int cx, int cy);
		void set_group_anchor		(int x, int y, int cx, int cy);
        void set_sheet_anchor		(int colFrom, int xFrom, int rwFrom, int yFrom, int colTo, int xTo, int rwTo, int yTo, 
										double x, double y, double cx, double cy);

        void set_properties			(const std::wstring & str);
        void set_hyperlink			(const std::wstring & link, const std::wstring & display, bool is_external);

		void set_text				(const std::wstring & text);
		void set_text_vml			(const std::wstring & text);
		void set_text_wrap			(int val);
		void set_text_align			(int val);
		void set_text_vert_align	(int val);
		void set_text_vertical		(int val);
		void set_text_margin		(RECT & val);
		void set_text_fit_shape		(bool val);
		
		void set_wordart_text		(const std::wstring & text);
		void set_wordart_font		(const std::wstring & text);
		void set_wordart_size		(int val);
		void set_wordart_bold		(bool val);
		void set_wordart_italic		(bool val);
		void set_wordart_underline	(bool val);
		void set_wordart_strike		(bool val);
		void set_wordart_vertical	(bool val);
		void set_wordart_spacing	(double val);
//---------------------------------------------------------------------------------------------
		void set_object_id			(int val);
		void set_object_visible		(bool val);
		void set_object_anchor		(int col, int row);
		void set_object_link		(const std::wstring & fmla);
		void set_object_x_val		(int val);
		void set_object_x_min		(int val);
		void set_object_x_max		(int val);
		void set_object_x_inc		(int val);
		void set_object_x_page		(int val);
		void set_object_x_sel		(int val);
		void set_object_x_sel_type	(int val);
		void set_object_lct			(int val);
		void set_object_fmlaRange	(const std::wstring & fmla);
		void set_object_drop_style	(int val);
		void set_object_drop_lines	(int val);
//---------------------------------------------------------------------------------------------
		void set_custom_rect		(_rect							& rect);
		void set_custom_verticles	(std::vector<ODRAW::MSOPOINT>	& points);
		void set_custom_segments	(std::vector<ODRAW::MSOPATHINFO>& segments);
		void set_custom_guides		(std::vector<ODRAW::MSOSG>		& guides);
		void set_custom_adjustHandles(std::vector<ODRAW::ADJH>		& handles);
		void set_custom_adjustValues(std::vector<_CP_OPT(int)>		& values);
		void set_custom_path		(int type_path);
		void set_custom_connection	(std::vector<ODRAW::MSOPOINT>	& points);
		void set_custom_connectionDir(std::vector<OSHARED::FixedPoint>& points);
		void set_custom_inscribe	(std::vector<ODRAW::MSORECT>	& rects);
		void set_custom_x_limo		(int val);
		void set_custom_y_limo		(int val);
//------------------------------------------------------------------------------	
		void serialize_group		();
		void serialize_shape		(_drawing_state_ptr & drawing_state);
		void serialize_chart		(_drawing_state_ptr & drawing_state);
		void serialize_pic			(_drawing_state_ptr & drawing_state);
		void serialize_control		(_drawing_state_ptr & drawing_state);
//-----------------------------------------------------------------------------------		
		void serialize_vml_shape	(_drawing_state_ptr & drawing_state);			
		void serialize_vml_pic		(_drawing_state_ptr & drawing_state);	
//-----------------------------------------------------------------------------------		
		void serialize_fill			(std::wostream & strm, _drawing_state_ptr & drawing_state);
		void serialize_fill			(std::wostream & strm);
//-----------------------------------------------------------------------------------		
		void serialize				(std::wostream & strm, _drawing_state_ptr & drawing_state);		
		void serialize_object		(std::wostream & strm, _drawing_state_ptr & drawing_state);		
		void serialize_control		(std::wostream & strm, _drawing_state_ptr & drawing_state);
		void serialize_control_props(std::wostream & strm, _drawing_state_ptr & drawing_state);
//-----------------------------------------------------------------------------------		
		void serialize_controls		(std::wostream & strm);
		void serialize_objects		(std::wostream & strm);  
		void serialize_vml_HF		(std::wostream & strm);
		void serialize_vml			(std::wostream & strm);
		void serialize				(std::wostream & strm);
//-----------------------------------------------------------------------------------
		bool is_lined_shape			(_drawing_state_ptr & drawing_state);
	void end_drawing();
	void end_group();
private:
	int							current_level;
    xlsx_conversion_context		& context_;
	
	xlsx_drawing_context_handle	& handle_;
	
	xlsx_drawings_rels_ptr		rels_;
	xlsx_drawings_rels_ptr		vml_rels_;
	xlsx_drawings_rels_ptr		vml_HF_rels_;
	xlsx_drawings_rels_ptr		sheet_rels_;

	int							count_object;
	bool						in_chart_;
	
	std::vector<_hlink_desc>	hlinks_;

	std::vector<_drawing_state_ptr>		drawing_states_vml_HF;
	std::vector<_drawing_state_ptr>		drawing_states_vml;

	std::vector<_drawing_state_ptr>		drawing_states;
	std::vector<_drawing_state_ptr>*	current_drawing_states;

	std::vector<_drawing_state_ptr>		drawing_states_objects;//копии для сериализации ole
	std::vector<_drawing_state_ptr>		drawing_states_controls;//копии для сериализации control
	
	void end_drawing			(_drawing_state_ptr & drawing_state);
	void reset_fill_pattern		(_drawing_state_ptr & drawing_state);
	
	void serialize_line			(std::wostream & stream, _drawing_state::_line & line);
	void serialize_fill			(std::wostream & stream, _drawing_state::_fill & fill);
	void serialize_gradient_fill(std::wostream & stream, _drawing_state::_fill & fill);
	void serialize_bitmap_fill	(std::wostream & stream, _drawing_state::_fill & fill, std::wstring rId, const std::wstring ns = L"a:");
	void serialize_solid_fill	(std::wostream & stream, const _color &color, double opacity = 0);
	void serialize_none_fill	(std::wostream & stream);
	
	void serialize_line			(std::wostream & stream, _drawing_state_ptr & drawing_state);
	void serialize_xfrm			(std::wostream & stream, _drawing_state_ptr & drawing_state);
	void serialize_anchor		(std::wostream & stream, _drawing_state_ptr & drawing_state, std::wstring ns = L"xdr:");
	void serialize_text			(std::wostream & stream, _drawing_state_ptr & drawing_state);
	void serialize_color		(std::wostream & stream, const _color &color, double opacity = 0);

	void serialize_arrow		(std::wostream & stream, std::wstring name, int type, int width, int length);

	bool ChangeBlack2ColorImage(std::wstring sRgbColor1, std::wstring sRgbColor2, _drawing_state_ptr & drawing_state);
	_color CorrectSysColor(int nColorCode, _drawing_state_ptr & drawing_state);
	
	std::wstring convert_custom_shape(_drawing_state_ptr & drawing_state);
};

}
