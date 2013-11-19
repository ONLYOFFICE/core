#include "precompiled_cpodf.h"
#include "xlsx_cell_styles.h"
#include "xlsx_cell_style.h"
#include <cpdoccore/xml/simple_xml_writer.h>
#include <boost/foreach.hpp>

namespace cpdoccore {
namespace oox {

class xlsx_cell_styles::Impl
{
public:
    std::vector<xlsx_cell_style> cell_styles_;    
};

xlsx_cell_styles::xlsx_cell_styles() : impl_(new xlsx_cell_styles::Impl())
{
    xlsx_cell_style default_style;
    default_style.xfId = 0;
    default_style.builtinId = 0;
    default_style.name = L"Default";

    impl_->cell_styles_.push_back(default_style);
}

xlsx_cell_styles::~xlsx_cell_styles()
{}

void xlsx_cell_styles::xlsx_serialize(std::wostream & _Wostream) const
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"cellStyles")
        {
            CP_XML_ATTR(L"count", impl_->cell_styles_.size());

            BOOST_FOREACH(const xlsx_cell_style & s, impl_->cell_styles_)
            {
                ::cpdoccore::oox::xlsx_serialize(CP_XML_STREAM(), s);                    
            }
        }
    }

    //_Wostream << L"<cellStyles count=\"" << impl_->cell_styles_.size() << L"\">";    
    //
    //BOOST_FOREACH(const xlsx_cell_style & s, impl_->cell_styles_)
    //{
    //    ::cpdoccore::oox::xlsx_serialize(_Wostream, s);                    
    //}

    //_Wostream << L"</cellStyles>";
}


}
}
