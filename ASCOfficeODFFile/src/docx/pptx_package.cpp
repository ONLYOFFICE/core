#include "precompiled_cpodf.h"
#include "pptx_package.h"
#include "pptx_output_xml.h"

#include <utf8cpp/utf8.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <cpdoccore/common/boost_filesystem_version.h>

namespace cpdoccore { 
namespace oox {
namespace package {

namespace fs = boost::filesystem;


pptx_content_types_file::pptx_content_types_file()
{
    content_type_.add_default(L"rels",  L"application/vnd.openxmlformats-package.relationships+xml");
    content_type_.add_default(L"xml",   L"application/xml");

    content_type_.add_default(L"jpg",     L"image/jpeg");
    content_type_.add_default(L"gif",     L"image/gif");
    content_type_.add_default(L"jpeg",    L"image/jpeg");
    content_type_.add_default(L"png",     L"image/png");
    content_type_.add_default(L"wmf",     L"image/x-wmf");
 
	content_type_.add_override(L"/_rels/.rels",			L"application/vnd.openxmlformats-package.relationships+xml");
    
	content_type_.add_override(L"/ppt/presentation.xml",L"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml");
  //  content_type_.add_override(L"/ppt/tableStyles.xml",	L"application/vnd.openxmlformats-officedocument.presentationml.tableStyles+xml");необяательно
  
	content_type_.add_override(L"/docProps/app.xml",	L"application/vnd.openxmlformats-officedocument.extended-properties+xml");
    content_type_.add_override(L"/docProps/core.xml",	L"application/vnd.openxmlformats-package.core-properties+xml");
}

pptx_document::pptx_document()
{
    ppt_files_.set_main_document(this);
   
	rels_file_ptr relFile = rels_file::create(L".rels");
  
	relFile->get_rels().add(relationship(L"rId1", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument",		L"ppt/presentation.xml"));
    relFile->get_rels().add(relationship(L"rId2", L"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties",		L"docProps/core.xml"));
    relFile->get_rels().add(relationship(L"rId3", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties",	L"docProps/app.xml"));
   
	rels_files_.add_rel_file( relFile );
}

void pptx_document::write(const std::wstring & RootPath)
{
    ppt_files_.write(RootPath);
    docProps_files_.write(RootPath);
    content_type_.write(RootPath);
    rels_files_.write(RootPath);
}

////////////////////////////////////////////

slide_content::slide_content() : rels_(rels_file::create(L""))
{
        
}

_CP_PTR(slide_content) slide_content::create()
{
    return boost::make_shared<slide_content>();
}

void slide_content::add_rel(relationship const & r)
{
    rels_->get_rels().add(r);
}

void slide_content::add_rels(rels & r)
{
    BOOST_FOREACH(relationship & item, r.relationships())
	{
		rels_->get_rels().add(item);
	}
}
////////////

slides_files::slides_files()
{}

void slides_files::add_slide(slide_content_ptr slide)
{
    slides_.push_back(slide);
}

void slides_files::write(const std::wstring & RootPath)
{
    fs::wpath path = fs::wpath(RootPath) / L"slides" ;
    fs::create_directory(path);

	content_type & contentTypes = this->get_main_document()->content_type().get_content_type();
	static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.presentationml.slide+xml";
    
	size_t count = 0;

    BOOST_FOREACH(const slide_content_ptr & item, slides_)
    {
        if (item)
        {
            count++;
            const std::wstring fileName = std::wstring(L"slide") + boost::lexical_cast<std::wstring>(count) + L".xml";
           
			contentTypes.add_override(std::wstring(L"/ppt/slides/") + fileName, kWSConType);

            if (rels_)
            {
                const std::wstring id = std::wstring(L"sId") + boost::lexical_cast<std::wstring>(count);
                static const std::wstring kWSRel = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide";
                const std::wstring fileRef = std::wstring(L"slides/") + fileName;
                rels_->add(id, kWSRel, fileRef);
            }

            item->get_rel_file()->set_file_name(fileName + L".rels");//внитренние релсы
            
			rels_files relFiles;
            relFiles.add_rel_file(item->get_rel_file());
            relFiles.write(BOOST_STRING_PATH(path));           

            package::simple_element(fileName, item->str()).write(BOOST_STRING_PATH(path));
        }
    }
}

////////////////////////////////////////////
slideMasters_files::slideMasters_files()
{}

void slideMasters_files::add_slide(slide_content_ptr slide)
{
    slides_.push_back(slide);
}

void slideMasters_files::write(const std::wstring & RootPath)
{
    fs::wpath path = fs::wpath(RootPath) / L"slideMasters" ;
    fs::create_directory(path);

	content_type & contentTypes = this->get_main_document()->content_type().get_content_type();
	static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml";
   
	size_t count = 0;

    BOOST_FOREACH(const slide_content_ptr & item, slides_)
    {
        if (item)
        {
            count++;
            const std::wstring fileName = std::wstring(L"slideMaster") + boost::lexical_cast<std::wstring>(count) + L".xml";
            contentTypes.add_override(std::wstring(L"/ppt/slideMasters/") + fileName, kWSConType);

            if (rels_)
            {
                const std::wstring id = std::wstring(L"smId") + boost::lexical_cast<std::wstring>(count);
                static const std::wstring kWSRel = L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster";
                const std::wstring fileRef = std::wstring(L"slideMasters/") + fileName;
                rels_->add(id, kWSRel, fileRef);
            }

            item->get_rel_file()->set_file_name(fileName + L".rels");//внитренние релсы
            
			rels_files relFiles;
            relFiles.add_rel_file(item->get_rel_file());
            relFiles.write(BOOST_STRING_PATH(path));           

            package::simple_element(fileName, item->str()).write(BOOST_STRING_PATH(path));
        }
    }
}

////////////////////////////////////////////
slideLayouts_files::slideLayouts_files()
{}

void slideLayouts_files::add_slide(slide_content_ptr slide)
{
    slides_.push_back(slide);
}

void slideLayouts_files::write(const std::wstring & RootPath)
{
    fs::wpath path = fs::wpath(RootPath) / L"slideLayouts" ;
    fs::create_directory(path);

    size_t count = 0;

    BOOST_FOREACH(const slide_content_ptr & item, slides_)
    {
        if (item)
        {
            count++;
            const std::wstring fileName = std::wstring(L"slideLayout") + boost::lexical_cast<std::wstring>(count) + L".xml";
            content_type & contentTypes = get_main_document()->content_type().get_content_type();
            static const std::wstring kWSConType = L"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml";
            contentTypes.add_override(std::wstring(L"/ppt/slideLayouts/") + fileName, kWSConType);

            item->get_rel_file()->set_file_name(fileName + L".rels");//внитренние релсы
            
			rels_files relFiles;
            relFiles.add_rel_file(item->get_rel_file());
            relFiles.write(BOOST_STRING_PATH(path));           

            package::simple_element(fileName, item->str()).write(BOOST_STRING_PATH(path));
        }
    }
}

theme_elements::theme_elements(pptx_xml_theme_ptr & theme) : theme_(theme)
{
}
void theme_elements::write(const std::wstring & RootPath)
{
    fs::wpath path = fs::wpath(RootPath) / L"theme" ;
    fs::create_directory(path);
	const std::wstring file_name = std::wstring(L"theme") + boost::lexical_cast<std::wstring>(theme_->id()) + L".xml";

	std::wstringstream content_theme;
	theme_->write_to(content_theme);

    simple_element(file_name, content_theme.str()).write(BOOST_STRING_PATH(path));
    
    if (get_main_document())
        get_main_document()->content_type().get_content_type().add_override(std::wstring(L"/ppt/theme/") + file_name, L"application/vnd.openxmlformats-officedocument.theme+xml");


}
////////////////////////////////////////////
ppt_files::ppt_files()
{
    rels_files_.add_rel_file(rels_file::create(L"presentation.xml.rels"));
}

void ppt_files::write(const std::wstring & RootPath)
{
    fs::wpath path = fs::wpath(RootPath) / L"ppt";
    fs::create_directory(path);

    slides_files_.set_rels(&rels_files_);
    slides_files_.set_main_document( this->get_main_document() );
    slides_files_.write(BOOST_STRING_PATH(path));

    slideLayouts_files_.set_main_document( this->get_main_document() );
    slideLayouts_files_.write(BOOST_STRING_PATH(path));

    slideMasters_files_.set_rels(&rels_files_);
    slideMasters_files_.set_main_document( this->get_main_document() );
    slideMasters_files_.write(BOOST_STRING_PATH(path));
	

	//rels_files_.add( relationship( L"hId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument", L"ppt/presentation.xml" ) );

    //if (styles_)
    //{
    //   styles_->write(BOOST_STRING_PATH(path));
    //   rels_files_.add( relationship( L"stId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles", L"tableStyles.xml" ) );
    //}

    if (presentation_)
    {
        presentation_->write(BOOST_STRING_PATH(path));
    }

    if (theme_)
    {
		rels_files_.add( relationship( L"tId1",  L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme", L"theme/theme1.xml" ) );
        theme_->write(BOOST_STRING_PATH(path));
    }

    if (media_)
    {
        media_->set_main_document(get_main_document());
        media_->write(BOOST_STRING_PATH(path));
    }


    rels_files_.write(BOOST_STRING_PATH(path));
}

void ppt_files::set_presentation(pptx_xml_presentation & presentation)
{
	std::wstringstream strm_presentation;
	presentation.write_to(strm_presentation);

	element_ptr  elm = package::simple_element::create(L"presentation.xml", strm_presentation.str());

    elm->set_main_document( this->get_main_document() );
    presentation_ = elm;
}

void ppt_files::set_styles(element_ptr Element)
{
    tableStyles_ = Element;
}
void ppt_files::set_themes(pptx_xml_theme_ptr & theme) // потом до вектора
{
    theme_elements * elm = new theme_elements(theme); 
    elm->set_main_document( this->get_main_document() );
    theme_ = element_ptr( elm );
}
void ppt_files::add_slide(slide_content_ptr slide)
{
    slides_files_.add_slide(slide);
}
void ppt_files::add_slideLayout(slide_content_ptr slide)
{
    slideLayouts_files_.add_slide(slide);
}
void ppt_files::add_slideMaster(slide_content_ptr slide)
{
    slideMasters_files_.add_slide(slide);
}

void ppt_files::set_media(mediaitems & _Mediaitems)
{
    media_ = element_ptr( new media(_Mediaitems) );
}


}
}
}
