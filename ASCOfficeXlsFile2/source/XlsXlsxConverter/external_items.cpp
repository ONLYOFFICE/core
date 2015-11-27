
#include "external_items.h"

#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>

#include <utils.h>

#include "oox_rels.h"
#include "mediaitems_utils.h"

#include "../../../Common/DocxFormat/Source/Base/Base.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"

namespace oox {


external_items::item::item(	std::wstring const & _uri, Type _type, bool _mediaInternal, int _id, std::wstring const & _rId  )
                       : uri(_uri), type(_type), mediaInternal(_mediaInternal), id(_id), rId(_rId)
{    
}

//std::wstring external_items::add_or_find(const std::wstring & href, Type type, bool & isInternal)
//{
//    std::wstring ref;
//    return add_or_find(href, type, isInternal, ref);
//}

std::wstring static get_default_file_name(external_items::Type type)
{
    switch (type)
    {
    case external_items::typeImage:
        return L"image";
    case external_items::typeChart:
        return L"chart";
    case external_items::typeMedia:
        return L"media";
	default:
        return L"";
    }
}
//std::wstring external_items::create_file_name(const std::wstring & uri, external_items::Type type, size_t Num)
//{
//	if (uri.empty()) return L"";
//
//	std::wstring sExt;
//	int n = uri.rfind(L".");
//	if (n>0) sExt = uri.substr(n);
//	else if (n==0)
//	{
//
//	}
//	//todooo проверить
//   
//	return get_default_file_name(type) + boost::lexical_cast<std::wstring>(Num) + sExt;
//}



std::wstring external_items::add_image(const std::wstring & image, int bin_id)
{
    const bool isMediaInternal = true;
  
	std::wstring rId = std::wstring(L"picId") + boost::lexical_cast<std::wstring>(count_image+1);
	count_image++;  
		
	items_.push_back( item(image, typeImage, isMediaInternal, bin_id, rId) );

    return rId;
}
std::wstring external_items::add_chart(std::wstring & oox_target)
{
    const bool isMediaInternal = true;
  
	count_charts++;  
	
	std::wstring rId = std::wstring(L"chartId") + boost::lexical_cast<std::wstring>(count_charts);
	oox_target = std::wstring(L"charts/chart") + boost::lexical_cast<std::wstring>(count_charts) + L".xml";
		
	items_.push_back( item(oox_target, typeChart, isMediaInternal, -1, rId) );

    return rId;
}
std::wstring external_items::find_image(int _id, std::wstring & oox_target, bool & isInternal)
{
    for (int i=0 ; i <items_.size(); i ++)
	{
		if (items_[i].type == typeImage && items_[i].id == _id)
		{
			oox_target	= items_[i].uri;
			isInternal	= items_[i].mediaInternal;
			return items_[i].rId;
		}
	}
	return L"";
}
std::wstring external_items::find_image(const std::wstring & oox_target, bool & isInternal)
{
    for (int i=0 ; i <items_.size(); i ++)
	{
		if (items_[i].type == typeImage && items_[i].uri == oox_target)
		{
			isInternal = items_[i].mediaInternal;
			return items_[i].rId;
		}
	}
	return L"";
}
void external_items::dump_rels(rels & Rels)
{
    size_t i = 0;
    BOOST_FOREACH(item & elm, items_)
    {
        Rels.add( relationship(
					elm.rId, 
					utils::media::get_rel_type(elm.type), 
					elm.uri, 
					elm.mediaInternal ? L"" : L"External" )
                );
    }        
}


}
