#include "precompiled_cpodf.h"
#include "xlsx_hyperlinks.h"
#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/formulasconvert.h>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include "docx_rels.h"

namespace cpdoccore {
namespace oox {

class xlsx_hyperlinks::Impl
{
public:
    Impl() {}
    ~Impl() {}

    struct record
    {
        std::wstring ref;
        std::wstring display;
        std::wstring location;
        std::wstring id;
        std::wstring href;
        std::wstring type;
	};
	
	void dump_rels(rels & Rels) const
	{
		BOOST_FOREACH(const record & rec, records_)
		{
			if (rec.type == L"External")
			{
				Rels.add( relationship(rec.id, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink", rec.location, rec.type) );
			}
		}
	}

    std::wstring add(std::wstring const & ref, std::wstring const & target1, std::wstring const & display)
    {
		std::wstring target =target1;

		int res = target.rfind(L"/");
		if (res+1==target.length())
		{
			target.resize(target.length()-1);
		}
		
		record r;
        r.ref = ref;      
		r.display = display;
		r.id = std::wstring(L"hId") + boost::lexical_cast<std::wstring>(records_.size()+1);

        if (boost::algorithm::starts_with(target, "#"))//ссыль на страницу или метку в текущем документе
        {
			//адресация всегда на ячейку ...
			int pos =target.find(L".");
			if (pos <0)
			{
				target = target + std::wstring(L".A1");
			}
            r.location = converter_.convert_ref(std::wstring(target.begin() + 1, target.end()));
			r.type = L"Internal" ;
        }else
		{
			r.location = converter_.convert_spacechar(target);
			r.type = L"External" ;
		}
		if (r.display.length()<1)
			r.display =target;
        records_.push_back(r);

		return r.id;
    }

    void xlsx_serialize(std::wostream & _Wostream) const
    {
        BOOST_FOREACH(record const & r, records_)
        {
            CP_XML_WRITER(_Wostream)
            {
                CP_XML_NODE(L"hyperlink")
                {
                    CP_XML_ATTR(L"ref", r.ref);
                    CP_XML_ATTR(L"display", r.display);
                  
					if (!r.location.empty() && r.type == L"Internal")
                    {
                        CP_XML_ATTR(L"location", r.location);
                    }

                    if (!r.id.empty() && r.type == L"External")
                    {
                        CP_XML_ATTR(L"r:id", r.id);                    
                    }
                }
            }
        }
    }


private:
    std::vector<record> records_;
    formulasconvert::odf2oox_converter converter_;

};

xlsx_hyperlinks::xlsx_hyperlinks() : impl_( new xlsx_hyperlinks::Impl() )
{
}

xlsx_hyperlinks::~xlsx_hyperlinks()
{
}

std::wstring xlsx_hyperlinks::add(std::wstring const & ref, std::wstring const & target, std::wstring const & display)
{   
    return impl_->add(ref, target, display);
}

void xlsx_hyperlinks::xlsx_serialize(std::wostream & _Wostream) const
{
    return impl_->xlsx_serialize(_Wostream);
}
void xlsx_hyperlinks::dump_rels(rels & Rels) const
{
	return impl_->dump_rels(Rels);
}

}
}
