/// Реализация интерфейса через XmlUtils::CXmlLiteReader

#pragma once

//#include <iosfwd>
#include "../../include/cpdoccore/xml/sax.h"
#include "../../include/cpdoccore/CPSharedPtr.h"

namespace cpdoccore {
namespace xml {

sax_ptr create_sax_xmllite(const wchar_t * FileName);
//sax_ptr create_sax_xmllite( shared_ptr< std::istream >::Type istreamPtr);
//sax_ptr create_sax_xmllite( std::istream & istreamVal );

} 
}