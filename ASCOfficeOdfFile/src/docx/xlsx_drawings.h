#pragma once

//#include <iosfwd>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/xml/attributes.h>
#include "mediaitems.h"

namespace cpdoccore {
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

struct _xlsx_drawing;
class rels;

class xlsx_drawings
{
public:
    xlsx_drawings();
    ~xlsx_drawings();
    static xlsx_drawings_ptr create();

public:
    void add(_xlsx_drawing const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
		mediaitems::Type type
        );
    void add(/**/
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
		mediaitems::Type type
        );
    bool empty() const;

    void dump_rels(rels & Rels);
    friend void xlsx_serialize(std::wostream & _Wostream, xlsx_drawings const & val);    

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
    
};

}
}
