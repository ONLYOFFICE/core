#pragma once

#include <sstream>

#include <common.h>
#include "external_items.h"

namespace oox {

class xlsx_drawings_rels;
typedef _CP_PTR(xlsx_drawings_rels) xlsx_drawings_rels_ptr;

struct drawing_elm
{
    drawing_elm(std::wstring const & _filename, std::wstring const & _content, xlsx_drawings_rels_ptr _rels)
        : filename(_filename), content(_content), rels(_rels)
    {}
    
    std::wstring			filename;
    std::wstring			content;
	
	xlsx_drawings_rels_ptr	rels;
};

class rels;

class xlsx_drawings_rels
{
public:
    xlsx_drawings_rels();
    ~xlsx_drawings_rels();
    static xlsx_drawings_rels_ptr create();

public:
    void add(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
		external_items::Type type
        );

    void dump_rels(rels & Rels);

private:
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
