#ifndef _CPDOCCORE_OOX_CONVERSIONELEMENT_
#define _CPDOCCORE_OOX_CONVERSIONELEMENT_

#ifdef _MSC_VER
#pragma once
#endif

namespace cpdoccore { 
namespace oox {

class docx_conversion_context;
class xlsx_conversion_context;

class conversion_element
{
public:
    virtual void docx_convert(docx_conversion_context & Context)/* const*/;
    virtual void xlsx_convert(xlsx_conversion_context & Context)/* const*/;
    virtual ~conversion_element() = 0;
};

inline conversion_element::~conversion_element()
{
}

}
}

#endif
