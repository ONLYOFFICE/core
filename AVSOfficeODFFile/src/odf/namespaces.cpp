#include "precompiled_cpodf.h"
#include "namespaces.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <cpdoccore/xml/serialize.h>

namespace cpdoccore { 
namespace odf {
namespace xmlns {

const office_xmlns office = { L"office", L"urn:oasis:names:tc:opendocument:xmlns:office:1.0" };
const office_xmlns style = { L"style", L"urn:oasis:names:tc:opendocument:xmlns:style:1.0" };
const office_xmlns text = { L"text", L"urn:oasis:names:tc:opendocument:xmlns:text:1.0" };
const office_xmlns table = { L"table", L"urn:oasis:names:tc:opendocument:xmlns:table:1.0" };
const office_xmlns draw = { L"draw", L"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0" };
const office_xmlns fo = { L"fo", L"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0" };
const office_xmlns xlink = { L"xlink", L"http://www.w3.org/1999/xlink" };
const office_xmlns dc = { L"dc", L"http://purl.org/dc/elements/1.1/" };
const office_xmlns meta = { L"meta", L"urn:oasis:names:tc:opendocument:xmlns:meta:1.0" };
const office_xmlns number = { L"number", L"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0" };
const office_xmlns svg = { L"svg", L"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0" };
const office_xmlns chart = { L"chart", L"urn:oasis:names:tc:opendocument:xmlns:chart:1.0" };
const office_xmlns dr3d = { L"dr3d", L"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0" };
const office_xmlns math = { L"math", L"http://www.w3.org/1998/Math/MathML" };
const office_xmlns form = { L"form", L"urn:oasis:names:tc:opendocument:xmlns:form:1.0" };
const office_xmlns script = { L"script", L"urn:oasis:names:tc:opendocument:xmlns:script:1.0" };
const office_xmlns ooo = { L"ooo", L"http://openoffice.org/2004/office" };
const office_xmlns ooow = { L"ooow", L"http://openoffice.org/2004/writer" };
const office_xmlns oooc = { L"oooc", L"http://openoffice.org/2004/calc" };
const office_xmlns dom = { L"dom", L"http://www.w3.org/2001/xml-events" };
const office_xmlns xforms = { L"xforms", L"http://www.w3.org/2002/xforms" };
const office_xmlns xsd = { L"xsd", L"http://www.w3.org/2001/XMLSchema" };
const office_xmlns xsi = { L"xsi", L"http://www.w3.org/2001/XMLSchema-instance" };
const office_xmlns rpt = { L"rpt", L"http://openoffice.org/2005/report" };
const office_xmlns of = { L"of", L"urn:oasis:names:tc:opendocument:xmlns:of:1.2" };
const office_xmlns rdfa = { L"rdfa", L"http://docs.oasis-open.org/opendocument/meta/rdfa#" };

office_xmlns const * const all_office_xmlns[] = 
{
    &office, &style, &text, &table, &draw,
    &fo, &xlink, &dc, &meta, &number, &svg,
    &chart, &dr3d, &math, &form, &script,
    &ooo, &ooow, &oooc, &dom, &xforms, &xsd,
    &xsi, &rpt, &of, &rdfa
};

std::wostream & serialize_all_ns(std::wostream & _Wostream)
{
    BOOST_FOREACH(office_xmlns const * const elm, all_office_xmlns)
    {
        xml::serialize_attribute(_Wostream, std::wstring(L"xmlns:") + elm->ns, elm->value);      
    }    
    return _Wostream;
}

}
}
}
