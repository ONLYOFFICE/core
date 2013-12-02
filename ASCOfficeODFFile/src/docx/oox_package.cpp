#include "precompiled_cpodf.h"
#include "oox_package.h"

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <utf8cpp/utf8.h>
#include <cpdoccore/common/boost_filesystem_version.h>

#include "mediaitems.h"

#include "..\..\..\..\AVSImageStudio3\AVSGraphics\OfficeSvmFile\SvmConverter.h"

namespace cpdoccore { 
namespace oox {
namespace package {

namespace fs = boost::filesystem;

content_types_file::content_types_file() : filename_(L"[Content_Types].xml") 
{}

void content_types_file::write(const std::wstring & RootPath)
{
    std::wstringstream resStream;
    content_type_.xml_to_stream(resStream);
    std::wstring res = resStream.str(); 

    simple_element elm(filename_, resStream.str());
    elm.write(RootPath);
}

/////////////////////////////////////////////////////////////////////////

simple_element::simple_element(const std::wstring & FileName, const std::wstring & Content) : file_name_(FileName)
{
    utf8::utf16to8(Content.begin(), Content.end(), std::back_inserter(content_utf8_));
}

void simple_element::write(const std::wstring & RootPath)
{
    fs::ofstream file( fs::wpath(RootPath) / file_name_, std::ios_base::out | std::ios_base::binary );
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    file << content_utf8_;
}

rels_file::rels_file(std::wstring const & FileName) : filename_(FileName) 
{};

rels_file_ptr rels_file::create(std::wstring const & FileName)
{
    return boost::make_shared<rels_file>( boost::ref(FileName) );
}

void rels_file::write(const std::wstring & RootPath)
{    
    std::wstringstream resStream;
    rels_.xml_to_stream(resStream);

    simple_element elm(filename_, resStream.str());
    elm.write(RootPath);
}

void rels_files::write(const std::wstring & RootPath)
{
    fs::wpath path = fs::wpath(RootPath) / L"_rels";
    fs::create_directory(path);
    if (rels_file_)
        rels_file_->write(BOOST_STRING_PATH(path));
}

void rels_files::add(relationship const & r)
{
    if (rels_file * rf = get_rel_file())
    {
        rf->get_rels().relationships().push_back(r);        
    }
}

void rels_files::add(std::wstring const & Id,
                     std::wstring const & Type,
                     std::wstring const & Target,
                     std::wstring const & TargetMode)
{
    return add(relationship(Id, Type, Target, TargetMode));
}
///////////////////////////////////////////

chart_content::chart_content() : rels_(rels_file::create(L""))
{
        
}

_CP_PTR(chart_content) chart_content::create()
{
    return boost::make_shared<chart_content>();
}

void chart_content::add_rel(relationship const & r)
{
    rels_->get_rels().add(r);
}

///////////////////////////////////////////
element_ptr simple_element::create(const std::wstring & FileName, const std::wstring & Content)
{
    return boost::make_shared<simple_element>(FileName, Content);
}

////////////

void core_file::write(const std::wstring & RootPath)
{
    std::wstringstream resStream;

    resStream << L"<cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" "
    L"xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" "
    L"xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" >";

	resStream << L"<dc:creator>Teamlab Office</dc:creator>";
	resStream << L"<cp:lastModifiedBy>Teamlab Office</cp:lastModifiedBy>";
	resStream << L"<cp:revision>1</cp:revision>";
    resStream << L"</cp:coreProperties>";

    simple_element elm(L"core.xml", resStream.str());
    elm.write(RootPath);
}

void app_file::write(const std::wstring & RootPath)
{
    std::wstringstream resStream;

    resStream << L"<Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" "
        L"xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\" >";
   
	resStream << L"<Application>Teamlab Office</Application>"; 
    resStream << L"</Properties>";
    
    simple_element elm(L"app.xml", resStream.str());
    elm.write(RootPath);
}

////////////

docProps_files::docProps_files()
{

}

void docProps_files::write(const std::wstring & RootPath)
{
    fs::wpath path = fs::wpath(RootPath) / L"docProps";
    fs::create_directory(path);

    core_.write(BOOST_STRING_PATH(path));
    app_.write(BOOST_STRING_PATH(path));
}

////////////


media::media(mediaitems & _Mediaitems) : mediaitems_(_Mediaitems)
{    
}

void media::write(const std::wstring & RootPath)
{
    fs::wpath path = fs::wpath(RootPath) / L"media";
    fs::create_directory(path);

    BOOST_FOREACH( mediaitems::item & item, mediaitems_.items() )
    {
        if (item.mediaInternal && item.valid && item.type == mediaitems::typeImage )
        {
			fs::wpath file_name  = fs::wpath(item.href);
			if (file_name.extension() == L".svm")
			{
				ConvertSvmToImage(file_name, fs::wpath(RootPath) / item.outputName);
			}
			else
				boost::filesystem::copy_file(item.href, fs::wpath(RootPath) / item.outputName);
        }
		//а если не внешна€€ картинка???
    }

}
///////////////////////////////////////////////////////////////////////////////////


charts::charts(mediaitems & _ChartsItems) : chartsitems_(_ChartsItems)
{    
}

void charts::write(const std::wstring & RootPath)
{

}
}
}
}
