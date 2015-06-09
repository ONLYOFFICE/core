#pragma once

#include <vector>
#include <string>

namespace cpdoccore { 
namespace oox {

class rels;

class hyperlinks
{
public:
	enum _type_place
	{
		document_place,
		endnote_place,
		footnote_place,
		comment_place

	};

	struct _ref
	{
		std::wstring	href;
		_type_place		type_place;
		bool			drawing;
		std::wstring	id;
		bool			used_rels;
	};

    std::wstring add(const std::wstring & href, _type_place type_place, bool drawing);

	_ref  last();
    
	void dump_rels(rels & Rels, _type_place type);


private:
    
	std::vector<_ref> hrefs_;
};

}
}
