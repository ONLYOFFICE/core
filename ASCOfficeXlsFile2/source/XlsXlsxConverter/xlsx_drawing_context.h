#pragma once

#include <vector>
#include <iosfwd>
#include <string.h>

#include "xlsx_drawings.h"

#include "ShapeType.h"	

namespace oox {

class external_items;

class xlsx_drawing_context_handle
{
public:
    xlsx_drawing_context_handle(external_items & items);
    ~xlsx_drawing_context_handle();
    
    std::pair<std::wstring, std::wstring> add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings);
    const std::vector<drawing_elm> & content() const;

    friend class xlsx_drawing_context;
private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

struct _color
{
	_color() : index(-1), bScheme(false), bPalette(false){}
	std::wstring	rgb;
	int				index;
	bool			bScheme;
	bool			bPalette;
};

struct _rect
{
	_rect() : left(0), top(0), right(0),bottom(0){}

    int   left;
    int   top;
    int   right;
    int   bottom;

};

class _drawing_state
{
public:
	_drawing_state() : isInternal(false), shape_id(msosptNotPrimitive),  flipH(false), flipV(false), bTextBox(false), bWordArt(false)
	{
		id = -1;
		memset(image_crop, 0, 4 * sizeof(int));
		rotation = 0;
	}

	int						shape_type;

	external_items::Type	type;
	std::wstring			name;
	std::wstring			description;

	std::wstring			anchor;
	std::wstring			shape;

	std::wstring			image_target;
	int						image_crop[4];
	bool					image_crop_enabled;
	
	int						id;
	MSOSPT					shape_id;
	
	bool					flipV;
	bool					flipH;
	std::wstring			hyperlink;
	int						rotation;

	std::wstring			path;
	_rect					path_rect;
	
	bool					isInternal;
	bool					bTextBox;
	bool					bWordArt;

	struct _line
	{
		_line() {opacity = 0; type = L"solidFill"; style = L"simple"; width = 0;}
		_color			color;
		int				opacity;
		std::wstring	type;
		std::wstring	style;
		long			width;
	}line;

};
struct _hlink_desc
{
    std::wstring hId;
    std::wstring hRef;
};

class xlsx_drawing_context
{
public:
    xlsx_drawing_context(xlsx_drawing_context_handle & h);
	~xlsx_drawing_context(){}

	xlsx_drawings_ptr get_drawings();
	bool empty();	
	
	bool start_drawing(int type);	
		void start_image();
		void start_shape(int type);

		void set_id			(long id);
		void set_FlipH		();
		void set_FlipV		();
		void set_shape_id	(int id);

        void set_name		(const std::wstring & str);
        void set_description(const std::wstring & str);
		
		void set_crop_top	(long val);
		void set_crop_bottom(long val);
		void set_crop_left	(long val);
		void set_crop_right	(long val);

		void set_rotation	(long val);

        void set_line_color	(const std::wstring & color);
		void set_line_color	(int index, int type);
		void set_line_type	(long val);
		void set_line_style	(long val);
		void set_line_width (long val);

        void set_image		(const std::wstring & str);
        void set_anchor		(const std::wstring & str);
        void set_properties	(const std::wstring & str);
        void set_hyperlink	(const std::wstring & str);

		void set_path_rect	(_rect & rect);
		void set_path		(const std::wstring & path);
		
//------------------------------------------------------------------------------		
		void serialize				(std::wostream & stream);
		void serialize_pic			(std::wstring rId);	
		void serialize_shape();			
		void serialize_color		(std::wostream & stream, const _color &color);
		void serialize_line			(std::wostream & stream);
		void serialize_fill			(std::wostream & stream);
		void serialize_bitmap_fill	(std::wostream & stream, std::wstring rId, const std::wstring ns = L"a:");
		void serialize_none_fill	(std::wostream & stream);
		void serialize_xfrm			(std::wostream & stream);

	void end_drawing();
private:

	std::vector<_drawing_state>		drawing_state;
	
	xlsx_drawing_context_handle	  & handle_;
	xlsx_drawings_ptr				xlsx_drawings_;
	int								count_object;
	
	std::vector<_hlink_desc>		hlinks_;
};

}
