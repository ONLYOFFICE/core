#pragma once
#include "oox_package.h"


namespace oox {

class oox_chart_context;
typedef _CP_PTR(oox_chart_context) oox_chart_context_ptr;

class oox_chart_context
{
public:
    oox_chart_context();
    ~oox_chart_context();

	std::wostream & chartData();

	void write_to	(std::wostream & strm);
	void dump_rels	(rels & Rels);

	static oox_chart_context_ptr create();
	void add_rels(	bool isInternal,
					std::wstring const & rid,
					std::wstring const & target,
					external_items::Type type);

private:
    class Impl;
    _CP_PTR(Impl) impl_;
              
};

}
