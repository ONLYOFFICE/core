#include "precompiled_cpodf.h"
#include "mediaitems.h"

#include <regex.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <cpdoccore/xml/utils.h>

#include "docx_rels.h"
#include "mediaitems_utils.h"
#include <cpdoccore/common/boost_filesystem_version.h>

namespace cpdoccore { 
namespace oox {


using boost::filesystem::wpath;

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
					   used_rels (false),
                       valid(true) //вообще говоря даже если файл покоцанный то мы все равно обязаны перенести "объект"
{    

}

std::wstring mediaitems::add_or_find(const std::wstring & href, Type type, bool & isInternal)
{
    std::wstring ref;
    return add_or_find(href, type, isInternal, ref);
}

namespace fs = boost::filesystem;

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
	else
		number= items_.size()+1;
	
	inputFileName = utils::media::create_file_name(href, type, number);
	
    std::wstring inputPath = isMediaInternal ? BOOST_STRING_PATH(wpath(odf_packet_) / href) : href;
	std::wstring outputPath = isMediaInternal ? ( sub_path + inputFileName) : href;
	if ( type == typeChart)outputPath= outputPath + L".xml";

	std::wstring id;
    BOOST_FOREACH(item const & elm, items_)
    {
		if (elm.href == inputPath)
		{
			id = elm.Id;
			outputPath  = elm.outputName;
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
			fs::wpath file_name  = fs::wpath(inputPath);
			if (file_name.extension() == L".svm" || file_name.extension().empty())
			{
				outputPath = outputPath + L".png"; 
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
		if (elm.used_rels)continue; // уже использовали этот релс выше(колонтитул ....)
        Rels.add( relationship(
                elm.Id, 
                utils::media::get_rel_type(elm.type), 
                elm.valid ? elm.outputName : L"NULL", 
                elm.mediaInternal ? L"" : L"External" )
                );
		elm.used_rels = true;

    }        
}


}
}
