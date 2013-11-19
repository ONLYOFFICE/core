#ifndef CPDOCCORE_XLSX_DRAWING_CONTEXT_H_
#define CPDOCCORE_XLSX_DRAWING_CONTEXT_H_

#include <string>
#include <boost/noncopyable.hpp>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>

#include "xlsx_drawings.h"

namespace cpdoccore { 

	namespace svg_path
	{
		struct _polyline;
	};
	
namespace oox {

struct simple_drawing_desc;
struct xlsx_table_position;
class xlsx_table_metrics;
class mediaitems;
class xlsx_drawings;

typedef _CP_PTR(xlsx_drawings) xlsx_drawings_ptr;

class xlsx_drawing_context_handle
{
public:
    xlsx_drawing_context_handle(mediaitems & items);
    ~xlsx_drawing_context_handle();
    
    std::pair<std::wstring, std::wstring> add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings);
    const std::vector<drawing_elm> & content() const;

    friend class xlsx_drawing_context;
private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

//class xlsx_drawings;
//typedef _CP_PTR(xlsx_drawings) xlsx_drawings_ptr;

class xlsx_drawing_context
{
public:
    xlsx_drawing_context(xlsx_drawing_context_handle & h);
    ~xlsx_drawing_context();

	void set_odf_packet_path(std::wstring path){odf_packet_path_ = path;}

    void start_shapes();
    void end_shapes();

    void start_drawing(std::wstring const & name);
	
	void set_rect(double width_pt, double height_pt, double x_pt, double y_pt);

	void set_translate(double x_pt, double y_pt);
	void set_scale(double cx_pt, double cy_pt);

	void set_anchor(std::wstring anchor, double x_pt, double y_pt);
	void set_property(odf::_property p);
	std::vector<odf::_property> & get_properties();
	void set_clipping(std::wstring & str );
	//...   
	void end_drawing();

	std::wstring add_hyperlink(std::wstring const & ref, bool object);

    void start_image(std::wstring const & path);
    void end_image();
   
	void start_chart(std::wstring const & path);
    void end_chart();

    void start_shape(int type);
	//...пока тока общие свойства ... частные для каждого объекта пооозже
    void end_shape();
	
	bool empty() const;

    void write_drawing(std::wostream & strm);
	std::wstring dump_path(std::vector<svg_path::_polyline> & path, double w,double h);

    xlsx_drawings_ptr get_drawings();

    void process_images(xlsx_table_metrics & table_metrics);
    void process_charts(xlsx_table_metrics & table_metrics);
    void process_shapes(xlsx_table_metrics & table_metrics);

	void process_position_properties(simple_drawing_desc & pic,xlsx_table_metrics & table_metrics,xlsx_table_position & from,xlsx_table_position & to);
	void process_common_properties(simple_drawing_desc& pic,_xlsx_drawing & drawing,xlsx_table_metrics & table_metrics);

private:
	void default_set();
	int hlinks_size_;
	std::wstring odf_packet_path_ ;
	float dpi_;
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
}

#endif
