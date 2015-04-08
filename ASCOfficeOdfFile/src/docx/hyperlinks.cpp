#include "../odf/precompiled_cpodf.h"
#include "hyperlinks.h"
#include "docx_rels.h"

#include <boost/foreach.hpp>
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

std::wstring hyperlinks::add(const std::wstring & href,bool draw)
{
	std::wstring id = std::wstring(L"rHpId") + boost::lexical_cast<std::wstring>(hrefs_.size()+1);
	
	_ref r ={xml::utils::replace_text_to_xml(href),draw,id,false};
	hrefs_.push_back(r);
	//по идее для гиперлинка с рисованого объекта нужно ввести флаг использования .. пока будем проверять последний на тип (глупо, но прокатит)

	return id;
}


void hyperlinks::dump_rels(rels & Rels)
{
    size_t i = 0;
    BOOST_FOREACH(_ref & elm, hrefs_)
    {
 		if (elm.used_rels)continue; // уже использовали этот релс выше(колонтитул ....)
		Rels.add( relationship(elm.id, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink", elm.href, L"External" ) );
		elm.used_rels = true;
    }
}

}
}
