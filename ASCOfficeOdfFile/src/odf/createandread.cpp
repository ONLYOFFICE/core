

#include "office_elements_create.h"
#include <cpdoccore/xml/sax.h>
#include <sstream>
#include <logging.h>

namespace cpdoccore { 
namespace odf {

class document_context;

bool create_element_and_read(xml::sax * Reader,
                             const ::std::wstring & Ns,
                             const ::std::wstring & Name,
                             office_element_ptr & _Element,
                             document_context * Context,
                             bool isRoot)
{
    if (office_element_ptr elm = office_element_creator::get()->create(Ns, Name, Context, isRoot))
    {
		elm->read_sax( Reader );
        elm->afterReadContent();
        
        if (_Element) // элемент читается повторно
        {
            _CP_LOG << L"[error] : duplicate element (" << Ns << L":" << Name << L")\n";
        }

        _Element = elm;
        return true;
    }
    else
    {
        _CP_LOG << L"[error] : create element failed (" << Ns << L":" << Name << L")\n";

        not_applicable_element(L"[!!!]", Reader, Ns, Name);
    }
    return false;
}

bool create_element_and_read(xml::sax * Reader,
                             const ::std::wstring & Ns,
                             const ::std::wstring & Name,
                             office_element_ptr_array & _Elements,
                             document_context * Context,
                             bool isRoot)
{
    office_element_ptr elm;
    if (create_element_and_read(Reader, Ns, Name, elm, Context, isRoot))
    {
        _Elements.push_back(elm);
        return true;
    }
    return false;
}



}
}
