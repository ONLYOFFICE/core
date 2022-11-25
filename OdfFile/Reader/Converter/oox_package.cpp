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

#include <boost/ref.hpp>

#include "../../Common/utf8cpp/utf8.h"

#include "mediaitems.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/SystemUtils.h"
#include "../../../DesktopEditor/graphics/pro/Image.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../../OOXML/Base/Base.h"

namespace cpdoccore { 
namespace oox {
namespace package {

static void ConvertSvmToImage(std::wstring &file_svm, std::wstring &file_png, NSFonts::IApplicationFonts *pAppFonts)
{
    MetaFile::IMetaFile* pMetaFile = MetaFile::Create(pAppFonts);

    if (pMetaFile->LoadFromFile(file_svm.c_str()))
	{
		double w, h, x, y;
        pMetaFile->GetBounds(&x, &y, &w, &h);
        pMetaFile->ConvertToRaster(file_png.c_str(), 4, w);
        pMetaFile->Close();
	}

    RELEASEOBJECT(pMetaFile);
}

static std::wstring get_mime_type(const std::wstring & extension)
{
		 if (L"eps" == extension)	return  L"image/x-eps";
	else if (L"wmf" == extension)	return  L"image/x-wmf";
	else if (L"emf" == extension)	return  L"image/x-emf";
  	else if (L"gif" == extension)	return  L"image/x-gif";
   	else if (L"png" == extension)	return  L"image/x-png";
 	else if (L"jpg" == extension)	return  L"image/x-jpeg";
  	else if (L"jpeg" == extension)	return  L"image/x-jpeg";
   	else if (L"tif" == extension)	return  L"image/x-tiff";
 	else if (L"tiff" == extension)	return  L"image/x-tiff";
	else if (L"pdf" == extension)	return  L"application/pdf";
	else if (L"bmp" == extension)	return  L"image/bmp";
	else if (L"wdp" == extension)	return  L"image/vnd.ms-photo";

	else if (L"wav" == extension)	return  L"audio/wav";
	else if (L"mp3" == extension)	return  L"audio/mpeg";
	else if (L"wma" == extension)	return  L"audio/x-ms-wma";
	else if (L"m4a" == extension)	return  L"audio/m4a";

	else if (L"avi" == extension)	return  L"video/x-msvideo";
	else if (L"wmv" == extension)	return  L"video/x-ms-wmv";
	else if (L"mov" == extension)	return  L"video/mov";
	else if (L"mp4" == extension)	return  L"video/mp4";
	else if (L"m4v" == extension)	return  L"video/m4v";
	else if (L"mkv" == extension)	return  L"video/mkv";
	else if (L"webm" == extension)	return  L"video/webm";

	else if (L"bin" == extension)	return  L"application/vnd.openxmlformats-officedocument.oleObject";
	else if (L"xlsx" == extension)	return  L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	else if (L"docx" == extension)	return  L"application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	else if (L"doc" == extension)	return  L"application/msword";
	else if (L"vsd" == extension)	return  L"application/vnd.visio";
	else if (L"vsdx" == extension)	return  L"application/vnd.ms-visio.drawing";
	else							return	L"application/octet-stream";


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

    simple_element elm(filename_, res);
    elm.write(RootPath);
}

content_type * content_types_file::content()
{ 
	return &content_type_content_;
}

bool content_types_file::add_or_find_default(const std::wstring & extension)
{
	std::vector<default_content_type> & defaults = content_type_content_.get_default();
	
	for (size_t i = 0 ; i < defaults.size(); i++)
	{
		if (defaults[i].extension() == extension)
			return true;
	}
	
	content_type_content_.add_default(extension, get_mime_type(extension));
	return true;
}

bool content_types_file::add_or_find_override(const std::wstring & fileName)
{
	std::vector<override_content_type> & override_ = content_type_content_.get_override();
	
	for (size_t i = 0 ; i < override_.size(); i++)
	{
		if (override_[i].part_name() == fileName)
			return true;
	}
	
	std::wstring content_type;
	
	int pos = fileName.rfind(L".");	
	std::wstring extension = pos >= 0 ? fileName.substr(pos + 1) : L"";
	
	if (extension == L"xlsx")
	{
		content_type = L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
		content_type_content_.add_override(fileName, content_type);
	}
	if (extension == L"bin")
	{
		//content_type = L"application/vnd.openxmlformats-officedocument.oleObject";

		add_or_find_default(extension);
	}

	return true;
}
void content_types_file::set_media(mediaitems_ptr & _Mediaitems)
{
	std::vector<mediaitems::item> & items_ = _Mediaitems->items();
    for (size_t i = 0; i < items_.size(); i++)
    {
		if ((items_[i].type == typeImage || 
			items_[i].type == typeMedia	||
			items_[i].type == typeVideo	||
			items_[i].type == typeAudio) && items_[i].mediaInternal)
		{
			int n = items_[i].outputName.rfind(L".");
			if (n > 0)
			{
				add_or_find_default(items_[i].outputName.substr(n + 1, items_[i].outputName.length() - n));
			}
		}
	}
}



void content_types_file::set_media(mediaitems & _Mediaitems)
{
	std::vector<mediaitems::item> & items_ = _Mediaitems.items();
    for (size_t i = 0; i < items_.size(); i++)
    {
		if ((items_[i].type == typeImage || 
			items_[i].type == typeMedia	||
			items_[i].type == typeVideo	||
			items_[i].type == typeAudio) && items_[i].mediaInternal)
		{
			int n = items_[i].outputName.rfind(L".");
			if (n > 0)
			{
				add_or_find_default(items_[i].outputName.substr(n + 1, items_[i].outputName.length() - n));
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////

simple_element::simple_element(const std::wstring & FileName, const std::wstring & Content) : file_name_(FileName), bXml(true)
{
    utf8::utf16to8(Content.begin(), Content.end(), std::back_inserter(content_utf8_));
}
simple_element::simple_element(const std::wstring & FileName, const std::string & Content) : file_name_(FileName), content_utf8_(Content), bXml(false)
{
}
void simple_element::write(const std::wstring & RootPath)
{
	std::wstring name_ = RootPath + FILE_SEPARATOR_STR + file_name_;

	NSFile::CFileBinary file;
	if ( file.CreateFileW(name_) == true)
	{
		if (bXml)
		{
			std::string root = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
			file.WriteFile((BYTE*)root.c_str(), root.length());
		}
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
//-----------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------
_CP_PTR(customXml_content) customXml_content::create(const std::wstring &item, const std::wstring &props)
{
    return boost::make_shared<customXml_content>(item, props);
}
//-----------------------------------------------------------------------------------------------
simple_element_ptr simple_element::create(const std::wstring & FileName, const std::wstring & Content)
{
    return boost::make_shared<simple_element>(FileName, Content);
}
simple_element_ptr simple_element::create(const std::wstring & FileName, const std::string & Content)
{
    return boost::make_shared<simple_element>(FileName, Content);
}
//-----------------------------------------------------------------------------------------------
docProps_files::docProps_files()
{

}
std::wstring docProps_files::create_core()
{
    std::wstringstream resStream;

    resStream << L"<cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" "
    L"xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" "
    L"xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" >";

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
	resStream << L"/" << std::wstring(s.begin(), s.end()) ;
#endif	
	resStream << L"</Application></Properties>";
    
    return resStream.str();
}
void docProps_files::set_app(element_ptr Element)
{
	app_ = Element;
}
void docProps_files::set_core(element_ptr Element)
{
	core_ = Element;
}
void docProps_files::write(const std::wstring & RootPath)
{
	std::wstring path = RootPath + FILE_SEPARATOR_STR + L"docProps";
	NSDirectory::CreateDirectory(path.c_str());

	if (!core_)	core_ = simple_element::create(L"core.xml", create_core());
	if (!app_)	app_ = simple_element::create(L"app.xml", create_app());
	
	core_->write(path);
	app_->write(path);
}
//---------------------------------------------------------------------------------------------------
media::media(mediaitems_ptr & _mediaitems, NSFonts::IApplicationFonts *pAppFonts) : mediaItems_(_mediaitems), appFonts_(pAppFonts)
{    
}
void media::write(const std::wstring & RootPath)
{
    std::wstring path = RootPath + FILE_SEPARATOR_STR + L"media";
    NSDirectory::CreateDirectory(path.c_str());

	mediaitems::items_array & items = mediaItems_->items();

    for (size_t i = 0; i < items.size(); i++ )
    {
        if (items[i].mediaInternal && items[i].valid && (	items[i].type == typeImage || 
															items[i].type == typeMedia ||
															items[i].type == typeAudio ||
															items[i].type == typeVideo ))
        {
			std::wstring &file_name	= items[i].href;
			std::wstring file_name_out	= RootPath + FILE_SEPARATOR_STR + items[i].outputName;
			
			if (file_name.empty()) continue;

			CImageFileFormatChecker svmFileChecker;
			if (svmFileChecker.isSvmFile(file_name))
			{
				ConvertSvmToImage(file_name, file_name_out, appFonts_);
			}
			else
				NSFile::CFileBinary::Copy(file_name, file_name_out);
        }
    }

}
//------------------------------------------------------------------------------------------------------------
charts::charts(mediaitems_ptr & _chartsItems) : chartsItems_(_chartsItems)
{    
}

void charts::write(const std::wstring & RootPath)
{

}
//--------------------------------------------------------------------------------------------------------------
embeddings::embeddings(mediaitems_ptr & _EmbeddingsItems) : embeddingsItems_(_EmbeddingsItems)
{    
}
void embeddings::write(const std::wstring & RootPath)
{
    std::wstring path = RootPath + FILE_SEPARATOR_STR + L"embeddings";
    NSDirectory::CreateDirectory(path.c_str());

	content_types_file & content_types = get_main_document()->get_content_types_file();           
    
	mediaitems::items_array & items = embeddingsItems_->items();

    for (size_t i = 0; i < items.size(); i++ )
    {
        if ( items[i].mediaInternal && items[i].valid &&
			(items[i].type == typeMsObject || items[i].type == typeOleObject))
        {
			int pos = items[i].outputName.rfind(L".");	
			std::wstring extension = pos >= 0 ? items[i].outputName.substr(pos + 1) : L"";
			
			content_types.add_or_find_default(extension);

			std::wstring file_name_out = RootPath + FILE_SEPARATOR_STR + items[i].outputName;
			
			NSFile::CFileBinary::Copy(items[i].href, file_name_out);
		}
    }
}

}
}
}
