#include <string>

namespace cpdoccore { 
namespace odf {

namespace xmlns {

struct office_xmlns
{
    std::wstring ns;
    std::wstring value;
};

extern const office_xmlns office;
extern const office_xmlns style;
extern const office_xmlns text;
extern const office_xmlns table;
extern const office_xmlns draw;
extern const office_xmlns fo;
extern const office_xmlns xlink;
extern const office_xmlns dc;
extern const office_xmlns meta;
extern const office_xmlns number;
extern const office_xmlns svg;
extern const office_xmlns chart;
extern const office_xmlns dr3d;
extern const office_xmlns math;
extern const office_xmlns form;
extern const office_xmlns script;
extern const office_xmlns ooo;
extern const office_xmlns ooow;
extern const office_xmlns oooc;
extern const office_xmlns dom;
extern const office_xmlns xforms;
extern const office_xmlns xsd;
extern const office_xmlns xsi;
extern const office_xmlns rpt;
extern const office_xmlns of;
extern const office_xmlns rdfa;

std::wostream & serialize_all_ns(std::wostream & _Wostream);

}
}
}
