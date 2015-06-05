#ifndef _CPDOCCORE_ODFATTRIBUTES_H_
#define _CPDOCCORE_ODFATTRIBUTES_H_

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include "errors.h"
#include <logging.h>

namespace cpdoccore {
namespace xml {

template <class V>
class attributes_get_val_impl_parse
{
public:
    typedef typename optional<V>::Type optional_v_type;

    static optional_v_type get_val(attributes & attr, const std::wstring & QualifiedName)
    {
        try 
        { 
            if (attributes::value_type val = attr.get(QualifiedName))
            {
                return optional_v_type( V::parse(*val) );
            }
        }
        catch(odf::errors::invalid_attribute &)
        {         
            attributes::value_type val = attr.get(QualifiedName);

            _CP_LOG << L"[error] : invalud attribute value [" << QualifiedName << L":" << ( (val) ? (*val) : (L"?")) << L"]\n";
        }
        return optional_v_type();
    }
};

#define APPLY_PARSE_XML_ATTRIBUTES(V) \
namespace xml { \
template<> \
class attributes_get_val_impl<V >\
{\
public:\
    typedef optional<V>::Type optional_v_type; \
    static optional_v_type get_val(attributes & attr, const std::wstring & QualifiedName)\
    {\
        return attributes_get_val_impl_parse<V>::get_val(attr, QualifiedName);\
    }\
};\
}

}
}

#endif
