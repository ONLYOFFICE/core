
#include "precompiled_cpxml.h"

#include "../../include/cpdoccore/CPSharedPtr.h"
#include "../../include/cpdoccore/xml/sax.h"
#include "sax_xmllite.h"
#include <sstream>

namespace cpdoccore {
namespace xml {

sax_ptr create_sax(const wchar_t * FileName)
{
    return create_sax_xmllite(FileName);
}

//sax_ptr create_sax(shared_ptr< std::istream >::Type istreamPtr)
//{
//    return create_sax_xmllite(istreamPtr);
//}
//
//sax_ptr create_sax(std::istream & istreamVal)
//{
//    return create_sax_xmllite(istreamVal);
//}

/*
sax_ptr create_sax(const std::string & String)
{
    shared_ptr< ::std::stringstream >::Type stream( new ::std::stringstream );
    (*stream) << String;
    return create_sax(stream);
}
*/

}
}
