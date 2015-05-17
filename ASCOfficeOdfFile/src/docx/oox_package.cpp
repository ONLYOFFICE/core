#include "../odf/precompiled_cpodf.h"
#include "oox_package.h"

#include <boost/foreach.hpp>
#include <cpdoccore/utf8cpp/utf8.h>

#include "mediaitems.h"

#include "../../../ASCImageStudio3/ASCGraphics/OfficeSvmFile/SvmConverter.h"

#include "../../DesktopEditor/common/File.h"

namespace cpdoccore { 
namespace oox {
namespace package {

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
	std::wstring name_ = RootPath + FILE_SEPARATOR_STR +  file_name_;

	NSFile::CFileBinary file;
	if ( file.CreateFileW(name_) == true)
	{
		std::string root = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
		file.WriteFile((BYTE*)root.c_str(), root.length());
		file.WriteFile((BYTE*)content_utf8_.c_str(), content_utf8_.length());
		file.CloseFile();
	}
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
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"_rels";

	FileSystem::Directory::CreateDirectory(path.c_str());
   
	if (rels_file_)
        rels_file_->write(path);
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

	resStream << L"<dc:creator>ONLYOFFICE Online Editor</dc:creator>";
	resStream << L"<cp:lastModifiedBy>ONLYOFFICE Online Editor</cp:lastModifiedBy>";
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
   
	resStream << L"<Application>ONLYOFFICE Online Editor</Application>"; 
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
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"docProps";
	FileSystem::Directory::CreateDirectory(path.c_str());

    core_.write(path);
    app_.write(path);
}

////////////


media::media(mediaitems & _Mediaitems) : mediaitems_(_Mediaitems)
{    
}

void media::write(const std::wstring & RootPath)
{
    std::wstring path = RootPath + FILE_SEPARATOR_STR + L"media";
	FileSystem::Directory::CreateDirectory(path.c_str());

    BOOST_FOREACH( mediaitems::item & item, mediaitems_.items() )
    {
        if (item.mediaInternal && item.valid && item.type == mediaitems::typeImage )
        {
			std::wstring & file_name  = item.href;
			std::wstring file_name_out = RootPath + FILE_SEPARATOR_STR + item.outputName;
			
			if (file_name.rfind(L".svm") >= 0)
			{
				ConvertSvmToImage(file_name, file_name_out);
			}
			//else if(file_name.extension().empty())
			//{
			//	//непонятный тип .. может быть svm, emf, wmf, vclmtf (это копия уравнений)
			//	//отдетектить???
			//	ImageFile::IImageFile3Ptr piImageFile = NULL;
			//	piImageFile.CreateInstance( __uuidof(ImageFile::ImageFile3) );
			//	if( NULL != piImageFile )
			//	{
			//		VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			//		IUnknown* pImage = NULL;
			//	
			//		BSTR bstrFilename = SysAllocString(item.href.data());
			//		try
			//		{
			//			piImageFile->LoadImage2(bstrFilename, &pImage, &vbSuccess);
			//		}
			//		catch(...)
			//		{
			//		}
			//		SysFreeString( bstrFilename );
			//		if (vbSuccess && pImage)
			//		{
			//			bstrFilename = SysAllocString(file_name_out.string().data());
			//			piImageFile->SaveImage2( &pImage, 4, bstrFilename, &vbSuccess );//to png
			//			SysFreeString( bstrFilename );
			//			pImage->Release();
			//		}
			//		if (vbSuccess == FALSE)
			//		{
			//			FileSystem::Directory::CopyFile(item.href, file_name_out);//ну не png это будет ... и чё? :-)
			//		}
			//	}

			//}
			else
				NSFile::CFileBinary::Copy(item.href, file_name_out);
        }
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
