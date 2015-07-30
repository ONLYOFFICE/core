#pragma once

#include <sstream>

#include <common.h>
#include "external_items.h"

namespace oox {

class xlsx_drawings;
typedef _CP_PTR(xlsx_drawings) xlsx_drawings_ptr;

struct drawing_elm
{
    drawing_elm(std::wstring const & _filename, std::wstring const & _content, xlsx_drawings_ptr _drawings)
        : filename(_filename), content(_content), drawings(_drawings)
    {}
    
    std::wstring filename;
    std::wstring content;
    xlsx_drawings_ptr drawings;
};

class rels;

class xlsx_drawings
{
public:
    xlsx_drawings();
    ~xlsx_drawings();
    static xlsx_drawings_ptr create();

public:
	void add(std::wstring const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
		external_items::Type type
        );
    void add(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
		external_items::Type type
        );
    bool empty() const;

    void dump_rels(rels & Rels);
   
	void serialize(std::wostream & _Wostream);  

private:
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
