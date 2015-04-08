#include "../precompiled_cpodf.h"
#include "markerstyle.h"

#include <ostream>

namespace cpdoccore { 
namespace odf {

std::wostream & operator << (std::wostream & _Wostream, const marker_style & _Val)
{
    //switch(_Val.get_type())
    //{
    //case marker_style::
    //    _Wostream << "none";
    //    break;
    //case marker_style::Single:
    //    _Wostream << "single";
    //    break;
    //case marker_style::Double:
    //    _Wostream << "double";
    //    break;
    //default:
    //    break;
    //}
	_Wostream << _Val.get();
    return _Wostream;
}

std::wstring marker_style::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

	int pos1,pos2;
	if ((pos1 = tmp.find(L"arrow"))>=0 && (pos2 =tmp.find(L"line"))>=0)	
		return L"arrow";
    else if ((pos1 =tmp.find(L"square"))>=0 || (pos2 =tmp.find(L"diamond"))>=0) 
		return L"diamond";
    else if ((pos1 =tmp.find(L"circle"))>=0)
		return L"oval";
    else
		return L"triangle";

		//Arrow_20_concave, 
		//Circle, 
		//Dimension_20_Lines, 
		//Double_20_Arrow, 
		//Line_20_Arrow, 
		//Rounded_20_large_20_Arrow, 
		//Rounded_20_short_20_Arrow, 
		//Small_20_Arrow, 
		//Square_20_45, and 
		//Symmetric_20_Arrow
}

//stealth (Stealth Arrow) Stealth arrow head ----??????


} }