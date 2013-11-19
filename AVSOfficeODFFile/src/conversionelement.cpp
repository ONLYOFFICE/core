#include "precompiled_cpodf.h"
#include "conversionelement.h"
#include "logging.h"

namespace cpdoccore { 
namespace oox {

void conversion_element::docx_convert(docx_conversion_context & Context) 
{
    _CP_LOG(info) << L"[warning] empty conversion_element::docx_convert: \"" << typeid(*this).name() << L"\"\n";
}

void conversion_element::xlsx_convert(xlsx_conversion_context & Context)
{
    _CP_LOG(info) << L"[warning] empty conversion_element::xlsx_convert: \"" << typeid(*this).name() << L"\"\n";
}

}
}
