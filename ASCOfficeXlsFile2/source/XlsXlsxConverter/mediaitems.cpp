
#include "mediaitems.h"

#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>

#include "utils.h"

#include "oox_rels.h"
#include "mediaitems_utils.h"

#include "../../../Common/DocxFormat/Source/Base/Base.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"

namespace oox {


mediaitems::item::item(	std::wstring const & _href,
                       Type _type,
                       std::wstring const & _outputName,
						bool _mediaInternal,
						std::wstring const & _Id
					   )
                       : href(_href),
                       type(_type),
                       outputName(_outputName),
                       mediaInternal(_mediaInternal),
					   Id(_Id),
                       valid(true) //вообще говоря даже если файл покоцанный то мы все равно обязаны перенести "объект"
{    
	count_add = 1;
	count_used = 0;
}

std::wstring mediaitems::add_or_find(const std::wstring & href, Type type, bool & isInternal)
{
    std::wstring ref;
    return add_or_find(href, type, isInternal, ref);
}

std::wstring static get_default_file_name(mediaitems::Type type)
{
    switch (type)
    {
    case mediaitems::typeImage:
        return L"image";
    case mediaitems::typeChart:
        return L"chart";
    case mediaitems::typeMedia:
        return L"media";
	default:
        return L"";
    }
}
std::wstring mediaitems::create_file_name(const std::wstring & uri, mediaitems::Type type, size_t Num)
{
	if (uri.empty()) return L"";

	std::wstring sExt;
	int n = uri.rfind(L".");
	if (n>0) sExt = uri.substr(n);
	else if (n==0)
	{

	}
	//todooo проверить
   
	return get_default_file_name(type) + boost::lexical_cast<std::wstring>(Num) + sExt;
}



std::wstring mediaitems::add_or_find(const std::wstring & href, Type type, bool & isInternal, std::wstring & ref)
{
    const bool isMediaInternal = utils::media::is_internal(href, odf_packet_);
  
	std::wstring sub_path = L"media/";
	
	std::wstring inputFileName;
	if ( type == typeChart)
	{
		sub_path = L"charts/";
	}
	int number=0;
	
	if ( type == typeChart)
		number= count_charts+1;
	else if ( type == typeImage)
		number= count_image+1;
	else if ( type == typeShape)
		number= count_shape+1;
	else if ( type == typeMedia)
		number= count_media+1;
	else
		number= items_.size()+1;
	
	inputFileName = create_file_name(href, type, number);
	
    std::wstring inputPath = isMediaInternal ? odf_packet_ + FILE_SEPARATOR_STR + href : href;
	std::wstring outputPath = isMediaInternal ? ( sub_path + inputFileName) : href;
	if ( type == typeChart)outputPath= outputPath + L".xml";

	std::wstring id;
    BOOST_FOREACH(item & elm, items_)
    {
		if (elm.href == inputPath)
		{
			id = elm.Id;
			outputPath  = elm.outputName;
			elm.count_add++;
			break;
		}
	}
	if (id.length() < 1)
	{
		if ( type == typeChart)
		{
			id = std::wstring(L"chId") + boost::lexical_cast<std::wstring>(count_charts+1);
			count_charts++;
		}
		else if ( type == typeImage)
		{
            int n_svm = outputPath.rfind (L".svm");
			if ( n_svm >= 0 )
			{
				outputPath = outputPath.substr(0, n_svm) + L".png"; 
			}

			id = std::wstring(L"picId") + boost::lexical_cast<std::wstring>(count_image+1);
			count_image++;
		}
		else
		{
			id = std::wstring(L"rId") + boost::lexical_cast<std::wstring>(count_shape+1);
			count_shape++;
		}
		
		items_.push_back( item(inputPath, type, xml::utils::replace_text_to_xml(outputPath), isMediaInternal, id) );
	}

  	ref = outputPath;
    isInternal = isMediaInternal;
    return id;
}

void mediaitems::dump_rels(rels & Rels)
{
    size_t i = 0;
    BOOST_FOREACH(item & elm, items_)
    {
		if (elm.count_used > elm.count_add)continue; // уже использовали этот релс выше(колонтитул ....)
        Rels.add( relationship(
                elm.Id, 
                utils::media::get_rel_type(elm.type), 
                elm.valid ? elm.outputName : L"NULL", 
                elm.mediaInternal ? L"" : L"External" )
                );
		elm.count_used++;
    }        
}


}
