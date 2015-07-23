#pragma once

#include "../Common/common.h"

namespace oox {

class rels;

class xlsx_hyperlinks
{
public:
    xlsx_hyperlinks();
    ~xlsx_hyperlinks();

    std::wstring add( std::wstring const & ref, std::wstring const & target, std::wstring const & display);

	void dump_rels(rels & Rels) const;
	void serialize(std::wostream & _stream) const;

private:
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}