#ifndef _CPDOCCORE_WRITEDOCELEMENT_H_
#define _CPDOCCORE_WRITEDOCELEMENT_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <iosfwd>

namespace cpdoccore { 

namespace xml {

class sax;
template <class StringT>
class attributes;

typedef attributes< ::std::wstring > attributes_wc;

typedef shared_ptr< attributes_wc >::Type attributes_wc_ptr;
}
    
namespace common {

class write_doc_element {
public:
    virtual ::std::wostream & write_stream( ::std::wostream & _Wostream ) = 0;
    virtual ~write_doc_element() = 0; 
};

inline write_doc_element::~write_doc_element()
{
}

}
}

#endif // #ifndef _CPDOCCORE_WRITEDOCELEMENT_H_
