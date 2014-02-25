#include "precompiled_cpodf.h"
#include "mediaitems.h"
#include "odf_rels.h"

#include <regex.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <cpdoccore/xml/utils.h>

#include "mediaitems_utils.h"
#include <cpdoccore/common/boost_filesystem_version.h>

namespace cpdoccore { 
namespace odf {


using boost::filesystem::wpath;

mediaitems::item::item(	std::wstring const & _href,
                       Type _type,
                       std::wstring const & _outputName
					   )
                       : href(_href),
                       type(_type),
                       outputName(_outputName)
{    

}


namespace fs = boost::filesystem;

void mediaitems::add_or_find(const std::wstring & href, Type type,  std::wstring & ref)
{
	std::wstring sub_path;//
	std::wstring inputFileName;
	int number=0;
	
	if (type == typeImage)
	{
		sub_path = L"Pictures/";
		number= count_image+1;
	}
	else
	{
		sub_path = L"Media/";
		number= count_media+1;
	}

	
	inputFileName = utils::media::create_file_name(href, type, number);//guid???
	
    std::wstring inputPath =  BOOST_STRING_PATH(wpath(oox_packet_) / href);
	std::wstring outputPath ;

	std::wstring id;
    BOOST_FOREACH(item const & elm, items_)
    {
		if (elm.href == inputPath)
		{
			outputPath  = elm.outputName;
			break;
		}
	}
	if (outputPath .length() < 1)
	{
		outputPath = ( sub_path + inputFileName) ;
		if ( type == typeImage)
		{
			fs::wpath file_name  = fs::wpath(inputPath);
			if (file_name.extension() == L".svm" || file_name.extension().empty())
			{
				outputPath = outputPath + L".png"; 
			}
			count_image++;
		}
		
		items_.push_back( item(inputPath, type, xml::utils::replace_text_to_xml(outputPath)) );
	}

  	ref = outputPath;
}

void mediaitems::dump_rels(rels & Rels)
{
    size_t i = 0;
    BOOST_FOREACH(item & elm, items_)
    {
        Rels.add( relationship(
                utils::media::get_rel_type(elm.type), 
                elm.outputName)
                );

    }        
}


}
}
