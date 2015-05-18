

#include "office_elements_create.h"

#include <iostream>
#include <logging.h>
#include "skipelement.h"

namespace cpdoccore { 
namespace odf {

    
office_element_creator * office_element_creator::instance_ = NULL;

office_element_creator::office_element_creator()
{
}

office_element_ptr office_element_creator::create(const ::std::wstring & ns, const ::std::wstring & name, document_context * Context, bool isRoot) const
{
    const std::wstring fullName = ns + (ns.size() > 0 ? L":" : L"") + name;

	MapType::const_iterator i = map_.find( fullName );
    if (i != map_.end() )
    {
        office_element_ptr element = (i->second)();
        
        if (element)
        {
            element->setContext(Context);
        }
        
        if (element && isRoot)
            element->set_root(true);

        if (element)
            element->afterCreate();        

        return element;
    }
    else
    {
        return office_element_ptr();
    }
}



office_element_creator * office_element_creator::get() 
{
    if (!instance_)
    {
        office_element_creator * temp = new office_element_creator();
        instance_ = temp;                
    }

    return instance_;
}

bool office_element_creator::register_element(const std::wstring &ns, const std::wstring & name, CreateFuncImpl f)
{
    const std::wstring fullName = ns + (ns.size() > 0 ? L":" : L"") + name;

    MapType::iterator i = map_.find( fullName );
    if ( i == map_.end() )
    {
        map_.insert(  MapType::value_type(fullName, f) );        
    }
    else
    {
        // TODO: элемент уже есть, непонятно что делать
        i->second = f;
    }
    return true;
}

void not_applicable_element(const std::wstring & Current, xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
#ifdef _DEBUG
	std::wstringstream ss;
    ss << L"[warning] : element " << Ns << L":" << Name << " not applicable in current place";
    if (!Current.empty())
        ss << L" ("<< Current << ")";
    ss << L"\n";
    _CP_LOG(error) << ss.str();
#endif
    
    //if (Reader)
    //    _skip_element::skip_element_.read_sax(Reader);
}

void not_applicable_element(const office_element * CurrentElm, xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    std::wstringstream ss;
    if (CurrentElm)
        ss << CurrentElm->get_ns() << L":" << CurrentElm->get_name();
    not_applicable_element(ss.str(), Reader, Ns, Name);
}

void not_applicable_element(const office_element * CurrentElm, const std::wstring & Ns, const std::wstring & Name)
{
    return not_applicable_element(CurrentElm, NULL, Ns, Name);
}

void not_applicable_element(const std::wstring & Current, const std::wstring & Ns, const std::wstring & Name)
{
    return not_applicable_element(Current, NULL, Ns, Name);    
}

}
}

