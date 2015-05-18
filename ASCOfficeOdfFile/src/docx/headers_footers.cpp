
#include "headers_footers.h"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace cpdoccore { 
namespace oox {

std::wstring headers_footers::create_id(size_t i)
{
    return std::wstring(L"rHFId") + boost::lexical_cast<std::wstring>(i); 
}

std::wstring headers_footers::create_name(size_t i, headers_footers::Type _Type)
{
    return ((_Type == header || _Type == headerLeft || _Type == headerFirst) ?  std::wstring(L"header") : std::wstring(L"footer") )
        + boost::lexical_cast<std::wstring>(i) + L".xml";
}

std::wstring headers_footers::add(const std::wstring & StyleName,
                                  const std::wstring & Content,
                                  Type type,
								  rels &_rels
                                  )
{
    size_++;
    const std::wstring id = create_id(size_);
    const std::wstring name = create_name(size_, type);
    instance_ptr inst = instance_ptr( new instance(id, Content, type, name) );
  
	BOOST_FOREACH(const relationship & r, _rels.relationships())
    {
		inst->rels_.add(r);
	}
    instances_[StyleName].push_back(inst);
	return id;
}

namespace 
{

std::wstring get_rel_type(headers_footers::Type _Type)
{
    if (_Type == headers_footers::header || _Type == headers_footers::headerLeft || _Type == headers_footers::headerFirst)
        return std::wstring(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/header");
    else
        return std::wstring(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer");
}
    
}

void headers_footers::dump_rels(rels & Rels) const//внешние релсы
{
    BOOST_FOREACH(const instances_map::value_type & instAr, instances_)
    {
        BOOST_FOREACH(const instance_ptr & inst, instAr.second)
        {
            Rels.add( relationship( inst->id_,  get_rel_type(inst->type_),  inst->name_, L"" ) );
        }
    }        
}

bool headers_footers::write_sectPr(const std::wstring & StyleName, std::wostream & _Wostream) const
{
    if (!instances_.count(StyleName))return false;

	bool first=false, left=false;
	bool res=false;
    BOOST_FOREACH(const instance_ptr & inst, instances_.at(StyleName))
    {
        std::wstring type = L"default";
		
		if ( inst->type_ == headerFirst || inst->type_ == footerFirst )
		{
			type = L"first";
			first=true;
		}
		else if ( inst->type_ == footerLeft || inst->type_ == headerLeft )
		{
			type = L"even";
			left=true;
		}

        std::wstring name = L"w:headerReference";  
		if (inst->type_ == footer || inst->type_ == footerLeft  || inst->type_ == footerFirst ) 
			name =L"w:footerReference";

        _Wostream << L"<" << name << L" r:id=\"" << inst->id_ << L"\" w:type=\"" << type << "\" />";
	
		if (first)_Wostream << L"<w:titlePg/>";
		res=true;
		
	}
	return res;
}

}
}
