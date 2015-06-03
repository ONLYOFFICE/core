#pragma once

#include <string>

#include "../CPSharedPtr.h"

namespace cpdoccore { 

namespace xml {

class sax;

class attributes;

typedef attributes attributes_wc;

typedef shared_ptr< attributes_wc >::Type attributes_wc_ptr;

}
    
namespace common {

// Базовый класс для элемента, который может быть прочитан sax-reader -ом
// Для каждого элемента будут вызваны методы
// add_attributes, add_child_element, add_text как колбеки для чтения соответствующих данных

class read_doc_element 
{
public:
    bool read_sax( xml::sax * Reader);
    virtual ~read_doc_element() = 0; 

    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name) = 0;
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes ) = 0;
    virtual void add_text(const std::wstring & Text) = 0;

};

inline read_doc_element::~read_doc_element()
{
}

}
}
