#include "precompiled_cpodf.h"

#include "office_elements_create.h"

#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include "logging.h"

namespace cpdoccore { 
namespace odf {

namespace {
    static boost::mutex create_lock_;
}
    
office_element_creator * office_element_creator::instance_ = NULL;

office_element_creator::office_element_creator() : rw_lock_( new boost::shared_mutex() )
{
}

office_element_ptr office_element_creator::create(const ::std::wstring & ns, const ::std::wstring & name, document_context * Context, bool isRoot) const
{
    const std::wstring fullName = ns + (ns.size() > 0 ? L":" : L"") + name;

    boost::shared_lock< boost::shared_mutex > lock( *rw_lock_ );

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
        boost::mutex::scoped_lock lock(create_lock_);
        if (!instance_)
        {
            office_element_creator * temp = new office_element_creator();
            instance_ = temp;                
        }
    }

    return instance_;
}

bool office_element_creator::register_element(const std::wstring &ns, const std::wstring & name, CreateFuncImpl f)
{
    const std::wstring fullName = ns + (ns.size() > 0 ? L":" : L"") + name;

    boost::unique_lock< boost::shared_mutex > lock( *rw_lock_ );

    MapType::iterator i = map_.find( fullName );
    if ( i == map_.end() )
    {
        map_.insert(  MapType::value_type(fullName, f) );        
    }
    else
    {
        // TODO: элемент уже есть, непонятно что делать
        CP_ASSERT(false);
        i->second = f;
    }
    return true;
}

void not_applicable_element(const std::wstring & Current,const std::wstring & Ns, const std::wstring & Name)
{
#ifdef _DEBUG
	std::wstringstream ss;
    ss << L"[warning] : element " << Ns << L":" << Name << " not applicable in current place";
    if (!Current.empty())
        ss << L" ("<< Current << ")";
    ss << L"\n";
    _CP_LOG(error) << ss.str();
#endif
    
}

void not_applicable_element(const office_element * CurrentElm, const std::wstring & Ns, const std::wstring & Name)
{
    std::wstringstream ss;
    if (CurrentElm)
        ss << CurrentElm->get_ns() << L":" << CurrentElm->get_name();
    not_applicable_element(ss.str(), Ns, Name);
}

bool create_element(const ::std::wstring & Ns,
                             const ::std::wstring & Name,
                             office_element_ptr & _Element,
                             document_context * Context,
                             bool isRoot)
{
    if (office_element_ptr elm = office_element_creator::get()->create(Ns, Name, Context, isRoot))
    {
        if (_Element) // элемент создается повторно
        {
            std::wstringstream ss;
            ss << L"[warning] : duplicate element (" << Ns << L":" << Name << L")\n";
            _CP_LOG(error) << ss.str();
        }

        _Element = elm;
        return true;
    }
    else
    {
#ifdef _DEBUG
		std::wstringstream ss;
        ss << L"[warning] : create element failed (" << Ns << L":" << Name << L")\n";
        _CP_LOG(error) << ss.str();
#endif
        not_applicable_element(L"[!!!]", 0, Ns, Name);
    }
    return false;
}

bool create_element(const ::std::wstring & Ns,
                             const ::std::wstring & Name,
                             office_element_ptr_array & _Elements,
                             document_context * Context,
                             bool isRoot)
{
    office_element_ptr elm;
    if (create_element(Ns, Name, elm, Context, isRoot))
    {
        _Elements.push_back(elm);
        return true;
    }
    return false;
}


}
}

