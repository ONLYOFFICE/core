#pragma once

#include <iosfwd>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/xml/attributes.h>
#include "mediaitems.h"

namespace cpdoccore {
namespace oox {

struct _pptx_drawing;

class rels;

class pptx_drawings;
typedef _CP_PTR(pptx_drawings) pptx_drawings_ptr;

class pptx_drawings
{
public:
    pptx_drawings();
    ~pptx_drawings();

    void add(_pptx_drawing const & d,
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

	void serialize(std::wostream & _Wostream);    
    
	static pptx_drawings_ptr create();

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
    
};

}
}
