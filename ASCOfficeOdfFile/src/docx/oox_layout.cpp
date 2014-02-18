#include "precompiled_cpodf.h"
#include "oox_layout.h"
#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <cpdoccore/CPHash.h>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "..\odf\style_text_properties.h"

namespace cpdoccore {
namespace oox {


void oox_layout::oox_serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE(L"c:layout")
        {
			//CP_XML_NODE(L"c:manualLayout") //вручную созданное описание и место
			//{
			//	CP_XML_NODE(L"c:layoutTarget") //вручную созданное описание и место
			//	{
			//		CP_XML_ATTR(L"val", "inner"); //все позиции относительно внешнего объекта (а не документа в целом)
			//	}
			//	//<c:xMode val="edge"/> 
			//	//<c:yMode val="edge"/>
			//	//<c:x val="3.9382768941490093E-2"/>
			//	//<c:y val="0.37756627842484747"/>
			//	//<c:w val="0.67611762675758824"/>
			//	//<c:h val="0.53420410427066001"/>
			//}
        }
    }

}

}
}
