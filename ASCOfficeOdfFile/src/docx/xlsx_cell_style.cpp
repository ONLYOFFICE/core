
#include "xlsx_cell_style.h"

#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

void xlsx_serialize(std::wostream & _Wostream, const xlsx_cell_style & cell_style)
{
    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"cellStyle")
        {
            if (cell_style.name)
                CP_XML_ATTR(L"name", cell_style.name.get());

            if (cell_style.xfId)
                CP_XML_ATTR(L"xfId", cell_style.xfId.get());

            if (cell_style.builtinId)
                CP_XML_ATTR(L"builtinId", cell_style.builtinId.get());        
        }
    }

    //_Wostream << L"<cellStyle ";

    //if (cell_style.name)
    //    _Wostream << L"name=\"" << cell_style.name.get() << L"\" ";
    //if (cell_style.xfId)
    //    _Wostream << L"xfId=\"" << cell_style.xfId.get() << L"\" ";
    //if (cell_style.builtinId)
    //    _Wostream << L"builtinId=\"" << cell_style.builtinId.get() << L"\" ";

    //_Wostream << L"/>";    
}

}
}
