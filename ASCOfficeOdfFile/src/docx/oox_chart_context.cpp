#include "../odf/precompiled_cpodf.h"

#include <boost/foreach.hpp>
#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "mediaitems_utils.h"
#include "docx_rels.h"

#include "oox_chart_context.h"

namespace cpdoccore {
namespace oox {

class oox_chart_context::Impl
{
public:
	Impl(){}
    Impl(std::wstring const & name){}
    std::wstring name_;

    std::wstringstream  chartData_;

    std::wstring drawingName_;
    std::wstring drawingId_;
};

oox_chart_context_ptr oox_chart_context::create()
{
    return boost::make_shared<oox_chart_context>();
}
oox_chart_context_ptr oox_chart_context::create(std::wstring const & name)
{
    return boost::make_shared<oox_chart_context>(name);
}
oox_chart_context::oox_chart_context()
					: impl_(new oox_chart_context::Impl())
{
}
oox_chart_context::oox_chart_context(std::wstring const & name)
							: impl_(new oox_chart_context::Impl(name))
{
}
std::wostream & oox_chart_context::chartData()
{
    return impl_->chartData_;
}

void oox_chart_context::write_to(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"c:chartSpace")
		{          
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
            CP_XML_ATTR(L"xmlns:c", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
		
			CP_XML_NODE(L"c:lang")
			{
				CP_XML_ATTR(L"val",L"en-US");
			}
			CP_XML_NODE(L"c:chart")
			{
				title_.oox_serialize(CP_XML_STREAM());
				plot_area_.oox_serialize(CP_XML_STREAM());
				legend_.oox_serialize(CP_XML_STREAM());
				
				CP_XML_NODE(L"c:plotVisOnly")
				{
					CP_XML_ATTR(L"val",1);
				}	
			}
			oox_chart_shape shape;
			shape.set(graphic_properties_,fill_);
			shape.oox_serialize(CP_XML_STREAM());
	

		}
	}

}

oox_chart_context::~oox_chart_context()
{
}

bool oox_chart_context::empty() const
{
    return false;//impl_->empty();
}


}
}

