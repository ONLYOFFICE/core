/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "oox_package.h"

#include <boost/foreach.hpp>
#include <boost/ref.hpp>
#include <boost/make_shared.hpp>

#include "../../../OdfFile/Common/utf8cpp/utf8.h"
#include "../../../OOXML/Base/Base.h"

#include "external_items.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/SystemUtils.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"

namespace oox {
namespace package {

static std::wstring get_mime_type(const std::wstring & extension)
{
		 if (L"eps" == extension)	return  L"image/x-eps";
	else if (L"wmf" == extension)	return  L"image/x-wmf";
	else if (L"emf" == extension)	return  L"image/x-emf";
  	else if (L"gif" == extension)	return  L"image/x-gif";
   	else if (L"png" == extension)	return  L"image/x-png";
 	else if (L"jpg" == extension)	return  L"image/x-jpeg";
  	else if (L"jpeg" == extension)	return  L"image/x-jpeg";
  	else if (L"tiff" == extension)	return  L"image/x-tiff";
	else if (L"bmp" == extension)	return  L"image/bmp";
	else if (L"pdf" == extension)	return  L"application/pdf";
	else if (L"wav" == extension)	return  L"audio/wav";
	else if (L"bin" == extension)	return  L"application/vnd.openxmlformats-officedocument.oleObject";
	else if (L"xlsx" == extension)	return  L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	else if (L"xls" == extension)	return  L"application/vnd.ms-excel";
	else if (L"doc" == extension)	return  L"application/msword";
	else if (L"vsd" == extension)	return  L"application/vnd.visio";
	else if (L"vsdx" == extension)	return  L"application/vnd.ms-visio.drawing";
	else if (L"pict" == extension)	return  L"image/pict";

	return L"";
}

//--------------------------------------------------------------------------------------------------------------------------
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

bool content_types_file::add_or_find_default(const std::wstring & extension)
{
	for (size_t i = 0 ; i < content_type_.get_default().size(); i++)
	{
		if (content_type_.get_default()[i].extension() == extension)
			return true;
	}
	content_type_.add_default(extension, get_mime_type(extension));
	return true;
}
void content_types_file::set_media(external_items & _Mediaitems)
{
    BOOST_FOREACH( external_items::item & item, _Mediaitems.items() )
    {
		if (!item.mediaInternal) continue;

		std::wstring extension;
		int n = item.uri.rfind(L".");
		if (n > 0) extension = item.uri.substr(n + 1);

		if (item.type == external_items::typeImage || item.type == external_items::typeMedia)
		{
			add_or_find_default(extension);
		}
		else if (item.type == external_items::typeOleObject)
		{
			std::wstring link = L"application/vnd.openxmlformats-officedocument.oleObject";
			
			if (extension == L"xls" || extension == L"doc") 
				add_or_find_default(extension);
			else
				content_type_.add_override(L"/xl/embeddings/" + item.uri, link);
		}
		else if (item.type == external_items::typeActiveX)
		{
			std::wstring link = L"application/vnd.ms-office.activeX";
			content_type_.add_override(L"/xl/activeX/" + item.uri, link);
		}
	}
}


//--------------------------------------------------------------------------------------------------------------------------
simple_element_ptr simple_element::create(const std::wstring & FileName, const std::wstring & Content)
{
    return boost::make_shared<simple_element>(FileName, Content);
}
simple_element::simple_element(const std::wstring & fileName, const std::wstring & content) : filename_(fileName), content_(content)
{
}
simple_element_ptr simple_element::create(const std::wstring & FileName, const std::string & contentUtf8)
{
    return boost::make_shared<simple_element>(FileName, contentUtf8);
}
simple_element::simple_element(const std::wstring & fileName, const std::string & contentUtf8) : filename_(fileName), content_utf8_(contentUtf8)
{
}
void simple_element::write(const std::wstring & RootPath)
{
	std::wstring name_ = RootPath + FILE_SEPARATOR_STR +  filename_;

	NSFile::CFileBinary file;
	if ( file.CreateFileW(name_) == true)
	{
		if (!content_utf8_.empty())
		{
			file.WriteFile((BYTE*)content_utf8_.c_str(), content_utf8_.size());
		}
		else
		{
			std::string root = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";			
			file.WriteFile((BYTE*)root.c_str(), root.length());

			file.WriteStringUTF8(content_);
		}
        file.CloseFile();
	}
}
//--------------------------------------------------------------------------------------------------------------------------
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

    NSDirectory::CreateDirectory(path.c_str());
   
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
//--------------------------------------------------------------------------------------------
customXml_content::customXml_content()
{      
}
_CP_PTR(customXml_content) customXml_content::create()
{
    return boost::make_shared<customXml_content>();
}
//--------------------------------------------------------------------------------------------
theme_content::theme_content(char* data, size_t size)
{      
	content_ = std::string(data, size);
}
_CP_PTR(theme_content) theme_content::create(char* data, size_t size)
{
    return boost::make_shared<theme_content>(data, size);
}
//----------------------------------------------------------------------------------------
chart_content::chart_content() : rels_file_(rels_file::create(L""))
{      
}
_CP_PTR(chart_content) chart_content::create()
{
    return boost::make_shared<chart_content>();
}
//----------------------------------------------------------------------------------------
docProps_files::docProps_files()
{

}
std::wstring docProps_files::create_core()
{
	std::wstringstream resStream;

	resStream << L"<cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" "
		L"xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" "
		L"xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" >";

	//resStream << L"<dc:creator>ONLYOFFICE</dc:creator>";
	//resStream << L"<cp:lastModifiedBy>ONLYOFFICE</cp:lastModifiedBy>";
	resStream << L"<cp:revision>1</cp:revision>";
	resStream << L"</cp:coreProperties>";

	return resStream.str();
}

std::wstring docProps_files::create_app()
{
	std::wstringstream resStream;

	resStream << L"<Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" "
		L"xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\" >";

	resStream << L"<Application>";
	std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
	if (sApplication.empty())
		sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
	resStream << sApplication;
#if defined(INTVER)
	std::string s = VALUE2STR(INTVER);
	resStream << L"/" << std::wstring(s.begin(), s.end());
#endif	
	resStream << L"</Application></Properties>";

	return resStream.str();
}
void docProps_files::set_app_content(const std::wstring & content)
{
	app_content_ = content;
}
void docProps_files::set_core_content(const std::wstring & content)
{
	core_content_ = content;
}
void docProps_files::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"docProps";
	NSDirectory::CreateDirectory(path.c_str());

	simple_element_ptr core = simple_element::create(L"core.xml", core_content_.empty() ? create_core() : core_content_);
	simple_element_ptr app = simple_element::create(L"app.xml", app_content_.empty() ? create_app() : app_content_);

	core->write(path);
	app->write(path);
}
//----------------------------------------------------------------------------------------------------------------
media::media(external_items & _items) : items_(_items)
{    
}

void media::write(const std::wstring & RootPath)
{

}


}
}
