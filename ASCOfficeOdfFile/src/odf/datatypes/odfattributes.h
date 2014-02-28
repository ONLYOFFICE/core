#ifndef _CPDOCCORE_ODFATTRIBUTES_H_
#define _CPDOCCORE_ODFATTRIBUTES_H_

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include "errors.h"
#include "logging.h"

namespace cpdoccore {
namespace xml {

template <class StringT, class V>
class attributes_get_val_impl_parse
{
public:
    static typename optional<V>::Type get_val(attributes<StringT> & attr, const StringT & QualifiedName)
    {
        try 
        { 
            if (attributes<StringT>::value_type val = attr.get(QualifiedName)) 
                return optional<V>::Type( V::parse(*val) );
        }
        catch(odf::errors::invalid_attribute &)
        {         
            attributes<StringT>::value_type val = attr.get(QualifiedName);
#ifdef _DEBUG
            _CP_LOG(error) << L"[warning] : invalud attribute value [" << QualifiedName << L":" << ( (val) ? (*val) : (L"?")) << L"]\n";
#endif
        }
        return optional<V>::Type();
    }
};

#define APPLY_PARSE_XML_ATTRIBUTES(V) \
namespace xml { \
template <class StringT> \
class attributes_get_val_impl<StringT, V >\
{\
public:\
    static typename optional< V >::Type get_val(attributes<StringT> & attr, const StringT & QualifiedName)\
    {\
        return attributes_get_val_impl_parse<StringT, V>::get_val(attr, QualifiedName);\
    }\
};\
}

}
}

#endif
