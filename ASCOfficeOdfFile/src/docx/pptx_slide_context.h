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
	void set_property(odf::_property p);
	std::vector<odf::_property> & get_properties();
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
