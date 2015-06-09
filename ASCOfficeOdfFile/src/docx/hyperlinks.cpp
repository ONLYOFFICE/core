
#include "hyperlinks.h"
#include "docx_rels.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <cpdoccore/xml/utils.h>

namespace cpdoccore { 
namespace oox {

hyperlinks::_ref  hyperlinks::last()
{
	_ref r={};
	if (hrefs_.size()>0)
		r = hrefs_.back();

	return r;
}

std::wstring hyperlinks::add(const std::wstring & href, _type_place type_place, bool drawing)
{
	std::wstring id = std::wstring(L"rHpId") + boost::lexical_cast<std::wstring>(hrefs_.size()+1);
	
	_ref r ={xml::utils::replace_text_to_xml(href), type_place, drawing, id, false};
	
	hrefs_.push_back(r);

	return id;
}


void hyperlinks::dump_rels(rels & Rels, _type_place type)
{
    size_t i = 0;
    BOOST_FOREACH(_ref & elm, hrefs_)
    {
 		if (elm.used_rels)continue; // уже использовали этот релс

		if (elm.type_place == type)
		{
			Rels.add( relationship(elm.id, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink", elm.href, L"External" ) );
			elm.used_rels = true;
		}
    }
}

}
}
