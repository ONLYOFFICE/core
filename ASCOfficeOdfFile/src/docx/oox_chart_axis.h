#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPSharedPtr.h>

#include "oox_chart_shape.h"

namespace cpdoccore {
namespace oox {

class oox_axis_content;
typedef _CP_PTR(oox_axis_content) oox_axis_content_ptr;

class oox_axis_content
{
public:
	odf::chart::oox_typeconvert oox_typeconvert_;

	oox_axis_content(int type/*,std::wstring name*/)
	{
        id_ = abs(*((int*)(this)));
		type_=type;		//dimension
	}
	~oox_axis_content(){}
	
	static oox_axis_content_ptr create(int type/*,std::wstring name*/);

	void oox_serialize(std::wostream & _Wostream);
	void oox_serialize_content(std::wostream & _Wostream);

	int get_Id(){return id_;}
	void add_CrossedId (int id){cross_id_.push_back( id);}

	int id_;
	std::vector<int> cross_id_;
	odf::chart::axis content_;

	int type_;
};

}
}
