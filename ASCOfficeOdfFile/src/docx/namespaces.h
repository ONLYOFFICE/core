#pragma once

#include <string>

namespace cpdoccore { 
namespace oox {
namespace xmlns {

	struct office_xmlns
	{
		std::wstring ns;
		std::wstring value;
	};

	extern const office_xmlns rels;
	extern const office_xmlns types;

	extern const office_xmlns ve;
	extern const office_xmlns o;
	extern const office_xmlns r;
	extern const office_xmlns m;
	extern const office_xmlns v;
	extern const office_xmlns wp;
	extern const office_xmlns w10;
	extern const office_xmlns w;
	extern const office_xmlns wne;

}
}
}
