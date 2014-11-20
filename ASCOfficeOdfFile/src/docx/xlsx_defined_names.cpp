#include "precompiled_cpodf.h"
#include "xlsx_defined_names.h"

#include <vector>
#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <boost/unordered_set.hpp>
#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/formulasconvert.h>

namespace cpdoccore {
namespace oox {

class xlsx_defined_names::Impl
{
public:
    void add(std::wstring const & name, std::wstring const & ref)
    {
		int is_file_link = 0;

		if ((is_file_link = ref.find(L"\\")) >=0) return;
		if ((is_file_link = ref.find(L"/")) >=0) return;
	 
        formulasconvert::odf2oox_converter converter;
        std::wstring const f = converter.convert_named_ref(ref);
        name_and_ref_.push_back(name_and_ref(name, f));
    }

    void xlsx_serialize(std::wostream & _Wostream)
    {
        if (name_and_ref_.size() > 0)
        {
            CP_XML_WRITER(_Wostream)
            {
                CP_XML_NODE(L"definedNames")
                {
                    BOOST_FOREACH(name_and_ref const & elm, name_and_ref_)
                    {
                        CP_XML_NODE(L"definedName")
                        {
                            CP_XML_ATTR(L"name", elm.first);
                            CP_XML_CONTENT(elm.second);
                        }
                    }
                }
            }
        }
    }

private:
    typedef std::pair<std::wstring, std::wstring> name_and_ref;
    std::vector<name_and_ref> name_and_ref_;
};

xlsx_defined_names::xlsx_defined_names() : impl_(new xlsx_defined_names::Impl())
{
}

xlsx_defined_names::~xlsx_defined_names()
{
}

void xlsx_defined_names::add(std::wstring const & name, std::wstring const & ref)
{
    return impl_->add(name, ref);        
}

void xlsx_serialize(std::wostream & _Wostream, xlsx_defined_names const & val)
{
    return val.impl_->xlsx_serialize(_Wostream);
}

}
}
