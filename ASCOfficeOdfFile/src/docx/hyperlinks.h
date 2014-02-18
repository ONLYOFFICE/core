#pragma once

#include <vector>
#include <string>

namespace cpdoccore { 
namespace oox {

class rels;

class hyperlinks
{
public:
	struct _ref
	{
		std::wstring href;
		bool drawing;
		std::wstring id;
		bool used_rels;
	};

    std::wstring add(const std::wstring & href, bool drawing);

	_ref  last();
    
	void dump_rels(rels & Rels);


private:
    
	std::vector<_ref> hrefs_;
};

}
}
