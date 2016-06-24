/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include <cpdoccore/utf8cpp/utf8.h>

#include "mediaitems.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/raster/Metafile/MetaFile.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"

namespace cpdoccore { 
namespace oox {
namespace package {

static void ConvertSvmToImage(std::wstring &file_svm, std::wstring &file_png, CApplicationFonts *pAppFonts)
{
	MetaFile::CMetaFile oMetaFile(pAppFonts);

	if (oMetaFile.LoadFromFile(file_svm.c_str()))
	{
		double w, h, x, y;
		oMetaFile.GetBounds(&x, &y, &w, &h);
		oMetaFile.ConvertToRaster(file_png.c_str(), 4, w);
		oMetaFile.Close();
	}
}

static std::wstring get_mime_type(const std::wstring & extension)
{
	if (L"eps" == extension)	return  L"image/x-eps";
	if (L"wmf" == extension)	return  L"image/x-wmf";
	if (L"emf" == extension)	return  L"image/x-emf";
  	if (L"gif" == extension)	return  L"image/x-gif";
   	if (L"png" == extension)	return  L"image/x-png";
 	if (L"jpg" == extension)	return  L"image/x-jpeg";
  	if (L"jpeg" == extension)	return  L"image/x-jpeg";
  	if (L"tiff" == extension)	return  L"image/x-tiff";
	if (L"pdf" == extension)	return  L"application/pdf";
	if (L"wav" == extension)	return  L"audio/wav";
	if (L"bin" == extension)	return  L"application/vnd.openxmlformats-officedocument.oleObject";
	if (L"xlsx" == extension)	return  L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";

	return L"";
}

//----------------------------------------------------------------

void element::set_main_document(document * _document) 
{ 
	document_ = _document; 
}
document *	element::get_main_document()						
{
	return document_; 
}

content_types_file::content_types_file() : filename_(L"[Content_Types].xml") 
{}

void content_types_file::write(const std::wstring & RootPath)
{
    std::wstringstream resStream;
    
	content_type_content_.xml_to_stream(resStream);
    std::wstring res = resStream.str(); 

    simple_element elm(filename_, resStream.str());
    elm.write(RootPath);
}

content_type_content * content_types_file::content()
{ 
	return &content_type_content_;
}

bool content_types_file::add_or_find_default(const std::wstring & extension)
{
	for (int i = 0 ; i < content_type_content_.get_default().size(); i++)
	{
		if (content_type_content_.get_default()[i].extension() == extension)
			return true;
	}
	content_type_content_.add_default(extension, get_mime_type(extension));
	return true;
}
void content_types_file::set_media(mediaitems & _Mediaitems)
{
    BOOST_FOREACH( mediaitems::item & item, _Mediaitems.items() )
    {
		if ((item.type == mediaitems::typeImage || item.type == mediaitems::typeMedia) && item.mediaInternal)
		{
			int n = item.outputName.rfind(L".");
			if (n > 0)
			{
				add_or_find_default(item.outputName.substr(n+1, item.outputName.length() - n));
			}
		}
	}
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

	//resStream << L"<dc:creator>ONLYOFFICE Online Editor</dc:creator>";
	//resStream << L"<cp:lastModifiedBy>ONLYOFFICE Online Editor</cp:lastModifiedBy>";
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
   
	resStream << L"<Application>OnlyOffice</Application>"; 
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


media::media(mediaitems & _Mediaitems, CApplicationFonts *pAppFonts) : mediaitems_(_Mediaitems), appFonts_(pAppFonts)
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
			
			CImageFileFormatChecker svmFileChecker;
			if (svmFileChecker.isSvmFile(file_name))
			{
				ConvertSvmToImage(file_name, file_name_out, appFonts_);
			}
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
