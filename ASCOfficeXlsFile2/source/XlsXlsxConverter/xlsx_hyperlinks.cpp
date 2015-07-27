#include "xlsx_hyperlinks.h"
#include "oox_rels.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <simple_xml_writer.h>

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
		if (res >=0 && res+1==target.length())
		{
			target.resize(target.length()-1);
		}
		
		record r;
        r.ref = ref;      
		r.display = display;
		r.id = std::wstring(L"hId") + boost::lexical_cast<std::wstring>(records_.size()+1);
		r.location = target;
		r.type = L"External" ;

		if (r.display.length()<1)
			r.display =target;
        records_.push_back(r);

		return r.id;
    }
   void serialize(std::wostream & _Wostream) const
    {
		if (records_.size() < 1) return;

		CP_XML_WRITER(_Wostream)
		{       
            CP_XML_NODE(L"hyperlinks")
            {			
				BOOST_FOREACH(record const & r, records_)
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
    }



 private:
    std::vector<record> records_;

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

void xlsx_hyperlinks::dump_rels(rels & Rels) const
{
	impl_->dump_rels(Rels);
}

void xlsx_hyperlinks::serialize(std::wostream & _stream) const
{
	impl_->serialize(_stream);
}

}

